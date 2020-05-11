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
*   DESCRIPTION : This file contains helper functions associated with OPSA
*                 related configuration.
*
*  NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "digi_loc.h"

#ifndef DOXYGEN_PUBLIC_ONLY
PRIVATE void digi_oduksw_chnl_qs_ch_en_get(digi_handle_t *digi_handle,
                                           util_global_switch_data_t *output_pc_ptr,
                                           UINT8 *qs_ch_en_ptr);

PRIVATE void digi_oduksw_chnl_qs_ch_en_get_all(digi_handle_t *digi_handle,
                                               UINT32 output_port,
                                               UINT8 *qs_ch_en_ptr);

PRIVATE void digi_opsa_cfg_mpma_one_port_add(digi_handle_t *digi_handle,
                                             hostmsg_opsa_subsystem_cfg_t subsystem,
                                             UINT32 chnl,
                                             UINT8 qs_ch_en,
                                             BOOL8 is_config_only);

PRIVATE void digi_opsa_cfg_mpma_one_port_drop(digi_handle_t *digi_handle,
                                              hostmsg_opsa_subsystem_cfg_t subsystem,
                                              UINT32 chnl);

PRIVATE void digi_opsa_cfg_mpma_all_chnl_add(digi_handle_t *digi_handle,
                                             hostmsg_opsa_subsystem_cfg_t subsystem,
                                             UINT32 input_chnl_list[96],
                                             UINT8 *qs_ch_en);

PRIVATE void digi_opsa_cfg_mpma_all_chnl_drop(digi_handle_t *digi_handle,
                                              hostmsg_opsa_subsystem_cfg_t subsystem,
                                              UINT32 chnl_list[96]);

PRIVATE void digi_opsa_cpb_config_nominal(digi_handle_t *digi_handle,
                                          util_global_switch_data_def_t *output_data_ptr);


PRIVATE PMC_ERROR digi_opsa_oduksw_config_nominal(digi_handle_t *digi_handle,
                                                  util_global_switch_data_def_t *output_data_ptr,
                                                  odu_struct_t *odu_ptr);

#endif /* DOXYGEN_PUBLIC_ONLY */

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*
** Exported Interface Function
*/

/*******************************************************************************
* digi_opsa_fifo_cent_trigger
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to FW to trigger FIFO centering for the specified set
*  of channels in COREOTN_FO1. 
*
* INPUTS:
*   *digi_handle  - pointer to DIGI handle instance.
*   num_chnl      - number of channels to force fifo centering
*   *chnl_handle  - pointer to an array of channel handles with length num_chnl
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_opsa_fifo_cent_trigger(digi_handle_t *digi_handle,
                                             UINT8 num_chnl,
                                             util_global_switch_data_t **chnl_handle)
{

    UINT32 chnl_add_msk[3] = {0, 0, 0};
    UINT32 i;

    util_global_switch_data_def_t *output_pc_data_ptr = NULL;
    util_global_oduk_port_data_t *local_output_pc_ptr = NULL;

    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(num_chnl <= 96, DIGI_ERR_CODE_ASSERT, 0, 0);
    PMC_ASSERT(chnl_handle != NULL, DIGI_ERR_CODE_ASSERT, 0, 0);

    for(i = 0; i < num_chnl; i++)
    {
        DIGI_SWITCH_DATA_HANDLE_CHECK(digi_handle, chnl_handle[i], result);
        if (PMC_SUCCESS == result)
        {
            output_pc_data_ptr = (util_global_switch_data_def_t *)(&(chnl_handle[i]));
            PMC_ASSERT(output_pc_data_ptr != NULL, DIGI_ERR_CODE_ASSERT, 0, 0);
            
            DIGI_CHNL_HANDLE_MAGIC_CHECK(output_pc_data_ptr);
            
            local_output_pc_ptr = &(output_pc_data_ptr->oduk_data);
            PMC_ASSERT(local_output_pc_ptr->port_type == UTIL_GLOBAL_ODUK_PORT_COREOTN, DIGI_ERR_CODE_ASSERT, 0, 0);
            
            digi_opsa_chnl_to_chnl_msk(local_output_pc_ptr->channel, chnl_add_msk);
        }
    }

    if (PMC_SUCCESS == result)
    {
        result = digi_fw_opsa_fifo_cent_trigger(digi_handle,
                                                chnl_add_msk);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_opsa_fifo_cent_trigger */

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* digi_opsa_oduksw_activate_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an internal helper function to determine the presenece of MPMA and
*   MPMO instances in a datapath.  If they are present, FW host message to
*   activate these is applied.
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   *input_pc_ptr         - util_global_switch_data_t pointer defining inbound 
*                           port
*   *output_pc_ptr        - util_global_switch_data_t pointer defining outbound 
*                           port
*   is_reader_primary     - indicator if the channel is primary reader for queue
*                           system
*   is_slave_zone_primary - indicator if the channel is the zone primary of all
*                           channels for this slave port
*   cfg_action            - configuration operation to be performed by this API.
*   activate_deactivate_mpmo - TRUE : activate or deativate MPMO OPSA FW segment
*                           FALSE : do not activate or deactivate the MPMO OPSA 
*                                   FW segement
*   is_config_only    - Determines the FW OPSA message to be sent to FW for
*                       the MPMO/MPMA add operations. A value of FALSE
*                       default HOSTMSG_CMD_CFG_OPSA_MPMA_PORT_ADD or
*                       HOSTMSG_CMD_CFG_OPSA_MPMO_PORT_ADD_ST or if TRUE
*                       HOSTMSG_CMD_CFG_OPSA_MPMO_CTXT_CFG or
*                       HOSTMSG_CMD_CFG_OPSA_MPMA_CTXT_CFG
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_opsa_oduksw_activate_cfg(digi_handle_t *digi_handle,
                                          util_global_switch_data_t *input_pc_ptr,
                                          util_global_switch_data_t *output_pc_ptr,
                                          BOOL8 is_reader_primary,
                                          BOOL8 is_slave_zone_primary,
                                          digi_opsa_cfg_action_t cfg_action,
                                          BOOL activate_deactivate_mpmo,
                                          BOOL8 is_config_only)
{

    digi_serdes_intf_t intf;

    util_global_switch_data_def_t *input_pc_data_ptr = (util_global_switch_data_def_t *)input_pc_ptr;
    util_global_switch_data_def_t *output_pc_data_ptr = (util_global_switch_data_def_t *)output_pc_ptr;

    util_global_oduk_port_data_t *local_input_pc_ptr = &input_pc_data_ptr->oduk_data;
    util_global_oduk_port_data_t *local_output_pc_ptr = &output_pc_data_ptr->oduk_data;

    hostmsg_opsa_subsystem_cfg_t output_mpmo_ss = LAST_OPSA_HOSTMSG_SS; /* initialize to illegal value */
    hostmsg_opsa_subsystem_cfg_t input_mpma_ss = LAST_OPSA_HOSTMSG_SS; /* initialize to illegal value */
    hostmsg_opsa_subsystem_cfg_t input_mpmo_ss = LAST_OPSA_HOSTMSG_SS; /* initialize to illegal value */
    digi_opsa_mpmo_target_t target = LAST_DIGI_OPSA_MPMO_TARGET;
    util_global_mapping_mode_t mapotn_mapping_mode = UTIL_GLOBAL_NO_MAP;
    UINT8 acb_inst = 0, ckctl_port_addr = 0;
    BOOL8 mpmo_cfg_req = FALSE;

    UINT8 qs_ch_en = 0, qs_ch_en_to_dis = 0;

    cpb_handle_t * ocpb_handle = coreotn_oduksw_handle_get(digi_handle->coreotn_handle);

    mapotn_src_dest_t mapotn_src = LAST_MAPOTN_DEST;
    mapotn_src_dest_t mapotn_dest = LAST_MAPOTN_DEST;
    util_global_map_adapt_func_t adapt_mode = UTIL_GLOBAL_MAP_NO_RATE;
    util_global_map_adapt_func_t dummy_adapt_mode = UTIL_GLOBAL_MAP_NO_RATE;

    PMC_ERROR result = PMC_SUCCESS;
    BOOL8 mpmo_proc_type;

    PMC_ENTRY();

    PMC_ASSERT(cfg_action < LAST_DIGI_OPSA_CFG_ACTION, DIGI_ERR_CODE_ASSERT, 0, 0);
        
    input_pc_data_ptr = (util_global_switch_data_def_t *)input_pc_ptr;
    output_pc_data_ptr = (util_global_switch_data_def_t *)output_pc_ptr;

    local_input_pc_ptr = &(input_pc_data_ptr->oduk_data);
    local_output_pc_ptr = &(output_pc_data_ptr->oduk_data);


    /* determine the details of the input port */
    switch(local_input_pc_ptr->port_type)
    {
        case UTIL_GLOBAL_ODUK_PORT_ILKN1:
            if(digi_handle->var.prod_app == DIGI_PROD_APP_SYSOTN_CARD)
            {
                /* ODUKSW port used by LINEOTN -> HO -> ODUKSW */
                input_mpma_ss = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_CTL;
            } else {
                /* ODUKSW port used by ILKN1 - no MPMA to configure */
            }
            break;
        case UTIL_GLOBAL_ODUK_PORT_ILKN2:
            /* ODUKSW port used by ILKN2 - no MPMA to configure */
            break;
        case UTIL_GLOBAL_ODUK_PORT_COREOTN:
            input_mpma_ss = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_FO1;
            break;
        case UTIL_GLOBAL_ODUK_PORT_MAPOTN:
            input_mpma_ss = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_FO2;
            result = mapotn_mapper_mpmo_cfg_get(digi_handle->mapotn_handle, local_input_pc_ptr->channel, &mpmo_cfg_req);
            PMC_ASSERT(result==PMC_SUCCESS, DIGI_ERR_CODE_ASSERT, 0, 0);
            digi_mapper_src_dest_get(digi_handle,
                                     digi_handle->handle_pool.mapper_chnl[local_input_pc_ptr->channel].mode,
                                     digi_handle->handle_pool.mapper_chnl[local_input_pc_ptr->channel].enet_pmon,
                                     &mapotn_src,
                                     &mapotn_dest);
            digi_mapotn_mode_params_convert(digi_handle->handle_pool.mapper_chnl[local_input_pc_ptr->channel].mode,
                                            &adapt_mode,
                                            &dummy_adapt_mode);
            result = mapotn_mapping_mode_get(digi_handle->mapotn_handle, 
                                             adapt_mode,
                                             &mapotn_mapping_mode);

            PMC_ASSERT(result==PMC_SUCCESS, DIGI_ERR_CODE_ASSERT, 0, 0);

            if(mpmo_cfg_req == TRUE && mapotn_src == MAPOTN_DEST_CPB && mapotn_mapping_mode != UTIL_GLOBAL_BMP) input_mpmo_ss = OPSA_HOSTMSG_CPB_SS_MAPOTN;
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
    }

    /* determine the details of the output port */
    switch(local_output_pc_ptr->port_type)
    {
        case UTIL_GLOBAL_ODUK_PORT_ILKN1:
            if(digi_handle->var.prod_app == DIGI_PROD_APP_SYSOTN_CARD)
            {
                /* ODUKSW port used by LINEOTN -> HO -> ODUKSW */
                output_mpmo_ss = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_CTL;
            } else {
                /* ODUKSW port used by ILKN1 - no MPMO to configure */
            }
            break;
        case UTIL_GLOBAL_ODUK_PORT_ILKN2:
            /* ODUKSW port used by ILKN2 - no MPMO to configure */
            break;
        case UTIL_GLOBAL_ODUK_PORT_COREOTN:
            output_mpmo_ss = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_FO1;
            break;
        case UTIL_GLOBAL_ODUK_PORT_MAPOTN:
            /* MPMA towards MAPOTN operated as a function of CPB activation - no action*/
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
    }

    /* need to find the unique data target if output_mpmo_ss is COREOTN_FO1 or COREOTN_CTL */
    if(output_mpmo_ss == OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_CTL || output_mpmo_ss == OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_FO1)
    {
        /* use odu_level to determine the target */
        switch(local_output_pc_ptr->odu_level)
        {
            case UTIL_GLOBAL_LEVEL_HO_ODU:
                /* collect Interface and ACB instance data for cases where ACB connected */
                digi_otn_acb_ckctl_get(digi_handle,
                                      output_pc_ptr,
                                      &intf,
                                      &acb_inst,
                                      &ckctl_port_addr);


                switch(intf)
                {
                    case DIGI_SERDES_LINE_INTF:
                        target = DIGI_OPSA_MPMO_TARGET_ACB_LINE;
                        break;
                    case DIGI_SERDES_SYSTEM_INTF:
                        target = DIGI_OPSA_MPMO_TARGET_ACB_SYS;
                        break;
                    case DIGI_SERDES_SFI_51_INTF:
                        target = DIGI_OPSA_MPMO_TARGET_ACB_SFI51;
                        break;
                    default:
                        PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
                        break;
                }
                break;
            case UTIL_GLOBAL_LEVEL_MO_ODU:
                if(digi_handle->var.prod_app == DIGI_PROD_APP_SYSOTN_CARD)
                {
                    target = DIGI_OPSA_MPMO_TARGET_TS_STAGE_2;
                } else {
                    target = DIGI_OPSA_MPMO_TARGET_TS_STAGE_1;
                }
                break;
            case UTIL_GLOBAL_LEVEL_LO_ODU:
                if (digi_handle->var.prod_app == DIGI_PROD_APP_SYSOTN_CARD && 
                    output_mpmo_ss == OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_CTL)
                {
                    /* collect Interface and ACB instance data for cases where ACB connected */
                    digi_otn_acb_ckctl_get(digi_handle,
                                           output_pc_ptr,
                                           &intf,
                                           &acb_inst,
                                           &ckctl_port_addr);
                    switch(intf)
                    {
                        case DIGI_SERDES_LINE_INTF:
                            target = DIGI_OPSA_MPMO_TARGET_ACB_LINE;
                            break;
                        case DIGI_SERDES_SYSTEM_INTF:
                            target = DIGI_OPSA_MPMO_TARGET_ACB_SYS;
                            break;
                        case DIGI_SERDES_SFI_51_INTF:
                            target = DIGI_OPSA_MPMO_TARGET_ACB_SFI51;
                            break;
                        default:
                            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
                            break;
                    }
    
                } else {
                    target = DIGI_OPSA_MPMO_TARGET_TS_STAGE_2;
                }
                break;
            case UTIL_GLOBAL_LEVEL_3B_ODU:
            case UTIL_GLOBAL_LEVEL_4_ODU:
            case UTIL_GLOBAL_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC:
            default:
                PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
                break;
        }

    }


    switch(cfg_action)
    {
        case DIGI_OPSA_CFG_ACTION_ACTIVATE:            
            /* activate output MPMO procedures as required */
            if(output_mpmo_ss != LAST_OPSA_HOSTMSG_SS)
            {
                if(activate_deactivate_mpmo)
                {
                    mpmo_proc_type = FALSE;                    
                    digi_opsa_cfg_mpmo_one_port_add(digi_handle,
                                                    output_mpmo_ss,
                                                    local_output_pc_ptr->channel,
                                                    target,
                                                    acb_inst,
                                                    ckctl_port_addr,
                                                    mpmo_proc_type,
                                                    FALSE,
                                                    0,
                                                    0,
                                                    is_config_only);
                }
            }

            if(is_reader_primary == TRUE)
            {
                /* activate input MPMA procedures as required */
                if(input_mpma_ss != LAST_OPSA_HOSTMSG_SS)
                {
                    cpb_slv_chnl_qs_ch_en_get(ocpb_handle,
                                              local_input_pc_ptr->port_type,
                                              local_input_pc_ptr->channel,
                                              &qs_ch_en);
    
                    digi_opsa_cfg_mpma_one_port_add(digi_handle,
                                                    input_mpma_ss,
                                                    local_input_pc_ptr->channel,
                                                    qs_ch_en,
                                                    is_config_only);
                } 
                else 
                {
                    /* set QSn_CH_EN for a DPI slave port that is not using an MPMA */
                    /* applies to ILKN 1 and ILKN 2 */
                    cpb_slv_chnl_qs_ch_en_get(ocpb_handle,
                                              local_input_pc_ptr->port_type,
                                              local_input_pc_ptr->channel,
                                              &qs_ch_en);
    
                    cpb_slv_chnl_qs_ch_en_set(ocpb_handle,
                                              local_input_pc_ptr->port_type,
                                              local_input_pc_ptr->channel,
                                              qs_ch_en);
                }
            }
            
            /* activate input MAPOTN MPMO if part of data path */
            if(input_mpmo_ss != LAST_OPSA_HOSTMSG_SS && is_reader_primary == TRUE && is_slave_zone_primary == TRUE)
            {
                PMC_ASSERT(input_mpmo_ss == OPSA_HOSTMSG_CPB_SS_MAPOTN, DIGI_ERR_CODE_ASSERT, 0, 0);
                digi_opsa_cfg_mpmo_one_port_add(digi_handle,
                                                input_mpmo_ss,
                                                local_input_pc_ptr->channel,
                                                DIGI_OPSA_MPMO_TARGET_MAPPER_TX,
                                                acb_inst,
                                                ckctl_port_addr,
                                                FALSE,
                                                FALSE,
                                                0,
                                                0,
                                                is_config_only);
            }            
            break;
        case DIGI_OPSA_CFG_ACTION_DEACTIVATE:
            /* MPMA and QSn_CH_EN deactivation */
            if(is_reader_primary == TRUE)
            {
                /* deactivate input MPMA procedures as required */
                if(input_mpma_ss == LAST_OPSA_HOSTMSG_SS)
                {
                    /* set QSn_CH_EN for a DPI slave port that is not using an MPMA */
                    /* applies to ILKN 1 and ILKN 2 and ENET_LINE/ENET_SYS in MAC modes */

                    cpb_slv_chnl_qs_ch_en_get(ocpb_handle,
                                              local_input_pc_ptr->port_type,
                                              local_input_pc_ptr->channel,
                                              &qs_ch_en);
    
                    digi_oduksw_chnl_qs_ch_en_get(digi_handle,
                                                  output_pc_ptr,
                                                  &qs_ch_en_to_dis);

                    cpb_slv_chnl_qs_ch_en_set(ocpb_handle,
                                              local_input_pc_ptr->port_type,
                                              local_input_pc_ptr->channel,
                                              (qs_ch_en^qs_ch_en_to_dis));
                } 
                else 
                {
                    if(is_slave_zone_primary != TRUE)
                    {
                        /* use FW to disable single QSn_CH_EN bit */
                        cpb_slv_chnl_qs_ch_en_get(ocpb_handle,
                                                  local_input_pc_ptr->port_type,
                                                  local_input_pc_ptr->channel,
                                                  &qs_ch_en);
        
                        digi_oduksw_chnl_qs_ch_en_get(digi_handle,
                                                      output_pc_ptr,
                                                      &qs_ch_en_to_dis);

                        digi_opsa_cfg_mpma_one_port_add(digi_handle,
                                                        input_mpma_ss,
                                                        local_input_pc_ptr->channel,
                                                        (qs_ch_en^qs_ch_en_to_dis),
                                                        is_config_only);
                    } else {
                        /* use FW to disable QSn_CH_EN and disable the MPMA */
                        digi_opsa_cfg_mpma_one_port_drop(digi_handle,
                                                         input_mpma_ss,
                                                         local_input_pc_ptr->channel);
                    }
                }
            }

            /* deactivate output MPMO procedures as required */
            if(output_mpmo_ss != LAST_OPSA_HOSTMSG_SS)
            {
                if(activate_deactivate_mpmo)
                {
                    digi_opsa_cfg_mpmo_one_port_drop(digi_handle,
                                                     output_mpmo_ss,
                                                     local_output_pc_ptr->channel);
                }

            }

            /* deactivate input MAPOTN MPMO only for the primary zone master */
            if(input_mpmo_ss != LAST_OPSA_HOSTMSG_SS && is_slave_zone_primary == TRUE)
            {

                digi_opsa_cfg_mpmo_one_port_drop(digi_handle,
                                                 input_mpmo_ss,
                                                 local_input_pc_ptr->channel);
            }
            break;
        case DIGI_OPSA_CFG_ACTION_RELEASE:
            /* unconditionaly drop MPMA and MPMO port/channel */

            /* All QSn_CH_EN bits need to be cleared */
            cpb_slv_chnl_qs_ch_en_set(digi_handle->dcpb_handle,
                                      local_input_pc_ptr->port_type,
                                      local_input_pc_ptr->channel,
                                      0);    
    
            /* deactivate MPMA procedures as required */
            if(input_mpma_ss != LAST_OPSA_HOSTMSG_SS)
            {
                digi_opsa_cfg_mpma_one_port_drop(digi_handle,
                                                 input_mpma_ss,
                                                 local_input_pc_ptr->channel);
            }
                
            /* deactivate output MPMO procedures as required */
            if(output_mpmo_ss != LAST_OPSA_HOSTMSG_SS)
            {
    
                digi_opsa_cfg_mpmo_one_port_drop(digi_handle,
                                                 output_mpmo_ss,
                                                 local_output_pc_ptr->channel);
    
            }
            break;

        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
    }

    PMC_RETURN();

} /* digi_opsa_oduksw_activate_cfg */

/*******************************************************************************
* digi_opsa_oduksw_activate_cfg_all
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an internal helper function to determine the presenece of MPMA and
*   MPMO instances in a datapath.  If they are present, FW host message to
*   activate these is applied.
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   *input_pc_ptr         - util_global_switch_data_t pointer defining inbound 
*                           port
*   *output_pc_ptr        - util_global_switch_data_t pointer defining outbound 
*                           port
*   is_reader_primary     - indicator if the channel is primary reader for queue
*                           system
*   is_slave_zone_primary - indicator if the channel is the zone primary of all
*                           channels for this slave port
*   cfg_action            - configuration operation to be performed by this API.
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_opsa_oduksw_activate_cfg_all(digi_handle_t *digi_handle,
                                              util_global_switch_data_t **input_pc_ptr,
                                              util_global_switch_data_t **output_pc_ptr,
                                              BOOL *is_reader_primary,
                                              BOOL *is_slave_zone_primary,
                                              digi_opsa_cfg_action_t cfg_action)
{

    digi_serdes_intf_t intf[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];
    
    util_global_switch_data_def_t *input_pc_data_ptr[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];
    util_global_switch_data_def_t *output_pc_data_ptr[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];
    util_global_oduk_port_data_t *local_input_pc_ptr[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];
    util_global_oduk_port_data_t *local_output_pc_ptr[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];
    
    UINT32 input_chnl_list[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];
    UINT32 output_chnl_list[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];
    UINT32 i;
    UINT32 primary_reader_input_chnl_list[UTIL_GLOBAL_MAX_NUM_ODU_CHNL]; 
    UINT32 non_primary_reader_chnl_list[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];  
    UINT32 primary_reader_slave_zone_chnl_list[UTIL_GLOBAL_MAX_NUM_ODU_CHNL]; 
    UINT32 primary_reader_non_slave_zone_chnl_list[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];
    UINT32 input_mpmo_filtered_list[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];
    digi_opsa_mpmo_target_t target[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];

    
    UINT8 qs_ch_en[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];    
    UINT8 qs_ch_val[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];
    UINT8 idx_primary = 0;
    UINT8 idx_non_primary = 0;
    UINT8 idx_non_slave_zone = 0;
    UINT8 idx_primary_slave_zone = 0;
    /*UINT8 qs_ch_en_to_dis[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];*/

    UINT8 qs_ch_en_to_dis;
    UINT8 ckctl_port_addr[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];
    UINT8 acb_inst[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];
    
    hostmsg_opsa_subsystem_cfg_t output_mpmo_ss = LAST_OPSA_HOSTMSG_SS; /* initialize to illegal value */
    hostmsg_opsa_subsystem_cfg_t input_mpma_ss = LAST_OPSA_HOSTMSG_SS; /* initialize to illegal value */
    hostmsg_opsa_subsystem_cfg_t input_mpmo_ss = LAST_OPSA_HOSTMSG_SS; /* initialize to illegal value */
 
    util_global_mapping_mode_t mapotn_mapping_mode = UTIL_GLOBAL_NO_MAP;
    BOOL8 mpmo_cfg_req = FALSE;
    /*
     * Use an input/outpot port value that can be used in the switch 
     * statement without fear of a pointer being NULL. If there is a valid
     * input/output channel then these variables will have a valid value
     * Otherwise they point to their initialized values.
     */
    util_global_oduk_port_t input_port_type = LAST_UTIL_GLOBAL_ODUK_PORT;
    util_global_oduk_port_t output_port_type = LAST_UTIL_GLOBAL_ODUK_PORT;

    cpb_handle_t * ocpb_handle = coreotn_oduksw_handle_get(digi_handle->coreotn_handle);

    mapotn_src_dest_t mapotn_src = LAST_MAPOTN_DEST;
    mapotn_src_dest_t mapotn_dest = LAST_MAPOTN_DEST;
    util_global_map_adapt_func_t adapt_mode = UTIL_GLOBAL_MAP_NO_RATE;
    util_global_map_adapt_func_t dummy_adapt_mode = UTIL_GLOBAL_MAP_NO_RATE;

    PMC_ERROR result = PMC_SUCCESS;

    coreotn_odu_chnl_prov_mode_t prov_mode = LAST_COREOTN_CHNL_PROV_MODE;

    PMC_ENTRY();

    result = coreotn_odu_chnl_prov_mode_get(digi_handle->coreotn_handle, &prov_mode);

    PMC_ASSERT(cfg_action < LAST_DIGI_OPSA_CFG_ACTION, DIGI_ERR_CODE_ASSERT, 0, 0);
    
    for (i = 0; i < UTIL_GLOBAL_MAX_NUM_ODU_CHNL; i++)
    {
        qs_ch_en[i] = 0;
        /*qs_ch_en_to_dis[i] = 0;   */ 
        qs_ch_val[i] = 0;
        ckctl_port_addr[i] = 0;
        acb_inst[i] = 0;
        intf[i] = DIGI_SERDES_LINE_INTF;
        target[i] = DIGI_OPSA_MPMO_TARGET_ACB_LINE;
                       
        if (input_pc_ptr[i] != NULL)
        {
            input_pc_data_ptr[i] = (util_global_switch_data_def_t *)input_pc_ptr[i];
            local_input_pc_ptr[i] = &(input_pc_data_ptr[i]->oduk_data);
            input_port_type = local_input_pc_ptr[i]->port_type;
            input_chnl_list[i] = local_input_pc_ptr[i]->channel;
            
            if (is_reader_primary[i] == TRUE)
            {
                primary_reader_input_chnl_list[idx_primary] = input_chnl_list[i];
                idx_primary++;
                
                if (is_slave_zone_primary[i] == TRUE)
                {
                    primary_reader_slave_zone_chnl_list[idx_primary_slave_zone] = input_chnl_list[i];
                    input_mpmo_filtered_list[idx_primary_slave_zone] = input_chnl_list[i];
                    idx_primary_slave_zone++;
                }
                else {
                    /* if the channel is a reader primary but not slave zone primary */
                    primary_reader_non_slave_zone_chnl_list[idx_non_slave_zone] = input_chnl_list[i];
                    idx_non_slave_zone++;
                }
            }
            else {
                non_primary_reader_chnl_list[idx_non_primary] = input_chnl_list[i];
                idx_non_primary++;
            }
            
        }
        else {
                
            input_pc_data_ptr[i] = NULL;
            local_input_pc_ptr[i] = NULL;
            input_chnl_list[i] = 0xFF;
        } 
        
        if (output_pc_ptr[i] != NULL)
        {
            output_pc_data_ptr[i] = (util_global_switch_data_def_t *)output_pc_ptr[i];
            local_output_pc_ptr[i] = &(output_pc_data_ptr[i]->oduk_data);
            output_chnl_list[i] = local_output_pc_ptr[i]->channel;
            output_port_type = local_output_pc_ptr[i]->port_type;
        }
        else {
            output_pc_data_ptr[i] = NULL;
            local_output_pc_ptr[i] = NULL;
            output_chnl_list[i] = 0xFF;
        }
    }
    
    /* initialize unused elements in the array */
    if (idx_primary < UTIL_GLOBAL_MAX_NUM_ODU_CHNL)
    {
        for (i = idx_primary; i < UTIL_GLOBAL_MAX_NUM_ODU_CHNL; i++)
        {
            primary_reader_input_chnl_list[i] = DIGI_DCI_CHANNEL_UNASSIGNED;
        }
    }
    
    /* initialize unused elements in the array */
    if (idx_non_slave_zone < UTIL_GLOBAL_MAX_NUM_ODU_CHNL)
    {
        for (i = idx_non_slave_zone; i < UTIL_GLOBAL_MAX_NUM_ODU_CHNL; i++)
        {
            primary_reader_non_slave_zone_chnl_list[i] = DIGI_DCI_CHANNEL_UNASSIGNED;
        }
    }
    
    if (idx_primary_slave_zone < UTIL_GLOBAL_MAX_NUM_ODU_CHNL)
    {
        for (i = idx_primary_slave_zone; i < UTIL_GLOBAL_MAX_NUM_ODU_CHNL; i++)
        {
            primary_reader_slave_zone_chnl_list[i] = DIGI_DCI_CHANNEL_UNASSIGNED;
            input_mpmo_filtered_list[i] = DIGI_DCI_CHANNEL_UNASSIGNED;
        }
    }
        
    /* determine the details of the input port */

    switch(input_port_type)
    {
        case UTIL_GLOBAL_ODUK_PORT_ILKN1:
            if(digi_handle->var.prod_app == DIGI_PROD_APP_SYSOTN_CARD)
            {
                /* ODUKSW port used by LINEOTN -> HO -> ODUKSW */
                input_mpma_ss = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_CTL;
            } else {
                /* ODUKSW port used by ILKN1 - no MPMA to configure */
            }
            break;
        case UTIL_GLOBAL_ODUK_PORT_ILKN2:
            /* ODUKSW port used by ILKN2 - no MPMA to configure */
            break;
        case UTIL_GLOBAL_ODUK_PORT_COREOTN:
            input_mpma_ss = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_FO1;
            break;
        case UTIL_GLOBAL_ODUK_PORT_MAPOTN:

            input_mpma_ss = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_FO2;

            for (i = 0; i < idx_primary_slave_zone; i++)
            {          
                    result = mapotn_mapper_mpmo_cfg_get(digi_handle->mapotn_handle, input_mpmo_filtered_list[i], &mpmo_cfg_req);
                    PMC_ASSERT(result==PMC_SUCCESS, DIGI_ERR_CODE_ASSERT, 0, 0);
                    digi_mapper_src_dest_get(digi_handle,
                                             digi_handle->handle_pool.mapper_chnl[input_mpmo_filtered_list[i]].mode,
                                             digi_handle->handle_pool.mapper_chnl[input_mpmo_filtered_list[i]].enet_pmon,
                                             &mapotn_src,
                                             &mapotn_dest);
                    digi_mapotn_mode_params_convert(digi_handle->handle_pool.mapper_chnl[input_mpmo_filtered_list[i]].mode,
                                                    &adapt_mode,
                                                    &dummy_adapt_mode);
                    result = mapotn_mapping_mode_get(digi_handle->mapotn_handle, 
                                                     adapt_mode,
                                                     &mapotn_mapping_mode);

                    PMC_ASSERT(result==PMC_SUCCESS, DIGI_ERR_CODE_ASSERT, 0, 0);

                    if(mpmo_cfg_req == TRUE && mapotn_src == MAPOTN_DEST_CPB && mapotn_mapping_mode != UTIL_GLOBAL_BMP)
                    {
                        input_mpmo_ss = OPSA_HOSTMSG_CPB_SS_MAPOTN;
                    }
                    else
                    {
                        input_mpmo_filtered_list[i] = DIGI_DCI_CHANNEL_UNASSIGNED;
                    }
            }

            break;
        default:
            /*
             * If may be that there is no input port for this port that is
             * being cleaned in which case simply breaking out is fine.
             */
            break;
    }

    /* determine the details of the output port */
    switch(output_port_type)
    {
        case UTIL_GLOBAL_ODUK_PORT_ILKN1:
            if(digi_handle->var.prod_app == DIGI_PROD_APP_SYSOTN_CARD)
            {
                /* ODUKSW port used by LINEOTN -> HO -> ODUKSW */
                output_mpmo_ss = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_CTL;
            } else {
                /* ODUKSW port used by ILKN1 - no MPMO to configure */
            }
            break;
        case UTIL_GLOBAL_ODUK_PORT_ILKN2:
            /* ODUKSW port used by ILKN2 - no MPMO to configure */
            break;
        case UTIL_GLOBAL_ODUK_PORT_COREOTN:
            output_mpmo_ss = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_FO1;
            break;
        case UTIL_GLOBAL_ODUK_PORT_MAPOTN:
            /* MPMA towards MAPOTN operated as a function of CPB activation - no action*/
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
    }

    /* need to find the unique data target if output_mpmo_ss is COREOTN_FO1 or COREOTN_CTL */
    if(output_mpmo_ss == OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_CTL || output_mpmo_ss == OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_FO1)
    {
        /* use odu_level to determine the target */
        
        switch(local_output_pc_ptr[0]->odu_level)
        {
            case UTIL_GLOBAL_LEVEL_HO_ODU:
                i = 0;

                while (output_pc_ptr[i] != NULL)
                {
                    /* collect Interface and ACB instance data for cases where ACB connected */
                    digi_otn_acb_ckctl_get(digi_handle,
                                           output_pc_ptr[0],
                                           &intf[i],
                                           &acb_inst[i],
                                           &ckctl_port_addr[i]);


                    switch(intf[i])
                    {
                        case DIGI_SERDES_LINE_INTF:
                            target[i] = DIGI_OPSA_MPMO_TARGET_ACB_LINE;
                            break;
                        case DIGI_SERDES_SYSTEM_INTF:
                            target[i] = DIGI_OPSA_MPMO_TARGET_ACB_SYS;
                            break;
                        case DIGI_SERDES_SFI_51_INTF:
                            target[i] = DIGI_OPSA_MPMO_TARGET_ACB_SFI51;
                            break;
                        default:
                            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
                            break;
                    }
                    i++;
                }
                break;
            case UTIL_GLOBAL_LEVEL_MO_ODU:
                i = 0;
                if(digi_handle->var.prod_app == DIGI_PROD_APP_SYSOTN_CARD)
                {
                    while (output_pc_ptr[i] != NULL)
                    {
                        target[i] = DIGI_OPSA_MPMO_TARGET_TS_STAGE_2;
                        i++;
                    }
                } else {
                    while (output_pc_ptr[i] != NULL)
                    {
                        target[i] = DIGI_OPSA_MPMO_TARGET_TS_STAGE_1;
                        i++;
                    }
                }
                break;
            case UTIL_GLOBAL_LEVEL_LO_ODU:
                if (digi_handle->var.prod_app == DIGI_PROD_APP_SYSOTN_CARD && 
                    output_mpmo_ss == OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_CTL)
                {
                    i = 0;
                    while (output_pc_ptr[i] != NULL)
                    {
                        /* collect Interface and ACB instance data for cases where ACB connected */
                        digi_otn_acb_ckctl_get(digi_handle,
                                               output_pc_ptr[i],
                                               &intf[i],
                                               &acb_inst[i],
                                               &ckctl_port_addr[i]);
                        
                    
                        switch(intf[i])
                        {
                            case DIGI_SERDES_LINE_INTF:
                                target[i] = DIGI_OPSA_MPMO_TARGET_ACB_LINE;
                                break;
                            case DIGI_SERDES_SYSTEM_INTF:
                                target[i] = DIGI_OPSA_MPMO_TARGET_ACB_SYS;
                                break;
                            case DIGI_SERDES_SFI_51_INTF:
                                target[i] = DIGI_OPSA_MPMO_TARGET_ACB_SFI51;
                                break;
                            default:
                                PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
                                break;
                        }
                        i++;
                    }
    
                } 
                else {
                    i = 0;
                    while (output_pc_ptr[i] != NULL)
                    {
                        target[i] = DIGI_OPSA_MPMO_TARGET_TS_STAGE_2;
                        i++;
                    }
                }
                break;
            case UTIL_GLOBAL_LEVEL_3B_ODU:
            case UTIL_GLOBAL_LEVEL_4_ODU:
            case UTIL_GLOBAL_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC:
            default:
                PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
                break;
        }

    }

    switch(cfg_action)
    {
        case DIGI_OPSA_CFG_ACTION_ACTIVATE:            
            /* activate output MPMO procedures as required */
            if(output_mpmo_ss != LAST_OPSA_HOSTMSG_SS)
            {
                if (prov_mode != COREOTN_ODU_CHNL_PROV_ODUKSW_ONLY_BULK) {
                    digi_opsa_cfg_mpmo_all_chnl_add(digi_handle,
                                                output_mpmo_ss,
                                                output_chnl_list,
                                                target,
                                                acb_inst,
                                                ckctl_port_addr,
                                                FALSE,
                                                FALSE,
                                                0,
                                                0);    
                }
            }

                               
            /* activate input MPMA procedures as required */
            if(input_mpma_ss != LAST_OPSA_HOSTMSG_SS)
            {

                cpb_ocpb_slv_chnl_qs_ch_en_get_all(ocpb_handle,
                                                   input_port_type,
                                                   primary_reader_input_chnl_list,
                                                   TRUE,
                                                   qs_ch_en); 
                
                digi_opsa_cfg_mpma_all_chnl_add(digi_handle,
                                                input_mpma_ss,
                                                primary_reader_input_chnl_list,
                                                qs_ch_en);

            }
            else { 
                
                cpb_ocpb_slv_chnl_qs_ch_en_get_all(ocpb_handle,
                                                   input_port_type,
                                                   primary_reader_input_chnl_list,
                                                   TRUE,
                                                   qs_ch_en); 
                  
                for (i = 0; i < idx_primary; i++)
                {
                    /* set QSn_CH_EN for a DPI slave port that is not using an MPMA */
                    /* applies to ILKN 1 and ILKN 2 */
                    if (NULL != local_input_pc_ptr[i])
                    {
                        cpb_slv_chnl_qs_ch_en_set(ocpb_handle,
                                                  local_input_pc_ptr[i]->port_type,
                                                  primary_reader_input_chnl_list[i],
                                                  qs_ch_en[i]);
                    }
                }
            }

            /* activate input MAPOTN MPMO if part of data path */
            if(input_mpmo_ss != LAST_OPSA_HOSTMSG_SS)
            {
                PMC_ASSERT(input_mpmo_ss == OPSA_HOSTMSG_CPB_SS_MAPOTN, DIGI_ERR_CODE_ASSERT, 0, 0);
                
                for (i = 0; i < idx_primary_slave_zone; i++)
                {
                    if (input_mpmo_filtered_list[i] != DIGI_DCI_CHANNEL_UNASSIGNED)
                    {
                        target[i] = DIGI_OPSA_MPMO_TARGET_MAPPER_TX;
                    }
                }
                digi_opsa_cfg_mpmo_all_chnl_add(digi_handle,
                                                input_mpmo_ss,
                                                input_mpmo_filtered_list,
                                                target,
                                                acb_inst,
                                                ckctl_port_addr,
                                                FALSE,
                                                FALSE,
                                                0,
                                                0);  
            }     

                   
            break;
        case DIGI_OPSA_CFG_ACTION_DEACTIVATE:
            
            /* MPMA and QSn_CH_EN deactivation */
                       
            /* deactivate input MPMA procedures as required */
            if(input_mpma_ss == LAST_OPSA_HOSTMSG_SS)
            {
                /* set QSn_CH_EN for a DPI slave port that is not using an MPMA */
                /* applies to ILKN 1 and ILKN 2 and ENET_LINE/ENET_SYS in MAC modes */
                cpb_ocpb_slv_chnl_qs_ch_en_get_all(ocpb_handle,
                                                   input_port_type,
                                                   primary_reader_input_chnl_list,
                                                   FALSE,
                                                   qs_ch_en);  
                
                digi_oduksw_chnl_qs_ch_en_get_all(digi_handle, 
                                                  output_port_type,
                                                  &qs_ch_en_to_dis);   

                for (i = 0; i < idx_primary; i++)
                {
                    cpb_slv_chnl_qs_ch_en_set(ocpb_handle,
                                              local_input_pc_ptr[i]->port_type,
                                              primary_reader_input_chnl_list[i],
                                              (qs_ch_en[i]^qs_ch_en_to_dis)); 
                }   
                                                          
            } 
            else 
            {
                /* the channel is a primary reader but not a slave_zone_primary */
                /* use FW to disable single QSn_CH_EN bit */
                cpb_ocpb_slv_chnl_qs_ch_en_get_all(ocpb_handle,
                                                   input_port_type,
                                                   primary_reader_non_slave_zone_chnl_list,
                                                   FALSE,
                                                   qs_ch_en); 

                digi_oduksw_chnl_qs_ch_en_get_all(digi_handle, 
                                                  output_port_type,
                                                  &qs_ch_en_to_dis); 

                for (i = 0; i < idx_non_slave_zone; i++)
                {                              
                    qs_ch_val[i] = qs_ch_en[i]^qs_ch_en_to_dis;                                                  
                }

                if (idx_non_slave_zone != 0)
                {
                    digi_opsa_cfg_mpma_all_chnl_add(digi_handle,
                                                    input_mpma_ss,
                                                    primary_reader_non_slave_zone_chnl_list,
                                                    qs_ch_val);
                }
                
                if(input_mpma_ss != LAST_OPSA_HOSTMSG_SS)
                {
                    /* the channel is a primary reader and is a slave zone primary */
                    
                    /* use FW to disable QSn_CH_EN and disable the MPMA */
                    /* create digi_opsa_cfg_mpma_all_chnl_drop */  
                    digi_opsa_cfg_mpma_all_chnl_drop(digi_handle,
                                                     input_mpma_ss,
                                                     primary_reader_slave_zone_chnl_list);
                                                                                                    
                }

            }

            if (prov_mode != COREOTN_ODU_CHNL_PROV_ODUKSW_ONLY_BULK) {
                /* deactivate output MPMO procedures as required */
                if(output_mpmo_ss != LAST_OPSA_HOSTMSG_SS)
                {
                    digi_opsa_cfg_mpmo_all_chnl_drop(digi_handle,
                                                     output_mpmo_ss,
                                                     output_chnl_list);
                }
            }

            /* deactivate input MAPOTN MPMO only for the primary zone master */
            /*if(input_mpmo_ss != LAST_OPSA_HOSTMSG_SS && is_slave_zone_primary == TRUE)*/
            if(input_mpmo_ss != LAST_OPSA_HOSTMSG_SS)
            {

                digi_opsa_cfg_mpmo_all_chnl_drop(digi_handle,
                                                 input_mpmo_ss,
                                                 input_mpmo_filtered_list);
            }
            break;

        case DIGI_OPSA_CFG_ACTION_RELEASE:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
    }

    PMC_RETURN();

} /* digi_opsa_oduksw_activate_cfg_all */

/*******************************************************************************
* digi_fw_opsa_update
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Updates the FW OPSA structure which is required if moving from a 3.04
*  FW version to something later than 3.06.  Ensures the upgrade sequence
*  does not skip over OPSA related data structures which were created after
*  3.04 FW.
*  Iterates over the digi_handles looking at all ACTIVE inputs and their
*  outputs and determines if MPMO modules are in use. Then needs to 
*  account for any that may have been dropped as a result of nominal
*  replacement.
*
* INPUTS:
*  digi_handle - DIGI device handle
*  
* OUTPUTS:
*   none
*
*  RETURNS:
*  PMC_SUCCESS - Operation was successful
*  PMC_ERROR   - Otherwise
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_opsa_fw_update(digi_handle_t *digi_handle)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;

    PMC_ENTRY();
    /*
     * For the CPB side there are a maximum of 12 ENET LINE, SYS or CBRC
     * resources. Using ENET_CHNL_MAX which is set to 12 as the upper limit
     */
    for (i = 0; i < DIGI_ENET_CHNL_MAX; i++)
    {
        /*
         * For each resource handled connected to the CPB:
         * 1. Retrieve its input channel handle
         * 2. Determine whether the output pointer is a primary reader,
         *    or slave_zone_primary.
         * 3. Call digi_opsa_cpb_activate_cfg with activate action and
         *    value of TRUE to indicate that this is a configuration 
         *    operation with respect to MPMO/MPMA add/drop.
         * These steps are handled in digi_opsa_cpb_config_nominal for 
         * enet line/sys and cbrc.
         * MAPOTN MPMO is configured as part of digi_opsa_oduksw_activate
         * and ILKN 1 and 2 do not use MPMO and do not need to be called
         * as an output.
         */
        digi_opsa_cpb_config_nominal(digi_handle,
                                     &digi_handle->handle_pool.enet_line_chnl[i].switch_data);
        digi_opsa_cpb_config_nominal(digi_handle,
                                     &digi_handle->handle_pool.enet_sys_chnl[i].switch_data);
        digi_opsa_cpb_config_nominal(digi_handle,
                                     &digi_handle->handle_pool.cbr_chnl[i].switch_data);
    }
    /*
     * Iterate over the 96 possible LO and MO ODU channels and first 12 HO
     * and for any that are provisioned, unchannelized and active call
     * digi_opsa_config_nominal to determine its input value and whether
     * MPMO/MPMA add operations are required in FW.
     */
    for (i = 0; i < DIGI_OTN_SERVER_LO_CHNL_MAX; i++)
    {
        if (i < DIGI_OTN_SERVER_HO_CHNL_MAX &&
            UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED == digi_handle->handle_pool.ho_odu_struct[i].switch_data.header.prov_state &&
            UTIL_GLOBAL_ODU_UNCHANNELIZED == digi_handle->handle_pool.ho_odu_struct[i].mem_ptr->payload_format)
        {
            result = digi_opsa_oduksw_config_nominal(digi_handle,
                                                     &digi_handle->handle_pool.ho_odu_struct[i].switch_data,
                                                     &digi_handle->handle_pool.ho_odu_struct[i]);
            PMC_ASSERT(PMC_SUCCESS == result, result, 0, 0);
        }
        else if (UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED == digi_handle->handle_pool.mo_odu_struct[i].switch_data.header.prov_state &&
                UTIL_GLOBAL_ODU_UNCHANNELIZED == digi_handle->handle_pool.mo_odu_struct[i].mem_ptr->payload_format)
        {
            result = digi_opsa_oduksw_config_nominal(digi_handle,
                                                     &digi_handle->handle_pool.mo_odu_struct[i].switch_data,
                                                     &digi_handle->handle_pool.mo_odu_struct[i]);
            PMC_ASSERT(PMC_SUCCESS == result, result, 0, 0);
        }
        else if (UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED == digi_handle->handle_pool.lo_odu_struct[i].switch_data.header.prov_state &&
                 UTIL_GLOBAL_ODU_UNCHANNELIZED == digi_handle->handle_pool.lo_odu_struct[i].mem_ptr->payload_format)
        {
            result = digi_opsa_oduksw_config_nominal(digi_handle,
                                                     &digi_handle->handle_pool.lo_odu_struct[i].switch_data,
                                                     &digi_handle->handle_pool.lo_odu_struct[i]);
            PMC_ASSERT(PMC_SUCCESS == result, result, 0, 0);
        }
    }

    for (i = UTIL_GLOBAL_ILKN_CPB_DPI_CHNLS_MAX; i < UTIL_GLOBAL_ILKN_ARB_DPI_CHANS_MAX; i++)
    {
        if (UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED == digi_handle->handle_pool.ilkn_1_chnl[i].switch_data.header.prov_state)
        {
            result = digi_opsa_oduksw_config_nominal(digi_handle,
                                                     &digi_handle->handle_pool.ilkn_1_chnl[i].switch_data,
                                                     NULL);
            PMC_ASSERT(PMC_SUCCESS == result, result, 0, 0);
        }
    }

    PMC_RETURN(result);
}


/*******************************************************************************
* digi_opsa_cpb_activate_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an internal helper function to determine the presenece of MPMA and
*   MPMO instances in a datapath.  If they are present, FW host message to
*   activate these is applied.
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   *input_pc_ptr         - util_global_switch_data_t pointer defining inbound 
*                           port
*   *output_pc_ptr        - util_global_switch_data_t pointer defining outbound 
*                           port
*   is_reader_primary     - indicator if the channel is primary reader for queue
*                           system
*   is_slave_zone_primary - indicator if the channel is the zone primary of all
*                           channels for this slave port
*   cfg_action            - configuration operation to be performed by this API.
*   is_config             - Flag that indicates whether the OPSA ADD/DROP
*                           operations are actual add/drops or configuration
*                           only.
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
PUBLIC void digi_opsa_cpb_activate_cfg(digi_handle_t *digi_handle,
                                       util_global_switch_data_t *input_pc_ptr,
                                       util_global_switch_data_t *output_pc_ptr,
                                       BOOL8 is_reader_primary,
                                       BOOL8 is_slave_zone_primary,
                                       digi_opsa_cfg_action_t cfg_action,
                                       BOOL8 is_config)
{

    digi_serdes_intf_t intf;
    UINT32 input_fo2_mpmo_chnl = 0;
    UINT8 ckctl_port_addr = 0, fo2_ckctl_port_addr = 0;
    BOOL8 mpmo_proc_type = FALSE;
    BOOL8 cbrc_monitor_type = FALSE;
    util_global_map_adapt_func_t adapt_mode = UTIL_GLOBAL_MAP_NO_RATE;
    util_global_map_adapt_func_t dummy_adapt_mode = UTIL_GLOBAL_MAP_NO_RATE;
    util_global_mapping_mode_t mapotn_mapping_mode = UTIL_GLOBAL_NO_MAP;
    util_global_switch_data_def_t *input_pc_data_ptr = NULL;
    util_global_switch_data_def_t *output_pc_data_ptr = NULL;

    util_global_cpb_port_data_t *local_input_pc_ptr = (util_global_cpb_port_data_t*)input_pc_ptr;
    util_global_cpb_port_data_t *local_output_pc_ptr = (util_global_cpb_port_data_t*)output_pc_ptr;

    digi_serdes_acb_mode_t acb_timing_mode = DIGI_SERDES_ACB_MODE_NOMINAL;
    digi_enet_client_chnl_def_t *enet_chnl_handle_ptr;
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    digi_mapping_mode_t mode = LAST_DIGI_MAP;
    digi_enet_mode_t e_mode;

    hostmsg_opsa_subsystem_cfg_t input_fo2_mpmo_ss = LAST_OPSA_HOSTMSG_SS; /* initialize to illegal value */
    hostmsg_opsa_subsystem_cfg_t output_mpmo_ss = LAST_OPSA_HOSTMSG_SS; /* initialize to illegal value */
    hostmsg_opsa_subsystem_cfg_t input_mpma_ss = LAST_OPSA_HOSTMSG_SS; /* initialize to illegal value */
    digi_opsa_mpmo_target_t data_target = LAST_DIGI_OPSA_MPMO_TARGET;

    UINT8 acb_inst = 0;

    BOOL8 mpmo_cfg_req = FALSE, input_mpma_cfg_required = FALSE;
    UINT8 qs_ch_en = 0, qs_ch_en_to_dis = 0;

    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(cfg_action < LAST_DIGI_OPSA_CFG_ACTION, DIGI_ERR_CODE_ASSERT, 0, 0);


    input_pc_data_ptr = (util_global_switch_data_def_t *)input_pc_ptr;
    output_pc_data_ptr = (util_global_switch_data_def_t *)output_pc_ptr;

    local_input_pc_ptr = &(input_pc_data_ptr->cpb_data);
    local_output_pc_ptr = &(output_pc_data_ptr->cpb_data);

    /* determine if MPMA or FO2 MPMO in the data path */
    switch(local_input_pc_ptr->port_type)
    {
        case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
            if(local_input_pc_ptr->data_source == 1) /* MAPOTN Channel Handle Case */
            {
                mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(input_pc_ptr);
                mode = mapper_chnl_handle_ptr->mode;

                result = digi_mapotn_data_src_acb_mode_find(digi_handle, mode, &acb_timing_mode);
                PMC_LOG_TRACE("DIGI_MAPPER_ALLOC_MAPPER_WITH_ENET_LINE mapping mode is:%d\n", mode);
                if(acb_timing_mode != DIGI_SERDES_ACB_MODE_NOMINAL || DIGI_OPSA_CFG_ACTION_RELEASE == cfg_action)
                {
                    input_fo2_mpmo_ss = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_FO2;
                    input_fo2_mpmo_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
                    input_mpma_ss = OPSA_HOSTMSG_CPB_SS_ENET_LINE;
                }
            }
            else /* ENET_LINE Channel Handle Case */
            {
                enet_chnl_handle_ptr = (digi_enet_client_chnl_def_t*)(input_pc_ptr);
                e_mode = enet_chnl_handle_ptr->mode;
                result = digi_enet_data_src_acb_mode_find(digi_handle, e_mode, &acb_timing_mode);
                if(acb_timing_mode != DIGI_SERDES_ACB_MODE_NOMINAL &&
                   e_mode != DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT &&
                   e_mode != DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON)
                {
                    input_mpma_ss = OPSA_HOSTMSG_CPB_SS_ENET_LINE;
                }
                PMC_LOG_TRACE("NON DIGI_MAPPER_ALLOC_MAPPER_WITH_ENET_LINE mapping mode is:%d\n", e_mode);
            }
            break;
        case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
            enet_chnl_handle_ptr = (digi_enet_client_chnl_def_t*)(input_pc_ptr);
            e_mode = enet_chnl_handle_ptr->mode;
            result = digi_enet_data_src_acb_mode_find(digi_handle, e_mode, &acb_timing_mode);
            if(acb_timing_mode != DIGI_SERDES_ACB_MODE_NOMINAL &&
               e_mode != DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT &&
               e_mode != DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON)
            {
                input_mpma_ss = OPSA_HOSTMSG_CPB_SS_ENET_SYS;
            }
            break;
        case UTIL_GLOBAL_CPB_PORT_CBRC:
            acb_timing_mode = DIGI_SERDES_ACB_MODE_C;
            /* CBRC SS uses MPMA for all cases except FC1200-TTT - use the data destination mode to 
               determine if MPMA present or not. */
            if(local_output_pc_ptr->port_type==UTIL_GLOBAL_CPB_PORT_MAPOTN)
            {
                mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(output_pc_ptr);
                mode = mapper_chnl_handle_ptr->mode;
                if(mode != DIGI_MAP_ODU2EP_FC_1200_GFP) input_mpma_cfg_required = TRUE;
            } else {
                input_mpma_cfg_required = TRUE;
            }

            if(input_mpma_cfg_required == TRUE) input_mpma_ss = OPSA_HOSTMSG_CPB_SS_CBRC;

            break;
        case UTIL_GLOBAL_CPB_PORT_MAPOTN:
            mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(input_pc_ptr);
            mode = mapper_chnl_handle_ptr->mode;

            result = digi_mapotn_data_src_acb_mode_find(digi_handle, mode, &acb_timing_mode);
            if(acb_timing_mode != DIGI_SERDES_ACB_MODE_NOMINAL)
            {
                input_fo2_mpmo_ss = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_FO2;
                input_fo2_mpmo_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
                if(mode != DIGI_MAP_ODU2EP_FC_1200_GFP)
                {
                    input_mpma_ss = OPSA_HOSTMSG_CPB_SS_MAPOTN;
                }
            }

            digi_mapotn_mode_params_convert(mode,
                                            &adapt_mode,
                                            &dummy_adapt_mode);
            result = mapotn_mapping_mode_get(digi_handle->mapotn_handle, 
                                             adapt_mode,
                                             &mapotn_mapping_mode);

            PMC_ASSERT(result==PMC_SUCCESS, DIGI_ERR_CODE_ASSERT, 0, 0);

            break;
        default:
            /* cases that do not use MPMA or FO2 MPMO */
            break;
    }
    PMC_ASSERT(result==PMC_SUCCESS, DIGI_ERR_CODE_ASSERT, 0, 0);

    PMC_LOG_TRACE("acb_timing_mode: %d \n", acb_timing_mode);
    PMC_LOG_TRACE("digi_handle->var.prod_app: %d \n", DIGI_PROD_APP_GET(digi_handle));
    PMC_LOG_TRACE("digi_handle->var.port_type: %d \n", local_output_pc_ptr->port_type);

    /* determine if MPMO in outbound port */
    switch(local_output_pc_ptr->port_type)
    {
        case UTIL_GLOBAL_CPB_PORT_CBRC:
            result = cbrc_mpmo_cfg_get(digi_handle->cbrc_handle, local_output_pc_ptr->channel, &mpmo_cfg_req);
            if(mpmo_cfg_req == TRUE || DIGI_OPSA_CFG_ACTION_RELEASE == cfg_action) output_mpmo_ss = OPSA_HOSTMSG_CPB_SS_CBRC;
            /* if MPMO configuration is required and data source is BMP demapped from MAPOTN SS then alternate MPMO 
               FW procedure required */
            if(mode == DIGI_MAP_ODU2EP_FC_1200_GFP)
            {
                cbrc_monitor_type = TRUE;
                output_mpmo_ss = OPSA_HOSTMSG_CPB_SS_CBRC;
            }
            if(mapotn_mapping_mode == UTIL_GLOBAL_BMP && output_mpmo_ss == OPSA_HOSTMSG_CPB_SS_CBRC)
            {
                mpmo_proc_type = TRUE;
            }
            break;
        case UTIL_GLOBAL_CPB_PORT_MAPOTN:
            /* MAPOTN MPMO is configured in digi_opsa_oduksw_activate_cfg - no action required in this API for setting
               MAPOTN MPMO */
            /* The destination is the MAPOTN_TX path - data_target must be updated accordingly */
            data_target = DIGI_OPSA_MPMO_TARGET_MAPPER_TX;
            break;
        case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
            result = enet_mpmo_cfg_get(digi_handle->enet_line_handle, local_output_pc_ptr->channel, &mpmo_cfg_req);
            if((mpmo_cfg_req == TRUE || DIGI_OPSA_CFG_ACTION_RELEASE == cfg_action)&& is_slave_zone_primary == TRUE) output_mpmo_ss = OPSA_HOSTMSG_CPB_SS_ENET_LINE;
            /* if MPMO configuration is required and data source is BMP demapped from MAPOTN SS then alternate MPMO 
               FW procedure required */
            if(mapotn_mapping_mode == UTIL_GLOBAL_BMP && output_mpmo_ss == OPSA_HOSTMSG_CPB_SS_ENET_LINE)
                mpmo_proc_type = TRUE;
            break;
        case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
            result = enet_mpmo_cfg_get(digi_handle->enet_sys_handle, local_output_pc_ptr->channel, &mpmo_cfg_req);
            if((mpmo_cfg_req == TRUE || DIGI_OPSA_CFG_ACTION_RELEASE == cfg_action)&& is_slave_zone_primary == TRUE) output_mpmo_ss = OPSA_HOSTMSG_CPB_SS_ENET_SYS;
            /* if MPMO configuration is required and data source is BMP demapped from MAPOTN SS then alternate MPMO 
               FW procedure required */
            if(mapotn_mapping_mode == UTIL_GLOBAL_BMP && output_mpmo_ss == OPSA_HOSTMSG_CPB_SS_ENET_SYS)
                mpmo_proc_type = TRUE;
            break;
        case UTIL_GLOBAL_CPB_PORT_ILKN1:
        case UTIL_GLOBAL_CPB_PORT_ILKN2:
        case UTIL_GLOBAL_CPB_PORT_NOT_USED:
        case LAST_UTIL_GLOBAL_CPB_PORT:
        default:
            /* cases that do not use MPMO */
            break;
    }
    PMC_ASSERT(result==PMC_SUCCESS, DIGI_ERR_CODE_ASSERT, 0, 0);

    /* collect Interface and ACB instance data for cases where ACB connected */
    if((local_output_pc_ptr->port_type == UTIL_GLOBAL_CPB_PORT_ENET_SYS ||
        local_output_pc_ptr->port_type == UTIL_GLOBAL_CPB_PORT_CBRC ||
        local_output_pc_ptr->port_type == UTIL_GLOBAL_CPB_PORT_ENET_LINE) &&
        (output_mpmo_ss != LAST_OPSA_HOSTMSG_SS || input_fo2_mpmo_ss != LAST_OPSA_HOSTMSG_SS)&&
        is_slave_zone_primary == TRUE)
    {
        digi_non_otn_acb_ckctl_get(digi_handle,
                                   output_pc_ptr,
                                   &intf,
                                   &acb_inst,
                                   &ckctl_port_addr,
                                   &fo2_ckctl_port_addr);

        switch(intf)
        {
            case DIGI_SERDES_LINE_INTF:
                data_target = DIGI_OPSA_MPMO_TARGET_ACB_LINE;
                break;
            case DIGI_SERDES_SYSTEM_INTF:
                data_target = DIGI_OPSA_MPMO_TARGET_ACB_SYS;
                break;
            case DIGI_SERDES_SFI_51_INTF:
                data_target = DIGI_OPSA_MPMO_TARGET_ACB_SFI51;
                break;
            default:
                PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
                break;
        }
    }

    switch(cfg_action)
    {
        case DIGI_OPSA_CFG_ACTION_ACTIVATE:
            /* activate output MPMO procedures as required */
            if(output_mpmo_ss != LAST_OPSA_HOSTMSG_SS)
            {
                digi_opsa_cfg_mpmo_one_port_add(digi_handle,
                                                output_mpmo_ss,
                                                local_output_pc_ptr->channel,
                                                data_target,
                                                acb_inst,
                                                ckctl_port_addr,
                                                mpmo_proc_type,
                                                cbrc_monitor_type,
                                                input_fo2_mpmo_chnl,
                                                fo2_ckctl_port_addr,
                                                is_config);

            }

            /* activate input MPMA procedures as required */
            if(is_reader_primary == TRUE)
            {
                if(input_mpma_ss != LAST_OPSA_HOSTMSG_SS)
                {
                    cpb_slv_chnl_qs_ch_en_get(digi_handle->dcpb_handle,
                                              local_input_pc_ptr->port_type,
                                              local_input_pc_ptr->channel,
                                              &qs_ch_en);
    
                    digi_opsa_cfg_mpma_one_port_add(digi_handle,
                                                    input_mpma_ss,
                                                    local_input_pc_ptr->channel,
                                                    qs_ch_en,
                                                    is_config);
                }
                else 
                {
                    cpb_slv_chnl_qs_ch_en_get(digi_handle->dcpb_handle,
                                              local_input_pc_ptr->port_type,
                                              local_input_pc_ptr->channel,
                                              &qs_ch_en);
    
                    /* set QSn_CH_EN for a DPI slave port that is not using an MPMA */
                    cpb_slv_chnl_qs_ch_en_set(digi_handle->dcpb_handle,
                                              local_input_pc_ptr->port_type,
                                              local_input_pc_ptr->channel,
                                              qs_ch_en);
                }
            }


            /* activate input FO2 MPMO only for the primary zone master */
            if(input_fo2_mpmo_ss != LAST_OPSA_HOSTMSG_SS && is_slave_zone_primary == TRUE && mpmo_proc_type == FALSE)
            {
                digi_opsa_cfg_mpmo_one_port_add(digi_handle,
                                                input_fo2_mpmo_ss,
                                                input_fo2_mpmo_chnl,
                                                data_target,
                                                acb_inst,
                                                fo2_ckctl_port_addr,
                                                FALSE,
                                                FALSE,
                                                0,
                                                0,
                                                is_config);
            }
            break;
        case DIGI_OPSA_CFG_ACTION_DEACTIVATE:

            if(input_mpma_ss == LAST_OPSA_HOSTMSG_SS && is_reader_primary == TRUE)
            {
                /* set QSn_CH_EN for a DPI slave port that is not using an MPMA */
                cpb_slv_chnl_qs_ch_en_get(digi_handle->dcpb_handle,
                                          local_input_pc_ptr->port_type,
                                          local_input_pc_ptr->channel,
                                          &qs_ch_en);

                digi_cpb_chnl_qs_ch_en_get(digi_handle,
                                           input_pc_ptr,
                                           output_pc_ptr,
                                           &qs_ch_en_to_dis);

                cpb_slv_chnl_qs_ch_en_set(digi_handle->dcpb_handle,
                                          local_input_pc_ptr->port_type,
                                          local_input_pc_ptr->channel,
                                          qs_ch_en^qs_ch_en_to_dis);
            }


            /* activate input MPMA procedures as required */
            if(input_mpma_ss != LAST_OPSA_HOSTMSG_SS && is_reader_primary == FALSE && is_slave_zone_primary == TRUE)
            {
                cpb_slv_chnl_qs_ch_en_get(digi_handle->dcpb_handle,
                                          local_input_pc_ptr->port_type,
                                          local_input_pc_ptr->channel,
                                          &qs_ch_en);

                digi_cpb_chnl_qs_ch_en_get(digi_handle,
                                           input_pc_ptr,
                                           output_pc_ptr,
                                           &qs_ch_en_to_dis);


                digi_opsa_cfg_mpma_one_port_add(digi_handle,
                                                input_mpma_ss,
                                                local_input_pc_ptr->channel,
                                                (qs_ch_en^qs_ch_en_to_dis),
                                                is_config);

            } else if(input_mpma_ss != LAST_OPSA_HOSTMSG_SS && is_slave_zone_primary == TRUE) {

                digi_opsa_cfg_mpma_one_port_drop(digi_handle,
                                                 input_mpma_ss,
                                                 local_input_pc_ptr->channel);

            }
            /* activate output MPMO procedures as required */
            if(output_mpmo_ss != LAST_OPSA_HOSTMSG_SS)
            {

                digi_opsa_cfg_mpmo_one_port_drop(digi_handle,
                                                 output_mpmo_ss,
                                                 local_output_pc_ptr->channel);

            }


            /* activate input FO2 MPMO only for the primary zone master */
            if(input_fo2_mpmo_ss != LAST_OPSA_HOSTMSG_SS && is_slave_zone_primary == TRUE && mpmo_proc_type == FALSE)
            {
                digi_opsa_cfg_mpmo_one_port_drop(digi_handle,
                                                 input_fo2_mpmo_ss,
                                                 input_fo2_mpmo_chnl);

            }
            break;
        case DIGI_OPSA_CFG_ACTION_RELEASE:
            /* unconditionaly drop MPMA and MPMO port/channel */
            /* */
            if(input_mpma_ss != LAST_OPSA_HOSTMSG_SS)
            {
                digi_opsa_cfg_mpma_one_port_drop(digi_handle,
                                                 input_mpma_ss,
                                                 local_input_pc_ptr->channel);
            }
            
            /* deactivate output MPMO procedures as required */
            if(output_mpmo_ss != LAST_OPSA_HOSTMSG_SS)
            {

                digi_opsa_cfg_mpmo_one_port_drop(digi_handle,
                                                 output_mpmo_ss,
                                                 local_output_pc_ptr->channel);

            }


            /* deactivate input FO2 MPMO only for the primary zone master */
            if(input_fo2_mpmo_ss != LAST_OPSA_HOSTMSG_SS && is_slave_zone_primary == TRUE)
            {

                digi_opsa_cfg_mpmo_one_port_drop(digi_handle,
                                                 input_fo2_mpmo_ss,
                                                 input_fo2_mpmo_chnl);

            }
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
    }

    PMC_RETURN();

} /* digi_opsa_cpb_activate_cfg */

/*******************************************************************************
* digi_opsa_cpb_port_release_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an internal helper function to unconditionally release FW
*   functionality from an MPMA or MPMO instance.\n\n
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   port_type          - UTIL_GLOBAL_CPB_PORT_TYPE identified.
*   chnl_mask_ptr      - 3 element integer array representing the channels 
*                        to be droped. \n
*                        Element 0: 31:0 \n
*                        Element 1: 63:32 \n
*                        Element 3: 95:64
*   dir                - scope of operation to perform against port_type and
*                        channel mask.
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
PUBLIC void digi_opsa_cpb_port_release_cfg(digi_handle_t *digi_handle,
                                           util_global_cpb_port_t port_type,
                                           UINT32 *chnl_mask_ptr,
                                           digi_opsa_dir_t dir)
{
    hostmsg_opsa_subsystem_cfg_t inbound_fo2_mpmo_ss = LAST_OPSA_HOSTMSG_SS; /* initialize to illegal value */
    hostmsg_opsa_subsystem_cfg_t outbound_mpmo_ss = LAST_OPSA_HOSTMSG_SS; /* initialize to illegal value */
    hostmsg_opsa_subsystem_cfg_t inbound_mpma_ss = LAST_OPSA_HOSTMSG_SS; /* initialize to illegal value */

    PMC_ENTRY();

    PMC_ASSERT(dir < LAST_DIGI_OPSA_DIR, DIGI_ERR_CODE_ASSERT, 0, 0);

    /* Remap switch data to identify host message subsystems that need to be modified */
    if(dir == DIGI_OPSA_DIR_OUTBOUND || dir == DIGI_OPSA_DIR_BIDIR)
    {
        switch(port_type)
        {
            case UTIL_GLOBAL_CPB_PORT_CBRC:
                outbound_mpmo_ss = OPSA_HOSTMSG_CPB_SS_CBRC;
                break;
            case UTIL_GLOBAL_CPB_PORT_MAPOTN:
                /* MAPOTN MPMO is configured in digi_opsa_oduksw_activate_cfg - no action required in this API */
                break;
            case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
                outbound_mpmo_ss = OPSA_HOSTMSG_CPB_SS_ENET_LINE;
                break;
            case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
                outbound_mpmo_ss = OPSA_HOSTMSG_CPB_SS_ENET_SYS;
                break;
            case UTIL_GLOBAL_CPB_PORT_ILKN1:
            case UTIL_GLOBAL_CPB_PORT_ILKN2:
            case UTIL_GLOBAL_CPB_PORT_NOT_USED:
            case LAST_UTIL_GLOBAL_CPB_PORT:
            default:
                /* cases that do not use MPMO */
                break;
        }
    }

    if(dir == DIGI_OPSA_DIR_INBOUND || dir == DIGI_OPSA_DIR_BIDIR)
    {
        switch(port_type)
        {
            case UTIL_GLOBAL_CPB_PORT_CBRC:
                inbound_mpma_ss = OPSA_HOSTMSG_CPB_SS_CBRC;
                break;
            case UTIL_GLOBAL_CPB_PORT_MAPOTN:
                inbound_mpma_ss = OPSA_HOSTMSG_CPB_SS_MAPOTN;
                inbound_fo2_mpmo_ss = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_FO2;
                break;
            case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
                inbound_mpma_ss = OPSA_HOSTMSG_CPB_SS_ENET_LINE;
                break;
            case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
                inbound_mpma_ss = OPSA_HOSTMSG_CPB_SS_ENET_SYS;
                break;
            case UTIL_GLOBAL_CPB_PORT_ILKN1:
            case UTIL_GLOBAL_CPB_PORT_ILKN2:
            case UTIL_GLOBAL_CPB_PORT_NOT_USED:
            case LAST_UTIL_GLOBAL_CPB_PORT:
            default:
                /* cases that do not use MPMO */
                break;
        }
    }

    /* drop MPMA/MPMO and clear QSn_CH_EN bits */
    if(outbound_mpmo_ss != LAST_OPSA_HOSTMSG_SS)
    {
         digi_fw_opsa_cfg_mpmo_port_drop(digi_handle,
                                         outbound_mpmo_ss,
                                         chnl_mask_ptr);
    }

    /* MPMA needs to be unconditionally turned off from FW processing */
    if(inbound_mpma_ss != LAST_OPSA_HOSTMSG_SS)
    {
        digi_fw_opsa_cfg_mpma_port_drop(digi_handle,
                                         inbound_mpma_ss,
                                         chnl_mask_ptr);
    }

    if(dir == DIGI_OPSA_DIR_INBOUND || dir == DIGI_OPSA_DIR_BIDIR)
    {
        UINT32 i = 0;
        
        while (i < 96)
        {
            if ((chnl_mask_ptr[i/32] >> (i % 32)) & 1)
            {
                /* All QSn_CH_EN bits need to be cleared */
                cpb_slv_chnl_qs_ch_en_set(digi_handle->dcpb_handle,
                                          port_type,
                                          i,
                                          0); 
            }
            i++;
        }
    }

    /* if MAPOTN unconditionally turn off FO2 MPMO */
    if(inbound_fo2_mpmo_ss != LAST_OPSA_HOSTMSG_SS)
    {
        digi_fw_opsa_cfg_mpmo_port_drop(digi_handle,
                                         inbound_fo2_mpmo_ss,
                                         chnl_mask_ptr);
    }

    PMC_RETURN();

} /* digi_opsa_cpb_port_release_cfg */

/*******************************************************************************
* digi_opsa_oduksw_port_release_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an internal helper function to unconditionally release FW
*   functionality from an MPMA or MPMO instance.\n\n
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   port_type        - UTIL_GLOBAL_ODUK_PORT_TYPE identifier.
*   chnl_mask_ptr    - 3 element integer array representing the channels to
*                      be droped. \n
*                      Element 0: 31:0 \n
*                      Element 1: 63:32 \n
*                      Element 3: 95:64
*   dir              - scope of operation to perform against port_type and
*                      channel mask.
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
PUBLIC void digi_opsa_oduksw_port_release_cfg(digi_handle_t *digi_handle,
                                              util_global_oduk_port_t port_type,
                                              UINT32 *chnl_mask_ptr,
                                              digi_opsa_dir_t dir)
{

    hostmsg_opsa_subsystem_cfg_t inbound_mapotn_mpmo_ss = LAST_OPSA_HOSTMSG_SS; /* initialize to illegal value */
    hostmsg_opsa_subsystem_cfg_t outbound_mpmo_ss = LAST_OPSA_HOSTMSG_SS; /* initialize to illegal value */
    hostmsg_opsa_subsystem_cfg_t inbound_mpma_ss = LAST_OPSA_HOSTMSG_SS; /* initialize to illegal value */

    PMC_ENTRY();

    PMC_ASSERT(dir < LAST_DIGI_OPSA_DIR, DIGI_ERR_CODE_ASSERT, 0, 0);

    cpb_handle_t * ocpb_handle = coreotn_oduksw_handle_get(digi_handle->coreotn_handle);


    /* Remap switch data to identify host message subsystems that need to be modified */
    if(dir == DIGI_OPSA_DIR_OUTBOUND || dir == DIGI_OPSA_DIR_BIDIR)
    {
        /* determine the details of the output port */
        switch(port_type)
        {
            case UTIL_GLOBAL_ODUK_PORT_ILKN1:
                if(digi_handle->var.prod_app == DIGI_PROD_APP_SYSOTN_CARD)
                {
                    /* ODUKSW port used by LINEOTN -> HO -> ODUKSW */
                    outbound_mpmo_ss = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_CTL;
                } else {
                    /* ODUKSW port used by ILKN1 - no MPMO to configure */
                }
                break;
            case UTIL_GLOBAL_ODUK_PORT_ILKN2:
                /* ODUKSW port used by ILKN2 - no MPMO to configure */
                break;
            case UTIL_GLOBAL_ODUK_PORT_COREOTN:
                outbound_mpmo_ss = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_FO1;
                break;
            case UTIL_GLOBAL_ODUK_PORT_MAPOTN:
                /* MPMA towards MAPOTN operated as a function of CPB activation - no action*/
                break;
            default:
                PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
                break;
        }
    }

    if(dir == DIGI_OPSA_DIR_INBOUND || dir == DIGI_OPSA_DIR_BIDIR)
    {
        switch(port_type)
        {
            case UTIL_GLOBAL_ODUK_PORT_ILKN1:
                if(digi_handle->var.prod_app == DIGI_PROD_APP_SYSOTN_CARD)
                {
                    /* ODUKSW port used by LINEOTN -> HO -> ODUKSW */
                    inbound_mpma_ss = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_CTL;
                } else {
                    /* ODUKSW port used by ILKN1 - no MPMA to configure */
                }
                break;
            case UTIL_GLOBAL_ODUK_PORT_ILKN2:
                /* ODUKSW port used by ILKN2 - no MPMA to configure */
                break;
            case UTIL_GLOBAL_ODUK_PORT_COREOTN:
                inbound_mpma_ss = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_FO1;
                break;
            case UTIL_GLOBAL_ODUK_PORT_MAPOTN:
                inbound_mpma_ss = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_FO2;
                inbound_mapotn_mpmo_ss = OPSA_HOSTMSG_CPB_SS_MAPOTN;
                break;
            default:
                PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
                break;
        }
    }

    /* drop MPMA/MPMO and clear QSn_CH_EN bits */
    if(outbound_mpmo_ss != LAST_OPSA_HOSTMSG_SS)
    {
        digi_fw_opsa_cfg_mpmo_port_drop(digi_handle,
                                        outbound_mpmo_ss,
                                        chnl_mask_ptr);
    }

    /* MPMA needs to be unconditionally turned off from FW processing */
    if(inbound_mpma_ss != LAST_OPSA_HOSTMSG_SS)
    {
        digi_fw_opsa_cfg_mpma_port_drop(digi_handle,
                                        inbound_mpma_ss,
                                        chnl_mask_ptr);
    }

    if(dir == DIGI_OPSA_DIR_INBOUND || dir == DIGI_OPSA_DIR_BIDIR)
    {
        UINT32 i = 0;

        while (i < 96)
        {
            if ((chnl_mask_ptr[i/32] >> (i % 32)) & 1)
            {
                /* All QSn_CH_EN bits need to be cleared */
                cpb_slv_chnl_qs_ch_en_set(ocpb_handle,
                                          port_type,
                                          i,
                                          0);
            }
            i++;
        }
    }

    /* if FO2 MPMA unconditionally turn off MAPOTN MPMO, use the
       ODUKSW data to ensure ENET_LINE resource not inadvertently touched */
    if(inbound_mapotn_mpmo_ss != LAST_OPSA_HOSTMSG_SS)
    {
        digi_fw_opsa_cfg_mpmo_port_drop(digi_handle,
                                            inbound_mapotn_mpmo_ss,
                                            chnl_mask_ptr);
    }

    PMC_RETURN();

} /* digi_opsa_oduksw_port_release_cfg */

/*******************************************************************************
* digi_opsa_cfg_dcpb_mpma_chnl_backup_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Adds or drops FW operation on an DCPB CBRC MPMA instance that is used for a
*   backup channel.
*
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance.
*   subsystem       - subsystem to configure
*   chnl            - chnl to configure
*   action          - TRUE: add the channel, FALSE: drop the channel
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
PUBLIC void digi_opsa_cfg_dcpb_mpma_chnl_backup_cfg(digi_handle_t *digi_handle,
                                                    hostmsg_opsa_subsystem_cfg_t subsystem,
                                                    UINT32 chnl,
                                                    BOOL8 action)
{
    PMC_ENTRY();

    PMC_ASSERT(subsystem == OPSA_HOSTMSG_CPB_SS_CBRC, DIGI_ERR_CODE_ASSERT, 0, 0);

    if(action)
    {
        digi_opsa_cfg_mpma_one_port_add(digi_handle,
                                        subsystem,
                                        chnl,
                                        0,
                                        FALSE);
    }
    else
    {
        digi_opsa_cfg_mpma_one_port_drop(digi_handle,
                                         subsystem,
                                         chnl);
    }

    PMC_RETURN();

} /* digi_opsa_cfg_dcpb_mpma_chnl_backup_cfg */


/*******************************************************************************
* digi_opsa_cfg_one_fmf1_txjc_cfc_fifo_clrb
* ______________________________________________________________________________
*
* DESCRIPTION:
*   See digi_fw_opsa_cfg_fmf1_txjc_cfc_fifo_clrb().  This API provides a wrapper so
*   that one channel may have the FIFO_CLRB field set or cleared.
*
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   chnl                - chnl to configure
*   value               - Value to set: \n
*                         TRUE:  Set to 1 \n
*                         FALSE: Set to 0 \n
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
PUBLIC void digi_opsa_cfg_one_fmf1_txjc_cfc_fifo_clrb(digi_handle_t *digi_handle,
                                                      UINT32 chnl,
                                                      BOOL8 value)
{
    UINT32 chnl_add_msk[3] = {0, 0, 0};

    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(chnl < 96, DIGI_ERR_CODE_ASSERT, 0, 0);

    digi_opsa_chnl_to_chnl_msk(chnl, chnl_add_msk);

    switch(value)
    {
        case TRUE:
            result = digi_fw_opsa_cfg_fmf1_txjc_cfc_fifo_clrb(digi_handle,
                                                              1, chnl_add_msk);
            break;
        case FALSE:
            result = digi_fw_opsa_cfg_fmf1_txjc_cfc_fifo_clrb(digi_handle,
                                                              0, chnl_add_msk);
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
    }
    PMC_ASSERT(result==PMC_SUCCESS, result, 0, 0);

    PMC_RETURN();

} /* digi_opsa_cfg_one_fmf1_txjc_cfc_fifo_clrb */

/*******************************************************************************
* digi_opsa_cfg_one_tgmp_txjc_cfc_fifo_clrb
* ______________________________________________________________________________
*
* DESCRIPTION:
*   See digi_fw_opsa_cfg_tgmp_txjc_cfc_fifo_clrb().  This API provides a wrapper so
*   that one channel may have the FIFO_CLRB field set or cleared.
*
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   chnl                - chnl to configure
*   value               - Value to set: \n
*                         TRUE:  Set to 1 \n
*                         FALSE: Set to 0 \n
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
PUBLIC void digi_opsa_cfg_one_tgmp_txjc_cfc_fifo_clrb(digi_handle_t *digi_handle,
                                                      UINT32 chnl,
                                                      BOOL8 value)
{
    UINT32 chnl_add_msk[3] = {0, 0, 0};

    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(chnl < 96, DIGI_ERR_CODE_ASSERT, 0, 0);

    digi_opsa_chnl_to_chnl_msk(chnl, chnl_add_msk);

    switch(value)
    {
        case TRUE:
            result = digi_fw_opsa_cfg_tgmp_txjc_cfc_fifo_clrb(digi_handle,
                                                              1, chnl_add_msk);
            break;
        case FALSE:
            result = digi_fw_opsa_cfg_tgmp_txjc_cfc_fifo_clrb(digi_handle,
                                                              0, chnl_add_msk);
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
    }
    PMC_ASSERT(result==PMC_SUCCESS, result, 0, 0);

    PMC_RETURN();

} /* digi_opsa_cfg_one_tgmp_txjc_cfc_fifo_clrb */

/*******************************************************************************
* digi_opsa_chnl_to_chnl_msk
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API sets a bit in a 3 x 32 bit array for the bit identified with input
*   chnl.
*
* INPUTS:
*   chnl                  - chnl number
*
* OUTPUTS:
*   *chnl_msk_ptr         - 3 x 32 bit mask updated by function to set the bit
*                           offset specified by chnl to 1, other bits set to 0
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_opsa_chnl_to_chnl_msk(UINT32 chnl,
                                       UINT32 *chnl_msk_ptr)
{


    PMC_ENTRY();

    PMC_ASSERT(chnl < 96, DIGI_ERR_CODE_ASSERT, 0, 0);

    chnl_msk_ptr[chnl/32] |= 1 << (chnl %32);

    PMC_RETURN();

} /* digi_opsa_chnl_to_chnl_msk */

/*******************************************************************************
* digi_cpb_chnl_qs_ch_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function retrieves the QSn_CH_EN bit that applies is associated with
*   the specified master dpi port channel.
*
*
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   *input_pc_ptr    - util_global_switch_data_t pointer defining inbound port
*   *output_pc_ptr   - util_global_switch_data_t pointer defining outbound port
* OUTPUTS:
*   *qs_ch_en_ptr    - Pointer updated by API call with the
*                      set of QSn_CH_EN bit for the requested
*                      channel.  Only one bit will be set.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_cpb_chnl_qs_ch_en_get(digi_handle_t *digi_handle,
                                       util_global_switch_data_t *input_pc_ptr,
                                       util_global_switch_data_t *output_pc_ptr,
                                       UINT8 *qs_ch_en_ptr)
{

    util_global_switch_data_def_t *input_pc_data_ptr = NULL;
    util_global_switch_data_def_t *output_pc_data_ptr = NULL;

    util_global_cpb_port_data_t *local_input_pc_ptr = (util_global_cpb_port_data_t*)input_pc_ptr;
    util_global_cpb_port_data_t *local_output_pc_ptr = (util_global_cpb_port_data_t*)output_pc_ptr;

    UINT32 dummy, data_dest = 0, egress_mux_input_port = 0xFFFFFFFF;

    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    input_pc_data_ptr = (util_global_switch_data_def_t *)input_pc_ptr;
    output_pc_data_ptr = (util_global_switch_data_def_t *)output_pc_ptr;

    local_input_pc_ptr = &(input_pc_data_ptr->cpb_data);
    local_output_pc_ptr = &(output_pc_data_ptr->cpb_data);

    if(local_output_pc_ptr->port_type == UTIL_GLOBAL_CPB_PORT_ENET_LINE){
        data_dest = (enet_is_link_mapotn_test(digi_handle->enet_line_handle, local_output_pc_ptr->channel, &dummy) == TRUE) ? 1:0;
    }

    /* determine the ingress_mux_output_port, and egress_mux_input_port, function will return
       error if the requested port mapping is invalid */
    result = digi_cpb_push_pop_port_get( digi_handle,
                                         local_input_pc_ptr->port_type,
                                         local_output_pc_ptr->port_type,
                                         local_input_pc_ptr->data_source,
                                         data_dest,
                                         &dummy,
                                         &dummy,
                                         &egress_mux_input_port,
                                         &dummy );
    PMC_ASSERT(result == PMC_SUCCESS, DIGI_ERR_CODE_ASSERT, 0, 0);
    *qs_ch_en_ptr = 1 << egress_mux_input_port;

    PMC_RETURN();

} /* digi_cpb_chnl_qs_ch_en_get*/

/*******************************************************************************
* digi_opsa_chnl_hndl_for_port_chnl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Converts the port id into the appropriate port_type based on the oduk
*  switch flag and then assigns the corresponding handle id and channel
*  handle pointer to the chnl_conn_ptr structure.
*
* INPUTS:
*   *digi_handle         - pointer to DIGI handle instance.
*   port_id              - The numeric value of the CPB or ODUK port type
*                          enumerated type.
*   chnl_id              - Channel identifier
*   is_oduk_switch       - Whether the port_type is associated with the 
*                          ODUK switch defined values (TRUE) or CPB switch
*                          defined values.
*
* OUTPUTS:
*   chnl_hndl_pptr      - Channel handle pointer to channel handle pool
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_opsa_chnl_hndl_for_port_chnl_get(digi_handle_t *digi_handle,
                                                  UINT32 port_id,
                                                  UINT32 chnl_id,
                                                  BOOL8 is_oduk_switch,
                                                  util_global_switch_data_t **chnl_hndl_pptr)
{

    UINT32 mapotn_chnl = 999;
    BOOL8 done = FALSE;

    PMC_ENTRY();

    if (TRUE == is_oduk_switch)
    {
        util_global_oduk_port_t port_type = (util_global_oduk_port_t)port_id;

        switch (port_type)
        {
          case UTIL_GLOBAL_ODUK_PORT_ILKN1:
            if(digi_handle->var.prod_app != DIGI_PROD_APP_SYSOTN_CARD)
            {
                if (chnl_id < DIGI_SIFD_ILKN_ODUK_DPI_CHNLS_MAX)
                {
                    chnl_id += DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
                }

                PMC_ASSERT(chnl_id < ( sizeof(digi_handle->handle_pool.ilkn_1_chnl)/sizeof(digi_handle->handle_pool.ilkn_1_chnl[0]) ), DIGI_ERR_INVALID_ARG, chnl_id, 0 );
                *chnl_hndl_pptr = (util_global_switch_data_t *)&digi_handle->handle_pool.ilkn_1_chnl[chnl_id];
            }
            else
            {
                PMC_ASSERT(chnl_id < ( sizeof(digi_handle->handle_pool.lo_odu_struct)/sizeof(digi_handle->handle_pool.lo_odu_struct[0]) ), DIGI_ERR_INVALID_ARG, chnl_id, 0 );
                *chnl_hndl_pptr = (util_global_switch_data_t *)&digi_handle->handle_pool.lo_odu_struct[chnl_id];
            }

            break;

          case UTIL_GLOBAL_ODUK_PORT_ILKN2:
            if (chnl_id < DIGI_SIFD_ILKN_ODUK_DPI_CHNLS_MAX)
            {
                chnl_id += DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX;
            }

            PMC_ASSERT(chnl_id < ( sizeof(digi_handle->handle_pool.ilkn_2_chnl)/sizeof(digi_handle->handle_pool.ilkn_2_chnl[0]) ), DIGI_ERR_INVALID_ARG, chnl_id, 0 );                
            *chnl_hndl_pptr = (util_global_switch_data_t *)&digi_handle->handle_pool.ilkn_2_chnl[chnl_id];
            break;

          case UTIL_GLOBAL_ODUK_PORT_COREOTN:
            /*
             * For COREOTN need to determine which ODU level the call is
             * operating on. Use the channel value and take the first ODU
             * channel with that is CHANNELIZED.
             */
            if(done == FALSE)
            {
                PMC_ASSERT(chnl_id < ( sizeof(digi_handle->handle_pool.lo_odu_struct)/sizeof(digi_handle->handle_pool.lo_odu_struct[0]) ), DIGI_ERR_INVALID_ARG, chnl_id, 0 );
                if (UTIL_GLOBAL_ODU_UNCHANNELIZED == digi_handle->handle_pool.lo_odu_struct[chnl_id].mem_ptr->payload_format &&
                    (TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.lo_odu_struct[chnl_id].switch_data.header.prov_state)) &&
                    digi_handle->var.prod_app != DIGI_PROD_APP_SYSOTN_CARD)
                {
                    *chnl_hndl_pptr = (util_global_switch_data_t *)&digi_handle->handle_pool.lo_odu_struct[chnl_id];
                    done = TRUE;
                }
            }

            if(done == FALSE)
            {
                PMC_ASSERT(chnl_id < ( sizeof(digi_handle->handle_pool.mo_odu_struct)/sizeof(digi_handle->handle_pool.mo_odu_struct[0]) ), DIGI_ERR_INVALID_ARG, chnl_id, 0 );
                if (UTIL_GLOBAL_ODU_UNCHANNELIZED == digi_handle->handle_pool.mo_odu_struct[chnl_id].mem_ptr->payload_format &&
                    (TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.mo_odu_struct[chnl_id].switch_data.header.prov_state)))
                {
                    *chnl_hndl_pptr = (util_global_switch_data_t *)&digi_handle->handle_pool.mo_odu_struct[chnl_id];
                    done = TRUE;
                }
            }

            if(done == FALSE)
            {
                PMC_ASSERT(chnl_id < ( sizeof(digi_handle->handle_pool.ho_odu_struct)/sizeof(digi_handle->handle_pool.ho_odu_struct[0]) ), DIGI_ERR_INVALID_ARG, chnl_id, 0 );
                if (UTIL_GLOBAL_ODU_UNCHANNELIZED ==  digi_handle->handle_pool.ho_odu_struct[chnl_id].mem_ptr->payload_format &&
                    (TRUE == util_global_rx_or_tx_chnl_prov_status_get(digi_handle->handle_pool.ho_odu_struct[chnl_id].switch_data.header.prov_state)))
                {
                    *chnl_hndl_pptr = (util_global_switch_data_t *)&digi_handle->handle_pool.ho_odu_struct[chnl_id];
                    done = TRUE;
                }
            }

            if(done == FALSE)
            {
                *chnl_hndl_pptr = NULL;
            }
            break;

          case UTIL_GLOBAL_ODUK_PORT_MAPOTN:
            PMC_ASSERT(chnl_id < ( sizeof(digi_handle->handle_pool.mapper_chnl)/sizeof(digi_handle->handle_pool.mapper_chnl[0]) ), DIGI_ERR_INVALID_ARG, chnl_id, 0 );                
            *chnl_hndl_pptr = (util_global_switch_data_t *)&digi_handle->handle_pool.mapper_chnl[chnl_id];
            break;

          default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        }
    }
    else
    {
        util_global_cpb_port_t port_type = (util_global_cpb_port_t)port_id;

        switch (port_type)
        {
          case UTIL_GLOBAL_CPB_PORT_ILKN1:
            PMC_ASSERT(chnl_id < ( sizeof(digi_handle->handle_pool.ilkn_1_chnl)/sizeof(digi_handle->handle_pool.ilkn_1_chnl[0]) ), DIGI_ERR_INVALID_ARG, chnl_id, 0 );               
            *chnl_hndl_pptr = (util_global_switch_data_t *)&digi_handle->handle_pool.ilkn_1_chnl[chnl_id];
            break;
          case UTIL_GLOBAL_CPB_PORT_ILKN2:
            PMC_ASSERT(chnl_id < ( sizeof(digi_handle->handle_pool.ilkn_2_chnl)/sizeof(digi_handle->handle_pool.ilkn_2_chnl[0]) ), DIGI_ERR_INVALID_ARG, chnl_id, 0 );
            *chnl_hndl_pptr = (util_global_switch_data_t *)&digi_handle->handle_pool.ilkn_2_chnl[chnl_id];
            break;
          case UTIL_GLOBAL_CPB_PORT_CBRC:
            PMC_ASSERT(chnl_id < ( sizeof(digi_handle->handle_pool.cbr_chnl)/sizeof(digi_handle->handle_pool.cbr_chnl[0]) ), DIGI_ERR_INVALID_ARG, chnl_id, 0 );
            *chnl_hndl_pptr = (util_global_switch_data_t *)&digi_handle->handle_pool.cbr_chnl[chnl_id];
            break;
          case UTIL_GLOBAL_CPB_PORT_MAPOTN:
             PMC_ASSERT(chnl_id < ( sizeof(digi_handle->handle_pool.mapper_chnl)/sizeof(digi_handle->handle_pool.mapper_chnl[0]) ), DIGI_ERR_INVALID_ARG, chnl_id, 0 );
            *chnl_hndl_pptr = (util_global_switch_data_t *)&digi_handle->handle_pool.mapper_chnl[chnl_id];
            break;
          case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
            if(enet_is_link_mapotn_test(digi_handle->enet_line_handle, chnl_id, &mapotn_chnl) == TRUE)
            {
                /* MAPOTN Channel handle case */
                *chnl_hndl_pptr = (util_global_switch_data_t *)&digi_handle->handle_pool.mapper_chnl[mapotn_chnl];
            } 
            else 
            {
                /* ENET_LINE Channel handle case */
                PMC_ASSERT(chnl_id < ( sizeof(digi_handle->handle_pool.enet_line_chnl)/sizeof(digi_handle->handle_pool.enet_line_chnl[0]) ), DIGI_ERR_INVALID_ARG, chnl_id, 0 );
                *chnl_hndl_pptr = (util_global_switch_data_t *)&digi_handle->handle_pool.enet_line_chnl[chnl_id];
            }
            break;

          case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
            PMC_ASSERT(chnl_id < ( sizeof(digi_handle->handle_pool.enet_sys_chnl)/sizeof(digi_handle->handle_pool.enet_sys_chnl[0]) ), DIGI_ERR_INVALID_ARG, chnl_id, 0 );
            *chnl_hndl_pptr = (util_global_switch_data_t *)&digi_handle->handle_pool.enet_sys_chnl[chnl_id];
            break;

          default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
            break;
        }
    }

    PMC_RETURN();
} /* digi_opsa_chnl_hndl_for_port_chnl_get */


/*******************************************************************************
* digi_opsa_src_chnl_hndl_for_dest_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Converts the port id into the appropriate port_type based on the oduk
*  switch flag and then assigns the corresponding handle id and channel
*  handle pointer to the chnl_conn_ptr structure.
*
* INPUTS:
*   *digi_handle         - pointer to DIGI handle instance.
*   dest_chnl_hndl_ptr   - Destination port for which source is being requested
*   is_oduk_switch       - Whether the port_type is associated with the 
*                          ODUK switch defined values (TRUE) or CPB switch
*                          defined values.
*
* OUTPUTS:
*   src_chnl_hndl_pptr  - Source Channel handle pointer to channel handle pool
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_opsa_src_chnl_hndl_for_dest_get(digi_handle_t *digi_handle,
                                                 util_global_switch_data_t *dest_chnl_hndl_ptr,
                                                 BOOL8 is_oduk_switch,
                                                 util_global_switch_data_t **src_chnl_hndl_pptr)
{
    /* extract the destination port and channel to query for the source
       port and channel */

    BOOL8 is_broadcast = FALSE;
    BOOL8 is_multicast = FALSE;
    util_global_switch_data_def_t *local_output_sw_ptr = (util_global_switch_data_def_t *)dest_chnl_hndl_ptr;

    UINT32 output_port_id = 999, output_chnl_id = 999;
    UINT32 input_port_id = 999, input_chnl_id = 999;

    cpb_handle_t *cpb_handle;

    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    /* get the cpb_handle based on is_oduk_switch */
    if(is_oduk_switch == TRUE)
    {
        cpb_handle = coreotn_oduksw_handle_get(digi_handle->coreotn_handle);
        output_port_id = (UINT32)local_output_sw_ptr->oduk_data.port_type;
        output_chnl_id = (UINT32)local_output_sw_ptr->oduk_data.channel;
    } else {
        cpb_handle = digi_handle->dcpb_handle;
        output_port_id = (UINT32)local_output_sw_ptr->cpb_data.port_type;
        output_chnl_id = (UINT32)local_output_sw_ptr->cpb_data.channel;
    }

    result = cpb_chnl_source_and_type_test(cpb_handle,
                                           output_port_id,
                                           output_chnl_id,
                                           &input_port_id,
                                           &input_chnl_id,
                                           &is_broadcast,
                                           &is_multicast);
        
    /* get the channel handle for the source port and channel */
    if(result == PMC_SUCCESS)
    {

        digi_opsa_chnl_hndl_for_port_chnl_get(digi_handle,
                                              input_port_id,
                                              input_chnl_id,
                                              is_oduk_switch,
                                              src_chnl_hndl_pptr);
        
    }
    else {
        *src_chnl_hndl_pptr = NULL;
    }

    if (!(PMC_SUCCESS == result || CPB_ERR_ZONE_SOURCE_NOT_FOUND == result))
    {
        PMC_ASSERT(result == PMC_SUCCESS, result, 0, 0);
    }

    PMC_RETURN();
} /* digi_opsa_src_chnl_hndl_for_dest_get */

/*******************************************************************************
* digi_oduksw_chnl_qs_ch_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function retrieves the QSn_CH_EN bit that applies is associated with
*   the specified master dpi port channel.
*
*
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   *output_pc_ptr   - util_global_switch_data_t pointer defining outbound port
* OUTPUTS:
*   *qs_ch_en_ptr    - Pointer updated by API call with the
*                      set of QSn_CH_EN bit for the requested
*                      channel.  Only one bit will be set.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_oduksw_chnl_qs_ch_en_get(digi_handle_t *digi_handle,
                                           util_global_switch_data_t *output_pc_ptr,
                                           UINT8 *qs_ch_en_ptr)
{

    util_global_switch_data_def_t *output_pc_data_ptr = NULL;

    util_global_oduk_port_data_t *local_output_pc_ptr = (util_global_oduk_port_data_t*)output_pc_ptr;

    PMC_ENTRY();

    output_pc_data_ptr = (util_global_switch_data_def_t *)output_pc_ptr;

    local_output_pc_ptr = &(output_pc_data_ptr->oduk_data);

    switch(local_output_pc_ptr->port_type)
    {
        case UTIL_GLOBAL_ODUK_PORT_ILKN1:
            *qs_ch_en_ptr = 1;
            break;
        case UTIL_GLOBAL_ODUK_PORT_ILKN2:
            *qs_ch_en_ptr = 1 << 1;
            break;
        case UTIL_GLOBAL_ODUK_PORT_COREOTN:
            *qs_ch_en_ptr = 1 << 2;
            break;
        case UTIL_GLOBAL_ODUK_PORT_MAPOTN:
            *qs_ch_en_ptr = 1 << 3;
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;       
    }

    PMC_RETURN();

} /* digi_oduksw_chnl_qs_ch_en_get */


/*******************************************************************************
* digi_oduksw_chnl_qs_ch_en_get_all
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function retrieves the QSn_CH_EN bit that applies is associated with
*   the specified master dpi port channel.
*
*
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance.
*   output_port     - output port type
* OUTPUTS:
*   *qs_ch_en_ptr    - Pointer updated by API call with the
*                      set of QSn_CH_EN bit for the requested
*                      channel.  Only one bit will be set.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_oduksw_chnl_qs_ch_en_get_all(digi_handle_t *digi_handle,
                                               UINT32 output_port,
                                               UINT8 *qs_ch_en_ptr)
{

    PMC_ENTRY();

    switch(output_port)
    {
        case UTIL_GLOBAL_ODUK_PORT_ILKN1:
            *qs_ch_en_ptr = 1;
            break;
        case UTIL_GLOBAL_ODUK_PORT_ILKN2:
            *qs_ch_en_ptr = 1 << 1;
            break;
        case UTIL_GLOBAL_ODUK_PORT_COREOTN:
            *qs_ch_en_ptr = 1 << 2;
            break;
        case UTIL_GLOBAL_ODUK_PORT_MAPOTN:
            *qs_ch_en_ptr = 1 << 3;
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;       
    }

    PMC_RETURN();

} /* digi_oduksw_chnl_qs_ch_en_get_all */

/*******************************************************************************
* digi_opsa_cfg_mpma_one_port_add
* ______________________________________________________________________________
*
* DESCRIPTION:
*   See digi_fw_opsa_cfg_mpma_port_add().  This API provides a wrapper so
*   that one channel may be added.
*
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance.
*   subsystem       - subsystem to configure
*   chnl            - chnl to configure
*   qs_ch_en        - QSn_CH_EN config mask where
*                     each bit index in mask[3:0] corresponds to n
*                     in QSn_CH_EN.  Multiple bits may be set.
*                     The array must have a size of 96 to cover the maximum
*                     number of instances of MPMA blocks.
*  is_config_only   - Flag used for FW restart upgrades from 3.04 to post
*                     3.06.
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
PRIVATE void digi_opsa_cfg_mpma_one_port_add(digi_handle_t *digi_handle,
                                             hostmsg_opsa_subsystem_cfg_t subsystem,
                                             UINT32 chnl,
                                             UINT8 qs_ch_en,
                                             BOOL8 is_config_only)
{
    UINT8 qs_ch_en_array[96] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    UINT32 chnl_add_msk[3] = {0, 0, 0};

    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(subsystem < LAST_OPSA_HOSTMSG_SS, DIGI_ERR_CODE_ASSERT, 0, 0);
    PMC_ASSERT(chnl < 96, DIGI_ERR_CODE_ASSERT, 0, 0);
    PMC_ASSERT(qs_ch_en < 0x10, DIGI_ERR_INVALID_ARG, 0, 0);

    digi_opsa_chnl_to_chnl_msk(chnl, chnl_add_msk);
    qs_ch_en_array[chnl] = qs_ch_en;

    result = digi_fw_opsa_cfg_mpma_port_add(digi_handle,
                                            subsystem,
                                            chnl_add_msk,
                                            qs_ch_en_array,
                                            is_config_only);
    PMC_ASSERT(result==PMC_SUCCESS, result, 0, 0);


    PMC_RETURN();

} /* digi_opsa_cfg_mpma_one_port_add */

/*******************************************************************************
* digi_opsa_cfg_mpma_one_port_drop
* ______________________________________________________________________________
*
* DESCRIPTION:
*   See digi_fw_opsa_cfg_mpma_port_drop().  This API provides a wrapper so
*   that one channel may be removed.
*
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   subsystem           - subsystem to configure
*   chnl                - chnl to configure
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
PRIVATE void digi_opsa_cfg_mpma_one_port_drop(digi_handle_t *digi_handle,
                                              hostmsg_opsa_subsystem_cfg_t subsystem,
                                              UINT32 chnl)
{
    UINT32 chnl_add_msk[3] = {0, 0, 0};

    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(subsystem < LAST_OPSA_HOSTMSG_SS, DIGI_ERR_CODE_ASSERT, 0, 0);
    PMC_ASSERT(chnl < 96, DIGI_ERR_CODE_ASSERT, 0, 0);

    digi_opsa_chnl_to_chnl_msk(chnl, chnl_add_msk);


    result = digi_fw_opsa_cfg_mpma_port_drop(digi_handle,
                                            subsystem,
                                            chnl_add_msk);
    PMC_ASSERT(result==PMC_SUCCESS, result, 0, 0);


    PMC_RETURN();

} /* digi_opsa_cfg_mpma_one_port_drop */

/*******************************************************************************
* digi_opsa_cfg_mpma_all_chnl_drop
* ______________________________________________________________________________
*
* DESCRIPTION:
*   See digi_fw_opsa_cfg_mpma_port_drop().  This API is used to remove
*   multiple channels from MPMA at one time.
*
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   subsystem           - subsystem to configure
*   chnl_list           - array of channels to be configured
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
PRIVATE void digi_opsa_cfg_mpma_all_chnl_drop(digi_handle_t *digi_handle,
                                              hostmsg_opsa_subsystem_cfg_t subsystem,
                                              UINT32 chnl_list[96])
{
    UINT32 chnl_add_msk[3] = {0, 0, 0};
    UINT32 i;

    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(subsystem < LAST_OPSA_HOSTMSG_SS, DIGI_ERR_CODE_ASSERT, 0, 0);
    
    for (i = 0; i < UTIL_GLOBAL_MAX_NUM_ODU_CHNL; i++)
    {
        if (chnl_list[i] != DIGI_DCI_CHANNEL_UNASSIGNED)
        {
            PMC_ASSERT(chnl_list[i] < 96, DIGI_ERR_CODE_ASSERT, 0, 0);
        }
    }            

    for (i = 0; i < UTIL_GLOBAL_MAX_NUM_ODU_CHNL; i++)
    {
        if (chnl_list[i] != 0xFF)
        {
            digi_opsa_chnl_to_chnl_msk(chnl_list[i], chnl_add_msk);
        }
    }

    result = digi_fw_opsa_cfg_mpma_port_drop(digi_handle,
                                             subsystem,
                                             chnl_add_msk);
    PMC_ASSERT(result==PMC_SUCCESS, result, 0, 0);


    PMC_RETURN();

} /* digi_opsa_cfg_mpma_all_chnl_drop */

/*******************************************************************************
* digi_opsa_cfg_mpmo_one_port_add
* ______________________________________________________________________________
*
* DESCRIPTION:
*   See digi_fw_opsa_cfg_mpmo_port_add().  This API provides a wrapper so
*   that one channel may be added.
*
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   subsystem           - subsystem to configure
*   chnl                - chnl to configure
*   data_target         - Pointer to array of channel data target
*                         configuration bits.
*                         ch_data_target_x[6:4] specifies the
*                         downstream_data_target:
*                         0x0: Tx Serdes ACB LINE_S16
*                         0x1: Tx Serdes ACB SYS_S16
*                         0x2: Tx Serdes ACB SFI5.1
*                         0x3: Stage 1 Mux Tributary Slots
*                         0x4: Stage 2 Mux Tributary Slots
*                         0x5: Backplane
*   acb_inst            - If downstream_data_target = 0x00 to 0x02
*                         (Tx Serdes ACB xxx), then bits 3:0 specifies the
*                         ACB instance used for downstream Tx Serdes ACB
*                         targets:
*                         For Tx Serdes ACB LINE_S16 and SYS_S16,
*                         valid values are 0 to 11.
*                         For LINE_S16, channels 0-11 map to CKCTL ports 0-11.
*                         For SYS_S16, channels 0-11 map to CKCTL ports 14-25.
*                         For Tx Serdes ACB SFI5.1, values of 0 to 1 are valid.
*                         These channels map to CKCTL ports 12-13.
*                         For other values of downstream_data_target,
*                         bits 3:0 are ignored.
*                         The array must have a size of 96 to cover the maximum
*                         number of instances of MPMA blocks.
*   ckctl_port_addr     - The MPMO register address containing the CKCTL_EN
*                         field FW must control.
*   mpmo_proc_type      - Selects between MPMO procedure 1 and 6 for ENET_LINE, 
*                         ENET_SYS and CBRC MPMO instances \n
*                         False: Use MPMO procedure 1 \n
*                         True:  Use MPMO procedure 6
*   cbrc_monitor_type   - the type of status to retrieve from CBRC MPMO instance 
*                         for MPMO procedure 6 \n
*                         False: Use SSF_GEN_I
*                         True:  Use UNDERRUN
*   coreotn_fo2_channel  - for the case mpmo_proc_type is TRUE and cbrc_monitor_type is TRUE,\n
*                         then also need to specify the coreotn_fo2_channel number.
*   coreotn_fo2_ckctl_port - for the case mpmo_proc_type is TRUE and cbrc_monitor_type is TRUE,\n
*                         then also need to specify the coreotn_fo2_ckctl_port
*   is_config_only    - Flag if TRUE results in a OPSA_HOSTMSG_MPMO_CTXT_CFG
*                       being sent rather than a
*                       HOSTMSG_CMD_CFG_OPSA_MPMO_PORT_ADD_ST which would
*                       typically be sent. Used for 3.04 to post 3.06
*                       upgrades to ensure FW OPSA structures are updated.
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
PUBLIC void digi_opsa_cfg_mpmo_one_port_add(digi_handle_t *digi_handle,
                                            hostmsg_opsa_subsystem_cfg_t subsystem,
                                            UINT32 chnl,
                                            digi_opsa_mpmo_target_t data_target,
                                            UINT8 acb_inst,
                                            UINT8 ckctl_port_addr,
                                            BOOL8 mpmo_proc_type,
                                            BOOL8 cbrc_monitor_type,
                                            UINT8 coreotn_fo2_channel,
                                            UINT8 coreotn_fo2_ckctl_port,
                                            BOOL8 is_config_only)
{
    UINT8 ch_data_target[96] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    UINT8 ch_ckctl_port[96] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    UINT32 chnl_add_msk[3] = {0, 0, 0};
    UINT8 coreotn_fo2_channels[24] = {0};
    UINT8 coreotn_fo2_ckctl_ports[24] = {0};
    UINT8 num_coreotn_fo2_channels = 0;

    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(chnl < 96, DIGI_ERR_CODE_ASSERT, 0, 0);
    PMC_ASSERT(subsystem < LAST_OPSA_HOSTMSG_SS, DIGI_ERR_CODE_ASSERT, 0, 0);
    PMC_ASSERT(data_target < LAST_DIGI_OPSA_MPMO_TARGET, DIGI_ERR_INVALID_ARG, 0, 0);
    if(data_target == DIGI_OPSA_MPMO_TARGET_ACB_LINE)
        PMC_ASSERT(acb_inst <= 11, DIGI_ERR_INVALID_ARG, 0, 0);
    if(data_target == DIGI_OPSA_MPMO_TARGET_ACB_SYS)
        PMC_ASSERT(acb_inst <= 11, DIGI_ERR_INVALID_ARG, 0, 0);
    if(data_target == DIGI_OPSA_MPMO_TARGET_ACB_SFI51)
        PMC_ASSERT(acb_inst <= 1, DIGI_ERR_INVALID_ARG, 0, 0);

    digi_opsa_chnl_to_chnl_msk(chnl, chnl_add_msk);
    ch_data_target[chnl] = ((UINT8)data_target << 4) | acb_inst;
    /* if reading BUF_UNDERRUN_I bit,... then mpmo channel must be less than 12 */
    if (1 == cbrc_monitor_type) {
        PMC_ASSERT(chnl <= 11, DIGI_ERR_CODE_ASSERT, 0, 0);
    }
    ch_ckctl_port[chnl] = mpmo_proc_type << 7 | cbrc_monitor_type << 6 | ckctl_port_addr;

    if (mpmo_proc_type) {
        coreotn_fo2_channels[num_coreotn_fo2_channels] = coreotn_fo2_channel;
        coreotn_fo2_ckctl_ports[num_coreotn_fo2_channels] = coreotn_fo2_ckctl_port;
        num_coreotn_fo2_channels ++;
    }

    result = digi_fw_opsa_cfg_mpmo_port_add(digi_handle,
                                            subsystem,
                                            chnl_add_msk,
                                            ch_data_target,
                                            ch_ckctl_port,
                                            coreotn_fo2_channels,
                                            coreotn_fo2_ckctl_ports,
                                            num_coreotn_fo2_channels,
                                            is_config_only);
    PMC_ASSERT(result==PMC_SUCCESS, result, 0, 0);


    PMC_RETURN();

} /* digi_opsa_cfg_mpmo_one_port_add */


/*******************************************************************************
* digi_opsa_cfg_mpma_all_chnl_add
* ______________________________________________________________________________
*
* DESCRIPTION:
*   See digi_fw_opsa_cfg_mpma_port_add().  This API is used to provision
*   multiple channels into MPMA.
*
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance.
*   subsystem       - subsystem to configure
*   input_chnl_list - array of input channnels to configure
*   qs_ch_en        - QSn_CH_EN config mask where
*                     each element in the array corresponds to n
*                     in QSn_CH_EN.  
*                     The array must have a size of 96 to cover the maximum
*                     number of instances of MPMA blocks.
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
PRIVATE void digi_opsa_cfg_mpma_all_chnl_add(digi_handle_t *digi_handle,
                                             hostmsg_opsa_subsystem_cfg_t subsystem,
                                             UINT32 input_chnl_list[96],
                                             UINT8 *qs_ch_en)
{
    UINT8 qs_ch_en_array[96] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    UINT32 chnl_add_msk[3] = {0, 0, 0};

    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    PMC_ENTRY();

    PMC_ASSERT(subsystem < LAST_OPSA_HOSTMSG_SS, DIGI_ERR_CODE_ASSERT, 0, 0);
    
    for (i = 0; i < UTIL_GLOBAL_MAX_NUM_ODU_CHNL; i++)
    {
        if (input_chnl_list[i] != DIGI_DCI_CHANNEL_UNASSIGNED)
        {
            PMC_ASSERT(input_chnl_list[i] < 96, DIGI_ERR_CODE_ASSERT, 0, 0);
            PMC_ASSERT(qs_ch_en[i] < 0x10, DIGI_ERR_INVALID_ARG, 0, 0);
        }        
    }
    
    for (i = 0; i < UTIL_GLOBAL_MAX_NUM_ODU_CHNL; i++)
    {
        if (input_chnl_list[i] != 0xFF)
        {
            digi_opsa_chnl_to_chnl_msk(input_chnl_list[i],
                                       chnl_add_msk);
                                        
            qs_ch_en_array[input_chnl_list[i]] = qs_ch_en[i];
        }
    }
    

    result = digi_fw_opsa_cfg_mpma_port_add(digi_handle,
                                            subsystem,
                                            chnl_add_msk,
                                            qs_ch_en_array,
                                            FALSE);
    PMC_ASSERT(result==PMC_SUCCESS, result, 0, 0);


    PMC_RETURN();

} /* digi_opsa_cfg_mpma_all_chnl_add */

/*******************************************************************************
* digi_opsa_cfg_mpmo_all_chnl_add
* ______________________________________________________________________________
*
* DESCRIPTION:
*   See digi_fw_opsa_cfg_mpmo_port_add().  This API provides a wrapper so
*   that one channel may be added.
*
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   subsystem           - subsystem to configure
*   output_chnl_list    - array of output channels to configure
*   data_target         - Pointer to array of channel data target
*                         configuration bits.
*                         ch_data_target_x[6:4] specifies the
*                         downstream_data_target: \n
*                         0x0: Tx Serdes ACB LINE_S16 \n
*                         0x1: Tx Serdes ACB SYS_S16 \n
*                         0x2: Tx Serdes ACB SFI5.1 \n
*                         0x3: Stage 1 Mux Tributary Slots \n
*                         0x4: Stage 2 Mux Tributary Slots \n
*                         0x5: Backplane
*   acb_inst            - If downstream_data_target = 0x00 to 0x02
*                         (Tx Serdes ACB xxx), then bits 3:0 specifies the
*                         ACB instance used for downstream Tx Serdes ACB
*                         targets: \n
*                         For Tx Serdes ACB LINE_S16 and SYS_S16,
*                         valid values are 0 to 11. \n
*                         For LINE_S16, channels 0-11 map to CKCTL ports 0-11. \n
*                         For SYS_S16, channels 0-11 map to CKCTL ports 14-25. \n
*                         For Tx Serdes ACB SFI5.1, values of 0 to 1 are valid.
*                         These channels map to CKCTL ports 12-13. \n
*                         For other values of downstream_data_target,
*                         bits 3:0 are ignored. \n
*                         The array must have a size of 96 to cover the maximum
*                         number of instances of MPMA blocks.
*   ckctl_port_addr     - The MPMO register address containing the CKCTL_EN
*                         field FW must control.
*   mpmo_proc_type      - Selects between MPMO procedure 1 and 6 for ENET_LINE, 
*                         ENET_SYS and CBRC MPMO instances \n
*                         False: Use MPMO procedure 1 \n
*                         True:  Use MPMO procedure 6
*   cbrc_monitor_type   - the type of status to retrieve from CBRC MPMO instance 
*                         for MPMO procedure 6 \n
*                         False: Use SSF_GEN_I \n
*                         True:  Use UNDERRUN
*   coreotn_fo2_channel  - for the case mpmo_proc_type is TRUE and cbrc_monitor_type is TRUE,\n
*                         then also need to specify the coreotn_fo2_channel number.
*   coreotn_fo2_ckctl_port - for the case mpmo_proc_type is TRUE and cbrc_monitor_type is TRUE,\n
*                         then also need to specify the coreotn_fo2_ckctl_port
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
PUBLIC void digi_opsa_cfg_mpmo_all_chnl_add(digi_handle_t *digi_handle,
                                            hostmsg_opsa_subsystem_cfg_t subsystem,
                                            UINT32 output_chnl_list[96],
                                            digi_opsa_mpmo_target_t data_target[96],
                                            UINT8 acb_inst[96],
                                            UINT8 ckctl_port_addr[96],
                                            BOOL8 mpmo_proc_type,
                                            BOOL8 cbrc_monitor_type,
                                            UINT8 coreotn_fo2_channel,
                                            UINT8 coreotn_fo2_ckctl_port)
{
    UINT8 ch_data_target[96] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    UINT8 ch_ckctl_port[96] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    UINT32 chnl_add_msk[3] = {0, 0, 0};
    UINT8 coreotn_fo2_channels[24] = {0};
    UINT8 coreotn_fo2_ckctl_ports[24] = {0};
    UINT8 num_coreotn_fo2_channels = 0;

    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    
    PMC_ENTRY();

    for (i = 0; i < 96; i++)
    {
        if (output_chnl_list[i] == 0xFF)
        {
            break;
        }
        PMC_ASSERT(output_chnl_list[i] < 96, DIGI_ERR_CODE_ASSERT, 0, 0);
        PMC_ASSERT(data_target[i] < LAST_DIGI_OPSA_MPMO_TARGET, DIGI_ERR_INVALID_ARG, 0, 0);

        if(data_target[i] == DIGI_OPSA_MPMO_TARGET_ACB_LINE)
            PMC_ASSERT(acb_inst[i] <= 11, DIGI_ERR_INVALID_ARG, 0, 0);
        if(data_target[i] == DIGI_OPSA_MPMO_TARGET_ACB_SYS)
            PMC_ASSERT(acb_inst[i] <= 11, DIGI_ERR_INVALID_ARG, 0, 0);
        if(data_target[i] == DIGI_OPSA_MPMO_TARGET_ACB_SFI51)
            PMC_ASSERT(acb_inst[i] <= 1, DIGI_ERR_INVALID_ARG, 0, 0);
    }
    
    PMC_ASSERT(subsystem < LAST_OPSA_HOSTMSG_SS, DIGI_ERR_CODE_ASSERT, 0, 0);

    /* if reading BUF_UNDERRUN_I bit,... then mpmo channel must be less than 12 */
    if (1 == cbrc_monitor_type) 
    {
        for (i = 0; i < UTIL_GLOBAL_MAX_NUM_ODU_CHNL; i++)
        {
            if (output_chnl_list[i] != 0xFF)
            {
                PMC_ASSERT(output_chnl_list[i] <= 11, DIGI_ERR_CODE_ASSERT, 0, 0);
            }
        }
    }
    
    for (i = 0; i < UTIL_GLOBAL_MAX_NUM_ODU_CHNL; i++)
    {
        if (output_chnl_list[i] != 0xFF)
        {
            digi_opsa_chnl_to_chnl_msk(output_chnl_list[i],
                                       chnl_add_msk);
                                        
            ch_data_target[output_chnl_list[i]] = ((UINT8)data_target[i] << 4) | acb_inst[i];
            ch_ckctl_port[output_chnl_list[i]] = mpmo_proc_type << 7 | cbrc_monitor_type << 6 | ckctl_port_addr[i];
            if (mpmo_proc_type && cbrc_monitor_type) {
                PMC_ASSERT(num_coreotn_fo2_channels < 24, DIGI_ERR_CODE_ASSERT, 0, 0);
                coreotn_fo2_channels[num_coreotn_fo2_channels] = coreotn_fo2_channel;
                coreotn_fo2_ckctl_ports[num_coreotn_fo2_channels] = coreotn_fo2_ckctl_port;
                num_coreotn_fo2_channels ++;
            }
        }
    }

    result = digi_fw_opsa_cfg_mpmo_port_add(digi_handle,
                                            subsystem,
                                            chnl_add_msk,
                                            ch_data_target,
                                            ch_ckctl_port,
                                            coreotn_fo2_channels,
                                            coreotn_fo2_ckctl_ports,
                                            num_coreotn_fo2_channels,
                                            FALSE);
                                            
    PMC_ASSERT(result==PMC_SUCCESS, result, 0, 0);


    PMC_RETURN();

} /* digi_opsa_cfg_mpmo_all_chnl_add */


/*******************************************************************************
* digi_opsa_cfg_mpmo_one_port_drop
* ______________________________________________________________________________
*
* DESCRIPTION:
*   See digi_fw_opsa_cfg_mpmo_port_drop().  This API provides a wrapper so
*   that one channel may be removed.
*
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   subsystem           - subsystem to configure
*   chnl                - chnl to configure
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
PUBLIC void digi_opsa_cfg_mpmo_one_port_drop(digi_handle_t *digi_handle,
                                             hostmsg_opsa_subsystem_cfg_t subsystem,
                                             UINT32 chnl)
{
    UINT32 chnl_add_msk[3] = {0, 0, 0};

    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(subsystem < LAST_OPSA_HOSTMSG_SS, DIGI_ERR_CODE_ASSERT, 0, 0);
    PMC_ASSERT(chnl < 96, DIGI_ERR_CODE_ASSERT, 0, 0);

    digi_opsa_chnl_to_chnl_msk(chnl, chnl_add_msk);


    result = digi_fw_opsa_cfg_mpmo_port_drop(digi_handle,
                                            subsystem,
                                            chnl_add_msk);
    PMC_ASSERT(result==PMC_SUCCESS, result, 0, 0);


    PMC_RETURN();

} /* digi_opsa_cfg_mpmo_one_port_drop */

/*******************************************************************************
* digi_opsa_cfg_mpmo_all_chnl_drop
* ______________________________________________________________________________
*
* DESCRIPTION:
*   See digi_fw_opsa_cfg_mpmo_port_drop().  This API is used to remove
*   multiple channels from MPMO at one time.
*
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   subsystem           - subsystem to configure
*   chnl_list           - array of channels to configure
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
PUBLIC void digi_opsa_cfg_mpmo_all_chnl_drop(digi_handle_t *digi_handle,
                                             hostmsg_opsa_subsystem_cfg_t subsystem,
                                             UINT32 chnl_list[96])
{
    UINT32 chnl_add_msk[3] = {0, 0, 0};
    UINT32 i;

    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(subsystem < LAST_OPSA_HOSTMSG_SS, DIGI_ERR_CODE_ASSERT, 0, 0);
    
    for (i = 0; i < UTIL_GLOBAL_MAX_NUM_ODU_CHNL; i++)
    {
        if (chnl_list[i] != 0xFF)
        {
            PMC_ASSERT(chnl_list[i] < UTIL_GLOBAL_MAX_NUM_ODU_CHNL, DIGI_ERR_CODE_ASSERT, 0, 0);
        }
    }
    
    for  (i = 0; i < UTIL_GLOBAL_MAX_NUM_ODU_CHNL; i++)
    {
        if (chnl_list[i] != 0xFF)
        {
            digi_opsa_chnl_to_chnl_msk(chnl_list[i], chnl_add_msk);
        }            
    }

    result = digi_fw_opsa_cfg_mpmo_port_drop(digi_handle,
                                             subsystem,
                                             chnl_add_msk);
    PMC_ASSERT(result==PMC_SUCCESS, result, 0, 0);


    PMC_RETURN();

} /* digi_opsa_cfg_mpmo_all_chnl_drop */


/*******************************************************************************
* digi_opsa_cpb_config_nominal
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Function finds the input pointer and the appropriate parameters and then
*  calls digi_opsa_cpb_activate_cfg to set the MPMO/MPMA accordingly.
*
* INPUTS:
*  digi_handle      - DIGI device handle
*  switch_data_ptr  - Reference to the util_global pointer representing the
*                     output pointer.
*  
* OUTPUTS:
*   none
*
* RETURNS:
*  None 
*
*******************************************************************************/
PRIVATE void digi_opsa_cpb_config_nominal(digi_handle_t *digi_handle,
                                          util_global_switch_data_def_t *switch_data_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 input_port_type;
    UINT32 input_channel;
    BOOL8 is_broadcast = FALSE;
    BOOL8 is_multicast = FALSE;
    BOOL8 is_reader_primary = FALSE;
    BOOL8 is_slave_zone_primary = FALSE;
    util_global_switch_data_t *input_ptr = NULL;

    PMC_ENTRY();
    /*
     * If the odu_ptr is not ACTIVE then there is nothing to do.
     */
    if (!(UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED == switch_data_ptr->header.prov_state &&
          UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE == switch_data_ptr->cpb_data.op_state.active_state))
    {
        PMC_RETURN();
    }
    input_ptr = digi_cpb_input_port_get(digi_handle, switch_data_ptr);

    if (NULL == input_ptr)
    {
        PMC_RETURN();
    }

   result = cpb_chnl_source_type_and_primary_test(digi_handle->dcpb_handle,
                                                  switch_data_ptr->cpb_data.port_type,
                                                  switch_data_ptr->cpb_data.channel,
                                                  &input_port_type,
                                                  &input_channel,
                                                  &is_broadcast,
                                                  &is_multicast,
                                                  &is_reader_primary,
                                                  &is_slave_zone_primary);


   if (PMC_SUCCESS != result)
   {
       PMC_RETURN();
   }

   digi_opsa_cpb_activate_cfg(digi_handle,
                              input_ptr, 
                              (util_global_switch_data_t *)switch_data_ptr,
                              is_reader_primary,
                              is_slave_zone_primary,
                              DIGI_OPSA_CFG_ACTION_ACTIVATE,
                              TRUE);

    PMC_RETURN();

} /* digi_opsa_cpb_config_nominal */


/*******************************************************************************
* digi_opsa_oduksw_config_nominal
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Function finds the input pointer and the appropriate parameters and then
*  calls digi_opsa_oduksw_activate_cfg to set the MPMO/MPMA accordingly.
*
* INPUTS:
*  digi_handle      - DIGI device handle
*  switch_data_ptr  - Reference to the util_global pointer representing the
*                     output pointer.
*  odu_ptr          - Reference to the odu_struct_t if applicable.
*  
* OUTPUTS:
*   none
*
*  RETURNS:
*  PMC_SUCCESS - Operation was successful
*  PMC_ERROR   - Otherwise
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_opsa_oduksw_config_nominal(digi_handle_t *digi_handle,
                                                  util_global_switch_data_def_t *switch_data_ptr,
                                                  odu_struct_t *odu_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 input_port_type;
    UINT32 input_channel;
    BOOL is_broadcast = FALSE;
    BOOL is_multicast = FALSE;
    BOOL is_reader_primary = FALSE;
    BOOL is_slave_primary = FALSE;
    util_global_switch_data_t *input_ptr = NULL;
    BOOL nominal_tx;

    PMC_ENTRY();
    /*
     * If the odu_ptr is not ACTIVE then there is nothing to do.
     */
    if (UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE != 
        switch_data_ptr->oduk_data.op_state.active_state)
    {
        PMC_RETURN(result);
    }

    result = coreotn_oduksw_chnl_source_and_type_test(digi_handle->coreotn_handle,
                                                      switch_data_ptr->oduk_data.port_type,
                                                      switch_data_ptr->oduk_data.channel,
                                                      &input_port_type,
                                                      &input_channel,
                                                      &is_broadcast,
                                                      &is_multicast);
                                                          
    if (PMC_SUCCESS == result)
    { 
        result = coreotn_oduksw_chnl_type_get(digi_handle->coreotn_handle,
                                              switch_data_ptr->oduk_data.port_type,
                                              switch_data_ptr->oduk_data.channel,
                                              &is_reader_primary,
                                              &is_slave_primary,
                                              &is_broadcast,
                                              &is_multicast);
    }  

    if (PMC_SUCCESS == result)
    {
        input_ptr = digi_oduksw_input_port_get(digi_handle, 
                                               input_port_type,
                                               input_channel,
                                               DIGI_ILKN_CHNL_TARGET_ODUK);

        if (NULL == input_ptr)
        {
            result = DIGI_ERR_ODUKSW_NOT_ACTIVE;
        }
    }
    /*
     * Add a check to ensure that the output port type is valid for checking
     * againts the OPSA ODUK_SW activate to ensure an ASSERT is not
     * triggered.
     */
    if (PMC_SUCCESS == result &&
        (UTIL_GLOBAL_ODUK_PORT_ILKN1 ==switch_data_ptr->oduk_data.port_type ||
         UTIL_GLOBAL_ODUK_PORT_ILKN2 ==switch_data_ptr->oduk_data.port_type ||
         UTIL_GLOBAL_ODUK_PORT_COREOTN == switch_data_ptr->oduk_data.port_type ||
         UTIL_GLOBAL_ODUK_PORT_MAPOTN == switch_data_ptr->oduk_data.port_type))
    {
        BOOL8 activate_mpmo = TRUE;

        if ((UTIL_GLOBAL_ODUK_PORT_COREOTN == switch_data_ptr->oduk_data.port_type ||
            (UTIL_GLOBAL_ODUK_PORT_ILKN1 == switch_data_ptr->oduk_data.port_type &&
             digi_handle->var.prod_app == DIGI_PROD_APP_SYSOTN_CARD)) &&
            NULL != odu_ptr)
        {

            nominal_tx = digi_otn_nominal_tx_unchannelized_get(digi_handle,
                                                               (odu_struct_t *)odu_ptr);
            if(nominal_tx)
            {
                activate_mpmo = FALSE;
            }
        }
        digi_opsa_oduksw_activate_cfg(digi_handle,
                                      input_ptr,
                                      (util_global_switch_data_t *)switch_data_ptr,
                                      is_reader_primary,
                                      is_slave_primary,
                                      DIGI_OPSA_CFG_ACTION_ACTIVATE,
                                      activate_mpmo,
                                      TRUE);
    }

    PMC_RETURN(result);

} /* digi_opsa_oduksw_config_nominal */


/*
** Computational Operation Function
*/

/*
** Register Access Operation
*/

#endif /* DOXYGEN_PUBLIC_ONLY */



/*
** End of file
*/
