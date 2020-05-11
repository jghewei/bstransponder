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
*     This file contains the internal coversion API for DIGI Adapter usage.
*     Customer needn't use this in normal case.  
*
*   NOTES:
*     None.
*
*******************************************************************************/
/*
** Include Files
*/
#include "digi_adpt.h"
#ifdef PMC_COMEXPRESS
#include <sys/time.h>
#endif

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
digi_cfg_container_t *adpt_cfg_handle[ADPT_DEV_NUMBER_MAX] = 
{
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

UINT32 adpt_dev_id[ADPT_DEV_NUMBER_MAX] = 
{
    ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID,
    ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID,
    ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID,
    ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID,
    ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID,
    ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID,
    ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID,
    ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID,
    ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID,
    ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID,
    ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID,
    ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID,
    ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID,
    ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID,
    ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID,
    ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID,
    ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID,
    ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID,
    ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID,
    ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID, ADPT_INVALID_DEV_ID,
};

const char *adpt_dev_name[ADPT_DIGI_DEV_MAX] = 
{
    "DIGI_0",
    "DIGI_1",
    "DIGI_2",
    "DIGI_3",
    "DIGI_4",
    "DIGI_5",
    "DIGI_6",
    "DIGI_7",
};

/*
** Public Functions
*/
/*******************************************************************************
* adpt_handle_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates DIGI adapter container pointer according to device identity.
*
* INPUTS:
*   dev_id             - device identity.
*   warm_init          - TRUE: warm initialization.
*                        FALSE: cold initialization.
*   *file_name         - file name.
*
* OUTPUTS:
*   **adpt_handle      - point to DIGI container handle.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_handle_create(UINT32 dev_id, 
                                 BOOL warm_init,
                                 char *file_name,
                                 digi_cfg_container_t **adpt_handle)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 id_pos = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(adpt_handle);
    if (ADPT_INVALID_DEV_ID == dev_id)
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* check if dev_id has been existed */
    for (id_pos = 0; id_pos < ADPT_DEV_NUMBER_MAX; id_pos++)
    {
        if (dev_id == adpt_dev_id[id_pos])
        {
            ADPT_ERROR("Input the same dev_id[%d].\r\n", dev_id);
            PMC_RETURN(PMC_ERR_FAIL);
        }
    }

    /* create configuration container */
    for (id_pos = 0; id_pos < ADPT_DEV_NUMBER_MAX; id_pos++)
    {
        if (ADPT_INVALID_DEV_ID == adpt_dev_id[id_pos])
        {
            /* allocate resource for adapter container */
            rc = adpt_handle_allocate(dev_id,
                                      sizeof(digi_cfg_container_t), 
                                      (void**)&adpt_cfg_handle[id_pos]);
            ADPT_RETURN_VERIFY(rc);

            if (NULL == adpt_cfg_handle[id_pos])
            {
                ADPT_ERROR("The adpt_handle is NULL! \r\n");
                PMC_RETURN(PMC_ERR_FAIL);
            }

            if (warm_init)
            {
                /* restore database */
                rc = adpt_database_restore((char*)file_name, 
                                           sizeof(digi_cfg_container_t), 
                                           adpt_cfg_handle[id_pos]);
            }
            else
            {
                /* reset Adapter handle */
                PMC_MEMSET(adpt_cfg_handle[id_pos], 0, sizeof(digi_cfg_container_t));
            }
            ADPT_RETURN_VERIFY(rc);

            adpt_cfg_handle[id_pos]->pmon_data   = (digi_pmon_data_t*) PMC_CALLOC(sizeof(digi_pmon_data_t));
            adpt_cfg_handle[id_pos]->pmon_backup = (digi_pmon_data_t*) PMC_CALLOC(sizeof(digi_pmon_data_t));
            /* reset initialization state */
            adpt_cfg_handle[id_pos]->init_state  = DIGI_INIT_RESET;
            /* get the suitable pointer */
            *adpt_handle = adpt_cfg_handle[id_pos];
            /* record dev_id */
            adpt_dev_id[id_pos] = dev_id;
            break;
        }
    }
    
    PMC_RETURN(rc);
}/* adpt_handle_create */


/*******************************************************************************
* adpt_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the DIGI Adapter handle container pointer for specific usage.
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   **adpt_handle      - pointer to DIGI container handle.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_handle_get(UINT32 dev_id, digi_cfg_container_t **adpt_handle)
{
    PMC_ERROR rc = PMC_ERR_FAIL;
    UINT32 id_pos = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(adpt_handle);
    if (ADPT_INVALID_DEV_ID == dev_id)
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* get container pointer based on dev_id */
    for (id_pos = 0; id_pos < ADPT_DEV_NUMBER_MAX; id_pos++)
    {
        if (dev_id == adpt_dev_id[id_pos])
        {
            if ((NULL != adpt_cfg_handle[id_pos]) 
                && (DIGI_INIT_RESET != adpt_cfg_handle[id_pos]->init_state))
            {
                rc = PMC_SUCCESS;
            }

            /* get the correct pointer */
            *adpt_handle = adpt_cfg_handle[id_pos];
            /* break */
            break;
        }
    }

    PMC_RETURN(rc);
}/* adpt_handle_get */


/*******************************************************************************
* adpt_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes the DIGI Adapter handle container.
*
* INPUTS:
*   *adpt_handle       - pointer to DIGI container handle.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_handle_init(digi_cfg_container_t *adpt_handle)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 pos = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(adpt_handle);

    /* reset MAPOTN channel */
    for (index = 0; index < DIGI_MAPPER_NUM_SECONDARY_GROUPS; index++)
    {
        adpt_handle->map_lane[index] = DIGI_DEFAULT_CHNL_ID;
    }

    /* default datapath operation mode */
    adpt_handle->dp_mode   = DIGI_DP_NORMAL;

    /* reset RCP */
    adpt_handle->ri_rcp_en = FALSE;
    adpt_handle->di_rcp_en = FALSE;

    for (index = 0; index < DIGI_NUM_CHNL_MAX; index++)
    {
        /* reset handle */
        adpt_handle->traffic_info[index].odu_handle  = NULL;
        adpt_handle->traffic_info[index].otu_handle  = NULL;
        adpt_handle->traffic_info[index].enet_handle = NULL;
        adpt_handle->traffic_info[index].cbr_handle  = NULL;
        adpt_handle->traffic_info[index].map_handle  = NULL;
        adpt_handle->traffic_info[index].ilkn_handle = NULL;

        /* reset channel ID */
        adpt_handle->traffic_info[index].chnl_id    = DIGI_DEFAULT_CHNL_ID;
        adpt_handle->traffic_info[index].ho_chnl_id = DIGI_DEFAULT_CHNL_ID;

        /* default TCM priority */
        for (pos = 0; pos < 4; pos++)
        {
            /* sink position */
            if ((0 == pos) || (2 == pos))
            {
                adpt_handle->traffic_info[index].tcm_pri[pos][0] = DIGI_TCM_PRI_6;
                adpt_handle->traffic_info[index].tcm_pri[pos][1] = DIGI_TCM_PRI_5;
                adpt_handle->traffic_info[index].tcm_pri[pos][2] = DIGI_TCM_PRI_4;
                adpt_handle->traffic_info[index].tcm_pri[pos][3] = DIGI_TCM_PRI_3;
                adpt_handle->traffic_info[index].tcm_pri[pos][4] = DIGI_TCM_PRI_2;
                adpt_handle->traffic_info[index].tcm_pri[pos][5] = DIGI_TCM_PRI_1;
                adpt_handle->traffic_info[index].tcm_pri[pos][6] = DIGI_TCM_PRI_0;
            }
            else
            {
                adpt_handle->traffic_info[index].tcm_pri[pos][0] = DIGI_TCM_PRI_0;
                adpt_handle->traffic_info[index].tcm_pri[pos][1] = DIGI_TCM_PRI_1;
                adpt_handle->traffic_info[index].tcm_pri[pos][2] = DIGI_TCM_PRI_2;
                adpt_handle->traffic_info[index].tcm_pri[pos][3] = DIGI_TCM_PRI_3;
                adpt_handle->traffic_info[index].tcm_pri[pos][4] = DIGI_TCM_PRI_4;
                adpt_handle->traffic_info[index].tcm_pri[pos][5] = DIGI_TCM_PRI_5;
                adpt_handle->traffic_info[index].tcm_pri[pos][6] = DIGI_TCM_PRI_6;
            }
        }

        /* default TCM pool ID */
        PMC_MEMSET((void*)&(adpt_handle->traffic_info[index].tcm_pool_id), 0, 
                   sizeof(adpt_handle->traffic_info[index].tcm_pool_id));

        /* default PRBS */
        adpt_handle->traffic_info[index].prbs_mode   = LAST_DIGI_ENET_PRBS;
        adpt_handle->traffic_info[index].prbs_enable = FALSE;
        adpt_handle->traffic_info[index].prbs_chid   = 0;

        /* switch information */
        adpt_handle->traffic_info[index].cpb_src_index  = 0;
        adpt_handle->traffic_info[index].oduk_src_index = 0;
        adpt_handle->traffic_info[index].map_sw_info    = 0;
    }

    PMC_RETURN(rc);
}/* adpt_handle_init */


/*******************************************************************************
* adpt_handle_query
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the DIGI Adapter handle container pointer for specific usage based
*   on digi handler.
*
* INPUTS:
*  *digi_handle        - pointer to digi handle instance.
*
* OUTPUTS:
*   **adpt_handle      - pointer to DIGI container handle.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_handle_query(digi_handle_t *digi_handle, digi_cfg_container_t **adpt_handle)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 id_pos = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(digi_handle);

    /* get container pointer based on dev_id */
    for (id_pos = 0; id_pos < ADPT_DEV_NUMBER_MAX; id_pos++)
    {
        if ((NULL != adpt_cfg_handle[id_pos]) 
            && (digi_handle == adpt_cfg_handle[id_pos]->digi_handle))
        {
            /* get the correct pointer */
            *adpt_handle = adpt_cfg_handle[id_pos];
            /* return successfully */
            PMC_RETURN(rc);
        }
    }

    PMC_RETURN(PMC_ERR_FAIL);
}/* adpt_handle_query */


/*******************************************************************************
* adpt_handle_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys DIGI adapter container pointer according to device identity.
*
* INPUTS:
*   dev_id             - device identity.
*   **adpt_handle      - point to DIGI container handle.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_handle_destroy(UINT32 dev_id, digi_cfg_container_t **adpt_handle)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 id_pos = 0;
    UINT32 index = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(adpt_handle);
    if (ADPT_INVALID_DEV_ID == dev_id)
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* find the existed dev_id */
    for (id_pos = 0; id_pos < ADPT_DEV_NUMBER_MAX; id_pos++)
    {
        if (dev_id == adpt_dev_id[id_pos])
        {
            /* reset dev_id */
            adpt_dev_id[id_pos] = ADPT_INVALID_DEV_ID;

            /* free memory for Adapter container */
            PMC_FREE(&((*adpt_handle)->pmon_data));
            PMC_FREE(&((*adpt_handle)->pmon_backup));
            if (DIGI_INIT_DONE != (*adpt_handle)->init_state)
            {
                for (index = 0; index < DIGI_NUM_CHNL_MAX; index++)
                {
                     PMC_FREE(&((*adpt_handle)->restart_info[index]));
                }
            }

            /* free Adapter container */
            adpt_handle_free(dev_id, (void**)adpt_handle);

            /* reset pointer as NULL */
            *adpt_handle = NULL;
            adpt_cfg_handle[id_pos] = NULL;

            /* break */
            break;
        }
    }
    
    PMC_RETURN(rc);
}/* adpt_handle_destroy */


/*******************************************************************************
* adpt_ctxt_handle_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Updates DIGI SDK handle in DIGI Adapter.
*
* INPUTS:
*   *adpt_handle       - point to DIGI Adapter handle.
*   *dev_info          - point to device information.
*   *digi_handle       - point to DIGI SDK handle.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_ctxt_handle_update(digi_cfg_container_t *adpt_handle,
                                      digi_dev_info_t *dev_info,
                                      digi_handle_t *digi_handle)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    PMC_PTRDIFF offset = 0;
    digi_resource_handle_summary_t *digi_resource = NULL;
    UINT32 i = 0;
    UINT32 j = 0;
    BOOL exist = FALSE;
    UINT32 map_sw_info = 0;
    BOOL map_sw_recover = FALSE;
    digi_handle_id_t handle_id = DIGI_CHANNEL_HANDLE_ID_LINEOTN_T;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(adpt_handle);
    ADPT_PARA_VERIFY(dev_info);
    ADPT_PARA_VERIFY(digi_handle);

    /* virtual address offset between previous context and new one */
    offset = (PMC_PTRDIFF)digi_handle - (PMC_PTRDIFF)adpt_handle->digi_handle;

    /* DIGI level handle update */
    adpt_handle->digi_handle = digi_handle;

    /* force the datapath operation mode as DIGI_DP_NORMAL */
    adpt_handle->dp_mode = DIGI_DP_NORMAL;

    /* DIGI SDK resource recovery */
    digi_resource = (digi_resource_handle_summary_t*)PMC_CALLOC(sizeof(digi_resource_handle_summary_t));
    rc = digi_resource_state_get(adpt_handle->digi_handle, digi_resource);
    if (PMC_SUCCESS != rc)
    {
        PMC_FREE(&digi_resource);
        PMC_RETURN(rc);
    }

    for (index = 0; index < DIGI_SERDES_XFI_LANES_MAX; index++)
    {
        if (NULL != adpt_handle->line_serdes[index])
        {
            adpt_handle->line_serdes[index] = (digi_serdes_port_t*)((PMC_PTRDIFF)adpt_handle->line_serdes[index] + offset);
        }

        exist = FALSE;
        for (i = 0; i < DIGI_XFI_PIN_NUM; i++)
        {
            if (adpt_handle->line_serdes[index] == digi_resource->line_serdes_cfg[i].port_ctxt_ptr)
            {
                exist = TRUE;
                break;
            }                
        }

        for (i = 0; i < DIGI_SFI51_INTF_NUM; i++)
        {
            if (adpt_handle->line_serdes[index] == digi_resource->sfi51_serdes_cfg[i].port_ctxt_ptr)
            {
                exist = TRUE;
                break;
            }                
        }

        if (!exist)
        {
            ADPT_TRACE("resource destroy -- index[%d] line_serdes[%p].\r\n", 
                       index, adpt_handle->line_serdes[index]);

            adpt_handle->line_serdes[index] = NULL;
        }
    }

    for (index = 0; index < DIGI_SERDES_SYS_LANES_MAX; index++)
    {
        if (NULL != adpt_handle->sys_serdes[index])
        {
            adpt_handle->sys_serdes[index] = (digi_serdes_port_t*)((PMC_PTRDIFF)adpt_handle->sys_serdes[index] + offset);
        }

        exist = FALSE;
        for (i = 0; i < DIGI_XFI_PIN_NUM; i++)
        {
            if (adpt_handle->sys_serdes[index] == digi_resource->sys_serdes_cfg[i].port_ctxt_ptr)
            {
                exist = TRUE;
                break;
            }                
        }

        if (!exist)
        {
            ADPT_TRACE("resource destroy -- index[%d] sys_serdes[%p].\r\n", 
                       index, adpt_handle->sys_serdes[index]);

            adpt_handle->sys_serdes[index] = NULL;
        }
    }

    if (NULL != adpt_handle->ilkn_1_ctxt)
    {
        adpt_handle->ilkn_1_ctxt = (digi_ilkn_link_t*)((PMC_PTRDIFF)adpt_handle->ilkn_1_ctxt + offset);
    }
    if (NULL != adpt_handle->ilkn_2_ctxt)
    {
        adpt_handle->ilkn_2_ctxt = (digi_ilkn_link_t*)((PMC_PTRDIFF)adpt_handle->ilkn_2_ctxt + offset);
    }

    /* TP level handle update */
    for (index = 0; index < DIGI_NUM_CHNL_MAX; index++)
    {
        if (!(adpt_handle->traffic_info[index].enable))
        {
            continue;
        }

        /* clear datapath enable flag */
        adpt_handle->traffic_info[index].enable         = DIGI_FLAG_CLEAR(adpt_handle->traffic_info[index].enable);
        adpt_handle->traffic_info[index].map_enable     = DIGI_FLAG_CLEAR(adpt_handle->traffic_info[index].map_enable);
        adpt_handle->traffic_info[index].cpb_src_index  = DIGI_SWT_FLAG_CLEAR(adpt_handle->traffic_info[index].cpb_src_index);
        adpt_handle->traffic_info[index].oduk_src_index = DIGI_SWT_FLAG_CLEAR(adpt_handle->traffic_info[index].oduk_src_index);
        adpt_handle->traffic_info[index].map_sw_info    = DIGI_CPB_FLAG_CLEAR(adpt_handle->traffic_info[index].map_sw_info);

        if (NULL != adpt_handle->traffic_info[index].otu_handle)
        {
            adpt_handle->traffic_info[index].otu_handle = (digi_otn_server_chnl_t*)((PMC_PTRDIFF)adpt_handle->traffic_info[index].otu_handle + offset);

            exist = FALSE;
            for (i = 0; i < DIGI_XFI_PIN_NUM; i++)
            {
                if ((adpt_handle->traffic_info[index].otu_handle == digi_resource->line_serdes_cfg[i].chnl_handle_ptr)
                    || (adpt_handle->traffic_info[index].otu_handle == digi_resource->sys_serdes_cfg[i].chnl_handle_ptr))
                {
                    exist = TRUE;
                    break;
                }                
            }

            for (i = 0; i < DIGI_SFI51_INTF_NUM; i++)
            {
                if (adpt_handle->traffic_info[index].otu_handle == digi_resource->sfi51_serdes_cfg[i].chnl_handle_ptr)
                {
                    exist = TRUE;
                    break;
                }                
            }

            if (!exist)
            {
                ADPT_TRACE("resource destroy -- chnl_id[0x%x] index[%d] otu_handle[%p].\r\n", 
                           adpt_handle->traffic_info[index].chnl_id, index,
                           adpt_handle->traffic_info[index].otu_handle);

                rc = adpt_chnl_index_destroy(adpt_handle, adpt_handle->traffic_info[index].chnl_id, index);
                if (PMC_SUCCESS != rc)
                {
                    PMC_FREE(&digi_resource);
                    PMC_RETURN(rc);
                }
            }
        }

        if (NULL != adpt_handle->traffic_info[index].odu_handle)
        {
            adpt_handle->traffic_info[index].odu_handle = (digi_otn_odu_chnl_t*)((PMC_PTRDIFF)adpt_handle->traffic_info[index].odu_handle + offset);

            exist = FALSE;
            switch (adpt_handle->traffic_info[index].odu_stage)
            {
                case DIGI_ODU_STRUCT_LEVEL_HO_ODU:
                    /* line side */
                    /* system side */
                    for (i = 0; i < DIGI_OTN_SERVER_HO_CHNL_MAX; i++)
                    {
                        if ((adpt_handle->traffic_info[index].odu_handle == digi_resource->line_otn_ho_container_cfg[i].chnl_handle_ptr)
                             || (adpt_handle->traffic_info[index].odu_handle == digi_resource->sys_otn_ho_container_cfg[i].chnl_handle_ptr))
                        {
                            exist = TRUE;
                            break;
                        }
                    }
                    break;

                case DIGI_ODU_STRUCT_LEVEL_MO_ODU:
                    /* line side */
                    /* system side */
                    for (i = 0; i < DIGI_OTN_SERVER_MO_CHNL_MAX; i++)
                    {
                        if ((adpt_handle->traffic_info[index].odu_handle == digi_resource->line_otn_mo_container_cfg[i].chnl_handle_ptr)
                            || (adpt_handle->traffic_info[index].odu_handle == digi_resource->sys_otn_mo_container_cfg[i].chnl_handle_ptr))
                        {
                            exist = TRUE;
                            break;
                        }
                    }
                    break;

                case DIGI_ODU_STRUCT_LEVEL_LO_ODU:
                    /* line side */
                    for (i = 0; i < DIGI_OTN_SERVER_LO_CHNL_MAX; i++)
                    {
                        if (adpt_handle->traffic_info[index].odu_handle == digi_resource->line_otn_lo_container_cfg[i].chnl_handle_ptr)
                        {
                            exist = TRUE;
                            break;
                        }
                    }
                    break;

                default:
                    PMC_FREE(&digi_resource);
                    PMC_RETURN(PMC_ERR_FAIL);
                    break;
            }

            if (!exist)
            {
                ADPT_TRACE("resource destroy -- chnl_id[0x%x] index[%d] odu_handle[%p].\r\n", 
                           adpt_handle->traffic_info[index].chnl_id, index,
                           adpt_handle->traffic_info[index].odu_handle);

                rc = adpt_chnl_index_destroy(adpt_handle, adpt_handle->traffic_info[index].chnl_id, index);
                if (PMC_SUCCESS != rc)
                {
                    PMC_FREE(&digi_resource);
                    PMC_RETURN(rc);
                }
            }
        }

        if (NULL != adpt_handle->traffic_info[index].enet_handle)
        {
            adpt_handle->traffic_info[index].enet_handle = (digi_enet_client_chnl_t*)((PMC_PTRDIFF)adpt_handle->traffic_info[index].enet_handle + offset);

            exist = FALSE;
            for (i = 0; i < DIGI_XFI_PIN_NUM; i++)
            {
                if ((adpt_handle->traffic_info[index].enet_handle == digi_resource->line_serdes_cfg[i].chnl_handle_ptr)
                    || (adpt_handle->traffic_info[index].enet_handle == digi_resource->sys_serdes_cfg[i].chnl_handle_ptr))
                {
                    exist = TRUE;
                    break;
                }                
            }

            for (i = 0; i < DIGI_SFI51_INTF_NUM; i++)
            {
                if (adpt_handle->traffic_info[index].enet_handle == digi_resource->sfi51_serdes_cfg[i].chnl_handle_ptr)
                {
                    exist = TRUE;
                    break;
                }                
            }

            if (!exist)
            {
                ADPT_TRACE("resource destroy -- chnl_id[0x%x] index[%d] enet_handle[%p].\r\n", 
                           adpt_handle->traffic_info[index].chnl_id, index,
                           adpt_handle->traffic_info[index].enet_handle);

                rc = adpt_chnl_index_destroy(adpt_handle, adpt_handle->traffic_info[index].chnl_id, index);
                if (PMC_SUCCESS != rc)
                {
                    PMC_FREE(&digi_resource);
                    PMC_RETURN(rc);
                }
            }
        }

        if (NULL != adpt_handle->traffic_info[index].cbr_handle)
        {
            adpt_handle->traffic_info[index].cbr_handle = (digi_cbr_client_chnl_t*)((PMC_PTRDIFF)adpt_handle->traffic_info[index].cbr_handle + offset);

            exist = FALSE;
            for (i = 0; i < DIGI_XFI_PIN_NUM; i++)
            {
                if ((adpt_handle->traffic_info[index].cbr_handle == digi_resource->line_serdes_cfg[i].chnl_handle_ptr)
                    || (adpt_handle->traffic_info[index].cbr_handle == digi_resource->sys_serdes_cfg[i].chnl_handle_ptr))
                {
                    exist = TRUE;
                    break;
                }                
            }

            for (i = 0; i < DIGI_SFI51_INTF_NUM; i++)
            {
                if (adpt_handle->traffic_info[index].cbr_handle == digi_resource->sfi51_serdes_cfg[i].chnl_handle_ptr)
                {
                    exist = TRUE;
                    break;
                }                
            }

            if (!exist)
            {
                ADPT_TRACE("resource destroy -- chnl_id[0x%x] index[%d] cbr_handle[%p].\r\n", 
                           adpt_handle->traffic_info[index].chnl_id, index,
                           adpt_handle->traffic_info[index].cbr_handle);

                rc = adpt_chnl_index_destroy(adpt_handle, adpt_handle->traffic_info[index].chnl_id, index);
                if (PMC_SUCCESS != rc)
                {
                    PMC_FREE(&digi_resource);
                    PMC_RETURN(rc);
                }
            }
        }

        if (NULL != adpt_handle->traffic_info[index].ilkn_handle)
        {
            adpt_handle->traffic_info[index].ilkn_handle = (digi_ilkn_chnl_t*)((PMC_PTRDIFF)adpt_handle->traffic_info[index].ilkn_handle + offset);

            exist = FALSE;
            for (i = 0; i < DIGI_ILKN_INTF_NUM; i++)
            {
                for (j = 0; j < DIGI_ILKN_CHNL_NUM; j++)
                {
                    if (adpt_handle->traffic_info[index].ilkn_handle == digi_resource->ilkn_chnl_cfg[i][j].ilkn_chnl_handle_ptr)
                    {
                        exist = TRUE;
                        break;
                    }                
                }

                if (exist)
                {
                    break;
                }
            }

            if (!exist)
            {
                ADPT_TRACE("resource destroy -- chnl_id[0x%x] index[%d] ilkn_handle[%p].\r\n", 
                           adpt_handle->traffic_info[index].chnl_id, index,
                           adpt_handle->traffic_info[index].ilkn_handle);

                rc = adpt_chnl_index_destroy(adpt_handle, adpt_handle->traffic_info[index].chnl_id, index);
                if (PMC_SUCCESS != rc)
                {
                    PMC_FREE(&digi_resource);
                    PMC_RETURN(rc);
                }
            }
        }

        if (NULL != adpt_handle->traffic_info[index].map_handle)
        {
            adpt_handle->traffic_info[index].map_handle = (digi_mapper_chnl_t*)((PMC_PTRDIFF)adpt_handle->traffic_info[index].map_handle + offset);

            exist = FALSE;
            for (i = 0; i < DIGI_MAPOTN_CHNL_MAX; i++)
            {
                if (adpt_handle->traffic_info[index].map_handle == digi_resource->mapper_cfg[i].mapper_handle_ptr)
                {
                    exist = TRUE;
                    break;
                }                
            }

            if (!exist)
            {
                ADPT_TRACE("resource destroy -- chnl_id[0x%x] index[%d] map_handle[%p].\r\n", 
                           adpt_handle->traffic_info[index].chnl_id, index,
                           adpt_handle->traffic_info[index].map_handle);

                adpt_handle->traffic_info[index].map_handle = NULL;
                adpt_handle->traffic_info[index].map_enable = FALSE;
                /* destroy Primary/Secondary */
                for (i = 0; i < DIGI_MAPPER_NUM_SECONDARY_GROUPS; i++)
                {
                    if (adpt_handle->traffic_info[index].chnl_id == adpt_handle->map_lane[i])
                    {
                        adpt_handle->map_lane[i] = DIGI_DEFAULT_CHNL_ID;
                    }
                }

                /* destroy ODUk SW/CPB */
                adpt_handle->traffic_info[index].oduk_src_index= 0;
                adpt_handle->traffic_info[index].map_sw_info    = 0;
                for (i = 0; i < DIGI_NUM_CHNL_MAX; i++)
                {
                    if (!(adpt_handle->traffic_info[i].enable))
                    {
                        continue;
                    }

                    if ((DIGI_SWT_EN_GET(adpt_handle->traffic_info[i].oduk_src_index))
                         && (index == DIGI_SWT_ID_GET(adpt_handle->traffic_info[i].oduk_src_index)))
                    {
                        adpt_handle->traffic_info[i].oduk_src_index = 0;
                    }
                }
            }

            if (0 != adpt_handle->traffic_info[index].map_sw_info)
            {
                map_sw_info = adpt_handle->traffic_info[index].map_sw_info;

                /* ENET/CBR/ILKN to MAPOTN */
                exist = (BOOL)digi_resource_is_channel_activated(adpt_handle->digi_handle,
                                                                 digi_resource,
                                                                 adpt_handle->traffic_info[index].map_handle,
                                                                 DIGI_CHANNEL_HANDLE_ID_MAPPER_T,
                                                                 FALSE);
                if (!exist && DIGI_SWT_MAP_EN_GET(map_sw_info))
                {
                    ADPT_TRACE("CPB destroy from CBR to MAP -- chnl_id[0x%x] index[%d] map_handle[%p].\r\n", 
                               adpt_handle->traffic_info[index].chnl_id, index,
                               adpt_handle->traffic_info[index].map_handle);

                    adpt_handle->traffic_info[index].map_sw_info = DIGI_SWT_MAP_SET(map_sw_info, FALSE, FALSE, DIGI_CPB_CBR_TO_MAP);
                }

                /* MAPOTN to ENET/CBR/ILKN */
                chnl_handle = NULL;
                if (NULL != adpt_handle->traffic_info[index].enet_handle)
                {
                    chnl_handle = adpt_handle->traffic_info[index].enet_handle;
                    handle_id   = DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T;
                }
                else if (NULL != adpt_handle->traffic_info[index].cbr_handle)
                {
                    chnl_handle = adpt_handle->traffic_info[index].cbr_handle;
                    handle_id   = DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T;
                }
                else if (NULL != adpt_handle->traffic_info[index].ilkn_handle)
                {
                    chnl_handle = adpt_handle->traffic_info[index].ilkn_handle;
                    handle_id   = DIGI_CHANNEL_HANDLE_ID_ILKN_1_T;
                }

                exist = (BOOL)digi_resource_is_channel_activated(adpt_handle->digi_handle,
                                                                 digi_resource,
                                                                 chnl_handle,
                                                                 handle_id,
                                                                 FALSE);
                if (!exist && DIGI_SWT_CBR_EN_GET(map_sw_info))
                {
                    ADPT_TRACE("CPB destroy from MAP to CBR -- chnl_id[0x%x] index[%d] map_handle[%p].\r\n", 
                               adpt_handle->traffic_info[index].chnl_id, index,
                               adpt_handle->traffic_info[index].map_handle);

                    adpt_handle->traffic_info[index].map_sw_info = DIGI_SWT_CBR_SET(map_sw_info, FALSE, FALSE, DIGI_CPB_MAP_TO_CBR);
                }

                /* workaround to recover CPB switch to MAPOTN for SW upgrade */
                map_sw_recover = TRUE;
            }
        }

        /* ODUk SW cross connection */
        if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[index].oduk_src_index))
        {
            chnl_handle = NULL;
            if (NULL != adpt_handle->traffic_info[index].map_handle)
            {
                chnl_handle = adpt_handle->traffic_info[index].map_handle;
                handle_id   = DIGI_CHANNEL_HANDLE_ID_MAPPER_T;
            }
            else if (NULL != adpt_handle->traffic_info[index].odu_handle)
            {
                chnl_handle = adpt_handle->traffic_info[index].odu_handle;
                handle_id   = DIGI_CHANNEL_HANDLE_ID_LO_ODU_T;
            }
            else if (NULL != adpt_handle->traffic_info[index].ilkn_handle)
            {
                chnl_handle = adpt_handle->traffic_info[index].ilkn_handle;
                handle_id   = DIGI_CHANNEL_HANDLE_ID_ILKN_1_T;
            }

            exist = (BOOL)digi_resource_is_channel_activated(adpt_handle->digi_handle,
                                                             digi_resource,
                                                             chnl_handle,
                                                             handle_id,
                                                             TRUE);
            if (!exist)
            {
                ADPT_TRACE("ODUk SW destroy -- chnl_id[0x%x] index[%d] chnl_handle[%p].\r\n", 
                           adpt_handle->traffic_info[index].chnl_id, index, chnl_handle);

                adpt_handle->traffic_info[index].oduk_src_index = 0;
            }
        }

        /* CPB cross connection */
        if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[index].cpb_src_index))
        {
            chnl_handle = NULL;
            if (NULL != adpt_handle->traffic_info[index].enet_handle)
            {
                chnl_handle = adpt_handle->traffic_info[index].enet_handle;
                handle_id   = DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T;
            }
            else if (NULL != adpt_handle->traffic_info[index].cbr_handle)
            {
                chnl_handle = adpt_handle->traffic_info[index].cbr_handle;
                handle_id   = DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T;
            }
            else if (NULL != adpt_handle->traffic_info[index].ilkn_handle)
            {
                chnl_handle = adpt_handle->traffic_info[index].ilkn_handle;
                handle_id   = DIGI_CHANNEL_HANDLE_ID_ILKN_1_T;
            }

            exist = (BOOL)digi_resource_is_channel_activated(adpt_handle->digi_handle,
                                                             digi_resource,
                                                             chnl_handle,
                                                             handle_id,
                                                             FALSE);
            if (!exist)
            {
                ADPT_TRACE("CPB destroy -- chnl_id[0x%x] index[%d] chnl_handle[%p].\r\n", 
                           adpt_handle->traffic_info[index].chnl_id, index, chnl_handle);

                adpt_handle->traffic_info[index].cpb_src_index = 0;
            }
        }
    }

    /* workaround to recover CPB switch to MAPOTN for SW upgrade */
    if (!map_sw_recover)
    {
        for (i = 0; i < DIGI_NUM_CHNL_MAX; i++)
        {
            if (!(adpt_handle->traffic_info[i].enable))
            {
                continue;
            }

            if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[i].oduk_src_index))
            {
                index = DIGI_SWT_ID_GET(adpt_handle->traffic_info[i].oduk_src_index);

                /* CBR/ENET to MAPOTN */
                if (NULL != adpt_handle->traffic_info[index].map_handle)
                {
                    map_sw_info = adpt_handle->traffic_info[index].map_sw_info;
                    adpt_handle->traffic_info[index].map_sw_info = DIGI_SWT_MAP_SET(map_sw_info, TRUE, FALSE, DIGI_CPB_CBR_TO_MAP);
                }

                /* MAPOTN to CBR/ENET */
                if (NULL != adpt_handle->traffic_info[i].map_handle)
                {
                    map_sw_info = adpt_handle->traffic_info[i].map_sw_info;
                    adpt_handle->traffic_info[i].map_sw_info = DIGI_SWT_CBR_SET(map_sw_info, TRUE, FALSE, DIGI_CPB_MAP_TO_CBR);
                }
            }
        }
    }

    /* free memory */
    PMC_FREE(&digi_resource);

    PMC_RETURN(rc);
}/* adpt_ctxt_handle_update */


/*******************************************************************************
* adpt_chnl_index_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates index of traffic_info based on new ODU channel identity.
*
* INPUTS:
*   *adpt_handle       - point to DIGI container handle.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *index_ptr         - index of traffic_info in the container.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_chnl_index_create(digi_cfg_container_t *adpt_handle,
                                    UINT32 chnl_id,
                                    UINT32 *index_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 i = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(adpt_handle);
    ADPT_PARA_VERIFY(index_ptr);

    /* chnl_id shouldn't be 0xFFFFFFFF or 0xFFFFFFFE */
    if ((DIGI_DEFAULT_CHNL_ID == chnl_id) || (DIGI_SWITCH_CHNL_ID == chnl_id))
    {
        /* return */
        PMC_RETURN(PMC_ERR_FAIL); 
    }

    /* check if there is a same one*/
    for (i = 0; i < DIGI_NUM_CHNL_MAX; i++)
    {
        if (chnl_id == adpt_handle->traffic_info[i].chnl_id)
        {
            /* return */
            PMC_RETURN(PMC_ERR_FAIL); 
        }
    }
                
    /* assign a new one */
    for (i = 0; i < DIGI_NUM_CHNL_MAX; i++)
    {
        if (DIGI_DEFAULT_CHNL_ID == adpt_handle->traffic_info[i].chnl_id)
        {
            *index_ptr = i;
            adpt_handle->traffic_info[i].chnl_id = chnl_id;
            /* return */
            PMC_RETURN(rc);
        }
    }

    PMC_RETURN(PMC_ERR_FAIL);
} /* adpt_chnl_index_create */


/*******************************************************************************
* adpt_chnl_index_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves index of traffic_info according to channel identity.
*
* INPUTS:
*   *adpt_handle       - point to DIGI container handle.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *index_ptr         - index of traffic_info in the container.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_chnl_index_get(digi_cfg_container_t *adpt_handle,
                                  UINT32 chnl_id,
                                  UINT32 *index_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 i = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(adpt_handle);
    ADPT_PARA_VERIFY(index_ptr);

    /* check if there is a same one*/
    for (i = 0; i < DIGI_NUM_CHNL_MAX; i++)
    {
        if (chnl_id == adpt_handle->traffic_info[i].chnl_id)
        {
            *index_ptr = i;
            /* return */
            PMC_RETURN(rc); 
        }
    }
    
    PMC_RETURN(PMC_ERR_FAIL);
} /* adpt_chnl_index_get */


/*******************************************************************************
* adpt_chnl_index_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys index of traffic_info based on new ODU channel identity.
*
* INPUTS:
*   *adpt_handle       - point to DIGI container handle.
*   chnl_id            - channel identity.
*   index              - index of traffic_info in the container.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_chnl_index_destroy(digi_cfg_container_t *adpt_handle,
                                     UINT32 chnl_id,
                                     UINT32 index)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 pos = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(adpt_handle);

    if ((index >= DIGI_NUM_CHNL_MAX) || (chnl_id != adpt_handle->traffic_info[index].chnl_id))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }    

    /* reset channel ID */
    adpt_handle->traffic_info[index].chnl_id     = DIGI_DEFAULT_CHNL_ID;
    adpt_handle->traffic_info[index].ho_chnl_id  = DIGI_DEFAULT_CHNL_ID;

    /* reset handle */
    adpt_handle->traffic_info[index].odu_handle  = NULL;
    adpt_handle->traffic_info[index].otu_handle  = NULL;
    adpt_handle->traffic_info[index].enet_handle = NULL;
    adpt_handle->traffic_info[index].cbr_handle  = NULL;
    adpt_handle->traffic_info[index].map_handle  = NULL;
    adpt_handle->traffic_info[index].ilkn_handle = NULL;

    /* default TCM priority */
    for (pos = 0; pos < 4; pos++)
    {
        /* sink position */
        if ((0 == pos) || (2 == pos))
        {
            adpt_handle->traffic_info[index].tcm_pri[pos][0] = DIGI_TCM_PRI_6;
            adpt_handle->traffic_info[index].tcm_pri[pos][1] = DIGI_TCM_PRI_5;
            adpt_handle->traffic_info[index].tcm_pri[pos][2] = DIGI_TCM_PRI_4;
            adpt_handle->traffic_info[index].tcm_pri[pos][3] = DIGI_TCM_PRI_3;
            adpt_handle->traffic_info[index].tcm_pri[pos][4] = DIGI_TCM_PRI_2;
            adpt_handle->traffic_info[index].tcm_pri[pos][5] = DIGI_TCM_PRI_1;
            adpt_handle->traffic_info[index].tcm_pri[pos][6] = DIGI_TCM_PRI_0;
        }
        else
        {
            adpt_handle->traffic_info[index].tcm_pri[pos][0] = DIGI_TCM_PRI_0;
            adpt_handle->traffic_info[index].tcm_pri[pos][1] = DIGI_TCM_PRI_1;
            adpt_handle->traffic_info[index].tcm_pri[pos][2] = DIGI_TCM_PRI_2;
            adpt_handle->traffic_info[index].tcm_pri[pos][3] = DIGI_TCM_PRI_3;
            adpt_handle->traffic_info[index].tcm_pri[pos][4] = DIGI_TCM_PRI_4;
            adpt_handle->traffic_info[index].tcm_pri[pos][5] = DIGI_TCM_PRI_5;
            adpt_handle->traffic_info[index].tcm_pri[pos][6] = DIGI_TCM_PRI_6;
        }
    }

    /* default ODUk configuration */
    adpt_handle->traffic_info[index].mapping_type  = DIGI_OTN_MAP_AMP;
    adpt_handle->traffic_info[index].oduk_type     = DIGI_ODUK_ODU0;
    adpt_handle->traffic_info[index].payload_type  = DIGI_ODU_UNCHANNELIZED;
    adpt_handle->traffic_info[index].oduflex_rate  = 0;
    adpt_handle->traffic_info[index].odu_stage     = LAST_DIGI_ODU_STRUCT_LEVEL;
    adpt_handle->traffic_info[index].enable        = 0;
    adpt_handle->traffic_info[index].otn_dir       = (digi_otn_dir_t)0;
    for (pos = 0; pos < 3; pos++)
    {
        adpt_handle->traffic_info[index].rx_ts_mask[pos] = 0;
        adpt_handle->traffic_info[index].tx_ts_mask[pos] = 0;
    }
    /* default TCM pool ID */
    PMC_MEMSET((void*)&(adpt_handle->traffic_info[index].tcm_pool_id), 0, 
               sizeof(adpt_handle->traffic_info[index].tcm_pool_id));

    /* default PRBS */
    adpt_handle->traffic_info[index].prbs_mode   = LAST_DIGI_ENET_PRBS;
    adpt_handle->traffic_info[index].prbs_enable = FALSE;
    adpt_handle->traffic_info[index].prbs_chid   = 0;

    /* switch information */
    adpt_handle->traffic_info[index].cpb_src_index  = 0;
    adpt_handle->traffic_info[index].oduk_src_index = 0;
    adpt_handle->traffic_info[index].map_sw_info    = 0;

    PMC_RETURN(rc);
} /* adpt_chnl_index_destroy */


/*******************************************************************************
* adpt_rst_chnl_index_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates index of restart_info based on the new ODU channel identity during
*   crash / warm restart.
*
* INPUTS:
*   *adpt_handle       - point to DIGI container handle.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *index_ptr         - index of restart_info in the container.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_rst_chnl_index_create(digi_cfg_container_t *adpt_handle,
                                       UINT32 chnl_id,
                                       UINT32 *index_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 i = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(adpt_handle);
    ADPT_PARA_VERIFY(index_ptr);

    if (DIGI_INIT_RESTART != adpt_handle->init_state)
    {
        PMC_RETURN(PMC_ERR_FAIL);
    }

    /* check if there is a same one*/
    for (i = 0; i < DIGI_NUM_CHNL_MAX; i++)
    {
        if (chnl_id == adpt_handle->restart_info[i]->chnl_id)
        {
            /* return */
            PMC_RETURN(PMC_ERR_FAIL); 
        }
    }

    /* assign a new one */
    for (i = 0; i < DIGI_NUM_CHNL_MAX; i++)
    {
        if (DIGI_DEFAULT_CHNL_ID == adpt_handle->restart_info[i]->chnl_id)
        {
            *index_ptr = i;
            adpt_handle->restart_info[i]->chnl_id = chnl_id;
            /* return */
            PMC_RETURN(rc);
        }
    }

    PMC_RETURN(PMC_ERR_FAIL);
} /* adpt_rst_chnl_index_create */


/*******************************************************************************
* adpt_rst_chnl_index_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves index of restart_info or traffic_info according to channel identity
*   during crash or warm restart.
*
* INPUTS:
*   *adpt_handle       - point to DIGI container handle.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *index_ptr         - index of restart_info in the container.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_rst_chnl_index_get(digi_cfg_container_t *adpt_handle,
                                     UINT32 chnl_id,
                                     UINT32 *index_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 i = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(adpt_handle);
    ADPT_PARA_VERIFY(index_ptr);

    if ((DIGI_INIT_RESTART != adpt_handle->init_state)
         && (DIGI_INIT_RESTORE != adpt_handle->init_state))
    {
        PMC_RETURN(PMC_ERR_FAIL);
    }

    /* check channel ID in restart_info */
    for (i = 0; i < DIGI_NUM_CHNL_MAX; i++)
    {
        if (chnl_id == adpt_handle->restart_info[i]->chnl_id)
        {
            *index_ptr = i;
            /* return */
            PMC_RETURN(rc); 
        }
    }

    PMC_RETURN(PMC_ERR_FAIL);
} /* adpt_rst_chnl_index_get */


/*******************************************************************************
* adpt_rst_chnl_index_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys index of restart_info based on the new ODU channel identity.
*
* INPUTS:
*   *adpt_handle       - point to DIGI container handle.
*   chnl_id            - channel identity.
*   index              - index of restart_info in the container.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_rst_chnl_index_destroy(digi_cfg_container_t *adpt_handle,
                                        UINT32 chnl_id,
                                        UINT32 index)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 pos = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(adpt_handle);

    if (DIGI_INIT_RESTART != adpt_handle->init_state)
    {
        PMC_RETURN(PMC_ERR_FAIL);
    }

    if ((index >= DIGI_NUM_CHNL_MAX) || (chnl_id != adpt_handle->restart_info[index]->chnl_id))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }    

    /* reset channel ID */
    adpt_handle->restart_info[index]->chnl_id     = DIGI_DEFAULT_CHNL_ID;
    adpt_handle->restart_info[index]->ho_chnl_id  = DIGI_DEFAULT_CHNL_ID;

    /* reset handle */
    adpt_handle->restart_info[index]->odu_handle  = NULL;
    adpt_handle->restart_info[index]->otu_handle  = NULL;
    adpt_handle->restart_info[index]->enet_handle = NULL;
    adpt_handle->restart_info[index]->cbr_handle  = NULL;
    adpt_handle->restart_info[index]->map_handle  = NULL;
    adpt_handle->restart_info[index]->ilkn_handle = NULL;

    /* default TCM priority */
    for (pos = 0; pos < 4; pos++)
    {
        /* sink position */
        if ((0 == pos) || (2 == pos))
        {
            adpt_handle->restart_info[index]->tcm_pri[pos][0] = DIGI_TCM_PRI_6;
            adpt_handle->restart_info[index]->tcm_pri[pos][1] = DIGI_TCM_PRI_5;
            adpt_handle->restart_info[index]->tcm_pri[pos][2] = DIGI_TCM_PRI_4;
            adpt_handle->restart_info[index]->tcm_pri[pos][3] = DIGI_TCM_PRI_3;
            adpt_handle->restart_info[index]->tcm_pri[pos][4] = DIGI_TCM_PRI_2;
            adpt_handle->restart_info[index]->tcm_pri[pos][5] = DIGI_TCM_PRI_1;
            adpt_handle->restart_info[index]->tcm_pri[pos][6] = DIGI_TCM_PRI_0;
        }
        else
        {
            adpt_handle->restart_info[index]->tcm_pri[pos][0] = DIGI_TCM_PRI_0;
            adpt_handle->restart_info[index]->tcm_pri[pos][1] = DIGI_TCM_PRI_1;
            adpt_handle->restart_info[index]->tcm_pri[pos][2] = DIGI_TCM_PRI_2;
            adpt_handle->restart_info[index]->tcm_pri[pos][3] = DIGI_TCM_PRI_3;
            adpt_handle->restart_info[index]->tcm_pri[pos][4] = DIGI_TCM_PRI_4;
            adpt_handle->restart_info[index]->tcm_pri[pos][5] = DIGI_TCM_PRI_5;
            adpt_handle->restart_info[index]->tcm_pri[pos][6] = DIGI_TCM_PRI_6;
        }
    }

    /* default ODUk configuration */
    adpt_handle->restart_info[index]->mapping_type  = DIGI_OTN_MAP_AMP;
    adpt_handle->restart_info[index]->oduk_type     = DIGI_ODUK_ODU0;
    adpt_handle->restart_info[index]->payload_type  = DIGI_ODU_UNCHANNELIZED;
    adpt_handle->restart_info[index]->oduflex_rate  = 0;
    adpt_handle->restart_info[index]->odu_stage     = LAST_DIGI_ODU_STRUCT_LEVEL;
    adpt_handle->restart_info[index]->enable        = FALSE;
    adpt_handle->restart_info[index]->otn_dir       = (digi_otn_dir_t)0;
    for (pos = 0; pos < 3; pos++)
    {
        adpt_handle->restart_info[index]->rx_ts_mask[pos] = 0;
        adpt_handle->restart_info[index]->tx_ts_mask[pos] = 0;
    }
    /* default TCM pool ID */
    PMC_MEMSET((void*)&(adpt_handle->restart_info[index]->tcm_pool_id), 0, 
               sizeof(adpt_handle->restart_info[index]->tcm_pool_id));

    /* default PRBS */
    adpt_handle->restart_info[index]->prbs_mode   = LAST_DIGI_ENET_PRBS;
    adpt_handle->restart_info[index]->prbs_enable = FALSE;
    adpt_handle->restart_info[index]->prbs_chid   = 0;

    /* switch information */
    adpt_handle->restart_info[index]->cpb_src_index  = 0;
    adpt_handle->restart_info[index]->oduk_src_index = 0;
    adpt_handle->restart_info[index]->map_sw_info    = 0;

    PMC_RETURN(rc);
} /* adpt_rst_chnl_index_destroy */


/*******************************************************************************
* adpt_device_name_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves DIGI device name.
*
* INPUTS:
*   digi_dev           - DIGI device ID.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   DIGI device name.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_device_name_get(UINT32 digi_dev)
{
    if (digi_dev < ADPT_DIGI_DEV_MAX)
    {
        return adpt_dev_name[digi_dev];
    }
    else
    {
        /* default device name */
        return adpt_dev_name[0];
    }
} /* adpt_device_name_get */


/*******************************************************************************
* adpt_oduflex_ts_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves oduflex tributary slot number from oduk. 
*
* INPUTS:
*   oduk_type          - oduk type. 
*   ts_mask            - tributary slot information.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   oduflex rate.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 adpt_oduflex_ts_get(digi_oduk_t oduk_type, UINT32 ts_mask[3])
{
    UINT32 ts_number = 0;
    UINT32 i = 0;

    /* get tributary slot number */
    for (i = 0; i < 96; i++)
    {
        if (ts_mask[i/32] & (1 << (i % 32)))
        {
            ts_number++;
        }
    }
    
    return ts_number;
} /* adpt_oduflex_ts_get */


/*******************************************************************************
* adpt_mapper_para_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves mapper paramters from mapping mode. 
*
* INPUTS:
*   *adpt_handle       - Adapter handler.
*   traffic_mode       - traffic mode.
*   mapping_mode       - mapping mode. 
*
* OUTPUTS:
*   *enet_pmon_ptr     - pointer to ethernet performance monitor control.
*   *odu_type_ptr      - pointer to oduk type.
*   *num_lanes_ptr     - pointer to number of physical lanes.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_mapper_para_get(digi_cfg_container_t *adpt_handle,
                                    digi_traffic_mode_t traffic_mode,
                                    digi_mapotn_mode_t mapping_mode,
                                    BOOL *enet_pmon_ptr,
                                    digi_oduk_t *odu_type_ptr,
                                    UINT8 *num_lanes_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(enet_pmon_ptr);
    ADPT_PARA_VERIFY(odu_type_ptr);
    ADPT_PARA_VERIFY(num_lanes_ptr);
    ADPT_PARA_VERIFY(adpt_handle);

    switch (mapping_mode)
    {
        case DIGI_MAPOTN_ODU0_PACKET_GFP:
            if (DIGI_LINE_10GE_MAC == traffic_mode)
            {
                *enet_pmon_ptr = adpt_handle->dev_info->tx_enet_pmon;
            }
            else
            {
                *enet_pmon_ptr = FALSE;
            }
            *odu_type_ptr  = DIGI_ODUK_ODU0;
            *num_lanes_ptr = 1;
            break;
            
        case DIGI_MAPOTN_ODU1_PACKET_GFP:
            if (DIGI_LINE_10GE_MAC == traffic_mode)
            {
                *enet_pmon_ptr = adpt_handle->dev_info->tx_enet_pmon;
            }
            else
            {
                *enet_pmon_ptr = FALSE;
            }
            *odu_type_ptr  = DIGI_ODUK_ODU1;
            *num_lanes_ptr = 1;
            break;
            
        case DIGI_MAPOTN_ODU1E_PACKET_GFP:
            *enet_pmon_ptr = FALSE;           
            *odu_type_ptr  = DIGI_ODUK_ODU1E;
            *num_lanes_ptr = 1;
            break;
            
        case DIGI_MAPOTN_ODU1F_PACKET_GFP:
            *enet_pmon_ptr = FALSE;           
            *odu_type_ptr  = DIGI_ODUK_ODU1F;
            *num_lanes_ptr = 1;
            break;
            
        case DIGI_MAPOTN_ODU2_PACKET_GFP:
            if (DIGI_LINE_10GE_MAC == traffic_mode)
            {
                *enet_pmon_ptr = adpt_handle->dev_info->tx_enet_pmon;
            }
            else
            {
                *enet_pmon_ptr = FALSE;
            }
            *odu_type_ptr  = DIGI_ODUK_ODU2;
            *num_lanes_ptr = 1;
            break;
            
        case DIGI_MAPOTN_ODU2E_PACKET_GFP:
            *enet_pmon_ptr = FALSE;           
            *odu_type_ptr  = DIGI_ODUK_ODU2E;
            *num_lanes_ptr = 1;
            break;
            
        case DIGI_MAPOTN_ODU2F_PACKET_GFP:
            *enet_pmon_ptr = FALSE;           
            *odu_type_ptr  = DIGI_ODUK_ODU2F;
            *num_lanes_ptr = 1;
            break;
            
        case DIGI_MAPOTN_ODU3_PACKET_GFP:
            if (DIGI_LINE_40GE_MAC == traffic_mode)
            {
                *enet_pmon_ptr = adpt_handle->dev_info->tx_enet_pmon;
            }
            else
            {
                *enet_pmon_ptr = FALSE;
            }
            *odu_type_ptr  = DIGI_ODUK_ODU3;
            *num_lanes_ptr = 4;
            break;
            
        case DIGI_MAPOTN_ODU3E1_PACKET_GFP:
            *enet_pmon_ptr = FALSE;           
            *odu_type_ptr  = DIGI_ODUK_ODU3E1;
            *num_lanes_ptr = 4;
            break;
            
        case DIGI_MAPOTN_ODU3E2_PACKET_GFP:
            *enet_pmon_ptr = FALSE;           
            *odu_type_ptr  = DIGI_ODUK_ODU3E2;
            *num_lanes_ptr = 4;
            break;
           
        case DIGI_MAPOTN_ODU4_PACKET_GFP:
            if (DIGI_LINE_100GE_MAC == traffic_mode)
            {
                *enet_pmon_ptr = adpt_handle->dev_info->tx_enet_pmon;
            }
            else
            {
                *enet_pmon_ptr = FALSE;
            }
            *odu_type_ptr  = DIGI_ODUK_ODU4;
            *num_lanes_ptr = 10;
            break;
            
        case DIGI_MAPOTN_ODUFLEX_PACKET_GFP:
            if ((DIGI_LINE_10GE_MAC == traffic_mode) 
                 || (DIGI_LINE_40GE_MAC == traffic_mode)
                 || (DIGI_LINE_100GE_MAC == traffic_mode))
            {
                *enet_pmon_ptr = adpt_handle->dev_info->tx_enet_pmon;
            }
            else
            {
                *enet_pmon_ptr = FALSE;
            }
            *odu_type_ptr  = DIGI_ODUK_ODUFLEX;
            switch (DIGI_CLIENT_MODE_GET(traffic_mode))
            {
                case DIGI_ENET_SERDES_100GE_MAC:
                case DIGI_ENET_SERDES_100GE_TRANSPARENT:
                case DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
                    *num_lanes_ptr = 10;
                    break;

                case DIGI_ENET_SERDES_40GE_MAC:
                case DIGI_ENET_SERDES_40GE_TRANSPARENT:
                case DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
                    *num_lanes_ptr = 4;
                    break;

                case DIGI_ENET_SERDES_10GE_MAC:
                case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
                case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
                case DIGI_ENET_SERDES_10GE_TRANSPARENT:
                case DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON:           
                    *num_lanes_ptr = 1;
                    break;

                default:
                    rc = PMC_ERR_INVALID_PARAMETERS;
                    break;
            }
            break;

        case DIGI_MAPOTN_ODUFLEX_5G_GDPS_BMP:
        case DIGI_MAPOTN_ODUFLEX_10G_GDPS_BMP:
        case DIGI_MAPOTN_ODUFLEX_CPRI_5_BMP:
        case DIGI_MAPOTN_ODUFLEX_CPRI_10_BMP:
        case DIGI_MAPOTN_ODUFLEX_CPRI_6_BMP:
        case DIGI_MAPOTN_ODUFLEX_FC_800_BMP:
            *enet_pmon_ptr = FALSE;
            *odu_type_ptr  = DIGI_ODUK_ODUFLEX;
            *num_lanes_ptr = 1;
            break;

        case DIGI_MAPOTN_ODU4P_100_GE_GFP:
            *enet_pmon_ptr = FALSE;
            *odu_type_ptr  = DIGI_ODUK_ODU4;
            *num_lanes_ptr = 10;
            break;

        case DIGI_MAPOTN_ODU4P_100_GE_GFP_ENET:
            if (DIGI_LINE_SIDE == DIGI_TRAFFIC_POS_GET(traffic_mode))
            {
                *enet_pmon_ptr = adpt_handle->dev_info->tx_enet_pmon;
            }
            else
            {
                *enet_pmon_ptr = FALSE;
            }
            *odu_type_ptr  = DIGI_ODUK_ODU4;
            *num_lanes_ptr = 10;
            break;

        case DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP:
        case DIGI_MAPOTN_ODU4P_CBR_100_GE_GMP:
            *enet_pmon_ptr = adpt_handle->dev_info->tx_enet_pmon;
            *odu_type_ptr  = DIGI_ODUK_ODU4;
            *num_lanes_ptr = 10;
            break;

        case DIGI_MAPOTN_ODU3P_40_GE_GFP_ENET:
            if ((DIGI_LINE_SIDE == DIGI_TRAFFIC_POS_GET(traffic_mode))
                 || (DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == adpt_handle->dev_info->card_type))
            {
                *enet_pmon_ptr = adpt_handle->dev_info->tx_enet_pmon;
            }
            else
            {
                *enet_pmon_ptr = FALSE;
            }
            *odu_type_ptr  = DIGI_ODUK_ODU3;
            *num_lanes_ptr = 4;
            break;

        case DIGI_MAPOTN_ODU3P_CBR_40_GE_GMP:
        case DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP:
            *enet_pmon_ptr = adpt_handle->dev_info->tx_enet_pmon;
            *odu_type_ptr  = DIGI_ODUK_ODU3;
            *num_lanes_ptr = 4;
            break;

        case DIGI_MAPOTN_ODU3P_40_GE_GFP:
        case DIGI_MAPOTN_ODU3P_OC_768_CBR_BMP:
        case DIGI_MAPOTN_ODU3P_OC_768_CBR_AMP:
            *enet_pmon_ptr = FALSE;
            *odu_type_ptr  = DIGI_ODUK_ODU3;
            *num_lanes_ptr = 4;
            break;

        case DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP_ENET:
        case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP_ENET:
        case DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP:
            if (DIGI_LINE_SIDE == DIGI_TRAFFIC_POS_GET(traffic_mode))
            {
                *enet_pmon_ptr = adpt_handle->dev_info->tx_enet_pmon;
            }
            else
            {
                *enet_pmon_ptr = FALSE;
            }
            *odu_type_ptr  = DIGI_ODUK_ODU2;
            *num_lanes_ptr = 1;
            break;

        case DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP:
        case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP:
        case DIGI_MAPOTN_ODU2P_FC_800_GMP:
        case DIGI_MAPOTN_ODU2P_5G_GDPS_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_5_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_10_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_6_GMP:
        case DIGI_MAPOTN_ODU2P_OC_192_CBR_BMP:
        case DIGI_MAPOTN_ODU2P_OC_192_CBR_AMP:
            *enet_pmon_ptr = FALSE;
            *odu_type_ptr  = DIGI_ODUK_ODU2;
            *num_lanes_ptr = 1;
            break;

        case DIGI_MAPOTN_ODU1EP_CBR_10G3_7_2_BMP:
        case DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP:
            *enet_pmon_ptr = adpt_handle->dev_info->tx_enet_pmon;
            *odu_type_ptr  = DIGI_ODUK_ODU1E;
            *num_lanes_ptr = 1;
            break;

        case DIGI_MAPOTN_ODU1FP_FC_1200_BMP:
            *enet_pmon_ptr = FALSE;
            *odu_type_ptr  = DIGI_ODUK_ODU1F;
            *num_lanes_ptr = 1;
            break;

        case DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP:
        case DIGI_MAPOTN_ODU2EP_CBR_10G3_7_1_BMP:
            *enet_pmon_ptr = adpt_handle->dev_info->tx_enet_pmon;
            *odu_type_ptr  = DIGI_ODUK_ODU2E;
            *num_lanes_ptr = 1;
            break;

        case DIGI_MAPOTN_ODU2EP_FC_1200_GFP:
        case DIGI_MAPOTN_ODU2EP_10G_GDPS_GMP:
            *enet_pmon_ptr = FALSE;
            *odu_type_ptr  = DIGI_ODUK_ODU2E;
            *num_lanes_ptr = 1;
            break;

        case DIGI_MAPOTN_ODU2FP_FC_1200_BMP:
            *enet_pmon_ptr = FALSE;
            *odu_type_ptr  = DIGI_ODUK_ODU2F;
            *num_lanes_ptr = 1;
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    /* workaround on DIGI-60 */
    if ((DIGI60_PROD_APP_LINE_CARD == adpt_handle->dev_info->card_type)
        || (DIGI60_PROD_APP_TRANSMUXPONDER_CARD == adpt_handle->dev_info->card_type))
    {
        switch (DIGI_CLIENT_MODE_GET(traffic_mode))
        {
            case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
            case DIGI_ENET_SERDES_10GE_TRANSPARENT:
            case DIGI_ENET_SERDES_40GE_TRANSPARENT:
                *enet_pmon_ptr = FALSE;
                break;

            default:
                break;
        }    
    }

    PMC_RETURN(rc);
} /* adpt_mapper_para_get */


/*******************************************************************************
* adpt_mapper_lane_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function allocates mapper lanes in MAPOTN. Each lane is ODU0 size. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   mapping_mode       - mapping mode. 
*   ts_number          - tributary slot number. Only available for ODUflex.
*   *port_grp_usage    - pointer to a digi_port_usage_grp_struct_t.
*
* OUTPUTS:
*   *primary_grp       - The primary group associated with this space.
*   *secondary_grp     - The secondary group associated with this space.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_mapper_lane_create(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_mapotn_mode_t mapping_mode,
                                      UINT32 ts_number,
                                      digi_port_usage_grp_struct_t *port_grp_usage,
                                      UINT8 *primary_grp,
                                      UINT8 *secondary_grp)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 lane_num = 0;
    UINT32 index = 0;
    UINT32 avail_lane = 0;
    UINT32 start_lane = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 serdes_port = 0;
    UINT32 tx_serdes_port = 0;
    digi_mapper_primary_group_t pri_grp;
    digi_mapper_secondary_group_t sec_grp;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(port_grp_usage);
    ADPT_PARA_VERIFY(primary_grp);
    ADPT_PARA_VERIFY(secondary_grp);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* workaround on DIGI-60 */
    if ((DIGI60_PROD_APP_LINE_CARD == adpt_handle->dev_info->card_type)
        || (DIGI60_PROD_APP_TRANSMUXPONDER_CARD == adpt_handle->dev_info->card_type))
    {
        /* get serdes port */
        serdes_port = DIGI_SERDES_PORT_GET(chnl_id);
        if (serdes_port >= DIGI_SERDES_XFI_LANES_MAX)
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }

        if (DIGI60_PROD_APP_LINE_CARD == adpt_handle->dev_info->card_type)
        {
            rc = digi_enhanced_pmon_link_get(adpt_handle->digi_handle,
                                             adpt_handle->line_serdes[serdes_port],
                                             &pri_grp,
                                             &sec_grp);
            ADPT_RETURN_VERIFY(rc);
        }
        else
        {
            rc = adpt_serdes_port_get(dev_id, serdes_port, &tx_serdes_port);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_enhanced_pmon_link_get(adpt_handle->digi_handle,
                                             adpt_handle->line_serdes[tx_serdes_port],
                                             &pri_grp,
                                             &sec_grp);
            ADPT_RETURN_VERIFY(rc);
        }

        *primary_grp   = (UINT8)pri_grp;
        *secondary_grp = (UINT8)sec_grp;

        /* return directly */
        PMC_RETURN(rc);
    }
    else if (DIGI60_PROD_APP_TRANSMUXPONDER_SYS_CLIENT_CARD == adpt_handle->dev_info->card_type)
    {
        /* get serdes port */
        serdes_port = DIGI_SERDES_PORT_GET(chnl_id);
        if (serdes_port >= DIGI_SERDES_XFI_LANES_MAX)
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }

        rc = digi_enhanced_pmon_link_get(adpt_handle->digi_handle,
                                         adpt_handle->line_serdes[serdes_port],    
                                         &pri_grp,
                                         &sec_grp);
        ADPT_RETURN_VERIFY(rc);

        *primary_grp   = (UINT8)pri_grp;
        *secondary_grp = (UINT8)sec_grp;

        /* return directly */
        PMC_RETURN(rc);
    }

    switch (mapping_mode)
    {
        /* OTUk <> ODUk <> ODUk SW <> MAPOTN <> CPB <> ILKN */
        case DIGI_MAPOTN_ODU0_PACKET_GFP:
        case DIGI_MAPOTN_ODU1_PACKET_GFP:
        case DIGI_MAPOTN_ODU1E_PACKET_GFP:
        case DIGI_MAPOTN_ODU1F_PACKET_GFP:
        case DIGI_MAPOTN_ODU2_PACKET_GFP:
        case DIGI_MAPOTN_ODU2E_PACKET_GFP:
        case DIGI_MAPOTN_ODU2F_PACKET_GFP:
        case DIGI_MAPOTN_ODU3_PACKET_GFP:
        case DIGI_MAPOTN_ODU3E1_PACKET_GFP:
        case DIGI_MAPOTN_ODU3E2_PACKET_GFP:
        case DIGI_MAPOTN_ODUFLEX_PACKET_GFP:
        case DIGI_MAPOTN_ODU4_PACKET_GFP:
        /* ODUk SW <> MAPOTN <> CPB <> CBR */
        case DIGI_MAPOTN_ODU3P_OC_768_CBR_BMP:
        case DIGI_MAPOTN_ODU3P_OC_768_CBR_AMP:
        case DIGI_MAPOTN_ODU2P_FC_800_GMP:
        case DIGI_MAPOTN_ODU2P_5G_GDPS_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_5_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_10_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_6_GMP:
        case DIGI_MAPOTN_ODU2P_OC_192_CBR_BMP:
        case DIGI_MAPOTN_ODU2P_OC_192_CBR_AMP:
        case DIGI_MAPOTN_ODU1FP_FC_1200_BMP:
        case DIGI_MAPOTN_ODU2EP_FC_1200_GFP:
        case DIGI_MAPOTN_ODU2EP_10G_GDPS_GMP:
        case DIGI_MAPOTN_ODU2FP_FC_1200_BMP:
        case DIGI_MAPOTN_ODUFLEX_5G_GDPS_BMP:
        case DIGI_MAPOTN_ODUFLEX_10G_GDPS_BMP:
        case DIGI_MAPOTN_ODUFLEX_CPRI_5_BMP:
        case DIGI_MAPOTN_ODUFLEX_CPRI_10_BMP:
        case DIGI_MAPOTN_ODUFLEX_CPRI_6_BMP:
        case DIGI_MAPOTN_ODUFLEX_FC_800_BMP:
        /* ODUk SW <> MAPOTN <> CPB <> ENET_LINE */
        case DIGI_MAPOTN_ODU3P_40_GE_GFP_ENET:
        case DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP_ENET:
        case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP_ENET:
        case DIGI_MAPOTN_ODU4P_100_GE_GFP_ENET:
        case DIGI_MAPOTN_ODU4P_CBR_100_GE_GMP:
            *primary_grp   = DIGI_MAPPER_PRIMARY_GROUP_DONT_CARE;
            *secondary_grp = DIGI_MAPPER_SECONDARY_GROUP_DONT_CARE;

            PMC_RETURN(rc);
            break;

        case DIGI_MAPOTN_ODU3P_CBR_40_GE_GMP:
            if (DIGI_PROD_APP_TRANSMUXPONDER_CARD != adpt_handle->dev_info->card_type)
            {
                *primary_grp   = DIGI_MAPPER_PRIMARY_GROUP_DONT_CARE;
                *secondary_grp = DIGI_MAPPER_SECONDARY_GROUP_DONT_CARE;

                PMC_RETURN(rc);
            }
            else
            {
                lane_num = 4;
            }
            break;

        case DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP:
        case DIGI_MAPOTN_ODU1EP_CBR_10G3_7_2_BMP:
        case DIGI_MAPOTN_ODU2EP_CBR_10G3_7_1_BMP:
            if (DIGI_PROD_APP_TRANSMUXPONDER_CARD != adpt_handle->dev_info->card_type)
            {
                *primary_grp   = DIGI_MAPPER_PRIMARY_GROUP_DONT_CARE;
                *secondary_grp = DIGI_MAPPER_SECONDARY_GROUP_DONT_CARE;

                PMC_RETURN(rc);
            }
            else
            {
                lane_num = 1;
            }
            break;

        case DIGI_MAPOTN_ODU4P_100_GE_GFP:
        case DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP:
            *primary_grp   = DIGI_MAPPER_PRIMARY_GROUP_0;
            *secondary_grp = DIGI_MAPPER_SECONDARY_GROUP_0;

            PMC_RETURN(rc);
            break;

        case DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP:
        case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP:
        case DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP:
        case DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP:
            lane_num = 1;
            break;

        case DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP:
        case DIGI_MAPOTN_ODU3P_40_GE_GFP:
            lane_num = 4;
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    /* check available lanes */
    for (index = 0; index < DIGI_MAPPER_NUM_SECONDARY_GROUPS; index++)
    {
        if ((DIGI_DEFAULT_CHNL_ID == adpt_handle->map_lane[index])
            && (DIGI_MAPPER_ALLOC_UNUSED == port_grp_usage->port_usage[index].grp_alloc)
            && ((DIGI_SERDES_PROV_STATE_NO_SERDES == port_grp_usage->port_usage[index].prov_state)
            || (DIGI_SERDES_PROV_STATE_LINEOTN == port_grp_usage->port_usage[index].prov_state)
            || (DIGI_SERDES_PROV_STATE_ENET_SYS == port_grp_usage->port_usage[index].prov_state)))
        {
            if ((++avail_lane) == lane_num)
            {
                break;
            }
        }
        else
        {
            /* ODU3 should start from 0, 4, 8 */
            if (4 == lane_num)
            {
                avail_lane = 0;
                index = index - (index % 4) + 3;
                start_lane = index + 1;
            }
            else
            {
                avail_lane = 0;
                start_lane = index + 1;
            }
        }
    }

    if (avail_lane == lane_num)
    {
        /* reset */
        avail_lane = 0;

        for (index = start_lane; index < DIGI_MAPPER_NUM_SECONDARY_GROUPS; index++)
        {
            adpt_handle->map_lane[index] = chnl_id;

            if ((++avail_lane) == lane_num)
            {
                break;
            }
        }

        *primary_grp   = start_lane / 4;
        *secondary_grp = start_lane % 4;
    }
    else
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_mapper_lane_create */


/*******************************************************************************
* adpt_mapper_lane_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function destroys mapper lanes in MAPOTN. Each lane is ODU0 size. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_mapper_lane_destroy(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    for (index = 0; index < DIGI_MAPPER_NUM_SECONDARY_GROUPS; index++)
    {
        if (chnl_id == adpt_handle->map_lane[index])
        {
            adpt_handle->map_lane[index] = DIGI_DEFAULT_CHNL_ID;
        }
    }

    PMC_RETURN(rc);
} /* adpt_mapper_lane_destroy */


/*******************************************************************************
* adpt_mapping_mode_cvt
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves SDK mapping mode from ADPT mapping mode. 
*
* INPUTS:
*   mapping_mode       - mapping mode. 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   SDK defined mapping mode. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC digi_mapping_mode_t adpt_mapping_mode_cvt(digi_mapotn_mode_t mapping_mode)
{
    switch (mapping_mode)
    {
        case DIGI_MAPOTN_ODU4P_100_GE_GFP:
            return DIGI_MAP_ODU4P_100_GE_GFP;
            break;

        case DIGI_MAPOTN_ODU4P_100_GE_GFP_ENET:
            return DIGI_MAP_ODU4P_100_GE_GFP_ENET;
            break;

        case DIGI_MAPOTN_ODU3P_40_GE_GFP:
            return DIGI_MAP_ODU3P_40_GE_GFP;
            break;

        case DIGI_MAPOTN_ODU3P_40_GE_GFP_ENET:
            return DIGI_MAP_ODU3P_40_GE_GFP_ENET;
            break;

        case DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP:
            return DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP;
            break;

        case DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP_ENET:
            return DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP_ENET;
            break;

        case DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP:
            return DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP;
            break;

        case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP:
            return DIGI_MAP_ODU2P_10_GE_6_2_GFP;
            break;

        case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP_ENET:
            return DIGI_MAP_ODU2P_10_GE_6_2_GFP_ENET;
            break;

        case DIGI_MAPOTN_ODU2EP_FC_1200_GFP:
            return DIGI_MAP_ODU2EP_FC_1200_GFP;
            break;

        case DIGI_MAPOTN_ODU2P_FC_800_GMP:
            return DIGI_MAP_ODU2P_FC_800_GMP;
            break;

        case DIGI_MAPOTN_ODU2EP_10G_GDPS_GMP:
            return DIGI_MAP_ODU2EP_10G_GDPS_GMP;
            break;

        case DIGI_MAPOTN_ODU2P_5G_GDPS_GMP:
            return DIGI_MAP_ODU2P_5G_GDPS_GMP;
            break;

        case DIGI_MAPOTN_ODU2P_CPRI_5_GMP:
            return DIGI_MAP_ODU2P_CPRI_5_GMP;
            break;

        case DIGI_MAPOTN_ODU2P_CPRI_10_GMP:
            return DIGI_MAP_ODU2P_CPRI_10_GMP;
            break;

        case DIGI_MAPOTN_ODU2P_CPRI_6_GMP:
            return DIGI_MAP_ODU2P_CPRI_6_GMP;
            break;

        case DIGI_MAPOTN_ODU3P_CBR_40_GE_GMP:
            return DIGI_MAP_ODU3P_CBR_40_GE_GMP;
            break;

        case DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP:
            return DIGI_MAP_ODU3P_PKT_40_GE_GMP;
            break;

        case DIGI_MAPOTN_ODU4P_CBR_100_GE_GMP:
            return DIGI_MAP_ODU4P_CBR_100_GE_GMP;
            break;

        case DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP:
            return DIGI_MAP_ODU4P_PKT_100_GE_GMP;
            break;

        case DIGI_MAPOTN_ODUFLEX_5G_GDPS_BMP:
            return DIGI_MAP_ODUFLEX_5G_GDPS_BMP;
            break;

        case DIGI_MAPOTN_ODUFLEX_10G_GDPS_BMP:
            return DIGI_MAP_ODUFLEX_10G_GDPS_BMP;
            break;

        case DIGI_MAPOTN_ODUFLEX_CPRI_5_BMP:
            return DIGI_MAP_ODUFLEX_CPRI_5_BMP;
            break;

        case DIGI_MAPOTN_ODUFLEX_CPRI_10_BMP:
            return DIGI_MAP_ODUFLEX_CPRI_10_BMP;
            break;

        case DIGI_MAPOTN_ODUFLEX_CPRI_6_BMP:
            return DIGI_MAP_ODUFLEX_CPRI_6_BMP;
            break;

        case DIGI_MAPOTN_ODUFLEX_FC_800_BMP:
            return DIGI_MAP_ODUFLEX_FC_800_BMP;
            break;

        case DIGI_MAPOTN_ODU2FP_FC_1200_BMP:
            return DIGI_MAP_ODU2FP_FC_1200_BMP;
            break;

        case DIGI_MAPOTN_ODU1FP_FC_1200_BMP:
            return DIGI_MAP_ODU1FP_FC_1200_BMP;
            break;

        case DIGI_MAPOTN_ODU1EP_CBR_10G3_7_2_BMP:
            return DIGI_MAP_ODU1EP_CBR_10G3_7_2_BMP;
            break;

        case DIGI_MAPOTN_ODU2EP_CBR_10G3_7_1_BMP:
            return DIGI_MAP_ODU2EP_CBR_10G3_7_1_BMP;
            break;

        case DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP:
            return DIGI_MAP_ODU1EP_PKT_10G3_7_2_BMP;
            break;

        case DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP:
            return DIGI_MAP_ODU2EP_PKT_10G3_7_1_BMP;
            break;

        case DIGI_MAPOTN_ODU3P_OC_768_CBR_BMP:
            return DIGI_MAP_ODU3P_OC_768_CBR_BMP;
            break;

        case DIGI_MAPOTN_ODU2P_OC_192_CBR_BMP:
            return DIGI_MAP_ODU2P_OC_192_CBR_BMP;
            break;

        case DIGI_MAPOTN_ODU3P_OC_768_CBR_AMP:
            return DIGI_MAP_ODU3P_OC_768_CBR_AMP;
            break;

        case DIGI_MAPOTN_ODU2P_OC_192_CBR_AMP:
            return DIGI_MAP_ODU2P_OC_192_CBR_AMP;
            break;

        case DIGI_MAPOTN_ODU0_PACKET_GFP:
        case DIGI_MAPOTN_ODU1_PACKET_GFP:
        case DIGI_MAPOTN_ODU1E_PACKET_GFP:
        case DIGI_MAPOTN_ODU1F_PACKET_GFP:
        case DIGI_MAPOTN_ODU2_PACKET_GFP:
        case DIGI_MAPOTN_ODU2E_PACKET_GFP:
        case DIGI_MAPOTN_ODU2F_PACKET_GFP:
        case DIGI_MAPOTN_ODU3_PACKET_GFP:           
        case DIGI_MAPOTN_ODU3E1_PACKET_GFP:
        case DIGI_MAPOTN_ODU3E2_PACKET_GFP:
        case DIGI_MAPOTN_ODU4_PACKET_GFP:
        case DIGI_MAPOTN_ODUFLEX_PACKET_GFP:
            return DIGI_MAP_ODUKP_PACKET_GFP;
            break;

        default:
            return (digi_mapping_mode_t)mapping_mode;
            break;
    }

    return LAST_DIGI_MAP;
} /* adpt_mapping_mode_cvt */


/*******************************************************************************
* adpt_gfp_mapping_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function judges if mapping mode belongs to GFP mapping. 
*
* INPUTS:
*   mapping_mode       - mapping mode. 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   DIGI_MAP_AMP_BMP_GMP: AMP/BMP/GMP mapping.
*   DIGI_MAP_GFP: GFP mapping except G.sup43 7.3.
*   DIGI_MAP_7_3_GFP: GFP mapping with G.sup43 7.3.
*
* NOTES:
*
*******************************************************************************/
PUBLIC digi_mapping_type_t adpt_gfp_mapping_get(digi_mapotn_mode_t mapping_mode)
{
    switch (mapping_mode)
    {
        case DIGI_MAPOTN_ODU4P_100_GE_GFP:
        case DIGI_MAPOTN_ODU4P_100_GE_GFP_ENET:
        case DIGI_MAPOTN_ODU3P_40_GE_GFP:
        case DIGI_MAPOTN_ODU3P_40_GE_GFP_ENET:
        case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP:
        case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP_ENET:
        case DIGI_MAPOTN_ODU2EP_FC_1200_GFP:
        case DIGI_MAPOTN_ODU0_PACKET_GFP:
        case DIGI_MAPOTN_ODU1_PACKET_GFP:
        case DIGI_MAPOTN_ODU1E_PACKET_GFP:
        case DIGI_MAPOTN_ODU1F_PACKET_GFP:
        case DIGI_MAPOTN_ODU2_PACKET_GFP:
        case DIGI_MAPOTN_ODU2E_PACKET_GFP:
        case DIGI_MAPOTN_ODU2F_PACKET_GFP:
        case DIGI_MAPOTN_ODU3_PACKET_GFP:
        case DIGI_MAPOTN_ODU3E1_PACKET_GFP:
        case DIGI_MAPOTN_ODU3E2_PACKET_GFP:
        case DIGI_MAPOTN_ODU4_PACKET_GFP:
        case DIGI_MAPOTN_ODUFLEX_PACKET_GFP:
            return DIGI_MAP_GFP;
            break;

        case DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP:
        case DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP_ENET:
        case DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP:
            return DIGI_MAP_7_3_GFP;
            break;

        default:
            break;
    }

    return DIGI_MAP_AMP_BMP_GMP;
} /* adpt_gfp_mapping_get */


/*******************************************************************************
* adpt_mapper_ts_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves mapper tributary slot number for a given mapping mode. 
*
* INPUTS:
*   mapping_mode       - mapping mode. 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TS number.
*
* NOTES:
*   As to the DIGI_MAPOTN_ODUFLEX_PACKET_GFP mapping, TS is fixed as 7.
*
*******************************************************************************/
PUBLIC UINT32 adpt_mapper_ts_get(digi_mapotn_mode_t mapping_mode)
{
    switch (mapping_mode)
    {
        case DIGI_MAPOTN_ODU4_PACKET_GFP:
        case DIGI_MAPOTN_ODU4P_100_GE_GFP:
        case DIGI_MAPOTN_ODU4P_100_GE_GFP_ENET:
        case DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP:
        case DIGI_MAPOTN_ODU4P_CBR_100_GE_GMP:
            return 80;
            break;

        case DIGI_MAPOTN_ODU3P_40_GE_GFP_ENET:
        case DIGI_MAPOTN_ODU3P_CBR_40_GE_GMP:
        case DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP:
        case DIGI_MAPOTN_ODU3P_40_GE_GFP:
        case DIGI_MAPOTN_ODU3P_OC_768_CBR_BMP:
        case DIGI_MAPOTN_ODU3P_OC_768_CBR_AMP:
        case DIGI_MAPOTN_ODU3E2_PACKET_GFP:
        case DIGI_MAPOTN_ODU3E1_PACKET_GFP:
        case DIGI_MAPOTN_ODU3_PACKET_GFP:
            return 32;
            break;

        case DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP_ENET:
        case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP_ENET:
        case DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP:
        case DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP:
        case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP:
        case DIGI_MAPOTN_ODU2P_FC_800_GMP:
        case DIGI_MAPOTN_ODU2P_5G_GDPS_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_5_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_10_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_6_GMP:
        case DIGI_MAPOTN_ODU2P_OC_192_CBR_BMP:
        case DIGI_MAPOTN_ODU2P_OC_192_CBR_AMP:
        case DIGI_MAPOTN_ODU1EP_CBR_10G3_7_2_BMP:
        case DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP:
        case DIGI_MAPOTN_ODU1FP_FC_1200_BMP:
        case DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP:
        case DIGI_MAPOTN_ODU2EP_CBR_10G3_7_1_BMP:
        case DIGI_MAPOTN_ODU2EP_FC_1200_GFP:
        case DIGI_MAPOTN_ODU2EP_10G_GDPS_GMP:
        case DIGI_MAPOTN_ODU2FP_FC_1200_BMP:
        case DIGI_MAPOTN_ODU1E_PACKET_GFP:
        case DIGI_MAPOTN_ODU1F_PACKET_GFP:
        case DIGI_MAPOTN_ODU2_PACKET_GFP:
        case DIGI_MAPOTN_ODU2E_PACKET_GFP:
        case DIGI_MAPOTN_ODU2F_PACKET_GFP:
            return 8;
            break;

        case DIGI_MAPOTN_ODUFLEX_PACKET_GFP:
        case DIGI_MAPOTN_ODUFLEX_5G_GDPS_BMP:
        case DIGI_MAPOTN_ODUFLEX_CPRI_5_BMP:
        case DIGI_MAPOTN_ODUFLEX_FC_800_BMP:
            return 7;
            break;

        case DIGI_MAPOTN_ODUFLEX_CPRI_6_BMP:
            return 5;
            break;

        case DIGI_MAPOTN_ODUFLEX_CPRI_10_BMP:
            return 8;
            break;

        case DIGI_MAPOTN_ODUFLEX_10G_GDPS_BMP:
            return 9;
            break;

        case DIGI_MAPOTN_ODU1_PACKET_GFP:
            return 2;
            break;

        case DIGI_MAPOTN_ODU0_PACKET_GFP:
            return 1;
            break;

        default:
            break;
    }

    return 0;
} /* adpt_mapper_ts_get */


/*******************************************************************************
* adpt_cpb_eth_para_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves CPB parameters from Ethernet traffic. 
*
* INPUTS:
*   traffic_mode       - traffic mode. 
*
* OUTPUTS:
*   *cpb_stream_ptr    - pointer to CPB stream buffer.
*   *halt_buffer_ptr   - pointer to halt buffer block. The block size in 
*                        bytes per unit is: 12288 bytes
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cpb_eth_para_get(digi_traffic_mode_t traffic_mode,
                                    digi_cpb_stream_t *cpb_stream_ptr,
                                    UINT32 *halt_buffer_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_enet_mode_t client_mode = LAST_DIGI_ENET;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(cpb_stream_ptr);
    ADPT_PARA_VERIFY(halt_buffer_ptr);

    /* get client mode */
    client_mode = (digi_enet_mode_t)DIGI_CLIENT_MODE_GET(traffic_mode);

    switch (client_mode)
    {
        case DIGI_ENET_SERDES_100GE_MAC:
        case DIGI_ENET_SERDES_100GE_TRANSPARENT:
        case DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
            *cpb_stream_ptr  = DIGI_CPB_STREAM_100G;
            *halt_buffer_ptr = 2;
            break;

        case DIGI_ENET_SERDES_40GE_MAC:
        case DIGI_ENET_SERDES_40GE_TRANSPARENT:
        case DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
            *cpb_stream_ptr  = DIGI_CPB_STREAM_40G;
            *halt_buffer_ptr = 0;        
            break;

        case DIGI_ENET_SERDES_10GE_MAC:
        case DIGI_ENET_SERDES_10GE_TRANSPARENT:
        case DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
            *cpb_stream_ptr  = DIGI_CPB_STREAM_10G;
            *halt_buffer_ptr = 0;        
            break;

        case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
        case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
            *cpb_stream_ptr  = DIGI_CPB_STREAM_10G;
            *halt_buffer_ptr = 0;        
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_cpb_eth_para_get */


/*******************************************************************************
* adpt_cpb_cbr_para_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves CPB parameters from CBR traffic. 
*
* INPUTS:
*   traffic_mode       - traffic mode. 
*
* OUTPUTS:
*   *cpb_stream_ptr    - pointer to CPB stream buffer.
*   *halt_buffer_ptr   - pointer to halt buffer block. The block size in 
*                        bytes per unit is: 12288 bytes
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cpb_cbr_para_get(digi_traffic_mode_t traffic_mode,
                                    digi_cpb_stream_t *cpb_stream_ptr,
                                    UINT32 *halt_buffer_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cbr_client_mode_t client_mode = LAST_DIGI_CBR_CLIENT;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(cpb_stream_ptr);
    ADPT_PARA_VERIFY(halt_buffer_ptr);

    /* get client mode */
    client_mode = (digi_cbr_client_mode_t)DIGI_CLIENT_MODE_GET(traffic_mode);

    switch (client_mode)
    {
        case DIGI_CBR_CLIENT_10G_STS192:
        case DIGI_CBR_CLIENT_10G_STS192_PRBS:
        case DIGI_CBR_CLIENT_10G_STM64:
        case DIGI_CBR_CLIENT_10G_STM64_PRBS:
        case DIGI_CBR_CLIENT_10G_FC1200_ODU1F:
        case DIGI_CBR_CLIENT_10G_FC1200_ODU2F:
        case DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E:
        case DIGI_CBR_CLIENT_10G_FC800:
        case DIGI_CBR_CLIENT_10G_CPRI_5G:
        case DIGI_CBR_CLIENT_10G_CPRI_6G:
        case DIGI_CBR_CLIENT_10G_CPRI_10G:
        case DIGI_CBR_CLIENT_10G_GDPS_5G:
        case DIGI_CBR_CLIENT_10G_GDPS_10G:
            *cpb_stream_ptr  = DIGI_CPB_STREAM_10G;
            *halt_buffer_ptr = 0;        
            break;

        case DIGI_CBR_CLIENT_40G_STS768:
        case DIGI_CBR_CLIENT_40G_STM256:
            *cpb_stream_ptr  = DIGI_CPB_STREAM_40G;
            *halt_buffer_ptr = 0;        
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_cpb_cbr_para_get */


/*******************************************************************************
* adpt_cpb_ilkn_para_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves CPB parameters from interlaken interface. 
*
* INPUTS:
*   ilkn_info          - interlaken information. 
*
* OUTPUTS:
*   *cpb_stream_ptr    - pointer to CPB stream buffer.
*   *halt_buffer_ptr   - pointer to halt buffer block. The block size in 
*                        bytes per unit is: 12288 bytes
*   *ts_number         - tributory number.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cpb_ilkn_para_get(UINT16 ilkn_info,
                                    digi_cpb_stream_t *cpb_stream_ptr,
                                    UINT32 *halt_buffer_ptr,
                                    UINT32 *ts_number)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(cpb_stream_ptr);
    ADPT_PARA_VERIFY(halt_buffer_ptr);
    ADPT_PARA_VERIFY(ts_number);

    *ts_number       = DIGI_ILKN_RATE_GET(ilkn_info);
    *halt_buffer_ptr = 0;

    switch (*ts_number)
    {
        case 1: 
            *cpb_stream_ptr = DIGI_CPB_STREAM_1G25;
            break;

        case 2:
            *cpb_stream_ptr = DIGI_CPB_STREAM_2G5;
            break;

        case 8:
            *cpb_stream_ptr = DIGI_CPB_STREAM_10G;
            break;

        case 32:
            *cpb_stream_ptr = DIGI_CPB_STREAM_40G;
            break;

        case 80:
            *cpb_stream_ptr = DIGI_CPB_STREAM_100G;
            break;

        default:
            *cpb_stream_ptr = DIGI_CPB_STREAM_FLEX;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_cpb_ilkn_para_get */


/*******************************************************************************
* adpt_oduksw_odu_para_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves oduksw parameters from oduk type. 
*
* INPUTS:
*   oduk_type          - oduk type. 
*   oduflex_rate       - For ODUFLEX(CBR), specify the client bit rate in bps. 
*                        For all the other LO ODU types, set to 0.  
*   ts_mask            - tributary slot information.
*
* OUTPUTS:
*   *oduksw_stream_ptr - pointer to oduksw stream buffer.
*   *ts_number_ptr     - pointer to 1.25G tributary slot number. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_oduksw_odu_para_get(digi_oduk_t oduk_type,
                                        DOUBLE oduflex_rate,
                                        UINT32 ts_mask[3],
                                        digi_oduksw_stream_t *oduksw_stream_ptr,
                                        UINT32 *ts_number_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 ts_number = 0;
    UINT32 i = 0;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(oduksw_stream_ptr);
    ADPT_PARA_VERIFY(ts_number_ptr);

    /* get tributary slot number */
    for (i = 0; i < 96; i++)
    {
        if (ts_mask[i/32] & (1 << (i % 32)))
        {
            ts_number++;
        }
    }

    switch (oduk_type)
    {
        case DIGI_ODUK_ODU0: 
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU0;
            *ts_number_ptr     = 0;
            break;

        case DIGI_ODUK_ODU1:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU1;
            *ts_number_ptr     = 0;
            break;

        case DIGI_ODUK_ODU1E:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU1E;
            *ts_number_ptr     = 0;
            break;

        case DIGI_ODUK_ODU1F:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU1F;
            *ts_number_ptr     = 0;
            break;

        case DIGI_ODUK_ODU2:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU2;
            *ts_number_ptr     = 0;
            break;

        case DIGI_ODUK_ODU2E:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU2E;
            *ts_number_ptr     = 0;
            break;

        case DIGI_ODUK_ODU2F:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU2F;
            *ts_number_ptr     = 0;
            break;

        case DIGI_ODUK_ODU3:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU3;
            *ts_number_ptr     = 0;
            break;

        case DIGI_ODUK_ODU3E1:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU3E1;
            *ts_number_ptr     = 0;
            break;

        case DIGI_ODUK_ODU3E2:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU3E2;
            *ts_number_ptr     = 0;
            break;

        case DIGI_ODUK_ODU4:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU4;
            *ts_number_ptr     = 0;
            break;

        case DIGI_ODUK_ODUFLEX:
            if (oduflex_rate < 1000.0)
            {
                *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_PKT_GFPF_ODUFLEX;
            }
            else
            {
                *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODUFLEX;
            }
            *ts_number_ptr   = ts_number;
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_oduksw_odu_para_get */


/*******************************************************************************
* adpt_oduksw_map_para_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves oduksw parameters from mapping mode. 
*
* INPUTS:
*   mapping_mode       - mapping mode. 
*
* OUTPUTS:
*   *oduksw_stream_ptr - pointer to oduksw stream buffer.
*   *oduflex_rate      - ODUflex bitrate in kbps if oduksw stream 'rate_data' is  
*                        set to DIGI_ODUKSW_STREAM_CBR_ODUFLEX.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_oduksw_map_para_get(digi_mapotn_mode_t mapping_mode,
                                        digi_oduksw_stream_t *oduksw_stream_ptr,
                                        DOUBLE *oduflex_rate)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(oduksw_stream_ptr);
    ADPT_PARA_VERIFY(oduflex_rate);

    /* initialization */
    *oduflex_rate = 0;

    switch (mapping_mode)
    {
        case DIGI_MAPOTN_ODU0_PACKET_GFP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU0;
            break;

        case DIGI_MAPOTN_ODU1_PACKET_GFP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU1;
            break;

        case DIGI_MAPOTN_ODU1E_PACKET_GFP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU1E;
            break;

        case DIGI_MAPOTN_ODU1F_PACKET_GFP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU1F;
            break;

        case DIGI_MAPOTN_ODU2_PACKET_GFP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU2;
            break;

        case DIGI_MAPOTN_ODU2E_PACKET_GFP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU2E;
            break;

        case DIGI_MAPOTN_ODU2F_PACKET_GFP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU2F;
            break;

        case DIGI_MAPOTN_ODU3_PACKET_GFP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU3;
            break;

        case DIGI_MAPOTN_ODU3E1_PACKET_GFP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU3E1;
            break;

        case DIGI_MAPOTN_ODU3E2_PACKET_GFP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU3E2;
            break;

        case DIGI_MAPOTN_ODU4_PACKET_GFP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU4;
            break;

        case DIGI_MAPOTN_ODUFLEX_PACKET_GFP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_PKT_GFPF_ODUFLEX;
            break;        

        case DIGI_MAPOTN_ODUFLEX_5G_GDPS_BMP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODUFLEX;
            *oduflex_rate = UTIL_GLOBAL_ODU_FLEX_GDPS_5G_RATE;
            break;

        case DIGI_MAPOTN_ODUFLEX_10G_GDPS_BMP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODUFLEX;
            *oduflex_rate = UTIL_GLOBAL_ODU_FLEX_GDPS_10G_RATE;
            break;

        case DIGI_MAPOTN_ODUFLEX_CPRI_5_BMP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODUFLEX;
            *oduflex_rate = UTIL_GLOBAL_ODU_FLEX_CPRI_5_RATE;
            break;

        case DIGI_MAPOTN_ODUFLEX_CPRI_10_BMP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODUFLEX;
            *oduflex_rate = UTIL_GLOBAL_ODU_FLEX_CPRI_7_RATE;
            break;

        case DIGI_MAPOTN_ODUFLEX_CPRI_6_BMP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODUFLEX;
            *oduflex_rate = UTIL_GLOBAL_ODU_FLEX_CPRI_6_RATE;
            break;

        case DIGI_MAPOTN_ODUFLEX_FC_800_BMP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODUFLEX;
            *oduflex_rate = UTIL_GLOBAL_ODU_FLEX_FC_800_RATE;
            break;

        case DIGI_MAPOTN_ODU2P_5G_GDPS_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_5_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_10_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_6_GMP:
        case DIGI_MAPOTN_ODU2P_FC_800_GMP:
        case DIGI_MAPOTN_ODU2P_OC_192_CBR_AMP:
        case DIGI_MAPOTN_ODU2P_OC_192_CBR_BMP:
        case DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP:
        case DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP_ENET:
        case DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP:
        case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP:
        case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP_ENET:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU2;
            break;

        case DIGI_MAPOTN_ODU2EP_10G_GDPS_GMP:
        case DIGI_MAPOTN_ODU2EP_FC_1200_GFP:
        case DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP:
        case DIGI_MAPOTN_ODU2EP_CBR_10G3_7_1_BMP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU2E;
            break;

        case DIGI_MAPOTN_ODU2FP_FC_1200_BMP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU2F;
            break;

        case DIGI_MAPOTN_ODU1FP_FC_1200_BMP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU1F;
            break;

        case DIGI_MAPOTN_ODU4P_100_GE_GFP:
        case DIGI_MAPOTN_ODU4P_100_GE_GFP_ENET:
        case DIGI_MAPOTN_ODU4P_CBR_100_GE_GMP:
        case DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU4;
            break;

        case DIGI_MAPOTN_ODU3P_OC_768_CBR_BMP:
        case DIGI_MAPOTN_ODU3P_OC_768_CBR_AMP:
        case DIGI_MAPOTN_ODU3P_40_GE_GFP:
        case DIGI_MAPOTN_ODU3P_40_GE_GFP_ENET:
        case DIGI_MAPOTN_ODU3P_CBR_40_GE_GMP:
        case DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU3;
            break;

        case DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP:
        case DIGI_MAPOTN_ODU1EP_CBR_10G3_7_2_BMP:
            *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_CBR_ODU1E;
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_oduksw_map_para_get */


/*******************************************************************************
* adpt_oduksw_ilkn_para_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves oduksw parameters from interlaken interface. 
*
* INPUTS:
*   ilkn_info          - interlaken information. 
*
* OUTPUTS:
*   *oduksw_stream_ptr - pointer to oduksw stream buffer.
*   *oduflex_rate      - client bitrate in kbps if oduksw stream 'rate_data' is  
*                        set to DIGI_ODUKSW_STREAM_CBR_ODUFLEX.
*   *ts_number         - tributory number.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_oduksw_ilkn_para_get(UINT16 ilkn_info,
                                        digi_oduksw_stream_t *oduksw_stream_ptr,
                                        DOUBLE *oduflex_rate,
                                        UINT32 *ts_number)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(oduksw_stream_ptr);
    ADPT_PARA_VERIFY(oduflex_rate);
    ADPT_PARA_VERIFY(ts_number);

    *oduflex_rate      = 0;
    *ts_number         = DIGI_ILKN_RATE_GET(ilkn_info);
    *oduksw_stream_ptr = DIGI_ODUKSW_STREAM_PKT_GFPF_ODUFLEX;

    PMC_RETURN(rc);
} /* adpt_oduksw_ilkn_para_get */


/*******************************************************************************
* adpt_oduksw_para_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves oduksw parameters from ILKN/COREOTN/MAPOTN. 
*
* INPUTS:
*   dev_id             - device identity.
*   src_chnl_id        - source channel identity.
*   dest_chnl_id       - destination channel identity.
*
* OUTPUTS:
*   *oduksw_stream     - oduksw stream buffer.
*   *oduflex_rate      - client bitrate in kbps if oduksw stream 'rate_data' is  
*                        set to DIGI_ODUKSW_STREAM_CBR_ODUFLEX.
*   *ts_number         - tributory number.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_oduksw_para_get(UINT32 dev_id,
                                    UINT32 src_chnl_id,
                                    UINT32 dest_chnl_id,
                                    digi_oduksw_stream_t *oduksw_stream,
                                    DOUBLE *oduflex_rate,
                                    UINT32 *ts_number)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 src_index = 0;
    UINT32 dest_index = 0;
    UINT32 src_traffic_type = 0;
    UINT32 dest_traffic_type = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(oduksw_stream);
    ADPT_PARA_VERIFY(oduflex_rate);
    ADPT_PARA_VERIFY(ts_number);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc |= adpt_chnl_index_get(adpt_handle, src_chnl_id, &src_index);
    rc |= adpt_chnl_index_get(adpt_handle, dest_chnl_id, &dest_index);
    ADPT_RETURN_VERIFY(rc);

    /* get traffic type */
    src_traffic_type  = DIGI_TRAFFIC_TYPE_GET(adpt_handle->traffic_info[src_index].traffic_mode);
    dest_traffic_type = DIGI_TRAFFIC_TYPE_GET(adpt_handle->traffic_info[dest_index].traffic_mode);
    if ((src_traffic_type >= DIGI_TYPE_MAX) || (dest_traffic_type >= DIGI_TYPE_MAX))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    switch (src_traffic_type)
    {
        case DIGI_TYPE_OTU:
            /* get oduksw parameter from oduk type */
            *oduflex_rate = adpt_handle->traffic_info[src_index].oduflex_rate;
            rc = adpt_oduksw_odu_para_get(adpt_handle->traffic_info[src_index].oduk_type,
                                          adpt_handle->traffic_info[src_index].oduflex_rate,
                                          &(adpt_handle->traffic_info[src_index].rx_ts_mask[0]),
                                          oduksw_stream,
                                          ts_number);
            break;

        case DIGI_TYPE_ETH:
        case DIGI_TYPE_CBR:
            /* get oduksw parameter from mapotn */
            rc = adpt_oduksw_map_para_get(adpt_handle->traffic_info[src_index].mapping_mode,
                                          oduksw_stream,
                                          oduflex_rate);
            *ts_number = adpt_handle->traffic_info[src_index].ts_number;
            break;

        case DIGI_TYPE_INLK:
            switch (dest_traffic_type)
            {
                case DIGI_TYPE_OTU:
                    /* get oduksw parameter from oduk type */
                    *oduflex_rate = adpt_handle->traffic_info[dest_index].oduflex_rate;
                    rc = adpt_oduksw_odu_para_get(adpt_handle->traffic_info[dest_index].oduk_type,
                                                  adpt_handle->traffic_info[dest_index].oduflex_rate,
                                                  &(adpt_handle->traffic_info[dest_index].tx_ts_mask[0]),
                                                  oduksw_stream,
                                                  ts_number);
                    break;

                case DIGI_TYPE_ETH:
                case DIGI_TYPE_CBR:
                    /* get oduksw parameter from mapotn */
                    rc = adpt_oduksw_map_para_get(adpt_handle->traffic_info[dest_index].mapping_mode,
                                                  oduksw_stream,
                                                  oduflex_rate);
                    *ts_number = adpt_handle->traffic_info[dest_index].ts_number;
                    break;

                case DIGI_TYPE_INLK:
                    if (NULL != adpt_handle->traffic_info[dest_index].map_handle)
                    {
                        /* get oduksw parameter from dest MAPOTN */
                        rc = adpt_oduksw_map_para_get(adpt_handle->traffic_info[dest_index].mapping_mode,
                                                      oduksw_stream,
                                                      oduflex_rate);
                        *ts_number = adpt_handle->traffic_info[dest_index].ts_number;
                    }
                    else if (NULL != adpt_handle->traffic_info[src_index].map_handle)
                    {
                        /* get oduksw parameter from source MAPOTN */
                        rc = adpt_oduksw_map_para_get(adpt_handle->traffic_info[src_index].mapping_mode,
                                                      oduksw_stream,
                                                      oduflex_rate);
                        *ts_number = adpt_handle->traffic_info[src_index].ts_number;
                    }
                    else
                    {
                        /* get oduksw parameter from interlaken */
                        rc = adpt_oduksw_odu_para_get(adpt_handle->traffic_info[dest_index].oduk_type,
                                                      adpt_handle->traffic_info[dest_index].oduflex_rate,
                                                      &(adpt_handle->traffic_info[dest_index].tx_ts_mask[0]),
                                                      oduksw_stream,
                                                      ts_number);
                        *oduflex_rate = adpt_handle->traffic_info[dest_index].oduflex_rate;
                        *ts_number    = DIGI_ILKN_RATE_GET(adpt_handle->traffic_info[dest_index].ilkn_info);
                    }
                    break;

                default:
                    PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                    break;
            }
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_oduksw_para_get */


/*******************************************************************************
* adpt_cpb_map_para_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves CPB parameters from mapping mode. 
*
* INPUTS:
*   mapping_mode       - mapping mode. 
*
* OUTPUTS:
*   *cpb_stream_ptr    - pointer to CPB stream buffer.
*   *halt_buffer_ptr   - pointer to halt buffer block. The block size in 
*                        bytes per unit is: 12288 bytes
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cpb_map_para_get(digi_mapotn_mode_t mapping_mode,
                                     digi_cpb_stream_t *cpb_stream_ptr,
                                     UINT32 *halt_buffer_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(cpb_stream_ptr);
    ADPT_PARA_VERIFY(halt_buffer_ptr);

    switch (mapping_mode)
    {
        case DIGI_MAPOTN_ODU0_PACKET_GFP:
            *cpb_stream_ptr  = DIGI_CPB_STREAM_1G25;
            *halt_buffer_ptr = 0;
            break;

        case DIGI_MAPOTN_ODU1_PACKET_GFP:
            *cpb_stream_ptr  = DIGI_CPB_STREAM_2G5;
            *halt_buffer_ptr = 0;
            break;

        case DIGI_MAPOTN_ODU1E_PACKET_GFP:
        case DIGI_MAPOTN_ODU1F_PACKET_GFP:
        case DIGI_MAPOTN_ODU2_PACKET_GFP:
        case DIGI_MAPOTN_ODU2E_PACKET_GFP:
        case DIGI_MAPOTN_ODU2F_PACKET_GFP:
        case DIGI_MAPOTN_ODU2P_5G_GDPS_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_10_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_5_GMP:
        case DIGI_MAPOTN_ODU2EP_10G_GDPS_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_6_GMP:
        case DIGI_MAPOTN_ODU2P_FC_800_GMP:
        case DIGI_MAPOTN_ODU2FP_FC_1200_BMP:
        case DIGI_MAPOTN_ODU1FP_FC_1200_BMP:
        case DIGI_MAPOTN_ODU2EP_FC_1200_GFP:
        case DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP:
        case DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP_ENET:
        case DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP:
        case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP:
        case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP_ENET:
        case DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP:
        case DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP:
        case DIGI_MAPOTN_ODU1EP_CBR_10G3_7_2_BMP:
        case DIGI_MAPOTN_ODU2EP_CBR_10G3_7_1_BMP:
        case DIGI_MAPOTN_ODU2P_OC_192_CBR_AMP:
        case DIGI_MAPOTN_ODU2P_OC_192_CBR_BMP:
            *cpb_stream_ptr  = DIGI_CPB_STREAM_10G;
            *halt_buffer_ptr = 0;
            break;

        case DIGI_MAPOTN_ODU3_PACKET_GFP:
        case DIGI_MAPOTN_ODU3E1_PACKET_GFP:
        case DIGI_MAPOTN_ODU3E2_PACKET_GFP:
        case DIGI_MAPOTN_ODU3P_OC_768_CBR_BMP:
        case DIGI_MAPOTN_ODU3P_OC_768_CBR_AMP:
        case DIGI_MAPOTN_ODU3P_40_GE_GFP:
        case DIGI_MAPOTN_ODU3P_40_GE_GFP_ENET:
        case DIGI_MAPOTN_ODU3P_CBR_40_GE_GMP:
        case DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP:
            *cpb_stream_ptr  = DIGI_CPB_STREAM_40G;
            *halt_buffer_ptr = 0;
            break;

        case DIGI_MAPOTN_ODUFLEX_PACKET_GFP:
        case DIGI_MAPOTN_ODUFLEX_5G_GDPS_BMP:
        case DIGI_MAPOTN_ODUFLEX_10G_GDPS_BMP:
        case DIGI_MAPOTN_ODUFLEX_CPRI_5_BMP:
        case DIGI_MAPOTN_ODUFLEX_CPRI_10_BMP:
        case DIGI_MAPOTN_ODUFLEX_CPRI_6_BMP:
        case DIGI_MAPOTN_ODUFLEX_FC_800_BMP:
            *cpb_stream_ptr  = DIGI_CPB_STREAM_FLEX;
            *halt_buffer_ptr = 0;
            break;

        case DIGI_MAPOTN_ODU4_PACKET_GFP:
        case DIGI_MAPOTN_ODU4P_100_GE_GFP:
        case DIGI_MAPOTN_ODU4P_100_GE_GFP_ENET:
        case DIGI_MAPOTN_ODU4P_CBR_100_GE_GMP:
        case DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP:
            *cpb_stream_ptr  = DIGI_CPB_STREAM_100G;
            *halt_buffer_ptr = 2;
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_cpb_map_para_get */


/*******************************************************************************
* adpt_gfp_chnl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves GFP channel ID in MAPOTN block. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *odu_chnl          - ODU channel in stage 4 of COREOTN block.
*   *gfp_chnl          - GFP channel in MAPOTN block.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_chnl_get(UINT32 dev_id, 
                                UINT32 chnl_id, 
                                UINT32 *gfp_chnl,
                                UINT32 *odu_chnl)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 enet_sys_link = 0; 
    UINT32 enet_line_link = 0;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(gfp_chnl);
    ADPT_PARA_VERIFY(odu_chnl);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].map_handle);

    rc = digi_mapper_channel_num_get(adpt_handle->digi_handle,
                                     adpt_handle->traffic_info[index].map_handle,
                                     gfp_chnl,
                                     odu_chnl,
                                     &enet_sys_link,
                                     &enet_line_link);

    /* invalid channel ID */
    if ((*gfp_chnl >= DIGI_ODU_CHNLS_MAX) || (*odu_chnl >= DIGI_ODU_CHNLS_MAX))
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_gfp_chnl_get */


/*******************************************************************************
* adpt_otu_chnl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves OTU channel ID and ODU stage number in COREOTN block. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *otu_chnl          - OTU channel in LINEOTN block.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otu_chnl_get(UINT32 dev_id, UINT32 chnl_id, UINT32 *otu_chnl)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(otu_chnl);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].otu_handle);

    *otu_chnl = digi_otn_server_chnl_num_get(adpt_handle->digi_handle,
                                             adpt_handle->traffic_info[index].otu_handle);

    /* invalid channel ID */
    if (*otu_chnl >= DIGI_OTN_SERVER_CHNL_MAX)
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_otu_chnl_get */


/*******************************************************************************
* adpt_odu_chnl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves ODU channel ID and ODU stage number in COREOTN block. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *odu_chnl          - ODU channel in COREOTN block.
*   *odu_stage         - ODU stage number in COREOTN block.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_odu_chnl_get(UINT32 dev_id, 
                                 UINT32 chnl_id, 
                                 UINT32 *odu_chnl,
                                 digi_odu_struct_odu_level_t *odu_stage)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(odu_chnl);
    ADPT_PARA_VERIFY(odu_stage);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].odu_handle);

    *odu_chnl = digi_otn_odu_chnl_num_get(adpt_handle->digi_handle,
                                          adpt_handle->traffic_info[index].odu_handle);
    *odu_stage = adpt_handle->traffic_info[index].odu_stage;

    /* invalid channel ID */
    if (*odu_chnl >= DIGI_ODU_CHNLS_MAX)
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_odu_chnl_get */


/*******************************************************************************
* adpt_enet_chnl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves ENET channel ID in ENET(LINE)/(SYS) subsystem. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *enet_chnl         - ENET channel in ENET(LINE)/(SYS) block.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_chnl_get(UINT32 dev_id, UINT32 chnl_id, UINT32 *enet_chnl)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(enet_chnl);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].enet_handle);

    rc = digi_enet_channel_num_get(adpt_handle->digi_handle,
                                   adpt_handle->traffic_info[index].enet_handle,
                                   enet_chnl);
    ADPT_RETURN_VERIFY(rc);

    /* invalid channel ID */
    if (*enet_chnl >= DIGI_ENET_CHNL_MAX)
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_enet_chnl_get */


/*******************************************************************************
* adpt_cbr_chnl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves CBR channel ID in CBR subsystem. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *cbr_chnl          - CBR channel in CBR block.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cbr_chnl_get(UINT32 dev_id, UINT32 chnl_id, UINT32 *cbr_chnl)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(cbr_chnl);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].cbr_handle);

    *cbr_chnl = digi_cbr_channel_num_get(adpt_handle->digi_handle,
                                         adpt_handle->traffic_info[index].cbr_handle);

    /* invalid channel ID */
    if (*cbr_chnl >= DIGI_CBRC_CHNL_MAX)
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_cbr_chnl_get */


/*******************************************************************************
* adpt_etrans_chnl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves ETRANS channel ID in the ENET subsystem based on the ILKN handle. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *enet_chnl         - ETRANS channel in the ENET block.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_etrans_chnl_get(UINT32 dev_id, UINT32 chnl_id, UINT32 *enet_chnl)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(enet_chnl);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].map_handle);

    rc = digi_enet_channel_num_get(adpt_handle->digi_handle,
                                   adpt_handle->traffic_info[index].map_handle,
                                   enet_chnl);
    ADPT_RETURN_VERIFY(rc);

    /* invalid channel ID */
    if (*enet_chnl >= DIGI_ENET_CHNL_MAX)
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_etrans_chnl_get */


/*******************************************************************************
* adpt_lo_odu_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the status if TP belongs to low order or high order. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *lo_odu            - TRUE: lower order.
*                        FALSE: high order.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_lo_odu_status_get(UINT32 dev_id, 
                                    UINT32 chnl_id, 
                                    BOOL *lo_odu)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(lo_odu);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check odu_handle validation */
    if (NULL == adpt_handle->traffic_info[index].odu_handle)
    {
        PMC_RETURN(PMC_ERR_FAIL);
    }

    if (DIGI_ODU_UNCHANNELIZED == adpt_handle->traffic_info[index].payload_type)
    {
        *lo_odu = TRUE;
    }
    else
    {
        *lo_odu = FALSE;
    }

    PMC_RETURN(rc);
} /* adpt_lo_odu_status_get */


/*******************************************************************************
* adpt_msi_len_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves MSI length in COREOTN. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   msi_len            - MSI length.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_msi_len_get(UINT32 dev_id, UINT32 chnl_id, UINT32 *msi_len)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    switch (adpt_handle->traffic_info[index].oduk_type)
    {
        case DIGI_ODUK_ODU1:
            *msi_len = 2;
            break;

        case DIGI_ODUK_ODU2:
            *msi_len = 8;
            break;

        case DIGI_ODUK_ODU3:
        case DIGI_ODUK_ODU3E1:
        case DIGI_ODUK_ODU3E2:
            *msi_len = 32;
            break;

        case DIGI_ODUK_ODU4:
            *msi_len = 80;
            break;

        default:
            *msi_len = 0;
            rc = PMC_ERR_FAIL;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_msi_len_get */


/*******************************************************************************
* adpt_odu_type_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves ODUk type string. 
*
* INPUTS:
*   oduk_type          - ODUk type.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   ODUk type string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_odu_type_get(digi_oduk_t oduk_type)
{
    switch (oduk_type)
    {
        case DIGI_ODUK_ODU0:
            return "ODU0";
            break;

        case DIGI_ODUK_ODU1:
            return "ODU1";
            break;

        case DIGI_ODUK_ODU1E:
            return "ODU1E";
            break;

        case DIGI_ODUK_ODU1F:
            return "ODU1F";
            break;

        case DIGI_ODUK_ODU2:
            return "ODU2";
            break;

        case DIGI_ODUK_ODU2E:
            return "ODU2E";
            break;

        case DIGI_ODUK_ODU2F:
            return "ODU2F";
            break;

        case DIGI_ODUK_ODU3:
            return "ODU3";
            break;

        case DIGI_ODUK_ODU3E1:
            return "ODU3E1";
            break;

        case DIGI_ODUK_ODU3E2:
            return "ODU3E2";
            break;


        case DIGI_ODUK_ODU4:
            return "ODU4";
            break;

        case DIGI_ODUK_ODUFLEX:
            return "ODUF";
            break;

        default:
            return "N/A";
            break;
    }
} /* adpt_odu_type_get */


/*******************************************************************************
* adpt_otu_odu_cvt
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves high order ODUk type from OTUk type. 
*
* INPUTS:
*   otuk_type          - OTUk type.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   ODUk type.
*
* NOTES:
*
*******************************************************************************/
PUBLIC digi_oduk_t adpt_otu_odu_cvt(digi_otn_server_otuk_t otuk_type)
{
    switch (otuk_type)
    {
        case DIGI_OTN_SERVER_OTU4:
            return DIGI_ODUK_ODU4;
            break;

        case DIGI_OTN_SERVER_OTU3E2:
            return DIGI_ODUK_ODU3E2;
            break;

        case DIGI_OTN_SERVER_OTU3E1:
            return DIGI_ODUK_ODU3E1;
            break;

        case DIGI_OTN_SERVER_OTU3:
            return DIGI_ODUK_ODU3;
            break;

        case DIGI_OTN_SERVER_OTU2F:
            return DIGI_ODUK_ODU2F;
            break;

        case DIGI_OTN_SERVER_OTU1F:
            return DIGI_ODUK_ODU1F;
            break;

        case DIGI_OTN_SERVER_OTU2E:
            return DIGI_ODUK_ODU2E;
            break;

        case DIGI_OTN_SERVER_OTU1E:
            return DIGI_ODUK_ODU1E;
            break;

        case DIGI_OTN_SERVER_OTU2:
            return DIGI_ODUK_ODU2;
            break;

        default:
            break;
    }

    return DIGI_ODUK_ODU0;
} /* adpt_otu_odu_cvt */


/*******************************************************************************
* adpt_otu_deskew_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves deskew type from OTUk type. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   traffic_mode       - traffic mode.
*
* OUTPUTS:
*   *deskew_type       - deskew type.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otu_deskew_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_traffic_mode_t traffic_mode,
                                   digi_sfis_deskew_type_t *deskew_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 serdes_port = 0;
    UINT32 traffic_loc = 0;
    UINT32 lane_num = 0;
    UINT32 lane_id = 0;
    UINT32 port_lane = 0;
    digi_otn_server_otuk_t otuk_type = (digi_otn_server_otuk_t)DIGI_CLIENT_MODE_GET(traffic_mode);
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(deskew_type);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get serdes port */
    serdes_port = DIGI_SERDES_PORT_GET(chnl_id);
    /* get traffic location */
    traffic_loc = DIGI_TRAFFIC_POS_GET(traffic_mode);
    if ((serdes_port >= DIGI_SERDES_XFI_LANES_MAX) || (traffic_loc >= DIGI_SIDE_MAX))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    for (lane_id = 0; lane_id < DIGI_SERDES_XFI_LANES_MAX; lane_id++)
    {
        if (DIGI_LINE_SIDE == traffic_loc)
        {
            port_lane = adpt_handle->dev_info->line_port_lane_tx[serdes_port][lane_id];
        }
        else
        {
            port_lane = adpt_handle->dev_info->sys_port_lane_tx[serdes_port][lane_id];
        }

        if (DIGI_SERDES_PIN_NOT_USED != port_lane)
        {
            lane_num++;
        }
    }

    /* default */
    if ((DIGI_SERDES_SFI_51_INTF == adpt_handle->dev_info->serdes_type[serdes_port])
         && (DIGI_LINE_SIDE == traffic_loc))
    {
        *deskew_type = DIGI_OTN_SERVER_OIF_SFIS_DESKEW;
    }
    else
    {
        *deskew_type = DIGI_OTN_SERVER_NO_DESKEW;
    }

    switch (otuk_type)
    {
        case DIGI_OTN_SERVER_OTU4:
            if (11 == lane_num)
            {
                *deskew_type = DIGI_OTN_SERVER_OIF_SFIS_DESKEW;
            }
            break;


        case DIGI_OTN_SERVER_OTU3:
        case DIGI_OTN_SERVER_OTU3E2:
        case DIGI_OTN_SERVER_OTU3E1:
            if (5 == lane_num)
            {
                *deskew_type = DIGI_OTN_SERVER_OIF_SFIS_DESKEW;
            }
            break;

        default:
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otu_deskew_get */


/*******************************************************************************
* adpt_prbs_status_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves PRBS status string. 
*
* INPUTS:
*   status          - PRBS status.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PRBS status string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_prbs_status_str(util_patt_status_t status)
{
    switch (status)
    {
        case UTIL_PATT_STATUS_LOCKED:
            return "locked";
            break;

        case UTIL_PATT_STATUS_TRYING_TO_LOCK:
            return "trying to lock";
            break;

        case UTIL_PATT_STATUS_ZERO_DET:
            return "zero";
            break;

        case UTIL_PATT_STATUS_DISABLED:
            return "disabled";
            break;

        default:
            return "N/A";
            break;
    }
} /* adpt_prbs_status_str */


/*******************************************************************************
* adpt_ge_serdes_type_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves GE SerDes type string. 
*
* INPUTS:
*   ge_serdes         - GE SerDes type.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   GE SerDes type string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_ge_serdes_type_str(digi_ge_serdes_id_t ge_serdes)
{
    switch (ge_serdes)
    {
        case DIGI_SERDES_INTF_OHP_0:
            return "OHP_0";
            break;

        case DIGI_SERDES_INTF_OHP_1:
            return "OHP_1";
            break;

        case DIGI_SERDES_INTF_OHP_2:
            return "OHP_2";
            break;

        case DIGI_SERDES_INTF_EMGMT:
            return "EMGMT";
            break;

        case DIGI_SERDES_INTF_RI_RCP:
            return "RI_RCP";
            break;

        case DIGI_SERDES_INTF_DI_RCP:
            return "DI_RCP";
            break;

        default:
            return "N/A";
            break;
    }
} /* adpt_ge_serdes_type_str */


/*******************************************************************************
* adpt_mt_type_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves maintenance signal string. 
*
* INPUTS:
*   mt_type          - maintenance signal.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   maintenance signal string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_mt_type_str(digi_mt_type_t mt_type)
{
    switch (mt_type)
    {
        case DIGI_MT_NORMAL:
            return "NOR ";
            break;

        case DIGI_MT_AIS:
            return "AIS ";
            break;

        case DIGI_MT_OCI:
            return "OCI ";
            break;

        case DIGI_MT_LCK:
            return "LCK ";
            break;

        case DIGI_MT_AIS_NOMINAL:
            return "AISN";
            break;

        case DIGI_MT_LCK_NOMINAL:
            return "LCKN";
            break;

        default:
            return "N/A ";
            break;
    }
} /* adpt_mt_type_str */


/*******************************************************************************
* adpt_tcm_mode_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves TCM mode string. 
*
* INPUTS:
*   tcm_mode          - TCM mode.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TCM mode string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_tcm_mode_str(digi_tcm_mode_t tcm_mode)
{
    switch (tcm_mode)
    {
        case DIGI_TCM_OPERATIONAL:
            return "OPR";
            break;

        case DIGI_TCM_TRANSPARENT:
            return "TRN";
            break;

        case DIGI_TCM_MONITOR:
            return "MON";
            break;

        case DIGI_TCM_NON_INTRUSIVE_MONITOR:
            return "NIM";
            break;

        case DIGI_TCM_OPERATIONAL_OHNZ:
            return "OPN";
            break;

        default:
            return "N/A";
            break;
    }
} /* adpt_tcm_mode_str */


/*******************************************************************************
* adpt_tcm_pri_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves TCM priority string. 
*
* INPUTS:
*   tcm_pri          - TCM priority.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TCM priority string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_tcm_pri_str(digi_tcm_pri_t tcm_pri)
{
    switch (tcm_pri)
    {
        case DIGI_TCM_PRI_0:
            return "PRI0";
            break;

        case DIGI_TCM_PRI_1:
            return "PRI1";
            break;

        case DIGI_TCM_PRI_2:
            return "PRI2";
            break;

        case DIGI_TCM_PRI_3:
            return "PRI3";
            break;

        case DIGI_TCM_PRI_4:
            return "PRI4";
            break;

        case DIGI_TCM_PRI_5:
            return "PRI5";
            break;

        case DIGI_TCM_PRI_6:
            return "PRI6";
            break;

        default:
            return " N/A";
            break;
    }
} /* adpt_tcm_pri_str */


/*******************************************************************************
* adpt_tim_mode_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves dTIM detection mode string. 
*
* INPUTS:
*   tim_mode          - dTIM detection mode.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   dTIM detection mode string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_tim_mode_str(digi_otn_tim_mode_t tim_mode)
{
    switch (tim_mode)
    {
        case DIGI_TIM_MODE_OFF:
            return " OFF";
            break;

        case DIGI_TIM_MODE_SAPI:
            return "SAPI";
            break;

        case DIGI_TIM_MODE_DAPI:
            return "DAPI";
            break;

        case DIGI_TIM_MODE_SAPI_DAPI:
            return "BOTH";
            break;

        default:
            return " N/A";
            break;
    }
} /* adpt_tim_mode_str */


/*******************************************************************************
* adpt_fec_type_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves FEC type string. 
*
* INPUTS:
*  fec_type          - FEC type.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   FEC type string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_fec_type_str(digi_otn_server_fec_t fec_type)
{
    switch (fec_type)
    {
        case DIGI_OTN_SERVER_FEC_NONE:
            return "NONE";
            break;

        case DIGI_OTN_SERVER_FEC_I4:
            return " I.4";
            break;

        case DIGI_OTN_SERVER_FEC_I7:
            return " I.7";
            break;

        case DIGI_OTN_SERVER_FEC_G709:
            return "G709";
            break;

        case DIGI_OTN_SERVER_FEC_SWIZZLE_100G:
            return "SWZ1";
            break;

        case DIGI_OTN_SERVER_FEC_SWIZZLE_40G:
            return "SWZ4";
            break;



        default:
            return " N/A";
            break;
    }
} /* adpt_fec_type_str */


/*******************************************************************************
* adpt_seg_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves segment string. 
*
* INPUTS:
*  seg_id          - segment ID.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Segment ID string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_seg_str(digi_otn_seg_t seg_id)
{
    switch (seg_id)
    {
        case DIGI_OTN_SEG_PM:
            return " PM ";
            break;

        case DIGI_OTN_SEG_TCM1:
            return "TCM1";
            break;

        case DIGI_OTN_SEG_TCM2:
            return "TCM2";
            break;

        case DIGI_OTN_SEG_TCM3:
            return "TCM3";
            break;

        case DIGI_OTN_SEG_TCM4:
            return "TCM4";
            break;

        case DIGI_OTN_SEG_TCM5:
            return "TCM5";
            break;

        case DIGI_OTN_SEG_TCM6:
            return "TCM6";
            break;

        case DIGI_OTN_SEG_PM_NIM:
            return "PNIM";
            break;

        case DIGI_OTN_SEG_DCI:
            return " DCI";
            break;

        case DIGI_OTN_SEG_SM:
            return " SM ";
            break;

        default:
            return " N/A";
            break;
    }
} /* adpt_seg_str */


/*******************************************************************************
* adpt_intf_type_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves interface type string. 
*
* INPUTS:
*  intf_type       - adaptation interface type.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Segment ID string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_intf_type_str(hostmsg_adapt_intf_type_t intf_type)
{
    switch (intf_type)
    {
        case HOSTMSG_ADAPT_INTF_S16_LINE:
            return "S16 LINE";
            break;

        case HOSTMSG_ADAPT_INTF_S16_SYS:
            return "S16 SYS";
            break;

        case HOSTMSG_ADAPT_INTF_C8:
            return "C8";
            break;

        case HOSTMSG_ADAPT_INTF_T8:
            return "T8";
            break;

        default:
            return "N/A";
            break;
    }
} /* adpt_intf_type_str */


/*******************************************************************************
* adpt_deg_mode_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves dDEG mode string. 
*
* INPUTS:
*  deg_mode         - dDEG mode.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   dDEG mode string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_deg_mode_str(digi_otn_ddeg_mode_t deg_mode)
{
    switch (deg_mode)
    {
        case DIGI_OTN_HARDWARE_DDEG:
            return " HW ";
            break;

        case DIGI_OTN_ENHANCED_DDEG:
            return " ENH";
            break;

        case DIGI_OTN_ENH_BURSTY_DDEG:
            return "ENHB";
            break;

        default:
            return " N/A";
            break;
    }
} /* adpt_deg_mode_str */


/*******************************************************************************
* adpt_deg_mon_seg_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves dDEG monitor segment string. 
*
* INPUTS:
*  deg_seg         - dDEG monitor segment.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   dDEG monitor segment string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_deg_mon_seg_str(digi_otn_ddeg_mon_seg_t deg_seg)
{
    switch (deg_seg)
    {
        case DIGI_OTN_DDEG_MON_LINE:
            return "LINE";
            break;

        case DIGI_OTN_DDEG_MON_CLIENT:
            return " CLI";
            break;

        default:
            return " N/A";
            break;
    }
} /* adpt_deg_mon_seg_str */


/*******************************************************************************
* adpt_deg_interval_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves dDEG interval string. 
*
* INPUTS:
*   interval         - dDEG interval.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   dDEG interval string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_deg_interval_str(digi_deg_interval_t interval)
{
    switch (interval)
    {
        case DIGI_DEG_10MS:
            return " 10ms";
            break;

        case DIGI_DEG_100MS:
            return "100ms";
            break;

        case DIGI_DEG_1000MS:
            return "  1s ";
            break;

        default:
            return "  N/A";
            break;
    }
} /* adpt_deg_interval_str */


/*******************************************************************************
* adpt_fcs_mode_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves FCS mode string. 
*
* INPUTS:
*   fcs_mode         - FCS mode.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   FCS mode string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_fcs_mode_str(digi_gfp_fcs_mode_t fcs_mode)
{
    switch (fcs_mode)
    {
        case DIGI_GFP_NO_FCS:
            return "NO_FCS";
            break;

        case DIGI_GFP_PFCS:
            return "  PFCS";
            break;

        case DIGI_GFP_EFCS_4_BYTES:
            return " EFCS4";
            break;

        case DIGI_GFP_EFCS_8_BYTES:
            return " EFCS8";
            break;

        case DIGI_GFP_EFCS_12_BYTES:
            return "EFCS12";
            break;

        case DIGI_GFP_EFCS_16_BYTES:
            return "EFCS16";
            break;

        case DIGI_GFP_EFCS:
            return "  EFCS";
            break;

        default:
            return "   N/A";
            break;
    }
} /* adpt_fcs_mode_str */


/*******************************************************************************
* adpt_rx_fcs_mode_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves Rx FCS mode string. 
*
* INPUTS:
*   fcs_mode         - Rx FCS mode.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Rx FCS mode string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_rx_fcs_mode_str(digi_gfp_rx_fcs_mode_t fcs_mode)
{
    switch (fcs_mode)
    {
        case DIGI_GFP_RX_NO_FCS:
            return "NO_FCS";
            break;

        case DIGI_GFP_RX_PFCS:
            return "  PFCS";
            break;

        case DIGI_GFP_RX_EFCS_N_BYTES:
            return " EFCSN";
            break;

        case DIGI_GFP_RX_EFCS:
            return "  EFCS";
            break;

        default:
            return "   N/A";
            break;
    }
} /* adpt_rx_fcs_mode_str */


/*******************************************************************************
* adpt_timing_mode_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves timing mode string. 
*
* INPUTS:
*   timing_mode         - timing mode.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Timing mode string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_timing_mode_str(digi_enet_tx_timing_mode_t timing_mode)
{
    switch (timing_mode)
    {
        case DIGI_ENET_TX_LOOP_TIMING_MODE:
            return "LOOP";
            break;

        case DIGI_ENET_TX_NOMINAL_TIMING_MODE:
            return "NORM";
            break;

        case DIGI_ENET_TX_LINE_TIMING_MODE:
            return "LINE";
            break;

        case DIGI_ENET_TX_THROUGH_TIMING_MODE:
            return "TRGH";
            break;

        default:
            return " N/A";
            break;
    }
} /* adpt_timing_mode_str */


/*******************************************************************************
* adpt_tti_len_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves SDH TTI length string. 
*
* INPUTS:
*   tti_len          - TTI length.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TTI length string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_tti_len_str(digi_sdh_tti_len_t tti_len)
{
    switch (tti_len)
    {
        case DIGI_SDH_TTI_LEN_64_BYTE:
            return "64B";
            break;

        case DIGI_SDH_TTI_LEN_16_BYTE:
            return "16B";
            break;

        case DIGI_SDH_TTI_LEN_1_BYTE:
            return " 1B";
            break;

        default:
            return "N/A";
            break;
    }
} /* adpt_tti_len_str */


/*******************************************************************************
* adpt_bn_type_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves SDH B1/B2 type string. 
*
* INPUTS:
*   bn_type          - B1/B2 type.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   B1/B2 type string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_bn_type_str(digi_sdh_pm_t bn_type)
{
    switch (bn_type)
    {
        case DIGI_SDH_B1:
            return " B1";
            break;

        case DIGI_SDH_B2:
            return " B2";
            break;

        case DIGI_SDH_REI:
            return "REI";
            break;

        default:
            return "N/A";
            break;
    }
} /* adpt_bn_type_str */


/*******************************************************************************
* adpt_bn_thr_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves SDH B1/B2 threshold string. 
*
* INPUTS:
*   bn_thr          - B1/B2 threshold.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   B1/B2 threshold string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_bn_thr_str(digi_sdh_thr_t bn_thr)
{
    switch (bn_thr)
    {
        case DIGI_SDH_DISABLED:
            return " DIS";
            break;

        case DIGI_SDH_1E_04:
            return "1E04";
            break;

        case DIGI_SDH_1E_05:
            return "1E05";
            break;

        case DIGI_SDH_1E_06:
            return "1E06";
            break;

        case DIGI_SDH_1E_07:
            return "1E07";
            break;

        case DIGI_SDH_1E_08:
            return "1E08";
            break;

        case DIGI_SDH_1E_09:
            return "1E09";
            break;

        case DIGI_SDH_1E_10:
            return "1E10";
            break;

        case DIGI_SDH_1E_11:
            return "1E11";
            break;

        case DIGI_SDH_1E_12:
            return "1E12";
            break;

        default:
            return " N/A";
            break;
    }
} /* adpt_bn_thr_str */


/*******************************************************************************
* adpt_bip_mode_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves SDH B1/B2 BIP mode string. 
*
* INPUTS:
*   bip_mode          - B1/B2 BIP mode.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   B1/B2 BIP mode string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_bip_mode_str(digi_bip_mode_t bip_mode)
{
    switch (bip_mode)
    {
        case DIGI_SDH_BIP_ERR:
            return "BIP_ERR";
            break;

        case DIGI_SDH_BLOCK_ERR:
            return "BLK_ERR";
            break;

        default:
            return "    N/A";
            break;
    }
} /* adpt_bip_mode_str */


/*******************************************************************************
* adpt_dev_version_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves DIGI device reversion string. 
*
* INPUTS:
*   dev_version          - DIGI device reversion.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   DIGI device reversion string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_dev_version_str(UINT8 dev_version)
{
    switch (dev_version)
    {
        case DIGI_REVISION_CODE_REV_A:
            return "REV_A";
            break;

        case DIGI_REVISION_CODE_REV_B:
            return "REV_B";
            break;

        default:
            return "N/A";
            break;
    }
} /* adpt_dev_version_str */


/*******************************************************************************
* adpt_otn_dir_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves OTN direction string.
*
* INPUTS:
*   otn_dir          - OTN direction.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   OTN direction string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_otn_dir_str(digi_otn_dir_t otn_dir)
{
    switch (otn_dir)
    {
        case DIGI_OTN_DIR_RX:
            return "RX";
            break;

        case DIGI_OTN_DIR_TX:
            return "TX";
            break;

        case DIGI_OTN_DIR_RX_TX:
            return "RX_TX";
            break;

        default:
            return "N/A";
            break;
    }
} /* adpt_otn_dir_str */


/*******************************************************************************
* adpt_sgmii_port_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves SGMII port type string.
*
* INPUTS:
*   port_type          - SGMII port type.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   SGMII port type string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_sgmii_port_str(digi_sgmii_port_type_t port_type)
{
    switch (port_type)
    {
        case DIGI_PORT_RI_RCP:
            return "RI_RCP";
            break;

        case DIGI_PORT_DI_RCP:
            return "DI_RCP";
            break;

        case DIGI_PORT_OHP1:
            return "OHP1";
            break;

        case DIGI_PORT_OHP2:
            return "OHP2";
            break;

        case DIGI_PORT_OHP3:
            return "OHP3";
            break;

        case DIGI_PORT_MGMT:
            return "MGMT";
            break;

        default:
            return "N/A";
            break;
    }
} /* adpt_sgmii_port_str */


/*******************************************************************************
* adpt_enet_fault_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves Ethernet fault type string.
*
* INPUTS:
*   fault_type         - Ethernet fault type.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Ethernet fault type string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_enet_fault_str(digi_enet_fault_t fault_type)
{
    switch (fault_type)
    {
        case DIGI_ENET_FT_CLEAR:
            return "CLR";
            break;

        case DIGI_ENET_FT_LFI:
            return "LFI";
            break;

        case DIGI_ENET_FT_RFI:
            return "RFI";
            break;

        case DIGI_ENET_FT_AUTO:
            return "AUO";
            break;

        case DIGI_ENET_FT_UNIDIR:
            return "UNI";
            break;

        default:
            return "N/A";
            break;
    }
} /* adpt_enet_fault_str */


/*******************************************************************************
* adpt_dcsu_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves DCSU type string.
*
* INPUTS:
*   dcsu_type          - DCSU type.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   DCSU type string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_dcsu_str(digi_dcsu_inst_t dcsu_type)
{
    switch (dcsu_type)
    {
        case DIGI_DCSU_INST_OTN1:
            return "OTN1 ";
            break;

        case DIGI_DCSU_INST_OTN2:
            return "OTN2 ";
            break;

        case DIGI_DCSU_INST_ENET3:
            return "ENET3";
            break;


        case DIGI_DCSU_INST_ENET1:
            return "ENET1";
            break;

        case DIGI_DCSU_INST_ENET2:
            return "ENET2";
            break;

        case DIGI_DCSU_INST_PTP:
            return "PTP  ";
            break;

        case DIGI_DCSU_INST_SYS1:
            return "SYS1 ";
            break;

        case DIGI_DCSU_INST_SYS2:
            return "SYS2 ";
            break;

        default:
            return "N/A  ";
            break;
    }
} /* adpt_dcsu_str */


/*******************************************************************************
* adpt_odu_level_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves ODUk level string.
*
* INPUTS:
*   odu_level          - ODUk level.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   ODUk level string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_odu_level_str(odu_struct_odu_level_t odu_level)
{
    switch (odu_level)
    {
        case ODU_STRUCT_LEVEL_HO_ODU:
            return "HO_ODU";
            break;

        case ODU_STRUCT_LEVEL_MO_ODU:
            return "MO_ODU";
            break;

        case ODU_STRUCT_LEVEL_LO_ODU:
            return "LO_ODU";
            break;

        case ODU_STRUCT_LEVEL_3B_ODU:
            return "3B_ODU";
            break;

        case ODU_STRUCT_LEVEL_4_ODU:
            return "4_ODU ";
            break;

        default:
            return "N/A   ";
            break;
    }
} /* adpt_odu_level_str */


/*******************************************************************************
* adpt_dm_mode_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves OTN DM(Delay Measurement) mode string.
*
* INPUTS:
*   dm_mode          - OTN delay measurement mode.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   DM mode string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_dm_mode_str(digi_dm_mode_t dm_mode)
{
    switch (dm_mode)
    {
        case DIGI_DM_INITIATOR:
            return "INIT";
            break;

        case DIGI_DM_LOOPBACK:
            return "LOOP";
            break;

        default:
            return "N/A ";
            break;
    }
} /* adpt_dm_mode_str */


/*******************************************************************************
* adpt_ts_type_str
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves TS(tributary slot) type string.
*
* INPUTS:
*   ts_type          - TS type.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TS type string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_ts_type_str(digi_odu_line_payload_t ts_type)
{
    switch (ts_type)
    {
        case DIGI_ODU_UNCHANNELIZED:
            return "UCHL";
            break;

        case DIGI_ODU_TS_1G25:
            return "1G25";
            break;

        case DIGI_ODU_TS_2G5:
            return "2G5 ";
            break;

        case DIGI_ODU_NULL:
            return "NULL";
            break;

        default:
            return "N/A ";
            break;
    }
} /* adpt_ts_type_str */


/*******************************************************************************
* adpt_otn_dir_cvt
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Converts OTN direction from DIGI Adapter to SDK.
*
* INPUTS:
*   otn_dir          - OTN direction.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   OTN direction string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC digi_otn_asym_deprov_t adpt_otn_dir_cvt(digi_otn_dir_t otn_dir)
{
    switch (otn_dir)
    {
        case DIGI_OTN_DIR_RX:
            return DIGI_OTN_DEPROV_RX;
            break;

        case DIGI_OTN_DIR_TX:
            return DIGI_OTN_DEPROV_TX;
            break;

        case DIGI_OTN_DIR_RX_TX:
            return DIGI_OTN_DEPROV_RX_AND_TX;
            break;

        default:
            return LAST_DIGI_OTN_DEPROV_NUM;
            break;
    }
} /* adpt_otn_dir_cvt */


/*******************************************************************************
* adpt_parent_segid_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves parent segment identity. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity. Only be available for PM, TCM1 - TCM6.
*   location           - line side(stage 1/2/3A) or system side (stage 3B).
*                        Not available for stage 4.
*   position           - sink/source position.
*
* OUTPUTS:
*   *p_seg_id          - parent segment identity. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_parent_segid_get(UINT32 dev_id, 
                                   UINT32 chnl_id, 
                                   digi_otn_seg_t seg_id,
                                   digi_otn_loc_t location,
                                   digi_otn_pos_t position,
                                   digi_otn_seg_t *p_seg_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 tcm_index = 0;
    BOOL p_exist = FALSE;
    UINT16 tcm_pool_id = 0;
    BOOL lo_odu = FALSE;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(p_seg_id);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* parameter check */
    if (((seg_id > DIGI_OTN_SEG_TCM6) && (DIGI_OTN_SEG_PM_NIM != seg_id)) 
        || (location >= LAST_DIGI_OTN_SIDE)
        || (position >= LAST_DIGI_OTN_POS))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* PM_NIM can be only connected to DCI */
    if (DIGI_OTN_SEG_PM_NIM == seg_id)
    {
        *p_seg_id = DIGI_OTN_SEG_DCI;
        PMC_RETURN(rc);
    }

    for (tcm_index = DIGI_OTN_SEG_PM; tcm_index <= DIGI_OTN_SEG_TCM6; tcm_index++)
    {
        /* remove self */
        if (seg_id == tcm_index)
        {
            continue;
        }

        tcm_pool_id = adpt_handle->traffic_info[index].tcm_pool_id[location*2+position][tcm_index];
        if ((DIGI_TCM_EN_GET(tcm_pool_id) == DIGI_TCM_ENABLE)
             && (DIGI_TCM_MODE_GET(tcm_pool_id) != DIGI_TCM_NON_INTRUSIVE_MONITOR)
             && (adpt_handle->traffic_info[index].tcm_pri[location*2+position][tcm_index] 
             < adpt_handle->traffic_info[index].tcm_pri[location*2+position][seg_id]))
        {
            if (p_exist)
            {
                if (adpt_handle->traffic_info[index].tcm_pri[location*2+position][tcm_index] 
                    > adpt_handle->traffic_info[index].tcm_pri[location*2+position][*p_seg_id])
                {
                    *p_seg_id = (digi_otn_seg_t)tcm_index;
                }
            }
            else
            {
                *p_seg_id = (digi_otn_seg_t)tcm_index;
                p_exist   = TRUE;
            }
        }
    }

    if (!p_exist)
    {
        if (DIGI_OTN_SINK_POS == position)
        {
            *p_seg_id = DIGI_OTN_SEG_DCI;
        }
        else
        {
            adpt_lo_odu_status_get(dev_id, chnl_id, &lo_odu);
            if ((!lo_odu) || (NULL != adpt_handle->traffic_info[index].map_handle)
                || adpt_handle->traffic_info[index].prbs_enable)
            {
                *p_seg_id = DIGI_OTN_SEG_PM;
            }
            else
            {
                *p_seg_id = DIGI_OTN_SEG_DCI;
            }
        }
    }

    PMC_RETURN(rc);
} /* adpt_parent_segid_get */


/*******************************************************************************
* adpt_otu_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the DIGI Adapter OTU handle pointer for specific usage.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   **digi_handle      - pointer to DIGI handle.
*   **chnl_handle      - pointer to OTU handle.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otu_handle_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_handle_t **digi_handle,
                                  digi_otn_server_chnl_t **chnl_handle)
{
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(digi_handle);
    ADPT_PARA_VERIFY(chnl_handle);
    
    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    if (NULL != adpt_handle->traffic_info[index].otu_handle)
    {
        *digi_handle = adpt_handle->digi_handle;
        *chnl_handle = adpt_handle->traffic_info[index].otu_handle;
    }
    else
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* handle verification */
    ADPT_PARA_VERIFY(*digi_handle);
    ADPT_PARA_VERIFY(*chnl_handle);

    PMC_RETURN(rc);
}/* adpt_otu_handle_get */


/*******************************************************************************
* adpt_odu_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the DIGI Adapter ODU handle container pointer for specific usage.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   location           - line side(stage 1/2/3A) or system side (stage 3B).
*                        Not availabe for stage 4.
*
* OUTPUTS:
*   **digi_handle      - pointer to DIGI handle.
*   **odu_handle       - pointer to ODU handle.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_odu_handle_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_otn_loc_t location,
                                   digi_handle_t **digi_handle,
                                   digi_otn_odu_chnl_t **odu_handle)
{
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(odu_handle);
    
    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    if (NULL != adpt_handle->traffic_info[index].odu_handle)
    {
        switch (location)
        {
            case DIGI_OTN_LINE_SIDE:
                *digi_handle = adpt_handle->digi_handle;
                *odu_handle = adpt_handle->traffic_info[index].odu_handle;
                break;

            case DIGI_OTN_SYS_SIDE:
                *digi_handle = adpt_handle->digi_handle;
                rc = digi_otn_get_chnl_ptr_for_tcm_apps(adpt_handle->digi_handle, 
                                                        adpt_handle->traffic_info[index].odu_handle,
                                                        odu_handle);
                ADPT_RETURN_VERIFY(rc);
                break;

            default:
                PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                break;
        }
    }
    else if (NULL != adpt_handle->traffic_info[index].map_handle)
    {
        *digi_handle = adpt_handle->digi_handle;
        map_handle = adpt_handle->traffic_info[index].map_handle;
        /* get stage 4 handle */
        rc = digi_otn_get_chnl_ptr_for_tcm_apps(adpt_handle->digi_handle, 
                                                (digi_otn_odu_chnl_t*)map_handle,
                                                odu_handle);
        ADPT_RETURN_VERIFY(rc);
    }
    else
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* handle verification */
    ADPT_PARA_VERIFY(*digi_handle);
    ADPT_PARA_VERIFY(*odu_handle);

    PMC_RETURN(rc);
}/* adpt_odu_handle_get */


/*******************************************************************************
* adpt_enet_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the DIGI Adapter ENET handle pointer for specific usage.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   **digi_handle      - pointer to DIGI handle.
*   **chnl_handle      - pointer to ENET or MAPOTN handle.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_handle_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_handle_t **digi_handle,
                                   util_global_switch_data_t **chnl_handle)
{
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(digi_handle);
    ADPT_PARA_VERIFY(chnl_handle);
    
    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    if (NULL != adpt_handle->traffic_info[index].enet_handle)
    {
        *digi_handle = adpt_handle->digi_handle;
        *chnl_handle = adpt_handle->traffic_info[index].enet_handle;
    }
    else if (NULL != adpt_handle->traffic_info[index].map_handle)
    {
        *digi_handle = adpt_handle->digi_handle;
        *chnl_handle = adpt_handle->traffic_info[index].map_handle;
    }
    else
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* handle verification */
    ADPT_PARA_VERIFY(*digi_handle);
    ADPT_PARA_VERIFY(*chnl_handle);

    PMC_RETURN(rc);
}/* adpt_enet_handle_get */


/*******************************************************************************
* adpt_cbr_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the DIGI Adapter CBR handle pointer for specific usage.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   **digi_handle      - pointer to DIGI handle.
*   **chnl_handle      - pointer to CBR handle.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cbr_handle_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_handle_t **digi_handle,
                                   digi_cbr_client_chnl_t **chnl_handle)
{
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(digi_handle);
    ADPT_PARA_VERIFY(chnl_handle);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    if (NULL != adpt_handle->traffic_info[index].cbr_handle)
    {
        *digi_handle = adpt_handle->digi_handle;
        *chnl_handle = adpt_handle->traffic_info[index].cbr_handle;
    }
    else
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* handle verification */
    ADPT_PARA_VERIFY(*digi_handle);
    ADPT_PARA_VERIFY(*chnl_handle);

    PMC_RETURN(rc);
}/* adpt_cbr_handle_get */


/*******************************************************************************
* adpt_map_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the DIGI Adapter MAPOTN handle pointer for specific usage.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   **digi_handle      - pointer to DIGI handle.
*   **chnl_handle      - pointer to MAPOTN handle.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_map_handle_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_handle_t **digi_handle,
                                   digi_mapper_chnl_t **chnl_handle)
{
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(digi_handle);
    ADPT_PARA_VERIFY(chnl_handle);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    if (NULL != adpt_handle->traffic_info[index].map_handle)
    {
        *digi_handle = adpt_handle->digi_handle;
        *chnl_handle = adpt_handle->traffic_info[index].map_handle;
    }
    else
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* handle verification */
    ADPT_PARA_VERIFY(*digi_handle);
    ADPT_PARA_VERIFY(*chnl_handle);

    PMC_RETURN(rc);
}/* adpt_map_handle_get */


/*******************************************************************************
* adpt_ilkn_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the DIGI Adapter ILKN handle pointer for specific usage.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   **digi_handle      - pointer to DIGI handle.
*   **chnl_handle      - pointer to ILKN handle.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_ilkn_handle_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_handle_t **digi_handle,
                                  digi_ilkn_chnl_t **chnl_handle)
{
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(digi_handle);
    ADPT_PARA_VERIFY(chnl_handle);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    if (NULL != adpt_handle->traffic_info[index].ilkn_handle)
    {
        *digi_handle = adpt_handle->digi_handle;
        *chnl_handle = adpt_handle->traffic_info[index].ilkn_handle;
    }
    else
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* handle verification */
    ADPT_PARA_VERIFY(*digi_handle);
    ADPT_PARA_VERIFY(*chnl_handle);

    PMC_RETURN(rc);
}/* adpt_ilkn_handle_get */


/*******************************************************************************
* adpt_enet_pmon_merge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Merges ENET PMON data from shared memory to Adapter. 
*
* INPUTS:
*   *src_pmon          - shared memory ENET PMON data.
*   *dest_pmon         - Adapter ENET PMON data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_pmon_merge(digi_enet_pmon_t *src_pmon,
                                    digi_enet_pmon_t *dest_pmon)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 i = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(src_pmon);
    ADPT_PARA_VERIFY(dest_pmon);

    for (index = 0; index < DIGI_ENET_CHNL_MAX; index++)
    {
        dest_pmon->handle[index] = src_pmon->handle[index];
        if (NULL == src_pmon->handle[index])
        {
            continue;
        }

        for (i = 0; i < (sizeof(digi_enet_pmon_chnl_t)/4); i++)
        {
            *((UINT32*)(&dest_pmon->enet_chnl[index]) + i) += 
            PMC_LE_TO_CPU_32(*((UINT32*)(&src_pmon->enet_chnl[index]) + i));
        }
    }

    PMC_RETURN(rc);
} /* adpt_enet_pmon_merge */


/*******************************************************************************
* adpt_odu_pmon_merge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Merges ODU PMON data from shared memory to Adapter. 
*
* INPUTS:
*   *src_pmon          - shared memory ODU PMON data.
*   *dest_pmon         - Adapter ODU PMON data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_odu_pmon_merge(digi_otn_odu_pmon_chnl_data_t *src_pmon,
                                    digi_otn_odu_pmon_chnl_data_t *dest_pmon)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 i = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(src_pmon);
    ADPT_PARA_VERIFY(dest_pmon);

    /* PM */
    for (i = 0; i < (sizeof(digi_pmon_coreotn_odukp_counters_t)/4); i++)
    {
        *((UINT32*)(&src_pmon->pm) + i) = 
        PMC_LE_TO_CPU_32(*((UINT32*)(&src_pmon->pm) + i));
    }
    dest_pmon->pm.odukp_sk_pn_ds_pm = src_pmon->pm.odukp_sk_pn_ds_pm;
    dest_pmon->pm.odukp_sk_pn_ebc_pm += src_pmon->pm.odukp_sk_pn_ebc_pm;
    dest_pmon->pm.odukp_sk_pf_ds_pm = src_pmon->pm.odukp_sk_pf_ds_pm;
    dest_pmon->pm.odukp_sk_pf_ebc_pm += src_pmon->pm.odukp_sk_pf_ebc_pm;

    /* TCM */
    for (index = 0; index < DIGI_OTN_SERVER_PIDS_PER_CHNL; index++)
    {
        dest_pmon->tcm_pool_ids[index] = src_pmon->tcm_pool_ids[index];
        for (i = 0; i < (sizeof(digi_pmon_coreotn_odukt_counters_t)/4); i++)
        {
            *((UINT32*)(&src_pmon->tcm[index]) + i) = 
            PMC_LE_TO_CPU_32(*((UINT32*)(&src_pmon->tcm[index]) + i));
        }
        dest_pmon->tcm[index].odukt_sk_piae = src_pmon->tcm[index].odukt_sk_piae;
        dest_pmon->tcm[index].odukt_sk_pn_ds = src_pmon->tcm[index].odukt_sk_pn_ds;
        dest_pmon->tcm[index].odukt_sk_pn_ebc += src_pmon->tcm[index].odukt_sk_pn_ebc;
        dest_pmon->tcm[index].odukt_sk_pbiae = src_pmon->tcm[index].odukt_sk_pbiae;
        dest_pmon->tcm[index].odukt_sk_pf_ds = src_pmon->tcm[index].odukt_sk_pf_ds;
        dest_pmon->tcm[index].odukt_sk_pf_ebc += src_pmon->tcm[index].odukt_sk_pf_ebc;
    }

    /* PRBS */
    dest_pmon->prbs_valid = src_pmon->prbs_valid;
    for (i = 0; i < (sizeof(digi_pmon_coreotn_odukp_prbs_counter_t)/4); i++)
    {
        *((UINT32*)(&dest_pmon->prbs) + i) += 
        PMC_LE_TO_CPU_32(*((UINT32*)(&src_pmon->prbs) + i));
    }

    PMC_RETURN(rc);
} /* adpt_otn_pmon_merge */


/*******************************************************************************
* adpt_otn_pmon_merge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Merges OTN PMON data from shared memory to Adapter. 
*
* INPUTS:
*   *src_pmon          - shared memory OTN PMON data.
*   *dest_pmon         - Adapter OTN PMON data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_pmon_merge(digi_otn_pmon_t *src_pmon,
                                    digi_otn_pmon_t *dest_pmon)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 i = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(src_pmon);
    ADPT_PARA_VERIFY(dest_pmon);

    /* LINEOTN */
    for (index = 0; index < DIGI_OTN_SERVER_CHNL_MAX; index++)
    {
        dest_pmon->otn_server_handle[index] = src_pmon->otn_server_handle[index];
        if (NULL == src_pmon->otn_server_handle[index])
        {
            continue;
        }

        /* OTU PMON */
        for (i = 0; i < (sizeof(digi_pmon_lineotn_otu_frm_counters_t)/4); i++)
        {
            *((UINT32*)(&src_pmon->otn_server_chnl[index].framer) + i) = 
            PMC_LE_TO_CPU_32(*((UINT32*)(&src_pmon->otn_server_chnl[index].framer) + i));
        }
        dest_pmon->otn_server_chnl[index].framer.otu_frm_sk_sm_p_iae = 
        src_pmon->otn_server_chnl[index].framer.otu_frm_sk_sm_p_iae;
        dest_pmon->otn_server_chnl[index].framer.otu_frm_sk_sm_p_n_ds = 
        src_pmon->otn_server_chnl[index].framer.otu_frm_sk_sm_p_n_ds;
        dest_pmon->otn_server_chnl[index].framer.otu_frm_sk_sm_p_n_ebc += 
        src_pmon->otn_server_chnl[index].framer.otu_frm_sk_sm_p_n_ebc;
        dest_pmon->otn_server_chnl[index].framer.otu_frm_sk_sm_p_biae = 
        src_pmon->otn_server_chnl[index].framer.otu_frm_sk_sm_p_biae;
        dest_pmon->otn_server_chnl[index].framer.otu_frm_sk_sm_p_f_ds = 
        src_pmon->otn_server_chnl[index].framer.otu_frm_sk_sm_p_f_ds;
        dest_pmon->otn_server_chnl[index].framer.otu_frm_sk_sm_p_f_ebc += 
        src_pmon->otn_server_chnl[index].framer.otu_frm_sk_sm_p_f_ebc;

        /* FEC */
        for (i = 0; i < (sizeof(digi_pmon_lineotn_fec_counters_t)/4); i++)
        {
            *((UINT32*)(&dest_pmon->otn_server_chnl[index].fec) + i) += 
            PMC_LE_TO_CPU_32(*((UINT32*)(&src_pmon->otn_server_chnl[index].fec) + i));
        }
    }

    /* SYSOTN */
    for (index = 0; index < DIGI_OTN_SERVER_SYS_CHNL_MAX; index++)
    {
        dest_pmon->sysotn_server_handle[index] = src_pmon->sysotn_server_handle[index];
        if (NULL == src_pmon->sysotn_server_handle[index])
        {
            continue;
        }

        /* OTU PMON */
        for (i = 0; i < (sizeof(digi_pmon_lineotn_otu_frm_counters_t)/4); i++)
        {
            *((UINT32*)(&src_pmon->sysotn_server_chnl[index].framer) + i) = 
            PMC_LE_TO_CPU_32(*((UINT32*)(&src_pmon->sysotn_server_chnl[index].framer) + i));
        }
        dest_pmon->sysotn_server_chnl[index].framer.otu_frm_sk_sm_p_iae = 
        src_pmon->sysotn_server_chnl[index].framer.otu_frm_sk_sm_p_iae;
        dest_pmon->sysotn_server_chnl[index].framer.otu_frm_sk_sm_p_n_ds = 
        src_pmon->sysotn_server_chnl[index].framer.otu_frm_sk_sm_p_n_ds;
        dest_pmon->sysotn_server_chnl[index].framer.otu_frm_sk_sm_p_n_ebc += 
        src_pmon->sysotn_server_chnl[index].framer.otu_frm_sk_sm_p_n_ebc;
        dest_pmon->sysotn_server_chnl[index].framer.otu_frm_sk_sm_p_biae = 
        src_pmon->sysotn_server_chnl[index].framer.otu_frm_sk_sm_p_biae;
        dest_pmon->sysotn_server_chnl[index].framer.otu_frm_sk_sm_p_f_ds = 
        src_pmon->sysotn_server_chnl[index].framer.otu_frm_sk_sm_p_f_ds;
        dest_pmon->sysotn_server_chnl[index].framer.otu_frm_sk_sm_p_f_ebc += 
        src_pmon->sysotn_server_chnl[index].framer.otu_frm_sk_sm_p_f_ebc;

        /* FEC */
        for (i = 0; i < (sizeof(digi_pmon_lineotn_fec_counters_t)/4); i++)
        {
            *((UINT32*)(&dest_pmon->sysotn_server_chnl[index].fec) + i) += 
            PMC_LE_TO_CPU_32(*((UINT32*)(&src_pmon->sysotn_server_chnl[index].fec) + i));
        }
    }

    /* high order */
    for (index = 0; index < DIGI_OTN_SERVER_HO_CHNL_MAX; index++)
    {
        dest_pmon->otn_odu_ho_handle[index] = src_pmon->otn_odu_ho_handle[index];
        if (NULL == src_pmon->otn_odu_ho_handle[index])
        {
            continue;
        }

        rc = adpt_odu_pmon_merge(&src_pmon->otn_odu_ho_chnl[index].pmon,
                                 &dest_pmon->otn_odu_ho_chnl[index].pmon);
        dest_pmon->otn_odu_ho_chnl[index].stg3b_valid = 
        src_pmon->otn_odu_ho_chnl[index].stg3b_valid;
        rc = adpt_odu_pmon_merge(&src_pmon->otn_odu_ho_chnl[index].stg3b,
                                 &dest_pmon->otn_odu_ho_chnl[index].stg3b);
    }

    /* middle order */
    for (index = 0; index < DIGI_OTN_SERVER_MO_CHNL_MAX; index++)
    {
        dest_pmon->otn_odu_mo_handle[index] = src_pmon->otn_odu_mo_handle[index];
        if (NULL == src_pmon->otn_odu_mo_handle[index])
        {
            continue;
        }

        rc = adpt_odu_pmon_merge(&src_pmon->otn_odu_mo_chnl[index].pmon,
                                 &dest_pmon->otn_odu_mo_chnl[index].pmon);
        dest_pmon->otn_odu_mo_chnl[index].stg3b_valid = 
        src_pmon->otn_odu_mo_chnl[index].stg3b_valid;
        rc = adpt_odu_pmon_merge(&src_pmon->otn_odu_mo_chnl[index].stg3b,
                                 &dest_pmon->otn_odu_mo_chnl[index].stg3b);
    }

    /* low order */
    for (index = 0; index < DIGI_OTN_SERVER_LO_CHNL_MAX; index++)
    {
        dest_pmon->otn_odu_lo_handle[index] = src_pmon->otn_odu_lo_handle[index];
        if (NULL == src_pmon->otn_odu_lo_handle[index])
        {
            continue;
        }

        rc = adpt_odu_pmon_merge(&src_pmon->otn_odu_lo_chnl[index].pmon,
                                 &dest_pmon->otn_odu_lo_chnl[index].pmon);
        dest_pmon->otn_odu_lo_chnl[index].stg3b_valid = 
        src_pmon->otn_odu_lo_chnl[index].stg3b_valid;
        rc = adpt_odu_pmon_merge(&src_pmon->otn_odu_lo_chnl[index].stg3b,
                                 &dest_pmon->otn_odu_lo_chnl[index].stg3b);
    }

    PMC_RETURN(rc);
} /* adpt_otn_pmon_merge */


/*******************************************************************************
* adpt_mapper_pmon_merge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Merges mapper PMON data from shared memory to Adapter. 
*
* INPUTS:
*   *src_pmon          - shared memory mapper PMON data.
*   *dest_pmon         - Adapter mapper PMON data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_mapper_pmon_merge(digi_mapper_pmon_t *src_pmon,
                                       digi_mapper_pmon_t *dest_pmon)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 i = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(src_pmon);
    ADPT_PARA_VERIFY(dest_pmon);

    /* MAPOTN */
    for (index = 0; index < DIGI_MAPOTN_CHNL_MAX; index++)
    {
        dest_pmon->handle[index] = src_pmon->handle[index];
        if (NULL == src_pmon->handle[index])
        {
            continue;
        }

        /* MAPOTN PMON */
        for (i = 0; i < (sizeof(digi_pmon_mapotn_counts_t)/4); i++)
        {
            *((UINT32*)(&dest_pmon->mapper_chnl[index].counts) + i) += 
            PMC_LE_TO_CPU_32(*((UINT32*)(&src_pmon->mapper_chnl[index].counts) + i));
        }

        /* ENET */
        dest_pmon->mapper_chnl[index].enet_mode = 
        src_pmon->mapper_chnl[index].enet_mode;
        for (i = 0; i < (sizeof(digi_enet_pmon_chnl_t)/4); i++)
        {
            *((UINT32*)(&dest_pmon->mapper_chnl[index].enet) + i) += 
            PMC_LE_TO_CPU_32(*((UINT32*)(&src_pmon->mapper_chnl[index].enet) + i));
        }
    }

    PMC_RETURN(rc);
} /* adpt_mapper_pmon_merge */


/*******************************************************************************
* adpt_mapper_otn_pmon_merge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Merges mapper OTN PMON data from shared memory to Adapter. 
*
* INPUTS:
*   *src_pmon          - shared memory mapper OTN PMON data.
*   *dest_pmon         - Adapter mapper OTN PMON data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_mapper_otn_pmon_merge(digi_mapper_pmon_fo2_t *src_pmon,
                                           digi_mapper_pmon_fo2_t *dest_pmon)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(src_pmon);
    ADPT_PARA_VERIFY(dest_pmon);

    /* MAPOTN */
    for (index = 0; index < DIGI_MAPOTN_CHNL_MAX; index++)
    {
        dest_pmon->handle[index] = src_pmon->handle[index];
        if (NULL == src_pmon->handle[index])
        {
            continue;
        }

        /* ODU PMON */
        rc = adpt_odu_pmon_merge(&src_pmon->mapper_fo2_chnl[index].pmon,
                                 &dest_pmon->mapper_fo2_chnl[index].pmon);
        dest_pmon->mapper_fo2_chnl[index].stg3b_valid = 
        src_pmon->mapper_fo2_chnl[index].stg3b_valid;
        rc = adpt_odu_pmon_merge(&src_pmon->mapper_fo2_chnl[index].stg3b,
                                 &dest_pmon->mapper_fo2_chnl[index].stg3b);
    }

    PMC_RETURN(rc);
} /* adpt_mapper_otn_pmon_merge */


/*******************************************************************************
* adpt_sifd_pmon_merge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Merges SIFD PMON data from shared memory to Adapter. 
*
* INPUTS:
*   *src_pmon          - shared memory SIFD PMON data.
*   *dest_pmon         - Adapter SIFD PMON data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_sifd_pmon_merge(digi_sifd_pmon_t *src_pmon,
                                    digi_sifd_pmon_t *dest_pmon)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 i = 0;
    UINT32 j = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(src_pmon);
    ADPT_PARA_VERIFY(dest_pmon);

    for (index = 0; index < DIGI_SIFD_ILKN_NUM; index++)
    {
        dest_pmon->handle[index] = src_pmon->handle[index];
        if (NULL == src_pmon->handle[index])
        {
            continue;
        }

        /* SIFD */
        for (i = 0; i < (sizeof(digi_pmon_sifd_t)/4); i++)
        {
            *((UINT32*)(&dest_pmon->sifd_link[index].pmon) + i) += 
            PMC_LE_TO_CPU_32(*((UINT32*)(&src_pmon->sifd_link[index].pmon) + i));
        }

        /* ENET */
        for (j = 0; j < DIGI_ENET_CHNL_MAX; j++)
        {
            dest_pmon->sifd_link[index].enet_mode[j] = 
            src_pmon->sifd_link[index].enet_mode[j];
            dest_pmon->sifd_link[index].ilkn_handle[j] = 
            src_pmon->sifd_link[index].ilkn_handle[j];
            for (i = 0; i < (sizeof(digi_enet_pmon_chnl_t)/4); i++)
            {
                *((UINT32*)(&dest_pmon->sifd_link[index].enet[j]) + i) += 
                PMC_LE_TO_CPU_32(*((UINT32*)(&src_pmon->sifd_link[index].enet[j]) + i));
            }
        }
    }

    PMC_RETURN(rc);
} /* adpt_sifd_pmon_merge */


/*******************************************************************************
* adpt_cbr_pmon_merge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Merges CBR PMON data from shared memory to Adapter. 
*
* INPUTS:
*   *src_pmon          - shared memory CBR PMON data.
*   *dest_pmon         - Adapter CBR PMON data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cbr_pmon_merge(digi_cbr_pmon_t *src_pmon,
                                    digi_cbr_pmon_t *dest_pmon)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 i = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(src_pmon);
    ADPT_PARA_VERIFY(dest_pmon);

    for (index = 0; index < DIGI_CBRC_CHNL_MAX; index++)
    {
        dest_pmon->handle[index] = src_pmon->handle[index];
        if (NULL == src_pmon->handle[index])
        {
            continue;
        }

        for (i = 0; i < (sizeof(digi_cbr_pmon_chnl_t)/4); i++)
        {
            *((UINT32*)(&dest_pmon->cbr_chnl[index].sdh_pmg) + i) += 
            PMC_LE_TO_CPU_32(*((UINT32*)(&src_pmon->cbr_chnl[index].sdh_pmg) + i));
        }
    }

    PMC_RETURN(rc);
} /* adpt_cbr_pmon_merge */


/*******************************************************************************
* adpt_enet_chnl_int_merge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Merges ENET defect and interrupt data from shared memory to Adapter per channel. 
*
* INPUTS:
*   *src_int           - shared memory ENET defect and int data.
*   *dest_int          - Adapter ENET defect and int data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_chnl_int_merge(enet_int_chnl_t *src_int,
                                      enet_int_chnl_t *dest_int)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index  = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(src_int);
    ADPT_PARA_VERIFY(dest_int);

    /* EMAC */
    dest_int->emac.phy_los_i            |= src_int->emac.phy_los_i; 
    dest_int->emac.rx_loc_fault_i       |= src_int->emac.rx_loc_fault_i;
    dest_int->emac.rx_rem_fault_i       |= src_int->emac.rx_rem_fault_i;
    dest_int->emac.hi_ber_i             |= src_int->emac.hi_ber_i;
    dest_int->emac.block_lock_i         |= src_int->emac.block_lock_i;
    dest_int->emac.align_status_i       |= src_int->emac.align_status_i;
    dest_int->emac.emac_tx_overflow_i   |= src_int->emac.emac_tx_overflow_i;
    dest_int->emac.emac_tx_underflow_i  |= src_int->emac.emac_tx_underflow_i;

    dest_int->emac.phy_los_v             = src_int->emac.phy_los_v;
    dest_int->emac.rx_loc_fault_v        = src_int->emac.rx_loc_fault_v;
    dest_int->emac.rx_rem_fault_v        = src_int->emac.rx_rem_fault_v;
    dest_int->emac.hi_ber_v              = src_int->emac.hi_ber_v;
    dest_int->emac.block_lock_v          = src_int->emac.block_lock_v;
    dest_int->emac.align_status_v        = src_int->emac.align_status_v;
    dest_int->emac.link_status           = src_int->emac.link_status;
    dest_int->emac.all_lanes_lock        = src_int->emac.all_lanes_lock;
    dest_int->emac.lane_19_12_block_lock = src_int->emac.lane_19_12_block_lock;
    dest_int->emac.lane_7_0_block_lock   = src_int->emac.lane_7_0_block_lock;
    dest_int->emac.lane_align_lock_19_12 = src_int->emac.lane_align_lock_19_12;
    dest_int->emac.lane_align_lock_7_0   = src_int->emac.lane_align_lock_7_0;

    /* ETRANS */
    dest_int->etrans.exco3m_valid = src_int->etrans.exco3m_valid; 
    if (src_int->etrans.exco3m_valid)
    {
        dest_int->etrans.exco3m.tx_los_i |= src_int->etrans.exco3m.tx_los_i;
        dest_int->etrans.exco3m.tx_lf_os_det_i |= src_int->etrans.exco3m.tx_lf_os_det_i;
        dest_int->etrans.exco3m.tx_rf_os_det_i |= src_int->etrans.exco3m.tx_rf_os_det_i;
        dest_int->etrans.exco3m.tx_idle_test_err_det_i |= src_int->etrans.exco3m.tx_idle_test_err_det_i;
        dest_int->etrans.exco3m.tx_skew_range_viol_i |= src_int->etrans.exco3m.tx_skew_range_viol_i;
        dest_int->etrans.exco3m.tx_pcs_inv_blk_det_i |= src_int->etrans.exco3m.tx_pcs_inv_blk_det_i;
        dest_int->etrans.exco3m.tx_pcs_err_blk_det_i |= src_int->etrans.exco3m.tx_pcs_err_blk_det_i;
        dest_int->etrans.exco3m.tx_pcs_hber_i |= src_int->etrans.exco3m.tx_pcs_hber_i;
        dest_int->etrans.exco3m.rx_tsf_i |= src_int->etrans.exco3m.rx_tsf_i;
        dest_int->etrans.exco3m.rx_lf_os_det_i |= src_int->etrans.exco3m.rx_lf_os_det_i;
        dest_int->etrans.exco3m.rx_rf_os_det_i |= src_int->etrans.exco3m.rx_rf_os_det_i;
        dest_int->etrans.exco3m.rx_idle_test_err_det_i |= src_int->etrans.exco3m.rx_idle_test_err_det_i;
        dest_int->etrans.exco3m.rx_pcs_err_blk_det_i |= src_int->etrans.exco3m.rx_pcs_err_blk_det_i;
        dest_int->etrans.exco3m.rx_1027b_lobl_i |= src_int->etrans.exco3m.rx_1027b_lobl_i;
        dest_int->etrans.exco3m.rx_1027b_hber_i |= src_int->etrans.exco3m.rx_1027b_hber_i;
        dest_int->etrans.exco3m.rx_1027b_ber_i |= src_int->etrans.exco3m.rx_1027b_ber_i;
        dest_int->etrans.exco3m.rx_skew_range_viol_i |= src_int->etrans.exco3m.rx_skew_range_viol_i;
        dest_int->etrans.exco3m.rx_513b_pos_err_det_i |= src_int->etrans.exco3m.rx_513b_pos_err_det_i;
        dest_int->etrans.exco3m.rx_well_pkt_err_i |= src_int->etrans.exco3m.rx_well_pkt_err_i;
        dest_int->etrans.exco3m.rx_lane_algn_mrk_err_i |= src_int->etrans.exco3m.rx_lane_algn_mrk_err_i;
        dest_int->etrans.exco3m.rx_1027b_par_err_i |= src_int->etrans.exco3m.rx_1027b_par_err_i;

        for (index = 0; index < EXCO3M_N_LANES; index++)
        {
            dest_int->etrans.exco3m.tx_pcs_lobl_i[index] |= src_int->etrans.exco3m.tx_pcs_lobl_i[index];
            dest_int->etrans.exco3m.tx_pcs_ber_det_i[index] |= src_int->etrans.exco3m.tx_pcs_ber_det_i[index];
            dest_int->etrans.exco3m.tx_pcs_bip_err_det_i[index] |= src_int->etrans.exco3m.tx_pcs_bip_err_det_i[index];
            dest_int->etrans.exco3m.tx_loam_i[index] |= src_int->etrans.exco3m.tx_loam_i[index];
            dest_int->etrans.exco3m.tx_mrk_inv_det_i[index] |= src_int->etrans.exco3m.tx_mrk_inv_det_i[index];
            dest_int->etrans.exco3m.rx_otn_bip_err_det_i[index] |= src_int->etrans.exco3m.rx_otn_bip_err_det_i[index];
            dest_int->etrans.exco3m.rx_loam_i[index] |= src_int->etrans.exco3m.rx_loam_i[index];
            dest_int->etrans.exco3m.rx_pcs_mrk_inv_det_i[index] |= src_int->etrans.exco3m.rx_pcs_mrk_inv_det_i[index];
        }

        dest_int->etrans.exco3m.tx_los_v        = src_int->etrans.exco3m.tx_los_v;
        dest_int->etrans.exco3m.tx_pcs_hber_v   = src_int->etrans.exco3m.tx_pcs_hber_v;
        dest_int->etrans.exco3m.rx_tsf_v        = src_int->etrans.exco3m.rx_tsf_v;
        dest_int->etrans.exco3m.rx_1027b_lobl_v = src_int->etrans.exco3m.rx_1027b_lobl_v;
        dest_int->etrans.exco3m.rx_1027b_hber_v = src_int->etrans.exco3m.rx_1027b_hber_v;

        for (index = 0; index < EXCO3M_N_LANES; index++)
        {
            dest_int->etrans.exco3m.tx_pcs_lobl_v[index] = src_int->etrans.exco3m.tx_pcs_lobl_v[index];
            dest_int->etrans.exco3m.tx_loam_v[index] = src_int->etrans.exco3m.tx_loam_v[index];
            dest_int->etrans.exco3m.rx_loam_v[index] = src_int->etrans.exco3m.rx_loam_v[index];
        }
    }

    dest_int->etrans.exco4m_valid = src_int->etrans.exco4m_valid; 
    if (src_int->etrans.exco4m_valid)
    {
        dest_int->etrans.exco4m.tx_los_i |= src_int->etrans.exco4m.tx_los_i;
        dest_int->etrans.exco4m.tx_lf_os_det_i |= src_int->etrans.exco4m.tx_lf_os_det_i;
        dest_int->etrans.exco4m.tx_rf_os_det_i |= src_int->etrans.exco4m.tx_rf_os_det_i;
        dest_int->etrans.exco4m.tx_idle_test_err_det_i |= src_int->etrans.exco4m.tx_idle_test_err_det_i;
        dest_int->etrans.exco4m.tx_skew_range_viol_i |= src_int->etrans.exco4m.tx_skew_range_viol_i;
        dest_int->etrans.exco4m.tx_pcs_inv_blk_det_i |= src_int->etrans.exco4m.tx_pcs_inv_blk_det_i;
        dest_int->etrans.exco4m.tx_pcs_err_blk_det_i |= src_int->etrans.exco4m.tx_pcs_err_blk_det_i;
        dest_int->etrans.exco4m.tx_pcs_hber_i |= src_int->etrans.exco4m.tx_pcs_hber_i;
        dest_int->etrans.exco4m.rx_tsf_i |= src_int->etrans.exco4m.rx_tsf_i;
        dest_int->etrans.exco4m.rx_lf_os_det_i |= src_int->etrans.exco4m.rx_lf_os_det_i;
        dest_int->etrans.exco4m.rx_rf_os_det_i |= src_int->etrans.exco4m.rx_rf_os_det_i;
        dest_int->etrans.exco4m.rx_idle_test_err_det_i |= src_int->etrans.exco4m.rx_idle_test_err_det_i;
        dest_int->etrans.exco4m.rx_pcs_err_blk_det_i |= src_int->etrans.exco4m.rx_pcs_err_blk_det_i;
        dest_int->etrans.exco4m.rx_pcs_inv_blk_det_i |= src_int->etrans.exco4m.rx_pcs_inv_blk_det_i;
        dest_int->etrans.exco4m.rx_pcs_hber_i |= src_int->etrans.exco4m.rx_pcs_hber_i;
        dest_int->etrans.exco4m.rx_lane_algn_mrk_err_i |= src_int->etrans.exco4m.rx_lane_algn_mrk_err_i;

        for (index = 0; index < EXCO4M_N_LANES; index++)
        {
            dest_int->etrans.exco4m.tx_pcs_lobl_i[index] |= src_int->etrans.exco4m.tx_pcs_lobl_i[index];
            dest_int->etrans.exco4m.tx_pcs_ber_det_i[index] |= src_int->etrans.exco4m.tx_pcs_ber_det_i[index];
            dest_int->etrans.exco4m.tx_pcs_bip_err_det_i[index] |= src_int->etrans.exco4m.tx_pcs_bip_err_det_i[index];
            dest_int->etrans.exco4m.tx_loam_i[index] |= src_int->etrans.exco4m.tx_loam_i[index];
            dest_int->etrans.exco4m.tx_mrk_inv_det_i[index] |= src_int->etrans.exco4m.tx_mrk_inv_det_i[index];
            dest_int->etrans.exco4m.rx_lobl_i[index] |= src_int->etrans.exco4m.rx_lobl_i[index];
            dest_int->etrans.exco4m.rx_pcs_ber_det_i[index] |= src_int->etrans.exco4m.rx_pcs_ber_det_i[index];
            dest_int->etrans.exco4m.rx_pcs_bip_err_det_i[index] |= src_int->etrans.exco4m.rx_pcs_bip_err_det_i[index];
            dest_int->etrans.exco4m.rx_loam_i[index] |= src_int->etrans.exco4m.rx_loam_i[index];
            dest_int->etrans.exco4m.rx_pcs_mrk_inv_det_i[index] |= src_int->etrans.exco4m.rx_pcs_mrk_inv_det_i[index];
        }

        dest_int->etrans.exco4m.tx_los_v        = src_int->etrans.exco4m.tx_los_v;
        dest_int->etrans.exco4m.tx_pcs_hber_v   = src_int->etrans.exco4m.tx_pcs_hber_v;
        dest_int->etrans.exco4m.rx_tsf_v        = src_int->etrans.exco4m.rx_tsf_v;
        dest_int->etrans.exco4m.rx_pcs_hber_v   = src_int->etrans.exco4m.rx_pcs_hber_v;

        for (index = 0; index < EXCO4M_N_LANES; index++)
        {
            dest_int->etrans.exco4m.tx_pcs_lobl_v[index] = src_int->etrans.exco4m.tx_pcs_lobl_v[index];
            dest_int->etrans.exco4m.tx_loam_v[index] = src_int->etrans.exco4m.tx_loam_v[index];
            dest_int->etrans.exco4m.rx_lobl_v[index] = src_int->etrans.exco4m.rx_lobl_v[index];
            dest_int->etrans.exco4m.rx_loam_v[index] = src_int->etrans.exco4m.rx_loam_v[index];
        }
    }

    dest_int->etrans.gsup43_c73_valid = src_int->etrans.gsup43_c73_valid; 
    if (src_int->etrans.gsup43_c73_valid)
    {
        dest_int->etrans.gsup43_c73.los_i |= src_int->etrans.gsup43_c73.los_i; 
        dest_int->etrans.gsup43_c73.los_sync_i |= src_int->etrans.gsup43_c73.los_sync_i; 
        dest_int->etrans.gsup43_c73.hi_ber_i |= src_int->etrans.gsup43_c73.hi_ber_i; 
        dest_int->etrans.gsup43_c73.link_fail_i |= src_int->etrans.gsup43_c73.link_fail_i; 
        dest_int->etrans.gsup43_c73.rx_fault_i |= src_int->etrans.gsup43_c73.rx_fault_i; 
        dest_int->etrans.gsup43_c73.blk_type_err_i |= src_int->etrans.gsup43_c73.blk_type_err_i; 
        dest_int->etrans.gsup43_c73.rx_prbs31_err_i |= src_int->etrans.gsup43_c73.rx_prbs31_err_i; 
        dest_int->etrans.gsup43_c73.all_ones_or_zeros_i |= src_int->etrans.gsup43_c73.all_ones_or_zeros_i; 
        dest_int->etrans.gsup43_c73.rx_rf_i |= src_int->etrans.gsup43_c73.rx_rf_i; 
        dest_int->etrans.gsup43_c73.rx_lf_i |= src_int->etrans.gsup43_c73.rx_lf_i; 
        dest_int->etrans.gsup43_c73.fifo_undrun_i |= src_int->etrans.gsup43_c73.fifo_undrun_i; 
        dest_int->etrans.gsup43_c73.fifo_ovrun_i |= src_int->etrans.gsup43_c73.fifo_ovrun_i; 
        dest_int->etrans.gsup43_c73.send_rf_i |= src_int->etrans.gsup43_c73.send_rf_i; 
        dest_int->etrans.gsup43_c73.send_lf_i |= src_int->etrans.gsup43_c73.send_lf_i; 
        dest_int->etrans.gsup43_c73.send_idle_i |= src_int->etrans.gsup43_c73.send_idle_i; 
        dest_int->etrans.gsup43_c73.type_err_i |= src_int->etrans.gsup43_c73.type_err_i; 

        dest_int->etrans.gsup43_c73.los_v = src_int->etrans.gsup43_c73.los_v; 
        dest_int->etrans.gsup43_c73.los_sync_v = src_int->etrans.gsup43_c73.los_sync_v; 
        dest_int->etrans.gsup43_c73.hi_ber_v = src_int->etrans.gsup43_c73.hi_ber_v; 
        dest_int->etrans.gsup43_c73.link_fail_v = src_int->etrans.gsup43_c73.link_fail_v; 
        dest_int->etrans.gsup43_c73.rx_fault_v = src_int->etrans.gsup43_c73.rx_fault_v; 
        dest_int->etrans.gsup43_c73.all_ones_or_zeros_v = src_int->etrans.gsup43_c73.all_ones_or_zeros_v; 
        dest_int->etrans.gsup43_c73.rx_rf_v = src_int->etrans.gsup43_c73.rx_rf_v; 
        dest_int->etrans.gsup43_c73.rx_lf_v = src_int->etrans.gsup43_c73.rx_lf_v; 
        dest_int->etrans.gsup43_c73.lf_fault_v = src_int->etrans.gsup43_c73.lf_fault_v; 
        dest_int->etrans.gsup43_c73.rf_fault_v = src_int->etrans.gsup43_c73.rf_fault_v; 
        dest_int->etrans.gsup43_c73.send_rf_v = src_int->etrans.gsup43_c73.send_rf_v; 
        dest_int->etrans.gsup43_c73.send_lf_v = src_int->etrans.gsup43_c73.send_lf_v; 
        dest_int->etrans.gsup43_c73.send_idle_v = src_int->etrans.gsup43_c73.send_idle_v; 
    }

    dest_int->etrans.etrans_10g_valid = src_int->etrans.etrans_10g_valid; 
    if (src_int->etrans.etrans_10g_valid)
    {
        dest_int->etrans.rx_10g_fifo_ovf_i |= src_int->etrans.rx_10g_fifo_ovf_i;
        dest_int->etrans.rx_10g_fifo_udr_i |= src_int->etrans.rx_10g_fifo_udr_i;
        dest_int->etrans.tx_link_10g_fifo_ovf_i |= src_int->etrans.tx_link_10g_fifo_ovf_i;
        dest_int->etrans.tx_10g_fifo_udr_i |= src_int->etrans.tx_10g_fifo_udr_i;
        dest_int->etrans.rx_10g_fifo_ovf_v = src_int->etrans.rx_10g_fifo_ovf_v;
        dest_int->etrans.rx_10g_fifo_udr_v = src_int->etrans.rx_10g_fifo_udr_v;
        dest_int->etrans.tx_link_10g_fifo_ovf_v = src_int->etrans.tx_link_10g_fifo_ovf_v;
        dest_int->etrans.tx_10g_fifo_udr_v = src_int->etrans.tx_10g_fifo_udr_v;
    }

    dest_int->etrans.etrans_40g_valid = src_int->etrans.etrans_40g_valid; 
    if (src_int->etrans.etrans_40g_valid)
    {
        dest_int->etrans.rx_40g_fifo_ovf_i |= src_int->etrans.rx_40g_fifo_ovf_i;
        dest_int->etrans.rx_40g_fifo_udr_i |= src_int->etrans.rx_40g_fifo_udr_i;
        dest_int->etrans.tx_40g_fifo_ovf_i |= src_int->etrans.tx_40g_fifo_ovf_i;
        dest_int->etrans.tx_40g_fifo_udr_i |= src_int->etrans.tx_40g_fifo_udr_i;
        dest_int->etrans.rx_40g_fifo_ovf_v = src_int->etrans.rx_40g_fifo_ovf_v;
        dest_int->etrans.rx_40g_fifo_udr_v = src_int->etrans.rx_40g_fifo_udr_v;
        dest_int->etrans.tx_40g_fifo_ovf_v = src_int->etrans.tx_40g_fifo_ovf_v;
        dest_int->etrans.tx_40g_fifo_udr_v = src_int->etrans.tx_40g_fifo_udr_v;
    }

    dest_int->etrans.etrans_100g_valid = src_int->etrans.etrans_100g_valid; 
    if (src_int->etrans.etrans_100g_valid)
    {
        dest_int->etrans.rx_100g_fifo_ovf_i |= src_int->etrans.rx_100g_fifo_ovf_i;
        dest_int->etrans.rx_100g_fifo_udr_i |= src_int->etrans.rx_100g_fifo_udr_i;
        dest_int->etrans.tx_100g_fifo_ovf_i |= src_int->etrans.tx_100g_fifo_ovf_i;
        dest_int->etrans.tx_100g_fifo_udr_i |= src_int->etrans.tx_100g_fifo_udr_i;
        dest_int->etrans.rx_100g_fifo_ovf_v = src_int->etrans.rx_100g_fifo_ovf_v;
        dest_int->etrans.rx_100g_fifo_udr_v = src_int->etrans.rx_100g_fifo_udr_v;
        dest_int->etrans.tx_100g_fifo_ovf_v = src_int->etrans.tx_100g_fifo_ovf_v;
        dest_int->etrans.tx_100g_fifo_udr_v = src_int->etrans.tx_100g_fifo_udr_v;
    }

    /* EPMM */
    dest_int->epmm.exco3m_valid = src_int->epmm.exco3m_valid; 
    if (src_int->epmm.exco3m_valid)
    {
        dest_int->epmm.exco3m.tx_los_i |= src_int->epmm.exco3m.tx_los_i;
        dest_int->epmm.exco3m.tx_lf_os_det_i |= src_int->epmm.exco3m.tx_lf_os_det_i;
        dest_int->epmm.exco3m.tx_rf_os_det_i |= src_int->epmm.exco3m.tx_rf_os_det_i;
        dest_int->epmm.exco3m.tx_idle_test_err_det_i |= src_int->epmm.exco3m.tx_idle_test_err_det_i;
        dest_int->epmm.exco3m.tx_skew_range_viol_i |= src_int->epmm.exco3m.tx_skew_range_viol_i;
        dest_int->epmm.exco3m.tx_pcs_inv_blk_det_i |= src_int->epmm.exco3m.tx_pcs_inv_blk_det_i;
        dest_int->epmm.exco3m.tx_pcs_err_blk_det_i |= src_int->epmm.exco3m.tx_pcs_err_blk_det_i;
        dest_int->epmm.exco3m.tx_pcs_hber_i |= src_int->epmm.exco3m.tx_pcs_hber_i;
        dest_int->epmm.exco3m.rx_tsf_i |= src_int->epmm.exco3m.rx_tsf_i;
        dest_int->epmm.exco3m.rx_lf_os_det_i |= src_int->epmm.exco3m.rx_lf_os_det_i;
        dest_int->epmm.exco3m.rx_rf_os_det_i |= src_int->epmm.exco3m.rx_rf_os_det_i;
        dest_int->epmm.exco3m.rx_idle_test_err_det_i |= src_int->epmm.exco3m.rx_idle_test_err_det_i;
        dest_int->epmm.exco3m.rx_pcs_err_blk_det_i |= src_int->epmm.exco3m.rx_pcs_err_blk_det_i;
        dest_int->epmm.exco3m.rx_1027b_lobl_i |= src_int->epmm.exco3m.rx_1027b_lobl_i;
        dest_int->epmm.exco3m.rx_1027b_hber_i |= src_int->epmm.exco3m.rx_1027b_hber_i;
        dest_int->epmm.exco3m.rx_1027b_ber_i |= src_int->epmm.exco3m.rx_1027b_ber_i;
        dest_int->epmm.exco3m.rx_skew_range_viol_i |= src_int->epmm.exco3m.rx_skew_range_viol_i;
        dest_int->epmm.exco3m.rx_513b_pos_err_det_i |= src_int->epmm.exco3m.rx_513b_pos_err_det_i;
        dest_int->epmm.exco3m.rx_well_pkt_err_i |= src_int->epmm.exco3m.rx_well_pkt_err_i;
        dest_int->epmm.exco3m.rx_lane_algn_mrk_err_i |= src_int->epmm.exco3m.rx_lane_algn_mrk_err_i;
        dest_int->epmm.exco3m.rx_1027b_par_err_i |= src_int->epmm.exco3m.rx_1027b_par_err_i;

        for (index = 0; index < EXCO3M_N_LANES; index++)
        {
            dest_int->epmm.exco3m.tx_pcs_lobl_i[index] |= src_int->epmm.exco3m.tx_pcs_lobl_i[index];
            dest_int->epmm.exco3m.tx_pcs_ber_det_i[index] |= src_int->epmm.exco3m.tx_pcs_ber_det_i[index];
            dest_int->epmm.exco3m.tx_pcs_bip_err_det_i[index] |= src_int->epmm.exco3m.tx_pcs_bip_err_det_i[index];
            dest_int->epmm.exco3m.tx_loam_i[index] |= src_int->epmm.exco3m.tx_loam_i[index];
            dest_int->epmm.exco3m.tx_mrk_inv_det_i[index] |= src_int->epmm.exco3m.tx_mrk_inv_det_i[index];
            dest_int->epmm.exco3m.rx_otn_bip_err_det_i[index] |= src_int->epmm.exco3m.rx_otn_bip_err_det_i[index];
            dest_int->epmm.exco3m.rx_loam_i[index] |= src_int->epmm.exco3m.rx_loam_i[index];
            dest_int->epmm.exco3m.rx_pcs_mrk_inv_det_i[index] |= src_int->epmm.exco3m.rx_pcs_mrk_inv_det_i[index];
        }

        dest_int->epmm.exco3m.tx_los_v        = src_int->epmm.exco3m.tx_los_v;
        dest_int->epmm.exco3m.tx_pcs_hber_v   = src_int->epmm.exco3m.tx_pcs_hber_v;
        dest_int->epmm.exco3m.rx_tsf_v        = src_int->epmm.exco3m.rx_tsf_v;
        dest_int->epmm.exco3m.rx_1027b_lobl_v = src_int->epmm.exco3m.rx_1027b_lobl_v;
        dest_int->epmm.exco3m.rx_1027b_hber_v = src_int->epmm.exco3m.rx_1027b_hber_v;

        for (index = 0; index < EXCO3M_N_LANES; index++)
        {
            dest_int->epmm.exco3m.tx_pcs_lobl_v[index] = src_int->epmm.exco3m.tx_pcs_lobl_v[index];
            dest_int->epmm.exco3m.tx_loam_v[index] = src_int->epmm.exco3m.tx_loam_v[index];
            dest_int->epmm.exco3m.rx_loam_v[index] = src_int->epmm.exco3m.rx_loam_v[index];
        }
    }

    dest_int->epmm.exco4m_valid = src_int->epmm.exco4m_valid; 
    if (src_int->epmm.exco4m_valid)
    {
        dest_int->epmm.exco4m.tx_los_i |= src_int->epmm.exco4m.tx_los_i;
        dest_int->epmm.exco4m.tx_lf_os_det_i |= src_int->epmm.exco4m.tx_lf_os_det_i;
        dest_int->epmm.exco4m.tx_rf_os_det_i |= src_int->epmm.exco4m.tx_rf_os_det_i;
        dest_int->epmm.exco4m.tx_idle_test_err_det_i |= src_int->epmm.exco4m.tx_idle_test_err_det_i;
        dest_int->epmm.exco4m.tx_skew_range_viol_i |= src_int->epmm.exco4m.tx_skew_range_viol_i;
        dest_int->epmm.exco4m.tx_pcs_inv_blk_det_i |= src_int->epmm.exco4m.tx_pcs_inv_blk_det_i;
        dest_int->epmm.exco4m.tx_pcs_err_blk_det_i |= src_int->epmm.exco4m.tx_pcs_err_blk_det_i;
        dest_int->epmm.exco4m.tx_pcs_hber_i |= src_int->epmm.exco4m.tx_pcs_hber_i;
        dest_int->epmm.exco4m.rx_tsf_i |= src_int->epmm.exco4m.rx_tsf_i;
        dest_int->epmm.exco4m.rx_lf_os_det_i |= src_int->epmm.exco4m.rx_lf_os_det_i;
        dest_int->epmm.exco4m.rx_rf_os_det_i |= src_int->epmm.exco4m.rx_rf_os_det_i;
        dest_int->epmm.exco4m.rx_idle_test_err_det_i |= src_int->epmm.exco4m.rx_idle_test_err_det_i;
        dest_int->epmm.exco4m.rx_pcs_err_blk_det_i |= src_int->epmm.exco4m.rx_pcs_err_blk_det_i;
        dest_int->epmm.exco4m.rx_pcs_inv_blk_det_i |= src_int->epmm.exco4m.rx_pcs_inv_blk_det_i;
        dest_int->epmm.exco4m.rx_pcs_hber_i |= src_int->epmm.exco4m.rx_pcs_hber_i;
        dest_int->epmm.exco4m.rx_lane_algn_mrk_err_i |= src_int->epmm.exco4m.rx_lane_algn_mrk_err_i;

        for (index = 0; index < EXCO4M_N_LANES; index++)
        {
            dest_int->epmm.exco4m.tx_pcs_lobl_i[index] |= src_int->epmm.exco4m.tx_pcs_lobl_i[index];
            dest_int->epmm.exco4m.tx_pcs_ber_det_i[index] |= src_int->epmm.exco4m.tx_pcs_ber_det_i[index];
            dest_int->epmm.exco4m.tx_pcs_bip_err_det_i[index] |= src_int->epmm.exco4m.tx_pcs_bip_err_det_i[index];
            dest_int->epmm.exco4m.tx_loam_i[index] |= src_int->epmm.exco4m.tx_loam_i[index];
            dest_int->epmm.exco4m.tx_mrk_inv_det_i[index] |= src_int->epmm.exco4m.tx_mrk_inv_det_i[index];
            dest_int->epmm.exco4m.rx_lobl_i[index] |= src_int->epmm.exco4m.rx_lobl_i[index];
            dest_int->epmm.exco4m.rx_pcs_ber_det_i[index] |= src_int->epmm.exco4m.rx_pcs_ber_det_i[index];
            dest_int->epmm.exco4m.rx_pcs_bip_err_det_i[index] |= src_int->epmm.exco4m.rx_pcs_bip_err_det_i[index];
            dest_int->epmm.exco4m.rx_loam_i[index] |= src_int->epmm.exco4m.rx_loam_i[index];
            dest_int->epmm.exco4m.rx_pcs_mrk_inv_det_i[index] |= src_int->epmm.exco4m.rx_pcs_mrk_inv_det_i[index];
        }

        dest_int->epmm.exco4m.tx_los_v        = src_int->epmm.exco4m.tx_los_v;
        dest_int->epmm.exco4m.tx_pcs_hber_v   = src_int->epmm.exco4m.tx_pcs_hber_v;
        dest_int->epmm.exco4m.rx_tsf_v        = src_int->epmm.exco4m.rx_tsf_v;
        dest_int->epmm.exco4m.rx_pcs_hber_v   = src_int->epmm.exco4m.rx_pcs_hber_v;

        for (index = 0; index < EXCO4M_N_LANES; index++)
        {
            dest_int->epmm.exco4m.tx_pcs_lobl_v[index] = src_int->epmm.exco4m.tx_pcs_lobl_v[index];
            dest_int->epmm.exco4m.tx_loam_v[index] = src_int->epmm.exco4m.tx_loam_v[index];
            dest_int->epmm.exco4m.rx_lobl_v[index] = src_int->epmm.exco4m.rx_lobl_v[index];
            dest_int->epmm.exco4m.rx_loam_v[index] = src_int->epmm.exco4m.rx_loam_v[index];
        }
    }

    dest_int->epmm.gsup43_c73_valid = src_int->epmm.gsup43_c73_valid; 
    if (src_int->epmm.gsup43_c73_valid)
    {
        dest_int->epmm.gsup43_c73.los_i |= src_int->epmm.gsup43_c73.los_i; 
        dest_int->epmm.gsup43_c73.los_sync_i |= src_int->epmm.gsup43_c73.los_sync_i; 
        dest_int->epmm.gsup43_c73.hi_ber_i |= src_int->epmm.gsup43_c73.hi_ber_i; 
        dest_int->epmm.gsup43_c73.link_fail_i |= src_int->epmm.gsup43_c73.link_fail_i; 
        dest_int->epmm.gsup43_c73.rx_fault_i |= src_int->epmm.gsup43_c73.rx_fault_i; 
        dest_int->epmm.gsup43_c73.blk_type_err_i |= src_int->epmm.gsup43_c73.blk_type_err_i; 
        dest_int->epmm.gsup43_c73.rx_prbs31_err_i |= src_int->epmm.gsup43_c73.rx_prbs31_err_i; 
        dest_int->epmm.gsup43_c73.all_ones_or_zeros_i |= src_int->epmm.gsup43_c73.all_ones_or_zeros_i; 
        dest_int->epmm.gsup43_c73.rx_rf_i |= src_int->epmm.gsup43_c73.rx_rf_i; 
        dest_int->epmm.gsup43_c73.rx_lf_i |= src_int->epmm.gsup43_c73.rx_lf_i; 
        dest_int->epmm.gsup43_c73.fifo_undrun_i |= src_int->epmm.gsup43_c73.fifo_undrun_i; 
        dest_int->epmm.gsup43_c73.fifo_ovrun_i |= src_int->epmm.gsup43_c73.fifo_ovrun_i; 
        dest_int->epmm.gsup43_c73.send_rf_i |= src_int->epmm.gsup43_c73.send_rf_i; 
        dest_int->epmm.gsup43_c73.send_lf_i |= src_int->epmm.gsup43_c73.send_lf_i; 
        dest_int->epmm.gsup43_c73.send_idle_i |= src_int->epmm.gsup43_c73.send_idle_i; 
        dest_int->epmm.gsup43_c73.type_err_i |= src_int->epmm.gsup43_c73.type_err_i; 

        dest_int->epmm.gsup43_c73.los_v = src_int->epmm.gsup43_c73.los_v; 
        dest_int->epmm.gsup43_c73.los_sync_v = src_int->epmm.gsup43_c73.los_sync_v; 
        dest_int->epmm.gsup43_c73.hi_ber_v = src_int->epmm.gsup43_c73.hi_ber_v; 
        dest_int->epmm.gsup43_c73.link_fail_v = src_int->epmm.gsup43_c73.link_fail_v; 
        dest_int->epmm.gsup43_c73.rx_fault_v = src_int->epmm.gsup43_c73.rx_fault_v; 
        dest_int->epmm.gsup43_c73.all_ones_or_zeros_v = src_int->epmm.gsup43_c73.all_ones_or_zeros_v; 
        dest_int->epmm.gsup43_c73.rx_rf_v = src_int->epmm.gsup43_c73.rx_rf_v; 
        dest_int->epmm.gsup43_c73.rx_lf_v = src_int->epmm.gsup43_c73.rx_lf_v; 
        dest_int->epmm.gsup43_c73.lf_fault_v = src_int->epmm.gsup43_c73.lf_fault_v; 
        dest_int->epmm.gsup43_c73.rf_fault_v = src_int->epmm.gsup43_c73.rf_fault_v; 
        dest_int->epmm.gsup43_c73.send_rf_v = src_int->epmm.gsup43_c73.send_rf_v; 
        dest_int->epmm.gsup43_c73.send_lf_v = src_int->epmm.gsup43_c73.send_lf_v; 
        dest_int->epmm.gsup43_c73.send_idle_v = src_int->epmm.gsup43_c73.send_idle_v; 
    }

    /* MPMA */
    dest_int->epmm.mpma.cfc.fifo_ovr_i |= src_int->epmm.mpma.cfc.fifo_ovr_i;
    dest_int->epmm.mpma.cfc.fifo_udr_i |= src_int->epmm.mpma.cfc.fifo_udr_i;
    dest_int->epmm.mpma.cfc.ipt_err_i  |= src_int->epmm.mpma.cfc.ipt_err_i;
    dest_int->epmm.mpma.psd_fifo_ovr_i |= src_int->epmm.mpma.psd_fifo_ovr_i;
    dest_int->epmm.mpma.psd_fifo_udr_i |= src_int->epmm.mpma.psd_fifo_udr_i;

    /* MPMO */
    dest_int->epmm.mpmo.cfc.fifo_ovr_i |= src_int->epmm.mpmo.cfc.fifo_ovr_i;
    dest_int->epmm.mpmo.cfc.fifo_udr_i |= src_int->epmm.mpmo.cfc.fifo_udr_i;
    dest_int->epmm.mpmo.cfc.ipt_err_i |= src_int->epmm.mpmo.cfc.ipt_err_i;
    dest_int->epmm.mpmo.pkt_drop_replace_i |= src_int->epmm.mpmo.pkt_drop_replace_i;
    dest_int->epmm.mpmo.pkt_drop_no_replace_i |= src_int->epmm.mpmo.pkt_drop_no_replace_i;
   
    PMC_RETURN(rc);
} /* adpt_enet_chnl_int_merge */


/*******************************************************************************
* adpt_enet_int_merge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Merges ENET defect and interrupt data from shared memory to Adapter. 
*
* INPUTS:
*   *src_int           - shared memory ENET defect and int data.
*   *dest_int          - Adapter ENET defect and int data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_int_merge(digi_enet_int_t *src_int,
                                  digi_enet_int_t *dest_int)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(src_int);
    ADPT_PARA_VERIFY(dest_int);

    for (index = 0; index < DIGI_ENET_CHNL_MAX; index++)
    {
        dest_int->handle[index] = src_int->handle[index];
        if (NULL == src_int->handle[index])
        {
            continue;
        }

        rc = adpt_enet_chnl_int_merge(&src_int->enet_chnl[index].enet,
                                      &dest_int->enet_chnl[index].enet);

        /* CPB */
        dest_int->enet_chnl[index].cpb.ch_drop_ovrsize_i |=
        src_int->enet_chnl[index].cpb.ch_drop_ovrsize_i;
        dest_int->enet_chnl[index].cpb.ch_drop_client_i |=
        src_int->enet_chnl[index].cpb.ch_drop_client_i;
        dest_int->enet_chnl[index].cpb.ch_drop_full_i |=
        src_int->enet_chnl[index].cpb.ch_drop_full_i;
    }

    PMC_RETURN(rc);
} /* adpt_enet_int_merge */


/*******************************************************************************
* adpt_fege_int_merge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Merges FEGE defect and interrupt data from shared memory to Adapter. 
*
* INPUTS:
*   *src_int           - shared memory FEGE defect and int data.
*   *dest_int          - Adapter FEGE defect and int data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fege_int_merge(enet_fege_int_t *src_int, 
                                  enet_fege_int_t *dest_int)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(src_int);
    ADPT_PARA_VERIFY(dest_int);

    /* interrupts */
    dest_int->tx_tstmpr_tod_sign_ovfl_i         |= src_int->tx_tstmpr_tod_sign_ovfl_i;
    dest_int->rx_tstmpr_tod_sign_ovfl_i         |= src_int->rx_tstmpr_tod_sign_ovfl_i;
    dest_int->overflow_i                        |= src_int->overflow_i;          
    dest_int->rx_comma_del_chg_i                |= src_int->rx_comma_del_chg_i;    
    dest_int->tx_tstmp_fifo_i                   |= src_int->tx_tstmp_fifo_i;
    dest_int->tx_sof_i                          |= src_int->tx_sof_i;              
    dest_int->rx_sof_i                          |= src_int->rx_sof_i;  
    dest_int->rx_long_length_error_i            |= src_int->rx_long_length_error_i;
    dest_int->rx_inrange_length_error_i         |= src_int->rx_inrange_length_error_i;
    dest_int->rx_layer1_error_i                 |= src_int->rx_layer1_error_i;        
    dest_int->rx_symbol_error_i                 |= src_int->rx_symbol_error_i;        
    dest_int->rx_fcs_error_i                    |= src_int->rx_fcs_error_i;           
    dest_int->rx_mac_control_frames_ok_i        |= src_int->rx_mac_control_frames_ok_i;
    dest_int->rx_mac_pause_control_frames_ok_i  |= src_int->rx_mac_pause_control_frames_ok_i;
    dest_int->rx_tagged_frames_ok_i             |= src_int->rx_tagged_frames_ok_i;           
    dest_int->rx_multicast_frames_ok_i          |= src_int->rx_multicast_frames_ok_i;        
    dest_int->rx_broadcast_frames_ok_i          |= src_int->rx_broadcast_frames_ok_i;        
    dest_int->rx_unicast_frames_ok_i            |= src_int->rx_unicast_frames_ok_i;          
    dest_int->rx_octets_i                       |= src_int->rx_octets_i;                     
    dest_int->rx_frames_i                       |= src_int->rx_frames_i;                     
    dest_int->rx_octets_ok_i                    |= src_int->rx_octets_ok_i;                  
    dest_int->rx_frames_ok_i                    |= src_int->rx_frames_ok_i;                  
    dest_int->rx_variable_i                     |= src_int->rx_variable_i;                   
    dest_int->rx_station_address_filtered_i     |= src_int->rx_station_address_filtered_i;   
    dest_int->rx_frames_1519_to_max_size_i      |= src_int->rx_frames_1519_to_max_size_i;    
    dest_int->rx_frames_1024_to_1518_octets_i   |= src_int->rx_frames_1024_to_1518_octets_i; 
    dest_int->rx_frames_512_to_1023_octets_i    |= src_int->rx_frames_512_to_1023_octets_i;  
    dest_int->rx_frames_256_to_511_octets_i     |= src_int->rx_frames_256_to_511_octets_i;   
    dest_int->rx_frames_128_to_255_octets_i     |= src_int->rx_frames_128_to_255_octets_i;   
    dest_int->rx_frames_65_to_127_octets_i      |= src_int->rx_frames_65_to_127_octets_i;    
    dest_int->rx_frames_64_octets_i             |= src_int->rx_frames_64_octets_i;           
    dest_int->rx_short_length_crc_error_i       |= src_int->rx_short_length_crc_error_i;     
    dest_int->rx_short_length_error_i           |= src_int->rx_short_length_error_i;         
    dest_int->rx_long_length_crc_error_i        |= src_int->rx_long_length_crc_error_i;      
    dest_int->rx_frames_2501_to_max_size_i      |= src_int->rx_frames_2501_to_max_size_i;    
    dest_int->rx_frames_2001_to_2500_octets_i   |= src_int->rx_frames_2001_to_2500_octets_i; 
    dest_int->rx_frames_1549_to_2000_octets_i   |= src_int->rx_frames_1549_to_2000_octets_i; 
    dest_int->rx_frames_1523_to_1548_octets_i   |= src_int->rx_frames_1523_to_1548_octets_i; 
    dest_int->rx_frames_1519_to_1522_octets_i   |= src_int->rx_frames_1519_to_1522_octets_i; 
    dest_int->tx_unicast_frames_attempted_i     |= src_int->tx_unicast_frames_attempted_i;   
    dest_int->tx_multicast_frames_ok_i          |= src_int->tx_multicast_frames_ok_i;        
    dest_int->tx_broadcast_frames_ok_i          |= src_int->tx_broadcast_frames_ok_i;        
    dest_int->tx_unicast_frames_ok_i            |= src_int->tx_unicast_frames_ok_i;          
    dest_int->tx_mac_control_frames_ok_i        |= src_int->tx_mac_control_frames_ok_i;      
    dest_int->tx_frames_1523_to_max_size_i      |= src_int->tx_frames_1523_to_max_size_i;    
    dest_int->tx_mac_error_i                    |= src_int->tx_mac_error_i;                  
    dest_int->tx_system_error_i                 |= src_int->tx_system_error_i;               
    dest_int->tx_long_length_error_i            |= src_int->tx_long_length_error_i;          
    dest_int->tx_short_length_error_i           |= src_int->tx_short_length_error_i;         
    dest_int->tx_fcs_error_i                    |= src_int->tx_fcs_error_i;                  
    dest_int->tx_mac_pause_control_frames_ok_i  |= src_int->tx_mac_pause_control_frames_ok_i;
    dest_int->tx_tagged_frames_ok_i             |= src_int->tx_tagged_frames_ok_i;           
    dest_int->tx_octets_i                       |= src_int->tx_octets_i;                     
    dest_int->tx_octets_ok_i                    |= src_int->tx_octets_ok_i;                  
    dest_int->tx_frames_ok_i                    |= src_int->tx_frames_ok_i;                  
    dest_int->tx_frames_2501_to_max_size_i      |= src_int->tx_frames_2501_to_max_size_i;    
    dest_int->tx_frames_2001_to_2500_octets_i   |= src_int->tx_frames_2001_to_2500_octets_i; 
    dest_int->tx_frames_1549_to_2000_octets_i   |= src_int->tx_frames_1549_to_2000_octets_i; 
    dest_int->tx_frames_1523_to_1548_octets_i   |= src_int->tx_frames_1523_to_1548_octets_i; 
    dest_int->tx_frames_1519_to_1522_octets_i   |= src_int->tx_frames_1519_to_1522_octets_i; 
    dest_int->rx_frames_1523_to_max_size_i      |= src_int->rx_frames_1523_to_max_size_i;    
    dest_int->tx_variable_i                     |= src_int->tx_variable_i;                   
    dest_int->tx_frames_1519_to_max_size_i      |= src_int->tx_frames_1519_to_max_size_i;    
    dest_int->tx_frames_1024_to_1518_octets_i   |= src_int->tx_frames_1024_to_1518_octets_i; 
    dest_int->tx_frames_512_to_1023_octets_i    |= src_int->tx_frames_512_to_1023_octets_i;  
    dest_int->tx_frames_256_to_511_octets_i     |= src_int->tx_frames_256_to_511_octets_i;   
    dest_int->tx_frames_128_to_255_octets_i     |= src_int->tx_frames_128_to_255_octets_i;   
    dest_int->tx_frames_65_to_127_octets_i      |= src_int->tx_frames_65_to_127_octets_i;    
    dest_int->tx_frames_64_octets_i             |= src_int->tx_frames_64_octets_i;           
    dest_int->tx_multicast_frames_attempted_i   |= src_int->tx_multicast_frames_attempted_i; 
    dest_int->tx_broadcast_frames_attempted_i   |= src_int->tx_broadcast_frames_attempted_i;   
    dest_int->los_i                             |= src_int->los_i;                           
    dest_int->sdet_in_i                         |= src_int->sdet_in_i;                       
    dest_int->align_change_i                    |= src_int->align_change_i;
    dest_int->link_mon_i                        |= src_int->link_mon_i;
    dest_int->fec_stat_i                        |= src_int->fec_stat_i;
    dest_int->carrier_stat_i                    |= src_int->carrier_stat_i;
    dest_int->rx_err_i                          |= src_int->rx_err_i;                        
    dest_int->lcv_i                             |= src_int->lcv_i;                           
    dest_int->auto_neg_lp_ablty_upd_i           |= src_int->auto_neg_lp_ablty_upd_i;         
    dest_int->link_ok_i                         |= src_int->link_ok_i;                      
    dest_int->sync_ok_i                         |= src_int->sync_ok_i;

    /* status */
    dest_int->tx_tstmpr_tod_sign_ovfl_v     = src_int->tx_tstmpr_tod_sign_ovfl_v;
    dest_int->rx_tstmpr_tod_sign_ovfl_v     = src_int->rx_tstmpr_tod_sign_ovfl_v;
    dest_int->los_v                         = src_int->los_v;                           
    dest_int->sdet_in_v                     = src_int->sdet_in_v;                       
    dest_int->align_change_v                = src_int->align_change_v;
    dest_int->link_mon_v                    = src_int->link_mon_v;
    dest_int->fec_stat_v                    = src_int->fec_stat_v;
    dest_int->carrier_stat_v                = src_int->carrier_stat_v;                  
    dest_int->auto_neg_lp_ablty_upd_v       = src_int->auto_neg_lp_ablty_upd_v;         
    dest_int->link_ok_v                     = src_int->link_ok_v;
    dest_int->sync_ok_v                     = src_int->sync_ok_v;

    PMC_RETURN(rc);
} /* adpt_fege_int_merge */


/*******************************************************************************
* adpt_ohp_int_merge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Merges OHP defect and interrupt data from shared memory to Adapter. 
*
* INPUTS:
*   *src_int           - shared memory OHP defect and int data.
*   *dest_int          - Adapter OHP defect and int data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_ohp_int_merge(odu_ohp_int_t *src_int, 
                                  odu_ohp_int_t *dest_int)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(src_int);
    ADPT_PARA_VERIFY(dest_int);

    /* FEGE interrupt */
    rc = adpt_fege_int_merge(&src_int->enet_fege, &dest_int->enet_fege);

    dest_int->rxmac_uid_fail_i    |= src_int->rxmac_uid_fail_i;
    dest_int->rxmac_frmrid_fail_i |= src_int->rxmac_frmrid_fail_i;
    dest_int->rxmac_chid_fail_i   |= src_int->rxmac_chid_fail_i;
    dest_int->rxmac_pkterr_i      |= src_int->rxmac_pkterr_i;
    dest_int->rxmac_fifo_err_i    |= src_int->rxmac_fifo_err_i;

    PMC_RETURN(rc);
} /* adpt_ohp_int_merge */


/*******************************************************************************
* adpt_rcp_int_merge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Merges DI_RCP/RI_RCP defect and interrupt data from shared memory to Adapter. 
*
* INPUTS:
*   *src_int           - shared memory RCP defect and int data.
*   *dest_int          - Adapter RCP defect and int data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_rcp_int_merge(odu_rcp_int_t *src_int, 
                                 odu_rcp_int_t *dest_int)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(src_int);
    ADPT_PARA_VERIFY(dest_int);

    /* FEGE interrupt */
    rc = adpt_fege_int_merge(&src_int->enet_fege, &dest_int->enet_fege);

    dest_int->pmon_tx_done_i |= src_int->pmon_tx_done_i;
    dest_int->dfec_tx_done_i |= src_int->dfec_tx_done_i;

    PMC_RETURN(rc);
} /* adpt_rcp_int_merge */


/*******************************************************************************
* adpt_otu_int_merge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Merges OTU defect and interrupt data from shared memory to Adapter. 
*
* INPUTS:
*   *src_int           - shared memory ODU defect and int data.
*   *dest_int          - Adapter ODU defect and int data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otu_int_merge(lineotn_int_chnl_t *src_int,
                                 lineotn_int_chnl_t *dest_int)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 i = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(src_int);
    ADPT_PARA_VERIFY(dest_int);

    /* OTU interrupt */
    dest_int->otu_frm.otu_frm_d_lom_i |= src_int->otu_frm.otu_frm_d_lom_i;
    dest_int->otu_frm.otu_frm_oom_i   |= src_int->otu_frm.otu_frm_oom_i;
    dest_int->otu_frm.otu_sk_a_ais_i  |= src_int->otu_frm.otu_sk_a_ais_i; 
    dest_int->otu_frm.otu_sk_a_ssd_i  |= src_int->otu_frm.otu_sk_a_ssd_i;
    dest_int->otu_frm.otu_sk_a_ssf_i  |= src_int->otu_frm.otu_sk_a_ssf_i; 
    dest_int->otu_frm.otu_sk_a_tsf_i  |= src_int->otu_frm.otu_sk_a_tsf_i;
    dest_int->otu_frm.otu_sk_a_tsd_i  |= src_int->otu_frm.otu_sk_a_tsd_i;
    dest_int->otu_frm.otu_sk_a_biae_i |= src_int->otu_frm.otu_sk_a_biae_i;
    dest_int->otu_frm.otu_sk_a_bei_i  |= src_int->otu_frm.otu_sk_a_bei_i; 
    dest_int->otu_frm.otu_sk_a_bdi_i  |= src_int->otu_frm.otu_sk_a_bdi_i; 
    dest_int->otu_frm.otu_sk_d_iae_i  |= src_int->otu_frm.otu_sk_d_iae_i; 
    dest_int->otu_frm.otu_sk_d_tim_i  |= src_int->otu_frm.otu_sk_d_tim_i; 
    dest_int->otu_frm.otu_sk_d_deg_i  |= src_int->otu_frm.otu_sk_d_deg_i; 
    dest_int->otu_frm.otu_sk_d_deg_enh_i    |= src_int->otu_frm.otu_sk_d_deg_enh_i; 
    dest_int->otu_frm.otu_sk_d_deg_bursty_i |= src_int->otu_frm.otu_sk_d_deg_bursty_i; 
    dest_int->otu_frm.otu_sk_d_bdi_i  |= src_int->otu_frm.otu_sk_d_bdi_i; 
    dest_int->otu_frm.otu_sk_d_biae_i |= src_int->otu_frm.otu_sk_d_biae_i;
    dest_int->otu_frm.otu_sk_ci_ssf_i |= src_int->otu_frm.otu_sk_ci_ssf_i;
    dest_int->otu_frm.otu_so_a_iae_i  |= src_int->otu_frm.otu_so_a_iae_i; 

    dest_int->otu_frm.otu_frm_d_lom_v = src_int->otu_frm.otu_frm_d_lom_v;
    dest_int->otu_frm.otu_frm_oom_v   = src_int->otu_frm.otu_frm_oom_v;  
    dest_int->otu_frm.otu_sk_a_ais_v  = src_int->otu_frm.otu_sk_a_ais_v; 
    dest_int->otu_frm.otu_sk_a_ssd_v  = src_int->otu_frm.otu_sk_a_ssd_v; 
    dest_int->otu_frm.otu_sk_a_ssf_v  = src_int->otu_frm.otu_sk_a_ssf_v; 
    dest_int->otu_frm.otu_sk_a_tsf_v  = src_int->otu_frm.otu_sk_a_tsf_v; 
    dest_int->otu_frm.otu_sk_a_tsd_v  = src_int->otu_frm.otu_sk_a_tsd_v; 
    dest_int->otu_frm.otu_sk_a_biae_v = src_int->otu_frm.otu_sk_a_biae_v;
    dest_int->otu_frm.otu_sk_a_bei_v  = src_int->otu_frm.otu_sk_a_bei_v; 
    dest_int->otu_frm.otu_sk_a_bdi_v  = src_int->otu_frm.otu_sk_a_bdi_v; 
    dest_int->otu_frm.otu_sk_d_iae_v  = src_int->otu_frm.otu_sk_d_iae_v; 
    dest_int->otu_frm.otu_sk_d_tim_v  = src_int->otu_frm.otu_sk_d_tim_v; 
    dest_int->otu_frm.otu_sk_d_deg_v  = src_int->otu_frm.otu_sk_d_deg_v; 
    dest_int->otu_frm.otu_sk_d_deg_enh_v    = src_int->otu_frm.otu_sk_d_deg_enh_v; 
    dest_int->otu_frm.otu_sk_d_deg_bursty_v = src_int->otu_frm.otu_sk_d_deg_bursty_v; 
    dest_int->otu_frm.otu_sk_d_bdi_v  = src_int->otu_frm.otu_sk_d_bdi_v; 
    dest_int->otu_frm.otu_sk_d_biae_v = src_int->otu_frm.otu_sk_d_biae_v;
    dest_int->otu_frm.otu_sk_ci_ssf_v = src_int->otu_frm.otu_sk_ci_ssf_v;

    /* ODUk SC */
    dest_int->otu_frm.oduksc.src_zone_i |= src_int->otu_frm.oduksc.src_zone_i;
    dest_int->otu_frm.oduksc.ramp_done_i |= src_int->otu_frm.oduksc.ramp_done_i;
    dest_int->otu_frm.oduksc.src_zone_v = src_int->otu_frm.oduksc.src_zone_v;

    /* OPSM */
    if (src_int->opsm23_valid)
    {
        dest_int->opsm23_valid = src_int->opsm23_valid;
        dest_int->opsm23_rx_sfi_los_int_i |= src_int->opsm23_rx_sfi_los_int_i;
        dest_int->opsm23_rx_sfi_oof_int_i |= src_int->opsm23_rx_sfi_oof_int_i;
        dest_int->opsm23_rx_sfi_lof_int_i |= src_int->opsm23_rx_sfi_lof_int_i;
        dest_int->opsm23_rx_otu_oof_int_i |= src_int->opsm23_rx_otu_oof_int_i;
        dest_int->opsm23_rx_otu_lof_int_i |= src_int->opsm23_rx_otu_lof_int_i;
        dest_int->opsm23_rx_sfi_ais_int_i |= src_int->opsm23_rx_sfi_ais_int_i;
        dest_int->opsm23_rx_lol_int_i     |= src_int->opsm23_rx_lol_int_i;
        dest_int->opsm23_rx_skew_range_violation_int_i |= src_int->opsm23_rx_skew_range_violation_int_i;
        dest_int->opsm23_rx_align_change_int_i  |= src_int->opsm23_rx_align_change_int_i;
        dest_int->opsm23_rx_deskew_change_int_i |= src_int->opsm23_rx_deskew_change_int_i;
        dest_int->opsm23_rx_sfi_fifo_overflow_int_i |= src_int->opsm23_rx_sfi_fifo_overflow_int_i;
        dest_int->opsm23_rx_sfi_fifo_overflow_int_i |= src_int->opsm23_rx_sfi_fifo_overflow_int_i;
        for (i = 0; i < 4; i++)
        {
            dest_int->opsm23_rx_lane_los_int_i[i] |= src_int->opsm23_rx_lane_los_int_i[i];
            dest_int->opsm23_rx_lane_oof_int_i[i] |= src_int->opsm23_rx_lane_oof_int_i[i];
            dest_int->opsm23_rx_lane_lof_loflane_int_i[i] |= src_int->opsm23_rx_lane_lof_loflane_int_i[i];
            dest_int->opsm23_rx_lane_otuk_ais_int_i[i] |= src_int->opsm23_rx_lane_otuk_ais_int_i[i];
            dest_int->opsm23_rx_lane_otl_ais_int_i[i] |= src_int->opsm23_rx_lane_otl_ais_int_i[i];
            dest_int->opsm23_rx_lane_align_change_int_i[i] |= src_int->opsm23_rx_lane_align_change_int_i[i];
            dest_int->opsm23_rx_lane_fifo_error_int_i[i] |= src_int->opsm23_rx_lane_fifo_error_int_i[i];
            dest_int->opsm23_rx_lane_oor_int_i[i] |= src_int->opsm23_rx_lane_oor_int_i[i];
            dest_int->opsm23_rx_lane_lor_int_i[i] |= src_int->opsm23_rx_lane_lor_int_i[i];
        }

        dest_int->opsm23_tx_sfi_fifo_overflow_int_v   = src_int->opsm23_tx_sfi_fifo_overflow_int_v;
        dest_int->opsm23_tx_sfi_fifo_underflow_int_v  = src_int->opsm23_tx_sfi_fifo_underflow_int_v;
        dest_int->opsm23_rx_sfi_los_int_v = src_int->opsm23_rx_sfi_los_int_v;
        dest_int->opsm23_rx_sfi_oof_int_v = src_int->opsm23_rx_sfi_oof_int_v;
        dest_int->opsm23_rx_sfi_lof_int_v = src_int->opsm23_rx_sfi_lof_int_v;
        dest_int->opsm23_rx_otu_oof_int_v = src_int->opsm23_rx_otu_oof_int_v;
        dest_int->opsm23_rx_otu_lof_int_v = src_int->opsm23_rx_otu_lof_int_v;
        dest_int->opsm23_rx_sfi_ais_int_v = src_int->opsm23_rx_sfi_ais_int_v;
        dest_int->opsm23_rx_lol_int_v     = src_int->opsm23_rx_lol_int_v;
        dest_int->opsm23_rx_skew_range_violation_int_v = src_int->opsm23_rx_skew_range_violation_int_v;
        dest_int->opsm23_rx_align_change_int_v  = src_int->opsm23_rx_align_change_int_v;
        dest_int->opsm23_rx_deskew_change_int_v = src_int->opsm23_rx_deskew_change_int_v;
        dest_int->opsm23_rx_sfi_fifo_overflow_int_v  = src_int->opsm23_rx_sfi_fifo_overflow_int_v;
        dest_int->opsm23_rx_sfi_fifo_underflow_int_v = src_int->opsm23_rx_sfi_fifo_underflow_int_v;
        for (i = 0; i < LINEOTN_MAX_LANES_PER_OPSM23_SLICE; i++)
        {
            dest_int->opsm23_tx_sys_fifo_overflow_int_v[i] = src_int->opsm23_tx_sys_fifo_overflow_int_v[i];
            dest_int->opsm23_tx_sys_fifo_underflow_int_v[i] = src_int->opsm23_tx_sys_fifo_underflow_int_v[i];
            dest_int->opsm23_tx_lane_fifo_overflow_int_v[i] = src_int->opsm23_tx_lane_fifo_overflow_int_v[i];
            dest_int->opsm23_tx_lane_fifo_underflow_int_v[i] = src_int->opsm23_tx_lane_fifo_underflow_int_v[i];
            dest_int->opsm23_rx_lane_los_int_v[i] = src_int->opsm23_rx_lane_los_int_v[i];
            dest_int->opsm23_rx_lane_oof_int_v[i] = src_int->opsm23_rx_lane_oof_int_v[i];
            dest_int->opsm23_rx_lane_lof_loflane_int_v[i] = src_int->opsm23_rx_lane_lof_loflane_int_v[i];
            dest_int->opsm23_rx_lane_otuk_ais_int_v[i] = src_int->opsm23_rx_lane_otuk_ais_int_v[i];
            dest_int->opsm23_rx_lane_otl_ais_int_v[i] = src_int->opsm23_rx_lane_otl_ais_int_v[i];
            dest_int->opsm23_rx_lane_align_change_int_v[i] = src_int->opsm23_rx_lane_align_change_int_v[i];
            dest_int->opsm23_rx_lane_fifo_error_int_v[i] = src_int->opsm23_rx_lane_fifo_error_int_v[i];
            dest_int->opsm23_rx_lane_oor_int_v[i] = src_int->opsm23_rx_lane_oor_int_v[i];
            dest_int->opsm23_rx_lane_lor_int_v[i] = src_int->opsm23_rx_lane_lor_int_v[i];
            dest_int->opsm23_rx_lane_lor_int_v[i] = src_int->opsm23_rx_lane_lor_int_v[i];
        }
    }

    if (src_int->opsm4_valid)
    {
        dest_int->opsm4_valid = src_int->opsm4_valid;

        dest_int->opsm4_tx_sfi_fifo_overflow_int_i   |= src_int->opsm4_tx_sfi_fifo_overflow_int_i;
        dest_int->opsm4_tx_sfi_fifo_underflow_int_i  |= src_int->opsm4_tx_sfi_fifo_underflow_int_i;
        dest_int->opsm4_tx_sys_fifo_overflow_int_i   |= src_int->opsm4_tx_sys_fifo_overflow_int_i;
        dest_int->opsm4_tx_sys_fifo_underflow_int_i  |= src_int->opsm4_tx_sys_fifo_underflow_int_i;
        dest_int->opsm4_tx_lane_fas_iae_int_i        |= src_int->opsm4_tx_lane_fas_iae_int_i;
        dest_int->opsm4_tx_lane_mfas_iae_int_i       |= src_int->opsm4_tx_lane_mfas_iae_int_i;
        dest_int->opsm4_tx_lane_fifo_overflow_int_i  |= src_int->opsm4_tx_lane_fifo_overflow_int_i;
        dest_int->opsm4_tx_lane_fifo_underflow_int_i |= src_int->opsm4_tx_lane_fifo_underflow_int_i;
        dest_int->opsm4_tx_otuk_ais_int_i |= src_int->opsm4_tx_otuk_ais_int_i;
        dest_int->opsm4_rx_sfi_los_int_i  |= src_int->opsm4_rx_sfi_los_int_i;
        dest_int->opsm4_rx_sfi_oof_int_i  |= src_int->opsm4_rx_sfi_oof_int_i;
        dest_int->opsm4_rx_sfi_lof_int_i  |= src_int->opsm4_rx_sfi_lof_int_i;
        dest_int->opsm4_rx_otu_oof_int_i  |= src_int->opsm4_rx_otu_oof_int_i;
        dest_int->opsm4_rx_otu_lof_int_i  |= src_int->opsm4_rx_otu_lof_int_i;
        dest_int->opsm4_rx_sfi_ais_int_i  |= src_int->opsm4_rx_sfi_ais_int_i;
        dest_int->opsm4_rx_lol_int_i      |= src_int->opsm4_rx_lol_int_i;
        dest_int->opsm4_rx_skew_range_violation_int_i |= src_int->opsm4_rx_skew_range_violation_int_i;
        dest_int->opsm4_rx_sfi_align_change_int_i   |= src_int->opsm4_rx_sfi_align_change_int_i;
        dest_int->opsm4_rx_deskew_change_int_i      |= src_int->opsm4_rx_deskew_change_int_i;
        dest_int->opsm4_rx_sfi_fifo_overflow_int_i  |= src_int->opsm4_rx_sfi_fifo_overflow_int_i;
        dest_int->opsm4_rx_sfi_fifo_underflow_int_i |= src_int->opsm4_rx_sfi_fifo_underflow_int_i;
        for (i = 0; i < 20; i++)
        {
            dest_int->opsm4_rx_lane_los_int_i[i] |= src_int->opsm4_rx_lane_los_int_i[i];
            dest_int->opsm4_rx_lane_oof_int_i[i] |= src_int->opsm4_rx_lane_oof_int_i[i];
            dest_int->opsm4_rx_lane_lof_loflane_int_i[i] |= src_int->opsm4_rx_lane_lof_loflane_int_i[i];
            dest_int->opsm4_rx_lane_oom_int_i[i] |= src_int->opsm4_rx_lane_oom_int_i[i];
            dest_int->opsm4_rx_lane_otuk_ais_int_i[i] |= src_int->opsm4_rx_lane_otuk_ais_int_i[i];
            dest_int->opsm4_rx_lane_align_change_int_i[i] |= src_int->opsm4_rx_lane_align_change_int_i[i];
            dest_int->opsm4_rx_lane_fifo_error_int_i[i] |= src_int->opsm4_rx_lane_fifo_error_int_i[i];
            dest_int->opsm4_rx_lane_oor_int_i[i] |= src_int->opsm4_rx_lane_oor_int_i[i];
            dest_int->opsm4_rx_lane_lor_int_i[i] |= src_int->opsm4_rx_lane_lor_int_i[i];
        }

        dest_int->opsm4_tx_sfi_fifo_underflow_int_v = src_int->opsm4_tx_sfi_fifo_underflow_int_v;
        dest_int->opsm4_tx_sys_fifo_overflow_int_v  = src_int->opsm4_tx_sys_fifo_overflow_int_v;
        dest_int->opsm4_tx_sys_fifo_underflow_int_v = src_int->opsm4_tx_sys_fifo_underflow_int_v;
        dest_int->opsm4_tx_lane_fas_iae_int_v  = src_int->opsm4_tx_lane_fas_iae_int_v;
        dest_int->opsm4_tx_lane_mfas_iae_int_v = src_int->opsm4_tx_lane_mfas_iae_int_v;
        dest_int->opsm4_tx_lane_fifo_overflow_int_v  = src_int->opsm4_tx_lane_fifo_overflow_int_v;
        dest_int->opsm4_tx_lane_fifo_underflow_int_v = src_int->opsm4_tx_lane_fifo_underflow_int_v;
        dest_int->opsm4_tx_otuk_ais_int_v = src_int->opsm4_tx_otuk_ais_int_v;
        dest_int->opsm4_rx_sfi_los_int_v = src_int->opsm4_rx_sfi_los_int_v;
        dest_int->opsm4_rx_sfi_oof_int_v = src_int->opsm4_rx_sfi_oof_int_v;
        dest_int->opsm4_rx_sfi_lof_int_v = src_int->opsm4_rx_sfi_lof_int_v;
        dest_int->opsm4_rx_otu_oof_int_v = src_int->opsm4_rx_otu_oof_int_v;
        dest_int->opsm4_rx_otu_lof_int_v = src_int->opsm4_rx_otu_lof_int_v;
        dest_int->opsm4_rx_sfi_ais_int_v = src_int->opsm4_rx_sfi_ais_int_v;
        dest_int->opsm4_rx_lol_int_v = src_int->opsm4_rx_lol_int_v;
        dest_int->opsm4_rx_skew_range_violation_int_v = src_int->opsm4_rx_skew_range_violation_int_v;
        dest_int->opsm4_rx_sfi_align_change_int_v = src_int->opsm4_rx_sfi_align_change_int_v;
        dest_int->opsm4_rx_deskew_change_int_v = src_int->opsm4_rx_deskew_change_int_v;
        dest_int->opsm4_rx_sfi_fifo_overflow_int_v = src_int->opsm4_rx_sfi_fifo_overflow_int_v;
        dest_int->opsm4_rx_sfi_fifo_underflow_int_v = src_int->opsm4_rx_sfi_fifo_underflow_int_v;
        for (i = 0; i < 20; i++)
        {
            dest_int->opsm4_rx_lane_los_int_v[i] = src_int->opsm4_rx_lane_los_int_v[i];
            dest_int->opsm4_rx_lane_oof_int_v[i] = src_int->opsm4_rx_lane_oof_int_v[i];
            dest_int->opsm4_rx_lane_lof_loflane_int_v[i] = src_int->opsm4_rx_lane_lof_loflane_int_v[i];
            dest_int->opsm4_rx_lane_oom_int_v[i] = src_int->opsm4_rx_lane_oom_int_v[i];
            dest_int->opsm4_rx_lane_otuk_ais_int_v[i] = src_int->opsm4_rx_lane_otuk_ais_int_v[i];
            dest_int->opsm4_rx_lane_align_change_int_v[i] = src_int->opsm4_rx_lane_align_change_int_v[i];
            dest_int->opsm4_rx_lane_fifo_error_int_v[i] = src_int->opsm4_rx_lane_fifo_error_int_v[i];
            dest_int->opsm4_rx_lane_oor_int_v[i] = src_int->opsm4_rx_lane_oor_int_v[i];
            dest_int->opsm4_rx_lane_lor_int_v[i] = src_int->opsm4_rx_lane_lor_int_v[i];
        }
    }

    PMC_RETURN(rc);
} /* adpt_otu_int_merge */


/*******************************************************************************
* adpt_odu_int_merge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Merges ODU defect and interrupt data from shared memory to Adapter. 
*
* INPUTS:
*   *src_int           - shared memory ODU defect and int data.
*   *dest_int          - Adapter ODU defect and int data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_odu_int_merge(coreotn_int_chnl_t *src_int,
                                  coreotn_int_chnl_t *dest_int)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(src_int);
    ADPT_PARA_VERIFY(dest_int);

    /* PM interrupt */
    dest_int->framer_chnl.rfrm_valid = src_int->framer_chnl.rfrm_valid;
    dest_int->framer_chnl.rfrm.ac_aps_i |= src_int->framer_chnl.rfrm.ac_aps_i;
    dest_int->framer_chnl.rfrm.dlck_pm_i |= src_int->framer_chnl.rfrm.dlck_pm_i;
    dest_int->framer_chnl.rfrm.doci_pm_i |= src_int->framer_chnl.rfrm.doci_pm_i;
    dest_int->framer_chnl.rfrm.dais_pm_i |= src_int->framer_chnl.rfrm.dais_pm_i;
    dest_int->framer_chnl.rfrm.dtim_pm_i |= src_int->framer_chnl.rfrm.dtim_pm_i;
    dest_int->framer_chnl.rfrm.ddeg_pm_i |= src_int->framer_chnl.rfrm.ddeg_pm_i;
    dest_int->framer_chnl.rfrm.ddeg_enh_pm_i |= src_int->framer_chnl.rfrm.ddeg_enh_pm_i;
    dest_int->framer_chnl.rfrm.ddeg_bursty_pm_i |= src_int->framer_chnl.rfrm.ddeg_bursty_pm_i;
    dest_int->framer_chnl.rfrm.dbdi_pm_i |= src_int->framer_chnl.rfrm.dbdi_pm_i;
    dest_int->framer_chnl.rfrm.oof_i |= src_int->framer_chnl.rfrm.oof_i;
    dest_int->framer_chnl.rfrm.dloflom_i |= src_int->framer_chnl.rfrm.dloflom_i;
    dest_int->framer_chnl.rfrm.atsf_pm_i |= src_int->framer_chnl.rfrm.atsf_pm_i;
    dest_int->framer_chnl.rfrm.atsd_pm_i |= src_int->framer_chnl.rfrm.atsd_pm_i;
    dest_int->framer_chnl.rfrm.abdi_pm_i |= src_int->framer_chnl.rfrm.abdi_pm_i;
    
    dest_int->framer_chnl.rfrm.dlck_pm_v = src_int->framer_chnl.rfrm.dlck_pm_v;
    dest_int->framer_chnl.rfrm.doci_pm_v = src_int->framer_chnl.rfrm.doci_pm_v;
    dest_int->framer_chnl.rfrm.dais_pm_v = src_int->framer_chnl.rfrm.dais_pm_v;
    dest_int->framer_chnl.rfrm.dtim_pm_v = src_int->framer_chnl.rfrm.dtim_pm_v;
    dest_int->framer_chnl.rfrm.ddeg_pm_v = src_int->framer_chnl.rfrm.ddeg_pm_v;
    dest_int->framer_chnl.rfrm.ddeg_enh_pm_v = src_int->framer_chnl.rfrm.ddeg_enh_pm_v;
    dest_int->framer_chnl.rfrm.ddeg_bursty_pm_v = src_int->framer_chnl.rfrm.ddeg_bursty_pm_v;
    dest_int->framer_chnl.rfrm.dbdi_pm_v = src_int->framer_chnl.rfrm.dbdi_pm_v;
    dest_int->framer_chnl.rfrm.oof_v = src_int->framer_chnl.rfrm.oof_v;
    dest_int->framer_chnl.rfrm.dloflom_v = src_int->framer_chnl.rfrm.dloflom_v;
    dest_int->framer_chnl.rfrm.ci_ssf_pm_v = src_int->framer_chnl.rfrm.ci_ssf_pm_v;
    dest_int->framer_chnl.rfrm.atsf_pm_v = src_int->framer_chnl.rfrm.atsf_pm_v;
    dest_int->framer_chnl.rfrm.atsd_pm_v = src_int->framer_chnl.rfrm.atsd_pm_v;
    dest_int->framer_chnl.rfrm.abdi_pm_v = src_int->framer_chnl.rfrm.abdi_pm_v;

    /* tfrm part */
    if (src_int->framer_chnl.tfrm_valid)
    {
        dest_int->framer_chnl.tfrm_valid = src_int->framer_chnl.tfrm_valid;
        dest_int->framer_chnl.tfrm.iae_i |= src_int->framer_chnl.tfrm.iae_i;
        dest_int->framer_chnl.tfrm.dm_pm_i |= src_int->framer_chnl.tfrm.dm_pm_i;

        dest_int->framer_chnl.tfrm.iae_v = src_int->framer_chnl.tfrm.iae_v;
    }

    /* demux part */
    dest_int->framer_chnl.odtu_dmx_valid = src_int->framer_chnl.odtu_dmx_valid;
    dest_int->framer_chnl.odtu_dmx.dplm_i |= src_int->framer_chnl.odtu_dmx.dplm_i;
    dest_int->framer_chnl.odtu_dmx.dmsim_i |= src_int->framer_chnl.odtu_dmx.dmsim_i;
    dest_int->framer_chnl.odtu_dmx.ssf_sf_rcvd_i |= src_int->framer_chnl.odtu_dmx.ssf_sf_rcvd_i;
    dest_int->framer_chnl.odtu_dmx.cm_out_limit_i |= src_int->framer_chnl.odtu_dmx.cm_out_limit_i;
    dest_int->framer_chnl.odtu_dmx.cnd_max_i |= src_int->framer_chnl.odtu_dmx.cnd_max_i;
    dest_int->framer_chnl.odtu_dmx.jc_nomtch_lossync_i |= src_int->framer_chnl.odtu_dmx.jc_nomtch_lossync_i;
    dest_int->framer_chnl.odtu_dmx.cm_cnd_crc_err_i |= src_int->framer_chnl.odtu_dmx.cm_cnd_crc_err_i;

    dest_int->framer_chnl.odtu_dmx.dplm_v = src_int->framer_chnl.odtu_dmx.dplm_v;
    dest_int->framer_chnl.odtu_dmx.dmsim_v = src_int->framer_chnl.odtu_dmx.dmsim_v;
    dest_int->framer_chnl.odtu_dmx.ssf_sf_rcvd_v = src_int->framer_chnl.odtu_dmx.ssf_sf_rcvd_v;

    /* tgmp part */
    dest_int->framer_chnl.odtu_mux_valid = src_int->framer_chnl.odtu_mux_valid;
    if (src_int->framer_chnl.odtu_mux_valid)
    {
        dest_int->framer_chnl.odtu_mux.tgmp.ohfs_insert.cfc.fifo_ovr_i |= 
        src_int->framer_chnl.odtu_mux.tgmp.ohfs_insert.cfc.fifo_ovr_i;
        dest_int->framer_chnl.odtu_mux.tgmp.ohfs_insert.cfc.fifo_udr_i |= 
        src_int->framer_chnl.odtu_mux.tgmp.ohfs_insert.cfc.fifo_udr_i;

        dest_int->framer_chnl.odtu_mux.tgmp.txjc_proc.cfc.fifo_ovr_i |= 
        src_int->framer_chnl.odtu_mux.tgmp.txjc_proc.cfc.fifo_ovr_i;
        dest_int->framer_chnl.odtu_mux.tgmp.txjc_proc.cfc.fifo_udr_i |= 
        src_int->framer_chnl.odtu_mux.tgmp.txjc_proc.cfc.fifo_udr_i;

        dest_int->framer_chnl.odtu_mux.tgmp.expand_ibuf.cfc.fifo_ovr_i |= 
        src_int->framer_chnl.odtu_mux.tgmp.expand_ibuf.cfc.fifo_ovr_i;
        dest_int->framer_chnl.odtu_mux.tgmp.expand_ibuf.cfc.fifo_udr_i |= 
        src_int->framer_chnl.odtu_mux.tgmp.expand_ibuf.cfc.fifo_udr_i;
    }

    /* oduksc part */
    dest_int->framer_chnl.oduksc_valid = src_int->framer_chnl.oduksc_valid;
    if (src_int->framer_chnl.oduksc_valid)
    {
        dest_int->framer_chnl.oduksc.src_zone_i |= src_int->framer_chnl.oduksc.src_zone_i;
        dest_int->framer_chnl.oduksc.ramp_done_i |= src_int->framer_chnl.oduksc.ramp_done_i;
        dest_int->framer_chnl.oduksc.src_zone_v = src_int->framer_chnl.oduksc.src_zone_v;
    }

    /* odujat part */
    dest_int->framer_chnl.odujat_valid = src_int->framer_chnl.odujat_valid;
    if (src_int->framer_chnl.odujat_valid)
    {
        dest_int->framer_chnl.odujat.cfc.fifo_ovr_i |= src_int->framer_chnl.odujat.cfc.fifo_ovr_i;
        dest_int->framer_chnl.odujat.cfc.fifo_udr_i |= src_int->framer_chnl.odujat.cfc.fifo_udr_i;
    }

    /* mpmo */
    dest_int->framer_chnl.mpmo_valid = src_int->framer_chnl.mpmo_valid;
    if (src_int->framer_chnl.mpmo_valid)
    {
        dest_int->framer_chnl.mpmo.cfc.fifo_ovr_i |= src_int->framer_chnl.mpmo.cfc.fifo_ovr_i;
        dest_int->framer_chnl.mpmo.cfc.fifo_udr_i |= src_int->framer_chnl.mpmo.cfc.fifo_udr_i;
        dest_int->framer_chnl.mpmo.cfc.ipt_err_i |= src_int->framer_chnl.mpmo.cfc.ipt_err_i;
        dest_int->framer_chnl.mpmo.pkt_drop_replace_i |= src_int->framer_chnl.mpmo.pkt_drop_replace_i;
        dest_int->framer_chnl.mpmo.pkt_drop_no_replace_i |= src_int->framer_chnl.mpmo.pkt_drop_no_replace_i;
    }

    /* mpma */
    dest_int->framer_chnl.mpma_valid = src_int->framer_chnl.mpma_valid;
    if (src_int->framer_chnl.mpma_valid)
    {
        dest_int->framer_chnl.mpma.cfc.fifo_ovr_i |= src_int->framer_chnl.mpma.cfc.fifo_ovr_i;
        dest_int->framer_chnl.mpma.cfc.fifo_udr_i |= src_int->framer_chnl.mpma.cfc.fifo_udr_i;
        dest_int->framer_chnl.mpma.cfc.ipt_err_i  |= src_int->framer_chnl.mpma.cfc.ipt_err_i;
        dest_int->framer_chnl.mpma.psd_fifo_ovr_i |= src_int->framer_chnl.mpma.psd_fifo_ovr_i;
        dest_int->framer_chnl.mpma.psd_fifo_udr_i |= src_int->framer_chnl.mpma.psd_fifo_udr_i;
    }

    /* TCM interrupt */
    for (index = 0; index < DIGI_OTN_SERVER_PIDS_PER_CHNL; index++)
    {
        dest_int->pid.rx_tcm_pool_ids[index] = src_int->pid.rx_tcm_pool_ids[index];
        if (ODU_RFRM_UNASSIGNED_POOL_ID == src_int->pid.rx_tcm_pool_ids[index])
        {
            continue;
        }

        dest_int->pid.rx_tcm[index].dlck_i |= src_int->pid.rx_tcm[index].dlck_i;
        dest_int->pid.rx_tcm[index].doci_i |= src_int->pid.rx_tcm[index].doci_i;
        dest_int->pid.rx_tcm[index].dais_i |= src_int->pid.rx_tcm[index].dais_i;
        dest_int->pid.rx_tcm[index].dtim_i |= src_int->pid.rx_tcm[index].dtim_i;
        dest_int->pid.rx_tcm[index].ddeg_i |= src_int->pid.rx_tcm[index].ddeg_i;
        dest_int->pid.rx_tcm[index].ddeg_enh_i |= src_int->pid.rx_tcm[index].ddeg_enh_i;
        dest_int->pid.rx_tcm[index].ddeg_bursty_i |= src_int->pid.rx_tcm[index].ddeg_bursty_i;
        dest_int->pid.rx_tcm[index].dbdi_i |= src_int->pid.rx_tcm[index].dbdi_i;
        dest_int->pid.rx_tcm[index].dbiae_i |= src_int->pid.rx_tcm[index].dbiae_i;
        dest_int->pid.rx_tcm[index].dltc_i |= src_int->pid.rx_tcm[index].dltc_i;
        dest_int->pid.rx_tcm[index].diae_i |= src_int->pid.rx_tcm[index].diae_i;
        dest_int->pid.rx_tcm[index].atsf_i |= src_int->pid.rx_tcm[index].atsf_i;
        dest_int->pid.rx_tcm[index].atsd_i |= src_int->pid.rx_tcm[index].atsd_i;
        dest_int->pid.rx_tcm[index].aais_i |= src_int->pid.rx_tcm[index].aais_i;
        dest_int->pid.rx_tcm[index].abdi_i |= src_int->pid.rx_tcm[index].abdi_i;

        dest_int->pid.rx_tcm[index].dlck_v = src_int->pid.rx_tcm[index].dlck_v;
        dest_int->pid.rx_tcm[index].doci_v = src_int->pid.rx_tcm[index].doci_v;
        dest_int->pid.rx_tcm[index].dais_v = src_int->pid.rx_tcm[index].dais_v;
        dest_int->pid.rx_tcm[index].dtim_v = src_int->pid.rx_tcm[index].dtim_v;
        dest_int->pid.rx_tcm[index].ddeg_v = src_int->pid.rx_tcm[index].ddeg_v;
        dest_int->pid.rx_tcm[index].ddeg_enh_v = src_int->pid.rx_tcm[index].ddeg_enh_v;
        dest_int->pid.rx_tcm[index].ddeg_bursty_v = src_int->pid.rx_tcm[index].ddeg_bursty_v;
        dest_int->pid.rx_tcm[index].dbdi_v = src_int->pid.rx_tcm[index].dbdi_v;
        dest_int->pid.rx_tcm[index].dbiae_v = src_int->pid.rx_tcm[index].dbiae_v;
        dest_int->pid.rx_tcm[index].dltc_v = src_int->pid.rx_tcm[index].dltc_v;
        dest_int->pid.rx_tcm[index].diae_v = src_int->pid.rx_tcm[index].diae_v;
        dest_int->pid.rx_tcm[index].ci_ssf_v = src_int->pid.rx_tcm[index].ci_ssf_v;
        dest_int->pid.rx_tcm[index].atsf_v = src_int->pid.rx_tcm[index].atsf_v;
        dest_int->pid.rx_tcm[index].atsd_v = src_int->pid.rx_tcm[index].atsd_v;
        dest_int->pid.rx_tcm[index].aais_v = src_int->pid.rx_tcm[index].aais_v;
        dest_int->pid.rx_tcm[index].abdi_v = src_int->pid.rx_tcm[index].abdi_v;
    }

    dest_int->prbs_valid = src_int->prbs_valid;
    if (src_int->prbs_valid)
    {
        /* PRBS interrupt */
        dest_int->prbs.prbs_dlss_i |= src_int->prbs.prbs_dlss_i;
        dest_int->prbs.prbs_dlss_v = src_int->prbs.prbs_dlss_v;
    }

    PMC_RETURN(rc);
} /* adpt_odu_int_merge */


/*******************************************************************************
* adpt_otn_int_merge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Merges OTN defect and interrupt data from shared memory to Adapter. 
*
* INPUTS:
*   *src_int           - shared memory OTN defect and int data.
*   *dest_int          - Adapter OTN defect and int data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_int_merge(digi_otn_int_t *src_int,
                                 digi_otn_int_t *dest_int)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(src_int);
    ADPT_PARA_VERIFY(dest_int);


    /* LINEOTN */
    dest_int->lineotn.otu_frm.oduksc.acc_undf_i |= src_int->lineotn.otu_frm.oduksc.acc_undf_i;
    dest_int->lineotn.otu_frm.oduksc.acc_ovf_i |= src_int->lineotn.otu_frm.oduksc.acc_ovf_i;
    dest_int->lineotn.fecstat_air_err_int_i |= src_int->lineotn.fecstat_air_err_int_i;  
    for (index = 0; index < DIGI_OTN_SERVER_CHNL_MAX; index++)
    {
        dest_int->otn_server_handle[index] = src_int->otn_server_handle[index];
        if (NULL == src_int->otn_server_handle[index])
        {
            continue;
        }

        rc = adpt_otu_int_merge(&src_int->otn_server_chnl[index],
                                &dest_int->otn_server_chnl[index]);
    }

    /* SYSOTN */
    dest_int->sysotn.otu_frm.oduksc.acc_undf_i |= src_int->sysotn.otu_frm.oduksc.acc_undf_i;
    dest_int->sysotn.otu_frm.oduksc.acc_ovf_i |= src_int->sysotn.otu_frm.oduksc.acc_ovf_i;
    dest_int->sysotn.fecstat_air_err_int_i |= src_int->sysotn.fecstat_air_err_int_i;  
    for (index = 0; index < DIGI_OTN_SERVER_SYS_CHNL_MAX; index++)
    {
        dest_int->sysotn_server_handle[index] = src_int->sysotn_server_handle[index];
        if (NULL == src_int->sysotn_server_handle[index])
        {
            continue;
        }

        rc = adpt_otu_int_merge(&src_int->sysotn_server_chnl[index],
                                &dest_int->sysotn_server_chnl[index]);
    }

    /* COREOTN */
    if (src_int->coreotn.fmf1_dmx_valid)
    {
        dest_int->coreotn.fmf1_dmx_valid = src_int->coreotn.fmf1_dmx_valid;
        dest_int->coreotn.fmf1_dmx.dloomfi_i |= src_int->coreotn.fmf1_dmx.dloomfi_i;
        dest_int->coreotn.fmf1_dmx.dloomfi_v = src_int->coreotn.fmf1_dmx.dloomfi_v;
    }

    if (src_int->coreotn.fmf2_dmx_valid)
    {
        dest_int->coreotn.fmf2_dmx_valid = src_int->coreotn.fmf2_dmx_valid;
        dest_int->coreotn.fmf2_dmx.dloomfi_i |= src_int->coreotn.fmf2_dmx.dloomfi_i;
        dest_int->coreotn.fmf2_dmx.dloomfi_v = src_int->coreotn.fmf2_dmx.dloomfi_v;
    }

    if (src_int->coreotn.fmf1_oduksc_valid)
    {
        dest_int->coreotn.fmf1_oduksc_valid = src_int->coreotn.fmf1_oduksc_valid;
        dest_int->coreotn.fmf1_oduksc.acc_undf_i |= src_int->coreotn.fmf1_oduksc.acc_undf_i;
        dest_int->coreotn.fmf1_oduksc.acc_ovf_i |= src_int->coreotn.fmf1_oduksc.acc_ovf_i;
    }

    if (src_int->coreotn.fmf2_oduksc_valid)
    {
        dest_int->coreotn.fmf2_oduksc_valid = src_int->coreotn.fmf2_oduksc_valid;
        dest_int->coreotn.fmf2_oduksc.acc_undf_i |= src_int->coreotn.fmf2_oduksc.acc_undf_i;
        dest_int->coreotn.fmf2_oduksc.acc_ovf_i |= src_int->coreotn.fmf2_oduksc.acc_ovf_i;
    }

    if (src_int->coreotn.fmf1_odujat_valid)
    {
        dest_int->coreotn.fmf1_odujat_valid = src_int->coreotn.fmf1_odujat_valid;
        dest_int->coreotn.fmf1_odujat.gain_ovf_i |= src_int->coreotn.fmf1_odujat.gain_ovf_i;
        dest_int->coreotn.fmf1_odujat.gain_undf_i |= src_int->coreotn.fmf1_odujat.gain_undf_i;
        dest_int->coreotn.fmf1_odujat.dq_limit_i |= src_int->coreotn.fmf1_odujat.dq_limit_i;
    }

    if (src_int->coreotn.fmf2_odujat_valid)
    {
        dest_int->coreotn.fmf2_odujat_valid = src_int->coreotn.fmf2_odujat_valid;
        dest_int->coreotn.fmf2_odujat.gain_ovf_i |= src_int->coreotn.fmf2_odujat.gain_ovf_i;
        dest_int->coreotn.fmf2_odujat.gain_undf_i |= src_int->coreotn.fmf2_odujat.gain_undf_i;
        dest_int->coreotn.fmf2_odujat.dq_limit_i |= src_int->coreotn.fmf2_odujat.dq_limit_i;
    }

    if ((src_int->coreotn.framer_stg1.tfrm_valid) || (src_int->coreotn.framer_stg1.rfrm_valid))
    {
        dest_int->coreotn.framer_stg1.tfrm_valid = src_int->coreotn.framer_stg1.tfrm_valid;
        dest_int->coreotn.framer_stg1.tfrm.dm_fifo_udr_i |= src_int->coreotn.framer_stg1.tfrm.dm_fifo_udr_i;
        dest_int->coreotn.framer_stg1.tfrm.dm_fifo_ovr_i |= src_int->coreotn.framer_stg1.tfrm.dm_fifo_ovr_i;
        dest_int->coreotn.framer_stg1.rfrm_valid = src_int->coreotn.framer_stg1.rfrm_valid;
        dest_int->coreotn.framer_stg1.rfrm.comb_dlck_i  |= src_int->coreotn.framer_stg1.rfrm.comb_dlck_i;
        dest_int->coreotn.framer_stg1.rfrm.comb_doci_i  |= src_int->coreotn.framer_stg1.rfrm.comb_doci_i;
        dest_int->coreotn.framer_stg1.rfrm.comb_dais_i  |= src_int->coreotn.framer_stg1.rfrm.comb_dais_i;
        dest_int->coreotn.framer_stg1.rfrm.comb_dtim_i  |= src_int->coreotn.framer_stg1.rfrm.comb_dtim_i;
        dest_int->coreotn.framer_stg1.rfrm.comb_ddeg_i  |= src_int->coreotn.framer_stg1.rfrm.comb_ddeg_i;
        dest_int->coreotn.framer_stg1.rfrm.comb_dbdi_i  |= src_int->coreotn.framer_stg1.rfrm.comb_dbdi_i;
        dest_int->coreotn.framer_stg1.rfrm.comb_dltc_i  |= src_int->coreotn.framer_stg1.rfrm.comb_dltc_i;
        dest_int->coreotn.framer_stg1.rfrm.comb_diae_i  |= src_int->coreotn.framer_stg1.rfrm.comb_diae_i;
        dest_int->coreotn.framer_stg1.rfrm.comb_dbiae_i |= src_int->coreotn.framer_stg1.rfrm.comb_dbiae_i;
        dest_int->coreotn.framer_stg1.rfrm.comb_oof_i   |= src_int->coreotn.framer_stg1.rfrm.comb_oof_i;
        dest_int->coreotn.framer_stg1.rfrm.comb_atsf_i  |= src_int->coreotn.framer_stg1.rfrm.comb_atsf_i;
        dest_int->coreotn.framer_stg1.rfrm.comb_atsd_i  |= src_int->coreotn.framer_stg1.rfrm.comb_atsd_i;
        dest_int->coreotn.framer_stg1.rfrm.comb_aais_i  |= src_int->coreotn.framer_stg1.rfrm.comb_aais_i;
        dest_int->coreotn.framer_stg1.rfrm.comb_abdi_i  |= src_int->coreotn.framer_stg1.rfrm.comb_abdi_i;
        dest_int->coreotn.framer_stg1.rfrm.comb_dloflom_i |= src_int->coreotn.framer_stg1.rfrm.comb_dloflom_i;
    }

    if ((src_int->coreotn.framer_stg2.tfrm_valid) || (src_int->coreotn.framer_stg2.rfrm_valid))
    {
        dest_int->coreotn.framer_stg2.tfrm_valid = src_int->coreotn.framer_stg2.tfrm_valid;
        dest_int->coreotn.framer_stg2.tfrm.dm_fifo_udr_i |= src_int->coreotn.framer_stg2.tfrm.dm_fifo_udr_i;
        dest_int->coreotn.framer_stg2.tfrm.dm_fifo_ovr_i |= src_int->coreotn.framer_stg2.tfrm.dm_fifo_ovr_i;
        dest_int->coreotn.framer_stg2.rfrm_valid = src_int->coreotn.framer_stg2.rfrm_valid;
        dest_int->coreotn.framer_stg2.rfrm.comb_dlck_i  |= src_int->coreotn.framer_stg2.rfrm.comb_dlck_i;
        dest_int->coreotn.framer_stg2.rfrm.comb_doci_i  |= src_int->coreotn.framer_stg2.rfrm.comb_doci_i;
        dest_int->coreotn.framer_stg2.rfrm.comb_dais_i  |= src_int->coreotn.framer_stg2.rfrm.comb_dais_i;
        dest_int->coreotn.framer_stg2.rfrm.comb_dtim_i  |= src_int->coreotn.framer_stg2.rfrm.comb_dtim_i;
        dest_int->coreotn.framer_stg2.rfrm.comb_ddeg_i  |= src_int->coreotn.framer_stg2.rfrm.comb_ddeg_i;
        dest_int->coreotn.framer_stg2.rfrm.comb_dbdi_i  |= src_int->coreotn.framer_stg2.rfrm.comb_dbdi_i;
        dest_int->coreotn.framer_stg2.rfrm.comb_dltc_i  |= src_int->coreotn.framer_stg2.rfrm.comb_dltc_i;
        dest_int->coreotn.framer_stg2.rfrm.comb_diae_i  |= src_int->coreotn.framer_stg2.rfrm.comb_diae_i;
        dest_int->coreotn.framer_stg2.rfrm.comb_dbiae_i |= src_int->coreotn.framer_stg2.rfrm.comb_dbiae_i;
        dest_int->coreotn.framer_stg2.rfrm.comb_oof_i   |= src_int->coreotn.framer_stg2.rfrm.comb_oof_i;
        dest_int->coreotn.framer_stg2.rfrm.comb_atsf_i  |= src_int->coreotn.framer_stg2.rfrm.comb_atsf_i;
        dest_int->coreotn.framer_stg2.rfrm.comb_atsd_i  |= src_int->coreotn.framer_stg2.rfrm.comb_atsd_i;
        dest_int->coreotn.framer_stg2.rfrm.comb_aais_i  |= src_int->coreotn.framer_stg2.rfrm.comb_aais_i;
        dest_int->coreotn.framer_stg2.rfrm.comb_abdi_i  |= src_int->coreotn.framer_stg2.rfrm.comb_abdi_i;
        dest_int->coreotn.framer_stg2.rfrm.comb_dloflom_i |= src_int->coreotn.framer_stg2.rfrm.comb_dloflom_i;
    }

    if ((src_int->coreotn.framer_stg3a.tfrm_valid) || (src_int->coreotn.framer_stg3a.rfrm_valid))
    {
        dest_int->coreotn.framer_stg3a.tfrm_valid = src_int->coreotn.framer_stg3a.tfrm_valid;
        dest_int->coreotn.framer_stg3a.tfrm.dm_fifo_udr_i |= src_int->coreotn.framer_stg3a.tfrm.dm_fifo_udr_i;
        dest_int->coreotn.framer_stg3a.tfrm.dm_fifo_ovr_i |= src_int->coreotn.framer_stg3a.tfrm.dm_fifo_ovr_i;
        dest_int->coreotn.framer_stg3a.rfrm_valid = src_int->coreotn.framer_stg3a.rfrm_valid;
        dest_int->coreotn.framer_stg3a.rfrm.comb_dlck_i  |= src_int->coreotn.framer_stg3a.rfrm.comb_dlck_i;
        dest_int->coreotn.framer_stg3a.rfrm.comb_doci_i  |= src_int->coreotn.framer_stg3a.rfrm.comb_doci_i;
        dest_int->coreotn.framer_stg3a.rfrm.comb_dais_i  |= src_int->coreotn.framer_stg3a.rfrm.comb_dais_i;
        dest_int->coreotn.framer_stg3a.rfrm.comb_dtim_i  |= src_int->coreotn.framer_stg3a.rfrm.comb_dtim_i;
        dest_int->coreotn.framer_stg3a.rfrm.comb_ddeg_i  |= src_int->coreotn.framer_stg3a.rfrm.comb_ddeg_i;
        dest_int->coreotn.framer_stg3a.rfrm.comb_dbdi_i  |= src_int->coreotn.framer_stg3a.rfrm.comb_dbdi_i;
        dest_int->coreotn.framer_stg3a.rfrm.comb_dltc_i  |= src_int->coreotn.framer_stg3a.rfrm.comb_dltc_i;
        dest_int->coreotn.framer_stg3a.rfrm.comb_diae_i  |= src_int->coreotn.framer_stg3a.rfrm.comb_diae_i;
        dest_int->coreotn.framer_stg3a.rfrm.comb_dbiae_i |= src_int->coreotn.framer_stg3a.rfrm.comb_dbiae_i;
        dest_int->coreotn.framer_stg3a.rfrm.comb_oof_i   |= src_int->coreotn.framer_stg3a.rfrm.comb_oof_i;
        dest_int->coreotn.framer_stg3a.rfrm.comb_atsf_i  |= src_int->coreotn.framer_stg3a.rfrm.comb_atsf_i;
        dest_int->coreotn.framer_stg3a.rfrm.comb_atsd_i  |= src_int->coreotn.framer_stg3a.rfrm.comb_atsd_i;
        dest_int->coreotn.framer_stg3a.rfrm.comb_aais_i  |= src_int->coreotn.framer_stg3a.rfrm.comb_aais_i;
        dest_int->coreotn.framer_stg3a.rfrm.comb_abdi_i  |= src_int->coreotn.framer_stg3a.rfrm.comb_abdi_i;
        dest_int->coreotn.framer_stg3a.rfrm.comb_dloflom_i |= src_int->coreotn.framer_stg3a.rfrm.comb_dloflom_i;
    }

    if ((src_int->coreotn.framer_stg3b.tfrm_valid) || (src_int->coreotn.framer_stg3b.rfrm_valid))
    {
        dest_int->coreotn.framer_stg3b.tfrm_valid = src_int->coreotn.framer_stg3b.tfrm_valid;
        dest_int->coreotn.framer_stg3b.tfrm.dm_fifo_udr_i |= src_int->coreotn.framer_stg3b.tfrm.dm_fifo_udr_i;
        dest_int->coreotn.framer_stg3b.tfrm.dm_fifo_ovr_i |= src_int->coreotn.framer_stg3b.tfrm.dm_fifo_ovr_i;
        dest_int->coreotn.framer_stg3b.rfrm_valid = src_int->coreotn.framer_stg3b.rfrm_valid;
        dest_int->coreotn.framer_stg3b.rfrm.comb_dlck_i  |= src_int->coreotn.framer_stg3b.rfrm.comb_dlck_i;
        dest_int->coreotn.framer_stg3b.rfrm.comb_doci_i  |= src_int->coreotn.framer_stg3b.rfrm.comb_doci_i;
        dest_int->coreotn.framer_stg3b.rfrm.comb_dais_i  |= src_int->coreotn.framer_stg3b.rfrm.comb_dais_i;
        dest_int->coreotn.framer_stg3b.rfrm.comb_dtim_i  |= src_int->coreotn.framer_stg3b.rfrm.comb_dtim_i;
        dest_int->coreotn.framer_stg3b.rfrm.comb_ddeg_i  |= src_int->coreotn.framer_stg3b.rfrm.comb_ddeg_i;
        dest_int->coreotn.framer_stg3b.rfrm.comb_dbdi_i  |= src_int->coreotn.framer_stg3b.rfrm.comb_dbdi_i;
        dest_int->coreotn.framer_stg3b.rfrm.comb_dltc_i  |= src_int->coreotn.framer_stg3b.rfrm.comb_dltc_i;
        dest_int->coreotn.framer_stg3b.rfrm.comb_diae_i  |= src_int->coreotn.framer_stg3b.rfrm.comb_diae_i;
        dest_int->coreotn.framer_stg3b.rfrm.comb_dbiae_i |= src_int->coreotn.framer_stg3b.rfrm.comb_dbiae_i;
        dest_int->coreotn.framer_stg3b.rfrm.comb_oof_i   |= src_int->coreotn.framer_stg3b.rfrm.comb_oof_i;
        dest_int->coreotn.framer_stg3b.rfrm.comb_atsf_i  |= src_int->coreotn.framer_stg3b.rfrm.comb_atsf_i;
        dest_int->coreotn.framer_stg3b.rfrm.comb_atsd_i  |= src_int->coreotn.framer_stg3b.rfrm.comb_atsd_i;
        dest_int->coreotn.framer_stg3b.rfrm.comb_aais_i  |= src_int->coreotn.framer_stg3b.rfrm.comb_aais_i;
        dest_int->coreotn.framer_stg3b.rfrm.comb_abdi_i  |= src_int->coreotn.framer_stg3b.rfrm.comb_abdi_i;
        dest_int->coreotn.framer_stg3b.rfrm.comb_dloflom_i |= src_int->coreotn.framer_stg3b.rfrm.comb_dloflom_i;
    }

    if ((src_int->coreotn.framer_stgN.tfrm_valid) || (src_int->coreotn.framer_stgN.rfrm_valid))
    {
        dest_int->coreotn.framer_stgN.tfrm_valid = src_int->coreotn.framer_stgN.tfrm_valid;
        dest_int->coreotn.framer_stgN.tfrm.dm_fifo_udr_i |= src_int->coreotn.framer_stgN.tfrm.dm_fifo_udr_i;
        dest_int->coreotn.framer_stgN.tfrm.dm_fifo_ovr_i |= src_int->coreotn.framer_stgN.tfrm.dm_fifo_ovr_i;
        dest_int->coreotn.framer_stgN.rfrm_valid = src_int->coreotn.framer_stgN.rfrm_valid;
        dest_int->coreotn.framer_stgN.rfrm.comb_dlck_i  |= src_int->coreotn.framer_stgN.rfrm.comb_dlck_i;
        dest_int->coreotn.framer_stgN.rfrm.comb_doci_i  |= src_int->coreotn.framer_stgN.rfrm.comb_doci_i;
        dest_int->coreotn.framer_stgN.rfrm.comb_dais_i  |= src_int->coreotn.framer_stgN.rfrm.comb_dais_i;
        dest_int->coreotn.framer_stgN.rfrm.comb_dtim_i  |= src_int->coreotn.framer_stgN.rfrm.comb_dtim_i;
        dest_int->coreotn.framer_stgN.rfrm.comb_ddeg_i  |= src_int->coreotn.framer_stgN.rfrm.comb_ddeg_i;
        dest_int->coreotn.framer_stgN.rfrm.comb_dbdi_i  |= src_int->coreotn.framer_stgN.rfrm.comb_dbdi_i;
        dest_int->coreotn.framer_stgN.rfrm.comb_dltc_i  |= src_int->coreotn.framer_stgN.rfrm.comb_dltc_i;
        dest_int->coreotn.framer_stgN.rfrm.comb_diae_i  |= src_int->coreotn.framer_stgN.rfrm.comb_diae_i;
        dest_int->coreotn.framer_stgN.rfrm.comb_dbiae_i |= src_int->coreotn.framer_stgN.rfrm.comb_dbiae_i;
        dest_int->coreotn.framer_stgN.rfrm.comb_oof_i   |= src_int->coreotn.framer_stgN.rfrm.comb_oof_i;
        dest_int->coreotn.framer_stgN.rfrm.comb_atsf_i  |= src_int->coreotn.framer_stgN.rfrm.comb_atsf_i;
        dest_int->coreotn.framer_stgN.rfrm.comb_atsd_i  |= src_int->coreotn.framer_stgN.rfrm.comb_atsd_i;
        dest_int->coreotn.framer_stgN.rfrm.comb_aais_i  |= src_int->coreotn.framer_stgN.rfrm.comb_aais_i;
        dest_int->coreotn.framer_stgN.rfrm.comb_abdi_i  |= src_int->coreotn.framer_stgN.rfrm.comb_abdi_i;
        dest_int->coreotn.framer_stgN.rfrm.comb_dloflom_i |= src_int->coreotn.framer_stgN.rfrm.comb_dloflom_i;
    }

    /* high order */
    for (index = 0; index < DIGI_OTN_SERVER_HO_CHNL_MAX; index++)
    {
        dest_int->otn_odu_ho_handle[index] = src_int->otn_odu_ho_handle[index];
        if (NULL == src_int->otn_odu_ho_handle[index])
        {
            continue;
        }

        rc = adpt_odu_int_merge(&src_int->otn_odu_ho_chnl[index].status,
                                &dest_int->otn_odu_ho_chnl[index].status);

        dest_int->otn_odu_ho_chnl[index].stg3b_valid = 
        src_int->otn_odu_ho_chnl[index].stg3b_valid;
        if (src_int->otn_odu_ho_chnl[index].stg3b_valid)
        {
            rc = adpt_odu_int_merge(&src_int->otn_odu_ho_chnl[index].stg3b,
                                    &dest_int->otn_odu_ho_chnl[index].stg3b);
        }
    }

    /* middle order */
    for (index = 0; index < DIGI_OTN_SERVER_MO_CHNL_MAX; index++)
    {
        dest_int->otn_odu_mo_handle[index] = src_int->otn_odu_mo_handle[index];
        if (NULL == src_int->otn_odu_mo_handle[index])
        {
            continue;
        }

        rc = adpt_odu_int_merge(&src_int->otn_odu_mo_chnl[index].status,
                                &dest_int->otn_odu_mo_chnl[index].status);

        dest_int->otn_odu_mo_chnl[index].stg3b_valid = 
        src_int->otn_odu_mo_chnl[index].stg3b_valid;
        if (src_int->otn_odu_mo_chnl[index].stg3b_valid)
        {
            rc = adpt_odu_int_merge(&src_int->otn_odu_mo_chnl[index].stg3b,
                                    &dest_int->otn_odu_mo_chnl[index].stg3b);
        }
    }

    /* low order */
    for (index = 0; index < DIGI_OTN_SERVER_LO_CHNL_MAX; index++)
    {
        dest_int->otn_odu_lo_handle[index] = src_int->otn_odu_lo_handle[index];
        if (NULL == src_int->otn_odu_lo_handle[index])
        {
            continue;
        }

        rc = adpt_odu_int_merge(&src_int->otn_odu_lo_chnl[index].status,
                                &dest_int->otn_odu_lo_chnl[index].status);

        dest_int->otn_odu_lo_chnl[index].stg3b_valid = 
        src_int->otn_odu_lo_chnl[index].stg3b_valid;
        if (src_int->otn_odu_lo_chnl[index].stg3b_valid)
        {
            rc = adpt_odu_int_merge(&src_int->otn_odu_lo_chnl[index].stg3b,
                                    &dest_int->otn_odu_lo_chnl[index].stg3b);
        }
    }

    /* SYSOTN Line TCM Monitoring */
    for (index = 0; index < DIGI_OTN_SERVER_LO_CHNL_MAX; index++)
    {
        dest_int->otn_odu_lo_mon_handle[index] = src_int->otn_odu_lo_mon_handle[index];
        if (NULL == src_int->otn_odu_lo_mon_handle[index])
        {
            continue;
        }

        rc = adpt_odu_int_merge(&src_int->otn_odu_lo_mon_chnl[index].status,
                                &dest_int->otn_odu_lo_mon_chnl[index].status);

        dest_int->otn_odu_lo_mon_chnl[index].stg3b_valid = 
        src_int->otn_odu_lo_mon_chnl[index].stg3b_valid;
        if (src_int->otn_odu_lo_mon_chnl[index].stg3b_valid)
        {
            rc = adpt_odu_int_merge(&src_int->otn_odu_lo_mon_chnl[index].stg3b,
                                    &dest_int->otn_odu_lo_mon_chnl[index].stg3b);
        }
    }

    /* OHP */
    for (index = 0; index < COREOTN_NUM_ODU_OHP_INST; index++)
    {
        rc = adpt_ohp_int_merge(&src_int->ohp[index], &dest_int->ohp[index]);
    }

    /* DI RCP */
    rc = adpt_rcp_int_merge(&src_int->di_rcp, &dest_int->di_rcp);

    /* RI RCP */
    rc = adpt_rcp_int_merge(&src_int->ri_rcp, &dest_int->ri_rcp);

    PMC_RETURN(rc);
} /* adpt_otn_int_merge */


/*******************************************************************************
* adpt_mapper_int_merge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Merges mapper defect and interrupt data from shared memory to Adapter. 
*
* INPUTS:
*   *src_int           - shared memory mapper defect and int data.
*   *dest_int          - Adapter mapper defect and int data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_mapper_int_merge(digi_mapper_int_t *src_int,
                                     digi_mapper_int_t *dest_int)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 i = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(src_int);
    ADPT_PARA_VERIFY(dest_int);

    /* MAPOTN */
    for (index = 0; index < DIGI_MAPOTN_CHNL_MAX; index++)
    {
        dest_int->handle[index] = src_int->handle[index];
        if (NULL == src_int->handle[index])
        {
            continue;
        }

        /* MAPOTN interrupt */
        dest_int->mapper_chnl[index].status.rgmp_valid = 
        src_int->mapper_chnl[index].status.rgmp_valid;
        dest_int->mapper_chnl[index].status.rgmp.dplm_i |= 
        src_int->mapper_chnl[index].status.rgmp.dplm_i;
        dest_int->mapper_chnl[index].status.rgmp.csf_i |= 
        src_int->mapper_chnl[index].status.rgmp.csf_i;
        dest_int->mapper_chnl[index].status.rgmp.cm_out_limit_i |= 
        src_int->mapper_chnl[index].status.rgmp.cm_out_limit_i;
        dest_int->mapper_chnl[index].status.rgmp.cnd_max_i |= 
        src_int->mapper_chnl[index].status.rgmp.cnd_max_i;
        dest_int->mapper_chnl[index].status.rgmp.jc_err_syncloss_i |= 
        src_int->mapper_chnl[index].status.rgmp.jc_err_syncloss_i;
        dest_int->mapper_chnl[index].status.rgmp.cm_cnd_crc_err_i |= 
        src_int->mapper_chnl[index].status.rgmp.cm_cnd_crc_err_i;
        dest_int->mapper_chnl[index].status.rgmp.cm_cnd_crc_err_i |= 
        src_int->mapper_chnl[index].status.rgmp.cm_cnd_crc_err_i;
        dest_int->mapper_chnl[index].status.rgmp.pserv_late_i |= 
        src_int->mapper_chnl[index].status.rgmp.pserv_late_i;
        dest_int->mapper_chnl[index].status.rgmp.ssf_sf_rcvd_i |= 
        src_int->mapper_chnl[index].status.rgmp.ssf_sf_rcvd_i;
        dest_int->mapper_chnl[index].status.rgmp.dplm_v = 
        src_int->mapper_chnl[index].status.rgmp.dplm_v;
        dest_int->mapper_chnl[index].status.rgmp.csf_v = 
        src_int->mapper_chnl[index].status.rgmp.csf_v;
        dest_int->mapper_chnl[index].status.rgmp.ssf_sf_rcvd_v = 
        src_int->mapper_chnl[index].status.rgmp.ssf_sf_rcvd_v;

        dest_int->mapper_chnl[index].status.lofd_i |= 
        src_int->mapper_chnl[index].status.lofd_i;
        dest_int->mapper_chnl[index].status.idle_i |= 
        src_int->mapper_chnl[index].status.idle_i;
        dest_int->mapper_chnl[index].status.ctrlfrm_i |= 
        src_int->mapper_chnl[index].status.ctrlfrm_i;
        dest_int->mapper_chnl[index].status.unpack_fifo_drop_i |= 
        src_int->mapper_chnl[index].status.unpack_fifo_drop_i;
        dest_int->mapper_chnl[index].status.uchec_i |= 
        src_int->mapper_chnl[index].status.uchec_i;
        dest_int->mapper_chnl[index].status.cchec_i |= 
        src_int->mapper_chnl[index].status.cchec_i;
        dest_int->mapper_chnl[index].status.uthec_i |= 
        src_int->mapper_chnl[index].status.uthec_i;
        dest_int->mapper_chnl[index].status.cthec_i |= 
        src_int->mapper_chnl[index].status.uehec_i;
        dest_int->mapper_chnl[index].status.cehec_i |= 
        src_int->mapper_chnl[index].status.cehec_i;
        dest_int->mapper_chnl[index].status.exi_i |= 
        src_int->mapper_chnl[index].status.exi_i;
        dest_int->mapper_chnl[index].status.pti_upi_msmch_i |= 
        src_int->mapper_chnl[index].status.pti_upi_msmch_i;
        dest_int->mapper_chnl[index].status.pti_upi_msmch_i |= 
        src_int->mapper_chnl[index].status.pti_upi_msmch_i;
        dest_int->mapper_chnl[index].status.csf_i |= 
        src_int->mapper_chnl[index].status.csf_i;
        dest_int->mapper_chnl[index].status.drop_i |= 
        src_int->mapper_chnl[index].status.drop_i;
        dest_int->mapper_chnl[index].status.datafrm_i |= 
        src_int->mapper_chnl[index].status.datafrm_i;
        dest_int->mapper_chnl[index].status.pause_i |= 
        src_int->mapper_chnl[index].status.pause_i;
        dest_int->mapper_chnl[index].status.max_frm_len_i |= 
        src_int->mapper_chnl[index].status.max_frm_len_i;
        dest_int->mapper_chnl[index].status.min_frm_len_i |= 
        src_int->mapper_chnl[index].status.min_frm_len_i;
        for (i = 0; i < 7; i++)
        {
            dest_int->mapper_chnl[index].status.frm_len_stat_intr_i[i] |= 
            src_int->mapper_chnl[index].status.frm_len_stat_intr_i[i];
        }
        dest_int->mapper_chnl[index].status.pfcse_i |= 
        src_int->mapper_chnl[index].status.pfcse_i;
        dest_int->mapper_chnl[index].status.dwnstrm_frm_i |= 
        src_int->mapper_chnl[index].status.dwnstrm_frm_i;
        dest_int->mapper_chnl[index].status.ssf_sf_rcvd_i |= 
        src_int->mapper_chnl[index].status.ssf_sf_rcvd_i;
        dest_int->mapper_chnl[index].status.hao_status_avl_i |= 
        src_int->mapper_chnl[index].status.hao_status_avl_i;
        dest_int->mapper_chnl[index].status.dplm_i |= 
        src_int->mapper_chnl[index].status.dplm_i;
        dest_int->mapper_chnl[index].status.opu_csf_i |= 
        src_int->mapper_chnl[index].status.opu_csf_i;
        dest_int->mapper_chnl[index].status.len_err_int_i |= 
        src_int->mapper_chnl[index].status.len_err_int_i;
        dest_int->mapper_chnl[index].status.pause_frame_tx_int_i |= 
        src_int->mapper_chnl[index].status.pause_frame_tx_int_i;
        dest_int->mapper_chnl[index].status.mgmt_frame_tx_int_i |= 
        src_int->mapper_chnl[index].status.mgmt_frame_tx_int_i;
        dest_int->mapper_chnl[index].status.oversize_frm_rx_int_i |= 
        src_int->mapper_chnl[index].status.oversize_frm_rx_int_i;
        dest_int->mapper_chnl[index].status.undersize_frm_rx_int_i |= 
        src_int->mapper_chnl[index].status.undersize_frm_rx_int_i;
        dest_int->mapper_chnl[index].status.ord_set_len_err_int_i |= 
        src_int->mapper_chnl[index].status.ord_set_len_err_int_i;

        dest_int->mapper_chnl[index].status.lofd_v = 
        src_int->mapper_chnl[index].status.lofd_v;
        dest_int->mapper_chnl[index].status.idle_v = 
        src_int->mapper_chnl[index].status.idle_v;
        dest_int->mapper_chnl[index].status.csf_v = 
        src_int->mapper_chnl[index].status.csf_v;
        dest_int->mapper_chnl[index].status.exi_v = 
        src_int->mapper_chnl[index].status.exi_v;
        dest_int->mapper_chnl[index].status.pti_upi_msmch_v = 
        src_int->mapper_chnl[index].status.pti_upi_msmch_v;
        dest_int->mapper_chnl[index].status.ssf_sf_rcvd_v = 
        src_int->mapper_chnl[index].status.ssf_sf_rcvd_v;
        dest_int->mapper_chnl[index].status.dplm_v = 
        src_int->mapper_chnl[index].status.dplm_v;
        dest_int->mapper_chnl[index].status.opu_csf_v = 
        src_int->mapper_chnl[index].status.opu_csf_v;
        dest_int->mapper_chnl[index].status.in_sfail_v = 
        src_int->mapper_chnl[index].status.in_sfail_v;

        /* ODU interrupt */
        rc = adpt_odu_int_merge(&src_int->mapper_chnl[index].fo2.status,
                                &dest_int->mapper_chnl[index].fo2.status);
        dest_int->mapper_chnl[index].fo2.stg3b_valid = 
        src_int->mapper_chnl[index].fo2.stg3b_valid;
        rc = adpt_odu_int_merge(&src_int->mapper_chnl[index].fo2.stg3b,
                                &dest_int->mapper_chnl[index].fo2.stg3b);

        /* ENET interrupt */
        dest_int->mapper_chnl[index].enet_etrans_valid =
        src_int->mapper_chnl[index].enet_etrans_valid;
        if (src_int->mapper_chnl[index].enet_etrans_valid)
        {
            rc = adpt_enet_chnl_int_merge(&src_int->mapper_chnl[index].enet_etrans,
                                          &dest_int->mapper_chnl[index].enet_etrans);
        }
        dest_int->mapper_chnl[index].enet_pmon_valid =
        src_int->mapper_chnl[index].enet_pmon_valid;
        if (src_int->mapper_chnl[index].enet_pmon_valid)
        {
            rc = adpt_enet_chnl_int_merge(&src_int->mapper_chnl[index].enet_pmon,
                                          &dest_int->mapper_chnl[index].enet_pmon);
        }
    }

    PMC_RETURN(rc);
} /* adpt_mapper_int_merge */


/*******************************************************************************
* adpt_sifd_int_merge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Merges SIFD defect and interrupt data from shared memory to Adapter. 
*
* INPUTS:
*   *src_int           - shared memory SIFD defect and int data.
*   *dest_int          - Adapter SIFD defect and int data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_sifd_int_merge(digi_sifd_int_t *src_int,
                                  digi_sifd_int_t *dest_int)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(src_int);
    ADPT_PARA_VERIFY(dest_int);

    /* SIFD */
    for (index = 0; index < DIGI_SIFD_ILKN_NUM; index++)
    {
        dest_int->handle[index] = src_int->handle[index];
        if (NULL == src_int->handle[index])
        {
            continue;
        }

        /* SIFD interrupt */
        dest_int->sifd_link[index].fifo_over_i |=
        src_int->sifd_link[index].fifo_over_i;
        dest_int->sifd_link[index].fifo_under_i |=
        src_int->sifd_link[index].fifo_under_i;
        dest_int->sifd_link[index].oob_fc_err_i |=
        src_int->sifd_link[index].oob_fc_err_i;
        dest_int->sifd_link[index].tx_status_msg_change_i |=
        src_int->sifd_link[index].tx_status_msg_change_i;
        dest_int->sifd_link[index].tx_fc_cnt_fill_i |=
        src_int->sifd_link[index].tx_fc_cnt_fill_i;
        dest_int->sifd_link[index].tx_chan_cnt_fill_i |=
        src_int->sifd_link[index].tx_chan_cnt_fill_i;
        dest_int->sifd_link[index].ctlbit_err_i |=
        src_int->sifd_link[index].ctlbit_err_i;
        dest_int->sifd_link[index].mf_err_i |=
        src_int->sifd_link[index].mf_err_i;
        dest_int->sifd_link[index].badctl_err_i |=
        src_int->sifd_link[index].badctl_err_i;
        dest_int->sifd_link[index].scram_err_i |=
        src_int->sifd_link[index].scram_err_i;
        dest_int->sifd_link[index].crc32_err_i |=
        src_int->sifd_link[index].crc32_err_i;
        dest_int->sifd_link[index].cdr_err_i |=
        src_int->sifd_link[index].cdr_err_i;
        dest_int->sifd_link[index].wrd_sync_change_i |=
        src_int->sifd_link[index].wrd_sync_change_i;
        dest_int->sifd_link[index].align_change_i |=
        src_int->sifd_link[index].align_change_i;
        dest_int->sifd_link[index].align_err_i |=
        src_int->sifd_link[index].align_err_i;
        dest_int->sifd_link[index].burst_err_i |=
        src_int->sifd_link[index].burst_err_i;
        dest_int->sifd_link[index].bmin_err_i |=
        src_int->sifd_link[index].bmin_err_i;
        dest_int->sifd_link[index].bmax_err_i |=
        src_int->sifd_link[index].bmax_err_i;
        dest_int->sifd_link[index].crc24_err_i |=
        src_int->sifd_link[index].crc24_err_i;
        dest_int->sifd_link[index].ib_fc_err_i |=
        src_int->sifd_link[index].ib_fc_err_i;
        dest_int->sifd_link[index].rx_status_msg_change_i |=
        src_int->sifd_link[index].rx_status_msg_change_i;
        dest_int->sifd_link[index].badctl_cnt_fill_i |=
        src_int->sifd_link[index].badctl_cnt_fill_i;
        dest_int->sifd_link[index].crc32_cnt_fill_i |=
        src_int->sifd_link[index].crc32_cnt_fill_i;
        dest_int->sifd_link[index].cdr_cnt_fill_i |=
        src_int->sifd_link[index].cdr_cnt_fill_i;
        dest_int->sifd_link[index].wrd_sync_cnt_fill_i |=
        src_int->sifd_link[index].wrd_sync_cnt_fill_i;
        dest_int->sifd_link[index].align_fail_cnt_fill_i |=
        src_int->sifd_link[index].align_fail_cnt_fill_i;
        dest_int->sifd_link[index].align_cnt_fill_i |=
        src_int->sifd_link[index].align_cnt_fill_i;
        dest_int->sifd_link[index].burst_cnt_fill_i |=
        src_int->sifd_link[index].burst_cnt_fill_i;
        dest_int->sifd_link[index].bmin_cnt_fill_i |=
        src_int->sifd_link[index].bmin_cnt_fill_i;
        dest_int->sifd_link[index].bmax_cnt_fill_i |=
        src_int->sifd_link[index].bmax_cnt_fill_i;
        dest_int->sifd_link[index].crc24_cnt_fill_i |=
        src_int->sifd_link[index].crc24_cnt_fill_i;
        dest_int->sifd_link[index].rx_fc_cnt_fill_i |=
        src_int->sifd_link[index].rx_fc_cnt_fill_i;
        dest_int->sifd_link[index].rx_chan_cnt_fill_i |=
        src_int->sifd_link[index].rx_chan_cnt_fill_i;

        dest_int->sifd_link[index].cpb_tx_buf_un_int_i0 |=
        src_int->sifd_link[index].cpb_tx_buf_un_int_i0;
        dest_int->sifd_link[index].cpb_tx_buf_align_int_i0 |=
        src_int->sifd_link[index].cpb_tx_buf_align_int_i0;
        dest_int->sifd_link[index].oduk_tx_buf_un_int_i0 |=
        src_int->sifd_link[index].oduk_tx_buf_un_int_i0;
        dest_int->sifd_link[index].oduk_tx_buf_align_int_i0 |=
        src_int->sifd_link[index].oduk_tx_buf_align_int_i0;
        dest_int->sifd_link[index].rx_buf_un_int_i0 |=
        src_int->sifd_link[index].rx_buf_un_int_i0;
        dest_int->sifd_link[index].rx_buf_align_int_i0 |=
        src_int->sifd_link[index].rx_buf_align_int_i0;
        dest_int->sifd_link[index].ser_rx_los_int_i0 |=
        src_int->sifd_link[index].ser_rx_los_int_i0;
        dest_int->sifd_link[index].rx_sob_err_int_i0 |=
        src_int->sifd_link[index].rx_sob_err_int_i0;
        dest_int->sifd_link[index].rx_burst_len_err_int_i0 |=
        src_int->sifd_link[index].rx_burst_len_err_int_i0;
        dest_int->sifd_link[index].rx_burst_drop_err_int_i0 |=
        src_int->sifd_link[index].rx_burst_drop_err_int_i0;
        dest_int->sifd_link[index].rx_pkt_ch_change_err_int_i0 |=
        src_int->sifd_link[index].rx_pkt_ch_change_err_int_i0;
        dest_int->sifd_link[index].rx_eob_err_int_i0 |=
        src_int->sifd_link[index].rx_eob_err_int_i0;
        dest_int->sifd_link[index].rx_sop_err_int_i0 |=
        src_int->sifd_link[index].rx_sop_err_int_i0;
        dest_int->sifd_link[index].rx_eop_err_int_i0 |=
        src_int->sifd_link[index].rx_eop_err_int_i0;
        dest_int->sifd_link[index].rx_il_err_all_int_i0 |=
        src_int->sifd_link[index].rx_il_err_all_int_i0;
        dest_int->sifd_link[index].rx_il_align_int_i0 |=
        src_int->sifd_link[index].rx_il_align_int_i0;
        dest_int->sifd_link[index].rx_il_dat_err_int_i0 |=
        src_int->sifd_link[index].rx_il_dat_err_int_i0;
        dest_int->sifd_link[index].rx_oduk_h2b_cam_err_int_i0 |=
        src_int->sifd_link[index].rx_oduk_h2b_cam_err_int_i0;
        dest_int->sifd_link[index].rx_oduk_h2b_cam_mmatch_err_int_i0 |=
        src_int->sifd_link[index].rx_oduk_h2b_cam_mmatch_err_int_i0;
        dest_int->sifd_link[index].rx_oduk_h2b_cmpr_err_int_i0 |=
        src_int->sifd_link[index].rx_oduk_h2b_cmpr_err_int_i0;
        dest_int->sifd_link[index].rx_oduk_h4b_cmpr_err_int_i0 |=
        src_int->sifd_link[index].rx_oduk_h4b_cmpr_err_int_i0;

        /* SIFD status */
        dest_int->sifd_link[index].rx_mf_err =
        src_int->sifd_link[index].rx_mf_err;
        dest_int->sifd_link[index].rx_wrd_sync_lane =
        src_int->sifd_link[index].rx_wrd_sync_lane;
        dest_int->sifd_link[index].rx_wrd_sync_cnt_fill_lane =
        src_int->sifd_link[index].rx_wrd_sync_cnt_fill_lane;
        dest_int->sifd_link[index].rx_crc32_err =
        src_int->sifd_link[index].rx_crc32_err;
        dest_int->sifd_link[index].rc_crc32_cnt_fill_lane =
        src_int->sifd_link[index].rc_crc32_cnt_fill_lane;
        dest_int->sifd_link[index].rx_badctl_err =
        src_int->sifd_link[index].rx_badctl_err;
        dest_int->sifd_link[index].rx_scram_err =
        src_int->sifd_link[index].rx_scram_err;
        dest_int->sifd_link[index].rx_cdr_err =
        src_int->sifd_link[index].rx_cdr_err;
        dest_int->sifd_link[index].rx_crd_cnt_fill_lane =
        src_int->sifd_link[index].rx_crd_cnt_fill_lane;
        dest_int->sifd_link[index].rx_wrd_sync_chg =
        src_int->sifd_link[index].rx_wrd_sync_chg;
        dest_int->sifd_link[index].rx_align_good =
        src_int->sifd_link[index].rx_align_good;
        dest_int->sifd_link[index].rx_align_err =
        src_int->sifd_link[index].rx_align_err;

        /* tx_stat_message_rxd */
        dest_int->sifd_link[index].current =
        src_int->sifd_link[index].current;
        dest_int->sifd_link[index].valid =
        src_int->sifd_link[index].valid;
        dest_int->sifd_link[index].sync =
        src_int->sifd_link[index].sync;
        dest_int->sifd_link[index].sif =
        src_int->sifd_link[index].sif;
        dest_int->sifd_link[index].tx_sl =
        src_int->sifd_link[index].tx_sl;

        /* stat_message_rxd */
        dest_int->sifd_link[index].rx_status_msg_valid_all =
        src_int->sifd_link[index].rx_status_msg_valid_all;
        dest_int->sifd_link[index].rx_status_msg_valid_any =
        src_int->sifd_link[index].rx_status_msg_valid_any;
        dest_int->sifd_link[index].rx_status_msg_sync =
        src_int->sifd_link[index].rx_status_msg_sync;
        dest_int->sifd_link[index].sif_all =
        src_int->sifd_link[index].sif_all;
        dest_int->sifd_link[index].rx_sl =   
        src_int->sifd_link[index].rx_sl;    
        dest_int->sifd_link[index].sif_any = 
        src_int->sifd_link[index].sif_any;  
    }


    PMC_RETURN(rc);
} /* adpt_sifd_int_merge */


/*******************************************************************************
* adpt_cbr_int_merge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Merges CBR defect and interrupt data from shared memory to Adapter. 
*
* INPUTS:
*   *src_int           - shared memory CBR defect and int data.
*   *dest_int          - Adapter CBR defect and int data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cbr_int_merge(digi_cbr_int_t *src_int,
                                 digi_cbr_int_t *dest_int)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(src_int);
    ADPT_PARA_VERIFY(dest_int);

    /* SIFD */
    for (index = 0; index < DIGI_CBRC_CHNL_MAX; index++)
    {
        dest_int->handle[index] = src_int->handle[index];
        if (NULL == src_int->handle[index])
        {
            continue;
        }

        /* CBR general interrupt */
        dest_int->cbrc_chnl[index].cbrc.cbr_dint_fifo_uf_int_i |=
        src_int->cbrc_chnl[index].cbrc.cbr_dint_fifo_uf_int_i;
        dest_int->cbrc_chnl[index].cbrc.cbr_dint_fifo_ovf_int_i |=
        src_int->cbrc_chnl[index].cbrc.cbr_dint_fifo_ovf_int_i;
        dest_int->cbrc_chnl[index].cbrc.cbr_dint_stag_buf_ovf_int_i |=
        src_int->cbrc_chnl[index].cbrc.cbr_dint_stag_buf_ovf_int_i;
        dest_int->cbrc_chnl[index].cbrc.pkt_dint_cfc_fifo_ovr_i |=
        src_int->cbrc_chnl[index].cbrc.pkt_dint_cfc_fifo_ovr_i;
        dest_int->cbrc_chnl[index].cbrc.pkt_dint_cfc_fifo_udr_i |=
        src_int->cbrc_chnl[index].cbrc.pkt_dint_cfc_fifo_udr_i;
        dest_int->cbrc_chnl[index].cbrc.pkt_int_cfc_fifo_ovr_i |=
        src_int->cbrc_chnl[index].cbrc.pkt_int_cfc_fifo_ovr_i;
        dest_int->cbrc_chnl[index].cbrc.pkt_int_cfc_fifo_udr_i |=
        src_int->cbrc_chnl[index].cbrc.pkt_int_cfc_fifo_udr_i;

        dest_int->cbrc_chnl[index].cbrc.cbr_dint_fifo_uf_int_v =
        src_int->cbrc_chnl[index].cbrc.cbr_dint_fifo_uf_int_v;
        dest_int->cbrc_chnl[index].cbrc.cbr_dint_fifo_ovf_int_v =
        src_int->cbrc_chnl[index].cbrc.cbr_dint_fifo_ovf_int_v;
        dest_int->cbrc_chnl[index].cbrc.cbr_dint_stag_buf_ovf_int_v =
        src_int->cbrc_chnl[index].cbrc.cbr_dint_stag_buf_ovf_int_v;
        dest_int->cbrc_chnl[index].cbrc.pkt_dint_cfc_fifo_ovr_v =
        src_int->cbrc_chnl[index].cbrc.pkt_dint_cfc_fifo_ovr_v;
        dest_int->cbrc_chnl[index].cbrc.pkt_dint_cfc_fifo_udr_v =
        src_int->cbrc_chnl[index].cbrc.pkt_dint_cfc_fifo_udr_v;
        dest_int->cbrc_chnl[index].cbrc.pkt_int_cfc_fifo_ovr_v =
        src_int->cbrc_chnl[index].cbrc.pkt_int_cfc_fifo_ovr_v;
        dest_int->cbrc_chnl[index].cbrc.pkt_int_cfc_fifo_udr_v =
        src_int->cbrc_chnl[index].cbrc.pkt_int_cfc_fifo_udr_v;

        /* SDH PMG */
        dest_int->cbrc_chnl[index].cbrc.sdh_pmg_valid =
        src_int->cbrc_chnl[index].cbrc.sdh_pmg_valid;
        if (dest_int->cbrc_chnl[index].cbrc.sdh_pmg_valid)
        {
            /* rx direction */
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.cok2_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.cok2_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.lreie_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.lreie_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.lbipe_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.lbipe_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.sbipe_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.sbipe_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.cossm_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.cossm_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.coaps_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.coaps_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.apsbf_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.apsbf_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.lrdi_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.lrdi_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.lais_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.lais_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.los_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.los_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.lof_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.lof_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.oof_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.oof_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.tim_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.tim_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.tiu_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.tiu_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.sf_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.sf_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.sd_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.sd_i;

            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.apsbf_v =
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.apsbf_v;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.lrdi_v =
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.lrdi_v;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.lais_v =
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.lais_v;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.los_v =
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.los_v;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.lof_v =
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.lof_v;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.oof_v =
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.oof_v;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.tim_v =
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.tim_v;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.tiu_v =
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.tiu_v;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.sf_v =
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.sf_v;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.sd_v =
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_rx.sd_v;

            /* tx direction */
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.cok2_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.cok2_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.lreie_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.lreie_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.lbipe_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.lbipe_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.sbipe_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.sbipe_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.cossm_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.cossm_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.coaps_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.coaps_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.apsbf_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.apsbf_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.lrdi_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.lrdi_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.lais_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.lais_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.los_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.los_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.lof_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.lof_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.oof_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.oof_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.tim_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.tim_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.tiu_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.tiu_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.sf_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.sf_i;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.sd_i |=
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.sd_i;

            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.apsbf_v =
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.apsbf_v;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.lrdi_v =
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.lrdi_v;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.lais_v =
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.lais_v;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.los_v =
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.los_v;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.lof_v =
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.lof_v;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.oof_v =
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.oof_v;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.tim_v =
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.tim_v;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.tiu_v =
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.tiu_v;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.sf_v =
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.sf_v;
            dest_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.sd_v =
            src_int->cbrc_chnl[index].cbrc.sdh_pmg_tx.sd_v;
        }

        /* FC800 PMG */
        dest_int->cbrc_chnl[index].cbrc.fc800_pmg_valid =
        src_int->cbrc_chnl[index].cbrc.fc800_pmg_valid;
        if (dest_int->cbrc_chnl[index].cbrc.fc800_pmg_valid)
        {
            /* rx direction */
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_disp_invert_i |=
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_disp_invert_i;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_xdet_i |=
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_xdet_i;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_sof_det_i |=
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_sof_det_i;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_dlolb_i |=
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_dlolb_i;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_asd_i |=
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_asd_i;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_sync_i |=
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_sync_i;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_lcv_i |=
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_lcv_i;

            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_disp_invert_v =
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_disp_invert_v;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_xdet_v =
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_xdet_v;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_dlolb_v =
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_dlolb_v;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_asd_v =
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_asd_v;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_sync_v =
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_sync_v;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_lcv_v =
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_rx.rx_lcv_v;

            /* rx direction */
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_disp_invert_i |=
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_disp_invert_i;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_xdet_i |=
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_xdet_i;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_sof_det_i |=
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_sof_det_i;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_dlolb_i |=
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_dlolb_i;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_asd_i |=
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_asd_i;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_sync_i |=
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_sync_i;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_lcv_i |=
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_lcv_i;

            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_disp_invert_v =
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_disp_invert_v;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_xdet_v =
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_xdet_v;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_dlolb_v =
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_dlolb_v;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_asd_v =
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_asd_v;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_sync_v =
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_sync_v;
            dest_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_lcv_v =
            src_int->cbrc_chnl[index].cbrc.fc800_pmg_tx.rx_lcv_v;
        }

        /* CBR FSGM */
        dest_int->cbrc_chnl[index].cbrc.cbr_fsgm_valid =
        src_int->cbrc_chnl[index].cbrc.cbr_fsgm_valid;
        if (dest_int->cbrc_chnl[index].cbrc.cbr_fsgm_valid)
        {
            /* rx direction */
            dest_int->cbrc_chnl[index].cbrc.cbr_fsgm_rx.pn11_det_i |=
            src_int->cbrc_chnl[index].cbrc.cbr_fsgm_rx.pn11_det_i;
            dest_int->cbrc_chnl[index].cbrc.cbr_fsgm_rx.rx_pn11_sync_i |=
            src_int->cbrc_chnl[index].cbrc.cbr_fsgm_rx.rx_pn11_sync_i;
            dest_int->cbrc_chnl[index].cbrc.cbr_fsgm_rx.rx_pn11_err_i |=
            src_int->cbrc_chnl[index].cbrc.cbr_fsgm_rx.rx_pn11_err_i;
            dest_int->cbrc_chnl[index].cbrc.cbr_fsgm_rx.rx_prbs31_err_i |=
            src_int->cbrc_chnl[index].cbrc.cbr_fsgm_rx.rx_prbs31_err_i;
            dest_int->cbrc_chnl[index].cbrc.cbr_fsgm_rx.rx_prbs31_all_ones_or_zeros_i |=
            src_int->cbrc_chnl[index].cbrc.cbr_fsgm_rx.rx_prbs31_all_ones_or_zeros_i;

            dest_int->cbrc_chnl[index].cbrc.cbr_fsgm_rx.pn11_det_v =
            src_int->cbrc_chnl[index].cbrc.cbr_fsgm_rx.pn11_det_v;
            dest_int->cbrc_chnl[index].cbrc.cbr_fsgm_rx.rx_pn11_sync_v =
            src_int->cbrc_chnl[index].cbrc.cbr_fsgm_rx.rx_pn11_sync_v;
            dest_int->cbrc_chnl[index].cbrc.cbr_fsgm_rx.rx_prbs31_all_ones_or_zeros_v =
            src_int->cbrc_chnl[index].cbrc.cbr_fsgm_rx.rx_prbs31_all_ones_or_zeros_v;

            /* tx direction */
            dest_int->cbrc_chnl[index].cbrc.cbr_fsgm_tx.pn11_det_i |=
            src_int->cbrc_chnl[index].cbrc.cbr_fsgm_tx.pn11_det_i;
            dest_int->cbrc_chnl[index].cbrc.cbr_fsgm_tx.rx_pn11_sync_i |=
            src_int->cbrc_chnl[index].cbrc.cbr_fsgm_tx.rx_pn11_sync_i;
            dest_int->cbrc_chnl[index].cbrc.cbr_fsgm_tx.rx_pn11_err_i |=
            src_int->cbrc_chnl[index].cbrc.cbr_fsgm_tx.rx_pn11_err_i;
            dest_int->cbrc_chnl[index].cbrc.cbr_fsgm_tx.rx_prbs31_err_i |=
            src_int->cbrc_chnl[index].cbrc.cbr_fsgm_tx.rx_prbs31_err_i;
            dest_int->cbrc_chnl[index].cbrc.cbr_fsgm_tx.rx_prbs31_all_ones_or_zeros_i |=
            src_int->cbrc_chnl[index].cbrc.cbr_fsgm_tx.rx_prbs31_all_ones_or_zeros_i;

            dest_int->cbrc_chnl[index].cbrc.cbr_fsgm_tx.pn11_det_v =
            src_int->cbrc_chnl[index].cbrc.cbr_fsgm_tx.pn11_det_v;
            dest_int->cbrc_chnl[index].cbrc.cbr_fsgm_tx.rx_pn11_sync_v =
            src_int->cbrc_chnl[index].cbrc.cbr_fsgm_tx.rx_pn11_sync_v;
            dest_int->cbrc_chnl[index].cbrc.cbr_fsgm_tx.rx_prbs31_all_ones_or_zeros_v =
            src_int->cbrc_chnl[index].cbrc.cbr_fsgm_tx.rx_prbs31_all_ones_or_zeros_v;
        }

        /* FC1200 (PMON, PCS, TTTD, TTTM) */
        dest_int->cbrc_chnl[index].cbrc.fc1200_valid =
        src_int->cbrc_chnl[index].cbrc.fc1200_valid;
        if (dest_int->cbrc_chnl[index].cbrc.fc1200_valid)
        {
            /* rx direction */
            dest_int->cbrc_chnl[index].cbrc.fc1200_pmon_rx.high_ber_int_i |=
            src_int->cbrc_chnl[index].cbrc.fc1200_pmon_rx.high_ber_int_i;
            dest_int->cbrc_chnl[index].cbrc.fc1200_pmon_rx.loss_sync_int_i |=
            src_int->cbrc_chnl[index].cbrc.fc1200_pmon_rx.loss_sync_int_i;
            dest_int->cbrc_chnl[index].cbrc.fc1200_pmon_rx.los_int_i |=
            src_int->cbrc_chnl[index].cbrc.fc1200_pmon_rx.los_int_i;

            dest_int->cbrc_chnl[index].cbrc.fc1200_pmon_rx.high_ber_int_v =
            src_int->cbrc_chnl[index].cbrc.fc1200_pmon_rx.high_ber_int_v;
            dest_int->cbrc_chnl[index].cbrc.fc1200_pmon_rx.loss_sync_int_v =
            src_int->cbrc_chnl[index].cbrc.fc1200_pmon_rx.loss_sync_int_v;
            dest_int->cbrc_chnl[index].cbrc.fc1200_pmon_rx.los_int_v =
            src_int->cbrc_chnl[index].cbrc.fc1200_pmon_rx.los_int_v;

            /* tx direction */
            dest_int->cbrc_chnl[index].cbrc.fc1200_pmon_tx.high_ber_int_i |=
            src_int->cbrc_chnl[index].cbrc.fc1200_pmon_tx.high_ber_int_i;
            dest_int->cbrc_chnl[index].cbrc.fc1200_pmon_tx.loss_sync_int_i |=
            src_int->cbrc_chnl[index].cbrc.fc1200_pmon_tx.loss_sync_int_i;
            dest_int->cbrc_chnl[index].cbrc.fc1200_pmon_tx.los_int_i |=
            src_int->cbrc_chnl[index].cbrc.fc1200_pmon_tx.los_int_i;

            dest_int->cbrc_chnl[index].cbrc.fc1200_pmon_tx.high_ber_int_v =
            src_int->cbrc_chnl[index].cbrc.fc1200_pmon_tx.high_ber_int_v;
            dest_int->cbrc_chnl[index].cbrc.fc1200_pmon_tx.loss_sync_int_v =
            src_int->cbrc_chnl[index].cbrc.fc1200_pmon_tx.loss_sync_int_v;
            dest_int->cbrc_chnl[index].cbrc.fc1200_pmon_tx.los_int_v =
            src_int->cbrc_chnl[index].cbrc.fc1200_pmon_tx.los_int_v;
        }

        dest_int->cbrc_chnl[index].cbrc.fc1200_pcs_rx_los_i |=
        src_int->cbrc_chnl[index].cbrc.fc1200_pcs_rx_los_i;
        dest_int->cbrc_chnl[index].cbrc.fc1200_pcs_rx_loss_sync_i |=
        src_int->cbrc_chnl[index].cbrc.fc1200_pcs_rx_loss_sync_i;
        dest_int->cbrc_chnl[index].cbrc.fc1200_pcs_rx_high_ber_i |=
        src_int->cbrc_chnl[index].cbrc.fc1200_pcs_rx_high_ber_i;
        dest_int->cbrc_chnl[index].cbrc.fc1200_pcs_rx_lf_i |=
        src_int->cbrc_chnl[index].cbrc.fc1200_pcs_rx_lf_i;
        dest_int->cbrc_chnl[index].cbrc.fc1200_pcs_rx_rf_i |=
        src_int->cbrc_chnl[index].cbrc.fc1200_pcs_rx_rf_i;
        dest_int->cbrc_chnl[index].cbrc.fc1200_pcs_tx_local_fault_i |=
        src_int->cbrc_chnl[index].cbrc.fc1200_pcs_tx_local_fault_i;
        dest_int->cbrc_chnl[index].cbrc.fc1200_pcs_tx_buf_underrun_i |=
        src_int->cbrc_chnl[index].cbrc.fc1200_pcs_tx_buf_underrun_i;
        dest_int->cbrc_chnl[index].cbrc.fc_tttd_crc24err_i |=
        src_int->cbrc_chnl[index].cbrc.fc_tttd_crc24err_i;
        dest_int->cbrc_chnl[index].cbrc.fc_tttd_underrun_i |=
        src_int->cbrc_chnl[index].cbrc.fc_tttd_underrun_i;
        dest_int->cbrc_chnl[index].cbrc.fc_tttd_invalblkhdr_i |=
        src_int->cbrc_chnl[index].cbrc.fc_tttd_invalblkhdr_i;
        dest_int->cbrc_chnl[index].cbrc.fc_tttm_link_fail_i |=
        src_int->cbrc_chnl[index].cbrc.fc_tttm_link_fail_i;
        dest_int->cbrc_chnl[index].cbrc.fc_tttm_hiber_i |=
        src_int->cbrc_chnl[index].cbrc.fc_tttm_hiber_i;
        dest_int->cbrc_chnl[index].cbrc.fc_tttm_losync_i |=
        src_int->cbrc_chnl[index].cbrc.fc_tttm_losync_i;
        dest_int->cbrc_chnl[index].cbrc.fc_tttm_badblk_i |=
        src_int->cbrc_chnl[index].cbrc.fc_tttm_badblk_i;
        dest_int->cbrc_chnl[index].cbrc.fc_tttm_fovr_i |=
        src_int->cbrc_chnl[index].cbrc.fc_tttm_fovr_i;
        dest_int->cbrc_chnl[index].cbrc.fc_tttm_dpi_i |=
        src_int->cbrc_chnl[index].cbrc.fc_tttm_dpi_i;

        dest_int->cbrc_chnl[index].cbrc.fc1200_pcs_rx_los_v =
        src_int->cbrc_chnl[index].cbrc.fc1200_pcs_rx_los_v;
        dest_int->cbrc_chnl[index].cbrc.fc1200_pcs_rx_loss_sync_v =
        src_int->cbrc_chnl[index].cbrc.fc1200_pcs_rx_loss_sync_v;
        dest_int->cbrc_chnl[index].cbrc.fc1200_pcs_rx_high_ber_v =
        src_int->cbrc_chnl[index].cbrc.fc1200_pcs_rx_high_ber_v;
        dest_int->cbrc_chnl[index].cbrc.fc1200_pcs_rx_lf_v =
        src_int->cbrc_chnl[index].cbrc.fc1200_pcs_rx_lf_v;
        dest_int->cbrc_chnl[index].cbrc.fc1200_pcs_rx_rf_v =
        src_int->cbrc_chnl[index].cbrc.fc1200_pcs_rx_rf_v;
        dest_int->cbrc_chnl[index].cbrc.fc1200_pcs_tx_local_fault_v =
        src_int->cbrc_chnl[index].cbrc.fc1200_pcs_tx_local_fault_v;
        dest_int->cbrc_chnl[index].cbrc.fc1200_pcs_tx_buf_underrun_v =
        src_int->cbrc_chnl[index].cbrc.fc1200_pcs_tx_buf_underrun_v;
        dest_int->cbrc_chnl[index].cbrc.fc_tttm_link_fail_v =
        src_int->cbrc_chnl[index].cbrc.fc_tttm_link_fail_v;
        dest_int->cbrc_chnl[index].cbrc.fc_tttm_hiber_v =
        src_int->cbrc_chnl[index].cbrc.fc_tttm_hiber_v;
        dest_int->cbrc_chnl[index].cbrc.fc_tttm_losync_v =
        src_int->cbrc_chnl[index].cbrc.fc_tttm_losync_v;

        /* CPB */
        dest_int->cbrc_chnl[index].cpb.ch_drop_ovrsize_i |=
        src_int->cbrc_chnl[index].cpb.ch_drop_ovrsize_i;
        dest_int->cbrc_chnl[index].cpb.ch_drop_client_i |=
        src_int->cbrc_chnl[index].cpb.ch_drop_client_i;
        dest_int->cbrc_chnl[index].cpb.ch_drop_full_i |=
        src_int->cbrc_chnl[index].cpb.ch_drop_full_i;
    }

    PMC_RETURN(rc);
} /* adpt_cbr_int_merge */


/*******************************************************************************
* adpt_otu_status_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Prints OTU defect and interrupt data.
*
* INPUTS:
*   *otu_chnl          - LINEOTN/SYSOTN OTU defect and int data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otu_status_print(lineotn_int_chnl_t *otu_chnl)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 i = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(otu_chnl);

    /* OTU defect and interrupt */
    ADPT_PRINT("otu_frm_d_lom_i     = [%d] \r\n", otu_chnl->otu_frm.otu_frm_d_lom_i);
    ADPT_PRINT("otu_frm_oom_i       = [%d] \r\n", otu_chnl->otu_frm.otu_frm_oom_i);
    ADPT_PRINT("otu_sk_a_ais_i      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_a_ais_i);
    ADPT_PRINT("otu_sk_a_ssd_i      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_a_ssd_i);
    ADPT_PRINT("otu_sk_a_ssf_i      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_a_ssf_i);
    ADPT_PRINT("otu_sk_a_tsf_i      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_a_tsf_i);
    ADPT_PRINT("otu_sk_a_tsd_i      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_a_tsd_i);
    ADPT_PRINT("otu_sk_a_biae_i     = [%d] \r\n", otu_chnl->otu_frm.otu_sk_a_biae_i);
    ADPT_PRINT("otu_sk_a_bei_i      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_a_bei_i);
    ADPT_PRINT("otu_sk_a_bdi_i      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_a_bdi_i);
    ADPT_PRINT("otu_sk_d_iae_i      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_d_iae_i);
    ADPT_PRINT("otu_sk_d_tim_i      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_d_tim_i);
    ADPT_PRINT("otu_sk_d_deg_i      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_d_deg_i);
    ADPT_PRINT("otu_sk_d_deg_enh_i  = [%d] \r\n", otu_chnl->otu_frm.otu_sk_d_deg_enh_i);
    ADPT_PRINT("otu_sk_d_deg_bur_i  = [%d] \r\n", otu_chnl->otu_frm.otu_sk_d_deg_bursty_i);
    ADPT_PRINT("otu_sk_d_bdi_i      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_d_bdi_i);
    ADPT_PRINT("otu_sk_d_biae_i     = [%d] \r\n", otu_chnl->otu_frm.otu_sk_d_biae_i);
    ADPT_PRINT("otu_sk_ci_ssf_i     = [%d] \r\n", otu_chnl->otu_frm.otu_sk_ci_ssf_i);
    ADPT_PRINT("otu_so_a_iae_i      = [%d] \r\n", otu_chnl->otu_frm.otu_so_a_iae_i);
    ADPT_PRINT("otu_frm_d_lom_v     = [%d] \r\n", otu_chnl->otu_frm.otu_frm_d_lom_v);
    ADPT_PRINT("otu_frm_oom_v       = [%d] \r\n", otu_chnl->otu_frm.otu_frm_oom_v);
    ADPT_PRINT("otu_sk_a_ais_v      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_a_ais_v);
    ADPT_PRINT("otu_sk_a_ssd_v      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_a_ssd_v);
    ADPT_PRINT("otu_sk_a_ssf_v      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_a_ssf_v);
    ADPT_PRINT("otu_sk_a_tsf_v      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_a_tsf_v);
    ADPT_PRINT("otu_sk_a_tsd_v      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_a_tsd_v);
    ADPT_PRINT("otu_sk_a_biae_v     = [%d] \r\n", otu_chnl->otu_frm.otu_sk_a_biae_v);
    ADPT_PRINT("otu_sk_a_bei_v      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_a_bei_v);
    ADPT_PRINT("otu_sk_a_bdi_v      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_a_bdi_v);
    ADPT_PRINT("otu_sk_d_tim_v      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_d_tim_v);
    ADPT_PRINT("otu_sk_d_deg_v      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_d_deg_v);
    ADPT_PRINT("otu_sk_d_deg_enh_v  = [%d] \r\n", otu_chnl->otu_frm.otu_sk_d_deg_enh_v);
    ADPT_PRINT("otu_sk_d_deg_bur_v  = [%d] \r\n", otu_chnl->otu_frm.otu_sk_d_deg_bursty_v);
    ADPT_PRINT("otu_sk_d_bdi_v      = [%d] \r\n", otu_chnl->otu_frm.otu_sk_d_bdi_v);
    ADPT_PRINT("otu_sk_d_biae_v     = [%d] \r\n", otu_chnl->otu_frm.otu_sk_d_biae_v);
    ADPT_PRINT("otu_sk_ci_ssf_v     = [%d] \r\n", otu_chnl->otu_frm.otu_sk_ci_ssf_v);
    /* clear interrupt */
    otu_chnl->otu_frm.otu_frm_d_lom_i       = FALSE;
    otu_chnl->otu_frm.otu_frm_oom_i         = FALSE;
    otu_chnl->otu_frm.otu_sk_a_ais_i        = FALSE; 
    otu_chnl->otu_frm.otu_sk_a_ssd_i        = FALSE;
    otu_chnl->otu_frm.otu_sk_a_ssf_i        = FALSE; 
    otu_chnl->otu_frm.otu_sk_a_tsf_i        = FALSE;
    otu_chnl->otu_frm.otu_sk_a_tsd_i        = FALSE;
    otu_chnl->otu_frm.otu_sk_a_biae_i       = FALSE;
    otu_chnl->otu_frm.otu_sk_a_bei_i        = FALSE; 
    otu_chnl->otu_frm.otu_sk_a_bdi_i        = FALSE; 
    otu_chnl->otu_frm.otu_sk_d_iae_i        = FALSE; 
    otu_chnl->otu_frm.otu_sk_d_tim_i        = FALSE; 
    otu_chnl->otu_frm.otu_sk_d_deg_i        = FALSE; 
    otu_chnl->otu_frm.otu_sk_d_deg_enh_i    = FALSE; 
    otu_chnl->otu_frm.otu_sk_d_deg_bursty_i = FALSE; 
    otu_chnl->otu_frm.otu_sk_d_bdi_i        = FALSE; 
    otu_chnl->otu_frm.otu_sk_d_biae_i       = FALSE;
    otu_chnl->otu_frm.otu_sk_ci_ssf_i       = FALSE;
    otu_chnl->otu_frm.otu_so_a_iae_i        = FALSE; 

    /* OPSM defect and interrupt */
    ADPT_PRINT("opsm23_rx_sfi_los_int_i            = [%d] \r\n", otu_chnl->opsm23_rx_sfi_los_int_i);
    ADPT_PRINT("opsm23_rx_sfi_oof_int_i            = [%d] \r\n", otu_chnl->opsm23_rx_sfi_oof_int_i);
    ADPT_PRINT("opsm23_rx_sfi_lof_int_i            = [%d] \r\n", otu_chnl->opsm23_rx_sfi_lof_int_i);
    ADPT_PRINT("opsm23_rx_otu_oof_int_i            = [%d] \r\n", otu_chnl->opsm23_rx_otu_oof_int_i);
    ADPT_PRINT("opsm23_rx_otu_lof_int_i            = [%d] \r\n", otu_chnl->opsm23_rx_otu_lof_int_i);
    ADPT_PRINT("opsm23_rx_sfi_ais_int_i            = [%d] \r\n", otu_chnl->opsm23_rx_sfi_ais_int_i);
    ADPT_PRINT("opsm23_rx_lol_int_i                = [%d] \r\n", otu_chnl->opsm23_rx_lol_int_i);
    ADPT_PRINT("opsm23_rx_skew_range_v_int_i       = [%d] \r\n", otu_chnl->opsm23_rx_skew_range_violation_int_i);
    ADPT_PRINT("opsm23_rx_align_change_int_i       = [%d] \r\n", otu_chnl->opsm23_rx_align_change_int_i);
    ADPT_PRINT("opsm23_rx_deskew_change_int_i      = [%d] \r\n", otu_chnl->opsm23_rx_deskew_change_int_i);
    ADPT_PRINT("opsm23_rx_sfi_fifo_overflow_int_i  = [%d] \r\n", otu_chnl->opsm23_rx_sfi_fifo_overflow_int_i);
    ADPT_PRINT("opsm23_rx_sfi_fifo_overflow_int_i  = [%d] \r\n", otu_chnl->opsm23_rx_sfi_fifo_overflow_int_i);
    for (i = 0; i < 4; i++)
    {
        ADPT_PRINT("opsm23_rx_lane_los_int_i[%d]        = [%d] \r\n", i, otu_chnl->opsm23_rx_lane_los_int_i[i]);
        ADPT_PRINT("opsm23_rx_lane_oof_int_i[%d]        = [%d] \r\n", i, otu_chnl->opsm23_rx_lane_oof_int_i[i]);
        ADPT_PRINT("opsm23_rx_lane_loflane_int_i[%d]    = [%d] \r\n", i, otu_chnl->opsm23_rx_lane_lof_loflane_int_i[i]);
        ADPT_PRINT("opsm23_rx_lane_otuk_ais_int_i[%d]   = [%d] \r\n", i, otu_chnl->opsm23_rx_lane_otuk_ais_int_i[i]);
        ADPT_PRINT("opsm23_rx_lane_align_chg_int_i[%d]  = [%d] \r\n", i, otu_chnl->opsm23_rx_lane_align_change_int_i[i]);
        ADPT_PRINT("opsm23_rx_lane_fifo_error_int_i[%d] = [%d] \r\n", i, otu_chnl->opsm23_rx_lane_fifo_error_int_i[i]);
        ADPT_PRINT("opsm23_rx_lane_lor_int_i[%d]        = [%d] \r\n", i, otu_chnl->opsm23_rx_lane_lor_int_i[i]);
    }
    ADPT_PRINT("opsm4_tx_sfi_fifo_overflow_int_i   = [%d] \r\n", otu_chnl->opsm4_tx_sfi_fifo_overflow_int_i);
    ADPT_PRINT("opsm4_tx_sfi_fifo_underflow_int_i  = [%d] \r\n", otu_chnl->opsm4_tx_sfi_fifo_underflow_int_i);
    ADPT_PRINT("opsm4_tx_sys_fifo_overflow_int_i   = [%d] \r\n", otu_chnl->opsm4_tx_sys_fifo_overflow_int_i);
    ADPT_PRINT("opsm4_tx_sys_fifo_underflow_int_i  = [%d] \r\n", otu_chnl->opsm4_tx_sys_fifo_underflow_int_i);
    ADPT_PRINT("opsm4_tx_lane_fas_iae_int_i        = [%d] \r\n", otu_chnl->opsm4_tx_lane_fas_iae_int_i);
    ADPT_PRINT("opsm4_tx_lane_mfas_iae_int_i       = [%d] \r\n", otu_chnl->opsm4_tx_lane_mfas_iae_int_i);
    ADPT_PRINT("opsm4_tx_lane_fifo_overflow_int_i  = [%d] \r\n", otu_chnl->opsm4_tx_lane_fifo_overflow_int_i);
    ADPT_PRINT("opsm4_tx_lane_fifo_overflow_int_i  = [%d] \r\n", otu_chnl->opsm4_tx_lane_fifo_overflow_int_i);
    ADPT_PRINT("opsm4_tx_otuk_ais_int_i            = [%d] \r\n", otu_chnl->opsm4_tx_otuk_ais_int_i);
    ADPT_PRINT("opsm4_rx_sfi_los_int_i             = [%d] \r\n", otu_chnl->opsm4_rx_sfi_los_int_i);
    ADPT_PRINT("opsm4_rx_sfi_oof_int_i             = [%d] \r\n", otu_chnl->opsm4_rx_sfi_oof_int_i);
    ADPT_PRINT("opsm4_rx_sfi_lof_int_i             = [%d] \r\n", otu_chnl->opsm4_rx_sfi_lof_int_i);
    ADPT_PRINT("opsm4_rx_otu_oof_int_i             = [%d] \r\n", otu_chnl->opsm4_rx_otu_oof_int_i);
    ADPT_PRINT("opsm4_rx_otu_lof_int_i             = [%d] \r\n", otu_chnl->opsm4_rx_otu_lof_int_i);
    ADPT_PRINT("opsm4_rx_sfi_ais_int_i             = [%d] \r\n", otu_chnl->opsm4_rx_sfi_ais_int_i);
    ADPT_PRINT("opsm4_rx_lol_int_i                 = [%d] \r\n", otu_chnl->opsm4_rx_lol_int_i);
    ADPT_PRINT("opsm4_rx_skew_range_vio_int_i      = [%d] \r\n", otu_chnl->opsm4_rx_skew_range_violation_int_i);
    ADPT_PRINT("opsm4_rx_sfi_align_change_int_i    = [%d] \r\n", otu_chnl->opsm4_rx_sfi_align_change_int_i);
    ADPT_PRINT("opsm4_rx_deskew_change_int_i       = [%d] \r\n", otu_chnl->opsm4_rx_deskew_change_int_i);
    ADPT_PRINT("opsm4_rx_sfi_fifo_overflow_int_i   = [%d] \r\n", otu_chnl->opsm4_rx_sfi_fifo_overflow_int_i);
    ADPT_PRINT("opsm4_rx_sfi_fifo_underflow_int_i  = [%d] \r\n", otu_chnl->opsm4_rx_sfi_fifo_underflow_int_i);
    for (i = 0; i < 20; i++)
    {
        ADPT_PRINT("opsm4_rx_lane_los_int_i[%d]         = [%d] \r\n", i, otu_chnl->opsm4_rx_lane_los_int_i[i]);
        ADPT_PRINT("opsm4_rx_lane_oof_int_i[%d]         = [%d] \r\n", i, otu_chnl->opsm4_rx_lane_oof_int_i[i]);
        ADPT_PRINT("opsm4_rx_lane_lof_loflane_int_i[%d] = [%d] \r\n", i, otu_chnl->opsm4_rx_lane_lof_loflane_int_i[i]);
        ADPT_PRINT("opsm4_rx_lane_oom_int_i[%d]         = [%d] \r\n", i, otu_chnl->opsm4_rx_lane_oom_int_i[i]);
        ADPT_PRINT("opsm4_rx_lane_otuk_ais_int_i[%d]    = [%d] \r\n", i, otu_chnl->opsm4_rx_lane_otuk_ais_int_i[i]);
        ADPT_PRINT("opsm4_rx_lane_align_chg_int_i[%d]   = [%d] \r\n", i, otu_chnl->opsm4_rx_lane_align_change_int_i[i]);
        ADPT_PRINT("opsm4_rx_lane_fifo_error_int_i[%d]  = [%d] \r\n", i, otu_chnl->opsm4_rx_lane_fifo_error_int_i[i]);
        ADPT_PRINT("opsm4_rx_lane_oor_int_i[%d]         = [%d] \r\n", i, otu_chnl->opsm4_rx_lane_oor_int_i[i]);
        ADPT_PRINT("opsm4_rx_lane_lor_int_i[%d]         = [%d] \r\n", i, otu_chnl->opsm4_rx_lane_lor_int_i[i]);
    }
    ADPT_PRINT("opsm23_rx_sfi_los_int_v            = [%d] \r\n", otu_chnl->opsm23_rx_sfi_los_int_v);
    ADPT_PRINT("opsm23_rx_sfi_oof_int_v            = [%d] \r\n", otu_chnl->opsm23_rx_sfi_oof_int_v);
    ADPT_PRINT("opsm23_rx_sfi_lof_int_v            = [%d] \r\n", otu_chnl->opsm23_rx_sfi_lof_int_v);
    ADPT_PRINT("opsm23_rx_otu_oof_int_v            = [%d] \r\n", otu_chnl->opsm23_rx_otu_oof_int_v);
    ADPT_PRINT("opsm23_rx_otu_lof_int_v            = [%d] \r\n", otu_chnl->opsm23_rx_otu_lof_int_v);
    ADPT_PRINT("opsm23_rx_sfi_ais_int_v            = [%d] \r\n", otu_chnl->opsm23_rx_sfi_ais_int_v);
    ADPT_PRINT("opsm23_rx_lol_int_v                = [%d] \r\n", otu_chnl->opsm23_rx_lol_int_v);
    ADPT_PRINT("opsm23_rx_skew_range_v_int_v       = [%d] \r\n", otu_chnl->opsm23_rx_skew_range_violation_int_v);
    ADPT_PRINT("opsm23_rx_align_change_int_v       = [%d] \r\n", otu_chnl->opsm23_rx_align_change_int_v);
    ADPT_PRINT("opsm23_rx_deskew_change_int_v      = [%d] \r\n", otu_chnl->opsm23_rx_deskew_change_int_v);
    ADPT_PRINT("opsm23_rx_sfi_fifo_overflow_int_v  = [%d] \r\n", otu_chnl->opsm23_rx_sfi_fifo_overflow_int_v);
    ADPT_PRINT("opsm23_rx_sfi_fifo_overflow_int_v  = [%d] \r\n", otu_chnl->opsm23_rx_sfi_fifo_overflow_int_v);
    for (i = 0; i < 4; i++)
    {
        ADPT_PRINT("opsm23_rx_lane_los_int_v[%d]        = [%d] \r\n", i, otu_chnl->opsm23_rx_lane_los_int_v[i]);
        ADPT_PRINT("opsm23_rx_lane_oof_int_v[%d]        = [%d] \r\n", i, otu_chnl->opsm23_rx_lane_oof_int_v[i]);
        ADPT_PRINT("opsm23_rx_lane_loflane_int_v[%d]    = [%d] \r\n", i, otu_chnl->opsm23_rx_lane_lof_loflane_int_v[i]);
        ADPT_PRINT("opsm23_rx_lane_otuk_ais_int_v[%d]   = [%d] \r\n", i, otu_chnl->opsm23_rx_lane_otuk_ais_int_v[i]);
        ADPT_PRINT("opsm23_rx_lane_align_chg_int_v[%d]  = [%d] \r\n", i, otu_chnl->opsm23_rx_lane_align_change_int_v[i]);
        ADPT_PRINT("opsm23_rx_lane_fifo_error_int_v[%d] = [%d] \r\n", i, otu_chnl->opsm23_rx_lane_fifo_error_int_v[i]);
        ADPT_PRINT("opsm23_rx_lane_lor_int_v[%d]        = [%d] \r\n", i, otu_chnl->opsm23_rx_lane_lor_int_v[i]);
    }
    ADPT_PRINT("opsm4_tx_sfi_fifo_overflow_int_v   = [%d] \r\n", otu_chnl->opsm4_tx_sfi_fifo_overflow_int_v);
    ADPT_PRINT("opsm4_tx_sfi_fifo_underflow_int_v  = [%d] \r\n", otu_chnl->opsm4_tx_sfi_fifo_underflow_int_v);
    ADPT_PRINT("opsm4_tx_sys_fifo_overflow_int_v   = [%d] \r\n", otu_chnl->opsm4_tx_sys_fifo_overflow_int_v);
    ADPT_PRINT("opsm4_tx_sys_fifo_underflow_int_v  = [%d] \r\n", otu_chnl->opsm4_tx_sys_fifo_underflow_int_v);
    ADPT_PRINT("opsm4_tx_lane_fas_iae_int_v        = [%d] \r\n", otu_chnl->opsm4_tx_lane_fas_iae_int_v);
    ADPT_PRINT("opsm4_tx_lane_mfas_iae_int_v       = [%d] \r\n", otu_chnl->opsm4_tx_lane_mfas_iae_int_v);
    ADPT_PRINT("opsm4_tx_lane_fifo_overflow_int_v  = [%d] \r\n", otu_chnl->opsm4_tx_lane_fifo_overflow_int_v);
    ADPT_PRINT("opsm4_tx_lane_fifo_overflow_int_v  = [%d] \r\n", otu_chnl->opsm4_tx_lane_fifo_overflow_int_v);
    ADPT_PRINT("opsm4_tx_otuk_ais_int_v            = [%d] \r\n", otu_chnl->opsm4_tx_otuk_ais_int_v);
    ADPT_PRINT("opsm4_rx_sfi_los_int_v             = [%d] \r\n", otu_chnl->opsm4_rx_sfi_los_int_v);
    ADPT_PRINT("opsm4_rx_sfi_oof_int_v             = [%d] \r\n", otu_chnl->opsm4_rx_sfi_oof_int_v);
    ADPT_PRINT("opsm4_rx_sfi_lof_int_v             = [%d] \r\n", otu_chnl->opsm4_rx_sfi_lof_int_v);
    ADPT_PRINT("opsm4_rx_otu_oof_int_v             = [%d] \r\n", otu_chnl->opsm4_rx_otu_oof_int_v);
    ADPT_PRINT("opsm4_rx_otu_lof_int_v             = [%d] \r\n", otu_chnl->opsm4_rx_otu_lof_int_v);
    ADPT_PRINT("opsm4_rx_sfi_ais_int_v             = [%d] \r\n", otu_chnl->opsm4_rx_sfi_ais_int_v);
    ADPT_PRINT("opsm4_rx_lol_int_v                 = [%d] \r\n", otu_chnl->opsm4_rx_lol_int_v);
    ADPT_PRINT("opsm4_rx_skew_range_vio_int_v      = [%d] \r\n", otu_chnl->opsm4_rx_skew_range_violation_int_v);
    ADPT_PRINT("opsm4_rx_sfi_align_change_int_v    = [%d] \r\n", otu_chnl->opsm4_rx_sfi_align_change_int_v);
    ADPT_PRINT("opsm4_rx_deskew_change_int_v       = [%d] \r\n", otu_chnl->opsm4_rx_deskew_change_int_v);
    ADPT_PRINT("opsm4_rx_sfi_fifo_overflow_int_v   = [%d] \r\n", otu_chnl->opsm4_rx_sfi_fifo_overflow_int_v);
    ADPT_PRINT("opsm4_rx_sfi_fifo_underflow_int_v  = [%d] \r\n", otu_chnl->opsm4_rx_sfi_fifo_underflow_int_v);
    for (i = 0; i < 20; i++)
    {
        ADPT_PRINT("opsm4_rx_lane_los_int_v[%d]         = [%d] \r\n", i, otu_chnl->opsm4_rx_lane_los_int_v[i]);
        ADPT_PRINT("opsm4_rx_lane_oof_int_v[%d]         = [%d] \r\n", i, otu_chnl->opsm4_rx_lane_oof_int_v[i]);
        ADPT_PRINT("opsm4_rx_lane_lof_loflane_int_v[%d] = [%d] \r\n", i, otu_chnl->opsm4_rx_lane_lof_loflane_int_v[i]);
        ADPT_PRINT("opsm4_rx_lane_oom_int_v[%d]         = [%d] \r\n", i, otu_chnl->opsm4_rx_lane_oom_int_v[i]);
        ADPT_PRINT("opsm4_rx_lane_otuk_ais_int_v[%d]    = [%d] \r\n", i, otu_chnl->opsm4_rx_lane_otuk_ais_int_v[i]);
        ADPT_PRINT("opsm4_rx_lane_align_chg_int_v[%d]   = [%d] \r\n", i, otu_chnl->opsm4_rx_lane_align_change_int_v[i]);
        ADPT_PRINT("opsm4_rx_lane_fifo_error_int_v[%d]  = [%d] \r\n", i, otu_chnl->opsm4_rx_lane_fifo_error_int_v[i]);
        ADPT_PRINT("opsm4_rx_lane_oor_int_v[%d]         = [%d] \r\n", i, otu_chnl->opsm4_rx_lane_oor_int_v[i]);
        ADPT_PRINT("opsm4_rx_lane_lor_int_v[%d]         = [%d] \r\n", i, otu_chnl->opsm4_rx_lane_lor_int_v[i]);
    }
    /* clear interrupt */
    otu_chnl->opsm23_rx_sfi_los_int_i              = FALSE;
    otu_chnl->opsm23_rx_sfi_oof_int_i              = FALSE;
    otu_chnl->opsm23_rx_sfi_lof_int_i              = FALSE;
    otu_chnl->opsm23_rx_otu_oof_int_i              = FALSE;
    otu_chnl->opsm23_rx_otu_lof_int_i              = FALSE;
    otu_chnl->opsm23_rx_sfi_ais_int_i              = FALSE;
    otu_chnl->opsm23_rx_lol_int_i                  = FALSE;
    otu_chnl->opsm23_rx_skew_range_violation_int_i = FALSE;
    otu_chnl->opsm23_rx_align_change_int_i         = FALSE;
    otu_chnl->opsm23_rx_deskew_change_int_i        = FALSE;
    otu_chnl->opsm23_rx_sfi_fifo_overflow_int_i    = FALSE; 
    otu_chnl->opsm23_rx_sfi_fifo_underflow_int_i   = FALSE; 
    for (i = 0; i < 4; i++)
    {
        otu_chnl->opsm23_rx_lane_los_int_i[i]          = FALSE; 
        otu_chnl->opsm23_rx_lane_oof_int_i[i]          = FALSE; 
        otu_chnl->opsm23_rx_lane_lof_loflane_int_i[i]  = FALSE; 
        otu_chnl->opsm23_rx_lane_otuk_ais_int_i[i]     = FALSE; 
        otu_chnl->opsm23_rx_lane_otl_ais_int_i[i]      = FALSE; 
        otu_chnl->opsm23_rx_lane_align_change_int_i[i] = FALSE; 
        otu_chnl->opsm23_rx_lane_fifo_error_int_i[i]   = FALSE; 
        otu_chnl->opsm23_rx_lane_oor_int_i[i]          = FALSE; 
        otu_chnl->opsm23_rx_lane_lor_int_i[i]          = FALSE; 
    }

    otu_chnl->opsm4_tx_sfi_fifo_overflow_int_i    = FALSE; 
    otu_chnl->opsm4_tx_sfi_fifo_underflow_int_i   = FALSE; 
    otu_chnl->opsm4_tx_sys_fifo_overflow_int_i    = FALSE; 
    otu_chnl->opsm4_tx_sys_fifo_underflow_int_i   = FALSE; 
    otu_chnl->opsm4_tx_lane_fas_iae_int_i         = FALSE; 
    otu_chnl->opsm4_tx_lane_mfas_iae_int_i        = FALSE; 
    otu_chnl->opsm4_tx_lane_fifo_overflow_int_i   = FALSE; 
    otu_chnl->opsm4_tx_lane_fifo_underflow_int_i  = FALSE; 
    otu_chnl->opsm4_tx_otuk_ais_int_i             = FALSE; 
    otu_chnl->opsm4_rx_sfi_los_int_i              = FALSE; 
    otu_chnl->opsm4_rx_sfi_oof_int_i              = FALSE; 
    otu_chnl->opsm4_rx_sfi_lof_int_i              = FALSE; 
    otu_chnl->opsm4_rx_otu_oof_int_i              = FALSE; 
    otu_chnl->opsm4_rx_otu_lof_int_i              = FALSE; 
    otu_chnl->opsm4_rx_sfi_ais_int_i              = FALSE; 
    otu_chnl->opsm4_rx_lol_int_i                  = FALSE; 
    otu_chnl->opsm4_rx_skew_range_violation_int_i = FALSE; 
    otu_chnl->opsm4_rx_sfi_align_change_int_i     = FALSE; 
    otu_chnl->opsm4_rx_deskew_change_int_i        = FALSE; 
    otu_chnl->opsm4_rx_sfi_fifo_overflow_int_i    = FALSE; 
    otu_chnl->opsm4_rx_sfi_fifo_underflow_int_i   = FALSE; 
    for (i = 0; i < 20; i++)
    {
        otu_chnl->opsm4_rx_lane_los_int_i[i]          = FALSE; 
        otu_chnl->opsm4_rx_lane_oof_int_i[i]          = FALSE; 
        otu_chnl->opsm4_rx_lane_lof_loflane_int_i[i]  = FALSE; 
        otu_chnl->opsm4_rx_lane_oom_int_i[i]          = FALSE; 
        otu_chnl->opsm4_rx_lane_otuk_ais_int_i[i]     = FALSE; 
        otu_chnl->opsm4_rx_lane_align_change_int_i[i] = FALSE; 
        otu_chnl->opsm4_rx_lane_fifo_error_int_i[i]   = FALSE; 
        otu_chnl->opsm4_rx_lane_oor_int_i[i]          = FALSE; 
        otu_chnl->opsm4_rx_lane_lor_int_i[i]          = FALSE; 
    }

    PMC_RETURN(rc);
} /* adpt_otu_status_print */


/*******************************************************************************
* adpt_odu_status_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Prints ODU defect and interrupt data.
*
* INPUTS:
*   *odu_chnl          - COREOTN ODU defect and int data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_odu_status_print(coreotn_int_chnl_t *odu_chnl)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(odu_chnl);

    /* PM defect and interrupt */
    ADPT_PRINT("ac_aps_i            = [%d] \r\n", odu_chnl->framer_chnl.rfrm.ac_aps_i);
    ADPT_PRINT("dlck_pm_i           = [%d] \r\n", odu_chnl->framer_chnl.rfrm.dlck_pm_i);
    ADPT_PRINT("doci_pm_i           = [%d] \r\n", odu_chnl->framer_chnl.rfrm.doci_pm_i);
    ADPT_PRINT("dais_pm_i           = [%d] \r\n", odu_chnl->framer_chnl.rfrm.dais_pm_i);
    ADPT_PRINT("dtim_pm_i           = [%d] \r\n", odu_chnl->framer_chnl.rfrm.dtim_pm_i);
    ADPT_PRINT("ddeg_pm_i           = [%d] \r\n", odu_chnl->framer_chnl.rfrm.ddeg_pm_i);
    ADPT_PRINT("ddeg_enh_pm_i       = [%d] \r\n", odu_chnl->framer_chnl.rfrm.ddeg_enh_pm_i);
    ADPT_PRINT("ddeg_bursty_pm_i    = [%d] \r\n", odu_chnl->framer_chnl.rfrm.ddeg_bursty_pm_i);
    ADPT_PRINT("dbdi_pm_i           = [%d] \r\n", odu_chnl->framer_chnl.rfrm.dbdi_pm_i);
    ADPT_PRINT("oof_i               = [%d] \r\n", odu_chnl->framer_chnl.rfrm.oof_i);
    ADPT_PRINT("dloflom_i           = [%d] \r\n", odu_chnl->framer_chnl.rfrm.dloflom_i);
    ADPT_PRINT("atsf_pm_i           = [%d] \r\n", odu_chnl->framer_chnl.rfrm.atsf_pm_i);
    ADPT_PRINT("atsd_pm_i           = [%d] \r\n", odu_chnl->framer_chnl.rfrm.atsd_pm_i);
    ADPT_PRINT("abdi_pm_i           = [%d] \r\n", odu_chnl->framer_chnl.rfrm.abdi_pm_i);
    ADPT_PRINT("dlck_pm_v           = [%d] \r\n", odu_chnl->framer_chnl.rfrm.dlck_pm_v);
    ADPT_PRINT("doci_pm_v           = [%d] \r\n", odu_chnl->framer_chnl.rfrm.doci_pm_v);
    ADPT_PRINT("dais_pm_v           = [%d] \r\n", odu_chnl->framer_chnl.rfrm.dais_pm_v);
    ADPT_PRINT("dtim_pm_v           = [%d] \r\n", odu_chnl->framer_chnl.rfrm.dtim_pm_v);
    ADPT_PRINT("ddeg_pm_v           = [%d] \r\n", odu_chnl->framer_chnl.rfrm.ddeg_pm_v);
    ADPT_PRINT("ddeg_enh_pm_v       = [%d] \r\n", odu_chnl->framer_chnl.rfrm.ddeg_enh_pm_v);
    ADPT_PRINT("ddeg_bursty_pm_v    = [%d] \r\n", odu_chnl->framer_chnl.rfrm.ddeg_bursty_pm_v);
    ADPT_PRINT("dbdi_pm_v           = [%d] \r\n", odu_chnl->framer_chnl.rfrm.dbdi_pm_v);
    ADPT_PRINT("oof_v               = [%d] \r\n", odu_chnl->framer_chnl.rfrm.oof_v);
    ADPT_PRINT("dloflom_v           = [%d] \r\n", odu_chnl->framer_chnl.rfrm.dloflom_v);
    ADPT_PRINT("ci_ssf_pm_v         = [%d] \r\n", odu_chnl->framer_chnl.rfrm.ci_ssf_pm_v);
    ADPT_PRINT("atsf_pm_v           = [%d] \r\n", odu_chnl->framer_chnl.rfrm.atsf_pm_v);
    ADPT_PRINT("atsd_pm_v           = [%d] \r\n", odu_chnl->framer_chnl.rfrm.atsd_pm_v);
    ADPT_PRINT("abdi_pm_v           = [%d] \r\n", odu_chnl->framer_chnl.rfrm.abdi_pm_v);
    /* clear interrupt */
    odu_chnl->framer_chnl.rfrm.ac_aps_i         = FALSE;
    odu_chnl->framer_chnl.rfrm.dlck_pm_i        = FALSE;
    odu_chnl->framer_chnl.rfrm.doci_pm_i        = FALSE;
    odu_chnl->framer_chnl.rfrm.dais_pm_i        = FALSE;
    odu_chnl->framer_chnl.rfrm.dtim_pm_i        = FALSE;
    odu_chnl->framer_chnl.rfrm.ddeg_pm_i        = FALSE;
    odu_chnl->framer_chnl.rfrm.ddeg_enh_pm_i    = FALSE;
    odu_chnl->framer_chnl.rfrm.ddeg_bursty_pm_i = FALSE;
    odu_chnl->framer_chnl.rfrm.dbdi_pm_i        = FALSE;
    odu_chnl->framer_chnl.rfrm.oof_i            = FALSE;
    odu_chnl->framer_chnl.rfrm.dloflom_i        = FALSE;
    odu_chnl->framer_chnl.rfrm.atsf_pm_i        = FALSE;
    odu_chnl->framer_chnl.rfrm.atsd_pm_i        = FALSE;
    odu_chnl->framer_chnl.rfrm.abdi_pm_i        = FALSE;

    ADPT_PRINT("tfrm.dm_pm_i        = [%d] \r\n", odu_chnl->framer_chnl.tfrm.iae_i);
    ADPT_PRINT("tfrm.dm_pm_i        = [%d] \r\n", odu_chnl->framer_chnl.tfrm.dm_pm_i);
    ADPT_PRINT("tfrm.tfrm.iae_v     = [%d] \r\n", odu_chnl->framer_chnl.tfrm.iae_v);
    /* clear interrupt */
    odu_chnl->framer_chnl.tfrm.iae_i         = FALSE;
    odu_chnl->framer_chnl.tfrm.dm_pm_i       = FALSE;

    ADPT_PRINT("dplm_i              = [%d] \r\n", odu_chnl->framer_chnl.odtu_dmx.dplm_i);
    ADPT_PRINT("dmsim_i             = [%d] \r\n", odu_chnl->framer_chnl.odtu_dmx.dmsim_i);
    ADPT_PRINT("ssf_sf_rcvd_i       = [%d] \r\n", odu_chnl->framer_chnl.odtu_dmx.ssf_sf_rcvd_i);
    ADPT_PRINT("cm_out_limit_i      = [%d] \r\n", odu_chnl->framer_chnl.odtu_dmx.cm_out_limit_i);
    ADPT_PRINT("cnd_max_i           = [%d] \r\n", odu_chnl->framer_chnl.odtu_dmx.cnd_max_i);
    ADPT_PRINT("jc_nomtch_lossync_i = [%d] \r\n", odu_chnl->framer_chnl.odtu_dmx.jc_nomtch_lossync_i);
    ADPT_PRINT("cm_cnd_crc_err_i    = [%d] \r\n", odu_chnl->framer_chnl.odtu_dmx.cm_cnd_crc_err_i);
    ADPT_PRINT("dplm_v              = [%d] \r\n", odu_chnl->framer_chnl.odtu_dmx.dplm_v);
    ADPT_PRINT("dmsim_v             = [%d] \r\n", odu_chnl->framer_chnl.odtu_dmx.dmsim_v);
    ADPT_PRINT("ssf_sf_rcvd_v       = [%d] \r\n", odu_chnl->framer_chnl.odtu_dmx.dplm_v);
    /* clear interrupt */
    odu_chnl->framer_chnl.odtu_dmx.dplm_i              = FALSE;
    odu_chnl->framer_chnl.odtu_dmx.dmsim_i             = FALSE;
    odu_chnl->framer_chnl.odtu_dmx.ssf_sf_rcvd_i       = FALSE;
    odu_chnl->framer_chnl.odtu_dmx.cm_out_limit_i      = FALSE;
    odu_chnl->framer_chnl.odtu_dmx.cnd_max_i           = FALSE;
    odu_chnl->framer_chnl.odtu_dmx.jc_nomtch_lossync_i = FALSE;
    odu_chnl->framer_chnl.odtu_dmx.cm_cnd_crc_err_i    = FALSE;

    ADPT_PRINT("ohfs_ist.fifo_ovr_i = [%d] \r\n", odu_chnl->framer_chnl.odtu_mux.tgmp.ohfs_insert.cfc.fifo_ovr_i);
    ADPT_PRINT("ohfs_ist.fifo_udr_i = [%d] \r\n", odu_chnl->framer_chnl.odtu_mux.tgmp.ohfs_insert.cfc.fifo_udr_i);
    /* clear interrupt */
    odu_chnl->framer_chnl.odtu_mux.tgmp.ohfs_insert.cfc.fifo_ovr_i = FALSE;
    odu_chnl->framer_chnl.odtu_mux.tgmp.ohfs_insert.cfc.fifo_udr_i = FALSE; 

    ADPT_PRINT("txjc_pro.fifo_ovr_i = [%d] \r\n", odu_chnl->framer_chnl.odtu_mux.tgmp.txjc_proc.cfc.fifo_ovr_i);
    ADPT_PRINT("txjc_pro.fifo_udr_i = [%d] \r\n", odu_chnl->framer_chnl.odtu_mux.tgmp.txjc_proc.cfc.fifo_udr_i);
    /* clear interrupt */
    odu_chnl->framer_chnl.odtu_mux.tgmp.txjc_proc.cfc.fifo_ovr_i = FALSE;
    odu_chnl->framer_chnl.odtu_mux.tgmp.txjc_proc.cfc.fifo_udr_i = FALSE; 

    ADPT_PRINT("exp_ibuf.fifo_ovr_i = [%d] \r\n", odu_chnl->framer_chnl.odtu_mux.tgmp.expand_ibuf.cfc.fifo_ovr_i);
    ADPT_PRINT("exp_ibuf.fifo_udr_i = [%d] \r\n", odu_chnl->framer_chnl.odtu_mux.tgmp.expand_ibuf.cfc.fifo_udr_i);
    /* clear interrupt */
    odu_chnl->framer_chnl.odtu_mux.tgmp.expand_ibuf.cfc.fifo_ovr_i = FALSE;
    odu_chnl->framer_chnl.odtu_mux.tgmp.expand_ibuf.cfc.fifo_udr_i = FALSE; 

    ADPT_PRINT("oduksc.src_zone_i   = [%d] \r\n", odu_chnl->framer_chnl.oduksc.src_zone_i);
    ADPT_PRINT("oduksc.ramp_done_i  = [%d] \r\n", odu_chnl->framer_chnl.oduksc.ramp_done_i);
    ADPT_PRINT("oduksc.src_zone_v   = [%d] \r\n", odu_chnl->framer_chnl.oduksc.src_zone_v);
    /* clear interrupt */
    odu_chnl->framer_chnl.oduksc.src_zone_i  = FALSE;
    odu_chnl->framer_chnl.oduksc.ramp_done_i = FALSE;

    ADPT_PRINT("odujat.fifo_ovr_i   = [%d] \r\n", odu_chnl->framer_chnl.odujat.cfc.fifo_ovr_i);
    ADPT_PRINT("odujat.fifo_udr_i   = [%d] \r\n", odu_chnl->framer_chnl.odujat.cfc.fifo_udr_i);
    /* clear interrupt */
    odu_chnl->framer_chnl.odujat.cfc.fifo_ovr_i     = FALSE;
    odu_chnl->framer_chnl.odujat.cfc.fifo_udr_i     = FALSE;

    ADPT_PRINT("mpmo_valid          = [%d] \r\n", odu_chnl->framer_chnl.mpmo_valid);
    ADPT_PRINT("mpmo.cfc.fifo_ovr_i = [%d] \r\n", odu_chnl->framer_chnl.mpmo.cfc.fifo_ovr_i);
    ADPT_PRINT("mpmo.cfc.fifo_udr_i = [%d] \r\n", odu_chnl->framer_chnl.mpmo.cfc.fifo_udr_i);
    ADPT_PRINT("mpmo.pkt_drop_r_i   = [%d] \r\n", odu_chnl->framer_chnl.mpmo.pkt_drop_replace_i);
    ADPT_PRINT("mpmo.pkt_drop_n_r_i = [%d] \r\n", odu_chnl->framer_chnl.mpmo.pkt_drop_no_replace_i);
    /* clear interrupt */
    odu_chnl->framer_chnl.mpmo.cfc.fifo_ovr_i        = FALSE;
    odu_chnl->framer_chnl.mpmo.cfc.fifo_udr_i        = FALSE;
    odu_chnl->framer_chnl.mpmo.pkt_drop_replace_i    = FALSE;
    odu_chnl->framer_chnl.mpmo.pkt_drop_no_replace_i = FALSE;

    ADPT_PRINT("mpma_valid          = [%d] \r\n", odu_chnl->framer_chnl.mpma_valid);
    ADPT_PRINT("mpma.cfc.fifo_ovr_i = [%d] \r\n", odu_chnl->framer_chnl.mpma.cfc.fifo_ovr_i);
    ADPT_PRINT("mpma.cfc.fifo_udr_i = [%d] \r\n", odu_chnl->framer_chnl.mpma.cfc.fifo_udr_i);
    ADPT_PRINT("mpma.psd_fifo_ovr_i = [%d] \r\n", odu_chnl->framer_chnl.mpma.psd_fifo_ovr_i);
    ADPT_PRINT("mpma.psd_fifo_udr_i = [%d] \r\n", odu_chnl->framer_chnl.mpma.psd_fifo_udr_i);
    /* clear interrupt */
    odu_chnl->framer_chnl.mpma.cfc.fifo_ovr_i = FALSE;
    odu_chnl->framer_chnl.mpma.cfc.fifo_udr_i = FALSE;
    odu_chnl->framer_chnl.mpma.psd_fifo_ovr_i = FALSE;
    odu_chnl->framer_chnl.mpma.psd_fifo_udr_i = FALSE;

    /* TCM defect and interrupt */
    for (index = 0; index < 6; index++)
    {
        ADPT_PRINT("dlck_i[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].dlck_i);
        ADPT_PRINT("doci_i[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].doci_i);
        ADPT_PRINT("dais_i[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].dais_i);
        ADPT_PRINT("dtim_i[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].dtim_i);
        ADPT_PRINT("ddeg_i[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].ddeg_i);
        ADPT_PRINT("ddeg_enh_i[TCM%d]    = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].ddeg_enh_i);
        ADPT_PRINT("ddeg_bursty_i[TCM%d] = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].ddeg_bursty_i);
        ADPT_PRINT("dbdi_i[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].dbdi_i);
        ADPT_PRINT("dbiae_i[TCM%d]       = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].dbiae_i);
        ADPT_PRINT("dltc_i[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].dltc_i);
        ADPT_PRINT("diae_i[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].diae_i);
        ADPT_PRINT("atsf_i[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].atsf_i);
        ADPT_PRINT("atsd_i[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].atsd_i);
        ADPT_PRINT("aais_i[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].aais_i);
        ADPT_PRINT("abdi_i[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].abdi_i);
        ADPT_PRINT("dlck_v[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].dlck_v);
        ADPT_PRINT("doci_v[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].doci_v);
        ADPT_PRINT("dais_v[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].dais_v);
        ADPT_PRINT("dtim_v[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].dtim_v);
        ADPT_PRINT("ddeg_v[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].ddeg_v);
        ADPT_PRINT("ddeg_enh_v[TCM%d]    = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].ddeg_enh_v);
        ADPT_PRINT("ddeg_bursty_v[TCM%d] = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].ddeg_bursty_v);
        ADPT_PRINT("dbdi_v[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].dbdi_v);
        ADPT_PRINT("dbiae_v[TCM%d]       = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].dbiae_v);
        ADPT_PRINT("dltc_v[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].dltc_v);
        ADPT_PRINT("diae_v[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].diae_v);
        ADPT_PRINT("ci_ssf_v[TCM%d]      = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].ci_ssf_v);
        ADPT_PRINT("atsf_v[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].atsf_v);
        ADPT_PRINT("atsd_v[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].atsd_v);
        ADPT_PRINT("aais_v[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].aais_v);
        ADPT_PRINT("abdi_v[TCM%d]        = [%d] \r\n", index + 1, odu_chnl->pid.rx_tcm[index].abdi_v);
        /* clear interrupt */
        odu_chnl->pid.rx_tcm[index].dlck_i        = FALSE;
        odu_chnl->pid.rx_tcm[index].doci_i        = FALSE;
        odu_chnl->pid.rx_tcm[index].dais_i        = FALSE;
        odu_chnl->pid.rx_tcm[index].dtim_i        = FALSE;
        odu_chnl->pid.rx_tcm[index].ddeg_i        = FALSE;
        odu_chnl->pid.rx_tcm[index].ddeg_enh_i    = FALSE;
        odu_chnl->pid.rx_tcm[index].ddeg_bursty_i = FALSE;
        odu_chnl->pid.rx_tcm[index].dbdi_i        = FALSE;
        odu_chnl->pid.rx_tcm[index].dbiae_i       = FALSE;
        odu_chnl->pid.rx_tcm[index].dltc_i        = FALSE;
        odu_chnl->pid.rx_tcm[index].diae_i        = FALSE;
        odu_chnl->pid.rx_tcm[index].atsf_i        = FALSE;
        odu_chnl->pid.rx_tcm[index].atsd_i        = FALSE;
        odu_chnl->pid.rx_tcm[index].aais_i        = FALSE;
        odu_chnl->pid.rx_tcm[index].abdi_i        = FALSE;
    }

    /* PRBS defect and interrupt */
    if (odu_chnl->prbs_valid)
    {
        ADPT_PRINT("prbs_dlss_i         = [%d] \r\n", odu_chnl->prbs.prbs_dlss_i);
        ADPT_PRINT("prbs_dlss_v         = [%d] \r\n", odu_chnl->prbs.prbs_dlss_v);
        /* clear interrupt */
        odu_chnl->prbs.prbs_dlss_i = FALSE;
    }

    PMC_RETURN(rc);
} /* adpt_odu_status_print */


/*******************************************************************************
* adpt_t8_vert_hist
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves T8 SerDes histogram information. 
*
* INPUTS:
*   dev_id             - device identity.
*   slice              - DIGI device concept.
*   subch              - DIGI device concept.
*
* OUTPUTS:
*   *buffer            - T8 histogram value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_t8_vert_hist(UINT32 dev_id,
                               UINT32 slice, 
                               UINT32 subch, 
                               UINT64 *buffer)
{

    UINT32 base_addr = 0x40000 + 0x1000 * slice + 0x600 * subch;
    UINT32 i = 0;
    UINT32 j = 0;
    UINT32 capture_running = 0;
    UINT32 index = 0;
    UINT32 eye_index = 0; 
    UINT64 value_31_0 = 0;
    UINT64 value_39_32 = 0;
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    pmc_sys_reg_write(adpt_handle->sys_handle, base_addr + 0x20c, 0x0FFFFFFF); 
    pmc_sys_reg_write(adpt_handle->sys_handle, base_addr + 0x208, 0x04); 
    pmc_sys_reg_write(adpt_handle->sys_handle, base_addr + 0x208, 0x05); 
    pmc_sys_reg_write(adpt_handle->sys_handle, base_addr + 0x208, 0x0D); 
    pmc_sys_reg_write(adpt_handle->sys_handle, base_addr + 0x208, 0x0D); 
    pmc_sys_reg_write(adpt_handle->sys_handle, base_addr + 0x208, 0x05); 
    pmc_sys_reg_write(adpt_handle->sys_handle, base_addr + 0x208, 0x04); 
    pmc_sys_reg_write(adpt_handle->sys_handle, base_addr + 0x208, 0x06); 

    index  = 0;
    for (i = 0; i < 8; i++)
    {
        do
        {
            capture_running = pmc_sys_reg_read(adpt_handle->sys_handle, base_addr + 0x210);
        } 
        while (capture_running & 0x1);

        for (j = 0; j < 8; j++)
        {
            value_31_0  = pmc_sys_reg_read (adpt_handle->sys_handle, base_addr + 0x214);
            value_39_32 = pmc_sys_reg_read (adpt_handle->sys_handle, base_addr + 0x210);
            value_39_32 = value_39_32 & 0xFF000000;
            if (index < 32) 
            { 
                eye_index = 31 - index; 
            } 
            else 
            { 
                eye_index = 95 - index; 
            } 
            buffer[eye_index] = value_31_0 + (value_39_32 << 8);            

            index++;
            pmc_sys_reg_write(adpt_handle->sys_handle, base_addr + 0x208, 0x02);
            pmc_sys_reg_write(adpt_handle->sys_handle, base_addr + 0x208, 0x06);
        }
    }

    pmc_sys_reg_write(adpt_handle->sys_handle, base_addr + 0x208, 0x04);

    PMC_RETURN(rc);
} /* adpt_t8_vert_hist */


#ifdef PMC_COMEXPRESS
/*******************************************************************************
* adpt_time_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves time tag. 
*
* INPUTS:
*   None.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Time tag.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 adpt_time_get()
{
    struct timeval tv;
    UINT32 time_us;

    gettimeofday(&tv, NULL);

    time_us = ((tv.tv_sec % 1000) * 1000000) + (tv.tv_usec % 1000000);

    return time_us;
}/* adpt_time_get */
#endif


/*******************************************************************************
* adpt_oduksw_latency_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the latency from backplane in ODUk SW MPMO block. Units 3.215 
*   nsecond ticks.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *debug_latency     - PKT_AGE from debug registers. 
*   *min_latency       - PKT_AGE using a raw retieval mechanism.
*   *max_latency       - PKT_AGE using a raw retieval mechanism.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   This API should be used for development purposes only. It should not be 
*   used for operational paths in fielded systems.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_oduksw_latency_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT32 *debug_latency,
                                      UINT32 *min_latency,
                                      UINT32 *max_latency)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 traffic_type = 0;
    util_global_switch_data_t *switch_data = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(debug_latency);
    ADPT_PARA_VERIFY(min_latency);
    ADPT_PARA_VERIFY(max_latency);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get traffic type */
    traffic_type = DIGI_TRAFFIC_TYPE_GET(adpt_handle->traffic_info[index].traffic_mode);
    switch (traffic_type)
    {
        case DIGI_TYPE_OTU:
            switch_data = (util_global_switch_data_t*)(adpt_handle->traffic_info[index].odu_handle);
            break;

        case DIGI_TYPE_ETH:
            if (NULL != adpt_handle->traffic_info[index].map_handle)
            {
                switch_data = (util_global_switch_data_t*)(adpt_handle->traffic_info[index].map_handle);
            }
            else
            {
                switch_data = (util_global_switch_data_t*)(adpt_handle->traffic_info[index].enet_handle);
            }
            break;

        case DIGI_TYPE_CBR:
            if (NULL != adpt_handle->traffic_info[index].map_handle)
            {
                switch_data = (util_global_switch_data_t*)(adpt_handle->traffic_info[index].map_handle);
            }
            else
            {
                switch_data = (util_global_switch_data_t*)(adpt_handle->traffic_info[index].cbr_handle);
            }
            break;

        case DIGI_TYPE_INLK:
            if (NULL != adpt_handle->traffic_info[index].map_handle)
            {
                switch_data = (util_global_switch_data_t*)(adpt_handle->traffic_info[index].map_handle);
            }
            else
            {
                switch_data = (util_global_switch_data_t*)(adpt_handle->traffic_info[index].ilkn_handle);
            }
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[index].oduk_src_index)
        || DIGI_SWT_EN_GET(adpt_handle->traffic_info[index].cpb_src_index))
    {
        rc |= digi_oduksw_mpmo_debug_latency_get(adpt_handle->digi_handle,
                                                 switch_data,
                                                 debug_latency);
        ADPT_RETURN_VERIFY(rc);

        rc |= digi_oduksw_mpmo_raw_latency_get(adpt_handle->digi_handle,
                                               switch_data,
                                               min_latency,
                                               max_latency);
        ADPT_RETURN_VERIFY(rc);
    }
    else
    {
        *debug_latency = 0;
        *min_latency   = 0;
        *max_latency   = 0;
    }

    PMC_RETURN(rc);
} /* adpt_oduksw_latency_get */


/*******************************************************************************
* adpt_tcm_pool_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resets the resources of the TCM pool ID. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_tcm_pool_reset(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 i = 0;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    for (i = 0; i < 7; i++)
    {
        /* the sink position on stage 3B */
        adpt_handle->traffic_info[index].tcm_pool_id[2][i] = 0;
        /* the source position on stage 3B */
        adpt_handle->traffic_info[index].tcm_pool_id[3][i] = 0;
    }

    PMC_RETURN(rc);
} /* adpt_tcm_pool_reset */


/*******************************************************************************
* adpt_otn_odu_level_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the ODU level information based on the given channel.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   ODU level information.
*
* NOTES:
*
*******************************************************************************/
PUBLIC odu_struct_odu_level_t adpt_otn_odu_level_get(UINT32 dev_id, 
                                                  UINT32 chnl_id,
                                                  digi_otn_loc_t location)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    odu_struct_odu_level_t odu_level = LAST_ODU_STRUCT_LEVEL;
    PMC_ENTRY();

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    if (PMC_SUCCESS != rc)
    {
        PMC_RETURN(odu_level);
    }

    odu_level = digi_otn_odu_level_get(digi_handle, odu_handle);

    PMC_RETURN(odu_level);
} /* adpt_otn_odu_level_get */


/*******************************************************************************
* adpt_tx_conseq_act_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resets those unecessary consequent actions in the Tx direction.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *conseq_act        - consequent action table.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_tx_conseq_act_reset(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_client2trunk_conseq_action_t *conseq_act)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL client_sdh  = FALSE;
    BOOL client_cbr  = FALSE;
    BOOL client_fc   = FALSE;
    BOOL client_enet = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(conseq_act);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    switch (adpt_handle->traffic_info[index].mapping_mode)
    {
        case DIGI_MAPOTN_ODU3P_OC_768_CBR_BMP:
        case DIGI_MAPOTN_ODU3P_OC_768_CBR_AMP:
        case DIGI_MAPOTN_ODU2P_OC_192_CBR_BMP:
        case DIGI_MAPOTN_ODU2P_OC_192_CBR_AMP:
            client_sdh = TRUE;
            break;

        case DIGI_MAPOTN_ODU2P_5G_GDPS_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_5_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_10_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_6_GMP:
        case DIGI_MAPOTN_ODU2P_FC_800_GMP:
        case DIGI_MAPOTN_ODUFLEX_FC_800_BMP:
        case DIGI_MAPOTN_ODUFLEX_5G_GDPS_BMP:
        case DIGI_MAPOTN_ODUFLEX_10G_GDPS_BMP:
        case DIGI_MAPOTN_ODUFLEX_CPRI_5_BMP:
        case DIGI_MAPOTN_ODUFLEX_CPRI_10_BMP:
        case DIGI_MAPOTN_ODUFLEX_CPRI_6_BMP:
        case DIGI_MAPOTN_ODU2EP_10G_GDPS_GMP:
            client_cbr = TRUE;
            break;

        case DIGI_MAPOTN_ODU2EP_FC_1200_GFP:
        case DIGI_MAPOTN_ODU2FP_FC_1200_BMP:
        case DIGI_MAPOTN_ODU1FP_FC_1200_BMP:
            client_fc = TRUE;
            break;

        case DIGI_MAPOTN_ODU0_PACKET_GFP:
        case DIGI_MAPOTN_ODU1_PACKET_GFP:
        case DIGI_MAPOTN_ODU1E_PACKET_GFP:
        case DIGI_MAPOTN_ODU1F_PACKET_GFP:
        case DIGI_MAPOTN_ODU2_PACKET_GFP:
        case DIGI_MAPOTN_ODU2E_PACKET_GFP:
        case DIGI_MAPOTN_ODU2F_PACKET_GFP:
        case DIGI_MAPOTN_ODU3_PACKET_GFP:
        case DIGI_MAPOTN_ODU3E1_PACKET_GFP:
        case DIGI_MAPOTN_ODU3E2_PACKET_GFP:
        case DIGI_MAPOTN_ODU4_PACKET_GFP:
        case DIGI_MAPOTN_ODUFLEX_PACKET_GFP:
        case DIGI_MAPOTN_ODU4P_100_GE_GFP:
        case DIGI_MAPOTN_ODU4P_100_GE_GFP_ENET:
        case DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP:
        case DIGI_MAPOTN_ODU4P_CBR_100_GE_GMP:
        case DIGI_MAPOTN_ODU3P_40_GE_GFP_ENET:
        case DIGI_MAPOTN_ODU3P_CBR_40_GE_GMP:
        case DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP:
        case DIGI_MAPOTN_ODU3P_40_GE_GFP:
        case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP_ENET:
        case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP:
        case DIGI_MAPOTN_ODU1EP_CBR_10G3_7_2_BMP:
        case DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP:
        case DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP:
        case DIGI_MAPOTN_ODU2EP_CBR_10G3_7_1_BMP:
        case DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP_ENET:
        case DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP:
        case DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP:
            client_enet = TRUE;
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    if (!client_cbr)
    {
        PMC_MEMSET((void*)&conseq_act->cbrc_8b10b_loss_signal_mon, 0, 
                   sizeof(conseq_act->cbrc_8b10b_loss_signal_mon));
        PMC_MEMSET((void*)&conseq_act->cbrc_8b10b_loss_sync_mon, 0, 
                   sizeof(conseq_act->cbrc_8b10b_loss_sync_mon));
    }

    if (!client_sdh)
    {
        PMC_MEMSET((void*)&conseq_act->sonet_loss_signal_mon, 0, 
                   sizeof(conseq_act->sonet_loss_signal_mon));
        PMC_MEMSET((void*)&conseq_act->sonet_loss_frame_mon, 0, 
                   sizeof(conseq_act->sonet_loss_frame_mon));
    }

    if (!client_fc)
    {
        PMC_MEMSET((void*)&conseq_act->fc1200_loss_sync_mon, 0, 
                   sizeof(conseq_act->fc1200_loss_sync_mon));
        PMC_MEMSET((void*)&conseq_act->fc1200_loss_signal_mon, 0, 
                   sizeof(conseq_act->fc1200_loss_signal_mon));
    }

    if (!client_enet)
    {
        PMC_MEMSET((void*)&conseq_act->enet_emac_phy_los_mon, 0, 
                   sizeof(conseq_act->enet_emac_phy_los_mon));
        PMC_MEMSET((void*)&conseq_act->enet_emac_block_lock_mon, 0, 
                   sizeof(conseq_act->enet_emac_block_lock_mon));
        PMC_MEMSET((void*)&conseq_act->enet_emac_hi_ber_mon, 0, 
                   sizeof(conseq_act->enet_emac_hi_ber_mon));
        PMC_MEMSET((void*)&conseq_act->enet_emac_rx_loc_fault_mon, 0, 
                   sizeof(conseq_act->enet_emac_rx_loc_fault_mon));
        PMC_MEMSET((void*)&conseq_act->enet_emac_rx_rem_fault_mon, 0, 
                   sizeof(conseq_act->enet_emac_rx_rem_fault_mon));
        PMC_MEMSET((void*)&conseq_act->enet_epmm_loss_signal_mon, 0, 
                   sizeof(conseq_act->enet_epmm_loss_signal_mon));
        PMC_MEMSET((void*)&conseq_act->enet_epmm_local_fault_mon, 0, 
                   sizeof(conseq_act->enet_epmm_local_fault_mon));
        PMC_MEMSET((void*)&conseq_act->enet_epmm_remote_fault_mon, 0, 
                   sizeof(conseq_act->enet_epmm_remote_fault_mon));
        PMC_MEMSET((void*)&conseq_act->enet_mac_40g_100g_loam_mon, 0, 
                   sizeof(conseq_act->enet_mac_40g_100g_loam_mon));
    }

    PMC_RETURN(rc);
} /* adpt_tx_conseq_act_reset */


/*******************************************************************************
* adpt_rx_conseq_act_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resets those unecessary consequent actions in the Rx direction.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *conseq_act        - consequent action table.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_rx_conseq_act_reset(UINT32 dev_id,
                                      UINT32 chnl_id, 
                                      cmf_trunk2client_conseqAction_t *conseq_act)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL gfp_mapping  = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(conseq_act);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    switch (adpt_handle->traffic_info[index].mapping_mode)
    {
        case DIGI_MAPOTN_ODU2EP_FC_1200_GFP:
        case DIGI_MAPOTN_ODU0_PACKET_GFP:
        case DIGI_MAPOTN_ODU1_PACKET_GFP:
        case DIGI_MAPOTN_ODU1E_PACKET_GFP:
        case DIGI_MAPOTN_ODU1F_PACKET_GFP:
        case DIGI_MAPOTN_ODU2_PACKET_GFP:
        case DIGI_MAPOTN_ODU2E_PACKET_GFP:
        case DIGI_MAPOTN_ODU2F_PACKET_GFP:
        case DIGI_MAPOTN_ODU3_PACKET_GFP:
        case DIGI_MAPOTN_ODU3E1_PACKET_GFP:
        case DIGI_MAPOTN_ODU3E2_PACKET_GFP:
        case DIGI_MAPOTN_ODU4_PACKET_GFP:
        case DIGI_MAPOTN_ODUFLEX_PACKET_GFP:
        case DIGI_MAPOTN_ODU4P_100_GE_GFP:
        case DIGI_MAPOTN_ODU4P_100_GE_GFP_ENET:
        case DIGI_MAPOTN_ODU3P_40_GE_GFP_ENET:
        case DIGI_MAPOTN_ODU3P_40_GE_GFP:
        case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP_ENET:
        case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP:
        case DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP_ENET:
        case DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP:
        case DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP:
            gfp_mapping = TRUE;
            break;

        case DIGI_MAPOTN_ODU3P_OC_768_CBR_BMP:
        case DIGI_MAPOTN_ODU3P_OC_768_CBR_AMP:
        case DIGI_MAPOTN_ODU2P_OC_192_CBR_BMP:
        case DIGI_MAPOTN_ODU2P_OC_192_CBR_AMP:
        case DIGI_MAPOTN_ODU2P_5G_GDPS_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_5_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_10_GMP:
        case DIGI_MAPOTN_ODU2P_CPRI_6_GMP:
        case DIGI_MAPOTN_ODU2P_FC_800_GMP:
        case DIGI_MAPOTN_ODUFLEX_FC_800_BMP:
        case DIGI_MAPOTN_ODUFLEX_5G_GDPS_BMP:
        case DIGI_MAPOTN_ODUFLEX_10G_GDPS_BMP:
        case DIGI_MAPOTN_ODUFLEX_CPRI_5_BMP:
        case DIGI_MAPOTN_ODUFLEX_CPRI_10_BMP:
        case DIGI_MAPOTN_ODUFLEX_CPRI_6_BMP:
        case DIGI_MAPOTN_ODU2EP_10G_GDPS_GMP:
        case DIGI_MAPOTN_ODU2FP_FC_1200_BMP:
        case DIGI_MAPOTN_ODU1FP_FC_1200_BMP:
        case DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP:
        case DIGI_MAPOTN_ODU4P_CBR_100_GE_GMP:
        case DIGI_MAPOTN_ODU3P_CBR_40_GE_GMP:
        case DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP:
        case DIGI_MAPOTN_ODU1EP_CBR_10G3_7_2_BMP:
        case DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP:
        case DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP:
        case DIGI_MAPOTN_ODU2EP_CBR_10G3_7_1_BMP:
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    if (!gfp_mapping)
    {
        PMC_MEMSET((void*)&conseq_act->gfp_lofd_mon, 0, 
                   sizeof(conseq_act->gfp_lofd_mon));
        PMC_MEMSET((void*)&conseq_act->gfp_pti_upi_mismatch_mon, 0, 
                   sizeof(conseq_act->gfp_pti_upi_mismatch_mon));
        PMC_MEMSET((void*)&conseq_act->gfp_cmf_csf_loss_client_signal_mon, 0, 
                   sizeof(conseq_act->gfp_cmf_csf_loss_client_signal_mon));
        PMC_MEMSET((void*)&conseq_act->gfp_cmf_csf_loss_char_sync_mon, 0, 
                   sizeof(conseq_act->gfp_cmf_csf_loss_char_sync_mon));
        PMC_MEMSET((void*)&conseq_act->gfp_cmf_fdi_mon, 0, 
                   sizeof(conseq_act->gfp_cmf_fdi_mon));
        PMC_MEMSET((void*)&conseq_act->gfp_cmf_rdi_mon, 0, 
                   sizeof(conseq_act->gfp_cmf_rdi_mon));
        PMC_MEMSET((void*)&conseq_act->gfp_dexm_mon, 0, 
                   sizeof(conseq_act->gfp_dexm_mon));
    }

    PMC_RETURN(rc);
} /* adpt_rx_conseq_act_reset */


/*******************************************************************************
* adpt_gfp_rx_conseq_act_ex_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the client traffic defect auto insertion in the Rx 
*   direction which allows for two consequent actions. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   alm_msk            - defect mask to trigger client traffic defect auto 
*                        insertion. The defect mask lists as follows.
*                        DIGI_ACT_SSF_SF
*                        DIGI_ACT_PLM
*                        DIGI_ACT_OPU_CSF
*                        DIGI_ACT_GFP_LFD
*                        DIGI_ACT_GFP_UPM
*                        DIGI_ACT_GFP_CSF_LOS
*                        DIGI_ACT_GFP_CSF_LOCS
*                        DIGI_ACT_GFP_FDI
*                        DIGI_ACT_GFP_RDI
*                        DIGI_ACT_SDH_SFIS_LOF
*                        DIGI_ACT_GFP_EXM
*   alm_act1           - the first defect action for insertion.
*   alm_act2           - the second defect action for insertion.
*   holdoff_time       - time in ms to wait before turning off action1 and turning
*                        on action2. This must be in multiples of 100ms(0,100,200..1000 ms).
*   enable             - TRUE: enable consequent action auto insertion.
*                        FALSE: disable consequent action auto insertion.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   1. This API is available for those datapaths through MAPOTN.
*   2. This API can only be called after a full datapath provisioning and 
*      activation. After reactivating the datapath, the consequential actions 
*      may once again be re-configured.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_rx_conseq_act_ex_set(UINT32 dev_id,
                                           UINT32 chnl_id,
                                           UINT32 alm_msk,
                                           digi_gfp_rx_act_t alm_act1,
                                           digi_gfp_rx_act_t alm_act2,
                                           UINT16 holdoff_time,
                                           BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    digi_trunk2client_tables_cmf_t csf_table;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d].\r\n", dev_id, chnl_id, 
               alm_msk, alm_act1, alm_act2, enable);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    /* reset */
    PMC_MEMSET((void*)&csf_table, 0, sizeof(digi_trunk2client_tables_cmf_t));

    rc = digi_cmf_trunktoclient_conseq_action_get(digi_handle,
                                                  map_handle,
                                                  &csf_table.conseq_action_cmf);
    ADPT_RETURN_VERIFY(rc);
    rc = digi_cmf_trunktoclient_port_mapping_get(digi_handle,
                                                 map_handle,
                                                 &csf_table.port_mapping_cmf);
    ADPT_RETURN_VERIFY(rc);

    /* workaround: reset unecessary consequent action */
    rc = adpt_rx_conseq_act_reset(dev_id, chnl_id, &csf_table.conseq_action_cmf);
    ADPT_RETURN_VERIFY(rc);

    /* SSF_SF */
    if (DIGI_ACT_SSF_SF & alm_msk)
    {
        csf_table.conseq_action_cmf.ssf_sf_rcvd_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.ssf_sf_rcvd_mon.action1        = (digi_trunk2client_actions_t)alm_act1;
        csf_table.conseq_action_cmf.ssf_sf_rcvd_mon.action2        = (digi_trunk2client_actions_t)alm_act2;
        csf_table.conseq_action_cmf.ssf_sf_rcvd_mon.second_action_holdoff_time = holdoff_time;
        csf_table.conseq_action_cmf.ssf_sf_rcvd_mon.priority_idx   = enable ? 1 : 0;
    }
    /* dPLM */
    if (DIGI_ACT_PLM & alm_msk)
    {
        csf_table.conseq_action_cmf.dplm_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.dplm_mon.action1        = (digi_trunk2client_actions_t)alm_act1;
        csf_table.conseq_action_cmf.dplm_mon.action2        = (digi_trunk2client_actions_t)alm_act2;
        csf_table.conseq_action_cmf.dplm_mon.second_action_holdoff_time = holdoff_time;
        csf_table.conseq_action_cmf.dplm_mon.priority_idx   = enable ? 2 : 0;
    }
    /* OPUdCSF */
    if (DIGI_ACT_OPU_CSF & alm_msk)
    {
        csf_table.conseq_action_cmf.opu_csf_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.opu_csf_mon.action1        = (digi_trunk2client_actions_t)alm_act1;
        csf_table.conseq_action_cmf.opu_csf_mon.action2        = (digi_trunk2client_actions_t)alm_act2;
        csf_table.conseq_action_cmf.opu_csf_mon.second_action_holdoff_time = holdoff_time;
        csf_table.conseq_action_cmf.opu_csf_mon.priority_idx   = enable ? 3 : 0;
    }
    /* GFPdLFD */
    if (DIGI_ACT_GFP_LFD & alm_msk)
    {
        csf_table.conseq_action_cmf.gfp_lofd_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.gfp_lofd_mon.action1        = (digi_trunk2client_actions_t)alm_act1;
        csf_table.conseq_action_cmf.gfp_lofd_mon.action2        = (digi_trunk2client_actions_t)alm_act2;
        csf_table.conseq_action_cmf.gfp_lofd_mon.second_action_holdoff_time = holdoff_time;
        csf_table.conseq_action_cmf.gfp_lofd_mon.priority_idx   = enable ? 4 : 0;
    }
    /* GFPdUPM */
    if (DIGI_ACT_GFP_UPM & alm_msk)
    {
        csf_table.conseq_action_cmf.gfp_pti_upi_mismatch_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.gfp_pti_upi_mismatch_mon.action1        = (digi_trunk2client_actions_t)alm_act1;
        csf_table.conseq_action_cmf.gfp_pti_upi_mismatch_mon.action2        = (digi_trunk2client_actions_t)alm_act2;
        csf_table.conseq_action_cmf.gfp_pti_upi_mismatch_mon.second_action_holdoff_time = holdoff_time;
        csf_table.conseq_action_cmf.gfp_pti_upi_mismatch_mon.priority_idx   = enable ? 5 : 0;
    }
    /* GFPdCSF_LOS */
    if (DIGI_ACT_GFP_CSF_LOS & alm_msk)
    {
        csf_table.conseq_action_cmf.gfp_cmf_csf_loss_client_signal_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.gfp_cmf_csf_loss_client_signal_mon.action1        = (digi_trunk2client_actions_t)alm_act1;
        csf_table.conseq_action_cmf.gfp_cmf_csf_loss_client_signal_mon.action2        = (digi_trunk2client_actions_t)alm_act2;
        csf_table.conseq_action_cmf.gfp_cmf_csf_loss_client_signal_mon.second_action_holdoff_time = holdoff_time;
        csf_table.conseq_action_cmf.gfp_cmf_csf_loss_client_signal_mon.priority_idx   = enable ? 6 : 0;
    }
    /* GFPdCSF_LOCS */
    if (DIGI_ACT_GFP_CSF_LOCS & alm_msk)
    {
        csf_table.conseq_action_cmf.gfp_cmf_csf_loss_char_sync_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.gfp_cmf_csf_loss_char_sync_mon.action1        = (digi_trunk2client_actions_t)alm_act1;
        csf_table.conseq_action_cmf.gfp_cmf_csf_loss_char_sync_mon.action2        = (digi_trunk2client_actions_t)alm_act2;
        csf_table.conseq_action_cmf.gfp_cmf_csf_loss_char_sync_mon.second_action_holdoff_time = holdoff_time;
        csf_table.conseq_action_cmf.gfp_cmf_csf_loss_char_sync_mon.priority_idx   = enable ? 7 : 0;
    }
    /* GFPdEXM */
    if (DIGI_ACT_GFP_EXM & alm_msk)
    {
        csf_table.conseq_action_cmf.gfp_dexm_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.gfp_dexm_mon.action1        = (digi_trunk2client_actions_t)alm_act1;
        csf_table.conseq_action_cmf.gfp_dexm_mon.action2        = (digi_trunk2client_actions_t)alm_act2;
        csf_table.conseq_action_cmf.gfp_dexm_mon.second_action_holdoff_time = holdoff_time;
        csf_table.conseq_action_cmf.gfp_dexm_mon.priority_idx   = enable ? 8 : 0;
    }
    /* GFPdFDI */
    if (DIGI_ACT_GFP_FDI & alm_msk)
    {
        csf_table.conseq_action_cmf.gfp_cmf_fdi_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.gfp_cmf_fdi_mon.action1        = (digi_trunk2client_actions_t)alm_act1;
        csf_table.conseq_action_cmf.gfp_cmf_fdi_mon.action2        = (digi_trunk2client_actions_t)alm_act2;
        csf_table.conseq_action_cmf.gfp_cmf_fdi_mon.second_action_holdoff_time = holdoff_time;
        csf_table.conseq_action_cmf.gfp_cmf_fdi_mon.priority_idx   = enable ? 9 : 0;
    }
    /* GFPdRDI */
    if (DIGI_ACT_GFP_RDI & alm_msk)
    {
        csf_table.conseq_action_cmf.gfp_cmf_rdi_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.gfp_cmf_rdi_mon.action1        = (digi_trunk2client_actions_t)alm_act1;
        csf_table.conseq_action_cmf.gfp_cmf_rdi_mon.action2        = (digi_trunk2client_actions_t)alm_act2;
        csf_table.conseq_action_cmf.gfp_cmf_rdi_mon.second_action_holdoff_time = holdoff_time;
        csf_table.conseq_action_cmf.gfp_cmf_rdi_mon.priority_idx   = enable ? 10 : 0;
    }
    /* SDHdLOF */
    if (DIGI_ACT_SDH_SFIS_LOF & alm_msk)
    {
        csf_table.conseq_action_cmf.sonet_sfis_lof_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.sonet_sfis_lof_mon.action1        = (digi_trunk2client_actions_t)alm_act1;
        csf_table.conseq_action_cmf.sonet_sfis_lof_mon.action2        = (digi_trunk2client_actions_t)alm_act2;
        csf_table.conseq_action_cmf.sonet_sfis_lof_mon.second_action_holdoff_time = holdoff_time;
        csf_table.conseq_action_cmf.sonet_sfis_lof_mon.priority_idx   = enable ? 11 : 0;
    }

    rc = digi_cmf_trunktoclient_conseq_action_cfg(digi_handle,
                                                  map_handle,
                                                  &csf_table);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cmf_trunktoclient_tables_update(digi_handle,
                                              map_handle);

    PMC_RETURN(rc);
} /* adpt_gfp_rx_conseq_act_ex_set */


/*******************************************************************************
* adpt_gfp_rx_conseq_act_ex_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves enable / disable status of the client traffic defect auto insertion 
*   in the Rx direction which allows for two consequent actions. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   alm_item           - defect item to trigger client traffic defect auto 
*                        insertion.
*
* OUTPUTS:
*   *alm_act1          - the first defect action for insertion.
*   *alm_act2          - the second defect action for insertion.
*   *holdoff_time      - time in ms to wait before turning off action1 and turning
*                        on action2. This must be in multiples of 100ms(0,100,200..1000 ms).
*   *enable            - TRUE: enable consequent action auto insertion.
*                        FALSE: disable consequent action auto insertion.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   This API is available for those datapaths through MAPOTN. Only one mask can
*   be used for each calling.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_rx_conseq_act_ex_get(UINT32 dev_id,
                                           UINT32 chnl_id,
                                           digi_rx_alm_act_mask_t alm_item,
                                           digi_gfp_rx_act_t *alm_act1,
                                           digi_gfp_rx_act_t *alm_act2,
                                           UINT16 *holdoff_time,
                                           BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    digi_trunk2client_tables_cmf_t csf_table;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(alm_act1);
    ADPT_PARA_VERIFY(alm_act2);
    ADPT_PARA_VERIFY(holdoff_time);
    ADPT_PARA_VERIFY(enable);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    /* reset */
    PMC_MEMSET((void*)&csf_table, 0, sizeof(digi_trunk2client_tables_cmf_t));
    rc = digi_cmf_trunktoclient_conseq_action_get(digi_handle,
                                                  map_handle,
                                                  &csf_table.conseq_action_cmf);
    ADPT_RETURN_VERIFY(rc);

    /* SSF_SF */
    if (DIGI_ACT_SSF_SF & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.ssf_sf_rcvd_mon.monitor_defect;
        *alm_act1 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.ssf_sf_rcvd_mon.action1;
        *alm_act2 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.ssf_sf_rcvd_mon.action2;
        *holdoff_time = csf_table.conseq_action_cmf.ssf_sf_rcvd_mon.second_action_holdoff_time;
        PMC_RETURN(rc);
    }
    /* dPLM */
    if (DIGI_ACT_PLM & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.dplm_mon.monitor_defect;
        *alm_act1 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.dplm_mon.action1;
        *alm_act2 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.dplm_mon.action2;
        *holdoff_time = csf_table.conseq_action_cmf.dplm_mon.second_action_holdoff_time;
        PMC_RETURN(rc);
    }
    /* OPUdCSF */
    if (DIGI_ACT_OPU_CSF & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.opu_csf_mon.monitor_defect;
        *alm_act1 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.opu_csf_mon.action1;
        *alm_act2 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.opu_csf_mon.action2;
        *holdoff_time = csf_table.conseq_action_cmf.opu_csf_mon.second_action_holdoff_time;
        PMC_RETURN(rc);
    }
    /* GFPdLFD */
    if (DIGI_ACT_GFP_LFD & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.gfp_lofd_mon.monitor_defect;
        *alm_act1 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_lofd_mon.action1;
        *alm_act2 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_lofd_mon.action2;
        *holdoff_time = csf_table.conseq_action_cmf.gfp_lofd_mon.second_action_holdoff_time;
        PMC_RETURN(rc);
    }
    /* GFPdUPM */
    if (DIGI_ACT_GFP_UPM & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.gfp_pti_upi_mismatch_mon.monitor_defect;
        *alm_act1 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_pti_upi_mismatch_mon.action1;
        *alm_act2 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_pti_upi_mismatch_mon.action2;
        *holdoff_time = csf_table.conseq_action_cmf.gfp_pti_upi_mismatch_mon.second_action_holdoff_time;
        PMC_RETURN(rc);
    }
    /* GFPdCSF_LOS */
    if (DIGI_ACT_GFP_CSF_LOS & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.gfp_cmf_csf_loss_client_signal_mon.monitor_defect;
        *alm_act1 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_cmf_csf_loss_client_signal_mon.action1;
        *alm_act2 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_cmf_csf_loss_client_signal_mon.action2;
        *holdoff_time = csf_table.conseq_action_cmf.gfp_cmf_csf_loss_client_signal_mon.second_action_holdoff_time;
        PMC_RETURN(rc);
    }
    /* GFPdCSF_LOCS */
    if (DIGI_ACT_GFP_CSF_LOCS & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.gfp_cmf_csf_loss_char_sync_mon.monitor_defect;
        *alm_act1 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_cmf_csf_loss_char_sync_mon.action1;
        *alm_act2 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_cmf_csf_loss_char_sync_mon.action2;
        *holdoff_time = csf_table.conseq_action_cmf.gfp_cmf_csf_loss_char_sync_mon.second_action_holdoff_time;
        PMC_RETURN(rc);
    }
    /* GFPdEXM */
    if (DIGI_ACT_GFP_EXM & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.gfp_dexm_mon.monitor_defect;
        *alm_act1 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_dexm_mon.action1;
        *alm_act2 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_dexm_mon.action2;
        *holdoff_time = csf_table.conseq_action_cmf.gfp_dexm_mon.second_action_holdoff_time;
        PMC_RETURN(rc);
    }
    /* GFPdFDI */
    if (DIGI_ACT_GFP_FDI & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.gfp_cmf_fdi_mon.monitor_defect;
        *alm_act1 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_cmf_fdi_mon.action1;
        *alm_act2 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_cmf_fdi_mon.action2;
        *holdoff_time = csf_table.conseq_action_cmf.gfp_cmf_fdi_mon.second_action_holdoff_time;
        PMC_RETURN(rc);
    }
    /* GFPdRDI */
    if (DIGI_ACT_GFP_RDI & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.gfp_cmf_rdi_mon.monitor_defect;
        *alm_act1 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_cmf_rdi_mon.action1;
        *alm_act2 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_cmf_rdi_mon.action2;
        *holdoff_time = csf_table.conseq_action_cmf.gfp_cmf_rdi_mon.second_action_holdoff_time;
        PMC_RETURN(rc);
    }
    /* SDHdLOF */
    if (DIGI_ACT_SDH_SFIS_LOF & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.sonet_sfis_lof_mon.monitor_defect;
        *alm_act1 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.sonet_sfis_lof_mon.action1;
        *alm_act2 = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.sonet_sfis_lof_mon.action2;
        *holdoff_time = csf_table.conseq_action_cmf.sonet_sfis_lof_mon.second_action_holdoff_time;
        PMC_RETURN(rc);
    }

    PMC_RETURN(rc);
} /* adpt_gfp_rx_conseq_act_ex_get */


/*******************************************************************************
* adpt_serdes_bp_lane_prbs_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures serdes PRBS function of one T8 or C8 or S16 serdes per lane
*   in the backplane interface.
*
* INPUTS:
*   dev_id             - device identity.
*   dir                - traffic direction: Tx, Rx, Rx and Tx.
*   *sifd_1_lane_cfg   - pointer to SIFD interlaken 1 lanes configuration.
*   *sifd_2_lane_cfg   - pointer to SIFD interlaken 2 lanes configuration.
*   *patt_cfg          - pointer to a pattern configuration.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_bp_lane_prbs_set(UINT32 dev_id,
                                          digi_serdes_dir_t dir,
                                          digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg,
                                          digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg,
                                          util_serdes_patt_cfg_t *patt_cfg)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    util_serdes_patt_mon_cfg_t prbs_mon_cfg;
    util_serdes_patt_gen_cfg_t prbs_gen_cfg;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(patt_cfg);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* pattern configuration */
    PMC_MEMCPY(&prbs_mon_cfg.patt_cfg, patt_cfg, sizeof(util_serdes_patt_cfg_t));
    PMC_MEMCPY(&prbs_gen_cfg.patt_cfg, patt_cfg, sizeof(util_serdes_patt_cfg_t));

    if (patt_cfg->enable)
    {
        /* configuration on generation */
        if ((DIGI_SERDES_DIR_TX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
        {
            rc = digi_serdes_bp_patt_gen_prov(adpt_handle->digi_handle,
                                              sifd_1_lane_cfg,
                                              sifd_2_lane_cfg,
                                              &prbs_gen_cfg);
            ADPT_RETURN_VERIFY(rc);
        }

        /* configuration on monitor */
        if ((DIGI_SERDES_DIR_RX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
        {
            rc = digi_serdes_bp_patt_mon_prov(adpt_handle->digi_handle,
                                              sifd_1_lane_cfg,
                                              sifd_2_lane_cfg,
                                              &prbs_mon_cfg);
            ADPT_RETURN_VERIFY(rc);

            /* PRBS monitor resynchronization */
            rc = digi_serdes_bp_patt_mon_resync(adpt_handle->digi_handle,
                                                sifd_1_lane_cfg,
                                                sifd_2_lane_cfg);
            ADPT_RETURN_VERIFY(rc);
        }
    }
    else
    {
        if ((DIGI_SERDES_DIR_TX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
        {
            rc = digi_serdes_bp_patt_gen_deprov(adpt_handle->digi_handle,
                                                sifd_1_lane_cfg,
                                                sifd_2_lane_cfg);
            ADPT_RETURN_VERIFY(rc);
        }

        if ((DIGI_SERDES_DIR_RX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
        {
            rc = digi_serdes_bp_patt_mon_deprov(adpt_handle->digi_handle,
                                                sifd_1_lane_cfg,
                                                sifd_2_lane_cfg);
            ADPT_RETURN_VERIFY(rc);
        }
    }

    PMC_RETURN(rc);
} /* adpt_serdes_bp_lane_prbs_set */


/*******************************************************************************
* adpt_serdes_bp_lane_tx_err_inst_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures error insertion per lane in the Tx direction of serdes PRBS  
*   function generated by one T8 or C8 or S16 serdes in the backplane interface.
*
* INPUTS:
*   dev_id             - device identity.
*   *sifd_1_lane_cfg   - pointer to SIFD interlaken 1 lanes configuration.
*   *sifd_2_lane_cfg   - pointer to SIFD interlaken 2 lanes configuration.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_bp_lane_tx_err_inst_set(UINT32 dev_id, 
                                               digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg,
                                               digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_serdes_bp_patt_gen_err_insert_cfg(adpt_handle->digi_handle,
                                                sifd_1_lane_cfg,
                                                sifd_2_lane_cfg);    

    PMC_RETURN(rc);
} /* adpt_serdes_bp_lane_tx_err_inst_set */


/*******************************************************************************
* adpt_serdes_bp_lane_rx_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves pattern monitor status and the number of errors detected by 
*   one T8 or C8 or S16 serdes per lane in the backplane interface.
*
* INPUTS:
*   dev_id             - device identity.
*   *sifd_1_lane_cfg   - pointer to SIFD interlaken 1 lanes configuration.
*   *sifd_2_lane_cfg   - pointer to SIFD interlaken 2 lanes configuration.
*
* OUTPUTS:
*   *status            - pattern monitor status: Locked, trying to lock, etc.                  
*   *err_cnt           - number of errors detected by pattern monitor. 
*                        The value shall be ignored when pattern monitor
*                        is not in locked state.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_bp_lane_rx_state_get(UINT32 dev_id,
                                             digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg,
                                             digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg,
                                             util_patt_status_t *status,
                                             UINT32 *err_cnt)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(status);
    ADPT_PARA_VERIFY(err_cnt);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get PRBS status */
    rc = digi_serdes_bp_patt_mon_err_count_get(adpt_handle->digi_handle,
                                               sifd_1_lane_cfg,
                                               sifd_2_lane_cfg,
                                               status,
                                               err_cnt);

    PMC_RETURN(rc);
} /* adpt_serdes_bp_lane_rx_state_get */


/*
** End of file
*/
