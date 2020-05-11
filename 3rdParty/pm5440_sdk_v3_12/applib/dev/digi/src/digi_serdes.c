/*******************************************************************************
*   COPYRIGHT (C) 2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
* --------------------------------------------------------------------------
*  This software embodies materials and concepts which are prdioprietary and
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
*   DESCRIPTION : 
*     This file contains C functions for configuring DIGI line, system,
*     SFI51, OTN OH RCP and FEGE management interfaces serdes.
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "digi.h"
#include "digi_loc.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
/* DIGI_GE_SERDES_INTF_INIT_L: GE serdes interafces default values */
#define DIGI_GE_SERDES_ID_INIT_L {DIGI_SERDES_INTF_OHP_0,       \
            DIGI_SERDES_INTF_OHP_1,                             \
            DIGI_SERDES_INTF_OHP_2,                             \
            DIGI_SERDES_INTF_EMGMT,                             \
            DIGI_SERDES_INTF_RI_RCP,                            \
            DIGI_SERDES_INTF_DI_RCP}




#if !defined(DIGI_SERDES_PATT_GEN_BUS_WIDTH_MAP_DEFAULT)
#define  DIGI_SERDES_PATT_GEN_BUS_WIDTH_MAP_DEFAULT {   \
        {DIGI_SERDES_PORT_MODE_T8_3G125, 10},           \
        {DIGI_SERDES_PORT_MODE_T8_6G25, 10},            \
        {DIGI_SERDES_PORT_MODE_C8_3G125, 10},           \
        {DIGI_SERDES_PORT_MODE_C8_6G25, 10},            \
            }
#endif



#ifndef DIGI_SERDES_PORT_MODE_IN_STRING
#define DIGI_SERDES_PORT_MODE_IN_STRING {"ANY_LOWPWR",  \
            "S16_100_GE",                               \
            "S16_40_GE",                                \
            "S16_10_GE",                                \
            "S16_DIC",                                \
            "S16_FC_1200",                              \
            "S16_FC_800",                               \
            "S16_10G_GDPS",                             \
            "S16_5G_GDPS",                              \
            "S16_CPRI_10G",                             \
            "S16_CPRI_6G",                              \
            "S16_CPRI_5G",                              \
            "S16_OC_768",                               \
            "S16_OC_192",                               \
            "S16_OTU4",                                 \
            "S16_OTU3",                                 \
            "S16_OTU3E2",                               \
            "S16_OTU3E1",                               \
            "S16_OTU2",                                 \
            "S16_OTU2E",                                \
            "S16_OTU1E",                                \
            "S16_OTU2F",                                \
            "S16_OTU1F",                                \
            "D8_OC_768",                                \
            "D8_OTU3",                                  \
            "D8_OTU3E2",                                \
            "D8_OTU3E1",                                \
            "D8_GE",                                    \
            "T8_3G125",                                 \
            "T8_6G25",                                  \
            "C8_3G125",                                 \
            "C8_6G25",                                  \
            "S16_3G125",                                \
            "S16_6G25",                                 \
            "S16_10G3125",                              \
            "S16_11G5",                                 \
            "S16_12G5",                                 \
            "LAST_DIGI_SERDES_PORT_MODE"                \
            };
#endif/* #ifndef DIGI_SERDES_PORT_MODE_IN_STRING */

/* Maximum number of characters per line in a serdes configuration file */
#define DIGI_SERDES_FILE_LINE_MAX_LENGTH     201

/*
** Local Macro Definitions
*/


/*number of T8 2x slices*/
#define DIGI_SERDES_MAX_T8_SLICE  (DIGI_SERDES_MAX_T8_E1_IN_BP_LR / T8_MAX_NUM_SERDES)
#define DIGI_SERDES_T8_SLICE_ID_GET(serdes_id)  \
    (serdes_id / T8_MAX_NUM_SERDES)
    

/*
** Local Structures and Unions
*/

/*
** Local Variables
*/

/*
** Log Messaging
*/

/*
** Forward References
*/
#ifndef DOXYGEN_PUBLIC_ONLY
PUBLIC PMC_ERROR digi_serdes_single_sfi51_cfg(digi_handle_t           *digi_handle, 
                                              digi_serdes_port_mode_t  serdes_port_mode,                  
                                              UINT8 inst,
                                              BOOL8 enable);
PRIVATE PMC_ERROR digi_serdes_internal_start_adapt_posted_status_get(digi_handle_t *digi_handle, 
                                                                     hostmsg_adapt_intf_type_t interface,
                                                                     UINT32                    lane,
                                                                     digi_start_adapt_serdes_status_t *start_adapt_serdes_status_ptr);
PRIVATE PMC_ERROR digi_serdes_internal_sfi51_posted_cfg(digi_handle_t *digi_handle, 
                                                        digi_serdes_port_mode_t  serdes_port_mode, 
                                                        UINT8 step,
                                                        UINT8 inst,
                                                        BOOL8 enable);
PRIVATE PMC_ERROR digi_serdes_internal_posted_step_start_adaptation(digi_handle_t            *digi_handle, 
                                                                    hostmsg_adapt_intf_type_t interface,
                                                                    UINT32                    lane,
                                                                    UINT8 step);
PRIVATE PMC_ERROR digi_serdes_internal_start_adaptation(digi_handle_t            *digi_handle, 
                                                        hostmsg_adapt_intf_type_t interface,
                                                        UINT32                    lane_0_19_msk,
                                                        UINT32                    lane_20_39_msk);
PRIVATE PMC_ERROR digi_serdes_internal_posted_start_adaptation(digi_handle_t            *digi_handle, 
                                                               hostmsg_adapt_intf_type_t interface,
                                                               UINT32                    lane);
PRIVATE PMC_ERROR digi_serdes_internal_sfi51_posted_status_get(digi_handle_t *digi_handle, 
                                                               UINT8 inst,
                                                               digi_sfi51_serdes_status_t *sfi51_serdes_status_ptr);
PRIVATE PMC_ERROR digi_serdes_internal_d8_posted_status_get(digi_handle_t *digi_handle, 
                                                            hostmsg_d8_intf_type_enum d8_intf_type,
                                                            digi_d8_serdes_status_t *d8_serdes_status_ptr);
PRIVATE PMC_ERROR digi_serdes_internal_d8_posted_cfg(digi_handle_t *digi_handle, 
                                                     hostmsg_d8_intf_type_enum d8_intf_type,
                                                     BOOL8 cfg_csu, 
                                                     UINT8 step,
                                                     BOOL8 enable);
PRIVATE PMC_ERROR digi_serdes_d8_multi_step_cfg(digi_handle_t *digi_handle, 
                                                hostmsg_d8_intf_type_enum d8_intf_type,
                                                BOOL8 cfg_csu, 
                                                BOOL8 enable);
PRIVATE PMC_ERROR digi_serdes_line_sys_patt_mon_internal_prov(digi_handle_t *digi_handle,
                                                              digi_serdes_port_t *port_ctxt_ptr,
                                                              UINT8 logical_lane,
                                                              util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr);
PRIVATE PMC_ERROR digi_serdes_line_sys_patt_mon_internal_deprov(digi_handle_t *digi_handle,
                                                                digi_serdes_port_t *port_ctxt_ptr,
                                                                UINT8 logical_lane);
PRIVATE PMC_ERROR digi_serdes_line_sys_patt_mon_err_count_internal_get(digi_handle_t *digi_handle,
                                                                       digi_serdes_port_t *port_ctxt_ptr,
                                                                       UINT8 logical_lane,
                                                                       util_patt_status_t    *status,
                                                                       UINT32 *err_cnt);
PRIVATE PMC_ERROR digi_serdes_line_sys_polarity_internal_get(digi_handle_t            *digi_handle,
                                                             digi_serdes_port_t       *port_ctxt_ptr,
                                                             UINT8                     logical_lane,
                                                             util_global_direction_t   dir,
                                                             BOOL8                    *polarity_inv);
PRIVATE PMC_ERROR digi_serdes_debug_status_internal_get(digi_handle_t *digi_handle,
                                                        serdes_debug_t *serdes_debug_ptr,
                                                        BOOL clear_val);
PRIVATE PMC_ERROR digi_serdes_port_lifd_xfi_disable(digi_handle_t *digi_handle,
                                                    UINT32 port_uid);

PRIVATE PMC_ERROR digi_serdes_port_sifd_xfi_disable(digi_handle_t *digi_handle,
                                                    UINT32 port_uid);

PRIVATE PMC_ERROR digi_serdes_port_lifd_sfi51_disable(digi_handle_t *digi_handle,
                                                      UINT32 port_uid);

PRIVATE PMC_ERROR digi_serdes_intf_to_hostmsg_d8_convert( digi_ge_serdes_id_t ge_intf,
                                                          hostmsg_d8_intf_type_enum *hostmsg_d8_intf);
PRIVATE PMC_ERROR digi_serdes_sfi51_info_get(digi_handle_t *digi_handle,
                                             UINT32 port_uid, 
                                             lifd_sfi5point1_int_t *sfi51_intf,
                                             BOOL *is_sfi_one_and_two_swapped,
                                             lifd_rxs_cfg_t *rxs_config);
PRIVATE PMC_ERROR digi_serdes_port_intf_type_get(digi_handle_t *digi_handle,
                                                 UINT32 port_uid,
                                                 digi_serdes_intf_t *intf_type);

PRIVATE UINT32 digi_sys_serdes_n1_optic_pin_use_get(digi_handle_t *digi_handle);
PRIVATE PMC_ERROR digi_ge_intf_serdes_cfg(digi_handle_t *digi_handle,
                                          digi_ge_serdes_id_t ge_intf,
                                          BOOL8 enable);

PRIVATE UINT32 digi_port_pin_bit_mask_s16_clk_get(digi_handle_t* digi_handle,
                                                  digi_serdes_port_def_t* port_def_ptr,
                                                  UINT16* bit_mask);

PRIVATE PMC_ERROR digi_bp_intf_serdes_cfg_helper(digi_handle_t *digi_handle,
                                                 digi_sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                                 digi_sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr,
                                                 digi_bp_intf_serdes_operation_t bp_intf_serdes_operation,
                                                 BOOL8 enable,
                                                 BOOL8 *enabled_ptr);
                                                 
PRIVATE PMC_ERROR digi_acb_capture_transmit_rate_calc(digi_handle_t *digi_handle,
                                                      digi_serdes_port_mode_t serdes_mode,
                                                      digi_mapping_mode_t map_mode,
                                                      digi_serdes_acb_mode_t acb_timing_mode,
                                                      DOUBLE *capture_rate, 
                                                      DOUBLE *transmit_rate, 
                                                      DOUBLE *server_rate,
                                                      BOOL8 *enet_mapping);       
PRIVATE void digi_serdes_s16_all_shutdown(digi_handle_t *digi_handle);
PRIVATE void digi_serdes_d8_all_shutdown(digi_handle_t *digi_handle);

PRIVATE PMC_ERROR digi_serdes_line_sys_patt_mon_cfg(digi_handle_t *digi_handle,
                                                    digi_serdes_port_t *port_ctxt_ptr,
                                                    UINT8 logical_lane,
                                                    BOOL8 is_resync,
                                                    util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr);

#ifdef PMC_DEPRECATED

PRIVATE PMC_ERROR digi_serdes_line_sys_patt_all_ones_detection_cfg(digi_handle_t *digi_handle,
                                                                   digi_serdes_port_t *port_ctxt_ptr,
                                                                   UINT8 logical_lane,
                                                                   util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr);

#endif /* PMC_DEPRECATED */

PRIVATE PMC_ERROR digi_serdes_line_sys_patt_gen_cfg(digi_handle_t *digi_handle,
                                                    digi_serdes_port_t *port_ctxt_ptr,
                                                    UINT8 logical_lane,
                                                    util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr);
PRIVATE PMC_ERROR digi_serdes_bp_patt_mon_cfg(digi_handle_t *digi_handle,
                                              digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg_ptr,
                                              digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg_ptr,
                                              BOOL8 is_resync,
                                              util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr); 
PRIVATE PMC_ERROR digi_bp_serdes_used_pin_id_get(digi_handle_t *digi_handle,
                                                 digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg_ptr,
                                                 digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg_ptr,
                                                 digi_serdes_type_in_bp_t *serdes_type,
                                                 UINT8 *used_pin_id,
                                                 digi_ilkn_serdes_rate_t *serdes_rate); 
PRIVATE PMC_ERROR digi_serdes_bp_patt_gen_cfg(digi_handle_t *digi_handle,
                                              digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg_ptr,
                                              digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg_ptr,
                                              util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr);


PRIVATE PMC_ERROR digi_serdes_ge_patt_mon_cfg(digi_handle_t *digi_handle,
                                              digi_ge_serdes_id_t ge_intf,
                                              BOOL8 is_resync,
                                              util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr);
PRIVATE PMC_ERROR digi_serdes_ge_patt_mon_cfg_get(digi_handle_t *digi_handle,
                                                  digi_ge_serdes_id_t ge_intf,
                                                  BOOL8 is_resync,
                                                  util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr);
PRIVATE PMC_ERROR digi_serdes_ge_patt_gen_cfg(digi_handle_t *digi_handle,
                                              digi_ge_serdes_id_t ge_intf,
                                              util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr);
PRIVATE PMC_ERROR digi_serdes_ge_patt_gen_cfg_get(digi_handle_t *digi_handle,
                                                  digi_ge_serdes_id_t ge_intf,
                                                  util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr);
PRIVATE PMC_ERROR digi_line_sys_serdes_pin_id_get(digi_handle_t *digi_handle,
                                                  digi_serdes_port_t *port_ctxt_ptr,
                                                  UINT8 logical_lane,
                                                  BOOL8 direction, 
                                                  UINT8 *pin); 
PRIVATE PMC_ERROR digi_serdes_sfi51_serdes_pin_id_get(digi_handle_t *digi_handle,
                                                      digi_serdes_port_t *port_ctxt_ptr,
                                                      UINT8 pin_id,
                                                      BOOL8 direction,
                                                      UINT8 *sfi51_id,
                                                      UINT8 *serdes_id);

PRIVATE BOOL digi_is_line_cage_id_unique(digi_handle_t *h,
                                         UINT8 cage_id);
PRIVATE BOOL digi_is_sys_cage_id_unique(digi_handle_t *h, UINT8 cage_id);

PRIVATE PMC_ERROR digi_sys_cage_validate(digi_handle_t *h,
                                         UINT8 cage_id_per_pin[DIGI_SERDES_SYS_LANES_MAX],
                                         BOOL *is_cfp);
PRIVATE PMC_ERROR digi_line_cage_validate(digi_handle_t *h,
                                          UINT8 cage_id_per_pin[DIGI_SERDES_XFI_LANES_MAX],
                                          BOOL *is_cfp);

PRIVATE digi_serdes_intf_t digi_serdes_interface_from_string_get(char *text);
PRIVATE digi_serdes_port_mode_t digi_serdes_port_mode_from_string_get(char *text);
PRIVATE void digi_serdes_field_get(char *line, 
                                   UINT32 start_offset,
                                   char *field_text);

PRIVATE PMC_ERROR digi_serdes_s16_write_adaptation_values(digi_handle_t *digi_handle, 
                                                          hostmsg_s16_intf_type_t s16_intf_type, 
                                                          UINT32 lane,
                                                          serdes_s16_adapt_objects_t *serdes_s16_adapt_objects_ptr);

PRIVATE PMC_ERROR digi_serdes_t8_write_adaptation_values(digi_handle_t *digi_handle, 
                                                         UINT32 lane,
                                                         serdes_t8_adapt_objects_t *serdes_t8_adapt_objects_ptr);

PRIVATE PMC_ERROR digi_serdes_c8_write_adaptation_values(digi_handle_t *digi_handle, 
                                                         UINT32 lane,
                                                         serdes_c8_adapt_objects_t *serdes_c8_adapt_objects_ptr);

PRIVATE BOOL8 digi_serdes_adapt_params_validate(digi_handle_t               *digi_handle, 
                                                digi_serdes_intf_t       intf,
                                                UINT32                   lane,
                                                digi_serdes_port_mode_t  port_mode);



#ifndef PMC_SW_SIMULATION 
PRIVATE PMC_ERROR digi_serdes_adaptation_values_read(digi_handle_t *digi_handle, 
                                                     hostmsg_adapt_intf_type_t interface,
                                                     UINT32 lane);


PRIVATE PMC_ERROR digi_serdes_s16_adapt_params_internal_get(digi_handle_t          *digi_handle, 
                                                            hostmsg_s16_intf_type_t      s16_intf_type,
                                                            UINT32                       lane,
                                                            serdes_s16_adapt_objects_t  *serdes_s16_adapt_objects_ptr);

PRIVATE PMC_ERROR digi_serdes_t8_adapt_params_internal_get(digi_handle_t               *digi_handle, 
                                                           UINT32                       lane,
                                                           serdes_t8_adapt_objects_t   *serdes_t8_adapt_objects_ptr);

PRIVATE PMC_ERROR digi_serdes_c8_adapt_params_internal_get(digi_handle_t               *digi_handle, 
                                                           UINT32                       lane,
                                                           serdes_c8_adapt_objects_t   *serdes_c8_adapt_objects_ptr);

PRIVATE PMC_ERROR digi_serdes_t8_read_adaptation_values(digi_handle_t *digi_handle,
                                                        UINT32 lane,
                                                        serdes_t8_adapt_objects_t *serdes_t8_adapt_objects_ptr);

PRIVATE PMC_ERROR digi_serdes_c8_read_adaptation_values(digi_handle_t *digi_handle,
                                                        UINT32 lane,
                                                        serdes_c8_adapt_objects_t *serdes_c8_adapt_objects_ptr);

PRIVATE PMC_ERROR digi_serdes_s16_read_adaptation_values(digi_handle_t *digi_handle,
                                                         hostmsg_s16_intf_type_t s16_intf_type, UINT32 lane,
                                                         serdes_s16_adapt_objects_t *serdes_s16_adapt_objects_ptr);

#endif /*#ifndef PMC_SW_SIMULATION */

PRIVATE PMC_ERROR digi_serdes_internal_adapt_status_get(digi_handle_t *digi_handle,
                                                        serdes_adapt_status_t *serdes_adapt_status_ptr);


PRIVATE PMC_ERROR digi_serdes_s16_tx_swing_emphasis_levels_in_ctxt_set(digi_handle_t       *digi_handle,
                                                                       digi_serdes_intf_t   s16_intf, 
                                                                       UINT32               lane, 
                                                                       s16_tx_swing_levels_t  *levels_cfg_ptr);

PRIVATE PMC_ERROR digi_serdes_t8_tx_swing_emphasis_levels_in_ctxt_set(digi_handle_t       *digi_handle,
                                                                      UINT32               lane, 
                                                                      t8_tx_swing_levels_t   *levels_cfg_ptr);

PRIVATE PMC_ERROR digi_serdes_c8_tx_swing_emphasis_levels_in_ctxt_set(digi_handle_t          *digi_handle,
                                                                      UINT32                  lane, 
                                                                      c8_tx_swing_levels_t   *levels_cfg_ptr);


PRIVATE void digi_serdes_c8_lanes_params_write(digi_handle_t   *digi_handl, 
                                               UINT32           lanes_msk,
                                               digi_serdes_port_mode_t serdes_port_mode);

PRIVATE void digi_serdes_s16_lanes_params_write(digi_handle_t   *digi_handl, 
                                                hostmsg_s16_intf_type_t s16_intf_type,
                                                UINT32           enable_msk,
                                                UINT32           disable_msk,
                                                digi_serdes_port_mode_t serdes_rate);

PRIVATE void digi_serdes_t8_lanes_params_write(digi_handle_t   *digi_handl, 
                                               UINT32           lane_0_19_enable_msk,
                                               UINT32           lane_20_39_enable_msk,
                                               digi_serdes_port_mode_t lane_0_19_port_mode, 
                                               digi_serdes_port_mode_t lane_20_39_port_mode);

PRIVATE PMC_ERROR digi_serdes_s16_dc_coupling_in_ctxt_set(digi_handle_t       *digi_handle, 
                                                          digi_serdes_intf_t   s16_intf_type,
                                                          UINT32               lanes_msk,
                                                          BOOL8                dc_coupling_enabled);

PRIVATE PMC_ERROR digi_serdes_t8_dc_coupling_in_ctxt_set(digi_handle_t   *digi_handle, 
                                                         UINT32           lane_0_19_msk,
                                                         UINT32           lane_20_39_msk,
                                                         BOOL8            dc_coupling_enabled);

PRIVATE PMC_ERROR digi_serdes_s16_polarity_in_ctxt_set(digi_handle_t            *digi_handle,
                                                       digi_serdes_intf_t        s16_intf, 
                                                       UINT32                    lanes_msk,
                                                       util_global_direction_t   dir,
                                                       BOOL8                     polarity_inversed);

PRIVATE PMC_ERROR digi_serdes_t8_polarity_in_ctxt_set(digi_handle_t             *digi_handle,
                                                      UINT32                     lane_0_19_msk,
                                                      UINT32                     lane_20_39_msk,
                                                      util_global_direction_t    dir,
                                                      BOOL8                      polarity_inversed);

PRIVATE PMC_ERROR digi_serdes_c8_polarity_in_ctxt_set(digi_handle_t             *digi_handle,
                                                      UINT32                     lanes_msk,
                                                      util_global_direction_t    dir,
                                                      BOOL8                      polarity_inversed);

PRIVATE PMC_ERROR digi_serdes_sfi51_polarity_in_ctxt_set(digi_handle_t           *digi_handle,
                                                         UINT32                   bus_id,      
                                                         UINT32                   serdes_id,
                                                         util_global_direction_t  dir,
                                                         BOOL8                    polarity_inversed);

PRIVATE PMC_ERROR digi_serdes_d8_tx_dclk_validate_params_and_instance_get(digi_handle_t* digi_handle,
                                                                          digi_serdes_port_t* port_def_ptr,
                                                                          UINT8* index);
PRIVATE UINT32 digi_min_post_mux_lane_find(digi_handle_t* digi_handle,
                                           digi_serdes_port_def_t* port_def_ptr,
                                           UINT32 * min);
PUBLIC void digi_debug_dsi_print(digi_handle_t* digi_handle, const char *config_name);

PRIVATE PMC_ERROR digi_serdes_adaptation_values_reset(digi_handle_t            *digi_handle,
                                                      hostmsg_adapt_intf_type_t interface,
                                                      UINT32                    lane_0_19_msk,
                                                      UINT32                    lane_20_39_msk);
PRIVATE PMC_ERROR digi_serdes_adaptation_reset_check(digi_handle_t            *digi_handle,
                                                     hostmsg_adapt_intf_type_t interface,
                                                     UINT32                    lane_0_19_msk,
                                                     UINT32                    lane_20_39_msk);

PRIVATE void digi_serdes_s16_adaptation_default_values_get(serdes_s16_adapt_objects_t  *serdes_s16_adapt_objects_ptr);

PRIVATE void digi_serdes_t8_adaptation_default_values_get(serdes_t8_adapt_objects_t  *serdes_t8_adapt_objects_ptr);

PRIVATE void digi_serdes_c8_adaptation_default_values_get(serdes_c8_adapt_objects_t  *serdes_c8_adapt_objects_ptr);

PRIVATE PMC_ERROR digi_serdes_xfi_pin_ctxt_audit(digi_handle_t                  *digi_handle,
                                                 util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                                 digi_serdes_intf_t              intf_type);

PRIVATE PMC_ERROR digi_serdes_sfi51_bus_ctxt_audit(digi_handle_t                  *digi_handle,
                                                   util_global_restart_init_cfg_t *restart_init_cfg_ptr);

PRIVATE PMC_ERROR digi_serdes_bp_ctxt_audit(digi_handle_t                  *digi_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr);

PRIVATE PMC_ERROR digi_serdes_ge_ctxt_audit(digi_handle_t                  *digi_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr);

PRIVATE BOOL8 digi_serdes_is_pending_state_get(digi_serdes_port_rate_prov_state_t     rate_prov_state);

PRIVATE digi_serdes_port_rate_prov_state_t digi_serdes_pending_state_transition(digi_serdes_port_rate_prov_state_t  rate_prov_state);

                              
PRIVATE BOOL8 digi_serdes_is_serdes_connect_to_enet_pmon(digi_handle_t                  *digi_handle,
                                                         digi_serdes_intf_t              intf_type,
                                                         digi_xfi_pin_ctxt_t            *xfi_pin_ctxt_ptr,
                                                         UINT32                          xfi_pin_port_uid);
PUBLIC PMC_ERROR digi_serdes_single_s16_cfg(digi_handle_t *digi_handle, 
                                             hostmsg_s16_intf_type_t s16_intf_type, 
                                             UINT8 serdes_id,
                                             digi_serdes_port_mode_t serdes_rate, 
                                             BOOL8 enable);
PRIVATE PMC_ERROR digi_serdes_internal_single_s16_cfg(digi_handle_t *digi_handle, 
                                                      hostmsg_s16_intf_type_t s16_intf_type, 
                                                      UINT8 serdes_id,
                                                      digi_serdes_port_mode_t serdes_rate, 
                                                      UINT8 step,
                                                      BOOL8 enable);
PRIVATE PMC_ERROR digi_serdes_internal_single_s16_status_get(digi_handle_t *digi_handle, 
                                                             hostmsg_s16_intf_type_t s16_intf_type, 
                                                             UINT8 serdes_id,
                                                             digi_s16_serdes_status_t *s16_serdes_status_ptr);

PRIVATE PMC_ERROR digi_serdes_ilkn_rx_reset_toggle(digi_handle_t   *digi_handle,    
                                                   UINT32           t8_19_00_msk,
                                                   UINT32           t8_39_20_msk,
                                                   UINT32           s16_sys_msk,
                                                   UINT32           c8_msk);

PRIVATE PMC_ERROR digi_serdes_non_otn_acb_cfg_get(digi_handle_t *digi_handle,
                                                  digi_serdes_intf_t intf,
                                                  util_global_switch_data_t *input_pc_ptr, 
                                                  util_global_switch_data_t *output_pc_ptr,
                                                  digi_serdes_acb_mode_t acb_timing_mode,
                                                  UINT32 num_10g_data_lane,
                                                  digi_enet_tx_timing_mode_t force_enet_timing_mode,
                                                  acb_ft_prov_cfg_t *acb_ft_prov_cfg_ptr,
                                                  BOOL8 *enet_mapping);

PUBLIC PMC_ERROR digi_serdes_otn_acb_cfg_get(digi_handle_t *digi_handle,
                                             digi_serdes_port_mode_t serdes_mode,
                                             coreotn_mpmo_inst_t mpmo_instance,
                                             UINT32 chnl,
                                             UINT32 num_10g_data_lane,
                                             acb_ft_prov_cfg_t *acb_ft_prov_cfg_ptr,
                                             BOOL8 *enet_mapping);
#endif /* DOXYGEN_PUBLIC_ONLY */
/*
** Public Functions
*/

/*******************************************************************************
* digi_debug_dsi_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing digi handle dsi allocation fields.
*
* INPUTS:
*   digi_handle         - pointer to DIGI handle instance.
*   config_name         - a string to include in the print out    
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_debug_dsi_print(digi_handle_t* digi_handle, const char *config_name)
{
#ifndef LOGGING_DISABLED
    UINT8 i;
    UINT8 k;

    char  bus_name[][7] = {"bus 0","bus 1"};
    char  prov_state_name[][11] = {"NO_SERDES", "UNASSIGNED", "LINEOTN",
                                   "ENET_LINE", "CBRC", "ENET_SYS", "LAST"};

    char  map_type_name[][16] = {"UNUSED", "ENET_LINE", "W/O_ENET_LINE",
                                 "WITH_ENET_LINE", "AND_ENET_LINE", "LAST"};
#endif
    PMC_ENTRY();
#ifndef LOGGING_DISABLED
    
    PMC_LOG_NOTRACE("\n---------------------- %s Start ----------------------\n",config_name);
    PMC_LOG_NOTRACE("\n=========line_port_ctxt=========|=========================xfi_line_pin_ctxt========================\n");
    PMC_LOG_NOTRACE("lane   init    port_uid   master  post mux lane    post mux port    cage id    tx_index    rx_index\n");

    for(i=0;i<12;i++) {

        PMC_LOG_NOTRACE("%4d   %4d    %8d     %4d  %13d    %13d    %7d    %8d    %8d\n",
                        i,
                        digi_handle->var.port_ctxt.line_port_ctxt[i].init,
                        digi_handle->var.port_ctxt.line_port_ctxt[i].port_uid,
                        digi_handle->var.port_ctxt.line_port_ctxt[i].master_lane,
                        digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].post_mux_dsi_lane,
                        digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].port_uid,
                        digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].cage_id,
                        digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].module_lane_tx_index,
                        digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].module_lane_rx_index);
    }

    PMC_LOG_NOTRACE("\npost mld lane    =======post_mld_dsi_line_ctxt==========\n");                     
    PMC_LOG_NOTRACE("index            line post mld port    cage uid    prov state    map state    num ODU0s\n");

    for(i=0;i<12;i++) {

        PMC_LOG_NOTRACE("%13d    %18d    %8d    %10s    %9s    %9d\n",
                        i,
                        digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].port_uid,
                        digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].cage_uid,
                        prov_state_name[digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].prov_state],
                        map_type_name[digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].map_type],
                        digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].num_odu0);
    }

    PMC_LOG_NOTRACE("\n==========sys_port_ctxt=========|==========================xfi_sys_pin_ctxt========================\n");
    PMC_LOG_NOTRACE("lane   init    port_uid   master  post mux lane    post mux port    cage id    tx_index    rx_index\n");

    for(i=0;i<12;i++) {

        PMC_LOG_NOTRACE("%4d   %4d    %8d     %4d  %13d    %13d    %7d    %8d    %8d\n",
                        i,
                        digi_handle->var.port_ctxt.sys_port_ctxt[i].init,
                        digi_handle->var.port_ctxt.sys_port_ctxt[i].port_uid,
                        digi_handle->var.port_ctxt.sys_port_ctxt[i].master_lane,
                        digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].post_mux_dsi_lane,
                        digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].port_uid,
                        digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].cage_id,
                        digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].module_lane_tx_index,
                        digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].module_lane_rx_index);

    }

    PMC_LOG_NOTRACE("\npost mld lane    =======post_mld_dsi_line_ctxt==========\n");                     
    PMC_LOG_NOTRACE("index            line post mld port    cage uid    prov state    map state    num ODU0s\n");

    for(i=0;i<12;i++) {

        PMC_LOG_NOTRACE("%13d    %18d    %8d    %10s    %9s    %9d\n",
                        i,
                        digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[i].port_uid,
                        digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[i].cage_uid,
                        prov_state_name[digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[i].prov_state],
                        map_type_name[digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[i].map_type],
                        digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[i].num_odu0);
    }

    PMC_LOG_NOTRACE("\n============== sfi5.1 config ==============\n");
    PMC_LOG_NOTRACE("cfg bus    init    allocated bus    sfi 5.1 post mld port\n");
    /* find the dsi lane on xfi that corresponds to sfi5.1 port id */
    for(i=0;i<2;i++)
    {
        for(k=0;k<12;k++)
            if(digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].port_uid ==
               digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[k].port_uid)
                break;

        if (digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].post_mux_2G5_bus_allocation <= DIGI_POST_MUX_BUS_1)
        {
            PMC_LOG_NOTRACE("%7d    %4d    %13s    %21d\n",
                            i,
                            digi_handle->var.port_ctxt.sfi51_port_ctxt[i].init,
                            bus_name[digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].post_mux_2G5_bus_allocation],
                            (k<12?digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[k].port_uid:DIGI_SERDES_UNASSIGNED));
        }
    }

    PMC_LOG_NOTRACE("\n---------------------- %s  end ----------------------\n",config_name);
#endif
    PMC_RETURN();

} /* digi_debug_dsi_print */


/*******************************************************************************
* digi_serdes_port_init_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function returns the provioning status of one serdes.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   *port_ctxt_ptr         - serdes port context pointer 
*
* OUTPUTS:
*  *init_status            - TRUE when the serdes port is initialized.
*                            FALSE when serdes port is uninitialized.             
*
* RETURNS:
*   PMC_SUCCESS            - when API execution is successful otherwise a 
*                            descriptive error is returned. 
*
* NOTES:
*  None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_port_init_status_get(digi_handle_t      *digi_handle,
                                                  digi_serdes_port_t *port_ctxt_ptr,
                                                  BOOL8              *init_status)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT (NULL != init_status, DIGI_ERR_INVALID_ARG, 0, 0);

    /* verify that port ctxt ptr belongs to digi handle pool */
    DIGI_SERDES_PORT_CTXT_HANDLE_CHECK(digi_handle, port_ctxt_ptr, rc);

    *init_status = ((digi_serdes_port_def_t *)port_ctxt_ptr)->init;

    PMC_ATOMIC_RETURN(digi_handle, rc);

}/* digi_serdes_port_init_status_get */


/*******************************************************************************
* digi_serdes_port_rate_prov_status_get
* ______________________________________________________________________________
*y

* DESCRIPTION:
*
*   The function returns the provioning status of one serdes.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   *port_ctxt_ptr         - serdes port context pointer 
*
* OUTPUTS:
*  *prov_status            - TRUE when the serdes is provisionned 
*                            FALSE when serdes is not provionned 
*  *port_mode              - serdes port rate, value is valid only 
*                             if *prov_status is TRUE  
*  *prov_state             - serdes port current state                      
*
* RETURNS:
*   PMC_SUCCESS            - when API execution is successful otherwise a 
*                            descriptive error is returned. 
*
* NOTES:
*  The function uses digi_by_port_uid_post_mld_prov_state_get to extract the state 
*   of the serdes 
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_port_rate_prov_status_get(digi_handle_t *digi_handle,
                                                       digi_serdes_port_t *port_ctxt_ptr,
                                                       BOOL8 *prov_status,
                                                       digi_serdes_port_mode_t *port_mode,
                                                       digi_serdes_prov_state_t *prov_state)
{    
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* verify that port ctxt ptr belongs to digi handle pool */
    DIGI_SERDES_PORT_CTXT_HANDLE_CHECK(digi_handle, port_ctxt_ptr, result);

    result = digi_serdes_port_rate_prov_status_internal_get(digi_handle,
                                                            port_ctxt_ptr,
                                                            TRUE,
                                                            prov_status,
                                                            port_mode,
                                                            prov_state);
    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_port_rate_prov_status_get*/


#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* digi_serdes_port_rate_prov_status_internal_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   see digi_serdes_port_rate_prov_status_ge description
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   *port_ctxt_ptr         - serdes port context pointer 
*    post_mld_check        - When TRUE, digi_by_port_uid_post_mld_prov_state_get 
*                            to extract serdes provisioning state.
*                            When FALSE, only serdes context content is used.
*
* OUTPUTS:
*  *prov_status            - TRUE when the serdes is provisionned 
*                            FALSE when serdes is not provionned 
*  *port_mode              - serdes port rate, value is valid  
*                            when *prov_status is TRUE  
*  *prov_state             - serdes port current state                      
*
* RETURNS:
*   PMC_SUCCESS            - when API execution is successful otherwise a 
*                            descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_port_rate_prov_status_internal_get(digi_handle_t     *digi_handle,
                                                                digi_serdes_port_t *port_ctxt_ptr,
                                                                BOOL8               post_mld_check,
                                                                BOOL8              *prov_status,
                                                                digi_serdes_port_mode_t *port_mode,
                                                                digi_serdes_prov_state_t *prov_state)
{    
    digi_serdes_port_def_t *port_def_ptr = NULL;
    UINT32 i = 0;
    BOOL8 found = FALSE;
    UINT32 port_uid;
    PMC_ERROR result = PMC_SUCCESS;
    digi_serdes_port_rate_prov_state_t prov_port_state;
    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(prov_status != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_mode != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(prov_state != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* by default, let's consider that serdes port is not provisinned */
    *prov_status = FALSE;     

    /* check that serdes type is associated with a cage */
    /* and get port_uidto retrieve serdes prov state and get port_mode */
    port_def_ptr = (digi_serdes_port_def_t *)port_ctxt_ptr;
    switch (port_def_ptr->intf)
    {
    case DIGI_SERDES_LINE_INTF:    
        while (TRUE != found && i < DIGI_SERDES_XFI_LANES_MAX)
        {        
            if (digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].port_uid == port_def_ptr->port_uid)
            {
                port_uid = digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].port_uid;
                *port_mode = digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].serdes_rate_state.port_mode;
                prov_port_state = digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].serdes_rate_state.rate_prov_state;
                *prov_state = digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].serdes_rate_state.port_prov_state;
                found = TRUE;
            }
            i++;
        }
        break;
    case DIGI_SERDES_SYSTEM_INTF:
        while (TRUE != found && i < DIGI_SERDES_SYS_LANES_MAX)
        {        
            if (digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].port_uid == port_def_ptr->port_uid)
            {
                port_uid = digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].port_uid;
                *port_mode = digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].serdes_rate_state.port_mode; 
                prov_port_state = digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].serdes_rate_state.rate_prov_state;
                *prov_state = digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].serdes_rate_state.port_prov_state;
                found = TRUE;
            }
            i++;
        }
        break;
    case DIGI_SERDES_SFI_51_INTF:
        while (TRUE != found && i < DIGI_NUM_SFI51_PORTS_MAX)
        {
            if (digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].port_uid == port_def_ptr->port_uid)
            {
                port_uid = digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].port_uid;
                *port_mode = digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].serdes_rate_state.port_mode;
                prov_port_state = digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].serdes_rate_state.rate_prov_state;
                *prov_state = digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].serdes_rate_state.port_prov_state;
                found = TRUE;
            }
            i++;
        }
        break;
    default:
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
        break;
    }

    /* if port_uid is not retrieved then return error code */
    if (FALSE == found)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);    
    }
    if ( DIGI_SERDES_PORT_RATE_STATE_PROV != prov_port_state &&
         DIGI_SERDES_PORT_RATE_STATE_PROV_FACILITY_LPBK != prov_port_state &&
         DIGI_SERDES_PORT_RATE_STATE_PROV_DIAG_LPBK != prov_port_state)
    {
        if (TRUE == post_mld_check)
        {
            /* check that serdes is provisioned in a datapath */
            result = digi_by_port_uid_post_mld_prov_state_get(digi_handle, port_uid, prov_state);
            
            if (PMC_SUCCESS == result)
            {
                /* if serdes is provisioned, get serdes rate */
                if (LAST_DIGI_SERDES_PROV_STATE == *prov_state)
                {
                    PMC_RETURN(DIGI_ERR_UNKNOWN);
                }
                if ( DIGI_SERDES_PROV_STATE_NO_SERDES != *prov_state &&
                     DIGI_SERDES_PROV_STATE_UNASSIGNED != *prov_state) 
                {
                    *prov_status = TRUE;
                }
            }
        }
    }
    else
    {
        *prov_status = TRUE;
    }

    PMC_RETURN(result);
}/*digi_serdes_port_rate_prov_status_internal_get*/
#endif /* DOXYGEN */

/*******************************************************************************
* digi_serdes_port_rate_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function provisions and configures the rate of one given serdes port of
*   Line, Sys or SFI51 interface.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *port_ctxt_ptr         - serdes port context pointer
*   port_mode              - serdes port rate
*
* OUTPUTS:
*   NONE.                         
*
* RETURNS:
*   PMC_SUCCESS            - when API execution is successful otherwise 
*                            a descriptive error is returned. 
*
* NOTES:
*  The function should be called only for Line/Sys S16 + SFI5.1 D8 Serdes 
*
*******************************************************************************/     
PUBLIC PMC_ERROR digi_serdes_port_rate_prov(digi_handle_t *digi_handle,
                                            digi_serdes_port_t *port_ctxt_ptr,
                                            digi_serdes_port_mode_t port_mode)
{
    BOOL8 prov_status;
    digi_serdes_prov_state_t prov_state;
    digi_serdes_port_def_t *port_def_ptr; 
    UINT16 en_mask =0;
    UINT16 dis_mask = 0;
    UINT32 i;
    digi_serdes_port_mode_t tmp_port_mode;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    if (digi_device_init_status_get(digi_handle) == FALSE) 
    {
        result = DIGI_ERR_DEVICE_NOT_INIT;
    }

    if (PMC_SUCCESS == result)
    {    
        /* verify that port ctxt ptr belongs to digi handle pool */
        DIGI_SERDES_PORT_CTXT_HANDLE_CHECK(digi_handle, port_ctxt_ptr, result);
        
        /* cast serdes port context */
        port_def_ptr = (digi_serdes_port_def_t *)port_ctxt_ptr;
        
        /* get serdes provioning state */
        result = digi_serdes_port_rate_prov_status_internal_get(digi_handle, port_ctxt_ptr,
                                                                TRUE, 
                                                                &prov_status, &tmp_port_mode, &prov_state);
    }

    /* if serdes is not already provisioned */
    if (PMC_SUCCESS == result)
    {
        if (FALSE == prov_status)
        {
            /* Provision serdes rate */        
            switch (port_def_ptr->intf)
            {
            case DIGI_SERDES_LINE_INTF:
                /* build enable mask */
                for (i = 0; i < DIGI_SERDES_XFI_LANES_MAX; i++)
                {                          
                    if ( digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].port_uid == port_def_ptr->port_uid)
                    {
                        en_mask |= (1 << i); 
                    }
                }
                /* configure S16 serdes */
                /* Check S16 port mode */
                result = digi_serdes_s16_cfg(digi_handle, HOSTMSG_S16_SERDES_INTF_LINE, en_mask, port_mode, dis_mask);
                break;
            case DIGI_SERDES_SYSTEM_INTF:
                /* build enable mask */
                for (i = 0; i < DIGI_SERDES_SYS_LANES_MAX; i++)
                {
                    if (digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].port_uid == port_def_ptr->port_uid)
                    {
                        en_mask |= (1 << i); 
                    }
                }
                /* configure S16 serdes */
                result = digi_serdes_s16_cfg(digi_handle, HOSTMSG_S16_SERDES_INTF_SYS, en_mask, port_mode, dis_mask);
                break;
            case DIGI_SERDES_SFI_51_INTF:
                /* build enable mask */
                for (i = 0; i < DIGI_NUM_SFI51_PORTS_MAX; i++)
                {
                    if (digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].port_uid == port_def_ptr->port_uid)
                    {
                        en_mask |= (1 << i); 
                    }
                }
                /* configure SFI51 D8 serdes */
                result = digi_serdes_sfi51_cfg(digi_handle, en_mask, port_mode, dis_mask);
              
                break;
            default:
                PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                break;
            }

            if (PMC_SUCCESS == result)
            {
                result = digi_serdes_port_rate_state_set(digi_handle, port_ctxt_ptr, 
                                                         port_mode, prov_state, 
                                                         DIGI_SERDES_PORT_RATE_STATE_PROV);
            }
        }
        else
        {
            result = DIGI_ERR_SERDES_PORT_ALREADY_PROV;
        }

    }
    
    
    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_port_rate_prov*/

/*******************************************************************************
* digi_serdes_port_rate_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function deprovisions and frees any resource of one given serdes port of
*   Line, Sys or SFI51 interface.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *port_ctxt_ptr         - serdes port context pointer
*
* OUTPUTS:
*   NONE.                         
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
*
* NOTES:
*  The function should be called only for Line/Sys S16 + SFI5.1 D8 Serdes 
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_port_rate_deprov(digi_handle_t *digi_handle,
                                              digi_serdes_port_t *port_ctxt_ptr)
{
    UINT16 en_mask = 0;
    UINT16 dis_mask = 0;
    digi_serdes_port_def_t *port_def_ptr; 
    UINT32 i;
    digi_serdes_port_mode_t port_mode = LAST_DIGI_SERDES_PORT_MODE;
    BOOL8 prov_status;
    digi_serdes_prov_state_t prov_state;
    digi_serdes_port_mode_t tmp_port_mode;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* verify that port ctxt ptr belongs to digi handle pool */
    DIGI_SERDES_PORT_CTXT_HANDLE_CHECK(digi_handle, port_ctxt_ptr, result);

    /* cast serdes port context */
    port_def_ptr = (digi_serdes_port_def_t *)port_ctxt_ptr;


    /* get serdes provioning state */
    result = digi_serdes_port_rate_prov_status_internal_get(digi_handle, port_ctxt_ptr,
                                                            TRUE, 
                                                            &prov_status, &tmp_port_mode, &prov_state);

    if (PMC_SUCCESS != result)
    {
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_SERDES_CAGE_STILL_PROV);
    }
     
    if (DIGI_SERDES_PROV_STATE_NO_SERDES != prov_state &&
        DIGI_SERDES_PROV_STATE_UNASSIGNED != prov_state)
    {
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_SERDES_PORT_NOT_PROV);
    }

    if (TRUE != prov_status)
    {
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_SERDES_PORT_NOT_PROV);        
    }

    /* Deprovision serdes rate */        
    switch (port_def_ptr->intf)
    {
    case DIGI_SERDES_LINE_INTF:
        /* build enable mask */
        for (i = 0; i < DIGI_SERDES_XFI_LANES_MAX; i++)
        {
            if ( digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].port_uid == port_def_ptr->port_uid && 
                 (DIGI_SERDES_PROV_STATE_UNASSIGNED == digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].serdes_rate_state.port_prov_state || 
                  DIGI_SERDES_PROV_STATE_NO_SERDES == digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].serdes_rate_state.port_prov_state))
            {
                dis_mask |= (1 << i);     
                port_mode = digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].serdes_rate_state.port_mode;        
            }
        }
        if (dis_mask != 0)
        {
            /* disable S16 serdes */
            result = digi_serdes_s16_cfg(digi_handle, HOSTMSG_S16_SERDES_INTF_LINE, en_mask, port_mode, dis_mask);
        }
        else
        {
            result = DIGI_ERR_SERDES_PORT_ALREADY_PROV;
        }
        break;
    case DIGI_SERDES_SYSTEM_INTF:
        /* build enable mask */
        for (i = 0; i < DIGI_SERDES_SYS_LANES_MAX; i++)
        {
            if (digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].port_uid == port_def_ptr->port_uid && 
                (DIGI_SERDES_PROV_STATE_UNASSIGNED == digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].serdes_rate_state.port_prov_state || 
                 DIGI_SERDES_PROV_STATE_NO_SERDES == digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].serdes_rate_state.port_prov_state))
            {
                dis_mask |= (1 << i);     
                port_mode = digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].serdes_rate_state.port_mode;
            }
        }
        if (dis_mask != 0)
        {
            /* disable S16 serdes */
            result = digi_serdes_s16_cfg(digi_handle, HOSTMSG_S16_SERDES_INTF_SYS, en_mask, port_mode, dis_mask);
        }
        else
        {
            result = DIGI_ERR_SERDES_PORT_ALREADY_PROV;
        }
        break;
    case DIGI_SERDES_SFI_51_INTF:
        /* build enable mask */
        for (i = 0; i < DIGI_NUM_SFI51_PORTS_MAX; i++)
        {
            if (digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].port_uid == port_def_ptr->port_uid &&
                (DIGI_SERDES_PROV_STATE_UNASSIGNED == digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].serdes_rate_state.port_prov_state || 
                 DIGI_SERDES_PROV_STATE_NO_SERDES == digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].serdes_rate_state.port_prov_state))
            {
                dis_mask |= (1 << i); 
                port_mode = digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].serdes_rate_state.port_mode;
            }
        }
        if (dis_mask != 0)
        {
            /* disable SFI51 D8 serdes */
            result = digi_serdes_sfi51_cfg(digi_handle, en_mask, port_mode, dis_mask);
        }
        else
        {
            result = DIGI_ERR_SERDES_SFI51_BUS_ALREADY_PROV;
        }
        break;
    default:
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
        break;
    }

    if (PMC_SUCCESS == result)
    {
        result = digi_serdes_port_rate_state_set(digi_handle, 
                                                 port_ctxt_ptr, port_mode, 
                                                 DIGI_SERDES_PROV_STATE_UNASSIGNED,
                                                 DIGI_SERDES_PORT_RATE_STATE_DEPROV);
    }


    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_port_rate_deprov*/

/*******************************************************************************
* digi_serdes_line_sys_patt_mon_resync
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function triggers start of synchronization of line, system  
*   or SFI51 interface serdes pattern monitor.
*   Function digi_serdes_line_sys_patt_mon_prov shall be called prior to 
*   this function.
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   *port_ctxt_ptr      - serdes port context pointer.
*   logical_lane        - For line and system interface, it is the lane
*                         index associated to the serdes module.
*                         For SFI51 interface, it is the serdes pin index.  
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_line_sys_patt_mon_resync(digi_handle_t *digi_handle,
                                                      digi_serdes_port_t *port_ctxt_ptr,
                                                      UINT8 logical_lane)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* verify that port ctxt ptr belongs to digi handle pool */
    DIGI_SERDES_PORT_CTXT_HANDLE_CHECK(digi_handle, port_ctxt_ptr, result);
    
    /* Configure serdes pattern monitor */
    result = digi_serdes_line_sys_patt_mon_cfg(digi_handle, port_ctxt_ptr, logical_lane, TRUE,  NULL);

    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_line_sys_patt_mon_resync*/


/*******************************************************************************
* digi_serdes_line_sys_patt_mon_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function configures the pattern monitor of one line, system  
*   or SFI51 interface serdes. Function digi_serdes_line_sys_patt_mon_resync
*   shall be called to resynchronize patter monitor when pattern reception started.
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance
*   *port_ctxt_ptr      - serdes port context pointer
*   logical_lane        - For line and system interface,
*                         it is the lane index associated to the serdes module.
*                         For SFI51 interface, it is the device pin index. 
*   *patt_mon_cfg_ptr   - pointer to a pattern monitor configuration
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_line_sys_patt_mon_prov(digi_handle_t *digi_handle,
                                                    digi_serdes_port_t *port_ctxt_ptr,
                                                    UINT8 logical_lane,
                                                    util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* verify that port ctxt ptr belongs to digi handle pool */
    DIGI_SERDES_PORT_CTXT_HANDLE_CHECK(digi_handle, port_ctxt_ptr, result);

    result = digi_serdes_line_sys_patt_mon_internal_prov(digi_handle,port_ctxt_ptr,logical_lane,patt_mon_cfg_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_line_sys_patt_mon_prov*/
    
/*******************************************************************************
* digi_serdes_line_sys_patt_mon_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function stops the pattern monitor of one line, system or SFI51 
*   interface serdes. 
*
*
*
* INPUTS:
*   *digi_handle               - pointer to DIGI handle instance.
*   *port_ctxt_ptr             - serdes port context pointer.
*   logical_lane               - For line and system interface,
*                                it is the logical lane associated to the serdes.
*                                For SFI51 interface, it is the serdes pin number. 
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_line_sys_patt_mon_deprov(digi_handle_t *digi_handle,
                                                      digi_serdes_port_t *port_ctxt_ptr,
                                                      UINT8 logical_lane)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* verify that port ctxt ptr belongs to digi handle pool */
    DIGI_SERDES_PORT_CTXT_HANDLE_CHECK(digi_handle, port_ctxt_ptr, result);

    result = digi_serdes_line_sys_patt_mon_internal_deprov(digi_handle,
                                                           port_ctxt_ptr,
                                                           logical_lane);

    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_line_sys_patt_mon_deprov*/
   

/*******************************************************************************
* digi_serdes_line_sys_patt_mon_err_count_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function returns pattern monitor status and errors counter value of
*   line, system or SFI51 interface serdes.
*
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   *port_ctxt_ptr        - serdes port context pointer.
*   logical_lane          - For line and system interface, it is the lane
*                           index associated to the serdes module.
*                           For SFI51 interface, it is the device pin index.  
*
* OUTPUTS:
*   *status               - Pattern monitor status: Locked, trying to lock, etc.                  
*   *err_cnt              - Number of errors detected by pattern monitor. 
*                           The value shall be ignored when pattern monitor
*                           is not in locked state.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_line_sys_patt_mon_err_count_get(digi_handle_t *digi_handle,
                                                             digi_serdes_port_t *port_ctxt_ptr,
                                                             UINT8 logical_lane,
                                                             util_patt_status_t    *status,
                                                             UINT32 *err_cnt)
{    

    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* verify that port ctxt ptr belongs to digi handle pool */
    DIGI_SERDES_PORT_CTXT_HANDLE_CHECK(digi_handle, port_ctxt_ptr, result);

    result =  digi_serdes_line_sys_patt_mon_err_count_internal_get(digi_handle,
                                                                   port_ctxt_ptr,
                                                                   logical_lane,
                                                                   status,
                                                                   err_cnt);
    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_patt_mon_get_err_count*/


/*******************************************************************************
* digi_serdes_line_sys_patt_polarity_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function returns serdes polarity associated to given logical lane.
*
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   *port_ctxt_ptr        - serdes port context pointer. 
*   dir                   - serdes logical lane direction: RX or TX. 
*                           UTIL_GLOBAL_DIRECTION_BOTH is not a valid value, 
*                           and an error will be returned in such case.
*   logical_lane          - For line and system interface, it is the lane
*                           index associated to the serdes module.
*                           For SFI51 interface, it is the device pin index. 
*
* OUTPUTS:
*   *polarity_inv         - TRUE when polarity is inverted, otherwise FALSE.
*
* RETURNS:
*   PMC_SUCCESS otherwise a descriptive error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_line_sys_polarity_get(digi_handle_t          *digi_handle,
                                                   digi_serdes_port_t     *port_ctxt_ptr,
                                                   UINT8                   logical_lane,
                                                   util_global_direction_t dir,
                                                   BOOL8                  *polarity_inv)
{    
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* verify that port ctxt ptr belongs to digi handle pool */
    DIGI_SERDES_PORT_CTXT_HANDLE_CHECK(digi_handle, port_ctxt_ptr, result);

    result = digi_serdes_line_sys_polarity_internal_get(digi_handle,port_ctxt_ptr,logical_lane, dir, polarity_inv);

    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_line_sys_polarity_get*/
    

/*******************************************************************************
* digi_serdes_line_sys_patt_gen_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function configures the pattern generator of line, system 
*   or SFI51 interface serdes.
*
*
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   *port_ctxt_ptr        - serdes port context pointer.
*   logical_lane          - For line and system interface,
*                           it is the logical lane associated to the serdes.
*                           For SFI51 interface, it is the serdes pin number. 
*   *patt_gen_cfg_ptr     - pointer to a pattern generator configuration.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
*
* NOTES:
*  Note pattern generator error insertion is not configured by the function
*  Function digi_serdes_patt_gen_err_insert_cfg should be called to enable
*  error injection.
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_line_sys_patt_gen_prov(digi_handle_t *digi_handle,
                                                    digi_serdes_port_t *port_ctxt_ptr,
                                                    UINT8 logical_lane,
                                                    util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(patt_gen_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* verify that port ctxt ptr belongs to digi handle pool */
    DIGI_SERDES_PORT_CTXT_HANDLE_CHECK(digi_handle, port_ctxt_ptr, result);

    /* enable pattern generator */
    patt_gen_cfg_ptr->patt_cfg.enable = TRUE;

    /* Configure serdes pattern generator */
    result = digi_serdes_line_sys_patt_gen_cfg(digi_handle, port_ctxt_ptr, 
                                               logical_lane, patt_gen_cfg_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_line_sys_patt_gen_prov*/
    
/*******************************************************************************
* digi_serdes_line_sys_patt_gen_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function stops pattern generator of line, system or SFI51 interface
*   serdes.
*
*
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   *port_ctxt_ptr        - serdes port context pointer.
*   logical_lane          - For line and system interface,
*                           it is the logical lane associated to the serdes.
*                           For SFI51 interface, it is the serdes pin number. 
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_line_sys_patt_gen_deprov(digi_handle_t *digi_handle,
                                                      digi_serdes_port_t *port_ctxt_ptr,
                                                      UINT8 logical_lane)
{
    util_serdes_patt_gen_cfg_t patt_gen_cfg;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    /* verify that port ctxt ptr belongs to digi handle pool */
    DIGI_SERDES_PORT_CTXT_HANDLE_CHECK(digi_handle, port_ctxt_ptr, result);

    /* disable pattern generator */
    patt_gen_cfg.patt_cfg.enable = FALSE;

    /* Configure serdes pattern generator */
    result = digi_serdes_line_sys_patt_gen_cfg(digi_handle, port_ctxt_ptr, logical_lane, &patt_gen_cfg);

    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_patt_gen_deprov*/

/*******************************************************************************
* digi_serdes_line_sys_patt_gen_err_insert_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function triggers one error bit insertion in pattern generated by  
*   one line, system or SFI51 interface serdes.
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   *port_ctxt_ptr        - serdes port context pointer.
*   logical_lane          - For line and system interface,
*                           it is the logical lane associated to the serdes.
*                           For SFI51 interface, it is the serdes pin number. 
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_line_sys_patt_gen_err_insert_cfg(digi_handle_t *digi_handle,
                                                              digi_serdes_port_t *port_ctxt_ptr,
                                                              UINT8 logical_lane) 
{    
    digi_serdes_port_def_t *port_def_ptr; 
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 pin;
    UINT8 sfi51_id;
    UINT8 serdes_id;
    BOOL8 prov_status;
    digi_serdes_prov_state_t prov_state;
    digi_serdes_port_mode_t port_mode;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* verify that port ctxt ptr belongs to digi handle pool */
    DIGI_SERDES_PORT_CTXT_HANDLE_CHECK(digi_handle, port_ctxt_ptr, result);

    result = digi_line_sys_serdes_pin_id_get(digi_handle, port_ctxt_ptr, logical_lane, FALSE, &pin);
    if (PMC_SUCCESS == result)
    {
    
        /* get serdes provioning state */
        result = digi_serdes_port_rate_prov_status_internal_get(digi_handle, 
                                                                port_ctxt_ptr, 
                                                                TRUE,
                                                                &prov_status, &port_mode, &prov_state);
        if(PMC_SUCCESS != result)
        {
            PMC_ATOMIC_RETURN(digi_handle,result);
        }

        if(FALSE == prov_status)
        {
            result = DIGI_ERR_SERDES_PORT_NOT_PROV;
        }

        /* cast serdes port context */
        port_def_ptr = (digi_serdes_port_def_t *)port_ctxt_ptr;    

        /* configure appropriate serdes according to interface type */    
        switch (port_def_ptr->intf)
        {
        case DIGI_SERDES_LINE_INTF:
            s16_patt_gen_err_insert_cfg(digi_handle->s16_line_handle, pin);
            break;
        case DIGI_SERDES_SYSTEM_INTF:
            s16_patt_gen_err_insert_cfg(digi_handle->s16_sys_handle, pin);
            break;
        case DIGI_SERDES_SFI_51_INTF:
            result = digi_serdes_sfi51_serdes_pin_id_get(digi_handle, port_ctxt_ptr, 
                                                         pin, FALSE, &sfi51_id, &serdes_id); 
            if (PMC_SUCCESS == result)
            {
                sfi51_d8_tx_18x_patt_gen_err_insert_cfg(digi_handle->sfi51_d8_tx_18x_handle[sfi51_id], 
                                                        serdes_id);
            }
            break;
        default:
            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
            break;
        }    
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

}/*digi_serdes_patt_gen_err_insert_cfg*/


/*******************************************************************************
* digi_serdes_bp_patt_mon_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function configures a serdes pattern monitor of one T8 or C8 or S16 serdes 
*   in the backplane interface.
*   After this function call, digi_serdes_bp_patt_mon_resync shall be called to 
*   resynchronize patter monitor when pattern reception started.
*
*
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance
*   *sifd_1_lane_cfg_ptr  - SIFD interlaken 1 lanes configuration pointer
*   *sifd_2_lane_cfg_ptr  - SIFD interlaken 2 lanes configuration pointer
*   *patt_mon_cfg_ptr     - pointer to a pattern monitor configuration
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_bp_patt_mon_prov(digi_handle_t *digi_handle,
                                              digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg_ptr,
                                              digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg_ptr,
                                              util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(sifd_1_lane_cfg_ptr != NULL || sifd_2_lane_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(patt_mon_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* enable pattern monitor */
    patt_mon_cfg_ptr->patt_cfg.enable = TRUE;
    
    /* Configure serdes pattern monitor */
    result = digi_serdes_bp_patt_mon_cfg(digi_handle, sifd_1_lane_cfg_ptr, 
                                         sifd_2_lane_cfg_ptr, FALSE, patt_mon_cfg_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_bp_patt_mon_prov*/

/*******************************************************************************
* digi_serdes_bp_patt_mon_resync
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function triggers start of synchronization of T8 or C8 or S16 serdes
*   pattern monitor in the backplane interface. 
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance
*   *sifd_1_lane_cfg_ptr  - SIFD interlaken 1 lanes configuration pointer
*   *sifd_2_lane_cfg_ptr  - SIFD interlaken 2 lanes configuration pointer
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_bp_patt_mon_resync(digi_handle_t *digi_handle,
                                                digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg_ptr,
                                                digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(sifd_1_lane_cfg_ptr != NULL || sifd_2_lane_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    /* Configure serdes pattern monitor */
    result = digi_serdes_bp_patt_mon_cfg(digi_handle, sifd_1_lane_cfg_ptr, 
                                         sifd_2_lane_cfg_ptr, TRUE, NULL);

    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_bp_patt_mon_resync*/

/*******************************************************************************
* digi_serdes_bp_patt_mon_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function stops a serdes pattern monitor of one T8 or C8 or S16 serdes 
*   in the backplane interface.
*
*
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   *sifd_1_lane_cfg_ptr  - SIFD interlaken 1 lanes configuration pointer.
*   *sifd_2_lane_cfg_ptr  - SIFD interlaken 2 lanes configuration pointer.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_bp_patt_mon_deprov(digi_handle_t *digi_handle,
                                                digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg_ptr,
                                                digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg_ptr)
{
    util_serdes_patt_mon_cfg_t patt_mon_cfg;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0); 
    PMC_ASSERT(sifd_1_lane_cfg_ptr != NULL || sifd_2_lane_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
   
    /* disable pattern monitor */
    patt_mon_cfg.patt_cfg.enable = FALSE;

    /* configure serdes pattern monitor */
    result = digi_serdes_bp_patt_mon_cfg(digi_handle, sifd_1_lane_cfg_ptr, 
                                         sifd_2_lane_cfg_ptr, FALSE, &patt_mon_cfg);


    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_bp_patt_mon_deprov*/

/*******************************************************************************
* digi_serdes_bp_patt_mon_err_count_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function retuns pattern monitor status and the number of errors detected by 
*    of one T8 or C8 or S16 serdes in the backplane interface.
*
*
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance
*   *sifd_1_lane_cfg_ptr  - SIFD interlaken 1 lanes configuration pointer
*   *sifd_2_lane_cfg_ptr  - SIFD interlaken 2 lanes configuration pointer
*
* OUTPUTS:
*   *status               - Pattern monitor status: Locked, trying to lock, etc.                  
*   *err_cnt              - Number of errors detected by pattern monitor. 
*                           The value shall be ignored when pattern monitor
*                           is not in locked state.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_bp_patt_mon_err_count_get(digi_handle_t                 *digi_handle,
                                                       digi_sifd_ilkn_1_lane_cfg_t   *sifd_1_lane_cfg_ptr,
                                                       digi_sifd_ilkn_2_lane_cfg_t   *sifd_2_lane_cfg_ptr,
                                                       util_patt_status_t            *status,
                                                       UINT32                        *err_cnt)
{
    UINT8 pin_id;
    UINT8 serdes_id;
    digi_serdes_type_in_bp_t serdes_type;
    digi_ilkn_serdes_rate_t serdes_rate_dummy;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(sifd_1_lane_cfg_ptr != NULL || sifd_2_lane_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(err_cnt != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    result = digi_bp_serdes_used_pin_id_get(digi_handle, sifd_1_lane_cfg_ptr, sifd_2_lane_cfg_ptr, &serdes_type, &pin_id, &serdes_rate_dummy);

    if (PMC_SUCCESS == result)
    {
        switch(serdes_type)
        {
        case DIGI_SERDES_TYPE_IN_BP_T8:
            serdes_id = digi_serdes_map_t8_serdes(pin_id);
            result = t8_patt_mon_err_count_get(digi_handle->bp_lr_t8_handle[DIGI_SERDES_T8_SLICE_ID_GET(pin_id)], 
                                               serdes_id, status, err_cnt);
            break;
        case DIGI_SERDES_TYPE_IN_BP_C8:
            
            result = c8_patt_mon_err_count_get(digi_handle->bp_sr_c8_handle, 
                                               pin_id, status, err_cnt);
            break;
        case DIGI_SERDES_TYPE_IN_BP_S16:
            s16_patt_mon_err_count_get(digi_handle->s16_sys_handle, pin_id, status, err_cnt);
            break;
        default:
            result = DIGI_ERR_ILKN_CHNL_NOT_PROV;
            break;
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_bp_patt_mon_err_count_get*/


/*******************************************************************************
* digi_serdes_bp_patt_gen_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function configures and enables a pattern generator of one T8 or C8
*   or S16 serdes in the backplane interface.
*
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance
*   *sifd_1_lane_cfg_ptr  - SIFD interlaken 1 lanes configuration pointer
*   *sifd_2_lane_cfg_ptr  - SIFD interlaken 2 lanes configuration pointer
*   *patt_gen_cfg_ptr     - pointer to a pattern generator configuration
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_bp_patt_gen_prov(digi_handle_t                 *digi_handle,
                                              digi_sifd_ilkn_1_lane_cfg_t   *sifd_1_lane_cfg_ptr,
                                              digi_sifd_ilkn_2_lane_cfg_t   *sifd_2_lane_cfg_ptr,
                                              util_serdes_patt_gen_cfg_t    *patt_gen_cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);    
    PMC_ASSERT(sifd_1_lane_cfg_ptr != NULL || sifd_2_lane_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(patt_gen_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* enable pattern generator */
    patt_gen_cfg_ptr->patt_cfg.enable = TRUE;
    
    /* Configure serdes pattern generator */
    result = digi_serdes_bp_patt_gen_cfg(digi_handle, sifd_1_lane_cfg_ptr, 
                                         sifd_2_lane_cfg_ptr, patt_gen_cfg_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_bp_patt_gen_prov*/

/*******************************************************************************
* digi_serdes_bp_patt_gen_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function stops pattern generator of one T8 or C8 or S16 serdes 
*   in the backplane interface.
*
*
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   *sifd_1_lane_cfg_ptr  - SIFD interlaken 1 lanes configuration pointer.
*   *sifd_2_lane_cfg_ptr  - SIFD interlaken 2 lanes configuration pointer.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS    - when API execution is successful otherwise a descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_bp_patt_gen_deprov(digi_handle_t               *digi_handle,
                                                digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg_ptr,
                                                digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg_ptr)
{
    util_serdes_patt_gen_cfg_t patt_gen_cfg;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);    
    PMC_ASSERT(sifd_1_lane_cfg_ptr != NULL || sifd_2_lane_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
   
    /* disable pattern generator */
    patt_gen_cfg.patt_cfg.enable = FALSE;

    /* Configure serdes pattern generator */
    result = digi_serdes_bp_patt_gen_cfg(digi_handle, sifd_1_lane_cfg_ptr, sifd_2_lane_cfg_ptr, &patt_gen_cfg);


    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_bp_patt_gen_deprov*/


/*******************************************************************************
* digi_serdes_bp_patt_gen_err_insert_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function triggers one error bit insertion in pattern generated by one
*   T8 or C8 or S16 serdes in the backplane interface.
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   *sifd_1_lane_cfg_ptr  - pointer to SIFD 1 lanes configuration.
*   *sifd_2_lane_cfg_ptr  - pointer to SIFD 2 lanes configuration.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_bp_patt_gen_err_insert_cfg(digi_handle_t                 *digi_handle,
                                                        digi_sifd_ilkn_1_lane_cfg_t   *sifd_1_lane_cfg_ptr,
                                                        digi_sifd_ilkn_2_lane_cfg_t   *sifd_2_lane_cfg_ptr) 
{

    UINT8 pin_id;
    UINT8 serdes_id;
    digi_serdes_type_in_bp_t serdes_type;
    digi_ilkn_serdes_rate_t serdes_rate_dummy;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);     
    PMC_ASSERT(sifd_1_lane_cfg_ptr != NULL || sifd_2_lane_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    result = digi_bp_serdes_used_pin_id_get(digi_handle, sifd_1_lane_cfg_ptr, sifd_2_lane_cfg_ptr, &serdes_type, &pin_id, &serdes_rate_dummy);

    if (PMC_SUCCESS == result)
    {
        switch(serdes_type)
        {
        case DIGI_SERDES_TYPE_IN_BP_T8:
            serdes_id = digi_serdes_map_t8_serdes(pin_id);
            result = t8_patt_gen_err_insert_cfg(digi_handle->bp_lr_t8_handle[DIGI_SERDES_T8_SLICE_ID_GET(pin_id)], 
                                                serdes_id);
            break;
        case DIGI_SERDES_TYPE_IN_BP_C8:

            result = c8_patt_gen_err_insert_cfg(digi_handle->bp_sr_c8_handle, 
                                                pin_id);
            break;
        case DIGI_SERDES_TYPE_IN_BP_S16:
            result = s16_patt_gen_err_insert_cfg(digi_handle->s16_sys_handle, pin_id);
            break;
        default:
            result = DIGI_ERR_ILKN_CHNL_NOT_PROV;
            break;
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_bp_patt_gen_err_insert_cfg*/






/*******************************************************************************
* digi_serdes_ge_patt_mon_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function configures the pattern monitor of one GE serdes: OHP, RCP, FEGE.
*   After this function call, digi_serdes_ge_patt_mon_resync shall be called to 
*   resynchronize patter monitor when pattern reception started.
*
* INPUTS:
*   *digi_handle               - pointer to DIGI handle instance
*   ge_intf                    - GE serdes interface type.
*   *patt_mon_cfg_ptr          - pointer to a pattern monitor configuration
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_ge_patt_mon_prov(digi_handle_t *digi_handle,
                                              digi_ge_serdes_id_t ge_intf,
                                              util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* enable pattern monitor */
    patt_mon_cfg_ptr->patt_cfg.enable = TRUE;
    
    /* Configure serdes pattern monitor */
    result = digi_serdes_ge_patt_mon_cfg(digi_handle, ge_intf, FALSE, patt_mon_cfg_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_ge_patt_mon_prov*/

/*******************************************************************************
* digi_serdes_ge_patt_mon_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function configures the pattern monitor of one GE serdes: OHP, RCP, FEGE.
*   After this function call, digi_serdes_ge_patt_mon_resync shall be called to 
*   resynchronize patter monitor when pattern reception started.
*
* INPUTS:
*   *digi_handle               - pointer to DIGI handle instance
*   ge_intf                    - GE serdes interface type.
*
* OUTPUTS:
*   *patt_mon_cfg_ptr          - pointer to a pattern monitor configuration
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_ge_patt_mon_get(digi_handle_t *digi_handle,
                                             digi_ge_serdes_id_t ge_intf,
                                             util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* Configure serdes pattern monitor */
    result = digi_serdes_ge_patt_mon_cfg_get(digi_handle, ge_intf, FALSE, patt_mon_cfg_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_ge_patt_mon_get*/

/*******************************************************************************
* digi_serdes_ge_patt_mon_resync
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function triggers start of synchronization of GE serdes: OHP, RCP, FEGE,
*   pattern monitor.
*   Function digi_serdes_ge_patt_mon_prov shall be called prior to 
*   this function.
*
* INPUTS:
*   *digi_handle               - pointer to DIGI handle instance.
*   ge_intf                    - GE serdes interface type.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_ge_patt_mon_resync(digi_handle_t *digi_handle,
                                                digi_ge_serdes_id_t ge_intf)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    

    /* Configure serdes pattern monitor */
    result = digi_serdes_ge_patt_mon_cfg(digi_handle, ge_intf, TRUE, NULL);

    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_ge_patt_mon_resync*/


/*******************************************************************************
* digi_serdes_ge_patt_mon_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function stops the pattern monitor of one GE serdes: OHP, RCP, FEGE.
*
* INPUTS:
*   *digi_handle               - pointer to DIGI handle instance.
*   ge_intf                    - GE serdes interface type.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_ge_patt_mon_deprov(digi_handle_t *digi_handle,
                                                digi_ge_serdes_id_t ge_intf)
{
    util_serdes_patt_mon_cfg_t patt_mon_cfg;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    /* disable pattern monitor */
    patt_mon_cfg.patt_cfg.enable = FALSE;

    /* Configure serdes pattern monitor */
    result = digi_serdes_ge_patt_mon_cfg(digi_handle, ge_intf, FALSE, &patt_mon_cfg);

    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_ge_patt_mon_deprov*/
    
/*******************************************************************************
* digi_serdes_patt_ge_mon_get_err_count
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function returns serdes pattern monitor status and errors counter value 
*   of one GE serdes: OHP, RCP, FEGE.
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   ge_intf               - GE serdes interface type.
*
* OUTPUTS:
*   *status               - Pattern monitor status: Locked, trying to lock, etc.                  
*   *err_cnt              - Number of errors detected by pattern monitor. 
*                           The value shall be ignored when pattern monitor
*                           is not in locked state.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_ge_patt_mon_err_count_get(digi_handle_t         *digi_handle,
                                                       digi_ge_serdes_id_t    ge_intf,
                                                       util_patt_status_t    *status,
                                                       UINT32                *err_cnt)
{

    d8_rx_2x_handle_t *d8_rx_2x_handle;
    UINT8 serdes_id;
    digi_ge_serdes_ctxt_t *ge_ctxt;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    result = digi_ge_serdes_ctxt_get(digi_handle, ge_intf, &ge_ctxt);
    
    if (PMC_SUCCESS != result)
    {
        PMC_ATOMIC_RETURN(digi_handle,result);
    }

    /* check serdes state */
    if (FALSE == ge_ctxt->enabled)
    {
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_SERDES_PORT_NOT_PROV);
    } 
    else if (FALSE == ge_ctxt->prbs_mon_enabled)
    {
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_PRBS_NOT_PROV );
    }

    /* retrieve d8_rx_2x handle associated to ge serdes */
    result = digi_ge_d8_rx_2x_handle_get(digi_handle, ge_intf, &d8_rx_2x_handle, &serdes_id);
    
    if(PMC_SUCCESS != result)
    {
        PMC_ATOMIC_RETURN(digi_handle,result);
    }

    result =  d8_rx_2x_patt_mon_err_count_get(d8_rx_2x_handle,
                                              serdes_id,
                                              status,
                                              err_cnt);


    PMC_ATOMIC_RETURN(digi_handle,result);

}/*digi_serdes_patt_mon_get_err_count*/
    

/*******************************************************************************
* digi_serdes_ge_patt_gen_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function configures the pattern generator of one GE serdes: OHP, RCP, FEGE.
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*    ge_intf              - GE serdes interface type.
*   *patt_gen_cfg_ptr     - pointer to a pattern generator configuration.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
*
* NOTES:
*  Note pattern generator error insertion is not configured by the function
*  Function digi_serdes_patt_gen_err_insert_cfg should be called to enable
*  error injection.
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_ge_patt_gen_prov(digi_handle_t *digi_handle,
                                              digi_ge_serdes_id_t ge_intf,
                                              util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* enable pattern generator */
    patt_gen_cfg_ptr->patt_cfg.enable = TRUE;

    /* Configure serdes pattern generator */
    result = digi_serdes_ge_patt_gen_cfg(digi_handle, ge_intf, patt_gen_cfg_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_ge_patt_gen_prov*/

/*******************************************************************************
* digi_serdes_ge_patt_gen_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function gets configurations of the pattern generator of one GE serdes: OHP, RCP, FEGE.
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*    ge_intf              - GE serdes interface type.
*
* OUTPUTS:
*   *patt_gen_cfg_ptr     - pointer to a pattern generator configuration.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
*
* NOTES:
*  Note pattern generator error insertion is not configured by the function
*  Function digi_serdes_patt_gen_err_insert_cfg should be called to enable
*  error injection.
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_ge_patt_gen_get(digi_handle_t *digi_handle,
                                             digi_ge_serdes_id_t ge_intf,
                                             util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* Configure serdes pattern generator */
    result = digi_serdes_ge_patt_gen_cfg_get(digi_handle, ge_intf, patt_gen_cfg_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_ge_patt_gen_get*/

/*******************************************************************************
* digi_serdes_ge_patt_gen_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function stops pattern generator of one GE serdes: OHP, RCP, FEGE.
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   ge_intf               - GE serdes interface type.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_ge_patt_gen_deprov(digi_handle_t *digi_handle,
                                                digi_ge_serdes_id_t ge_intf)
{
    util_serdes_patt_gen_cfg_t patt_gen_cfg;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    
    /* disable pattern generator */
    patt_gen_cfg.patt_cfg.enable = FALSE;

    /* Configure serdes pattern generator */
    result = digi_serdes_ge_patt_gen_cfg(digi_handle, ge_intf, &patt_gen_cfg);

    PMC_ATOMIC_RETURN(digi_handle,result);
}/*digi_serdes_patt_gen_deprov*/

/*******************************************************************************
* digi_serdes_ge_patt_gen_err_insert_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function triggers one error bit insertion in pattern generated by one 
*   one GE serdes: OHP, RCP, FEGE.
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   ge_intf               - GE serdes interface type.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_ge_patt_gen_err_insert_cfg(digi_handle_t *digi_handle,
                                                        digi_ge_serdes_id_t ge_intf) 
{
    d8_tx_2x_handle_t *d8_tx_2x_handle;
    UINT8 serdes_id;
    digi_ge_serdes_ctxt_t *ge_ctxt;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    result = digi_ge_serdes_ctxt_get(digi_handle, ge_intf, &ge_ctxt);
    
    if (PMC_SUCCESS != result)
    {
        PMC_ATOMIC_RETURN(digi_handle,result);
    }

    /* check serdes state */
    if (FALSE == ge_ctxt->enabled)
    {
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_SERDES_PORT_NOT_PROV);
    } 
    else if (FALSE == ge_ctxt->prbs_gen_enabled)
    {
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_PRBS_NOT_PROV);
    }    

    /* retrieve d8_tx_2x handle associated to ge serdes */
    result = digi_ge_d8_tx_2x_handle_get(digi_handle, ge_intf, &d8_tx_2x_handle, &serdes_id);
    
    if(PMC_SUCCESS != result)
    {
        PMC_ATOMIC_RETURN(digi_handle,result);
    }

    result =  d8_tx_2x_patt_gen_err_insert_cfg(d8_tx_2x_handle,
                                               serdes_id);


    PMC_ATOMIC_RETURN(digi_handle,result);

}/*digi_serdes_patt_gen_err_insert_cfg*/

/*******************************************************************************
* digi_serdes_port_polarity_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function enables or disables lane polarity inversion for one or multiple
*   serdes ports associated to a serdes port:
*       -Line interface (S16)
*       -System Interface (S16)
*       -SFI 5.1 Interface (D8)
*   
*   This function alters the polarity of each lane associated with the specified port.
*
* INPUTS:
*   *digi_handle               - pointer to DIGI handle instance
*   *port_ctxt_ptr             - serdes port context pointer
*   logical_lanes_msk          - For line and system interface,
*                                it is the logical lane mask associated to serdes.
*                                For SFI51 interface, it is the serdes pins mask. 
*   dir                        - serdes lanes  direction: RX, TX, or both. 
*   enable_inv_polarity        - enable or disable the polarity inversion.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_port_polarity_cfg(digi_handle_t           *digi_handle,
                                               digi_serdes_port_t      *port_ctxt_ptr, 
                                               UINT32                   logical_lanes_msk,
                                               util_global_direction_t  dir,
                                               BOOL8                    enable_inv_polarity)
{
    digi_serdes_port_def_t *port_def_ptr;
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 pin_id;
    UINT8 sfi51_id;
    UINT8 serdes_id;
    UINT8 i;
    UINT32 logical_lane;
    BOOL8 one_serdes_cfg = FALSE;
    BOOL8 inv_rx;
    BOOL8 inv_tx;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* verify that port ctxt ptr belongs to digi handle pool */
    DIGI_SERDES_PORT_CTXT_HANDLE_CHECK(digi_handle, port_ctxt_ptr, rc);

    /* cast serdes port context */
    port_def_ptr =  (digi_serdes_port_def_t*) port_ctxt_ptr;

    for (i = 0; i < 32 && PMC_SUCCESS == rc; i++)
    {
        inv_rx = FALSE;
        inv_tx = FALSE;
        logical_lane = 1 << i;
        if ((logical_lane & logical_lanes_msk) == 0)
        {
            continue;
        }

        rc = digi_line_sys_serdes_pin_id_get(digi_handle, port_ctxt_ptr, i, FALSE, &pin_id);

        if (PMC_SUCCESS != rc)
        {
            rc = digi_line_sys_serdes_pin_id_get(digi_handle, port_ctxt_ptr, i, TRUE, &pin_id);
            
            if (PMC_SUCCESS != rc)
            {
                break;
            }
        }
        
        /* Configure appropriate serdes based on interface type */
        switch (port_def_ptr->intf)
        {
        case DIGI_SERDES_SYSTEM_INTF:
        {
            s16_polarity_cfg(digi_handle->s16_sys_handle, pin_id, dir, enable_inv_polarity);
            digi_serdes_s16_polarity_in_ctxt_set(digi_handle, DIGI_SERDES_SYSTEM_INTF, 1 << pin_id, dir, enable_inv_polarity);
            break;
        }
        case DIGI_SERDES_LINE_INTF:
        {
            s16_polarity_cfg(digi_handle->s16_line_handle, pin_id, dir, enable_inv_polarity);  
            digi_serdes_s16_polarity_in_ctxt_set(digi_handle, DIGI_SERDES_LINE_INTF, 1 << pin_id, dir, enable_inv_polarity);
            break;
        }
        case DIGI_SERDES_SFI_51_INTF:
        {
            switch (dir)
            {
            case UTIL_GLOBAL_DIR_TX:
                inv_tx = TRUE;
                break;
            case UTIL_GLOBAL_DIR_RX:
                inv_rx = TRUE;
                break;
            case UTIL_GLOBAL_DIR_BOTH:
                inv_rx = TRUE;
                inv_tx = TRUE;
                break;
            default:
                break;
            }
                             
            if (TRUE ==  inv_tx)
            {      
                rc = digi_serdes_sfi51_serdes_pin_id_get(digi_handle, port_ctxt_ptr,
                                                         pin_id, FALSE, &sfi51_id, &serdes_id);

                rc = sfi51_d8_tx_18x_polarity_cfg(digi_handle->sfi51_d8_tx_18x_handle[sfi51_id],
                                                  serdes_id, enable_inv_polarity);   

                /* save polarity in serdes configuration saved in context */
                digi_serdes_sfi51_polarity_in_ctxt_set(digi_handle, sfi51_id, serdes_id, UTIL_GLOBAL_DIR_TX, enable_inv_polarity);    
            }

            if (TRUE ==  inv_rx && PMC_SUCCESS == rc)
            {      
                rc = digi_serdes_sfi51_serdes_pin_id_get(digi_handle, port_ctxt_ptr,
                                                         pin_id, TRUE, &sfi51_id, &serdes_id);
                
                rc = sfi51_d8_rx_18x_polarity_cfg(digi_handle->sfi51_d8_rx_18x_handle[sfi51_id], 
                                                  serdes_id, enable_inv_polarity); 
                /* save polarity in serdes configuration saved in context */
                digi_serdes_sfi51_polarity_in_ctxt_set(digi_handle, sfi51_id, serdes_id, UTIL_GLOBAL_DIR_RX, enable_inv_polarity);           
            }
            break;
        }
        default:
            rc = DIGI_ERR_INVALID_ARG;
            break;
        }
        if (PMC_SUCCESS == rc)
        {
            one_serdes_cfg = TRUE;
        }
    }

    if (FALSE == one_serdes_cfg && PMC_SUCCESS == rc)
    {
        rc = DIGI_ERR_INVALID_ARG;
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_serdes_port_polarity_cfg */


/*******************************************************************************
* digi_serdes_ge_polarity_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function enables or disables lane polarity inversion on a GE management 
*   interface (D8).

* INPUTS:
*   *digi_handle               - pointer to DIGI handle instance
*   num_ge_serdes              - number of elements in ge_intf list
*   ge_intfs                   - list of GE serdes interface to configure.
*   dir                        - serdes lanes direction: RX or TX or both. 
*   enable_inv_polarity        - enable or disable the polarity inversion.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_ge_polarity_cfg(digi_handle_t           *digi_handle, 
                                             UINT8                    num_ge_serdes,
                                             digi_ge_serdes_id_t      ge_intfs[], 
                                             util_global_direction_t  dir,
                                             BOOL8                    enable_inv_polarity)
{
    d8_rx_2x_handle_t *d8_rx_2x_handle;
    d8_tx_2x_handle_t *d8_tx_2x_handle;
    UINT8 serdes_id;
    UINT8 i;
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL8 inv_rx;
    BOOL8 inv_tx;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    for (i = 0; i < num_ge_serdes && PMC_SUCCESS == rc; i++)
    {
        inv_rx = FALSE;
        inv_tx = FALSE;

        switch (dir)
        {
        case UTIL_GLOBAL_DIR_TX:
            inv_tx = TRUE;
            break;
        case UTIL_GLOBAL_DIR_RX:
            inv_rx = TRUE;
            break;
        case UTIL_GLOBAL_DIR_BOTH:
            inv_rx = TRUE;
            inv_tx = TRUE;
            break;
        default:
            break;
        }

        if (TRUE == inv_rx)
        {
            /* retrieve d8_rx_2x handle associated to ge serdes */
            rc = digi_ge_d8_rx_2x_handle_get(digi_handle, ge_intfs[i], &d8_rx_2x_handle, &serdes_id);
            if (PMC_SUCCESS == rc)
            {
                /* configure pin polarity */   
                d8_rx_2x_polarity_cfg(d8_rx_2x_handle, serdes_id, enable_inv_polarity);    
            }
        }

        if (TRUE == inv_tx && PMC_SUCCESS == rc)
        {
            rc = digi_ge_d8_tx_2x_handle_get(digi_handle, ge_intfs[i], &d8_tx_2x_handle, &serdes_id);  
                        
            if (PMC_SUCCESS == rc)
            {
                d8_tx_2x_polarity_cfg(d8_tx_2x_handle,serdes_id, enable_inv_polarity);    
            }
        }

        /* save polarity value in serdes conxtext */
        if (PMC_SUCCESS == rc)
        {
            digi_handle->var.serdes_ctxt.serdes_cfg->ge_cfg[ge_intfs[i]].polarity_inversed = util_global_direction_value_compute(digi_handle->var.serdes_ctxt.serdes_cfg->ge_cfg[ge_intfs[i]].polarity_inversed, dir, enable_inv_polarity);
        }
    }


    PMC_ATOMIC_RETURN(digi_handle, rc);    
} /* digi_serdes_ge_polarity_cfg */

/*******************************************************************************
* digi_serdes_bp_polarity_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The function configures pattern monitor of one or multiple serdes 
*  in backplane interface.
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   *sifd_1_lane_cfg_ptr  - SIFD interlaken 1 lanes configuration pointer.
*   *sifd_2_lane_cfg_ptr  - SIFD interlaken 2 lanes configuration pointer.
*    dir                  - serdes lanes direction: RX or TX or both. 
*    enable_inv_pol       - enable or disable the polarity inversion.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_bp_polarity_cfg(digi_handle_t                *digi_handle,
                                             digi_sifd_ilkn_1_lane_cfg_t  *sifd_1_lane_cfg_ptr,
                                             digi_sifd_ilkn_2_lane_cfg_t  *sifd_2_lane_cfg_ptr,
                                             util_global_direction_t       dir,
                                             BOOL8                         enable_inv_pol)
{
    PMC_ERROR result = PMC_SUCCESS;  
    UINT32                     e1_19_00_msk;
    UINT32                     e1_39_20_msk;
    UINT32                     n1_msk;
    UINT32                     n2_msk;
    UINT32 i;
    UINT32 mask;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0); 
    PMC_ASSERT(sifd_1_lane_cfg_ptr != NULL || sifd_2_lane_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    result = digi_bp_serdes_mask_get(digi_handle,
                                     sifd_1_lane_cfg_ptr,
                                     sifd_2_lane_cfg_ptr,
                                     &e1_19_00_msk,
                                     &e1_39_20_msk,
                                     &n1_msk,
                                     &n2_msk);
    
    if (PMC_SUCCESS == result)
    {
        for (i = 0; i < 32 && PMC_SUCCESS == result; i++)
        {
            mask = 1 << i;
            if ((mask & e1_19_00_msk) != 0)
            {
                result = digi_serdes_polarity_t8_cfg(digi_handle,
                                                     i, 
                                                     dir,
                                                     enable_inv_pol);
            }
            if (PMC_SUCCESS == result && (mask & e1_39_20_msk) != 0)
            {
                result = digi_serdes_polarity_t8_cfg(digi_handle,
                                                     i + (DIGI_SERDES_MAX_T8_SLICE), 
                                                     dir,
                                                     enable_inv_pol);
            }
        } 
        for (i = 0; i < 32 && PMC_SUCCESS == result; i++)
        {
            mask = 1 << i;
            if (PMC_SUCCESS == result && (mask & n1_msk) != 0)
            {
                result = digi_serdes_polarity_bp_s16_cfg(digi_handle, 
                                                         i, 
                                                         dir, 
                                                         enable_inv_pol);
            }
        }
        for (i = 0; i < 32 && PMC_SUCCESS == result; i++)
        {
            mask = 1 << i;
            if (PMC_SUCCESS == result && (mask & n2_msk) != 0)
            {
                result = digi_serdes_polarity_c8_cfg(digi_handle, 
                                                     i, 
                                                     dir, 
                                                     enable_inv_pol);
            }
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
}/* digi_serdes_bp_polarity_cfg*/

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* digi_serdes_ilkn_rx_reset_toggle
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The function toggles ILKN RX block associated to given serdes.
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   t8_19_00_msk          - T8 serdes lanes 19 to 0 mask.
*   t8_39_20_msk          - T8 serdes lanes 39 to 20 mask.
*   s16_sys_msk           - S16 SYS serdes lanes 15 to 0 mask.
*   c8_msk                - C8 serdes lanes 7 to 0 mask.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_ilkn_rx_reset_toggle(digi_handle_t             *digi_handle,    
                                                   UINT32                     t8_19_00_msk,
                                                   UINT32                     t8_39_20_msk,
                                                   UINT32                     s16_sys_msk,
                                                   UINT32                     c8_msk)
{
    PMC_ERROR result = PMC_SUCCESS;  
    digi_ilkn_serdes_rate_t ilkn_1_rate = LAST_DIGI_ILKN_SERDES_RATE;
    digi_ilkn_serdes_rate_t ilkn_2_rate = LAST_DIGI_ILKN_SERDES_RATE;
    UINT32 ilkn1_e1_39_20_msk = 0;
    UINT32 ilkn1_e1_19_00_msk = 0;
    UINT32 ilkn1_n1_msk = 0;     
    UINT32 ilkn1_n2_msk = 0;
    UINT32 ilkn2_e1_39_20_msk = 0;
    UINT32 ilkn2_e1_19_00_msk = 0;
    UINT32 ilkn2_n1_msk = 0;     
    UINT32 ilkn2_n2_msk = 0;  
    sifd_ilkn_1_cfg_t *loc_sifd_1_cfg_ptr = NULL;
    sifd_ilkn_2_cfg_t *loc_sifd_2_cfg_ptr = NULL;
    sifd_ilkn_1_cfg_t loc_sifd_1_cfg;
    sifd_ilkn_2_cfg_t loc_sifd_2_cfg;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* determine which RX ILKN blocks should be reset */
    if (TRUE == digi_handle->var.is_ilkn_enabled)
    {
        /* retrieve ILKN1 and ILKN2, E1, N1 and N2 lanes mask */
        digi_ilkn_serdes_pin_use_get(digi_handle,
                                     &digi_handle->var.sifd_ilkn_1_cfg,
                                     &digi_handle->var.sifd_ilkn_2_cfg,
                                     &ilkn_1_rate,
                                     &ilkn_2_rate,
                                     &ilkn1_e1_39_20_msk,
                                     &ilkn1_e1_19_00_msk,
                                     &ilkn1_n1_msk,
                                     &ilkn1_n2_msk,
                                     &ilkn2_e1_39_20_msk,
                                     &ilkn2_e1_19_00_msk,
                                     &ilkn2_n1_msk,
                                     &ilkn2_n2_msk);

        /* determined if serdes are connected to ILKN1 */
        if (0 != (t8_19_00_msk & ilkn1_e1_19_00_msk) ||
            0 != (t8_39_20_msk & ilkn1_e1_39_20_msk) ||
            0 != (s16_sys_msk & ilkn1_n1_msk) ||
            0 != (c8_msk & ilkn1_n2_msk))
        {
            result = digi_copy_struct_ilkn_1_cfg(&loc_sifd_1_cfg, &digi_handle->var.sifd_ilkn_1_cfg);
            loc_sifd_1_cfg_ptr = &loc_sifd_1_cfg;
        }
        /* determined if serdes are connected to ILKN2 */
        if (PMC_SUCCESS == result &&
            (0 != (t8_19_00_msk & ilkn2_e1_19_00_msk) ||
            0 != (t8_39_20_msk & ilkn2_e1_39_20_msk) ||
            0 != (s16_sys_msk & ilkn2_n1_msk) ||
             0 != (c8_msk & ilkn2_n2_msk)))
        {
            result = digi_copy_struct_ilkn_2_cfg(&loc_sifd_2_cfg, &digi_handle->var.sifd_ilkn_2_cfg);
            loc_sifd_2_cfg_ptr = &loc_sifd_2_cfg;
        }
    }
        
    /* toggle sifd ilkn 1 and/or 2 rx reset associated to serdes */
    if (NULL != loc_sifd_1_cfg_ptr ||
        NULL != loc_sifd_2_cfg_ptr)
    {
        result = sifd_ilkn_rx_reset_toggle(digi_handle->sifd_handle,
                                           loc_sifd_1_cfg_ptr, 
                                           loc_sifd_2_cfg_ptr);
    }
    
    PMC_RETURN(result);
}/* digi_serdes_ilkn_rx_reset_toggle */
#endif /* DOXYGEN_PUBLIC_ONLY */

/*******************************************************************************
* digi_serdes_ge_port_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The functin provisions a given GE serdes: OHP_0, OHP_1, OHP_2, 
*  RI_RCP, DI_RCP or ENENT_MGMT
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   ge_intf             - GE serdes interface description.
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_ERROR
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_ge_port_prov(digi_handle_t *digi_handle,
                                          digi_ge_serdes_id_t ge_intf)
{
    PMC_ERROR result = PMC_SUCCESS;    

    PMC_ATOMIC_ENTRY(digi_handle);
    
    result = digi_serdes_ge_port_internal_prov(digi_handle,ge_intf);
        
    PMC_ATOMIC_RETURN(digi_handle,result);

}/* digi_serdes_ge_port_prov */


/*******************************************************************************
* digi_serdes_ge_port_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The functin deprovisions a given GE serdes: OHP_0, OHP_1, OHP_2, 
*  RI_RCP, DI_RCP or ENENT_MGMT
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   ge_intf            - GE serdes interface description.
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_ERROR
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_ge_port_deprov(digi_handle_t *digi_handle,
                                            digi_ge_serdes_id_t ge_intf)
{
    PMC_ERROR result = PMC_SUCCESS;    

    PMC_ATOMIC_ENTRY(digi_handle);
    
    result = digi_serdes_ge_port_internal_deprov(digi_handle,ge_intf);

    PMC_ATOMIC_RETURN(digi_handle,result);
}/* digi_serdes_ge_port_deprov */

#ifndef DOXYGEN_PUBLIC_ONLY

/*******************************************************************************
* digi_serdes_ge_port_internal_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The functin provisions a given GE serdes: OHP_0, OHP_1, OHP_2, 
*  RI_RCP, DI_RCP or ENENT_MGMT
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   ge_intf             - GE serdes interface description.
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_ERROR
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_ge_port_internal_prov(digi_handle_t *digi_handle,
                                                   digi_ge_serdes_id_t ge_intf)
{
    PMC_ERROR result = PMC_SUCCESS;    

    PMC_ENTRY();
    
    PMC_ASSERT((NULL != digi_handle), DIGI_ERR_NULL_HANDLE, 0, 0);
    
    result = digi_ge_intf_serdes_cfg(digi_handle, ge_intf, TRUE);
    
    PMC_RETURN(result);

}/* digi_serdes_ge_port_internal_prov */


/*******************************************************************************
* digi_serdes_ge_port_internal_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The functin deprovisions a given GE serdes: OHP_0, OHP_1, OHP_2, 
*  RI_RCP, DI_RCP or ENENT_MGMT
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   ge_intf            - GE serdes interface description.
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_ERROR
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_ge_port_internal_deprov(digi_handle_t *digi_handle,
                                                     digi_ge_serdes_id_t ge_intf)
{
    PMC_ERROR result = PMC_SUCCESS;    

    PMC_ENTRY();

    PMC_ASSERT((NULL != digi_handle), DIGI_ERR_NULL_HANDLE, 0, 0);
    
    result = digi_ge_intf_serdes_cfg(digi_handle, ge_intf, FALSE);

    PMC_RETURN(result);
}/* digi_serdes_ge_port_internal_deprov */

/*******************************************************************************
* digi_serdes_ge_port_internal_prov_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The functin get prov status a given GE serdes: OHP_0, OHP_1, OHP_2, 
*  RI_RCP, DI_RCP or ENENT_MGMT
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   ge_intf             - GE serdes interface description.
*
* OUTPUTS:
*   enable              - TRUE Ge port is enabled, FALSE it is disabled
*
* RETURNS:
*   PMC_ERROR
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_ge_port_internal_prov_get(digi_handle_t *digi_handle,
                                                       digi_ge_serdes_id_t ge_intf,
                                                       BOOL8 *enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    digi_ge_serdes_ctxt_t *ge_ctxt;
    *enable = FALSE;

    PMC_ENTRY();
    
    PMC_ASSERT((NULL != digi_handle), DIGI_ERR_NULL_HANDLE, 0, 0);
       
    if (PMC_SUCCESS == digi_ge_serdes_ctxt_get(digi_handle, ge_intf, &ge_ctxt))
    {
        *enable = ge_ctxt->enabled;
    }

    PMC_RETURN(result);

}/* digi_serdes_ge_port_internal_prov_get */


#endif /* DOXYGEN_PUBLIC_ONLY */
/*******************************************************************************
* digi_serdes_start_adaptation
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to start adaptation on the specified 
*  SERDES. API returns when adaptation is completed. An error code is returned
*  when adaptation failed for at least one serdes.
*
* INPUTS:
*  digi_handle     - DIGI device handle.
*  interface       - SERDES interface to configure.
*  lane_0_19_msk   - Bitmask of lanes 0-19 to configure. 
*                    Bit 0 corresponds to lane 0.
*  lane_20_39_msk  - Bitmask of lanes 20-39 to configure. 
*                    Bit 0 corresponds to lane 20.
*                    Ignored for S16 or C8 serdes.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_start_adaptation(digi_handle_t            *digi_handle, 
                                              hostmsg_adapt_intf_type_t interface,
                                              UINT32                    lane_0_19_msk,
                                              UINT32                    lane_20_39_msk)
{
    PMC_ERROR result = PMC_SUCCESS, tmp_result = PMC_SUCCESS;   
    UINT32 i;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);    
    
    result  = digi_serdes_adaptation_reset_check(digi_handle,interface, lane_0_19_msk, lane_20_39_msk);


    if (PMC_SUCCESS == result)
    {
        UINT32 local_lane_0_19_msk;
        UINT32 local_lane_20_39_msk;
        /* send serdes adaptation host command */
        local_lane_0_19_msk = 0;
        local_lane_20_39_msk = 0;
        for (i = 0; i < 20 ; i++)
        {
            local_lane_0_19_msk = 0x1 << i;
            if ((local_lane_0_19_msk & lane_0_19_msk) != 0)
            {
                tmp_result = digi_serdes_internal_start_adaptation(digi_handle,  interface,local_lane_0_19_msk,local_lane_20_39_msk);
            }
            if (tmp_result != PMC_SUCCESS) 
            {
                result = tmp_result;
            }

        }
        local_lane_0_19_msk = 0;
        for (i = 0; i < 20; i++)
        {
            local_lane_20_39_msk = 0x1 << i;
            if ((local_lane_20_39_msk & lane_20_39_msk) != 0)
            {
                tmp_result = digi_serdes_internal_start_adaptation(digi_handle,  interface,local_lane_0_19_msk,local_lane_20_39_msk);
            }
            if (tmp_result != PMC_SUCCESS) 
            {
                result = tmp_result;
            }
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End: digi_serdes_start_adaptation() */

/*******************************************************************************
* digi_serdes_cfg_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Set DIGI serdes configuration.
*
* INPUTS:
*  digi_handle        - DIGI device handle.
*  *serdes_cfg_ptr    - pointer to serdes configuration.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_cfg_set(digi_handle_t                *digi_handle,
                                     digi_serdes_cfg_t            *serdes_cfg_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* check parameters */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != serdes_cfg_ptr, DIGI_ERR_INVALID_ARG, 0, 0);

    PMC_MEMCPY(digi_handle->var.serdes_ctxt.serdes_cfg, serdes_cfg_ptr, sizeof(digi_serdes_cfg_t));

    PMC_ATOMIC_RETURN(digi_handle,rc);
}/* digi_serdes_cfg_set */


/*******************************************************************************
* digi_serdes_cfg_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Retrieve DIGI serdes configuration. Memory is allocated and serdes
*  configuration is copied in the allocated memory space. Pointer to 
*  allocated memory is returned. Allocated memory should be freed by function
*  caller.
*
* INPUTS:
*  digi_handle       - DIGI device handle.
*
* OUTPUTS:
*  *serdes_cfg_ptr  - Pointer to serdes configuration
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_cfg_get(digi_handle_t        *digi_handle,
                                     digi_serdes_cfg_t    *serdes_cfg_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* check parameters */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != serdes_cfg_ptr, DIGI_ERR_INVALID_ARG, 0, 0);

    PMC_MEMCPY(serdes_cfg_ptr, digi_handle->var.serdes_ctxt.serdes_cfg, sizeof(digi_serdes_cfg_t));

    PMC_ATOMIC_RETURN(digi_handle,rc);
}/* digi_serdes_cfg_get */

/*******************************************************************************
* digi_serdes_cfg_file_parse
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Open and parse a CSV-format file that contains serdes adaptation parameters.
*  Each line of the CSV-format file specifies adaptation parameters for one rate
*  of one serdes. One serdes is identified by its interface type and its lane id.
*  The function excepts the following fields per line:
*  For S16 and T8 serdes:
*  Serdes type,Serdes port mode,Lane,p_ffe_a,dp_ffe_b,dp_ffe_c,tr_ffe_a,tr_ffe_b,dfe_coef0,dfe_coef1,dfe_coef2,pga_gain,pga_offset,target_amp,adc_map_scale
*  
*  For C8 serdes
*  Serdes type,Serdes rates,Lane,p_ffe_a,target_amp,adc_map_scale
*
*  Serdes type range of values is:
*  >S16_LINE
*  >S16_SYS
*  >T8
*  >C8
*
* Line that starts with an unsupported serdes type are discarded.
*
*  Serdes rates range of value is:
*  >"S16_100_GE"
*  >"S16_40_GE",
*  >"S16_10_GE",
*  >"S16_FC_1200",
*  >"S16_FC_800",
*  >"S16_10G_GDPS",
*  >"S16_5G_GDPS",
*  >"S16_CPRI_10G",
*  >"S16_CPRI_6G",
*  >"S16_CPRI_5G",
*  >"S16_OC_768",
*  >"S16_OC_192",
*  >"S16_OTU4",
*  >"S16_OTU3",
*  >"S16_OTU3E2",
*  >"S16_OTU3E1",
*  >"S16_OTU2",
*  >"S16_OTU2E",
*  >"S16_OTU1E",
*  >"S16_OTU2F",
*  >"S16_OTU1F",
*  >"D8_OC_768",
*  >"D8_OTU3",
*  >"D8_OTU3E2",
*  >"D8_OTU3E1",
*  >"D8_GE",
*  >"T8_3G125",
*  >"T8_6G25",
*  >"C8_3G125",
*  >"C8_6G25",
*  >"S16_3G125",
*  >"S16_6G25",
*  >"S16_10G3125",
*  >"S16_11G5",
*  >"S16_12G5",
*
*
* INPUTS:
*  digi_handle       - DIGI device handle.
*  filename          - File name.
*  *serdes_cfg_ptr   - List of serdes adaptation parameters
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_cfg_file_parse(digi_handle_t        *digi_handle,
                                            char                 *filename,
                                            digi_serdes_cfg_t    *serdes_cfg_ptr)
{
    UINT32 i;
    UINT32 j;
    PMC_ERROR rc = PMC_SUCCESS;
    FILE* adapt_file = NULL;
    char *line;
    UINT32 line_offset = 0;
    char field_str[100];
    serdes_c8_adapt_objects_t    c8_adapt_object;
    serdes_t8_adapt_objects_t    t8_adapt_object;
    serdes_s16_adapt_objects_t   s16_adapt_object;
    UINT32 adapt_params_idx;
    UINT32 tmp;
    UINT32 nb_adapt_params;
    UINT32 f_offset = 0;
    UINT32 end_offset;
    UINT32 field_offset = 0;
    UINT8 rd_len;
    BOOL8 line_found;
    BOOL8 field_found;
    UINT32 *tmp_ptr;
    UINT8   buf[5];
    UINT32 lane;
    digi_serdes_intf_t serdes_intf;
    digi_serdes_port_mode_t serdes_rate;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != filename, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != serdes_cfg_ptr, DIGI_ERR_INVALID_ARG, 0, 0);    

    /* open file */
    adapt_file = pmc_sys_file_open(filename);
    if (NULL == adapt_file)
    {
        rc = PMC_SHARED_ERR_FILE_OPEN_FAILED;
    }

    if (PMC_SUCCESS == rc)
    {
        line = (char *)PMC_CALLOC(DIGI_SERDES_FILE_LINE_MAX_LENGTH * sizeof(char));

        /* for each line */
        do
        {
            /* pmc_sys_file_read only returns 4 bytes. Thus, we have to buffer enough bytes until end of line if read */   
            rc = pmc_sys_file_read(adapt_file, buf, &f_offset, &rd_len);
            if (PMC_SUCCESS != rc || rd_len == 0)
            {
                PMC_LOG_TRACE("EOF hit\n");
            }
            line_found = FALSE;
            if (PMC_SUCCESS == rc)
            { 
                for (i = 0; i < rd_len; i ++)
                {
                    if ((DIGI_SERDES_FILE_LINE_MAX_LENGTH -1) > line_offset)
                    {
                        /* buf size is 5, needs protection. */
                        if(i < 5)
                        {
                            line[line_offset] = buf[i];
                            line_offset +=1;    
                        }
                    }
                    else
                    {
                        rc =  DIGI_ERR_INVALID_ARG;
                        break;
                    }                    
                }
            }

            if (PMC_SUCCESS == rc)
            { 
                line[(DIGI_SERDES_FILE_LINE_MAX_LENGTH - 1)] = '\0';
                f_offset += rd_len;
                line_found = FALSE;
                if (4 > rd_len)
                {
                    line[line_offset] = '\n';
                    line_offset += 1;
                } 

                for (i = 0; i < line_offset && line_found == FALSE; i++)
                {                    
                    if ('\n' == line[i] || '\0' == line[i])
                    {
                        line_found = TRUE;
                    }
                }

                if (TRUE == line_found)
                {
                    end_offset = i;
                    field_offset = 0;
                    /* extract serdes type as the first field of the line */
                    digi_serdes_field_get(line, field_offset, field_str);
                    field_offset += PMC_STRLEN(field_str) + 1;
                    if (0 < PMC_STRLEN(field_str))
                    {
                        serdes_intf = digi_serdes_interface_from_string_get(field_str);
                        if (LAST_DIGI_SERDES_INTF != serdes_intf)
                        {
                            field_found = TRUE;
                            /* serdes interface found on line, then parse the rest of the line */
                            /* parse port_mode */
                            digi_serdes_field_get(line, field_offset, field_str);
                            field_offset += PMC_STRLEN(field_str) + 1;
                            serdes_rate = digi_serdes_port_mode_from_string_get(field_str);
                            if (LAST_DIGI_SERDES_PORT_MODE == serdes_rate)
                            {
                                rc = DIGI_ERR_INVALID_SERDES_RATE;
                            }
                            else 
                            {
                                /* parse lane */
                                digi_serdes_field_get(line, field_offset, field_str);
                                field_offset += PMC_STRLEN(field_str) + 1;
                                /* verify that the field is a digital */
                                if (0 < PMC_STRLEN(field_str) && util_global_isnumber(field_str))
                                {                       
                                    lane = pmc_atoi(field_str);                      
                                    /* parse adaptation parameters */
                                    switch (serdes_intf)
                                    {
                                        /* fallthrough */
                                    case DIGI_SERDES_LINE_INTF:
                                    case DIGI_SERDES_SYSTEM_INTF:
                                        tmp_ptr = (UINT32 *)&s16_adapt_object.dp_ffe_a;
                                        s16_adapt_object.port_mode = serdes_rate;
                                        s16_adapt_object.valid = TRUE;
                                        nb_adapt_params = 12;
                                        break;
                                        /* fallthrough */
                                    case DIGI_SERDES_BACKPLANE_LR_INTF:
                                        tmp_ptr = (UINT32 *)&t8_adapt_object.dp_ffe_a;
                                        t8_adapt_object.port_mode = serdes_rate;
                                        t8_adapt_object.valid = TRUE;
                                        nb_adapt_params = 15;
                                        break;
                                    case DIGI_SERDES_BACKPLANE_SR_INTF:
                                        tmp_ptr = (UINT32 *)&c8_adapt_object.dp_ffe_a;
                                        c8_adapt_object.port_mode = serdes_rate;
                                        c8_adapt_object.valid = TRUE;
                                        nb_adapt_params = 3;
                                        break;
                                    default:
                                        nb_adapt_params = 0;
                                        break;
                                    }
                                    for (i = 0; i < nb_adapt_params && TRUE == field_found; i++)
                                    {
                                        digi_serdes_field_get(line, field_offset, field_str);    
                                        field_offset += PMC_STRLEN(field_str) + 1;
                                        /* verify that the field is a digital */
                                        if (0 < PMC_STRLEN(field_str) && util_global_isnumber(field_str))
                                        {
                                            tmp = pmc_atoi(field_str);       
                                            *tmp_ptr = tmp;
                                            tmp_ptr += 1;
                                        }
                                        else
                                        {
                                            field_found = FALSE;
                                        }/* parse adaptation parameters */
                                    }
                                    if (TRUE == field_found && 0 < nb_adapt_params)
                                    {
                                        /* validate extracted parameters before it is writen */ 
                                        if (TRUE == digi_serdes_adapt_params_validate(digi_handle, serdes_intf, lane, serdes_rate))
                                        {
                                            /* parse adaptation parameters */
                                            switch (serdes_intf)
                                            {
                                            case DIGI_SERDES_LINE_INTF:
                                                for (adapt_params_idx = 0; adapt_params_idx < DIGI_SERDES_S16_NUM_RATES_MAX; adapt_params_idx++)
                                                {
                                                    if (FALSE == serdes_cfg_ptr->s16_line_cfg[lane].s16_line_adapt_params[adapt_params_idx].valid)
                                                    {
                                                        break;
                                                    }
                                                }
                                                if (DIGI_SERDES_S16_NUM_RATES_MAX > adapt_params_idx)
                                                {
                                                    PMC_MEMCPY(&serdes_cfg_ptr->s16_line_cfg[lane].s16_line_adapt_params[adapt_params_idx], 
                                                               (UINT8 *)&s16_adapt_object, 
                                                               sizeof(serdes_s16_adapt_objects_t));   
                                                }
                                                else
                                                {
                                                    rc = DIGI_ERR_UNKNOWN;
                                                }
                                                break;
                                            case DIGI_SERDES_SYSTEM_INTF:
                                                for (adapt_params_idx = 0; adapt_params_idx < DIGI_SERDES_S16_NUM_RATES_MAX; adapt_params_idx++)    
                                                {
                                                    if (FALSE == serdes_cfg_ptr->s16_sys_cfg[lane].s16_sys_adapt_params[adapt_params_idx].valid)
                                                    {
                                                        break;
                                                    }
                                                }
                                                if (DIGI_SERDES_S16_NUM_RATES_MAX > adapt_params_idx)
                                                {
                                                    PMC_MEMCPY(&serdes_cfg_ptr->s16_sys_cfg[lane].s16_sys_adapt_params[adapt_params_idx], 
                                                               (UINT8 *)&s16_adapt_object, 
                                                               sizeof(serdes_s16_adapt_objects_t));
                                                }
                                                else
                                                {
                                                    rc = DIGI_ERR_UNKNOWN;
                                                }
                                                break;
                                            case DIGI_SERDES_BACKPLANE_LR_INTF:
                                                for (adapt_params_idx = 0; adapt_params_idx < DIGI_SERDES_T8_NUM_RATES_MAX; adapt_params_idx++)
                                                {
                                                    if (FALSE == serdes_cfg_ptr->t8_cfg[lane].t8_adapt_params[adapt_params_idx].valid)
                                                    {
                                                        break;
                                                    }
                                                }
                                                if (DIGI_SERDES_T8_NUM_RATES_MAX > adapt_params_idx)
                                                {
                                                    PMC_MEMCPY(&serdes_cfg_ptr->t8_cfg[lane].t8_adapt_params[adapt_params_idx], 
                                                               (UINT8 *)&t8_adapt_object, 
                                                               sizeof(serdes_t8_adapt_objects_t)); 
                                                }
                                                else
                                                {
                                                    rc = DIGI_ERR_UNKNOWN;
                                                }
                                                break;
                                            case DIGI_SERDES_BACKPLANE_SR_INTF:
                                                for (adapt_params_idx = 0; adapt_params_idx < DIGI_SERDES_C8_NUM_RATES_MAX; adapt_params_idx++)
                                                {
                                                    if (FALSE == serdes_cfg_ptr->c8_cfg[lane].c8_adapt_params[adapt_params_idx].valid)
                                                    {
                                                        break;
                                                    }
                                                }

                                                if (DIGI_SERDES_C8_NUM_RATES_MAX > adapt_params_idx)
                                                {
                                                    PMC_MEMCPY(&serdes_cfg_ptr->c8_cfg[lane].c8_adapt_params[adapt_params_idx], 
                                                               (UINT8 *)&c8_adapt_object, 
                                                               sizeof(serdes_c8_adapt_objects_t)); 
                                                }
                                                else
                                                {
                                                    rc = DIGI_ERR_UNKNOWN;
                                                }
                                                break;
                                            default:
                                                break;
                                            }
                                        }     
                                        else
                                        {
                                            rc = DIGI_ERR_UNKNOWN;
                                        }
                                    }       
                                    else
                                    {
                                        rc = DIGI_ERR_UNKNOWN;
                                    }
                                }  
                                else
                                {
                                    field_found = FALSE;
                                }/* parse port mode */                  
                            } /* parse lane */
                        }
                    }

                    i = end_offset;
                    /* shift next line characters in the beginning */
                    for (j = 0; i < line_offset; j++, i ++)
                    {
                        line[j] = line[i];
                    }
                    line_offset = j;
                }
            }
        }
        while(PMC_SUCCESS == rc && rd_len == 4);
        
        PMC_FREE(&line);
    }
    
    if (NULL != adapt_file)
    {
        pmc_sys_file_close(adapt_file);
    }
    
    PMC_ATOMIC_RETURN(digi_handle,rc);
}/*digi_serdes_cfg_file_parse*/

/*******************************************************************************
* FUNCTION: digi_serdes_cfg_file_save()
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Save serdes adaptation parameters in a CSV file in which each line defines:
*   Serdes Interface,Rate,Lane,dp_ffe_a,dp_ffe_b,dp_ffe_c,tr_ffe_a,tr_ffe_b,dfe_coef0,dfe_coef1,dfe_coef2,dfe_coef3,dfe_coef4,dfe_coef5,pga_gain,pga_offset,target_amp,adc_map_scale
*
*  CSV file first contains the colums header.
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   file_path_ptr   - file path
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None.
*
*******************************************************************************/
PUBLIC void digi_serdes_cfg_file_save(digi_handle_t            *digi_handle,
                                      const char               *file_path_ptr)
{
    
#ifdef PMC_FPRINTF
    UINT32 itr;
    UINT32 jtr;
    FILE* adapt_file = NULL;
    const char *port_mode_string_l[] = DIGI_SERDES_PORT_MODE_IN_STRING;
#endif /*PMC_FPRINTF*/
#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif
    /* check parameters */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that PMC_FPRINTF operation is supported in the environment */
#ifndef PMC_FPRINTF
    PMC_ASSERT(0, DIGI_ERR_UNKNOWN, 0, 0);
    PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_UNKNOWN, 0, 0, "FPRINTF operation not supported. Check that PMC_FPRINTF is implemented\n");
#else
    /* open file */
    adapt_file = pmc_sys_output_file_open(file_path_ptr);
    
    PMC_ASSERT(NULL != adapt_file, PMC_SHARED_ERR_FILE_OPEN_FAILED, 0, 0);

    PMC_FPRINTF(adapt_file, "Serdes Interface,Rate,Lane,dp_ffe_a,dp_ffe_b,dp_ffe_c,tr_ffe_a,tr_ffe_b,dfe_coef0,dfe_coef1,dfe_coef2,dfe_coef3,dfe_coef4,dfe_coef5,pga_gain,pga_offset,target_amp,adc_map_scale\n");

    /* write S16 LINE serdes adaptation parameters in file */
    for (itr = 0; itr < DIGI_SERDES_XFI_LANES_MAX; itr++)
    {
        for (jtr = 0; jtr < DIGI_SERDES_S16_NUM_RATES_MAX; jtr++)
        {
            /*only write valid paramters in the CSV file */
            if (TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[itr].s16_line_adapt_params[jtr].valid && 
                (DIGI_SERDES_PORT_MODE_ANY_LOWPWR <= digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[itr].s16_line_adapt_params[jtr].port_mode &&
                 LAST_DIGI_SERDES_PORT_MODE > digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[itr].s16_line_adapt_params[jtr].port_mode))
            {
                PMC_FPRINTF(adapt_file, "S16_LINE,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", port_mode_string_l[digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[itr].s16_line_adapt_params[jtr].port_mode], 
                            itr,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[itr].s16_line_adapt_params[jtr].dp_ffe_a,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[itr].s16_line_adapt_params[jtr].dp_ffe_b,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[itr].s16_line_adapt_params[jtr].dp_ffe_c,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[itr].s16_line_adapt_params[jtr].tr_ffe_a,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[itr].s16_line_adapt_params[jtr].tr_ffe_b,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[itr].s16_line_adapt_params[jtr].dfe_coef0,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[itr].s16_line_adapt_params[jtr].dfe_coef1,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[itr].s16_line_adapt_params[jtr].dfe_coef2,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[itr].s16_line_adapt_params[jtr].pga_gain,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[itr].s16_line_adapt_params[jtr].pga_offset,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[itr].s16_line_adapt_params[jtr].target_amp,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[itr].s16_line_adapt_params[jtr].adc_map_scale);
            }
        }
    }

    /* write S16 SYS serdes adaptation parameters in file */
    for (itr = 0; itr <  DIGI_SERDES_MAX_S16_IN_BP; itr++)
    {
        for (jtr = 0; jtr < DIGI_SERDES_S16_NUM_RATES_MAX; jtr++)
        {
            /*only write valid paramters in the CSV file */
            if (TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[itr].s16_sys_adapt_params[jtr].valid && 
                (DIGI_SERDES_PORT_MODE_ANY_LOWPWR <= digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[itr].s16_sys_adapt_params[jtr].port_mode &&
                 LAST_DIGI_SERDES_PORT_MODE > digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[itr].s16_sys_adapt_params[jtr].port_mode))
            {
                PMC_FPRINTF(adapt_file, "S16_SYS,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", port_mode_string_l[digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[itr].s16_sys_adapt_params[jtr].port_mode], 
                            itr,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[itr].s16_sys_adapt_params[jtr].dp_ffe_a,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[itr].s16_sys_adapt_params[jtr].dp_ffe_b,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[itr].s16_sys_adapt_params[jtr].dp_ffe_c,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[itr].s16_sys_adapt_params[jtr].tr_ffe_a,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[itr].s16_sys_adapt_params[jtr].tr_ffe_b,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[itr].s16_sys_adapt_params[jtr].dfe_coef0,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[itr].s16_sys_adapt_params[jtr].dfe_coef1,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[itr].s16_sys_adapt_params[jtr].dfe_coef2,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[itr].s16_sys_adapt_params[jtr].pga_gain,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[itr].s16_sys_adapt_params[jtr].pga_offset,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[itr].s16_sys_adapt_params[jtr].target_amp,
                            digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[itr].s16_sys_adapt_params[jtr].adc_map_scale);
            }
        }
    }

    /* write T8 serdes adaptation parameters in file */
    for (itr = 0; itr < DIGI_SERDES_MAX_T8_E1_IN_BP_LR; itr++)
    {
        for (jtr = 0; jtr < DIGI_SERDES_T8_NUM_RATES_MAX; jtr++)
        {
            /*only write valid paramters in the CSV file */
            if (TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[itr].t8_adapt_params[jtr].valid && 
                (DIGI_SERDES_PORT_MODE_ANY_LOWPWR <= digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[itr].t8_adapt_params[jtr].port_mode &&
                 LAST_DIGI_SERDES_PORT_MODE > digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[itr].t8_adapt_params[jtr].port_mode))
            {
                PMC_FPRINTF(adapt_file, "T8,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", port_mode_string_l[digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[itr].t8_adapt_params[jtr].port_mode], 
                            itr,
                            digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[itr].t8_adapt_params[jtr].dp_ffe_a,
                            digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[itr].t8_adapt_params[jtr].dp_ffe_b,
                            digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[itr].t8_adapt_params[jtr].dp_ffe_c,
                            digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[itr].t8_adapt_params[jtr].tr_ffe_a,
                            digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[itr].t8_adapt_params[jtr].tr_ffe_b,
                            digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[itr].t8_adapt_params[jtr].dfe_coef0,
                            digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[itr].t8_adapt_params[jtr].dfe_coef1,
                            digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[itr].t8_adapt_params[jtr].dfe_coef2,
                            digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[itr].t8_adapt_params[jtr].dfe_coef3,
                            digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[itr].t8_adapt_params[jtr].dfe_coef4,
                            digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[itr].t8_adapt_params[jtr].dfe_coef5,
                            digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[itr].t8_adapt_params[jtr].pga_gain,
                            digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[itr].t8_adapt_params[jtr].pga_offset,
                            digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[itr].t8_adapt_params[jtr].target_amp,
                            digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[itr].t8_adapt_params[jtr].adc_map_scale);
            }
        }
    }

    /* write C8 serdes adaptation parameters in file */
    for (itr = 0; itr < DIGI_SERDES_MAX_C8_IN_BP_SR; itr++)
    {
        for (jtr = 0; jtr < DIGI_SERDES_C8_NUM_RATES_MAX; jtr++)
        {
            /*only write valid paramters in the CSV file */
            if (TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[itr].c8_adapt_params[jtr].valid && 
                (DIGI_SERDES_PORT_MODE_ANY_LOWPWR <= digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[itr].c8_adapt_params[jtr].port_mode &&
                 LAST_DIGI_SERDES_PORT_MODE > digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[itr].c8_adapt_params[jtr].port_mode))
            {
                PMC_FPRINTF(adapt_file, "C8,%s,%d,%d,%d,%d\n", port_mode_string_l[digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[itr].c8_adapt_params[jtr].port_mode], 
                            itr,
                            digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[itr].c8_adapt_params[jtr].dp_ffe_a,
                            digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[itr].c8_adapt_params[jtr].target_amp,
                            digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[itr].c8_adapt_params[jtr].adc_map_scale);
            }
        }
    }

    /* close file */
    if (NULL != adapt_file)
    {
        pmc_sys_file_close(adapt_file);
    }

#endif/* #ifndef PMC_FPRINTF */

    PMC_ATOMIC_RETURN(digi_handle);
}/* digi_serdes_cfg_file_save() */


/*******************************************************************************
* digi_serdes_s16_tx_swing_emphasis_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Function sets TX swing and de-emphasis levels for a S16 serdes.
*  TX swing and de-emphasis levels are also saved in serdes context after
*  values are successfully written in the device registers.
*
* INPUTS:
*  digi_handle   - DIGI device handle.
*  s16_intf      - S16 interface: LINE or SYSTEM
*  lane          - Lane to configure.
*  amplitude     - TX launch amplitude for calibration.
*  post_cursor   - TX de-emphasis for calibration.
*  pre_cursor    - TX edge rate for calibration.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_s16_tx_swing_emphasis_set(digi_handle_t       *digi_handle,
                                                       digi_serdes_intf_t   s16_intf,
                                                       UINT32               lane,
                                                       UINT8                amplitude,
                                                       UINT8                post_cursor,
                                                       UINT8                pre_cursor)
{
    PMC_ERROR rc = PMC_SUCCESS;
    s16_handle_t *s16_handle;
    s16_tx_swing_levels_t  levels_cfg;
    UINT32 nb_lanes_max;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT((NULL != digi_handle), DIGI_ERR_NULL_HANDLE, 0, 0);

    switch (s16_intf)
    {
    case DIGI_SERDES_LINE_INTF:
        s16_handle = digi_handle->s16_line_handle;
        nb_lanes_max = DIGI_SERDES_XFI_LANES_MAX;
        break;
    case DIGI_SERDES_SYSTEM_INTF:
        s16_handle = digi_handle->s16_sys_handle;
        nb_lanes_max = DIGI_SERDES_MAX_S16_IN_BP;
        break;
    default:
        rc = DIGI_ERR_INVALID_ARG;
        break;
    }

    if (PMC_SUCCESS == rc)
    {
        PMC_ASSERT(nb_lanes_max >= lane, DIGI_ERR_INVALID_ARG, 0, 0);
        
        /* Write levels in device */
        levels_cfg.amplitude = amplitude;
        levels_cfg.post_cursor = post_cursor;
        levels_cfg.pre_cursor = pre_cursor;
        rc = s16_tx_swing_emphasis_levels_set(s16_handle, lane, &levels_cfg);

        /* save TX swing properties in serdes context */
        if (PMC_SUCCESS == rc)
        {
            rc = digi_serdes_s16_tx_swing_emphasis_levels_in_ctxt_set(
                digi_handle, s16_intf, lane, &levels_cfg);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
}/*digi_serdes_s16_tx_swing_emphasis_set*/

/*******************************************************************************
* digi_serdes_t8_tx_swing_emphasis_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Function sets TX swing and de-emphasis levels for a T8 serdes.
*  TX swing and de-emphasis levels are also saved in serdes context after
*  values are successfully written in the device registers.
*
* INPUTS:
*  digi_handle  - DIGI device handle.
*  lane         - Lane to configure.
*  tx_dpeni1    - First pre-emphasis driver stages state.
*  tx_dpeni2    - Second pre-emphasis driver stages state.
*  deni         - output driver stages state.
*  tx_psel      - TX slave selector.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_t8_tx_swing_emphasis_set(digi_handle_t       *digi_handle,
                                                      UINT32              lane,
                                                      UINT8               tx_dpeni1,
                                                      UINT8               tx_dpeni2,
                                                      UINT32              deni,
                                                      UINT32              tx_psel)
{
    UINT8 serdes_id;
    t8_tx_swing_levels_t  levels_cfg;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT((NULL != digi_handle), DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(DIGI_SERDES_MAX_T8_E1_IN_BP_LR >= lane, DIGI_ERR_INVALID_ARG, 0, 0);


    if (PMC_SUCCESS == rc)
    {
        /* Write levels in device */
        levels_cfg.tx_dpeni1 = tx_dpeni1;
        levels_cfg.tx_dpeni2 = tx_dpeni2;
        levels_cfg.deni = deni;
        levels_cfg.tx_psel = tx_psel;
        serdes_id = digi_serdes_map_t8_serdes((UINT8)lane);

        rc = t8_tx_swing_emphasis_levels_set(digi_handle->bp_lr_t8_handle[DIGI_SERDES_T8_SLICE_ID_GET(lane)], 
                                             serdes_id, &levels_cfg);

        /* save TX swing properties in serdes context */
        if (PMC_SUCCESS == rc)
        {
            rc = digi_serdes_t8_tx_swing_emphasis_levels_in_ctxt_set(digi_handle,
                                                                     lane, &levels_cfg);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
}/*digi_serdes_t8_tx_swing_emphasis_set*/

/*******************************************************************************
* digi_serdes_c8_tx_swing_emphasis_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Function sets TX swing and de-emphasis levels for a C8 serdes.
*  TX swing and de-emphasis levels are also saved in serdes context after
*  values are successfully written in the device registers.
*
* INPUTS:
*  digi_handle  - DIGI device handle.
*  lane         - Lane to configure.
*  tx_dpeni1    - First pre-emphasis driver stages state.
*  tx_dpeni2    - Second pre-emphasis driver stages state.
*  deni         - output driver stages state.
*  tx_psel      - TX slave selector.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_c8_tx_swing_emphasis_set(digi_handle_t       *digi_handle,
                                                      UINT32              lane,
                                                      UINT8               tx_dpeni1,
                                                      UINT8               tx_dpeni2,
                                                      UINT32              deni,
                                                      UINT32              tx_psel)
{
    c8_tx_swing_levels_t  levels_cfg;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT((NULL != digi_handle), DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(DIGI_SERDES_MAX_C8_IN_BP_SR >= lane, DIGI_ERR_INVALID_ARG, 0, 0);
    
    if (PMC_SUCCESS == rc)
    {
        /* Write levels in device */
        levels_cfg.tx_dpeni1 = tx_dpeni1;
        levels_cfg.tx_dpeni2 = tx_dpeni2;
        levels_cfg.deni = deni;
        levels_cfg.tx_psel = tx_psel;
        rc = c8_tx_swing_emphasis_levels_set(digi_handle->bp_sr_c8_handle, 
                                             lane, &levels_cfg);

        /* save TX swing properties in serdes context */
        if (PMC_SUCCESS == rc)
        {

            rc = digi_serdes_c8_tx_swing_emphasis_levels_in_ctxt_set(digi_handle,
                                                                     lane, &levels_cfg);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
}/*digi_serdes_c8_tx_swing_emphasis_set*/

/*******************************************************************************
* digi_serdes_sfi51_tx_swing_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Function sets SFI51 bus's serdes TX swing level.
*  TX swing level is also saved in serdes context after
*  value is successfully written in the device register.
*
* INPUTS:
*   digi_handle        - DIGI device handle.
*   bus_id             - SFI51 bus identifier
*   lane               - Serdes lane to configure. Legal range 0 to 16.
*   tx_swing_level     - TX swing new value. Possible values are:
*                           D8_TX_2X_TX_SWING_LEVEL_326     =  326 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_372     =  372 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_419     =  419 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_465     =  465 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_511     =  511 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_557     =  557 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_603     =  603 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_648     =  648 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_693     =  693 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_737     =  737 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_781     =  781 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_825     =  825 mVppd (Default value)
*                           D8_TX_2X_TX_SWING_LEVEL_868     =  868 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_910     =  910 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_952     =  952 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_992     =  992 mVppd
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_sfi51_tx_swing_set(digi_handle_t             *digi_handle,
                                                UINT32                     bus_id,
                                                UINT32                     lane,
                                                d8_tx_2x_tx_swing_level_t  tx_swing_level)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 itr;
    const UINT8 sfi51_tx_serdes_pair[] = DIGI_SFI51_TX_SERDES_PAIR_LIST;
    BOOL8 found;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT((NULL != digi_handle), DIGI_ERR_NULL_HANDLE, 0, 0);

    if (PMC_SUCCESS == rc)
    {
        /* check bus id */
        if (DIGI_NUM_SFI51_PORTS_MAX <= bus_id)
        {
            rc = DIGI_ERR_INVALID_ARG;
        }

        /* check that pin_id is in range [0..17]*/
        if (SFI51_D8_TX_MAX_NUM_SERDES <= lane)
        {
            rc = DIGI_ERR_INVALID_ARG;
        }

        if (PMC_SUCCESS == rc)
        {        
            /* Write levels in device */
            rc = sfi51_d8_tx_18x_swing_level_set(digi_handle->sfi51_d8_tx_18x_handle[bus_id], sfi51_tx_serdes_pair[lane], tx_swing_level);

            /* save setting in serdes context */
            if (PMC_SUCCESS == rc)
            {
                found = FALSE;
                if (LAST_DIGI_SERDES_PORT_MODE != digi_handle->var.serdes_ctxt.sfi51_lanes_rate[bus_id])
                {
                    found = TRUE;
                }

                /* Save TX swing level in context */
                if (TRUE == found)
                {
                    found = FALSE;
                    
                    for (itr = 0; itr < DIGI_SERDES_SFI51_D8_NUM_RATES_MAX && FALSE == found; itr++)
                    {
                        if (TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->sfi51_cfg[bus_id].sfi51_d8_cfg[lane].d8_cfg[itr].d8_tx_swing_level.valid &&                            
                            digi_handle->var.serdes_ctxt.sfi51_lanes_rate[bus_id] == digi_handle->var.serdes_ctxt.serdes_cfg->sfi51_cfg[bus_id].sfi51_d8_cfg[lane].d8_cfg[itr].d8_tx_swing_level.port_mode)
                        {
                            digi_handle->var.serdes_ctxt.serdes_cfg->sfi51_cfg[bus_id].sfi51_d8_cfg[lane].d8_cfg[itr].d8_tx_swing_level.tx_swing_level = tx_swing_level;
                            found = TRUE;
                            PMC_LOG_TRACE("in %s br 0 -> sfi51 tx swing updated in ctxt %d %d %d %d %d\n", __FUNCTION__, bus_id, lane, itr, digi_handle->var.serdes_ctxt.serdes_cfg->sfi51_cfg[bus_id].sfi51_d8_cfg[lane].d8_cfg[itr].d8_tx_swing_level.port_mode, tx_swing_level);
                        }
                    }
                    

                    if (FALSE == found)
                    {
                        for (itr = 0; itr < DIGI_SERDES_SFI51_D8_NUM_RATES_MAX && FALSE == found; itr++)
                        {
                            if (FALSE == digi_handle->var.serdes_ctxt.serdes_cfg->sfi51_cfg[bus_id].sfi51_d8_cfg[lane].d8_cfg[itr].d8_tx_swing_level.valid)
                            {
                                digi_handle->var.serdes_ctxt.serdes_cfg->sfi51_cfg[bus_id].sfi51_d8_cfg[lane].d8_cfg[itr].d8_tx_swing_level.valid = TRUE;
                                digi_handle->var.serdes_ctxt.serdes_cfg->sfi51_cfg[bus_id].sfi51_d8_cfg[lane].d8_cfg[itr].d8_tx_swing_level.port_mode = digi_handle->var.serdes_ctxt.sfi51_lanes_rate[bus_id];
                                digi_handle->var.serdes_ctxt.serdes_cfg->sfi51_cfg[bus_id].sfi51_d8_cfg[lane].d8_cfg[itr].d8_tx_swing_level.tx_swing_level = tx_swing_level;
                                found = TRUE;
                                PMC_LOG_TRACE("in %s br 1 -> sfi51 tx swing updated in ctxt %d %d %d %d %d\n", __FUNCTION__, bus_id, lane, itr, digi_handle->var.serdes_ctxt.serdes_cfg->sfi51_cfg[bus_id].sfi51_d8_cfg[lane].d8_cfg[itr].d8_tx_swing_level.port_mode, tx_swing_level);
                            }
                        }
                    }   
                }

                if (FALSE == found)
                {
                    rc = DIGI_ERR_SERDES_PARAMS_NOT_SAVED_IN_CTXT;
                }
            }
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /*digi_serdes_sfi51_tx_swing_set*/

/*******************************************************************************
* digi_serdes_ge_tx_swing_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Function sets TX swing level for a supporting interface serdes (OHP, RCP, MGMT_FEGE).
*  TX swing level is also saved in serdes context after
*  value is successfully written in the device register.
*
* INPUTS:
*  digi_handle       - DIGI device handle
*  ge_intf           - GE interface associated to serdes
*  tx_swing_level    - Tx swing level new value. Possible values are:
*                           D8_TX_2X_TX_SWING_LEVEL_326     =  326 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_372     =  372 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_419     =  419 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_465     =  465 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_511     =  511 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_557     =  557 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_603     =  603 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_648     =  648 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_693     =  693 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_737     =  737 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_781     =  781 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_825     =  825 mVppd (Default value)
*                           D8_TX_2X_TX_SWING_LEVEL_868     =  868 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_910     =  910 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_952     =  952 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_992     =  992 mVppd
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_ge_tx_swing_set(digi_handle_t              *digi_handle,
                                             digi_ge_serdes_id_t         ge_intf,
                                             d8_tx_2x_tx_swing_level_t   tx_swing_level)
{
    d8_tx_2x_handle_t *d8_tx_2x_handle;
    UINT8 serdes_id;         
    digi_ge_serdes_ctxt_t *ge_serdes_ctxt;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* retrieve d8_tx_2x handle associated to ge serdes */
    result = digi_ge_d8_tx_2x_handle_get(digi_handle, ge_intf, &d8_tx_2x_handle, &serdes_id);

    if (PMC_SUCCESS == result)
    {
        result = digi_ge_serdes_ctxt_get(digi_handle,
                                         ge_intf,
                                         &ge_serdes_ctxt);
        if (PMC_SUCCESS == result)
        {
            if (FALSE == ge_serdes_ctxt->enabled)
            {
                result = DIGI_ERR_GE_SERDES_NOT_PROV;
            }
        }
    }

    if (PMC_SUCCESS == result)
    {
        result =  d8_tx_2x_tx_swing_level_set(d8_tx_2x_handle,
                                              serdes_id,
                                              tx_swing_level);
        /* save setting in serdes context */
        if (PMC_SUCCESS == result)
        {
            digi_handle->var.serdes_ctxt.serdes_cfg->ge_cfg[(UINT32)ge_intf].d8_cfg.d8_tx_swing_level.valid = TRUE;
            digi_handle->var.serdes_ctxt.serdes_cfg->ge_cfg[(UINT32)ge_intf].d8_cfg.d8_tx_swing_level.tx_swing_level = tx_swing_level;
        }
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
}/*digi_serdes_ge_tx_swing_set*/

/*******************************************************************************
* digi_serdes_dc_coupling_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Function sets Serdes in DC coupling mode or in AC mode. The API should be called
*  before digi_device_init() call, otherwise dc coupling mode for T8 and S16 SYS
*  will not be configured in the device.
*
*
* INPUTS:
*  digi_handle           - DIGI device handle.
*  interface             - Serdes type: S16 LINE, S16 SYS, T8.
*  lane_0_19_msk         - Serdes 0 to 19 lanes mask.
*                          Bit 0 corresponds to lane 0. 
*  lane_20_39_msk        - Serdes 20 to 39 lanes mask.
*                          Bit 0 corresponds to lane 20. 
*  dc_coupling_enabled   - when TRUE Serdes are set in DC coupling mode
*                          otherwise AC coupling mode is configured
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code 
*   is returned
*
* NOTES:
*   C8 serdes only supports AC coupling mode. An error code is returned when this API 
*   is called to set DC coupling mode for C8.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_dc_coupling_set(digi_handle_t       *digi_handle,
                                             digi_serdes_intf_t   interface,
                                             UINT32               lane_0_19_msk,
                                             UINT32               lane_20_39_msk,
                                             BOOL8                dc_coupling_enabled)
{
    PMC_ERROR rc = DIGI_ERR_INVALID_ARG;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT((NULL != digi_handle), DIGI_ERR_NULL_HANDLE, 0, 0);

    /* call DC coupling set sub-function per interface type */
    switch (interface)
    {
    case DIGI_SERDES_LINE_INTF:
        if (0 == lane_20_39_msk)
        {
            rc = digi_serdes_s16_dc_coupling_in_ctxt_set(digi_handle, interface, 
                                                         lane_0_19_msk, dc_coupling_enabled);
        }
        break;
    case DIGI_SERDES_SYSTEM_INTF:
        if (0 == lane_20_39_msk)
        {
            rc = digi_serdes_s16_dc_coupling_in_ctxt_set(digi_handle, interface, 
                                                         lane_0_19_msk, dc_coupling_enabled);
        }
        break;
    case DIGI_SERDES_BACKPLANE_LR_INTF:
        rc = digi_serdes_t8_dc_coupling_in_ctxt_set(digi_handle, lane_0_19_msk, 
                                                    lane_20_39_msk, dc_coupling_enabled);
        break;
    default:
        break;        
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
}/*digi_serdes_dc_coupling_set*/





/*******************************************************************************
* digi_serdes_sfi51_tx_refclk_source_select
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Function sets SFI51 interface TX reference clock source.
*
* INPUTS:
*  digi_handle                   - DIGI device handle.
*  port_ctxt_ptr                 - Pointer to the SFI51. SERDES instance.
*  refclk_src                    - Reference clock source: source 0, 1, or 2.
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, a descriptive error code otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_sfi51_tx_refclk_source_set(digi_handle_t                     *digi_handle, 
                                                        digi_serdes_port_t                *port_ctxt_ptr,
                                                        util_global_sfi51_refclk_source_t  refclk_src)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    UINT8 index;
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* verify that port ctxt ptr belongs to digi handle pool */
    DIGI_SERDES_PORT_CTXT_HANDLE_CHECK(digi_handle, port_ctxt_ptr, rc);

    rc = digi_serdes_d8_tx_dclk_validate_params_and_instance_get(digi_handle, 
                                                                 port_ctxt_ptr, 
                                                                 &index);

    if (rc == PMC_SUCCESS) 
    {
        rc = sfi51_d8_tx_18x_tx_refclk_source_set(digi_handle->sfi51_d8_tx_18x_handle[index], refclk_src);
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);

} /* digi_serdes_sfi51_tx_refclk_source_set */

/*******************************************************************************
* digi_serdes_adapt_status_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves adaptation results for SERDES configuration.
*
* INPUTS:
*  digi_handle - DIGI device handle.
*
* OUTPUTS:
*  serdes_adapt_status_ptr - pointer to storage for returned status.\n
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_adapt_status_get(digi_handle_t *digi_handle,
                                              serdes_adapt_status_t *serdes_adapt_status_ptr)
{
    PMC_ERROR result;  

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_serdes_internal_adapt_status_get(digi_handle, serdes_adapt_status_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_serdes_adapt_status_get */

/*******************************************************************************
* digi_serdes_debug_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to configure debug variables for SERDES.
*
* INPUTS:
*  digi_handle     - DIGI device handle.
*  skip_cfg        - TRUE: Skip SERDES configuration when processing
*                          SERDES host messages.
*                    FALSE: Perform SERDES configuration when processing
*                           SERDES host messages (default).
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_debug_cfg(digi_handle_t *digi_handle, 
                                       BOOL skip_cfg)
{
    PMC_ERROR result = PMC_SUCCESS; 
    UINT32 data[1];

    PMC_ATOMIC_ENTRY(digi_handle);

    /* CMD_CFG_SERDES_DEBUG have the following format:
     * Word  bits 24..31   bits 16..23   bits 8..15   bits 0..7
     *   0    skip_cfg            reserved
     */
    data[0] = (UINT32)skip_cfg << 24;

    /* Send message*/
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t*)digi_handle,MODID_SERDES,HOSTMSG_CMD_CFG_SERDES_DEBUG_ST,data,1,stat);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End: digi_serdes_debug_cfg() */


/*******************************************************************************
* digi_serdes_debug_status_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves debug variables for SERDES configuration.
*
* INPUTS:
*  digi_handle - DIGI device handle.
*  clear_val   - TRUE: Clear debug variables after reading.
*                FALSE: Do not clear debug variables after reading.
*
* OUTPUTS:
*  serdes_debug_ptr - pointer to storage for returned status.\n
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES: CMD_REQ_SERDES_Get_Debug_Status message.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_debug_status_get(digi_handle_t *digi_handle,
                                              serdes_debug_t *serdes_debug_ptr,
                                              BOOL clear_val)
{

    PMC_ERROR result = PMC_SUCCESS;        

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_serdes_debug_status_internal_get(digi_handle,serdes_debug_ptr,clear_val);

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_serdes_debug_status_get */

/*******************************************************************************
*  digi_serdes_port_tx_refclk_div_cfg
*  _____________________________________________________________________________
*
*  DESCRIPTION:
*   This function configures transmit reference clock divider (DIV16 or DIV64)
*   for one or multiple serdes ports associated to a serdes port:\n
*        -Line interface (S16)\n
*        -System Interface (S16)
*
*  INPUTS:
*   digi_handle             - pointer to DIGI handle instance
*   port_ctxt_ptr           - serdes port context pointer (SFI5.1 is not supported)
*   logical_lanes_msk       - the logical lane mask associated to serdes.
*   clk_divider             - clock divider (16 or 64)
*
*  OUTPUTS:
*   None
*
*  RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive
*                           error is returned.
*
*  NOTES:
*
*******************************************************************************/

/* LCOV_EXCL_START */

PUBLIC PMC_ERROR digi_serdes_port_tx_refclk_div_cfg(digi_handle_t            *digi_handle, 
                                                    digi_serdes_port_t       *port_ctxt_ptr, 
                                                    UINT32                    logical_lanes_msk, 
                                                    digi_s16_clk_divider_t    clk_divider) 
{


    digi_serdes_port_def_t *port_def_ptr;
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 pin_id;
    UINT8 i;
    UINT32 logical_lane;
    BOOL8 one_serdes_cfg = FALSE;
    BOOL8 div16_sel;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* verify that port ctxt ptr belongs to digi handle pool */
    DIGI_SERDES_PORT_CTXT_HANDLE_CHECK(digi_handle, port_ctxt_ptr, rc);

    /* cast serdes port context */
    port_def_ptr =  (digi_serdes_port_def_t*) port_ctxt_ptr;

    if (PMC_SUCCESS == rc)
    {
        switch (clk_divider)
        {
        case DIGI_S16_CLK_DIVIDER_16:
            div16_sel = TRUE;
            break;
        case DIGI_S16_CLK_DIVIDER_64:
            div16_sel = FALSE;
            break;
        default:
            rc = DIGI_ERR_INVALID_ARG;
            break;
        }
    }

    if (PMC_SUCCESS == rc)
    {
        for (i = 0; i < 32 && PMC_SUCCESS == rc; i++)
        {
            logical_lane = 1 << i;
            if ((logical_lane & logical_lanes_msk) == 0)
            {
                continue;
            }
            
            rc = digi_line_sys_serdes_pin_id_get(digi_handle, port_ctxt_ptr, i, FALSE, &pin_id);
            
            if (PMC_SUCCESS != rc)
            {
                PMC_ATOMIC_RETURN(digi_handle,rc);
            }
            
            /* Configure appropriate serdes based on interface type */
            switch (port_def_ptr->intf)
            {
            case DIGI_SERDES_SYSTEM_INTF:
            {
                s16_tx_refclk_cfg(digi_handle->s16_sys_handle, pin_id, 
                                  div16_sel);      
                break;
            }
            case DIGI_SERDES_LINE_INTF:
            {
                s16_tx_refclk_cfg(digi_handle->s16_line_handle, pin_id,
                                  div16_sel);      
                break;
            }
            default:
                rc = DIGI_ERR_INVALID_ARG;
                break;
            }

            if (PMC_SUCCESS == rc)
            {
                one_serdes_cfg = TRUE;
            }
        }
        
        if (FALSE == one_serdes_cfg && PMC_SUCCESS == rc)
        {
            rc = DIGI_ERR_INVALID_ARG;
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);    
} /* digi_serdes_port_tx_refclk_div_cfg */

/*******************************************************************************
*  digi_serdes_sfi51_loop_timing_cfg
*  _____________________________________________________________________________
*
*  DESCRIPTION:
*   This funtion configures sfi5.1 serdes loop timing
*
*  INPUTS:
*   digi_handle      - pointer to DIGI handle instance
*   port_ctxt_ptr    - serdes port context pointer (SFI5.1)
*   enable            - enable or disable
*
*  OUTPUTS:
*   None
*
*  RETURNS:
*   PMC_SUCCESS       - when API execution is successful otherwise a descriptive
*                       error is returned.
*
*  NOTES:
*   This function is used to enable loop timing when using lineotn mld
*   facility loopback.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_sfi51_loop_timing_cfg(digi_handle_t            *digi_handle, 
                                                   digi_serdes_port_t       *port_ctxt_ptr, 
                                                   BOOL                      enable)
{

    digi_serdes_port_def_t *port_def_ptr = (digi_serdes_port_def_t*) port_ctxt_ptr;
    PMC_ERROR result = PMC_SUCCESS;
    BOOL8 prov_status;
    digi_serdes_port_mode_t port_mode;
    digi_serdes_prov_state_t prov_state;
    sfi51_d8_tx_18x_port_mode_t d8_mode; 
    UINT32 i;
    BOOL8 disable_fw_proc = FALSE;
    odu_struct_t                 *odu_ptr = NULL;
    hostmsg_opsa_subsystem_cfg_t subsystem = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_FO1;
    util_global_switch_data_def_t *dummy_switch_ptr = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_def_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* get serdes provioning state */
    result = digi_serdes_port_rate_prov_status_internal_get(digi_handle, port_ctxt_ptr,
                                                            TRUE, 
                                                            &prov_status, &port_mode, 
                                                            &prov_state);
    if(PMC_SUCCESS == result && FALSE == prov_status)
    {
        result = DIGI_ERR_SERDES_PORT_NOT_PROV;
    }

    if (PMC_SUCCESS != result)
    {
        PMC_ATOMIC_RETURN(digi_handle,result);
    }

    port_def_ptr = (digi_serdes_port_def_t *)port_ctxt_ptr;    
    PMC_ASSERT(port_def_ptr->intf == DIGI_SERDES_SFI_51_INTF, DIGI_ERR_SERDES_PORT_NOT_PROV,0,0);
        
    for (i = 0; i < DIGI_NUM_SFI51_PORTS_MAX; i++)
    {
        if (digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].port_uid == port_def_ptr->port_uid)
            break;
    }
    if (i >=  DIGI_NUM_SFI51_PORTS_MAX)
    {
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_SERDES_PORT_NOT_PROV);
    }

    disable_fw_proc = digi_disable_fw_acb_proc(digi_handle, 
                                               port_ctxt_ptr,
                                               &odu_ptr,
                                               &dummy_switch_ptr,
                                               &subsystem);
    /*
     * This call is an OTN centric call. digi_disable_fw_acb_proc could
     * return non-OTN data. Add a check to ensure that this returns OTN 
     * expected data.
     */
    if (TRUE == disable_fw_proc &&
        (NULL == odu_ptr || NULL != dummy_switch_ptr ||
        !(OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_FO1 == subsystem ||
          OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_FO2 == subsystem ||
          OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_CTL == subsystem)))
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "NON-OTN subsystem=%d returned or ODU_PTR is NULL\n", 
                subsystem);
        PMC_ATOMIC_RETURN(digi_handle, DIGI_ERR_INVALID_ARG);
    }
    
    if (TRUE == enable && disable_fw_proc == TRUE)
    {
        digi_opsa_cfg_mpmo_one_port_drop(digi_handle,subsystem,odu_ptr->switch_data.oduk_data.channel);
    }

    switch(port_mode)
    {
    case DIGI_SERDES_PORT_MODE_D8_OTU3: 
        d8_mode = SFI51_D8_TX_18X_OTU3; 
        break;
    case DIGI_SERDES_PORT_MODE_D8_OTU3E1:
        d8_mode = SFI51_D8_TX_18X_OTU3E1; 
        break; 
    case DIGI_SERDES_PORT_MODE_D8_OTU3E2:
        d8_mode = SFI51_D8_TX_18X_ODU3E2; 
        break;
    case DIGI_SERDES_PORT_MODE_D8_OC_768: 
        d8_mode = SFI51_D8_TX_18X_OC_768; 
        break;
    default:
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_SERDES_RATE);
    }
    
    result = sfi51_d8_tx_18x_loop_timing_cfg(digi_handle->sfi51_d8_tx_18x_handle[i],d8_mode,enable);

    if (FALSE == enable && disable_fw_proc == TRUE)
    {
        digi_otn_remove_nominal_replacement(digi_handle,odu_ptr);
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);
                    
} /* digi_serdes_sfi51_loop_timing_cfg */

/* LCOV_EXCL_STOP */

/*******************************************************************************
*  digi_serdes_vert_hist_get
*  _____________________________________________________________________________
*
*  DESCRIPTION:
*   This function retrieves S16 or T8 vertical histogram information.
*
*  INPUTS:
*   digi_handle             - Pointer to DIGI handle instance
*   s16_line_lane_0_11_msk  - Bitmask of s16 line lanes 0-11 to configure.
*                             Bit 0 corresponds to lane 0.
*   s16_sys_lane_0_15_msk   - Bitmask of s16 sys lanes 0-15 to configure.
*                             Bit 0 corresponds to lane 0.
*   t8_lane_0_19_msk        - Bitmask of t8 lanes 0-19 to configure.
*                             Bit 0 corresponds to t8 lane 0.
*   t8_lane_20_39_msk       - Bitmask of t8 lanes 20-39 to configure.
*                             Bit 0 corresponds to t8 lane 20.
*   test_length             - Enum,requested the number of samples to capture\n
*                             UTIL_VHIST_DEBUG\n
*                             UTIL_VHIST_SHORT\n
*                             UTIL_VHIST_LONG\n
*                             UTIL_VHIST_USERDEF
*   userdef_length          - The vertical histgram counter-timer limit value
*                             that is defined by user.
*                             This parameter is only valid if test_lenth is set
*                             to UTIL_VHIST_USERDEF.
*
*  OUTPUTS:
*   s16_line_bin_buffer  - Array to vertical histogram value for s16 line lanes
*   s16_sys_bin_buffer   - Array to vertical histogram value for s16 sys lanes
*   t8_bin_buffer        - Array to vertical histogram value for t8 lanes
*
*  RETURNS:
*   PMC_SUCCESS             -  on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR  digi_serdes_vert_hist_get(digi_handle_t *digi_handle,
                                            UINT32                    s16_line_lane_0_11_msk,
                                            UINT32                    s16_sys_lane_0_15_msk,
                                            UINT32                    t8_lane_0_19_msk,
                                            UINT32                    t8_lane_20_39_msk,
                                            util_vhist_test_length_t  test_length,
                                            UINT64                    userdef_length,
                                            UINT64                    s16_line_bin_buffer[][64],
                                            UINT64                    s16_sys_bin_buffer[][64],
                                            UINT64                    t8_bin_buffer[][64])
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    if(rc == PMC_SUCCESS) {
        rc = s16_vert_hist_get(digi_handle->s16_line_handle, 
                               s16_line_lane_0_11_msk, 
                               test_length,
                               userdef_length,
                               s16_line_bin_buffer);
    }

    if(rc == PMC_SUCCESS) {
        rc = s16_vert_hist_get(digi_handle->s16_sys_handle, 
                               s16_sys_lane_0_15_msk, 
                               test_length,
                               userdef_length,
                               s16_sys_bin_buffer);
    }

    if(rc == PMC_SUCCESS) {
        rc = t8_vert_hist_get(digi_handle->bp_lr_t8_handle,
                              t8_lane_0_19_msk,
                              t8_lane_20_39_msk,
                              test_length,
                              userdef_length,
                              t8_bin_buffer);
    }

    PMC_ATOMIC_RETURN(digi_handle, rc);
}

/*******************************************************************************
*  digi_serdes_vert_eye_opening_calc
*  _____________________________________________________________________________
*
*  DESCRIPTION:
*   This function computes the vertical eye opening in number of bins
*   for the S16 or T8 SerDes.
*
*  INPUTS:
*   digi_handle             - Pointer to DIGI handle instance
*   s16_line_lane_0_11_msk  - Bitmask of s16 line lanes 0-11 to configure.
*                             Bit 0 corresponds to lane 0.
*   s16_sys_lane_0_15_msk   - Bitmask of s16 sys lanes 0-15 to configure.
*                             Bit 0 corresponds to lane 0.
*   t8_lane_0_19_msk        - Bitmask of t8 lanes 0-19 to configure.
*                             Bit 0 corresponds to t8 lane 0.
*   t8_lane_20_39_msk       - Bitmask of t8 lanes 20-39 to configure.
*                             Bit 0 corresponds to t8 lane 20.
*   s16_line_bin_buffer     - Array to vertical histogram value for s16 line lanes
*   s16_sys_bin_buffer      - Array to vertical histogram value for s16 sys lanes
*   t8_bin_buffer           - Array to vertical histogram value for t8 lanes
*
*  OUTPUTS:
*   s16_line_port_verteye_opening - pointer to vertical eye opening value for s16 line lanes
*   s16_sys_port_verteye_opening - pointer to vertical eye opening value for s16 sys lanes
*   t8_port_verteye_opening - pointer to vertical eye opening value for t8 lanes
*
*  RETURNS:
*   PMC_SUCCESS -  on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PUBLIC void digi_serdes_vert_eye_opening_calc(digi_handle_t *digi_handle, 
                                              UINT32  s16_line_lane_0_11_msk,
                                              UINT32  s16_sys_lane_0_15_msk,
                                              UINT32  t8_lane_0_19_msk,
                                              UINT32  t8_lane_20_39_msk,
                                              UINT64  s16_line_bin_buffer[][64],
                                              UINT64  s16_sys_bin_buffer[][64],
                                              UINT64  t8_bin_buffer[][64],
                                              UINT32  s16_line_port_verteye_opening[12],
                                              UINT32  s16_sys_port_verteye_opening[16],
                                              UINT32  t8_port_verteye_opening[40])
{
    UINT64 all_lane_msk = 0;
    UINT32 i;
    UINT64 msk = 0;
    UINT64 flag_one = 0;
#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif
    for(i = 0; i < DIGI_SERDES_XFI_LANES_MAX; i++) {
        msk = 0x1 << i;
        if(s16_line_lane_0_11_msk & msk) {
            util_global_vert_eye_opening_calc(s16_line_bin_buffer[i], &s16_line_port_verteye_opening[i]);
        } else {
            s16_line_port_verteye_opening[i] = 0xFF;
        }
    }

    for(i = 0; i < DIGI_SERDES_MAX_S16_IN_BP; i++) {
        msk = 0x1 << i;
        if(s16_sys_lane_0_15_msk & msk) {
            util_global_vert_eye_opening_calc(s16_sys_bin_buffer[i], &s16_sys_port_verteye_opening[i]);
        } else {
            s16_sys_port_verteye_opening[i] = 0xFF;
        }
    }

    all_lane_msk = t8_lane_20_39_msk;
    all_lane_msk = (all_lane_msk << 20) + (UINT64)t8_lane_0_19_msk;
    flag_one = 1;
    for(i = 0; i < DIGI_SERDES_MAX_T8_E1_IN_BP_LR; i++) {
        /* Immediate can not shift more than 30 bit. Need use one uint64 valiable to handle the shift */
        msk = flag_one << i;
        if(all_lane_msk & msk) {
            util_global_vert_eye_opening_calc(t8_bin_buffer[i], &t8_port_verteye_opening[i]);
        } else {
            t8_port_verteye_opening[i] = 0xFF;
        }
    }
    
    PMC_ATOMIC_RETURN(digi_handle);
    
} /* digi_serdes_vert_eye_opening_calc */


#ifndef DOXYGEN_PUBLIC_ONLY

/*******************************************************************************
* digi_serdes_single_s16_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to configure a single S16 SERDES on 
*  the line or system side.
*
* INPUTS:
*  digi_handle            - DIGI device handle.
*  s16_intf_type          - S16 interface type
*  serdes_id              - SEREDES identifier
*  serdes_rate            - SERDES rate to configure
*  enable                 - TRUE to enable, FALSE to disable
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_single_s16_cfg(digi_handle_t *digi_handle, 
                                             hostmsg_s16_intf_type_t s16_intf_type, 
                                             UINT8 serdes_id,
                                             digi_serdes_port_mode_t serdes_rate, 
                                             BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;        

    digi_s16_serdes_status_t s16_serdes_status;
    digi_s16_serdes_status_t *s16_serdes_status_ptr  = &s16_serdes_status;
    UINT32 poll_itr, max_poll_itr = 1000;
    PMC_ENTRY();

#if  defined(PMC_SW_SIMULATION)
    PMC_RETURN(PMC_SUCCESS);
#endif
    
    PMC_MEMSET(s16_serdes_status_ptr,0,sizeof(digi_s16_serdes_status_t));
    /* execute step 0 when we enable, we don't care when we disable */
    result = digi_serdes_internal_single_s16_cfg(digi_handle, s16_intf_type, serdes_id, serdes_rate, 0, enable);

    if (PMC_SUCCESS == result && enable == TRUE)
    {
        /* STEP 0  polling */
        result = digi_serdes_internal_single_s16_status_get(digi_handle, s16_intf_type, serdes_id,&s16_serdes_status);
        poll_itr = 0;
        while (PMC_SUCCESS == result && poll_itr < max_poll_itr && 
               (s16_serdes_status.csu0_vcoc_done_v == 0))
        {
            poll_itr += 1;
            /* release mutex and wait 1250 us  */
            PMC_ATOMIC_YIELD(digi_handle,1250);             
            result = digi_serdes_internal_single_s16_status_get(digi_handle, s16_intf_type, serdes_id,&s16_serdes_status);
        }
        if (PMC_SUCCESS == result && poll_itr >= max_poll_itr)
        {
            result = HOSTMSG_ERR_TIMEOUT;
        }
        /* ready for step 1 */
        if (PMC_SUCCESS == result)
        {
            result = digi_serdes_internal_single_s16_cfg(digi_handle, s16_intf_type, serdes_id, serdes_rate, 1, enable);         
        }
        if (PMC_SUCCESS == result)
        {
            result = digi_serdes_internal_single_s16_status_get(digi_handle, s16_intf_type, serdes_id,&s16_serdes_status);
            poll_itr = 0;
            while (PMC_SUCCESS == result && poll_itr < max_poll_itr && 
                   s16_serdes_status.csu0_lock_gain_v  == 0)
            {
                poll_itr += 1;
                /* release mutex and wait 1250 us  */
                PMC_ATOMIC_YIELD(digi_handle,1250);             
                result = digi_serdes_internal_single_s16_status_get(digi_handle, s16_intf_type, serdes_id,&s16_serdes_status);
            }
            if (PMC_SUCCESS == result && poll_itr  >= max_poll_itr)
            {
                result = HOSTMSG_ERR_TIMEOUT;
            }            
        }
        /* ready for step 2 */
        if (PMC_SUCCESS == result)
        {
            result = digi_serdes_internal_single_s16_cfg(digi_handle, s16_intf_type, serdes_id, serdes_rate, 2, enable);         
        }
        if (PMC_SUCCESS == result)
        {
            result = digi_serdes_internal_single_s16_status_get(digi_handle, s16_intf_type, serdes_id,&s16_serdes_status);
            poll_itr = 0;
            while (PMC_SUCCESS == result && poll_itr < max_poll_itr && 
                   s16_serdes_status.all_calib_done_v  == 0)
            {
                poll_itr += 1;
                /* release mutex and wait 1250 us  */
                PMC_ATOMIC_YIELD(digi_handle,1250);             
                result = digi_serdes_internal_single_s16_status_get(digi_handle, s16_intf_type, serdes_id,&s16_serdes_status);
            }
            if (PMC_SUCCESS == result && poll_itr  >= max_poll_itr)
            {
                result = HOSTMSG_ERR_TIMEOUT;
            }            
        }
    }
     
    PMC_RETURN(result);

} /* End: digi_serdes_single_s16_cfg() */

/*******************************************************************************
* digi_serdes_internal_single_s16_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to configure a single S16 SERDES on 
*  the line or system side.
*
* INPUTS:
*  digi_handle            - DIGI device handle.
*  s16_intf_type          - S16 interface type
*  serdes_id              - SEREDES identifier
*  serdes_rate            - SERDES rate to configure
*  step                   - configuration step
*  enable                 - TRUE to enable, FALSE to disable
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_internal_single_s16_cfg(digi_handle_t *digi_handle, 
                                                      hostmsg_s16_intf_type_t s16_intf_type, 
                                                      UINT8 serdes_id,
                                                      digi_serdes_port_mode_t serdes_rate, 
                                                      UINT8 step,
                                                      BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;        
    DOUBLE s16_lane_serdes_rate;
    BOOL8 s16_otu_client;
    hostmsg_s16_serdes_rate_t s16_serdes_rate;
    s16_port_mode_t  s16_port_mode_dummy;
    UINT32 data[2];

    PMC_ENTRY();
    
    /* Convert SERDES rate to host message parameters */
    result = digi_serdes_s16_rate_convert(serdes_rate, &s16_serdes_rate,&s16_lane_serdes_rate,&s16_otu_client, &s16_port_mode_dummy);
    
    data[0] = ((UINT8)s16_intf_type << 24) | ((UINT8) serdes_id << 16) | ((UINT8) step << 8) | (UINT8)s16_serdes_rate;
    data[1] = ((UINT8)enable << 24);
    
            
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_CFG_SERDES_SINGLE_S16_ST,data,2,stat);
    }

    /* Display FW debug info if the serdes config sequence failed */
    if (result != PMC_SUCCESS && enable == TRUE)
    {
        serdes_debug_t serdes_debug;
        PMC_ERROR status_get_result;
        status_get_result = digi_serdes_debug_status_internal_get(digi_handle,&serdes_debug,TRUE);
                    
        PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_S16_SERDES_CONFIG,
                serdes_debug.s16_cur_step,
                serdes_debug.s16_config_rv,
                "s16 serdes debug s16_cur_step=%d s16_cur_chnl %d s16_config_rv=%x\n", 
                serdes_debug.s16_cur_step, serdes_debug.s16_cur_chnl, serdes_debug.s16_config_rv);
    }

    PMC_RETURN(result);

} /* End: digi_serdes_internal_single_s16_cfg() */

/*******************************************************************************
* digi_serdes_internal_single_s16_status_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to gte status from a single S16 SERDES on 
*  the line or system side.
*
* INPUTS:
*  digi_handle            - DIGI device handle.
*  s16_intf_type          - S16 interface type
*  serdes_id              - SEREDES identifier
*
* OUTPUTS:
*   *s16_serdes_status_ptr  - S16 serdes status 
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_internal_single_s16_status_get(digi_handle_t *digi_handle, 
                                                             hostmsg_s16_intf_type_t s16_intf_type, 
                                                             UINT8 serdes_id,
                                                             digi_s16_serdes_status_t *s16_serdes_status_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;  
    UINT32 data[1];
    UINT32 response[3];

    PMC_ENTRY();

    /* Add message body to the previously created header. */
    data[0] = ((UINT8)s16_intf_type << 24) | ((UINT8) serdes_id << 16);
 
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_variable_length_query((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_REQ_SERDES_SINGLE_S16_STATUS_ST,data,1,response,3,NULL,stat);
    }

    if (PMC_SUCCESS == result)
    {
        /* Return requested data. */
        /* C8 data. */
        s16_serdes_status_ptr->csu0_vcoc_done_v = response[0];
        s16_serdes_status_ptr->all_calib_done_v = response[1];
        s16_serdes_status_ptr->csu0_lock_gain_v = response[2];
    }

    PMC_RETURN(result);

} /* End: digi_serdes_internal_single_s16_status_get() */

/*******************************************************************************
* digi_serdes_line_sys_patt_mon_internal_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function configures the pattern monitor of one line, system  
*   or SFI51 interface serdes. Function digi_serdes_line_sys_patt_mon_resync
*   shall be called to resynchronize patter monitor when pattern reception started.
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance
*   *port_ctxt_ptr      - serdes port context pointer
*   logical_lane        - For line and system interface,
*                         it is the lane index associated to the serdes module.
*                         For SFI51 interface, it is the device pin index. 
*   *patt_mon_cfg_ptr   - pointer to a pattern monitor configuration
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_line_sys_patt_mon_internal_prov(digi_handle_t *digi_handle,
                                                              digi_serdes_port_t *port_ctxt_ptr,
                                                              UINT8 logical_lane,
                                                              util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    

    /* Configure serdes pattern monitor */
    result = digi_serdes_line_sys_patt_mon_cfg(digi_handle, port_ctxt_ptr, logical_lane, FALSE, patt_mon_cfg_ptr);

    PMC_RETURN(result);
}/*digi_serdes_line_sys_patt_mon_internal_prov*/
    
/*******************************************************************************
* digi_serdes_line_sys_patt_mon_internal_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function stops the pattern monitor of one line, system or SFI51 
*   interface serdes. 
*
*
*
* INPUTS:
*   *digi_handle               - pointer to DIGI handle instance.
*   *port_ctxt_ptr             - serdes port context pointer.
*   logical_lane               - For line and system interface,
*                                it is the logical lane associated to the serdes.
*                                For SFI51 interface, it is the serdes pin number. 
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_line_sys_patt_mon_internal_deprov(digi_handle_t *digi_handle,
                                                                digi_serdes_port_t *port_ctxt_ptr,
                                                                UINT8 logical_lane)
{
    util_serdes_patt_mon_cfg_t patt_mon_cfg;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    
    /* disable pattern monitor */
    patt_mon_cfg.patt_cfg.enable = FALSE;

    /* Configure serdes pattern monitor */
    result = digi_serdes_line_sys_patt_mon_cfg(digi_handle, port_ctxt_ptr,
                                               logical_lane, FALSE, 
                                               &patt_mon_cfg);


    PMC_RETURN(result);
}/*digi_serdes_line_sys_patt_mon_internal_deprov*/
   

/*******************************************************************************
* digi_serdes_patt_line_sys_mon_err_count_internal_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function returns pattern monitor status and errors counter value of
*   line, system or SFI51 interface serdes.
*
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   *port_ctxt_ptr        - serdes port context pointer.
*   logical_lane          - For line and system interface, it is the lane
*                           index associated to the serdes module.
*                           For SFI51 interface, it is the device pin index.  
*
* OUTPUTS:
*   *status               - Pattern monitor status: Locked, trying to lock, etc.                  
*   *err_cnt              - Number of errors detected by pattern monitor. 
*                           The value shall be ignored when pattern monitor
*                           is not in locked state.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_line_sys_patt_mon_err_count_internal_get(digi_handle_t *digi_handle,
                                                                       digi_serdes_port_t *port_ctxt_ptr,
                                                                       UINT8 logical_lane,
                                                                       util_patt_status_t    *status,
                                                                       UINT32 *err_cnt)
{    
    digi_serdes_port_def_t *port_def_ptr; 
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 pin;
    UINT8 sfi51_id;
    UINT8 serdes_id;
    BOOL8 prov_status;
    digi_serdes_prov_state_t prov_state;
    digi_serdes_port_mode_t tmp_port_mode;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(err_cnt != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    

    /* get serdes provioning state and return error when serdes port is not provisioned */
    result = digi_serdes_port_rate_prov_status_internal_get(digi_handle, port_ctxt_ptr,
                                                            TRUE, 
                                                            &prov_status, &tmp_port_mode, &prov_state);
    if ( PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }
    if(FALSE == prov_status)
    {
        result = DIGI_ERR_SERDES_PORT_NOT_PROV;
    }
        
    /* find serdes pin id */
    result = digi_line_sys_serdes_pin_id_get(digi_handle, port_ctxt_ptr, 
                                             logical_lane, TRUE, &pin);
    if (PMC_SUCCESS == result)
    {    
        /* cast serdes port context */
        port_def_ptr = (digi_serdes_port_def_t *)port_ctxt_ptr;    
    
        /* get counter according to interface type */    
        switch (port_def_ptr->intf)
        {
        case DIGI_SERDES_LINE_INTF:
            result = s16_patt_mon_err_count_get(digi_handle->s16_line_handle, pin, status, err_cnt);
            break;
        case DIGI_SERDES_SYSTEM_INTF:
            result = s16_patt_mon_err_count_get(digi_handle->s16_sys_handle, pin, status, err_cnt);
            break;
        case DIGI_SERDES_SFI_51_INTF:  
            result = digi_serdes_sfi51_serdes_pin_id_get(digi_handle, port_ctxt_ptr,
                                                         pin, TRUE, &sfi51_id, &serdes_id); 
            if (PMC_SUCCESS == result)
            {          
                result = sfi51_d8_rx_18x_patt_mon_err_count_get(digi_handle->sfi51_d8_rx_18x_handle[sfi51_id], 
                                                                serdes_id, 
                                                                status, err_cnt);
            }
            break;
        default:
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
            break;
        }    
    }

    PMC_RETURN(result);
}/*digi_serdes_patt_mon_err_count_internal_get*/

/*******************************************************************************
* digi_serdes_patt_line_sys_polarity_internal_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function returns serdes poalrity associated to given logical lane.
*
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   *port_ctxt_ptr        - serdes port context pointer. 
*   dir                   - logical lane direction. TRUE = RX, FALSE = TX.
*   logical_lane          - For line and system interface, it is the lane
*                           index associated to the serdes module.
*                           For SFI51 interface, it is the device pin index. 
*
* OUTPUTS:
*   *polarity_inv         - TRUE when polarity is inverted, otherwise FALSE.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_line_sys_polarity_internal_get(digi_handle_t            *digi_handle,
                                                             digi_serdes_port_t       *port_ctxt_ptr,
                                                             UINT8                     logical_lane,
                                                             util_global_direction_t   dir,
                                                             BOOL8                     *polarity_inv)
{    
    digi_serdes_port_def_t *port_def_ptr; 
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 pin;
    BOOL8 prov_status;
    digi_serdes_prov_state_t prov_state;
    digi_serdes_port_mode_t tmp_port_mode;
    UINT8 sfi51_id;
    UINT8 serdes_id;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    if (LAST_UTIL_GLOBAL_DIR == dir ||
        UTIL_GLOBAL_DIR_BOTH == dir)
    {
        result =  DIGI_ERR_INVALID_ARG;
    }

    if ( PMC_SUCCESS == result)
    {
        /* get serdes provioning state and return error when serdes port is not provisioned */
        result = digi_serdes_port_rate_prov_status_internal_get(digi_handle, port_ctxt_ptr, 
                                                                TRUE,
                                                                &prov_status, &tmp_port_mode, &prov_state);
        
        if (PMC_SUCCESS == result)
        {
            if (FALSE == prov_status)
            {
                result = DIGI_ERR_SERDES_PORT_NOT_PROV;
            }        
        }
    }

    if (PMC_SUCCESS == result)
    {    
        /* find serdes pin id */
        result = digi_line_sys_serdes_pin_id_get(digi_handle, port_ctxt_ptr, 
                                                 logical_lane, dir, &pin);
        if (PMC_SUCCESS == result)
        {    
            /* cast serdes port context */
            port_def_ptr = (digi_serdes_port_def_t *)port_ctxt_ptr;    
            
            /* get counter according to interface type */    
            switch (port_def_ptr->intf)
            {
            case DIGI_SERDES_LINE_INTF:
                result = s16_polarity_get(digi_handle->s16_line_handle, pin, dir, polarity_inv);
                break;
            case DIGI_SERDES_SYSTEM_INTF:
                result = s16_polarity_get(digi_handle->s16_sys_handle, pin, dir, polarity_inv);
                break;
            case DIGI_SERDES_SFI_51_INTF:  
                switch(dir)
                {
                case UTIL_GLOBAL_DIR_TX:
                    result = digi_serdes_sfi51_serdes_pin_id_get(digi_handle, port_ctxt_ptr,
                                                                 pin, FALSE, &sfi51_id, &serdes_id);  
                    if (PMC_SUCCESS == result)
                    {
                        result = sfi51_d8_tx_18x_polarity_get(digi_handle->sfi51_d8_tx_18x_handle[sfi51_id], 
                                                  serdes_id, polarity_inv);
                    }
                    break;
                case UTIL_GLOBAL_DIR_RX:
                    result = digi_serdes_sfi51_serdes_pin_id_get(digi_handle, port_ctxt_ptr,
                                                                 pin, TRUE, &sfi51_id, &serdes_id);  
                    if (PMC_SUCCESS == result)
                    {
                        result = sfi51_d8_rx_18x_polarity_get(digi_handle->sfi51_d8_rx_18x_handle[sfi51_id], 
                                                              serdes_id, polarity_inv);
                    }
                    break;
                default:
                    break;
                }
                break;
            default:
                result = DIGI_ERR_INVALID_ARG;
                break;
            }    
        }
    }

    PMC_RETURN(result);
}/*digi_serdes_line_sys_polarity_internal_get*/

/*******************************************************************************
* digi_serdes_debug_status_internal_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves debug variables for SERDES configuration.
*
* INPUTS:
*  digi_handle - DIGI device handle.
*  clear_val   - TRUE: Clear debug variables after reading.
*                FALSE: Do not clear debug variables after reading.
*
* OUTPUTS:
*  serdes_debug_ptr - pointer to storage for returned status.\n
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES: CMD_REQ_SERDES_Get_Debug_Status message.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_debug_status_internal_get(digi_handle_t *digi_handle,
                                                        serdes_debug_t *serdes_debug_ptr,
                                                        BOOL clear_val)
{
    PMC_ERROR result = PMC_SUCCESS;        
    UINT32 data[1];
    UINT32 response[17];

    PMC_ENTRY();

    /* Add message body to the previously created header. */
    data[0] = clear_val << 24;    

    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_variable_length_query((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_REQ_SERDES_GET_DEBUG_STAT_ST,data,1,response,17,NULL,stat);
    }

    if (PMC_SUCCESS == result)
    {

        /* Return requested data. */
        /* C8 data. */
        serdes_debug_ptr->c8_cur_lane     = response[0];
        serdes_debug_ptr->c8_cur_step     = response[1];
        serdes_debug_ptr->c8_config_rv    = response[2];
        /* T8 data. */
        serdes_debug_ptr->t8_cur_slice    = response[3];
        serdes_debug_ptr->t8_cur_subch    = response[4];
        serdes_debug_ptr->t8_cur_step     = response[5];
        serdes_debug_ptr->t8_config_rv    = response[6];
        /* S16 data. */
        serdes_debug_ptr->s16_cur_chnl    = response[7];
        serdes_debug_ptr->s16_cur_step    = response[8];
        serdes_debug_ptr->s16_config_rv   = response[9];
        /* SFI5.1 data. */
        serdes_debug_ptr->sfi51_cur_chnl  = response[10];
        serdes_debug_ptr->sfi51_cur_inst  = response[11];
        serdes_debug_ptr->sfi51_cur_side  = response[12];
        serdes_debug_ptr->sfi51_cur_step  = response[13];
        serdes_debug_ptr->sfi51_config_rv = response[14];
        /* D8 data. */
        serdes_debug_ptr->d8_cur_step     = response[15];
        serdes_debug_ptr->d8_config_rv    = response[16];
    }

    PMC_RETURN(result);

} /* digi_serdes_debug_status_internal_get */


/*******************************************************************************
* digi_sys_serdes_pin_use_collision_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function tests for collision of resouces assigned to system serdes
*   pins.
*
* INPUTS:
*   *digi_handle   - pointer to DIGI handle instance.
*   sifd_1_cfg_ptr - structure of ILKN1 pins associated with Serdes
*   sifd_2_cfg_ptr - structure of ILKN2 pins associated with Serdes
*
* OUTPUTS:
*
*
* RETURNS:
*   BOOL                    - TRUE: Pin Usage Collision Detected \n
*                             FALSE: No Pin Usage Collision Detected
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL digi_sys_serdes_pin_use_collision_test(digi_handle_t *digi_handle, 
                                                   digi_sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                                   digi_sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr)
{
    BOOL collision = FALSE;
    UINT32 n1_optic_pin_msk = 0, n1_ilkn1_pin_msk = 0, n1_ilkn2_pin_msk = 0;
    digi_sifd_ilkn_1_lane_cfg_t ilkn_1_lane_cfg;
    digi_sifd_ilkn_2_lane_cfg_t ilkn_2_lane_cfg;

    PMC_ENTRY();
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);    

    if(sifd_1_cfg_ptr == NULL)
    {
        ilkn_1_lane_cfg.blk_to_E1_enable_mask = 0;
        ilkn_1_lane_cfg.blk_to_N1_enable_mask = 0;
        ilkn_1_lane_cfg.blk_to_N2_enable_mask = 0;
    } else {
        ilkn_1_lane_cfg.blk_to_E1_enable_mask = sifd_1_cfg_ptr->lane.blk_to_E1_enable_mask;
        ilkn_1_lane_cfg.blk_to_N1_enable_mask = sifd_1_cfg_ptr->lane.blk_to_N1_enable_mask;
        ilkn_1_lane_cfg.blk_to_N2_enable_mask = sifd_1_cfg_ptr->lane.blk_to_N2_enable_mask;
    }

    if(sifd_2_cfg_ptr == NULL)
    {
        ilkn_2_lane_cfg.blk_to_E1_enable_mask = 0;
        ilkn_2_lane_cfg.blk_to_N1_enable_mask = 0;
        ilkn_2_lane_cfg.blk_to_N2_enable_mask = 0;
    } else {
        ilkn_2_lane_cfg.blk_to_E1_enable_mask = sifd_2_cfg_ptr->lane.blk_to_E1_enable_mask;
        ilkn_2_lane_cfg.blk_to_N1_enable_mask = sifd_2_cfg_ptr->lane.blk_to_N1_enable_mask;
        ilkn_2_lane_cfg.blk_to_N2_enable_mask = sifd_2_cfg_ptr->lane.blk_to_N2_enable_mask;
    }

    n1_optic_pin_msk = digi_sys_serdes_n1_optic_pin_use_get(digi_handle);

    /* normalize ilkn1_blk_to_N1_enable_mask to N1 pins 15:0 */
    /* ILKN1[15:0] with mask 0xFFFF and offset 0 map to 
       N1[15:0] with offset 0 */
    n1_ilkn1_pin_msk = ilkn_1_lane_cfg.blk_to_N1_enable_mask & 0xFFFF;

    /* normalize ilkn2_blk_to_N1_enable_mask to N1 pins 15:0 */
    /* ILKN2[7:0] with mask 0xFF and offset 0 map to 
       N1[15:8] with offset 8 */
    n1_ilkn2_pin_msk = (ilkn_2_lane_cfg.blk_to_N1_enable_mask & 0xFF) << 8;

    /* Confirm System Serdes usage with cage/ILKN1/ILKN2 do not overlap */
    if((n1_optic_pin_msk & n1_ilkn1_pin_msk) != 0)
    {
        collision = TRUE;    
        PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_SERDES_PIN_ALLOC_COLLISION, 0, 0); 
    }
    if((n1_optic_pin_msk & n1_ilkn2_pin_msk) != 0)
    {
        collision = TRUE;    
        PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_SERDES_PIN_ALLOC_COLLISION, 0, 0); 
    }
    if((n1_ilkn1_pin_msk & n1_ilkn2_pin_msk) != 0)
    {
        collision = TRUE;    
        PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_SERDES_PIN_ALLOC_COLLISION, 0, 0); 
    }

    /* Confirm E1 pin usage for ILKN1/ILKN2 does not overlap */
    /* ILKN1 uses E1[39:20] ILKN2 uses E1[23:0] the overlapping set is E1[23:20]
       ILKN1[16:19] maps to E1[23:20]
       ILKN2[23:20] maps to E1[23:20]
    */
    /* ILKN1[16] <-> ILKN2[23] */
    if(((ilkn_1_lane_cfg.blk_to_E1_enable_mask & 0x10000) != 0) && ((ilkn_2_lane_cfg.blk_to_E1_enable_mask & 0x800000) != 0))
    {
        collision = TRUE;    
        PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_SERDES_PIN_ALLOC_COLLISION, 0, 0); 
    }
    /* ILKN1[17] <-> ILKN2[22] */
    if(((ilkn_1_lane_cfg.blk_to_E1_enable_mask & 0x20000) != 0) && ((ilkn_2_lane_cfg.blk_to_E1_enable_mask & 0x400000) != 0))
    {
        collision = TRUE;    
        PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_SERDES_PIN_ALLOC_COLLISION, 0, 0); 
    }
    /* ILKN1[18] <-> ILKN2[21] */
    if(((ilkn_1_lane_cfg.blk_to_E1_enable_mask & 0x40000) != 0) && ((ilkn_2_lane_cfg.blk_to_E1_enable_mask & 0x200000) != 0))
    {
        collision = TRUE;    
        PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_SERDES_PIN_ALLOC_COLLISION, 0, 0); 
    }
    /* ILKN1[19] <-> ILKN2[20] */
    if(((ilkn_1_lane_cfg.blk_to_E1_enable_mask & 0x80000) != 0) && ((ilkn_2_lane_cfg.blk_to_E1_enable_mask & 0x100000) != 0))
    {
        collision = TRUE;    
        PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_SERDES_PIN_ALLOC_COLLISION, 0, 0); 
    }



    PMC_RETURN(collision);

} /* digi_sys_serdes_pin_use_collision_test */

/*******************************************************************************
* digi_sys_serdes_bp_csu_multi_rate_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function test E1 and N2 bac kplane serdes to determine if the
*   resources configured against each of these interfaces share a common
*   rate.\n\n
*
*   The device architecture has one CSU per 20 E1 pins and one CSU for the
*   N2 Serdes.  As such, mixed rates within the E1 and N2 Serdes interfaces
*   is not supported.
*
* INPUTS:
*   *digi_handle              - pointer to DIGI handle instance.
*   *sifd_1_cfg_ptr           - pointer to ILKN1 interface context
*   *sifd_2_cfg_ptr           - pointer to ILKN2 interface context
*
* OUTPUTS:
*
*                                    
* RETURNS:                           
*   BOOL                      - Detection of mixed rate CSU for BP Serdes
*                               detected\n
*                               TRUE: More than one rate requested for a BP
*                               Serdes interface\n
*                               FALSE: One rate requested for a BP Serdes
*                               interface
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL digi_sys_serdes_bp_csu_multi_rate_test(digi_handle_t *digi_handle,
                                                   digi_sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                                   digi_sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr)
{
    digi_ilkn_serdes_rate_t ilkn_1_rate = LAST_DIGI_ILKN_SERDES_RATE;
    UINT32 ilkn1_e1_39_20_msk = 0;
    UINT32 ilkn1_e1_19_00_msk = 0;
    UINT32 ilkn1_n2_msk = 0;
    digi_ilkn_serdes_rate_t ilkn_2_rate = LAST_DIGI_ILKN_SERDES_RATE;
    UINT32 ilkn2_e1_39_20_msk = 0;
    UINT32 ilkn2_e1_19_00_msk = 0;
    UINT32 ilkn2_n2_msk = 0;
    UINT32 dummy = 0;
    BOOL mix = FALSE;


    PMC_ENTRY();

    digi_ilkn_serdes_pin_use_get(digi_handle,
                                 sifd_1_cfg_ptr,
                                 sifd_2_cfg_ptr,
                                 &ilkn_1_rate,
                                 &ilkn_2_rate,
                                 &ilkn1_e1_39_20_msk,
                                 &ilkn1_e1_19_00_msk,
                                 &dummy,
                                 &ilkn1_n2_msk,
                                 &ilkn2_e1_39_20_msk,
                                 &ilkn2_e1_19_00_msk,
                                 &dummy,
                                 &ilkn2_n2_msk);

    if(sifd_1_cfg_ptr != NULL && sifd_2_cfg_ptr != NULL)
    {
        if(ilkn_1_rate != ilkn_2_rate)
        {
            /* Determine if rate overlap on E1[39:20] pins */
            if(ilkn1_e1_39_20_msk != 0 && ilkn2_e1_39_20_msk != 0)
            {
                mix = TRUE;
                PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_SERDES_CSU_MULTIRATE_REQUEST, 0, 0); 
            }
        
            /* Determine if rate overlap on E1[19:00] pins */
            if(ilkn1_e1_19_00_msk != 0 && ilkn2_e1_19_00_msk != 0)
            {
                mix = TRUE;
                PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_SERDES_CSU_MULTIRATE_REQUEST, 0, 0); 
            }
        
            /* Determine if rate overlap of N2[7:0] */
            if(ilkn1_n2_msk != 0 && ilkn2_n2_msk != 0)
            {
                mix = TRUE;
                PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_SERDES_CSU_MULTIRATE_REQUEST, 0, 0); 
            }
        }
    }



    PMC_RETURN(mix);

} /* digi_sys_serdes_bp_csu_multi_rate_test */

/*******************************************************************************
* digi_serdes_line_cage_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a helper function for the cage configuration in the xfi interface
*   case.
*   See the description for digi_serdes_cage_cfg for more general information.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   cage_id_per_pin        - Array defining upto 12 unique
*                            cages connected to interface specified with intf.
*                            The array is indexed by serdes pin number.  Should
*                            a pin not be associated with a cage, the value
*                            DIGI_SERDES_PIN_NOT_USED (0xFF) may be used to
*                            define 'Not Connected'.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS on successful configuration, an error code otherwise.
*
* NOTES:
*   This function may only be called once per DIGI instance.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_line_cage_cfg(digi_handle_t *digi_handle,
                                           UINT8 cage_id_per_pin[DIGI_SERDES_XFI_LANES_MAX])
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    BOOL is_cfp = FALSE;
    UINT8 i = 0; /* counter */

    PMC_ENTRY();

    if (digi_handle->var.cage_cfg.is_line_cage_cfg == TRUE) {

        rc = DIGI_ERR_SERDES_ALL_CAGE_ALREADY;
    } else {

        /* Check to see if the cage_id_per_pin is sane before proceeding */
        rc = digi_line_cage_validate(digi_handle, cage_id_per_pin, &is_cfp);
    }

    if (rc == PMC_SUCCESS) {

        for (i = 0; i < DIGI_SERDES_XFI_LANES_MAX; i++) {

            digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].cage_id = cage_id_per_pin[i];
        }

        digi_handle->var.cage_cfg.is_line_cage_cfg = TRUE;
        digi_handle->var.cage_cfg.is_line_cfp = is_cfp;
        rc = PMC_SUCCESS;
    }

    PMC_RETURN(rc);

} /* digi_serdes_line_xfi_cage_cfg */

/*******************************************************************************
* digi_serdes_sys_cage_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a helper function for the cage configuration in the xfi interface
*   case.
*   See the description for digi_serdes_cage_cfg for more general information.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   cage_id_per_pin        - Array defining upto 12 unique
*                            cages connected to interface specified with intf.
*                            The array is indexed by serdes pin number.  Should
*                            a pin not be associated with a cage, the value
*                            DIGI_SERDES_PIN_NOT_USED (0xFF) may be used to
*                            define 'Not Connected'.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS on successful configuration, an error code otherwise.
*
* NOTES:
*   This function may only be called once per DIGI instance.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_sys_cage_cfg(digi_handle_t *digi_handle,
                                          UINT8 cage_id_per_pin[DIGI_SERDES_SYS_LANES_MAX])
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    BOOL is_cfp = FALSE;
    UINT8 i = 0; /* counter */

    PMC_ENTRY();

    if (digi_handle->var.cage_cfg.is_sys_cage_cfg == TRUE) {

        rc = DIGI_ERR_SERDES_ALL_CAGE_ALREADY;
    } else {

        /* Check to see if the cage_id_per_pin is sane before proceeding */
        rc = digi_sys_cage_validate(digi_handle, cage_id_per_pin, &is_cfp);
    }

    if (rc == PMC_SUCCESS) {

        for (i = 0; i < DIGI_SERDES_SYS_LANES_MAX; i++) {

            digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].cage_id = cage_id_per_pin[i];
        }

        digi_handle->var.cage_cfg.is_sys_cage_cfg = TRUE;
        digi_handle->var.cage_cfg.is_sys_cfp = is_cfp;
        rc = PMC_SUCCESS;
    }

    PMC_RETURN(rc);

} /* digi_serdes_sys_xfi_cage_cfg */

/*******************************************************************************
* digi_serdes_sfi51_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to configure the SFI5.1 SERDES on 
*  the line side.
*
* INPUTS:
*  digi_handle      - DIGI device handle.
*  enable_msk       - Bitmask of channels to enable (0-1). 
*                     Bit 0 corresponds to lane 0.
*  serdes_port_mode - SERDES rate to configure
*  disable_msk       - Bitmask of channels to disable (0-1). 
*                      Bit 0 corresponds to lane 0.
*                      This field takes precedence over enable_msk.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_sfi51_cfg(digi_handle_t           *digi_handle, 
                                       UINT8                    enable_msk, 
                                       digi_serdes_port_mode_t  serdes_port_mode, 
                                       UINT8                    disable_msk)
{
    PMC_ERROR result = PMC_SUCCESS; 
    hostmsg_sfi51_serdes_rate_t sfi51_serdes_rate;
    BOOL8 enable;
    UINT32 enable_itr;
    UINT8 msk;
    UINT32 data[1];

    PMC_ENTRY();

    enable = enable_msk != 0?TRUE:FALSE;
    msk    = (TRUE == enable)?enable_msk:disable_msk;
    
    for (enable_itr = 0;enable_itr < 2 && result == PMC_SUCCESS; enable_itr++) 
    {
        if ((msk & (1 << enable_itr)) != 0)
        {            
            result  = digi_serdes_single_sfi51_cfg(digi_handle,  serdes_port_mode,enable_itr, enable);
        }
    }
    if (HOSTMSG_ERR_RSP_NACK == result) 
    {
        result = PMC_SUCCESS;

        /* Convert SERDES rate to host message parameters */
        result = digi_serdes_sfi51_d8_rate_convert(serdes_port_mode, &sfi51_serdes_rate);

        if (PMC_SUCCESS == result)
        {
            /* CMD_CFG_SERDES_SFI51 has the following format:
             * Word  bits 24..31   bits 16..23   bits 8..15   bits 0..7
             *   0  rate     enable_msk  disable_msk  reserved
             */
            data[0] = ((UINT8)sfi51_serdes_rate << 24) | 
                (enable_msk << 16) |
                (disable_msk << 8);
        
            {
                HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
                result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_CFG_SERDES_SFI51_ST,data,1,stat);
            }

            /* Display FW debug info if the serdes config sequence failed */
            if (result != PMC_SUCCESS && enable_msk) 
            {
                serdes_debug_t serdes_debug;
                PMC_ERROR status_get_result;
                status_get_result = digi_serdes_debug_status_internal_get(digi_handle,&serdes_debug,TRUE);
                PMC_ASSERT(status_get_result == PMC_SUCCESS,status_get_result,0,0);
            
                PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_D8_SERDES_CONFIG,
                        serdes_debug.sfi51_cur_step,
                        serdes_debug.sfi51_config_rv,
                        "SFI51 serdes debug sfi51_cur_step=%d sfi51_config_rv=%x\n", 
                        serdes_debug.sfi51_cur_step, serdes_debug.sfi51_config_rv);
            }
        }
    }
    if (PMC_SUCCESS == result)
    {
        (void)digi_serdes_sfi51_params_wr(digi_handle, enable_msk, disable_msk, serdes_port_mode);
    }

    PMC_RETURN(result);

} /* End: digi_serdes_sfi51_cfg() */

/*******************************************************************************
* digi_serdes_d8_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to configure the D8 SERDES on 
*  the system backplane.
*
* INPUTS:
*  digi_handle   - DIGI device handle.
*  d8_intf_type  - D8 Interface type to configure
*  cfg_csu       - TRUE: Configure CSU. FALSE: Do not configure CSU
*                  Since all six SERDES ports are driven by the same CSU, 
*                  the user must specify whether the CSU should be reconfigured. 
*                  Reconfiguring the CSU will temporarily disrupt the traffic on all SERDES ports
*  enable        - TRUE: The specified interface will be taken out of reset and configured. 
*                        If cfg_csu = 1, the CSU will also be reset and configured.
*                  FALSE: The specified interface and its pair SERDES will be placed in reset. 
*                         OHP0 and OHP1 will be disabled together. 
*                         OHP2 and ENET_MGMT will be disabled together. 
*                         RI_RCP and DI_RCP will be disabled together.
*                         If cfg_csu = 1, the CSU will also be placed in reset.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_d8_cfg(digi_handle_t *digi_handle, 
                                    hostmsg_d8_intf_type_enum d8_intf_type,
                                    BOOL8 cfg_csu, 
                                    BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;  
    UINT32 data[1];

    PMC_ENTRY();

    PMC_ASSERT((NULL != digi_handle), DIGI_ERR_NULL_HANDLE, 0, 0);

    result = digi_serdes_d8_multi_step_cfg(digi_handle,  d8_intf_type, cfg_csu, enable);
    if (HOSTMSG_ERR_RSP_NACK == result) 
    {
        result = PMC_SUCCESS; 

        /* CMD_CFG_SERDES_D8 has the following format:
         * Word  bits 24..31   bits 16..23   bits 8..15   bits 0..7
         *   0  interface  configure_csu  enable   reserved
         */
        data[0] = ((UINT8)d8_intf_type << 24) | 
            ((UINT32)cfg_csu << 16) |
            ((UINT32)enable << 8);

        PMC_LOG_TRACE("D8 SERDES CFG: d8 intf = %d CSU enable = %d serdes enable = %d\n",d8_intf_type,cfg_csu,enable);

        {
            HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
            result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_CFG_SERDES_D8_ST,data,1,stat);
        }

        /* Display FW debug info if the serdes config sequence failed */
        if (result != PMC_SUCCESS && enable) 
        {
            serdes_debug_t serdes_debug;
            PMC_ERROR status_get_result;
            status_get_result = digi_serdes_debug_status_internal_get(digi_handle,&serdes_debug,TRUE);
            PMC_ASSERT(status_get_result == PMC_SUCCESS,status_get_result,0,0);

            PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_D8_SERDES_CONFIG,
                    serdes_debug.d8_cur_step,
                    serdes_debug.d8_config_rv,
                    "d8 serdes debug d8_cur_step=%d d8_config_rv=%x\n", 
                    serdes_debug.d8_cur_step, serdes_debug.d8_config_rv);
        }
    }

    /* If the CSU was configured, update the CSU status */
    if (cfg_csu && result == PMC_SUCCESS) 
    {
        digi_handle->ge_intf_handle->csu_enabled = enable;
    }
    
    /* write serdes configuration parameters in device */
    if (PMC_SUCCESS == result)
    {        
        result = digi_serdes_ge_params_wr(digi_handle, 
                                          d8_intf_type,
                                          enable);
    }

    PMC_RETURN(result);
} /* End: digi_serdes_d8_cfg() */

/*******************************************************************************
* digi_serdes_port_rate_state_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function saves serdes port properties: port_mode and provisioning state.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   *port_ctxt_ptr         - serdes port context pointer. 
*   port_mode              - serdes port rate.
*   prov_state             - serdes port provisioning state to save
*   rate_prov_state        - serdes rate provisioning state to save
*
* OUTPUTS:
*   NONE.                         
*
* RETURNS:
*   PMC_SUCCESS         - when API execution is successful otherwise 
*                         a descriptive error is returned. 
*
* NOTES:
*  The function should be called only for Line/Sys S16 + SFI5.1 D8 Serdes 
*
*******************************************************************************/ 
PUBLIC PMC_ERROR digi_serdes_port_rate_state_set(digi_handle_t *digi_handle,
                                                 digi_serdes_port_t *port_ctxt_ptr,
                                                 digi_serdes_port_mode_t port_mode,
                                                 digi_serdes_prov_state_t prov_state,
                                                 digi_serdes_port_rate_prov_state_t rate_prov_state)
{
    digi_serdes_port_def_t *port_def_ptr = NULL;
    UINT32 i;
    BOOL8 found = FALSE;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    /* cast serdes port context */
    port_def_ptr = (digi_serdes_port_def_t *)port_ctxt_ptr;

    /* Provision serdes rate */        
    switch (port_def_ptr->intf)
    {
    case DIGI_SERDES_LINE_INTF:
        /* build enable mask */
        for (i = 0; i < DIGI_SERDES_XFI_LANES_MAX; i++)
        {    
            if (port_def_ptr->port_uid == digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].port_uid)
            {
                if (FALSE == digi_serdes_is_pending_state_get(rate_prov_state))
                {                    
                    digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].serdes_rate_state.port_prov_state = prov_state;
                    digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].serdes_rate_state.port_mode = port_mode;                 
                }
                if (DIGI_SERDES_PORT_RATE_STATE_RESTART_PENDING == rate_prov_state)
                {
                    digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].serdes_rate_state.rate_prov_state = digi_serdes_pending_state_transition(digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].serdes_rate_state.rate_prov_state);
                }
                else
                {
                    digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].serdes_rate_state.rate_prov_state = rate_prov_state;
                }

                found = TRUE;
            }
        }
        break;
    case DIGI_SERDES_SYSTEM_INTF:
        /* build enable mask */
        for (i = 0; i < DIGI_SERDES_SYS_LANES_MAX; i++)
        {
            if (port_def_ptr->port_uid == digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].port_uid)
            {
                if (FALSE == digi_serdes_is_pending_state_get(rate_prov_state))
                {  
                    digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].serdes_rate_state.port_prov_state = prov_state;
                    digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].serdes_rate_state.port_mode = port_mode;
                }
                if (DIGI_SERDES_PORT_RATE_STATE_RESTART_PENDING == rate_prov_state)
                {
                    digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].serdes_rate_state.rate_prov_state = digi_serdes_pending_state_transition(digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].serdes_rate_state.rate_prov_state);
                }
                else
                {
                    digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].serdes_rate_state.rate_prov_state = rate_prov_state;
                }
                found = TRUE;
            }
        }
      
        break;
    case DIGI_SERDES_SFI_51_INTF:
        /* build enable mask */
        for (i = 0; i < DIGI_NUM_SFI51_PORTS_MAX; i++)
        {
            if (port_def_ptr->port_uid == digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].port_uid)
            {
                if (FALSE == digi_serdes_is_pending_state_get(rate_prov_state))
                {  
                    digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].serdes_rate_state.port_prov_state = prov_state;
                    digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].serdes_rate_state.port_mode = port_mode;
                }
                if (DIGI_SERDES_PORT_RATE_STATE_RESTART_PENDING == rate_prov_state)
                {
                    digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].serdes_rate_state.rate_prov_state  = digi_serdes_pending_state_transition(digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].serdes_rate_state.rate_prov_state);
                }
                else
                {
                    digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].serdes_rate_state.rate_prov_state  = rate_prov_state;
                }
                                                                                                                                              
                found = TRUE;
            }        
        }
        break;
    default:
        result = DIGI_ERR_INVALID_ARG;
        break;
    }

    if (PMC_SUCCESS == result && FALSE == found)
    {
        result = DIGI_ERR_INVALID_ARG;
    }

    PMC_RETURN(PMC_SUCCESS);
}/* digi_serdes_port_rate_state_set*/

/*******************************************************************************
* digi_serdes_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function cleanup serdes register space and context.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*
* OUTPUTS:
*   NONE.                         
*
* RETURNS:
*   NONE. 
*
* NOTES:
*  The function should be called only for Line/Sys S16 + SFI5.1 D8 Serdes 
*
*******************************************************************************/ 
PUBLIC void digi_serdes_cleanup(digi_handle_t *digi_handle)
{
    UINT32 i;
    UINT16 dis_mask = 0;

    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* build enable mask */
    for (i = 0; i < DIGI_SERDES_XFI_LANES_MAX; i++)
    {   
        if (DIGI_SERDES_PROV_STATE_UNASSIGNED == digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].serdes_rate_state.port_prov_state)
        {
            if (DIGI_SERDES_UNASSIGNED != digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].port_uid && 
                FALSE == digi_serdes_is_serdes_connect_to_enet_pmon(digi_handle,
                                                                    DIGI_SERDES_LINE_INTF,
                                                                    digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt,
                                                                    digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].port_uid))
            {
                dis_mask |= (1 << i);  
                (void) digi_serdes_port_lifd_sifd_subsytem_disable(digi_handle,digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].port_uid);
            }
        }
        else if (TRUE == digi_serdes_is_pending_state_get(digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].serdes_rate_state.rate_prov_state))
        {
            s16_restart(digi_handle->s16_line_handle, i);
            digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].serdes_rate_state.rate_prov_state = digi_serdes_pending_state_transition(digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].serdes_rate_state.rate_prov_state);
        }
    }

    /* unconfigure s16 line serdes */
    if (0 != dis_mask)
    {            
        (void)digi_serdes_s16_cfg(digi_handle, HOSTMSG_S16_SERDES_INTF_LINE, 
                                  DIGI_SERDES_MASK_NONE,
                                  DIGI_SERDES_PORT_MODE_ANY_LOWPWR,
                                  dis_mask);
    }
 
    dis_mask = 0;
    for (i = 0; i < DIGI_SERDES_SYS_LANES_MAX; i++)
    {
        if (DIGI_SERDES_PROV_STATE_UNASSIGNED == digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].serdes_rate_state.port_prov_state)
        {       
            /*check that serdes is not connected to ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON enet sys channel */
            if (TRUE == (FALSE == s16_reset_state_get(digi_handle->s16_sys_handle, i) && TRUE == s16_tx_squelch_enable_status_get(digi_handle->s16_sys_handle, i) && (i == digi_handle->var.digi_enet_pmon_GSUP43_7_3_sys_usage)))
            {
                /* don't cleanup serdes if it belongs to ENET GSUP43_7_3 T PMON channel */
            }
            else if (DIGI_SERDES_UNASSIGNED != digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].port_uid && 
                FALSE == digi_serdes_is_serdes_connect_to_enet_pmon(digi_handle,
                                                                    DIGI_SERDES_SYSTEM_INTF,
                                                                    digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt,
                                                                    digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].port_uid))
            {
                dis_mask |= (1 << i);  
         
                (void) digi_serdes_port_lifd_sifd_subsytem_disable(digi_handle,digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].port_uid);
            }
        }
        else if (TRUE == digi_serdes_is_pending_state_get(digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].serdes_rate_state.rate_prov_state))
        {
            s16_restart(digi_handle->s16_sys_handle, i);
            digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].serdes_rate_state.rate_prov_state = digi_serdes_pending_state_transition(digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].serdes_rate_state.rate_prov_state);
        }
    }
    /* unconfigure s16 sys serdes */
    if (0 != dis_mask)
    {            
        (void)digi_serdes_s16_cfg(digi_handle, HOSTMSG_S16_SERDES_INTF_SYS, DIGI_SERDES_MASK_NONE, DIGI_SERDES_PORT_MODE_ANY_LOWPWR, dis_mask);
    }

    /* build enable mask */
    dis_mask = 0;
    for (i = 0; i < DIGI_NUM_SFI51_PORTS_MAX; i++)
    {
        if (DIGI_SERDES_PROV_STATE_UNASSIGNED ==  digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].serdes_rate_state.port_prov_state)
        {
            dis_mask |= (1 << i);
        }
        else if (TRUE == digi_serdes_is_pending_state_get(digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].serdes_rate_state.rate_prov_state))
        {
            sfi51_d8_tx_18x_restart(digi_handle->sfi51_d8_tx_18x_handle[i]);
            digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].serdes_rate_state.rate_prov_state = digi_serdes_pending_state_transition(digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].serdes_rate_state.rate_prov_state);
        }        
    }

    /* unconfigure sfi51 line serdes */
    if (0 != dis_mask)
    {            
        (void)digi_serdes_sfi51_cfg(digi_handle,
                                    DIGI_SERDES_MASK_NONE, 
                                    DIGI_SERDES_PORT_MODE_ANY_LOWPWR,
                                    dis_mask);
    }
    

    PMC_RETURN();
}/* digi_serdes_cleanup */

/*******************************************************************************
* digi_ilkn_serdes_pin_use_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function test E1 and N2 bac kplane serdes to determine if the
*   resources configured against each of these interfaces share a common
*   rate.\n\n
*
*   The device architecture has one CSU per 20 E1 pins and one CSU for the
*   N2 Serdes.  As such, mixed rates within the E1 and N2 Serdes interfaces
*   is not supported.
*
* INPUTS:
*   *digi_handle              - pointer to DIGI handle instance.
*   *sifd_1_cfg_ptr           - pointer to ILKN1 interface context
*   *sifd_2_cfg_ptr           - pointer to ILKN2 interface context
*
* OUTPUTS:
*   *ilkn_1_rate_ptr          - pointer to enum of serdes rate for ILKN1
*   *ilkn_2_rate_ptr          - pointer to enum of serdes rate for ILKN2
*   *ilkn1_e1_39_20_msk_ptr   - pointer to E1[39:20] pin use for ILKN1
*   *ilkn1_e1_19_00_msk_ptr   - pointer to E1[19:00] pin use for ILKN1      
*   *ilkn1_n1_msk_ptr         - pointer to N1[15:00] pin use for ILKN1      
*   *ilkn1_n2_msk_ptr         - pointer to N2[07:00] pin use for ILKN1      
*   *ilkn2_e1_39_20_msk_ptr   - pointer to E1[39:20] pin use for ILKN2            
*   *ilkn2_e1_19_00_msk_ptr   - pointer to E1[19:00] pin use for ILKN2      
*   *ilkn2_n1_msk_ptr         - pointer to N1[15:00] pin use for ILKN2            
*   *ilkn2_n2_msk_ptr         - pointer to N2[07:00] pin use for ILKN2            
*                                    
* RETURNS:                           
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_ilkn_serdes_pin_use_get(digi_handle_t *digi_handle,
                                         digi_sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                         digi_sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr,
                                         digi_ilkn_serdes_rate_t *ilkn_1_rate_ptr,
                                         digi_ilkn_serdes_rate_t *ilkn_2_rate_ptr,
                                         UINT32 *ilkn1_e1_39_20_msk_ptr,
                                         UINT32 *ilkn1_e1_19_00_msk_ptr,
                                         UINT32 *ilkn1_n1_msk_ptr,
                                         UINT32 *ilkn1_n2_msk_ptr,
                                         UINT32 *ilkn2_e1_39_20_msk_ptr,
                                         UINT32 *ilkn2_e1_19_00_msk_ptr,
                                         UINT32 *ilkn2_n1_msk_ptr,
                                         UINT32 *ilkn2_n2_msk_ptr)
{
    digi_ilkn_serdes_rate_t ilkn_1_rate = LAST_DIGI_ILKN_SERDES_RATE;
    UINT32 i = 0, temp = 0;
    UINT32 ilkn1_e1_39_20_msk = 0;
    UINT32 ilkn1_e1_19_00_msk = 0;
    UINT32 ilkn1_n1_msk = 0;
    UINT32 ilkn1_n2_msk = 0;
    digi_ilkn_serdes_rate_t ilkn_2_rate = LAST_DIGI_ILKN_SERDES_RATE;
    UINT32 ilkn2_e1_39_20_msk = 0;
    UINT32 ilkn2_e1_19_00_msk = 0;
    UINT32 ilkn2_n1_msk = 0;
    UINT32 ilkn2_n2_msk = 0;

    PMC_ENTRY();

    if(sifd_1_cfg_ptr != NULL)
    {
        /* Retrieve E1 pins used by ILKN1 */
        /* ILKN1[19:0] with offset 0 with mask 0xFFFFF maps to E1[39:20] in reverse order */
        temp = (sifd_1_cfg_ptr->lane.blk_to_E1_enable_mask >> 0) & 0xFFFFF;
        /* Correct the reverse bit ordering of ilkn1_e1_39_20_msk */
        for(i=0; i < (DIGI_SERDES_MAX_T8_SLICE); i++)
        {
            if((temp & (1 << i)) != 0)
            {
                ilkn1_e1_39_20_msk |= (1 << (19 - i));
            }
        }
        /* ILKN1[23:0] does not map to E1[19:0] */
        ilkn1_e1_19_00_msk = 0;
        /* Retrieve N1 pins used by ILKN1 */
        /* ILKN1[15:0] with offset of 0 maps to N1[15:0] with offset of 0 and mask 0xFFFF */
        ilkn1_n1_msk = sifd_1_cfg_ptr->lane.blk_to_N1_enable_mask & 0xFFFF;
        /* Retrieve N2 pins used by ILKN1 */
        /* ILKN1[23:16] with offset of 16 maps to N2[7:0] with offset of 0 and mask 0xFF */
        ilkn1_n2_msk = (sifd_1_cfg_ptr->lane.blk_to_N2_enable_mask >> 16) & 0xFF;
    
        /* Retrive rate of ILKN1 */
        ilkn_1_rate = sifd_1_cfg_ptr->ilkn.rate;
    }

    if(sifd_2_cfg_ptr != NULL)
    {
        /* Retrieve E1 pins used by ILKN 2 */
        /* ILKN2[23:20] maps to E1[23:20] with offset 20 and mask 0xF */
        ilkn2_e1_39_20_msk = (sifd_2_cfg_ptr->lane.blk_to_E1_enable_mask >> 20) & 0xF;
        /* ILKN2[19:0] maps to E1[19:0] with offset 0 and mask 0xFFFFF */
        ilkn2_e1_19_00_msk = (sifd_2_cfg_ptr->lane.blk_to_E1_enable_mask >> 0) & 0xFFFFF;
        /* Retrieve N1 pins used by ILKN2 */
        /* ILKN2[7:0] map to N1[15:8] with offset 8 and mask 0xFF*/
        ilkn2_n1_msk = (sifd_2_cfg_ptr->lane.blk_to_N1_enable_mask & 0xFF) << 8;
        /* Retrieve N2 pins used by ILKN2 */
        /* ILKN2[15:8] with offset of 8 maps to N2[7:0] with offset of 0 and mask 0xFF */
        ilkn2_n2_msk = (sifd_2_cfg_ptr->lane.blk_to_N2_enable_mask >> 8) & 0xFF;

        /* Retrive rate of ILKN 2 */
        ilkn_2_rate = sifd_2_cfg_ptr->ilkn.rate;
    }

    *ilkn_1_rate_ptr         = ilkn_1_rate;
    *ilkn_2_rate_ptr         = ilkn_2_rate;
    *ilkn1_e1_39_20_msk_ptr  = ilkn1_e1_39_20_msk;
    *ilkn1_e1_19_00_msk_ptr  = ilkn1_e1_19_00_msk;
    *ilkn1_n1_msk_ptr        = ilkn1_n1_msk;
    *ilkn1_n2_msk_ptr        = ilkn1_n2_msk;
    *ilkn2_e1_39_20_msk_ptr  = ilkn2_e1_39_20_msk;
    *ilkn2_e1_19_00_msk_ptr  = ilkn2_e1_19_00_msk;
    *ilkn2_n1_msk_ptr        = ilkn2_n1_msk;
    *ilkn2_n2_msk_ptr        = ilkn2_n2_msk;

    PMC_RETURN();

} /* digi_ilkn_serdes_pin_use_get */

/*******************************************************************************
* digi_bp_serdes_mask_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configure E1, N1 and N2 system serdes ports that have been
*   specified for usage with backplane interface. This function is used to 
*   configure SERDES rate or loopback 
*
* INPUTS:
*   *digi_handle              - pointer to DIGI handle instance.
*   *sifd_1_lane_cfg_ptr  - SIFD interlaken 1 lanes configuration pointer
*   *sifd_2_lane_cfg_ptr  - SIFD interlaken 2 lanes configuration pointer

* OUTPUTS:
*   *e1_19_00_msk               - E1 serdes 19 to 0 enable mask
*   *e1_39_20_msk               - E1 serdes 39 to 20 enable mask
*   *n1_msk                     - N1 serdes enable mask
*   *n2_msk                     - N2 serdes enable mask
*
* RETURNS:
*   PMC_ERROR
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_bp_serdes_mask_get(digi_handle_t               *digi_handle,
                                         digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg_ptr,
                                         digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg_ptr,
                                         UINT32                      *e1_19_00_msk,
                                         UINT32                      *e1_39_20_msk,
                                         UINT32                      *n1_msk,
                                         UINT32                      *n2_msk)
{
    digi_sifd_ilkn_1_cfg_t sifd_1_cfg;
    digi_sifd_ilkn_2_cfg_t sifd_2_cfg;
    digi_sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr = NULL;
    digi_sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr = NULL;
    digi_ilkn_serdes_rate_t ilkn_1_rate = LAST_DIGI_ILKN_SERDES_RATE;
    digi_ilkn_serdes_rate_t ilkn_2_rate = LAST_DIGI_ILKN_SERDES_RATE;
    UINT32 ilkn1_e1_39_20_msk = 0;
    UINT32 ilkn1_e1_19_00_msk = 0;
    UINT32 ilkn1_n1_msk = 0;     
    UINT32 ilkn1_n2_msk = 0;
    UINT32 ilkn2_e1_39_20_msk = 0;
    UINT32 ilkn2_e1_19_00_msk = 0;
    UINT32 ilkn2_n1_msk = 0;     
    UINT32 ilkn2_n2_msk = 0;   

    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0); 

    /* prepare serdes *digi_ilkn_serdes_pin_use_get function arguments */
    if( NULL != sifd_1_lane_cfg_ptr )
    {
        PMC_MEMCPY(&sifd_1_cfg.lane, sifd_1_lane_cfg_ptr, sizeof(digi_sifd_ilkn_1_lane_cfg_t));
        PMC_MEMCPY(&sifd_1_cfg.ilkn, &digi_handle->var.sifd_ilkn_1_cfg.ilkn, sizeof(digi_sifd_ilkn_cfg_t));
        sifd_1_cfg_ptr = &sifd_1_cfg;
    }
    if (NULL != sifd_2_lane_cfg_ptr)
    {
        PMC_MEMCPY(&sifd_2_cfg.lane, sifd_2_lane_cfg_ptr, sizeof(digi_sifd_ilkn_2_lane_cfg_t));   
        PMC_MEMCPY(&sifd_2_cfg.ilkn, &digi_handle->var.sifd_ilkn_2_cfg.ilkn, sizeof(digi_sifd_ilkn_cfg_t));
        sifd_2_cfg_ptr = &sifd_2_cfg;
    }

    digi_ilkn_serdes_pin_use_get(digi_handle,
                                 sifd_1_cfg_ptr,
                                 sifd_2_cfg_ptr,
                                 &ilkn_1_rate,
                                 &ilkn_2_rate,
                                 &ilkn1_e1_39_20_msk,
                                 &ilkn1_e1_19_00_msk,
                                 &ilkn1_n1_msk,
                                 &ilkn1_n2_msk,
                                 &ilkn2_e1_39_20_msk,
                                 &ilkn2_e1_19_00_msk,
                                 &ilkn2_n1_msk,
                                 &ilkn2_n2_msk);


    *e1_39_20_msk = 0;
    *e1_19_00_msk = 0;
    *n1_msk = 0;
    *n2_msk = 0;

    /* E1 19 to 0 serdes mask */
    if(ilkn1_e1_39_20_msk != 0)
    {
        *e1_39_20_msk = ilkn1_e1_39_20_msk;
    }
    if(ilkn2_e1_39_20_msk != 0)
    {
        *e1_39_20_msk |= ilkn2_e1_39_20_msk;
    }

    /* E1 39 to 20 serdes mask */
    if(ilkn1_e1_19_00_msk != 0)
    {
        *e1_39_20_msk = ilkn1_e1_39_20_msk;
    }
    if(ilkn2_e1_19_00_msk != 0)
    {
        *e1_19_00_msk |= ilkn2_e1_19_00_msk;
    }


    /* build n2 serdes mask */
    if(ilkn1_n2_msk != 0)
    {
        *n2_msk = ilkn1_n2_msk;
    }

    if(ilkn2_n2_msk != 0)
    {
        *n2_msk |= ilkn2_n2_msk;
    }


    /* build n1 serdes mask */
    *n1_msk = ilkn1_n1_msk | ilkn2_n1_msk; 


    PMC_RETURN(result);
}/* digi_bp_serdes_mask_get */




/*******************************************************************************
* digi_ge_d8_rx_2x_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The function lookups d8_rx_2x handle corresponding to GE serdes and also
*  outputs serdes identifier in D8 RX 2 slice block: 0 or 1. 
*   
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance
*   ge_intf             - GE serdes interface type
*
* OUTPUTS:
*  **d8_rx_2x_handle    - D8 serdes handle associated to given GE serdes interface.
*  *serdes_id           - serdes identifier: 0 or 1
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ge_d8_rx_2x_handle_get(digi_handle_t *digi_handle,
                                             digi_ge_serdes_id_t ge_intf,
                                             d8_rx_2x_handle_t  **d8_rx_2x_handle,
                                             UINT8 *serdes_id)
{
    BOOL8 found = FALSE;
    UINT32 i;    
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT((NULL != digi_handle), DIGI_ERR_NULL_HANDLE,  0, 0);
    PMC_ASSERT((NULL != d8_rx_2x_handle), DIGI_ERR_NULL_HANDLE,  0, 0);
    PMC_ASSERT((NULL != serdes_id), DIGI_ERR_NULL_HANDLE,  0, 0);

    if (ge_intf >= LAST_DIGI_GE_SERDES_ID)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }

    /* lookup digi_handle->ge_intf_handle->ge_port_ctxt to find d8_rx_handle */
    /* matching ge_intf type and ohp or rcp intf */
    for (i = 0; i < DIGI_GE_SERDES_D8_2X_MAX * 2 && found == FALSE; i++)
    {
        if (digi_handle->ge_intf_handle->ge_port_ctxt[i] != NULL && digi_handle->ge_intf_handle->ge_port_ctxt[i]->port_id == ge_intf)
        {
            found = TRUE;        
            *d8_rx_2x_handle = digi_handle->ge_intf_handle->d8_rx_2x_handle[((UINT8) ge_intf) / 2];
            *serdes_id = ((UINT8) ge_intf) % 2;
        }
    }
    if (FALSE == found)
    {
        *d8_rx_2x_handle = NULL;    
        result = DIGI_ERR_INVALID_ARG;
    }
     
    PMC_RETURN(result);    
}/* digi_ge_d8_rx_2x_handle_get */

/*******************************************************************************
* digi_ge_d8_tx_2x_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The function lookups d8_tx_2x handle corresponding to GE serdes and also
*  outputs serdes identifier in D8 TX 2 slice block: 0 or 1. 
*   
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance
*   ge_intf             - GE serdes interface type
*
* OUTPUTS:
*  **d8_tx_2x_handle    - D8 serdes handle associated to given GE serdes interface.
*  *serdes_id           - serdes identifier: 0 or 1
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ge_d8_tx_2x_handle_get(digi_handle_t       *digi_handle,
                                             digi_ge_serdes_id_t  ge_intf,
                                             d8_tx_2x_handle_t  **d8_tx_2x_handle,
                                             UINT8               *serdes_id)
{
    BOOL8 found = FALSE;
    UINT32 i;    
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT((NULL != digi_handle), DIGI_ERR_NULL_HANDLE,  0, 0);
    PMC_ASSERT((NULL != d8_tx_2x_handle), DIGI_ERR_NULL_HANDLE,  0, 0);
    PMC_ASSERT((NULL != serdes_id), DIGI_ERR_NULL_HANDLE,  0, 0);

    if (ge_intf >= LAST_DIGI_GE_SERDES_ID)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }

    /* lookup digi_handle->ge_intf_handle->ge_port_ctxt to find d8_tx_handle */
    /* matching ge_intf type and ohp or rcp intf */
    for (i = 0; i < DIGI_GE_SERDES_D8_2X_MAX*2 && found == FALSE; i++)
    {
        if (digi_handle->ge_intf_handle->ge_port_ctxt[i] != NULL && digi_handle->ge_intf_handle->ge_port_ctxt[i]->port_id == ge_intf)
        {
      
            found = TRUE;        
            *d8_tx_2x_handle = digi_handle->ge_intf_handle->d8_tx_2x_handle[((UINT8)digi_handle->ge_intf_handle->ge_port_ctxt[i]->port_id) / 2];
            *serdes_id = ((UINT8)digi_handle->ge_intf_handle->ge_port_ctxt[i]->port_id) % 2;
        }
    }

    if (FALSE == found)
    {
        *d8_tx_2x_handle = NULL;
        result = DIGI_ERR_INVALID_ARG;
    }

    PMC_RETURN(result);    
}/* digi_ge_d8_tx_2x_handle_get */


/*******************************************************************************
* digi_serdes_polarity_c8_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Configure C8 SERDES lane polarity inversion
*
* INPUTS:
*  digi_handle      - DIGI device handle.
*  pin_id           - Serdes pin index.
*  dir              - serdes lanes direction: RX or TX. 
*  enable           - Enable or disable polarity.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_polarity_c8_cfg(digi_handle_t           *digi_handle, 
                                             UINT8                    pin_id,
                                             util_global_direction_t  dir,
                                             BOOL8                    enable)
{
    PMC_ERROR result = PMC_SUCCESS;  
    PMC_ENTRY();

    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    
    c8_polarity_cfg(digi_handle->bp_sr_c8_handle, pin_id, dir, enable);            

    digi_serdes_c8_polarity_in_ctxt_set(digi_handle, 1 << pin_id, dir, enable);

    PMC_RETURN(result);

} /* End: digi_serdes_polarity_c8_cfg() */

/*******************************************************************************
* digi_serdes_polarity_t8_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Configure T8 SERDES in polarity mode.
*
* INPUTS:
*  digi_handle            - DIGI device handle.
*  pin_id                 - Serdes pin index.
*  dir                    - serdes lanes direction: RX or TX. 
*  enable                 - Enable or disable polarity.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, error codes otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_polarity_t8_cfg(digi_handle_t           *digi_handle, 
                                             UINT8                    pin_id,
                                             util_global_direction_t  dir,
                                             BOOL8                    enable)
{
    PMC_ERROR result = PMC_SUCCESS;  
    UINT8 serdes_id;
    PMC_ENTRY();

    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
      
    serdes_id = digi_serdes_map_t8_serdes(pin_id);
    t8_polarity_cfg(digi_handle->bp_lr_t8_handle[DIGI_SERDES_T8_SLICE_ID_GET(pin_id)],  serdes_id, dir, enable);

    if ((DIGI_SERDES_MAX_T8_SLICE) > pin_id)
    {
        digi_serdes_t8_polarity_in_ctxt_set(digi_handle, 1 << pin_id, 0, dir, enable);
    }
    else
    {
        digi_serdes_t8_polarity_in_ctxt_set(digi_handle, 0, 1 << (pin_id - (DIGI_SERDES_MAX_T8_SLICE)), dir, enable);
    }

    PMC_RETURN(result);

} /* End: digi_serdes_polarity_t8_cfg() */

#ifdef PMC_DEPRECATED

/*******************************************************************************
* digi_serdes_t8_dfe_error_invert
* ___________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED-Function has been marked as deprecated starting in the 3.02 release
*   The function inverts T8 serdes error signal of the equalizer only when 
*   serdes polarity is inverted.
*
* INPUTS:
*  digi_handle            - DIGI device handle.
*  pin_id                 - Serdes pin index.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*   Use of this function is not required to configure the T8 SerDes or to perform 
*   capture of diagnostic data
*
*******************************************************************************/

/* LCOV_EXCL_START */

PUBLIC void digi_serdes_t8_dfe_error_invert(digi_handle_t *digi_handle,
                                            UINT8          pin_id)
{
    UINT8 serdes_id;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT((NULL != digi_handle), DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(DIGI_SERDES_MAX_T8_E1_IN_BP_LR > pin_id, DIGI_ERR_INVALID_ARG, 0, 0);
    
    if (LAST_UTIL_GLOBAL_DIR != 
        digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[pin_id].polarity_inversed)
    {
        serdes_id = digi_serdes_map_t8_serdes(pin_id);
        t8_dfe_error_invert(digi_handle->bp_lr_t8_handle[DIGI_SERDES_T8_SLICE_ID_GET(pin_id)], serdes_id);
    }

    PMC_ATOMIC_RETURN(digi_handle);

} /* End: digi_serdes_t8_dfe_error_invert() */



/*******************************************************************************
* digi_serdes_sfi51_tx_dclk_enable
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Function to enable the SFI1_TXCKSRC_P/IN or SFI2_TXCKSRC_P/N on an SFI5.1/D8 
*  serdes interface. For power saving reasons, this clock is not enabled by 
*  default. The correct physical resource will be modified appropriately 
*  depending on the SFI5.1 port_ctxt_ptr.
*
* INPUTS:
*  digi_handle                   - DIGI device handle.
*  port_ctxt_ptr                 - A pointer to the SFI51. SERDES instance.
*  enable                        - TRUE to enable. FALSE to disable.
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, a descriptive error code otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_sfi51_tx_dclk_enable(digi_handle_t* digi_handle, 
                                                  digi_serdes_port_t* port_ctxt_ptr,
                                                  BOOL8 enable)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    UINT8 index;
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* verify that port ctxt ptr belongs to digi handle pool */
    DIGI_SERDES_PORT_CTXT_HANDLE_CHECK(digi_handle, port_ctxt_ptr, rc);

    rc = digi_serdes_d8_tx_dclk_validate_params_and_instance_get(digi_handle, 
                                                                 port_ctxt_ptr, 
                                                                 &index);

    if (rc == PMC_SUCCESS) {

        if (enable == TRUE) {

            rc = sfi51_d8_rx_18x_txdclk_enable(digi_handle->sfi51_d8_rx_18x_handle[index]);

        } else {

            rc = sfi51_d8_rx_18x_txdclk_disable(digi_handle->sfi51_d8_rx_18x_handle[index]);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);

} /* digi_serdes_sfi51_tx_dclk_enable */

/* LCOV_EXCL_STOP */



#endif /* PMC_DEPRECATED */

#endif /* DOXYGEN_PUBLIC_ONLY */
/*******************************************************************************
* digi_serdes_t8_dfe_error_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*   The function returns value of T8 serdes error signal of the equalizer.
*
* INPUTS:
*  digi_handle            - DIGI device handle.
*  pin_id                 - Serdes pin index.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Value of error signal of the equalizer.
*
* NOTES:
*   None.
*
*******************************************************************************/

/* LCOV_EXCL_START */

PUBLIC BOOL8 digi_serdes_t8_dfe_error_get(digi_handle_t *digi_handle,
                                          UINT8          pin_id)
{
    UINT8 serdes_id;
    BOOL8 rc;

#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif
    PMC_ASSERT((NULL != digi_handle), DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(DIGI_SERDES_MAX_T8_E1_IN_BP_LR > pin_id, DIGI_ERR_INVALID_ARG, 0, 0);
    
    serdes_id = digi_serdes_map_t8_serdes(pin_id);
    rc = t8_dfe_error_get(digi_handle->bp_lr_t8_handle[DIGI_SERDES_T8_SLICE_ID_GET(pin_id)],  serdes_id);

    PMC_ATOMIC_RETURN(digi_handle,rc);

} /* End: digi_serdes_t8_dfe_error_get() */

/* LCOV_EXCL_STOP */

#ifndef DOXYGEN_PUBLIC_ONLY

/*******************************************************************************
* digi_serdes_polarity_bp_s16_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to configure the S16 SERDES in 
*  the backplane interface.
*
* INPUTS:
*  digi_handle            - DIGI device handle.
*  pin_id                 - Serdes pin index.
*  dir                    - serdes lanes direction: RX or TX. 
*  enable                 - Enable or disable polarity
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_polarity_bp_s16_cfg(digi_handle_t           *digi_handle, 
                                                 UINT8                    pin_id, 
                                                 util_global_direction_t  dir,
                                                 BOOL8                    enable)
{
    PMC_ERROR result = PMC_SUCCESS;  
    PMC_ENTRY();
    
    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
  
    s16_polarity_cfg(digi_handle->s16_sys_handle, pin_id, dir, enable);

    digi_serdes_s16_polarity_in_ctxt_set(digi_handle, DIGI_SERDES_SYSTEM_INTF, 1 << pin_id,dir, enable);

    PMC_RETURN(result);
}/*digi_serdes_polarity_s16_cfg */




/*******************************************************************************
* digi_serdes_map_t8_serdes
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Returns serdes id A or B corresponding to pin index.
*
* INPUTS:
*  pin_id  - T8 serdes index
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   T8 serdes id A or B is returned.
*
* NOTES:
*   
*
*******************************************************************************/
PUBLIC UINT8 digi_serdes_map_t8_serdes(UINT8 pin_id)
{
    UINT8 serdes_id;

    PMC_ENTRY();

    serdes_id = (pin_id % 2) == 0 ? 1 : 0;
    
    PMC_RETURN(serdes_id);
}/* digi_serdes_map_t8_serdes */

/*******************************************************************************
* digi_serdes_c8_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to configure the C8 SERDES on 
*  the system backplane.
*
* INPUTS:
*  digi_handle      - DIGI device handle.
*  enable_msk       - Bitmask of lanes to enable (0-7). 
*                     Bit 0 corresponds to lane 0.
*  serdes_port_mode - SERDES rate to configure
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_c8_cfg(digi_handle_t *digi_handle, 
                                    UINT8 enable_msk,
                                    digi_serdes_port_mode_t serdes_port_mode)
{
    PMC_ERROR result = PMC_SUCCESS; 
    UINT32 i;
    UINT8 patt_bus_width = 0;
    digi_serdes_patt_gen_bus_width_map_t  c8_bus_width_map[] = DIGI_SERDES_PATT_GEN_BUS_WIDTH_MAP_DEFAULT;
    /* Initialize it, so it is set before used. */
    hostmsg_c8_serdes_rate_t c8_serdes_rate = HOSTMSG_C8_SERDES_RATE_MAX_NUM;
    UINT32 data[1];

    PMC_ENTRY();

    /* Convert SERDES rate to host message parameters */
    result = digi_serdes_c8_rate_convert(serdes_port_mode , &c8_serdes_rate);

    /* Send message*/
    if (PMC_SUCCESS == result)
    {
        /* CMD_CFG_SERDES_C8 has the following format:
         * Word  bits 24..31   bits 16..23   bits 8..15   bits 0..7
         *   0    rate         enable_msk           reserved
         */
        data[0] = ((UINT8)c8_serdes_rate << 24) | (enable_msk << 16);

        {
            HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
            result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_CFG_SERDES_C8_ST,data,1,stat);
        }
    }

    /* Display FW debug info if the serdes config sequence failed */
    if (result != PMC_SUCCESS  && enable_msk != DIGI_SERDES_MASK_NONE)
    {
        serdes_debug_t serdes_debug;
        PMC_ERROR status_get_result;
        status_get_result = digi_serdes_debug_status_internal_get(digi_handle,&serdes_debug,TRUE);
        PMC_ASSERT(status_get_result == PMC_SUCCESS,status_get_result,0,0);

        PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_C8_SERDES_CONFIG,
                serdes_debug.c8_cur_step,
                serdes_debug.c8_config_rv,
                "c8 serdes debug c8_cur_step=%d c8_cur_lane %d c8_config_rv=%x\n", 
                serdes_debug.c8_cur_step, serdes_debug.c8_cur_lane, serdes_debug.c8_config_rv);
    }

     
    if (PMC_SUCCESS == result)
    {

        /* define pattern generator bus width*/
        for (i = 0; i < DIGI_SERDES_T8_NUM_RATES_MAX+DIGI_SERDES_C8_NUM_RATES_MAX; i++)
        {
            if (serdes_port_mode == c8_bus_width_map[i].rate)
            {
                patt_bus_width = c8_bus_width_map[i].patt_gen_bus_width;
            }
        }

        for (i = 0; i < DIGI_SERDES_MAX_C8_IN_BP_SR; i++)
        {
            if (0 != ((1 << i) & enable_msk))
            {
                /* update serdes pattern generator bus width */          
                c8_patt_gen_bus_width_set(digi_handle->bp_sr_c8_handle,
                                          i, patt_bus_width);
            }
        }
        
        /* Write serdes properties stored in context */
        digi_serdes_c8_lanes_params_write(digi_handle, enable_msk, serdes_port_mode);
    }
    
    PMC_RETURN(result);
    
} /* End: digi_serdes_c8_cfg() */

/*******************************************************************************
* digi_serdes_t8_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to configure the T8 SERDES on 
*  the system backplane.
*
* INPUTS:
*  digi_handle            - DIGI device handle.
*  lane_0_19_enable_msk   - Bitmask of lanes 0-19 to enable. 
*                           Bit 0 corresponds to lane 0.
*  lane_20_39_enable_msk  - Bitmask of lanes 20-39 to enable. 
*                           Bit 0 corresponds to lane 20.
*  lane_0_19_port_mode    - SERDES rate to configure for lanes 0-19
*  lane_20_39_port_mode   - SERDES rate to configure for lanes 20-39
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_t8_cfg(digi_handle_t *digi_handle, 
                                    UINT32 lane_0_19_enable_msk,
                                    UINT32 lane_20_39_enable_msk,
                                    digi_serdes_port_mode_t lane_0_19_port_mode, 
                                    digi_serdes_port_mode_t lane_20_39_port_mode)
{
    PMC_ERROR result = PMC_SUCCESS;        
    digi_serdes_port_mode_t serdes_port_mode;
    /* Initialize to default, so it is set before used. */
    hostmsg_t8_serdes_rate_t t8_serdes_rate = HOSTMSG_T8_SERDES_RATE_MAX_NUM;
    UINT32 idx;
    UINT32 data[2];
    UINT32 lane_mask;
    UINT8 serdes_id;
    UINT32 i;
    UINT32 lane_0_19_bus_width = 0;
    UINT32 lane_20_39_bus_width = 0;
    digi_serdes_patt_gen_bus_width_map_t  t8_bus_width_map[] = DIGI_SERDES_PATT_GEN_BUS_WIDTH_MAP_DEFAULT;

    PMC_ENTRY();

    /* Convert SERDES rate to host message parameters */
    for (idx = 0; idx < 2; idx++) {
        if (0 == idx) {
            serdes_port_mode = lane_0_19_port_mode;
            lane_mask = lane_0_19_enable_msk;
        } else {
            serdes_port_mode = lane_20_39_port_mode;
            lane_mask = lane_20_39_enable_msk;
        }
        
        result = digi_serdes_t8_rate_convert(serdes_port_mode, &t8_serdes_rate);

        if (PMC_SUCCESS != result && 0 == lane_mask)
        {
            result = PMC_SUCCESS;
        }

        /* CMD_CFG_SERDES_T8 have the following format:
         * Word  bits 24..31   | bits 16..23 |  bits 8..15  | bits 0..7
         *   0  lane_0_19_rate |   reserved  |  lane_0_19_enable_msk
         *   1  lane_20_39_rate|             |  lane_20_39_enable_msk
         */
        data[idx] = ((UINT8)t8_serdes_rate << 24) | (lane_mask & 0xfffff);
    } /* end for */

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_CFG_SERDES_T8_ST,data,2,stat);
    }

    /* Display FW debug info if the serdes config sequence failed */
    if (result != PMC_SUCCESS && (lane_0_19_enable_msk != DIGI_SERDES_MASK_NONE 
                                  || lane_20_39_enable_msk != DIGI_SERDES_MASK_NONE))

    {
        serdes_debug_t serdes_debug;
        PMC_ERROR status_get_result;
        status_get_result = digi_serdes_debug_status_internal_get(digi_handle,&serdes_debug,TRUE);
        PMC_ASSERT(status_get_result == PMC_SUCCESS,status_get_result,0,0);

        PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_T8_SERDES_CONFIG,
                serdes_debug.t8_cur_step,
                serdes_debug.t8_config_rv,
                "t8 serdes debug t8_cur_step=%d t8_cur_slice %d t8_cur_subch %d t8_config_rv=%x\n", 
                serdes_debug.t8_cur_step, serdes_debug.t8_cur_slice, serdes_debug.t8_cur_subch, 
                serdes_debug.t8_config_rv);
    }

    if (PMC_SUCCESS == result)
    {
        /* define pattern generator bus width*/
        for (i = 0; i < DIGI_SERDES_T8_NUM_RATES_MAX+DIGI_SERDES_C8_NUM_RATES_MAX; i++)
        {
            if (lane_0_19_port_mode == t8_bus_width_map[i].rate)
            {
                lane_0_19_bus_width = t8_bus_width_map[i].patt_gen_bus_width;
            }
            if (lane_20_39_port_mode == t8_bus_width_map[i].rate)
            {
                lane_20_39_bus_width = t8_bus_width_map[i].patt_gen_bus_width;
            }
        } 

        for (i = 0; i < DIGI_SERDES_MAX_T8_E1_IN_BP_LR; i++)
        {
            if (i < (DIGI_SERDES_MAX_T8_SLICE))
            {
                if (0 != ((1 << i) & lane_0_19_enable_msk))
                {
                    /* update serdes pattern generator bus width */
                    serdes_id = digi_serdes_map_t8_serdes(i);                
                    t8_patt_gen_bus_width_set(digi_handle->bp_lr_t8_handle[DIGI_SERDES_T8_SLICE_ID_GET(i)],
                                              serdes_id, lane_0_19_bus_width);
                }
            }
            else
            {
                if (0 != ((1 << (i-(DIGI_SERDES_MAX_T8_SLICE))) & lane_20_39_enable_msk))
                {
                    /* update serdes pattern generator bus width */
                    serdes_id = digi_serdes_map_t8_serdes(i);                
                    t8_patt_gen_bus_width_set(digi_handle->bp_lr_t8_handle[DIGI_SERDES_T8_SLICE_ID_GET(i)],
                                              serdes_id, lane_20_39_bus_width);
                    t8_ext_ctxt_state_set(digi_handle->bp_lr_t8_handle[DIGI_SERDES_T8_SLICE_ID_GET(i)], 0 == serdes_id ? 1 : 2, TRUE);
                }
            }
        }

        /* Write serdes properties stored in context */
        digi_serdes_t8_lanes_params_write(digi_handle, 
                                          lane_0_19_enable_msk, 
                                          lane_20_39_enable_msk, 
                                          lane_0_19_port_mode, 
                                          lane_20_39_port_mode);
    }

    PMC_RETURN(result);

} /* End: digi_serdes_t8_cfg() */

/*******************************************************************************
* digi_serdes_s16_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to configure the S16 SERDES on 
*  the line or system side.
*
* INPUTS:
*  digi_handle            - DIGI device handle.
*  s16_intf_type          - S16 interface type
*  enable_msk             - Bitmask of channels to configure. Bit 0
*                           corresponds to channel 0. 
*  serdes_rate            - SERDES rate to configure
*  disable_msk            - Bitmask of channels to disable. Bit 0
*                           corresponds to channel 0. 
*                           If this field is non-zero, the rate, master_lane
*                           and enable_msk are ignored.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_s16_cfg(digi_handle_t             *digi_handle, 
                                     hostmsg_s16_intf_type_t    s16_intf_type, 
                                     UINT16                     enable_msk,
                                     digi_serdes_port_mode_t    serdes_rate, 
                                     UINT16                     disable_msk)
{
    PMC_ERROR result = PMC_SUCCESS;        
    DOUBLE s16_lane_serdes_rate;
    BOOL8 s16_otu_client;
    hostmsg_s16_serdes_rate_t s16_serdes_rate;
    s16_port_mode_t  s16_port_mode_dummy;
    UINT16 enable_itr UNUSED= 0;
    UINT16 enable_num UNUSED = (s16_intf_type == HOSTMSG_S16_SERDES_INTF_LINE)?DIGI_SERDES_XFI_LANES_MAX:DIGI_SERDES_MAX_S16_IN_BP;
    BOOL8 enable = FALSE;
    UINT16 msk = 0;
    UINT32 data[2];

    PMC_ENTRY();

    PMC_ASSERT(enable_msk == 0 || disable_msk == 0, HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    enable = enable_msk != 0?TRUE:FALSE;
    msk = (TRUE == enable)?enable_msk:disable_msk;

    for (enable_itr = 0;enable_itr < enable_num && result == PMC_SUCCESS; enable_itr++) 
    {
        if ((msk & (1 << enable_itr)) != 0)
        {            
            result  = digi_serdes_single_s16_cfg(digi_handle,s16_intf_type, enable_itr,  serdes_rate, enable);
        }
    }
    if (HOSTMSG_ERR_RSP_NACK == result) 
    {

        /* Convert SERDES rate to host message parameters */
        result = digi_serdes_s16_rate_convert(serdes_rate, &s16_serdes_rate,&s16_lane_serdes_rate,&s16_otu_client, &s16_port_mode_dummy);

        /* CMD_CFG_SERDES_S16 have the following format:
         * Word  bits 24..31   bits 16..23   bits 8..15   bits 0..7
         *   0  line_sys_sel    reserved          disable_msk
         *   1  rate            master_lane?       enable_msk
         */
        data[0] = ((UINT8)s16_intf_type << 24)   | disable_msk;
        data[1] = ((UINT8)s16_serdes_rate << 24) | enable_msk;

        if (PMC_SUCCESS == result)
        {
            HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
            result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_CFG_SERDES_S16_ST,data,2,stat);
        }

        /* Display FW debug info if the serdes config sequence failed */
        if (result != PMC_SUCCESS && enable_msk != DIGI_SERDES_MASK_NONE)
        {
            serdes_debug_t serdes_debug;
            PMC_ERROR status_get_result;
            status_get_result = digi_serdes_debug_status_internal_get(digi_handle,&serdes_debug,TRUE);
            PMC_ASSERT(status_get_result == PMC_SUCCESS,status_get_result,0,0);

            PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_S16_SERDES_CONFIG,
                    serdes_debug.s16_cur_step,
                    serdes_debug.s16_config_rv,
                    "s16 serdes debug s16_cur_step=%d s16_cur_chnl %d s16_config_rv=%x\n", 
                    serdes_debug.s16_cur_step, serdes_debug.s16_cur_chnl, serdes_debug.s16_config_rv);
        }
    }

    if (PMC_SUCCESS == result)
    {
        digi_serdes_s16_lanes_params_write(digi_handle, s16_intf_type, 
                                           enable_msk, disable_msk, serdes_rate);
    }

    PMC_RETURN(result);

} /* End: digi_serdes_s16_cfg() */

/*******************************************************************************
* digi_serdes_enet_line_port_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function performs simple parameter validation for the call to
*   digi_enet_line_prov_params_validate
*
* INPUTS:
*   *port_def_ptr          - Port datapath handle defining the physcial SerDes
*                            to configure the ENET_LINE resource against.\n
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_enet_line_port_validate(digi_serdes_port_def_t *port_def_ptr)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;

    PMC_ENTRY();

    if (port_def_ptr != NULL) {
        if (port_def_ptr->intf == DIGI_SERDES_LINE_INTF)
        {
            rc = PMC_SUCCESS;
        }
    }

    PMC_RETURN(rc);
} /* digi_serdes_enet_line_port_validate */

/*******************************************************************************
* digi_serdes_enet_sys_port_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function performs simple parameter validation for the call to
*   digi_enet_sys_prov_params_validate
*
* INPUTS:
*   *port_def_ptr          - Port datapath handle defining the physcial SerDes
*                            to configure the ENET_LINE resource against.\n
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_enet_sys_port_validate(digi_serdes_port_def_t *port_def_ptr)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;

    PMC_ENTRY();

    if (port_def_ptr != NULL) {
        if (port_def_ptr->intf == DIGI_SERDES_SYSTEM_INTF)
        {
            rc = PMC_SUCCESS;
        }
    }

    PMC_RETURN(rc);
} /* digi_serdes_enet_sys_port_validate */


/*******************************************************************************
* digi_port_ctxt_find
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a helper function to find the first available port of type
*   digi_serdes_inf_t matching the specified port_uid.
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   port_uid           - the unique identifier of the SERDES port.
*                        To find the first unused one use:
*                        DIGI_SERDES_UNASSIGNED
*   intf               - the requested SERDES type.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   A pointer to the requested matching digi_serdes_port_t structure of type
*   intf, or NULL if there are no more available or the requested one could not
*   be found.
*
* NOTES:
*
*******************************************************************************/
PUBLIC digi_serdes_port_def_t* digi_port_ctxt_find(digi_handle_t *digi_handle,
                                                   UINT32 port_uid,
                                                   digi_serdes_intf_t intf)
{
    digi_serdes_port_t* rp = NULL;
    UINT8 i = 0;

    PMC_ENTRY();

    switch (intf) {

    case DIGI_SERDES_LINE_INTF:
        for (i = 0; i < DIGI_NUM_LINE_PORTS_MAX; i ++) {

            if (digi_handle->var.port_ctxt.line_port_ctxt[i].port_uid ==
                port_uid) {

                rp = &(digi_handle->var.port_ctxt.line_port_ctxt[i]);
                break;
            }
        }

        break;
    case DIGI_SERDES_SYSTEM_INTF:
        for (i = 0; i < DIGI_NUM_SYS_PORTS_MAX; i ++) {

            if (digi_handle->var.port_ctxt.sys_port_ctxt[i].port_uid ==
                port_uid) {

                rp = &(digi_handle->var.port_ctxt.sys_port_ctxt[i]);
                break;
            }
        }

        break;
    case DIGI_SERDES_SFI_51_INTF:
        for (i = 0; i < DIGI_NUM_SFI51_PORTS_MAX; i ++) {

            if (digi_handle->var.port_ctxt.sfi51_port_ctxt[i].port_uid ==
                port_uid) {

                rp = &(digi_handle->var.port_ctxt.sfi51_port_ctxt[i]);
                break;
            }
        }
        break;
    default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    }

    PMC_RETURN((digi_serdes_port_def_t*)rp);
} /* digi_port_ctxt_find */

/*******************************************************************************
* digi_port_ctxt_all_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_port_ctxt_all_reset(digi_handle_t *digi_handle)
{
    UINT8 i = 0;

    PMC_ENTRY();

    for (i = 0; i < DIGI_NUM_LINE_PORTS_MAX; i ++) {

        digi_handle->var.port_ctxt.line_port_ctxt[i].init = FALSE;
    }

    for (i = 0; i < DIGI_NUM_SYS_PORTS_MAX; i ++) {

        digi_handle->var.port_ctxt.sys_port_ctxt[i].init = FALSE;
    }

    for (i = 0; i < DIGI_NUM_SFI51_PORTS_MAX; i ++) {

        digi_handle->var.port_ctxt.sfi51_port_ctxt[i].init = FALSE;
    }

    PMC_RETURN();
} /* digi_port_ctxt_all_reset */

/*******************************************************************************
* digi_port_ctxt_and_intf_find
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a helper function to find the first available port of type
*   digi_serdes_inf_t matching the specified port_uid.
*   It also return the interface that this port_uid was found on.
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   port_uid           - the unique identifier of the SERDES port.
*                        To find the first unused one use:
*                        DIGI_SERDES_UNASSIGNED
*
* OUTPUTS:
*   intf*              - the SERDES type of this port
*
* RETURNS:
*   A pointer to the requested matching digi_serdes_port_t structure of type
*   intf, or NULL if there are no more available or the requested one could not
*   be found.
*
* NOTES:
*
*******************************************************************************/
PUBLIC digi_serdes_port_def_t* digi_port_ctxt_and_intf_find(digi_handle_t *digi_handle,
                                                            UINT32 port_uid,
                                                            digi_serdes_intf_t* intf)
{
    digi_serdes_port_t* rp = NULL;
    digi_serdes_intf_t  local_intf;
    UINT8 i = 0;

    PMC_ENTRY();

    local_intf = DIGI_SERDES_LINE_INTF;

    for (i = 0; i < DIGI_NUM_LINE_PORTS_MAX; i ++) {

        if (digi_handle->var.port_ctxt.line_port_ctxt[i].port_uid == port_uid) {

            rp = &(digi_handle->var.port_ctxt.line_port_ctxt[i]);
            break;
        }
    }

    if (rp == NULL) {

        local_intf = DIGI_SERDES_SYSTEM_INTF;

        for (i = 0; i < DIGI_NUM_SYS_PORTS_MAX; i ++) {

            if (digi_handle->var.port_ctxt.sys_port_ctxt[i].port_uid == port_uid) {

                rp = &(digi_handle->var.port_ctxt.sys_port_ctxt[i]);
                break;
            }
        }

        if (rp == NULL) {

            local_intf = DIGI_SERDES_SFI_51_INTF;

            for (i = 0; i < DIGI_NUM_SFI51_PORTS_MAX; i ++) {

                if (digi_handle->var.port_ctxt.sfi51_port_ctxt[i].port_uid == port_uid) {

                    rp = &(digi_handle->var.port_ctxt.sfi51_port_ctxt[i]);
                    break;
                }
            }
        }
    }

    if (rp != NULL) {

        *intf = local_intf;
    }

    PMC_RETURN((digi_serdes_port_def_t*)rp);
} /* digi_port_ctxt_and_intf_find */

/*******************************************************************************
* digi_port_ctxt_no_type_find
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a helper function to find the first available port of an unknown
*   type matching the specified port_uid.
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   port_uid           - the unique identifier of the SERDES port.
*                        To find the first unused one use:
*                        DIGI_SERDES_UNASSIGNED
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   A pointer to the requested matching digi_serdes_port_t structure of an
*   unknown type, or NULL if there are no more available or the requested one
*   could not be found.
*
* NOTES:
*
*******************************************************************************/
PUBLIC digi_serdes_port_t* digi_port_ctxt_no_type_find(digi_handle_t *digi_handle,
                                                       UINT32 port_uid)
{
    digi_serdes_port_t* xfi_port_ptr = NULL;
    digi_serdes_port_t* sfi51_port_ptr = NULL;
    digi_serdes_port_t* sys_port_ptr = NULL;
    digi_serdes_port_t* return_port_ptr = NULL;

    PMC_ENTRY();

    xfi_port_ptr = digi_port_ctxt_find(digi_handle, port_uid, DIGI_SERDES_LINE_INTF);
    sys_port_ptr = digi_port_ctxt_find(digi_handle, port_uid, DIGI_SERDES_SYSTEM_INTF);
    sfi51_port_ptr = digi_port_ctxt_find(digi_handle, port_uid, DIGI_SERDES_SFI_51_INTF);

    if ((xfi_port_ptr != NULL) && (sfi51_port_ptr != NULL)) {

        /* ASSERT as we shouldn't have both of these set */
        PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);

    } else if ((xfi_port_ptr != NULL) && (sys_port_ptr != NULL)) {

        /* ASSERT as we shouldn't have both of these set */
        PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);

    } else if ((sfi51_port_ptr != NULL) && (sys_port_ptr != NULL)) {

        /* ASSERT as we shouldn't have both of these set */
        PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);

    } else if (xfi_port_ptr != NULL) {

        return_port_ptr = xfi_port_ptr;

    } else if (sfi51_port_ptr != NULL) {

        return_port_ptr = sfi51_port_ptr;

    } else if (sys_port_ptr != NULL) {

        return_port_ptr = sys_port_ptr;

    } else {

        /* NO-OP */
    }

    PMC_RETURN(return_port_ptr);
} /* digi_port_ctxt_no_type_find */

/*******************************************************************************
* digi_serdes_port_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   a serdes port for a major mode of operation as specified by the enum
*   digi_serdes_port_mode_t.
*
*   For applications that require multiple serdes ports, this API must be called
*   once for each API within the set of serdes.
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   *port_ctxt_ptr     - pointer to a Serdes ctxt instance
*   mode               - Configuration state for serdes port
*
* OUTPUTS:
*    None.
* 
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
* 
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_port_prov(digi_handle_t *digi_handle,
                                       digi_serdes_port_t *port_ctxt_ptr,
                                       digi_serdes_port_mode_t mode)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    util_serdes_patt_gen_cfg_t patt_gen_cfg;    
    digi_serdes_port_def_t* port_data_ptr = (digi_serdes_port_def_t*)port_ctxt_ptr;
    digi_serdes_intf_t intf = port_data_ptr->intf;
    UINT16 enable_mask;
    s16_port_mode_t s16_port_mode;
    hostmsg_s16_serdes_rate_t s16_serdes_rate_dummy;
    DOUBLE s16_lane_serdes_rate_dummy;
    BOOL8 s16_otu_client_dummy;

    PMC_ENTRY();

    /* identify the set of serdes pins assigned to the port */
    rc = digi_port_pin_bit_mask_s16_clk_get(digi_handle, (digi_serdes_port_def_t*)port_ctxt_ptr, &enable_mask);
    
    if (rc == PMC_SUCCESS) {

        switch (intf) {
        case DIGI_SERDES_LINE_INTF:
            /* Line side digi_serdes_s16_cfg case */
            rc = digi_serdes_s16_cfg(digi_handle, 
                                     HOSTMSG_S16_SERDES_INTF_LINE, 
                                     enable_mask, 
                                     mode, 
                                     DIGI_SERDES_MASK_NONE);

            if (DIGI_SERDES_PIN_NOT_USED != port_data_ptr->s16_clk_cfg.pin_index)  {
                rc = digi_serdes_s16_rate_convert(mode,  &s16_serdes_rate_dummy ,&s16_lane_serdes_rate_dummy, &s16_otu_client_dummy, &s16_port_mode);
                
                if (PMC_SUCCESS == rc)
                {
                    digi_serdes_s16_clk_patt_gen_cfg_get(digi_handle,TRUE,port_data_ptr->s16_clk_cfg.divider,&patt_gen_cfg);
                    rc = s16_patt_gen_cfg(digi_handle->s16_line_handle,port_data_ptr->s16_clk_cfg.pin_index, s16_port_mode, &patt_gen_cfg);                    
                }
            }
            break; 
        case DIGI_SERDES_SYSTEM_INTF:
            /* System side digi_serdes_s16_cfg case */
            rc = digi_serdes_s16_cfg(digi_handle, 
                                     HOSTMSG_S16_SERDES_INTF_SYS, 
                                     enable_mask, 
                                     mode, 
                                     DIGI_SERDES_MASK_NONE);
            if (DIGI_SERDES_PIN_NOT_USED != port_data_ptr->s16_clk_cfg.pin_index)  {
                rc = digi_serdes_s16_rate_convert(mode,  &s16_serdes_rate_dummy ,&s16_lane_serdes_rate_dummy, &s16_otu_client_dummy, &s16_port_mode);
                if (PMC_SUCCESS == rc)
                {
                    digi_serdes_s16_clk_patt_gen_cfg_get(digi_handle,TRUE,port_data_ptr->s16_clk_cfg.divider,&patt_gen_cfg);
                    rc = s16_patt_gen_cfg(digi_handle->s16_sys_handle,port_data_ptr->s16_clk_cfg.pin_index, s16_port_mode, &patt_gen_cfg);
                }
            }
            break;
        case DIGI_SERDES_SFI_51_INTF:
            /* Line side digi_serdes_sfi51_cfg case */
            rc = digi_serdes_sfi51_cfg(digi_handle, 
                                       enable_mask, 
                                       mode, 
                                       DIGI_SERDES_MASK_NONE);
            break;
        default:
            rc = DIGI_ERR_INVALID_ARG;
            break;
        }
    }
    


    PMC_RETURN(rc);
} /* digi_serdes_port_prov */

/*******************************************************************************
* digi_serdes_port_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   a serdes port for a major mode of operation as specified by the enum
*   digi_serdes_port_mode_t.
*
*   For applications that require multiple serdes ports, this API must be called
*   once for each API within the set of serdes.
*
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   *port_ctxt_ptr     - pointer to a Serdes ctxt instance
*
* OUTPUTS:
*    None.
*
* RETURNS:
*    None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_port_deprov(digi_handle_t *digi_handle,
                                         digi_serdes_port_t *port_ctxt_ptr)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    util_serdes_patt_gen_cfg_t patt_gen_cfg;
    digi_serdes_port_def_t* port_data_ptr = (digi_serdes_port_def_t*)port_ctxt_ptr;
    digi_serdes_intf_t intf = port_data_ptr->intf;
    UINT16 disable_mask;
    s16_port_mode_t s16_port_mode = (s16_port_mode_t)0;

    PMC_ENTRY();

    /* identify the set of serdes pins assigned to the port */
    rc = digi_port_pin_bit_mask_s16_clk_get(digi_handle, (digi_serdes_port_def_t*)port_ctxt_ptr, &disable_mask);

    if (rc == PMC_SUCCESS) {

        switch (intf) {
        case DIGI_SERDES_LINE_INTF:
            if (DIGI_SERDES_PIN_NOT_USED != port_data_ptr->s16_clk_cfg.pin_index)  {
                digi_serdes_s16_clk_patt_gen_cfg_get(digi_handle,FALSE,port_data_ptr->s16_clk_cfg.divider,&patt_gen_cfg);
                (void)s16_patt_gen_cfg(digi_handle->s16_line_handle,port_data_ptr->s16_clk_cfg.pin_index, s16_port_mode, &patt_gen_cfg);
            }
            /* Line side digi_serdes_s16_cfg case */
            rc = digi_serdes_s16_cfg(digi_handle, 
                                     HOSTMSG_S16_SERDES_INTF_LINE, 
                                     DIGI_SERDES_MASK_NONE, 
                                     DIGI_SERDES_PORT_MODE_ANY_LOWPWR, 
                                     disable_mask);
            break;
        case DIGI_SERDES_SYSTEM_INTF:
            if (DIGI_SERDES_PIN_NOT_USED != port_data_ptr->s16_clk_cfg.pin_index)  {
                digi_serdes_s16_clk_patt_gen_cfg_get(digi_handle,FALSE,port_data_ptr->s16_clk_cfg.divider,&patt_gen_cfg);
                (void)s16_patt_gen_cfg(digi_handle->s16_sys_handle,port_data_ptr->s16_clk_cfg.pin_index, s16_port_mode, &patt_gen_cfg);
            }
            /* System side digi_serdes_s16_cfg case */
            rc = digi_serdes_s16_cfg(digi_handle, 
                                     HOSTMSG_S16_SERDES_INTF_SYS, 
                                     DIGI_SERDES_MASK_NONE, 
                                     DIGI_SERDES_PORT_MODE_ANY_LOWPWR, 
                                     disable_mask);
            break;
        case DIGI_SERDES_SFI_51_INTF:
            /* Line side digi_serdes_sfi51_cfg case */
            rc = digi_serdes_sfi51_cfg(digi_handle, 
                                       DIGI_SERDES_MASK_NONE, 
                                       DIGI_SERDES_PORT_MODE_ANY_LOWPWR, 
                                       disable_mask);
            break;
        default:
            rc = DIGI_ERR_INVALID_ARG;
            break;
        }
    }

    if (PMC_SUCCESS == rc)
    {
        rc = digi_serdes_port_rate_state_set(digi_handle, port_ctxt_ptr, LAST_DIGI_SERDES_PORT_MODE, DIGI_SERDES_PROV_STATE_UNASSIGNED, DIGI_SERDES_PORT_RATE_STATE_DEPROV);
    }

    PMC_RETURN(rc);
} /* digi_serdes_port_deprov */

/*******************************************************************************
* digi_enet_serdes_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine the DIGI SERDES mode provided the
*   ENET mode.
*
*
* INPUTS:
*   enet_mode        - the ENET mode to translate
*
* OUTPUTS:
*   serdes_mode      - the corresponding DIGI SERDES mode.
* 
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_enet_serdes_mode_get(digi_enet_mode_t enet_mode,
                                      digi_serdes_port_mode_t* serdes_mode)
{

    PMC_ENTRY();

    switch (enet_mode) {

        /* 100G cases */
    case DIGI_ENET_SERDES_100GE_MAC:
    case DIGI_ENET_SERDES_100GE_TRANSPARENT:
    case DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
        *serdes_mode = DIGI_SERDES_PORT_MODE_S16_100_GE;
        break;

        /* 40G cases */
    case DIGI_ENET_SERDES_40GE_MAC:
    case DIGI_ENET_SERDES_40GE_TRANSPARENT:
    case DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
        *serdes_mode = DIGI_SERDES_PORT_MODE_S16_40_GE;
        break;

        /* 10G cases */
    case DIGI_ENET_SERDES_10GE_MAC:
    case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
    case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
    case DIGI_ENET_SERDES_10GE_TRANSPARENT:
    case DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
        *serdes_mode = DIGI_SERDES_PORT_MODE_S16_10_GE;    
        break;
    default:
        PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
        break;
    }

    PMC_RETURN();
} /* digi_enet_serdes_mode_get */
    
/*******************************************************************************
* digi_cbrc_serdes_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine the DIGI SERDES mode provided the
*   CBRC mode.
*
*
* INPUTS:
*   cbrc_mode        - the CBRC mode to translate
*   lane_type        - specifies the serdes type (line, system, SFI-5.1)
*
* OUTPUTS:
*   serdes_mode      - the corresponding DIGI SERDES mode.
* 
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_cbrc_serdes_mode_get(digi_cbr_client_mode_t cbrc_mode,
                                      digi_serdes_intf_t lane_type,
                                      digi_serdes_port_mode_t* serdes_mode)
{

    PMC_ENTRY();

    if (lane_type == DIGI_SERDES_SFI_51_INTF) {

        *serdes_mode = DIGI_SERDES_PORT_MODE_D8_OC_768;

    } else {

        switch (cbrc_mode) {

        case DIGI_CBR_CLIENT_10G_FC800:
        case DIGI_CBR_CLIENT_10G_8B_10B_IDLE:
        case DIGI_CBR_CLIENT_10G_8B_10B_ARB:      
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_FC_800;
            break;
        case DIGI_CBR_CLIENT_10G_FC1200_ODU1F:
        case DIGI_CBR_CLIENT_10G_FC1200_ODU2F:
        case DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E:
		case DIGI_CBR_CLIENT_10G_64B_66B_IDLE:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_FC_1200;
            break;
        case DIGI_CBR_CLIENT_10G_GDPS_10G:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_10G_GDPS;
            break;
        case DIGI_CBR_CLIENT_10G_GDPS_5G :
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_5G_GDPS;
            break;
        case DIGI_CBR_CLIENT_10G_CPRI_5G:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_CPRI_5G;
            break;
        case DIGI_CBR_CLIENT_10G_CPRI_10G:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_CPRI_10G;
            break;
        case DIGI_CBR_CLIENT_10G_CPRI_6G:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_CPRI_6G;
            break;
            /* 2x40G CBR cases */
        case DIGI_CBR_CLIENT_40G_STS768:
        case DIGI_CBR_CLIENT_40G_STM256:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_OC_768;
            break;
            /* 4x10G CBR cases */
        case DIGI_CBR_CLIENT_10G_STS192:
        case DIGI_CBR_CLIENT_10G_STM64:
        case DIGI_CBR_CLIENT_10G_STS192_PRBS:
        case DIGI_CBR_CLIENT_10G_STM64_PRBS:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_OC_192;
            break;

        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
        }
    }

    PMC_RETURN();
} /* digi_cbrc_serdes_mode_get */

/*******************************************************************************
* digi_otn_serdes_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine the DIGI SERDES mode provided the
*   OTN mode.
*
*
* INPUTS:
*   otuk_mode         - the OTN mode to translate
*   lane_type         - serdes interface type
*
* OUTPUTS:
*   serdes_mode      - the corresponding DIGI SERDES mode.
* 
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_otn_serdes_mode_get(digi_otn_server_otuk_t otuk_mode,
                                     digi_serdes_intf_t lane_type,
                                     digi_serdes_port_mode_t* serdes_mode)
{
    PMC_ENTRY();

    if (lane_type == DIGI_SERDES_SFI_51_INTF) {

        switch (otuk_mode) {
        case DIGI_OTN_SERVER_OTU3:
            *serdes_mode = DIGI_SERDES_PORT_MODE_D8_OTU3;
            break;
        case DIGI_OTN_SERVER_OTU3E2:
            *serdes_mode = DIGI_SERDES_PORT_MODE_D8_OTU3E2;
            break;
        case DIGI_OTN_SERVER_OTU3E1:
            *serdes_mode = DIGI_SERDES_PORT_MODE_D8_OTU3E1;
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
        }

    } else {

        switch (otuk_mode) {

        case DIGI_OTN_SERVER_OTU4:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_OTU4;
            break;
        case DIGI_OTN_SERVER_OTU3:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_OTU3;
            break;
        case DIGI_OTN_SERVER_OTU3E2:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_OTU3E2;
            break;
        case DIGI_OTN_SERVER_OTU3E1:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_OTU3E1;
            break;
        case DIGI_OTN_SERVER_OTU2:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_OTU2;
            break;
        case DIGI_OTN_SERVER_OTU2E:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_OTU2E;
            break;
        case DIGI_OTN_SERVER_OTU1E:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_OTU1E;
            break;
        case DIGI_OTN_SERVER_OTU2F:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_OTU2F;
            break;
        case DIGI_OTN_SERVER_OTU1F:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_OTU1F;
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;

        }
    }

    PMC_RETURN();
} /* digi_otn_serdes_mode_get */


/*******************************************************************************
* digi_mapotn_serdes_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine the DIGI SERDES mode provided the
*   CBRC mode.
*
*
* INPUTS:
*   map_mode        - the MAPOTN mode to translate
*   lane_type        - specifies the serdes type (line, system, SFI-5.1)
*
* OUTPUTS:
*   serdes_mode      - the corresponding DIGI SERDES mode.
* 
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_mapotn_serdes_mode_get(digi_mapping_mode_t map_mode,
                                        digi_serdes_intf_t lane_type,
                                        digi_serdes_port_mode_t* serdes_mode)
{

    PMC_ENTRY();

    if (lane_type == DIGI_SERDES_SFI_51_INTF) {

        *serdes_mode = DIGI_SERDES_PORT_MODE_D8_OC_768;

    } else {
        
        switch(map_mode) {
        case DIGI_MAP_ODUKP_PACKET_GFP:
            break;
        case DIGI_MAP_ODU4P_100_GE_GFP:
        case DIGI_MAP_ODU4P_CBR_100_GE_GMP:
        case DIGI_MAP_ODU4P_PKT_100_GE_GMP:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_100_GE;
            break;
        case DIGI_MAP_ODU3P_40_GE_GFP:
        case DIGI_MAP_ODU3P_CBR_40_GE_GMP:
        case DIGI_MAP_ODU3P_PKT_40_GE_GMP:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_40_GE;
            break;
        case DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP:
        case DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP:
        case DIGI_MAP_ODU2P_10_GE_6_2_GFP:
        case DIGI_MAP_ODU1EP_CBR_10G3_7_2_BMP:
        case DIGI_MAP_ODU2EP_CBR_10G3_7_1_BMP:
        case DIGI_MAP_ODU1EP_PKT_10G3_7_2_BMP:
        case DIGI_MAP_ODU2EP_PKT_10G3_7_1_BMP:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_10_GE;
            break;
        case DIGI_MAP_ODU2EP_FC_1200_GFP:
        case DIGI_MAP_ODU2FP_FC_1200_BMP:
        case DIGI_MAP_ODU1FP_FC_1200_BMP:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_FC_1200;
            break;
        case DIGI_MAP_ODU2P_FC_800_GMP:
        case DIGI_MAP_ODUFLEX_FC_800_BMP:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_FC_800;
            break;
        case DIGI_MAP_ODU2EP_10G_GDPS_GMP:
        case DIGI_MAP_ODUFLEX_10G_GDPS_BMP:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_10G_GDPS;
            break;
        case DIGI_MAP_ODU2P_5G_GDPS_GMP:
        case DIGI_MAP_ODUFLEX_5G_GDPS_BMP:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_5G_GDPS;
            break;
        case DIGI_MAP_ODU2P_CPRI_5_GMP:
        case DIGI_MAP_ODUFLEX_CPRI_5_BMP:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_CPRI_5G;
            break;
        case DIGI_MAP_ODU2P_CPRI_10_GMP:
        case DIGI_MAP_ODUFLEX_CPRI_10_BMP:    
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_CPRI_10G;
            break;
        case DIGI_MAP_ODU2P_CPRI_6_GMP:
        case DIGI_MAP_ODUFLEX_CPRI_6_BMP:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_CPRI_6G;
            break;
        case DIGI_MAP_ODU3P_OC_768_CBR_BMP:
        case DIGI_MAP_ODU3P_OC_768_CBR_AMP:    
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_OC_768;
            break;
        case DIGI_MAP_ODU2P_OC_192_CBR_BMP:
        case DIGI_MAP_ODU2P_OC_192_CBR_AMP:
            *serdes_mode = DIGI_SERDES_PORT_MODE_S16_OC_192;
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
    
        }
    }

    PMC_RETURN();
} /* digi_mapotn_serdes_mode_get */

/*******************************************************************************
* digi_serdes_num_10g_data_lane_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine the number of (10G) SERDES data lane 
*   for a given ODUk rate
*
* INPUTS:
*   odu_type         - the oduk data enum rate
*
* OUTPUTS:
*   None
* 
* RETURNS:
*   The number of 10G serdes to carry to the specified rate
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 digi_serdes_num_10g_data_lane_get(util_global_odukp_type_t odu_type )
{
    UINT32  num_data_lane;
    PMC_ENTRY();
    
    if (odu_type >= UTIL_GLOBAL_ODU4) 
    {
        num_data_lane = 10;
    } else if (odu_type >= UTIL_GLOBAL_ODU3)
    {
        num_data_lane = 4;
    } else 
    {
        num_data_lane = 1;
    };
    
    PMC_RETURN(num_data_lane);
} /* digi_serdes_num_10g_data_lane_get */

#ifdef PMC_DEPRECATED

/*******************************************************************************
* digi_serdes_all_zeros_or_ones_det_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED-Function has been marked as deprecated starting in the 3.02 release
*   This function configures enet channel serdes pattern monitor.
*   When PRBS is enabled, all-zeroes or all-ones pattern detection is activated
*   in enet channels associated serdes. When PRBS is disabled, pattern detection
*   is disabled.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*    enable                - When TRUE serdes user-pattern monitor is configured
*                            otherwise it is disabled.
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

/* LCOV_EXCL_START */

PUBLIC PMC_ERROR  digi_serdes_all_zeros_or_ones_det_cfg(digi_handle_t              *digi_handle,
                                                        util_global_switch_data_t  *chnl_handle,
                                                        BOOL8                       enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_serdes_port_t *port_ctxt_ptr;
    UINT32 i;
    UINT32 chnl;
    digi_serdes_prov_state_t prov_state;
    util_serdes_patt_mon_cfg_t patt_mon_cfg;
    BOOL8 polarity_inv;

    PMC_ENTRY();


    {        
        /* retireve serdes prov state associated to enet channel */
        chnl = ((digi_enet_client_chnl_def_t*) chnl_handle)->switch_data.cpb_data.channel;
        rc = digi_by_mld_index_post_mld_prov_state_get(digi_handle,
                                                       chnl,
                                                       &prov_state);
        
        /* configure serdes pattern */
        if (PMC_SUCCESS == rc)
        {
            /* depending on which ENET ss the channel is associated with: ENET_LINE or ENET_SYS */
            rc = digi_port_ctxt_from_channel_number_get(digi_handle, 
                                                        DIGI_SERDES_PROV_STATE_ENET_LINE, 
                                                        chnl, &port_ctxt_ptr);
            if (PMC_SUCCESS != rc)
            {
                rc = digi_port_ctxt_from_channel_number_get(digi_handle, 
                                                            DIGI_SERDES_PROV_STATE_ENET_SYS, 
                                                            chnl, &port_ctxt_ptr); 
            }
            if (PMC_SUCCESS != rc)
            {
                rc = digi_port_ctxt_from_channel_number_get(digi_handle, 
                                                            DIGI_SERDES_PROV_STATE_CBRC, 
                                                            chnl, &port_ctxt_ptr);
            }

            if (PMC_SUCCESS == rc)
            {
            
                /* Prepare serdes pattern monitor configuration */
                if (TRUE == enable)
                {
                    patt_mon_cfg.patt_cfg.enable = TRUE;
                    patt_mon_cfg.patt_cfg.mode = UTIL_PATT_MODE_USER_DEF;
                    patt_mon_cfg.patt_cfg.usr_patt_size = UTIL_PATTERN_GEN_MON_USER_DATA_MAX_BIT_SIZE;                    
                }            
                else
                {
                    patt_mon_cfg.patt_cfg.enable = FALSE;
                }

                /* retrieve the right serdes status */
                switch (((digi_serdes_port_def_t *)port_ctxt_ptr)->intf)
                {
                case DIGI_SERDES_LINE_INTF:            
                    for (i = 0; i < DIGI_SERDES_XFI_LANES_MAX && PMC_SUCCESS == rc; i++)
                    {
                        if (((digi_serdes_port_def_t *)port_ctxt_ptr)->port_uid == digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].port_uid)
                        {
                            if (DIGI_SERDES_PIN_NOT_USED != digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].module_lane_rx_index)
                            {
                                if (TRUE == enable)
                                {
                                    /* retrieve serdes polarity */
                                    rc = digi_serdes_line_sys_polarity_internal_get(
                                        digi_handle, port_ctxt_ptr, 
                                        digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].module_lane_rx_index, 
                                        UTIL_GLOBAL_DIR_RX,
                                        &polarity_inv);
                                    if (PMC_SUCCESS == rc)
                                    {
                                        /* user-pattern is all-zeroes or all-ones depending on serdes polarity */

                                        patt_mon_cfg.patt_cfg.inv_cfg = polarity_inv;
                                        PMC_MEMSET(&patt_mon_cfg.patt_cfg.usr_patt, 
                                                   0xff,
                                                   sizeof(UINT32) * UTIL_PATTERN_GEN_MON_USER_DATA_S16_WORD_SIZE);
                                        PMC_LOG_TRACE("about to configure serdes pattern monitor index = %d, inv_pol = %d\n", digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].module_lane_rx_index, polarity_inv);
                                        
                                        /* configure associated serdes pattern monitor to detect all-zeroes or all-ones pattern */
                                        rc = digi_serdes_line_sys_patt_all_ones_detection_cfg(digi_handle, 
                                                                                              port_ctxt_ptr, 
                                                                                              digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].module_lane_rx_index, 
                                                                                              &patt_mon_cfg);
                                    }
                                }
                                else
                                {
                                    /* configure associated serdes pattern monitor to detect all-zeroes or all-ones pattern */
                                    rc = digi_serdes_line_sys_patt_all_ones_detection_cfg(digi_handle, 
                                                                                          port_ctxt_ptr, 
                                                                                          digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].module_lane_rx_index, 
                                                                                          &patt_mon_cfg);
                                }
                            }
                        } 
                    }        
                    break;

                case DIGI_SERDES_SYSTEM_INTF:            
                    for (i = 0; i < DIGI_SERDES_SYS_LANES_MAX && PMC_SUCCESS == rc; i++)
                    {
                        if (((digi_serdes_port_def_t *)port_ctxt_ptr)->port_uid == digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].port_uid)
                        {
                            if (DIGI_SERDES_PIN_NOT_USED != digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].module_lane_rx_index)
                            {
                                if (TRUE == enable)
                                {
                                    /* retrieve serdes polarity */
                                    rc = digi_serdes_line_sys_polarity_internal_get(
                                        digi_handle, port_ctxt_ptr, 
                                        digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].module_lane_rx_index, 
                                        UTIL_GLOBAL_DIR_RX,
                                        &polarity_inv);
                                    if (PMC_SUCCESS == rc)
                                    {
                                        /* user-pattern is all-zeroes or all-ones depending on serdes polarity */
                                        patt_mon_cfg.patt_cfg.inv_cfg = polarity_inv;
                                        PMC_MEMSET(&patt_mon_cfg.patt_cfg.usr_patt,
                                                   0xff,
                                                   sizeof(UINT32) * UTIL_PATTERN_GEN_MON_USER_DATA_S16_WORD_SIZE);
                                               
                                        /* configure associated serdes pattern monitor to detect all-zeroes or all-ones pattern */
                                        rc = digi_serdes_line_sys_patt_all_ones_detection_cfg(digi_handle, 
                                                                                              port_ctxt_ptr, 
                                                                                              digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].module_lane_rx_index, 
                                                                                              &patt_mon_cfg);
                                    }
                                }
                                else
                                {
                                    /* configure associated serdes pattern monitor to detect all-zeroes or all-ones pattern */
                                    rc = digi_serdes_line_sys_patt_all_ones_detection_cfg(digi_handle, 
                                                                                          port_ctxt_ptr, 
                                                                                          digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].module_lane_rx_index, 
                                                                                          &patt_mon_cfg);
                                }
                            } 
                        }
                    }
                    break;

                case DIGI_SERDES_SFI_51_INTF:            
                    
                    for (i = 0; i < DIGI_NUM_SFI51_PORTS_MAX && PMC_SUCCESS == rc; i++)
                    {
                        if (((digi_serdes_port_def_t *)port_ctxt_ptr)->port_uid == digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].port_uid)
                        {
                            if (DIGI_SERDES_PIN_NOT_USED != digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].lane_swap_rx)
                            {
                                if (TRUE == enable)
                                {
                                    /* retrieve serdes polarity */
                                    rc = digi_serdes_line_sys_polarity_internal_get(
                                        digi_handle, port_ctxt_ptr, 
                                        digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].lane_swap_rx, 
                                        UTIL_GLOBAL_DIR_RX,
                                        &polarity_inv);
                                    if (PMC_SUCCESS == rc)
                                    {
                                        /* user-pattern is all-zeroes or all-ones depending on serdes polarity */
                                        patt_mon_cfg.patt_cfg.inv_cfg = polarity_inv;
                                        PMC_MEMSET(&patt_mon_cfg.patt_cfg.usr_patt, 
                                                   0xff, 
                                                   sizeof(UINT32) * D8_RX_2X_USR_PATT_BIT_SIZE);
                                        /* configure associated serdes pattern monitor to detect all-zeroes or all-ones pattern */
                                        rc = digi_serdes_line_sys_patt_mon_internal_prov(
                                            digi_handle, port_ctxt_ptr, 
                                            digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].lane_swap_rx, 
                                            &patt_mon_cfg);
                                    }
                                }
                                else
                                {
                                    /* deactivate serdes pattern monitor */
                                    rc = digi_serdes_line_sys_patt_mon_internal_deprov(digi_handle, port_ctxt_ptr, 
                                                                                       digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].lane_swap_rx);
                                }
                            } 
                        }
                    }
                    break;
                default:
                    rc = DIGI_ERR_INVALID_ARG;
                    break;
                }
            }
        } 
    }

    PMC_RETURN(rc);
}/*  digi_serdes_all_zeros_or_ones_det_cfg */

/* LCOV_EXCL_STOP */

/*******************************************************************************
* digi_serdes_all_zeros_or_ones_det_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED-Function has been marked as deprecated starting in the 3.02 release
*   This function retrieves enet channel serdes all-zeroes or all-ones pattern 
*   monitor status. 
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   *status                - Serdes PRBS monitor status: LOCKED or unkown
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

/* LCOV_EXCL_START */

PUBLIC PMC_ERROR digi_serdes_all_zeros_or_ones_det_status_get(digi_handle_t              *digi_handle,
                                                              util_global_switch_data_t  *chnl_handle,
                                                              util_patt_status_t         *status)
{    
    PMC_ERROR rc = PMC_SUCCESS;
    digi_serdes_port_t *port_ctxt_ptr;
    UINT32 i;
    UINT32 chnl;
    digi_serdes_prov_state_t prov_state;
    UINT32 err_cnt;
    BOOL8 polarity_inv;

    PMC_ENTRY();

    *status = LAST_UTIL_PATT_STATUS;
    
            
    /* Check associated serdes PRBS monitor status */
    if (PMC_SUCCESS == rc)
    {
        
        /* retireve serdes prov state associated to enet channel */
        chnl = ((digi_enet_client_chnl_def_t*) chnl_handle)->switch_data.cpb_data.channel;
        rc = digi_by_mld_index_post_mld_prov_state_get(digi_handle,
                                                       chnl,
                                                       &prov_state);
    
        if (PMC_SUCCESS == rc)
        {
            rc = digi_port_ctxt_from_channel_number_get(digi_handle, 
                                                        DIGI_SERDES_PROV_STATE_ENET_LINE, 
                                                        chnl, &port_ctxt_ptr);
        
            if (PMC_SUCCESS != rc)
            {
                rc = digi_port_ctxt_from_channel_number_get(digi_handle, 
                                                            DIGI_SERDES_PROV_STATE_ENET_SYS, 
                                                            chnl, &port_ctxt_ptr);
            }
            if (PMC_SUCCESS != rc)
            {
                rc = digi_port_ctxt_from_channel_number_get(digi_handle, 
                                                            DIGI_SERDES_PROV_STATE_CBRC, 
                                                            chnl, &port_ctxt_ptr);       
            }
        }
    
    
        /* retrieve serdes pattern monitor status for all serdes associated to enet channel */
        if (PMC_SUCCESS == rc)
        {
            /* depending on which ENET ss the channel is associated with: ENET_LINE or ENET_SYS */
            /* retrieve the right serdes status */
            switch (((digi_serdes_port_def_t *)port_ctxt_ptr)->intf)
            {
            case DIGI_SERDES_LINE_INTF: 
                for (i = 0; i < DIGI_SERDES_XFI_LANES_MAX && PMC_SUCCESS == rc; i++)
                {
                    if (((digi_serdes_port_def_t *)port_ctxt_ptr)->port_uid == 
                        digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].port_uid)
                    {
                        if (DIGI_SERDES_PIN_NOT_USED != digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].module_lane_rx_index)
                        {
                            /* retrieve serdes pattern monitor status */
                            rc = digi_serdes_line_sys_patt_mon_err_count_internal_get(digi_handle, port_ctxt_ptr, 
                                                                                      digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].module_lane_rx_index, status, &err_cnt);   
                            
                            if (PMC_SUCCESS == rc)
                            {                
                                /* if one serdes pattern monitor is not locked then return unknow status */ 
                                if (UTIL_PATT_STATUS_LOCKED == *status && 0 == err_cnt)
                                {

                                    /* retrieve serdes polarity */
                                    rc = digi_serdes_line_sys_polarity_internal_get(
                                        digi_handle, port_ctxt_ptr, 
                                        digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].module_lane_rx_index, 
                                        UTIL_GLOBAL_DIR_RX,
                                        &polarity_inv);
                                    if (PMC_SUCCESS == rc)
                                    {
                                        if (TRUE == polarity_inv)
                                        {
                                            *status = UTIL_PATT_STATUS_ZERO_DET;         
                                        } 
                                        else
                                        {
                                            *status = UTIL_PATT_STATUS_ONES_DET;
                                        }
                                        break;
                                    }
                                }                                
                            }        
                        }                           
                    }
                }   
                break;
                
            case DIGI_SERDES_SYSTEM_INTF:       
                for (i = 0; i < DIGI_SERDES_SYS_LANES_MAX && PMC_SUCCESS == rc; i++)
                {
                    if (((digi_serdes_port_def_t *)port_ctxt_ptr)->port_uid == digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].port_uid)
                    {
                        if (DIGI_SERDES_PIN_NOT_USED != digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].module_lane_rx_index)
                        {  
                            /* retrieve serdes pattern monitor status */
                            rc = digi_serdes_line_sys_patt_mon_err_count_internal_get(digi_handle, port_ctxt_ptr, 
                                                                                      digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].module_lane_rx_index, 
                                                                                      status, &err_cnt);                                  
                            if (PMC_SUCCESS == rc)
                            {
                                /* if one serdes pattern monitor is not locked then return unknow status */ 
                                if (UTIL_PATT_STATUS_LOCKED == *status &&
                                    0 == err_cnt)
                                {

                                    /* retrieve serdes polarity */
                                    rc = digi_serdes_line_sys_polarity_internal_get(
                                        digi_handle, port_ctxt_ptr, 
                                        digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].module_lane_rx_index,
                                        UTIL_GLOBAL_DIR_RX,
                                        &polarity_inv);
                                    if (PMC_SUCCESS == rc)
                                    {
                                        if (TRUE == polarity_inv)
                                        {
                                            *status = UTIL_PATT_STATUS_ZERO_DET;         
                                        } 
                                        else
                                        {
                                            *status = UTIL_PATT_STATUS_ONES_DET;
                                        }
                                        break;
                                    }
                                }                                                            
                            }
                        } 
                    }
                }
                break;
            case DIGI_SERDES_SFI_51_INTF:       
                for (i = 0; i < DIGI_NUM_SFI51_PORTS_MAX && PMC_SUCCESS == rc; i++)
                {
                    if (((digi_serdes_port_def_t *)port_ctxt_ptr)->port_uid == digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].port_uid)
                    {
                        if (DIGI_SERDES_PIN_NOT_USED != digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].lane_swap_rx)
                        {  
                            /* retrieve serdes pattern monitor status */
                            rc = digi_serdes_line_sys_patt_mon_err_count_internal_get(digi_handle, port_ctxt_ptr, 
                                                                                      digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].lane_swap_rx, 
                                                                                      status, &err_cnt);                                  
                            if (PMC_SUCCESS == rc)
                            {
                                /* if one serdes pattern monitor is not locked then return unknow status */ 
                                if (UTIL_PATT_STATUS_LOCKED == *status &&
                                    0 == err_cnt)
                                {

                                    /* retrieve serdes polarity */
                                    rc = digi_serdes_line_sys_polarity_internal_get(
                                        digi_handle, port_ctxt_ptr, 
                                        digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].lane_swap_rx,                                         
                                        UTIL_GLOBAL_DIR_RX,
                                        &polarity_inv);
                                    if (PMC_SUCCESS == rc)
                                    {
                                        if (TRUE == polarity_inv)
                                        {
                                            *status = UTIL_PATT_STATUS_ZERO_DET;         
                                        } 
                                        else
                                        {
                                            *status = UTIL_PATT_STATUS_ONES_DET;
                                        }
                                        break;
                                    }                                    
                                }                    
                            }
                        } 
                    }
                }
                
                break;
            default:
                rc = DIGI_ERR_INVALID_ARG;
                break;
            }/*switch(intf)*/
        }
    }

    PMC_RETURN(rc);
}/* digi_serdes_all_zeros_or_ones_det_status_get */

/* LCOV_EXCL_STOP */

#endif /* PMC_DEPRECATED */

/*******************************************************************************
* digi_is_line_cage_id_unique
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a helper function for the the digi data path layer API.
*   This function determines if the cage ids defined in the xfi cage
*   are indeed unique across the device.
*
* INPUTS:
*    *h                 - pointer to DIGI handle instance
*    cage_id            - the cage_id to check
*
* OUTPUTS:
*    None.
*
* RETURNS:
*   PMC_SUCCESS on SUCCESS or specific error code during failure
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL digi_is_line_cage_id_unique(digi_handle_t *h,
                                         UINT8 cage_id)
{
    BOOL rc = TRUE;
    UINT8 i = 0; /* counter */
    UINT8 j = 0; /* counter */

    BOOL sys_cage_prov = h->var.cage_cfg.is_sys_cage_cfg;

    PMC_ENTRY();

    for (i = 0; i < DIGI_SERDES_XFI_LANES_MAX; i++) {

        if (sys_cage_prov == TRUE) {

            /* Check the sys_cage_prov for dups */
            for (j = 0; j < DIGI_SERDES_SYS_LANES_MAX; j++) {

                if (cage_id == h->var.pin_bus_ctxt.xfi_sys_pin_ctxt[j].cage_id) {

                    rc = FALSE;
                    break;
                }
            }
        }
    }

    PMC_RETURN(rc);
} /* digi_is_line_cage_id_unique */

/*******************************************************************************
* digi_is_sys_cage_id_unique
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a helper function for the the digi data path layer API.
*   This function determines if the cage ids defined in the sys cage
*   are indeed unique across the device.
*
* INPUTS:
*    *h                 - pointer to DIGI handle instance
*    cage_id            - the cage_id to check
*
* OUTPUTS:
*    None.
*
* RETURNS:
*   PMC_SUCCESS on SUCCESS or specific error code during failure
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL digi_is_sys_cage_id_unique(digi_handle_t *h,
                                        UINT8 cage_id)
{
    BOOL rc = TRUE;
    UINT8 i = 0; /* counter */
    UINT8 j = 0; /* counter */
    BOOL xfi_cage_prov = h->var.cage_cfg.is_line_cage_cfg;

    PMC_ENTRY();

    for (i = 0; i < DIGI_SERDES_SYS_LANES_MAX; i++) {

        if (xfi_cage_prov == TRUE) {

            /* Check the xfi_cage_prov for dups */
            for (j = 0; j < DIGI_SERDES_XFI_LANES_MAX; j++) {

                if (cage_id == h->var.pin_bus_ctxt.xfi_line_pin_ctxt[j].cage_id) {

                    rc = FALSE;
                    break;
                }
            }
        }
    }

    PMC_RETURN(rc);
} /* digi_is_sys_cage_id_unique */


/*******************************************************************************
* digi_line_cage_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a helper function for the the digi data path layer API.
*   This function validates the cage configuration.
*
* INPUTS:
*    *h                 - pointer to DIGI handle instance
*    cage_id_per_pin    - Array defining upto 12 unique
*                         cages connected to interface specified with intf.
*                         The array is indexed by serdes pin number.  Should
*                         a pin not be associated with a cage, the value
*                         DIGI_SERDES_PIN_NOT_USED (0xFF) may be used to
*                         define 'Not Connected'.
*    *is_cfp            - Returns TRUE if it is possible to provision a CFP
*                         port against this cage configuration.
*
* OUTPUTS:
*    NONE.
*
* RETURNS:
*   PMC_SUCCESS on SUCCESS or specific error code during failure
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_line_cage_validate(digi_handle_t *h,
                                          UINT8 cage_id_per_pin[DIGI_SERDES_XFI_LANES_MAX],
                                          BOOL *is_cfp)
{
    UINT8 i = 0;
    UINT8 j = 0;
    UINT8 pin_count = 0;
    BOOL pin_used = FALSE;
    BOOL dup_found = FALSE;
    BOOL invalid_pin_configuration = FALSE;
    PMC_ERROR rc = DIGI_ERR_SERDES_XFI_NO_PIN_PROV;

    PMC_ENTRY();

    for (i = 0; i < DIGI_SERDES_XFI_LANES_MAX; i++) {

        pin_count = 0;
        if (cage_id_per_pin[i] != DIGI_SERDES_PIN_NOT_USED) {

            /* The user has specified at least one valid pin. */
            pin_used = TRUE;

            /* Check the other provisioned cages for id dups */
            if (digi_is_line_cage_id_unique(h, cage_id_per_pin[i]) == FALSE) {

                dup_found = TRUE;
                break;
            }

            for (j = 0; j < DIGI_SERDES_XFI_LANES_MAX; j++) {

                if (cage_id_per_pin[j] == cage_id_per_pin[i]) {

                    pin_count++;
                }
            }

            if ((pin_count == DIGI_VALID_CAGE_PIN_CASE_1) ||
                (pin_count == DIGI_VALID_CAGE_PIN_CASE_2) ||
                (pin_count == DIGI_VALID_CAGE_PIN_CASE_3)){
                
                /* These are all fine configurations, nothing to do */
            } else if ((pin_count == DIGI_VALID_CAGE_PIN_CASE_4) ||
                       (pin_count == DIGI_VALID_CAGE_PIN_CASE_5) ||
                       (pin_count == DIGI_VALID_CAGE_PIN_CASE_6)) {

                for (j = 0; j < DIGI_SERDES_XFI_LANES_MAX; j++) {

                    if ((cage_id_per_pin[j] != cage_id_per_pin[i]) &&
                        (cage_id_per_pin[j] != DIGI_SERDES_PIN_NOT_USED)) {

                        /* If you have CFP configured, you cannot have
                           any other cages */
                        invalid_pin_configuration = TRUE;
                        break;
                    }
                }
                *is_cfp = TRUE;

            } else {

                invalid_pin_configuration = TRUE;
                break;
            }
        }
    }

    if ((pin_used == TRUE) && (dup_found == FALSE)) {

        rc = PMC_SUCCESS;

    } else if (dup_found == TRUE) {

        rc = DIGI_ERR_SERDES_CAGE_ALREADY_PROV;

    } else if (invalid_pin_configuration) {

        rc = DIGI_ERR_INVALID_ARG;
    }

    PMC_RETURN(rc);
} /* digi_xfi_cage_validate */

/*******************************************************************************
* digi_sys_cage_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a helper function for the the digi data path layer API.
*   This function validates the cage configuration.
*
* INPUTS:
*    *h                 - pointer to DIGI handle instance
*    cage_id_per_pin    - Array defining upto 12 unique
*                         cages connected to interface specified with intf.
*                         The array is indexed by serdes pin number.  Should
*                         a pin not be associated with a cage, the value
*                         DIGI_SERDES_PIN_NOT_USED (0xFF) may be used to
*                         define 'Not Connected'.
*    *is_cfp            - Returns TRUE if it is possible to provision a CFP
*                         port against this cage configuration.
*
* OUTPUTS:
*    NONE.
*
* RETURNS:
*   PMC_SUCCESS on SUCCESS or specific error code during failure
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_sys_cage_validate(digi_handle_t *h,
                                         UINT8 cage_id_per_pin[DIGI_SERDES_SYS_LANES_MAX],
                                         BOOL *is_cfp)
{
    UINT8 i = 0;
    UINT8 j = 0;
    UINT8 pin_count = 0;
    BOOL pin_used = FALSE;
    BOOL dup_found = FALSE;
    BOOL invalid_pin_configuration = FALSE;
    PMC_ERROR rc = DIGI_ERR_SERDES_SYS_NO_PIN_PROV;

    PMC_ENTRY();

    for (i = 0; i < DIGI_SERDES_SYS_LANES_MAX; i++) {

        pin_count = 0;
        if (cage_id_per_pin[i] != DIGI_SERDES_PIN_NOT_USED) {

            /* The user has specified at least one valid pin. */
            pin_used = TRUE;

            /* Check the other provisioned cages for id dups */
            if (digi_is_sys_cage_id_unique(h, cage_id_per_pin[i]) == FALSE) {

                dup_found = TRUE;
                break;
            }

            for (j = 0; j < DIGI_SERDES_SYS_LANES_MAX; j++) {

                if (cage_id_per_pin[j] == cage_id_per_pin[i]) {

                    pin_count++;
                }
            }

            if ((pin_count == DIGI_VALID_CAGE_PIN_CASE_1) ||
                (pin_count == DIGI_VALID_CAGE_PIN_CASE_2) ||
                (pin_count == DIGI_VALID_CAGE_PIN_CASE_3)) {

                /* These are all fine configurations, nothing to do */
            } else if (pin_count == DIGI_VALID_CAGE_PIN_CASE_4 ||
                       pin_count == DIGI_VALID_CAGE_PIN_CASE_5 ||
                       pin_count == DIGI_VALID_CAGE_PIN_CASE_6) {

                for (j = 0; j < DIGI_SERDES_SYS_LANES_MAX; j++) {

                    if ((cage_id_per_pin[j] != cage_id_per_pin[i]) &&
                        (cage_id_per_pin[j] != DIGI_SERDES_PIN_NOT_USED)) {

                        /* If you have CFP configured, you cannot have
                           any other cages */
                        invalid_pin_configuration = TRUE;
                        break;
                    }
                }
                *is_cfp = TRUE;

            } else {

                invalid_pin_configuration = TRUE;
                break;
            }
        }
    }

    if ((pin_used == TRUE) && (dup_found == FALSE)) {

        rc = PMC_SUCCESS;

    } else if (dup_found == TRUE) {

        rc = DIGI_ERR_SERDES_CAGE_ALREADY_PROV;

    } else if (invalid_pin_configuration) {

        rc = DIGI_ERR_INVALID_ARG;
    }

    PMC_RETURN(rc);
} /* digi_sys_cage_validate */

/*******************************************************************************
* digi_acb_capture_transmit_rate_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is an internal helper function to calculate capture rate and transmit
*   rate needed for ACB configuration. In case of DIGI_SERDES_ACB_MODE_B_AND_C
*   we also need to get the server rate.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   serdes_mode      - Configuration mode for serdes port
*   map_mode         - Enum, OPU payload mapping modes.
*   acb_timing_mode  - ACB mode of operation.
*
*
* OUTPUTS:
*  capture_rate      - capture rate
*  transmit_rate     - transmit rate
*  server_rate       - server_rate 
*  enet_mapping      - It an ENET mapping 
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_acb_capture_transmit_rate_calc(digi_handle_t *digi_handle,
                                                      digi_serdes_port_mode_t serdes_mode,
                                                      digi_mapping_mode_t map_mode,
                                                      digi_serdes_acb_mode_t acb_timing_mode,
                                                      DOUBLE *capture_rate, 
                                                      DOUBLE *transmit_rate, 
                                                      DOUBLE *server_rate,
                                                      BOOL8 *enet_mapping)
{
    DOUBLE client_rate = 0;
    DOUBLE s_rate = 0;
    DOUBLE tx_rate = 0;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_LOG_TRACE("serdes mode:%d  mapping mode:%d  acb_timing_mode:%d\n", serdes_mode, map_mode, acb_timing_mode);
    
    *enet_mapping = FALSE;

    if(DIGI_SERDES_ACB_MODE_B_ON_C == acb_timing_mode ||
       DIGI_SERDES_ACB_MODE_B_AND_C == acb_timing_mode )
    {
        switch(map_mode) {
        case DIGI_MAP_ODUKP_PACKET_GFP:
            break;
        case DIGI_MAP_ODU4P_100_GE_GFP:
            break;
        case DIGI_MAP_ODU3P_40_GE_GFP:
            break;
        case DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP:
            break;
        case DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP:
            break;
        case DIGI_MAP_ODU2P_10_GE_6_2_GFP:
            break;
        case DIGI_MAP_ODU2EP_FC_1200_GFP:
            client_rate = UTIL_GLOBAL_FC_1200_RATE;
            s_rate      = UTIL_GLOBAL_ODU2E_RATE;
            tx_rate     = UTIL_GLOBAL_FC_1200_RATE;
            break;
        case DIGI_MAP_ODU2P_FC_800_GMP:
            client_rate = UTIL_GLOBAL_FC_800_RATE;
            s_rate      = UTIL_GLOBAL_ODU2_RATE;
            tx_rate     = UTIL_GLOBAL_FC_800_RATE;
            break;
        case DIGI_MAP_ODU2EP_10G_GDPS_GMP:
            client_rate = UTIL_GLOBAL_GDPS_10G_RATE;
            s_rate      = UTIL_GLOBAL_ODU2E_RATE;
            tx_rate     = UTIL_GLOBAL_GDPS_10G_RATE;
            break;
        case DIGI_MAP_ODU2P_5G_GDPS_GMP:
            client_rate = UTIL_GLOBAL_GDPS_5G_RATE;
            s_rate      = UTIL_GLOBAL_ODU2_RATE;
            tx_rate     = UTIL_GLOBAL_GDPS_5G_RATE;
            break;
        case DIGI_MAP_ODU2P_CPRI_5_GMP:
            client_rate = UTIL_GLOBAL_CPRI_5_RATE;
            s_rate      = UTIL_GLOBAL_ODU2_RATE;
            tx_rate     = UTIL_GLOBAL_CPRI_5_RATE;
            break;
        case DIGI_MAP_ODU2P_CPRI_10_GMP:
            client_rate = UTIL_GLOBAL_CPRI_7_RATE;
            s_rate      = UTIL_GLOBAL_ODU2_RATE;
            tx_rate     = UTIL_GLOBAL_CPRI_7_RATE;
            break;
        case DIGI_MAP_ODU2P_CPRI_6_GMP:
            client_rate = UTIL_GLOBAL_CPRI_6_RATE;
            s_rate      = UTIL_GLOBAL_ODU2_RATE;
            tx_rate     = UTIL_GLOBAL_CPRI_6_RATE;
            break;
        case DIGI_MAP_ODU3P_CBR_40_GE_GMP:
            client_rate = UTIL_GLOBAL_ENET_40GE_XCD_RATE;
            s_rate      = UTIL_GLOBAL_ODU3_RATE;
            tx_rate     = UTIL_GLOBAL_ENET_40GE_RATE;
            *enet_mapping = TRUE;
            break;
        case DIGI_MAP_ODU3P_PKT_40_GE_GMP:
            break;    
        case DIGI_MAP_ODU4P_CBR_100_GE_GMP:
            client_rate = UTIL_GLOBAL_ENET_100GE_RATE;
            s_rate      = UTIL_GLOBAL_ODU4_RATE;
            tx_rate     = UTIL_GLOBAL_ENET_100GE_RATE;
            *enet_mapping = TRUE;
            break;
        case DIGI_MAP_ODU4P_PKT_100_GE_GMP:
            break;
        case DIGI_MAP_ODUFLEX_5G_GDPS_BMP:
            client_rate = UTIL_GLOBAL_GDPS_5G_RATE;
            s_rate      = (DOUBLE)239/238 * UTIL_GLOBAL_GDPS_5G_RATE;
            tx_rate     = UTIL_GLOBAL_GDPS_5G_RATE;
            break;
        case DIGI_MAP_ODUFLEX_10G_GDPS_BMP:
            client_rate = UTIL_GLOBAL_GDPS_10G_RATE;
            s_rate      = (DOUBLE)239/238 * UTIL_GLOBAL_GDPS_10G_RATE;
            tx_rate     = UTIL_GLOBAL_GDPS_10G_RATE;
            break;
        case DIGI_MAP_ODUFLEX_CPRI_5_BMP:
            client_rate = UTIL_GLOBAL_CPRI_5_RATE;
            s_rate      = (DOUBLE)239/238 * UTIL_GLOBAL_CPRI_5_RATE;
            tx_rate     = UTIL_GLOBAL_CPRI_5_RATE;
            break;
        case DIGI_MAP_ODUFLEX_CPRI_10_BMP:
            client_rate = UTIL_GLOBAL_CPRI_7_RATE;
            s_rate      = (DOUBLE)239/238 * UTIL_GLOBAL_CPRI_7_RATE;
            tx_rate     = UTIL_GLOBAL_CPRI_7_RATE;
            break;
        case DIGI_MAP_ODUFLEX_CPRI_6_BMP:
            client_rate = UTIL_GLOBAL_CPRI_6_RATE;
            s_rate      = (DOUBLE)239/238 * UTIL_GLOBAL_CPRI_6_RATE;
            tx_rate     = UTIL_GLOBAL_CPRI_6_RATE;
            break;
        case DIGI_MAP_ODUFLEX_FC_800_BMP:
            client_rate = UTIL_GLOBAL_FC_800_RATE;
            s_rate      = (DOUBLE)239/238 * UTIL_GLOBAL_FC_800_RATE;
            tx_rate     = UTIL_GLOBAL_FC_800_RATE;
            break;
        case DIGI_MAP_ODU2FP_FC_1200_BMP:
            client_rate = UTIL_GLOBAL_FC_1200_RATE;
            s_rate      = UTIL_GLOBAL_ODU2F_RATE;
            tx_rate     = UTIL_GLOBAL_FC_1200_RATE;
            break;
        case DIGI_MAP_ODU1FP_FC_1200_BMP:
            client_rate = UTIL_GLOBAL_FC_1200_RATE;
            s_rate      = UTIL_GLOBAL_ODU1F_RATE;
            tx_rate     = UTIL_GLOBAL_FC_1200_RATE;
            break;
        case DIGI_MAP_ODU1EP_CBR_10G3_7_2_BMP:
            client_rate = UTIL_GLOBAL_ENET_10GE_RATE;
            s_rate      = UTIL_GLOBAL_ODU1E_RATE;
            tx_rate     = UTIL_GLOBAL_ENET_10GE_RATE;
            *enet_mapping = TRUE;
            break;
        case DIGI_MAP_ODU2EP_CBR_10G3_7_1_BMP:
            client_rate = UTIL_GLOBAL_ENET_10GE_RATE;
            s_rate      = UTIL_GLOBAL_ODU2E_RATE;
            tx_rate     = UTIL_GLOBAL_ENET_10GE_RATE;
            *enet_mapping = TRUE;
            break;
        case DIGI_MAP_ODU1EP_PKT_10G3_7_2_BMP:
            break;
        case DIGI_MAP_ODU2EP_PKT_10G3_7_1_BMP:
            break;
        case DIGI_MAP_ODU3P_OC_768_CBR_BMP:
            client_rate = UTIL_GLOBAL_STS_768_STM_256_RATE;
            s_rate      = UTIL_GLOBAL_ODU3_RATE;
            tx_rate     = UTIL_GLOBAL_STS_768_STM_256_RATE;
            break;
        case DIGI_MAP_ODU2P_OC_192_CBR_BMP:
            client_rate = UTIL_GLOBAL_STS_192_STM_64_RATE;
            s_rate      = UTIL_GLOBAL_ODU2_RATE;
            tx_rate     = UTIL_GLOBAL_STS_192_STM_64_RATE;
            break;
        case DIGI_MAP_ODU3P_OC_768_CBR_AMP:
            client_rate = UTIL_GLOBAL_STS_768_STM_256_RATE;
            s_rate      = UTIL_GLOBAL_ODU3_RATE;
            tx_rate     = UTIL_GLOBAL_STS_768_STM_256_RATE;
            break;
        case DIGI_MAP_ODU2P_OC_192_CBR_AMP:
            client_rate = UTIL_GLOBAL_STS_192_STM_64_RATE;
            s_rate      = UTIL_GLOBAL_ODU2_RATE;
            tx_rate     = UTIL_GLOBAL_STS_192_STM_64_RATE;
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
        }
        
        if(DIGI_SERDES_ACB_MODE_B_ON_C == acb_timing_mode)
        {
            *capture_rate = s_rate;
        }
        else
        {
            *capture_rate = client_rate;
        }
    }
    else if(DIGI_SERDES_ACB_MODE_C == acb_timing_mode ||
            DIGI_SERDES_ACB_MODE_LOOP_TIMING == acb_timing_mode )
    {
        switch(serdes_mode)
        {
        case DIGI_SERDES_PORT_MODE_ANY_LOWPWR:
            break;
        case DIGI_SERDES_PORT_MODE_S16_100_GE:
            *capture_rate = UTIL_GLOBAL_ENET_100GE_RATE;
            tx_rate       = UTIL_GLOBAL_ENET_100GE_RATE;
            *enet_mapping = TRUE;
            break;
        case DIGI_SERDES_PORT_MODE_S16_40_GE:
            *capture_rate = UTIL_GLOBAL_ENET_40GE_XCD_RATE;
            tx_rate       = UTIL_GLOBAL_ENET_40GE_RATE;
            *enet_mapping = TRUE;
            break;
        case DIGI_SERDES_PORT_MODE_S16_10_GE:
            *capture_rate = UTIL_GLOBAL_ENET_10GE_RATE;
            tx_rate       = UTIL_GLOBAL_ENET_10GE_RATE;
            *enet_mapping = TRUE;
            break;
        case DIGI_SERDES_PORT_MODE_S16_FC_1200:
            *capture_rate = UTIL_GLOBAL_FC_1200_RATE;
            tx_rate       = UTIL_GLOBAL_FC_1200_RATE;
            break;
        case DIGI_SERDES_PORT_MODE_S16_FC_800:
            *capture_rate = UTIL_GLOBAL_FC_800_RATE;
            tx_rate       = UTIL_GLOBAL_FC_800_RATE;
            break;
        case DIGI_SERDES_PORT_MODE_S16_10G_GDPS:
            *capture_rate = UTIL_GLOBAL_GDPS_10G_RATE;
            tx_rate       = UTIL_GLOBAL_GDPS_10G_RATE;
            break;
        case DIGI_SERDES_PORT_MODE_S16_5G_GDPS:
            *capture_rate = UTIL_GLOBAL_GDPS_5G_RATE;
            tx_rate       = UTIL_GLOBAL_GDPS_5G_RATE;
            break;
        case DIGI_SERDES_PORT_MODE_S16_CPRI_10G:
            *capture_rate = UTIL_GLOBAL_CPRI_7_RATE;
            tx_rate       = UTIL_GLOBAL_CPRI_7_RATE;
            break;
        case DIGI_SERDES_PORT_MODE_S16_CPRI_6G:
            *capture_rate = UTIL_GLOBAL_CPRI_6_RATE;
            tx_rate       = UTIL_GLOBAL_CPRI_6_RATE;
            break;
        case DIGI_SERDES_PORT_MODE_S16_CPRI_5G:
            *capture_rate = UTIL_GLOBAL_CPRI_5_RATE;
            tx_rate       = UTIL_GLOBAL_CPRI_5_RATE;
            break;
        case DIGI_SERDES_PORT_MODE_S16_OC_768:
            *capture_rate = UTIL_GLOBAL_STS_768_STM_256_RATE;
            tx_rate       = UTIL_GLOBAL_STS_768_STM_256_RATE;
            break;
        case DIGI_SERDES_PORT_MODE_S16_OC_192:
            *capture_rate = UTIL_GLOBAL_STS_192_STM_64_RATE;
            tx_rate       = UTIL_GLOBAL_STS_192_STM_64_RATE;
            break;
        case DIGI_SERDES_PORT_MODE_S16_OTU4:
            *capture_rate = UTIL_GLOBAL_ODU4_RATE;
            tx_rate       = UTIL_GLOBAL_OTU4_RATE;
            break;
        case DIGI_SERDES_PORT_MODE_S16_OTU3:
            *capture_rate = UTIL_GLOBAL_ODU3_RATE;
            tx_rate       = UTIL_GLOBAL_OTU3_RATE;
            break;     
        case DIGI_SERDES_PORT_MODE_S16_OTU3E2:
            *capture_rate = UTIL_GLOBAL_ODU3E2_RATE;
            tx_rate       = UTIL_GLOBAL_OTU3E2_RATE;
            break;
        case DIGI_SERDES_PORT_MODE_S16_OTU3E1:
            *capture_rate = UTIL_GLOBAL_ODU3E1_RATE;
            tx_rate       = UTIL_GLOBAL_OTU3E1_RATE;
            break;
        case DIGI_SERDES_PORT_MODE_S16_OTU2:
            *capture_rate = UTIL_GLOBAL_ODU2_RATE;
            tx_rate       = UTIL_GLOBAL_OTU2_RATE;
            break;
        case DIGI_SERDES_PORT_MODE_S16_OTU2E:
            *capture_rate = UTIL_GLOBAL_ODU2E_RATE;
            tx_rate       = UTIL_GLOBAL_OTU2E_RATE;
            break;
        case DIGI_SERDES_PORT_MODE_S16_OTU1E:
            *capture_rate = UTIL_GLOBAL_ODU1E_RATE;
            tx_rate       = UTIL_GLOBAL_OTU1E_RATE;
            break;
        case DIGI_SERDES_PORT_MODE_S16_OTU2F:
            *capture_rate = UTIL_GLOBAL_ODU2F_RATE;
            tx_rate       = UTIL_GLOBAL_OTU2F_RATE;
            break;
        case DIGI_SERDES_PORT_MODE_S16_OTU1F:
            *capture_rate = UTIL_GLOBAL_ODU1F_RATE;
            tx_rate       = UTIL_GLOBAL_OTU1F_RATE;
            break;
        case DIGI_SERDES_PORT_MODE_D8_OC_768:
            *capture_rate = UTIL_GLOBAL_STS_768_STM_256_RATE;
            tx_rate       = UTIL_GLOBAL_STS_768_STM_256_RATE;
            break;
        case DIGI_SERDES_PORT_MODE_D8_OTU3:
            *capture_rate = UTIL_GLOBAL_ODU3_RATE;
            tx_rate       = UTIL_GLOBAL_OTU3_RATE;
            break;
        case DIGI_SERDES_PORT_MODE_D8_OTU3E2:
            *capture_rate = UTIL_GLOBAL_ODU3E2_RATE;
            tx_rate       = UTIL_GLOBAL_OTU3E2_RATE;
            break;
        case DIGI_SERDES_PORT_MODE_D8_OTU3E1:
            *capture_rate = UTIL_GLOBAL_ODU3E1_RATE;
            tx_rate       = UTIL_GLOBAL_OTU3E1_RATE;
            break; 
        case DIGI_SERDES_PORT_MODE_D8_GE:
        case DIGI_SERDES_PORT_MODE_T8_3G125:
        case DIGI_SERDES_PORT_MODE_T8_6G25:
        case DIGI_SERDES_PORT_MODE_C8_3G125:
        case DIGI_SERDES_PORT_MODE_C8_6G25:
        case DIGI_SERDES_PORT_MODE_S16_3G125:
        case DIGI_SERDES_PORT_MODE_S16_6G25:
        case DIGI_SERDES_PORT_MODE_S16_10G3125:
        case DIGI_SERDES_PORT_MODE_S16_11G5:
        case DIGI_SERDES_PORT_MODE_S16_12G5:
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
        }
    }
    else
    {
        result = DIGI_ERR_INVALID_ARG;
    }

    *transmit_rate = tx_rate;
    *server_rate = s_rate;
    
    PMC_LOG_TRACE("capture_rate:%f, transmit_rate:%f, server_rate:%f\n", *capture_rate, *transmit_rate, *server_rate);
    
    
    PMC_RETURN(result);
} /* digi_acb_capture_transmit_rate_calc */


/*******************************************************************************
* digi_s16_clk_acb_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is an internal helper function that applies ACB configuration to a S16
*   serdes configured as a clock. ACB of a S16 as a clock is always configured 
*   as a slave in nominal mode. 
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   intf               - Interface configuration related to, valid
*                        values:\n
*                        DIGI_SERDES_LINE_INTF\n
*                        DIGI_SERDES_SYSTEM_INTF\n
*   port_uid           - the serdes port associated to the S16 clock
*   master_lane        - the master lane of the serdes port
*   serdes_cfg_action  - specify the action to perform
*   *acb_ft_prov_cfg_ptr - Pointer to an acb_ft provisionning cfg
*   cfg_clk_as_master    - when TRUE, serdes lane used as a clock is configured as a master lane
*                        - when FALSE, serdes lane used as a clock is configured as a slave lane
*                                      lane defined as master in serdes port definition
*                                      is used as master lane
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_s16_clk_acb_cfg(digi_handle_t            *digi_handle, 
                                      digi_serdes_intf_t        intf, 
                                      UINT32                    port_uid, 
                                      UINT32                    master_lane, 
                                      digi_serdes_cfg_action_t  serdes_cfg_action,
                                      acb_ft_prov_cfg_t        *acb_ft_prov_cfg_ptr,
                                      BOOL8                     cfg_clk_as_master)
{
    PMC_ERROR rc = PMC_SUCCESS;   
    digi_serdes_port_def_t *port_def_ptr;
    s16_handle_t *s16_handle;
    s16_clk_cfg_t clk_cfg;

    PMC_ENTRY();
    PMC_ASSERT(intf != DIGI_SERDES_SFI_51_INTF, DIGI_ERR_INVALID_ARG, 0, 0);

    /* get serdes port definition */    
    port_def_ptr = digi_port_ctxt_find(digi_handle,port_uid,intf); 
    PMC_ASSERT(port_def_ptr != NULL,DIGI_ERR_NULL_HANDLE,0,0); 

    if (DIGI_SERDES_LINE_INTF == intf)
    {
        s16_handle = digi_handle->s16_line_handle;
    } else 
    {
        s16_handle = digi_handle->s16_sys_handle;
    }
    if (DIGI_SERDES_PIN_NOT_USED != port_def_ptr->s16_clk_cfg.pin_index) 
    {

        clk_cfg = FALSE == cfg_clk_as_master ? S16_CLK_CFG_AS_SLAVE : S16_CLK_CFG_AS_MASTER;

        switch (serdes_cfg_action)
        {
        case DIGI_SERDES_ENET_OTN_MAX_PPM_SET:
            s16_max_ppm_set(digi_handle->s16_line_handle, port_def_ptr->s16_clk_cfg.pin_index, DIGI_SERDES_MAX_PPM_COMPUTE(DIGI_SERDES_ENET_MAX_PPM, DIGI_SERDES_OTN_MAX_PPM));
            break;
        case DIGI_SERDES_PROV:
            rc = s16_prov(s16_handle, port_def_ptr->s16_clk_cfg.pin_index, master_lane, acb_ft_prov_cfg_ptr, clk_cfg); 
            break;
        case DIGI_SERDES_PROV_WITH_CLK_AS_MASTER:            
            acb_ft_prov_cfg_ptr->serdes_if = ACB_FT_SERDES_20b;
            rc = s16_prov(s16_handle, port_def_ptr->s16_clk_cfg.pin_index, master_lane, acb_ft_prov_cfg_ptr, clk_cfg); 
            break;
            
        case DIGI_SERDES_PROV_ALIGNER_ONLY:  
        {
            acb_ft_prov_cfg_ptr->serdes_if = ACB_FT_SERDES_20b;
            rc = s16_prov(s16_handle, port_def_ptr->s16_clk_cfg.pin_index,master_lane,acb_ft_prov_cfg_ptr,clk_cfg);
            if (PMC_SUCCESS == rc)
            {
                rc = s16_activate(s16_handle,
                                  port_def_ptr->s16_clk_cfg.pin_index,
                                  TRUE);
            }
            break;
        }
        case DIGI_SERDES_DEPROV:  
        {
            rc = s16_deprov(s16_handle, port_def_ptr->s16_clk_cfg.pin_index);
            break;
        }
        case DIGI_SERDES_ACTIVATE: 
        {
            rc = s16_activate(s16_handle,
                              port_def_ptr->s16_clk_cfg.pin_index,
                              TRUE);
            break;
        }
        case DIGI_SERDES_DEACTIVATE:
        {
            rc = s16_deactivate(s16_handle, port_def_ptr->s16_clk_cfg.pin_index);
            break;
        }
        default:
        {
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
        }
        }    
    }         
    
    PMC_RETURN(rc);
} /* digi_s16_clk_acb_cfg */

/*******************************************************************************
* digi_serdes_non_otn_acb_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is an internal helper function to configure ACB settings for CPB 
*   related applications.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   port             - Serdes instance
*   master_port      - Master Serdes instance in a multi-lane configuration 
*   intf             - Interface configuration related to, valid
*                      values:\n
*                      DIGI_SERDES_LINE_INTF\n
*                      DIGI_SERDES_SYSTEM_INTF\n
*                      DIGI_SERDES_SFI_51_INTF
*   *input_pc_ptr    - util_global_switch_data_t pointer defining inbound port
*   *output_pc_ptr   - util_global_switch_data_t pointer defining outbound port
*   acb_timing_mode  - ACB mode of operation.
*   num_10g_data_lane  - Number of 10G data lane 
*   force_enet_timing_mode - when this config is set, ENET timing mode is forced 
*                            to the specified
*
* OUTPUTS:
*  *acb_ft_prov_cfg_ptr - ACB provision configuration
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_non_otn_acb_cfg(digi_handle_t *digi_handle,
                                             UINT32 port,
                                             UINT32 master_port,
                                             digi_serdes_intf_t intf,
                                             util_global_switch_data_t *input_pc_ptr, 
                                             util_global_switch_data_t *output_pc_ptr,
                                             digi_serdes_acb_mode_t acb_timing_mode,
                                             UINT32 num_10g_data_lane,
                                             digi_enet_tx_timing_mode_t force_enet_timing_mode,
                                             acb_ft_prov_cfg_t *acb_ft_prov_cfg_ptr)
{
    BOOL8 enet_mapping = FALSE;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    if(result == PMC_SUCCESS)
    {
        result = digi_serdes_non_otn_acb_cfg_get(digi_handle,
                                                 intf,
                                                 input_pc_ptr, 
                                                 output_pc_ptr,
                                                 acb_timing_mode,
                                                 num_10g_data_lane,
                                                 force_enet_timing_mode,
                                                 acb_ft_prov_cfg_ptr,
                                                 &enet_mapping);
    }


    if(PMC_SUCCESS == result)
    {
        switch (intf)
        {
        case DIGI_SERDES_LINE_INTF:
            acb_ft_prov_cfg_ptr->serdes_if     = (enet_mapping == TRUE)?ACB_FT_SERDES_20b:ACB_FT_SERDES_32b;
            result = s16_prov(digi_handle->s16_line_handle, port, master_port, acb_ft_prov_cfg_ptr,S16_CLK_CFG_NOT_CLK_LANE);
            break;
        case DIGI_SERDES_SYSTEM_INTF:
            acb_ft_prov_cfg_ptr->serdes_if     = (enet_mapping == TRUE)?ACB_FT_SERDES_20b:ACB_FT_SERDES_32b;
            result = s16_prov(digi_handle->s16_sys_handle, port, master_port, acb_ft_prov_cfg_ptr,S16_CLK_CFG_NOT_CLK_LANE);
            break;
        case DIGI_SERDES_SFI_51_INTF:
            acb_ft_prov_cfg_ptr->num_lane      = 16;
            acb_ft_prov_cfg_ptr->serdes_if     = ACB_FT_SERDES_8b;            
            result = sfi51_d8_tx_18x_prov(digi_handle->sfi51_d8_tx_18x_handle[port], acb_ft_prov_cfg_ptr);   
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
        }
    }
    PMC_RETURN(result);
    
} /* digi_serdes_non_otn_acb_cfg */

/*******************************************************************************
* digi_serdes_otn_acb_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is an internal helper function to configure ACB settings for COREOTN 
*   related applications.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   port             - Serdes instance
*   master_port      - Master Serdes instance in a multi-lane configuration 
*   intf             - Interface configuration related to, valid
*                      values:\n
*                      DIGI_SERDES_LINE_INTF\n
*                      DIGI_SERDES_SYSTEM_INTF\n
*                      DIGI_SERDES_SFI_51_INTF
*   serdes_mode      - Configuration mode for serdes port
*   mpmo_instance    - MPMO instance in COREOTN. See coreotn_mpmo_inst_t
*   chnl             - Channel ID valid range 0-96
*   num_10g_data_lane  - Number of 10G data lane 
*   prov               - when TRUE, serdes is provisionned
*                        when FALSE, serdes is not provisioned but acb_ft 
*                                    prov. configuration is returned
*
* OUTPUTS:
*  *acb_ft_prov_cfg_ptr - ACB provision configuration
*
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_otn_acb_cfg(digi_handle_t *digi_handle,
                                         UINT32 port,
                                         UINT32 master_port,
                                         digi_serdes_intf_t intf,
                                         digi_serdes_port_mode_t serdes_mode,
                                         coreotn_mpmo_inst_t mpmo_instance,
                                         UINT32 chnl,
                                         UINT32 num_10g_data_lane,
                                         acb_ft_prov_cfg_t *acb_ft_prov_cfg_ptr,
                                         BOOL8  prov)
{

    BOOL8 enet_mapping = FALSE; 
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_LOG_TRACE("coreotn_mpmo_inst:%d\n", mpmo_instance);
    

    if(result == PMC_SUCCESS)
    {
        result = digi_serdes_otn_acb_cfg_get(digi_handle,
                                             serdes_mode,
                                             mpmo_instance,
                                             chnl,
                                             num_10g_data_lane,
                                             acb_ft_prov_cfg_ptr,
                                             &enet_mapping);
    }

    if(PMC_SUCCESS == result)
    {
        switch (intf)
        {
        case DIGI_SERDES_LINE_INTF:
            acb_ft_prov_cfg_ptr->serdes_if     = (enet_mapping == TRUE)? ACB_FT_SERDES_20b : ACB_FT_SERDES_32b;
            if (TRUE == prov)
            {
                result = s16_prov(digi_handle->s16_line_handle, port, master_port, acb_ft_prov_cfg_ptr,S16_CLK_CFG_NOT_CLK_LANE);
            }
            break;
        case DIGI_SERDES_SYSTEM_INTF:
            acb_ft_prov_cfg_ptr->serdes_if     = (enet_mapping == TRUE)? ACB_FT_SERDES_20b : ACB_FT_SERDES_32b;
            if (TRUE == prov)
            {
                result = s16_prov(digi_handle->s16_sys_handle, port, master_port, acb_ft_prov_cfg_ptr, S16_CLK_CFG_NOT_CLK_LANE);
            }
            break;
        case DIGI_SERDES_SFI_51_INTF:
            acb_ft_prov_cfg_ptr->num_lane      = 16;
            acb_ft_prov_cfg_ptr->serdes_if     = ACB_FT_SERDES_8b;
            if (TRUE == prov)
            {
                result = sfi51_d8_tx_18x_prov(digi_handle->sfi51_d8_tx_18x_handle[port], acb_ft_prov_cfg_ptr);     
            }
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
        }
    }
    PMC_RETURN(result);
    
} /* digi_serdes_otn_acb_cfg */



/*******************************************************************************
* digi_serdes_otn_acb_reprov_pkt_size_configs
* ______________________________________________________________________________
*
* DESCRIPTION:
*   API to reprovision ACB configs in the event of an OPSA packet size change.
*   This will only be used for UNCHANNELIZED ODUs connecting from LINE or
*   SYS to the ODUKSW.   
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   port             - Serdes instance
*   master_port      - Master Serdes instance in a multi-lane configuration 
*   intf             - Interface configuration related to, valid
*                      values:\n
*                      DIGI_SERDES_LINE_INTF\n
*                      DIGI_SERDES_SYSTEM_INTF\n
*                      DIGI_SERDES_SFI_51_INTF
*   serdes_mode      - Configuration mode for serdes port
*   mpmo_instance    - MPMO instance in COREOTN. See coreotn_mpmo_inst_t
*   chnl             - Channel ID valid range 0-96
*   num_10g_data_lane  - Number of 10G data lane 
*
* OUTPUTS:
*  *acb_ft_prov_cfg_ptr - ACB provision configuration
*
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_otn_acb_reprov_pkt_size_configs(digi_handle_t *digi_handle,
                                                             UINT32 port,
                                                             UINT32 master_port,
                                                             digi_serdes_intf_t intf,
                                                             digi_serdes_port_mode_t serdes_mode,
                                                             coreotn_mpmo_inst_t mpmo_instance,
                                                             UINT32 chnl,
                                                             UINT32 num_10g_data_lane,
                                                             acb_ft_prov_cfg_t *acb_ft_prov_cfg_ptr)
{
    BOOL8 enet_mapping = FALSE; 
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 csu_r;
    UINT64 csu_n;

    PMC_ENTRY();
    
    PMC_LOG_TRACE("coreotn_mpmo_inst:%d\n", mpmo_instance);
   
    if(result == PMC_SUCCESS)
    {
        result = digi_serdes_otn_acb_cfg_get(digi_handle,
                                             serdes_mode,
                                             mpmo_instance,
                                             chnl,
                                             num_10g_data_lane,
                                             acb_ft_prov_cfg_ptr,
                                             &enet_mapping);
    }

    if(PMC_SUCCESS == result)
    {

        switch (intf)
        {
        case DIGI_SERDES_LINE_INTF:
            acb_ft_prov_cfg_ptr->serdes_if     = (enet_mapping == TRUE)? ACB_FT_SERDES_20b : ACB_FT_SERDES_32b;

            s16_csu_div_val_get(digi_handle->s16_line_handle,port,&acb_ft_prov_cfg_ptr->csu_div, &csu_n, &acb_ft_prov_cfg_ptr->csu_denomin, (UINT32 *)&csu_r);

            result = acb_ft_reprov_pkt_size_configs(digi_handle->s16_line_handle->acb_ft_handle[port], acb_ft_prov_cfg_ptr);
            
            break;
        case DIGI_SERDES_SYSTEM_INTF:
            acb_ft_prov_cfg_ptr->serdes_if     = (enet_mapping == TRUE)? ACB_FT_SERDES_20b : ACB_FT_SERDES_32b;

            s16_csu_div_val_get(digi_handle->s16_sys_handle,port,&acb_ft_prov_cfg_ptr->csu_div, &csu_n, &acb_ft_prov_cfg_ptr->csu_denomin, (UINT32 *)&csu_r);

            result = acb_ft_reprov_pkt_size_configs(digi_handle->s16_sys_handle->acb_ft_handle[port], acb_ft_prov_cfg_ptr);
            break;
        case DIGI_SERDES_SFI_51_INTF:
            acb_ft_prov_cfg_ptr->num_lane      = 16;
            acb_ft_prov_cfg_ptr->serdes_if     = ACB_FT_SERDES_8b;

            /* extract required ACB_FT configuration field from CSU */
            sfi51_d8_tx_18x_csu_div_val_get(digi_handle->sfi51_d8_tx_18x_handle[port],&acb_ft_prov_cfg_ptr->csu_div, &csu_n, &acb_ft_prov_cfg_ptr->csu_denomin, (UINT32 *) &csu_r);
            /* provision ACB_FT block */
            result = acb_ft_reprov_pkt_size_configs(digi_handle->sfi51_d8_tx_18x_handle[port]->acb_ft_handle, acb_ft_prov_cfg_ptr);

            if (PMC_SUCCESS == result) 
            {       
                acb_ft_pll_s_max_cfg(digi_handle->sfi51_d8_tx_18x_handle[port]->acb_ft_handle,
                                     acb_ft_prov_cfg_ptr->max_ppm,
                                     csu_n,
                                     acb_ft_prov_cfg_ptr->csu_denomin,
                                     csu_r);
            }
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
        }
    }
    PMC_RETURN(result);
    
} /* digi_serdes_otn_acb_reprov_pkt_size_configs */

/*******************************************************************************
* digi_serdes_non_otn_acb_reprov_pkt_size_configs
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is an internal helper function to reconfigure ACB settings for CPB 
*   related applications when there is a packet size change at the ODUKSW.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   port             - Serdes instance
*   master_port      - Master Serdes instance in a multi-lane configuration 
*   intf             - Interface configuration related to, valid
*                      values:\n
*                      DIGI_SERDES_LINE_INTF\n
*                      DIGI_SERDES_SYSTEM_INTF\n
*                      DIGI_SERDES_SFI_51_INTF
*   *input_pc_ptr    - util_global_switch_data_t pointer defining inbound port
*   *output_pc_ptr   - util_global_switch_data_t pointer defining outbound port
*   acb_timing_mode  - ACB mode of operation.
*   num_10g_data_lane  - Number of 10G data lane 
*   force_enet_timing_mode - when this config is set, ENET timing mode is forced 
*                            to the specified
*
* OUTPUTS:
*  *acb_ft_prov_cfg_ptr - ACB provision configuration
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_non_otn_acb_reprov_pkt_size_configs(digi_handle_t *digi_handle,
                                                                 UINT32 port,
                                                                 UINT32 master_port,
                                                                 digi_serdes_intf_t intf,
                                                                 util_global_switch_data_t *input_pc_ptr, 
                                                                 util_global_switch_data_t *output_pc_ptr,
                                                                 digi_serdes_acb_mode_t acb_timing_mode,
                                                                 UINT32 num_10g_data_lane,
                                                                 digi_enet_tx_timing_mode_t force_enet_timing_mode,
                                                                 acb_ft_prov_cfg_t *acb_ft_prov_cfg_ptr)
{
    BOOL8 enet_mapping = FALSE;
    util_global_switch_data_def_t *input_pc_data_ptr = NULL;
    util_global_switch_data_def_t *output_pc_data_ptr = NULL;
    util_global_cpb_port_data_t *local_input_pc_ptr =  (util_global_cpb_port_data_t *) input_pc_ptr;
    util_global_cpb_port_data_t *local_output_pc_ptr = (util_global_cpb_port_data_t *) output_pc_ptr;
    UINT32 csu_r;
    UINT64 csu_n;

    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    acb_ft_prov_cfg_ptr->mode             = ACB_FT_MODE_NOMINAL;
    input_pc_data_ptr = (util_global_switch_data_def_t *)input_pc_ptr;
    output_pc_data_ptr = (util_global_switch_data_def_t *)output_pc_ptr;
    
    local_input_pc_ptr = &(input_pc_data_ptr->cpb_data);
    local_output_pc_ptr = &(output_pc_data_ptr->cpb_data);
    
    PMC_LOG_TRACE("\n local_output_pc_ptr->port_type:%d\
               \n local_input_pc_ptr->port_type:%d\
               \n local_output_pc_ptr->channel:%d\
               \n local_input_pc_ptr->channel:%d\
               \n acb_timing_mode:%d\n",
                  local_output_pc_ptr->port_type, local_input_pc_ptr->port_type,local_output_pc_ptr->channel, 
                  local_input_pc_ptr->channel, acb_timing_mode);
   

    if(result == PMC_SUCCESS)
    {
        result = digi_serdes_non_otn_acb_cfg_get(digi_handle,
                                                 intf,
                                                 input_pc_ptr, 
                                                 output_pc_ptr,
                                                 acb_timing_mode,
                                                 num_10g_data_lane,
                                                 force_enet_timing_mode,
                                                 acb_ft_prov_cfg_ptr,
                                                 &enet_mapping);
    }


 
    if(PMC_SUCCESS == result)
    {
        switch (intf)
        {
        case DIGI_SERDES_LINE_INTF:
            acb_ft_prov_cfg_ptr->serdes_if     = (enet_mapping == TRUE)? ACB_FT_SERDES_20b : ACB_FT_SERDES_32b;

            s16_csu_div_val_get(digi_handle->s16_line_handle,port,&acb_ft_prov_cfg_ptr->csu_div, &csu_n, &acb_ft_prov_cfg_ptr->csu_denomin, (UINT32 *)&csu_r);

            result = acb_ft_reprov_pkt_size_configs(digi_handle->s16_line_handle->acb_ft_handle[port], acb_ft_prov_cfg_ptr);
            
            break;
        case DIGI_SERDES_SYSTEM_INTF:
            acb_ft_prov_cfg_ptr->serdes_if     = (enet_mapping == TRUE)? ACB_FT_SERDES_20b : ACB_FT_SERDES_32b;

            s16_csu_div_val_get(digi_handle->s16_sys_handle,port,&acb_ft_prov_cfg_ptr->csu_div, &csu_n, &acb_ft_prov_cfg_ptr->csu_denomin, (UINT32 *)&csu_r);

            result = acb_ft_reprov_pkt_size_configs(digi_handle->s16_sys_handle->acb_ft_handle[port], acb_ft_prov_cfg_ptr);
            break;
        case DIGI_SERDES_SFI_51_INTF:
            acb_ft_prov_cfg_ptr->num_lane      = 16;
            acb_ft_prov_cfg_ptr->serdes_if     = ACB_FT_SERDES_8b;

            /* extract required ACB_FT configuration field from CSU */
            sfi51_d8_tx_18x_csu_div_val_get(digi_handle->sfi51_d8_tx_18x_handle[port],&acb_ft_prov_cfg_ptr->csu_div, &csu_n, &acb_ft_prov_cfg_ptr->csu_denomin, (UINT32 *) &csu_r);
            /* provision ACB_FT block */
            result = acb_ft_reprov_pkt_size_configs(digi_handle->sfi51_d8_tx_18x_handle[port]->acb_ft_handle, acb_ft_prov_cfg_ptr);

            if (PMC_SUCCESS == result) 
            {       
                acb_ft_pll_s_max_cfg(digi_handle->sfi51_d8_tx_18x_handle[port]->acb_ft_handle,
                                     acb_ft_prov_cfg_ptr->max_ppm,
                                     csu_n,
                                     acb_ft_prov_cfg_ptr->csu_denomin,
                                     csu_r);
            }
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
        }
    }
    PMC_RETURN(result);
        
} /* digi_serdes_non_otn_acb_reprov_pkt_size_configs */

/*******************************************************************************
* digi_serdes_all_shutdown
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Shutdown the SERDES only. Errors here are only logged to file as we
*   wish to try and proceed to shutdown as much as we can.
*
* INPUTS:
*   *digi_handle   - pointer to DIGI handle instance
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
*   None
*
*******************************************************************************/
PUBLIC void digi_serdes_all_shutdown(digi_handle_t *digi_handle)
{
    PMC_ENTRY();

    digi_serdes_s16_all_shutdown(digi_handle);

    if (digi_serdes_c8_cfg(digi_handle, 
                           DIGI_SERDES_MASK_NONE,
                           DIGI_SERDES_PORT_MODE_ANY_LOWPWR) != PMC_SUCCESS) {

        /* Log failure, but continue to try and shutdown the next SERDES */
        PMC_LOG(PMC_LOG_SEV_LOWEST, DIGI_ERR_SERDES_COULD_NOT_SHUTDOWN, 
                DIGI_ALL_SERDES_INTF_C8_BP_SR, 0); 
    }

    if (digi_serdes_t8_cfg(digi_handle, 
                           DIGI_SERDES_MASK_NONE,
                           DIGI_SERDES_MASK_NONE,
                           DIGI_SERDES_PORT_MODE_ANY_LOWPWR,
                           DIGI_SERDES_PORT_MODE_ANY_LOWPWR) != PMC_SUCCESS) {

        /* Log failure, but continue to try and shutdown the next SERDES */
        PMC_LOG(PMC_LOG_SEV_LOWEST, DIGI_ERR_SERDES_COULD_NOT_SHUTDOWN, 
                DIGI_ALL_SERDES_INTF_T8_BP_LR, 0); 
    }

    if (digi_serdes_sfi51_cfg(digi_handle, 
                              DIGI_SERDES_MASK_NONE,
                              DIGI_SERDES_PORT_MODE_ANY_LOWPWR, 
                              DIGI_SERDES_SFI51_MASK_ALL) != PMC_SUCCESS) {

        /* Log failure, but continue to try and shutdown the next SERDES */
        PMC_LOG(PMC_LOG_SEV_LOWEST, DIGI_ERR_SERDES_COULD_NOT_SHUTDOWN, 
                DIGI_ALL_SERDES_INTF_D8_SFI_51, 0); 
    } 

    digi_serdes_d8_all_shutdown(digi_handle);

    PMC_RETURN();

} /* digi_serdes_all_shutdown */

/*******************************************************************************
* digi_serdes_d8_all_shutdown
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Shutdown the d8 SERDES only. Errors here are only logged to file as we
*   wish to try and proceed to shutdown as much as we can.
*
* INPUTS:
*   *digi_handle   - pointer to DIGI handle instance
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
*   None
*
*******************************************************************************/
PUBLIC void digi_serdes_d8_all_shutdown(digi_handle_t *digi_handle)
{
    PMC_ENTRY();

    /* There are 5 of these */
    if (digi_serdes_d8_cfg(digi_handle, 
                           HOSTMSG_D8_SERDES_INTF_OHP_0,
                           TRUE,
                           FALSE) != PMC_SUCCESS) {

        /* Log failure, but continue to try and shutdown the next SERDES */
        PMC_LOG(PMC_LOG_SEV_LOWEST, DIGI_ERR_SERDES_COULD_NOT_SHUTDOWN, 
                DIGI_ALL_SERDES_INTF_D8_RCP, HOSTMSG_D8_SERDES_INTF_OHP_0); 
    }

    if (digi_serdes_d8_cfg(digi_handle, 
                           HOSTMSG_D8_SERDES_INTF_OHP_1,
                           TRUE,
                           FALSE) != PMC_SUCCESS) {

        /* Log failure, but continue to try and shutdown the next SERDES */
        PMC_LOG(PMC_LOG_SEV_LOWEST, DIGI_ERR_SERDES_COULD_NOT_SHUTDOWN, 
                DIGI_ALL_SERDES_INTF_D8_RCP, HOSTMSG_D8_SERDES_INTF_OHP_1); 
    }

    if (digi_serdes_d8_cfg(digi_handle, 
                           HOSTMSG_D8_SERDES_INTF_OHP_2,
                           TRUE,
                           FALSE) != PMC_SUCCESS) {

        /* Log failure, but continue to try and shutdown the next SERDES */
        PMC_LOG(PMC_LOG_SEV_LOWEST, DIGI_ERR_SERDES_COULD_NOT_SHUTDOWN, 
                DIGI_ALL_SERDES_INTF_D8_RCP, HOSTMSG_D8_SERDES_INTF_OHP_2); 
    }

    if (digi_serdes_d8_cfg(digi_handle, 
                           HOSTMSG_D8_SERDES_INTF_ENET_MGMT,
                           TRUE,
                           FALSE) != PMC_SUCCESS) {

        /* Log failure, but continue to try and shutdown the next SERDES */
        PMC_LOG(PMC_LOG_SEV_LOWEST, DIGI_ERR_SERDES_COULD_NOT_SHUTDOWN, 
                DIGI_ALL_SERDES_INTF_D8_EMGMT, HOSTMSG_D8_SERDES_INTF_ENET_MGMT); 
    }

    if (digi_serdes_d8_cfg(digi_handle, 
                           HOSTMSG_D8_SERDES_INTF_RI_RCP,
                           TRUE,
                           FALSE) != PMC_SUCCESS) {

        /* Log failure, but continue to try and shutdown the next SERDES */
        PMC_LOG(PMC_LOG_SEV_LOWEST, DIGI_ERR_SERDES_COULD_NOT_SHUTDOWN, 
                DIGI_ALL_SERDES_INTF_D8_RCP, HOSTMSG_D8_SERDES_INTF_RI_RCP); 
    }

    if (digi_serdes_d8_cfg(digi_handle, 
                           HOSTMSG_D8_SERDES_INTF_DI_RCP,
                           TRUE,
                           FALSE) != PMC_SUCCESS) {

        /* Log failure, but continue to try and shutdown the next SERDES */
        PMC_LOG(PMC_LOG_SEV_LOWEST, DIGI_ERR_SERDES_COULD_NOT_SHUTDOWN, 
                DIGI_ALL_SERDES_INTF_D8_RCP, HOSTMSG_D8_SERDES_INTF_DI_RCP); 
    }

    PMC_RETURN();
}

/*******************************************************************************
* digi_serdes_lifd_xfi_configure
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Verifies the parameters and determines the lanes to be used before calling the
*   lifd_serdes_subsystem_configure function to configure the actual lane in the lifd.
*   Note that it is implicit that this is called only for the LINE INTERFACE.
*
* INPUTS:
*  *digi_handle   - Pointer to the digi handle instance
*  *port_def_ptr  - Serdes port pointer for which LIFD and or SIFD 
*                   muxing to be configured
*  subsystem_type - Enum, defining the subsystem that the serdes
*                   port is to be routed to.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_lifd_xfi_configure(digi_handle_t *digi_handle,
                                                digi_serdes_port_def_t *port_def_ptr,
                                                xifd_dsis_type_t subsystem_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 j;
    UINT8 k;
    UINT8 post_mux_lane;
    UINT32 los_pin = 0xFF;
    UINT32 los_inv = 0xFF;
    UINT32 post_mux_los_mask = 0; 
    UINT32 skipped_pins = 0;
    UINT8 module_lane_rx;
    UINT8 module_lane_tx;
    BOOL single_lane_otn_cbrc = FALSE;
    /* Counts the number of lanes for this port. */
    UINT8 lane_count = 0;;
    UINT16 pin_bit_mask = 0;
    lifd_los_cfg_t los_cfg;
    UINT32 serdes_offset = 0; /* Used for routing from system side */
    digi_xfi_pin_ctxt_t * xfi_line_pins = digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt;
    UINT32 min_post_mux_lane;
    BOOL8 enable_los_pin;

    PMC_ENTRY();

    digi_debug_dsi_print(digi_handle, "lifd_dsi_onfig");

    /* In a CFP application the port can be routed from the system side */
    if(port_def_ptr->intf == DIGI_SERDES_SYSTEM_INTF)
    {
        xfi_line_pins = (digi_xfi_pin_ctxt_t *) digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt;
        serdes_offset = DIGI_SERDES_XFI_LANES_MAX;
    }

    for(j=0;j<DIGI_SERDES_XFI_LANES_MAX;j++) 
    {
        /*
         * If this is the system interface and the port_uid equals the port 
         * in  the port context or we are line and the ports match and 
         * increse the lane count.
         */
        if (xfi_line_pins[j].port_uid == port_def_ptr->port_uid) 
        {
            lane_count++;
        }
    } 
               
    if( (lane_count == 1) &&
        ( (subsystem_type == XIFD_DSIS_OTN) || 
          (subsystem_type == XIFD_DSIS_CBRC) ) )
    {
        single_lane_otn_cbrc = TRUE;
    }
    
    if (rc == PMC_SUCCESS)        
        rc = digi_port_pin_bit_mask_get(digi_handle, port_def_ptr, 
                                        &pin_bit_mask);

    if (rc == PMC_SUCCESS)    
        rc = digi_min_post_mux_lane_find(digi_handle, port_def_ptr,
                                         &min_post_mux_lane);
    
    if (rc == PMC_SUCCESS) 
    {
        for(j = 0; j < DIGI_SERDES_XFI_LANES_MAX; j++) 
        {

            if(pin_bit_mask & (1 << j)) 
            {
                /* For each post mux lane in this port, search the
                 * xfi context for a matching serdes tx and rx
                 * lane. When it's found, the index of the xfi
                 * context maps to that post mux lane */
                post_mux_lane = xfi_line_pins[j].post_mux_dsi_lane;
                module_lane_rx = DIGI_SERDES_PIN_NOT_USED;
                module_lane_tx = DIGI_SERDES_PIN_NOT_USED;

                for(k=0;k<DIGI_SERDES_XFI_LANES_MAX;k++)
                {
                    if(xfi_line_pins[k].module_lane_rx_index == post_mux_lane - min_post_mux_lane && 
                       xfi_line_pins[k].port_uid == port_def_ptr->port_uid)
                    {
                        module_lane_rx = k;
                    } 

                    if(xfi_line_pins[k].module_lane_tx_index == post_mux_lane - min_post_mux_lane  && 
                       xfi_line_pins[k].port_uid == port_def_ptr->port_uid)
                    {
                        module_lane_tx = k;
                    }
                }

                /* Couldn't find a post mux lane for this module
                 * lane index */
                if ( (module_lane_rx == DIGI_SERDES_PIN_NOT_USED) ||
                     (module_lane_tx == DIGI_SERDES_PIN_NOT_USED) )
                {
                    PMC_RETURN(DIGI_ERR_INVALID_ARG);
                }
 
                PMC_LOG_TRACE("\nlifd_serdes_subsystem_configure, tx: %d, rx: %d, subsystem: %d, master: %d\n",module_lane_tx,module_lane_rx,post_mux_lane,port_def_ptr->master_lane);

                los_cfg.pin_los_en = TRUE;
                los_cfg.los_pol_inv = FALSE;
          
                los_cfg.pin_los_inv = xfi_line_pins[j].los_inv;
                     
                rc = lifd_serdes_subsystem_configure(digi_handle->lifd_handle,
                                                     post_mux_lane, 
                                                     module_lane_rx + serdes_offset, 
                                                     module_lane_tx + serdes_offset, 
                                                     subsystem_type,
                                                     single_lane_otn_cbrc,
                                                     port_def_ptr->master_lane + serdes_offset,
                                                     (BOOL)(digi_handle->var.prod_app == DIGI_PROD_APP_100G_3_X_40G_CFP_CARD));
                
                if(port_def_ptr->los_pin == DIGI_SERDES_PORT_DLOS_LOS_SOURCE) /* using DLOS */
                {
                    rc = lifd_dsis_los_cfg(digi_handle->lifd_handle, 
                                           module_lane_rx + serdes_offset, /*DLOS pin is the same as serdes rx lane*/
                                           xfi_line_pins[j].los_inv, 
                                           (1 << post_mux_lane),
                                           TRUE,
                                           &los_cfg,
                                           TRUE);
                }

                PMC_LOG_TRACE("\n%s\n",pmc_log_string_get(rc));

                if (rc != PMC_SUCCESS)
                {
                    PMC_RETURN(rc);  
                }

                los_pin = xfi_line_pins[j].los_pin + serdes_offset;
                los_inv = xfi_line_pins[j].los_inv;
                post_mux_los_mask |= (1 << post_mux_lane);

            }
            else
            {
                skipped_pins++;
            }
            PMC_ATOMIC_YIELD(digi_handle,0);
        }

        if (port_def_ptr->los_pin == DIGI_SERDES_PORT_NO_LOS_SOURCE) {

            enable_los_pin = FALSE;

        } else if (port_def_ptr->los_pin != DIGI_SERDES_PORT_DLOS_LOS_SOURCE) {

            enable_los_pin = TRUE;
        }

        /* Apply LOS settings if not in DLOS modes*/                 
        if(port_def_ptr->los_pin != DIGI_SERDES_PORT_DLOS_LOS_SOURCE) /* using DLOS */
        {
            rc = lifd_dsis_los_cfg(digi_handle->lifd_handle,
                                   los_pin,
                                   los_inv,
                                   post_mux_los_mask,
                                   enable_los_pin,
                                   &los_cfg,
                                   FALSE);
        }
    }

    PMC_RETURN(rc);
} /* digi_serdes_lifd_xfi_configure */


/*******************************************************************************
* digi_serdes_lifd_xfi_align_tx_rx_configure
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Verifies the parameters and determines the lanes to be used before calling the
*   lifd_serdes_subsystem_configure function to configure the actual lane in the lifd.
*   API configures LIFD such as RX and TX lanes are symetric/aligned
*   (TX lanes are used to configure RX lanes).
*   Note that it is implicit that this is called only for the LINE INTERFACE.
*
* INPUTS:
*  *digi_handle   - Pointer to the digi handle instance
*  *port_def_ptr  - Serdes port pointer for which LIFD and or SIFD 
*                   muxing to be configured
*  subsystem_type - Enum, defining the subsystem that the serdes
*                   port is to be routed to.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_lifd_xfi_align_tx_rx_configure(digi_handle_t *digi_handle,
                                                            digi_serdes_port_def_t *port_def_ptr,
                                                            xifd_dsis_type_t subsystem_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 j;
    UINT8 k;
    UINT8 post_mux_lane;
    UINT32 los_pin = 0xFF;
    UINT32 los_inv = 0xFF;
    UINT32 post_mux_los_mask = 0; 
    UINT32 skipped_pins = 0;
    UINT8 module_lane_rx;
    UINT8 module_lane_tx;
    BOOL single_lane_otn_cbrc = FALSE;
    /* Counts the number of lanes for this port. */
    UINT8 lane_count = 0;;
    UINT16 pin_bit_mask = 0;
    lifd_los_cfg_t los_cfg;
    UINT32 serdes_offset = 0; /* Used for routing from system side */
    digi_xfi_pin_ctxt_t * xfi_line_pins = digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt;
    UINT32 min_post_mux_lane;
    BOOL8 enable_los_pin;

    PMC_ENTRY();

    digi_debug_dsi_print(digi_handle, "lifd_dsi_onfig");

    /* In a CFP application the port can be routed from the system side */
    if(port_def_ptr->intf == DIGI_SERDES_SYSTEM_INTF)
    {
        xfi_line_pins = (digi_xfi_pin_ctxt_t *) digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt;
        serdes_offset = DIGI_SERDES_XFI_LANES_MAX;
    }

    for(j=0;j<DIGI_SERDES_XFI_LANES_MAX;j++) 
    {
        /*
         * If this is the system interface and the port_uid equals the port 
         * in  the port context or we are line and the ports match and 
         * increse the lane count.
         */
        if (xfi_line_pins[j].port_uid == port_def_ptr->port_uid) 
        {
            lane_count++;
        }
    } 
               
    if( (lane_count == 1) &&
        ( (subsystem_type == XIFD_DSIS_OTN) || 
          (subsystem_type == XIFD_DSIS_CBRC) ) )
    {
        single_lane_otn_cbrc = TRUE;
    }
    
    if (rc == PMC_SUCCESS)        
        rc = digi_port_pin_bit_mask_get(digi_handle, port_def_ptr, 
                                        &pin_bit_mask);

    if (rc == PMC_SUCCESS)    
        rc = digi_min_post_mux_lane_find(digi_handle, port_def_ptr,
                                         &min_post_mux_lane);
    
    if (rc == PMC_SUCCESS) 
    {
        for(j = 0; j < DIGI_SERDES_XFI_LANES_MAX; j++) 
        {

            if(pin_bit_mask & (1 << j)) 
            {
                /* For each post mux lane in this port, search the
                 * xfi context for a matching serdes tx and rx
                 * lane. When it's found, the index of the xfi
                 * context maps to that post mux lane */
                post_mux_lane = xfi_line_pins[j].post_mux_dsi_lane;
                module_lane_rx = DIGI_SERDES_PIN_NOT_USED;
                module_lane_tx = DIGI_SERDES_PIN_NOT_USED;

                for(k=0;k<DIGI_SERDES_XFI_LANES_MAX;k++)
                {
                    if(xfi_line_pins[k].module_lane_tx_index == post_mux_lane - min_post_mux_lane  && 
                       xfi_line_pins[k].port_uid == port_def_ptr->port_uid)
                    {
                        module_lane_tx = k;
                        module_lane_rx = k;
                    }
                }

                /* Couldn't find a post mux lane for this module
                 * lane index */
                if ( (module_lane_rx == DIGI_SERDES_PIN_NOT_USED) ||
                     (module_lane_tx == DIGI_SERDES_PIN_NOT_USED) )
                {
                    PMC_RETURN(DIGI_ERR_INVALID_ARG);
                }
 
                PMC_LOG_TRACE("\nlifd_serdes_subsystem_configure, tx: %d, rx: %d, subsystem: %d, master: %d\n",module_lane_tx,module_lane_rx,post_mux_lane,port_def_ptr->master_lane);

                los_cfg.pin_los_en = TRUE;
                los_cfg.los_pol_inv = FALSE;
          
                los_cfg.pin_los_inv = xfi_line_pins[j].los_inv;
                     
                rc = lifd_serdes_subsystem_configure(digi_handle->lifd_handle,
                                                     post_mux_lane, 
                                                     module_lane_rx + serdes_offset, 
                                                     module_lane_tx + serdes_offset, 
                                                     subsystem_type,
                                                     single_lane_otn_cbrc,
                                                     port_def_ptr->master_lane + serdes_offset,
                                                     (BOOL)(digi_handle->var.prod_app == DIGI_PROD_APP_100G_3_X_40G_CFP_CARD));
                
                if(port_def_ptr->los_pin == DIGI_SERDES_PORT_DLOS_LOS_SOURCE) /* using DLOS */
                {
                    rc = lifd_dsis_los_cfg(digi_handle->lifd_handle, 
                                           module_lane_rx + serdes_offset, /*DLOS pin is the same as serdes rx lane*/
                                           xfi_line_pins[j].los_inv, 
                                           (1 << post_mux_lane),
                                           TRUE,
                                           &los_cfg,
                                           TRUE);
                }

                PMC_LOG_TRACE("\n%s\n",pmc_log_string_get(rc));

                if (rc != PMC_SUCCESS)
                {
                    PMC_RETURN(rc);  
                }

                los_pin = xfi_line_pins[j].los_pin + serdes_offset;
                los_inv = xfi_line_pins[j].los_inv;
                post_mux_los_mask |= (1 << post_mux_lane);

            }
            else
            {
                skipped_pins++;
            }
            PMC_ATOMIC_YIELD(digi_handle,0);
        }

        if (port_def_ptr->los_pin == DIGI_SERDES_PORT_NO_LOS_SOURCE) {

            enable_los_pin = FALSE;

        } else if (port_def_ptr->los_pin != DIGI_SERDES_PORT_DLOS_LOS_SOURCE) {

            enable_los_pin = TRUE;
        }

        /* Apply LOS settings if not in DLOS modes*/                 
        if(port_def_ptr->los_pin != DIGI_SERDES_PORT_DLOS_LOS_SOURCE) /* using DLOS */
        {
            rc = lifd_dsis_los_cfg(digi_handle->lifd_handle,
                                   los_pin,
                                   los_inv,
                                   post_mux_los_mask,
                                   enable_los_pin,
                                   &los_cfg,
                                   FALSE);
        }
    }

    PMC_RETURN(rc);
} /* digi_serdes_lifd_xfi_align_tx_rx_configure */

/*******************************************************************************
* digi_serdes_lifd_sfi51_configure
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Verifies the parameters and determines the lanes to be used before calling the
*   lifd_serdes_subsystem_configure function to configure the actual lane in the lifd.
*   Note that it is implicit that this is called only for the SFI51 LINE INTERFACE.
*
* INPUTS:
*  *digi_handle   - Pointer to the digi handle instance
*  *port_def_ptr  - Serdes port pointer for which LIFD muxing to be configured
*  subsystem_type - Enum, defining the subsystem that the serdes
*                   port is to be routed to.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_lifd_sfi51_configure(digi_handle_t *digi_handle,
                                                  digi_serdes_port_def_t *port_def_ptr,
                                                  xifd_dsis_type_t subsystem_type)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    lifd_sfi5point1_int_t sfi51_intf = LIFD_SFI5POINT1_NONE;
    BOOL is_sfi_one_and_two_swapped = FALSE;
    lifd_rxs_cfg_t rxs_config;
    PMC_ENTRY();

    if (digi_serdes_sfi51_info_get(digi_handle, port_def_ptr->port_uid, 
                                   &sfi51_intf, &is_sfi_one_and_two_swapped, &rxs_config) != PMC_SUCCESS) {

        /* This is a no-op it should assert in the calling function */

    } else {

        rc = lifd_sfi_subsystem_select(digi_handle->lifd_handle, 
                                       sfi51_intf,
                                       is_sfi_one_and_two_swapped,
                                       subsystem_type,
                                       TRUE,
                                       rxs_config);
    }

    PMC_RETURN(rc);
} /* digi_serdes_lifd_sfi51_configure */

/*******************************************************************************
* digi_serdes_lifd_xfi_pin_los_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Configures LIFD MUX XFI port pin LOS.
*
* INPUTS:
*  *digi_handle   - Pointer to the digi handle instance
*  *port_def_ptr  - Serdes port pointer for which LIFD and or SIFD 
*                   muxing to be configured
*  enable         - LIFD MUX PIN LOS new value
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_lifd_xfi_pin_los_cfg(digi_handle_t            *digi_handle,
                                                  digi_serdes_port_def_t   *port_def_ptr,
                                                  BOOL8                     enable)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    UINT8 j;
    UINT8 k;
    UINT8 post_mux_lane;
    UINT32 los_pin = 0xFF;
    UINT32 los_inv = 0xFF;
    UINT32 post_mux_los_mask = 0;
    UINT32 skipped_pins = 0;
    UINT8 module_lane_rx;
    UINT8 module_lane_tx;
    /* BOOL single_lane_otn_cbrc = FALSE; */
    /* Counts the number of lanes for this port. */
    UINT8 lane_count = 0;
    UINT16 pin_bit_mask = 0;
    lifd_los_cfg_t los_cfg;
    digi_xfi_pin_ctxt_t * xfi_line_pins = digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt;
    UINT32 serdes_offset = 0;
    BOOL8 use_dlos_los_source = FALSE;

    PMC_ENTRY();

    if (port_def_ptr->intf == DIGI_SERDES_SYSTEM_INTF && digi_handle->var.prod_app == DIGI_PROD_APP_100G_3_X_40G_CFP_CARD)
    {
        xfi_line_pins = (digi_xfi_pin_ctxt_t *) digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt;
        serdes_offset = 12;
    }

    for(j=0;j<DIGI_SERDES_XFI_LANES_MAX;j++) 
    {
        /*
         * If this is the system interface and the port_uid equals the port 
         * in  the port context or we are line and the ports match and 
         * increse the lane count.
         */
        if (xfi_line_pins[j].port_uid == port_def_ptr->port_uid) 
        {
            lane_count++;
        }
    }               
            
    rc = digi_port_pin_bit_mask_get(digi_handle, port_def_ptr, 
                                    &pin_bit_mask);
    
    if (rc == PMC_SUCCESS) 
    {

        for(j = 0; j < DIGI_SERDES_XFI_LANES_MAX; j++) 
        {

            if(pin_bit_mask & (1 << j)) 
            {
                /* For each post mux lane in this port, search the
                 * xfi context for a matching serdes tx and rx
                 * lane. When it's found, the index of the xfi
                 * context maps to that post mux lane */
                post_mux_lane = xfi_line_pins[j].post_mux_dsi_lane;
                module_lane_rx = DIGI_SERDES_PIN_NOT_USED;
                module_lane_tx = DIGI_SERDES_PIN_NOT_USED;
              
                for(k=0;k<DIGI_SERDES_XFI_LANES_MAX;k++)
                {

                    if ((xfi_line_pins[k].module_lane_rx_index == (j - skipped_pins) ||
                         xfi_line_pins[k].module_lane_rx_index == j) && 
                        xfi_line_pins[k].port_uid == port_def_ptr->port_uid)
                    {
                        module_lane_rx = k;
                    } 

                    if ((xfi_line_pins[k].module_lane_tx_index == (j - skipped_pins) ||
                         xfi_line_pins[k].module_lane_tx_index == j) && 
                        xfi_line_pins[k].port_uid == port_def_ptr->port_uid)
                    {
                        module_lane_tx = k;
                    } 
                } /* end for k */
                
                /* Couldn't find a post mux lane for this module
                 * lane index */
                if ( (module_lane_rx == DIGI_SERDES_PIN_NOT_USED) ||
                     (module_lane_tx == DIGI_SERDES_PIN_NOT_USED) )
                {
                    PMC_RETURN(DIGI_ERR_INVALID_ARG);
                }
            
                PMC_LOG_TRACE("\nlifd_serdes_subsystem_pin_los_cfg, tx: %d, rx: %d, subsystem: %d, master: %d\n",module_lane_tx,module_lane_rx,post_mux_lane,port_def_ptr->master_lane);

                PMC_MEMSET((void *)&los_cfg, 0, sizeof(lifd_los_cfg_t));
                los_inv = xfi_line_pins[j].los_inv;
                post_mux_los_mask |= (1 << post_mux_lane);
                
                if (DIGI_SERDES_PORT_DLOS_LOS_SOURCE != xfi_line_pins[j].los_pin)
                {
                    rc = lifd_serdes_los_cfg_get(digi_handle->lifd_handle,
                                                 xfi_line_pins[j].los_pin + serdes_offset, 
                                                 &los_cfg);
                    
                    los_pin = xfi_line_pins[j].los_pin + serdes_offset;
                    if (PMC_SUCCESS == rc)
                    {
                        los_cfg.pin_los_en = enable;
                    }
                }
                else
                {
                    los_pin = module_lane_rx + serdes_offset;
                    rc = lifd_serdes_los_cfg_get(digi_handle->lifd_handle,
                                                 los_pin, 
                                                 &los_cfg);
                    if (PMC_SUCCESS == rc)
                    {
                        use_dlos_los_source = TRUE;
                        los_cfg.pin_los_en = FALSE;
                        rc = lifd_dsis_los_cfg(digi_handle->lifd_handle, 
                                               los_pin, 
                                               los_inv,
                                               (1 << post_mux_lane),
                                               enable,
                                               &los_cfg,
                                               use_dlos_los_source);
                    }
                }

                PMC_LOG_TRACE("\n%s\n",pmc_log_string_get(rc));

            }
            else
            {
                skipped_pins++;
            }
        }
        /* Apply LOS settings */
        if (TRUE != use_dlos_los_source)
        {
            rc = lifd_dsis_los_cfg(digi_handle->lifd_handle, 
                                   los_pin, 
                                   los_inv,
                                   post_mux_los_mask,
                                   enable,
                                   &los_cfg,
                                   use_dlos_los_source);
        }
    }
    
    PMC_RETURN(rc);
} /* digi_serdes_lifd_xfi_pin_los_cfg */

/*******************************************************************************
* digi_serdes_los_force
* ___________________________________________________________________________
*
* DESCRIPTION:
*  This API provides a method for systems which use DLOS feature for LOS
*  detection and have physical LOS pins at a deterministic continuous setting
*  to force the LOS condition at CBRC, ENET, LINEOTN or SYSOTN subsytems.\n\n
*
*  Note that this API is intended to be used only in this special design case.\n\n
*
*  To force LOS conditon, the user must set dlos_en to FALSE and los_pol_inv to 
*  TRUE.\n\n
* 
*  To remove the forced LOS condition, the user must set dlos_en back to TRUE 
*  and los_pol_inv to FALSE.
*
* INPUTS:
*  *digi_handle   - Pointer to the digi handle instance
*  *port_ptr      - S16_LINE or S16_SYSTEM Serdes port pointer
*  dlos_en        - dlos enable
*  los_pol_inv    - los polarity invert
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_los_force (digi_handle_t *digi_handle,
                                                  digi_serdes_port_t *port_ptr,
                                                  BOOL8 dlos_en, BOOL8 los_pol_inv)
{
    UINT8 j;
    digi_serdes_port_def_t *port_def_ptr = (digi_serdes_port_def_t *)port_ptr;
    digi_xfi_pin_ctxt_t * xfi_line_pins;
    digi_xfi_pin_ctxt_t * xfi_sys_pins;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    /* arguments checking */
    if (NULL == port_def_ptr || !(port_def_ptr->init) || 
        (port_def_ptr->intf != DIGI_SERDES_LINE_INTF && port_def_ptr->intf != DIGI_SERDES_SYSTEM_INTF)) {
        rc = DIGI_ERR_INVALID_ARG;
    }

    /* LIFD */
    if (rc == PMC_SUCCESS && port_def_ptr->intf == DIGI_SERDES_LINE_INTF) {
        xfi_line_pins = digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt;
        for(j = 0; j < DIGI_SERDES_XFI_LANES_MAX; j++) {
            if (xfi_line_pins[j].port_uid == port_def_ptr->port_uid) {
                rc = lifd_serdes_los_force(digi_handle->lifd_handle, j, dlos_en, los_pol_inv);
                if (rc != PMC_SUCCESS) {
                    break;
                }
            }
        }
    }

    /* SIFD */
    if (rc == PMC_SUCCESS && port_def_ptr->intf == DIGI_SERDES_SYSTEM_INTF) {
        xfi_sys_pins = digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt;
        for(j = 0; j < DIGI_SERDES_SYS_LANES_MAX; j++) {
            if (xfi_sys_pins[j].port_uid == port_def_ptr->port_uid) {
                rc = sifd_serdes_los_force(digi_handle->sifd_handle, j, dlos_en, los_pol_inv);
                if (rc != PMC_SUCCESS) {
                    break;
                }
            }
        }
    }

    PMC_RETURN(rc);
} /* digi_serdes_los_force */

/*******************************************************************************
* digi_serdes_los_force_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  This API is called to obtain the configuration set by digi_serdes_los_force.
*
* INPUTS:
*  *digi_handle   - Pointer to the digi handle instance
*  *port_ptr      - S16_LINE or S16_SYSTEM Serdes port pointer
*
* OUTPUTS:
*  *dlos_en       - dlos enable
*  *los_pol_inv   - los polarity invert
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_los_force_get (digi_handle_t *digi_handle,
                                                  digi_serdes_port_t *port_ptr,
                                                  BOOL8 *dlos_en, BOOL8 *los_pol_inv)
{
    UINT8 j;
    digi_serdes_port_def_t *port_def_ptr = (digi_serdes_port_def_t *)port_ptr;
    digi_xfi_pin_ctxt_t * xfi_line_pins;
    digi_xfi_pin_ctxt_t * xfi_sys_pins;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    /* arguments checking */
    if (NULL == port_def_ptr || !(port_def_ptr->init) || 
        NULL == dlos_en || NULL == los_pol_inv ||
        (port_def_ptr->intf != DIGI_SERDES_LINE_INTF && port_def_ptr->intf != DIGI_SERDES_SYSTEM_INTF)) {
        rc = DIGI_ERR_INVALID_ARG;
    }

    /* LIFD */
    if (rc == PMC_SUCCESS && port_def_ptr->intf == DIGI_SERDES_LINE_INTF) {
        xfi_line_pins = digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt;
        for(j = 0; j < DIGI_SERDES_XFI_LANES_MAX; j++) {
            if (xfi_line_pins[j].port_uid == port_def_ptr->port_uid) {
                rc = lifd_serdes_los_force_get(digi_handle->lifd_handle, j, dlos_en, los_pol_inv);
                break;
            }
        }
    }

    /* SIFD */
    if (rc == PMC_SUCCESS && port_def_ptr->intf == DIGI_SERDES_SYSTEM_INTF) {
        xfi_sys_pins = digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt;
        for(j = 0; j < DIGI_SERDES_SYS_LANES_MAX; j++) {
            if (xfi_sys_pins[j].port_uid == port_def_ptr->port_uid) {
                rc = sifd_serdes_los_force_get(digi_handle->sifd_handle, j, dlos_en, los_pol_inv);
                break;
            }
        }
    }

    PMC_RETURN(rc);
} /* digi_serdes_los_force_get */

/*******************************************************************************
* digi_serdes_sifd_pin_los_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Configures SIFD MUX SYSTEM port pin LOS.
*
* INPUTS:
*  *digi_handle   - Pointer to the digi handle instance.
*  *port_def_ptr  - Serdes port pointer for which LIFD and or SIFD 
*                   muxing to be configured
*   enable        - SIFD MUX port pin LOS new value.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_sifd_pin_los_cfg(digi_handle_t            *digi_handle,
                                              digi_serdes_port_def_t   *port_def_ptr,
                                              BOOL8                     enable)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    UINT8 j;
    UINT8 k;
    UINT8 post_mux_lane;
    UINT32 skipped_pins = 0;
    UINT8 module_lane_rx;
    UINT8 module_lane_tx;
    /* Counts the number of lanes for this port. */
    UINT8 lane_count = 0;
    UINT16 pin_bit_mask = 0;
    sifd_los_cfg_t los_cfg;
    UINT32 serdes_lane = 0;
    digi_xfi_pin_ctxt_t * xfi_sys_pins = digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt;

    PMC_ENTRY();

    for(j=0;j<DIGI_SERDES_SYS_LANES_MAX;j++) 
    {
        /*
         * If this is the system interface and the port_uid equals the port 
         * in  the port context or we are line and the ports match and 
         * increse the lane count.
         */
        if (xfi_sys_pins[j].port_uid == port_def_ptr->port_uid) 
        {
            lane_count++;
        }
    }                

    
    rc = digi_port_pin_bit_mask_get(digi_handle, port_def_ptr, 
                                    &pin_bit_mask);
    
    if (rc == PMC_SUCCESS) 
    {
        
        for(j = 0; j < DIGI_SERDES_SYS_LANES_MAX; j++) 
        {
            
            if(pin_bit_mask & (1 << j)) 
            {
                /* For each post mux lane in this port, search the
                 * xfi context for a matching serdes tx and rx
                 * lane. When it's found, the index of the xfi
                 * context maps to that post mux lane 
                 */
                post_mux_lane = xfi_sys_pins[j].post_mux_dsi_lane;
                module_lane_rx = DIGI_SERDES_PIN_NOT_USED;
                module_lane_tx = DIGI_SERDES_PIN_NOT_USED;
                
                for(k = 0; k < DIGI_SERDES_SYS_LANES_MAX; k++)
                {
                    
                    if ((xfi_sys_pins[k].module_lane_rx_index == (j - skipped_pins) ||
                         xfi_sys_pins[k].module_lane_rx_index == j) && 
                        xfi_sys_pins[k].port_uid == port_def_ptr->port_uid)
                    {
                        module_lane_rx = k;
                    } 
                    
                    if ((xfi_sys_pins[k].module_lane_tx_index == (j - skipped_pins)  ||
                         xfi_sys_pins[k].module_lane_tx_index == j) && 
                        xfi_sys_pins[k].port_uid == port_def_ptr->port_uid)
                    {
                        module_lane_tx = k;
                    } 
                } /* end for k */
                
                /* Couldn't find a post mux lane for this module
                 * lane index */
                if ( (module_lane_rx == DIGI_SERDES_PIN_NOT_USED) ||
                     (module_lane_tx == DIGI_SERDES_PIN_NOT_USED) )
                {
                    PMC_RETURN(DIGI_ERR_INVALID_ARG);
                }
                
                PMC_LOG_TRACE("\nsifd_serdes_subsystem_pin_los_cfg, tx: %d, rx: %d, subsystem: %d, master: %d\n",module_lane_tx,module_lane_rx,post_mux_lane,port_def_ptr->master_lane);
                
                if( DIGI_SERDES_PORT_DLOS_LOS_SOURCE == xfi_sys_pins[j].los_pin )
                {
                    serdes_lane = j;
                } 
                else
                {
                    serdes_lane = xfi_sys_pins[j].los_pin;
                }
                
                PMC_MEMSET((void *)&los_cfg, 0, sizeof(sifd_los_cfg_t));
                rc = sifd_serdes_los_cfg_get(digi_handle->sifd_handle,
                                             serdes_lane,
                                             &los_cfg);

                if (PMC_SUCCESS == rc)
                {
                    los_cfg.pin_los_en = enable;

                    if(xfi_sys_pins[j].los_pin == DIGI_SERDES_PORT_DLOS_LOS_SOURCE)
                    {
                        rc = sifd_subsystem_los_configure(digi_handle->sifd_handle,
                                                          j,
                                                          post_mux_lane, 
                                                          &los_cfg,
                                                          TRUE);
                    }
                    else
                    {
                        rc = sifd_subsystem_los_configure(digi_handle->sifd_handle,
                                                          xfi_sys_pins[j].los_pin,
                                                          post_mux_lane, 
                                                          &los_cfg,
                                                          FALSE);
                    }
                }
                PMC_LOG_TRACE("\n%s\n",pmc_log_string_get(rc));
                
                if (rc != PMC_SUCCESS)
                {
                    PMC_RETURN(rc);  
                }
            }
            else
            {
                skipped_pins++;
            }
        }
    }

    PMC_RETURN(rc);
} /* digi_serdes_sifd_pin_los_cfg */

/*******************************************************************************
* digi_serdes_lifd_sfi51_pin_los_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Verifies the parameters and determines the lanes to be used before calling the
*   lifd_serdes_subsystem_pin_los_cfg function to configure the actual lane in the lifd.
*   Note that it is implicit that this is called only for the SFI51 LINE INTERFACE.
*
* INPUTS:
*  *digi_handle   - Pointer to the digi handle instance
*  *port_def_ptr  - Serdes port pointer for which LIFD muxing to be configured
*   enable        - LIFD MUX port pin LOS new value
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_lifd_sfi51_pin_los_cfg(digi_handle_t           *digi_handle,
                                                    digi_serdes_port_def_t   *port_def_ptr,
                                                    BOOL8                     enable)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    PMC_ENTRY();

    PMC_RETURN(rc);
} /* digi_serdes_lifd_sfi51_pin_los_cfg */


/*******************************************************************************
* digi_serdes_sfi51_info_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Verifies the parameters and determines the lanes to be used before calling the
*   lifd_serdes_subsystem_configure function to configure the actual lane in the lifd.
*   Note that it is implicit that this is called only for the SFI51 LINE INTERFACE.
*
* INPUTS:
*  *digi_handle   - Pointer to the digi handle instance
*  port_uid       - The unique identifier for the port.
*
* OUTPUTS:
*  *sfi51_intf    - A pointer to the interface type
*  *is_sfi_one_and_two_swapped - A pointer to a boolean indicating if sfi one 
*                   and two are swapped. TRUE: If they are swapped, 
*                   FALSE otherwise.
*  *rxs_config    - A pointer to the lifd_rxs_cfg_t.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_sfi51_info_get(digi_handle_t *digi_handle,
                                             UINT32 port_uid, 
                                             lifd_sfi5point1_int_t *sfi51_intf,
                                             BOOL *is_sfi_one_and_two_swapped,
                                             lifd_rxs_cfg_t *rxs_config)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    PMC_ENTRY();

    rxs_config->rxs_inv = FALSE;
    rxs_config->rxs_en = TRUE;

    if (digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[DIGI_POST_MUX_BUS_0].port_uid == port_uid) {

        *sfi51_intf = LIFD_SFI5POINT1_ONE;

        if (digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[DIGI_POST_MUX_BUS_0].post_mux_2G5_bus_allocation == DIGI_POST_MUX_BUS_1) {

            *is_sfi_one_and_two_swapped = TRUE;
            
        }

        rc = PMC_SUCCESS;

    } else if (digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[DIGI_POST_MUX_BUS_1].port_uid == port_uid) {

        *sfi51_intf = LIFD_SFI5POINT1_TWO;

        if (digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[DIGI_POST_MUX_BUS_1].post_mux_2G5_bus_allocation == DIGI_POST_MUX_BUS_0) {

            *is_sfi_one_and_two_swapped = TRUE;
            
        }

        rc = PMC_SUCCESS;

    } else {

        /* Shouldn't be in here as we only have two SFI_51_INTF instances */
        PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
    }

    PMC_RETURN(rc);
} /* digi_serdes_sfi51_info_get */

/*******************************************************************************
* digi_serdes_sifd_configure
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Verifies the parameters and determines the lanes to be used before calling the
*   sifd_serdes_subsystem_configure function to configure the actual lane in the sifd.
*   Note that it is implicit that this is called only for the SYSTEM INTERFACE.
*
* INPUTS:
*  *digi_handle   - Pointer to the digi handle instance.
*  *port_def_ptr  - Serdes port pointer for which LIFD and or SIFD 
*                   muxing to be configured
*  subsystem_type - Enum, defining the subsystem that the serdes
*                   port is to be routed to.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_sifd_configure(digi_handle_t *digi_handle,
                                            digi_serdes_port_def_t *port_def_ptr,
                                            xifd_dsis_type_t subsystem_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 j;
    UINT8 k;
    UINT8 post_mux_lane;
    UINT32 skipped_pins = 0;
    UINT8 module_lane_rx;
    UINT8 module_lane_tx;
    BOOL single_lane_otn_cbrc = FALSE;
    /* Counts the number of lanes for this port. */
    UINT8 lane_count = 0;;
    UINT16 pin_bit_mask = 0;
    sifd_los_cfg_t los_cfg;
    digi_xfi_pin_ctxt_t * xfi_sys_pins = digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt;
    UINT32 min_post_mux_lane;

    PMC_ENTRY();

    digi_debug_dsi_print(digi_handle, "sifd_dsi_config");

    for(j=0;j<DIGI_SERDES_SYS_LANES_MAX;j++) 
    {
        /*
         * If this is the system interface and the port_uid equals the port 
         * in  the port context or we are line and the ports match and 
         * increse the lane count.
         */
        if (xfi_sys_pins[j].port_uid == port_def_ptr->port_uid) 
        {
            lane_count++;
        }
    }                

    if( (lane_count == 1) &&
        ( (subsystem_type == XIFD_DSIS_OTN) || 
          (subsystem_type == XIFD_DSIS_CBRC) ) )
    {
        single_lane_otn_cbrc = TRUE;
    }

    if (rc == PMC_SUCCESS)    
        rc = digi_port_pin_bit_mask_get(digi_handle, port_def_ptr, 
                                        &pin_bit_mask);

    if (rc == PMC_SUCCESS)    
        rc = digi_min_post_mux_lane_find(digi_handle, port_def_ptr,
                                         &min_post_mux_lane);

    if (rc == PMC_SUCCESS) 
    {
        
        for(j = 0; j < DIGI_SERDES_XFI_LANES_MAX; j++) 
        {
            
            if(pin_bit_mask & (1 << j)) 
            {
                /* For each post mux lane in this port, search the
                 * xfi context for a matching serdes tx and rx
                 * lane. When it's found, the index of the xfi
                 * context maps to that post mux lane 
                 */
                post_mux_lane = xfi_sys_pins[j].post_mux_dsi_lane;
                module_lane_rx = DIGI_SERDES_PIN_NOT_USED;
                module_lane_tx = DIGI_SERDES_PIN_NOT_USED;

                for(k=0;k<DIGI_SERDES_XFI_LANES_MAX;k++)
                {
                    if(xfi_sys_pins[k].module_lane_rx_index == post_mux_lane - min_post_mux_lane && 
                       xfi_sys_pins[k].port_uid == port_def_ptr->port_uid)
                    {
                        module_lane_rx = k;
                    } 

                    if(xfi_sys_pins[k].module_lane_tx_index == post_mux_lane - min_post_mux_lane  && 
                       xfi_sys_pins[k].port_uid == port_def_ptr->port_uid)
                    {
                        module_lane_tx = k;
                    }
                }

                /* Couldn't find a post mux lane for this module
                 * lane index */
                if ( (module_lane_rx == DIGI_SERDES_PIN_NOT_USED) ||
                     (module_lane_tx == DIGI_SERDES_PIN_NOT_USED) )
                {
                    PMC_RETURN(DIGI_ERR_INVALID_ARG);
                }
                
                PMC_LOG_TRACE("\nsifd_serdes_subsystem_configure, tx: %d, rx: %d, subsystem: %d, master: %d\n",module_lane_tx,module_lane_rx,post_mux_lane,port_def_ptr->master_lane);
                
                if (xfi_sys_pins[j].los_pin == DIGI_SERDES_PORT_NO_LOS_SOURCE) {

                    los_cfg.pin_los_en = FALSE;
                } else {

                    los_cfg.pin_los_en = TRUE;
                }

                los_cfg.los_pol_inv = FALSE;
                
                los_cfg.pin_los_inv = xfi_sys_pins[j].los_inv;
                rc = sifd_serdes_subsystem_configure(digi_handle->sifd_handle,
                                                     post_mux_lane, 
                                                     module_lane_rx, 
                                                     module_lane_tx, 
                                                     (sifd_dsis_type_t)subsystem_type,
                                                     single_lane_otn_cbrc,
                                                     port_def_ptr->master_lane);
                PMC_LOG_TRACE("\n%s\n",pmc_log_string_get(rc));
                PMC_ATOMIC_YIELD(digi_handle,0);
                if (rc != PMC_SUCCESS)
                {
                    PMC_RETURN(rc);  
                }

                if (port_def_ptr->los_pin == DIGI_SERDES_PORT_DLOS_LOS_SOURCE) 
                {
                    /* Apply DLOS settings if not in DLOS */
                    rc = sifd_subsystem_los_configure(digi_handle->sifd_handle, 
                                                      j, 
                                                      post_mux_lane, 
                                                      &los_cfg,
                                                      TRUE);

                } 
                else 
                {
                    /* Apply LOS settings if not in DLOS modes*/
                    rc = sifd_subsystem_los_configure(digi_handle->sifd_handle, 
                                                      xfi_sys_pins[j].los_pin, 
                                                      post_mux_lane, 
                                                      &los_cfg,
                                                      FALSE);
                }

                if (rc != PMC_SUCCESS) {

                    PMC_RETURN(rc);
                }

            }
            else
            {
                skipped_pins++;
            }
        }
    }

    PMC_RETURN(rc);
} /* digi_serdes_sifd_configure */

/*******************************************************************************
* digi_serdes_ge_all_ports_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The function initializes GE serdes ports context and handle.
*
* INPUTS:
*   *digi_handle              - pointer to DIGI handle instance.
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS when ge serdes port context initialization succeeded
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_ge_all_ports_init(digi_handle_t *digi_handle)
{
    digi_ge_serdes_id_t ge_serdes[] = DIGI_GE_SERDES_ID_INIT_L;
    UINT32 i;
    PMC_ERROR result = PMC_SUCCESS;    

    PMC_ENTRY();

    PMC_ASSERT((NULL != digi_handle), DIGI_ERR_NULL_HANDLE, 0, 0);
    
    /* initialize handle pointer to ge_poart_ctxt to NULL */
    for (i = 0; i < DIGI_GE_SERDES_D8_2X_MAX * 2; i++)
    {
        digi_handle->ge_intf_handle->ge_port_ctxt[i] = NULL ;
    }

    /* initialize the state of the CSU shared by all serdes */
    digi_handle->ge_intf_handle->csu_enabled = FALSE;

    /* copy GE interface default description */
    /* link ge_ctxt with appropriate handle */
    /* and initialize serdes state to disabled */
    for (i = 0; i < DIGI_GE_SERDES_D8_2X_MAX*2; i++)
    {
        digi_handle->var.pin_bus_ctxt.ge_ctxt[i].port_id = ge_serdes[i];
        digi_handle->var.pin_bus_ctxt.ge_ctxt[i].enabled = FALSE;
        digi_handle->var.pin_bus_ctxt.ge_ctxt[i].loopback_enabled = FALSE;
        digi_handle->var.pin_bus_ctxt.ge_ctxt[i].prbs_mon_enabled = FALSE;
        digi_handle->var.pin_bus_ctxt.ge_ctxt[i].prbs_gen_enabled = FALSE;

        switch (ge_serdes[i])
        {
        case DIGI_SERDES_INTF_OHP_0:
            digi_handle->ge_intf_handle->ge_port_ctxt[(UINT8)digi_handle->var.pin_bus_ctxt.ge_ctxt[i].port_id] = &digi_handle->var.pin_bus_ctxt.ge_ctxt[i];             
            PMC_CTXT_REGISTER_PTR(&digi_handle->ge_intf_handle->ge_port_ctxt[(UINT8)digi_handle->var.pin_bus_ctxt.ge_ctxt[i].port_id], digi_handle);
            break;
        case DIGI_SERDES_INTF_OHP_1:
            digi_handle->ge_intf_handle->ge_port_ctxt[(UINT8)digi_handle->var.pin_bus_ctxt.ge_ctxt[i].port_id] = &digi_handle->var.pin_bus_ctxt.ge_ctxt[i];             
            PMC_CTXT_REGISTER_PTR(&digi_handle->ge_intf_handle->ge_port_ctxt[(UINT8)digi_handle->var.pin_bus_ctxt.ge_ctxt[i].port_id], digi_handle);
            break;
        case DIGI_SERDES_INTF_OHP_2:
            digi_handle->ge_intf_handle->ge_port_ctxt[(UINT8)digi_handle->var.pin_bus_ctxt.ge_ctxt[i].port_id] = &digi_handle->var.pin_bus_ctxt.ge_ctxt[i];
            PMC_CTXT_REGISTER_PTR(&digi_handle->ge_intf_handle->ge_port_ctxt[(UINT8)digi_handle->var.pin_bus_ctxt.ge_ctxt[i].port_id], digi_handle);
            break;
        case DIGI_SERDES_INTF_RI_RCP:        
            digi_handle->ge_intf_handle->ge_port_ctxt[(UINT8)digi_handle->var.pin_bus_ctxt.ge_ctxt[i].port_id] = &digi_handle->var.pin_bus_ctxt.ge_ctxt[i];
            PMC_CTXT_REGISTER_PTR(&digi_handle->ge_intf_handle->ge_port_ctxt[(UINT8)digi_handle->var.pin_bus_ctxt.ge_ctxt[i].port_id], digi_handle);
            break;
        case DIGI_SERDES_INTF_DI_RCP:  
            digi_handle->ge_intf_handle->ge_port_ctxt[(UINT8)digi_handle->var.pin_bus_ctxt.ge_ctxt[i].port_id] = &digi_handle->var.pin_bus_ctxt.ge_ctxt[i];         
            PMC_CTXT_REGISTER_PTR(&digi_handle->ge_intf_handle->ge_port_ctxt[(UINT8)digi_handle->var.pin_bus_ctxt.ge_ctxt[i].port_id], digi_handle);
            break;              
        case DIGI_SERDES_INTF_EMGMT:
            digi_handle->ge_intf_handle->ge_port_ctxt[(UINT8)digi_handle->var.pin_bus_ctxt.ge_ctxt[i].port_id] = &digi_handle->var.pin_bus_ctxt.ge_ctxt[i];
            PMC_CTXT_REGISTER_PTR(&digi_handle->ge_intf_handle->ge_port_ctxt[(UINT8)digi_handle->var.pin_bus_ctxt.ge_ctxt[i].port_id], digi_handle);
            break;
        default:        
            break;
        }

    }
    
    PMC_RETURN(result);
} /* digi_serdes_ge_all_ports_init */

/*******************************************************************************
* digi_ge_serdes_ctxt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The function finds the ge serdes constext associated to a given GE serdes.
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   ge_intf             - GE serdes interface description.
*
* OUTPUTS:
*   **ge_serdes_ctxt     - address of GE serdes context if found, otherwise NULL.
*
* RETURNS:
*   PMC_ERROR           - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ge_serdes_ctxt_get(digi_handle_t *digi_handle,
                                         digi_ge_serdes_id_t ge_intf,
                                         digi_ge_serdes_ctxt_t **ge_serdes_ctxt)
{
    BOOL8 found = FALSE;
    UINT32 i;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT((NULL != digi_handle), DIGI_ERR_NULL_HANDLE,  0, 0);
    for(i = 0; i < DIGI_SERDES_GE_PORTS_MAX && found == FALSE; i++)
    {        
        if (digi_handle->var.pin_bus_ctxt.ge_ctxt[i].port_id == ge_intf){           
            *ge_serdes_ctxt = &digi_handle->var.pin_bus_ctxt.ge_ctxt[i];
            found = TRUE;
        }
    }

    if (FALSE == found)
    {
        *ge_serdes_ctxt = NULL; 
        result = DIGI_ERR_INVALID_ARG;
    }

    PMC_RETURN(result);
}/* digi_ge_serdes_ctxt_get */

/*******************************************************************************
* digi_serdes_s16_rate_enet_mapping_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Returns TRUE when S16 rate maps to enet interface.
*
* INPUTS:
*  serdes_rate   - SERDES rate to convert
*
* OUTPUTS:   
*  NONE.
*
* RETURNS:
*   Returns TRUE when S16 rate maps to enet interface.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 digi_serdes_s16_rate_enet_mapping_get(digi_serdes_port_mode_t    serdes_rate)
{
    BOOL8                     enet_mapping;
    
    PMC_ENTRY();
    
    enet_mapping = FALSE;
    
    /* Convert SERDES rate to host message parameters */
    switch (serdes_rate) 
    {
    case DIGI_SERDES_PORT_MODE_S16_100_GE:
    
        enet_mapping = TRUE;
        break;
    
    case DIGI_SERDES_PORT_MODE_S16_40_GE:
    
        enet_mapping = TRUE;
        break;
    
    case DIGI_SERDES_PORT_MODE_S16_10_GE:    
        enet_mapping = TRUE;
        break;
    default:
        break;
    }

    PMC_RETURN(enet_mapping);
}/* digi_serdes_s16_rate_enet_mapping_get */

/*******************************************************************************
* digi_sys_serdes_n1_optic_pin_use_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves masks of serdes port usage relative to N1 serdes pin 
*   masks.
*
* INPUTS:
*   *digi_handle              - pointer to DIGI handle instance.
*
* OUTPUTS:
*
*
* RETURNS:
*   UINT32                    - mask of N1 pins used by Serdes Ports
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 digi_sys_serdes_n1_optic_pin_use_get(digi_handle_t *digi_handle)
{
    UINT32 i, sys_serdes_n1_optic_msk = 0;
    digi_xfi_pin_ctxt_t *sys_ctxt_ptr = NULL;
    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    /* Test all system serdes pins for assignment to optical modules */
    for (i = 0; i < DIGI_SERDES_SYS_LANES_MAX; i++) {

        sys_ctxt_ptr = &(digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i]);
        if(sys_ctxt_ptr->cage_id != DIGI_SERDES_PIN_NOT_USED)
        {
            sys_serdes_n1_optic_msk |= 1 << i;
        }
    }
    PMC_RETURN(sys_serdes_n1_optic_msk);
} /* digi_sys_serdes_n1_optic_pin_use_get */



/*******************************************************************************
* digi_serdes_port_lifd_sifd_subsytem_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a helper function to disable the lifd and sifd  subsytem lane based 
*   on the port_uid.
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   port_uid           - the unique identifier of the SERDES port.
*
* OUTPUTS:
*
* RETURNS:
*   DIGI_ERR_UNKNOWN on failure, PMC_SUCCESS otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_port_lifd_sifd_subsytem_disable(digi_handle_t *digi_handle,
                                                             UINT32 port_uid)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;

    digi_serdes_intf_t intf_type;

    PMC_ENTRY();

    rc = digi_serdes_port_intf_type_get(digi_handle, port_uid, &intf_type);

    if (rc == PMC_SUCCESS) {

        /* Case statement */
        switch(intf_type) {

        case DIGI_SERDES_LINE_INTF:
            rc = digi_serdes_port_lifd_xfi_disable(digi_handle, port_uid);
            break;
        case DIGI_SERDES_SYSTEM_INTF:
            rc = digi_serdes_port_sifd_xfi_disable(digi_handle, port_uid);
            break;
        case DIGI_SERDES_SFI_51_INTF:
            rc = digi_serdes_port_lifd_sfi51_disable(digi_handle, port_uid);
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, intf_type, port_uid);
            break; 
        }
    }

    PMC_RETURN(rc);

} /* digi_serdes_port_lifd_sifd_subsytem_disable */


/*******************************************************************************
* digi_serdes_port_prov_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function returns the port rate provioning status of a port
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   *port_uid              - serdes port context unique id
*
* OUTPUTS: 
*    None
*  
* RETURNS:
*    The port rate status
*
* NOTES:
*
*******************************************************************************/
PUBLIC digi_serdes_port_rate_prov_state_t digi_serdes_port_prov_status_get(digi_handle_t *digi_handle,
                                                                           UINT32 port_uid)
{    
    UINT32 i = 0;
    BOOL8 found = FALSE;
    digi_serdes_port_rate_prov_state_t prov_port_state = LAST_DIGI_SERDES_PORT_RATE_STATE;
    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that serdes type is associated with a cage */
    /* and get port_uidto retrieve serdes prov state and get port_mode */

    while (TRUE != found && i < DIGI_SERDES_XFI_LANES_MAX)
    {        
        if (digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].port_uid == port_uid)
        {
            prov_port_state = digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].serdes_rate_state.rate_prov_state;
            found = TRUE;
        }
        i++;
    }

    i = 0;
    while (TRUE != found && i < DIGI_SERDES_SYS_LANES_MAX)
    {        
        if (digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].port_uid == port_uid)
        {
            prov_port_state = digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].serdes_rate_state.rate_prov_state;
            found = TRUE;
        }
        i++;
    }
    
    i = 0;
    while (TRUE != found && i < DIGI_NUM_SFI51_PORTS_MAX)
    {
        if (digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].port_uid == port_uid)
        {
            prov_port_state = digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].serdes_rate_state.rate_prov_state;
            found = TRUE;
        }
        i++;
    }
    
    PMC_RETURN(prov_port_state);
}/*digi_serdes_port_prov_status_get*/


/*******************************************************************************
* digi_serdes_ilkn_enum_conversion                                                          
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:                                                                  
*   This function maps digi_ilkn_serdes_rate_t to digi_serdes_port_mode_t for
*   E1, N1 and N2 Serdes types.
*
*   If the digi_ilkn_serdes_rate_t term is not valid for one of the Serdes
*   types, the value will be returned as LAST_DIGI_SERDES_PORT_MODE.   
*                                                                               
* INPUTS:                                                                       
*   *digi_handle              - pointer to DIGI handle instance.                
*   ilkn_rate                 - Enum, defining ILKN serdes pin rate.
*                                                                               
* OUTPUTS:    
*   *serdes_e1_port_mode_ptr  - Enum, defining E1 rate for ILKN serdes pin rate.                                                                  
*   *serdes_n1_port_mode_ptr  - Enum, defining N1 rate for ILKN serdes pin rate.                                                                  
*   *serdes_n2_port_mode_ptr  - Enum, defining N2 rate for ILKN serdes pin rate.                                                                  
*                                                                               
* RETURNS:                                                                      
*   PMC_ERROR                                                                   
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_ilkn_enum_conversion(digi_handle_t *digi_handle, 
                                                  digi_ilkn_serdes_rate_t ilkn_rate, 
                                                  digi_serdes_port_mode_t *serdes_e1_port_mode_ptr,
                                                  digi_serdes_port_mode_t *serdes_n1_port_mode_ptr,
                                                  digi_serdes_port_mode_t *serdes_n2_port_mode_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);     

    switch(ilkn_rate)
    {
    case DIGI_ILKN_SERDES_RATE_3G125:
        *serdes_e1_port_mode_ptr = DIGI_SERDES_PORT_MODE_T8_3G125;
        *serdes_n1_port_mode_ptr = DIGI_SERDES_PORT_MODE_S16_3G125;
        *serdes_n2_port_mode_ptr = DIGI_SERDES_PORT_MODE_C8_3G125;
        break;
    case DIGI_ILKN_SERDES_RATE_6G25:
        *serdes_e1_port_mode_ptr = DIGI_SERDES_PORT_MODE_T8_6G25;
        *serdes_n1_port_mode_ptr = DIGI_SERDES_PORT_MODE_S16_6G25;
        *serdes_n2_port_mode_ptr = DIGI_SERDES_PORT_MODE_C8_6G25;
        break;                      
    case DIGI_ILKN_SERDES_RATE_10G3125:
        *serdes_e1_port_mode_ptr = LAST_DIGI_SERDES_PORT_MODE;
        *serdes_n1_port_mode_ptr = DIGI_SERDES_PORT_MODE_S16_10G3125;
        *serdes_n2_port_mode_ptr = LAST_DIGI_SERDES_PORT_MODE;
        break;                      
    case DIGI_ILKN_SERDES_RATE_11G5:
        *serdes_e1_port_mode_ptr = LAST_DIGI_SERDES_PORT_MODE;
        *serdes_n1_port_mode_ptr = DIGI_SERDES_PORT_MODE_S16_11G5;
        *serdes_n2_port_mode_ptr = LAST_DIGI_SERDES_PORT_MODE;
        break;                      
    case DIGI_ILKN_SERDES_RATE_12G5:
        *serdes_e1_port_mode_ptr = LAST_DIGI_SERDES_PORT_MODE;
        *serdes_n1_port_mode_ptr = DIGI_SERDES_PORT_MODE_S16_12G5;
        *serdes_n2_port_mode_ptr = LAST_DIGI_SERDES_PORT_MODE;
        break;                      
    default:
        *serdes_e1_port_mode_ptr = LAST_DIGI_SERDES_PORT_MODE;
        *serdes_n1_port_mode_ptr = LAST_DIGI_SERDES_PORT_MODE;
        *serdes_n2_port_mode_ptr = LAST_DIGI_SERDES_PORT_MODE;
        result = DIGI_ERR_INVALID_ARG;
    }

    PMC_RETURN(result);

} /* digi_serdes_ilkn_enum_conversion */

/*******************************************************************************
* digi_bp_intf_serdes_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configure E1, N1 and N2 system serdes ports that have been
*   specified for usage with backplane interface. This function is used to 
*   configure SERDES rate or loopback 
*
* INPUTS:
*   *digi_handle              - pointer to DIGI handle instance.
*   *sifd_1_cfg_ptr           - pointer to ILKN1 interface context
*   *sifd_2_cfg_ptr           - pointer to ILKN2 interface context
*   bp_intf_serdes_operation  - the requested operation
*   enable                    - the enable associated with the requested operation

* OUTPUTS:
*
* RETURNS:
*   PMC_ERROR
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_bp_intf_serdes_cfg(digi_handle_t *digi_handle,
                                         digi_sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                         digi_sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr,
                                         digi_bp_intf_serdes_operation_t bp_intf_serdes_operation,
                                         BOOL8 enable)
{
    BOOL8 enabled;
    PMC_ERROR rc;
    PMC_ENTRY();
    rc = digi_bp_intf_serdes_cfg_helper(digi_handle,sifd_1_cfg_ptr,sifd_2_cfg_ptr,
                                        bp_intf_serdes_operation,enable,&enabled);
    PMC_RETURN(rc);
}

/*******************************************************************************
* digi_bp_intf_serdes_loopback_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the status of a backplane SERDES loopback.
*
* INPUTS:
*   *digi_handle              - pointer to DIGI handle instance.
*   *sifd_1_cfg_ptr           - pointer to ILKN1 interface context
*   *sifd_2_cfg_ptr           - pointer to ILKN2 interface context
*
* OUTPUTS:
*  *enabled_ptr               - Set to true if the loopback is enabled
*
* RETURNS:
*   PMC_ERROR
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_bp_intf_serdes_loopback_status_get(digi_handle_t *digi_handle,
                                                         digi_sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                                         digi_sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr,
                                                         BOOL8 *enabled_ptr)
{
    PMC_ERROR rc;
    PMC_ENTRY();
    rc = digi_bp_intf_serdes_cfg_helper(digi_handle,sifd_1_cfg_ptr,sifd_2_cfg_ptr,
                                        DIGI_BP_INTF_SERDES_LPBK_STATUS_GET,FALSE,enabled_ptr);
    PMC_RETURN(rc);
}
/* digi_bp_intf_serdes_status_get */

/*******************************************************************************
* digi_bp_intf_serdes_cfg_helper
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Helper function for digi_bp_intf_serdes_cfg and digi_bp_intf_serdes_status_get.
*   If the function is called from bp_intf_serdes_cfg, it's used ot configure
*   the E1, N1 and N2 system SERDES Ports. If it's called from 
*   digi_bp_intf_serdes_status_get, it's used to retrieve the loopback status.
*
* INPUTS:
*   *digi_handle              - pointer to DIGI handle instance.
*   *sifd_1_cfg_ptr           - pointer to ILKN1 interface context
*   *sifd_2_cfg_ptr           - pointer to ILKN2 interface context
*   bp_intf_serdes_operation  - the requested operation
*   enable                    - the enable associated with the requested operation
*
* OUTPUTS:
*   enabled_ptr               - the enable associated with the requested operation
*
* RETURNS:
*   PMC_ERROR
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_bp_intf_serdes_cfg_helper(digi_handle_t *digi_handle,
                                                 digi_sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                                 digi_sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr,
                                                 digi_bp_intf_serdes_operation_t bp_intf_serdes_operation,
                                                 BOOL8 enable,
                                                 BOOL8 *enabled_ptr)
{

    digi_ilkn_serdes_rate_t ilkn_1_rate = LAST_DIGI_ILKN_SERDES_RATE;
    digi_ilkn_serdes_rate_t ilkn_2_rate = LAST_DIGI_ILKN_SERDES_RATE;
    UINT32 ilkn1_e1_39_20_msk = 0;
    UINT32 ilkn1_e1_19_00_msk = 0;
    UINT32 ilkn1_n1_msk = 0;     
    UINT32 ilkn1_n2_msk = 0;
    UINT32 ilkn2_e1_39_20_msk = 0;

    UINT32 ilkn2_e1_19_00_msk = 0;
    UINT32 ilkn2_n1_msk = 0;     
    UINT32 ilkn2_n2_msk = 0;   

    digi_ilkn_serdes_rate_t e1_19_00_rate = LAST_DIGI_ILKN_SERDES_RATE;
    digi_ilkn_serdes_rate_t e1_39_20_rate = LAST_DIGI_ILKN_SERDES_RATE;
    digi_ilkn_serdes_rate_t n2_rate = LAST_DIGI_ILKN_SERDES_RATE;
    UINT32 e1_39_20_cfg_msk = 0;
    UINT32 e1_19_00_cfg_msk = 0;
    UINT32 n2_cfg_msk = 0;

    digi_serdes_port_mode_t serdes_e1_19_00_port_mode = LAST_DIGI_SERDES_PORT_MODE;
    digi_serdes_port_mode_t serdes_e1_39_20_port_mode = LAST_DIGI_SERDES_PORT_MODE;
    digi_serdes_port_mode_t serdes_n1_port_mode = LAST_DIGI_SERDES_PORT_MODE;
    digi_serdes_port_mode_t serdes_n2_port_mode = LAST_DIGI_SERDES_PORT_MODE;  
    digi_serdes_port_mode_t port_mode_dummy     = LAST_DIGI_SERDES_PORT_MODE;  

    sifd_ilkn_1_cfg_t loc_sifd_1_cfg;
    sifd_ilkn_2_cfg_t loc_sifd_2_cfg;
    sifd_ilkn_1_cfg_t *loc_sifd_1_cfg_ptr;
    sifd_ilkn_2_cfg_t *loc_sifd_2_cfg_ptr;

    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0); 

    *enabled_ptr = FALSE;
    digi_ilkn_serdes_pin_use_get(digi_handle,
                                 sifd_1_cfg_ptr,
                                 sifd_2_cfg_ptr,
                                 &ilkn_1_rate,
                                 &ilkn_2_rate,
                                 &ilkn1_e1_39_20_msk,
                                 &ilkn1_e1_19_00_msk,
                                 &ilkn1_n1_msk,
                                 &ilkn1_n2_msk,
                                 &ilkn2_e1_39_20_msk,
                                 &ilkn2_e1_19_00_msk,
                                 &ilkn2_n1_msk,
                                 &ilkn2_n2_msk);

    /* prepare SIFD configurations */
    if (NULL != sifd_1_cfg_ptr)
    {
        result = digi_copy_struct_ilkn_1_cfg(&loc_sifd_1_cfg, sifd_1_cfg_ptr);
        loc_sifd_1_cfg_ptr = &loc_sifd_1_cfg;
    }
    else
    {
        loc_sifd_1_cfg_ptr = NULL;
    }

    if (NULL != sifd_2_cfg_ptr)
    {
        result = digi_copy_struct_ilkn_2_cfg(&loc_sifd_2_cfg, sifd_2_cfg_ptr);
        loc_sifd_2_cfg_ptr = &loc_sifd_2_cfg;
    }
    else
    {
        loc_sifd_2_cfg_ptr = NULL;
    }



    /* merge E1[39:20] masks and confirm the rate is the same */
    if(ilkn1_e1_39_20_msk != 0)
    {
        e1_39_20_rate = ilkn_1_rate;
        PMC_ASSERT(e1_39_20_rate!=LAST_DIGI_ILKN_SERDES_RATE, DIGI_ERR_INVALID_ARG, 0, 0);        
        e1_39_20_cfg_msk = ilkn1_e1_39_20_msk;
    }

    if(ilkn2_e1_39_20_msk != 0)
    {
        if(e1_39_20_rate == LAST_DIGI_ILKN_SERDES_RATE)
        {
            e1_39_20_rate = ilkn_2_rate;
            PMC_ASSERT(e1_39_20_rate!=LAST_DIGI_ILKN_SERDES_RATE, DIGI_ERR_INVALID_ARG, 0, 0);        
        } else {
            PMC_ASSERT(e1_39_20_rate==ilkn_2_rate, DIGI_ERR_SERDES_CSU_MULTIRATE_REQUEST, 0, 0);
        }
        PMC_ASSERT(((e1_39_20_cfg_msk&ilkn2_e1_39_20_msk) == 0), DIGI_ERR_SERDES_PIN_ALLOC_COLLISION, 0, 0);
        e1_39_20_cfg_msk |= ilkn2_e1_39_20_msk;
    }



    /* merge E1[19:00] masks and confirm the rate is the same */
    if(ilkn1_e1_19_00_msk != 0)
    {
        e1_19_00_rate = ilkn_1_rate;
        PMC_ASSERT(e1_19_00_rate!=LAST_DIGI_ILKN_SERDES_RATE, DIGI_ERR_INVALID_ARG, 0, 0);        
        e1_39_20_cfg_msk = ilkn1_e1_39_20_msk;
    }

    if(ilkn2_e1_19_00_msk != 0)
    {
        if(e1_19_00_rate == LAST_DIGI_ILKN_SERDES_RATE)
        {
            e1_19_00_rate = ilkn_2_rate;
            PMC_ASSERT(e1_19_00_rate!=LAST_DIGI_ILKN_SERDES_RATE, DIGI_ERR_INVALID_ARG, 0, 0);        
        } else {
            PMC_ASSERT(e1_19_00_rate==ilkn_2_rate, DIGI_ERR_SERDES_CSU_MULTIRATE_REQUEST, 0, 0);
        }
        PMC_ASSERT(((e1_19_00_cfg_msk&ilkn2_e1_19_00_msk) == 0), DIGI_ERR_SERDES_PIN_ALLOC_COLLISION, 0, 0);
        e1_19_00_cfg_msk |= ilkn2_e1_19_00_msk;
    }


    if(e1_39_20_cfg_msk != 0 || e1_19_00_cfg_msk != 0)
    {
        if(result == PMC_SUCCESS && e1_19_00_cfg_msk != 0)
        {
            result = digi_serdes_ilkn_enum_conversion(digi_handle, 
                                                      e1_19_00_rate, 
                                                      &serdes_e1_19_00_port_mode,
                                                      &port_mode_dummy,
                                                      &port_mode_dummy);
        }
    
        if(result == PMC_SUCCESS && e1_39_20_cfg_msk != 0)
        {
            result = digi_serdes_ilkn_enum_conversion(digi_handle, 
                                                      e1_39_20_rate, 
                                                      &serdes_e1_39_20_port_mode,
                                                      &port_mode_dummy,
                                                      &port_mode_dummy);
        }
    
        if(result == PMC_SUCCESS)
        {
            switch (bp_intf_serdes_operation)
            {
            case DIGI_BP_INTF_SERDES_CFG:
            {
                result = digi_serdes_t8_cfg(digi_handle, 
                                            e1_19_00_cfg_msk, 
                                            e1_39_20_cfg_msk, 
                                            serdes_e1_19_00_port_mode, 
                                            serdes_e1_39_20_port_mode);
                break;
            }
            case DIGI_BP_INTF_SERDES_LPBK_CFG:
            {
                result = digi_serdes_loopback_t8_cfg(digi_handle, 
                                                     e1_19_00_cfg_msk, 
                                                     e1_39_20_cfg_msk, 
                                                     serdes_e1_19_00_port_mode, 
                                                     serdes_e1_39_20_port_mode,
                                                     enable);
                break;
            } 
            case DIGI_BP_INTF_SERDES_LPBK_STATUS_GET:
            {
                result = digi_serdes_loopback_t8_status_get(digi_handle, 
                                                            e1_19_00_cfg_msk, 
                                                            e1_39_20_cfg_msk, 
                                                            serdes_e1_19_00_port_mode, 
                                                            serdes_e1_39_20_port_mode,
                                                            enabled_ptr);
                break;
            } 
            default:
                result = DIGI_ERR_INVALID_ARG;
            }            
        }
    }

    /* merge N2[07:00] masks and confirm the rate is the same */
    if(ilkn1_n2_msk != 0)
    {
        n2_rate = ilkn_1_rate;
        PMC_ASSERT(n2_rate!=LAST_DIGI_ILKN_SERDES_RATE, DIGI_ERR_INVALID_ARG, 0, 0);        
        n2_cfg_msk = ilkn1_n2_msk;
    }

    if(ilkn2_n2_msk != 0)
    {
        if(n2_rate == LAST_DIGI_ILKN_SERDES_RATE)
        {
            n2_rate = ilkn_2_rate;
            PMC_ASSERT(n2_rate!=LAST_DIGI_ILKN_SERDES_RATE, DIGI_ERR_INVALID_ARG, 0, 0);        
        } else {
            PMC_ASSERT(n2_rate==ilkn_2_rate, DIGI_ERR_SERDES_CSU_MULTIRATE_REQUEST, 0, 0);
        }
        PMC_ASSERT(((n2_cfg_msk&ilkn2_n2_msk) == 0), DIGI_ERR_SERDES_PIN_ALLOC_COLLISION, 0, 0);
        n2_cfg_msk |= ilkn2_n2_msk;
    }

    /* perform N2 configuration - C8 */
    if(n2_cfg_msk != 0)
    {
        if(result == PMC_SUCCESS)
        {
            result = digi_serdes_ilkn_enum_conversion(digi_handle, 
                                                      n2_rate, 
                                                      &port_mode_dummy,
                                                      &port_mode_dummy,
                                                      &serdes_n2_port_mode);
        }
        if(result == PMC_SUCCESS)
        {
            switch (bp_intf_serdes_operation)
            {
            case DIGI_BP_INTF_SERDES_CFG:
            {
                result = digi_serdes_c8_cfg(digi_handle, n2_cfg_msk, serdes_n2_port_mode);
                break;
            }
            case DIGI_BP_INTF_SERDES_LPBK_CFG:
            {
                result = digi_serdes_loopback_c8_cfg(digi_handle, n2_cfg_msk, serdes_n2_port_mode,enable);
                break;
            } 
            case DIGI_BP_INTF_SERDES_LPBK_STATUS_GET:
            {
                result = digi_serdes_loopback_c8_status_get(digi_handle, n2_cfg_msk, serdes_n2_port_mode,enabled_ptr);
                break;
            }
            default:
                result = DIGI_ERR_INVALID_ARG;
            }       
        }
    }

    /* perform N1 configuration - S16 */
    if(result == PMC_SUCCESS)
    {
        if(ilkn1_n1_msk != 0)
        {
            PMC_ASSERT(ilkn_1_rate != LAST_DIGI_ILKN_SERDES_RATE, DIGI_ERR_INVALID_ARG, 0, 0);

            result = digi_serdes_ilkn_enum_conversion(digi_handle, 
                                                      ilkn_1_rate, 
                                                      &port_mode_dummy,
                                                      &serdes_n1_port_mode,
                                                      &port_mode_dummy);
            if(result == PMC_SUCCESS)
            {
                switch (bp_intf_serdes_operation)
                {
                case DIGI_BP_INTF_SERDES_CFG:
                {
                    result = digi_serdes_s16_cfg(digi_handle, HOSTMSG_S16_SERDES_INTF_SYS, ilkn1_n1_msk, serdes_n1_port_mode, 0);
                    break;
                }
                case DIGI_BP_INTF_SERDES_LPBK_CFG:
                {
                    result = digi_serdes_loopback_s16_cfg(digi_handle, HOSTMSG_S16_SERDES_INTF_SYS, ilkn1_n1_msk, serdes_n1_port_mode, enable);
                    break;
                } 
                case DIGI_BP_INTF_SERDES_LPBK_STATUS_GET:
                {
                    result = digi_serdes_loopback_s16_status_get(digi_handle, HOSTMSG_S16_SERDES_INTF_SYS, ilkn1_n1_msk, serdes_n1_port_mode, enabled_ptr);
                    break;
                } 
                default:
                    result = DIGI_ERR_INVALID_ARG;
                }                              
            }
        }
    }

    if(result == PMC_SUCCESS)
    {
        if(ilkn2_n1_msk != 0)
        {
            PMC_ASSERT(ilkn_2_rate != LAST_DIGI_ILKN_SERDES_RATE, DIGI_ERR_INVALID_ARG, 0, 0);

            result = digi_serdes_ilkn_enum_conversion(digi_handle, 
                                                      ilkn_2_rate, 
                                                      &port_mode_dummy,
                                                      &serdes_n1_port_mode,
                                                      &port_mode_dummy);
            if(result == PMC_SUCCESS)
            {
                switch (bp_intf_serdes_operation)
                {
                case DIGI_BP_INTF_SERDES_CFG:
                {
                    result = digi_serdes_s16_cfg(digi_handle, HOSTMSG_S16_SERDES_INTF_SYS, ilkn2_n1_msk, serdes_n1_port_mode, 0);
                    break;
                }
                case DIGI_BP_INTF_SERDES_LPBK_CFG:
                {
                    result = digi_serdes_loopback_s16_cfg(digi_handle, HOSTMSG_S16_SERDES_INTF_SYS, ilkn2_n1_msk, serdes_n1_port_mode,enable);
                    break;
                } 
                case DIGI_BP_INTF_SERDES_LPBK_STATUS_GET:
                {
                    result = digi_serdes_loopback_s16_status_get(digi_handle, HOSTMSG_S16_SERDES_INTF_SYS, ilkn2_n1_msk, serdes_n1_port_mode,enabled_ptr);
                    break;
                } 
                default:
                    result = DIGI_ERR_INVALID_ARG;
                } 
            }
        }
    }


    if (result == PMC_SUCCESS)
    {
        if(DIGI_BP_INTF_SERDES_LPBK_CFG == bp_intf_serdes_operation)
        {
            /* toggle sifd ilkn rx reset after T8, C8 and S16 serdes loopback configuration */
            result = sifd_ilkn_rx_reset_toggle(digi_handle->sifd_handle,
                                               loc_sifd_1_cfg_ptr, 
                                               loc_sifd_2_cfg_ptr);
            
            /* If loopback is enabled, disable serdes->interlaken DLOS inversion. */
            if (result == PMC_SUCCESS)
            {
                result = sifd_ilkn_dlos_cfg(digi_handle->sifd_handle,
                                            loc_sifd_1_cfg_ptr,
                                            loc_sifd_2_cfg_ptr,
                                            enable ? FALSE : TRUE);
            }
        }
    }
    
    PMC_RETURN(result);

} /* digi_bp_intf_serdes_cfg_helper */


/*******************************************************************************
* digi_serdes_s16_rate_convert
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Converts the enum type digi_serdes_port_mode_t for S16 SERDES rates
*  to the hostmsg_s16_serdes_rate_t type for sending a firmware host message.
*
* INPUTS:
*  serdes_rate   - SERDES rate to convert
*
* OUTPUTS:
*   hostmsg_s16_serdes_rate_ptr - Pointer to converted SERDES rate
*  s16_serdes_rate              - The serdes rate in DOUBLE notation 
*  s16_otu_client               - TRUE when it is an OTU rate, otherwise FALSE
*  s16_serdes_port_mode_ptr     - The serdes rate converted in S16 serdes port mode
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_s16_rate_convert(digi_serdes_port_mode_t serdes_rate, 
                                              hostmsg_s16_serdes_rate_t *hostmsg_s16_serdes_rate_ptr,
                                              DOUBLE                    *s16_serdes_rate,
                                              BOOL8                     *s16_otu_client,
                                              s16_port_mode_t           *s16_serdes_port_mode_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;        

    PMC_ENTRY();
    
    *s16_otu_client  = FALSE;
    *s16_serdes_rate = 0;

    /* Convert SERDES rate to host message parameters */
    switch (serdes_rate) {
    case DIGI_SERDES_PORT_MODE_ANY_LOWPWR:
    {
        /* Host message for low power mode ignores rate */
        *hostmsg_s16_serdes_rate_ptr = (hostmsg_s16_serdes_rate_t) 0;  
        *s16_serdes_rate             = 0;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_6G250;  /* Interlaken */
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_3G125:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_3G125;  /* Interlaken */
        *s16_serdes_rate = UTIL_GLOBAL_ILKN_3G125_RATE;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_3G125;  /* Interlaken */
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_6G25:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_6G250;  /* Interlaken */
        *s16_serdes_rate = UTIL_GLOBAL_ILKN_6G25_RATE;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_6G250;  /* Interlaken */
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_10G3125:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_10G312;  /* Interlaken */
        *s16_serdes_rate = UTIL_GLOBAL_ILKN_10G3125_RATE;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_10G312;  /* Interlaken */
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_11G5:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_11G500;  /* Interlaken */
        *s16_serdes_rate = UTIL_GLOBAL_ILKN_11G5_RATE;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_11G500;  /* Interlaken */
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_12G5:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_12G500;  /* Interlaken */
        *s16_serdes_rate = UTIL_GLOBAL_ILKN_12G5_RATE;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_12G500;  /* Interlaken */
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_100_GE:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_10G312;
        *s16_serdes_rate = UTIL_GLOBAL_ENET_100GE_RATE / 10;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_10G312;
        PMC_LOG_TRACE("DIGI_SERDES_PORT_MODE_S16_100_GE converted serdes_rate = %f\n", *s16_serdes_rate);
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_DIC:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_10G312_DIC;
        *s16_serdes_rate = UTIL_GLOBAL_ENET_10GE_RATE;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_10G312_DIC;
        PMC_LOG_TRACE("DIGI_SERDES_PORT_MODE_S16_100_GE_DIC converted serdes_rate = %f\n", *s16_serdes_rate);
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_40_GE:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_10G312;
        *s16_serdes_rate = UTIL_GLOBAL_ENET_40GE_RATE / 4;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_10G312;
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_10_GE:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_10G312;  /* 10GE - LAN */
        *s16_serdes_rate =  UTIL_GLOBAL_ENET_10GE_RATE;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_10G312;  /* 10GE - LAN */
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_FC_1200:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_10G518;
        *s16_serdes_rate = UTIL_GLOBAL_FC_1200_RATE;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_10G518;
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_FC_800:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_8G500;
        *s16_serdes_rate = UTIL_GLOBAL_FC_800_RATE;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_8G500;
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_10G_GDPS:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_10G000;
        *s16_serdes_rate =  UTIL_GLOBAL_GDPS_10G_RATE;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_10G000;
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_5G_GDPS:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_5G000;
        *s16_serdes_rate = UTIL_GLOBAL_GDPS_5G_RATE;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_5G000;
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_CPRI_10G:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_9G830;
        *s16_serdes_rate = UTIL_GLOBAL_CPRI_7_RATE;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_9G830;
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_CPRI_6G:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_6G144;
        *s16_serdes_rate = UTIL_GLOBAL_CPRI_6_RATE;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_6G144;
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_CPRI_5G:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_4G915;
        *s16_serdes_rate = UTIL_GLOBAL_CPRI_5_RATE;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_4G915;
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_OC_768:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_9G953;
        *s16_serdes_rate = UTIL_GLOBAL_STS_768_STM_256_RATE/4;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_9G953;
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_OC_192:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_9G953;
        *s16_serdes_rate = UTIL_GLOBAL_STS_192_STM_64_RATE;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_9G953;
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_OTU4:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_11G180;
        *s16_otu_client = TRUE;
        *s16_serdes_rate = UTIL_GLOBAL_OTU4_RATE/10;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_11G180;
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_OTU3:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_10G754;
        *s16_otu_client = TRUE;
        *s16_serdes_rate = UTIL_GLOBAL_OTU3_RATE/4;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_10G754;
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_OTU3E2:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_11G145;
        *s16_otu_client = TRUE;
        *s16_serdes_rate = UTIL_GLOBAL_OTU3E2_RATE/4;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_11G145;
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_OTU3E1:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_11G142;
        *s16_otu_client = TRUE;
        *s16_serdes_rate = UTIL_GLOBAL_OTU3E1_RATE/4;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_11G142;
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_OTU2:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_10G709;
        *s16_otu_client = TRUE;
        *s16_serdes_rate = UTIL_GLOBAL_OTU2_RATE;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_10G709;
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_OTU2E:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_11G095;
        *s16_otu_client = TRUE;
        *s16_serdes_rate = UTIL_GLOBAL_OTU2E_RATE;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_11G095;
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_OTU1E:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_11G049;
        *s16_otu_client = TRUE;
        *s16_serdes_rate = UTIL_GLOBAL_OTU1E_RATE;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_11G049;
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_OTU2F:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_11G317;
        *s16_otu_client = TRUE;
        *s16_serdes_rate = UTIL_GLOBAL_OTU2F_RATE;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_11G317;
        break;
    }
    case DIGI_SERDES_PORT_MODE_S16_OTU1F:
    {
        *hostmsg_s16_serdes_rate_ptr = HOSTMSG_S16_SERDES_RATE_11G270;
        *s16_otu_client = TRUE;
        *s16_serdes_rate =  UTIL_GLOBAL_OTU1F_RATE;
        *s16_serdes_port_mode_ptr = S16_PORT_MODE_11G270;
        break;
    }
    default:
    {
        *s16_serdes_rate =  HOSTMSG_S16_SERDES_RATE_MAX_NUM;
        *s16_serdes_port_mode_ptr = LAST_S16_PORT_MODE;  
        break;
    }
    } /* end switch */

    PMC_RETURN(result);

} /* End: digi_serdes_s16_rate_convert() */

/*******************************************************************************
* digi_serdes_t8_rate_convert
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Converts the enum type digi_serdes_port_mode_t for T8 SERDES rates
*  to the hostmsg_t8_serdes_rate_t type for sending a firmware host message.
*
* INPUTS:
*  serdes_rate   - SERDES rate to convert
*
* OUTPUTS:
*   hostmsg_t8_serdes_rate_ptr - Pointer to converted SERDES rate
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_t8_rate_convert(digi_serdes_port_mode_t     serdes_rate, 
                                             hostmsg_t8_serdes_rate_t   *hostmsg_t8_serdes_rate_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;        
    
    PMC_ENTRY();

    PMC_ASSERT(NULL != hostmsg_t8_serdes_rate_ptr, DIGI_ERR_INVALID_ARG, 0, 0);

    switch (serdes_rate) {
    case DIGI_SERDES_PORT_MODE_T8_3G125:
    {
        *hostmsg_t8_serdes_rate_ptr = HOSTMSG_T8_SERDES_RATE_3G125;
        break;
    }
    case DIGI_SERDES_PORT_MODE_T8_6G25:
    {
        *hostmsg_t8_serdes_rate_ptr = HOSTMSG_T8_SERDES_RATE_6G25;
        break;
    }
    default:
        *hostmsg_t8_serdes_rate_ptr =  HOSTMSG_T8_SERDES_RATE_MAX_NUM;
        break;
    } /* end switch */
    
    PMC_RETURN(rc);
}/*digi_serdes_t8_rate_convert*/

/*******************************************************************************
* digi_serdes_c8_rate_convert
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Converts the enum type digi_serdes_port_mode_t for C8 SERDES rates
*  to the hostmsg_c8_serdes_rate_t type for sending a firmware host message.
*
* INPUTS:
*  serdes_rate   - SERDES rate to convert
*
* OUTPUTS:
*   hostmsg_c8_serdes_rate_ptr - Pointer to converted SERDES rate
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_c8_rate_convert(digi_serdes_port_mode_t     serdes_rate, 
                                             hostmsg_c8_serdes_rate_t   *hostmsg_c8_serdes_rate_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;        
    
    PMC_ENTRY();

    PMC_ASSERT(NULL != hostmsg_c8_serdes_rate_ptr, DIGI_ERR_INVALID_ARG, 0, 0);

    /* Convert SERDES rate to host message parameters */
    switch (serdes_rate) {
    case DIGI_SERDES_PORT_MODE_C8_6G25:
    {
        *hostmsg_c8_serdes_rate_ptr = HOSTMSG_C8_SERDES_RATE_6G25;
        break;
    }
    case DIGI_SERDES_PORT_MODE_C8_3G125:
    {
        *hostmsg_c8_serdes_rate_ptr = HOSTMSG_C8_SERDES_RATE_3G125;
        break;
    }
    default:
    {
        *hostmsg_c8_serdes_rate_ptr =  HOSTMSG_C8_SERDES_RATE_MAX_NUM;
        break;
    }
    } /* end switch */

    PMC_RETURN(rc);
}/*digi_serdes_c8_rate_convert*/

/*******************************************************************************
* digi_serdes_sfi51_d8_rate_convert
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Converts the enum type digi_serdes_port_mode_t for SFI51 D8 SERDES rates
*  to the hostmsg_sfi51_serdes_rate_t type for sending a firmware host message.
*
* INPUTS:
*  serdes_rate   - SERDES rate to convert
*
* OUTPUTS:
*   hostmsg_sfi51_d8_serdes_rate_ptr - Pointer to converted SERDES rate
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_sfi51_d8_rate_convert(digi_serdes_port_mode_t       serdes_rate, 
                                                   hostmsg_sfi51_serdes_rate_t  *hostmsg_sfi51_d8_serdes_rate_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;        
    
    PMC_ENTRY();

    PMC_ASSERT(NULL != hostmsg_sfi51_d8_serdes_rate_ptr, DIGI_ERR_INVALID_ARG, 0, 0);

    switch (serdes_rate) {
    case DIGI_SERDES_PORT_MODE_ANY_LOWPWR:
    {
        /* Host message for low power ignores the rate */
        *hostmsg_sfi51_d8_serdes_rate_ptr = (hostmsg_sfi51_serdes_rate_t) 0;
        break;
    }
    case DIGI_SERDES_PORT_MODE_D8_OC_768:
    {
        *hostmsg_sfi51_d8_serdes_rate_ptr = HOSTMSG_SFI51_SERDES_RATE_OC768_STM256;
        break;
    }
    case DIGI_SERDES_PORT_MODE_D8_OTU3:
    {
        *hostmsg_sfi51_d8_serdes_rate_ptr = HOSTMSG_SFI51_SERDES_RATE_OTU3;
        break;
    }
    case DIGI_SERDES_PORT_MODE_D8_OTU3E2:
    {
        *hostmsg_sfi51_d8_serdes_rate_ptr = HOSTMSG_SFI51_SERDES_RATE_OTU3E2;
        break;
    }
    case DIGI_SERDES_PORT_MODE_D8_OTU3E1:
    {
        *hostmsg_sfi51_d8_serdes_rate_ptr = HOSTMSG_SFI51_SERDES_RATE_OTU3E1;
        break;
    }
    default:
    {
        rc = HOSTMSG_ERR_INVALID_PARAMETERS;
        break;
    }
    } /* end switch */

    PMC_RETURN(rc);
}/*digi_serdes_sfi51_d8_rate_convert*/


/*******************************************************************************
* digi_serdes_port_prov_state_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves the SERDES port provision state.
*
* INPUTS:
*  *digi_handle     - pointer to DIGI handle instance.
*   intf            - Enum, the interface type. See digi_serdes_intf_t.
*  port_num         - SERDES port number
*
* OUTPUTS:
*  *serdes_port_prov_state          - the SERDES port provision state
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_serdes_port_prov_state_get(digi_handle_t *digi_handle,
                                            digi_serdes_intf_t intf,
                                            UINT8 port_num,
                                            digi_serdes_prov_state_t *serdes_port_prov_state)
                                                  
                                                 
{
    PMC_ENTRY();

    switch (intf) 
    {
        case DIGI_SERDES_LINE_INTF:
            PMC_ASSERT(port_num < DIGI_SERDES_XFI_LANES_MAX, DIGI_ERR_INVALID_ARG, port_num, 0);            
            *serdes_port_prov_state = digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[port_num].serdes_rate_state.port_prov_state;
            break;
        case DIGI_SERDES_SYSTEM_INTF:
            PMC_ASSERT(port_num < DIGI_SERDES_SYS_LANES_MAX, DIGI_ERR_INVALID_ARG, port_num, 0); 
            *serdes_port_prov_state = digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[port_num].serdes_rate_state.port_prov_state;
            break;
        case DIGI_SERDES_SFI_51_INTF:
            PMC_ASSERT(port_num < DIGI_NUM_SFI51_PORTS_MAX, DIGI_ERR_INVALID_ARG, port_num, 0); 
            *serdes_port_prov_state = digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[port_num].serdes_rate_state.port_prov_state;
            break;
        default:
            break;
    }

    PMC_RETURN();
} /* digi_serdes_port_prov_state_get */

/*******************************************************************************
* digi_serdes_port_prov_state_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves the SERDES port lane number given the connected subsystem lane number.
*
* INPUTS:
*  *digi_handle     - pointer to DIGI handle instance.
*   intf            - Enum, the interface type. See digi_serdes_intf_t.
*  module_lane      - subsystem lane number
*
* OUTPUTS:
*   SYS SERDES lane index 
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 digi_serdes_line_and_sys_port_lane_index_for_module_lane_get(digi_handle_t *digi_handle,
                                                                           digi_serdes_intf_t intf,
                                                                           UINT8 module_lane)
                                                  
                                                 
{
    PMC_ENTRY();

    UINT32 lane_index = 0xFF;
    UINT32 i = 0;

    switch (intf) 
    {
        case DIGI_SERDES_LINE_INTF:
            PMC_ASSERT(module_lane < DIGI_SERDES_XFI_LANES_MAX, DIGI_ERR_INVALID_ARG, module_lane, 0);            
            for (i = 0; i < DIGI_SERDES_XFI_LANES_MAX; i++)
            {
                if (digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].cage_id != 0xFF
                    && digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].module_lane_tx_index == module_lane
                    && digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].module_lane_rx_index == module_lane)
                {
                    lane_index = i;
                }
            }
            break;
        case DIGI_SERDES_SYSTEM_INTF:
            PMC_ASSERT(module_lane < DIGI_SERDES_SYS_LANES_MAX, DIGI_ERR_INVALID_ARG, module_lane, 0); 

            for (i = 0; i < DIGI_SERDES_SYS_LANES_MAX; i++)
            {
                if (digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].cage_id != 0xFF
                    && digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].module_lane_tx_index == module_lane
                    && digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].module_lane_rx_index == module_lane)
                {
                    lane_index = i;
                }
            }
            break;
        case DIGI_SERDES_SFI_51_INTF:
        default:
            break;
    }

    PMC_RETURN(lane_index);
} /* digi_serdes_line_and_sys_port_lane_index_for_module_lane_get */

/*
** Private Functions
*/

/*******************************************************************************
* digi_serdes_port_intf_type_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The function returns the interface type given the unique port id.
*   
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   port_uid           - the unique identifier.
*
* OUTPUTS:
*  *intf_type          - the interface type.
*
* RETURNS:
*   PMC_SUCCESS        - when this call is successful otherwise a descriptive 
*                        error is returned.
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_port_intf_type_get(digi_handle_t *digi_handle,
                                                 UINT32 port_uid,
                                                 digi_serdes_intf_t *intf_type)
{

    PMC_ERROR result = DIGI_ERR_INVALID_ARG;
    UINT32 i = 0;

    PMC_ENTRY();
    
    for (i = 0; i < DIGI_SERDES_XFI_LANES_MAX; i++) {

        if (digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].port_uid == port_uid) {

            *intf_type = DIGI_SERDES_LINE_INTF;
            result = PMC_SUCCESS;
        }
    }

    if (result != PMC_SUCCESS) {

        for (i = 0; i < DIGI_SERDES_SYS_LANES_MAX; i++) {        

            if (digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].port_uid == port_uid)
            {
                *intf_type = DIGI_SERDES_SYSTEM_INTF;
                result = PMC_SUCCESS;
            }
        }
    }

    if (result != PMC_SUCCESS) {

        for (i = 0; i < DIGI_NUM_SFI51_PORTS_MAX; i++) {

            if (digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].port_uid == port_uid)
            {
                *intf_type = DIGI_SERDES_SFI_51_INTF;
                result = PMC_SUCCESS;
            }
        }
    }

    PMC_RETURN(result);
} /* digi_serdes_port_intf_type_get */


/*******************************************************************************
* digi_serdes_port_lifd_xfi_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a helper function to disable the lifd xfi subsytem lane based 
*   on the port_uid.
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   port_uid           - the unique identifier of the SERDES port.
*
* OUTPUTS:
*
* RETURNS:
*   DIGI_ERR_UNKNOWN on failure, PMC_SUCCESS otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_port_lifd_xfi_disable(digi_handle_t *digi_handle,
                                                    UINT32 port_uid)
{

    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    UINT8 i = 0; /* counter */
    PMC_ENTRY();

    for (i = 0; i < DIGI_SERDES_DSI_LANES_MAX; i ++) {

        /* Check the line side first */
        if (digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].port_uid == port_uid) {

            rc = lifd_serdes_subsystem_disable(digi_handle->lifd_handle, 
                                               i,
                                               digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].post_mux_dsi_lane);

            if (rc != PMC_SUCCESS) {

                break;
            }
        }
    }

    PMC_RETURN(rc);
} /* digi_serdes_port_lifd_xfi_disable */

/*******************************************************************************
* digi_serdes_port_sifd_xfi_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a helper function to disable the sifd xfi subsytem lane based 
*   on the port_uid.
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   port_uid           - the unique identifier of the SERDES port.
*
* OUTPUTS:
*
* RETURNS:
*   DIGI_ERR_UNKNOWN on failure, PMC_SUCCESS otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_port_sifd_xfi_disable(digi_handle_t *digi_handle,
                                                    UINT32 port_uid)
{

    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    UINT8 i = 0; /* counter */
    PMC_ENTRY();

    for (i = 0; i < DIGI_SERDES_DSI_LANES_MAX; i ++) {
        /* Check the system side second */
        if (digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].port_uid == port_uid) {

            rc = sifd_serdes_subsystem_disable(digi_handle->sifd_handle, 
                                               i, 
                                               digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].post_mux_dsi_lane);

            if (rc != PMC_SUCCESS) {

                break;
            }
        }
    }

    PMC_RETURN(rc);
} /* digi_serdes_port_sifd_xfi_disable */

/*******************************************************************************
* digi_serdes_port_lifd_sfi51_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a helper function to disable the lifd sfi51 interface based
*   on the port_uid.
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   port_uid           - the unique identifier of the SERDES port.
*
* OUTPUTS:
*
* RETURNS:
*   DIGI_ERR_UNKNOWN on failure, PMC_SUCCESS otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_port_lifd_sfi51_disable(digi_handle_t *digi_handle,
                                                      UINT32 port_uid)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;

    lifd_sfi5point1_int_t sfi51_intf = LIFD_SFI5POINT1_NONE;
    BOOL is_sfi_one_and_two_swapped = FALSE;
    lifd_rxs_cfg_t rxs_config;
    PMC_ENTRY();

    if (digi_serdes_sfi51_info_get(digi_handle, port_uid, 
                                   &sfi51_intf, &is_sfi_one_and_two_swapped, &rxs_config) != PMC_SUCCESS) {

        /* This is a no-op it should assert in the calling function */

    } else {

        rc = lifd_sfi_subsystem_select(digi_handle->lifd_handle, 
                                       sfi51_intf,
                                       is_sfi_one_and_two_swapped,
                                       XIFD_DSIS_NONE,
                                       FALSE,
                                       rxs_config);
    }

    PMC_RETURN(rc);
} /* digi_serdes_port_lifd_sfi51_disable */



/*******************************************************************************
* digi_port_pin_bit_mask_s16_clk_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a helper function to search through a pin context and return
*   a bit mask representing all the pins belonging to a givin port.
*   It also add to the mask the additional clock when it is configured 
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   *port_def_ptr       - a pointer to the port to find the pin usage on.
*
* OUTPUTS:
*   *bit_mask           - the bit mask of the pins used by this port.
*
* RETURNS:
*   PMC_SUCCESS on success, or a specific error code otherwise.
*
* NOTES:
*   bit_mask is only valid upon PMC_SUCCESS.
*
*******************************************************************************/
PRIVATE UINT32 digi_port_pin_bit_mask_s16_clk_get(digi_handle_t* digi_handle,
                                                  digi_serdes_port_def_t* port_def_ptr,
                                                  UINT16* bit_mask)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    digi_serdes_intf_t intf = port_def_ptr->intf;
    
    PMC_ENTRY();

    /* get date/deskew pins */
    rc = digi_port_pin_bit_mask_get(digi_handle, port_def_ptr,bit_mask);
    
    if (PMC_SUCCESS == rc &&  
        DIGI_SERDES_SFI_51_INTF != intf && 
        DIGI_SERDES_PIN_NOT_USED != port_def_ptr->s16_clk_cfg.pin_index)
    {
        *bit_mask = (*bit_mask | (1 <<  port_def_ptr->s16_clk_cfg.pin_index));
    }
    
    PMC_RETURN(rc);
} /* digi_port_pin_bit_mask_s16_clk_get */

/*******************************************************************************
* digi_port_pin_bit_mask_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a helper function to search through a pin context and return
*   a bit mask representing all the pins belonging to a givin port.
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   *port_def_ptr       - a pointer to the port to find the pin usage on.
*
* OUTPUTS:
*   *bit_mask           - the bit mask of the pins used by this port.
*
* RETURNS:
*   PMC_SUCCESS on success, or a specific error code otherwise.
*
* NOTES:
*   bit_mask is only valid upon PMC_SUCCESS.
*
*******************************************************************************/
PUBLIC UINT32 digi_port_pin_bit_mask_get(digi_handle_t* digi_handle,
                                          digi_serdes_port_def_t* port_def_ptr,
                                          UINT16* bit_mask)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    UINT8 i; /* counter */   
    digi_serdes_intf_t intf = port_def_ptr->intf;
    UINT32 port_uid = port_def_ptr->port_uid;
    PMC_ENTRY();

    *bit_mask = 0;

    /* handle each interface type seperately */
    switch(intf)
    {
    case DIGI_SERDES_LINE_INTF:
        for(i = 0; i < DIGI_SERDES_XFI_LANES_MAX; i++) {

            if(digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].port_uid == port_uid) {
                *bit_mask = (*bit_mask | (1 << i));
            }
        }
        rc = PMC_SUCCESS;
        break;
    case DIGI_SERDES_SYSTEM_INTF:
        for(i = 0; i < DIGI_SERDES_SYS_LANES_MAX; i++) {
            if(digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].port_uid == port_uid) {
                *bit_mask = (*bit_mask | (1 << i));
            }
        }
        rc = PMC_SUCCESS;
        break;
    case DIGI_SERDES_SFI_51_INTF:
        for (i = 0; i < DIGI_NUM_SFI51_PORTS_MAX; i++) {
            if(digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].port_uid == port_uid) {
                *bit_mask = (*bit_mask | (1 << i));
            }
        }
        rc = PMC_SUCCESS;
        break;
    default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    }

    PMC_RETURN(rc);
} /* digi_port_pin_bit_mask_get */
/*******************************************************************************
* digi_min_post_mux_lane_find
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function finds the minimum post mux lane in a given port.
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   *port_def_ptr       - pointer to the port in which to find the minimum.
*
* OUTPUTS:
*   *min                - The minimum post_mux_lane in this port.
*
* RETURNS:
*   PMC_SUCCESS on success, or a specific error code otherwise.
*
* NOTES:
*   
*******************************************************************************/
PRIVATE UINT32 digi_min_post_mux_lane_find(digi_handle_t* digi_handle,
                                           digi_serdes_port_def_t* port_def_ptr,
                                           UINT32 * min)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    UINT8 i; /* counter */   
    digi_serdes_intf_t intf = port_def_ptr->intf;
    UINT32 port_uid = port_def_ptr->port_uid;
    PMC_ENTRY();

    *min = 0xFF;

    /* handle each interface type seperately */
    switch(intf)
    {
    case DIGI_SERDES_LINE_INTF:
        for(i = 0; i < DIGI_SERDES_XFI_LANES_MAX; i++) 
        {

            if(digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].post_mux_dsi_lane < *min &&
               digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].port_uid == port_uid) 
            {
                *min = digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].post_mux_dsi_lane;
            }
        }
        rc = PMC_SUCCESS;
        break;
    case DIGI_SERDES_SYSTEM_INTF:
        for(i = 0; i < DIGI_SERDES_XFI_LANES_MAX; i++) 
        {
            if(digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].post_mux_dsi_lane < *min &&
               digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].port_uid == port_uid) 
            {
                *min = digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].post_mux_dsi_lane;
            }
        }
        rc = PMC_SUCCESS;
        break;

    case DIGI_SERDES_SFI_51_INTF:
        break;
    default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    }

    PMC_RETURN(rc);
} /* */

/*******************************************************************************
* digi_line_sys_serdes_pin_id_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The function returns serdes pin id associated to a given logical lane associated to
*  line, system or SFI51 interface.
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   *port_ctxt_ptr    - serdes port context pointer.
*   logical_lane      - For line and system interface,
*                       it is the logical lane associated to the serdes.
*                        For SFI51 interface, it is the serdes pin number.
*   direction         - serdes datapath direction: TRUE: RX, FALSE, TX.
*
* OUTPUTS:
*  *pin               - serdes pin id associated to logical lane.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_line_sys_serdes_pin_id_get(digi_handle_t *digi_handle,
                                                  digi_serdes_port_t *port_ctxt_ptr,
                                                  UINT8 logical_lane,
                                                  BOOL8 direction, 
                                                  UINT8 *pin)
{
    
    digi_serdes_port_def_t *port_def_ptr; 
    UINT8 lane_index;
    UINT32 i;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* cast serdes port context */
    port_def_ptr = (digi_serdes_port_def_t *)port_ctxt_ptr;    

       
    switch (port_def_ptr->intf)
    {
    case DIGI_SERDES_LINE_INTF:
        /* build enable mask */
        for (i = 0; i < DIGI_SERDES_XFI_LANES_MAX; i++)
        {
          
            if ( digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].port_uid == port_def_ptr->port_uid)
            {
                if (direction == TRUE)
                {
                    lane_index = digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].module_lane_rx_index;         
                }
                else
                {
                    lane_index = digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].module_lane_tx_index;                
                }
        
                if (lane_index == logical_lane)
                {
                    *pin = i;
                    PMC_RETURN(PMC_SUCCESS);
                }        
            }
        } 

        result = DIGI_ERR_SERDES_PORT_NOT_PROV;

        break;
    case DIGI_SERDES_SYSTEM_INTF:
        /* search pin ID */
        for (i = 0; i < DIGI_SERDES_SYS_LANES_MAX; i++)
        {
            if ( digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].port_uid == port_def_ptr->port_uid)
            {
                if (direction == TRUE)
                {
                    lane_index = digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].module_lane_rx_index;         
                }
                else
                {
                    lane_index = digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].module_lane_tx_index;                
                }
                if (lane_index == logical_lane)
                {
                    *pin = i;
                    PMC_RETURN(PMC_SUCCESS);
                }        
            }
        }
        result = DIGI_ERR_SERDES_PORT_NOT_PROV;
        break;
    case DIGI_SERDES_SFI_51_INTF:
        *pin = logical_lane;
        break;
    default:
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
        break;
    }

    PMC_RETURN(result);
}/*digi_line_sys_serdes_pin_id_get*/


/*******************************************************************************
* digi_serdes_sfi51_serdes_pin_id_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The function compute SFI51 block id and serdes id associated to a pin id.
*   
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   *port_ctxt_ptr    - serdes port context pointer.
*   pin_id            - serdes pin id (0-16)
*   direction         - serdes direction in datapath: RX: TRUE, TX: FALSE
*
* OUTPUTS:
*  *sfi51_id          - SFI51 block id: 0 or 1
*  *serdes_id         - serdes identifier in SFI51 block: 0 to 16
*
* RETURNS:
*   PMC_SUCCESS       - when operation execution is successful otherwise a descriptive 
*                       error is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_sfi51_serdes_pin_id_get(digi_handle_t *digi_handle,
                                                      digi_serdes_port_t *port_ctxt_ptr,
                                                      UINT8 pin_id,
                                                      BOOL8 direction,
                                                      UINT8 *sfi51_id,
                                                      UINT8 *serdes_id)
{    
    digi_serdes_port_def_t *port_def_ptr; 
    const UINT8 sfi51_rx_serdes_pair[SFI51_D8_RX_18X_MAX_NUM_SERDES] = DIGI_SFI51_RX_SERDES_PAIR_LIST;
    const UINT8 sfi51_tx_serdes_pair[SFI51_D8_TX_MAX_NUM_SERDES] = DIGI_SFI51_TX_SERDES_PAIR_LIST;
    UINT32 i;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(sfi51_id != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(serdes_id != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* cast serdes port context */
    port_def_ptr = (digi_serdes_port_def_t *)port_ctxt_ptr;   

    /* check that pin_id is in range [0..17]*/
    if (SFI51_D8_TX_MAX_NUM_SERDES <= pin_id && FALSE == direction)
    {
        result = DIGI_ERR_INVALID_ARG;
    }

    if (SFI51_D8_RX_18X_MAX_NUM_SERDES <= pin_id && TRUE == direction)
    {
        result = DIGI_ERR_INVALID_ARG;
    }

    /* search pin ID */
    for (i = 0; i < DIGI_NUM_SFI51_PORTS_MAX && PMC_SUCCESS == result; i++)
    {
        if ( digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].port_uid == port_def_ptr->port_uid)
        {
            *sfi51_id = i;
            
            /* lookup serdes instance ID */
            if (TRUE == direction)
            {
                if (SFI51_D8_RX_18X_MAX_NUM_SERDES > sfi51_rx_serdes_pair[pin_id])
                {
                    *serdes_id = sfi51_rx_serdes_pair[pin_id];
                }
                else
                {
                    result = DIGI_ERR_INVALID_ARG;
                }
            }
            else
            {
                if (SFI51_D8_TX_MAX_NUM_SERDES > pin_id)
                {
                    *serdes_id = sfi51_tx_serdes_pair[pin_id];
                }
            }

            result = PMC_SUCCESS;        
            break;
        }
    }

    PMC_RETURN(result);
}/* digi_serdes_sfi51_serdes_pin_id_get */


/*******************************************************************************
* digi_serdes_line_sys_port_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The function returns the port rate of on serdes belonging to line, system or 
*  SFI51 interface.
*   
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   *port_ctxt_ptr     - serdes port context pointer.
*
* OUTPUTS:
*  *port_mode          - address where serdes port mode is copied.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_line_sys_port_mode_get(digi_handle_t *digi_handle,
                                                     digi_serdes_port_t *port_ctxt_ptr,
                                                     digi_serdes_port_mode_t *port_mode)
{

    digi_serdes_port_def_t *port_def_ptr = NULL;
    UINT32 i = 0;
    BOOL8 found = FALSE;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_mode != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);


    /* check that serdes type is associated with a cage */
    /* and get port_uid to retrieve serdes prov state and get port_mode */
    port_def_ptr = (digi_serdes_port_def_t *)port_ctxt_ptr;
    
    switch (port_def_ptr->intf)
    {
    case DIGI_SERDES_LINE_INTF:    
        while (TRUE != found && i < DIGI_SERDES_XFI_LANES_MAX)
        {    
            if (digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].port_uid == port_def_ptr->port_uid)
            {
                *port_mode = digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].serdes_rate_state.port_mode;
                found = TRUE;
            }
            i++;
        }
        break;
    case DIGI_SERDES_SYSTEM_INTF:
        while (TRUE != found && i < DIGI_SERDES_SYS_LANES_MAX)
        {        
            if (digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].port_uid == port_def_ptr->port_uid)
            {
                *port_mode = digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].serdes_rate_state.port_mode;
                found = TRUE;
            }
            i++;
        }
        break;
    case DIGI_SERDES_SFI_51_INTF:
        while (TRUE != found && i < DIGI_NUM_SFI51_PORTS_MAX)
        {
            if (digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].port_uid == port_def_ptr->port_uid)
            {
                *port_mode = digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].serdes_rate_state.port_mode;
                found = TRUE;
            }
            i++;
        }
        break;
    default:
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
        break;
    }

    PMC_RETURN(PMC_SUCCESS);
    
}/* digi_serdes_line_sys_port_mode_get */

/*******************************************************************************
* digi_serdes_line_sys_patt_mon_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The function configures pattern monitor of one serdes in line, system or
*  SFI51 interface.
*   
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   *port_ctxt_ptr     - serdes port context pointer.
*   logical_lane       - For line and system interface,
*                        it is the logical lane associated to the serdes.
*                        For SFI51 interface, it is the serdes pin number. 
*   is_resync          - When TRUE, function executes monitor resynchronization
*                        and patt_mon is not configured when TRUE.
*   *patt_mon_cfg_ptr  - pointer to a pattern monitor configuration.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_line_sys_patt_mon_cfg(digi_handle_t *digi_handle,
                                                    digi_serdes_port_t *port_ctxt_ptr,
                                                    UINT8 logical_lane,
                                                    BOOL8 is_resync,
                                                    util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr)
{
    digi_serdes_port_def_t *port_def_ptr; 
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 pin;
    UINT8 sfi51_id;
    UINT8 serdes_id;
    s16_port_mode_t s16_port_mode = LAST_S16_PORT_MODE;
    digi_serdes_port_mode_t port_mode;
    BOOL8 prov_status;
    digi_serdes_prov_state_t prov_state;
    digi_serdes_port_mode_t tmp_port_mode;
    hostmsg_s16_serdes_rate_t s16_serdes_rate_dummy;
    DOUBLE s16_lane_serdes_rate_dummy;
    BOOL8 s16_otu_client_dummy;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(TRUE == is_resync || patt_mon_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    if (FALSE == is_resync && TRUE == patt_mon_cfg_ptr->patt_cfg.enable)
    {        
        /* get serdes provioning state */
        result = digi_serdes_port_rate_prov_status_internal_get(digi_handle, port_ctxt_ptr, 
                                                                TRUE,
                                                                &prov_status, &tmp_port_mode, &prov_state);
        if(PMC_SUCCESS == result && FALSE == prov_status)
        {
            result = DIGI_ERR_SERDES_PORT_NOT_PROV;
        }
    }

    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    result = digi_line_sys_serdes_pin_id_get(digi_handle, port_ctxt_ptr, logical_lane, TRUE, &pin);

    if (PMC_SUCCESS == result)
    {
    
        /* cast serdes port context */
        port_def_ptr = (digi_serdes_port_def_t *)port_ctxt_ptr;    

        /* configure appropriate serdes according to interface type */    
        switch (port_def_ptr->intf)
        {
        case DIGI_SERDES_LINE_INTF:
            if (FALSE == is_resync && TRUE == patt_mon_cfg_ptr->patt_cfg.enable)
            {
                /* get serdes provioning state */
                result = digi_serdes_line_sys_port_mode_get(digi_handle, port_ctxt_ptr, 
                                                            &port_mode);
                if (PMC_SUCCESS != result)
                {
                    PMC_RETURN(result);
                }
                
                result = digi_serdes_s16_rate_convert(port_mode,  &s16_serdes_rate_dummy ,&s16_lane_serdes_rate_dummy, &s16_otu_client_dummy, &s16_port_mode);
                
                if (PMC_SUCCESS != result)
                {
                    PMC_RETURN(result);
                }
            }
            if (FALSE == is_resync)
            {
                result = s16_patt_mon_cfg(digi_handle->s16_line_handle, pin, s16_port_mode, patt_mon_cfg_ptr);
            }
            else
            {
                result = s16_patt_mon_resync(digi_handle->s16_line_handle, pin);
            }
            break;
        case DIGI_SERDES_SYSTEM_INTF:
            if (FALSE == is_resync && TRUE == patt_mon_cfg_ptr->patt_cfg.enable)
            {
                /* get serdes provioning state */
                result = digi_serdes_line_sys_port_mode_get(digi_handle, port_ctxt_ptr, 
                                                            &port_mode);
                if (PMC_SUCCESS != result)
                {
                    PMC_RETURN(result);
                }
                
                result = digi_serdes_s16_rate_convert(port_mode,  &s16_serdes_rate_dummy ,&s16_lane_serdes_rate_dummy, &s16_otu_client_dummy, &s16_port_mode);
                
                if (PMC_SUCCESS != result)
                {
                    PMC_RETURN(result);
                }
            }
            if (FALSE == is_resync)
            {
                result = s16_patt_mon_cfg(digi_handle->s16_sys_handle, pin,s16_port_mode,  patt_mon_cfg_ptr);
            }
            else
            {
                result = s16_patt_mon_resync(digi_handle->s16_sys_handle, pin);
            }
            break;
        case DIGI_SERDES_SFI_51_INTF:      
            result = digi_serdes_sfi51_serdes_pin_id_get(digi_handle, 
                                                         port_ctxt_ptr, pin, 
                                                         TRUE, &sfi51_id, &serdes_id);
            if (PMC_SUCCESS == result)
            {

                if (FALSE == is_resync)
                {
                    result = sfi51_d8_rx_18x_patt_mon_cfg(digi_handle->sfi51_d8_rx_18x_handle[sfi51_id],
                                                          serdes_id, 
                                                          patt_mon_cfg_ptr);
                }
                else
                {
                    result = sfi51_d8_rx_18x_patt_mon_resync(digi_handle->sfi51_d8_rx_18x_handle[sfi51_id],
                                                             serdes_id);
                    
                }
            }
            break;
        default:
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
            break;
        }    
    }
    
    PMC_RETURN(result);
}/*digi_serdes_line_sys_patt_mon_cfg*/

#ifdef PMC_DEPRECATED

/*******************************************************************************
* digi_serdes_line_sys_patt_all_ones_detection_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*  DEPRECATED-Function has been marked as deprecated starting in the 3.02 release
*  The function configures pattern monitor of one serdes in line, system or
*  SFI51 interface to detect all-ones or all-zeroes pattern.
*   
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   *port_ctxt_ptr     - serdes port context pointer.
*   logical_lane       - For line and system interface,
*                        it is the logical lane associated to the serdes.
*                        For SFI51 interface, it is the serdes pin number. 
*   *patt_mon_cfg_ptr  - pointer to a pattern monitor configuration.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
* NOTES:
*
*******************************************************************************/

/* LCOV_EXCL_START */

PRIVATE PMC_ERROR digi_serdes_line_sys_patt_all_ones_detection_cfg(digi_handle_t *digi_handle,
                                                                   digi_serdes_port_t *port_ctxt_ptr,
                                                                   UINT8 logical_lane,
                                                                   util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr)
{
    digi_serdes_port_def_t *port_def_ptr; 
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 pin;
    UINT8 sfi51_id;
    UINT8 serdes_id;
    BOOL8 prov_status;
    digi_serdes_prov_state_t prov_state;
    digi_serdes_port_mode_t tmp_port_mode;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
       
    /* get serdes provioning state */
    result = digi_serdes_port_rate_prov_status_internal_get(digi_handle, port_ctxt_ptr,
                                                            TRUE, 
                                                            &prov_status, 
                                                            &tmp_port_mode, 
                                                            &prov_state);

    if(PMC_SUCCESS == result && FALSE == prov_status)
    {
        result = DIGI_ERR_SERDES_PORT_NOT_PROV;
    }

    if (PMC_SUCCESS == result)
    {
        result = digi_line_sys_serdes_pin_id_get(digi_handle, port_ctxt_ptr, logical_lane, TRUE, &pin);
    }

    if (PMC_SUCCESS == result)
    {
    
        /* cast serdes port context */
        port_def_ptr = (digi_serdes_port_def_t *)port_ctxt_ptr;    

        /* configure appropriate serdes according to interface type */    
        switch (port_def_ptr->intf)
        {
        case DIGI_SERDES_LINE_INTF:            
            result = s16_all_ones_zeroes_detection_cfg(digi_handle->s16_line_handle, pin, patt_mon_cfg_ptr->patt_cfg.enable, TRUE == patt_mon_cfg_ptr->patt_cfg.inv_cfg ? FALSE : TRUE);
            break;
        case DIGI_SERDES_SYSTEM_INTF:         
            result = s16_all_ones_zeroes_detection_cfg(digi_handle->s16_sys_handle, pin, patt_mon_cfg_ptr->patt_cfg.enable, TRUE == patt_mon_cfg_ptr->patt_cfg.inv_cfg ? FALSE : TRUE);
            break;
        case DIGI_SERDES_SFI_51_INTF:      
            result = digi_serdes_sfi51_serdes_pin_id_get(digi_handle, 
                                                         port_ctxt_ptr, pin, 
                                                         TRUE, &sfi51_id, &serdes_id);
            if (PMC_SUCCESS == result)
            {
                result = sfi51_d8_rx_18x_patt_mon_resync(digi_handle->sfi51_d8_rx_18x_handle[sfi51_id],
                                                         serdes_id);
            }
            break;
        default:
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
            break;
        }    
    }
    
    PMC_RETURN(result);
}/*digi_serdes_line_sys_patt_all_ones_detection_cfg*/

/* LCOV_EXCL_STOP */

#endif /* PMC_DEPRECATED */

/*******************************************************************************
* digi_serdes_line_sys_patt_gen_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function configures (or disables) the pattern generator of line, system 
*   or SFI51 interface serdes.
*   
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   *port_ctxt_ptr     - serdes port context pointer.
*   logical_lane       - For line and system interface, it is the lane
*                        index associated to the serdes module.
*                        For SFI51 interface, it is the device pin index. 
*   *patt_gen_cfg_ptr  - pointer to a pattern generator configuration.
*   
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_line_sys_patt_gen_cfg(digi_handle_t *digi_handle,
                                                    digi_serdes_port_t *port_ctxt_ptr,
                                                    UINT8 logical_lane,
                                                    util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr)
{
    digi_serdes_port_def_t *port_def_ptr; 
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 pin;
    UINT8 sfi51_id;
    UINT8 serdes_id;
    s16_port_mode_t s16_port_mode = LAST_S16_PORT_MODE;
    BOOL8 prov_status;
    digi_serdes_prov_state_t prov_state;
    digi_serdes_port_mode_t port_mode;
    hostmsg_s16_serdes_rate_t s16_serdes_rate_dummy;
    DOUBLE s16_lane_serdes_rate_dummy;
    BOOL8 s16_otu_client_dummy;
    BOOL8 disable_fw_proc = FALSE;
    odu_struct_t *odu_ptr = NULL;
    util_global_switch_data_def_t *switch_data_ptr = NULL;
    hostmsg_opsa_subsystem_cfg_t subsystem = LAST_OPSA_HOSTMSG_SS;
    
    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(patt_gen_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    if (TRUE == patt_gen_cfg_ptr->patt_cfg.enable)
    {        
        /* get serdes provioning state */
        result = digi_serdes_port_rate_prov_status_internal_get(digi_handle, port_ctxt_ptr, 
                                                                TRUE,
                                                                &prov_status, &port_mode, &prov_state);
        if(PMC_SUCCESS != result)
        {
            PMC_RETURN(result);
        }

        if(FALSE == prov_status)
        {
            result = DIGI_ERR_SERDES_PORT_NOT_PROV;
        }
    }

    result = digi_line_sys_serdes_pin_id_get(digi_handle, port_ctxt_ptr, 
                                             logical_lane, FALSE, &pin);
    /* cast serdes port context */
    port_def_ptr = (digi_serdes_port_def_t *)port_ctxt_ptr;    
    disable_fw_proc = digi_disable_fw_acb_proc(digi_handle, 
                                               port_ctxt_ptr,
                                               &odu_ptr,
                                               &switch_data_ptr,
                                               &subsystem);
    if (TRUE == patt_gen_cfg_ptr->patt_cfg.enable && disable_fw_proc == TRUE)
    {
        if (NULL != odu_ptr)
        {
            digi_opsa_cfg_mpmo_one_port_drop(digi_handle,
                                             subsystem,
                                             odu_ptr->switch_data.oduk_data.channel);
        }
        else if (NULL != switch_data_ptr)
        {
            result = digi_cpb_update_nominal_replacement(digi_handle,
                                                         0,
                                                         switch_data_ptr,
                                                         DIGI_OPSA_CFG_ACTION_DEACTIVATE,
                                                         FALSE);
        }
        else
        {
            PMC_ASSERT(FALSE, DIGI_ERR_NULL_HANDLE, 0, 0);
        }
    }
    if (PMC_SUCCESS == result)
    {
    
        
        /* configure appropriate serdes according to interface type */    
        switch (port_def_ptr->intf)
        {
        case DIGI_SERDES_LINE_INTF:
            
            if (TRUE == patt_gen_cfg_ptr->patt_cfg.enable)
            {          
                result = digi_serdes_s16_rate_convert(port_mode,  &s16_serdes_rate_dummy ,&s16_lane_serdes_rate_dummy, &s16_otu_client_dummy, &s16_port_mode);
            }
            if (PMC_SUCCESS == result)
            {
                result = s16_patt_gen_cfg(digi_handle->s16_line_handle, pin, s16_port_mode,  patt_gen_cfg_ptr);
            }
            break;
        case DIGI_SERDES_SYSTEM_INTF:            
            if (TRUE == patt_gen_cfg_ptr->patt_cfg.enable)
            {       
                result = digi_serdes_s16_rate_convert(port_mode,  &s16_serdes_rate_dummy ,&s16_lane_serdes_rate_dummy, &s16_otu_client_dummy, &s16_port_mode);
            }
            if (PMC_SUCCESS == result)
            {
                result = s16_patt_gen_cfg(digi_handle->s16_sys_handle, pin, s16_port_mode,  patt_gen_cfg_ptr);
            }
            break;
        case DIGI_SERDES_SFI_51_INTF:

            result = digi_serdes_sfi51_serdes_pin_id_get(digi_handle, port_ctxt_ptr, pin, FALSE, &sfi51_id, &serdes_id); 
            if (PMC_SUCCESS == result)
            {
                result = sfi51_d8_tx_18x_patt_gen_cfg(digi_handle->sfi51_d8_tx_18x_handle[sfi51_id],
                                                      serdes_id,  
                                                      patt_gen_cfg_ptr);
            }
            break;
        default:
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
            break;
        }    
    }

    if (FALSE == patt_gen_cfg_ptr->patt_cfg.enable && disable_fw_proc == TRUE)
    {
        if (NULL != odu_ptr)
        {
            digi_otn_remove_nominal_replacement(digi_handle, odu_ptr);
        }
        else if (NULL != switch_data_ptr)
        {
            digi_serdes_port_def_t *serdes_ptr = (digi_serdes_port_def_t *)port_ctxt_ptr;
            digi_cpb_update_nominal_replacement(digi_handle,
                                                serdes_ptr->port_uid,
                                                switch_data_ptr,
                                                DIGI_OPSA_CFG_ACTION_ACTIVATE,
                                                FALSE);
        }    
    }

    PMC_RETURN(result);
}/*digi_serdes_line_sys_patt_gen_cfg*/


/*******************************************************************************
* digi_bp_serdes_used_pin_id_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The function finds pin identifier associated to ONLY one enabled serdes in
*  given SIFD configurations and also returns serdes type: T8 or C8.
*  The function returns a code error when no serdes is enabled or there are
*  more than one serdes enabled in SIFD configurations.
*   
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   *sifd_1_lane_cfg_ptr  - SIFD interlaken 1 lanes configuration pointer
*   *sifd_2_lane_cfg_ptr  - SIFD interlaken 2 lanes configuration pointer
*   
* OUTPUTS:
*  *serdes_type           - serdes type: T8, C8 or S16
*  *used_pin_id           - pin id of the enabled serdes
*  *serdes_rate           - serdes port rate
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_bp_serdes_used_pin_id_get(digi_handle_t *digi_handle,
                                                 digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg_ptr,
                                                 digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg_ptr,
                                                 digi_serdes_type_in_bp_t *serdes_type,
                                                 UINT8 *used_pin_id,
                                                 digi_ilkn_serdes_rate_t *serdes_rate)
{

    digi_sifd_ilkn_1_cfg_t sifd_1_cfg;
    digi_sifd_ilkn_2_cfg_t sifd_2_cfg;
    digi_sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr = NULL;
    digi_sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr = NULL;
    digi_ilkn_serdes_rate_t ilkn_1_rate = LAST_DIGI_ILKN_SERDES_RATE;
    digi_ilkn_serdes_rate_t ilkn_2_rate = LAST_DIGI_ILKN_SERDES_RATE;
    UINT32 ilkn1_e1_39_20_msk = 0;
    UINT32 ilkn1_e1_19_00_msk = 0;
    UINT32 ilkn1_n1_msk = 0;     
    UINT32 ilkn1_n2_msk = 0;
    UINT32 ilkn2_e1_39_20_msk = 0;
    UINT32 ilkn2_e1_19_00_msk = 0;
    UINT32 ilkn2_n1_msk = 0;     
    UINT32 ilkn2_n2_msk = 0;   


    UINT32 ilkn_e1_39_20_msk = 0;
    UINT32 ilkn_e1_19_00_msk = 0;
    UINT32 ilkn_n2_msk = 0;
    UINT32 ilkn_n1_msk = 0;
    UINT8 i;
    UINT32 nb_used_pins = 0;
    UINT32 mask;
    BOOL8 is_serdes_prov = FALSE;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(sifd_1_lane_cfg_ptr != NULL || sifd_2_lane_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != serdes_rate, DIGI_ERR_NULL_HANDLE, 0, 0);


    /* Check that associated serdes is provionned */ 
    if ( NULL != sifd_1_lane_cfg_ptr)
    {
        /* check that number os selected pin is equal to 1 */
        for (i = 0; i < 32 && 2 > nb_used_pins; i ++)
        {
            mask = 1 << i;
            if ((mask & sifd_1_lane_cfg_ptr->blk_to_E1_enable_mask) != 0)
            {
                nb_used_pins += 1;
            }           
            if ((mask & sifd_1_lane_cfg_ptr->blk_to_N1_enable_mask) != 0)
            {
                nb_used_pins += 1;
            }           
            if ((mask & sifd_1_lane_cfg_ptr->blk_to_N2_enable_mask) != 0)
            {
                nb_used_pins += 1;
            }            
        }

        if (2 > nb_used_pins)
        {
            /* make sure that at least one e1, n1, or n2 is enabled */
            if ((sifd_1_lane_cfg_ptr->blk_to_E1_enable_mask & 
                 digi_handle->var.sifd_ilkn_1_cfg.lane.blk_to_E1_enable_mask) != 0)
            {
                is_serdes_prov = TRUE;
            }
            
            /* make sure that at least one e1, n1, or n2 is enabled */
            if ((FALSE == is_serdes_prov) && 
                ((sifd_1_lane_cfg_ptr->blk_to_N1_enable_mask & 
                  digi_handle->var.sifd_ilkn_1_cfg.lane.blk_to_N1_enable_mask) != 0))
            {
                is_serdes_prov = TRUE;
            }
            
            /* make sure that at least one e1, n1, or n2 is enabled */
            if ((FALSE == is_serdes_prov) && 
                ((sifd_1_lane_cfg_ptr->blk_to_N2_enable_mask & digi_handle->var.sifd_ilkn_1_cfg.lane.blk_to_N2_enable_mask) != 0))
            {
                is_serdes_prov = TRUE;
            }        
        }
    }
    if ((2 > nb_used_pins) && NULL != sifd_2_lane_cfg_ptr)
    {
        /* check that number os selected pin is equal to 1 */
        for (i = 0; i < 32 && 2 > nb_used_pins; i ++)
        {
            mask = 1 << i;
            if ((mask & sifd_2_lane_cfg_ptr->blk_to_E1_enable_mask) != 0)
            {
                nb_used_pins += 1;
            }           
            if ((mask & sifd_2_lane_cfg_ptr->blk_to_N1_enable_mask) != 0)
            {
                nb_used_pins += 1;
            }           
            if ((mask & sifd_2_lane_cfg_ptr->blk_to_N2_enable_mask) != 0)
            {
                nb_used_pins += 1;
            }            
        }
        if ( 2 > nb_used_pins)
        {
            /* make sure that at least one e1, n1, or n2 is enabled */
            if ((FALSE == is_serdes_prov) && 
                ((sifd_2_lane_cfg_ptr->blk_to_E1_enable_mask & digi_handle->var.sifd_ilkn_2_cfg.lane.blk_to_E1_enable_mask) != 0))
            {
                is_serdes_prov = TRUE;
            }
            /* make sure that at least one e1, n1, or n2 is enabled */
            if ((FALSE == is_serdes_prov) && 
                ((sifd_2_lane_cfg_ptr->blk_to_N1_enable_mask & digi_handle->var.sifd_ilkn_2_cfg.lane.blk_to_N1_enable_mask) != 0))
            {
                is_serdes_prov = TRUE;
            }
            
            /* make sure that at least one e1, n1, or n2 is enabled */
            if ((FALSE == is_serdes_prov) && 
                ((sifd_2_lane_cfg_ptr->blk_to_N2_enable_mask & digi_handle->var.sifd_ilkn_2_cfg.lane.blk_to_N2_enable_mask) != 0))
            {
                is_serdes_prov = TRUE;
            }
        }
    }
    
    if (2 <= nb_used_pins)
    {
        PMC_RETURN(DIGI_ERR_MORE_THAN_ONE_PIN_SELECTED);
    }
    
    /* no need to go further if no serdes is provisionned */
    if ( FALSE == is_serdes_prov)
    {
        PMC_RETURN(DIGI_ERR_ILKN_CHNL_NOT_PROV);
    }

    /* prepare *digi_ilkn_serdes_pin_use_get function arguments */
    if( NULL != sifd_1_lane_cfg_ptr )
    {
        PMC_MEMCPY(&sifd_1_cfg.lane, sifd_1_lane_cfg_ptr, sizeof(digi_sifd_ilkn_1_lane_cfg_t));
        PMC_MEMCPY(&sifd_1_cfg.ilkn, &digi_handle->var.sifd_ilkn_1_cfg.ilkn, sizeof(digi_sifd_ilkn_cfg_t));
        sifd_1_cfg_ptr = &sifd_1_cfg;
    }
    if (NULL != sifd_2_lane_cfg_ptr)
    {
        PMC_MEMCPY(&sifd_2_cfg.lane, sifd_2_lane_cfg_ptr, sizeof(digi_sifd_ilkn_2_lane_cfg_t));   
        PMC_MEMCPY(&sifd_2_cfg.ilkn, &digi_handle->var.sifd_ilkn_2_cfg.ilkn, sizeof(digi_sifd_ilkn_cfg_t));
        sifd_2_cfg_ptr = &sifd_2_cfg;
    }


    /* The following code is inspired from digi_bp_intf_serdes_cfg function*/
    /* get ILKN serdes pin use */
    digi_ilkn_serdes_pin_use_get(digi_handle,
                                 sifd_1_cfg_ptr,
                                 sifd_2_cfg_ptr,
                                 &ilkn_1_rate,
                                 &ilkn_2_rate,
                                 &ilkn1_e1_39_20_msk,
                                 &ilkn1_e1_19_00_msk,
                                 &ilkn1_n1_msk,
                                 &ilkn1_n2_msk,
                                 &ilkn2_e1_39_20_msk,
                                 &ilkn2_e1_19_00_msk,
                                 &ilkn2_n1_msk,
                                 &ilkn2_n2_msk);


    /* initialize serdes rate */
    *serdes_rate = LAST_DIGI_ILKN_SERDES_RATE; 

    /* merge N1[15:0] masks and save rate */
    ilkn_n1_msk = ilkn1_n1_msk | (ilkn2_n1_msk >> 8);
    if (0 != ilkn1_n1_msk)
    {
        *serdes_rate = ilkn_1_rate;
    }
    if (LAST_DIGI_ILKN_SERDES_RATE == *serdes_rate && 0 != ilkn2_n1_msk)
    {
        *serdes_rate = ilkn_2_rate;
    }

    /* merge E1[39:20] masks and confirm the rate is the same */
    if(ilkn1_e1_39_20_msk != 0)
    {
        *serdes_rate = ilkn_1_rate;
        PMC_ASSERT(*serdes_rate!=LAST_DIGI_ILKN_SERDES_RATE, DIGI_ERR_INVALID_ARG, 0, 0);        
        ilkn_e1_39_20_msk = ilkn1_e1_39_20_msk;
    }

    if(ilkn2_e1_39_20_msk != 0)
    {
        if(*serdes_rate == LAST_DIGI_ILKN_SERDES_RATE)
        {
            *serdes_rate = ilkn_2_rate;
            PMC_ASSERT(*serdes_rate!=LAST_DIGI_ILKN_SERDES_RATE, DIGI_ERR_INVALID_ARG, 0, 0);        
        } else {
            PMC_ASSERT(*serdes_rate==ilkn_2_rate, DIGI_ERR_SERDES_CSU_MULTIRATE_REQUEST, 0, 0);
        }
        PMC_ASSERT(((ilkn_e1_39_20_msk&ilkn2_e1_39_20_msk) == 0), DIGI_ERR_SERDES_PIN_ALLOC_COLLISION, 0, 0);
        ilkn_e1_39_20_msk |= ilkn2_e1_39_20_msk;
    }

    /* merge E1[19:00] masks and confirm the rate is the same */
    if(ilkn1_e1_19_00_msk != 0)
    {
        *serdes_rate = ilkn_1_rate;
        PMC_ASSERT(*serdes_rate!=LAST_DIGI_ILKN_SERDES_RATE, DIGI_ERR_INVALID_ARG, 0, 0);        
        ilkn_e1_39_20_msk = ilkn1_e1_39_20_msk;
    }

    if(ilkn2_e1_19_00_msk != 0)
    {
        if(*serdes_rate == LAST_DIGI_ILKN_SERDES_RATE)
        {
            *serdes_rate = ilkn_2_rate;
            PMC_ASSERT(*serdes_rate!=LAST_DIGI_ILKN_SERDES_RATE, DIGI_ERR_INVALID_ARG, 0, 0);        
        } else {
            PMC_ASSERT(*serdes_rate==ilkn_2_rate, DIGI_ERR_SERDES_CSU_MULTIRATE_REQUEST, 0, 0);
        }
        PMC_ASSERT(((ilkn_e1_19_00_msk&ilkn2_e1_19_00_msk) == 0), DIGI_ERR_SERDES_PIN_ALLOC_COLLISION, 0, 0);
        ilkn_e1_19_00_msk |= ilkn2_e1_19_00_msk;
    }
    /* merge N2[07:00] masks and confirm the rate is the same */
    if(ilkn1_n2_msk != 0)
    {
        *serdes_rate = ilkn_1_rate;
        PMC_ASSERT(*serdes_rate!=LAST_DIGI_ILKN_SERDES_RATE, DIGI_ERR_INVALID_ARG, 0, 0);        
        ilkn_n2_msk = ilkn1_n2_msk;
    }

    if(ilkn2_n2_msk != 0)
    {
        if(*serdes_rate == LAST_DIGI_ILKN_SERDES_RATE)
        {
            *serdes_rate = ilkn_2_rate;
            PMC_ASSERT(*serdes_rate!=LAST_DIGI_ILKN_SERDES_RATE, DIGI_ERR_INVALID_ARG, 0, 0);        
        } else {
            PMC_ASSERT(*serdes_rate==ilkn_2_rate, DIGI_ERR_SERDES_CSU_MULTIRATE_REQUEST, 0, 0);
        }
        PMC_ASSERT(((ilkn_n2_msk&ilkn2_n2_msk) == 0), DIGI_ERR_SERDES_PIN_ALLOC_COLLISION, 0, 0);
        ilkn_n2_msk |= ilkn2_n2_msk;
    }


    nb_used_pins = 0;
    if(ilkn_e1_39_20_msk != 0 || ilkn_e1_19_00_msk != 0)
    {
        *serdes_type = DIGI_SERDES_TYPE_IN_BP_T8;
        /* T8 serdes */
        i = 0;
        while(i < (DIGI_SERDES_MAX_T8_SLICE) && nb_used_pins <= 2)
        {
            mask = 1 << i;
            if ((ilkn_e1_39_20_msk & mask) != 0)
            {
                nb_used_pins += 1;
                *used_pin_id = 20 + i;
            }
            i++;
        }
        i = 0;
        if (nb_used_pins < 1)
        {
            while(i < (DIGI_SERDES_MAX_T8_SLICE) && nb_used_pins <= 2)
            {
                mask = 1 << i;
                if ((ilkn_e1_19_00_msk & mask) != 0)
                {
                    nb_used_pins += 1;
                    *used_pin_id = i;
                }
                i++;
            }
        }
    }

    /* perform N2 configuration - C8 */
    if(0 == nb_used_pins && 0 != ilkn_n2_msk)
    {
        /* C8 serdes */
        *serdes_type = DIGI_SERDES_TYPE_IN_BP_C8;

        i = 0;
        while(i < DIGI_SERDES_MAX_C8_IN_BP_SR && nb_used_pins <= 2)
        {
            mask = 1 << i;
            if ((ilkn_n2_msk & mask) != 0)
            {
                nb_used_pins += 1;
                *used_pin_id = i;
            }
            i++;
        }
    }

    /* perform N2 configuration - C8 */
    if(0 == nb_used_pins && 0 != ilkn_n1_msk)
    {
        /* S16 serdes */
        *serdes_type = DIGI_SERDES_TYPE_IN_BP_S16;

        i = 0;
        while(i < DIGI_SERDES_MAX_S16_IN_BP && nb_used_pins <= 2)
        {
            mask = 1 << i;
            if ((ilkn_n1_msk & mask) != 0)
            {
                nb_used_pins += 1;
                *used_pin_id = i;
            }
            i++;
        }
    }

    /* return error when more than 1 serdes is used or none is used */
    if (nb_used_pins != 1)
    {
        result = DIGI_ERR_MORE_THAN_ONE_PIN_SELECTED;
    }

    PMC_RETURN(result);

}/*digi_bp_serdes_used_pin_id_get*/

/*******************************************************************************
* digi_serdes_bp_patt_mon_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The function configures pattern monitor of one serdes in backplane interface.
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   *sifd_1_lane_cfg_ptr  - SIFD interlaken 1 lanes configuration pointer.
*   *sifd_2_lane_cfg_ptr  - SIFD interlaken 2 lanes configuration pointer.
*   is_resync             - When TRUE, function executes monitor resynchronization
*                           and patt_mon is not configured when TRUE.
*   *patt_mon_cfg_ptr     - pointer to a pattern monitor configuration.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_bp_patt_mon_cfg(digi_handle_t *digi_handle,
                                              digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg_ptr,
                                              digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg_ptr,
                                              BOOL8 is_resync,
                                              util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr)
{
    UINT8 pin_id;
    UINT8 serdes_id;
    digi_serdes_type_in_bp_t serdes_type;
    digi_ilkn_serdes_rate_t ilkn_rate = LAST_DIGI_ILKN_SERDES_RATE;
    digi_serdes_port_mode_t serdes_n1_port_mode = LAST_DIGI_SERDES_PORT_MODE;  
    digi_serdes_port_mode_t port_mode_dummy     = LAST_DIGI_SERDES_PORT_MODE; 
    s16_port_mode_t s16_port_mode;
    PMC_ERROR result = PMC_SUCCESS;
    hostmsg_s16_serdes_rate_t s16_serdes_rate_dummy;
    DOUBLE s16_lane_serdes_rate_dummy;
    BOOL8 s16_otu_client_dummy;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0); 
    PMC_ASSERT(sifd_1_lane_cfg_ptr != NULL || sifd_2_lane_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(TRUE == is_resync || patt_mon_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    

    /* retrieve serdes id and serdes type */
    result = digi_bp_serdes_used_pin_id_get(digi_handle, sifd_1_lane_cfg_ptr, sifd_2_lane_cfg_ptr, &serdes_type, &pin_id, &ilkn_rate);

    if (PMC_SUCCESS == result)
    {
        switch(serdes_type)
        {
        case DIGI_SERDES_TYPE_IN_BP_T8:
            serdes_id = digi_serdes_map_t8_serdes(pin_id);
            if (FALSE == is_resync)
            {
                result = t8_patt_mon_cfg(digi_handle->bp_lr_t8_handle[DIGI_SERDES_T8_SLICE_ID_GET(pin_id)], 
                                         serdes_id, patt_mon_cfg_ptr);
            }
            else
            {
                result = t8_patt_mon_resync(digi_handle->bp_lr_t8_handle[DIGI_SERDES_T8_SLICE_ID_GET(pin_id)], 
                                            serdes_id);                
            }
            break;
        case DIGI_SERDES_TYPE_IN_BP_C8:
            if (FALSE == is_resync)
            {
                result = c8_patt_mon_cfg(digi_handle->bp_sr_c8_handle, 
                                         pin_id, patt_mon_cfg_ptr);        
            }
            else
            {
                result = c8_patt_mon_resync(digi_handle->bp_sr_c8_handle, 
                                            pin_id);                
            }
            break;
        case DIGI_SERDES_TYPE_IN_BP_S16:
            if (FALSE == is_resync)
            {
                PMC_ASSERT(ilkn_rate != LAST_DIGI_ILKN_SERDES_RATE, DIGI_ERR_INVALID_ARG, 0, 0);
                
                result = digi_serdes_ilkn_enum_conversion(digi_handle, 
                                                          ilkn_rate, 
                                                          &port_mode_dummy,
                                                          &serdes_n1_port_mode,
                                                          &port_mode_dummy);
                if(result == PMC_SUCCESS)
                {    
                    result = digi_serdes_s16_rate_convert(serdes_n1_port_mode,  &s16_serdes_rate_dummy ,&s16_lane_serdes_rate_dummy, &s16_otu_client_dummy, &s16_port_mode);
                    if ( PMC_SUCCESS == result)
                    {
                        result = s16_patt_mon_cfg(digi_handle->s16_sys_handle, pin_id, 
                                                  s16_port_mode, patt_mon_cfg_ptr);
                    }
                }
            }
            else
            {
                result = s16_patt_mon_resync(digi_handle->s16_sys_handle, pin_id);
            }
            break;
        default:
            result = DIGI_ERR_ILKN_CHNL_NOT_PROV;
            break;
        }
    }

    PMC_RETURN(result);
}/*digi_serdes_bp_patt_mon_cfg*/

/*******************************************************************************
* digi_serdes_bp_patt_gen_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The function configures pattern generator of one serdes in backplane interface.
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance
*   *sifd_1_lane_cfg_ptr  - SIFD interlaken 1 lanes configuration pointer
*   *sifd_2_lane_cfg_ptr  - SIFD interlaken 2 lanes configuration pointer
*   *patt_gen_cfg_ptr     - pointer to a pattern generator configuration
*   
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned. 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_bp_patt_gen_cfg(digi_handle_t *digi_handle,
                                              digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg_ptr,
                                              digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg_ptr,
                                              util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr)
{

    UINT8 pin_id;
    UINT8 serdes_id;
    digi_serdes_type_in_bp_t serdes_type;
    digi_ilkn_serdes_rate_t ilkn_rate = LAST_DIGI_ILKN_SERDES_RATE;
    digi_serdes_port_mode_t serdes_n1_port_mode = LAST_DIGI_SERDES_PORT_MODE;  
    digi_serdes_port_mode_t port_mode_dummy     = LAST_DIGI_SERDES_PORT_MODE; 
    s16_port_mode_t s16_port_mode;
    PMC_ERROR result = PMC_SUCCESS;
    hostmsg_s16_serdes_rate_t s16_serdes_rate_dummy;
    DOUBLE s16_lane_serdes_rate_dummy;
    BOOL8 s16_otu_client_dummy;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);  
    PMC_ASSERT(sifd_1_lane_cfg_ptr != NULL || sifd_2_lane_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(patt_gen_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* retireve serdes id and serdes type */
    result = digi_bp_serdes_used_pin_id_get(digi_handle, sifd_1_lane_cfg_ptr, 
                                            sifd_2_lane_cfg_ptr, &serdes_type,
                                            &pin_id, &ilkn_rate);

    if (PMC_SUCCESS == result)
    {
        switch(serdes_type)
        {
        case DIGI_SERDES_TYPE_IN_BP_T8:
            serdes_id = digi_serdes_map_t8_serdes(pin_id);
            result = t8_patt_gen_cfg(digi_handle->bp_lr_t8_handle[DIGI_SERDES_T8_SLICE_ID_GET(pin_id)], 
                                     serdes_id, patt_gen_cfg_ptr);
            break;
        case DIGI_SERDES_TYPE_IN_BP_C8:

            result = c8_patt_gen_cfg(digi_handle->bp_sr_c8_handle, 
                                     pin_id, patt_gen_cfg_ptr);
            break;
        case DIGI_SERDES_TYPE_IN_BP_S16:

            PMC_ASSERT(ilkn_rate != LAST_DIGI_ILKN_SERDES_RATE, DIGI_ERR_INVALID_ARG, 0, 0);

            result = digi_serdes_ilkn_enum_conversion(digi_handle, 
                                                      ilkn_rate, 
                                                      &port_mode_dummy,
                                                      &serdes_n1_port_mode,
                                                      &port_mode_dummy);

            if(result == PMC_SUCCESS)
            {
                result = digi_serdes_s16_rate_convert(serdes_n1_port_mode,  &s16_serdes_rate_dummy ,&s16_lane_serdes_rate_dummy, &s16_otu_client_dummy, &s16_port_mode);
                if ( PMC_SUCCESS == result)
                {
                    result = s16_patt_gen_cfg(digi_handle->s16_sys_handle, pin_id, s16_port_mode,  patt_gen_cfg_ptr);    
                }
            }

            break;
        default:
            result = DIGI_ERR_ILKN_CHNL_NOT_PROV;
            break;
        }
    }

    PMC_RETURN(result);
}/*digi_serdes_bp_patt_gen_cfg*/



/*******************************************************************************
* digi_serdes_ge_patt_mon_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The function configures pattern monitor of one GE serdes: OHP, RCP, EMGMT.
*   
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   ge_intf             - GE serdes interface type.
*   is_resync           - When TRUE, function executes monitor resynchronization
*                         and patt_mon is not configured when TRUE.
*   *patt_mon_cfg_ptr   - pointer to a pattern monitor configuration.
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned. 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_ge_patt_mon_cfg(digi_handle_t *digi_handle,
                                              digi_ge_serdes_id_t ge_intf,
                                              BOOL8 is_resync,
                                              util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr)
{
    d8_rx_2x_handle_t *d8_rx_2x_handle;
    UINT8 serdes_id;
    digi_ge_serdes_ctxt_t *ge_ctxt;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(TRUE == is_resync || patt_mon_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    result = digi_ge_serdes_ctxt_get(digi_handle, ge_intf, &ge_ctxt);
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* check serdes state */
    if ((FALSE == is_resync && patt_mon_cfg_ptr->patt_cfg.enable) && FALSE == ge_ctxt->enabled)
    {
        PMC_RETURN(DIGI_ERR_SERDES_PORT_NOT_PROV);
    } 
    else if ((FALSE == is_resync && TRUE == patt_mon_cfg_ptr->patt_cfg.enable) && TRUE == ge_ctxt->prbs_mon_enabled)
    {
        PMC_RETURN(DIGI_ERR_PRBS_ALREADY_PROV );
    }
    else if ((TRUE == is_resync || (FALSE == is_resync && FALSE == patt_mon_cfg_ptr->patt_cfg.enable)) && FALSE == ge_ctxt->prbs_mon_enabled)
    {
        PMC_RETURN(DIGI_ERR_PRBS_NOT_PROV);
    }
        
    /* retrieve d8_rx_2x handle associated to ge serdes */
    result = digi_ge_d8_rx_2x_handle_get(digi_handle, ge_intf, &d8_rx_2x_handle, &serdes_id);

    if(PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    if (FALSE == is_resync)
    {
        result =  d8_rx_2x_patt_mon_cfg(d8_rx_2x_handle,
                                        serdes_id,
                                        patt_mon_cfg_ptr);
        if (PMC_SUCCESS == result)
        {
            ge_ctxt->prbs_mon_enabled = patt_mon_cfg_ptr->patt_cfg.enable;
        }
    }
    else
    {
        result =  d8_rx_2x_patt_mon_resync(d8_rx_2x_handle,
                                           serdes_id);        
    }


    PMC_RETURN(result);
}/*digi_serdes_ge_patt_mon_cfg*/

/*******************************************************************************
* digi_serdes_ge_patt_mon_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The function gets config of pattern monitor of one GE serdes: OHP, RCP, EMGMT.
*   
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   ge_intf             - GE serdes interface type.
*   is_resync           - When TRUE, function executes monitor resynchronization
*                         and patt_mon is not configured when TRUE.
*   *patt_mon_cfg_ptr   - pointer to a pattern monitor configuration.
*      nhnbggggb h m/.,mbcx, 
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned. 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_ge_patt_mon_cfg_get(digi_handle_t *digi_handle,
                                                  digi_ge_serdes_id_t ge_intf,
                                                  BOOL8 is_resync,
                                                  util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr)
{
    d8_rx_2x_handle_t *d8_rx_2x_handle;
    UINT8 serdes_id;
    digi_ge_serdes_ctxt_t *ge_ctxt;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(TRUE == is_resync || patt_mon_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    result = digi_ge_serdes_ctxt_get(digi_handle, ge_intf, &ge_ctxt);
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    patt_mon_cfg_ptr->patt_cfg.enable = ge_ctxt->prbs_mon_enabled;
      
    /* retrieve d8_rx_2x handle associated to ge serdes */
    result = digi_ge_d8_rx_2x_handle_get(digi_handle, ge_intf, &d8_rx_2x_handle, &serdes_id);

    if(PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    if (FALSE == is_resync)
    {
        result =  d8_rx_2x_patt_mon_cfg_get(d8_rx_2x_handle,
                                            serdes_id,
                                            patt_mon_cfg_ptr);
        if (PMC_SUCCESS == result)
        {
            
        }
    }

    PMC_RETURN(result);
}/*digi_serdes_ge_patt_mon_cfg_get*/

/*******************************************************************************
* digi_serdes_ge_patt_gen_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The function configures pattern generator of one GE serdes: OHP, RCP, EMGMT. 
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance
*   ge_intf            - GE serdes interface type.
*   *patt_gen_cfg_ptr  - pointer to a pattern generator configuration
*   
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned. 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_ge_patt_gen_cfg(digi_handle_t *digi_handle,
                                              digi_ge_serdes_id_t ge_intf,
                                              util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr)
{
    d8_tx_2x_handle_t *d8_tx_2x_handle;
    UINT8 serdes_id;
    digi_ge_serdes_ctxt_t *ge_ctxt;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(patt_gen_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    result = digi_ge_serdes_ctxt_get(digi_handle, ge_intf, &ge_ctxt);
    
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* check serdes state */
    if ((TRUE == patt_gen_cfg_ptr->patt_cfg.enable) && FALSE == ge_ctxt->enabled)
    {
        PMC_RETURN(DIGI_ERR_SERDES_PORT_NOT_PROV);
    } 
    else if ((TRUE == patt_gen_cfg_ptr->patt_cfg.enable) && TRUE == ge_ctxt->prbs_gen_enabled)
    {
        PMC_RETURN(DIGI_ERR_PRBS_ALREADY_PROV );
    }
    else if ((FALSE == patt_gen_cfg_ptr->patt_cfg.enable)  && FALSE == ge_ctxt->prbs_gen_enabled)
    {
        PMC_RETURN(DIGI_ERR_PRBS_NOT_PROV);
    }    
    
    /* retrieve d8_tx_2x handle associated to ge serdes */
    result = digi_ge_d8_tx_2x_handle_get(digi_handle, ge_intf, &d8_tx_2x_handle, &serdes_id);
    
    if(PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    result =  d8_tx_2x_patt_gen_cfg(d8_tx_2x_handle,
                                    serdes_id,
                                    patt_gen_cfg_ptr);
    if (PMC_SUCCESS == result)
    {
        if (PMC_SUCCESS == result)
        {
            ge_ctxt->prbs_gen_enabled = patt_gen_cfg_ptr->patt_cfg.enable;
        }
    }

    PMC_RETURN(result);
}/*digi_serdes_ge_patt_gen_cfg*/

/*******************************************************************************
* digi_serdes_ge_patt_gen_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The function gets config of pattern generator of one GE serdes: OHP, RCP, EMGMT. 
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance
*   ge_intf            - GE serdes interface type.
*
*
* OUTPUTS:
*   *patt_gen_cfg_ptr  - pointer to a pattern generator configuration
*
* RETURNS:
*   PMC_SUCCESS        - when API execution is successful otherwise a descriptive 
*                        error is returned. 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_ge_patt_gen_cfg_get(digi_handle_t *digi_handle,
                                                  digi_ge_serdes_id_t ge_intf,
                                                  util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr)
{
    d8_tx_2x_handle_t *d8_tx_2x_handle;
    UINT8 serdes_id;
    digi_ge_serdes_ctxt_t *ge_ctxt;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(patt_gen_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    result = digi_ge_serdes_ctxt_get(digi_handle, ge_intf, &ge_ctxt);

    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    patt_gen_cfg_ptr->patt_cfg.enable = ge_ctxt->prbs_gen_enabled;

    if (TRUE == patt_gen_cfg_ptr->patt_cfg.enable)
    {
        /* retrieve d8_tx_2x handle associated to ge serdes */
        result = digi_ge_d8_tx_2x_handle_get(digi_handle, ge_intf, &d8_tx_2x_handle, &serdes_id);
        
        if(PMC_SUCCESS != result)
        {
            PMC_RETURN(result);
        }

        result =  d8_tx_2x_patt_gen_cfg_get(d8_tx_2x_handle,
                                            serdes_id,
                                            patt_gen_cfg_ptr);
    }

    PMC_RETURN(result);
}/*digi_serdes_ge_patt_gen_cfg_get*/

/*******************************************************************************
* digi_serdes_s16_clk_patt_gen_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is an internal helper function that setup the pattern generator to 
*   configure a S16 serdes instance a clock
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   enable             - do we want to enable or disable the pattern generator
*   divider            - the divider to apply to clock 
*
* OUTPUTS:
*  *patt_cfg_ptr   - the pattern generator configure 
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_serdes_s16_clk_patt_gen_cfg_get(digi_handle_t *digi_handle, 
                                                 BOOL8 enable, 
                                                 digi_s16_clk_divider_t divider,
                                                 util_serdes_patt_gen_cfg_t *patt_cfg_ptr)
{
    UINT32 itr;

    PMC_ENTRY();
    PMC_ASSERT(patt_cfg_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    
    patt_cfg_ptr->patt_cfg.enable = enable;
    patt_cfg_ptr->patt_cfg.inv_cfg = FALSE;
    if (TRUE == enable) 
    {
        patt_cfg_ptr->patt_cfg.mode = UTIL_PATT_MODE_USER_DEF;
        patt_cfg_ptr->patt_cfg.usr_patt_size = UTIL_PATTERN_GEN_MON_USER_DATA_MAX_BIT_SIZE;
        for (itr = 0; itr < UTIL_PATTERN_GEN_MON_USER_DATA_S16_WORD_SIZE; itr++)
        {               
            if (DIGI_S16_CLK_DIVIDER_16 == divider) 
            {
                patt_cfg_ptr->patt_cfg.usr_patt[itr] = 0x00FF00FF;
            }
            else if (DIGI_S16_CLK_DIVIDER_64 == divider)
            {
                patt_cfg_ptr->patt_cfg.usr_patt[itr] = (itr %2)==1?0x00000000:0xFFFFFFFF;
            }
        }
    }
    
    PMC_RETURN();

} /* digi_serdes_s16_clk_patt_gen_cfg_get */

/*******************************************************************************
* digi_serdes_s16_all_shutdown
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Shutdown the s16 type SERDES only. Errors here are only logged to file as we
*   wish to try and proceed to shutdown as much as we can.
*
* INPUTS:
*   *digi_handle   - pointer to DIGI handle instance
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
*   None
*
*******************************************************************************/
PRIVATE void digi_serdes_s16_all_shutdown(digi_handle_t *digi_handle)
{
    PMC_ENTRY();


    if (digi_serdes_s16_cfg(digi_handle, 
                            HOSTMSG_S16_SERDES_INTF_LINE, 
                            DIGI_SERDES_MASK_NONE,
                            DIGI_SERDES_PORT_MODE_ANY_LOWPWR,
                            DIGI_SERDES_S16_MASK_ALL) != PMC_SUCCESS) {

        /* Log failure, but continue to try and shutdown the next SERDES */
        PMC_LOG(PMC_LOG_SEV_LOWEST, DIGI_ERR_SERDES_COULD_NOT_SHUTDOWN, 
                DIGI_ALL_SERDES_INTF_S16_LINE, 0); 

    }

    if (digi_serdes_s16_cfg(digi_handle, 
                            HOSTMSG_S16_SERDES_INTF_SYS, 
                            DIGI_SERDES_MASK_NONE,
                            DIGI_SERDES_PORT_MODE_ANY_LOWPWR,
                            DIGI_SERDES_S16_MASK_ALL) != PMC_SUCCESS) {

        /* Log failure, but continue to try and shutdown the next SERDES */
        PMC_LOG(PMC_LOG_SEV_LOWEST, DIGI_ERR_SERDES_COULD_NOT_SHUTDOWN, 
                DIGI_ALL_SERDES_INTF_S16_SYSTEM, 0); 
    }

    PMC_RETURN();

} /* digi_serdes_s16_all_shutdown */


/*******************************************************************************
* digi_ge_intf_serdes_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures one GE serdes port.
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   ge_intf               - GE serdes interface description
*   enable                - TRUE Ge port is enabled, FALSE it is disabled
*
* OUTPUTS:
*  NONE.
*
* RETURNS:
*   PMC_ERROR           - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_ge_intf_serdes_cfg(digi_handle_t *digi_handle,
                                          digi_ge_serdes_id_t ge_intf,
                                          BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;        
    hostmsg_d8_intf_type_enum d8_intf = HOSTMSG_D8_SERDES_INTF_MAX_NUM;
    digi_ge_serdes_ctxt_t *ge_ctxt, *mate_ge_ctxt;
    digi_ge_serdes_id_t mate_ge_intf;
    BOOL do_csu_cfg = FALSE;

    PMC_ENTRY();

    PMC_ASSERT((NULL != digi_handle), DIGI_ERR_NULL_HANDLE, 0, 0);

    if (PMC_SUCCESS == result)
    {        
        /* find firmware code associated to given interface */
        result = digi_serdes_intf_to_hostmsg_d8_convert(ge_intf, &d8_intf);
        PMC_ASSERT(result == PMC_SUCCESS, result,0,0);
        
        /* retrieve GE serdes context */
        result = digi_ge_serdes_ctxt_get(digi_handle, ge_intf, &ge_ctxt);
        PMC_ASSERT(result == PMC_SUCCESS, result,0,0);

        if (((UINT32)ge_intf % 2) == 1)
        {
            mate_ge_intf = (digi_ge_serdes_id_t) (((UINT32) ge_intf) - 1);
        } else 
        {
            mate_ge_intf = (digi_ge_serdes_id_t) (((UINT32) ge_intf) + 1);
        }
        result = digi_ge_serdes_ctxt_get(digi_handle, mate_ge_intf , &mate_ge_ctxt);
        PMC_ASSERT(result == PMC_SUCCESS, result,0,0);
        
        if (ge_ctxt->enabled == enable)
        {
            result = DIGI_ERR_GE_SERDES_ALREADY_CFG;
        }

        if (PMC_SUCCESS == result)
        {
            if (TRUE == ge_ctxt->loopback_enabled)
            {
                result = DIGI_ERR_LOOPBACK_STILL_ACTIVE;
            }
            else if (TRUE == ge_ctxt->prbs_mon_enabled)
            {
                result = DIGI_ERR_PRBS_MON_STILL_ACTIVE;
            }
            else if (TRUE == ge_ctxt->prbs_gen_enabled)
            {
                result = DIGI_ERR_PRBS_GEN_STILL_ACTIVE;
            }
        }
        
        /* 
         * There's one CSU for all slices - enable it only once.
         * Note: the digi_serdes_d8_cfg function will update the csu_enabled status.
         */
        if (enable) 
            do_csu_cfg = (digi_handle->ge_intf_handle->csu_enabled == FALSE);
        else
            do_csu_cfg = FALSE;

        if (PMC_SUCCESS == result)
        {
            if (FALSE == enable && mate_ge_ctxt->enabled == TRUE)
            {
                /* GE serdes are paired, if we reset 1, we also clear the other one*/
                digi_serdes_ge_params_wr(digi_handle, d8_intf, FALSE);
            } else 
            {
                result = digi_serdes_d8_cfg(digi_handle, d8_intf, do_csu_cfg, enable);
            }
        }        
    }

    PMC_RETURN(result);
}/*digi_ge_intf_serdes_cfg*/



/*******************************************************************************
* digi_serdes_intf_to_hostmsg_d8_convert
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function convert digi_ge_serdes_id_t enum to hostmsg_d8_intf_type_enum.
*
* INPUTS:
*   ge_intf               - GE serdes interface description
*
* OUTPUTS:
*   hostmsg_d8_intf       - hostmsg_d8_intf_type_enum corresponding to GE serdes type.
*
* RETURNS:
*   PMC_ERROR             - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_intf_to_hostmsg_d8_convert( digi_ge_serdes_id_t ge_intf,
                                                          hostmsg_d8_intf_type_enum *hostmsg_d8_intf)
{    
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(hostmsg_d8_intf != NULL, DIGI_ERR_NULL_HANDLE, 0, 0); 
    
    switch(ge_intf)
    {
    case DIGI_SERDES_INTF_OHP_0:
        *hostmsg_d8_intf = HOSTMSG_D8_SERDES_INTF_OHP_0;
        break;
    case DIGI_SERDES_INTF_OHP_1:
        *hostmsg_d8_intf = HOSTMSG_D8_SERDES_INTF_OHP_1;
        break;
    case DIGI_SERDES_INTF_OHP_2:
        *hostmsg_d8_intf = HOSTMSG_D8_SERDES_INTF_OHP_2;
        break;
    case DIGI_SERDES_INTF_EMGMT:
        *hostmsg_d8_intf = HOSTMSG_D8_SERDES_INTF_ENET_MGMT;
        break;
    case DIGI_SERDES_INTF_DI_RCP:
        *hostmsg_d8_intf = HOSTMSG_D8_SERDES_INTF_DI_RCP;
        break;
    case DIGI_SERDES_INTF_RI_RCP:
        *hostmsg_d8_intf = HOSTMSG_D8_SERDES_INTF_RI_RCP;
        break;
    default:
        result = DIGI_ERR_INVALID_ARG;
        break; 
    }   
    
    PMC_RETURN(result);
}/*digi_serdes_intf_to_hostmsg_d8_convert*/


/*******************************************************************************
* digi_serdes_interface_from_string_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Returns serdes interface matching a given string.
*
* INPUTS:
*  st - string to parse
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   DIGI serdes interface type.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE digi_serdes_intf_t digi_serdes_interface_from_string_get(char *st)
{
    digi_serdes_intf_t rc = LAST_DIGI_SERDES_INTF;
    PMC_ENTRY();

    if (0 == strcmp(st, "S16_LINE"))
    {
        rc = DIGI_SERDES_LINE_INTF;
    }
    else if (0 == strcmp(st, "S16_SYS"))
    {
        rc = DIGI_SERDES_SYSTEM_INTF;
    }
    else if (0 == strcmp(st, "T8"))
    {
        rc = DIGI_SERDES_BACKPLANE_LR_INTF;
    }
    else if (0 == strcmp(st, "C8"))
    {
        rc = DIGI_SERDES_BACKPLANE_SR_INTF;
    }

    PMC_RETURN(rc);
}/* digi_serdes_interface_from_string_get */

/*******************************************************************************
* digi_serdes_port_mode_from_string_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Returns serdes port mode matching a given string.
*
* INPUTS:
*  st - string to parse
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   DIGI serdes port mode.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE digi_serdes_port_mode_t digi_serdes_port_mode_from_string_get(char *st)
{
    UINT32 i;
    digi_serdes_port_mode_t rc = LAST_DIGI_SERDES_PORT_MODE;
    /* The following list is built from digi_serdes_port_mode_t enum */
    /* the list should be updated when digi_serdes_port_mode_t is changed */
    const char *port_mode_string_l[] = DIGI_SERDES_PORT_MODE_IN_STRING;

    PMC_ENTRY();
    
    /* lookup in serdes port mode string table and find a matching string */
    for (i = 0; i < (UINT32)LAST_DIGI_SERDES_PORT_MODE && rc == LAST_DIGI_SERDES_PORT_MODE; i++)
    {
        if (0 == strcmp(st, port_mode_string_l[i]))
        {
            rc = (digi_serdes_port_mode_t)i;
        }
    }
    
    PMC_RETURN(rc);
} /*digi_serdes_port_mode_from_string_get */

/*******************************************************************************
* digi_serdes_field_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Returns one field in a given line and at a given position.
*
* INPUTS:
*   line            - given line in which the field should be extracted.
*   start_offset    - field start position in the given line.
*
* OUTPUTS:
*   field_str      - field value as a string
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE void digi_serdes_field_get(char      *line, 
                                   UINT32     start_offset,
                                   char      *field_str)
{
    UINT32 i;
    UINT32 j = 0;

    PMC_ENTRY();

    PMC_ASSERT(NULL != line, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != field_str, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT((UINT32)PMC_STRLEN(line) > start_offset, DIGI_ERR_INVALID_ARG, 0, 0 );
    
    for (i = start_offset; i < (UINT32)PMC_STRLEN(line) && (line[i] != ',' && line[i] != '\n'); i++)
    {
        field_str[j] = line[i];
        j++;
    }

    /* add end of string character */
    field_str[j] = '\0';
    PMC_RETURN();
}/*digi_serdes_field_get */


#ifndef PMC_SW_SIMULATION 
/*******************************************************************************
* digi_serdes_adaptation_values_read
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Read device adaptation values depending on serdes current rate.
*
* INPUTS:
*  digi_handle     - DIGI device handle.
*  interface       - SERDES interface to configure.
*  lane            - Serdes lane identifier.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_adaptation_values_read(digi_handle_t               *digi_handle, 
                                                     hostmsg_adapt_intf_type_t   interface,
                                                     UINT32                      lane)
{
    serdes_c8_adapt_objects_t   serdes_c8_adapt_objects;
    serdes_t8_adapt_objects_t   serdes_t8_adapt_objects;
    serdes_s16_adapt_objects_t   serdes_s16_adapt_objects;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    
    /* find adaptation params for each serdes lane */
    switch(interface)
    {
    case HOSTMSG_ADAPT_INTF_S16_LINE:
        rc = digi_serdes_s16_adapt_params_internal_get(digi_handle, HOSTMSG_S16_SERDES_INTF_LINE, lane, &serdes_s16_adapt_objects);
        break;
    case HOSTMSG_ADAPT_INTF_S16_SYS:
        rc = digi_serdes_s16_adapt_params_internal_get(digi_handle, HOSTMSG_S16_SERDES_INTF_SYS, lane, &serdes_s16_adapt_objects);
        break;
    case HOSTMSG_ADAPT_INTF_T8:
        rc = digi_serdes_t8_adapt_params_internal_get(digi_handle, lane, &serdes_t8_adapt_objects);
        break;
    case HOSTMSG_ADAPT_INTF_C8:
        rc = digi_serdes_c8_adapt_params_internal_get(digi_handle, lane, &serdes_c8_adapt_objects);
        break;
    default:
        rc = DIGI_ERR_UNKNOWN;
        break;
        
    }
    
    PMC_RETURN(rc);
}/* digi_serdes_adaptation_values_read */

/*******************************************************************************
* digi_serdes_c8_adapt_params_internal_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Internal implementation that retrieves C8 serdes adaptation parameters.
*
* INPUTS:
*  digi_handle                   - DIGI device handle.
*  lane                          - Lane to configure (0-7).
*
* OUTPUTS:
*  *serdes_c8_adapt_objects_ptr  - Pointer to adaptation values to write.
*                                 Note that for dp_ffe_a, the least 
*                                 significant 2 bits (1:0) will be zeroed
*                                 when writing to the SERDES.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_c8_adapt_params_internal_get(digi_handle_t               *digi_handle, 
                                                           UINT32                       lane,
                                                           serdes_c8_adapt_objects_t   *serdes_c8_adapt_objects_ptr)
{
    PMC_ERROR rc = DIGI_ERR_INVALID_ARG;
    UINT32 j;
    BOOL8 found = FALSE;

    PMC_ENTRY();
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != serdes_c8_adapt_objects_ptr, DIGI_ERR_INVALID_ARG, 0, 0);


    /* if the adaptation params are not found in serdes context, retrieve it from the device */
    rc = digi_serdes_c8_read_adaptation_values(digi_handle, lane, serdes_c8_adapt_objects_ptr);

    if (PMC_SUCCESS == rc && DIGI_SERDES_MAX_C8_IN_BP_SR > lane)
    {
        for (j = 0; j < DIGI_SERDES_C8_NUM_RATES_MAX && 
                 FALSE == found; j++)
        { 
            /* lookup entry matching C8 serdes, and its current rate */
            if (TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[lane].c8_adapt_params[j].valid &&
                digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[lane].c8_adapt_params[j].port_mode == 
                digi_handle->var.serdes_ctxt.c8_lanes_rate[lane])
            {
                serdes_c8_adapt_objects_ptr->port_mode = digi_handle->var.serdes_ctxt.c8_lanes_rate[lane];
                serdes_c8_adapt_objects_ptr->valid = TRUE;
                PMC_MEMCPY(&digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[lane].c8_adapt_params[j], 
                           serdes_c8_adapt_objects_ptr, 
                           sizeof(serdes_c8_adapt_objects_t)); 
                found = TRUE;
            }
        }
        for (j = 0; j < DIGI_SERDES_C8_NUM_RATES_MAX && 
                 FALSE == found; j++)
        { 
            /* lookup entry matching C8 serdes, and its current rate */
            if (FALSE == digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[lane].c8_adapt_params[j].valid)
            {
                serdes_c8_adapt_objects_ptr->valid = TRUE;
                serdes_c8_adapt_objects_ptr->port_mode = digi_handle->var.serdes_ctxt.c8_lanes_rate[lane];
                PMC_MEMCPY(&digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[lane].c8_adapt_params[j], 
                           serdes_c8_adapt_objects_ptr, 
                           sizeof(serdes_c8_adapt_objects_t)); 
                found = TRUE;
            }
        }
    }

    /* return error code when serdes context is not updated */        
    if (PMC_SUCCESS == rc && FALSE == found)
    {
        rc = DIGI_ERR_INVALID_ARG;
    }

    PMC_RETURN(rc);
}/* digi_serdes_c8_adapt_params_internal_get */

/*******************************************************************************
* digi_serdes_s16_adapt_params_internal_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Internal implementation that retrieves S16 serdes adaptation parameters.
*
* INPUTS:
*  digi_handle                   - DIGI device handle.
*  s16_intf_type                 - interface type of S16 serdes: LINE or SYS
*  lane                          - Lane to configure (0-12|16).
*
* OUTPUTS:
*  *serdes_s16_adapt_objects_ptr  - Pointer to adaptation values.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_s16_adapt_params_internal_get(digi_handle_t               *digi_handle, 
                                                            hostmsg_s16_intf_type_t      s16_intf_type,
                                                            UINT32                       lane,
                                                            serdes_s16_adapt_objects_t  *serdes_s16_adapt_objects_ptr)
{
    PMC_ERROR rc = DIGI_ERR_INVALID_ARG;
    UINT32 j;
    digi_serdes_intf_t serdes_intf = LAST_DIGI_SERDES_INTF;
    digi_serdes_port_mode_t *rates_l = NULL;
    BOOL8 found = FALSE;
    UINT32 nb_lanes;
    
    PMC_ENTRY();

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != serdes_s16_adapt_objects_ptr, DIGI_ERR_INVALID_ARG, 0, 0);

    /* select interface type and serdes rates table */
    switch (s16_intf_type)
    {
    case HOSTMSG_S16_SERDES_INTF_LINE:
        serdes_intf = DIGI_SERDES_LINE_INTF;
        rates_l = digi_handle->var.serdes_ctxt.s16_line_lanes_rate;
        rc = PMC_SUCCESS;
        nb_lanes = DIGI_SERDES_XFI_LANES_MAX;
        break;
    case HOSTMSG_S16_SERDES_INTF_SYS:
        serdes_intf = DIGI_SERDES_SYSTEM_INTF;
        rates_l = digi_handle->var.serdes_ctxt.s16_sys_lanes_rate;
        rc = PMC_SUCCESS;
        nb_lanes = DIGI_SERDES_MAX_S16_IN_BP;
        break;
    default:
        break;
    }

    /* check that lane is in range of values */
    if (PMC_SUCCESS == rc)
    {
        if (nb_lanes <= lane)
        {
            rc = DIGI_ERR_INVALID_ARG;
        }
    }

    if (PMC_SUCCESS == rc)
    {        
        /* retrieve the adaptation params are not found in serdes context, it from the device */
        rc = digi_serdes_s16_read_adaptation_values(digi_handle, s16_intf_type, lane, serdes_s16_adapt_objects_ptr);
        
        if (PMC_SUCCESS == rc && NULL != rates_l)
        {
            /* lookup entry matching S16 serdes, and its current rate */
            /* if the serdes is not provisioned the function returns an error */
            for (j = 0; j < DIGI_SERDES_S16_NUM_RATES_MAX && 
                     FALSE == found; j++)
            { 
                /* select interface type and serdes rates table */
                switch (s16_intf_type)
                {
                case HOSTMSG_S16_SERDES_INTF_LINE:  
                    /* lookup entry matching C8 serdes, and its current rate */
                    if (lane < DIGI_SERDES_XFI_LANES_MAX &&
                        digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[lane].s16_line_adapt_params[j].port_mode == rates_l[lane] &&
                        TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[lane].s16_line_adapt_params[j].valid)
                    {      
                        serdes_s16_adapt_objects_ptr->port_mode = rates_l[lane];
                        serdes_s16_adapt_objects_ptr->valid = TRUE;
                        PMC_MEMCPY(&digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[lane].s16_line_adapt_params[j], 
                                   serdes_s16_adapt_objects_ptr, 
                                   sizeof(serdes_s16_adapt_objects_t)); 
                        found = TRUE;
                    }
                    break;
                case HOSTMSG_S16_SERDES_INTF_SYS:
                    /* lookup entry matching C8 serdes, and its current rate */
                    if (lane < DIGI_SERDES_MAX_S16_IN_BP &&
                        digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[lane].s16_sys_adapt_params[j].port_mode == rates_l[lane] &&
                        TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[lane].s16_sys_adapt_params[j].valid)
                    {      
                        serdes_s16_adapt_objects_ptr->port_mode = rates_l[lane];
                        serdes_s16_adapt_objects_ptr->valid = TRUE;
                        PMC_MEMCPY(&digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[lane].s16_sys_adapt_params[j], 
                                   serdes_s16_adapt_objects_ptr, 
                                   sizeof(serdes_s16_adapt_objects_t)); 
                        found = TRUE;
                    }
                    break;
                default:
                    break;
                }
            }
            /* if entry not found, added new entry */
            /* lookup entry matching S16 serdes, and its current rate */
            /* if the serdes is not provisioned the function returns an error */
            for (j = 0; j < DIGI_SERDES_S16_NUM_RATES_MAX && 
                     FALSE == found; j++)
            { 
                /* select interface type and serdes rates table */
                switch (s16_intf_type)
                {
                case HOSTMSG_S16_SERDES_INTF_LINE:  
                    /* lookup entry matching S16 LINE  serdes, and its current rate */

                    if (lane < DIGI_SERDES_XFI_LANES_MAX &&
                        FALSE == digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[lane].s16_line_adapt_params[j].valid)
                    {      
                        serdes_s16_adapt_objects_ptr->port_mode = rates_l[lane];
                        serdes_s16_adapt_objects_ptr->valid = TRUE;
                        PMC_MEMCPY(&digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[lane].s16_line_adapt_params[j], 
                                   serdes_s16_adapt_objects_ptr, 
                                   sizeof(serdes_s16_adapt_objects_t)); 
                        found = TRUE;
                    }
                    
                    break;
                case HOSTMSG_S16_SERDES_INTF_SYS:
                    /* lookup entry matching S16 SYS serdes, and its current rate */
                    if (lane < DIGI_SERDES_MAX_S16_IN_BP &&
                        FALSE == digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[lane].s16_sys_adapt_params[j].valid)
                    {      
                        serdes_s16_adapt_objects_ptr->port_mode = rates_l[lane];
                        serdes_s16_adapt_objects_ptr->valid = TRUE;
                        PMC_MEMCPY(&digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[lane].s16_sys_adapt_params[j], 
                                   serdes_s16_adapt_objects_ptr, 
                                   sizeof(serdes_s16_adapt_objects_t)); 
                        found = TRUE;
                    }
                    break;
                default:
                    break;
                }
            }            
        }    
    }

    /* return error code when serdes context is not updated */        
    if (PMC_SUCCESS == rc && FALSE == found)
    {
        rc = DIGI_ERR_INVALID_ARG;
    }

    PMC_RETURN(rc);
}/* digi_serdes_s16_adapt_params_internal_get */


/*******************************************************************************
* digi_serdes_t8_adapt_params_intenral_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Internal implementation that retrieves T8 serdes adaptation parameters.
*
* INPUTS:
*  digi_handle                   - DIGI device handle.
*  lane                          - Lane to configure (0-39).
*
* OUTPUTS:
*  *serdes_t8_adapt_objects_ptr  - Pointer to adaptation values to write.
*                                 Note that for dp_ffe_a, the least 
*                                 significant 2 bits (1:0) will be zeroed
*                                 when writing to the SERDES.
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_t8_adapt_params_internal_get(digi_handle_t               *digi_handle, 
                                                           UINT32                       lane,
                                                           serdes_t8_adapt_objects_t   *serdes_t8_adapt_objects_ptr)
{
    PMC_ERROR rc = DIGI_ERR_INVALID_ARG;
    UINT32 j;
    BOOL8 found = FALSE;

    PMC_ENTRY();
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != serdes_t8_adapt_objects_ptr, DIGI_ERR_INVALID_ARG, 0, 0);

    rc = digi_serdes_t8_read_adaptation_values(digi_handle, lane, serdes_t8_adapt_objects_ptr);

    if (PMC_SUCCESS == rc && DIGI_SERDES_MAX_T8_E1_IN_BP_LR > lane)
    {
        for (j = 0; j < DIGI_SERDES_T8_NUM_RATES_MAX && 
                 FALSE == found; j++)
        { 
            /* lookup entry matching T8 serdes, and its current rate */
            if (TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].t8_adapt_params[j].valid &&
                digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].t8_adapt_params[j].port_mode == 
                digi_handle->var.serdes_ctxt.t8_lanes_rate[lane])
            {                
                serdes_t8_adapt_objects_ptr->port_mode = digi_handle->var.serdes_ctxt.t8_lanes_rate[lane];
                serdes_t8_adapt_objects_ptr->valid = TRUE;        
                PMC_MEMCPY(&digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].t8_adapt_params[j], 
                           serdes_t8_adapt_objects_ptr, 
                           sizeof(serdes_t8_adapt_objects_t)); 
                found = TRUE;
            }
        } 
        for (j = 0; j < DIGI_SERDES_T8_NUM_RATES_MAX && 
                 FALSE == found; j++)
        { 
            /* lookup entry matching T8 serdes, and its current rate */
            if (FALSE == digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].t8_adapt_params[j].valid)
            { 
                serdes_t8_adapt_objects_ptr->port_mode = digi_handle->var.serdes_ctxt.t8_lanes_rate[lane];
                serdes_t8_adapt_objects_ptr->valid = TRUE;  
                PMC_MEMCPY(&digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].t8_adapt_params[j], 
                           serdes_t8_adapt_objects_ptr, 
                           sizeof(serdes_t8_adapt_objects_t)); 
                found = TRUE;
            }
        }   
        for (j = 0; j < DIGI_SERDES_T8_NUM_RATES_MAX && 
                 FALSE == found; j++)
        { 
            /* lookup entry matching T8 serdes, and its current rate */
            if (FALSE == digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].t8_adapt_params[j].valid &&
                LAST_DIGI_SERDES_PORT_MODE != digi_handle->var.serdes_ctxt.t8_lanes_rate[lane])
            { 
                serdes_t8_adapt_objects_ptr->port_mode = digi_handle->var.serdes_ctxt.t8_lanes_rate[lane];
                serdes_t8_adapt_objects_ptr->valid = TRUE;  
                PMC_MEMCPY(&digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].t8_adapt_params[j], 
                           serdes_t8_adapt_objects_ptr, 
                           sizeof(serdes_t8_adapt_objects_t)); 
                found = TRUE;
            }
        }   
    }

    /* return error code when serdes context is not updated */        
    if (PMC_SUCCESS == rc && FALSE == found)
    {
        rc = DIGI_ERR_INVALID_ARG;
    }

    PMC_RETURN(rc);
}/* digi_serdes_t8_adapt_params_internal_get */


/*******************************************************************************
* digi_serdes_c8_read_adaptation_values
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to read adaptation values for C8 SERDES.
*
* INPUTS:
*  digi_handle                  - DIGI device handle.
*  lane                         - Lane to read from (0-7).
*
* OUTPUTS:
*  serdes_c8_adapt_objects_ptr  - Pointer to adaptation values
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_c8_read_adaptation_values(digi_handle_t             *digi_handle, 
                                                        UINT32                     lane,
                                                        serdes_c8_adapt_objects_t *serdes_c8_adapt_objects_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;   
    UINT32 data[1];
    UINT32 response[3];

    PMC_ENTRY();

    PMC_ASSERT((NULL != serdes_c8_adapt_objects_ptr),
               HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Add message body to the previously created header. */
    data[0] = (UINT8)lane << 24;    

    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_variable_length_query((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_CFG_SERDES_C8_READ_ADAPT_ST,data,1,response,3,NULL,stat);
    }

    if (PMC_SUCCESS == result)
    {

        /* Return requested data. */
        serdes_c8_adapt_objects_ptr->dp_ffe_a      = response[0];
        serdes_c8_adapt_objects_ptr->target_amp    = response[1];
        serdes_c8_adapt_objects_ptr->adc_map_scale = response[2];
    }

    PMC_RETURN(result);
} /* End: digi_serdes_c8_read_adaptation_values() */

/*******************************************************************************
* digi_serdes_t8_read_adaptation_values
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to read adaptation values for T8 SERDES.
*
* INPUTS:
*  digi_handle                  - DIGI device handle.
*  lane                         - Lane to read from (0-39).
*
* OUTPUTS:
*  serdes_t8_adapt_objects_ptr  - Pointer to adaptation values
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_t8_read_adaptation_values(digi_handle_t             *digi_handle, 
                                                        UINT32                     lane,
                                                        serdes_t8_adapt_objects_t *serdes_t8_adapt_objects_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;   
    UINT32 data[1];
    UINT32 response[15];

    PMC_ENTRY();

    PMC_ASSERT((NULL != serdes_t8_adapt_objects_ptr),
               HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Add message body to the previously created header. */
    data[0] = (UINT8)lane << 24;    

    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_variable_length_query((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_CFG_SERDES_T8_READ_ADAPT_ST,data,1,response,15,NULL,stat);
    }

    if (PMC_SUCCESS == result)
    {

        /* Return requested data. */
        serdes_t8_adapt_objects_ptr->dp_ffe_a      = response[0];
        serdes_t8_adapt_objects_ptr->dp_ffe_b      = response[1];
        serdes_t8_adapt_objects_ptr->dp_ffe_c      = response[2];
        serdes_t8_adapt_objects_ptr->tr_ffe_a      = response[3];
        serdes_t8_adapt_objects_ptr->tr_ffe_b      = response[4];
        serdes_t8_adapt_objects_ptr->dfe_coef0     = response[5];
        serdes_t8_adapt_objects_ptr->dfe_coef1     = response[6];
        serdes_t8_adapt_objects_ptr->dfe_coef2     = response[7];
        serdes_t8_adapt_objects_ptr->dfe_coef3     = response[8];
        serdes_t8_adapt_objects_ptr->dfe_coef4     = response[9];
        serdes_t8_adapt_objects_ptr->dfe_coef5     = response[10];
        serdes_t8_adapt_objects_ptr->pga_gain      = response[11];
        serdes_t8_adapt_objects_ptr->pga_offset    = response[12];
        serdes_t8_adapt_objects_ptr->target_amp    = response[13];
        serdes_t8_adapt_objects_ptr->adc_map_scale = response[14];
    }

    PMC_RETURN(result);
} /* End: digi_serdes_t8_read_adaptation_values() */

/*******************************************************************************
* digi_serdes_s16_read_adaptation_values
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to read adaptation values for S16 SERDES.
*
* INPUTS:
*  digi_handle                  - DIGI device handle.
*  s16_intf_type                - S16 interface type
*  lane                         - Lane to read (0:11 for Line S16,
*                                 0:15 for System S16).
*
* OUTPUTS:
*  serdes_s16_adapt_objects_ptr  - Pointer to adaptation values
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_s16_read_adaptation_values(digi_handle_t              *digi_handle, 
                                                         hostmsg_s16_intf_type_t     s16_intf_type, 
                                                         UINT32                      lane,
                                                         serdes_s16_adapt_objects_t *serdes_s16_adapt_objects_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];
    UINT32 response[12];

    PMC_ENTRY();

    PMC_ASSERT((NULL != serdes_s16_adapt_objects_ptr),
               HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Add message body to the previously created header. */
    data[0] = ((UINT8)s16_intf_type << 24) | ((UINT8)lane << 16);      

    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_variable_length_query((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_CFG_SERDES_S16_READ_ADAPT_ST,data,1,response,12,NULL,stat);
    }

    if (PMC_SUCCESS == result)
    {
        /* Return requested data. */
        serdes_s16_adapt_objects_ptr->dp_ffe_a      = response[0];
        serdes_s16_adapt_objects_ptr->dp_ffe_b      = response[1];
        serdes_s16_adapt_objects_ptr->dp_ffe_c      = response[2];
        serdes_s16_adapt_objects_ptr->tr_ffe_a      = response[3];
        serdes_s16_adapt_objects_ptr->tr_ffe_b      = response[4];
        serdes_s16_adapt_objects_ptr->dfe_coef0     = response[5];
        serdes_s16_adapt_objects_ptr->dfe_coef1     = response[6];
        serdes_s16_adapt_objects_ptr->dfe_coef2     = response[7];
        serdes_s16_adapt_objects_ptr->pga_gain      = response[8];
        serdes_s16_adapt_objects_ptr->pga_offset    = response[9];
        serdes_s16_adapt_objects_ptr->target_amp    = response[10];
        serdes_s16_adapt_objects_ptr->adc_map_scale = response[11];
    }

    PMC_RETURN(result);
} /* End: digi_serdes_s16_read_adaptation_values() */


#endif /*#ifndef PMC_SW_SIMULATION */

/*******************************************************************************
* digi_serdes_adapt_params_validate
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Validates fields of the configuration of a given adaptation parameters.
*  It checks that the interface, serdes rate and lane_id are correct.
*
* INPUTS:
*  digi_handle         - DIGI device handle.
*  intf                - serdes interface.
*  lane                - serdes lane id.
*  port_mode           - serdes rate.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE when configuration field are correct, FALSE otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE BOOL8 digi_serdes_adapt_params_validate(digi_handle_t           *digi_handle, 
                                                digi_serdes_intf_t       intf,
                                                UINT32                   lane,
                                                digi_serdes_port_mode_t  port_mode)
{

    hostmsg_s16_serdes_rate_t hostmsg_s16_serdes_rate;
    hostmsg_t8_serdes_rate_t hostmsg_t8_serdes_rate;
    hostmsg_c8_serdes_rate_t hostmsg_c8_serdes_rate;
    DOUBLE                    s16_serdes_rate;
    BOOL8                     s16_otu_client;
    s16_port_mode_t           s16_serdes_port_mode;    
    PMC_ERROR convert_rc;
    BOOL8 rc = FALSE;

    PMC_ENTRY();
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    switch(intf)
    {
    case DIGI_SERDES_LINE_INTF:
        /* validate port mode */
        convert_rc =  digi_serdes_s16_rate_convert(port_mode, 
                                                   &hostmsg_s16_serdes_rate,
                                                   &s16_serdes_rate,
                                                   &s16_otu_client,
                                                   &s16_serdes_port_mode);
        if (PMC_SUCCESS == convert_rc)
        {
            /* validate lane id */
            if (DIGI_SERDES_XFI_LANES_MAX > lane)
            {
                rc = TRUE;
            }
        }

        break;
    case DIGI_SERDES_SYSTEM_INTF:
        /* validate port mode */
        convert_rc =  digi_serdes_s16_rate_convert(port_mode, 
                                                   &hostmsg_s16_serdes_rate,
                                                   &s16_serdes_rate,
                                                   &s16_otu_client,
                                                   &s16_serdes_port_mode);
        if (PMC_SUCCESS == convert_rc)
        {
            /* validate lane id */
            if (DIGI_SERDES_MAX_S16_IN_BP > lane)
            {
                rc = TRUE;
            }
        }

        break;
    case DIGI_SERDES_BACKPLANE_LR_INTF:
        /* validate port mode */
        convert_rc =  digi_serdes_t8_rate_convert(port_mode, 
                                                  &hostmsg_t8_serdes_rate);
        if (PMC_SUCCESS == convert_rc)
        {
            /* validate lane id */
            if (DIGI_SERDES_MAX_T8_E1_IN_BP_LR > lane)
            {
                rc = TRUE;
            }
        }

        break;
    case DIGI_SERDES_BACKPLANE_SR_INTF:
        /* validate port mode */
        convert_rc =  digi_serdes_c8_rate_convert(port_mode, 
                                                  &hostmsg_c8_serdes_rate);
        if (PMC_SUCCESS == convert_rc)
        {
            /* validate lane id */
            if (DIGI_SERDES_MAX_C8_IN_BP_SR > lane)
            {
                rc = TRUE;
            }
        }
        break;
    default:
        break;
    }
    
    PMC_RETURN(rc);
    
}/* digi_serdes_adapt_params_validate */

/*******************************************************************************
* digi_serdes_c8_write_adaptation_values
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to manually write adaptation values
*  for C8 SERDES.
*
* INPUTS:
*  digi_handle                  - DIGI device handle.
*  lane                         - Lane to configure (0-7).
*  serdes_c8_adapt_objects_ptr  - Pointer to adaptation values to write.
*                                 Note that for dp_ffe_a, the least 
*                                 significant 2 bits (1:0) will be zeroed
*                                 when writing to the SERDES.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_c8_write_adaptation_values(digi_handle_t              *digi_handle, 
                                                         UINT32                      lane,
                                                         serdes_c8_adapt_objects_t  *serdes_c8_adapt_objects_ptr)
{
    PMC_ERROR result = PMC_SUCCESS; 
    UINT32 data[4];

    PMC_ENTRY();

    /* Add message body to the previously created header. */
    data[0] = (UINT8)lane << 24;
    data[1] = serdes_c8_adapt_objects_ptr->dp_ffe_a; 
    data[2] = serdes_c8_adapt_objects_ptr->target_amp; 
    data[3] = serdes_c8_adapt_objects_ptr->adc_map_scale; 

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_CFG_SERDES_C8_WRITE_ADAPT_ST,data,4,stat);
    }


    PMC_RETURN(result);
} /* End: digi_serdes_c8_write_adaptation_values() */

/*******************************************************************************
* digi_serdes_t8_write_adaptation_values
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to manually write adaptation values
*  for T8 SERDES.
*
* INPUTS:
*  digi_handle                  - DIGI device handle.
*  lane                         - Lane to configure (0-39).
*  serdes_t8_adapt_objects_ptr  - Pointer to adaptation values to write
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_t8_write_adaptation_values(digi_handle_t              *digi_handle, 
                                                         UINT32                      lane,
                                                         serdes_t8_adapt_objects_t  *serdes_t8_adapt_objects_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;  
    UINT32 data[16];

    PMC_ENTRY();

    /* Add message body to the previously created header. */
    data[0]  = (UINT8)lane << 24;
    data[1]  = serdes_t8_adapt_objects_ptr->dp_ffe_a;
    data[2]  = serdes_t8_adapt_objects_ptr->dp_ffe_b;
    data[3]  = serdes_t8_adapt_objects_ptr->dp_ffe_c;
    data[4]  = serdes_t8_adapt_objects_ptr->tr_ffe_a;
    data[5]  = serdes_t8_adapt_objects_ptr->tr_ffe_b;
    data[6]  = serdes_t8_adapt_objects_ptr->dfe_coef0;
    data[7]  = serdes_t8_adapt_objects_ptr->dfe_coef1;
    data[8]  = serdes_t8_adapt_objects_ptr->dfe_coef2;
    data[9]  = serdes_t8_adapt_objects_ptr->dfe_coef3;
    data[10] = serdes_t8_adapt_objects_ptr->dfe_coef4;
    data[11] = serdes_t8_adapt_objects_ptr->dfe_coef5;
    data[12] = serdes_t8_adapt_objects_ptr->pga_gain;
    data[13] = serdes_t8_adapt_objects_ptr->pga_offset;
    data[14] = serdes_t8_adapt_objects_ptr->target_amp;
    data[15] = serdes_t8_adapt_objects_ptr->adc_map_scale;

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_CFG_SERDES_T8_WRITE_ADAPT_ST,data,16,stat);
    }

    PMC_RETURN(result);
} /* End: digi_serdes_t8_write_adaptation_values() */

/*******************************************************************************
* digi_serdes_s16_write_adaptation_values
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to manually write adaptation values
*  for S16 SERDES.
*
* INPUTS:
*  digi_handle                  - DIGI device handle.
*  s16_intf_type                - S16 interface type
*  lane                         - Lane to configure (0:11 for Line S16,
*                                 0:15 for System S16).
*  serdes_s16_adapt_objects_ptr - Pointer to adaptation values to write
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_s16_write_adaptation_values(digi_handle_t               *digi_handle, 
                                                          hostmsg_s16_intf_type_t      s16_intf_type, 
                                                          UINT32                       lane,
                                                          serdes_s16_adapt_objects_t  *serdes_s16_adapt_objects_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;  
    UINT32 data[13];

    PMC_ENTRY();

    /* Add message body to the previously created header. */
    data[0]  =  ((UINT8)s16_intf_type << 24) | ((UINT8)lane << 16);
    data[1]  =  serdes_s16_adapt_objects_ptr->dp_ffe_a;
    data[2]  =  serdes_s16_adapt_objects_ptr->dp_ffe_b;
    data[3]  =  serdes_s16_adapt_objects_ptr->dp_ffe_c;
    data[4]  =  serdes_s16_adapt_objects_ptr->tr_ffe_a;
    data[5]  =  serdes_s16_adapt_objects_ptr->tr_ffe_b;
    data[6]  =  serdes_s16_adapt_objects_ptr->dfe_coef0;
    data[7]  =  serdes_s16_adapt_objects_ptr->dfe_coef1;
    data[8]  =  serdes_s16_adapt_objects_ptr->dfe_coef2;
    data[9]  =  serdes_s16_adapt_objects_ptr->pga_gain;
    data[10] =  serdes_s16_adapt_objects_ptr->pga_offset;
    data[11] =  serdes_s16_adapt_objects_ptr->target_amp;
    data[12] =  serdes_s16_adapt_objects_ptr->adc_map_scale;

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_CFG_SERDES_S16_WRITE_ADAPT_ST,data,13,stat);
    }

    PMC_RETURN(result);
} /* End: digi_serdes_s16_write_adaptation_values() */

/*******************************************************************************
* digi_serdes_internal_adapt_status_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves adaptation results for SERDES configuration.
*
* INPUTS:
*  digi_handle - DIGI device handle.
*
* OUTPUTS:
*  serdes_adapt_status_ptr - pointer to storage for returned status.\n
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_internal_adapt_status_get(digi_handle_t         *digi_handle,
                                                       serdes_adapt_status_t *serdes_adapt_status_ptr)
{
    PMC_ERROR result = PMC_SUCCESS; 
    UINT32 response[7];

    PMC_ENTRY();

    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_variable_length_query((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_REQ_SERDES_GET_ADAPT_STAT_ST,NULL,0,response,7,NULL,stat);
    }

    if (PMC_SUCCESS == result)
    {

        /* Return requested data. */
        serdes_adapt_status_ptr->adapt_intf_type = (hostmsg_adapt_intf_type_t)((response[0] >> 24) & 0xff);
        serdes_adapt_status_ptr->cfg_lane_msk_19_0 = response[1];
        serdes_adapt_status_ptr->cfg_lane_msk_39_20 = response[2];
        serdes_adapt_status_ptr->success_lane_msk_19_0 = response[3];
        serdes_adapt_status_ptr->success_lane_msk_39_20 = response[4];
        serdes_adapt_status_ptr->fail_lane_msk_19_0 = response[5];
        serdes_adapt_status_ptr->fail_lane_msk_39_20 = response[6];
    }

    PMC_RETURN(result);

} /* digi_serdes_internal_adapt_status_get */

/*******************************************************************************
* digi_serdes_c8_tx_swing_emphasis_levels_in_ctxt_set
* ___________________________________________________________________________
*
* DESCRIPTION:
* Save C8 serdes TX swing and de-emphasis levels in serdes context.
*
* INPUTS:
*  digi_handle                   - DIGI device handle.
*  lane                          - Lane to configure (0-7).
*  *levels_cfg_ptr               - pointer to TX swing and de-emphasis levels
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_c8_tx_swing_emphasis_levels_in_ctxt_set(digi_handle_t          *digi_handle,
                                                                      UINT32                  lane,
                                                                      c8_tx_swing_levels_t   *levels_cfg_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 j;
    BOOL8 found = FALSE;

    PMC_ENTRY();
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != levels_cfg_ptr, DIGI_ERR_INVALID_ARG, 0, 0);

    if (DIGI_SERDES_MAX_C8_IN_BP_SR > lane)
    {
        for (j = 0; j < DIGI_SERDES_C8_NUM_RATES_MAX && 
                 FALSE == found; j++)
        { 
            /* lookup entry matching C8 serdes, and its current rate */
            if (TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[lane].c8_tx_swing_levels[j].valid &&
                digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[lane].c8_tx_swing_levels[j].port_mode == 
                digi_handle->var.serdes_ctxt.c8_lanes_rate[lane])
            {
                PMC_MEMCPY(&digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[lane].c8_tx_swing_levels[j].levels, 
                           levels_cfg_ptr, 
                           sizeof(c8_tx_swing_levels_t)); 
                found = TRUE;
            }
        }
        for (j = 0; j < DIGI_SERDES_C8_NUM_RATES_MAX && 
                 FALSE == found; j++)
        { 
            /* lookup entry matching C8 serdes, and its current rate */
            if (FALSE == digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[lane].c8_tx_swing_levels[j].valid)
            {
                digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[lane].c8_tx_swing_levels[j].valid = TRUE;
                digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[lane].c8_tx_swing_levels[j].port_mode = digi_handle->var.serdes_ctxt.c8_lanes_rate[lane];
                PMC_MEMCPY(&digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[lane].c8_tx_swing_levels[j].levels, 
                           levels_cfg_ptr, 
                           sizeof(c8_tx_swing_levels_t)); 
                found = TRUE;
            }
        }
    }

    /* return error code when serdes context is not updated */        
    if (PMC_SUCCESS == rc && FALSE == found)
    {
        rc = DIGI_ERR_INVALID_ARG;
    }

    PMC_RETURN(rc);
}/* digi_serdes_c8_tx_swing_emphasis_levels_in_ctxt_set */

/*******************************************************************************
* digi_serdes_s16_tx_swing_emphasis_levels_in_ctxt_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Save S16 serdes TX swing and de-emphasis levels in serdes context.
*
* INPUTS:
*  digi_handle                   - DIGI device handle.
*  s16_intf                      - interface type of S16 serdes: LINE or SYS
*  lane                          - Lane to configure (0-12|16).
*  *levels_cfg_ptr               - Pointer to TX swing and de-emphasis levels.
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_s16_tx_swing_emphasis_levels_in_ctxt_set(digi_handle_t             *digi_handle,
                                                                       digi_serdes_intf_t         s16_intf,
                                                                       UINT32                     lane,
                                                                       s16_tx_swing_levels_t     *levels_cfg_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 j;
    digi_serdes_port_mode_t *rates_l = NULL;
    BOOL8 found = FALSE;
    UINT32 nb_lanes;
    
    PMC_ENTRY();

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != levels_cfg_ptr, DIGI_ERR_INVALID_ARG, 0, 0);

    /* select interface type and serdes rates table */
    switch (s16_intf)
    {
    case DIGI_SERDES_LINE_INTF:
        rates_l = digi_handle->var.serdes_ctxt.s16_line_lanes_rate;
        nb_lanes = DIGI_SERDES_XFI_LANES_MAX;
        break;
    case DIGI_SERDES_SYSTEM_INTF:
        rates_l = digi_handle->var.serdes_ctxt.s16_sys_lanes_rate;
        nb_lanes = DIGI_SERDES_MAX_S16_IN_BP;
        break;
    default:
        rc = DIGI_ERR_INVALID_ARG;
        break;
    }

    /* check that lane is in range of values */
    if (PMC_SUCCESS == rc)
    {
        if (nb_lanes <= lane)
        {
            rc = DIGI_ERR_INVALID_ARG;
        }
    }

    if (PMC_SUCCESS == rc)
    {        
        if (NULL != rates_l)
        {
            /* lookup entry matching S16 serdes, and its current rate */
            /* if the serdes is not provisioned the function returns an error */
            for (j = 0; j < DIGI_SERDES_S16_NUM_RATES_MAX && 
                     FALSE == found; j++)
            { 
                /* select interface type and serdes rates table */
                switch (s16_intf)
                {
                case DIGI_SERDES_LINE_INTF:  
                    /* lookup entry matching C8 serdes, and its current rate */
                    if (lane < DIGI_SERDES_XFI_LANES_MAX &&
                        digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[lane].s16_line_tx_swing_levels[j].port_mode == rates_l[lane] &&
                        TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[lane].s16_line_tx_swing_levels[j].valid)
                    {      
                        PMC_MEMCPY(&digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[lane].s16_line_tx_swing_levels[j].levels,
                                   levels_cfg_ptr, 
                                   sizeof(s16_tx_swing_levels_t)); 
                        found = TRUE;
                    }
                    
                    break;
                case DIGI_SERDES_SYSTEM_INTF:
                    /* lookup entry matching C8 serdes, and its current rate */
                    if (lane < DIGI_SERDES_MAX_S16_IN_BP &&
                        digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[lane].s16_sys_tx_swing_levels[j].port_mode == rates_l[lane] &&
                        TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[lane].s16_sys_tx_swing_levels[j].valid)
                    {      
                        PMC_MEMCPY(&digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[lane].s16_sys_tx_swing_levels[j].levels, 
                                   levels_cfg_ptr, 
                                   sizeof(s16_tx_swing_levels_t)); 
                        found = TRUE;
                    }
                    break;
                default:
                    break;
                }
            }
            /* if entry not found, added new entry */
            /* lookup entry matching S16 serdes, and its current rate */
            /* if the serdes is not provisioned the function returns an error */
            for (j = 0; j < DIGI_SERDES_S16_NUM_RATES_MAX && 
                     FALSE == found; j++)
            { 
                /* select interface type and serdes rates table */
                switch (s16_intf)
                {
                case DIGI_SERDES_LINE_INTF:
                    /* lookup entry matching C8 serdes, and its current rate */
                    if (lane < DIGI_SERDES_XFI_LANES_MAX &&
                        FALSE == digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[lane].s16_line_tx_swing_levels[j].valid)
                    {      
                        digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[lane].s16_line_tx_swing_levels[j].port_mode = rates_l[lane];
                        digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[lane].s16_line_tx_swing_levels[j].valid = TRUE;
                        PMC_MEMCPY(&digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[lane].s16_line_tx_swing_levels[j].levels, 
                                   levels_cfg_ptr, 
                                   sizeof(s16_tx_swing_levels_t)); 
                        found = TRUE;
                    }
                    
                    break;
                case DIGI_SERDES_SYSTEM_INTF:
                    /* lookup entry matching C8 serdes, and its current rate */
                    if (lane < DIGI_SERDES_MAX_S16_IN_BP &&
                        FALSE == digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[lane].s16_sys_tx_swing_levels[j].valid)
                    {      
                        digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[lane].s16_sys_tx_swing_levels[j].port_mode = rates_l[lane];
                        digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[lane].s16_sys_tx_swing_levels[j].valid = TRUE;
                        PMC_MEMCPY(&digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[lane].s16_sys_tx_swing_levels[j].levels, 
                                   levels_cfg_ptr, 
                                   sizeof(s16_tx_swing_levels_t)); 
                        found = TRUE;
                    }
                    break;
                default:
                    break;
                }
            }            
        }    
    }

    /* return error code when serdes context is not updated */        
    if (PMC_SUCCESS == rc && FALSE == found)
    {
        rc = DIGI_ERR_INVALID_ARG;
    }

    PMC_RETURN(rc);
}/* digi_serdes_s16_tx_swing_emphasis_levels_in_ctxt_set */


/*******************************************************************************
* digi_serdes_t8_tx_swing_emphasis_levels_in_ctxt_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Save S16 serdes TX swing and de-emphasis levels in serdes context.
*
* INPUTS:
*  digi_handle                   - DIGI device handle.
*  lane                          - Lane to configure (0-7).
*  *levels_cfg_ptr               - pointer to TX swing and de-emphasis levels
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_t8_tx_swing_emphasis_levels_in_ctxt_set(digi_handle_t         *digi_handle,
                                                                      UINT32                 lane,
                                                                      t8_tx_swing_levels_t  *levels_cfg_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 j;
    BOOL8 found = FALSE;

    PMC_ENTRY();
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != levels_cfg_ptr, DIGI_ERR_INVALID_ARG, 0, 0);

    if (DIGI_SERDES_MAX_T8_E1_IN_BP_LR > lane)
    {
        for (j = 0; j < DIGI_SERDES_T8_NUM_RATES_MAX && 
                 FALSE == found; j++)
        { 
            /* lookup entry matching T8 serdes, and its current rate */
            if (TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].t8_tx_swing_levels[j].valid &&
                digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].t8_tx_swing_levels[j].port_mode == 
                digi_handle->var.serdes_ctxt.t8_lanes_rate[lane])
            {
                PMC_MEMCPY(&digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].t8_tx_swing_levels[j].levels, 
                           levels_cfg_ptr, 
                           sizeof(t8_tx_swing_levels_t)); 
                found = TRUE;
            }
        }
        for (j = 0; j < DIGI_SERDES_T8_NUM_RATES_MAX && 
                 FALSE == found; j++)
        { 
            /* lookup entry matching T8 serdes, and its current rate */
            if (FALSE == digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].t8_tx_swing_levels[j].valid)
            {
                digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].t8_tx_swing_levels[j].valid = TRUE;
                digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].t8_tx_swing_levels[j].port_mode = digi_handle->var.serdes_ctxt.t8_lanes_rate[lane];
                PMC_MEMCPY(&digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].t8_tx_swing_levels[j].levels, 
                           levels_cfg_ptr, 
                           sizeof(t8_tx_swing_levels_t)); 
                found = TRUE;
            }
        }
    }

    /* return error code when serdes context is not updated */        
    if (PMC_SUCCESS == rc && FALSE == found)
    {
        rc = DIGI_ERR_INVALID_ARG;
    }

    PMC_RETURN(rc);
}/* digi_serdes_t8_tx_swing_emphasis_levels_in_ctxt_set */


/*******************************************************************************
* digi_serdes_t8_lanes_params_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Configure T8 serdes device with properties saved in context.
*
* INPUTS:
*  digi_handle            - DIGI device handle.
*  lane_0_19_enable_msk   - Bitmask of lanes 0-19 to enable. 
*                           Bit 0 corresponds to lane 0.
*  lane_20_39_enable_msk  - Bitmask of lanes 20-39 to enable. 
*                           Bit 0 corresponds to lane 20.
*  lane_0_19_port_mode    - SERDES rate to configure for lanes 0-19
*  lane_20_39_port_mode   - SERDES rate to configure for lanes 20-39
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE void digi_serdes_t8_lanes_params_write(digi_handle_t            *digi_handle, 
                                               UINT32                    lane_0_19_enable_msk,
                                               UINT32                    lane_20_39_enable_msk,
                                               digi_serdes_port_mode_t   lane_0_19_port_mode, 
                                               digi_serdes_port_mode_t   lane_20_39_port_mode)

{
    UINT32 idx;

    PMC_ENTRY();

    /* save serdes rate */
    for (idx = 0; idx < (DIGI_SERDES_MAX_T8_SLICE); idx++)
    {
        if (((lane_0_19_enable_msk >> idx) & 0x1) != 0)
        {
            digi_handle->var.serdes_ctxt.t8_lanes_rate[idx] = lane_0_19_port_mode;
        }
        if (((lane_20_39_enable_msk >> idx) & 0x1) != 0)
        {
            digi_handle->var.serdes_ctxt.t8_lanes_rate[idx + (DIGI_SERDES_MAX_T8_SLICE)] = lane_20_39_port_mode;
        }
    }

    /* write serdes parameters such as: adaptations, tx swing, etc in device */
    digi_serdes_t8_params_wr(digi_handle, lane_0_19_enable_msk, lane_20_39_enable_msk);

    PMC_RETURN();
} /*digi_serdes_t8_lanes_params_write*/

/*******************************************************************************
* digi_serdes_c8_lanes_params_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Configure C8 serdes device with properties saved in context.
*
* INPUTS:
*  digi_handle     - DIGI device handle.
*  lanes_msk       - Bitmask of lanes 0-19 to configure. 
*                    Bit 0 corresponds to lane 0.
*  serdes_port_mode - SERDES rate to configure
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE void digi_serdes_c8_lanes_params_write(digi_handle_t             *digi_handle, 
                                               UINT32                     lanes_msk,
                                               digi_serdes_port_mode_t    serdes_port_mode)

{
    UINT32 idx;

    PMC_ENTRY();

    /* save serdes rate */
    for (idx = 0; idx < DIGI_SERDES_MAX_C8_IN_BP_SR; idx++)
    {
        if (((lanes_msk >> idx) & 0x1) != 0)
        {
            digi_handle->var.serdes_ctxt.c8_lanes_rate[idx] = serdes_port_mode;
        }
    }

    /* write serdes parameters such as: adaptations, tx swing, etc in device */
    digi_serdes_c8_params_wr(digi_handle, lanes_msk);

    /* update c8 serdes block internal state */
    c8_ext_ctxt_state_set(digi_handle->bp_sr_c8_handle,
                          lanes_msk,
                          TRUE);
    PMC_RETURN();
} /*digi_serdes_c8_lanes_params_write*/

/*******************************************************************************
* digi_serdes_s16_lanes_params_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Configure S16 serdes device with properties saved in context.
*
* INPUTS:
*  digi_handle     - DIGI device handle.
*  s16_intf_type   - S16 interface type
*  enable_msk      - enabled lanes Bitmask of lanes 0-15 to configure. 
*                    Bit 0 corresponds to lane 0.
*  disable_msk     - disabled lanes Bitmask of lanes 0-15 to configure. 
*                    Bit 0 corresponds to lane 0.
*  serdes_rate     - SERDES rate to configure
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE void digi_serdes_s16_lanes_params_write(digi_handle_t           *digi_handle, 
                                                hostmsg_s16_intf_type_t  s16_intf_type,
                                                UINT32                   enable_msk,
                                                UINT32                   disable_msk,
                                                digi_serdes_port_mode_t  serdes_rate )

{
    UINT32 nb_lanes;
    UINT32 i;
    digi_serdes_port_mode_t *serdes_rate_l;
    UINT16 tmp;
    UINT16 reg_value;

    PMC_ENTRY();

    /* save serdes rate */
    switch(s16_intf_type)
    {
    case HOSTMSG_S16_SERDES_INTF_LINE:
        nb_lanes = DIGI_SERDES_XFI_LANES_MAX;
        serdes_rate_l = digi_handle->var.serdes_ctxt.s16_line_lanes_rate;
        break;
    case HOSTMSG_S16_SERDES_INTF_SYS:
        nb_lanes = DIGI_SERDES_MAX_S16_IN_BP;
        serdes_rate_l = digi_handle->var.serdes_ctxt.s16_sys_lanes_rate;
        break;
    default:
        nb_lanes = 0;
        break;
    }
    
    for (i = 0; i < nb_lanes; i++)
    {
        if (((enable_msk >> i) & 0x1) != 0)
        {
            serdes_rate_l[i] = serdes_rate;
        }
        if (((disable_msk >> i) & 0x1) != 0)
        {
            serdes_rate_l[i] = LAST_DIGI_SERDES_PORT_MODE;
        }
    }
    

    /* write serdes parameters such as: adaptations, tx swing, etc in device */
    switch(s16_intf_type)
    {
    case HOSTMSG_S16_SERDES_INTF_LINE:
        digi_serdes_s16_line_params_wr(digi_handle, enable_msk);
        break;
    case HOSTMSG_S16_SERDES_INTF_SYS:
        digi_serdes_s16_sys_params_wr(digi_handle, enable_msk);
        break;
    default:
        break;
    }

    /* force s16 reset from top level layer, needed by crash/restart */  
    switch(s16_intf_type)
    {
    case HOSTMSG_S16_SERDES_INTF_LINE:
        
        reg_value =  tl_ic_field_S16_LINE_RESET_get(NULL, 
                                                    digi_handle);
        if (0 != enable_msk)
        {
            s16_ext_ctxt_state_set(digi_handle->s16_line_handle, enable_msk, TRUE);
        }
        if (0 != disable_msk)
        {
            tmp = (TL_IC_REG_RESET_CTRL3_BIT_S16_LINE_RESET_MSK >> TL_IC_REG_RESET_CTRL3_BIT_S16_LINE_RESET_OFF) & reg_value;

            tmp = (tmp | disable_msk);

            tl_ic_field_S16_LINE_RESET_set(NULL, 
                                           digi_handle, tmp);
            
            s16_ext_ctxt_state_set(digi_handle->s16_line_handle, disable_msk, FALSE);
        }
        /* update line serdes interrupt enable at DIGI top layer */
        reg_value = TL_IC_REG_RESET_CTRL3_BIT_S16_LINE_RESET_MSK & ~(reg_value | disable_msk);
        tl_ic_field_LINE_SERDES_INTR_EN_DEVICE_set(NULL, digi_handle, reg_value);
        tl_ic_field_LINE_SERDES_INTR_EN_PCIE_set(NULL, digi_handle, reg_value);        
        break;
    case HOSTMSG_S16_SERDES_INTF_SYS:
        reg_value = tl_ic_field_S16_SYS_RESET_get(NULL, 
                                                  digi_handle);

        if (0 != enable_msk)
        {
            s16_ext_ctxt_state_set(digi_handle->s16_sys_handle, enable_msk, TRUE);
        }
        if (0 != disable_msk)
        {
            tmp = (TL_IC_REG_RESET_CTRL1_BIT_S16_SYS_RESET_MSK >> TL_IC_REG_RESET_CTRL1_BIT_S16_SYS_RESET_OFF) & reg_value;
            
            tmp = (tmp | disable_msk); 
            
            tl_ic_field_S16_SYS_RESET_set(NULL,
                                          digi_handle, tmp);

            s16_ext_ctxt_state_set(digi_handle->s16_sys_handle, disable_msk, FALSE);
        }
        /* update sys serdes interrupt enable at DIGI top layer */
        reg_value = TL_IC_REG_RESET_CTRL1_BIT_S16_SYS_RESET_MSK & ~(reg_value | disable_msk);
        tl_ic_field_SYS_SERDES_INTR_EN_DEVICE_set(NULL, digi_handle, reg_value);
        tl_ic_field_SYS_SERDES_INTR_EN_PCIE_set(NULL, digi_handle, reg_value);
        
        break;
    default:
        break;
    }

    PMC_RETURN();
}/*digi_serdes_s16_lanes_params_write*/

/*******************************************************************************
* digi_serdes_s16_dc_coupling_in_ctxt_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Save S16 serdes coupling mode: DC or AC in context.
*
* INPUTS:
*  digi_handle         - DIGI device handle.
*  s16_intf_type       - S16 interface type
*  lanes_msk           - enabled lanes Bitmask of lanes 0-15 to configure. 
*                        Bit 0 corresponds to lane 0.
*  dc_coupling_enabled - when TRUE, DC coupling is enabled otherwise AC coupling is enabled
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_s16_dc_coupling_in_ctxt_set(digi_handle_t       *digi_handle, 
                                                          digi_serdes_intf_t   s16_intf_type,
                                                          UINT32               lanes_msk, 
                                                          BOOL8                dc_coupling_enabled)
    
{
    UINT32            nb_lanes_max = 0;
    UINT32 i; 
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    /* get maximum number of lanes associated to S16 interface */
    switch(s16_intf_type)
    {
    case DIGI_SERDES_LINE_INTF:
        nb_lanes_max = DIGI_SERDES_XFI_LANES_MAX;
        break;
    case DIGI_SERDES_SYSTEM_INTF:
        nb_lanes_max = DIGI_SERDES_MAX_S16_IN_BP;
        break;
    default:
        break;
    }

    if (0 != (lanes_msk >> nb_lanes_max))
    {
        rc = DIGI_ERR_INVALID_ARG;
    }

    /* save coupling mode in serdes context */
    for (i = 0; i < nb_lanes_max && PMC_SUCCESS == rc; i++)
    {
        if (((lanes_msk >> i) & 0x1) != 0)
        {         
            switch(s16_intf_type)
            {
            case DIGI_SERDES_LINE_INTF:
                digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[i].dc_coupling_enabled = dc_coupling_enabled;
                break;
            case DIGI_SERDES_SYSTEM_INTF:
                digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[i].dc_coupling_enabled = dc_coupling_enabled;
                break;
            default:
                break;
            }   
        }
    }
    
    PMC_RETURN(rc);
}/*digi_serdes_s16_dc_coupling_in_ctxt_set*/


/*******************************************************************************
* digi_serdes_t8_dc_coupling_in_ctxt_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Save T8 serdes coupling mode: DC or AC in context.
*
* INPUTS:
*  digi_handle          - DIGI device handle.
*  lane_0_19_msk        - enabled lanes Bitmask of lanes 0-19 to configure. 
*                         Bit 0 corresponds to lane 0.
*  lane_20_39_msk       - enabled lanes Bitmask of lanes 20-39 to configure. 
*                         Bit 0 corresponds to lane 20.
*  dc_coupling_enabled  - when TRUE, DC coupling is enabled otherwise AC coupling is enabled
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_t8_dc_coupling_in_ctxt_set(digi_handle_t   *digi_handle, 
                                                         UINT32           lane_0_19_msk,
                                                         UINT32           lane_20_39_msk,
                                                         BOOL8            dc_coupling_enabled)    
{
    UINT32 i; 
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    
    /* check parameters */
    if (0 != (lane_0_19_msk >> (DIGI_SERDES_MAX_T8_SLICE)))
    {
        rc = DIGI_ERR_INVALID_ARG;
    }
    else
    {
        if (0 != (lane_20_39_msk >> (DIGI_SERDES_MAX_T8_SLICE)))
        {
            rc = DIGI_ERR_INVALID_ARG;
        }
    }

    /* save coupling mode in serdes context */
    for (i = 0; i < (DIGI_SERDES_MAX_T8_SLICE)  && PMC_SUCCESS == rc; i++)
    {
        if (((lane_0_19_msk >> i) & 0x1) != 0)
        {
            digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[i].dc_coupling_enabled = dc_coupling_enabled;
        }
        if (((lane_20_39_msk >> i) & 0x1) != 0)
        {
            digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[i + (DIGI_SERDES_MAX_T8_SLICE)].dc_coupling_enabled = dc_coupling_enabled;
        }
    }
    
    PMC_RETURN(rc);
}/*digi_serdes_t8_dc_coupling_in_ctxt_set*/

/*******************************************************************************
* digi_serdes_s16_polarity_in_ctxt_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Save S16 serdes polarity in context.
*
* INPUTS:
*  digi_handle        - DIGI device handle.
*  s16_intf_type      - S16 interface type
*  lanes_msk          - enabled lanes bitmask of lanes 0-19 to configure. 
*                       Bit 0 corresponds to lane 0.
*  dir                - serdes lane direction: RX or TX or both. 
*  polarity_inversed  - when TRUE, polarity is inversed, otherwise it is reverted.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_s16_polarity_in_ctxt_set(digi_handle_t          *digi_handle, 
                                                       digi_serdes_intf_t      s16_intf_type,
                                                       UINT32                  lanes_msk,
                                                       util_global_direction_t dir,
                                                       BOOL8                   polarity_inversed)
    
{
    UINT32            nb_lanes_max = 0;
    UINT32 i; 
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    /* get maximum number of lanes associated to S16 interface */
    switch(s16_intf_type)
    {
    case DIGI_SERDES_LINE_INTF:
        nb_lanes_max = DIGI_SERDES_XFI_LANES_MAX;
        break;
    case DIGI_SERDES_SYSTEM_INTF:
        nb_lanes_max = DIGI_SERDES_MAX_S16_IN_BP;
        break;
    default:
        break;
    }

    if (0 != (lanes_msk >> nb_lanes_max))
    {
        rc = DIGI_ERR_INVALID_ARG;
    }

    /* save polarity in serdes context */
    for (i = 0; i < nb_lanes_max && PMC_SUCCESS == rc; i++)
    {
        if (((lanes_msk >> i) & 0x1) != 0)
        {         
            switch(s16_intf_type)
            {
            case DIGI_SERDES_LINE_INTF:
                digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[i].polarity_inversed = 
                    util_global_direction_value_compute(digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[i].polarity_inversed, dir, polarity_inversed);
                break;
            case DIGI_SERDES_SYSTEM_INTF:
                digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[i].polarity_inversed = 
                    util_global_direction_value_compute(digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[i].polarity_inversed, dir, polarity_inversed);
                break;
            default:
                break;
            }   
        }
    }
    
    PMC_RETURN(rc);
}/*digi_serdes_s16_polarity_in_ctxt_set*/

/*******************************************************************************
* digi_serdes_c8_polarity_in_ctxt_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Save C8 serdes polarity in context.
*
* INPUTS:
*  digi_handle        - DIGI device handle.
*  lanes_msk          - enabled lanes Bitmask of lanes 0-19 to configure. 
*                       Bit 0 corresponds to lane 0.
*  dir                - serdes logical lane direction. 
*  polarity_inversed  - when TRUE, polarity is inversed, otherwise it is reverted.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_c8_polarity_in_ctxt_set(digi_handle_t          *digi_handle, 
                                                      UINT32                  lanes_msk,  
                                                      util_global_direction_t dir,
                                                      BOOL8                   polarity_inversed)
    
{
    UINT32 i; 
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    if (0 != (lanes_msk >> DIGI_SERDES_MAX_C8_IN_BP_SR))
    {
        rc = DIGI_ERR_INVALID_ARG;
    }

    /* save polarity in serdes context */
    for (i = 0; i < DIGI_SERDES_MAX_C8_IN_BP_SR && PMC_SUCCESS == rc; i++)
    {
        if (((lanes_msk >> i) & 0x1) != 0)
        {                     
            digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[i].polarity_inversed =
                util_global_direction_value_compute(digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[i].polarity_inversed, dir, polarity_inversed);
        }
    }
    
    PMC_RETURN(rc);
}/*digi_serdes_c8_polarity_in_ctxt_set*/


/*******************************************************************************
* digi_serdes_t8_polarity_in_ctxt_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Save T8 serdes polarity in context.
*
* INPUTS:
*  digi_handle          - DIGI device handle.
*  lane_0_19_msk        - enabled lanes Bitmask of lanes 0-19 to configure. 
*                         Bit 0 corresponds to lane 0.
*  lane_20_39_msk       - enabled lanes Bitmask of lanes 20-39 to configure. 
*                         Bit 0 corresponds to lane 20.
*  dir                  - serdes logical lane direction. 
*  polarity_inversed    - when TRUE, polarity is inversed, otherwise it is reverted.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_t8_polarity_in_ctxt_set(digi_handle_t           *digi_handle, 
                                                      UINT32                   lane_0_19_msk,
                                                      UINT32                   lane_20_39_msk,
                                                      util_global_direction_t  dir,
                                                      BOOL8                    polarity_inversed)    
{
    UINT32 i; 
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    
    /* check parameters */
    if (0 != (lane_0_19_msk >> (DIGI_SERDES_MAX_T8_SLICE)))
    {
        rc = DIGI_ERR_INVALID_ARG;
    }
    else
    {
        if (0 != (lane_20_39_msk >> (DIGI_SERDES_MAX_T8_SLICE)))
        {
            rc = DIGI_ERR_INVALID_ARG;
        }
    }

    /* save polarity in serdes context */
    for (i = 0; i < (DIGI_SERDES_MAX_T8_SLICE)  && PMC_SUCCESS == rc; i++)
    {
        if (((lane_0_19_msk >> i) & 0x1) != 0)
        {
            digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[i].polarity_inversed =
                util_global_direction_value_compute(digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[i].polarity_inversed, dir, polarity_inversed);
        }
        if (((lane_20_39_msk >> i) & 0x1) != 0)
        {
            digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[i + (DIGI_SERDES_MAX_T8_SLICE)].polarity_inversed =
                util_global_direction_value_compute(digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[i + (DIGI_SERDES_MAX_T8_SLICE)].polarity_inversed, dir, polarity_inversed);
        }
    }
    
    PMC_RETURN(rc);
}/*digi_serdes_t8_polarity_in_ctxt_set*/


/*******************************************************************************
* digi_serdes_sfi51_polarity_in_ctxt_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Save T8 serdes polarity in context.
*
* INPUTS:
*  digi_handle          - DIGI device handle.
*  bus_id               - SFI51 bus identifier.
*  serdes_id            - serdes lane identifier.
*  dir                  - serdes logical lane direction. 
*  polarity_inversed    - when TRUE, polarity is inversed, otherwise it is reverted.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_sfi51_polarity_in_ctxt_set(digi_handle_t           *digi_handle, 
                                                         UINT32                   bus_id,
                                                         UINT32                   serdes_id,
                                                         util_global_direction_t  dir,
                                                         BOOL8                    polarity_inversed)    
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT((NULL != digi_handle), DIGI_ERR_NULL_HANDLE, 0, 0);
    /* check that pin_id is in range [0..17]*/
    if (DIGI_NUM_SFI51_PORTS_MAX <= bus_id ||
        SFI51_D8_TX_MAX_NUM_SERDES <= serdes_id)
    {
        rc = DIGI_ERR_INVALID_ARG;
    }

    if (PMC_SUCCESS == rc)
    {
        digi_handle->var.serdes_ctxt.serdes_cfg->sfi51_cfg[bus_id].sfi51_d8_cfg[serdes_id].polarity_inversed = util_global_direction_value_compute(digi_handle->var.serdes_ctxt.serdes_cfg->sfi51_cfg[bus_id].sfi51_d8_cfg[serdes_id].polarity_inversed, dir, polarity_inversed);    
    }
    
    PMC_RETURN(rc);
}/*digi_serdes_sgi51_polarity_in_ctxt_set*/

/*******************************************************************************
* digi_serdes_c8_params_wr
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Load C8 serdes parameters in device.
*
* INPUTS:
*  digi_handle        - DIGI device handle.
*  lanes_msk          - enabled lanes Bitmask of lanes 0-7 to configure. 
*                       Bit 0 corresponds to lane 0.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_c8_params_wr(digi_handle_t            *digi_handle, 
                                          UINT32                   lanes_msk)
    
{
    UINT32 i; 
    UINT32 j; 
    BOOL8 adapt_params_found = FALSE;
    BOOL8 tx_swing_found = FALSE;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    if (0 != (lanes_msk >> DIGI_SERDES_MAX_C8_IN_BP_SR))
    {
        rc = DIGI_ERR_INVALID_ARG;
    }

    /* configure Serdes coupling mode  */
    for (i = 0; i < DIGI_SERDES_MAX_C8_IN_BP_SR && PMC_SUCCESS == rc; i++)
    {
        /* set polarity in device */
        if (((lanes_msk >> i) & 0x1) != 0)
        {
            /* set polarity */
            c8_polarity_cfg(digi_handle->bp_sr_c8_handle, 
                            i, 
                            digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[i].polarity_inversed,
                            TRUE);
   
            
            /* find the lane rate and tx swing levels parameters */
            for (j = 0; j < DIGI_SERDES_C8_NUM_RATES_MAX &&
                     PMC_SUCCESS == rc; j++)
            {
                /* extract selected lane id */
                if (digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[i].c8_tx_swing_levels[j].port_mode == 
                    digi_handle->var.serdes_ctxt.c8_lanes_rate[i] &&
                    TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[i].c8_tx_swing_levels[j].valid)
                {
                    rc = c8_tx_swing_emphasis_levels_set(digi_handle->bp_sr_c8_handle, i, &digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[i].c8_tx_swing_levels[j].levels);
                    tx_swing_found = TRUE;
                }
                
                if (PMC_SUCCESS == rc)
                {
                    if (digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[i].c8_adapt_params[j].port_mode == 
                        digi_handle->var.serdes_ctxt.c8_lanes_rate[i] &&
                        TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[i].c8_adapt_params[j].valid)
                    {
                        rc = digi_serdes_c8_write_adaptation_values(
                            digi_handle, i,
                            &digi_handle->var.serdes_ctxt.serdes_cfg->c8_cfg[i].c8_adapt_params[j]);
                        if (PMC_SUCCESS == rc)
                        {
                            adapt_params_found = TRUE;
                        }
                    }
                }
            }            
        }
    }

    if (PMC_SUCCESS == rc && FALSE == adapt_params_found)
    {
        rc = DIGI_ERR_SERDES_ADAPT_PARAMS_NOT_FOUND;
    }
    
    if (PMC_SUCCESS == rc && FALSE == tx_swing_found)
    {
        rc = DIGI_ERR_SERDES_TX_SWING_LEVELS_NOT_FOUND;
    }

    PMC_RETURN(rc);
}/*digi_serdes_c8_params_wr*/

/*******************************************************************************
* digi_serdes_s16_line_params_wr
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Load S16 line serdes parameters in device.
*
* INPUTS:
*  digi_handle             - DIGI device handle.
*  lanes_msk               - enabled lanes Bitmask of lanes 0-11 to configure. 
*                            Bit 0 corresponds to lane 0.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_s16_line_params_wr(digi_handle_t            *digi_handle, 
                                                UINT32                   lanes_msk)
    
{
    UINT32 i; 
    UINT32 j; 
    BOOL8 adapt_params_found = FALSE;
    BOOL8 tx_swing_found = FALSE;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    if (0 != (lanes_msk >> DIGI_SERDES_XFI_LANES_MAX))
    {
        rc = DIGI_ERR_INVALID_ARG;
    }

    /* configure Serdes coupling mode  */
    for (i = 0; i < DIGI_SERDES_XFI_LANES_MAX && PMC_SUCCESS == rc; i++)
    {
        /* set polarity in device */
        if (((lanes_msk >> i) & 0x1) != 0)
        {
            /* set coulping mode */
            rc = s16_dc_coupling_set(digi_handle->s16_line_handle, 
                                     i, 
                                     digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[i].dc_coupling_enabled);  
            if (PMC_SUCCESS == rc)
            {
                /* set polarity */
                s16_polarity_cfg(digi_handle->s16_line_handle, 
                                 i, 
                                 digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[i].polarity_inversed,
                                 TRUE); 
            }
            
            /* find the lane rate and tx swing levels parameters */
            for (j = 0; j < DIGI_SERDES_S16_NUM_RATES_MAX &&
                     PMC_SUCCESS == rc; j++)
            {
                /* extract selected lane id */
                if (digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[i].s16_line_tx_swing_levels[j].port_mode == 
                    digi_handle->var.serdes_ctxt.s16_line_lanes_rate[i] &&
                    TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[i].s16_line_tx_swing_levels[j].valid)
                {
                    rc = s16_tx_swing_emphasis_levels_set(
                        digi_handle->s16_line_handle,
                        i, 
                        &digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[i].s16_line_tx_swing_levels[j].levels);
                    tx_swing_found = TRUE;
                }
                if (PMC_SUCCESS == rc)
                {
                    if (digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[i].s16_line_adapt_params[j].port_mode == 
                        digi_handle->var.serdes_ctxt.s16_line_lanes_rate[i] &&
                        TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[i].s16_line_adapt_params[j].valid)
                    {
                        rc = digi_serdes_s16_write_adaptation_values(
                            digi_handle, HOSTMSG_S16_SERDES_INTF_LINE,
                            i,
                            &digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[i].s16_line_adapt_params[j]);
                        if (PMC_SUCCESS == rc)
                        {
                            adapt_params_found = TRUE;
                        }
                    }
                }
            }            
        }
    }

    if (PMC_SUCCESS == rc && FALSE == adapt_params_found)
    {
        rc = DIGI_ERR_SERDES_ADAPT_PARAMS_NOT_FOUND;
    }
    
    if (PMC_SUCCESS == rc && FALSE == tx_swing_found)
    {
        rc = DIGI_ERR_SERDES_TX_SWING_LEVELS_NOT_FOUND;
    }

    PMC_RETURN(rc);
}/*digi_serdes_s16_line_params_wr*/

/*******************************************************************************
* digi_serdes_s16_sys_params_wr
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Load S16 sys serdes parameters in device.
*
* INPUTS:
*  digi_handle          - DIGI device handle.
*  lanes_msk            - enabled lanes Bitmask of lanes 0-15 to configure. 
*                         Bit 0 corresponds to lane 0.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_s16_sys_params_wr(digi_handle_t            *digi_handle, 
                                               UINT32                   lanes_msk)
    
{
    UINT32 i; 
    UINT32 j; 
    BOOL8 adapt_params_found = FALSE;
    BOOL8 tx_swing_found = FALSE;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    if (0 != (lanes_msk >> DIGI_SERDES_MAX_S16_IN_BP))
    {
        rc = DIGI_ERR_INVALID_ARG;
    }

    /* configure Serdes coupling mode  */
    for (i = 0; i < DIGI_SERDES_MAX_S16_IN_BP && PMC_SUCCESS == rc; i++)
    {
        /* set polarity in device */
        if (((lanes_msk >> i) & 0x1) != 0)
        {
            /* set coulping mode */
            rc = s16_dc_coupling_set(digi_handle->s16_sys_handle, 
                                     i, 
                                     digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[i].dc_coupling_enabled);  
            if (PMC_SUCCESS == rc)
            {
                /* set polarity */
                s16_polarity_cfg(digi_handle->s16_sys_handle, 
                                 i, 
                                 digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[i].polarity_inversed,
                                 TRUE); 
            }

            /* find the lane rate and tx swing levels parameters */
            for (j = 0; j < DIGI_SERDES_S16_NUM_RATES_MAX &&
                     PMC_SUCCESS == rc; j++)
            {
                /* extract selected lane id */
                if (digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[i].s16_sys_tx_swing_levels[j].port_mode == 
                    digi_handle->var.serdes_ctxt.s16_sys_lanes_rate[i] &&
                    TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[i].s16_sys_tx_swing_levels[j].valid)
                {
                    rc = s16_tx_swing_emphasis_levels_set(
                        digi_handle->s16_sys_handle,
                        i, 
                        &digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[i].s16_sys_tx_swing_levels[j].levels);
                    tx_swing_found = TRUE;
                }
                
                if (PMC_SUCCESS == rc)
                {
                    if (digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[i].s16_sys_adapt_params[j].port_mode == 
                        digi_handle->var.serdes_ctxt.s16_sys_lanes_rate[i] &&
                        TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[i].s16_sys_adapt_params[j].valid)
                    {
                        rc = digi_serdes_s16_write_adaptation_values(
                            digi_handle, HOSTMSG_S16_SERDES_INTF_SYS,
                            i,
                            &digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[i].s16_sys_adapt_params[j]);
                        if (PMC_SUCCESS == rc)
                        {
                            adapt_params_found = TRUE;
                        }
                    }
                }
            }            
        }
    }

    if (PMC_SUCCESS == rc && FALSE == adapt_params_found)
    {
        rc = DIGI_ERR_SERDES_ADAPT_PARAMS_NOT_FOUND;
    }
    
    if (PMC_SUCCESS == rc && FALSE == tx_swing_found)
    {
        rc = DIGI_ERR_SERDES_TX_SWING_LEVELS_NOT_FOUND;
    }

    PMC_RETURN(rc);
}/*digi_serdes_s16_sys_params_wr*/

/*******************************************************************************
* digi_serdes_t8_params_wr
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Load T8 serdes parameters in device.
*
* INPUTS:
*  digi_handle          - DIGI device handle.
*  lane_0_19_msk        - enabled lanes Bitmask of lanes 0-19 to configure. 
*                         Bit 0 corresponds to lane 0.
*  lane_20_39_msk       - enabled lanes Bitmask of lanes 20-39 to configure. 
*                         Bit 0 corresponds to lane 20.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_t8_params_wr(digi_handle_t            *digi_handle, 
                                          UINT32                    lane_0_19_msk, 
                                          UINT32                    lane_20_39_msk)
    
{
    UINT32 i; 
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    if (0 != (lane_0_19_msk >> (DIGI_SERDES_MAX_T8_SLICE)))
    {
        rc = DIGI_ERR_INVALID_ARG;
    }
    else
    {
        if (0 != (lane_20_39_msk >> (DIGI_SERDES_MAX_T8_SLICE)))
        {
            rc = DIGI_ERR_INVALID_ARG;
        }
    }

    /* write serdes parameters in device */
    for (i = 0; i < (DIGI_SERDES_MAX_T8_SLICE) && PMC_SUCCESS == rc; i++)
    {
        /* for lanes 0 to 19 */
        if (((lane_0_19_msk >> i) & 0x1) != 0)
        {
            digi_serdes_one_t8_params_wr(digi_handle, i);
        }
        /* for lanes 20 to 39 */
        if (((lane_20_39_msk >> i) & 0x1) != 0)
        {
            digi_serdes_one_t8_params_wr(digi_handle, i + (DIGI_SERDES_MAX_T8_SLICE));
        }
    }


    PMC_RETURN(rc);
}/*digi_serdes_t8_params_wr*/

/*******************************************************************************
* digi_serdes_one_t8_params_wr
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Write prorperties for a group of T8 serdes.
*
* INPUTS:
*  digi_handle          - DIGI device handle.
*  lane                 - enabled lane
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_one_t8_params_wr(digi_handle_t            *digi_handle, 
                                              UINT32                    lane)
    
{
    UINT32 j;
    UINT8 serdes_id;
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL8 tx_swing_found = FALSE;
    BOOL8 adapt_params_found = FALSE;

    PMC_ENTRY();

    if (DIGI_SERDES_MAX_T8_E1_IN_BP_LR <= lane)
    {
        rc = DIGI_ERR_INVALID_ARG;
    }

    if (PMC_SUCCESS == rc)
    {
        serdes_id = digi_serdes_map_t8_serdes(lane);

        /* set coulping mode */
        rc = t8_dc_coupling_set(digi_handle->bp_lr_t8_handle[DIGI_SERDES_T8_SLICE_ID_GET(lane)], 
                                serdes_id, 
                                digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].dc_coupling_enabled);  
        
        if (PMC_SUCCESS == rc)
        {
            /* set polarity */
            t8_polarity_cfg(digi_handle->bp_lr_t8_handle[DIGI_SERDES_T8_SLICE_ID_GET(lane)], 
                            serdes_id, 
                            digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].polarity_inversed,
                            TRUE);
        }
        
        /* find the lane rate and tx swing levels parameters */
        for (j = 0; j < DIGI_SERDES_T8_NUM_RATES_MAX &&
                 PMC_SUCCESS == rc; j++)
        {
            /* extract selected lane id */
            if (digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].t8_tx_swing_levels[j].port_mode == 
                digi_handle->var.serdes_ctxt.t8_lanes_rate[lane] &&
                TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].t8_tx_swing_levels[j].valid)
            {
                rc = t8_tx_swing_emphasis_levels_set(digi_handle->bp_lr_t8_handle[DIGI_SERDES_T8_SLICE_ID_GET(lane)
                                                         ], 
                                                     serdes_id, 
                                                     &digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].t8_tx_swing_levels[j].levels);
                tx_swing_found = TRUE;
            }
        
            if (PMC_SUCCESS == rc)
            {
                if (digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].t8_adapt_params[j].port_mode == 
                    digi_handle->var.serdes_ctxt.t8_lanes_rate[lane] &&
                    TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].t8_adapt_params[j].valid)
                {
                    rc = digi_serdes_t8_write_adaptation_values(
                        digi_handle, lane,
                        &digi_handle->var.serdes_ctxt.serdes_cfg->t8_cfg[lane].t8_adapt_params[j]);
                    if (PMC_SUCCESS == rc)
                    {
                        adapt_params_found = TRUE;
                    }
                }
            }            
        }

        /* set serdes state */
        t8_ext_ctxt_state_set(digi_handle->bp_lr_t8_handle[DIGI_SERDES_T8_SLICE_ID_GET(lane)], 0 == serdes_id ? 1 : 2, TRUE);
    }

    if (PMC_SUCCESS == rc && FALSE == adapt_params_found)
    {
        rc = DIGI_ERR_SERDES_ADAPT_PARAMS_NOT_FOUND;
    }
    
    if (PMC_SUCCESS == rc && FALSE == tx_swing_found)
    {
        rc = DIGI_ERR_SERDES_TX_SWING_LEVELS_NOT_FOUND;
    }
    
    PMC_RETURN(rc);
}/*digi_serdes_one_t8_params_wr*/

/*******************************************************************************
* digi_serdes_sfi51_params_wr
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Load SFI51 bus D8 serdes parameters in device. Parameters are retrieved from 
*  serdes context.
*
* INPUTS:
*  digi_handle          - DIGI device handle.
*  enable_msk           - SFI51 bus id enable mask.
*  disable_msk          - SFI51 bus id disable mask.
*  serdes_port_mode     - serdes rate
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_sfi51_params_wr(digi_handle_t            *digi_handle, 
                                             UINT32                    enable_msk,
                                             UINT32                    disable_msk,
                                             digi_serdes_port_mode_t   serdes_port_mode)
{
    UINT32 itr;
    UINT32 bus_id;
    UINT32 serdes_itr;
    UINT32 rate_itr;
    const UINT8 sfi51_tx_serdes_pair[] = DIGI_SFI51_TX_SERDES_PAIR_LIST;
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL8 found;
    BOOL8 inv_rx = FALSE;
    BOOL8 inv_tx = FALSE;
    UINT32 tmp;

    PMC_ENTRY();

    /* for each enabled bus */
    for (itr = 0; itr < DIGI_NUM_SFI51_PORTS_MAX && PMC_SUCCESS == rc; itr++)
    {
        if (0 != (0x1 & (enable_msk >> itr)))
        {
            bus_id = itr;

            /* update SFI51 bus current rate */
            digi_handle->var.serdes_ctxt.sfi51_lanes_rate[bus_id] = serdes_port_mode;

            /* write serdes parameters in device */
            for (serdes_itr = 0; serdes_itr < SFI51_D8_TX_MAX_NUM_SERDES && PMC_SUCCESS == rc; serdes_itr++)
            {
                found = FALSE;
                /* lookup tx swing level associated to serdes pin in context */
                for (rate_itr = 0; 
                     rate_itr < DIGI_SERDES_SFI51_D8_NUM_RATES_MAX && PMC_SUCCESS == rc; 
                     rate_itr++)
                {
                    if (TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->sfi51_cfg[bus_id].sfi51_d8_cfg[serdes_itr].d8_cfg[rate_itr].d8_tx_swing_level.valid &&
                        digi_handle->var.serdes_ctxt.sfi51_lanes_rate[bus_id] == digi_handle->var.serdes_ctxt.serdes_cfg->sfi51_cfg[bus_id].sfi51_d8_cfg[serdes_itr].d8_cfg[rate_itr].d8_tx_swing_level.port_mode)
                    {                    
                        rc = sfi51_d8_tx_18x_swing_level_set(digi_handle->sfi51_d8_tx_18x_handle[bus_id], sfi51_tx_serdes_pair[serdes_itr], digi_handle->var.serdes_ctxt.serdes_cfg->sfi51_cfg[bus_id].sfi51_d8_cfg[serdes_itr].d8_cfg[rate_itr].d8_tx_swing_level.tx_swing_level);
                        PMC_LOG_TRACE("in %s -> %d %d %d %d\n", __FUNCTION__, bus_id, serdes_itr, rate_itr, digi_handle->var.serdes_ctxt.serdes_cfg->sfi51_cfg[bus_id].sfi51_d8_cfg[serdes_itr].d8_cfg[rate_itr].d8_tx_swing_level.tx_swing_level);
                        found = TRUE;
                    }
                }
                /* if no tx swing is found, write tx swing default value */
                if (PMC_SUCCESS == rc && FALSE == found)
                {               
                    rc = sfi51_d8_tx_18x_swing_level_set(digi_handle->sfi51_d8_tx_18x_handle[bus_id], sfi51_tx_serdes_pair[serdes_itr], SFI51_D8_TX_2X_TX_SWING_LEVEL_DEFAULT_VALUE);  
                }

                /* set polarity */
                
                switch (digi_handle->var.serdes_ctxt.serdes_cfg->sfi51_cfg[bus_id].sfi51_d8_cfg[serdes_itr].polarity_inversed)
                {
                case UTIL_GLOBAL_DIR_TX:
                    inv_tx = TRUE;
                    break;
                case UTIL_GLOBAL_DIR_RX:
                    inv_rx = TRUE;
                    break;
                case UTIL_GLOBAL_DIR_BOTH:
                    inv_rx = TRUE;
                    inv_tx = TRUE;
                    break;
                default:
                    break;
                }
                /* Configure serdes poalrity */
                if (PMC_SUCCESS == rc)
                {
                    rc = sfi51_d8_tx_18x_polarity_cfg(digi_handle->sfi51_d8_tx_18x_handle[bus_id], 
                                                      serdes_itr, inv_tx);   
                }
                    
                if (PMC_SUCCESS == rc)
                {
                    rc = sfi51_d8_rx_18x_polarity_cfg(digi_handle->sfi51_d8_rx_18x_handle[bus_id], 
                                                      serdes_itr, inv_rx);        
                }
            }    

            /* Configure serdes squelch default value */
            if (PMC_SUCCESS == rc)
            {
                sfi51_d8_tx_18x_squelch_default_set(digi_handle->sfi51_d8_tx_18x_handle[bus_id]);
            }        
        }
    }


    /* force sfi51 reset from top level layer, needed by crash/restart */  
    for (itr = 0; itr < DIGI_NUM_SFI51_PORTS_MAX; itr += 1)
    {
        if (0x1 == (0x1 & (enable_msk >> itr)))
        {
            sfi51_d8_tx_18x_ext_ctxt_state_set(digi_handle->sfi51_d8_tx_18x_handle[itr], TRUE);
            rc = sfi51_d8_rx_18x_ext_ctxt_state_set(digi_handle->sfi51_d8_rx_18x_handle[itr], TRUE);
        }
        if (PMC_SUCCESS == rc)
        {
            if (0x1 == (0x1 & (disable_msk >> itr)))
            {
                /* update SFI51 bus current rate */
                digi_handle->var.serdes_ctxt.sfi51_lanes_rate[itr] = LAST_DIGI_SERDES_PORT_MODE;            
                sfi51_d8_tx_18x_ext_ctxt_state_set(digi_handle->sfi51_d8_tx_18x_handle[itr], FALSE);
                rc = sfi51_d8_rx_18x_ext_ctxt_state_set(digi_handle->sfi51_d8_rx_18x_handle[itr], FALSE);
            }
        }
    }
    if (PMC_SUCCESS == rc)
    {
        if (0 != disable_msk)
        {
            tmp = (TL_IC_REG_RESET_CTRL3_BIT_SFI51_TX_RESET_MSK >> TL_IC_REG_RESET_CTRL3_BIT_SFI51_TX_RESET_OFF) & 
                tl_ic_field_SFI51_TX_RESET_get(NULL,
                                               digi_handle);
                
            tmp = (tmp | disable_msk); 
                
            tl_ic_field_SFI51_TX_RESET_set(NULL, 
                                           digi_handle, tmp);
                
            tmp =  (TL_IC_REG_RESET_CTRL3_BIT_SFI51_RX_RESET_MSK >> TL_IC_REG_RESET_CTRL3_BIT_SFI51_RX_RESET_OFF) & 
                tl_ic_field_SFI51_RX_RESET_get(NULL, 
                                               digi_handle);
                
            tmp = (tmp | disable_msk); 
                
            tl_ic_field_SFI51_RX_RESET_set(NULL, 
                                           digi_handle, tmp);
        }
    }

    PMC_RETURN(rc);
}/*digi_serdes_sfi51_params_wr*/

/*******************************************************************************
* digi_serdes_ge_params_wr
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Load GE interface bus D8 serdes parameters in device. Parameters are retrieved 
*  from serdes context.
*
* INPUTS:
*  digi_handle          - DIGI device handle.
*  d8_intf_type         - GE interface type
*  enable               - serdes new status: enabled or disabled
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_ge_params_wr(digi_handle_t               *digi_handle, 
                                          hostmsg_d8_intf_type_enum    d8_intf_type,
                                          BOOL8                        enable)
{
    digi_ge_serdes_id_t ge_intf;
    d8_tx_2x_handle_t *d8_tx_2x_handle;
    d8_rx_2x_handle_t *d8_rx_2x_handle;
    UINT8 tx_serdes_id;
    UINT8 rx_serdes_id;
    digi_ge_serdes_ctxt_t *ge_serdes_ctxt;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    
    switch(d8_intf_type)
    {
    case HOSTMSG_D8_SERDES_INTF_OHP_0:
        ge_intf = DIGI_SERDES_INTF_OHP_0;
        break;
    case HOSTMSG_D8_SERDES_INTF_OHP_1:
        ge_intf = DIGI_SERDES_INTF_OHP_1;
        break;
    case HOSTMSG_D8_SERDES_INTF_OHP_2:
        ge_intf = DIGI_SERDES_INTF_OHP_2;
        break;
    case HOSTMSG_D8_SERDES_INTF_RI_RCP:
        ge_intf = DIGI_SERDES_INTF_RI_RCP;
        break;
    case HOSTMSG_D8_SERDES_INTF_DI_RCP:
        ge_intf = DIGI_SERDES_INTF_DI_RCP;
        break;
    case HOSTMSG_D8_SERDES_INTF_ENET_MGMT:
        ge_intf = DIGI_SERDES_INTF_EMGMT;
        break;
    default:
        rc = DIGI_ERR_INVALID_ARG;
        break;
    }


    /* retrieve d8_tx_2x handle associated to ge serdes */
    if (PMC_SUCCESS == rc)
    {
        rc = digi_ge_d8_tx_2x_handle_get(digi_handle, ge_intf, &d8_tx_2x_handle, &tx_serdes_id);
    }

    /* retrieve d8_rx_2x handle associated to ge serdes */
    if (PMC_SUCCESS == rc)
    {
        rc = digi_ge_d8_rx_2x_handle_get(digi_handle, ge_intf, &d8_rx_2x_handle, &rx_serdes_id);
    }

    if (PMC_SUCCESS == rc)
    {
        rc = digi_ge_serdes_ctxt_get(digi_handle,
                                     ge_intf,
                                     &ge_serdes_ctxt);
    }

    if (PMC_SUCCESS == rc)
    {
        /* update ge serdes provisioning state */
        ge_serdes_ctxt->enabled = enable;
    }

    /* write serdes parameters in device if there there are any written in serdes context*/
    if (PMC_SUCCESS == rc && TRUE == enable)
    {
        if (TRUE == digi_handle->var.serdes_ctxt.serdes_cfg->ge_cfg[(UINT32)ge_intf].d8_cfg.d8_tx_swing_level.valid)
        {                    
            rc =  d8_tx_2x_tx_swing_level_set(d8_tx_2x_handle,
                                              tx_serdes_id,
                                              digi_handle->var.serdes_ctxt.serdes_cfg->ge_cfg[(UINT32)ge_intf].d8_cfg.d8_tx_swing_level.tx_swing_level);        
            PMC_LOG_TRACE("Set tx swing for GE serdes %d %d\n", ge_intf, (UINT32)digi_handle->var.serdes_ctxt.serdes_cfg->                         ge_cfg[(UINT32)ge_intf].d8_cfg.d8_tx_swing_level.tx_swing_level);
        }
    }

    if (PMC_SUCCESS == rc)
    {
        rc = d8_tx_2x_ext_ctxt_state_set(d8_tx_2x_handle, 0 == tx_serdes_id ? 1 : 2, enable);
    }

    if (PMC_SUCCESS == rc)
    {
        rc = d8_rx_2x_ext_ctxt_state_set(d8_rx_2x_handle, 0 == rx_serdes_id ? 1 : 2, enable);
    }

    PMC_RETURN(rc);
}/*digi_serdes_ge_params_wr*/

/*******************************************************************************
* digi_serdes_d8_tx_dclk_validate_params_and_instance_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Helper function to validate the parameters and obtain the SFI51. instance
*
* INPUTS:
*  digi_handle                   - DIGI device handle.
*  port_ctxt_ptr                 - A pointer to the SFI51. SERDES instance.
*
* OUTPUTS:
*  index                         - The instance/index of the SFI5.1 interface.
*
* RETURNS:
*   PMC_SUCCESS - on success, a descriptive error code otherwise.
*
* NOTES:
*   index is only valid upon return of PMC_SUCCESS.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_d8_tx_dclk_validate_params_and_instance_get(digi_handle_t* digi_handle,
                                                                          digi_serdes_port_t* port_ctxt_ptr,
                                                                          UINT8* index)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    digi_serdes_port_def_t* port_def_ptr;
    UINT8 i; 
    
    PMC_ENTRY();

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != port_ctxt_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    port_def_ptr = (digi_serdes_port_def_t*)port_ctxt_ptr;

    PMC_ASSERT(port_def_ptr->intf == DIGI_SERDES_SFI_51_INTF, DIGI_ERR_INVALID_ARG, 0, 0);

    for (i = 0; i < DIGI_NUM_SFI51_PORTS_MAX; i++) {

        if (port_def_ptr == &(digi_handle->var.port_ctxt.sfi51_port_ctxt[i])) {

            *index = i;
            rc = PMC_SUCCESS;
            break;
        }
    }

    PMC_RETURN(rc);

} /* digi_serdes_d8_tx_dclk_validate_params_and_instance_get */


/*******************************************************************************
* digi_serdes_s16_adaptation_default_values_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Helper function fill up a s16_serdes_adapt_object with default values defined in register doc.
*
* INPUTS:
*  None.
*
* OUTPUTS:
*  serdes_s16_adapt_objects_ptr - initialized S16 adaptation values object
*
* RETURNS:
*   PMC_SUCCESS - on success, a descriptive error code otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_serdes_s16_adaptation_default_values_get(serdes_s16_adapt_objects_t  *serdes_s16_adapt_objects_ptr)
{

    PMC_ENTRY();

    PMC_ASSERT(NULL != serdes_s16_adapt_objects_ptr, DIGI_ERR_INVALID_ARG, 0, 0);

    serdes_s16_adapt_objects_ptr->dp_ffe_a = 0;
    serdes_s16_adapt_objects_ptr->dp_ffe_b = 0;
    serdes_s16_adapt_objects_ptr->dp_ffe_c = 0;
    serdes_s16_adapt_objects_ptr->tr_ffe_a = 0;
    serdes_s16_adapt_objects_ptr->tr_ffe_b = 0;
    serdes_s16_adapt_objects_ptr->dfe_coef0 = 0;
    serdes_s16_adapt_objects_ptr->dfe_coef1 = 0;
    serdes_s16_adapt_objects_ptr->dfe_coef2 = 0;
    serdes_s16_adapt_objects_ptr->pga_gain = 0x10;
    serdes_s16_adapt_objects_ptr->pga_offset = 0;
    serdes_s16_adapt_objects_ptr->target_amp = 0x27;
    serdes_s16_adapt_objects_ptr->adc_map_scale = 0x8;    
 
    PMC_RETURN();
}/* digi_serdes_s16_adaptation_default_values_get */

/*******************************************************************************
* digi_serdes_t8_adaptation_default_values_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Helper function fill up a t8_serdes_adapt_object with default values defined in register doc.
*
* INPUTS:
*  None.
*
* OUTPUTS:
*  serdes_t8_adapt_objects_ptr - initialized T8 adaptation values object
*
* RETURNS:
*   PMC_SUCCESS - on success, a descriptive error code otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_serdes_t8_adaptation_default_values_get(serdes_t8_adapt_objects_t  *serdes_t8_adapt_objects_ptr)
{

    PMC_ENTRY();

    PMC_ASSERT(NULL != serdes_t8_adapt_objects_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    
    serdes_t8_adapt_objects_ptr->dp_ffe_a = 0;
    serdes_t8_adapt_objects_ptr->dp_ffe_b = 0;
    serdes_t8_adapt_objects_ptr->dp_ffe_c = 0;
    serdes_t8_adapt_objects_ptr->tr_ffe_a = 0;
    serdes_t8_adapt_objects_ptr->tr_ffe_b = 0;
    serdes_t8_adapt_objects_ptr->dfe_coef0 = 0;
    serdes_t8_adapt_objects_ptr->dfe_coef1 = 0;
    serdes_t8_adapt_objects_ptr->dfe_coef2 = 0;
    serdes_t8_adapt_objects_ptr->dfe_coef3 = 0;
    serdes_t8_adapt_objects_ptr->dfe_coef4 = 0;
    serdes_t8_adapt_objects_ptr->dfe_coef5 = 0;
    serdes_t8_adapt_objects_ptr->pga_gain = 0x11;
    serdes_t8_adapt_objects_ptr->pga_offset = 0;
    serdes_t8_adapt_objects_ptr->target_amp = 0x27;
    serdes_t8_adapt_objects_ptr->adc_map_scale = 0x8;    

    PMC_RETURN();
}/* digi_serdes_t8_adaptation_default_values_get */


/*******************************************************************************
* digi_serdes_c8_adaptation_default_values_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Helper function fill up a c8_serdes_adapt_object with default values defined in register doc.
*
* INPUTS:
*  None.
*
* OUTPUTS:
*  serdes_c8_adapt_objects_ptr - initialized C8 adaptation values object
*
* RETURNS:
*   PMC_SUCCESS - on success, a descriptive error code otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_serdes_c8_adaptation_default_values_get(serdes_c8_adapt_objects_t  *serdes_c8_adapt_objects_ptr)
{

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != serdes_c8_adapt_objects_ptr, DIGI_ERR_INVALID_ARG, 0, 0);

    serdes_c8_adapt_objects_ptr->dp_ffe_a = 0;
    serdes_c8_adapt_objects_ptr->target_amp = 0x1A;
    serdes_c8_adapt_objects_ptr->adc_map_scale = 0x8;    

    PMC_RETURN();
}/* digi_serdes_c8_adaptation_default_values_get */

/*******************************************************************************
* digi_serdes_adaptation_reset_check
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Helper function to check if we try to adapt serdes in reset
*
* INPUTS:
*  digi_handle     - DIGI device handle.
*  interface       - SERDES interface to configure.
*  lane_0_19_msk   - Bitmask of lanes 0-19 to configure. 
*                    Bit 0 corresponds to lane 0.
*  lane_20_39_msk  - Bitmask of lanes 20-39 to configure. 
*                    Bit 0 corresponds to lane 20.
*                    Ignored for S16 or C8 serdes.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS - on success, a descriptive error code otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_adaptation_reset_check(digi_handle_t            *digi_handle,
                                                     hostmsg_adapt_intf_type_t interface,
                                                     UINT32                    lane_0_19_msk,
                                                     UINT32                    lane_20_39_msk)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 num_inst = 0, itr;
    s16_handle_t * s16_handle = NULL;
    UINT32 serdes_id;
    PMC_ENTRY();
    

    switch(interface)
    {
    case HOSTMSG_ADAPT_INTF_S16_LINE:        
    case HOSTMSG_ADAPT_INTF_S16_SYS:      
        if (HOSTMSG_ADAPT_INTF_S16_LINE == interface)
        {
            s16_handle = digi_handle->s16_line_handle;
            num_inst = DIGI_SERDES_XFI_LANES_MAX;
        } else 
        {
            s16_handle = digi_handle->s16_sys_handle;
            num_inst = DIGI_SERDES_MAX_S16_IN_BP;
            
        }

        for (itr = 0; itr < num_inst && PMC_SUCCESS == rc; itr++)
        {
            if (0 != ((lane_0_19_msk >> itr )  & 0x1))
            {
                if (s16_reset_state_get(s16_handle,itr) == TRUE)
                {
                    rc = DIGI_ERR_SERDES_ADAPT_ON_RESET;
                }
            }
        }
        break;
    case HOSTMSG_ADAPT_INTF_C8:
        for (itr = 0; itr < DIGI_SERDES_MAX_C8_IN_BP_SR && PMC_SUCCESS == rc; itr++)
        {
            if (0 != ((lane_0_19_msk >> itr) & 0x1))
            {
                if (TRUE == c8_reset_state_get(digi_handle->bp_sr_c8_handle,itr))
                {
                    rc = DIGI_ERR_SERDES_ADAPT_ON_RESET;
                }
            }
        }
        break;
    case HOSTMSG_ADAPT_INTF_T8:
        for (itr = 0; itr <  DIGI_SERDES_MAX_T8_SLICE && PMC_SUCCESS == rc ; itr++)
        {            
            serdes_id = digi_serdes_map_t8_serdes(itr);
            if (0 != ((lane_0_19_msk >> itr) & 0x1))
            {
                if (TRUE == t8_reset_state_get(digi_handle->bp_lr_t8_handle[DIGI_SERDES_T8_SLICE_ID_GET(itr)],serdes_id))
                {
                    rc = DIGI_ERR_SERDES_ADAPT_ON_RESET;
                }
            }
            if (0 != ((lane_20_39_msk >> itr) & 0x1))
            {
                UINT32 s = itr+DIGI_SERDES_MAX_T8_SLICE;
                if (TRUE == t8_reset_state_get(digi_handle->bp_lr_t8_handle[DIGI_SERDES_T8_SLICE_ID_GET(s)], serdes_id))
                {
                    rc = DIGI_ERR_SERDES_ADAPT_ON_RESET;
                }
            }           
        }
        break;
    default:
        rc = DIGI_ERR_INVALID_ARG;
        break;
    }

    PMC_RETURN(rc);
} /* digi_serdes_adaptation_reset_check */

/*******************************************************************************
* digi_serdes_adaptation_values_reset
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Helper function to reset serdes adaptatin values. This function should be called
*  before serdes adaptation.
*
* INPUTS:
*  digi_handle     - DIGI device handle.
*  interface       - SERDES interface to configure.
*  lane_0_19_msk   - Bitmask of lanes 0-19 to configure. 
*                    Bit 0 corresponds to lane 0.
*  lane_20_39_msk  - Bitmask of lanes 20-39 to configure. 
*                    Bit 0 corresponds to lane 20.
*                    Ignored for S16 or C8 serdes.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS - on success, a descriptive error code otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_adaptation_values_reset(digi_handle_t            *digi_handle,
                                                      hostmsg_adapt_intf_type_t interface,
                                                      UINT32                    lane_0_19_msk,
                                                      UINT32                    lane_20_39_msk)
{
    serdes_s16_adapt_objects_t  serdes_s16_adapt_objects;
    serdes_c8_adapt_objects_t  serdes_c8_adapt_objects;
    serdes_t8_adapt_objects_t  serdes_t8_adapt_objects;
    UINT32 itr;
    PMC_ERROR rc = PMC_SUCCESS;


    PMC_ENTRY();

    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);    

    /* Initialize serdes adapation vlaues before start adaptation */
    switch(interface)
    {
    case HOSTMSG_ADAPT_INTF_S16_LINE:
    case HOSTMSG_ADAPT_INTF_S16_SYS:
        digi_serdes_s16_adaptation_default_values_get(&serdes_s16_adapt_objects);
        for (itr = 0; (HOSTMSG_ADAPT_INTF_S16_LINE == interface && itr < DIGI_SERDES_XFI_LANES_MAX) ||  (HOSTMSG_ADAPT_INTF_S16_SYS == interface && itr < DIGI_SERDES_MAX_S16_IN_BP); itr++)
        {
            if (0 != (lane_0_19_msk & 0x1))
            {
                rc = digi_serdes_s16_write_adaptation_values(digi_handle, HOSTMSG_ADAPT_INTF_S16_LINE == interface ? HOSTMSG_S16_SERDES_INTF_LINE : HOSTMSG_S16_SERDES_INTF_SYS, itr, &serdes_s16_adapt_objects);
            }
            lane_0_19_msk = (lane_0_19_msk >> 1);
        }
        if (0 != lane_20_39_msk || 0 != lane_0_19_msk)
        {
            rc = DIGI_ERR_INVALID_ARG;
        }
        break;
    case HOSTMSG_ADAPT_INTF_C8:
        digi_serdes_c8_adaptation_default_values_get(&serdes_c8_adapt_objects);
        for (itr = 0; itr < DIGI_SERDES_MAX_C8_IN_BP_SR; itr++)
        {
            if (0 != (lane_0_19_msk & 0x1))
            {
                rc = digi_serdes_c8_write_adaptation_values(digi_handle, itr, &serdes_c8_adapt_objects);
            }
            lane_0_19_msk = (lane_0_19_msk >> 1);
        }
        if (0 != lane_20_39_msk || 0 != lane_0_19_msk)
        {
            rc = DIGI_ERR_INVALID_ARG;
        }
        break;
    case HOSTMSG_ADAPT_INTF_T8:
        digi_serdes_t8_adaptation_default_values_get(&serdes_t8_adapt_objects);
        for (itr = 0; itr < (DIGI_SERDES_MAX_T8_SLICE); itr++)
        {
            if (0 != (lane_0_19_msk & 0x1))
            {
                rc = digi_serdes_t8_write_adaptation_values(digi_handle, itr, &serdes_t8_adapt_objects);
            }
            if (0 != (lane_20_39_msk & 0x1))
            {
                rc = digi_serdes_t8_write_adaptation_values(digi_handle, itr + (DIGI_SERDES_MAX_T8_SLICE
                                                                ), &serdes_t8_adapt_objects);
            }
            lane_0_19_msk = (lane_0_19_msk >> 1);
            lane_20_39_msk = (lane_20_39_msk >> 1);
        }
        if (0 != lane_20_39_msk || 0 != lane_0_19_msk)
        {
            rc = DIGI_ERR_INVALID_ARG;
        }
        break;
    default:
        rc = DIGI_ERR_INVALID_ARG;
        break;
    }

    PMC_RETURN(rc);
}/*digi_serdes_adaptation_values_reset*/

/*******************************************************************************
* digi_serdes_handle_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*   helper function to return an s16 handle for a given interface 
*
* INPUTS:
*  digi_handle     - DIGI device handle.
*  intf            - SERDES interface handle to get.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   returns the appropriate a pointer to a s16_handle based on the INTF selected
*
* NOTES:
*
*******************************************************************************/
PUBLIC void *digi_serdes_handle_get(digi_handle_t      *digi_handle, 
                                    digi_serdes_intf_t  intf)
{ 
    s16_handle_t * s16_handle;
    void *rc_ptr;

    PMC_ENTRY();

    if (intf == DIGI_SERDES_LINE_INTF )
    {
        s16_handle = digi_handle->s16_line_handle;
    } 
    else 
    {
        /*DIGI_SERDES_SYSTEM_INTF*/
        s16_handle = digi_handle->s16_sys_handle;
    }

    rc_ptr = (void *)s16_handle;

    PMC_RETURN(rc_ptr);
}/* digi_serdes_handle_get */

/*******************************************************************************
* digi_serdes_restart_init
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Audit and cleanup serdes context at digi top layer and in block level.
*
* INPUTS:
*   digi_handle            - DIGI device handle.
*  *restart_init_cfg_ptr   - pointer to restart init config
*
* OUTPUTS:
*  None.
*
* RETURNS:
*  PMC_SUCCESS when operation is successful otherwise a descriptive error
*  is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_restart_init(digi_handle_t                  *digi_handle,
                                          util_global_restart_init_cfg_t *restart_init_cfg_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    /* Audit S16 line serdes */  
    rc = digi_serdes_xfi_pin_ctxt_audit(digi_handle,
                                        restart_init_cfg_ptr,
                                        DIGI_SERDES_LINE_INTF);
    
    /* Audit S16 system serdes */
    if (PMC_SUCCESS == rc)
    {
        rc = digi_serdes_xfi_pin_ctxt_audit(digi_handle,
                                            restart_init_cfg_ptr,
                                            DIGI_SERDES_SYSTEM_INTF);
    }

    /* Audit SFI51 system serdes */
    if (PMC_SUCCESS == rc)
    {
        rc = digi_serdes_sfi51_bus_ctxt_audit(digi_handle,
                                              restart_init_cfg_ptr);
    }

    /* Audit backplane serdes */
    if (PMC_SUCCESS == rc)
    {
        rc = digi_serdes_bp_ctxt_audit(digi_handle,
                                       restart_init_cfg_ptr);
    }

    /* Audit GE serdes */
    if (PMC_SUCCESS == rc)
    {
        rc = digi_serdes_ge_ctxt_audit(digi_handle,
                                       restart_init_cfg_ptr);
    }

    PMC_RETURN(rc);
}/*digi_serdes_restart_init*/

/*******************************************************************************
* digi_serdes_xfi_pin_ctxt_audit
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Audit and cleanup S16 SYS or LINE serdes context at digi top layer 
*  and in block level.
*
* INPUTS:
*   digi_handle            - DIGI device handle.
*  *restart_init_cfg_ptr   - pointer to restart init config
*   intf_type              - serdes interface type: line or sys.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*  PMC_SUCCESS when operation is successful otherwise a descriptive error
*  is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_xfi_pin_ctxt_audit(digi_handle_t                  *digi_handle,
                                                 util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                                 digi_serdes_intf_t              intf_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 s16_reset_msk;
    UINT32 itr;
    UINT32 jtr;
    s16_handle_t *s16_handle_ptr = NULL;
    digi_xfi_pin_ctxt_t *xfi_pin_ctxt_ptr;
    UINT32 num_lanes;
    BOOL8 serdes_reset_state;
    UINT32 dummy_0 = 0, dummy_1 = 0, dummy_2 = 0;
    UINT32 bp_s16_msk = 0;
    digi_sifd_ilkn_1_lane_cfg_t *sifd_1_cfg_ptr;
    digi_sifd_ilkn_2_lane_cfg_t *sifd_2_cfg_ptr;
    BOOL8 bp_s16_map = FALSE;
    digi_serdes_port_mode_t *rates_l;  
    BOOL8 serdes_used_by_pmon;    
    BOOL8 is_s16_clk;
    digi_serdes_port_def_t *serdes_port_ptr;

    PMC_ENTRY();

    /* Setup serdes properties */
    switch (intf_type)
    {
    case DIGI_SERDES_LINE_INTF: 
        s16_reset_msk = tl_ic_field_S16_LINE_RESET_get(NULL,
                                                       digi_handle);
        s16_handle_ptr = digi_handle->s16_line_handle;
        xfi_pin_ctxt_ptr = digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt;
        num_lanes = DIGI_SERDES_XFI_LANES_MAX;
        rates_l = &digi_handle->var.serdes_ctxt.s16_line_lanes_rate[0];
        serdes_port_ptr = &(digi_handle->var.port_ctxt.line_port_ctxt[0]);
        break;
    case DIGI_SERDES_SYSTEM_INTF:
        s16_reset_msk = tl_ic_field_S16_SYS_RESET_get(NULL,
                                                      digi_handle);
        s16_handle_ptr = digi_handle->s16_sys_handle;
        xfi_pin_ctxt_ptr = digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt;
        num_lanes = DIGI_SERDES_SYS_LANES_MAX;
        rates_l = &digi_handle->var.serdes_ctxt.s16_sys_lanes_rate[0];
        serdes_port_ptr = &(digi_handle->var.port_ctxt.sys_port_ctxt[0]);
        /* retrieve S16 SYS serdes mapped to backplane interface */
        sifd_1_cfg_ptr = &digi_handle->var.sifd_ilkn_1_cfg.lane;
        sifd_2_cfg_ptr = &digi_handle->var.sifd_ilkn_2_cfg.lane;
        rc = digi_bp_serdes_mask_get(digi_handle, 
                                     sifd_1_cfg_ptr,
                                     sifd_2_cfg_ptr,
                                     &dummy_0,
                                     &dummy_1,
                                     &bp_s16_msk,
                                     &dummy_2);
        break;

    default:
        rc = DIGI_ERR_INVALID_ARG;
        break;
    }

    /* s16 block level restart init */
    if (rc == PMC_SUCCESS)
    {       
        rc = s16_handle_restart_init(s16_handle_ptr,
                                     restart_init_cfg_ptr,
                                     s16_reset_msk,
                                     bp_s16_msk);
    }

    /* audit and cleanup (if necessary) context at digi top layer */
    if (rc == PMC_SUCCESS)
    {       
        /* audit and cleanup XFI serdes port context */
        for (itr = 0; itr < num_lanes; itr += 1)
        {
            rc = PMC_SUCCESS; 
            bp_s16_map = FALSE;
            if (DIGI_SERDES_SYSTEM_INTF == intf_type)
            {                        
                bp_s16_map = (0x1 != (0x1 & (bp_s16_msk >> itr))) ? FALSE : TRUE;
            }

            /*S16 SYS serdes could be mapped to backplane, in that case accept reset status */
            if (FALSE == bp_s16_map)
            {
                /* check that serdes is in prov state when reset bit is down */
                if (0 == (0x1 & (s16_reset_msk >> itr)))
                {
                    if ((DIGI_SERDES_PROV_STATE_UNASSIGNED == xfi_pin_ctxt_ptr[itr].serdes_rate_state.port_prov_state || 
                         DIGI_SERDES_PROV_STATE_NO_SERDES == xfi_pin_ctxt_ptr[itr].serdes_rate_state.port_prov_state) &&
                        (FALSE == (DIGI_SERDES_SYSTEM_INTF == intf_type &&
                                   FALSE == s16_reset_state_get(digi_handle->s16_sys_handle, itr) && TRUE == s16_tx_squelch_enable_status_get(s16_handle_ptr, itr) && (itr == digi_handle->var.digi_enet_pmon_GSUP43_7_3_sys_usage))))
                    {       
                        /* check that serdes is not connected to enet line channel */
                        serdes_used_by_pmon = digi_serdes_is_serdes_connect_to_enet_pmon(digi_handle,
                                                                                        intf_type,
                                                                                        xfi_pin_ctxt_ptr,
                                                                                        xfi_pin_ctxt_ptr[itr].port_uid);
                        /* Check that s16 serdes is not used as a clock */
                        is_s16_clk = FALSE;                                    
                        if (FALSE == serdes_used_by_pmon)
                        {
                            for(jtr = 0; jtr < num_lanes; jtr += 1)
                            {
                                if (serdes_port_ptr[jtr].s16_clk_cfg.pin_index == itr)
                                {
                                    is_s16_clk = TRUE;
                                    break;
                                }
                            }
                        }

                        /* if serdes is not connected to neither enet line neither enet sys, return error */
                        if (FALSE == serdes_used_by_pmon && FALSE == is_s16_clk)
                        {
                            rc = DIGI_ERR_CONTEXT_INCOHERENT;
                        }
                    }
                    /* check any incoherency between digi top layer ctxt and s16 block context */
                    else if (LAST_DIGI_SERDES_PORT_MODE == rates_l[itr])
                    {                                        
                        rc = DIGI_ERR_CONTEXT_INCOHERENT;
                    }
                }
                else
                {
                    /* check that serdes is not in prov state when reset bit is high  */
                    if ((DIGI_SERDES_PROV_STATE_UNASSIGNED != xfi_pin_ctxt_ptr[itr].serdes_rate_state.port_prov_state && 
                         DIGI_SERDES_PROV_STATE_NO_SERDES != xfi_pin_ctxt_ptr[itr].serdes_rate_state.port_prov_state))
                    {
                        rc = DIGI_ERR_CONTEXT_INCOHERENT;
                    }
                    /* check any incoherency between digi top layer ctxt and s16 block context */
                    else if (LAST_DIGI_SERDES_PORT_MODE != rates_l[itr])
                    {                                        
                        rc = DIGI_ERR_CONTEXT_INCOHERENT;
                    }
                }
                /* in crash restart: reset serdes context if there is any incoherency */
                if (FALSE == restart_init_cfg_ptr->is_warm_restart)
                { 
                    serdes_reset_state = s16_reset_state_get(s16_handle_ptr, 
                                                             itr);
                    if (PMC_SUCCESS != rc || 
                        0x1 == (0x1 & (s16_reset_msk >> itr)) ||
                        (FALSE == bp_s16_map &&
                         TRUE == serdes_reset_state))
                    {
                        xfi_pin_ctxt_ptr[itr].serdes_rate_state.port_mode = LAST_DIGI_SERDES_PORT_MODE;
                        xfi_pin_ctxt_ptr[itr].serdes_rate_state.port_prov_state = DIGI_SERDES_PROV_STATE_UNASSIGNED;
                        xfi_pin_ctxt_ptr[itr].serdes_rate_state.rate_prov_state = DIGI_SERDES_PORT_RATE_STATE_DEPROV;
                        rc = PMC_SUCCESS;
                    }
                } 
            }

            /* in warm restart: stop auditing if any incoherency is found */
            if (PMC_SUCCESS != rc && TRUE == restart_init_cfg_ptr->is_warm_restart)
            { 
                break;
            }
        }
    }

    PMC_RETURN(rc);
}/* digi_serdes_xfi_pin_ctxt_audit */

/*******************************************************************************
* digi_serdes_sfi51_bus_ctxt_audit
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Audit and cleanup SFI51 bus context at digi top layer 
*  and in block level.
*
* INPUTS:
*   digi_handle            - DIGI device handle.
*  *restart_init_cfg_ptr   - pointer to restart init config
*
* OUTPUTS:
*  None.
*
* RETURNS:
*  PMC_SUCCESS when operation is successful otherwise a descriptive error
*  is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_sfi51_bus_ctxt_audit(digi_handle_t                  *digi_handle,
                                                   util_global_restart_init_cfg_t *restart_init_cfg_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL8 tx_reset;
    BOOL8 rx_reset;
    
    UINT32 itr;

    PMC_ENTRY();

    for (itr = 0; itr < DIGI_NUM_SFI51_PORTS_MAX && PMC_SUCCESS == rc; itr += 1)
    {

        tx_reset = 0 != tl_ic_field_range_SFI51_TX_RESET_get(NULL,
                                                             digi_handle,
                                                             itr, itr) ? TRUE : FALSE;
        
        rx_reset = tx_reset;
        
        /* sfi51 tx block level restart init */
        rc = sfi51_d8_tx_18x_handle_restart_init(digi_handle->sfi51_d8_tx_18x_handle[itr],
                                                 restart_init_cfg_ptr,
                                                 tx_reset);
        
        if (rc == PMC_SUCCESS)
        {       
            /* sfi51 rx block level restart init */
            rc = sfi51_d8_rx_18x_handle_restart_init(digi_handle->sfi51_d8_rx_18x_handle[itr],
                                                     restart_init_cfg_ptr,
                                                     rx_reset);
        }   

        /* audit and cleanup (if necessary) context at digi top layer */
        if (rc == PMC_SUCCESS)
        {       
            if (PMC_SUCCESS == rc)
            {
                /* check that serdes is in prov state when reset bit is down */
                if (FALSE == tx_reset)
                {
                    if ((DIGI_SERDES_PROV_STATE_UNASSIGNED == digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[itr].serdes_rate_state.port_prov_state || 
                         DIGI_SERDES_PROV_STATE_NO_SERDES == digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[itr].serdes_rate_state.port_prov_state))
                    {
                        rc = DIGI_ERR_CONTEXT_INCOHERENT;
                    }
                    else if (LAST_DIGI_SERDES_PORT_MODE == digi_handle->var.serdes_ctxt.sfi51_lanes_rate[itr] )
                    {
                        rc = DIGI_ERR_CONTEXT_INCOHERENT;                        
                    }
                }
                else
                {
                    /* check that serdes is not in prov state when reset bit is high  */
                    if ((DIGI_SERDES_PROV_STATE_UNASSIGNED != digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[itr].serdes_rate_state.port_prov_state && 
                         DIGI_SERDES_PROV_STATE_NO_SERDES != digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[itr].serdes_rate_state.port_prov_state))
                    {
                        rc = DIGI_ERR_CONTEXT_INCOHERENT;
                    }
                    else if (LAST_DIGI_SERDES_PORT_MODE != digi_handle->var.serdes_ctxt.sfi51_lanes_rate[itr] )
                    {
                        rc = DIGI_ERR_CONTEXT_INCOHERENT;                        
                    }
                }
            }
            /* in crash restart: reset serdes context if there is any incoherency */
            if (FALSE == restart_init_cfg_ptr->is_warm_restart)
            { 
                if (PMC_SUCCESS != rc || 
                    TRUE == tx_reset ||
                    TRUE == rx_reset ||
                    TRUE == sfi51_d8_tx_18x_reset_state_get(digi_handle->sfi51_d8_tx_18x_handle[itr]) ||
                    TRUE == sfi51_d8_rx_18x_reset_state_get(digi_handle->sfi51_d8_rx_18x_handle[itr]))
                {
                    digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[itr].serdes_rate_state.port_mode = LAST_DIGI_SERDES_PORT_MODE;
                    digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[itr].serdes_rate_state.port_prov_state = DIGI_SERDES_PROV_STATE_UNASSIGNED;
                    digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[itr].serdes_rate_state.rate_prov_state = DIGI_SERDES_PORT_RATE_STATE_DEPROV;
                    rc = PMC_SUCCESS;
                }
            } 
            /* in warm restart: stop auditing if any incoherency is found */
            if (PMC_SUCCESS != rc && TRUE == restart_init_cfg_ptr->is_warm_restart)
            { 
                break;
            }
        }
    }

    PMC_RETURN(rc);
}/*digi_serdes_sfi51_bus_audit*/

/*******************************************************************************
* digi_serdes_bp_ctxt_audit
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Audit and cleanup backplane interface serdes at digi top layer 
*  and in block level.
*
* INPUTS:
*   digi_handle            - DIGI device handle.
*  *restart_init_cfg_ptr   - pointer to restart init config.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*  PMC_SUCCESS when operation is successful otherwise a descriptive error
*  is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_bp_ctxt_audit(digi_handle_t                  *digi_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 s16_reset_msk;
    UINT32 c8_reset_msk = 0;
    UINT32 t8_reset_msk;
    UINT32 itr;
    UINT32 jtr;
    UINT32 ltr;
    s16_handle_t *s16_handle_ptr = NULL;
    digi_xfi_pin_ctxt_t *xfi_pin_ctxt_ptr;
    UINT32 s16_num_lanes;
    UINT32 t8_19_00_msk = 0;
    UINT32 t8_39_20_msk = 0;
    UINT32 c8_msk = 0;
    UINT32 bp_s16_msk = 0;

    digi_sifd_ilkn_1_lane_cfg_t *sifd_1_cfg_ptr;
    digi_sifd_ilkn_2_lane_cfg_t *sifd_2_cfg_ptr;
    util_global_restart_init_cfg_t restart_init_cfg_int;
    digi_serdes_port_mode_t *rates_l;

    PMC_ENTRY();

    PMC_MEMCPY((void *)&restart_init_cfg_int, (void *)restart_init_cfg_ptr, sizeof(util_global_restart_init_cfg_t));
    
    restart_init_cfg_int.is_warm_restart = TRUE;

    /* prepare serdes properties */
    s16_reset_msk = tl_ic_field_S16_SYS_RESET_get(NULL,
                                                  digi_handle);
    s16_handle_ptr = digi_handle->s16_sys_handle;
    xfi_pin_ctxt_ptr = digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt;
    s16_num_lanes = DIGI_SERDES_MAX_S16_IN_BP;
    rates_l = &digi_handle->var.serdes_ctxt.s16_sys_lanes_rate[0];
    /* retrieve S16 SYS serdes mapped to backplane interface */
    sifd_1_cfg_ptr = &digi_handle->var.sifd_ilkn_1_cfg.lane;
    sifd_2_cfg_ptr = &digi_handle->var.sifd_ilkn_2_cfg.lane;
    rc = digi_bp_serdes_mask_get(digi_handle, 
                                 sifd_1_cfg_ptr,
                                 sifd_2_cfg_ptr,
                                 &t8_19_00_msk,
                                 &t8_39_20_msk,
                                 &bp_s16_msk,
                                 &c8_msk);


    /* s16 block level restart init */
    if (rc == PMC_SUCCESS)
    {       
        rc = s16_handle_restart_init(s16_handle_ptr,
                                     &restart_init_cfg_int,
                                     s16_reset_msk,
                                     0xFFFF & ~bp_s16_msk);
    }
    
    if (rc == PMC_SUCCESS)
    {    
        t8_reset_msk = tl_ic_field_TITAN_RESET_get(NULL,
                                                   digi_handle);   
        for (itr = 0; itr < DIGI_SERDES_MAX_T8_2X && PMC_SUCCESS == rc; itr += 1)
        {            
            rc = t8_handle_restart_init(digi_handle->bp_lr_t8_handle[itr],
                                        &restart_init_cfg_int,
                                        0x1 & (t8_reset_msk >> itr));
        }
    }

    if (rc == PMC_SUCCESS)
    {    
        c8_reset_msk = tl_ic_field_C8_RESET_get(NULL,
                                                digi_handle);   
#ifdef C8_RESTART
        rc = c8_handle_restart_init(digi_handle->bp_sr_c8_handle,
                                    &restart_init_cfg_int,
                                    c8_reset_msk);
#endif
    }

    /* audit and cleanup T8 serdes (if necessary) context at digi top layer */
    if (rc == PMC_SUCCESS)
    {
        /* audit and cleanup XFI serdes port context */
        for (itr = 0; itr < DIGI_SERDES_MAX_T8_2X && PMC_SUCCESS == rc; itr += 1)
        {
            for (jtr = 0; jtr < T8_MAX_NUM_SERDES; jtr += 1)
            {
                /* check any incoherency between digi top layer ctxt and t8 block context */
                if ((TRUE == t8_reset_state_get(digi_handle->bp_lr_t8_handle[itr], jtr) &&
                     LAST_DIGI_SERDES_PORT_MODE != digi_handle->var.serdes_ctxt.t8_lanes_rate[(itr*T8_MAX_NUM_SERDES) + jtr]) ||
                    (TRUE != t8_reset_state_get(digi_handle->bp_lr_t8_handle[itr], jtr) &&
                     LAST_DIGI_SERDES_PORT_MODE == digi_handle->var.serdes_ctxt.t8_lanes_rate[(itr*T8_MAX_NUM_SERDES) + jtr]))
                {                                        
                    rc = DIGI_ERR_CONTEXT_INCOHERENT;
                }
                if (0x1 == (0x1 & (t8_reset_msk >> itr)))
                {
                    if (LAST_DIGI_SERDES_PORT_MODE != digi_handle->var.serdes_ctxt.t8_lanes_rate[(itr*T8_MAX_NUM_SERDES) + jtr])
                    {
                        rc = DIGI_ERR_CONTEXT_INCOHERENT;
                    }
                }
                else
                {
                    for (ltr = 0; ltr < T8_MAX_NUM_SERDES; ltr += 1)
                    {
                        if (LAST_DIGI_SERDES_PORT_MODE == digi_handle->var.serdes_ctxt.t8_lanes_rate[(itr*T8_MAX_NUM_SERDES) + jtr])
                        {
                            if (jtr == ltr)
                            {
                                continue;
                            }
                            if (LAST_DIGI_SERDES_PORT_MODE == digi_handle->var.serdes_ctxt.t8_lanes_rate[(itr*T8_MAX_NUM_SERDES) + ltr])
                            {                                  
                                rc = DIGI_ERR_CONTEXT_INCOHERENT;
                            }
                        }
                    }
                    
                }
            }
        }
    }/* end of T8 serdes context audit*/

    /* audit and cleanup C8 serdes (if necessary) context at digi top layer */
    if (rc == PMC_SUCCESS)
    {
        for (itr = 0; itr < C8_MAX_NUM_SERDES && PMC_SUCCESS == rc; itr += 1)
        {
            /* check any incoherency between digi top layer ctxt and t8 block context */
            if ((TRUE == c8_reset_state_get(digi_handle->bp_sr_c8_handle, itr) &&
                 LAST_DIGI_SERDES_PORT_MODE != digi_handle->var.serdes_ctxt.c8_lanes_rate[itr]) ||
                (TRUE != c8_reset_state_get(digi_handle->bp_sr_c8_handle, itr) &&
                 LAST_DIGI_SERDES_PORT_MODE == digi_handle->var.serdes_ctxt.c8_lanes_rate[itr]))
            {                                        
                rc = DIGI_ERR_CONTEXT_INCOHERENT;
            }
            if (0x1 == (0x1 & (c8_reset_msk >> itr)))
            {
                if (LAST_DIGI_SERDES_PORT_MODE != digi_handle->var.serdes_ctxt.c8_lanes_rate[itr])
                {
                    rc = DIGI_ERR_CONTEXT_INCOHERENT;
                }
            }
            else
            {
                if (LAST_DIGI_SERDES_PORT_MODE == digi_handle->var.serdes_ctxt.c8_lanes_rate[itr])
                {
                    rc = DIGI_ERR_CONTEXT_INCOHERENT;
                }
            }          
        }
    }/*end of C8 serdes context audit*/

    /* audit and cleanup S16 SYS (if necessary) context at digi top layer */
    if (rc == PMC_SUCCESS)
    {
        for (itr = 0; itr < s16_num_lanes; itr += 1)
        {
            if (0x1 == (0x1 & bp_s16_msk >> itr))
            {
                /* check any incoherency between digi top layer ctxt and t8 block context */
                if ((TRUE == s16_reset_state_get(digi_handle->s16_sys_handle, itr) &&
                     LAST_DIGI_SERDES_PORT_MODE != rates_l[itr]) ||
                    (TRUE !=  s16_reset_state_get(digi_handle->s16_sys_handle, itr) &&
                     LAST_DIGI_SERDES_PORT_MODE == rates_l[itr]))
                {                                        
                    rc = DIGI_ERR_CONTEXT_INCOHERENT;
                }             
                else if (0x1 == (0x1 & s16_reset_msk >> itr))
                {                       
                    if (LAST_DIGI_SERDES_PORT_MODE != rates_l[itr])
                    { 
                        rc = DIGI_ERR_CONTEXT_INCOHERENT;
                    }
                }
                else
                {               
                    if (LAST_DIGI_SERDES_PORT_MODE == rates_l[itr])
                    { 
                        rc = DIGI_ERR_CONTEXT_INCOHERENT;
                    }
                }
            }
        }
    }/*end of S16 SYS serdes context audit*/

    PMC_RETURN(rc);
}/* digi_serdes_bp_ctxt_audit */

/*******************************************************************************
* digi_serdes_ge_ctxt_audit
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Audit and cleanup backplane interface serdes at digi top layer 
*  and in block level.
*
* INPUTS:
*   digi_handle            - DIGI device handle.
*  *restart_init_cfg_ptr   - pointer to restart init config.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*  PMC_SUCCESS when operation is successful otherwise a descriptive error
*  is returned.
*
* NOTES:
********************************************************************************/
PRIVATE PMC_ERROR digi_serdes_ge_ctxt_audit(digi_handle_t                  *digi_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ERROR rc_slice1 = PMC_SUCCESS;
    UINT32 rx_d8_reset_msk;
    UINT32 tx_d8_reset_msk;
    UINT32 itr;
    d8_rx_2x_handle_t  *d8_rx_2x_handle;
    UINT8 rx_serdes_id;
    d8_tx_2x_handle_t  *d8_tx_2x_handle;
    UINT8 tx_serdes_id;
    digi_ge_serdes_ctxt_t *ge_serdes_ctxt;
    digi_ge_serdes_ctxt_t *ge_serdes_ctxt_slice2;
    util_global_restart_init_cfg_t restart_init_cfg_int;
    UINT32 serdes_slice;
    BOOL8 rx_d8_reset_status;
    BOOL8 tx_d8_reset_status;

    PMC_ENTRY();

    PMC_MEMCPY((void *)&restart_init_cfg_int, (void *)restart_init_cfg_ptr, sizeof(util_global_restart_init_cfg_t));
    
    restart_init_cfg_int.is_warm_restart = TRUE;


    for(itr = 0; itr < DIGI_SERDES_GE_PORTS_MAX && PMC_SUCCESS == rc; itr += 1)
    {        
        rc = digi_ge_d8_rx_2x_handle_get(digi_handle, (digi_ge_serdes_id_t)itr,
                                         &d8_rx_2x_handle,
                                         &rx_serdes_id);
        if (PMC_SUCCESS == rc)
        {
            rc = digi_ge_d8_tx_2x_handle_get(digi_handle, (digi_ge_serdes_id_t)itr,
                                             &d8_tx_2x_handle,
                                             &tx_serdes_id);
        } 
        
        if (0x0 == (itr % 2))
        {
            switch(itr/2)
            {
            case 0:
                serdes_slice = 0;
                break;
            case 1:
                serdes_slice = 1;
                break;
            case 2:
                serdes_slice = 2;
                break;
            default:
                break;
            } 
            /* prepare serdes properties */
            rx_d8_reset_msk = tl_ic_field_range_OH_SERDES_RX_RESET_get(NULL,
                                                                       digi_handle, serdes_slice, serdes_slice);
            switch(itr/2)
            {
            case 0:
                serdes_slice = 1;
                break;
            case 1:
                serdes_slice = 2;
                break;
            case 2:
                serdes_slice = 0;
                break;
            default:
                break;
            } 
            tx_d8_reset_msk = tl_ic_field_range_OH_SERDES_TX_RESET_get(NULL,
                                                                       digi_handle, serdes_slice, serdes_slice);

            if (PMC_SUCCESS == rc)
            {
                rc = digi_ge_serdes_ctxt_get(digi_handle,
                                             (digi_ge_serdes_id_t)itr,
                                             &ge_serdes_ctxt);
            }

            ge_serdes_ctxt_slice2 = NULL;
            if (PMC_SUCCESS == rc)
            {
                rc = digi_ge_serdes_ctxt_get(digi_handle,
                                             (digi_ge_serdes_id_t)(itr+1),
                                             &ge_serdes_ctxt_slice2);
            }
        }
        else
        {
            ge_serdes_ctxt = ge_serdes_ctxt_slice2;
        }

        /* Call d8_rx_2x block crash/restart routine */
        if (PMC_SUCCESS == rc)
        {
            rc = d8_rx_2x_handle_restart_init(d8_rx_2x_handle,
                                              &restart_init_cfg_int,
                                              rx_d8_reset_msk);
        }
        
        /* Call d8_tx_2x block crash/restart routine */
        if (PMC_SUCCESS == rc)
        {
            rc = d8_tx_2x_handle_restart_init(d8_tx_2x_handle,
                                              &restart_init_cfg_int,
                                              tx_d8_reset_msk);
        }

        if (PMC_SUCCESS == rc)
        {
            rx_d8_reset_status = d8_rx_2x_reset_state_get(d8_rx_2x_handle, rx_serdes_id);
            tx_d8_reset_status = d8_tx_2x_reset_state_get(d8_tx_2x_handle, tx_serdes_id);

            if (tx_d8_reset_msk != rx_d8_reset_msk) 
            {  
                rc = DIGI_ERR_CONTEXT_INCOHERENT;
            }            
            else if (tx_d8_reset_status != rx_d8_reset_status) 
            {  
                rc = DIGI_ERR_CONTEXT_INCOHERENT;
            }
            else
            {
              if(ge_serdes_ctxt_slice2 != NULL)
              {
          if ((TRUE == tx_d8_reset_status &&
             TRUE == ge_serdes_ctxt->enabled) ||
            (TRUE != tx_d8_reset_status &&
             FALSE == ge_serdes_ctxt->enabled))
          {
            rc = DIGI_ERR_CONTEXT_INCOHERENT;
          }
          else if ((TRUE == rx_d8_reset_status &&
                TRUE == ge_serdes_ctxt->enabled) ||
               (TRUE != rx_d8_reset_status &&
                FALSE == ge_serdes_ctxt->enabled))
          {
            rc = DIGI_ERR_CONTEXT_INCOHERENT;
          }
          else if (0x1 == (0x1 & rx_d8_reset_msk) &&
               FALSE != ge_serdes_ctxt->enabled)
          {
            rc = DIGI_ERR_CONTEXT_INCOHERENT;
          }
          else if (0x1 == (0x1 & tx_d8_reset_msk) &&
               FALSE != ge_serdes_ctxt->enabled)
          {
            rc = DIGI_ERR_CONTEXT_INCOHERENT;
          }
          else if (0 == (itr % 2))
          {
            if (0 == rx_d8_reset_msk &&
              FALSE == ge_serdes_ctxt->enabled &&
              FALSE == ge_serdes_ctxt_slice2->enabled)
            {
              rc = DIGI_ERR_CONTEXT_INCOHERENT;
            }
          }
              }
              else
              {
                rc = DIGI_ERR_CONTEXT_INCOHERENT;
              }
            }
        }

        if (0x0 == (itr % 2))
        {
            rc_slice1 = rc;
            rc = PMC_SUCCESS;
        }
        else
        {
            if (PMC_SUCCESS != rc || PMC_SUCCESS != rc_slice1)
            {
                /* to refine */
                if (PMC_SUCCESS == rc || PMC_SUCCESS == rc_slice1)
                {
                    rc = PMC_SUCCESS;
                }
                else
                {
                    if (PMC_SUCCESS != rc_slice1)
                    {
                        rc = rc_slice1;
                    }
                    break;
                }
            }
        }
    }
    
    PMC_RETURN(rc);
}/* digi_serdes_ge_ctxt_audit */
    
/*******************************************************************************
* digi_serdes_is_pending_state_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  returns TRUE when serdes needs to be cleaned.
*
* INPUTS:
*  rate_prov_state          - serdes rate provisioning state.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*  TRUE when serdes provisioning state is on temporary and it needs to be cleaned.
*  FALSE otherwise.
*
* NOTES:
********************************************************************************/
PRIVATE BOOL8 digi_serdes_is_pending_state_get(digi_serdes_port_rate_prov_state_t     rate_prov_state)
{
    BOOL8 rc = FALSE;

    PMC_ENTRY();
    
    switch (rate_prov_state)
    {
    case DIGI_SERDES_PORT_RATE_STATE_RESTART_PENDING:
    case DIGI_SERDES_PORT_RATE_STATE_PROV_RESTART_PENDING:
    case DIGI_SERDES_PORT_RATE_STATE_PROV_FACILITY_LPBK_RESTART_PENDING:
    case DIGI_SERDES_PORT_RATE_STATE_PROV_DIAG_LPBK_RESTART_PENDING:
        rc = TRUE;
        break;
    default:
        break;
    }

    PMC_RETURN(rc);
}/* digi_serdes_is_pending_state_get */

/*******************************************************************************
* digi_serdes_pending_state_transition
* ___________________________________________________________________________
*
* DESCRIPTION:
*  returns new rate provisoning state after serdes cleanup.
*
* INPUTS:
*  rate_prov_state          - serdes rate provisioning state.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*  New rate provisioning state after serdes cleanup.
*
* NOTES:
********************************************************************************/
PRIVATE digi_serdes_port_rate_prov_state_t digi_serdes_pending_state_transition(digi_serdes_port_rate_prov_state_t  rate_prov_state)
{
    digi_serdes_port_rate_prov_state_t rc = rate_prov_state;

    PMC_ENTRY();
    
    switch (rate_prov_state)
    {
    case DIGI_SERDES_PORT_RATE_STATE_PROV:
        rc = DIGI_SERDES_PORT_RATE_STATE_PROV_RESTART_PENDING;
        break;
    case DIGI_SERDES_PORT_RATE_STATE_PROV_RESTART_PENDING:
        rc = DIGI_SERDES_PORT_RATE_STATE_PROV;
        break;
    case DIGI_SERDES_PORT_RATE_STATE_PROV_FACILITY_LPBK:
        rc = DIGI_SERDES_PORT_RATE_STATE_PROV_FACILITY_LPBK_RESTART_PENDING;
        break;
    case DIGI_SERDES_PORT_RATE_STATE_PROV_FACILITY_LPBK_RESTART_PENDING:
        rc = DIGI_SERDES_PORT_RATE_STATE_PROV_FACILITY_LPBK;
        break;
    case DIGI_SERDES_PORT_RATE_STATE_PROV_DIAG_LPBK:
        rc = DIGI_SERDES_PORT_RATE_STATE_PROV_DIAG_LPBK_RESTART_PENDING;
        break;
    case DIGI_SERDES_PORT_RATE_STATE_PROV_DIAG_LPBK_RESTART_PENDING:
        rc = DIGI_SERDES_PORT_RATE_STATE_PROV_DIAG_LPBK;
        break;
    default:
        break;
    }

    PMC_RETURN(rc);
}/* digi_serdes_pending_state_transition */

/*******************************************************************************
* digi_serdes_is_serdes_connect_to_enet_pmon
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Returns TRUE when serdes port associated to port uid is connected to an
*  ENET enhanced channel.
*
* INPUTS:
*   digi_handle            - DIGI device handle.
*   intf_type              - serdes interface type: line or sys.
*   xfi_pin_ctxt_ptr       - pointer to S16 LINE or SYS XFI pin context.
*   xfi_pin_port_uid       - serdes port unique identifier.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*  TRUE when serdes is connected to a ENET enhanced PMON, FALSE otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 digi_serdes_is_serdes_connect_to_enet_pmon(digi_handle_t                  *digi_handle,
                                                         digi_serdes_intf_t              intf_type,
                                                         digi_xfi_pin_ctxt_t            *xfi_pin_ctxt_ptr,
                                                         UINT32                          xfi_pin_port_uid)
{
    BOOL8 rc = FALSE;
    PMC_ERROR rc_int;
    UINT32 jtr;
    UINT32 port_uid;

    PMC_ENTRY();

    /* check that serdes is connected to enet line enhanced channel */
    if (DIGI_SERDES_LINE_INTF == intf_type ||
        (DIGI_SERDES_SYSTEM_INTF == intf_type && DIGI_PROD_APP_IS_100G_3X40G_CFP_CARD(digi_handle))) 
    {   
        for (jtr = 0; jtr < DIGI_ENET_LINE_CHNL_MAX; jtr += 1)
        {
            
            rc_int = digi_port_uid_from_enet_link_get(digi_handle,
                                                      digi_handle->handle_pool.enet_line_chnl[jtr].switch_data.cpb_data.channel,
                                                      TRUE,
                                                      &port_uid);
            if (PMC_SUCCESS == rc_int &&
                port_uid == xfi_pin_port_uid)
            {
                break;
            }
        }
        
        if (jtr < DIGI_ENET_LINE_CHNL_MAX)
        {
            if (0x1 == (0x1 & (digi_handle->var.digi_enet_pmon_line_usage >> jtr)))
            {
                rc = TRUE;
            }
        }
    }
    
    /* check that serdes is connected to enet sys enhanced channel */
    if (FALSE == rc)
    {
        for (jtr = 0; jtr < DIGI_ENET_SYS_CHNL_MAX; jtr += 1)
        {
            
            rc_int = digi_port_uid_from_enet_link_get(digi_handle,
                                                      digi_handle->handle_pool.enet_sys_chnl[jtr].switch_data.cpb_data.channel,
                                                      FALSE,
                                                      &port_uid);
            if (PMC_SUCCESS == rc_int &&
                port_uid == xfi_pin_port_uid)
            {
                break;
            }
        }
        
        if (jtr < DIGI_ENET_SYS_CHNL_MAX)
        {
            if (0x1 == (0x1 & (digi_handle->var.digi_enet_pmon_sys_usage >> jtr)))
            {
                rc = TRUE;
            }
        }
    }

    PMC_RETURN(rc);
}/*digi_serdes_is_serdes_connect_to_enet_pmon*/

/*******************************************************************************
* digi_serdes_d8_multi_step_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Configure a D8 serdes in a multi mode
*
* INPUTS:
*  digi_handle   - DIGI device handle.
*  d8_intf_type  - D8 Interface type to configure
*  cfg_csu       - TRUE: Configure CSU. FALSE: Do not configure CSU
*                  Since all six SERDES ports are driven by the same CSU, 
*                  the user must specify whether the CSU should be reconfigured. 
*                  Reconfiguring the CSU will temporarily disrupt the traffic on all SERDES ports
*  enable        - TRUE: The specified interface will be taken out of reset and configured. 
*                        If cfg_csu = 1, the CSU will also be reset and configured.
*                  FALSE: The specified interface and its pair SERDES will be placed in reset. 
*                         OHP0 and OHP1 will be disabled together. 
*                         OHP2 and ENET_MGMT will be disabled together. 
*                         RI_RCP and DI_RCP will be disabled together.
*                         If cfg_csu = 1, the CSU will also be placed in reset.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_d8_multi_step_cfg(digi_handle_t *digi_handle, 
                                                hostmsg_d8_intf_type_enum d8_intf_type,
                                                BOOL8 cfg_csu, 
                                                BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;        
    digi_d8_serdes_status_t d8_serdes_status;
    digi_d8_serdes_status_t *d8_serdes_status_ptr  = &d8_serdes_status;
    
    UINT32 poll_itr = 0, max_poll_itr = 1000;
    PMC_ENTRY();

#if  defined(PMC_SW_SIMULATION)
    PMC_RETURN(PMC_SUCCESS);
#endif
    
    PMC_MEMSET(d8_serdes_status_ptr,0,sizeof(digi_d8_serdes_status_t));
    /* execute step 0 when we enable, we don't care when we disable */
    result = digi_serdes_internal_d8_posted_cfg(digi_handle,d8_intf_type,cfg_csu, 0,enable);

    if (PMC_SUCCESS == result && enable == TRUE)
    {
        /* release mutex and wait 250 us  */
        PMC_ATOMIC_YIELD(digi_handle,250);                     
        /* execute step 1 */
        result = digi_serdes_internal_d8_posted_cfg(digi_handle,d8_intf_type,cfg_csu, 1, enable);
        poll_itr = 0;
        while (PMC_SUCCESS == result && poll_itr < max_poll_itr && 
               (d8_serdes_status.vcoc_done_i2 == 0))
        {
            poll_itr += 1;
            /* release mutex and wait 1250 us  */
            PMC_ATOMIC_YIELD(digi_handle,1250);      
            result = digi_serdes_internal_d8_posted_status_get(digi_handle,d8_intf_type,&d8_serdes_status);            
        }
        if (PMC_SUCCESS == result && poll_itr >= max_poll_itr)
        {
            result = HOSTMSG_ERR_TIMEOUT;
        }
        if (PMC_SUCCESS == result)
        {
            /* execute step 2 */
            result = digi_serdes_internal_d8_posted_cfg(digi_handle,d8_intf_type,cfg_csu, 2, enable);
        }
        if (PMC_SUCCESS == result)
        {
            /* release mutex and wait 1 ms  */
            PMC_ATOMIC_YIELD(digi_handle,1000);       
        }
        if (PMC_SUCCESS == result)
        {
            /* execute step 3 */
            result = digi_serdes_internal_d8_posted_cfg(digi_handle,d8_intf_type,cfg_csu, 3, enable);
        }
    }
    PMC_RETURN(result);
} /* End: digi_serdes_d8_muli_step_cfg() */

/*******************************************************************************
* digi_serdes_d8_internal_posted_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to configure the D8 SERDES in multi-step mode
*
* INPUTS:
*  digi_handle   - DIGI device handle.
*  d8_intf_type  - D8 Interface type to configure
*  cfg_csu       - TRUE: Configure CSU. FALSE: Do not configure CSU
*                  Since all six SERDES ports are driven by the same CSU, 
*                  the user must specify whether the CSU should be reconfigured. 
*                  Reconfiguring the CSU will temporarily disrupt the traffic on all SERDES ports
*  step          - configuration step
*  enable        - TRUE: The specified interface will be taken out of reset and configured. 
*                        If cfg_csu = 1, the CSU will also be reset and configured.
*                  FALSE: The specified interface and its pair SERDES will be placed in reset. 
*                         OHP0 and OHP1 will be disabled together. 
*                         OHP2 and ENET_MGMT will be disabled together. 
*                         RI_RCP and DI_RCP will be disabled together.
*                         If cfg_csu = 1, the CSU will also be placed in reset.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_internal_d8_posted_cfg(digi_handle_t *digi_handle, 
                                                     hostmsg_d8_intf_type_enum d8_intf_type,
                                                     BOOL8 cfg_csu, 
                                                     UINT8 step,
                                                     BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;  
    UINT32 data[1];

    PMC_ENTRY();

    /* CMD_CFG_SERDES_D8 has the following format:
     * Word | bits 24..31 | bits 16..23   | bits 8..15 | bits 0..7 |
     *   0  | interface   | configure_csu | enable     | step      |
     */
    data[0] = ((UINT8)d8_intf_type << 24) | 
        ((UINT32)cfg_csu << 16) |
        ((UINT32)enable << 8) | (UINT32) step;

    PMC_LOG_TRACE("D8 POSTED SERDES CFG: d8 intf = %d CSU enable = %d serdes enable = %d step = %d\n",d8_intf_type,cfg_csu,enable, step);

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_CFG_SERDES_POSTED_D8_ST,data,1,stat);
    }

    /* Display FW debug info if the serdes config sequence failed */
    if (result != PMC_SUCCESS && enable) 
    {
        serdes_debug_t serdes_debug;
        PMC_ERROR status_get_result;
        status_get_result = digi_serdes_debug_status_internal_get(digi_handle,&serdes_debug,TRUE);

        PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_D8_SERDES_CONFIG,
                serdes_debug.d8_cur_step,
                serdes_debug.d8_config_rv,
                "d8 serdes debug d8_cur_step=%d d8_config_rv=%x\n", 
                serdes_debug.d8_cur_step, serdes_debug.d8_config_rv);
    }

    PMC_RETURN(result);
} /* End: digi_serdes_internal_d8_posted_cfg() */

/*******************************************************************************
* digi_serdes_internal_d8_posted_status_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to gte status from a posted D8 SERDES on 
*  the line or system side.
*
* INPUTS:
*  digi_handle            - DIGI device handle.
*  d8_intf_type  - D8 Interface type to configure
*
* OUTPUTS:
*   *d8_serdes_status_ptr  - D8 serdes status 
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_internal_d8_posted_status_get(digi_handle_t *digi_handle, 
                                                            hostmsg_d8_intf_type_enum d8_intf_type,
                                                            digi_d8_serdes_status_t *d8_serdes_status_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;  
    UINT32 data[1];
    UINT32 response[1];

    PMC_ENTRY();
      
    PMC_ASSERT((NULL != d8_serdes_status_ptr),
               HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Add message body to the previously created header. */
    data[0] = ((UINT8)d8_intf_type << 24);


    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_variable_length_query((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_REQ_SERDES_POSTED_D8_STATUS_ST,data,1,response,1,NULL,stat);
    }
    if (PMC_SUCCESS == result)
    {
        /* Return requested data. */       
        d8_serdes_status_ptr->vcoc_done_i2 = response[0];
    }

    PMC_RETURN(result);

} /* End: digi_serdes_internal_d8_posted_status_get */

/*******************************************************************************
* digi_serdes_sfi51_internal_posted_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to configure the D8 SERDES in multi-step mode
*
* INPUTS:
*  digi_handle   - DIGI device handle.
*  serdes_port_mode - SERDES rate to configure
*  step          - configuration step
*  inst          - instance number
*  enable        - TRUE: The specified interface will be taken out of reset and configured. 
*                  FALSE: The specified interface and its pair SERDES will be placed in reset. 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_internal_sfi51_posted_cfg(digi_handle_t *digi_handle, 
                                                        digi_serdes_port_mode_t  serdes_port_mode, 
                                                        UINT8 step,
                                                        UINT8 inst,
                                                        BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;  
    UINT32 data[1];
    hostmsg_sfi51_serdes_rate_t sfi51_serdes_rate;

    PMC_ENTRY();

    /* Convert SERDES rate to host message parameters */
    result = digi_serdes_sfi51_d8_rate_convert(serdes_port_mode, &sfi51_serdes_rate);

    /* CMD_CFG_SERDES_D8 has the following format:
     * Word | bits 24..31 | bits 16..23   | bits 8..15 | bits 0..7 |
     *   0  | interface   | configure_csu | enable     | step      |
     */
    data[0] = ((UINT8)sfi51_serdes_rate << 24) | 
        ((UINT32)inst << 16) |
        ((UINT32)enable << 8) | (UINT32) step;

    PMC_LOG_TRACE("SFI51 POSTED SERDES CFG: SFI51 rate = %d inst = %d serdes enable = %d step = %d\n",serdes_port_mode,inst,enable, step);

    /* Send message*/
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_CFG_SERDES_POSTED_SFI51_ST,data,1,stat);
    }

    /* Display FW debug info if the serdes config sequence failed */
    if (result != PMC_SUCCESS && enable == TRUE)
    {
        serdes_debug_t serdes_debug;
        PMC_ERROR status_get_result;
        status_get_result = digi_serdes_debug_status_internal_get(digi_handle,&serdes_debug,TRUE);

        PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_D8_SERDES_CONFIG,
                serdes_debug.sfi51_cur_step,
                serdes_debug.sfi51_config_rv,
                "SFI51 serdes debug sfi51_cur_step=%d sfi51_config_rv=%x\n", 
                serdes_debug.sfi51_cur_step, serdes_debug.sfi51_config_rv);
    }

    PMC_RETURN(result);
} /* End: digi_serdes_internal_sfi51_posted_cfg() */

/*******************************************************************************
* digi_serdes_internal_sfi51_posted_status_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to gte status from a posted SFI51 SERDES on 
*  the line or system side.
*
* INPUTS:
*  digi_handle            - DIGI device handle.
*  inst                   - instance number 
*
* OUTPUTS:
*   *sfi51_serdes_status_ptr  - SFI51 serdes status 
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_internal_sfi51_posted_status_get(digi_handle_t *digi_handle, 
                                                               UINT8 inst,
                                                               digi_sfi51_serdes_status_t *sfi51_serdes_status_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;  
    UINT32 data[1];
    UINT32 response[3];

    PMC_ENTRY();
     
    PMC_ASSERT((NULL != sfi51_serdes_status_ptr),
               HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Add message body to the previously created header. */
    data[0] = ((UINT32)inst << 16);

    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_variable_length_query((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_REQ_SERDES_POSTED_SFI51_STATUS_ST,data,1,response,3,NULL,stat);
    }


    if (PMC_SUCCESS == result)
    {

        /* Return requested data. */       
        sfi51_serdes_status_ptr->rx_vcoc_done_i2 = response[0];
        sfi51_serdes_status_ptr->tx_vcoc_done_i2 = response[1];
        sfi51_serdes_status_ptr->loc_oc_pass     = response[2];
    }

    PMC_RETURN(result);

} /* End: digi_serdes_internal_sfi51_posted_status_get */

/*******************************************************************************
* digi_serdes_single_sfi51_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to configure the SFI5.1 SERDES on 
*  the line side (a single channel in multi-step mode)
*
* INPUTS:
*  digi_handle      - DIGI device handle.
*  serdes_port_mode - SERDES rate to configure
*  inst          - instance number
*  enable        - TRUE: The specified interface will be taken out of reset and configured. 
*                  FALSE: The specified interface and its pair SERDES will be placed in reset. 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_single_sfi51_cfg(digi_handle_t           *digi_handle, 
                                              digi_serdes_port_mode_t  serdes_port_mode, 
                                              UINT8 inst,
                                              BOOL8 enable)   
{
    PMC_ERROR result = PMC_SUCCESS;        
    digi_sfi51_serdes_status_t sfi51_serdes_status;
    digi_sfi51_serdes_status_t *sfi51_serdes_status_ptr  = &sfi51_serdes_status;
    
    UINT32 poll_itr = 0, max_poll_itr = 1000;
    PMC_ENTRY();

#if  defined(PMC_SW_SIMULATION)
    PMC_RETURN(PMC_SUCCESS);
#endif
    
    PMC_MEMSET(sfi51_serdes_status_ptr,0,sizeof(digi_sfi51_serdes_status_t));
    /* execute step 0 when we enable, we don't care when we disable */
    result = digi_serdes_internal_sfi51_posted_cfg(digi_handle,serdes_port_mode, 0,inst,enable);
    if (PMC_SUCCESS == result && enable == TRUE)
    {
        /* release mutex and wait 250 us  */
        PMC_ATOMIC_YIELD(digi_handle,250);                     
        /* execute step 1 */
        result = digi_serdes_internal_sfi51_posted_cfg(digi_handle,serdes_port_mode, 1,inst,enable);
        while (PMC_SUCCESS == result && poll_itr < max_poll_itr && 
               (sfi51_serdes_status.rx_vcoc_done_i2 == 0 && 
                sfi51_serdes_status.tx_vcoc_done_i2 == 0))
        {
            poll_itr += 1;
            /* release mutex and wait 1250 us  */
            PMC_ATOMIC_YIELD(digi_handle,1250);      
            result = digi_serdes_internal_sfi51_posted_status_get(digi_handle,inst,&sfi51_serdes_status);            
        }
        if (PMC_SUCCESS == result && poll_itr >= max_poll_itr)
        {
            result = HOSTMSG_ERR_TIMEOUT;
        }
        if (PMC_SUCCESS == result)
        {
            /* execute step 2 */
            result = digi_serdes_internal_sfi51_posted_cfg(digi_handle,serdes_port_mode, 2,inst,enable);
        }
        if (PMC_SUCCESS == result)
        {
            PMC_ATOMIC_YIELD(digi_handle,1000);      
            /* execute step 3 */
            result = digi_serdes_internal_sfi51_posted_cfg(digi_handle,serdes_port_mode, 3,inst,enable);
        }
        /* execute step 4 */
        if (PMC_SUCCESS == result)
        {
            /* release mutex and wait 250 us  */
            PMC_ATOMIC_YIELD(digi_handle,250);   
            result = digi_serdes_internal_sfi51_posted_cfg(digi_handle,serdes_port_mode, 4,inst,enable);
        }
        
        /* wait for calibration lock */        
        poll_itr = 0;
        while (PMC_SUCCESS == result && poll_itr < max_poll_itr && 
               (sfi51_serdes_status.loc_oc_pass == 0 ))
        {
            poll_itr += 1;
            /* release mutex and wait 1250 us  */
            PMC_ATOMIC_YIELD(digi_handle,1250);      
            result = digi_serdes_internal_sfi51_posted_status_get(digi_handle,inst,&sfi51_serdes_status);            
        }
        if (PMC_SUCCESS == result && poll_itr >= max_poll_itr)
        {
            result = HOSTMSG_ERR_TIMEOUT;
        }        
        /* execute step 5 */
        if (PMC_SUCCESS == result)
        {
            result = digi_serdes_internal_sfi51_posted_cfg(digi_handle,serdes_port_mode, 5,inst,enable);
        }
    }
    PMC_RETURN(result);

} /* End: digi_serdes_single_sfi51_cfg() */

/*******************************************************************************
* digi_serdes_internal_posted_start_adaptation
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to start adaptation on the specified 
*  SERDES. API returns when adaptation is completed. An error code is returned
*  when adaptation failed for at least one serdes.
*
* INPUTS:
*  digi_handle     - DIGI device handle.
*  interface       - SERDES interface to configure.
*  lane            - lane to configure. 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_internal_posted_start_adaptation(digi_handle_t            *digi_handle, 
                                                               hostmsg_adapt_intf_type_t interface,
                                                               UINT32                    lane)
{
    PMC_ERROR result = PMC_SUCCESS;   
    digi_start_adapt_serdes_status_t start_adapt_serdes_status;
    digi_start_adapt_serdes_status_t *start_adapt_serdes_status_ptr  = &start_adapt_serdes_status;
    UINT8 expected_value = 0;
    UINT32 poll_itr = 0, max_poll_itr = 80;

    PMC_ENTRY();

    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);    

#if  defined(PMC_SW_SIMULATION)
    PMC_RETURN(PMC_SUCCESS);
#endif
    
    PMC_MEMSET(start_adapt_serdes_status_ptr,0,sizeof(digi_start_adapt_serdes_status_t));

    /* execute step 0*/
    result = digi_serdes_internal_posted_step_start_adaptation(digi_handle,interface,lane, 0);
    /* wait for step done  */        
    poll_itr = 0;
    /* we expect 0x1 */
    if (interface == HOSTMSG_ADAPT_INTF_S16_LINE ||
        interface == HOSTMSG_ADAPT_INTF_S16_SYS)
    {
        expected_value  = 0;
        start_adapt_serdes_status.step0_done = 1;
    } else 
    {
        expected_value  = 1;
        start_adapt_serdes_status.step0_done = 0;
    }
    while (PMC_SUCCESS == result && poll_itr < max_poll_itr && 
           (start_adapt_serdes_status.step0_done !=  expected_value))
    {
        poll_itr += 1;
        /* release mutex and wait 1250 us  */
        PMC_ATOMIC_YIELD(digi_handle,1250);      
        result = digi_serdes_internal_start_adapt_posted_status_get(digi_handle,interface,lane,&start_adapt_serdes_status);            
    }
    if (PMC_SUCCESS == result && poll_itr >= max_poll_itr)
    {
        result = HOSTMSG_ERR_TIMEOUT;
    }        
        
    /* execute step 1*/
    if (PMC_SUCCESS == result)
    {     
        result = digi_serdes_internal_posted_step_start_adaptation(digi_handle,interface,lane, 1);     
    }

    if (PMC_SUCCESS == result)
    {
        /* we have an additional step for S16 */
        if (interface == HOSTMSG_ADAPT_INTF_S16_LINE ||
            interface == HOSTMSG_ADAPT_INTF_S16_SYS)
        {
            /* wait for step done  */        
            poll_itr = 0;
            expected_value  = 1;
            start_adapt_serdes_status.step1_done = 0;
            while (PMC_SUCCESS == result && poll_itr < max_poll_itr && 
                   (start_adapt_serdes_status.step1_done != expected_value ))
            {
                poll_itr += 1;
                /* release mutex and wait 1250 us  */
                PMC_ATOMIC_YIELD(digi_handle,1250);      
                result = digi_serdes_internal_start_adapt_posted_status_get(digi_handle,interface,lane,&start_adapt_serdes_status);            
            }
            if (PMC_SUCCESS == result && poll_itr >= max_poll_itr)
            {
                result = HOSTMSG_ERR_TIMEOUT;
            }        
            if (PMC_SUCCESS == result)
            {
                result = digi_serdes_internal_posted_step_start_adaptation(digi_handle,interface,lane, 2);             
            }
        }
        PMC_ATOMIC_YIELD(digi_handle,250);
    }

    PMC_RETURN(result);
} /* End: digi_serdes_internal_posted_start_adaptation() */


/*******************************************************************************
* digi_serdes_internal_posted_step_start_adaptation
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to start adaptation on the specified 
*  SERDES. API returns when adaptation is completed. An error code is returned
*  when adaptation failed for at least one serdes.
*
* INPUTS:
*  digi_handle     - DIGI device handle.
*  interface       - SERDES interface to configure.
*  lane            - lane to configure. 
*  step            - step to configure 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_internal_posted_step_start_adaptation(digi_handle_t            *digi_handle, 
                                                                    hostmsg_adapt_intf_type_t interface,
                                                                    UINT32                    lane,
                                                                    UINT8 step)
{
    PMC_ERROR result = PMC_SUCCESS;   
    UINT32 data[1];
    UINT32 msg_ctrl;

    PMC_ENTRY();

    /* CMD_CFG_SERDES_POSTED_START_ADAPT have the following format:
     * Word | bits 24..31 | bits 16..23 | bits 8..15 | bits 0..7 |
     *   0  | interface   | lane        | step       | reserved  |
     */
    data[0] = 
        ((UINT8)interface << 24) +
        ((UINT8)lane << 16) +
        ((UINT8)step << 8) ;

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message_with_error_info((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_CFG_SERDES_POSTED_START_ADAPT_ST,data,1,&msg_ctrl,stat);
    }  
    PMC_RETURN(result);
} /* End: digi_serdes_internal_posted_step_start_adaptation() */

/*******************************************************************************
* digi_serdes_internal_start_adaptation
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to start adaptation on the specified 
*  SERDES. API returns when adaptation is completed. An error code is returned
*  when adaptation failed for at least one serdes.
*
* INPUTS:
*  digi_handle     - DIGI device handle.
*  interface       - SERDES interface to configure.
*  lane_0_19_msk   - Bitmask of lanes 0-19 to configure. 
*                    Bit 0 corresponds to lane 0.
*  lane_20_39_msk  - Bitmask of lanes 20-39 to configure. 
*                    Bit 0 corresponds to lane 20.
*                    Ignored for S16 or C8 serdes.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, otherwise a descriptive error code is returned
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_internal_start_adaptation(digi_handle_t            *digi_handle, 
                                                        hostmsg_adapt_intf_type_t interface,
                                                        UINT32                    lane_0_19_msk,
                                                        UINT32                    lane_20_39_msk)
{
    PMC_ERROR result = PMC_SUCCESS;   
    UINT32 i;    
#ifndef PMC_SW_SIMULATION
    serdes_adapt_status_t serdes_adapt_status;

#endif /*#ifndef PMC_SW_SIMULATION*/ 

    PMC_ENTRY();

    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);    
    
    if (PMC_SUCCESS == result)
    {
        /* Initialize serdes adaptation values */
        result = digi_serdes_adaptation_values_reset(digi_handle, 
                                                     interface, 
                                                     lane_0_19_msk,
                                                     lane_20_39_msk);
        PMC_ATOMIC_YIELD(digi_handle,250);
    }
    
    if (PMC_SUCCESS == result)
    {
        for (i = 0; i < 20 && PMC_SUCCESS == result; i++)
        {
            if ((lane_0_19_msk & (1 << i)) != 0)
            {
                result = digi_serdes_internal_posted_start_adaptation(digi_handle,interface,i);
                PMC_ATOMIC_YIELD(digi_handle,250);
            } 
            if ((lane_20_39_msk & (1 << i)) != 0)
            {
                result = digi_serdes_internal_posted_start_adaptation(digi_handle,interface,i+20);
                PMC_ATOMIC_YIELD(digi_handle,250);
            }             
        }
    }
    
    if (HOSTMSG_ERR_RSP_NACK == result)  
    {
        UINT32 data[2];
        UINT32 msg_ctrl;
        result = PMC_SUCCESS;
    
        /* Add message body to the previously created header. */
        /* Words 2 and 3 of CMD_CFG_SERDES_START_ADAPT have the following format:
         * Word Byte 0           Byte 1   Byte 2   Byte 3 
         *   2  interface          lane_0_19_msk
         *   3  reserved           lane_20_39_msk
         */
        data[0] = ((UINT8)interface << 24) | (lane_0_19_msk & 0xfffff);
        data[1] = lane_20_39_msk & 0xfffff;
        
        {
            HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
            result = hostmsg_send_cfg_message_with_error_info((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_CFG_SERDES_START_ADAPT_ST,data,2,&msg_ctrl,stat);
        }
        PMC_ATOMIC_YIELD(digi_handle,250);
    }

    if (PMC_SUCCESS == result)
    {
        UINT32 t8_19_00_msk = 0; 
        UINT32 t8_39_20_msk = 0;
        UINT32 s16_sys_msk = 0;
        UINT32 c8_msk = 0;
        BOOL8  reset_ilkn = TRUE;

        switch(interface)
        {
        case HOSTMSG_ADAPT_INTF_S16_SYS:
            s16_sys_msk = lane_0_19_msk;
            break;
        case HOSTMSG_ADAPT_INTF_C8:
            c8_msk = lane_0_19_msk;
            break;
        case HOSTMSG_ADAPT_INTF_T8:
            t8_19_00_msk = lane_0_19_msk;
            t8_39_20_msk = lane_20_39_msk;            
            break;
        default:
            reset_ilkn = FALSE;
            break;
        }
        if (TRUE == reset_ilkn)
        {
            result = digi_serdes_ilkn_rx_reset_toggle(digi_handle, 
                                                      t8_19_00_msk,
                                                      t8_39_20_msk,
                                                      s16_sys_msk,
                                                      c8_msk);
            PMC_ATOMIC_YIELD(digi_handle,250);
        }
    }

#ifndef PMC_SW_SIMULATION 
    if (PMC_SUCCESS == result)
    {
        /* retrieve adaptatin status */ 
        result = digi_serdes_internal_adapt_status_get(digi_handle,
                                                       &serdes_adapt_status);
        PMC_ATOMIC_YIELD(digi_handle,250);
        if (PMC_SUCCESS == result)
        {
            for (i = 0; i < (DIGI_SERDES_MAX_T8_SLICE) && PMC_SUCCESS == result; i++)
            {
                if (((serdes_adapt_status.success_lane_msk_19_0 >> i) & 0x1) != 0)
                {
                    result = digi_serdes_adaptation_values_read(digi_handle, interface, i);
                    PMC_ATOMIC_YIELD(digi_handle,250);
                    
                }
                if (PMC_SUCCESS == result && HOSTMSG_ADAPT_INTF_T8 == interface)
                {
                    if (((serdes_adapt_status.success_lane_msk_39_20 >> i) & 0x1) != 0)  
                    {
                        result = digi_serdes_adaptation_values_read(digi_handle, interface, i + (DIGI_SERDES_MAX_T8_SLICE));
                        PMC_ATOMIC_YIELD(digi_handle,250);
                    }
                }
            }
        }
        if (PMC_SUCCESS == result)
        {
            if (lane_0_19_msk != serdes_adapt_status.success_lane_msk_19_0)
            {
                result = DIGI_ERR_SERDES_ADAPT_FAILED;
            }
            else
            {
                if ( HOSTMSG_ADAPT_INTF_T8 == interface)
                {
                    if (lane_20_39_msk != serdes_adapt_status.success_lane_msk_39_20)
                    {
                        result = DIGI_ERR_SERDES_ADAPT_FAILED;
                    }
                }
            }
        }
    }
#endif /* PMC_SW_SIMULATION  */


    PMC_RETURN(result);
} /* End: digi_serdes_internal_start_adaptation() */

/*******************************************************************************
* digi_serdes_internal_start_adpat_posted_status_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to gee status from a posted SERDES adaptation
*
* INPUTS:
*  digi_handle     - DIGI device handle.
*  interface       - SERDES interface to configure.
*  lane            - lane to configure. 
*
* OUTPUTS:
*   *start_adapt_serdes_status_ptr  - START adaptation serdes status 
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_internal_start_adapt_posted_status_get(digi_handle_t *digi_handle, 
                                                                     hostmsg_adapt_intf_type_t interface,
                                                                     UINT32                    lane,
                                                                     digi_start_adapt_serdes_status_t *start_adapt_serdes_status_ptr)
{
    PMC_ERROR result = PMC_SUCCESS; 
    UINT32 data[1];
    UINT32 response[2];

    PMC_ENTRY();

    PMC_ASSERT((NULL != start_adapt_serdes_status_ptr),
               HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Add message body to the previously created header. */
    /* Words 2 and 3 of CMD_REQ_SERDES_GET_POSTED_START_ADAPT have the following format:
     * Word Byte 0           Byte 1   Byte 2   Byte 3 
     *   2  interface        lane     reserved0
     */
    data[0] = 
        ((UINT8)interface << 24) +
        ((UINT8)lane << 16) ;

    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_variable_length_query((pmc_handle_t *)digi_handle,MODID_SERDES,HOSTMSG_CMD_REQ_SERDES_POSTED_START_ADAPT_STATUS_ST,data,1,response,2,NULL,stat);
    }

    if (PMC_SUCCESS == result)
    {
        /* Return requested data. */       
        start_adapt_serdes_status_ptr->step0_done = response[0];
        start_adapt_serdes_status_ptr->step1_done = response[1];
    }

    PMC_RETURN(result);

} /* End: digi_serdes_internal_start_adapt_posted_status_get */


/*******************************************************************************
* digi_serdes_non_otn_acb_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is an internal helper function to configure ACB settings for CPB 
*   related applications.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   intf             - Interface configuration related to, valid
*                      values:\n
*                      DIGI_SERDES_LINE_INTF\n
*                      DIGI_SERDES_SYSTEM_INTF\n
*                      DIGI_SERDES_SFI_51_INTF
*   *input_pc_ptr    - util_global_switch_data_t pointer defining inbound port
*   *output_pc_ptr   - util_global_switch_data_t pointer defining outbound port
*   acb_timing_mode  - ACB mode of operation.
*   num_10g_data_lane  - Number of 10G data lane 
*   force_enet_timing_mode - when this config is set, ENET timing mode is forced 
*                            to the specified
*
* OUTPUTS:
*  *acb_ft_prov_cfg_ptr - ACB provision configuration
*   enet_mapping       - TRUE : enet mapping case
*                        FALSE : not enet mapping case
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_non_otn_acb_cfg_get(digi_handle_t *digi_handle,
                                                  digi_serdes_intf_t intf,
                                                  util_global_switch_data_t *input_pc_ptr, 
                                                  util_global_switch_data_t *output_pc_ptr,
                                                  digi_serdes_acb_mode_t acb_timing_mode,
                                                  UINT32 num_10g_data_lane,
                                                  digi_enet_tx_timing_mode_t force_enet_timing_mode,
                                                  acb_ft_prov_cfg_t *acb_ft_prov_cfg_ptr,
                                                  BOOL8 *enet_mapping)
{    
    DOUBLE capture_rate = 0;
    DOUBLE transmit_rate = 0;
    DOUBLE server_rate = 0;
    UINT32 c_input_pkt_size= 0;
    UINT32 c_input_pkt_per_int = 0;
    UINT32 c_input_pkt_per_n = 0;
    UINT32 c_input_pkt_per_d = 0;
    UINT32 b_input_pkt_size= 0;
    UINT32 b_input_pkt_per_int = 0;
    UINT32 b_input_pkt_per_n = 0;
    UINT32 b_input_pkt_per_d = 0;
    util_global_cpb_port_data_t *local_input_pc_ptr =  NULL;
    util_global_cpb_port_data_t *local_output_pc_ptr = NULL;
    util_global_switch_data_def_t *input_pc_data_ptr = NULL;
    util_global_switch_data_def_t *output_pc_data_ptr = NULL;
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    digi_enet_client_chnl_def_t *enet_chnl_handle_ptr;
    digi_cbr_client_chnl_def_t *cbrc_chnl_handle_ptr;
    digi_mapping_mode_t map_mode = LAST_DIGI_MAP;
    digi_serdes_port_mode_t serdes_mode = LAST_DIGI_SERDES_PORT_MODE;
    
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    acb_ft_prov_cfg_ptr->mode             = ACB_FT_MODE_NOMINAL;
    input_pc_data_ptr = (util_global_switch_data_def_t *)input_pc_ptr;
    output_pc_data_ptr = (util_global_switch_data_def_t *)output_pc_ptr;
    local_input_pc_ptr = &(input_pc_data_ptr->cpb_data);
    local_output_pc_ptr = &(output_pc_data_ptr->cpb_data);


    PMC_LOG_TRACE("\n local_output_pc_ptr->port_type:%d\
               \n local_input_pc_ptr->port_type:%d\
               \n local_output_pc_ptr->channel:%d\
               \n local_input_pc_ptr->channel:%d\
               \n acb_timing_mode:%d\n",
                  local_output_pc_ptr->port_type, local_input_pc_ptr->port_type,local_output_pc_ptr->channel, 
                  local_input_pc_ptr->channel, acb_timing_mode);

    if(DIGI_SERDES_ACB_MODE_B_ON_C == acb_timing_mode)
    {
        acb_ft_prov_cfg_ptr->mode             = ACB_FT_MODE_B_ON_C;
        /*get coreotn FO2 mpmo data*/

        result = coreotn_mpmo_data_get(digi_handle->coreotn_handle, COREOTN_MPMO_TO_MAPOTN, 
                                       local_input_pc_ptr->channel, &b_input_pkt_size, &b_input_pkt_per_int, 
                                       &b_input_pkt_per_n, &b_input_pkt_per_d);
    }
    else if(DIGI_SERDES_ACB_MODE_B_AND_C == acb_timing_mode ||
            DIGI_SERDES_ACB_MODE_C == acb_timing_mode)
    {
        acb_ft_prov_cfg_ptr->mode  = DIGI_SERDES_ACB_MODE_B_AND_C == acb_timing_mode?ACB_FT_MODE_B_AND_C:ACB_FT_MODE_C;
        if (DIGI_ENET_TX_LINE_TIMING_MODE == force_enet_timing_mode)
        {
            if(digi_handle->var.digi_m1_ctxt.pmm_lite_ref_clk_src == DIGI_M1_PMM_LITE_CLK_REF_IRIG_CLKI ||
               digi_handle->var.digi_m1_ctxt.pmm_lite_ref_clk_src == DIGI_M1_PMM_LITE_CLK_REF_IRIG_DATI ||
               digi_handle->var.digi_m1_ctxt.pmm_lite_ref_clk_src == DIGI_M1_PMM_LITE_CLK_REF_PTP)
            {
                acb_ft_prov_cfg_ptr->mode = ACB_FT_MODE_LINE_TIMING_125MHz;
            }
            else if(digi_handle->var.digi_m1_ctxt.pmm_lite_ref_clk_src == DIGI_M1_PMM_LITE_CLK_REF_PGMRCLK0 ||
                    digi_handle->var.digi_m1_ctxt.pmm_lite_ref_clk_src == DIGI_M1_PMM_LITE_CLK_REF_PGMRCLK1 ||
                    digi_handle->var.digi_m1_ctxt.pmm_lite_ref_clk_src == DIGI_M1_PMM_LITE_CLK_REF_PGMRCLK2 ||
                    digi_handle->var.digi_m1_ctxt.pmm_lite_ref_clk_src == DIGI_M1_PMM_LITE_CLK_REF_PGMRCLK0)
            {
                acb_ft_prov_cfg_ptr->mode = ACB_FT_MODE_LINE_TIMING_PGMRCLK;
            }
            else
            {
                PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            }
            c_input_pkt_size    = digi_handle->var.digi_m1_ctxt.pmm_lite_pkt_size;
            c_input_pkt_per_int = digi_handle->var.digi_m1_ctxt.pmm_lite_pkt_period;
            c_input_pkt_per_n = 0;
            c_input_pkt_per_d = 1;
        } else if(UTIL_GLOBAL_CPB_PORT_CBRC == local_output_pc_ptr->port_type)
        {
            /*get CBRC mpmo data*/
            result = cbrc_mpmo_data_get(digi_handle->cbrc_handle, local_output_pc_ptr->channel, 
                                        &c_input_pkt_size, &c_input_pkt_per_int, 
                                        &c_input_pkt_per_n, &c_input_pkt_per_d);
        }
        else if (UTIL_GLOBAL_CPB_PORT_ENET_LINE == local_output_pc_ptr->port_type)
        {
            /*get ENET line mpmo data */
            result = enet_mpmo_data_get(digi_handle->enet_line_handle, local_output_pc_ptr->channel, 
                                        &c_input_pkt_size, &c_input_pkt_per_int, 
                                        &c_input_pkt_per_n, &c_input_pkt_per_d);
        }
        else if (UTIL_GLOBAL_CPB_PORT_ENET_SYS == local_output_pc_ptr->port_type)
        {
            /*get ENET sys mpmo data */
            result = enet_mpmo_data_get(digi_handle->enet_sys_handle, local_output_pc_ptr->channel, 
                                        &c_input_pkt_size, &c_input_pkt_per_int, 
                                        &c_input_pkt_per_n, &c_input_pkt_per_d);
        }
        else
        {
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
        }
        
        if(PMC_SUCCESS == result && DIGI_SERDES_ACB_MODE_B_AND_C == acb_timing_mode)
        {
            /*get coreotn FO2 mpmo data */
            result = coreotn_mpmo_data_get(digi_handle->coreotn_handle, COREOTN_MPMO_TO_MAPOTN, 
                                           local_input_pc_ptr->channel, &b_input_pkt_size, 
                                           &b_input_pkt_per_int, &b_input_pkt_per_n, &b_input_pkt_per_d);
        }
    } else  if (DIGI_SERDES_ACB_MODE_LOOP_TIMING == acb_timing_mode) 
    {
        acb_ft_prov_cfg_ptr->mode  = ACB_FT_MODE_LOOP_TIMING;
        /* packet size/period are internal computed in the ACB_FT driver */
    }                                                                
    
    if(PMC_SUCCESS == result)
    {
        if (DIGI_SERDES_ACB_MODE_LOOP_TIMING == acb_timing_mode ||
            DIGI_ENET_TX_LINE_TIMING_MODE == force_enet_timing_mode) 
        {
            /* we only look at the output */
            enet_chnl_handle_ptr = (digi_enet_client_chnl_def_t*)(output_pc_ptr);
            map_mode = (digi_mapping_mode_t) 0;
            serdes_mode = enet_chnl_handle_ptr->serdes_mode;
        } else if(UTIL_GLOBAL_CPB_PORT_MAPOTN == local_input_pc_ptr->port_type)
        {
            mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(input_pc_ptr);
            map_mode = mapper_chnl_handle_ptr->mode;
            digi_mapotn_serdes_mode_get(map_mode, intf, &serdes_mode);
        }
        else if(UTIL_GLOBAL_CPB_PORT_ENET_LINE == local_input_pc_ptr->port_type ||
                UTIL_GLOBAL_CPB_PORT_ENET_SYS == local_input_pc_ptr->port_type)
        {
            enet_chnl_handle_ptr = (digi_enet_client_chnl_def_t*)(input_pc_ptr);
            map_mode = (digi_mapping_mode_t) 0;
            serdes_mode = enet_chnl_handle_ptr->serdes_mode;
        }
        else if(UTIL_GLOBAL_CPB_PORT_CBRC == local_input_pc_ptr->port_type)
        {
            cbrc_chnl_handle_ptr = (digi_cbr_client_chnl_def_t*)(input_pc_ptr);
            map_mode = (digi_mapping_mode_t) 0;
            serdes_mode = cbrc_chnl_handle_ptr->serdes_mode;
        }
        
        result = digi_acb_capture_transmit_rate_calc(digi_handle, serdes_mode, map_mode, acb_timing_mode, 
                                                     &capture_rate, &transmit_rate, &server_rate, enet_mapping);

        /* modify capture rate if PGMRCLK to PMM_LITE is used */
        if(acb_ft_prov_cfg_ptr->mode == ACB_FT_MODE_LINE_TIMING_PGMRCLK)
        {
            capture_rate = digi_handle->var.digi_m1_ctxt.pmm_lite_ref_clk_freq * 8;
        }
    }
  
    if(PMC_SUCCESS == result)
    {
        /* prepare ACB_FT configuration */          
        acb_ft_prov_cfg_ptr->packet_size_C    = c_input_pkt_size;
        acb_ft_prov_cfg_ptr->packet_per_int_C = c_input_pkt_per_int;
        acb_ft_prov_cfg_ptr->packet_per_n_C   = c_input_pkt_per_n;
        acb_ft_prov_cfg_ptr->packet_per_d_C   = c_input_pkt_per_d;
        acb_ft_prov_cfg_ptr->packet_size_B    = b_input_pkt_size;
        acb_ft_prov_cfg_ptr->packet_per_int_B = b_input_pkt_per_int;
        acb_ft_prov_cfg_ptr->packet_per_n_B   = b_input_pkt_per_n;
        acb_ft_prov_cfg_ptr->packet_per_d_B   = b_input_pkt_per_d;
        /* set reference clock */
        if (DIGI_ENET_TX_LINE_TIMING_MODE == force_enet_timing_mode && acb_ft_prov_cfg_ptr->mode == ACB_FT_MODE_LINE_TIMING_125MHz)
        {
            acb_ft_prov_cfg_ptr->ref_clk = digi_handle->var.digi_m1_ctxt.pmm_lite_ref_clk_freq;
        } else
        {
            acb_ft_prov_cfg_ptr->ref_clk = UTIL_GLOBAL_REF_FREQ/2;
        }

        PMC_LOG_TRACE("acb_ft_prov_cfg_ptr->packet_size_C    = %d\n",acb_ft_prov_cfg_ptr->packet_size_C   );
        PMC_LOG_TRACE("acb_ft_prov_cfg_ptr->packet_per_int_C = %d\n",acb_ft_prov_cfg_ptr->packet_per_int_C);
        PMC_LOG_TRACE("acb_ft_prov_cfg_ptr->packet_per_n_C   = %d\n",acb_ft_prov_cfg_ptr->packet_per_n_C  );
        PMC_LOG_TRACE("acb_ft_prov_cfg_ptr->packet_per_d_C   = %d\n",acb_ft_prov_cfg_ptr->packet_per_d_C  );
        PMC_LOG_TRACE("acb_ft_prov_cfg_ptr->packet_size_B    = %d\n",acb_ft_prov_cfg_ptr->packet_size_B   );
        PMC_LOG_TRACE("acb_ft_prov_cfg_ptr->packet_per_int_B = %d\n",acb_ft_prov_cfg_ptr->packet_per_int_B);
        PMC_LOG_TRACE("acb_ft_prov_cfg_ptr->packet_per_n_B   = %d\n",acb_ft_prov_cfg_ptr->packet_per_n_B  );
        PMC_LOG_TRACE("acb_ft_prov_cfg_ptr->packet_per_d_B   = %d\n",acb_ft_prov_cfg_ptr->packet_per_d_B    );

        acb_ft_prov_cfg_ptr->capture_rate  = capture_rate;
        acb_ft_prov_cfg_ptr->transmit_rate = transmit_rate;
        acb_ft_prov_cfg_ptr->server_rate   = server_rate;
        acb_ft_prov_cfg_ptr->client_type   = num_10g_data_lane!=10 ? ACB_FT_DEMAPPED:ACB_FT_100GE_CBR;
        acb_ft_prov_cfg_ptr->num_lane      = num_10g_data_lane;
    }

    PMC_RETURN(result);

} /* digi_serdes_non_otn_acb_cfg_get */


/*******************************************************************************
* digi_serdes_otn_acb_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is an internal helper function to configure ACB settings for COREOTN 
*   related applications.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   serdes_mode      - Configuration mode for serdes port
*   mpmo_instance    - MPMO instance in COREOTN. See coreotn_mpmo_inst_t
*   chnl             - Channel ID valid range 0-96
*   num_10g_data_lane  - Number of 10G data lane
*
* OUTPUTS:
*  *acb_ft_prov_cfg_ptr - ACB provision configuration
*   enet_mapping       - TRUE : enet mapping case
*                        FALSE : not enet mapping case
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_otn_acb_cfg_get(digi_handle_t *digi_handle,
                                             digi_serdes_port_mode_t serdes_mode,
                                             coreotn_mpmo_inst_t mpmo_instance,
                                             UINT32 chnl,
                                             UINT32 num_10g_data_lane,
                                             acb_ft_prov_cfg_t *acb_ft_prov_cfg_ptr,
                                             BOOL8 *enet_mapping)
{
    DOUBLE capture_rate = 0;
    DOUBLE transmit_rate = 0;
    DOUBLE server_rate = 0;
    UINT32 c_input_pkt_size= 0;
    UINT32 c_input_pkt_per_int = 0;
    UINT32 c_input_pkt_per_n = 0;
    UINT32 c_input_pkt_per_d = 0;
    DOUBLE temp_rate_odu4 = UTIL_GLOBAL_ODU4_RATE;
    DOUBLE temp_rate_odu3 = UTIL_GLOBAL_ODU3_RATE;
    DOUBLE temp_rate_odu3e1 = UTIL_GLOBAL_ODU3E1_RATE;
    DOUBLE temp_rate_odu3e2 = UTIL_GLOBAL_ODU3E2_RATE;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    /*get coreotn mpmo data*/
    result = coreotn_mpmo_data_get(digi_handle->coreotn_handle, mpmo_instance, chnl, 
                                   &c_input_pkt_size, &c_input_pkt_per_int,
                                   &c_input_pkt_per_n, &c_input_pkt_per_d);
                                     
    
    if(PMC_SUCCESS == result)
    {
        result = digi_acb_capture_transmit_rate_calc(digi_handle, 
                                                     serdes_mode, 
                                                     (digi_mapping_mode_t) 0, 
                                                     DIGI_SERDES_ACB_MODE_C, 
                                                     &capture_rate, 
                                                     &transmit_rate, 
                                                     &server_rate,
                                                     enet_mapping);
    }
    
    if(PMC_SUCCESS == result)
    {
        /* prepare ACB_FT configuration */
        acb_ft_prov_cfg_ptr->mode             = ACB_FT_MODE_C;     
        acb_ft_prov_cfg_ptr->packet_size_C    = c_input_pkt_size;
        acb_ft_prov_cfg_ptr->packet_per_int_C = c_input_pkt_per_int;
        acb_ft_prov_cfg_ptr->packet_per_n_C   = c_input_pkt_per_n;
        acb_ft_prov_cfg_ptr->packet_per_d_C   = c_input_pkt_per_d;
        acb_ft_prov_cfg_ptr->ref_clk       = UTIL_GLOBAL_REF_FREQ/2;        
        acb_ft_prov_cfg_ptr->capture_rate  = capture_rate;
        acb_ft_prov_cfg_ptr->transmit_rate = transmit_rate;
        acb_ft_prov_cfg_ptr->server_rate   = server_rate;
        if((UINT32)capture_rate == (UINT32)temp_rate_odu4   ||
           (UINT32)capture_rate == (UINT32)temp_rate_odu3   ||
           (UINT32)capture_rate == (UINT32)temp_rate_odu3e1 ||
           (UINT32)capture_rate == (UINT32)temp_rate_odu3e2)
        {
            acb_ft_prov_cfg_ptr->client_type   = ACB_FT_OTN_ODU4_ODU3;
        }
        else
        {
            acb_ft_prov_cfg_ptr->client_type   = ACB_FT_OTN;
        }
        
        acb_ft_prov_cfg_ptr->num_lane      = num_10g_data_lane;


    }

    PMC_RETURN(result);

} /* digi_serdes_otn_acb_cfg_get */


#endif /* DOXYGEN_PUBLIC_ONLY */
/*
** End of file
*/

