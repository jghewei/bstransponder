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
*     The file describes basic functionality, including global configuration,
*     device level configuration, initialziation configuraiton.
*
*   NOTES:
*     None.
*
*******************************************************************************/
/*
** Include Files
*/
#include "digi_adpt.h"

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
pmc_log_sev_t adpt_log_sev = PMC_LOG_SEV_DISABLE;
#ifdef PMC_COMEXPRESS
PRIVATE char sv_host_str[64];
#endif

/*
** Public Functions
*/
/*******************************************************************************
* adpt_log_init()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes the logging feature.  
*
* INPUTS:
*   log_sev     - Verbosity level desired for logging output.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR   - PMC_SUCCESS: get valid SW status.
*                 PMC_ERR_FAIL: get invalid SW status.
*                 PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_log_init(pmc_log_sev_t log_sev)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    ADPT_TRACE("Input para[%d].\r\n", log_sev);

    pmc_log_init(PMC_LOG_DEFAULT_SIZE);
    pmc_log_verbosity_level_set(log_sev);
    pmc_log_block_strings_register(PMC_SHARED_LOG_ERR_STRINGS[0], 
                                   PMC_SHARED_LOG_ERR_TABLE_BASE, 
                                   PMC_SHARED_LOG_ERR_COUNT);
    adpt_log_sev = log_sev;

    PMC_RETURN(rc);
} /* adpt_log_init */


/*******************************************************************************
* adpt_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize digi SDK and firmware.
*
* INPUTS:
*   dev_id             - device identity.
*
*                        31 30 ... 25 24  23 22 ... 17 16  15 14 13 ... 2 1 0
*                        _______________  _______________  __________________
*                         slot_id[31:24]  digi_dev[23:16]    card_id[15:0]       
*
*   base_addr          - device physical base address.
*   init_type          - DIGI_INIT_COLD(0):  cold initialization.
*                        DIGI_INIT_WARM(1):  warm initialization.
*                        DIGI_INIT_CRASH(2): crash initialization.
*                        DIGI_INIT_WARM_FW_RELOAD(3): warm initiaization with FW reload.
*                        DIGI_INIT_CRASH_FW_RELOAD(4): crash initiaization with FW reload.
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
PUBLIC PMC_ERROR adpt_init(UINT32 dev_id, UINT32 base_addr, digi_init_type_t init_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 serdes_id = 0;
    digi_dev_info_t *dev_info = NULL;
    digi_cfg_container_t *adpt_handle = NULL;
    void *ctxt_ptr = NULL;
#ifdef PMC_COMEXPRESS
    char file_name[256];
#else
    char file_name[ADPT_FILE_NAME_LEN];
#endif
    UINT32 buf_size = 0;
    pmc_mem_pool_handle pool;
    PMC_UINTPTR buf_addr_vir = 0;
    UINT64 buf_addr_phy = 0;
    digi_pmon_work_order_msg_t pmon_order;
    digi_serdes_cfg_t *serdes_cfg = NULL;
    digi_handle_t *digi_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][0x%x][%d].\r\n", dev_id, base_addr, init_type);

    /* get card information */
    rc = adpt_dev_info_get(dev_id, &dev_info);
    ADPT_RETURN_VERIFY(rc);

    /* file name */
#ifdef PMC_COMEXPRESS
    gethostname(sv_host_str, 63);

    PMC_SPRINTF((char*)&file_name, "%s_adpt%d_%s.bin", 
                (char*)&(dev_info->file_name), 
                DIGI_DIGI_DEV_GET(dev_id), sv_host_str);

    if (NULL != getenv("PMC_CTXT_FILENAME_NO_HOSTNAME")) 
    {
        PMC_SPRINTF((char*)&file_name, "%s_adpt%d.bin", 
                    (char*)&(dev_info->file_name), 
                    DIGI_DIGI_DEV_GET(dev_id));
    } 
#else
    PMC_SPRINTF((char*)&file_name, "%s_adpt%d", 
                (char*)&(dev_info->file_name), 
                DIGI_DIGI_DEV_GET(dev_id));
#endif

    /* create adapter container handle */
    /* restore adapter container from flash or non volatile memory */
    rc = adpt_handle_create(dev_id, (DIGI_INIT_COLD == init_type) ? FALSE : TRUE, 
                            (char*)&file_name, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* initialize system handle */
    adpt_handle->sys_handle = pmc_sys_handle_create();
    rc |= pmc_sys_init(adpt_handle->sys_handle, 
                       (void*)(PMC_UINTPTR)base_addr, 
                       adpt_device_name_get(DIGI_DIGI_DEV_GET(dev_id)));

    /* cold reboot or power up */
    if (DIGI_INIT_COLD == init_type)
    {
        /* initialize adapter container handle */
        rc |= adpt_handle_init(adpt_handle);
        ADPT_RETURN_VERIFY(rc);

        /* initialize DIGI instance */
        adpt_handle->digi_handle = digi_ctxt_create(NULL, 
                                                    DIGI0_BASE_ADDRESS, 
                                                    adpt_handle->sys_handle, 
                                                    adpt_device_name_get(DIGI_DIGI_DEV_GET(dev_id)));

        /* configure firmware file name */
        if (NULL != dev_info->fw_filename)
        {
            digi_fw_filename_set(adpt_handle->digi_handle, dev_info->fw_filename);
        }

        /* initialize DIGI handle */
        digi_handle_init(adpt_handle->digi_handle, NULL);

        /* interrupt type */
        rc = digi_int_init(adpt_handle->digi_handle,
                           dev_info->int_type,
                           (CPUIF_INT_DISABLE != dev_info->int_type) ? TRUE : FALSE);
        ADPT_RETURN_VERIFY(rc);

        /* serdes initialization in line side */
        if (dev_info->num_line_serdes_port > 0)
        {
            /* send the cage descriptions for system reservation */
            rc |= digi_serdes_cage_cfg(adpt_handle->digi_handle, 
                                       DIGI_SERDES_LINE_INTF, 
                                       dev_info->line_cage_id);
            /* send the port descriptions for system reservation */
            for (serdes_id = 0; serdes_id < dev_info->num_line_serdes_port; serdes_id++) 
            {
                rc |= digi_serdes_port_init(adpt_handle->digi_handle, 
                                            serdes_id, 
                                            dev_info->serdes_type[serdes_id], 
                                            dev_info->line_port_lane_tx[serdes_id],
                                            dev_info->line_port_lane_rx[serdes_id],
                                            dev_info->line_port_los_pin[serdes_id],
                                            dev_info->line_port_los_inv,
                                            NULL,
                                            &(adpt_handle->line_serdes[serdes_id]));
            }
        }
        ADPT_RETURN_VERIFY(rc);

        /* serdes initialization in system side */
        if (dev_info->num_sys_serdes_port > 0)
        {
            /* send the cage descriptions for system reservation */
            rc |= digi_serdes_cage_cfg(adpt_handle->digi_handle, 
                                       DIGI_SERDES_SYSTEM_INTF, 
                                       dev_info->sys_cage_id);
            /* send the port descriptions for system reservation */
            for (serdes_id = 0; serdes_id < dev_info->num_sys_serdes_port; serdes_id++) 
            {
                rc |= digi_serdes_port_init(adpt_handle->digi_handle, 
                                            dev_info->num_line_serdes_port + serdes_id, 
                                            DIGI_SERDES_SYSTEM_INTF, 
                                            dev_info->sys_port_lane_tx[serdes_id],
                                            dev_info->sys_port_lane_rx[serdes_id],
                                            dev_info->sys_port_los_pin[serdes_id],
                                            dev_info->sys_port_los_inv,
                                            NULL,
                                            &(adpt_handle->sys_serdes[serdes_id]));
            }
        }
        ADPT_RETURN_VERIFY(rc);



        /* static SerDes adaptation configuraiton */
        if (dev_info->static_serdes_adpt)
        {
            serdes_cfg = (digi_serdes_cfg_t*)PMC_CALLOC(sizeof(digi_serdes_cfg_t));

            /* convert file */
            rc |= adpt_serdes_cfg_file_parse(dev_id, 
                                             (char*)adpt_serdes_csv_name_get(dev_id), 
                                             serdes_cfg);
            /* set serdes configuration */
            if (PMC_SUCCESS == rc)
            {
                rc = adpt_serdes_cfg_set(dev_id, serdes_cfg);
            }

            PMC_FREE(&serdes_cfg);
            ADPT_RETURN_VERIFY(rc);
        }

        /* enable ETRANS algorithm */
        digi_mapotn_etrans_scheduling_mode_set(adpt_handle->digi_handle, TRUE);

        /* device bringup and interlaken initialization */
        switch (dev_info->sifd_type)
        {
            case ILKN_SIFD_1:
                rc |= digi_device_init(adpt_handle->digi_handle, 
                                       &(dev_info->sifd_1_cfg), 
                                       NULL, 
                                       dev_info->card_type, 
                                       dev_info->mux_stage,
                                       &(adpt_handle->ilkn_1_ctxt),
                                       &(adpt_handle->ilkn_2_ctxt));
                break;
                
            case ILKN_SIFD_2:
                rc |= digi_device_init(adpt_handle->digi_handle, 
                                       NULL, 
                                       &(dev_info->sifd_2_cfg),
                                       dev_info->card_type, 
                                       dev_info->mux_stage,
                                       &(adpt_handle->ilkn_1_ctxt), 
                                       &(adpt_handle->ilkn_2_ctxt));
                break;
                
            case ILKN_SIFD_1_2:
                rc |= digi_device_init(adpt_handle->digi_handle, 
                                       &(dev_info->sifd_1_cfg), 
                                       &(dev_info->sifd_2_cfg),
                                       dev_info->card_type, 
                                       dev_info->mux_stage,
                                       &(adpt_handle->ilkn_1_ctxt), 
                                       &(adpt_handle->ilkn_2_ctxt));
                break;
                
            case ILKN_SIFD_NONE:
                rc |= digi_device_init(adpt_handle->digi_handle, 
                                       NULL, 
                                       NULL,
                                       dev_info->card_type, 
                                       dev_info->mux_stage,
                                       &(adpt_handle->ilkn_1_ctxt), 
                                       &(adpt_handle->ilkn_2_ctxt));
                break;

            default:
                ADPT_ERROR("Wrong SIFD type[%d].\r\n", dev_info->sifd_type);
                rc = PMC_ERR_FAIL;
                break;
        }
        ADPT_RETURN_VERIFY(rc);

        /* interlaken table initialization */
        rc |= digi_ilkn_chnl_cfg_lock(adpt_handle->digi_handle);
        rc |= digi_ilkn_chnl_cfg_unlock(adpt_handle->digi_handle);
        ADPT_RETURN_VERIFY(rc);
    }
    /* warm reboot */
    else
    {
        /* initialize DIGI instance */
        digi_handle = digi_ctxt_restart_create(ctxt_ptr, adpt_handle->sys_handle);
        ADPT_PARA_VERIFY(digi_handle);

        if ((DIGI_INIT_WARM_FW_RELOAD == init_type) || (DIGI_INIT_CRASH_FW_RELOAD == init_type))
        {
            /* configure firmware file name */
            if (NULL != dev_info->fw_filename)
            {
                digi_fw_filename_set(digi_handle, dev_info->fw_filename);
            }

            /* FW reload */
            rc = digi_fw_reload(digi_handle); 
            ADPT_RETURN_VERIFY(rc);
        }

        if ((DIGI_INIT_WARM == init_type) || (DIGI_INIT_WARM_FW_RELOAD == init_type))
        {
            rc = digi_handle_restart_init(digi_handle, TRUE);
        }
        else
        {
            rc = digi_handle_restart_init(digi_handle, FALSE);
        }

        if (PMC_SUCCESS != rc)
        {
            if (DIGI_WARM_CTXT_COPY == dev_info->warm_type)
            {
                /* destroy digi handle */
                digi_ctxt_destroy(&digi_handle);
            }
            /* destroy sys handle */
            pmc_sys_handle_destroy(&(adpt_handle->sys_handle));
            /* destory Adapter container handle */
            adpt_handle_destroy(dev_id, &adpt_handle);
        }
        ADPT_RETURN_VERIFY(rc);

        /* SDK handle update */
        rc = adpt_ctxt_handle_update(adpt_handle, dev_info, digi_handle);
        ADPT_RETURN_VERIFY(rc);

        /* static SerDes adaptation configuraiton */
        if (dev_info->static_serdes_adpt)
        {
            serdes_cfg = (digi_serdes_cfg_t*)PMC_CALLOC(sizeof(digi_serdes_cfg_t));

            /* convert file */
            rc |= adpt_serdes_cfg_file_parse(dev_id, 
                                             (char*)adpt_serdes_csv_name_get(dev_id), 
                                             serdes_cfg);
            /* set serdes configuration */
            if (PMC_SUCCESS == rc)
            {
                rc = adpt_serdes_cfg_set(dev_id, serdes_cfg);
            }

            PMC_FREE(&serdes_cfg);
            ADPT_RETURN_VERIFY(rc);
        }

        /* interrupt type */
        rc = digi_int_init(adpt_handle->digi_handle,
                           dev_info->int_type,
                           (CPUIF_INT_DISABLE != dev_info->int_type) ? TRUE : FALSE);
    }
    ADPT_RETURN_VERIFY(rc);

    /* PMON initialization process */
    /* create memory pool handle */
    rc = digi_pmon_shared_mem_size_get(adpt_handle->digi_handle, &buf_size);
    pool = pmc_mem_pool_create(adpt_handle->sys_handle, 
                               buf_size, 1, PMC_MEM_POOL_TYPE_SHARED, 
                               PMC_MEM_POOL_ALIGN_PMON);
    /* allocate memory. The return of this function is virtual address */
    buf_addr_vir = (PMC_UINTPTR)pmc_mem_pool_alloc(pool, buf_size);
    /* convert virtual address to physical address for FW */
    buf_addr_phy = pmc_convert_mem_pool_addr_to_physical_addr(pool, (void*)buf_addr_vir);

    /* set up PMON collection mode */
    rc |= digi_pmon_collection_mode_cfg(adpt_handle->digi_handle,
                                        dev_info->pmon_trig_mode,
                                        FALSE,
                                        dev_info->trig_period,
                                        buf_addr_phy,
                                        (void*)buf_addr_vir,
                                        buf_size / 4,
                                        1);
    ADPT_RETURN_VERIFY(rc);

    /* set up PMON work order */
    pmon_order.enable_hw_block_msk  = dev_info->pmon_work_order;
    pmon_order.disable_hw_block_msk = 0;
    rc = digi_pmon_work_order_cfg(adpt_handle->digi_handle, &pmon_order);
    ADPT_RETURN_VERIFY(rc);

    /* set up interrupt callbacks */
    /* performance register */
    rc = digi_pmon_int_register_cb(adpt_handle->digi_handle, NULL, 0, adpt_int_cb_handle);
    ADPT_RETURN_VERIFY(rc);
    /* ENET defect register */
    rc = digi_enet_line_int_register_cb(adpt_handle->digi_handle, NULL, 0, adpt_int_cb_handle);
    ADPT_RETURN_VERIFY(rc);
    rc = digi_enet_sys_int_register_cb(adpt_handle->digi_handle, NULL, 0, adpt_int_cb_handle);
    ADPT_RETURN_VERIFY(rc);
    /* OTN defect register */
    rc = digi_otn_int_register_cb(adpt_handle->digi_handle, NULL, 0, adpt_int_cb_handle);
    ADPT_RETURN_VERIFY(rc);
    /* SIFD defect register */
    rc = digi_sifd_int_register_cb(adpt_handle->digi_handle, NULL, 0, adpt_int_cb_handle);
    ADPT_RETURN_VERIFY(rc);
    /* CBR defect register */
    rc = digi_cbr_int_register_cb(adpt_handle->digi_handle, NULL, 0, adpt_int_cb_handle);
    ADPT_RETURN_VERIFY(rc);
    /* MAPPER defect register */
    rc = digi_mapper_int_register_cb(adpt_handle->digi_handle, NULL, 0, adpt_int_cb_handle);
    ADPT_RETURN_VERIFY(rc);

    /* record information */
    if (PMC_SUCCESS == rc)
    {
        adpt_handle->dev_info      = dev_info;
        adpt_handle->pmon_buf_addr = buf_addr_vir;
        adpt_handle->pmon_pool     = pool;
        adpt_handle->init_state    = DIGI_INIT_DONE;
    }

    PMC_RETURN(rc);
} /* adpt_init */


/*******************************************************************************
* adpt_uninit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function destroys the digi handle and system handle. 
*
* INPUTS:
*   dev_id             - device identity.
*   shutdown           - TRUE: shutdown the power of DIGI.
*                        FALSE: don't shutdown the power of DIGI.
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
PUBLIC PMC_ERROR adpt_uninit(UINT32 dev_id, BOOL shutdown)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d].\r\n", dev_id, shutdown);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_PARA_VERIFY(adpt_handle);

    /* disable interrupt monitoring */
    rc = digi_int_init(adpt_handle->digi_handle, CPUIF_INT_DISABLE, FALSE);

    /* shutdown DIGI device */
    if (shutdown)
    {
        digi_device_shutdown(adpt_handle->digi_handle);
    }

    /* destroy PMON memory pool */
    if (0 != adpt_handle->pmon_buf_addr)
    {
        pmc_mem_pool_free(adpt_handle->pmon_pool,
                          (void**)&(adpt_handle->pmon_buf_addr));
    }

    if (shutdown || (DIGI_WARM_CTXT_COPY == adpt_handle->dev_info->warm_type))
    {
        /* destroy digi handle */
        digi_ctxt_destroy(&(adpt_handle->digi_handle));
    }
    /* destroy sys handle */
    pmc_sys_handle_destroy(&(adpt_handle->sys_handle));
    /* destory Adapter container handle */
    adpt_handle_destroy(dev_id, &adpt_handle);

    PMC_RETURN(rc);
} /* adpt_uninit */


/*******************************************************************************
* adpt_version_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve the following information:
*   device ID
*   SDK version
*   firmware version
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   *ver_info          - pointer to a status structure containing device ID,
*                        SDK version and firmware version.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_version_get(UINT32 dev_id, digi_version_t *ver_info)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_sw_status_t sw_status;
    digi_fw_status_t fw_status;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p].\r\n", dev_id, ver_info);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    PMC_MEMSET((void*)&sw_status, 0, sizeof(digi_sw_status_t));    
    /* get chip status inforamtion */
    rc |= digi_software_status_get(adpt_handle->digi_handle, &sw_status);
    PMC_MEMSET((void*)&fw_status, 0, sizeof(digi_fw_status_t));    
    /* get firmware status information */
    rc |= digi_firmware_status_get(adpt_handle->digi_handle, &fw_status);

    if (PMC_SUCCESS == rc)
    {
        ver_info->device_id      = sw_status.device_id;
        ver_info->sdk_version    = sw_status.sw_version;
        ver_info->firm_version   = fw_status.fw_version;
        ver_info->device_version = (UINT32)sw_status.device_revision_code;
        ver_info->boot_version   = fw_status.bootloader_version;
        ver_info->firm_build     = fw_status.fw_build;
    }

    PMC_RETURN(rc);
} /* adpt_version_get */


/*******************************************************************************
* adpt_device_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the device, firmware and bootloader status. 
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   *sw_status         - pointer to software SDK status.
*   *fw_status         - pointer to firmware and bootloader status.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_device_status_get(UINT32 dev_id, 
                                    digi_sw_status_t *sw_status,
                                    digi_fw_status_t *fw_status)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(sw_status);
    ADPT_PARA_VERIFY(fw_status);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get chip status inforamtion */
    rc = digi_software_status_get(adpt_handle->digi_handle, sw_status);
    ADPT_RETURN_VERIFY(rc);

    /* get firmware status information */
    rc = digi_firmware_status_get(adpt_handle->digi_handle, fw_status);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_device_status_get */


/*******************************************************************************
* adpt_warm_restart_handle
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Save the data structure of DIGI Adapter and DIGI SDK for warm restart
*   functionality. 
*
* INPUTS:
*   dev_id             - device identity.
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
*   Before warm restart for context copy type, this API should be called first. 
*   During this operation, other behaviour should be forbidden.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_warm_restart_handle(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 data_size = 0;
    void *data_ptr = NULL;
#ifdef PMC_COMEXPRESS
    char file_name[256];
#else
    char file_name[ADPT_FILE_NAME_LEN+7];
#endif
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x].\r\n", dev_id);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    PMC_MEMSET((void*)&file_name, '\0', sizeof(file_name));

    /* save DIGI SDK database */
    /* file name */
    PMC_SPRINTF((char*)&file_name, "%s_sdk%d", 
                (char*)&(adpt_handle->dev_info->file_name), 
                DIGI_DIGI_DEV_GET(dev_id));
    /* get storage memory and memory size */
    rc = digi_warm_restart_context_get(adpt_handle->digi_handle, &data_size, &data_ptr);
    ADPT_RETURN_VERIFY(rc);

    /* save DIGI Adapter database */
    /* file name */
#ifdef PMC_COMEXPRESS
    PMC_SPRINTF((char*)&file_name, "%s_adpt%d_%s.bin", 
                (char*)&(adpt_handle->dev_info->file_name), 
                DIGI_DIGI_DEV_GET(dev_id), sv_host_str);

    if (NULL != getenv("PMC_CTXT_FILENAME_NO_HOSTNAME")) 
    {
        PMC_SPRINTF((char*)&file_name, "%s_adpt%d.bin", 
                    (char*)&(adpt_handle->dev_info->file_name), 
                    DIGI_DIGI_DEV_GET(dev_id));
    } 
#else
    PMC_SPRINTF((char*)&file_name, "%s_adpt%d", 
                (char*)&(adpt_handle->dev_info->file_name), 
                DIGI_DIGI_DEV_GET(dev_id));
#endif

    /* save database */
    rc = adpt_database_save((char*)&file_name, sizeof(digi_cfg_container_t), (void*)adpt_handle);

    PMC_RETURN(rc);
} /* adpt_warm_restart_handle */


/*******************************************************************************
* adpt_reg_read
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrives the DIGI device register value.
*
* INPUTS:
*   dev_id             - device identity.
*   addr               - register address.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32             - register value.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 adpt_reg_read(UINT32 dev_id, UINT32 addr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    if (PMC_SUCCESS != (rc))
    {
        ADPT_ERROR("Input wrong dev_id[%d].\r\n", dev_id);
        return 0xFFFFFFFF;
    }

    return pmc_sys_reg_read(adpt_handle->sys_handle, addr);
} /* adpt_reg_read */


/*******************************************************************************
* adpt_reg_write
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the DIGI device register value.
*
* INPUTS:
*   dev_id             - device identity.
*   addr               - register address.
*   value              - register value.
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
PUBLIC PMC_ERROR adpt_reg_write(UINT32 dev_id, UINT32 addr, UINT32 value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    if (PMC_SUCCESS != (rc))
    {
        ADPT_ERROR("Input wrong dev_id[%d].\r\n", dev_id);
        PMC_RETURN(rc);
    }

    pmc_sys_reg_write(adpt_handle->sys_handle, addr, value);

    PMC_RETURN(rc);
} /* adpt_reg_write */


/*
** End of file
*/
