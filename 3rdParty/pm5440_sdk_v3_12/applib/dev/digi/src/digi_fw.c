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
*   DESCRIPTION : This module contains all misc hostmsg methods.
*
*   NOTES:
*
*******************************************************************************/

#include "digi_loc.h"
#include "digi_resource_loc.h"
#include "hostmsg_loc.h"

/* CMD_REQ_INFO_Get_Status and RSP_REQ_INFO_Get_Status message lengths */
#define HOSTMSG_CMD_REQ_INFO_GET_STATUS_LEN                 0x03
#define HOSTMSG_RSP_REQ_INFO_GET_STATUS_LEN                 0x03
/* CMD_REQ_INFO_Get_Version_Info and RSP_REQ_INFO_Get_Version_Info message lengths */
#define HOSTMSG_CMD_REQ_GET_VERSION_INFO_LEN                0x03
#define HOSTMSG_RSP_REQ_GET_VERSION_INFO_REV_ONLY_LEN       0x03
/* RSP_REQ_INFO_Get_Version_Info message length for getting both revision and build number*/
#define HOSTMSG_RSP_REQ_GET_VERSION_INFO_REV_AND_BUILD_LEN  0x04
#define HOSTMSG_RSP_REQ_GET_VERSION_INFO_REV_AND_BUILD_LEN_LEGACY_MODE  0x02 
/* CMD_CFG_INFO_FW_Shutdown and RSP_CFG_INFO_FW_Shutdown message lengths */
#define HOSTMSG_CMD_CFG_INFO_FW_SHUTDOWN_LEN                0x03
#define HOSTMSG_RSP_CFG_INFO_FW_SHUTDOWN_LEN                0x02
/* CMD_CFG_INFO_SPRAM_PCIe_Write_Enable message length */
#define HOSTMSG_CMD_CFG_INFO_SPRAM_PCIE_WRITE_EN_LEN        0x03
/* CMD_CFG_INFO_Get_SPRAM_SW_Reserved_Addr message length */
#define HOSTMSG_CMD_REQ_GET_SPRAM_SW_RESERVED_ADDR_LEN      0x03
#define HOSTMSG_RSP_REQ_GET_SPRAM_SW_RESERVED_ADDR_LEN      0x04
/* CMD_REQ_PERFORMANCE_Init_fw_performance_queue */
#define HOSTMSG_CMD_REQ_INIT_FW_PERFORMANCE_QUEUE_GET_SPRAM_ADDR_LEN 0x4
#define HOSTMSG_RSP_REQ_INIT_FW_PERFORMANCE_QUEUE_GET_SPRAM_ADDR_LEN 0x4
/* CMD_RSP_CMF_OPU_CSF_STATUS */
#define HOSTMSG_CMD_REQ_OPU_CSF_STATUS_LEN 0x3 
#define HOSTMSG_RSP_REQ_OPU_CSF_STATUS_LEN 0x5

#define DIGI_FW_BOOT_PASS                  0x0
#define DIGI_FW_BOOT_FAIL                  0x1
#define DIGI_FW_BOOT_NOT_DONE              0x0
#define DIGI_FW_BOOT_DONE                  0x1


#define DIGI_FW_REVB_3_04_RELEASE_REV         0x03040002
#define DIGI_FW_REVB_3_04_RESTART_BUILD_NUM   0x20131204
#define DIGI_FW_REVB_3_06_RELEASE_REV         0x03060002
#define DIGI_FW_REVB_3_06_RELEASE_REV_MASK    0xFFFF00FF
#define DIGI_FW_REVB_3_06_RESTART_BUILD_NUM   0x00042559
#define DIGI_FW_REVB_BUILD_DEVEL_MAGIC        0x01234567
#define DIGI_FW_REVB_3_05_RELEASE_REV         0x03050001
#define DIGI_FW_REVB_MAJOR                    0x03000000
#define DIGI_FW_REVA_MAJOR                    0x02000000
#define DIGI_FW_REVA_2_09_RELEASE_REV         0x02090001


/* 
 ** Forward References 
 */
#ifndef DOXYGEN_PUBLIC_ONLY
PRIVATE PMC_ERROR digi_fw_open_file_write_pcie(digi_handle_t *digi_handle, 
                                               BOOL8 skip_range,
                                               UINT32 from_addr,
                                               UINT32 to_addr);
PRIVATE PMC_ERROR digi_fw_info_status_get(digi_handle_t *digi_handle,
                                          digi_fw_info_request_t info_req,
                                          UINT32 *stat_type_ptr,
                                          UINT32 *stat_code_ptr);
PRIVATE PMC_ERROR digi_fw_spram_pcie_write_enable(digi_handle_t *digi_handle);
PRIVATE PMC_ERROR digi_fw_intr_monitor_restart(digi_handle_t *digi_handle);

PUBLIC BOOL digi_fw_is_in_reset(digi_handle_t *digi_handle);
PUBLIC BOOL digi_fw_is_running(digi_handle_t *digi_handle);
PRIVATE BOOL8 digi_fw_has_restart_support(digi_handle_t *digi_handle);
PRIVATE BOOL digi_fw_is_ver_3_04_restart(digi_handle_t *digi_handle);
PUBLIC PMC_ERROR digi_fw_migrate_3_04_restart(digi_handle_t *digi_handle);

/*******************************************************************************
* digi_fw_info_status_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  This method gets firmware status.
*
* INPUTS:
*  digi_handle - DIGI device handle.
*  info_req       - Info Request\n
*                     DIGI_FW_INFO_REQ_FW_STATUS: request status information\n
*                     DIGI_FW_INFO_REQ_FW_VERSION: request FW version check \n
*                                                  information
*
* OUTPUTS:
*  stat_type_ptr - pointer to storage for returned status type.\n
*                  This value should be matched with info_req\n
*                  value.
*  stat_code_ptr - pointer to storage for returned status code.\n
*                  When info_req = DIGI_FW_INFO_REQ_FW_STATUS\n
*                    DIGI_FW_STATUS_ALIVE: Firmware is alive, no error\n
*                  When info_req = DIGI_FW_INFO_REQ_FW_VERSION\n
*                    DIGI_FW_VERSION_SUPPORTED: Firmware version supported on \n
*                                               current device revision\n
*                    DIGI_FW_VERSION_NOT_SUPPORTED: Firmware version not \n
*                                               supported on current device \n
*                                               revision\n
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES: CMD_REQ_INFO_Get_Status message.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_fw_info_status_get(digi_handle_t *digi_handle,
                                          digi_fw_info_request_t info_req,
                                          UINT32 *stat_type_ptr,
                                          UINT32 *stat_code_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;        
    UINT32 data[1];
    UINT32 response[1];

    PMC_ENTRY();

    PMC_ASSERT((NULL != stat_code_ptr) && (NULL != stat_type_ptr),
               HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Add message body to the previously created header. */
    /* Fill stat_ctrl field. */
    data[0] = (UINT32)info_req;    

    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_fixed_length_query((pmc_handle_t *)digi_handle,MODID_INFO_DBG,HOSTMSG_CMD_INFO_GET_STATUS_ST,data,1,response,1,stat);
    }

    if (PMC_SUCCESS == result)
    {
        PMC_LOG(PMC_LOG_SEV_LOW, DIGI_LOG_OBND_DATA, response[0], 0,
                "data[2] = %x\n", response[0]);

        /* Return requested data. */
        *stat_code_ptr = (response[0] >> 16) & 0x0000FFFF;
        *stat_type_ptr = response[0] & 0x0000FFFF;
    }

    PMC_RETURN(result);

} /* digi_fw_info_status_get*/

/*******************************************************************************
* digi_fw_spram_pcie_write_enable
* ___________________________________________________________________________
*
* DESCRIPTION:
*  This method triggers FW to enable PCIe write access to the entire SPRAM.  
*  This host message should only be called when the host software wants to 
*  overwrite the firmware image in the SPRAM. During normal operation, 
*  PCIe write access is only enabled for the 
*  debug data, host message mailboxes and the software context sections of SPRAM.  
*  
*  If a failure is reported by this function call, then the caller function
*  should assume FW has failed unexpected and the regular FW recovery mechanism
*  should be used.
*
* INPUTS:
*  digi_handle - DIGI device handle.
*
* OUTPUTS: None.
*
*  RETURNS:
*    PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
*  NOTES: CMD_CFG_INFO_SPRAM_PCIe_Write_Enable
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_fw_spram_pcie_write_enable(digi_handle_t *digi_handle)
{
    UINT32 data[1] = {0}; /* reserved for future use */
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

#if  defined(PMC_SW_SIMULATION)
    PMC_RETURN(PMC_SUCCESS);
#endif
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message(&digi_handle->base,
                                          MODID_INFO_DBG,
                                          HOSTMSG_CMD_INFO_SPRAM_PCIE_WRITE_EN_ST,
                                          data,
                                          1,
                                          stat);
    }

    PMC_RETURN(result);

} /* digi_fw_spram_pcie_write_enable */


/*******************************************************************************
* digi_fw_pathname_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the path name on disc where the driver is to look for the
*   firmware image.
*
* INPUTS:
*   digi_handle    - DIGI device handle.
*   *pathname_ptr  - path name.
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
PUBLIC void digi_fw_pathname_set(digi_handle_t  *digi_handle,
                                 char const     *pathname_ptr)
{
    UINT32 pathname_len = 0;
    PMC_ENTRY();

    PMC_ASSERT((digi_handle != NULL), DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT((pathname_ptr != NULL), DIGI_ERR_INVALID_ARG, 0, 0);

    if (digi_handle->var.fw_pathname != NULL) 
    {
        PMC_FREE(&digi_handle->var.fw_pathname);
    }

    pathname_len = PMC_STRLEN(pathname_ptr);

    /* PMC_CALLOC will assert on OOM, so it is safe to continue */
    digi_handle->var.fw_pathname = (char*)PMC_CALLOC(sizeof(char) * (pathname_len + 2));

    PMC_MEMCPY(digi_handle->var.fw_pathname, pathname_ptr, pathname_len);

    if(pathname_ptr[pathname_len-1] != '/') {
        /* Add '/' at the end */
        digi_handle->var.fw_pathname[pathname_len] = '/';
    }

    PMC_RETURN();
} /* digi_fw_pathname_set */

/*******************************************************************************
* digi_fw_filename_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the location on disc where the driver is to look for the
*   firmware image.
*
* INPUTS:
*   digi_handle    - DIGI device handle.
*   *filename_ptr  - path to binary file.
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
PUBLIC void digi_fw_filename_set(digi_handle_t *digi_handle,
                                 char const* filename_ptr)
{
    UINT32 filename_len = 0;
    PMC_ENTRY();

    PMC_ASSERT((digi_handle != NULL), DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT((filename_ptr != NULL), DIGI_ERR_INVALID_ARG, 0, 0);

    if (digi_handle->var.fw_filename != NULL) 
    {
        PMC_FREE(&digi_handle->var.fw_filename);
    }

    filename_len = PMC_STRLEN(filename_ptr);

    /* PMC_CALLOC will assert on OOM, so it is safe to continue */
    digi_handle->var.fw_filename = (char*)PMC_CALLOC(sizeof(char) * (filename_len + 1));

    PMC_MEMCPY(digi_handle->var.fw_filename, filename_ptr, filename_len);

    PMC_RETURN();
} /* digi_fw_filename_set */

/*******************************************************************************
* digi_fw_download_from_hard_reset
* ___________________________________________________________________________
*
* DESCRIPTION:
* This function downloads the firmware image to SPRAM after a device hard
* reset. This function must be called at the beginning of the devcie start-up
* before any communication can be established with the firmware inside the
* device. In addition, the function digi_fw_filename_set() must be called to
* setup the location of the firmware image prior to calling this function.
*
* INPUTS:
*   digi_handle    - DIGI device handle.
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
PUBLIC PMC_ERROR digi_fw_download_from_hard_reset(digi_handle_t *digi_handle)
{
    UINT32 boot_info = 0;
    hostmsg_handle_t *cpup34k_handle = NULL;
    PMC_ERROR result = DIGI_ERR_UNKNOWN;

    PMC_ENTRY();

    /* use a hostmsg handle to access cpup34k block  */        
    cpup34k_handle = (hostmsg_handle_t *)(digi_handle->base.common_info->hostmsg_handle);
    PMC_ASSERT((NULL != cpup34k_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* This wait time is to ensure boot loader has configured the PCIe
     * interface properly. Otherwise, the register access from host SW will
     * fail. */
    PMC_Q_USLEEP(digi_handle,DIGI_FW_PRE_DOWNLOAD_WAIT_PERIOD_IN_US);

    if (digi_handle->var.fw_filename == NULL) {

        /* Set the default filename */
        digi_fw_filename_set(digi_handle, DIGI_FW_DOWNLOAD_DEFAULT_FILE_NAME);
    }

    /* Check that boot loader is ready before eloading FW */
    boot_info = cpup34k_field_BOOT_INFO_get(NULL, (pmc_handle_t *)cpup34k_handle);

    if (boot_info != DIGI_FW_POLL_BOOT_ENABLE) 
    {

        result = DIGI_ERR_FW_BOOT_PCIE_INIT_FAIL;

    } else if (digi_fw_open_file_write_pcie(digi_handle, FALSE, 0, 0) != PMC_SUCCESS) {

        result = DIGI_ERR_FW_DOWNLOAD_FAIL;

    } else {
        /* set cold start flag */
        cpup34k_field_APP_SCRATCH6_set(NULL,(pmc_handle_t *)cpup34k_handle, FW_COLD_START_MAGIC_ID);

        /* Set BOOT_JMP_ADDR_REG to the supplied value */
        cpup34k_field_BOOT_JMP_ADDR_set(NULL, (pmc_handle_t *)cpup34k_handle, DIGI_FW_JMP_OFFSET);

        /* Set BOOT_ENABLE to start code execution */
        cpup34k_field_BOOT_ENABLE_set(NULL, (pmc_handle_t *)cpup34k_handle, 1);

        /* Wait for FW startup to complete */
        PMC_Q_USLEEP(digi_handle,DIGI_FW_POST_DOWNLOAD_WAIT_PERIOD_IN_US);

        result = PMC_SUCCESS;
    }
    
    PMC_RETURN(result);
} /* digi_fw_download_from_hard_reset */

/*******************************************************************************
* digi_fw_stats_init
* ___________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the spram address of firmware statistics and various
*   debug locations.
*
* INPUTS:
*   digi_handle    - DIGI device handle.
*
* OUTPUTS:
*   None.   
*
* RETURNS:
*   PMC_SUCCESS - on success
*
* NOTES: 
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_stats_init(digi_handle_t *digi_handle)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

#if defined(DIGI_FW_STATS) && (!defined(PMC_SW_SIMULATION))
    /* get SPRAM addresses of firmware statistics */
    {
        digi_fw_stats_t *digi_fw_stats = &digi_handle->var.digi_pmon_mode_profile->digi_fw_stats;
        digi_fw_stats->app_log_last_sequence_number_displayed = 0;

        if (TRUE == pmc_is_digi_rev_a_revision(&digi_handle->base))
        {
            digi_fw_stats->display_firmware_stats = FALSE;
        }
        else
        {
            result = digi_fw_stats_get(digi_handle, digi_fw_stats);
            
            /* if this fails, don't display statistics, but don't error out, the _addr's are set to 0 on failure */
            if (result != PMC_SUCCESS)
              digi_fw_stats->display_firmware_stats = FALSE;
            else
              digi_fw_stats->display_firmware_stats = TRUE;
            result = PMC_SUCCESS;
        }
        result = digi_fw_opsa_mpmo_debug_addr_get(digi_handle,&digi_fw_stats->opsa_mpmo_debug_addr,&digi_fw_stats->opsa_mpmo_debug_size);
        if (result != PMC_SUCCESS)
        {
            PMC_LOG_TRACE("error calling digi_fw_opsa_mpmo_debug_addr_get\n");
        }
    }
#endif

    PMC_RETURN(result);
}
#endif /* DOXYGEN_PUBLIC_ONLY */

/*******************************************************************************
* digi_fw_reload
* ___________________________________________________________________________
*
* DESCRIPTION:
*   This function reloads the firmware when firmware is running
*      
*   Prior to using this function to reload new FW, ensure a previous version 
*   of FW is running and firmware file is available
*
* INPUTS:
*   digi_handle    - DIGI device handle.
*
* OUTPUTS:
*   None.   
*
* RETURNS:
*   PMC_SUCCESS - on success
*
* NOTES: 
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_reload(digi_handle_t *digi_handle)
{
    PMC_ERROR result = PMC_SUCCESS;
    BOOL8     fw_has_restart_support;
    BOOL8 is_opsa_update_required = FALSE;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Get firmware version prior the reload */
    fw_has_restart_support = digi_fw_has_restart_support(digi_handle);
    
    /* current firmware don't have firmware restart and is a revA build */
    if (FALSE  == fw_has_restart_support && digi_handle->digi_fw_status.fw_version >= DIGI_FW_REVB_MAJOR)
    {
        /* evenif firmware is not supported, we supported on a specific version with 
           some ajustment */
        if (FALSE == digi_fw_is_ver_3_04_restart(digi_handle))
        {
            result  = DIGI_ERR_FW_RESTART_NOT_SUPPORTED;
        } 
        else 
        {
            result = digi_fw_migrate_3_04_restart(digi_handle);
            is_opsa_update_required = TRUE;
        }
    }

    
    if (PMC_SUCCESS == result)
    {        
        /* unconditionally clear PMON/interrupt configuration */
        digi_resource_clean_int_pmon(digi_handle);
        
        /* shutdown firmware queue */

#ifndef DONT_USE_FW_Q     
        if (TRUE == digi_handle_init_uses_fw_q_flag)
        {
            digi_fw_performance_queue_init(digi_handle , FALSE );
        }
#endif
        /* use default file names if it has not been specified */
        if (NULL == digi_handle->var.fw_filename) {
            /* fw_filename has not been set by digi_fw_filename_set() previously yet, so use the default name */
            if (TRUE == pmc_is_digi_rev_a_revision(&digi_handle->base))
            {
                /* Set the default filename for the REV_A case */
                digi_fw_filename_set(digi_handle, DIGI_FW_DOWNLOAD_DEFAULT_FILE_NAME);
                PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0, "REV A being set. FileName=%s\n", digi_handle->var.fw_filename);
            }
            else
            {
                /*
                 * If this is not REV_A then use the REV_B FW file name
                 */
                digi_fw_filename_set(digi_handle, DIGI_FW_DOWNLOAD_REVB_FILE_NAME);
                PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0, "REV B being set. FileName=%s\n", digi_handle->var.fw_filename);
            }
        }
        
        /* reload firmware 
           FW context per module wil only be reloaded for a 
        */
        result = digi_fw_reload_internal(digi_handle, FALSE);
        if (PMC_SUCCESS == result) {
            result = digi_fw_stats_init(digi_handle);
        }

#ifndef DONT_USE_FW_Q
        /* restart firmware queue when it was enabled */
        if (PMC_SUCCESS == result && 
            TRUE == digi_handle_init_uses_fw_q_flag)
        {        
            digi_fw_performance_queue_init(digi_handle , TRUE );
        }
#endif
#ifdef DONT_USE_FW_Q        
        if (PMC_SUCCESS == result && 
            TRUE == digi_handle_init_uses_fw_q_flag) 
        { 
            result= digi_fw_performance_queue_init(digi_handle , FALSE );
        } 
#endif 

        if (PMC_SUCCESS == result && 
            FALSE == fw_has_restart_support)
        {
            /* restart CBRC/ENET monitor if they were enabled */
            digi_fw_intr_monitor_restart(digi_handle);
        }
    }
    /* update firmware status */
    if (PMC_SUCCESS == result)
    {
        digi_fw_status_t fw_status;
        result = digi_firmware_status_get(digi_handle,&fw_status);
    }

    /*
     * Now need to check if opsa_update_required flag is true and if this
     * version is now greater than 3.06. If so need to update FW OPSA,
     * equivalent of calling digi_fw_opsa_cfg_mpmo_port_add for all related
     * resources types without doing an actual add. Rather a CONFIG command
     * is set to configured the FW OPSA since the previous version, 3.04,
     * does not have this data.
     */
     if (PMC_SUCCESS == result &&
         TRUE == is_opsa_update_required &&
         digi_handle->digi_fw_status.fw_version > DIGI_FW_REVB_3_06_RELEASE_REV && 
         digi_handle->digi_fw_status.fw_build > DIGI_FW_REVB_3_06_RESTART_BUILD_NUM)
     {
         result = digi_opsa_fw_update(digi_handle);
     }


    PMC_ATOMIC_RETURN(digi_handle, result);

} /* digi_fw_reload */

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* digi_fw_poll_for_alive
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Helper function to poll the digi fw to see if it is alive or not
*
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS   - means firmw is alive and hostmsg is working
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_poll_for_alive(digi_handle_t *digi_handle)
{
#ifndef PMC_SW_SIMULATION
    UINT8  i = 0; /* counter */
#endif
    UINT32 stat_type = 0;
    UINT32 stat_code = 0;
    digi_fw_info_request_t info_req = DIGI_FW_INFO_REQ_FW_STATUS;
    PMC_ERROR result = DIGI_ERR_UNKNOWN;

    PMC_ENTRY();
#ifndef PMC_SW_SIMULATION
    /* Poll for status */
    while ( i < DIGI_FW_STATUS_POLL_TRY_MAX && result != PMC_SUCCESS)
    {
        result = digi_fw_info_status_get(digi_handle, info_req, &stat_type, &stat_code);
        if (PMC_SUCCESS == result)
        {
            if ((stat_type == (UINT32)info_req) && (stat_code == DIGI_FW_STATUS_ALIVE) )
            {
                result = PMC_SUCCESS;
                break;
            }
        }
        /* Sleep for 500 MS between tries */
        i++;
        PMC_Q_USLEEP(digi_handle,DIGI_FW_STATUS_POLL_INTERVAL_IN_US);
    }
#else
    if(0)
    {
        result = digi_fw_info_status_get(digi_handle, info_req, &stat_type, &stat_code);
    }
    result = PMC_SUCCESS;
#endif

    PMC_RETURN(result);
} /* digi_fw_poll_for_alive */

/*******************************************************************************
* digi_fw_is_in_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a helper function to determine if the mips core is in reset state
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   ONLY CAUSE_MIPS_SW_RST and MIPS_SW_RST are set to 1,it returns TRUE
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL digi_fw_is_in_reset(digi_handle_t *digi_handle)
{
    BOOL is_in_reset = FALSE;

    hostmsg_handle_t *cpup34k_handle = NULL;

    UINT8 cause_mips_sw_rst_val = 0;
    UINT8 mips_sw_rst_val = 0;

     PMC_ENTRY();

    /* use a hostmsg handle to access cpup34k block  */        
    cpup34k_handle = (hostmsg_handle_t *)(digi_handle->base.common_info->hostmsg_handle);
    PMC_ASSERT((NULL != cpup34k_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /*ONLY CAUSE_MIPS_SW_RST and MIPS_SW_RST are set to 1,it returns TRUE*/
    cause_mips_sw_rst_val = cpup34k_field_CAUSE_MIPS_SW_RST_get(NULL, (pmc_handle_t *)cpup34k_handle);
    mips_sw_rst_val = cpup34k_field_MIPS_SW_RST_get(NULL, (pmc_handle_t *)cpup34k_handle);
    if (cause_mips_sw_rst_val && mips_sw_rst_val) {
        is_in_reset = TRUE;
    }
    
    PMC_RETURN(is_in_reset);
} /* digi_fw_is_in_reset */

/*******************************************************************************
* digi_fw_is_running
* ___________________________________________________________________________
*
* DESCRIPTION:
* This function checks if the FW is currently running.
*
* INPUTS:
*   digi_handle    - DIGI device handle.
*
* OUTPUTS:
*   None.   
*
* RETURNS:
*   TRUE - FW is running
*   FALSE - FW is not running
*
* NOTES: 
*
*******************************************************************************/
PUBLIC BOOL digi_fw_is_running(digi_handle_t *digi_handle)
{
    UINT32 boot_info = 0;
    BOOL is_fw_running = FALSE;
    hostmsg_handle_t *cpup34k_handle = NULL;
    UINT32 inbound_mbox_size = 0;   
    UINT32 outbound_mbox_size = 0;   

    PMC_ENTRY();

    /* use a hostmsg handle to access cpup34k block  */        
    cpup34k_handle = (hostmsg_handle_t *)(digi_handle->base.common_info->hostmsg_handle);
    PMC_ASSERT((NULL != cpup34k_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Check BOOT_INFO to determine if FW is running */
    boot_info = cpup34k_field_BOOT_INFO_get(NULL, (pmc_handle_t *)cpup34k_handle);

    /* 
     * If FW booted from cold reset or soft reset and mips core is not in reset,
     * then indicate that it is running.
     */
    if (!digi_fw_is_in_reset(digi_handle))
    {
        if ((DIGI_FW_BOOT_SUCCESS == boot_info) || (DIGI_FW_DETECT_SOFT_RESET == boot_info))
        {
            inbound_mbox_size = cpup34k_field_SIZE_INBOUND_MAILBOX_get(NULL, (pmc_handle_t *)digi_handle->base.common_info->hostmsg_handle);
            outbound_mbox_size = cpup34k_field_SIZE_OUTBOUND_MAILBOX_get(NULL, (pmc_handle_t *)digi_handle->base.common_info->hostmsg_handle);
    
            if (inbound_mbox_size == DIGI_FW_HOSTMSG_MAILBOX_SIZE 
                && outbound_mbox_size == DIGI_FW_HOSTMSG_MAILBOX_SIZE)
            {
                is_fw_running = TRUE;      
            }
        }        
    } 

    PMC_RETURN(is_fw_running);
} /* digi_fw_is_running */

/*******************************************************************************
* digi_fw_reload_internal
* ___________________________________________________________________________
*
* DESCRIPTION:
*   This function reloads the firmware when firmware is running
*      
*   Prior to using this function to reload new FW, ensure a previous version 
*   of FW is running and firmware file is available
*
* INPUTS:
*   digi_handle            - DIGI device handle.
*   load_fw_context        - FALSE: FW context sections are not loaded \n
*                            TRUE : FW context sections are loaded 
*
* OUTPUTS:
*   None.   
*
* RETURNS:
*   PMC_SUCCESS - on success
*
* NOTES: 
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_reload_internal(digi_handle_t *digi_handle,
                                         BOOL load_fw_context)
{

    PMC_ERROR result = PMC_SUCCESS;
    BOOL fw_filename_is_NULL = FALSE;
    pmc_handle_t *pmc_handle = &digi_handle->base;

    PMC_ENTRY();     
    
    /* First to check the firmware file name is not DIGI_FW_DOWNLOAD_NULL */
  
    if (PMC_STRLEN(digi_handle->var.fw_filename) == PMC_STRLEN(DIGI_FW_DOWNLOAD_NULL))
    {
        if (0==PMC_MEMCMP(digi_handle->var.fw_filename, DIGI_FW_DOWNLOAD_NULL, (PMC_STRLEN(DIGI_FW_DOWNLOAD_NULL)))) 
        {
           fw_filename_is_NULL = TRUE;
           result = DIGI_ERR_FW_UNAVAILABLE;
           PMC_LOG(PMC_LOG_SEV_HIGH, DIGI_ERR_FW_UNAVAILABLE, 0, 0,
                            "Digi firmware filename is not set, reload is failed. \n");       
        }
    }

    /*only firmware is running and firmware filename is set, we can call reload */
    if (PMC_SUCCESS == result)
    {
        if (FALSE == digi_fw_is_running(digi_handle))
        {
            result = DIGI_ERR_FW_IS_NOT_RUNNING;
             PMC_LOG(PMC_LOG_SEV_HIGH, DIGI_ERR_FW_IS_NOT_RUNNING, 0, 0,
               "Digi firmware is not running, reload is failed. \n"); 
        }
    }

    if (PMC_SUCCESS == result)
    {
        result = digi_fw_shutdown_and_put_mips_in_reset(digi_handle);
    }

    if (PMC_SUCCESS == result)
    {
        result = digi_fw_download_and_out_of_reset(digi_handle, load_fw_context);  
    }

    if (PMC_SUCCESS == result)
    {
        /* Clear outbound doorbell after FW completes initialization */
        hostmsg_clear_outbound_doorbell(pmc_handle);
        /*wait firmware startup is completed*/
        result = digi_fw_poll_for_alive(digi_handle);
    }
        
    PMC_RETURN(result);
} /* digi_fw_reload_internal */


/*******************************************************************************
* digi_fw_open_file_write_pcie
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Helper function to open the file and write the new image through the PCIE 
*
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   skip_range            - TRUE means that we are not writing the address 
*                           range from_addr to to_addr
*   from_addr             - lower boundary when we skip a range 
*   to_addr               - upper boundary when we skip a range 
*
* OUTPUTS:
*   None.
*
*  RETURNS:
*    PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_fw_open_file_write_pcie(digi_handle_t *digi_handle, 
                                               BOOL8 skip_range,
                                               UINT32 from_addr,
                                               UINT32 to_addr)
{
    FILE *f;
    pmc_sys_handle_t *hndl = NULL;
    UINT32 idx;
    UINT32 temp_val;
    UINT32 temp_val_inc;
    UINT32 file_pos_offset = 0;
    UINT32 burst_addr = CPU_SPRAM_OFFSET;
    UINT8 length_read;
    UINT8 burst_buffer[DIGI_FW_DOWNLOAD_BUFFER_SIZE_IN_WORDS * 4]; /* temporary buffer */
    char *full_path_name = NULL;
    UINT32 pathname_len = 0;
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();
    

    /* 
     * Check that PCIe write access to SPRAM is enabled. Firmware will restrict  
     * PCIe write access to SPRAM once firmware starts running.
     * PCIe write access to the entire SPRAM is enabled in
     * digi_fw_shutdown_and_put_mips_in_reset() using the call to
     * digi_fw_spram_pcie_write_enable().
     */
    if (PMC_SUCCESS == result) 
    {
        hndl = digi_handle->base.sys_handle;
        /* Write 10 32-bit words to SPRAM */
        temp_val = 0x01020304;
        temp_val_inc = 0x01010101;
        for (idx = 0; idx < 10; idx++) {
            pmc_sys_reg_write(hndl, CPU_SPRAM_OFFSET + (idx * 4), (temp_val + (idx*temp_val_inc)));
        }
        /* Read back and compare */
        for (idx = 0; idx < 10; idx++) {
            if (pmc_sys_reg_read(hndl, CPU_SPRAM_OFFSET + (idx * 4)) != (temp_val + (idx*temp_val_inc)))
            {
                result = DIGI_ERR_FW_SPRAM_PCIE_WRITE_FAIL;
            }
        }
        if (result != PMC_SUCCESS) {
            hostmsg_handle_t *hostmsg_handle = (hostmsg_handle_t *)(digi_handle->base.common_info->hostmsg_handle);

            /* Bring MIPS out of reset */
            cpup34k_field_MIPS_SW_RST_CLEAR_set(NULL, (pmc_handle_t *)hostmsg_handle, 1);

            PMC_Q_USLEEP(digi_handle,1000);

            /* try the spram test again */

            result = PMC_SUCCESS;
            /* Write 10 32-bit words to SPRAM */
            temp_val = 0x01020304;
            temp_val_inc = 0x01010101;
            for (idx = 0; idx < 10; idx++) {
                pmc_sys_reg_write(hndl, CPU_SPRAM_OFFSET + (idx * 4), (temp_val + (idx*temp_val_inc)));
            }
            /* Read back and compare */
            for (idx = 0; idx < 10; idx++) {
                if (pmc_sys_reg_read(hndl, CPU_SPRAM_OFFSET + (idx * 4)) != (temp_val + (idx*temp_val_inc)))
                {
                    result = DIGI_ERR_FW_SPRAM_PCIE_WRITE_FAIL;
                }
            }
        }
    }

    /* Open the file for reading */
    if (PMC_SUCCESS == result) 
    {
        if(digi_handle->var.fw_pathname != NULL) {
            /* Create full path name */
            pathname_len = PMC_STRLEN(digi_handle->var.fw_pathname);
            pathname_len += PMC_STRLEN(digi_handle->var.fw_filename);
            full_path_name = (char*)PMC_CALLOC(sizeof(char) * (pathname_len + 1));
            PMC_SPRINTF(full_path_name, "%s%s",
                        digi_handle->var.fw_pathname, 
                        digi_handle->var.fw_filename);
            
        } else {
            full_path_name = digi_handle->var.fw_filename;
        }
        
        f = pmc_sys_file_open(full_path_name);

        if (f == NULL) 
        {
          PMC_LOG(PMC_LOG_SEV_HIGH, DIGI_ERR_INVALID_ARG, 0, 0, "filestream is NULL. Filename=%s\n", digi_handle->var.fw_filename);
            result = DIGI_ERR_INVALID_ARG;
        }

        if(digi_handle->var.fw_pathname != NULL) {
            /* Free full path name memory */
            PMC_FREE(&full_path_name);
        }
    }
    
    /* read contents from file and write to SPRAM */
    if (PMC_SUCCESS == result) 
    {
        do 
        {
            /* read bytes from file - note length_read indicates max read and returns actual amount read */
            length_read = DIGI_FW_DOWNLOAD_BUFFER_SIZE_IN_WORDS * 4;
            result = pmc_sys_file_read(f, burst_buffer, &file_pos_offset, &length_read);

            /* increment file pointer position */
            file_pos_offset += length_read;

            if ((result == PMC_SUCCESS) && (length_read > 0)) 
            {
                if (skip_range == FALSE ||
                    (skip_range == TRUE && (burst_addr < from_addr || burst_addr >= to_addr)))
                {
                    /* Write the same amount we read to SPRAM */
                    digi_mem_write_internal(digi_handle, burst_addr, length_read, burst_buffer);
                }
                burst_addr += length_read;
            }
        } while ((result == PMC_SUCCESS) && (length_read > 0));

        /* close the file */
        pmc_sys_file_close(f);
    }

    PMC_RETURN(result);
} /* digi_fw_open_file_write_pcie */

/*******************************************************************************
* digi_fw_version_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  This method gets firmware revision number and build number.
*
* INPUTS:
*  digi_handle - DIGI device handle.
*  info_req    - Info Request\n
*                HOSTMSG_INFO_REQ_VER_GET_REV_NUM_ONLY: request FW revision number only\n
*                HOSTMSG_INFO_REQ_VER_GET_REV_AND_BUILD_NUM: request FW revision number and build number
*
* OUTPUTS:
*  fw_revision_number_ptr         - firmware revision number
*  fw_build_number_ptr            - firmware build number
*  bootloader_revision_number_ptr - bootloader revision number
*  bootloader_build_number_ptr    - bootloader build number
*
*  RETURNS:
*    PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
*  NOTES: CMD_REQ_INFO_Get_Version_Info.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_version_get(digi_handle_t *digi_handle,
                                     hostmsg_info_req_ver_t info_req,
                                     UINT32 *fw_revision_number_ptr,
                                     UINT32 *fw_build_number_ptr,
                                     UINT32 *bootloader_revision_number_ptr,
                                     UINT32 *bootloader_build_number_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;        
    UINT32 data[1];
    UINT32 response[4];
    UINT32 actual_response_len;

    *bootloader_revision_number_ptr = 0;
    *bootloader_build_number_ptr = 0;

    PMC_ENTRY();

    PMC_ASSERT((NULL != fw_revision_number_ptr) && (NULL != fw_build_number_ptr),
               HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Add message body to the previously created header. */
    /* Fill stat_ctrl field. */
    data[0] = (UINT32)info_req;
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_variable_length_query((pmc_handle_t *)digi_handle,MODID_INFO_DBG,HOSTMSG_CMD_INFO_GET_VERSION_ST,data,1,response,4,&actual_response_len,stat);
    }

    /* Get result*/
    if (PMC_SUCCESS == result)
    {

        /* Return revision number */
        *fw_revision_number_ptr = response[0];

        if (HOSTMSG_INFO_REQ_VER_GET_REV_AND_BUILD_NUM == info_req)
        {
#ifdef PMC_SW_SIMULATION
if(0)
{
#endif /* PMC_SW_SIMULATION */
            UINT32 expected_msg_len;
            /* Check if FW version is between 3.0 and 3.4 and 2.09 and 2.0 (rev_a) */
            if ( ((*fw_revision_number_ptr <= DIGI_FW_REVB_3_04_RELEASE_REV) && (*fw_revision_number_ptr >= DIGI_FW_REVB_MAJOR)) || 
                 ((*fw_revision_number_ptr <= DIGI_FW_REVA_2_09_RELEASE_REV) && (*fw_revision_number_ptr >= DIGI_FW_REVA_MAJOR)) )
            {
                expected_msg_len = 2;
            } else 
            {
                expected_msg_len = 4;
            }
            if (expected_msg_len != actual_response_len) 
            {
                result = HOSTMSG_FAILURE;
                PMC_LOG(PMC_LOG_SEV_HIGH, HOSTMSG_ERR_RSP_MSG_LEN,
                        expected_msg_len, actual_response_len,
                        "expected_msg_len=0x%x, actual_msg_len=%u\n",
                        expected_msg_len, actual_response_len);
            }            
        
            /* Return build number if needed */
            if (PMC_SUCCESS == result)
            {
                *fw_build_number_ptr   = response[1];
                if (expected_msg_len == HOSTMSG_RSP_REQ_GET_VERSION_INFO_REV_AND_BUILD_LEN_LEGACY_MODE)
                {
                    *bootloader_revision_number_ptr = 0;
                    *bootloader_build_number_ptr = 0;
                } else 
                {
                    *bootloader_revision_number_ptr = response[2];
                    *bootloader_build_number_ptr = response[3];
                }
            } 
#ifdef PMC_SW_SIMULATION
}
#endif /* PMC_SW_SIMULATION */
        }
    }

    PMC_RETURN(result);

}

/*******************************************************************************
* digi_fw_is_ver_3_04_restart
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a helper function to determine if this fw version supported to 
*   restart\n
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE ~ FW revision/build supports FW restart
*   FALSE ~ It is not supported
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL digi_fw_is_ver_3_04_restart(digi_handle_t *digi_handle)
{
    BOOL result = FALSE;

    PMC_ENTRY();

    /* Check that we are at the specific 3.04 revision and build where fw restart is supported */
    if ( digi_handle->digi_fw_status.fw_version == DIGI_FW_REVB_3_04_RELEASE_REV && 
         digi_handle->digi_fw_status.fw_build   == DIGI_FW_REVB_3_04_RESTART_BUILD_NUM)
    {     
        result = TRUE;
    }
    PMC_RETURN(result);

} /* digi_fw_is_ver_3_04_restart */

/*******************************************************************************
* digi_fw_is_ver_3_05
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a helper function to determine if the fw version is 3.05 or more recent\n
*   It is necessary as some of the hostmessage response lengths have changed since 3.05
*   release\n
*   It will also look at if this is a developer build after 3.05\n
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE ~ FW revision is 3.05.00.01
*   FALSE ~ FW revision is more recent than 3.05 or 3.05.xx.02
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL digi_fw_is_ver_3_05(digi_handle_t *digi_handle)
{

    UINT32 fw_ver;

    PMC_ENTRY();
    /* Get the FW version stored upon startup */
    fw_ver = digi_handle->digi_fw_status.fw_version;

    /* Check that we are not using 3.05.xx.01 release version
       We can use 3.05.xx.02 (dev FW) or 3.06 but must be backward compatible with 3.05 release */
    if ( fw_ver != (DIGI_FW_REVB_3_05_RELEASE_REV) )
    {
        PMC_RETURN(FALSE);
    }

    PMC_RETURN(TRUE);
} /* digi_fw_is_ver_3_05 */
#endif /* DOXYGEN_PUBLIC_ONLY */

/*******************************************************************************
* digi_fw_spram_sw_reserved_addr_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  This method gets the start address (physical) of the memory section in SPRAM
*  reserved for host software to use, as well as its size (in bytes). 
*
* INPUTS:
*  digi_handle - DIGI device handle.
*
* OUTPUTS:
*  sw_rsvd_addr_ptr - start address (physical) of SW reserved memory section\n
*                     NOTE: This is a 32-bit physical address within the DIGI\n
*                           device as viewed from the PCIe host.
*  sw_rsvd_size_ptr - size of SW reserved memory section (in bytes)
*
*  RETURNS:
*    PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
*  NOTES: CMD_REQ_INFO_Get_SPRAM_SW_Reserved_Addr.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_spram_sw_reserved_addr_get(digi_handle_t *digi_handle,
                                                    UINT32 *sw_rsvd_addr_ptr,
                                                    UINT32 *sw_rsvd_size_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 response[2];

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT((NULL != sw_rsvd_addr_ptr) && (NULL != sw_rsvd_size_ptr),
               HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_fixed_length_query((pmc_handle_t *)digi_handle,MODID_INFO_DBG,HOSTMSG_CMD_INFO_GET_SPRAM_SW_RESERVED_ADDR_ST,NULL,0,response,2,stat);
    }

    /* Get result*/
    if (PMC_SUCCESS == result)
    {
        /* Return SPRAM addr and size */
        *sw_rsvd_addr_ptr = response[0];
        *sw_rsvd_size_ptr = response[1];
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

}

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* digi_fw_shutdown_and_put_mips_in_reset
* ___________________________________________________________________________
*
* DESCRIPTION:
*  This method is a helper function used to shutdown the firmware and put
*  MIPS in reset state
* 
* INPUTS:
*  digi_handle - DIGI device handle.
*
*
* OUTPUTS: None.
*
*  RETURNS:
*    PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
*  NOTES: CMD_CFG_INFO_FW_Shutdown.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_shutdown_and_put_mips_in_reset(digi_handle_t *digi_handle)
{
    hostmsg_handle_t *hostmsg_handle = NULL;
    pmc_handle_t *pmc_handle = NULL;
    PMC_ERROR result = DIGI_ERR_UNKNOWN;

    PMC_ASSERT((digi_handle != NULL), DIGI_ERR_INVALID_ARG, 0, 0);
    pmc_handle = &digi_handle->base;
    PMC_ASSERT((pmc_handle != NULL), DIGI_ERR_INVALID_ARG, 0, 0);
    hostmsg_handle = (hostmsg_handle_t *)(pmc_handle->common_info->hostmsg_handle);
    PMC_ASSERT((hostmsg_handle != NULL), DIGI_ERR_INVALID_ARG, 0, 0);

    PMC_ENTRY();

    /* Enable PCIe write access to entire SPRAM before resetting MIPS */
    result = digi_fw_spram_pcie_write_enable(digi_handle);

    if (PMC_SUCCESS != result) 
    { 
        /* Unexpected failure. Failed to enable PCIe write access to entire SPRAM. */ 
        PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_FW_SPRAM_UNLOCK_FAIL, result, 0);
        
        /* Set result back to success since we will be resetting the MIPS */
        result = PMC_SUCCESS; 
    } else {
        result = digi_fw_shutdown(digi_handle); 
        if (PMC_SUCCESS != result) 
        { 
            /* Fail to shutdown firmware. This is unexpected.  Log the failure code return from digi_fw_shutdown() */ 
            PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_FW_FAILED_TO_SHUTDOWN, result, 0); 
        }
    }

    /* 
     * NOTE: We need to go ahead and put MIPS in reset even if we failed to 
     * enable the PCIe write access to the entire SPRAM or to shutdown FW.  
     * We will catch the error when we attempt to download the FW and find 
     * that SW failed to write to SPRAM through PCIe.
     * The failure to enable write access to SPRAM or shutdown FW
     * may be caused by FW getting stuck and the watchdog not kicking in.
     */

    /* According to FW documentation, need to sleep for 10 MS */
    PMC_Q_USLEEP(digi_handle,DIGI_FW_COALESCE_WAIT_PERIOD_IN_US);

    /* Place MIPS core in reset - Trigger the enabling of the RST bit */
    cpup34k_field_MIPS_SW_RESET_EN_set(NULL, (pmc_handle_t *)hostmsg_handle, 1);

    /* permit modification of NIC_SW_RST */
    cpup34k_field_NIC_SW_RST_EN_set(NULL, (pmc_handle_t *)hostmsg_handle, 1);



    /* Place MIPS core in reset */
    cpup34k_field_MIPS_SW_RST_set(NULL, (pmc_handle_t *)hostmsg_handle, 1);

    /* Soft reset for AMBA network interconnect (note: this bit auto-clears) */
    cpup34k_field_NIC_SW_RST_set(NULL, (pmc_handle_t *)hostmsg_handle, 1);
    
    /* Wait for reset to take effect (20 milliseconds) */
    PMC_Q_USLEEP(digi_handle,DIGI_FW_NIC_SW_RST_WAIT_IN_US);

    /* disable modification of NIC_SW_RST */
    cpup34k_field_NIC_SW_RST_EN_set(NULL, (pmc_handle_t *)hostmsg_handle, 0);

    PMC_RETURN(result);
} /* digi_fw_shutdown_and_put_mips_in_reset */

/*----------------------------------------------------------------------*/


/*******************************************************************************
* digi_fw_download_and_out_of_reset
* ___________________________________________________________________________
*
* DESCRIPTION:
*  This method is a helper function used to download the firmware download the 
*  firmware, clear the reset bit and then poll for fw status.
* 
* INPUTS:
*  digi_handle - DIGI device handle.
*  load_fw_context    - FALSE: FW context sections are not loaded \n
*                       TRUE : FW context sections are loaded
*
* OUTPUTS: None.
*
*  RETURNS:
*    PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
*  NOTES: CMD_CFG_INFO_FW_Shutdown.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_download_and_out_of_reset(digi_handle_t *digi_handle,
                                                   BOOL load_fw_context)
{
    hostmsg_handle_t *hostmsg_handle = NULL;
    pmc_handle_t *pmc_handle = NULL;
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    PMC_ASSERT((digi_handle != NULL), DIGI_ERR_INVALID_ARG, 0, 0);
    pmc_handle = &digi_handle->base;
    PMC_ASSERT((pmc_handle != NULL), DIGI_ERR_INVALID_ARG, 0, 0);
    hostmsg_handle = (hostmsg_handle_t *)(pmc_handle->common_info->hostmsg_handle);
    PMC_ASSERT((hostmsg_handle != NULL), DIGI_ERR_INVALID_ARG, 0, 0);

    PMC_ENTRY();


    result = digi_fw_open_file_write_pcie(digi_handle,FALSE, 0, 0 );

    if (result != PMC_SUCCESS) {

        result = DIGI_ERR_FW_DOWNLOAD_FAIL;

    } else {
        /* set restart flag */
        cpup34k_field_APP_SCRATCH6_set(NULL,(pmc_handle_t *)hostmsg_handle,load_fw_context == FALSE?FW_WARM_RESTART_MAGIC_ID:FW_COLD_START_MAGIC_ID);      


        /* Bring MIPS out of reset */
        cpup34k_field_MIPS_SW_RST_CLEAR_set(NULL, (pmc_handle_t *)hostmsg_handle, 1);
        /* Wait for FW startup to complete */
        PMC_Q_USLEEP(digi_handle,DIGI_FW_POST_DOWNLOAD_WAIT_PERIOD_IN_US);

        result = PMC_SUCCESS;  
    }
    
    PMC_RETURN(result);

} /* digi_fw_download_and_out_of_reset */

/*******************************************************************************
* digi_fw_shutdown
* ___________________________________________________________________________
*
* DESCRIPTION:
*  This method triggers FW to stop executing to be ready for a MIPS reset. 
* 
*  The caller of this function should wait for 10ms after receiving a positive 
*  acknowledgement from FW that it will shutdown before resetting the MIPS core.
*  This is the time required for FW to shutdown.
*
*  If a failure is reported by this function call, then the caller function
*  should assume FW has failed unexpected and the regular FW recovery mechanism
*  should be used.
*
* INPUTS:
*  digi_handle - DIGI device handle.
*
* OUTPUTS: None.
*
*  RETURNS:
*    PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
*  NOTES: CMD_CFG_INFO_FW_Shutdown.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_shutdown(digi_handle_t *digi_handle)
{
    PMC_ERROR result = PMC_SUCCESS; 
    UINT32 data[1];

    PMC_ENTRY();

#if  defined(PMC_SW_SIMULATION)
    PMC_RETURN(PMC_SUCCESS);
#endif
    /* Add message body to the previously created header. */
    /* The third word of CMD_CFG_INFO_FW_Shutdown is reserved*/
    data[0] = 0x0;

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_INFO_DBG,HOSTMSG_CMD_INFO_FW_SHUTDOWN_ST,data,1,stat);
    }

    PMC_RETURN(result);
} /* digi_fw_shutdown */


/*******************************************************************************
* digi_fw_enet_client_source_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function sends a host message to configure ENET client source: Line or Sys.
*
* INPUTS:
*   digi_handle       - pointer to DIGI handle instance.
*   cpb_port          - ENET CPB port 
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
PUBLIC PMC_ERROR digi_fw_enet_client_source_cfg(digi_handle_t *digi_handle,  
                                                util_global_cpb_port_t cpb_port)
{
    PMC_ERROR result = PMC_SUCCESS;
    BOOL8 is_enet_line = FALSE;
    UINT32 data[1];

    PMC_ENTRY();

    if(cpb_port == UTIL_GLOBAL_CPB_PORT_ENET_LINE)
    {
        is_enet_line = TRUE;
    } else if (cpb_port != UTIL_GLOBAL_CPB_PORT_ENET_SYS)
    {
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }

    data[0] = (UINT8)is_enet_line & 0x1;
    
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_ENET,HOSTMSG_CMD_CFG_ENET_SET_ENET_CLIENT_SOURCE_ST,data,1,stat);
    }

    if (PMC_SUCCESS == result) {
        digi_handle->var.digi_pmon_mode_profile->fw_enet_client_source.valid = TRUE;
        digi_handle->var.digi_pmon_mode_profile->fw_enet_client_source.is_enet_line = is_enet_line;
    }

    PMC_RETURN(result);
}/* digi_fw_enet_client_source_cfg */

/*******************************************************************************
* digi_fw_enet_client_source_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function sends a host message to get the configured ENET client source: \n
*   Line or Sys.
*
* INPUTS:
*   digi_handle          - pointer to DIGI handle instance.
*
* OUTPUTS:
*   *is_enet_line        - TRUE means ENET_LINE, FALSE means ENET_SYS 
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*   
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_enet_client_source_get(digi_handle_t *digi_handle, 
                                                BOOL8 *is_enet_line)
{
    PMC_ERROR result = PMC_SUCCESS;
    *is_enet_line = TRUE;
    UINT32 response[1];
    
    PMC_ENTRY();
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_fixed_length_query((pmc_handle_t *)digi_handle,MODID_ENET,HOSTMSG_CMD_CFG_ENET_GET_ENET_CLIENT_SOURCE_ST,NULL,0,response,1,stat);
    }
    /* Get result*/
    if (PMC_SUCCESS == result)
    {

        /* Return fw-based I-bit status */
        *is_enet_line = (response[0] == 1)?TRUE:FALSE;
        digi_handle->var.digi_pmon_mode_profile->fw_enet_client_source.valid = TRUE;
        digi_handle->var.digi_pmon_mode_profile->fw_enet_client_source.is_enet_line = *is_enet_line;
    }

    PMC_RETURN(result);
} /* digi_fw_enet_client_source_get */

/*******************************************************************************
* digi_fw_enet_eclass_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function sends a host message to initialize ENET ECLASS. 
*   ENET ECLASS routing filter outputs are disabled and hwired filter
*   configured to extract NO packets.
*
* INPUTS:
*   digi_handle          - pointer to DIGI handle instance.
*   datapath             - ENET datapath: Ingress or Egress
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
PUBLIC PMC_ERROR digi_fw_enet_eclass_init(digi_handle_t             *digi_handle,
                                          enet_dir_t                datapath)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];

    PMC_ENTRY();

    data[0] = datapath;
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_ENET,HOSTMSG_CMD_CFG_ENET_ECLASS_INIT_ST,data,1,stat);
    }

    PMC_RETURN(result);
}/* digi_fw_enet_eclass_init */

/*******************************************************************************
* digi_fw_eclass_hwire_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function sends a host message to configure ECLASS Hwire Filter.
*
* INPUTS:
*   digi_handle          - pointer to DIGI handle instance.
*   pkt_fwd_dst          - forwarding destination: DCPB, CPU or MGMT_FEGE
*   datapath             - ENET datapath direction
*   link                 - ENET Link ID (0 to 11)
*   link_enable          - Enable (TRUE) or disable (FALSE) the classification (using ECLASS block) for the ENET Link ID
*   extract_enable       - Enable (TRUE) or disable (FALSE) the extraction of the specified packet
*   pkt_type             - Packet type.
*   include_ts           - Specify if timestamp is included (TRUE) or not included (FALSE)
*   is_vlan_tagged       - TRUE when packet is VLAN-tagged, otherwise FALSE.
*                          Packet VLAN is replaced when is_vlan_tagged is TRUE.
*   vlan_tpid            - VLAN TPID
*   vlan_tci             - VLAN TCI 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES: This function is must be called only after the ENET is provisined and 
         the digi_enet_client_source_cfg() is called once. The latter configures
*        firmware to use the ENET-LINE or ENET-SYS ECLASS.
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_eclass_hwire_cfg(digi_handle_t           *digi_handle,
                                          digi_enet_pkt_fwd_dst_t pkt_fwd_dst,
                                          enet_dir_t              datapath,
                                          UINT32                  link,
                                          BOOL8                   link_enable,
                                          BOOL8                   extract_enable,
                                          util_global_pkt_type_t  pkt_type,
                                          BOOL8                   include_ts,
                                          BOOL8                   is_vlan_tagged,
                                          UINT16                  vlan_tpid,
                                          UINT16                  vlan_tci)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[3];

    PMC_ENTRY();

    /* HOSTMSG_CMD_CFG_ENET_ECLASS_CONFIG_HWIRED_FILTER_ST has the following format:
     * Word |  bits 24..31       | bits 16..23    | bits 8..15        | bits 0..7   |
     * 0    | packet_destination | datapath       | link              | link_enable |
     * 1    | packet_type        | extract_enable | include_timestamp | reserved    |
     * 2    |              vlan_tpid              |           vlan_tci              |
     */
    data[0] = ((UINT8)pkt_fwd_dst << 24) +
                                             ((UINT8)datapath << 16) + 
                                             ((UINT8)link << 8) + 
                                             (UINT8)link_enable;
    data[1] = ((UINT8)pkt_type << 24) + 
                                                 ((UINT8)extract_enable << 16) + 
                                                  ((UINT8)include_ts << 8);
    data[2] = (vlan_tpid << 16) + vlan_tci;
    

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_ENET,HOSTMSG_CMD_CFG_ENET_ECLASS_CONFIG_HWIRED_FILTER_ST,data,3,stat);
    }

    PMC_RETURN(result);
} /* digi_fw_eclass_hwire_cfg */

/*******************************************************************************
* digi_fw_enet_intr_monitor_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function sends a host message to configure the ENET interrupt
*   monitoring using FW-based I-bits instead of HW interrupts.
*   See INPUTS section in this header for the list of the supported interrupts.
*
* INPUTS:
*   digi_handle          - pointer to DIGI handle instance
*   int_id               - enumerated interrupt ID:
*                          INTR_IPI_ENET_LINE_EPMM_CBR_RX_LOBL,
*                          INTR_IPI_ENET_SYS_EPMM_CBR_RX_LOBL,
*                          INTR_IPI_ENET_LINE_ETRANS_CBR_RX_LOBL.
*                          See digi_intr_ipi_monitor_t for details.
*   enable               - Enable (TRUE) or disable (FALSE) the ENET interrupt
*                          monitoring task for the specified interrupt ID
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES: Note that, 0x1801348 IPI to PCIE Interrupt Enable (Third set): IPI2PCIE_2_INT_EN
*        register is used to configure ENET interrupt monitoring IDs. 
*        
*       The CMD_REQ_ENET_Intr_Monitor_Cfg host message is used to configure
*       ENET interrupt monitoring.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_enet_intr_monitor_cfg(digi_handle_t *digi_handle,
                                               digi_intr_ipi_monitor_t int_id,
                                               BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];

    PMC_ENTRY();

    /* HOSTMSG_CMD_CFG_ENET_INTR_MONITOR_CFG_ST has the following format:
     * Word |  bits 24..31 | bits 16..23 | bits 8..15 | bits 0..7 |
     *  0   |           int_id           | reserved   | enable    |
     */
    data[0] = (((UINT16)int_id) << 16) + enable;

    /* Send host message to firmware */
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_ENET,HOSTMSG_CMD_CFG_ENET_INTR_MONITOR_CFG_ST,data,1,stat);
    }

    PMC_RETURN(result);
}/* digi_fw_enet_intr_monitor_cfg */

/*******************************************************************************
* digi_fw_enet_intr_monitor_status_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  This method gets the ENET interrupt monitoring status (fw-based I-bits)
*  for the specified interrupt ID.
*  The fw-based I-bits are cleared for all ENET lanes of the specified interrupt,
*  when this function is returned. In essence, the fw I-bits are read-to-clear interrupts
*  for the specified ENET interrupt. Therefore, the caller function must store the
*  received FW I-bit, if it needs to be used.
* 
* INPUTS:
*  digi_handle - DIGI device handle.
*  int_id      - enumerated interrupt ID, see digi_intr_ipi_monitor_t
*                for details
*  clr_status  - TRUE: clear status; FALSE: don't clear status
*
* OUTPUTS:
*  fw_i_bit_status_ptr - Pointer to the ENET interrupt monitoring status (FW-based I-bit). 
*                        The valid range of the ENET FW-based I-bits is [19:0], 
*                        where each bit corresponds to the ENET link of the specified interrupt. 
*                        i.e. Bit0 indicates the fw-based I-bit of the ENET lane 0.
*
*  RETURNS:
*    PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
*  NOTES: CMD_REQ_ENET_Intr_Monitor_Status_Get.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_enet_intr_monitor_status_get(digi_handle_t *digi_handle,
                                                      digi_intr_ipi_monitor_t int_id,
                                                      BOOL8  clr_status,
                                                      UINT32 *fw_i_bit_status_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[2];
    UINT32 response[1];

    PMC_ENTRY();

    PMC_ASSERT((NULL != fw_i_bit_status_ptr), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Add message body to the previously created header. */
    /* Fill int_id field. */
    data[0] = int_id;
    data[1] = clr_status;
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_fixed_length_query((pmc_handle_t *)digi_handle,MODID_ENET,HOSTMSG_CMD_CFG_ENET_INTR_MONITOR_STATUS_GET_ST,data,2,response,1,stat);
    }


    /* Get result*/
    if (PMC_SUCCESS == result)
    {
        /* Return fw-based I-bit status */
        *fw_i_bit_status_ptr = response[0];
    }

    PMC_RETURN(result);
} /* digi_fw_enet_intr_monitor_status_get */

/*******************************************************************************
* digi_fw_cbrc_intr_monitor_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function sends a host message to configure the CBRC interrupt
*   monitoring using FW-based I-bits instead of HW interrupts.
*   See INPUTS section in this header for the list of the supported interrupts.
*
* INPUTS:
*   digi_handle          - pointer to DIGI handle instance.
*   int_id               - enumerated interrupt ID:
                           INTR_IPI_CBRC_FC1200_PCS_RX_RF,
                           INTR_IPI_CBRC_FC1200_PCS_RX_LF,
                           INTR_IPI_CBRC_FC1200_PCS_RX_LOSS_SYNC,
                           INTR_IPI_CBRC_FC1200_PCS_RX_HIGH_BER,
                           INTR_IPI_CBRC_FC1200_PCS_RX_LOS,
                           INTR_IPI_CBRC_FC1200_PMON_ILVR_LOS_INT,
                           INTR_IPI_CBRC_FC1200_PMON_ILVR_LOSS_SYNC_INT,
                           INTR_IPI_CBRC_FC1200_PMON_ILVR_HIGH_BER,
                           INTR_IPI_CBRC_FC1200_PMON_DLVR_LOS_INT,
                           INTR_IPI_CBRC_FC1200_PMON_DLVR_LOSS_SYNC_INT,
                           INTR_IPI_CBRC_FC1200_PMON_DLVR_HIGH_BER.
*                          See digi_intr_ipi_monitor_t for details.
*   enable               - Enable (TRUE) or disable (FALSE) the CBRC interrupt
*                          monitoring task for the specified interrupt ID
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES: Note that, 0x1801348 IPI to PCIE Interrupt Enable (Third set): IPI2PCIE_2_INT_EN
*        register is used to configure CBRC interrupt monitoring IDs. 
*        The CMD_REQ_CBRC_Intr_Monitor_Cfg host message is used to configure
*        CBRC interrupt monitoring.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_cbrc_intr_monitor_cfg (digi_handle_t *digi_handle,
                                                digi_intr_ipi_monitor_t int_id,
                                                BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];

    PMC_ENTRY();

    /* HOSTMSG_CMD_CFG_CBRC_INTR_MONITOR_CFG_ST has the following format:
     * Word |  bits 24..31 | bits 16..23 | bits 8..15 | bits 0..7 |
     *  0   |           int_id           | reserved   | enable    |
     */
    data[0] = (UINT32)(int_id << 16) + enable;

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_CBRC,HOSTMSG_CMD_CFG_CBRC_INTR_MONITOR_CFG_ST,data,1,stat);
    }

    PMC_RETURN(result);
}/* digi_fw_cbrc_intr_monitor_cfg */

/*******************************************************************************
* digi_fw_cbrc_intr_monitor_status_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  This method gets the CBRC interrupt monitoring status (fw-based I-bits)
*  for the specified interrupt ID.
*  The fw-based I-bits are cleared for all CBRC links of the specified interrupt,
*  when this function is returned. In essence, the fw I-bits are read-to-clear interrupts
*  for the specified CBRC interrupt. Therefore, the caller function must store the
*  received FW I-bit, if it needs to be used.
*
* INPUTS:
*  digi_handle - DIGI device handle.
*  int_id      - enumerated interrupt ID, see digi_intr_ipi_monitor_t
*                for details
*
* OUTPUTS:
*  fw_i_bit_status_ptr - Pointer to the CBRC interrupt monitoring status (FW-based I-bit). 
*                        The valid range of the CBRC FW-based I-bits is [11:0], 
*                        where each bit corresponds to the CBRC link of the specified interrupt. 
*                        i.e. Bit0 indicates the fw-based I-bit of the CBRC link 0.
*
*  RETURNS:
*    PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
*  NOTES:
*  The CMD_REQ_CBRC_Intr_Monitor_Status_Get host message is used to retrieve
*         interrupt monitoring status. 
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_cbrc_intr_monitor_status_get(digi_handle_t *digi_handle,
                                                      digi_intr_ipi_monitor_t int_id,
                                                      UINT32 *fw_i_bit_status_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];
    UINT32 response[1];

    PMC_ENTRY();

    PMC_ASSERT((NULL != fw_i_bit_status_ptr), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);


    /* HOSTMSG_CMD_CFG_CBRC_INTR_MONITOR_STATUS_GET_ST has the following format:
     * Word |  bits 24..31 | bits 16..23 | bits 8..15 | bits 0..7 |
     *  0   |                    int_id                           |
     */
    data[0] = int_id;

    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_fixed_length_query((pmc_handle_t *)digi_handle,MODID_CBRC,HOSTMSG_CMD_CFG_CBRC_INTR_MONITOR_STATUS_GET_ST,data,1,response,1,stat);
    }

    /* Get result*/
    if (PMC_SUCCESS == result)
    {
        /* Return fw-based I-bit status */
        *fw_i_bit_status_ptr = response[0];
    }

    PMC_RETURN(result);
} /* digi_fw_cbrc_intr_monitor_status_get */


/*******************************************************************************
* digi_fw_cbrc_intr_channelized_monitor_status_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  This method gets the CBRC interrupt monitoring status (fw-based I-bits)
*  for the specified interrupt ID and the specified channel.
*
* INPUTS:
*  digi_handle - DIGI device handle.
*  int_id      - enumerated interrupt ID, see digi_intr_ipi_monitor_t
*                for details
*  ch_id       - CBRC link number
*
* OUTPUTS:
*  fw_i_bit_status_ptr - Pointer to the CBRC interrupt monitoring status (FW-based I-bit). 
*                        The valid range of the CBRC FW-based I-bit is 0 or 1, 
*
*  RETURNS:
*    PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
*  NOTES:
*  The CMD_REQ_CBRC_Intr_Channelized_Monitor_Status_Get host message is used to retrieve
*         interrupt monitoring status. 
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle_t *digi_handle,
                                                                  digi_intr_ipi_monitor_t int_id,
                                                                  UINT32 ch_id,
                                                                  UINT32 *fw_i_bit_status_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[2];
    UINT32 response[1];

    PMC_ENTRY();

    PMC_ASSERT((NULL != fw_i_bit_status_ptr), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);


    /* HOSTMSG_CMD_CFG_CBRC_INTR_MONITOR_STATUS_GET_ST has the following format:
     * Word |  bits 24..31 | bits 16..23 | bits 8..15 | bits 0..7 |
     *  0   |                    int_id                           |
     *  1   |                    ch_id                            |
     */
    data[0] = int_id;
    data[1] = ch_id;

    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_fixed_length_query((pmc_handle_t *)digi_handle,MODID_CBRC,HOSTMSG_CMD_CFG_CBRC_INTR_CHANNELIZED_MONITOR_STATUS_GET_ST,data,2,response,1,stat);
    }

    /* Get result*/
    if (PMC_SUCCESS == result)
    {
        /* Return fw-based I-bit status */
        *fw_i_bit_status_ptr = response[0];
    }

    PMC_RETURN(result);
} /* digi_fw_cbrc_intr_channelized_monitor_status_get */


/*******************************************************************************
* digi_fw_cbrc_intr_channelized_clear
* ___________________________________________________________________________
*
* DESCRIPTION:
*  This method clears the CBRC interrupt monitoring status (fw-based I-bits)
*  for the specified interrupt ID and the specified channel.
*
* INPUTS:
*  digi_handle - DIGI device handle.
*  int_id      - enumerated interrupt ID, see digi_intr_ipi_monitor_t
*                for details
*  ch_id       - CBRC link number
*
* OUTPUTS:
*
*  RETURNS:
*    PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
*  NOTES:
*  The HOSTMSG_CMD_CFG_CBRC_CHANNELIZED_INTR_CLEAR_MONITOR_STATUS host message is used to retrieve
*         interrupt monitoring status. 
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_cbrc_intr_channelized_clear(digi_handle_t *digi_handle,
                                                      digi_intr_ipi_monitor_t int_id,
                                                      UINT32 ch_id)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[2];

    PMC_ENTRY();

    /* HOSTMSG_CMD_CFG_CBRC_INTR_CLEAR_MONITOR_STATUS has the following format:
     * Word |  bits 24..31 | bits 16..23 | bits 8..15 | bits 0..7 |
     *  0   |                    int_id                           |
     *  1   |                    ch_id                            |
     */
    data[0] = int_id;
    data[1] = ch_id;

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_start_cfg_message((pmc_handle_t *)digi_handle,MODID_CBRC,HOSTMSG_CMD_CFG_CBRC_CHANNELIZED_INTR_CLEAR_MONITOR_STATUS,data,2,stat);
    }

    PMC_RETURN(result);
} /* digi_fw_cbrc_intr_channelized_clear */

/*******************************************************************************
* digi_fw_opsa_cfg_mpma_port_add
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to FW to perform the MPMA startup operational procedure.
*  FW is designed such that in broadcast scenarios, should a second add for 
*  a channel occur, the FW will check if initial MPMA configuration 
*  has completed and if so, just update QSn_CH_EN of the specified DPI Slave 
*  Port. Otherwise it will update the current job to use the 
*  new QSn_CH_EN settings provided.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   subsystem       - subsystem to configure
*   chnl_add_msk_ptr - pointer to array of bitmask specifying channels to 
*                      enable
*                      chnl_add_msk_ptr[0]: Bitmask for channels 31:0
*                      chnl_add_msk_ptr[1]: Bitmask for channels 63:32
*                      chnl_add_msk_ptr[2]: Bitmask for channels 95:64
*   qs_ch_en_ptr    - Pointer to array of QSn_CH_EN config mask where 
*                     each bit index in mask[3:0] corresponds to n 
*                     in QSn_CH_EN.  Multiple bits may be set.
*                     The array must have a size of 96 to cover the maximum
*                     number of instances of MPMA blocks.
*   is_config_only - Used to differentiate between a normal OPSA add 
*                     operation in FW (FALSE) and an OPSA structure update 
*                     dome after a FW restart (TRUE).
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   CMD_CFG_OPSA_MPMA_Port_Add
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_opsa_cfg_mpma_port_add(digi_handle_t *digi_handle,
                                                hostmsg_opsa_subsystem_cfg_t subsystem,
                                                UINT32 *chnl_add_msk_ptr,
                                                UINT8  *qs_ch_en_ptr,
                                                BOOL8 is_config_only)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[28];
    UINT32 i;
    hostmsg_cmd_opsa_subtype_t msg_type = HOSTMSG_CMD_CFG_OPSA_MPMA_PORT_ADD_ST;

    PMC_ENTRY();

    if (TRUE == is_config_only)
    {
        msg_type = HOSTMSG_CMD_CFG_OPSA_MPMA_CTXT_CFG;
    }

    /* Subsystem and channel masks */
    data[0] = (UINT16)subsystem << 16;
    data[1] = chnl_add_msk_ptr[0]; 
    data[2] = chnl_add_msk_ptr[1]; 
    data[3] = chnl_add_msk_ptr[2]; 

    /* QS_CH_EN bits */
    for (i = 0; i < DIGI_MPMA_MPMO_NUM_MAX; i = i + sizeof(UINT32)) {
        data[4+i/4] = ((((UINT32)qs_ch_en_ptr[i]) << 24) & 0xff000000) | \
                   ((((UINT32)qs_ch_en_ptr[i+1]) << 16) & 0x00ff0000) | \
                   ((((UINT32)qs_ch_en_ptr[i+2]) << 8) & 0x0000ff00) | \
                   (((UINT32)qs_ch_en_ptr[i+3]) & 0x000000ff);
    } /* end for */

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_start_cfg_message((pmc_handle_t *)digi_handle,
                                           MODID_OPSA,
                                           msg_type,
                                           data, 28, stat);
    }

    PMC_RETURN(result);

} /* End: digi_fw_opsa_cfg_mpma_port_add() */

/*******************************************************************************
* digi_fw_opsa_cfg_mpma_port_drop
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to FW to configure FW to stop performing 
*  the MPMA startup operational procedure.  FW will put the MPMA_DSP in 
*  reset and set all QSn_CH_EN bits to 0 for all channels 
*  specified in this host message.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   subsystem       - subsystem to configure
*   chnl_drop_msk_ptr - Pointer to array of bitmask specifying channels to 
*                       disable
*                       chnl_drop_msk_ptr[0]: Bitmask for channels 31:0
*                       chnl_drop_msk_ptr[1]: Bitmask for channels 63:32
*                       chnl_drop_msk_ptr[2]: Bitmask for channels 95:64
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   CMD_CFG_OPSA_MPMA_Port_Drop
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_opsa_cfg_mpma_port_drop(digi_handle_t *digi_handle,
                                                 hostmsg_opsa_subsystem_cfg_t subsystem,
                                                 UINT32 *chnl_drop_msk_ptr)
{
    PMC_ERROR result = PMC_SUCCESS; 
    UINT32 data[4];       

    PMC_ENTRY();

    /* Subsystem and channel masks */
    data[0] = (UINT16)subsystem << 16;
    data[1] = chnl_drop_msk_ptr[0]; 
    data[2] = chnl_drop_msk_ptr[1]; 
    data[3] = chnl_drop_msk_ptr[2]; 

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_start_cfg_message((pmc_handle_t *)digi_handle,MODID_OPSA,HOSTMSG_CMD_CFG_OPSA_MPMA_PORT_DROP_ST,data,4,stat);
    }

    PMC_RETURN(result);

} /* End: digi_fw_opsa_cfg_mpma_port_drop() */

/*******************************************************************************
* digi_fw_opsa_cfg_mpmo_port_add
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to FW to configure FW to perform the MPMO startup  
*  operational procedure and the ongoing service routine.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   subsystem       - subsystem to configure
*   chnl_add_msk_ptr - Pointer to array of bitmask specifying channels to 
*                      enable
*                      chnl_add_msk_ptr[0]: Bitmask for channels 31:0
*                      chnl_add_msk_ptr[1]: Bitmask for channels 63:32
*                      chnl_add_msk_ptr[2]: Bitmask for channels 95:64
*   ch_data_target_ptr  - Pointer to array of channel data target
*                         configuration bits.
*                         ch_data_target_x[6:4] specifies the 
*                         downstream_data_target:
*                         0x1: Tx Serdes ACB LINE_S16     
*                         0x2: Tx Serdes ACB SYS_S16      
*                         0x3: Tx Serdes ACB SFI5.1       
*                         0x4: Stage 1 Mux Tributary Slots
*                         0x5: Stage 2 Mux Tributary Slots
*                         0x6: MAPOTN
*
*                         Valid data targets for each subsystem are: 
*                         CBRC: 0x1-0x3                              
*                         MAPOTN: 0x6                                
*                         ENET_LINE: 0x1-0x3                         
*                         ENET_SYS: 0x1-0x3                          
*                         COREOTN_FO1: 0x1-0x5                       
*                         COREOTN_FO2: 0x1-0x3                       
*                         COREOTN_CTL: 0x1-0x3                       
*
*                         If downstream_data_target = 0x01 to 0x03
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
*   ch_ckctl_port_ptr   - Pointer to array of CKCTL port number
*                         The CKCTL port number is used to specify the 'N' that
*                         is used to access the CKCTL_EN bit in the CKCTL_CTRL_REG.
*
*                         Valid Range of this parameter is: 0 ~ 25
*
*                         For example:
*                         If downstream target is Tx Serdes ACB SFI5.1, the valid 
*                         range of value in this parameter shall be [12:13]
*
*                         If the downstream target is Tx Serdes ACB SYS_S16 and 
*                         input the subsystem is ENET_SYS, then the valid range
*                         of value in this parameter shall be [0:11]
*
*                         If the downstream target is Tx Serdes ACB SYS_S16 and 
*                         input the subsystem is *NOT* ENET_SYS, then the valid 
*                         range of value in this parameter shall be [14:25]
*
*                         The caller function must implement the rules to select 
*                         the proper "ch_ckctl_port" for the corresponding channel 
*                         based on the product application and datapath configuration.
*   coreotn_fo2_channel_ptr - Pointer to array of coreotn_fo2 channels
*   coreotn_fo2_ckctl_port_ptr - Pointer to array of coreotn_fo2 ckctl ports
*   num_coreotn_fo2_channels - number of coreotn fo2 channels in the coreotn_fo2 arrays.
*   is_config_only   - Flag determines Host message sent to FW. Typically 
*                      HOSTMSG_CMD_CFG_OPSA_MPMO_PORT_ADD_ST however for 
*                      3.04 to 3.07 or higher upgrades then need to 
*                      reconfigure FW OPSA and 
*                      HOSTMSG_CMD_CFG_OPSA_MPMO_CTXT_CFG is used.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   CMD_CFG_OPSA_MPMO_Port_Add
*
*******************************************************************************/

PUBLIC PMC_ERROR digi_fw_opsa_cfg_mpmo_port_add(digi_handle_t *digi_handle,
                                                hostmsg_opsa_subsystem_cfg_t subsystem,
                                                UINT32 chnl_add_msk_ptr[3],
                                                UINT8  ch_data_target_ptr[96],
                                                UINT8  ch_ckctl_port_ptr[96],
                                                UINT8  coreotn_fo2_channel_ptr[24],
                                                UINT8  coreotn_fo2_ckctl_port_ptr[24],
                                                UINT8   num_coreotn_fo2_channels,
                                                BOOL8 is_config_only)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    UINT32 data[62];
    hostmsg_cmd_opsa_subtype_t msg_type = HOSTMSG_CMD_CFG_OPSA_MPMO_PORT_ADD_ST;

    PMC_ENTRY();

    PMC_ASSERT(num_coreotn_fo2_channels<20,HOSTMSG_ERR_INVALID_PARAMETERS,num_coreotn_fo2_channels,0);

    /* Subsystem and channel masks */
    data[0] = (UINT16)subsystem << 16;
    data[1] = chnl_add_msk_ptr[0]; 
    data[2] = chnl_add_msk_ptr[1]; 
    data[3] = chnl_add_msk_ptr[2]; 

    /* Channel data target bits (data[4..27]) */
    for (i = 0; i < DIGI_MPMA_MPMO_NUM_MAX; i = i + sizeof(UINT32)) {
        data[4+i/4] = ((((UINT32)ch_data_target_ptr[i]) << 24) & 0xff000000) | \
                   ((((UINT32)ch_data_target_ptr[i+1]) << 16) & 0x00ff0000) | \
                   ((((UINT32)ch_data_target_ptr[i+2]) << 8) & 0x0000ff00) | \
                   (((UINT32)ch_data_target_ptr[i+3]) & 0x000000ff);
    } /* end for */

    /* Channel CKCTL port data (data[28..51])*/
    for (i = 0; i < DIGI_MPMA_MPMO_NUM_MAX; i = i + sizeof(UINT32)) {
        data[28+i/4] = ((((UINT32)ch_ckctl_port_ptr[i]) << 24) & 0xff000000) | \
                   ((((UINT32)ch_ckctl_port_ptr[i+1]) << 16) & 0x00ff0000) | \
                   ((((UINT32)ch_ckctl_port_ptr[i+2]) << 8) & 0x0000ff00) | \
                   (((UINT32)ch_ckctl_port_ptr[i+3]) & 0x000000ff);
    } /* end for */

    if (TRUE == is_config_only)
    {
        msg_type = HOSTMSG_CMD_CFG_OPSA_MPMO_CTXT_CFG;
    }

    /* COREOTN FO2 channel and ckctl_port (data[51..63]) */
    for (i=0;i<num_coreotn_fo2_channels;i = i + 2) {
        data[52+i/2] = ((((UINT32)coreotn_fo2_channel_ptr[i]) << 24) & 0xff000000) | \
                   ((((UINT32)coreotn_fo2_ckctl_port_ptr[i]) << 16) & 0x00ff0000) | \
                   ((((UINT32)coreotn_fo2_channel_ptr[i+1]) << 8) & 0x0000ff00) | \
                   (((UINT32)coreotn_fo2_ckctl_port_ptr[i+1]) & 0x000000ff);
    }

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_start_cfg_message((pmc_handle_t *)digi_handle,
                                           MODID_OPSA,
                                           msg_type,
                                           data,
                                           62,
                                           stat);
    }

    PMC_RETURN(result);

} /* End: digi_fw_opsa_cfg_mpmo_port_add() */

/*******************************************************************************
* digi_fw_opsa_cfg_mpmo_port_drop
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to FW to configure FW to stop performing the 
*  MPMO startup operational procedure.  FW will disable timing propagation 
*  for the resource being removed if applicable.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   subsystem       - subsystem to configure
*   chnl_drop_msk_ptr - Pointer to array of bitmask specifying channels to 
*                       disable
*                       chnl_drop_msk_ptr[0]: Bitmask for channels 31:0
*                       chnl_drop_msk_ptr[1]: Bitmask for channels 63:32
*                       chnl_drop_msk_ptr[2]: Bitmask for channels 95:64
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   CMD_CFG_OPSA_MPMA_Port_Drop
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_opsa_cfg_mpmo_port_drop(digi_handle_t *digi_handle,
                                                hostmsg_opsa_subsystem_cfg_t subsystem,
                                                 UINT32 chnl_drop_msk_ptr[3])
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[4];

    PMC_ENTRY();

    /* Subsystem and channel masks */
    data[0] = (UINT16)subsystem << 16;
    data[1] = chnl_drop_msk_ptr[0]; 
    data[2] = chnl_drop_msk_ptr[1]; 
    data[3] = chnl_drop_msk_ptr[2]; 

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_start_cfg_message((pmc_handle_t *)digi_handle,MODID_OPSA,HOSTMSG_CMD_CFG_OPSA_MPMO_PORT_DROP_ST,data,4,stat);
    }

    PMC_RETURN(result);

} /* End: digi_fw_opsa_cfg_mpmo_port_drop() */

/*******************************************************************************
* digi_fw_opsa_cfg_fmf1_txjc_cfc_fifo_clrb
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to FW to configure the FIFO_CLRB bits in the 
*  specified channels of the COREOTN FMF1 TXJC_CFC block. 
*  This host message is used by AppLib as part of the COREOTN datapath 
*  configuration since FW owns the FIFO_CLRB bits. 
*
* INPUTS:
*   digi_handle - pointer to DIGI handle instance.
*   value       - value to write (0, 1)
*   chnl_msk_ptr - Pointer to array of bitmask specifying channels to write to
*                  chnl_msk_ptr[0]: Bitmask for channels 31:0
*                  chnl_msk_ptr[1]: Bitmask for channels 63:32
*                  chnl_msk_ptr[2]: Bitmask for channels 95:64
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   CMD_CFG_OPSA_TXJC_CFC_FIFO_CLRB
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_opsa_cfg_fmf1_txjc_cfc_fifo_clrb(digi_handle_t *digi_handle,
                                                          UINT8 value, UINT32 *chnl_msk_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[4];

    PMC_ENTRY();

    /* Subsystem and channel masks */
    /* Note: a new parameter is added into the FW hostmsg in order to know which subsystem should 
       be used to control the FIFO_CLRB bit */
    data[0] = ((UINT8)value << 24) | ((UINT8)HOSTMSG_OPSA_FIFO_CLRB_TARGET_SS_COREOTN_FMF1 << 16);
    data[1] = chnl_msk_ptr[0]; 
    data[2] = chnl_msk_ptr[1]; 
    data[3] = chnl_msk_ptr[2]; 

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_start_cfg_message((pmc_handle_t *)digi_handle,MODID_OPSA,HOSTMSG_CMD_CFG_OPSA_TXJC_CFC_FIFO_CLRB_ST,data,4,stat);
    }

    PMC_RETURN(result);

} /* End: digi_fw_opsa_cfg_fmf1_txjc_cfc_fifo_clrb() */

/*******************************************************************************
* digi_fw_opsa_cfg_tgmp_txjc_cfc_fifo_clrb
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to FW to configure the FIFO_CLRB bits in the 
*  specified channels of the MAPOTN TGMP TXJC block. 
*  This host message is used by AppLib as part of the MAPOTN datapath 
*  configuration since FW owns the FIFO_CLRB bits. 
*
* INPUTS:
*   digi_handle - pointer to DIGI handle instance.
*   value       - value to write (0, 1)
*   chnl_msk_ptr - Pointer to array of bitmask specifying channels to write to
*                  chnl_msk_ptr[0]: Bitmask for channels 31:0
*                  chnl_msk_ptr[1]: Bitmask for channels 63:32
*                  chnl_msk_ptr[2]: Bitmask for channels 95:64
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   CMD_CFG_OPSA_TXJC_CFC_FIFO_CLRB
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_opsa_cfg_tgmp_txjc_cfc_fifo_clrb(digi_handle_t *digi_handle,
                                                          UINT8 value, UINT32 *chnl_msk_ptr)
{
    PMC_ERROR result = PMC_SUCCESS; 
    UINT32 data[4]; 

    PMC_ENTRY();

    /* Subsystem and channel masks */
    data[0] = ((UINT8)value << 24) | ((UINT8)HOSTMSG_OPSA_FIFO_CLRB_TARGET_SS_MAPOTN << 16);
    data[1] = chnl_msk_ptr[0]; 
    data[2] = chnl_msk_ptr[1]; 
    data[3] = chnl_msk_ptr[2]; 

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_OPSA,HOSTMSG_CMD_CFG_OPSA_TXJC_CFC_FIFO_CLRB_ST,data,4,stat);
    }

    PMC_RETURN(result);

} /* End: digi_fw_opsa_cfg_tgmp_txjc_cfc_fifo_clrb() */

/*******************************************************************************
* digi_fw_opsa_fifo_cent_trigger
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to FW to trigger FIFO centering or Zone centering for
*  the specified channels in the COREOTN_FO1. The target 
*  that is triggered for each channel (FIFO or zone centering) is 
*  determined by the configuration specified in the call to
*  digi_fw_opsa_cfg_mpmo_port_add().
*
* INPUTS:
*   digi_handle - pointer to DIGI handle instance.
*   chnl_msk_ptr - Pointer to array of bitmask specifying channels to trigger
*                 chnl_msk_ptr[0]: Bitmask for channels 31:0
*                 chnl_msk_ptr[1]: Bitmask for channels 63:32
*                 chnl_msk_ptr[2]: Bitmask for channels 95:64
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   CMD_CFG_OPSA_FIFO_CENT_Trigger
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_opsa_fifo_cent_trigger(digi_handle_t *digi_handle, UINT32 *chnl_msk_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[3];

    PMC_ENTRY();

    /* Channel masks */
    data[0] = chnl_msk_ptr[0]; 
    data[1] = chnl_msk_ptr[1]; 
    data[2] = chnl_msk_ptr[2]; 

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_OPSA,HOSTMSG_CMD_CFG_OPSA_FIFO_CENT_TRIGGER_ST,data,3,stat);
    }

    PMC_RETURN(result);

} /* End: digi_fw_opsa_fifo_cent_trigger() */


/*******************************************************************************
* digi_fw_opsa_cfg_mpmo_times
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to FW to configure mpmo times for OPSA algorithms. 
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   opsa_mpmo_times - pointer to structure of times \n
*                     note: set a time to 0xffffffff to keep existing value.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   To just set one field, fill the structure with 0xff first like:
*
*   {
*     digi_fw_opsa_mpmo_times_t opsa_mpmo_times;
*
*     memset(&opsa_mpmo_times,0xff,sizeof(digi_fw_opsa_mpmo_times_t));
*     opsa_mpmo_times.proc4_clear_window_in_us = 1000*1000; / * just modify proc4_clear_window * /
*     result = digi_fw_opsa_cfg_mpmo_times( digi_handle, &opsa_mpmo_times );
*   }
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_opsa_cfg_mpmo_times(digi_handle_t *digi_handle,
                                            digi_fw_opsa_mpmo_times_t *opsa_mpmo_times)
{
    PMC_ERROR result = PMC_SUCCESS;
    HOSTMSG_DECLARE_CFG_MSG_STAT(stat);

    PMC_ENTRY();

    result = hostmsg_send_cfg_message((pmc_handle_t*)digi_handle,
                                      MODID_OPSA,
                                      HOSTMSG_CMD_CFG_OPSA_MPMO_TIMES_ST,
                                      (UINT32 *)opsa_mpmo_times,
                                      sizeof(digi_fw_opsa_mpmo_times_t)/sizeof(UINT32),
                                      stat);

    PMC_RETURN(result);
} /* End: digi_fw_opsa_cfg_mpmo_times() */

/*******************************************************************************
* digi_fw_opsa_cfg_mpmo_fifo_trigger_enables
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to FW to configure mpmo fifo trigger enables for OPSA algorithms. 
*
* INPUTS:
*   digi_handle                            - pointer to DIGI handle instance.
*   digi_fw_opsa_mpmo_fifo_trigger_enables - pointer to structure of fifo trigger enables \n
*                                            note: set an enable to 0xffffffff to keep existing value.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   To just set one field, fill the structure with 0xffffffff first like:
*
*   {
*     digi_fw_opsa_mpmo_fifo_trigger_enables_t opsa_mpmo_fifo_trigger_enables;
*
*     memset(&opsa_mpmo_fifo_trigger_enables,0xff,sizeof(digi_fw_opsa_mpmo_fifo_trigger_enables_t));
*     opsa_mpmo_fifo_trigger_enables.proc1_fifo_trigger_enabled = 0; / * just modify proc1_fifo_trigger_enable * /
*     result = digi_fw_opsa_cfg_mpmo_fifo_trigger_enables( digi_handle, &opsa_mpmo_fifo_trigger_enables );
*   }
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_opsa_cfg_mpmo_fifo_trigger_enables(digi_handle_t *digi_handle,
                                            digi_fw_opsa_mpmo_fifo_trigger_enables_t *digi_fw_opsa_mpmo_fifo_trigger_enables)
{
    PMC_ERROR result = PMC_SUCCESS;
    HOSTMSG_DECLARE_CFG_MSG_STAT(stat);

    PMC_ENTRY();

    result = hostmsg_send_cfg_message((pmc_handle_t*)digi_handle,
                                      MODID_OPSA,
                                      HOSTMSG_CMD_CFG_OPSA_MPMO_FIFO_TRIGGER_ENABLES_ST,
                                      (UINT32 *)digi_fw_opsa_mpmo_fifo_trigger_enables,
                                      sizeof(digi_fw_opsa_mpmo_fifo_trigger_enables_t)/sizeof(UINT32),
                                      stat);

    PMC_RETURN(result);
} /* End: digi_fw_opsa_cfg_mpmo_fifo_trigger_enables() */

/*******************************************************************************
* digi_fw_opsa_cfg_tau_value
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to FW to configure tau value for OPSA startup. 
*  The tau value is used as waiting window value in the OPSA startup
*  procedure. This host message should be called before the procedure
* is enabled on any channel.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   subsystem       - subsystem to configure
*   cfg_mpma        - TRUE: Configure MPMA. FALSE: Configure MPMO.
*   tau_value_us    - Tau value in microseconds. Valid range is 0-10000us.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   CMD_CFG_OPSA_Tau_Value
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_opsa_cfg_tau_value(digi_handle_t *digi_handle,
                                            hostmsg_opsa_subsystem_cfg_t subsystem,
                                            BOOL cfg_mpma, UINT16 tau_value_us)
{
    UINT32 cfg_msg[2];
    PMC_ERROR result = PMC_SUCCESS;
    HOSTMSG_DECLARE_CFG_MSG_STAT(stat);

    PMC_ENTRY();

    /* Build message body */
    cfg_msg[0] = ((UINT16)subsystem << 16) | cfg_mpma;
    cfg_msg[1] = tau_value_us; 

    result = hostmsg_send_cfg_message((pmc_handle_t*)digi_handle,
                                      MODID_OPSA,
                                      HOSTMSG_CMD_CFG_OPSA_TAU_VALUE_ST,
                                      cfg_msg,
                                      sizeof(cfg_msg)/sizeof(UINT32),
                                      stat);

    PMC_RETURN(result);

} /* End: digi_fw_opsa_cfg_tau_value() */

/*******************************************************************************
*  digi_fw_coreotn_cfg_dloomfi_mon
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This method sends host message to FW to triggers firmware to
*               enable or disable the periodic task that monitors the dLOOFMI
*               defect and report the defect status to the DI_RCP.  
*
*  INPUTS:
*               digi_handle - System handle.
*               is_enable - Enable/disable dLOOFMI Monitoring and Reporting.
*               defect_src- dLOOMFI Defect Source. 0 indicate LINEOTN is used
*               as server, and defect should be read from COREOTN FMF1 Demux.
*               1 indicate SYSOTN is used as server, and defect should be read
*               from COREOTN FMF2 Demux.
*
*  OUTPUTS:
*               None.
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_CFG_CoreOTN_Config_dLOOMFI_Monitor
*
******************************************************************************/
PUBLIC PMC_ERROR digi_fw_coreotn_cfg_dloomfi_mon(digi_handle_t *digi_handle,
                                                 BOOL8 is_enable,
                                                 hostmsg_otn_server_src_t defect_src)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];

    PMC_ENTRY();

    PMC_ASSERT((HOSTMSG_OTN_SERVER_SRC_MAX_NUM > defect_src), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);


    /* CMD_CFG_COREOTN_Config_dLOOMFI_Monitor  has the following format:
     * Word |  bits 24..31 | bits 16..23 | bits 8..15 | bits 0..7 |
     *  0   |  is_enable   | defect_src  |         reserved       |
     */
    data[0] = (((UINT8)is_enable << 24) + ((UINT8)defect_src << 16));

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_COREOTN,HOSTMSG_CMD_CFG_COREOTN_CONFIG_DLOOMFI_MON_ST,data,1,stat);
    } 

    PMC_RETURN(result);

} /* digi_fw_coreotn_cfg_dloomfi_mon */


/*******************************************************************************
*  digi_fw_coreotn_ohp_en
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This method sends host message to FW to triggers firmware to
*               enable or disable the periodic task that monitors monitor if ohp port is CFC_fifo_full.
*
*  INPUTS:
*               digi_handle - System handle.
*               ohp_port - ohp1 ohp2 ohp3.
*               ohp_en - Enable/disable ohp_port.
*               portA_en - portB_en.
*               portB_en - portB_en.
*
*  OUTPUTS:
*               None.
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               COREOTN_HOSTMSG_STYPE_CFG_OHP_EN
*
******************************************************************************/
PUBLIC PMC_ERROR digi_fw_coreotn_ohp_en(digi_handle_t *digi_handle,
                                                 UINT8 ohp_port,
                                                 BOOL8 ohp_en,
                                                 BOOL8 portA_en,
                                                 BOOL8 portB_en)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];

    PMC_ENTRY();


    /* CMD_CFG_COREOTN_Config_dLOOMFI_Monitor  has the following format:
     * Word |  bits 24..31 | bits 16..23 | bits 8..15 | bits 0..7 |
     *  0   |  is_enable   | defect_src  |         reserved       |
     */
    data[0] = (((UINT8)ohp_port << 24) + ((UINT8)ohp_en << 16) +  ((UINT8)portA_en <<8) +  ((UINT8)portB_en << 0) );

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_COREOTN,HOSTMSG_CMD_CFG_COREOTN_HOSTMSG_STYPE_CFG_OHP_EN_ST,data,1,stat);
    } 

    PMC_RETURN(result);

} /* digi_fw_coreotn_cfg_dloomfi_mon */


/*******************************************************************************
*  digi_fw_led_set_spi_dev_addr
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This method sends host message to FW to configure the register
*               offset for register 1 within the SPI slave device for LED
*               activity monitoring. The maximum offset supported is 0x1000000.
*               The address for register 2 equals to register offset 1 + 0x4. 
*
*  INPUTS:
*               digi_handle - System handle.
*               spi_reg_offset - Offset for register in SPI slave device used
*                                for LED activity monitoring.
*
*  OUTPUTS:
*               None.
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_CFG_LED_Set_SPI_Dev_Addr
*
******************************************************************************/
PUBLIC PMC_ERROR digi_fw_led_set_spi_dev_addr(digi_handle_t *digi_handle,
                                              UINT32 spi_reg_offset)
{   
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];

    PMC_ENTRY();

    /* CMD_CFG_LED_Set_SPI_Dev_Addr has the following format:
     * Word |  bits 24..31 | bits 16..23 | bits 8..15 | bits 0..7 |
     *  0   |         SPI slave register_offset_1                 |
     */
    data[0] = spi_reg_offset;

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_COREOTN,HOSTMSG_CMD_CFG_LED_SET_SPI_DEV_ADDR_ST,data,1,stat);
    } 

    PMC_RETURN(result);
} /* digi_fw_led_set_spi_dev_addr */


/*******************************************************************************
*  digi_fw_led_set_mon_ctrl
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This method sends host message to FW to Enable/disable the LED
*               status monitoring function.  When the LED status monitoring
*               function is enabled, firmware periodically checks the link
*               activity on the client port based on the client port type
*               (e.g. Ethernet or Fiber Channel) specified in this message.
*               In addition, the firmware writes out the link activity status
*               in the format defined in FUM. For ENET and CBRC, only one channel
*               can be enabled/disabled by each function call. Users must disable
*               the monitoring of an interface before re-configuring it to
*               monitor a different interface on the same channel. 
*
*  INPUTS:
*               digi_handle   - System handle.
*               is_enable     - Enable/disable monitoring for the channel
*               chnl_num      - Channel nubmer. \n
*                               For interfaces 10G ENET and 10G CBRC, the valid
*                               channel number is 0 - 11. \n
*                               For interfaces 40G ENET, the valid channel number
*                               is 0..2. \n
*                               For interfaces 100G ENET, the valid channel number
*                               is 0. \n
*                               For other interfaces, the channel number is discarded
*               led_interface - Interface type to configure. Since the interfaces 
*                               are shared between rates and interface types, the 
*                               following rules must be observed when enabling 
*                               monitoring:\n
*                               1.	If a 100G line/system interface is enabled, 
*                                  monitoring for all 40G and 10G channels on that
*                                  same interface can't be enabled before disable
*                                  the 100G line/system interface.\n
*                               2.	If a 40G channel is enabled, the corresponding 
*                                  10G channels can't be enabled before the 40G
*                                  channel is disabled. For example, if 40G channel
*                                  1 is enabled, monitoring for channels 4-7 on
*                                  the 10G link can't be enabled before 40G channel
*                                  1 is disabled.\n
*                               3.	Only ENET or CBRC monitoring can be enabled on 
*                                  each 10G channel.\n
*                               4.	CBRC monitoring can only be enabled on either
*                                  line or system interface at one time. There is
*                                  only one CBRC block. The line/system selection
*                                  determines which bits (line or system 10G) will
*                                  be set to '1' by firmware in the SPI data. \n
*                               5.	ENET_MGMT, RI_RCP, DI_RCP and OTN_OH1-3 can be 
*                                  enabled independently with no effect on other
*                                  interfaces.\n
*               led_cbrc_type - CBRC type, which is used for monitoring CBRC 
*                               channels only. For other interfaces, this field
*                               is discarded.
*  OUTPUTS:
*               None.
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_CFG_LED_Set_Monitoring_Control
*
******************************************************************************/
PUBLIC PMC_ERROR digi_fw_led_set_mon_ctrl(digi_handle_t *digi_handle,
                                          BOOL8 is_enable,
                                          UINT8 chnl_num,
                                          digi_led_interface_t led_interface,
                                          digi_led_cbrc_type_t led_cbrc_type)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];

    PMC_ENTRY();

    /* CMD_CFG_LED_Set_Monitoring_Control  has the following format:
     * Word |  bits 24..31 | bits 16..23 | bits 8..15 | bits 0..7  |
     *  0   |  interface   | channel     | enable     | cbrc_type  |
     */
    data[0] = ((UINT8)(led_interface)<<24) + (chnl_num<<16) +((UINT8)is_enable<<8) + ((UINT8)led_cbrc_type);

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_LED,HOSTMSG_CMD_CFG_LED_SET_MON_CTRL_ST,data,1,stat);
    }

    PMC_RETURN(result);
} /* digi_fw_led_set_mon_ctrl */

#ifdef USE_FW_LOG
/* defined in pmc_log.c, but having swig issues with variable argument lists */
void fwvLog( const char *fmt, va_list ap );
#endif

#endif /* DOXYGEN_PUBLIC_ONLY */
/*******************************************************************************
* digi_fw_performance_queue_init
* ___________________________________________________________________________
*
* DESCRIPTION:
*  This function will initialize the host memory and then call FW to set up
*  the performance queue. It can either turn this feature on or off. 
*
* INPUTS:
*  digi_handle - DIGI device handle.
*  enable - TRUE if we want to use the queue, FALSE to disable the queue
*
* OUTPUTS: - none
*
*  RETURNS:
*    PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
*  NOTES: CMD_REQ_PERFORMANCE_Init_fw_performance_queue.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_performance_queue_init(digi_handle_t *digi_handle, BOOL8 enable )
{
    PMC_ERROR result = PMC_SUCCESS;  
    pmc_handle_t *pmc_handle = NULL;
    UINT32 buf_size = sizeof(l5sys_shared_host_memory_t);
    size_t buf_align;

    pmc_sys_handle_t *sys_handle  = NULL;

    /* Shared memory variables */
    UINT32 host_shared_memory_addr_lo;
    UINT32 host_shared_memory_addr_hi;

    pmc_mem_pool_handle pool;
    l5pmc_sys_handle_t *h;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    h = *(l5pmc_sys_handle_t**)digi_handle->base.sys_handle;
    pmc_handle = &digi_handle->base;

#ifdef USE_FW_LOG
    if (digi_fw_firmware_logging_enabled) {
        l5sys_log_function(digi_handle,fwvLog);
    }
#endif

    /* just return TRUE if digi is REV_A */
    if (TRUE == pmc_is_digi_rev_a_revision((pmc_handle_t*)digi_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }
    if (h->pipe_created && enable) {
        PMC_LOG(PMC_LOG_SEV_HIGHEST, 0,
            0, 0,
            "request to enable firmware queue, but queue is already created\n");
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }
    if (!h->pipe_created && !enable) {
        PMC_LOG(PMC_LOG_SEV_HIGHEST, 0,
            0, 0,
            "firmware queue is already deleted\n");
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }

#if  defined(PMC_SW_SIMULATION)
    PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
#endif   

    sys_handle = digi_handle->base.sys_handle;

    if (enable) /* Set up shared memory */
    {
        if (h->shared_host_memory_ptr == NULL) {
            UINT64              buf_addr_phy;         /* physical base address of buffer allocated */
            PMC_UINTPTR         buf_addr_vir;

            /* Set up Shared Memory for the FW read result and the driver index. */

            /* ensure all bottom bits of address of the memory pool are zero */
            for (buf_align = 12;(UINT32)(1<<buf_align) < buf_size;buf_align++)
                ;
            PMC_LOG(PMC_LOG_SEV_MEDIUM, 0, buf_size, 1<<buf_align,
                    "allocating %d bytes (%d with alignment) for firmware queue\n",buf_size,1<<buf_align);

            /* Create memory pool handle */
            pool = pmc_mem_pool_create(sys_handle, buf_size, 1, 0, (pmc_mem_align_enum)buf_align); 

            /* Allocating memory. The return of this function is virtual address */
            buf_addr_vir = (PMC_UINTPTR)pmc_mem_pool_alloc(pool, buf_size);

            /* Convert virtual address to physical address for FW */
            buf_addr_phy = pmc_convert_mem_pool_addr_to_physical_addr(pool, (void *) buf_addr_vir);

            /* Check that physcal FW address is non zero */
            if (0x0 == buf_addr_phy)
            {
                result = DIGI_SDK_ERR_BUF_ADDR_NULL;
                PMC_ASSERT((result == PMC_SUCCESS), DIGI_SDK_ERR_BUF_ADDR_NULL, 0, 0);
            }

            /* Check that physical FW address' lower 12 bits are 0x0 
             * as proper input to digi_fw_performance_queue_init_spram_addr_get. Adjust otherwise
             */
            if (0x0 != (0xFFF & buf_addr_phy))
            {
                result = DIGI_SDK_ERR_BUF_ADDR_ERROR;
                PMC_ASSERT((result == PMC_SUCCESS), DIGI_SDK_ERR_BUF_ADDR_ERROR, 0, 0);

            }
            h->shared_host_memory_ptr = (l5sys_shared_host_memory_t *)buf_addr_vir;
            h->physical_address = buf_addr_phy;
            /*we also have buf_addr_vir - 3 paramaters we need back from the allocation of shared memory */
            /* Finished setting up Shared Memory */
            PMC_LOG(PMC_LOG_SEV_MEDIUM, PMC_SHARED_LOG_TRACE, 0, 0, "digi allocated firmware queue address at: %lx (%d bytes)\n", (unsigned long)h->physical_address,buf_size);
        }

        host_shared_memory_addr_lo = (UINT32)(h->physical_address & 0xFFFFFFFF);
        host_shared_memory_addr_hi = (UINT32)((h->physical_address >> 32) & 0xFFFFFFFF);
        /* initialize values to some recognizable values */
        h->shared_host_memory_ptr->dma_queue_fw_index = 12345678;
        h->shared_host_memory_ptr->poll_result = 0xf00deeee;
        h->shared_host_memory_ptr->read_result = 0xfeedcafe;
        h->shared_host_memory_ptr->should_be_0x12345678 = 0;
    }
    else
    {
        /* If disabling the queue, send in both zero values for the shared memory high and low */
        host_shared_memory_addr_lo = 0;
        host_shared_memory_addr_hi = 0;
    }

    /* Have a way to turn off the FW queue.. at this point the FW will have been turned off.. */
    if (!enable)
    {
        PMC_LOG(PMC_LOG_SEV_MEDIUM, PMC_SHARED_LOG_TRACE, 0, 0, "disabling firmware queue\n" );
        l5sys_wait_queue_done(pmc_handle);
        h->use_pipe = FALSE;
        h->pipe_created = FALSE;
    } 

    {
        UINT32 cfg_msg[5];
        UINT32 response[1];
        size_t offset_of_queue;
        HOSTMSG_DECLARE_QUERY_STAT(stat);

        cfg_msg[0] = host_shared_memory_addr_lo;
        cfg_msg[1] = host_shared_memory_addr_hi;
        cfg_msg[2] = buf_size;
        offset_of_queue = (size_t)&(h->shared_host_memory_ptr->pmcfw_dma_queue) - (size_t)h->shared_host_memory_ptr;
        cfg_msg[3] = offset_of_queue;
        cfg_msg[4] = sizeof(h->shared_host_memory_ptr->pmcfw_dma_queue)/sizeof(UINT32);
        result = hostmsg_perform_fixed_length_query(pmc_handle,
                                                    MODID_PERFORMANCE,
                                                    HOSTMSG_CMD_CFG_INIT_FW_PERFORMANCE_HOST_QUEUE_ST,
                                                    cfg_msg,5,
                                                    response,1,
                                                    stat);

        if (PMC_SUCCESS == result) {

            /*Turn on the queue in sw.. */
            if (enable)
            {
                UINT32 spram_address_of_host_index = response[0]&0x1ffffff;

                l5sys_init_queue(pmc_handle,spram_address_of_host_index);
            }
        }
    }
    if (PMC_SUCCESS == result)
    {
        if (enable) {
            digi_fw_log_init(digi_handle);
        }
    }

    if (PMC_SUCCESS == result)
    {
        if ((enable)) {
            result = l5sys_queue_init_test(digi_handle);
            if (PMC_SUCCESS != result) {
                h->pipe_created = FALSE;
                h->use_pipe = FALSE;
            }
        }
    }
    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* End: digi_fw_performance_queue_init */


/*******************************************************************************
* digi_fw_opu_csf_int_rsp
* ___________________________________________________________________________
*
* DESCRIPTION:
*  This function will return the statuses of the OPU CSF interrupts after checking
*  that the defect was valid for three full frames. 
*
* INPUTS:
*  digi_handle - DIGI device handle.
*  bits96_64 - bitmask for top 32  mapper channels. (1 ~ active, 0 ~ not active)
*  bits64_32 - bitmask for middle 32 mapper channels. (1 ~ active, 0 ~ not active)
*  bits32_0  - bitmask for lower 32 mapper channels. (1 ~ active, 0 ~ not active)
*
* OUTPUTS: - none
*
*  RETURNS:
*    PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
*  NOTES: CMD_RSP_CMF_OPU_CSF_STATUS
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_opu_csf_int_rsp(digi_handle_t *digi_handle, UINT32 *bits96_64, UINT32 *bits64_32, UINT32 *bits32_0 )
{
    PMC_ERROR result = PMC_SUCCESS; 
    UINT32 data[1];
    UINT32 response[3];


    PMC_ATOMIC_ENTRY(digi_handle);

#if defined(PMC_SW_SIMULATION)
    PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
#endif

    /* Build the rest of the message */
    /* We dont need this field yet - reserved */
    data[0] = 0x0;

    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_fixed_length_query((pmc_handle_t *)digi_handle,MODID_CMF,HOSTMSG_CMD_CMF_REQ_OPU_CSF_STAT_ST,data,1,response,3,stat);
    }


    /* Get result*/
    if (PMC_SUCCESS == result)
    {

        PMC_LOG(PMC_LOG_SEV_LOW, DIGI_LOG_OBND_DATA, response[0], 0,
                "data[0] = %x\n", response[0]);
        PMC_LOG(PMC_LOG_SEV_LOW, DIGI_LOG_OBND_DATA, response[1], 0,
                "data[1] = %x\n", response[1]);
        PMC_LOG(PMC_LOG_SEV_LOW, DIGI_LOG_OBND_DATA, response[2], 0,
                "data[2] = %x\n", response[2]);

        /* Return the statuses of the OPU CSF interrupts */
        *bits32_0 = response[0];
        *bits64_32 = response[1];
        *bits96_64 = response[2];

    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End: digi_fw_opu_csf_int_rsp */

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* digi_fw_intr_monitor_restart
* ___________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to restart interrupt monitors on ENET LINE//SYS and 
*   CBRC subsystem. 
*   This function is intended to be used after a firmware restart when 
*   auto-restart is not supported 
*
* INPUTS:
*   digi_handle - DIGI device handle
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_fw_intr_monitor_restart(digi_handle_t *digi_handle)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    if (digi_handle->var.fw_intr_mon_running[INTR_IPI_ENET_LINE] == TRUE)
    {
        /* Start ENET LINE interrupt monitors in FW */
        digi_fw_enet_intr_monitor_cfg(digi_handle, INTR_IPI_ENET_LINE_EPMM_CBR_RX_LOBL,   TRUE);
        digi_fw_enet_intr_monitor_cfg(digi_handle, INTR_IPI_ENET_LINE_ETRANS_CBR_RX_LOBL, TRUE);
    }

    if (digi_handle->var.fw_intr_mon_running[INTR_IPI_ENET_SYS] == FALSE)
    {
        /* Start ENET SYS interrupt monitors in FW */
        digi_fw_enet_intr_monitor_cfg(digi_handle, INTR_IPI_ENET_SYS_EPMM_CBR_RX_LOBL, TRUE);
    }


    if (digi_handle->var.fw_intr_mon_running[INTR_IPI_CBRC_FC1200] == TRUE)
    {
        /* Start CBRC interrupt monitors in FW */
        digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PCS_RX_RF,               TRUE);
        digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PCS_RX_LF,               TRUE);
        digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PCS_RX_LOSS_SYNC,        TRUE);
        digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PCS_RX_HIGH_BER,         TRUE);
        digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PCS_RX_LOS,              TRUE);
        digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PMON_ILVR_LOS_INT,       TRUE);
        digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PMON_ILVR_LOSS_SYNC_INT, TRUE);
        digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PMON_ILVR_HIGH_BER,      TRUE);
        digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PMON_DLVR_LOS_INT,       TRUE);
        digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PMON_DLVR_LOSS_SYNC_INT, TRUE);
        digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PMON_DLVR_HIGH_BER,      TRUE);
    }
    PMC_RETURN(rc);
} /* digi_fw_intr_monitor_restart */

/*******************************************************************************
* digi_fw_has_restart_support
* ___________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to determine if firmware restart is supported
*   for the current version 
*
* INPUTS:
*   digi_handle - DIGI device handle
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    BOOL8 - TRUE when firmware supports restart  
*
*******************************************************************************/
PRIVATE BOOL8 digi_fw_has_restart_support(digi_handle_t *digi_handle)
{
    BOOL8  result = FALSE;
    PMC_ENTRY();

    if ((digi_handle->digi_fw_status.fw_build ==  DIGI_FW_REVB_BUILD_DEVEL_MAGIC ||
         digi_handle->digi_fw_status.fw_build >= DIGI_FW_REVB_3_06_RESTART_BUILD_NUM) && 
        ( (digi_handle->digi_fw_status.fw_version & DIGI_FW_REVB_3_06_RELEASE_REV_MASK) >= DIGI_FW_REVB_3_06_RELEASE_REV))
    {
        result = TRUE;
    }
    
    PMC_RETURN(result);
}


/*******************************************************************************
* ARRAY: update_loader
* ______________________________________________________________________________
*
* DESCRIPTION:
*       This assembly loader sets up the initial FW message to
*   invalidate instruction cache at 0x9bd73e20 and jump to 0x9bd73e34
*
* ELEMENTS:
*       see Array.
*******************************************************************************/
static UINT32 update_loader[] = { 

0x3c189bd7,0x37183e20,0x03001825,0x10000004,0x270f1014,0xbc510000,0xbc500000,0x24630020,0x006f702b,0x55c0fffb, 
0x00601025,0x270d0014,0x01a0f809,0x00000000, 

};

/*******************************************************************************
* digi_fw_migrate_3_04_restart
* ___________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to migrate a FW 3.04 context to a context format 
*   supported by the firmware restart 
*
* INPUTS:
*   digi_handle - DIGI device handle
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*******************************************************************************/
PUBLIC PMC_ERROR  digi_fw_migrate_3_04_restart(digi_handle_t *digi_handle)
{
    
#define ADDRESS_OF_BENCH_MSG_TEST_IN_305           0x01d7415c
#define ADDRESS_OF_bench_test_function_IN_304    0x01d73e34 

    UINT32 i;
    PMC_ERROR rc;
    HOSTMSG_DECLARE_QUERY_STAT(stat);
    UINT32 update_bin[] = {

        0x3c09bb80,
        0x352901a8,
        0x34070003,
        0xad271e60,
        0x41626000,
        0x3c039bda,
        0x3c0a9bd8,
        0x354a09d8,
        0x34639818,
        0x8d460000,
        0xac666b68,
        0x8d450004,
        0x24686b78,
        0xac656b6c,
        0x8d440008,
        0x00603825,
        0xac646b70,
        0x8d42000c,
        0x34060410,
        0xac626b74,
        0x24e70004,
        0x25080004,
        0x8cf8fffc,
        0x24c6ffff,
        0x14c0fffb,
        0xad18fffc,
        0x3c0f9bd9,
        0x35eff800,
        0x01e02825,
        0x340442e0,
        0x24a50004,
        0x2484ffff,
        0x1480fffd,
        0xaca0fffc,
        0x24637f68,
        0x34021448,
        0x24630004,
        0x2442ffff,
        0x1440fffd,
        0xac60fffc,
        0x340d002c,
        0x004d001b,
        0x3c0b9bd8,
        0x356bdfc8,
        0x00022880,
        0x00ab2021,
        0x00007012,
        0x8c830000,
        0x000e5100,
        0x004a4021,
        0x00083880,
        0x00ef3021,
        0x24420001,
        0x2c580210,
        0x1700fff1,
        0xacc30480,
        0x25f80fc0,
        0x256e0840,
        0x340d0048,
        0x25ce0004,
        0x27180004,
        0x8dcafffc,
        0x25adffff,
        0x15a0fffb,
        0xaf0afffc,
        0x0000c025,
        0x34070028,
        0x0307001b,
        0x00181080,
        0x004b7021,
        0x00004012,
        0x8dcd4560,
        0x00083080,
        0x03062821,
        0x00052080,
        0x008f1821,
        0x27180001,
        0x2f0a0f00,
        0x1540fff3,
        0xac6d10e0,
        0x00006825,
        0x3c069bd9,
        0x34c6731e,
        0x000d5040,
        0x01463821,
        0x014f4021,
        0x94e50000,
        0x25ad0001,
        0x2da400c0,
        0x1480fff7,
        0xa50552e0,
        0x3c089bdb,
        0x10000003,
        0x350868a0,
        0xbc550000,
        0x25ef0020,
        0x01e8182b,
        0x5460fffc,
        0x01e01025,
        0x8d2e0000,
        0x000e7042,
        0x000e7040,
        0xad2e0000,
        0x3c0bbbdb,
        0x356bfc00,
        0x3c0fff0f,
        0x8d780100,
        0x3c0d00f0,
        0x030f1024,
        0x35ad0002,
        0x01a27025,
        0x8d6a0104,
        0xad6e0000,
        0xad6a0004,
        0x0000000f,
        0x34080001,
        0xad281088,
        0x42000020,
        0x1000fffe,
        0x00000000

    };                                                                                                                                 
 
    PMC_ENTRY();

    /* unlock pcie spram */
    hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,0x80,4,0,0,stat);
    {
        UINT32 addr  = ADDRESS_OF_BENCH_MSG_TEST_IN_305;
        UINT32 *data = update_loader;
        UINT32 len   = sizeof(update_loader)/sizeof(UINT32);

        /* reprogram the bench_msg function since this function is never accessed and should not be in the instruction cache */
        FW_LOG("downloading upgrade function (%d words) 0x%08x to 0x%08x\n",len,addr,addr+(len-1)*4);
        for (i=0;i<len;i++) {
            digi_reg_write(digi_handle,addr+i*4,data[i]);
        }
        for (i=0;i<len;i++) {
            UINT32 reg_val = digi_reg_read(digi_handle,addr+i*4);
            if (data[i] != reg_val) {
                FW_LOG("verify failed addr=0x%08x, wrote 0x%08x, read 0x%08x\n",addr+i*4,data[i],reg_val);
            }
        }
    }

    {
        UINT32 addr  = ADDRESS_OF_bench_test_function_IN_304;
        UINT32 *data = update_bin;
        UINT32 len   = sizeof(update_bin)/sizeof(UINT32);

        /* reprogram the bench_msg function since this function is never accessed and should not be in the instruction cache */
        FW_LOG("downloading upgrade function (%d words) 0x%08x to 0x%08x\n",len,addr,addr+(len-1)*4);
        for (i=0;i<len;i++) {
            digi_reg_write(digi_handle,addr+i*4,data[i]);
        }
        for (i=0;i<len;i++) {
            UINT32 reg_val = digi_reg_read(digi_handle,addr+i*4);
            if (data[i] != reg_val) {
                FW_LOG("verify failed addr=0x%08x, wrote 0x%08x, read 0x%08x\n",addr+i*4,data[i],reg_val);
            }
        }
    }

    /* call the bench_msg function */
    rc = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,0x82,1,0,0,stat);

    
    PMC_Q_USLEEP(digi_handle,DIGI_FW_STATUS_POLL_INTERVAL_IN_US);    

    PMC_RETURN(rc);
}

#endif /* DOXYGEN_PUBLIC_ONLY */
