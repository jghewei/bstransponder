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
#include <stdio.h>
#include "digi_loc.h"
#include "digi_resource_loc.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
/*
** Global variables
*/
/*! flag to differentiate line from sysotn side */
BOOL8 is_line_side = FALSE; 
/*! Global defined ODUK Switch connect array variable */
util_global_cpb_connect_t *g_oduk_connect_data_ptr = NULL;
/*! Global defined CPB Switch connect array variable */
util_global_cpb_connect_t *g_cpb_connect_data_ptr = NULL;
/*! String returned by conversion functions ensuring it does not go out of scope before it is used. */
PRIVATE const char *global_string = "";


#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */

PRIVATE void digi_resource_handle_init(digi_resource_handle_summary_t *digi_resource_handle);

PRIVATE PMC_ERROR digi_resource_handles_get(digi_handle_t *digi_handle,
                                            UINT8 digi_serdes_index,
                                            digi_serdes_intf_t intf,
                                            digi_resource_handle_summary_t *digi_resource_handle,
                                            UINT16 *last_index,
                                            digi_resource_next_handle_t *last_handle);

PRIVATE BOOL8 digi_is_cbrc_connected(digi_handle_t *digi_handle,
                                     UINT8 digi_serdes_index,
                                     digi_serdes_intf_t intf,
                                     UINT32 port_uid,
                                     digi_resource_handle_summary_t *digi_resource_handle);

PRIVATE BOOL8 digi_is_enet_connected(digi_handle_t *digi_handle,
                                     UINT8 digi_serdes_index,
                                     digi_serdes_intf_t intf,
                                     UINT32 port_uid,
                                     digi_resource_handle_summary_t *digi_resource_handle);

PRIVATE BOOL8 digi_is_otn_connected(digi_handle_t *digi_handle,
                                    UINT8 serdes_index,
                                    digi_serdes_intf_t intf,
                                    UINT32 port_uid,
                                    digi_resource_handle_summary_t *digi_resource_handle);

PRIVATE PMC_ERROR digi_line_otn_subcontainer_set(digi_handle_t *digi_handle,
                                                 UINT8 ho_index,
                                                 digi_resource_handle_summary_t *digi_resource_handle,
                                                 UINT16 *last_index,
                                                 digi_resource_next_handle_t *last_handle);

PRIVATE PMC_ERROR digi_lo_otn_subcontainer_set(digi_handle_t *digi_handle,
                                               UINT8 mo_index,
                                               digi_resource_handle_summary_t *digi_resource_handle);

PRIVATE PMC_ERROR digi_sys_otn_subcontainer_set(digi_handle_t *digi_handle,
                                                UINT8 ho_index,
                                                digi_resource_handle_summary_t *digi_resource_handle);

PRIVATE void digi_resource_connect_switch_data(digi_handle_t *digi_handle, 
                                               digi_resource_handle_summary_t *digi_resource_handle);

#ifdef PMC_DEPRECATED

/* LCOV_EXCL_START */
PRIVATE void digi_resource_clean_dcsu(digi_handle_t *digi_handle);
/* LCOV_EXCL_STOP */

#endif /* PMC_DEPRECATED */
PRIVATE void digi_resource_clean_post_mld_dsi_ctxt(digi_handle_t *digi_handle,
                                                   digi_serdes_intf_t  intf);

PRIVATE void digi_resource_clean_firmware(digi_handle_t *digi_handle);
PRIVATE void digi_resource_clean_mcpb_switch(digi_handle_t                  *digi_handle);

PRIVATE void digi_resource_mcpb_port_cleanup(digi_handle_t                   *digi_handle,
                                             UINT32                           link,
                                             util_global_mcpb_port_t          port_1,
                                             util_global_mcpb_port_t          port_2,
                                             BOOL8                            unprov);

PRIVATE BOOL8 digi_resource_post_mld_lane_mapotn_chnl_status_get(digi_handle_t      *digi_handle,
                                                                 digi_serdes_intf_t  intf,
                                                                 UINT8               lane);

/*
 * PRINT related utility functions
 */

/* LCOV_EXCL_START */


PRIVATE const char * digi_convert_handle_id(digi_handle_id_t handle_id);

PRIVATE const char * digi_resource_convert_resource_handle(digi_resource_next_handle_t resource_id);

PRIVATE const char * digi_convert_payload(digi_odu_line_payload_t payload);

PRIVATE BOOL8 digi_print_next_resource(digi_handle_t *digi_handle,
                                       digi_resource_next_handle_t next_resource,
                                       UINT16 next_index,
                                       UINT16 parent_index,
                                       digi_resource_handle_summary_t *digi_resource_handle);

PRIVATE void digi_print_ilkn_channels(digi_resource_handle_summary_t *digi_resource_handle);

PRIVATE void digi_print_mapotn_channels(digi_handle_t *digi_handle,
                                        digi_resource_handle_summary_t *digi_resource_handle);

PRIVATE void digi_print_switch(digi_resource_handle_summary_t *digi_resource_handle);

PRIVATE void digi_resource_print_coreotn_channels(const char *first_string,
                                                  UINT16 parent_index,
                                                  UINT16 current_index,
                                                  const char *handle_type,
                                                  void *chnl_handle_ptr,
                                                  digi_odu_line_payload_t payload_mode,
                                                  UINT16 lo_chnl_entries,
                                                  const char *next_associate_handle,
                                                  UINT16 next_associate_index,
                                                  const char *next_resource_handle,
                                                  UINT16 next_handle_index,
                                                  UINT32 tx_trib_slot_mask[3],
                                                  UINT32 rx_trib_slot_mask[3]);

PRIVATE void digi_print_header_footer(const char *header_string, BOOL8 header);

PRIVATE const char *digi_resource_convert_mapper_mode(digi_handle_t *digi_handle,
                                                      digi_mapper_chnl_t *enet_ptr);

PRIVATE BOOL8 digi_resource_check_path(digi_handle_t *digi_handle,
                                       digi_resource_handle_summary_t *digi_resource_handle,
                                       digi_serdes_port_intf_ctxt_t *serdes_port_ptr,
                                       BOOL8 *operational_path);

PRIVATE void digi_resource_release_datapath(digi_handle_t *digi_handle,
                                            digi_resource_handle_summary_t *digi_resource_handle,
                                            digi_serdes_port_intf_ctxt_t *serdes_port_ptr);



/* LCOV_EXCL_STOP */


#endif /* DOXYGEN_PUBLIC_ONLY */

/*
** Public Functions
*/
/*******************************************************************************
* digi_resource_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Functions constructs the digi_resource_handle_summary_t structure from the
*  digi_handle.  Structure recreates the data paths as defined in DIGI 
*  Applib within the digi_resource_handle_summary_t structure. The structure
*  provides channel handles as well the next resource the handle is 
*  connected to. The connectivity of the structures represents the various
*  DIGI Applib and HW sub systems and their relationship within a data path.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*
* OUTPUTS:
*   *digi_resource_handle  - Structure to hold the dynamic and static handles
*                            representing the current DIGI connectivity.
*
* RETURNS:
*   PMC_SUCCESS            - All went well.
*   DIGI_ERR_INVALID_ARG   - Parameters are invalid.
*   DIGI_ERR_??
*
* NOTES:
*   When this function is called it is assumed that any coherency 
*   validation between DIGI Applib and HW has been performed and that the
*   state of the underlying data is either in use or not in use and is not
*   in an intermediate state within the operation being performed. That is
*   if provisioned it is fully provisioned, if activated fully activated,etc
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_resource_state_get(digi_handle_t *digi_handle,
                                         digi_resource_handle_summary_t *digi_resource_handle)
{
    UINT8 i;
    UINT16 last_index = 0;
    digi_resource_next_handle_t last_handle = LAST_DIGI_RESOURCE_HANDLE;    
    BOOL8 serdes_prov_status;
    digi_serdes_port_mode_t serdes_port_mode;
    digi_serdes_prov_state_t serdes_prov_state;
    /*
     * Flag indicates whether or not there are any valid connections. If
     * false the table is re-initialized to default values before exiting
     * the function.
     */
    BOOL8 valid_result = FALSE;
    PMC_ATOMIC_ENTRY(digi_handle);
    /*
     * Initial sanity check
     */
    if (digi_handle == NULL || digi_resource_handle == NULL)
    {
        PMC_ATOMIC_RETURN(digi_handle, DIGI_ERR_NULL_HANDLE);
    }  
    digi_resource_handle_init(digi_resource_handle);
    /*
     * Retrieve the cpb and oduk switch configuration that is used to set
     * source and destination handles and indices. Ensure the structures are
     * reset before using as this may not be the first call.
     */
    coreotn_oduksw_connect_data_state_get(digi_handle->coreotn_handle,
                                          &g_oduk_connect_data_ptr);

    cpb_connect_data_state_get(digi_handle->dcpb_handle,
                              &g_cpb_connect_data_ptr);


    /*
     * Retrieve the ILKN and MAPOTN channel handles first so that if 
     * the underlying COREOTN module connects to one of them the channel
     * handles will be filled in and accessible for determining what OTN
     * is connected to.
     * NOTE: It may be that all modules need to fill in there handles before
     * determining their connectivity. For now it has been determined that 
     * these 2 modules need to be set the handles before anything else.
     */
    for (i=0; i < DIGI_MAPOTN_CHNL_MAX; i++)
    {
        if (TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.mapper_chnl[i].switch_data.header.prov_state))
        {
            digi_resource_handle->mapper_cfg[i].mapper_handle_ptr = (digi_mapper_chnl_t *)&(digi_handle->handle_pool.mapper_chnl[i]);
        }
        else
        {
            digi_resource_handle->mapper_cfg[i].mapper_handle_ptr = NULL;
        }
    }

    for (i=0; i < DIGI_ILKN_INTF_NUM; i++)
    {
        UINT16 j;

        /*
         * Copy over the interlaken instance as set in the digi_handle.
         */
        digi_resource_handle->ilkn_inst_cfg[i] = &(digi_handle->handle_pool.ilkn_link[i]);
        
        for (j=0; j < DIGI_ILKN_CHNL_NUM; j++)
        {
            digi_ilkn_chnl_def_t* ilkn_chnl_ptr;
            
            if (0 == i)
            {
                ilkn_chnl_ptr = &(digi_handle->handle_pool.ilkn_1_chnl[j]);
            }
            else
            {
                ilkn_chnl_ptr = &(digi_handle->handle_pool.ilkn_2_chnl[j]);
            }

            if (UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG != ilkn_chnl_ptr->switch_data.header.prov_state)
            {
                digi_resource_handle->ilkn_chnl_cfg[i][j].ilkn_chnl_handle_ptr = (digi_ilkn_chnl_t *)ilkn_chnl_ptr;
                digi_resource_handle->ilkn_chnl_cfg[i][j].ilkn_chnl_desc.target = ilkn_chnl_ptr->target;
                digi_resource_handle->ilkn_chnl_cfg[i][j].ilkn_chnl_desc.tx_bcw_chnl = ilkn_chnl_ptr->tx_ilkn_info.ilkn_bcw_chnl;
                digi_resource_handle->ilkn_chnl_cfg[i][j].ilkn_chnl_desc.tx_2b_header = ilkn_chnl_ptr->tx_ilkn_info.ilkn_2b_header;
                digi_resource_handle->ilkn_chnl_cfg[i][j].ilkn_chnl_desc.tx_4b_header = ilkn_chnl_ptr->tx_ilkn_info.ilkn_4b_header;
                digi_resource_handle->ilkn_chnl_cfg[i][j].ilkn_chnl_desc.rx_bcw_chnl = ilkn_chnl_ptr->rx_ilkn_info.ilkn_bcw_chnl;
                digi_resource_handle->ilkn_chnl_cfg[i][j].ilkn_chnl_desc.rx_2b_header = ilkn_chnl_ptr->rx_ilkn_info.ilkn_2b_header;
                digi_resource_handle->ilkn_chnl_cfg[i][j].ilkn_chnl_desc.rx_4b_header = ilkn_chnl_ptr->rx_ilkn_info.ilkn_4b_header;
            }
            else
            {
                digi_resource_handle->ilkn_chnl_cfg[i][j].ilkn_chnl_handle_ptr = NULL;
            }
        }
    }
    
    /*
     * Iterate over the line ports. Use a flag to is set to true as 
     * long as one valid configuration is found. It is possible that 
     * there are more serdes ports that have been initialized than 
     * were configured.
     */
    /*
     * Iterate over the line side serdes ports to determine which if any
     * are configured and in use.
     */
    for (i=0; i < DIGI_NUM_LINE_PORTS_MAX; i++)
    {
        /* Retrieve serdes status */
        PMC_ERROR result = digi_serdes_port_rate_prov_status_internal_get(digi_handle,
                                                                          (digi_serdes_port_t *)&digi_handle->var.port_ctxt.line_port_ctxt[i],
                                                                          TRUE,
                                                                          &serdes_prov_status,
                                                                          &serdes_port_mode,
                                                                          &serdes_prov_state);
        
        if (PMC_SUCCESS == result &&
            DIGI_SERDES_UNASSIGNED != digi_handle->var.port_ctxt.line_port_ctxt[i].port_uid && 
            DIGI_SERDES_PROV_STATE_NO_SERDES != serdes_prov_state)
        {
            if (DIGI_SERDES_PROV_STATE_UNASSIGNED != serdes_prov_state)
            {
                /*
                 * For each unassigned port identifier determine what it is
                 * connected to.
                 */
                result = digi_resource_handles_get(digi_handle, i, 
                                                   DIGI_SERDES_LINE_INTF,
                                                   digi_resource_handle,
                                                   &last_index,
                                                   &last_handle);
                
                if (PMC_SUCCESS == result)
                {
                    valid_result = TRUE;
                }
            }
            else
            {
                digi_resource_handle->line_serdes_cfg[i].port_ctxt_ptr = (digi_serdes_port_t *)&(digi_handle->var.port_ctxt.line_port_ctxt[i]);
                digi_resource_handle->line_serdes_cfg[i].port_uid = digi_handle->var.port_ctxt.line_port_ctxt[i].port_uid;
            }
        }
    }
    /*
     * Determine the connectivity of the SYSTEM ports. Reset the 
     * last_handle to ensure that associate handles are set only between
     * associate connections per interface.
     */
    last_handle = LAST_DIGI_RESOURCE_HANDLE;

    for (i=0; i < DIGI_NUM_SYS_PORTS_MAX; i++)
    {
        /* Retrieve serdes status */
        PMC_ERROR result = digi_serdes_port_rate_prov_status_internal_get(digi_handle,
                                                                          (digi_serdes_port_t *)&digi_handle->var.port_ctxt.sys_port_ctxt[i],
                                                                          TRUE,
                                                                          &serdes_prov_status,
                                                                          &serdes_port_mode,
                                                                          &serdes_prov_state);
        
        if (PMC_SUCCESS == result &&
            DIGI_SERDES_UNASSIGNED != digi_handle->var.port_ctxt.sys_port_ctxt[i].port_uid && 
            DIGI_SERDES_PROV_STATE_NO_SERDES != serdes_prov_state)
        {
            if (DIGI_SERDES_PROV_STATE_UNASSIGNED != serdes_prov_state)
            {
                result = digi_resource_handles_get(digi_handle, i, 
                                                   DIGI_SERDES_SYSTEM_INTF,
                                                   digi_resource_handle,
                                                   &last_index,
                                                   &last_handle);
                if (PMC_SUCCESS == result)
                {
                    valid_result = TRUE;
                }
            }
            else
            {
                digi_resource_handle->sys_serdes_cfg[i].port_ctxt_ptr = (digi_serdes_port_t *)&(digi_handle->var.port_ctxt.sys_port_ctxt[i]);
                digi_resource_handle->sys_serdes_cfg[i].port_uid = digi_handle->var.port_ctxt.sys_port_ctxt[i].port_uid;
            }
        }
    }
    /*
     * Determine the connectivity of the SFI51 ports. Reset the 
     * last_handle to ensure that associate handles are set only between
     * associate connections per interface.
     */
    last_handle = LAST_DIGI_RESOURCE_HANDLE;

    for (i=0; i < DIGI_NUM_SFI51_PORTS_MAX; i++)
    {
        /* Retrieve serdes status */
        PMC_ERROR result = digi_serdes_port_rate_prov_status_internal_get(digi_handle,
                                                                          (digi_serdes_port_t *)&digi_handle->var.port_ctxt.sfi51_port_ctxt[i],
                                                                          TRUE,
                                                                          &serdes_prov_status,
                                                                          &serdes_port_mode,
                                                                          &serdes_prov_state);

        if (PMC_SUCCESS == result &&
            DIGI_SERDES_UNASSIGNED != digi_handle->var.port_ctxt.sfi51_port_ctxt[i].port_uid && 
            DIGI_SERDES_PROV_STATE_NO_SERDES != serdes_prov_state)
        {
            if (DIGI_SERDES_PROV_STATE_UNASSIGNED != serdes_prov_state)
            {
                result = digi_resource_handles_get(digi_handle, i, 
                                                   DIGI_SERDES_SFI_51_INTF,
                                                   digi_resource_handle,
                                                   &last_index,
                                                   &last_handle);
            
                if (PMC_SUCCESS == result)
                {
                    valid_result = TRUE;
                }
            }
            else
            {
                digi_resource_handle->sfi51_serdes_cfg[i].port_ctxt_ptr = (digi_serdes_port_t *)&(digi_handle->var.port_ctxt.sfi51_port_ctxt[i]);
                digi_resource_handle->sfi51_serdes_cfg[i].port_uid = digi_handle->var.port_ctxt.sfi51_port_ctxt[i].port_uid;
            }
        }
    }

    /*
     * If this is a ports application then  set the valid flag to true.
     */
    if (DIGI_PROD_APP_PORTLESS_CARD == digi_handle->var.prod_app)
    {
        valid_result = TRUE;
    }


    /*
     * Now set up the mapper connectivity model if there is at least one
     * valid resource in use.
     */
    for (i=0; i < DIGI_MAPOTN_CHNL_MAX; i++)
    {
        if (NULL != digi_resource_handle->mapper_cfg[i].mapper_handle_ptr)
        {
            digi_resource_handle->mapper_cfg[i].cpb_switch_index = digi_get_index_and_set_cpb_data(digi_handle, i, DIGI_OTN_MAPPER, digi_resource_handle);
            digi_resource_handle->mapper_cfg[i].oduk_switch_index = digi_get_index_and_set_oduk_data(digi_handle, i, DIGI_OTN_MAPPER, digi_resource_handle);
        }
        else
        {
            digi_resource_handle->mapper_cfg[i].cpb_switch_index = INDEX_UNDEFINED;
            digi_resource_handle->mapper_cfg[i].oduk_switch_index = INDEX_UNDEFINED;
        }
    }

    /*
     * Determine the ILKN 1 and 2 connectivity
     */
    for (i=0; i < DIGI_ILKN_INTF_NUM; i++)
    {
        UINT16 j;
        
        for (j=0; j < DIGI_ILKN_CHNL_NUM; j++)
        {
            digi_ilkn_chnl_def_t* ilkn_chnl_ptr = (digi_ilkn_chnl_def_t*)digi_resource_handle->ilkn_chnl_cfg[i][j].ilkn_chnl_handle_ptr;
            /*
             * Check the provisioning state of the ILKN data. ILKN 
             * channel handles are maintained within digi resource 
             * table to ensure they can be recovered in the case of 
             * restart scenarios. However the resource may not actually
             *  be in use so need to check that has actually being 
             * provisioned first.
             */
            if (NULL != ilkn_chnl_ptr && 
                (TRUE == util_global_rx_or_tx_chnl_prov_status_get(ilkn_chnl_ptr->switch_data.header.prov_state)))
            {
                digi_resource_next_handle_t handle_id = DIGI_ILKN_1;
                
                if (1 == i)
                {
                    handle_id = DIGI_ILKN_2;
                }
                
                if (DIGI_ILKN_CHNL_TARGET_CPB == ilkn_chnl_ptr->target)
                {
                    digi_resource_handle->ilkn_chnl_cfg[i][j].next_resource_handle = DIGI_CPB_SWITCH;
                    digi_resource_handle->ilkn_chnl_cfg[i][j].next_handle_index = digi_get_index_and_set_cpb_data(digi_handle, j, handle_id, digi_resource_handle);
                }
                else if (DIGI_ILKN_CHNL_TARGET_ODUK == ilkn_chnl_ptr->target)
                {
                    digi_resource_handle->ilkn_chnl_cfg[i][j].next_resource_handle = DIGI_ODUK_SWITCH;
                    digi_resource_handle->ilkn_chnl_cfg[i][j].next_handle_index = digi_get_index_and_set_oduk_data(digi_handle, j, handle_id, digi_resource_handle);
                }
            }
            else
            {
                digi_resource_handle->ilkn_chnl_cfg[i][j].next_resource_handle = LAST_DIGI_RESOURCE_HANDLE;
                digi_resource_handle->ilkn_chnl_cfg[i][j].next_handle_index = 0; 
            }
        }
    }
    /*
     * If operations have been good to here and this is not a portless 
     * card then set up the switch information for ODUK connected enties.
     * If portless there is nothing more to do.
     */
    if (TRUE == valid_result && DIGI_PROD_APP_PORTLESS_CARD != digi_handle->var.prod_app)
    {
        digi_resource_connect_switch_data(digi_handle, digi_resource_handle);
    }

    PMC_ATOMIC_RETURN(digi_handle, PMC_SUCCESS);
} /* digi_resource_state_get */

#ifndef DOXYGEN_PUBLIC_ONLY

/*******************************************************************************
* digi_resource_clean_switch
* ______________________________________________________________________________
** DESCRIPTION:
*  Called after resource table has been cleaned in a restart. Iterates over
*  resource table removing any entry found that is not connected to anything
*  Additionally 
*  
*
* INPUTS:
*  *digi_handle             - pointer to DIGI handle instance
*  digi_resource_handle_ptr - Reference to the resource table.
*
* OUTPUTS:
    None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_resource_clean_switch(digi_handle_t *digi_handle,
                                       digi_resource_handle_summary_t *digi_resource_handle_ptr)
{
    UINT32 i = 0;

    UINT32 enet_line_chnl_array[DIGI_ENET_LINE_CHNL_MAX];
    UINT32 enet_index = 0;
    UINT32 chnl_mask[3] = {0, 0, 0};
    BOOL8 unprov = FALSE;
    BOOL8 prov_unconnected = FALSE;
    BOOL8 has_no_activate_dest = FALSE;
    BOOL8 bFound = FALSE;
    BOOL8 bFoundInput = FALSE;
    BOOL8 enh_pmon = FALSE;
    UINT32 input_chnl_mask[3] = {0, 0, 0};
    digi_serdes_port_t *port_ctxt_ptr = NULL;
    digi_otn_odu_chnl_t    *unchannelized_chnl_ptr;
    digi_otn_server_chnl_t *otu_otn_server_ptr;
    enet_data_rate_t enet_data_rate;

    PMC_ENTRY();

    PMC_ASSERT(NULL != digi_resource_handle_ptr, DIGI_ERR_NULL_HANDLE, 0, 
               0);
    /*
     * If MAPOTN resources are to be deprovisioned it will be deprovisioned
     * in digi_resource_clean_cpb. Thus the order here is important and
     * the digi_resource_clean_oduk should be called before 
     * digi_resource_clean_cpb.
     */
    digi_resource_clean_oduk(digi_handle,
                             digi_resource_handle_ptr);

    digi_resource_clean_cpb(digi_handle,
                            digi_resource_handle_ptr);

    digi_resource_clean_mapotn(digi_handle,
                               digi_resource_handle_ptr);
    /*
     * At this point the digi_resource table should be clean and there 
     * should be at least one valid datapath.  Now iterate over the 
     * digi_handle subsystems for each unused channel within that subsystem
     * and ensure there are no MPMO resources in use. 
     * Algorithm:
     * 1. Iterate over any used mapotn channel handle in the resource table
     *    and query that handle.
     * 2. Iterate over any ILKN unused channel handle.
     * 3. At the coreotn determine if any ODU level channel handle is
     *    connected to 
     */
    /* MAPOTN DCPB */
    for (i = 0; i < DIGI_MAPOTN_CHNL_MAX; i++)
    {
        unprov = (TRUE != util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.mapper_chnl[i].switch_data.header.prov_state));
        prov_unconnected =  (FALSE == unprov && 
                             digi_handle->handle_pool.mapper_chnl[i].switch_data.cpb_data.op_state.active_state != UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE);
        has_no_activate_dest = (FALSE == unprov && 
                                digi_handle->handle_pool.mapper_chnl[i].switch_data.cpb_data.op_state.map_state.dest_active_count == 0);

        if (TRUE == unprov || TRUE == prov_unconnected)            
        {
            digi_opsa_chnl_to_chnl_msk(i, chnl_mask);
            bFound = TRUE;
        }
        if (TRUE == unprov || TRUE == has_no_activate_dest) 
        {
            bFoundInput = TRUE;
            digi_opsa_chnl_to_chnl_msk(i, input_chnl_mask);
        }
        else if ((TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.mapper_chnl[i].switch_data.header.prov_state)) && 
                 UTIL_GLOBAL_CPB_PORT_ENET_LINE == digi_handle->handle_pool.mapper_chnl[i].switch_data.cpb_data.port_type)
        {
            /*
             * If this is the ETRANS case then the cpb port type will be
             * ENET_LINE. If it is then capture the channel value. Want to
             * ensure that this channel is skipped when cleaning the ENET
             * line channels.
             */
            PMC_ASSERT(enet_index < DIGI_ENET_LINE_CHNL_MAX, DIGI_ERR_INVALID_ARG, 0, 0);
            enet_line_chnl_array[enet_index++] = digi_handle->handle_pool.mapper_chnl[i].switch_data.cpb_data.channel;
        }
    }
    /*
     * Now check the flag to determine if there is anything to release
     * with respect to MAPOTN
     */
    if (TRUE == bFound)
    {
        digi_opsa_cpb_port_release_cfg(digi_handle,
                                       UTIL_GLOBAL_CPB_PORT_MAPOTN,
                                       chnl_mask,
                                       DIGI_OPSA_DIR_OUTBOUND);
        bFound = FALSE;
        chnl_mask[0] = 0;
        chnl_mask[1] = 0;
        chnl_mask[2] = 0;
    }  
    if (TRUE == bFoundInput)
    {
        digi_opsa_cpb_port_release_cfg(digi_handle,
                                       UTIL_GLOBAL_CPB_PORT_MAPOTN,
                                       input_chnl_mask,
                                       DIGI_OPSA_DIR_INBOUND);
        bFoundInput = FALSE;
        input_chnl_mask[0] = 0;
        input_chnl_mask[1] = 0;
        input_chnl_mask[2] = 0;
    } 
    /*
     * Now iterate over the ENET_LINE and determine which of those are in
     * use and any that are not are not being used as ETRANS case.
     */
    for (i = 0; i < DIGI_ENET_LINE_CHNL_MAX; i++)
    {
        UINT8 index;
        BOOL8 is_etrans = FALSE;
        unprov = (TRUE != util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.enet_line_chnl[i].switch_data.header.prov_state));
        prov_unconnected =  (FALSE == unprov && 
                             digi_handle->handle_pool.enet_line_chnl[i].switch_data.cpb_data.op_state.active_state != UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE);
        has_no_activate_dest = (FALSE == unprov &&
                                digi_handle->handle_pool.enet_line_chnl[i].switch_data.cpb_data.op_state.map_state.dest_active_count == 0);
        enh_pmon =            enet_enhanced_pmon_prov_get(digi_handle->enet_line_handle,i,FALSE,&enet_data_rate);

        for (index=0; index < enet_index; index++)
        {
            if (i == enet_line_chnl_array[index])
            {
                is_etrans = TRUE;
                break;
            }
        }
        if (FALSE == enh_pmon) 
        {
            if  (FALSE == is_etrans && (TRUE == unprov  || TRUE == prov_unconnected))
            {          
                digi_opsa_chnl_to_chnl_msk(i, chnl_mask);
                bFound = TRUE;
                /* mark associated SERDES for cleanup */
                if (prov_unconnected == TRUE)
                {
                    (void) digi_port_ctxt_from_channel_number_get(digi_handle,
                                                                  DIGI_SERDES_PROV_STATE_ENET_LINE,
                                                                  i,
                                                                  &port_ctxt_ptr);
                    (void) digi_serdes_port_rate_state_set (digi_handle,
                                                            port_ctxt_ptr,
                                                            LAST_DIGI_SERDES_PORT_MODE,
                                                            LAST_DIGI_SERDES_PROV_STATE,
                                                            DIGI_SERDES_PORT_RATE_STATE_RESTART_PENDING);
                }                        
            }
            if (FALSE == is_etrans && (TRUE == unprov  || TRUE == has_no_activate_dest ))
            {
                bFoundInput = TRUE;
                digi_opsa_chnl_to_chnl_msk(i, input_chnl_mask);
            }
        }
    }
    if (TRUE == bFound)
    {
        digi_opsa_cpb_port_release_cfg(digi_handle,
                                       UTIL_GLOBAL_CPB_PORT_ENET_LINE,
                                       chnl_mask,
                                       DIGI_OPSA_DIR_OUTBOUND);
        bFound = FALSE;
        chnl_mask[0] = 0;
        chnl_mask[1] = 0;
        chnl_mask[2] = 0;
    }
    if (TRUE == bFoundInput)
    {
        digi_opsa_cpb_port_release_cfg(digi_handle,
                                       UTIL_GLOBAL_CPB_PORT_ENET_LINE,
                                       input_chnl_mask,
                                       DIGI_OPSA_DIR_INBOUND);
        bFoundInput = FALSE;
        input_chnl_mask[0] = 0;
        input_chnl_mask[1] = 0;
        input_chnl_mask[2] = 0;
    }

    for (i = 0; i < DIGI_ENET_SYS_CHNL_MAX; i++)
    {

        unprov = (TRUE != util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.enet_sys_chnl[i].switch_data.header.prov_state));
        prov_unconnected =  (FALSE == unprov &&
                             digi_handle->handle_pool.enet_sys_chnl[i].switch_data.cpb_data.op_state.active_state != UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE);
        has_no_activate_dest = (FALSE == unprov &&
                                digi_handle->handle_pool.enet_sys_chnl[i].switch_data.cpb_data.op_state.map_state.dest_active_count == 0);
        enh_pmon =            enet_enhanced_pmon_prov_get(digi_handle->enet_sys_handle,i,FALSE,&enet_data_rate);
        if (FALSE == enh_pmon) 
        {
            if (TRUE == unprov  || TRUE == prov_unconnected)
            {
                digi_opsa_chnl_to_chnl_msk(i, chnl_mask);
                bFound = TRUE;
                /* mark associated SERDES for cleanup */
                if (prov_unconnected == TRUE)
                {
                    (void) digi_port_ctxt_from_channel_number_get(digi_handle,
                                                                  DIGI_SERDES_PROV_STATE_ENET_SYS,
                                                                  i,
                                                                  &port_ctxt_ptr);
                    (void) digi_serdes_port_rate_state_set (digi_handle,
                                                            port_ctxt_ptr,
                                                            LAST_DIGI_SERDES_PORT_MODE,
                                                            LAST_DIGI_SERDES_PROV_STATE,
                                                            DIGI_SERDES_PORT_RATE_STATE_RESTART_PENDING);
                }
            }
            if (TRUE == unprov  || TRUE == has_no_activate_dest )
            {
                bFoundInput = TRUE;
                digi_opsa_chnl_to_chnl_msk(i, input_chnl_mask);
            }
        }
    }

    if (TRUE == bFound)
    {
        digi_opsa_cpb_port_release_cfg(digi_handle,
                                       UTIL_GLOBAL_CPB_PORT_ENET_SYS,
                                       chnl_mask,
                                       DIGI_OPSA_DIR_OUTBOUND);
        bFound = FALSE;
        chnl_mask[0] = 0;
        chnl_mask[1] = 0;
        chnl_mask[2] = 0;
    }
    if (TRUE == bFoundInput)
    {
        digi_opsa_cpb_port_release_cfg(digi_handle,
                                       UTIL_GLOBAL_CPB_PORT_ENET_SYS,
                                       input_chnl_mask,
                                       DIGI_OPSA_DIR_INBOUND);
        bFoundInput = FALSE;
        input_chnl_mask[0] = 0;
        input_chnl_mask[1] = 0;
        input_chnl_mask[2] = 0;
    }

    for (i = 0; i < DIGI_CBRC_CHNL_MAX; i++)
    {
        /* test to determine if this CBRC channel is being used as BACKUP for ENET_LINE or ENET_SYS, if so, retain the provisioned
           state */
        UINT8 dest_mst_port = g_cpb_connect_data_ptr->chnl[UTIL_GLOBAL_CPB_PORT_CBRC][i].slv.dest_mst_port;
        UINT8 dest_mst_chnl = g_cpb_connect_data_ptr->chnl[UTIL_GLOBAL_CPB_PORT_CBRC][i].slv.dest_mst_chnl;
        UINT8 source_slv_port = CPB_INDEX_LAST;
        UINT8 source_slv_chnl = CPB_INDEX_LAST;
        BOOL8 cbrc_w_no_cbrc_handle = FALSE;

        if(dest_mst_port != CPB_INDEX_LAST && dest_mst_chnl != CPB_INDEX_LAST)
        {
            source_slv_port = g_cpb_connect_data_ptr->chnl[dest_mst_port][dest_mst_chnl].mst.source_slv_port;
            source_slv_chnl = g_cpb_connect_data_ptr->chnl[dest_mst_port][dest_mst_chnl].mst.source_slv_chnl;
        }

        if((util_global_cpb_port_t)source_slv_port == UTIL_GLOBAL_CPB_PORT_ENET_LINE || (util_global_cpb_port_t)source_slv_port == UTIL_GLOBAL_CPB_PORT_ENET_SYS)
        {
            if((g_cpb_connect_data_ptr->chnl[UTIL_GLOBAL_CPB_PORT_CBRC][i].slv.state == UTIL_GLOBAL_CHNL_OPERATIONAL && 
                g_cpb_connect_data_ptr->chnl[source_slv_port][source_slv_chnl].slv.state == UTIL_GLOBAL_CHNL_BACKUP) ||
               (g_cpb_connect_data_ptr->chnl[UTIL_GLOBAL_CPB_PORT_CBRC][i].slv.state == UTIL_GLOBAL_CHNL_BACKUP && 
                g_cpb_connect_data_ptr->chnl[source_slv_port][source_slv_chnl].slv.state == UTIL_GLOBAL_CHNL_OPERATIONAL))
            {
                cbrc_w_no_cbrc_handle = TRUE;
            }
        }

        if(cbrc_w_no_cbrc_handle == FALSE)
        {
            unprov = (TRUE != util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.cbr_chnl[i].switch_data.header.prov_state));
    
            prov_unconnected =  (FALSE == unprov &&
                                 digi_handle->handle_pool.cbr_chnl[i].switch_data.cpb_data.op_state.active_state != UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE);
            has_no_activate_dest = (FALSE == unprov &&
                                    digi_handle->handle_pool.cbr_chnl[i].switch_data.cpb_data.op_state.map_state.dest_active_count == 0);
        }
        else
        {
            unprov = FALSE;
            prov_unconnected = FALSE;
            has_no_activate_dest = FALSE;
        }

        if (TRUE == unprov  || TRUE == prov_unconnected)
        {
            digi_opsa_chnl_to_chnl_msk(i, chnl_mask);
            bFound = TRUE;
            /* mark associated SERDES for cleanup */
            if (prov_unconnected == TRUE)
            {
                (void) digi_port_ctxt_from_channel_number_get(digi_handle,
                                                              DIGI_SERDES_PROV_STATE_CBRC,
                                                              i,
                                                              &port_ctxt_ptr);
                (void) digi_serdes_port_rate_state_set (digi_handle,
                                                        port_ctxt_ptr,
                                                        LAST_DIGI_SERDES_PORT_MODE,
                                                        LAST_DIGI_SERDES_PROV_STATE,
                                                        DIGI_SERDES_PORT_RATE_STATE_RESTART_PENDING);
            }
        }
        if (TRUE == unprov  || TRUE == has_no_activate_dest )
        {
            bFoundInput = TRUE;
            digi_opsa_chnl_to_chnl_msk(i, input_chnl_mask);
        }
    }
    if (TRUE == bFound)
    {
        digi_opsa_cpb_port_release_cfg(digi_handle,
                                       UTIL_GLOBAL_CPB_PORT_CBRC,
                                       chnl_mask,
                                       DIGI_OPSA_DIR_OUTBOUND);
        bFound = FALSE;
        chnl_mask[0] = 0;
        chnl_mask[1] = 0;
        chnl_mask[2] = 0;
    }

    if (TRUE == bFoundInput)
    {
        digi_opsa_cpb_port_release_cfg(digi_handle,
                                       UTIL_GLOBAL_CPB_PORT_CBRC,
                                       input_chnl_mask,
                                       DIGI_OPSA_DIR_INBOUND);
        bFoundInput = FALSE;
        input_chnl_mask[0] = 0;
        input_chnl_mask[1] = 0;
        input_chnl_mask[2] = 0;
    }

    /* ILKN1 */
    for (i = 0; i < 96; i++)
    {
        unprov = ((FALSE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.ilkn_1_chnl[i].switch_data.header.prov_state)));
        prov_unconnected =  (FALSE == unprov &&
                             digi_handle->handle_pool.ilkn_1_chnl[i].switch_data.cpb_data.op_state.active_state != UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE);
        has_no_activate_dest = (FALSE == unprov &&
                                digi_handle->handle_pool.ilkn_1_chnl[i].switch_data.cpb_data.op_state.map_state.dest_active_count == 0);

        if (TRUE == unprov  || TRUE == prov_unconnected)
        {
            digi_opsa_chnl_to_chnl_msk(i, chnl_mask);
            bFound = TRUE;
            /* mark associated SERDES for cleanup */          
        }
        if (TRUE == unprov  || TRUE == has_no_activate_dest )
        {
            bFoundInput = TRUE;
            digi_opsa_chnl_to_chnl_msk(i, input_chnl_mask);
        }
    }
    if (TRUE == bFound)
    {
        digi_opsa_cpb_port_release_cfg(digi_handle,
                                       UTIL_GLOBAL_CPB_PORT_ILKN1,
                                       chnl_mask,
                                       DIGI_OPSA_DIR_OUTBOUND);
        bFound = FALSE;
        chnl_mask[0] = 0;
        chnl_mask[1] = 0;
        chnl_mask[2] = 0;
    }

    if (TRUE == bFoundInput)
    {
        digi_opsa_cpb_port_release_cfg(digi_handle,
                                       UTIL_GLOBAL_CPB_PORT_ILKN1,
                                       input_chnl_mask,
                                       DIGI_OPSA_DIR_INBOUND);
        bFoundInput = FALSE;
        input_chnl_mask[0] = 0;
        input_chnl_mask[1] = 0;
        input_chnl_mask[2] = 0;
    }
    /* ILKN2 */
    for (i = 0; i < 96; i++)
    {
        unprov = ((FALSE  == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.ilkn_2_chnl[i].switch_data.header.prov_state)));
        prov_unconnected = (FALSE == unprov &&
                            digi_handle->handle_pool.ilkn_2_chnl[i].switch_data.cpb_data.op_state.active_state != UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE);
        has_no_activate_dest = (FALSE == unprov &&
                                digi_handle->handle_pool.ilkn_2_chnl[i].switch_data.cpb_data.op_state.map_state.dest_active_count == 0);

        if (TRUE == unprov  || TRUE == prov_unconnected)
        {
            digi_opsa_chnl_to_chnl_msk(i, chnl_mask);
            bFound = TRUE;
            /* mark associated SERDES for cleanup */          
        }
        if (TRUE == unprov  || TRUE == has_no_activate_dest )
        {
            bFoundInput = TRUE;
            digi_opsa_chnl_to_chnl_msk(i, input_chnl_mask);
        }
    }
    if (TRUE == bFound)
    {
        digi_opsa_cpb_port_release_cfg(digi_handle,
                                       UTIL_GLOBAL_CPB_PORT_ILKN2,
                                       chnl_mask,
                                       DIGI_OPSA_DIR_OUTBOUND);
        bFound = FALSE;
        chnl_mask[0] = 0;
        chnl_mask[1] = 0;
        chnl_mask[2] = 0;
    }

    if (TRUE == bFoundInput)
    {
        digi_opsa_cpb_port_release_cfg(digi_handle,
                                       UTIL_GLOBAL_CPB_PORT_ILKN2,
                                       input_chnl_mask,
                                       DIGI_OPSA_DIR_INBOUND);
        bFoundInput = FALSE;
        input_chnl_mask[0] = 0;
        input_chnl_mask[1] = 0;
        input_chnl_mask[2] = 0;
    }

    /* check channel connected to  UTIL_GLOBAL_ODUK_PORT_MAPOTN interface */       
    /* 
       MAPOTN ODUKSW 
    */
    for (i = 0; i < DIGI_MAPOTN_CHNL_MAX; i++)
    {
        unprov = (TRUE != util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.mapper_chnl[i].switch_data.header.prov_state));
        prov_unconnected =  (FALSE == unprov && 
                             digi_handle->handle_pool.mapper_chnl[i].switch_data.oduk_data.op_state.active_state != UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE);
        has_no_activate_dest = (FALSE == unprov &&
                                digi_handle->handle_pool.mapper_chnl[i].switch_data.oduk_data.op_state.map_state.dest_active_count == 0);

        if (TRUE == unprov  || TRUE == prov_unconnected)
        {
            digi_opsa_chnl_to_chnl_msk(i, chnl_mask);
            bFound = TRUE;
        }
        if (TRUE == unprov  || TRUE == has_no_activate_dest )
        {
            bFoundInput = TRUE;
            digi_opsa_chnl_to_chnl_msk(i, input_chnl_mask);
        }
    }
    /*
     * Now check the flag to determine if there is anything to release
     * with respect to MAPOTN
     */
    if (TRUE == bFound)
    {
        digi_opsa_oduksw_port_release_cfg(digi_handle,
                                          UTIL_GLOBAL_ODUK_PORT_MAPOTN,
                                          chnl_mask,
                                          DIGI_OPSA_DIR_OUTBOUND);

        bFound = FALSE;
        chnl_mask[0] = 0;
        chnl_mask[1] = 0;
        chnl_mask[2] = 0;
    }
    if (TRUE == bFoundInput)
    {
        digi_opsa_oduksw_port_release_cfg(digi_handle,
                                          UTIL_GLOBAL_ODUK_PORT_MAPOTN,
                                          input_chnl_mask,
                                          DIGI_OPSA_DIR_INBOUND);

        bFoundInput = FALSE;
        input_chnl_mask[0] = 0;
        input_chnl_mask[1] = 0;
        input_chnl_mask[2] = 0;
    }
    /* check channel connected to  UTIL_GLOBAL_ODUK_PORT_COREOTN interface */       
    for (i = 0; i < DIGI_OTN_SERVER_LO_CHNL_MAX; i++)
    {
        unchannelized_chnl_ptr = NULL;
        otu_otn_server_ptr     = NULL;

        /*
         * Iterate over the HO/MO/LO order channels and add only those 
         * channels that are provision and connected at the F01 stage,
         * namely unchannelized. The mask is then inverted such that the
         * reverse channels are released.
         */
        if (i < DIGI_OTN_SERVER_HO_CHNL_MAX)
        {
            if (TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.ho_odu_struct[i].switch_data.header.prov_state) && 
                digi_handle->handle_pool.ho_odu_chnl[i].payload_format == UTIL_GLOBAL_ODU_UNCHANNELIZED)
            {
                if (digi_handle->handle_pool.ho_odu_struct[i].switch_data.oduk_data.op_state.active_state == UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE)
                {               
                    digi_opsa_chnl_to_chnl_msk(i, chnl_mask);                    
                } else 
                {
                    bFound = TRUE;
                }
                if (digi_handle->handle_pool.ho_odu_struct[i].switch_data.oduk_data.op_state.map_state.dest_active_count != 0)
                {
                      digi_opsa_chnl_to_chnl_msk(i, input_chnl_mask);
                } else 
                {
                    bFoundInput = TRUE;
                }
                unchannelized_chnl_ptr = (digi_otn_odu_chnl_t *) &digi_handle->handle_pool.ho_odu_chnl[i];
                if (digi_handle->var.prod_app == DIGI_PROD_APP_SYSOTN_CARD)
                {
                    otu_otn_server_ptr = ( digi_otn_server_chnl_t *) &digi_handle->handle_pool.sysotn_server_chnl;
                } else 
                {
                    otu_otn_server_ptr = ( digi_otn_server_chnl_t *) &digi_handle->handle_pool.otn_server_chnl[i];
                }
                if (digi_handle->handle_pool.ho_odu_struct[i].switch_data.oduk_data.op_state.active_state  != UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE)
                {
                    if (digi_handle->var.prod_app == DIGI_PROD_APP_SYSOTN_CARD)
                    {
                        port_ctxt_ptr = digi_port_ctxt_no_type_find(digi_handle,digi_handle->handle_pool.sysotn_server_chnl.port_uid);                        
                    } else 
                    {
                        port_ctxt_ptr = digi_port_ctxt_no_type_find(digi_handle,digi_handle->handle_pool.otn_server_chnl[i].port_uid);                        
                    }                        
                    (void) digi_serdes_port_rate_state_set (digi_handle,
                                                            port_ctxt_ptr,
                                                            LAST_DIGI_SERDES_PORT_MODE,
                                                            LAST_DIGI_SERDES_PROV_STATE,
                                                            DIGI_SERDES_PORT_RATE_STATE_RESTART_PENDING);          
                    
                }
            } else 
            {
                bFound = TRUE;
                bFoundInput = TRUE;
            }
        }
        if (i < DIGI_OTN_SERVER_MO_CHNL_MAX)
        {
            if ((TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.mo_odu_struct[i].switch_data.header.prov_state)) && 
                digi_handle->handle_pool.mo_odu_chnl[i].payload_format == UTIL_GLOBAL_ODU_UNCHANNELIZED)
            {
                if (digi_handle->handle_pool.mo_odu_struct[i].switch_data.oduk_data.op_state.active_state == UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE)
                { 
                    digi_opsa_chnl_to_chnl_msk(i, chnl_mask);
                } else 
                {
                    bFound = TRUE;
                }
             
                if (digi_handle->handle_pool.mo_odu_struct[i].switch_data.oduk_data.op_state.map_state.dest_active_count != 0)
                {
                      digi_opsa_chnl_to_chnl_msk(i, input_chnl_mask);             
                }else 
                {
                    bFoundInput = TRUE;
                }
                unchannelized_chnl_ptr = (digi_otn_odu_chnl_t *) &digi_handle->handle_pool.mo_odu_chnl[i];
            } else 
            {
                bFound = TRUE;
                bFoundInput = TRUE;
            }
        }
        if (digi_handle->var.prod_app != DIGI_PROD_APP_SYSOTN_CARD)
        { 
            if (TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.lo_odu_struct[i].switch_data.header.prov_state) && 
                digi_handle->handle_pool.lo_odu_chnl[i].payload_format == UTIL_GLOBAL_ODU_UNCHANNELIZED)
            {
                if (digi_handle->handle_pool.lo_odu_struct[i].switch_data.oduk_data.op_state.active_state == UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE)
                { 
                    digi_opsa_chnl_to_chnl_msk(i, chnl_mask);
                } else 
                {
                    bFound = TRUE;
                }
                if (digi_handle->handle_pool.lo_odu_struct[i].switch_data.oduk_data.op_state.map_state.dest_active_count != 0)
                {
                      digi_opsa_chnl_to_chnl_msk(i, input_chnl_mask);                 
                } else 
                {
                    bFoundInput = TRUE;
                }
                unchannelized_chnl_ptr = (digi_otn_odu_chnl_t *) &digi_handle->handle_pool.lo_odu_chnl[i];
            } else 
            {
                bFound = TRUE;
                bFoundInput = TRUE;
            }
        }

    }

    if (TRUE == bFound)
    {
        /* invert mask */
        chnl_mask[0] = ~chnl_mask[0];
        chnl_mask[1] = ~chnl_mask[1];
        chnl_mask[2] = ~chnl_mask[2];

        digi_opsa_oduksw_port_release_cfg(digi_handle,
                                          UTIL_GLOBAL_ODUK_PORT_COREOTN,
                                          chnl_mask,
                                          DIGI_OPSA_DIR_OUTBOUND);
        bFound = FALSE;
        chnl_mask[0] = 0;
        chnl_mask[1] = 0;
        chnl_mask[2] = 0;
    }
    if (TRUE == bFoundInput)
    {
        /* invert mask */
        input_chnl_mask[0] = ~input_chnl_mask[0];
        input_chnl_mask[1] = ~input_chnl_mask[1];
        input_chnl_mask[2] = ~input_chnl_mask[2];

        digi_opsa_oduksw_port_release_cfg(digi_handle,
                                          UTIL_GLOBAL_ODUK_PORT_COREOTN,
                                          input_chnl_mask,
                                          DIGI_OPSA_DIR_INBOUND);
        bFoundInput = FALSE;
        input_chnl_mask[0] = 0;
        input_chnl_mask[1] = 0;
        input_chnl_mask[2] = 0;
    }
  
    /* check channel connected to  UTIL_GLOBAL_ODUK_PORT_ILKN1 interface */    
    /*
     * SYSOTN line side uses Low Order channel handles so if this is
     * a SYSOTN card then check those handles.
     */
     if (digi_handle->var.prod_app == DIGI_PROD_APP_SYSOTN_CARD)
     { 
         for (i = 0; i < DIGI_OTN_SERVER_HO_CHNL_MAX; i++)
         {

             if (TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.lo_odu_struct[i].switch_data.header.prov_state) && 
                 digi_handle->handle_pool.lo_odu_chnl[i].payload_format == UTIL_GLOBAL_ODU_UNCHANNELIZED)
             {
                 if (digi_handle->handle_pool.lo_odu_struct[i].switch_data.oduk_data.op_state.active_state == UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE)
                 {
                     digi_opsa_chnl_to_chnl_msk(i, chnl_mask);
                 } else 
                 {
                     bFound = TRUE;
                 }
                 if (digi_handle->handle_pool.lo_odu_struct[i].switch_data.oduk_data.op_state.map_state.dest_active_count != 0)
                 {
                     digi_opsa_chnl_to_chnl_msk(i, input_chnl_mask);
                 } else 
                 {
                     bFoundInput = TRUE;
                 }

             } else 
             {
                 bFound = TRUE;
                 bFoundInput = TRUE;
             }
         }

         if (TRUE == bFound)
         {
             /* invert mask */
             chnl_mask[0] = ~chnl_mask[0];
             chnl_mask[1] = ~chnl_mask[1];
             chnl_mask[2] = ~chnl_mask[2];
         
             digi_opsa_oduksw_port_release_cfg(digi_handle,
                                               UTIL_GLOBAL_ODUK_PORT_ILKN1,
                                               chnl_mask,
                                               DIGI_OPSA_DIR_OUTBOUND);
             bFound = FALSE;
             chnl_mask[0] = 0;
             chnl_mask[1] = 0;
             chnl_mask[2] = 0;
         }
         if (TRUE == bFoundInput)
         {
             /* invert mask */
             input_chnl_mask[0] = ~input_chnl_mask[0];
             input_chnl_mask[1] = ~input_chnl_mask[1];
             input_chnl_mask[2] = ~input_chnl_mask[2];

             digi_opsa_oduksw_port_release_cfg(digi_handle,
                                               UTIL_GLOBAL_ODUK_PORT_ILKN1,
                                               input_chnl_mask,
                                               DIGI_OPSA_DIR_INBOUND);
             bFoundInput = FALSE;
             input_chnl_mask[0] = 0;
             input_chnl_mask[1] = 0;
             input_chnl_mask[2] = 0;
         }
     } else 
     {
         /* ILKN1 */
         for (i = 0; i <  96; i++)
         {
             unprov = ((FALSE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.ilkn_1_chnl[DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX+i].switch_data.header.prov_state)));

             prov_unconnected =  (FALSE == unprov &&
                                  digi_handle->handle_pool.ilkn_1_chnl[DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX+i].switch_data.oduk_data.op_state.active_state != UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE);
             has_no_activate_dest = (FALSE == unprov &&
                                     digi_handle->handle_pool.ilkn_1_chnl[DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX+i].switch_data.oduk_data.op_state.map_state.dest_active_count == 0);
             
             if (TRUE == unprov  || TRUE == prov_unconnected)
             {
                 digi_opsa_chnl_to_chnl_msk(i, chnl_mask);
                 bFound = TRUE;
                 /* mark associated SERDES for cleanup */          
             }
             if (TRUE == unprov  || TRUE == has_no_activate_dest )
             {
                 bFoundInput = TRUE;
                 digi_opsa_chnl_to_chnl_msk(i, input_chnl_mask);
             }
         }
         if (TRUE == bFound)
         {
             digi_opsa_oduksw_port_release_cfg(digi_handle,
                                               UTIL_GLOBAL_ODUK_PORT_ILKN1,
                                               chnl_mask,
                                               DIGI_OPSA_DIR_OUTBOUND);
             bFound = FALSE;
             chnl_mask[0] = 0;
             chnl_mask[1] = 0;
             chnl_mask[2] = 0;
         }

         if (TRUE == bFoundInput)
         {
             digi_opsa_oduksw_port_release_cfg(digi_handle,
                                               UTIL_GLOBAL_ODUK_PORT_ILKN1,
                                               input_chnl_mask,
                                               DIGI_OPSA_DIR_INBOUND);
             bFoundInput = FALSE;
             input_chnl_mask[0] = 0;
             input_chnl_mask[1] = 0;
             input_chnl_mask[2] = 0;
         }
     }
     /* ILKN2 */    
     for (i = 0; i <  96; i++)
     {
             unprov = ((FALSE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.ilkn_2_chnl[DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX+i].switch_data.header.prov_state)));

         prov_unconnected =  (FALSE == unprov &&
                              digi_handle->handle_pool.ilkn_2_chnl[DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX+i].switch_data.oduk_data.op_state.active_state != UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE);
         has_no_activate_dest = (FALSE == unprov &&
                                 digi_handle->handle_pool.ilkn_2_chnl[DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX+i].switch_data.oduk_data.op_state.map_state.dest_active_count == 0);
             
         if (TRUE == unprov  || TRUE == prov_unconnected)
         {
             digi_opsa_chnl_to_chnl_msk(i, chnl_mask);
             bFound = TRUE;
             /* mark associated SERDES for cleanup */          
         }
         if (TRUE == unprov  || TRUE == has_no_activate_dest )
         {
             bFoundInput = TRUE;
             digi_opsa_chnl_to_chnl_msk(i, input_chnl_mask);
         }
     }
     if (TRUE == bFound)
     {
         digi_opsa_oduksw_port_release_cfg(digi_handle,
                                           UTIL_GLOBAL_ODUK_PORT_ILKN2,
                                           chnl_mask,
                                           DIGI_OPSA_DIR_OUTBOUND);
         bFound = FALSE;
         chnl_mask[0] = 0;
         chnl_mask[1] = 0;
         chnl_mask[2] = 0;
     }

     if (TRUE == bFoundInput)
     {
         digi_opsa_oduksw_port_release_cfg(digi_handle,
                                           UTIL_GLOBAL_ODUK_PORT_ILKN2,
                                           input_chnl_mask,
                                           DIGI_OPSA_DIR_INBOUND);
         bFoundInput = FALSE;
         input_chnl_mask[0] = 0;
         input_chnl_mask[1] = 0;
         input_chnl_mask[2] = 0;
     }

    /* 
       clean SIFD flow control calendar entries 
       digi_sifd_fc_calendar_update uses DCPB/OCPB context to validate flow control entries 
     */
    {
        /* ILKN 1*/
        if (digi_handle->var.sifd_ilkn_1_cfg.ilkn.fc_mode != DIGI_SIFD_FC_MODE_NONE)
        {                        
            /* DCPB */
            /* Rx entries */
            (void) digi_sifd_fc_calendar_update(digi_handle, 
                                                SIFD_COPI_ILKN1,
                                                (UINT32) UTIL_GLOBAL_CPB_PORT_ILKN1,
                                                FALSE, /* FALSE means RX data direction */
                                                FALSE,
                                                SIFD_ILKN_NULL_CAL_ENTRY);
            /* TX entries */
            (void) digi_sifd_fc_calendar_update(digi_handle, 
                                                SIFD_COPI_ILKN1,
                                                (UINT32) UTIL_GLOBAL_CPB_PORT_ILKN1,
                                                TRUE, /* FALSE means RX data direction */
                                                FALSE,
                                                SIFD_ILKN_NULL_CAL_ENTRY);
            /* ODUKSW */
            /* Rx entries */
            (void) digi_sifd_fc_calendar_update(digi_handle, 
                                                SIFD_COPI_ILKN1,
                                                (UINT32) UTIL_GLOBAL_CPB_PORT_ILKN1,
                                                FALSE, /* FALSE means RX data direction */
                                                TRUE,
                                                SIFD_ILKN_NULL_CAL_ENTRY);
            /* TX entries */
            (void) digi_sifd_fc_calendar_update(digi_handle, 
                                                SIFD_COPI_ILKN1,
                                                (UINT32) UTIL_GLOBAL_CPB_PORT_ILKN1,
                                                TRUE, /* FALSE means RX data direction */
                                                TRUE,
                                                SIFD_ILKN_NULL_CAL_ENTRY);
        }
        /* ILKN 2*/
        if (digi_handle->var.sifd_ilkn_2_cfg.ilkn.fc_mode != DIGI_SIFD_FC_MODE_NONE)
        {            
            /* DCPB */
            /* Rx entries */
            (void) digi_sifd_fc_calendar_update(digi_handle, 
                                                SIFD_COPI_ILKN2,
                                                (UINT32) UTIL_GLOBAL_CPB_PORT_ILKN2,
                                                FALSE, /* FALSE means RX data direction */
                                                FALSE,
                                                SIFD_ILKN_NULL_CAL_ENTRY);
            /* TX entries */
            (void) digi_sifd_fc_calendar_update(digi_handle, 
                                                SIFD_COPI_ILKN2,
                                                (UINT32) UTIL_GLOBAL_CPB_PORT_ILKN2,
                                                TRUE, /* FALSE means RX data direction */
                                                FALSE,
                                                SIFD_ILKN_NULL_CAL_ENTRY);
            /* ODUKSW */
            /* Rx entries */
            (void) digi_sifd_fc_calendar_update(digi_handle, 
                                                SIFD_COPI_ILKN2,
                                                (UINT32) UTIL_GLOBAL_CPB_PORT_ILKN2,
                                                FALSE, /* FALSE means RX data direction */
                                                TRUE,
                                                SIFD_ILKN_NULL_CAL_ENTRY);
            /* TX entries */
            (void) digi_sifd_fc_calendar_update(digi_handle, 
                                                SIFD_COPI_ILKN2,
                                                (UINT32) UTIL_GLOBAL_CPB_PORT_ILKN2,
                                                TRUE, /* FALSE means RX data direction */
                                                TRUE,
                                                SIFD_ILKN_NULL_CAL_ENTRY);
        }        
    }

    /* clean MCPB switch */
    digi_resource_clean_mcpb_switch(digi_handle);

    PMC_RETURN();
} /* digi_resource_clean_switch */

/*******************************************************************************
* digi_resource_clean_enet_enh_pmon
* ______________________________________________________________________________
** DESCRIPTION:
*  Called after resource table has been cleaned in a restart. Iterates over
*  resource table removing any entry found that is not connected to anything
*  
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*
* OUTPUTS:
    None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_resource_clean_enet_enh_pmon(digi_handle_t *digi_handle)
{
    PMC_ENTRY();
    cpb_connect_data_state_get(digi_handle->dcpb_handle,
                               &g_cpb_connect_data_ptr);

    /* 
       clear dangling enhanced PMON 
    */
    {
        UINT32 link_itr = 0;
        UINT32 source_slv_port;
        UINT32 source_slv_chnl;
        enet_data_rate_t enet_data_rate;
        digi_enet_mode_t enet_mode;
        
        /* regenerate pmon_line_usage */
        digi_handle->var.digi_enet_pmon_line_usage = 0;
        digi_handle->var.digi_enet_pmon_sys_usage  = 0;

        if (FALSE == enet_start_state_test(digi_handle->enet_line_handle))
        {
            for (link_itr = 0; link_itr < DIGI_ENET_LINE_CHNL_MAX; link_itr++)
            {
                if (TRUE == enet_enhanced_pmon_prov_get(digi_handle->enet_line_handle,link_itr,FALSE,&enet_data_rate))
                {
                    /* check if the */
                    if (UTIL_GLOBAL_CHNL_OPERATIONAL == g_cpb_connect_data_ptr->chnl[(UINT32)UTIL_GLOBAL_CPB_PORT_ENET_LINE][link_itr].mst.state)
                    {
                        /* there is a connection to a PMON in enhanced PMON mode */
                        source_slv_port = g_cpb_connect_data_ptr->chnl[(UINT32)UTIL_GLOBAL_CPB_PORT_ENET_LINE][link_itr].mst.source_slv_port;
                        source_slv_chnl = g_cpb_connect_data_ptr->chnl[(UINT32)UTIL_GLOBAL_CPB_PORT_ENET_LINE][link_itr].mst.source_slv_chnl;

                        /* the slave port/chnl shall be operational and in broadcast  */
                        if (!(UTIL_GLOBAL_CHNL_OPERATIONAL == g_cpb_connect_data_ptr->chnl[source_slv_port][source_slv_chnl].slv.state && 
                              TRUE == g_cpb_connect_data_ptr->chnl[source_slv_port][source_slv_chnl].slv.is_broadcast))
                        {
                            /* The monitored link is dead, kill the enhanced PMON */
                            (void) digi_enet_line_db_deprov_cfg(digi_handle, link_itr,FALSE);
                        } else 
                        {                            
                            enet_mode = (ENET_LINK_100G == enet_data_rate)?DIGI_ENET_SERDES_100GE_MAC:
                                (ENET_LINK_40G == enet_data_rate)?DIGI_ENET_SERDES_40GE_MAC:DIGI_ENET_SERDES_10GE_MAC;
                            /* regen gsup pmon_line fields */
                            digi_enet_pmon_line_usage_set(digi_handle,link_itr,enet_mode,TRUE);
                        }
                    }                
                } else if (TRUE == enet_enhanced_pmon_prov_get(digi_handle->enet_line_handle,link_itr,TRUE,&enet_data_rate))
                {
                    /* enhanced PMON umbilical mode */
                    /* this case is always valid as it is provisionned and activated in otn mapper segment */
                    enet_mode = (ENET_LINK_100G == enet_data_rate)?DIGI_ENET_SERDES_100GE_MAC:
                        (ENET_LINK_40G == enet_data_rate)?DIGI_ENET_SERDES_40GE_MAC:DIGI_ENET_SERDES_10GE_MAC;
                    /* regen gsup pmon_line fields */
                    digi_enet_pmon_line_usage_set(digi_handle,link_itr,enet_mode,TRUE);
                    
                }            
            }           

        }
        /* SYSTEM side */
        if (FALSE == enet_start_state_test(digi_handle->enet_sys_handle) )
        {
            for (link_itr = 0; link_itr < DIGI_ENET_SYS_CHNL_MAX; link_itr++)
            {
                if (TRUE == enet_enhanced_pmon_prov_get(digi_handle->enet_sys_handle,link_itr,FALSE,&enet_data_rate))
                {
                    /* check if the */
                    if (UTIL_GLOBAL_CHNL_OPERATIONAL == g_cpb_connect_data_ptr->chnl[(UINT32)UTIL_GLOBAL_CPB_PORT_ENET_LINE][link_itr].mst.state)
                    {
                        /* there is a connection to a PMON in enhanced PMON mode */
                        source_slv_port = g_cpb_connect_data_ptr->chnl[(UINT32)UTIL_GLOBAL_CPB_PORT_ENET_SYS][link_itr].mst.source_slv_port;
                        source_slv_chnl = g_cpb_connect_data_ptr->chnl[(UINT32)UTIL_GLOBAL_CPB_PORT_ENET_SYS][link_itr].mst.source_slv_chnl;

                        /* the slave port/chnl shall be operational and in broadcast  */
                        if (!(UTIL_GLOBAL_CHNL_OPERATIONAL == g_cpb_connect_data_ptr->chnl[source_slv_port][source_slv_chnl].slv.state && 
                              TRUE == g_cpb_connect_data_ptr->chnl[source_slv_port][source_slv_chnl].slv.is_broadcast))
                        {
                            /* The monitored link is dead, kill the enhanced PMON */
                            (void) digi_enet_sys_db_deprov_cfg(digi_handle, link_itr);       
                        } 
                        else 
                        {
                            enet_mode = (ENET_LINK_100G == enet_data_rate)?DIGI_ENET_SERDES_100GE_MAC:
                                (ENET_LINK_40G == enet_data_rate)?DIGI_ENET_SERDES_40GE_MAC:DIGI_ENET_SERDES_10GE_MAC;
                            /* regen gsup pmon_line fields */
                            digi_enet_pmon_sys_usage_set(digi_handle,link_itr,enet_mode,TRUE);
                            
                        }
                    }                
                }            
            }                        
        }        
    }
    /* an S16 serdes may be configured to provide an RX  clock to the GSUP block 
       however, that is only required when we provide the block. so in crash-restart, we let serdes restart
       clear this additional serdes and the context can be cleared.
    */
    /* unset pmon sys gsup43_7_3 clock is there is no enet sys ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON datapath provisioned */
    if (0 == enet_enhanced_pmon_num_links_set_up_gsup43_pmon(digi_handle->enet_sys_handle))
    {
        digi_enet_enhanced_pmon_sys_map_gsup43_7_3_pmon_clk_unset_helper(digi_handle);
        digi_handle->var.digi_enet_pmon_GSUP43_7_3_sys_usage = DIGI_SERDES_PIN_NOT_USED;
        digi_handle->var.digi_enet_pmon_GSUP43_7_3_sys_clk_type = DIGI_ENET_GSUP43_PMON_CLK_SRC_NONE;
    }

    PMC_RETURN();
}

/*******************************************************************************
* digi_resource_clean
* ______________________________________________________________________________
** DESCRIPTION:
*  Called after resource table has been cleaned in a restart. Iterates over
*  resource table removing any entry found that is not connected to anything
*  
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*
* OUTPUTS:
    None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_resource_clean(digi_handle_t *digi_handle)
{
    PMC_ENTRY();

    digi_resource_clean_firmware(digi_handle);
    digi_resource_clean_post_mld_dsi_ctxt(digi_handle, DIGI_SERDES_LINE_INTF);
    digi_resource_clean_post_mld_dsi_ctxt(digi_handle, DIGI_SERDES_SYSTEM_INTF);
    
    PMC_RETURN();
}

#endif /* DOXYGEN_PUBLIC_ONLY */

/*******************************************************************************
* digi_resource_handle_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Prints the table
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *digi_resource_handle  - Structure to be printed
*
* OUTPUTS:
    None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_resource_handle_print(digi_handle_t *digi_handle,
                                       digi_resource_handle_summary_t *digi_resource_handle)
{
    UINT32 i = 0;
    BOOL8 print_header = TRUE;
#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif

    PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
            "===============================|=======================================================================================|\n");    
    for (i=0; i < DIGI_XFI_PIN_NUM; i++)
    {
        if (NULL != digi_resource_handle->line_serdes_cfg[i].port_ctxt_ptr)
        {
            if (TRUE == print_header)
            {
                digi_print_header_footer("  LINE SIDE SERDES  ", TRUE);
            }
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                    "SerdesPort\t  PortHandle\t  ChnlHandleType\t   ChannelHandle\t   NextHandle\t     NextIndex\n");

            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                           "   %d\t  %20p\t %13s\t %12p\t %18s\t %10d\n", digi_resource_handle->line_serdes_cfg[i].port_uid, digi_resource_handle->line_serdes_cfg[i].port_ctxt_ptr, digi_convert_handle_id(digi_resource_handle->line_serdes_cfg[i].chnl_handle_type), digi_resource_handle->line_serdes_cfg[i].chnl_handle_ptr, digi_resource_convert_resource_handle(digi_resource_handle->line_serdes_cfg[i].next_resource_handle), digi_resource_handle->line_serdes_cfg[i].next_handle_index);
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                    "----------|--------------------|--------------------|----------------------|-------------------------|---------------------|\n");
            print_header = digi_print_next_resource(digi_handle, digi_resource_handle->line_serdes_cfg[i].next_resource_handle, digi_resource_handle->line_serdes_cfg[i].next_handle_index, i, digi_resource_handle);

            if (TRUE == print_header)
            {
                digi_print_header_footer("", FALSE);
            }
            else if ( (i+1) ==  DIGI_XFI_PIN_NUM)
            {
                digi_print_header_footer("", FALSE);
            }
        }
    }
    print_header = TRUE;

    for (i=0; i< DIGI_XFI_PIN_NUM; i++)
    {
        if (NULL != digi_resource_handle->sys_serdes_cfg[i].port_ctxt_ptr)
        {
            if (TRUE == print_header)
            {
                digi_print_header_footer("  SYSTEM SIDE SERDES  ", TRUE);
            }
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                    "SerdesPort\t  PortHandle\t  ChnlHandleType\t   ChannelHandle\t   NextHandle\t     NextIndex\n");
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                    "   %d\t  %20p\t %17s\t %15p\t %12s\t %10d\n", digi_resource_handle->sys_serdes_cfg[i].port_uid, digi_resource_handle->sys_serdes_cfg[i].port_ctxt_ptr, digi_convert_handle_id(digi_resource_handle->sys_serdes_cfg[i].chnl_handle_type), digi_resource_handle->sys_serdes_cfg[i].chnl_handle_ptr, digi_resource_convert_resource_handle(digi_resource_handle->sys_serdes_cfg[i].next_resource_handle), digi_resource_handle->sys_serdes_cfg[i].next_handle_index);
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                    "----------|--------------------|--------------------|----------------------|-------------------------|---------------------|\n");
            print_header = digi_print_next_resource(digi_handle, digi_resource_handle->sys_serdes_cfg[i].next_resource_handle, digi_resource_handle->sys_serdes_cfg[i].next_handle_index, i, digi_resource_handle);

            if (TRUE == print_header)
            {
                digi_print_header_footer("", FALSE);
            }
            else if ( (i+1) ==  DIGI_XFI_PIN_NUM)
            {
                digi_print_header_footer("", FALSE);
            }
        }
    }
    print_header = TRUE;

    for (i=0; i< DIGI_SFI51_INTF_NUM; i++)
    {
        if (NULL != digi_resource_handle->sfi51_serdes_cfg[i].port_ctxt_ptr)
        {
            if (TRUE == print_header)
            {
                digi_print_header_footer("  SFI 51 SIDE SERDES  ", TRUE);
            }
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                    "SerdesPort\t  PortHandle\t  ChnlHandleType\t   ChannelHandle\t   NextHandle\t     NextIndex\n");
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                    "   %d\t  %20p\t %17s\t %15p\t %12s\t %10d\n", digi_resource_handle->sfi51_serdes_cfg[i].port_uid, digi_resource_handle->sfi51_serdes_cfg[i].port_ctxt_ptr, digi_convert_handle_id(digi_resource_handle->sfi51_serdes_cfg[i].chnl_handle_type), digi_resource_handle->sfi51_serdes_cfg[i].chnl_handle_ptr, digi_resource_convert_resource_handle(digi_resource_handle->sfi51_serdes_cfg[i].next_resource_handle), digi_resource_handle->sfi51_serdes_cfg[i].next_handle_index);

            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                    "----------|--------------------|--------------------|----------------------|-------------------------|---------------------|\n");
            print_header = digi_print_next_resource(digi_handle, digi_resource_handle->sfi51_serdes_cfg[i].next_resource_handle, digi_resource_handle->sfi51_serdes_cfg[i].next_handle_index, i, digi_resource_handle);

            if (TRUE == print_header)
            {
                digi_print_header_footer("", FALSE);
            }
            else if ( (i+1) ==  DIGI_SFI51_INTF_NUM)
            {
                digi_print_header_footer("", FALSE);
            }
        }
    }
    digi_print_switch(digi_resource_handle);

    digi_print_ilkn_channels(digi_resource_handle);

    digi_print_mapotn_channels(digi_handle, digi_resource_handle);

    fflush(stdout);

    PMC_ATOMIC_RETURN(digi_handle);
} /* digi_resource_handle_print */

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* digi_resource_is_handle_provisioned
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Called at the of a digi_handle_restart_init if there are not operational
*  channels found and determines if there is at least one provisioned 
*  resource. The resource is any resource other than MAPOTN. Function 
*  returns TRUE if one is found.
*
* INPUTS:
*   *digi_handle           - Reference to DIGI handle instance
*   *digi_resource_handle  - Resource structure checked for ILKN handles
*
* OUTPUTS:
    None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 digi_resource_is_handle_provisioned(digi_handle_t *digi_handle,
                                                    digi_resource_handle_summary_t *digi_resource_handle)
{
    BOOL8 result = FALSE;
    UINT32 i = 0;

    PMC_ENTRY();

    /*
     * Check that there are not any CBRC/ENET Line/Sys data handles that
     * have been provisioned but not connected.  This is a valid scenario
     */
    for (i = 0; i < DIGI_XFI_PIN_NUM; i++)
    {
        if (NULL != digi_resource_handle->line_serdes_cfg[i].port_ctxt_ptr &&
            LAST_DIGI_HANDLE_ID != digi_resource_handle->line_serdes_cfg[i].chnl_handle_type &&
            NULL != digi_resource_handle->line_serdes_cfg[i].chnl_handle_ptr &&
            DIGI_CPB_SWITCH == digi_resource_handle->line_serdes_cfg[i].next_resource_handle)
        {
            /*
             * If here then this is a valid data handle. To be definite 
             * check the provisioning state of the data handle.
             */
            util_global_switch_data_def_t *hndl_ptr = (util_global_switch_data_def_t *)digi_resource_handle->line_serdes_cfg[i].chnl_handle_ptr;

            if (TRUE == util_global_rx_or_tx_chnl_prov_status_get(hndl_ptr->header.prov_state))
            {
                PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                        "Provisioned Line Serdes Handle found. Port=%d\n",
                        digi_resource_handle->line_serdes_cfg[i].port_uid);
                result = TRUE;
                break;
            }
        }
        if (NULL != digi_resource_handle->line_serdes_cfg[i].port_ctxt_ptr &&
            LAST_DIGI_HANDLE_ID != digi_resource_handle->line_serdes_cfg[i].chnl_handle_type &&
            NULL != digi_resource_handle->line_serdes_cfg[i].chnl_handle_ptr &&
            DIGI_LINE_OTN_HO_CONTAINER == digi_resource_handle->line_serdes_cfg[i].next_resource_handle)
        {
            UINT32 index = digi_resource_handle->line_serdes_cfg[i].next_handle_index;
            odu_struct_t *odu_ptr = (odu_struct_t *)digi_resource_handle->line_otn_ho_container_cfg[index].chnl_handle_ptr;
            /*
             * Need to check the line_otn_ho_container to confirm that
             * it is allocated and provisioned
             */
            if (NULL != odu_ptr && 
                (TRUE == util_global_rx_or_tx_chnl_prov_status_get(odu_ptr->switch_data.header.prov_state)))
            {
                result = TRUE;
                break;
            }
        }

        if (NULL != digi_resource_handle->sys_serdes_cfg[i].port_ctxt_ptr &&
            LAST_DIGI_HANDLE_ID != digi_resource_handle->sys_serdes_cfg[i].chnl_handle_type &&
            NULL != digi_resource_handle->sys_serdes_cfg[i].chnl_handle_ptr &&
            DIGI_CPB_SWITCH == digi_resource_handle->sys_serdes_cfg[i].next_resource_handle)
        {
            /*
             * If here then this is a valid data handle. To be definite 
             * check the provisioning state of the data handle.
             */
            util_global_switch_data_def_t *hndl_ptr = (util_global_switch_data_def_t *)digi_resource_handle->sys_serdes_cfg[i].chnl_handle_ptr;

            if (TRUE == util_global_rx_or_tx_chnl_prov_status_get(hndl_ptr->header.prov_state))
            {
                PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                        "Provisioned SYS Serdes Handle found. Port=%d\n",
                        digi_resource_handle->line_serdes_cfg[i].port_uid);
                result = TRUE;
                break;
            }
        }
        if (NULL != digi_resource_handle->sys_serdes_cfg[i].port_ctxt_ptr &&
            LAST_DIGI_HANDLE_ID != digi_resource_handle->sys_serdes_cfg[i].chnl_handle_type &&
            NULL != digi_resource_handle->sys_serdes_cfg[i].chnl_handle_ptr &&
            DIGI_SYS_OTN_HO_CONTAINER == digi_resource_handle->sys_serdes_cfg[i].next_resource_handle)
        {
            UINT32 index = digi_resource_handle->sys_serdes_cfg[i].next_handle_index;
            odu_struct_t *odu_ptr = (odu_struct_t *)digi_resource_handle->sys_otn_ho_container_cfg[index].chnl_handle_ptr;
            /*
             * Need to check the line_otn_ho_container to confirm that
             * it is allocated and provisioned
             */
            if (NULL != odu_ptr && 
                (TRUE == util_global_rx_or_tx_chnl_prov_status_get(odu_ptr->switch_data.header.prov_state)))
            {
                result = TRUE;
                break;
            }            
        }
        if (i < DIGI_SFI51_INTF_NUM &&
            NULL != digi_resource_handle->sfi51_serdes_cfg[i].port_ctxt_ptr &&
            LAST_DIGI_HANDLE_ID != digi_resource_handle->sfi51_serdes_cfg[i].chnl_handle_type &&
            NULL != digi_resource_handle->sfi51_serdes_cfg[i].chnl_handle_ptr &&
            DIGI_CPB_SWITCH == digi_resource_handle->sfi51_serdes_cfg[i].next_resource_handle)
        {
            /*
             * If here then this is a valid data handle. To be definite 
             * check the provisioning state of the data handle.
             */
            util_global_switch_data_def_t *hndl_ptr = (util_global_switch_data_def_t *)digi_resource_handle->sys_serdes_cfg[i].chnl_handle_ptr;

            if (TRUE == util_global_rx_or_tx_chnl_prov_status_get(hndl_ptr->header.prov_state))
            {
                PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                        "Provisioned SFI51 Serdes Handle found. Port=%d\n",
                        digi_resource_handle->line_serdes_cfg[i].port_uid);
                result = TRUE;
                break;
            }
        }
        if (i < DIGI_SFI51_INTF_NUM &&
            NULL != digi_resource_handle->sfi51_serdes_cfg[i].port_ctxt_ptr &&
            LAST_DIGI_HANDLE_ID != digi_resource_handle->sfi51_serdes_cfg[i].chnl_handle_type &&
            NULL != digi_resource_handle->sfi51_serdes_cfg[i].chnl_handle_ptr &&
            DIGI_LINE_OTN_HO_CONTAINER == digi_resource_handle->sfi51_serdes_cfg[i].next_resource_handle)
        {
            UINT32 index = digi_resource_handle->sfi51_serdes_cfg[i].next_handle_index;
            odu_struct_t *odu_ptr = (odu_struct_t *)digi_resource_handle->line_otn_ho_container_cfg[index].chnl_handle_ptr;
            /*
             * Need to check the line_otn_ho_container to confirm that
             * it is allocated and provisioned
             */
            if (NULL != odu_ptr && 
                (TRUE == util_global_rx_or_tx_chnl_prov_status_get(odu_ptr->switch_data.header.prov_state)))
            {
                result = TRUE;
                break;
            }
        }
    }

    if (FALSE == result &&
        LAST_DIGI_SIFD_ILKN == ((digi_ilkn_link_def_t *)digi_resource_handle->ilkn_inst_cfg[0])->ilkn_inst &&
        LAST_DIGI_SIFD_ILKN == ((digi_ilkn_link_def_t *)digi_resource_handle->ilkn_inst_cfg[1])->ilkn_inst)
    {
        /*
         * If the ILKN instances are not set then there are no ILKN 
         * channels configured and thus provisioned.
         */
        PMC_RETURN(FALSE);
    }

    /*
     * The ILKN instances have been defined. Now determined if there is at
     * least one ILKN channel that has been provisioned. 
     */
    for (i = 0; i < DIGI_ILKN_CHNL_NUM && FALSE == result; i++)
    {
      digi_ilkn_chnl_def_t *ilkn_ptr = (digi_ilkn_chnl_def_t *)digi_resource_handle->ilkn_chnl_cfg[0][i].ilkn_chnl_handle_ptr;

        if (NULL != ilkn_ptr &&
            (TRUE == util_global_rx_or_tx_chnl_prov_status_get(ilkn_ptr->switch_data.header.prov_state)))
        {
            result = TRUE;
            break;
        }
        ilkn_ptr = (digi_ilkn_chnl_def_t *)digi_resource_handle->ilkn_chnl_cfg[1][i].ilkn_chnl_handle_ptr;

        if (NULL != ilkn_ptr &&
            (TRUE == util_global_rx_or_tx_chnl_prov_status_get(ilkn_ptr->switch_data.header.prov_state)))
        {
            result = TRUE;
            break;
        }
    }

    PMC_RETURN(result);
} /* digi_resource_is_handle_provisioned */


/*******************************************************************************
* digi_resource_is_path_operational
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Checks the operational level of the various serdes port datapaths and 
*  cleans any path that is incomplete in a particular direction. The order
*  in which the serdes handles are checked are based on the OTN component
*  that can be multiplexed. For SYSOTN applications that is the system side.
*  For all else it is line side.
*
* INPUTS:
*  digi_handle               - Reference to DIGI handle instance
*  digi_resource_handle_ptr  - Resource structure checked for ILKN handles
*  first_intf                - Interface that should be checked first. For
*                              SYSOTN applications that is the system side.
*
* OUTPUTS:
    None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 digi_resource_is_path_operational(digi_handle_t *digi_handle,
                                               digi_resource_handle_summary_t *digi_resource_handle_ptr,
                                               digi_serdes_intf_t first_intf)
{
    BOOL8 operational_path_exists = FALSE;
    BOOL8 is_tx_reset_required = FALSE;
    BOOL8 is_rx_reset_required = FALSE;
    digi_serdes_port_intf_ctxt_t *serdes_port_ptr = NULL;
    digi_serdes_intf_t current_intf = LAST_DIGI_SERDES_INTF;
    UINT32 i;
    UINT32 inst = 0;
    UINT32 num_elements = 0;
    PMC_ENTRY();

    for (inst = 0; inst < 3; inst++)
    {
        if (LAST_DIGI_SERDES_INTF == current_intf)
        {
            current_intf = first_intf;
        }
        else if (DIGI_SERDES_LINE_INTF == current_intf)
        {
            current_intf = DIGI_SERDES_SFI_51_INTF;
        }
        else if (DIGI_SERDES_SFI_51_INTF == current_intf)
        {
            current_intf = DIGI_SERDES_SYSTEM_INTF;
        }
        else if (DIGI_SERDES_SYSTEM_INTF == current_intf)
        {
            current_intf = DIGI_SERDES_LINE_INTF;
        }
        else
        {
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        }

        if (DIGI_SERDES_LINE_INTF == current_intf)
        {
            num_elements = DIGI_NUM_LINE_PORTS_MAX;
        }
        else if (DIGI_SERDES_SFI_51_INTF == current_intf)
        {
            num_elements = DIGI_NUM_SFI51_PORTS_MAX;
        }
        else if (DIGI_SERDES_SYSTEM_INTF == current_intf)
        {
            num_elements = DIGI_NUM_SYS_PORTS_MAX;
        }

        for (i=0; i < num_elements; i++)
        {
            BOOL8 one_operational_path = FALSE;

            if (DIGI_SERDES_LINE_INTF == current_intf)
            {
                if (NULL != digi_resource_handle_ptr->line_serdes_cfg[i].port_ctxt_ptr)
                {
                    serdes_port_ptr = &digi_resource_handle_ptr->line_serdes_cfg[i];
                }
            }
            else if (DIGI_SERDES_SFI_51_INTF == current_intf)
            {
                if (NULL != digi_resource_handle_ptr->sfi51_serdes_cfg[i].port_ctxt_ptr)
                {
                    serdes_port_ptr = &digi_resource_handle_ptr->sfi51_serdes_cfg[i];
                }
            }
            else if (DIGI_SERDES_SYSTEM_INTF == current_intf)
            {
                if (NULL != digi_resource_handle_ptr->sys_serdes_cfg[i].port_ctxt_ptr)
                {
                    serdes_port_ptr = &digi_resource_handle_ptr->sys_serdes_cfg[i];
                }
            }

            if (NULL != serdes_port_ptr)
            {
                BOOL8 is_operational = digi_resource_check_path(digi_handle,
                                                                digi_resource_handle_ptr,
                                                                serdes_port_ptr,
                                                                &one_operational_path);
                
                if (FALSE == is_operational)
                {
                    digi_resource_release_datapath(digi_handle,
                                                   digi_resource_handle_ptr,
                                                   serdes_port_ptr);
                    
                    if (FALSE == serdes_port_ptr->tx_path_is_valid)
                    {
                        is_tx_reset_required = TRUE;
                    }
                    if (FALSE == serdes_port_ptr->rx_path_is_valid)
                    {
                        is_rx_reset_required = TRUE;
                    }
                }
                if (TRUE == one_operational_path)
                {
                  operational_path_exists = one_operational_path;
                }
                serdes_port_ptr = NULL;
            }
        }
    }
    if (FALSE == operational_path_exists)
    {
        /*
         * There is nothing currently connected to a serdes interface. Run
         * a separate check to determine if there exists at least one
         * provisioned ILKN channel handle. If there is then an 
         * operational path exists.
         */ 
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "Calling digi_resource_is_ilkn_hndl_provisioned\n");
        operational_path_exists = digi_resource_is_handle_provisioned(digi_handle, digi_resource_handle_ptr);
    }

    PMC_LOG_NOINFO(PMC_LOG_SEV_HIGH, 0, 0, 0,
                   "Operational Path check is complete. Results:\n");
    PMC_LOG_NOINFO(PMC_LOG_SEV_HIGH, 0, 0, 0,
                   "\tOperational Path(s) Exist=%d\n", operational_path_exists);
    PMC_LOG_NOINFO(PMC_LOG_SEV_HIGH, 0, 0, 0,
                   "\tPossible Incomplete Tx Operational Datapath Exist = %d which if TRUE may result in a data path reset\n", is_tx_reset_required);
    PMC_LOG_NOINFO(PMC_LOG_SEV_HIGH, 0, 0, 0,
                   "\tPossible Incomplete Rx Operational Datapath Exist = %d which if TRUE may result in a data path reset\n", is_rx_reset_required);


    PMC_RETURN(operational_path_exists);
} /* digi_resource_is_path_operational */

#endif /* DOXYGEN_PUBLIC_ONLY */

/*******************************************************************************
* digi_serdes_port_init_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function returns the assoiated serdes interface port handle
*   based on the interface and physical serdes port id.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   port_uid               - Unique user assigned value for identifying
*                            serdes port.  The value 0xFFFFFFFF
*                            DIGI_SERDES_UNASSIGNED is reserved as the
*                            default unassigned setting.
*
* OUTPUTS:
*   **port_ctxt_pptr       - pointer to a pointer to a serdes port data used for
*                            identifying serdes context for usage by other API
*                            of the SW driver.
*
* RETURNS:
*   DIGI_ERR_INVALID_ARG, or
*   DIGI_ERR_NULL_HANDLE on failure.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_port_init_get(digi_handle_t *digi_handle,
                                           UINT32 port_uid,
                                           digi_serdes_port_t **port_ctxt_pptr)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    digi_serdes_port_def_t *port_ctxt_ptr = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != port_ctxt_pptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    result = digi_resource_get_port_context(digi_handle,
                                            port_uid,
                                            &port_ctxt_ptr);
    
    if (NULL == port_ctxt_ptr)
    {
        result = DIGI_ERR_NULL_HANDLE;
    } 
    else 
    {
        *port_ctxt_pptr = (digi_serdes_port_t*)port_ctxt_ptr;
        result = PMC_SUCCESS;
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_serdes_port_init_get */


/*******************************************************************************
* digi_device_init_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Returns paraneters set with digi_device_init was called.
*
* INPUTS:
*  digi_handle       - pointer to DIGI handle instance.
*
* OUTPUTS:
*  prod_app_ptr      - Reference to the product application value.
*  mux_stages_ptr    - Reference to the number of muxing stages that were
*                      set.
*  ilkn_1_ctxt_pptr  - Assigned a value
*  ilkn_2_ctxt_pptr  - Assigned value fo ILKNK 2 instance it enabled.
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_device_init_get(digi_handle_t *digi_handle,
                                      digi_prod_app_t *prod_app_ptr,
                                      digi_otn_mux_stages_t *mux_stages_ptr,
                                      digi_ilkn_link_t **ilkn_1_ctxt_pptr,
                                      digi_ilkn_link_t **ilkn_2_ctxt_pptr)
{
#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != prod_app_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    PMC_ASSERT(NULL != mux_stages_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != ilkn_1_ctxt_pptr, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != ilkn_2_ctxt_pptr, DIGI_ERR_NULL_HANDLE, 0, 0);


    *prod_app_ptr = digi_handle->var.prod_app;
    *mux_stages_ptr = digi_handle->var.mux_stages;
    *ilkn_1_ctxt_pptr =  &(digi_handle->handle_pool.ilkn_link[DIGI_SIFD_ILKN1]);
    *ilkn_2_ctxt_pptr =  &(digi_handle->handle_pool.ilkn_link[DIGI_SIFD_ILKN2]);

    PMC_ATOMIC_RETURN(digi_handle);
}

/**************************************************************************
* digi_serdes_port_chnl_hndl_get
* _________________________________________________________________________
*
* DESCRIPTION:
*
*   This function retrieves the channel handle and channel handle type for a
*   serdes port.\n\n
*
*   The channel handle type returned is void, usage in subsequent API calls may
*   require type casting.
*   
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   *port_ctxt_ptr         - Reference to the SERDES port handle
*
* OUTPUTS:
*   *chnl_hndl_type_ptr    - type of subsystem handle associated with serdes port
*   **chnl_hndl_pptr       - subsystem channel handle associated with serdes port
*   **ho_chnl_hndl_pptr    - if chnl_hndl_type_ptr is DIGI_CHANNEL_HANDLE_ID_LINEOTN_T
*                            or DIGI_CHANNEL_HANDLE_ID_SYSOTN_T, then provides the
*                            DIGI_CHANNEL_HANDLE_ID_HO_ODU_T channel handle if HO 
*                            is provisioned, NULL otherwise.
*    
* RETURNS:
*  PMC_SUCCESS - A connection was found associated with the port_ctxt_ptr.
*  DIGI_ERR_SERDES_PORT_NOT_PROV - If no subsystem is provisioned against
*                                  the serdes port handle.
*   DIGI_ERR_INVALID_ARG - If one or more of the passed in parameters are
*                          INVALID.
*   DIGI_ERR_ASSERT      - If one or more of the passed in parameters are
*                          NULL
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_port_chnl_hndl_get(digi_handle_t *digi_handle,
                                                digi_serdes_port_t *port_ctxt_ptr,
                                                digi_handle_id_t *chnl_hndl_type_ptr,
                                                digi_chnl_hndl_t **chnl_hndl_pptr,
                                                digi_otn_odu_chnl_t **ho_chnl_hndl_pptr)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    digi_serdes_port_mode_t serdes_port_mode;
    digi_serdes_prov_state_t serdes_prov_state;
    BOOL8 serdes_prov_status;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != port_ctxt_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_hndl_type_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_hndl_pptr, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != ho_chnl_hndl_pptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    *chnl_hndl_type_ptr = LAST_DIGI_HANDLE_ID;

    if (DIGI_SERDES_UNASSIGNED == ((digi_serdes_port_def_t *)port_ctxt_ptr)->port_uid)
    {
        PMC_ATOMIC_RETURN(digi_handle, DIGI_ERR_SERDES_PORT_NOT_PROV);
    }

    result = digi_serdes_port_rate_prov_status_internal_get(digi_handle,
                                                            port_ctxt_ptr,
                                                            FALSE,
                                                            &serdes_prov_status,
                                                            &serdes_port_mode,
                                                            &serdes_prov_state);

    if (PMC_SUCCESS != result)
    {
        PMC_ATOMIC_RETURN(digi_handle, result);
    }

    if (DIGI_SERDES_PROV_STATE_NO_SERDES == serdes_prov_state ||
        DIGI_SERDES_PROV_STATE_UNASSIGNED == serdes_prov_state)
    {
        PMC_ATOMIC_RETURN(digi_handle, DIGI_ERR_SERDES_PORT_NOT_PROV);
    }
    result = digi_otn_server_prov_chnl_get(digi_handle, port_ctxt_ptr,
                                           (digi_otn_server_chnl_t **)chnl_hndl_pptr);

    if (PMC_SUCCESS == result)
    {
        digi_otn_server_chnl_def_t *otn_server_ptr = (digi_otn_server_chnl_def_t *)*chnl_hndl_pptr;

        if (TRUE == otn_server_ptr->sys_line_src)
        {
            *chnl_hndl_type_ptr = DIGI_CHANNEL_HANDLE_ID_SYSOTN_T;
        }
        else
        {
            *chnl_hndl_type_ptr = DIGI_CHANNEL_HANDLE_ID_LINEOTN_T;
        }
        /*
         * Now retrieve the High order channel associated with the lineotn
         * handle.
         */
        *ho_chnl_hndl_pptr = NULL;

        result = digi_otn_server_ho_prov_chnl_get(digi_handle,
                                                  (digi_otn_server_chnl_t *)*chnl_hndl_pptr,
                                                  ho_chnl_hndl_pptr);

        if (PMC_SUCCESS != result || NULL == *ho_chnl_hndl_pptr )
        {
            result = DIGI_ERR_NO_OPERATIONAL_DATAPATH;
        }
    }
    else if (PMC_SUCCESS == digi_enet_prov_chnl_get(digi_handle,
                                                    port_ctxt_ptr,
                                                    (digi_enet_client_chnl_t **)chnl_hndl_pptr))
    {
        if (DIGI_SERDES_LINE_INTF == ((digi_serdes_port_def_t *)port_ctxt_ptr)->intf ||
            (DIGI_SERDES_SYSTEM_INTF == ((digi_serdes_port_def_t *)port_ctxt_ptr)->intf &&
             DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == digi_handle->var.prod_app))
        {
            *chnl_hndl_type_ptr = DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T;
        }
        else if (DIGI_SERDES_SYSTEM_INTF == ((digi_serdes_port_def_t *)port_ctxt_ptr)->intf)
        {
            *chnl_hndl_type_ptr = DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T;
        }

        result = PMC_SUCCESS;
    }
    else if (PMC_SUCCESS == digi_cbr_client_prov_chnl_get(digi_handle,
                                                          port_ctxt_ptr,
                                                          (digi_enet_client_chnl_t **)chnl_hndl_pptr))
    {
          *chnl_hndl_type_ptr = DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T;
          result = PMC_SUCCESS;
    }
    else
    {
        result = DIGI_ERR_SERDES_PORT_NOT_PROV;
    }
    PMC_ATOMIC_RETURN(digi_handle, result);
} /*  digi_serdes_port_chnl_hndl_get */

/*******************************************************************************
* digi_ho_subcontainer_chnl_hndl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function retrieves the set of channel handles for the subcontainers 
*   immediately associated with a HO or MO container. DIGI_CHANNEL_HANDLE_ID_HO_ODU_T,
*   and DIGI_CHANNEL_HANDLE_ID_MO_ODU_T respectively.\n\n
*
*   If the HO or MO container is unchannelized, the container is directly 
*   connected to ODUKSW.  In this case error code DIGI_ERR_HO_MO_CONN_TO_ODUKSW
*   is returned. Otherwise the chnl_lo_hndl_ptr is populated with the lower
*   order channels from index 0 forward. The first NULL encountered in the
*   chnl_lo_hndl_ptr array indicates the last lo order handle has been 
*   found.
*   
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*    ho_chnl_hndl_type     - Type of ODU subsystem that represents the
*                            higher order handle. Only
*                            DIGI_CHANNEL_HANDLE_ID_HO_ODU_T and
*                            DIGI_CHANNEL_HANDLE_ID_MO_ODU_T are valid.
*   *ho_chnl_hndl_ptr      - The higher order handle whose subcontainers are
*                            being retrieved.
*   
*
* OUTPUTS:
*   *lo_chnl_hndl_type_ptr - References to the digi_handle_id_t that defines
*                            the subcontainer type. Only
*                            DIGI_CHANNEL_HANDLE_ID_MO_ODU_T or
                             DIGI_CHANNEL_HANDLE_ID_LO_ODU_T are valid.
*   *lo_chnl_hndl_ptr[]    - Pointer references of the subcontainers
*                            associated with this higher order handle.
*    
* RETURNS:
*   PMC_SUCCESS - At least one lo order channel was found associted with the
*                 higher order handle.
*   DIGI_ERR_NO_OPERATIONAL_DATAPATH - The HO/MO order channel is not
*                                      unchannelized but there are no lower
*                                      order channels associated with it.
*   DIGI_ERR_HO_MO_CONN_TO_ODUKSW    - The Higher order channel referenced
*                                      by ho_chnl_hndl_ptr is connected to 
*                                      ODUKSW directly.
*   DIGI_ERR_INVALID_ARG - If one or more of the parameters are invalid.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ho_subcontainer_chnl_hndl_get(digi_handle_t *digi_handle,
                                                    digi_handle_id_t ho_chnl_hndl_type,
                                                    digi_otn_odu_chnl_t *ho_chnl_hndl_ptr,
                                                    digi_handle_id_t *lo_chnl_hndl_type_ptr,
                                                    digi_otn_odu_chnl_t *lo_chnl_hndl_ptr[DIGI_OTN_SERVER_MAX_TRIBSLOT])
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 ho_cal_array[DIGI_OTN_SERVER_MO_CHNL_MAX];
    UINT32 num_cal_entries = 0;
    UINT32 i;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != ho_chnl_hndl_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != lo_chnl_hndl_type_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != lo_chnl_hndl_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);
    /*
     * Ensure the low order channel handle array is initialzed to NULL
     * values so that the caller knows when to stop searching through
     * it.
     */
    for (i = 0; i < DIGI_OTN_SERVER_MAX_TRIBSLOT; i++)
    {
        lo_chnl_hndl_ptr[i] = NULL;
    }

    result = digi_resource_get_coreotn_subcontainers(digi_handle,
                                                     ho_chnl_hndl_type,
                                                     ho_chnl_hndl_ptr,
                                                     ho_cal_array,
                                                     &num_cal_entries);

    if (PMC_SUCCESS == result && 0 == num_cal_entries)
    {
        result = DIGI_ERR_NO_OPERATIONAL_DATAPATH;
    }
    else if (PMC_SUCCESS == result && num_cal_entries > 0 )
    {
        UINT32 index = 0;
        odu_struct_t *lo_odu_struct_ptr = NULL;
        /*
         * Iterate over the calendar entries based on whether the channel
         * handle is active or not.
         */
        for (i = 0; i < num_cal_entries; i++)
        {
            UINT32 lo_chnl_id = ho_cal_array[i];

            if (DIGI_CHANNEL_HANDLE_ID_HO_ODU_T == ho_chnl_hndl_type)
            {
                lo_odu_struct_ptr = &(digi_handle->handle_pool.mo_odu_struct[lo_chnl_id]);
                *lo_chnl_hndl_type_ptr = DIGI_CHANNEL_HANDLE_ID_MO_ODU_T;   
            }
            else
            {
                lo_odu_struct_ptr = &(digi_handle->handle_pool.lo_odu_struct[lo_chnl_id]);
                *lo_chnl_hndl_type_ptr = DIGI_CHANNEL_HANDLE_ID_LO_ODU_T;
            }
            
            if (NULL != lo_odu_struct_ptr && TRUE == lo_odu_struct_ptr->active)
            {
                lo_chnl_hndl_ptr[index++] = (digi_otn_odu_chnl_t *)lo_odu_struct_ptr;
            }
        }
    }
  
    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_ho_subcontainer_chnl_hndl_get */


/*******************************************************************************
* digi_ho_subcontainer_trib_slot_chnl_hndl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function retrieves the a single channel handle if it matches the 
*   tributary slot usage of the HO or MO container specified by tx_trib_slot
*   and rx_trib_slot parameters.\n\n
*
*   If the HO or MO container is unchannelized, the container is directly 
*   connected to ODUKSW.  In this case error code DIGI_ERR_HO_MO_CONN_TO_ODUKSW
*   is returned.\n\n
*
*   If the tributary slots are empty DIGI_OTN_TIME_SLOT_OCCUPIED_ALL_FREE use
*   data is provided.\n\n
*
*   If more than one subcontainer occupies the tributary slots, or the 
*   subcontainer extends beyond these tributary slots then
*   DIGI_OTN_TIME_SLOT_OCCUPIED_MIXED_USE use date is provided.\n\n
*   
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*    ho_chnl_hndl_type     - type of subsystem handle associated with serdes port
*   *ho_chnl_hndl_ptr      - subsystem channel handle associated with serdes port
*  tx_trib_slot            - The tx tributary slot mask to be checked.
*  rx_trib_slot            - The rx tributary slot mask to be checked.
*
* OUTPUTS:
*  *slot_use_ptr           - enumeration describing usage of tributary slots
*  *lo_chnl_hndl_type_ptr  - subcontainer channel handle if use_ptr is
*                             DIGI_OTN_TIME_SLOT_OCCUPIED_ONE_CHANNEL
*  **lo_chnl_hndl_ptr      - Pointer to the first lo order channel found
*                            using any of the time slots in the mask.
*    
* RETURNS:
*   PMC_SUCCESS - At least one lo order channel was found associted with the
*                 higher order handle.  Check the slot_use_ptr to determine
*                 the trib slot use.
*   PMC_ERROR   - If there was an issue retrieving lo order channels.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ho_subcontainer_trib_slot_chnl_hndl_get(digi_handle_t *digi_handle,
                                                              digi_handle_id_t ho_chnl_hndl_type,
                                                              digi_otn_odu_chnl_t *ho_chnl_hndl_ptr,
                                                              UINT32 tx_trib_slot[3],
                                                              UINT32 rx_trib_slot[3],
                                                              digi_otn_time_slot_occupied_t *slot_use_ptr,
                                                              digi_handle_id_t *lo_chnl_hndl_type_ptr,
                                                              digi_otn_odu_chnl_t *lo_chnl_hndl_ptr[DIGI_OTN_SERVER_MAX_TRIBSLOT])
{

    PMC_ERROR result = PMC_SUCCESS;
    UINT32 ho_cal_array[DIGI_OTN_SERVER_MO_CHNL_MAX];
    UINT32 num_cal_entries = 0;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != ho_chnl_hndl_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != slot_use_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != lo_chnl_hndl_type_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    result = digi_resource_get_coreotn_subcontainers(digi_handle,
                                                     ho_chnl_hndl_type,
                                                     ho_chnl_hndl_ptr,
                                                     ho_cal_array,
                                                     &num_cal_entries);

    if (PMC_SUCCESS == result && 0 == num_cal_entries)
    {
        *slot_use_ptr = DIGI_OTN_TIME_SLOT_OCCUPIED_ALL_FREE;
        result = DIGI_ERR_NO_OPERATIONAL_DATAPATH;
    }
    else if (PMC_SUCCESS == result)
    {
        result = digi_resource_lo_channel_get_by_trib_slot(digi_handle,
                                                           num_cal_entries,
                                                           ho_cal_array,
                                                           ho_chnl_hndl_type,
                                                           tx_trib_slot,
                                                           rx_trib_slot,
                                                           slot_use_ptr,
                                                           lo_chnl_hndl_ptr);
    }
    *lo_chnl_hndl_type_ptr = LAST_DIGI_HANDLE_ID;

    if (PMC_SUCCESS == result)
    {
        if (DIGI_OTN_TIME_SLOT_OCCUPIED_ONE_CHANNEL == *slot_use_ptr)
        {
            if (DIGI_CHANNEL_HANDLE_ID_HO_ODU_T == ho_chnl_hndl_type)
            {
                *lo_chnl_hndl_type_ptr = DIGI_CHANNEL_HANDLE_ID_MO_ODU_T;
            }
            else if (DIGI_CHANNEL_HANDLE_ID_MO_ODU_T == ho_chnl_hndl_type)
            {
                *lo_chnl_hndl_type_ptr = DIGI_CHANNEL_HANDLE_ID_LO_ODU_T;
            }
        }
    }
    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_ho_subcontainer_trib_slot_chnl_hndl_get */

/*******************************************************************************
* digi_otn_mapper_prov_hndl_all_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Iterates over the set of mapotn channels and returns all handles that
*   are at least provisioned.

* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*
* OUTPUTS:
*   *map_chnl_hndl_ptr     - Array of MAPOTN handles that is populated with
*                            all provisioned MAPOTN channel handles.
*    
* RETURNS:
*   PMC_SUCCESS - At keast one MAPOTN channel handle is provisioned/mapped/activated.
*
*   DIGI_ERR_NO_OPERATIONAL_DATAPATH - No MAPOTN handles are currently 
*                                      provisioned/mapped/activated.
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_otn_mapper_prov_hndl_all_get(digi_handle_t *digi_handle,
                                                   digi_mapper_chnl_t *map_chnl_hndl_ptr[DIGI_MAPOTN_CHNL_MAX])
{

    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    UINT32 i = 0;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != map_chnl_hndl_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);
    result = DIGI_ERR_NO_OPERATIONAL_DATAPATH;

    for (i = 0; i < DIGI_MAPOTN_CHNL_MAX; i++)
    {
        if (TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.mapper_chnl[i].switch_data.header.prov_state))
        {
            map_chnl_hndl_ptr[i] = &(digi_handle->handle_pool.mapper_chnl[i]);
            result = PMC_SUCCESS;
        }
        else
        {
            map_chnl_hndl_ptr[i] = NULL;
        }
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_otn_mapper_prov_hndl_all_get */

/*******************************************************************************
* digi_ilkn_chnl_prov_hndl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*  Finds the ILKN channel handle and returns it given the BCW RX/TX channel
*  number used when it was configured. Note that both the RX/TX must match
*  otherwise DIGI_ERR_INVALID_ARG is returned.

* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   ilkn_handle_id         - ILKN instance that is being searched. Only
*                            valid values are:
*                            DIGI_CHANNEL_HANDLE_ID_ILKN_1_T or
*                            DIGI_CHANNEL_HANDLE_ID_ILKN_2_T
*   tx_ilkn_bcw_chnl       - Transmit BCW channel id.
*   rx_ilkn_bcw_chnl       - Receive BCW channel id.

*
* OUTPUTS:
*   **ilkn_chnl_hndl_pptr  - Reference to the ILKN channel handle that will
*                            be assigned to if the BCW channels ids match an
*                            existing value.
*    
* RETURNS:
*   PMC_SUCCESS - ILKN channel handle found that match the RX/TX BCW ids.
*
*   PMC_ERROR   - Returns an appropriate error if not match is found.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ilkn_chnl_prov_hndl_get(digi_handle_t *digi_handle,
                                              digi_handle_id_t ilkn_handle_id,
                                              UINT32 tx_ilkn_bcw_chnl,
                                              UINT32 rx_ilkn_bcw_chnl,
                                              digi_ilkn_chnl_t **ilkn_chnl_hndl_pptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    digi_ilkn_chnl_def_t *ilkn_chnl_ptr = NULL;
    UINT32 ilkn_inst;
    BOOL8 ilkn_found = FALSE;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != ilkn_chnl_hndl_pptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    if (DIGI_CHANNEL_HANDLE_ID_ILKN_1_T == ilkn_handle_id)
    {
        ilkn_inst = 0;
    }
    else if (DIGI_CHANNEL_HANDLE_ID_ILKN_2_T == ilkn_handle_id)
    {
        ilkn_inst = 1;
    }
    else
    {
        result = DIGI_ERR_INVALID_ARG;
    }    

    if (PMC_SUCCESS == result)
    {
        {
            UINT32 i;

            for(i = 0; i < DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX && PMC_SUCCESS == result && FALSE == ilkn_found; i++)
            {
                if (0 == ilkn_inst)
                {
                    ilkn_chnl_ptr = &(digi_handle->handle_pool.ilkn_1_chnl[i]);
                }
                else
                {
                    ilkn_chnl_ptr = &(digi_handle->handle_pool.ilkn_2_chnl[i]);
                }
            
                if (NULL != ilkn_chnl_ptr &&
                    UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG != ilkn_chnl_ptr->switch_data.header.prov_state &&
                    (ilkn_chnl_ptr->tx_ilkn_info.ilkn_bcw_chnl == tx_ilkn_bcw_chnl &&
                     ilkn_chnl_ptr->rx_ilkn_info.ilkn_bcw_chnl == rx_ilkn_bcw_chnl))
                {
                    *ilkn_chnl_hndl_pptr = (digi_ilkn_chnl_t *)ilkn_chnl_ptr;
                    ilkn_found = TRUE;
                }
            }
            /*
             * If the entire set of possible ILKN channels was searched and not 
             * found then set the appropriate error code.
             */
            if (FALSE == ilkn_found)
            {
                result = DIGI_ERR_NULL_HANDLE;
            }
        }
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_ilkn_chnl_prov_hndl_get */

/*******************************************************************************
* digi_ilkn_chnl_prov_2b_header_hndl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*  Finds the ILKN channel handle and returns it given the Tx/Rx 2 byte header
*  values used when it was configured. Note that both the RX/TX must match
*  otherwise DIGI_ERR_INVALID_ARG is returned.\n\n
*
*  This API is only valid when the ILKN interface has been configured with
*  oduksw_2b_header_mode set to FALSE and oduksw_header set to
*  DIGI_SIFD_ODUKSW_ILKN_HEADER_2B.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   ilkn_handle_id         - ILKN instance that is to be searched.
*   tx_2b_header           - Transmit 2B header value
*   rx_2b_header           - Receive 2B header value
*
* OUTPUTS:
*   **ilkn_chnl_hndl_pptr  - Reference to the ILKN channel handle that will
*                            be assigned to if the BCW channels ids match an
*                            existing value.
*    
* RETURNS:
*   PMC_SUCCESS - ILKN channel handle found that match the RX/TX BCW ids.
*
*   DIGI_ERR_INVALID_ARG - If no channel handle is found for the passed in
*                          BCW channel ids
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ilkn_chnl_prov_2b_header_hndl_get(digi_handle_t *digi_handle,
                                                        digi_handle_id_t ilkn_handle_id,
                                                        UINT32 tx_2b_header,
                                                        UINT32 rx_2b_header,
                                                        digi_ilkn_chnl_t **ilkn_chnl_hndl_pptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 ilkn_inst;

    PMC_ATOMIC_ENTRY(digi_handle);
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != ilkn_chnl_hndl_pptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    if (DIGI_CHANNEL_HANDLE_ID_ILKN_1_T == ilkn_handle_id)
    {
        ilkn_inst = 0;
    }
    else if (DIGI_CHANNEL_HANDLE_ID_ILKN_2_T == ilkn_handle_id)
    {
        ilkn_inst = 1;
    }
    else
    {
        result = DIGI_ERR_INVALID_ARG;
    }

    if (PMC_SUCCESS == result)
    {
        UINT32 i;
        BOOL8 ilkn_found = FALSE;
        digi_ilkn_chnl_def_t *ilkn_chnl_ptr = NULL;

        for(i = 0; i < DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX && FALSE == ilkn_found; i++)
        {
            if (0 == ilkn_inst)
            {
                ilkn_chnl_ptr = &(digi_handle->handle_pool.ilkn_1_chnl[i]);
            }
            else
            {
                ilkn_chnl_ptr = &(digi_handle->handle_pool.ilkn_2_chnl[i]);
            }
            
            if (NULL != ilkn_chnl_ptr &&
                UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG != ilkn_chnl_ptr->switch_data.header.prov_state &&
                ilkn_chnl_ptr->tx_ilkn_info.ilkn_2b_header == tx_2b_header &&
                ilkn_chnl_ptr->rx_ilkn_info.ilkn_2b_header == rx_2b_header)
            {
                *ilkn_chnl_hndl_pptr = (digi_ilkn_chnl_t *)ilkn_chnl_ptr;
                ilkn_found = TRUE;
            }
        }
        /*
         * If the entire set of possible ILKN channels was searched and not 
         * found then set the appropriate error code.
         */
        if (FALSE == ilkn_found)
        {
            result = DIGI_ERR_NULL_HANDLE;
        }
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_ilkn_chnl_prov_2b_header_hndl_get */

/*******************************************************************************
* digi_otn_server_prov_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves the OTUK mode, the fec mode and deskew type used when either
*  the LineOTN or SYSOTN channel was provisioned. For SYSOTN the 
*  otuk_mode_ptr value is set to LAST_DIGI_OTN_SERVER.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   otn_server_ptr         - Reference to the LineOTN channel handle.
*
* OUTPUTS:
*   otuk_mode_ptr          - ODUK Mode set when the channel was provisioned
*   fec_mode_ptr           - FEC mode set when the channel was provisioned
*   deskew_type_ptr        - Deskew type set when the channel was proviosned
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_otn_server_prov_get(digi_handle_t *digi_handle,
                                     digi_otn_server_chnl_t *otn_server_ptr,
                                     digi_otn_server_otuk_t *otuk_mode_ptr,
                                     digi_otn_server_fec_t *fec_mode_ptr,
                                     digi_sfis_deskew_type_t *deskew_type_ptr)
{
    digi_otn_server_chnl_def_t *local_server_ptr = NULL;
#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif

    PMC_ASSERT(otn_server_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(otuk_mode_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    local_server_ptr = (digi_otn_server_chnl_def_t*)otn_server_ptr;

    if (PMC_SUCCESS != digi_otn_fec_type_get(digi_handle, otn_server_ptr, 
                                             fec_mode_ptr))
    {
        /*
         * The call to digi_otn_fec_type_get should not fail however if it
         * does then set the fec_mode_ptr to the out of range value.
         */
        *fec_mode_ptr = LAST_DIGI_OTN_SERVER_FEC;
    }

    if (FALSE == local_server_ptr->sys_line_src)
    {
        switch (local_server_ptr->oduk_type)
        {
          case UTIL_GLOBAL_ODU2:
            *otuk_mode_ptr = DIGI_OTN_SERVER_OTU2;
            break;
            
          case UTIL_GLOBAL_ODU1E:
            *otuk_mode_ptr = DIGI_OTN_SERVER_OTU1E;
            break;
            
          case UTIL_GLOBAL_ODU2E:
            *otuk_mode_ptr = DIGI_OTN_SERVER_OTU2E;
            break;
            
          case UTIL_GLOBAL_ODU1F:
            *otuk_mode_ptr = DIGI_OTN_SERVER_OTU1F;
            break;
            
          case UTIL_GLOBAL_ODU2F:
            *otuk_mode_ptr =  DIGI_OTN_SERVER_OTU2F;
            break;
            
          case UTIL_GLOBAL_ODU3:   
            *otuk_mode_ptr = DIGI_OTN_SERVER_OTU3;
            break;
            
          case UTIL_GLOBAL_ODU3E1:
            *otuk_mode_ptr = DIGI_OTN_SERVER_OTU3E1;
            break;
            
          case UTIL_GLOBAL_ODU3E2:
            *otuk_mode_ptr = DIGI_OTN_SERVER_OTU3E2;
            break;
            
            
          case UTIL_GLOBAL_ODU4:
            *otuk_mode_ptr = DIGI_OTN_SERVER_OTU4;
            break;
            
          default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        }
        *deskew_type_ptr = (digi_sfis_deskew_type_t)lineotn_deskew_get(digi_handle->lineotn_handle, local_server_ptr->dsi_base_chnl);
    }
    else
    {
        *otuk_mode_ptr = LAST_DIGI_OTN_SERVER;
        *deskew_type_ptr = (digi_sfis_deskew_type_t)lineotn_deskew_get(digi_handle->sysotn_handle, local_server_ptr->dsi_base_chnl);
    }

    PMC_ATOMIC_RETURN(digi_handle);
} /* digi_otn_server_prov_get */


/*******************************************************************************
* digi_otn_server_ho_prov_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves the cfg_mode and payload_format set when the High Order 
*  channel was provisioned.
*
* INPUTS:
*  digi_handle           - pointer to DIGI handle instance
*  ho_chnl_ptr           - Reference to the LINEOTN/SYSOTN channel handle.
*
* OUTPUTS:
*  cfg_mode_ptr           - CFG mode. Should be 
*                           DIGI_OTN_DIRECT_DEVICE_CONFIG
*  payload_mode_ptr       - Payload mode. Either UNCHANNELIZED, or 
*                           CHANNELIZED 1G25 or 2G5
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_otn_server_ho_prov_get(digi_handle_t *digi_handle,
                                        digi_otn_server_chnl_t *ho_chnl_ptr,
                                        digi_otn_cfg_mode_t *cfg_mode_ptr,
                                        digi_odu_line_payload_t *payload_mode_ptr)
{
    odu_struct_t *local_chnl_ptr = NULL;

#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif
    PMC_ASSERT(ho_chnl_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(cfg_mode_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(payload_mode_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    
    local_chnl_ptr = ((digi_otn_server_chnl_def_t *)ho_chnl_ptr)->odu_struct_ptr;
    *cfg_mode_ptr = digi_handle->var.otn_cfg_mode;
    *payload_mode_ptr = (digi_odu_line_payload_t)local_chnl_ptr->mem_ptr->payload_format;

    PMC_ATOMIC_RETURN(digi_handle);
} /* digi_otn_server_ho_prov_get */


/*******************************************************************************
* digi_otn_server_lo_prov_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves the parameters used to provisioned the low order channel.
*
* INPUTS:
*  digi_handle           - pointer to DIGI handle instance
*  lo_chnl_ptr           - Reference to the Low order channel
*
* OUTPUTS:
*  cfg_mode_ptr           - CFG mode. Should be 
*                           DIGI_OTN_DIRECT_DEVICE_CONFIG
*  oduk_type_ptr          - Reference to the digi_oduk_t enumerand
*  client_bitrate_ptr     - Reference to the bitrate valued used.
*  map_mode_ptr           - Reference to the mapping mode enumerand.
*  trib_slot_mask         - Tributary slot mask used for this channel.
*  payload_mode_ptr       - Payload mode. Either UNCHANNELIZED, or 
*                           CHANNELIZED 1G25 or 2G5
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_otn_server_lo_prov_get(digi_handle_t *digi_handle,
                                        digi_otn_odu_chnl_t *lo_chnl_ptr,
                                        digi_otn_cfg_mode_t *cfg_mode_ptr,
                                        digi_oduk_t *oduk_type_ptr,
                                        DOUBLE *client_bitrate_ptr,
                                        digi_otn_mapping_type_t *map_mode_ptr,
                                        UINT32 trib_slot_mask[3],
                                        digi_odu_line_payload_t *payload_mode_ptr)
{
    odu_struct_t *local_chnl_ptr = NULL;
    UINT32 i;
#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif
    PMC_ASSERT(lo_chnl_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(cfg_mode_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(oduk_type_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(client_bitrate_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(map_mode_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(payload_mode_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    local_chnl_ptr = (odu_struct_t *)lo_chnl_ptr;

    *cfg_mode_ptr = DIGI_OTN_DIRECT_DEVICE_CONFIG;

    if (UTIL_GLOBAL_ODUFLEX_GFP == local_chnl_ptr->mem_ptr->odu_type ||
        UTIL_GLOBAL_ODUFLEX_CBR == local_chnl_ptr->mem_ptr->odu_type)
    {
        *oduk_type_ptr = DIGI_ODUK_ODUFLEX;
    }
    else
    {
        switch(local_chnl_ptr->mem_ptr->odu_type)
        {
          case UTIL_GLOBAL_ODU0:
            *oduk_type_ptr = DIGI_ODUK_ODU0;
            break;

          case UTIL_GLOBAL_ODU1:
            *oduk_type_ptr = DIGI_ODUK_ODU1;
            break;

          case UTIL_GLOBAL_ODU2:
            *oduk_type_ptr = DIGI_ODUK_ODU2;
            break;

          case UTIL_GLOBAL_ODU2E:
            *oduk_type_ptr =  DIGI_ODUK_ODU2E;
            break;

          case UTIL_GLOBAL_ODU1E:
            *oduk_type_ptr = DIGI_ODUK_ODU1E;
            break;

          case UTIL_GLOBAL_ODU2F:
            *oduk_type_ptr =  DIGI_ODUK_ODU2F;
            break;

          case UTIL_GLOBAL_ODU1F:
            *oduk_type_ptr =  DIGI_ODUK_ODU1F;
            break;

          case UTIL_GLOBAL_ODU3:
            *oduk_type_ptr =  DIGI_ODUK_ODU3;
            break;

          case UTIL_GLOBAL_ODU3E2:
            *oduk_type_ptr = DIGI_ODUK_ODU3E2;
            break;

          case UTIL_GLOBAL_ODU3E1:
            *oduk_type_ptr = DIGI_ODUK_ODU3E1;
            break;

          case UTIL_GLOBAL_ODU4:
            *oduk_type_ptr =  DIGI_ODUK_ODU4;
            break;
          default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
            break;
        }
    }
    *client_bitrate_ptr = local_chnl_ptr->mem_ptr->client_bitrate;

    switch (local_chnl_ptr->mem_ptr->mapping_mode)
    {
      case UTIL_GLOBAL_AMP:
        *map_mode_ptr = DIGI_OTN_MAP_AMP;
        break;
        
      case UTIL_GLOBAL_GMP:
        *map_mode_ptr = DIGI_OTN_MAP_GMP;
        break;
      default:
        *map_mode_ptr = LAST_DIGI_OTN_MAP;
    }

    for (i = 0; i < 3; i++)
    {
        trib_slot_mask[i] = local_chnl_ptr->mem_ptr->tx_trib_loc_mask[i];
    }

    *payload_mode_ptr = (digi_odu_line_payload_t)local_chnl_ptr->mem_ptr->payload_format;

    PMC_ATOMIC_RETURN(digi_handle);
}

/*******************************************************************************
* digi_enet_prov_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves the enet_mode assigned to the ENET channel handle when it was
*  provisioned. Applies to ENET_LINE or ENET_SYS channel handles.
*
* INPUTS:
*   digi_handle           - Pointer to DIGI handle instance
*   enet_chnl_ptr         - Reference to the ENET channel handle.
*
* OUTPUTS:
*   enet_mode_ptr         - ENET mode parameter assigned the current setting
*                           in enet_chnl_ptr
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_enet_prov_get(digi_handle_t *digi_handle,
                                digi_enet_client_chnl_t *enet_chnl_ptr,
                                digi_enet_mode_t *enet_mode_ptr)           
{
    digi_enet_client_chnl_def_t *local_enet_ptr = NULL;
#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif
    PMC_ASSERT(enet_chnl_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(enet_mode_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    local_enet_ptr = (digi_enet_client_chnl_def_t *)enet_chnl_ptr;
    *enet_mode_ptr = local_enet_ptr->mode;

    PMC_ATOMIC_RETURN(digi_handle);
}  /* digi_enet_prov_get */

/*******************************************************************************
* digi_cbr_client_prov_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves the enet_mode assigned to the CBRC channel handle when it was
*  provisioned.
*
* INPUTS:
*   digi_handle           - Pointer to DIGI handle instance
*   cbrc_chnl_ptr         - Reference to the CBRC channel handle.
*
* OUTPUTS:
*   cbrc_mode_ptr         - CBRC mode parameter assigned the current setting
*                           in cbrc_chnl_ptr
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_cbr_client_prov_get(digi_handle_t *digi_handle,
                                     digi_cbr_client_chnl_t *cbrc_chnl_ptr,
                                     digi_cbr_client_mode_t *cbrc_mode_ptr)
{
#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif
    PMC_ASSERT(cbrc_chnl_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(cbrc_mode_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    *cbrc_mode_ptr = ((digi_cbr_client_chnl_def_t *)cbrc_chnl_ptr)->mode;

    PMC_ATOMIC_RETURN(digi_handle);
}  /* digi_cbr_cLient_prov_get */


/*******************************************************************************
* digi_otn_mapper_prov_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves the mapping mode set when the MAPOTN channel was provisioned.
*  provisioned.
*
* INPUTS:
*   digi_handle           - Pointer to DIGI handle instance
*   map_chnl_ptr          - Reference to the MAPOTN channel handle.
*
* OUTPUTS:
*  primary_group_ptr     - Primary group setting.
*  secondary_group_ptr   - Secondary group setting
*  mapping_mode_ptr      - Mapping mode reference.
*  enet_pmon_ptr         - Flag indicating whether PMON has been enabled.
*  odu_data_ptr          - ODU container data set when the channel was
*                          provisioned.
*  odu_flex_data_ptr     - Number of ODU0 tributary slots making up the
*                          ODUFlex container.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_otn_mapper_prov_get(digi_handle_t *digi_handle,
                                     digi_mapper_chnl_t *map_chnl_ptr,
                                     digi_mapper_primary_group_t *primary_group_ptr,
                                     digi_mapper_secondary_group_t *secondary_group_ptr,
                                     digi_mapping_mode_t *mapping_mode_ptr,
                                     BOOL8 *enet_pmon_ptr,
                                     digi_oduk_t *odu_data_ptr,
                                     UINT32 *odu_flex_data_ptr)           
{
#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif
    PMC_ASSERT(map_chnl_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(mapping_mode_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    *mapping_mode_ptr = ((digi_mapper_chnl_def_t *)map_chnl_ptr)->mode;

    *primary_group_ptr = ((digi_mapper_chnl_def_t *)map_chnl_ptr)->primary_group;
    *secondary_group_ptr = ((digi_mapper_chnl_def_t *)map_chnl_ptr)->secondary_group; 

    if (DIGI_NO_PMON == ((digi_mapper_chnl_def_t *)map_chnl_ptr)->enet_pmon)
    {
        *enet_pmon_ptr = FALSE;
    }
    else
    {
        *enet_pmon_ptr = TRUE;
    }
    *odu_data_ptr = ((digi_mapper_chnl_def_t *)map_chnl_ptr)->odu_data;
    *odu_flex_data_ptr = ((digi_mapper_chnl_def_t *)map_chnl_ptr)->odu_flex_data;

    PMC_ATOMIC_RETURN(digi_handle);
}  /* digi_otn__mapper_prov_get */


/*******************************************************************************
* digi_ilkn_chnl_prov_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves the parameters used when the ILKN channel was provisioned.
*
* INPUTS:
*  digi_handle        - Pointer to DIGI handle instance
*  ilkn_chnl_ptr      - Reference to the ILKN channel handle.
*
* OUTPUTS:
*  is_oduk_switch     - Flag indicating whethe the ILKN channel handle is 
*                       routed through the ODUK switch(TRUE) or the CPB(FASE).
*  rate_ptr           - channel rate.  specified as an integer fraction of
*                        96. Used to allocate scheduling for signal
*                        transferred towards backplane.  The following must
*                        be true rate/96 >= (signal_rate Gpbs)/(120 Gbps)
*  enet_pmon_ptr      - Whether ENET PMON was enabled.
*  tx_type_header_ptr - Defines if the device appends the four byte TYPE
*                        field for data transfers across the ILKN interface.
*                        Valid values are: \n
*                          1 -- ILKN data tranfers have TYPE header \n
*                          0 -- ILKN data transfer have no TYPE header 
*  rx_type_header_ptr -  Specifies the presence of TYPE header on data sent
*                        from backplane to ILKN interface. Valid values are:
*                        1 -- ILKN data tranfers have TYPE header \n
*                        0 -- ILKN data transfer have no TYPE header
*  rx_fc_reach_extend_ptr - Configure data source (MAPOTN/ENET) to directly
*                           use ILKN XOFF indication for channel to 
*                           backpressure FE transmitter.  Used in cases 
*                           where large  downstream channel buffering is 
*                           present as required for reach extension
*                           applications. Valid values are:\n
*                           0 --  Reach Extension Flow Control disabled \n
*                           1 --  Reach Extension Flow Control enabled
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_ilkn_chnl_prov_get(digi_handle_t *digi_handle,
                                    digi_ilkn_chnl_t *ilkn_chnl_ptr,
                                    BOOL8 *is_oduk_switch,
                                    UINT8 *rate_ptr,
                                    BOOL8 *enet_pmon_ptr,
                                    BOOL8 *tx_type_header_ptr,
                                    BOOL8 *rx_type_header_ptr,
                                    BOOL8 *rx_fc_reach_extend_ptr)          
{
    digi_ilkn_chnl_def_t *ilkn_local_ptr = (digi_ilkn_chnl_def_t *)ilkn_chnl_ptr;
#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif
    PMC_ASSERT(ilkn_chnl_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    
    if (DIGI_NO_PMON == ilkn_local_ptr->enet_pmon)
    {
        *enet_pmon_ptr = FALSE;
    }
    else
    {
        *enet_pmon_ptr = TRUE;
    }
    /*
     * tx and rx type headers are set only for cpb channel provisions and 
     * not oduksw. 
     */
    *tx_type_header_ptr = ilkn_local_ptr->tx_type_header;
    *rx_type_header_ptr = ilkn_local_ptr->rx_type_header;
    *rate_ptr = ilkn_local_ptr->rate;
    *rx_fc_reach_extend_ptr = ilkn_local_ptr->rx_fc_reach_extend;
    if (DIGI_ILKN_CHNL_TARGET_CPB == ilkn_local_ptr->target)
    {
        *is_oduk_switch = FALSE;
    }
    else
    {
        *is_oduk_switch = TRUE;
    }

    PMC_ATOMIC_RETURN(digi_handle);
} /* digi_ilkn_chnl_prov_get */




/*******************************************************************************
* digi_resource_flow_control_calendar_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Returns the flow control calendars for TX and RX ODUK channels for the
*  specified ILKN type based on what was configured in function digi_ilkn_oduk_chnl_prov.
*  Each flow control calendar entry contains:
*       a. If an ILKN channel's BCW or 2B header was provided in digi_ilkn_oduk_chnl_prov:
*          1. If that channel is activated - returns the BCW or 2B header in the calendar entry.
*          2. Otherwise (channel is de-activated) - SIFD_ILKN_NULL_CAL_ENTRY
*       b. If SIFD_ILKN_LINK_FLOW_CONTROL/SIFD_ILKN_NULL_CAL_ENTRY value was provided in
*          digi_ilkn_oduk_chnl_prov - returns same value.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ilkn_type              - ILKN Interface 1 or 2
*
* OUTPUTS:
*   tx_ilkn_fc_ptr  - Reference to the Transmit flow control calendar that
*                     that will be populated from the ILKN interface type.
*   rx_ilkn_fc_ptr  - Reference to the Receive flow control calendar that
*                     that will be populated from the ILKN interface type.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_resource_flow_control_calendar_get(digi_handle_t *digi_handle,
                                                    digi_ilkn_intf_t ilkn_type,
                                                    digi_sifd_ilkn_calendar32_t *tx_ilkn_fc_ptr,
                                                    digi_sifd_ilkn_calendar32_t *rx_ilkn_fc_ptr)
{
    UINT32 i;
    sifd_ilkn_calendar_t sifd_ilkn_tx_fc;
    sifd_ilkn_calendar_t sifd_ilkn_rx_fc;

    /* prepare resources for non 2B header */
    UINT8 * tx_ilkn_bcw_dpi_map = NULL;
    UINT8 * rx_ilkn_bcw_dpi_map = NULL;
    sifd_ilkn_inst_t ilkn_inst = LAST_SIFD_ILKN;
    digi_sifd_ilkn_cfg_t* ilkn;

#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif
    /*
     * Sanity check
     */
    if (NULL == tx_ilkn_fc_ptr || NULL == rx_ilkn_fc_ptr || 
        ilkn_type > DIGI_ILKN2)
    {
        PMC_LOG_TRACE("Parameters Values Incorrect:\n"
                      "\ttk_ilkn_fc_ptr=%p\n\trx_ilkn_fc_ptr=%p\n"
                      "\tilkn_type=%d\n", tx_ilkn_fc_ptr, rx_ilkn_fc_ptr,
                      ilkn_type);
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }

    /* select the right interlaken */
    if (DIGI_ILKN1 == ilkn_type)
    {
        ilkn_inst =           SIFD_COPI_ILKN1;
        ilkn = 	              &(digi_handle->var.sifd_ilkn_1_cfg.ilkn);
        tx_ilkn_bcw_dpi_map = digi_handle->var.tx_ilkn_1_bcw_dpi_map;
        rx_ilkn_bcw_dpi_map = digi_handle->var.rx_ilkn_1_bcw_dpi_map;
    }
    else
    {
        ilkn_inst =           SIFD_COPI_ILKN2;
        ilkn = 	              &(digi_handle->var.sifd_ilkn_2_cfg.ilkn);
        tx_ilkn_bcw_dpi_map = digi_handle->var.tx_ilkn_2_bcw_dpi_map;
        rx_ilkn_bcw_dpi_map = digi_handle->var.rx_ilkn_2_bcw_dpi_map;
    }

    if(ilkn->oduksw_header == DIGI_SIFD_ODUKSW_ILKN_HEADER_2B &&
       ilkn->oduksw_2b_header_mode == 0 &&
       ilkn->oduksw_intf_en == 1)
    {
        /* tx calendary entries */
        digi_sifd_flow_control_calendar_2B_header_get(digi_handle,
                                                      ilkn_inst,
                                                      TRUE, /* is tx */
                                                      tx_ilkn_fc_ptr);

        /* rx calendary entries */
        digi_sifd_flow_control_calendar_2B_header_get(digi_handle,
                                                      ilkn_inst,
                                                      FALSE, /* is rx */
                                                      rx_ilkn_fc_ptr);

    }
    else /* Not using ODUkSW 2B header mode 0 */
    {
        sifd_flow_control_calendar_get(digi_handle->sifd_handle,
                                       ilkn_inst,
                                       tx_ilkn_bcw_dpi_map,
                                       rx_ilkn_bcw_dpi_map,
                                       &sifd_ilkn_tx_fc, &sifd_ilkn_rx_fc);

        /* convert to 32 bit */
        for(i=0 ; i<SIFD_ILKN_MAX_CAL_ENTRIES ; i++)
        {
            tx_ilkn_fc_ptr->entry[i] = (UINT32)sifd_ilkn_tx_fc.entry[i];
            rx_ilkn_fc_ptr->entry[i] = (UINT32)sifd_ilkn_rx_fc.entry[i];
        }
    }

    PMC_ATOMIC_RETURN(digi_handle);
} /*digi_resource_flow_control_calendar_get*/


/*******************************************************************************
* digi_chnl_hndl_map_act_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function retrieves the data associated with the mapping and activation
*   state of a channel handle. \n\n
*
*   If the channel handle type does not have CPB or ODUKSW data then the
*   function returns DIGI_ERR_NO_SWITCH_DATA, this could be in cases such as
*   when a HO channel handle is configured for tributary slot payload and does
*   not directly connect to ODUKSW. \n\n
*
*   The API retrieves the channel handle data for the source of data across
*   ODUKSW or CPB for this channel.  If the data source is BC/MC it also returns
*   the set of channel handles for these companion channels.\n\n
*
*   The API also retrieves the primary destination that channel is sending data
*   to across the ODUKSW/CPB.  In the event that BC/MC is used the multiple
*   destinations are provided.\n\n
*
*   The primary indications are important because channel tear down requires
*   non-primary to be deactivation to be performed for non-primary channels
*   first with primary channel last.
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
*                            into ODUKSW/CPB
*   *multi_dest_ptr         - other destinations this subsystem transmitting
*                            into ODUKSW/CPB
*    
* RETURNS:
*   DIGI_ERR_INVALID_ARG
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_chnl_hndl_map_act_state_get(digi_handle_t *digi_handle,
                                                  digi_handle_id_t chnl_hndl_type,
                                                  digi_chnl_hndl_t *chnl_hndl_ptr,
                                                  digi_mapotn_conn_use_t mapotn_use,
                                                  digi_chnl_hndl_conn_t *source_ptr,
                                                  BOOL8 *companion_source_ptr,
                                                  digi_chnl_hndl_conn_t *primary_dest_ptr,
                                                  digi_chnl_hndl_conn_t multi_dest_ptr[DIGI_RESOURCE_MULTI_DEST_MAX])
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    if (DIGI_CHANNEL_HANDLE_ID_LINEOTN_T == chnl_hndl_type ||
        DIGI_CHANNEL_HANDLE_ID_SYSOTN_T == chnl_hndl_type ||
        DIGI_CHANNEL_HANDLE_ID_LINE_SERDES_T == chnl_hndl_type ||
        DIGI_CHANNEL_HANDLE_ID_SYS_SERDES_T == chnl_hndl_type ||
        DIGI_CHANNEL_HANDLE_ID_SFI51_SERDES_T == chnl_hndl_type)
    {
        result = DIGI_ERR_NO_SWITCH_DATA;
    }
    else if ((DIGI_CHANNEL_HANDLE_ID_HO_ODU_T == chnl_hndl_type ||
              DIGI_CHANNEL_HANDLE_ID_MO_ODU_T == chnl_hndl_type) &&
             UTIL_GLOBAL_ODU_UNCHANNELIZED != ((odu_struct_t *)chnl_hndl_ptr)->mem_ptr->payload_format)
    {
        result = DIGI_ERR_NO_SWITCH_DATA;
    }
    else if ((DIGI_CHANNEL_HANDLE_ID_MAPPER_T == chnl_hndl_type &&
              !(DIGI_MAPOTN_CONN_USE_CPB_INTF == mapotn_use ||
                DIGI_MAPOTN_CONN_USE_ODUKSW_INTF == mapotn_use)) ||
             !(DIGI_CHANNEL_HANDLE_ID_MAPPER_T == chnl_hndl_type ||
               DIGI_MAPOTN_CONN_USE_NOT_MAPOTN == mapotn_use))
    {
        /*
         * If this is a MAPOTN handle type and the mapotn_use flag is not 
         * CPB or ODUK or this is some other handle type and mapotn_use is
         * NOT_MAPOTN then return an error.
         */
        result = DIGI_ERR_INVALID_ARG;
    }
    else if (DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T == chnl_hndl_type ||
             DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T  == chnl_hndl_type ||
             DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T == chnl_hndl_type ||
             (DIGI_CHANNEL_HANDLE_ID_MAPPER_T == chnl_hndl_type &&
              DIGI_MAPOTN_CONN_USE_CPB_INTF == mapotn_use) ||
             ((DIGI_CHANNEL_HANDLE_ID_ILKN_1_T == chnl_hndl_type || 
               DIGI_CHANNEL_HANDLE_ID_ILKN_2_T == chnl_hndl_type) &&
              (DIGI_ILKN_CHNL_TARGET_CPB == ((digi_ilkn_chnl_def_t *)chnl_hndl_ptr)->target)))
    {
        digi_resource_populate_cpb_switch_data(digi_handle, chnl_hndl_type,
                                               chnl_hndl_ptr, mapotn_use, 
                                               source_ptr, 
                                               companion_source_ptr,
                                               primary_dest_ptr, 
                                               multi_dest_ptr);
    }
    else
    {
        digi_resource_populate_oduk_switch_data(digi_handle, chnl_hndl_type,
                                                chnl_hndl_ptr, mapotn_use, 
                                                source_ptr, 
                                                companion_source_ptr,
                                                primary_dest_ptr, 
                                                multi_dest_ptr);
    }

    if (PMC_SUCCESS == result)
    {
      /*
        result = digi_resource_read_connection_data(
      */
    }
    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_chnl_hndl_map_act_state_get */

/*******************************************************************************
* digi_resource_is_channel_provisioned
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Checks the state of the chnl_hndl_ptr based on the chnl_hndl_type and 
*  returns TRUE if it is PROVISIONED. FALSE otherwise.  Verifies the pased
*  in pointer is in range based on its handle type before checking the 
*  provisioning state.
*
*
* INPUTS:
*  *digi_handle        - pointer to DIGI handle instance
*  chnl_hndl_type      - Identifies the handle type being queried.
*  chnl_hndl_ptr       - Reference to the channel handle being queried
*
* OUTPUTS:
*   None
*
* RETURNS:
*   TRUE  - Channel handle is provisioned
*   FALSE - Otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 digi_resource_is_channel_provisioned(digi_handle_t *digi_handle,
                                                  digi_handle_id_t chnl_hndl_type,
                                                  digi_chnl_hndl_t *chnl_hndl_ptr)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;

#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif

    if (NULL == chnl_hndl_ptr)
    {
        PMC_ATOMIC_RETURN(digi_handle, FALSE);
    }

    switch (chnl_hndl_type)
    {
      case DIGI_CHANNEL_HANDLE_ID_LINEOTN_T:
      case DIGI_CHANNEL_HANDLE_ID_SYSOTN_T:
        result = digi_otn_server_chnl_handle_validate(digi_handle, 
                                                      chnl_hndl_ptr);

        if (PMC_SUCCESS == result)
        {
            /*
             * Check the provsioning state here as this channel handle will
             * not convert to a util_global structure.
             */
            if ( UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED ==
                 ((digi_otn_server_chnl_def_t *)chnl_hndl_ptr)->header.prov_state)
            {
                PMC_ATOMIC_RETURN(digi_handle, TRUE);
            }
            result = DIGI_ERR_ODUKSW_CHNL_NOT_PROV;
        }

        break;

      case DIGI_CHANNEL_HANDLE_ID_HO_ODU_T:
        result = digi_ho_odu_struct_handle_validate(digi_handle, 
                                                    chnl_hndl_ptr);
        /*
         * If a SYSOTN application it may be the channel handle is actually
         * in the lower order handles. If the ho check failed then look
         * there
         */
        if (PMC_SUCCESS != result &&
            DIGI_PROD_APP_SYSOTN_CARD == digi_handle->var.prod_app)
        {
            result = digi_lo_odu_struct_handle_validate(digi_handle, 
                                                    chnl_hndl_ptr);
        }
        break;

      case DIGI_CHANNEL_HANDLE_ID_MO_ODU_T:
        result = digi_mo_odu_struct_handle_validate(digi_handle, 
                                                    chnl_hndl_ptr);
        break;

      case DIGI_CHANNEL_HANDLE_ID_LO_ODU_T:
        result = digi_lo_odu_struct_handle_validate(digi_handle, 
                                                    chnl_hndl_ptr);
        break;

      case DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T:
        result = digi_cbr_chnl_handle_validate(digi_handle, chnl_hndl_ptr);
        break;

      case DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T:
        result = digi_enet_line_chnl_handle_validate(digi_handle, 
                                                     chnl_hndl_ptr);
        break;

      case DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T:
        result = digi_enet_sys_chnl_handle_validate(digi_handle, 
                                                     chnl_hndl_ptr);
        break;

      case DIGI_CHANNEL_HANDLE_ID_MAPPER_T:
        result = digi_mapper_chnl_handle_validate(digi_handle, 
                                                  chnl_hndl_ptr);
        break;

      case DIGI_CHANNEL_HANDLE_ID_ILKN_1_T:
      case DIGI_CHANNEL_HANDLE_ID_ILKN_2_T:
        result = digi_ilkn_chnl_handle_validate(digi_handle, chnl_hndl_ptr);
        break;


      default:
        break;
    }

    if (PMC_SUCCESS == result)
    {
        util_global_switch_data_def_t *switch_data_ptr = (util_global_switch_data_def_t *)chnl_hndl_ptr;
        
        if (UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED == switch_data_ptr->header.prov_state)
        {
            PMC_ATOMIC_RETURN(digi_handle, TRUE);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle, FALSE);
} /* digi_resource_is_channel_provisioned */


/*******************************************************************************
* digi_resource_is_channel_activated
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Checks the state of the chnl_hndl_ptr based on the chnl_hndl_type and 
*  returns TRUE if it is ACTIVATED at the specified switch. That is at the
*  CPB if is_oduk_switch is FALSE and the ODUK switch otherwise.
*  Verifies the channel handle pointer is in range based on its handle type
*  before checking activation state.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *digi_resource_handle  - Resource handle table 
*   chnl_handle_ptr        - Reference to the channel handle being queried
*   chnl_handle_type       - Identifies the handle type being queried.
*   is_oduk_switch         - Boolean indicating the switch type. TRUE is
*                            ODUK, FALSE indicates CPB
*
* OUTPUTS:
*   None
*
* RETURNS:
*   TRUE  - Current resource is activated on the specific switch.
*   FALSE - Otherwise not activated.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 digi_resource_is_channel_activated(digi_handle_t *digi_handle,
                                                digi_resource_handle_summary_t *digi_resource_handle,
                                                void *chnl_handle_ptr,
                                                digi_handle_id_t chnl_handle_type,
                                                BOOL8 is_oduk_switch)
{
    PMC_ERROR rc;
    BOOL8 result = FALSE;
#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif

    if (NULL == chnl_handle_ptr)
    {
        PMC_ATOMIC_RETURN(digi_handle, result);
    }

    DIGI_SWITCH_DATA_HANDLE_CHECK(digi_handle, chnl_handle_ptr, rc);
    
    if (PMC_SUCCESS != rc)
    {
        PMC_ATOMIC_RETURN(digi_handle, result);
    }

    switch (chnl_handle_type)
    {
      case DIGI_CHANNEL_HANDLE_ID_MAPPER_T:
        {
            digi_mapper_chnl_def_t *map_handle_ptr = (digi_mapper_chnl_def_t *)chnl_handle_ptr;
            
            if (TRUE == is_oduk_switch)
            {
                if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE ==
                    map_handle_ptr->switch_data.oduk_data.op_state.active_state)
                {
                    result = TRUE;
                }
            }
            else if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE ==
                     map_handle_ptr->switch_data.cpb_data.op_state.active_state)
            {
                result = TRUE;
            }
        }

      case DIGI_CHANNEL_HANDLE_ID_ILKN_1_T:
      case DIGI_CHANNEL_HANDLE_ID_ILKN_2_T:
        {
            digi_ilkn_chnl_def_t *ilkn_handle_ptr = (digi_ilkn_chnl_def_t *)chnl_handle_ptr;

            if (TRUE == is_oduk_switch)
            {
                if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE ==
                    ilkn_handle_ptr->switch_data.oduk_data.op_state.active_state)
                {
                    result = TRUE;
                }
            }
            else if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE ==
                     ilkn_handle_ptr->switch_data.cpb_data.op_state.active_state)
            {
                result = TRUE;
            }
        }
        break;

      case DIGI_CHANNEL_HANDLE_ID_HO_ODU_T:
      case DIGI_CHANNEL_HANDLE_ID_MO_ODU_T:
      case DIGI_CHANNEL_HANDLE_ID_LO_ODU_T:
        {
            odu_struct_t *otn_handle_ptr = (odu_struct_t *)chnl_handle_ptr;

            if (TRUE == is_oduk_switch)
            {
                if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE ==
                    otn_handle_ptr->switch_data.oduk_data.op_state.active_state)
                {
                    result = TRUE;
                }
            }
            else if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE ==
                     otn_handle_ptr->switch_data.cpb_data.op_state.active_state)
            {
                result = TRUE;
            }          
        }
        break;

      case DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T:
      case DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T:
        {
            digi_enet_client_chnl_def_t *enet_handle_ptr = (digi_enet_client_chnl_def_t *)chnl_handle_ptr;

            if (TRUE == is_oduk_switch)
            {
                if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE ==
                    enet_handle_ptr->switch_data.oduk_data.op_state.active_state)
                {
                    result = TRUE;
                }
            }
            else if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE ==
                     enet_handle_ptr->switch_data.cpb_data.op_state.active_state)
            {
                result = TRUE;
            }
        }
        break;

      case DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T:
        {
            digi_cbr_client_chnl_def_t *cbr_handle_ptr = (digi_cbr_client_chnl_def_t *)chnl_handle_ptr;

            if (TRUE == is_oduk_switch)
            {
                if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE ==
                    cbr_handle_ptr->switch_data.oduk_data.op_state.active_state)
                {
                    result = TRUE;
                }
            }
            else if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE ==
                     cbr_handle_ptr->switch_data.cpb_data.op_state.active_state)
            {
                result = TRUE;
            }
        }
        break;

      default:
        break;
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_resource_is_channel_activated */


/*******************************************************************************
* digi_resource_is_channel_mapped
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Checks the state of the chnl_hndl_ptr based on the chnl_hndl_type and 
*  returns TRUE if it is MAPPED at the specified switch. That is at the
*  CPB if is_oduk_switch is FALSE and the ODUK switch otherwise.
*  Verifies the channel handle pointer is in range based on its handle type
*  before checking mapping state.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *digi_resource_handle  - Resource handle table 
*   *chnl_handle_ptr       - Channel handle used to find the particular 
*                            resource being queried.
*   chnl_handle_type       - Resource type being queried.
*   is_oduk_switch         - Boolean indicating whether this query is on
*                            ODUK or CPB switch. 
*
* OUTPUTS:
*   None
*
* RETURNS:
*   TRUE       - Channel is mapped on the specified switched
*   FALSE      - Channel is not mapped.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC BOOL8 digi_resource_is_channel_mapped(digi_handle_t *digi_handle,
                                             digi_resource_handle_summary_t *digi_resource_handle,
                                             void *chnl_handle_ptr,
                                             digi_handle_id_t chnl_handle_type,
                                             BOOL8 is_oduk_switch)
{
    PMC_ERROR rc;
    BOOL8 result = FALSE;
#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif

    if (NULL == chnl_handle_ptr)
    {
        PMC_ATOMIC_RETURN(digi_handle, result);
    }

    DIGI_SWITCH_DATA_HANDLE_CHECK(digi_handle, chnl_handle_ptr, rc);
    
    if (PMC_SUCCESS != rc)
    {
        PMC_ATOMIC_RETURN(digi_handle, result);
    }

    switch (chnl_handle_type)
    {
      case DIGI_CHANNEL_HANDLE_ID_MAPPER_T:
        {
            digi_mapper_chnl_def_t *map_handle_ptr = (digi_mapper_chnl_def_t *)chnl_handle_ptr;
            
            if (TRUE == is_oduk_switch)
            {
                result = map_handle_ptr->switch_data.oduk_data.op_state.map_state.is_output_mapped;
            }
            else
            {
                result = map_handle_ptr->switch_data.cpb_data.op_state.map_state.is_output_mapped;
            }
        }

      case DIGI_CHANNEL_HANDLE_ID_ILKN_1_T:
      case DIGI_CHANNEL_HANDLE_ID_ILKN_2_T:
        {
            digi_ilkn_chnl_def_t *ilkn_handle_ptr = (digi_ilkn_chnl_def_t *)chnl_handle_ptr;

            if (TRUE == is_oduk_switch)
            {
                result = ilkn_handle_ptr->switch_data.oduk_data.op_state.map_state.is_output_mapped;
            }
            else
            {
                result = ilkn_handle_ptr->switch_data.cpb_data.op_state.map_state.is_output_mapped;
            }
        }
        break;

      case DIGI_CHANNEL_HANDLE_ID_HO_ODU_T:
      case DIGI_CHANNEL_HANDLE_ID_MO_ODU_T:
      case DIGI_CHANNEL_HANDLE_ID_LO_ODU_T:
        {
            odu_struct_t *otn_handle_ptr = (odu_struct_t *)chnl_handle_ptr;

            if (TRUE == is_oduk_switch)
            {
                result = otn_handle_ptr->switch_data.oduk_data.op_state.map_state.is_output_mapped;
            }
            else
            {
                result = otn_handle_ptr->switch_data.cpb_data.op_state.map_state.is_output_mapped;
            }          
        }
        break;

      case DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T:
      case DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T:
        {
            digi_enet_client_chnl_def_t *enet_handle_ptr = (digi_enet_client_chnl_def_t *)chnl_handle_ptr;

            if (TRUE == is_oduk_switch)
            {
                result = enet_handle_ptr->switch_data.oduk_data.op_state.map_state.is_output_mapped;
            }
            else
            {
                result = enet_handle_ptr->switch_data.cpb_data.op_state.map_state.is_output_mapped;
            }
        }
        break;

      case DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T:
        {
            digi_cbr_client_chnl_def_t *cbr_handle_ptr = (digi_cbr_client_chnl_def_t *)chnl_handle_ptr;

            if (TRUE == is_oduk_switch)
            {
                result = cbr_handle_ptr->switch_data.oduk_data.op_state.map_state.is_output_mapped;
            }
            else
            {
                result = cbr_handle_ptr->switch_data.cpb_data.op_state.map_state.is_output_mapped;
            }
        }
        break;

      default:
        break;
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_resource_is_channel_mapped */

/*******************************************************************************
* digi_serdes_port_release 
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Releases resources connected from the specifie serdes port up to the
*  first sowtching element if the switch_release is FALSE. Otherwise keeps
*  datapath intact and decouples it from the switch if switch_release is
*  TRUE. If the datapath contains a MAPOTN channel resource then it will 
*  release that resource beyond the switch as well if mapotn_release is 
*  TRUE.
*
* INPUTS:
*  *digi_handle    - pointer to DIGI handle instance
*  *port_ctxt_ptr  - Port context data pointer.
*  switch_release  - Flag that indicates whether the data path starting at
*                    port_ctxt_ptr should remain intact and then only 
*                    decoupled from the switch. Namely demap/deactivated at
*                    switching element only.
*  mapotn_release  - Flag indicating that the MAPOTN resource if on the
*                    datapath should be released as well.  Can be used in
*                    conjunction with switch_release.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS    - Release operations was successful.
*   PMC_ERROR   - There was an error either deactivating. demapping or
*                 deprovisioning. Datapath is in an indeterminate state.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_port_release(digi_handle_t *digi_handle,
                                          digi_serdes_port_t *port_ctxt_ptr,
                                          BOOL8 switch_release,
                                          BOOL8 mapotn_release)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    digi_serdes_port_def_t *local_ctxt_ptr = NULL;
    BOOL8 serdes_prov_status;
    digi_serdes_port_mode_t serdes_port_mode;
    digi_serdes_prov_state_t serdes_prov_state;
    digi_chnl_hndl_t *chnl_hndl_ptr = NULL;
    digi_handle_id_t chnl_hndl_type = LAST_DIGI_HANDLE_ID;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != port_ctxt_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    local_ctxt_ptr = (digi_serdes_port_def_t *)port_ctxt_ptr;

    result = digi_serdes_port_rate_prov_status_internal_get(digi_handle,
                                                            port_ctxt_ptr,
                                                            FALSE,
                                                            &serdes_prov_status,
                                                            &serdes_port_mode,
                                                            &serdes_prov_state);

    if (PMC_SUCCESS == result)
    {
        if (DIGI_SERDES_UNASSIGNED == local_ctxt_ptr->port_uid ||
            DIGI_SERDES_PROV_STATE_NO_SERDES == serdes_prov_state ||
            DIGI_SERDES_PROV_STATE_UNASSIGNED == serdes_prov_state)
        {
            result = DIGI_ERR_SERDES_PORT_NOT_PROV;
        }
    }

    if (PMC_SUCCESS == result)
    {
        result = digi_resource_get_serdes_channel_handle(digi_handle,
                                                         local_ctxt_ptr,
                                                         serdes_prov_state,
                                                         &chnl_hndl_type,
                                                         &chnl_hndl_ptr);
    }

    if (PMC_SUCCESS == result)
    {
        result = digi_resource_internal_serdes_port_release(digi_handle,
                                                            chnl_hndl_type,
                                                            chnl_hndl_ptr,
                                                            switch_release,
                                                            mapotn_release);
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_serdes_port_release */


/*******************************************************************************
* digi_bp_intf_chnl_release
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*  Releases resources starting at the specified back plane channel up to the
*  ODUKSW or CPB. Parameters switch_release and mapotn_release can
*  be used to modify this behavior.\n\n
*
*  Channel type must be of type: DIGI_CHANNEL_HANDLE_ID_ILKN_CHNL_T.
*
*  The switch_release=TRUE leaves ILKN channel in provisioned 
*  and performs demapping and deactivating of the ODUKSW or CPB only. \n\n
*
*  The mapotn_release flag if TRUE performs MAPOTN channel deprovisioning,
*  for any MAPOTN channels that are connected via ODUKSW or CPB to the
*  backplane channel \n\n
*
* INPUTS:
*  *digi_handle    - pointer to DIGI handle instance
*  chnl_hndl_type  - Backplane channel handle type
*  chnl_hndl_ptr   - Reference to the back plane channel handle.
*  switch_release  - Flag if TRUE keeps the datapath intact and releases it
*                    at the switching point only. 
*  mapotn_release  - Releases mapoton resource if TRUE and in the datapath.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS    - Release operations was successful.
*   PMC_ERROR      - There was an error either deactivating. demapping or
*                    deprovisioning. Datapath is in an indeterminate state.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_bp_intf_chnl_release(digi_handle_t *digi_handle,
                                           digi_handle_id_t chnl_hndl_type,
                                           digi_chnl_hndl_t *chnl_hndl_ptr,
                                           BOOL8 switch_release,
                                           BOOL8 mapotn_release)
{
    PMC_ERROR result = PMC_SUCCESS;
    digi_ilkn_chnl_def_t *ilkn_hndl_ptr = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != chnl_hndl_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(chnl_hndl_type < LAST_DIGI_HANDLE_ID,  DIGI_ERR_INVALID_ARG, 0, 0);

    switch (chnl_hndl_type)
    {
      case DIGI_CHANNEL_HANDLE_ID_ILKN_1_T:
      case DIGI_CHANNEL_HANDLE_ID_ILKN_2_T:
          ilkn_hndl_ptr = (digi_ilkn_chnl_def_t *)chnl_hndl_ptr;
          /*
           * If the state is not provisioned there is nothing to do.
           */
          if (FALSE == util_global_rx_or_tx_chnl_prov_status_get(ilkn_hndl_ptr->switch_data.header.prov_state))
          {
              result = DIGI_ERR_ILKN_CHNL_NOT_PROV;
              break;
          }
          /*
           * Now need to determine on which switch this resource is
           * configured.
           */
          if (DIGI_ILKN_CHNL_TARGET_CPB == ilkn_hndl_ptr->target)
          {
              cpb_connect_data_state_get(digi_handle->dcpb_handle,
                                         &g_cpb_connect_data_ptr);

              PMC_ASSERT(ilkn_hndl_ptr->switch_data.cpb_data.port_type < UTIL_GLOBAL_CPB_PORT_NOT_USED, DIGI_ERR_INVALID_ARG, 0, 0);
              result = digi_resource_release_cpb_resource(digi_handle,
                                                          chnl_hndl_ptr,
                                                          ilkn_hndl_ptr->switch_data.cpb_data.port_type,
                                                          ilkn_hndl_ptr->switch_data.cpb_data.channel,
                                                          switch_release,
                                                          mapotn_release,
                                                          g_cpb_connect_data_ptr);
          }
          else if (DIGI_ILKN_CHNL_TARGET_ODUK == ilkn_hndl_ptr->target)
          {
              UINT32 chnl_id = ilkn_hndl_ptr->switch_data.oduk_data.channel;
              coreotn_oduksw_connect_data_state_get(digi_handle->coreotn_handle,
                                                    &g_oduk_connect_data_ptr);

              if (UTIL_GLOBAL_CPB_PORT_ILKN1 == (util_global_cpb_port_t)ilkn_hndl_ptr->switch_data.oduk_data.port_type &&
                  chnl_id >=  DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX)
              {
                  chnl_id -= DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;

              }
              result = digi_resource_release_oduk_resource(digi_handle,
                                                           chnl_hndl_ptr,
                                                           ilkn_hndl_ptr->switch_data.oduk_data.port_type,
                                                           chnl_id,
                                                           switch_release,
                                                           mapotn_release,
                                                           g_oduk_connect_data_ptr);
                                                          
          }
          else
          {
              PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
          }

          if (PMC_SUCCESS == result && FALSE == switch_release)
          {
              digi_sifd_ilkn_calendar32_t tx_ilkn_fc;
              digi_sifd_ilkn_calendar32_t rx_ilkn_fc;
              digi_ilkn_intf_t ilkn_inst = DIGI_ILKN2;

              if (DIGI_CHANNEL_HANDLE_ID_ILKN_1_T  == chnl_hndl_type)

              {
                  ilkn_inst = DIGI_ILKN1;
              }

              if (DIGI_CHANNEL_HANDLE_ID_ILKN_2_T  == chnl_hndl_type)
              {
                  ilkn_inst = DIGI_ILKN2;
              }
              /*
               * If here the ILKN handle has been provisioned but is neither
               * mapped or activated. Simply need to determine its target
               * and then release it
               */
              digi_resource_flow_control_calendar_get(digi_handle,
                                                      ilkn_inst,
                                                      &tx_ilkn_fc,
                                                      &rx_ilkn_fc);

              if (DIGI_ILKN_CHNL_TARGET_ODUK == ilkn_hndl_ptr->target)
              {
                  result = digi_ilkn_oduk_chnl_deprov(digi_handle,
                                                      ilkn_hndl_ptr,
                                                      &tx_ilkn_fc,
                                                      &rx_ilkn_fc);
              }
              else
              {
                  result = digi_ilkn_cpb_chnl_deprov(digi_handle,
                                                     ilkn_hndl_ptr,
                                                     &tx_ilkn_fc,
                                                     &rx_ilkn_fc);
              }
          }
          break;

      default:
        result = DIGI_ERR_INVALID_ARG;
        break;
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_bp_intf_chnl_release */


/*******************************************************************************
* digi_trib_slot_chnl_release
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Releases a set of lower order sub container channels used under a 
*  specified higher channel handle based on the tributary time slots. All 
*  lower order channels using the time slots are demapped/deactivated and 
*  deprovisioned. Higher order channel is otherwise unaffected. Parameters 
*  switch_release and mapotn_release can be used to modify this behavior.\n\n
*
*  The switch_release=TRUE leaves subcontainer channels provisioned 
*  and performs demapping and deactivating of the ODUKSW or CPB only. \n\n
*
*  The mapotn_release flag if TRUE performs MAPOTN channel deprovisioning,
*  for any MAPOTN channels that are connected via ODUKSW or CPB to the
*  subcontainer channels \n\n
*
* INPUTS:
*  *digi_handle      - pointer to DIGI handle instance
*  ho_chnl_hndl_type - Handle type of the trib slot owner. Either HO or MO 
*                      ODU value.
*  ho_chnl_hndl_ptr  - Reference to the higher order channel whose lower 
*                      order channels are being release. Only 
*                      DIGI_CHANNEL_HANDLE_ID_HO_ODU_T and
*                      DIGI_CHANNEL_HANDLE_ID_HO_ODU_T are valid.
*  tx_trib_slot    - The tx tributary slot mask to be released.
*  rx_trib_slot    - The rx tributary slot mask to be released.
*  switch_release  - Flag if TRUE keeps the datapath intact and releases it
*                    at the switching point only. 
*  mapotn_release  - Releases mapoton resource if TRUE and in the datapath.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS    - Release operations was successful.
*   PMC_ERROR      - There was an error either deactivating. demapping or
*                    deprovisioning. Datapath is in an indeterminate state.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_trib_slot_chnl_release(digi_handle_t *digi_handle,
                                             digi_handle_id_t ho_chnl_hndl_type,
                                             digi_chnl_hndl_t *ho_chnl_hndl_ptr,
                                             UINT32 tx_trib_slot[3],
                                             UINT32 rx_trib_slot[3],
                                             BOOL8 switch_release,
                                             BOOL8 mapotn_release)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 ho_cal_array[DIGI_OTN_SERVER_MO_CHNL_MAX];
    UINT32 num_cal_entries = 0;
    digi_otn_time_slot_occupied_t slot_use;
    digi_otn_odu_chnl_t *lo_chnl_hndl_ptr[DIGI_OTN_SERVER_MAX_TRIBSLOT];
    coreotn_odu_chnl_prov_mode_t initial_prov_mode = LAST_COREOTN_CHNL_PROV_MODE;
    /*
     * Specific flag to ensure the mode is reset to the original before
     * leaving if it was updated at the beginning
     */
    BOOL8 mode_updated = FALSE;
    /*
     * If the MO or HO channel passed in is itself unchannelized then want
     * to ensure that switch related connections are handled but the actual
     * channel is deprovisioned. That is handled by the caller.
     * The release operators only deprovision subcontainers under the passed
     * in higher order container.
     */
    BOOL8 lo_deprov = TRUE;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != ho_chnl_hndl_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(ho_chnl_hndl_type < LAST_DIGI_HANDLE_ID,  DIGI_ERR_INVALID_ARG, 0, 0);

    if (!(DIGI_CHANNEL_HANDLE_ID_HO_ODU_T == ho_chnl_hndl_type ||
          DIGI_CHANNEL_HANDLE_ID_MO_ODU_T == ho_chnl_hndl_type))
    {
        PMC_ATOMIC_RETURN(digi_handle, DIGI_ERR_INVALID_ARG);
    }

    if (FALSE == switch_release && UTIL_GLOBAL_ODU_UNCHANNELIZED != ((odu_struct_t *)ho_chnl_hndl_ptr)->mem_ptr->payload_format)
    {
        result = coreotn_odu_chnl_prov_mode_get(digi_handle->coreotn_handle, &initial_prov_mode);

        if (PMC_SUCCESS == result)
        {
            result = digi_otn_odu_chnl_prov_mode_set(digi_handle, DIGI_OTN_ODU_CHNL_PROV_ALL);
            mode_updated = TRUE;
        }
    }
    /*
     * If there are no subcontainers ensure the first element reflects that
     */
    lo_chnl_hndl_ptr[0] = NULL;
    /*
     * Check that the High Order handle is not unchannelized before 
     * retrieving its subcontainers. If it is unchannelized then it can be
     * released. Since a High Order channel does not set a trib_slot or uses
     * all slots this is a match regardless.
     */
    if (UTIL_GLOBAL_ODU_UNCHANNELIZED != ((odu_struct_t *)ho_chnl_hndl_ptr)->mem_ptr->payload_format)
    {
        result = digi_resource_get_coreotn_subcontainers(digi_handle,
                                                         ho_chnl_hndl_type,
                                                         ho_chnl_hndl_ptr,
                                                         ho_cal_array,
                                                         &num_cal_entries);
        
        if (PMC_SUCCESS == result && num_cal_entries > 0)
        {
            result = digi_resource_lo_channel_get_by_trib_slot(digi_handle,
                                                               num_cal_entries,
                                                               ho_cal_array,
                                                               ho_chnl_hndl_type,
                                                               tx_trib_slot,
                                                               rx_trib_slot,
                                                               &slot_use,
                                                               lo_chnl_hndl_ptr);
        }
        else
        {
            lo_deprov = FALSE;
        }
    }
    else
    {
        /*
         * If the HO is unchannelized then there is only work to do if
         * switch_release is TRUE. Otherwise there is nothing to do.
         */
        if (TRUE == switch_release)
        {
            /*
             * Manually set the number calendar entries and the lo order 
             * channel handle which will be released below.
             */

            num_cal_entries = 1;
            lo_chnl_hndl_ptr[0] = (digi_otn_odu_chnl_t *)ho_chnl_hndl_ptr;
            lo_chnl_hndl_ptr[1] = NULL;
        }
        lo_deprov = FALSE;
    }

    if (PMC_SUCCESS == result && num_cal_entries > 0)
    {
        UINT32 i = 0;
        util_global_cpb_connect_t *oduk_connect_state_ptr = NULL;
        coreotn_oduksw_connect_data_state_get(digi_handle->coreotn_handle,
                                              &oduk_connect_state_ptr);
        /*
         * Release only unchannelized lower order channel handles.
         */
        while (i < DIGI_OTN_SERVER_MAX_TRIBSLOT && NULL != lo_chnl_hndl_ptr[i] && PMC_SUCCESS == result)
        {
            if (UTIL_GLOBAL_ODU_UNCHANNELIZED == ((odu_struct_t *)lo_chnl_hndl_ptr[i])->mem_ptr->payload_format)
            {
                util_global_oduk_port_t port_type = ((odu_struct_t *)lo_chnl_hndl_ptr[i])->switch_data.oduk_data.port_type;
                UINT32 channel = ((odu_struct_t *)lo_chnl_hndl_ptr[i])->switch_data.oduk_data.channel;
                result = digi_resource_release_oduk_resource(digi_handle,
                                                             lo_chnl_hndl_ptr[i],
                                                             port_type,
                                                             channel,
                                                             switch_release,
                                                             mapotn_release,
                                                             oduk_connect_state_ptr);
            }
            else if (UTIL_GLOBAL_LEVEL_MO_ODU == ((odu_struct_t *)lo_chnl_hndl_ptr[i])->switch_data.oduk_data.odu_level)
            {
                result = digi_resource_release_subcontainers(digi_handle,
                                                             lo_chnl_hndl_ptr[i],
                                                             DIGI_CHANNEL_HANDLE_ID_MO_ODU_T,
                                                             switch_release,
                                                             mapotn_release,
                                                             oduk_connect_state_ptr);
            }
            else
            {
                PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                        "TribSlot Release. ODU Level=%d invalid. Only HO/MO is valid\n",
                        ((odu_struct_t *)lo_chnl_hndl_ptr[i])->switch_data.oduk_data.odu_level);
                result = DIGI_ERR_ODU_LEVEL_NOT_VALID;
            }

            if (PMC_SUCCESS == result && FALSE == switch_release && 
                TRUE == lo_deprov)
            {
                result = digi_otn_server_lo_deprov(digi_handle,
                                                   &lo_chnl_hndl_ptr[i],
                                                   &ho_chnl_hndl_ptr);
            }

            i++;
        }
    }
    if (PMC_SUCCESS == result && FALSE == switch_release && 
        TRUE == lo_deprov && TRUE == mode_updated)
    {
        result = digi_otn_server_lo_deprov_end(digi_handle, 
                                               &ho_chnl_hndl_ptr);

    }
    /*
     * Reset the provisioning mode regardless of the result. It was updated
     * above based on the switch_release value and want to ensure that it is
     * reset before function terminates. 
     */
    if (TRUE == mode_updated)
    {
        result = digi_otn_odu_chnl_prov_mode_set(digi_handle, 
                                                 (digi_otn_odu_chnl_prov_mode_t)initial_prov_mode);
   } 

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_trib_slot_chnl_release */


/*******************************************************************************
* digi_subcontainer_chnl_release
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Releases all subcontainers attached to the high order parent channel 
*  handle.  For an OTN handle it releases all lower order sub containers
*  associated with it. If connected to a ODUK or CPB switch then demaps/
*  deactivates prior to deprovisioning. The High Order channel is not
*  deprovisioned by this function.
*
*  The switch_release=TRUE leaves channels provisioned 
*  and performs demapping and deactivating of the ODUKSW or CPB only.
*  Operation performs not operation of TRUE and handle is not connected
*  to a switching. \n\n
*
*  The mapotn_release flag if TRUE performs MAPOTN channel deprovisioning,
*  for any MAPOTN channels that are connected via ODUKSW or CPB to the
*  subcontainer channels. Ignored if handle not connected to a switch or
*  there is no MAPOTN channel handle connected. \n\n
*
*  NOTE: This function will release multiple levels of muxed lower order
*  channels.
*
* INPUTS:
*  *digi_handle    - pointer to DIGI handle instance
*  chnl_hndl_type  - Handle type of the trib slot owner. Either HO or MO ODU
*                    value.
*  chnl_hndl_ptr   - Reference to the higher order channel whose lower order
*                    channels are being release. Only 
*                    DIGI_CHANNEL_HANDLE_ID_MO_ODU_T and
*                    DIGI_CHANNEL_HANDLE_ID_LO_ODU_T are valid.
*  switch_release  - Flag if TRUE keeps the datapath intact and releases it
*                    at the switching point only. 
*  mapotn_release  - Releases mapoton resource if TRUE and in the datapath.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS    - Release operation was successful, otherwise a meaningful
*                    error code returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_subcontainer_chnl_release(digi_handle_t *digi_handle,
                                                digi_handle_id_t chnl_hndl_type,
                                                digi_chnl_hndl_t *chnl_hndl_ptr,
                                                BOOL8 switch_release,
                                                BOOL8 mapotn_release)
{
    PMC_ERROR result = PMC_SUCCESS;
    digi_otn_odu_chnl_t *lo_chnl_hndl_ptr[DIGI_OTN_SERVER_MAX_TRIBSLOT];
    digi_handle_id_t lo_chnl_hndl_type = LAST_DIGI_HANDLE_ID;
    coreotn_odu_chnl_prov_mode_t initial_prov_mode = LAST_COREOTN_CHNL_PROV_MODE;
    /*
     * Specific flag to ensure the mode is reset to the original before
     * leaving if it was updated at the beginning
     */
    BOOL8 mode_updated = FALSE;
    BOOL8 lo_deprov = TRUE;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != chnl_hndl_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(chnl_hndl_type < LAST_DIGI_HANDLE_ID,  DIGI_ERR_INVALID_ARG, 0, 0);

    if (!(DIGI_CHANNEL_HANDLE_ID_HO_ODU_T == chnl_hndl_type ||
          DIGI_CHANNEL_HANDLE_ID_MO_ODU_T == chnl_hndl_type))
    {
        PMC_ATOMIC_RETURN(digi_handle, DIGI_ERR_INVALID_ARG);
    }

    if (FALSE == switch_release)
    {
        result = coreotn_odu_chnl_prov_mode_get(digi_handle->coreotn_handle, &initial_prov_mode);

        if (PMC_SUCCESS == result)
        {
            result = digi_otn_odu_chnl_prov_mode_set(digi_handle, DIGI_OTN_ODU_CHNL_PROV_ALL);
            mode_updated = TRUE;
        }
    }
    lo_chnl_hndl_ptr[0] = NULL;

    if (PMC_SUCCESS == result)
    {
        if (UTIL_GLOBAL_ODU_UNCHANNELIZED != ((odu_struct_t *)chnl_hndl_ptr)->mem_ptr->payload_format)
        {
            result = digi_ho_subcontainer_chnl_hndl_get(digi_handle,
                                                        chnl_hndl_type,
                                                        chnl_hndl_ptr,
                                                        &lo_chnl_hndl_type,
                                                        lo_chnl_hndl_ptr);
        }
        else
        {
            if (FALSE == switch_release)
            {
                result = DIGI_ERR_NO_OPERATIONAL_DATAPATH;
            }
            else
            {
                lo_chnl_hndl_ptr[0] = chnl_hndl_ptr;
                lo_chnl_hndl_ptr[1] = NULL;
                lo_deprov = FALSE;
            }
        }
    }
    if (PMC_SUCCESS == result)
    {
        UINT32 index = 0;
        util_global_cpb_connect_t *oduk_connect_state_ptr = NULL;
        util_global_oduk_port_t port_type =  LAST_UTIL_GLOBAL_ODUK_PORT;
        UINT32 channel = 0;

        /*
         * If lo_deprov has been set to FALSE above then the sanity check
         * below does not apply.
         */
        if (TRUE == lo_deprov)
        {
            if (DIGI_CHANNEL_HANDLE_ID_HO_ODU_T == chnl_hndl_type)
            {
                PMC_ASSERT(DIGI_CHANNEL_HANDLE_ID_MO_ODU_T == lo_chnl_hndl_type,
                           DIGI_ERR_INVALID_ARG, 0, 0);
            }
            else if (DIGI_CHANNEL_HANDLE_ID_MO_ODU_T == chnl_hndl_type)
            {
                PMC_ASSERT(DIGI_CHANNEL_HANDLE_ID_LO_ODU_T == lo_chnl_hndl_type,
                           DIGI_ERR_INVALID_ARG, 0, 0);
            }
        }
        coreotn_oduksw_connect_data_state_get(digi_handle->coreotn_handle,
                                              &oduk_connect_state_ptr);

        while (index < DIGI_OTN_SERVER_MAX_TRIBSLOT && NULL != lo_chnl_hndl_ptr[index] && PMC_SUCCESS == result)
        {
            odu_struct_t *lo_ptr = (odu_struct_t *)lo_chnl_hndl_ptr[index];

            if (UTIL_GLOBAL_ODU_UNCHANNELIZED != lo_ptr->mem_ptr->payload_format)
            {
                PMC_ASSERT(DIGI_CHANNEL_HANDLE_ID_MO_ODU_T == lo_chnl_hndl_type,
                           DIGI_ERR_INVALID_ARG, 0, 0);
                result = digi_subcontainer_chnl_release(digi_handle,
                                                        lo_chnl_hndl_type,
                                                        lo_chnl_hndl_ptr[index],
                                                        switch_release,
                                                        mapotn_release);
            }
            else
            {
                port_type = lo_ptr->switch_data.oduk_data.port_type;
                channel = lo_ptr->switch_data.oduk_data.channel;
                
                result = digi_resource_release_oduk_resource(digi_handle,
                                                             lo_chnl_hndl_ptr[index],
                                                             port_type,
                                                             channel,
                                                             switch_release,
                                                             mapotn_release,
                                                             oduk_connect_state_ptr);
            }
            if (PMC_SUCCESS == result && FALSE == switch_release && 
                TRUE == lo_deprov)
            {
                result = digi_otn_server_lo_deprov(digi_handle,
                                                   &lo_chnl_hndl_ptr[index],
                                                   &chnl_hndl_ptr);
            }
            index++;
        }

        if (PMC_SUCCESS == result && TRUE == mode_updated && 
            TRUE == lo_deprov)
        {
            result = digi_otn_server_lo_deprov_end(digi_handle, &chnl_hndl_ptr);
        }
    }

    /*
     * No operational datapath indicates that this Ho/Mo has no
     * subcontainers. Since this is a release operation this is consider a
     * success so update the result value.
     */
    if (DIGI_ERR_NO_OPERATIONAL_DATAPATH == result)
    {
        result = PMC_SUCCESS;
    }

    if (TRUE == mode_updated)
    {
        result = digi_otn_odu_chnl_prov_mode_set(digi_handle, 
                                                 (digi_otn_odu_chnl_prov_mode_t)initial_prov_mode);
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_subcontainer_chnl_release */

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* digi_resource_check_path
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Iterates from the specified serdes interface structure and determmines
*  whether there is a path from this serdes interface through the entire
*  device. It is assumed that the digi_resource_handle is an accurate 
*  representation of the DIGI SDK's view of the device.  That is 
*  digi_resource_state_get was called prior to this function being called
*  and no DIGI SDK APIs were called in between.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *digi_resource_handle  - Resource handle table 
*   serdes_port_ptr        - Reference to the particular serdes port being
*                            checked.
*
* OUTPUTS:
*   operational_path       - Whether the path is actually operational. TRUE
*                            being returned could be returned if the serdes
*                            channel handle was NULL. This is not an error
*                            however it is not a fully operational path.
*
* RETURNS:
*   TRUE       - The path is fully operational. is it has been connected
*                from a serdes port across multiple interfaces and has been
*                activated
*   FALSE      - Channel is not mapped.
*
* NOTES:
*  A fully operation data path is defined as a series of spans from a serdes
*  port across multiple interfaces all of which have been activated with
*  traffic either operating across that data path or having the capability
*  of doing so.
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_check_path(digi_handle_t *digi_handle,
                                       digi_resource_handle_summary_t *digi_resource_handle,
                                       digi_serdes_port_intf_ctxt_t *serdes_port_ptr,
                                       BOOL8 *operational_path)
{
    BOOL8 result = FALSE;
    BOOL8 rx_result = FALSE;
    BOOL8 tx_result = FALSE;
    BOOL8 prov_status;
    digi_serdes_port_mode_t port_mode;
    digi_serdes_prov_state_t prov_state;
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;

    PMC_ENTRY();

    *operational_path = FALSE;

    if (NULL == serdes_port_ptr->chnl_handle_ptr)
    {
        /*
         * If the channel handle pointer is NULL then that is not an error.
         * This channel handle was never set up.  However if the 
         * next resource handle is last resource then that is an error.
         * For now return true for the former case and ASSERT on the latter
         * case.
         */
        PMC_RETURN(TRUE);
    }

    /* retrieve serdes port state */
    rc = digi_serdes_port_rate_prov_status_internal_get(digi_handle,
                                                        (digi_serdes_port_t *)serdes_port_ptr->port_ctxt_ptr,
                                                        FALSE,
                                                        &prov_status,
                                                        &port_mode,
                                                        &prov_state);
    if (PMC_SUCCESS != rc || 
        DIGI_SERDES_PROV_STATE_NO_SERDES == prov_state ||
        DIGI_SERDES_PROV_STATE_UNASSIGNED == prov_state)
    {
        result = FALSE;
        PMC_RETURN(result);
    }
    else if (LAST_DIGI_RESOURCE_HANDLE == serdes_port_ptr->next_resource_handle)
    {
        result = TRUE;
        /*
         * If here check that the channel handle type has been set. If 
         * ENET_LINE/SYS or CBRC the next resource handle should be the 
         * CPB_SWITCH. The only valid connection element to a serdes port 
         * that would not need be connected to a switch or some other 
         * resource is LINEOTN or SYSOTN. If that is the case mark the 
         * path as operational.
         */
        if (DIGI_CHANNEL_HANDLE_ID_LINEOTN_T == serdes_port_ptr->chnl_handle_type ||
            DIGI_CHANNEL_HANDLE_ID_SYSOTN_T == serdes_port_ptr->chnl_handle_type)
        {
            *operational_path = TRUE;
        }
        PMC_RETURN(result);
    }
    else if (TRUE == serdes_port_ptr->path_verified)
    {
        /*
         * Likely need to add an already scanned data path to ensure that
         * the data path is not scanned in each direction for data paths
         * that connect to the same serdes interface.  For now if either
         * of the paths are valid then set the result to true as this path
         * has already been worked over.
         */
        if (TRUE == serdes_port_ptr->tx_path_is_valid && 
            TRUE == serdes_port_ptr->rx_path_is_valid) 
        {
            result = TRUE;
            *operational_path = TRUE;
            PMC_RETURN(result);
        }
        else if (FALSE == serdes_port_ptr->tx_path_is_valid && 
                 FALSE == serdes_port_ptr->rx_path_is_valid)
        {
            *operational_path = FALSE;
            PMC_RETURN(result);
        }
    }

    {
        digi_direction_check_t direction = DIGI_DIRECTION_TX_RX;
        
        if (DIGI_SERDES_SYSTEM_INTF == ((digi_serdes_port_def_t *)serdes_port_ptr->port_ctxt_ptr)->intf)
        {
            is_line_side = FALSE;
        }
        else
        {
            is_line_side = TRUE;
        }

        if (TRUE == serdes_port_ptr->path_verified)
        {
            direction = (TRUE == serdes_port_ptr->tx_path_is_valid) ? DIGI_DIRECTION_RX_ONLY : DIGI_DIRECTION_TX_ONLY;
        }

        switch (serdes_port_ptr->chnl_handle_type)
        {
          case DIGI_CHANNEL_HANDLE_ID_LINEOTN_T:
             result = digi_resource_is_lineotn_operational(digi_handle,
                                                           digi_resource_handle,
                                                           serdes_port_ptr,
                                                           TRUE,
                                                           TRUE,
                                                           direction,
                                                           operational_path);
            break;
            
          case DIGI_CHANNEL_HANDLE_ID_SYSOTN_T:
             result = digi_resource_is_lineotn_operational(digi_handle,
                                                           digi_resource_handle,
                                                           serdes_port_ptr,
                                                           FALSE,
                                                           TRUE,
                                                           direction,
                                                           operational_path);
            break;
            
          case DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T:
            if (FALSE == serdes_port_ptr->path_verified)
            {
                tx_result = digi_resource_is_cbrc_operational(digi_handle,
                                                              digi_resource_handle,
                                                              serdes_port_ptr,
                                                              TRUE,
                                                              DIGI_DIRECTION_TX_ONLY);

                rx_result = digi_resource_is_cbrc_operational(digi_handle,
                                                              digi_resource_handle,
                                                              serdes_port_ptr,
                                                              TRUE,
                                                              DIGI_DIRECTION_RX_ONLY);

                if (TRUE == tx_result && TRUE == rx_result)
                {
                    result = TRUE;
                }
            }
            else
            {
                result = digi_resource_is_cbrc_operational(digi_handle,
                                                           digi_resource_handle,
                                                           serdes_port_ptr,
                                                           TRUE,
                                                           direction);
            }
            /*
             * If here there is a resource provisioned against the serdes
             * port therefore the path is operational.
             */
            *operational_path = TRUE;
            break;
            
          case DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T:
          case DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T:
            if (FALSE == serdes_port_ptr->path_verified)
            {
                tx_result = digi_resource_is_enet_operational(digi_handle,
                                                              digi_resource_handle,
                                                              serdes_port_ptr,
                                                              TRUE,
                                                              DIGI_DIRECTION_TX_ONLY);

                rx_result = digi_resource_is_enet_operational(digi_handle,
                                                              digi_resource_handle,
                                                              serdes_port_ptr,
                                                              TRUE,
                                                              DIGI_DIRECTION_RX_ONLY);
                
                if (TRUE == tx_result && TRUE == rx_result)
                {
                    result = TRUE;
                }
            }
            else
            {
                result = digi_resource_is_enet_operational(digi_handle,
                                                          digi_resource_handle,
                                                          serdes_port_ptr,
                                                          TRUE,
                                                          direction);
            }
            /*
             * If here there is a resource provisioned against the serdes
             * port therefore the path is operational.
             */
            *operational_path = TRUE;
            break;

          default:
            /*
             * No other handle type is valid at this level. Set the result to
             * false and leave
             */
            result = FALSE;
            break;
        }

        /* if serdes is not provioned, reset result */
        if (PMC_SUCCESS != rc ||
            (FALSE == prov_status) ||
            (DIGI_SERDES_PROV_STATE_UNASSIGNED == prov_state)||
            (DIGI_SERDES_PROV_STATE_NO_SERDES == prov_state))
        {
            result = FALSE;
            *operational_path = FALSE;
        }

        if (TRUE == result)
        {
            *operational_path = TRUE;
        }
    }

    PMC_RETURN(result);
} /* digi_resource_check_path */

/*******************************************************************************
* digi_resource_release_datapath
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Iterate over the datapath and reset the underlying database for this
*   channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *digi_resource_handle  - Resource handle table 
*   serdes_port_ptr        - References to the serdes port resource
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
PRIVATE void digi_resource_release_datapath(digi_handle_t *digi_handle,
                                           digi_resource_handle_summary_t *digi_resource_handle,
                                            digi_serdes_port_intf_ctxt_t *serdes_port_ptr)
{
    PMC_ENTRY();

    switch (serdes_port_ptr->chnl_handle_type)
    {
      case DIGI_CHANNEL_HANDLE_ID_LINEOTN_T:
        digi_resource_release_lineotn(digi_handle,
                                      digi_resource_handle,
                                      serdes_port_ptr,
                                      TRUE);
        break;
        
      case DIGI_CHANNEL_HANDLE_ID_SYSOTN_T:
        digi_resource_release_lineotn(digi_handle,
                                      digi_resource_handle,
                                      serdes_port_ptr,
                                      FALSE);
        break;
        
      case DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T:
        digi_resource_release_cbrc(digi_handle,
                                   digi_resource_handle,
                                   serdes_port_ptr);        
        break;
        
      case DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T:
      case DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T:
        digi_resource_release_enet(digi_handle,
                                   digi_resource_handle,
                                   serdes_port_ptr);
        break;
        
      default:
        /*
         * No other handle type is valid at this level. ASSERT to catch any
         * events that should not happen
         */
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_PORT_TYPE, 0, 0);
    }
                                                      
    PMC_RETURN();
} /*digi_resource_release_datapath */


/*******************************************************************************
* digi_resource_handle_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Initializes the digi_resource_handle structure to default values. Namely
*  all pointers set to NULL, counter to 0, and enumerands to there last 
*  value.
*
* INPUTS:
*   None.
* 
* OUTPUTS:
*   *digi_resource_handle  - Structure to be initialized.
*
* RETURNS:
*   PMC_SUCCESS            - Return value included for completeness.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_handle_init(digi_resource_handle_summary_t *digi_resource_handle)
{
    UINT16 i;

    PMC_ENTRY();

    for (i=0; i < DIGI_XFI_PIN_NUM; i++)
    {
        digi_resource_handle->line_serdes_cfg[i].port_ctxt_ptr = NULL;
        digi_resource_handle->line_serdes_cfg[i].port_uid = 0;
        digi_resource_handle->line_serdes_cfg[i].chnl_handle_type = LAST_DIGI_HANDLE_ID;
        digi_resource_handle->line_serdes_cfg[i].chnl_handle_ptr = NULL;
        digi_resource_handle->line_serdes_cfg[i].next_resource_handle = LAST_DIGI_RESOURCE_HANDLE;
        digi_resource_handle->line_serdes_cfg[i].next_handle_index = 0;
        digi_resource_handle->line_serdes_cfg[i].tx_path_is_valid = FALSE;
        digi_resource_handle->line_serdes_cfg[i].rx_path_is_valid = FALSE;
        digi_resource_handle->line_serdes_cfg[i].path_verified = FALSE;

        digi_resource_handle->sys_serdes_cfg[i].port_ctxt_ptr = NULL;
        digi_resource_handle->sys_serdes_cfg[i].port_uid = 0;
        digi_resource_handle->sys_serdes_cfg[i].chnl_handle_type = LAST_DIGI_HANDLE_ID;
        digi_resource_handle->sys_serdes_cfg[i].chnl_handle_ptr = NULL;
        digi_resource_handle->sys_serdes_cfg[i].next_resource_handle = LAST_DIGI_RESOURCE_HANDLE;
        digi_resource_handle->sys_serdes_cfg[i].next_handle_index = 0;
        digi_resource_handle->sys_serdes_cfg[i].tx_path_is_valid = FALSE;
        digi_resource_handle->sys_serdes_cfg[i].rx_path_is_valid = FALSE;
        digi_resource_handle->sys_serdes_cfg[i].path_verified = FALSE;

        if (i < DIGI_SFI51_INTF_NUM)
        {
            digi_resource_handle->sfi51_serdes_cfg[i].port_ctxt_ptr = NULL;
            digi_resource_handle->sfi51_serdes_cfg[i].port_uid = 0;
            digi_resource_handle->sfi51_serdes_cfg[i].chnl_handle_type = LAST_DIGI_HANDLE_ID;
            digi_resource_handle->sfi51_serdes_cfg[i].chnl_handle_ptr = NULL;
            digi_resource_handle->sfi51_serdes_cfg[i].next_resource_handle = LAST_DIGI_RESOURCE_HANDLE;
            digi_resource_handle->sfi51_serdes_cfg[i].next_handle_index = 0;
            digi_resource_handle->sfi51_serdes_cfg[i].tx_path_is_valid = FALSE;
            digi_resource_handle->sfi51_serdes_cfg[i].rx_path_is_valid = FALSE;
            digi_resource_handle->sfi51_serdes_cfg[i].path_verified = FALSE;
        }
    }

    for (i=0; i < DIGI_MAPOTN_CHNL_MAX; i++)
    {
        digi_resource_handle->mapper_cfg[i].mapper_handle_ptr = NULL;
        digi_resource_handle->mapper_cfg[i].oduk_switch_index = 0;
        digi_resource_handle->mapper_cfg[i].cpb_switch_index = 0;
    }
    /*
     * Run one loop for both. The upper end is 426 and the lower is 396.
     */
    for (i=0; i < DIGI_CPB_SWITCH_TOTAL_CHANNELS; i++)
    {
        digi_resource_handle->cpbsw_cfg[i].provisioned_handle_type = LAST_DIGI_RESOURCE_HANDLE;
        digi_resource_handle->cpbsw_cfg[i].provisioned_handle_index = 0;
        digi_resource_handle->cpbsw_cfg[i].source_handle_type = LAST_DIGI_RESOURCE_HANDLE;
        digi_resource_handle->cpbsw_cfg[i].source_handle_index = 0;
        digi_resource_handle->cpbsw_cfg[i].source_activated = FALSE;
        digi_resource_handle->cpbsw_cfg[i].primary_dest_handle_type = LAST_DIGI_RESOURCE_HANDLE;
        digi_resource_handle->cpbsw_cfg[i].primary_dest_index = 0;
        digi_resource_handle->cpbsw_cfg[i].dest_activated = FALSE;
        digi_resource_handle->cpbsw_cfg[i].associate_dest_handle_type = LAST_DIGI_RESOURCE_HANDLE;
        digi_resource_handle->cpbsw_cfg[i].associate_dest_index = 0;
        digi_resource_handle->cpbsw_cfg[i].num_dest = 0;

        if (i < DIGI_ODUK_SWITCH_TOTAL_CHANNELS)
        {
            digi_resource_handle->oduksw_cfg[i].provisioned_handle_type = LAST_DIGI_RESOURCE_HANDLE;
            digi_resource_handle->oduksw_cfg[i].provisioned_handle_index = 0;
            digi_resource_handle->oduksw_cfg[i].source_handle_type = LAST_DIGI_RESOURCE_HANDLE;
            digi_resource_handle->oduksw_cfg[i].source_handle_index = 0;
            digi_resource_handle->oduksw_cfg[i].source_activated = FALSE;
            digi_resource_handle->oduksw_cfg[i].primary_dest_handle_type = LAST_DIGI_RESOURCE_HANDLE;
            digi_resource_handle->oduksw_cfg[i].primary_dest_index = 0;
            digi_resource_handle->oduksw_cfg[i].dest_activated = FALSE;
            digi_resource_handle->oduksw_cfg[i].associate_dest_handle_type = LAST_DIGI_RESOURCE_HANDLE;
            digi_resource_handle->oduksw_cfg[i].associate_dest_index = 0;
            digi_resource_handle->oduksw_cfg[i].num_dest = 0;
        }
    }

    for (i = 0; i < DIGI_OTN_SERVER_LO_CHNL_MAX; i++)
    {
        digi_resource_handle->line_otn_lo_container_cfg[i].parent_handle_type = LAST_DIGI_RESOURCE_HANDLE;
        digi_resource_handle->line_otn_lo_container_cfg[i].parent_handle_index = 0;
        digi_resource_handle->line_otn_lo_container_cfg[i].handle_type = LAST_DIGI_HANDLE_ID;
        digi_resource_handle->line_otn_lo_container_cfg[i].chnl_handle_ptr = NULL;
        digi_resource_handle->line_otn_lo_container_cfg[i].payload_mode = LAST_DIGI_ODU_PAYLOAD_FORMAT;
        digi_resource_handle->line_otn_lo_container_cfg[i].tx_trib_slot_mask[0] = 0;
        digi_resource_handle->line_otn_lo_container_cfg[i].tx_trib_slot_mask[1] = 0;
        digi_resource_handle->line_otn_lo_container_cfg[i].tx_trib_slot_mask[2] = 0;
        digi_resource_handle->line_otn_lo_container_cfg[i].rx_trib_slot_mask[0] = 0;
        digi_resource_handle->line_otn_lo_container_cfg[i].rx_trib_slot_mask[1] = 0;
        digi_resource_handle->line_otn_lo_container_cfg[i].rx_trib_slot_mask[2] = 0;
        digi_resource_handle->line_otn_lo_container_cfg[i].lo_chnl_entries = 0;
        digi_resource_handle->line_otn_lo_container_cfg[i].next_resource_handle = LAST_DIGI_RESOURCE_HANDLE;
        digi_resource_handle->line_otn_lo_container_cfg[i].next_handle_index = 0;
        digi_resource_handle->line_otn_lo_container_cfg[i].next_associate_handle = LAST_DIGI_RESOURCE_HANDLE;
        digi_resource_handle->line_otn_lo_container_cfg[i].next_associate_index = 0;
        digi_resource_handle->line_otn_lo_container_cfg[i].tx_path_is_valid = FALSE;
        digi_resource_handle->line_otn_lo_container_cfg[i].rx_path_is_valid = FALSE;
        /*
         * Currently DIGI_OTN_SERVER_MO_CHNL_MAX and 
         * DIGI_OTN_SERVER_LO_CHNL_MAX are the same size. For completeness
         * add a check to ensure we do not over initialize if these sizes 
         * ever change.
         */
        if (i < DIGI_OTN_SERVER_MO_CHNL_MAX)
        {
            digi_resource_handle->line_otn_mo_container_cfg[i].parent_handle_type = LAST_DIGI_RESOURCE_HANDLE;
            digi_resource_handle->line_otn_mo_container_cfg[i].parent_handle_index = 0;
            digi_resource_handle->line_otn_mo_container_cfg[i].handle_type = LAST_DIGI_HANDLE_ID;
            digi_resource_handle->line_otn_mo_container_cfg[i].chnl_handle_ptr = NULL;
            digi_resource_handle->line_otn_mo_container_cfg[i].payload_mode = LAST_DIGI_ODU_PAYLOAD_FORMAT;
            digi_resource_handle->line_otn_mo_container_cfg[i].tx_trib_slot_mask[0] = 0;
            digi_resource_handle->line_otn_mo_container_cfg[i].tx_trib_slot_mask[1] = 0;
            digi_resource_handle->line_otn_mo_container_cfg[i].tx_trib_slot_mask[2] = 0;
            digi_resource_handle->line_otn_mo_container_cfg[i].rx_trib_slot_mask[0] = 0;
            digi_resource_handle->line_otn_mo_container_cfg[i].rx_trib_slot_mask[1] = 0;
            digi_resource_handle->line_otn_mo_container_cfg[i].rx_trib_slot_mask[2] = 0;
            digi_resource_handle->line_otn_mo_container_cfg[i].lo_chnl_entries = 0;
            digi_resource_handle->line_otn_mo_container_cfg[i].next_resource_handle = LAST_DIGI_RESOURCE_HANDLE;
            digi_resource_handle->line_otn_mo_container_cfg[i].next_handle_index = 0;
            digi_resource_handle->line_otn_mo_container_cfg[i].next_associate_handle = LAST_DIGI_RESOURCE_HANDLE;
            digi_resource_handle->line_otn_mo_container_cfg[i].next_associate_index = 0;
            digi_resource_handle->line_otn_mo_container_cfg[i].tx_path_is_valid = FALSE;
            digi_resource_handle->line_otn_mo_container_cfg[i].rx_path_is_valid = FALSE;
            digi_resource_handle->sys_otn_mo_container_cfg[i].parent_handle_type = LAST_DIGI_RESOURCE_HANDLE;   
            digi_resource_handle->sys_otn_mo_container_cfg[i].parent_handle_index = 0;   
            digi_resource_handle->sys_otn_mo_container_cfg[i].handle_type = LAST_DIGI_HANDLE_ID;
            digi_resource_handle->sys_otn_mo_container_cfg[i].chnl_handle_ptr = NULL;
            digi_resource_handle->sys_otn_mo_container_cfg[i].payload_mode = LAST_DIGI_ODU_PAYLOAD_FORMAT;
            digi_resource_handle->sys_otn_mo_container_cfg[i].tx_trib_slot_mask[0] = 0;
            digi_resource_handle->sys_otn_mo_container_cfg[i].tx_trib_slot_mask[1] = 0;
            digi_resource_handle->sys_otn_mo_container_cfg[i].tx_trib_slot_mask[2] = 0;
            digi_resource_handle->sys_otn_mo_container_cfg[i].rx_trib_slot_mask[0] = 0;
            digi_resource_handle->sys_otn_mo_container_cfg[i].rx_trib_slot_mask[1] = 0;
            digi_resource_handle->sys_otn_mo_container_cfg[i].rx_trib_slot_mask[2] = 0;
            digi_resource_handle->sys_otn_mo_container_cfg[i].lo_chnl_entries = 0;
            digi_resource_handle->sys_otn_mo_container_cfg[i].next_resource_handle = LAST_DIGI_RESOURCE_HANDLE;
            digi_resource_handle->sys_otn_mo_container_cfg[i].next_handle_index = 0;
            digi_resource_handle->sys_otn_mo_container_cfg[i].next_associate_handle = LAST_DIGI_RESOURCE_HANDLE;
            digi_resource_handle->sys_otn_mo_container_cfg[i].next_associate_index = 0;
            digi_resource_handle->sys_otn_mo_container_cfg[i].tx_path_is_valid = FALSE;
            digi_resource_handle->sys_otn_mo_container_cfg[i].rx_path_is_valid = FALSE;
        }

        if (i < DIGI_OTN_SERVER_HO_CHNL_MAX)
        {
            digi_resource_handle->line_otn_ho_container_cfg[i].parent_handle_type = LAST_DIGI_RESOURCE_HANDLE;
            digi_resource_handle->line_otn_ho_container_cfg[i].parent_handle_index = 0;
            digi_resource_handle->line_otn_ho_container_cfg[i].handle_type = LAST_DIGI_HANDLE_ID;
            digi_resource_handle->line_otn_ho_container_cfg[i].chnl_handle_ptr = NULL;
            digi_resource_handle->line_otn_ho_container_cfg[i].payload_mode = LAST_DIGI_ODU_PAYLOAD_FORMAT;
            digi_resource_handle->line_otn_ho_container_cfg[i].tx_trib_slot_mask[0] = 0;
            digi_resource_handle->line_otn_ho_container_cfg[i].tx_trib_slot_mask[1] = 0;
            digi_resource_handle->line_otn_ho_container_cfg[i].tx_trib_slot_mask[2] = 0;
            digi_resource_handle->line_otn_ho_container_cfg[i].rx_trib_slot_mask[0] = 0;
            digi_resource_handle->line_otn_ho_container_cfg[i].rx_trib_slot_mask[1] = 0;
            digi_resource_handle->line_otn_ho_container_cfg[i].rx_trib_slot_mask[2] = 0;
            digi_resource_handle->line_otn_ho_container_cfg[i].lo_chnl_entries = 0;

            digi_resource_handle->line_otn_ho_container_cfg[i].next_resource_handle = LAST_DIGI_RESOURCE_HANDLE;
            digi_resource_handle->line_otn_ho_container_cfg[i].next_handle_index = 0;
            digi_resource_handle->line_otn_ho_container_cfg[i].next_associate_handle = LAST_DIGI_RESOURCE_HANDLE;
            digi_resource_handle->line_otn_ho_container_cfg[i].next_associate_index = 0;
            digi_resource_handle->line_otn_ho_container_cfg[i].tx_path_is_valid = FALSE;
            digi_resource_handle->line_otn_ho_container_cfg[i].rx_path_is_valid = FALSE;
            digi_resource_handle->sys_otn_ho_container_cfg[i].parent_handle_type = LAST_DIGI_RESOURCE_HANDLE;
            digi_resource_handle->sys_otn_ho_container_cfg[i].parent_handle_index = 0;
            digi_resource_handle->sys_otn_ho_container_cfg[i].handle_type = LAST_DIGI_HANDLE_ID;
            digi_resource_handle->sys_otn_ho_container_cfg[i].chnl_handle_ptr = NULL;
            digi_resource_handle->sys_otn_ho_container_cfg[i].payload_mode = LAST_DIGI_ODU_PAYLOAD_FORMAT;
            digi_resource_handle->sys_otn_ho_container_cfg[i].tx_trib_slot_mask[0] = 0;
            digi_resource_handle->sys_otn_ho_container_cfg[i].tx_trib_slot_mask[1] = 0;
            digi_resource_handle->sys_otn_ho_container_cfg[i].tx_trib_slot_mask[2] = 0;
            digi_resource_handle->sys_otn_ho_container_cfg[i].rx_trib_slot_mask[0] = 0;
            digi_resource_handle->sys_otn_ho_container_cfg[i].rx_trib_slot_mask[1] = 0;
            digi_resource_handle->sys_otn_ho_container_cfg[i].rx_trib_slot_mask[2] = 0;
            digi_resource_handle->sys_otn_ho_container_cfg[i].lo_chnl_entries = 0;
            digi_resource_handle->sys_otn_ho_container_cfg[i].next_resource_handle = LAST_DIGI_RESOURCE_HANDLE;
            digi_resource_handle->sys_otn_ho_container_cfg[i].next_handle_index = 0;
            digi_resource_handle->sys_otn_ho_container_cfg[i].next_associate_handle = LAST_DIGI_RESOURCE_HANDLE;
            digi_resource_handle->sys_otn_ho_container_cfg[i].next_associate_index = 0;
            digi_resource_handle->sys_otn_ho_container_cfg[i].tx_path_is_valid = FALSE;
            digi_resource_handle->sys_otn_ho_container_cfg[i].rx_path_is_valid = FALSE;
        }
    }

    for (i=0; i < DIGI_ILKN_INTF_NUM; i++)
    {
        digi_resource_handle->ilkn_inst_cfg[i] = NULL;
    }

    for (i=0; i < DIGI_ILKN_CHNL_NUM; i++)
    {
        digi_resource_handle->ilkn_chnl_cfg[0][i].ilkn_chnl_handle_ptr = NULL;
        digi_resource_handle->ilkn_chnl_cfg[0][i].next_resource_handle = LAST_DIGI_RESOURCE_HANDLE;
        digi_resource_handle->ilkn_chnl_cfg[0][i].next_handle_index = 0;
        PMC_MEMSET((void *)&digi_resource_handle->ilkn_chnl_cfg[0][i].ilkn_chnl_desc, 0, sizeof(digi_ilkn_chnl_desc_t));
        digi_resource_handle->ilkn_chnl_cfg[0][i].ilkn_chnl_desc.target = LAST_DIGI_ILKN_CHNL_TARGET;
        digi_resource_handle->ilkn_chnl_cfg[1][i].ilkn_chnl_handle_ptr = NULL;
        digi_resource_handle->ilkn_chnl_cfg[1][i].next_resource_handle = LAST_DIGI_RESOURCE_HANDLE;
        digi_resource_handle->ilkn_chnl_cfg[1][i].next_handle_index = 0;
        PMC_MEMSET((void *)&digi_resource_handle->ilkn_chnl_cfg[1][i].ilkn_chnl_desc, 0, sizeof(digi_ilkn_chnl_desc_t));
        digi_resource_handle->ilkn_chnl_cfg[1][i].ilkn_chnl_desc.target = LAST_DIGI_ILKN_CHNL_TARGET;
    }
    digi_resource_handle->datapath_release = FALSE;

    PMC_RETURN();
}


/*******************************************************************************
* digi_resource_line_handles_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the line side resources. From the serdes port handle through
*   the device or until the current span terminates.
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance
*   serdes_index        - Indice of the serdes port connection array.
*   intf                - SERDES interface (Line, SFI51 or SYS) being used.
*
* OUTPUTS:
*   digi_resource_handle - Resource handle being updated.
*   last_index           - Reference to the last OTN indice. Used when there
*                          are mulitple OTN channels for a particular 
*                          interface.
*                          NOTE: Pass here as the loop associated with OTN
*                          channels is done from the perspective of the 
*                          serdes ports and must be passed in at this level
*                          in order to work.
*   last_handle          - Reference to the last associate handle set. Used
*                          primarily as a barometer as to whether or not the
*                          last one was set or more appropriately whether 
*                          there are more than one OTN channel.
*
* RETURNS:
*   PMC_SUCCESS: Operation was successful.
*   PMC_ERROR: Some unexpected operation occurred.
*
* NOTES:
*
*******************************************************************************/

PRIVATE PMC_ERROR digi_resource_handles_get(digi_handle_t *digi_handle,
                                            UINT8 serdes_index,
                                            digi_serdes_intf_t intf,
                                            digi_resource_handle_summary_t *digi_resource_handle,
                                            UINT16 *last_index,
                                            digi_resource_next_handle_t *last_handle)
{ 
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    UINT32 port_uid = DIGI_SERDES_UNASSIGNED;
    BOOL8 bool_result = FALSE;

    PMC_ENTRY();

    if (DIGI_SERDES_LINE_INTF == intf)
    {
        digi_resource_handle->line_serdes_cfg[serdes_index].port_ctxt_ptr = (digi_serdes_port_t *)&(digi_handle->var.port_ctxt.line_port_ctxt[serdes_index]);
        digi_resource_handle->line_serdes_cfg[serdes_index].port_uid = digi_handle->var.port_ctxt.line_port_ctxt[serdes_index].port_uid;
        port_uid = digi_resource_handle->line_serdes_cfg[serdes_index].port_uid;
    }
    else if (DIGI_SERDES_SYSTEM_INTF == intf)
    {
        digi_resource_handle->sys_serdes_cfg[serdes_index].port_ctxt_ptr = (digi_serdes_port_t *)&(digi_handle->var.port_ctxt.sys_port_ctxt[serdes_index]);
        port_uid = digi_handle->var.port_ctxt.sys_port_ctxt[serdes_index].port_uid;
        digi_resource_handle->sys_serdes_cfg[serdes_index].port_uid = port_uid;
    }
    else if (DIGI_SERDES_SFI_51_INTF == intf)
    {
        digi_resource_handle->sfi51_serdes_cfg[serdes_index].port_ctxt_ptr = (digi_serdes_port_t *)&(digi_handle->var.port_ctxt.sfi51_port_ctxt[serdes_index]);
        port_uid = digi_handle->var.port_ctxt.sfi51_port_ctxt[serdes_index].port_uid;
        digi_resource_handle->sfi51_serdes_cfg[serdes_index].port_uid = port_uid;
    }
    bool_result = digi_is_otn_connected(digi_handle, serdes_index, intf, 
                                        port_uid, digi_resource_handle);


    if (TRUE == bool_result)
    {
        result = PMC_SUCCESS;
        /*
         * For each interface need to check the next_resource_handle. if
         * it is ODUK_SWITCH then it has already been set and there is 
         * nothing else to do.
         * NOTE: WILL NEED TO ADD ANOTHER CASE AS WELL AS NEXT RESOURCE
         * HANDLE COULD BE UNCONFIGURED. NAMELY LINE OR SYS OTN HAVE BEEN
         * SET BUT NOT CONNECTED TO ANYTHING YET. THIS CASE NEEDS TO BE 
         * HANDLED.
         */
        if (DIGI_SERDES_SFI_51_INTF == intf)
        {
            if (digi_resource_handle->sfi51_serdes_cfg[serdes_index].next_resource_handle != DIGI_ODUK_SWITCH)
            {
                result = digi_line_otn_subcontainer_set(digi_handle, 
                                                        digi_resource_handle->sfi51_serdes_cfg[serdes_index].next_handle_index, 
                                                        digi_resource_handle,
                                                        last_index,
                                                        last_handle);
            }
        }
        else if (DIGI_SERDES_LINE_INTF == intf)
        {
            if (digi_resource_handle->line_serdes_cfg[serdes_index].next_resource_handle != DIGI_ODUK_SWITCH)
            {
                result = digi_line_otn_subcontainer_set(digi_handle, 
                                                        digi_resource_handle->line_serdes_cfg[serdes_index].next_handle_index,
                                                        digi_resource_handle,
                                                        last_index,
                                                        last_handle);
            }
        }
        else if (DIGI_SERDES_SYSTEM_INTF == intf)
        {
            if (digi_resource_handle->sys_serdes_cfg[serdes_index].next_resource_handle != DIGI_ODUK_SWITCH)
            {
                result = digi_sys_otn_subcontainer_set(digi_handle,
                                                       digi_resource_handle->sys_serdes_cfg[serdes_index].next_handle_index,
                                                       digi_resource_handle);
            }
        }
    }
    if (FALSE == bool_result)
    {
        bool_result = digi_is_enet_connected(digi_handle, serdes_index, 
                                             intf, port_uid,
                                             digi_resource_handle);
    }
    if (FALSE == bool_result)
    {
        bool_result = digi_is_cbrc_connected(digi_handle, serdes_index,
                                             intf, port_uid,
                                             digi_resource_handle);
    }

    if (TRUE == bool_result)
    {
        result = PMC_SUCCESS;
    }

    PMC_RETURN(result);
} /* digi_resource_handles_get */


/*******************************************************************************
* digi_is_cbrc_connected
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Determines whether the serdes port handle is connected to the line otn
*   and if so then determines the connectivity of the underlying ho/mo/lo
*   order channels.
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance
*   serdes_index        - Index into the digi_handle for the serdes port
*                         handles which is the same index used in the
*                         handle summary structure for the associated 
*                         digi_serdes_pin_intf_ctxt_t structure.
*   intf                - Interface being used, Line, SFI51 or SYS
*   port_uid            - Serdes port identifier being looked up.
*
* OUTPUTS:
*   digi_resource_handle - Resource handle being updated.
*
* RETURNS:
*   TRUE  - Serdes Port handle is connected to CBRC subsystem.
*   FALSE - Otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 digi_is_cbrc_connected(digi_handle_t *digi_handle,
                                     UINT8 serdes_index,
                                     digi_serdes_intf_t intf,
                                     UINT32 port_uid,
                                     digi_resource_handle_summary_t *digi_resource_handle)
{
    BOOL8 result = FALSE;
    UINT8 i;

    PMC_ENTRY();

    for (i = 0; i < DIGI_SERDES_DSI_LANES_MAX; i++)
    {
        /*
         * Rather than using internal functions for this since CBRC does not
         * have a line/system side designation iterate over the dsi settings
         * on the system and line side directly.
         */
        if (digi_handle->handle_pool.cbr_chnl[i].port_uid == port_uid &&
            (TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.cbr_chnl[i].switch_data.header.prov_state)))
        {
            result = TRUE;

            if (DIGI_SERDES_SYSTEM_INTF == intf)
            {
                digi_resource_handle->sys_serdes_cfg[serdes_index].chnl_handle_type = DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T;
                digi_resource_handle->sys_serdes_cfg[serdes_index].chnl_handle_ptr = (void *)&(digi_handle->handle_pool.cbr_chnl[i]);
                digi_resource_handle->sys_serdes_cfg[serdes_index].next_resource_handle = DIGI_CPB_SWITCH;
                digi_resource_handle->sys_serdes_cfg[serdes_index].next_handle_index = digi_get_index_and_set_cpb_data(digi_handle, serdes_index, DIGI_SYS_SERDES_CFG, digi_resource_handle);

            }
            else if (DIGI_SERDES_LINE_INTF == intf)
            {
                digi_resource_handle->line_serdes_cfg[serdes_index].chnl_handle_type = DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T;
                digi_resource_handle->line_serdes_cfg[serdes_index].chnl_handle_ptr = (void *)&(digi_handle->handle_pool.cbr_chnl[i]);
                digi_resource_handle->line_serdes_cfg[serdes_index].next_resource_handle = DIGI_CPB_SWITCH;
                digi_resource_handle->line_serdes_cfg[serdes_index].next_handle_index = digi_get_index_and_set_cpb_data(digi_handle, serdes_index, DIGI_LINE_SERDES_CFG, digi_resource_handle);

            }
            else if (DIGI_SERDES_SFI_51_INTF == intf)
            {
                digi_resource_handle->sfi51_serdes_cfg[serdes_index].chnl_handle_type = DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T;
                digi_resource_handle->sfi51_serdes_cfg[serdes_index].chnl_handle_ptr = (void *)&(digi_handle->handle_pool.cbr_chnl[i]);
                digi_resource_handle->sfi51_serdes_cfg[serdes_index].next_resource_handle = DIGI_CPB_SWITCH;
                digi_resource_handle->sfi51_serdes_cfg[serdes_index].next_handle_index = digi_get_index_and_set_cpb_data(digi_handle, serdes_index, DIGI_SFI51_SERDES_CFG, digi_resource_handle);
            }
            break;
        }
    }

    PMC_RETURN(result);
} /* digi_is_cbrc_connected */

/*******************************************************************************
* digi_is_enet_connected
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Determines whether the serdes port handle is connected to the line otn
*   and if so then determines the connectivity of the underlying ho/mo/lo
*   order channels.
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance
*   serdes_index        - Index into the digi_handle for the serdes port
*                         handles which is the same index used in the
*                         handle summary structure for the associated 
*                         digi_serdes_pin_intf_ctxt_t structure.
*   intf                - Interface being used, Line, SFI51 or SYS
*   port_uid            - Serdes port identifier being looked up.
*
* OUTPUTS:
*   digi_resource_handle - Resource handle being updated.
*
* RETURNS:
*   TRUE  - Serdes Port handle is connected to ENET line or ENET sys
*           depending on the value of intf
*   FALSE - Otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 digi_is_enet_connected(digi_handle_t *digi_handle,
                                     UINT8 serdes_index,
                                     digi_serdes_intf_t intf,
                                     UINT32 port_uid,
                                     digi_resource_handle_summary_t *digi_resource_handle)
{
    BOOL8 result = FALSE;
    digi_post_mld_dsi_ctxt_t *post_mld_dsi_ptr = NULL;
    util_global_cpb_port_t port_type;
    digi_enet_client_chnl_def_t* chnl_ctxt_ptr = NULL;

    PMC_ENTRY();

    /*
     * Retrieve the mkd DSI pointer.Unlike digi_first_sys_post_mld_dsi_find
     * digi_first_post_mld_dsi_find checks both the line and sys side.
     * The former checks only the sys side.
     */
    post_mld_dsi_ptr = digi_first_post_mld_dsi_find(digi_handle, 
                                                    port_uid);

    if (NULL != post_mld_dsi_ptr)
    {
        if ((DIGI_SERDES_LINE_INTF == intf ||
             (DIGI_SERDES_SYSTEM_INTF == intf &&
              DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == digi_handle->var.prod_app)) &&
            DIGI_SERDES_PROV_STATE_ENET_LINE == post_mld_dsi_ptr->prov_state)
        {
            port_type = UTIL_GLOBAL_CPB_PORT_ENET_LINE;
            chnl_ctxt_ptr = &(digi_handle->handle_pool.enet_line_chnl[post_mld_dsi_ptr->lane_index]);
        }
        else if (DIGI_SERDES_SYSTEM_INTF == intf &&
                 DIGI_SERDES_PROV_STATE_ENET_SYS ==
                 post_mld_dsi_ptr->prov_state)
        {
             port_type = UTIL_GLOBAL_CPB_PORT_ENET_SYS;
             chnl_ctxt_ptr = &(digi_handle->handle_pool.enet_sys_chnl[post_mld_dsi_ptr->lane_index]);
        }
    } 

    /*
     * Not sure whether the CPB port type test is valid. The channel context
     * has for enet line or sys has been returned based on the port_uid 
     * and the underlying dsi lane index.
     */
    if (NULL != chnl_ctxt_ptr && 
        (TRUE == util_global_rx_or_tx_chnl_prov_status_get(chnl_ctxt_ptr->switch_data.header.prov_state)) &&
        chnl_ctxt_ptr->switch_data.cpb_data.port_type == port_type)
    {
        result = TRUE;

        if (DIGI_SERDES_LINE_INTF == intf)
        {
            digi_resource_handle->line_serdes_cfg[serdes_index].chnl_handle_type = DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T;
            digi_resource_handle->line_serdes_cfg[serdes_index].chnl_handle_ptr = (void *)chnl_ctxt_ptr;
            digi_resource_handle->line_serdes_cfg[serdes_index].next_resource_handle = DIGI_CPB_SWITCH;
            digi_resource_handle->line_serdes_cfg[serdes_index].next_handle_index = digi_get_index_and_set_cpb_data(digi_handle, serdes_index, DIGI_LINE_SERDES_CFG, digi_resource_handle);
        }
        else if (DIGI_SERDES_SYSTEM_INTF == intf)
        {
            digi_resource_handle->sys_serdes_cfg[serdes_index].chnl_handle_type = DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T;
            digi_resource_handle->sys_serdes_cfg[serdes_index].chnl_handle_ptr = (void *)chnl_ctxt_ptr;
            digi_resource_handle->sys_serdes_cfg[serdes_index].next_resource_handle = DIGI_CPB_SWITCH;
            digi_resource_handle->sys_serdes_cfg[serdes_index].next_handle_index = digi_get_index_and_set_cpb_data(digi_handle, serdes_index, DIGI_SYS_SERDES_CFG, digi_resource_handle);
        }
    }

    PMC_RETURN(result);
} /* digi_is_enet_connected */

/*******************************************************************************
* digi_is_otn_connected
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Determines whether the serdes port handle is connected to the line otn
*   and if so then determines the connectivity of the underlying ho/mo/lo
*   order channels.
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance
*   serdes_index        - Index into the digi_handle for the serdes port
*                         handles which is the same index used in the
*                         handle summary structure for the associated 
*                         digi_serdes_pin_intf_ctxt_t structure.
*   intf                - Interface being used, Line, SFI51 or SYS
*   port_uid            - Serdes port identifier being looked up.
*
* OUTPUTS:
*   digi_resource_handle - Resource handle being updated.
*
* RETURNS:
*   TRUE  - Serdes Port handle is connected to line otn container.
*   FALSE - Otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 digi_is_otn_connected(digi_handle_t *digi_handle,
                                    UINT8 serdes_index,
                                    digi_serdes_intf_t intf,
                                    UINT32 port_uid,
                                    digi_resource_handle_summary_t *digi_resource_handle)
{
    BOOL8 result = FALSE;
    digi_otn_server_chnl_t *otn_chnl_ptr = NULL;
    UINT32 chnl_id;
    UINT8 i;
    PMC_ENTRY();

    /*
     * Now go look to find what this is connected to. On the line
     * side this could be lineotn, cbrc, or enet line.
     */
    for (i=0; i < DIGI_OTN_SERVER_CHNL_MAX; i++)
    {
        BOOL8 port_found = FALSE;

        if (DIGI_SERDES_SYSTEM_INTF == intf &&
            digi_handle->handle_pool.sysotn_server_chnl.header.prov_state != 
            UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG &&
            digi_handle->handle_pool.sysotn_server_chnl.port_uid == 
            port_uid)
        {
            otn_chnl_ptr = (digi_otn_server_chnl_t *)&(digi_handle->handle_pool.sysotn_server_chnl);
            chnl_id = ((digi_otn_server_chnl_def_t *)otn_chnl_ptr)->dci_chnl;
            port_found = TRUE;
        }
        else if (digi_handle->handle_pool.otn_server_chnl[i].header.prov_state != 
            UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG &&
            digi_handle->handle_pool.otn_server_chnl[i].port_uid ==
            port_uid)
        {
            /*
             * OTN line side port handle was found. Set the connection
             * pointers and break from the loop.
             * NOTE: FOR SYSOTN CASE THE LINE SIDE CHANNELS WILL BE 
             * CONNECTED DIRECTLY TO THE ODUK SWITCH AND NOT THE High 
             * Order OTN CONTAINER. IN THAT CASE THE NEXT RESOURCE HANDLE
             * SHOULD BE ODUK SWITCH AND THE NEXT_HANDLE_INDEX SHOULD BE
             * THE SWITCH INDICE. FOR THAT CASE OR POSSIBLY FOR A STRAIGHT
             * LINE SIDE HOW IS THAT DETERMINED?
             */
            otn_chnl_ptr = (digi_otn_server_chnl_t *)&(digi_handle->handle_pool.otn_server_chnl[i]);
            chnl_id = ((digi_otn_server_chnl_def_t *)otn_chnl_ptr)->dci_chnl;
            port_found = TRUE;
        }
        if (TRUE == port_found)
        {
            if (DIGI_SERDES_SFI_51_INTF == intf)
            {
                digi_resource_handle->sfi51_serdes_cfg[serdes_index].chnl_handle_type = DIGI_CHANNEL_HANDLE_ID_LINEOTN_T;
                digi_resource_handle->sfi51_serdes_cfg[serdes_index].chnl_handle_ptr = otn_chnl_ptr;
                digi_resource_handle->sfi51_serdes_cfg[serdes_index].next_resource_handle = DIGI_LINE_OTN_HO_CONTAINER;
                digi_resource_handle->sfi51_serdes_cfg[serdes_index].next_handle_index = chnl_id;
            }
            else if (DIGI_SERDES_LINE_INTF == intf)
            {
                digi_resource_handle->line_serdes_cfg[serdes_index].chnl_handle_type = DIGI_CHANNEL_HANDLE_ID_LINEOTN_T;
                digi_resource_handle->line_serdes_cfg[serdes_index].chnl_handle_ptr = otn_chnl_ptr;
                digi_resource_handle->line_serdes_cfg[serdes_index].next_resource_handle = DIGI_LINE_OTN_HO_CONTAINER;
                digi_resource_handle->line_serdes_cfg[serdes_index].next_handle_index = chnl_id;
            }
            else if (DIGI_SERDES_SYSTEM_INTF == intf)
            {
                if (DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == digi_handle->var.prod_app)
                {
                    digi_resource_handle->sys_serdes_cfg[serdes_index].chnl_handle_type = DIGI_CHANNEL_HANDLE_ID_LINEOTN_T;
                }
                else
                {
                    digi_resource_handle->sys_serdes_cfg[serdes_index].chnl_handle_type = DIGI_CHANNEL_HANDLE_ID_SYSOTN_T;
                }
                digi_resource_handle->sys_serdes_cfg[serdes_index].chnl_handle_ptr = otn_chnl_ptr;
                digi_resource_handle->sys_serdes_cfg[serdes_index].next_resource_handle = DIGI_SYS_OTN_HO_CONTAINER;
                digi_resource_handle->sys_serdes_cfg[serdes_index].next_handle_index = chnl_id;
            }            
            break;
        }        
    }    
    /*
     * If i is less than the constant then serdes port handle is
     * connected to lineotn/coreotn configuration. Now need to 
     * find the underlying ho/lo/mo sub_containers that may exist
     * for the provisioned lineotn resource.
     */
    if (i < DIGI_OTN_SERVER_CHNL_MAX)
    {
        odu_struct_t *odu_struct_ptr = NULL;
        result = TRUE;
        /*
         * For non-sysotn applications first connections are made with 
         * high order channels of COREOTN. SYSOTN connects to Hight Order
         * channels and performs at most one level of muxing. The LINEOTN
         * in this case uses the Low Order channels to connect to COREOTN
         * as the SYSOTN side will never use those channel handles.
         */
        if (TRUE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle) &&
            (DIGI_SERDES_LINE_INTF == intf ||
             DIGI_SERDES_SFI_51_INTF == intf))
        {
            odu_struct_ptr = &(digi_handle->handle_pool.lo_odu_struct[chnl_id]);
        }
        else
        {
            odu_struct_ptr = &(digi_handle->handle_pool.ho_odu_struct[chnl_id]);
        }
        
        /*
         * NOTE: The null check is a sanity check as this memory is 
         * static and will always point to something. For now the 
         * prov_state is used to reset the line/sfi51 side to map directly
         * to the ODUK Switch. It is however not a valid solution since
         * if we are SYSOTN the prov_state would be something other than
         * UNCONFIG. Need to be able to determine whether this structure
         * is used for LINEOTN or SYSOTN side setting. For completeness
         * included a case for LINEOTN as well. Note that if the prov_state
         * is unconfig for all interfaces then the connection would not 
         * be going to the switch. It would likely be not yet configured.
         */
        if (NULL == odu_struct_ptr ||
            odu_struct_ptr->switch_data.header.prov_state ==
            UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG)
        {
            if (DIGI_SERDES_SFI_51_INTF == intf)
            {
                digi_resource_handle->sfi51_serdes_cfg[serdes_index].next_resource_handle = LAST_DIGI_RESOURCE_HANDLE;
                digi_resource_handle->sfi51_serdes_cfg[serdes_index].next_handle_index = 0;
            }
            else if (DIGI_SERDES_LINE_INTF == intf)
            {
                digi_resource_handle->line_serdes_cfg[serdes_index].next_resource_handle = LAST_DIGI_RESOURCE_HANDLE;
                digi_resource_handle->line_serdes_cfg[serdes_index].next_handle_index = 0;
            }            
            else if (DIGI_SERDES_SYSTEM_INTF == intf)
            {
                digi_resource_handle->sys_serdes_cfg[serdes_index].next_resource_handle = LAST_DIGI_RESOURCE_HANDLE;
                digi_resource_handle->sys_serdes_cfg[serdes_index].next_handle_index = 0;
            }        
        } 
        else if (DIGI_SERDES_SFI_51_INTF == intf || 
                 DIGI_SERDES_LINE_INTF == intf)
        {
            digi_resource_handle->line_otn_ho_container_cfg[chnl_id].chnl_handle_ptr = (digi_otn_odu_chnl_t *)odu_struct_ptr;
            
            digi_resource_handle->line_otn_ho_container_cfg[chnl_id].handle_type = DIGI_CHANNEL_HANDLE_ID_HO_ODU_T;

            digi_resource_handle->line_otn_ho_container_cfg[chnl_id].parent_handle_type = DIGI_LINE_SERDES_CFG;

            if (DIGI_SERDES_SFI_51_INTF == intf)
            {
                digi_resource_handle->line_otn_ho_container_cfg[chnl_id].parent_handle_type = DIGI_SFI51_SERDES_CFG;
            }
            digi_resource_handle->line_otn_ho_container_cfg[chnl_id].parent_handle_index = serdes_index;
            digi_resource_handle->line_otn_ho_container_cfg[chnl_id].payload_mode = (digi_odu_line_payload_t)odu_struct_ptr->mem_ptr->payload_format;
        }
        else if (DIGI_SERDES_SYSTEM_INTF == intf)
          {
            /*
             * This is a system side interface but is it a SYSOTN interface.
             * If sysotn server is not configured then this must be the
             * 100G x 3x40G configuration. For this use the line otn
             * configuration
             */
            if (UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG != digi_handle->handle_pool.sysotn_server_chnl.header.prov_state)
            {
                digi_resource_handle->sys_otn_ho_container_cfg[0].chnl_handle_ptr = (digi_otn_odu_chnl_t *)odu_struct_ptr;
                digi_resource_handle->sys_otn_ho_container_cfg[0].handle_type = DIGI_CHANNEL_HANDLE_ID_HO_ODU_T;
                digi_resource_handle->sys_otn_ho_container_cfg[0].payload_mode = (digi_odu_line_payload_t)odu_struct_ptr->mem_ptr->payload_format;
                digi_resource_handle->sys_otn_ho_container_cfg[0].parent_handle_type = DIGI_SYS_SERDES_CFG;
                digi_resource_handle->sys_otn_ho_container_cfg[0].parent_handle_index = serdes_index;
            }
            else
            {
                digi_resource_handle->sys_otn_ho_container_cfg[chnl_id].chnl_handle_ptr = (digi_otn_odu_chnl_t *)odu_struct_ptr;
                digi_resource_handle->sys_otn_ho_container_cfg[chnl_id].handle_type = DIGI_CHANNEL_HANDLE_ID_HO_ODU_T;
                digi_resource_handle->sys_otn_ho_container_cfg[chnl_id].payload_mode = (digi_odu_line_payload_t)odu_struct_ptr->mem_ptr->payload_format;
                digi_resource_handle->sys_otn_ho_container_cfg[chnl_id].parent_handle_type = DIGI_SYS_SERDES_CFG;
                digi_resource_handle->sys_otn_ho_container_cfg[chnl_id].parent_handle_index = serdes_index;
            }
        }
    }

    PMC_RETURN(result);
} /*digi_is_otn_connected */


/*******************************************************************************
* digi_line_otn_subcontainer_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the underlying Mid Order and low order channel sub containers
*   based on the passed in high order channel pointer.
*
* INPUTS:
*  *digi_handle        - pointer to DIGI handle instance
*  ho_index            - Index into the line_otn_ho_container_cfg container
*                        in the digi_resource_handle_summary_t table.
*
* OUTPUTS:
*   digi_resource_handle - Resource handle being updated.
*   ho_last_index        - Set to the indice of the current high order 
*                          channel.  Used as the indice of the previous
*                          high order channel for setting the associate
*                          index of the handle if not last resource.
*   ho_last_handle       - Set to HO order channel if one is found and
*                          with last_index sets the previous associate 
*                          value if not last resource.
*
* RETURNS:
*   PMC_SUCCESS: Operation was successful.
*   DIGI_ERR_UNKNOWN - ODU level is not HO or MO.
*   PMC_ERROR: Error returned from coreotn ho or mo calendar entry 
*              retrieval.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_line_otn_subcontainer_set(digi_handle_t *digi_handle,
                                                 UINT8 ho_index,
                                                 digi_resource_handle_summary_t *digi_resource_handle,
                                                 UINT16 *ho_last_index,
                                                 digi_resource_next_handle_t *ho_last_handle)
{ 
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 ho_cal_array[DIGI_OTN_SERVER_MO_CHNL_MAX];
    UINT32 num_cal_entries = 0;
    odu_struct_t *ho_odu_struct_ptr = NULL;
    UINT8 i;
    UINT8 k;

    PMC_ENTRY();

    if (DIGI_ODU_UNCHANNELIZED == digi_resource_handle->line_otn_ho_container_cfg[ho_index].payload_mode)
    {
        if (LAST_DIGI_RESOURCE_HANDLE != *ho_last_handle)
        {
            digi_resource_handle->line_otn_ho_container_cfg[*ho_last_index].next_associate_index = ho_index;
            digi_resource_handle->line_otn_ho_container_cfg[*ho_last_index].next_associate_handle = *ho_last_handle;
        }
        *ho_last_index = ho_index;
        *ho_last_handle = DIGI_LINE_OTN_HO_CONTAINER;

        digi_resource_handle->line_otn_ho_container_cfg[ho_index].next_resource_handle = DIGI_ODUK_SWITCH;
        PMC_RETURN(PMC_SUCCESS);
    }
    ho_odu_struct_ptr = &(digi_handle->handle_pool.ho_odu_struct[ho_index]);
    /*
     * Retrieve the sub container channels associated with the high order
     * level and channel id.  The coreotn function returns all Mid Order
     * containers regardless of whether they are uni or bi-directional
     */
    coreotn_lo_chnl_get(digi_handle->coreotn_handle,
                                 TRUE,
                                 COREOTN_CHNL_ORDER_HO,
                                 ho_odu_struct_ptr->mem_ptr->chnl_id,
                                 ho_cal_array, 
                                 &num_cal_entries);

    if (PMC_SUCCESS == result && num_cal_entries > 0)
    {
        UINT16 last_index = 0;
        digi_resource_next_handle_t last_handle = LAST_DIGI_RESOURCE_HANDLE;
        /*
         * Update the next_associate_index and handle if there is more than
         * one High order channel. This operation sets the current channel
         * in the previous entries index.  This is done for High/Mid/Low
         * order channel handles. 
         */
        if (LAST_DIGI_RESOURCE_HANDLE != *ho_last_handle)
        {
            digi_resource_handle->line_otn_ho_container_cfg[*ho_last_index].next_associate_index = ho_index;
            digi_resource_handle->line_otn_ho_container_cfg[*ho_last_index].next_associate_handle = *ho_last_handle;
        }
        *ho_last_index = ho_index;
        *ho_last_handle = DIGI_LINE_OTN_HO_CONTAINER;

        /*
         * Iterate over the calendar entries based on whether the channel
         * handle is active or not.
         */
        for (i = 0; i < num_cal_entries; i++)
        {
            UINT32 mo_chnl_id = ho_cal_array[i];
            odu_struct_t *mo_odu_struct_ptr = &(digi_handle->handle_pool.mo_odu_struct[mo_chnl_id]);
            
            if (mo_odu_struct_ptr && TRUE == mo_odu_struct_ptr->active)
            {
                /*
                 * If this is the first time through this High Order 
                 * container then set its next_resource_handle and next
                 * handle index
                 */
                if (LAST_DIGI_RESOURCE_HANDLE == digi_resource_handle->line_otn_ho_container_cfg[ho_index].next_resource_handle)
                {
                    digi_resource_handle->line_otn_ho_container_cfg[ho_index].next_resource_handle = DIGI_LINE_OTN_MO_CONTAINER;
                    digi_resource_handle->line_otn_ho_container_cfg[ho_index].next_handle_index = mo_chnl_id;
                }
                /*
                 * Increment the number of Low order channels under this
                 * High order container.
                 */
                digi_resource_handle->line_otn_ho_container_cfg[ho_index].lo_chnl_entries++;

                if (LAST_DIGI_RESOURCE_HANDLE != last_handle)
                {
                    digi_resource_handle->line_otn_mo_container_cfg[last_index].next_associate_handle = last_handle;
                    digi_resource_handle->line_otn_mo_container_cfg[last_index].next_associate_index = mo_chnl_id;
                }
                last_handle = DIGI_LINE_OTN_MO_CONTAINER;
                last_index = mo_chnl_id;
                /*
                 * Assign the parent index and type
                 */
                digi_resource_handle->line_otn_mo_container_cfg[mo_chnl_id].parent_handle_index = ho_index;
                digi_resource_handle->line_otn_mo_container_cfg[mo_chnl_id].parent_handle_type = DIGI_LINE_OTN_HO_CONTAINER;
                /*
                 * Set the type of this container and the channel handle 
                 * pointer taken from the digi_handle
                 */
                digi_resource_handle->line_otn_mo_container_cfg[mo_chnl_id].handle_type = DIGI_CHANNEL_HANDLE_ID_MO_ODU_T;
                digi_resource_handle->line_otn_mo_container_cfg[mo_chnl_id].chnl_handle_ptr = mo_odu_struct_ptr;
                /*
                 * Copy over the tributary slot mask for this channel.
                 */
                for (k=0; k < 3; k++)
                {
                    digi_resource_handle->line_otn_mo_container_cfg[mo_chnl_id].tx_trib_slot_mask[k] = mo_odu_struct_ptr->mem_ptr->tx_trib_loc_mask[k];
                    digi_resource_handle->line_otn_mo_container_cfg[mo_chnl_id].rx_trib_slot_mask[k] = mo_odu_struct_ptr->mem_ptr->rx_trib_loc_mask[k];
                }

                /*
                 * Set the payload mode for this channel. With the 
                 * tributary slots retrieve from the ODU struct should 
                 * allow a user to determine the physical resource 
                 * associated with this channel.
                 */
                digi_resource_handle->line_otn_mo_container_cfg[mo_chnl_id].payload_mode = (digi_odu_line_payload_t)mo_odu_struct_ptr->mem_ptr->payload_format;

                if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE ==
                    mo_odu_struct_ptr->switch_data.oduk_data.op_state.active_state)
                {
                    digi_resource_handle->line_otn_mo_container_cfg[mo_chnl_id].next_resource_handle = DIGI_ODUK_SWITCH;
                }
                else
                {
                    /*
                     * Now set the lower order channels for this mid order 
                     * if there are any.
                     */
                    result = digi_lo_otn_subcontainer_set(digi_handle,
                                                          mo_chnl_id,
                                                          digi_resource_handle);
                }
            }
        }
    }
    else if (PMC_SUCCESS != result || 0 == num_cal_entries)
    {
        /*
         * If we are here something is wrong. We are unchannelized and
         * if success then should have calendar entries.
         */
        PMC_LOG_TRACE("Result=%d.Calendar Entries=%d. ERROR!!\n", result,
                      num_cal_entries);
    }

    PMC_RETURN(result);
} /* digi_line_otn_subcontainer_set */


/*******************************************************************************
* digi_sys_otn_subcontainers_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the underlying Mid Order and low order channel sub containers
*   based on the passed in high order channel pointer.
*
* INPUTS:
*  *digi_handle        - pointer to DIGI handle instance
*  ho_index            - Index into the sys_otn_ho_container_cfg container
*                        in digi_resource_handle_summary_t table.
*
* OUTPUTS:
*   digi_resource_handle - Resource handle being updated.
*
* RETURNS:
*   PMC_SUCCESS: Operation was successful.
*   DIGI_ERR_UNKNOWN - ODU level is not HO or MO.
*   PMC_ERROR: Error returned from coreotn ho or mo calendar entry 
*              retrieval.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_sys_otn_subcontainer_set(digi_handle_t *digi_handle,
                                                UINT8 ho_index,
                                                digi_resource_handle_summary_t *digi_resource_handle)
{ 
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 ho_cal_array[DIGI_OTN_SERVER_MO_CHNL_MAX];
    UINT32 num_cal_entries = 0;
    odu_struct_t *ho_odu_struct_ptr = NULL;
    UINT8 i;
    UINT32 index;

    PMC_ENTRY();

    if (TRUE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle))
    {
        index = 0;
    }
    else
    {
        index = ho_index;
    }

    if (DIGI_ODU_UNCHANNELIZED == digi_resource_handle->sys_otn_ho_container_cfg[index].payload_mode)
    {
        digi_resource_handle->sys_otn_ho_container_cfg[index].next_resource_handle = DIGI_ODUK_SWITCH;
        PMC_RETURN(result);
    }
    ho_odu_struct_ptr = &(digi_handle->handle_pool.ho_odu_struct[ho_index]);

    /*
     * Retrieve the sub container channels associated with the high order
     * level and channel id.  The coreotn function returns all Mid Order
     * containers regardless of whether they are uni or bi-directional
     */
    coreotn_lo_chnl_get(digi_handle->coreotn_handle,
                        FALSE,
                        COREOTN_CHNL_ORDER_HO,
                        ho_odu_struct_ptr->mem_ptr->chnl_id,
                        ho_cal_array, 
                        &num_cal_entries);

    if (PMC_SUCCESS == result && num_cal_entries > 0 )
    {
        UINT16 last_index = 0;
        digi_resource_next_handle_t last_handle = LAST_DIGI_RESOURCE_HANDLE;
        UINT8 k = 0;
        /*
         * Now go through the calendar entries and populate the
         * low order channels
         */
        for (i = 0; i < num_cal_entries; i++)
        {
            UINT8 mo_chnl_id = ho_cal_array[i];
            odu_struct_t *mo_odu_struct_ptr = &(digi_handle->handle_pool.mo_odu_struct[mo_chnl_id]);

            PMC_LOG_TRACE("NumCalendarEntries=%d. SYS MO ChannelId=%d\n",
                          num_cal_entries, mo_chnl_id);
            
            if (mo_odu_struct_ptr && TRUE == mo_odu_struct_ptr->active)
            {
                /*
                 * If this is the first time through this High Order 
                 * container then set its next_resource_handle and next
                 * handle index
                 */
                if (LAST_DIGI_RESOURCE_HANDLE == digi_resource_handle->sys_otn_ho_container_cfg[index].next_resource_handle)
                {
                    if ( DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == digi_handle->var.prod_app)
                    {
                        digi_resource_handle->sys_otn_ho_container_cfg[index].next_resource_handle = DIGI_LINE_OTN_MO_CONTAINER;
                    }
                    else
                    {
                        digi_resource_handle->sys_otn_ho_container_cfg[index].next_resource_handle = DIGI_SYS_OTN_MO_CONTAINER;
                    }
                    digi_resource_handle->sys_otn_ho_container_cfg[index].next_handle_index = mo_chnl_id;
                }
                digi_resource_handle->sys_otn_ho_container_cfg[index].lo_chnl_entries++;
                /*
                 * Assign the associate handle index if the last_handle was
                 * set to a non-default value. Namely this is not the first
                 * lower order channel being configured. Then grab the
                 * current handle and index to be assigned the next time
                 * through the loop.
                 */
                if (LAST_DIGI_RESOURCE_HANDLE != last_handle)
                {
                    digi_resource_handle->sys_otn_mo_container_cfg[last_index].next_associate_handle = last_handle;
                    digi_resource_handle->sys_otn_mo_container_cfg[last_index].next_associate_index = mo_chnl_id;
                    PMC_LOG_TRACE("LastIndex=%d and Current MoChnlid=%d\n",
                                  last_index, mo_chnl_id);
                }
                last_handle = DIGI_SYS_OTN_MO_CONTAINER;
                last_index = mo_chnl_id;
                /*
                 * Assign the parent index and type
                 */
                digi_resource_handle->sys_otn_mo_container_cfg[mo_chnl_id].parent_handle_index = ho_index;
                digi_resource_handle->sys_otn_mo_container_cfg[mo_chnl_id].parent_handle_type = DIGI_SYS_OTN_HO_CONTAINER;
                /*
                 * Set the type of this container and the channel handle 
                 * pointer taken from the digi_handle
                 */
                digi_resource_handle->sys_otn_mo_container_cfg[mo_chnl_id].handle_type = DIGI_CHANNEL_HANDLE_ID_MO_ODU_T;
                digi_resource_handle->sys_otn_mo_container_cfg[mo_chnl_id].chnl_handle_ptr = mo_odu_struct_ptr;
                /*
                 * Copy over the tributary slot mask for this channel.
                 */
                for (k=0; k < 3; k++)
                {
                    digi_resource_handle->sys_otn_mo_container_cfg[mo_chnl_id].tx_trib_slot_mask[k] = mo_odu_struct_ptr->mem_ptr->tx_trib_loc_mask[k];
                    digi_resource_handle->sys_otn_mo_container_cfg[mo_chnl_id].rx_trib_slot_mask[k] = mo_odu_struct_ptr->mem_ptr->rx_trib_loc_mask[k];
                }
                /*
                 * Set the payload mode for this channel. This should be
                 * UNCHANNELIZED as we are going directly to the switch 
                 * from here.
                 */
                digi_resource_handle->sys_otn_mo_container_cfg[mo_chnl_id].payload_mode = (digi_odu_line_payload_t)mo_odu_struct_ptr->mem_ptr->payload_format;
                if (DIGI_ODU_UNCHANNELIZED == digi_resource_handle->sys_otn_mo_container_cfg[mo_chnl_id].payload_mode)
                {
                    digi_resource_handle->sys_otn_mo_container_cfg[mo_chnl_id].lo_chnl_entries = 0;
                    digi_resource_handle->sys_otn_mo_container_cfg[mo_chnl_id].next_resource_handle = DIGI_ODUK_SWITCH;
                }
                else if (DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == digi_handle->var.prod_app && DIGI_ODU_NULL != digi_resource_handle->sys_otn_mo_container_cfg[mo_chnl_id].payload_mode)
                {
                    result = digi_lo_otn_subcontainer_set(digi_handle,
                                                          mo_chnl_id,
                                                          digi_resource_handle);
                }
                else if (DIGI_ODU_NULL !=  digi_resource_handle->sys_otn_mo_container_cfg[mo_chnl_id].payload_mode)
                {
                    PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
                }
            }
        }
    }
    else if (PMC_SUCCESS != result || 0 == num_cal_entries)
    {
        /*
         * If we are here something is wrong. We are unchannelized and
         * if success then should have calendar entries.
         */
        PMC_LOG_TRACE("Result=%d.Calendar Entries=%d. ERROR!!\n", 
                      result, num_cal_entries);
    }
    PMC_RETURN(result);
} /* digi_sys_otn_container_set */


/*******************************************************************************
* digi_lo_otn_subcontainers_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Takes the MO channel and determines whether or not there are any lo order
*   channels under it. If there are then configures the underlying lo order
*   channels accordingly.
*
* INPUTS:
*  *digi_handle        - pointer to DIGI handle instance
*  mo_index            - Index into the line_otn_mo_container_cfg container
*                        in the digi_resource_handle_summary_t table.
*
* OUTPUTS:
*   digi_resource_handle - Resource handle being updated.
*
* RETURNS:
*   PMC_SUCCESS: Operation was successful.
*   PMC_ERROR: Error returned from coreotn ho or mo calendar entry 
*              retrieval.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_lo_otn_subcontainer_set(digi_handle_t *digi_handle,
                                               UINT8 mo_index,
                                               digi_resource_handle_summary_t *digi_resource_handle)
{
    odu_struct_t *mo_odu_struct_ptr = NULL;
    UINT32 mo_cal_array[DIGI_OTN_SERVER_LO_CHNL_MAX];
    UINT32 num_cal_entries = 0;
    digi_otn_container_cfg_t *otn_container_ptr = NULL;

    PMC_ENTRY();

    if (DIGI_PROD_APP_100G_3_X_40G_CFP_CARD != digi_handle->var.prod_app)
    {
        if (DIGI_ODU_UNCHANNELIZED == digi_resource_handle->line_otn_mo_container_cfg[mo_index].payload_mode)
        {
            digi_resource_handle->line_otn_mo_container_cfg[mo_index].next_resource_handle = DIGI_ODUK_SWITCH;
            PMC_RETURN(PMC_SUCCESS);
        } 
        otn_container_ptr = &digi_resource_handle->line_otn_mo_container_cfg[mo_index];
    }
    else 
    {
         otn_container_ptr = &digi_resource_handle->sys_otn_mo_container_cfg[mo_index];
    }
    mo_odu_struct_ptr = (odu_struct_t *)otn_container_ptr->chnl_handle_ptr;
    /*
    * Retrieve the sub container channels associated with the high order
     * level and channel id.  The subcontainers are direction relative. For
     * bi-directional connections the TX and RX will be the same and as such
     * the TX call should handle all the set up required. For different
     * subcontainers for RX and TX then need to handle each separately.
     * For each direction if there are no subcontainers then there is
     * nothing to do. For TX only the TX handles need be checked since RX
     * call will follow. For RX however need to retrieve the TX and 
     * determine whether the current RX channel exists in the TX array 
     * indicate it is bi-directional and not uniquely RX.
     */
    coreotn_lo_chnl_get(digi_handle->coreotn_handle,
                                 TRUE,
                                 COREOTN_CHNL_ORDER_MO,
                                 mo_odu_struct_ptr->mem_ptr->chnl_id,
                                 mo_cal_array, 
                                 &num_cal_entries);

    if (num_cal_entries > 0)
    {
        UINT8 i = 0;
        UINT8 k = 0;
        UINT16 last_index = 0;
        digi_resource_next_handle_t last_handle = LAST_DIGI_RESOURCE_HANDLE;
        /*
         * Now go through the calendar entries and populate the
         * low order channel array.
         * For the lower order channels the array should be a listing of
         * the channels associated with the mid-order (higher order) parent
         * That is if the parent has 2 lower order channels there should be
         * 2 channels returned.
         */
        for (i = 0; i < num_cal_entries; i++)
        {
            UINT32 lo_chnl_id = mo_cal_array[i];
            odu_struct_t *lo_odu_struct_ptr = &(digi_handle->handle_pool.lo_odu_struct[lo_chnl_id]);

            if (lo_odu_struct_ptr && TRUE == lo_odu_struct_ptr->active)
            {
                if (LAST_DIGI_RESOURCE_HANDLE == otn_container_ptr->next_resource_handle)
                {
                    otn_container_ptr->next_resource_handle = DIGI_LINE_OTN_LO_CONTAINER;
                    otn_container_ptr->next_handle_index = lo_chnl_id;
                }
                otn_container_ptr->lo_chnl_entries++;

                digi_resource_handle->line_otn_lo_container_cfg[lo_chnl_id].parent_handle_index = mo_index;
                digi_resource_handle->line_otn_lo_container_cfg[lo_chnl_id].parent_handle_type = DIGI_LINE_OTN_MO_CONTAINER;
                /*
                 * Copy over the tributary slot mask for this channel.
                 */
                for (k=0; k < 3; k++)
                {
                    digi_resource_handle->line_otn_lo_container_cfg[lo_chnl_id].tx_trib_slot_mask[k] = lo_odu_struct_ptr->mem_ptr->tx_trib_loc_mask[k];
                    digi_resource_handle->line_otn_lo_container_cfg[lo_chnl_id].rx_trib_slot_mask[k] = lo_odu_struct_ptr->mem_ptr->rx_trib_loc_mask[k];
                }
                /*
                 * Assign the associated handle. If the last handle has
                 * been set then go back to the previous lo container and
                 * set the associate index and handle to point to this one.
                 * Then assign the index and handle to the current one.
                 */
                if (LAST_DIGI_RESOURCE_HANDLE != last_handle)
                {
                    digi_resource_handle->line_otn_lo_container_cfg[last_index].next_associate_handle = last_handle;
                    digi_resource_handle->line_otn_lo_container_cfg[last_index].next_associate_index = lo_chnl_id;
                }
                last_index = lo_chnl_id;
                last_handle = DIGI_LINE_OTN_LO_CONTAINER;

                digi_resource_handle->line_otn_lo_container_cfg[lo_chnl_id].handle_type = DIGI_CHANNEL_HANDLE_ID_LO_ODU_T;
                digi_resource_handle->line_otn_lo_container_cfg[lo_chnl_id].chnl_handle_ptr = lo_odu_struct_ptr;
                digi_resource_handle->line_otn_lo_container_cfg[lo_chnl_id].lo_chnl_entries = 0;
                digi_resource_handle->line_otn_lo_container_cfg[lo_chnl_id].payload_mode = (digi_odu_line_payload_t)lo_odu_struct_ptr->mem_ptr->payload_format;
                digi_resource_handle->line_otn_lo_container_cfg[lo_chnl_id].next_resource_handle = DIGI_ODUK_SWITCH;
                digi_resource_handle->line_otn_lo_container_cfg[lo_chnl_id].payload_mode = (digi_odu_line_payload_t)lo_odu_struct_ptr->mem_ptr->payload_format;
            }
        }
    }
    else if (0 == num_cal_entries)
    {
        PMC_LOG(PMC_LOG_SEV_MEDIUM, 0, 0, 0,
                "Mid Order Index = %d has no lower order containers\n",
                mo_index);
    }
    PMC_RETURN(PMC_SUCCESS);
} /* digi_lo_otn_subcontainer_set */


/*******************************************************************************
* digi_resource_connect_switch_data
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Connect ODUK Switch related resources. Iterates over the various 
*  COREOTN subcontainers looking for next_resource_handle equal to ODUK
*  switch and then determines the ODUK switch channel for that container.
*  CPB switch is handle directly at the level of the subcontainer that 
*  connects to it.
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance
*
* OUTPUTS:
*   digi_resource_handle - Resource handle being updated.
*
* RETURNS:
*   PMC_SUCCESS: Operation completed successfully
*   PMC_ERROR:   Returns the error that reflects the issue that occurred
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_connect_switch_data(digi_handle_t *digi_handle,
                                               digi_resource_handle_summary_t *digi_resource_handle)
{
    UINT8 i = 0;

    PMC_ENTRY();

    for (i=0; i < DIGI_OTN_SERVER_LO_CHNL_MAX; i++)
    {
        if (i < DIGI_OTN_SERVER_HO_CHNL_MAX &&
            DIGI_ODUK_SWITCH == digi_resource_handle->line_otn_ho_container_cfg[i].next_resource_handle)
        {
            digi_resource_handle->line_otn_ho_container_cfg[i].next_handle_index = digi_get_index_and_set_oduk_data(digi_handle, i, DIGI_LINE_OTN_HO_CONTAINER, digi_resource_handle);
        }
        if (DIGI_ODUK_SWITCH == digi_resource_handle->line_otn_mo_container_cfg[i].next_resource_handle)
        {
            digi_resource_handle->line_otn_mo_container_cfg[i].next_handle_index = digi_get_index_and_set_oduk_data(digi_handle, i, DIGI_LINE_OTN_MO_CONTAINER, digi_resource_handle);
        }
        if (DIGI_ODUK_SWITCH == digi_resource_handle->line_otn_lo_container_cfg[i].next_resource_handle)
        {
            digi_resource_handle->line_otn_lo_container_cfg[i].next_handle_index = digi_get_index_and_set_oduk_data(digi_handle, i, DIGI_LINE_OTN_LO_CONTAINER, digi_resource_handle);
        }
        if (DIGI_ODUK_SWITCH == digi_resource_handle->sys_otn_mo_container_cfg[i].next_resource_handle)
        {
            digi_resource_handle->sys_otn_mo_container_cfg[i].next_handle_index = digi_get_index_and_set_oduk_data(digi_handle, i, DIGI_SYS_OTN_MO_CONTAINER, digi_resource_handle);
        }
    }

    if (TRUE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle))
    {
        if (DIGI_ODUK_SWITCH == digi_resource_handle->sys_otn_ho_container_cfg[0].next_resource_handle)
        {
            digi_resource_handle->sys_otn_ho_container_cfg[0].next_handle_index = digi_get_index_and_set_oduk_data(digi_handle, 0, DIGI_SYS_OTN_HO_CONTAINER, digi_resource_handle);   
        }
    }
    else
    {
        for (i=0; i < DIGI_OTN_SERVER_HO_CHNL_MAX; i++)
        {
            if (DIGI_ODUK_SWITCH == digi_resource_handle->sys_otn_ho_container_cfg[i].next_resource_handle)
            {
                digi_resource_handle->sys_otn_ho_container_cfg[i].next_handle_index = digi_get_index_and_set_oduk_data(digi_handle, i, DIGI_SYS_OTN_HO_CONTAINER, digi_resource_handle); 
            }
        }
    }

    PMC_RETURN();
} /* digi_resource_connect_switch_data */

#ifdef PMC_DEPRECATED

/* LCOV_EXCL_START */

/*******************************************************************************
* digi_resource_clean_dcsu
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Calls dcsu_conditional_unit for an DCSU inst that could have been left
*  behind in a restart scenario. It is posible that there are no ENET LINE
*  resources but that the ENET1 or ENET DCSU register setting set.
*
* INPUTS:
*   digi_handle  - Reference to the digi_handle
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
PRIVATE void digi_resource_clean_dcsu(digi_handle_t *digi_handle)
{
    PMC_ENTRY();
    /*
     * Call digi_dcsu_conditional_uninit to uninitialize the DCSU 
     * associated with the particular resource.  The DCSU is uninitialized
     * only if there are no associated resources which is checked in the
     * function below. Note that checking  the state of the DCSU first
     * would not be reliable as the context and the HW are likely not in
     * sync.
     */
    digi_dcsu_conditional_uninit(digi_handle, DIGI_DCSU_INST_OTN2);
    digi_dcsu_conditional_uninit(digi_handle, DIGI_DCSU_INST_ENET1);
    digi_dcsu_conditional_uninit(digi_handle, DIGI_DCSU_INST_ENET2);
    digi_dcsu_conditional_uninit(digi_handle, DIGI_DCSU_INST_ENET3);

    PMC_RETURN();
}
/* LCOV_EXCL_STOP */

#endif /* PMC_DEPRECATED */

/*******************************************************************************
* digi_resource_clean_firmware
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Called within digi_ressource_clean. Cancel active firmware processes 
*  associated to cleaned ressources 
*  
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*
* OUTPUTS:
    None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_clean_firmware(digi_handle_t *digi_handle)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 itr = 0;
    BOOL8 is_enet_line;
    digi_trunk2client_tables_cmf_t  trunk2client_tables;
	digi_client2trunk_tables_cmf_t  client2trunk_tables;
    util_global_switch_data_t *enet_chnl_ptr[DIGI_ENET_CHNL_MAX] = { NULL };
    util_global_switch_data_t* enet_switch_data_ptr;

    PMC_ENTRY();

    UINT8 cmf_payload[8] = {0,0,0,0,0,0,0,0};


    
    /* 3- Intrusive Monitor */
     rc = PMC_SUCCESS;
	 memset(&client2trunk_tables,0,sizeof(digi_client2trunk_tables_cmf_t));    
     for(itr = 0;itr < DIGI_ENET_LINE_CHNL_MAX; itr++)
     {
         if (TRUE != util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.enet_line_chnl[itr].switch_data.header.prov_state))
         {
             if( digi_handle->var.cbrc_intf == CBRC_SERDES_INTF_SYS ||
                 (TRUE != util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.cbr_chnl[itr].switch_data.header.prov_state)))
             {
                 (void) digi_cmf_clienttotrunk_port_action_table_internal_set(digi_handle,
                                                                              itr,
                                                                              0,0,0,0,0,0,0,0,0,0,&cmf_payload[0]);
                 (void) digi_cmf_clienttotrunk_conseq_action_table_internal_set(digi_handle,
                                                                                itr,
                                                                                &client2trunk_tables.conseq_action_cmf);     
                 (void) digi_cmf_clienttotrunk_tables_update_internal(digi_handle,itr);  
             }
         }
     }
     
     memset(&trunk2client_tables, 0, sizeof(digi_trunk2client_tables_cmf_t));
     for(itr = 0;itr < DIGI_MAPOTN_CHNL_MAX; itr++)
     {
         if (TRUE != util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.mapper_chnl[itr].switch_data.header.prov_state))
         {
             /* clear all consequential actions */
             (void) digi_cmf_trunktoclient_conseq_action_table_internal_set(digi_handle,
                                                                            itr,
                                                                            &trunk2client_tables); 
             /* update table */
             digi_handle->handle_pool.mapper_chnl[itr].mapotn_chnl = itr;
             (void) digi_cmf_trunktoclient_tables_update(digi_handle, (digi_mapper_chnl_t *)&digi_handle->handle_pool.mapper_chnl[itr]);             
         }         
     }

     /* Resync FW context */
     rc = digi_fw_enet_client_source_get(digi_handle,&is_enet_line);
     if (PMC_SUCCESS == rc)
     {
         BOOL8 bArraySet = FALSE;

         if ((FALSE == enet_start_state_test(digi_handle->enet_line_handle) && is_enet_line == TRUE) ||
             (FALSE == enet_start_state_test(digi_handle->enet_sys_handle) && is_enet_line == FALSE))
         {
    
             /*enet_chnl = (TRUE == is_enet_line)?
                 (util_global_switch_data_t*)&(digi_handle->handle_pool.enet_line_chnl[0]):
                 (util_global_switch_data_t*)&(digi_handle->handle_pool.enet_sys_chnl[0]);*/
             
             for(itr = 0;itr < DIGI_ENET_LINE_CHNL_MAX; itr++)
             {
                 if (TRUE == is_enet_line)
                 {
                     enet_switch_data_ptr = ((util_global_switch_data_t*)&(digi_handle->handle_pool.enet_line_chnl[itr]));
                 }
                 else {
                     enet_switch_data_ptr = ((util_global_switch_data_t*)&(digi_handle->handle_pool.enet_sys_chnl[itr]));
                 }

                 if (TRUE != util_global_rx_or_tx_chnl_prov_status_get(((util_global_switch_data_def_t*)enet_switch_data_ptr)->header.prov_state))
                 {
                     /*ch_disable_bit_msk |= (1 << itr);*/
                     enet_chnl_ptr[itr] = enet_switch_data_ptr;

                     if (NULL != enet_chnl_ptr[itr])
                     {
                         bArraySet = TRUE;
                     }
                 }
             }
         }
         /* SYNCE is disabled on unprovisionned channels */
         if (TRUE == bArraySet)
         {
             (void) digi_timesync_synce_ch_en(digi_handle, enet_chnl_ptr, FALSE);
         }
     }

     /* reset unused serdes */
     digi_serdes_cleanup(digi_handle);

     PMC_RETURN();
} /* digi_resource_clean_firmware */

/*******************************************************************************
* digi_resource_clean_post_mld_dsi_ctxt
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Cleanup digi_handle post_mld_dsi_ctxt to avoid any incoherencies that can prevent
*  customer from provisionning a cleanup datapath.
*
* INPUTS:
*   digi_handle  - Reference to the digi_handle.
*   intf         - Post MLD context type: LINE or SYSTEM.
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
PRIVATE void digi_resource_clean_post_mld_dsi_ctxt(digi_handle_t      *digi_handle,
                                                   digi_serdes_intf_t  intf)
{
    BOOL8 skip_func = FALSE;
    BOOL8 skip_cleanup;
    UINT8 i = 0; /* counter */
    UINT32 jtr;
    digi_post_mld_dsi_ctxt_t *post_mld_dsi_ctxt_ptr;
    digi_enet_client_chnl_def_t *enet_chnl_handle_ptr;
    digi_cbr_client_chnl_def_t *cbrc_chnl_handle_ptr;
    digi_otn_server_chnl_def_t  *otn_server_chnl_ptr;
    BOOL8 is_enet_line;
    UINT32 max_num_chnl;
    BOOL8 is_sysotn;
  
    PMC_ENTRY();

    switch (intf)
    {
    case DIGI_SERDES_LINE_INTF:
        post_mld_dsi_ctxt_ptr = &digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[0];
        break;
    case DIGI_SERDES_SYSTEM_INTF:
        post_mld_dsi_ctxt_ptr = &digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[0];
        break;
    default:
        skip_func = TRUE;
    }

     if (FALSE == skip_func)
    {
        for (i = 0; i < DIGI_SERDES_DSI_LANES_MAX; i ++) {            
            skip_cleanup = TRUE;
            is_enet_line = FALSE;
            is_sysotn = FALSE;
            /* Check the line side first */
            switch (post_mld_dsi_ctxt_ptr[i].prov_state)
            {
                /* Check that post mld lane is connected to a SYSOTN or LINEOTN channel */
            case DIGI_SERDES_PROV_STATE_SYSOTN:
                is_sysotn = TRUE;
            case DIGI_SERDES_PROV_STATE_LINEOTN:
                if (FALSE == is_sysotn)
                {                    
                    otn_server_chnl_ptr = &digi_handle->handle_pool.otn_server_chnl[0];
                    max_num_chnl = DIGI_OTN_SERVER_CHNL_MAX;
                }
                else
                {
                    otn_server_chnl_ptr = &digi_handle->handle_pool.sysotn_server_chnl;
                    max_num_chnl = 1;
                }
                for (jtr = 0; jtr < max_num_chnl; jtr += 1)
                {

                    if (UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG != otn_server_chnl_ptr[jtr].header.prov_state)
                    {
                        if (post_mld_dsi_ctxt_ptr[i].port_uid == otn_server_chnl_ptr[jtr].port_uid)
                        {
                            break;
                        }
                    }
                }
                if (jtr >= max_num_chnl)
                {
                    skip_cleanup = FALSE;
                }
                else if (TRUE == skip_cleanup)
                {                    
                    /* if no MAPOTN channel is associated to post mld lane, cleanup num_odu0 and map_type in lane context */                       
                    if (FALSE == digi_resource_post_mld_lane_mapotn_chnl_status_get(digi_handle,
                                                                                    intf,
                                                                                    i))
                    {
                        post_mld_dsi_ctxt_ptr[i].num_odu0 = 0;
                        post_mld_dsi_ctxt_ptr[i].map_type = DIGI_MAPPER_ALLOC_UNUSED;
                    }            
                }                                 
                break;
                /* Check that post mld lane is connected to a ENET LINE or SYS channel */
                /* passthru */
            case DIGI_SERDES_PROV_STATE_ENET_LINE:
                is_enet_line = TRUE;
            case DIGI_SERDES_PROV_STATE_ENET_SYS: 
                if (TRUE == is_enet_line)
                {
                    enet_chnl_handle_ptr = &digi_handle->handle_pool.enet_line_chnl[0];                   
                    max_num_chnl = DIGI_ENET_LINE_CHNL_MAX;
                }
                else
                {
                    enet_chnl_handle_ptr = &digi_handle->handle_pool.enet_sys_chnl[0];                   
                    max_num_chnl = DIGI_ENET_SYS_CHNL_MAX;
                }

                for (jtr = 0; jtr < max_num_chnl; jtr += 1)
                {                    
                    if (UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG != enet_chnl_handle_ptr[jtr].switch_data.header.prov_state)
                    {                            
                        if ((DIGI_SERDES_DSI_LANES_MAX > enet_chnl_handle_ptr[jtr].dsi_base_lane &&
                             post_mld_dsi_ctxt_ptr[i].port_uid == post_mld_dsi_ctxt_ptr[enet_chnl_handle_ptr[jtr].dsi_base_lane].port_uid))
                        {
                            break;
                        }
                    }
                }
                if ( jtr >= max_num_chnl)
                {
                    if ((DIGI_MAPPER_ALLOC_UNUSED != post_mld_dsi_ctxt_ptr[i].map_type &&
                         LAST_DIGI_MAPPER_ALLOC != post_mld_dsi_ctxt_ptr[i].map_type))
                    {
                        /* if no MAPOTN channel is associated to post mld lane, cleanup num_odu0 and map_type in lane context */
                        if (FALSE ==  digi_resource_post_mld_lane_mapotn_chnl_status_get(digi_handle,
                                                                                         intf,
                                                                                         i))
                        {
                            post_mld_dsi_ctxt_ptr[i].num_odu0 = 0;
                            post_mld_dsi_ctxt_ptr[i].map_type = DIGI_MAPPER_ALLOC_UNUSED;
                        }
                    }
                    else
                    {
                        skip_cleanup = FALSE;
                    }
                }

                break;
                /* passthru */
                /* Check that post mld lane is connected to a CBRC channel */
            case DIGI_SERDES_PROV_STATE_CBRC:
                max_num_chnl = DIGI_CBRC_CHNL_MAX;
                cbrc_chnl_handle_ptr = &digi_handle->handle_pool.cbr_chnl[0];
                for (jtr = 0; jtr < max_num_chnl; jtr += 1)
                {                    
                    if (UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG != cbrc_chnl_handle_ptr[jtr].switch_data.header.prov_state)
                    {                            
                        if (post_mld_dsi_ctxt_ptr[i].port_uid == cbrc_chnl_handle_ptr[jtr].port_uid)
                        {
                            break;
                        }
                    }
                }
                if (jtr >= max_num_chnl)
                {
                    skip_cleanup = FALSE;
                }
                break;
            case DIGI_SERDES_PROV_STATE_NO_SERDES:
            case DIGI_SERDES_PROV_STATE_UNASSIGNED:
                /* Check that post mld lane is connected to a MAPOTN/ETRANS channel */
                skip_cleanup  = digi_resource_post_mld_lane_mapotn_chnl_status_get(digi_handle,
                                                                                   intf,
                                                                                   i);                
                break;
            default:
                skip_cleanup = FALSE;
                break;
            }

            /* re-init lane post-mld context when no channel is associated to it */
            if (FALSE == skip_cleanup)
            {
                if (DIGI_SERDES_PROV_STATE_NO_SERDES != post_mld_dsi_ctxt_ptr[i].prov_state)
                {
                    post_mld_dsi_ctxt_ptr[i].prov_state = DIGI_SERDES_PROV_STATE_UNASSIGNED;
                }
                post_mld_dsi_ctxt_ptr[i].num_odu0 = 0;
                post_mld_dsi_ctxt_ptr[i].map_type = DIGI_MAPPER_ALLOC_UNUSED;
            }
        }
    }



    PMC_RETURN();
}/*digi_resource_clean_post_mld_dsi_ctxt*/


/*******************************************************************************
* digi_resource_post_mld_lane_mapotn_chnl_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Returns TRUE when post mld is associated to at least on MAPOTN channel.
*
* INPUTS:
*   digi_handle  - Reference to the digi_handle
*   intf         - Post MLD context type: LINE or SYSTEM.
*   lane         - post mld lane.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   TRUE when post mld lane is associated to at least one MAPOTN channel
*    otherwise FALSE.
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 digi_resource_post_mld_lane_mapotn_chnl_status_get(digi_handle_t      *digi_handle,
                                                                 digi_serdes_intf_t  intf,
                                                                 UINT8               lane)
{  
    digi_post_mld_dsi_ctxt_t *post_mld_dsi_ctxt_ptr;
    BOOL8 skip_func = FALSE;
    BOOL8 mapotn_cfg = FALSE;    
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 jtr;

    PMC_ENTRY();

    switch (intf)
    {
    case DIGI_SERDES_LINE_INTF:
        post_mld_dsi_ctxt_ptr = &digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[0];
        break;
    case DIGI_SERDES_SYSTEM_INTF:
        post_mld_dsi_ctxt_ptr = &digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[0];
        break;
    default:
        skip_func = TRUE;
    }

    if (FALSE == skip_func)
    {
        if (DIGI_MAPPER_ALLOC_UNUSED != post_mld_dsi_ctxt_ptr[lane].map_type &&
            LAST_DIGI_MAPPER_ALLOC != post_mld_dsi_ctxt_ptr[lane].map_type)
        {
            for (jtr = 0; jtr < DIGI_MAPOTN_CHNL_MAX; jtr += 1)
            {
                mapper_chnl_handle_ptr = &digi_handle->handle_pool.mapper_chnl[jtr];
                                
                if ((UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG != mapper_chnl_handle_ptr->switch_data.header.prov_state))
                {                            
                    if (lane ==  mapper_chnl_handle_ptr->post_mld_index)
                    {
                        break;
                    }
                }
            }
            if ( jtr < DIGI_MAPOTN_CHNL_MAX)
            {
                mapotn_cfg= TRUE;
            }
        }
    }

    PMC_RETURN(mapotn_cfg);
}/*digi_resource_clean_post_mld_dsi_ctxt_num_odu0*/

#ifndef DOXYGEN_PUBLIC_ONLY

/*******************************************************************************
* digi_resource_clean_int_pmon
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Called within digi_ressource_clean. Cancel PMON/interrupts configuration.
*  
*  
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*
* OUTPUTS:
    None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_resource_clean_int_pmon(digi_handle_t *digi_handle)
{
    digi_pmon_work_order_msg_t pmon_order;

    PMC_ENTRY();
    /* clear interrupt configuration  */
    /* DISABLE IPI int */
    (void) digi_pmon_ipi_int_enable(digi_handle,
                                    DIGI_PMON_IPI_TYPE_DDEG_ENH,
                                    FALSE);        
    (void) digi_pmon_ipi_int_enable(digi_handle,
                                    DIGI_PMON_IPI_TYPE_DDEG_BURSTY,
                                    FALSE);
    
    /* clear pmon collection */
    /* We have to review if it is required to call this function */
    (void) digi_pmon_collection_mode_cfg(digi_handle,
                                         FALSE, /* trig_mode */
                                         FALSE,
                                         250,
                                         0,
                                         NULL,
                                         0,
                                         1);
    /* disable FW PMON collection */
    pmon_order.enable_hw_block_msk  = 0;
    pmon_order.disable_hw_block_msk = 0x7F07;
    (void) digi_pmon_work_order_cfg(digi_handle, &pmon_order);
 
    /* disable all interrupts */
    /* digi_int_disable_all(digi_handle);  */

    /* clear IPI bits */
    (void) digi_pmon_ipi_bit_set(digi_handle,DIGI_PMON_IPI_TYPE_PMON_OTN, FALSE);
    (void) digi_pmon_ipi_bit_set(digi_handle,DIGI_PMON_IPI_TYPE_PMON_MISC, FALSE);
    (void) digi_pmon_ipi_bit_set(digi_handle,DIGI_PMON_IPI_TYPE_DDEG_ENH, FALSE);
    (void) digi_pmon_ipi_bit_set(digi_handle,DIGI_PMON_IPI_TYPE_DDEG_BURSTY, FALSE);

    PMC_RETURN();
    
} /* digi_resource_clean_int_pmon */

#endif

/*******************************************************************************
* digi_resource_convert_resource_handle
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Converts the resource handle enumerands into there equivalent string
*
* INPUTS:
*   resource_id   - Enumerand
*
* OUTPUTS:
*   resource string - Value converted to a string for printing
*                           port.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE const char * digi_resource_convert_resource_handle(digi_resource_next_handle_t resource_id)
{
    switch(resource_id)
    {
      case DIGI_LINE_SERDES_CFG:
        global_string = "LINE SERDES";
        break;
      case DIGI_SYS_SERDES_CFG:
        global_string = "SYS SERDES";
        break;

      case DIGI_SFI51_SERDES_CFG: 
        global_string = "SFI 51 SERDES";
        break;

      case DIGI_LINE_OTN_HO_CONTAINER:
        global_string = "LINE OTN HO";
        break;  

      case DIGI_LINE_OTN_MO_CONTAINER:
        global_string = "LINE OTN MO";
        break; 

      case DIGI_LINE_OTN_LO_CONTAINER:
        global_string = "LINE OTN LO";
        break; 

      case DIGI_SYS_OTN_HO_CONTAINER:
        global_string = "SYS OTN HO";
        break;  

      case DIGI_SYS_OTN_MO_CONTAINER:
        global_string = "SYS OTN MO";
        break; 

      case DIGI_ILKN_INTF_CONTEXT:
        global_string = "ILKN INTF";
        break; 

      case DIGI_ILKN_CHNL_CONTEXT:
        global_string = "ILKN CHNL";
        break;

      case DIGI_OTN_MAPPER:
        global_string = "OTN MAPPER";
        break;

      case DIGI_ODUK_SWITCH:
        global_string = "ODUK SWITCH";
        break;

      case DIGI_CPB_SWITCH:
        global_string = "CPB SWITCH";
        break;

      case DIGI_ILKN_1:
        global_string = "ILKN 1";
        break;    

      case DIGI_ILKN_2:
        global_string = "ILKN 2";
        break;         

      default:
        global_string = "NO RESOURCE";
        break;
    }

    return(global_string);
}

/* LCOV_EXCL_START */

/*******************************************************************************
* digi_convert_handle_id
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Converts the enumerand into a string for printing purposes
*
* INPUTS:
*  handle_id  - Handle to be converted
* 
* OUTPUTS:
*   handle_id - Enumerand as a string
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/

PRIVATE const char * digi_convert_handle_id(digi_handle_id_t handle_id){
    switch(handle_id)
    {
      case DIGI_CHANNEL_HANDLE_ID_LINEOTN_T:
        global_string = "LINEOTN HANDLE";
        break;

      case DIGI_CHANNEL_HANDLE_ID_SYSOTN_T:
        global_string = "SYSOTN HANDLE";
        break;

      case DIGI_CHANNEL_HANDLE_ID_HO_ODU_T:
        global_string = "HO ODU HANDLE";
        break;

      case DIGI_CHANNEL_HANDLE_ID_MO_ODU_T:
        global_string = "MO ODU HANDLE";
        break;

      case DIGI_CHANNEL_HANDLE_ID_LO_ODU_T:
        global_string = "LO ODU HANDLE";
        break;

      case DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T:
        global_string = "CBRC HANDLE";
        break;
      case DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T:
        global_string = "ENET LINE HANDLE";
        break;

      case DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T:
        global_string = "ENET SYS HANDLE";
        break;

      case DIGI_CHANNEL_HANDLE_ID_MAPPER_T:
        global_string = "MAPOTN HANDLE";
        break;

      case DIGI_CHANNEL_HANDLE_ID_ILKN_1_T:
        global_string = "ILKN 1 HANDLE";
        break;

      case DIGI_CHANNEL_HANDLE_ID_ILKN_2_T:
        global_string = "ILKN 2 HANDLE";
        break;

      case DIGI_CHANNEL_HANDLE_ID_ILKN_CHNL_T:
        global_string = "ILKN CHNL HANDLE";
        break;

      case DIGI_CHANNEL_HANDLE_ID_ILKN_INTF_T:
        global_string = "ILKN INTF HANDLE";
        break;

      default:
        global_string = "NO HANDLE";
        break;
    }

    return(global_string);
}

/*******************************************************************************
* digi_convert_payload
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Converts the digi_odu_line_payload_t enumerands into strings for printing
*  in a table
*
* INPUTS:
*  payload - Specific payload mode value.
*
* OUTPUTS:
*  None
*
* RETURNS:
*  global_string - Converted enumerand into a string.
*
* NOTES:
*
*******************************************************************************/

PRIVATE const char * digi_convert_payload(digi_odu_line_payload_t payload)
{
    switch(payload)
    { 
      case DIGI_ODU_UNCHANNELIZED:
        global_string = "UNCHANNELIZED";
        break;

      case DIGI_ODU_TS_1G25:
        global_string = "1G25";
        break;

      case DIGI_ODU_TS_2G5:
        global_string = "2G5";
        break;

    case DIGI_ODU_NULL:
        global_string = "DIGI_ODU_NULL";
        break;

      default:
        global_string = "UNSET";
        break;
    }

    return(global_string);
}

/*******************************************************************************
* digi_print_next_resource
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Prints the table
*
* INPUTS:
*   digi_handle            - Reference to the digi_handle context
*   next_resource          - Resource handle that is to be printed. In this
*                            case used for different OTN related containers
*                            in digi_resource_handle_summary_t
*   next_index             - Next handle index from the perspective of the
*                            serdes structures. Used for the Line or System
*                            side HO container.
*   parent_index           - Index of the structure that is connected to the
*                            OTN subsystem being printed. For HO this is the
*                            serdes (line/sfi51/sys) structure. For MO it is
*                            the HO container and for LO it is the MO 
*                            container.
*   *digi_resource_handle  - Table holding the various container structures 
*                            to be printed.
* 
* OUTPUTS:
*   None
*
* RETURNS:
*   TRUE  - Printed out another resource.
*   FALSE - Nothing was printed. Used by the serdes line print to know 
*           whether to print the header/footer again.
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 digi_print_next_resource(digi_handle_t *digi_handle,
                                       digi_resource_next_handle_t next_resource,
                                       UINT16 next_index,
                                       UINT16 parent_index,
                                       digi_resource_handle_summary_t *digi_resource_handle)
{
    if (DIGI_LINE_OTN_HO_CONTAINER == next_resource)
    {
        UINT16 i = next_index;
        digi_print_header_footer("  HIGH ORDER CHANNELS ", TRUE);

        PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                "\nIndex\t    HandleType\t    HandlePointer   PayloadMode  LO Channels\tAssociateHandle AssociateIndex     NextHandle\t  NextIndex\n");
        PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                "%3d\t %12s  %8p\t %11s\t %4d\t %20s %11d %20s %15d\n", i, digi_convert_handle_id(digi_resource_handle->line_otn_ho_container_cfg[i].handle_type), digi_resource_handle->line_otn_ho_container_cfg[i].chnl_handle_ptr, digi_convert_payload(digi_resource_handle->line_otn_ho_container_cfg[i].payload_mode), digi_resource_handle->line_otn_ho_container_cfg[i].lo_chnl_entries, digi_resource_convert_resource_handle(digi_resource_handle->line_otn_ho_container_cfg[i].next_associate_handle), digi_resource_handle->line_otn_ho_container_cfg[i].next_associate_index, digi_resource_convert_resource_handle(digi_resource_handle->line_otn_ho_container_cfg[i].next_resource_handle), digi_resource_handle->line_otn_ho_container_cfg[i].next_handle_index);
        PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                "-------|-----------------|-----------------|------------|-------------|--------------|---------------------------|-------------|\n");
        (void)digi_print_next_resource(digi_handle, digi_resource_handle->line_otn_ho_container_cfg[i].next_resource_handle, digi_resource_handle->line_otn_ho_container_cfg[i].next_handle_index, i, digi_resource_handle);
        digi_print_header_footer("", FALSE); 
    }
    else if (DIGI_LINE_OTN_MO_CONTAINER == next_resource)
    {
        UINT8 j = next_index;
        digi_otn_container_cfg_t *otn_container_ptr = NULL;

        /*for (j = 0; j < DIGI_OTN_SERVER_MO_CHNL_MAX; j++)*/
        while (j < DIGI_OTN_SERVER_MO_CHNL_MAX)
        {
            otn_container_ptr = &digi_resource_handle->line_otn_mo_container_cfg[j];
            
            if (DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == digi_handle->var.prod_app)
            {
                otn_container_ptr = &digi_resource_handle->sys_otn_mo_container_cfg[j];
            }
            if (NULL != otn_container_ptr->chnl_handle_ptr && parent_index == otn_container_ptr->parent_handle_index)
            {
                digi_print_header_footer("  MID ORDER CHANNELS ", TRUE);
                digi_resource_print_coreotn_channels("HO Index  MO Index",  parent_index,
                                                     j, digi_convert_handle_id(otn_container_ptr->handle_type), 
                                                     otn_container_ptr->chnl_handle_ptr, 
                                                     otn_container_ptr->payload_mode,
                                                     otn_container_ptr->lo_chnl_entries,
                                                     digi_resource_convert_resource_handle(otn_container_ptr->next_associate_handle),
                                                     otn_container_ptr->next_associate_index,
                                                     digi_resource_convert_resource_handle(otn_container_ptr->next_resource_handle), 
                                                     otn_container_ptr->next_handle_index,
                                                     otn_container_ptr->tx_trib_slot_mask,
                                                     otn_container_ptr->rx_trib_slot_mask);

                (void)digi_print_next_resource(digi_handle, 
                                               otn_container_ptr->next_resource_handle, 
                                               otn_container_ptr->next_handle_index, j, digi_resource_handle);
            }
            if (LAST_DIGI_RESOURCE_HANDLE == otn_container_ptr->next_associate_handle)
            {
                break;
            }
            j = otn_container_ptr->next_associate_index;
        }
        digi_print_header_footer("", FALSE);
    }
    else if (DIGI_LINE_OTN_LO_CONTAINER == next_resource)
    {
        UINT8 j = next_index;

        /*for (j = 0; j < DIGI_OTN_SERVER_LO_CHNL_MAX; j++)*/
        while (j < DIGI_OTN_SERVER_LO_CHNL_MAX)
        {
            if (NULL != digi_resource_handle->line_otn_lo_container_cfg[j].chnl_handle_ptr && parent_index == digi_resource_handle->line_otn_lo_container_cfg[j].parent_handle_index)
            {
                digi_print_header_footer("  LO ORDER CHANNELS ", TRUE);
                digi_resource_print_coreotn_channels("MO Index  LO Index",  parent_index,
                                       j, digi_convert_handle_id(digi_resource_handle->line_otn_lo_container_cfg[j].handle_type), 
                                       digi_resource_handle->line_otn_lo_container_cfg[j].chnl_handle_ptr, 
                                       digi_resource_handle->line_otn_lo_container_cfg[j].payload_mode,
                                       digi_resource_handle->line_otn_lo_container_cfg[j].lo_chnl_entries,
                                       digi_resource_convert_resource_handle(digi_resource_handle->line_otn_lo_container_cfg[j].next_associate_handle),
                                       digi_resource_handle->line_otn_lo_container_cfg[j].next_associate_index,
                                       digi_resource_convert_resource_handle(digi_resource_handle->line_otn_lo_container_cfg[j].next_resource_handle), 
                                       digi_resource_handle->line_otn_lo_container_cfg[j].next_handle_index,
                                       digi_resource_handle->line_otn_lo_container_cfg[j].tx_trib_slot_mask,
                                       digi_resource_handle->line_otn_lo_container_cfg[j].rx_trib_slot_mask);
            }
            if (LAST_DIGI_RESOURCE_HANDLE == digi_resource_handle->line_otn_lo_container_cfg[j].next_associate_handle)
            {
                break;
            }
            j = digi_resource_handle->line_otn_lo_container_cfg[j].next_associate_index;
        }
        digi_print_header_footer("", FALSE);
    }
    else if (DIGI_SYS_OTN_HO_CONTAINER == next_resource)
    {
        digi_print_header_footer(" SYS HIGH ORDER CHANNELS ", TRUE);

        PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                "Index\t    HandleType\t    HandlePointer   PayloadMode  LO Channels\tAssociateHandle AssociateIndex     NextHandle\t  NextIndex\n");

        if (TRUE == DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle))
        {
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                           "%3d\t %12s  %4p %11s\t %4d\t %20s %11d %20s %15d\n", next_index, digi_convert_handle_id(digi_resource_handle->sys_otn_ho_container_cfg[0].handle_type), digi_resource_handle->sys_otn_ho_container_cfg[0].chnl_handle_ptr, digi_convert_payload(digi_resource_handle->sys_otn_ho_container_cfg[0].payload_mode), digi_resource_handle->sys_otn_ho_container_cfg[0].lo_chnl_entries, digi_resource_convert_resource_handle(digi_resource_handle->sys_otn_ho_container_cfg[0].next_associate_handle), digi_resource_handle->sys_otn_ho_container_cfg[0].next_associate_index, digi_resource_convert_resource_handle(digi_resource_handle->sys_otn_ho_container_cfg[0].next_resource_handle), digi_resource_handle->sys_otn_ho_container_cfg[0].next_handle_index);
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                           "-------|-----------------|-----------------|------------|-------------|--------------|---------------------------|-------------|\n");
            (void)digi_print_next_resource(digi_handle, digi_resource_handle->sys_otn_ho_container_cfg[0].next_resource_handle, digi_resource_handle->sys_otn_ho_container_cfg[0].next_handle_index, next_index, digi_resource_handle);
        }
        else
        {
            UINT8 j = next_index;

            while (j < DIGI_OTN_SERVER_HO_CHNL_MAX)
            {
                if (NULL != digi_resource_handle->sys_otn_ho_container_cfg[j].chnl_handle_ptr && parent_index == digi_resource_handle->sys_otn_ho_container_cfg[j].parent_handle_index)
                {
                    PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                                   "%3d\t %12s  %12p %11s\t %4d\t %20s %11d %20s %15d\n", next_index, digi_convert_handle_id(digi_resource_handle->sys_otn_ho_container_cfg[j].handle_type), digi_resource_handle->sys_otn_ho_container_cfg[j].chnl_handle_ptr, digi_convert_payload(digi_resource_handle->sys_otn_ho_container_cfg[j].payload_mode), digi_resource_handle->sys_otn_ho_container_cfg[j].lo_chnl_entries, digi_resource_convert_resource_handle(digi_resource_handle->sys_otn_ho_container_cfg[j].next_associate_handle), digi_resource_handle->sys_otn_ho_container_cfg[j].next_associate_index, digi_resource_convert_resource_handle(digi_resource_handle->sys_otn_ho_container_cfg[j].next_resource_handle), digi_resource_handle->sys_otn_ho_container_cfg[j].next_handle_index);

                    PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                                   "-------|-----------------|-----------------|------------|-------------|--------------|---------------------------|-------------|\n");
                    (void)digi_print_next_resource(digi_handle, digi_resource_handle->sys_otn_ho_container_cfg[j].next_resource_handle, digi_resource_handle->sys_otn_ho_container_cfg[j].next_handle_index, next_index, digi_resource_handle);
                }
                j++;
            }
        }
        digi_print_header_footer("", FALSE); 
    }
    else if (DIGI_SYS_OTN_MO_CONTAINER == next_resource)
    {
        UINT8 j = next_index;

        while (j < DIGI_OTN_SERVER_LO_CHNL_MAX)
        {
            if (NULL != digi_resource_handle->sys_otn_mo_container_cfg[j].chnl_handle_ptr && parent_index == digi_resource_handle->sys_otn_mo_container_cfg[j].parent_handle_index)
            {
                digi_print_header_footer("  SYS MO ORDER CHANNELS ", TRUE);
                digi_resource_print_coreotn_channels("HO Index  MO Index",  parent_index,
                                       j, digi_convert_handle_id(digi_resource_handle->sys_otn_mo_container_cfg[j].handle_type), 
                                       digi_resource_handle->sys_otn_mo_container_cfg[j].chnl_handle_ptr, 
                                       digi_resource_handle->sys_otn_mo_container_cfg[j].payload_mode,
                                       digi_resource_handle->sys_otn_mo_container_cfg[j].lo_chnl_entries,
                                       digi_resource_convert_resource_handle(digi_resource_handle->sys_otn_mo_container_cfg[j].next_associate_handle),
                                       digi_resource_handle->sys_otn_mo_container_cfg[j].next_associate_index,
                                       digi_resource_convert_resource_handle(digi_resource_handle->sys_otn_mo_container_cfg[j].next_resource_handle), 
                                       digi_resource_handle->sys_otn_mo_container_cfg[j].next_handle_index,
                                       digi_resource_handle->sys_otn_mo_container_cfg[j].tx_trib_slot_mask,
                                       digi_resource_handle->sys_otn_mo_container_cfg[j].rx_trib_slot_mask);

                if (DIGI_LINE_OTN_LO_CONTAINER == digi_resource_handle->sys_otn_mo_container_cfg[j].next_resource_handle)
                {
                    (void)digi_print_next_resource(digi_handle, digi_resource_handle->sys_otn_mo_container_cfg[j].next_resource_handle, digi_resource_handle->sys_otn_mo_container_cfg[j].next_handle_index, j, digi_resource_handle);
                }
            }
            if (LAST_DIGI_RESOURCE_HANDLE == digi_resource_handle->sys_otn_mo_container_cfg[j].next_associate_handle)
            {
                break;
            }
            j = digi_resource_handle->sys_otn_mo_container_cfg[j].next_associate_index;
        }
        digi_print_header_footer("", FALSE);
    }
    else
    {
        return(FALSE);
    }

    return(TRUE);
}  /* digi_print_next_resource */

/*******************************************************************************
* digi_print_switch
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Prints the table
*
* INPUTS:
*   *digi_resource_handle  - Table holding the various container structures 
*                            to be printed.
*
* OUTPUTS:
    None
*
* RETURNS:
*   PMC_SUCCESS            - Return value included for completeness.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_print_switch(digi_resource_handle_summary_t *digi_resource_handle)
{
    UINT32 i;
    BOOL8 first = TRUE;

    for (i=0; i< DIGI_ODUK_SWITCH_TOTAL_CHANNELS; i++)
    {
        if (LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->oduksw_cfg[i].provisioned_handle_type)
        {
            if (TRUE == first)
            {
                digi_print_header_footer("  ODUK SWITCH  ", TRUE);
                first = FALSE;
            }
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                    "ODUK Index\tProvisionType\tProvisionIndex SourceHandleType  SourceIndex  SourceActivated  PrimaryDestHandle  PrimaryHandleIndex  DestActivated  NumDestinations SecondaryHandle SecondaryIndex\n");
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                    "%6d\t %16s\t %6d %20s %10d %13d %23s %12d %15d %16d %23s %10d\n", i,  digi_resource_convert_resource_handle(digi_resource_handle->oduksw_cfg[i].provisioned_handle_type), digi_resource_handle->oduksw_cfg[i].provisioned_handle_index, digi_resource_convert_resource_handle(digi_resource_handle->oduksw_cfg[i].source_handle_type), digi_resource_handle->oduksw_cfg[i].source_handle_index, digi_resource_handle->oduksw_cfg[i].source_activated, digi_resource_convert_resource_handle(digi_resource_handle->oduksw_cfg[i].primary_dest_handle_type), digi_resource_handle->oduksw_cfg[i].primary_dest_index, digi_resource_handle->oduksw_cfg[i].dest_activated,  digi_resource_handle->oduksw_cfg[i].num_dest, digi_resource_convert_resource_handle(digi_resource_handle->oduksw_cfg[i].associate_dest_handle_type), digi_resource_handle->oduksw_cfg[i].associate_dest_index);
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,"----------|--------------------|---------------|--------------|---------------|---------------------|---------------------|--------------------|------------------|\n");
        }
    }
    digi_print_header_footer("", FALSE);
    first = TRUE;

    for (i=0; i< DIGI_CPB_SWITCH_TOTAL_CHANNELS; i++)
    {
        if (LAST_DIGI_RESOURCE_HANDLE != digi_resource_handle->cpbsw_cfg[i].provisioned_handle_type)
        {
            if (TRUE == first)
            {
                digi_print_header_footer("  CPB SWITCH  ", TRUE);
                first = FALSE;
            }
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                    "CPB Index\tProvisionType\tProvisionIndex SourceHandleType  SourceIndex  SourceActivated  PrimaryDestHandle  PrimaryHandleIndex  DestActivated  NumDestinations SecondaryHandle SecondaryIndex\n");
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                    "%6d\t %16s\t %6d %20s %10d %13d %23s %12d %15d %16d %23s %10d\n", i,  digi_resource_convert_resource_handle(digi_resource_handle->cpbsw_cfg[i].provisioned_handle_type), digi_resource_handle->cpbsw_cfg[i].provisioned_handle_index, digi_resource_convert_resource_handle(digi_resource_handle->cpbsw_cfg[i].source_handle_type), digi_resource_handle->cpbsw_cfg[i].source_handle_index, digi_resource_handle->cpbsw_cfg[i].source_activated, digi_resource_convert_resource_handle(digi_resource_handle->cpbsw_cfg[i].primary_dest_handle_type), digi_resource_handle->cpbsw_cfg[i].primary_dest_index, digi_resource_handle->cpbsw_cfg[i].dest_activated,  digi_resource_handle->cpbsw_cfg[i].num_dest, digi_resource_convert_resource_handle(digi_resource_handle->cpbsw_cfg[i].associate_dest_handle_type), digi_resource_handle->cpbsw_cfg[i].associate_dest_index);

            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                    "----------|--------------------|---------------|--------------|---------------|---------------------|---------------------|--------------------|------------------|\n");
        }
    }
    digi_print_header_footer("", FALSE);

} /* digi_print_switch */


/*******************************************************************************
* digi_print_ilkn_channels
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Prints the table
*
* INPUTS:
*   *digi_resource_handle  - Table holding the various container structures 
*                            to be printed.
*
* OUTPUTS:
    None
*
* RETURNS:
*   PMC_SUCCESS            - Return value included for completeness.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_print_ilkn_channels(digi_resource_handle_summary_t *digi_resource_handle)
{
    UINT32 i;

    digi_print_header_footer("ILKN CHANNELS", TRUE);

    for (i=0; i< DIGI_ILKN_CHNL_NUM; i++)
    {
        if (NULL != digi_resource_handle->ilkn_chnl_cfg[0][i].ilkn_chnl_handle_ptr &&
           TRUE == util_global_rx_or_tx_chnl_prov_status_get(((digi_ilkn_chnl_def_t *)digi_resource_handle->ilkn_chnl_cfg[0][i].ilkn_chnl_handle_ptr)->switch_data.header.prov_state))
        {
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                    "ILKN #\tILKNChannel\tILKNHandlePtr\tNextResource NextIndex\t Target\t Tx_BCW\t Tx_2b_Header  Tx_4b_Header  Rx_BCW  Rx_2b_Header  Rx_4b_Header\n");
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                           "  1\t %4d\t %20p\t %6s %7d\t%4d %8d %8d\t %12d %10d %12d %12d\n", i, digi_resource_handle->ilkn_chnl_cfg[0][i].ilkn_chnl_handle_ptr, digi_resource_convert_resource_handle(digi_resource_handle->ilkn_chnl_cfg[0][i].next_resource_handle), digi_resource_handle->ilkn_chnl_cfg[0][i].next_handle_index, (UINT8)digi_resource_handle->ilkn_chnl_cfg[0][i].ilkn_chnl_desc.target, digi_resource_handle->ilkn_chnl_cfg[0][i].ilkn_chnl_desc.tx_bcw_chnl, digi_resource_handle->ilkn_chnl_cfg[0][i].ilkn_chnl_desc.tx_2b_header, digi_resource_handle->ilkn_chnl_cfg[0][i].ilkn_chnl_desc.tx_4b_header, digi_resource_handle->ilkn_chnl_cfg[0][i].ilkn_chnl_desc.rx_bcw_chnl, digi_resource_handle->ilkn_chnl_cfg[0][i].ilkn_chnl_desc.rx_2b_header, digi_resource_handle->ilkn_chnl_cfg[0][i].ilkn_chnl_desc.rx_4b_header);
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,"-----|--------------|-----------------|---------------|--------|-------|---------|----------|------------|---------|--------------|------------|\n");
        }
    }


    for (i=0; i< DIGI_ILKN_CHNL_NUM; i++)
    {
        if (NULL != digi_resource_handle->ilkn_chnl_cfg[1][i].ilkn_chnl_handle_ptr &&
            TRUE == util_global_rx_or_tx_chnl_prov_status_get(((digi_ilkn_chnl_def_t *)digi_resource_handle->ilkn_chnl_cfg[1][i].ilkn_chnl_handle_ptr)->switch_data.header.prov_state))
        {
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                    "ILKN #\tILKNChannel\t ILKNHandlePtr\tNextResource NextIndex\n");
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                    "  2\t %12d\t %8p\t %5s %7d\n", i,  digi_resource_handle->ilkn_chnl_cfg[1][i].ilkn_chnl_handle_ptr, digi_resource_convert_resource_handle(digi_resource_handle->ilkn_chnl_cfg[1][i].next_resource_handle), digi_resource_handle->ilkn_chnl_cfg[1][i].next_handle_index);
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                    "----------|--------------------|---------------|--------------|\n");
        }
    }

    digi_print_header_footer("", FALSE);
}


/*******************************************************************************
* digi_print_mapotn_channels
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Prints the table
*
* INPUTS:
*   digi_handle            - Reference to the digi_handle.
*   digi_resource_handle   - Table holding the various container structures 
*                            to be printed.
*
* OUTPUTS:
    None
*
* RETURNS:
*   PMC_SUCCESS            - Return value included for completeness.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_print_mapotn_channels(digi_handle_t *digi_handle,
                                        digi_resource_handle_summary_t *digi_resource_handle)
{
    UINT32 i;

    digi_print_header_footer("MAPOTN CHANNELS", TRUE);

    for (i=0; i< DIGI_MAPOTN_CHNL_MAX; i++)
    {
        if (NULL != digi_resource_handle->mapper_cfg[i].mapper_handle_ptr &&
            (TRUE == util_global_rx_or_tx_chnl_prov_status_get(((digi_ilkn_chnl_def_t *)digi_resource_handle->mapper_cfg[i].mapper_handle_ptr)->switch_data.header.prov_state)))
        {
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                    "MAPOTN Channel\tMAPOTNHandlePtr\tODUK Index\tCPB Index\tMapping Mode\n");
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                           " %6d\t %20p\t %4d\t %10d\t%14s\n", i, digi_resource_handle->mapper_cfg[i].mapper_handle_ptr, digi_resource_handle->mapper_cfg[i].oduk_switch_index, digi_resource_handle->mapper_cfg[i].cpb_switch_index, digi_resource_convert_mapper_mode(digi_handle, digi_resource_handle->mapper_cfg[i].mapper_handle_ptr));
            PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,"----------|--------------------|---------------|--------------|\n");
        }
    }

    digi_print_header_footer("", FALSE);
}  /* digi_print_mapotn_channels */


/*******************************************************************************
*  digi_resource_print_coreotn_channels
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Localize the printing of the digi_otn_container_cfg_t so that spacing
*  adjustments are reflected across all ODU levels.
*
* INPUTS:
*  first_string          - First string to be printed
*  parent_index          - Index of the parent
*  current_index         - Index of the current resource
*  handle_type           - String representing the resource type.
*  chnl_handle_ptr       - Reference to the current channel handle.
*  payload_mode          - Payload value for the current resource
*  lo_chnl_entries       - Number of lower order channels under the current
*                          one
*  next_associate_handle - Handle of the next associated resource
*  next_associate_index  - Index of the next associated resource.
*  next_resource_handle  - Next resource from the current one.
*  next_handle_index     - Index of the next resource
*  tx_trib_slot_mask     - TX Trib slot mask used to provision the current 
*                          resource.
*  rx_trib_slot_mask     - RX Trib slot mask used to provision the current 
*                          resource.
*
* OUTPUTS:
    None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_print_coreotn_channels(const char *first_string,
                                                  UINT16 parent_index,
                                                  UINT16 current_index,
                                                  const char *handle_type,
                                                  void *chnl_handle_ptr,
                                                  digi_odu_line_payload_t payload_mode,
                                                  UINT16 lo_chnl_entries,
                                                  const char *next_associate_handle,
                                                  UINT16 next_associate_index,
                                                  const char *next_resource_handle,
                                                  UINT16 next_handle_index,
                                                  UINT32 tx_trib_slot_mask[3],
                                                  UINT32 rx_trib_slot_mask[3])
{
    PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
            "%s   HandleType\t        HandlePtr\t  PayloadMode\tLO Channels\tAssociateHandle  AssociateIndex\tNextHandle\tNextIndex   TributaryMask\n", first_string);

    if (DIGI_ODU_UNCHANNELIZED == payload_mode)
    {
        PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                "%4d\t %4d\t %16s\t %p\t %6s\t %6d\t %19s\t %6d\t %18s\t %4d\t      [%x][%x][%x]   [%x][%x][%x]\n", parent_index, current_index, handle_type, 
                        chnl_handle_ptr, digi_convert_payload(payload_mode),
                        lo_chnl_entries, next_associate_handle, 
                        next_associate_index, next_resource_handle, 
                        next_handle_index, tx_trib_slot_mask[0], 
                        tx_trib_slot_mask[1], tx_trib_slot_mask[2],
                        rx_trib_slot_mask[0], 
                        rx_trib_slot_mask[1], rx_trib_slot_mask[2]);
    }
    else
    {
        PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,
                "%4d\t %4d\t %16s\t %p\t %6s\t %12d\t %18s\t %6d\t %18s\t %4d\t      [%x][%x][%x]  [%x][%x][%x]\n", parent_index, current_index, handle_type, 
                        chnl_handle_ptr, digi_convert_payload(payload_mode),
                        lo_chnl_entries, next_associate_handle, 
                        next_associate_index, next_resource_handle, 
                        next_handle_index, tx_trib_slot_mask[0], 
                        tx_trib_slot_mask[1], tx_trib_slot_mask[2],
                        rx_trib_slot_mask[0], 
                        rx_trib_slot_mask[1], rx_trib_slot_mask[2]);


    }
    PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,"----------|--------------------|---------------|--------------|---------------|-----------------------|------------------|--------------------|\n");
}

/*******************************************************************************
* digi_print_header_footer
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Prints the header or footer depending on the flags
*
* INPUTS:
*   *header_string   - String to be added to header. Ignored for footer
*   header           - True prints header, false prints footer
*
* OUTPUTS:
    None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_print_header_footer(const char *header_string, 
                                      BOOL8 header)
{
    if (TRUE == header)
    {
        PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++%s++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n", header_string);
    }
    else
    {
        PMC_LOG_NOINFO(PMC_LOG_SEV_HIGHEST, PMC_SHARED_LOG_TRACE, 0, 0,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    }

}

/*******************************************************************************
* digi_resource_convert_mapper_mode
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Converts the mapping mode set when the MAPOTN channel handle was 
*  provisioned into a string so that it can be logged.
*
* INPUTS:
*  digi_handle         - Reference to the digi_handle
*  mapper_ptr          - Reference to the MAPOTN channel handle
*
* OUTPUTS:
    None
*
* RETURNS:
*   global_string      - MAPOTN mapping mode converted to a string.
*
* NOTES:
*
*******************************************************************************/
PRIVATE const char *digi_resource_convert_mapper_mode(digi_handle_t *digi_handle,
                                                     digi_mapper_chnl_t *mapper_ptr)
{
    digi_mapping_mode_t mode = LAST_DIGI_MAP;
    BOOL8 enet_pmon;
    digi_mapper_primary_group_t primary_group;
    digi_mapper_secondary_group_t secondary_group;
    digi_oduk_t odu_data;
    UINT32 odu_flex_data;

    PMC_ENTRY();

    digi_otn_mapper_prov_get(digi_handle, mapper_ptr, &primary_group, 
                              &secondary_group, &mode, &enet_pmon,
                              &odu_data, &odu_flex_data);

    global_string = "UNDEFINED";

    switch (mode)
    {
      case DIGI_MAP_ODUKP_PACKET_GFP:
        global_string = "ODUKP_PACK_GFP";
        break;

      case DIGI_MAP_ODU4P_100_GE_GFP:
        global_string = "ODU4P_100_GE_GFP";
        break;

      case DIGI_MAP_ODU4P_100_GE_GFP_ENET:
        global_string = "ODU4P_100_GE_GFP_ENET";
        break;      

      case DIGI_MAP_ODU3P_40_GE_GFP:
        global_string = "ODU3P_40_GE_GFP";
        break; 

      case DIGI_MAP_ODU3P_40_GE_GFP_ENET:
        global_string = "ODU3P_40_GE_GFP_ENET";
        break;

      case DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP:
        global_string = "ODU2P_CBR_10_GE_7_3_GFP";
        break; 

      case DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP_ENET:
        global_string = "ODU2P_CBR_10_GE_7_3_GFP_ENET";
        break; 

      case DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP:
        global_string = "ODU2P_PKT_10_GE_7_3_GFP";
        break;

      case DIGI_MAP_ODU2P_10_GE_6_2_GFP:
        global_string = "ODU2P_10_GE_6_2_GFP";
        break;

      case DIGI_MAP_ODU2P_10_GE_6_2_GFP_ENET:
        global_string = "ODU2P_10_GE_6_2_GFP_ENET";
        break;

      case DIGI_MAP_ODU2EP_FC_1200_GFP:
        global_string = "ODU2EP_FC_1200_GFP";
        break;

      case DIGI_MAP_ODU2P_FC_800_GMP:
        global_string = "ODU2P_FC_800_GMP";
        break;
 
      case DIGI_MAP_ODU2EP_10G_GDPS_GMP:
        global_string = "ODU2EP_10G_GDPS_GMP";
        break;

      case DIGI_MAP_ODU2P_5G_GDPS_GMP:
        global_string = "ODU2P_5G_GDPS_GMP";
        break;

      case DIGI_MAP_ODU2P_CPRI_5_GMP:
        global_string = "ODU2P_CPRI_5_GMP";
        break;

      case DIGI_MAP_ODU2P_CPRI_10_GMP:
        global_string = "ODU2P_CPRI_10_GMP";
        break;

      case DIGI_MAP_ODU2P_CPRI_6_GMP:
        global_string = "ODU2P_CPRI_6_GMP";
        break;

      case DIGI_MAP_ODU3P_CBR_40_GE_GMP:
        global_string = "ODU3P_CBR_40_GE_GMP";
        break;    

      case DIGI_MAP_ODU3P_PKT_40_GE_GMP:
        global_string = "ODU3P_PKT_40_GE_GMP";
        break; 

      case DIGI_MAP_ODU4P_CBR_100_GE_GMP:
        global_string = "ODU4P_CBR_100_GE_GMP";
        break;
 
      case DIGI_MAP_ODU4P_PKT_100_GE_GMP:
        global_string = "ODU4P_PKT_100_GE_GMP";
        break; 

      case DIGI_MAP_ODUFLEX_5G_GDPS_BMP:
        global_string = "ODUFLEX_5G_GDPS_BMP";
        break; 

      case DIGI_MAP_ODUFLEX_10G_GDPS_BMP:
        global_string = "ODUFLEX_10G_GDPS_BMP";
        break;
  
      case DIGI_MAP_ODUFLEX_CPRI_5_BMP:
        global_string = "ODUFLEX_CPRI_5_BMP";
        break;

      case DIGI_MAP_ODUFLEX_CPRI_10_BMP:
        global_string = "ODUFLEX_CPRI_10_BMP";
        break;

      case DIGI_MAP_ODUFLEX_CPRI_6_BMP:
        global_string = "ODUFLEX_CPRI_6_BMP";
        break;

      case DIGI_MAP_ODUFLEX_FC_800_BMP:
        global_string = "ODUFLEX_FC_800_BMP";
        break;

      case DIGI_MAP_ODU2FP_FC_1200_BMP:
        global_string = "ODUFLEX_FC_1200_BMP";
        break;

      case DIGI_MAP_ODU1FP_FC_1200_BMP:
        global_string = "ODU1FP_FC_1200_BMP";
        break;

      case DIGI_MAP_ODU1EP_CBR_10G3_7_2_BMP:
        global_string = "ODU1EP_CBR_10G3_7_2_BMP";
        break;

      case DIGI_MAP_ODU2EP_CBR_10G3_7_1_BMP:
        global_string = "ODU2EP_CBR_10G3_7_1_BMP";
        break;
        
      case DIGI_MAP_ODU1EP_PKT_10G3_7_2_BMP:
        global_string = "ODU1EP_PKT_10G3_7_2_BMP";
        break;

      case DIGI_MAP_ODU2EP_PKT_10G3_7_1_BMP:
        global_string = "ODU1EP_PKT_10G3_7_2_BMP";
        break;

      case DIGI_MAP_ODU3P_OC_768_CBR_BMP:
        global_string = "ODU3P_OC_768_CBR_BMP";
        break;

      case DIGI_MAP_ODU2P_OC_192_CBR_BMP:
        global_string = "ODU2P_OC_192_CBR_BMP";
        break;

      case DIGI_MAP_ODU3P_OC_768_CBR_AMP:
        global_string = "ODU2P_OC_768_CBR_AMP";
        break;

      case DIGI_MAP_ODU2P_OC_192_CBR_AMP:
        global_string = "ODU2P_OC_192_CBR_AMP";
        break;      
      default:
        break;
    }


    PMC_RETURN(global_string);
} /* digi_resource_convert_mapper_mode */


/*******************************************************************************
* digi_resource_clean_mcpb_switch
* ______________________________________________________________________________
** DESCRIPTION:
*  Called after resource table has been cleaned in a restart. Iterates to
*  clean MCPB switch unprovisoned channels.
*  
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*
* OUTPUTS:
    None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_clean_mcpb_switch(digi_handle_t                  *digi_handle)
{
    UINT32 i = 0;
    BOOL8 unprov = FALSE;
    enet_handle_t *enet_handle_ptr = NULL;
    

    PMC_ENTRY();

    /*
     * Now iterate over the ENET_LINE and determine which of those are in
     * use and any that are not are not being used as ETRANS case.
     */
    /* egress_port = CPB_MCPB_EGR_FEGE; */
    /* enet_pkt_dst = DIGI_ENET_PKT_FWD_DST_MGMT_FEGE; */

    if (TRUE == digi_handle->var.mcpb_ctxt.mcpb_mux_mode_init &&
        UTIL_GLOBAL_MCPB_PORT_ENET_LINE == digi_mcpb_enet_port_type_get(digi_handle))
    {
        enet_handle_ptr = digi_handle->enet_line_handle;
        for (i = 0; i < DIGI_ENET_LINE_CHNL_MAX; i++)
        {
            unprov = (TRUE != util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.enet_line_chnl[i].switch_data.header.prov_state));
            
            /* ENET LINE -> MGMT_FEGE and MGMT_FEGE -> ENET LINE datapath */
            digi_resource_mcpb_port_cleanup(digi_handle,
                                            i,
                                            UTIL_GLOBAL_MCPB_PORT_ENET_LINE,
                                            UTIL_GLOBAL_MCPB_PORT_MGMT_FEGE,
                                            unprov);
            /* ENET LINE -> CPU and CPU -> ENET LINE datapath */
            digi_resource_mcpb_port_cleanup(digi_handle,
                                            i,
                                            UTIL_GLOBAL_MCPB_PORT_ENET_LINE,
                                            UTIL_GLOBAL_MCPB_PORT_CPU,
                                            unprov);
                                            
        }

    }

    if (TRUE == digi_handle->var.mcpb_ctxt.mcpb_mux_mode_init &&
        UTIL_GLOBAL_MCPB_PORT_ENET_SYS == digi_mcpb_enet_port_type_get(digi_handle))
    {
        enet_handle_ptr = digi_handle->enet_sys_handle;
        for (i = 0; i < DIGI_ENET_SYS_CHNL_MAX; i++)
        {
            unprov = (TRUE != util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.enet_sys_chnl[i].switch_data.header.prov_state));

            /* ENET  -> MGMT_FEGE and MGMT_FEGE -> ENET SYS datapath */
            digi_resource_mcpb_port_cleanup(digi_handle,
                                            i,
                                            UTIL_GLOBAL_MCPB_PORT_ENET_SYS,
                                            UTIL_GLOBAL_MCPB_PORT_MGMT_FEGE,
                                            unprov);
            /* ENET SYS -> CPU and CPU -> ENET SYS datapath */
            digi_resource_mcpb_port_cleanup(digi_handle,
                                            i,
                                            UTIL_GLOBAL_MCPB_PORT_ENET_SYS,
                                            UTIL_GLOBAL_MCPB_PORT_CPU,
                                            unprov);
            
        }
    }

    /* special case for MGMT_FEGE -> CPU & CPU -> MGMT_FEGE */
    digi_resource_mcpb_port_cleanup(digi_handle,
                                    i,
                                    UTIL_GLOBAL_MCPB_PORT_CPU,
                                    UTIL_GLOBAL_MCPB_PORT_MGMT_FEGE,
                                    FALSE);


    PMC_RETURN();
} /* digi_resource_clean_mcpb_switch */


/*******************************************************************************
* digi_resource_enet_mcpb_cleanup
* ______________________________________________________________________________
* DESCRIPTION:
* Cleanup possible MCPB datapaths between 2 ports.   
*  
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*    link                  - link id (used when port 1 is associated to ENET LINE or ENET SYS)
*    port_1                - MCPB port 1 (possible values: UTIL_GLOBAL_MCPB_PORT_ENET_LINE, UTIL_GLOBAL_MCPB_PORT_ENET_SYS, UTIL_GLOBAL_MCPB_PORT_CPU)
*    port_2                - MCPB port 2 (possible values: UTIL_GLOBAL_MCPB_PORT_CPU, UTIL_GLOBAL_MCPB_PORT_MGMT_FEGE)
*    unprov                - when TRUE link is unprovisionned, otherwise it is provisioned.
*                        
*
* OUTPUTS:
    None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_resource_mcpb_port_cleanup(digi_handle_t                   *digi_handle,
                                             UINT32                           link,
                                             util_global_mcpb_port_t          port_1,
                                             util_global_mcpb_port_t          port_2,
                                             BOOL8                            unprov)
{
    UINT32 jtr;
    UINT32 egress_output_port_chnl_l[2];
    cpb_mcpb_ingr_input_port_type_t ingr_port_l[2];
    cpb_mcpb_egr_output_port_type_t mcpb_egr_port[2];
    UINT32 egr_chnl;
    UINT32 ing_chnl;
    BOOL8  valid = TRUE;    
    PMC_ERROR rc;
    UINT32 ingress_mux_input_port;
    UINT32 ingress_mux_input_port_chnl;
    BOOL8 is_broadcast;
    BOOL8 is_multicast;
    BOOL8 is_reader_primary;
    BOOL8 is_slave_zone_primary;
    digi_enet_pkt_fwd_dst_t enet_pkt_dst;
    BOOL8 cleanup;
    UINT8 cleanup_eclass_i;
    UINT8 cleanup_eclass_e;
    enet_handle_t                   *enet_handle_ptr = NULL;
    cpb_mcpb_egr_output_port_type_t  egress_port;
    BOOL8 is_mgmt_fege_port = FALSE;
    UINT32 num_ports = 1;
    util_global_switch_data_t  *enet_chnl_handle_0 = NULL;
    
    PMC_ENTRY();
    
    ingr_port_l[0] = LAST_CPB_MCPB_INGRESS_PORT;
    ingr_port_l[1] = LAST_CPB_MCPB_INGRESS_PORT;
    /* prepare possible ingress and egress ports associated to port 1 */
    switch(port_1)
    {
    case UTIL_GLOBAL_MCPB_PORT_ENET_LINE:   
        ingr_port_l[0] = CPB_MCPB_ING_LINE_IM_EXT;
        ingr_port_l[1] = CPB_MCPB_ING_LINE_EM_EXT;

        mcpb_egr_port[0] = CPB_MCPB_EGR_LINE_ING;
        mcpb_egr_port[1] = CPB_MCPB_EGR_LINE_EG;
        enet_handle_ptr = digi_handle->enet_line_handle;
        enet_chnl_handle_0 = (util_global_switch_data_t  *)&digi_handle->handle_pool.enet_line_chnl[0];
        num_ports = 2;
        break;

    case UTIL_GLOBAL_MCPB_PORT_ENET_SYS:   
        ingr_port_l[0] = CPB_MCPB_ING_SYS_IM_EXT;
        ingr_port_l[1] = CPB_MCPB_ING_SYS_EM_EXT;

        mcpb_egr_port[0] = CPB_MCPB_EGR_SYS_ING;
        mcpb_egr_port[1] = CPB_MCPB_EGR_SYS_EG;
        enet_handle_ptr = digi_handle->enet_sys_handle;        
        enet_chnl_handle_0 = (util_global_switch_data_t  *)&digi_handle->handle_pool.enet_sys_chnl[0];
        num_ports = 2;

        break;
    case UTIL_GLOBAL_MCPB_PORT_CPU:
        ingr_port_l[0] = CPB_MCPB_ING_CPU_INS;
        mcpb_egr_port[0] = CPB_MCPB_EGR_FEGE;
        break;
    default:
        valid = FALSE;
        break;
    }

    /* Update FW enet client source if necessary */
    if (NULL != enet_handle_ptr)
    {
        rc = digi_enet_fw_enet_client_source_conditional_update(digi_handle, enet_chnl_handle_0);
    }


    /* prepare possible egress ports associated to port 2 */
    if (TRUE == valid)
    {
        switch(port_2)
        {
        case UTIL_GLOBAL_MCPB_PORT_MGMT_FEGE:
            enet_pkt_dst = DIGI_ENET_PKT_FWD_DST_MGMT_FEGE;
            egress_port = CPB_MCPB_EGR_FEGE;
            is_mgmt_fege_port = TRUE;
            break;
        case UTIL_GLOBAL_MCPB_PORT_CPU:
            enet_pkt_dst = DIGI_ENET_PKT_FWD_DST_CPU;
            egress_port = CPB_MCPB_EGR_CPU;
            break;
        default:
            valid = FALSE;
            break;
        }
    }
        

    if (TRUE == valid)
    {
        if (NULL != enet_handle_ptr)
        {
            egress_output_port_chnl_l[0] = (link << 2) + 1;
            egress_output_port_chnl_l[1] = (link << 2);
        }
        else
        {
            egress_output_port_chnl_l[0] = DIGI_CPU_TO_MGMT_FEGE_EGR_DPI_ID;
        }

        cleanup_eclass_e = FALSE;
        cleanup_eclass_i = FALSE;

        /* ingress path: port_1 -> MCPB -> port_2 */
        /* ENET has 2 inputs port in ingress side: 1 from ins (CLIENT -> ENET) and one from ext (DCPB -> ENET) paths */
        for (jtr = 0; jtr < num_ports; jtr += 1)
        {
            cleanup = FALSE;
            rc = PMC_SUCCESS;
            rc = cpb_chnl_source_type_and_primary_test(digi_handle->mcpb_handle,
                                                       egress_port,
                                                       egress_output_port_chnl_l[jtr],
                                                       &ingress_mux_input_port,
                                                       &ingress_mux_input_port_chnl,
                                                       &is_broadcast,
                                                       &is_multicast,
                                                       &is_reader_primary,
                                                       &is_slave_zone_primary);
            if (NULL != enet_handle_ptr && TRUE == unprov)
            {    
                cleanup = TRUE;
                cleanup_eclass_i = TRUE;
                cleanup_eclass_e = TRUE;
            }
            else
            {   
                if (NULL != enet_handle_ptr)
                {
                    if (jtr == 0 && FALSE == enet_handle_ptr->var.lnk_ctxt[link].empei_i_eclass_en)
                    {
                        /* cleanup port_1 possible channels in ingress side */
                        cleanup = TRUE;          
                        cleanup_eclass_i = TRUE;
                    }
                    else if (jtr == 1 && FALSE == enet_handle_ptr->var.lnk_ctxt[link].empei_e_eclass_en)
                    {
                        /* cleanup port_1 possible channels in egress side */
                        cleanup = TRUE;
                        cleanup_eclass_e = TRUE;
                    }                         
                }
                
                if (FALSE == cleanup)
                {
                    if (TRUE == is_mgmt_fege_port && FALSE == mgmt_fege_pkt_fwd_status_get(digi_handle->mgmt_fege_handle,
                                                                                           egress_output_port_chnl_l[jtr],
                                                                                           UTIL_GLOBAL_DATAPATH_DIR_EGRESS))
                    {
                        if (PMC_SUCCESS == rc)
                        {
                            if (ingr_port_l[0] == (cpb_mcpb_ingr_input_port_type_t)ingress_mux_input_port)
                            {
                                /* cleanup MGMT FEGE or CPU possible link in ingress side */
                                cleanup = TRUE;          
                                cleanup_eclass_i = TRUE;
                            }
                            else if (NULL != enet_handle_ptr &&
                                     ingr_port_l[1] == (cpb_mcpb_ingr_input_port_type_t)ingress_mux_input_port)
                            {
                                cleanup = TRUE;
                                cleanup_eclass_e = TRUE;
                            }         
                        }
                        else
                        {
                            cleanup = TRUE;
                        }
                    }                    
                }
            }

            /* if cleanup required: cleanup MCPB and MGMT_FEGE channel */
            if (TRUE == cleanup)
            {
                if (PMC_SUCCESS == rc)
                {
                    (void)digi_mcpb_chnl_deprov(digi_handle,
                                                egress_port,
                                                egress_output_port_chnl_l[jtr],
                                                (cpb_mcpb_ingr_input_port_type_t)ingress_mux_input_port,
                                                ingress_mux_input_port_chnl);  
                }
                if (TRUE == is_mgmt_fege_port)
                {
                    mgmt_fege_ingr_pkt_fwd_cleanup(digi_handle->mgmt_fege_handle, egress_output_port_chnl_l[jtr]);  
                }
            }
        }
        
        if (NULL != enet_handle_ptr)
        {
            /* cleanup eclass in eclass egress side if necessary */
            if (TRUE == cleanup_eclass_e)
            {
                (void)enet_eclass_pkt_extract_cleanup(enet_handle_ptr,
                                                      link,
                                                      ENET_EGRESS);   
                if (PMC_SUCCESS == rc)
                {
                    (void) digi_fw_eclass_hwire_cfg(digi_handle,
                                                    enet_pkt_dst,
                                                    ENET_EGRESS,
                                                    link,
                                                    FALSE,
                                                    FALSE,
                                                    UTIL_GLOBAL_PKT_TYPE_PTP_ALL,
                                                    FALSE,FALSE,0,0);
                }
            }

            /* cleanup eclass in eclass ingress side if necessary */
            if (TRUE == cleanup_eclass_i)
            {
                (void)enet_eclass_pkt_extract_cleanup(enet_handle_ptr,
                                                      link,
                                                      ENET_INGRESS); 
                if (PMC_SUCCESS == rc)
                {
                    (void) digi_fw_eclass_hwire_cfg(digi_handle,
                                                    enet_pkt_dst,
                                                    ENET_INGRESS,
                                                    link,
                                                    FALSE,
                                                    FALSE,
                                                    UTIL_GLOBAL_PKT_TYPE_PTP_ALL,
                                                    FALSE,FALSE,0,0);
                }
            }
        }
        
        /* egress path: port_2 -> MCPB -> port_1 */
        /* ENET has 2 inputs port in egress side: 1 from ing (CLIENT <- ENET) and one from eg (DCPB <- ENET) paths */
        /* or it could be MGMT_FEGE -> CPU channel */
        for (jtr = 0; jtr < num_ports; jtr += 1)
        { 
            valid = TRUE;
            switch (mcpb_egr_port[jtr])
            {
            case CPB_MCPB_EGR_LINE_ING:
            case CPB_MCPB_EGR_SYS_ING:
                ing_chnl = (link * 2);
                egr_chnl = link;
                break;

            case CPB_MCPB_EGR_LINE_EG:
            case CPB_MCPB_EGR_SYS_EG:
                ing_chnl = (link * 2) + 1;
                egr_chnl = link;
                break;              
            case CPB_MCPB_EGR_CPU:
                ing_chnl = DIGI_MGMT_FEGE_TO_CPU_ING_DPI_ID;
                egr_chnl = DIGI_MGMT_FEGE_TO_CPU_EGR_DPI_ID;
                break;
            default:
                valid = FALSE;
                break;             
            }

            if (FALSE == valid)
            {
                continue;
            }

            cleanup = FALSE;
            /* ingress path MGMT_FEGE -> ENET */
            rc = cpb_chnl_source_type_and_primary_test(digi_handle->mcpb_handle,
                                                       mcpb_egr_port[jtr],
                                                       egr_chnl,
                                                       &ingress_mux_input_port,
                                                       &ingress_mux_input_port_chnl,
                                                       &is_broadcast,
                                                       &is_multicast,
                                                       &is_reader_primary,
                                                       &is_slave_zone_primary);
            if (NULL != enet_handle_ptr && TRUE == unprov)
            {       
                cleanup = TRUE;  
            }
            else
            {      
                /* if MCPB path is valid but MGMT_FEGE channel is not configured then cleanup */
                if (PMC_SUCCESS == rc &&
                    TRUE == is_mgmt_fege_port && FALSE == mgmt_fege_pkt_fwd_status_get(digi_handle->mgmt_fege_handle,
                                                                                       ing_chnl,
                                                                                       UTIL_GLOBAL_DATAPATH_DIR_INGRESS))
                { 
                    /* cleanup MGMT FEGE possible link in ingress side */
                    cleanup = TRUE;  
                }       
                if (FALSE == cleanup)
                {
                    if (NULL != enet_handle_ptr)
                    {
                        if (jtr == 0 && ENET_LNK_START == enet_handle_ptr->var.lnk_ctxt[link].rx_state)
                        {
                            /* cleanup MGMT FEGE possible link in ingress side */
                            cleanup = TRUE;          
                        } 
                        if (jtr == 1 && ENET_LNK_START == enet_handle_ptr->var.lnk_ctxt[link].tx_state)
                        {
                            /* cleanup MGMT FEGE possible link in ingress side */
                            cleanup = TRUE;          
                        } 
                    }
                }                
            }  

            /* clenaup MCPB channel and MGMT_FEGE if required */
            if (TRUE == cleanup)
            {             
                if (PMC_SUCCESS == rc)
                {
                    if (CPB_MCPB_ING_FEGE_INS == ingress_mux_input_port ||
                        CPB_MCPB_ING_CPU_INS == ingress_mux_input_port)
                    {
                        if (ing_chnl == ingress_mux_input_port_chnl)
                        {
                            (void)digi_mcpb_chnl_deprov(digi_handle,
                                                        mcpb_egr_port[jtr],
                                                        egr_chnl,
                                                        TRUE == is_mgmt_fege_port ? CPB_MCPB_ING_FEGE_INS: CPB_MCPB_ING_CPU_INS,
                                                        ing_chnl);
                        }
                    }
                }
                if (TRUE == is_mgmt_fege_port)
                {
                    mgmt_fege_egr_pkt_fwd_cleanup(digi_handle->mgmt_fege_handle, ing_chnl); 
                }
            }
        }
    }
    
    PMC_RETURN();
} 

#endif /* DOXYGEN_PUBLIC_ONLY */

#ifdef PMC_DEPRECATED

/*******************************************************************************
* digi_resource_serdes_port_release
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Releases a data path from the specified serdes port up to the first 
*  switching point.  All OTN Resources are that configured as channelized 
*  OTN Payloads between the serdes ports and the first switching point are 
*  released.  Any resources that are beyond the current switching point
*  on the curent datapath are not released and need to be released
*  separately. If MAPOTN is included in the datapath its configured
*  resources associated with the switching point are released.
*  The actual MAPOTN channel however is not deprovisioned if it is used with
*  the another switching point.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *digi_resource_handle  - Resource handle table that has already been 
*                            built
*   intf                   - Serdes interface (SYSTEM, LINE, SFI51) the 
*                            serdes port emanates from.
*   serdes_port            - Serdes port to be released.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS - Datapath associated with the serdes port was successfully
*                 deactivated, demapped and deprovisioned.
*   PMC_ERROR   - Otherwise
*
* NOTES:
*   If the serdes port is not defined then an error is returned. If it is
*   defined by currently not configured then PMC_SUCCESS is returned.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_resource_serdes_port_release(digi_handle_t *digi_handle,
                                              digi_resource_handle_summary_t *digi_resource_handle,
                                              digi_serdes_intf_t intf,
                                              UINT16 serdes_port)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    BOOL8 port_found = FALSE;
    UINT8 i;

    PMC_ATOMIC_ENTRY(digi_handle);

    for (i=0; i < DIGI_XFI_PIN_NUM; i++)
    {
        switch (intf)
        {
          case DIGI_SERDES_LINE_INTF:

            if (serdes_port == digi_resource_handle->line_serdes_cfg[i].port_uid)
            {
                port_found = TRUE;

                if (NULL == digi_resource_handle->line_serdes_cfg[i].port_ctxt_ptr || NULL == digi_resource_handle->line_serdes_cfg[i].chnl_handle_ptr)
                {
                    /*
                     * If the handles are NULL then somethings is wrong. 
                     * This should serve as a sanity check and should never
                     * happe.
                     */
                  PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                          "Serdes Port=%d found but LineSerdes handles not set\n", serdes_port);
                    break;
                }
                else if (LAST_DIGI_RESOURCE_HANDLE == digi_resource_handle->line_serdes_cfg[i].next_resource_handle)
                {
                    /*
                     * If no next resource handle has been configured then
                     * this is not an error. Break with success in this case
                     */
                    result = PMC_SUCCESS;
                    break;
                }
                /*
                 * digi_resource_release_port_to_switch will recursively 
                 * move through the datapath deactivatin, demapping and 
                 * deprovisioning the resources as required.
                 */
                result = digi_resource_release_port_to_switch(digi_handle, 
                                                              digi_resource_handle,
                                                              digi_resource_handle->line_serdes_cfg[i].chnl_handle_ptr,
                                                              digi_resource_handle->line_serdes_cfg[i].next_resource_handle,
                                                              digi_resource_handle->line_serdes_cfg[i].next_handle_index);

                /*
                 * Before releasing the resources check whether or not this
                 * is an ENET_LINE or CBRC. If it is then need to 
                 * deprovision those channels.
                 */
                if (PMC_SUCCESS == result)
                {
                  result = digi_resource_deprovision_channel(digi_handle,
                                                             digi_resource_handle->line_serdes_cfg[i].chnl_handle_type,
                                                             digi_resource_handle->line_serdes_cfg[i].chnl_handle_ptr);
                }
                
                if (PMC_SUCCESS == result)
                {
                    digi_resource_handle->line_serdes_cfg[i].port_ctxt_ptr = NULL;
                    digi_resource_handle->line_serdes_cfg[i].port_uid = 0;
                    digi_resource_handle->line_serdes_cfg[i].chnl_handle_type = LAST_DIGI_HANDLE_ID;
                    digi_resource_handle->line_serdes_cfg[i].chnl_handle_ptr = NULL;
                    digi_resource_handle->line_serdes_cfg[i].next_resource_handle = LAST_DIGI_RESOURCE_HANDLE;
                    digi_resource_handle->line_serdes_cfg[i].next_handle_index = 0;
                }
            }
            break;
          case DIGI_SERDES_SYSTEM_INTF:
            if (serdes_port == digi_resource_handle->sys_serdes_cfg[i].port_uid)
            {
                port_found = TRUE;

                if (NULL == digi_resource_handle->sys_serdes_cfg[i].port_ctxt_ptr || NULL == digi_resource_handle->sys_serdes_cfg[i].chnl_handle_ptr)
                {
                    /*
                     * If the handles are NULL then somethings is wrong. 
                     * This should serve as a sanity check and should never
                     * happe.
                     */
                    break;
                }
                else if (LAST_DIGI_RESOURCE_HANDLE == digi_resource_handle->sys_serdes_cfg[i].next_resource_handle)
                {
                    /*
                     * If no next resource handle has been configured then
                     * this is not an error. Break with success in this case
                     */
                    result = PMC_SUCCESS;
                    break;
                }
                result = digi_resource_release_port_to_switch(digi_handle, 
                                                              digi_resource_handle,
                                                              digi_resource_handle->sys_serdes_cfg[i].chnl_handle_ptr,
                                                              digi_resource_handle->sys_serdes_cfg[i].next_resource_handle,
                                                              digi_resource_handle->sys_serdes_cfg[i].next_handle_index);

                if (PMC_SUCCESS == result)
                {
                    result = digi_resource_deprovision_channel(digi_handle,
                                                               digi_resource_handle->sys_serdes_cfg[i].chnl_handle_type,
                                                               digi_resource_handle->sys_serdes_cfg[i].chnl_handle_ptr);
                }

                if (PMC_SUCCESS == result)
                {
                    digi_resource_handle->sys_serdes_cfg[i].port_ctxt_ptr = NULL;
                    digi_resource_handle->sys_serdes_cfg[i].port_uid = 0;
                    digi_resource_handle->sys_serdes_cfg[i].chnl_handle_type = LAST_DIGI_HANDLE_ID;
                    digi_resource_handle->sys_serdes_cfg[i].chnl_handle_ptr = NULL;
                    digi_resource_handle->sys_serdes_cfg[i].next_resource_handle = LAST_DIGI_RESOURCE_HANDLE;
                    digi_resource_handle->sys_serdes_cfg[i].next_handle_index = 0;
                }
            }
            break;
          case DIGI_SERDES_SFI_51_INTF:
            PMC_LOG_TRACE("Inteface=SFI 51, SerdesPort=%d and PortAtIndex=%d is %d\n", serdes_port, i, digi_resource_handle->sfi51_serdes_cfg[i].port_uid);            
            if (DIGI_SFI51_INTF_NUM == i)
            {
                /*
                 * Set the port_found flag so that the for loop does not
                 * continue to iterate.
                 */
                port_found = TRUE;
                break;
            }
            if (serdes_port == digi_resource_handle->sfi51_serdes_cfg[i].port_uid)
            {
               port_found = TRUE;

                if (NULL == digi_resource_handle->sfi51_serdes_cfg[i].port_ctxt_ptr || NULL == digi_resource_handle->sfi51_serdes_cfg[i].chnl_handle_ptr)
                {
                    /*
                     * If the handles are NULL then somethings is wrong. 
                     * This should serve as a sanity check and should never
                     * happe.
                     */
                    break;
                }
                else if (LAST_DIGI_RESOURCE_HANDLE == digi_resource_handle->sfi51_serdes_cfg[i].next_resource_handle)
                {
                    /*
                     * If no next resource handle has been configured then
                     * this is not an error. Break with success in this case
                     */
                    result = PMC_SUCCESS;
                    break;
                }
                result = digi_resource_release_port_to_switch(digi_handle, 
                                                              digi_resource_handle,
                                                              digi_resource_handle->sfi51_serdes_cfg[i].chnl_handle_ptr,
                                                              digi_resource_handle->sfi51_serdes_cfg[i].next_resource_handle,
                                                              digi_resource_handle->sfi51_serdes_cfg[i].next_handle_index);

                if (PMC_SUCCESS == result)
                {
                    result = digi_resource_deprovision_channel(digi_handle,
                                                               digi_resource_handle->sfi51_serdes_cfg[i].chnl_handle_type,
                                                               digi_resource_handle->sfi51_serdes_cfg[i].chnl_handle_ptr);
                }
                
                if (PMC_SUCCESS == result)
                {
                    digi_resource_handle->sfi51_serdes_cfg[i].port_ctxt_ptr = NULL;
                    digi_resource_handle->sfi51_serdes_cfg[i].port_uid = 0;
                    digi_resource_handle->sfi51_serdes_cfg[i].chnl_handle_type = LAST_DIGI_HANDLE_ID;
                    digi_resource_handle->sfi51_serdes_cfg[i].chnl_handle_ptr = NULL;
                    digi_resource_handle->sfi51_serdes_cfg[i].next_resource_handle = LAST_DIGI_RESOURCE_HANDLE;
                    digi_resource_handle->sfi51_serdes_cfg[i].next_handle_index = 0;
                }
            }
            break;
            
          default:
            PMC_LOG_TRACE("Interface=%d is valid for the resource table\n", 
                          intf);
            break;
        }

        if (TRUE == port_found)
        {
            break;
        }
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_resource_serdes_port_release */

/*******************************************************************************
* digi_resource_ilkn_channel_release
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Releases all provisioned related resources associated with a ILKN channel
*  based its transmit and receive Burst Control Word channel . The BCW 
*  channel was used when it was configured and provisioned and is used to
*  locate the channel and the deactivate it if it was activated, demap it
*  and finally deprovision it.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *digi_resource_handle  - Resource handle table 
*   tx_ilkn_bcw_chnl       - Tranmit BCW channel value.
*   rx_ilkn_bcw_chnl       - Receive BCW channel value.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS - Resources associated with an ILKN channel identified by
*                 its tx/tx BCW channels has been successfully deprovisioned
*   PMC_ERROR   - There was an error eithe deactivating. demapping or
*                 deprovisioning.
*
* NOTES:
*  Operation is idempotent. If the BCW RX/TX channels are not associated
*  with an ILKN channel handle then success is return. As well if not ILKN 
*  channels have been provisioned then SUCCESS is returned. However if one 
*  of the channels is and one is not then an ERROR is returned.  That is if
*  the Tx BCW channel is associated with an ILKN channel handle but the Rx 
*  is not then an ERROR is returned.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_resource_ilkn_channel_release(digi_handle_t *digi_handle,
                                                    digi_resource_handle_summary_t *digi_resource_handle,
                                                    UINT32 tx_ilkn_bcw_chnl,
                                                    UINT32 rx_ilkn_bcw_chnl)                                                   
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    BOOL8 ilkn_found = FALSE;
    digi_resource_next_handle_t ilkn_handle = DIGI_ILKN_1;
    digi_ilkn_chnl_def_t *ilkn_chnl_ptr = NULL;
    UINT8 ilkn_index;
    UINT16 i;

    PMC_ATOMIC_ENTRY(digi_handle);

    for(ilkn_index=0; ilkn_index < 2; ilkn_index++)
    {
        if (1 == ilkn_index)
        {
            ilkn_handle =  DIGI_ILKN_2;
        }
        for (i=0; i < DIGI_ILKN_CHNL_NUM; i++)
        {
            ilkn_chnl_ptr = (digi_ilkn_chnl_def_t *)digi_resource_handle->ilkn_chnl_cfg[ilkn_index][i].ilkn_chnl_handle_ptr;
            
            if (NULL != ilkn_chnl_ptr &&
                UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG != ilkn_chnl_ptr->switch_data.header.prov_state &&
                (ilkn_chnl_ptr->tx_ilkn_info.ilkn_bcw_chnl == tx_ilkn_bcw_chnl ||
                 ilkn_chnl_ptr->rx_ilkn_info.ilkn_bcw_chnl == rx_ilkn_bcw_chnl))
            {
                ilkn_found = TRUE;
           
                /*
                 * Check that both rx and tx match. If only one matches then
                 * return error.
                 */
                if (!(ilkn_chnl_ptr->tx_ilkn_info.ilkn_bcw_chnl == tx_ilkn_bcw_chnl &&
                      ilkn_chnl_ptr->rx_ilkn_info.ilkn_bcw_chnl == rx_ilkn_bcw_chnl))
                {
                    result = DIGI_ERR_INVALID_ARG;
                    break;
                }
                
                /*
                 * Call the existing API to release the ILKN channel. It is
                 * used slightly differently then other calls although the
                 * other calls would not be calling to release an ILKN
                 * channel.
                 */
                result = digi_resource_release_port_to_switch(digi_handle, 
                                                              digi_resource_handle,
                                                              ilkn_chnl_ptr,
                                                              ilkn_handle,
                                                              i);
                break;
              }
          }

        if (TRUE == ilkn_found)
        {
            break;
        }
    }
    if (DIGI_ILKN_CHNL_NUM == i && FALSE == ilkn_found)
    {
        /*
         * If have iterated over both ILKN 1 and 2 and not Tx/Rx BCW channel
         * was found then set the result to SUCCESS and return.
         */
        result = PMC_SUCCESS;
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_resource_ilkn_channel_release */

#endif /* PMC_DEPRECATED */

/* LCOV_EXCL_STOP */
