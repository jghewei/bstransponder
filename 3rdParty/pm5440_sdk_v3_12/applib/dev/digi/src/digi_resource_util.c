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
#include "digi_api.h"
#include "digi_resource_api.h"
#include "digi_loc.h"
#include "digi_resource_loc.h"
#include "digi_enet_loc.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
#define DIGI_RESOURCE_LAST_INDEX 0xFFFF

/*! flag to differentiate line from sysotn side */
extern BOOL8 is_line_side; 
/*! Global defined ODUK Switch connect array variable */
extern util_global_cpb_connect_t *g_oduk_connect_data_ptr;
/*! Global defined CPB Switch connect array variable */
extern util_global_cpb_connect_t *g_cpb_connect_data_ptr;

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */

PRIVATE void digi_resource_clean_ilkn(digi_handle_t *digi_handle,
                                      digi_resource_handle_summary_t *digi_resource_handle,
                                      util_global_cpb_connect_t *cpb_connect_data_ptr);

PRIVATE void digi_resource_clean_otn(digi_handle_t *digi_handle,
                                     digi_resource_handle_summary_t *digi_resource_handle,
                                     util_global_cpb_connect_t *oduk_connect_data_ptr);

PRIVATE BOOL8 digi_resource_is_switch_operational(digi_handle_t *digi_handle,
                                                  digi_resource_handle_summary_t *digi_resource_handle,
                                                  UINT16 switch_index,
                                                  digi_resource_next_handle_t switch_type,
                                                  BOOL8 is_source,
                                                  digi_direction_check_t direction);

PRIVATE BOOL8 digi_resource_is_otn_operational(digi_handle_t *digi_handle,
                                               digi_resource_handle_summary_t *digi_resource_handle,
                                               digi_resource_next_handle_t otn_handle,
                                               UINT16 otn_index,
                                               BOOL8 is_from_oduk,
                                               digi_direction_check_t direction);

PRIVATE BOOL8 digi_resource_is_ilkn_operational(digi_handle_t *digi_handle,
                                                digi_resource_handle_summary_t *digi_resource_handle,
                                                digi_resource_next_handle_t otn_handle,
                                                UINT16 otn_index,
                                                digi_resource_next_handle_t switch_type,
                                                digi_direction_check_t direction);

PRIVATE BOOL8  digi_resource_check_operational_level(digi_handle_t *digi_handle,
                                                     digi_resource_handle_summary_t *digi_resource_handle,
                                                     digi_resource_next_handle_t handle_type,
                                                     UINT16 handle_index,
                                                     digi_resource_next_handle_t switch_type,
                                                     digi_direction_check_t direction);

PRIVATE BOOL8  digi_resource_find_secondary_check_op_level(digi_handle_t *digi_handle,
                                                           digi_resource_handle_summary_t *digi_resource_handle,
                                                           digi_resource_next_handle_t handle_type,
                                                           UINT16 handle_index,
                                                           digi_resource_next_handle_t switch_type,
                                                           digi_direction_check_t direction);

PRIVATE void digi_resource_cpb_chnl_deactivate(digi_handle_t *digi_handle,
                                               digi_resource_handle_summary_t *digi_resource_handle,
                                               UINT32 index,
                                               util_global_switch_data_t *output_pc_ptr);

PRIVATE void digi_resource_cpb_chnl_demap(digi_handle_t *digi_handle,
                                          digi_resource_handle_summary_t *digi_resource_handle,
                                          UINT32 index,
                                          util_global_switch_data_t *output_pc_ptr);

PRIVATE void digi_resource_oduksw_chnl_deactivate(digi_handle_t *digi_handle,                                                  util_global_switch_data_t *output_pc_ptr);

PRIVATE void digi_resource_oduksw_chnl_demap(digi_handle_t *digi_handle,
                                             digi_resource_handle_summary_t *digi_resource_handle,
                                             UINT32 index,
                                             util_global_switch_data_t *output_pc_ptr);

PRIVATE void digi_resource_enet_deprov(digi_handle_t *digi_handle,
                                       digi_enet_client_chnl_t **chnl_ctxt_pptr,
                                       digi_serdes_port_t *port_ctxt_ptr,
                                       UINT32 port_uid);

PRIVATE void digi_resource_cbrc_deprov(digi_handle_t *digi_handle,
                                       digi_enet_client_chnl_t **chnl_ctxt_pptr,
                                       digi_serdes_port_t *port_ctxt_ptr,
                                       UINT32 port_uid);

PRIVATE void digi_resource_otn_mapper_deprov(digi_handle_t *digi_handle,
                                             digi_mapper_chnl_t **chnl_ctxt_pptr);

PRIVATE void digi_resource_otn_server_line_deprov(digi_handle_t *digi_handle,
                                                  digi_otn_server_chnl_t **chnl_lineotn_pptr);

PRIVATE void digi_resource_otn_server_sys_deprov(digi_handle_t *digi_handle,
                                                 digi_otn_server_chnl_t **chnl_lineotn_pptr);

PRIVATE void digi_resource_otn_server_ho_deprov(digi_handle_t *digi_handle,
                                                digi_otn_odu_chnl_t  **odu_chnl_ctxt_pptr,
                                                digi_otn_server_chnl_t **otn_server_pptr);


PRIVATE void digi_resource_otn_server_lo_deprov(digi_handle_t *digi_handle,
                                                digi_resource_handle_summary_t *digi_resource_handle,
                                                UINT32 oduk_index,
                                                digi_otn_odu_chnl_t  **odu_chnl_ctxt_pptr,
                                                digi_otn_odu_chnl_t **ho_odu_chnl_ctxt_pptr);


PRIVATE void digi_resource_restart_release_switch_chnl(digi_handle_t *digi_handle,
                                                       digi_resource_handle_summary_t *digi_resource_handle,
                                                       digi_resource_next_handle_t next_handle,
                                                       UINT16 next_index,
                                                       BOOL8 is_oduk_to_cpb,
                                                       digi_direction_check_t direction);

PRIVATE BOOL8 digi_resource_verify_switch_operational(digi_handle_t *digi_handle,
                                                      digi_resource_handle_summary_t *digi_resource_handle,
                                                      digi_switch_ctxt_t *switch_ptr,
                                                      digi_resource_next_handle_t switch_type,
                                                      BOOL8 is_first,
                                                      digi_resource_next_handle_t prov_type,
                                                      UINT16 prov_index,
                                                      digi_direction_check_t direction);

PRIVATE BOOL8 digi_resource_is_mapotn_loopback(digi_switch_ctxt_t *switch_ptr,
                                               digi_resource_next_handle_t *source_type_ptr,
                                               UINT16 *source_index_ptr);

PRIVATE void digi_resource_release_next_chnl(digi_handle_t *digi_handle,
                                             digi_resource_handle_summary_t *digi_resource_handle,
                                             digi_resource_next_handle_t handle_type,
                                             UINT16 handle_index,
                                             digi_direction_check_t direction);

PRIVATE digi_resource_next_handle_t digi_resource_get_oduk_resource_type(digi_handle_t *digi_handle,
                                                                         digi_resource_handle_summary_t *digi_resource_handle, 
                                                                        util_global_oduk_port_t dest_port_type, 
                                                                        UINT32 dest_channel,
                                                                        digi_resource_next_handle_t provisioned_handle);

PRIVATE BOOL8 digi_resource_check_otn_prov_get(digi_handle_t *digi_handle,
                                               digi_resource_handle_summary_t *digi_resource_handle,
                                               digi_resource_next_handle_t otn_handle,
                                               digi_otn_container_cfg_t *otn_container_ptr,
                                               digi_direction_check_t direction);

PRIVATE void digi_resource_configure_switch_destination(digi_handle_t *digi_handle,
                                                        digi_resource_handle_summary_t *digi_resource_handle,
                                                        digi_switch_ctxt_t *switch_ptr,
                                                        digi_resource_next_handle_t switch_type,
                                                        util_global_oduk_port_t oduk_port_type,
                                                        util_global_cpb_port_t cpb_port_type,
                                                        UINT32 prov_channel,
                                                        BOOL8 enet_pmon);

PRIVATE void digi_resource_reset_serdes_handle(digi_serdes_port_intf_ctxt_t *serdes_port_ptr);

PRIVATE BOOL8 digi_resource_check_pmon_validity(digi_resource_handle_summary_t *digi_resource, 
                                                UINT16 element_index,
                                                util_global_cpb_port_t cpb_port_type, 
                                                UINT32 chnl);

PRIVATE void digi_resource_convert_port_type(digi_handle_t *digi_handle,
                                             UINT32 port_type,
                                             UINT32 chnl_id,
                                             BOOL8 is_oduk_switch,
                                             BOOL8 check_mapotn,
                                             digi_chnl_hndl_conn_t *chnl_conn_ptr);

PRIVATE BOOL8 digi_resource_set_destination_pointer(digi_handle_t *digi_handle,
                                                    util_global_cpb_connect_t *oduk_connect_state_ptr,
                                                    BOOL8 is_oduk_switch,
                                                    BOOL8 is_first,
                                                    UINT32 *port_type_ptr,
                                                    UINT32 *chnl_id_ptr,
                                                    digi_chnl_hndl_conn_t *primary_dest_ptr);


PRIVATE PMC_ERROR digi_resource_release_mapotn_resources(digi_handle_t *digi_handle,
                                                         digi_chnl_hndl_t *chnl_hndl_ptr,
                                                         BOOL8 full_deprov,
                                                         BOOL8 oduk_first);

PRIVATE BOOL8 digi_resource_set_directional_result(digi_direction_check_t direction,
                                                   BOOL8 tx_result,
                                                   BOOL8 rx_result);

PRIVATE BOOL8 digi_resource_restart_release_mapotn(digi_handle_t *digi_handle,
                                                   digi_resource_handle_summary_t *digi_resource,
                                                   UINT32 next_index,
                                                   BOOL8 is_oduk_to_cpb,
                                                   digi_direction_check_t direction);

PRIVATE BOOL8 digi_resource_evaluate_mapping(digi_handle_t *digi_handle,
                                             digi_resource_handle_summary_t *digi_resource_handle,
                                             BOOL8 is_oduk_caller,
                                             util_global_switch_data_t *prov_ptr,
                                             digi_direction_check_t direction,
                                             UINT32 switch_index,
                                             BOOL8 is_mapper);

PRIVATE BOOL8 digi_resource_evaluate_activate(digi_handle_t *digi_handle,
                                              digi_resource_handle_summary_t *digi_resource_handle,
                                              BOOL8 is_oduk_caller,
                                              util_global_switch_data_t *prov_ptr,
                                              digi_direction_check_t direction,
                                              UINT32 switch_index,
                                              BOOL8 is_mapper);

PRIVATE BOOL8 digi_resource_is_unconnected(digi_handle_t *digi_handle,
                                           util_global_switch_data_t *chnl_hndl_ptr);

PRIVATE void digi_resource_set_valid_path(digi_handle_t *ddigi_handle,
                                          digi_resource_next_handle_t otn_handle,
                                          digi_direction_check_t direction,
                                          BOOL8 is_special,
                                          BOOL8 result,
                                          digi_otn_container_cfg_t *otn_container_ptr);

PRIVATE BOOL8 digi_resource_is_direction_valid(digi_direction_check_t direction,
                                               BOOL8 check_all,
                                               util_global_chnl_activated_state_t active_state,
                                               UINT32 input_map_count,
                                               digi_switch_ctxt_t *switch_ptr,
                                               BOOL8 *is_tx_behaviour_ptr);

PRIVATE digi_chnl_hndl_t *digi_resource_get_input_pointer(digi_handle_t *digi_handle,
                                                          digi_resource_handle_summary_t *digi_resource_handle,
                                                          digi_resource_next_handle_t input_handle,
                                                          UINT32 input_index);

PRIVATE void digi_resource_check_src_dest(digi_handle_t *digi_handle,
                                          digi_resource_handle_summary_t *digi_resource_handle,
                                          BOOL8 is_oduk,
                                          BOOL8 is_oduk_to_cpb,
                                          UINT32 next_index,
                                          digi_direction_check_t direction,
                                          BOOL8 *check_dest_ptr,
                                          BOOL8 *check_src_ptr);

PRIVATE PMC_ERROR digi_resource_release_oduk_broadcast(digi_handle_t *digi_handle,
                                                       util_global_oduk_port_t oduk_port,
                                                       UINT32 chnl_id,
                                                       BOOL8 switch_release,
                                                       BOOL8 mapotn_release,
                                                       util_global_cpb_connect_t *oduk_connect_state_ptr);

PRIVATE BOOL8 digi_resource_clean_otn_handle(digi_handle_t *digi_handle,
                                             digi_resource_handle_summary_t *digi_resource_handle_ptr,
                                             odu_struct_t *otn_ptr,
                                             util_global_cpb_connect_t *oduk_connect_data_ptr,
                                             UINT32 table_index,
                                             UINT32 port_type);

PRIVATE void digi_resource_mapotn_release(digi_handle_t *digi_handle,
                                          digi_resource_handle_summary_t *digi_resource_handle_ptr,
                                          util_global_cpb_connect_t *cpb_connect_data_ptr,
                                          util_global_switch_data_def_t *chnl_ptr,
                                          UINT32 map_port,
                                          UINT32 map_chnl);

PRIVATE void digi_resource_oduk_free_handle(digi_handle_t *digi_handle,
                                            digi_resource_handle_summary_t *digi_resource_handle_ptr,
                                            util_global_cpb_connect_t *oduk_connect_data_ptr,
                                            UINT32 dest_port,
                                            UINT32 dest_chnl);

PRIVATE void digi_resource_cpb_free_handle(digi_handle_t *digi_handle,
                                            digi_resource_handle_summary_t *digi_resource_handle_ptr,
                                            util_global_cpb_connect_t *cpb_connect_data_ptr,
                                            UINT32 dest_port,
                                            UINT32 dest_chnl);

PRIVATE util_global_switch_data_def_t *digi_resource_find_otn_handle(digi_handle_t *digi_handle,
                                                                     UINT32 otn_chnl);

PRIVATE void digi_resource_deprovision_otn(digi_handle_t *digi_handle,
                                           digi_resource_handle_summary_t *digi_resource_handle_ptr,
                                           util_global_cpb_connect_t *oduk_connect_data_ptr,
                                           odu_struct_t *otn_ptr,
                                           util_global_odu_level_t odu_level,
                                           UINT32 port_type);

PRIVATE BOOL8 digi_resource_oduk_still_connected(digi_handle_t *digi_handle,
                                                 util_global_cpb_connect_t *cpb_connect_data_ptr,
                                                 util_global_switch_data_def_t *chnl_ptr,
                                                 UINT32 map_port,
                                                 UINT32 map_chnl,
                                                 BOOL8 *is_cpb_clean_ptr);

PRIVATE void digi_resource_clean_otn_subcontainers(digi_handle_t *digi_handle,
                                                   odu_struct_t *otn_ptr);


PRIVATE PMC_ERROR digi_resource_release_otn(digi_handle_t *digi_handle,
                                            digi_resource_handle_summary_t *digi_resource_handle,
                                            digi_otn_container_cfg_t *otn_container_ptr,
                                            digi_direction_check_t direction,
                                            BOOL8 is_oduk_to_cpb,
                                            digi_resource_next_handle_t otn_handle,
                                            void *parent_handle_ptr);

PRIVATE BOOL8 digi_resource_find_parent_handle(digi_handle_t *digi_handle,
                                               digi_resource_handle_summary_t *digi_resource_handle_ptr,
                                               util_global_odu_level_t odu_level,
                                               UINT32 chnl,
                                               digi_otn_odu_chnl_t **otn_parent_pptr);

PRIVATE BOOL8 digi_resource_check_cpb_broadcast(digi_handle_t *digi_handle,
                                                digi_resource_handle_summary_t *digi_resource_handle,
                                                digi_serdes_port_intf_ctxt_t *serdes_port_ptr);

#ifdef PMC_DEPRECATED
PRIVATE void digi_resource_reset_container(digi_resource_handle_summary_t *digi_resource_handle,
                                           digi_resource_next_handle_t handle,
                                           digi_otn_container_cfg_t *otn_container_ptr,
                                           UINT16 index);
#endif /* PMC_DEPRECATED */

#endif /* DOXYGEN_PUBLIC_ONLY */


/*
** Public Functions
*/
/*******************************************************************************
* digi_get_index_and_set_cpb_data
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance
*   element_index       - Index into the sub system array identified by
*                         element type.
*   element_type        - Resource type associated the index that is the
*                         subsystem connected to the CPB switch. In the
*                         case of LINE, SYS or SFI51 serdes the underlying
*                         connection data is accessed to determine the
*                         actual provisioned sub system.
*
* OUTPUTS:
*   digi_resource_handle - Resource handle being updated.
*
* RETURNS:
*   CPB index in the range of 0-425.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT16 digi_get_index_and_set_cpb_data(digi_handle_t *digi_handle,
                                               UINT8 element_index,
                                               digi_resource_next_handle_t element_type,
                                               digi_resource_handle_summary_t *digi_resource_handle)
{
    UINT16 switch_index = 0;
    util_global_switch_data_def_t *switch_data_ptr = NULL;
    util_global_cpb_port_t cpb_port_type = LAST_UTIL_GLOBAL_CPB_PORT;
    UINT32 cpb_chnl;
    util_global_cpb_port_t tmp_cpb_port_type = LAST_UTIL_GLOBAL_CPB_PORT;
    UINT32 tmp_cpb_chnl;    
    BOOL8 enet_pmon = FALSE;

    PMC_ENTRY();

    if ( DIGI_ILKN_1 == element_type || DIGI_ILKN_2 == element_type)
    {
        digi_ilkn_chnl_def_t *ilkn_def_ptr = NULL;
        UINT8 ilkn_index = 0;

        if (DIGI_ILKN_2 == element_type)
        {
            ilkn_index = 1;
        }
        ilkn_def_ptr = (digi_ilkn_chnl_def_t *)digi_resource_handle->ilkn_chnl_cfg[ilkn_index][element_index].ilkn_chnl_handle_ptr;
        switch_data_ptr = &(ilkn_def_ptr->switch_data);
    }
    else if (DIGI_OTN_MAPPER == element_type)
    {
        digi_mapper_chnl_def_t *map_def_ptr = (digi_mapper_chnl_def_t *)digi_resource_handle->mapper_cfg[element_index].mapper_handle_ptr;

        if (NULL != map_def_ptr)
        {
            switch_data_ptr = &(map_def_ptr->switch_data);
        }
    }
    else if (DIGI_LINE_SERDES_CFG == element_type)
    {
        if (digi_resource_handle->line_serdes_cfg[element_index].chnl_handle_type == DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T)
        {
            digi_cbr_client_chnl_def_t *cbr_chnl_ptr = (digi_cbr_client_chnl_def_t *)digi_resource_handle->line_serdes_cfg[element_index].chnl_handle_ptr;
            switch_data_ptr = &(cbr_chnl_ptr->switch_data);
        }
        else if (digi_resource_handle->line_serdes_cfg[element_index].chnl_handle_type == DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T)
        {
            digi_enet_client_chnl_def_t *enet_chnl_ptr = (digi_enet_client_chnl_def_t *)digi_resource_handle->line_serdes_cfg[element_index].chnl_handle_ptr;
            switch_data_ptr = &(enet_chnl_ptr->switch_data);
        }
    }
    else if (DIGI_SFI51_SERDES_CFG == element_type)
    {
        if (digi_resource_handle->sfi51_serdes_cfg[element_index].chnl_handle_type == DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T)
        {
            digi_cbr_client_chnl_def_t *cbr_chnl_ptr = (digi_cbr_client_chnl_def_t *)digi_resource_handle->sfi51_serdes_cfg[element_index].chnl_handle_ptr;
            switch_data_ptr = &(cbr_chnl_ptr->switch_data);
        }
        else if (digi_resource_handle->sfi51_serdes_cfg[element_index].chnl_handle_type == DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T)
        {
            digi_enet_client_chnl_def_t *enet_chnl_ptr = (digi_enet_client_chnl_def_t *)digi_resource_handle->sfi51_serdes_cfg[element_index].chnl_handle_ptr;
            switch_data_ptr = &(enet_chnl_ptr->switch_data);
        }
    }
    else if (DIGI_SYS_SERDES_CFG == element_type)
    {
        if (digi_resource_handle->sys_serdes_cfg[element_index].chnl_handle_type == DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T)
        {
            digi_cbr_client_chnl_def_t *cbr_chnl_ptr = (digi_cbr_client_chnl_def_t *)digi_resource_handle->sys_serdes_cfg[element_index].chnl_handle_ptr;
            switch_data_ptr = &(cbr_chnl_ptr->switch_data);
        }
        else if (digi_resource_handle->sys_serdes_cfg[element_index].chnl_handle_type == DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T)
        {
            digi_enet_client_chnl_def_t *enet_chnl_ptr = (digi_enet_client_chnl_def_t *)digi_resource_handle->sys_serdes_cfg[element_index].chnl_handle_ptr;
            switch_data_ptr = &(enet_chnl_ptr->switch_data);
        }
    }
    /*
     * Explicitly check the switch_data_ptr for NULL and use that to 
     * trigger whether or not to continue.
     */
    if (NULL == switch_data_ptr)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0 , 0, 0,
                "ElementType=%d is NULL FOR CPB SWITCH Index=%d\n",
                element_type, element_index);
        digi_resource_handle->cpbsw_cfg[switch_index].provisioned_handle_type = LAST_DIGI_RESOURCE_HANDLE;
        digi_resource_handle->cpbsw_cfg[switch_index].provisioned_handle_index = INDEX_UNDEFINED;
        PMC_RETURN(switch_index);
    }
    cpb_port_type = switch_data_ptr->cpb_data.port_type;
    cpb_chnl = switch_data_ptr->cpb_data.channel;

    if (cpb_port_type >= UTIL_GLOBAL_CPB_PORT_NOT_USED)
    {
        PMC_RETURN(switch_index);
    }

    tmp_cpb_port_type = cpb_port_type;
    tmp_cpb_chnl = cpb_chnl;

    if (DIGI_OTN_MAPPER == element_type && 
        UTIL_GLOBAL_CPB_PORT_ENET_LINE == cpb_port_type)
    {
        tmp_cpb_port_type = UTIL_GLOBAL_CPB_PORT_MAPOTN;
        /*
         * The channel in CPB and ODUK side are the same for MAPOTN. if
         * ENET_LINE however the channel is relative to ENET_LINE and not
         * MAPOTN. Ensure the correct MAPOTN channel is used.
         */
        tmp_cpb_chnl = switch_data_ptr->oduk_data.channel;
    }
    /*
     * Use tmp_cpb_port_type and tmp_cpb_chnl to ensure the index for the 
     * switch is correct with respect to the display. 
     */
    if (FALSE == digi_resource_get_cpb_switch_index(tmp_cpb_port_type, tmp_cpb_chnl, &switch_index))
    {
        PMC_LOG_TRACE("CPB DATA PortType=%d  is not ILKN1/2, COREOTN or"
                      " MAPOTN\n", switch_data_ptr->cpb_data.port_type);
        PMC_RETURN(0);    
    }
    digi_resource_handle->cpbsw_cfg[switch_index].provisioned_handle_type = element_type;
    digi_resource_handle->cpbsw_cfg[switch_index].provisioned_handle_index = element_index;

   /*
     * The provisioned resource has its destination activated if its
     * slave state is ooperational.  The num_dest is a function of the
     * destination and is incremented only if the master side is 
     * OPERATIONAL indicating that it is actually receiving a signal.
     */
    digi_resource_handle->cpbsw_cfg[switch_index].dest_activated = FALSE;

    if (UTIL_GLOBAL_CHNL_OPERATIONAL == g_cpb_connect_data_ptr->chnl[cpb_port_type][cpb_chnl].mst.state && UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == switch_data_ptr->cpb_data.op_state.active_state)
    {
        digi_resource_handle->cpbsw_cfg[switch_index].dest_activated = TRUE;
    }
    else
    {
        UINT32 dest_port = g_cpb_connect_data_ptr->chnl[cpb_port_type][cpb_chnl].slv.dest_mst_port;
        UINT32 dest_chnl = g_cpb_connect_data_ptr->chnl[cpb_port_type][cpb_chnl].slv.dest_mst_chnl;

        if ((CPB_INDEX_LAST != dest_port && CPB_INDEX_LAST != dest_chnl) &&
            UTIL_GLOBAL_CHNL_OPERATIONAL == g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.state &&
            (UINT32)cpb_port_type == g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.source_slv_port &&
            cpb_chnl == g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.source_slv_chnl)
        {
            digi_resource_handle->cpbsw_cfg[switch_index].source_activated = TRUE;
        }
    }
    /*
     * Set the number of inputs to this channel.
     */
    digi_resource_handle->cpbsw_cfg[switch_index].num_dest = 0;


    if (UTIL_GLOBAL_CHNL_START != g_cpb_connect_data_ptr->chnl[cpb_port_type][cpb_chnl].slv.state &&
        CPB_INDEX_LAST != g_cpb_connect_data_ptr->chnl[cpb_port_type][cpb_chnl].slv.dest_mst_port)
    {
        UINT32 dest_port = g_cpb_connect_data_ptr->chnl[cpb_port_type][cpb_chnl].slv.dest_mst_port;
        UINT32 dest_chnl = g_cpb_connect_data_ptr->chnl[cpb_port_type][cpb_chnl].slv.dest_mst_chnl;

        if (CPB_INDEX_LAST != dest_port && CPB_INDEX_LAST != dest_chnl &&
            UTIL_GLOBAL_CHNL_OPERATIONAL == g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.state)
        {
            /*
             * The master side is operational and increment the num_dest.
             * Now check whether this switch index is broadcasting/
             * multicasting
             */
            digi_resource_handle->cpbsw_cfg[switch_index].num_dest++;

            if (g_cpb_connect_data_ptr->chnl[cpb_port_type][cpb_chnl].slv.is_broadcast ||
                g_cpb_connect_data_ptr->chnl[cpb_port_type][cpb_chnl].slv.is_multicast)
              {
                /*
                 * If this is a broadcast case, the port type is mapotn and
                 * the destination port is ENET_SYS then it may be that this
                 * is a PMON configuration meaning this is not a true
                 * broadcast case.
                 */
                if (TRUE == g_cpb_connect_data_ptr->chnl[cpb_port_type][cpb_chnl].slv.is_broadcast)
                {
                  enet_pmon = digi_resource_check_pmon_validity(digi_resource_handle, element_index, switch_data_ptr->cpb_data.port_type, switch_data_ptr->cpb_data.channel);
                }
                /*
                 * Check that the master side is operational and that the
                 * next dst port is not FFFFF. If it so increment the 
                 * num_dest counter and then check the next element.
                 */
                UINT32 orig_port = dest_port;
                UINT32 orig_chnl = dest_chnl;

                while (FALSE == enet_pmon &&
                       UTIL_GLOBAL_CHNL_OPERATIONAL == g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.state &&
                       CPB_INDEX_LAST != g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.next_dest_mst_port)
                {
                    /*
                     * Check that this is not a loop. Either loopback to
                     * itself or a loop between different elements
                     */
                    if (orig_port == g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.next_dest_mst_port &&
                        orig_chnl == g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.next_dest_mst_chnl)
                    {
                        break;
                    }
                    digi_resource_handle->cpbsw_cfg[switch_index].num_dest++;
                    UINT32 tmp_port = g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.next_dest_mst_port;
                    dest_chnl = g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.next_dest_mst_chnl;
                    dest_port = tmp_port;
                }
            }
        }
    }

    if (TRUE != util_global_rx_or_tx_chnl_prov_status_get(switch_data_ptr->header.prov_state) ||
      (FALSE == switch_data_ptr->cpb_data.op_state.map_state.is_input_mapped && 
       FALSE == switch_data_ptr->cpb_data.op_state.map_state.is_output_mapped))
    {
        PMC_LOG_TRACE("CPB Switch INPUT and OUTPUT is FALSE or prov_state is not PROVISIONED!!!!\n");
        digi_resource_handle->cpbsw_cfg[switch_index].source_handle_type = LAST_DIGI_RESOURCE_HANDLE;
        digi_resource_handle->cpbsw_cfg[switch_index].source_handle_index = 0;
        digi_resource_handle->cpbsw_cfg[switch_index].source_activated = FALSE;
        digi_resource_handle->cpbsw_cfg[switch_index].primary_dest_handle_type = LAST_DIGI_RESOURCE_HANDLE;
        digi_resource_handle->cpbsw_cfg[switch_index].primary_dest_index = 0;
    }
    else
    {
        PMC_LOG_TRACE("Else calling set_source_cpb_switch_side\n");
        digi_resource_set_source_cpb_switch(digi_handle, switch_data_ptr,
                                            switch_index, 
                                            enet_pmon,
                                            digi_resource_handle);
    }
    PMC_RETURN(switch_index);
} /* digi_get_index_and_set_cpb_data */

/*******************************************************************************
* digi_get_index_and_set_oduk_data
* ______________________________________________________________________________
*
* DESCRIPTION:

*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance
*   element_index       - Indice into line_otn_mo_container_cfg
*                         array for the MO channel set by the caller.
*   element_type        - Resource handle in the digi_resource_handle_summary
*                         structure that represnts the sub system being
*                         examined
*
* OUTPUTS:
*   digi_resource_handle - Resource handle being updated.
*
* RETURNS:
*   Returns the oduk switch array index in the range of 0 - 395
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT16 digi_get_index_and_set_oduk_data(digi_handle_t *digi_handle,
                                                UINT8 element_index,
                                                digi_resource_next_handle_t element_type,
                                                digi_resource_handle_summary_t *digi_resource_handle)
{
    UINT16 switch_index = 0;
    util_global_switch_data_def_t *switch_data_ptr = NULL;

    PMC_ENTRY();

    switch (element_type)
    {
      case DIGI_LINE_OTN_HO_CONTAINER:
        PMC_ASSERT(element_index < DIGI_OTN_SERVER_HO_CHNL_MAX,
                   DIGI_ERR_INVALID_ARG, 0, 0);
        switch_data_ptr = &(((odu_struct_t *)digi_resource_handle->line_otn_ho_container_cfg[element_index].chnl_handle_ptr)->switch_data);
        break;

      case DIGI_LINE_OTN_MO_CONTAINER:
        switch_data_ptr = &(((odu_struct_t *)digi_resource_handle->line_otn_mo_container_cfg[element_index].chnl_handle_ptr)->switch_data);
        break;

      case DIGI_LINE_OTN_LO_CONTAINER:
        switch_data_ptr = &(((odu_struct_t *)digi_resource_handle->line_otn_lo_container_cfg[element_index].chnl_handle_ptr)->switch_data);
        break;
        
      case DIGI_SYS_OTN_HO_CONTAINER:
        switch_data_ptr = &(((odu_struct_t *)digi_resource_handle->sys_otn_ho_container_cfg[element_index].chnl_handle_ptr)->switch_data);
        break;
        
      case DIGI_SYS_OTN_MO_CONTAINER:
        switch_data_ptr = &(((odu_struct_t *)digi_resource_handle->sys_otn_mo_container_cfg[element_index].chnl_handle_ptr)->switch_data);
        break;
        
      case DIGI_OTN_MAPPER:
        {
          digi_mapper_chnl_def_t *map_def_ptr = (digi_mapper_chnl_def_t *)digi_resource_handle->mapper_cfg[element_index].mapper_handle_ptr;

          if (NULL != map_def_ptr)
          {
              switch_data_ptr = &(map_def_ptr->switch_data);
          }
        }
        break;

      case DIGI_ILKN_1:
      case DIGI_ILKN_2:
        {
          UINT8 ilkn_index = 0;
          digi_ilkn_chnl_def_t *ilkn_def_ptr = NULL;

          if (DIGI_ILKN_2 == element_type)
          {
              ilkn_index = 1;
          }
          ilkn_def_ptr = (digi_ilkn_chnl_def_t *)digi_resource_handle->ilkn_chnl_cfg[ilkn_index][element_index].ilkn_chnl_handle_ptr;
          switch_data_ptr = &(ilkn_def_ptr->switch_data);      
        }
        break;
        
      default:
        PMC_LOG_TRACE("ElementType=%d is not handled FOR ODUK SWITCH\n",
                      element_type);
        PMC_RETURN(switch_index);
    }

    if (NULL == switch_data_ptr)
    {
        digi_resource_handle->oduksw_cfg[switch_index].provisioned_handle_type = LAST_DIGI_RESOURCE_HANDLE;
        digi_resource_handle->oduksw_cfg[switch_index].provisioned_handle_index = INDEX_UNDEFINED;
        PMC_LOG(PMC_LOG_SEV_HIGH, 0 , 0, 0,
                "ElementType=%d is NULL FOR ODUK SWITCH Index=%d\n",
                element_type, element_index);
        PMC_RETURN(switch_index);
    }

    if (FALSE == digi_resource_get_oduk_switch_index(switch_data_ptr->oduk_data.port_type, switch_data_ptr->oduk_data.channel, &switch_index) )
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "ODUK DATA PortType=%d  is not ILKN1/2, COREOTN or"
                " MAPOTN\n", switch_data_ptr->oduk_data.port_type);
        PMC_RETURN(0);    
    }
    digi_resource_handle->oduksw_cfg[switch_index].provisioned_handle_type = element_type;
    digi_resource_handle->oduksw_cfg[switch_index].provisioned_handle_index = element_index;

    /*
     * The provisioned resource has its destination activated if its
     * slave state is operational.  The num_dest is a function of the
     * destination and is incremented only if the master side is 
     * OPERATIONAL indicating that it is actually receiving a signal.
     */
    if (UTIL_GLOBAL_CHNL_OPERATIONAL == g_oduk_connect_data_ptr->chnl[switch_data_ptr->oduk_data.port_type][switch_data_ptr->oduk_data.channel].mst.state && UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == switch_data_ptr->oduk_data.op_state.active_state)
    {
        digi_resource_handle->oduksw_cfg[switch_index].dest_activated = TRUE;
    }
    else
    {
        /*
         * If the master state is not operational then it may be the case
         * that the destination is activated. If the destination port is
         * defined and its master state is operational and its source is
         * this port and channel then the destination for this port and 
         * channel are activated.
         */
        UINT32 dest_port = g_oduk_connect_data_ptr->chnl[switch_data_ptr->oduk_data.port_type][switch_data_ptr->oduk_data.channel].slv.dest_mst_port;
        UINT32 dest_chnl = g_oduk_connect_data_ptr->chnl[switch_data_ptr->oduk_data.port_type][switch_data_ptr->oduk_data.channel].slv.dest_mst_chnl;

        if ((CPB_INDEX_LAST != dest_port && CPB_INDEX_LAST != dest_chnl) &&
            UTIL_GLOBAL_CHNL_OPERATIONAL == g_oduk_connect_data_ptr->chnl[dest_port][dest_chnl].mst.state &&
            (UINT32)switch_data_ptr->oduk_data.port_type == g_oduk_connect_data_ptr->chnl[dest_port][dest_chnl].mst.source_slv_port &&
            switch_data_ptr->oduk_data.channel == g_oduk_connect_data_ptr->chnl[dest_port][dest_chnl].mst.source_slv_chnl)
        {
            digi_resource_handle->oduksw_cfg[switch_index].source_activated = TRUE;
        }
    }

    if (UTIL_GLOBAL_CHNL_START != g_oduk_connect_data_ptr->chnl[switch_data_ptr->oduk_data.port_type][switch_data_ptr->oduk_data.channel].slv.state &&
        CPB_INDEX_LAST != g_oduk_connect_data_ptr->chnl[switch_data_ptr->oduk_data.port_type][switch_data_ptr->oduk_data.channel].slv.dest_mst_port)
    {
        UINT32 dest_port = g_oduk_connect_data_ptr->chnl[switch_data_ptr->oduk_data.port_type][switch_data_ptr->oduk_data.channel].slv.dest_mst_port;
        UINT32 dest_chnl = g_oduk_connect_data_ptr->chnl[switch_data_ptr->oduk_data.port_type][switch_data_ptr->oduk_data.channel].slv.dest_mst_chnl;

        if (CPB_INDEX_LAST != dest_port && CPB_INDEX_LAST != dest_chnl &&
            UTIL_GLOBAL_CHNL_OPERATIONAL == g_oduk_connect_data_ptr->chnl[dest_port][dest_chnl].mst.state)
        {
            /*
             * The master side is operational and increment the num_dest.
             * Now check whether this switch index is broadcasting/
             * multicasting
             */
            digi_resource_handle->oduksw_cfg[switch_index].num_dest++;

            if (g_oduk_connect_data_ptr->chnl[switch_data_ptr->oduk_data.port_type][switch_data_ptr->oduk_data.channel].slv.is_broadcast ||
                g_oduk_connect_data_ptr->chnl[switch_data_ptr->oduk_data.port_type][switch_data_ptr->oduk_data.channel].slv.is_multicast)
            {
                UINT32 orig_port = dest_port;
                UINT32 orig_chnl = dest_chnl;
                /*
                 * Check that the master side is operational and that the
                 * next dst port is not FFFFF. If it so increment the 
                 * num_dest counter and then check the next element.
                 */
                while (UTIL_GLOBAL_CHNL_OPERATIONAL == g_oduk_connect_data_ptr->chnl[dest_port][dest_chnl].mst.state &&
                       CPB_INDEX_LAST != g_oduk_connect_data_ptr->chnl[dest_port][dest_chnl].mst.next_dest_mst_port)
                {
                   /*
                     * Check that this is not a loop. Either loopback to
                     * itself or a loop between different elements
                     */
                    if (orig_port == g_oduk_connect_data_ptr->chnl[dest_port][dest_chnl].mst.next_dest_mst_port &&
                        orig_chnl == g_oduk_connect_data_ptr->chnl[dest_port][dest_chnl].mst.next_dest_mst_chnl)
                    {
                        break;
                    }
                    digi_resource_handle->oduksw_cfg[switch_index].num_dest++;
                    UINT32 tmp_port = g_oduk_connect_data_ptr->chnl[dest_port][dest_chnl].mst.next_dest_mst_port;
                    dest_chnl = g_oduk_connect_data_ptr->chnl[dest_port][dest_chnl].mst.next_dest_mst_chnl;
                    dest_port = tmp_port;
                }
            }
        }
    }

    if (TRUE != util_global_rx_or_tx_chnl_prov_status_get(switch_data_ptr->header.prov_state) ||
        (FALSE == switch_data_ptr->oduk_data.op_state.map_state.is_input_mapped && 
         FALSE == switch_data_ptr->oduk_data.op_state.map_state.is_output_mapped))
    {
      PMC_LOG_TRACE("ODUK Switch HEADER NOT PROVISIONED or NOT MAPPED. SwitchIndex=%d\n", switch_index);
      digi_resource_handle->oduksw_cfg[switch_index].source_handle_type = LAST_DIGI_RESOURCE_HANDLE;
        digi_resource_handle->oduksw_cfg[switch_index].source_handle_index = 0;
        digi_resource_handle->oduksw_cfg[switch_index].source_activated = FALSE;
        digi_resource_handle->oduksw_cfg[switch_index].primary_dest_handle_type = LAST_DIGI_RESOURCE_HANDLE;
        digi_resource_handle->oduksw_cfg[switch_index].primary_dest_index = 0;
    }
    else
    {
        digi_resource_set_source_oduk_switch(digi_handle, switch_data_ptr, 
                                             switch_index, digi_resource_handle);
    }

    PMC_RETURN(switch_index);
} /* digi_get_index_and_set_oduk_data */

/*******************************************************************************
* digi_resource_clean_source_and_destination
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Deactivates and demaps the primary_destination port type and channel for
*  the provisioned handle caller.  Utilizes the CPB connection data to 
*  determine primary and secondary connection relationships and then
*  deacticvates and demaps the secondary first.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance*   
*   *digi_resource_handle  - Resource handle table that has already been 
*                            built
*   switch_ptr             - Reference to the digi_switch_ctxt_t entry that
*                            is being cleaned.
*   port_type              - CPB (util_global_cpb_port_t) or ODUK 
*                            (util_global_oduk_port_t port value used to
*                            identfy the switch port type being operated on
*   port_chnl              - CPB or ODUK switch channel value used within
*                            the respective port.
*   switch_type            - Either DIGI_ODUK_SWITCH or DIGI_CPB_SWITCH 
*                            which identifies the switch.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS - Associated serdes interface successfully released.
*   PMC_ERROR   - Error occurred along the way
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_resource_clean_source_and_destination(digi_handle_t *digi_handle,
                                                             digi_resource_handle_summary_t *digi_resource_handle,
                                                             digi_switch_ctxt_t *switch_ptr,
                                                             UINT16 port_type,
                                                             UINT32 port_chnl,
                                                             digi_resource_next_handle_t switch_type)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 channel;
    UINT16 dest_port_type;
    UINT16 next_dest_mst_port;
    UINT16 next_dest_mst_chnl = DIGI_RESOURCE_LAST_INDEX;
    PMC_ENTRY();
    /*
     * Ensure there is a primary destination handle. If not there is 
     * nothing to do.
     */
    if (LAST_DIGI_RESOURCE_HANDLE == switch_ptr->primary_dest_handle_type)
    {
      PMC_LOG_TRACE("ProvisionedHandle=%d has no primary Destination\n",
                    switch_ptr->provisioned_handle_type);
        PMC_RETURN(PMC_SUCCESS);
    }
    channel = switch_ptr->primary_dest_index;

    if (DIGI_ODUK_SWITCH == switch_type)
    {
        /*
         * First run a sanity test.
         */
        if (!(UTIL_GLOBAL_CHNL_OPERATIONAL == g_oduk_connect_data_ptr->chnl[port_type][port_chnl].slv.state ||
              UTIL_GLOBAL_CHNL_EQUIPPED == g_oduk_connect_data_ptr->chnl[port_type][port_chnl].slv.state))
        {
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        }
        /*
         * Determine if there is a secondary destination which if there is
         * needs to be deactivated and demapped first. This occurs for 
         * broad cast cases. Namely if the destination port type is not
         * the LAST_INDEX.
         */
        dest_port_type = (UINT16)digi_resource_get_oduk_port_type(switch_ptr->primary_dest_handle_type, &channel);

        if (!(UTIL_GLOBAL_CHNL_START == g_oduk_connect_data_ptr->chnl[dest_port_type][channel].mst.state ||
              CPB_INDEX_LAST == g_oduk_connect_data_ptr->chnl[dest_port_type][channel].mst.next_dest_mst_port))
          {
              next_dest_mst_port = g_oduk_connect_data_ptr->chnl[dest_port_type][channel].mst.next_dest_mst_port;
              next_dest_mst_chnl = g_oduk_connect_data_ptr->chnl[dest_port_type][channel].mst.next_dest_mst_chnl;
          }
    }
    else
    {
        /*
         * First run a sanity test.
         */
        if (!(UTIL_GLOBAL_CHNL_OPERATIONAL == g_cpb_connect_data_ptr->chnl[port_type][port_chnl].slv.state ||
              UTIL_GLOBAL_CHNL_EQUIPPED == g_cpb_connect_data_ptr->chnl[port_type][port_chnl].slv.state))
        {
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        }
        /*
         * Determine if there is a secondary destination which if there is
         * needs to be deactivated and demapped first. This occurs for 
         * broad cast cases. Namely if the destination port type is not
         * the LAST_INDEX.
         */
        dest_port_type = (UINT16)digi_resource_get_cpb_port_type(digi_resource_handle,
                                                                 switch_ptr->primary_dest_handle_type, 
                                                                 switch_ptr->primary_dest_index);
        PMC_ASSERT(dest_port_type < UTIL_GLOBAL_CPB_PORT_NOT_USED, DIGI_ERR_INVALID_ARG, 0, 0);

        if (!(UTIL_GLOBAL_CHNL_START == g_cpb_connect_data_ptr->chnl[dest_port_type][channel].mst.state ||
              CPB_INDEX_LAST == g_cpb_connect_data_ptr->chnl[dest_port_type][channel].mst.next_dest_mst_port))
        {
              next_dest_mst_port = g_cpb_connect_data_ptr->chnl[dest_port_type][channel].mst.next_dest_mst_port;
              next_dest_mst_chnl = g_cpb_connect_data_ptr->chnl[dest_port_type][channel].mst.next_dest_mst_chnl;
        }
    }

    if (DIGI_RESOURCE_LAST_INDEX != next_dest_mst_chnl)
    {
        result = digi_resource_deactivate_demap(digi_handle, 
                                                digi_resource_handle,
                                                next_dest_mst_port,
                                                next_dest_mst_chnl,
                                                switch_type);
    }
    if (PMC_SUCCESS == result)
    {
        result = digi_resource_handle_demap(digi_handle,
                                            digi_resource_handle,
                                            switch_ptr->primary_dest_handle_type,
                                            switch_ptr->primary_dest_index,
                                            switch_type);
    }

    PMC_RETURN(result);
} /* digi_resource_clean_source_and_destination */


/*******************************************************************************
* digi_resource_clean_cpb
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Iterates over the CPB for each port type and clean anything that is
*  inconsistent between the connect array and the associated data handle.
*
* INPUTS:
*   *digi_handle               - pointer to DIGI handle instance*   
*   *digi_resource_handle_ptr  - Reference to the resource handle table
*
* OUTPUTS:
*  None
*
* RETURNS:
*  None
*
*******************************************************************************/
PUBLIC void digi_resource_clean_cpb(digi_handle_t *digi_handle,
                                    digi_resource_handle_summary_t *digi_resource_handle_ptr)
{
    UINT32 i = 0;
    UINT32 port_index;
    util_global_cpb_connect_t *cpb_connect_data_ptr = NULL;

    PMC_ENTRY();

    cpb_connect_data_state_get(digi_handle->dcpb_handle,
                               &cpb_connect_data_ptr);

    digi_resource_clean_ilkn(digi_handle,
                             digi_resource_handle_ptr,
                             cpb_connect_data_ptr);

    for (port_index = 2; port_index < 6; port_index++)
    {
        util_global_cpb_port_t port_type = LAST_UTIL_GLOBAL_CPB_PORT;
        UINT32 num_elements = 0;

        switch(port_index)
        {
          case 2:
            port_type = UTIL_GLOBAL_CPB_PORT_CBRC;
            num_elements = DIGI_CBRC_CHNL_MAX;
            break;

          case 3:
            port_type = UTIL_GLOBAL_CPB_PORT_MAPOTN;
            num_elements = DIGI_MAPOTN_CHNL_MAX;
            break;

          case 4:
            port_type = UTIL_GLOBAL_CPB_PORT_ENET_LINE;
            num_elements = DIGI_ENET_LINE_CHNL_MAX;
            break;

          case 5:
            port_type = UTIL_GLOBAL_CPB_PORT_ENET_SYS;
            num_elements = DIGI_ENET_SYS_CHNL_MAX;
            break;

          default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        }

        for (i = 0; i < num_elements; i++)
        {
            util_global_switch_data_def_t *chnl_ptr = NULL;
            UINT32 port_value = (UINT32)port_type;
            UINT32 chnl_value = i;

            switch (port_type)
            {
              case UTIL_GLOBAL_CPB_PORT_CBRC:
                chnl_ptr = (util_global_switch_data_def_t *)&(digi_handle->handle_pool.cbr_chnl[i]);
                break;

              case UTIL_GLOBAL_CPB_PORT_MAPOTN:
                chnl_ptr = (util_global_switch_data_def_t *)&(digi_handle->handle_pool.mapper_chnl[i]);
                port_value = (UINT32)chnl_ptr->cpb_data.port_type;
                chnl_value = chnl_ptr->cpb_data.channel;
                break;

              case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
                chnl_ptr = (util_global_switch_data_def_t *)&(digi_handle->handle_pool.enet_line_chnl[i]);
                break;

              case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
                chnl_ptr = (util_global_switch_data_def_t *)&(digi_handle->handle_pool.enet_sys_chnl[i]);
                break;

              default:
                PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
              }
            
            if (TRUE == util_global_rx_or_tx_chnl_prov_status_get(chnl_ptr->header.prov_state))
            {
                if ((UTIL_GLOBAL_CHNL_OPERATIONAL == cpb_connect_data_ptr->chnl[port_value][chnl_value].mst.state && UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE != chnl_ptr->cpb_data.op_state.active_state) ||
                    (UTIL_GLOBAL_CHNL_OPERATIONAL != cpb_connect_data_ptr->chnl[port_value][chnl_value].mst.state && UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == chnl_ptr->cpb_data.op_state.active_state))
                {
                    digi_resource_cpb_chnl_deactivate(digi_handle,
                                                      digi_resource_handle_ptr,
                                                      DIGI_CPB_SWITCH_TOTAL_CHANNELS,
                                                      chnl_ptr);

                    digi_resource_cpb_chnl_demap(digi_handle,
                                                 digi_resource_handle_ptr,
                                                 DIGI_CPB_SWITCH_TOTAL_CHANNELS,
                                                 chnl_ptr);  
                }
                else if ((UTIL_GLOBAL_CHNL_OPERATIONAL != cpb_connect_data_ptr->chnl[port_value][chnl_value].mst.state && TRUE == chnl_ptr->cpb_data.op_state.map_state.is_output_mapped) ||
                         (UTIL_GLOBAL_CHNL_START != cpb_connect_data_ptr->chnl[port_value][chnl_value].mst.state && FALSE == chnl_ptr->cpb_data.op_state.map_state.is_output_mapped))
                {
                    digi_resource_cpb_chnl_demap(digi_handle,
                                                 digi_resource_handle_ptr,
                                                 DIGI_CPB_SWITCH_TOTAL_CHANNELS,
                                                 chnl_ptr);
                                             
                }

                if (chnl_ptr->cpb_data.op_state.map_state.input_map_count > 0 && !(UTIL_GLOBAL_CHNL_OPERATIONAL == cpb_connect_data_ptr->chnl[port_value][chnl_value].slv.state
                    || UTIL_GLOBAL_CHNL_BACKUP == cpb_connect_data_ptr->chnl[port_value][chnl_value].slv.state))
                {
                    chnl_ptr->cpb_data.op_state.map_state.input_map_count--;
                    if (0 == chnl_ptr->cpb_data.op_state.map_state.input_map_count)
                    {
                        chnl_ptr->cpb_data.op_state.map_state.is_input_mapped = FALSE;
                    }
                }
                if (UTIL_GLOBAL_CPB_PORT_MAPOTN == port_type)
                {
                    /*
                     * Check the mapotn resource to determine if it is still
                     * connected to anything and if not deprovision it.
                     */
                    digi_resource_mapotn_release(digi_handle,
                                                 digi_resource_handle_ptr,
                                                 cpb_connect_data_ptr,
                                                 chnl_ptr,
                                                 port_value,
                                                 chnl_value);
                }
            }
        }
    }

    PMC_RETURN();

} /* digi_resource_clean_cpb */


/*******************************************************************************
* digi_resource_clean_oduk
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Iterates over the CPB for each port type and clean anything that is
*  inconsistent between the connect array and the associated data handle.
*
* INPUTS:
*   *digi_handle               - pointer to DIGI handle instance*   
*   *digi_resource_handle_ptr  - Reference to the resource handle table
*
* OUTPUTS:
*  None
*
* RETURNS:
*  None
*
*******************************************************************************/
PUBLIC void digi_resource_clean_oduk(digi_handle_t *digi_handle,
                                    digi_resource_handle_summary_t *digi_resource_handle_ptr)
{
    UINT32 i = 0;
    UINT32 port_index;
    util_global_cpb_connect_t *oduk_connect_data_ptr = NULL;

    PMC_ENTRY();

    coreotn_oduksw_connect_data_state_get(digi_handle->coreotn_handle,
                                          &oduk_connect_data_ptr);
    digi_resource_clean_otn(digi_handle, 
                            digi_resource_handle_ptr,
                            oduk_connect_data_ptr);

    for (port_index = 0; port_index < 3; port_index++)
    {
        util_global_oduk_port_t port_type = LAST_UTIL_GLOBAL_ODUK_PORT;
        UINT32 num_elements = 0;
        UINT32 start = 0;
        UINT32 chnl = 0;
        /*
         * COREOTN is handled separately and is not referenced here. As 
         * well want to remove MAPOTN before ILKN which is why the ordering
         * is different.
         */
        switch(port_index)
        {
          case 0:
            port_type = UTIL_GLOBAL_ODUK_PORT_MAPOTN;
            num_elements = DIGI_MAPOTN_CHNL_MAX;
            break;

          case 1:
            port_type = UTIL_GLOBAL_ODUK_PORT_ILKN1;
            num_elements = DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX;
            start = DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
            break;

          case 2:
            port_type = UTIL_GLOBAL_ODUK_PORT_ILKN2;
            num_elements = DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX;
            start = DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
            break;

          default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        }

        for (i = start; i < num_elements; i++)
        {
            util_global_switch_data_def_t *chnl_ptr = NULL;
            chnl = i;

            switch (port_type)
            {
              case UTIL_GLOBAL_ODUK_PORT_ILKN1:
                chnl_ptr = (util_global_switch_data_def_t *)&(digi_handle->handle_pool.ilkn_1_chnl[i]);
                chnl -= DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
                break;

              case UTIL_GLOBAL_ODUK_PORT_ILKN2:
                chnl_ptr = (util_global_switch_data_def_t *)&(digi_handle->handle_pool.ilkn_2_chnl[i]);
                chnl -= DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
                break;

              case UTIL_GLOBAL_ODUK_PORT_MAPOTN:
                chnl_ptr = (util_global_switch_data_def_t *)&(digi_handle->handle_pool.mapper_chnl[i]);
                break;

              default:
                PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
              }
            
            if (TRUE == util_global_rx_or_tx_chnl_prov_status_get(chnl_ptr->header.prov_state))
            {
                if ((UTIL_GLOBAL_CHNL_OPERATIONAL == oduk_connect_data_ptr->chnl[(UINT32)port_type][chnl].mst.state && UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE != chnl_ptr->oduk_data.op_state.active_state) ||
                    (UTIL_GLOBAL_CHNL_OPERATIONAL != oduk_connect_data_ptr->chnl[(UINT32)port_type][chnl].mst.state && UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == chnl_ptr->oduk_data.op_state.active_state))
                {
                    /*
                     * Check for a loopback.  If there is a loopback and
                     *  the if statement matches then it is likely already
                     * been resolved. The channel pointer is update for
                     * all operations but the connect array is not
                     * dynamically updated.
                     */
                    if (!(TRUE == oduk_connect_data_ptr->chnl[(UINT32)port_type][chnl].slv.is_broadcast &&
                      (UINT32)port_type == oduk_connect_data_ptr->chnl[(UINT32)port_type][chnl].mst.source_slv_port &&
                        chnl == oduk_connect_data_ptr->chnl[(UINT32)port_type][chnl].mst.source_slv_chnl &&
                        CPB_INDEX_LAST != oduk_connect_data_ptr->chnl[(UINT32)port_type][chnl].slv.dest_mst_port &&
                         CPB_INDEX_LAST != oduk_connect_data_ptr->chnl[(UINT32)port_type][chnl].slv.dest_mst_chnl &&
                         FALSE == chnl_ptr->oduk_data.op_state.map_state.is_output_mapped))
                    {
                        digi_resource_oduksw_chnl_deactivate(digi_handle,
                                                             chnl_ptr);

                        digi_resource_oduksw_chnl_demap(digi_handle,
                                                        digi_resource_handle_ptr,
                                                        DIGI_ODUK_SWITCH_TOTAL_CHANNELS,
                                                        chnl_ptr);
                    }
                }
                else if ((UTIL_GLOBAL_CHNL_EQUIPPED == oduk_connect_data_ptr->chnl[(UINT32)port_type][chnl].mst.state && TRUE != chnl_ptr->oduk_data.op_state.map_state.is_output_mapped) ||
                         (UTIL_GLOBAL_CHNL_START == oduk_connect_data_ptr->chnl[(UINT32)port_type][chnl].mst.state && TRUE == chnl_ptr->oduk_data.op_state.map_state.is_output_mapped))
                {
                    digi_resource_oduksw_chnl_demap(digi_handle,
                                                    digi_resource_handle_ptr,
                                                    DIGI_ODUK_SWITCH_TOTAL_CHANNELS,
                                                    chnl_ptr);
                                             
                }

                if (chnl_ptr->oduk_data.op_state.map_state.input_map_count > 0 && UTIL_GLOBAL_CHNL_OPERATIONAL != oduk_connect_data_ptr->chnl[(UINT32)port_type][chnl].slv.state)
                {
                    chnl_ptr->oduk_data.op_state.map_state.input_map_count--;
                    if (0 == chnl_ptr->oduk_data.op_state.map_state.input_map_count)
                    {
                        chnl_ptr->oduk_data.op_state.map_state.is_input_mapped = FALSE;
                    }
                }
            }
        }
    }

    PMC_RETURN();

} /* digi_resource_clean_oduk */

/*******************************************************************************
* digi_resource_clean_mapotn
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Final check of the MAPOTN resources. If the state is provisioned and 
*  none of the CPB/ODUK inputs are mapped or activated then deprovision 
*  this resource.
*
* INPUTS:
*   *digi_handle               - pointer to DIGI handle instance*   
*   *digi_resource_handle_ptr  - Reference to the resource handle table
*
* OUTPUTS:
*  None
*
* RETURNS:
*  None
*
*******************************************************************************/
PUBLIC void digi_resource_clean_mapotn(digi_handle_t *digi_handle,
                                       digi_resource_handle_summary_t *digi_resource_handle_ptr)
{
  UINT32 i;

    PMC_ENTRY();

    for (i = 0; i < DIGI_MAPOTN_CHNL_MAX; i++)
    {
        if ((TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.mapper_chnl[i].switch_data.header.prov_state)) &&
            FALSE == digi_handle->handle_pool.mapper_chnl[i].switch_data.cpb_data.op_state.map_state.is_input_mapped &&
            FALSE == digi_handle->handle_pool.mapper_chnl[i].switch_data.cpb_data.op_state.map_state.is_output_mapped &&
            FALSE == digi_handle->handle_pool.mapper_chnl[i].switch_data.oduk_data.op_state.map_state.is_input_mapped &&
            FALSE == digi_handle->handle_pool.mapper_chnl[i].switch_data.oduk_data.op_state.map_state.is_output_mapped)
          {
            digi_mapper_chnl_def_t *map_chnl = &digi_handle->handle_pool.mapper_chnl[i];
            digi_resource_otn_mapper_deprov(digi_handle, 
                                            (digi_mapper_chnl_t **)&map_chnl);
        }
    }

    PMC_RETURN();
}
/*******************************************************************************
* digi_resource_deactivate_demap
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Function deactivates and demaps the destinations from the secondary
*   into the primary to ensure the operations are done on the port/channels
*   in the correct order. If the current port/channel does not have a next
*   destination port then it is demapped. Otherwise the function is called
*   again with the next port/channel in the chain.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance*   
*   *digi_resource_handle  - Resource handle table that has already been 
*                            built
*   port_type              - Representation of either util_global_cpb_port_t
*                            if CPB switch or util_global_oduk_port_t
*   port_chnl              - Channel used for that port type. Would match
*                            the channel used internally with the exception
*                            of MAPOTN which may use ENET_LINE
*   switch_type            - Either DIGI_CPB_SWITCH or DIGI_ODUK_SWITCH.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS - Operation was successful
*   PMC_ERROR   - Error occurred along the way
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_resource_deactivate_demap(digi_handle_t *digi_handle,
                                                 digi_resource_handle_summary_t *digi_resource_handle,
                                                 UINT16 port_type,
                                                 UINT32 port_chnl,
                                                 digi_resource_next_handle_t switch_type)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    if (DIGI_ODUK_SWITCH == switch_type)
    {
        util_global_oduk_port_t oduk_port_type = (util_global_oduk_port_t)port_type;

        if (UTIL_GLOBAL_CHNL_START == g_oduk_connect_data_ptr->chnl[oduk_port_type][port_chnl].mst.state ||
            CPB_INDEX_LAST == g_oduk_connect_data_ptr->chnl[oduk_port_type][port_chnl].mst.next_dest_mst_port)
        {
            UINT16 switch_index;
            result = digi_resource_find_switch_index(digi_handle, digi_resource_handle, port_type, port_chnl, switch_type, &switch_index);

            if (PMC_SUCCESS == result)
            {
                /*
                 * Capture the source type and index before calling
                 * digi_resource_handle_demap which will set those values
                 * to defaults if successful. Once complete and if success
                 * then need to find the source of this input and decrement
                 * its num_dest counter to ensure that its primary 
                 * destination type can be reset to defaults.
                 */
                digi_resource_next_handle_t src_handle = digi_resource_handle->oduksw_cfg[switch_index].source_handle_type;
                UINT16 src_index = digi_resource_handle->oduksw_cfg[switch_index].source_handle_index;
                result = digi_resource_handle_demap(digi_handle,
                                                    digi_resource_handle,
                                                    digi_resource_handle->oduksw_cfg[switch_index].provisioned_handle_type,
                                                    digi_resource_handle->oduksw_cfg[switch_index].provisioned_handle_index, 
                                                    switch_type);

                if (PMC_SUCCESS == result)
                {
                    /*
                     * The source of this provisioned handle needs to be 
                     * found and the is num_dests should be decremeted.
                     */
                    digi_resource_decrement_destinations(digi_handle,
                                                         digi_resource_handle,
                                                         src_handle,
                                                         src_index,
                                                         switch_type);
                }
            }
        }
        else
        {
            /*
             * The port is not the end of the line. Get the next port type
             * and recursively call the function with the new port_type 
             * and channel that is the secondary destination for this port.
             */
            port_type = g_oduk_connect_data_ptr->chnl[oduk_port_type][port_chnl].mst.next_dest_mst_port;
            port_chnl =  g_oduk_connect_data_ptr->chnl[oduk_port_type][port_chnl].mst.next_dest_mst_chnl;
            result = digi_resource_deactivate_demap(digi_handle,
                                                    digi_resource_handle,
                                                    port_type,
                                                    port_chnl,
                                                    switch_type);
        }
    }
    else
    {
        util_global_cpb_port_t cpb_port_type = (util_global_cpb_port_t)port_type;
        if (UTIL_GLOBAL_CHNL_START == g_cpb_connect_data_ptr->chnl[cpb_port_type][port_chnl].mst.state ||
            CPB_INDEX_LAST == g_cpb_connect_data_ptr->chnl[cpb_port_type][port_chnl].mst.next_dest_mst_port)
        {
            UINT16 switch_index;
            result = digi_resource_find_switch_index(digi_handle, digi_resource_handle, port_type, port_chnl, switch_type, &switch_index);

            if (PMC_SUCCESS == result)
            {
                result = digi_resource_handle_demap(digi_handle,
                                                    digi_resource_handle,
                                                    digi_resource_handle->cpbsw_cfg[switch_index].provisioned_handle_type,
                                                    digi_resource_handle->cpbsw_cfg[switch_index].provisioned_handle_index, 
                                                    switch_type);
            }
        }
        else
        {
            port_type = g_cpb_connect_data_ptr->chnl[cpb_port_type][port_chnl].mst.next_dest_mst_port;
            port_chnl =  g_cpb_connect_data_ptr->chnl[cpb_port_type][port_chnl].mst.next_dest_mst_chnl;
            result = digi_resource_deactivate_demap(digi_handle,
                                                    digi_resource_handle,
                                                    port_type,
                                                    port_chnl,
                                                    switch_type);
        }
    }

    PMC_RETURN(result);
} /* digi_resource_deactivate_demap */


/*******************************************************************************
* digi_resource_decrement_destinations
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance*   
*   *digi_resource_handle  - Resource handle table that has already been 
*                            built
*   handle_type            - Resource handle to be found.
*   index                  - Index of the handle
*   switch_type            - Whether this is a CPB or ODUK switch.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS - Operation was successful
*   PMC_ERROR   - Error occurred along the way
*
*******************************************************************************/
PUBLIC void digi_resource_decrement_destinations(digi_handle_t *digi_handle,
                                                  digi_resource_handle_summary_t *digi_resource_handle,
                                                  digi_resource_next_handle_t handle_type,
                                                  UINT16 index,
                                                  digi_resource_next_handle_t switch_type)
{
    UINT16 switch_index = DIGI_RESOURCE_LAST_INDEX;
    digi_switch_ctxt_t *switch_ptr = NULL;

    PMC_ENTRY();
    
    PMC_LOG_TRACE("HandleType = %d, Index=%d\n", handle_type, index);

    /*
     * The most likely cases is the source is ILKN 1/2 and MAPOTN resource.
     * The caller is likely an OTN line or system subcontainer or ENENT LINE
     * SYS or CBRC. As a result of loopbacks however it is possible that
     * any resource capable of being a source could be handle_type all
     * cases are incldued.
     * The index is the handle_type's index and so should be valid and is 
     * not checked that it falls in a particular range.
     */
    switch(handle_type)
    {
      case DIGI_OTN_MAPPER:
        if (DIGI_ODUK_SWITCH == switch_type)
        {
            switch_index = digi_resource_handle->mapper_cfg[index].oduk_switch_index;
        }
        else
        {
            switch_index = digi_resource_handle->mapper_cfg[index].cpb_switch_index;
        }
        break;

    case DIGI_ILKN_1:
      switch_index = digi_resource_handle->ilkn_chnl_cfg[0][index].next_handle_index;
      break;

    case DIGI_ILKN_2:
      switch_index = digi_resource_handle->ilkn_chnl_cfg[1][index].next_handle_index;
      break;

    case DIGI_LINE_OTN_HO_CONTAINER:
      switch_index = digi_resource_handle->line_otn_ho_container_cfg[index].next_handle_index;
      break;

    case DIGI_LINE_OTN_MO_CONTAINER:
      switch_index = digi_resource_handle->line_otn_mo_container_cfg[index].next_handle_index;
      break;

    case DIGI_LINE_OTN_LO_CONTAINER:
      switch_index = digi_resource_handle->line_otn_lo_container_cfg[index].next_handle_index;
      break;

    case DIGI_SYS_OTN_HO_CONTAINER:
      switch_index = digi_resource_handle->sys_otn_ho_container_cfg[index].next_handle_index;
      break;

    case DIGI_SYS_OTN_MO_CONTAINER:
      switch_index = digi_resource_handle->sys_otn_mo_container_cfg[index].next_handle_index;
      break;

    case DIGI_LINE_SERDES_CFG:
      switch_index = digi_resource_handle->line_serdes_cfg[index].next_handle_index;
      break;

    case DIGI_SYS_SERDES_CFG:
      switch_index = digi_resource_handle->sys_serdes_cfg[index].next_handle_index;
      break;

    case DIGI_SFI51_SERDES_CFG:
      switch_index = digi_resource_handle->sfi51_serdes_cfg[index].next_handle_index;
      break;

    default:
      break;
    }

    if (DIGI_RESOURCE_LAST_INDEX != switch_index)
    {
        if (DIGI_ODUK_SWITCH == switch_type)
        {
        	PMC_ASSERT((switch_index < DIGI_ODUK_SWITCH_TOTAL_CHANNELS), DIGI_ERR_INVALID_ARG,  switch_index, DIGI_ODUK_SWITCH_TOTAL_CHANNELS);
            switch_ptr = &digi_resource_handle->oduksw_cfg[switch_index];
        }
        else if (DIGI_CPB_SWITCH == switch_type)
        {
        	PMC_ASSERT((switch_index < DIGI_CPB_SWITCH_TOTAL_CHANNELS), DIGI_ERR_INVALID_ARG,  switch_index, DIGI_CPB_SWITCH_TOTAL_CHANNELS);
            switch_ptr = &digi_resource_handle->cpbsw_cfg[switch_index];
        }
        else
        {
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        }
    }


    if (NULL != switch_ptr)
    {
        if (handle_type != switch_ptr->provisioned_handle_type && 
            index != switch_ptr->provisioned_handle_index)
        {
            PMC_LOG_TRACE("HandleType=%d does not matchProvHandle=%d. ODUK SwitchIndex=%d\n", handle_type, 
                          switch_ptr->provisioned_handle_type, 
                          switch_index);
        }
        else
        {
            /*
             * If here the num_dest value should be greater than 0 so 
             * decrement without checking
             */
            switch_ptr->num_dest--;

            if (0 == switch_ptr->num_dest)
            {
                switch_ptr->primary_dest_handle_type = LAST_DIGI_RESOURCE_HANDLE;
                switch_ptr->primary_dest_index = 0;
            }
        }
    }

    PMC_RETURN();
} /* digi_resource_decrement_destinations */

/*******************************************************************************
* digi_resource_find_switch_index
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Iterates over the switch matching the port_type and channel to the
*  the provisioned resource values until it either finds a match or fails.
*
* INPUTS:
*   *digi_handle           - Digi handle reference
*   *digi_resource_handle  - Resource handle table that has already been 
*                            built
*   port_type              - Representation of either util_global_cpb_port_t
*                            if CPB switch or util_global_oduk_port_t
*   port_chnl              - Channel used for that port type. Would match
*                            the channel used internally with the exception
*                            of MAPOTN which may use ENET_LINE
*   switch_type            - Either DIGI_ODUK_SWITCH or DIGI_CPB_SWITCH
*
* OUTPUTS:
*   *switch_index          - Index into the switch that matches.
*
* RETURNS:
*   PMC_SUCCESS - Switch index match was found
*   PMC_ERROR   - Otherwise.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_resource_find_switch_index(digi_handle_t *digi_handle,
                                                  digi_resource_handle_summary_t *digi_resource_handle, 
                                                  UINT16 port_type, 
                                                  UINT32 port_chnl, 
                                                  digi_resource_next_handle_t switch_type, 
                                                  UINT16 *switch_index)
{
    digi_resource_next_handle_t resource_type = LAST_DIGI_RESOURCE_HANDLE;
    UINT16 start = 0;
    UINT16 end=0;
    UINT16 i;
    PMC_ERROR result = DIGI_ERR_UNKNOWN;

    PMC_ENTRY();
    
    /*
     * Determine the switch type and from that determine the associated
     * port type and from that determine the start and end point of the
     * underlying for loop. That way the loop only iterates over the switch
     * information applicable to that port type and does not have to iterate
     * over the entire switch,
     */
    if (DIGI_ODUK_SWITCH == switch_type)
    {
        util_global_oduk_port_t oduk_port_type = (util_global_oduk_port_t)port_type;

        resource_type = digi_resource_get_oduk_resource_type(digi_handle,
                                                             digi_resource_handle, 
                                                             oduk_port_type, 
                                                             port_chnl,
                                                             LAST_DIGI_RESOURCE_HANDLE);

        if (LAST_DIGI_RESOURCE_HANDLE == resource_type)
        {
            PMC_RETURN(result);
        }
        else if ((DIGI_ILKN_1 == resource_type ||
                  DIGI_ILKN_2 == resource_type) && 
                 port_chnl < DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX)
        {
            port_chnl += DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
        }

        switch (oduk_port_type)
        {
          case UTIL_GLOBAL_ODUK_PORT_ILKN1:
            start = DIGI_ODU_SWITCH_PORT0_OFFSET;
            end = DIGI_ODU_SWITCH_PORT1_OFFSET;
            break;

          case UTIL_GLOBAL_ODUK_PORT_ILKN2:
            start = DIGI_ODU_SWITCH_PORT1_OFFSET;
            end = DIGI_ODU_SWITCH_PORT2_OFFSET;
            break;
  
         case UTIL_GLOBAL_ODUK_PORT_COREOTN:
            start = DIGI_ODU_SWITCH_PORT2_OFFSET;
            end = DIGI_ODU_SWITCH_PORT3_OFFSET;
            break; 
         case UTIL_GLOBAL_ODUK_PORT_MAPOTN:
            start = DIGI_ODU_SWITCH_PORT3_OFFSET;
            end = DIGI_ODUK_SWITCH_TOTAL_CHANNELS;
            break; 
          default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
            break;
        }
    }
    else
    {
        util_global_cpb_port_t cpb_port_type = (util_global_cpb_port_t)port_type;
        resource_type = digi_resource_get_cpb_resource_type(digi_handle,
                                                            digi_resource_handle, 
                                                            cpb_port_type, 
                                                            &port_chnl);
        if (LAST_DIGI_RESOURCE_HANDLE == resource_type)
        {
            PMC_RETURN(result);
        }

        switch (cpb_port_type)
        {
          case UTIL_GLOBAL_CPB_PORT_ILKN1:
            start = DIGI_CPB_SWITCH_PORT0_OFFSET;
            end = DIGI_CPB_SWITCH_PORT1_OFFSET;
            break;

          case UTIL_GLOBAL_CPB_PORT_ILKN2:
            start = DIGI_CPB_SWITCH_PORT1_OFFSET;
            end = DIGI_CPB_SWITCH_PORT2_OFFSET;
            break;
  
         case UTIL_GLOBAL_CPB_PORT_CBRC:
            start = DIGI_CPB_SWITCH_PORT2_OFFSET;
            end = DIGI_CPB_SWITCH_PORT3_OFFSET;
            break; 

         case UTIL_GLOBAL_CPB_PORT_MAPOTN:
            start = DIGI_CPB_SWITCH_PORT3_OFFSET;
            end = DIGI_CPB_SWITCH_PORT4_OFFSET;
            break; 

         case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
            start = DIGI_CPB_SWITCH_PORT4_OFFSET;
            end = DIGI_CPB_SWITCH_PORT5_OFFSET;
            break; 

         case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
            start = DIGI_CPB_SWITCH_PORT5_OFFSET;
            end = DIGI_CPB_SWITCH_TOTAL_CHANNELS;
            break; 

          default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
            break;
        }
    }

    for (i = start; i < end; i++)
    {
        digi_switch_ctxt_t *switch_ptr = NULL;

        if (DIGI_ODUK_SWITCH == switch_type)
        {
            switch_ptr = &digi_resource_handle->oduksw_cfg[i];
        }
        else
        {
            switch_ptr = &digi_resource_handle->cpbsw_cfg[i];
        }

        if (resource_type == switch_ptr->provisioned_handle_type && 
            port_chnl == switch_ptr->provisioned_handle_index)
        {
            *switch_index = i;
            result = PMC_SUCCESS;
            break;
        }
    }

    PMC_RETURN(result);
} /* digi_resource_find_switch_index */


/*******************************************************************************
* digi_resource_handle_demap
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Called to deactivate and demap the source of the resource being 
*   released.  
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *digi_resource_handle  - Resource handle table that has already been 
*                            built
*   handle_type            - Resource handle that is being deactivated and
*                            demapped.
*   handle_index           - Index in the resource table of the resource
*                            identified by the handle_type.
*   switch_handle          - Whether this is ODUK or CPB switch
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS - Associated serdes interface successfully released.
*   PMC_ERROR   - Error occurred along the way
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_resource_handle_demap(digi_handle_t *digi_handle,
                                             digi_resource_handle_summary_t *digi_resource_handle,
                                             digi_resource_next_handle_t handle_type,
                                             UINT16 handle_index,
                                             digi_resource_next_handle_t switch_handle)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    UINT16 switch_index;
    util_global_switch_data_t *prov_ptr = NULL;
    digi_resource_next_handle_t next_resource = LAST_DIGI_RESOURCE_HANDLE;
    digi_switch_ctxt_t *switch_index_ptr = NULL;

    PMC_ENTRY();
    /*
     * ILKN 1/2 or MAPOTN resources could be connected across either
     * switch. Any OTN related resource is ODUK only while ENET LINE/SYS
     * or CBRC are CPB only.
     */
    if (DIGI_ILKN_1 == handle_type || DIGI_ILKN_2 == handle_type)
    {
        UINT16 ilkn_inst = 0;

        if (DIGI_ILKN_2 == handle_type)
        {
            ilkn_inst = 1;
        }
        switch_index = digi_resource_handle->ilkn_chnl_cfg[ilkn_inst][handle_index].next_handle_index;
        next_resource = digi_resource_handle->ilkn_chnl_cfg[ilkn_inst][handle_index].next_resource_handle;
        prov_ptr = digi_resource_handle->ilkn_chnl_cfg[ilkn_inst][handle_index].ilkn_chnl_handle_ptr;
    }
    else if (DIGI_OTN_MAPPER == handle_type)
    {
        if (DIGI_ODUK_SWITCH == switch_handle)
        {
            switch_index = digi_resource_handle->mapper_cfg[handle_index].oduk_switch_index;
        }
        else
        {
            switch_index = digi_resource_handle->mapper_cfg[handle_index].cpb_switch_index;
        }
        /*
         * Mapper structure does not have a next resource so set the pased
         * in value to ensure that passes.
         */
        next_resource = switch_handle;
        prov_ptr = digi_resource_handle->mapper_cfg[handle_index].mapper_handle_ptr;
    }
    else if (DIGI_ODUK_SWITCH == switch_handle &&
             DIGI_SYS_OTN_HO_CONTAINER == handle_type)
    {
        switch_index = digi_resource_handle->sys_otn_ho_container_cfg[handle_index].next_handle_index;
        next_resource = digi_resource_handle->sys_otn_ho_container_cfg[handle_index].next_resource_handle;
        prov_ptr = digi_resource_handle->sys_otn_ho_container_cfg[handle_index].chnl_handle_ptr; 
    }
    else if (DIGI_ODUK_SWITCH == switch_handle &&
             DIGI_SYS_OTN_MO_CONTAINER == handle_type)
    {
        switch_index = digi_resource_handle->sys_otn_mo_container_cfg[handle_index].next_handle_index;
        next_resource = digi_resource_handle->sys_otn_mo_container_cfg[handle_index].next_resource_handle;
        prov_ptr = digi_resource_handle->sys_otn_mo_container_cfg[handle_index].chnl_handle_ptr; 
    }
    else if (DIGI_ODUK_SWITCH == switch_handle &&
             DIGI_LINE_OTN_HO_CONTAINER == handle_type)
    {
        switch_index = digi_resource_handle->line_otn_ho_container_cfg[handle_index].next_handle_index;
        next_resource = digi_resource_handle->line_otn_ho_container_cfg[handle_index].next_resource_handle;
        prov_ptr = digi_resource_handle->line_otn_ho_container_cfg[handle_index].chnl_handle_ptr;
    }
    else if (DIGI_ODUK_SWITCH == switch_handle &&
             DIGI_LINE_OTN_MO_CONTAINER == handle_type)
    {
        switch_index = digi_resource_handle->line_otn_mo_container_cfg[handle_index].next_handle_index;
        next_resource = digi_resource_handle->line_otn_mo_container_cfg[handle_index].next_resource_handle;
        prov_ptr = digi_resource_handle->line_otn_mo_container_cfg[handle_index].chnl_handle_ptr;
    }
    else if (DIGI_ODUK_SWITCH == switch_handle &&
             DIGI_LINE_OTN_LO_CONTAINER == handle_type)
    {
        switch_index = digi_resource_handle->line_otn_lo_container_cfg[handle_index].next_handle_index;
        next_resource = digi_resource_handle->line_otn_lo_container_cfg[handle_index].next_resource_handle;
        prov_ptr = digi_resource_handle->line_otn_lo_container_cfg[handle_index].chnl_handle_ptr;
    }
    else if (DIGI_CPB_SWITCH == switch_handle &&
             DIGI_LINE_SERDES_CFG == handle_type)
    {
        switch_index = digi_resource_handle->line_serdes_cfg[handle_index].next_handle_index;
        next_resource = digi_resource_handle->line_serdes_cfg[handle_index].next_resource_handle;
        prov_ptr = digi_resource_handle->line_serdes_cfg[handle_index].chnl_handle_ptr;
    }
    else if (DIGI_CPB_SWITCH == switch_handle &&
             DIGI_SFI51_SERDES_CFG == handle_type)
    {
        switch_index = digi_resource_handle->sfi51_serdes_cfg[handle_index].next_handle_index;
        next_resource = digi_resource_handle->sfi51_serdes_cfg[handle_index].next_resource_handle;
        prov_ptr = digi_resource_handle->sfi51_serdes_cfg[handle_index].chnl_handle_ptr;
    }
    else if (DIGI_CPB_SWITCH == switch_handle &&
             DIGI_SYS_SERDES_CFG == handle_type)
    {
        switch_index = digi_resource_handle->sys_serdes_cfg[handle_index].next_handle_index;
        next_resource = digi_resource_handle->sys_serdes_cfg[handle_index].next_resource_handle;
        prov_ptr = digi_resource_handle->sys_serdes_cfg[handle_index].chnl_handle_ptr;
    }
    else
    {
        /*
         * Unexpected switch handle to be processed. Return an error as 
         * this case should not occur.
         */
        PMC_RETURN(result);
    }

    if (DIGI_ODUK_SWITCH == switch_handle)
    {
        switch_index_ptr = &digi_resource_handle->oduksw_cfg[switch_index];
    }
    else
    {
        switch_index_ptr = &digi_resource_handle->cpbsw_cfg[switch_index];
    }
    PMC_LOG_TRACE("SwitchHandle=%d NextResource=%d\n\tProvHandleType=%d\n"
                  "\tHandleType=%d\n",
                  switch_handle, next_resource, 
                  switch_index_ptr->provisioned_handle_type, handle_type);

    if (switch_handle == next_resource && 
        handle_type == switch_index_ptr->provisioned_handle_type)
    {
        if (TRUE == switch_index_ptr->dest_activated)
        {
            if (DIGI_ODUK_SWITCH == switch_handle)
            {     
                result = digi_oduksw_chnl_deactivate(digi_handle, prov_ptr);
            }
            else
            {
                result = digi_cpb_chnl_deactivate(digi_handle, prov_ptr);
            }
        }
        else
        {
            /*
             * result is initialized to ERROR to catch the case that the
             * sanity check fails. However if the destination has not been
             * activatd the channel stills needs to be demapped so set the
             * result to SUCCESS.
             */
            result = PMC_SUCCESS;
        }
        if (PMC_SUCCESS == result)
        {
            switch_index_ptr->dest_activated = FALSE;

            if (DIGI_ODUK_SWITCH == switch_handle)
            { 
                result = digi_oduksw_chnl_demap(digi_handle, prov_ptr);
            }
            else
            {
                result = digi_cpb_chnl_demap(digi_handle, prov_ptr);
            }
        }

        if (PMC_SUCCESS == result)
        {
            /*
             * It is valid in this case for num_dest to be 0 since this
             * resource may not be transmitting. Add a check to ensure that
             * it is greater than 0 before decrementing.
             */
            if (switch_index_ptr->num_dest > 0)
            {
                switch_index_ptr->num_dest--;
                
                if (0 == switch_index_ptr->num_dest)
                {
                    PMC_LOG_TRACE("Num_dest is 0 for ProvisionedHandle=%d and SwitchIndex=%d\n", switch_index_ptr->provisioned_handle_type, switch_index);
                    switch_index_ptr->primary_dest_handle_type = LAST_DIGI_RESOURCE_HANDLE;
                    switch_index_ptr->primary_dest_index = 0;
                }
            }
            switch_index_ptr->source_handle_type =  LAST_DIGI_RESOURCE_HANDLE;
            switch_index_ptr->source_handle_index = 0;
            switch_index_ptr->source_activated = FALSE;
        }
    }

    PMC_RETURN(result);
}  /* digi_resource_handle_demap */


/*******************************************************************************
* digi_resource_restart_release_port
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Function recursively calls the next resource handle if connected and then
*  handles the deprovisioning of the current channel.  Any deactivation and
*  demapping operations are handled at the switch in the function
*  digi_resource_restart_release_switch_chnl.
*  Function is used in a crash restart as part of releasing a datapath. 
*  Unlike a normal release operation the sub-operators used ignore errors
*  and failures and go as far as possible to ensure that the digi layer
*  state is reset for resources within the current data path.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *digi_resource_handle  - Resource handle table that has already been 
*                            built
*   parent_handle_ptr      - Pointer to the channel handle that is connected
*                            to the next_handle
*   next_handle            - Resource handle type value of what the 
*                            serdes_connection_handle is connected to.
*   next_index             - Indice of the next handle in the digi 
*                            resource table.
*   is_oduk_to_cpb         - Used for MAPOTN to determine the perspective
*                            of the caller. Is this being torn down from
*                            the ODUK side to CPB, TRUE, or CPB to ODUK
*                            FALSE,
*   direction              - Direction the data path is being released.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS - Associated serdes interface successfully released.
*   PMC_ERROR   - Error occurred along the way
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_resource_restart_release_port(digi_handle_t *digi_handle,
                                                    digi_resource_handle_summary_t *digi_resource_handle,
                                                    void *parent_handle_ptr,
                                                    digi_resource_next_handle_t next_handle,
                                                    UINT16 next_index,
                                                    BOOL8 is_oduk_to_cpb,
                                                    digi_direction_check_t direction)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT16 ilkn_index;
    digi_ilkn_intf_t ilkn_type;
    digi_otn_container_cfg_t *otn_container_ptr = NULL;
    UINT32 switch_index;

    PMC_ENTRY();

    /*
     * Below is a listing of what the next handle from a serdes port can
     * be connected to. 
     * If next_handle is a CPB or OUDK switch then call the 
     * digi_resource_restart_release_switch_chnl to handle the deactivating
     * and demapping cases.  Although a serdes port may not be directly 
     * connected this function is recursive and will be called by sub
     * containers of the serdes port.
     * All other handle values check for connectivity below and call the
     * function again recursively with its connection. When that completes
     * it can deprovision itself.
     * LINE_OTN_HO has to deprovision the LINEOTN channel afterwards as does
     * SYS_OTN HO channel.
     */
    switch (next_handle)
    {
      case DIGI_ODUK_SWITCH:
      case DIGI_CPB_SWITCH:
         digi_resource_restart_release_switch_chnl(digi_handle,
                                                   digi_resource_handle,
                                                   next_handle,
                                                   next_index,
                                                   is_oduk_to_cpb,
                                                   direction);
        break;

      case DIGI_LINE_OTN_HO_CONTAINER:
        otn_container_ptr = &digi_resource_handle->line_otn_ho_container_cfg[next_index];

        result = digi_resource_release_otn(digi_handle,
                                           digi_resource_handle,
                                           otn_container_ptr,
                                           direction,
                                           is_oduk_to_cpb,
                                           DIGI_LINE_OTN_HO_CONTAINER,
                                           parent_handle_ptr);
        break;

      case DIGI_LINE_OTN_MO_CONTAINER:
        otn_container_ptr = &digi_resource_handle->line_otn_mo_container_cfg[next_index];

        if (DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == digi_handle->var.prod_app)
        {
             otn_container_ptr = &digi_resource_handle->sys_otn_mo_container_cfg[next_index];
        }
        result = digi_resource_release_otn(digi_handle,
                                           digi_resource_handle,
                                           otn_container_ptr,
                                           direction,
                                           is_oduk_to_cpb,
                                           DIGI_LINE_OTN_MO_CONTAINER,
                                           parent_handle_ptr);
        break;

      case DIGI_LINE_OTN_LO_CONTAINER:
        otn_container_ptr = &digi_resource_handle->line_otn_lo_container_cfg[next_index];
       result = digi_resource_release_otn(digi_handle,
                                          digi_resource_handle,
                                          otn_container_ptr,
                                          direction,
                                          is_oduk_to_cpb,
                                          DIGI_LINE_OTN_LO_CONTAINER,
                                          parent_handle_ptr);
        break;

      case DIGI_SYS_OTN_HO_CONTAINER:
        /*
         * SYS HO only has one element. If the index is not 0 then want to
         * stop right here.
         * NOTE: Not sure if this case can even get hit. Need to inestigate
         * and remove if not,
         */
        if (TRUE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle))
        {
            PMC_ASSERT(0 == next_index, DIGI_ERR_INVALID_ARG, 0, 0);
        }
        otn_container_ptr = &digi_resource_handle->sys_otn_ho_container_cfg[next_index];
        result = digi_resource_release_otn(digi_handle,
                                           digi_resource_handle,
                                           otn_container_ptr,
                                           direction,
                                           is_oduk_to_cpb,
                                           DIGI_SYS_OTN_HO_CONTAINER,
                                           parent_handle_ptr);

        break;

      case DIGI_SYS_OTN_MO_CONTAINER:
        otn_container_ptr = &digi_resource_handle->sys_otn_mo_container_cfg[next_index];
        result = digi_resource_release_otn(digi_handle,
                                           digi_resource_handle,
                                           otn_container_ptr,
                                           direction,
                                           is_oduk_to_cpb,
                                           DIGI_SYS_OTN_MO_CONTAINER,
                                           parent_handle_ptr);

        break;

      case DIGI_ILKN_1:
      case DIGI_ILKN_2:
        /*
         * ILKN 1 and 2 are combined and only need an index to determine
         * which one is invoked. Use the 
         * digi_resource_restat_release_switch_chnl
         * function to handle demap/deactivate operations and then 
         * perform the deprovisioning. Need to reconstruct the flow control
         * calendar before deprovisioning.
         */
        ilkn_index = 0;
        ilkn_type = DIGI_ILKN1;

        if (DIGI_ILKN_2 == next_handle)
        {
            ilkn_index = 1;
            ilkn_type = DIGI_ILKN2;
        }
        switch_index = digi_resource_handle->ilkn_chnl_cfg[ilkn_index][next_index].next_handle_index;

        if (FALSE == digi_resource_handle->force_release &&
            FALSE == digi_resource_handle->datapath_release &&
            FALSE == is_oduk_to_cpb && 
            DIGI_DIRECTION_RX_ONLY == direction &&
            DIGI_ODUK_SWITCH == digi_resource_handle->ilkn_chnl_cfg[ilkn_index][next_index].next_resource_handle)
        {
            if (digi_resource_handle->oduksw_cfg[switch_index].num_dest > 1 &&
                TRUE == digi_resource_handle->oduksw_cfg[switch_index].source_activated &&
                TRUE == digi_resource_handle->oduksw_cfg[switch_index].dest_activated &&
                LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->oduksw_cfg[switch_index].source_handle_type &&
                LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->oduksw_cfg[switch_index].primary_dest_handle_type)
            {
                PMC_RETURN(PMC_SUCCESS);
            }
        }
        /*
         * Handle the case that ILKN is bi-directional connected to 
         * separate uni-directional resources. That it is receiving its
         * signal from a channel handle that is different from what it is
         * sending to. In that case there are or could be 2 uni-directional
         * data paths connected to a bi-directional channel handle.
         */
        if (FALSE == digi_resource_handle->force_release &&
            FALSE == digi_resource_handle->datapath_release &&
            TRUE == is_oduk_to_cpb &&
            TRUE == digi_resource_handle->oduksw_cfg[switch_index].source_activated &&
            TRUE == digi_resource_handle->oduksw_cfg[switch_index].dest_activated &&
            LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->oduksw_cfg[switch_index].source_handle_type &&
            LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->oduksw_cfg[switch_index].primary_dest_handle_type &&
            (digi_resource_handle->oduksw_cfg[switch_index].source_handle_type !=
             digi_resource_handle->oduksw_cfg[switch_index].primary_dest_handle_type ||
             digi_resource_handle->oduksw_cfg[switch_index].source_handle_index !=
             digi_resource_handle->oduksw_cfg[switch_index].primary_dest_index))
          {
            PMC_RETURN(PMC_SUCCESS);
        }

        if (LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->ilkn_chnl_cfg[ilkn_index][next_index].next_resource_handle)
        {
            digi_resource_restart_release_switch_chnl(digi_handle, 
                                                      digi_resource_handle,
                                                      digi_resource_handle->ilkn_chnl_cfg[ilkn_index][next_index].next_resource_handle,
                                                      switch_index,
                                                      is_oduk_to_cpb,
                                                      direction);
        }
        break;

      case DIGI_OTN_MAPPER:
        result = digi_resource_restart_release_mapotn(digi_handle,
                                                      digi_resource_handle,
                                                      next_index,
                                                      is_oduk_to_cpb,
                                                      direction);
        break;

      default:
        break;
    }

    PMC_RETURN(result);
}


/*******************************************************************************
* digi_resource_deprovision_channel
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Handles the deprovisioning of enet_line, enet_sys or cbr client channels
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   chnl_handle            - digi_handle_id_t identifying which of the 3
*                            channel types is being deprovisioned.
*   chnl_handle_ptr        - Reference to the channel handle stored in the
*                            particular serdes structure.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS - Channel handle was successfully deprovisioned
*   PMC_ERROR   - Otherwise
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_resource_deprovision_channel(digi_handle_t *digi_handle,
                                                    digi_handle_id_t chnl_handle,
                                                    void *chnl_handle_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    if (DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T == chnl_handle)
    {
        result = digi_enet_line_deprov(digi_handle, 
                                       (digi_enet_client_chnl_t **)&chnl_handle_ptr);
    }
    else if (DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T == chnl_handle)
    {
        result = digi_enet_sys_deprov(digi_handle, (digi_enet_client_chnl_t **)&chnl_handle_ptr);
    }
    else if (DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T == chnl_handle)
    {
        result = digi_cbr_client_deprov(digi_handle, (digi_cbr_client_chnl_t **)&chnl_handle_ptr);
    }
    else if (DIGI_CHANNEL_HANDLE_ID_MAPPER_T == chnl_handle)
    {
        result = digi_otn_mapper_deprov(digi_handle,
                                        (digi_mapper_chnl_t **)&chnl_handle_ptr);
    }
   
    PMC_RETURN(result);
}  /* digi_resource_deprovision_channel */


/*******************************************************************************
* digi_resource_get_oduk_port_type
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Given a resource handle returns the associated ODUK port type.
*
* INPUTS:
*  resource_type  - Resource type to be converted.

* OUTPUTS:
*  *channel       - If the resource type is ILKN1 then may need to reset the
*                   channel to make it 0 relative rather than 150 relative
*
* RETURNS:
*  UTIL_GLOBAL_ODUK_PORT_ILKN1 - ILKN 1 Port
*  UTIL_GLOBAL_ODUK_PORT_ILKN2 - ILKN 2 Port
*  UTIL_GLOBAL_ODUK_PORT_COREOTN - COREOTN Port type.
*  UTIL_GLOBAL_ODUK_PORT_MAPOTN - MAPOTN Port Type
*  LAST_UTIL_GLOBAL_ODUK_PORT - Otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC util_global_oduk_port_t digi_resource_get_oduk_port_type(digi_resource_next_handle_t resource_type,
                                                                 UINT32 *channel)
{
    util_global_oduk_port_t port_type = LAST_UTIL_GLOBAL_ODUK_PORT;

    PMC_ENTRY();

    switch(resource_type)
    {
    case DIGI_ILKN_1:
      port_type = UTIL_GLOBAL_ODUK_PORT_ILKN1;

      if (*channel >= DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX)
      {
          *channel -= DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
      }

      break;

    case DIGI_ILKN_2:
      port_type = UTIL_GLOBAL_ODUK_PORT_ILKN2;

      if (*channel >= DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX)
      {
          *channel -= DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
      }
      break;

    case DIGI_OTN_MAPPER:
      port_type = UTIL_GLOBAL_ODUK_PORT_MAPOTN;
      break;

    case DIGI_LINE_OTN_HO_CONTAINER:
    case DIGI_LINE_OTN_MO_CONTAINER:
    case DIGI_LINE_OTN_LO_CONTAINER:
    case DIGI_SYS_OTN_HO_CONTAINER:
    case DIGI_SYS_OTN_MO_CONTAINER:
      port_type = UTIL_GLOBAL_ODUK_PORT_COREOTN;
      break;
      
    default:
      break;
    }

    PMC_RETURN(port_type);
} /*digi_resource_get_oduk_port_type*/


/*******************************************************************************
* digi_resource_get_cpb_port_type
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Given a resource handle returns the associated CPB port type.
*
* INPUTS:
*  digi_resource_handle - Reference to the digi_resource_handle structure.
*  resource_type  - Resource type to be converted.
*  index          - Index of the resource type in the resource table. 
*                   Required if the resource type is LINE/SYS/SFI SERDES
*                   CFG so that the actual port type can be identified.
* OUTPUTS:
*  None
*
* RETURNS:
*  UTIL_GLOBAL_CPB_PORT_ILKN1 - ILKN 1 Port
*  UTIL_GLOBAL_CPB_PORT_ILKN2 - ILKN 2 Port
*  UTIL_GLOBAL_CPB_PORT_CBRC  - CBRC Port type.
*  UTIL_GLOBAL_CPB_PORT_MAPOTN - MAPOTN Port Type
*  UTIL_GLOBAL_CPB_PORT_ENET_LINE - ENET Line Port Type
*  UTIL_GLOBAL_CPB_PORT_ENET_SYS - ENET SYS Port Type
*  LAST_UTIL_GLOBAL_CPB_PORT - Otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC util_global_cpb_port_t digi_resource_get_cpb_port_type(digi_resource_handle_summary_t *digi_resource_handle,
                                                              digi_resource_next_handle_t resource_type,
                                                              UINT16 index)
{
    util_global_cpb_port_t port_type = LAST_UTIL_GLOBAL_CPB_PORT;

    PMC_ENTRY();

    switch(resource_type)
    {
    case DIGI_ILKN_1:
      port_type = UTIL_GLOBAL_CPB_PORT_ILKN1;
      break;

    case DIGI_ILKN_2:
      port_type = UTIL_GLOBAL_CPB_PORT_ILKN2;
      break;

    case DIGI_OTN_MAPPER:
      port_type = UTIL_GLOBAL_CPB_PORT_MAPOTN;
      break;

    case DIGI_LINE_SERDES_CFG:
      if (DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T == digi_resource_handle->line_serdes_cfg[index].chnl_handle_type)
      {
          port_type = UTIL_GLOBAL_CPB_PORT_ENET_LINE;
      }
      else if (DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T == digi_resource_handle->line_serdes_cfg[index].chnl_handle_type)
      {
          port_type = UTIL_GLOBAL_CPB_PORT_CBRC;
      }

      break;
    case DIGI_SFI51_SERDES_CFG:
      if (DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T == digi_resource_handle->sfi51_serdes_cfg[index].chnl_handle_type)
      {
          port_type = UTIL_GLOBAL_CPB_PORT_ENET_LINE;
      }
      else if (DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T == digi_resource_handle->sfi51_serdes_cfg[index].chnl_handle_type)
      {
          port_type = UTIL_GLOBAL_CPB_PORT_CBRC;
      }      

      break;
    case DIGI_SYS_SERDES_CFG:
      if (DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T == digi_resource_handle->sys_serdes_cfg[index].chnl_handle_type)
      {
          port_type = UTIL_GLOBAL_CPB_PORT_ENET_SYS;
      }
      else if (DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T == digi_resource_handle->sys_serdes_cfg[index].chnl_handle_type)
      {
          port_type = UTIL_GLOBAL_CPB_PORT_CBRC;
      }  
      break;

    default:
      /*
       * Need to add ENET_LINE, SYS and CBRC. This is extracted via the
       * serdes configuration and will need extra data for this.
       */
      break;
    }

    PMC_RETURN(port_type);
}  /* digi_resource_get_cpb_port_type */


/*******************************************************************************
* digi_resource_get_cpb_resource_type
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Converts the util_global_cpb_port_t into the appropriate 
*  digi_resource_next_handle_t handle. 
*
* INPUTS:
*   digi_handle          - Reference to the digi_handle
*   digi_resource_handle - Reference to the resource table.
*   port_type            - One of the 6 CPB DPI port types.
*
* OUTPUTS:
*   channel              - Other than for ENET_LINE masked as MAPOTN the 
*                          channel value remains unchanged. For ENET_LINE 
*                          cases the channel is the channel in the ENET LINE
*                          subsystem and not the index relative to where the
*                          MAPOTN channel handle is. For this the channel 
*                          is updated.
*
* RETURNS:
*   digi_resource_next_handle_t - value matching the port_type.
*
* NOTES:
*
*******************************************************************************/
PUBLIC digi_resource_next_handle_t digi_resource_get_cpb_resource_type(digi_handle_t *digi_handle,
                                                                        digi_resource_handle_summary_t *digi_resource_handle, 
                                                                         util_global_cpb_port_t port_type, 
                                                                         UINT32 *channel)
{
    digi_resource_next_handle_t resource_type = LAST_DIGI_RESOURCE_HANDLE;
    UINT16 i;

    PMC_ENTRY();

    switch (port_type)
    {
    case UTIL_GLOBAL_CPB_PORT_ILKN1:
      resource_type = DIGI_ILKN_1;
      break;

    case UTIL_GLOBAL_CPB_PORT_ILKN2:
      resource_type = DIGI_ILKN_2;
      break;

    case UTIL_GLOBAL_CPB_PORT_CBRC:
    {
      /*
       * This could be LINE, SFI51 or SYSTEM side connection. Need some
       * other parameters to determine what value this.
       */
        void *cbrc_handle_ptr = (void *)&digi_handle->handle_pool.cbr_chnl[*channel];
        
        for (i=0; i < DIGI_XFI_PIN_NUM; i++)
        {
            if (DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T ==
                digi_resource_handle->line_serdes_cfg[i].chnl_handle_type &&
                digi_resource_handle->line_serdes_cfg[i].chnl_handle_ptr ==
                cbrc_handle_ptr)
            {
                *channel = i;
                resource_type = DIGI_LINE_SERDES_CFG;
                break;
            }
        }
        for (i=0; i < DIGI_XFI_PIN_NUM; i++)
        {
            if (DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T ==
                digi_resource_handle->sys_serdes_cfg[i].chnl_handle_type &&
                digi_resource_handle->sys_serdes_cfg[i].chnl_handle_ptr ==
                cbrc_handle_ptr)
            {
                *channel = i;
                resource_type = DIGI_SYS_SERDES_CFG;
                break;
            }
        }
      
        for (i=0; i < DIGI_SFI51_INTF_NUM; i++)
        {
            if (DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T ==
                digi_resource_handle->sfi51_serdes_cfg[i].chnl_handle_type &&
                digi_resource_handle->sfi51_serdes_cfg[i].chnl_handle_ptr ==
                cbrc_handle_ptr)
            {
                *channel = i;
                resource_type = DIGI_SFI51_SERDES_CFG;
                break;
            }
        }
    }
      break;

    case UTIL_GLOBAL_CPB_PORT_MAPOTN:
      resource_type = DIGI_OTN_MAPPER;
      break;

    case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
      /*
       * For ENET_LINE need to determine whether or not it is ENET_LINE or
       * ENET_LINE filling in for MAPOTN
       * Search through the MAPOTN channel handles to determine if there is
       * a port type channel match on the CPB Switch data. If there is then
       * this is MAPOTN and the channel needs to be updated.
       */
      resource_type = DIGI_LINE_SERDES_CFG;

      for (i=0; i < DIGI_MAPOTN_CHNL_MAX; i++)
      {
          digi_mapper_chnl_def_t *map_def_ptr = (digi_mapper_chnl_def_t *)digi_resource_handle->mapper_cfg[i].mapper_handle_ptr;

          if (NULL != map_def_ptr)
          {
              util_global_switch_data_def_t *switch_data_ptr = &(map_def_ptr->switch_data);
              if (UTIL_GLOBAL_CPB_PORT_ENET_LINE == switch_data_ptr->cpb_data.port_type &&
                  *channel == switch_data_ptr->cpb_data.channel)
              {
                  resource_type = DIGI_OTN_MAPPER;
                  *channel = i;
                  break;
              }
          }
      }
      /*
       * In a 10G cage then serdes_index and the enet channel handle will
       * be the same. In a 40G cage however they very well may not. If this
       * is a SERDES_LINE and not MAPTON then Need to
       * iterate over the serdes channel handles and find the pointer that 
       * matches the one in the digi_handle. When it does then that is the
       * index that needs to be inserted into the table.
       */
      if (DIGI_LINE_SERDES_CFG == resource_type)
      {
          BOOL8 bFound = FALSE;
          void *enet_handle_ptr = (void *)&digi_handle->handle_pool.enet_line_chnl[*channel];
          
          for (i=0; i < DIGI_XFI_PIN_NUM; i++)
          {
              if (DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T ==
                  digi_resource_handle->line_serdes_cfg[i].chnl_handle_type &&
                  digi_resource_handle->line_serdes_cfg[i].chnl_handle_ptr ==
                  enet_handle_ptr)
              {
                  *channel = i;
                  bFound = TRUE;
                  break;
              }
          }
          if (FALSE == bFound &&
              DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == digi_handle->var.prod_app)
          {
              for (i=0; i < DIGI_XFI_PIN_NUM; i++)
              {
                  if (DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T ==
                      digi_resource_handle->sys_serdes_cfg[i].chnl_handle_type &&
                      digi_resource_handle->sys_serdes_cfg[i].chnl_handle_ptr ==
                      enet_handle_ptr)
                  {
                      *channel = i;
                      resource_type = DIGI_SYS_SERDES_CFG;
                      break;
                  }
              }
          }
      }
      break;

    case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
      resource_type = DIGI_SYS_SERDES_CFG;

      /*
       * In a 10G cage then serdes_index and the enet channel handle will
       * be the same. In a 40G cage however they very well may not. Need to
       * iterate over the serdes channel handles and find then that is the
       * index that needs to be inserted into the table.
       */
      if (DIGI_SYS_SERDES_CFG == resource_type)
      {
          void *enet_handle_ptr = (void *)&digi_handle->handle_pool.enet_sys_chnl[*channel];

          for (i=0; i < DIGI_XFI_PIN_NUM; i++)
          {
              if (DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T ==
                  digi_resource_handle->sys_serdes_cfg[i].chnl_handle_type &&
                  digi_resource_handle->sys_serdes_cfg[i].chnl_handle_ptr ==
                  enet_handle_ptr)
              {
                  *channel = i;
                  break;
              }
          }
      }
      break;

    default:
      break;
    }

    PMC_RETURN(resource_type);
} /* digi_resource_get_cpb_resource_type */

/*******************************************************************************
* digi_resource_is_cbrc_operational
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Iterates over the CBRC data and determines if it is operational.
*
* INPUTS:
*   digi_handle          - Reference to the digi_handle
*   digi_resource_handle - Reference to the resource table.
*   serdes_port_ptr      - References to the serdes port information
*   check_all            - Flag that indicates whether or not only this enet
*                          line resource should be checked or should 
*                          continue checking.
*   direction            - Relative direction the data path is being 
*                          checked in.
* OUTPUTS:
*   None
*
* RETURNS:
*   TRUE:  Path is fully operational and good.
*   FALSE: There is something that is not operational in the path.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 digi_resource_is_cbrc_operational(digi_handle_t *digi_handle,
                                               digi_resource_handle_summary_t *digi_resource_handle,
                                               digi_serdes_port_intf_ctxt_t *serdes_port_ptr,
                                               BOOL8 check_all,
                                               digi_direction_check_t direction)
{
    BOOL8 result = FALSE;
    UINT16 switch_index;
    UINT32 cpb_chnl;
    digi_cbr_client_chnl_def_t *cbr_ptr;
    BOOL8 tx_result = FALSE;
    BOOL8 rx_result = FALSE;
    BOOL8 is_tx_behaviour = TRUE;

    PMC_ENTRY();

    if (NULL == serdes_port_ptr->chnl_handle_ptr)
    {
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }
    cbr_ptr = (digi_cbr_client_chnl_def_t *)serdes_port_ptr->chnl_handle_ptr;

    if (NULL != cbr_ptr)
    {
        cpb_chnl = cbr_ptr->switch_data.cpb_data.channel;
        switch_index = serdes_port_ptr->next_handle_index;
    }
    if (DIGI_CPB_SWITCH != serdes_port_ptr->next_resource_handle)
    {
        PMC_LOG_TRACE("ENET LINE/SYS Next Resource is not CPB_SWITCH\n");
    }
    else if (NULL == cbr_ptr || 
             UTIL_GLOBAL_CPB_PORT_CBRC != cbr_ptr->switch_data.cpb_data.port_type)
    {
        /*
         * If the pointer is NULL, the active state is not ACTIVE, namely
         * operational, ir  the type is note CBRC then no need to go any
         * further.
         */
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0, "ENET LINE/SYS not VALID or NOT ACTIVE\n");
    }
    else if (!(DIGI_LINE_SERDES_CFG == digi_resource_handle->cpbsw_cfg[switch_index].provisioned_handle_type || 
               DIGI_SFI51_SERDES_CFG == digi_resource_handle->cpbsw_cfg[switch_index].provisioned_handle_type ||
               DIGI_SYS_SERDES_CFG == digi_resource_handle->cpbsw_cfg[switch_index].provisioned_handle_type))
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0, 
                "PortType=%d is CBRC but Resource MISCONFIGURED\n",
                cbr_ptr->switch_data.cpb_data.port_type);   
    }
    else if (FALSE == digi_resource_is_direction_valid(direction,
                                                       check_all,
                                                       cbr_ptr->switch_data.cpb_data.op_state.active_state,
                                                       cbr_ptr->switch_data.cpb_data.op_state.map_state.input_map_count,
                                                       &digi_resource_handle->cpbsw_cfg[switch_index],
                                                       &is_tx_behaviour))
    {
         /*
         * Check that the destination for TX have been set. Otherwise this
         * is an error
         */
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "CBRC Chnl=%d not TX OPERATIONAL on CPB\n", cpb_chnl);
    }
    else if (TRUE == is_tx_behaviour &&
             FALSE == cbrc_prov_get(digi_handle->cbrc_handle,
                                    cbr_ptr->switch_data.cpb_data.channel,
                                    FALSE))
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "CBRC PortType=%d Chnl=%d not OPERATIONAL on TX side\n", 
                cbr_ptr->switch_data.cpb_data.port_type, cpb_chnl);
        digi_resource_handle->force_release = TRUE;
    }
    else if (FALSE == is_tx_behaviour &&
             FALSE == cbrc_prov_get(digi_handle->cbrc_handle,
                                    cbr_ptr->switch_data.cpb_data.channel,
                                    TRUE))
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "CBRC PortType=%d Chnl=%d not OPERATIONAL on RX side\n", 
                cbr_ptr->switch_data.cpb_data.port_type, cpb_chnl);
        digi_resource_handle->force_release = TRUE;
    }
    else if (TRUE == check_all && 
             !(LAST_DIGI_RESOURCE_HANDLE == digi_resource_handle->cpbsw_cfg[switch_index].source_handle_type &&
               LAST_DIGI_RESOURCE_HANDLE == digi_resource_handle->cpbsw_cfg[switch_index].primary_dest_handle_type))
    {
        /*
         * If checking all ensure that there is at least a source or 
         * primary destination handle defined.  If there is not this is 
         * still valid and will be set below.
         * From here the state is validity of the path is based on whether
         * not the path is complete in the direction being checked.
         */
        if (DIGI_DIRECTION_TX_RX == direction ||
            DIGI_DIRECTION_TX_ONLY == direction)
        {
            tx_result = digi_resource_is_switch_operational(digi_handle,
                                                            digi_resource_handle,
                                                            switch_index,
                                                            DIGI_CPB_SWITCH,
                                                            TRUE,
                                                            DIGI_DIRECTION_TX_ONLY);
            serdes_port_ptr->tx_path_is_valid = tx_result;
        }
        if (DIGI_DIRECTION_TX_RX == direction ||
            DIGI_DIRECTION_RX_ONLY == direction)
        {
            rx_result =  digi_resource_is_switch_operational(digi_handle,
                                                             digi_resource_handle,
                                                             switch_index,
                                                             DIGI_CPB_SWITCH,
                                                             TRUE,
                                                             DIGI_DIRECTION_RX_ONLY);
            serdes_port_ptr->rx_path_is_valid = rx_result;
        }
        result = digi_resource_set_directional_result(direction, tx_result, rx_result);
        serdes_port_ptr->path_verified = TRUE;
    }
    else
    {
        result = TRUE;
        /*
         * Note the directions are crossed on purpose because if here
         * have arrived from another serdes port. From that port TX is
         * RX on the other side.
         */
        if (DIGI_DIRECTION_TX_ONLY == direction)
        {
            serdes_port_ptr->rx_path_is_valid = TRUE;
        }
        else
        {
            serdes_port_ptr->tx_path_is_valid = TRUE;
        }
        serdes_port_ptr->path_verified = TRUE;
    }

    if (TRUE == result && TRUE == check_all &&
        LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->cpbsw_cfg[switch_index].source_handle_type &&
        LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->cpbsw_cfg[switch_index].primary_dest_handle_type &&    
        !(digi_resource_handle->cpbsw_cfg[switch_index].source_handle_type ==
          digi_resource_handle->cpbsw_cfg[switch_index].primary_dest_handle_type &&
          digi_resource_handle->cpbsw_cfg[switch_index].source_handle_index ==
          digi_resource_handle->cpbsw_cfg[switch_index].primary_dest_index))
    {
       /*
         * Handles cases where the source and destination are not the
         * same indicating that this is likely a broad case/multi case
         * case.
         */
        if (DIGI_DIRECTION_TX_RX == direction ||
            DIGI_DIRECTION_TX_ONLY == direction)
        {
            tx_result = digi_resource_is_switch_operational(digi_handle,
                                                            digi_resource_handle,
                                                            switch_index,
                                                            DIGI_CPB_SWITCH,
                                                            TRUE,
                                                            DIGI_DIRECTION_TX_ONLY);
            serdes_port_ptr->tx_path_is_valid = tx_result;
        }
        if (DIGI_DIRECTION_TX_RX == direction ||
            DIGI_DIRECTION_RX_ONLY == direction)
        {
            rx_result = digi_resource_is_switch_operational(digi_handle,
                                                            digi_resource_handle,
                                                            switch_index,
                                                            DIGI_CPB_SWITCH,
                                                            TRUE,
                                                            DIGI_DIRECTION_RX_ONLY);
            serdes_port_ptr->rx_path_is_valid = rx_result;
        }

        result = digi_resource_set_directional_result(direction, tx_result, rx_result);
    }

    PMC_RETURN(result);
} /*digi_resource_is_cbrc_operational */


/*******************************************************************************
* digi_resource_is_enet_operational
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Iterates over the ENET LINE/SYS data and determines if it is operational.
*
* INPUTS:
*   digi_handle          - Reference to the digi_handle
*   digi_resource_handle - Reference to the resource table.
*   serdes_port_ptr      - References to the serdes port information
*   check_all            - Flag that indicates whether or not only this enet
*                          line resource should be checked or should 
*                          continue checking.
*   direction            - Relative direction the data path is being
*                          checked.
* OUTPUTS:
*   None
*
* RETURNS:
*   TRUE:  Path is fully operational and good.
*   FALSE: There is something that is not operational in the path.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 digi_resource_is_enet_operational(digi_handle_t *digi_handle,
                                               digi_resource_handle_summary_t *digi_resource_handle,
                                               digi_serdes_port_intf_ctxt_t *serdes_port_ptr,
                                               BOOL8 check_all,
                                               digi_direction_check_t direction)
{
    BOOL8 result = FALSE;
    UINT16 switch_index;
    UINT32 cpb_chnl;
    digi_enet_client_chnl_def_t *enet_ptr;
    BOOL8 tx_result = FALSE;
    BOOL8 rx_result = FALSE;
    BOOL8 is_tx_behaviour = TRUE;

    PMC_ENTRY();

    if (NULL == serdes_port_ptr->chnl_handle_ptr)
    {
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }
    enet_ptr = (digi_enet_client_chnl_def_t *)serdes_port_ptr->chnl_handle_ptr;

    if (NULL != enet_ptr)
    {
        cpb_chnl = enet_ptr->switch_data.cpb_data.channel;
        switch_index = serdes_port_ptr->next_handle_index;

    }

    if (DIGI_CPB_SWITCH != serdes_port_ptr->next_resource_handle)
    {
        PMC_LOG_TRACE("ENET LINE/SYS Next Resource is not CPB_SWITCH\n");
    }
    else if (NULL == enet_ptr || 
             !(UTIL_GLOBAL_CPB_PORT_ENET_LINE == enet_ptr->switch_data.cpb_data.port_type ||
               UTIL_GLOBAL_CPB_PORT_ENET_SYS == enet_ptr->switch_data.cpb_data.port_type) ||
             FALSE != enet_ptr->switch_data.cpb_data.data_source) 
    {
        /*
         * If the pointer is NULL, the active state is not ACTIVE, namely
         * operational, the type is note ENET LINE or SYS, or the data
         * source is TRUE indicating it is from ODUK and not SERDES then
         * that is an error.
         */
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0, "ENET LINE/SYS not VALID or NOT ACTIVE\n");
    }
    else if (!(DIGI_LINE_SERDES_CFG == digi_resource_handle->cpbsw_cfg[switch_index].provisioned_handle_type || 
               DIGI_SFI51_SERDES_CFG == digi_resource_handle->cpbsw_cfg[switch_index].provisioned_handle_type ||
               DIGI_SYS_SERDES_CFG == digi_resource_handle->cpbsw_cfg[switch_index].provisioned_handle_type))
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0, 
                "PortType=%d is ENET LINE/SYS but Resource MISCONFIGURED\n",
                enet_ptr->switch_data.cpb_data.port_type);   
    }
    else if (FALSE == digi_resource_is_direction_valid(direction,
                                                       check_all,
                                                       enet_ptr->switch_data.cpb_data.op_state.active_state,
                                                       enet_ptr->switch_data.cpb_data.op_state.map_state.input_map_count,
                                                       &digi_resource_handle->cpbsw_cfg[switch_index],
                                                       &is_tx_behaviour))
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "ENET PortType=%d Chnl=%d TX not OPERATIONAL on CPB\n", 
                enet_ptr->switch_data.cpb_data.port_type, cpb_chnl);

    }
    else if (TRUE == is_tx_behaviour &&
             FALSE == digi_enet_does_state_match(digi_handle, 
                                                 cpb_chnl, 
                                                 FALSE,
                                                 enet_ptr->switch_data.cpb_data.port_type,
                                                 UTIL_GLOBAL_CHNL_OPERATIONAL))
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "ENET PortType=%d Chnl=%d not OPERATIONAL on TX side in ENET SUBSYSTEM\n", 
                enet_ptr->switch_data.cpb_data.port_type, cpb_chnl);
        digi_resource_handle->force_release = TRUE;
    }
    else if (FALSE == is_tx_behaviour &&
             FALSE == digi_enet_does_state_match(digi_handle, 
                                                 cpb_chnl, 
                                                 TRUE,
                                                 enet_ptr->switch_data.cpb_data.port_type,
                                                 UTIL_GLOBAL_CHNL_OPERATIONAL))
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "ENET PortType=%d Chnl=%d not OPERATIONAL on RX side in ENET SUBSYSTEM\n", 
                enet_ptr->switch_data.cpb_data.port_type, cpb_chnl);
        digi_resource_handle->force_release = TRUE;
    }
    else if (TRUE == check_all &&
             !(LAST_DIGI_RESOURCE_HANDLE == digi_resource_handle->cpbsw_cfg[switch_index].source_handle_type &&
               LAST_DIGI_RESOURCE_HANDLE == digi_resource_handle->cpbsw_cfg[switch_index].primary_dest_handle_type))
    {
        /*
         * If here then ENET channel is activated and up. Now need to
         * check what it is connected to and follow the path along.
         * Each direction is checked separately.
         */
        if (DIGI_DIRECTION_TX_RX == direction ||
            DIGI_DIRECTION_TX_ONLY == direction)
        {
            tx_result = digi_resource_is_switch_operational(digi_handle,
                                                            digi_resource_handle,
                                                            switch_index,
                                                            DIGI_CPB_SWITCH,
                                                            TRUE,
                                                            DIGI_DIRECTION_TX_ONLY);
            serdes_port_ptr->tx_path_is_valid = tx_result;
        }

        if (DIGI_DIRECTION_TX_RX == direction ||
            DIGI_DIRECTION_RX_ONLY == direction)
        {
            rx_result =  digi_resource_is_switch_operational(digi_handle,
                                                             digi_resource_handle,
                                                             switch_index,
                                                             DIGI_CPB_SWITCH,
                                                             TRUE,
                                                             DIGI_DIRECTION_RX_ONLY);
            serdes_port_ptr->rx_path_is_valid = rx_result;
        }
        /*
         * Set the result to TRUE as the datapath is valid as there is
         * at least a fully provisioned port.
         */
        result = digi_resource_set_directional_result(direction, tx_result,
                                                      rx_result);
        serdes_port_ptr->path_verified = TRUE;
    }
    else
    {
        result = TRUE;
        /*
         * Note the directions are crossed on purpose because if here
         * have arrived from another serdes port. From that port TX is
         * RX on the other side.
         */
        if (DIGI_DIRECTION_TX_ONLY == direction)
        {
            serdes_port_ptr->rx_path_is_valid = TRUE;
        }
        else
        {
            serdes_port_ptr->tx_path_is_valid = TRUE;
        }
        serdes_port_ptr->path_verified = TRUE;
    }
    if (TRUE == result && TRUE == check_all &&
        LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->cpbsw_cfg[switch_index].source_handle_type &&
        LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->cpbsw_cfg[switch_index].primary_dest_handle_type && 
        !(digi_resource_handle->cpbsw_cfg[switch_index].source_handle_type ==
          digi_resource_handle->cpbsw_cfg[switch_index].primary_dest_handle_type &&
          digi_resource_handle->cpbsw_cfg[switch_index].source_handle_index ==
          digi_resource_handle->cpbsw_cfg[switch_index].primary_dest_index))
    {
        /*
         * Handles cases where the source and destination are not the
         * same indicating that this is likely a broad case/multi case
         * case.
         */
        if (DIGI_DIRECTION_TX_RX == direction ||
            DIGI_DIRECTION_TX_ONLY == direction)
        {
            tx_result = digi_resource_is_switch_operational(digi_handle,
                                                            digi_resource_handle,
                                                            switch_index,
                                                            DIGI_CPB_SWITCH,
                                                            TRUE,
                                                            DIGI_DIRECTION_TX_ONLY);
            serdes_port_ptr->tx_path_is_valid = tx_result;
        }
        if (DIGI_DIRECTION_TX_RX == direction ||
            DIGI_DIRECTION_RX_ONLY == direction)
        {
            rx_result = digi_resource_is_switch_operational(digi_handle,
                                                            digi_resource_handle,
                                                            switch_index,
                                                            DIGI_CPB_SWITCH,
                                                            TRUE,
                                                            DIGI_DIRECTION_RX_ONLY);
            serdes_port_ptr->rx_path_is_valid = rx_result;
        }

        result = digi_resource_set_directional_result(direction, tx_result, rx_result);;
        serdes_port_ptr->path_verified = TRUE;
    }

    PMC_RETURN(result);
} /*digi_resource_is_enet_operational */

/*******************************************************************************
* digi_resource_is_lineotn_operational
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Iterates over the ENET LINE?SYS data and determines if it is operational.
*
* INPUTS:
*   digi_handle          - Reference to the digi_handle
*   digi_resource_handle - Reference to the resource table.
*   serdes_port_ptr      - References to the serdes port information
*   is_line_side_serdes  - Set to FALSE if a SYSOTN applicatin and 
*                          examining the SYSOTN datapath. TRUE otherwise.
*   check_all            - Flag that indicates this call is the originator
*                          of the checking and need to continue underlying
*                          resources or whether this is the last check and
*                          only this resource need be checked.
*   direction            - Direction being evaluated. Examined only if
*                          check_all is false.
* OUTPUTS:
*   is_operational_ptr   - Whether there is an operational component with
*                          in the OTN hierarchy.
*
* RETURNS:
*   TRUE:  Path is fully operational and good.
*   FALSE: There is something that is not operational in the path.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 digi_resource_is_lineotn_operational(digi_handle_t *digi_handle,
                                                  digi_resource_handle_summary_t *digi_resource_handle,
                                                  digi_serdes_port_intf_ctxt_t *serdes_port_ptr,
                                                  BOOL8 is_line_side_serdes,
                                                  BOOL8 check_all,
                                                  digi_direction_check_t direction,
                                                  BOOL8 *is_operational_ptr)
{
    digi_otn_server_chnl_def_t *otn_chnl_ptr = NULL;
    BOOL8 result = FALSE;
    PMC_ENTRY();
    
    is_line_side = is_line_side_serdes;


    if (FALSE == is_line_side && FALSE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle))
    {
        is_line_side = TRUE;
    }

    if (NULL == serdes_port_ptr || NULL == serdes_port_ptr->chnl_handle_ptr)
    {
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }
    otn_chnl_ptr = (digi_otn_server_chnl_def_t *)serdes_port_ptr->chnl_handle_ptr;

    if (NULL == otn_chnl_ptr ||
        (TRUE != util_global_rx_or_tx_chnl_prov_status_get(otn_chnl_ptr->header.prov_state)))
    {
        PMC_LOG_TRACE("LineChannelPointer is NULL or State NOT PROVISIONED");
    }
    else if (LAST_DIGI_RESOURCE_HANDLE == serdes_port_ptr->next_resource_handle)
    {
        PMC_LOG_TRACE("LineOTN Next resource not provisioned\n");
    }
    else if (TRUE == is_line_side)
    {
        /*
         * NOTE: Currently lineotn is assumed to be bi-directional although
         * that is not necessarily the case. Need to look at adding a 
         * directional parameter and running this as 2 separate Tx and Rx
         * datapath checks.
         */
        if (FALSE == lineotn_prov_get(digi_handle->lineotn_handle,
                                         otn_chnl_ptr->dsi_base_chnl,
                                         TRUE))
        {
            PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                    "LineOTN DSI Rx Chnl=%d is not PROVISIONED\n",
                    otn_chnl_ptr->dsi_base_chnl);
            digi_resource_handle->force_release = TRUE;
        }
        else if (FALSE == lineotn_prov_get(digi_handle->lineotn_handle,
                                           otn_chnl_ptr->dsi_base_chnl,
                                           FALSE))
        {
            PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                    "LineOTN DSI Tx Chnl=%d is not PROVISIONED\n",
                    otn_chnl_ptr->dsi_base_chnl);
            digi_resource_handle->force_release = TRUE;
        }
        else
        {
            result = TRUE;
        }
    }
    else if (FALSE == lineotn_prov_get(digi_handle->sysotn_handle,
                                       otn_chnl_ptr->dsi_base_chnl,
                                       TRUE))
    {
        PMC_LOG_TRACE("SysOTN DSI Rx Chnl=%d is not PROVISIONED\n",
                      otn_chnl_ptr->dsi_base_chnl);
        digi_resource_handle->force_release = TRUE;
    }
    else if (FALSE == lineotn_prov_get(digi_handle->sysotn_handle,
                                       otn_chnl_ptr->dsi_base_chnl,
                                       FALSE))
    {
        PMC_LOG_TRACE("SysOTN DSI Tx Chnl=%d is not PROVISIONED\n",
                      otn_chnl_ptr->dsi_base_chnl);
        digi_resource_handle->force_release = TRUE;
    }
    else
    {
        result = TRUE;
    }

    if (TRUE == result && TRUE == check_all &&
        LAST_DIGI_RESOURCE_HANDLE != serdes_port_ptr->next_resource_handle)
    {
        BOOL8 tx_result = FALSE;
        BOOL8 rx_result = FALSE;
        /*
         * If here then lineOTN channel is activated and up. Now need to 
         * check the underlying OTN connections to determine what is up and
         * what is not. 
         * The direction value takes a BOTH as currently at this level it is
         * assumed bi-directional. That will be adjusted as required when
         * the OTN containers are checked.
         */
        if (DIGI_DIRECTION_TX_RX == direction ||
            DIGI_DIRECTION_TX_ONLY == direction)
        {
            tx_result = digi_resource_is_otn_operational(digi_handle,
                                                         digi_resource_handle,
                                                         serdes_port_ptr->next_resource_handle,
                                                         serdes_port_ptr->next_handle_index,
                                                         FALSE,
                                                         DIGI_DIRECTION_TX_ONLY);
            serdes_port_ptr->tx_path_is_valid = tx_result;
        }

        if (DIGI_DIRECTION_TX_RX == direction ||
            DIGI_DIRECTION_RX_ONLY == direction)
        {
            rx_result = digi_resource_is_otn_operational(digi_handle,
                                                         digi_resource_handle,
                                                         serdes_port_ptr->next_resource_handle,
                                                         serdes_port_ptr->next_handle_index,
                                                         FALSE,
                                                         DIGI_DIRECTION_RX_ONLY);
            serdes_port_ptr->rx_path_is_valid = rx_result;
        }
        serdes_port_ptr->path_verified = TRUE;
        *is_operational_ptr = TRUE;

        if (DIGI_LINE_OTN_HO_CONTAINER == serdes_port_ptr->next_resource_handle)
        {
            if (TRUE == serdes_port_ptr->rx_path_is_valid && TRUE == serdes_port_ptr->tx_path_is_valid)
            {
                result = TRUE;
            }
            else
            {
                result = FALSE;
            }
        } 
        else if (DIGI_SYS_OTN_HO_CONTAINER == serdes_port_ptr->next_resource_handle)
        {
            result = FALSE;

            if (TRUE == serdes_port_ptr->rx_path_is_valid && TRUE == serdes_port_ptr->tx_path_is_valid)
            {
                result = TRUE;
            }
        }
    }
    else if (LAST_DIGI_RESOURCE_HANDLE == serdes_port_ptr->next_resource_handle)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "Next Resource UNSET. Path is invalid. SerdesPort=%d\n",
                serdes_port_ptr->port_uid);
        result = FALSE;
        serdes_port_ptr->path_verified = TRUE;
        *is_operational_ptr = FALSE;
    }
    else if (TRUE == result)
    {
        /*
         * Note the directions are crossed on purpose because if here
         * have arrived from another serdes port. From that port TX is
         * RX on the other side.
         */
        if (DIGI_DIRECTION_TX_ONLY == direction)
        {
            serdes_port_ptr->rx_path_is_valid = TRUE;
        }
        else
        {
            serdes_port_ptr->tx_path_is_valid = TRUE;
        }
        serdes_port_ptr->path_verified = TRUE;
        *is_operational_ptr = TRUE;
    }

    PMC_RETURN(result);

} /*digi_resource_is_lineotn_operational */


/*******************************************************************************
* digi_resource_release_enet
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Release the data path initiated from the enet sys or line resource 
*  connected to a serdes port.  Releases all digi layer resources and resets
*  DIGI level data base entries for the data path. Underlying subsystem 
*  release related operations are not specifically called and it is 
*  expected that the subsystems handle_restart_init will be called after-
*  wards to clean up reset any resources associated with this data path.
*  Function is associated with the cleaning of resources as they pertain
*  to crash restart.
*
*
* INPUTS:
*   digi_handle          - Reference to the digi_handle
*   digi_resource_handle - Reference to the resource table.
*   serdes_port_ptr      - References to the serdes port information
*
* OUTPUTS:
*   None
*
* RETURNS:
*   TRUE:  Path is fully operational and good.
*   FALSE: There is something that is not operational in the path.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_resource_release_enet(digi_handle_t *digi_handle,
                                       digi_resource_handle_summary_t *digi_resource_handle,
                                       digi_serdes_port_intf_ctxt_t *serdes_port_ptr)
{
    PMC_ENTRY();

    if (NULL == serdes_port_ptr->chnl_handle_ptr)
    {
        PMC_LOG_TRACE("ENET Line ptr is NULL. There is nothing to do\n");
        PMC_RETURN();
    }

    if (FALSE == digi_resource_handle->force_release)
    {
        BOOL8 prov_status;
        digi_serdes_port_mode_t port_mode;
        digi_serdes_prov_state_t prov_state;
        PMC_ERROR result;

        result = digi_serdes_port_rate_prov_status_internal_get(digi_handle,
                                                                (digi_serdes_port_t *)serdes_port_ptr->port_ctxt_ptr,
                                                                FALSE,
                                                                &prov_status,
                                                                &port_mode,
                                                                &prov_state);
        
        if (PMC_SUCCESS != result || FALSE == prov_status ||
            DIGI_SERDES_PROV_STATE_UNASSIGNED == prov_state ||
            DIGI_SERDES_PROV_STATE_NO_SERDES == prov_state)
        {
            digi_resource_handle->force_release = TRUE;
        }
    }

    if (LAST_DIGI_RESOURCE_HANDLE != serdes_port_ptr->next_resource_handle)
    {
        digi_resource_handle->datapath_release = FALSE;

        if (FALSE == serdes_port_ptr->tx_path_is_valid ||
            TRUE == digi_resource_handle->force_release)
        {
            BOOL8 is_broadcast = FALSE;

            if (FALSE == digi_resource_handle->force_release)
            {
                is_broadcast = digi_resource_check_cpb_broadcast(digi_handle,
                                                                 digi_resource_handle,
                                                                 serdes_port_ptr);
            }

            if (FALSE == is_broadcast)
            {
                (void)digi_resource_restart_release_port(digi_handle,
                                                         digi_resource_handle,
                                                         serdes_port_ptr->chnl_handle_ptr,
                                                         serdes_port_ptr->next_resource_handle,
                                                         serdes_port_ptr->next_handle_index,
                                                         FALSE,
                                                         DIGI_DIRECTION_TX_ONLY);
            }
        }
        digi_resource_handle->datapath_release = FALSE;

        if (FALSE == serdes_port_ptr->rx_path_is_valid ||
            TRUE == digi_resource_handle->force_release)
        {
            (void)digi_resource_restart_release_port(digi_handle,
                                                     digi_resource_handle,
                                                     serdes_port_ptr->chnl_handle_ptr,
                                                     serdes_port_ptr->next_resource_handle,
                                                     serdes_port_ptr->next_handle_index,
                                                     FALSE,
                                                     DIGI_DIRECTION_RX_ONLY);
        }
    }
    /*
     * Deprovision the enet line resource is no other resources are 
     * currently using that interface.
     */
    if (TRUE == digi_resource_handle->force_release)
    {
        digi_resource_enet_deprov(digi_handle, 
                                  (digi_enet_client_chnl_t **)&serdes_port_ptr->chnl_handle_ptr,
                                  serdes_port_ptr->port_ctxt_ptr,
                                  serdes_port_ptr->port_uid);
        digi_resource_reset_serdes_handle(serdes_port_ptr);
        PMC_LOG_TRACE("EnetLine deprovision CALLED\n");
    }        
    digi_resource_handle->force_release = FALSE;
    digi_resource_handle->datapath_release = FALSE;

    PMC_RETURN();
} /* digi_resource_release_enet */


/*******************************************************************************
* digi_resource_release_cbrc
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Release the data path initiated cbrc client resource connected to a 
*  serdes port.  Releases all digi layer resources and resets
*  DIGI level data base entries for the data path. Underlying subsystem 
*  release related operations are not specifically called and it is 
*  expected that the subsystems handle_restart_init will be called after-
*  wards to clean up reset any resources associated with this data path.
*  Function is associated with the cleaning of resources as they pertain
*  to crash restart.
*
*
* INPUTS:
*   digi_handle          - Reference to the digi_handle
*   digi_resource_handle - Reference to the resource table.
*   serdes_port_ptr      - References to the serdes port information
*
* OUTPUTS:
*   None
*
* RETURNS:
*   TRUE:  Path is fully operational and good.
*   FALSE: There is something that is not operational in the path.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_resource_release_cbrc(digi_handle_t *digi_handle,
                                       digi_resource_handle_summary_t *digi_resource_handle,
                                       digi_serdes_port_intf_ctxt_t *serdes_port_ptr)
{
    PMC_ENTRY();

    if (NULL == serdes_port_ptr->chnl_handle_ptr)
    {
        PMC_LOG_TRACE("CBR Client ptr is NULL. There is nothing to do\n");
        PMC_RETURN();
    }

    if (FALSE == digi_resource_handle->force_release)
    {
        PMC_ERROR result;
        BOOL8 prov_status;
        digi_serdes_port_mode_t port_mode;
        digi_serdes_prov_state_t prov_state;

        result = digi_serdes_port_rate_prov_status_internal_get(digi_handle,
                                                                (digi_serdes_port_t *)serdes_port_ptr->port_ctxt_ptr,
                                                                FALSE,
                                                                &prov_status,
                                                                &port_mode,
                                                                &prov_state);

        if (PMC_SUCCESS != result || FALSE == prov_status ||
            DIGI_SERDES_PROV_STATE_UNASSIGNED == prov_state ||
            DIGI_SERDES_PROV_STATE_NO_SERDES == prov_state)
        {
            digi_resource_handle->force_release = TRUE;
        }
    }

    if (LAST_DIGI_RESOURCE_HANDLE != serdes_port_ptr->next_resource_handle)
    {
        digi_resource_handle->datapath_release = FALSE;

        if(FALSE == serdes_port_ptr->tx_path_is_valid ||
           TRUE == digi_resource_handle->force_release)
        {
            BOOL8 is_broadcast = FALSE;

            if (FALSE == digi_resource_handle->force_release)
            {
                is_broadcast = digi_resource_check_cpb_broadcast(digi_handle,
                                                                 digi_resource_handle,
                                                                 serdes_port_ptr);
            }

            if (FALSE == is_broadcast)
            {
                (void)digi_resource_restart_release_port(digi_handle,
                                                         digi_resource_handle,
                                                         serdes_port_ptr->chnl_handle_ptr,
                                                         serdes_port_ptr->next_resource_handle,
                                                         serdes_port_ptr->next_handle_index,
                                                         FALSE,
                                                         DIGI_DIRECTION_TX_ONLY);
            }
        }
        digi_resource_handle->datapath_release = FALSE;
        
        if (FALSE == serdes_port_ptr->rx_path_is_valid ||
            TRUE == digi_resource_handle->force_release)
        {
            (void)digi_resource_restart_release_port(digi_handle,
                                                     digi_resource_handle,
                                                     serdes_port_ptr->chnl_handle_ptr,
                                                     serdes_port_ptr->next_resource_handle,
                                                     serdes_port_ptr->next_handle_index,
                                                     FALSE,
                                                     DIGI_DIRECTION_RX_ONLY);
        }
    }

    /*
     * Deprovision the CBRC port if neither the tx or rx path is valid
     */
    if (TRUE == digi_resource_handle->force_release)
    {
        digi_resource_cbrc_deprov(digi_handle, 
                                  (digi_enet_client_chnl_t **)&serdes_port_ptr->chnl_handle_ptr,
                                  serdes_port_ptr->port_ctxt_ptr,
                                  serdes_port_ptr->port_uid); 
        digi_resource_reset_serdes_handle(serdes_port_ptr);
    }
    digi_resource_handle->force_release = FALSE;
    digi_resource_handle->datapath_release = FALSE;

    PMC_RETURN();
} /* digi_resource_release_cbrc */


/*******************************************************************************
* digi_resource_release_lineotn
* ______________________________________________________________________________
*
* DESCRIPTION:
*  LineOTN channel handle is connected to a serdes SFI51 or lines side port
*  Need to release any and all resources found along the path.
*
*
* INPUTS:
*   digi_handle          - Reference to the digi_handle
*   digi_resource_handle - Reference to the resource table.
*   serdes_port_ptr      - References to the serdes port information
*   is_lineotn           - Flag to differentiate between release initiated
*                          from line side(TRUE) versus from sysotn sys side
*                          (FALSE)..
*
* OUTPUTS:
*   None
*
* RETURNS:
*   TRUE:  Path is fully operational and good.
*   FALSE: There is something that is not operational in the path.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_resource_release_lineotn(digi_handle_t *digi_handle,
                                          digi_resource_handle_summary_t *digi_resource_handle,
                                          digi_serdes_port_intf_ctxt_t *serdes_port_ptr,
                                          BOOL8 is_lineotn)
{
    PMC_ENTRY();    

    if (NULL == serdes_port_ptr || NULL == serdes_port_ptr->chnl_handle_ptr)
    {
        PMC_LOG_TRACE("LineOTN ptr is NULL. There is nothing to do\n");
        PMC_RETURN();
    }

    if (FALSE == digi_resource_handle->force_release)
    {
        PMC_ERROR result;
        BOOL8 prov_status;
        digi_serdes_port_mode_t port_mode;
        digi_serdes_prov_state_t prov_state;

        result = digi_serdes_port_rate_prov_status_internal_get(digi_handle,
                                                                (digi_serdes_port_t *)serdes_port_ptr->port_ctxt_ptr,
                                                                FALSE,
                                                                &prov_status,
                                                                &port_mode,
                                                                &prov_state);


        if (PMC_SUCCESS != result || FALSE == prov_status ||
            DIGI_SERDES_PROV_STATE_UNASSIGNED == prov_state ||
            DIGI_SERDES_PROV_STATE_NO_SERDES == prov_state)
        {
            digi_resource_handle->force_release = TRUE;
        }
    }

    if (LAST_DIGI_RESOURCE_HANDLE != serdes_port_ptr->next_resource_handle)
    {
        digi_resource_handle->datapath_release = FALSE;

        if (FALSE == serdes_port_ptr->tx_path_is_valid ||
            TRUE == digi_resource_handle->force_release)
        {
            (void)digi_resource_restart_release_port(digi_handle, 
                                                     digi_resource_handle,
                                                     serdes_port_ptr->chnl_handle_ptr,
                                                     serdes_port_ptr->next_resource_handle,
                                                     serdes_port_ptr->next_handle_index,
                                                     TRUE,
                                                     DIGI_DIRECTION_TX_ONLY);
        }
        digi_resource_handle->datapath_release = FALSE;

        if (FALSE == serdes_port_ptr->rx_path_is_valid ||
            TRUE == digi_resource_handle->force_release)
        {
            (void)digi_resource_restart_release_port(digi_handle, 
                                                     digi_resource_handle,
                                                     serdes_port_ptr->chnl_handle_ptr,
                                                     serdes_port_ptr->next_resource_handle,
                                                     serdes_port_ptr->next_handle_index,
                                                     TRUE,
                                                     DIGI_DIRECTION_RX_ONLY);
        }
    }

    if (TRUE == digi_resource_handle->force_release)
    {
        if (TRUE == is_lineotn)
        {
            digi_resource_otn_server_line_deprov(digi_handle,
                                                 (digi_otn_server_chnl_t**)&serdes_port_ptr->chnl_handle_ptr);
        }
        else
        {
            digi_resource_otn_server_sys_deprov(digi_handle,
                                                (digi_otn_server_chnl_t**)&serdes_port_ptr->chnl_handle_ptr);
        }
        digi_resource_reset_serdes_handle(serdes_port_ptr);
    }
    digi_resource_handle->force_release = FALSE;
    digi_resource_handle->datapath_release = FALSE;

    PMC_RETURN();
} /* digi_resource_release_lineotn */

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* digi_resource_is_mapper_operational
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Function determines whether the MAPPER channel for the particular
*  switch it valid, namely OPERATIONAL. If not then it resets its DB setting
*  and returns FALSE to the caller. Else returns true.
*  Needs to be called once for each switch and the caller determines whether
*  or not the operation was a success.  Namely if it is OPERATIONAl to a
*  switch but not to the other then that is consider acceptable.
*
* INPUTS:
*   digi_handle          - Reference to the digi_handle
*   digi_resource_handle - Reference to the resource table.
*   map_index            - Indice of the MAPOTN channel handle being 
*                          examined for the particular switch
*   switch_type          - Either CPB or ODUK switch and is the reference
*                          for the side of the switch that is initially 
*                          being examined. The switch_type side must be
*                          OPERATIONAL for this to be valid. The other side
*                          can be PROVISIONED and this be considered valid.
*   is_first             - Indicates whether this is the first call for this
*                          path or not. If MAPOTN is connected to both 
*                          switches then only the second call require it to
*                          check the operational state of the source/dest
*                          handles. Else if MAPOTN is connected to only one
*                          switch then it must check all associated 
*                          resources when called for that switch.
*                          Else if TRUE and connected to both switches then
*                          only the state of this MAPOTN resource need be
*                          verified to be OPERATIONAL.
*  direction             - RX/TX relative operational check.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   TRUE:  MAPOTN resource identified by map_index is OPERATIONAL.
*   FALSE: Otherwise
*
* NOTES:
*   Functin does not currently handle the case that source and destination
*   are different or that the caller is only sending data to MAPOTN but is
*   not receiving from it. Will require a lower level of granularity with
*   respect to checking as source and destination being different or 
*   multiply mapped will need to be accounted for.
*
*******************************************************************************/
PUBLIC BOOL8 digi_resource_is_mapper_operational(digi_handle_t *digi_handle,
                                                 digi_resource_handle_summary_t *digi_resource_handle,
                                                 UINT16 map_index,
                                                 digi_resource_next_handle_t switch_type,
                                                 BOOL8 is_first,
                                                 digi_direction_check_t direction)
{
    PMC_ERROR result = FALSE;
    digi_mapper_chnl_def_t *map_ptr;
    BOOL8 is_operational = FALSE;
    BOOL8 is_enet_provisioned = FALSE;
    BOOL8 tx_result = FALSE;
    BOOL8 rx_result = FALSE;

    PMC_ENTRY();

    /*
     * MAPOTN may not be connected at both switches. The caller calls for
     * each switch type. First check is to ensure that the switch index for
     * the specified MAPOTN channel is valid. 
     * Otherwise if not connected at the specified switch then return TRUE.
     * For the specified switch set is_first to FALSE, forcing it to
     * check all connected resources if this switch is connected but the
     * other is not.
     */
    if (0 == digi_resource_handle->mapper_cfg[map_index].oduk_switch_index &&
        0 == digi_resource_handle->mapper_cfg[map_index].cpb_switch_index)
    {
        PMC_LOG_TRACE("MAPOTN index=%d is not connected at either switch\n",
                      map_index);
        PMC_RETURN(FALSE);
    }
    if ((DIGI_ODUK_SWITCH == switch_type && 0 == digi_resource_handle->mapper_cfg[map_index].oduk_switch_index) ||
        (DIGI_CPB_SWITCH == switch_type && 0 == digi_resource_handle->mapper_cfg[map_index].cpb_switch_index))
    {
        PMC_RETURN(TRUE);
    }

    map_ptr = (digi_mapper_chnl_def_t *)digi_resource_handle->mapper_cfg[map_index].mapper_handle_ptr;
    /*
     * The truth table for the direction and the activation level is the
     * same for each switch and the only thing that differs is whether this
     * is the first time MAPOTN is being called where DC = Don't Care
     * First = TRUE and Direction = TX then State = DC Mapped = TRUE
     * First = TRUE and Direction = RX then State = ACTIVE Mapped = DC
     * First = FALSE and Direction = TX then State = ACTIVE Mapped = DC
     * First = FALSE and Direction = RX then State = DC Mapped = TRUE
     */
    if (NULL == map_ptr)
    {
        PMC_LOG_TRACE("MAPOTN MapIndex=%d is NULL\n", map_index);
        PMC_RETURN(FALSE);
    }

    if (TRUE != util_global_rx_or_tx_chnl_prov_status_get(map_ptr->switch_data.header.prov_state))
    {
        PMC_LOG_TRACE("MAPOTN MapIndex=%d is not PROVISIONED\n", map_index);
    }
    else if (((DIGI_DIRECTION_TX_ONLY == direction && FALSE == is_first) ||
              (DIGI_DIRECTION_RX_ONLY == direction && TRUE == is_first)) &&
             ((DIGI_CPB_SWITCH == switch_type &&
               UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE != map_ptr->switch_data.cpb_data.op_state.active_state) ||
               (DIGI_ODUK_SWITCH == switch_type &&
                UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE != map_ptr->switch_data.oduk_data.op_state.active_state)))
      {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "MAPOTN MapIndex=%d is not ACTIVE. Direction=%d, is_first=%d\n", map_index, direction, is_first);
    }
    else if (((DIGI_DIRECTION_TX_ONLY == direction && TRUE == is_first) ||
              (DIGI_DIRECTION_RX_ONLY == direction && FALSE == is_first)) &&
             ((DIGI_CPB_SWITCH == switch_type &&
               map_ptr->switch_data.cpb_data.op_state.map_state.input_map_count < 1) ||
              (DIGI_ODUK_SWITCH == switch_type &&
               map_ptr->switch_data.oduk_data.op_state.map_state.input_map_count < 1)))
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "MAPOTN MapIndex=%d is not MAPPED. Direction=%d, is_first=%d\n", map_index, direction, is_first);
    }
    else
    {
        util_global_map_adapt_func_t map_mode;
        util_global_map_adapt_func_t demap_mode;
        mapotn_src_dest_t mapotn_src;
        mapotn_src_dest_t mapotn_dest;

        digi_mapotn_mode_params_convert(map_ptr->mode,
                                        &map_mode,
                                        &demap_mode);
        
        digi_mapper_src_dest_get(digi_handle, 
                                 map_ptr->mode,
                                 map_ptr->enet_pmon, 
                                 &mapotn_src, 
                                 &mapotn_dest);

        if (DIGI_CPB_SWITCH == switch_type)
        {
            if (MAPOTN_DEST_CPB_ENET == mapotn_dest ||
                MAPOTN_DEST_ENET == mapotn_dest ||
                MAPOTN_DEST_ENET == mapotn_src)
            {
                is_enet_provisioned = TRUE;
            }
        }
        /*
         * The enet_provisioned flag is set to TRUE based on the MAPOTN
         * mode which would normalluy imply an ETrans scenario. However if
         * enet_pmon is enabled and this is a tansmuxponder board then the
         * flag will be set. Verify this by checking the port type is not
         * ENET_LINE.
         */
        if (TRUE == is_enet_provisioned && 
            (DIGI_PROD_APP_IS_TRANSMUXPONDER_CARD(digi_handle) ||
             DIGI60_PROD_APP_IS_TRANSMUXPONDER_CARD(digi_handle)||
             DIGI60_PROD_APP_IS_TRANSMUXPONDER_SYS_CLIENT_CARD(digi_handle))  &&
            TRUE == map_ptr->enet_pmon &&
            UTIL_GLOBAL_CPB_PORT_ENET_LINE != map_ptr->switch_data.cpb_data.port_type)
        {
            is_enet_provisioned = FALSE;
        }

        PMC_LOG_TRACE("Calling mapotn_demapper_prov_get for Channel=%d\n",
                      map_ptr->mapotn_chnl);
        /*
        if ((DIGI_DIRECTION_RX_ONLY == direction && TRUE == is_first) ||
            (DIGI_DIRECTION_TX_ONLY == direction && FALSE == is_first))
        */
        if ((DIGI_DIRECTION_RX_ONLY == direction && DIGI_ODUK_SWITCH == switch_type) ||
            (DIGI_DIRECTION_TX_ONLY == direction && DIGI_CPB_SWITCH == switch_type))
        {
            if (TRUE == is_first)
            {
                is_operational = mapotn_demapper_prov_get(digi_handle->mapotn_handle,
                                                          map_ptr->mapotn_chnl,
                                                          demap_mode);
            }
            else
            {
                is_operational = mapotn_mapper_prov_get(digi_handle->mapotn_handle,
                                                        map_ptr->mapotn_chnl, 
                                                        map_mode);
            }
        }
        else if ((DIGI_DIRECTION_RX_ONLY == direction && DIGI_CPB_SWITCH == switch_type) ||
                 (DIGI_DIRECTION_TX_ONLY == direction && DIGI_ODUK_SWITCH == switch_type))
        {
            if (TRUE == is_first)
            {
                is_operational = mapotn_mapper_prov_get(digi_handle->mapotn_handle,
                                                        map_ptr->mapotn_chnl, 
                                                        map_mode);

            }
            else
            {
                is_operational = mapotn_demapper_prov_get(digi_handle->mapotn_handle,
                                                          map_ptr->mapotn_chnl,
                                                          demap_mode);
            }
        }
    }
    if (FALSE == is_operational)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "mapotn_demapper_prov_get=FALSE, Direction=%d, Chnl=%d\n",
                direction, map_ptr->mapotn_chnl);
    }
    else if (TRUE == is_enet_provisioned && UTIL_GLOBAL_CPB_PORT_ENET_LINE != map_ptr->switch_data.cpb_data.port_type)
    {
        /*
         * Confirm that the ENET port and channel is OPERATIONAL.Both
         * sides need to be fully up.
         */
      PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
              "MAPPER with ENET but ENET not Set. Direction=%d, Chnl=%d\n",
              direction, map_ptr->mapotn_chnl);
    }
    else if (TRUE == is_enet_provisioned)
    {
        /*
         * Truth table for:
         * Dir=RX, is_first=TRUE then Master State = OPERATIONAL
         * Dir=TX, is_first=FALSE then Master State = OPERATIONAL
         * Dir=RX, is_first=FALSE then Slave State = OPERATIONAL
         * Dir=TX, is_first=TRUE then Slave State = OPERATIONAL
         */
        if (DIGI_DIRECTION_TX_RX == direction ||
            (DIGI_DIRECTION_RX_ONLY == direction && TRUE == is_first) ||
            (DIGI_DIRECTION_TX_ONLY == direction && FALSE == is_first))
        {
            if (UTIL_GLOBAL_CHNL_OPERATIONAL != g_cpb_connect_data_ptr->chnl[UTIL_GLOBAL_CPB_PORT_ENET_LINE][map_ptr->switch_data.cpb_data.channel].mst.state)
            {
                 PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                         "EnetLINE Channel=%d not fully operational\n",
                         map_ptr->switch_data.cpb_data.channel);
            }
            else if (FALSE == digi_enet_does_state_match(digi_handle,
                                                         map_ptr->switch_data.cpb_data.channel,
                                                         FALSE,
                                                         UTIL_GLOBAL_CPB_PORT_ENET_LINE,
                                                         UTIL_GLOBAL_CHNL_OPERATIONAL))
            {
                PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                        "EnetLINE Channel=%d TX not fully operational\n",
                        map_ptr->switch_data.cpb_data.channel);
            }
            else
            {
                result = TRUE;
            }
        }
        if (DIGI_DIRECTION_TX_RX == direction ||
            (DIGI_DIRECTION_RX_ONLY == direction && FALSE == is_first) ||
            (DIGI_DIRECTION_TX_ONLY == direction && TRUE == is_first))
        {
            if (UTIL_GLOBAL_CHNL_OPERATIONAL != g_cpb_connect_data_ptr->chnl[UTIL_GLOBAL_CPB_PORT_ENET_LINE][map_ptr->switch_data.cpb_data.channel].slv.state)
            {
                 PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                         "EnetLINE Channel=%d not fully operational\n",
                         map_ptr->switch_data.cpb_data.channel);
            }
            else if (FALSE == digi_enet_does_state_match(digi_handle,
                                                         map_ptr->switch_data.cpb_data.channel,
                                                         TRUE,
                                                         UTIL_GLOBAL_CPB_PORT_ENET_LINE,
                                                         UTIL_GLOBAL_CHNL_OPERATIONAL))
            {
                PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                        "EnetLINE Channel=%d RX not fully operational\n",
                        map_ptr->switch_data.cpb_data.channel);
            }
            else
            {    
              result = TRUE;
            }
        }
    }
    else if (DIGI_CPB_SWITCH == switch_type && 
             UTIL_GLOBAL_CPB_PORT_MAPOTN != map_ptr->switch_data.cpb_data.port_type)
    {
            /*
             * Run a sanity check here. If this is not enet_provisioned then
             * enet line is not being used on behalf of MAPOTN. Check that
             * the port type is valid. Note that the slv and dst state
             * of the connection array should not need to be checked as it
             * is used to set the source and destination active state.
             * In either case if this is not MAPOTN then that is a problem.
             */
            PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                    "ENET Provision=FALSE, PortType=%d is not MAPOTN\n", 
                    map_ptr->switch_data.cpb_data.port_type );
    }
    else if (DIGI_ODUK_SWITCH == switch_type)
    {
        UINT16 oduk_chnl = map_ptr->switch_data.oduk_data.channel;

        if (TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.s4_odu_struct[oduk_chnl].switch_data.header.prov_state))
        {
            UINT16 stg4_chnl = digi_handle->handle_pool.s4_odu_struct[oduk_chnl].switch_data.oduk_data.channel;
            
            if ((DIGI_DIRECTION_TX_RX == direction ||
                 DIGI_DIRECTION_TX_ONLY == direction) &&
                TRUE == coreotn_stg4_tx_chnl_prov_get(digi_handle->coreotn_handle,
                                                      stg4_chnl))
            {
                tx_result = TRUE;
            }
            if ((DIGI_DIRECTION_TX_RX == direction ||
                 DIGI_DIRECTION_RX_ONLY == direction) &&
                TRUE == coreotn_stg4_rx_chnl_prov_get(digi_handle->coreotn_handle,
                                                     stg4_chnl))
            {
                rx_result = TRUE;
            }

            /*
             * If both directions are being checked then both rx and tx must
             * be TRUE. Otherwise either one being TRUE works.
             */
            result = digi_resource_set_directional_result(direction,
                                                          tx_result,
                                                          rx_result);
        }
    }
    else
    {
        result = TRUE;
    }

    if (TRUE == result)
    {
        digi_switch_ctxt_t *switch_ptr = NULL;

        if (DIGI_ODUK_SWITCH == switch_type)
        {
            UINT16 switch_index = digi_resource_handle->mapper_cfg[map_index].oduk_switch_index;
            switch_ptr = &digi_resource_handle->oduksw_cfg[switch_index];
        }
        else
        {
            UINT16 switch_index = digi_resource_handle->mapper_cfg[map_index].cpb_switch_index;
            switch_ptr = &digi_resource_handle->cpbsw_cfg[switch_index];
        }
        /*
         * is_first determines whether the verify_switch call needs to
         * check beyond this resource or only this resource. If the flag
         * was true but there is not other resource defined then set
         */
        if (TRUE == is_first)
        {
            if ((DIGI_ODUK_SWITCH == switch_type && 0 == digi_resource_handle->mapper_cfg[map_index].cpb_switch_index)  ||
                (DIGI_CPB_SWITCH == switch_type && 0 == digi_resource_handle->mapper_cfg[map_index].oduk_switch_index))
            {
                is_first = FALSE;
            }
        }
        result = digi_resource_verify_switch_operational(digi_handle,
                                                         digi_resource_handle,
                                                         switch_ptr,
                                                         switch_type,
                                                         is_first,
                                                         DIGI_OTN_MAPPER,
                                                         map_index,
                                                         direction);
    }

    PMC_RETURN(result);
} /* digi_resource_is_mapper_operational */
#endif /* DOXYGEN_PUBLIC_ONLY */

/*******************************************************************************
* digi_resource_get_oduk_switch_index
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Takes the port type and channel and calculates the index into the ODUK
*  switch array based on the number of channels used by each ODUK DPI port.
*  Break down is:\n
*  PORT 0 (ILKN1)  : 0-103\n
*  PORT 1 (ILKN2)  : 104-207\n
*  PORT 2 (COREOTN): 208-303\n
*  PORT 3 (MAPOTN) : 304-399\n
*  The offset set used is the starting index for each PORT.
*
* INPUTS:
*   port_type      - One of the 4 DPI port types.
*   channel        - 0 relative channel used within the port_type range

* OUTPUTS:
*   *index         - Calculated index.
*
* RETURNS:
*   TRUE           - Port type is one of the 4 defined DPI ports for this
*                    switch
*   FALSE          - Otherwise, In this case the index parameter is unset
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 digi_resource_get_oduk_switch_index(util_global_oduk_port_t port_type,
                                                  UINT32 channel,
                                                  UINT16 *index)
{
    BOOL8 result = TRUE;

    PMC_ENTRY();
    /*
     * Switch index is assigned the offset based on the port type and 
     * then the channel is added to it to get the point in the switch
     * context array. Note that each channel is 0 relative to its port
     * and the offsets are calculated based on the channel capacity of each
     */    
    switch (port_type)
    {
      case UTIL_GLOBAL_ODUK_PORT_ILKN1:
        *index = DIGI_ODU_SWITCH_PORT0_OFFSET;
        break;
        
      case UTIL_GLOBAL_ODUK_PORT_ILKN2:
        *index = DIGI_ODU_SWITCH_PORT1_OFFSET;
        break;
        
      case UTIL_GLOBAL_ODUK_PORT_COREOTN:
        *index = DIGI_ODU_SWITCH_PORT2_OFFSET;
        break;
        
      case UTIL_GLOBAL_ODUK_PORT_MAPOTN:
        *index = DIGI_ODU_SWITCH_PORT3_OFFSET;
        break;
        
      default:
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "ODUK DATA PortType=%d  is not ILKN1/2, COREOTN or"
                " MAPOTN\n", port_type);
        result = FALSE;
        break;
    }

    if (TRUE == result)
    {
        *index += channel;
    }

    PMC_RETURN(result);
}  /* digi_resource_get_oduk_switch_index */

/*******************************************************************************
* digi_resource_get_cpb_switch_index
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Takes the port type and channel and calculates the index into the CPB
*  switch array based on the number of channels used by each CPB DPI port.
*  Break down is:\n
*  PORT 0 (ILKN1)     : 0-149\n
*  PORT 1 (ILKN2)     : 150-299\n
*  PORT 2 (CBRC)      : 300-311\n
*  PORT 3 (MAPOTN)    : 312-407\n
*  PORT 4 (ENET_LINE) : 408-419\n
*  PORT 5 (ENET_SYS)  : 420-431\n
*  The offset set used is the starting index for each PORT.
*
* INPUTS:
*   port_type      - One of the 6 DPI port types.
*   channel        - 0 relative channel used within the port_type range

* OUTPUTS:
*   *index         - Calculated index.
*
* RETURNS:
*   TRUE           - Port type is one of the 4 defined DPI ports for this
*                    switch
*   FALSE          - Otherwise, In this case the index parameter is unset
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 digi_resource_get_cpb_switch_index(util_global_cpb_port_t port_type,
                                   UINT32 channel,
                                   UINT16 *index)
{
    BOOL8 result = TRUE;

    PMC_ENTRY();
    /*
     * Switch index is assigned the offset based on the port type and 
     * then the channel is added to it to get the point in the switch
     * context array. Note that each channel is 0 relative to its port
     * and the offsets are calculated based on the channel capacity of each
     */    
    switch (port_type)
    {
      case UTIL_GLOBAL_CPB_PORT_ILKN1:
        *index = DIGI_CPB_SWITCH_PORT0_OFFSET;
        break;
        
      case UTIL_GLOBAL_CPB_PORT_ILKN2:
        *index = DIGI_CPB_SWITCH_PORT1_OFFSET;
        break;
        
      case UTIL_GLOBAL_CPB_PORT_CBRC:
        *index = DIGI_CPB_SWITCH_PORT2_OFFSET;
        break;
        
      case UTIL_GLOBAL_CPB_PORT_MAPOTN:
        *index = DIGI_CPB_SWITCH_PORT3_OFFSET;
        break;

      case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
        *index = DIGI_CPB_SWITCH_PORT4_OFFSET;
        break;

      case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
        *index = DIGI_CPB_SWITCH_PORT5_OFFSET;
        break;
        
      default:
        PMC_LOG_TRACE("CPB DATA PortType=%d  is not ILKN1/2, CBRC, MAPTOTN"
                      ", ENET_LINE or ENET_SYS\n", port_type);
        result = FALSE;
        break;
    }

    if (TRUE == result)
    {
        *index += channel;
    }

    PMC_RETURN(result);
} /* digi_resource_get_cpb_switch_index */


/*******************************************************************************
* digi_resource_set_source_oduk_switch
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Sets the source and destination handles associated with the specific ODUK
*  switch index table. Uses the CPB returned connection data structure to
*  construct and determin the primary destination handles and the source for
*  the current resource.
*
* INPUTS:
*   digi_handle    - Digi handle
*   switch_ptr     - Pointer to the switch related data for the subsystem
*                    being examined.
*   switch_index   - Index into the ODUK switch array for the subsystem.
*
* OUTPUTS:
*   *digi_resource_handle - Updated with the source related data structures
*                           at the switch_index and the destination.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_resource_set_source_oduk_switch(digi_handle_t *digi_handle,
                                                  util_global_switch_data_def_t *switch_ptr,
                                                  UINT16 switch_index,
                                                  digi_resource_handle_summary_t *digi_resource_handle)
{
    util_global_oduk_port_t prov_port_type;
    UINT32 prov_channel;


    PMC_ENTRY();
    /*
     * Use the switch_ptr channel value rather than the index. If this is
     * ILKN channel then it will be updated. If this is a COREOTN port then
     * it may be that the index used in the resource table and the channel
     * in the switch_ptr are not the same.
     */
    prov_channel = switch_ptr->oduk_data.channel;
    prov_port_type = digi_resource_get_oduk_port_type(digi_resource_handle->oduksw_cfg[switch_index].provisioned_handle_type, &prov_channel);


    /*
     * If the prov_port_type is COREOTN but this is a SYSOTN application
     * and the switch_data oduk port is ILKN1 then switch it to be ILKN1 as
     * the connectivity data in the ODUK connection array is relative to
     * ILKN1 and not COREOTN. This ensure that the correct source and
     * destination data is found.
     */
    if (TRUE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle) && 
        UTIL_GLOBAL_ODUK_PORT_COREOTN == prov_port_type &&
        UTIL_GLOBAL_ODUK_PORT_ILKN1 == switch_ptr->oduk_data.port_type)
    {
        prov_port_type = UTIL_GLOBAL_ODUK_PORT_ILKN1;
    }
    
    if (!(UTIL_GLOBAL_CHNL_EQUIPPED == g_oduk_connect_data_ptr->chnl[prov_port_type][prov_channel].slv.state ||
          UTIL_GLOBAL_CHNL_OPERATIONAL == g_oduk_connect_data_ptr->chnl[prov_port_type][prov_channel].slv.state))
      
    {
        PMC_LOG_TRACE("ProvPortType=%d and Channel=%d is not EQUIP or OPERATIONAL\n",
                      prov_port_type, prov_channel);
        digi_resource_handle->oduksw_cfg[switch_index].primary_dest_handle_type = LAST_DIGI_RESOURCE_HANDLE;
        digi_resource_handle->oduksw_cfg[switch_index].primary_dest_index = 0; 
        digi_resource_handle->oduksw_cfg[switch_index].num_dest = 0;
    }
    else
    {
        digi_resource_configure_switch_destination(digi_handle,
                                                   digi_resource_handle,
                                                   &digi_resource_handle->oduksw_cfg[switch_index],
                                                   DIGI_ODUK_SWITCH,
                                                   prov_port_type,
                                                   LAST_UTIL_GLOBAL_CPB_PORT,
                                                   prov_channel,
                                                   FALSE);
    }

    if (UTIL_GLOBAL_CHNL_START != g_oduk_connect_data_ptr->chnl[prov_port_type][prov_channel].mst.state)
    {
        util_global_oduk_port_t src_port_type = (util_global_oduk_port_t)g_oduk_connect_data_ptr->chnl[prov_port_type][prov_channel].mst.source_slv_port;
        UINT32 src_port_chnl = g_oduk_connect_data_ptr->chnl[prov_port_type][prov_channel].mst.source_slv_chnl;

        /*
         * The src_port_type and src_port_chnl is the source of the
         * provisined port type. Whether the provisioned port source is
         * activated is if the slave state is operational and the
         * destination port master is operational and its source is this
         * port.
         */
        if (UTIL_GLOBAL_CHNL_OPERATIONAL == g_oduk_connect_data_ptr->chnl[prov_port_type][prov_channel].slv.state && 
            CPB_INDEX_LAST != g_oduk_connect_data_ptr->chnl[prov_port_type][prov_channel].slv.dest_mst_port)
        {
            util_global_oduk_port_t dest_port = (util_global_oduk_port_t)g_oduk_connect_data_ptr->chnl[prov_port_type][prov_channel].slv.dest_mst_port;
            UINT32 dest_chnl =  g_oduk_connect_data_ptr->chnl[prov_port_type][prov_channel].slv.dest_mst_chnl;
            
            if (UTIL_GLOBAL_CHNL_OPERATIONAL == g_oduk_connect_data_ptr->chnl[dest_port][dest_chnl].mst.state &&
                prov_port_type == (util_global_oduk_port_t)g_oduk_connect_data_ptr->chnl[dest_port][dest_chnl].mst.source_slv_port &&
                prov_channel == g_oduk_connect_data_ptr->chnl[dest_port][dest_chnl].mst.source_slv_chnl)
            {
                digi_resource_handle->oduksw_cfg[switch_index].source_activated = TRUE;
            }
        }
        digi_resource_handle->oduksw_cfg[switch_index].source_handle_type = digi_resource_get_oduk_resource_type(digi_handle, digi_resource_handle, src_port_type, src_port_chnl, digi_resource_handle->oduksw_cfg[switch_index].provisioned_handle_type);

        if (LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->oduksw_cfg[switch_index].source_handle_type)
        {
            digi_resource_handle->oduksw_cfg[switch_index].source_handle_index = src_port_chnl;
        }
    }

    /*
     * Reset the ILKN 1 channels for source and destination if required.
     */
    if ((DIGI_ILKN_1 == digi_resource_handle->oduksw_cfg[switch_index].primary_dest_handle_type || DIGI_ILKN_2 == digi_resource_handle->oduksw_cfg[switch_index].primary_dest_handle_type) &&
        digi_resource_handle->oduksw_cfg[switch_index].primary_dest_index < DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX)
    {
        digi_resource_handle->oduksw_cfg[switch_index].primary_dest_index += DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
    }

    if ((DIGI_ILKN_1 == digi_resource_handle->oduksw_cfg[switch_index].source_handle_type || DIGI_ILKN_2 == digi_resource_handle->oduksw_cfg[switch_index].source_handle_type) &&
        digi_resource_handle->oduksw_cfg[switch_index].source_handle_index < DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX)
    {
        digi_resource_handle->oduksw_cfg[switch_index].source_handle_index += DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
    }

    PMC_RETURN();
}  /*  digi_resource_set_source_oduk_switch  */

/*******************************************************************************
* digi_resource_set_source_cpb_switch
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Determines the source of the stream to this subsystem
*
* INPUTS:
*   digi_handle    - digi to be operated on
*   switch_ptr     - Pointer to the subsystem being operated on. Used to 
*                    determine the source of data to this subsystem.
*   switch_index   - Indice into the ODUK or CPB switch structure associated
*                    with the output_ptr in the 
*                    digi_resource_handle_summary_t table.
*   enet_pmon      - Whether ENET PMON has been enabled or not.
*
* OUTPUTS:
*   *digi_resource_handle - Updated based on the source of data to the 
*                           output_ptr subsystem.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_resource_set_source_cpb_switch(digi_handle_t *digi_handle,
                                                util_global_switch_data_def_t *switch_ptr,
                                                UINT16 switch_index,
                                                BOOL8 enet_pmon,
                                                digi_resource_handle_summary_t *digi_resource_handle)
{
    util_global_cpb_port_t prov_port_type;
    UINT32 prov_channel;

    PMC_ENTRY();

    /*
     * This is likely overkill as the switch_ptr has the port type and all
     * that is need is to use the provisioned_handle_type to check if it is
     * MAPOTN.  This has been tested and verified to work. Will likely 
     * simplify this down later.
     */
    prov_port_type = digi_resource_get_cpb_port_type(digi_resource_handle, digi_resource_handle->cpbsw_cfg[switch_index].provisioned_handle_type, digi_resource_handle->cpbsw_cfg[switch_index].provisioned_handle_index);
    prov_channel = switch_ptr->cpb_data.channel;

    if (UTIL_GLOBAL_CPB_PORT_MAPOTN == prov_port_type &&
        UTIL_GLOBAL_CPB_PORT_ENET_LINE == switch_ptr->cpb_data.port_type)
    {
        prov_port_type = UTIL_GLOBAL_CPB_PORT_ENET_LINE;
    }
    else if (UTIL_GLOBAL_CPB_PORT_ENET_SYS == prov_port_type &&
             DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == digi_handle->var.prod_app)
    {
        prov_port_type = switch_ptr->cpb_data.port_type;
    }
    /*
     * If the current port/chnl is not equiped or operational then it is 
     * not transmitting a stream nor is it mapped to do so. Set the
     * destination side data to unset.
     */
    PMC_ASSERT(prov_port_type < UTIL_GLOBAL_CPB_PORT_NOT_USED, DIGI_ERR_INVALID_ARG, 0, 0);

    if (!(UTIL_GLOBAL_CHNL_EQUIPPED == g_cpb_connect_data_ptr->chnl[prov_port_type][prov_channel].slv.state ||
          UTIL_GLOBAL_CHNL_OPERATIONAL == g_cpb_connect_data_ptr->chnl[prov_port_type][prov_channel].slv.state ||
          (UTIL_GLOBAL_CHNL_BACKUP == g_cpb_connect_data_ptr->chnl[prov_port_type][prov_channel].slv.state && 
           prov_port_type != (UINT32)UTIL_GLOBAL_CPB_PORT_CBRC)))
      
    {
        digi_resource_handle->cpbsw_cfg[switch_index].primary_dest_handle_type = LAST_DIGI_RESOURCE_HANDLE;
        digi_resource_handle->cpbsw_cfg[switch_index].primary_dest_index = 0; 
        digi_resource_handle->cpbsw_cfg[switch_index].num_dest = 0;
    }
    else
    {
        digi_resource_configure_switch_destination(digi_handle,
                                                   digi_resource_handle,
                                                   &digi_resource_handle->cpbsw_cfg[switch_index],
                                                   DIGI_CPB_SWITCH,
                                                   LAST_UTIL_GLOBAL_ODUK_PORT,
                                                   prov_port_type,
                                                   prov_channel,
                                                   enet_pmon);
    }

    if (UTIL_GLOBAL_CHNL_START != g_cpb_connect_data_ptr->chnl[prov_port_type][prov_channel].mst.state)
    {
        util_global_cpb_port_t src_port_type = (util_global_cpb_port_t)g_cpb_connect_data_ptr->chnl[prov_port_type][prov_channel].mst.source_slv_port;
        UINT32 src_port_chnl = g_cpb_connect_data_ptr->chnl[prov_port_type][prov_channel].mst.source_slv_chnl;

        /*
         * The source side is activated this resource is transmitting to
         * another resource. If the slave side is operational then retrieve
         * the destination port and determine whether this resource is
         * that resource's source. If it is then the source is activated on
         * this resource
         */
        if ((UTIL_GLOBAL_CHNL_OPERATIONAL == g_cpb_connect_data_ptr->chnl[prov_port_type][prov_channel].slv.state ||
            (UTIL_GLOBAL_CHNL_BACKUP == g_cpb_connect_data_ptr->chnl[prov_port_type][prov_channel].slv.state && 
             (prov_port_type == UTIL_GLOBAL_CPB_PORT_ENET_LINE || prov_port_type == UTIL_GLOBAL_CPB_PORT_ENET_SYS)))&& 
            CPB_INDEX_LAST != g_cpb_connect_data_ptr->chnl[prov_port_type][prov_channel].slv.dest_mst_port)
        {
            util_global_cpb_port_t dest_port = (util_global_cpb_port_t)g_cpb_connect_data_ptr->chnl[prov_port_type][prov_channel].slv.dest_mst_port;
            UINT32 dest_chnl =  g_cpb_connect_data_ptr->chnl[prov_port_type][prov_channel].slv.dest_mst_chnl;
            
            if (UTIL_GLOBAL_CHNL_OPERATIONAL == g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.state &&
                prov_port_type == (util_global_cpb_port_t)g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.source_slv_port &&
                prov_channel == g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.source_slv_chnl)
            {
                digi_resource_handle->cpbsw_cfg[switch_index].source_activated = TRUE;
            }
        }
        digi_resource_handle->cpbsw_cfg[switch_index].source_handle_type = digi_resource_get_cpb_resource_type(digi_handle, digi_resource_handle, src_port_type, &src_port_chnl);

        if (LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->cpbsw_cfg[switch_index].source_handle_type)
        {
            digi_resource_handle->cpbsw_cfg[switch_index].source_handle_index = src_port_chnl;
        }
    }

    PMC_RETURN();
} /* digi_resource_set_source_cpb_switch */


/*******************************************************************************
* digi_resource_get_port_context
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Finds the port context pointer associated with the serdes port uid if 
*  there is one or returns an error.
*
* INPUTS:
*  digi_handle      - Reference to the digi handle being operated on
*  port_uid         - Serdes port identifier  
*
* OUTPUTS:
*  **port_ctxt_pptr - Port context pointer structure associated with the
*                     serdes port.
*
* RETURNS:
*  PMC_SUCCESS          - Operation was successful
*  DIGI_ERR_INVALID_ARG - Serdes port uid is unassigned or not serdes port
*                         id is not currently in use.
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_resource_get_port_context(digi_handle_t *digi_handle,
                                                UINT32 port_uid,
                                                digi_serdes_port_def_t **port_ctxt_pptr)
{
    PMC_ERROR result = DIGI_ERR_INVALID_ARG;

    PMC_ENTRY();

    *port_ctxt_pptr = NULL;

    if (port_uid != DIGI_SERDES_UNASSIGNED)
    {
        UINT32 i;

        for (i = 0; i < DIGI_NUM_LINE_PORTS_MAX; i ++) 
        {
            if (digi_handle->var.port_ctxt.line_port_ctxt[i].port_uid ==
                port_uid) 
            {
                *port_ctxt_pptr = &(digi_handle->var.port_ctxt.line_port_ctxt[i]);
                break;
            }
            else if (i < DIGI_NUM_SYS_PORTS_MAX &&
                     digi_handle->var.port_ctxt.sys_port_ctxt[i].port_uid == port_uid)
            {
                *port_ctxt_pptr = &(digi_handle->var.port_ctxt.sys_port_ctxt[i]);
                break;
            }
            else if (i < DIGI_NUM_SFI51_PORTS_MAX &&
                     digi_handle->var.port_ctxt.sfi51_port_ctxt[i].port_uid == port_uid)
            {
                *port_ctxt_pptr = &(digi_handle->var.port_ctxt.sfi51_port_ctxt[i]);
                break;
            }
        }
    }

    if (NULL != *port_ctxt_pptr)
    {
        result = PMC_SUCCESS;
    }

    PMC_RETURN(result);
} /*  digi_resource_get_port_context */


/*******************************************************************************
* digi_resource_populate_cpb_switch_data
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Populates the connection information for the chnl_hndl_ptr from the 
*  perspective of the CPB switch.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*    chnl_hndl_type        - type of subsystem handle associated with serdes port
*   *chnl_hndl_ptr         - subsystem channel handle associated with serdes port
*    mapotn_use            - when chnl_hndl_type DIGI_CHANNEL_HANDLE_ID_MAPPER_T,
*                            defines if channel handle non-MAPOTN or the ODUKSW
*                            or CPB sides of a MAPOTN channel handle
*
* OUTPUTS:
*   *source_ptr            - data source for subsystem data 
*   *companion_source_ptr  - Boolean indicating whether the source_ptr has
*                            multiple destinations.  If TRUE then this API
*                            will need to be called again with the 
*                            source_ptr->chnl_hndl_ptr as the chnl_hndl_ptr
*                            to get that resources destinations.
*   *primary_dest_ptr      - primary destination this subsystem transmitting
*                            into CPB
*   *multi_dest_ptr         - other destinations this subsystem transmitting
*                            into CPB
*
* RETURNS:
*  PMC_SUCCESS          - Operation was successful
*  DIGI_ERR_INVALID_ARG - Serdes port uid is unassigned or not serdes port
*                         id is not currently in use.
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_resource_populate_cpb_switch_data(digi_handle_t *digi_handle,
                                                        digi_handle_id_t chnl_hndl_type,
                                                        digi_chnl_hndl_t *chnl_hndl_ptr,
                                                        digi_mapotn_conn_use_t mapotn_use,
                                                        digi_chnl_hndl_conn_t *source_ptr,
                                                        BOOL8 *companion_source_ptr,
                                                        digi_chnl_hndl_conn_t *primary_dest_ptr,
                                                        digi_chnl_hndl_conn_t multi_dest_ptr[DIGI_RESOURCE_MULTI_DEST_MAX])
{
    PMC_ERROR result = PMC_SUCCESS;
    util_global_cpb_connect_t *cpb_connect_state_ptr = NULL;
    util_global_cpb_port_t port_type = LAST_UTIL_GLOBAL_CPB_PORT;
    UINT32 chnl_id = 0;

    PMC_ENTRY();

    switch (chnl_hndl_type)
    {
      case DIGI_CHANNEL_HANDLE_ID_ILKN_1_T:
        port_type = UTIL_GLOBAL_CPB_PORT_ILKN1;
        chnl_id = ((digi_ilkn_chnl_def_t *)chnl_hndl_ptr)->switch_data.cpb_data.channel;
        break;

      case DIGI_CHANNEL_HANDLE_ID_ILKN_2_T:
        port_type = UTIL_GLOBAL_CPB_PORT_ILKN2;
        chnl_id = ((digi_ilkn_chnl_def_t *)chnl_hndl_ptr)->switch_data.cpb_data.channel;
        break;

      case DIGI_CHANNEL_HANDLE_ID_MAPPER_T:
            chnl_id = ((digi_mapper_chnl_def_t *)chnl_hndl_ptr)->switch_data.cpb_data.channel;
            port_type = UTIL_GLOBAL_CPB_PORT_MAPOTN; 
            /*
             * Check if ENET LINE is being used and if so use that value 
             * instead.  The chnl_id should be the same.
             */
            if (UTIL_GLOBAL_CPB_PORT_ENET_LINE == ((digi_mapper_chnl_def_t *)chnl_hndl_ptr)->switch_data.cpb_data.port_type)
            {
                port_type = UTIL_GLOBAL_CPB_PORT_ENET_LINE;
            }
        break;

      case DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T:
        port_type = UTIL_GLOBAL_CPB_PORT_ENET_LINE;
        chnl_id = ((digi_enet_client_chnl_def_t *)chnl_hndl_ptr)->switch_data.cpb_data.channel;
        break;

      case DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T:
        port_type = UTIL_GLOBAL_CPB_PORT_ENET_SYS;
        chnl_id = ((digi_enet_client_chnl_def_t *)chnl_hndl_ptr)->switch_data.cpb_data.channel;
        break;

      case DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T:
        port_type =  UTIL_GLOBAL_CPB_PORT_CBRC;
        chnl_id = ((digi_cbr_client_chnl_def_t *)chnl_hndl_ptr)->switch_data.cpb_data.channel;
        break;

      default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    }
    cpb_connect_data_state_get(digi_handle->dcpb_handle,
                               &cpb_connect_state_ptr);
    PMC_MEMSET((void *)source_ptr, 0, sizeof(digi_chnl_hndl_conn_t));
    PMC_MEMSET((void *)primary_dest_ptr, 0, sizeof(digi_chnl_hndl_conn_t));
    source_ptr->chnl_hndl_type = LAST_DIGI_HANDLE_ID;
    primary_dest_ptr->chnl_hndl_type = LAST_DIGI_HANDLE_ID;
    *companion_source_ptr = FALSE;
    /*
     * Determine the state of the current port and channel.
     */
    if (CPB_INDEX_LAST != cpb_connect_state_ptr->chnl[port_type][chnl_id].mst.source_slv_port &&
        CPB_INDEX_LAST != cpb_connect_state_ptr->chnl[port_type][chnl_id].mst.source_slv_chnl)
    {
        digi_resource_convert_port_type(digi_handle,
                                        cpb_connect_state_ptr->chnl[port_type][chnl_id].mst.source_slv_port,
                                        cpb_connect_state_ptr->chnl[port_type][chnl_id].mst.source_slv_chnl,
                                        FALSE,
                                        FALSE,
                                        source_ptr);

        if (NULL != source_ptr->chnl_hndl_ptr)
        {
            UINT32 port = cpb_connect_state_ptr->chnl[port_type][chnl_id].mst.source_slv_port;
            UINT32 chnl = cpb_connect_state_ptr->chnl[port_type][chnl_id].mst.source_slv_chnl;

            if (cpb_connect_state_ptr->chnl[port][chnl].mst.is_slave_zone_primary)
            {
                source_ptr->chnl_primary = TRUE;
            }
            if (UTIL_GLOBAL_CHNL_OPERATIONAL == cpb_connect_state_ptr->chnl[port][chnl].mst.state)
            {
                source_ptr->chnl_activated = TRUE;
            }


            if (TRUE == cpb_connect_state_ptr->chnl[port][chnl].slv.is_broadcast ||
                TRUE == cpb_connect_state_ptr->chnl[port][chnl].slv.is_multicast)
            {
                /*
                 * The companion_source_ptr is true if this is a broadcast/
                 * multicast at the source ptr is sending to multiple 
                 * destinations. THUS AN EXTRA CHECK IS REQUIRED HERE TO
                 * ENSURE THAT IS THE CASE. NOTE that the flags being true
                 * may be sufficient
                 */
                *companion_source_ptr = TRUE;
            }
        }
        else
        {
            result = DIGI_ERR_NULL_HANDLE;
        }
    }

    if (PMC_SUCCESS == result &&
        UTIL_GLOBAL_CHNL_START != cpb_connect_state_ptr->chnl[port_type][chnl_id].slv.state)
    {
        BOOL8 is_multi_destinations = FALSE;

        if (TRUE == cpb_connect_state_ptr->chnl[port_type][chnl_id].slv.is_broadcast ||
            TRUE == cpb_connect_state_ptr->chnl[port_type][chnl_id].slv.is_multicast)
        {
            is_multi_destinations = TRUE;
        }
        (void)digi_resource_set_destination_pointer(digi_handle,
                                                    cpb_connect_state_ptr,
                                                    FALSE,
                                                    TRUE,
                                                    (UINT32 *)&port_type,
                                                    &chnl_id,
                                                    primary_dest_ptr);

        if (TRUE == is_multi_destinations)
        {
            UINT32 index = 0;

            while (1)
            {
                BOOL8 rc = digi_resource_set_destination_pointer(digi_handle,
                                                                 cpb_connect_state_ptr,
                                                                 FALSE,
                                                                 FALSE,
                                                                 (UINT32 *)&port_type,
                                                                 &chnl_id,
                                                                 &multi_dest_ptr[index]);
                /*
                 * Function returns false whenever it finds in invalid
                 * destination index. Break out of the loop when that
                 * occurs.
                 */
                if (FALSE == rc)
                {
                    break;
                }
                index++;
            }
        }
    }

    PMC_RETURN(result);

} /* digi_resource_populate_cpb_switch_data */


/*******************************************************************************
* digi_resource_populate_oduk_switch_data
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Populates the connection information for the chnl_hndl_ptr from the 
*  perspective of the ODUK switch.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*    chnl_hndl_type        - type of subsystem handle associated with serdes port
*   *chnl_hndl_ptr         - subsystem channel handle associated with serdes port
*    mapotn_use            - when chnl_hndl_type DIGI_CHANNEL_HANDLE_ID_MAPPER_T,
*                            defines if channel handle non-MAPOTN or the ODUKSW
*                            or CPB sides of a MAPOTN channel handle
*
* OUTPUTS:
*   *source_ptr            - data source for subsystem data 
*   *companion_source_ptr  - Boolean indicating whether the source_ptr has
*                            multiple destinations.  If TRUE then this API
*                            will need to be called again with the 
*                            source_ptr->chnl_hndl_ptr as the chnl_hndl_ptr
*                            to get that resources destinations.
*   *primary_dest_ptr      - primary destination this subsystem transmitting
*                            into ODUKSW
*   multi_dest_ptr         - other destinations this subsystem transmitting
*                            into ODUKSW
*
* RETURNS:
*  PMC_SUCCESS          - Operation was successful
*  DIGI_ERR_INVALID_ARG - Serdes port uid is unassigned or not serdes port
*                         id is not currently in use.
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_resource_populate_oduk_switch_data(digi_handle_t *digi_handle,
                                                         digi_handle_id_t chnl_hndl_type,
                                                         digi_chnl_hndl_t *chnl_hndl_ptr,
                                                         digi_mapotn_conn_use_t mapotn_use,
                                                         digi_chnl_hndl_conn_t *source_ptr,
                                                         BOOL8 *companion_source_ptr,
                                                         digi_chnl_hndl_conn_t *primary_dest_ptr,
                                                         digi_chnl_hndl_conn_t multi_dest_ptr[DIGI_RESOURCE_MULTI_DEST_MAX])
{
    PMC_ERROR result = PMC_SUCCESS;
    util_global_cpb_connect_t *oduk_connect_state_ptr = NULL;
    util_global_oduk_port_t port_type = LAST_UTIL_GLOBAL_ODUK_PORT;
    UINT32 chnl_id = 0;

    PMC_ENTRY();

    switch (chnl_hndl_type)
    {
      case DIGI_CHANNEL_HANDLE_ID_HO_ODU_T:
      case DIGI_CHANNEL_HANDLE_ID_MO_ODU_T:
      case DIGI_CHANNEL_HANDLE_ID_LO_ODU_T:
        port_type = UTIL_GLOBAL_ODUK_PORT_COREOTN;
        chnl_id = ((odu_struct_t *)chnl_hndl_ptr)->switch_data.oduk_data.channel;
        break;

      case DIGI_CHANNEL_HANDLE_ID_MAPPER_T:
        port_type = UTIL_GLOBAL_ODUK_PORT_MAPOTN;
        chnl_id = ((digi_mapper_chnl_def_t *)chnl_hndl_ptr)->switch_data.oduk_data.channel;
        break;

      case DIGI_CHANNEL_HANDLE_ID_ILKN_1_T:
        port_type = UTIL_GLOBAL_ODUK_PORT_ILKN1;
        chnl_id = ((digi_ilkn_chnl_def_t *)chnl_hndl_ptr)->switch_data.oduk_data.channel;

        if (chnl_id >= DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX)
        {
            chnl_id -= DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
        }
        break;

      case DIGI_CHANNEL_HANDLE_ID_ILKN_2_T:
        port_type = UTIL_GLOBAL_ODUK_PORT_ILKN2;
        chnl_id = ((digi_ilkn_chnl_def_t *)chnl_hndl_ptr)->switch_data.oduk_data.channel;

        if (chnl_id >= DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX)
        {
            chnl_id -= DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
        }

        break;

      default:
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }

    /*
     * If here then the handle should either be a OTN handle, or MAPOTN
     * with mapotn_use being ILKN or ILKN1/2 and the target is ODUK
     */
    coreotn_oduksw_connect_data_state_get(digi_handle->coreotn_handle,
                                          &oduk_connect_state_ptr);
    PMC_MEMSET((void *)source_ptr, 0, sizeof(digi_chnl_hndl_conn_t));
    PMC_MEMSET((void *)primary_dest_ptr, 0, sizeof(digi_chnl_hndl_conn_t));
    source_ptr->chnl_hndl_type = LAST_DIGI_HANDLE_ID;
    primary_dest_ptr->chnl_hndl_type = LAST_DIGI_HANDLE_ID;
    *companion_source_ptr = FALSE;

    if (CPB_INDEX_LAST != oduk_connect_state_ptr->chnl[port_type][chnl_id].mst.source_slv_port &&
        CPB_INDEX_LAST != oduk_connect_state_ptr->chnl[port_type][chnl_id].mst.source_slv_chnl)
    {
        digi_resource_convert_port_type(digi_handle,
                                        oduk_connect_state_ptr->chnl[port_type][chnl_id].mst.source_slv_port,
                                        oduk_connect_state_ptr->chnl[port_type][chnl_id].mst.source_slv_chnl,
                                        TRUE,
                                        FALSE,
                                        source_ptr);

        if (NULL != source_ptr->chnl_hndl_ptr)
        {
            UINT32 port = oduk_connect_state_ptr->chnl[port_type][chnl_id].mst.source_slv_port;
            UINT32 chnl =  oduk_connect_state_ptr->chnl[port_type][chnl_id].mst.source_slv_chnl;

            if (oduk_connect_state_ptr->chnl[port][chnl].mst.is_slave_zone_primary)
            {
                source_ptr->chnl_primary = TRUE;
            }
            if (UTIL_GLOBAL_CHNL_OPERATIONAL == oduk_connect_state_ptr->chnl[port][chnl].mst.state)
            {
                source_ptr->chnl_activated = TRUE;
            }


            if (TRUE == oduk_connect_state_ptr->chnl[port][chnl].slv.is_broadcast ||
                TRUE == oduk_connect_state_ptr->chnl[port][chnl].slv.is_multicast)
            {
                *companion_source_ptr = TRUE;
            }
        }
        else
        {
            result = DIGI_ERR_NULL_HANDLE;
        }
    }

    if (PMC_SUCCESS == result &&
        UTIL_GLOBAL_CHNL_START != oduk_connect_state_ptr->chnl[port_type][chnl_id].slv.state)
    {
        BOOL8 is_multi_destinations = FALSE;

        if (TRUE == oduk_connect_state_ptr->chnl[port_type][chnl_id].slv.is_broadcast ||
            TRUE == oduk_connect_state_ptr->chnl[port_type][chnl_id].slv.is_multicast)
        {
            is_multi_destinations = TRUE;
        }
        (void)digi_resource_set_destination_pointer(digi_handle,
                                                    oduk_connect_state_ptr,
                                                    TRUE,
                                                    TRUE,
                                                    (UINT32 *)&port_type,
                                                    &chnl_id,
                                                    primary_dest_ptr);

        if (TRUE == is_multi_destinations)
        {
            UINT32 index = 0;
 
            while (1)
            {
                BOOL8 rc = digi_resource_set_destination_pointer(digi_handle,
                                                                 oduk_connect_state_ptr,
                                                                 TRUE,
                                                                 FALSE,
                                                                 (UINT32 *)&port_type,
                                                                 &chnl_id,
                                                                 &multi_dest_ptr[index]);
                /*
                 * Function returns false whenever it finds in invalid
                 * destination index. Break out of the loop when that
                 * occurs.
                 * 
                 */
                if (FALSE == rc)
                {
                    break;
                }
                index++;
            }
        }
    }

    PMC_RETURN(result);

}  /* digi_resource_populate_oduk_switch_data */

/*******************************************************************************
* digi_resource_get_serdes_channel_handle
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle          - Reference to the digi handle
*  port_ctxt_ptr        - Reference to the serdes port handle
*  serdes_prov_state    - The serdes provision state.
*
* OUTPUTS:
*  chnl_hndl_type_ptr   - Reference to the channel handle type the serdes 
*                         port is connected to.
*  chnl_hndl_pptr       - Channel handle that is connected to the serdes
*                         port.
*
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_resource_get_serdes_channel_handle(digi_handle_t *digi_handle,
                                                         digi_serdes_port_def_t *port_ctxt_ptr,
                                                         digi_serdes_prov_state_t serdes_prov_state,
                                                         digi_handle_id_t *chnl_hndl_type_ptr,
                                                         digi_chnl_hndl_t **chnl_hndl_pptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    digi_post_mld_dsi_ctxt_t *post_mld_dsi_ptr = NULL;
    UINT32 i;

    PMC_ENTRY();
    *chnl_hndl_pptr = NULL;

    switch (serdes_prov_state)
    {
      case DIGI_SERDES_PROV_STATE_SYSOTN:
        if (DIGI_SERDES_SYSTEM_INTF == port_ctxt_ptr->intf &&
            digi_handle->handle_pool.sysotn_server_chnl.header.prov_state != 
            UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG &&
            digi_handle->handle_pool.sysotn_server_chnl.port_uid == 
            port_ctxt_ptr->port_uid)
        {
            *chnl_hndl_type_ptr = DIGI_CHANNEL_HANDLE_ID_SYSOTN_T;
            *chnl_hndl_pptr = &(digi_handle->handle_pool.sysotn_server_chnl);
        }
        else
        {
            result = DIGI_ERR_SERDES_PORT_NOT_PROV;
        }
        break;

      case DIGI_SERDES_PROV_STATE_LINEOTN:
        for (i=0; i < DIGI_SERDES_DSI_LANES_MAX; i++)
        {
            if (digi_handle->handle_pool.otn_server_chnl[i].port_uid ==
                port_ctxt_ptr->port_uid)
            {
                if (digi_handle->handle_pool.otn_server_chnl[i].header.prov_state != 
                    UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG)
                {
                    *chnl_hndl_type_ptr = DIGI_CHANNEL_HANDLE_ID_LINEOTN_T;
                    *chnl_hndl_pptr = &(digi_handle->handle_pool.otn_server_chnl[i]);
                }
                else
                {
                    result = DIGI_ERR_SERDES_PORT_NOT_PROV;
                }
                break;
            }
        }
        break;

      case DIGI_SERDES_PROV_STATE_ENET_LINE:
      case DIGI_SERDES_PROV_STATE_CBRC:
      case DIGI_SERDES_PROV_STATE_ENET_SYS:
         post_mld_dsi_ptr = digi_first_post_mld_dsi_find(digi_handle, 
                                                         port_ctxt_ptr->port_uid);
         if (NULL != post_mld_dsi_ptr)
         {
             if (DIGI_SERDES_SYSTEM_INTF == port_ctxt_ptr->intf &&
                 DIGI_SERDES_PROV_STATE_ENET_SYS == post_mld_dsi_ptr->prov_state)
             {
                 *chnl_hndl_type_ptr = DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T;
                 *chnl_hndl_pptr = &(digi_handle->handle_pool.enet_sys_chnl[post_mld_dsi_ptr->lane_index]);
             }
             else if (DIGI_SERDES_SYSTEM_INTF != port_ctxt_ptr->intf &&
                      DIGI_SERDES_PROV_STATE_ENET_LINE ==
                      post_mld_dsi_ptr->prov_state)
             {
                 *chnl_hndl_type_ptr = DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T;
                 *chnl_hndl_pptr = &(digi_handle->handle_pool.enet_line_chnl[post_mld_dsi_ptr->lane_index]);
             }
             else if (DIGI_SERDES_PROV_STATE_CBRC == 
                      post_mld_dsi_ptr->prov_state)
             {
                 *chnl_hndl_type_ptr = DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T;
                 *chnl_hndl_pptr = NULL;
                 /*
                  * Iterate over the channel handles looking for a match
                  * on the port_uid. Cannot rely on the dsi lane index for
                  * this case.
                  */
                 for (i = 0; i < DIGI_CBRC_CHNL_MAX; i++)
                 {
                     if (digi_handle->handle_pool.cbr_chnl[i].port_uid ==
                         port_ctxt_ptr->port_uid)
                     {
                         *chnl_hndl_pptr = &(digi_handle->handle_pool.cbr_chnl[i]);
                         break;
                     }
                 }

                 if (NULL == *chnl_hndl_pptr)
                 {
                     result = DIGI_ERR_SERDES_PORT_NOT_PROV;
                 }
             }
         }
         else
         {
             result = DIGI_ERR_SERDES_PORT_NOT_PROV;
         }
         break;

      default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    }

    PMC_RETURN(result);
}  /* digi_resource_get_serdes_channel_handle */


/*******************************************************************************
* digi_resource_internal_serdes_port_release
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle          - Reference to the digi handle
*  chnl_hndl_type       - Channel handle type being released.
*  chnl_hndl_ptr        - Reference to the channel handle being released.
*  switch_release       - Whether to release the switch resources only
*  mapotn_release       - Whether to release mapotn if found.
*
* OUTPUTS:
*
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_resource_internal_serdes_port_release(digi_handle_t *digi_handle,
                                                             digi_handle_id_t chnl_hndl_type,
                                                             digi_chnl_hndl_t *chnl_hndl_ptr,
                                                             BOOL8 switch_release,
                                                             BOOL8 mapotn_release)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    util_global_cpb_port_t cpb_port;
    digi_otn_odu_chnl_t *ho_chnl_ptr = NULL;
    UINT32 chnl_id;
    util_global_cpb_connect_t *oduk_connect_state_ptr = NULL;
    util_global_cpb_connect_t *cpb_connect_state_ptr = NULL;

    PMC_ENTRY();

    switch (chnl_hndl_type)
    {
      case DIGI_CHANNEL_HANDLE_ID_LINEOTN_T:
      case DIGI_CHANNEL_HANDLE_ID_SYSOTN_T:
        result =  digi_otn_server_ho_prov_chnl_get(digi_handle,
                                                   (digi_otn_server_chnl_t *)chnl_hndl_ptr,
                                                   &ho_chnl_ptr);
        if (PMC_SUCCESS == result && NULL != ho_chnl_ptr)
        {
            coreotn_oduksw_connect_data_state_get(digi_handle->coreotn_handle,
                                                  &oduk_connect_state_ptr);
            result = digi_resource_release_subcontainers(digi_handle,
                                                         ho_chnl_ptr,
                                                         DIGI_CHANNEL_HANDLE_ID_HO_ODU_T,
                                                         switch_release,
                                                         mapotn_release,
                                                         oduk_connect_state_ptr);

            if (PMC_SUCCESS == result && FALSE == switch_release)
              {
                result = digi_otn_server_ho_deprov(digi_handle,
                                                   &ho_chnl_ptr,
                                                   (digi_otn_server_chnl_t**)&chnl_hndl_ptr);
              }
        }
        else if (DIGI_ERR_INCORRECT_ODU_PROV_MODE == result)
        {
            /*
             * If digi_otn_server_ho_prov_chnl_get returns the above result
             * then the OTN handle is valid and has no ODU handles hanging
             * off of it. Need to clean up the associated handle and
             * reset the return code to success.
             */
            result = PMC_SUCCESS;
        }
 
        if (PMC_SUCCESS == result && FALSE == switch_release)
        {
            if (DIGI_CHANNEL_HANDLE_ID_LINEOTN_T == chnl_hndl_type)
            {
                result = digi_otn_server_line_deprov(digi_handle,
                                                     (digi_otn_server_chnl_t**)&chnl_hndl_ptr);
            }
            else
            {
                result = digi_otn_server_sys_deprov(digi_handle,
                                                    (digi_otn_server_chnl_t**)&chnl_hndl_ptr);
            }
            
        }
        break;

      case DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T:
      case DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T:
      case DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T:
        cpb_connect_data_state_get(digi_handle->dcpb_handle,
                                   &cpb_connect_state_ptr);

        if (DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T == chnl_hndl_type)
        {
            digi_cbr_client_chnl_def_t *cbr_ptr = (digi_cbr_client_chnl_def_t *)chnl_hndl_ptr;
            chnl_id = cbr_ptr->switch_data.cpb_data.channel;
            cpb_port = UTIL_GLOBAL_CPB_PORT_CBRC;
        }
        else if (DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T == chnl_hndl_type)
        {
            digi_enet_client_chnl_def_t *enet_ptr = (digi_enet_client_chnl_def_t *)chnl_hndl_ptr;
            chnl_id = enet_ptr->switch_data.cpb_data.channel;
            cpb_port = UTIL_GLOBAL_CPB_PORT_ENET_LINE;
        }
        else
        {
            digi_enet_client_chnl_def_t *enet_ptr = (digi_enet_client_chnl_def_t *)chnl_hndl_ptr;
            chnl_id = enet_ptr->switch_data.cpb_data.channel;
            cpb_port = UTIL_GLOBAL_CPB_PORT_ENET_SYS;
        }
        /*
         * At this point need to pass the chnl id and port type to a 
         * function to release CPB related resources.
         */
        result = digi_resource_release_cpb_resource(digi_handle,
                                                    chnl_hndl_ptr,
                                                    cpb_port,
                                                    chnl_id,
                                                    switch_release,
                                                    mapotn_release,
                                                    cpb_connect_state_ptr);

        if (PMC_SUCCESS == result && FALSE == switch_release)
        {
            if (DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T == chnl_hndl_type)
            {
                result = digi_enet_line_deprov(digi_handle, &chnl_hndl_ptr);
            }
            else if (DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T == chnl_hndl_type)
            {
                result = digi_enet_sys_deprov(digi_handle, &chnl_hndl_ptr);
            }
            else
            {
                result = digi_cbr_client_deprov(digi_handle, &chnl_hndl_ptr);
            }
        }
        break;


      default:
        result = DIGI_ERR_INVALID_ARG;
        break;
        
    }

    PMC_RETURN(result);
} /* digi_resource_internal_serdes_port_release */


/*******************************************************************************
* digi_resource_release_oduk_resource
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle          - Reference to the digi handle
*  chnl_ctxt_ptr        - Reference to the channel handle connected to the
*                         ODUK switch.
*  oduk_port            - Port type of the channel handle
*  oduk_chnl            - Channel identifier.
*  switch_release       - Whether to decouple only the switch related 
*                         connections and leave the resource (TRUE)
*  mapotn_release       - Flag if TRUE will release MAPOTN resource if the
*                         channel handle is connected to a MAPOTN resource
*  oduk_connect_state_ptr - Reference to the connection array representing
*                           the ODUK switch connections.
*
* OUTPUTS:
*
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_resource_release_oduk_resource(digi_handle_t *digi_handle,
                                                     digi_chnl_hndl_t *chnl_ctxt_ptr,
                                                     util_global_oduk_port_t oduk_port,
                                                     UINT32 oduk_chnl,
                                                     BOOL8 switch_release,
                                                     BOOL8 mapotn_release,
                                                     util_global_cpb_connect_t *oduk_connect_state_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    digi_chnl_hndl_conn_t source_hndl = { LAST_DIGI_HANDLE_ID, NULL, FALSE, FALSE };

    PMC_ENTRY();

    if (UTIL_GLOBAL_CHNL_START != oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].slv.state && (FALSE == oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].slv.is_broadcast && FALSE == oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].slv.is_multicast))
    {
        UINT32 dest_port = oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].slv.dest_mst_port;
        UINT32 dest_chnl = oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].slv.dest_mst_chnl;
        /*
         * Set the source of the signal for this oduk port.
         */
        if (CPB_INDEX_LAST != dest_port && 
            CPB_INDEX_LAST != dest_chnl)
        {
            digi_resource_convert_port_type(digi_handle, dest_port,
                                            dest_chnl, TRUE,
                                            FALSE,
                                            &source_hndl);
        }

        if (CPB_INDEX_LAST != dest_port && CPB_INDEX_LAST != dest_chnl &&
            UTIL_GLOBAL_CHNL_OPERATIONAL == oduk_connect_state_ptr->chnl[dest_port][dest_chnl].mst.state)
          {
            result = digi_oduksw_chnl_deactivate(digi_handle,
                                                 source_hndl.chnl_hndl_ptr);
          }
        
        if (PMC_SUCCESS == result &&
            UTIL_GLOBAL_CHNL_OPERATIONAL == oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].mst.state && source_hndl.chnl_hndl_ptr != chnl_ctxt_ptr)
        {
            result = digi_oduksw_chnl_deactivate(digi_handle,
                                                 chnl_ctxt_ptr);
        }
        
        if (PMC_SUCCESS == result && CPB_INDEX_LAST != dest_port && 
            CPB_INDEX_LAST != dest_chnl &&
            UTIL_GLOBAL_CHNL_START != oduk_connect_state_ptr->chnl[dest_port][dest_chnl].slv.state)
        {
            /*
             * If this is a MAPOTN channel then it may be connected at the
             * CPB switch. Call cpb_activate for the MAPOTN channel and 
             * ignore the return code as it is not clear that it actually
             * connected.
             */
            if (DIGI_CHANNEL_HANDLE_ID_MAPPER_T == source_hndl.chnl_hndl_type)
            {
                result = digi_resource_release_mapotn_resources(digi_handle,
                                                                source_hndl.chnl_hndl_ptr,
                                                                mapotn_release,
                                                                TRUE);
            }
            else 
            {
                result = digi_oduksw_chnl_demap(digi_handle,
                                                source_hndl.chnl_hndl_ptr);
            }
        }
        
        if (PMC_SUCCESS == result &&
            UTIL_GLOBAL_CHNL_START != oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].mst.state && source_hndl.chnl_hndl_ptr != chnl_ctxt_ptr)
        {
            result = digi_oduksw_chnl_demap(digi_handle,
                                            chnl_ctxt_ptr);
        }

        if (PMC_SUCCESS == result && 
            DIGI_CHANNEL_HANDLE_ID_MAPPER_T == source_hndl.chnl_hndl_type && 
            TRUE == mapotn_release)
        {
            result = digi_otn_mapper_deprov(digi_handle, &source_hndl.chnl_hndl_ptr);
        }
    }
    else if (UTIL_GLOBAL_CHNL_START != oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].slv.state && (TRUE == oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].slv.is_broadcast || TRUE == oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].slv.is_multicast))
    {
        result = digi_resource_release_oduk_broadcast(digi_handle,
                                                      oduk_port,
                                                      oduk_chnl,
                                                      switch_release,
                                                      mapotn_release,
                                                      oduk_connect_state_ptr);
    }
    PMC_RETURN(result);
}  /* digi_resource_release_oduk_resource */


/*******************************************************************************
* digi_resource_release_cpb_resources
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle          - Reference to the digi handle
*  cpb_chnl_hndl_ptr    - Reference to the channel handle connected to the
*                         CPB switch.
*  cpb_port             - Type of port connected to the switch.
*  cpb_chnl             - Channel identifer
*  switch_release       - Flag indicating whether the cpb_hndl_ptr should be
*                         deactivated/demapped/deproved (FALSE) or whether
*                         to simply disconnect the resources at the switch.
*  mapotn_release       - If TRUE and cpb_chnl_hndl_ptr is connected to
*                         MAPOTN then MAPOTN is also releeased
*  cpb_connect_state_ptr - Array of connections around the cpb switch
*
* OUTPUTS:
*
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_resource_release_cpb_resource(digi_handle_t *digi_handle,
                                                    digi_chnl_hndl_t *cpb_chnl_hndl_ptr,
                                                    util_global_cpb_port_t cpb_port,
                                                    UINT32 cpb_chnl,
                                                    BOOL8 switch_release,
                                                    BOOL8 mapotn_release,
                                                    util_global_cpb_connect_t *cpb_connect_state_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    digi_chnl_hndl_conn_t source_hndl = { LAST_DIGI_HANDLE_ID, NULL, FALSE, FALSE };

    PMC_ENTRY();

    if (UTIL_GLOBAL_CHNL_START != cpb_connect_state_ptr->chnl[cpb_port][cpb_chnl].slv.state)
    {
        UINT32 dest_port = cpb_connect_state_ptr->chnl[cpb_port][cpb_chnl].slv.dest_mst_port;
        UINT32 dest_chnl = cpb_connect_state_ptr->chnl[cpb_port][cpb_chnl].slv.dest_mst_chnl;

        if (CPB_INDEX_LAST != dest_port && 
            CPB_INDEX_LAST != dest_chnl &&
            UTIL_GLOBAL_CHNL_START != cpb_connect_state_ptr->chnl[cpb_port][cpb_chnl].slv.state)
        {
            digi_resource_convert_port_type(digi_handle, dest_port,
                                            dest_chnl, FALSE,
                                            TRUE,
                                            &source_hndl);
        }

        if (CPB_INDEX_LAST != dest_port && CPB_INDEX_LAST != dest_chnl &&
            UTIL_GLOBAL_CHNL_OPERATIONAL == cpb_connect_state_ptr->chnl[dest_port][dest_chnl].mst.state)
        {
            result = digi_cpb_chnl_deactivate(digi_handle,
                                              source_hndl.chnl_hndl_ptr);
        }

        if (PMC_SUCCESS == result &&
            UTIL_GLOBAL_CHNL_OPERATIONAL == cpb_connect_state_ptr->chnl[cpb_port][cpb_chnl].mst.state && source_hndl.chnl_hndl_ptr != cpb_chnl_hndl_ptr)
        {
            result = digi_cpb_chnl_deactivate(digi_handle,
                                              cpb_chnl_hndl_ptr);
        }

        if (PMC_SUCCESS == result && CPB_INDEX_LAST != dest_port && 
            CPB_INDEX_LAST != dest_chnl &&
            UTIL_GLOBAL_CHNL_START != cpb_connect_state_ptr->chnl[dest_port][dest_chnl].slv.state)
        {
            /*
             * If this is a MAPOTN channel then it may be connected at the
             * CPB switch. Call cpb_activate for the MAPOTN channel and 
             * ignore the return code as it is not clear that it actually
             * connected.
             */
            if (DIGI_CHANNEL_HANDLE_ID_MAPPER_T == source_hndl.chnl_hndl_type)
            {
                result = digi_resource_release_mapotn_resources(digi_handle,
                                                                source_hndl.chnl_hndl_ptr,
                                                                mapotn_release,
                                                                FALSE);
            }
            else
            {
                result = digi_cpb_chnl_demap(digi_handle,
                                             source_hndl.chnl_hndl_ptr);
            }
        }

        if (PMC_SUCCESS == result &&
            UTIL_GLOBAL_CHNL_START != cpb_connect_state_ptr->chnl[cpb_port][cpb_chnl].mst.state && source_hndl.chnl_hndl_ptr != cpb_chnl_hndl_ptr)
        {
            result = digi_cpb_chnl_demap(digi_handle,
                                         cpb_chnl_hndl_ptr);
        }

        if (PMC_SUCCESS == result && 
            DIGI_CHANNEL_HANDLE_ID_MAPPER_T == source_hndl.chnl_hndl_type && 
            TRUE == mapotn_release)
        {
            result = digi_otn_mapper_deprov(digi_handle, &source_hndl.chnl_hndl_ptr);
        }
    }

    PMC_RETURN(result);
} /* digi_resource_release_cpb_resource */


/*******************************************************************************
* digi_resource_lo_channel_get_by_trib_slot
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle          - Reference to the digi handle
*  num_cal_entries      - Number of calendar entries in the ho_cal array.
*  ho_cal_array         - Array of calendar entries
*  ho_chnl_hndl_type    - High order channel handle type. Either:
*                         DIGI_CHANNEL_HANDLE_ID_HO_ODU_T or
*                         DIGI_CHANNEL_HANDLE_ID_MO_ODU_T
*  tx_trib_slot         - Tx tributary slot mask
*  rx_trib_slot         - Rx Tributary mask
*
* OUTPUTS:
*  slot_use_ptr         - Reference to the type of low order channels 
*                         returned based on the tributary mask matching.
*  lo_chnl_hndl_ptr     - Array of lo order channels that match.
*
*
* RETURNS:
*  PMC_SUCCESS - Regardless of the outcome returns this value.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR  digi_resource_lo_channel_get_by_trib_slot(digi_handle_t *digi_handle,
                                                            UINT32 num_cal_entries,
                                                            UINT32 ho_cal_array[DIGI_OTN_SERVER_MO_CHNL_MAX],
                                                            digi_handle_id_t ho_chnl_hndl_type,
                                                            UINT32 tx_trib_slot[3],
                                                            UINT32 rx_trib_slot[3],
                                                            digi_otn_time_slot_occupied_t *slot_use_ptr,
                                                            digi_otn_odu_chnl_t *lo_chnl_hndl_ptr[DIGI_OTN_SERVER_MAX_TRIBSLOT])
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    odu_struct_t *lo_odu_struct_ptr = NULL;
    UINT32 lo_index = 0;

    PMC_ENTRY();

    /*
     * Reset the array pointers to NULL
     */
    for (i = 0; i < DIGI_OTN_SERVER_MAX_TRIBSLOT; i++)
    {
        lo_chnl_hndl_ptr[i] = NULL;
    }
    /*
     * Set the slot array to all free so if there are no matches then
     * there is nothing to set later.
     */
    *slot_use_ptr = DIGI_OTN_TIME_SLOT_OCCUPIED_ALL_FREE;
    
    /*
     * Iterate over the calendar entries based on whether the channel
     * handle is active or not.  Note tat once there is a MIXED_USE
     * case there is nothing else to check as nothing else can be
     * set so time to stop checking.
     */
    for (i = 0; i < num_cal_entries; i++)
    {
        UINT32 lo_chnl_id = ho_cal_array[i];
        
        if (DIGI_CHANNEL_HANDLE_ID_HO_ODU_T == ho_chnl_hndl_type)
        {
            lo_odu_struct_ptr =  &(digi_handle->handle_pool.mo_odu_struct[lo_chnl_id]);
        }
        else
        {
            lo_odu_struct_ptr =  &(digi_handle->handle_pool.lo_odu_struct[lo_chnl_id]);
        }
        
        if (NULL != lo_odu_struct_ptr && 
            TRUE == lo_odu_struct_ptr->active)
        {
            UINT32 k;
            BOOL8 match_found = FALSE;
            BOOL8 exact_match = TRUE;

            for (k=0; k < 3; k++)
            {
                UINT32 j;
                UINT32 tx_match = 0;
                UINT32 rx_match = 0;
                
                for (j=0; j < DIGI_WORD_BIT_SIZE;j++)
                {
                    UINT32 local = lo_odu_struct_ptr->mem_ptr->tx_trib_loc_mask[k] >> j;
                    UINT32 param = tx_trib_slot[k] >> j;

                    if ((local & 0x01) && (param & 0x01))
                    {
                        tx_match++;
                    }
                    if (local != param)
                    {
                        exact_match = FALSE;
                    }
                    local = lo_odu_struct_ptr->mem_ptr->rx_trib_loc_mask[k] >> j;
                    param = rx_trib_slot[k] >> j;
                    
                    if ((local & 0x01) && (param & 0x01))
                    {
                        rx_match++;
                    }
                    if (local != param)
                    {
                        exact_match = FALSE;
                    }
                }
                
                if (tx_match > 0 || rx_match > 0)
                {
                    if (FALSE == match_found)
                    {
                        lo_chnl_hndl_ptr[lo_index++] = (digi_otn_odu_chnl_t *)lo_odu_struct_ptr;
                        match_found = TRUE;
                    }
                    if (DIGI_OTN_TIME_SLOT_OCCUPIED_ALL_FREE == *slot_use_ptr)
                    {
                        if (TRUE == exact_match)
                        {
                            *slot_use_ptr = DIGI_OTN_TIME_SLOT_OCCUPIED_ONE_CHANNEL;
                        }
                        else
                        {
                            *slot_use_ptr = DIGI_OTN_TIME_SLOT_OCCUPIED_MIXED_USE;
                        }
                    }
                    else
                    {
                        *slot_use_ptr = DIGI_OTN_TIME_SLOT_OCCUPIED_MIXED_USE;
                    }
                }
            }
        }
    }


    PMC_RETURN(result);
} /* digi_resource_lo_channel_get_by_trib_slot */



/*******************************************************************************
* digi_resource_get_coreotn_subcontainers
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle          - Reference to the digi handle
*  ho_chnl_hndl_type    - Higher order channel handle type.
*  ho_chnl_hndl_ptr     - Reference to high order channel handle.
*
* OUTPUTS:
*  ho_cal_array         - Array of lower order channel ids associated with
*                         the higher order channel.
*  num_entries_ptr      - Reference to the number of entries in the array.
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_resource_get_coreotn_subcontainers(digi_handle_t *digi_handle,
                                                         digi_handle_id_t ho_chnl_hndl_type,
                                                         digi_otn_odu_chnl_t *ho_chnl_hndl_ptr,
                                                         UINT32 ho_cal_array[DIGI_OTN_SERVER_MO_CHNL_MAX],
                                                         UINT32 *num_entries_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_struct_t *odu_struct_ptr = NULL;
    coreotn_chnl_order_t coreotn_chnl_order_level = LAST_COREOTN_CHNL_ORDER;
    PMC_ENTRY();

    odu_struct_ptr = (odu_struct_t *)ho_chnl_hndl_ptr;

    if (UTIL_GLOBAL_ODU_UNCHANNELIZED == odu_struct_ptr->mem_ptr->payload_format)
    {
        result = DIGI_ERR_HO_MO_CONN_TO_ODUKSW;
    }
    else if (DIGI_CHANNEL_HANDLE_ID_HO_ODU_T == ho_chnl_hndl_type)
    {
        coreotn_chnl_order_level = COREOTN_CHNL_ORDER_HO;
    }
    else if (DIGI_CHANNEL_HANDLE_ID_MO_ODU_T == ho_chnl_hndl_type)
    {
        coreotn_chnl_order_level = COREOTN_CHNL_ORDER_MO;
    }
    else
    {
        result = DIGI_ERR_INVALID_ARG;
    }

    if (PMC_SUCCESS == result)
    {
        coreotn_lo_chnl_get(digi_handle->coreotn_handle,
                                     odu_struct_ptr->switch_data.oduk_data.lineotn_src,
                                     coreotn_chnl_order_level,
                                     odu_struct_ptr->mem_ptr->chnl_id,
                                     ho_cal_array, 
                                     num_entries_ptr);
    }

    PMC_RETURN(result);
} /*digi_resource_get_coreotn_subcontainers*/

/*******************************************************************************
* digi_resource_release_subcontainer
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle          - Reference to the digi handle
*  ho_chnl_ctxt_ptr     - Reference to the high order channel .
*  odu_level            - ODU level of ho_chnl_ctxt_ptr
*  switch_release       - Switch release flag
*  mapotn_release       - Whether to release MAPOTN resource.
*  oduk_connect_state_ptr - Reference to the ODUK connection array.
*
* OUTPUTS:
*
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_resource_release_subcontainers(digi_handle_t *digi_handle,
                                                     digi_otn_odu_chnl_t *ho_chnl_ctxt_ptr,
                                                     digi_handle_id_t odu_level,
                                                     BOOL8 switch_release,
                                                     BOOL8 mapotn_release,
                                                     util_global_cpb_connect_t *oduk_connect_state_ptr)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    odu_struct_t *ho_chnl_ptr = (odu_struct_t *)ho_chnl_ctxt_ptr;
    digi_otn_odu_chnl_t *mo_chnl_ctxt_ptr[DIGI_OTN_SERVER_MAX_TRIBSLOT];
    digi_handle_id_t handle_type = LAST_DIGI_HANDLE_ID;
    UINT32 i;
    coreotn_odu_chnl_prov_mode_t initial_prov_mode = LAST_COREOTN_CHNL_PROV_MODE;
    BOOL8 mode_updated = FALSE;

    PMC_ENTRY();

    if (FALSE == switch_release && UTIL_GLOBAL_ODU_UNCHANNELIZED != ho_chnl_ptr->mem_ptr->payload_format)
    {
        result = coreotn_odu_chnl_prov_mode_get(digi_handle->coreotn_handle, &initial_prov_mode);

        if (PMC_SUCCESS == result)
        {
            result = digi_otn_odu_chnl_prov_mode_set(digi_handle, DIGI_OTN_ODU_CHNL_PROV_ALL);
            mode_updated = TRUE;
        }
    }
    mo_chnl_ctxt_ptr[0] = NULL;

    if (UTIL_GLOBAL_ODU_UNCHANNELIZED == ho_chnl_ptr->mem_ptr->payload_format)
      {
        util_global_oduk_port_t port_type = ho_chnl_ptr->switch_data.oduk_data.port_type;

        result = digi_resource_release_oduk_resource(digi_handle,
                                                     ho_chnl_ptr,
                                                     port_type,
                                                     ho_chnl_ptr->switch_data.oduk_data.channel,
                                                     switch_release,
                                                     mapotn_release,
                                                     oduk_connect_state_ptr);

        if (PMC_SUCCESS == result)
        {
            /*
             * Temporary value to get through the maze below wich is reset
             * at the bottom
             */
            result = DIGI_ERR_NO_OPERATIONAL_DATAPATH;
        }
    }
    else
    {
        result = digi_ho_subcontainer_chnl_hndl_get(digi_handle,
                                                    odu_level,
                                                    ho_chnl_ptr,
                                                    &handle_type,
                                                    mo_chnl_ctxt_ptr);
    }

    
    if (PMC_SUCCESS != result || 
        !(DIGI_CHANNEL_HANDLE_ID_MO_ODU_T == handle_type ||
          DIGI_CHANNEL_HANDLE_ID_LO_ODU_T == handle_type))
    {
        if (PMC_SUCCESS == result)
        {
            result = DIGI_ERR_INVALID_ARG;
        }
    }

    for (i = 0; i < DIGI_OTN_SERVER_MAX_TRIBSLOT && PMC_SUCCESS == result; i++)
    {
        if (NULL == mo_chnl_ctxt_ptr[i])
        {
            break;
        }
        odu_struct_t *mo_chnl_ptr = (odu_struct_t *)mo_chnl_ctxt_ptr[i];

        if (UTIL_GLOBAL_ODU_UNCHANNELIZED == mo_chnl_ptr->mem_ptr->payload_format)
        {
            result = digi_resource_release_oduk_resource(digi_handle,
                                                         mo_chnl_ptr,
                                                         UTIL_GLOBAL_ODUK_PORT_COREOTN,
                                                         mo_chnl_ptr->switch_data.oduk_data.channel,
                                                         switch_release,
                                                         mapotn_release,
                                                         oduk_connect_state_ptr);

        }
        else if (DIGI_CHANNEL_HANDLE_ID_HO_ODU_T == odu_level)
        {
            /*
             * Need to make a recursive call here to do this on the lower
             * order container as this is 2 level muxing.
             */
            result = digi_resource_release_subcontainers(digi_handle,
                                                         mo_chnl_ctxt_ptr[i],
                                                         DIGI_CHANNEL_HANDLE_ID_MO_ODU_T,
                                                         switch_release,
                                                         mapotn_release,
                                                         oduk_connect_state_ptr);
        }
        else
        {
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        }
        if (PMC_SUCCESS == result && FALSE == switch_release)
        {
            result = digi_otn_server_lo_deprov(digi_handle,
                                               &mo_chnl_ctxt_ptr[i],
                                               &ho_chnl_ctxt_ptr);
        }
    }

    if (PMC_SUCCESS == result && TRUE == mode_updated)
    {
        result = digi_otn_server_lo_deprov_end(digi_handle, &ho_chnl_ctxt_ptr);
    }

    if (DIGI_ERR_NO_OPERATIONAL_DATAPATH == result)
    {
        /*
         * If digi_ho_subcontainer_chnl_hndl_get fails because there are no
         * subcontainers then from the perspective of this release operator
         * that is a success
         */
        result = PMC_SUCCESS;
    }

    if (TRUE == mode_updated)
    {
        /*
         * Use a local return code here to ensure any error reported 
         * above is not overwritten.
         */
        PMC_ERROR rc = digi_otn_odu_chnl_prov_mode_set(digi_handle, 
                                                       (digi_otn_odu_chnl_prov_mode_t)initial_prov_mode);

        if (PMC_SUCCESS != rc)
        {
            result = rc;
        }
    }

    PMC_RETURN(result);
} /* digi_resource_release_subcontainers */

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */
/**************** END of PUBLIC FUNCTIONS *********************/
/*******************************************************************************
* digi_resource_clean_ilkn
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Iterates over the ILKN channel handles and cleans any that are mapped
*  but not operational.  Used specifically in a DIGI_PROD_APP_PORTLESS_CARD
*  scenario
*
* INPUTS:
*  digi_handle               - pointer to DIGI handle instance*   
*  digi_resource_handle_ptr  - Reference to the resource handle table
*  cpb_connect_data_ptr      - Reference to the CPB switch connection array
*
* OUTPUTS:
*  None
*
* RETURNS:
*  None
*
*******************************************************************************/
PRIVATE void digi_resource_clean_ilkn(digi_handle_t *digi_handle,
                                      digi_resource_handle_summary_t *digi_resource_handle_ptr,
                                      util_global_cpb_connect_t *cpb_connect_data_ptr)
{
    UINT32 i = 0;
    PMC_ENTRY();

    for (i = 0; i < DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX; i++)
    {
        UINT32 j = 0;

        for (j = 0; j < 2; j++)
        {
            digi_ilkn_chnl_def_t *ilkn_chnl_ptr = &(digi_handle->handle_pool.ilkn_1_chnl[i]);
            UINT32 ilkn_inst = 0;

            if (1 == j)
            {
                ilkn_chnl_ptr = &(digi_handle->handle_pool.ilkn_2_chnl[i]);
                ilkn_inst = 1;
            }
            /*
             * If the ILKN handle is not active or the master side state is
             * not operational then need to deactivate this resource.
             */
            if (TRUE == util_global_rx_or_tx_chnl_prov_status_get(ilkn_chnl_ptr->switch_data.header.prov_state) &&
                !(UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == ilkn_chnl_ptr->switch_data.cpb_data.op_state.active_state &&
                 UTIL_GLOBAL_CHNL_OPERATIONAL == cpb_connect_data_ptr->chnl[ilkn_inst][i].mst.state))
            {
                PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                        "Clean ILKN Inst=%d. Ch=%d, OutputMapped=%d, MstState=%d\n",
                        (UINT32)ilkn_inst,
                        ilkn_chnl_ptr->switch_data.cpb_data.channel,
                        ilkn_chnl_ptr->switch_data.cpb_data.op_state.map_state.is_output_mapped,
                        cpb_connect_data_ptr->chnl[ilkn_inst][i].mst.state);
                        
                if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE ==
                    ilkn_chnl_ptr->switch_data.cpb_data.op_state.active_state)
                {
                    digi_resource_cpb_chnl_deactivate(digi_handle,
                                                      digi_resource_handle_ptr,
                                                      i,
                                                      ilkn_chnl_ptr);
                }
                digi_resource_cpb_chnl_demap(digi_handle,
                                             digi_resource_handle_ptr,
                                             i,
                                             ilkn_chnl_ptr);
                /*
                 * If this channel handle state was start then it is likely
                 * that the input is as well. Check the connect array 
                 * slave state. If start then reset the input counters as
                 * well.
                 */
                if (UTIL_GLOBAL_CHNL_START == cpb_connect_data_ptr->chnl[ilkn_inst][i].slv.state)
                {
                    ilkn_chnl_ptr->switch_data.cpb_data.op_state.map_state.input_map_count = 0;
                    ilkn_chnl_ptr->switch_data.cpb_data.op_state.map_state.is_input_mapped = FALSE;
                    ilkn_chnl_ptr->switch_data.cpb_data.op_state.map_state.dest_active_count = 0;
                    ilkn_chnl_ptr->switch_data.cpb_data.op_state.active_state = UTIL_GLOBAL_CHNL_ACTIVE_STATE_INACTIVE;
                }
            }
            else if (TRUE == util_global_rx_or_tx_chnl_prov_status_get(ilkn_chnl_ptr->switch_data.header.prov_state))
            {
                UINT32 src_port = cpb_connect_data_ptr->chnl[ilkn_inst][i].mst.source_slv_port;
                /*
                 * If the ILKN channel is valid and connected to MAPOTN
                 * then need to check that MAPOTN is valid. The ODUK SW
                 * is checked first and if MAPOTN is unconnected to a OTN
                 * resource then it is not connected to a serdes port and
                 * may be missed. Account for that here.
                 */
                if (UTIL_GLOBAL_CPB_PORT_MAPOTN == (util_global_cpb_port_t)src_port)
                {
                    UINT32 src_chnl = cpb_connect_data_ptr->chnl[ilkn_inst][i].mst.source_slv_chnl;
                    util_global_cpb_connect_t *oduk_connect_data_ptr = NULL;
                    coreotn_oduksw_connect_data_state_get(digi_handle->coreotn_handle,
                                                          &oduk_connect_data_ptr);
                    if (!(UTIL_GLOBAL_CHNL_OPERATIONAL == oduk_connect_data_ptr->chnl[src_port][src_chnl].slv.state ||
                        UTIL_GLOBAL_CHNL_OPERATIONAL == oduk_connect_data_ptr->chnl[src_port][src_chnl].mst.state))
                    {
                        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                                "Clean ILKN as MAPOTN is unconnected. IlknCh=%d, MAPOTNChnl=%d\n",
                                ilkn_chnl_ptr->switch_data.cpb_data.channel,
                                src_chnl);
                        
                        if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE ==
                            ilkn_chnl_ptr->switch_data.cpb_data.op_state.active_state)
                        {
                            digi_resource_cpb_chnl_deactivate(digi_handle,
                                                              digi_resource_handle_ptr,
                                                              i,
                                                              ilkn_chnl_ptr);
                        }
                        digi_resource_cpb_chnl_demap(digi_handle,
                                                     digi_resource_handle_ptr,
                                                     i,
                                                     ilkn_chnl_ptr);
                        /*
                         * If this channel handle state was start then it is likely
                         * that the input is as well. Check the connect array 
                         * slave state. If start then reset the input counters as
                         * well.
                         */
                        ilkn_chnl_ptr->switch_data.cpb_data.op_state.map_state.input_map_count = 0;
                        ilkn_chnl_ptr->switch_data.cpb_data.op_state.map_state.is_input_mapped = FALSE;
                        ilkn_chnl_ptr->switch_data.cpb_data.op_state.map_state.dest_active_count = 0;
                        ilkn_chnl_ptr->switch_data.cpb_data.op_state.active_state = UTIL_GLOBAL_CHNL_ACTIVE_STATE_INACTIVE;
                    }
                }
            }
        }   
    }

    PMC_RETURN();
} /* digi_resource_clean_ilkn */


/*******************************************************************************
* digi_resource_clean_otn
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Iterates over the OTN handles and if there are any inconsistencies 
*  between the handles and the ODUK switch connect array cleans them
*
* INPUTS:
*  digi_handle               - pointer to DIGI handle instance*   
*  digi_resource_handle_ptr  - Reference to the resource handle table
*  oduk_connect_data_ptr     - Reference to ODUK connect array.
*
* OUTPUTS:
*  None
*
* RETURNS:
*  None
*
*******************************************************************************/
PRIVATE void digi_resource_clean_otn(digi_handle_t *digi_handle,
                                     digi_resource_handle_summary_t *digi_resource_handle_ptr,
                                     util_global_cpb_connect_t *oduk_connect_data_ptr)
{
    UINT32 i = 0;
    odu_struct_t *otn_ptr = NULL;

    PMC_ENTRY();

    for (i = 0; i < DIGI_OTN_SERVER_LO_CHNL_MAX; i++)
    {
        UINT32 port_type = (UINT32)UTIL_GLOBAL_ODUK_PORT_COREOTN;
        otn_ptr = &(digi_handle->handle_pool.lo_odu_struct[i]);

        if (digi_handle->var.prod_app == DIGI_PROD_APP_SYSOTN_CARD)
        {
            port_type = (UINT32)UTIL_GLOBAL_ODUK_PORT_ILKN1;
        }

        if (FALSE == digi_resource_clean_otn_handle(digi_handle,
                                                    digi_resource_handle_ptr,
                                                    otn_ptr,
                                                    oduk_connect_data_ptr,
                                                    i,
                                                    port_type))
        {
            digi_resource_deprovision_otn(digi_handle,
                                          digi_resource_handle_ptr,
                                          oduk_connect_data_ptr,
                                          otn_ptr,
                                          UTIL_GLOBAL_LEVEL_LO_ODU,
                                          port_type);
        }
    }

    for (i = 0; i < DIGI_OTN_SERVER_MO_CHNL_MAX; i++)
    {
        otn_ptr = &(digi_handle->handle_pool.mo_odu_struct[i]);

        if (FALSE == digi_resource_clean_otn_handle(digi_handle,
                                                    digi_resource_handle_ptr,
                                                    otn_ptr,
                                                    oduk_connect_data_ptr,
                                                    i,
                                                    (UINT32)UTIL_GLOBAL_ODUK_PORT_COREOTN))
        {
            digi_resource_deprovision_otn(digi_handle,
                                          digi_resource_handle_ptr,
                                          oduk_connect_data_ptr,
                                          otn_ptr,
                                          UTIL_GLOBAL_LEVEL_MO_ODU,
                                          (UINT32)UTIL_GLOBAL_ODUK_PORT_COREOTN);
        }
    }

    for (i = 0; i < DIGI_OTN_SERVER_HO_CHNL_MAX; i++)
    {
        otn_ptr = &(digi_handle->handle_pool.ho_odu_struct[i]);

        if (FALSE == digi_resource_clean_otn_handle(digi_handle,
                                                    digi_resource_handle_ptr,
                                                    otn_ptr,
                                                    oduk_connect_data_ptr,
                                                    i,
                                                    (UINT32)UTIL_GLOBAL_ODUK_PORT_COREOTN))
        {
            digi_resource_deprovision_otn(digi_handle,
                                          digi_resource_handle_ptr,
                                          oduk_connect_data_ptr,
                                          otn_ptr,
                                          UTIL_GLOBAL_LEVEL_HO_ODU,
                                          (UINT32)UTIL_GLOBAL_ODUK_PORT_COREOTN);
        }
    }

    PMC_RETURN();
} /* digi_resource_clean_otn */


/*******************************************************************************
* digi_resource_is_switch_operational
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Determines the state of the connection at the particular switch based on
*  the soruce or destination being examined. From there calls the 
*  appropriate subsystem operation state to determine the state of that
*  resource.
*
* INPUTS:
*   digi_handle          - Reference to the digi_handle
*   digi_resource_handle - Reference to the resource table.
*   switch_index         - Index of the switch from the perspective of
*                          the provisioned handle that has MAPOTN as its
*                          source.
*   switch_type          - Either CPB or ODUK switch and is the reference
*                          for the side of the switch that is initially 
*                          being examined. The switch_type side must be
*                          OPERATIONAL for this to be valid. The other side
*                          can be PROVISIONED and this be considered valid.
*   is_source            - Whether being called from the perspective of 
*                          the source or the destination.
*   direction            - Direction the data path is being checked.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   TRUE:  MAPOTN resource identified by map_index is OPERATIONAL.
*   FALSE: Otherwise
*
* NOTES:
*  Function assumes that this is called for the source first and then the
*  destination. The call for the operational check is only done on the 
*  destination if it is different than the source as it assumes that if it
*  is the source has already been checked.
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_is_switch_operational(digi_handle_t *digi_handle,
                                                  digi_resource_handle_summary_t *digi_resource_handle,
                                                  UINT16 switch_index,
                                                  digi_resource_next_handle_t switch_type,
                                                  BOOL8 is_source,
                                                  digi_direction_check_t direction)
{
    BOOL8 result = FALSE;
    digi_switch_ctxt_t *switch_ptr;

    PMC_ENTRY();

    if (DIGI_CPB_SWITCH == switch_type)
    {
        switch_ptr = &digi_resource_handle->cpbsw_cfg[switch_index];
    }
    else
    {
        switch_ptr = &digi_resource_handle->oduksw_cfg[switch_index];
    }
    /*
     * For OTN related resources TX is true if the destination is activated
     * and RX is source is activated. However for the SYS MO the opposite
     * is true if coming from the line side. Set the is_source to FALSE
     * to ensure that the correct check is made.
     */
    if (TRUE == is_source && TRUE == is_line_side && 
        TRUE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle) &&
        (DIGI_SYS_OTN_MO_CONTAINER == switch_ptr->provisioned_handle_type ||
         DIGI_SYS_OTN_HO_CONTAINER == switch_ptr->provisioned_handle_type))
    {
        is_source = FALSE;
    }

    if (DIGI_DIRECTION_RX_ONLY == direction && 
        FALSE == switch_ptr->source_activated && 
        LAST_DIGI_RESOURCE_HANDLE != switch_ptr->associate_dest_handle_type &&
        LAST_DIGI_RESOURCE_HANDLE != switch_ptr->source_handle_type &&
        switch_ptr->associate_dest_handle_type == switch_ptr->source_handle_type &&
        switch_ptr->associate_dest_index == switch_ptr->source_handle_index)

    {
        /*
         * Handles this as a special case. The serdes port is not actually
         * connected in the RX direction but is in a broadcast situation
         * and its secondary port is the same as its source port. This
         * indicates that the source port is mapped back to itself. As
         * long as that path is complete in the RX direction then this
         * data path is good.
         */
        result = digi_resource_check_operational_level(digi_handle,
                                                       digi_resource_handle,
                                                       switch_ptr->associate_dest_handle_type,
                                                       switch_ptr->associate_dest_index,
                                                       switch_type,
                                                       direction);
        PMC_RETURN(result);
    }
    /*
     * If the current element is the source and the direction is TX then
     * its destination needs to be activated. Otherwise if RX then its
     * source needs to be activated. If both directions then both have to
     * be activated.
     * If coming at this from the other direction then the converse is 
     * TRUE.
     */
    if (TRUE == is_source &&
        ((DIGI_DIRECTION_TX_ONLY == direction && FALSE == switch_ptr->dest_activated) ||
         (DIGI_DIRECTION_RX_ONLY == direction && FALSE == switch_ptr->source_activated) ||
         (DIGI_DIRECTION_TX_RX == direction && (FALSE == switch_ptr->dest_activated || FALSE == switch_ptr->source_activated))))
      {
        /*
         * If coming at this from the RX direction check whether this is
         * a broad/multi cast case. If the associate_dest_handle_type is
         * set for this datapath and even though the source is not activate
         * it may be a valid path.
         * If the associated_dest_handle_type is fully operational and its
         * destination is broad/multi cast and is fully OPERATIONAL then
         * set this to valid.  That is this RX datapath is the secondary
         * destination of a fully operational data path and as such is
         * fully operational.
         */
        if (DIGI_DIRECTION_RX_ONLY == direction &&
            LAST_DIGI_RESOURCE_HANDLE != switch_ptr->associate_dest_handle_type)
        {
            UINT32 chnl = switch_ptr->associate_dest_index;
            UINT32 port = 
              (UINT32)digi_resource_get_oduk_port_type(switch_ptr->associate_dest_handle_type,
                                               &chnl);
            util_global_cpb_connect_t *oduk_connect_ptr = NULL;
            coreotn_oduksw_connect_data_state_get(digi_handle->coreotn_handle,
                                                  &oduk_connect_ptr);

            if (UTIL_GLOBAL_CHNL_OPERATIONAL == oduk_connect_ptr->chnl[port][chnl].slv.state &&
                UTIL_GLOBAL_CHNL_OPERATIONAL == oduk_connect_ptr->chnl[port][chnl].mst.state)
            {
                UINT32 dest_port = oduk_connect_ptr->chnl[port][chnl].slv.dest_mst_port;
                UINT32 dest_chnl = oduk_connect_ptr->chnl[port][chnl].slv.dest_mst_chnl;

                if ((TRUE == oduk_connect_ptr->chnl[dest_port][dest_chnl].slv.is_broadcast || 
                     TRUE == oduk_connect_ptr->chnl[dest_port][dest_chnl].slv.is_multicast) &&
                    UTIL_GLOBAL_CHNL_OPERATIONAL == oduk_connect_ptr->chnl[dest_port][dest_chnl].slv.state &&
                    UTIL_GLOBAL_CHNL_OPERATIONAL == oduk_connect_ptr->chnl[dest_port][dest_chnl].mst.state &&
                    port ==  oduk_connect_ptr->chnl[dest_port][dest_chnl].mst.source_slv_port &&
                    chnl == oduk_connect_ptr->chnl[dest_port][dest_chnl].mst.source_slv_chnl)
                {
                    PMC_RETURN(TRUE);
                }
            }
        }
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "SwitchIndex=%d for ProvisionedHandle=%d is not fully OPERATIONAL. Direction=%d\n", 
                switch_index, switch_ptr->provisioned_handle_type,
                (UINT32)direction);
    }
    else if (FALSE == is_source &&
             ((DIGI_DIRECTION_TX_ONLY == direction && FALSE == switch_ptr->source_activated) ||
              (DIGI_DIRECTION_RX_ONLY == direction && FALSE == switch_ptr->dest_activated) ||
              (DIGI_DIRECTION_TX_RX == direction && (FALSE == switch_ptr->dest_activated || FALSE == switch_ptr->source_activated))))
    {
        PMC_LOG_TRACE("SwitchIndex=%d for ProvisionedHandle=%d is not fully OPERATIONAL. Direction=%d\n", 
                      switch_index, switch_ptr->provisioned_handle_type,
                      direction);
    }
    else if ((DIGI_DIRECTION_TX_ONLY == direction || DIGI_DIRECTION_TX_RX == direction) && !(switch_ptr->provisioned_handle_type == switch_ptr->source_handle_type &&
              switch_ptr->provisioned_handle_index == switch_ptr->source_handle_index))
    {
        digi_resource_next_handle_t src_handle = switch_ptr->source_handle_type;
        UINT16 src_index = switch_ptr->source_handle_index;

        if (TRUE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle) &&
            DIGI_SYS_OTN_MO_CONTAINER == switch_ptr->provisioned_handle_type && TRUE == is_line_side &&
            DIGI_SYS_OTN_MO_CONTAINER == digi_resource_handle->sys_otn_ho_container_cfg[0].next_resource_handle)
        {
            src_handle = DIGI_SYS_OTN_HO_CONTAINER;
            src_index = digi_resource_handle->sys_otn_ho_container_cfg[0].next_handle_index;
        }

        /*
         * This is the source side and the provisioned handle is not the
         * same as the source meaning that it is not loopbacked to itself.
         */
        result = digi_resource_check_operational_level(digi_handle,
                                                       digi_resource_handle,
                                                       src_handle,
                                                       src_index,
                                                       switch_type,
                                                       direction);
    }
    else if ((DIGI_DIRECTION_RX_ONLY == direction || DIGI_DIRECTION_TX_RX == direction) && !(switch_ptr->provisioned_handle_type == switch_ptr->primary_dest_handle_type && switch_ptr->provisioned_handle_index == switch_ptr->primary_dest_index) && switch_ptr->num_dest < 2)
    {
        digi_resource_next_handle_t dest_handle = switch_ptr->primary_dest_handle_type;
        UINT16 dest_index = switch_ptr->primary_dest_index;

        if (TRUE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle) &&
            DIGI_SYS_OTN_MO_CONTAINER == switch_ptr->provisioned_handle_type && TRUE == is_line_side &&
            DIGI_SYS_OTN_MO_CONTAINER == digi_resource_handle->sys_otn_ho_container_cfg[0].next_resource_handle)
        {
            dest_handle = DIGI_SYS_OTN_HO_CONTAINER;
            dest_index = digi_resource_handle->sys_otn_ho_container_cfg[0].next_handle_index;
        }

        result = digi_resource_check_operational_level(digi_handle,
                                                       digi_resource_handle,
                                                       dest_handle,
                                                       dest_index,
                                                       switch_type,
                                                       direction);
    }
    else if (switch_ptr->num_dest > 1
             && !(switch_ptr->source_handle_type == switch_ptr->primary_dest_handle_type &&
               switch_ptr->source_handle_index == switch_ptr->primary_dest_index) &&
             !(switch_ptr->provisioned_handle_type == switch_ptr->primary_dest_handle_type &&
               switch_ptr->provisioned_handle_index == switch_ptr->primary_dest_index))
      {
        /*
         * This is a broadcast case where the source and destination are
         * not the same. For the RX or TX direction to be valid then one of the
         * destinations needs to be mapped. Either the primary_destination
         * or the secondary.  Call digi_resource_check_operational_level
         * on the destination. If that fails need to determine the 
         * secondary destination and need to determine if that is valid
         * in the direction.
         */
        result = digi_resource_check_operational_level(digi_handle,
                                                       digi_resource_handle,
                                                       switch_ptr->primary_dest_handle_type,
                                                       switch_ptr->primary_dest_index, 
                                                       switch_type,
                                                       direction);

        if (FALSE == result)
        {
            result = digi_resource_find_secondary_check_op_level(digi_handle,
                                                                 digi_resource_handle,
                                                                 switch_ptr->primary_dest_handle_type,
                                                                 switch_ptr->primary_dest_index, 
                                                                 switch_type,
                                                                 direction);                                                            
        }
      }
    else if (!(switch_ptr->source_handle_type == switch_ptr->primary_dest_handle_type &&
               switch_ptr->source_handle_index == switch_ptr->primary_dest_index) &&
             !(switch_ptr->provisioned_handle_type == switch_ptr->primary_dest_handle_type &&
               switch_ptr->provisioned_handle_index == switch_ptr->primary_dest_index))
    {
        /*
         * This is the destination. Source and destination are not the same
         * and the provisioned handle is not the same as the destination 
         * indicating that the destination is not loopbacked to itself.
         */
        result = digi_resource_check_operational_level(digi_handle,
                                                       digi_resource_handle,
                                                       switch_ptr->primary_dest_handle_type,
                                                       switch_ptr->primary_dest_index, 
                                                       switch_type,
                                                       direction);
    }
    else
    {
        /*
         * This is a loopback at the source side. The resource at this
         * level and at the switch has been verified so return TRUE.
         */
        result = TRUE;
    }

    PMC_RETURN(result);
} /* digi_resource_is_switch_operational */


/*******************************************************************************
* digi_resource_is_otn_operational
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Determines the operational state of the OTN subcontainer. Can be called
*  for any of the OTN related subcontainers
*
* INPUTS:
*   digi_handle          - Reference to the digi_handle
*   digi_resource_handle - Reference to the resource table.
*   otn_handle           - Handle to the OTN subcontainer being examined.
*   otn_index            - Index into the resource table for the particular
*                          subcontainer.
*   is_oduk_side         - From which perspective is this being called. A
*                          value of FALSE indicates coming from LINEOTN 
*                          towards ODUK. TRUE indicates ODUK to LINEOTN
*   direction            - Represents the direction the data path should be
*                          checked to determine the operational level.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   TRUE:  OTN resource identified by otn_handle and all the it is connected
*          to is an OPERATIONAL state.
*   FALSE: Otherwise
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_is_otn_operational(digi_handle_t *digi_handle,
                                               digi_resource_handle_summary_t *digi_resource_handle,
                                               digi_resource_next_handle_t otn_handle,
                                               UINT16 otn_index,
                                               BOOL8 is_oduk_side,
                                               digi_direction_check_t direction)
{
    BOOL8 result = FALSE;
    digi_otn_container_cfg_t *otn_container_ptr = NULL;
    digi_handle_id_t handle_id = LAST_DIGI_HANDLE_ID;
    BOOL8 final_result = TRUE;

    PMC_ENTRY();

    switch (otn_handle)
    {
    case DIGI_LINE_OTN_HO_CONTAINER:
        otn_container_ptr = &digi_resource_handle->line_otn_ho_container_cfg[otn_index];
        handle_id = DIGI_CHANNEL_HANDLE_ID_HO_ODU_T;
        break;

    case DIGI_LINE_OTN_MO_CONTAINER:
        if (DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == digi_handle->var.prod_app)
        {
            otn_container_ptr = &digi_resource_handle->sys_otn_mo_container_cfg[otn_index];
        }
        else
        {
            otn_container_ptr = &digi_resource_handle->line_otn_mo_container_cfg[otn_index];
        }
        handle_id = DIGI_CHANNEL_HANDLE_ID_MO_ODU_T;
        break;

    case DIGI_LINE_OTN_LO_CONTAINER:
        otn_container_ptr = &digi_resource_handle->line_otn_lo_container_cfg[otn_index];
        handle_id = DIGI_CHANNEL_HANDLE_ID_LO_ODU_T;
        break;

    case DIGI_SYS_OTN_HO_CONTAINER:
        otn_container_ptr = &digi_resource_handle->sys_otn_ho_container_cfg[otn_index];
        handle_id = DIGI_CHANNEL_HANDLE_ID_HO_ODU_T;
        break;

    case DIGI_SYS_OTN_MO_CONTAINER:
        otn_container_ptr = &digi_resource_handle->sys_otn_mo_container_cfg[otn_index];
        handle_id = DIGI_CHANNEL_HANDLE_ID_MO_ODU_T;
        break;
    case DIGI_SYS_SERDES_CFG:
        if (DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == digi_handle->var.prod_app)
        {
            otn_container_ptr = &digi_resource_handle->sys_otn_ho_container_cfg[otn_index];
        }
        else
        {
            otn_container_ptr = &digi_resource_handle->line_otn_ho_container_cfg[otn_index];
        }
        handle_id = DIGI_CHANNEL_HANDLE_ID_HO_ODU_T;
        break;

    case LAST_DIGI_RESOURCE_HANDLE:
      PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
              "OTN Handle is UNDEFINED. Return FALSE\n");
      PMC_RETURN(FALSE);

    default:
      PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0, 
              "OTN_Handle=%d is not an OTN Container", otn_handle);
      fflush(stdout);
      PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }
    /*
     * Perform a little sanity checking on the state of things. then
     * the resource is either unchannelized and connected to the ODUK
     * switch or not and connected to another OTN lower subcontainer. For
     * the former call the switch related operational check and for the
     * latter call this function again with the next resource handle.
     * If it is not an OTN container then it should fail on the null ==
     * otn_container check.
     */
    if (NULL == otn_container_ptr)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0, 
                "OTN Container=%d at index=%d not FOUND or INVALID\n",
                otn_handle, otn_index);
    }
    else if (handle_id != otn_container_ptr->handle_type)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0, 
                "OTN Container=%d Does not match ContainerHandleType=%d\n",
                handle_id, otn_container_ptr->handle_type);
    }
    else if (NULL == otn_container_ptr->chnl_handle_ptr)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0, 
                "OTN Container=%d Channel handle is NULL\n",
                otn_handle);
    }
    else if (LAST_DIGI_RESOURCE_HANDLE == otn_container_ptr->next_resource_handle && DIGI_LINE_OTN_LO_CONTAINER == otn_handle)
      
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0, 
                "Next resource handle is not SET. Path is not OPERATIONAL\n");
    }
    else if ((DIGI_LINE_OTN_LO_CONTAINER == otn_handle ||
              DIGI_SYS_OTN_MO_CONTAINER == otn_handle) &&
             DIGI_ODU_UNCHANNELIZED != otn_container_ptr->payload_mode)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0, 
                "Handle is LO_CONTAINER but payload is not UNCHANNELIZED\n");
    }
    else if (DIGI_ODU_UNCHANNELIZED == otn_container_ptr->payload_mode &&
             DIGI_ODUK_SWITCH != otn_container_ptr->next_resource_handle)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0, 
                "nextResourceHandle=%d is not ODUK_SWITCH as expected\n", 
                otn_container_ptr->next_resource_handle);
    }
    else
    {
        BOOL8 tx_result = FALSE;
        BOOL8 rx_result = FALSE;
        
        if (DIGI_DIRECTION_TX_RX == direction ||
            DIGI_DIRECTION_TX_ONLY == direction)
        {
            tx_result = digi_resource_check_otn_prov_get(digi_handle,
                                                       digi_resource_handle,
                                                       otn_handle,
                                                       otn_container_ptr,
                                                       DIGI_DIRECTION_TX_ONLY);
        }

        if (DIGI_DIRECTION_TX_RX == direction ||
            DIGI_DIRECTION_RX_ONLY == direction)
        {
            rx_result = digi_resource_check_otn_prov_get(digi_handle,
                                                       digi_resource_handle,
                                                       otn_handle,
                                                       otn_container_ptr,
                                                       DIGI_DIRECTION_RX_ONLY);
        }

        result = digi_resource_set_directional_result(direction, tx_result,
                                                      rx_result);
    }                                              

    if (TRUE == result) 
    {
        /*
         * If the payload mode is unchannelized then must be connected to
         * the switch. Otherwise there is another otn subcontainer that this
         * otn container is connected to.  Since is_oduk_side is false then
         * if unchannelized then must going to the switch and coming from 
         * LineOTN side.
         * NOTE: Added explicit check to ensure that any incorrect 
         * connectins are caught as soon as possible.
         */
        if (DIGI_ODU_UNCHANNELIZED == otn_container_ptr->payload_mode &&
            DIGI_ODUK_SWITCH == otn_container_ptr->next_resource_handle &&
            FALSE == is_oduk_side)
        {
            result = digi_resource_is_switch_operational(digi_handle,
                                                         digi_resource_handle,
                                                         otn_container_ptr->next_handle_index,
                                                         otn_container_ptr->next_resource_handle,
                                                         TRUE,
                                                         direction);
            
            digi_resource_set_valid_path(digi_handle,
                                         otn_handle,
                                         direction,
                                         FALSE,
                                         result,
                                         otn_container_ptr);
        }
        else if (TRUE == result &&
                 (DIGI_ODU_TS_1G25 == otn_container_ptr->payload_mode ||
                  DIGI_ODU_TS_2G5 == otn_container_ptr->payload_mode))
        {
            if (TRUE == is_oduk_side)
            {
                PMC_RETURN(TRUE);
            }
            result = digi_resource_is_otn_operational(digi_handle,
                                                      digi_resource_handle,
                                                      otn_container_ptr->next_resource_handle,
                                                      otn_container_ptr->next_handle_index,
                                                      is_oduk_side,
                                                      direction);
        }
        else if (DIGI_ODU_UNCHANNELIZED == otn_container_ptr->payload_mode &&
                 DIGI_ODUK_SWITCH == otn_container_ptr->next_resource_handle &&
                 TRUE == is_oduk_side && TRUE == result)
        {
            BOOL8 tx_value;
            /*
             * If here then being checked from CPB side and need to
             * confirm that everything is fine towards the lineotn and
             * serdes port.
             */
            digi_resource_set_valid_path(digi_handle,
                                         otn_handle,
                                         direction,
                                         FALSE,
                                         result,
                                         otn_container_ptr);

            /*
             * Need to cross the rx and tx as the check is coming from 
             * the other side.  Cross over the values here to ensure that
             * rx and tx is correct based on the perspective from which the
             * data paths is checked.
             */
            tx_value = otn_container_ptr->tx_path_is_valid;
            otn_container_ptr->tx_path_is_valid = otn_container_ptr->rx_path_is_valid;
            otn_container_ptr->rx_path_is_valid = tx_value;

            if (DIGI_LINE_OTN_HO_CONTAINER == otn_handle)
            {
                /*
                 * If here then the OTN container is correctly provisioned and the
                 * connectivity check is coming from the ODUK switch and moving
                 * through the OTN Container(s) to the LineOTN subsystem.
                 */
                digi_serdes_port_intf_ctxt_t *serdes_port_ptr = NULL;
                UINT16 index = otn_container_ptr->parent_handle_index;
                BOOL8 is_operational = FALSE;
                UINT32 tmp_line_side = is_line_side;
                
                if (DIGI_LINE_SERDES_CFG == otn_container_ptr->parent_handle_type)
                {
                    serdes_port_ptr = &digi_resource_handle->line_serdes_cfg[index];
                }
                else if (DIGI_SYS_SERDES_CFG == otn_container_ptr->parent_handle_type)
                {
                    serdes_port_ptr = &digi_resource_handle->sys_serdes_cfg[index];
                }
                else if (DIGI_SFI51_SERDES_CFG == otn_container_ptr->parent_handle_type)
                {
                    serdes_port_ptr = &digi_resource_handle->sfi51_serdes_cfg[index];
                }
                /*
                 * Passed in serdes_port_ptr could technically be NULL although the
                 * possible choices are checked. If it is NULL then the called
                 * function will assert.
                 */
                result = digi_resource_is_lineotn_operational(digi_handle,
                                                              digi_resource_handle,
                                                              serdes_port_ptr,
                                                              TRUE,
                                                              FALSE,
                                                              direction,
                                                              &is_operational);
                /*
                 * The TRUE parameter indicates that the call is being
                 * made on behalf of the line side. Reset the global
                 * is_line_side to what it was before the call so that if
                 * SYSPOTN it will not hamper check results for the 
                 * next otn subcontainer if there is one.
                 */
                is_line_side = tmp_line_side;
            }
            else
            {
                result = digi_resource_is_otn_operational(digi_handle,
                                                          digi_resource_handle,
                                                          otn_container_ptr->parent_handle_type,
                                                          otn_container_ptr->parent_handle_index,
                                                          is_oduk_side,
                                                          direction);
            }
        }
        else if (DIGI_ODU_NULL != otn_container_ptr->payload_mode)
        {
            PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0, 
                    "Mode=%d and NextResource=%d. Mismatch ERROR!", 
                    otn_container_ptr->payload_mode,
                    otn_container_ptr->next_resource_handle);
            result = FALSE;
        }
    }
    /*
     * Set the final_result twice. Once before the next call to ensure
     * it is not missed if this container failed but the next one passes.
     * As well after the call to the next container in case it is the last
     * one that fails.
     */
    if (FALSE == result)
    {
        final_result = FALSE;
    }
    /*
     * If not at the top level, that is High Order channel, then need to
     * check the underlying siblings of this High Order connection.
     * There are other channels as long as this is a low or mid order
     * channel and the next_associate_handle points to a valid value
     * Will need to include the SYSOTN cases as well.
     * Need to include the check regardless of whether the previous call 
     * was successful or not. Need to ensure all siblings of the current
     * mid/lo order channe is checked.
     */
    if (FALSE == is_oduk_side &&
        (DIGI_LINE_OTN_LO_CONTAINER == otn_handle ||
         DIGI_LINE_OTN_MO_CONTAINER == otn_handle ||
         (DIGI_SYS_OTN_MO_CONTAINER == otn_handle && FALSE == is_line_side)) &&
        LAST_DIGI_RESOURCE_HANDLE != otn_container_ptr->next_associate_handle)
    {
        /*
         * If this is a Mid order container and it is channelized then
         * set its state here as its state is based only on its underlying
         * containers and not the subcontainers of other Mid Order 
         * containers.
         */
        if (DIGI_LINE_OTN_MO_CONTAINER == otn_handle &&
            DIGI_ODU_UNCHANNELIZED != otn_container_ptr->payload_mode)
        {
            digi_resource_set_valid_path(digi_handle,
                                     otn_handle,
                                     direction,
                                     FALSE,
                                     final_result,
                                     otn_container_ptr);
        }
        result = digi_resource_is_otn_operational(digi_handle,
                                                  digi_resource_handle,
                                                  otn_container_ptr->next_associate_handle,
                                                  otn_container_ptr->next_associate_index,
                                                  FALSE,
                                                  direction);
    }    

    if (FALSE == result)
    {
        final_result = FALSE;
    }   
    /*
     * Set only the HO container here as it is based on all the underlying
     * containers and for its state to be set to valid then all underlying
     * containers for this direction must also be valid.
     */
    if (DIGI_ODU_UNCHANNELIZED != otn_container_ptr->payload_mode &&
        DIGI_LINE_OTN_HO_CONTAINER == otn_handle )
    {
        digi_resource_set_valid_path(digi_handle,
                                     otn_handle,
                                     direction,
                                     FALSE,
                                     final_result,
                                     otn_container_ptr);
    }

    PMC_RETURN(final_result);
}  /* digi_resource_is_otn_operational */


/*******************************************************************************
* digi_resource_is_ilkn_operational
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Determines the operational state of the ILKN N subcontainer. Called for
*  either ILKN interfaces and curently expected to be called from the ODUK
*  or CPB switches
*
* INPUTS:
*   digi_handle          - Reference to the digi_handle
*   digi_resource_handle - Reference to the resource table.
*   ilkn_handle          - Handle to the OTN subcontainer being examined.
*   ilkn_index           - Index into the resource table for the particular
*                          subcontainer.
*   switch_type          - Perspective of the caller. Eithr ODUK or CPB
*                          switch
*   direction            - Direction the data path is being checked.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   TRUE:  ILKN resource is fully operational.
*   FALSE: Otherwise
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_is_ilkn_operational(digi_handle_t *digi_handle,
                                                digi_resource_handle_summary_t *digi_resource_handle,
                                                digi_resource_next_handle_t ilkn_handle,
                                                UINT16 ilkn_index,
                                                digi_resource_next_handle_t switch_type,
                                                 digi_direction_check_t direction)
{
    BOOL8 result = FALSE;
    digi_ilkn_chnl_ctxt_t *ilkn_ptr = NULL;

    PMC_ENTRY();

    if (DIGI_ILKN_1 == ilkn_handle)
    {
        ilkn_ptr = &digi_resource_handle->ilkn_chnl_cfg[0][ilkn_index];
    }
    else
    {
        ilkn_ptr = &digi_resource_handle->ilkn_chnl_cfg[1][ilkn_index];
    }

    /*
     * Perform a little sanity checking on the state of things. then
     * the resource is either unchannelized and connected to the ODUK
     * switch or not and connected to another OTN lower subcontainer. For
     * the former call the switch related operational check and for the
     * latter call this function again with the next resource handle.
     * If it is not an OTN container then it should fail on the null ==
     * otn_container check.
     */
    if (NULL == ilkn_ptr)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0, 
                "ILKN index=%d not FOUND or INVALID\n", ilkn_index);
    }
    else if (NULL == ilkn_ptr->ilkn_chnl_handle_ptr)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "ILKN Channel handle at Index=%d is NULL\n", ilkn_index);
    }
    else if (LAST_DIGI_RESOURCE_HANDLE == ilkn_ptr->next_resource_handle)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "ILKN Container at Index=%d NextResource is LAST_DIGI\n",
                ilkn_index);
    }
    else
    {
        digi_ilkn_chnl_def_t *ilkn_chnl_def_ptr = (digi_ilkn_chnl_def_t *)ilkn_ptr->ilkn_chnl_handle_ptr;
        digi_switch_ctxt_t *switch_ptr = NULL;

        /*
         * ILKN should be the end of the line regardless of which switch
         * it is connected across. For the TX case it must be mapped and
         * for RX is must be activated.
         */
        if (NULL == ilkn_chnl_def_ptr || 
            (DIGI_DIRECTION_RX_ONLY == direction &&
             ((DIGI_ODUK_SWITCH == switch_type &&
             UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE != ilkn_chnl_def_ptr->switch_data.oduk_data.op_state.active_state) ||
            (DIGI_CPB_SWITCH == switch_type &&
             UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE != ilkn_chnl_def_ptr->switch_data.cpb_data.op_state.active_state))) ||
           (DIGI_DIRECTION_TX_ONLY == direction &&
           ((DIGI_ODUK_SWITCH == switch_type &&
              ilkn_chnl_def_ptr->switch_data.oduk_data.op_state.map_state.input_map_count < 1) ||
            (DIGI_CPB_SWITCH == switch_type &&
             ilkn_chnl_def_ptr->switch_data.cpb_data.op_state.map_state.input_map_count < 1))))
        {
            /*
             * If the pointer is NULL or this is called from the ODUK side
             * but it is not ACTIVE, or called from CPB and CPB is not
             * active then this is an ERROR.
             */
            PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                    "ILKN Channel Pointer is Not Valid for SwitchType=%d\n",
                    switch_type);
        }
        else if (DIGI_ODUK_SWITCH == ilkn_ptr->next_resource_handle)
        {
            switch_ptr = &digi_resource_handle->oduksw_cfg[ilkn_ptr->next_handle_index];
        }
        else if (DIGI_CPB_SWITCH == ilkn_ptr->next_resource_handle)
        {
            switch_ptr = &digi_resource_handle->cpbsw_cfg[ilkn_ptr->next_handle_index];
        }
        if (NULL == switch_ptr ||
            ilkn_handle != switch_ptr->provisioned_handle_type ||
            ilkn_index != switch_ptr->provisioned_handle_index)
        {
             PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                     "ILKN Channel HANDLE failed sanity TEST\n");
        }
        else if (DIGI_DIRECTION_TX_RX == direction &&
                 (TRUE == switch_ptr->dest_activated &&
                  TRUE == switch_ptr->source_activated &&
                  LAST_DIGI_RESOURCE_HANDLE != switch_ptr->source_handle_type &&
                  LAST_DIGI_RESOURCE_HANDLE != switch_ptr->primary_dest_handle_type))
        {
            result = TRUE;
        }
        else if (DIGI_DIRECTION_TX_ONLY == direction &&
                 TRUE == switch_ptr->source_activated &&
                 LAST_DIGI_RESOURCE_HANDLE != switch_ptr->primary_dest_handle_type)
        {
            result = TRUE;
        }
        else if (DIGI_DIRECTION_RX_ONLY == direction &&
                 TRUE == switch_ptr->dest_activated &&
                 LAST_DIGI_RESOURCE_HANDLE != switch_ptr->source_handle_type)
        {
            result = TRUE;
        }
    }                                              

    if (TRUE == result &&
        switch_type != ilkn_ptr->next_resource_handle) 
    {
        /*
         * If the next resource is not the switch that ILKN check is called
         * from then need to continue checking. Otherwise there is nothing
         * else to check.
         */
        result = digi_resource_check_operational_level(digi_handle,
                                                       digi_resource_handle,
                                                       ilkn_ptr->next_resource_handle,
                                                       ilkn_ptr->next_handle_index,
                                                       switch_type,
                                                       direction);
    }

    PMC_RETURN(result);
} /* digi_resource_is_ilkn_operational */


/*******************************************************************************
*  digi_resource_check_operational_level
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Caller has determined the source or destination is valid from the 
*  perspective of checking. Functions determines the handle type and then
*  calls the appropriate function to check its operational level.
*
* INPUTS:
*   digi_handle          - Reference to the digi_handle
*   digi_resource_handle - Reference to the resource table.
*   handle_type          - Type of the handle to be examined.
*   handle_index         - Index of the handle to be examined.
*   switch_type          - Either CPB or ODUK switch and is the reference
*                          for the side of the switch that is initially 
*                          being examined. The switch_type side must be
*                          OPERATIONAL for this to be valid. The other side
*                          can be PROVISIONED and this be considered valid.
*   direction            - Direction in which the resource is being checked
*                          for its operational level.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   TRUE:  Resource is operational
*   FALSE: Otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_check_operational_level(digi_handle_t *digi_handle,
                                                    digi_resource_handle_summary_t *digi_resource_handle,
                                                    digi_resource_next_handle_t handle_type,
                                                    UINT16 handle_index,
                                                    digi_resource_next_handle_t switch_type,
                                                    digi_direction_check_t direction)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    switch (handle_type)
    {
      case DIGI_OTN_MAPPER:
        /*
         * The TRUE and FALSE flags indicate on which switch is the mapotn
         * check being initiated from. Only one side needs to carry on the
         * operational checking and it is the side that is set to FALSE. 
         * Initially the source and destination are expected to be the same
         * that is bi-directional data paths. That will require updating.
         */
        result = digi_resource_is_mapper_operational(digi_handle,
                                                     digi_resource_handle,
                                                     handle_index,
                                                     switch_type,
                                                     TRUE,
                                                     direction);

        if (TRUE == result)
        {
            if (DIGI_ODUK_SWITCH == switch_type)
            {
                switch_type = DIGI_CPB_SWITCH;
            }
            else
            {
                switch_type = DIGI_ODUK_SWITCH;
            }
            
            /*
             * If the first side was successful then look at the other side of
             * the MAPOTN channel.
             */
            result = digi_resource_is_mapper_operational(digi_handle,
                                                         digi_resource_handle,
                                                         handle_index,
                                                         switch_type,
                                                         FALSE,
                                                         direction);
        }

        break;

    case DIGI_LINE_SERDES_CFG:
      if (DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T ==
          digi_resource_handle->line_serdes_cfg[handle_index].chnl_handle_type)
      {
          result = digi_resource_is_enet_operational(digi_handle,
                                                     digi_resource_handle,
                                                     &digi_resource_handle->line_serdes_cfg[handle_index],
                                                     FALSE,
                                                     direction);

      } else if (DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T == 
                 digi_resource_handle->line_serdes_cfg[handle_index].chnl_handle_type)
      {
          result = digi_resource_is_cbrc_operational(digi_handle,
                                                     digi_resource_handle,
                                                     &digi_resource_handle->line_serdes_cfg[handle_index],
                                                     FALSE,
                                                     direction);
      }
      break;

    case DIGI_SYS_SERDES_CFG:      
      if (DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T ==
          digi_resource_handle->sys_serdes_cfg[handle_index].chnl_handle_type)
      {
          result = digi_resource_is_enet_operational(digi_handle,
                                                     digi_resource_handle,
                                                     &digi_resource_handle->sys_serdes_cfg[handle_index],
                                                     FALSE,
                                                     direction);
      } else if (DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T == 
          digi_resource_handle->sys_serdes_cfg[handle_index].chnl_handle_type)
      {
          result = digi_resource_is_cbrc_operational(digi_handle,
                                                     digi_resource_handle,
                                                     &digi_resource_handle->sys_serdes_cfg[handle_index],
                                                     FALSE,
                                                     direction);
      }
      break;

    case DIGI_SFI51_SERDES_CFG:      
      if (DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T ==
          digi_resource_handle->sfi51_serdes_cfg[handle_index].chnl_handle_type)
      {
          result = digi_resource_is_enet_operational(digi_handle,
                                                     digi_resource_handle,
                                                     &digi_resource_handle->sfi51_serdes_cfg[handle_index],
                                                     FALSE,
                                                     direction);
      } else if (DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T == 
          digi_resource_handle->sfi51_serdes_cfg[handle_index].chnl_handle_type)
      {
          result = digi_resource_is_cbrc_operational(digi_handle,
                                                     digi_resource_handle,
                                                     &digi_resource_handle->sfi51_serdes_cfg[handle_index],
                                                     FALSE,
                                                     direction);
      }
      break;

    case DIGI_LINE_OTN_HO_CONTAINER:
    case DIGI_LINE_OTN_MO_CONTAINER:
    case DIGI_LINE_OTN_LO_CONTAINER:
    case DIGI_SYS_OTN_HO_CONTAINER:
    case DIGI_SYS_OTN_MO_CONTAINER:
       result = digi_resource_is_otn_operational(digi_handle,
                                                 digi_resource_handle,
                                                 handle_type,
                                                 handle_index,
                                                 TRUE,
                                                 direction);
      break;

    case DIGI_ILKN_1:
    case DIGI_ILKN_2:
      result = digi_resource_is_ilkn_operational(digi_handle,
                                                 digi_resource_handle,
                                                 handle_type,
                                                 handle_index,
                                                 switch_type,
                                                 direction);
      break;

    default:
      break;
    }
    PMC_RETURN(result);
}  /*  digi_resource_check_operational_level */


/*******************************************************************************
*  digi_resource_find_secondary_check_op_level
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This is a broadcast/multicast case and it typically valid only in the TX
*  direction. The primary destination has returned false for being valid in
*  the TX direction. Need to recover the secondary port type from the
*  passed handle type and then determine whether or not it is valid.
*
* INPUTS:
*   digi_handle          - Reference to the digi_handle
*   digi_resource_handle - Reference to the resource table.
*   handle_type          - Type of the handle to be examined.
*   handle_index         - Index of the handle to be examined.
*   switch_type          - Either CPB or ODUK switch and is the reference
*                          for the side of the switch that is initially 
*                          being examined. The switch_type side must be
*                          OPERATIONAL for this to be valid. The other side
*                          can be PROVISIONED and this be considered valid.
*   direction            - Direction in which the resource is being checked
*                          for its operational level.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   TRUE:  Resource is operational
*   FALSE: Otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_find_secondary_check_op_level(digi_handle_t *digi_handle,
                                                          digi_resource_handle_summary_t *digi_resource_handle,
                                                          digi_resource_next_handle_t handle_type,
                                                          UINT16 handle_index,
                                                          digi_resource_next_handle_t switch_type,
                                                          digi_direction_check_t direction)
{
    BOOL8 result = FALSE;
    UINT32 switch_index;

    PMC_ENTRY();

    switch (handle_type)
    {
      case DIGI_ILKN_1:
        if (handle_index >= UTIL_GLOBAL_ILKN_CPB_DPI_CHNLS_MAX)
        {
            handle_index -= UTIL_GLOBAL_ILKN_CPB_DPI_CHNLS_MAX;
        }
        switch_index = DIGI_ODU_SWITCH_PORT0_OFFSET + handle_index;
        break;

      case DIGI_ILKN_2:
        if (handle_index >= UTIL_GLOBAL_ILKN_CPB_DPI_CHNLS_MAX)
        {
            handle_index -= UTIL_GLOBAL_ILKN_CPB_DPI_CHNLS_MAX;
        }
        switch_index = DIGI_ODU_SWITCH_PORT1_OFFSET + handle_index;
        break;

      case DIGI_LINE_OTN_HO_CONTAINER:
      case DIGI_LINE_OTN_MO_CONTAINER:
      case DIGI_LINE_OTN_LO_CONTAINER:
      case DIGI_SYS_OTN_HO_CONTAINER:
      case DIGI_SYS_OTN_MO_CONTAINER:
        switch_index = DIGI_ODU_SWITCH_PORT2_OFFSET + handle_index;
        break;

      case DIGI_OTN_MAPPER:
          switch_index = DIGI_ODU_SWITCH_PORT3_OFFSET + handle_index;
          break;

      default:
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0,  0,
                "Passed in handleType=%d is not a valid ODUK PortType\n",
                handle_type);
        PMC_RETURN(FALSE);
    }

    if (LAST_DIGI_RESOURCE_HANDLE == digi_resource_handle->oduksw_cfg[switch_index].associate_dest_handle_type ||
        handle_type ==  digi_resource_handle->oduksw_cfg[switch_index].associate_dest_handle_type)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0,  0,
                "Secondary Port handle=%d for Index=%d is either invalid or the same as Passed in handle=%d\n",
                digi_resource_handle->oduksw_cfg[switch_index].associate_dest_handle_type,
                switch_index,
                handle_type);
        PMC_RETURN(FALSE);
    }
    result = digi_resource_check_operational_level(digi_handle,
                                                   digi_resource_handle,
                                                   digi_resource_handle->oduksw_cfg[switch_index].associate_dest_handle_type,
                                                   digi_resource_handle->oduksw_cfg[switch_index].associate_dest_index,
                                                   switch_type,
                                                   direction);
    PMC_RETURN(result);
} /* digi_resource_find_secondary_check_op_level */

/*******************************************************************************
*  digi_resource_cpb_chnl_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle          - Reference to the digi_handle
*  digi_resource_handle - Reference to the resource table. Used if the
*                         input pointer cannot be found.
*  index                - Index into the resource table for the channel
*                         handle.
*  chnl_hndl_ptr        - Reference to the channel handle being deactivated
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_cpb_chnl_deactivate(digi_handle_t *digi_handle,
                                               digi_resource_handle_summary_t *digi_resource_handle,
                                               UINT32 index,
                                               util_global_switch_data_t *chnl_hndl_ptr)
{
    PMC_ERROR result;
    UINT32 ingress_mux_input_port = LAST_UTIL_GLOBAL_CPB_PORT;
    UINT32 ingress_mux_input_port_chnl = 0;
    BOOL8 is_multicast = FALSE;
    BOOL8 is_broadcast = FALSE;
    BOOL8 is_reader_primary = FALSE;
    BOOL8 is_slave_zone_primary = FALSE;
    util_global_switch_data_def_t *output_pc_data_ptr = NULL;
    util_global_switch_data_def_t *local_input_sw_ptr = NULL;
    util_global_cpb_port_data_t *local_output_pc_ptr = (util_global_cpb_port_data_t*)chnl_hndl_ptr;
    cpb_handle_t *cpb_handle = NULL;
    UINT32 cpb_output_port;
    util_global_switch_data_t *input_pc_ptr;

    PMC_ENTRY();

    result = digi_enet_enhanced_pmon_deactivate(digi_handle, 
                                                chnl_hndl_ptr);

    output_pc_data_ptr = (util_global_switch_data_def_t *)chnl_hndl_ptr;

    local_output_pc_ptr = &(output_pc_data_ptr->cpb_data);

    if (local_output_pc_ptr->port_type >= UTIL_GLOBAL_CPB_PORT_NOT_USED)
    {
        PMC_RETURN();
    }
    /* retrieve required CPB handle */
    cpb_handle = digi_handle->dcpb_handle;
    cpb_output_port = local_output_pc_ptr->port_type;

    result = cpb_chnl_source_type_and_primary_test(cpb_handle,
                                                   local_output_pc_ptr->port_type,
                                                   local_output_pc_ptr->channel,
                                                   &ingress_mux_input_port,
                                                   &ingress_mux_input_port_chnl,
                                                   &is_broadcast,
                                                   &is_multicast,
                                                   &is_reader_primary,
                                                   &is_slave_zone_primary);
    local_output_pc_ptr->op_state.active_state = UTIL_GLOBAL_CHNL_ACTIVE_STATE_INACTIVE;

    if (PMC_SUCCESS == result)
    {
        input_pc_ptr = digi_cpb_input_port_get(digi_handle, chnl_hndl_ptr);
        
        if (NULL == input_pc_ptr && 
            index < DIGI_CPB_SWITCH_TOTAL_CHANNELS &&
            LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->cpbsw_cfg[index].primary_dest_handle_type)
        {
            input_pc_ptr = digi_resource_get_input_pointer(digi_handle,
                                                           digi_resource_handle,
                                                           digi_resource_handle->cpbsw_cfg[index].primary_dest_handle_type,
                                                       digi_resource_handle->cpbsw_cfg[index].primary_dest_index);
        }
        
        if (NULL != input_pc_ptr)
        {
            local_input_sw_ptr = (util_global_switch_data_def_t*)input_pc_ptr; 
            
            if (NULL != local_input_sw_ptr)
            {
                local_input_sw_ptr->cpb_data.op_state.map_state.dest_active_count = 0;
            }
        }
    }

    if (PMC_SUCCESS == result)
    {
        result = cpb_chnl_deactivate(digi_handle->dcpb_handle,
                                     local_output_pc_ptr->port_type,
                                     local_output_pc_ptr->channel);
    }


    PMC_RETURN();
} /* digi_resource_cpb_chnl_deactivate */

/*******************************************************************************
*  digi_resource_cpb_chnl_demap
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle          - Reference to the digi_handle
*  digi_resource_handle - Reference to the resource table. Used if the
*                         input pointer cannot be found.
*  index                - CPB switch index used if the input pointer is 
*                         not found.
*  chnl_hndl_ptr        - Reference to the channel handle being deactivated
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_cpb_chnl_demap(digi_handle_t *digi_handle,
                                          digi_resource_handle_summary_t *digi_resource_handle,
                                          UINT32 index,
                                          util_global_switch_data_t *chnl_hndl_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    util_global_switch_data_def_t *output_pc_data_ptr = NULL;
    util_global_switch_data_t* input_pc_ptr;
    util_global_cpb_port_data_t *local_output_pc_ptr = (util_global_cpb_port_data_t*)chnl_hndl_ptr;
    cpb_handle_t *cpb_handle = NULL;
    UINT32 cpb_output_port;

    PMC_ENTRY();

    output_pc_data_ptr = (util_global_switch_data_def_t *)chnl_hndl_ptr;

    DIGI_CHNL_HANDLE_MAGIC_CHECK(output_pc_data_ptr);

    local_output_pc_ptr = &(output_pc_data_ptr->cpb_data);

    if (local_output_pc_ptr->port_type >= UTIL_GLOBAL_CPB_PORT_NOT_USED)
    {
        PMC_RETURN();
    }

    /* retrieve required CPB handle */
    cpb_handle = digi_handle->dcpb_handle;
    cpb_output_port = local_output_pc_ptr->port_type;

    /* We need to look up the input_chnl based on this output chnl. */
    input_pc_ptr = digi_cpb_input_port_get(digi_handle, chnl_hndl_ptr);

    local_output_pc_ptr->op_state.map_state.is_output_mapped = FALSE;

    if (NULL == input_pc_ptr && index < DIGI_CPB_SWITCH_TOTAL_CHANNELS &&
        LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->cpbsw_cfg[index].primary_dest_handle_type)
    {
        input_pc_ptr = digi_resource_get_input_pointer(digi_handle,
                                                       digi_resource_handle,
                                                       digi_resource_handle->cpbsw_cfg[index].primary_dest_handle_type,
                                                       digi_resource_handle->cpbsw_cfg[index].primary_dest_index);
    }

    
    if (NULL != input_pc_ptr)
    {
        util_global_cpb_port_data_t* local_input_pc_ptr = &(((util_global_switch_data_def_t*)input_pc_ptr)->cpb_data);

        result = digi_enet_enhanced_pmon_demap(digi_handle, input_pc_ptr);

        if (local_input_pc_ptr->op_state.map_state.input_map_count > 0)
        {
            local_input_pc_ptr->op_state.map_state.input_map_count--;
        }
        
        if (local_input_pc_ptr->op_state.map_state.input_map_count == 0) 
        {
            local_input_pc_ptr->op_state.map_state.is_input_mapped = FALSE;
        }
    }

    PMC_RETURN();
} /* digi_resource_cpb_chnl_demap */

/*******************************************************************************
*  digi_resource_oduksw_chnl_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle    - Reference to the digi_handle
*  output_pc_ptr  - Reference to the resource being deactivated.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_oduksw_chnl_deactivate(digi_handle_t *digi_handle,
                                                  util_global_switch_data_t *output_pc_ptr)
{
     PMC_ERROR result = PMC_SUCCESS;

    /* variable definition */
    UINT32 ingress_mux_input_port = UTIL_GLOBAL_ODUK_PORT_NOT_USED;
    UINT32 ingress_mux_input_port_chnl;
    BOOL dummy;
    util_global_switch_data_def_t *output_pc_data_ptr = NULL;
    util_global_oduk_port_data_t *local_output_pc_ptr = (util_global_oduk_port_data_t*)output_pc_ptr;
    util_global_switch_data_def_t *local_input_sw_ptr = NULL;
    util_global_switch_data_t *input_pc_ptr = NULL;
    
    PMC_ENTRY();

    output_pc_data_ptr = (util_global_switch_data_def_t *)output_pc_ptr;

    if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE !=
        output_pc_data_ptr->oduk_data.op_state.active_state)
    {
        PMC_RETURN();
    }
    DIGI_CHNL_HANDLE_MAGIC_CHECK(output_pc_data_ptr);
    local_output_pc_ptr = &(output_pc_data_ptr->oduk_data);

    if (local_output_pc_ptr->port_type >= UTIL_GLOBAL_ODUK_PORT_NOT_USED)
    {
        PMC_RETURN();
    }
    result = coreotn_oduksw_chnl_source_and_type_test(digi_handle->coreotn_handle,
                                                      local_output_pc_ptr->port_type,
                                                      local_output_pc_ptr->channel,
                                                      &ingress_mux_input_port,
                                                      &ingress_mux_input_port_chnl,
                                                      &dummy,
                                                      &dummy);
    /*
     * Ensure there is a valid ingress port value to ensure that the 
     * underlying functions do not assert.
     */
    if (ingress_mux_input_port < UTIL_GLOBAL_ODUK_PORT_NOT_USED)
    {
        input_pc_ptr = digi_oduksw_input_port_get(digi_handle, 
                                                  ingress_mux_input_port,
                                                  ingress_mux_input_port_chnl,
                                                  DIGI_ILKN_CHNL_TARGET_ODUK);
        
        local_input_sw_ptr = (util_global_switch_data_def_t*)input_pc_ptr; 
    }
    local_output_pc_ptr->op_state.active_state = UTIL_GLOBAL_CHNL_ACTIVE_STATE_INACTIVE;

    if (NULL != local_input_sw_ptr && local_input_sw_ptr->oduk_data.op_state.map_state.dest_active_count > 0)
    {
        local_input_sw_ptr->oduk_data.op_state.map_state.dest_active_count--;
    }
    
    PMC_RETURN();
} /* digi_resource_oduksw_chnl_deactivate */

/*******************************************************************************
*  digi_resource_oduksw_chnl_demap
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle          - Reference to the digi_handle
*  digi_resource_handle - Reference to the resource table. Used if the
*                         input pointer cannot be found.
*  index                - ODUK switch index used if the input pointer is 
*                         not found.
*  chnl_hndl_ptr        - Reference to the channel handle being deactivated
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_oduksw_chnl_demap(digi_handle_t *digi_handle,
                                             digi_resource_handle_summary_t *digi_resource_handle,
                                             UINT32 index,
                                             util_global_switch_data_t *chnl_hndl_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    /* variable definition */
    util_global_switch_data_def_t *output_pc_data_ptr = NULL;
    util_global_oduk_port_data_t *local_output_pc_ptr = NULL;
    util_global_switch_data_t* input_pc_ptr = NULL;
    util_global_oduk_port_data_t* local_input_pc_ptr = NULL;
    util_global_oduk_port_data_t local_input_pc;
    BOOL is_broadcast = FALSE;
    BOOL is_multicast = FALSE;

    PMC_ENTRY();

    output_pc_data_ptr = (util_global_switch_data_def_t *)chnl_hndl_ptr;

    if (FALSE == output_pc_data_ptr->oduk_data.op_state.map_state.is_input_mapped && 
        FALSE == output_pc_data_ptr->oduk_data.op_state.map_state.is_output_mapped)
    {
        PMC_RETURN();
    }
    DIGI_CHNL_HANDLE_MAGIC_CHECK(output_pc_data_ptr);
    local_output_pc_ptr = &(output_pc_data_ptr->oduk_data);
    local_input_pc.port_type = LAST_UTIL_GLOBAL_ODUK_PORT;
    result = coreotn_oduksw_chnl_source_and_type_test(digi_handle->coreotn_handle,
                                                      local_output_pc_ptr->port_type,
                                                      local_output_pc_ptr->channel,
                                                      (UINT32*)&(local_input_pc.port_type),
                                                      &(local_input_pc.channel),
                                                      &is_broadcast,
                                                      &is_multicast);

    /* Clear ODUKSW DB */
    (void) coreotn_oduksw_db_entry_clear(digi_handle->coreotn_handle,
                                         (coreotn_oduksw_output_port_type_t)local_output_pc_ptr->port_type,
                                         local_output_pc_ptr->channel);
    local_output_pc_ptr->op_state.map_state.is_output_mapped = FALSE;

    if (local_input_pc.port_type < LAST_UTIL_GLOBAL_ODUK_PORT)
    {
        /* We need to look up the input_chnl based on this output chnl. */
        input_pc_ptr = digi_oduksw_input_port_get(digi_handle,
                                                  local_input_pc.port_type,
                                                  local_input_pc.channel,
                                                  DIGI_ILKN_CHNL_TARGET_ODUK);
    }
        
    if (NULL == input_pc_ptr && index <DIGI_ODUK_SWITCH_TOTAL_CHANNELS && 
        LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->oduksw_cfg[index].source_handle_type)
    {
        input_pc_ptr = digi_resource_get_input_pointer(digi_handle,
                                                       digi_resource_handle,
                                                       digi_resource_handle->oduksw_cfg[index].source_handle_type,
                                                       digi_resource_handle->oduksw_cfg[index].source_handle_index);
    }

    if (NULL != input_pc_ptr)
    {    
        local_input_pc_ptr = &(((util_global_switch_data_def_t*)input_pc_ptr)->oduk_data);
        
        if (NULL != local_input_pc_ptr)
        {
            if (local_input_pc_ptr->op_state.map_state.input_map_count > 0)
            {
                local_input_pc_ptr->op_state.map_state.input_map_count--;
            }
            if (local_input_pc_ptr->op_state.map_state.input_map_count == 0) 
            {
                local_input_pc_ptr->op_state.map_state.is_input_mapped = FALSE;
            }                
        }
    }

    PMC_RETURN();
} /* digi_resource_oduksw_chnl_demap */


/*******************************************************************************
*  digi_resource_enet_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Deprovision a ENET LINE or SYS channel handle as well as deprovisioning
*  the associated serdes port. Resets the serdes ports to default values.
*  The deprovisioning is done only at the DIGI layer and includes resetting
*  the enet line/sys channel in  the data base. Does not deprovision the
*  resource at the enet subsystem or lower and expects that 
*  enet_handle_restart_init will be called afterwards to clean any that
*  is required at that level.
*
* INPUTS:
*  digi_handle         - Reference to the digi_handle
*  chnl_ctxt_pptr      - ENET resources being deprovisioned
*  port_ctxt_ptr       - Serdes Port handle used by the enet resource
*  port_uid            - Serdes port identifier
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_enet_deprov(digi_handle_t *digi_handle,
                                       digi_enet_client_chnl_t **chnl_ctxt_pptr,
                                       digi_serdes_port_t *port_ctxt_ptr,
                                       UINT32 port_uid)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    digi_enet_client_chnl_def_t *enet_chnl_handle_ptr;
    UINT8 i;

    PMC_ENTRY();

    enet_chnl_handle_ptr = (digi_enet_client_chnl_def_t *) (*chnl_ctxt_pptr);
    if (!((enet_chnl_handle_ptr->switch_data.cpb_data.port_type ==
           UTIL_GLOBAL_CPB_PORT_ENET_LINE ||
           enet_chnl_handle_ptr->switch_data.cpb_data.port_type ==
           UTIL_GLOBAL_CPB_PORT_ENET_SYS) &&
          enet_chnl_handle_ptr->switch_data.cpb_data.data_source == 0))
    {
        PMC_RETURN();
    }
    
    /* clear timing mode */
    enet_chnl_handle_ptr->timing_mode = LAST_DIGI_ENET_TX_TIMING_MODE;

    if (UTIL_GLOBAL_CPB_PORT_ENET_LINE == enet_chnl_handle_ptr->switch_data.cpb_data.port_type)
    {
        /* If it's a enet line mirror port -> use cpb dcs db 3 instead of 2 */
        BOOL enet_mirror_mode = digi_cpb_enet_mirror_chk(digi_handle,
                                                     (util_global_switch_data_def_t *)enet_chnl_handle_ptr);

        (void)digi_enet_line_db_deprov_cfg(digi_handle,
                                           enet_chnl_handle_ptr->switch_data.cpb_data.channel, enet_mirror_mode);
    }
    else
    {
        (void)digi_enet_sys_db_deprov_cfg(digi_handle,
                                          enet_chnl_handle_ptr->switch_data.cpb_data.channel);
    }
        
    result = digi_serdes_port_lifd_sifd_subsytem_disable(digi_handle, 
                                                         port_uid);

    for (i = 0; i < DIGI_SERDES_DSI_LANES_MAX; i++)
    {
        if (UTIL_GLOBAL_CPB_PORT_ENET_LINE == enet_chnl_handle_ptr->switch_data.cpb_data.port_type)
        {
            if (digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].port_uid == port_uid)
            {
                digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].prov_state = DIGI_SERDES_PROV_STATE_UNASSIGNED;
                digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].map_type = DIGI_MAPPER_ALLOC_UNUSED;
                digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].num_odu0 = 0;
            }
        }
        else
        {
            if (digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[i].port_uid == port_uid)
            {
                digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[i].prov_state = DIGI_SERDES_PROV_STATE_UNASSIGNED;
                digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[i].map_type = DIGI_MAPPER_ALLOC_UNUSED;
                digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[i].num_odu0 = 0;
            }
        }
    }

    (void)digi_serdes_port_rate_state_set(digi_handle,
                                          port_ctxt_ptr,
                                          LAST_DIGI_SERDES_PORT_MODE,
                                          DIGI_SERDES_PROV_STATE_UNASSIGNED,
                                          DIGI_SERDES_PORT_RATE_STATE_DEPROV);
    
    digi_dcsu_conditional_uninit(digi_handle,
                                 DIGI_DCSU_INST_ENET1);
    digi_dcsu_conditional_uninit(digi_handle,
                                 DIGI_DCSU_INST_ENET2);
    digi_chnl_ptr_initialize((util_global_switch_data_def_t *) *(chnl_ctxt_pptr));
    *chnl_ctxt_pptr = NULL;
    PMC_RETURN();
} /* digi_resource_enet_deprov */



/*******************************************************************************
*  digi_resource_cbrc_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Deprovision a ENET LINE or SYS channel handle as well as deprovisioning
*  the associated serdes port. Resets the serdes ports to default values.
*  The deprovisioning is done only at the DIGI layer and includes resetting
*  the enet line/sys channel in  the data base. Does not deprovision the
*  resource at the enet subsystem or lower and expects that 
*  enet_handle_restart_init will be called afterwards to clean any that
*  is required at that level.
*
* INPUTS:
*  digi_handle         - Reference to the digi_handle
*  chnl_ctxt_pptr      - ENET resources being deprovisioned
*  port_ctxt_ptr       - Serdes Port handle used by the enet resource
*  port_uid            - Serdes port identifier
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_cbrc_deprov(digi_handle_t *digi_handle,
                                       digi_enet_client_chnl_t **chnl_ctxt_pptr,
                                       digi_serdes_port_t *port_ctxt_ptr,
                                       UINT32 port_uid)
{
    digi_cbr_client_chnl_def_t *cbr_chnl_handle_ptr = (digi_cbr_client_chnl_def_t *) (*chnl_ctxt_pptr);
    UINT32 i = 0;
    PMC_ENTRY();
    
    if (cbr_chnl_handle_ptr->switch_data.cpb_data.port_type !=
        UTIL_GLOBAL_CPB_PORT_CBRC)
    {
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_PORT_TYPE, 0, 0);
    }

    (void)digi_cbrc_db_deprov_cfg(digi_handle,
                                  cbr_chnl_handle_ptr->switch_data.cpb_data.channel);
      
    (void)digi_serdes_port_rate_state_set(digi_handle,
                                          port_ctxt_ptr,
                                          LAST_DIGI_SERDES_PORT_MODE,
                                          DIGI_SERDES_PROV_STATE_UNASSIGNED,
                                          DIGI_SERDES_PORT_RATE_STATE_DEPROV);

    for (i = 0; i < DIGI_SERDES_DSI_LANES_MAX; i++)
    {
        if (digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].port_uid == port_uid)
        {
            digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].prov_state = DIGI_SERDES_PROV_STATE_UNASSIGNED;
            digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].map_type = DIGI_MAPPER_ALLOC_UNUSED;
            digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].num_odu0 = 0;
        }
    }


    digi_chnl_ptr_initialize((util_global_switch_data_def_t *) *(chnl_ctxt_pptr));
    *chnl_ctxt_pptr = NULL;

    PMC_RETURN();
} /* digi_resource_cbrc_deprov */


/*******************************************************************************
*  digi_resource_otn_mapper_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle     - Reference to the digi_handle
*  chnl_ctxt_pptr  - Reference to the mapper handle being deprovisioned
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_otn_mapper_deprov(digi_handle_t *digi_handle,
                                             digi_mapper_chnl_t **chnl_ctxt_pptr)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    BOOL8 is_umbilical_case = FALSE;
    UINT16 oduk_chnl;

    PMC_ENTRY();

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(*chnl_ctxt_pptr);

    DIGI_CHNL_HANDLE_MAGIC_CHECK(mapper_chnl_handle_ptr);

    is_umbilical_case = digi_enet_is_enhanced_pmon_umbilical_case(digi_handle, 
                                                                  mapper_chnl_handle_ptr, 
                                                                  mapper_chnl_handle_ptr->enet_pmon);

    oduk_chnl = mapper_chnl_handle_ptr->switch_data.oduk_data.channel;

    if (TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.s4_odu_struct[oduk_chnl].switch_data.header.prov_state))
    {
        digi_chnl_ptr_initialize((util_global_switch_data_def_t *)&digi_handle->handle_pool.s4_odu_struct[oduk_chnl].switch_data);
    }

    if((UTIL_GLOBAL_CPB_PORT_ENET_LINE == mapper_chnl_handle_ptr->switch_data.cpb_data.port_type &&
        1 == mapper_chnl_handle_ptr->switch_data.cpb_data.data_source) ||
       TRUE == is_umbilical_case)
    {
        if(digi_handle->var.etrans_dcpb_use_model == FALSE)
        {
            digi_num_odus_and_map_type_deprov_set(digi_handle,
                                                  mapper_chnl_handle_ptr->switch_data.cpb_data.port_type,
                                                  mapper_chnl_handle_ptr->mapotn_chnl);
        } 
        else 
        {
            digi_num_odus_and_map_type_etrans_deprov_set(digi_handle,
                                                         mapper_chnl_handle_ptr->switch_data.cpb_data.port_type,
                                                         mapper_chnl_handle_ptr->switch_data.cpb_data.channel);
        }
        /* Shut off ENET1/2 DCSU if ENET is in start state */
        digi_dcsu_conditional_uninit(digi_handle, DIGI_DCSU_INST_ENET1);
        digi_dcsu_conditional_uninit(digi_handle, DIGI_DCSU_INST_ENET2);
        digi_dcsu_conditional_uninit(digi_handle, DIGI_DCSU_INST_ENET3);
    }
    else if(mapper_chnl_handle_ptr->switch_data.cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_MAPOTN &&
            mapper_chnl_handle_ptr->switch_data.cpb_data.data_source == 1 )
    {
        digi_num_odus_and_map_type_deprov_set(digi_handle,
                                              mapper_chnl_handle_ptr->switch_data.cpb_data.port_type,
                                              mapper_chnl_handle_ptr->mapotn_chnl);
    }

    result = digi_mapper_db_deprov_cfg(digi_handle,
                                       mapper_chnl_handle_ptr->switch_data.cpb_data.port_type,
                                       mapper_chnl_handle_ptr->mapotn_chnl,
                                       mapper_chnl_handle_ptr->switch_data.cpb_data.channel, 
                                       is_umbilical_case);

    digi_chnl_ptr_initialize((util_global_switch_data_def_t*)*(chnl_ctxt_pptr));
    mapper_chnl_handle_ptr->enet_pmon = DIGI_NO_PMON;
    mapper_chnl_handle_ptr->enet_pmon_data = NULL;

    *chnl_ctxt_pptr = NULL;     

    PMC_RETURN();
} /*digi_resource_otn_mapper_deprov*/


/*******************************************************************************
*  digi_resource_otn_server_line_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle       - Reference to the digi_handle
*  chnl_lineotn_pptr - Reference to the lineotn handle being deprovisioned
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_otn_server_line_deprov(digi_handle_t *digi_handle,
                                                  digi_otn_server_chnl_t **chnl_lineotn_pptr)
{
    PMC_ERROR   result = PMC_SUCCESS;
    
    digi_otn_server_chnl_def_t* otn_server_ptr=NULL;
    digi_post_mld_dsi_ctxt_t *p_post_mld_lanes = NULL;
    digi_serdes_port_t* port_ctxt_ptr = NULL;
    digi_serdes_prov_state_t prov_state;
    BOOL8 is_sfi_chnl = FALSE;

    PMC_ENTRY();

    otn_server_ptr = (digi_otn_server_chnl_def_t*)(*chnl_lineotn_pptr);

    /*
     * Sanity check. if this is a SFI serdes dsi type then verify that the
     * channel is 0 or 17 based on the SFI serdes type. Otherwise ensure 
     * that the dci_chnl or the dsi_base_chnl does not exceed the maximum 
     * value and for that matter the dsi_serdes_type is valid. Otherwise 
     * lets get out before going any farther.
     */
    if ((otn_server_ptr->dsi_serdes_type == LINE_OTN_DSI_TYPE_SFI_51_A &&
         otn_server_ptr->dsi_base_chnl == 0) ||
        (otn_server_ptr->dsi_serdes_type == LINE_OTN_DSI_TYPE_SFI_51_B &&
         otn_server_ptr->dsi_base_chnl == 17))
    {
        if (otn_server_ptr->dci_chnl < DIGI_SERDES_XFI_LANES_MAX)
        {
            is_sfi_chnl = TRUE;
        }
    }

    if (FALSE == is_sfi_chnl &&
        (otn_server_ptr->dci_chnl >= DIGI_SERDES_XFI_LANES_MAX ||
        otn_server_ptr->dsi_base_chnl >= DIGI_SERDES_XFI_LANES_MAX ||
         otn_server_ptr->dsi_serdes_type >= (UINT8)LAST_LINE_OTN_DSI_TYPE))
    {
        PMC_RETURN();
    }
    port_ctxt_ptr = digi_port_ctxt_no_type_find(digi_handle, 
                                                otn_server_ptr->port_uid);

    if (port_ctxt_ptr == NULL) 
    {
        PMC_RETURN();

    } 
    result = digi_by_port_uid_post_mld_prov_state_get(digi_handle, 
                                                      otn_server_ptr->port_uid,
                                                      &prov_state);

    if (prov_state != DIGI_SERDES_PROV_STATE_LINEOTN)
    {
        PMC_RETURN();
    }

    if (prov_state == DIGI_SERDES_PROV_STATE_LINEOTN && 
        TRUE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle))
    {
        (void) coreotn_db_ho_entry_clear(digi_handle->coreotn_handle,
                                         UTIL_GEN_DB_LINEOTN_SYSOTN_MODE,
                                         otn_server_ptr->dci_chnl);        
    }
    else
    {
        (void) coreotn_db_ho_entry_clear(digi_handle->coreotn_handle,
                                         UTIL_GEN_DB_LINE_CORE_OTN_RX_DB,
                                         otn_server_ptr->dci_chnl);
        (void) coreotn_db_ho_entry_clear(digi_handle->coreotn_handle,
                                         UTIL_GEN_DB_LINE_CORE_OTN_TX_DB,
                                         otn_server_ptr->dci_chnl);
    }

    /* last step - update the prov state with unassigned status */
    p_post_mld_lanes = digi_first_post_mld_dsi_find(digi_handle,
                                                    otn_server_ptr->port_uid);
            
    if(p_post_mld_lanes != NULL && 
       DIGI_SERDES_PROV_STATE_LINEOTN == p_post_mld_lanes->prov_state)
    {
        result = digi_serdes_port_lifd_sifd_subsytem_disable(digi_handle,
                                                             otn_server_ptr->port_uid);
        /* if this is lineotn port - unassign it  */
        result = digi_post_mld_prov_state_set(digi_handle,
                                              otn_server_ptr->port_uid,
                                              DIGI_SERDES_PROV_STATE_UNASSIGNED); 

        /* reset digi_otn_server_chnl_t memeory */
        otn_server_ptr->header.prov_state = UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG;
        *chnl_lineotn_pptr = NULL;
        digi_dcsu_conditional_uninit(digi_handle, DIGI_DCSU_INST_OTN2);
    }

    /* Update serdes por state */    
    (void)digi_serdes_port_rate_state_set(digi_handle,
                                          port_ctxt_ptr,
                                          LAST_DIGI_SERDES_PORT_MODE,
                                          DIGI_SERDES_PROV_STATE_UNASSIGNED,
                                          DIGI_SERDES_PORT_RATE_STATE_DEPROV);
    

    PMC_RETURN();
} /* digi_resource_otn_server_line_deprov */

/*******************************************************************************
*  digi_resource_otn_server_sys_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle       - Reference to the digi_handle
*  chnl_lineotn_pptr - Reference to the sysotn handle to be deprovisioned
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_otn_server_sys_deprov(digi_handle_t *digi_handle,
                                                 digi_otn_server_chnl_t **chnl_lineotn_pptr)
{
    PMC_ERROR   result = PMC_SUCCESS;
    
    digi_otn_server_chnl_def_t* otn_server_ptr=NULL;
    digi_post_mld_dsi_ctxt_t *p_post_mld_lanes = NULL;
    digi_serdes_port_t* port_ctxt_ptr = NULL;
    digi_serdes_prov_state_t prov_state;

    PMC_ENTRY();

    otn_server_ptr = (digi_otn_server_chnl_def_t*)(*chnl_lineotn_pptr);

    if (NULL == otn_server_ptr ||
        (otn_server_ptr->dsi_base_chnl >= DIGI_SERDES_XFI_LANES_MAX ||
         otn_server_ptr->dsi_serdes_type >= (UINT8)LAST_LINE_OTN_DSI_TYPE))
    {
        PMC_RETURN();
    }
    port_ctxt_ptr = digi_port_ctxt_no_type_find(digi_handle, 
                                                otn_server_ptr->port_uid);

    if (port_ctxt_ptr == NULL) 
    {
        PMC_RETURN();
    } 
    result = digi_by_port_uid_post_mld_prov_state_get(digi_handle, 
                                                      otn_server_ptr->port_uid,
                                                      &prov_state);

    if (prov_state != DIGI_SERDES_PROV_STATE_SYSOTN)
    {
        PMC_RETURN();
    }
    (void) coreotn_db_ho_entry_clear(digi_handle->coreotn_handle,
                                     UTIL_GEN_DB_LINEOTN_SYSOTN_MODE,
                                     otn_server_ptr->dci_chnl);        

    /* last step - update the prov state with unassigned status */
    p_post_mld_lanes = digi_first_post_mld_dsi_find(digi_handle,
                                                    otn_server_ptr->port_uid);
            
    if (p_post_mld_lanes != NULL)
    {
        result = digi_serdes_port_lifd_sifd_subsytem_disable(digi_handle,
                                                             otn_server_ptr->port_uid);
        /* if this is lineotn port - unassign it  */
        result = digi_post_mld_prov_state_set(digi_handle,
                                              otn_server_ptr->port_uid,
                                              DIGI_SERDES_PROV_STATE_UNASSIGNED); 


        /* reset digi_otn_server_chnl_t memeory */
        otn_server_ptr->header.prov_state = UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG;
        *chnl_lineotn_pptr = NULL;
    }

    (void)digi_serdes_port_rate_state_set(digi_handle,
                                          port_ctxt_ptr,
                                          LAST_DIGI_SERDES_PORT_MODE,
                                          DIGI_SERDES_PROV_STATE_UNASSIGNED,
                                          DIGI_SERDES_PORT_RATE_STATE_DEPROV);

    PMC_RETURN();
} /* digi_resource_otn_server_sys_deprov */

/*******************************************************************************
*  digi_resource_otn_server_ho_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle           - Reference to the digi_handle
*  ho_odu_chnl_ctxt_pptr - High order channel being deprovisioned.
*  otn_server_pptr       - Reference to lineotn handle parent of HO handle,
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_otn_server_ho_deprov(digi_handle_t *digi_handle,
                                                digi_otn_odu_chnl_t  **ho_odu_chnl_ctxt_pptr,
                                                digi_otn_server_chnl_t **otn_server_pptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    digi_otn_server_chnl_def_t *otn_server_ptr = NULL;
    odu_struct_t *ho_odu_struct_ptr = NULL;
    UINT32 ho_chnl_id;
    BOOL8 is_src_sysotn_server;

    PMC_ENTRY();

    otn_server_ptr = (digi_otn_server_chnl_def_t *)(*otn_server_pptr);
    ho_odu_struct_ptr = (odu_struct_t*)(*ho_odu_chnl_ctxt_pptr);

    /* deactivate odu_struct */
    ho_odu_struct_ptr->active = FALSE;

    /* 
     * first look up to see if the server is sourced from LINEOTN or SYSOTN
     */
    is_src_sysotn_server = otn_server_ptr->sys_line_src;

    /* retrieve ho dci channel id */
    ho_chnl_id = otn_server_ptr->dci_chnl;

    /* deactivate lineotn OTU channel if it is terminated */
    if (ho_odu_struct_ptr->mem_ptr->payload_format != UTIL_GLOBAL_ODU_UNCHANNELIZED) 
    {
        /* in multi-lane configuration, we deactivate the multi-lane aligner */
        if( (lineotn_dsi_type_t)otn_server_ptr->dsi_serdes_type != LINE_OTN_DSI_TYPE_OTUK ) 
        {
            result = digi_otn_acb_ckctl_cfg(digi_handle, otn_server_ptr,DIGI_SERDES_DEACTIVATE);            
            result = digi_otn_acb_ckctl_cfg(digi_handle, otn_server_ptr,DIGI_SERDES_DEPROV);         
        }
    }        
    else
    {
        odu_struct_t *s3b_odu_struct_ptr = &(digi_handle->handle_pool.s3b_odu_struct[ho_chnl_id]);
        /* deactivate the 3b odu_struct */
        digi_chnl_ptr_initialize((util_global_switch_data_def_t*)s3b_odu_struct_ptr);
    }
    /* set the reference to the odu struct to NULL in the lineotn channel handle */
    otn_server_ptr->odu_struct_ptr = NULL;
    
    digi_chnl_ptr_initialize((util_global_switch_data_def_t*)ho_odu_struct_ptr);
    *ho_odu_chnl_ctxt_pptr = NULL;

    if (FALSE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle) ||
        (TRUE == is_src_sysotn_server && TRUE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle)))
    { 
        /* Update the channel payload context */
        digi_handle->var.odu_chnl_payload[ho_chnl_id] = LAST_DIGI_ODU_PAYLOAD_FORMAT;
    }
    else
    {
        odu_struct_t* sysline_ctxt_ptr = &(digi_handle->handle_pool.sysotn_line_struct[ho_chnl_id]);

         if (sysline_ctxt_ptr == NULL) 
         {             
             PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);             
         }
         else
         {
             digi_chnl_ptr_initialize((util_global_switch_data_def_t*)sysline_ctxt_ptr);             
         }
    }

    PMC_RETURN();
} /* digi_resource_otn_server_ho_deprov */


/*******************************************************************************
*  digi_resource_otn_server_lo_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle          - Reference to the digi_handle
*  digi_resource_handle - Reference to the resource table.
*  oduk_index           - Index of LO channel in ODUK switch table. Used if
*                         resource needs to be deactivated or demapped
*                         before being deprovisioned.
*  lo_odu_chnl_ctxt_pptr - Reference to the LO channel being deprovisioned
*  ho_odu_chnl_ctxt_pptr - Reference High order parent.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_otn_server_lo_deprov(digi_handle_t *digi_handle,
                                                digi_resource_handle_summary_t *digi_resource_handle,
                                                UINT32 oduk_index,
                                                digi_otn_odu_chnl_t  **lo_odu_chnl_ctxt_pptr,
                                                digi_otn_odu_chnl_t **ho_odu_chnl_ctxt_pptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    /* variable declaration */     
    coreotn_mux_stage_t mux_stage = LAST_COREOTN_MUX_STAGE;
    odu_struct_t *ho_odu_struct_ptr = NULL;
    odu_struct_t *lo_odu_struct_ptr = NULL;
    UINT32 ho_chnl_id;
    UINT32 lo_chnl_id;
    UINT32 i;

    PMC_ENTRY();
    /* retrieve the odu_struct for this ho channel */
    ho_odu_struct_ptr = (odu_struct_t*)(*ho_odu_chnl_ctxt_pptr);

    lo_odu_struct_ptr = (odu_struct_t*)(*lo_odu_chnl_ctxt_pptr);

    /*
     * If there are no lo order channels hanging off of this higher order
     * then there is nothing to do.
     */
    if (lo_odu_struct_ptr->mem_ptr->payload_format != UTIL_GLOBAL_ODU_UNCHANNELIZED &&
        (lo_odu_struct_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_rx != 0 ||
        lo_odu_struct_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_tx != 0 ))
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "LO ODU Struct Channel=%d PayloadFormat=%d is not UNCHANNELIZED or RxCount=%d or TxCount=%d is not 0!!!\n",
                lo_odu_struct_ptr->mem_ptr->chnl_id,
                (UINT32)lo_odu_struct_ptr->mem_ptr->payload_format,
                lo_odu_struct_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_rx,
                lo_odu_struct_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_tx);
        PMC_RETURN();
    }  

    /*
     * Whether or not something is mapped is a function of the direction
     * in which it is being checked. It is possible that the resource
     * connected to this resource has been lost within the data path and as
     * such this resource was never demapped or deactivated. Check the
     * internal state before releasing to ensure it is deactivated and 
     * demapped before it is released
     */
    if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == lo_odu_struct_ptr->switch_data.oduk_data.op_state.active_state)
    {
        digi_resource_oduksw_chnl_deactivate(digi_handle,
                                             lo_odu_struct_ptr);
    }

    if (TRUE == lo_odu_struct_ptr->switch_data.oduk_data.op_state.map_state.is_output_mapped)
    {
        digi_resource_oduksw_chnl_demap(digi_handle,
                                        digi_resource_handle,
                                        oduk_index,
                                        lo_odu_struct_ptr);

    }

    /* retrieve ho dci channel id */
    ho_chnl_id = ho_odu_struct_ptr->mem_ptr->chnl_id;
    lo_chnl_id = lo_odu_struct_ptr->mem_ptr->chnl_id;
    
    if (ho_odu_struct_ptr->mem_ptr->odu_level == ODU_STRUCT_LEVEL_HO_ODU)
    {     
        /* deprov mo channel */
        mux_stage = COREOTN_MUX_STAGE_ONE;
    }
    else if (ho_odu_struct_ptr->mem_ptr->odu_level == ODU_STRUCT_LEVEL_MO_ODU)
    {
        /* deprov lo channels */
        mux_stage = COREOTN_MUX_STAGE_TWO;
    }

    if (COREOTN_MUX_STAGE_ONE == mux_stage)
    {
        digi_handle->var.mo_prov_state_tx[lo_chnl_id] = FALSE;
        digi_handle->var.mo_prov_state_rx[lo_chnl_id] = FALSE;
        /* update payload_mode for MO channel to release */
        digi_handle->var.odu_chnl_payload[DIGI_NUM_HO_ODU_CHNL_MAX+lo_chnl_id] = LAST_DIGI_ODU_PAYLOAD_FORMAT;        
    }
    else if (COREOTN_MUX_STAGE_TWO == mux_stage)
    {
        /* update payload mode for LO channel to release */
        digi_handle->var.odu_chnl_payload[DIGI_NUM_HO_ODU_CHNL_MAX+DIGI_NUM_MO_ODU_CHNL_MAX+lo_chnl_id] = LAST_DIGI_ODU_PAYLOAD_FORMAT;
    }     
 
    if (mux_stage == COREOTN_MUX_STAGE_TWO
        || (mux_stage == COREOTN_MUX_STAGE_ONE && lo_odu_struct_ptr->mem_ptr->payload_format == UTIL_GLOBAL_ODU_UNCHANNELIZED))
    {
        odu_struct_t *s3b_odu_struct_ptr = &(digi_handle->handle_pool.s3b_odu_struct[lo_chnl_id]);
        /* deactivate the 3b odu_struct */
        digi_chnl_ptr_initialize((util_global_switch_data_def_t*)s3b_odu_struct_ptr);
        s3b_odu_struct_ptr = NULL;
    }
   
    /* update the parent odu struct */
    if (ho_odu_struct_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_rx > 0)
    {
        ho_odu_struct_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_rx--;
    }
    if (ho_odu_struct_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_tx > 0)
    {
        ho_odu_struct_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_tx--;
    }
   
    /* free the channel id */
    for (i = 0; i < 80; i++)
    {
        if (ho_odu_struct_ptr->mem_ptr->trib_slot_ctxt.sub_chnl_ids_tx[i] == lo_chnl_id)
        {
            ho_odu_struct_ptr->mem_ptr->trib_slot_ctxt.sub_chnl_ids_tx[i] = DIGI_DCI_CHANNEL_UNASSIGNED;
            /* also free tribs_port */
            ho_odu_struct_ptr->mem_ptr->trib_slot_ctxt.tx_trib_port[i] = DIGI_TRIBPORT_UNUSED;
        }

        if (ho_odu_struct_ptr->mem_ptr->trib_slot_ctxt.sub_chnl_ids_rx[i] == lo_chnl_id)
        {
            ho_odu_struct_ptr->mem_ptr->trib_slot_ctxt.sub_chnl_ids_rx[i] = DIGI_DCI_CHANNEL_UNASSIGNED;
            /* also free tribs_port */
            ho_odu_struct_ptr->mem_ptr->trib_slot_ctxt.rx_trib_port[i] = DIGI_TRIBPORT_UNUSED;
        }
    }    

    if (COREOTN_MUX_STAGE_ONE == mux_stage)
    {
        result = coreotn_db_mo_entry_update(digi_handle->coreotn_handle, 
                                           UTIL_GEN_DB_LINE_CORE_OTN_RX_DB,
                                            lo_chnl_id);
        
        result = coreotn_db_mo_entry_update(digi_handle->coreotn_handle, 
                                            UTIL_GEN_DB_LINE_CORE_OTN_TX_DB,
                                            lo_chnl_id);
    }
    else if (COREOTN_MUX_STAGE_TWO == mux_stage)
    {
        result = coreotn_db_lo_entry_update(digi_handle->coreotn_handle, 
                                            UTIL_GEN_DB_LINE_CORE_OTN_RX_DB,
                                            lo_chnl_id);
            
        result = coreotn_db_lo_entry_update(digi_handle->coreotn_handle,
                                            UTIL_GEN_DB_LINE_CORE_OTN_TX_DB,
                                            lo_chnl_id);
    }
    if (digi_handle->var.otn_cfg_mode == DIGI_OTN_DIRECT_DEVICE_CONFIG) 
    {
        digi_chnl_ptr_initialize((util_global_switch_data_def_t*)lo_odu_struct_ptr);
        lo_odu_struct_ptr->active = FALSE;
        *lo_odu_chnl_ctxt_pptr = NULL;
    }

    PMC_RETURN();
} /* digi_resource_otn_server_lo_deprov */


/*******************************************************************************
* digi_resource_restart_release_switch_chnl
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *digi_resource_handle  - Resource handle table that has already been 
*                            built
*   next_handle            - Resource handle type value of what the 
*                            serdes_connection_handle is connected to.
*   next_index             - Indice of the next handle in the digi 
*                            resource table.
*   is_oduk_to_cpb         - Represents the perspective of who is calling
*                            this function. If TRUE then called from ODUK 
*                            to CPB direction. False otherwise.
*   direction              - Direction of the release.
*
* OUTPUTS:
*  None
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_restart_release_switch_chnl(digi_handle_t *digi_handle,
                                                      digi_resource_handle_summary_t *digi_resource_handle,
                                                      digi_resource_next_handle_t next_handle,
                                                       UINT16 next_index,
                                                       BOOL8 is_oduk_to_cpb,
                                                       digi_direction_check_t direction)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    util_global_switch_data_t *prov_ptr = NULL;
    UINT16 prov_index;
    BOOL8 is_mapper = FALSE;
    BOOL8 is_ilkn_handle = FALSE;
    BOOL8 check_dest = FALSE;
    BOOL8 check_src = FALSE;

    PMC_ENTRY();

    /*
     * Below is a listing of what the next handle from a serdes port can
     * be connected to. Not sure if a CPB_SWITCH is valid.
     * Line HO is used from LINEOTN is a normal line application.
     * Line LO is used from LINEOTN in a SYSOTN Application
     * SYS HO is used by SYSOTN in a SYSOTN application
     * If the next_resource_handle in the current next handle is LAST_DIGI
     * RESOURCE HANDLE then the current resource is not mapped or active. 
     * Need to deprovision the current resource only.
     */
    switch (next_handle)
    {
      case DIGI_ODUK_SWITCH:
        /*
         * The caller is connected to the ODUK switch. In this case we are
         * at a minimum mapped and likely activated as well. Need to
         * determine to whom the caller is connected and what operations
         * are required.
         */
        prov_index = digi_resource_handle->oduksw_cfg[next_index].provisioned_handle_index;
        PMC_LOG_TRACE("ODUK Switch Next Index=%d, Provisioned Index=%d\n",
                      next_index, prov_index);
        PMC_LOG_TRACE("ODUK Switch and ProvHandleType = %d\n", digi_resource_handle->oduksw_cfg[next_index].provisioned_handle_type);
                      

        switch(digi_resource_handle->oduksw_cfg[next_index].provisioned_handle_type)
        {
          case DIGI_LINE_OTN_HO_CONTAINER:
            prov_ptr =  digi_resource_handle->line_otn_ho_container_cfg[prov_index].chnl_handle_ptr;

            if (FALSE == is_oduk_to_cpb)
            {
                is_mapper = TRUE;
            }
            break;
            
          case DIGI_LINE_OTN_MO_CONTAINER:
            prov_ptr =  digi_resource_handle->line_otn_mo_container_cfg[prov_index].chnl_handle_ptr;
            break;
            
          case DIGI_LINE_OTN_LO_CONTAINER:
            prov_ptr = digi_resource_handle->line_otn_lo_container_cfg[prov_index].chnl_handle_ptr;
            break;
            
          case DIGI_SYS_OTN_HO_CONTAINER:
            prov_ptr =  digi_resource_handle->sys_otn_ho_container_cfg[prov_index].chnl_handle_ptr;
            /*
             * If coming from the CPB side want to ensure that do not
             * go around again. This should be the end of the data path.
             */
            if (FALSE == is_oduk_to_cpb)
            {
                is_ilkn_handle = TRUE;
            }
            break;

          case DIGI_SYS_OTN_MO_CONTAINER:
            prov_ptr =  digi_resource_handle->sys_otn_mo_container_cfg[prov_index].chnl_handle_ptr;
            /*
             * If coming from the CPB side want to ensure that do not
             * go around again. This should be the end of the data path.
             */
            if (FALSE == is_oduk_to_cpb)
            {
                is_ilkn_handle = TRUE;
            }
            break;
            
          case DIGI_ILKN_1:
            prov_ptr =  digi_resource_handle->ilkn_chnl_cfg[0][prov_index].ilkn_chnl_handle_ptr;
            /*
             * ILKN 1 and 2 are the end of the line so the evaluation of
             * what is activated or mapped is the same as MAPOTN across 
             * the ODUK. Set the mapper flag so that it is evaluated as
             * if this is a MAPOTN component
             */
            is_mapper = TRUE;
            is_ilkn_handle = TRUE;
            break;
            
          case DIGI_ILKN_2:
            prov_ptr =  digi_resource_handle->ilkn_chnl_cfg[1][prov_index].ilkn_chnl_handle_ptr;
            is_mapper = TRUE;
            is_ilkn_handle = TRUE;
            break;

        case DIGI_OTN_MAPPER:
            prov_ptr =  digi_resource_handle->mapper_cfg[prov_index].mapper_handle_ptr;
            is_mapper = TRUE;

            if (FALSE == is_oduk_to_cpb)
            {
                is_mapper = FALSE;
            }
          break;

          default:
            PMC_RETURN();
            break;
        }
        /*
         * If the prov_ptr is NULL and digi_resource_handle->force_release is TRUE then this
         * data path is connected to another serdes terminated connection
         * that was already cleaned so simply return.  Otherwise want to
         * catch this error so assert.
         */
        if (NULL == prov_ptr)
        {
            if (TRUE == digi_resource_handle->force_release)
            {
                PMC_RETURN();
            }
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        }

        if (FALSE  == is_ilkn_handle)
        {
            digi_resource_check_src_dest(digi_handle,
                                         digi_resource_handle,
                                         TRUE,
                                         is_oduk_to_cpb,
                                         next_index,
                                         direction,
                                         &check_dest,
                                         &check_src);
        }

        if (TRUE == digi_resource_evaluate_activate(digi_handle,
                                                    digi_resource_handle,
                                                    TRUE,
                                                    prov_ptr,
                                                    direction,
                                                    next_index,
                                                    is_mapper))
        {
            /*
             * The call is idempotent as it checks the state of prov_ptr,
             * active_state, and if not ACTIVE returns
             */
            digi_resource_oduksw_chnl_deactivate(digi_handle, prov_ptr);
            digi_resource_handle->oduksw_cfg[next_index].dest_activated = FALSE;
            digi_resource_handle->datapath_release = TRUE;
        }

        if (TRUE == digi_resource_evaluate_mapping(digi_handle,
                                                   digi_resource_handle,
                                                   TRUE,
                                                   prov_ptr,
                                                   direction,
                                                   next_index,
                                                   is_mapper))
        {
            digi_resource_oduksw_chnl_demap(digi_handle, 
                                            digi_resource_handle,
                                            next_index,
                                            prov_ptr);
            digi_resource_handle->datapath_release = TRUE;
        }
        /*
         * IF the source is ILKN1, 2 or MAPOTN and is not the same as the
         * provisioned handle indicating that this is not a loop back then
         * go around again so that the ILKN 1, 2 or MAPOTN resource can be
         * freed.
         * THE SYS_OTN_MO case is to handle a SYSOTN that loops back at
         * ODUK to the line side.  If SYSOTN to MAPOTN then do not want
         * to trigger another call to release_port. However if coming from
         * line side and SYS_OTN_MO is the source, that is loopback, then
         * need to release that resource.
         */
        if (FALSE == is_ilkn_handle && TRUE == check_src)
        {
            digi_resource_next_handle_t source_type = digi_resource_handle->oduksw_cfg[next_index].source_handle_type;
            digi_resource_next_handle_t dest_type = digi_resource_handle->oduksw_cfg[next_index].primary_dest_handle_type;
            
            digi_resource_handle->oduksw_cfg[next_index].source_handle_type = LAST_DIGI_RESOURCE_HANDLE;
            digi_resource_handle->oduksw_cfg[next_index].primary_dest_handle_type = LAST_DIGI_RESOURCE_HANDLE;
            result = digi_resource_restart_release_port(digi_handle, 
                                                        digi_resource_handle,
                                                        NULL,
                                                        source_type,
                                                        digi_resource_handle->oduksw_cfg[next_index].source_handle_index,
                                                        is_oduk_to_cpb,
                                                        direction);
            digi_resource_handle->oduksw_cfg[next_index].source_handle_type = source_type;
            digi_resource_handle->oduksw_cfg[next_index].primary_dest_handle_type = dest_type;
        }
        else if (TRUE == check_dest)
        {
            /*
             * Ensure that if the source side is not connected that the
             * primary destination handle gets cleaned if its ILKN1 or 2 or
             * MAPOTN
             * Ensure that the provisioned handle is not looped back to the
             * destination before going around again.
             */
            digi_resource_next_handle_t dest_type = digi_resource_handle->oduksw_cfg[next_index].primary_dest_handle_type;

            digi_resource_handle->oduksw_cfg[next_index].primary_dest_handle_type = LAST_DIGI_RESOURCE_HANDLE;

            result = digi_resource_restart_release_port(digi_handle, 
                                                        digi_resource_handle,
                                                        NULL,
                                                        dest_type,
                                                        digi_resource_handle->oduksw_cfg[next_index].primary_dest_index,
                                                        is_oduk_to_cpb,
                                                        direction);
            digi_resource_handle->oduksw_cfg[next_index].primary_dest_handle_type = dest_type;            
        }
        else if (FALSE == is_ilkn_handle && FALSE == check_src && FALSE == check_dest)
        {
            UINT32 prov_chnl = ((odu_struct_t *)prov_ptr)->switch_data.oduk_data.channel;
            UINT32 prov_port = (UINT32)((odu_struct_t *)prov_ptr)->switch_data.oduk_data.port_type;
            UINT32 src_port = g_oduk_connect_data_ptr->chnl[prov_port][prov_chnl].mst.source_slv_port;
            UINT32 src_chnl = g_oduk_connect_data_ptr->chnl[prov_port][prov_chnl].mst.source_slv_chnl;

            if (CPB_INDEX_LAST != src_port && CPB_INDEX_LAST != src_chnl &&
                UTIL_GLOBAL_ODUK_PORT_MAPOTN == (util_global_oduk_port_t)src_port && 
                UTIL_GLOBAL_ODUK_PORT_MAPOTN != (util_global_oduk_port_t)prov_port)
            {
                result = digi_resource_restart_release_port(digi_handle, 
                                                            digi_resource_handle,
                                                            NULL,
                                                            DIGI_OTN_MAPPER,
                                                            src_chnl,
                                                            is_oduk_to_cpb,
                                                            direction);
            }

        }
        break;

      case DIGI_CPB_SWITCH:
       /*
         * The caller is connected to the CPB switch. In this case we are
         * at a minimum mapped and likely activated as well. Need to
         * determine to whom the caller is connected and what operations
         * are required.
         */
        prov_index = digi_resource_handle->cpbsw_cfg[next_index].provisioned_handle_index;
        PMC_LOG_TRACE("CPB Switch Case, NextIndex=%d, Prov_index=%d\n",
                      next_index, prov_index);

        switch(digi_resource_handle->cpbsw_cfg[next_index].provisioned_handle_type)
        {
          case DIGI_LINE_SERDES_CFG:
            prov_ptr = digi_resource_handle->line_serdes_cfg[prov_index].chnl_handle_ptr;
            break;

          case DIGI_SYS_SERDES_CFG:
            prov_ptr = digi_resource_handle->sys_serdes_cfg[prov_index].chnl_handle_ptr;
            break;

          case DIGI_SFI51_SERDES_CFG:
            prov_ptr = digi_resource_handle->sfi51_serdes_cfg[prov_index].chnl_handle_ptr;
            break;

          case DIGI_ILKN_1:
            prov_ptr =  digi_resource_handle->ilkn_chnl_cfg[0][prov_index].ilkn_chnl_handle_ptr;

            /*
             * ILKN behaves as does MAPOTN across ODUK connected to 
             * a OTN resource.  
             * NOTE: The only exclusion to that is if this is being done
             * from the backplane. A condition may need to be added if
             * that is the case.
             */
            is_mapper = TRUE;
            is_ilkn_handle = TRUE;
            break;
            
          case DIGI_ILKN_2:
            prov_ptr =  digi_resource_handle->ilkn_chnl_cfg[1][prov_index].ilkn_chnl_handle_ptr;
            is_mapper = TRUE;
            is_ilkn_handle = TRUE;
            break;
  
          case DIGI_OTN_MAPPER:
             prov_ptr =  digi_resource_handle->mapper_cfg[prov_index].mapper_handle_ptr;

             if (TRUE == is_oduk_to_cpb)
             {
                 is_mapper = FALSE;
             }
             else
             {
                 is_mapper = TRUE;
             }
             break;

          default:
            PMC_RETURN();
        }

        if (NULL == prov_ptr)
        {
            if (TRUE == digi_resource_handle->force_release)
            {
                PMC_RETURN();
            }
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        }
        if (FALSE == is_ilkn_handle)
        {
            digi_resource_check_src_dest(digi_handle,
                                         digi_resource_handle,
                                         FALSE,
                                         is_oduk_to_cpb,
                                         next_index,
                                         direction,
                                         &check_dest,
                                         &check_src);
        }

         if (TRUE == digi_resource_evaluate_activate(digi_handle,
                                                     digi_resource_handle,
                                                     FALSE,
                                                     prov_ptr,
                                                     direction,
                                                     next_index,
                                                     is_mapper))
         {            
             digi_resource_cpb_chnl_deactivate(digi_handle, 
                                               digi_resource_handle,
                                               next_index,
                                               prov_ptr);
            digi_resource_handle->cpbsw_cfg[next_index].dest_activated = FALSE;
            digi_resource_handle->datapath_release = TRUE;
        }       

        if (TRUE == digi_resource_evaluate_mapping(digi_handle,
                                                   digi_resource_handle,
                                                   FALSE,
                                                   prov_ptr,
                                                   direction,
                                                   next_index,
                                                   is_mapper))
        {            
            digi_resource_cpb_chnl_demap(digi_handle, digi_resource_handle,
                                         next_index, prov_ptr);
            digi_resource_handle->datapath_release = TRUE;
        }
        /*
         * First check ensures that this is not a loop back where the
         * provisioned channel is also the source.
         * If the source type is ILKN 1 or 2 or it is MAPOTN and the 
         * provisioned handle is not ILKN 1 or 2 then go around again.
         * The latter check is to ensure that MAPOTN is not release again
         * if the provisioned port is from the ILKN interface. That is 
         * ILKN 1 or 2 are the provisioned type and the source is MAPOTN
         * then this function was called from the perspective of MAPOTN. 
         */
        if (FALSE == is_ilkn_handle && TRUE == check_src)
        {
            digi_resource_next_handle_t source_type = digi_resource_handle->cpbsw_cfg[next_index].source_handle_type;
            digi_resource_next_handle_t dest_type = digi_resource_handle->cpbsw_cfg[next_index].primary_dest_handle_type;
            digi_resource_handle->cpbsw_cfg[next_index].source_handle_type = LAST_DIGI_RESOURCE_HANDLE;

            result = digi_resource_restart_release_port(digi_handle, 
                                                        digi_resource_handle,
                                                        NULL,
                                                        source_type,
                                                        digi_resource_handle->cpbsw_cfg[next_index].source_handle_index,
                                                        is_oduk_to_cpb,
                                                        direction);

            digi_resource_handle->cpbsw_cfg[next_index].source_handle_type = source_type;
            digi_resource_handle->cpbsw_cfg[next_index].primary_dest_handle_type = dest_type;
        }
        else if (FALSE == is_ilkn_handle && TRUE == check_dest)
        {
            /*
             * Ensure that if the source side is not connected that the
             * primary destination handle gets cleaned if its ILKN1 or 2 or
             * MAPOTN
             * Also ensure that the provisioned side is looped back to the
             * destination.
             */
            digi_resource_next_handle_t dest_type = digi_resource_handle->cpbsw_cfg[next_index].primary_dest_handle_type;

            digi_resource_handle->cpbsw_cfg[next_index].primary_dest_handle_type = LAST_DIGI_RESOURCE_HANDLE;

            result = digi_resource_restart_release_port(digi_handle, 
                                                        digi_resource_handle,
                                                        NULL,
                                                        dest_type,
                                                        digi_resource_handle->cpbsw_cfg[next_index].primary_dest_index,
                                                        is_oduk_to_cpb,
                                                        direction);

            digi_resource_handle->cpbsw_cfg[next_index].primary_dest_handle_type = dest_type;
        }
        break;

    default:
      break;
    }

    PMC_RETURN();
} /* digi_resource_restart_release_switch_chnl */

/*******************************************************************************
* digi_resource_verify_switch_operational
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *digi_resource_handle  - Resource handle table that has already been 
*                            built
*   switch_ptr             - Pointer to the particular switch element being
*                            verified
*   switch_type            - DIGI_ODUK_SWITCH or DIGI_CPB_SWITCH
*   check_switch_only      - Flag that indicates whether only the state of
*                            the switch is to be checked or need to carry
*                            on checking state of source type.
*   prov_type              - The provisioned handle type.
*   prov_index             - Provisioned handle index.
*   direction              - Direction the datapath is being checked.
*
* OUTPUTS:
*  None
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_verify_switch_operational(digi_handle_t *digi_handle,
                                                      digi_resource_handle_summary_t *digi_resource_handle,
                                                      digi_switch_ctxt_t *switch_ptr,
                                                      digi_resource_next_handle_t switch_type,
                                                      BOOL8 check_switch_only,
                                                      digi_resource_next_handle_t prov_type,
                                                      UINT16 prov_index,
                                                      digi_direction_check_t direction)
{
    BOOL8 result = FALSE;
    digi_resource_next_handle_t operational_hndl = LAST_DIGI_RESOURCE_HANDLE;
    UINT32 operational_chnl = 0;

    PMC_ENTRY();

    if (!(prov_type == switch_ptr->provisioned_handle_type ||
          prov_index == switch_ptr->provisioned_handle_index))
    {
        PMC_LOG_TRACE("ProHandle=%d is not MAPOTN or Source=%d, Dest=%d is not ACTIVATED\n", 
                      switch_ptr->provisioned_handle_type,
                      switch_ptr->source_activated,
                      switch_ptr->dest_activated);
    }
    if (TRUE == check_switch_only)
    {
        if (DIGI_DIRECTION_TX_ONLY == direction && 
            (LAST_DIGI_RESOURCE_HANDLE == switch_ptr->primary_dest_handle_type || FALSE == switch_ptr->source_activated))
        {
            PMC_LOG_TRACE("Direction is TX ONLY and Destination=%d is LAST_RESOURCE or Source=%d is not activated", 
                          switch_ptr->primary_dest_handle_type,
                          switch_ptr->source_activated);
        }
        else if (DIGI_DIRECTION_RX_ONLY == direction && 
                 (LAST_DIGI_RESOURCE_HANDLE == switch_ptr->source_handle_type || FALSE == switch_ptr->dest_activated)) 
        {    
            PMC_LOG_TRACE("Direction is RX ONLY and Source=%d is LAST_RESOURCE or Destination=%d is not activated", 
                          switch_ptr->primary_dest_handle_type,
                          switch_ptr->source_activated);
        }
        else if (DIGI_DIRECTION_TX_RX == direction && 
                 (LAST_DIGI_RESOURCE_HANDLE == switch_ptr->source_handle_type ||
                  LAST_DIGI_RESOURCE_HANDLE == switch_ptr->primary_dest_handle_type || FALSE == switch_ptr->dest_activated || FALSE == switch_ptr->source_activated))
        {
            PMC_LOG_TRACE("Direction is TX_RX and Source or Destination are not active");
        }
        else if (DIGI_DIRECTION_TX_RX == direction &&
                 !(switch_ptr->source_handle_type == switch_ptr->primary_dest_handle_type && 
                   switch_ptr->source_handle_index == switch_ptr->primary_dest_index))
        {
            /*
             * At this point the source and destination only need to be checked
             * if they are different. Since this is called from the CPB side
             * then whatever MAPOTN is connected to over the CPB switch should
             * have already been checked.
             */
            
            /*
             * Source and destination for this MAPOTN channel is not the 
             * same. Need to determine whether or not there is a check to
             * be done here.
             * MAY NEED A FLAG TO INDICATE THAT THE RESOURCE HAS BEEN 
             * VERIFIED TO BE OPERATIONAL.
             * FOR NOW TREAT THIS AS AN ERROR CASE.
             */
            PMC_LOG_TRACE("Source=%d and Destination=%d are not the same or are not the same element.\n", 
                          switch_ptr->source_handle_type, 
                          switch_ptr->primary_dest_index);
        }
        else
        {
            if (DIGI_DIRECTION_TX_ONLY == direction)
            {
                operational_hndl = switch_ptr->primary_dest_handle_type;
                operational_chnl = switch_ptr->primary_dest_index;
            }
            else
            {
                operational_hndl = switch_ptr->source_handle_type;
                operational_chnl = switch_ptr->source_handle_index;
            }
            result = TRUE;
        }
    }
    else
    {
        /*
         * If this is the second time through then the orientation of RX
         * and TX flips.
         */
        if (DIGI_DIRECTION_TX_ONLY == direction && 
            (LAST_DIGI_RESOURCE_HANDLE == switch_ptr->source_handle_type || FALSE == switch_ptr->dest_activated)) 
        {    
            PMC_LOG_TRACE("Direction is TX ONLY and Source=%d is LAST_RESOURCE or Destination=%d is not activated", 
                          switch_ptr->source_handle_type,
                          switch_ptr->dest_activated);
        }
        else if (DIGI_DIRECTION_RX_ONLY == direction && 
                 (LAST_DIGI_RESOURCE_HANDLE == switch_ptr->primary_dest_handle_type || FALSE == switch_ptr->source_activated))
        {
            PMC_LOG_TRACE("Direction is RX ONLY and Destination=%d is LAST_RESOURCE or Source=%d is not activated", 
                          switch_ptr->primary_dest_handle_type,
                          switch_ptr->source_activated);
        }
        else if (DIGI_DIRECTION_TX_RX == direction && 
                 (LAST_DIGI_RESOURCE_HANDLE == switch_ptr->source_handle_type ||
                  LAST_DIGI_RESOURCE_HANDLE == switch_ptr->primary_dest_handle_type || FALSE == switch_ptr->dest_activated || FALSE == switch_ptr->source_activated))
        {
            PMC_LOG_TRACE("Direction is TX_RX and Source or Destination are not active");
        }
        else if (DIGI_DIRECTION_TX_RX == direction &&
                 !(switch_ptr->source_handle_type == switch_ptr->primary_dest_handle_type && 
                   switch_ptr->source_handle_index == switch_ptr->primary_dest_index))
        {
            PMC_LOG_TRACE("Source=%d and Destination=%d are not the same or are not the same element.\n", 
                          switch_ptr->source_handle_type, 
                          switch_ptr->primary_dest_index);
        }
        else
        {
            if (DIGI_DIRECTION_RX_ONLY == direction)
            {
                operational_hndl = switch_ptr->primary_dest_handle_type;
                operational_chnl = switch_ptr->primary_dest_index;
            }
            else
            {
                operational_hndl = switch_ptr->source_handle_type;
                operational_chnl = switch_ptr->source_handle_index;
            }

            result = TRUE;
        }
    }

    if (TRUE == result)
    {
        if (!(TRUE == check_switch_only ||
              (prov_type == switch_ptr->source_handle_type &&
               prov_index == switch_ptr->source_handle_index)))
          
        {
            result = digi_resource_check_operational_level(digi_handle,
                                                           digi_resource_handle,
                                                           operational_hndl,
                                                           operational_chnl,
                                                           switch_type,
                                                           direction);
        }
    }

    PMC_RETURN(result);
}  /* digi_resource_verify_switch_operational */


/*******************************************************************************
* digi_resource_is_mapotn_loopback
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*   switch_ptr             - Pointer to the particular switch element being
*                            verified
*
* OUTPUTS:
*   source_type_ptr        - Assigned the source handle if applicable
*   source_index_ptr       - Index of the source element

*
* RETURNS:
*  TRUE - MAPOTN is loop back or not connected to anything.
*  FALSE - MAPOTN is connected to another resource.
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_is_mapotn_loopback(digi_switch_ctxt_t *switch_ptr,
                                               digi_resource_next_handle_t *source_type_ptr,
                                               UINT16 *source_index_ptr)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if ((DIGI_OTN_MAPPER == switch_ptr->source_handle_type && 
         switch_ptr->provisioned_handle_index == switch_ptr->source_handle_index) ||
        (DIGI_OTN_MAPPER == switch_ptr->primary_dest_handle_type && 
         switch_ptr->provisioned_handle_index == switch_ptr->primary_dest_index))
    {
        result = TRUE;
    }
    else
    {
        *source_type_ptr = switch_ptr->source_handle_type;
        *source_index_ptr = switch_ptr->source_handle_index;

        if (LAST_DIGI_RESOURCE_HANDLE == switch_ptr->source_handle_type)
        {
            if (LAST_DIGI_RESOURCE_HANDLE == switch_ptr->primary_dest_handle_type)
            {
                result = TRUE;
            }
            else
            {
                *source_type_ptr = switch_ptr->primary_dest_handle_type;
                *source_index_ptr = switch_ptr->primary_dest_index;
            }
        }
    }


    PMC_RETURN(result);
} /*digi_resource_is_mapotn_loopback*/


/*******************************************************************************
* digi_resource_release_next_chnl
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle          - Refernece to the digi handle
*  digi_resource_handle - Reference to the digi resource table.
*  handle_type          - Handle type to be deprovisioned.
*  handle_index         - index of the handle type in the resource table.
*  direction            - Direction of the path to be release if applicable
*
* OUTPUTS:
*  None
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_release_next_chnl(digi_handle_t *digi_handle,
                                             digi_resource_handle_summary_t *digi_resource_handle,
                                             digi_resource_next_handle_t handle_type,
                                             UINT16 handle_index,
                                             digi_direction_check_t direction)
{
    digi_serdes_port_intf_ctxt_t *serdes_port_ptr = NULL;

    PMC_ENTRY();

    switch (handle_type)
    {
    case DIGI_LINE_SERDES_CFG:
        if (handle_index >= DIGI_XFI_PIN_NUM)
        {
            PMC_LOG_TRACE("Line Serdes Handle=%d is out of range\n", handle_index);
            break;
        }
        
        serdes_port_ptr = &digi_resource_handle->line_serdes_cfg[handle_index];
        break;

    case DIGI_SYS_SERDES_CFG:
        if (handle_index >= DIGI_XFI_PIN_NUM)
        {
            PMC_LOG_TRACE("System Serdes Handle=%d is out of range\n", handle_index);
            break;
        }
        serdes_port_ptr = &digi_resource_handle->sys_serdes_cfg[handle_index];
        break;

    case DIGI_SFI51_SERDES_CFG:
        if (handle_index >= DIGI_SFI51_INTF_NUM)
        {
            PMC_LOG_TRACE("SFI51 Serdes Handle=%d is out of range\n", handle_index);
            break;
        }
        serdes_port_ptr = &digi_resource_handle->sfi51_serdes_cfg[handle_index];
        break;

    default:
        /*
         * ILKN channels should be handled via 
         * digi_resource_restart_release_switch_chnl when the MAPOTN
         * source and/or destination is processed.
         */ 
        break;

    }
    /*
     * This code needs to be updated to ensure that the resource is
     * not mapped or activated based on the direction. Otherwise there
     * is nothing to do. Namely it should  not be deprovisioned and the
     * serdes state should not be reset.
     */
    if (serdes_port_ptr && NULL != serdes_port_ptr->chnl_handle_ptr &&
        DIGI_CPB_SWITCH == serdes_port_ptr->next_resource_handle)
    {
        UINT16 index = serdes_port_ptr->next_handle_index;

        if (DIGI_DIRECTION_RX_ONLY == direction &&
            TRUE == digi_resource_handle->cpbsw_cfg[index].dest_activated)
        {
            digi_resource_cpb_chnl_deactivate(digi_handle, 
                                              digi_resource_handle,
                                              index,
                                              (util_global_switch_data_t *)serdes_port_ptr->chnl_handle_ptr);
        }

        if (DIGI_DIRECTION_RX_ONLY == direction &&
            LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->cpbsw_cfg[index].source_handle_type)
        {
            digi_resource_cpb_chnl_demap(digi_handle, 
                                         digi_resource_handle,
                                         index,
                                         (util_global_switch_data_t *)serdes_port_ptr->chnl_handle_ptr);
        }

        serdes_port_ptr->path_verified = TRUE;
    }
    

    PMC_RETURN();
} /* digi_resource_release_next_chnl */


/*******************************************************************************
* digi_resource_get_oduk_resource_type
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Converts the util_global_oduk_port_t into the appropriate 
*  digi_resource_next_handle_t handle. For COREOTN values it searches 
*  through the OTN Container values and matches the unchannelized ODUK 
*  next resource and the channel to determine the OTN value.
*
* INPUTS:
*   digi_handle          - Reference to the digi_handle
*   digi_resource_handle - Reference to the resource table.
*   port_type            - One of the 4 DPI port types.
*   channel              - Channel used within the DPI port.
*   provisioned_handle   - Provisioned handle used for SYSOTN cases to 
*                          determine which OTN container to search.
* OUTPUTS:
*   None
*
* RETURNS:
*   digi_resource_next_handle_t - value matching the port_type.
*
* NOTES:
*
*******************************************************************************/
PRIVATE digi_resource_next_handle_t digi_resource_get_oduk_resource_type(digi_handle_t *digi_handle,
                                                                         digi_resource_handle_summary_t *digi_resource_handle, 
                                                                        util_global_oduk_port_t port_type, 
                                                                        UINT32 channel,
                                                                        digi_resource_next_handle_t provisioned_handle)
{
    digi_resource_next_handle_t resource_type = LAST_DIGI_RESOURCE_HANDLE;

    PMC_ENTRY();

    switch (port_type)
    {
    case UTIL_GLOBAL_ODUK_PORT_ILKN1:
      resource_type = DIGI_ILKN_1;

      if (TRUE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle))
      {
          /* 
           * If this is a sysotn application then need to change this
           * to be LINE OTN LO container as it is the line side connection
           * in the SYSOTN application.
           * destination.
           */
          resource_type = DIGI_LINE_OTN_HO_CONTAINER;
      }
      break;

    case UTIL_GLOBAL_ODUK_PORT_ILKN2:
      resource_type = DIGI_ILKN_2;
      break;

    case UTIL_GLOBAL_ODUK_PORT_MAPOTN:
      resource_type = DIGI_OTN_MAPPER;
      break;

  case UTIL_GLOBAL_ODUK_PORT_COREOTN:
      /*
       * If this is a SYSOTN app and the provisioned handle is the Line side
       * high order container then need to check through the sys otn 
       * container for the likely candidate.
       */
      if (TRUE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle))
      {
          if (DIGI_ODUK_SWITCH == digi_resource_handle->sys_otn_ho_container_cfg[0].next_resource_handle)
          {
              if (0 == channel)
              {
                  resource_type = DIGI_SYS_OTN_HO_CONTAINER;
              }
              else
              {
                  /*
                   * There is only one sysotn ho channel possible. If it is
                   * channelized then no muxing so the only possible channel
                   * that could use it is channel 0. If there is anything
                   * else something is wrong. Assert so it can be caught.
                   */
                  PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
              }
              break;
          }
          /*
           * If here then SYSOTN is doing one level of muxing. Check the
           * sysotn Mid order value for this channel and determine if it is
           * valid.
           */
          if (channel < DIGI_OTN_SERVER_MO_CHNL_MAX &&
              DIGI_ODUK_SWITCH == digi_resource_handle->sys_otn_mo_container_cfg[channel].next_resource_handle)
          {
              resource_type = DIGI_SYS_OTN_MO_CONTAINER;
          }
          break;
      } 
      else
      {
          /*
           * Need to determine what container is being used. Look through 
           * the High Order, Mid Order and Lower order channels. 
           * If next_resource_handle is DIGI_ODUK_SWITCH and channel 
           * matches then have a match.
           */
          if (channel < DIGI_OTN_SERVER_HO_CHNL_MAX &&
              DIGI_ODUK_SWITCH == digi_resource_handle->line_otn_ho_container_cfg[channel].next_resource_handle)
          {
              resource_type = DIGI_LINE_OTN_HO_CONTAINER;
              break;
          }

          if (channel < DIGI_OTN_SERVER_MO_CHNL_MAX && 
              DIGI_ODUK_SWITCH == digi_resource_handle->line_otn_mo_container_cfg[channel].next_resource_handle)
          {
              resource_type = DIGI_LINE_OTN_MO_CONTAINER;
              break;
          }

          if (channel < DIGI_OTN_SERVER_LO_CHNL_MAX &&
              DIGI_ODUK_SWITCH == digi_resource_handle->line_otn_lo_container_cfg[channel].next_resource_handle)
          {
              resource_type = DIGI_LINE_OTN_LO_CONTAINER;
              break;
          }
          
          if (DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == digi_handle->var.prod_app &&
              channel < DIGI_OTN_SERVER_HO_CHNL_MAX &&
              DIGI_ODUK_SWITCH == digi_resource_handle->sys_otn_ho_container_cfg[channel].next_resource_handle)  
          {
              resource_type = DIGI_SYS_OTN_HO_CONTAINER;
              break;
          }
          else if (DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == digi_handle->var.prod_app &&
                   channel < DIGI_OTN_SERVER_MO_CHNL_MAX &&
                   DIGI_ODUK_SWITCH == digi_resource_handle->sys_otn_mo_container_cfg[channel].next_resource_handle)
          {
              resource_type = DIGI_SYS_OTN_MO_CONTAINER;
              break;
          }
      }
      break;  

    default:
      break;
    }

    PMC_RETURN(resource_type);
} /* digi_resource_get_oduk_resource_type */


/*******************************************************************************
* digi_resource_check_otn_prov_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Utility function that calls the particular OTN prov_get function to
*  determine whether or not the operational level of the OTN sub-container
*  matches what is stored in the resource level. Namely verifies that it is
*  indeed operational.
*
* INPUTS:
*  digi_handle          - Refernece to the digi handle
*  digi_resource_handle - Reference to the digi resource table.
*  otn_handle           - OTN Container type
*  otn_container_ptr    - Reference to the particular 
*                         digi_otn_container_cfg_t structure.
*  direction            - Direction of the datapath
*
* OUTPUTS:
*  None
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_check_otn_prov_get(digi_handle_t *digi_handle,
                                               digi_resource_handle_summary_t *digi_resource_handle,
                                               digi_resource_next_handle_t otn_handle,
                                               digi_otn_container_cfg_t *otn_container_ptr,
                                               digi_direction_check_t direction)
{
    BOOL8 result = FALSE;
    BOOL8 tx_result = FALSE;
    BOOL8 rx_result = FALSE;
    coreotn_mux_stage_t mux_stage = LAST_COREOTN_MUX_STAGE;
    digi_resource_next_handle_t local_handle = otn_handle;

    PMC_ENTRY();

    if (TRUE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle) &&
        DIGI_LINE_OTN_HO_CONTAINER == local_handle)
    {
        local_handle = DIGI_LINE_OTN_LO_CONTAINER;
    }

    switch (local_handle)
    {
    case DIGI_LINE_OTN_HO_CONTAINER:
    case DIGI_SYS_OTN_HO_CONTAINER:
      /*
       * If SYSOTN and HO have the same behaviour here. If unchannelized 
       * then it is connected directly to the ODUK.
       */
      mux_stage = COREOTN_MUX_STAGE_NONE;

      if (DIGI_ODU_UNCHANNELIZED != otn_container_ptr->payload_mode)
      {
          mux_stage = COREOTN_MUX_STAGE_ONE;
      }
      if (DIGI_DIRECTION_TX_RX == direction ||
          DIGI_DIRECTION_TX_ONLY == direction)
      {
          tx_result = coreotn_ho_chnl_prov_get(digi_handle->coreotn_handle,
                                               ((odu_struct_t *)otn_container_ptr->chnl_handle_ptr)->mem_ptr->chnl_id,
                                               FALSE,
                                               mux_stage,
                                               (util_global_odu_line_payload_t)otn_container_ptr->payload_mode,
                                               UTIL_GLOBAL_CHNL_OPERATIONAL);
      }
      
      if (DIGI_DIRECTION_TX_RX == direction ||
          DIGI_DIRECTION_RX_ONLY == direction)
      {
          rx_result = coreotn_ho_chnl_prov_get(digi_handle->coreotn_handle,
                                               ((odu_struct_t *)otn_container_ptr->chnl_handle_ptr)->mem_ptr->chnl_id,
                                               TRUE,
                                               mux_stage,
                                               (util_global_odu_line_payload_t)otn_container_ptr->payload_mode,
                                               UTIL_GLOBAL_CHNL_OPERATIONAL);
      }
      result = digi_resource_set_directional_result(direction, tx_result,
                                                    rx_result);

      /*
       * A high order channel is bi-directional and so if either test 
       * fails need to ensure the resource is deprovisioned.
       */
      if (FALSE == result)
      {
          digi_resource_handle->force_release = TRUE;
      }

      break;

    case DIGI_LINE_OTN_MO_CONTAINER:
    case DIGI_SYS_OTN_MO_CONTAINER:
      mux_stage = COREOTN_MUX_STAGE_ONE;

      if (DIGI_DIRECTION_TX_RX == direction ||
          DIGI_DIRECTION_TX_ONLY == direction)
      {
          tx_result = coreotn_lo_chnl_prov_get(digi_handle->coreotn_handle,
                                               ((odu_struct_t *)otn_container_ptr->chnl_handle_ptr)->mem_ptr->chnl_id,
                                               FALSE,
                                               mux_stage,
                                               (util_global_odu_line_payload_t)otn_container_ptr->payload_mode,
                                               otn_container_ptr->tx_trib_slot_mask,
                                               otn_container_ptr->rx_trib_slot_mask,
                                               UTIL_GLOBAL_CHNL_OPERATIONAL);
      }
      
      if (DIGI_DIRECTION_TX_RX == direction ||
          DIGI_DIRECTION_RX_ONLY == direction)
      {
          rx_result = coreotn_lo_chnl_prov_get(digi_handle->coreotn_handle,
                                               ((odu_struct_t *)otn_container_ptr->chnl_handle_ptr)->mem_ptr->chnl_id,
                                               TRUE,
                                               mux_stage,
                                               (util_global_odu_line_payload_t)otn_container_ptr->payload_mode,
                                               otn_container_ptr->tx_trib_slot_mask,
                                               otn_container_ptr->rx_trib_slot_mask,
                                               UTIL_GLOBAL_CHNL_OPERATIONAL);
      }
      result = digi_resource_set_directional_result(direction, tx_result,
                                                    rx_result);

      /*
       * Mid order channels if unchannelized are considered uni-directional
       * and thus a failure is valid. If however it is channelized then 
       * the coreotn check should work in either direction. In this case
       * the datapath is considered invalid and should be released.
       */
      if (FALSE == result &&
          (DIGI_ODU_TS_1G25 == otn_container_ptr->payload_mode ||
           DIGI_ODU_TS_2G5 == otn_container_ptr->payload_mode))
      {
          digi_resource_handle->force_release = TRUE;
      }

      break;

    case DIGI_LINE_OTN_LO_CONTAINER:
      /*
       * If this is a SYSOTN application then the LO_CONTAINER in this case
       * is the LineOTN connection to LO ODU container and then the switch
       * These is no muxing in this case.
       */
      if (TRUE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle))
      {
          /*
           * for DIGI_PROD_APP_SYSOTN_CARD product application, coreotn uses
           * the 'channelized' context for the segment from SYSOTN. The 
           * segment from LINEOTN cannot be channelized and use a dedicated
           * context. coreotn_sysapp_line_chnl_prov_get is used to get the
           * state of this dedicated context 
           */            
          result = coreotn_sysapp_line_chnl_prov_get(digi_handle->coreotn_handle,
                                                     ((odu_struct_t *)otn_container_ptr->chnl_handle_ptr)->mem_ptr->chnl_id,
                                                     UTIL_GLOBAL_CHNL_OPERATIONAL);

          
          /*
           * A high order channel is bi-directional and so if either test 
           * fails need to ensure the resource is deprovisioned.
           */
          if (FALSE == result)
          {
              digi_resource_handle->force_release = TRUE;
          }
      } 
      else 
      {
          mux_stage = COREOTN_MUX_STAGE_TWO;

          if (DIGI_DIRECTION_TX_RX == direction ||
              DIGI_DIRECTION_TX_ONLY == direction)
          {
              tx_result = coreotn_lo_chnl_prov_get(digi_handle->coreotn_handle,
                                                   ((odu_struct_t *)otn_container_ptr->chnl_handle_ptr)->mem_ptr->chnl_id,
                                                   FALSE,
                                                   mux_stage,
                                                   (util_global_odu_line_payload_t)otn_container_ptr->payload_mode,
                                                   otn_container_ptr->tx_trib_slot_mask,
                                                   otn_container_ptr->rx_trib_slot_mask,
                                                   UTIL_GLOBAL_CHNL_OPERATIONAL);
          }
          
          if (DIGI_DIRECTION_TX_RX == direction ||
              DIGI_DIRECTION_RX_ONLY == direction)
          {
              rx_result = coreotn_lo_chnl_prov_get(digi_handle->coreotn_handle,
                                                   ((odu_struct_t *)otn_container_ptr->chnl_handle_ptr)->mem_ptr->chnl_id,
                                                   TRUE,
                                                   mux_stage,
                                                   (util_global_odu_line_payload_t)otn_container_ptr->payload_mode,
                                                   otn_container_ptr->tx_trib_slot_mask,
                                                   otn_container_ptr->rx_trib_slot_mask,
                                                   UTIL_GLOBAL_CHNL_OPERATIONAL);
          }
          result = digi_resource_set_directional_result(direction,
                                                         tx_result,
                                                         rx_result);

      }
      break;
      
    default:
      PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
      break;
    }

    PMC_RETURN(result);
} /* digi_resource_check_otn_prov_get */


/*******************************************************************************
* digi_resource_configure_switch_destination
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Sets the primary and secondary (if applicable )destinations of the 
*  current provisioned subsystem. Uses the CPB/ODUK connection data. If not
*  broadcast the the slv.dest_mst_port is the primary destination. If
*  broadcast then need to examine the dest_mst_port to determine if it is
*  the slave_zone_primary and what the next_dest_mst_port is. The slave
*  side is initially examined to determine what this subsystem is sending
*  to the switch. For broadcast cases however need to examine the master
*  side to determine where the switch is sending the signal to. The
*  secondary destination of the provisioned subsystem is the first non-
*  slave zone primary. For greater than 2 destinations need to set the 
*  secondary resource table value in that switch table entry.

*
* INPUTS:
*  digi_handle          - Refernece to the digi handle
*  digi_resource_handle - Reference to the digi resource table.
*  switch_ptr           - Reference to the resource table switch array for
*                         the provisioned subsystem whose destination 
*                         handles are being set.
*  switch_type          - Either DIGI_ODUK_SWITCH or DIGI_CPB_SWITCH.
*  oduk_port_type       - The provisoned ODUK port type to be used. With 
*                         PMON the provisioned value may not be the cpb 
*                         conection array value that needs to be examined.
*  cpb_port_type        - The util_global_cpb_port_t value representing the
*                         provisioned port value for CPB switch. Required
*                         to handle eTrans case of MAPOTN using ENET line.
*                         Not used for ODUK switch instances
*  prov_channel         - Provisioned channel which may be different than
*                         the switch index value.
*  enet_pmon            - Whether enet pmon has been enabled.
*
* OUTPUTS:
*  None
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_configure_switch_destination(digi_handle_t *digi_handle,
                                                        digi_resource_handle_summary_t *digi_resource_handle,
                                                        digi_switch_ctxt_t *switch_ptr,
                                                        digi_resource_next_handle_t switch_type,                                         
                                                        util_global_oduk_port_t oduk_port_type,
                                                        util_global_cpb_port_t cpb_port_type,
                                                        UINT32 prov_channel,
                                                        BOOL8 enet_pmon)
{
    UINT32 dest_port;
    UINT32 dest_chnl;
    UINT32 orig_port;
    UINT32 orig_chnl;
    UINT32 next_port;
    UINT32 next_chnl;

    PMC_ENTRY();

    if (DIGI_ODUK_SWITCH == switch_type)
    {
        /*
         * Retrieve the slave side destination master port and channel.
         * if not broadcast/multicast then this is the primary destination
         * and there is no secondard destination for this subsystem. Else
         * need to follow that dest_mst_port to determine the primary and
         * secondary ports.
         * If the port type is ILKN1 then need to check the channel as it
         * is likely 150 or greater where as the g_oduk_connect_data_ptr
         * value is 0 relative. Namely 150 is 0, 151 is 1, etc.
         */

        if ((UTIL_GLOBAL_ODUK_PORT_ILKN1 == oduk_port_type ||
            UTIL_GLOBAL_ODUK_PORT_ILKN2 == oduk_port_type ) &&
            prov_channel >= DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX)
        {
            prov_channel -= DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
        }

        dest_port = g_oduk_connect_data_ptr->chnl[oduk_port_type][prov_channel].slv.dest_mst_port;
        dest_chnl = g_oduk_connect_data_ptr->chnl[oduk_port_type][prov_channel].slv.dest_mst_chnl;

        if (FALSE == g_oduk_connect_data_ptr->chnl[oduk_port_type][prov_channel].slv.is_broadcast &&
            FALSE == g_oduk_connect_data_ptr->chnl[oduk_port_type][prov_channel].slv.is_multicast)
        {
          switch_ptr->primary_dest_handle_type = digi_resource_get_oduk_resource_type(digi_handle, digi_resource_handle, (util_global_oduk_port_t)dest_port, dest_chnl, switch_ptr->provisioned_handle_type);
            switch_ptr->primary_dest_index = dest_chnl;
            PMC_RETURN();
        }
        orig_port = dest_port;
        orig_chnl = dest_chnl;

        while (CPB_INDEX_LAST != dest_port && CPB_INDEX_LAST != dest_chnl)
        {
            UINT16 switch_index = 0;
            /*
             * The primary destination is the connection that is the slave
             * zone primary. As soon as  that is true then set the primary
             * destination for the current switch.  If this 
             */
            if (TRUE == g_oduk_connect_data_ptr->chnl[dest_port][dest_chnl].mst.is_slave_zone_primary)
            {
              switch_ptr->primary_dest_handle_type = digi_resource_get_oduk_resource_type(digi_handle, digi_resource_handle, (util_global_oduk_port_t)dest_port,dest_chnl, switch_ptr->provisioned_handle_type);
                switch_ptr->primary_dest_index = dest_chnl;
            }
            if (CPB_INDEX_LAST == g_oduk_connect_data_ptr->chnl[dest_port][dest_chnl].mst.next_dest_mst_port)
            {
                break;
            }
            /*
             * Set the secondary index of the dest_port and channel to the
             * next_dest_mst_port. In order to do that need to find the
             * switch element associated with the dest_port
             */
            if (TRUE == digi_resource_get_oduk_switch_index((util_global_oduk_port_t)dest_port,
                                                            dest_chnl,
                                                            &switch_index))
            {
                PMC_ASSERT(dest_port < UTIL_GLOBAL_CPB_PORT_NOT_USED, DIGI_ERR_INVALID_ARG, 0, 0);
                next_port = g_oduk_connect_data_ptr->chnl[dest_port][dest_chnl].mst.next_dest_mst_port;
                next_chnl = g_oduk_connect_data_ptr->chnl[dest_port][dest_chnl].mst.next_dest_mst_chnl;
                /*
                 * Check that the switch_ptr and the switch_index are not
                 * the same.  If they are then need to assign the 
                 * associated handle and index to the switch index of the
                 * next port/chnl. That is the secondary handle in this 
                 * case is the dest_port/chnl and that needs to be assigned
                 * to the next_port/chnl index which needs to be looked up.
                 */
                if (!(switch_ptr->provisioned_handle_type == digi_resource_handle->oduksw_cfg[switch_index].provisioned_handle_type && 
                      switch_ptr->provisioned_handle_index == digi_resource_handle->oduksw_cfg[switch_index].provisioned_handle_index))
                {
                    digi_resource_handle->oduksw_cfg[switch_index].associate_dest_handle_type = 
                      digi_resource_get_oduk_resource_type(digi_handle, 
                                                           digi_resource_handle,
                                                           (util_global_oduk_port_t)next_port,
                                                           next_chnl,
                                                           digi_resource_handle->oduksw_cfg[switch_index].provisioned_handle_type);
                    digi_resource_handle->oduksw_cfg[switch_index].associate_dest_index = next_chnl;
                }
                else if (TRUE == digi_resource_get_oduk_switch_index((util_global_oduk_port_t)next_port,
                                                                     next_chnl,
                                                                     &switch_index))
                {
                    digi_resource_handle->oduksw_cfg[switch_index].associate_dest_handle_type = digi_resource_get_oduk_resource_type(digi_handle, 
                                                           digi_resource_handle,
                                                           (util_global_oduk_port_t)dest_port,
                                                           dest_chnl,
                                                           digi_resource_handle->oduksw_cfg[switch_index].provisioned_handle_type);
                    digi_resource_handle->oduksw_cfg[switch_index].associate_dest_index = dest_chnl;
                }
                if (DIGI_ILKN_1 == digi_resource_handle->oduksw_cfg[switch_index].associate_dest_handle_type &&
                    digi_resource_handle->oduksw_cfg[switch_index].associate_dest_index < DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX)
                {
                    digi_resource_handle->oduksw_cfg[switch_index].associate_dest_index += DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
                }
                dest_port = next_port;
                dest_chnl = next_chnl;
            }
            else
            {
                /*
                 * For completeness and to ensure robust behaviour need to
                 * handle this case. This however should not occur sine
                 * the dest_port is valid it should not result in the
                 * switch index not being found.
                 */
                PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                        "ODUK SwitchIndex for DestPortType=%d, Chnl=%d,SwitchType=%d not found!\n", dest_port, dest_chnl, switch_type);
                break;
            }
            if (orig_port == dest_port && orig_chnl == dest_chnl)
            {
                /*
                 * This is a loop. May be a loopback to itself or a loop
                 * over multiple nodes. Regardless it has been processed.
                 */
                break;
            }
        }
    }
    else
    {
        /*
         * For the CPB the provisioned channel is the channel used for the
         * particular subsystem which may not be the same as the index used
         * in the switch table. For example if 40G cage and ENET_LINE then
         * the provisioned index may be 1 but the actual ENET_LINE channel
         * used is 4. The cpb_handle represents the actual ENET_LINE channel
         * where the provisioned handle index represents the index into the
         * table.
         */     
        util_global_cpb_port_t prov_port_type = cpb_port_type;
        PMC_ASSERT(prov_port_type < UTIL_GLOBAL_CPB_PORT_NOT_USED, DIGI_ERR_INVALID_ARG, 0, 0);
        dest_port = g_cpb_connect_data_ptr->chnl[prov_port_type][prov_channel].slv.dest_mst_port;
        dest_chnl = g_cpb_connect_data_ptr->chnl[prov_port_type][prov_channel].slv.dest_mst_chnl;
        
        if (FALSE == g_cpb_connect_data_ptr->chnl[prov_port_type][prov_channel].slv.is_broadcast &&
            FALSE == g_cpb_connect_data_ptr->chnl[prov_port_type][prov_channel].slv.is_multicast)
        {
            switch_ptr->primary_dest_handle_type = digi_resource_get_cpb_resource_type(digi_handle, digi_resource_handle, (util_global_cpb_port_t)dest_port, &dest_chnl);
            switch_ptr->primary_dest_index = dest_chnl;
            PMC_RETURN();
        }
        orig_port = dest_port;
        orig_chnl = dest_chnl;

        while (CPB_INDEX_LAST != dest_port && CPB_INDEX_LAST != dest_chnl)
        {
            UINT16 switch_index = 0;
            /*
             * The primary destination is the connection that is the slave
             * zone primary. As soon as  that is true then set the primary
             * destination for the current switch.  If this 
             */
            if (TRUE == g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.is_slave_zone_primary)
            {
                switch_ptr->primary_dest_handle_type = digi_resource_get_cpb_resource_type(digi_handle, digi_resource_handle, (util_global_cpb_port_t)dest_port, &dest_chnl);
                switch_ptr->primary_dest_index = dest_chnl;
            }
            else if (TRUE == enet_pmon &&
                     UTIL_GLOBAL_CPB_PORT_ENET_SYS == dest_port)
            {
                next_port = g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.next_dest_mst_port;
                next_chnl = g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.next_dest_mst_chnl;

                if (!(CPB_INDEX_LAST == next_port || CPB_INDEX_LAST == next_chnl) && TRUE == g_cpb_connect_data_ptr->chnl[next_port][next_chnl].mst.is_slave_zone_primary)
                {
                    switch_ptr->primary_dest_handle_type = digi_resource_get_cpb_resource_type(digi_handle, digi_resource_handle, (util_global_cpb_port_t)next_port, &next_chnl);
                    switch_ptr->primary_dest_index = next_chnl;
                }
            }

            if (TRUE == enet_pmon &&
                UTIL_GLOBAL_CPB_PORT_MAPOTN == prov_port_type &&
                UTIL_GLOBAL_CPB_PORT_ENET_SYS != dest_port)
            {
                if (prov_port_type == g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.source_slv_port &&
                    prov_channel == g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.source_slv_chnl &&
                    UTIL_GLOBAL_CPB_PORT_ENET_SYS == g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.next_dest_mst_port)
                {
                    break;
                  }
            }

            /*
             * For now use the dest_port value regarding whether or
             * not to break. May need to look at whether broad/multi cast 
             * is possible with enet_pmon and as such another mechanism
             * may be required to fill in the associate handles.
             */
            if (CPB_INDEX_LAST == g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.next_dest_mst_port)
            {
                break;
            }
            /*
             * Set the secondary index of the dest_port and channel to the
             * next_dest_mst_port. In order to do that need to find the
             * switch element associated with the dest_port
             */
            if (TRUE == digi_resource_get_cpb_switch_index((util_global_cpb_port_t)dest_port,
                                                           dest_chnl,
                                                           &switch_index))
            {
                PMC_ASSERT(dest_port < UTIL_GLOBAL_CPB_PORT_NOT_USED, DIGI_ERR_INVALID_ARG, 0, 0);
                next_port = g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.next_dest_mst_port;
                next_chnl = g_cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.next_dest_mst_chnl;
                /*
                 * Ensure that the associated handle is not being set for
                 * the broadcast case which would be the case in an 
                 * extended loop back setting.
                 */
                if (!(switch_ptr->provisioned_handle_type == digi_resource_handle->cpbsw_cfg[switch_index].provisioned_handle_type && 
                      switch_ptr->provisioned_handle_index == digi_resource_handle->cpbsw_cfg[switch_index].provisioned_handle_index))
                {
                    digi_resource_handle->cpbsw_cfg[switch_index].associate_dest_handle_type = 
                      digi_resource_get_cpb_resource_type(digi_handle,
                                                          digi_resource_handle,
                                                          (util_global_cpb_port_t)next_port,
                                                          &next_chnl);
                    digi_resource_handle->cpbsw_cfg[switch_index].associate_dest_index = next_chnl;
                }
                dest_port = next_port;
                dest_chnl = next_chnl;
            }
            else
            {
                /*
                 * For completeness and to ensure robust behaviour need to
                 * handle this case. This however should not occur since
                 * the dest_port is valid it should not result in the
                 * switch index not being found.
                 */
                PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                        "CPB SwitchIndex for DestPortType=%d, Chnl=%d,SwitchType=%d not found!\n", dest_port, dest_chnl, switch_type);
                break;
            }
            if (orig_port == dest_port && orig_chnl == dest_chnl)
            {
                /*
                 * This is a loop. May be a loopback to itself or a loop
                 * over multiple nodes. Regardless it has been processed.
                 */
                break;
            }
        }
    }

    PMC_RETURN();
} /*digi_resource_configure_switch_destination */

/*******************************************************************************
* digi_resource_reset_serdes_handle
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*
* INPUTS:
*  serdes_port_ptr      - Reference to the serdes array element being reset
*
* OUTPUTS:
*  None
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_reset_serdes_handle(digi_serdes_port_intf_ctxt_t *serdes_port_ptr)
{
    PMC_ENTRY();

    serdes_port_ptr->chnl_handle_type = LAST_DIGI_HANDLE_ID;
    serdes_port_ptr->chnl_handle_ptr = NULL;
    serdes_port_ptr->next_resource_handle = LAST_DIGI_RESOURCE_HANDLE;
    serdes_port_ptr->next_handle_index = 0;
    
    PMC_RETURN();
} /* digi_resource_reset_serdes_handle */

/*******************************************************************************
* digi_resource_check_pmon_validity
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Determines whether or not PMON data is set and valid for the current
*  channel handle and PMON type.
*
* INPUTS:
*  digi_resource_handle - Reference to the resource handle
*  element_index        - Index into the resource handle to retrieve the 
*                         channel handle being processed
*  cpb_port_type        - The port type representing the channel handle that
*                         is using the PMON data. Either MAPOTN/ILKN 1 or 2.
*  cpb_chnl             - Channel in the cpb_connection_data array that 
*                         corresponds with the cpb_port_type.
* OUTPUTS:
*  None
*
* RETURNS:
*  TRUE:   PMON is specified and valid
*  FALSE:  No PMON data or invalid.
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_check_pmon_validity(digi_resource_handle_summary_t *digi_resource_handle, 
                                                UINT16 element_index,
                                                util_global_cpb_port_t cpb_port_type, 
                                                UINT32 cpb_chnl)
{
    BOOL8 enet_pmon_valid = FALSE;
    digi_enet_pmon_type_t enet_pmon = LAST_DIGI_ENET_PMON;

    PMC_ENTRY();
    /*
     * Determine which CPB port type is being examined and retrieve the
     * enet_pmon type.
     */
    if (UTIL_GLOBAL_CPB_PORT_MAPOTN == cpb_port_type)
    {
        digi_mapper_chnl_def_t *map_def_ptr = (digi_mapper_chnl_def_t *)digi_resource_handle->mapper_cfg[element_index].mapper_handle_ptr;

        if (NULL != map_def_ptr &&
            NULL != map_def_ptr->enet_pmon_data)
        {
            enet_pmon = map_def_ptr->enet_pmon;
        }
    }
    else if (UTIL_GLOBAL_CPB_PORT_ILKN1 == cpb_port_type)
    {
        digi_ilkn_chnl_def_t *ilkn_def_ptr = (digi_ilkn_chnl_def_t *)digi_resource_handle->ilkn_chnl_cfg[0][element_index].ilkn_chnl_handle_ptr;

        if (ilkn_def_ptr &&
            NULL != ilkn_def_ptr->enet_pmon_data)
        {
            enet_pmon = ilkn_def_ptr->enet_pmon;
        }
    }
    else if (UTIL_GLOBAL_CPB_PORT_ILKN2 == cpb_port_type)
    {
        digi_ilkn_chnl_def_t *ilkn_def_ptr = (digi_ilkn_chnl_def_t *)digi_resource_handle->ilkn_chnl_cfg[1][element_index].ilkn_chnl_handle_ptr;

        if (ilkn_def_ptr &&
            NULL != ilkn_def_ptr->enet_pmon_data)
        {
            enet_pmon = ilkn_def_ptr->enet_pmon;
        }
    }
    /*
     * If the enet_pmon has been set to a valid value then check that the
     * enet_port_type and the enet_pmon flag lines up. Namely find it in
     * the connect array. Removed the operational check for the entity as
     * it is known that it is set.
     */
    if (LAST_DIGI_ENET_PMON != enet_pmon)
    {
        /*
         * First check looks in dest_mst_port which for all intent and
         * purpose is not the enet_pmon. The second and subsequent check
         * checks the master side next_dest_mst_port/chnl which is where
         * PMON resource should be.
         */
        UINT32 enet_port = g_cpb_connect_data_ptr->chnl[cpb_port_type][cpb_chnl].slv.dest_mst_port;
        UINT32 enet_chnl = g_cpb_connect_data_ptr->chnl[cpb_port_type][cpb_chnl].slv.dest_mst_chnl;
        UINT32 tmp_port;

        while (CPB_INDEX_LAST != enet_port && CPB_INDEX_LAST != enet_chnl)
        {
            if ((DIGI_ENET_SYS_PMON == enet_pmon &&
                 UTIL_GLOBAL_CPB_PORT_ENET_SYS ==  (util_global_cpb_port_t)enet_port) ||
                (DIGI_ENET_LINE_PMON == enet_pmon && 
                 UTIL_GLOBAL_CPB_PORT_ENET_LINE == (util_global_cpb_port_t)enet_port))
            {
                enet_pmon_valid = TRUE;
                break;
            }
            tmp_port = g_cpb_connect_data_ptr->chnl[enet_port][enet_chnl].mst.next_dest_mst_port;
            enet_chnl = g_cpb_connect_data_ptr->chnl[enet_port][enet_chnl].mst.next_dest_mst_chnl;
            enet_port = tmp_port;
        }
    }

    PMC_RETURN(enet_pmon_valid);
}


/*******************************************************************************
* digi_resource_convert_port_type
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Converts the port id into the appropriate port_type based on the oduk
*  switch flag and then assigns the corresponding handle id and channel
*  handle pointer to the chnl_conn_ptr structure.
*
* INPUTS:
*  digi_handle          - Reference to the digi handle
*  port_id              - The numeric value of the CPB or ODUK port type
*                         enumerated type.
*  chnl_id              - Channel identifier
*  is_oduk_switch       - Whether the port_type is associated with the 
*                         ODUK switch defined values (TRUE) or CPB switch
*                         defined values.
*  check_mapotn         - Whether or not MAPOTN channel handles need to be
*                         check if this is a CPB port type and port_id is
*                         ENET_LINE
*
* OUTPUTS:
*  chnl_conn_ptr     - Connection pointer to be updated with the handle
*                      type, the pointer, whether it is primary and 
*                      activated.
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_convert_port_type(digi_handle_t *digi_handle,
                                             UINT32 port_id,
                                             UINT32 chnl_id,
                                             BOOL8 is_oduk_switch,
                                             BOOL8 check_mapotn,
                                             digi_chnl_hndl_conn_t *chnl_conn_ptr)
{

    PMC_ENTRY();

    if (CPB_INDEX_LAST == port_id || CPB_INDEX_LAST == chnl_id)
    {
        chnl_conn_ptr->chnl_hndl_type = LAST_DIGI_HANDLE_ID;
        chnl_conn_ptr->chnl_hndl_ptr = NULL;
        PMC_RETURN();
    }

    if (TRUE == is_oduk_switch)
    {
        util_global_oduk_port_t port_type = (util_global_oduk_port_t)port_id;

        switch (port_type)
        {
          case UTIL_GLOBAL_ODUK_PORT_ILKN1:
            if(digi_handle->var.prod_app != DIGI_PROD_APP_SYSOTN_CARD)
            {
                /*
                 * Confirm that the channel id is less that the max number
                 * of ODUK channels, namely 104, and if so tack on 150
                 * to push it into the upper range.
                 */
                if (chnl_id < DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX)
                {
                    chnl_id += DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
                }
                PMC_ASSERT(chnl_id < DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX,
                           DIGI_ERR_INVALID_ARG, 0, 0);
                chnl_conn_ptr->chnl_hndl_ptr = (digi_chnl_hndl_t *)&digi_handle->handle_pool.ilkn_1_chnl[chnl_id];
            }
            else
            {
                chnl_conn_ptr->chnl_hndl_ptr = (digi_chnl_hndl_t *)&digi_handle->handle_pool.lo_odu_struct[chnl_id];
            }
            chnl_conn_ptr->chnl_hndl_type = DIGI_CHANNEL_HANDLE_ID_ILKN_1_T;
            break;

          case UTIL_GLOBAL_ODUK_PORT_ILKN2:
            if (chnl_id < DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX)
            {
                chnl_id += DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
            }
            PMC_ASSERT(chnl_id < DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX,
                       DIGI_ERR_INVALID_ARG, 0, 0);
            chnl_conn_ptr->chnl_hndl_ptr = (digi_chnl_hndl_t *)&digi_handle->handle_pool.ilkn_2_chnl[chnl_id];
            chnl_conn_ptr->chnl_hndl_type = DIGI_CHANNEL_HANDLE_ID_ILKN_2_T;
            break;

          case UTIL_GLOBAL_ODUK_PORT_COREOTN:
            /*
             * For COREOTN need to determine which ODU level the call is
             * operating on. Use the channel value and take the first ODU
             * channel with that is CHANNELIZED.
             */
            if(digi_handle->var.prod_app == DIGI_PROD_APP_SYSOTN_CARD)
            {
                if (UTIL_GLOBAL_ODU_UNCHANNELIZED == digi_handle->handle_pool.mo_odu_struct[chnl_id].mem_ptr->payload_format &&
                  (TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.mo_odu_struct[chnl_id].switch_data.header.prov_state)))
                {
                    chnl_conn_ptr->chnl_hndl_ptr = (digi_chnl_hndl_t *)&digi_handle->handle_pool.mo_odu_struct[chnl_id];
                    chnl_conn_ptr->chnl_hndl_type = DIGI_CHANNEL_HANDLE_ID_MO_ODU_T;
                }
                else if (UTIL_GLOBAL_ODU_UNCHANNELIZED ==  digi_handle->handle_pool.ho_odu_struct[chnl_id].mem_ptr->payload_format &&
                         (TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.ho_odu_struct[chnl_id].switch_data.header.prov_state)))
                {
                    chnl_conn_ptr->chnl_hndl_ptr = (digi_chnl_hndl_t *)&digi_handle->handle_pool.ho_odu_struct[chnl_id];
                    chnl_conn_ptr->chnl_hndl_type = DIGI_CHANNEL_HANDLE_ID_HO_ODU_T;
                }
            }
            else if (UTIL_GLOBAL_ODU_UNCHANNELIZED == digi_handle->handle_pool.lo_odu_struct[chnl_id].mem_ptr->payload_format &&
                (TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.lo_odu_struct[chnl_id].switch_data.header.prov_state)))
            {
                chnl_conn_ptr->chnl_hndl_ptr = (digi_chnl_hndl_t *)&digi_handle->handle_pool.lo_odu_struct[chnl_id];
                chnl_conn_ptr->chnl_hndl_type = DIGI_CHANNEL_HANDLE_ID_LO_ODU_T;
            }
            else if (UTIL_GLOBAL_ODU_UNCHANNELIZED == digi_handle->handle_pool.mo_odu_struct[chnl_id].mem_ptr->payload_format &&
                     (TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.mo_odu_struct[chnl_id].switch_data.header.prov_state)))
            {
                chnl_conn_ptr->chnl_hndl_ptr = (digi_chnl_hndl_t *)&digi_handle->handle_pool.mo_odu_struct[chnl_id];
                chnl_conn_ptr->chnl_hndl_type = DIGI_CHANNEL_HANDLE_ID_MO_ODU_T;
            }
            else if (UTIL_GLOBAL_ODU_UNCHANNELIZED ==  digi_handle->handle_pool.ho_odu_struct[chnl_id].mem_ptr->payload_format &&
                     (TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.ho_odu_struct[chnl_id].switch_data.header.prov_state)))
            {
                chnl_conn_ptr->chnl_hndl_ptr = (digi_chnl_hndl_t *)&digi_handle->handle_pool.ho_odu_struct[chnl_id];
                chnl_conn_ptr->chnl_hndl_type = DIGI_CHANNEL_HANDLE_ID_HO_ODU_T;
            }
            else
            {
                chnl_conn_ptr->chnl_hndl_ptr = NULL;
                chnl_conn_ptr->chnl_hndl_type = LAST_DIGI_HANDLE_ID;
            }
            break;

          case UTIL_GLOBAL_ODUK_PORT_MAPOTN:
            chnl_conn_ptr->chnl_hndl_ptr = (digi_chnl_hndl_t *)&digi_handle->handle_pool.mapper_chnl[chnl_id];
            chnl_conn_ptr->chnl_hndl_type = DIGI_CHANNEL_HANDLE_ID_MAPPER_T;
            break;

          default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        }
    }
    else
    {
        util_global_cpb_port_t port_type = (util_global_cpb_port_t)port_id;
        UINT32 i;

        switch (port_type)
        {
          case UTIL_GLOBAL_CPB_PORT_ILKN1:
            chnl_conn_ptr->chnl_hndl_ptr = (digi_chnl_hndl_t *)&digi_handle->handle_pool.ilkn_1_chnl[chnl_id];
            chnl_conn_ptr->chnl_hndl_type = DIGI_CHANNEL_HANDLE_ID_ILKN_1_T;
            break;
          case UTIL_GLOBAL_CPB_PORT_ILKN2:
            chnl_conn_ptr->chnl_hndl_ptr = (digi_chnl_hndl_t *)&digi_handle->handle_pool.ilkn_2_chnl[chnl_id];
            chnl_conn_ptr->chnl_hndl_type = DIGI_CHANNEL_HANDLE_ID_ILKN_2_T;
            break;
          case UTIL_GLOBAL_CPB_PORT_CBRC:
            chnl_conn_ptr->chnl_hndl_ptr = (digi_chnl_hndl_t *)&digi_handle->handle_pool.cbr_chnl[chnl_id];
            chnl_conn_ptr->chnl_hndl_type = DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T;
            break;
          case UTIL_GLOBAL_CPB_PORT_MAPOTN:
            chnl_conn_ptr->chnl_hndl_ptr = (digi_chnl_hndl_t *)&digi_handle->handle_pool.mapper_chnl[chnl_id];
            chnl_conn_ptr->chnl_hndl_type = DIGI_CHANNEL_HANDLE_ID_MAPPER_T;
            break;
          case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
            /*
             * For the ENET_LINE need to check the ETRANS case. Iterate over
             * the MAPOTN handles to determine if there is a MAPOTN case
             * that is using ENET_LINE and this channel value as part of
             * its cpb_data.
             */
            chnl_conn_ptr->chnl_hndl_ptr = NULL;

            if (TRUE == check_mapotn)
            {
                for (i = 0; i < DIGI_MAPOTN_CHNL_MAX; i++)
                {
                    if ((TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.mapper_chnl[i].switch_data.header.prov_state)) &&
                        UTIL_GLOBAL_CPB_PORT_ENET_LINE == digi_handle->handle_pool.mapper_chnl[i].switch_data.cpb_data.port_type && 
                        chnl_id == digi_handle->handle_pool.mapper_chnl[i].switch_data.cpb_data.channel)
                    {
                        chnl_conn_ptr->chnl_hndl_ptr = (digi_chnl_hndl_t *)&digi_handle->handle_pool.mapper_chnl[i];
                        chnl_conn_ptr->chnl_hndl_type = DIGI_CHANNEL_HANDLE_ID_MAPPER_T;
                        break;
                    }
                }
            }
            /*
             * If the handle is still NULL then this is must be an ENET LINE
             * case so handle that.
             */
            if (NULL == chnl_conn_ptr->chnl_hndl_ptr)
            {
                chnl_conn_ptr->chnl_hndl_ptr = (digi_chnl_hndl_t *)&digi_handle->handle_pool.enet_line_chnl[chnl_id];
                chnl_conn_ptr->chnl_hndl_type = DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T;
            }
            break;

          case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
            chnl_conn_ptr->chnl_hndl_ptr = (digi_chnl_hndl_t *)&digi_handle->handle_pool.enet_sys_chnl[chnl_id];
            chnl_conn_ptr->chnl_hndl_type = DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T;
            break;

          default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
            break;
        }
    }

    PMC_RETURN();
}


/*******************************************************************************
* digi_resource_set_destination_pointer
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle          - Reference to the digi handle
*  connect_state_ptr    - ODUK or CPB connection array.
*  is_oduk_switch       - Whether this is ODUK switch relative
*  is_first             - TRUE if this is the first call and data should be
*                         read from slv.dest_mst_port and not
*                         mst.next_dest_mst_port
*
* OUTPUTS:
*  port_type_ptr        - Port type access as well as the new destination
*                         port for the next iteration if required.
*  chnl_id_ptr          - Channel id access and new destination channel id

*  destination_ptr      - Reference to the destination
*
* RETURNS:
*  TRUE  - Element was found and destination_ptr updated.
*  FALSE - Otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_set_destination_pointer(digi_handle_t *digi_handle,
                                                    util_global_cpb_connect_t *connect_state_ptr,
                                                    BOOL8 is_oduk_switch,
                                                    BOOL8 is_first,
                                                    UINT32 *port_type_ptr,
                                                    UINT32 *chnl_id_ptr,
                                                    digi_chnl_hndl_conn_t *destination_ptr)
{
    PMC_ENTRY();
    UINT32 local_port_type;
    UINT32 local_chnl_id;
    BOOL8 result = TRUE;

    if (TRUE == is_first)
    {
        local_port_type = connect_state_ptr->chnl[*port_type_ptr][*chnl_id_ptr].slv.dest_mst_port;
        local_chnl_id = connect_state_ptr->chnl[*port_type_ptr][*chnl_id_ptr].slv.dest_mst_chnl;
    }
    else
    {
        local_port_type = connect_state_ptr->chnl[*port_type_ptr][*chnl_id_ptr].mst.next_dest_mst_port;
        local_chnl_id = connect_state_ptr->chnl[*port_type_ptr][*chnl_id_ptr].mst.next_dest_mst_chnl;
    }
    if (CPB_INDEX_LAST == local_port_type || 
        CPB_INDEX_LAST == local_chnl_id)
    {
        destination_ptr->chnl_primary = FALSE;
        destination_ptr->chnl_activated = TRUE;
        destination_ptr->chnl_hndl_ptr = NULL;
        destination_ptr->chnl_hndl_type = LAST_DIGI_HANDLE_ID;
        result = FALSE;
    }

    if (TRUE == result)
    {
        digi_resource_convert_port_type(digi_handle,
                                        local_port_type,
                                        local_chnl_id,
                                        is_oduk_switch,
                                        TRUE,
                                        destination_ptr);
    
        if (NULL != destination_ptr->chnl_hndl_ptr)
        {
            destination_ptr->chnl_primary = FALSE;            

            if (TRUE == connect_state_ptr->chnl[local_port_type][local_chnl_id].mst.is_slave_zone_primary)
            {
                destination_ptr->chnl_primary = TRUE;
            }
            
            if (UTIL_GLOBAL_CHNL_OPERATIONAL == connect_state_ptr->chnl[local_port_type][local_chnl_id].mst.state)
            {
                destination_ptr->chnl_activated = TRUE;
            }
        }
    }
    *port_type_ptr = local_port_type;
    *chnl_id_ptr = local_chnl_id;

    PMC_RETURN(result);
} /* digi_resource_set_destination_pointer */


/*******************************************************************************
*   digi_resource_check_mapotn_broadcast_path
* ______________________________________________________________________________
*
* DESCRIPTION:
*  A datapath that was being released detected a broadcast connection
*  before any resources were released. At this point need to check whether
*  the secondary path is valid. If so set its path_verified flag to FALSE
*  so that it can be checked independently of this data path.
*
* INPUTS:
*  digi_handle          - Handle to be released.
*  digi_resource_handle - Digi resource table
*  map_index            - Index into the mapotn array
*  is_oduk_to_cpb       - What is the direction the release is moving in
*  direction            - Direction to be checked. Only 
*                         DIGI_DIRECTION_TX_ONLY or DIGI_DIRECTION_RX_ONLY
*                         are valid
*
* OUTPUTS:
*  None
*
* RETURNS:
*  PMC_SUCCESS - Operation was successful
*  PMC_ERROR   - Otherwise
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_check_mapotn_broadcast_path(digi_handle_t *digi_handle,
                                                        digi_resource_handle_summary_t *digi_resource_handle,
                                                        UINT32 map_index,
                                                        BOOL8 is_oduk_to_cpb,
                                                        digi_direction_check_t direction)
{
    BOOL8 result = FALSE;
    UINT32 cpb_index;

    PMC_ENTRY();

    cpb_index = digi_resource_handle->mapper_cfg[map_index].cpb_switch_index;
    /*
     * Confirm that this is a MAPOTN resource that it is is activated in 
     * both directions at the ODUK switch and it is SERDES connected over
     * the CPB.
     */
    if (cpb_index > 0 &&
        DIGI_OTN_MAPPER == digi_resource_handle->cpbsw_cfg[cpb_index].provisioned_handle_type &&
        map_index == digi_resource_handle->cpbsw_cfg[cpb_index].provisioned_handle_index &&
        TRUE == digi_resource_handle->cpbsw_cfg[cpb_index].source_activated &&
        TRUE == digi_resource_handle->cpbsw_cfg[cpb_index].dest_activated &&
        (DIGI_LINE_SERDES_CFG == digi_resource_handle->cpbsw_cfg[cpb_index].source_handle_type ||
         DIGI_SYS_SERDES_CFG == digi_resource_handle->cpbsw_cfg[cpb_index].source_handle_type ||
         DIGI_SFI51_SERDES_CFG == digi_resource_handle->cpbsw_cfg[cpb_index].source_handle_type ||
         DIGI_LINE_SERDES_CFG == digi_resource_handle->cpbsw_cfg[cpb_index].primary_dest_handle_type ||
         DIGI_SYS_SERDES_CFG == digi_resource_handle->cpbsw_cfg[cpb_index].primary_dest_handle_type ||
         DIGI_SFI51_SERDES_CFG == digi_resource_handle->cpbsw_cfg[cpb_index].primary_dest_handle_type ||
         DIGI_ILKN_1 == digi_resource_handle->cpbsw_cfg[cpb_index].primary_dest_handle_type ||
         DIGI_ILKN_2 == digi_resource_handle->cpbsw_cfg[cpb_index].primary_dest_handle_type))
    {
        digi_serdes_port_intf_ctxt_t *serdes_ptr = NULL;
        UINT32 serdes_index = digi_resource_handle->cpbsw_cfg[cpb_index].primary_dest_index;
        
        if (DIGI_LINE_SERDES_CFG == digi_resource_handle->cpbsw_cfg[cpb_index].primary_dest_handle_type)
        {
            serdes_ptr = &digi_resource_handle->line_serdes_cfg[serdes_index];
        }
        else if (DIGI_SYS_SERDES_CFG == digi_resource_handle->cpbsw_cfg[cpb_index].primary_dest_handle_type)
        {
            serdes_ptr = &digi_resource_handle->sys_serdes_cfg[serdes_index];
        }
        else if (DIGI_SFI51_SERDES_CFG == digi_resource_handle->cpbsw_cfg[cpb_index].primary_dest_handle_type)
        {
            serdes_ptr = &digi_resource_handle->sfi51_serdes_cfg[serdes_index];
        }

        if (NULL != serdes_ptr &&
            DIGI_CPB_SWITCH == serdes_ptr->next_resource_handle)
        {
            UINT32 index = serdes_ptr->next_handle_index;

            if (TRUE == digi_resource_handle->cpbsw_cfg[index].source_activated &&
                TRUE == digi_resource_handle->cpbsw_cfg[index].dest_activated)
            {
                result = TRUE;
                serdes_ptr->path_verified = FALSE;
            }
        }
        else if (DIGI_ILKN_1 == digi_resource_handle->cpbsw_cfg[cpb_index].primary_dest_handle_type ||
                 DIGI_ILKN_2 == digi_resource_handle->cpbsw_cfg[cpb_index].primary_dest_handle_type)
        {
            result = TRUE;
        }
    }

    PMC_RETURN(result);
} /* digi_resource_check_mapotn_broadcast_path */

/*******************************************************************************
* digi_resource_release_mapotn_resources
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle          - Reference to the digi handle
*  chnl_hndl_ptr        - Reference to the handle to be released
*  mapotn_release       - Whether to release MAPOTN resource
*  oduk_first           - Whether this call was generated by ODUK going to
*                         CPB (TRUE) or the other way.
** OUTPUTS:
*
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_resource_release_mapotn_resources(digi_handle_t *digi_handle,
                                                         digi_chnl_hndl_t *chnl_hndl_ptr,
                                                         BOOL8 mapotn_release,
                                                         BOOL8 oduk_first)
{
    PMC_ERROR result = PMC_SUCCESS;
    util_global_switch_data_t *input_pc_ptr = NULL;
    util_global_switch_data_def_t *mapotn_data_ptr =  (util_global_switch_data_def_t *)chnl_hndl_ptr;
    BOOL8 is_loopback = FALSE;
    UINT32 dest_chnl = 0;

    PMC_ENTRY();

    if (TRUE == oduk_first)
    {
        /*
         * Retrieve the input pointer and deactivate it at the CPB. This is
         * required to ensure that any related MPMO resources are also released.
         */
        util_global_cpb_port_t dest_port;
        UINT32 map_port = (UINT32)mapotn_data_ptr->cpb_data.port_type;
        util_global_cpb_connect_t *cpb_connect_data_ptr = NULL;

        cpb_connect_data_state_get(digi_handle->dcpb_handle,
                                   &cpb_connect_data_ptr);
        dest_port = (util_global_cpb_port_t)cpb_connect_data_ptr->chnl[map_port][mapotn_data_ptr->cpb_data.channel].slv.dest_mst_port;
        dest_chnl = cpb_connect_data_ptr->chnl[map_port][mapotn_data_ptr->cpb_data.channel].slv.dest_mst_chnl;

        if (CPB_INDEX_LAST != dest_port && CPB_INDEX_LAST != dest_chnl)
        {
            input_pc_ptr = digi_cpb_retrieve_input_port(digi_handle,
                                                        dest_port,
                                                        dest_chnl);
        }
    }
    else
    {
        util_global_oduk_port_t dest_port;
        util_global_cpb_connect_t *oduk_connect_data_ptr = NULL;

        coreotn_oduksw_connect_data_state_get(digi_handle->coreotn_handle,
                                              &oduk_connect_data_ptr);
        dest_port = (util_global_oduk_port_t)oduk_connect_data_ptr->chnl[3][mapotn_data_ptr->oduk_data.channel].slv.dest_mst_port;
        dest_chnl = oduk_connect_data_ptr->chnl[3][mapotn_data_ptr->oduk_data.channel].slv.dest_mst_chnl;

        if (CPB_INDEX_LAST != dest_port && CPB_INDEX_LAST != dest_chnl)
        {
            input_pc_ptr = digi_oduksw_input_port_get(digi_handle, 
                                                      dest_port,
                                                      dest_chnl,
                                                      DIGI_ILKN_CHNL_TARGET_ODUK);
        }
    }
    if (NULL != input_pc_ptr && TRUE == oduk_first)
    {
        result = digi_cpb_chnl_deactivate(digi_handle, input_pc_ptr);
    }
    else if (NULL != input_pc_ptr)
    {
        result = digi_oduksw_chnl_deactivate(digi_handle, input_pc_ptr);
    }
    else if ((UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == mapotn_data_ptr->oduk_data.op_state.active_state && FALSE == oduk_first) ||
             (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == mapotn_data_ptr->cpb_data.op_state.active_state && TRUE == oduk_first))
    {
        result = DIGI_ERR_NULL_HANDLE;
    }
    /*
     * Need to ensure that MAPOTN is not loopback at the CPB so as not to
     * do all this twice.
     */
    if (PMC_SUCCESS == result && TRUE == oduk_first && NULL != input_pc_ptr)
    {
        util_global_switch_data_def_t *input_data_ptr = (util_global_switch_data_def_t *)input_pc_ptr;

        if (mapotn_data_ptr->cpb_data.port_type == input_data_ptr->cpb_data.port_type &&
            mapotn_data_ptr->cpb_data.channel ==input_data_ptr->cpb_data.channel)
        {
            is_loopback = TRUE;
        }
    }
    else if (PMC_SUCCESS == result && NULL != input_pc_ptr)
    {
        util_global_switch_data_def_t *input_data_ptr = (util_global_switch_data_def_t *)input_pc_ptr;

        if (mapotn_data_ptr->oduk_data.port_type == input_data_ptr->oduk_data.port_type &&
            mapotn_data_ptr->oduk_data.channel == input_data_ptr->oduk_data.channel)
        {
            is_loopback = TRUE;
        }
    }
    /*
     * If not loopbacked then deactivate MAPOTN at the CPB.  If loopback
     * then theoretically it already occurred above.
     */
    if (PMC_SUCCESS == result && FALSE == is_loopback)
    {
        if (TRUE == oduk_first && UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == mapotn_data_ptr->cpb_data.op_state.active_state)
        {
            result = digi_cpb_chnl_deactivate(digi_handle, chnl_hndl_ptr);
        }
        else if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == mapotn_data_ptr->oduk_data.op_state.active_state)
        {
            result = digi_oduksw_chnl_deactivate(digi_handle, chnl_hndl_ptr);
        }
    }

    /*
     * Regardless release the oduksw mapping for the mapotn handle as that
     * is required to be done.
     */
    if (PMC_SUCCESS == result && TRUE == oduk_first  &&
        TRUE == mapotn_data_ptr->oduk_data.op_state.map_state.is_output_mapped)
    {
        result = digi_oduksw_chnl_demap(digi_handle,
                                        chnl_hndl_ptr);        
    }
    else if (PMC_SUCCESS == result && 
             TRUE == mapotn_data_ptr->cpb_data.op_state.map_state.is_output_mapped)
    {
          result = digi_cpb_chnl_demap(digi_handle,
                                       chnl_hndl_ptr);              
    }

    /*
     * If mapotn is being released and this is not a loop back then need to
     * ensure that whatever is connected to mapotn is also release.
     */
    if (PMC_SUCCESS == result && TRUE == mapotn_release && 
        FALSE == is_loopback && NULL != input_pc_ptr)
    {
        if (TRUE == oduk_first)
        {
            result = digi_cpb_chnl_demap(digi_handle, input_pc_ptr);
        }
        else
        {
            result = digi_oduksw_chnl_demap(digi_handle, input_pc_ptr);
        }
    }

    /*
     * demap the cpb resource for mapotn if this is a full mapotn release
     * and this started from the ODUK side. Otherwise release the ODUK
     * mapping.
     */
    if (PMC_SUCCESS == result && TRUE == mapotn_release)
    {
        if (TRUE == oduk_first &&
            TRUE == mapotn_data_ptr->cpb_data.op_state.map_state.is_output_mapped)
        {
            result = digi_cpb_chnl_demap(digi_handle, chnl_hndl_ptr);
        }
        else if (TRUE == mapotn_data_ptr->oduk_data.op_state.map_state.is_output_mapped)
        {
            result = digi_oduksw_chnl_demap(digi_handle, chnl_hndl_ptr);
        }
    }

    PMC_RETURN(result);
} /* digi_resource_release_mapotn_resources */

/*******************************************************************************
* digi_resource_set_directional_result
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Utility function for setting the result based on the direction and the
*  rx ot tx result.
*
* INPUTS:
*   direction          - Data path direction being verified
*   tx_result          - Result of the transmit direction
*   rx_result          - Result of the receive direction
*
* OUTPUTS:
*  None
*
* RETURNS:
*  TRUE  - Directional result is valid.
*  FALSE - Otherwise
*
*******************************************************************************/
PRIVATE BOOL8  digi_resource_set_directional_result(digi_direction_check_t direction,
                                                    BOOL8 tx_result,
                                                    BOOL8 rx_result)
{
    BOOL8 result = FALSE;


    PMC_ENTRY();
    
    if ((DIGI_DIRECTION_TX_RX == direction) &&
        (TRUE == tx_result && TRUE == rx_result))
    {
        result = TRUE;
    }
    else if (DIGI_DIRECTION_TX_ONLY == direction && TRUE == tx_result)
    {
        result = TRUE;
    }
    else if (DIGI_DIRECTION_RX_ONLY == direction && TRUE == rx_result)
    {
        result = TRUE;
    }

    PMC_RETURN(result);
} /* digi_resource_set_directional_result */

/*******************************************************************************
*   digi_resource_restart_release_mapotn
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Based on the direction releases MAPOTN resources if applicable. If
*  not looks for the next connection and determines if that resources needs
*  to be released.
*
* INPUTS:
*  digi_handle          - Handle to be released.
*  digi_resource_handle - Digi resource table
*  map_index            - Index into the mapotn array
*  is_oduk_to_cpb       - What is the direction the release is moving in
*  direction            - Direction to be checked. Only 
*                         DIGI_DIRECTION_TX_ONLY or DIGI_DIRECTION_RX_ONLY
*                         are valid
*
* OUTPUTS:
*  None
*
* RETURNS:
*  PMC_SUCCESS - Operation was successful
*  PMC_ERROR   - Otherwise
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_restart_release_mapotn(digi_handle_t *digi_handle,
                                                   digi_resource_handle_summary_t *digi_resource_handle,
                                                   UINT32 map_index,
                                                   BOOL8 is_oduk_to_cpb,
                                                   digi_direction_check_t direction)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 switch_index;
    BOOL8 is_mapotn_loopback;
    digi_resource_next_handle_t source_type = LAST_DIGI_RESOURCE_HANDLE;
    UINT16 source_index = 0;
    static BOOL8 has_been_visited = FALSE;

    PMC_ENTRY();

    if (TRUE == has_been_visited)
    {
        PMC_RETURN(PMC_SUCCESS);
    }
    else if (NULL == digi_resource_handle->mapper_cfg[map_index].mapper_handle_ptr)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "MAPOTN at MapIndex=%d NULL. Already released??\n",
                map_index);
        PMC_RETURN(PMC_SUCCESS);
    }
    has_been_visited = TRUE;
 
    switch_index = digi_resource_handle->mapper_cfg[map_index].oduk_switch_index;
    /*
     * Check whether there is a loop back at the ODUK switch. For
     * this side ignore whether this is called from ODUK side or CPB
     */
    is_mapotn_loopback = digi_resource_is_mapotn_loopback(&digi_resource_handle->oduksw_cfg[switch_index], &source_type, &source_index);

    /*
     * This is a broadcast/multicast case coming from ODUK to CPB
     * meaning this was started from OTN connections. This is likely the
     * Rx direction. If the MAPOTN connection is fully connected then need
     * check the CPB side. If that is fully connected then then this side
     * of the release is done. 
     */
    if (FALSE == digi_resource_handle->force_release &&
        FALSE == digi_resource_handle->datapath_release &&
        TRUE == is_oduk_to_cpb &&
        digi_resource_handle->oduksw_cfg[switch_index].num_dest > 1 &&
        TRUE == digi_resource_handle->oduksw_cfg[switch_index].source_activated &&
        TRUE == digi_resource_handle->oduksw_cfg[switch_index].dest_activated)
    {
        if (TRUE == digi_resource_check_mapotn_broadcast_path(digi_handle,
                                                              digi_resource_handle,
                                                              map_index,
                                                              is_oduk_to_cpb,
                                                              direction))
        {
            has_been_visited = FALSE;
            PMC_RETURN(PMC_SUCCESS);
        }
    }
    /*
     * If this is TX call and the primary_destination handle is unset
     * then there is nothing to release. Similarly if RX and there is
     * no source then there is nothing to release.
     * Note that the perspective is from which side this is being 
     * called. From the ODUK side (first caller) then TX aplies to
     * primary. However if called from the CPB then the TX aplies to
     * the source side.  The reverse is true for the CPB check below.
     * NOTE: WILL NEED TO LOOK AT SECONDARY DESTINATION HANDLE FOR
     * BROADCAST CASE AND DETERMINE THE VALIDITY OF THAT.
     */
    if (DIGI_OTN_MAPPER == digi_resource_handle->oduksw_cfg[switch_index].provisioned_handle_type && switch_index > 0)
    {
        digi_resource_restart_release_switch_chnl(digi_handle, 
                                                  digi_resource_handle,
                                                  DIGI_ODUK_SWITCH,
                                                  switch_index,
                                                  is_oduk_to_cpb,
                                                  direction);
    }
    switch_index = digi_resource_handle->mapper_cfg[map_index].cpb_switch_index;
    if (FALSE == is_mapotn_loopback)
    {
        is_mapotn_loopback = digi_resource_is_mapotn_loopback(&digi_resource_handle->cpbsw_cfg[switch_index], &source_type, &source_index);
    }    

    /*
     * if coming from the ODUK then TX applies to the source handle
     * and RX applies to the primary_destination handle. If this was
     * initiated from the CPB then it is primary associated with TX and
     * source associated with RX.
     */
    if (DIGI_OTN_MAPPER == digi_resource_handle->cpbsw_cfg[switch_index].provisioned_handle_type && switch_index > 0)
    {
        digi_resource_restart_release_switch_chnl(digi_handle, 
                                                  digi_resource_handle,
                                                  DIGI_CPB_SWITCH,
                                                  switch_index,
                                                  is_oduk_to_cpb,
                                                  direction);
    }

    if (FALSE == is_mapotn_loopback)
    {
        if (TRUE == is_oduk_to_cpb)
        {
            /*
             * Need to add a check to ensure that the ILKN channel handle
             * is processed. If this is TX and primary_dest_handle is ILKN
             * 1 or 2 then update the source_type as this resource has not
             * been checked. 
             */
            if (source_type != digi_resource_handle->cpbsw_cfg[switch_index].primary_dest_handle_type && 
              (DIGI_ILKN_1 ==  digi_resource_handle->cpbsw_cfg[switch_index].primary_dest_handle_type || 
               DIGI_ILKN_2 ==  digi_resource_handle->cpbsw_cfg[switch_index].primary_dest_handle_type))
            {
                source_type =  digi_resource_handle->cpbsw_cfg[switch_index].primary_dest_handle_type;
                source_index = digi_resource_handle->cpbsw_cfg[switch_index].primary_dest_index;
            }
            digi_resource_release_next_chnl(digi_handle,
                                            digi_resource_handle,
                                            source_type,
                                            source_index,
                                            direction);
        }
    }
    /*
     * Add the check for th NULL handle pointer since this will be 
     * called twice. Once for ODUK Switch mapping and once for CPB if
     * MAPOTN is mapped across both switches.
     * NOTE: Need to add a function that checks the state of the mapotn
     * channel handle rather than the resource table.  It is more 
     * conclusive. The function should take a util_global_switch_data_t and
     * case it to util_global_switch_data_def_t and then check it
     */
    if (PMC_SUCCESS == result && 
        NULL != digi_resource_handle->mapper_cfg[map_index].mapper_handle_ptr && 
        TRUE == digi_resource_is_unconnected(digi_handle,
                                             digi_resource_handle->mapper_cfg[map_index].mapper_handle_ptr))
    {
        digi_resource_otn_mapper_deprov(digi_handle, 
                                        &digi_resource_handle->mapper_cfg[map_index].mapper_handle_ptr);
        digi_resource_handle->datapath_release = TRUE;
    }
    has_been_visited = FALSE;

    switch_index = digi_resource_handle->mapper_cfg[map_index].oduk_switch_index;
    if (LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->oduksw_cfg[switch_index].associate_dest_handle_type)
      {
        result = digi_resource_restart_release_port(digi_handle,
                                                    digi_resource_handle,
                                                    NULL,
                                                    digi_resource_handle->oduksw_cfg[switch_index].associate_dest_handle_type,
                                                    digi_resource_handle->oduksw_cfg[switch_index].associate_dest_index,
                                                    TRUE,
                                                    direction);
    }

    PMC_RETURN(result);
} /* digi_resource_restart_release_mapotn */


/*******************************************************************************
*  digi_resource_evaluate_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
* Determines whether a check is required based on the direction
*
* INPUTS:
*  digi_handle          - Handle to be released.
*  digi_resource_handle - Digi resource table
*  is_oduk_caller       - Whether call is relative to ODUK or CPB
*  prov_ptr             - Channel handle being examined whose switch 
*                         relative active state will be checked.
*  direction            - Direction to be checked. Only 
*                         DIGI_DIRECTION_TX_ONLY or DIGI_DIRECTION_RX_ONLY
*                         are valid
*  switch_index         - Index of the switch to be checked.
*  is_mapper            - If applying to MAPOTN (TRUE) then the evaluation
*                         is backwards.
*
* OUTPUTS:
*
* RETURNS:
*  TRUE  - Directional result is valid.
*  FALSE - Otherwise
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_evaluate_activate(digi_handle_t *digi_handle,
                                              digi_resource_handle_summary_t *digi_resource_handle,
                                               BOOL8 is_oduk_caller,
                                               util_global_switch_data_t *prov_ptr,
                                               digi_direction_check_t direction,
                                               UINT32 switch_index,
                                               BOOL8 is_mapper)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();
    /*
     * If  this is on the left hand side of the switch and the direction
     * is TX or force release is TRUE and the relative switch is active 
     * either in the resource table entry or the channel handle then want
     * to deactivate.  Conversely if on the right hand side of the switch
     * then this evaluation applies to the RX direction.
     */
    if ((FALSE == is_mapper && DIGI_DIRECTION_TX_ONLY == direction) ||
        TRUE == digi_resource_handle->force_release)
    {
        if (FALSE == is_oduk_caller &&
            (TRUE == digi_resource_handle->cpbsw_cfg[switch_index].dest_activated || UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == ((util_global_switch_data_def_t *)prov_ptr)->cpb_data.op_state.active_state))
        {
            result = TRUE;
        }
        else if (TRUE == is_oduk_caller &&
                 (TRUE == digi_resource_handle->oduksw_cfg[switch_index].dest_activated || UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == ((util_global_switch_data_def_t *)prov_ptr)->oduk_data.op_state.active_state))
        {
            result = TRUE;
        }
    }
    else if ((TRUE == is_mapper && DIGI_DIRECTION_RX_ONLY == direction) ||
             TRUE == digi_resource_handle->force_release)
    {
        if (FALSE == is_oduk_caller && 
            (TRUE == digi_resource_handle->cpbsw_cfg[switch_index].dest_activated || UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == ((util_global_switch_data_def_t *)prov_ptr)->cpb_data.op_state.active_state))
        {
            result = TRUE;
        }
        else if (TRUE == is_oduk_caller && 
                 (TRUE == digi_resource_handle->oduksw_cfg[switch_index].dest_activated || UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == ((util_global_switch_data_def_t *)prov_ptr)->oduk_data.op_state.active_state))
        {
            result = TRUE;
        }
    }

    PMC_RETURN(result);
} /* digi_resource_evaluate_activate */

/*******************************************************************************
*  digi_resource_evaluate_mapping
* ______________________________________________________________________________
*
* DESCRIPTION:
* Determines whether a check is required based on the direction
*
* INPUTS:
*  digi_handle          - Handle to be released.
*  digi_resource_handle - Digi resource table
*  is_oduk_caller       - Whether call is relative to ODUK or CPB
*  prov_ptr             - Channel handle whose output mapped flag for the
*                         particular switch is to be evaluated.
*  direction            - Direction to be checked. Only 
*                         DIGI_DIRECTION_TX_ONLY or DIGI_DIRECTION_RX_ONLY
*                         are valid
*  switch_index         - Index of the switch to be checked.
*  is_mapper            - If applying to MAPOTN (TRUE) then the evaluation
*                         is backwards.
*
* OUTPUTS:
*
* RETURNS:
*  TRUE  - Directional result is valid.
*  FALSE - Otherwise
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_evaluate_mapping(digi_handle_t *digi_handle,
                                             digi_resource_handle_summary_t *digi_resource_handle,
                                             BOOL8 is_oduk_caller,
                                             util_global_switch_data_t *prov_ptr,
                                             digi_direction_check_t direction,
                                             UINT32 switch_index,
                                             BOOL8 is_mapper)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if ((FALSE == is_mapper && DIGI_DIRECTION_TX_ONLY == direction) ||
        TRUE == digi_resource_handle->force_release)
    {
        if (FALSE == is_oduk_caller && 
            (LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->cpbsw_cfg[switch_index].source_handle_type || TRUE == ((util_global_switch_data_def_t *)prov_ptr)->cpb_data.op_state.map_state.is_output_mapped))
        {
            result = TRUE;
        }
        else if (TRUE == is_oduk_caller && 
                 (LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->oduksw_cfg[switch_index].source_handle_type || TRUE == ((util_global_switch_data_def_t *)prov_ptr)->oduk_data.op_state.map_state.is_output_mapped))
        {
            result = TRUE;
        }
    }
    else if ((TRUE == is_mapper && DIGI_DIRECTION_RX_ONLY == direction) ||
             TRUE == digi_resource_handle->force_release)
    {
        if (FALSE == is_oduk_caller && 
            (LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->cpbsw_cfg[switch_index].source_handle_type || TRUE == ((util_global_switch_data_def_t *)prov_ptr)->cpb_data.op_state.map_state.is_output_mapped))
        {
            result = TRUE;
        }
        else if (TRUE == is_oduk_caller && 
                 (LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->oduksw_cfg[switch_index].source_handle_type || TRUE == ((util_global_switch_data_def_t *)prov_ptr)->oduk_data.op_state.map_state.is_output_mapped))
        {
            result = TRUE;
        }
    }

    PMC_RETURN(result);
} /* digi_resource_evaluate_mapping */

/*******************************************************************************
*  digi_resource_is_unconnected
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Casts the channel handle into a util_global_switch_data_def_t and 
*  returns TRUE if it is not connected to anything. That is it can be
*  deprovisioned
*
* INPUTS:
*  digi_handle          - Handle to be released.
*  chnl_hndl_ptr        - Void * reference
*
* OUTPUTS:
*  None
*
* RETURNS:
*  TRUE  - Resource is unconnected
*  FALSE - Otherwise
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_is_unconnected(digi_handle_t *digi_handle,
                                           util_global_switch_data_t *chnl_hndl_ptr)
{
    BOOL8 result = FALSE;
    util_global_switch_data_def_t *local_hndl_ptr = (util_global_switch_data_def_t *)chnl_hndl_ptr;
    PMC_ENTRY();

    if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE != local_hndl_ptr->cpb_data.op_state.active_state && 
        0 == local_hndl_ptr->cpb_data.op_state.map_state.input_map_count &&
        0 == local_hndl_ptr->cpb_data.op_state.map_state.dest_active_count &&
        FALSE == local_hndl_ptr->cpb_data.op_state.map_state.is_input_mapped &&
        FALSE == local_hndl_ptr->cpb_data.op_state.map_state.is_output_mapped &&
        UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE != local_hndl_ptr->oduk_data.op_state.active_state && 
        0 == local_hndl_ptr->oduk_data.op_state.map_state.input_map_count &&
        0 == local_hndl_ptr->oduk_data.op_state.map_state.dest_active_count &&
        FALSE == local_hndl_ptr->oduk_data.op_state.map_state.is_input_mapped &&
        FALSE == local_hndl_ptr->oduk_data.op_state.map_state.is_output_mapped)
    {
        result = TRUE;
    }

    PMC_RETURN(result);
} /* digi_resource_is_unconnected */


/*******************************************************************************
*  digi_resource_set_valid_path
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Sets the path settings in the otn_contaier_ptr based on either the
*  direction or whether this is a sysotn use.
*
* INPUTS:
*  digi_handle          - Handle to be released.
*  otn_handle           - Handle type being referenced
*  direction            - Direction the datapath is being scanned.
*  is_sysotn            - Whether sysotn considerations need to be used.
*  result               - Value to be assigned.
*
* OUTPUTS:
*  otn_container_ptr    - OTN resource table handle being updated.
*
* RETURNS:
*  TRUE  - Resource is unconnected
*  FALSE - Otherwise
*
*******************************************************************************/
PRIVATE void digi_resource_set_valid_path(digi_handle_t *digi_handle,
                                          digi_resource_next_handle_t otn_handle,
                                          digi_direction_check_t direction,
                                          BOOL8 is_sysotn,
                                          BOOL8 result,
                                          digi_otn_container_cfg_t *otn_container_ptr)
{
    PMC_ENTRY();

    if (TRUE == is_sysotn && TRUE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle))
    {
        if (DIGI_DIRECTION_TX_ONLY == direction && FALSE == otn_container_ptr->tx_path_is_valid)
        {
            otn_container_ptr->tx_path_is_valid = result;
        }
        else if (DIGI_DIRECTION_RX_ONLY == direction && FALSE == otn_container_ptr->rx_path_is_valid)
        {
            otn_container_ptr->rx_path_is_valid = result;
        }

    }
    else if (DIGI_DIRECTION_TX_ONLY == direction)
    {
        otn_container_ptr->tx_path_is_valid = result;
    }
    else if (DIGI_DIRECTION_RX_ONLY == direction)
    {
        otn_container_ptr->rx_path_is_valid = result;
    }
    else if (DIGI_DIRECTION_TX_RX == direction)
    {
        otn_container_ptr->tx_path_is_valid = result;
        otn_container_ptr->rx_path_is_valid = result;
    }

    PMC_RETURN();
} /* digi_resource_set_valid_path */

/*******************************************************************************
*  digi_resource_is_direction_valid
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The direction is relative to the way the serdes entity is called. Namely
*  the specific determination based on the direction is also based on 
*  whether is called directly from digi_handle_restart_init or as a result
*  of checking intiiated from another serdes port.
*
* INPUTS:
*  direction            - Direction the data path is being checked.
*  check_all            - If TRUE indicates called directly from digi
*                         handle_restart_init. False otherwise.
*  active_state         - State relative to the particular data handle
*  input_map_count      - Map count of the particular channel handle
*  switch_ptr           - References to the switch pointer being checked
*  
*
* OUTPUTS:
*  is_tx_behaviour_ptr  - Reference to the tx behaviour indicated by the
*                         direction and check_all flag,
*
* RETURNS:
*  TRUE  - Resource is unconnected
*  FALSE - Otherwise
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_is_direction_valid(digi_direction_check_t direction,
                                               BOOL8 check_all,
                                               util_global_chnl_activated_state_t active_state,
                                               UINT32 input_map_count,
                                               digi_switch_ctxt_t *switch_ptr,
                                               BOOL8 *is_tx_behaviour_ptr)
{
    BOOL8 result = FALSE;
    BOOL8 is_tx_behaviour = FALSE;

    PMC_ENTRY();

    /*
     * The check for which side is activated is a function of the
     * direction and whether coming at this from the line side or the
     * system side. If the line side and direction is TX then the 
     * expectation is the destination is activated as the line side is
     * receiving data from some entity. However if the SYS side then
     * that relationship turns around and it is transmitting data to 
     * some resource and thus its source needs to be activated.
     */
    if (FALSE == check_all)
    {
        if (DIGI_DIRECTION_TX_ONLY == direction)
        {
            is_tx_behaviour = FALSE;
        }
        else
        {
            is_tx_behaviour = TRUE;
        }
    }
    else if (DIGI_DIRECTION_TX_ONLY == direction)
    {
        is_tx_behaviour = TRUE;
    }
    else
    {
        is_tx_behaviour = FALSE;
    }
    
    if (TRUE == is_tx_behaviour &&
        (FALSE == switch_ptr->dest_activated || 
         UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE != active_state))
    {
        /*
         * For TX check the destination is operational.
         */
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "Direction=%d not OPERATIONAL on CPB\n", direction);
    }
    else if (FALSE == is_tx_behaviour &&
             (FALSE == switch_ptr->source_activated ||
              input_map_count < 1))
    {
        /*
         * This is the RX diection and need to check whether this is 
         * possible a broadcast and that the broadcast is a loopback to
         * the source side of this connection. In that case this would be
         * an exception for this datapath.  As long as the rest of the
         * data path in the RX direction was correctly configured then
         * this is a valid scenario.
         */
        if (LAST_DIGI_RESOURCE_HANDLE == switch_ptr->associate_dest_handle_type)
        {
            PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                    "Direction=%d not OPERATIONAL on CPB\n", direction);
        }
        else if (!(switch_ptr->source_handle_type == switch_ptr->associate_dest_handle_type &&
                   switch_ptr->source_handle_index == switch_ptr->associate_dest_index))
        {
            PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                    "Direction=RX and associate handled defined. Not the same as the source however. Seting state as not OPERATIONAL\n");
        }
        else
        {
            result = TRUE;
        }
    }
    else
    {
        result = TRUE;
    }
    *is_tx_behaviour_ptr = is_tx_behaviour;

    PMC_RETURN(result);
} /* digi_resource_is_direction_valid */


/*******************************************************************************
*  digi_resource_get_input_ptr
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Used to return the channel handle that is associated with an output
*  pointer being demapped or deactivated on the CPB switch.
*
* INPUTS:
*  digi_handle           - Reference to the digi_handle
*  digi_resource_handle  - Reference to the resource table.
*  input_handle          - Resource handle being retrieved.
*  input_index           - Index of the handle being retrieved
*  
*
* OUTPUTS:
*  None
*
* RETURNS:
*  Returns a void* pointer to the handle if found or NULL otherwise.
*
*******************************************************************************/
PRIVATE digi_chnl_hndl_t *digi_resource_get_input_pointer(digi_handle_t *digi_handle,
                                                          digi_resource_handle_summary_t *digi_resource_handle,
                                                          digi_resource_next_handle_t input_handle,
                                                          UINT32 input_index)
{
    digi_chnl_hndl_t *input_ptr = NULL;

    PMC_ENTRY();

    switch (input_handle)
      {
      case DIGI_OTN_MAPPER:
        input_ptr = digi_resource_handle->mapper_cfg[input_index].mapper_handle_ptr;
        break;

      case DIGI_ILKN_1:
        input_ptr = digi_resource_handle->ilkn_chnl_cfg[0][input_index].ilkn_chnl_handle_ptr;
        break;

      case DIGI_ILKN_2:
        input_ptr = digi_resource_handle->ilkn_chnl_cfg[1][input_index].ilkn_chnl_handle_ptr;
        break;

      case DIGI_LINE_SERDES_CFG:
        input_ptr = digi_resource_handle->line_serdes_cfg[input_index].chnl_handle_ptr;
        break;

      case DIGI_SYS_SERDES_CFG:
       input_ptr = digi_resource_handle->sys_serdes_cfg[input_index].chnl_handle_ptr;
        break;

      case DIGI_SFI51_SERDES_CFG:
        input_ptr = digi_resource_handle->sfi51_serdes_cfg[input_index].chnl_handle_ptr;
        break;

      case DIGI_LINE_OTN_HO_CONTAINER:
        input_ptr = digi_resource_handle->line_otn_ho_container_cfg[input_index].chnl_handle_ptr;
        break;

      case DIGI_LINE_OTN_MO_CONTAINER:
        input_ptr = digi_resource_handle->line_otn_mo_container_cfg[input_index].chnl_handle_ptr;
        break;

      case DIGI_LINE_OTN_LO_CONTAINER:
        input_ptr = digi_resource_handle->line_otn_lo_container_cfg[input_index].chnl_handle_ptr;
        break;

      case DIGI_SYS_OTN_HO_CONTAINER:
        input_ptr = digi_resource_handle->sys_otn_ho_container_cfg[input_index].chnl_handle_ptr;
        break;

      case DIGI_SYS_OTN_MO_CONTAINER:
        input_ptr = digi_resource_handle->sys_otn_mo_container_cfg[input_index].chnl_handle_ptr;
        break;

      default:
        break;
      }
  
    PMC_RETURN(input_ptr);
} /* digi_resource_get_input_pointer */


/*******************************************************************************
*  digi_resource_check_src_dest
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Based on the switch type, the provisioned resource and the direction
*  determines whether the source or the destination connetivity should be
*  checked.
*
* INPUTS:
*  digi_handle           - Reference to the digi_handle
*  digi_resource_handle  - Reference to the resource table.
*  is_oduk_switch        - Is this ODUK or CPB switch relative.
*  is_oduk_to_cpb        - Flag indicating on which side of the switch the
*                          evaluation is being made on.
*  next_index            - Switch index in the resource table for the
*                          provisioned handle.
*  direction             - Direction the datapath is being checked in.
*
* OUTPUTS:
*  check_dest_ptr        - Set to TRUE if the destination side should be
*                          checked. Set to FALSE otherwise
*  check_src_ptr         - Set to TRUE if the source side should be checked
*                          FALSE otherwise.
*
* RETURNS:
*  None
*
*******************************************************************************/
PRIVATE void digi_resource_check_src_dest(digi_handle_t *digi_handle,
                                          digi_resource_handle_summary_t *digi_resource_handle,
                                          BOOL8 is_oduk_switch,
                                          BOOL8 is_oduk_to_cpb,
                                          UINT32 next_index,
                                          digi_direction_check_t direction,
                                          BOOL8 *check_dest_ptr,
                                          BOOL8 *check_src_ptr)
{
    PMC_ENTRY();

    *check_src_ptr = FALSE;
    *check_src_ptr = FALSE;

    if (FALSE == is_oduk_to_cpb && TRUE == is_oduk_switch &&
        DIGI_OTN_MAPPER == digi_resource_handle->oduksw_cfg[next_index].provisioned_handle_type)
    {
        if (DIGI_DIRECTION_RX_ONLY == direction &&
            (DIGI_LINE_OTN_HO_CONTAINER == 
             digi_resource_handle->oduksw_cfg[next_index].primary_dest_handle_type ||
             DIGI_LINE_OTN_MO_CONTAINER == 
             digi_resource_handle->oduksw_cfg[next_index].primary_dest_handle_type ||
             DIGI_LINE_OTN_LO_CONTAINER == 
             digi_resource_handle->oduksw_cfg[next_index].primary_dest_handle_type))
        {
            *check_dest_ptr = TRUE;
        }
        else if (DIGI_DIRECTION_TX_ONLY == direction &&
                 (DIGI_LINE_OTN_HO_CONTAINER == 
                  digi_resource_handle->oduksw_cfg[next_index].primary_dest_handle_type ||
                  DIGI_LINE_OTN_MO_CONTAINER == 
                  digi_resource_handle->oduksw_cfg[next_index].primary_dest_handle_type ||
                  DIGI_LINE_OTN_LO_CONTAINER == 
                  digi_resource_handle->oduksw_cfg[next_index].primary_dest_handle_type))
        {
            *check_src_ptr = TRUE;
        }
    }
    if (TRUE == *check_src_ptr || TRUE == *check_dest_ptr)
    {
        PMC_RETURN();
    }
    if (TRUE == is_oduk_switch)
    {
        if (LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->oduksw_cfg[next_index].source_handle_type &&
            !(digi_resource_handle->oduksw_cfg[next_index].provisioned_handle_type == digi_resource_handle->oduksw_cfg[next_index].source_handle_type &&
              digi_resource_handle->oduksw_cfg[next_index].provisioned_handle_index == digi_resource_handle->oduksw_cfg[next_index].source_handle_index) &&
            (DIGI_ILKN_1 == digi_resource_handle->oduksw_cfg[next_index].source_handle_type ||
             DIGI_ILKN_2 == digi_resource_handle->oduksw_cfg[next_index].source_handle_type ||
             DIGI_OTN_MAPPER == digi_resource_handle->oduksw_cfg[next_index].source_handle_type))
        {
            *check_src_ptr = TRUE;
        }
        else if (TRUE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle) &&
                 (DIGI_SYS_OTN_HO_CONTAINER == digi_resource_handle->oduksw_cfg[next_index].provisioned_handle_type ||
                  DIGI_SYS_OTN_MO_CONTAINER == digi_resource_handle->oduksw_cfg[next_index].provisioned_handle_type) &&
                 (DIGI_LINE_OTN_HO_CONTAINER == digi_resource_handle->oduksw_cfg[next_index].source_handle_type))
        {
            *check_src_ptr = TRUE;
        }
        

        if (LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->oduksw_cfg[next_index].primary_dest_handle_type &&                  
            !(digi_resource_handle->oduksw_cfg[next_index].provisioned_handle_type == digi_resource_handle->oduksw_cfg[next_index].primary_dest_handle_type &&
              digi_resource_handle->oduksw_cfg[next_index].provisioned_handle_index == digi_resource_handle->oduksw_cfg[next_index].primary_dest_index) &&
            (DIGI_ILKN_1 == digi_resource_handle->oduksw_cfg[next_index].primary_dest_handle_type ||
             DIGI_ILKN_2 == digi_resource_handle->oduksw_cfg[next_index].primary_dest_handle_type ||
             DIGI_OTN_MAPPER == digi_resource_handle->oduksw_cfg[next_index].primary_dest_handle_type ||
             DIGI_SYS_OTN_MO_CONTAINER == digi_resource_handle->oduksw_cfg[next_index].primary_dest_handle_type))
        {
            *check_dest_ptr = TRUE;
        }
        else if (TRUE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle) &&
                 (DIGI_SYS_OTN_HO_CONTAINER == digi_resource_handle->oduksw_cfg[next_index].provisioned_handle_type ||
                  DIGI_SYS_OTN_MO_CONTAINER == digi_resource_handle->oduksw_cfg[next_index].provisioned_handle_type) &&
                 (DIGI_LINE_OTN_HO_CONTAINER == digi_resource_handle->oduksw_cfg[next_index].primary_dest_handle_type))
        {
            *check_dest_ptr = TRUE;
        }
    }
    else if (LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->cpbsw_cfg[next_index].source_handle_type && 
            !(digi_resource_handle->cpbsw_cfg[next_index].provisioned_handle_type == digi_resource_handle->cpbsw_cfg[next_index].source_handle_type &&
              digi_resource_handle->cpbsw_cfg[next_index].provisioned_handle_index == digi_resource_handle->cpbsw_cfg[next_index].source_handle_index) &&
            ((DIGI_ILKN_1 == digi_resource_handle->cpbsw_cfg[next_index].source_handle_type ||
              DIGI_ILKN_2 == digi_resource_handle->cpbsw_cfg[next_index].source_handle_type ||
              DIGI_OTN_MAPPER == digi_resource_handle->cpbsw_cfg[next_index].source_handle_type)))
    {
      *check_src_ptr = TRUE;
    }
    else if (LAST_DIGI_RESOURCE_HANDLE == digi_resource_handle->cpbsw_cfg[next_index].source_handle_type && 
             !(digi_resource_handle->cpbsw_cfg[next_index].provisioned_handle_type == digi_resource_handle->cpbsw_cfg[next_index].primary_dest_handle_type &&
               digi_resource_handle->cpbsw_cfg[next_index].provisioned_handle_index == digi_resource_handle->cpbsw_cfg[next_index].primary_dest_index) &&
             (DIGI_ILKN_1 == digi_resource_handle->cpbsw_cfg[next_index].primary_dest_handle_type ||
              DIGI_ILKN_2 == digi_resource_handle->cpbsw_cfg[next_index].primary_dest_handle_type ||
              DIGI_OTN_MAPPER == digi_resource_handle->cpbsw_cfg[next_index].primary_dest_handle_type ||
              DIGI_SYS_OTN_MO_CONTAINER == digi_resource_handle->cpbsw_cfg[next_index].primary_dest_handle_type))
    {
        *check_dest_ptr = TRUE;
    }

    PMC_RETURN();
} /* digi_resource_check_src_dest */

/*******************************************************************************
* digi_resource_release_oduk_broadcast
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle          - Reference to the digi handle
*  oduk_port            - Port type of the channel handle
*  oduk_chnl            - Channel identifier.
*  switch_release       - Whether to decouple only the switch related 
*                         connections and leave the resource (TRUE)
*  mapotn_release       - Flag if TRUE will release MAPOTN resource if the
*                         channel handle is connected to a MAPOTN resource
*  oduk_connect_state_ptr - Reference to the connection array representing
*                           the ODUK switch connections.
*
* OUTPUTS:
*
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_resource_release_oduk_broadcast(digi_handle_t *digi_handle,
                                                       util_global_oduk_port_t oduk_port,
                                                       UINT32 oduk_chnl,
                                                       BOOL8 switch_release,
                                                       BOOL8 mapotn_release,
                                                       util_global_cpb_connect_t *oduk_connect_state_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    digi_chnl_hndl_conn_t source_hndl = { LAST_DIGI_HANDLE_ID, NULL, FALSE, FALSE };
    digi_chnl_hndl_conn_t dest_hndl = { LAST_DIGI_HANDLE_ID, NULL, FALSE, FALSE };
    digi_chnl_hndl_conn_t tmp_hndl = { LAST_DIGI_HANDLE_ID, NULL, FALSE, FALSE };

    PMC_ENTRY();

    if (TRUE == oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].mst.is_slave_zone_primary)
    {
        UINT32 dest_port = oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].slv.dest_mst_port;
        UINT32 dest_chnl = oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].slv.dest_mst_chnl;

        if (CPB_INDEX_LAST != oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].mst.next_dest_mst_port &&
            CPB_INDEX_LAST != oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].mst.next_dest_mst_chnl)
        {
            dest_port = oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].mst.next_dest_mst_port;
            dest_chnl = oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].mst.next_dest_mst_chnl;
        }
        result = digi_resource_release_oduk_broadcast(digi_handle,
                                                      (util_global_oduk_port_t)dest_port,
                                                      dest_chnl,
                                                      switch_release,
                                                      mapotn_release,
                                                      oduk_connect_state_ptr);
    }

    if (PMC_SUCCESS == result &&
        UTIL_GLOBAL_CHNL_OPERATIONAL == oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].mst.state)
    {
        digi_resource_convert_port_type(digi_handle, oduk_port,
                                        oduk_chnl, TRUE,
                                        TRUE,
                                        &dest_hndl);
        result = digi_oduksw_chnl_deactivate(digi_handle,
                                             dest_hndl.chnl_hndl_ptr);


        if (PMC_SUCCESS == result && DIGI_CHANNEL_HANDLE_ID_MAPPER_T == dest_hndl.chnl_hndl_type)
        {
            result = digi_resource_release_mapotn_resources(digi_handle,
                                                            dest_hndl.chnl_hndl_ptr,
                                                            mapotn_release,
                                                            TRUE);
        }
    }

    if (PMC_SUCCESS == result)
    {
        UINT32 src_port = oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].mst.source_slv_port;
        UINT32 src_chnl = oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].mst.source_slv_chnl;

        if (CPB_INDEX_LAST != src_port && CPB_INDEX_LAST != src_chnl)
        {
            if (UTIL_GLOBAL_CHNL_OPERATIONAL == oduk_connect_state_ptr->chnl[src_port][src_chnl].mst.state)
            {
                digi_resource_convert_port_type(digi_handle, src_port,
                                                src_chnl, TRUE,
                                                TRUE,
                                                &source_hndl);
                result = digi_oduksw_chnl_deactivate(digi_handle,
                                                     source_hndl.chnl_hndl_ptr);
            }
            
            if (PMC_SUCCESS == result &&
                UTIL_GLOBAL_CHNL_EQUIPPED == oduk_connect_state_ptr->chnl[src_port][src_chnl].mst.state &&
                src_port == oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].slv.dest_mst_port &&
                src_chnl == oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].slv.dest_mst_chnl)
            {
                if (NULL ==  source_hndl.chnl_hndl_ptr)
                {
                    digi_resource_convert_port_type(digi_handle, src_port,
                                                    src_chnl, TRUE,
                                                    TRUE,
                                                    &source_hndl);
                }
                result = digi_oduksw_chnl_demap(digi_handle,
                                                source_hndl.chnl_hndl_ptr);
            }
        }
        if (PMC_SUCCESS == result &&
            UTIL_GLOBAL_CHNL_EQUIPPED == oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].mst.state)
        {
            if (NULL == dest_hndl.chnl_hndl_ptr)
            {
                digi_resource_convert_port_type(digi_handle, oduk_port,
                                                oduk_chnl, TRUE,
                                                TRUE,
                                                &dest_hndl);
            }

            if (TRUE == oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].slv.is_broadcast || TRUE == oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].slv.is_multicast)
            {
                /*
                 * If is is a broadcast/multicast node then grab the src
                 * value. Later if it is MAPOTN and MAPOTN_RELEASE is true
                 * then it will be deprovisioned. Cannot deprovision the
                 * MAPOTN node when processing it if mapotn_release is 
                 * TRUE and is will bugger up the deprovisioning of the
                 * lead mode.
                 */
                digi_resource_convert_port_type(digi_handle,
                                                oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].mst.source_slv_port,
                                                oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].mst.source_slv_chnl,
                                                TRUE,
                                                TRUE,
                                                &tmp_hndl);
            }
            /*
             * demap the destination node.
             */
            result = digi_oduksw_chnl_demap(digi_handle,
                                            dest_hndl.chnl_hndl_ptr);
        }
    }

    if (PMC_SUCCESS == result && 
        DIGI_CHANNEL_HANDLE_ID_MAPPER_T == dest_hndl.chnl_hndl_type && 
        TRUE == mapotn_release &&
        FALSE == oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].mst.is_slave_zone_primary)
    {
        result = digi_otn_mapper_deprov(digi_handle, &dest_hndl.chnl_hndl_ptr);
    }
    else if (PMC_SUCCESS == result && 
             (TRUE == oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].slv.is_broadcast || TRUE == oduk_connect_state_ptr->chnl[oduk_port][oduk_chnl].slv.is_multicast) &&
             DIGI_CHANNEL_HANDLE_ID_MAPPER_T == tmp_hndl.chnl_hndl_type &&
             TRUE == mapotn_release)
      {
        result = digi_otn_mapper_deprov(digi_handle, &tmp_hndl.chnl_hndl_ptr);
      }

    PMC_RETURN(result);
}  /* digi_resource_release_oduk_broadcast */

/*******************************************************************************
* digi_resource_clean_otn_handle
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle              - Reference to the digi handle
*  digi_resource_handle_ptr - Reference to the resource table.
*  otn_ptr                  - Reference to the odu_struct_t being checked.
*  oduk_connect_data_ptr    - Reference to the connection array 
*                             representing the ODUK switch connections.
*  table_index              - Index into the resource table.
*  port_type                - ODUK connection array port type.
*
* OUTPUTS:
*
*
* RETURNS:
*  TRUE  - OTN handle is either unconfigured or is valid
*  FALSE - Data handle is not valid and should be deprovisioned. 
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_clean_otn_handle(digi_handle_t *digi_handle,
                                             digi_resource_handle_summary_t *digi_resource_handle_ptr,
                                             odu_struct_t *otn_ptr,
                                             util_global_cpb_connect_t *oduk_connect_data_ptr,
                                             UINT32 table_index,
                                             UINT32 port_type)
{
    UINT32 chnl;
    coreotn_mux_stage_t mux_stage = LAST_COREOTN_MUX_STAGE;
    BOOL8 rx_result = TRUE;
    BOOL8 tx_result = TRUE;

    PMC_ENTRY();

    /*
     * If either the OTN_PTR is null or it is not provisioned or it is
     * channelized then there is nothing to do so return.
     */
    if (!(NULL != otn_ptr && 
          (TRUE == util_global_rx_or_tx_chnl_prov_status_get(otn_ptr->switch_data.header.prov_state))))
    {
        PMC_RETURN(TRUE);
    }

    switch (otn_ptr->mem_ptr->odu_level)
    {
      case ODU_STRUCT_LEVEL_HO_ODU:
        mux_stage = COREOTN_MUX_STAGE_NONE;

        if (UTIL_GLOBAL_ODU_UNCHANNELIZED != otn_ptr->mem_ptr->payload_format)
        {
            mux_stage = COREOTN_MUX_STAGE_ONE;
        }
        rx_result = coreotn_ho_chnl_prov_get(digi_handle->coreotn_handle,
                                             otn_ptr->mem_ptr->chnl_id,
                                             TRUE,
                                             mux_stage,
                                             otn_ptr->mem_ptr->payload_format,
                                             UTIL_GLOBAL_CHNL_OPERATIONAL);
        tx_result = coreotn_ho_chnl_prov_get(digi_handle->coreotn_handle,
                                             otn_ptr->mem_ptr->chnl_id,
                                             FALSE,
                                             mux_stage,
                                             otn_ptr->mem_ptr->payload_format,
                                             UTIL_GLOBAL_CHNL_OPERATIONAL);

        break;

      case ODU_STRUCT_LEVEL_MO_ODU:
      case ODU_STRUCT_LEVEL_LO_ODU:
        /*
         * SYSOTN uses a separate api to determine connectivity.  In this
         * case check that we are a lo order channel and if sysotn then
         * call that API. Since there is only one call set the tx to be
         * the same as the rx.
         */
        if (ODU_STRUCT_LEVEL_LO_ODU == otn_ptr->mem_ptr->odu_level &&
            digi_handle->var.prod_app == DIGI_PROD_APP_SYSOTN_CARD)
        {
            rx_result = coreotn_sysapp_line_chnl_prov_get(digi_handle->coreotn_handle,
                                                          otn_ptr->mem_ptr->chnl_id,
                                                          UTIL_GLOBAL_CHNL_OPERATIONAL);
            tx_result = rx_result;
            break;

        }
        mux_stage = COREOTN_MUX_STAGE_TWO;

        if (ODU_STRUCT_LEVEL_MO_ODU == otn_ptr->mem_ptr->odu_level)
        { 
            /*
             * Ensure the muxing stage is set to ONE for the unchannelized
             * and NULL cases otherwise coreotn_lo_chnl_prov_get will fail
             */
            mux_stage = COREOTN_MUX_STAGE_ONE;
        }

        rx_result = coreotn_lo_chnl_prov_get(digi_handle->coreotn_handle,
                                             otn_ptr->mem_ptr->chnl_id,
                                             TRUE,
                                             mux_stage,
                                             otn_ptr->mem_ptr->payload_format,
                                             otn_ptr->mem_ptr->tx_trib_loc_mask,
                                             otn_ptr->mem_ptr->rx_trib_loc_mask,
                                             UTIL_GLOBAL_CHNL_OPERATIONAL);

        tx_result = coreotn_lo_chnl_prov_get(digi_handle->coreotn_handle,
                                             otn_ptr->mem_ptr->chnl_id,
                                             FALSE,
                                             mux_stage,
                                             otn_ptr->mem_ptr->payload_format,
                                             otn_ptr->mem_ptr->tx_trib_loc_mask,
                                             otn_ptr->mem_ptr->rx_trib_loc_mask,
                                             UTIL_GLOBAL_CHNL_OPERATIONAL);

        break;

      default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }

    if (FALSE == tx_result && FALSE == rx_result)
    {
        PMC_RETURN(FALSE);
    }

    if (UTIL_GLOBAL_ODU_UNCHANNELIZED != otn_ptr->mem_ptr->payload_format)
    {
        /*
         * Before leaving check that the number of subcontainers in the
         * otn_ptr match what is actually provisioned. If not update both
         * the subcontainre values and the sub_chnl_ids_rx/tx and the
         * trib_port entries.
         */
        digi_resource_clean_otn_subcontainers(digi_handle,
                                              otn_ptr);
        PMC_RETURN(TRUE);
    }
    chnl = otn_ptr->switch_data.oduk_data.channel;

    if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == otn_ptr->switch_data.oduk_data.op_state.active_state && 
        UTIL_GLOBAL_CHNL_OPERATIONAL != oduk_connect_data_ptr->chnl[port_type][chnl].mst.state)
    {
        (void)digi_resource_oduksw_chnl_deactivate(digi_handle,
                                                   otn_ptr);
    }

    if (TRUE == otn_ptr->switch_data.oduk_data.op_state.map_state.is_output_mapped && UTIL_GLOBAL_CHNL_START == oduk_connect_data_ptr->chnl[port_type][chnl].mst.state)
    {
        (void)digi_resource_oduksw_chnl_demap(digi_handle,
                                              digi_resource_handle_ptr,
                                              table_index,
                                              otn_ptr);
    }
    if (TRUE == otn_ptr->switch_data.oduk_data.op_state.map_state.is_input_mapped && 
        UTIL_GLOBAL_CHNL_START == oduk_connect_data_ptr->chnl[port_type][chnl].slv.state)
    {
        otn_ptr->switch_data.oduk_data.op_state.map_state.is_input_mapped = FALSE;
        otn_ptr->switch_data.oduk_data.op_state.map_state.input_map_count = 0;
    }

    PMC_RETURN(TRUE);
} /* digi_resource_clean_otn_handle */

/*******************************************************************************
* digi_resource_mapotn_release
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*  digi_handle              - Reference to the digi handle
*  digi_resource_handle_ptr - Reference to the resource table.
*  cpb_connect_data_ptr     - Reference to the CPB switch connection array
*  chnl_ptr                 - Reference to the MAPOTN channel handle being
*                             released if applicable.
*  map_port                 - Value representing the MAPOTN port type. It
*                             is either 3 or 4 (ENET_LINE) if ETrans.
*  map_chnl                 - Index of the mapotn channel in the CPB
*                             connect array
*
* OUTPUTS:
*
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_mapotn_release(digi_handle_t *digi_handle,
                                          digi_resource_handle_summary_t *digi_resource_handle_ptr,
                                          util_global_cpb_connect_t *cpb_connect_data_ptr,
                                          util_global_switch_data_def_t *chnl_ptr,
                                          UINT32 map_port,
                                          UINT32 map_chnl)
{
    BOOL8 clean_cpb = FALSE;

    PMC_ENTRY();
    
    /*
     * If the data handle is unused based on the state and input/output
     * mapping then it can be deprovisioned.
     */
    if (FALSE == chnl_ptr->cpb_data.op_state.map_state.is_input_mapped &&
        FALSE == chnl_ptr->cpb_data.op_state.map_state.is_output_mapped &&
        UTIL_GLOBAL_CHNL_ACTIVE_STATE_INACTIVE == chnl_ptr->cpb_data.op_state.active_state &&
        FALSE == chnl_ptr->oduk_data.op_state.map_state.is_input_mapped &&
        FALSE == chnl_ptr->oduk_data.op_state.map_state.is_output_mapped &&
        UTIL_GLOBAL_CHNL_ACTIVE_STATE_INACTIVE == chnl_ptr->oduk_data.op_state.active_state)
    {
        digi_resource_otn_mapper_deprov(digi_handle, 
                                        (digi_mapper_chnl_t **)&chnl_ptr);
    }
    else if (TRUE == digi_resource_oduk_still_connected(digi_handle,
                                                        cpb_connect_data_ptr,
                                                        chnl_ptr,
                                                        map_port,
                                                        map_chnl,
                                                        &clean_cpb))
    {
        /*
         * If here there was likely something left behind on
         * the oduk side.
         */
        if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == chnl_ptr->oduk_data.op_state.active_state)
        {
            digi_resource_oduksw_chnl_deactivate(digi_handle,
                                                 chnl_ptr);
        }

        if (TRUE == chnl_ptr->oduk_data.op_state.map_state.is_output_mapped)
        {
            digi_resource_oduksw_chnl_demap(digi_handle,
                                            digi_resource_handle_ptr,
                                            DIGI_ODUK_SWITCH_TOTAL_CHANNELS,
                                            chnl_ptr);  
        }
        
        if (TRUE == chnl_ptr->oduk_data.op_state.map_state.is_input_mapped)
        {
            /*
             * Something is transmitting to this MAPOTN channel
             * Use the ODUK connect array to find what it is. Note that the
             * value of 3 (MAPOTN ODUK Port) is used and not the map_port
             * which may be ENET_LINE if ETrans case.
             */
            util_global_cpb_connect_t *oduk_connect_data_ptr = NULL;
            
            coreotn_oduksw_connect_data_state_get(digi_handle->coreotn_handle,
                                                  &oduk_connect_data_ptr);
            if (UTIL_GLOBAL_CHNL_START != oduk_connect_data_ptr->chnl[3][map_chnl].slv.state)
            {
                UINT32 dest_port = oduk_connect_data_ptr->chnl[3][map_chnl].slv.dest_mst_port;
                UINT32 dest_chnl =  oduk_connect_data_ptr->chnl[3][map_chnl].slv.dest_mst_chnl;
                digi_resource_oduk_free_handle(digi_handle,
                                               digi_resource_handle_ptr,
                                               oduk_connect_data_ptr,
                                               dest_port,
                                               dest_chnl);
            }
        }
        chnl_ptr->oduk_data.op_state.map_state.input_map_count = 0;
        chnl_ptr->oduk_data.op_state.map_state.is_input_mapped = FALSE;
        
        if (TRUE == clean_cpb)
        {
            UINT32 dest_port = cpb_connect_data_ptr->chnl[map_port][map_chnl].slv.dest_mst_port;
            UINT32 dest_chnl = cpb_connect_data_ptr->chnl[map_port][map_chnl].slv.dest_mst_chnl;
            digi_resource_cpb_free_handle(digi_handle,
                                          digi_resource_handle_ptr,
                                          cpb_connect_data_ptr,
                                          dest_port,
                                          dest_chnl);
        }

        if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == chnl_ptr->cpb_data.op_state.active_state)
        {
            digi_resource_cpb_chnl_deactivate(digi_handle,
                                              digi_resource_handle_ptr,
                                              DIGI_CPB_SWITCH_TOTAL_CHANNELS,
                                              chnl_ptr);
        }

        if (TRUE == chnl_ptr->cpb_data.op_state.map_state.is_output_mapped)
        {
            digi_resource_cpb_chnl_demap(digi_handle,
                                         digi_resource_handle_ptr,
                                         DIGI_CPB_SWITCH_TOTAL_CHANNELS,
                                         chnl_ptr);  
        }
        

        digi_resource_otn_mapper_deprov(digi_handle, 
                                        (digi_mapper_chnl_t **)&chnl_ptr);
    }
    PMC_RETURN();
} /*digi_resource_mapotn_release */


/*******************************************************************************
* digi_resource_oduk_free_handle
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Determines the resource that is mapped to the MAPOTN channel over the
*  ODUK switch and deactivates/demaps it.
*
* INPUTS:
*  digi_handle              - Reference to the digi handle
*  digi_resource_handle_ptr - Reference to the resource table.
*  oduk_connect_data_ptr    - Reference to the ODUK switch connection array
*  dest_port                - Port type to be deactivated/demapped
*  dest_chnl                - Channel
*
* OUTPUTS:
*
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_oduk_free_handle(digi_handle_t *digi_handle,
                                            digi_resource_handle_summary_t *digi_resource_handle_ptr,
                                            util_global_cpb_connect_t *oduk_connect_data_ptr,
                                            UINT32 dest_port,
                                            UINT32 dest_chnl)
{
    util_global_oduk_port_t port_type = (util_global_oduk_port_t)dest_port;
    util_global_switch_data_def_t *switch_ptr = NULL;
    PMC_ENTRY();

    /*
     * Make sure there is valid data before proceeding. It is not an 
     * error if there is. Need to ensure that not trying to process
     * something that is.
     */
    if (CPB_INDEX_LAST == dest_port || CPB_INDEX_LAST == dest_chnl)
    {
        PMC_RETURN();
    }

    switch (port_type)
    {
      case UTIL_GLOBAL_ODUK_PORT_ILKN1:
        switch_ptr = (util_global_switch_data_def_t *)&(digi_handle->handle_pool.ilkn_1_chnl[dest_chnl+DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX]);
        break;

      case UTIL_GLOBAL_ODUK_PORT_ILKN2:
        switch_ptr = (util_global_switch_data_def_t *)&(digi_handle->handle_pool.ilkn_2_chnl[dest_chnl+DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX]);
        break;

      case UTIL_GLOBAL_ODUK_PORT_COREOTN:
        switch_ptr = digi_resource_find_otn_handle(digi_handle,
                                                   dest_chnl);
        break;

      case UTIL_GLOBAL_ODUK_PORT_MAPOTN:
        switch_ptr = (util_global_switch_data_def_t *)&(digi_handle->handle_pool.mapper_chnl[dest_chnl]);
        break;

      default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }

    if (NULL == switch_ptr)
    {
        PMC_ASSERT(FALSE, DIGI_ERR_NULL_HANDLE, 0, 0);
    }

    if (UTIL_GLOBAL_CHNL_OPERATIONAL == 
        oduk_connect_data_ptr->chnl[dest_port][dest_chnl].mst.state ||
        UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == switch_ptr->oduk_data.op_state.active_state)
    {
        digi_resource_oduksw_chnl_deactivate(digi_handle,
                                             switch_ptr);

        if (UTIL_GLOBAL_ODUK_PORT_MAPOTN == port_type &&
            UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == switch_ptr->cpb_data.op_state.active_state)
        {
            digi_resource_cpb_chnl_deactivate(digi_handle,
                                              digi_resource_handle_ptr,
                                              DIGI_CPB_SWITCH_TOTAL_CHANNELS,
                                              switch_ptr);
        } 
    }

    if (UTIL_GLOBAL_CHNL_START != 
        oduk_connect_data_ptr->chnl[dest_port][dest_chnl].mst.state ||
        TRUE == switch_ptr->oduk_data.op_state.map_state.is_output_mapped)
    {
        digi_resource_oduksw_chnl_demap(digi_handle,
                                        digi_resource_handle_ptr,
                                        DIGI_ODUK_SWITCH_TOTAL_CHANNELS,
                                        switch_ptr);

        if (UTIL_GLOBAL_ODUK_PORT_MAPOTN == port_type &&
            UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == switch_ptr->cpb_data.op_state.map_state.is_output_mapped)
        {
            digi_resource_cpb_chnl_demap(digi_handle,
                                         digi_resource_handle_ptr,
                                         DIGI_CPB_SWITCH_TOTAL_CHANNELS,
                                         switch_ptr);
        }   
    }

    PMC_RETURN();
} /*  digi_resource_oduk_free_handle */


/*******************************************************************************
* digi_resource_cpb_free_handle
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Determines the resource that is mapped to the MAPOTN channel over the
*  ODUK switch and deactivates/demaps it.
*
* INPUTS:
*  digi_handle              - Reference to the digi handle
*  digi_resource_handle_ptr - Reference to the resource table.
*  cpb_connect_data_ptr     - Reference to the ODUK switch connection array
*  dest_port                - Port type to be deactivated/demapped
*  dest_chnl                - Channel
*
* OUTPUTS:
*
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_cpb_free_handle(digi_handle_t *digi_handle,
                                            digi_resource_handle_summary_t *digi_resource_handle_ptr,
                                            util_global_cpb_connect_t *cpb_connect_data_ptr,
                                            UINT32 dest_port,
                                            UINT32 dest_chnl)
{
    util_global_cpb_port_t port_type = (util_global_cpb_port_t)dest_port;
    util_global_switch_data_def_t *switch_ptr = NULL;
    PMC_ENTRY();

    /*
     * Make sure there is valid data before proceeding. It is not an 
     * error if there is. Need to ensure that not trying to process
     * something that is.
     */
    if (CPB_INDEX_LAST == dest_port || CPB_INDEX_LAST == dest_chnl)
    {
        PMC_RETURN();
    }

    switch (port_type)
    {
      case UTIL_GLOBAL_CPB_PORT_ILKN1:
        switch_ptr = (util_global_switch_data_def_t *)&(digi_handle->handle_pool.ilkn_1_chnl[dest_chnl]);
        break;

      case UTIL_GLOBAL_CPB_PORT_ILKN2:
        switch_ptr = (util_global_switch_data_def_t *)&(digi_handle->handle_pool.ilkn_2_chnl[dest_chnl]);
        break;

      case UTIL_GLOBAL_CPB_PORT_CBRC:
        switch_ptr = (util_global_switch_data_def_t *)&(digi_handle->handle_pool.cbr_chnl[dest_chnl]);
        break;

      case UTIL_GLOBAL_CPB_PORT_MAPOTN:
        switch_ptr = (util_global_switch_data_def_t *)&(digi_handle->handle_pool.mapper_chnl[dest_chnl]);
        break;

      case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
        switch_ptr = (util_global_switch_data_def_t *)&(digi_handle->handle_pool.enet_line_chnl[dest_chnl]);
        break;

      case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
         switch_ptr = (util_global_switch_data_def_t *)&(digi_handle->handle_pool.enet_sys_chnl[dest_chnl]);
        break;

      default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }

    if (NULL == switch_ptr)
    {
        PMC_ASSERT(FALSE, DIGI_ERR_NULL_HANDLE, 0, 0);
    }

    if (UTIL_GLOBAL_CHNL_OPERATIONAL == 
        cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.state ||
        UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == switch_ptr->cpb_data.op_state.active_state)
    {
        digi_resource_cpb_chnl_deactivate(digi_handle,
                                          digi_resource_handle_ptr,
                                          DIGI_CPB_SWITCH_TOTAL_CHANNELS,
                                          switch_ptr);

        if (UTIL_GLOBAL_CPB_PORT_MAPOTN == port_type &&
            UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == switch_ptr->oduk_data.op_state.active_state)
        {
            digi_resource_oduksw_chnl_deactivate(digi_handle,
                                                 switch_ptr);
        } 
    }

    if (UTIL_GLOBAL_CHNL_START != 
        cpb_connect_data_ptr->chnl[dest_port][dest_chnl].mst.state ||
        TRUE == switch_ptr->cpb_data.op_state.map_state.is_output_mapped)
    {
        digi_resource_cpb_chnl_demap(digi_handle,
                                     digi_resource_handle_ptr,
                                     DIGI_CPB_SWITCH_TOTAL_CHANNELS,
                                     switch_ptr);  

        if (UTIL_GLOBAL_CPB_PORT_MAPOTN == port_type &&
            TRUE == switch_ptr->oduk_data.op_state.map_state.is_output_mapped)
        {
            digi_resource_oduksw_chnl_demap(digi_handle,
                                            digi_resource_handle_ptr,
                                            DIGI_ODUK_SWITCH_TOTAL_CHANNELS,
                                            switch_ptr);
        }
    }

    PMC_RETURN();
} /* digi_resource_cpb_free_handle */


/*******************************************************************************
* digi_resource_find_otn_handle
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Determines the resource that is mapped to the MAPOTN channel over the
*  ODUK switch and deactivates/demaps it.
*
* INPUTS:
*  digi_handle              - Reference to the digi handle
*  otn_chnl                - Channel
*
* OUTPUTS:
*
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PRIVATE util_global_switch_data_def_t *digi_resource_find_otn_handle(digi_handle_t *digi_handle,
                                                                     UINT32 otn_chnl)
{
    util_global_switch_data_def_t *chnl_hndl_ptr = NULL;

    PMC_ENTRY();

    if (otn_chnl < DIGI_OTN_SERVER_HO_CHNL_MAX &&
        (TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.ho_odu_struct[otn_chnl].switch_data.header.prov_state)) &&
        UTIL_GLOBAL_ODU_UNCHANNELIZED == digi_handle->handle_pool.ho_odu_struct[otn_chnl].mem_ptr->payload_format)
    {
        chnl_hndl_ptr = (util_global_switch_data_def_t *)&(digi_handle->handle_pool.ho_odu_struct[otn_chnl]);

    }
    else if (otn_chnl < DIGI_OTN_SERVER_MO_CHNL_MAX &&
             (TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.mo_odu_struct[otn_chnl].switch_data.header.prov_state)) &&
             UTIL_GLOBAL_ODU_UNCHANNELIZED == digi_handle->handle_pool.mo_odu_struct[otn_chnl].mem_ptr->payload_format)
    {
        chnl_hndl_ptr = (util_global_switch_data_def_t *)&(digi_handle->handle_pool.mo_odu_struct[otn_chnl]);

    }
    else if (otn_chnl < DIGI_OTN_SERVER_MO_CHNL_MAX &&
             (TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.lo_odu_struct[otn_chnl].switch_data.header.prov_state)) &&
             UTIL_GLOBAL_ODU_UNCHANNELIZED == digi_handle->handle_pool.lo_odu_struct[otn_chnl].mem_ptr->payload_format)
    {
        chnl_hndl_ptr = (util_global_switch_data_def_t *)&(digi_handle->handle_pool.lo_odu_struct[otn_chnl]);

    }
    else
    {
        PMC_ASSERT(FALSE, DIGI_ERR_NULL_HANDLE, 0, 0);
    }

    PMC_RETURN(chnl_hndl_ptr);
}


/*******************************************************************************
* digi_resource_deprovision_otn
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*
* INPUTS:
*  digi_handle              - Reference to the digi handle
*  digi_resource_handle_ptr - Reference to the resource table.
*  oduk_connect_data_ptr    - Reference to the ODUK connect array
*  otn_ptr                  - Reference to the OTN handle to be 
*                             deprovisioned
*  odu_level                - ODU level being released.
*  port_type                - Port type used in the connect array. COREOTN
*                             or ILKN if SYSOTN and HO.
*
* OUTPUTS:
*
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_deprovision_otn(digi_handle_t *digi_handle,
                                           digi_resource_handle_summary_t *digi_resource_handle_ptr,
                                           util_global_cpb_connect_t *oduk_connect_data_ptr,
                                           odu_struct_t *otn_ptr,
                                           util_global_odu_level_t odu_level,
                                           UINT32 port_type)
{
    UINT32 chnl = otn_ptr->switch_data.oduk_data.channel;
    UINT32 i = 0;

    PMC_ENTRY();

    if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == otn_ptr->switch_data.oduk_data.op_state.active_state)
    {
        digi_resource_oduksw_chnl_deactivate(digi_handle,
                                             otn_ptr);
    }

    if (TRUE == otn_ptr->switch_data.oduk_data.op_state.map_state.is_input_mapped)
    {
        digi_resource_oduksw_chnl_demap(digi_handle,
                                        digi_resource_handle_ptr,
                                        DIGI_ODUK_SWITCH_TOTAL_CHANNELS,
                                        otn_ptr);
    }
    /*
     * If this coreotn component is unchannelized then ensure whatever it
     * is connected to needs to be demapped and deactivated.
     */
    if (UTIL_GLOBAL_ODU_UNCHANNELIZED == otn_ptr->mem_ptr->payload_format &&
        UTIL_GLOBAL_CHNL_START != oduk_connect_data_ptr->chnl[port_type][chnl].slv.state &&
        CPB_INDEX_LAST != oduk_connect_data_ptr->chnl[port_type][chnl].slv.dest_mst_port)
    {
        digi_resource_oduk_free_handle(digi_handle,
                                       digi_resource_handle_ptr,
                                       oduk_connect_data_ptr,
                                       oduk_connect_data_ptr->chnl[port_type][chnl].slv.dest_mst_port,
                                       oduk_connect_data_ptr->chnl[port_type][chnl].slv.dest_mst_chnl);
    }

    if (UTIL_GLOBAL_LEVEL_HO_ODU == odu_level)
    {
        digi_otn_server_chnl_t *otn_server_chnl_ptr = NULL;

        if (DIGI_PROD_APP_SYSOTN_CARD == digi_handle->var.prod_app)
        {
            otn_server_chnl_ptr = (digi_otn_server_chnl_t *)&digi_handle->handle_pool.sysotn_server_chnl;
        }
        else
        {
            for (i = 0; i < DIGI_OTN_SERVER_CHNL_MAX; i++)
            {
                if (chnl == digi_handle->handle_pool.otn_server_chnl[i].dci_chnl)
                {
                    otn_server_chnl_ptr = (digi_otn_server_chnl_t *)&digi_handle->handle_pool.otn_server_chnl[i];
                    break;
                }
            }
        }
        PMC_ASSERT(NULL != otn_server_chnl_ptr, DIGI_ERR_NULL_HANDLE,
                   0, 0);
        
        digi_resource_otn_server_ho_deprov(digi_handle,
                                           (digi_otn_odu_chnl_t **)&otn_ptr,
                                           &otn_server_chnl_ptr);
    }
    else if (UTIL_GLOBAL_LEVEL_MO_ODU == odu_level ||
             UTIL_GLOBAL_LEVEL_LO_ODU == odu_level)
    {
        digi_otn_odu_chnl_t *otn_chnl_hndl_ptr = NULL;

        BOOL8 found = digi_resource_find_parent_handle(digi_handle,
                                                       digi_resource_handle_ptr,
                                                       odu_level,
                                                       chnl,
                                                       &otn_chnl_hndl_ptr);

        if (TRUE == found)
        {
            digi_resource_otn_server_lo_deprov(digi_handle,
                                               digi_resource_handle_ptr,
                                               DIGI_ODUK_SWITCH_TOTAL_CHANNELS,
                                               (digi_otn_odu_chnl_t **)&otn_ptr,
                                               &otn_chnl_hndl_ptr);
        }
        else
        {
            PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                    "No PARENT found for LO ORDER DEPROV Channel=%d\n", chnl);
        }
    }

    PMC_RETURN();
} /* digi_resource_deprovision_otn */


/*******************************************************************************
* digi_resource_oduk_still_connected
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Verifies that the MAPOTN is connected in at least one direction on both
*  switches.
*
* INPUTS:
*  digi_handle              - Reference to the digi handle
*  cpb_connect_data_ptr     - Reference to the CPB switch connection array
*  chnl_ptr                 - Reference to the MAPOTN channel handle being
*                             released if applicable.
*  map_port                 - Value representing the MAPOTN port type. It
*                             is either 3 or 4 (ENET_LINE) if ETrans.
*  map_chnl                 - Index of the mapotn channel in the CPB
*                             connect array
*
* OUTPUTS:
*  is_cpb_clean_ptr         - Whether it may be that the CPB still has 
*                             resources connected.
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_oduk_still_connected(digi_handle_t *digi_handle,
                                                 util_global_cpb_connect_t *cpb_connect_data_ptr,
                                                 util_global_switch_data_def_t *chnl_ptr,
                                                 UINT32 map_port,
                                                 UINT32 map_chnl,
                                                 BOOL8 *is_cpb_clean_ptr)
{
    BOOL8 result = FALSE;

    util_global_cpb_connect_t *oduk_connect_data_ptr = NULL;
    UINT32 oduk_chnl = chnl_ptr->oduk_data.channel;

    PMC_ENTRY();

    coreotn_oduksw_connect_data_state_get(digi_handle->coreotn_handle,
                                          &oduk_connect_data_ptr);


    if ((UTIL_GLOBAL_CHNL_START == cpb_connect_data_ptr->chnl[map_port][map_chnl].slv.state &&
         UTIL_GLOBAL_CHNL_START == cpb_connect_data_ptr->chnl[map_port][map_chnl].mst.state) ||
        (UTIL_GLOBAL_CHNL_OPERATIONAL == cpb_connect_data_ptr->chnl[map_port][map_chnl].slv.state &&
         UTIL_GLOBAL_CHNL_OPERATIONAL == cpb_connect_data_ptr->chnl[map_port][map_chnl].mst.state &&  
         (UTIL_GLOBAL_CHNL_ACTIVE_STATE_INACTIVE == chnl_ptr->oduk_data.op_state.active_state ||
          FALSE == chnl_ptr->oduk_data.op_state.map_state.is_output_mapped)))
    {
        result = TRUE;
    }
    else if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_INACTIVE == chnl_ptr->cpb_data.op_state.active_state &&
             FALSE == chnl_ptr->cpb_data.op_state.map_state.is_output_mapped &&
             FALSE == chnl_ptr->cpb_data.op_state.map_state.is_input_mapped)
    {
        /*
         * Check the state at the CPB channel handle to determine if there
         * is anything to clean. Used in multicast/broadcast cases.
         */
        result = TRUE;

        if (UTIL_GLOBAL_CHNL_START != cpb_connect_data_ptr->chnl[map_port][map_chnl].slv.state ||
            UTIL_GLOBAL_CHNL_START != cpb_connect_data_ptr->chnl[map_port][map_chnl].mst.state)
        {
            *is_cpb_clean_ptr = TRUE;
        }
    }
    else if ((UTIL_GLOBAL_CHNL_ACTIVE_STATE_INACTIVE == chnl_ptr->oduk_data.op_state.active_state &&
              FALSE == chnl_ptr->oduk_data.op_state.map_state.is_output_mapped &&
              FALSE == chnl_ptr->oduk_data.op_state.map_state.is_input_mapped) ||
             (UTIL_GLOBAL_CHNL_OPERATIONAL != oduk_connect_data_ptr->chnl[3][oduk_chnl].slv.state &&
              UTIL_GLOBAL_CHNL_OPERATIONAL != oduk_connect_data_ptr->chnl[3][oduk_chnl].mst.state))
    {
        /*
         * Check the state at ODUK switch in the channel handle and connect
         * array. If not connected then need to clean.
         */
        result = TRUE;

        if (UTIL_GLOBAL_CHNL_START != cpb_connect_data_ptr->chnl[map_port][map_chnl].slv.state ||
            UTIL_GLOBAL_CHNL_START != cpb_connect_data_ptr->chnl[map_port][map_chnl].mst.state)
        {
            *is_cpb_clean_ptr = TRUE;
        }
    }

    PMC_RETURN(result);
} /* digi_resource_oduk_still_connected */


/*******************************************************************************
* digi_resource_clean_otn_subcontainers
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Checks that the number of tx and rx subcontainers in the otn container
*  matches what is actually configured in the digi_handle
*
* INPUTS:
*  digi_handle              - Reference to the digi handle
*  otn_ptr                  - HO or MO otn container.      
*
* OUTPUTS:
*  None
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_clean_otn_subcontainers(digi_handle_t *digi_handle,
                                                   odu_struct_t *otn_ptr)
{
    PMC_ENTRY();

    if (otn_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_tx > 0)
    {
        UINT8 num_subcontainers = 0;
        UINT8 i = 0;

        for (i = 0; i < DIGI_NUM_CAL_ENTRIES_IN_100G; i++)
        {
            if (DIGI_DCI_CHANNEL_UNASSIGNED != otn_ptr->mem_ptr->trib_slot_ctxt.sub_chnl_ids_tx[i])
            {
                UINT8 chnl_id = otn_ptr->mem_ptr->trib_slot_ctxt.sub_chnl_ids_tx[i];
                
                if ((ODU_STRUCT_LEVEL_HO_ODU == otn_ptr->mem_ptr->odu_level &&
                     UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED != digi_handle->handle_pool.mo_odu_struct[chnl_id].switch_data.header.prov_state) ||
                    (ODU_STRUCT_LEVEL_MO_ODU == otn_ptr->mem_ptr->odu_level &&
                     UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED != digi_handle->handle_pool.lo_odu_struct[chnl_id].switch_data.header.prov_state))
                {
                    otn_ptr->mem_ptr->trib_slot_ctxt.sub_chnl_ids_tx[i] = DIGI_DCI_CHANNEL_UNASSIGNED;
                    otn_ptr->mem_ptr->trib_slot_ctxt.tx_trib_port[i] = DIGI_DCI_CHANNEL_UNASSIGNED;
                    PMC_ASSERT(otn_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_tx > 0, DIGI_ERR_INVALID_ARG, 0, 0);
                    otn_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_tx--;
                }
                else if (ODU_STRUCT_LEVEL_HO_ODU == otn_ptr->mem_ptr->odu_level &&
                         UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED == digi_handle->handle_pool.mo_odu_struct[chnl_id].switch_data.header.prov_state &&
                         UTIL_GLOBAL_CHNL_START ==
                         coreotn_odtu_chnl_state_get(digi_handle->coreotn_handle,
                                                     COREOTN_CHNL_ORDER_HO,
                                                     chnl_id,
                                                     FALSE))
                {
                    otn_ptr->mem_ptr->trib_slot_ctxt.sub_chnl_ids_tx[i] = DIGI_DCI_CHANNEL_UNASSIGNED;
                    otn_ptr->mem_ptr->trib_slot_ctxt.tx_trib_port[i] = DIGI_DCI_CHANNEL_UNASSIGNED;
                    PMC_ASSERT(otn_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_tx > 0, DIGI_ERR_INVALID_ARG, 0, 0);
                    otn_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_tx--;
                }
                else if (ODU_STRUCT_LEVEL_MO_ODU == otn_ptr->mem_ptr->odu_level &&
                          UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED == digi_handle->handle_pool.lo_odu_struct[chnl_id].switch_data.header.prov_state &&
                         UTIL_GLOBAL_CHNL_START ==
                         coreotn_odtu_chnl_state_get(digi_handle->coreotn_handle,
                                                     COREOTN_CHNL_ORDER_MO,
                                                     chnl_id,
                                                     FALSE))
                  {
                    otn_ptr->mem_ptr->trib_slot_ctxt.sub_chnl_ids_tx[i] = DIGI_DCI_CHANNEL_UNASSIGNED;
                    otn_ptr->mem_ptr->trib_slot_ctxt.tx_trib_port[i] = DIGI_DCI_CHANNEL_UNASSIGNED;
                    PMC_ASSERT(otn_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_tx > 0, DIGI_ERR_INVALID_ARG, 0, 0);
                    otn_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_tx--;
                  }
                else
                {
                    num_subcontainers++;
                }
            }
        }
        PMC_ASSERT(num_subcontainers == otn_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_tx,
                   DIGI_ERR_INVALID_ARG, 0, 0);
        
    }
    if (otn_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_rx > 0)
    {
        UINT8 num_subcontainers = 0;
        UINT8 i = 0;

        for (i = 0; i < DIGI_NUM_CAL_ENTRIES_IN_100G; i++)
        {
            if (DIGI_DCI_CHANNEL_UNASSIGNED != otn_ptr->mem_ptr->trib_slot_ctxt.sub_chnl_ids_rx[i])
            {
                UINT8 chnl_id = otn_ptr->mem_ptr->trib_slot_ctxt.sub_chnl_ids_rx[i];
                
                if ((ODU_STRUCT_LEVEL_HO_ODU == otn_ptr->mem_ptr->odu_level &&
                     UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED != digi_handle->handle_pool.mo_odu_struct[chnl_id].switch_data.header.prov_state) ||
                    (ODU_STRUCT_LEVEL_MO_ODU == otn_ptr->mem_ptr->odu_level &&
                     UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED != digi_handle->handle_pool.lo_odu_struct[chnl_id].switch_data.header.prov_state))                        
                {
                    otn_ptr->mem_ptr->trib_slot_ctxt.sub_chnl_ids_rx[i] = DIGI_DCI_CHANNEL_UNASSIGNED;
                    otn_ptr->mem_ptr->trib_slot_ctxt.rx_trib_port[i] = DIGI_DCI_CHANNEL_UNASSIGNED;
                    PMC_ASSERT(otn_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_rx > 0, DIGI_ERR_INVALID_ARG, 0, 0);
                    otn_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_rx--;
                }
                else if (ODU_STRUCT_LEVEL_HO_ODU == otn_ptr->mem_ptr->odu_level &&
                          UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED == digi_handle->handle_pool.mo_odu_struct[chnl_id].switch_data.header.prov_state &&
                         UTIL_GLOBAL_CHNL_START ==
                         coreotn_odtu_chnl_state_get(digi_handle->coreotn_handle,
                                                     COREOTN_CHNL_ORDER_HO,
                                                     chnl_id,
                                                     TRUE))
                {
                    otn_ptr->mem_ptr->trib_slot_ctxt.sub_chnl_ids_rx[i] = DIGI_DCI_CHANNEL_UNASSIGNED;
                    otn_ptr->mem_ptr->trib_slot_ctxt.rx_trib_port[i] = DIGI_DCI_CHANNEL_UNASSIGNED;
                    PMC_ASSERT(otn_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_rx > 0, DIGI_ERR_INVALID_ARG, 0, 0);
                    otn_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_rx--;
                }
                else if (ODU_STRUCT_LEVEL_MO_ODU == otn_ptr->mem_ptr->odu_level &&
                          UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED == digi_handle->handle_pool.lo_odu_struct[chnl_id].switch_data.header.prov_state &&
                         UTIL_GLOBAL_CHNL_START ==
                         coreotn_odtu_chnl_state_get(digi_handle->coreotn_handle,
                                                     COREOTN_CHNL_ORDER_MO,
                                                     chnl_id,
                                                     TRUE))
                {
                    otn_ptr->mem_ptr->trib_slot_ctxt.sub_chnl_ids_rx[i] = DIGI_DCI_CHANNEL_UNASSIGNED;
                    otn_ptr->mem_ptr->trib_slot_ctxt.rx_trib_port[i] = DIGI_DCI_CHANNEL_UNASSIGNED;
                    PMC_ASSERT(otn_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_rx > 0, DIGI_ERR_INVALID_ARG, 0, 0);
                    otn_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_rx--;
                }
                else
                {
                    num_subcontainers++;
                }
            }
        }
        PMC_ASSERT(num_subcontainers == otn_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_rx,
                   DIGI_ERR_INVALID_ARG, 0, 0);
    }
    
    PMC_RETURN();
}


/*******************************************************************************
* digi_resource_release_otn
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Determines whether the particular OTN container needs to be released
*  and release it if it does 
*
* INPUTS:
*  digi_handle              - Reference to the digi handle
*  digi_resource_handle     - Reference to the resource handle
*  otn_container_ptr        - Reference to the OTN container
*  direction                - Direction being checked.
*  is_oduk_to_cpb           - Whether going from ODUK to CPB(TRUE) or from
*                             CPB to ODUK
*  otn_handle               - Handle of the caller
*  parent_handle_ptr        - Reference to the parent subcontainer of the
*                             otn_container_ptr needed if deprovisioning 
*                             the resource.
*
* OUTPUTS:
*  None
*
* RETURNS:
*  PMC_SUCCESS              - Operation was successful
*  PMC_ERROR                - An error occurred
*
* NOTES:
*
******************************************************************************/
PRIVATE PMC_ERROR digi_resource_release_otn(digi_handle_t *digi_handle,
                                            digi_resource_handle_summary_t *digi_resource_handle,
                                            digi_otn_container_cfg_t *otn_container_ptr,
                                            digi_direction_check_t direction,
                                            BOOL8 is_oduk_to_cpb,
                                            digi_resource_next_handle_t otn_handle,
                                            void *parent_handle_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    digi_resource_next_handle_t next_resource = otn_container_ptr->next_resource_handle;
    UINT32 next_index = otn_container_ptr->next_handle_index;

    PMC_ENTRY();

    if (LAST_DIGI_RESOURCE_HANDLE == otn_container_ptr->next_resource_handle)
    {
        PMC_RETURN(result);
    }

    if (TRUE == digi_resource_handle->force_release)
    {
        otn_container_ptr->rx_path_is_valid = FALSE;
        otn_container_ptr->tx_path_is_valid = FALSE;
    }
    else if (DIGI_ODUK_SWITCH != next_resource &&
             ((DIGI_DIRECTION_TX_ONLY == direction && 
              TRUE == otn_container_ptr->tx_path_is_valid) ||
             (DIGI_DIRECTION_RX_ONLY == direction && 
              TRUE == otn_container_ptr->rx_path_is_valid)))
    {
        /*
         * If the next resource is not the switch but the path is valid
         * for this direction then there is nothing else to do.
         */
        PMC_RETURN(result);
    }
    else if (DIGI_DIRECTION_TX_ONLY == direction &&
             DIGI_ODUK_SWITCH == next_resource &&
             FALSE == otn_container_ptr->tx_path_is_valid)
      {
        /*
         * The TX direction for this data path has been marked for release
         * Need to check first whether it is being used in broad/multi
         * cast scenario and as such valid.
         * If the connect array indicates that and it is fully operational
         * then do not release it.
         */
        UINT32 index = otn_container_ptr->next_handle_index;

        if (TRUE == digi_resource_handle->oduksw_cfg[index].source_activated &&
            TRUE == digi_resource_handle->oduksw_cfg[index].dest_activated &&
            LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->oduksw_cfg[index].source_handle_type &&
            LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->oduksw_cfg[index].primary_dest_handle_type)
        {
            UINT32 chnl = digi_resource_handle->oduksw_cfg[index].provisioned_handle_index;
            util_global_oduk_port_t port = digi_resource_get_oduk_port_type(digi_resource_handle->oduksw_cfg[index].provisioned_handle_type, &chnl);
            util_global_cpb_connect_t *oduk_connect_ptr = NULL;

            if (UTIL_GLOBAL_ODUK_PORT_COREOTN == port &&
                TRUE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle))
            {
                port = UTIL_GLOBAL_ODUK_PORT_ILKN1;
            }
            coreotn_oduksw_connect_data_state_get(digi_handle->coreotn_handle,
                                                  &oduk_connect_ptr);

            if ((TRUE == oduk_connect_ptr->chnl[(UINT32)port][chnl].slv.is_broadcast || 
                 TRUE == oduk_connect_ptr->chnl[(UINT32)port][chnl].slv.is_multicast) &&
                UTIL_GLOBAL_CHNL_OPERATIONAL == oduk_connect_ptr->chnl[(UINT32)port][chnl].slv.state &&
                UTIL_GLOBAL_CHNL_OPERATIONAL == oduk_connect_ptr->chnl[(UINT32)port][chnl].mst.state)
            {
                PMC_RETURN(PMC_SUCCESS);
            }
        }
    }
    else if (DIGI_DIRECTION_RX_ONLY == direction &&
             DIGI_ODUK_SWITCH == next_resource &&
             FALSE == otn_container_ptr->rx_path_is_valid)
    {
        /*
         * Handle the case the OTN container is a uni-directional data path
         * where the Rx side is actually going to another resource. That is
         * this is a bi-directional data path that that uses separate 
         * end points. Since t his is RX retrieve the source handle and
         * determine if that is the case.
         */
        UINT32 index = otn_container_ptr->next_handle_index;

        if (TRUE == digi_resource_handle->oduksw_cfg[index].dest_activated &&
            LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->oduksw_cfg[index].source_handle_type)
        {
            UINT32 oduk_index = 0;
            UINT32 cpb_index = 0;
            /*
             * Name is not quite valid however it is set to 0xFFFFFFFF 
             * which was desired value.
             */
            UINT32 src_index = DIGI_SERDES_UNASSIGNED;

            switch (digi_resource_handle->oduksw_cfg[index].source_handle_type)
            {
              case DIGI_OTN_MAPPER:
                src_index = digi_resource_handle->oduksw_cfg[index].source_handle_index;
                oduk_index = digi_resource_handle->mapper_cfg[src_index].oduk_switch_index;
                cpb_index = digi_resource_handle->mapper_cfg[src_index].cpb_switch_index;
                break;

              case DIGI_ILKN_1:
                src_index = digi_resource_handle->oduksw_cfg[index].source_handle_index;
                if (DIGI_ODUK_SWITCH == digi_resource_handle->ilkn_chnl_cfg[0][src_index].next_resource_handle)
                {
                    oduk_index = digi_resource_handle->ilkn_chnl_cfg[0][src_index].next_handle_index;
                }
                else
                {
                    src_index = DIGI_SERDES_UNASSIGNED;
                }
                break;

              case DIGI_ILKN_2:
                src_index = digi_resource_handle->oduksw_cfg[index].source_handle_index;
                if (DIGI_ODUK_SWITCH == digi_resource_handle->ilkn_chnl_cfg[1][src_index].next_resource_handle)
                {
                    oduk_index = digi_resource_handle->ilkn_chnl_cfg[1][src_index].next_handle_index;
                }
                else
                {
                    src_index = DIGI_SERDES_UNASSIGNED;
                }
                break;

              default:
                break;
            }
            /*
             * If MAPOTN was the source then both CPB and ODUK need to be
             * checked which is TRUE if cpb_index is non-zero. Otherwise
             * only check the ODUK source. As long as that endpoint is 
             * fully bi-directional then this data path should be valid.
             * NOTE: Could add a check for the source/destination of this
             * element and given this is the way the resource table was 
             * build it should be correct.
             */
            if (DIGI_SERDES_UNASSIGNED != src_index &&
                (TRUE == digi_resource_handle->oduksw_cfg[oduk_index].source_activated &&
                 TRUE == digi_resource_handle->oduksw_cfg[oduk_index].dest_activated &&
                 LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->oduksw_cfg[oduk_index].source_handle_type &&
                 LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->oduksw_cfg[oduk_index].primary_dest_handle_type) &&
                (0 == cpb_index ||
                 (TRUE == digi_resource_handle->cpbsw_cfg[cpb_index].source_activated &&
                  TRUE == digi_resource_handle->cpbsw_cfg[cpb_index].dest_activated &&
                  LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->cpbsw_cfg[cpb_index].source_handle_type &&
                  LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->cpbsw_cfg[cpb_index].primary_dest_handle_type)))
            {
                PMC_RETURN(PMC_SUCCESS);
            }
        }
    }
    /*
     * If SYSOTN and the High Order is connected to the switch
     * and the direction is as expected then retrieve the next
     * resource and operate on that otherwise this resource is
     * going to be removed.
     * NOTE: If ODUK Switch and path is valid for the direction
     * then this level resource should be valid for all subcontainers. 
     * However need to ensure that any like resources at this level are 
     * also checked which is the reason for looking at the next
     * associate resource.
     */
    if (DIGI_ODUK_SWITCH == next_resource &&
        FALSE == digi_resource_handle->force_release)
    {
        if ((DIGI_DIRECTION_TX_ONLY == direction && 
            TRUE == otn_container_ptr->tx_path_is_valid) ||
            (DIGI_DIRECTION_RX_ONLY == direction && 
             (TRUE == otn_container_ptr->rx_path_is_valid ||
              LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->oduksw_cfg[next_index].associate_dest_handle_type)))
          {
            /*
             * Need to ensure that the sibling of this resource is also
             * checked. Namely there could be one or more OTN containers at
             * this level that need to be checked. If there are no more 
             * then success can be returned. Otherwise get the next 
             * resource which will be checked when entering the while loop.
             */
            if (LAST_DIGI_RESOURCE_HANDLE == otn_container_ptr->next_associate_handle ||
                DIGI_CHANNEL_HANDLE_ID_HO_ODU_T == otn_container_ptr->handle_type)
            {
                PMC_RETURN(PMC_SUCCESS);
            }
            next_resource = otn_container_ptr->next_associate_handle;
            next_index = otn_container_ptr->next_associate_index;
        }
    }
    
    while (LAST_DIGI_RESOURCE_HANDLE != next_resource)
    {
        result = digi_resource_restart_release_port(digi_handle, 
                                                    digi_resource_handle,
                                                    (void *)otn_container_ptr->chnl_handle_ptr,
                                                    next_resource,
                                                    next_index,
                                                    is_oduk_to_cpb,
                                                    direction);

        if (0 == otn_container_ptr->lo_chnl_entries || 
            PMC_SUCCESS != result)
        {
            /*
             * Note that any entity, HO/MO/LO container that is connected
             * to ODUK should have lo_chnl_entries equal to 0. 
             */
            break;
        }
        else if (DIGI_LINE_OTN_HO_CONTAINER == otn_handle &&
                 (DIGI_LINE_OTN_MO_CONTAINER == next_resource ||
                 (TRUE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle) && 
                  DIGI_SYS_OTN_MO_CONTAINER == next_resource)))
        {
            next_resource = digi_resource_handle->line_otn_mo_container_cfg[next_index].next_associate_handle;
            next_index = digi_resource_handle->line_otn_mo_container_cfg[next_index].next_associate_index;
        }
        else if (DIGI_SYS_OTN_HO_CONTAINER == otn_handle &&
                 (DIGI_SYS_OTN_MO_CONTAINER == next_resource ||
                  DIGI_LINE_OTN_MO_CONTAINER == next_resource))
        {
            next_resource = digi_resource_handle->sys_otn_mo_container_cfg[next_index].next_associate_handle;
            next_index = digi_resource_handle->sys_otn_mo_container_cfg[next_index].next_associate_index;
        }
        else if (DIGI_LINE_OTN_LO_CONTAINER == next_resource)
        {
            next_resource = digi_resource_handle->line_otn_lo_container_cfg[next_index].next_associate_handle;
            next_index = digi_resource_handle->line_otn_lo_container_cfg[next_index].next_associate_index;
        }
        else if (DIGI_CHANNEL_HANDLE_ID_LO_ODU_T != otn_container_ptr->handle_type)
        {
            /*
             * If here then in an infinite loop. Use the assert
             * to catch this case and then determine what change
             * is required.
             */
            PMC_ASSERT(0, DIGI_ERR_INVALID_ARG, 0, 0);
        }
    }


    /*
     * Determine whether this datapath has been determined to be
     * corrupted. If so pull it down.
     */
    if (PMC_SUCCESS == result &&
        DIGI_DIRECTION_RX_ONLY == direction && 
        NULL != parent_handle_ptr &&
        TRUE == digi_resource_handle->force_release)
    {
        if (DIGI_CHANNEL_HANDLE_ID_HO_ODU_T == otn_container_ptr->handle_type)
        {
            digi_resource_otn_server_ho_deprov(digi_handle, 
                                               &otn_container_ptr->chnl_handle_ptr,
                                               (digi_otn_server_chnl_t **)&parent_handle_ptr);
            digi_resource_handle->datapath_release = TRUE;
        }
        else if (DIGI_CHANNEL_HANDLE_ID_MO_ODU_T == otn_container_ptr->handle_type ||
                 DIGI_CHANNEL_HANDLE_ID_LO_ODU_T == otn_container_ptr->handle_type)
        {
            digi_resource_otn_server_lo_deprov(digi_handle, 
                                               digi_resource_handle,
                                               otn_container_ptr->next_handle_index,
                                               &otn_container_ptr->chnl_handle_ptr,
                                               (digi_otn_server_chnl_t **)&parent_handle_ptr);
            digi_resource_handle->datapath_release = TRUE;
        }
        else
        {
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        }
    }


    PMC_RETURN(result);
} /* digi_resource_release_otn */

/*******************************************************************************
* digi_resource_find_parent_handle
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Checks the resource table for the parent handle and if found assigns it.
*  Otherwise based on the odu_level checks the upper OTN container to 
*  determine if it has a reference to the channel and returns that. Returns
*  TRUE if valid handle found and FALSE otherwise.
*
* INPUTS:
*  digi_handle              - Reference to the digi handle
*  digi_resource_handle_ptr - Reference to the resource handle
*  odu_level                - ODU level of the subcontainer searching for a
*                             parent. Only MO or LO is valid.
*  chnl                     - Channel of the subcontainer looking for a 
*                             parent handle.
*
* OUTPUTS:
*  otn_parent_pptr          - Reference to the parent handle if found.
*
* RETURNS:
*  TRUE                     - A parent value was found.
*  FALSE                    - Otherwise.
*
* NOTES:
*
******************************************************************************/
PRIVATE BOOL8 digi_resource_find_parent_handle(digi_handle_t *digi_handle,
                                               digi_resource_handle_summary_t *digi_resource_handle_ptr,
                                               util_global_odu_level_t odu_level,
                                               UINT32 chnl,
                                               digi_otn_odu_chnl_t **otn_parent_pptr)
{
    digi_otn_container_cfg_t *otn_ptr = NULL;
    UINT32 otn_iter_size;
    UINT32 i;

    PMC_ENTRY();

    if (UTIL_GLOBAL_LEVEL_LO_ODU == odu_level)
    {
        otn_ptr = &digi_resource_handle_ptr->line_otn_lo_container_cfg[chnl];
        otn_iter_size = DIGI_OTN_SERVER_MO_CHNL_MAX;
    }
    else
    {
        otn_ptr = &digi_resource_handle_ptr->line_otn_mo_container_cfg[chnl];
        otn_iter_size = DIGI_OTN_SERVER_HO_CHNL_MAX;
    }


    if (LAST_DIGI_RESOURCE_HANDLE == otn_ptr->parent_handle_type)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "Parent handle for ChannelId=%d is not SET in resource table for ODU_LEVEL=%d\n", chnl, odu_level);
    }
    else
    {
        UINT32 parent_index = otn_ptr->parent_handle_index;

        if (UTIL_GLOBAL_LEVEL_LO_ODU == odu_level)
        {
            *otn_parent_pptr = digi_resource_handle_ptr->line_otn_mo_container_cfg[parent_index].chnl_handle_ptr;
        }
        else
        {
            *otn_parent_pptr = digi_resource_handle_ptr->line_otn_ho_container_cfg[parent_index].chnl_handle_ptr;
        }

        if (NULL != *otn_parent_pptr)
        {
            PMC_RETURN(TRUE);
        }
    }

    for (i = 0; i < otn_iter_size; i++)
    {
        odu_struct_t *odu_ptr = NULL;

        if (UTIL_GLOBAL_LEVEL_LO_ODU == odu_level)
        {
            odu_ptr = &(digi_handle->handle_pool.mo_odu_struct[i]);
        }
        else
        {
            odu_ptr = &(digi_handle->handle_pool.ho_odu_struct[i]);
        }
        /*
         * If in here then the parent is not defined in the resource table
         * so need to look for it in the particular upper OTN container.
         * Look only if there are defined subcontainers in either 
         * direction and return TRUE on the first match. Note that the
         * number of calendar entries is 80 and should be the case for HO
         * or MO containers.
         */
        if (odu_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_rx > 0 ||
            odu_ptr->mem_ptr->trib_slot_ctxt.num_sub_container_tx > 0)
        {
            UINT32 j;
            
            for (j = 0; j < DIGI_NUM_CAL_ENTRIES_IN_100G; j++)
            {
                if (chnl == odu_ptr->mem_ptr->trib_slot_ctxt.sub_chnl_ids_rx[j])
                {
                    *otn_parent_pptr = (digi_otn_server_chnl_t *)odu_ptr;
                    PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                            "Found PARENT=%d for LO ORDER DEPROV Channel=%d\n", odu_ptr->switch_data.oduk_data.channel, chnl);
                    PMC_RETURN(TRUE);
                }
                if (chnl == odu_ptr->mem_ptr->trib_slot_ctxt.sub_chnl_ids_tx[j])
                {
                    *otn_parent_pptr = (digi_otn_server_chnl_t *)odu_ptr;
                    PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                            "Found PARENT=%d for LO ORDER DEPROV Channel=%d\n", odu_ptr->switch_data.oduk_data.channel, chnl);
                    PMC_RETURN(TRUE);
                }
            }
        }
    }
    PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
            "Parent handle for ChannelId=%d, ODU_LEVEL=%d NOT FOUND!!\n",
            chnl, odu_level);
    PMC_RETURN(FALSE);
} /* digi_resource_find_parent_handle */

/*******************************************************************************
* digi_resource_check_cpb_broadcast
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Function determines whether the CPB connected channel handle is in a 
*  broadcast/multicast data path before releasing. For ENET/CBRC serdes
*  ports connected to OTN ports the path valid flag will be set based on
*  the connectivity of the OTN connection. If the OTN side is uni
*  directional but the CPB side is bi-directional one side of the CPB
*  channel handle will not be flagged. Before releasing confirm whether or
*  not it is valid.
*
* INPUTS:
*  digi_handle           - Reference to the digi_handle
*  digi_resource_handle  - Reference to the digi_resource handle
*  serdes_port_ptr       - Serdes information for the CPB resource being
*                          checked for a broadcast/multicast case.
*
* OUTPUTS:
*  None
*
* RETURNS:
*  TRUE  - Serdes port channel handle is in a broadcast/multicast data path
*          and is valid. Caller should not release the data path.
*  FALSE - Otherwise
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_check_cpb_broadcast(digi_handle_t *digi_handle,
                                                digi_resource_handle_summary_t *digi_resource_handle,
                                                digi_serdes_port_intf_ctxt_t *serdes_port_ptr)
{
    UINT32 cpb_index;
    BOOL8 result = FALSE;

    PMC_ENTRY();

    /*
     * Sanity check before starting.
     */
    if (!(FALSE == serdes_port_ptr->tx_path_is_valid &&
          DIGI_CPB_SWITCH == serdes_port_ptr->next_resource_handle &&
          TRUE == serdes_port_ptr->path_verified &&
          NULL != serdes_port_ptr->chnl_handle_ptr))
    {
        PMC_RETURN(result);
    }
    cpb_index = serdes_port_ptr->next_handle_index;

    if (!(TRUE == digi_resource_handle->cpbsw_cfg[cpb_index].source_activated &&
          TRUE == digi_resource_handle->cpbsw_cfg[cpb_index].dest_activated &&
          LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->cpbsw_cfg[cpb_index].source_handle_type &&
          LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->cpbsw_cfg[cpb_index].primary_dest_handle_type))
    {
        PMC_RETURN(result);
    }
    /*
     * If num_dest is 1 then this CPB serdes channel handle is not 
     * broadcasting.  Retrieve what it is connected to and determine if
     * that resource is.
     */
    if (1 == digi_resource_handle->cpbsw_cfg[cpb_index].num_dest)
    {
        if (DIGI_OTN_MAPPER == digi_resource_handle->cpbsw_cfg[cpb_index].primary_dest_handle_type)
        {
            UINT32 map_index = digi_resource_handle->cpbsw_cfg[cpb_index].primary_dest_index;
            UINT32 cpb_map_index =  digi_resource_handle->mapper_cfg[map_index].cpb_switch_index;

            if (DIGI_OTN_MAPPER != digi_resource_handle->cpbsw_cfg[cpb_map_index].provisioned_handle_type ||
                map_index != digi_resource_handle->cpbsw_cfg[cpb_map_index].provisioned_handle_index ||
                digi_resource_handle->cpbsw_cfg[cpb_map_index].num_dest < 1 ||
                FALSE == digi_resource_handle->cpbsw_cfg[cpb_map_index].source_activated ||
                FALSE == digi_resource_handle->cpbsw_cfg[cpb_map_index].dest_activated ||
                LAST_DIGI_RESOURCE_HANDLE == digi_resource_handle->cpbsw_cfg[cpb_map_index].source_handle_type ||
                LAST_DIGI_RESOURCE_HANDLE == digi_resource_handle->cpbsw_cfg[cpb_map_index].primary_dest_handle_type)
            {
                result = FALSE;
            }
            else
            {
                UINT16 oduk_index = digi_resource_handle->mapper_cfg[map_index].oduk_switch_index;
                if (DIGI_OTN_MAPPER == digi_resource_handle->oduksw_cfg[oduk_index].provisioned_handle_type &&
                    map_index == digi_resource_handle->oduksw_cfg[oduk_index].provisioned_handle_index &&
                    TRUE == digi_resource_handle->oduksw_cfg[oduk_index].source_activated &&
                    TRUE == digi_resource_handle->oduksw_cfg[oduk_index].dest_activated &&
                    LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->oduksw_cfg[oduk_index].source_handle_type &&
                    LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->oduksw_cfg[oduk_index].primary_dest_handle_type)
                {
                    result = TRUE;
                }
            }
        }
    }

    PMC_RETURN(result);
 } /* digi_resource_check_cpb_broadcast */

#endif

#ifdef PMC_DEPRECATED

/*******************************************************************************
* digi_resource_release_port_to_switch
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Function recursively calls the next resource handle if connected and then
*  handles the deprovisioning of the current channel up to the current 
*  switching unit only. Demaps and deacticates any connections at the
*  switch but does not deprovision those resources.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *digi_resource_handle  - Resource handle table that has already been 
*                            built
*   parent_handle_ptr      - Pointer to the channel handle that effectively
*                            is making the call to this function
*   next_handle            - Resource handle type that is to be
*                            deprovisioned
*   next_index             - Index into the next_handle array being
*                            processed
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS - Associated serdes interface successfully released.
*   PMC_ERROR   - Error occurred along the way
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_resource_release_port_to_switch(digi_handle_t *digi_handle,
                                                       digi_resource_handle_summary_t *digi_resource_handle,
                                                       void *parent_handle_ptr,
                                                       digi_resource_next_handle_t next_handle,
                                                       UINT16 next_index)
{
    PMC_ERROR result = PMC_SUCCESS;
    digi_otn_container_cfg_t *otn_container_ptr = NULL;
    coreotn_odu_chnl_prov_mode_t prov_mode = LAST_COREOTN_CHNL_PROV_MODE;

    PMC_ENTRY();

    /*
     * Below is a listing of what the next handle from a serdes port can
     * be connected to. 
     * If next_handle is a CPB or OUDK switch then call the 
     * digi_resource_demap_switch to handle the deactivating and demapping
     * cases.  
     * All other handle values check for connectivity below and call the
     * function again recursively with its connection. When that completes
     * it can deprovision itself.
     * LINE_OTN_HO has to deprovision the LINEOTN channel afterwards as does
     * SYS_OTN HO channel.
     * Deprovisioning of ENET_LINE/ENET_SYS and CBRC channel handles is done
     * after this call returns as they are connected directly to the serdes
     * resource configuration and can easily be accessed from there.
     */
    switch (next_handle)
    {
      case DIGI_ODUK_SWITCH:
      case DIGI_CPB_SWITCH:
        result = digi_resource_demap_switch(digi_handle,
                                            digi_resource_handle,
                                            next_handle,
                                            next_index);
        break;

      case DIGI_ILKN_1:
      case DIGI_ILKN_2:
        {
            digi_ilkn_intf_t ilkn_inst = DIGI_ILKN1;
            digi_resource_next_handle_t switch_handle;
            UINT16 switch_index;

            if (DIGI_ILKN_2 == next_handle)
            {
                ilkn_inst = DIGI_ILKN2;
            }
            switch_handle = digi_resource_handle->ilkn_chnl_cfg[ilkn_inst][next_index].next_resource_handle;
            switch_index = digi_resource_handle->ilkn_chnl_cfg[ilkn_inst][next_index].next_handle_index;
            

            result = digi_resource_demap_switch(digi_handle,
                                                digi_resource_handle,
                                                switch_handle,
                                                switch_index);
            
            if (PMC_SUCCESS == result)
            {
                /*
                 * Now deprovision the channel. Need to retrieve the flow
                 * control calendar for the ilkn interface intance.
                 */
                digi_sifd_ilkn_calendar32_t tx_ilkn_fc_ptr;
                digi_sifd_ilkn_calendar32_t rx_ilkn_fc_ptr;

                digi_resource_flow_control_calendar_get(digi_handle,
                                                        ilkn_inst,
                                                        &tx_ilkn_fc_ptr,
                                                        &rx_ilkn_fc_ptr);

                if (DIGI_ODUK_SWITCH == switch_handle)
                {
                    result = digi_ilkn_oduk_chnl_deprov(digi_handle,
                                                        digi_resource_handle->ilkn_chnl_cfg[ilkn_inst][next_index].ilkn_chnl_handle_ptr,
                                                        &tx_ilkn_fc_ptr,
                                                        &rx_ilkn_fc_ptr);
                }
                else
                {
                    result = digi_ilkn_cpb_chnl_deprov(digi_handle,
                                                       digi_resource_handle->ilkn_chnl_cfg[ilkn_inst][next_index].ilkn_chnl_handle_ptr,
                                                       &tx_ilkn_fc_ptr,
                                                       &rx_ilkn_fc_ptr);
                }
            }
        }
        break;

      case DIGI_LINE_OTN_HO_CONTAINER:

        if (LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->line_otn_ho_container_cfg[next_index].next_resource_handle)
        {
            /*
             * If not LAST_DIGI_RESOURCE then this handle is pointing at
             * something. While this resource has resources under it 
             * demap/deactivate and deprovision them. The underlying caller
             * resets the next_resource_handle and index and decrements the
             * lo_chnl_entries counter ensuring the loop does not run for
             * ever.
             */
            while (TRUE)
            {
                result = digi_resource_release_port_to_switch(digi_handle, 
                                                              digi_resource_handle,
                                                              (void *)digi_resource_handle->line_otn_ho_container_cfg[next_index].chnl_handle_ptr,
                                                              digi_resource_handle->line_otn_ho_container_cfg[next_index].next_resource_handle,
                                                              digi_resource_handle->line_otn_ho_container_cfg[next_index].next_handle_index);

                if (0 == digi_resource_handle->line_otn_ho_container_cfg[next_index].lo_chnl_entries || PMC_SUCCESS != result)
                {
                    break;
                }
            }
        }
        
        if (PMC_SUCCESS == result)
        {
            result = digi_otn_server_ho_deprov(digi_handle, 
                                               &digi_resource_handle->line_otn_ho_container_cfg[next_index].chnl_handle_ptr,
                                               (digi_otn_server_chnl_t **)&parent_handle_ptr);
        }
        
        if (PMC_SUCCESS == result)
        {
            result = digi_otn_server_line_deprov(digi_handle,
                                                 (digi_otn_server_chnl_t **)&parent_handle_ptr);
        }
        /*
         * reset_container updates associate_index pointers for this class
         * of container as well updates the next_resource handle of the 
         * parent if applicable. Then sets the container to default values
         */
        if (PMC_SUCCESS == result)
        {
            digi_resource_reset_container(digi_resource_handle,
                                          DIGI_LINE_OTN_HO_CONTAINER, 
                                          &digi_resource_handle->line_otn_ho_container_cfg[next_index],
                                          next_index);
        }

        break;

      case DIGI_LINE_OTN_MO_CONTAINER:
        if (LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->line_otn_mo_container_cfg[next_index].next_resource_handle)
        {
            /*
             * Iterate over the lower order channels and release them first
             * Each lower order channel should recursively release anything
             * it is connected to. As well it should decrement the lo_chnl
             * entry counter as well as reset the next_resource_handle and
             * index
             */
            while (TRUE)
            {
                result = digi_resource_release_port_to_switch(digi_handle, 
                                                              digi_resource_handle,
                                                              (void *)digi_resource_handle->line_otn_mo_container_cfg[next_index].chnl_handle_ptr,
                                                              digi_resource_handle->line_otn_mo_container_cfg[next_index].next_resource_handle,
                                                              digi_resource_handle->line_otn_mo_container_cfg[next_index].next_handle_index);

                if (0 == digi_resource_handle->line_otn_mo_container_cfg[next_index].lo_chnl_entries || PMC_SUCCESS != result)
                {
                    break;
                }
            }
        }

        if (PMC_SUCCESS == result)
        {
            result = digi_otn_server_lo_deprov(digi_handle, 
                                               &digi_resource_handle->line_otn_mo_container_cfg[next_index].chnl_handle_ptr,
                                               (digi_otn_odu_chnl_t **)&parent_handle_ptr);
        }
        if (PMC_SUCCESS == result)
        {
            result = coreotn_odu_chnl_prov_mode_get(digi_handle->coreotn_handle, &prov_mode);
            if (PMC_SUCCESS == result && COREOTN_ODU_CHNL_PROV_ALL == prov_mode)
            {
                result = digi_otn_server_lo_deprov_end(digi_handle, (digi_otn_odu_chnl_t **)&parent_handle_ptr);
                PMC_ASSERT(PMC_SUCCESS == result, DIGI_ERR_INVALID_ARG, 0, 0);
            }
        }
 
        if (PMC_SUCCESS == result)
        {
            digi_resource_reset_container(digi_resource_handle,
                                          DIGI_LINE_OTN_MO_CONTAINER, 
                                          &digi_resource_handle->line_otn_mo_container_cfg[next_index],
                                          next_index);
        }
        break;

      case DIGI_LINE_OTN_LO_CONTAINER:
        otn_container_ptr = &digi_resource_handle->line_otn_lo_container_cfg[next_index];

        if (LAST_DIGI_RESOURCE_HANDLE != otn_container_ptr->next_resource_handle)
        {
            result = digi_resource_release_port_to_switch(digi_handle, 
                                                          digi_resource_handle,
                                                          (void *)otn_container_ptr->chnl_handle_ptr,
                                                          otn_container_ptr->next_resource_handle,
                                                          otn_container_ptr->next_handle_index);
        }

        if (PMC_SUCCESS == result)
        {
            if (FALSE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle))
            {
                result = digi_otn_server_lo_deprov(digi_handle, 
                                                   &otn_container_ptr->chnl_handle_ptr,
                                                   (digi_otn_odu_chnl_t **)&parent_handle_ptr);
                if (PMC_SUCCESS == result)
                {
                    result = coreotn_odu_chnl_prov_mode_get(digi_handle->coreotn_handle, &prov_mode);
                    if (PMC_SUCCESS == result && COREOTN_ODU_CHNL_PROV_ALL == prov_mode)
                    {
                        result = digi_otn_server_lo_deprov_end(digi_handle, (digi_otn_odu_chnl_t **)&parent_handle_ptr);
                        PMC_ASSERT(PMC_SUCCESS == result, DIGI_ERR_INVALID_ARG, 0, 0);
                    }
                }
 
            }
            else
            {
                result = digi_otn_server_ho_deprov(digi_handle, 
                                                   &otn_container_ptr->chnl_handle_ptr,
                                                   (digi_otn_odu_chnl_t **)&parent_handle_ptr);
            }
        }
        if (PMC_SUCCESS == result)
        {
            digi_resource_reset_container(digi_resource_handle,
                                          DIGI_LINE_OTN_LO_CONTAINER, 
                                          otn_container_ptr,
                                          next_index);
        }
        break;

      case DIGI_SYS_OTN_HO_CONTAINER:
        if (LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->sys_otn_ho_container_cfg[next_index].next_resource_handle)
        {
            while (TRUE)
            {
                result = digi_resource_release_port_to_switch(digi_handle, 
                                                              digi_resource_handle,
                                                              (void *)digi_resource_handle->sys_otn_ho_container_cfg[next_index].chnl_handle_ptr,
                                                              digi_resource_handle->sys_otn_ho_container_cfg[next_index].next_resource_handle,
                                                              digi_resource_handle->sys_otn_ho_container_cfg[next_index].next_handle_index);

                if (0 == digi_resource_handle->sys_otn_ho_container_cfg[next_index].lo_chnl_entries || PMC_SUCCESS != result)
                {
                    break;
                }
            }
        }
        
        if (PMC_SUCCESS == result)
        {
            result = digi_otn_server_ho_deprov(digi_handle, 
                                               &digi_resource_handle->sys_otn_ho_container_cfg[next_index].chnl_handle_ptr,
                                               (digi_otn_server_chnl_t **)&parent_handle_ptr);
        }
        
        if (PMC_SUCCESS == result)
        {
            result = digi_otn_server_sys_deprov(digi_handle,
                                                (digi_otn_server_chnl_t **)&parent_handle_ptr);
        }
        if (PMC_SUCCESS == result)
        {
            digi_resource_reset_container(digi_resource_handle,
                                          DIGI_SYS_OTN_HO_CONTAINER, 
                                          &digi_resource_handle->line_otn_ho_container_cfg[next_index],
                                          next_index);
        }
        break;

      case DIGI_SYS_OTN_MO_CONTAINER:
        if (LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->sys_otn_mo_container_cfg[next_index].next_resource_handle)
        {
            result = digi_resource_release_port_to_switch(digi_handle, 
                                                          digi_resource_handle,
                                                          (void *)digi_resource_handle->sys_otn_mo_container_cfg[next_index].chnl_handle_ptr,
                                                          digi_resource_handle->sys_otn_mo_container_cfg[next_index].next_resource_handle,
                                                          digi_resource_handle->sys_otn_mo_container_cfg[next_index].next_handle_index);
        }
        
        if (PMC_SUCCESS == result)
        {
            if (NULL == parent_handle_ptr)
            {
                UINT16 parent_index = digi_resource_handle->sys_otn_mo_container_cfg[next_index].parent_handle_index;
                parent_handle_ptr = digi_resource_handle->sys_otn_ho_container_cfg[parent_index].chnl_handle_ptr;
            }
            result = digi_otn_server_lo_deprov(digi_handle, 
                                               &digi_resource_handle->sys_otn_mo_container_cfg[next_index].chnl_handle_ptr,
                                               (digi_otn_odu_chnl_t **)&parent_handle_ptr);
            if (PMC_SUCCESS == result)
            {
                result = coreotn_odu_chnl_prov_mode_get(digi_handle->coreotn_handle, &prov_mode);
                if (PMC_SUCCESS == result && COREOTN_ODU_CHNL_PROV_ALL == prov_mode)
                {
                    result = digi_otn_server_lo_deprov_end(digi_handle, (digi_otn_odu_chnl_t **)&parent_handle_ptr);
                    PMC_ASSERT(PMC_SUCCESS == result, DIGI_ERR_INVALID_ARG, 0, 0);
                }
            }
 
        }
        if (PMC_SUCCESS == result)
        {
            digi_resource_reset_container(digi_resource_handle,
                                          DIGI_SYS_OTN_MO_CONTAINER, 
                                          &digi_resource_handle->sys_otn_ho_container_cfg[next_index],
                                          next_index);
        }
        break;

      default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
      }

    PMC_RETURN(result);
} /* digi_resource_release_port_to_switch */


/*******************************************************************************
* digi_resource_demap_switch
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Determines what the next_handle/next_index is connected and deactivates
*   and demaps as well as deactivating and demapping the current resource.
*   When this returns the reosurce can be deprovisioned.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *digi_resource_handle  - Resource handle table that has already been 
*                            built
*   next_handle            - In this case represents one of the 2 switches
*   next_index             - Switch index of the resources to be demapped
*                            and deactivated.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS - Resources at the switch successfully demapped and 
*                 deactivated
*   PMC_ERROR   - Error occured demapping/deactivated or a sanity test 
*                 failed
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_resource_demap_switch(digi_handle_t *digi_handle,
                                             digi_resource_handle_summary_t *digi_resource_handle,
                                             digi_resource_next_handle_t next_handle,
                                             UINT16 next_index)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    util_global_switch_data_t *prov_ptr = NULL;
    digi_resource_next_handle_t prov_handle = LAST_DIGI_RESOURCE_HANDLE;
    UINT16 prov_index;
    UINT16 port_type;
    UINT32 port_chnl;
    digi_switch_ctxt_t *switch_ptr = NULL;
    BOOL8 secondary_set = FALSE;

    PMC_ENTRY();

    if (DIGI_ODUK_SWITCH == next_handle)
    {
        switch_ptr = &digi_resource_handle->oduksw_cfg[next_index];
        coreotn_oduksw_connect_data_state_get(digi_handle->coreotn_handle,
                                              &g_oduk_connect_data_ptr);
    }
    else
    {
        switch_ptr = &digi_resource_handle->cpbsw_cfg[next_index];
        cpb_connect_data_state_get(digi_handle->dcpb_handle,
                                   &g_cpb_connect_data_ptr);
    }
    prov_index = switch_ptr->provisioned_handle_index;
    prov_handle = switch_ptr->provisioned_handle_type;
    PMC_LOG_TRACE("Switch=%d Next Index=%d, handle=%d and handleIndex=%d\n",
                  next_handle, next_index, prov_handle, prov_index);

    switch (next_handle)
    {
      case DIGI_ODUK_SWITCH:

        switch(prov_handle)
        {
          case DIGI_LINE_OTN_HO_CONTAINER:
            prov_ptr =  digi_resource_handle->line_otn_ho_container_cfg[prov_index].chnl_handle_ptr;
            break;
            
          case DIGI_LINE_OTN_MO_CONTAINER:
            prov_ptr =  digi_resource_handle->line_otn_mo_container_cfg[prov_index].chnl_handle_ptr;
            break;
            
          case DIGI_LINE_OTN_LO_CONTAINER:
            prov_ptr = digi_resource_handle->line_otn_lo_container_cfg[prov_index].chnl_handle_ptr;
            break;
                
          case DIGI_SYS_OTN_MO_CONTAINER:
            prov_ptr =  digi_resource_handle->sys_otn_mo_container_cfg[prov_index].chnl_handle_ptr;
            break;
            
          case DIGI_ILKN_1:
            prov_ptr =  digi_resource_handle->ilkn_chnl_cfg[0][prov_index].ilkn_chnl_handle_ptr;
            break;
            
          case DIGI_ILKN_2:
            prov_ptr =  digi_resource_handle->ilkn_chnl_cfg[1][prov_index].ilkn_chnl_handle_ptr;
            break;

        case DIGI_OTN_MAPPER:
            prov_ptr =  digi_resource_handle->mapper_cfg[prov_index].mapper_handle_ptr;
          break;

          default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
            break;
        }
        port_chnl = ((util_global_switch_data_def_t *)prov_ptr)->oduk_data.channel;
        port_type = (UINT16)((util_global_switch_data_def_t *)prov_ptr)->oduk_data.port_type;
        break;
    case DIGI_CPB_SWITCH:
        /*
         * prov_handle should be relative to the particular switch.
         * ILKN 1/2 and MAPOTN overlap. Otherwise the handles are switch
         * specific.
         */
        switch(prov_handle)
        {
          case DIGI_LINE_SERDES_CFG:
            prov_ptr = digi_resource_handle->line_serdes_cfg[prov_index].chnl_handle_ptr;
            break;

          case DIGI_SYS_SERDES_CFG:
            prov_ptr = digi_resource_handle->sys_serdes_cfg[prov_index].chnl_handle_ptr;
            break;

          case DIGI_SFI51_SERDES_CFG:
            prov_ptr = digi_resource_handle->sfi51_serdes_cfg[prov_index].chnl_handle_ptr;
            break;

          case DIGI_ILKN_1:
            prov_ptr =  digi_resource_handle->ilkn_chnl_cfg[0][prov_index].ilkn_chnl_handle_ptr;
            break;
            
          case DIGI_ILKN_2:
            prov_ptr =  digi_resource_handle->ilkn_chnl_cfg[1][prov_index].ilkn_chnl_handle_ptr;
            break;
  
          case DIGI_OTN_MAPPER:
             prov_ptr =  digi_resource_handle->mapper_cfg[prov_index].mapper_handle_ptr;
            break;

          default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);

        }
        port_type = (UINT16)((util_global_switch_data_def_t *)prov_ptr)->cpb_data.port_type;
        port_chnl = ((util_global_switch_data_def_t *)prov_ptr)->cpb_data.channel;
        PMC_ASSERT(port_type < UTIL_GLOBAL_CPB_PORT_NOT_USED, DIGI_ERR_INVALID_ARG, 0, 0);
       break;

    default:
      PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
      break;
    }
    /*
     * Deactivate and demap the destinations that this resource is
     * transmitting to.
     */
    result = digi_resource_clean_source_and_destination(digi_handle,
                                                        digi_resource_handle,
                                                        switch_ptr,
                                                        port_type,
                                                        port_chnl,
                                                        next_handle);
    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /*
     * Need to determine if this resource is connected to another
     * resource that is broadcast and as such this resource is the
     * primary and that resource is the secondary. If that is the case
     * then the master.next_dest port needs to be deactivated and
     * demapped before this resource can even though this resource is
     * not directly connected to that other resource.
     */
    if (DIGI_ODUK_SWITCH == next_handle)
    {
        if (!(UTIL_GLOBAL_CHNL_START == g_oduk_connect_data_ptr->chnl[port_type][port_chnl].mst.state ||
              CPB_INDEX_LAST == g_oduk_connect_data_ptr->chnl[port_type][port_chnl].mst.next_dest_mst_port))
        {
            UINT16 tmp_port_type = g_oduk_connect_data_ptr->chnl[port_type][port_chnl].mst.next_dest_mst_port;
            port_chnl = g_oduk_connect_data_ptr->chnl[port_type][port_chnl].mst.next_dest_mst_chnl;
            port_type = tmp_port_type;
            secondary_set = TRUE;
        }
    }
    else if (!(UTIL_GLOBAL_CHNL_START == g_cpb_connect_data_ptr->chnl[port_type][port_chnl].mst.state ||
               CPB_INDEX_LAST == g_cpb_connect_data_ptr->chnl[port_type][port_chnl].mst.next_dest_mst_port))
    {
        UINT16 tmp_port_type = g_oduk_connect_data_ptr->chnl[port_type][port_chnl].mst.next_dest_mst_port;
        port_chnl = g_oduk_connect_data_ptr->chnl[port_type][port_chnl].mst.next_dest_mst_chnl;
        port_type = tmp_port_type;
        secondary_set = TRUE;
    }

    if (TRUE == secondary_set)
    {
        result = digi_resource_deactivate_demap(digi_handle,
                                                digi_resource_handle,
                                                port_type,
                                                port_chnl,
                                                next_handle);
    }
            
    if (PMC_SUCCESS == result)
    {
        /*
         * If this resource has been activated then deactivated it and 
         * then demap it.
         */
        if (TRUE == switch_ptr->dest_activated)            
        {
            if (DIGI_ODUK_SWITCH == next_handle)
            {
                result = digi_oduksw_chnl_deactivate(digi_handle, prov_ptr);
            }
            else
            {
                result = digi_cpb_chnl_deactivate(digi_handle, prov_ptr); 
            }
        }
        if (PMC_SUCCESS == result )
        {
            switch_ptr->dest_activated = FALSE;

            if (DIGI_ODUK_SWITCH == next_handle)
            {
                result = digi_oduksw_chnl_demap(digi_handle, prov_ptr);
            }
            else
            {
                result = digi_cpb_chnl_demap(digi_handle, prov_ptr);
            }
        }
    }
            
    if (PMC_SUCCESS == result)
    {
        switch_ptr->provisioned_handle_type = LAST_DIGI_RESOURCE_HANDLE;
        switch_ptr->provisioned_handle_index = 0;
        
        switch_ptr->source_handle_type = LAST_DIGI_RESOURCE_HANDLE;
        switch_ptr->source_handle_index = 0;
        switch_ptr->source_activated = FALSE;
        switch_ptr->primary_dest_handle_type = LAST_DIGI_RESOURCE_HANDLE;
        switch_ptr->primary_dest_index = 0;
        switch_ptr->num_dest = 0;
    }

    PMC_RETURN(result);
} /* digi_resource_demap_switch */

/*******************************************************************************
* digi_resource_reset_container
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Function to handle the reseting of a OTN container to default values.
*   Resets the associate handle index to what this entity points to for any
*   other handle pointing at this one. As well updates the next_resource 
*   index of the parent if pointing at this OTN container.
*
* INPUTS:
*   *digi_resource_handle  - Resource handle table that has already been 
*                            built
*   handle                 - OTN container resource to be reset
*   otn_container_ptr      - Pointer to the otn container.
*   index                  - OTN container index
*
* OUTPUTS:
*  None
*
* RETURNS:
*  None
*
*******************************************************************************/
PRIVATE void digi_resource_reset_container(digi_resource_handle_summary_t *digi_resource_handle,
                                           digi_resource_next_handle_t handle,
                                           digi_otn_container_cfg_t *otn_container_ptr,
                                           UINT16 index)
{
    digi_resource_next_handle_t associate_handle = otn_container_ptr->next_associate_handle;
    UINT16 associate_index = otn_container_ptr->next_associate_index;
    UINT8 i;
    UINT16 parent_index = 0;

    PMC_ENTRY();

    if (TRUE == otn_container_ptr->tx_path_is_valid ||
        TRUE == otn_container_ptr->rx_path_is_valid)
    {
        PMC_RETURN();
    }

    switch (handle)
    {
      case DIGI_LINE_OTN_HO_CONTAINER:
        for (i=0; i < DIGI_OTN_SERVER_HO_CHNL_MAX; i++)
        {
            /*
             * Iterate over the list of High Order channels. If any entry's
             * associate index is pointing at this index then its 
             * associated index needs to be updated with the handle and 
             * index of the container that is being cleaned.
             */
            if (i != index &&
                LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->line_otn_ho_container_cfg[i].next_associate_handle &&
                index == digi_resource_handle->line_otn_ho_container_cfg[i].next_associate_index)
            {
                digi_resource_handle->line_otn_ho_container_cfg[i].next_associate_index = associate_index;
                digi_resource_handle->line_otn_ho_container_cfg[i].next_associate_handle = associate_handle;
                break;
            }
        }
        break;

      case DIGI_LINE_OTN_MO_CONTAINER:
        for (i=0; i < DIGI_OTN_SERVER_MO_CHNL_MAX; i++)
        {
            /*
             * Iterate over the list of Mid Order channels. If any entry's
             * associate index is pointing at this index then its 
             * associated index needs to be updated with the handle and 
             * index of the container that is being cleaned.
             */
            if (i != index &&
                LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->line_otn_mo_container_cfg[i].next_associate_handle &&
                index == digi_resource_handle->line_otn_mo_container_cfg[i].next_associate_index)
            {
                digi_resource_handle->line_otn_mo_container_cfg[i].next_associate_index = associate_index;
                digi_resource_handle->line_otn_mo_container_cfg[i].next_associate_handle = associate_handle;
                break;
            }
        }
        /*
         * Find the parent index and decrement the lo_chnl_entries and
         * update the next_handle_index. If it is pointing to this
         * entry then have it point to the associate index so the next
         * one in the list will be released.
         */
        parent_index = otn_container_ptr->parent_handle_index;

        if (DIGI_LINE_OTN_HO_CONTAINER == otn_container_ptr->parent_handle_type)
        {
            digi_resource_handle->line_otn_ho_container_cfg[parent_index].lo_chnl_entries--;
        }
        break;

      case DIGI_LINE_OTN_LO_CONTAINER:
        for (i=0; i < DIGI_OTN_SERVER_LO_CHNL_MAX; i++)
        {
            if (i != index &&
                LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->line_otn_lo_container_cfg[i].next_associate_handle &&
                index == digi_resource_handle->line_otn_lo_container_cfg[i].next_associate_index)
            {
                digi_resource_handle->line_otn_lo_container_cfg[i].next_associate_index = associate_index;
                digi_resource_handle->line_otn_lo_container_cfg[i].next_associate_handle = associate_handle;
                break;
            }
        }
        /*
         * Find the parent index and decrement the lo_chnl_entries and
         * update the next_handle_index. If it is pointing to this
         * entry then have it point to the associate index so the next
         * one in the list will be released.
         */
        parent_index = otn_container_ptr->parent_handle_index;
        
        if (DIGI_LINE_OTN_MO_CONTAINER == otn_container_ptr->parent_handle_type)
        {
            digi_resource_handle->line_otn_mo_container_cfg[parent_index].lo_chnl_entries--;
        }
        break;

      case DIGI_SYS_OTN_MO_CONTAINER:
        for (i=0; i < DIGI_OTN_SERVER_MO_CHNL_MAX; i++)
        {
            if (i != index &&
                LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->sys_otn_mo_container_cfg[i].next_associate_handle &&
                index == digi_resource_handle->sys_otn_mo_container_cfg[i].next_associate_index)
            {
                digi_resource_handle->sys_otn_mo_container_cfg[i].next_associate_index = associate_index;
                digi_resource_handle->sys_otn_mo_container_cfg[i].next_associate_handle = associate_handle;
                break;
            }
        }
        /*
         * For SYSOTN there is only ever 1 HO container. The parent_index
         * is always and only 0.
         */
        if (DIGI_SYS_OTN_HO_CONTAINER == otn_container_ptr->parent_handle_type)
        {
            digi_resource_handle->sys_otn_ho_container_cfg[0].lo_chnl_entries--;
        }
        break;

      default:
        PMC_RETURN();
    }
    otn_container_ptr->parent_handle_type = LAST_DIGI_RESOURCE_HANDLE;
    otn_container_ptr->parent_handle_index = 0;
    otn_container_ptr->handle_type = LAST_DIGI_HANDLE_ID;
    otn_container_ptr->chnl_handle_ptr = NULL;
    otn_container_ptr->payload_mode = LAST_DIGI_ODU_PAYLOAD_FORMAT;
    otn_container_ptr->tx_trib_slot_mask[0] = 0;
    otn_container_ptr->tx_trib_slot_mask[1] = 0;
    otn_container_ptr->tx_trib_slot_mask[2] = 0;
    otn_container_ptr->rx_trib_slot_mask[0] = 0;
    otn_container_ptr->rx_trib_slot_mask[1] = 0;
    otn_container_ptr->rx_trib_slot_mask[2] = 0;
    otn_container_ptr->lo_chnl_entries = 0;
    otn_container_ptr->next_resource_handle = LAST_DIGI_RESOURCE_HANDLE;
    otn_container_ptr->next_handle_index = 0;

    PMC_RETURN();
} /* digi_resource_reset_container */

#endif /* PMC_DEPRECATED */
