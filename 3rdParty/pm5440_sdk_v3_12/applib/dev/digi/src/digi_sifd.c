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
*   DESCRIPTION : This file contains C functions for operating SIFD features.
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/
#include "digi_loc.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*
** Local Macro Definitions
*/

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
PRIVATE PMC_ERROR digi_sifd_pmon_link_retrieve(digi_handle_t           *digi_handle,
                                               digi_ilkn_link_t        *inst_handle,   
                                               digi_sifd_pmon_link_t   *pmon_table_ptr);
PRIVATE PMC_ERROR digi_pbrs_mode_to_uint_conv(util_patt_mode_t util_patt_mode,
                                              UINT32 *pattern);
PRIVATE PMC_ERROR digi_uint_to_pbrs_mode_conv(UINT32 pattern,
                                              util_patt_mode_t *util_patt_mode);

PUBLIC PMC_ERROR digi_sifd_burst_min_conv(digi_sifd_burst_min_t    digi_burst_min,
                                          sifd_burst_min_t        *sifd_burst_min);

PRIVATE PMC_ERROR digi_sifd_int_link_enabled_check_internal(digi_handle_t     *digi_handle,
                                                            digi_ilkn_chnl_t  *chnl_handle,
                                                            BOOL              *int_found_ptr);


PRIVATE PMC_ERROR digi_sifd_enet_pmon_int_chnl_cfg(digi_handle_t             *digi_handle,
                                                   digi_ilkn_chnl_t          *chnl_handle,
                                                   enet_int_chnl_t           *int_table_ptr,
                                                   BOOL8                      clear,
                                                   BOOL8                      enable);


#endif

/*******************************************************************************
* digi_ilkn_oduk_rx_2b_header_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*
*   This function can be used to dynamically update RX 2B header on a specified\n
*   channel. This function can only be used under the following conditions:
*    - ILKN link shall be configured in 2B header checking without 4B header 
*       - digi_sifd_ilkn_cfg_t::oduksw_2b_header_mode == FALSE when configuring ILKN link
*    - specified channel shall be mapped to the ODUKSW 
*       - digi_sifd_ilkn_cfg_t::oduksw_intf_en == TRUE when configuring ILKN link
*    - at least 1 bit should remain static (0 or 1) from the previous and new  \n
*      2B header value (see HWS  5.11.4.5.7 for details)
*   
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   *chnl_ctxt_ptr         - a pointer to a digi_ilkn_chnl_t handle.
*   rx_2b_header           - Defines the 2B header for sink ODUKSW channels.
*
* OUTPUTS:
*   NONE
*    
* RETURNS:
*   DIGI_ERR_INVALID_ARG
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ilkn_oduk_rx_2b_header_set(digi_handle_t *digi_handle,
                                                 digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                                 UINT16 rx_2b_header)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_ilkn_chnl_def_t *chnl_def_ptr;
    digi_sifd_ilkn_cfg_t *ilkn_ptr;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* unbox the opaque pointer */
    chnl_def_ptr = (digi_ilkn_chnl_def_t *) chnl_ctxt_ptr;


    /* check parameters */
    if (chnl_def_ptr->switch_data.header.prov_state != UTIL_GLOBAL_CHNL_PROV_STATE_CONFIG)
    {
        rc = DIGI_ERR_ILKN_CHNL_NOT_PROV;
    } else if (chnl_def_ptr->target == DIGI_ILKN_CHNL_TARGET_CPB)
    {
        rc  = DIGI_ERR_ILKN_CPB_PROV;
    } else if (chnl_def_ptr->sifd_path != DIGI_SIFD_PATH_ILKN_1 &&
               chnl_def_ptr->sifd_path != DIGI_SIFD_PATH_ILKN_2)        
    {
        rc = DIGI_ERR_ILKN_IN_WRONG_MAJ_MODE;
    } else 
    {
        ilkn_ptr     = (chnl_def_ptr->sifd_path ==  DIGI_SIFD_PATH_ILKN_1)?
            &digi_handle->var.sifd_ilkn_1_cfg.ilkn:
            &digi_handle->var.sifd_ilkn_2_cfg.ilkn;
        if (ilkn_ptr->oduksw_2b_header_mode == TRUE)
        {
            rc = DIGI_ERR_ILKN_IN_WRONG_HEADER_MODE;
        }        
    }
    /* 
       perform the update 
       the check for valid rx_2b_header is done at ss-level
    */
    rc = sifd_ilkn_chnl_2b_header_update(digi_handle->sifd_handle,
                                         chnl_def_ptr->sifd_path == DIGI_SIFD_PATH_ILKN_1?SIFD_COPI_ILKN1:SIFD_COPI_ILKN2,
                                         chnl_def_ptr->switch_data.oduk_data.channel,
                                         TRUE, /* update_rx */                                         
                                         chnl_def_ptr->rx_ilkn_info.ilkn_2b_header,
                                         rx_2b_header);
    if (PMC_SUCCESS == rc)
    {
        chnl_def_ptr->rx_ilkn_info.ilkn_2b_header = rx_2b_header;
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_ilkn_oduk_rx_2b_header_set */

/*******************************************************************************
* digi_ilkn_oduk_rx_2b_header_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*
*   This function returns the current RX 2B header from a specified channel.
*   
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   *chnl_ctxt_ptr         - a pointer to a digi_ilkn_chnl_t handle
*
* OUTPUTS:
*   *rx_2b_header          - the RX 2B header
*    
* RETURNS:
*   DIGI_ERR_INVALID_ARG
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ilkn_oduk_rx_2b_header_get(digi_handle_t *digi_handle,
                                                 digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                                 UINT16 *rx_2b_header)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_ilkn_chnl_def_t *chnl_def_ptr;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* unbox the opaque pointer */
    chnl_def_ptr = (digi_ilkn_chnl_def_t *) chnl_ctxt_ptr;
   
 
    /* check parameters */
    if (chnl_def_ptr->switch_data.header.prov_state == UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG)
    {
        rc = DIGI_ERR_ILKN_CHNL_NOT_PROV;
    } else if (chnl_def_ptr->target == DIGI_ILKN_CHNL_TARGET_CPB)
    {
        rc  = DIGI_ERR_ILKN_CPB_PROV;
    } else if (chnl_def_ptr->sifd_path != DIGI_SIFD_PATH_ILKN_1 &&
               chnl_def_ptr->sifd_path != DIGI_SIFD_PATH_ILKN_2)        
    {
        rc = DIGI_ERR_ILKN_IN_WRONG_MAJ_MODE;
    } 

    *rx_2b_header = chnl_def_ptr->rx_ilkn_info.ilkn_2b_header;

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_ilkn_oduk_rx_2b_header_get */

/*******************************************************************************
* digi_ilkn_oduk_tx_2b_header_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*
*   This function can be used to dynamically update TX 2B header on a specified\n
*   channel. This function can only be used under the following condition:
*    - ILKN link shall be configured in 2B header insertion without 4B header 
*       - digi_sifd_ilkn_cfg_t::oduksw_2b_header_mode == FALSE when configuring ILKN link
*    - specified channel shall be mapped to the ODUKSW 
*       - digi_sifd_ilkn_cfg_t::oduksw_intf_en == TRUE when configuring ILKN link
*    - at least 1 bit should remain static (0 or 1) from the previous and new  \n
*      2B header value (see HWS  5.11.4.5.7 for details)
*   
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   *chnl_ctxt_ptr         - a pointer to a digi_ilkn_chnl_t handle.
*   tx_2b_header              - Defines the 2B header for source ODUKSW channels.
*
* OUTPUTS:
*   NONE
*    
* RETURNS:
*   DIGI_ERR_INVALID_ARG
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ilkn_oduk_tx_2b_header_set(digi_handle_t *digi_handle,
                                                 digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                                 UINT16 tx_2b_header)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_ilkn_chnl_def_t *chnl_def_ptr;
    digi_sifd_ilkn_cfg_t *ilkn_ptr;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* unbox the opaque pointer */
    chnl_def_ptr = (digi_ilkn_chnl_def_t *) chnl_ctxt_ptr;


    /* check parameters */
    if (chnl_def_ptr->switch_data.header.prov_state != UTIL_GLOBAL_CHNL_PROV_STATE_CONFIG)
    {
        rc = DIGI_ERR_ILKN_CHNL_NOT_PROV;
    } else if (chnl_def_ptr->target == DIGI_ILKN_CHNL_TARGET_CPB)
    {
        rc  = DIGI_ERR_ILKN_CPB_PROV;
    } else if (chnl_def_ptr->sifd_path != DIGI_SIFD_PATH_ILKN_1 &&
               chnl_def_ptr->sifd_path != DIGI_SIFD_PATH_ILKN_2)        
    {
        rc = DIGI_ERR_ILKN_IN_WRONG_MAJ_MODE;
    } else 
    {
        ilkn_ptr     = (chnl_def_ptr->sifd_path ==  DIGI_SIFD_PATH_ILKN_1)?
            &digi_handle->var.sifd_ilkn_1_cfg.ilkn:
            &digi_handle->var.sifd_ilkn_2_cfg.ilkn;
        if (ilkn_ptr->oduksw_2b_header_mode == TRUE)
        {
            rc = DIGI_ERR_ILKN_IN_WRONG_HEADER_MODE;
        }        
    }

    /* 
       perform the update 
       the check for valid tx_2b_header is done at ss-level
    */
    rc = sifd_ilkn_chnl_2b_header_update(digi_handle->sifd_handle,
                                         chnl_def_ptr->sifd_path == DIGI_SIFD_PATH_ILKN_1?SIFD_COPI_ILKN1:SIFD_COPI_ILKN2,
                                         chnl_def_ptr->switch_data.oduk_data.channel,
                                         FALSE, /* update_rx */                                         
                                         chnl_def_ptr->tx_ilkn_info.ilkn_2b_header,
                                         tx_2b_header);
    if (PMC_SUCCESS == rc)
    {
        chnl_def_ptr->tx_ilkn_info.ilkn_2b_header = tx_2b_header;
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_ilkn_oduk_tx_2b_header_set */

/*******************************************************************************
* digi_ilkn_oduk_tx_2b_header_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*
*   This function returns the current TX 2b header from a specified channel.
*   
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   *chnl_ctxt_ptr         - a pointer to a digi_ilkn_chnl_t handle
*
* OUTPUTS:
*   *tx_2b_header             - the TX 2B header
*    
* RETURNS:
*   DIGI_ERR_INVALID_ARG
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ilkn_oduk_tx_2b_header_get(digi_handle_t *digi_handle,
                                                 digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                                 UINT16 *tx_2b_header)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_ilkn_chnl_def_t *chnl_def_ptr;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* unbox the opaque pointer */
    chnl_def_ptr = (digi_ilkn_chnl_def_t *) chnl_ctxt_ptr;
   

    /* check parameters */
    if (chnl_def_ptr->switch_data.header.prov_state == UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG)
    {
        rc = DIGI_ERR_ILKN_CHNL_NOT_PROV;
    } else if (chnl_def_ptr->target == DIGI_ILKN_CHNL_TARGET_CPB)
    {
        rc  = DIGI_ERR_ILKN_CPB_PROV;
    } else if (chnl_def_ptr->sifd_path != DIGI_SIFD_PATH_ILKN_1 &&
               chnl_def_ptr->sifd_path != DIGI_SIFD_PATH_ILKN_2)        
    {
        rc = DIGI_ERR_ILKN_IN_WRONG_MAJ_MODE;
    } 

    *tx_2b_header = chnl_def_ptr->tx_ilkn_info.ilkn_2b_header;

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_ilkn_oduk_tx_2b_header_get */

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* digi_sifd_db_test_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API test/update the scheduler address for all DB related to SIFD (CPB DCS) 
*   to ensure no allocation conflict and updates the relevant database set with the
*   appropriate sifd channel data.
*
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   sifd_ilkn_inst    - Enum, defining SIFD interface
*   schd_addr         - calendar offset to be read and set for link data.
*                       Valid values 0 to 95.
*   chnl              - SIFD channel
*
*
* OUTPUTS:
*
*
* RETURNS:
*   BOOL              - Success indication of updating the potential scheduler
*                       offset:\n
*                       TRUE:  Scheduler entry available and updated with
*                              provided channel data\n
*                       FALSE: Scheduler entry already occupied. No
*                              modification to db entry.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL digi_sifd_db_test_update(digi_handle_t *digi_handle,
                                     digi_sifd_ilkn_inst_t sifd_ilkn_inst,
                                     UINT32 schd_addr,
                                     UINT32 chnl)
{
    util_global_cpb_port_t cpb_port;
    util_gen_db_id_t dcs_inst;
    UINT32 output_port, output_chnl, rd_channel_id;
    PMC_ERROR result = PMC_SUCCESS;    

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    switch(sifd_ilkn_inst)
    {
    case DIGI_SIFD_ILKN1:
        dcs_inst = (util_gen_db_id_t)0;
        cpb_port = UTIL_GLOBAL_CPB_PORT_ILKN1;
        break;
    case DIGI_SIFD_ILKN2:
        dcs_inst = (util_gen_db_id_t)1;
        cpb_port = UTIL_GLOBAL_CPB_PORT_ILKN2;
        break;
    default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    }
    
    PMC_LOG_TRACE("dcpb dcs instance: %d\n", dcs_inst);     

    /* test all db for this entry being available */
    if(PMC_SUCCESS == result)
    {
        result = cpb_dcs_db_entry_get(digi_handle->dcpb_handle,
                                      dcs_inst, schd_addr,
                                      &output_port,
                                      &output_chnl,
                                      &rd_channel_id);
        if(DIGI_SCHD_ENTRY_UNASSGINED != rd_channel_id )
        {
            PMC_RETURN(FALSE);
        }
    }

    /* write all database with appropriate values */
    if(PMC_SUCCESS == result)
    {
        result = cpb_dcs_db_entry_set(digi_handle->dcpb_handle,
                                      dcs_inst,
                                      schd_addr,
                                      cpb_port,
                                      chnl);
    }

    if(PMC_SUCCESS != result)
    {
        PMC_ASSERT(FALSE, result, 0, 0);
    }

    PMC_RETURN(TRUE);

} /* digi_sifd_db_test_update */

                                            
/*******************************************************************************
* digi_sifd_db_prov_cfg
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
*   sifd_ilkn_inst    - Enum, defining SIFD interface
*   chnl              - SIFD channel
*   num_cal_entry_req - Number of scheduler entries required
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
PUBLIC PMC_ERROR digi_sifd_db_prov_cfg(digi_handle_t *digi_handle,
                                       digi_sifd_ilkn_inst_t sifd_ilkn_inst,
                                       UINT32 chnl,
                                       UINT8 num_cal_entry_req)
{
    UINT32 schd_addr;
    UINT8 success_update = 0;
    UINT32 num_current_entries = DIGI_SCHD_96_ENTRY + 1;
    BOOL update = FALSE;

    util_global_cpb_port_t cpb_port;
    util_gen_db_id_t dcs_inst;

    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl < 150,DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(num_cal_entry_req <= DIGI_SCHD_96_ENTRY,DIGI_ERR_INVALID_ARG, 0, 0);
     

    switch(sifd_ilkn_inst)
    {
    case DIGI_SIFD_ILKN1:
        dcs_inst = (util_gen_db_id_t)0;
        cpb_port = UTIL_GLOBAL_CPB_PORT_ILKN1;
        break;
    case DIGI_SIFD_ILKN2:
        dcs_inst = (util_gen_db_id_t)1;
        cpb_port = UTIL_GLOBAL_CPB_PORT_ILKN2;
        break;
    default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    }
    
    PMC_LOG_TRACE("dcpb dcs instance: %d\n", dcs_inst);                              

    /* determine how many entries already exist for the channel, if any, then 
       generate a driver error. */

    if(PMC_SUCCESS == result &&
       0 != num_cal_entry_req)
    {
        result = cpb_dcs_db_num_entry_get(digi_handle->dcpb_handle,
                                          dcs_inst,
                                          cpb_port,
                                          chnl,
                                          &num_current_entries);
        PMC_ASSERT(num_current_entries == 0, DIGI_ERR_CODE_ASSERT, 0, 0);
    }

    if(PMC_SUCCESS == result &&
       0 != num_cal_entry_req)
    {

        for(schd_addr=0; schd_addr<DIGI_SCHD_96_ENTRY; schd_addr++)
        {
            PMC_LOG_TRACE("schd_addr:%d  success_update:%d\n", schd_addr, success_update);
            update = digi_sifd_db_test_update(digi_handle, sifd_ilkn_inst, schd_addr, chnl);
            if(TRUE == update)
            {
                success_update ++;
            }
            if(num_cal_entry_req == success_update)
            {
                schd_addr = DIGI_SCHD_96_ENTRY;
            }
        }

        if(num_cal_entry_req != success_update)
        {
            /*remove all entries that was added if not enough recources was available  */
            result = digi_sifd_db_clear(digi_handle, sifd_ilkn_inst, chnl);

            if(PMC_SUCCESS == result)
            {
                result = DIGI_ERR_SCHD_INSUFFICIENT_RESOURCES_AVAILABLE;
            }
        }
    }
    PMC_RETURN(result);

} /* digi_sifd_db_prov_cfg */

/*******************************************************************************
* digi_sifd_db_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API clears database entries used by an ENET SYS link in the relevant
*   databases.
*
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   sifd_ilkn_inst    - Enum, defining SIFD interface
*   chnl              - SIFD channel
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
PUBLIC PMC_ERROR digi_sifd_db_clear(digi_handle_t *digi_handle,
                                    digi_sifd_ilkn_inst_t sifd_ilkn_inst,
                                    UINT32 chnl)
{
    util_global_cpb_port_t cpb_port;
    util_gen_db_id_t dcs_inst;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    switch(sifd_ilkn_inst)
    {
    case DIGI_SIFD_ILKN1:
        dcs_inst = (util_gen_db_id_t)CPB_DCS_INST_0;
        cpb_port = UTIL_GLOBAL_CPB_PORT_ILKN1;
        break;
    case DIGI_SIFD_ILKN2:
        dcs_inst = (util_gen_db_id_t)CPB_DCS_INST_1;
        cpb_port = UTIL_GLOBAL_CPB_PORT_ILKN2;
        break;
    default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    }

    result = cpb_dcs_db_entry_clear(digi_handle->dcpb_handle,
                                    dcs_inst,
                                    cpb_port,
                                    chnl);

    PMC_RETURN(result);

} /* digi_sifd_db_clear */

/*******************************************************************************
* digi_ilkn_unused_chnl_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Retrives the unused ILKN channel ID.
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *digi_handle         - pointer to DIGI handle instance to be operated on
*   target               - Subsystem to source/sink data for the channel.
*                          See digi_ilkn_chnl_target_t for more information.
*   digi_sifd_ilkn_inst  - The SIFD/ILKN instance to obtain an unused 
*                          channel from
*                                                                               
* OUTPUTS:
*   *chnl_index          - pointer to unused channel index.
*
* RETURNS:
*   PMC_SUCCESS on success, or specific error otherwise.
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC PMC_ERROR digi_ilkn_unused_chnl_get(digi_handle_t* digi_handle, 
                                           digi_ilkn_chnl_target_t target,
                                           digi_sifd_ilkn_inst_t digi_sifd_ilkn_inst,
                                           UINT32* chnl_index)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
   
    PMC_ENTRY();

    if (digi_sifd_ilkn_inst == DIGI_SIFD_ILKN1) {

        result = digi_ilkn_1_unused_chnl_get(digi_handle, target, chnl_index);

    } else if (digi_sifd_ilkn_inst == DIGI_SIFD_ILKN2) {

        result = digi_ilkn_2_unused_chnl_get(digi_handle, target, chnl_index);
    } 

    PMC_RETURN(result);
} /* digi_ilkn_unused_chnl_get */   

/*******************************************************************************
* digi_ilkn_1_unused_chnl_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Retrives the unused ILKN 1 channel ID.
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *digi_handle         - pointer to DIGI handle instance to be operated on
*   target               - Subsystem to source/sink data for the channel.
*                          See digi_ilkn_chnl_target_t for more information.
*                                                                               
* OUTPUTS:
*   *chnl_index          - pointer to unused channel index.
*
* RETURNS:
*   PMC_SUCCESS on success, or specific error otherwise.
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC PMC_ERROR digi_ilkn_1_unused_chnl_get(digi_handle_t* digi_handle, 
                                             digi_ilkn_chnl_target_t target,
                                             UINT32* chnl_index)
{
    PMC_ERROR result = DIGI_ERR_ILKN_1_CHNL_NOT_AVAIL;
    UINT32 i = 0;

    PMC_ENTRY();

    /* There are DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX usable channels 
     * 0 - (DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX-1) are reserved for CPB purposes
     * DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX through DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX 
     * are reserved for ODUK purposes */
    if (target == DIGI_ILKN_CHNL_TARGET_CPB) {

        for(i = 0; i < DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX; i++)
        {
            if (digi_handle->handle_pool.ilkn_1_chnl[i].switch_data.cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_NOT_USED)
            {
                /* found a unconfigured/not_used chnl */
                PMC_LOG_TRACE("ILKN 1 available dpi channel is:%d\n", i);
                *chnl_index = i;
                result = PMC_SUCCESS;
                break;
            }
        }
        
    } else {

        for(i = DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX; i < DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX; i++)
        {
            if (digi_handle->handle_pool.ilkn_1_chnl[i].switch_data.oduk_data.port_type == UTIL_GLOBAL_ODUK_PORT_NOT_USED)
            {
                /* found a unconfigured/not_used chnl */
                PMC_LOG_TRACE("ILKN 1 available dpi channel is:%d\n", i);
                *chnl_index = i;
                result = PMC_SUCCESS;
                break;
            }
        }
    }
    
    PMC_RETURN(result);
} /* digi_ilkn_1_unused_chnl_get */

/*******************************************************************************
* digi_ilkn_2_unused_chnl_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Retrives the unused ILKN 2 channel ID.
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *digi_handle         - pointer to DIGI handle instance to be operated on
*   target               - Subsystem to source/sink data for the channel.
*                          See digi_ilkn_chnl_target_t for more information.
*                                                                               
* OUTPUTS:
*   *chnl_index          - pointer to unused channel index.
*
* RETURNS:
*   PMC_SUCCESS on success, or specific error otherwise.
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC PMC_ERROR digi_ilkn_2_unused_chnl_get(digi_handle_t* digi_handle, 
                                             digi_ilkn_chnl_target_t target,
                                             UINT32* chnl_index)
{
    PMC_ERROR result = DIGI_ERR_ILKN_2_CHNL_NOT_AVAIL;
    UINT32 i = 0;

    PMC_ENTRY();

    /* There are DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX usable channels 
     * 0 - (DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX-1) are reserved for CPB purposes
     * DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX through DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX 
     * are reserved for ODUK purposes */
    if (target == DIGI_ILKN_CHNL_TARGET_CPB) {

        for(i = 0; i < DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX; i++)
        {
            if ((digi_handle->handle_pool.ilkn_2_chnl[i].switch_data.cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_NOT_USED))
            {
                /* found a unprovised/free chnl */
                PMC_LOG_TRACE("ILKN 2 available dpi channel is:%d\n", i);
                *chnl_index = i;
                result = PMC_SUCCESS;
                break;
            }
        }
        
    } else {

        for(i = DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX; i < DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX; i++)
        {
            if (digi_handle->handle_pool.ilkn_2_chnl[i].switch_data.oduk_data.port_type == UTIL_GLOBAL_ODUK_PORT_NOT_USED)
            {
                /* found a unprovised/free chnl */
                PMC_LOG_TRACE("ILKN 2 available dpi channel is:%d\n", i);
                *chnl_index = i;
                result = PMC_SUCCESS;
                break;
            }
        }
    }
    
    PMC_RETURN(result);
} /* digi_ilkn_2_unused_chnl_get */

/*******************************************************************************
* digi_ilkn_chnl_cfg_params_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*
*   This simply performs argument validation on the call to digi_ilkn_chnl_cfg
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   *ilkn_ctxt_ptr         - Interlaken configuration information obtained
*                            from digi_device_cfg(). Either the configuration
*                            for ILKN1 or ILKN2 will need to be passed here to
*                            configure a channel against either ILKN blk.
*   target                 - Subsystem to source/sink data for the channel.
*                            See digi_ilkn_chnl_target_t for more information.
*   tx_ilkn_bcw_chnl       - Defines the burst control word channel that will be
*                            associated with the outbound channel.
*                            Used for output_port ILKN1/ILKN2 only.
*   tx_2b_header           - Defines the 2B header for source ODUKSW channels.
*   tx_4b_header           - Defines the 4B header for source ODUKSW channels.
*   rx_ilkn_bcw_chnl       - Defines the burst control word channel that will be
*                            associated with the inbound channel
*   rx_2b_header           - Defines the 2B header for sink ODUKSW channels.
*   rx_4b_header           - Defines the expected 4B header for sink ODUKSW channels.
*
* OUTPUTS:
*   **chnl_ctxt_pptr       - a pointer to a pointer to a digi_ilkn_chnl_t
*                            structure consisting of a channel data this is
*                            returned to the user.
*
* RETURNS:
*   DIGI_ERR_INVALID_ARG
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ilkn_chnl_cfg_params_validate(digi_handle_t *digi_handle,
                                                    digi_ilkn_link_t *ilkn_ctxt_ptr,
                                                    digi_ilkn_chnl_target_t target,
                                                    UINT32 tx_ilkn_bcw_chnl,
                                                    UINT16 tx_2b_header,
                                                    UINT32 tx_4b_header,
                                                    UINT32 rx_ilkn_bcw_chnl,
                                                    UINT16 rx_2b_header,
                                                    UINT32 rx_4b_header,
                                                    digi_ilkn_chnl_t **chnl_ctxt_pptr)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    
    digi_ilkn_link_def_t* ilkn_data_ptr = NULL;

    UINT32 ilkn_index;

    PMC_ENTRY();

    if (digi_handle == NULL) {

        rc = DIGI_ERR_NULL_HANDLE;

    } else if (ilkn_ctxt_ptr == NULL) {

        rc = DIGI_ERR_INVALID_ARG;

    } else if (PMC_SUCCESS != digi_ilkn_link_handle_validate(digi_handle, ilkn_ctxt_ptr)) {
        
        rc = DIGI_ERR_INVALID_ARG;
    } else if (target >= LAST_DIGI_ILKN_CHNL_TARGET) {

        rc = DIGI_ERR_INVALID_ARG;

    } else if (rx_ilkn_bcw_chnl > DIGI_SIFD_BCW_CHANNEL_MAX) {

        rc = DIGI_ERR_INVALID_ARG;

    } else if (tx_ilkn_bcw_chnl > DIGI_SIFD_BCW_CHANNEL_MAX) {

        rc = DIGI_ERR_INVALID_ARG;

    } else if (chnl_ctxt_pptr == NULL) {

        rc = DIGI_ERR_NULL_HANDLE;

    } else {

        ilkn_data_ptr = (digi_ilkn_link_def_t*)ilkn_ctxt_ptr;

        switch(ilkn_data_ptr->ilkn_inst) {

        case DIGI_SIFD_ILKN1: 

            if ((target == DIGI_ILKN_CHNL_TARGET_ODUK) &&
                (digi_handle->var.sifd_ilkn_1_cfg.ilkn.oduksw_intf_en == FALSE)) {

                rc = DIGI_ERR_ILKN_ODUKSW_INTF_NOT_EN;

            } else if ((target == DIGI_ILKN_CHNL_TARGET_CPB) &&
                       (digi_handle->var.sifd_ilkn_1_cfg.ilkn.cpb_intf_en == FALSE)) {

                rc = DIGI_ERR_ILKN_CPB_INTF_NOT_EN;

            } else {
              
                rc = PMC_SUCCESS;
                ilkn_index = 0;
            }

            break;
        case DIGI_SIFD_ILKN2: 
            if ((target == DIGI_ILKN_CHNL_TARGET_ODUK) &&
                (digi_handle->var.sifd_ilkn_2_cfg.ilkn.oduksw_intf_en == FALSE)) {

                rc = DIGI_ERR_ILKN_ODUKSW_INTF_NOT_EN;

            } else if ((target == DIGI_ILKN_CHNL_TARGET_CPB) &&
                       (digi_handle->var.sifd_ilkn_2_cfg.ilkn.cpb_intf_en == FALSE)) {

                rc = DIGI_ERR_ILKN_CPB_INTF_NOT_EN;

            } else {

                rc = PMC_SUCCESS;
                ilkn_index = 1;
            }

            break;
        default:
            /* Should ASSERT here as this shouldn't be possible */
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
            rc = DIGI_ERR_INVALID_ARG;
        }

        if (PMC_SUCCESS == rc)
        { 
            /*
             * Now determine whether or not the ILKN BCW Rx channel value
             * is assigned or not. If it is return an error.
             * It is accessed based on whether it is ILKN1 or 2, and is
             * indice 0-7 (channel/32) where channel is 0-253, and then
             * the individual bit is channel mod 32. If it is 1 then it
             * is assigned
             */

            /* We only need to check for duplicate BCW channel IDs if we're
             * not in 2 byte header mode 0 */
            if(ilkn_data_ptr->ilkn_inst == DIGI_SIFD_ILKN1 &&
               digi_handle->var.sifd_ilkn_1_cfg.ilkn.oduksw_header == DIGI_SIFD_ODUKSW_ILKN_HEADER_2B &&
               digi_handle->var.sifd_ilkn_1_cfg.ilkn.oduksw_2b_header_mode == 0)
            { /* valid case */ }
            else if(ilkn_data_ptr->ilkn_inst == DIGI_SIFD_ILKN2 &&
                    digi_handle->var.sifd_ilkn_2_cfg.ilkn.oduksw_header == DIGI_SIFD_ODUKSW_ILKN_HEADER_2B &&
                    digi_handle->var.sifd_ilkn_2_cfg.ilkn.oduksw_2b_header_mode == 0)
            { /* valid case */ }
            else
            {
                UINT32 value = digi_handle->var.rx_bcw_chnl_id_assigned[ilkn_index][rx_ilkn_bcw_chnl/DIGI_WORD_BIT_SIZE] >> rx_ilkn_bcw_chnl%DIGI_WORD_BIT_SIZE;
                if (value & 0x1)
                {
                    /*
                     * BCW Channel is assigned. This is an error
                     */
                    rc = DIGI_ERR_BCW_CHANNEL_INUSE;
                }

            }
        }

    }

    PMC_RETURN(rc);

} /* digi_ilkn_chnl_cfg_params_validate */

/*******************************************************************************
* digi_ilkn_cpb_chnl_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*
*   This function locates the requested channel and returns a pointer to the
*   requested channel in static memory.
*
* INPUTS:
*   *digi_handle              - pointer to DIGI handle instance
*   digi_sifd_ilkn_inst       - The Interlaken instance in which to retrieve
*                               the channel instance from.
*   channel                   - The DPI channel that is being requested.
*
* OUTPUTS:
*
* RETURNS:
*   A pointer to the requested channel, or NULL if the channel could not be
*   located.
*
* NOTES:
*
*******************************************************************************/
PUBLIC digi_ilkn_chnl_def_t* digi_ilkn_cpb_chnl_handle_get(digi_handle_t *digi_handle, 
                                                           digi_sifd_ilkn_inst_t digi_sifd_ilkn_inst, 
                                                           UINT32 channel)
{
    digi_ilkn_chnl_def_t* ilkn_chnl_def = NULL;

    PMC_ENTRY();

    if (channel >= DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX) {

    } else if (digi_sifd_ilkn_inst == DIGI_SIFD_ILKN1) {

        ilkn_chnl_def = digi_ilkn_1_chnl_get(digi_handle, channel);

    } else if (digi_sifd_ilkn_inst == DIGI_SIFD_ILKN2) {

        ilkn_chnl_def = digi_ilkn_2_chnl_get(digi_handle, channel);
    } 

    PMC_RETURN(ilkn_chnl_def);

} /* digi_ilkn_cpb_chnl_handle_get */

/*******************************************************************************
* digi_ilkn_oduksw_chnl_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*
*   This function locates the requested channel and returns a pointer to the
*   requested channel in static memory.
*
* INPUTS:
*   *digi_handle              - pointer to DIGI handle instance
*   digi_sifd_ilkn_inst       - The Interlaken instance in which to retrieve
*                               the channel instance from.
*   channel                   - The DPI channel that is being requested.
*
* OUTPUTS:
*
* RETURNS:
*   A pointer to the requested channel, or NULL if the channel could not be
*   located.
*
* NOTES:
*
*******************************************************************************/
PUBLIC digi_ilkn_chnl_def_t* digi_ilkn_oduksw_chnl_handle_get(digi_handle_t *digi_handle, 
                                                              digi_sifd_ilkn_inst_t digi_sifd_ilkn_inst, 
                                                              UINT32 channel)
{
    digi_ilkn_chnl_def_t* ilkn_chnl_def = NULL;

    PMC_ENTRY();

    if (channel >= DIGI_SIFD_ILKN_ODUK_DPI_CHNLS_MAX) {

    } else if (digi_sifd_ilkn_inst == DIGI_SIFD_ILKN1) {

        ilkn_chnl_def = digi_ilkn_1_chnl_get(digi_handle, channel + DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX);

    } else if (digi_sifd_ilkn_inst == DIGI_SIFD_ILKN2) {

        ilkn_chnl_def = digi_ilkn_2_chnl_get(digi_handle, channel + DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX);
    } 

    PMC_RETURN(ilkn_chnl_def);
} /* digi_ilkn_oduksw_chnl_handle_get */

/*******************************************************************************
* digi_ilkn_1_chnl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*
*   This function locates the requested channel and returns a pointer to the
*   requested channel in static memory.
*
* INPUTS:
*   *digi_handle              - pointer to DIGI handle instance
*   channel                   - The DPI channel that is being requested.
*
* OUTPUTS:
*
* RETURNS:
*   A pointer to the requested channel, or NULL if the channel could not be
*   located.
*
* NOTES:
*
*******************************************************************************/
PUBLIC digi_ilkn_chnl_def_t* digi_ilkn_1_chnl_get(digi_handle_t *digi_handle, 
                                                  UINT32 channel)
{
    digi_ilkn_chnl_def_t* ilkn_chnl_def = NULL;

    PMC_ENTRY();

    if (channel < DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX) {

        ilkn_chnl_def = &(digi_handle->handle_pool.ilkn_1_chnl[channel]);
    }

    PMC_RETURN(ilkn_chnl_def);
} /* digi_ilkn_1_chnl_get */

/*******************************************************************************
* digi_ilkn_2_chnl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*
*   This function locates the requested channel and returns a pointer to the
*   requested channel in static memory.
*
* INPUTS:
*   *digi_handle              - pointer to DIGI handle instance
*   channel                   - The DPI channel that is being requested.
*
* OUTPUTS:
*
* RETURNS:
*   A pointer to the requested channel, or NULL if the channel could not be
*   located.
*
* NOTES:
*
*******************************************************************************/
PUBLIC digi_ilkn_chnl_def_t* digi_ilkn_2_chnl_get(digi_handle_t *digi_handle, 
                                                  UINT32 channel)
{
    digi_ilkn_chnl_def_t* ilkn_chnl_def = NULL;

    PMC_ENTRY();

    if (channel < DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX) {

        ilkn_chnl_def = &(digi_handle->handle_pool.ilkn_2_chnl[channel]);
    }

    PMC_RETURN(ilkn_chnl_def);
} /* digi_ilkn_2_chnl_get */

/*******************************************************************************
*  sifd_flow_control_calendar_2B_header_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Iterates over the sifd flow control calendar to determine which entries
*   are in use. For each LINK or unused value updates the passed in TX or
*   RX calendar entry. For each entry that is found the actual ILKN channel
*   is then searched in the associated ilkn channels for the oduksw channel id.
*   The channel id is then updated in the associated rx/tx
*   ilkn_fc_ptr map. The ilkn_fc_ptr calendars were originally provided by
*   digi_ilkn_chnl_def_t the client when the ILKN channel flow control
*   calendar was configured.
*
*
*  INPUTS:
*
*    *digi_handle            - pointer to DIGI handle instance
*    ilkn_inst               - indicates interlaken instance 1 or 2
*    is_tx_direction         - TRUE: This is TX direction calendar
*                              FALSE: This is an RX direction calendar
*
*  OUTPUTS:
*    *ilkn_fc_ptr  - Storage for TX or RX side re-engineered flow control
*                       calendar (depend on is_tx_direction value)
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
*******************************************************************************/
PUBLIC void digi_sifd_flow_control_calendar_2B_header_get(digi_handle_t*                digi_handle,
                                                          sifd_ilkn_inst_t              ilkn_inst,
                                                          BOOL8                         is_tx_direction,
                                                          digi_sifd_ilkn_calendar32_t*  ilkn_fc_ptr)
{
    UINT16 fc_index = 0;
    UINT8 channel_id = SIFD_ILKN_NULL_FLOW_CONTROL;
    digi_ilkn_chnl_def_t* ilkn_chnl = NULL;

    sifd_ilkn_calendar_t* flow_control_calendar;
    sifd_ilkn_calendar_t* flow_control_calendar_dummy;

    PMC_ENTRY();

    /* select the right ilkn channel */
    if(SIFD_COPI_ILKN1 == ilkn_inst)
    {
        ilkn_chnl = digi_handle->handle_pool.ilkn_1_chnl;
    }
    else
    {
        ilkn_chnl = digi_handle->handle_pool.ilkn_2_chnl;
    }

    /* select right flow direction */
    if (TRUE == is_tx_direction)
    {
        sifd_flow_control_calendar_array_get(digi_handle->sifd_handle,
                                             ilkn_inst,
                                             &flow_control_calendar,
                                             &flow_control_calendar_dummy);
    }
    else
    {
        sifd_flow_control_calendar_array_get(digi_handle->sifd_handle,
                                             ilkn_inst,
                                             &flow_control_calendar_dummy,
                                             &flow_control_calendar);
    }

    for (fc_index = 0; fc_index < SIFD_ILKN_MAX_CAL_ENTRIES; fc_index++)
    {
        /* tx/rx 2B header */
        channel_id = flow_control_calendar->entry[fc_index];

        if(SIFD_ILKN_NULL_FLOW_CONTROL == channel_id)
        {
            ilkn_fc_ptr->entry[fc_index] = SIFD_ILKN_NULL_FLOW_CONTROL_2B;
        }
        else if(SIFD_ILKN_LINK_FLOW_CONTROL == channel_id)
        {
            ilkn_fc_ptr->entry[fc_index] = SIFD_ILKN_LINK_FLOW_CONTROL_2B;
        }
        else if(DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX <= channel_id &&
                DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX > channel_id)
        {
            if ((ilkn_chnl[channel_id].switch_data.oduk_data.channel + DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX) == channel_id)
            {
                if (TRUE == is_tx_direction)
                    ilkn_fc_ptr->entry[fc_index] = (UINT32)ilkn_chnl[channel_id].tx_ilkn_info.ilkn_2b_header;
                else
                    ilkn_fc_ptr->entry[fc_index] = (UINT32)ilkn_chnl[channel_id].rx_ilkn_info.ilkn_2b_header;
            }
            else
            {
                PMC_LOG(PMC_LOG_SEV_HIGH, DIGI_ERR_UNKNOWN, 0, 0, "error in ilkn context : mismatch between channel_id and ILKN context");
            }
        }
        else
        {
            PMC_LOG(PMC_LOG_SEV_HIGH, DIGI_ERR_UNKNOWN, 0, 0, "error in ilkn context : invalid channel_id for flow control entry");
        }
    }

    PMC_RETURN();
} /* sifd_flow_control_calendar_2B_header_get */

/*******************************************************************************
* digi_ilkn_2B_header_to_dpi_map
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Only called if in 2B Header Mode 0, this API maps a calendar of 2 byte
*   header values to a calendar or DPI values.
*   
*  
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance
*   ilkn_inst               - indicates interlaken instance 1 or 2
*   is_tx_direction         - TRUE: This is TX direction calendar
*                             FALSE: This is an RX direction calendar
*   *fc_from_top            - pointer to the calendar passed in from the
*                             top level containing 2 byte header values


* OUTPUTS:
*   *fc_for_context         - pointer to the calendar in the DIGI context
*                             whick gets written with DPI channels
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ilkn_2B_header_to_dpi_map(digi_handle_t *digi_handle, 
                                                sifd_ilkn_inst_t ilkn_inst,
                                                BOOL8 is_tx_direction,
                                                digi_sifd_ilkn_calendar32_t *fc_from_top, 
                                                sifd_ilkn_calendar_t *fc_for_context)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i, j;
    UINT32 header_2B;
    digi_ilkn_chnl_def_t* ilkn_chnl = NULL;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(fc_from_top != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(fc_for_context != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    if(SIFD_COPI_ILKN1 == ilkn_inst)
    {
        ilkn_chnl = digi_handle->handle_pool.ilkn_1_chnl;
    }
    else 
    {
        ilkn_chnl = digi_handle->handle_pool.ilkn_2_chnl;
    }

    for(i=0;i<SIFD_ILKN_MAX_CAL_ENTRIES; i++)
    {
        header_2B = fc_from_top->entry[i];
        if(header_2B == SIFD_ILKN_NULL_FLOW_CONTROL_2B)
        {
            fc_for_context->entry[i] =  SIFD_ILKN_NULL_FLOW_CONTROL;
        }
        else if(header_2B == SIFD_ILKN_LINK_FLOW_CONTROL_2B)
        {
            fc_for_context->entry[i] =  SIFD_ILKN_LINK_FLOW_CONTROL;
        }
        else
        {
            for(j=DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;j<DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX;j++)
            {
                if(is_tx_direction &&
                        ilkn_chnl[j].tx_ilkn_info.ilkn_2b_header == header_2B)
                {
                    fc_for_context->entry[i] =  ilkn_chnl[j].switch_data.oduk_data.channel + DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
                    break;
                }
                else if(!is_tx_direction &&
                        ilkn_chnl[j].rx_ilkn_info.ilkn_2b_header == header_2B)
                {
                    fc_for_context->entry[i] =  ilkn_chnl[j].switch_data.oduk_data.channel + DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
                    break;
                }

            }
        }

    }

    PMC_RETURN(result);
} /* digi_ilkn_2B_header_to_dpi_map */

/*******************************************************************************
* digi_ilkn_bcw_to_dpi_map
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function converts flow control calendar data that is specified
*   relative to BCW channel ID to internal resource DPI channel ID
*
* INPUTS:
*   *digi_handle              - pointer to DIGI handle instance
*   fc_cal_length             - flow control calendar lenght 
*   *bcw_to_dpi_map_ptr       - pointer to data structure containing BCW
*                               Channel ID to DPI channel ID mapping
*   *ilkn_fc_ptr              - pointer to flow control data containing
*                               BCW Channel ID
*
* OUTPUTS:
*  *fc_ptr                    - pointer to generate flow control calendar
*                               data relative to DPI channel ID
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
* 
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ilkn_bcw_to_dpi_map(digi_handle_t *digi_handle, 
                                          UINT32 fc_cal_length,
                                          UINT8 *bcw_to_dpi_map_ptr, 
                                          digi_sifd_ilkn_calendar32_t *ilkn_fc_ptr, 
                                          sifd_ilkn_calendar_t *fc_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(ilkn_fc_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(fc_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    /* Note about casting to UINT8. On the applib level the flow
     * control calendar must be passed into digi_ilkn_XXX_chnl_prov
     * with 32 bit entries to accomodate SIFD 2 byte header mode 0.
     * This mapping function will only be called when not in this
     * mode so the cast is safe */

    for (i = 0; i < SIFD_ILKN_MAX_CAL_ENTRIES && PMC_SUCCESS == result; i++) 
    {
        if ( i < fc_cal_length)
        {
            if ((ilkn_fc_ptr->entry[i] == SIFD_ILKN_NULL_CAL_ENTRY) || 
                (ilkn_fc_ptr->entry[i] == SIFD_ILKN_LINK_FLOW_CONTROL))
            {    
                fc_ptr->entry[i] = (UINT8)ilkn_fc_ptr->entry[i];
            }
            else
            {
                if (DIGI_SIFD_BCW_CHANNEL_LINK == bcw_to_dpi_map_ptr[(UINT8)ilkn_fc_ptr->entry[i]])
                {
                    result = DIGI_ERR_TX_BCW_CHANNEL_ID_AMBIGUOUS;
                }
                else
                {
                    fc_ptr->entry[i] = bcw_to_dpi_map_ptr[(UINT8)ilkn_fc_ptr->entry[i]];
                }
            }
        }
        else 
        {
            fc_ptr->entry[i] = (UINT8)SIFD_ILKN_NULL_CAL_ENTRY;
        }
    }

    PMC_RETURN(result);
} /* digi_ilkn_bcw_to_dpi_map */

/*******************************************************************************
* digi_sifd_fc_calendar_save
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function takes in RX and TX flow control calendars, converts the
*   entries from BCW to internal resource DPI ID, and then saves the calendars
*   in DIGI later context.
*
* INPUTS:
*   *digi_handle              - pointer to DIGI handle instance
*   sifd_path                 - The specific ILKN instance.\n
*                                SIFD_COPI_ILKN1 Interlaken 1 Instance\n
*                                SIFD_COPI_ILKN2 Interlaken 2 Instance
*   *ilkn_tx_fc_ptr           - pointer to tx flow control data containing
*                               BCW Channel ID. 
*                               Ignored when pointer NULL.
*   *ilkn_rx_fc_ptr           - pointer to rx flow control data containing
*                               BCW Channel ID
*                               Ignored when pointer NULL.
*
*
* OUTPUTS:
*   None
* 
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
* 
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_fc_calendar_save(digi_handle_t *digi_handle, 
                                            sifd_ilkn_inst_t sifd_path,
                                            digi_sifd_ilkn_calendar32_t *ilkn_tx_fc_ptr, 
                                            digi_sifd_ilkn_calendar32_t *ilkn_rx_fc_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 fc_cal_length = 0;
    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(sifd_path <= SIFD_COPI_ILKN2, DIGI_ERR_INVALID_ARG, 0, 0);

    /* suppress all accesses to flow control data if flow control mode not enabled */
    if(sifd_path == SIFD_COPI_ILKN1)
    {
        if(digi_handle->var.sifd_ilkn_1_cfg.ilkn.fc_mode == DIGI_SIFD_FC_MODE_NONE)
            PMC_RETURN(result);
        fc_cal_length = digi_handle->var.sifd_ilkn_1_cfg.ilkn.fc_cal_length;
    }   
    if(sifd_path == SIFD_COPI_ILKN2)
    {
        if(digi_handle->var.sifd_ilkn_2_cfg.ilkn.fc_mode == DIGI_SIFD_FC_MODE_NONE)
            PMC_RETURN(result);
        fc_cal_length = digi_handle->var.sifd_ilkn_2_cfg.ilkn.fc_cal_length;
    }   

    PMC_ASSERT(ilkn_tx_fc_ptr != NULL || ilkn_rx_fc_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* convert BCW calendar data to DPI channel calendar data
     * and save in DIGI context */
    if(sifd_path == SIFD_COPI_ILKN1)
    {
        if(digi_handle->var.sifd_ilkn_1_cfg.ilkn.oduksw_header == DIGI_SIFD_ODUKSW_ILKN_HEADER_2B &&
           digi_handle->var.sifd_ilkn_1_cfg.ilkn.oduksw_2b_header_mode == 0 &&
           digi_handle->var.sifd_ilkn_1_cfg.ilkn.oduksw_intf_en == 1)
        {
            /* The CPB interface is not enabled because of a check in digi_device_init_params_validate */
            if (result == PMC_SUCCESS &&
                NULL != ilkn_tx_fc_ptr) {           
                result = digi_ilkn_2B_header_to_dpi_map(digi_handle,
                                                        sifd_path,
                                                        TRUE,
                                                        ilkn_tx_fc_ptr,
                                                        &digi_handle->var.ilkn1_tx_fc);
            }
            if (result == PMC_SUCCESS &&
                NULL != ilkn_rx_fc_ptr) {           
                result = digi_ilkn_2B_header_to_dpi_map(digi_handle,
                                                        sifd_path,
                                                        FALSE,
                                                        ilkn_rx_fc_ptr,
                                                        &digi_handle->var.ilkn1_rx_fc);
            }


        }
        else /* Not using ODUkSW 2B header mode 0 */
        {
            if (result == PMC_SUCCESS &&
                NULL != ilkn_tx_fc_ptr) {
                result = digi_ilkn_bcw_to_dpi_map(digi_handle, 
                                                  fc_cal_length,
                                                  digi_handle->var.tx_ilkn_1_bcw_dpi_map, 
                                                  ilkn_tx_fc_ptr, 
                                                  &digi_handle->var.ilkn1_tx_fc);
            }
            if (result == PMC_SUCCESS &&
                NULL != ilkn_rx_fc_ptr) {
                result = digi_ilkn_bcw_to_dpi_map(digi_handle, 
                                                  fc_cal_length,
                                                  digi_handle->var.rx_ilkn_1_bcw_dpi_map, 
                                                  ilkn_rx_fc_ptr, 
                                                  &digi_handle->var.ilkn1_rx_fc);
            }
        }
    } 
    else /* sifd_path == SIFD_COPI_ILKN2 */
    {
        if(digi_handle->var.sifd_ilkn_2_cfg.ilkn.oduksw_header == DIGI_SIFD_ODUKSW_ILKN_HEADER_2B &&
           digi_handle->var.sifd_ilkn_2_cfg.ilkn.oduksw_2b_header_mode == 0 &&
           digi_handle->var.sifd_ilkn_2_cfg.ilkn.oduksw_intf_en == 1)
        {
            /* The CPB interface is not enabled because of a check in digi_device_init_params_validate */
            if (result == PMC_SUCCESS &&
                NULL != ilkn_tx_fc_ptr) {           
                result = digi_ilkn_2B_header_to_dpi_map(digi_handle,
                                                        sifd_path,
                                                        TRUE,
                                                        ilkn_tx_fc_ptr,
                                                        &digi_handle->var.ilkn2_tx_fc);
            }
            if (result == PMC_SUCCESS &&
                NULL != ilkn_rx_fc_ptr) {           
                result = digi_ilkn_2B_header_to_dpi_map(digi_handle,
                                                        sifd_path,
                                                        FALSE,
                                                        ilkn_rx_fc_ptr,
                                                        &digi_handle->var.ilkn2_rx_fc);
            }

        }
        else /* Not using ODUkSW 2B header mode 0 */
        {

            if (result == PMC_SUCCESS &&
                NULL != ilkn_tx_fc_ptr)
            {
                result = digi_ilkn_bcw_to_dpi_map(digi_handle, 
                                                  fc_cal_length,
                                                  digi_handle->var.tx_ilkn_2_bcw_dpi_map, 
                                                  ilkn_tx_fc_ptr, 
                                                  &digi_handle->var.ilkn2_tx_fc);
            }
            if (result == PMC_SUCCESS &&
                NULL != ilkn_rx_fc_ptr) 
            {
                result = digi_ilkn_bcw_to_dpi_map(digi_handle, 
                                                  fc_cal_length,
                                                  digi_handle->var.rx_ilkn_2_bcw_dpi_map, 
                                                  ilkn_rx_fc_ptr, 
                                                  &digi_handle->var.ilkn2_rx_fc);
            }

        }
    }

    PMC_RETURN(result);
} /* digi_sifd_fc_calendar_save */


/*******************************************************************************
* digi_sifd_fc_calendar_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function converts flow control calendar data that is specified
*   relative to BCW channel ID to internal resource DPI channel ID and applies
*   the flow control calendar configuration to the device registers.
*
* INPUTS:
*   *digi_handle              - pointer to DIGI handle instance
*   ilkn_inst                 - The specific ILKN instance.\n
*                                SIFD_COPI_ILKN1 Interlaken 1 Instance\n
*                                SIFD_COPI_ILKN2 Interlaken 2 Instance
*   cpb_egress_port           - The egress port number of the CPB or
*                               ODUk switch
*   is_tx_data_dir            - TRUE: Data direction in TX (SIFD to fabric)
*                             - FALSE: Data direction is RX.
*   is_oduksw                 - TRUE: The calendar is connected to the ODUk
*                                     switch
*                               FALSE: The calendar is connected to the datapath
*                                      CPB
*   deactv_chnl              - A channel that is about to be deactivated. It\n
*                              is valid when it not equal to SIFD_ILKN_NULL_CAL_ENTRY.\n
*                              The FC value of this channel will be ignored \n
*                              even if it is in operational state
* 
* OUTPUTS:
*   None
* 
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_fc_calendar_update(digi_handle_t *digi_handle, 
                                              sifd_ilkn_inst_t ilkn_inst,
                                              UINT32 cpb_egress_port,
                                              BOOL8 is_tx_data_dir,
                                              BOOL8 is_oduksw,
                                              UINT32 deactv_chnl)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i, cpb_egress_channel;
    sifd_ilkn_calendar_t temp_fc_cal;
    sifd_ilkn_calendar_t * context_fc_cal_ptr;
    cpb_handle_t * oduksw_handle;
    util_schd_chnl_state_t channel_state = LAST_UTIL_SCHD_CHNL;
    UINT32 cpb_ingress_port, cpb_ingress_channel;
    BOOL8  is_deactv_chnl = FALSE;
    UINT32 fc_cal_length = 0;
    PMC_ENTRY();

    oduksw_handle = coreotn_oduksw_handle_get(digi_handle->coreotn_handle);

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(ilkn_inst <= SIFD_COPI_ILKN2, DIGI_ERR_INVALID_ARG, 0, 0);

    /* suppress all accesses to flow control data if flow control mode not enabled */
    if(ilkn_inst == SIFD_COPI_ILKN1)
    {
        if(digi_handle->var.sifd_ilkn_1_cfg.ilkn.fc_mode == DIGI_SIFD_FC_MODE_NONE)
            PMC_RETURN(result);
        fc_cal_length = digi_handle->var.sifd_ilkn_1_cfg.ilkn.fc_cal_length;
    }   
    if(ilkn_inst == SIFD_COPI_ILKN2)
    {
        if(digi_handle->var.sifd_ilkn_2_cfg.ilkn.fc_mode == DIGI_SIFD_FC_MODE_NONE)
            PMC_RETURN(result);
        fc_cal_length = digi_handle->var.sifd_ilkn_2_cfg.ilkn.fc_cal_length;
    }   

    /* Loop through entries, only copying entries that have related
     * operational CPB channels */
    if(is_tx_data_dir)
        if(ilkn_inst == SIFD_COPI_ILKN1)
            context_fc_cal_ptr = &digi_handle->var.ilkn1_tx_fc;
        else
            context_fc_cal_ptr = &digi_handle->var.ilkn2_tx_fc;
    else
        if(ilkn_inst == SIFD_COPI_ILKN1)
            context_fc_cal_ptr = &digi_handle->var.ilkn1_rx_fc;
        else    
            context_fc_cal_ptr = &digi_handle->var.ilkn2_rx_fc;

    for(i=0;i<SIFD_ILKN_MAX_CAL_ENTRIES;i++)
    {
        if (i >= fc_cal_length)
        {
            temp_fc_cal.entry[i] = SIFD_ILKN_NULL_CAL_ENTRY;            
        } else 
        {
            if(context_fc_cal_ptr->entry[i] != SIFD_ILKN_NULL_CAL_ENTRY &&
               context_fc_cal_ptr->entry[i] != SIFD_ILKN_LINK_FLOW_CONTROL)
            {
                if(is_tx_data_dir)
                {
                    /* 
                       entry[i] data is the contents of flow control calendar 
                       0 to 149 is DCPB DPI channel 0 to 149
                       150 to 253 is the OCPB DPI channel 0 to 103
                       254 is Link Level flow control
                       255 is NULL flow control entry.

                    */
                    cpb_egress_channel = context_fc_cal_ptr->entry[i];
                
                    if(cpb_egress_channel >= DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX)
                    {    
                        cpb_egress_channel -= DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
                        channel_state = cpb_channel_state_get(oduksw_handle,cpb_egress_port,cpb_egress_channel,TRUE);
                    }
                    else
                    {
                        channel_state = cpb_channel_state_get(digi_handle->dcpb_handle,cpb_egress_port,cpb_egress_channel,TRUE);
                    }
                }
                else /* rx data direction */
                {
                    cpb_ingress_port = ilkn_inst == SIFD_COPI_ILKN1 ? 0 : 1;
                    cpb_ingress_channel = context_fc_cal_ptr->entry[i];
                
                    if(cpb_ingress_channel >= DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX)
                    {    
                        cpb_ingress_channel -= DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
                        channel_state = cpb_channel_state_get(oduksw_handle,cpb_ingress_port,cpb_ingress_channel,FALSE);
                    }
                    else
                    {
                        channel_state = cpb_channel_state_get(digi_handle->dcpb_handle,cpb_ingress_port,cpb_ingress_channel,FALSE);
                    }

                }
                if(is_oduksw==TRUE)
                {
                    /* For ODUKSW the data range in flow control calandar is 104 to 253 mapping to OCPB DPI channels 0 to 103 */
                    is_deactv_chnl = (deactv_chnl != SIFD_ILKN_NULL_CAL_ENTRY && context_fc_cal_ptr->entry[i] == (deactv_chnl + DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX));
                }
                else
                {
                    is_deactv_chnl = (deactv_chnl != SIFD_ILKN_NULL_CAL_ENTRY && context_fc_cal_ptr->entry[i] == deactv_chnl);
                }
                if (is_deactv_chnl == FALSE && channel_state == UTIL_SCHD_CHNL_OPERATIONAL)
                {
                    temp_fc_cal.entry[i] = context_fc_cal_ptr->entry[i];
                }            
                else
                {
                    temp_fc_cal.entry[i] = SIFD_ILKN_NULL_CAL_ENTRY;
                }
            }
            else
            {
                temp_fc_cal.entry[i] = context_fc_cal_ptr->entry[i];
            }
        }
    }
    PMC_ATOMIC_YIELD(digi_handle,0);
    /* apply flow control calendar update */
    if (is_tx_data_dir) {
        result = sifd_ilkn_fc_calendar_update(digi_handle->sifd_handle,
                                              ilkn_inst,
                                              &temp_fc_cal,
                                              NULL);
    }
    else
    {
        result = sifd_ilkn_fc_calendar_update(digi_handle->sifd_handle,
                                              ilkn_inst,
                                              NULL,
                                              &temp_fc_cal);

    }

    PMC_RETURN(result);
} /* digi_sifd_fc_calendar_update */

/*******************************************************************************
* digi_sifd_fc_calendar_chnl_entries_remove
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function removes FC entries associated to given channel from ILKN context.
*   FC calendar in the device is also updated.
*
* INPUTS:
*   *digi_handle              - pointer to DIGI handle instance
*   chnl_data_ptr             - pointer to ILKN channel context.
* 
* OUTPUTS:
*   None
* 
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_fc_calendar_chnl_entries_remove(digi_handle_t         *digi_handle,
                                                           digi_ilkn_chnl_def_t  *chnl_data_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    sifd_ilkn_calendar_t * rx_context_fc_cal_ptr;
    sifd_ilkn_calendar_t * tx_context_fc_cal_ptr;
    digi_sifd_ilkn_inst_t sifd_ilkn_inst;
    UINT32 deactv_chnl;
    BOOL8 is_oduksw = FALSE;
    UINT32 port_type;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* retrieve ILKN instance associated to channel */
    if (result==PMC_SUCCESS)
    {
        result =  digi_ilkn_inst_get(chnl_data_ptr, &sifd_ilkn_inst);
    }

    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* exit if flow control is not enabled in ILKN interface  */
    if(sifd_ilkn_inst == DIGI_SIFD_ILKN1)
    {
        if(digi_handle->var.sifd_ilkn_1_cfg.ilkn.fc_mode == DIGI_SIFD_FC_MODE_NONE)
            PMC_RETURN(result);
    }   
    if(sifd_ilkn_inst == DIGI_SIFD_ILKN2)
    {
        if(digi_handle->var.sifd_ilkn_2_cfg.ilkn.fc_mode == DIGI_SIFD_FC_MODE_NONE)
            PMC_RETURN(result);
    }   
    
    /* retrieve flow control calendar pointers */
    if(sifd_ilkn_inst == DIGI_SIFD_ILKN1)
    {
        rx_context_fc_cal_ptr = &digi_handle->var.ilkn1_rx_fc;
        tx_context_fc_cal_ptr = &digi_handle->var.ilkn1_tx_fc;
    }
    else
    {
        rx_context_fc_cal_ptr = &digi_handle->var.ilkn2_rx_fc;
        tx_context_fc_cal_ptr = &digi_handle->var.ilkn2_tx_fc;
    }

    /* determine CPB or ODUK channel associated to channel */
    if (chnl_data_ptr->switch_data.cpb_data.port_type != UTIL_GLOBAL_CPB_PORT_NOT_USED)
    {
        deactv_chnl = chnl_data_ptr->switch_data.cpb_data.channel;   
        port_type = (UINT32)chnl_data_ptr->switch_data.cpb_data.port_type;
    }
    else if (chnl_data_ptr->switch_data.oduk_data.port_type != UTIL_GLOBAL_ODUK_PORT_NOT_USED)
    {
        deactv_chnl = chnl_data_ptr->switch_data.oduk_data.channel;
        is_oduksw = TRUE;  
        port_type = (UINT32)chnl_data_ptr->switch_data.oduk_data.port_type;
    }
    else
    {
        PMC_RETURN(PMC_SUCCESS);
    }

    /* overwrite  ILKN channel entries in context */
    if (chnl_data_ptr->switch_data.cpb_data.port_type != UTIL_GLOBAL_CPB_PORT_NOT_USED)
    {
        for(i=0;i<SIFD_ILKN_MAX_CAL_ENTRIES;i++)
        {
            if (tx_context_fc_cal_ptr->entry[i] == deactv_chnl)
            {
                tx_context_fc_cal_ptr->entry[i] = SIFD_ILKN_NULL_CAL_ENTRY;
            }
            if (rx_context_fc_cal_ptr->entry[i] == deactv_chnl)
            {
                rx_context_fc_cal_ptr->entry[i] = SIFD_ILKN_NULL_CAL_ENTRY;
            }
        }
    }
    else if (chnl_data_ptr->switch_data.oduk_data.port_type != UTIL_GLOBAL_ODUK_PORT_NOT_USED)
    {
        for(i=0;i<SIFD_ILKN_MAX_CAL_ENTRIES;i++)
        {           
            if (tx_context_fc_cal_ptr->entry[i] == (deactv_chnl + DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX))
            {
                tx_context_fc_cal_ptr->entry[i] = SIFD_ILKN_NULL_CAL_ENTRY;
            }
            if (rx_context_fc_cal_ptr->entry[i] == (deactv_chnl + DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX))
            {
                rx_context_fc_cal_ptr->entry[i] = SIFD_ILKN_NULL_CAL_ENTRY;
            }
        }
    }

    /* update device ILKN TX flow control calendar */
    if (PMC_SUCCESS == result)
    {
        result = digi_sifd_fc_calendar_update(digi_handle, 
                                              (sifd_ilkn_inst_t)sifd_ilkn_inst, 
                                              port_type,
                                              TRUE, /* TRUE means TX data direction */
                                              is_oduksw, /* TRUE means it's for ODUk switch, not CPB */
                                              deactv_chnl);
    }

    /* update device ILKN RX flow control calendar */
    if (PMC_SUCCESS == result)
    {
        result = digi_sifd_fc_calendar_update(digi_handle, 
                                              (sifd_ilkn_inst_t)sifd_ilkn_inst, 
                                              port_type,
                                              FALSE, /* TRUE means TX data direction */
                                              is_oduksw, /* TRUE means it's for ODUk switch, not CPB */
                                              deactv_chnl);
    }
    PMC_RETURN(result);
} /* digi_sifd_fc_calendar_chnl_entries_remove */

/*******************************************************************************
* digi_sifd_enet_pmon_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns enet handle and link id associated to Sifd ENET PMON channel.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   *chnl_handle    - pointer to sifd ilkn channel handle
*   
* OUTPUTS:
*   **enet_pptr            - pointer to a pointer to ENET handle (LINE or SYS)
*   *link_ptr              - pointer to link number
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_enet_pmon_handle_get(digi_handle_t          *digi_handle,
                                                digi_ilkn_chnl_t     *chnl_handle,
                                                enet_handle_t       **enet_pptr,
                                                UINT32               *link_ptr)
{
    
    PMC_ERROR                result = DIGI_ERR_INVALID_ARG;
    digi_enet_client_chnl_t *digi_enet_handle_ptr = NULL;
    
    PMC_ENTRY();

    digi_enet_handle_ptr = digi_enet_enhanced_pmon_ptr_get((util_global_switch_data_def_t*)chnl_handle);

    if (NULL != digi_enet_handle_ptr)
    {
        result = digi_enet_validate_parms(digi_handle,
                                          digi_enet_handle_ptr,
                                          enet_pptr,
                                          link_ptr);
    }
    if (PMC_SUCCESS != result)
    {
        *enet_pptr = NULL;
        result = PMC_SUCCESS;
    }

    PMC_RETURN(result);
}/* digi_sifd_enet_pmon_handle_get */

/*******************************************************************************
* digi_copy_struct_ilkn_1_cfg
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Copy data from one struct to another
*                                                                               
*                                                                               
* INPUTS:                                                                       
* sifd_ilkn_1_cfg           - Destination struct
* digi_sifd_ilkn_1_cfg_ptr  - Source struct
*                                                                               
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC PMC_ERROR digi_copy_struct_ilkn_1_cfg(sifd_ilkn_1_cfg_t       *sifd_ilkn_1_cfg,
                                             digi_sifd_ilkn_1_cfg_t  *digi_sifd_ilkn_1_cfg_ptr)

{
    UINT32 i;
    PMC_ERROR rc;
    PMC_ENTRY();


    rc = digi_sifd_burst_min_conv(digi_sifd_ilkn_1_cfg_ptr->ilkn.burst_min, &sifd_ilkn_1_cfg->ilkn.burst_min);

    if (PMC_SUCCESS == rc)
    {
        sifd_ilkn_1_cfg->ilkn.rate = (sifd_ilkn_serdes_rate_t)digi_sifd_ilkn_1_cfg_ptr->ilkn.rate;
        sifd_ilkn_1_cfg->ilkn.lane_mask = digi_sifd_ilkn_1_cfg_ptr->ilkn.lane_mask;
        
        for(i=0;i<DIGI_ILKN_LANES_MAX;i++)
        {
            sifd_ilkn_1_cfg->ilkn.lane_tx_order.lane[i] = digi_sifd_ilkn_1_cfg_ptr->ilkn.lane_tx_order.lane[i];
            sifd_ilkn_1_cfg->ilkn.lane_rx_order.lane[i] = digi_sifd_ilkn_1_cfg_ptr->ilkn.lane_rx_order.lane[i];
        }
        
        sifd_ilkn_1_cfg->ilkn.burst = (sifd_oc_burst_t)digi_sifd_ilkn_1_cfg_ptr->ilkn.burst;
        sifd_ilkn_1_cfg->ilkn.burst_short = (sifd_burst_short_t)digi_sifd_ilkn_1_cfg_ptr->ilkn.burst_short;
        sifd_ilkn_1_cfg->ilkn.metaframe = (sifd_ilkn_mf_t)digi_sifd_ilkn_1_cfg_ptr->ilkn.metaframe;
        sifd_ilkn_1_cfg->ilkn.tx_rate_limit.rate_limit = digi_sifd_ilkn_1_cfg_ptr->ilkn.tx_rate_limit.rate_limit;
        sifd_ilkn_1_cfg->ilkn.tx_rate_limit.burst_limit = digi_sifd_ilkn_1_cfg_ptr->ilkn.tx_rate_limit.burst_limit;
        sifd_ilkn_1_cfg->ilkn.tx_rate_limit.byte_granularity = digi_sifd_ilkn_1_cfg_ptr->ilkn.tx_rate_limit.byte_granularity;
        sifd_ilkn_1_cfg->ilkn.oduksw_intf_en = digi_sifd_ilkn_1_cfg_ptr->ilkn.oduksw_intf_en;
        sifd_ilkn_1_cfg->ilkn.oduksw_tx_xfer_mode = (sifd_mode_t)digi_sifd_ilkn_1_cfg_ptr->ilkn.oduksw_tx_xfer_mode;
        sifd_ilkn_1_cfg->ilkn.oduksw_header = (sifd_oduk_hdr_insert_t)digi_sifd_ilkn_1_cfg_ptr->ilkn.oduksw_header;
        sifd_ilkn_1_cfg->ilkn.oduksw_2b_header_mode = digi_sifd_ilkn_1_cfg_ptr->ilkn.oduksw_2b_header_mode;
        sifd_ilkn_1_cfg->ilkn.cpb_intf_en = digi_sifd_ilkn_1_cfg_ptr->ilkn.cpb_intf_en;
        sifd_ilkn_1_cfg->ilkn.cpb_tx_xfer_mode = (sifd_mode_t)digi_sifd_ilkn_1_cfg_ptr->ilkn.cpb_tx_xfer_mode;
        sifd_ilkn_1_cfg->ilkn.rx_xfer_mode = (sifd_mode_t)digi_sifd_ilkn_1_cfg_ptr->ilkn.rx_xfer_mode;
        sifd_ilkn_1_cfg->ilkn.fc_mode = (sifd_fc_mode_t)digi_sifd_ilkn_1_cfg_ptr->ilkn.fc_mode;
        sifd_ilkn_1_cfg->ilkn.fc_cal_length = digi_sifd_ilkn_1_cfg_ptr->ilkn.fc_cal_length;
        
        sifd_ilkn_1_cfg->lane.blk_to_E1_enable_mask = digi_sifd_ilkn_1_cfg_ptr->lane.blk_to_E1_enable_mask;
        sifd_ilkn_1_cfg->lane.blk_to_N1_enable_mask = digi_sifd_ilkn_1_cfg_ptr->lane.blk_to_N1_enable_mask;
        sifd_ilkn_1_cfg->lane.blk_to_N2_enable_mask = digi_sifd_ilkn_1_cfg_ptr->lane.blk_to_N2_enable_mask;
    }

    PMC_RETURN(rc);
}; /* digi_copy_struct_ilkn_1_cfg */

/*******************************************************************************
* digi_copy_struct_ilkn_2_cfg
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Copy data from one struct to another
*                                                                               
*                                                                               
* INPUTS:                                                                       
* sifd_ilkn_2_cfg           - Destination struct
* digi_sifd_ilkn_2_cfg_ptr  - Source struct
*                                                                               
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC PMC_ERROR digi_copy_struct_ilkn_2_cfg(sifd_ilkn_2_cfg_t       *sifd_ilkn_2_cfg,
                                             digi_sifd_ilkn_2_cfg_t  *digi_sifd_ilkn_2_cfg_ptr)

{
    UINT32 i;
    PMC_ERROR rc;

    PMC_ENTRY();


    rc = digi_sifd_burst_min_conv(digi_sifd_ilkn_2_cfg_ptr->ilkn.burst_min, &sifd_ilkn_2_cfg->ilkn.burst_min);

    if (PMC_SUCCESS == rc)
    {
        sifd_ilkn_2_cfg->ilkn.rate = (sifd_ilkn_serdes_rate_t)digi_sifd_ilkn_2_cfg_ptr->ilkn.rate;
        sifd_ilkn_2_cfg->ilkn.lane_mask = digi_sifd_ilkn_2_cfg_ptr->ilkn.lane_mask;
        
        for(i=0;i<DIGI_ILKN_LANES_MAX;i++)
        {
            sifd_ilkn_2_cfg->ilkn.lane_tx_order.lane[i] = digi_sifd_ilkn_2_cfg_ptr->ilkn.lane_tx_order.lane[i];
            sifd_ilkn_2_cfg->ilkn.lane_rx_order.lane[i] = digi_sifd_ilkn_2_cfg_ptr->ilkn.lane_rx_order.lane[i];
        }
        
        sifd_ilkn_2_cfg->ilkn.burst = (sifd_oc_burst_t)digi_sifd_ilkn_2_cfg_ptr->ilkn.burst;
        sifd_ilkn_2_cfg->ilkn.burst_short = (sifd_burst_short_t)digi_sifd_ilkn_2_cfg_ptr->ilkn.burst_short;
        sifd_ilkn_2_cfg->ilkn.metaframe = (sifd_ilkn_mf_t)digi_sifd_ilkn_2_cfg_ptr->ilkn.metaframe;
        sifd_ilkn_2_cfg->ilkn.tx_rate_limit.rate_limit = digi_sifd_ilkn_2_cfg_ptr->ilkn.tx_rate_limit.rate_limit;
        sifd_ilkn_2_cfg->ilkn.tx_rate_limit.burst_limit = digi_sifd_ilkn_2_cfg_ptr->ilkn.tx_rate_limit.burst_limit;
        sifd_ilkn_2_cfg->ilkn.tx_rate_limit.byte_granularity = digi_sifd_ilkn_2_cfg_ptr->ilkn.tx_rate_limit.byte_granularity;
        sifd_ilkn_2_cfg->ilkn.oduksw_intf_en = digi_sifd_ilkn_2_cfg_ptr->ilkn.oduksw_intf_en;
        sifd_ilkn_2_cfg->ilkn.oduksw_tx_xfer_mode = (sifd_mode_t)digi_sifd_ilkn_2_cfg_ptr->ilkn.oduksw_tx_xfer_mode;
        sifd_ilkn_2_cfg->ilkn.oduksw_header = (sifd_oduk_hdr_insert_t)digi_sifd_ilkn_2_cfg_ptr->ilkn.oduksw_header;
        sifd_ilkn_2_cfg->ilkn.oduksw_2b_header_mode = digi_sifd_ilkn_2_cfg_ptr->ilkn.oduksw_2b_header_mode;
        sifd_ilkn_2_cfg->ilkn.cpb_intf_en = digi_sifd_ilkn_2_cfg_ptr->ilkn.cpb_intf_en;
        sifd_ilkn_2_cfg->ilkn.cpb_tx_xfer_mode = (sifd_mode_t)digi_sifd_ilkn_2_cfg_ptr->ilkn.cpb_tx_xfer_mode;
        sifd_ilkn_2_cfg->ilkn.rx_xfer_mode = (sifd_mode_t)digi_sifd_ilkn_2_cfg_ptr->ilkn.rx_xfer_mode;
        sifd_ilkn_2_cfg->ilkn.fc_mode = (sifd_fc_mode_t)digi_sifd_ilkn_2_cfg_ptr->ilkn.fc_mode;
        sifd_ilkn_2_cfg->ilkn.fc_cal_length = digi_sifd_ilkn_2_cfg_ptr->ilkn.fc_cal_length;

        sifd_ilkn_2_cfg->lane.blk_to_E1_enable_mask = digi_sifd_ilkn_2_cfg_ptr->lane.blk_to_E1_enable_mask;
        sifd_ilkn_2_cfg->lane.blk_to_N1_enable_mask = digi_sifd_ilkn_2_cfg_ptr->lane.blk_to_N1_enable_mask;
        sifd_ilkn_2_cfg->lane.blk_to_N2_enable_mask = digi_sifd_ilkn_2_cfg_ptr->lane.blk_to_N2_enable_mask;
    }
    
    PMC_RETURN(rc);
}; /* digi_copy_struct_ilkn_2_cfg */
 
#endif /*DOXYGEN_PUBLIC_ONLY*/

/*
** Public Functions
*/

/*
**  Exported Interface Functions - Start
*/

/*******************************************************************************
*  digi_sifd_ilkn_mon_prog_test_patt_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                This function configures the PIF Interlaken Serdes test 
*                detector.  This feature allows one of 6 PRBS 
*                signals or a fixed programmable pattern to be detected.  
*
*                Up to 3 - 32 bit patterns
*                may be generated or detected.  The programmable pattern is  
*                formed as follows:
*
*                Pattern A * Repetition A, followed by Pattern B * Repetition B
*
*                When Repetion N(A or B) is set to 0, Pattern N is not
*                included in the expected pattern stream.
*
*                This function must be called when the PIF instance is in 
*                Major Bidirection State == 1.  It takes effect when the PIF
*                instance is transitioned to Major Bidirection State == 2.
*
*
*  INPUTS:
*                 *digi_handle        - pointer to DIGI handle instance
*                 *ilkn_inst_ptr      - pointer to  instance of ILKN block to access.
*                 enable              - when TRUE, programmable test pattern is activated
*                                       and pattern_a, pattern_b, rep_a, rep_b are updated.
*                                       when FALSE, programmable test pattern is deactivated
*                                       and pattern_a, pattern_b, rep_a, rep_b are ignored.
*                 
*                 pattern_a           - 32 bit pattern to be repeated  
*                                       rep_a times
*                                       0 to 6 
*                 pattern_b           - 32 bit pattern to be repeated  
*                                       rep_b times
*                                       0 to 6 
*                 rep_a               - number of time to repeat 
*                                       pattern_a.  
*                                       0 to 0xFF
*                 rep_b               - number of time to repeat 
*                                       pattern_b.  
*                                       0 to 0xFF
*
*
*  OUTPUTS:
*      None
*
*  RETURNS:        SIFD_LOG_ERR_INVALID_PARAMETERS
*                  PMC_SUCCESS
*  NOTES:
*      !!!! Important !!!
*      ILKN activated channels' traffic might be disrupted during this feature
*      configuration.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_ilkn_mon_prog_test_patt_cfg(digi_handle_t        *digi_handle,
                                                       digi_ilkn_link_t     *ilkn_inst_ptr,
                                                       BOOL8                 enable,
                                                       UINT32                pattern_a,
                                                       UINT32                pattern_b,
                                                       UINT8                 rep_a,
                                                       UINT8                 rep_b)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    digi_ilkn_link_def_t* ilkn_data_ptr = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    DIGI_ILKN_LINK_HANDLE_CHECK(digi_handle, ilkn_inst_ptr, rc);

    ilkn_data_ptr = (digi_ilkn_link_def_t*)ilkn_inst_ptr;
    sifd_ilkn_test_control_state_t test_mode = SIFD_ILKN_TEST_UNDEFINED;

    if(enable)
        test_mode = SIFD_ILKN_TEST_PROGRAMMABLE_PATTERN;
    else
        test_mode = SIFD_ILKN_TEST_NORMAL_OPERATION;

    rc = sifd_ilkn_test_cfg(digi_handle->sifd_handle, 
                            (sifd_ilkn_inst_t)ilkn_data_ptr->ilkn_inst,
                            SIFD_ILKN_RX_DIRECTION, 
                            test_mode, 
                            pattern_a, 
                            pattern_b,
                            rep_a, 
                            rep_b);

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_sifd_ilkn_mon_prog_test_patt_cfg */

/*******************************************************************************
*  digi_sifd_ilkn_gen_prog_test_patt_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                This function configures the PIF Interlaken Serdes test 
*                generator.  This feature allows a fixed 
*                programmable pattern to be generated.  
*
*                Up to 3 - 32 bit patterns
*                may be generated or detected.  The programmable pattern is  
*                formed as follows:
*
*                Pattern A * Repetition A, followed by Pattern B * Repetition B
*
*                When Repetion N(A or B) is set to 0, Pattern N is not
*                included in the expected pattern stream.
*
*                This function must be called when the PIF instance is in 
*                Major Bidirection State == 1.  It takes effect when the PIF
*                instance is transitioned to Major Bidirection State == 2.
*
*
*  INPUTS:
*                 *digi_handle       - pointer to DIGI handle instance
*                 *ilkn_inst_ptr     - pointer to instance of ILKN block to access.
*                 enable             - when TRUE, programmable test pattern is activated
*                                       and pattern_a, pattern_b, rep_a, rep_b are updated.
*                                       when FALSE, programmable test pattern is deactivated
*                                       and pattern_a, pattern_b, rep_a, rep_b are ignored.
*                 pattern_a           - 32 bit pattern to be repeated  
*                                       rep_a times
*                                       0 to 6 
*                 pattern_b           - 32 bit pattern to be repeated  
*                                       rep_b times
*                                       0 to 6 
*                 rep_a               - number of times to repeat 
*                                       pattern_a.  0 to 0xFF
*                 rep_b               - number of times to repeat 
*                                       pattern_b. 0 to 0xFF
*
*
*  OUTPUTS:
*      None
*
*  RETURNS:        SIFD_LOG_ERR_INVALID_PARAMETERS
*                  PMC_SUCCESS
*
*  NOTES:
*      !!!! Important !!!
*      ILKN activated channels' traffic might be disrupted during this feature
*      configuration.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_ilkn_gen_prog_test_patt_cfg(digi_handle_t      *digi_handle,
                                                       digi_ilkn_link_t   *ilkn_inst_ptr,
                                                       BOOL8               enable,
                                                       UINT32              pattern_a,
                                                       UINT32              pattern_b,
                                                       UINT8               rep_a,
                                                       UINT8               rep_b)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    digi_ilkn_link_def_t* ilkn_data_ptr = NULL;
    sifd_ilkn_test_control_state_t test_mode = SIFD_ILKN_TEST_UNDEFINED;


    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != digi_handle->sifd_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != ilkn_inst_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    
    DIGI_ILKN_LINK_HANDLE_CHECK(digi_handle, ilkn_inst_ptr, rc);

    ilkn_data_ptr = (digi_ilkn_link_def_t*)ilkn_inst_ptr;

    if(enable)
        test_mode = SIFD_ILKN_TEST_PROGRAMMABLE_PATTERN;
    else
        test_mode = SIFD_ILKN_TEST_NORMAL_OPERATION;

    rc = sifd_ilkn_test_cfg(digi_handle->sifd_handle, 
                            (sifd_ilkn_inst_t)ilkn_data_ptr->ilkn_inst,
                            SIFD_ILKN_TX_DIRECTION, 
                            test_mode, 
                            pattern_a, 
                            pattern_b,
                            rep_a, 
                            rep_b);


    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_sifd_ilkn_gen_prog_test_patt_cfg */

/*******************************************************************************
*  digi_sifd_ilkn_prbs_mon_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                This function configures the PIF Interlaken Serdes PRB Monitor.
*                This feature allows one of 6 PRBS signals to be detected. 
*
*                This function must be called when the PIF instance is in 
*                Major Bidirection State == 1.  It takes effect when the PIF
*                instance is transitioned to Major Bidirection State == 2.
*
*
*  INPUTS:
*                 *digi_handle       - pointer to DIGI handle instance
*                 *ilkn_inst_ptr     - instance of ILKN block to access.
*                 cfg_ptr            - pattern monitor configuration
*
*  OUTPUTS:
*      None
*
*  RETURNS:        SIFD_LOG_ERR_INVALID_PARAMETERS
*                  PMC_SUCCESS
*
*  NOTES:
*      !!!! Important !!!
*      ILKN activated channels' traffic might be disrupted during this feature
*      configuration.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_ilkn_prbs_mon_cfg(digi_handle_t          *digi_handle,
                                             digi_ilkn_link_t       *ilkn_inst_ptr,
                                             util_serdes_patt_cfg_t *cfg_ptr)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    digi_ilkn_link_def_t* ilkn_data_ptr = NULL;
    UINT32 pattern = 0;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != digi_handle->sifd_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != ilkn_inst_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    DIGI_ILKN_LINK_HANDLE_CHECK(digi_handle, ilkn_inst_ptr, rc);

    ilkn_data_ptr = (digi_ilkn_link_def_t*)ilkn_inst_ptr;
    sifd_ilkn_test_control_state_t test_mode = SIFD_ILKN_TEST_UNDEFINED;

    if(cfg_ptr->enable == FALSE)
        test_mode = SIFD_ILKN_TEST_NORMAL_OPERATION;
    else if (cfg_ptr->inv_cfg == TRUE)
        test_mode = SIFD_ILKN_TEST_PRBS_INV;
    else
        test_mode = SIFD_ILKN_TEST_PRBS;

    if(cfg_ptr->enable == TRUE)   
        digi_pbrs_mode_to_uint_conv(cfg_ptr->mode,
                                    &pattern);


    rc = sifd_ilkn_test_cfg(digi_handle->sifd_handle, 
                            (sifd_ilkn_inst_t)ilkn_data_ptr->ilkn_inst,
                            SIFD_ILKN_RX_DIRECTION, 
                            test_mode, 
                            pattern, 
                            0,
                            1, 
                            0);

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_sifd_ilkn_prbs_mon_cfg */

/*******************************************************************************
*  digi_sifd_ilkn_prbs_mon_cfg_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                This function gets config of the PIF Interlaken Serdes PRB Generator.
*
*
*  INPUTS:
*                 *digi_handle       - pointer to DIGI handle instance
*                 *ilkn_inst_ptr     - pointer to instance of ILKN block to access.
*
*  OUTPUTS:
*                 cfg_ptr            - pattern monitor configuration
*
*  RETURNS:        SIFD_LOG_ERR_INVALID_PARAMETERS
*                  PMC_SUCCESS
*
*  NOTES:
*      !!!! Important !!!
*      ILKN activated channels' traffic might be disrupted during this feature
*      configuration.
*      !!!! Not that Important !!!
*      cfg_ptr->usr_patt not used
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_ilkn_prbs_mon_cfg_get(digi_handle_t          *digi_handle,
                                                 digi_ilkn_link_t       *ilkn_inst_ptr,
                                                 util_serdes_patt_cfg_t *cfg_ptr)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    digi_ilkn_link_def_t* ilkn_data_ptr = NULL;
    UINT32 pattern = 0;
    util_patt_mode_t patt_mode = LAST_UTIL_PATT_MODE;
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != digi_handle->sifd_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != ilkn_inst_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    DIGI_ILKN_LINK_HANDLE_CHECK(digi_handle, ilkn_inst_ptr, rc);

    cfg_ptr->enable = TRUE;
    cfg_ptr->inv_cfg = FALSE;

    ilkn_data_ptr = (digi_ilkn_link_def_t*)ilkn_inst_ptr;

    sifd_ilkn_test_control_state_t test_mode = SIFD_ILKN_TEST_UNDEFINED;

    rc = sifd_ilkn_test_cfg_get(digi_handle->sifd_handle, 
                                (sifd_ilkn_inst_t)ilkn_data_ptr->ilkn_inst,
                                SIFD_ILKN_RX_DIRECTION, 
                                &test_mode, 
                                &pattern);

    if(test_mode == SIFD_ILKN_TEST_NORMAL_OPERATION)
    {
        cfg_ptr->enable = FALSE;
    }
    else if (test_mode == SIFD_ILKN_TEST_PRBS_INV)
    {
        cfg_ptr->inv_cfg = TRUE;
    }
    
    if(cfg_ptr->enable == TRUE)   
        digi_uint_to_pbrs_mode_conv(pattern,
                                    &patt_mode);

    cfg_ptr->mode = patt_mode;

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_sifd_ilkn_prbs_mon_cfg_get */

/*******************************************************************************
*  digi_sifd_ilkn_prbs_gen_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                This function configures the PIF Interlaken Serdes PRB Generator.
*                This feature allows one of 6 PRBS signals to be generated. 
*
*                This function must be called when the PIF instance is in 
*                Major Bidirection State == 1.  It takes effect when the PIF
*                instance is transitioned to Major Bidirection State == 2.
*
*
*  INPUTS:
*                 *digi_handle       - pointer to DIGI handle instance
*                 *ilkn_inst_ptr     - pointer to instance of ILKN block to access.
*                 cfg_ptr            - pattern monitor configuration
*
*  OUTPUTS:
*      None
*
*  RETURNS:        SIFD_LOG_ERR_INVALID_PARAMETERS
*                  PMC_SUCCESS
*
*  NOTES:
*      !!!! Important !!!
*      ILKN activated channels' traffic might be disrupted during this feature
*      configuration.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_ilkn_prbs_gen_cfg(digi_handle_t          *digi_handle,
                                             digi_ilkn_link_t       *ilkn_inst_ptr,
                                             util_serdes_patt_cfg_t *cfg_ptr)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    digi_ilkn_link_def_t* ilkn_data_ptr = NULL;
    UINT32 pattern = 0;
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != digi_handle->sifd_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != ilkn_inst_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    DIGI_ILKN_LINK_HANDLE_CHECK(digi_handle, ilkn_inst_ptr, rc);

    ilkn_data_ptr = (digi_ilkn_link_def_t*)ilkn_inst_ptr;

    sifd_ilkn_test_control_state_t test_mode = SIFD_ILKN_TEST_UNDEFINED;

    if(cfg_ptr->enable == FALSE)
        test_mode = SIFD_ILKN_TEST_NORMAL_OPERATION;
    else if (cfg_ptr->inv_cfg == TRUE)
        test_mode = SIFD_ILKN_TEST_PRBS_INV;
    else
        test_mode = SIFD_ILKN_TEST_PRBS;

    if(cfg_ptr->enable == TRUE)   
        digi_pbrs_mode_to_uint_conv(cfg_ptr->mode,
                                    &pattern);


    rc = sifd_ilkn_test_cfg(digi_handle->sifd_handle, 
                            (sifd_ilkn_inst_t)ilkn_data_ptr->ilkn_inst,
                            SIFD_ILKN_TX_DIRECTION, 
                            test_mode, 
                            pattern, 
                            0,
                            1, 
                            0);

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_sifd_ilkn_prbs_gen_cfg */

/*******************************************************************************
*  digi_sifd_ilkn_prbs_gen_cfg_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                This function gets config of the PIF Interlaken Serdes PRB Generator.
*
*
*  INPUTS:
*                 *digi_handle       - pointer to DIGI handle instance
*                 *ilkn_inst_ptr     - pointer to instance of ILKN block to access.
*
*  OUTPUTS:
*                 cfg_ptr            - pattern generation configuration
*
*  RETURNS:        SIFD_LOG_ERR_INVALID_PARAMETERS
*                  PMC_SUCCESS
*
*  NOTES:
*      !!!! Important !!!
*      ILKN activated channels' traffic might be disrupted during this feature
*      configuration.
*      !!!! Not that Important !!!
*      cfg_ptr->usr_patt not used
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_ilkn_prbs_gen_cfg_get(digi_handle_t          *digi_handle,
                                                 digi_ilkn_link_t       *ilkn_inst_ptr,
                                                 util_serdes_patt_cfg_t *cfg_ptr)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    digi_ilkn_link_def_t* ilkn_data_ptr = NULL;
    UINT32 pattern = 0;
    util_patt_mode_t patt_mode = LAST_UTIL_PATT_MODE;
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != digi_handle->sifd_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != ilkn_inst_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    DIGI_ILKN_LINK_HANDLE_CHECK(digi_handle, ilkn_inst_ptr, rc);

    cfg_ptr->enable = TRUE;
    cfg_ptr->inv_cfg = FALSE;

    ilkn_data_ptr = (digi_ilkn_link_def_t*)ilkn_inst_ptr;

    sifd_ilkn_test_control_state_t test_mode = SIFD_ILKN_TEST_UNDEFINED;

    rc = sifd_ilkn_test_cfg_get(digi_handle->sifd_handle,
                                (sifd_ilkn_inst_t)ilkn_data_ptr->ilkn_inst,
                                SIFD_ILKN_TX_DIRECTION,
                                &test_mode,
                                &pattern);

    if(test_mode == SIFD_ILKN_TEST_NORMAL_OPERATION)
    {
        cfg_ptr->enable = FALSE;
    }
    else if (test_mode == SIFD_ILKN_TEST_PRBS_INV)
    {
        cfg_ptr->inv_cfg = TRUE;
    }
    
    if(cfg_ptr->enable == TRUE)
        digi_uint_to_pbrs_mode_conv(pattern,
                                    &patt_mode);

    cfg_ptr->mode = patt_mode;

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_sifd_ilkn_prbs_gen_cfg_get */


/*******************************************************************************
*  digi_sifd_ilkn_prbs_status_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*    Retrieves the prbs_monitor synchroniation state
*
*  INPUTS:
*    *digi_handle       - pointer to DIGI handle instance
*    *ilkn_inst_ptr     - instance of ILKN block to access.\n
*
*  OUTPUTS:
*    *status            - when /= 0 RX PRBS checker is locked
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_ilkn_prbs_status_get(digi_handle_t      *digi_handle,
                                                digi_ilkn_link_t   *ilkn_inst_ptr,
                                                UINT32             *status)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    digi_ilkn_link_def_t* ilkn_data_ptr = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != digi_handle->sifd_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != ilkn_inst_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    DIGI_ILKN_LINK_HANDLE_CHECK(digi_handle, ilkn_inst_ptr, rc);

    ilkn_data_ptr = (digi_ilkn_link_def_t*)ilkn_inst_ptr;

    rc = sifd_ilkn_prbs_status_get(digi_handle->sifd_handle, 
                                   (sifd_ilkn_inst_t)ilkn_data_ptr->ilkn_inst, 
                                   status);

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_sifd_ilkn_prbs_status_get */



/*******************************************************************************
*  digi_sifd_ilkn_core_diag_lpbk
*  ___________________________________________________________________________
*
*  DESCRIPTION:    This function enable or disable PIF Interlaken Core
*                  diagnostic loopback.  The loopback is diagnostic relative to
*                  the CPB.  Data from Tx is looped back to Rx.
*
*                  The block must be in in 'Major Bidirectional State' of 2 in
*                  order for fields accessed by this API to be modified.
*
*                  Use this API with sifdIlknEnable as follows:
*                  1. call sifd_ilkn_enable with enable set to 0
*                  2. call sifd_ilkn_core_diag_lpbk with desired loopback
*                     setting
*                  3. call sifd_ilkn_enable with enable set to 1
*
*
*  INPUTS:
*                 *digi_handle        - pointer to DIGI handle instance
*                 *ilkn_inst_data_ptr - pointer to instance of ILKN block to access.
*                 enable              - Looback enable\n
*                                       FALSE: Loopback disabled\n
*                                       TRUE:  Loopback enabled
*
*  OUTPUTS:
*      None
*
*  RETURNS:        SIFD_LOG_ERR_INVALID_PARAMETERS
*                  PMC_SUCCESS
*
*  NOTES:
*      !!!! Important !!!
*      ILKN activated channels' traffic might be disrupted during this feature
*      configuration.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_ilkn_core_diag_lpbk(digi_handle_t       *digi_handle,
                                               digi_ilkn_link_t    *ilkn_inst_data_ptr,
                                               BOOL                 enable)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    digi_ilkn_link_def_t* ilkn_data_ptr = NULL;
    sifd_loopback_cfg_t loopback = SIFD_COPI_ILKN_CPB_INTERNAL_LOOPBACK;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != digi_handle->sifd_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != ilkn_inst_data_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    DIGI_ILKN_LINK_HANDLE_CHECK(digi_handle, ilkn_inst_data_ptr, rc);

    ilkn_data_ptr = (digi_ilkn_link_def_t*)ilkn_inst_data_ptr;
    /* API removed    
       rc = sifd_ilkn_core_diag_lpbk(digi_handle->sifd_handle,
       (sifd_ilkn_inst_t)ilkn_data_ptr->ilkn_inst, enable);
    */
    rc = sifd_loopback_cfg(digi_handle->sifd_handle, 
                           (sifd_ilkn_inst_t)ilkn_data_ptr->ilkn_inst,    
                           loopback);
    
    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_sifd_ilkn_core_diag_lpbk */

/*******************************************************************************
*  digi_sifd_ilkn_fifo_size_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*
*  This API outputs the ODUKSW or CPB FIFO size for a backplane to DIGI ILKN
*  channel.\n\n
*
*  The data storage in the ODUKSW and CPB FIFO has a granularity of 48 bytes.
*  Zone thresholds are specified in units of '48 byte increments'.\n\n
*
*  This API can be used to support developers in setting XON and XOFF Zone
*  thresholds.
*
*
*  INPUTS:
*         *digi_handle                - pointer to DIGI handle instance
*         *chnl_ctxt_ptr              - pointer to instance of ILKN channel
*                                       handle instance.
*
*  OUTPUTS:
*         *num_fifo_block_ptr         - pointer to number of blocks associated
*                                       with ILKN channel handle.
*         *num_fifo_48_byte_incr_ptr  - pointer to number of 48 byte increments
*                                       contained within the FIFO.
*
*  RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_ilkn_fifo_size_get(digi_handle_t       *digi_handle,
                                              digi_ilkn_chnl_t    *chnl_ctxt_ptr,
                                              UINT32              *num_fifo_block_ptr,
                                              UINT32              *num_fifo_48_byte_incr_ptr)
{
    
    cpb_handle_t *h = NULL;
    digi_ilkn_chnl_def_t* chnl_data_ptr = NULL;
    UINT32 slave_port = 0, slave_chnl = 0;
    PMC_ERROR rc = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);


    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    DIGI_ILKN_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, rc);

    chnl_data_ptr = (digi_ilkn_chnl_def_t*)chnl_ctxt_ptr;
   
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_data_ptr);
                        
    /* Do not process channel handle that has not been configured */
    PMC_ASSERT(chnl_data_ptr->switch_data.header.prov_state != UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG, DIGI_ERR_ILKN_CHNL_UNCONFIG, 0, 0);

    /* retreive CPB SS Module Handle */
    switch(chnl_data_ptr->target)
    {
    case DIGI_ILKN_CHNL_TARGET_CPB:
        h = digi_handle->dcpb_handle;    
        slave_port = chnl_data_ptr->switch_data.cpb_data.port_type;
        slave_chnl = chnl_data_ptr->switch_data.cpb_data.channel;
        break;
    case DIGI_ILKN_CHNL_TARGET_ODUK:
        h = coreotn_oduksw_handle_get(digi_handle->coreotn_handle);
        slave_port = chnl_data_ptr->switch_data.oduk_data.port_type;
        slave_chnl = chnl_data_ptr->switch_data.oduk_data.channel;
        break;
    default:
        rc = DIGI_ERR_INVALID_ARG;
        break;
    }

    if (PMC_SUCCESS == rc)
    {
        cpb_fifo_num_blocks_by_input_get(h,
                                         slave_port,
                                         slave_chnl,
                                         num_fifo_block_ptr,
                                         num_fifo_48_byte_incr_ptr);
    }

    PMC_ATOMIC_RETURN(digi_handle, rc);

} /* digi_sifd_ilkn_fifo_size_get */

/*******************************************************************************
*  digi_sifd_ilkn_fifo_thresh_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*
*  This API outputs the ODUKSW or CPB FIFO threshold settings for XON to XOFF
*  and XOFF to XON flow control status indications to upstream device.\n\n
*
*  The data storage in the ODUKSW and CPB FIFO has a granularity of 48 bytes.
*  Zone thresholds are specified in units of '48 byte increments'.\n\n
*
*  This API can be used to support developers in setting XON and XOFF Zone
*  thresholds.
*
*
*  INPUTS:
*                 *digi_handle        - pointer to DIGI handle instance
*                 *chnl_ctxt_ptr      - pointer to instance of ILKN channel
*                                       handle instance.
*
*  OUTPUTS:
*                 *xon_thresh_ptr     - pointer to number of number of 
*                                       '48 byte increments' in a FIFO in a FIFO
*                                       that will trigger XOFF to XON event
*                                       in ILKN flow control
*                 *xoff_thresh_ptr    - pointer to number of number of 
*                                       '48 byte increments' in a FIFO in a FIFO
*                                       that will trigger XON to XOFF event
*                                       in ILKN flow control
*
*  RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_ilkn_fifo_thresh_get(digi_handle_t       *digi_handle,
                                                digi_ilkn_chnl_t    *chnl_ctxt_ptr,
                                                UINT32              *xon_thresh_ptr,
                                                UINT32              *xoff_thresh_ptr)
{
    cpb_handle_t *h = NULL;
    digi_ilkn_chnl_def_t* chnl_data_ptr = NULL;
    UINT32 slave_port = 0, slave_chnl = 0;
    UINT32 dummy;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    DIGI_ILKN_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    chnl_data_ptr = (digi_ilkn_chnl_def_t*)chnl_ctxt_ptr;
   
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_data_ptr);
                        
    /* Do not process channel handle that has not been configured */
    PMC_ASSERT(chnl_data_ptr->switch_data.header.prov_state != UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG, DIGI_ERR_ILKN_CHNL_UNCONFIG, 0, 0);

    /* retreive CPB SS Module Handle */
    switch(chnl_data_ptr->target)
    {
    case DIGI_ILKN_CHNL_TARGET_CPB:
        h = digi_handle->dcpb_handle;    
        slave_port = chnl_data_ptr->switch_data.cpb_data.port_type;
        slave_chnl = chnl_data_ptr->switch_data.cpb_data.channel;
        break;
    case DIGI_ILKN_CHNL_TARGET_ODUK:
        h = coreotn_oduksw_handle_get(digi_handle->coreotn_handle);
        slave_port = chnl_data_ptr->switch_data.oduk_data.port_type;
        slave_chnl = chnl_data_ptr->switch_data.oduk_data.channel;
        break;
    default:
        result = DIGI_ERR_INVALID_ARG;
        break;
    }

    if (PMC_SUCCESS == result)
    {
        result = cpb_dpi_slave_port_chnl_zone_thresh_cfg_get(h,
                                                             slave_port,
                                                             slave_chnl,
                                                             xon_thresh_ptr,
                                                             &dummy,
                                                             xoff_thresh_ptr);
    }
    
    PMC_ATOMIC_RETURN(digi_handle, result);

} /* digi_sifd_ilkn_fifo_thresh_get */

/*******************************************************************************
*  digi_sifd_ilkn_fifo_thresh_set
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*
*  This API configures the ODUKSW or CPB FIFO threshold settings for XON to XOFF
*  and XOFF to XON flow control status indications to upstream device.\n\n
*
*  The data storage in the ODUKSW and CPB FIFO has a granularity of 48 bytes.
*  Zone thresholds are specified in units of '48 byte increments'.\n\n
*
*  This API can be used to support developers in setting XON and XOFF Zone
*  thresholds.
*
*
*  INPUTS:
*                 *digi_handle        - pointer to DIGI handle instance
*                 *chnl_ctxt_ptr      - pointer to instance of ILKN channel
*                                       handle instance.
*                 xon_thresh          - number of '48 byte increments' in a FIFO
*                                       that will trigger XOFF to XON event
*                                       in ILKN flow control
*                 xoff_thresh         - number of '48 byte increments' in a FIFO
*                                       that will trigger XON to XOFF event
*                                       in ILKN flow control
*
*  OUTPUTS:
*       None.
*
*  RETURNS:        
*   PMC_SUCCESS - on success, error otherwise.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_ilkn_fifo_thresh_set(digi_handle_t       *digi_handle,
                                                digi_ilkn_chnl_t    *chnl_ctxt_ptr,
                                                UINT32               xon_thresh,
                                                UINT32               xoff_thresh)
{
    cpb_handle_t *h = NULL;
    digi_ilkn_chnl_def_t* chnl_data_ptr = NULL;
    UINT32 slave_port = 0, slave_chnl = 0;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);


    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    DIGI_ILKN_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    chnl_data_ptr = (digi_ilkn_chnl_def_t*)chnl_ctxt_ptr;
   
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_data_ptr);
                        
    /* Do not process channel handle that has not been configured */
    PMC_ASSERT(chnl_data_ptr->switch_data.header.prov_state != UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG, DIGI_ERR_ILKN_CHNL_UNCONFIG, 0, 0);

    /* retreive CPB SS Module Handle */
    switch(chnl_data_ptr->target)
    {
    case DIGI_ILKN_CHNL_TARGET_CPB:
        h = digi_handle->dcpb_handle;    
        slave_port = chnl_data_ptr->switch_data.cpb_data.port_type;
        slave_chnl = chnl_data_ptr->switch_data.cpb_data.channel;
        break;
    case DIGI_ILKN_CHNL_TARGET_ODUK:
        h = coreotn_oduksw_handle_get(digi_handle->coreotn_handle);
        slave_port = chnl_data_ptr->switch_data.oduk_data.port_type;
        slave_chnl = chnl_data_ptr->switch_data.oduk_data.channel;
        break;
    default:
        result = DIGI_ERR_INVALID_ARG;
        break;
    }

    if (PMC_SUCCESS == result)
    {
        result = cpb_dpi_slave_port_chnl_zone_thresh_cfg(h,
                                                         slave_port,
                                                         slave_chnl,
                                                         xon_thresh,
                                                         xon_thresh,
                                                         xoff_thresh);
    }
    
    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_sifd_ilkn_fifo_thresh_set */

/*******************************************************************************
*  digi_sifd_ilkn_chnl_cnt_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*   This function configures an ILKN statistics channel counter.
*
*  INPUTS:
*   *digi_handle - pointer to DIGI handle instance
*   *ilkn_ctxt_ptr      - pointer to interlaken context structure
*   *chnl_ctxt_ptr      - pointer to interlaken channel structure
*   dir                 - Enum defining the direction to monitor.
*                         TX or RX.                              
*
*  OUTPUTS:
*   None
*
*  RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_ilkn_chnl_cnt_cfg(digi_handle_t *digi_handle,
                                             digi_ilkn_link_t *ilkn_ctxt_ptr,
                                             digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                             sifd_ilkn_dir_t dir)



                                            
{
    PMC_ERROR rc = PMC_SUCCESS;    
    digi_ilkn_link_def_t* digi_ilkn_link_data_ptr;
    digi_sifd_ilkn_inst_t digi_sifd_ilkn_inst;
    UINT32 channel;
    digi_ilkn_chnl_def_t* temp_chnl_ptr = (digi_ilkn_chnl_def_t*)chnl_ctxt_ptr;

    PMC_ATOMIC_ENTRY(digi_handle);

    DIGI_ILKN_LINK_HANDLE_CHECK(digi_handle, ilkn_ctxt_ptr, rc);
    DIGI_ILKN_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, rc);

    digi_ilkn_link_data_ptr = (digi_ilkn_link_def_t*)ilkn_ctxt_ptr;
    digi_sifd_ilkn_inst = digi_ilkn_link_data_ptr->ilkn_inst;

    if(dir == SIFD_ILKN_TX_DIRECTION)
        channel = temp_chnl_ptr->tx_ilkn_info.ilkn_bcw_chnl;
    else if(dir == SIFD_ILKN_RX_DIRECTION)
        channel = temp_chnl_ptr->rx_ilkn_info.ilkn_bcw_chnl;
    else
        rc = SIFD_LOG_ERR_INVALID_PARAMETERS;


    if(rc == PMC_SUCCESS)
        rc = sifd_ilkn_chnl_cnt_cfg(digi_handle->sifd_handle, 
                                    (sifd_ilkn_inst_t) digi_sifd_ilkn_inst,
                                    dir,
                                    channel);


    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_sifd_ilkn_chnl_cnt_cfg */

/*******************************************************************************
*  digi_sifd_ilkn_get_chnl_counts
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*   The functions retrieves statistics count and status information
*   for an ILKN channel.
*                                                            
*  INPUTS:
*   *digi_handle   - pointer to DIGI handle instance
*   *ilkn_ctxt_ptr - pointer to interlaken context structure
*   *chnl_ctxt_ptr - pointer to interlaken channel structure
*   dir            - Enum defining the direction to monitor.
*   *p_byte_count  - pointer to byte count data.
*   *p_pkt_count   - pointer to packet count data.
*   *p_err_count   - pointer to error count data.
*   *p_b_max_count - pointer to burst max count data.
*   *p_b_min_count - pointer to burst min count data.
*   *p_byte_status   - Byte counter status\n
*                    bit 3 RD_BOVER\n
*                    bit 2 RD_BHALF\n
*                    bit 1 RD_BUNCORR\n
*                    bit 0 RD_BCORR
*   *p_pkt_status    - Packet counter status\n
*                    bit 3 RD_POVER\n
*                    bit 2 RD_PHALF\n
*                    bit 1 RD_PUNCORR\n
*                    bit 0 RD_PCORR
*   *p_err_status    - Error counter status\n
*                    bit 3 RD_EOVER\n
*                    bit 2 RD_EHALF\n
*                    bit 1 RD_EUNCORR\n
*                    bit 0 RD_ECORR
*   *p_b_max_status   - Burst Max counter status\n
*                    bit 3 RD_EOVER\n
*                    bit 2 RD_EHALF\n
*                    bit 1 RD_EUNCORR\n
*                    bit 0 RD_ECORR
*   *p_b_min_status   - Burst Min counter status\n
*                    bit 3 RD_EOVER\n
*                    bit 2 RD_EHALF\n
*                    bit 1 RD_EUNCORR\n
*                    bit 0 RD_ECORR
*                   TX or RX.
* 
*  OUTPUTS:
*   None
*                                                            
*  RETURNS:   
*   PMC_SUCCESS - on success, error otherwise.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_ilkn_get_chnl_counts(digi_handle_t *digi_handle,
                                                digi_ilkn_link_t *ilkn_ctxt_ptr,
                                                digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                                sifd_ilkn_dir_t dir,
                                                UINT64 *p_byte_count, 
                                                UINT64 *p_pkt_count, 
                                                UINT64 *p_err_count, 
                                                UINT64 *p_b_max_count, 
                                                UINT64 *p_b_min_count, 
                                                UINT32 *p_byte_status, 
                                                UINT32 *p_pkt_status, 
                                                UINT32 *p_err_status, 
                                                UINT32 *p_b_max_status, 
                                                UINT32 *p_b_min_status)

{
    PMC_ERROR rc = PMC_SUCCESS;    
    digi_ilkn_link_def_t* digi_ilkn_link_data_ptr;
    digi_sifd_ilkn_inst_t digi_sifd_ilkn_inst;
    UINT32 channel;
    digi_ilkn_chnl_def_t* temp_chnl_ptr = (digi_ilkn_chnl_def_t*)chnl_ctxt_ptr;


    PMC_ATOMIC_ENTRY(digi_handle);

    DIGI_ILKN_LINK_HANDLE_CHECK(digi_handle, ilkn_ctxt_ptr, rc);
    DIGI_ILKN_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, rc);

    digi_ilkn_link_data_ptr = (digi_ilkn_link_def_t*)ilkn_ctxt_ptr;
    digi_sifd_ilkn_inst = digi_ilkn_link_data_ptr->ilkn_inst;

    if(dir == SIFD_ILKN_TX_DIRECTION)
        channel = temp_chnl_ptr->tx_ilkn_info.ilkn_bcw_chnl;
    else if(dir == SIFD_ILKN_RX_DIRECTION)
        channel = temp_chnl_ptr->rx_ilkn_info.ilkn_bcw_chnl;
    else
        rc = SIFD_LOG_ERR_INVALID_PARAMETERS;


    if(rc == PMC_SUCCESS)
        rc = sifd_ilkn_get_chnl_counts(digi_handle->sifd_handle, 
                                       (sifd_ilkn_inst_t) digi_sifd_ilkn_inst,
                                       dir,
                                       channel,
                                       p_byte_count, 
                                       p_pkt_count, 
                                       p_err_count, 
                                       p_b_max_count, 
                                       p_b_min_count, 
                                       p_byte_status, 
                                       p_pkt_status, 
                                       p_err_status, 
                                       p_b_max_status, 
                                       p_b_min_status);


    PMC_ATOMIC_RETURN(digi_handle,rc);

} /* digi_sifd_ilkn_get_chnl_counts */

/*******************************************************************************
*  digi_sifd_ilkn_get_rx_intf_counts
*  ___________________________________________________________________________
*                                                            
*  DESCRIPTION:    
*   This function retrives IB_FC_ERR and CRC24 counts for an ILKN RX
*   interface.
*                                                            
*  INPUTS:
*   *digi_handle - pointer to DIGI handle instance
*   *ilkn_ctxt_ptr - pointer to interlaken context structure
*   *p_ib_fc_err_cnt - pointer to IB_FC_ERR count
*   *p_crc24_cnt - pointer to CRC24_CNT count
*   *p_ib_fc_stat - pointer to IB_FC_ERR counter status\n
*                      bit 3 RD_EOVER\n
*                      bit 2 RD_EHALF\n
*                      bit 1 RD_EUNCORR\n
*                      bit 0 RD_ECORR
*   *p_crc24_stat - pointer to CRC24 counter status\n
*                      bit 3 RD_EOVER\n
*                      bit 2 RD_EHALF\n
*                      bit 1 RD_EUNCORR\n
*                      bit 0 RD_ECORR
*   
*  OUTPUTS:
*   None
*                                                            
*  RETURNS:   
*   PMC_SUCCESS - on success, error otherwise.
* 
*  NOTES:
*  
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_ilkn_get_rx_intf_counts(digi_handle_t *digi_handle,
                                                   digi_ilkn_link_t *ilkn_ctxt_ptr,
                                                   UINT64 *p_ib_fc_err_cnt, 
                                                   UINT64 *p_crc24_cnt, 
                                                   UINT32 *p_ib_fc_stat, 
                                                   UINT32 *p_crc24_stat)
{
    PMC_ERROR rc = PMC_SUCCESS;    
    digi_ilkn_link_def_t* digi_ilkn_link_data_ptr;
    digi_sifd_ilkn_inst_t digi_sifd_ilkn_inst;


    PMC_ATOMIC_ENTRY(digi_handle);

    DIGI_ILKN_LINK_HANDLE_CHECK(digi_handle, ilkn_ctxt_ptr, rc);

    digi_ilkn_link_data_ptr = (digi_ilkn_link_def_t*)ilkn_ctxt_ptr;
    digi_sifd_ilkn_inst = digi_ilkn_link_data_ptr->ilkn_inst;

 

    if(rc == PMC_SUCCESS)
        rc = sifd_ilkn_get_rx_intf_counts(digi_handle->sifd_handle, 
                                          (sifd_ilkn_inst_t) digi_sifd_ilkn_inst,
                                          p_ib_fc_err_cnt, 
                                          p_crc24_cnt, 
                                          p_ib_fc_stat, 
                                          p_crc24_stat);


    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_sifd_ilkn_get_rx_intf_counts */

/*******************************************************************************
*  digi_sifd_ilkn_get_serdes_lane_counts
*  ___________________________________________________________________________
*                                                            
*  DESCRIPTION:    
*   This function retrieves SERDES lane statistics.                                             
*                                                            
*  INPUTS:
*   *digi_handle   - pointer to DIGI handle instance
*   *ilkn_ctxt_ptr - pointer to interlaken context structure
*   lane           - Serdes Lane:
*                    0 to 23 
*   counter        - Counter to extract:\n
*                    0101: BURST_CNT\n
*                    0110: ALIGN_CNT\n
*                    0111: ALIGN_FAIL_CNT\n
*                    1000: WRD_SYNC_CNT\n
*                    1001: CDR_CNT\n
*                    1010: CRC32_CNT_FILL\n
*                    1011: BADCTL_CNT
*   *p_cnt_val       - Retrieved Counter value
*   *p_cnt_stat      - pointer to Counter status\n
*                      bit 3 RD_EOVER\n
*                      bit 2 RD_EHALF\n
*                      bit 1 RD_EUNCORR\n
*                      bit 0 RD_ECORR
*   
*  OUTPUTS:
*   None
*                                                            
*  RETURNS:   
*   PMC_SUCCESS - on success, error otherwise.
* 
*  NOTES:
*  
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_ilkn_get_serdes_lane_counts(digi_handle_t *digi_handle,
                                                       digi_ilkn_link_t *ilkn_ctxt_ptr,
                                                       UINT32 lane, 
                                                       UINT32 counter, 
                                                       UINT64 *p_cnt_val, 
                                                       UINT32 *p_cnt_stat)
{
    PMC_ERROR rc = PMC_SUCCESS;    
    digi_ilkn_link_def_t* digi_ilkn_link_data_ptr;
    digi_sifd_ilkn_inst_t digi_sifd_ilkn_inst;

    PMC_ATOMIC_ENTRY(digi_handle);

    DIGI_ILKN_LINK_HANDLE_CHECK(digi_handle, ilkn_ctxt_ptr, rc);

    digi_ilkn_link_data_ptr = (digi_ilkn_link_def_t*)ilkn_ctxt_ptr;
    digi_sifd_ilkn_inst = digi_ilkn_link_data_ptr->ilkn_inst;

    if(rc == PMC_SUCCESS)
        rc = sifd_ilkn_get_serdes_lane_counts(digi_handle->sifd_handle, 
                                              (sifd_ilkn_inst_t) digi_sifd_ilkn_inst,
                                              lane, 
                                              counter, 
                                              p_cnt_val, 
                                              p_cnt_stat);

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_sifd_ilkn_get_serdes_lane_counts */

/*******************************************************************************
*  digi_sifd_ilkn_copi_lowpwr_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*   This function configures COPI in low power state or not.
*
*  INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   *ilkn_ctxt_ptr  - pointer to interlaken context structure
*   lowpwr_flag     - TRUE: in low power state; 
*                     FALSE: not in low power state
*
*  OUTPUTS:
*   None
*
*  RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_ilkn_copi_lowpwr_cfg(digi_handle_t *digi_handle,
                                                digi_ilkn_link_t *ilkn_ctxt_ptr,
                                                BOOL8 lowpwr_flag)
{
    PMC_ERROR rc = PMC_SUCCESS;    
    digi_ilkn_link_def_t* digi_ilkn_link_data_ptr;
    digi_sifd_ilkn_inst_t digi_sifd_ilkn_inst;
    digi_ilkn_chnl_def_t* ilkn_chnl_def = NULL;
    UINT32 chnl = 0;

    PMC_ATOMIC_ENTRY(digi_handle);

    DIGI_ILKN_LINK_HANDLE_CHECK(digi_handle, ilkn_ctxt_ptr, rc);

    digi_ilkn_link_data_ptr = (digi_ilkn_link_def_t*)ilkn_ctxt_ptr;
    digi_sifd_ilkn_inst = digi_ilkn_link_data_ptr->ilkn_inst;


    for(chnl = 0; chnl < DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX; chnl ++)
    {
        if (digi_sifd_ilkn_inst == DIGI_SIFD_ILKN1) {

            ilkn_chnl_def = &(digi_handle->handle_pool.ilkn_1_chnl[chnl]);
        }
        else
        {
            ilkn_chnl_def = &(digi_handle->handle_pool.ilkn_2_chnl[chnl]);
        }

        if(ilkn_chnl_def->switch_data.header.prov_state != UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG)
        {
            rc = SIFD_LOG_ERR_INVALID_PRECONFIG;
            break;
        }
    }
    
    if(rc == PMC_SUCCESS)
    {
        rc = sifd_ilkn_copi_lowpwr_cfg(digi_handle->sifd_handle,
                                       (sifd_ilkn_inst_t) digi_sifd_ilkn_inst,
                                       lowpwr_flag);
    }
    
    PMC_ATOMIC_RETURN(digi_handle,rc);
}/* digi_sifd_ilkn_copi_lowpwr_cfg */


/*******************************************************************************
* FUNCTION: digi_sifd_pmon_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation PMON table with PMON counters 
*   over multiple links.
*
*   digi_sifd_pmon_link_get() can be used to retrieve a specific link
*   from the aggregation table.
*
*   digi_sifd_pmon_enet_chnl_get() can be used to retrieve the ENET PMON
*   for a specific ILKN channel in the link table.
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
PUBLIC PMC_ERROR digi_sifd_pmon_retrieve(digi_handle_t       *digi_handle,
                                         digi_sifd_pmon_t    *pmon_table_ptr)
{
    PMC_ERROR                     result = PMC_SUCCESS;
    digi_ilkn_link_t             *inst_handle;
    UINT32                        i;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_pmon_param_check(digi_handle, pmon_table_ptr); 

    if (PMC_SUCCESS == result)
    {
        PMC_MEMSET(pmon_table_ptr, 0, sizeof(digi_sifd_pmon_t));

        for (i = 0; (i < DIGI_SIFD_ILKN_NUM) && (PMC_SUCCESS == result); i++)
        {
            inst_handle = (digi_ilkn_link_t *) &digi_handle->handle_pool.ilkn_link[i];
            pmon_table_ptr->handle[i] = inst_handle;

            result = digi_sifd_pmon_link_retrieve(digi_handle,
                                                  inst_handle,
                                                  &pmon_table_ptr->sifd_link[i]);
        } /* end for */
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_sifd_pmon_retrieve */


/*******************************************************************************
* FUNCTION: digi_sifd_pmon_link_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Given a link handle and an aggregated PMON table, retrieve a pointer
*   to the corresponding link PMON table.
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   inst_handle     - pointer to link handle to retrieve PMON on
*   pmon_table_ptr  - Aggregated PMON table
*
* OUTPUTS:
*   pmon_link_table_pptr - pointer to link PMON table corresponding to link
*                          handle. Set to NULL if link has not been provisioned.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_pmon_link_get(digi_handle_t                *digi_handle,
                                         digi_ilkn_link_t             *inst_handle,
                                         digi_sifd_pmon_t             *pmon_table_ptr,
                                         digi_sifd_pmon_link_t       **pmon_link_table_pptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          index;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != inst_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != pmon_table_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != pmon_link_table_pptr, DIGI_ERR_INVALID_ARG, 0, 0);

    DIGI_ILKN_LINK_HANDLE_CHECK(digi_handle, inst_handle, result);

    /* Set return table to NULL */
    *pmon_link_table_pptr = NULL;

    /* Get index corresponding to handle */
    index = digi_sifd_inst_get(digi_handle, inst_handle);
    PMC_ASSERT(index < DIGI_SIFD_ILKN_NUM , DIGI_ERR_CODE_ASSERT, 0, 0);

    /* Set return table to link table */
    if (pmon_table_ptr->handle[index] == inst_handle)
    {
        *pmon_link_table_pptr = &pmon_table_ptr->sifd_link[index];
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_sifd_pmon_link_get */


/*******************************************************************************
* FUNCTION: digi_sifd_pmon_enet_chnl_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Given an ILKN chnl handle and a link PMON table, retrieve a pointer
*   to the corresponding channel ENET PMON table.
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   chnl_handle     - ILKN channel handle
*   pmon_table_ptr  - Link PMON table
*
* OUTPUTS:
*   enet_mode_ptr  - pointer to enet pmon mode, or NONE
*   enet_pmon_pptr - pointer to enet chnl PMON table corresponding to link
*                    handle. Set to NULL if chnl has not been provisioned.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_pmon_enet_chnl_get(digi_handle_t                *digi_handle,
                                              digi_ilkn_chnl_t             *chnl_handle,
                                              digi_sifd_pmon_link_t        *pmon_table_ptr,
                                              digi_enet_pmon_mode_t        *enet_mode_ptr,
                                              digi_enet_pmon_chnl_t       **enet_pmon_pptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          i;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != pmon_table_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != enet_mode_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != enet_pmon_pptr, DIGI_ERR_INVALID_ARG, 0, 0);

    DIGI_ILKN_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

    /* Set return table to NULL */
    *enet_mode_ptr  = DIGI_ENET_PMON_MODE_NONE;
    *enet_pmon_pptr = NULL;


    /* Search for matching chnl_handle in link PMON table */
    for (i = 0; i < DIGI_ENET_CHNL_MAX; i++)
    {
        if (chnl_handle == pmon_table_ptr->ilkn_handle[i])
        {
            *enet_mode_ptr  = pmon_table_ptr->enet_mode[i];
            *enet_pmon_pptr = &pmon_table_ptr->enet[i];
            break;
        }
    } 

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_sifd_pmon_enet_chnl_get */


/*******************************************************************************
* FUNCTION: digi_sifd_int_register_cb()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Registers a callback for aggregation interrupt table processing.
*
*   This callback will be called when an active interrupt is found in any
*   SIFD link.
*
*   The callback will be called a minimum of zero times if no active interrupts
*   are found, or a maximum of one time if there is an interrupt in at least
*   one link.
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
PUBLIC PMC_ERROR digi_sifd_int_register_cb(digi_handle_t      *digi_handle,
                                           void               *token,
                                           UINT32              token_size,
                                           digi_int_cb_fcn     user_cb)
{
    PMC_ERROR            result = PMC_SUCCESS;
    digi_int_type_t      int_type = DIGI_INT_TYPE_SIFD;
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);



    if (PMC_SUCCESS == result)
    {
        result = digi_int_callback_set(digi_handle,
                                       int_type,
                                       token,
                                       token_size,
                                       user_cb);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_sifd_int_register_cb */


/*******************************************************************************
* FUNCTION: digi_sifd_int_link_register_cb()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Registers a callback for channel interrupt table processing.
*
*   For each SIFD link where an active interrupt is found, the callback will be
*   called.
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
PUBLIC PMC_ERROR digi_sifd_int_link_register_cb(digi_handle_t      *digi_handle,
                                                void               *token,
                                                UINT32              token_size,
                                                digi_int_cb_fcn     user_cb)
{
    PMC_ERROR            result = PMC_SUCCESS;
    digi_int_type_t      int_type = DIGI_INT_LINK_TYPE_SIFD;
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);



    if (PMC_SUCCESS == result)
    {
        result = digi_int_callback_set(digi_handle,
                                       int_type,
                                       token,
                                       token_size,
                                       user_cb);
    }


    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_sifd_int_link_register_cb */


/*******************************************************************************
* FUNCTION: digi_sifd_int_link_enabled_check()
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
*   int_summary_ptr - If digi_sifd_int_summary_get() was run prior to 
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
PUBLIC PMC_ERROR digi_sifd_int_link_enabled_check(digi_handle_t            *digi_handle,
                                                  digi_sifd_int_summary_t  *int_summary_ptr)
{
    PMC_ERROR              result = PMC_SUCCESS;
    BOOL                   int_summary_free_req = FALSE;         
    digi_int_type_t        int_type = DIGI_INT_LINK_TYPE_SIFD;
    UINT32                 i;
    digi_ilkn_link_t      *inst_handle;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && FALSE == digi_sifd_system_required_test(digi_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }


    /* Callback for interrupt table must be registered before enabled check */
    if (PMC_SUCCESS == result)
    {
        if(!digi_int_is_cb_registered(digi_handle, DIGI_INT_TYPE_SIFD) &&
           !digi_int_is_cb_registered(digi_handle, DIGI_INT_LINK_TYPE_SIFD))
        {
            result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
        }
    }


    /* Find active channels */ 
    if ((PMC_SUCCESS == result) &&
        (NULL == int_summary_ptr))
    {
        int_summary_ptr = (digi_sifd_int_summary_t *) PMC_MALLOC(sizeof(digi_sifd_int_summary_t));
        PMC_MEMSET(int_summary_ptr, 0, sizeof(digi_sifd_int_summary_t));
        int_summary_free_req = TRUE;
        
        result = digi_sifd_int_summary_get(digi_handle,
                                           0, /* unused */
                                           int_summary_ptr);
    }


    /* Call user callback for all active channels */
    for (i = 0; (PMC_SUCCESS == result) && (i < int_summary_ptr->active_link_num); i++)
    {
        inst_handle = int_summary_ptr->active_link[i];

        result = digi_int_callback_run(digi_handle, 
                                       inst_handle,
                                       int_type);
    }
        
    
    if (TRUE == int_summary_free_req)
    {
        PMC_FREE(&int_summary_ptr);
    }

    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_sifd_int_link_enabled_check */

/*******************************************************************************
* FUNCTION: digi_sifd_int_link_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the link 
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   inst_handle     - pointer to link handle to enable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to enable interrupts on all links
*                     of the subsystem
*   int_table_ptr   - pointer to link interrupt table with bits set to
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
PUBLIC PMC_ERROR digi_sifd_int_link_enable(digi_handle_t           *digi_handle,
                                           digi_ilkn_link_t        *inst_handle,   
                                           sifd_int_link_t         *int_table_ptr)
{
    PMC_ERROR              result = PMC_SUCCESS;
    digi_int_type_t        int_type = DIGI_INT_LINK_TYPE_SIFD;

    sifd_handle_t         *sifd_handle;
    UINT32                 start_link = 0;
    UINT32                 end_link = 0;
    UINT32                 i;
    digi_sifd_int_t       *int_en_table_ptr;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);

    DIGI_ILKN_LINK_HANDLE_CHECK(digi_handle, inst_handle, result);

    result = digi_int_param_check(digi_handle, inst_handle, int_type, int_table_ptr); 
        
    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && FALSE == digi_sifd_system_required_test(digi_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }

    sifd_handle = digi_handle->sifd_handle;


    /* Callback for interrupt table must be registered before enable ints */
    if (PMC_SUCCESS == result)
    {
        if(!digi_int_is_cb_registered(digi_handle, DIGI_INT_TYPE_SIFD) &&
           !digi_int_is_cb_registered(digi_handle, DIGI_INT_LINK_TYPE_SIFD))
        {
            result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
        }
    }


    /* Determine if are modfiying one link or all links */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         (void*) inst_handle,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }


    /* Get internal enabled aggregate interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       DIGI_INT_TYPE_SIFD,
                                       (void**) &int_en_table_ptr);

    }


    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        /* Enable interrupts */
        result = sifd_int_link_enable(sifd_handle, 
                                      (sifd_ilkn_inst_t) i,
                                      int_table_ptr,
                                      &int_en_table_ptr->sifd_link[i],
                                      DIGI_ENABLE);
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_sifd_int_link_enable */


/*******************************************************************************
* FUNCTION: digi_sifd_int_link_disable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the link
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   inst_handle     - pointer to link handle to disable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to disable interrupts on all links
*                     of the subsystem
*   int_table_ptr   - pointer to link interrupt table with bits set to
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
PUBLIC PMC_ERROR digi_sifd_int_link_disable(digi_handle_t             *digi_handle,
                                            digi_ilkn_link_t        *inst_handle,   
                                            sifd_int_link_t         *int_table_ptr)
{
    PMC_ERROR              result = PMC_SUCCESS;
    digi_int_type_t        int_type = DIGI_INT_LINK_TYPE_SIFD;

    sifd_handle_t         *sifd_handle;
    UINT32                 start_link = 0;
    UINT32                 end_link = 0;
    UINT32                 i;
    digi_sifd_int_t     *int_en_table_ptr;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);

    DIGI_ILKN_LINK_HANDLE_CHECK(digi_handle, inst_handle, result);

    result = digi_int_param_check(digi_handle, inst_handle, int_type, int_table_ptr); 
        
    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && FALSE == digi_sifd_system_required_test(digi_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }

    sifd_handle = digi_handle->sifd_handle;

    /* Determine if are modfiying one link or all links */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         (void*) inst_handle,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }


    /* Get internal enabled aggregate interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       DIGI_INT_TYPE_SIFD,
                                       (void**) &int_en_table_ptr);

    }


    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        /* Enable interrupts */
        result = sifd_int_link_enable(sifd_handle, 
                                      (sifd_ilkn_inst_t) i,
                                      int_table_ptr,
                                      &int_en_table_ptr->sifd_link[i],
                                      DIGI_DISABLE);
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_sifd_int_link_disable */


/*******************************************************************************
* FUNCTION: digi_sifd_int_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the aggregated 
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_table_ptr   - pointer to link interrupt table with bits set to
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
PUBLIC PMC_ERROR digi_sifd_int_clear(digi_handle_t           *digi_handle,
                                     digi_sifd_int_t         *int_table_ptr)
{
    PMC_ERROR                result = PMC_SUCCESS;
    digi_int_type_t          int_type = DIGI_INT_LINK_TYPE_SIFD;

    sifd_handle_t           *sifd_handle = NULL;
    digi_sifd_int_t         *int_en_table_ptr = NULL;
    UINT32                   start_link = 0;
    UINT32                   end_link = 0;
    UINT32                   i;
    digi_sifd_ilkn_inst_t    ilkn_inst;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    result = digi_int_param_check(digi_handle, DIGI_INT_ALL_CHNLS, int_type, int_table_ptr); 

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && FALSE == digi_sifd_system_required_test(digi_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }

    sifd_handle = digi_handle->sifd_handle;



    /* Determine if are modfiying one link or all links */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         DIGI_INT_ALL_CHNLS,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }
    
    /* Get internal enabled interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       DIGI_INT_TYPE_SIFD,
                                       (void**) &int_en_table_ptr);

    }

    /* Clear interrupts */
    if (IS_DIGI_INT_ALL_INTS(int_table_ptr))
    {
        result = digi_sifd_int_link_clear(digi_handle,
                                          DIGI_INT_ALL_CHNLS,
                                          DIGI_INT_ALL_INTS);
    }
    else
    {
        for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
        {
            if (NULL != int_table_ptr->handle[i])
            {
                ilkn_inst = digi_sifd_inst_get(digi_handle, int_table_ptr->handle[i]);
                
                result = sifd_int_link_clear(sifd_handle, 
                                             (sifd_ilkn_inst_t) ilkn_inst,
                                             &int_table_ptr->sifd_link[i],
                                             &int_en_table_ptr->sifd_link[i]);
            }
        }
    }

    /* clear SIFD ILKN ENET PMON interrupts */   
    if (PMC_SUCCESS == result)
    {
        if (NULL == int_table_ptr)
        {
            result = digi_sifd_enet_pmon_int_chnl_cfg(digi_handle,
                                                      DIGI_INT_ALL_CHNLS,
                                                      DIGI_INT_ALL_INTS,
                                                      TRUE,
                                                      FALSE);
        }
        else
        {
            for (i = 0; (i < DIGI_ENET_CHNL_MAX) && (PMC_SUCCESS == result); i++)
            {
                if ((PMC_SUCCESS == digi_ilkn_chnl_handle_validate(digi_handle, int_table_ptr->enet_pmon_int_chnl[i].chnl_handle)) &&
                    int_table_ptr->enet_pmon_int_chnl[i].chnl_handle == int_en_table_ptr->enet_pmon_int_chnl[i].chnl_handle)
                {
                    result = digi_sifd_enet_pmon_int_chnl_cfg(digi_handle,
                                                              int_table_ptr->enet_pmon_int_chnl[i].chnl_handle,
                                                              &int_table_ptr->enet_pmon_int_chnl[i].enet_pmon,
                                                              TRUE,
                                                              FALSE);  
                }                    
            }
        }
    }
     
    
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_sifd_int_clear */


/*******************************************************************************
* FUNCTION: digi_sifd_int_link_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears link interrupts based on bits that are set in the aggregated 
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   inst_handle     - pointer to link handle to retrieve interrupt on
*   int_table_ptr   - pointer to link interrupt table with bits set to
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
PUBLIC PMC_ERROR digi_sifd_int_link_clear(digi_handle_t           *digi_handle,
                                          digi_ilkn_link_t        *inst_handle,
                                          sifd_int_link_t         *int_table_ptr)
{
    PMC_ERROR                result = PMC_SUCCESS;
    digi_int_type_t          int_type = DIGI_INT_LINK_TYPE_SIFD;

    sifd_handle_t           *sifd_handle;
    digi_sifd_int_t         *int_en_table_ptr;
    UINT32                   start_link = 0;
    UINT32                   end_link = 0;
    UINT32                   i;
    digi_ilkn_link_def_t    *handle_pool_ptr;
    BOOL8                    multichannels;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    result = digi_int_param_check(digi_handle, DIGI_INT_ALL_CHNLS, int_type, int_table_ptr); 

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && FALSE == digi_sifd_system_required_test(digi_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }

    sifd_handle = digi_handle->sifd_handle;
    handle_pool_ptr = digi_handle->handle_pool.ilkn_link;


    /* Determine if are modfiying one link or all links */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         inst_handle,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }
    if (IS_DIGI_INT_ALL_CHNLS(inst_handle))
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
                                       DIGI_INT_TYPE_SIFD,
                                       (void**) &int_en_table_ptr);

    }

    /* Clear interrupts */
    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        if (TRUE == multichannels)
        {
            inst_handle = &handle_pool_ptr[i];
        }

        if (NULL != inst_handle && 
            ((handle_pool_ptr[i].ilkn_inst ==  DIGI_SIFD_ILKN1 && digi_handle->var.sifd_1_major_mode != DIGI_SIFD_PATH_NOT_IN_USE) ||
             (handle_pool_ptr[i].ilkn_inst ==  DIGI_SIFD_ILKN2 && digi_handle->var.sifd_2_major_mode != DIGI_SIFD_PATH_NOT_IN_USE)))
        {
            result = sifd_int_link_clear(sifd_handle, 
                                         (sifd_ilkn_inst_t) i,
                                         int_table_ptr,
                                         &int_en_table_ptr->sifd_link[i]);
        }
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_sifd_int_link_clear */


/*******************************************************************************
* FUNCTION: digi_sifd_int_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with interrupt bits
*   over multiple links.
*
*   digi_sifd_int_link_get() can be used to retrieve a specific link table from
*   the aggregation table.
*
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
PUBLIC PMC_ERROR digi_sifd_int_retrieve(digi_handle_t       *digi_handle,
                                        digi_sifd_int_t   *int_table_ptr)
{
    PMC_ERROR                        result = PMC_SUCCESS;
    digi_int_type_t                  int_type = DIGI_INT_TYPE_SIFD;

    sifd_handle_t                   *sifd_handle;
    UINT32                           start_link = 0;
    UINT32                           end_link = 0;
    UINT32                           i;
    digi_sifd_int_t                 *int_en_table_ptr;
    digi_ilkn_link_def_t            *handle_pool_ptr;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);
    result = digi_int_param_check(digi_handle, DIGI_INT_ALL_CHNLS, int_type, int_table_ptr); 

    PMC_MEMSET(int_table_ptr, 0, sizeof(digi_sifd_int_t));

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && FALSE == digi_sifd_system_required_test(digi_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle, PMC_SUCCESS);
    }


    sifd_handle = digi_handle->sifd_handle;
    handle_pool_ptr = digi_handle->handle_pool.ilkn_link;


    /* Determine if are modfiying one link or all links */
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
                                       DIGI_INT_TYPE_SIFD,
                                       (void**) &int_en_table_ptr);

    }
   
    
    /* enable interrupt caching, clear cache */
    pmc_sys_int_cache_enable_set(((pmc_handle_t*)digi_handle)->sys_handle, TRUE, TRUE);

    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        int_table_ptr->handle[i] = (digi_ilkn_link_t *) &handle_pool_ptr[i];

        /* Retreive status for enabled interrupts */
        result = sifd_int_link_retrieve(sifd_handle, 
                                        (sifd_ilkn_inst_t) i,
                                        &int_en_table_ptr->sifd_link[i],
                                        &int_table_ptr->sifd_link[i]);
    }
    
    /* retrieve SIFD ILKN channels  ENENT PMON interrupts */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         DIGI_INT_ALL_CHNLS,
                                         DIGI_INT_CHNL_TYPE_SIFD_ENET_PMON,
                                         &start_link,
                                         &end_link);
        if (PMC_SUCCESS == result)
        {
            for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
            {                
                if (NULL != int_en_table_ptr->enet_pmon_int_chnl[i].chnl_handle &&
                    PMC_SUCCESS == digi_ilkn_chnl_handle_validate(digi_handle, int_en_table_ptr->enet_pmon_int_chnl[i].chnl_handle))
                {
                    result = digi_sifd_enet_pmon_int_chnl_filt_retrieve(digi_handle,
                                                                        int_en_table_ptr->enet_pmon_int_chnl[i].chnl_handle,                                          
                                                                        &int_en_table_ptr->enet_pmon_int_chnl[i].enet_pmon,
                                                                        &int_table_ptr->enet_pmon_int_chnl[i].enet_pmon);
                    if (PMC_SUCCESS == result)
                    {
                        int_table_ptr->enet_pmon_int_chnl[i].chnl_handle = int_en_table_ptr->enet_pmon_int_chnl[i].chnl_handle;
                    }
                }
            }
        }
    }
                                                            

    
    /* disable interrupt caching */
    if (PMC_SUCCESS == result)
    {
    	pmc_sys_int_cache_enable_set(((pmc_handle_t*)digi_handle)->sys_handle, FALSE, FALSE);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_sifd_int_retrieve */


/*******************************************************************************
* FUNCTION: digi_sifd_int_filt_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with interrupt bits
*   over multiple channels.
*
*   digi_sifd_int_link_get() can be used to retrieve a specific chnl table
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
PUBLIC PMC_ERROR digi_sifd_int_filt_retrieve(digi_handle_t           *digi_handle,
                                             sifd_int_link_t         *filt_table_ptr,   
                                             digi_sifd_int_t         *int_table_ptr)
{
    PMC_ERROR                result = PMC_SUCCESS;
    digi_int_type_t          int_type = DIGI_INT_TYPE_SIFD;

    UINT32                   start_link = 0;
    UINT32                   end_link = 0;
    UINT32                   i;
    digi_ilkn_link_def_t    *handle_pool_ptr;
    digi_ilkn_link_t        *inst_handle;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_int_param_check(digi_handle, DIGI_INT_ALL_CHNLS, int_type, int_table_ptr); 

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && FALSE == digi_sifd_system_required_test(digi_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle, PMC_SUCCESS);
    }

    PMC_MEMSET(int_table_ptr, 0, sizeof(digi_sifd_int_t));

    handle_pool_ptr = digi_handle->handle_pool.ilkn_link;


    /* Determine if are modfiying one channel or all channels */
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

    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        inst_handle = &handle_pool_ptr[i];

        if (NULL != inst_handle && 
            ((handle_pool_ptr[i].ilkn_inst ==  DIGI_SIFD_ILKN1 && digi_handle->var.sifd_1_major_mode != DIGI_SIFD_PATH_NOT_IN_USE) ||
             (handle_pool_ptr[i].ilkn_inst ==  DIGI_SIFD_ILKN2 && digi_handle->var.sifd_2_major_mode != DIGI_SIFD_PATH_NOT_IN_USE)))
        {
            int_table_ptr->handle[i] = inst_handle; 

            result = digi_sifd_int_link_filt_retrieve(digi_handle,
                                                      inst_handle,
                                                      filt_table_ptr,
                                                      &int_table_ptr->sifd_link[i]);
        }   
    }

    /* disable interrupt caching */
    pmc_sys_int_cache_enable_set(((pmc_handle_t*)digi_handle)->sys_handle, FALSE, FALSE);


    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_sifd_int_filt_retrieve */

/*******************************************************************************
* FUNCTION: digi_sifd_int_link_filt_retrieve()
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
PUBLIC PMC_ERROR digi_sifd_int_link_filt_retrieve(digi_handle_t         *digi_handle,
                                                  digi_ilkn_link_t      *chnl_handle,
                                                  sifd_int_link_t       *filt_table_ptr,
                                                  sifd_int_link_t       *int_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    sifd_handle_t       *sifd_handle;
    UINT32              start_link = 0;
    UINT32              end_link = 0;
    digi_int_type_t     int_type = DIGI_INT_LINK_TYPE_SIFD;

    PMC_ATOMIC_ENTRY(digi_handle);
   
    sifd_handle   = digi_handle->sifd_handle;


    result = digi_int_param_check(digi_handle, chnl_handle, int_type, int_table_ptr); 
   
    if ((PMC_SUCCESS == result) &&
        (NULL == chnl_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_NULL_HANDLE);
    }

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && FALSE == digi_sifd_system_required_test(digi_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle, PMC_SUCCESS);
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


    /* Retreive status for enabled interrupts */
    if (PMC_SUCCESS == result)
    {
        result = sifd_int_link_retrieve(sifd_handle, 
                                        (sifd_ilkn_inst_t)start_link,
                                        filt_table_ptr,
                                        int_table_ptr);
    }
           

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_sifd_int_link_filt_retrieve */


/*******************************************************************************
* FUNCTION: digi_sifd_int_summary_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves summary of sifd interrupts.
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   summary_mask    - Bit mask of bits from digi_sifd_int_summary_mask_t
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
PUBLIC PMC_ERROR digi_sifd_int_summary_get(digi_handle_t            *digi_handle,
                                           UINT32                    summary_mask,
                                           digi_sifd_int_summary_t  *int_summary_ptr)
{
    PMC_ERROR                        result = PMC_SUCCESS;
    UINT32                           start_link = 0;
    UINT32                           end_link = 0;
    digi_otn_int_t                  *int_en_table_ptr;
    UINT32                           i, j;
    sifd_handle_t                   *sifd_handle;
    digi_ilkn_link_def_t            *handle_pool_ptr;
    BOOL                             int_found;
    digi_int_type_t                  int_type = DIGI_INT_TYPE_SIFD;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    if (NULL == int_summary_ptr)
    {
        PMC_ATOMIC_RETURN(digi_handle, DIGI_ERR_NULL_HANDLE);
    }


    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && FALSE == digi_sifd_system_required_test(digi_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle, PMC_SUCCESS);
    }

    sifd_handle = digi_handle->sifd_handle;
    handle_pool_ptr = digi_handle->handle_pool.ilkn_link;

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
        int_found = FALSE;

        result = digi_sifd_int_link_enabled_check_internal(digi_handle,
                                                           &handle_pool_ptr[i],
                                                           &int_found);


        if ((TRUE == int_found) && (PMC_SUCCESS == result))
        {
            int_summary_ptr->active_link[j] = (sifd_int_link_t *) &handle_pool_ptr[i];
            j++;
        }
    }


    /* disable interrupt caching */
    pmc_sys_int_cache_enable_set(((pmc_handle_t*)digi_handle)->sys_handle, FALSE, FALSE);

    int_summary_ptr->active_link_num = j;


    if (PMC_SUCCESS == result)
    {
        result = digi_sifd_enet_pmon_int_chnl_enabled_check(digi_handle,
                                                            int_summary_ptr);
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_sifd_int_summary_get */


/*******************************************************************************
* FUNCTION: digi_sifd_int_link_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Given a link handle and an aggregated interrupt table, retrieve a pointer
*   to the corresponding link interrupt table.
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   inst_handle     - pointer to link handle to retrieve interrupt on
*   int_table_ptr   - Aggregated interrupt table
*
* OUTPUTS:
*   int_link_table_pptr  - pointer to link interrupt table corresponding to link
*                          handle. Set to NULL if link has not been provisioned.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_int_link_get(digi_handle_t              *digi_handle,
                                        digi_ilkn_link_t           *inst_handle,
                                        digi_sifd_int_t            *int_table_ptr,
                                        sifd_int_link_t           **int_link_table_pptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          index;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != inst_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != int_table_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != int_link_table_pptr, DIGI_ERR_INVALID_ARG, 0, 0);

    DIGI_ILKN_LINK_HANDLE_CHECK(digi_handle, inst_handle, result);

    /* Set return table to NULL */
    *int_link_table_pptr = NULL;

    /* Get index corresponding to handle */
    index = digi_sifd_inst_get(digi_handle, inst_handle);

    PMC_ASSERT(index < DIGI_SIFD_ILKN_NUM , DIGI_ERR_INVALID_ARG, index, DIGI_SIFD_ILKN_NUM);

    /* Set return table to link table */
    if (int_table_ptr->handle[index] == inst_handle)
    {
        *int_link_table_pptr = &int_table_ptr->sifd_link[index];
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_sifd_int_link_get */




/*******************************************************************************
* FUNCTION: digi_sifd_int_enabled_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with enabled interrupt bits
*   over multiple links.
*
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
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
PUBLIC PMC_ERROR digi_sifd_int_enabled_get(digi_handle_t       *digi_handle,
                                           digi_sifd_int_t   *int_table_ptr)
{
    PMC_ERROR                        result = PMC_SUCCESS;
    digi_int_type_t                  int_type = DIGI_INT_TYPE_SIFD;

    UINT32                           start_link = 0;
    UINT32                           end_link = 0;
    UINT32                           i;
    digi_sifd_int_t                 *int_en_table_ptr;
    digi_ilkn_link_def_t            *handle_pool_ptr;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);
    result = digi_int_param_check(digi_handle, DIGI_INT_ALL_CHNLS, int_type, int_table_ptr); 
    
    PMC_MEMSET(int_table_ptr, 0, sizeof(digi_sifd_int_t));

    handle_pool_ptr = digi_handle->handle_pool.ilkn_link;


    /* Determine if are modfiying one link or all links */
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
                                       DIGI_INT_TYPE_SIFD,
                                       (void**) &int_en_table_ptr);

    }
   
    if (PMC_SUCCESS == result)
    {
        /* Copy table to output */
        PMC_MEMCPY(int_table_ptr, int_en_table_ptr, sizeof(*int_table_ptr));
    }

    
    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        /* Store link handle */
        int_table_ptr->handle[i] = (digi_ilkn_link_t *) &handle_pool_ptr[i];
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_sifd_int_enabled_get */


/*******************************************************************************
* FUNCTION: digi_sifd_int_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in all links of the subsystem.
*
*   The callback associated with the aggregation interrupt type will be called
*   if an active interrupt is found in any link. The callback will be
*   called a minimum of zero times if no active interrupts are found, or
*   a maximum of one time if there is an interrupt in at least one link.
*
*   A callback must be registered for the interrupt type before this API
*   can be called.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS if operation is executed successfully otherwise a descriptive error
*   code is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_int_enabled_check(digi_handle_t      *digi_handle)
{
    PMC_ERROR                result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    result = digi_sifd_int_enabled_check_internal(digi_handle);
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_sifd_int_enabled_check */



/*******************************************************************************
* digi_sifd_int_enet_pmon_chnl_register_cb
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Registers a callback for SIFD ILKN channel ENET PMON interrupt table processing.
*
*   For each SIFD ILKN channel provisioned with an ENET PMON where an active 
*   interrupt is found, the callback will be called.
*
*   The callback is called a minimum of zero times if no active interrupts are
*   found ENET PMON channel, or a maximum of the number of possible ENET PMON channels
*   if there is an active interrupt in each of the channels.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   token           - user token to be stored and passed back to callback\n
*                     Optional, can be NULL.
*   token_size      - token size in bytes\n
*                     Optional, can be 0.
*   user_cb         - callback to be registered against this interrupt type
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_int_enet_pmon_chnl_register_cb(digi_handle_t      *digi_handle,
                                                          void               *token,
                                                          UINT32              token_size,
                                                          digi_int_cb_fcn     user_cb)
{
    PMC_ERROR            result = PMC_SUCCESS;
    digi_int_type_t      int_type = DIGI_INT_CHNL_TYPE_SIFD_ENET_PMON;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    
    if (PMC_SUCCESS == result)
    {
        result = digi_int_callback_set(digi_handle,
                                       int_type,
                                       token,
                                       token_size,
                                       user_cb);
    }

   
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_sifd_int_enet_pmon_chnl_register_cb */

/*******************************************************************************
* digi_sifd_enet_pmon_int_chnl_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the SIFD ILKN channel
*   ENET PMON interrupt table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to enable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to enable interrupts on all channels
*   int_table_ptr   - pointer to channel ENET interrupt table with bits set to
*                     enable interrupts
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_enet_pmon_int_chnl_enable(digi_handle_t             *digi_handle,
                                                     digi_ilkn_chnl_t          *chnl_handle,
                                                     enet_int_chnl_t           *int_table_ptr)
{
    PMC_ERROR                result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check that sifd ilkn channel handle belongs to digi context */ 
    DIGI_ILKN_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

    result = digi_sifd_enet_pmon_int_chnl_cfg(digi_handle,
                                              chnl_handle,
                                              int_table_ptr,
                                              FALSE,
                                              TRUE);
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_sifd_enet_pmon_int_chnl_enable */

/*******************************************************************************
* digi_sifd_enet_pmon_int_chnl_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disables interrupts based on bits that are set in the SIFD ILKN channel
*   ENET PMON interrupt table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to disable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to disable interrupts on all channels
*   int_table_ptr   - pointer to channel ENET interrupt table with bits set to
*                     disable interrupts
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_enet_pmon_int_chnl_disable(digi_handle_t             *digi_handle,
                                                      digi_ilkn_chnl_t        *chnl_handle,
                                                      enet_int_chnl_t         *int_table_ptr)
{
    PMC_ERROR                result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check that sifd ilkn channel handle belongs to digi context */ 
    DIGI_ILKN_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

    result = digi_sifd_enet_pmon_int_chnl_cfg(digi_handle,
                                              chnl_handle,
                                              int_table_ptr,
                                              FALSE,
                                              FALSE);
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_sifd_enet_pmon_int_chnl_disable */

/*******************************************************************************
* digi_sifd_enet_pmon_int_chnl_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clear interrupts based on bits that are set in the SIFD ILKN channel
*   ENET PMON interrupt table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to disable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to clear interrupts on all channels
*   int_table_ptr   - pointer to channel ENET interrupt table with bits set to
*                     disable interrupts
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_enet_pmon_int_chnl_clear(digi_handle_t           *digi_handle,
                                                    digi_ilkn_chnl_t        *chnl_handle,
                                                    enet_int_chnl_t         *int_table_ptr)
{
    PMC_ERROR                result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check that sifd ilkn channel handle belongs to digi context */ 
    DIGI_ILKN_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

    result = digi_sifd_enet_pmon_int_chnl_cfg(digi_handle,
                                              chnl_handle,
                                              int_table_ptr,
                                              TRUE,
                                              FALSE);
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_sifd_enet_pmon_int_chnl_clear */

/*******************************************************************************
* digi_sifd_enet_pmon_int_chnl_filt_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves a SIFD ILKN channel ENET PMON interrupt table with interrupt bits.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*
*
* INPUTS:
*   *digi_handle         - pointer to DIGI handle instance
*   chnl_handle          - pointer to channel handle to retrieve interrupt on
*   filt_table_ptr       - pointer to filter table. Mark fields as TRUE within this
*                          table to indicate that the interrupt/status will be retrieved
*                          to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                          interrupt/status.
*
* OUTPUTS:
*   int_table_ptr  - pointer to channel interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_enet_pmon_int_chnl_filt_retrieve(digi_handle_t     *digi_handle,
                                                            digi_ilkn_chnl_t  *chnl_handle,
                                                            enet_int_chnl_t   *filt_table_ptr,
                                                            enet_int_chnl_t   *int_table_ptr)
{
    PMC_ERROR                        result = PMC_SUCCESS;
    digi_int_type_t                  int_type = DIGI_INT_CHNL_TYPE_SIFD_ENET_PMON;
    enet_handle_t                   *enet_ptr;
    UINT32                           link;   

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check that sifd ilkn channel handle belongs to digi context */ 
    DIGI_ILKN_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

    result = digi_int_param_check(digi_handle, chnl_handle, int_type, int_table_ptr); 
   
    if ((PMC_SUCCESS == result) &&
        (NULL == chnl_handle))
    {
        result = DIGI_ERR_NULL_HANDLE;
    }

    /* retrieve enet pmon interrupts */
    if (PMC_SUCCESS == result)
    {
        /* get enet pmon handle */
        result = digi_sifd_enet_pmon_handle_get(digi_handle,
                                                (digi_ilkn_chnl_def_t *)chnl_handle,
                                                &enet_ptr,
                                                &link);
        if (PMC_SUCCESS == result && 
            NULL != enet_ptr)
        { 
            if (PMC_SUCCESS == result && TRUE == enet_start_state_test(enet_ptr))
            {
                PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
            }

            /* Retieve interrupts from FW */
            if (DIGI_ENET_LINE_PMON == ((digi_ilkn_chnl_def_t *)chnl_handle)->enet_pmon &&
                digi_handle->var.fw_intr_mon_running[INTR_IPI_ENET_LINE] == TRUE)
            {
                result = digi_enet_int_fw_chnl_exco4m_rx_lobl_i_retrieve(digi_handle,
                                                                         DIGI_INT_TYPE_ENET_LINE,
                                                                         TRUE,
                                                                         TRUE,
                                                                         filt_table_ptr,
                                                                         int_table_ptr);
            }   

            if (PMC_SUCCESS == result)
            {        
                if (DIGI_ENET_SYS_PMON == ((digi_ilkn_chnl_def_t *)chnl_handle)->enet_pmon && 
                    digi_handle->var.fw_intr_mon_running[INTR_IPI_ENET_SYS] == TRUE)
                {
                    result = digi_enet_int_fw_chnl_exco4m_rx_lobl_i_retrieve(digi_handle,
                                                                             DIGI_INT_TYPE_ENET_SYS,
                                                                             TRUE,
                                                                             FALSE,
                                                                             filt_table_ptr,
                                                                             int_table_ptr);
                }
            }

            if (PMC_SUCCESS == result)
            {
                /* Enable enet interrupts */
                result = enet_int_chnl_retrieve(enet_ptr, 
                                                link,
                                                filt_table_ptr,
                                                int_table_ptr);
            }
        }
        else
        {
            result = PMC_SUCCESS;
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_sifd_enet_pmon_int_chnl_filt_retrieve */

/*******************************************************************************
* digi_sifd_enet_pmon_int_chnl_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in all SIFD ILKN ENET PMON channels.
*
*   The callback associated with the channel interrupt type will be called
*   for each channel that has an active interrupt.

*   The callback will be called a minimum of zero times if no active interrupts
*   are found, or a maximum of once per channel if there is an active interrupt
*   on every channel.
*
*   A callback must be registered for DIGI_INT_CHNL_TYPE_SIFD_ENET_PMON interrupt 
*   type before this API can be called.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   int_summary_ptr - If digi_sifd_int_summary_get() was run prior to 
*                     calling this function, the pointer to the interrupt summary
*                     structure can be passed in to decrease the search
*                     time for active interrupts in this function.
*                     Set this parameter to NULL if the summary was not retrieved.
*
*   
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_enet_pmon_int_chnl_enabled_check(digi_handle_t            *digi_handle,
                                                            digi_sifd_int_summary_t  *int_summary_ptr)
{
    PMC_ERROR                    result = PMC_SUCCESS;
    UINT32                       i;  
    digi_int_type_t              int_type = DIGI_INT_CHNL_TYPE_SIFD_ENET_PMON;
    digi_ilkn_chnl_def_t        *chnl_handle = DIGI_INT_ALL_CHNLS;
    digi_sifd_int_t             *int_en_table_ptr;
    BOOL8                        int_found;
    UINT32                       start_link = 0;
    UINT32                       end_link = 0;
    enet_handle_t               *enet_ptr;
    UINT32                       link;
    UINT32                       j;
    enet_int_chnl_t              enet_int_table;
    UINT32                       ltr;
    BOOL                         int_summary_free_req = FALSE;    
    UINT32                       ipi_idx;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
   
    if((!digi_int_is_cb_registered(digi_handle, DIGI_INT_TYPE_SIFD)) && 
       (!digi_int_is_cb_registered(digi_handle, int_type)))
    {
        result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
    }

    /* Determine if are accessing one channel or all channels */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         chnl_handle,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }

    /* Get internal enabled interrupt table to check */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       DIGI_INT_TYPE_SIFD,
                                       (void**) &int_en_table_ptr);

    }
    if (PMC_SUCCESS == result &&
        (NULL == int_summary_ptr))
    {
        int_summary_ptr = (digi_sifd_int_summary_t *) PMC_MALLOC(sizeof(digi_sifd_int_summary_t));
        int_summary_free_req = TRUE;
    }

    if (PMC_SUCCESS == result)
    {
        PMC_MEMSET(int_summary_ptr, 0, sizeof(digi_sifd_int_summary_t));
    }

    /* Call user callback for all active channels */
    for (i = 0, j = 0; (PMC_SUCCESS == result) && (i < DIGI_ENET_CHNL_MAX); i += 1)
    {
        chnl_handle = (digi_ilkn_chnl_def_t*)int_en_table_ptr->enet_pmon_int_chnl[i].chnl_handle;
        int_found = FALSE;

        if (NULL != chnl_handle &&
            util_global_is_chnl_prov(chnl_handle))
        {            
            /* check ENET PMON interrupts */
            result = digi_sifd_enet_pmon_handle_get(digi_handle,
                                                    chnl_handle,
                                                    &enet_ptr,
                                                    &link);
            if (PMC_SUCCESS == result && 
                NULL != enet_ptr)
            {         
                /* Skip interrupt retrieve if subsystem is not powered up */
                if (TRUE == enet_start_state_test(enet_ptr))
                {
                    continue;
                }
                
                if (NULL != int_en_table_ptr->enet_pmon_int_chnl[link].chnl_handle)
                {
                    /* Retieve interrupts from FW */
                    ipi_idx = LAST_INTR_IPI_WORKAROUND;
                    if (0 == link)
                    {
                        if (DIGI_ENET_LINE_PMON == chnl_handle->enet_pmon)
                        {
                            ipi_idx = INTR_IPI_ENET_LINE;
                        }
                        else if (DIGI_ENET_SYS_PMON == chnl_handle->enet_pmon)
                        {
                            ipi_idx = INTR_IPI_ENET_SYS;
                        }
                    }
                    
                    if (LAST_INTR_IPI_WORKAROUND != ipi_idx &&
                        digi_handle->var.fw_intr_mon_running[ipi_idx] == TRUE)
                    {
                        PMC_MEMSET(enet_int_table.epmm.exco4m.rx_lobl_i, 0, sizeof(enet_int_table.epmm.exco4m.rx_lobl_i));
                        result = digi_enet_int_fw_chnl_exco4m_rx_lobl_i_retrieve(digi_handle,
                                                                                 DIGI_INT_TYPE_ENET_LINE,
                                                                                 TRUE,
                                                                                 FALSE,
                                                                                 &int_en_table_ptr->enet_pmon_int_chnl[i].enet_pmon,
                                                                                 &enet_int_table);
                        if (PMC_SUCCESS == result)
                        {
                            for (ltr = 0; ltr < EXCO4M_N_LANES && FALSE == int_found; ltr += 1)
                            {
                                if (TRUE == int_en_table_ptr->enet_pmon_int_chnl[i].enet_pmon.epmm.exco4m.rx_lobl_i[ltr] &&
                                    0 != enet_int_table.epmm.exco4m.rx_lobl_i[ltr])
                                {
                                    int_found = TRUE;
                                }
                            }
                        }
                    }
                    /* check device interrupt status */
                    if (TRUE != int_found)
                    {
                        result = enet_int_chnl_enabled_check(enet_ptr, 
                                                             link,
                                                             &int_en_table_ptr->enet_pmon_int_chnl[i].enet_pmon,
                                                             &int_found,
                                                             FALSE);
                    }
                }
            }
            if (TRUE == int_found)
            {
                int_summary_ptr->active_enet_pmon_chnl[j] = (digi_ilkn_chnl_t  *)chnl_handle;
                j += 1;
                result = digi_int_callback_run(digi_handle, 
                                               chnl_handle,
                                               int_type);
            }
        }
    }
        
    if (TRUE == int_summary_free_req)
    {
        PMC_FREE(&int_summary_ptr);
    }
    else
    {
        int_summary_ptr->active_enet_pmon_chnl_num = j;

        int_summary_ptr->mask = DIGI_SIFD_ENET_PMON_INT_SUMMARY_MASK_ACTIVE_CHNL_NUM |
            DIGI_SIFD_ENET_PMON_INT_SUMMARY_MASK_ACTIVE_CHNL;
    }


    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_sifd_enet_pmon_int_chnl_enabled_check */



#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */
/*******************************************************************************
* FUNCTION: digi_sifd_int_enabled_check_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in all links of the subsystem.
*
*   The callback associated with the aggregation interrupt type will be called
*   if an active interrupt is found in any link. The callback will be
*   called a minimum of zero times if no active interrupts are found, or
*   a maximum of one time if there is an interrupt in at least one link.
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
PUBLIC PMC_ERROR digi_sifd_int_enabled_check_internal(digi_handle_t      *digi_handle)
{
    PMC_ERROR                result = PMC_SUCCESS;
    digi_int_type_t          int_type = DIGI_INT_TYPE_SIFD;

    sifd_handle_t           *sifd_handle;
    UINT32                   start_link = 0;
    UINT32                   end_link = 0;
    UINT32                   i;
    digi_sifd_int_t         *int_en_table_ptr;
    BOOL                     int_found = FALSE;
    digi_ilkn_link_def_t    *handle_pool_ptr;
    
    PMC_ENTRY();
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && FALSE == digi_sifd_system_required_test(digi_handle))
    {
        PMC_RETURN(PMC_SUCCESS);
    }

    sifd_handle = digi_handle->sifd_handle;
    handle_pool_ptr = digi_handle->handle_pool.ilkn_link;
    
    if (PMC_SUCCESS == result)
    {
        if(!digi_int_is_cb_registered(digi_handle, int_type))
        {
            result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
        }
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
                                       DIGI_INT_TYPE_SIFD,
                                       (void**) &int_en_table_ptr);

    }
    
    
    /* enable interrupt caching, clear cache */
    pmc_sys_int_cache_enable_set(((pmc_handle_t*)digi_handle)->sys_handle, TRUE, TRUE);

    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result) && (FALSE == int_found); i++)
    {
        /* Check enabled interrupts */
        result = sifd_int_link_enabled_check(sifd_handle, 
                                             (sifd_ilkn_inst_t) i,
                                             &int_en_table_ptr->sifd_link[i],
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
} /* digi_sifd_int_enabled_check_internal */

/*******************************************************************************
* FUNCTION: digi_sifd_int_link_enabled_check_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in provided channel of the subsystem.
*   Returns if interrupt is found.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle     - pointer to channel handle to check interrupts
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
PRIVATE PMC_ERROR digi_sifd_int_link_enabled_check_internal(digi_handle_t       *digi_handle,
                                                            digi_ilkn_chnl_t    *chnl_handle,
                                                            BOOL                *int_found_ptr)
{
    PMC_ERROR                result = PMC_SUCCESS;
    digi_int_type_t          int_type = DIGI_INT_TYPE_SIFD;

    sifd_handle_t           *sifd_handle;
    UINT32                   start_link = 0;
    UINT32                   end_link = 0;
    UINT32                   i;
    digi_sifd_int_t         *int_en_table_ptr;
    digi_ilkn_link_def_t    *handle_pool_ptr;
    BOOL8                    multichannels;
    
    PMC_ENTRY();
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && FALSE == digi_sifd_system_required_test(digi_handle))
    {
        PMC_RETURN(PMC_SUCCESS);
    }


    sifd_handle = digi_handle->sifd_handle;
    handle_pool_ptr = digi_handle->handle_pool.ilkn_link;


    /* Determine if are accessing one channel or all channels */
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
    
    
    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result) && (FALSE == *int_found_ptr); i++)
    {
        if (TRUE == multichannels)
        {
            chnl_handle = &handle_pool_ptr[i];
        }

        if (NULL != chnl_handle && 
            ((handle_pool_ptr[i].ilkn_inst ==  DIGI_SIFD_ILKN1 && digi_handle->var.sifd_1_major_mode != DIGI_SIFD_PATH_NOT_IN_USE) ||
             (handle_pool_ptr[i].ilkn_inst ==  DIGI_SIFD_ILKN2 && digi_handle->var.sifd_2_major_mode != DIGI_SIFD_PATH_NOT_IN_USE)))
        {
            /* Check enabled interrupts */
            result = sifd_int_link_enabled_check(sifd_handle, 
                                                 (sifd_ilkn_inst_t) i,
                                                 &int_en_table_ptr->sifd_link[i],
                                                 (BOOL *) int_found_ptr);
        }
    }

    
    PMC_RETURN(result);
} /* digi_sifd_int_link_enabled_check_internal */


/*******************************************************************************
* FUNCTION: digi_sifd_pmon_link_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves a link PMON table with PMON counters
*   for a single link in the subsystem.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   inst_handle     - pointer to link handle to retrieve PMON on
*
* OUTPUTS:
*   pmon_table_ptr  - pointer to link PMON table to store counters 
*                     for a single link
*
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_sifd_pmon_link_retrieve(digi_handle_t           *digi_handle,
                                               digi_ilkn_link_t        *inst_handle,   
                                               digi_sifd_pmon_link_t   *pmon_table_ptr)
{
    PMC_ERROR                result = PMC_SUCCESS;
    digi_sifd_ilkn_inst_t    ilkn_inst;
    digi_ilkn_chnl_def_t    *ilkn_chnl;
    digi_pmon_t             *fw_pmon_ptr;
    UINT32                   i, tbl_index;
   

    PMC_ENTRY();
    
    tbl_index = 0;
   
    result = digi_pmon_chnl_param_check(digi_handle, inst_handle, pmon_table_ptr); 



    if (PMC_SUCCESS == result)
    {
        result = digi_pmon_shared_mem_get(digi_handle, &fw_pmon_ptr); 
    }

    if (PMC_SUCCESS == result)
    {        
        ilkn_inst = digi_sifd_inst_get(digi_handle, inst_handle);

        PMC_MEMCPY(&pmon_table_ptr->pmon,
                   &fw_pmon_ptr->sifd_pmon[ilkn_inst],
                   sizeof(digi_pmon_sifd_t));
    }

    /* Retrieve Pool of ILKN link handles */
    if (PMC_SUCCESS == result)
    {
        switch (ilkn_inst)
        {
        case DIGI_SIFD_ILKN1:
            ilkn_chnl = digi_handle->handle_pool.ilkn_1_chnl;
            break;
            
        case DIGI_SIFD_ILKN2:
            ilkn_chnl = digi_handle->handle_pool.ilkn_2_chnl;
            break;
        
        default:
            PMC_ASSERT(ilkn_inst < LAST_DIGI_SIFD_ILKN, DIGI_ERR_INVALID_ARG, ilkn_inst, LAST_DIGI_SIFD_ILKN);
            break;
        } /* end switch */
    }

    /* Find ILKN links that have enh ENET PMON provisioned */
    for (i = 0; (i < DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX) && (PMC_SUCCESS == result); i++)
    {
        PMC_ASSERT(tbl_index < DIGI_ENET_CHNL_MAX, DIGI_ERR_INVALID_ARG, tbl_index, DIGI_ENET_CHNL_MAX);
        /* Copy enhanced PMON tables and mode if provisioned on this ILKN link */
        result = digi_enet_enhanced_pmon_chnl_retrieve_internal(digi_handle,
                                                                &ilkn_chnl[i],
                                                                &pmon_table_ptr->enet[tbl_index],
                                                                &pmon_table_ptr->enet_mode[tbl_index]);
        
        if (DIGI_ENET_PMON_MODE_NONE != pmon_table_ptr->enet_mode[tbl_index])
        {
            pmon_table_ptr->ilkn_handle[tbl_index] = &ilkn_chnl[i];
            tbl_index++;
        }
    } /* end for */

    PMC_RETURN(result);
} /* digi_sifd_pmon_link_retrieve */


/*******************************************************************************
* digi_sifd_inst_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function gets the instance number.
*
* INPUTS:
*digi_handle           - pointer to DIGI handle instance
*   *inst_handle           - pointer to link handle.
*
* OUTPUTS:
*   none
*
* RETURNS:
*   sifd_inst           - SIFD instance number
*
* NOTES:
*
*******************************************************************************/
PUBLIC digi_sifd_ilkn_inst_t digi_sifd_inst_get(digi_handle_t      *digi_handle,
                                                digi_ilkn_link_t   *inst_handle)
{
    digi_ilkn_link_def_t    *ilkn_link_data_ptr;
    digi_sifd_ilkn_inst_t    ilkn_inst;

    PMC_ENTRY();
   
    /* check inputs */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != inst_handle, DIGI_ERR_INVALID_ARG, 0, 0);
   

    ilkn_link_data_ptr = (digi_ilkn_link_def_t*)inst_handle;
    ilkn_inst = ilkn_link_data_ptr->ilkn_inst;


    PMC_RETURN(ilkn_inst);

} /* digi_sifd_inst_get */


/*******************************************************************************
* digi_pbrs_mode_to_uint_conv 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert pattern mode defined defined in util global to sifd pattern mode enum.
*
*
* INPUTS:
*  util_patt_mode         - util global pattern mode enum
*
* OUTPUTS:
*   *pattern              - UINT PRBS pattern
*
* RETURNS:
*   PMC_SUCCESS           - when conversion is done 
*                           otherwise a descriptive error is returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_pbrs_mode_to_uint_conv(util_patt_mode_t util_patt_mode,
                                              UINT32 *pattern)
{
    PMC_ENTRY();

    switch (util_patt_mode)
    {
        case UTIL_PATT_MODE_PRBS_7:
            *pattern = (UINT32)SIFD_PATT_MODE_PRBS_7;
            break;  
        case UTIL_PATT_MODE_PRBS_11:
            *pattern = (UINT32)SIFD_PATT_MODE_PRBS_11;
            break;
        case UTIL_PATT_MODE_PRBS_15:
            *pattern = (UINT32)SIFD_PATT_MODE_PRBS_15;
            break;
        case UTIL_PATT_MODE_PRBS_23:
            *pattern = (UINT32)SIFD_PATT_MODE_PRBS_23;
            break;
        case UTIL_PATT_MODE_PRBS_31:
            *pattern = (UINT32)SIFD_PATT_MODE_PRBS_31;
            break;
        case UTIL_PATT_MODE_PRBS_9:
            *pattern = (UINT32)SIFD_PATT_MODE_PRBS_9;
            break; 
        case UTIL_PATT_MODE_PRBS_29:
            *pattern = (UINT32)SIFD_PATT_MODE_PRBS_29;
            break; 
        default:
            PMC_RETURN(SIFD_LOG_ERR_PRBS_MODE_NOT_FOUND);
    }

    PMC_RETURN(PMC_SUCCESS);
        
}/*digi_pbrs_mode_to_uint_conv*/

/*******************************************************************************
* digi_uint_to_pbrs_mode_conv 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert pattern mode defined defined in util global to sifd pattern mode enum.
*
*
* INPUTS:
*   pattern               - UINT PRBS pattern
*
* OUTPUTS:
*   *util_patt_mode       - util global pattern mode enum
*
* RETURNS:
*   PMC_SUCCESS           - when conversion is done 
*                           otherwise a descriptive error is returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_uint_to_pbrs_mode_conv(UINT32 pattern,
                                              util_patt_mode_t *util_patt_mode)
{
    PMC_ENTRY();

    switch (pattern)
    {
        case SIFD_PATT_MODE_PRBS_7:
            *util_patt_mode = UTIL_PATT_MODE_PRBS_7;
            break;  
        case SIFD_PATT_MODE_PRBS_11:
            *util_patt_mode = UTIL_PATT_MODE_PRBS_11;
            break;
        case SIFD_PATT_MODE_PRBS_15:
            *util_patt_mode = UTIL_PATT_MODE_PRBS_15;
            break;
        case SIFD_PATT_MODE_PRBS_23:
            *util_patt_mode = UTIL_PATT_MODE_PRBS_23;
            break;
        case SIFD_PATT_MODE_PRBS_31:
            *util_patt_mode = UTIL_PATT_MODE_PRBS_31;
            break;
        case SIFD_PATT_MODE_PRBS_9:
            *util_patt_mode = UTIL_PATT_MODE_PRBS_9;
            break; 
        case SIFD_PATT_MODE_PRBS_29:
            *util_patt_mode = UTIL_PATT_MODE_PRBS_29;
            break; 
        default:
            PMC_RETURN(SIFD_LOG_ERR_PRBS_MODE_NOT_FOUND);
    }

    PMC_RETURN(PMC_SUCCESS);
        
}/*digi_uint_to_pbrs_mode_conv*/

/*******************************************************************************
* digi_sifd_burst_min_conv
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   convert a minimal burst size from digi_sifd_burst_min_t type to sifd_burst_min_t.
*                                                                               
*                                                                               
* INPUTS:                                                                       
* digi_burst_min            - ILKN minimal burst size encoded in digi_sifd_burst_min_t type.
*                                                                               
* OUTPUTS:
* sifd_burst_min            - ILKN minimal burst size encoded in sifd_burst_min_t type.
*
* RETURNS:
*   PMC_SUCCESS when operation is successful otherwise a descriptive error code is returned.
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_burst_min_conv(digi_sifd_burst_min_t  digi_burst_min,
                                          sifd_burst_min_t      *sifd_burst_min)
    
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ASSERT(NULL != sifd_burst_min, DIGI_ERR_INVALID_ARG, 0, 0);

    PMC_ENTRY();

    switch (digi_burst_min)
    {
    case DIGI_SIFD_BURST_MIN_32:
        *sifd_burst_min = SIFD_BURST_MIN_32;
        break;
    case DIGI_SIFD_BURST_MIN_64:
        *sifd_burst_min = SIFD_BURST_MIN_64;
        break;
    case DIGI_SIFD_BURST_MIN_DISABLE: 
        *sifd_burst_min = SIFD_BURST_MIN_DISABLE;
        break;
    default:
        rc = DIGI_ERR_INVALID_ARG;
        break;
    }

    PMC_RETURN(rc);
}/* digi_sifd_burst_min_conv */


/*******************************************************************************
* digi_sifd_enet_pmon_int_chnl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clear, Enable, or Disable interrupts based on bits that are set in the SIFD ILKN channel
*   ENET PMON interrupt table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to enable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to enable interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to channel ENET interrupt table with bits set to
*                     enable interrupts
*   clear           - when TRUE interrupts are cleared and enable value is ignored
*                     when FALSE enable value is considered
*   enable          - when TRUE interrupts active in int_table_ptr are enabled
*                     otherwise they are disabled
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_sifd_enet_pmon_int_chnl_cfg(digi_handle_t             *digi_handle,
                                                   digi_ilkn_chnl_t          *chnl_handle,
                                                   enet_int_chnl_t           *int_table_ptr,
                                                   BOOL8                      clear,
                                                   BOOL8                      enable)
{
    PMC_ERROR                result = PMC_SUCCESS;
    digi_int_type_t          int_type = DIGI_INT_TYPE_SIFD;

    UINT32                   start_link = 0;
    UINT32                   end_link = 0;
    UINT32                   i;
    UINT32                   j;
    digi_sifd_int_t         *int_en_table_ptr;
    digi_ilkn_chnl_def_t    *handle_pool_ptr;
    digi_ilkn_chnl_def_t    *chnl_ptr;
    enet_handle_t           *enet_ptr;
    UINT32                   link;

    PMC_ENTRY();

    result = digi_int_param_check(digi_handle, chnl_handle, int_type, int_table_ptr);         

    /* Callback for interrupt table must be registered before enable ints */
    if (PMC_SUCCESS == result)
    {
        if(!digi_int_is_cb_registered(digi_handle, DIGI_INT_TYPE_SIFD) &&
           !digi_int_is_cb_registered(digi_handle, DIGI_INT_CHNL_TYPE_SIFD_ENET_PMON))
        {
            result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
        }
    }


    /* Determine if are modfiying one channel or all channels */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         (void*) chnl_handle,
                                         DIGI_INT_CHNL_TYPE_SIFD_ENET_PMON,
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
        for (j = 0; j < DIGI_SIFD_ILKN_NUM; j += 1)
        {
            chnl_ptr = NULL;
            if (IS_DIGI_INT_ALL_CHNLS(chnl_handle))
            {
                if (0 == j)
                {
                    handle_pool_ptr = digi_handle->handle_pool.ilkn_1_chnl;
                }
                else
                {
                    handle_pool_ptr = digi_handle->handle_pool.ilkn_2_chnl;
                }
                chnl_ptr = &handle_pool_ptr[i];
            }
            else
            {
                if (PMC_SUCCESS == digi_ilkn_chnl_handle_validate(digi_handle, chnl_ptr))
                {
                    chnl_ptr = (digi_ilkn_chnl_def_t*)chnl_handle;
                }
                else
                {
                    chnl_ptr = NULL;
                }
            }
            /* Process ENET PMON interrupts to enable */     
            if (NULL != chnl_ptr &&
                util_global_is_chnl_prov(chnl_ptr))
            {  
                result = digi_sifd_enet_pmon_handle_get(digi_handle,
                                                        chnl_ptr,
                                                        &enet_ptr,
                                                        &link);
                
                /* Enable enet interrupts */
                if (PMC_SUCCESS == result &&
                    NULL != enet_ptr)
                {
                    /* Skip interrupt retrieve if subsystem is not powered up */
                    if (PMC_SUCCESS == result && TRUE == enet_start_state_test(enet_ptr))
                    {
                        continue;
                    }

                    /* execute operation: enable/disable or clear */
                    if (FALSE == clear)
                    {
                        result = enet_int_chnl_enable(enet_ptr, 
                                                      link,
                                                      int_table_ptr,
                                                      &int_en_table_ptr->enet_pmon_int_chnl[link].enet_pmon,
                                                      enable,
                                                      FALSE);
                        if(PMC_SUCCESS == result)
                        {
                            int_en_table_ptr->enet_pmon_int_chnl[link].chnl_handle = (digi_ilkn_chnl_t  *)chnl_ptr;
                        }
                    }
                    else
                    {
                        result = enet_int_chnl_clear(enet_ptr, 
                                                     link,
                                                     NULL != int_table_ptr ? int_table_ptr : &int_en_table_ptr->enet_pmon_int_chnl[link].enet_pmon,
                                                     &int_en_table_ptr->enet_pmon_int_chnl[link].enet_pmon,
                                                     FALSE);
                    }
                }
            }
            if (FALSE == IS_DIGI_INT_ALL_CHNLS(chnl_handle))
            {
                break;
            }
        }
        if (FALSE == IS_DIGI_INT_ALL_CHNLS(chnl_handle))
        {
            break;
        }
    }    
        
    PMC_RETURN(result);
} /* digi_sifd_enet_pmon_int_chnl_cfg */

/*******************************************************************************
* digi_sifd_chnl_index_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns sifd ilkn channel index in digi handle pool.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle
*
* OUTPUTS:
*   *index_ptr       - sifd ilkn channel index in digi handle pool.
*
* RETURNS:
*   PMC_SUCCESS if index is returned successfully, otherwise a desciptive
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_sifd_chnl_index_get(digi_handle_t       *digi_handle,
                                          digi_ilkn_chnl_t    *chnl_handle,
                                          UINT32              *index_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 i;

    PMC_ENTRY();

    *index_ptr = DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX;

    for (i = 0; i < DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX; i++)
    {
        if (chnl_handle == (digi_ilkn_chnl_t *)&digi_handle->handle_pool.ilkn_1_chnl[i])
        {
            *index_ptr = i;
            break;
        }
        if (chnl_handle == (digi_ilkn_chnl_t *)&digi_handle->handle_pool.ilkn_2_chnl[i])
        {
            *index_ptr = i;
            break;
        }
    }
    if(DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX == *index_ptr)
    {
        rc = DIGI_ERR_INVALID_ARG;
    }
    PMC_RETURN(rc);
}/* digi_sifd_chnl_index_get */

 
/*******************************************************************************
* digi_ilkn_chnl_is_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Check if (possible) ILKN channel associated to input or output channels 
*   are provisoned.
*   
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   *input_chnl_ptr        - possible ILKN RX channel.
*   *output_chnl_ptr       - possible ILKN TX channel.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Returns TRUE when ILKN channel associated to input or output 
*   channels are provisoned.
*   It also returns TRUE when input AND output channel do no correspond to
*   ILKN channel.
*   Otherwise FALSE is returned when ILKN RX or TX channel is/are not provisioned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 digi_ilkn_chnl_is_prov(digi_handle_t              *digi_handle,
                                    util_global_switch_data_t  *input_chnl_ptr,
                                    util_global_switch_data_t  *output_chnl_ptr)
{
    BOOL8 is_prov = TRUE;
    PMC_ERROR rc = PMC_SUCCESS;
    digi_ilkn_chnl_def_t *chnl_data_ptr;

    PMC_ENTRY();

    /* check possible ILKN RX channel provioning status */
    if (NULL != input_chnl_ptr &&
        PMC_SUCCESS == digi_ilkn_chnl_handle_validate(digi_handle, input_chnl_ptr))
    {   
        chnl_data_ptr = (digi_ilkn_chnl_def_t*)input_chnl_ptr;
        if (PMC_SUCCESS == rc)
        {
            if (FALSE == util_global_chnl_prov_status_get(chnl_data_ptr->switch_data.header.prov_state, UTIL_GLOBAL_DIR_RX))
            {
                is_prov = FALSE;
            }
        }
        else
        {
            is_prov = FALSE;
        }
    }

    /* check possible ILKN TX channel provioning status */
    if (NULL != output_chnl_ptr &&
        PMC_SUCCESS == digi_ilkn_chnl_handle_validate(digi_handle, output_chnl_ptr))
    {   
        chnl_data_ptr = (digi_ilkn_chnl_def_t*)output_chnl_ptr;
       if (PMC_SUCCESS == rc)
        {
            if (FALSE == util_global_chnl_prov_status_get(chnl_data_ptr->switch_data.header.prov_state, UTIL_GLOBAL_DIR_TX))
            {
                is_prov = FALSE;
            }
        }
        else
        {
            is_prov = FALSE;
        }
    }

    PMC_RETURN(is_prov);
}/* digi_ilkn_chnl_is_prov */

/*******************************************************************************
* digi_ilkn_inst_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns ILKN instance assocaited to given channel.
*   
*
* INPUTS:
*   *chnl_ctxt_ptr         - a pointer to a pointer to a digi_ilkn_chnl_t
*                            structure consisting of a channel data this is
*                            set to NULL for the user.
*
* OUTPUTS:
*   *sifd_ilkn_inst_ptr    - reference to ILKN instance associated to channel.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ilkn_inst_get(digi_ilkn_chnl_def_t   *chnl_ctxt_ptr,
                                    digi_sifd_ilkn_inst_t  *sifd_ilkn_inst_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY(); 

    if (NULL == sifd_ilkn_inst_ptr)
    {
        rc = DIGI_ERR_INVALID_ARG;
    }
    else
    {
        *sifd_ilkn_inst_ptr = LAST_DIGI_SIFD_ILKN;        

        switch(chnl_ctxt_ptr->switch_data.cpb_data.port_type)
        {
            case UTIL_GLOBAL_CPB_PORT_ILKN1:
                *sifd_ilkn_inst_ptr = DIGI_SIFD_ILKN1;
            break;
            case UTIL_GLOBAL_CPB_PORT_ILKN2:
                *sifd_ilkn_inst_ptr = DIGI_SIFD_ILKN2;
                break;
            default: 
                rc = DIGI_ERR_INVALID_ARG;
                break;
        }  

        if (PMC_SUCCESS != rc)
        { 
            rc = PMC_SUCCESS;
            switch(chnl_ctxt_ptr->switch_data.oduk_data.port_type)
            {
            case UTIL_GLOBAL_ODUK_PORT_ILKN1:
                *sifd_ilkn_inst_ptr = DIGI_SIFD_ILKN1;
                break;
            case UTIL_GLOBAL_ODUK_PORT_ILKN2:
                *sifd_ilkn_inst_ptr = DIGI_SIFD_ILKN2;
            break;
            default: 
                rc = DIGI_ERR_INVALID_ARG;
                break;
            }  
        }
    }
    
    PMC_RETURN(rc);  
}/* digi_ilkn_inst_get */

/*******************************************************************************
* digi_ilkn_chnl_prov_state_new_val_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns ILKN channel new state.
*   
*
* INPUTS:
*   cur_state           - ILKN channel current state.
*   use_rx_chnl_state   - when TRUE RX direction new provisoning state is considered
*                         to determine channel new state. Otherwise it is not.
*   prov_rx_chnl        - RX direction new provisioning state
*                          TRUE (provisioned, FALSE: unprov)
*   use_tx_chnl_state   - when TRUE TX direction new provisoning state is considered
*                         to determine channel new state. Otherwise it is not.
*   prov_tx_chnl        - TX direction new provisioning state:
*                          TRUE (provisioned, FALSE: unprov)
*
* OUTPUTS:
*   None
*
* RETURNS:
*   New state value.
*
* NOTES:
*
*******************************************************************************/
PUBLIC util_global_chnl_provisioned_state_t digi_ilkn_chnl_prov_state_new_val_get(util_global_chnl_provisioned_state_t  cur_state,
                                                                                  BOOL8                                 use_rx_chnl_state,
                                                                                  BOOL8                                 prov_rx_chnl,
                                                                                  BOOL8                                 use_tx_chnl_state,
                                                                                  BOOL8                                 prov_tx_chnl)
{
    util_global_chnl_provisioned_state_t new_st = cur_state;

    PMC_ENTRY();

    if (TRUE == use_rx_chnl_state)
    {
        if (TRUE == prov_rx_chnl)
        {
            switch (new_st)
            {
            case UTIL_GLOBAL_CHNL_PROV_STATE_CONFIG:
                if (TRUE == use_tx_chnl_state && TRUE == prov_tx_chnl)
                {
                    new_st = UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED;
                }
                else
                {
                    new_st = UTIL_GLOBAL_CHNL_PROV_STATE_RX_PROVISIONED;                
                }            
                break;  
            case UTIL_GLOBAL_CHNL_PROV_STATE_TX_PROVISIONED:
                new_st = UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED;    
                break;
            default:
                break;
            }
        }
        else
        {
            switch (new_st)
            {
            case UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED:
                new_st = UTIL_GLOBAL_CHNL_PROV_STATE_TX_PROVISIONED;  
                break; 
            case UTIL_GLOBAL_CHNL_PROV_STATE_RX_PROVISIONED:
                new_st = UTIL_GLOBAL_CHNL_PROV_STATE_CONFIG;
                break; 
            default:
                break;
            }
        }
    }

    if (TRUE == use_tx_chnl_state)
    {
        if (TRUE == prov_tx_chnl)
        {
            switch (new_st)
            {
            case UTIL_GLOBAL_CHNL_PROV_STATE_CONFIG:
                /*if use_rx_chnl_state == TRUE and prov_rx_chnl == TRUE,
                then state will have progressed past UTIL_GLOBAL_CHNL_PROV_STATE_CONFIG*/
                new_st = UTIL_GLOBAL_CHNL_PROV_STATE_TX_PROVISIONED;  
                break;  
            case UTIL_GLOBAL_CHNL_PROV_STATE_RX_PROVISIONED:
                new_st = UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED;    
                break;
            default:
                break;
            }
        }
        else
        {
            switch (new_st)
            {
            case UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED:
                new_st = UTIL_GLOBAL_CHNL_PROV_STATE_RX_PROVISIONED;  
                break; 
            case UTIL_GLOBAL_CHNL_PROV_STATE_TX_PROVISIONED:
                new_st = UTIL_GLOBAL_CHNL_PROV_STATE_CONFIG;
                break; 
            default:
                break;
            }
        }
    }
    PMC_RETURN(new_st);  
}/* digi_ilkn_chnl_prov_state_new_val_get */


/*******************************************************************************
* digi_sifd_fc_calendar_bulk_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function updates the SIFD's flow control calendar for ILKN links that
*   are connected to the ODUKSW.
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance.
*   oduksw_port_num         - The number of the ODUKSW port that the ILKN is connected to.
*   port_direction          - TRUE: port direction is TX.
*                             FALSE: port direction is RX.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_sifd_fc_calendar_bulk_update(digi_handle_t *digi_handle,
                                              util_global_oduk_port_t oduksw_port_num,
                                              util_global_datapath_dir_t port_direction)
{
    PMC_ERROR result = PMC_SUCCESS;
    sifd_ilkn_inst_t ilkn_inst = LAST_SIFD_ILKN;
    BOOL8 is_tx_dir = FALSE;

    PMC_ENTRY();

    PMC_ASSERT(UTIL_GLOBAL_ODUK_PORT_ILKN1 == oduksw_port_num || UTIL_GLOBAL_ODUK_PORT_ILKN2 == oduksw_port_num,
               DIGI_ERR_INVALID_ARG, 0, 0);

    if (UTIL_GLOBAL_ODUK_PORT_ILKN1 == oduksw_port_num)
    {
        ilkn_inst = SIFD_COPI_ILKN1;
    }
    else
    {
        /*UTIL_GLOBAL_ODUK_PORT_ILKN2 == oduksw_port_num*/
        ilkn_inst = SIFD_COPI_ILKN2;
    }

    if (UTIL_GLOBAL_DATAPATH_DIR_EGRESS == port_direction)
    {
        is_tx_dir = TRUE;
    }
    else
    {
        is_tx_dir = FALSE;
    }

    result = digi_sifd_fc_calendar_update(digi_handle,
                                          ilkn_inst,
                                          oduksw_port_num,
                                          is_tx_dir,
                                          TRUE, /*is_oduksw*/
                                          SIFD_ILKN_NULL_CAL_ENTRY);
    PMC_ASSERT(PMC_SUCCESS == result, result, 0, 0);

    PMC_RETURN();
}/*digi_sifd_fc_calendar_bulk_update*/

/*******************************************************************************
* digi_cpb_ilkn_dcs_swap_force
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function performs a forced DCS page swap.  This operation applies to
*   ILKN connected DCS only.\n\n
*
*   This API is intended for usage when DCS swap fails due to ILKN link XOFF
*   state preventing the DCS from returning to top of calendar.
*
* INPUTS:
*   *digi_handle_ptr            - pointer to digi_handle_t handle instance
*   *cpb_handle             - pointer to cpb_handle_t handle instance
*   ilkn_inst               - Interlaken instance within SIFD
*
* OUTPUT:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_cpb_ilkn_dcs_swap_force(void *digi_handle_ptr,
                                         void *cpb_handle, 
                                         UINT32 ilkn_inst)
{
    digi_handle_t *digi_handle = (digi_handle_t *)digi_handle_ptr;

    sifd_ilkn_en_t sifd_tx_fc_en = SIFD_ILKN_DISABLE;
    sifd_ilkn_inst_t sifd_ilkn_link = (0==ilkn_inst)? SIFD_COPI_ILKN1:SIFD_COPI_ILKN2;
    BOOL8 oob_type = FALSE;
    digi_sifd_fc_mode_t fc_mode;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL!=digi_handle, DIGI_ERR_CODE_ASSERT, 0, 0);
    PMC_ASSERT(NULL!=cpb_handle, DIGI_ERR_CODE_ASSERT, 0, 0);

    fc_mode = (ilkn_inst == SIFD_COPI_ILKN1) ? digi_handle->var.sifd_ilkn_1_cfg.ilkn.fc_mode : digi_handle->var.sifd_ilkn_2_cfg.ilkn.fc_mode;
    /* confirm the DCS instance is connected to ILKN, if not return as there
       is nothing to do */


    if(fc_mode > DIGI_SIFD_FC_MODE_NONE && fc_mode < LAST_DIGI_SIFD_FC_MODE)
    {
        /* Get SIFD TX_IB_EN/TX_OOB_EN value */
        if(fc_mode == DIGI_SIFD_FC_MODE_IN_BAND_16 
            || fc_mode == DIGI_SIFD_FC_MODE_IN_BAND_24)
        {
            oob_type = FALSE;
        } 
        else if(fc_mode == DIGI_SIFD_FC_MODE_OUT_OF_BAND)
        {
            oob_type = TRUE;
        }
        sifd_tx_fc_en_get(((digi_handle_t*)digi_handle)->sifd_handle,
                          sifd_ilkn_link,
                          oob_type,
                          &sifd_tx_fc_en);
        sifd_tx_fc_en_set(((digi_handle_t*)digi_handle)->sifd_handle,
                          sifd_ilkn_link, oob_type, SIFD_ILKN_DISABLE);
    }

    (void)cpb_dcs_swap_poll(((cpb_handle_t*)cpb_handle), ilkn_inst);

    if(fc_mode > DIGI_SIFD_FC_MODE_NONE && fc_mode < LAST_DIGI_SIFD_FC_MODE)
    {
        sifd_tx_fc_en_set(((digi_handle_t*)digi_handle)->sifd_handle,
                          sifd_ilkn_link, oob_type, sifd_tx_fc_en);
    }

    PMC_RETURN();
} /* digi_cpb_ilkn_dcs_swap_force */

#endif

/*
** End of file
*/
