/*******************************************************************************
*  T (C) 2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
*   DESCRIPTION : This file contains definitions for C functions for
*   configuring SIFD features.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _DIGI_RESOURCES_LOC_H
#define _DIGI_RESOURCES_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "digi_util_loc.h"
  /*#include "digi_loc.h"*/
#include "pmc_log.h"
#include "pmc_types.h"

/*
** Constants
*/

/*
** Enumerated Types
*/

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*
** Function Prototypes
*/
PUBLIC UINT16 digi_get_index_and_set_cpb_data(digi_handle_t *digi_handle,
                                               UINT8 element_index,
                                               digi_resource_next_handle_t element_type,
                                               digi_resource_handle_summary_t *digi_resource_handle);

PUBLIC UINT16 digi_get_index_and_set_oduk_data(digi_handle_t *digi_handle,
                                                UINT8 element_index,
                                                digi_resource_next_handle_t element_type,
                                                digi_resource_handle_summary_t *digi_resource_handle);

PUBLIC PMC_ERROR digi_resource_handle_demap(digi_handle_t *digi_handle,
                                             digi_resource_handle_summary_t *digi_resource_handle,
                                             digi_resource_next_handle_t handle_type,
                                             UINT16 handle_index,
                                             digi_resource_next_handle_t switch_handle);

PUBLIC PMC_ERROR digi_resource_clean_source_and_destination(digi_handle_t *digi_handle,
                                                             digi_resource_handle_summary_t *digi_resource_handle,
                                                             digi_switch_ctxt_t *switch_ptr,
                                                             UINT16 port_type,
                                                             UINT32 port_channel,
                                                             digi_resource_next_handle_t switch_type);


PUBLIC PMC_ERROR digi_resource_deactivate_demap(digi_handle_t *digi_handle,
                                                 digi_resource_handle_summary_t *digi_resource_handle,
                                                 UINT16 port_type,
                                                 UINT32 port_chnl,
                                                 digi_resource_next_handle_t switch_type);

PUBLIC void digi_resource_decrement_destinations(digi_handle_t *digi_handle,
                                                  digi_resource_handle_summary_t * digi_resource_handle,
                                                  digi_resource_next_handle_t handle_type,
                                                  UINT16 index,
                                                  digi_resource_next_handle_t switch_type);

PUBLIC PMC_ERROR digi_resource_find_switch_index(digi_handle_t *digi_handle,
                                                  digi_resource_handle_summary_t *digi_resource_handle, 
                                                  UINT16 port_type, 
                                                  UINT32 port_chnl, 
                                                  digi_resource_next_handle_t switch_type, 
                                                  UINT16 *switch_index);

PUBLIC PMC_ERROR digi_resource_restart_release_port(digi_handle_t *digi_handle,
                                                    digi_resource_handle_summary_t *digi_resource_handle,
                                                    void *parent_handle_ptr,
                                                    digi_resource_next_handle_t next_handle,
                                                    UINT16 next_index,
                                                    BOOL8 is_oduk_caller,
                                                    digi_direction_check_t direction);
  
PUBLIC PMC_ERROR digi_resource_deprovision_channel(digi_handle_t *digi_handle,
                                                    digi_handle_id_t chnl_handle,
                                                    void *chnl_handle_ptr);


PUBLIC util_global_oduk_port_t digi_resource_get_oduk_port_type(digi_resource_next_handle_t resource_type,
                                                                 UINT32 *channel);

PUBLIC util_global_cpb_port_t digi_resource_get_cpb_port_type(digi_resource_handle_summary_t *digi_resource_handle,
                                                              digi_resource_next_handle_t resource_type,
                                                              UINT16 index);


PUBLIC digi_resource_next_handle_t digi_resource_get_cpb_resource_type(digi_handle_t *digi_handle,
                                                                        digi_resource_handle_summary_t *digi_resource_handle, 
                                                                         util_global_cpb_port_t dest_port_type, 
                                                                         UINT32 *dest_channel);

PUBLIC BOOL8 digi_resource_is_cbrc_operational(digi_handle_t *digi_handle,
                                               digi_resource_handle_summary_t *digi_resource_handle,
                                               digi_serdes_port_intf_ctxt_t *serdes_port_ptr,
                                               BOOL8 check_all,
                                               digi_direction_check_t direction);

PUBLIC BOOL8 digi_resource_is_enet_operational(digi_handle_t *digi_handle,
                                               digi_resource_handle_summary_t *digi_resource_handle,
                                               digi_serdes_port_intf_ctxt_t *serdes_port_ptr,
                                               BOOL8 check_all,
                                               digi_direction_check_t direction);

PUBLIC BOOL8 digi_resource_is_lineotn_operational(digi_handle_t *digi_handle,
                                                  digi_resource_handle_summary_t *digi_resource_handle,
                                                  digi_serdes_port_intf_ctxt_t *serdes_port_ptr,
                                                  BOOL8 is_line_side,
                                                  BOOL8 check_all,
                                                  digi_direction_check_t direction,
                                                  BOOL8 *is_operational);

PUBLIC BOOL8 digi_resource_is_mapper_operational(digi_handle_t *digi_handle,
                                                 digi_resource_handle_summary_t *digi_resource_handle,
                                                 UINT16 map_index,
                                                 digi_resource_next_handle_t switch_type,
                                                 BOOL8 is_source,
                                                 digi_direction_check_t direction);

PUBLIC void digi_resource_release_enet(digi_handle_t *digi_handle,
                                       digi_resource_handle_summary_t *digi_resource_handle,
                                       digi_serdes_port_intf_ctxt_t *serdes_port_ptr);

PUBLIC void digi_resource_release_lineotn(digi_handle_t *digi_handle,
                                          digi_resource_handle_summary_t *digi_resource_handle,
                                          digi_serdes_port_intf_ctxt_t *serdes_port_ptr,
                                          BOOL8 is_lineotn);

PUBLIC void digi_resource_release_cbrc(digi_handle_t *digi_handle,
                                       digi_resource_handle_summary_t *digi_resource_handle,
                                       digi_serdes_port_intf_ctxt_t *serdes_port_ptr);


PUBLIC BOOL8 digi_resource_get_oduk_switch_index(util_global_oduk_port_t port_type,
                                                 UINT32 channel,
                                                 UINT16 *ingress_index);

PUBLIC BOOL8 digi_resource_get_cpb_switch_index(util_global_cpb_port_t port_type,
                                                UINT32 channel,
                                                UINT16 *ingress_index);

PUBLIC void digi_resource_set_source_oduk_switch(digi_handle_t *digi_handle,
                                         util_global_switch_data_def_t *switch_ptr,
                                         UINT16 switch_index,
                                         digi_resource_handle_summary_t *digi_resource_handle);

PUBLIC void digi_resource_set_source_cpb_switch(digi_handle_t *digi_handle,
                                                util_global_switch_data_def_t *switch_ptr,
                                                UINT16 switch_index,
                                                BOOL8 enet_pmon,
                                                digi_resource_handle_summary_t *digi_resource_handle);

PUBLIC PMC_ERROR digi_resource_get_port_context(digi_handle_t *digi_handle,
                                                UINT32 port_uid,
                                                digi_serdes_port_def_t **port_ctxt_pptr);

#ifndef DOXYGEN_PUBLIC_ONLY
PUBLIC void digi_resource_clean(digi_handle_t *digi_handle);
PUBLIC void digi_resource_clean_switch(digi_handle_t *digi_handle,
                                       digi_resource_handle_summary_t *digi_resource_handle_ptr);
PUBLIC void digi_resource_clean_int_pmon(digi_handle_t *digi_handle);
PUBLIC void digi_resource_clean_enet_enh_pmon(digi_handle_t *digi_handle);

#endif


PUBLIC void digi_resource_clean_cpb(digi_handle_t *digi_handle,
                                    digi_resource_handle_summary_t *digi_resource_handle);

PUBLIC void digi_resource_clean_oduk(digi_handle_t *digi_handle,
                                     digi_resource_handle_summary_t *digi_resource_handle);

PUBLIC void digi_resource_clean_mapotn(digi_handle_t *digi_handle,
                                       digi_resource_handle_summary_t *digi_resource_handle);

PUBLIC PMC_ERROR digi_resource_populate_cpb_switch_data(digi_handle_t *digi_handle,
                                                        digi_handle_id_t chnl_hndl_type,
                                                        digi_chnl_hndl_t *chnl_hndl_ptr,
                                                        digi_mapotn_conn_use_t mapotn_use,
                                                        digi_chnl_hndl_conn_t *source_ptr,
                                                        BOOL8 *companion_source_ptr,
                                                        digi_chnl_hndl_conn_t *primary_dest_ptr,
                                                        digi_chnl_hndl_conn_t mult_dest_ptr[DIGI_RESOURCE_MULTI_DEST_MAX]);

PUBLIC PMC_ERROR digi_resource_populate_oduk_switch_data(digi_handle_t *digi_handle,
                                                         digi_handle_id_t chnl_hndl_type,
                                                         digi_chnl_hndl_t *chnl_hndl_ptr,
                                                         digi_mapotn_conn_use_t mapotn_use,
                                                         digi_chnl_hndl_conn_t *source_ptr,
                                                         BOOL8 *companion_source_ptr,
                                                         digi_chnl_hndl_conn_t *primary_dest_ptr,
                                                         digi_chnl_hndl_conn_t mult_dest_ptr[DIGI_RESOURCE_MULTI_DEST_MAX]);

PUBLIC PMC_ERROR digi_resource_get_serdes_channel_handle(digi_handle_t *digi_handle,
                                                         digi_serdes_port_def_t *local_ctxt_ptr,
                                                         digi_serdes_prov_state_t serdes_prov_state,
                                                         digi_handle_id_t *chnl_hndl_type_ptr,
                                                         digi_chnl_hndl_t **chnl_hndl_pptr);

PUBLIC PMC_ERROR digi_resource_internal_serdes_port_release(digi_handle_t *digi_handle,
                                                            digi_handle_id_t digi_chnl_hndl_type,
                                                            digi_chnl_hndl_t *digi_chnl_hndl_ptr,
                                                            BOOL8 switch_release,
                                                            BOOL8 mapotn_release);

PUBLIC PMC_ERROR digi_resource_release_oduk_resource(digi_handle_t *digi_handle,
                                                     digi_chnl_hndl_t *chnl_ctxt_ptr,
                                                     util_global_oduk_port_t oduk_port,
                                                     UINT32 chnl_id,
                                                     BOOL8 switch_release,
                                                     BOOL8 mapotn_release,
                                                     util_global_cpb_connect_t *oduk_connect_state_ptr);

PUBLIC PMC_ERROR digi_resource_release_cpb_resource(digi_handle_t *digi_handle,
                                                    digi_chnl_hndl_t *chnl_hndl_ptr,
                                                    util_global_cpb_port_t cpb_port,
                                                    UINT32 chnl_id,
                                                    BOOL8 switch_release,
                                                    BOOL8 mapotn_release,
                                                    util_global_cpb_connect_t *cpb_connect_state_ptr);


PUBLIC PMC_ERROR digi_resource_lo_channel_get_by_trib_slot(digi_handle_t *digi_handle,
                                                           UINT32 num_cal_entries,
                                                           UINT32 ho_cal_array[DIGI_OTN_SERVER_MO_CHNL_MAX],
                                                           digi_handle_id_t ho_chnl_type,
                                                           UINT32 tx_trib_slot[3],
                                                           UINT32 rx_trib_slot[3],
                                                           digi_otn_time_slot_occupied_t *slot_use_ptr,
                                                           digi_otn_odu_chnl_t *lo_chnl_hndl_ptr[DIGI_OTN_SERVER_MAX_TRIBSLOT]);

PUBLIC PMC_ERROR digi_resource_get_coreotn_subcontainers(digi_handle_t *digi_handle,
                                                         digi_handle_id_t ho_chnl_hndl_type,
                                                         digi_otn_odu_chnl_t *ho_chnl_hndl_ptr,
                                                         UINT32 ho_cal_array[DIGI_OTN_SERVER_MO_CHNL_MAX],
                                                         UINT32 *num_entries_ptr);

PUBLIC BOOL8 digi_resource_is_handle_provisioned(digi_handle_t *digi_handle,
                                                 digi_resource_handle_summary_t *digi_resource_handle);

PUBLIC PMC_ERROR digi_resource_release_subcontainers(digi_handle_t *digi_handle,
                                                     digi_otn_odu_chnl_t *ho_chnl_ctxt_ptr,
                                                     digi_handle_id_t odu_level,
                                                     BOOL8 switch_release,
                                                     BOOL8 mapotn_release,
                                                     util_global_cpb_connect_t *oduk_connect_state_ptr);

PUBLIC BOOL8 digi_resource_is_path_operational(digi_handle_t *digi_handle,
                                               digi_resource_handle_summary_t *digi_resource_handle_ptr,
                                               digi_serdes_intf_t first_intf);

#ifdef PMC_DEPRECATED
PUBLIC PMC_ERROR digi_resource_release_port_to_switch(digi_handle_t *digi_handle,
                                                       digi_resource_handle_summary_t *digi_resource_handle,
                                                       void *parent_handle_ptr,
                                                       digi_resource_next_handle_t next_handle,
                                                       UINT16 next_index);

PUBLIC PMC_ERROR digi_resource_demap_switch(digi_handle_t *digi_handle,
                                             digi_resource_handle_summary_t *digi_resource_handle,
                                             digi_resource_next_handle_t next_handle,
                                             UINT16 next_index);
#endif /* PMC_DEPRECATED */

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_RESOURCES_API_H */

/*
** end of file
*/

