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
*   DESCRIPTION : This file contains C functions for operating on ODU structs.
*   Functions for usage in higher level systems have prototypes defined in
*   odu_struct_api.h.  Other functions should be considered private and for the
*   internal use of the SW driver.
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include <stddef.h>
#include "coreotn_loc.h"


/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*
** Local Macro Definitions
*/
#define SIZEOF(s,m) \
    ((size_t) sizeof((s *)0)->m))


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


PRIVATE odtu_mux_handle_t* odu_struct_mux_handle_get(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct);

PRIVATE odtu_dmx_handle_t* odu_struct_dmx_handle_get(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct);

PRIVATE PMC_ERROR odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle_t *coreotn_handle,
                                                            odu_struct_t *odu_struct,
                                                            odu_struct_odu_frm_pm_tcm_id_t tcm_num,
                                                            UINT16 *tcm_pool_index);

PRIVATE odu_rfrm_handle_t *odu_struct_rfrm_handle_get(coreotn_handle_t *coreotn_handle,
                                                      odu_struct_t *odu_struct);

PRIVATE odu_tfrm_handle_t *odu_struct_tfrm_handle_get(coreotn_handle_t *coreotn_handle,
                                                      odu_struct_t *odu_struct);

PRIVATE odu_rcp_frm_id_t odu_struct_odu_level_rcp_convert(coreotn_handle_t *coreotn_handle,
                                                          odu_struct_t *odu_struct);

PRIVATE void odu_struct_tcm_pool_rsc_clean(odu_struct_t *odu_struct, 
                                           odu_struct_tcm_pool_rsc_t *dest); 

PRIVATE void odu_struct_remote_rcp_global_cfg(coreotn_handle_t *coreotn_handle,
                                              odu_rcp_frm_id_t odu_rcp_frm_id,                        
                                              BOOL8 enable);

PRIVATE void odu_struct_remote_rcp_cfg(coreotn_handle_t *coreotn_handle,
                                       odu_rcp_frm_id_t odu_rcp_frm_id,                        
                                       UINT32 chnl_id,
                                       BOOL8 enable);    
#endif


/*
** Public Functions
*/



/*
**  Exported Interface Functions - Start
*/
/*******************************************************************************
* odu_struct_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Allocates memory for an ODU struct.
*
* INPUTS:
*   parent          - a pointer to the parent's pmc_handle struct.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   *odu_struct_t   - pointer to the odu_struct
*
* NOTES:
*
*******************************************************************************/
PUBLIC odu_struct_t *odu_struct_ctxt_create(pmc_handle_t *parent)
{
    odu_struct_t *odu_struct;

    PMC_ENTRY();

    odu_struct = (odu_struct_t*)PMC_CTXT_CALLOC(sizeof(odu_struct_t), parent);

    PMC_RETURN(odu_struct);

} /* odu_struct_ctxt_create */

/*******************************************************************************
* coreotn_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys an ODU_STRUCT used for COREOTN subsystem configuration.
*
*
* INPUTS:
*   *odu_struct - pointer to ODU_STRUCT instance to be operated on
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
PUBLIC void odu_struct_ctxt_destroy(odu_struct_t *odu_struct)
{
    PMC_ENTRY();

    PMC_CTXT_FREE(&odu_struct, odu_struct);
    
    PMC_RETURN();

} /* odu_struct_ctxt_destroy */

/*******************************************************************************
* odu_struct_default_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Creates a default ODU_STRUCT filled with NULL values until the base
*   configuration information and other detailed field information is known.
*   This API can be also used when an existing odu_struct needs to be set to
*   default state by removing current configuration data stored in the odu struct.
*
* INPUTS:
*   odu_struct     - pointer to ODU_STRUCT to be operated on
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
PUBLIC void odu_struct_default_set(odu_struct_t *odu_struct)
{
    UINT8 i;
    /* set every field to NULL */

    PMC_ENTRY();

    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);
    
    /* create memory for mem_ptr */

    /* process the wrapper */
    odu_struct->switch_data.oduk_data.channel                    = 0xFF;                                 /* unused channel */
    odu_struct->mem_ptr->odu_type                   = UTIL_GLOBAL_LAST_ODUK;                /* LAST_DIGI_ODUK */
    odu_struct->mem_ptr->odu_level                  = LAST_ODU_STRUCT_LEVEL;                /* LAST_ODU_STRUCT */
    odu_struct->mem_ptr->mux_stage                  = LAST_ODU_STRUCT_OTN_MUX_STAGES;       /* LAST MUX STAGE */
    odu_struct->mem_ptr->mode                       = COREOTN_CHNL_PASSTHRU;                /* LAST CHNL MODE */
    odu_struct->mem_ptr->payload_type               = LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD;    /* LAST PT */
    odu_struct->mem_ptr->mapping_mode               = UTIL_GLOBAL_NO_MAP;                   /* LAST MAPPING MODE */

    /* defect actions CTRL */
    odu_struct->mem_ptr->sf_sd_prop                          = 0;
    odu_struct->mem_ptr->ssf_prop                            = 0;
    odu_struct->mem_ptr->defect_act_ctrl                     = 0;

    odu_struct->mem_ptr->pm_mode                             = 0;

    /* ODU Framer OH context */
    odu_struct->mem_ptr->pm_tcm_res_src                      = 0;
    odu_struct->mem_ptr->pm_tcm_res_force_val                = 0;
    odu_struct->mem_ptr->ftfl_src                            = 0;
    for (i = 0; i < 20; i++)
    {
        odu_struct->mem_ptr->ftfl[i]                        = 0;
    }
    odu_struct->mem_ptr->tcm_act_src                         = 0;
    odu_struct->mem_ptr->tcm_act_force_val                   = 0;
    odu_struct->mem_ptr->aps_pcc_src                         = 0;
    for (i = 0; i < 8; i++)
    {
        odu_struct->mem_ptr->aps_pcc_force_val[i]            = 0;
    }
    odu_struct->mem_ptr->res1_src                            = 0;
    odu_struct->mem_ptr->res1_force_val                      = 0;
    odu_struct->mem_ptr->res2_src                            = 0;

    for (i = 0; i < 6; i++)
    {
        odu_struct->mem_ptr->res2_force_val[i]               = 0;
    }
    odu_struct->mem_ptr->exp_src                             = 0;
    odu_struct->mem_ptr->exp_force_val                       = 0;
    odu_struct->mem_ptr->gcc1_src                            = 0;
    odu_struct->mem_ptr->gcc1_force_val                      = 0;
    odu_struct->mem_ptr->gcc2_src                            = 0;
    odu_struct->mem_ptr->gcc2_force_val                      = 0;

    /* PM configuration */
    odu_struct->mem_ptr->deg_m_pm                            = 0;
    odu_struct->mem_ptr->deg_th_pm                           = 0;
    odu_struct->mem_ptr->tim_mon_pm                          = 0;
    odu_struct->mem_ptr->tim_act_dis_pm                      = 0;

    odu_struct->mem_ptr->bei_biae_src                        = 0;
    odu_struct->mem_ptr->bei_biae_force_val_pm               = 0;
    odu_struct->mem_ptr->bdi_src                             = 0;
    odu_struct->mem_ptr->bdi_force_val_pm                    = 0;
    odu_struct->mem_ptr->stat_src                            = 0;
    odu_struct->mem_ptr->stat_force_val_pm                   = 0;
    odu_struct->mem_ptr->dmp_src                             = 0;
    odu_struct->mem_ptr->dmp_val                             = 0;

    /* TCM pool id based configuration */
    for (i = 0; i < 8; i++)
    {
        odu_struct->mem_ptr->tcm_pool_rsc[i].tcm_instance         = LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES;   /* this seems necessary when the only config data we have is odu_struct FRM APIs are written with argument pm_tcm_num to abstract away implementation detail of pool_id */
        odu_struct->mem_ptr->tcm_pool_rsc[i].tcm_pool_id_rx       = ODU_STRUCT_UNASSIGNED_POOL_ID;
        odu_struct->mem_ptr->tcm_pool_rsc[i].tcm_pool_id_tx       = ODU_STRUCT_UNASSIGNED_POOL_ID;
        odu_struct->mem_ptr->tcm_pool_rsc[i].deg_m                = 0;
        odu_struct->mem_ptr->tcm_pool_rsc[i].deg_th               = 0;
        odu_struct->mem_ptr->tcm_pool_rsc[i].tim_mon              = 0;
        odu_struct->mem_ptr->tcm_pool_rsc[i].tim_act_dis          = 0;
        odu_struct->mem_ptr->tcm_pool_rsc[i].ltc_to_ais           = 0;
                                         
        odu_struct->mem_ptr->tcm_pool_rsc[i].bei_biae_force_val   = 0;
        odu_struct->mem_ptr->tcm_pool_rsc[i].bdi_force_val        = 0;
        odu_struct->mem_ptr->tcm_pool_rsc[i].stat_force_val       = 0;
        odu_struct->mem_ptr->tcm_pool_rsc[i].dmt_src              = 0;
        odu_struct->mem_ptr->tcm_pool_rsc[i].dmt_val              = 0;
    }

    /** MUX data **/
    /* odu payload context */
    odu_struct->mem_ptr->num_tribslots                       = 0;
    for (i = 0; i < 3; i++)
    {
        odu_struct->mem_ptr->tx_trib_loc_mask[i]                = 0;
        odu_struct->mem_ptr->rx_trib_loc_mask[i]                = 0;
    }

    for (i = 0; i < UTIL_GLOBAL_MAX_NUM_TS; i++)
    {
        odu_struct->mem_ptr->trib_slot_ctxt.sub_chnl_ids_tx[i] = 0xFF;
        odu_struct->mem_ptr->trib_slot_ctxt.sub_chnl_ids_rx[i] = 0xFF;
        odu_struct->mem_ptr->trib_slot_ctxt.tx_trib_port[i] = 0xFF;
        odu_struct->mem_ptr->trib_slot_ctxt.rx_trib_port[i] = 0xFF;
    }
    
    odu_struct->mem_ptr->num_layers_rx                      = 0;
    odu_struct->mem_ptr->num_layers_tx                      = 0;
    odu_struct->mem_ptr->maint_sig_rx                       = 0;

    PMC_RETURN();

} /* odu_struct_default_set */


/*******************************************************************************
* odu_struct_base_build
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Creates a base ODU_STRUCT to be used to set up an ODU channel in the COREOTN
*   subsystem. The base ODU_STRUCT will set up the generic properties of an
*   ODU channel at any level.
*
* INPUTS:
*   odu_struct      - pointer to ODU_STRUCT to be operated on
*   chnl            - channel ID of the ODU channel
*   oduk_type       - container type of the ODU channel. See digi_oduk_t.
*   odu_level       - level of ODU struct. See odu_struct_odu_level_t.
*   payload_format  - ODU line payload format defined by the parent HO ODU
*                     struct. See util_global_odu_line_payload_t.
*   mux_stage       - number of multiplexing stages. See odu_struct_otn_mux_stages_t
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
PUBLIC PMC_ERROR odu_struct_base_build(odu_struct_t *odu_struct,
                                       UINT32 chnl,
                                       util_global_odukp_type_t oduk_type,
                                       odu_struct_odu_level_t odu_level,
                                       util_global_odu_line_payload_t payload_format,
                                       odu_struct_otn_mux_stages_t mux_stage)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    /* argument checking */
    if (chnl > 95 || oduk_type > UTIL_GLOBAL_LAST_ODUK
        || odu_level > LAST_ODU_STRUCT_LEVEL
        || payload_format > LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD
        || mux_stage > LAST_ODU_STRUCT_OTN_MUX_STAGES)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    odu_struct->switch_data.oduk_data.channel         = chnl;
    odu_struct->mem_ptr->odu_type        = oduk_type;
    odu_struct->mem_ptr->odu_level       = odu_level;
    odu_struct->mem_ptr->payload_format  = payload_format;
    odu_struct->mem_ptr->mux_stage       = mux_stage;
    odu_struct->mem_ptr->mode            = COREOTN_CHNL_PASSTHRU;


    PMC_RETURN(result);
} /* odu_struct_base_build */

/*******************************************************************************
* odu_struct_chnl_payload_type_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Sets opu_payload_type field in ODU_STRUCT. This API can be used to set
*   PT field of OPU overhead field of an ODU framer.
*
* INPUTS:
*   coreotn_handle  - pointer to COREOTN handle to be operated on
*   odu_struct      - pointer to ODU_STRUCT to be operated on
*   pt              - payload type of the ODU payload
*   direction       - direction to set PT. See odu_struct_stream_dir_t
*                     0 -- Rx
*                     1 -- Tx
*   target          - configuration mode. See odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_chnl_payload_type_set(coreotn_handle_t *coreotn_handle,
                                                  odu_struct_t *odu_struct,
                                                  UINT8 pt,
                                                  odu_struct_stream_dir_t direction,
                                                  odu_struct_target_t target)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    
    odtu_mux_handle_t* odtu_mux_handle;
    odtu_dmx_handle_t* odtu_dmx_handle;

                              
    PMC_ENTRY();

    odu_struct->mem_ptr->payload_type = 0;
    PMC_LOG_TRACE("odu_struct->mem_ptr->payload_type = %u\n", odu_struct->mem_ptr->payload_type);

    odu_struct->mem_ptr->payload_type = pt;

    odtu_mux_handle = odu_struct_mux_handle_get(coreotn_handle,
                                                odu_struct);

    odtu_dmx_handle = odu_struct_dmx_handle_get(coreotn_handle,
                                                odu_struct);


    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {

        /* first MUX */
        if (direction == 1)
        {
            /* Tx direction */
            result = ohfs_insert_pt_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                        odu_struct->switch_data.oduk_data.channel,
                                        pt);
        }
        else 
        {
            /* Rx direction */
            result = ohfs_remove_pt_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                        odu_struct->switch_data.oduk_data.channel,
                                        pt);
        }

    }
    else 
    {
        result = COREOTN_ERR_NO_SW_CONTEXT;
    }

    PMC_RETURN(result);
} /* odu_struct_payload_type_set */

/*******************************************************************************
* odu_struct_defect_actions_ctrl_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Controls whethere or not a defect will contribute to consequential action
*   declarations globally.
*
* INPUTS:
*   coreotn_handle      - pointer to COREOTN handle instance
*   odu_struct          - pointer to ODU_STRUCT to be operated on
*   defect_action_value  - mask to describe each defect action disable controls\n
*                         Bit 0 -- dLTC_ACT_DIS\n
*                                 0 -- dLTC contributes to aBDI consequential
*                                      action declaration\n
*                                 1 -- dLTC does not contribute to aBDI
*                                      consequential action declaration\n
*                         Bit 1 -- dAIS_ACT_DIS\n
*                                 0 -- dAIS contributes to aBDI and aTSF
*                                      consequential action declaration\n
*                                 1 -- dAIS does not contribute to aBDI and aTSF
*                                      consequential action declaration\n
*                         Bit 2 -- dOCI_ACT_DIS\n
*                                 0 -- dOCI contributes to aBDI, aTSF and aAIS
*                                     consequential action declarations\n
*                                 1 -- dOCI does not contribute to aBDI, aTSF
*                                      and aAIS consequential action declarations\n
*                         Bit 3 -- dLCK_ACT_DIS\n
*                                 0 -- dLCK contributes to aBDI, aTSF and aAIS
*                                      consequential action declarations\n
*                                 1 -- dLCK does not contribute to aBDI, aTSF,
*                                      and aAIS consequential action
*                                      declarations
*   defect_action_wr_mask - mask to control which values will get written
*                          Bit 0 -- write dLTC_ACT_DIS bit
*                          Bit 1 -- write dAIS_ACT_DIS bit
*                          Bit 2 -- write dOCI_ACT_DIS bit
*                          Bit 3 -- write dLCK_ACT_DIS bit
*   target              -  configuration target of odu struct data.
*                          see odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_defect_actions_ctrl_set(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    UINT8 defect_action_value,
                                                    UINT8 defect_action_wr_mask,
                                                    odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;

    odu_rfrm_handle_t *local_odu_rfrm_ptr = NULL;
    UINT32 dLTC_dis = 0;
    UINT32 dAIS_dis = 0;
    UINT32 dOCI_dis = 0;
    UINT32 dLCK_dis = 0;
    UINT32 i;

    PMC_ENTRY();

    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        /* process bit mask to configure each defect action */
        for (i = 0; i < 4; i++)
        {
            if ((defect_action_value >> i) & 1)
            {
                if (0 == i) 
                {
                    /* dLTC_ACT_DIS */
                    dLTC_dis = 1;
                }
                else if (1 == i) 
                {
                    /* dAIS_ACT_DIS */
                    dAIS_dis = 1;
                }
                else if (2 == i) 
                {
                    /* dOCI_ACT_DIS */
                    dOCI_dis = 1;
                }
                else 
                {
                    /* dLCK_ACT_DIS */
                    dLCK_dis = 1;
                }
            }
        }
        local_odu_rfrm_ptr = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
        
        if (NULL != local_odu_rfrm_ptr)
        {
            result = odu_rfrm_action_proc_cfg(local_odu_rfrm_ptr, dLTC_dis, dAIS_dis,
                                              dOCI_dis, dLCK_dis, defect_action_wr_mask);
        }
        else 
        {
            result = COREOTN_ERR_NULL_HANDLE;
        }                                              
    }
    else 
    {
        odu_struct->mem_ptr->defect_act_ctrl = defect_action_value & defect_action_wr_mask;
    }

    /* configure defect_act_ctrl */
    PMC_RETURN(result);
} /* odu_struct_defect_actions_ctrl_set */



/*******************************************************************************
* odu_struct_defect_actions_ctrl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the current configuration on defect action controls in ODU_RFRM.
*
* INPUTS:
*   coreotn_handle      - pointer to COREOTN handle instance
*   odu_struct          - pointer to ODU_STRUCT to be operated on
*   ext_mode            - See odu_struct_ext_mode_t 
*
* OUTPUTS:
*   defect_action_value  - mask to describe each defect action disable controls\n
*                         Bit 0 -- dLTC_ACT_DIS\n
*                                 0 -- dLTC contributes to aBDI consequential
*                                      action declaration\n
*                                 1 -- dLTC does not contribute to aBDI
*                                      consequential action declaration\n
*                         Bit 1 -- dAIS_ACT_DIS\n
*                                 0 -- dAIS contributes to aBDI and aTSF
*                                      consequential action declaration\n
*                                 1 -- dAIS does not contribute to aBDI and aTSF
*                                      consequential action declaration\n
*                         Bit 2 -- dOCI_ACT_DIS\n
*                                 0 -- dOCI contributes to aBDI, aTSF and aAIS
*                                     consequential action declarations\n
*                                 1 -- dOCI does not contribute to aBDI, aTSF
*                                      and aAIS consequential action declarations\n
*                         Bit 3 -- dLCK_ACT_DIS\n
*                                 0 -- dLCK contributes to aBDI, aTSF and aAIS
*                                      consequential action declarations\n
*                                 1 -- dLCK does not contribute to aBDI, aTSF,
*                                      and aAIS consequential action
*                                      declarations
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_defect_actions_ctrl_get(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    odu_struct_ext_mode_t ext_mode,
                                                    UINT8 *defect_action_value)
{
    PMC_ERROR result = PMC_SUCCESS;

    UINT32 dLTC_dis = 0;
    UINT32 dAIS_dis = 0;
    UINT32 dOCI_dis = 0;
    UINT32 dLCK_dis = 0;
    odu_rfrm_handle_t *local_odu_rfrm_ptr = NULL;

    PMC_ENTRY();


    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {

        local_odu_rfrm_ptr = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);

        result = odu_rfrm_action_proc_get(local_odu_rfrm_ptr,
                                          &dLTC_dis,
                                          &dAIS_dis,
                                          &dOCI_dis,
                                          &dLCK_dis);

        *defect_action_value =  ((dLCK_dis << 3) | (dOCI_dis << 2) | (dAIS_dis << 1) | dLTC_dis);

    } 
    else 
    {
        *defect_action_value = odu_struct->mem_ptr->defect_act_ctrl;
    }

    
    /* configure defect_act_ctrl */
    PMC_RETURN(result);
} /* odu_struct_defect_actions_ctrl_get */

/*******************************************************************************
* odu_struct_ais_dis_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Controls detection and generation of aAIS consequential action.
*
* INPUTS:
*   coreotn_handle      - pointer to COREOTN handle instance
*   odu_struct          - pointer to ODU_STRUCT to be operated on \n 
*   pm_tcm_num          - PM or TCMi field instance:\n
*                         1 to 6    -- TCM1 to TCM6\n
*                         7         -- Floating TCM\n
*                         8         -- PM NIM
*   ais_dis             - enables or disables aAIS consequential action
*                         0 -- aAIS consequential action is detected and 
*                              processed normally. \n
*                         1 -- aAIS consequential action is disabled. \n
*   target              - configuration target of odu struct data.
*                         see odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_ais_dis_set(coreotn_handle_t *coreotn_handle,
                                        odu_struct_t *odu_struct,
                                        odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                        UINT8 ais_dis,
                                        odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;

    odu_rfrm_handle_t *local_odu_rfrm_ptr = NULL;
    UINT16 tcm_pool_id = ODU_STRUCT_UNASSIGNED_POOL_ID;

    PMC_ENTRY();

    if (pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
    {
        PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
    }
    
    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        local_odu_rfrm_ptr = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
        
        if (NULL != local_odu_rfrm_ptr)
        {
            
            if (pm_tcm_num != ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
            {
                result = odu_struct_tcm_pool_id_from_instance_get(coreotn_handle,
                                                                  odu_struct,
                                                                  ODU_STRUCT_STREAM_RX,
                                                                  pm_tcm_num,
                                                                  &tcm_pool_id);
            }

            if(result == PMC_SUCCESS)
            {
                result = odu_rfrm_pm_tcmi_ais_disable(local_odu_rfrm_ptr,
                                                      odu_struct->switch_data.oduk_data.channel,
                                                      (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                                      ais_dis,
                                                      tcm_pool_id);
            }
        }
        else 
        {
            result = COREOTN_ERR_NULL_HANDLE;
        }                                              
    }
    else 
    {
        /* tcm_pool_id will be used here if we decide to add ais_dis control 
        ** in odu_struct
        */
        result = COREOTN_ERR_NO_SW_CONTEXT;
    }

    
    PMC_RETURN(result);
} /* odu_struct_ais_dis_set */

/*******************************************************************************
* odu_struct_ais_dis_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Retrieves the current configuration on the detection and generation of
*   aAIS consequential action.
*
* INPUTS:
*   coreotn_handle      - pointer to COREOTN handle instance
*   odu_struct          - pointer to ODU_STRUCT to be operated on \n 
*   pm_tcm_num          - PM or TCMi field instance:\n
*                         1 to 6    -- TCM1 to TCM6\n
*                         7         -- Floating TCM\n
*                         8         -- PM NIM
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   ais_dis             - current configuration value of aAIS consequential
*                         action detection and generation.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_ais_dis_get(coreotn_handle_t *coreotn_handle,
                                        odu_struct_t *odu_struct,
                                        odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                        odu_struct_ext_mode_t ext_mode,
                                        UINT8 *ais_dis)
{
    PMC_ERROR result = PMC_SUCCESS;

    odu_rfrm_handle_t *local_odu_rfrm_ptr = NULL;

    PMC_ENTRY();
    
    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES
        || ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        local_odu_rfrm_ptr = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
        
        if (NULL != local_odu_rfrm_ptr)
        {
            result = odu_rfrm_pm_tcmi_ais_dis_get(local_odu_rfrm_ptr,
                                                  odu_struct->switch_data.oduk_data.channel,
                                                  (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                                  ais_dis);
        }
        else 
        {
            result = COREOTN_ERR_NULL_HANDLE;
        }                                              
    }
    else 
    {
        result = COREOTN_ERR_NO_SW_CONTEXT;
    }

    PMC_RETURN(result);
} /* odu_struct_ais_dis_get */

/*******************************************************************************
* odu_struct_ebc_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retrieves COREOTN ODUk far/near end counters for PM and TCM.
*
* INPUTS:
*   coreotn_handle      - pointer to COREOTN handle instance
*   odu_struct          - pointer to ODU_STRUCT to be operated on \n 
*   pm_tcm_num          - PM or TCMi field instance:\n
*                         0         -- PM\n
*                         1 to 6    -- TCM1 to TCM6\n
*
* OUTPUTS:
*   *ebc             - COREOTN ODUk far/near end counters for a PM/TCM layer of
*                      an ODUk channel
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_ebc_get(coreotn_handle_t *coreotn_handle,
                                    odu_struct_t *odu_struct,
                                    odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                    UINT32 ebc[2])
{
    PMC_ERROR result = PMC_SUCCESS;

    odu_rfrm_handle_t *local_odu_rfrm_ptr = NULL;

    PMC_ENTRY();
    
    if (pm_tcm_num > ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_TCM6)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    local_odu_rfrm_ptr = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
    
    if (NULL != local_odu_rfrm_ptr)
    {
        if (pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
        {
            result = odu_rfrm_pm_ebc_get(local_odu_rfrm_ptr,
                                                  odu_struct->switch_data.oduk_data.channel,
                                                  (odu_rfrm_odukp_counters_t *)ebc);
        }
        else
        {
            result = odu_rfrm_tcm_ebc_get(local_odu_rfrm_ptr,
                                                  odu_struct->switch_data.oduk_data.channel,
                                                  (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                                  (odu_rfrm_odukt_counters_t *)ebc);
        }
    }
    else 
    {
        result = COREOTN_ERR_NULL_HANDLE;
    }

    PMC_RETURN(result);
} /* odu_struct_ebc_get */

/*******************************************************************************
* odu_struct_pm_tcmi_tsd_tsf_dis_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   configures whether or not aTSD and aTSF consequential actions are generated  
*   as a result of TSD or TSF defect detection.
*
* INPUTS:
*   coreotn_handle      - pointer to COREOTN handle instance
*   odu_struct          - pointer to ODU_STRUCT to be operated on \n 
*   pm_tcm_num          - PM or TCMi field instance:\n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6\n
*                         7         -- Floating TCM\n
*                         8         -- PM NIM
*   defect_ctrl         - defect control to configure
*                         0 -- TSD consequential action
*                         1 -- TSF consequential action
*                         2 -- both TSD and TSF consequential action
*   tsd_dis             - Controls the aTSD consequential action \n
*                         0 -- The aTSB consequential action is detected and
*                              processed normally. \n
*                         1 -- The aTSD consequential action is disabled.
*   tsf_dis             - Controls the aTSF consequential action \n
*                         0 -- The aTSF consequential action is detected and
*                              processed normally. \n
*                         1 -- The aTSF consequential action is disabled.
*   target              - configuration target of odu struct data.
*                         see odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_pm_tcmi_tsd_tsf_dis_set(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                    UINT8 defect_ctrl,
                                                    UINT8 tsd_dis,
                                                    UINT8 tsf_dis,
                                                    odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t *local_odu_rfrm_ptr = NULL;
    /* set to unused as default */
    UINT16 tcm_pool_id = ODU_STRUCT_UNASSIGNED_POOL_ID;

    PMC_ENTRY();

    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES
        || defect_ctrl > 2
        || tsd_dis > 1 || tsf_dis > 1 
        || target > LAST_CONFIG_TARGET)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        local_odu_rfrm_ptr = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
        
        if (NULL != local_odu_rfrm_ptr)
        {
            if(pm_tcm_num != ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
            {
                result = odu_struct_tcm_pool_id_from_instance_get(coreotn_handle,
                                                                  odu_struct,
                                                                  ODU_STRUCT_STREAM_RX,
                                                                  pm_tcm_num,
                                                                  &tcm_pool_id);
            }

            if(result == PMC_SUCCESS)
            {
                result = odu_rfrm_pm_tcmi_tsd_tsf_proc_cfg(local_odu_rfrm_ptr,
                                                           odu_struct->switch_data.oduk_data.channel,
                                                           (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                                           tcm_pool_id,
                                                           defect_ctrl,
                                                           tsd_dis,
                                                           tsf_dis);
            }
        }
        else {
            result = COREOTN_ERR_NULL_HANDLE;
        }                                              
    }

    
    PMC_RETURN(result);
} /* odu_struct_pm_tcmi_tsd_tsf_dis_set */

/*******************************************************************************
* odu_struct_pm_tcmi_tsd_tsf_dis_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This function retreives the current configuration of aTSD and aTSF defect 
*   generation and detection.
*
* INPUTS:
*   coreotn_handle      - pointer to COREOTN handle instance
*   odu_struct          - pointer to ODU_STRUCT to be operated on \n 
*   pm_tcm_num          - PM or TCMi field instance:\n
*                         1 to 6    -- TCM1 to TCM6\n
*                         7         -- Floating TCM\n
*                         8         -- PM NIM
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   *tsd_dis            - current configuration to disable the aTSD  
*                         consequential action \n
*   *tsf_dis            - current configuration to disable the aTSF  
*                         consequential action \n
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_pm_tcmi_tsd_tsf_dis_get(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                    odu_struct_ext_mode_t ext_mode,
                                                    UINT8 *tsd_dis,
                                                    UINT8 *tsf_dis)
{
    PMC_ERROR result = PMC_SUCCESS;

    odu_rfrm_handle_t *local_odu_rfrm_ptr = NULL;

    PMC_ENTRY();
    
    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES
        || ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        /* find the correct framer */
        local_odu_rfrm_ptr = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);

        if (NULL != local_odu_rfrm_ptr)
        {
            result = odu_rfrm_pm_tcmi_tsd_tsf_dis_get(local_odu_rfrm_ptr,
                                                      odu_struct->switch_data.oduk_data.channel,
                                                      (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                                      tsd_dis, tsf_dis);
        }
        else {
            PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
        }
    }
    
    PMC_RETURN(result);
} /* odu_struct_pm_tcmi_tsd_tsf_dis_get */



/*******************************************************************************
* odu_struct_tcmi_ltc_to_ais_tsf_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   configures if a loss of tandem connection defect will contribute to the
*   generation of AIS ans TSF consequenctial action for a TCM layer, a floating
*   TCM or PM NIM layer.
*
* INPUTS:
*   coreotn_handle      - pointer to COREOTN handle instance to be operated on
*   odu_struct          - pointer to ODU struct to be operated on
*   pm_tcm_num          - PM or TCMi field instance:\n
*                         1 to 6    -- TCM1 to TCM6\n
*                         7         -- Floating TCM\n
*                         8         -- PM NIM
*   dLTC_dis            - Controls whether or not dLTC defect contributes to
*                         declaration of AIS and TSF consequential action \n
*                         0 -- dLTC defect contributes to the aAIS and aTSF
*                              consequential action declaration. \n
*                         1 -- dLTC defect does not contribute to aAIS and aTSF
*                              consequential action declaration.
*   target              - configuration target of odu struct data.
*                         See odu_struct_target_t
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tcmi_ltc_to_ais_tsf_set(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                    BOOL8 dLTC_dis,
                                                    odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t *local_odu_rfrm_ptr = NULL;
    UINT16 tcm_pool_index;

    PMC_ENTRY();

    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES
        || dLTC_dis > 1
        || target > LAST_CONFIG_TARGET)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    /* Pool based resource configuration */
    result = odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle,
                                                       odu_struct,
                                                       pm_tcm_num,
                                                       &tcm_pool_index); 
    /* update ODU struct */
    if (PMC_SUCCESS == result)
    {
        /* find tcm_pool_id_<tcm_pool_index> */
        odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].ltc_to_ais = dLTC_dis; 
    }

    if (PMC_SUCCESS == result && target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        /* find the correct framer */
        local_odu_rfrm_ptr = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);

        if (NULL != local_odu_rfrm_ptr)
        {
            result = odu_rfrm_tcmi_ltc_to_ais_tsf_cfg(local_odu_rfrm_ptr,
                                                      odu_struct->switch_data.oduk_data.channel,
                                                      (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                                      dLTC_dis);
        }
        else {
            result = COREOTN_ERR_NULL_HANDLE;
        }
    }

    PMC_RETURN(result);
} /* odu_struct_tcmi_ltc_to_ais_tsf_set */


/*******************************************************************************
* odu_struct_tcmi_ltc_to_ais_tsf_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retrieves the mode for generation of AIS and TSF consequenctial action from
*   dLTC for a TCM layer, a floating TCM or PM NIM layer.
*
* INPUTS:
*   coreotn_handle      - pointer to COREOTN handle instance to be operated on
*   odu_struct          - pointer to ODU struct to be operated on
*   pm_tcm_num          - PM or TCMi field instance:\n
*                         1 to 6    -- TCM1 to TCM6\n
*                         7         -- Floating TCM\n
*                         8         -- PM NIM
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   *dLTC_dis          - mode for generation of AIS consequential action from
*                         dLTC
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tcmi_ltc_to_ais_tsf_get(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                    odu_struct_ext_mode_t ext_mode,
                                                    BOOL8 *dLTC_dis)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t *local_odu_rfrm_ptr = NULL;
    UINT16 tcm_pool_index;

    PMC_ENTRY();
    
    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES
        || ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        /* find the correct framer */
        local_odu_rfrm_ptr = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);

        if (NULL != local_odu_rfrm_ptr)
        {
            result = odu_rfrm_tcmi_ltc_to_ais_tsf_get(local_odu_rfrm_ptr,
                                                      odu_struct->switch_data.oduk_data.channel,
                                                      (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                                      dLTC_dis);
        }
        else {
            PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
        }
    }
    else {
        /* Pool based resource configuration */
        result = odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle,
                                                           odu_struct,
                                                           pm_tcm_num,
                                                           &tcm_pool_index);                                                       

        /* read from ODU struct */
        if (PMC_SUCCESS == result)
        {
            /* find tcm_pool_id_<tcm_pool_index> */   
            *dLTC_dis = odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].ltc_to_ais;   
        }
    }

    PMC_RETURN(result);
} /* odu_struct_tcmi_ltc_to_ais_get */

/*******************************************************************************
* odu_struct_chnl_maint_sig_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   configures the forced insertion of ODUk maintenance signals on the ODUk
*   channel represented in the ODU struct
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   *odu_struct         - pointer to ODU struct to be operated on
*   direction           - direction of the signal insertion. See odu_struct_stream_dir_t.
*                         0 -- Sink \n
*                         1 -- Source
*   force_alarm_sig     - enum,requested maintenance signal:
*                         CORE_OTN_NO_MAINT_SIG 
*                         CORE_OTN_ODUK_AIS 
*                         CORE_OTN_ODUK_LCK 
*                         CORE_OTN_ODUK_OCI
*   target              - configuration target of odu struct data.
*                         See odu_struct_target_t
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_chnl_maint_sig_set(coreotn_handle_t *coreotn_handle,
                                               odu_struct_t *odu_struct,
                                               odu_struct_stream_dir_t direction,
                                               coreotn_maint_sig_type_t force_alarm_sig,
                                               odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t *local_odu_rfrm_ptr = NULL;
    odu_tfrm_handle_t *local_odu_tfrm_ptr = NULL;


    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    /* argument checking */
    if (force_alarm_sig > CORE_OTN_ODUK_OCI || direction > 1 || target > LAST_CONFIG_TARGET)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (direction == 0)
    {
        /* save the context to be used in the layer processing */
        odu_struct->mem_ptr->maint_sig_rx = (UINT8)force_alarm_sig;
    }
    
    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        /* find the correct framer */
        if (direction == 1)
        {
            /* Tx direction */
            local_odu_tfrm_ptr = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

            if (NULL != local_odu_tfrm_ptr)
            {

                
                if (force_alarm_sig != CORE_OTN_NO_MAINT_SIG)
                {

                    
                    
                    /* insert maint_sig */
                    if (PMC_SUCCESS == result)
                    {
                        result = odu_tfrm_odukp_mux_maint_sig_cfg(local_odu_tfrm_ptr,
                                                                  odu_struct->switch_data.oduk_data.channel,
                                                                  (UINT32)force_alarm_sig);
                    }
                }
                else {
                    /* remove maint_sig */
                    result = odu_tfrm_odukp_mux_maint_sig_cfg(local_odu_tfrm_ptr,
                                                              odu_struct->switch_data.oduk_data.channel,
                                                              (UINT32)force_alarm_sig);



                }
            }
            else {
                PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
            }
        }
        else {
            /* Rx direction */
            local_odu_rfrm_ptr = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);

            if(odu_struct->mem_ptr->odu_level == ODU_STRUCT_LEVEL_3B_ODU)
            {

                coreotn_handle->var.prev_fo1_rfrm_maint_sig[odu_struct->switch_data.oduk_data.channel] = force_alarm_sig;

                if(coreotn_handle->var.dummy_fo1_maint_sig[odu_struct->switch_data.oduk_data.channel] == TRUE)
                {
                    if (NULL != local_odu_rfrm_ptr)
                    {
                        /*if force_alarm_sig != 0, we still want to apply new maintenance signal*/
                        if(force_alarm_sig != CORE_OTN_NO_MAINT_SIG)
                        {
                            result = odu_rfrm_chnl_maint_sig_cfg(local_odu_rfrm_ptr,
                                                                 odu_struct->switch_data.oduk_data.channel,
                                                                 (UINT32)force_alarm_sig);
                        }
                    }
                    else {
                         PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
                    }

                }
                else
                {
                    if (NULL != local_odu_rfrm_ptr)
                    {
                        result = odu_rfrm_chnl_maint_sig_cfg(local_odu_rfrm_ptr,
                                                             odu_struct->switch_data.oduk_data.channel,
                                                             (UINT32)force_alarm_sig);
                    }
                    else {
                         PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
                    }
                }

            }
            else if(odu_struct->mem_ptr->odu_level == ODU_STRUCT_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC)
            {

                coreotn_handle->var.prev_fmf1_sysotn_rfrm_maint_sig[odu_struct->switch_data.oduk_data.channel] = force_alarm_sig;


                if(coreotn_handle->var.dummy_fmf1_sysotn_maint_sig[odu_struct->switch_data.oduk_data.channel] == TRUE)
                {
                    if (NULL != local_odu_rfrm_ptr)
                    {
                        /*if force_alarm_sig != 0, we still want to apply new maintenance signal*/
                        if(force_alarm_sig != CORE_OTN_NO_MAINT_SIG)
                        {
                            result = odu_rfrm_chnl_maint_sig_cfg(local_odu_rfrm_ptr,
                                                                 odu_struct->switch_data.oduk_data.channel,
                                                                 (UINT32)force_alarm_sig);
                        }
                    }
                    else {
                         PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
                    }

                }
                else
                {


                    if (NULL != local_odu_rfrm_ptr)
                    {
                        result = odu_rfrm_chnl_maint_sig_cfg(local_odu_rfrm_ptr,
                                                             odu_struct->switch_data.oduk_data.channel,
                                                             force_alarm_sig);
                    }
                    else {
                         PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
                    }
                }
            }
            else
            {
                if (NULL != local_odu_rfrm_ptr)
                {
                    result = odu_rfrm_chnl_maint_sig_cfg(local_odu_rfrm_ptr,
                                                         odu_struct->switch_data.oduk_data.channel,
                                                         force_alarm_sig);
                }
                else {
                     PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
                }

            }            
        }
    }

    PMC_RETURN(result);
} /* odu_struct_maint_sig_set */


/*******************************************************************************
* odu_struct_odukp_pck_maint_sig_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   configures the forced insertion of ODUk maintenance signals on the ODUk
*   channel represented in the ODU struct.  This is a special API
*   for forcing maintenance at the ODUkP packet adaptation (on top
*   of the PM layer, but before ODUkT adaptation).  Use this API
*   if you wish to insert TCMs on top of the maintenance signal.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   *odu_struct         - pointer to ODU struct to be operated on
*   force_alarm_sig     - Enum, see coreotn_maint_sig_type_t
*                         ODUk Alarm Signal Control:
*                         CORE_OTN_NO_MAINT_SIG 
*                         CORE_OTN_ODUK_AIS 
*                         CORE_OTN_ODUK_LCK 
*                         CORE_OTN_ODUK_OCI 
*   target              - configuration target of odu struct data.
*                         See odu_struct_target_t
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_odukp_pck_maint_sig_set(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    coreotn_maint_sig_type_t force_alarm_sig,
                                                    odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_ptr = NULL;


    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    /* argument checking */
    if (force_alarm_sig > CORE_OTN_ODUK_OCI || target > LAST_CONFIG_TARGET)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
            local_odu_tfrm_ptr = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

            if (NULL != local_odu_tfrm_ptr)
            {
                
                if (force_alarm_sig != CORE_OTN_NO_MAINT_SIG)
                {
                         
                    /* insert maint_sig */
                    if (PMC_SUCCESS == result)
                    {
                        result = odu_tfrm_odukp_pck_maint_sig_cfg(local_odu_tfrm_ptr,
                                                                  odu_struct->switch_data.oduk_data.channel,
                                                                  force_alarm_sig);
                    }
                }
                else {
                    /* remove maint_sig */
                    result = odu_tfrm_odukp_pck_maint_sig_cfg(local_odu_tfrm_ptr,
                                                              odu_struct->switch_data.oduk_data.channel,
                                                              force_alarm_sig);
                }
            }
            else {
                PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
            }
    }

    PMC_RETURN(result);
} /* odu_struct_odukp_pck_maint_sig_set */

/*******************************************************************************
* odu_struct_odukp_pck_maint_sig_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Retrieves the forced insertion of ODUk maintenance signals on the ODUk
*   channel represented in the ODU struct.  This is a special API
*   for forcing maintenance at the ODUkP packet adaptation (on top
*   of the PM layer, but before ODUkT adaptation). 
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   *odu_struct         - pointer to ODU struct to be operated on
*
* OUTPUTS:
*   *force_alarm_sig    - Enum, see coreotn_maint_sig_type_t
*                         ODUk Alarm Signal Control:
*                         CORE_OTN_NO_MAINT_SIG 
*                         CORE_OTN_ODUK_AIS 
*                         CORE_OTN_ODUK_LCK 
*                         CORE_OTN_ODUK_OCI 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_odukp_pck_maint_sig_get(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    coreotn_maint_sig_type_t *force_alarm_sig)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_ptr = NULL;
    UINT32 force_alm_val = 0;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);
    PMC_ASSERT(force_alarm_sig != NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);

    local_odu_tfrm_ptr = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

    if (NULL != local_odu_tfrm_ptr)
    {
        (void)odu_tfrm_odukp_pck_maint_sig_get(local_odu_tfrm_ptr,
                                               odu_struct->switch_data.oduk_data.channel,
                                               &force_alm_val);
        *force_alarm_sig = (coreotn_maint_sig_type_t)force_alm_val;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
    }

    PMC_RETURN(result);
} /* odu_struct_odukp_pck_maint_sig_set */

/*******************************************************************************
* odu_struct_tcm_maint_sig_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   configures the forced insertion of ODUk maintenance signals on the ODUk
*   channel represented in the ODU struct.  This will insert a maintenance
*   signal on top of the TCM specified in pm_tcm_num
*
*   It is not valid to provide the PM layer for this API.  To force on
*   top of PM layer in the TX direction, use odu_struct_chnl_maint_sig_set 
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   *odu_struct         - pointer to ODU struct to be operated on
*   direction           - direction of the signal insertion. See odu_struct_stream_dir_t.
*                         0 -- Sink \n
*                         1 -- Source
*   pm_tcm_num          - TCMi instance. See odu_struct_odu_frm_pm_tcm_id_t.
*                         (not valid to provide PM layer for this API)
*   force_alarm_sig     - enum,requested maintenance signal: 
*                         CORE_OTN_NO_MAINT_SIG 
*                         CORE_OTN_ODUK_AIS 
*                         CORE_OTN_ODUK_LCK 
*                         CORE_OTN_ODUK_OCI 
*   target              - configuration target of odu struct data.
*                         See odu_struct_target_t
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tcm_maint_sig_set(coreotn_handle_t *coreotn_handle,
                                              odu_struct_t *odu_struct,
                                              odu_struct_stream_dir_t direction,
                                              odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                              coreotn_maint_sig_type_t force_alarm_sig,
                                              odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t *local_odu_rfrm_ptr = NULL;
    odu_tfrm_handle_t *local_odu_tfrm_ptr = NULL;
    UINT16 tcm_pool_index;
    UINT16 tcm_pool_id;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    /* argument checking */
    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES ||
        force_alarm_sig > CORE_OTN_ODUK_OCI || direction > 1 || target > LAST_CONFIG_TARGET
        || pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM ||
        (direction == 1 && (force_alarm_sig == CORE_OTN_ODUK_AIS || force_alarm_sig == CORE_OTN_ODUK_OCI))) /*only valid to insert LCK in ODUkT*/
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    /* store in odu_struct */
    /* Pool based resource configuration */
    result = odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle, odu_struct, pm_tcm_num, &tcm_pool_index);

    /* read from ODU struct */
    if (PMC_SUCCESS == result)
    {
        /* find tcm_pool_id_<tcm_pool_index> */   
        if (direction == 1)
        {
            /* source TX */
            odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].maint_sig_tx = force_alarm_sig;
            tcm_pool_id = odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].tcm_pool_id_tx;
        }
        else {
            /* sink RX */
            odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].maint_sig_rx = force_alarm_sig;
            tcm_pool_id = odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].tcm_pool_id_rx;
        }    
    }

    if (PMC_SUCCESS == result && target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
                                                                                                              
        if (direction == 1)
        {
            /* source TX */
            local_odu_tfrm_ptr = odu_struct_tfrm_handle_get(coreotn_handle,
                                                           odu_struct);

            if (NULL != local_odu_tfrm_ptr)
            {
                /*
                ** 1) first find the sub-layers using the information available in ODU_RFRM layer hierarchy
                ** 2) set the mode of each sub-layer to be transparent
                ** 3) insert maintenace signal on the desired layer
                */
               
                /* insert a maintenance signal */
                if (force_alarm_sig != CORE_OTN_NO_MAINT_SIG)
                {
                    /* insert a maintenance signal */
                    if (PMC_SUCCESS == result)
                    {
                        result = odu_tfrm_odukt_maint_sig_cfg(local_odu_tfrm_ptr,
                                                              odu_struct->switch_data.oduk_data.channel,
                                                              (odu_tfrm_pm_tcm_id_t)pm_tcm_num,
                                                              force_alarm_sig);
                    }
                }
                else {
                    /* remove a maintenance signal */
                    result = odu_tfrm_odukt_maint_sig_cfg(local_odu_tfrm_ptr,
                                                          odu_struct->switch_data.oduk_data.channel,
                                                          (odu_tfrm_pm_tcm_id_t)pm_tcm_num,
                                                          force_alarm_sig);          
                }


            }
            else {
                result = COREOTN_ERR_NULL_HANDLE;
            }
        }
        else {
            /* Sink RX */
            local_odu_rfrm_ptr = odu_struct_rfrm_handle_get(coreotn_handle,
                                                            odu_struct);

            if (NULL != local_odu_rfrm_ptr)
            {
                /* insert MAINT_SIG over the desired layer */
                if (PMC_SUCCESS == result)
                {
                    /* odu_rfrm_tcm_maint_sig_cfg looks after clearing all of the remove oh bits */
                    result = odu_rfrm_tcm_maint_sig_cfg(local_odu_rfrm_ptr,
                                                        odu_struct->switch_data.oduk_data.channel,
                                                        (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                                        force_alarm_sig);
                }
                /* if maintenance  */
            }
            else {
                result = COREOTN_ERR_NULL_HANDLE;
            }
        }
    }

    PMC_RETURN(result);
} /* odu_struct_tcm_maint_sig_set */

/*******************************************************************************
* odu_struct_rx_tcm_maint_sig_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retreives the maintenance signal on a TCM layer on the receive ODUk channel
*   represented in the ODU struct.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   *odu_struct         - pointer to ODU struct to be operated on
*   tcm_num             - TCMi instance. See odu_struct_odu_frm_pm_tcm_id_t.
*   ext_mode            - extraction mode of the requested field. \n
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   *maint_sig          - Retreived maintenance signal
*                         CORE_OTN_NO_MAINT_SIG 
*                         CORE_OTN_ODUK_AIS 
*                         CORE_OTN_ODUK_LCK 
*                         CORE_OTN_ODUK_OCI
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_tcm_maint_sig_get(coreotn_handle_t *coreotn_handle,
                                                 odu_struct_t *odu_struct,
                                                 odu_struct_odu_frm_pm_tcm_id_t tcm_num,
                                                 odu_struct_ext_mode_t ext_mode,
                                                 coreotn_maint_sig_type_t *maint_sig)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t *local_odu_rfrm_ptr = NULL;
    UINT16 tcm_pool_index;
    UINT8 maint_sig_val = 0;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);



    /* argument checking */
    if (tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES ||
        tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM ||
        ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        /* find the correct framer handle */
        local_odu_rfrm_ptr = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);

        if (NULL != local_odu_rfrm_ptr)
        {
            result = odu_rfrm_tcm_maint_sig_get(local_odu_rfrm_ptr,
                                                odu_struct->switch_data.oduk_data.channel,
                                                (odu_rfrm_pm_tcm_id_t)tcm_num,
                                                &maint_sig_val);
            /*
              00: No maintenance signal is forced.
              01: Force ODUk-AIS insertion.
              10: Force ODUk-LCK insertion.
              11: Force ODUk-OCI insertion.
            */
            switch(maint_sig_val)
            {
            case 0:
                *maint_sig = CORE_OTN_NO_MAINT_SIG;
                break;

            case 1:
                *maint_sig = CORE_OTN_ODUK_AIS;
                break;

            case 2:
                *maint_sig = CORE_OTN_ODUK_LCK;
                break;

            case 3:
                *maint_sig = CORE_OTN_ODUK_OCI;
                break;

            default:
                *maint_sig = LAST_CORE_OTN_MAINT_SIG;
                result = COREOTN_ERR_INVALID_PARAMETERS;
                break;
            }
        }
        else {
            result = COREOTN_ERR_NULL_HANDLE;
        }                                                
    }
    else {
        /* Pool based resource configuration */
        result = odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle, odu_struct, tcm_num, &tcm_pool_index);

        if (PMC_SUCCESS == result)
        {
            *maint_sig = (coreotn_maint_sig_type_t) odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].maint_sig_rx;
        }
    }

    PMC_RETURN(result);
} /* odu_struct_rx_tcm_maint_sig_get */



/*******************************************************************************
* odu_struct_tx_tcm_maint_sig_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retreives the maintenance signal on a TCM layer on the receive ODUk channel
*   represented in the ODU struct.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   *odu_struct         - pointer to ODU struct to be operated on
*   tcm_num             - TCMi instance. See odu_struct_odu_frm_pm_tcm_id_t.
*   ext_mode            - extraction mode of the requested field. \n
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   *maint_sig          - Retreived maintenance signal
*                         CORE_OTN_NO_MAINT_SIG 
*                         CORE_OTN_ODUK_AIS 
*                         CORE_OTN_ODUK_LCK 
*                         CORE_OTN_ODUK_OCI
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tx_tcm_maint_sig_get(coreotn_handle_t *coreotn_handle,
                                                 odu_struct_t *odu_struct,
                                                 odu_struct_odu_frm_pm_tcm_id_t tcm_num,
                                                 odu_struct_ext_mode_t ext_mode,
                                                 coreotn_maint_sig_type_t *maint_sig)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_ptr = NULL;
    UINT8 maint_sig_val = 0;
    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);



    /* argument checking */
    if (tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES ||
        tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM ||
        ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }


    /* find the correct framer handle */
    local_odu_tfrm_ptr = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

    if (NULL != local_odu_tfrm_ptr)
    {

        result = odu_tfrm_odukt_maint_sig_get(local_odu_tfrm_ptr,
                                              odu_struct->switch_data.oduk_data.channel,
                                              (odu_tfrm_pm_tcm_id_t)tcm_num,
                                              &maint_sig_val);
    }
    else {
        result = COREOTN_ERR_NULL_HANDLE;
    }   

    if (result == PMC_SUCCESS) 
    {
        switch(maint_sig_val)
        {
            case 0:
                *maint_sig = CORE_OTN_NO_MAINT_SIG;
                break;

            case 1:
                *maint_sig = CORE_OTN_ODUK_AIS;
                break;

            case 2:
                *maint_sig = CORE_OTN_ODUK_LCK;
                break;

            case 3:
                *maint_sig = CORE_OTN_ODUK_OCI;
                break;

            default:
                *maint_sig = LAST_CORE_OTN_MAINT_SIG;
                result = COREOTN_ERR_INVALID_PARAMETERS;
                break;
        }
    }
    
    PMC_RETURN(result);
} /* odu_struct_tx_tcm_maint_sig_get */


/*******************************************************************************
* odu_struct_chnl_sd_sf_ssf_prop_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Sets defect consequent action controls for an ODU channel.
*
* INPUTS:
*   coreotn_handle          - pointer to COREOTN handle instance
*   odu_struct              - pointer to ODU_STRUCT to be operated on
*   chnl_defect_action_msk  - bit mask\n
*                             Bit 0 -- sf_sd_prop: controls the propagation of
*                                      signal fail and signal degrade status.\n
*                                      0 -- Signal fail and signal degrade
*                                           status is propagated transparently\n
*                                      1 -- Signal fail and signal degrade
*                                           status is terminated
*                             Bit 1 -- ssf_prop: controls the propagation of
*                                      signal fail and signal degrade status.\n
*                                      0 -- Signal fail and signal degrade
*                                           status is propagated transparently\n
*                                      1 -- Signal fail and signal degrade
*                                           status is terminated
*   target                  - configuration target of odu struct data.
*                             see odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_chnl_sd_sf_ssf_prop_set(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    UINT8 chnl_defect_action_msk,
                                                    odu_struct_target_t target)
{
    UINT32 sd_sf_prop = 0;
    UINT32 ssf_prop = 0;
    odu_tfrm_handle_t *local_odu_tfrm_ptr = NULL;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    /* argument checking */
    if (target > LAST_CONFIG_TARGET || chnl_defect_action_msk > 0x3)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (chnl_defect_action_msk & 0x1)
    {
        odu_struct->mem_ptr->sf_sd_prop = 1;
    }
    else {
        odu_struct->mem_ptr->sf_sd_prop = 0;
    }

    if ((chnl_defect_action_msk >> 1) & 0x1)
    {
        odu_struct->mem_ptr->ssf_prop = 1;
    }
    else {
        odu_struct->mem_ptr->ssf_prop = 0;
    }

    odu_struct->mem_ptr->sf_sd_prop = sd_sf_prop;
    odu_struct->mem_ptr->ssf_prop = ssf_prop;

    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        sd_sf_prop = (chnl_defect_action_msk & 0x1)? 1:0;
        ssf_prop = ((chnl_defect_action_msk >> 1) & 0x1)? 1:0;

        local_odu_tfrm_ptr = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

        if (NULL != local_odu_tfrm_ptr)
        {
            result = odu_tfrm_sf_sd_proc_cfg(local_odu_tfrm_ptr, odu_struct->switch_data.oduk_data.channel,
                                              sd_sf_prop);
            
            if (PMC_SUCCESS == result)
            {
                result = odu_tfrm_ssf_proc_cfg(local_odu_tfrm_ptr,  
                                               odu_struct->switch_data.oduk_data.channel,
                                               ssf_prop);
            }                                              
        }
        else {
            result = COREOTN_ERR_NULL_HANDLE;
        } 
    }

    PMC_RETURN(result);
} /* odu_struct_chnl_sd_sf_ssf_set */

/*******************************************************************************
* odu_struct_chnl_lof_lom_act_dis_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Sets dLOFLOM consequent action control for the ODU channel.
*
* INPUTS:
*   *coreotn_handle - pointer to COREOTN handle
*   *odu_struct     - pointer to ODU_STRUCT to be operated on
*   lof_lom_act_dis - controls enabling/disabling of dLOFLOM consequenct action\n
*                     0 -- dLOFLOM defect causes nominal rate ODUk-AIS to be
*                         inserted in the channel.\n
*                     1 -- dLOFLOM defect does not cause ODUk-AIS to be inserted
*                         in the channel.
*   target          - configuration target of odu struct data.
*                     See odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_chnl_lof_lom_act_dis_set(coreotn_handle_t *coreotn_handle,
                                                     odu_struct_t *odu_struct,
                                                     BOOL8 lof_lom_act_dis,
                                                     odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (lof_lom_act_dis > 1)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }



    if(lof_lom_act_dis == 0 &&
       (odu_struct->mem_ptr->odu_level == ODU_STRUCT_LEVEL_3B_ODU ||
       odu_struct->mem_ptr->odu_level == ODU_STRUCT_LEVEL_4_ODU ||
       odu_struct->mem_ptr->odu_level == ODU_STRUCT_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC))
    {
        PMC_RETURN(COREOTN_ERR_LOFLOM_ACT_DIS);
    }

    odu_struct->mem_ptr->lof_lom_act_dis = lof_lom_act_dis;

    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);




        if (NULL != local_odu_rfrm_handle)
        {
            /* lof_lom_act_dis */
            result = odu_rfrm_chnl_ins_ais_cfg(local_odu_rfrm_handle,
                                               odu_struct->switch_data.oduk_data.channel,
                                               lof_lom_act_dis);
        }
        else {
            result = COREOTN_ERR_NULL_HANDLE;
        }
    }

    PMC_RETURN(result);
} /* odu_struct_chnl_lof_lom_act_dis_set */

/*******************************************************************************
* odu_struct_chnl_lof_lom_act_dis_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Gets dLOFLOM consequent action control for the ODU channel.
*
* INPUTS:
*   *coreotn_handle  - pointer to COREOTN handle
*   *odu_struct      - pointer to ODU_STRUCT to be operated on
*   ext_mode         - configuration target of odu struct data.
*                      See odu_struct_target_t
*
*
* OUTPUTS:
*   *lof_lom_act_dis - controls enabling/disabling of dLOFLOM consequenct action\n
*                      0 -- dLOFLOM defect causes nominal rate ODUk-AIS to be
*                           inserted in the channel.\n
*                      1 -- dLOFLOM defect does not cause ODUk-AIS to be inserted
*                           in the channel.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_chnl_lof_lom_act_dis_get(coreotn_handle_t *coreotn_handle,
                                                     odu_struct_t *odu_struct,
                                                     odu_struct_ext_mode_t ext_mode,
                                                     BOOL8 *lof_lom_act_dis)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);

        if (NULL != local_odu_rfrm_handle)
        {
            /* lof_lom_act_dis */
            result = odu_rfrm_chnl_dloflom_act_dis_get(local_odu_rfrm_handle,
                                                       odu_struct->switch_data.oduk_data.channel, 
                                                       lof_lom_act_dis);
        }
        else {
            result= COREOTN_ERR_NULL_HANDLE;
        } 
    }
    else {
        *lof_lom_act_dis = odu_struct->mem_ptr->lof_lom_act_dis;
    }
    
    PMC_RETURN(result);                                                           
} /* odu_struct_chnl_lof_lom_act_dis_get */

/*******************************************************************************
* odu_struct_tx_pm_tcmi_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   provisions or unprovisions PM or TCMi layer in source direction on the ODU  
*   channel represented by an ODU struct. 
*   To provision a layer, 'enbl' needs to be set to 1.
*   To unprovision a layer, 'enbl' needs to be set to 0 and 'source_mode ' needs 
*   to be set to ODU_STRUCT_PM_TCM_TT_TRANSPARENT. 
*
*   For direct configuration, this API is used to provision PM or TCMi layer
*   of the ODU channel.
*
*   For protect alternate configuration, this API adds TCM resource data into
*   the ODU struct
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi instance. See odu_struct_odu_frm_pm_tcm_id_t.
*                         When provisioning a PM layer, this will be the ultimate
*                         parent in the transmit layer hierarchy.
*   pool_assign_mode    - mode to assign TCM POOL ID resources. \n
*                         0 -- SW Driver assigns Pool ID for specified
*                              PM NIM/TCMi \n
*                         1 -- pool_id specifies the Pool ID used for specified
*                              PM NIM/TCMi.
*   tcm_pool_id         - pool id of the TCM resource. Only valid if pool_assign_mode == 1.
*                         Not used if pm_tcm_num is set to DIGI_ODU_FRM_MESSAGE_TYPE_PM.
*                         Set to 0 when pool_assign_mode == 0.
*   parent_tcm_num      - PM/TCMi instance of the parent of this current layer.
*                         This argument is used to control consequential action
*                         generation downstream. \n
*                         Set to DIGI_ODU_FRM_MESSAGE_TYPE_DCI if consequential
*                         action generation among layers is not considered or
*                         if it is the first layer or PM layer to be provisioned.
*   source_mode         - PM/TCMi operating mode function of source layer.
*                         See odu_struct_pm_tcm_mode_t.
*                         Valid modes are:
*                           ODU_STRUCT_PM_TCM_TT_OPERATIONAL,
*                           ODU_STRUCT_PM_TCM_TT_TRANSPARENT,
*   target              - configuration target of odu struct data.
*                         see odu_struct_target_t
*
* OUTPUTS:
*   *assigned_pool_id   - assigned pool id returned to the user when this function
*                         is called with 'pool_assign_mode' set to 0.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tx_pm_tcmi_set(coreotn_handle_t *coreotn_handle,
                                           odu_struct_t *odu_struct,
                                           odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                           UINT8 pool_assign_mode,
                                           UINT8 tcm_pool_id, 
                                           odu_struct_odu_frm_pm_tcm_id_t parent_tcm_num,
                                           odu_struct_pm_tcm_mode_t source_mode,
                                           odu_struct_target_t target,
                                           UINT16 *assigned_pool_id)
{

    PMC_ERROR ret_val = PMC_SUCCESS;
    PMC_ERROR pid_result = PMC_SUCCESS;
    BOOL8 enbl;
    UINT16 dummy_pid;
    UINT16 returned_pid;
    UINT32 i;
    UINT32 tcm_idx = 0;
    BOOL tcm_found;

    odu_tfrm_handle_t *odu_tfrm_hndl = NULL;
    odu_tfrm_layer_struct_t tcm_layers[ODU_SOURCE_MAX_PM_TCM_LAYERS];
    UINT32 num_layers;
    odu_tfrm_pm_tcm_id_t tcm_inst;
    UINT32 cnt;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);


    if(source_mode == ODU_STRUCT_PM_TCM_TT_TRANSPARENT)
    {
        enbl = 0;
    }
    else
    {
        enbl = 1;
    }

    /* argument checking */
    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES 
        || pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_FLOATING_TCM
        || pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM_NIM
        || pool_assign_mode > 1  
        || parent_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES 
        || parent_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_FLOATING_TCM
        || parent_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM_NIM
        || source_mode == ODU_STRUCT_PM_TCM_NON_INTRUSIVE_MONITOR
        || source_mode == ODU_STRUCT_PM_TCM_TT_MONITOR
        || source_mode == ODU_STRUCT_PM_TCM_TT_ERASE
        || enbl > 1 || target > LAST_CONFIG_TARGET
        || (enbl == 0 && source_mode != ODU_STRUCT_PM_TCM_TT_TRANSPARENT))
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (pool_assign_mode == 1)
    {
        /* SW doesn't need to return a pool id
        ** return the same pool id that was supplied by the user
        */
        returned_pid = tcm_pool_id;
        *assigned_pool_id = tcm_pool_id;
    }
    
    /* retreive correct framer handle for accessing ODU_RFRM and TFRM */
    odu_tfrm_hndl = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
    
    if (NULL == odu_tfrm_hndl)
    {
        PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
    }
    else {

        /* need to throw an error if the TCM/PM is already provisioned don't 
        ** want pool_id_assign to mess up the context** 
        */ 
        if(enbl == 1)
        {
            if (pm_tcm_num != ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
            {
                /* check to see if this resource has been provisioned already */
                pid_result = odu_tfrm_pool_id_return(odu_tfrm_hndl, odu_struct->switch_data.oduk_data.channel, (odu_tfrm_pm_tcm_id_t)pm_tcm_num, &dummy_pid);

                /* if a pool id was found successfully, then this resource has already been provisioned*/
                if(pid_result == PMC_SUCCESS)
                {
                    PMC_RETURN(ODU_TFRM_ERR_TCM_PROVISIONED);
                }

            }
            else
            {
                if(odu_tfrm_pm_prov_state_get(odu_tfrm_hndl, odu_struct->switch_data.oduk_data.channel) == TRUE)
                {
                    PMC_RETURN(ODU_TFRM_ERR_PM_LAYER_PROVISIONED);
                }
            }
        }
        else
        {
              if (pm_tcm_num != ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
              {
                  /* check to see if this resource has been provisioned already */
                  pid_result = odu_tfrm_pool_id_return(odu_tfrm_hndl, odu_struct->switch_data.oduk_data.channel, (odu_tfrm_pm_tcm_id_t)pm_tcm_num, &dummy_pid);

                  /* pool id must be found successfully (want to deprov an existing resource)*/
                  if(pid_result != PMC_SUCCESS)
                  {
                      PMC_RETURN(ODU_TFRM_ERR_TCM_NOT_PROVISIONED);
                  }

              }
              else
              {
                  if(odu_tfrm_pm_prov_state_get(odu_tfrm_hndl, odu_struct->switch_data.oduk_data.channel) == FALSE)
                  {
                      PMC_RETURN(ODU_TFRM_ERR_PM_NOT_PROVISIONED);
                  }

              }
        }

        if (pm_tcm_num != ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
        {
            /* SW internally assigns a pool id and returns it back to the user */
            /* assign pool id */
            if(enbl == 1)
            {
                ret_val = odu_tfrm_pool_id_assign(odu_tfrm_hndl, odu_struct->switch_data.oduk_data.channel,
                                                  (odu_tfrm_pm_tcm_id_t)pm_tcm_num,
                                                  pool_assign_mode, tcm_pool_id,
                                                  &returned_pid);         
                if (PMC_SUCCESS == ret_val)
                {
                    *assigned_pool_id = returned_pid;
                }
            }
            else {
                *assigned_pool_id = COREOTN_UNUSED_TCM_POOL_ID;
            }
                 
            /* for STG4 and SYSOTN_TCM_MON_LINE_TO_FABRIC stage, do not update 
            ** ODU struct as APS not applicable at these stages
            */
            /* In either mode, 'tcm_pool_id' is the current pool id being used for
            ** provisioning a layer
            */
            if (odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_4_ODU 
                && odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC)
            {

                if (PMC_SUCCESS == ret_val)
                {
                    /* find an empty TCM pool resource or find if the companion RFRM has 
                    ** the same TCM layer provisioned in the odu_struct
                    */

                    tcm_found = FALSE;

                    if(enbl == 1)
                    {
                    
                        for (i = 0; i < 8; i++)
                        {
                            if(odu_struct->mem_ptr->tcm_pool_rsc[i].tcm_instance == pm_tcm_num &&
                               tcm_found == FALSE)
                            {
                                tcm_found = TRUE;
                                tcm_idx = i;
                            }
                        }

                        for (i = 0; i < 8; i++)
                        {
                            if(odu_struct->mem_ptr->tcm_pool_rsc[i].tcm_instance == LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES &&
                               tcm_found == FALSE)
                            {
                                tcm_found = TRUE;
                                tcm_idx = i;
                            }
                        }


                    }
                    else
                    {
                        for (i = 0; i < 8; i++)
                        {
                            if(odu_struct->mem_ptr->tcm_pool_rsc[i].tcm_instance == pm_tcm_num &&
                               tcm_found == FALSE)
                            {
                                tcm_found = TRUE;
                                tcm_idx = i;
                            }

                        }
                    }



                    if(tcm_found == TRUE)
                    {
                        if (enbl == 1)
                        {
                              PMC_LOG_TRACE("tcm_pool_rsc[%u] taken\n", tcm_idx);
                              odu_struct->mem_ptr->tcm_pool_rsc[tcm_idx].tcm_instance = pm_tcm_num;
                        }
                        else {
                              if (odu_struct->mem_ptr->tcm_pool_rsc[tcm_idx].tcm_pool_id_rx == ODU_STRUCT_UNASSIGNED_POOL_ID)
                              { 
                                  odu_struct_tcm_pool_rsc_clean(odu_struct, &(odu_struct->mem_ptr->tcm_pool_rsc[tcm_idx]));
                              }
                              else { 
                                  odu_struct->mem_ptr->tcm_pool_rsc[tcm_idx].tcm_instance = pm_tcm_num;
                              }
                        }

                        odu_struct->mem_ptr->tcm_pool_rsc[tcm_idx].tcm_pool_id_tx = *assigned_pool_id;
                        odu_struct->mem_ptr->tcm_pool_rsc[tcm_idx].mode_tx = (UINT8)source_mode;
                                
                        if (enbl)
                        {
                            odu_struct->mem_ptr->num_layers_tx++;
                        }
                        else {
                            odu_struct->mem_ptr->num_layers_tx--;
                        }
                    }
                    else
                    {
                        PMC_RETURN(COREOTN_ERR_ODU_STRUCT_TCM_RSC_FULL);
                    }

                }
            }
        }
        else {
            /* PM layer does not require a pool id */
            *assigned_pool_id = COREOTN_UNUSED_TCM_POOL_ID;
        }
    
        if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
        {
            if(source_mode == ODU_STRUCT_PM_TCM_TT_TRANSPARENT)
            {
                /* To deprov the TFRM, must call with ODU_STRUCT_PM_TCM_START */
                source_mode = ODU_STRUCT_PM_TCM_START;
            }
           
            if (PMC_SUCCESS == ret_val)
            {
                ret_val = odu_tfrm_tcmi_cfg(odu_tfrm_hndl,
                                            odu_struct->switch_data.oduk_data.channel,
                                            (odu_tfrm_pm_tcm_id_t)pm_tcm_num,
                                            (odu_tfrm_pm_tcm_id_t)parent_tcm_num,
                                            (odu_tfrm_pm_tcm_mode_t)source_mode,
                                            enbl);
            }                                                                                      
        }
    }

    
    /* update layering heirarch in the ODU struct*/
    if (odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_4_ODU 
        && odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC)
    {
        if(PMC_SUCCESS == ret_val)
        {
            ret_val = odu_tfrm_layer_hierarchy_get(odu_tfrm_hndl,
                                                   odu_struct->switch_data.oduk_data.channel,
                                                   tcm_layers,
                                                   &num_layers);
            for(i=0;i<num_layers;i++)
            {
                tcm_inst = tcm_layers[i].pm_tcm_num;
              
                if((odu_struct_odu_frm_pm_tcm_id_t)tcm_inst != ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
                {

                    for (cnt = 0; cnt < (ODU_SINK_MAX_PM_TCM_LAYERS-1); cnt++) 
                    { 
                        if(odu_struct->mem_ptr->tcm_pool_rsc[cnt].tcm_instance == (odu_struct_odu_frm_pm_tcm_id_t)tcm_inst) 
                        { 
                            odu_struct->mem_ptr->tcm_pool_rsc[cnt].tx_layer_num = i; 
                            break; 
                        } 
                    }
                    
                }
            }
        }
    }

    PMC_RETURN(ret_val);

} /* odu_struct_tx_pm_tcmi_set */

/*******************************************************************************
* odu_struct_rx_pm_tcmi_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   provisions or unprovisions PM or TCMi layer in sink direction on the ODU  
*   channel represented by an ODU struct. 
*   To provision a layer, 'enbl' needs to be set to 1.
*   To unprovision a layer, 'enbl' needs to be set to 0 and 'sink_mode' needs
*   to be set to ODU_STRUCT_PM_TCM_TT_TRANSPARENT. 
*
*   For direct configuration, this API is used to provision PM or TCMi layer
*   of the ODU channel.
*
*   For protect alternate configuration, this API adds TCM resource data into
*   the ODU struct
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi instance. See odu_struct_odu_frm_pm_tcm_id_t.
*   pool_assign_mode    - mode to assign TCM POOL ID resources. \n
*                         0 -- SW Driver assigns Pool ID for specified
*                              PM NIM/TCMi \n
*                         1 -- pool_id specifies the Pool ID used for specified
*                              PM NIM/TCMi.
*   tcm_pool_id         - pool id of the TCM resource. Only valid if pool_assign_mode == 1.
*                         Not used if pm_tcm_num is set to DIGI_ODU_FRM_MESSAGE_TYPE_PM.
*                         Set to 0 when pool_assign_mode == 0.
*   float_tcmi_src      - TCMi instance if pm_tcm_num is set to
*                         DIGI_ODU_FRM_MESSAGE_TYPE_FLOATING_TCM
*   parent_tcm_num      - PM/TCMi instance of the parent of this current layer.
*                         This argument is used to control consequential action
*                         generation downstream. \n
*                         Set to DIGI_ODU_FRM_MESSAGE_TYPE_DCI if consequential
*                         action generation among layers is not considered or
*                         if it is the first layer to be provisioned.
*   sink_mode           - PM/TCMi operating mode function of sink layer.
*                         See odu_struct_pm_tcm_mode_t
*   target              - configuration target of odu struct data.
*                         see odu_struct_target_t
*
* OUTPUTS:
*   *assigned_pool_id   - assigned pool id returned to the user when this function
*                         is called with 'pool_assign_mode' set to 0.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_pm_tcmi_set(coreotn_handle_t *coreotn_handle,
                                           odu_struct_t *odu_struct,
                                           odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                           UINT8 pool_assign_mode,
                                           UINT8 tcm_pool_id, UINT8 float_tcmi_src,
                                           odu_struct_odu_frm_pm_tcm_id_t parent_tcm_num,
                                           odu_struct_pm_tcm_mode_t sink_mode,
                                           odu_struct_target_t target,
                                           UINT16 *assigned_pool_id)
{

    PMC_ERROR ret_val = PMC_SUCCESS;
    BOOL8 enbl = 0;
    
    odu_rfrm_handle_t *odu_rfrm_hndl = NULL;
    UINT16 returned_pool_id = 0;
    UINT32 i = 0;
    UINT32 tcm_pid_slot = 0;
    UINT32 tcm_idx = 0;
    BOOL tcm_found = 0;

    odu_rfrm_layer_struct_t tcm_layers[ODU_SINK_MAX_PM_TCM_LAYERS] = { { 0 } };
    UINT32 num_layers = 0;
    odu_rfrm_pm_tcm_id_t tcm_inst = ODU_RFRM_MESSAGE_DONT_CARE;
    UINT32 cnt = 0;

    PMC_ENTRY();
    
    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    PMC_MEMSET(&tcm_layers[0],0,sizeof(odu_rfrm_layer_struct_t)* (UINT32) ODU_SINK_MAX_PM_TCM_LAYERS);

    /* argument checking */
    if (pm_tcm_num > ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM_NIM ||
        pool_assign_mode > 1 || float_tcmi_src > 6 ||
        parent_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES ||
        sink_mode > LAST_ODU_STRUCT_PM_TCM_MODE ||
        target > LAST_CONFIG_TARGET)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }


    if(sink_mode == ODU_STRUCT_PM_TCM_TT_TRANSPARENT)
    {
        enbl = 0;
    }
    else
    {
        enbl = 1;
    }
        
    /* retreive correct framer handle for accessing ODU_RFRM */
    odu_rfrm_hndl = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
    
    if (NULL == odu_rfrm_hndl)
    {
        PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
    }


    ret_val = odu_rfrm_validate_pm_tcm_params(odu_rfrm_hndl,
                                              odu_struct->switch_data.oduk_data.channel,
                                              (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                              (odu_rfrm_pm_tcm_id_t)parent_tcm_num,
                                              (odu_rfrm_pm_tcm_mode_t)sink_mode,
                                              (BOOL)enbl,
                                              tcm_pool_id,
                                              pool_assign_mode);


    if (PMC_SUCCESS == ret_val)
    {
        if (pm_tcm_num != ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM && enbl == 1)
        {
            /* SW internally assigns a pool id */
            ret_val = odu_rfrm_pool_id_assign(odu_rfrm_hndl, odu_struct->switch_data.oduk_data.channel,
                                              (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                              pool_assign_mode, tcm_pool_id,
                                              &returned_pool_id);   
            
            if (PMC_SUCCESS == ret_val)
            {
                *assigned_pool_id = returned_pool_id; 
            }                                               
        }
        else {
            /* PM layer does not require a pool id */
            *assigned_pool_id = COREOTN_UNUSED_TCM_POOL_ID;
        }
    }


    /* update ODU struct context when adding or removing a TCM resource 
    ** 1. when adding a new resource, either find a new resource or if the same
    **    TCM instance has been assigned from TX side, use the same structure to
    **    store RX pool data. 
    ** 2. when removing an already existing resource, find the structure representing
    **    the TCM instance and update the pool id and mode. For updating pm_tcm_num,
    **    first check if TX side is also non operational. Only then update to 
    **    release the structure.
    */


    if(odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_4_ODU &&
       odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC)
    {
        if (pm_tcm_num != ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
        {
            /* In either mode, 'tcm_pool_id' is the current pool id being used for
            ** provisioning a layer
            */
            if (PMC_SUCCESS == ret_val)
            {
                tcm_found = FALSE;

                if(enbl == 1)
                {
            
                    for (i = 0; i < 8; i++)
                    {
                        if(odu_struct->mem_ptr->tcm_pool_rsc[i].tcm_instance == pm_tcm_num &&
                           tcm_found == FALSE)
                        {
                            tcm_found = TRUE;
                            tcm_idx = i;
                            break;
                        }

                    }

                    for (i = 0; i < 8 && FALSE == tcm_found; i++)
                    {
                        if(odu_struct->mem_ptr->tcm_pool_rsc[i].tcm_instance == LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES)
                        {
                            tcm_found = TRUE;
                            tcm_idx = i;
                            break;
                        }
                    }
                }
                else
                {
                    for (i = 0; i < 8; i++)
                    {
                        if(odu_struct->mem_ptr->tcm_pool_rsc[i].tcm_instance == pm_tcm_num &&
                           tcm_found == FALSE)
                        {
                            tcm_found = TRUE;
                            tcm_idx = i;
                            break;
                        }

                    }
                }

                if(tcm_found == TRUE)
                {
                    if (enbl == 1)
                    {
                          PMC_LOG_TRACE("tcm_pool_rsc[%u] taken\n", tcm_idx);
                          odu_struct->mem_ptr->tcm_pool_rsc[tcm_idx].tcm_instance = pm_tcm_num;
                    }
                    else {
                          if (odu_struct->mem_ptr->tcm_pool_rsc[tcm_idx].tcm_pool_id_tx == ODU_STRUCT_UNASSIGNED_POOL_ID)
                          { 
                              odu_struct_tcm_pool_rsc_clean(odu_struct, &(odu_struct->mem_ptr->tcm_pool_rsc[tcm_idx]));
                          }
                          else { 
                              odu_struct->mem_ptr->tcm_pool_rsc[tcm_idx].tcm_instance = pm_tcm_num;
                          }
                    }

                    odu_struct->mem_ptr->tcm_pool_rsc[tcm_idx].tcm_pool_id_rx = *assigned_pool_id;
                    odu_struct->mem_ptr->tcm_pool_rsc[tcm_idx].mode_rx = (UINT8)sink_mode;
                            
                    if (enbl)
                    {
                        odu_struct->mem_ptr->num_layers_rx++;
                    }
                    else {
                        odu_struct->mem_ptr->num_layers_rx--;
                    }
 
                    /* update the pool_id_array for PMON collection */      
                    tcm_pid_slot = tcm_idx;
                }
                else
                {
                    PMC_RETURN(COREOTN_ERR_ODU_STRUCT_TCM_RSC_FULL);
                }


            }
            
            
            if (odu_struct->mem_ptr->num_layers_rx > 8)
            {
                PMC_RETURN(COREOTN_ERR_ODU_STRUCT_TCM_RSC_FULL);
            }

        }
        else {
            /* if PM layer is to be provisioned then update the PM context in
            ** odu_struct 
            */
            odu_struct->mem_ptr->pm_mode = (UINT8)sink_mode;
            
        }
    }

    if (pm_tcm_num != ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
    {
        if (ret_val == PMC_SUCCESS)
        {
            /* update pool_id_array for PMON collection */
            odu_struct->pool_id_array[(UINT8)(pm_tcm_num-1)] = *assigned_pool_id; 
            
            /* update tcm_pid_valid bitarray for PMON collecting */
            ret_val = coreotn_tcm_pid_valid_set(coreotn_handle,
                                                odu_struct->mem_ptr->odu_level,
                                                odu_struct->switch_data.oduk_data.channel,
                                                (UINT32)(pm_tcm_num-1),
                                                enbl);
        } 
    }
   
    /* the following is not affected by odu_3b_struct */
    if(ret_val == PMC_SUCCESS)
    {
        if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
        {
           
            ret_val = odu_rfrm_pm_tcmi_cfg(odu_rfrm_hndl,
                                           odu_struct->switch_data.oduk_data.channel,
                                           (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                           float_tcmi_src,
                                           (odu_rfrm_pm_tcm_id_t)parent_tcm_num,
                                           (odu_rfrm_pm_tcm_mode_t)sink_mode,
                                           (BOOL)enbl);              
        }
    }

    /* update layering heirarch in the ODU struct*/
    if (odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_4_ODU 
        && odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC)
    {
        if(PMC_SUCCESS == ret_val)
        {
            ret_val = odu_rfrm_layer_hierarchy_get(odu_rfrm_hndl,
                                                   odu_struct->switch_data.oduk_data.channel,
                                                   &tcm_layers,
                                                   &num_layers);
            for(i=0;i<num_layers;i++)
            {
                tcm_inst = tcm_layers[i].pm_tcm_num;

                if((odu_struct_odu_frm_pm_tcm_id_t)tcm_inst != ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
                {

                    for (cnt = 0; cnt < (ODU_SINK_MAX_PM_TCM_LAYERS-1); cnt++) 
                    { 
                        if(odu_struct->mem_ptr->tcm_pool_rsc[cnt].tcm_instance == (odu_struct_odu_frm_pm_tcm_id_t)tcm_inst) 
                        { 
                            odu_struct->mem_ptr->tcm_pool_rsc[cnt].rx_layer_num = i; 
                            break; 
                        } 
                    }                   
                }
            }
        }
    }

    PMC_RETURN(ret_val);

} /* odu_struct_rx_pm_tcmi_set */

/*******************************************************************************
* odu_struct_rx_pm_tcmi_activate
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This function activates PM or TCMi layer in the ODU channel represented by
*   the ODU struct. Activation of a newly added layer requires 25ms delay after
*   the new layer has been provisioned in order for the stale defects to be 
*   cleared.
*
*   Prior to calling this API, odu_struct_pm_tcmi_set() must be called to
*   set up a TCM or PM layer properly and have a valid 'tcm_pool_id' to be used
*   for all other configurations (i.e. overhead insertion) related to this layer.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_pm_tcmi_activate(coreotn_handle_t *coreotn_handle,
                                                odu_struct_t *odu_struct)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    odu_rfrm_handle_t *odu_rfrm_hndl = NULL;
    odu_rfrm_layer_struct_t odu_rfrm_layers[ODU_SINK_MAX_PM_TCM_LAYERS];
    UINT32 num_layers;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    odu_rfrm_hndl = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
   
    if (NULL != odu_rfrm_hndl)
    {
        /* get the new rfrm layer_hierachy */
        ret_val = odu_rfrm_layer_hierarchy_get(odu_rfrm_hndl,
                                               odu_struct->switch_data.oduk_data.channel,
                                               &odu_rfrm_layers,
                                               &num_layers);
    
        if (PMC_SUCCESS == ret_val)
        {
            /* connect to the new parent */
            ret_val = odu_rfrm_parent_pid_parent_is_dci_update(odu_rfrm_hndl,
                                                               odu_struct->switch_data.oduk_data.channel,
                                                               odu_rfrm_layers,
                                                               num_layers);
        }     
        
        /*call function to set last_layer */
        if (PMC_SUCCESS == ret_val)
        {
            ret_val = odu_rfrm_last_layer_last_is_dci_update(odu_rfrm_hndl,
                                                             odu_struct->switch_data.oduk_data.channel,
                                                             odu_rfrm_layers,
                                                             num_layers);
        }                                                          
    }
    else {
        ret_val = COREOTN_ERR_NULL_HANDLE;
    }


    PMC_RETURN(ret_val);

} /* odu_struct_rx_pm_tcmi_activate */


/*******************************************************************************
* odu_struct_rx_tcmi_mode_update
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Changes the mode of an already operational TCM.  Support hitless transition
*   between the following modes without having to deprovision the TCM:
*      ODU_STRUCT_PM_TCM_TT_OPERATIONAL -> ODU_STRUCT_PM_TCM_TT_MONITOR
*      ODU_STRUCT_PM_TCM_TT_MONITOR -> ODU_STRUCT_PM_TCM_TT_OPERATIONAL
*      ODU_STRUCT_PM_TCM_TT_OPERATIONAL -> ODU_STRUCT_PM_TCM_ NON_INTRUSIVE_MONITOR
*      ODU_STRUCT_PM_TCM_ NON_INTRUSIVE_MONITOR-> ODU_STRUCT_PM_TCM_TT_OPERATIONAL
*      ODU_STRUCT_PM_TCM_TT_MONITOR -> ODU_STRUCT_PM_TCM_ NON_INTRUSIVE_MONITOR
*      ODU_STRUCT_PM_TCM_ NON_INTRUSIVE_MONITOR-> ODU_STRUCT_PM_TCM_TT_MONITOR    
*      ODU_STRUCT_PM_TCM_TT_OPERATIONAL_TCMOH_NOT_ZEROED  -> ODU_STRUCT_PM_TCM_TT_MONITOR
*      ODU_STRUCT_PM_TCM_TT_MONITOR -> ODU_STRUCT_PM_TCM_TT_OPERATIONAL_TCMOH_NOT_ZEROED
*      ODU_STRUCT_PM_TCM_TT_OPERATIONAL_TCMOH_NOT_ZEROED -> ODU_STRUCT_PM_TCM_ NON_INTRUSIVE_MONITOR
*      ODU_STRUCT_PM_TCM_ NON_INTRUSIVE_MONITOR-> ODU_STRUCT_PM_TCM_TT_OPERATIONAL_TCMOH_NOT_ZEROED
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi instance. See odu_struct_odu_frm_pm_tcm_id_t.
*   sink_mode           - PM/TCMi operating mode function of sink layer.
*                         See odu_struct_pm_tcm_mode_t
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_tcmi_mode_update(coreotn_handle_t *coreotn_handle,
                                                odu_struct_t *odu_struct,
                                                odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                odu_struct_pm_tcm_mode_t sink_mode)
{

    PMC_ERROR ret_val = PMC_SUCCESS; 
    odu_rfrm_handle_t *odu_rfrm_hndl = NULL;
    UINT32 returned_pool_id = 0;
    BOOL tcm_found;
    UINT32 tcm_idx = 0;
    UINT32 i;

    PMC_ENTRY();
    
    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    /* argument checking
    Only valid to call for TCM or floting TCM resource. Not valid
    to transition to ODU_STRUCT_PM_TCM_TT_TRANSPARENT*/
    if (pm_tcm_num > ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM_NIM
        || pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM
        || pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM_NIM
        || pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_DCI
        || sink_mode > LAST_ODU_STRUCT_PM_TCM_MODE
        || sink_mode == ODU_STRUCT_PM_TCM_START
        || sink_mode == ODU_STRUCT_PM_TCM_TT_TRANSPARENT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
        
    /* retreive correct framer handle for accessing ODU_RFRM */
    odu_rfrm_hndl = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
    
    if (NULL == odu_rfrm_hndl)
    {
        PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
    }

    if (PMC_SUCCESS == ret_val)
    {
        /*get pool id.  Will return an error if the TCM instance is not
        provisioned*/
        ret_val = odu_rfrm_pool_id_return(odu_rfrm_hndl, 
                                          odu_struct->switch_data.oduk_data.channel, 
                                          (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                          &returned_pool_id);
    }
    
    if(PMC_SUCCESS == ret_val)
    {

        tcm_found = FALSE;
            
        for (i = 0; i < 8; i++)
        {
              if(odu_struct->mem_ptr->tcm_pool_rsc[i].tcm_instance == pm_tcm_num &&
                 tcm_found == FALSE)
              {
                  tcm_found = TRUE;
                  tcm_idx = i;
              }
        }

        odu_struct->mem_ptr->tcm_pool_rsc[tcm_idx].mode_rx = (UINT8)sink_mode;
    }
                             
    if(ret_val == PMC_SUCCESS)
    {
            ret_val = odu_rfrm_tcmi_mode_update(odu_rfrm_hndl,
                                                   odu_struct->switch_data.oduk_data.channel,
                                                   (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                                   (odu_rfrm_pm_tcm_mode_t)sink_mode);
    }

    PMC_RETURN(ret_val);

} /* odu_struct_rx_tcmi_mode_update */

/*******************************************************************************
* odu_struct_rx_pm_tcmi_parent_update
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This API reconnects the specified TCM layer to a new parent.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi instance. See odu_struct_odu_frm_pm_tcm_id_t.
*   new_parent_tcm_num  - PM or TCMi instance. See odu_struct_odu_frm_pm_tcm_id_t.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_pm_tcmi_parent_update(coreotn_handle_t *coreotn_handle,
                                                     odu_struct_t *odu_struct,
                                                     odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                     odu_struct_odu_frm_pm_tcm_id_t new_parent_tcm_num)
{
    PMC_ERROR ret_val = PMC_SUCCESS; 
    odu_rfrm_handle_t *odu_rfrm_hndl = NULL;
    UINT32 num_layers = 0;
    UINT32 i;
    odu_rfrm_layer_struct_t tcm_layers[ODU_SINK_MAX_PM_TCM_LAYERS] = { { 0 } };
    odu_rfrm_pm_tcm_mode_t parent_tcm_mode = ODU_RFRM_PM_TCM_LAST;

    PMC_ENTRY();
    
    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    /* argument checking */
    if (pm_tcm_num > ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM_NIM
        || pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM
        || new_parent_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (PMC_SUCCESS == ret_val)
    {
        /* retreive correct framer handle for accessing ODU_RFRM */
        odu_rfrm_hndl = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
        
        if (NULL == odu_rfrm_hndl)
        {
            PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
        }
    }
    
    if (PMC_SUCCESS == ret_val)
    {
        ret_val = odu_rfrm_layer_hierarchy_get(odu_rfrm_hndl,
                                               odu_struct->switch_data.oduk_data.channel,
                                               &tcm_layers,
                                               &num_layers);
    }

    if (PMC_SUCCESS == ret_val)
    {
        /* check the mode of new parent layer */
        for (i = 0; i < num_layers; i++)
        {
            if (tcm_layers[i].pm_tcm_num == (odu_rfrm_pm_tcm_id_t)new_parent_tcm_num)
            {
                parent_tcm_mode = tcm_layers[i].tcm_mode;
                break;
            }
        }
    }

    if (parent_tcm_mode == (odu_rfrm_pm_tcm_mode_t)ODU_STRUCT_PM_TCM_NON_INTRUSIVE_MONITOR)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (PMC_SUCCESS == ret_val)
    {
        ret_val = odu_rfrm_pm_tcmi_parent_update(odu_rfrm_hndl,
                                                 odu_struct->switch_data.oduk_data.channel,
                                                 (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                                 (odu_rfrm_pm_tcm_id_t)new_parent_tcm_num,
                                                 tcm_layers,
                                                 num_layers);
    }

    PMC_RETURN(ret_val);

} /* odu_struct_rx_pm_tcmi_parent_update */


/*
** ODU OH configurations
*/
/*******************************************************************************
* odu_struct_pm_tcm_res_src_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   configures the source of PM&TCM reserved field of an ODU_STRUCT or the
*   device.
*
*   When configured for 'Incoming PM&TCM value' the value transferred on
*   PM&TCM byte will be from highest to lowest priority: \n
*       - Data from the External Overhead Port (if enabled) \n
*       - Register forcing \n
*       - Data from incoming ODUk frame
*
*   If external overhead port is enabled to insert PM&TCM byte, the value 
*   inserted from register using this API will be overwritten and have no effect. 
*
* INPUTS:
*   *coreotn_handle         - pointer to COREOTN handle
*   *odu_struct             - pointer to ODU_STRUCT to be operated on
*   pm_tcm_res_src          - PM_TCM_RES source\n
*                             0 -- Incoming PM&TCM reseved value\n
*                             1 -- Force PM&TCM reseved value
*   pm_tcm_res_force_val    - For source == 1 (otherwise set to 0)\n
*                             The value to be inserted into PM&TCM field of
*                             PM or TCMi layer\n
*                              Valid range: 0 - 1
*   target                  - configuration target of odu struct data.
*                             see odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_pm_tcm_res_src_set(coreotn_handle_t *coreotn_handle,
                                               odu_struct_t *odu_struct,
                                               odu_struct_odu_oh_ins_src_t pm_tcm_res_src,
                                               UINT8 pm_tcm_res_force_val,
                                               odu_struct_target_t target)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    /* argument checking */
    if (pm_tcm_res_src > LAST_ODU_STRUCT_OH_INS_SRC || target > LAST_CONFIG_TARGET)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    /* update device */
    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

        if (NULL != local_odu_tfrm_handle)
        {
            /* call API */
            ret_val = odu_tfrm_pm_tcm_src_cfg(local_odu_tfrm_handle,
                                              odu_struct->switch_data.oduk_data.channel,
                                              (odu_tfrm_oh_ins_src_t)pm_tcm_res_src, 
                                              pm_tcm_res_force_val);
        }
        else {
            PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
        }
    }

    PMC_RETURN(ret_val);
} /* odu_struct_pm_tcm_res_src_set */

/*******************************************************************************
* odu_struct_pm_tcm_res_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retrieves PM&TCM reserved field from an ODU_STRUCT or directly from the device.
*
* INPUTS:
*   coreotn_handle      - pointer to COREOTN handle instance
*   odu_struct          - pointer to ODU_STRUCT to be operated on
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   *pm_tcm_res_val      - retrieved PM&PM&TCM reseved value.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_pm_tcm_res_get(coreotn_handle_t *coreotn_handle,
                                           odu_struct_t *odu_struct,
                                           odu_struct_ext_mode_t ext_mode,
                                           UINT8 *pm_tcm_res_val)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_ptr = NULL;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_ODU_STRUCT)
    {
        *pm_tcm_res_val = odu_struct->mem_ptr->pm_tcm_res_force_val;

    }
    else {
        /* find the correct framer */
        local_odu_tfrm_ptr = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

        if (NULL != local_odu_tfrm_ptr)
        {
            result = odu_tfrm_pm_tcm_res_get(local_odu_tfrm_ptr,
                                             odu_struct->switch_data.oduk_data.channel,
                                             pm_tcm_res_val);
        }
        else {
            PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
        }
    }

    PMC_RETURN(result);
} /* odu_struct_pm_tcm_res_get */

/*******************************************************************************
* odu_struct_ftfl_src_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   configures the source of FTFL field of an ODU_STRUCT or the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   ftfl_source         - PM/TCMi FTFL source\n
*                         0 -- Incoming FTFL value\n
*                         1 -- Force FTFL value
*   ftfl_force_field    - Only valid when source = 2.\n
*                         0 -- force both backward and forward field\n
*                         1 -- force backward field\n
*                         2 -- force forward field
*   target              - configuration target of odu struct data.
*                         see odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_ftfl_src_set(coreotn_handle_t *coreotn_handle,
                                         odu_struct_t *odu_struct,
                                         odu_struct_odu_oh_ins_src_t ftfl_source,
                                         UINT8 ftfl_force_field,
                                         odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (ftfl_source > LAST_ODU_STRUCT_OH_INS_SRC || ftfl_force_field > 2 || target > LAST_CONFIG_TARGET)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    odu_struct->mem_ptr->ftfl_src = ftfl_source;

    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

        if (NULL != local_odu_tfrm_handle)
        {
            result = odu_tfrm_ftfl_src_cfg(local_odu_tfrm_handle,
                                           odu_struct->switch_data.oduk_data.channel, ftfl_source,
                                           ftfl_force_field);
        }
        else {
            PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
        }
    }

    PMC_RETURN(result);
} /* odu_struct_ftfl_src_set */

/*******************************************************************************
* odu_struct_tcm_act_src_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This API configures the insertion source and the value of TCM ACT field in 
*   an ODU_STRUCT or the device. 
*
*   When configured for 'tcm_act_source' = ODU_STRUCT_ODU_OH_INS_INCOMING,
*   the value transferred on TCM_ACT byte will be from highest to lowest priority: \n
*       -- Data from the External Overhead Port (if enabled) \n
*       -- Register forcing \n
*       -- Data from incoming ODUk frame
*
*   If external overhead port is enabled to insert TCM_ACT bytes, the value 
*   inserted from register using this API will be overwritten and have no effect. 
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   tcm_act_source      - Enum, TCM ACT source. See odu_struct_odu_oh_ins_src_t.
*                         Note: ODU_STRUCT_ODU_OH_INS_ODU_RCP is invalid for 
*                               TCM_ACT insertion.
*   tcm_act_force_val   - For 'tcm_act_source' = ODU_STRUCT_ODU_OH_INS_REGISTER_FORCE,
*                         The value to be inserted into TCM_ACT field of
*                         PM or TCMi layer \n
*                         Valid range: 0 - 0xFF
*                         For other insertion sources, set to 0.
*   target              - configuration target of odu struct data.
*                         see odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_tcm_act_src_set(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            odu_struct_odu_oh_ins_src_t tcm_act_source,
                                            UINT8 tcm_act_force_val,
                                            odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (tcm_act_source > LAST_ODU_STRUCT_OH_INS_SRC || target > LAST_CONFIG_TARGET)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    odu_struct->mem_ptr->tcm_act_src = tcm_act_source;

    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

        if (NULL != local_odu_tfrm_handle)
        {
            result = odu_tfrm_tcm_act_src_cfg(local_odu_tfrm_handle,
                                              odu_struct->switch_data.oduk_data.channel,
                                              (odu_tfrm_oh_ins_src_t)tcm_act_source,
                                              tcm_act_force_val);
        }
        else {
            PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
        }
    }

    PMC_RETURN(result);
} /* odu_struct_tcm_act_src_set */

/*******************************************************************************
* odu_struct_tcm_act_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retrieves TCM ACT field from an ODU_STRUCT or the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   direction           - direction of the stream to extract TCM_ACT field
*                         0 -- RX (Sink)
*                         1 -- TX (Source)
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   tcm_act_val         - retrieved TCM ACT value.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tcm_act_get(coreotn_handle_t *coreotn_handle,
                                        odu_struct_t *odu_struct,
                                        odu_struct_stream_dir_t direction,
                                        odu_struct_ext_mode_t ext_mode,
                                        UINT8 *tcm_act_val)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;  
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;   

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (ext_mode > LAST_ODU_STRUCT_EXTRACT || direction > 1)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        if (direction == 1)
        {
            /* Tx */
            local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
            
            if (NULL != local_odu_tfrm_handle)
            {
                result = odu_tfrm_tcm_act_get(local_odu_tfrm_handle,
                                              odu_struct->switch_data.oduk_data.channel, tcm_act_val);
            }
            else {
                result = COREOTN_ERR_NULL_HANDLE;
            }  
        }
        else {
            local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
            
            if (NULL != local_odu_rfrm_handle)
            {
                result = odu_rfrm_acpt_tcm_act_ext(local_odu_rfrm_handle,
                                                   odu_struct->switch_data.oduk_data.channel, tcm_act_val);
            }
            else {
                result = COREOTN_ERR_NULL_HANDLE;
            }
        }                                                   
    }
    else {
        *tcm_act_val = odu_struct->mem_ptr->tcm_act_force_val;
    }

    PMC_RETURN(result);
} /* odu_struct_tcm_act_get */

/*******************************************************************************
* odu_struct_aps_pcc_src_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Configures the source of APS/PCC field of a operational connection
*   monitoring layer in an ODU_STRUCT.
*
*   When configured for 'aps_pcc_source' = ODU_STRUCT_ODU_OH_INS_INCOMING,
*   the value transferred on APS_PCC bytes will be from highest to lowest priority: \n
*       -- Data from the External Overhead Port (if enabled) \n
*       -- Register forcing \n
*       -- Data from incoming ODUk frame
*
*   If external overhead port is enabled to insert APS_PCC bytes, the value 
*   inserted from register using this API will be overwritten and have no effect. 
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   mon_level           - PM or TCMi layer number\n
*                         0 -- PM\n
*                         1 to 6 -- TCM1 to TCM6\n
*                         7 -- ODUk server layer trail
*   aps_pcc_source      - APS/PCC source\n
*                         0 -- Incoming APS/PCC value\n
*                         1 -- Force APS/PCC value\n
*   aps_pcc_force_val   - For source == 1 (otherwise set to 0)
*                         The value to be inserted into APS/PCC field.
*                         Valid range: 0x0 - 0xFFFFFFFF
*   target              - configuration target of odu struct data.
*                         see odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_aps_pcc_src_set(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            UINT8 mon_level,
                                            odu_struct_odu_oh_ins_src_t aps_pcc_source,
                                            UINT32 aps_pcc_force_val,
                                            odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (mon_level > 7 || aps_pcc_source > LAST_ODU_STRUCT_OH_INS_SRC
        || target > LAST_CONFIG_TARGET)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    /* find index of aps_pcc_force_val */
    odu_struct->mem_ptr->aps_pcc_src = aps_pcc_source;

    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

        if (NULL != local_odu_tfrm_handle)
        {
            result = odu_tfrm_aps_pcc_src_cfg(local_odu_tfrm_handle,
                                              odu_struct->switch_data.oduk_data.channel,
                                              mon_level, 
                                              (odu_tfrm_oh_ins_src_t)aps_pcc_source,
                                              aps_pcc_force_val);
        }
        else {
            PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
        }
    }

    PMC_RETURN(result);
} /* odu_struct_aps_pcc_src_set */

/*******************************************************************************
* odu_struct_tx_aps_pcc_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Retrieves the value of APS/PCC field of a operational connection
*   monitoring layer in transmit ODU stream represented in ODU_STRUCT.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   mon_level           - PM or TCMi layer number\n
*                         0 -- PM\n
*                         1 to 6 -- TCM1 to TCM6\n
*                         7 -- ODUk server layer trail
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   *aps_pcc_val        - value of APS/PCC field\n
*                         
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tx_aps_pcc_get(coreotn_handle_t *coreotn_handle,
                                           odu_struct_t *odu_struct,
                                           UINT8 mon_level,
                                           odu_struct_ext_mode_t ext_mode,
                                           UINT32 *aps_pcc_val)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle != NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (mon_level > 7 || ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

        if (NULL != local_odu_tfrm_handle)
        {
            result = odu_tfrm_aps_pcc_ext(local_odu_tfrm_handle,
                                          odu_struct->switch_data.oduk_data.channel,
                                          mon_level, aps_pcc_val);
        }
        else {
            PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
        }
    }
    else {
        
        *aps_pcc_val = odu_struct->mem_ptr->aps_pcc_force_val[mon_level];
    }        

    PMC_RETURN(result);
} /* odu_struct_tx_aps_pcc_get */

/*******************************************************************************
* odu_struct_rx_aps_pcc_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Retrieves the value of APS/PCC field of a operational connection
*   monitoring layer in receive ODU stream represented in ODU_STRUCT.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   mon_level           - PM or TCMi layer number\n
*                         0 -- PM\n
*                         1 to 6 -- TCM1 to TCM6\n
*                         7 -- ODUk server layer trail
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   *aps_pcc_val        - value of APS/PCC field\n
*                         
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_aps_pcc_get(coreotn_handle_t *coreotn_handle,
                                           odu_struct_t *odu_struct,
                                           UINT8 mon_level,
                                           odu_struct_ext_mode_t ext_mode,
                                           UINT32 *aps_pcc_val)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle != NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (mon_level > 7 || ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);

        if (NULL != local_odu_rfrm_handle)
        {
            result = odu_rfrm_acpt_aps_ext(local_odu_rfrm_handle,
                                           odu_struct->switch_data.oduk_data.channel, mon_level,
                                           aps_pcc_val);
        }
        else {
            PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
        }
    }       

    PMC_RETURN(result);
} /* odu_struct_rx_aps_pcc_get */

/*******************************************************************************
* odu_struct_tx_res1_src_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This API configures the insertion source and the value of RES1 field in an 
*   ODU_STRUCT or the device.
*
*   When configured for 'res1_source' = ODU_STRUCT_ODU_OH_INS_INCOMING,
*   the value transferred on RES1 bytes will be from highest to lowest priority: \n
*       -- Data from the External Overhead Port (if enabled) \n
*       -- Register forcing \n
*       -- Data from incoming ODUk frame
*
*   If external overhead port is enabled to insert RES1 bytes, the value 
*   inserted from register using this API will be overwritten and have no effect.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   res1_source         - Enum, RES1 source. See odu_struct_odu_oh_ins_src_t.
*                         Note: ODU_STRUCT_ODU_OH_INS_ODU_RCP is invalid for 
*                               RES1 insertion.
*   res1_force_val      - For 'res1_source' = ODU_STRUCT_ODU_OH_INS_REGISTER_FORCE,
*                         the value to be inserted into bits[15:0] of RES field\n
*                         Valid range: 0x0 - 0xFFFF
*                         For all other sources, set to 0.
*   target              - configuration target of odu struct data.
*                         see odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_tx_res1_src_set(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            odu_struct_odu_oh_ins_src_t res1_source,
                                            UINT16 res1_force_val,
                                            odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle;
    
    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (res1_source > LAST_ODU_STRUCT_OH_INS_SRC || target > LAST_CONFIG_TARGET)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    odu_struct->mem_ptr->res1_src = res1_source;
    
    if (res1_source == ODU_STRUCT_ODU_OH_INS_REGISTER_FORCE)
    {
        odu_struct->mem_ptr->res1_force_val = res1_force_val;
    }    
    
    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
        PMC_ASSERT(local_odu_tfrm_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);

        result = odu_tfrm_res1_src_cfg(local_odu_tfrm_handle,
                                       odu_struct->switch_data.oduk_data.channel, 
                                       (odu_tfrm_oh_ins_src_t)res1_source,
                                       res1_force_val);
    }
    
    PMC_RETURN(result);
} /* odu_struct_tx_res1_src_set */


/*******************************************************************************
* odu_struct_tx_res2_src_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This API configures RES2 insertion source and value in an ODU_STRUCT 
*   or the device.
*
*   When configured for 'Incoming RES2 value' the value transferred on
*   TCM_ACT byte will be from highest to lowest priority: \n
*       -- Data from the External Overhead Port (if enabled) \n
*       -- Register forcing \n
*       -- Data from incoming ODUk frame
*
*   If external overhead port is enabled to insert RES2 bytes, the value 
*   inserted from register using this API will be overwritten and have no effect. 
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   res2_source         - Enum for RES2 source. See odu_struct_odu_oh_ins_src_t\n
*                         Note: ODU_STRUCT_ODU_OH_INS_ODU_RCP 
*   res2_force_val      - 6 bytes RES field. The value to be inserted into \n
*                         row 4 and col 9-14 of RES field.
*   target              - configuration target of odu struct data.
*                         see odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_tx_res2_src_set(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            odu_struct_odu_oh_ins_src_t res2_source,
                                            UINT8 res2_force_val[6],
                                            odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle;
    UINT32 i;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (res2_source > LAST_ODU_STRUCT_OH_INS_SRC || target > LAST_CONFIG_TARGET)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    /* Update RES2 insertion source field in ODU_struct */
    odu_struct->mem_ptr->res2_src = res2_source;

    /* Update RES2 force value in ODU struct if register forcing is the 
    *  insertion source
    */
    if (res2_source == ODU_STRUCT_ODU_OH_INS_REGISTER_FORCE)
    {
        for (i = 0; i < 6; i++)
        {
            odu_struct->mem_ptr->res2_force_val[i] = res2_force_val[i];
        }
    }
       
    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
        PMC_ASSERT(local_odu_tfrm_handle != NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
        
        result = odu_tfrm_res2_src_cfg(local_odu_tfrm_handle,
                                       odu_struct->switch_data.oduk_data.channel, 
                                       (odu_tfrm_oh_ins_src_t)res2_source,
                                       res2_force_val);
    }

    PMC_RETURN(result);
} /* odu_struct_tx_res2_src_set */

/*******************************************************************************
* odu_struct_res1_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retrieves RES1 field from an ODU_STRUCT or directly from the device.
*
* INPUTS:
*   coreotn_handle      - pointer to COREOTN handle instance
*   odu_struct          - pointer to ODU_STRUCT to be operated on
*   direction           - direction of the traffic stream to extract RES1 field
*                         0 -- RX (Sink)
*                         1 -- TX (Source)
*   res1_val            - pointer to storage for RES1 field
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   res1_val            - retrieved RES1 field value.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_res1_get(coreotn_handle_t *coreotn_handle,
                                     odu_struct_t *odu_struct,
                                     odu_struct_stream_dir_t direction,
                                     odu_struct_ext_mode_t ext_mode,
                                     UINT16 *res1_val)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle;
    odu_rfrm_handle_t *local_odu_rfrm_handle;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (ext_mode > LAST_ODU_STRUCT_EXTRACT || direction > 1)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        if (direction == 1)
        {
            /* Tx */
            local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
            PMC_ASSERT(local_odu_tfrm_handle != NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
            
            result = odu_tfrm_res1_get(local_odu_tfrm_handle,
                                       odu_struct->switch_data.oduk_data.channel, res1_val);
        }
        else {
            local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
            PMC_ASSERT(local_odu_rfrm_handle != NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
            
            result = odu_rfrm_acpt_res1_ext(local_odu_rfrm_handle,
                                            odu_struct->switch_data.oduk_data.channel, res1_val);
        }
    }
    else {
        *res1_val = odu_struct->mem_ptr->res1_force_val;
    }

    PMC_RETURN(result);
} /* odu_struct_tx_res1_get */

/*******************************************************************************
* odu_struct_res2_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retrieves RES2 field from an ODU_STRUCT or directly from the device.
*
* INPUTS:
*   coreotn_handle      - pointer to COREOTN handle instance
*   odu_struct          - pointer to ODU_STRUCT to be operated on
*   direction           - direction of the traffic stream to extract RES1 field.
*                         See odu_struct_stream_dir_t.
*                         0 -- RX (sink)
*                         1 -- TX (source)
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   res2_val            - retrieved RES2 value.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_res2_get(coreotn_handle_t *coreotn_handle,
                                     odu_struct_t *odu_struct,
                                     odu_struct_stream_dir_t direction,
                                     odu_struct_ext_mode_t ext_mode,
                                     UINT8 *res2_val)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    UINT32 i;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (ext_mode > LAST_ODU_STRUCT_EXTRACT || direction > 1)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        if (direction == 1)
        {
            local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
            PMC_ASSERT(local_odu_tfrm_handle != NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
            
            result = odu_tfrm_res2_get(local_odu_tfrm_handle,
                                       odu_struct->switch_data.oduk_data.channel, res2_val);
        }
        else {
            local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
            PMC_ASSERT(local_odu_rfrm_handle != NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
            
            result = odu_rfrm_acpt_res2_ext(local_odu_rfrm_handle,
                                            odu_struct->switch_data.oduk_data.channel, res2_val);
        }                        
    }
    else {
        for (i = 0; i < 6; i++)
        {
            *res2_val += odu_struct->mem_ptr->res2_force_val[i];
        }
    }

    PMC_RETURN(result);
} /* odu_struct_res2_get */

/*******************************************************************************
* odu_struct_tx_exp_src_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This API configures the source and the value of EXP field in an ODU_STRUCT 
*   or the device.
*
*   When configured for 'exp_source' = ODU_STRUCT_ODU_OH_INS_INCOMING,
*   the value transferred on EXP byte will be from highest to lowest priority: \n
*       -- Data from the External Overhead Port (if enabled) \n
*       -- Register forcing \n
*       -- Data from incoming ODUk frame
*
*   If external overhead port is enabled to insert EXP byte, the value 
*   inserted from register using this API will be overwritten and have no effect. 
*
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   exp_source          - Enum, EXP source. See odu_struct_odu_oh_ins_src_t.
*                         Note: ODU_STRUCT_ODU_OH_INS_ODU_RCP is invalid for
*                               EXP field insertion.
*   exp_force_val       - For 'exp_src' = ODU_STRUCT_ODU_OH_INS_REGISTER_FORCE, 
*                         the value to be inserted into row 2 and col 9-14 of 
*                         RES field.
*                         Valid range: 0 - 0xFF.
*                         For all other sources, set to 0.
*   target              - configuration target of odu struct data.
*                         see odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_tx_exp_src_set(coreotn_handle_t *coreotn_handle,
                                           odu_struct_t *odu_struct,
                                           odu_struct_odu_oh_ins_src_t exp_source,
                                           UINT16 exp_force_val,
                                           odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (exp_source > LAST_ODU_STRUCT_OH_INS_SRC || target > LAST_CONFIG_TARGET)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    odu_struct->mem_ptr->exp_src = exp_source;
    
    if (exp_source == ODU_STRUCT_ODU_OH_INS_REGISTER_FORCE)
    {
        odu_struct->mem_ptr->exp_force_val = exp_force_val;
    }    

    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
        PMC_ASSERT(local_odu_tfrm_handle != NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
        
        result = odu_tfrm_exp_src_cfg(local_odu_tfrm_handle,
                                      odu_struct->switch_data.oduk_data.channel, 
                                      (odu_tfrm_oh_ins_src_t)exp_source,
                                      exp_force_val);
    }

    PMC_RETURN(result);
} /* odu_struct_tx_exp_src_set */

/*******************************************************************************
* odu_struct_exp_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retrieves EXP field from an ODU_STRUCT or directly from the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   direction           - direction to extract EXP field. See odu_struct_stream_dir_t
*                         0 -- RX
*                         1 -- TX
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   exp_val             - retrieved EXP field value.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_exp_get(coreotn_handle_t *coreotn_handle,
                                    odu_struct_t *odu_struct,
                                    odu_struct_stream_dir_t direction,
                                    odu_struct_ext_mode_t ext_mode,
                                    UINT16 *exp_val)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (direction > 1 || ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        if (direction == 1)
        {
            /* TX */
            local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

            if (NULL != local_odu_tfrm_handle)
            {
                result = odu_tfrm_exp_get(local_odu_tfrm_handle,
                                          odu_struct->switch_data.oduk_data.channel, exp_val);
            }
            else {
                result = COREOTN_ERR_NULL_HANDLE;
            }
        }
        else {
            local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);

            if (NULL != local_odu_rfrm_handle)
            {
                result = odu_rfrm_acpt_exp_ext(local_odu_rfrm_handle,
                                               odu_struct->switch_data.oduk_data.channel, exp_val);
            }
            else {
                result = COREOTN_ERR_NULL_HANDLE;
            }
        }
    }
    else {
       *exp_val = odu_struct->mem_ptr->exp_force_val;
    }

    PMC_RETURN(result);
} /* odu_struct_exp_get */

/*******************************************************************************
* odu_struct_tx_oh_ins_src_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retrieves EXP field from an ODU_STRUCT or directly from the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   oh_field            - Enum for optionally inserted overhead fields. 
*                         See odu_struct_optional_oh_ins_t
*                         0 -- FTFL
*                         1 -- TCM_ACT
*                         2 -- PM_TCM_RES
*                         3 -- RES1
*                         4 -- EXP
*                         5 -- RES2
*                         6 -- GCC2
*                         7 -- GCC1
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   *oh_ins_src         - extracted insertion source of the selected overhead
*                         field.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tx_oh_ins_src_get(coreotn_handle_t *coreotn_handle,
                                              odu_struct_t *odu_struct,
                                              odu_struct_optional_oh_ins_t oh_field,
                                              odu_struct_ext_mode_t ext_mode,
                                              UINT8 *oh_ins_src)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle,
                                                           odu_struct);

        if (NULL != local_odu_tfrm_handle)
        {
            switch (oh_field)
            {
                case ODU_STRUCT_ODU_OH_FTFL: /* FTFL */
                    result = odu_tfrm_ftfl_src_get(local_odu_tfrm_handle,
                                                   odu_struct->switch_data.oduk_data.channel,
                                                   oh_ins_src);
                break;
                case ODU_STRUCT_ODU_OH_TCM_ACT: /* TCM_ACT */
                    result = odu_tfrm_tcm_act_src_get(local_odu_tfrm_handle,
                                                      odu_struct->switch_data.oduk_data.channel,
                                                      oh_ins_src);
                break;
                case ODU_STRUCT_ODU_OH_PM_TCM_RES: /* PM_TCM_RES */
                    result = odu_tfrm_pm_tcm_res_src_get(local_odu_tfrm_handle,
                                                         odu_struct->switch_data.oduk_data.channel,
                                                         oh_ins_src);
                break;
                case ODU_STRUCT_ODU_OH_RES1: /* RES1 */
                    result = odu_tfrm_res1_src_get(local_odu_tfrm_handle,
                                                   odu_struct->switch_data.oduk_data.channel,
                                                   oh_ins_src);
                break;
                case ODU_STRUCT_ODU_OH_EXP: /* EXP */
                    result = odu_tfrm_exp_src_get(local_odu_tfrm_handle,
                                                  odu_struct->switch_data.oduk_data.channel,
                                                  oh_ins_src);
                break;
                case ODU_STRUCT_ODU_OH_RES2: /* RES2 */
                    result = odu_tfrm_res2_src_get(local_odu_tfrm_handle,
                                                   odu_struct->switch_data.oduk_data.channel, 
                                                   oh_ins_src);
                break;
                case ODU_STRUCT_ODU_OH_GCC2: /* GCC2 */
                    result = odu_tfrm_gcc2_src_get(local_odu_tfrm_handle,
                                                   odu_struct->switch_data.oduk_data.channel, 
                                                   oh_ins_src);
                break;
                case ODU_STRUCT_ODU_OH_GCC1: /* GCC1 */
                    result = odu_tfrm_gcc1_src_get(local_odu_tfrm_handle,
                                                   odu_struct->switch_data.oduk_data.channel, 
                                                   oh_ins_src);
                break;
                default:
                    PMC_RETURN(COREOTN_ERR_INVALID_ARG);
                break;
            }                                                                   
        }
        else {
            result = COREOTN_ERR_NULL_HANDLE;
        }
    }
    else {
        switch (oh_field)
        {
            case ODU_STRUCT_ODU_OH_FTFL: /* FTFL */
                *oh_ins_src = odu_struct->mem_ptr->ftfl_src;
            break;
            case ODU_STRUCT_ODU_OH_TCM_ACT: /* TCM_ACT */
                *oh_ins_src = odu_struct->mem_ptr->tcm_act_src;
            break;
            case ODU_STRUCT_ODU_OH_PM_TCM_RES: /* PM_TCM_RES */
                *oh_ins_src = odu_struct->mem_ptr->pm_tcm_res_src;
            break;
            case ODU_STRUCT_ODU_OH_RES1: /* RES1 */
                *oh_ins_src = odu_struct->mem_ptr->res1_src;
            break;
            case ODU_STRUCT_ODU_OH_EXP: /* EXP */
                *oh_ins_src = odu_struct->mem_ptr->exp_src;
            break;
            case ODU_STRUCT_ODU_OH_RES2: /* RES2 */
                *oh_ins_src = odu_struct->mem_ptr->res2_src;
            break;
            case ODU_STRUCT_ODU_OH_GCC2: /* GCC2 */
                *oh_ins_src = odu_struct->mem_ptr->gcc2_src;
            break;
            case ODU_STRUCT_ODU_OH_GCC1: /* GCC1 */
                *oh_ins_src = odu_struct->mem_ptr->gcc1_src;
            break;
            default:
                PMC_RETURN(COREOTN_ERR_INVALID_ARG);
            break;
        }             
    }

    PMC_RETURN(result);
} /* odu_struct_tx_oh_ins_src_get */

/*******************************************************************************
* odu_struct_tx_aps_pcc_src_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retrieves EXP field from an ODU_STRUCT or directly from the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   mon_level           - connection monitoring level
*                         0 -- ODUk Path
*                         1 -- 6: TCM1 - 6
*                         7 -- ODUk server layer trail
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   *aps_pcc_src        - extracted insertion source of APS_PCC of the selected
*                         connection monitoring level
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tx_aps_pcc_src_get(coreotn_handle_t *coreotn_handle,
                                               odu_struct_t *odu_struct,
                                               UINT8 mon_level,
                                               odu_struct_ext_mode_t ext_mode,
                                               UINT8 *aps_pcc_src)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    if (ext_mode ==  ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle,
                                                           odu_struct);

        if (NULL != local_odu_tfrm_handle)
        {
            result = odu_tfrm_aps_pcc_src_get(local_odu_tfrm_handle,
                                              odu_struct->switch_data.oduk_data.channel, 
                                              mon_level,
                                              aps_pcc_src);
        }
        else {
            result =  COREOTN_ERR_NULL_HANDLE;
        }
    }
    else {
        *aps_pcc_src = (odu_struct->mem_ptr->aps_pcc_src >> mon_level) & 0x1;
    }
    
    PMC_RETURN(result);                                                               

} /* odu_struct_tx_aps_pcc_src_get */

/*******************************************************************************
* odu_struct_tx_gcc1_src_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This API configures the insertion source and the value of GCC1 field in an 
*   ODU_STRUCT or the device.
*
*   When configured for 'gcc1_source' = ODU_STRUCT_ODU_OH_INS_INCOMING,
*   the value transferred on GCC1 bytes will be from highest to lowest priority: \n
*       -- Data from the External Overhead Port (if enabled) \n
*       -- Register forcing \n
*       -- Data from incoming ODUk frame
*
*   If external overhead port is enabled to insert GCC1 byte, the value 
*   inserted from register using this API will be overwritten and have no effect. 
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   gcc1_source         - Enum, GCC1 source. See odu_struct_odu_oh_ins_src_t. \n
*                         Note: ODU_STRUCT_ODU_OH_INS_ODU_RCP is invalid for
*                               GCC1 insertion.
*   gcc1_force_val      - For 'gcc1_source' == ODU_STRUCT_ODU_OH_INS_REGISTER_FORCE,
*                         the value to be inserted into row 4 GCC1 field.\n
*                         Valid range: 0x0 - 0xFFFFFFFF \n
*                         For all other sources, set to 0.
*   target              - configuration target of odu struct data.
*                         see odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_tx_gcc1_src_set(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            odu_struct_odu_oh_ins_src_t gcc1_source,
                                            UINT16 gcc1_force_val,
                                            odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
    
    PMC_ENTRY();
    
    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (gcc1_source > LAST_ODU_STRUCT_OH_INS_SRC || target >= LAST_CONFIG_TARGET)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    odu_struct->mem_ptr->gcc1_src = gcc1_source;
    
    if (gcc1_source == ODU_STRUCT_ODU_OH_INS_REGISTER_FORCE)
    {
        odu_struct->mem_ptr->gcc1_force_val = gcc1_force_val;
    }
            
    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
        PMC_ASSERT(local_odu_tfrm_handle != NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
        
        result = odu_tfrm_gcc1_src_cfg(local_odu_tfrm_handle,
                                       odu_struct->switch_data.oduk_data.channel, 
                                       (odu_tfrm_oh_ins_src_t)gcc1_source,
                                       gcc1_force_val);
    }                                                          
    
    PMC_RETURN(result);
} /* odu_struct_tx_gcc1_src_set */


/*******************************************************************************
* odu_struct_tx_gcc2_src_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This API configures the insertion source and the value of GCC2 field in an  
*   ODU_STRUCT or the device.
*
*   When configured for 'gcc2_source' = ODU_STRUCT_ODU_OH_INS_INCOMING,
*   the value transferred on GCC2 bytes will be from highest to lowest priority: \n
*       -- Data from the External Overhead Port (if enabled) \n
*       -- Register forcing \n
*       -- Data from incoming ODUk frame
*
*   If external overhead port is enabled to insert GCC2 byte, the value 
*   inserted from register using this API will be overwritten and have no effect.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   gcc2_source         - Enum, GCC2 source. See odu_struct_odu_oh_ins_src_t \n
*                         Note: ODU_STRUCT_ODU_OH_INS_ODU_RCP is invalid for
*                               GCC2 insertion.
*   gcc2_force_val      - For 'gcc2_source' == ODU_STRUCT_ODU_OH_INS_REGISTER_FORCE, 
*                         the value to be inserted into row 4 GCC2 field.\n
*                         Valid range: 0x0 - 0xFFFFFFFF
*                         For all other source, set to 0.
*   target              - configuration target of odu struct data.
*                         see odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_tx_gcc2_src_set(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            odu_struct_odu_oh_ins_src_t gcc2_source,
                                            UINT16 gcc2_force_val,
                                            odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
    
    PMC_ENTRY();
    
    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (gcc2_source > LAST_ODU_STRUCT_OH_INS_SRC || target >= LAST_CONFIG_TARGET)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    odu_struct->mem_ptr->gcc2_src = gcc2_source;
    
    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
        PMC_ASSERT(local_odu_tfrm_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
        
        result = odu_tfrm_gcc2_src_cfg(local_odu_tfrm_handle,
                                       odu_struct->switch_data.oduk_data.channel, 
                                       (odu_tfrm_oh_ins_src_t)gcc2_source,
                                       gcc2_force_val);
    }  
    
    PMC_RETURN(result);
} /* odu_struct_tx_gcc2_src_set */

/*******************************************************************************
* odu_struct_gcc1_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retrieves GCC1 field from an ODU_STRUCT or directly from the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   direction           - direction of the traffic stream to extract GCC1
*                         0 -- RX
*                         1 -- TX
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   gcc1_val            - retrieved GCC1 field value.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_gcc1_get(coreotn_handle_t *coreotn_handle,
                                     odu_struct_t *odu_struct,
                                     odu_struct_stream_dir_t direction,
                                     odu_struct_ext_mode_t ext_mode,
                                     UINT16 *gcc1_val)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    
    PMC_ENTRY();
    
    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (direction > 1 || ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        if (direction == 1)
        {
            /* TX */
            local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
        
            if (NULL != local_odu_tfrm_handle)
            {
                result = odu_tfrm_gcc1_get(local_odu_tfrm_handle,
                                           odu_struct->switch_data.oduk_data.channel, gcc1_val);
            }
            else {
                result = COREOTN_ERR_NULL_HANDLE;
            }
        }
        else {
            /* RX */
            local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
            
            if (NULL != local_odu_rfrm_handle)
            {
                result = odu_rfrm_acpt_gcc_ext(local_odu_rfrm_handle,
                                               odu_struct->switch_data.oduk_data.channel, 0, gcc1_val);   
            }
            else {
                result = COREOTN_ERR_NULL_HANDLE;
            }
        }
    }
    else {
        *gcc1_val = odu_struct->mem_ptr->gcc1_force_val;
    }                        
     
    PMC_RETURN(result);
} /* odu_struct_gcc1_get */

/*******************************************************************************
* odu_struct_gcc2_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retrieves GCC2 field from an ODU_STRUCT or directly from the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   direction           - direction of the traffic stream to extract GCC1
*                         0 -- RX
*                         1 -- TX
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   *gcc2_val           - retrieved GCC2 field value.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_gcc2_get(coreotn_handle_t *coreotn_handle,
                                     odu_struct_t *odu_struct,
                                     odu_struct_stream_dir_t direction,
                                     odu_struct_ext_mode_t ext_mode,
                                     UINT16 *gcc2_val)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    
    PMC_ENTRY();
    
    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (direction > 1 || ext_mode >= LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        if (direction == 1)
        {
            /* TX */
            local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
        
            if (NULL != local_odu_tfrm_handle)
            {
                result = odu_tfrm_gcc2_get(local_odu_tfrm_handle,
                                           odu_struct->switch_data.oduk_data.channel, gcc2_val);
            }
            else {
                result = COREOTN_ERR_NULL_HANDLE;
            }
        }
        else {
            /* RX */
            local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
            
            if (NULL != local_odu_rfrm_handle)
            {
                result = odu_rfrm_acpt_gcc_ext(local_odu_rfrm_handle,
                                               odu_struct->switch_data.oduk_data.channel, 1, gcc2_val);   
            }
            else {
                result = COREOTN_ERR_NULL_HANDLE;
            }
        }
    }
    else {
        *gcc2_val = odu_struct->mem_ptr->gcc2_force_val;
    }               
    
    PMC_RETURN(result);
} /* odu_struct_gcc2_get */

/*******************************************************************************
* odu_struct_deg_m_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures number of degraded monitoring intervals required to
*   induced dDEG (DEGM). This API is only used for hardware dDEG.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi layer. See odu_struct_odu_frm_pm_tcm_id_t.
*   deg_m               - degrade defect consecutive one second monitoring
*                         intervals for the TCM or PM layer\n
*                         Valid range: 
*                          COREOTN_HARDWARE_DDEG: 1-15 seconds
*                          COREOTN_ENHANCED_DDEG or 
*                          COREOTN_ENH_BURSTY_DDEG: 2-10 seconds
*   target              - configuration target. Configuration of this API can
*                         be applied to ODU_STRUCT only or ODU_STRUCT and to the
*                         device. See odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_deg_m_set(coreotn_handle_t *coreotn_handle,
                                      odu_struct_t *odu_struct,
                                      odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                      UINT8 deg_m,
                                      odu_struct_target_t target)
{
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    UINT16 tcm_pool_index;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
       
    if (pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
    {
        odu_struct->mem_ptr->deg_m_pm = deg_m;
    }
    else {
        /* TCM */
        result = odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle, 
                                                           odu_struct,
                                                           pm_tcm_num, 
                                                           &tcm_pool_index);
    
        /* read from ODU struct */
        if (PMC_SUCCESS == result)
        {
            /* find tcm_pool_id_<tcm_pool_index> */
            odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].deg_m = deg_m; 
        }
    }        

    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);

        /* set deg_m in the RFRM */
        result = odu_rfrm_pm_tcmi_deg_m_set(local_odu_rfrm_handle,
                                            odu_struct->switch_data.oduk_data.channel,
                                            (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                            deg_m);
    }

    PMC_RETURN(result);

} /* odu_struct_deg_m_set */


/*******************************************************************************
* odu_struct_deg_m_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures number of degraded monitoring intervals required to
*   induced dDEG (DEGM). This API is used only for hardware dDEG reporting.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi layer. See odu_struct_odu_frm_pm_tcm_id_t.
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   deg_m               - degrade defect consecutive one second monitoring
*                         intervals for the TCM or PM layer\n
*                         
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_deg_m_get(coreotn_handle_t *coreotn_handle,
                                              odu_struct_t *odu_struct,
                                              odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                              odu_struct_ext_mode_t ext_mode,
                                              UINT8 *deg_m)
{

    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    UINT16 tcm_pool_index;
    PMC_ERROR result = PMC_SUCCESS;


    PMC_ENTRY();
      
    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE) 
    {

        local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);

        if (NULL != local_odu_rfrm_handle)
        {
            /* get deg_m in the RFRM */
            result = odu_rfrm_pm_tcmi_deg_m_get(local_odu_rfrm_handle,
                                                odu_struct->switch_data.oduk_data.channel,
                                                (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                                 deg_m);
        } 
        else {
            result = COREOTN_ERR_NULL_HANDLE;
        }            
    } 
    else {

       if (pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
        {
            *deg_m = odu_struct->mem_ptr->deg_m_pm;
        }
        else {

            /* TCM */
            result = odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle, 
                                                               odu_struct,
                                                               pm_tcm_num, 
                                                               &tcm_pool_index);
            /* read from ODU struct */
            if (PMC_SUCCESS == result)
            {
                /* find tcm_pool_id_<tcm_pool_index> */  
                *deg_m = odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].deg_m;  
            }
        }       
    }

    PMC_RETURN(result);

} /* odu_struct_deg_m_get */


/*******************************************************************************
* odu_struct_deg_th_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures degrade defect one second errored block count threshold in an
*   ODU_STRUCT.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi layer. See odu_struct_odu_frm_pm_tcm_id_t.
*   deg_th              - degrade defect one second errored block count threshold
*   target              - configuration target. Configuration of this API can
*                         be applied to ODU_STRUCT only or ODU_STRUCT and to the
*                         device. See odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_deg_th_set(coreotn_handle_t *coreotn_handle,
                                              odu_struct_t *odu_struct,
                                              odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                              UINT32 deg_th,
                                              odu_struct_target_t target)
{

    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    UINT16 tcm_pool_index;
    PMC_ERROR result = PMC_SUCCESS;


    PMC_ENTRY();

    if (pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
    {
        odu_struct->mem_ptr->deg_th_pm = deg_th;
    }
    else {
        /* TCM */
        result = odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle, 
                                                           odu_struct,
                                                           pm_tcm_num, 
                                                           &tcm_pool_index);
    
        /* read from ODU struct */
        if (PMC_SUCCESS == result)
        {
            /* find tcm_pool_id_<tcm_pool_index> */     
            odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].deg_th = deg_th;
        }
    }        

    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {       
        /* set deg_th in the RFRM */
        local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
        
        if (NULL != local_odu_rfrm_handle)
        {
            result = odu_rfrm_pm_tcmi_deg_th_set(local_odu_rfrm_handle,
                                                 odu_struct->switch_data.oduk_data.channel,
                                                 (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                                 deg_th);
        }
        else {
            result = COREOTN_ERR_NULL_HANDLE;
        }                                                          
    }

    PMC_RETURN(result);

} /* odu_struct_deg_th_set */


/*******************************************************************************
* odu_struct_deg_th_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets degrade defect one second errored block count threshold in an
*   ODU_STRUCT. This API is used for hardware ddeg mode only.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi layer. See odu_struct_odu_frm_pm_tcm_id_t.
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   deg_th              - degrade defect one second errored block count
*                         threshold.
*                         
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_deg_th_get(coreotn_handle_t *coreotn_handle,
                                       odu_struct_t *odu_struct,
                                       odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                       odu_struct_ext_mode_t ext_mode,
                                       UINT32 *deg_th)
{

    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    UINT16 tcm_pool_index;
    PMC_ERROR result = PMC_SUCCESS;


    PMC_ENTRY();
 
    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE) 
    {

        local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);

        if (NULL != local_odu_rfrm_handle)
        {
            /* get deg_m in the RFRM */
            result = odu_rfrm_pm_tcmi_deg_th_get(local_odu_rfrm_handle,
                                                 odu_struct->switch_data.oduk_data.channel,
                                                 (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                                 deg_th);
        }
        else {
            result = COREOTN_ERR_NULL_HANDLE;
        }            
    } 
    else {

       if (pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
        {
            *deg_th = odu_struct->mem_ptr->deg_th_pm;
        }
        else {

            /* TCM */
            result = odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle, 
                                                               odu_struct,
                                                               pm_tcm_num, 
                                                               &tcm_pool_index);
            /* read from ODU struct */
            if (PMC_SUCCESS == result)
            {
                /* find tcm_pool_id_<tcm_pool_index> */
                *deg_th = odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].deg_th;
                
            }
        }       
    }

    PMC_RETURN(result);

} /* odu_struct_deg_th_get */



/*******************************************************************************
* odu_struct_ddeg_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set the dDEG mode context (using firmware dDEG or hardware dDEG).
*   If firmware dDEG is selected, the hardware dDEG is disabled.
*   If the hardware dDEG is selected, the firmware dDEG is disabled.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   ddeg_mode           - mode for dDEG report
*                         0 -- use hardware dDEG 
*                         1 -- use firmware dDEG for both reports
*                         2 -- use firmware dDEG for enhanced statistical cycle report
*                         3 -- use firmware dDEG for enhanced bursty report
*   enable              - when TRUE Ddeg mode is configured, otherwise it is unconfigured
*   odu_segment         - selects the ODU segment to monitor
*                         0 -- COREOTN STG1/2/N/3A
*                         1 -- COREOTN STG4
*   
*   
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_ddeg_mode_set(coreotn_handle_t    *coreotn_handle,
                                          coreotn_ddeg_mode_t  ddeg_mode,
                                          BOOL8                enable,
                                          UINT8                odu_segment)
{

    PMC_ERROR result = PMC_SUCCESS;
    UINT32 tsd_val;
    UINT32 i;
    coreotn_ddeg_mode_t mode;

    PMC_ENTRY();
     
    if (LAST_COREOTN_DDEG_MODE <= ddeg_mode)
    {
        result = COREOTN_ERR_INVALID_ARG; 
    }

    /* only enhanced and bursty ddeg mode can be enabled in the same time, return error otherwise */
    if (PMC_SUCCESS == result)
    {
        if (COREOTN_HARDWARE_DDEG == ddeg_mode && 
            ((coreotn_handle->var.ddeg_mon[(UINT32)COREOTN_ENHANCED_DDEG].ddeg_mode) == COREOTN_ENHANCED_DDEG ||
             (coreotn_handle->var.ddeg_mon[(UINT32)COREOTN_ENH_BURSTY_DDEG]).ddeg_mode == COREOTN_ENH_BURSTY_DDEG))
        {
            result =  COREOTN_ERR_CONFLICT_WITH_ENABLED_DDEG;
        }
    }

    /* only enhanced and bursty ddeg mode can be enabled in the same time, return error otherwise */
    if (PMC_SUCCESS == result)
    {
        if ((COREOTN_ENHANCED_DDEG == ddeg_mode || 
             COREOTN_ENH_BURSTY_DDEG == ddeg_mode) && 
            (coreotn_handle->var.ddeg_mon[(UINT32)COREOTN_HARDWARE_DDEG]).ddeg_mode == COREOTN_HARDWARE_DDEG)
        {
            result = COREOTN_ERR_CONFLICT_WITH_ENABLED_DDEG;
        }
    }


    if (PMC_SUCCESS == result)
    {
        coreotn_handle->var.ddeg_mon[(UINT32)ddeg_mode].ddeg_mode = TRUE == enable ? ddeg_mode : LAST_COREOTN_DDEG_MODE;

        /*
         * Added switch statement to resolve UINT8 assigment to 
         * coreotn_ddeg_mode_t data type.
         */
        if (TRUE == enable)
        {
            mode = ddeg_mode;
        }
        else
        {
            mode = LAST_COREOTN_DDEG_MODE;
        }
    
        if (ddeg_mode == 0 && TRUE == enable) 
        {       
            /* HW dDEG report */
            /*  disable the firmware dDEG reporting */
            /* TSD_DIS = 0
            ** TSD_DIS_PM = 0 
            */  
        
            tsd_val = 0;
        
        }
        else 
        {  
            /* disable the hardware dDEG reporting */
            /* TSD_DIS should be set to 1 for all pool ids and 
            ** TSD_DIS_PM should be set to 1 for all channels 
            */
            tsd_val = 1;
        } 
       
        /* apply to all RFRMs */
        if (odu_segment == 0)
        {
            /* prepare for monitoring STG1/2/N/3A */ 
            for (i = 0; i < 96; i++)
            {
                PMC_ATOMIC_YIELD(coreotn_handle->base.parent_handle,0);
                if (PMC_SUCCESS == result)
                {
                    result = odu_rfrm_pm_tcmi_tsd_tsf_proc_cfg(coreotn_handle->odu_rfrm_stg1_handle,
                                                               i,
                                                               ODU_RFRM_MESSAGE_TYPE_PM,
                                                               0, 0, tsd_val, 0);
                }
            
                if (PMC_SUCCESS == result)
                {
                    result = odu_rfrm_pm_tcmi_tsd_tsf_proc_cfg(coreotn_handle->odu_rfrm_stg2_handle,
                                                               i,
                                                               ODU_RFRM_MESSAGE_TYPE_PM,
                                                               0, 0, tsd_val, 0);
                }
            
                if (PMC_SUCCESS == result)
                {
                    result = odu_rfrm_pm_tcmi_tsd_tsf_proc_cfg(coreotn_handle->odu_rfrm_stg3a_handle,
                                                               i,
                                                               ODU_RFRM_MESSAGE_TYPE_PM,
                                                               0, 0, tsd_val, 0);
                }
            
                if (PMC_SUCCESS == result)
                {
                    result = odu_rfrm_pm_tcmi_tsd_tsf_proc_cfg(coreotn_handle->odu_rfrm_stgN_handle,
                                                               i,
                                                               ODU_RFRM_MESSAGE_TYPE_PM,
                                                               0, 0, tsd_val, 0);
                }
            }
                                                                       
            for (i = 0; i < 256; i++)
            {
                PMC_ATOMIC_YIELD(coreotn_handle->base.parent_handle,0);
                if (PMC_SUCCESS == result)
                {
                    /* pick non-PM to clear for all pool ids */
                    result = odu_rfrm_pm_tcmi_tsd_tsf_proc_cfg(coreotn_handle->odu_rfrm_stg1_handle,
                                                               0,
                                                               ODU_RFRM_MESSAGE_TYPE_DCI,
                                                               i, 0, tsd_val, 0);
                }
            
                if (PMC_SUCCESS == result)
                {
                    /* pick non-PM to clear for all pool ids */
                    result = odu_rfrm_pm_tcmi_tsd_tsf_proc_cfg(coreotn_handle->odu_rfrm_stg2_handle,
                                                               0,
                                                               ODU_RFRM_MESSAGE_TYPE_DCI,
                                                               i, 0, tsd_val, 0);
                }
            
                if (PMC_SUCCESS == result)
                {
                    /* pick non-PM to clear for all pool ids */
                    result = odu_rfrm_pm_tcmi_tsd_tsf_proc_cfg(coreotn_handle->odu_rfrm_stg3a_handle,
                                                               0,
                                                               ODU_RFRM_MESSAGE_TYPE_DCI,
                                                               i, 0, tsd_val, 0);
                }
            
                if (PMC_SUCCESS == result)
                {
                    /* pick non-PM to clear for all pool ids */
                    result = odu_rfrm_pm_tcmi_tsd_tsf_proc_cfg(coreotn_handle->odu_rfrm_stgN_handle,
                                                               0,
                                                               ODU_RFRM_MESSAGE_TYPE_DCI,
                                                               i, 0, tsd_val, 0);
                }
            }
        }
        else {
            /* prepare for monitoring STG4 channel */
            for (i = 0; i < 96; i++)
            {
                PMC_ATOMIC_YIELD(coreotn_handle->base.parent_handle,0);
                if (PMC_SUCCESS == result)
                {
                    result = odu_rfrm_pm_tcmi_tsd_tsf_proc_cfg(coreotn_handle->odu_rfrm_stg4_handle,
                                                               i, ODU_RFRM_MESSAGE_TYPE_PM,
                                                               0, 0, tsd_val, 0);
                }
            }
        
            for (i = 0; i < 256; i++)
            {
                PMC_ATOMIC_YIELD(coreotn_handle->base.parent_handle,0);
                if (PMC_SUCCESS == result)
                {
                    /* pick non-PM to clear for all pool ids. pm_tcm_num instance
                    ** is a don't care in here 
                    */
                    result = odu_rfrm_pm_tcmi_tsd_tsf_proc_cfg(coreotn_handle->odu_rfrm_stg4_handle,
                                                               0, ODU_RFRM_MESSAGE_TYPE_DCI,
                                                               i, 0, tsd_val, 0);
                }
            }
        }
    }
        
    PMC_RETURN(result);

} /* odu_struct_ddeg_mode_set */

/*******************************************************************************
* odu_struct_tim_det_mon_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures TIM_DET_MO field in an ODU_STRUCT or the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi layer. See odu_struct_odu_frm_pm_tcm_id_t.
*   tim_det_mo          - TTI mismatch detection mode for the layer specified
*                         by 'pm_tcm_num'\n
*                         0 -- dTIM defect detection is disabled\n
*                         1 -- the expected SAPI field is used to determine dTIM
*                             defect\n
*                         2 -- the expected DAPI field is used to determine dTIM
*                             defect\n
*                         3 -- both the expected SAPI and DAPI fields are used
*                             to determine dTIM defect
*   target              - configuration target of odu struct data.
*                         See odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_tim_det_mon_set(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                            UINT8 tim_det_mo,
                                            odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT16 tcm_pool_index;
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    
    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES 
        || target > LAST_CONFIG_TARGET
        || tim_det_mo > 3)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    if (pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
    {
        odu_struct->mem_ptr->tim_mon_pm = tim_det_mo;
    }
    else {
        /* TCM */
        result = odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle, 
                                                           odu_struct,
                                                           pm_tcm_num, 
                                                           &tcm_pool_index);
    
        /* read from ODU struct */
        if (PMC_SUCCESS == result)
        {
            /* find tcm_pool_id_<tcm_pool_index> */
            odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].tim_mon = tim_det_mo;
        }
    }        
        
    if (PMC_SUCCESS == result && 
        target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
        
        if (NULL != local_odu_rfrm_handle)
        {
            result = odu_rfrm_pm_tcmi_tim_det_mode_cfg(local_odu_rfrm_handle,
                                                       odu_struct->switch_data.oduk_data.channel,
                                                       (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                                       tim_det_mo);
        }
        else {
            result = COREOTN_ERR_NULL_HANDLE;
        }
    }                                                     
    
    PMC_RETURN(result);

} /* odu_struct_tim_det_mon_set */

/*******************************************************************************
* odu_struct_tim_det_mon_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Retrieves TIM_DET_MO field of PM or TCMi layer from an ODU_STRUCT or
*   directly from the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi layer. See odu_struct_odu_frm_pm_tcm_id_t.
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   tim_det_mo          - retrieved TTI mismatch detection mode for PM or TCMi layer
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tim_det_mon_get(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                            odu_struct_ext_mode_t ext_mode,
                                            UINT8 *tim_det_mo)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    UINT16 tcm_pool_index;
    
    PMC_ENTRY();
    
    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES 
        || ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
        
        if (NULL != local_odu_rfrm_handle)
        {
            result = odu_rfrm_pm_tcmi_tim_det_mode_ext(local_odu_rfrm_handle,
                                                       odu_struct->switch_data.oduk_data.channel,
                                                       (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                                       tim_det_mo);
        }
        else {
            result = COREOTN_ERR_NULL_HANDLE;
        }
    }
    else 
    {
        if (pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
        {
            *tim_det_mo = odu_struct->mem_ptr->tim_mon_pm;
        }
        else 
        {
            /* TCM */
            result = odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle, 
                                                               odu_struct,
                                                               pm_tcm_num, 
                                                               &tcm_pool_index);
    
            /* read from ODU struct */
            if (PMC_SUCCESS == result)
            {
                /* find tcm_pool_id_<tcm_pool_index> */
                *tim_det_mo = odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].tim_mon;
            }
        }
    }        
    
    PMC_RETURN(result);
} /* odu_struct_tim_det_mon_get */

/*******************************************************************************
* odu_struct_tim_act_dis_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Configures dTIM consequential action field in an ODU_STRUCT.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi layer. See odu_struct_odu_frm_pm_tcm_id_t.
*   tim_act_dis         - controls TTI consequent action disable for the layer
*                         specified by 'pm_tcm_num'\n
*                         0 -- dTIM defect contributes to the aTSF and the aAIS
*                              consequential action declarations\n
*                         1 -- dTIM defect does not contribute to the aTSF and
*                              the aAIS consequential action declarations
*   target              - configuration target of odu struct data.
*                         see odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_tim_act_dis_set(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                            BOOL8 tim_act_dis,
                                            odu_struct_target_t target)
{
    UINT16 tcm_pool_index;
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    
    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES 
        || tim_act_dis > 1 || target > LAST_CONFIG_TARGET)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    if (pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
    {
        odu_struct->mem_ptr->tim_act_dis_pm = tim_act_dis;
    }
    else 
    {
        /* TCM */
        result = odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle, 
                                                           odu_struct,
                                                           pm_tcm_num, 
                                                           &tcm_pool_index);
        
        /* read from ODU struct */
        if (PMC_SUCCESS == result)
        {
            /* find tcm_pool_id_<tcm_pool_index> */
            odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].tim_act_dis = tim_act_dis;
        }
    }        
    
    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
        
        if (NULL != local_odu_rfrm_handle)
        {
            result = odu_rfrm_pm_tcmi_tim_dis_cfg(local_odu_rfrm_handle,
                                                  odu_struct->switch_data.oduk_data.channel,
                                                  (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                                  tim_act_dis);
        }
        else {
            result = COREOTN_ERR_NULL_HANDLE;
        }
    }                                                 
    
    PMC_RETURN(result);

} /* odu_struct_tim_act_dis_set */

/*******************************************************************************
* odu_struct_tim_act_dis_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Retrieves TIM_ACT_DIS field from an ODU_STRUCT or directly from the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi layer. See odu_struct_odu_frm_pm_tcm_id_t.
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   tim_act_dis         - retrieved value for TTI consequent action disable of
*                         the layer specified by 'pm_tcm_num'
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tim_act_dis_get(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                            odu_struct_ext_mode_t ext_mode,
                                            BOOL8 *tim_act_dis)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t * local_odu_rfrm_handle = NULL;
    UINT16 tcm_pool_index;
    
    PMC_ENTRY();
    
    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES 
        || ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
        
        if (NULL != local_odu_rfrm_handle)
        {
            result = odu_rfrm_pm_tcmi_tim_dis_get(local_odu_rfrm_handle,
                                                  odu_struct->switch_data.oduk_data.channel,
                                                  (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                                  tim_act_dis);
        }
        else {
            result = COREOTN_ERR_NULL_HANDLE;
        }
    }
    else 
    {
        if (pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
        {
            *tim_act_dis = odu_struct->mem_ptr->tim_act_dis_pm;
        }
        else {
            /* TCM */
            result = odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle, 
                                                               odu_struct,
                                                               pm_tcm_num, 
                                                               &tcm_pool_index);
    
            /* read from ODU struct */
            if (PMC_SUCCESS == result)
            {
                /* find tcm_pool_id_<tcm_pool_index> */
                *tim_act_dis = odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].tim_act_dis;
            }
        }
    }                                                                
    
    PMC_RETURN(result);
} /* odu_struct_tim_act_dis_get */

/*******************************************************************************
* odu_struct_bdi_bei_biae_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Configures BDI and/or BEI/BIAE field in an ODU_STRUCT or the device.
*   The overhead insertion source controls both BDI and BEI/BIAE.
*
*   For example, if BDI is selected to be inserted from RCP, this also means
*   the insertion source of BEI/BIAE field to RCP.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi instance. See odu_struct_odu_frm_pm_tcm_id_t.
*   oh_field            - Overhead field to be forced from register.
*                         Only valid if source is set to 1.
*                         0 -- Force BDI value \n
*                         1 -- Force BEI/BIAE value\n
*                         2 -- Force both BDI and BEI/BIAE
*   bdi_force_val       - For oh_field == 0 or 2 (otherwise set to 0)\n
*                         The value to be inserted into BDI field of
*                         PM or TCMi layer\n
*                         Valid range: 0 - 1
*   bei_biae_force_val  - For oh_field == 2 or 3 (otherwise set to 0)\n
*                         The value to be inserted into BEI/BIAE field of
*                         PM or TCMi layer\n
*                         Valid range: 0x0 - 0xF
*   target              - configuration target of odu struct data.
*                         see odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_bdi_bei_biae_set(coreotn_handle_t *coreotn_handle,
                                             odu_struct_t *odu_struct,
                                             odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                             UINT8 oh_field,
                                             UINT8 bdi_force_val,
                                             UINT8 bei_biae_force_val,
                                             odu_struct_target_t target)
{
    UINT16 tcm_pool_index;
    PMC_ERROR ret_val = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES
        || oh_field > 2 || bdi_force_val > 1
        || bei_biae_force_val > 0xF
        || target > LAST_CONFIG_TARGET)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

    if (NULL == local_odu_tfrm_handle) {
        PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
    }                                                      


    if (pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
    {
        /* PM configuration */
            if (oh_field == 0)
            {
                odu_struct->mem_ptr->bdi_force_val_pm = bdi_force_val;
            }
            else if (oh_field == 1)
            {
                odu_struct->mem_ptr->bei_biae_force_val_pm = bei_biae_force_val;
            }
            else
            {
                odu_struct->mem_ptr->bei_biae_force_val_pm = bei_biae_force_val;
                odu_struct->mem_ptr->bdi_force_val_pm = bdi_force_val;
            }
    }
    else
    {
        /* Pool based resource configuration */                                              
        ret_val = odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle,
                                                            odu_struct,
                                                            pm_tcm_num,
                                                            &tcm_pool_index);
                                                                    
        if (PMC_SUCCESS == ret_val)
        {
            /* find tcm_pool_id_<tcm_pool_index> */
            /* BDI insertion */
            if (oh_field != 1)
            {
                odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].bdi_force_val = bdi_force_val;
            }

            /* BEI/BIAE insertion */
            if (oh_field != 0)
            {
                odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].bei_biae_force_val = bei_biae_force_val;
            }
        }
    }

    if (PMC_SUCCESS == ret_val && target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        if (PMC_SUCCESS == ret_val && (oh_field == 1 || oh_field == 2))
        {
            /* force BEI/BIAE */
            ret_val = odu_tfrm_pm_tcmi_bdi_bei_biae_cfg(local_odu_tfrm_handle,
                                                        odu_struct->switch_data.oduk_data.channel,
                                                        (odu_tfrm_pm_tcm_id_t)pm_tcm_num,
                                                        1, bei_biae_force_val);
        }

        /* force BDI */
        if (PMC_SUCCESS == ret_val && (oh_field ==0 || oh_field == 2))
        {
            ret_val = odu_tfrm_pm_tcmi_bdi_bei_biae_cfg(local_odu_tfrm_handle,
                                                        odu_struct->switch_data.oduk_data.channel,
                                                        (odu_tfrm_pm_tcm_id_t)pm_tcm_num,
                                                        0, bdi_force_val);
        }
    }

    PMC_RETURN(ret_val);

} /* odu_struct_bdi_bei_biae_set */

/*******************************************************************************
* odu_struct_rcp_en_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Retrieves status of bdi_bei_biae insert for an ODU framer channel.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on                          
*
* OUTPUTS:
*   *rcp_en        - status of bdi_bei_biae insert.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rcp_en_get(coreotn_handle_t *coreotn_handle,
                                       odu_struct_t *odu_struct,
                                       UINT32 *rcp_en)
{
    PMC_ERROR result = COREOTN_ERR_NULL_HANDLE;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
 
    PMC_ENTRY();   
    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);
    
    local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
    
    if (NULL != local_odu_tfrm_handle)
    {
        result = odu_tfrm_pm_tcmi_bdi_bei_biae_src_get(local_odu_tfrm_handle,
                                                       odu_struct->switch_data.oduk_data.channel,
                                                       rcp_en);
    }
   
    PMC_RETURN(result);    
} /* odu_struct_rcp_en_get */


/*******************************************************************************
* odu_struct_bei_biae_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Retrieves BEI/BIAE field from an ODU_STRUCT or directly from the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi instance. See odu_struct_odu_frm_pm_tcm_id_t.
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   bei_biae_val        - value read from BEI/BIAE field of the specified PM or
*                         TCMi layer in an ODU struct
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_bei_biae_get(coreotn_handle_t *coreotn_handle,
                                         odu_struct_t *odu_struct,
                                         odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                         odu_struct_ext_mode_t ext_mode,
                                         UINT8 *bei_biae_val)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT16 tcm_pool_index;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES
        || ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }


    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

        if (NULL != local_odu_tfrm_handle)
        {
            result = odu_tfrm_pm_tcmi_bei_biae_get(local_odu_tfrm_handle,
                                                   odu_struct->switch_data.oduk_data.channel,
                                                   (odu_tfrm_pm_tcm_id_t)pm_tcm_num,
                                                   bei_biae_val);
        }
        else {
            PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
        }
    }
    else 
    {
        if (pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
        {
            *bei_biae_val = odu_struct->mem_ptr->bei_biae_force_val_pm;
        }
        else {
            result = odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle,
                                                               odu_struct,
                                                               pm_tcm_num,
                                                               &tcm_pool_index);

            if (PMC_SUCCESS == result)
            {        
                *bei_biae_val = odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].bei_biae_force_val;
            }
        }
    }

    PMC_RETURN(result);
} /* odu_struct_bei_biae_get */

/*******************************************************************************
* odu_struct_bdi_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Retrieves BDI field from an ODU_STRUCT or directly from the device.
*
* INPUTS:
*   *coreotn_handle - pointer to COREOTN handle
*   *odu_struct     - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num      - PM or TCMi instance. See odu_struct_odu_frm_pm_tcm_id_t.
*   bdi_val         - pointer to storage of BDI field of PM or TCMi layer
*                     read from ODU struct
*   ext_mode        - extraction mode of the requested field.
*                     See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   bdi_val         - BDI value retrieved.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_bdi_get(coreotn_handle_t *coreotn_handle,
                                    odu_struct_t *odu_struct,
                                    odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                    odu_struct_ext_mode_t ext_mode,
                                    UINT8 *bdi_val)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
    UINT16 tcm_pool_index;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES
        || ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

        if (NULL != local_odu_tfrm_handle)
        {
            result = odu_tfrm_pm_tcmi_bdi_get(local_odu_tfrm_handle,
                                              odu_struct->switch_data.oduk_data.channel,
                                              (odu_tfrm_pm_tcm_id_t)pm_tcm_num,
                                              bdi_val);
        }
        else {
            PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
        }
    }
    else 
    {
        if (pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
        {
            *bdi_val = odu_struct->mem_ptr->bdi_force_val_pm;
        }
        else 
        {
            result = odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle,
                                                               odu_struct,
                                                               pm_tcm_num,
                                                               &tcm_pool_index);

            if (PMC_SUCCESS == result)
            {                   
                *bdi_val = odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].bdi_force_val;
            }
        }
    }

    PMC_RETURN(result);
} /* odu_struct_bdi_get */

/*******************************************************************************
* odu_struct_stat_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Configures the STAT field and the forced value in an ODU_STRUCT
*   to be used in case of register insertion of STAT into ODU framer.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi instance. See odu_struct_odu_frm_pm_tcm_id_t.
*   force_val           - For source == 1 (otherwise set to 0)\n
*                         The value to be inserted into STAT field of
*                         PM or TCMi layer\n
*                         Valid range: 0x0 - 0x7
*   target              - configuration target of odu struct data.
*                         see odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_stat_set(coreotn_handle_t *coreotn_handle,
                                     odu_struct_t *odu_struct,
                                     odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                     UINT8 force_val,
                                     odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;

    UINT16 tcm_pool_index;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES
        || target > LAST_CONFIG_TARGET || force_val > 7)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
    {
        /* PM configuration */
        odu_struct->mem_ptr->stat_force_val_pm = force_val;
    }
    else
    {
        /* Pool based resource configuration */
        result = odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle,
                                                           odu_struct,
                                                           pm_tcm_num,
                                                           &tcm_pool_index);

        if (PMC_SUCCESS == result)
        {
            /* find tcm_pool_id_<tcm_pool_index> */  
            odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].stat_force_val = force_val;   
        }
    }

    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        /* write to the device */
        local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

        if (NULL != local_odu_tfrm_handle)
        {
            result = odu_tfrm_pm_tcmi_stat_src_cfg(local_odu_tfrm_handle,
                                                   odu_struct->switch_data.oduk_data.channel,
                                                   (odu_tfrm_pm_tcm_id_t)pm_tcm_num,
                                                   force_val);
        }
        else {
            PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
        }
    }

    PMC_RETURN(PMC_SUCCESS);

} /* odu_struct_stat_set */

/*******************************************************************************
* odu_struct_stat_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Retrieves the STAT field of PM or TCMI layer from an ODU_STRUCT or directly
*   from the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   direction           - direction of the traffic stream to extract STAT field.
*                         See odu_struct_stream_dir_t.
*                         0 -- RX (sink)
*                         1 -- TX (source)
*   pm_tcm_num          - PM or TCMi instance. See odu_struct_odu_frm_pm_tcm_id_t.
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   stat_val            - retrieved STAT field value.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_stat_get(coreotn_handle_t *coreotn_handle,
                                     odu_struct_t *odu_struct,
                                     odu_struct_stream_dir_t direction,
                                     odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                     odu_struct_ext_mode_t ext_mode,
                                     UINT8 *stat_val)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    UINT16 tcm_pool_index;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES
        || ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        if (direction == 1)
        {
            local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
            
            if (NULL != local_odu_tfrm_handle)
            {
                result = odu_tfrm_pm_tcmi_stat_get(local_odu_tfrm_handle,
                                                   odu_struct->switch_data.oduk_data.channel,
                                                   (odu_tfrm_pm_tcm_id_t)pm_tcm_num,
                                                   stat_val);
            }
            else {
                result = COREOTN_ERR_NULL_HANDLE;
            }  
        }
        else {
           local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
            
            if (NULL != local_odu_rfrm_handle)
            {
                result = odu_rfrm_acpt_stat_ext(local_odu_rfrm_handle,
                                                odu_struct->switch_data.oduk_data.channel, 
                                                (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                                stat_val);
            }
            else {
                result = COREOTN_ERR_NULL_HANDLE;
            }
        }                                   
    }
    else 
    {
        if (pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
        {
            /* PM configuration */
            *stat_val = odu_struct->mem_ptr->stat_force_val_pm;
        }
        else
        {
            if (direction == 1)
            {
                /* Pool based resource configuration */
                result = odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle,
                                                                   odu_struct,
                                                                   pm_tcm_num,
                                                                   &tcm_pool_index);
                
                if (PMC_SUCCESS == result)
                {
                    /* find tcm_pool_id_<tcm_pool_index>   */  
                    *stat_val = odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].stat_force_val;
                }  
            }
            else {
                PMC_RETURN(COREOTN_ERR_INVALID_ARG);                   
            }
        }
    }

    PMC_RETURN(result);
} /* odu_struct_stat_get */


/*******************************************************************************
* odu_struct_all_tcm_iae_force
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Forces an IAE event at all operational TCMs for this channel (inserts 010
*   into STAT field for 16 multiframes) 
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
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
PUBLIC PMC_ERROR odu_struct_all_tcm_iae_force(coreotn_handle_t *coreotn_handle,
                                              odu_struct_t *odu_struct)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);


    local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

    if (NULL != local_odu_tfrm_handle)
    {
        result = odu_trfm_all_tcm_iae_force(local_odu_tfrm_handle,
                                            odu_struct->switch_data.oduk_data.channel);
    }
    else {
        PMC_RETURN(COREOTN_ERR_NULL_HANDLE);
    }

    PMC_RETURN(result);

} /* odu_struct_all_tcm_iae_force */


/*******************************************************************************
* odu_struct_delay_measure_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the source of STAT field and the forced value in an ODU_STRUCT
*   to be used in case of register insertion of STAT into ODU framer.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi instance. See odu_struct_odu_frm_pm_tcm_id_t.
*   dm_mode             - See digi_otn_dm_mode_t
*                         ODU_STRUCT_DM_SOURCE (use this for provisioning
*                         a delay measurement source, or for forcing the
*                         dmx_val).
*                         ODU_STRUCT_DM_FEEDTHROUGH (use this for provisioning
*                         a delay measurement "feedthrough" device.
*   target              - configuration target of odu struct data.
*                         See odu_struct_target_t
*   dm_val              - If dm_mode == ODU_STRUCT_DM_SOURCE, this is that
*                         initial value to force in the dm overhead
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
PUBLIC PMC_ERROR odu_struct_delay_measure_init(coreotn_handle_t *coreotn_handle,
                                               odu_struct_t *odu_struct,
                                               odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                               odu_struct_dm_mode_t dm_mode,
                                               odu_struct_target_t target,
                                               UINT32 dm_val)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
    UINT16 tcm_pool_index;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES
        || dm_val > 1
        || target > LAST_CONFIG_TARGET)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    /* how do we keep the order of TCM layers the same as configured in the channel
    ** where do we read the order and how do we index it...
    **  */
    if (pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
    {
        /* PM configuration */
         odu_struct->mem_ptr->dmp_src = (dm_mode == ODU_STRUCT_DM_FEEDTHROUGH)? 0:1;
         odu_struct->mem_ptr->dmp_val = (dm_mode == ODU_STRUCT_DM_FEEDTHROUGH)? 0:dm_val;
    }
    else
    {
        /* Pool based resource configuration */
        ret_val = odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle,
                                                            odu_struct,
                                                            pm_tcm_num,
                                                            &tcm_pool_index);

        if (PMC_SUCCESS == ret_val)
        {
            /* find tcm_pool_id_<tcm_pool_index> */
            odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].dmt_src = (dm_mode == ODU_STRUCT_DM_FEEDTHROUGH)? 0:1;
            odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].dmt_val = (dm_mode == ODU_STRUCT_DM_FEEDTHROUGH)? 0:dm_val; 
        }            
    }

    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

        if (NULL != local_odu_tfrm_handle)
        {
            if (dm_mode == ODU_STRUCT_DM_FEEDTHROUGH) 
            {
                ret_val = odu_tfrm_pm_tcmi_delay_measure_init(local_odu_tfrm_handle,
                                                        odu_struct->switch_data.oduk_data.channel,
                                                        (odu_tfrm_pm_tcm_id_t)pm_tcm_num,
                                                        0,
                                                        dm_val);
            } 
            else 
            {
                ret_val = odu_tfrm_pm_tcmi_delay_measure_init(local_odu_tfrm_handle,
                                                        odu_struct->switch_data.oduk_data.channel,
                                                        (odu_tfrm_pm_tcm_id_t)pm_tcm_num,
                                                        1,
                                                        dm_val);
            }
        }
        else {
            ret_val = COREOTN_ERR_NULL_HANDLE;
        }            
    }

    PMC_RETURN(ret_val);

} /* odu_struct_delay_measure_init */

/*******************************************************************************
* odu_struct_delay_measure_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get delay measurement mode and its initial value.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi instance. See odu_struct_odu_frm_pm_tcm_id_t.
*
* OUTPUTS:
*   dm_mode             - see odu_struct_dm_mode_t.
*                         ODU_STRUCT_DM_SOURCE or ODU_STRUCT_DM_FEEDTHROUGH.
*   dm_val              - If *dm_mode == ODU_STRUCT_DM_SOURCE, dmx_val returned.
*                         otherwise 0 returned.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_delay_measure_mode_get(coreotn_handle_t *coreotn_handle,
                                                   odu_struct_t *odu_struct,
                                                   odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                   odu_struct_dm_mode_t *dm_mode,
                                                   UINT32 *dm_val)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
    UINT16 tcm_pool_index;
    UINT32 enable;
    UINT32 value;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle != NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

    if (NULL != local_odu_tfrm_handle)
    {
        ret_val = odu_tfrm_pm_tcmi_delay_measure_mode_get(local_odu_tfrm_handle,
                                                          odu_struct->switch_data.oduk_data.channel,
                                                          (odu_tfrm_pm_tcm_id_t)pm_tcm_num,
                                                          &enable,
                                                          &value);
        if(ret_val == PMC_SUCCESS)
        {
            if(enable)
            {
                *dm_mode = ODU_STRUCT_DM_SOURCE;
                *dm_val = value;
            }
            else
            {
                *dm_mode = ODU_STRUCT_DM_FEEDTHROUGH;
                *dm_val = 0;
            }
        }
    }
    else 
    {
        ret_val = COREOTN_ERR_NULL_HANDLE;
    }            

    PMC_RETURN(ret_val);

} /* odu_struct_delay_measure_mode_get */

/*******************************************************************************
* odu_struct_pm_tcmi_delay_measure_go
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function triggers the delay measurement on a desired layer.   
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - the PM/TCM to perform the delay measurement
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_pm_tcmi_delay_measure_go(coreotn_handle_t *coreotn_handle,
                                                     odu_struct_t *odu_struct,
                                                     odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
    PMC_ENTRY();
    
    local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
    
    if (NULL != local_odu_tfrm_handle)
    {
        result = odu_tfrm_pm_tcmi_delay_measure_go(local_odu_tfrm_handle,
                                                   odu_struct->switch_data.oduk_data.channel,
                                                   (odu_tfrm_pm_tcm_id_t)pm_tcm_num);
    }
    else {
        result = COREOTN_ERR_NULL_HANDLE;   
    }                                                        
    
    PMC_RETURN(result);
} /* odu_struct_pm_tcmi_delay_measure_go */                                                      



/*******************************************************************************
* odu_struct_pm_tcmi_get_delay_measure_count
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Accesses the delay measurement count from the ODU TFRM after a delay
*   measurement has completed.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - the PM/TCM to perform the delay measurement on
*   block_mode          - blocking mode. 
*                         0 for no-blocking
*                         1 for blocking
*   
* OUTPUTS:
*   dm_count            - Returned count of the delay measurement
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_pm_tcmi_get_delay_measure_count(coreotn_handle_t *coreotn_handle,
                                                            odu_struct_t *odu_struct,
                                                            odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                            UINT32 block_mode,
                                                            UINT32 *dm_count)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
    PMC_ENTRY();
    
    local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
    
    if (NULL != local_odu_tfrm_handle)
    {
        result = odu_tfrm_pm_tcmi_delay_measure_get_count(local_odu_tfrm_handle,
                                                          odu_struct->switch_data.oduk_data.channel,
                                                          (odu_tfrm_pm_tcm_id_t)pm_tcm_num,
                                                          block_mode,
                                                          dm_count);
    }
    else {
        result = COREOTN_ERR_NULL_HANDLE;   
    }                                                        
    
    PMC_RETURN(result);
} /* odu_struct_pm_tcmi_get_delay_measure_count */       


/*******************************************************************************
* odu_struct_rx_expected_msi_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Configures multiplex structure identifier for tributary slots used by
*   the ODU channel in an ODU_STRUCT.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   ts_port             - tributary port of the HO channel whose MSI value
*                         to be changed
*                         Valid range:
*                          HO ODU1 with 1G25 TS: 0 -- 1
*                          HO ODU1 with 2G5 TS : 0
*                          HO ODU2 with 1G25 TS: 0 -- 7
*                          HO ODU3 with 2G5 TS : 0 -- 15
*                          HO ODU4 with 1G25 TS: 0 -- 79
*   msi_byte            - expected MSI byte of this LO channel
*   target              - configuration target of odu struct data.
*                         See odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_rx_expected_msi_set(coreotn_handle_t *coreotn_handle,
                                                odu_struct_t *odu_struct,
                                                UINT8 ts_port,
                                                UINT8 msi_byte,
                                                odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *local_odtu_dmx_ptr = NULL;
    UINT32 i;
    UINT32 local_ho_chnl_id;
    /* for each tributary port, maximum two calendar entires are possible */
    UINT32 cal_entry_ptr[2];
    UINT32 num_cal_entries = 0;
    UINT32 num_tribslot = 0;
    UINT32 trib_port;
    
    PMC_ENTRY();

    PMC_ASSERT(odu_struct->mem_ptr->payload_format != UTIL_GLOBAL_ODU_UNCHANNELIZED, COREOTN_ERR_ODU_STRUCT_OPU, 0, 0);

    /* argument checking */
    if (target > LAST_CONFIG_TARGET)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    /* check if the ODU type is valid as a HO or MO */       
    if ((odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_4_ODU 
        && odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC)
        && (odu_struct->mem_ptr->odu_type == UTIL_GLOBAL_ODU0
            || odu_struct->mem_ptr->odu_type == UTIL_GLOBAL_ODUFLEX_CBR  
            || odu_struct->mem_ptr->odu_type == UTIL_GLOBAL_ODUFLEX_GFP
            || odu_struct->mem_ptr->odu_type > UTIL_GLOBAL_ODU2E_ETRANS))
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
     
    /* test if the ts_port is within the valid range */
    result = util_global_max_server_num_ts_get(UTIL_GLOBAL_MUX_DEMUX,
                                               odu_struct->mem_ptr->odu_type,
                                               odu_struct->mem_ptr->payload_format,
                                               &num_tribslot);
   
     
    local_odtu_dmx_ptr = odu_struct_dmx_handle_get(coreotn_handle,
                                                   odu_struct);

    if (PMC_SUCCESS == result)
    {
        if (ts_port >= num_tribslot)
        {
            result = COREOTN_ERR_INVALID_ARG;
        }
    }    
                                              
    /* find corresponding calendar entries that belong to the tributary port */
    if (PMC_SUCCESS == result)
    {
        result = coreotn_db_ts_port_cal_entry_get(coreotn_handle, 1, 
                                                  odu_struct->mem_ptr->odu_level,
                                                  odu_struct->switch_data.oduk_data.channel,
                                                  ts_port,
                                                  cal_entry_ptr,
                                                  &num_cal_entries);
    }   
    
    if (num_cal_entries == 0)
    {
        PMC_RETURN(COREOTN_ERR_TRIB_SLOT_NOT_IN_USE);
    }                                               

    if (PMC_SUCCESS == result)
    {
        if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
        {

            local_ho_chnl_id = odu_struct->switch_data.oduk_data.channel;
              
            for (i = 0; i < num_cal_entries; i++)
            {
                result = ohfs_remove_exp_msi_set(local_odtu_dmx_ptr->rgmp_handle->ohfs_remove_handle,
                                                 cal_entry_ptr[i],
                                                 msi_byte);
            }                                      
        }
    }
    
    if (odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_4_ODU 
        && odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC)
    {
        /* convert the MSI value to extract tributary port number for this tributary slot */
        if (odu_struct->mem_ptr->odu_type == UTIL_GLOBAL_ODU4 
            || odu_struct->mem_ptr->odu_type == UTIL_GLOBAL_ODU3E2)
        {
            trib_port = msi_byte & 0x7F;
        }
        else {
            trib_port = msi_byte & 0x3F;
        }
        
        if (PMC_SUCCESS == result)
        {
            /* update ODU_struct content */
            odu_struct->mem_ptr->trib_slot_ctxt.rx_trib_port[ts_port] = trib_port;
        }
    }

    PMC_RETURN(result);

} /* odu_struct_rx_expected_msi_set */

/*******************************************************************************
* odu_struct_rx_expected_msi_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Retrieves expected multiplex structure identifier for tributary slots used by
*   the ODU channel in an ODU_STRUCT.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   ext_mode            - extraction mode of odu struct data.
*                         See odu_struct_ext_mode_t
*
* OUTPUTS:
*   *msi_byte           - Array of MSI values for the HO.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_expected_msi_get(coreotn_handle_t *coreotn_handle,
                                                odu_struct_t *odu_struct,
                                                odu_struct_ext_mode_t ext_mode,
                                                UINT8 msi_byte[UTIL_GLOBAL_MAX_NUM_TS])
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    UINT32 num_active_ts_port = 0;
    UINT32 num_cal_entries = 0;
    UINT32 ts_port_ptr[UTIL_GLOBAL_MAX_NUM_TS];
    UINT32 cal_entry_ptr[UTIL_GLOBAL_MAX_NUM_TS];
    odtu_dmx_handle_t *local_odtu_dmx_ptr;
    UINT8 msi;

    PMC_ENTRY();

    PMC_ASSERT(odu_struct->mem_ptr->payload_format != UTIL_GLOBAL_ODU_UNCHANNELIZED, COREOTN_ERR_ODU_STRUCT_OPU, 0, 0);

   
    /* argument checking */
    if (ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
   
    local_odtu_dmx_ptr = odu_struct_dmx_handle_get(coreotn_handle,
                                                   odu_struct);
    
    for (i = 0; i < UTIL_GLOBAL_MAX_NUM_TS; i++)
    {
        ts_port_ptr[i] = 0;
        cal_entry_ptr[i] = 0;
        msi_byte[i] = 0;
    }
    
    if (PMC_SUCCESS == result)
    {                                          
        result = coreotn_db_cal_entry_to_ts_port_get(coreotn_handle,
                                                     1, /*RX DB*/
                                                     odu_struct->mem_ptr->odu_level,
                                                     odu_struct->switch_data.oduk_data.channel,
                                                     cal_entry_ptr,
                                                     ts_port_ptr,
                                                     &num_cal_entries,
                                                     &num_active_ts_port);
    }
                                                             
    if (num_cal_entries == 0)
    {
        PMC_RETURN(COREOTN_ERR_TRIB_SLOT_NOT_IN_USE);


    }  
    
    if(PMC_SUCCESS == result)
    {
        for(i = 0; i < num_cal_entries; i++)
        {
            result = ohfs_remove_exp_msi_get(local_odtu_dmx_ptr->rgmp_handle->ohfs_remove_handle,
                                             cal_entry_ptr[i],
                                             &msi);
            if (ts_port_ptr[i] != 0xFF)
            {
                msi_byte[ts_port_ptr[i]] = msi;
            }
        }
    }
            
    PMC_RETURN(result);
} /* odu_struct_rx_expected_msi_get */

/*******************************************************************************
* odu_struct_rx_expected_msi_per_ts_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Retrieves expected multiplex structure identifier for the specified 
*   tributary slot within the ODU channel in an ODU_STRUCT.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   ext_mode            - extraction mode of odu struct data.
*                         See odu_struct_ext_mode_t
*   ts_port             - tributary port of a HO or MO channel whose MSI value
*                         is to be extracted
*
* OUTPUTS:
*   *msi_byte           - MSI value.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_expected_msi_per_ts_get(coreotn_handle_t *coreotn_handle,
                                                       odu_struct_t *odu_struct,
                                                       odu_struct_ext_mode_t ext_mode,
                                                       UINT8 ts_port,
                                                       UINT8 *msi_byte)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    UINT32 num_tribslot = 0;
    UINT32 cal_entry_ptr[2];
    UINT32 num_cal_entries = 0;
    odtu_dmx_handle_t *local_odtu_dmx_ptr;
    
    PMC_ENTRY();

    PMC_ASSERT(odu_struct->mem_ptr->payload_format != UTIL_GLOBAL_ODU_UNCHANNELIZED, COREOTN_ERR_ODU_STRUCT_OPU, 0, 0);


    /* argument checking */
    if (ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    /* check if the ODU type is valid as a HO or MO */
    if ((odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_4_ODU 
         && odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC)
        && (odu_struct->mem_ptr->odu_type == UTIL_GLOBAL_ODU0
            || odu_struct->mem_ptr->odu_type == UTIL_GLOBAL_ODUFLEX_CBR  
            || odu_struct->mem_ptr->odu_type == UTIL_GLOBAL_ODUFLEX_GFP
            || odu_struct->mem_ptr->odu_type > UTIL_GLOBAL_ODU2E_ETRANS))
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
     
     
    local_odtu_dmx_ptr = odu_struct_dmx_handle_get(coreotn_handle,
                                                   odu_struct);


    /* test if the ts_port is within the valid range */
    result = util_global_max_server_num_ts_get(UTIL_GLOBAL_MUX_DEMUX,
                                               odu_struct->mem_ptr->odu_type,
                                               odu_struct->mem_ptr->payload_format,
                                               &num_tribslot);
    
    if (PMC_SUCCESS == result)
    {
        if (ts_port >= num_tribslot)
        {
            result = COREOTN_ERR_INVALID_ARG;
        }
    }    
    
    if (PMC_SUCCESS == result)
    {
        /* initialize cal_entry_ptr array */
        for (i = 0; i < 2; i++)
        {
            cal_entry_ptr[i] = 0x7F;
        }
        
        /* find corresponding calendar entries that belong to the tributary port */
        result = coreotn_db_ts_port_cal_entry_get(coreotn_handle, 
                                                  1, /* RX DB */ 
                                                  odu_struct->mem_ptr->odu_level,
                                                  odu_struct->switch_data.oduk_data.channel,
                                                  ts_port,
                                                  cal_entry_ptr,
                                                  &num_cal_entries);
    }
    
    if (PMC_SUCCESS == result)
    {

        result = ohfs_remove_exp_msi_get(local_odtu_dmx_ptr->rgmp_handle->ohfs_remove_handle,
                                         cal_entry_ptr[0],
                                         msi_byte);

    }
            
    PMC_RETURN(result);
} /* odu_struct_rx_expected_msi_per_ts_get */


/*******************************************************************************
* odu_struct_tx_msi_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Configures multiplex structure identifier for tributary slots used by
*   the ODU channel in an ODU_STRUCT.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   ts_num              - tributary slot number of the HO channel whose MSI value
*                         to be changed
*                         Valid range:
*                          HO ODU1 with 1G25 TS: 0 -- 1
*                          HO ODU1 with 2G5 TS : 0
*                          HO ODU2 with 1G25 TS: 0 -- 7
*                          HO ODU3 with 2G5 TS : 0 -- 15
*                          HO ODU4 with 1G25 TS: 0 -- 79
*   msi_byte            - MSI value for the selected 'ts_num'.
*   target              - configuration target of odu struct data.
*                         See odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_tx_msi_set(coreotn_handle_t *coreotn_handle,
                                       odu_struct_t *odu_struct,
                                       UINT8 ts_num,
                                       UINT8 msi_byte,
                                       odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;
    ohfs_insert_handle_t *local_ohfs_insert_ptr = NULL;
    expand_ibuf_handle_t *local_expand_ibuf_ptr = NULL;
    odtu_mux_handle_t *local_odtu_mux_ptr = NULL;
    UINT32 i;
    UINT32 local_ho_chnl_id;
    /* for each tributary port, maximum two calendar entires are possible */
    UINT32 cal_entry_ptr[2];
    UINT32 num_cal_entries = 0;
    UINT32 num_tribslot = 0; 
    UINT32 trib_port;
       
    PMC_ENTRY();

    PMC_ASSERT(odu_struct->mem_ptr->payload_format != UTIL_GLOBAL_ODU_UNCHANNELIZED, COREOTN_ERR_ODU_STRUCT_OPU, 0, 0);

    local_odtu_mux_ptr = odu_struct_mux_handle_get(coreotn_handle,
                                                   odu_struct);

    /* argument checking */
    if (target > LAST_CONFIG_TARGET)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    /* check if the ODU type is valid as a HO or MO */
    if ((odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_4_ODU 
        && odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC)
        && (odu_struct->mem_ptr->odu_type == UTIL_GLOBAL_ODU0
            || odu_struct->mem_ptr->odu_type == UTIL_GLOBAL_ODUFLEX_CBR  
            || odu_struct->mem_ptr->odu_type == UTIL_GLOBAL_ODUFLEX_GFP
            || odu_struct->mem_ptr->odu_type > UTIL_GLOBAL_ODU2E_ETRANS))
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
     
    /* test if the ts_port is within the valid range */
    result = util_global_max_server_num_ts_get(UTIL_GLOBAL_MUX_DEMUX,
                                               odu_struct->mem_ptr->odu_type,
                                               odu_struct->mem_ptr->payload_format,
                                               &num_tribslot);
    
    if (PMC_SUCCESS == result)
    {
        if (ts_num >= num_tribslot)
        {
            result = COREOTN_ERR_INVALID_ARG;
        }
    }   

    /* if this API is called then msi_mode is now flexible */
    odu_struct->mem_ptr->msi_mode                = 1;

    /* initialize cal_entry_ptr array */
    for (i = 0; i < 2; i++)
    {
        cal_entry_ptr[i] = 0x7F;
    }
               
    if (PMC_SUCCESS == result)
    {                                             
        /* find corresponding calendar entries that belong to the tributary port */
        result = coreotn_db_ts_port_cal_entry_get(coreotn_handle, 0, 
                                                  odu_struct->mem_ptr->odu_level,
                                                  odu_struct->switch_data.oduk_data.channel,
                                                  ts_num,
                                                  cal_entry_ptr,
                                                  &num_cal_entries);
    }
    
    if (num_cal_entries == 0)
    {
        PMC_RETURN(COREOTN_ERR_TRIB_SLOT_NOT_IN_USE);
    }

    if (PMC_SUCCESS == result)
    {
        if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
        {

            local_ho_chnl_id = odu_struct->switch_data.oduk_data.channel;
            local_ohfs_insert_ptr = local_odtu_mux_ptr->tgmp_handle->ohfs_insert_handle;
            local_expand_ibuf_ptr = local_odtu_mux_ptr->tgmp_handle->expand_ibuf_handle;
            
            /* HO MEM ID */  
            if (PMC_SUCCESS == result)
            {
                result = expand_ibuf_mem_ho_id_cfg(local_expand_ibuf_ptr,
                                                   local_ho_chnl_id);
            }

            if (PMC_SUCCESS == result && local_ohfs_insert_ptr != NULL)
            {
                /* write to transmit MSI directly 
                ** this will write to the offline page
                */ 
                
                for (i = 0; i < num_cal_entries; i++)
                {
                    result = ohfs_insert_msi_set(local_ohfs_insert_ptr, cal_entry_ptr[i], msi_byte);             
                }
                
                

                if (coreotn_handle->var.prov_mode != COREOTN_ODU_CHNL_PROV_ALL)
                {
                    /* flip the page so that we update both A/B pages for the new 
                    **  MSI byte
                    */
                    if (PMC_SUCCESS == result)
                    {
                        result = odtu_mux_config_update_set(local_odtu_mux_ptr,
                                                            local_ho_chnl_id, 1);
                    }
                    
                    if (PMC_SUCCESS == result)
                    {
                        odtu_mux_config_update_poll(local_odtu_mux_ptr, 
                                                    local_ho_chnl_id, 
                                                    20000);
                    }
                    
                    PMC_Q_USLEEP(local_odtu_mux_ptr,120);
                    
                    /* write to the online page */ 
                    for (i = 0; i < num_cal_entries; i++)
                    {
                        if (PMC_SUCCESS == result)
                        {
                            result = ohfs_insert_msi_set(local_ohfs_insert_ptr, cal_entry_ptr[i], msi_byte);
                        }
                    } 
                }
                                                                                                                                   
            }
            else {
                result = COREOTN_ERR_NULL_HANDLE;
            }
        }
    }

    /* convert the MSI value to extract tributary port number for this tributary slot */
    if (odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_4_ODU 
        && odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC)
    {
        if (odu_struct->mem_ptr->odu_type == UTIL_GLOBAL_ODU4 
            || odu_struct->mem_ptr->odu_type == UTIL_GLOBAL_ODU3E2)
        {
            trib_port = msi_byte & 0x7F;
        }
        else {
            trib_port = msi_byte & 0x3F;
        }
        
        if (PMC_SUCCESS == result)
        {
            /* update ODU_struct content */
            odu_struct->mem_ptr->trib_slot_ctxt.tx_trib_port[ts_num] = trib_port;
        }
    }

    PMC_RETURN(result);

} /* odu_struct_tx_msi_set */

/*******************************************************************************
* odu_struct_tx_msi_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Retrieves multiplex structure identifier for tributary slots used by
*   the ODU channel in an ODU_STRUCT.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t
*
* OUTPUTS:
*   *msi_byte           - MSI value.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tx_msi_get(coreotn_handle_t *coreotn_handle,
                                       odu_struct_t *odu_struct,
                                       odu_struct_ext_mode_t ext_mode,
                                       UINT8 *msi_byte)
{
    PMC_ERROR result = PMC_SUCCESS;
    ohfs_insert_handle_t *local_ohfs_insert_ptr = NULL;
    UINT32 i;
    UINT8 msi;
    UINT32 num_active_ts_port = 0;
    UINT32 num_cal_entries = 0;
    UINT32 ts_port_ptr[UTIL_GLOBAL_MAX_NUM_TS];
    UINT32 cal_entry_ptr[UTIL_GLOBAL_MAX_NUM_TS];
    odtu_mux_handle_t* odtu_mux_ptr;
    
    PMC_ENTRY();

    /* argument checking */
    if (ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    for (i = 0; i < UTIL_GLOBAL_MAX_NUM_TS; i++)
    {
        ts_port_ptr[i] = 0;
        cal_entry_ptr[i] = 0;
    }

    
    odtu_mux_ptr = odu_struct_mux_handle_get(coreotn_handle,
                                             odu_struct);

    local_ohfs_insert_ptr = odtu_mux_ptr->tgmp_handle->ohfs_insert_handle;

    /* find corresponding calendar entries that belong to the tributary port */
    if (PMC_SUCCESS == result)
    {                                          
        result = coreotn_db_cal_entry_to_ts_port_get(coreotn_handle, 0,
                                                     odu_struct->mem_ptr->odu_level,
                                                     odu_struct->switch_data.oduk_data.channel,
                                                     cal_entry_ptr,
                                                     ts_port_ptr,
                                                     &num_cal_entries,
                                                     &num_active_ts_port);
    }
    
    if (num_cal_entries == 0)
    {
        PMC_RETURN(COREOTN_ERR_TRIB_SLOT_NOT_IN_USE);
    }                                               

    if (PMC_SUCCESS == result)
    {
        if (local_ohfs_insert_ptr != NULL)
        {   
            for (i = 0; i < num_cal_entries; i++)
            {
                result = ohfs_insert_msi_get(local_ohfs_insert_ptr, cal_entry_ptr[i], &msi);
                
                if (ts_port_ptr[i] != 0xFF)
                {
                    *(msi_byte + (ts_port_ptr[i])) = msi;
                    
                } 
            }  
        }
        else {
            result = COREOTN_ERR_NULL_HANDLE;
        } 
    } 
    
    
    
    PMC_RETURN(result);
} /* odu_struct_tx_msi_get */

/*******************************************************************************
* odu_struct_tx_msi_per_ts_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Retrieves multiplex structure identifier for the specified tributary slot
*   within the ODU channel.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t
*   ts_port             - tributary port number within a HO or MO whose MSI value
*                         to be extracted
*
* OUTPUTS:
*   *msi_byte           - MSI value.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tx_msi_per_ts_get(coreotn_handle_t *coreotn_handle,
                                              odu_struct_t *odu_struct,
                                              odu_struct_ext_mode_t ext_mode,
                                              UINT8 ts_port,
                                              UINT8 *msi_byte)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    UINT32 num_tribslot = 0;
    UINT32 cal_entry_ptr[2];
    UINT32 num_cal_entries = 0;
    odtu_mux_handle_t* odtu_mux_ptr;
    
    PMC_ENTRY();

    PMC_ASSERT(odu_struct->mem_ptr->payload_format != UTIL_GLOBAL_ODU_UNCHANNELIZED, COREOTN_ERR_ODU_STRUCT_OPU, 0, 0);


    /* argument checking */
    if (ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    /* check if the ODU type is valid as a HO or MO */
    if ((odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_4_ODU 
        && odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC)
        && (odu_struct->mem_ptr->odu_type == UTIL_GLOBAL_ODU0
            || odu_struct->mem_ptr->odu_type == UTIL_GLOBAL_ODUFLEX_CBR  
            || odu_struct->mem_ptr->odu_type == UTIL_GLOBAL_ODUFLEX_GFP
            || odu_struct->mem_ptr->odu_type > UTIL_GLOBAL_ODU2E_ETRANS))
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    

    odtu_mux_ptr = odu_struct_mux_handle_get(coreotn_handle,
                                             odu_struct);


    /* test if the ts_port is within the valid range */
    result = util_global_max_server_num_ts_get(UTIL_GLOBAL_MUX_DEMUX,
                                               odu_struct->mem_ptr->odu_type,
                                               odu_struct->mem_ptr->payload_format,
                                               &num_tribslot);
    
    if (PMC_SUCCESS == result)
    {
        if (ts_port >= num_tribslot)
        {
            result = COREOTN_ERR_INVALID_ARG;
        }
    }    

    if (PMC_SUCCESS == result)
    {
        /* initialize cal_entry_ptr array */
        for (i = 0; i < 2; i++)
        {
            cal_entry_ptr[i] = 0x7F;
        }
        
        /* find corresponding calendar entries that belong to the tributary port */
        result = coreotn_db_ts_port_cal_entry_get(coreotn_handle, 0, 
                                                  odu_struct->mem_ptr->odu_level,
                                                  odu_struct->switch_data.oduk_data.channel,
                                                  ts_port,
                                                  cal_entry_ptr,
                                                  &num_cal_entries);
    }
                                             
    if (PMC_SUCCESS == result)
    {           
        result = ohfs_insert_msi_get(odtu_mux_ptr->tgmp_handle->ohfs_insert_handle,
                                     cal_entry_ptr[0],
                                     msi_byte);
    }
    
    PMC_RETURN(result);
} /* odu_struct_tx_msi_per_ts_get */




/*******************************************************************************
* odu_struct_rx_partial_ho_accept_msi_init
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Sets TS_VALID = 1 so MSI can be received on unused tribslots.  Or sets
*   TS_VALID=0 once MSI has been retrieved.
*
*   It is required to set TS_VALID on both online and offline page in the
*   event that a page swap occurs in-between
*   odu_struct_rx_partial_ho_accept_msi_init() and odu_struct_tx_msi_per_ts_get()
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   init_msi            - TRUE : Prepare the DMX to accept MSI on
*                                unused tribslots
*                         FALSE : remove work-around required to
*                                 receive MSI on unused tribslots
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_partial_ho_accept_msi_init(coreotn_handle_t *coreotn_handle,
                                                           odu_struct_t *odu_struct,
                                                           BOOL init_msi)
{
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *local_odtu_dmx_ptr = NULL;
    UINT32 i;
    UINT32 num_active_ts_port = 0;
    UINT32 num_cal_entries = 0;
    UINT32 ts_port_ptr[UTIL_GLOBAL_MAX_NUM_TS];
    UINT32 cal_entry_ptr[UTIL_GLOBAL_MAX_NUM_TS];

    PMC_ENTRY();

    PMC_ASSERT(odu_struct->mem_ptr->payload_format != UTIL_GLOBAL_ODU_UNCHANNELIZED, COREOTN_ERR_ODU_STRUCT_OPU, 0, 0);
 
    
    local_odtu_dmx_ptr = odu_struct_dmx_handle_get(coreotn_handle,
                                                   odu_struct);

    for (i = 0; i < UTIL_GLOBAL_MAX_NUM_TS; i++)
    {
        ts_port_ptr[i] = 0;
        cal_entry_ptr[i] = 0;
    }
                                          
    if (PMC_SUCCESS == result)
    {                                          
        result = coreotn_db_cal_entry_to_ts_port_get(coreotn_handle,
                                                     1, /*RX DB*/
                                                     odu_struct->mem_ptr->odu_level,
                                                     odu_struct->switch_data.oduk_data.channel,
                                                     cal_entry_ptr,
                                                     ts_port_ptr,
                                                     &num_cal_entries,
                                                     &num_active_ts_port);
    }
                                                    
    if (num_cal_entries == 0)
    {
        PMC_RETURN(COREOTN_ERR_TRIB_SLOT_NOT_IN_USE);
    }                                                   

      

    /* In order to receive MSI on all HO entries, it's required to set TS_VALID = 1
    on all unprovisioned tribslots*/
    for (i = 0; i < num_cal_entries; i++)
    {
        if (PMC_SUCCESS == result)
        {
            if (ts_port_ptr[i] == 0xFF)
            {
                result = ohfs_remove_ts_valid_cfg(local_odtu_dmx_ptr->rgmp_handle->ohfs_remove_handle, 
                                                  cal_entry_ptr[i],
                                                  init_msi);
            }
        
        }     
    }
                    
    /* flip the page so that we update both A/B pages for the new 
    **  MSI byte
    */   
    /* associate the LO channel to its parent HO channel */ 
    if (PMC_SUCCESS == result)
    {
        result = odtu_dmx_ho_config_addr_st_id_set(local_odtu_dmx_ptr, odu_struct->switch_data.oduk_data.channel);
    }
                
    if (PMC_SUCCESS == result)
    {
        result = odtu_dmx_reconfig_status_update(local_odtu_dmx_ptr,
                                                 odu_struct->switch_data.oduk_data.channel, 1);
    }
                    
    if (PMC_SUCCESS == result)
    {
        result = odtu_dmx_config_update_poll(local_odtu_dmx_ptr,
                                             odu_struct->switch_data.oduk_data.channel,
                                             COREOTN_POLL_COUNT_MAX);
                                                             
    }
             
    for (i = 0; i < num_cal_entries; i++)
    {
        if (PMC_SUCCESS == result)
        {
            if (ts_port_ptr[i] == 0xFF)
            {
                result = ohfs_remove_ts_valid_cfg(local_odtu_dmx_ptr->rgmp_handle->ohfs_remove_handle, 
                                                  cal_entry_ptr[i],
                                                  init_msi);
            }
        }     
    }

    PMC_RETURN(result);
} /* odu_struct_rx_partial_ho_accept_msi_init */


/*******************************************************************************
* odu_struct_rx_accpt_msi_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Retrieves accepted multiplex structure identifier for tributary slots used
*   by the ODU channel in an ODU_STRUCT.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t
*
* OUTPUTS:
*   *msi_byte           - MSI value.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_accpt_msi_get(coreotn_handle_t *coreotn_handle,
                                             odu_struct_t *odu_struct,
                                             odu_struct_ext_mode_t ext_mode,
                                             UINT8 *msi_byte)
{
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *local_odtu_dmx_ptr = NULL;
    UINT32 i;
    UINT8 msi;
    UINT32 num_active_ts_port = 0;
    UINT32 num_cal_entries = 0;
    UINT32 ts_port_ptr[UTIL_GLOBAL_MAX_NUM_TS];
    UINT32 cal_entry_ptr[UTIL_GLOBAL_MAX_NUM_TS];
    UINT32 msi_ptr[UTIL_GLOBAL_MAX_NUM_TS];
    UINT32 msi_acceptance_delay_us = 0;
    BOOL8 ts_valid;
    BOOL8 apply_ts_valid_workaround = FALSE;

    PMC_ENTRY();

    PMC_ASSERT(odu_struct->mem_ptr->payload_format != UTIL_GLOBAL_ODU_UNCHANNELIZED, COREOTN_ERR_ODU_STRUCT_OPU, 0, 0);


    /* argument checking */
    if (ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
      
    local_odtu_dmx_ptr = odu_struct_dmx_handle_get(coreotn_handle,
                                                   odu_struct);

    for (i = 0; i < UTIL_GLOBAL_MAX_NUM_TS; i++)
    {
        msi_ptr[i] = 0xFF;
        ts_port_ptr[i] = 0;
        cal_entry_ptr[i] = 0;
    }

    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {                                                  
        if (PMC_SUCCESS == result)
        {                                          
            result = coreotn_db_cal_entry_to_ts_port_get(coreotn_handle,
                                                         1, /*RX DB*/
                                                         odu_struct->mem_ptr->odu_level,
                                                         odu_struct->switch_data.oduk_data.channel,
                                                         cal_entry_ptr,
                                                         ts_port_ptr,
                                                         &num_cal_entries,
                                                         &num_active_ts_port);
        }
                                                    
        if (num_cal_entries == 0)
        {
            PMC_RETURN(COREOTN_ERR_TRIB_SLOT_NOT_IN_USE);
        }                                                   
     
        /* if num_cal_entries != num_active_ts_port, there there are unused tribslots
        within the HO*/
        if(num_cal_entries != num_active_ts_port)
        {         
            for (i = 0; i < num_cal_entries; i++)
            {
                if (PMC_SUCCESS == result)
                {
                    if (ts_port_ptr[i] == 0xFF)
                    {
                        ts_valid = ohfs_remove_ts_valid_get(local_odtu_dmx_ptr->rgmp_handle->ohfs_remove_handle, 
                                                            cal_entry_ptr[i]);

                        if(ts_valid == FALSE)
                        {
                            apply_ts_valid_workaround = TRUE;
                        }
                    }
                }     
            }
            
            if (apply_ts_valid_workaround)
            {

                /* In order to receive MSI on all HO entries, it's required to set TS_VALID = 1
                on all unprovisioned tribslots*/
                for (i = 0; i < num_cal_entries; i++)
                {
                    if (PMC_SUCCESS == result)
                    {
                        if (ts_port_ptr[i] == 0xFF)
                        {
                            result = ohfs_remove_ts_valid_cfg(local_odtu_dmx_ptr->rgmp_handle->ohfs_remove_handle, 
                                                              cal_entry_ptr[i],
                                                              TRUE);
                        }
                    }     
                }
                    
                /* flip the page so that we update both A/B pages for the new 
                **  MSI byte
                */   
                /* associate the LO channel to its parent HO channel */ 
                if (PMC_SUCCESS == result)
                {
                    result = odtu_dmx_ho_config_addr_st_id_set(local_odtu_dmx_ptr, odu_struct->switch_data.oduk_data.channel);
                }
                
                if (PMC_SUCCESS == result)
                {
                    result = odtu_dmx_reconfig_status_update(local_odtu_dmx_ptr,
                                                             odu_struct->switch_data.oduk_data.channel, 1);
                }
                    
                if (PMC_SUCCESS == result)
                {
                    result = odtu_dmx_config_update_poll(local_odtu_dmx_ptr,
                                                         odu_struct->switch_data.oduk_data.channel,
                                                         COREOTN_POLL_COUNT_MAX);
                                                             
                }
            
                switch(odu_struct->mem_ptr->odu_type)
                {
                    case UTIL_GLOBAL_ODU4:
                        msi_acceptance_delay_us = 2692; /*1.168*256*9*/
                        break;
                    case UTIL_GLOBAL_ODU3:
                    case UTIL_GLOBAL_ODU3E1:
                    case UTIL_GLOBAL_ODU3E2:
                        msi_acceptance_delay_us = 3108; /*3.035*256*4*/
                        break;
                    case UTIL_GLOBAL_ODU2:
                    case UTIL_GLOBAL_ODU2F:
                    case UTIL_GLOBAL_ODU2E:
                    case UTIL_GLOBAL_ODU1E:
                    case UTIL_GLOBAL_ODU1F:
                        msi_acceptance_delay_us = 12484; /*12.191*256*4*/
                        break;
                    case UTIL_GLOBAL_ODU1:
                        msi_acceptance_delay_us = 50147; /*48.971*256*4*/
                        break;
                    default:
                        result = COREOTN_ERR_INVALID_PARAMETERS;
                        break;
                }

                PMC_Q_USLEEP(local_odtu_dmx_ptr, msi_acceptance_delay_us);
            }
        }

        for (i = 0; i < num_cal_entries; i++)
        {
            result = ohfs_remove_accpt_msi_get(local_odtu_dmx_ptr->rgmp_handle->ohfs_remove_handle,
                                               cal_entry_ptr[i],
                                               &msi);
                
            PMC_LOG_TRACE("extracted MSI = %u\n", msi);
            msi_ptr[i] = msi;
                
            *(msi_byte + i) = msi_ptr[i];
                
        }          
          

        if (apply_ts_valid_workaround)
        {     
            if (PMC_SUCCESS == result)
            {
                result = odtu_dmx_reconfig_status_update(local_odtu_dmx_ptr,
                                                         odu_struct->switch_data.oduk_data.channel, 1);
            }

            if (PMC_SUCCESS == result)
            {
                result = odtu_dmx_config_update_poll(local_odtu_dmx_ptr,
                                                              odu_struct->switch_data.oduk_data.channel,
                                                              COREOTN_POLL_COUNT_MAX);
                                                     
            }          
            
            /* Write back TS_VALID = 0 on all unused tribslots */
            for (i = 0; i < num_cal_entries; i++)
            {
                if (PMC_SUCCESS == result)
                {
                    if (ts_port_ptr[i] == 0xFF)
                    {
                        result = ohfs_remove_ts_valid_cfg(local_odtu_dmx_ptr->rgmp_handle->ohfs_remove_handle, 
                                                          cal_entry_ptr[i],
                                                          FALSE);
                    }                                                          
                }     
            }
        }                                          
    }
    

    PMC_RETURN(result);
} /* odu_struct_rx_accpt_msi_get */


/*******************************************************************************
* odu_struct_rx_accpt_msi_per_ts_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Retrieves accepted multiplex structure identifier for tributary slots used
*   by the ODU channel in an ODU_STRUCT.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t
*   ts_port             - tributary slot to retrive MSI for
*
* OUTPUTS:
*   *msi_byte           - MSI value.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_accpt_msi_per_ts_get(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    odu_struct_ext_mode_t ext_mode,
                                                    UINT8 ts_port,
                                                    UINT8 *msi_byte)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    UINT32 num_tribslot = 0;
    UINT32 cal_entry_ptr[2];
    UINT32 num_cal_entries = 0;
    odtu_dmx_handle_t* local_odtu_dmx_ptr;
    
    PMC_ENTRY();

    PMC_ASSERT(odu_struct->mem_ptr->payload_format != UTIL_GLOBAL_ODU_UNCHANNELIZED, COREOTN_ERR_ODU_STRUCT_OPU, 0, 0);


    /* argument checking */
    if (ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    /* check if the ODU type is valid as a HO or MO */
    if ((odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_4_ODU 
         && odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC)
        && (odu_struct->mem_ptr->odu_type == UTIL_GLOBAL_ODU0
            || odu_struct->mem_ptr->odu_type == UTIL_GLOBAL_ODUFLEX_CBR  
            || odu_struct->mem_ptr->odu_type == UTIL_GLOBAL_ODUFLEX_GFP
            || odu_struct->mem_ptr->odu_type > UTIL_GLOBAL_ODU2E_ETRANS))
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    local_odtu_dmx_ptr = odu_struct_dmx_handle_get(coreotn_handle,
                                                   odu_struct);

    /* test if the ts_port is within the valid range */
    result = util_global_max_server_num_ts_get(UTIL_GLOBAL_MUX_DEMUX,
                                               odu_struct->mem_ptr->odu_type,
                                               odu_struct->mem_ptr->payload_format,
                                               &num_tribslot);
    
    if (PMC_SUCCESS == result)
    {
        if (ts_port >= num_tribslot)
        {
            result = COREOTN_ERR_INVALID_ARG;
        }
    }    

    /* initialize cal_entry_ptr array */
    for (i = 0; i < 2; i++)
    {
        cal_entry_ptr[i] = 0x7F;
    }
    
    /* find corresponding calendar entries that belong to the tributary port */
    result = coreotn_db_ts_port_cal_entry_get(coreotn_handle, 1, 
                                              odu_struct->mem_ptr->odu_level,
                                              odu_struct->switch_data.oduk_data.channel,
                                              ts_port,
                                              cal_entry_ptr,
                                              &num_cal_entries);
                                              
    if (PMC_SUCCESS == result && ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {

          result = ohfs_remove_accpt_msi_get(local_odtu_dmx_ptr->rgmp_handle->ohfs_remove_handle,
                                             cal_entry_ptr[0],
                                             msi_byte);
    }
    

    PMC_RETURN(result);
} /* odu_struct_rx_accpt_msi_per_ts_get */


/*******************************************************************************
* odu_struct_tx_ftfl_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   configures the transmit FTFL field of an ODU_STRUCT or the device.
*
* INPUTS:
*   coreotn_handle      - pointer to COREOTN handle instance
*   odu_struct          - pointer to ODU_STRUCT to be operated on
*   fwd_id              - Forward Indication Field.
*                         One byte field
*                         Byte 0 of the FTFL message
*   fwd_op_id           - Forward Operator Identifier Field
*                         9 byte field
*                         Byte 9:1 of the FTFL message
*   bwd_id              - Backward Indication Field.
*                         One byte field
*                         Byte 128 of the FTFL message
*   bwd_op_id           - Backward Operator Identifier Field.
*                         9 byte field
*                         Byte 137:129 of the FTFL message
*   target              - configuration target of odu struct data.
*                         see odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_tx_ftfl_set(coreotn_handle_t *coreotn_handle,
                                        odu_struct_t *odu_struct,
                                        UINT8 fwd_id,
                                        UINT8 fwd_op_id[9],
                                        UINT8 bwd_id,
                                        UINT8 bwd_op_id[9],
                                        odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_struct_handle = NULL;
    UINT32 i;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (target > LAST_CONFIG_TARGET)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    odu_struct->mem_ptr->ftfl[0] = fwd_id;
    odu_struct->mem_ptr->ftfl[10] = bwd_id;

    for (i = 1; i < 10; i++)
    {
        odu_struct->mem_ptr->ftfl[i] = fwd_op_id[i-1];
    }

    for (i = 11; i < 20; i++)
    {
        odu_struct->mem_ptr->ftfl[i] = bwd_op_id[i-11];
    }

    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        local_odu_struct_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

        if (NULL != local_odu_struct_handle)
        {
            result = odu_tfrm_ftfl_cfg(local_odu_struct_handle,
                                       odu_struct->switch_data.oduk_data.channel, fwd_id,
                                       fwd_op_id, bwd_id, bwd_op_id);
        }
        else {
            result = COREOTN_ERR_NULL_HANDLE;
        }            
    }

    PMC_RETURN(result);
} /* odu_struct_tx_ftfl_set */

/*******************************************************************************
* odu_struct_tx_ftfl_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retrieves the transmit FTFL field from an ODU_STRUCT or directly from the 
*   device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   ftfl                - pointer to storage for FTFL field.
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   *ftfl               - retrieved FTFL field value. \n
*                         Byte 0: forward fault type identifier \n
*                         Byte 1 -- 9: forward operator identifier \n
*                         Byte 10: backward fault type identifier \n
*                         Byte 11 -- 19: backward operator identifier
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tx_ftfl_get(coreotn_handle_t *coreotn_handle,
                                        odu_struct_t *odu_struct,
                                        odu_struct_ext_mode_t ext_mode,
                                        UINT8 *ftfl)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 fwd_id[1], bwd_id[1];
    UINT8 fwd_op_id[9];
    UINT8 bwd_op_id[9];
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
    UINT32 i;
    
    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle != NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        
        local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
        PMC_ASSERT(local_odu_tfrm_handle != NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);  
            
        result = odu_tfrm_ftfl_ext(local_odu_tfrm_handle,
                                   odu_struct->switch_data.oduk_data.channel, fwd_id,
                                   fwd_op_id, bwd_id, bwd_op_id);
        
        if (PMC_SUCCESS == result)
        {
            /* copy to the output array */
            ftfl[0] = fwd_id[0];
            ftfl[10] = bwd_id[0];
            
            for (i = 0; i < 9; i++)
            {
                ftfl[i+1] = fwd_op_id[i];
                ftfl[i+11] = bwd_op_id[i];
            }
        }                            
    }
    else {
        for (i = 0; i < 20; i++)
        {
            ftfl[i] = odu_struct->mem_ptr->ftfl[i];
        }
    }    
                                              
    PMC_RETURN(result);
} /* odu_struct_tx_ftfl_get */

/*******************************************************************************
* odu_struct_rx_ftfl_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retrieves the accepted FTFL field from an ODU_STRUCT or directly from the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   ftfl                - pointer to storage for FTFL field.
*   ext_mode            - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   ftfl                - retrieved FTFL field value
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_ftfl_get(coreotn_handle_t *coreotn_handle,
                                        odu_struct_t *odu_struct,
                                        odu_struct_ext_mode_t ext_mode,
                                        UINT8 *ftfl)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 fwd_id[1], bwd_id[1];
    UINT8 fwd_op_id[9];
    UINT8 bwd_op_id[9];
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    UINT32 i;
    
    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
        PMC_ASSERT(local_odu_rfrm_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
        
        result = odu_rfrm_ftfl_ext(local_odu_rfrm_handle,
                                   odu_struct->switch_data.oduk_data.channel, 
                                   fwd_id, fwd_op_id, bwd_id, bwd_op_id);
        
        if (PMC_SUCCESS == result)
        {
            /* copy to the output array */
            ftfl[0] = fwd_id[0];
            ftfl[10] = bwd_id[0];
            
            for (i = 0; i < 9; i++)
            {
                ftfl[i+1] = fwd_op_id[i];
                ftfl[i+11] = bwd_op_id[i];
            } 
        }                      
    }        
                                         
    PMC_RETURN(result);
} /* odu_struct_rx_ftfl_get */

/*******************************************************************************
* odu_struct_tx_tti_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   configures the transmitted TTI field in the device.
*
* INPUTS:
*   coreotn_handle      - pointer to COREOTN handle instance
*   odu_struct          - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0      -- PM \n
*                         1 to 6 -- TCM1 to TCM6
*   tti_oh_ins          - enum, selects which TTI field will be inserted to the 
*                         corresponding TCM.
*                         See odu_struct_tti_oh_ins_t.
*   sapi                - SAPI characters (16 bytes)
*   dapi                - DAPI characters (16 bytes)
*   op_spec             - OPERATOR SPECIFIC characters (32 bytes)
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
PUBLIC PMC_ERROR odu_struct_tx_tti_set( coreotn_handle_t *coreotn_handle,
                                        odu_struct_t *odu_struct,
                                        odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                        odu_struct_tti_oh_ins_t tti_oh_ins,
                                        UINT8 sapi[16],
                                        UINT8 dapi[16],
                                        UINT8 op_spec[32] )
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
    
    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }  
       
    local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
        
    if (NULL != local_odu_tfrm_handle)
    {
        result = odu_tfrm_tti_cfg(local_odu_tfrm_handle,
                                  odu_struct->switch_data.oduk_data.channel, 
                                  (odu_tfrm_pm_tcm_id_t)pm_tcm_num,
                                  (odu_tfrm_tti_oh_ins_t)tti_oh_ins,
                                  sapi, dapi, op_spec);
    }
    else {
        result = COREOTN_ERR_NULL_HANDLE;
    }                                    

    PMC_RETURN(result);
} /* odu_struct_tx_tti_set */

/*******************************************************************************
* odu_struct_tx_tti_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retrieves the transmitted TTI field from the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0      -- PM \n
*                         1 to 6 -- TCM1 to TCM6
*
* OUTPUTS:
*   *sapi_ptr           - retrieved SAPI field value
*   *dapi_ptr           - retrieved DAPI field value
*   *op_spec_ptr        - retrieved OP_SPEC field value
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tx_tti_get(coreotn_handle_t *coreotn_handle,
                                       odu_struct_t *odu_struct,
                                       odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                       UINT8 *sapi_ptr,
                                       UINT8 *dapi_ptr,
                                       UINT8 *op_spec_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_trfm_handle = NULL;
    
    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }  
       
    local_odu_trfm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
        
    if (NULL != local_odu_trfm_handle)
    {
        result = odu_tfrm_tti_ext(local_odu_trfm_handle,
                                  odu_struct->switch_data.oduk_data.channel, 
                                  (odu_tfrm_pm_tcm_id_t)pm_tcm_num,
                                  sapi_ptr, dapi_ptr, op_spec_ptr);
    }                                  
    else {
        result = COREOTN_ERR_NULL_HANDLE;
    }        

    PMC_RETURN(result);
} /* odu_struct_tx_tti_get */

/*******************************************************************************
* odu_struct_rx_exp_tti_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   configures the received expected TTI field in the device.
*
* INPUTS:
*   coreotn_handle      - pointer to COREOTN handle instance
*   odu_struct          - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0      -- PM \n
*                         1 to 6 -- TCM1 to TCM6
*   sapi                - SAPI characters (16 bytes)
*   dapi                - DAPI characters (16 bytes)
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
PUBLIC PMC_ERROR odu_struct_rx_exp_tti_set( coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                            UINT8 sapi[16],
                                            UINT8 dapi[16] )
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES
        || sapi == NULL || dapi == NULL)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
    
    if (NULL != local_odu_rfrm_handle)
    {
        result = odu_rfrm_exp_tti_cfg(local_odu_rfrm_handle,
                                      odu_struct->switch_data.oduk_data.channel, 
                                      (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                      sapi, dapi);
    }
    else {
        result = COREOTN_ERR_NULL_HANDLE;
    }                                              

    PMC_RETURN(result);
} /* odu_struct_rx_exp_tti_set */

/*******************************************************************************
* odu_struct_rx_exp_tti_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retrieves the receive expected TTI field directly from the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0      -- PM \n
*                         1 to 6 -- TCM1 to TCM6
*
* OUTPUTS:
*   *sapi_ptr           - retrieved SAPI field value
*   *dapi_ptr           - retrieved DAPI field value
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_exp_tti_get(coreotn_handle_t *coreotn_handle,
                                           odu_struct_t *odu_struct,
                                           odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                           UINT8 *sapi_ptr,
                                           UINT8 *dapi_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    
    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
    
    if (NULL != local_odu_rfrm_handle)
    {
        result = odu_rfrm_exp_tti_ext(local_odu_rfrm_handle,
                                      odu_struct->switch_data.oduk_data.channel, 
                                      (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                      sapi_ptr, dapi_ptr);
    }
    else {
        result = COREOTN_ERR_NULL_HANDLE;
    }                                              
    
    PMC_RETURN(result);
} /* odu_struct_rx_exp_tti_get */

/*******************************************************************************
* odu_struct_rx_accepted_tti_lock
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   helps retrieve the receive expected TTI field directly from the device.
*   Lock tti before retrieve, and unlock it after retrieve.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   set                 - 1(lock)/0(unlock)
*   
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_accepted_tti_lock(coreotn_handle_t *coreotn_handle,
                                                  BOOL8 set)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    if(coreotn_handle->odu_rfrm_stg1_handle!=NULL)
        result |= odu_rfrm_acpt_tti_lock(coreotn_handle->odu_rfrm_stg1_handle, set);
   
    if(coreotn_handle->odu_rfrm_stg2_handle!=NULL)
        result |= odu_rfrm_acpt_tti_lock(coreotn_handle->odu_rfrm_stg2_handle, set);

    if(coreotn_handle->odu_rfrm_stg3a_handle!=NULL)
        result |= odu_rfrm_acpt_tti_lock(coreotn_handle->odu_rfrm_stg3a_handle, set);

    if(coreotn_handle->odu_rfrm_stg3b_handle!=NULL)    
        result |= odu_rfrm_acpt_tti_lock(coreotn_handle->odu_rfrm_stg3b_handle, set);

    if(coreotn_handle->odu_rfrm_stgN_handle!=NULL)  
        result |= odu_rfrm_acpt_tti_lock(coreotn_handle->odu_rfrm_stgN_handle, set);

    if(coreotn_handle->odu_rfrm_stg4_handle!=NULL) 
        result |= odu_rfrm_acpt_tti_lock(coreotn_handle->odu_rfrm_stg4_handle, set);

    PMC_RETURN(result);
} /* odu_rx_accepted_tti_lock */

/*******************************************************************************
* odu_struct_rx_accpt_tti_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retrieves the receive expected TTI field directly from the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0      -- PM \n
*                         1 to 6 -- TCM1 to TCM6
*
* OUTPUTS:
*   *sapi_ptr           - retrieved SAPI field value
*   *dapi_ptr           - retrieved DAPI field value
*   *op_spec_ptr        - retrieved OP_SPEC field value
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_accpt_tti_get(coreotn_handle_t *coreotn_handle,
                                             odu_struct_t *odu_struct,
                                             odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                             UINT8 *sapi_ptr,
                                             UINT8 *dapi_ptr,
                                             UINT8 *op_spec_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    
    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
    
    if (NULL != local_odu_rfrm_handle)
    {
        result = odu_rfrm_acpt_tti_ext(local_odu_rfrm_handle,
                                       odu_struct->switch_data.oduk_data.channel, 
                                       (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                       sapi_ptr, dapi_ptr, op_spec_ptr);
    }
    else {
        result = COREOTN_ERR_NULL_HANDLE;
    }                                              
    
    PMC_RETURN(result);
} /* odu_struct_rx_accpt_tti_get */

/*******************************************************************************
* odu_struct_rx_pm_tcm_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the received PM&TCM OH field in the ODU channel 
*   represented in odu_struct.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*
* OUTPUTS:
*   *pm_tcm_val         - received PM&TCM value
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_pm_tcm_get(coreotn_handle_t *coreotn_handle,
                                          odu_struct_t *odu_struct,
                                          UINT8 *pm_tcm_val)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    
    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
    
    if (NULL != local_odu_rfrm_handle)
    {
        result = odu_rfrm_acpt_pm_tcm_ext(local_odu_rfrm_handle,
                                          odu_struct->switch_data.oduk_data.channel, pm_tcm_val);
    }
    else {
        result = COREOTN_ERR_NULL_HANDLE;
    }

    PMC_RETURN(result);                                                 
} /* odu_struct_rx_pm_tcm_get */

/*******************************************************************************
* odu_struct_pm_tcmi_bw_act_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the generation of backward consequential actions
*   for TCM1 to TCM6 or Floating TCM layer.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM1 \n
*                         8         -- PM NIM (non-intrusive monitoring)
*   bw_dis              - Controls whether or not backward consequential actions
*                         will be detected and processed. \n
*                         0 -- aBDI, aBEI and aBIAE consequential actions are
*                              detected and processed normally. \n
*                         1 -- aBDI, aBEI, and aBIAE consequential actions are
*                              disabled.
*   target              - Enum for configuration target of odu struct data.
*                         see odu_struct_target_t
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_pm_tcmi_bw_act_set(coreotn_handle_t *coreotn_handle,
                                               odu_struct_t *odu_struct,
                                               odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                               UINT8 bw_dis,
                                               odu_struct_target_t target)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    UINT16 tcm_pool_id = ODU_STRUCT_UNASSIGNED_POOL_ID;
    
    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES || bw_dis > 1)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    /* add BOOL8 bw_dis to odu_struct */
    /*odu_struct->mem_ptr->bw_dis = bw_dis;*/

    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
        
        if (NULL != local_odu_rfrm_handle)
        {

            if(pm_tcm_num != ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
            {
                result = odu_struct_tcm_pool_id_from_instance_get(coreotn_handle,
                                                                  odu_struct,
                                                                  ODU_STRUCT_STREAM_RX,
                                                                  pm_tcm_num,
                                                                  &tcm_pool_id);
            }

            if(result == PMC_SUCCESS)
            {
                result = odu_rfrm_pm_tcmi_bw_act_cfg(local_odu_rfrm_handle,
                                                     odu_struct->switch_data.oduk_data.channel,
                                                     (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                                     bw_dis,
                                                     tcm_pool_id);
            }
        }
        else {
            result = COREOTN_ERR_NULL_HANDLE;
        }
    }

    PMC_RETURN(result);                                                 
} /* odu_struct_pm_tcmi_bw_act_set */

/*******************************************************************************
* odu_struct_pm_tcmi_bw_act_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the configuration of backward consequential action
*   control for TCM1 to TCM6 or Floating TCM layer.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM1 \n
*                         8         -- PM NIM (non-intrusive monitoring)
*   
*   ext_mode            - extraction mode of the requested field. \n
*                         See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   *bw_dis             - extracted BW_DIS value
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_pm_tcmi_bw_act_get(coreotn_handle_t *coreotn_handle,
                                               odu_struct_t *odu_struct,
                                               odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                               odu_struct_ext_mode_t ext_mode,
                                               UINT8 *bw_dis)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    UINT16 tcm_pool_index;
    
    PMC_ENTRY();
    
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    /* argument checking */
    if (ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
        
        if (NULL != local_odu_rfrm_handle)
        {
            result = odu_rfrm_pm_tcmi_bw_act_get(local_odu_rfrm_handle,
                                                 odu_struct->switch_data.oduk_data.channel,
                                                 (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                                 bw_dis);
        }
        else {
            result = COREOTN_ERR_NULL_HANDLE;
        }
    }
    else {
        /* add BOOL8 bw_dis to odu_struct */
        if (pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
        {
            *bw_dis = odu_struct->mem_ptr->bw_dis;
        }
        else {
             result =odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle,
                                                               odu_struct,
                                                               pm_tcm_num,
                                                               &tcm_pool_index);
                                                               
             /* retrieve from ODU struct */   
             if (PMC_SUCCESS == result)
             {
                 *bw_dis = odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].bw_dis;
             }
        }  
    }
    
    PMC_RETURN(result);     
} /* odu_struct_pm_tcmi_bw_act_get */

/*******************************************************************************
* odu_struct_tx_pt_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   configures transmit PT field in an ODU_STRUCT or the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pt_val              - PT value
*   target              - configuration target of odu struct data.
*                         see odu_struct_target_t
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
PUBLIC PMC_ERROR odu_struct_tx_pt_set(coreotn_handle_t *coreotn_handle,
                                      odu_struct_t *odu_struct,
                                      UINT8 pt_val,
                                      odu_struct_target_t target)
{
    PMC_ERROR result = PMC_SUCCESS;
    odtu_mux_handle_t *local_odtu_mux_ptr = NULL;
    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);
    PMC_ASSERT(odu_struct->mem_ptr->payload_format != UTIL_GLOBAL_ODU_UNCHANNELIZED, COREOTN_ERR_ODU_STRUCT_OPU, 0, 0);


    odu_struct->mem_ptr->payload_type = pt_val;

    local_odtu_mux_ptr = odu_struct_mux_handle_get(coreotn_handle,
                                                   odu_struct);

    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE)
    {
        
        if (PMC_SUCCESS == result)
        {
            result = expand_ibuf_mem_ho_id_cfg(local_odtu_mux_ptr->tgmp_handle->expand_ibuf_handle,
                                               odu_struct->switch_data.oduk_data.channel);
        }

        if (PMC_SUCCESS == result)
        {
            result = ohfs_insert_pt_cfg(local_odtu_mux_ptr->tgmp_handle->ohfs_insert_handle,
                                        odu_struct->switch_data.oduk_data.channel,
                                        pt_val);
                                        
            /* flip the page so that we update both A/B pages for the new 
            **  MSI byte
            */
            if (PMC_SUCCESS == result)
            {
                result = odtu_mux_config_update_set(local_odtu_mux_ptr,
                                                    odu_struct->switch_data.oduk_data.channel, 1);
            }
            
            if (PMC_SUCCESS == result)
            {
                odtu_mux_config_update_poll(local_odtu_mux_ptr, 
                            odu_struct->switch_data.oduk_data.channel, 20000);
            }
            
            if (PMC_SUCCESS == result)
            {
                result = ohfs_insert_pt_cfg(local_odtu_mux_ptr->tgmp_handle->ohfs_insert_handle,
                                            odu_struct->switch_data.oduk_data.channel,
                                            pt_val);
            }                                                                                         
        }
        else 
        {
            result = COREOTN_ERR_INVALID_ARG;
        }
    }

    PMC_RETURN(result);
} /* odu_struct_tx_pt_set */

/*******************************************************************************
* odu_struct_tx_pt_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retrieves transmit PT field from an ODU_STRUCT or directly from the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   ext_mode            - extraction mode of the requested field. \n
*                         See odu_struct_ext_mode_t.
* OUTPUTS:
*   *pt_val             - retrieved PT field value.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tx_pt_get(coreotn_handle_t *coreotn_handle,
                                      odu_struct_t *odu_struct,
                                      odu_struct_ext_mode_t ext_mode,
                                      UINT8 *pt_val)
{
    PMC_ERROR result = PMC_SUCCESS;
    odtu_mux_handle_t *local_odtu_mux_ptr;

    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);
    PMC_ASSERT(odu_struct->mem_ptr->payload_format != UTIL_GLOBAL_ODU_UNCHANNELIZED, COREOTN_ERR_ODU_STRUCT_OPU, 0, 0);

    /* argument checking */
    if (ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    local_odtu_mux_ptr = odu_struct_mux_handle_get(coreotn_handle,
                                                   odu_struct);

    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {

        result = ohfs_insert_pt_get(local_odtu_mux_ptr->tgmp_handle->ohfs_insert_handle,
                                    odu_struct->switch_data.oduk_data.channel,
                                    pt_val);
    }
    else
    {
        *pt_val = odu_struct->mem_ptr->payload_type;
    }

    PMC_RETURN(result);
} /* odu_struct_tx_pt_get */

/*******************************************************************************
* odu_struct_rx_expected_pt_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   configures receive expected PT field in an ODU_STRUCT or the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   pt_val              - PT value
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
PUBLIC PMC_ERROR odu_struct_rx_expected_pt_set(coreotn_handle_t *coreotn_handle,
                                               odu_struct_t *odu_struct,
                                               UINT8 pt_val)
{
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *local_odtu_dmx_ptr = NULL;

    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);
    PMC_ASSERT(odu_struct->mem_ptr->payload_format != UTIL_GLOBAL_ODU_UNCHANNELIZED, COREOTN_ERR_ODU_STRUCT_OPU, 0, 0);


    local_odtu_dmx_ptr = odu_struct_dmx_handle_get(coreotn_handle,
                                                   odu_struct);

    odu_struct->mem_ptr->payload_type = pt_val;

    
    if (result == PMC_SUCCESS)
    {
        result = ohfs_remove_pt_cfg(local_odtu_dmx_ptr->rgmp_handle->ohfs_remove_handle,
                                    odu_struct->switch_data.oduk_data.channel, pt_val);   
                                    
        /* flip the page so that we update both A/B pages for the new 
        ** PT value
        */
        if (PMC_SUCCESS == result)
        {
            odtu_dmx_reconfig_status_update(local_odtu_dmx_ptr, odu_struct->switch_data.oduk_data.channel, 1);

            result = odtu_dmx_config_update_poll(local_odtu_dmx_ptr,
                                odu_struct->switch_data.oduk_data.channel, 
                                20000);
        
        }
        
        if (PMC_SUCCESS == result)
        {
            result = odtu_dmx_config_update_done_poll(local_odtu_dmx_ptr,
                                      odu_struct->switch_data.oduk_data.channel,
                                      20000);
        }
                                                 
        if (PMC_SUCCESS == result)
        {
            result = ohfs_remove_pt_cfg(local_odtu_dmx_ptr->rgmp_handle->ohfs_remove_handle,
                                        odu_struct->switch_data.oduk_data.channel, pt_val);  
        }                                                                                                       
    }
    else {
            result = COREOTN_ERR_NULL_HANDLE;
    }   

    PMC_RETURN(result);
} /* odu_struct_rx_expected_pt_set */

/*******************************************************************************
* odu_struct_rx_accepted_pt_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retrieves receive expected PT field from an ODU_STRUCT or directly from the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*
* OUTPUTS:
*   *accpt_pt_val             - retrieved PT field value.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_accepted_pt_get(coreotn_handle_t *coreotn_handle,
                                               odu_struct_t *odu_struct,
                                               UINT8 *accpt_pt_val)
{
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *local_odtu_dmx_ptr;

    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);
    PMC_ASSERT(odu_struct->mem_ptr->payload_format != UTIL_GLOBAL_ODU_UNCHANNELIZED, COREOTN_ERR_ODU_STRUCT_OPU, 0, 0);


    local_odtu_dmx_ptr = odu_struct_dmx_handle_get(coreotn_handle,
                                                   odu_struct);

    
    result = ohfs_remove_accpt_pt_get(local_odtu_dmx_ptr->rgmp_handle->ohfs_remove_handle,
                                      odu_struct->switch_data.oduk_data.channel,
                                      accpt_pt_val);

    PMC_RETURN(result);

} /* odu_struct_rx_accepted_pt_get */

/*******************************************************************************
* odu_struct_rx_expected_pt_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   retrieves receive expected PT field from an ODU_STRUCT or directly from the device.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*
* OUTPUTS:
*   *exp_pt_val             - retrieved PT field value.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_expected_pt_get(coreotn_handle_t *coreotn_handle,
                                               odu_struct_t *odu_struct,
                                               UINT8 *exp_pt_val)
{
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *local_odtu_dmx_ptr;

    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);
    PMC_ASSERT(odu_struct->mem_ptr->payload_format != UTIL_GLOBAL_ODU_UNCHANNELIZED, COREOTN_ERR_ODU_STRUCT_OPU, 0, 0);


    local_odtu_dmx_ptr = odu_struct_dmx_handle_get(coreotn_handle,
                                                   odu_struct);

    
    result = ohfs_remove_exp_pt_get(local_odtu_dmx_ptr->rgmp_handle->ohfs_remove_handle,
                                    odu_struct->switch_data.oduk_data.channel,
                                    exp_pt_val);

    PMC_RETURN(result);

} /* odu_struct_rx_expected_pt_get */

/*******************************************************************************
* odu_struct_maint_sig_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   extracts the type of maintenance signal present on the ODU channel 
*   represented by the ODU struct.
*
* INPUTS:
*   coreotn_handle      - pointer to COREOTN handle instance to be operated on
*   odu_struct          - pointer to ODU struct to be operated on
*   direction           - direction of the signal insertion. See odu_struct_stream_dir_t \n
*                         0 -- RX (Sink) \n
*                         1 -- TX (Source)
*
* OUTPUTS:
*   *maint_sig          - retrieved maintenance signal
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_maint_sig_get(coreotn_handle_t *coreotn_handle,
                                          odu_struct_t *odu_struct,
                                          odu_struct_stream_dir_t direction,
                                          coreotn_maint_sig_type_t *maint_sig)
{
    PMC_ERROR result = PMC_SUCCESS; 
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
    UINT8 maint_sig_val = 0;
    
    PMC_ENTRY();
    
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);
    
    if (direction > 1)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    if (direction  == 1)
    {
        /* TX */
        local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
        
        if (NULL != local_odu_tfrm_handle)
        {
            result = odu_tfrm_chnl_maint_sig_get(local_odu_tfrm_handle,
                                                 odu_struct->switch_data.oduk_data.channel, 
                                                 &maint_sig_val);
        }
        else {
            result = COREOTN_ERR_NULL_HANDLE;
        }
    }
    else {
        local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
        
        if (NULL != local_odu_rfrm_handle)
        {
            odu_rfrm_chnl_maint_sig_get(local_odu_rfrm_handle,
                                                 odu_struct->switch_data.oduk_data.channel, 
                                                 &maint_sig_val);
        } 
        else {
            result = COREOTN_ERR_NULL_HANDLE;
        } 
    }

    if(result == PMC_SUCCESS) {
            /*
              00: No maintenance signal is forced.
              01: Force ODUk-AIS insertion.
              10: Force ODUk-LCK insertion.
              11: Force ODUk-OCI insertion.
            */
            switch(maint_sig_val)
            {
            case 0:
                *maint_sig = CORE_OTN_NO_MAINT_SIG;
                break;

            case 1:
                *maint_sig = CORE_OTN_ODUK_AIS;
                break;

            case 2:
                *maint_sig = CORE_OTN_ODUK_LCK;
                break;

            case 3:
                *maint_sig = CORE_OTN_ODUK_OCI;
                break;

            default:
                *maint_sig = LAST_CORE_OTN_MAINT_SIG;
                result = COREOTN_ERR_INVALID_PARAMETERS;
                break;
            }
    }
     
    PMC_RETURN(result);
} /* odu_struct_maint_sig_get */

/*******************************************************************************
* odu_struct_rx_pm_tcmi_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the operational mode of PM or TCMi layer from an
*   ODU struct or the device.
*
*   Prior to calling this API, the layer must have been configured with
*   odu_struct_pm_tcmi_set() API.
*
* INPUTS:
*   *coreotn_handle      - pointer to COREOTN handle instance to be operated on
*   *odu_struct          - pointer to ODU struct to be operated on
*   pm_tcm_num           - PM or TCMi field instance: \n
*                          0         -- PM \n
*                          1 to 6    -- TCM1 to TCM6
*   ext_mode             - extraction mode of the requested field. \n
*                          See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   *mode                - functional mode of layer source function.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
* 
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_pm_tcmi_mode_get(coreotn_handle_t *coreotn_handle,
                                                odu_struct_t *odu_struct,
                                                odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                odu_struct_ext_mode_t ext_mode,
                                                odu_struct_pm_tcm_mode_t *mode)
{
    PMC_ERROR result = PMC_SUCCESS;  
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    UINT16 tcm_pool_index;
    
    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);
    
    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES
        || ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
        
        if (NULL != local_odu_rfrm_handle)
        {
            result = odu_rfrm_pm_tcmi_mode_get(local_odu_rfrm_handle,
                                               odu_struct->switch_data.oduk_data.channel,
                                               (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                               (odu_rfrm_pm_tcm_mode_t *)mode); 
        }
        else 
        {
            result = COREOTN_ERR_NULL_HANDLE;
        }            
    }
    else 
    {
        if (pm_tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM)
        {

            *mode = (odu_struct_pm_tcm_mode_t)odu_struct->mem_ptr->pm_mode;
        }
        else 
        {
            result = odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle,
                                                               odu_struct,
                                                               pm_tcm_num,
                                                               &tcm_pool_index);
            if (PMC_SUCCESS == result)
            {
                /* find tcm_pool_id_<tcm_pool_index>   */
                *mode = (odu_struct_pm_tcm_mode_t)(odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].mode_rx);
            }    
        }
    }                                                                                                               
        
    PMC_RETURN(result);

} /* odu_struct_rx_pm_tcmi_mode_get */



/*******************************************************************************
* odu_struct_rx_tcmi_resource_transition
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This API is used to transition between a TCMi-NIM provisioned resource to 
*    a floating TCM resource monitoring the same TCM level with the same 
*    pool id (visa-versa).  No changes in the TCM layering heirarchy are 
*    supported with this API.
*
*  For example:
*     DIGI_OTN_TCM_NUM_TCM1 as NIM -> DIGI_OTN_TCM_FLOATING_TCM (monitoring TCM1)
*       - or -
*     DIGI_OTN_TCM_FLOATING_TCM (monitoring TCM1) -> DIGI_OTN_TCM_NUM_TCM1 as NIM
*
* INPUTS:
*   *coreotn_handle      - pointer to COREOTN handle instance to be operated on
*   *odu_struct          - pointer to ODU struct to be operated on
*   pool_id              - Pool id of the TCM resource to transition (if the 
*                          resource is currently provisioned as a TCMi-NIM
*                          resource, it will transition to a floating TCM resource.  
*                          If currently provisioned as a floating TCM resource, 
*                          it will transition to a TCMi-NIM resource)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
* 
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_tcmi_resource_transition(coreotn_handle_t *coreotn_handle,
                                                        odu_struct_t *odu_struct,
                                                        UINT32 pool_id)
{
    PMC_ERROR result = PMC_SUCCESS;  
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    UINT32 num_layers;
    odu_rfrm_layer_struct_t odu_rfrm_layers[ODU_SINK_MAX_PM_TCM_LAYERS];
    UINT32 i,j;

    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);
    
    
    local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
        
    if (NULL != local_odu_rfrm_handle)
    {
        result  = odu_rfrm_tcmi_resource_transition(local_odu_rfrm_handle,
                                                    odu_struct->switch_data.oduk_data.channel,
                                                    pool_id);
    }
    else
    {
        result = COREOTN_ERR_NULL_HANDLE;
    }           


    if(result == PMC_SUCCESS)
    {
        result = odu_rfrm_layer_hierarchy_get(local_odu_rfrm_handle,
                                              odu_struct->switch_data.oduk_data.channel,
                                              &odu_rfrm_layers,
                                              &num_layers);
    }

    if(result == PMC_SUCCESS)
    {
        for(i=0;i<8;i++)
        {
              if(odu_struct->mem_ptr->tcm_pool_rsc[i].tcm_pool_id_rx == pool_id)
              {
                  for(j=0;j<num_layers;j++)
                  {
                      if(odu_rfrm_layers[j].pool_id == pool_id)
                      {
                          odu_struct->mem_ptr->tcm_pool_rsc[i].tcm_instance = (odu_struct_odu_frm_pm_tcm_id_t)odu_rfrm_layers[j].pm_tcm_num;
                      }
                  }
              }
        }
    }
    
        
    PMC_RETURN(result);

} /* odu_struct_rx_tcmi_resource_transition */

/*******************************************************************************
* odu_struct_tx_pm_tcmi_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the operational mode of PM or TCMi layer from an
*   ODU struct or the device.
*
*   Prior to calling this API, the layer must have been configured with
*   odu_struct_pm_tcmi_set() API.
*
* INPUTS:
*   *coreotn_handle      - pointer to COREOTN handle instance to be operated on
*   *odu_struct          - pointer to ODU struct to be operated on
*   pm_tcm_num           - PM or TCMi field instance: \n
*                          0         -- PM \n
*                          1 to 6    -- TCM1 to TCM6
*   ext_mode             - extraction mode of the requested field. \n
*                          See odu_struct_ext_mode_t.
*
* OUTPUTS:
*   *mode                - functional mode of layer source function.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tx_pm_tcmi_mode_get(coreotn_handle_t *coreotn_handle,
                                                odu_struct_t *odu_struct,
                                                odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                odu_struct_ext_mode_t ext_mode,
                                                odu_struct_pm_tcm_mode_t *mode)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
    UINT16 tcm_pool_index;
    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);
    
    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES
        || ext_mode > LAST_ODU_STRUCT_EXTRACT)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
        
        if (NULL != local_odu_tfrm_handle)
        {
            result = odu_tfrm_pm_tcmi_mode_get(local_odu_tfrm_handle,
                                               odu_struct->switch_data.oduk_data.channel,
                                               (odu_tfrm_pm_tcm_id_t)pm_tcm_num,
                                               (odu_tfrm_pm_tcm_mode_t *)mode);
        }
        else {
            result = COREOTN_ERR_NULL_HANDLE;
        }
    }
    else {
        result = odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle,
                                                           odu_struct,
                                                           pm_tcm_num,
                                                           &tcm_pool_index);
        if (PMC_SUCCESS == result)
        {
            /* find tcm_pool_id_<tcm_pool_index>   */
            *mode = (odu_struct_pm_tcm_mode_t)odu_struct->mem_ptr->tcm_pool_rsc[tcm_pool_index].mode_tx;
        }    
    }                                                  
    
    PMC_RETURN(result);

} /* odu_struct_tx_pm_tcmi_mode_get */


/*
** Status retrieval APIs
*/

/*******************************************************************************
* odu_struct_rx_tcmi_defect_stat_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the status of defects in the PM or a TCMi layer of
*   an ODUk channel.
* 
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on\n
*   *odu_struct         - pointer to ODU struct to be operated on\n
*   pm_tcm_num          - PM layer instance. Only 1 and 7 are valid options\n
*                         1 to 6 -- TCM1 to TCM6 \n
*                         7      -- Floating TCM 
*
* OUTPUTS:
*   *defect_stat        - Defects actions status for the TCMi layer of the
*                         channel. \n
*                         bit 12    -- ABDI_V  \n
*                         bit 11    -- AAIS_V  \n
*                         bit 10    -- ATSD_V  \n
*                         bit 9     -- ATSF_V  \n
*                         bit 8     -- DIAE_V  \n
*                         bit 7     -- DLTC_V  \n
*                         bit 6     -- DBIAE_V \n
*                         bit 5     -- DBDI_V  \n
*                         bit 4     -- DDEG_V  \n
*                         bit 3     -- DTIM_V  \n
*                         bit 2     -- DAIS_V  \n
*                         bit 1     -- DOCI_V  \n
*                         bit 0     -- DLCK_V  \n
*                         If set to 1, the defect was asserted at the end of the
*                         previous transfer interval. \n
*                         If set to 0, the defect was cleared at the end of the
*                         previous transfer interval.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_tcmi_defect_stat_get(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                    UINT32 *defect_stat)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    PMC_ENTRY();
    
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);
    
    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
        
    if (NULL != local_odu_rfrm_handle)
    {
        result = odu_rfrm_tcmi_defect_stat_get(local_odu_rfrm_handle,
                                               odu_struct->switch_data.oduk_data.channel,
                                               (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                               defect_stat);
    }
    else {
        result = COREOTN_ERR_NULL_HANDLE;
    }
                                                 
    PMC_RETURN(result);
} /* odu_struct_rx_tcmi_defect_stat_get */

/*******************************************************************************
* odu_struct_rx_pm_defect_stat_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the status of defects in a PM layer of
*   an ODUk channel.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on \n
*   *odu_struct         - pointer to ODU struct to be operated on \n
*   pm_tcm_num          - PM layer instance. Only 0 and 8 are valid options. \n
*                         0 -- PM \n
*                         8 -- PM NIM
*   
* OUTPUTS:
*   *defect_stat        - Defects actions status for the TCMi layer of the
*                         channel. \n
*                         bit 10    -- ABDI_V \n
*                         bit 9     -- ATSD_V \n
*                         bit 8     -- ATSF_V \n
*                         bit 7     -- DLOFLOM_V \n
*                         bit 6     -- DOOF_V \n
*                         bit 5     -- DBDI_V \n
*                         bit 4     -- DDEG_V \ n
*                         bit 3     -- DTIM_V \n
*                         bit 2     -- DAIS_V \n
*                         bit 1     -- DOCI_V \n
*                         bit 0     -- DLCK_V
*                         If set to 1, the defect was asserted at the end of the
*                         previous transfer interval. \n
*                         If set to 0, the defect was cleared at the end of the
*                         previous transfer interval.
* 
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_pm_defect_stat_get(coreotn_handle_t *coreotn_handle,
                                                  odu_struct_t *odu_struct,
                                                  odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                  UINT32 *defect_stat)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    PMC_ENTRY();
    
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);
    
    if (pm_tcm_num > LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }
    
    local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
        
    if (NULL != local_odu_rfrm_handle)
    {
        result = odu_rfrm_pm_defect_stat_get(local_odu_rfrm_handle,
                                             odu_struct->switch_data.oduk_data.channel, 
                                             (odu_rfrm_pm_tcm_id_t)pm_tcm_num,
                                             defect_stat);
    }
    else {
        result = COREOTN_ERR_NULL_HANDLE;
    }

    PMC_RETURN(result);
} /* odu_struct_rx_pm_defect_stat_get */

/*******************************************************************************
* odu_struct_tx_iae_stat_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the status of incoming alignment error in the
*   corresponding channel.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   *odu_struct         - pointer to ODU struct to be operated on
*
* OUTPUTS:
*   *status             - pointer to storage for IAE status bits\n
*                         Bit 95 -- IAE status on channel 95 \n
*                         Bit 94 -- IAE status on channel 94 \n
*                         Bit 93 -- IAE status on channel 93 \n
*                               ... \n
*                         Bit 1  -- IAE status on channel 1 \n
*                         Bit 0  -- IAE status on channel 0
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tx_iae_stat_get(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            UINT32 *status)
{
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
    PMC_ENTRY();
    
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);
    
    local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
    
    if (NULL != local_odu_tfrm_handle)
    {
        result = odu_tfrm_iae_stat_get(local_odu_tfrm_handle,
                                       odu_struct->switch_data.oduk_data.channel, status);
    }
    else {
        result = COREOTN_ERR_NULL_HANDLE;
    }                                               
                                       
    PMC_RETURN(result);
} /* odu_struct_tx_iae_stat_get */

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */ 
/* LCOV_EXCL_START */
/*******************************************************************************
* odu_struct_prbs_dlss_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to retreive the state of PRBS DLSS defects on the
*   PRBS channel.
*
* INPUTS:
*   *coreotn_handle        - pointer to COREOTN handle instance.
*   *odu_struct            - pointer to ODU struct channel
*   prbs_chnl              - prbs channel ID
*                            Valid range: 0 -- 11
*
* OUTPUTS:
*   *dlss_stat             - current state of PRBS dlss defect for the prbs channel
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_prbs_dlss_status_get(coreotn_handle_t* coreotn_handle,
                                                 odu_struct_t *odu_struct,
                                                 UINT32 prbs_chnl,
                                                 UINT8 *dlss_stat)
{
    PMC_ERROR result = PMC_SUCCESS;
    coreotn_prbs_inst_t prbs_inst;

    PMC_ENTRY();

    /* check for invalid pointers */
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    result = odu_struct_prbs_inst_get(coreotn_handle, odu_struct, &prbs_inst);

    if (PMC_SUCCESS == result)
    {
        result = coreotn_prbs_dlss_status_get(coreotn_handle, prbs_inst,
                                              prbs_chnl, dlss_stat);
    }

    PMC_RETURN(result);
} /* odu_struct_prbs_dlss_status_get */
/* LCOV_EXCL_STOP */
#endif/*#ifndef DOXYGEN_PUBLIC_ONLY*/

/*******************************************************************************
* FUNCTION: odu_struct_ri_rcp_cfc_int_sel_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Select RI RCP CFC interrupt source and framer.
*   
*
* INPUTS:
*   coreotn_handle   - Pointer to subsystem handle.
*   framer_id        - framer from which CFC interrupts are extracted.
*   src_dir          - CFC framer direction: TX or RX.
*                      UTIL_GLOBAL_DIR_BOTH is not supported.  
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
PUBLIC PMC_ERROR odu_struct_ri_rcp_cfc_int_sel_set(coreotn_handle_t        *coreotn_handle,
                                                   odu_rcp_frm_id_t         framer_id,
                                                   util_global_direction_t  src_dir)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    result = odu_ri_rcp_cfc_int_sel_set(coreotn_handle->odu_rcp_handle,
                                        framer_id,
                                        src_dir);

    PMC_RETURN(result);
} /* odu_struct_ri_rcp_cfc_int_src_sel_set */

/*******************************************************************************
* FUNCTION: odu_struct_ri_rcp_cfc_int_sel_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns RI RCP CFC interrupts selected  source and framer.
*   
*
* INPUTS:
*   coreotn_handle    - Pointer to subsystem handle
*
* OUTPUTS:
*   framer_id_ptr        - selected framer
*   src_dir_ptr          - framer direction: TX or RX.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odu_struct_ri_rcp_cfc_int_sel_get(coreotn_handle_t        *coreotn_handle,
                                              odu_rcp_frm_id_t        *framer_id_ptr,
                                              util_global_direction_t *src_dir_ptr)
{

    PMC_ENTRY();
    
    odu_ri_rcp_cfc_int_sel_get(coreotn_handle->odu_rcp_handle,
                               framer_id_ptr,
                               src_dir_ptr);
    
    PMC_RETURN();
} /* odu_struct_ri_rcp_cfc_int_sel_get */

/*******************************************************************************
* FUNCTION: odu_struct_ri_rcp_cfc_int_type_sel_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Selects RI RCP CFC interrupt type: underflow or overflow interrupt.
*   
* INPUTS:
*   coreotn_handle   - Pointer to subsystem handle.
*   odu_struct       - pointer to ODU_STRUCT instance to be operated on
*   int_type         - interrupt type.  
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
PUBLIC PMC_ERROR odu_struct_ri_rcp_cfc_int_type_sel_set(coreotn_handle_t           *coreotn_handle,
                                                        odu_struct_t               *odu_struct,
                                                        odu_ri_rcp_cfc_int_type_t   int_type)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    /* check inputs */
    PMC_ASSERT(NULL != coreotn_handle, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != odu_struct, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);  
    
    result = odu_ri_rcp_cfc_int_type_sel_set(coreotn_handle->odu_rcp_handle,
                                             odu_struct->switch_data.oduk_data.channel,
                                             int_type);
    
    PMC_RETURN(result);
} /* odu_struct_ri_rcp_cfc_int_type_sel_set */

/*******************************************************************************
* FUNCTION: odu_struct_ri_rcp_cfc_int_type_sel_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns RI RCP CFC selected interrupt type: underflow or overflow interrupt.
*
* INPUTS:
*   coreotn_handle    - Pointer to subsystem handle
*   odu_struct        - pointer to ODU_STRUCT instance to be operated on
*
* OUTPUTS:
*   int_type_ptr     - interrupt type.  
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*         
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_ri_rcp_cfc_int_type_sel_get(coreotn_handle_t           *coreotn_handle,
                                                        odu_struct_t               *odu_struct,
                                                        odu_ri_rcp_cfc_int_type_t  *int_type_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    /* check inputs */
    PMC_ASSERT(NULL != coreotn_handle, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != odu_struct, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);  

    result = odu_ri_rcp_cfc_int_type_sel_get(coreotn_handle->odu_rcp_handle,
                                             odu_struct->switch_data.oduk_data.channel,
                                             int_type_ptr);
    
    PMC_RETURN(result);
} /* odu_struct_ri_rcp_cfc_int_type_sel_get */


/*******************************************************************************
* odu_struct_ri_rcp_extract_cfg
* ______________________________________________________________________________
* 
* DESCRIPTION: 
*  
*  Configures DI RCP extraction for an ODU framer channel. A RI_RCP PDU is \n
*  assembled  with the following information for the specified channel:
*    - The BDI/BEI/BIAE bits from the TCMs and PM layers are extracted. \n
*    - The four APS/PCC bytes from a single connection monitoring level are \n
*      extracted (one of ODUk Path, ODUk TCM1-6, or ODUk server layer trail \n
*      where the server layer trail is an OTUk or HO ODUk). \n
*    - The Server Signal Fail (SSF) over the 8 ODUk frame window where the \n
*      APS/PCC bytes are captured. 
*
*  Assembled RI_RCP DPU are sent to a the destination specified by a \n
*  digi_otn_odu_ri_rcp_dst_cfg_t structure.  odu_struct_ri_rcp_dest_get \n
*  function can be used to retrieve a digi_otn_odu_ri_rcp_dst_cfg_t structure \n
*  from a provisioned ODU framer channel on this DIGI or on a remote DIGI.\n
*  The APS/PCC connection monitoring level can updated dynamically. \n
*  odu_ri_rcp_aps_pcc_cfg should be used to update this parameter.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance.
*   *odu_struct         - pointer to ODU struct channel
*   *ri_rcp_dst_cfg_ptr - pointer to ODU RI RCP destination configuration
*   aps_pcc_mon_level   - The APS/PCC connection monitoring level  (0 to 7)
*   ch_entries          - number of calendar entries per channel
*   enable              - RI_RCP extraction control. \n 
*                         TRUE: Enabled \n 
*                         FALSE: Disabled \n 
*                         When the is parameter is FALSE, aps_pcc_mon_level \n 
*                         and ri_rcp_dst_cfg_ptr parameters are ignored 
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
PUBLIC PMC_ERROR odu_struct_ri_rcp_extract_cfg( coreotn_handle_t *coreotn_handle,
                                                odu_struct_t *odu_struct,
                                                digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg_ptr,
                                                UINT8 aps_pcc_mon_level,
                                                UINT32 *ch_entries,
                                                BOOL8 enable)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    odu_rcp_frm_id_t odu_rcp_frm_id;

    PMC_ENTRY();
    
    /* check inputs */
    PMC_ASSERT(NULL != coreotn_handle, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != odu_struct, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);    
    PMC_ASSERT(NULL != ri_rcp_dst_cfg_ptr, COREOTN_ERR_INVALID_ARG, 0, 0);
    
    odu_rcp_frm_id = odu_struct_odu_level_rcp_convert(coreotn_handle,odu_struct);
    rc = otn_odu_ri_rcp_extract_cfg( coreotn_handle->odu_rcp_handle,
                                     odu_rcp_frm_id,
                                     odu_struct->switch_data.oduk_data.channel,
                                     ri_rcp_dst_cfg_ptr,
                                     aps_pcc_mon_level,
                                     ch_entries,
                                     enable);

    PMC_RETURN(rc);
} /* odu_struct_ri_rcp_extract_cfg */

/*******************************************************************************
* odu_struct_ri_rcp_extract_cfg_get
* ______________________________________________________________________________
* 
* DESCRIPTION: 
*  
*  Gets DI RCP extraction config status(en/disable) for an ODU framer channel.
*
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance.
*   *odu_struct         - pointer to ODU struct channel
*   
* OUTPUTS:
*   *ri_rcp_dst_cfg_ptr - pointer to ODU RI RCP destination configuration
*   aps_pcc_mon_level   - The APS/PCC connection monitoring level  (0 to 7)
*   enable              - RI_RCP extraction control. \n 
*                         TRUE: Enabled \n 
*                         FALSE: Disabled \n 
*                         When the is parameter is FALSE, aps_pcc_mon_level \n 
*                         and ri_rcp_dst_cfg_ptr parameters are ignored 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_ri_rcp_extract_cfg_get( coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg_ptr,
                                                    UINT8 *aps_pcc_mon_level,
                                                    BOOL8 *enable)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    odu_rcp_frm_id_t odu_rcp_frm_id;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(NULL != coreotn_handle, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != odu_struct, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    odu_rcp_frm_id = odu_struct_odu_level_rcp_convert(coreotn_handle,odu_struct);
    rc = otn_odu_ri_rcp_extract_cfg_get(coreotn_handle->odu_rcp_handle,
                                        odu_rcp_frm_id,
                                        odu_struct->switch_data.oduk_data.channel,
                                        ri_rcp_dst_cfg_ptr,
                                        aps_pcc_mon_level,
                                        enable);

    PMC_RETURN(rc);
} /* odu_struct_ri_rcp_extract_cfg_get */

/*******************************************************************************
* odu_struct_ri_rcp_resize
* ______________________________________________________________________________
* 
* DESCRIPTION:  
*  
*   DI RCP extraction channel resize.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance.
*   *odu_struct         - pointer to ODU struct channel
*   ch_entries          - number of calendar entries per channel
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
PUBLIC PMC_ERROR odu_struct_ri_rcp_resize(coreotn_handle_t *coreotn_handle,
                                          odu_struct_t *odu_struct,
                                          UINT32 *ch_entries)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    odu_rcp_frm_id_t odu_rcp_frm_id;

    PMC_ENTRY();
    
    /* check inputs */
    PMC_ASSERT(NULL != coreotn_handle, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != odu_struct, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);    
    
    odu_rcp_frm_id = odu_struct_odu_level_rcp_convert(coreotn_handle,odu_struct);
    rc = otn_odu_ri_rcp_extract_resize(coreotn_handle->odu_rcp_handle,
                                       odu_rcp_frm_id,
                                       odu_struct->switch_data.oduk_data.channel,
                                       ch_entries);

    PMC_RETURN(rc);
} /* odu_struct_ri_rcp_resize */

/*******************************************************************************
* odu_struct_ri_rcp_aps_pcc_mon_level_cfg
* ______________________________________________________________________________
* 
* DESCRIPTION: 
*  
*   Configures the APS/PCC connection monitoring for an ODU framer channel. This function 
*   shall be called on a channel where the DI RCP extraction has been already enabled using 
*   odu_struct_ri_rcp_extract_cfg. 
*   
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance.
*   *odu_struct         - pointer to ODU struct channel
*   aps_pcc_mon_level   - The APS/PCC connection monitoring level  (0 to 7)
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
PUBLIC PMC_ERROR odu_struct_ri_rcp_aps_pcc_mon_level_cfg( coreotn_handle_t *coreotn_handle,
                                                          odu_struct_t *odu_struct,
                                                          UINT8 aps_pcc_mon_level)
{

    PMC_ERROR   rc = PMC_SUCCESS;
    odu_rcp_frm_id_t odu_rcp_frm_id;

    PMC_ENTRY();
    
    /* check inputs */
    PMC_ASSERT(NULL != coreotn_handle, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != odu_struct, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);    
        
    odu_rcp_frm_id = odu_struct_odu_level_rcp_convert(coreotn_handle,odu_struct);
    rc = otn_odu_ri_rcp_aps_pcc_mon_level_cfg( coreotn_handle->odu_rcp_handle,
                                           odu_rcp_frm_id,
                                           odu_struct->switch_data.oduk_data.channel,
                                           aps_pcc_mon_level);
    PMC_RETURN(rc);
} /* odu_struct_ri_rcp_aps_pcc_mon_level_cfg */

/*******************************************************************************
* odu_struct_ri_rcp_dest_get
* ______________________________________________________________________________
* 
* DESCRIPTION:
*  
*  This function provides the RI_RCP destination configuration of a specified \n
*  ODU framer channel
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance.
*   *odu_struct         - pointer to ODU struct channel
*   
* OUTPUTS:
*   *ri_rcp_dst_cfg_ptr   - pointer to ODU RI RCP destination configuration
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_ri_rcp_dest_get( coreotn_handle_t *coreotn_handle,
                                             odu_struct_t *odu_struct,
                                             digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg_ptr)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    odu_rcp_frm_id_t odu_rcp_frm_id;

    PMC_ENTRY();
    
    /* check inputs */
    PMC_ASSERT(NULL != coreotn_handle, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != odu_struct, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);    
    
    odu_rcp_frm_id = odu_struct_odu_level_rcp_convert(coreotn_handle,odu_struct);
    rc = otn_odu_ri_rcp_dest_get(coreotn_handle->odu_rcp_handle,
                                 odu_rcp_frm_id,
                                 odu_struct->switch_data.oduk_data.channel,
                                 ri_rcp_dst_cfg_ptr);
    
    PMC_RETURN(rc);
} /* odu_struct_ri_rcp_dest_get */

/*******************************************************************************
* odu_struct_bdi_bei_biae_insert_cfg
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   For REV A devices, calling this function will impact the insertion source
*   of BDI/BEI/BIAE of all channels.
*
*   Configures the source of BDI/BEI bits in the TX framer of a specified \n
*   channel. They could come from the RI_RCP or a local RX framer. This enable\n
*    affects  all TCM's and PM layers of a particular channel.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance.
*   *odu_struct         - pointer to ODU struct channel
*   num_cal_entries     - Number of calendar entries
*   source              - Transmit BDI/BEI bits source: \n 
*                         0 : Local Rx Framer \n
*                         1 : RI_RCP \n
*                         2 : Force value at local TFRM
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
PUBLIC PMC_ERROR odu_struct_bdi_bei_biae_insert_cfg(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    UINT32 num_cal_entries,
                                                    UINT8 source)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    odu_rcp_frm_id_t odu_rcp_frm_id;
    odu_tfrm_handle_t* local_tfrm_ptr = NULL; 

    PMC_ENTRY();
    
    /* check inputs */
    PMC_ASSERT(NULL != coreotn_handle, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != odu_struct, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);    
    

    local_tfrm_ptr = odu_struct_tfrm_handle_get(coreotn_handle,
                                                    odu_struct);            

    if(source == 0 || source == 1){

        odu_struct->mem_ptr->bdi_src = 0;
        odu_struct->mem_ptr->bei_biae_src = 0;

        odu_rcp_frm_id = odu_struct_odu_level_rcp_convert(coreotn_handle,odu_struct);

        rc = otn_odu_ri_rcp_bdi_bei_biae_insert_cfg(coreotn_handle->odu_rcp_handle,
                                                    odu_rcp_frm_id,
                                                    odu_struct->switch_data.oduk_data.channel,
                                                    num_cal_entries,
                                                    0 == source ? FALSE : TRUE);
                                                          
                                                      
        /* when we set local framer, we may not have the ODU RI_RCP activated*/
        if (0 == source )
        {
            /* ignore those error */
            if (ODU_RCP_STATE_NOT_OPERATIONNAL == rc || 
                ODU_RCP_STATE_RI_INSERT_BACKWARD_DEFECT_NOT_ENABLED == rc)
            {
                rc = PMC_SUCCESS;
            }
        }
        
        /* configure REMOTE_RCP */
        if (PMC_SUCCESS == rc) 
        {
            if (TRUE == pmc_coreotn_odu_rcp_enable_all_chnls(&coreotn_handle->base))
            {
                odu_struct_remote_rcp_global_cfg(coreotn_handle,
                                                 odu_rcp_frm_id,                        
                                                 source);
            }
            else
            {
                odu_struct_remote_rcp_cfg(coreotn_handle,odu_rcp_frm_id,odu_struct->switch_data.oduk_data.channel,source);   
            }
            
            /* RCP_EN = 1 for both source = local RX framer and source = RI_RCP */
            if (PMC_SUCCESS == rc)
            {
                /* set RCP_EN = 1 in the TFRM */
                rc = odu_tfrm_pm_tcmi_bdi_bei_biae_src_cfg(local_tfrm_ptr,
                                                           odu_struct->switch_data.oduk_data.channel,
                                                           0);
            }
        }

    } else {

        odu_struct->mem_ptr->bdi_src = 1;
        odu_struct->mem_ptr->bei_biae_src = 1;


        /* Set RCP_EN = 0 in the TFRM */
        rc = odu_tfrm_pm_tcmi_bdi_bei_biae_src_cfg(local_tfrm_ptr,
                                                   odu_struct->switch_data.oduk_data.channel,
                                                   1);
        /* disable RCP defect insertion */
        if (PMC_SUCCESS == rc)
        {
            odu_rcp_frm_id = odu_struct_odu_level_rcp_convert(coreotn_handle,odu_struct);

            if (TRUE == pmc_coreotn_odu_rcp_enable_all_chnls(&coreotn_handle->base))
            {
                odu_struct_remote_rcp_global_cfg(coreotn_handle,
                                                 odu_rcp_frm_id,
                                                 source);
            }
            else
            {
                odu_struct_remote_rcp_cfg(coreotn_handle,odu_rcp_frm_id,
                                          odu_struct->switch_data.oduk_data.channel, FALSE);   
            }
            otn_odu_ri_rcp_bdi_bei_biae_insert_cfg(coreotn_handle->odu_rcp_handle,
                                                   odu_rcp_frm_id,
                                                   odu_struct->switch_data.oduk_data.channel,
                                                   num_cal_entries,
                                                   FALSE);
        }
    }


    PMC_RETURN(rc);
} /* odu_struct_bdi_bei_biae_insert_cfg */



/*******************************************************************************
* odu_struct_bdi_bei_biae_status_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   For REV A devices, calling this function will get the inserted source
*   of BDI/BEI/BIAE of all channels.
*
*   Get configured the source of BDI/BEI bits in the TX framer of a specified \n
*   channel. They could come from the RI_RCP or a local RX framer. This enable\n
*    affects  all TCM's and PM layers of a particular channel.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance.
*   *odu_struct         - pointer to ODU struct channel
*
*
* OUTPUTS:
*   source              - Transmit BDI/BEI bits source: \n 
*                         0 : Local Rx Framer \n
*                         1 : RI_RCP \n
*                         2 : Force value at local TFRM
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odu_struct_bdi_bei_biae_status_get(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    UINT8 *source)
{
    odu_rcp_frm_id_t odu_rcp_frm_id;
    odu_tfrm_handle_t* local_odu_tfrm_handle = NULL;
    UINT32  tfrm_rcp_en;
    BOOL8 ri_rcp_en;

    PMC_ENTRY();

     /* check inputs */
    PMC_ASSERT(NULL != coreotn_handle, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != odu_struct, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);    

    tfrm_rcp_en = 0;
    ri_rcp_en = TRUE; 


    local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);

    PMC_ASSERT(NULL != local_odu_tfrm_handle, COREOTN_ERR_NULL_HANDLE, 0, 0);

     odu_tfrm_pm_tcmi_bdi_bei_biae_src_get(local_odu_tfrm_handle,
                                                                       odu_struct->switch_data.oduk_data.channel,
                                                                        &tfrm_rcp_en);

    if (FALSE == tfrm_rcp_en) 
    {
         *source = 2;
    }
   else
   {
        odu_rcp_frm_id = odu_struct_odu_level_rcp_convert(coreotn_handle,odu_struct);

        otn_odu_ri_rcp_bdi_bei_biae_insert_get(coreotn_handle->odu_rcp_handle,
                                                   odu_rcp_frm_id,
                                                    odu_struct->switch_data.oduk_data.channel,
                                                    &ri_rcp_en);

        if (TRUE == ri_rcp_en)
        {
              *source = 1;
        }
        else
        {
              *source = 0;
        }
   }

   PMC_RETURN();

} /* odu_struct_bdi_bei_biae_status_get */


/*******************************************************************************
* odu_struct_ri_rcp_aps_insert_cfg
* ______________________________________________________________________________
* 
* DESCRIPTION: 
*   Configures the APS/PCC bytes  insertion for an ODU framer channel from the \n
*   RI RCP port. The insertion monitoring level is taken from the RI_RCP PDU \n
*   received on the port. 
* 
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance.
*   *odu_struct         - pointer to ODU struct channel
*   num_cal_entries     - Number of calendar entries for the channel
*   source              - APS/PCC bytes source: \n 
*                         TRUE: RI_RPC \n 
*                         FALSE: non-selected 
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
PUBLIC PMC_ERROR odu_struct_ri_rcp_aps_insert_cfg( coreotn_handle_t *coreotn_handle,
                                                   odu_struct_t *odu_struct, 
                                                   UINT32 num_cal_entries,                                
                                                   BOOL8 source)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    odu_rcp_frm_id_t odu_rcp_frm_id;

    PMC_ENTRY();
    
    /* check inputs */
    PMC_ASSERT(NULL != coreotn_handle, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != odu_struct, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);    
   
    odu_rcp_frm_id = odu_struct_odu_level_rcp_convert(coreotn_handle,odu_struct);
    rc = otn_odu_ri_rcp_aps_insert_cfg(coreotn_handle->odu_rcp_handle,
                                       odu_rcp_frm_id,
                                       odu_struct->switch_data.oduk_data.channel,
                                       num_cal_entries, 
                                       source);
    
    PMC_RETURN(rc);
} /* odu_struct_ri_rcp_aps_insert_cfg */

/*******************************************************************************
* odu_struct_ri_rcp_aps_insert_en_get
* ______________________________________________________________________________
* 
* DESCRIPTION: 
*   Gets the APS/PCC bytes insertion enable status for an ODU framer channel from the \n
*   RI RCP port. The insertion monitoring level is taken from the RI_RCP PDU \n
*   received on the port. 
* 
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance.
*   *odu_struct         - pointer to ODU struct channel
*                                                                         
*    
* OUTPUTS:
*   source              - APS/PCC bytes source: \n 
*                         TRUE: RI_RPC \n 
*                         FALSE: non-selected 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_ri_rcp_aps_insert_en_get( coreotn_handle_t *coreotn_handle,
                                                      odu_struct_t *odu_struct,                              
                                                      BOOL8 *source)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    odu_rcp_frm_id_t odu_rcp_frm_id;

    PMC_ENTRY();
    
    /* check inputs */
    PMC_ASSERT(NULL != coreotn_handle, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != odu_struct, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);    

    odu_rcp_frm_id = odu_struct_odu_level_rcp_convert(coreotn_handle,odu_struct);
    rc = otn_odu_ri_rcp_aps_insert_en_get(coreotn_handle->odu_rcp_handle,
                                          odu_rcp_frm_id,
                                          odu_struct->switch_data.oduk_data.channel,
                                          source);

    PMC_RETURN(rc);
} /* odu_struct_ri_rcp_aps_insert_en_get */

/*******************************************************************************
* odu_struct_odu_di_rcp_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures Defect Information (DI) Ring Control Port (RCP) PDU transmission.
*
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance.
*    di_rcp_pdu        - the DI RCP PDUs to enable or disable. Use digi_di_rcp_pdu_t bit masks
*                        to define which PDUs you want to enable by or-ing  masks. You can 
*                        EX:   DIGI_DI_RCP_GFPF | DIGI_DI_RCP_CLIENT 
*                              You can also start form all and disable some PDUs.
*                        EX:   DIGI_DI_RCP_ALL ^  DIGI_DI_RCP_CLIENT 
*   enable             - TRUE to enable transfer; FALSE to disable it
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successful otherwise a descriptive error 
*   code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_odu_di_rcp_cfg(coreotn_handle_t           *coreotn_handle,  
                                           UINT32                      di_rcp_pdu,
                                           BOOL8                       enable)
{   
    PMC_ERROR rc = PMC_SUCCESS;    
    
    PMC_ENTRY();

    PMC_ASSERT(NULL != coreotn_handle, COREOTN_ERR_NULL_HANDLE, 0, 0);
    
    rc = odu_di_rcp_cfg(coreotn_handle->odu_rcp_handle,
                        di_rcp_pdu,
                        enable);
    
    PMC_RETURN(rc);
} /* odu_struct_odu_di_rcp_cfg */

/*******************************************************************************
* odu_struct_odu_rcp_di_pdu_msk_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns DI RCP PDU mask.
*
*
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance.
*
* OUTPUTS:
*   *pdu_msk_ptr          - PDU mask.
*   *sdh_cnt_sel_ptr      - SDH counts select mask
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odu_struct_odu_rcp_di_pdu_msk_get(coreotn_handle_t           *coreotn_handle,
                                              UINT32                     *pdu_msk_ptr,
                                              UINT16                     *sdh_cnt_sel_ptr)
{
    PMC_ENTRY();
    
    PMC_ASSERT(NULL != coreotn_handle, COREOTN_ERR_NULL_HANDLE, 0, 0);
    
    odu_rcp_di_pdu_msk_get(coreotn_handle->odu_rcp_handle, pdu_msk_ptr, sdh_cnt_sel_ptr);

    PMC_RETURN();
} /* odu_struct_odu_rcp_di_pdu_msk_get */

/*******************************************************************************
* odu_struct_odu_rcp_di_sdh_cnt_sel_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Save in context ODU DI RCP SDH counts select mask.
*
*
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance.
*    sdh_cnt_sel        - SDH counts select mask
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
PUBLIC void odu_struct_odu_rcp_di_sdh_cnt_sel_set(coreotn_handle_t           *coreotn_handle,
                                                  UINT16                      sdh_cnt_sel)
{
    PMC_ENTRY();
    
    PMC_ASSERT(NULL != coreotn_handle, COREOTN_ERR_NULL_HANDLE, 0, 0);
    
    odu_rcp_di_sdh_cnt_sel_set(coreotn_handle->odu_rcp_handle, sdh_cnt_sel);

    PMC_RETURN();
} /* odu_struct_odu_rcp_di_sdh_cnt_sel_set */


#ifndef DOXYGEN_PUBLIC_ONLY

/*******************************************************************************
*  odu_struct_remote_rcp_global_cfg
* ______________________________________________________________________________
* 
* DESCRIPTION: 
*   Configures REMOTE RCP registers for all channels. If any one channel needs 
*   to have a new source for BDI/BEI/BIAE insertion, this function will
*   impact backward indication overhead bytes of all channels to be sourced from 
*   the new source.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance.
*   odu_rcp_frm_id      - framer stage 
*   enable              - RI_RCP extraction control. \n 
*                         TRUE: BDI/BEI/BIAE from RI_RCP (remote ODU_RFRM/external device)\n
*                         FALSE: BDI/BEI/BIAE from local corresponding ODU_RFRM TSB core.\n
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
PRIVATE void odu_struct_remote_rcp_global_cfg(coreotn_handle_t *coreotn_handle,
                                              odu_rcp_frm_id_t odu_rcp_frm_id,                        
                                              BOOL8 enable)
{   
    UINT32 rcp_en_val; 
    UINT32 rcp_en_val_long;
    
    PMC_ENTRY();
    
    if (enable)
    {
        rcp_en_val = 0xfff;
        rcp_en_val_long = 0xffffffff;
    }
    else {
        rcp_en_val = 0x0;
        rcp_en_val_long = 0x0;
    }
    
    
    switch(odu_rcp_frm_id)
    {
        case ODU_RCP_FRM_STG1:
            coreotn_fmf_field_range_REMOTE_RCP12_CFG_set(NULL, coreotn_handle, 0, 0, 11, rcp_en_val);
            break;
        case ODU_RCP_FRM_STGN:
            coreotn_fmf_field_range_REMOTE_RCP12_CFG_set(NULL, coreotn_handle, 1, 0, 11, rcp_en_val);
            break;
        case ODU_RCP_FRM_STG2:
            coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_set(NULL, coreotn_handle, 0, 0, 31, rcp_en_val_long);
            coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_set(NULL, coreotn_handle, 0, 32, 63, rcp_en_val_long);
            coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_set(NULL, coreotn_handle, 0, 64, 95, rcp_en_val_long);
            break;
        case ODU_RCP_FRM_STG3A:        
            coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_set(NULL, coreotn_handle, 1, 0, 31, rcp_en_val_long);
            coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_set(NULL, coreotn_handle, 1, 32, 63, rcp_en_val_long);
            coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_set(NULL, coreotn_handle, 1, 64, 95, rcp_en_val_long);
            break;
        case ODU_RCP_FRM_STG3B:
            coreotn_fo_lfield_range_REMOTE_RCP_CFG_set(NULL, coreotn_handle, 0, 0, 31, rcp_en_val_long);
            coreotn_fo_lfield_range_REMOTE_RCP_CFG_set(NULL, coreotn_handle, 0, 32, 63, rcp_en_val_long);
            coreotn_fo_lfield_range_REMOTE_RCP_CFG_set(NULL, coreotn_handle, 0, 64, 95, rcp_en_val_long);
            break;
        case ODU_RCP_FRM_STG4:
            coreotn_fo_lfield_range_REMOTE_RCP_CFG_set(NULL, coreotn_handle, 1, 0, 31, rcp_en_val_long);
            coreotn_fo_lfield_range_REMOTE_RCP_CFG_set(NULL, coreotn_handle, 1, 32, 63, rcp_en_val_long);
            coreotn_fo_lfield_range_REMOTE_RCP_CFG_set(NULL, coreotn_handle, 1, 64, 95, rcp_en_val_long);
            break;
        default:
            break;
    }

    PMC_RETURN();
    
} /* odu_struct_remote_rcp_global_cfg */

/*******************************************************************************
*  odu_struct_remote_rcp_cfg
* ______________________________________________________________________________
* 
* DESCRIPTION: 
*  
*  Configures REMOTE RCP registers
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance.
*   odu_rcp_frm_id      - framer stage 
*   chnl_id             - channel id
*   enable              - RI_RCP extraction control. \n 
*                         TRUE: BDI/BEI/BIAE from RI_RCP (remote ODU_RFRM/external device)\n
*                         FALSE: BDI/BEI/BIAE from local corresponding ODU_RFRM TSB core.\n
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
PRIVATE void odu_struct_remote_rcp_cfg(coreotn_handle_t *coreotn_handle,
                                       odu_rcp_frm_id_t odu_rcp_frm_id,                        
                                       UINT32 chnl_id,
                                       BOOL8 enable)
{
    PMC_ENTRY();
    
    switch(odu_rcp_frm_id)
    {
        case ODU_RCP_FRM_STG1:
            coreotn_fmf_field_range_REMOTE_RCP12_CFG_set(NULL,coreotn_handle,0,chnl_id,chnl_id,enable);
            break;
        case ODU_RCP_FRM_STGN:
            coreotn_fmf_field_range_REMOTE_RCP12_CFG_set(NULL,coreotn_handle,1,chnl_id,chnl_id,enable);
            break;
        case ODU_RCP_FRM_STG2:
            coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_set(NULL,coreotn_handle,0,chnl_id,chnl_id,enable);
            break;
        case ODU_RCP_FRM_STG3A:        
            coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_set(NULL,coreotn_handle,1,chnl_id,chnl_id,enable);
            break;
        case ODU_RCP_FRM_STG3B:
            coreotn_fo_lfield_range_REMOTE_RCP_CFG_set(NULL,coreotn_handle,0,chnl_id,chnl_id, enable);
            break;
        case ODU_RCP_FRM_STG4:
            coreotn_fo_lfield_range_REMOTE_RCP_CFG_set(NULL,coreotn_handle,1,chnl_id,chnl_id, enable);
            break;
        default:
            break;
    }

    PMC_RETURN();
    
} /* odu_struct_remote_rcp_cfg */


/*******************************************************************************
* odu_struct_frm_id_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the framer stage from an odu_struct 
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   The Framer stage id
*
* NOTES:
*
*******************************************************************************/
PUBLIC odu_struct_frm_id_t odu_struct_frm_id_get(coreotn_handle_t *coreotn_handle,
                                                 odu_struct_t *odu_struct)
{
    odu_struct_frm_id_t odu_frm_id = LAST_ODU_STRUCT_FRM_ID;
    coreotn_init_operation_t local_init_operation = coreotn_handle->var.init_operation;
    PMC_ENTRY();
        
    /* find the correct framer */
    switch (odu_struct->switch_data.oduk_data.odu_level)
    {
        case UTIL_GLOBAL_LEVEL_HO_ODU:
            if (local_init_operation == COREOTN_SS_SYSOTN_MODE 
                || local_init_operation == COREOTN_SS_SYSOTN_BYPASS_MODE) 
            {
                odu_frm_id = ODU_STRUCT_FRM_STGN;
            } else {
                /* stage 1 */
                odu_frm_id = ODU_STRUCT_FRM_STG1;
            }
            break;
        case UTIL_GLOBAL_LEVEL_MO_ODU:
            
            if (local_init_operation == COREOTN_SS_SYSOTN_MODE 
                || local_init_operation == COREOTN_SS_SYSOTN_BYPASS_MODE) 
            {
                odu_frm_id = ODU_STRUCT_FRM_STG3A;
            } else {
                /* stage 2 */
                odu_frm_id = ODU_STRUCT_FRM_STG2;
            }        
            break;
        case UTIL_GLOBAL_LEVEL_LO_ODU:
            if (local_init_operation == COREOTN_SS_SYSOTN_MODE 
                || local_init_operation == COREOTN_SS_SYSOTN_BYPASS_MODE) 
            {
                /* stage 1 */
                odu_frm_id = ODU_STRUCT_FRM_STG1;
            } else {
                /* stage 3a */
                odu_frm_id = ODU_STRUCT_FRM_STG3A;
            }
            break;
        case UTIL_GLOBAL_LEVEL_3B_ODU:
            /* stage 3b */
           odu_frm_id = ODU_STRUCT_FRM_STG3B;
            break;
        case UTIL_GLOBAL_LEVEL_4_ODU:
            /* stage4 */
            odu_frm_id = ODU_STRUCT_FRM_STG4;
            break;
        case UTIL_GLOBAL_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC:
            /* stage 2 */
            odu_frm_id = ODU_STRUCT_FRM_STG2;
            break;
        default:
            odu_frm_id = LAST_ODU_STRUCT_FRM_ID;
            break;
    }
    
    PMC_RETURN(odu_frm_id);
} /* odu_struct_frm_id_get */

/*******************************************************************************
* odu_struct_ch_id_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the ch id from an odu_struct 
*
* INPUTS:
*   *odu_struct_ptr        - pointer to ODU_STRUCT to be operated on
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   The ch id
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 odu_struct_ch_id_get(odu_struct_t *odu_struct_ptr)
{
    PMC_ENTRY();
       
    PMC_ASSERT(odu_struct_ptr != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);
    PMC_ASSERT(odu_struct_ptr->switch_data.header.data_key == UTIL_GLOBAL_SWITCH_DATA_MAGIC_ID, 
               COREOTN_ERR_ODU_STRUCT_INVALID_PTR, odu_struct_ptr->switch_data.header.data_key,0);
    PMC_RETURN(odu_struct_ptr->switch_data.oduk_data.channel);
} /* odu_struct_ch_id_get */

/*******************************************************************************
* odu_struct_odu_level_rcp_convert
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the framer stage from an odu_struct 
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   The Framer stage
*
* NOTES:
*
*******************************************************************************/
PRIVATE odu_rcp_frm_id_t odu_struct_odu_level_rcp_convert(coreotn_handle_t *coreotn_handle,
                                                          odu_struct_t *odu_struct)
{
    odu_struct_frm_id_t frm_id;
    PMC_ENTRY();
    frm_id = odu_struct_frm_id_get(coreotn_handle, odu_struct);
        
    switch (frm_id)
    {
        case ODU_STRUCT_FRM_STG1:  PMC_RETURN(ODU_RCP_FRM_STG1);
        case ODU_STRUCT_FRM_STG2:  PMC_RETURN(ODU_RCP_FRM_STG2);
        case ODU_STRUCT_FRM_STG3A: PMC_RETURN(ODU_RCP_FRM_STG3A);
        case ODU_STRUCT_FRM_STG3B: PMC_RETURN(ODU_RCP_FRM_STG3B);
        case ODU_STRUCT_FRM_STG4:  PMC_RETURN(ODU_RCP_FRM_STG4);
        case ODU_STRUCT_FRM_STGN:  PMC_RETURN(ODU_RCP_FRM_STGN);
        default:                   PMC_RETURN(LAST_ODU_RCP_FRM_ID);
    }
    PMC_RETURN(LAST_ODU_RCP_FRM_ID);
} /* odu_struct_odu_level_rcp_convert */

/*******************************************************************************
* odu_struct_tcm_pool_id_from_instance_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the index of a TCM struct that uses the tcm pool id inside the
*   ODU struct
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   direction           - direction of TCM monitoring. See odu_struct_stream_dir_t.
*                         0 -- sink (RX)
*                         1 -- source (TX)
*   tcm_num             - TCM instance. See odu_struct_odu_frm_pm_tcm_id_t.
*
* OUTPUTS:
*   *tcm_pool_id        - pool id of TCM resource inside the odu struct
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tcm_pool_id_from_instance_get(coreotn_handle_t *coreotn_handle,
                                                          odu_struct_t *odu_struct,
                                                          odu_struct_stream_dir_t direction,
                                                          odu_struct_odu_frm_pm_tcm_id_t tcm_num,
                                                          UINT16 *tcm_pool_id)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    odu_rfrm_handle_t *rfrm_handle_ptr = NULL;
    odu_tfrm_handle_t *tfrm_handle_ptr = NULL;
    
    PMC_ENTRY();
   
    *tcm_pool_id =  ODU_STRUCT_UNASSIGNED_POOL_ID;

    /* argument checking */
    if (direction > 1 || tcm_num == ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM
        || tcm_num > ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM_NIM)
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if(odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_4_ODU &&
       odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC)
    {
        /* odu_struct_tcm_pool_id_search */
        if (direction)
        {
            /* tx */
            for (i = 0; i < 8; i++)
            {
                if (odu_struct->mem_ptr->tcm_pool_rsc[i].tcm_instance == tcm_num)
                {
                    *tcm_pool_id = odu_struct->mem_ptr->tcm_pool_rsc[i].tcm_pool_id_tx;
                    
                    break;
                }    
            }
        }
        else {
            /* rx */
            for (i = 0; i < 8; i++)
            {
                if (odu_struct->mem_ptr->tcm_pool_rsc[i].tcm_instance == tcm_num)
                {
                    *tcm_pool_id = odu_struct->mem_ptr->tcm_pool_rsc[i].tcm_pool_id_rx;
                    
                    break;
                }    
            }
        } 
    }
    else
    {
        if (direction)
        {
            tfrm_handle_ptr = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
            if (NULL != tfrm_handle_ptr)
            {
                result = odu_tfrm_pool_id_return(tfrm_handle_ptr, 
                                                 odu_struct->switch_data.oduk_data.channel, 
                                                 (odu_tfrm_pm_tcm_id_t)tcm_num, tcm_pool_id);
            }
        }
        else
        {
            rfrm_handle_ptr = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
            if (NULL != rfrm_handle_ptr)
            {
                UINT32 tcm_pool_id_wide = ODU_STRUCT_UNASSIGNED_POOL_ID;
                result = odu_rfrm_pool_id_return(rfrm_handle_ptr, 
                                                 odu_struct->switch_data.oduk_data.channel, 
                                                 (odu_rfrm_pm_tcm_id_t)tcm_num, &tcm_pool_id_wide);
                *tcm_pool_id = tcm_pool_id_wide;
            }
        }
    }

    if (PMC_SUCCESS == result && ODU_STRUCT_UNASSIGNED_POOL_ID == *tcm_pool_id)
    {
        result = COREOTN_ERR_POOL_ID_UNAVAILABLE;
    }
    
    PMC_RETURN(result);
} /* odu_struct_tcm_pool_id_from_instance_get */


/*******************************************************************************
* odu_struct_tcm_pool_idx_from_instance_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the index of a TCM struct that matches TCM layer instnace inside
*   the ODU struct
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   tcm_num             - TCM instance. See odu_struct_odu_frm_pm_tcm_id_t.
*
* OUTPUTS:
*   *tcm_pool_index     - index of TCM struct inside the odu struct
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_struct_tcm_pool_idx_from_instance_get(coreotn_handle_t *coreotn_handle,
                                                            odu_struct_t *odu_struct,
                                                            odu_struct_odu_frm_pm_tcm_id_t tcm_num,
                                                            UINT16 *tcm_pool_index)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    BOOL found = FALSE;
    
    PMC_ENTRY();




    if(odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_4_ODU &&
       odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC)
    {
          /* odu_struct_tcm_pool_id_search */
          for (i = 0; i < COREOTN_PIDS_PER_CHNL; i++)
          {
              if (odu_struct->mem_ptr->tcm_pool_rsc[i].tcm_instance == tcm_num)
              {
                  *tcm_pool_index = i;  
                  found = TRUE;
                  break;
              }
          }

    }
    else
    {
        /* not allowed to access mem_ptr for stages ODU_STRUCT_LEVEL_4_ODU or 
          ODU_STRUCT_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC. The value returned from
          this API should only be used to store information to the mem_ptr
          struct for fast APS reconfig.  Since a dummy mem_ptr is used for these
          stages, it's acceptable to return 0 */
        found = TRUE;
        *tcm_pool_index = 0;
    }

            
    
    if (found != TRUE)
    {
        *tcm_pool_index = ODU_STRUCT_PIDS_PER_CHNL;
        result = COREOTN_ERR_INVALID_ARG;
    }

    PMC_RETURN(result);
} /* odu_struct_tcm_pool_idx_from_instance_get */

#endif /* DOXYGEN_PUBLIC_ONLY */


/*******************************************************************************
* odu_struct_prbs_inst_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the PRBS instance for a given ODU struct.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*
* OUTPUTS:
*   *prbs_inst          - prbs instance to be returned
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_prbs_inst_get(coreotn_handle_t *coreotn_handle,
                                          odu_struct_t *odu_struct,
                                          coreotn_prbs_inst_t *prbs_inst)
{
    PMC_ERROR   result = PMC_SUCCESS;
    PMC_ENTRY();

    result = odu_struct_prbs_inst_get_helper(coreotn_handle,
                                             odu_struct->mem_ptr->odu_level,
                                             prbs_inst);

    PMC_RETURN(result);
} /* odu_struct_prbs_inst_get */

/*******************************************************************************
* odu_struct_prbs_tfrm_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configure TFRM instance to enable PM layer when enabling 
*   prbs generator
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*   enable              - enable 
*
* OUTPUTS:
*   NONE
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_prbs_tfrm_cfg(coreotn_handle_t *coreotn_handle,
                                          odu_struct_t *odu_struct,
                                          BOOL8 enable)
{
    PMC_ERROR   result = PMC_SUCCESS;
    odu_tfrm_pm_tcm_mode_t tcm_mode;
    odu_tfrm_handle_t *local_odu_tfrm_ptr = NULL;
    coreotn_prbs_inst_t prbs_inst;

    PMC_ENTRY();
    /* Tx direction */
    local_odu_tfrm_ptr = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
    /* Get PRBS instance */
    result = odu_struct_prbs_inst_get_helper(coreotn_handle,
                                             odu_struct->mem_ptr->odu_level,
                                             &prbs_inst);
    
    if (PMC_SUCCESS == result)
    {
        result = odu_tfrm_pm_tcmi_mode_get(local_odu_tfrm_ptr,
                                           odu_struct->switch_data.oduk_data.channel,
                                           ODU_TFRM_MESSAGE_TYPE_PM,
                                           &tcm_mode);      
    }      
    if (PMC_SUCCESS == result)
    {
        if (TRUE == enable) 
        {
            if(coreotn_handle->var.prev_pm_mode[(UINT32)prbs_inst][odu_struct->switch_data.oduk_data.channel] == ODU_TFRM_PM_TCM_START) 
            {
                coreotn_handle->var.prev_pm_mode[(UINT32)prbs_inst][odu_struct->switch_data.oduk_data.channel] = tcm_mode;
            }
            
            if (PMC_SUCCESS == result && tcm_mode != ODU_TFRM_PM_TCM_TT_OPERATIONAL)
            {                
                result = odu_tfrm_tcmi_cfg(local_odu_tfrm_ptr,
                                           odu_struct->switch_data.oduk_data.channel,
                                           ODU_TFRM_MESSAGE_TYPE_PM,
                                           ODU_TFRM_MESSAGE_TYPE_DCI,
                                           ODU_TFRM_PM_TCM_TT_OPERATIONAL,
                                           TRUE);    
            }            
        } else 
        {
            if (tcm_mode != coreotn_handle->var.prev_pm_mode[(UINT32)prbs_inst][odu_struct->switch_data.oduk_data.channel])
            {

                result = odu_tfrm_tcmi_cfg(local_odu_tfrm_ptr,
                                           odu_struct->switch_data.oduk_data.channel,
                                           ODU_TFRM_MESSAGE_TYPE_PM,
                                           ODU_TFRM_MESSAGE_TYPE_DCI,
                                           ODU_TFRM_PM_TCM_TT_TRANSPARENT,
                                           FALSE);
            }

            coreotn_handle->var.prev_pm_mode[(UINT32)prbs_inst][odu_struct->switch_data.oduk_data.channel] = ODU_TFRM_PM_TCM_START;
        }       
    }

    PMC_RETURN(result);
} /* odu_struct_prbs_tfrm_cfg */

/*******************************************************************************
* odu_struct_prbs_inst_get_helper
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the PRBS instance for a given ODU level
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   odu_level           - odu level to retrieve PRBS instance
*
* OUTPUTS:
*   *prbs_inst_ptr      - prbs instance to be returned
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_prbs_inst_get_helper(coreotn_handle_t       *coreotn_handle,
                                                 odu_struct_odu_level_t  odu_level,
                                                 coreotn_prbs_inst_t    *prbs_inst_ptr)
{
    coreotn_init_operation_t local_init_operation = coreotn_handle->var.init_operation;
    PMC_ENTRY();

    /* find the correct framer */
    switch (odu_level)
    {
        case ODU_STRUCT_LEVEL_HO_ODU:
            if (local_init_operation == COREOTN_SS_SYSOTN_MODE 
                || local_init_operation == COREOTN_SS_SYSOTN_BYPASS_MODE) 
            {
                /* odu_rfrm stage N */
                *prbs_inst_ptr = COREOTN_FMF2_PRBS;
            } else 
            {
                /* odu_rfrm stage 1 */
                *prbs_inst_ptr = COREOTN_FMF1_PRBS;                                
            }
            break;
        case ODU_STRUCT_LEVEL_MO_ODU:
            if (local_init_operation == COREOTN_SS_SYSOTN_MODE) 
            {
                /* odu_rfrm stage 3a */
                *prbs_inst_ptr = COREOTN_FO1_PRBS;
            } else 
            {
                /* odu_rfrm stage 2 */
                *prbs_inst_ptr = COREOTN_FMF2_PRBS;
            }
            break;
        case ODU_STRUCT_LEVEL_LO_ODU:
            if (local_init_operation == COREOTN_SS_SYSOTN_MODE 
                || local_init_operation == COREOTN_SS_SYSOTN_BYPASS_MODE) 
            {
                /* in sysotn mode, LO_ODU by the HO segment from LINE side */
                /* odu_rfrm stage 1 */
                *prbs_inst_ptr = COREOTN_FMF1_PRBS;                                

            } else
            {
                PMC_RETURN(COREOTN_ERR_INVALID_ARG);
            }
            break;
        case ODU_STRUCT_LEVEL_3B_ODU:
            PMC_RETURN(COREOTN_ERR_INVALID_PRBS_CFG_STG3A3B);
            break;
        case ODU_STRUCT_LEVEL_4_ODU:
            /* odu_rfrm stage 4 */
            *prbs_inst_ptr = COREOTN_FO2_PRBS;
            break;
        default:
            PMC_RETURN(COREOTN_ERR_INVALID_ARG);
        break;
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_struct_prbs_inst_get */

/*******************************************************************************
* odu_struct_rfrm_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the ODU_RFRM handle for a given ODU struct.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   *odu_rfrm_handle_t - pointer to the odu_rfrm handle 
*
* NOTES:
*
*******************************************************************************/
PUBLIC odu_rfrm_handle_t* odu_struct_rfrm_handle_get(coreotn_handle_t *coreotn_handle,
                                                     odu_struct_t *odu_struct)
{
    odu_rfrm_handle_t *local_odu_rfrm_ptr = NULL;
    coreotn_init_operation_t local_init_operation = coreotn_handle->var.init_operation;

    PMC_ENTRY();

    /* find the correct framer */
    switch (odu_struct->mem_ptr->odu_level)
    {
        case ODU_STRUCT_LEVEL_HO_ODU:

            if (local_init_operation == COREOTN_SS_SYSOTN_MODE 
                || local_init_operation == COREOTN_SS_SYSOTN_BYPASS_MODE)
            {
                local_odu_rfrm_ptr = coreotn_handle->odu_rfrm_stgN_handle;
            } else {
                /* odu_rfrm stage 1 */
                local_odu_rfrm_ptr = coreotn_handle->odu_rfrm_stg1_handle;
            }

        break;
        case ODU_STRUCT_LEVEL_MO_ODU:

            if (local_init_operation == COREOTN_SS_SYSOTN_MODE 
                || local_init_operation == COREOTN_SS_SYSOTN_BYPASS_MODE) 
            {
                local_odu_rfrm_ptr = coreotn_handle->odu_rfrm_stg3a_handle;
            } else {
                /* odu_rfrm stage 2 */
                local_odu_rfrm_ptr = coreotn_handle->odu_rfrm_stg2_handle;
            }

        break;
        case ODU_STRUCT_LEVEL_LO_ODU:
            if (local_init_operation == COREOTN_SS_SYSOTN_MODE 
                || local_init_operation == COREOTN_SS_SYSOTN_BYPASS_MODE) 
            {
                /* odu_rfrm stage 1 */
                local_odu_rfrm_ptr = coreotn_handle->odu_rfrm_stg1_handle;
            } else {
                /* odu_rfrm stage 3a */
                local_odu_rfrm_ptr = coreotn_handle->odu_rfrm_stg3a_handle;
            }
        break;
        case ODU_STRUCT_LEVEL_3B_ODU:
            /* odu_rfrm stage 3b */
            local_odu_rfrm_ptr = coreotn_handle->odu_rfrm_stg3b_handle;
        break;
        case ODU_STRUCT_LEVEL_4_ODU:
            /* odu_rfrm stage 4 */
            local_odu_rfrm_ptr = coreotn_handle->odu_rfrm_stg4_handle;
        break;
        case ODU_STRUCT_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC:
            /* odu_rfrm stage 2 */
            local_odu_rfrm_ptr = coreotn_handle->odu_rfrm_stg2_handle;
        break;
        default:
            PMC_ASSERT(FALSE, COREOTN_ERR_CODE_ASSERT, 0, 0);
        break;
    }

    PMC_RETURN(local_odu_rfrm_ptr);
} /* odu_struct_rfrm_handle_get */

/*******************************************************************************
* odu_struct_tfrm_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the ODU_TFRM handle for a given ODU struct.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   *odu_tfrm_handle_t   - pointer to the odu_tfrm_handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC odu_tfrm_handle_t* odu_struct_tfrm_handle_get(coreotn_handle_t *coreotn_handle,
                                                     odu_struct_t *odu_struct)
{
    odu_tfrm_handle_t *local_odu_tfrm_ptr = NULL;
    coreotn_init_operation_t local_init_operation = coreotn_handle->var.init_operation;

    PMC_ENTRY();

    /* find the correct framer */
    switch (odu_struct->mem_ptr->odu_level)
    {
        case ODU_STRUCT_LEVEL_HO_ODU:
            if (local_init_operation == COREOTN_SS_SYSOTN_MODE 
                || local_init_operation == COREOTN_SS_SYSOTN_BYPASS_MODE) 
            {
                local_odu_tfrm_ptr = coreotn_handle->odu_tfrm_stgN_handle;
            } else {
                /* odu_rfrm stage 1 */
                local_odu_tfrm_ptr = coreotn_handle->odu_tfrm_stg1_handle;
            }
        break;
        case ODU_STRUCT_LEVEL_MO_ODU:
            
            if (local_init_operation == COREOTN_SS_SYSOTN_MODE 
                || local_init_operation == COREOTN_SS_SYSOTN_BYPASS_MODE)
            {
                local_odu_tfrm_ptr = coreotn_handle->odu_tfrm_stg3a_handle;
            } else {
                /* odu_rfrm stage 2 */
                local_odu_tfrm_ptr = coreotn_handle->odu_tfrm_stg2_handle;
            }
        break;
        case ODU_STRUCT_LEVEL_LO_ODU:
            if (local_init_operation == COREOTN_SS_SYSOTN_MODE 
                || local_init_operation == COREOTN_SS_SYSOTN_BYPASS_MODE)
            {
                /* odu_rfrm stage 1 */
                local_odu_tfrm_ptr = coreotn_handle->odu_tfrm_stg1_handle;
            } else {
                /* odu_rfrm stage 3a */
                local_odu_tfrm_ptr = coreotn_handle->odu_tfrm_stg3a_handle;
            }
        break;
        case ODU_STRUCT_LEVEL_3B_ODU:
            /* odu_rfrm stage 3b */
            local_odu_tfrm_ptr = coreotn_handle->odu_tfrm_stg3b_handle;
        break;
        case ODU_STRUCT_LEVEL_4_ODU:
            /* odu_rfrm stage4 */
            local_odu_tfrm_ptr = coreotn_handle->odu_tfrm_stg4_handle;
        break;
        case ODU_STRUCT_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC:
            /* odu_rfrm stage 1 */
            local_odu_tfrm_ptr = coreotn_handle->odu_tfrm_stg2_handle;
        break;
        default:
            PMC_ASSERT(FALSE, COREOTN_ERR_CODE_ASSERT, 0, 0);
        break;
    }

    PMC_RETURN(local_odu_tfrm_ptr);
} /* odu_struct_tfrm_handle_get */



/*******************************************************************************
* odu_struct_dmx_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the ODTU_DMX handle for a given ODU struct.
*   This can only be called for ODU_STRUCT_LEVEL_HO or ODU_STRUCT_LEVEL_MO
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   *odtu_dmx_handle_t  - pointer to the ODTU_DMX handle
*
* NOTES:
*
*******************************************************************************/
PRIVATE odtu_dmx_handle_t* odu_struct_dmx_handle_get(coreotn_handle_t *coreotn_handle,
                                                     odu_struct_t *odu_struct)
{
    odtu_dmx_handle_t *local_odtu_dmx_ptr = NULL;
    coreotn_init_operation_t local_init_operation = coreotn_handle->var.init_operation;

    PMC_ENTRY();


    /* error checking */
    if(odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_HO_ODU && odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_MO_ODU){
        PMC_ASSERT(FALSE, COREOTN_ERR_CODE_ASSERT, 0, 0);
    }

    /* find the correct framer */
    switch (odu_struct->mem_ptr->odu_level)
    {
        case ODU_STRUCT_LEVEL_HO_ODU:

            if(local_init_operation == COREOTN_SS_SYSOTN_MODE || local_init_operation == COREOTN_SS_SYSOTN_BYPASS_MODE) {
                local_odtu_dmx_ptr = coreotn_handle->odtu_dmx2_handle;
            } else {
                local_odtu_dmx_ptr = coreotn_handle->odtu_dmx1_handle;
            }

        break;
        case ODU_STRUCT_LEVEL_MO_ODU:
            if(local_init_operation == COREOTN_SS_SYSOTN_MODE || local_init_operation == COREOTN_SS_SYSOTN_BYPASS_MODE) {
                PMC_ASSERT(FALSE, COREOTN_ERR_CODE_ASSERT, 0, 0);
            } else {
                local_odtu_dmx_ptr = coreotn_handle->odtu_dmx2_handle;
            }
        break;
        default:
            PMC_ASSERT(FALSE, COREOTN_ERR_CODE_ASSERT, 0, 0);
        break;
    }

    PMC_RETURN(local_odtu_dmx_ptr);
} /* odu_struct_dmx_handle_get */


/*******************************************************************************
* odu_struct_mux_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the ODTU_MUX handle for a given ODU struct.
*   This can only be called for ODU_STRUCT_LEVEL_HO or ODU_STRUCT_LEVEL_MO
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   *odu_struct         - pointer to ODU_STRUCT to be operated on
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   *odtu_mux_handle_t  - pointer to the ODTU_MUX handle
*
* NOTES:
*
*******************************************************************************/
PRIVATE odtu_mux_handle_t* odu_struct_mux_handle_get(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct)
{
    odtu_mux_handle_t *local_odtu_mux_ptr = NULL;
    coreotn_init_operation_t local_init_operation = coreotn_handle->var.init_operation;

    PMC_ENTRY();


    /* error checking */
    if(odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_HO_ODU && odu_struct->mem_ptr->odu_level != ODU_STRUCT_LEVEL_MO_ODU){
        PMC_ASSERT(FALSE, COREOTN_ERR_CODE_ASSERT, 0, 0);
    }

    /* find the correct framer */
    switch (odu_struct->mem_ptr->odu_level)
    {
        case ODU_STRUCT_LEVEL_HO_ODU:

            if(local_init_operation == COREOTN_SS_SYSOTN_MODE || local_init_operation == COREOTN_SS_SYSOTN_BYPASS_MODE) {
                local_odtu_mux_ptr = coreotn_handle->odtu_mux2_handle;
            } else {
                local_odtu_mux_ptr = coreotn_handle->odtu_mux1_handle;
            }

        break;
        case ODU_STRUCT_LEVEL_MO_ODU:
            if(local_init_operation == COREOTN_SS_SYSOTN_MODE || local_init_operation == COREOTN_SS_SYSOTN_BYPASS_MODE) {
                PMC_ASSERT(FALSE, COREOTN_ERR_CODE_ASSERT, 0, 0);
            } else {
                local_odtu_mux_ptr = coreotn_handle->odtu_mux2_handle;
            }
        break;
        default:
            PMC_ASSERT(FALSE, COREOTN_ERR_CODE_ASSERT, 0, 0);
        break;
    }

    PMC_RETURN(local_odtu_mux_ptr);
} /*  odu_struct_mux_handle_get */

/*******************************************************************************
* odu_struct_otn_odu_dplm_to_ssf_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Controls consequential actions asociated with DPLM
*
* INPUTS:
*   *coreotn_handle - pointer to COREOTN handle instance
*   *odu_struct     - pointer to ODU framer channel handle to be operated on 
*   target          - configuration mode. See odu_struct_target_t 
*   enable          - 1 : enable DPLM to SSF consequential action
*                     0 : disable DPLM to SSF consequential action
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_otn_odu_dplm_to_ssf_set(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    odu_struct_target_t target,
                                                    BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *local_dmx_handle;
    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);
   
    if(target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE) {
        

          local_dmx_handle = odu_struct_dmx_handle_get(coreotn_handle,
                                                       odu_struct);

          result = ohfs_remove_map_ssf_set(local_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                           odu_struct->switch_data.oduk_data.channel,
                                           OHFS_REMOVE_MAP_DPLM_TO_SSF,
                                           (UINT32)enable);
          /* set in ODU struct */
          if (PMC_SUCCESS == result)
          {
              odu_struct->mem_ptr->dplm_to_ssf = enable;
          }
    } else {

        PMC_RETURN(COREOTN_ERR_NO_SW_CONTENT);


    }
        
    PMC_RETURN(result);
} /* odu_struct_otn_odu_dplm_to_ssf_set */

/*******************************************************************************
* odu_struct_otn_odu_dplm_to_ssf_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Gets consequential action associated with DPLM.
*
* INPUTS:
*   *coreotn_handle - pointer to COREOTN handle instance
*   *odu_struct     - pointer to ODU framer channel handle to be operated on 
*   ext_mode        - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
*                     DPLM is a HO based config, so you must provide the
*                     HO chnl_ptr
*
* OUTPUTS:
*   enable          - 1 : DPLM to SSF consequential action is enabled
*                     0 : DPLM to SSF consequential action is disabled
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_otn_odu_dplm_to_ssf_get(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    odu_struct_ext_mode_t ext_mode,
                                                    BOOL8 *enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *local_dmx_handle;
    PMC_ENTRY();
    
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);


    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
          local_dmx_handle = odu_struct_dmx_handle_get(coreotn_handle,
                                                       odu_struct);
          result = ohfs_remove_map_ssf_get(local_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                           odu_struct->switch_data.oduk_data.channel,
                                           OHFS_REMOVE_MAP_DPLM_TO_SSF,
                                           (UINT32*)enable);

    } 
    else {
        *enable = odu_struct->mem_ptr->dplm_to_ssf;
    }
    
        
    PMC_RETURN(result);
} /* odu_struct_otn_odu_dplm_to_ssf_get */

/*******************************************************************************
* odu_struct_otn_odu_dloomfi_to_ssf_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Controls consequential actions asociated with DLOOMFI
*
* INPUTS:
*   *coreotn_handle - pointer to COREOTN handle instance
*   *odu_struct     - pointer to ODU framer channel handle to be operated on 
*   target          - configuration mode. See odu_struct_target_t 
*   enable          - 1 : enable DLOOMFI to SSF consequential action
*                     0 : disable DLOOMFI to SSF consequential action
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_otn_odu_dloomfi_to_ssf_set(coreotn_handle_t *coreotn_handle,
                                                       odu_struct_t *odu_struct,
                                                       odu_struct_target_t target,
                                                       BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *local_dmx_handle;
    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    
    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE) 
    {
        local_dmx_handle = odu_struct_dmx_handle_get(coreotn_handle,
                                                     odu_struct);
        result = ohfs_remove_map_ssf_set(local_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                         odu_struct->switch_data.oduk_data.channel,
                                         OHFS_REMOVE_MAP_DLOOMFI_TO_SSF,
                                         (UINT32)enable);

        if (PMC_SUCCESS == result)
        {
            odu_struct->mem_ptr->dloomfi_to_ssf = enable;
        }
    } 
    else 
    {
        odu_struct->mem_ptr->dloomfi_to_ssf = enable;
    }
   
    PMC_RETURN(result);
} /* odu_struct_otn_odu_dloomfi_to_ssf_set */

/*******************************************************************************
* odu_struct_otn_odu_dloomfi_to_ssf_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Gets consequential action associated with DLOOMFI.
*
* INPUTS:
*   *coreotn_handle - pointer to COREOTN handle instance
*   *odu_struct     - pointer to ODU framer channel handle to be operated on 
*   ext_mode        - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
*                     DLOOMFI is a HO based config, so you must provide the
*                     HO chnl_ptr
*
* OUTPUTS:
*   enable          - 1 : DLOOMFI to SSF consequential action is enabled
*                     0 : DLOOMFI to SSF consequential action is disabled
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_otn_odu_dloomfi_to_ssf_get(coreotn_handle_t *coreotn_handle,
                                                       odu_struct_t *odu_struct,
                                                       odu_struct_ext_mode_t ext_mode,
                                                       BOOL8 *enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *local_dmx_handle;
    PMC_ENTRY();
    
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);

    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {
        local_dmx_handle = odu_struct_dmx_handle_get(coreotn_handle,
                                                     odu_struct);
        result = ohfs_remove_map_ssf_get(local_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                         odu_struct->switch_data.oduk_data.channel,
                                         OHFS_REMOVE_MAP_DLOOMFI_TO_SSF,
                                         (UINT32*)enable);
    } 
    else 
    {   
        /* retrieve from odu struct */
        *enable = odu_struct->mem_ptr->dloomfi_to_ssf;   
    }
            
    PMC_RETURN(result);
} /* odu_struct_otn_odu_dplm_to_ssf_get */

/*******************************************************************************
* odu_struct_otn_odu_dmsim_to_ssf_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Controls consequential actions asociated with DMSIM. This is a global
*   configuration that applies to all channels.
*
* INPUTS:
*   *coreotn_handle - pointer to COREOTN handle instance
*   *odu_struct     - pointer to ODU framer channel handle to be operated on 
*   target          - configuration mode. See odu_struct_target_t 
*   enable          - 1 : enable DMSIM to SSF consequential action
*                     0 : disable DMSIM to SSF consequential action
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_otn_odu_dmsim_to_ssf_set(coreotn_handle_t *coreotn_handle,
                                                     odu_struct_t *odu_struct,
                                                     odu_struct_target_t target,
                                                     BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *local_dmx_handle;

    PMC_ENTRY();
    
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);
   
    if (target == ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE) 
    {
        local_dmx_handle = odu_struct_dmx_handle_get(coreotn_handle,
                                                     odu_struct);

        result = sts_sw_map_dmsim_to_ssf_set(local_dmx_handle->sts_sw_handle,
                                             enable);

        if (PMC_SUCCESS == result)
        {
            odu_struct->mem_ptr->dmsim_to_ssf = enable;
        }
    } 
    else {
        /* set in odu struct */
        odu_struct->mem_ptr->dmsim_to_ssf = enable;
    }    
    
    PMC_RETURN(result);
} /* odu_struct_otn_odu_dmsim_to_ssf_set */

/*******************************************************************************
* odu_struct_otn_odu_dmsim_to_ssf_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Gets consequential action associated with DMSIM.
*
* INPUTS:
*   *coreotn_handle - pointer to COREOTN handle instance
*   *odu_struct     - pointer to ODU framer channel handle to be operated on 
*   ext_mode        - extraction mode of the requested field.
*                         See odu_struct_ext_mode_t.
*
*                     DMSIM is a HO based config, so you must provide the
*                     HO chnl_ptr
*
* OUTPUTS:
*   enable          - 1 : DMSIM to SSF consequential action is enabled
*                     0 : DMSIM to SSF consequential action is disabled
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_otn_odu_dmsim_to_ssf_get(coreotn_handle_t *coreotn_handle,
                                                     odu_struct_t *odu_struct,
                                                     odu_struct_ext_mode_t ext_mode,
                                                     BOOL8 *enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *local_dmx_handle;
    PMC_ENTRY();
   
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);


    if (ext_mode == ODU_STRUCT_EXTRACT_FROM_DEVICE)
    {   
        local_dmx_handle = odu_struct_dmx_handle_get(coreotn_handle,
                                                     odu_struct);

        result = sts_sw_map_dmsim_to_ssf_get(local_dmx_handle->sts_sw_handle,
                                             enable);
    } 
    else 
    {
        *enable = odu_struct->mem_ptr->dmsim_to_ssf;   
    }
    
        
    PMC_RETURN(result);
} /* odu_struct_otn_odu_dmsim_to_ssf_get */

/*******************************************************************************
* odu_struct_otn_opu_payload_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This function retrieves the payload format stored in context to indicate
*   if the ODU channel has NULL payload. This function should be called after 
*   the datapath has been brought up. 
* 
*
* INPUTS:
*   *coreotn_handle - pointer to COREOTN handle instance
*   *odu_struct     - pointer to ODU framer channel handle to be operated on 
*
* OUTPUTS:
*   *payload_format - enum for payload format. See util_global_odu_line_payload_t.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_otn_opu_payload_get(coreotn_handle_t *coreotn_handle,
                                                odu_struct_t *odu_struct,
                                                util_global_odu_line_payload_t *payload_format)
{
    PMC_ERROR result = PMC_SUCCESS;
    odtu_mux_handle_t *mux_handle_ptr;
    BOOL8 null_frame_en = 0;
    
    PMC_ENTRY();
   
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);
   
    mux_handle_ptr = odu_struct_mux_handle_get(coreotn_handle, odu_struct);
    
    odtu_mux_null_chan_mode_get(mux_handle_ptr,
                                odu_struct->switch_data.oduk_data.channel,
                                &null_frame_en);
    
    
    if (UTIL_GLOBAL_ODU_NULL == odu_struct->mem_ptr->payload_format)
    {
        if (null_frame_en == TRUE)
        {
            *payload_format = UTIL_GLOBAL_ODU_NULL; 
        }
        else 
        {
            result = COREOTN_ERR_NULL_PAYLOAD_INCOHERENT_CTXT;
        }
    }
    else 
    {
        *payload_format = odu_struct->mem_ptr->payload_format;
    } 
         
   
    PMC_RETURN(result);
} /*  odu_struct_otn_opu_payload_get */

/*******************************************************************************
* odu_struct_rx_odu_pm_tcmi_layer_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This function retrieves the current layering information of receive path
*   in ODU layer. 
* 
* INPUTS:
*   *coreotn_handle - pointer to COREOTN handle instance
*   *odu_struct     - pointer to ODU framer channel handle to be operated on 
*
* OUTPUTS:
*   *tcm_layers     - pointer to storage of TCM layer hierarchy on this channel.
*   *num_layers     - total number of layers on this channel.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_rx_odu_pm_tcmi_layer_get(coreotn_handle_t *coreotn_handle,
                                                     odu_struct_t *odu_struct,
                                                     odu_struct_odu_frm_layer_struct_t (*tcm_layers)[ODU_SINK_MAX_PM_TCM_LAYERS],
                                                     UINT32 *num_layers)
{                                              
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_handle_t *local_odu_rfrm_handle = NULL;
    odu_rfrm_layer_struct_t odu_rfrm_layers[ODU_SINK_MAX_PM_TCM_LAYERS];
    UINT32 i;
    
    PMC_ENTRY();
    
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);  
   
    /* initialize odu_tfrm_layers array */
    for (i = 0; i < ODU_SINK_MAX_PM_TCM_LAYERS; i++)
    {
        odu_rfrm_layers[i].pool_id = ODU_STRUCT_UNASSIGNED_POOL_ID;
        odu_rfrm_layers[i].pm_tcm_num = ODU_RFRM_MESSAGE_DONT_CARE;
        odu_rfrm_layers[i].tcm_mode = ODU_RFRM_PM_TCM_START;
        odu_rfrm_layers[i].maint_sig = 0;
    }
    
    local_odu_rfrm_handle = odu_struct_rfrm_handle_get(coreotn_handle, odu_struct);
    
    result = odu_rfrm_layer_hierarchy_get(local_odu_rfrm_handle,
                                          odu_struct->switch_data.oduk_data.channel,
                                          &odu_rfrm_layers,
                                          num_layers);
    
    if (PMC_SUCCESS == result)
    {
        for (i = 0; i < ODU_SINK_MAX_PM_TCM_LAYERS; i++)
        {
            if (odu_rfrm_layers[i].pm_tcm_num != ODU_RFRM_MESSAGE_TYPE_PM)
            {
                (*tcm_layers)[i].pool_id = odu_rfrm_layers[i].pool_id;
            }
            else 
            {
                (*tcm_layers)[i].pool_id = ODU_STRUCT_UNASSIGNED_POOL_ID;
            }
            (*tcm_layers)[i].pm_tcm_num = (odu_struct_odu_frm_pm_tcm_id_t)odu_rfrm_layers[i].pm_tcm_num;
            (*tcm_layers)[i].tcm_mode = (odu_struct_pm_tcm_mode_t)odu_rfrm_layers[i].tcm_mode;
        }
    }

    PMC_RETURN(result);                                         
} /* odu_struct_rx_odu_pm_tcmi_layer_get */     

/*******************************************************************************
* odu_struct_tx_odu_pm_tcmi_layer_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This function retrieves the current layering information of transmit path
*   in ODU layer. 
* 
* INPUTS:
*   *coreotn_handle - pointer to COREOTN handle instance
*   *odu_struct     - pointer to ODU framer channel handle to be operated on 
*
* OUTPUTS:
*   *tcm_layers     - pointer to storage of TCM layer hierarchy on this channel.
*   *num_layers     - total number of layers on this channel.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tx_odu_pm_tcmi_layer_get(coreotn_handle_t *coreotn_handle,
                                                     odu_struct_t *odu_struct,
                                                     odu_struct_odu_frm_layer_struct_t (*tcm_layers)[ODU_SOURCE_MAX_PM_TCM_LAYERS],
                                                     UINT32 *num_layers)
{                                              
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_handle_t *local_odu_tfrm_handle = NULL;
    odu_tfrm_layer_struct_t odu_tfrm_layers[ODU_SOURCE_MAX_PM_TCM_LAYERS];
    UINT32 i;
    /*UINT32 idx = 0;*/
    
    PMC_ENTRY();
    
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);  
  
    
    local_odu_tfrm_handle = odu_struct_tfrm_handle_get(coreotn_handle, odu_struct);
    
    if (NULL != local_odu_tfrm_handle)
    {
        result = odu_tfrm_layer_hierarchy_get(local_odu_tfrm_handle,
                                              odu_struct->switch_data.oduk_data.channel,
                                              odu_tfrm_layers,
                                              num_layers);
    }
    else 
    {
        result = COREOTN_ERR_INVALID_ARG;
    }                                          
    
    if (PMC_SUCCESS == result)
    {
        for (i = 0; i < ODU_SOURCE_MAX_PM_TCM_LAYERS; i++)
        {
            if (odu_tfrm_layers[i].pm_tcm_num != ODU_TFRM_MESSAGE_TYPE_PM)
            {
                (*tcm_layers)[i].pool_id = odu_tfrm_layers[i].pool_id;
            }
            else 
            {
                (*tcm_layers)[i].pool_id = ODU_STRUCT_UNASSIGNED_POOL_ID;
            }
            (*tcm_layers)[i].pm_tcm_num = (odu_struct_odu_frm_pm_tcm_id_t)odu_tfrm_layers[i].pm_tcm_num;
            (*tcm_layers)[i].tcm_mode =  ODU_STRUCT_PM_TCM_TT_OPERATIONAL;
        }
    }

    PMC_RETURN(result);                                         
} /* odu_struct_tx_odu_pm_tcmi_layer_get */   

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* odu_struct_tcm_pool_rsc_clean
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function cleans a tcm_pool_rsc struct.
*
* INPUTS:
*   *odu_struct - pointer to odu_strcut that contains tcm_pool_rsc structs.
*   *dest       - pointer to destination tcm_pool_rsc struct
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
PRIVATE void odu_struct_tcm_pool_rsc_clean(odu_struct_t *odu_struct, 
                                           odu_struct_tcm_pool_rsc_t *dest)
{
    
    PMC_ENTRY();
    
    dest->tcm_instance         = LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES;   /* this seems necessary when the only config data we have is odu_struct FRM APIs are written with argument pm_tcm_num to abstract away implementation detail of pool_id */
    dest->tcm_pool_id_rx       = ODU_STRUCT_UNASSIGNED_POOL_ID;
    dest->tcm_pool_id_tx       = ODU_STRUCT_UNASSIGNED_POOL_ID;
    dest->deg_m                = 0;
    dest->deg_th               = 0;
    dest->tim_mon              = 0;
    dest->tim_act_dis          = 0;
    dest->ltc_to_ais           = 0;
                                         
    dest->bei_biae_force_val   = 0;
    dest->bdi_force_val        = 0;
    dest->stat_force_val       = 0;
    dest->dmt_src              = 0;
    dest->dmt_val              = 0;
    dest->tx_layer_num         = 0xFF;
    dest->rx_layer_num         = 0xFF;
    
    PMC_RETURN();
} /* odu_struct_tcm_pool_rsc_copy */    
#endif /* DOXYGEN_PUBLIC_ONLY */

/*******************************************************************************
* odu_struct_tx_maint_sig_dummy_rfrm_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the dummy RFRM maintenance signal.
*
*   This dummy maintenance signal is required under two scenerios:
*      1) When provisioning OCI with an ODUKSW connection
*      2) When there an open ODUKSW connection
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance
*   odu_level           - See odu_struct_odu_level_t
*   chnl_id             - Channel id
*   enable              - enable 
*
* OUTPUTS:
*   NONE
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_tx_maint_sig_dummy_rfrm_cfg(coreotn_handle_t *coreotn_handle,
                                                        odu_struct_odu_level_t odu_level,
                                                        UINT32 chnl_id,
                                                        BOOL8 enable)
{
    PMC_ERROR   result = PMC_SUCCESS;

    UINT8 maint_sig;
    BOOL8 fmf1_sysotn;

    UINT8 *prev_maint_sig_ptr;
    BOOL8 *dummy_maint_sig_ptr;
    odu_rfrm_handle_t *local_rfrm_handle;

    PMC_ENTRY();


    fmf1_sysotn = (odu_level == ODU_STRUCT_LEVEL_LO_ODU &&
                    (coreotn_handle->var.init_operation == COREOTN_SS_SYSOTN_MODE
                      || coreotn_handle->var.init_operation == COREOTN_SS_SYSOTN_BYPASS_MODE));
    
    
    if (fmf1_sysotn)
    {
        local_rfrm_handle = coreotn_handle->odu_rfrm_stg2_handle;
        prev_maint_sig_ptr = &(coreotn_handle->var.prev_fmf1_sysotn_rfrm_maint_sig[chnl_id]);
        dummy_maint_sig_ptr = &(coreotn_handle->var.dummy_fmf1_sysotn_maint_sig[chnl_id]);
    }
    else
    {
        local_rfrm_handle = coreotn_handle->odu_rfrm_stg3b_handle;
        prev_maint_sig_ptr = &(coreotn_handle->var.prev_fo1_rfrm_maint_sig[chnl_id]);
        dummy_maint_sig_ptr = &(coreotn_handle->var.dummy_fo1_maint_sig[chnl_id]);
    }

    if (PMC_SUCCESS == result)
    {
        odu_rfrm_chnl_maint_sig_get(local_rfrm_handle,
                                    chnl_id,
                                    &maint_sig);
    }      

    if (PMC_SUCCESS == result)
    {
        if (TRUE == enable) 
        {
            /*only store dummy_fo1_maint_sig if no dummy maintenance signal is applied yet (in the event
            that a maintenance signal is applied at more than 1 point in the TX path */
            if (*dummy_maint_sig_ptr == FALSE)
            {
                *prev_maint_sig_ptr = maint_sig;
            }

            *dummy_maint_sig_ptr = TRUE;         
            
            /*if there is no maintenance signal applied, provision a dummy maintenance signal*/
            if (PMC_SUCCESS == result && maint_sig == 0)
            {
                result = odu_rfrm_chnl_maint_sig_cfg(local_rfrm_handle,
                                                     chnl_id,
                                                     1);
            }            
        } 
        else 
        {           
            /* Only restore prev_fo1_rfrm_maint_sig if a dummy maintenance signal is currently active */
            if (*dummy_maint_sig_ptr)
            {
                *dummy_maint_sig_ptr = FALSE;

                if (maint_sig != *prev_maint_sig_ptr)
                {
                    result = odu_rfrm_chnl_maint_sig_cfg(local_rfrm_handle,
                                                         chnl_id,
                                                         *prev_maint_sig_ptr);
                }
            }
        }       
    }

    PMC_RETURN(result);
} /* odu_struct_tx_maint_sig_dummy_rfrm_cfg */



/*******************************************************************************
* odu_struct_otn_odu_cm_min_max_reprov
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Reprovisions the CM_MIN/CM_MAX values on an already operational channel.
*
* INPUTS:
*   *coreotn_handle - pointer to COREOTN handle instance
*   *odu_struct     - pointer to ODU framer channel handle to be operated on 
*   use_custom_ppm      - TRUE : use ppm_offset parameter to calculate
*                                CM_MIN and CM_MAX
*                         FALSE : use ppm ofsets defined in G.709
*   ppm_offset          - If use_custom_ppm is TRUE, this is
*                         the user defined +/- ppm collar
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_struct_otn_odu_cm_min_max_reprov(coreotn_handle_t *coreotn_handle,
                                                      odu_struct_t *odu_struct,
                                                      BOOL use_custom_ppm,
                                                      UINT32 ppm_offset)
{
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *local_dmx_handle;
    UINT32 chnl_id;

    PMC_ENTRY();
   
    PMC_ASSERT(coreotn_handle!=NULL, COREOTN_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(odu_struct != NULL, COREOTN_ERR_ODU_STRUCT_INVALID_PTR, 0, 0);


    
    local_dmx_handle = odu_struct_dmx_handle_get(coreotn_handle,
                                                 odu_struct);



    chnl_id = odu_struct->switch_data.oduk_data.channel;

    if(odu_struct->mem_ptr->odu_level == ODU_STRUCT_LEVEL_MO_ODU)
    {
        if(coreotn_handle->var.rx_mo_chnl_ctxt[chnl_id].lo_odu_rate != UTIL_GLOBAL_ODUFLEX_CBR &&
           coreotn_handle->var.rx_mo_chnl_ctxt[chnl_id].lo_odu_rate != UTIL_GLOBAL_ODUFLEX_GFP)
        {
            /* There is only support for CBR/GFP flex */
            PMC_RETURN(COREOTN_ERR_INVALID_ARG);
        }

    }
    else if(odu_struct->mem_ptr->odu_level == ODU_STRUCT_LEVEL_LO_ODU)
    {
        if(coreotn_handle->var.rx_lo_chnl_ctxt[chnl_id].lo_odu_rate != UTIL_GLOBAL_ODUFLEX_CBR &&
           coreotn_handle->var.rx_lo_chnl_ctxt[chnl_id].lo_odu_rate != UTIL_GLOBAL_ODUFLEX_GFP)
        {
            /* There is only support for CBR/GFP flex */
            PMC_RETURN(COREOTN_ERR_INVALID_ARG);
        }
    }
    else
    {
        /* Can only call this on a MO or LO channel */
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if(odu_struct->mem_ptr->odu_level == ODU_STRUCT_LEVEL_MO_ODU)
    {

        result = odtu_dmx_cm_min_max_reprov(local_dmx_handle,
                                            chnl_id,
                                            coreotn_handle->var.rx_mo_chnl_ctxt[chnl_id].lo_odu_rate,
                                            coreotn_handle->var.rx_mo_chnl_ctxt[chnl_id].cbr_flex_rate,
                                            coreotn_handle->var.rx_mo_chnl_ctxt[chnl_id].odu_container,
                                            (util_global_mapping_mode_t)coreotn_handle->var.rx_mo_chnl_ctxt[chnl_id].mapping_mode,
                                            coreotn_handle->var.rx_mo_chnl_ctxt[chnl_id].odu_flex_rate,
                                            use_custom_ppm,
                                            ppm_offset);
    }
    else
    {
        result = odtu_dmx_cm_min_max_reprov(local_dmx_handle,
                                            chnl_id,
                                            coreotn_handle->var.rx_lo_chnl_ctxt[chnl_id].lo_odu_rate,
                                            coreotn_handle->var.rx_lo_chnl_ctxt[chnl_id].cbr_flex_rate,
                                            coreotn_handle->var.rx_lo_chnl_ctxt[chnl_id].odu_container,
                                            (util_global_mapping_mode_t)coreotn_handle->var.rx_lo_chnl_ctxt[chnl_id].mapping_mode,
                                            coreotn_handle->var.rx_lo_chnl_ctxt[chnl_id].odu_flex_rate,
                                            use_custom_ppm,
                                            ppm_offset);
    }
        
    PMC_RETURN(result);
} /* odu_struct_otn_odu_cm_min_max_reprov */


/*
** End of file
*/

