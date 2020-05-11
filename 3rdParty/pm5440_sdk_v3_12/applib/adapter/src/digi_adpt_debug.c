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
*     The file describes the debug functionality of DIGI Adapter.
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

/*
** Local Functions
*/

/*
** Public Functions
*/
/*******************************************************************************
* adpt_help
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps all the debug APIs.
*
* INPUTS:
*   None.
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
PUBLIC void adpt_help()
{
    ADPT_PRINT("\r\n===============================================================\r\n");
    ADPT_PRINT("adpt_help                     - Dump DIGI Adapter help info.\r\n");
    ADPT_PRINT("adpt_dump_all dev             - Dump all info.\r\n");
    ADPT_PRINT("adpt_dump_reg dev,saddr,eaddr - Dump DIGI register info.\r\n");
    ADPT_PRINT("adpt_dump_basic dev           - Dump basic device info.\r\n");
    ADPT_PRINT("adpt_dump_data dev,chnl       - Dump internal memory info.\r\n");
    ADPT_PRINT("adpt_dump_odu_mux dev,chnl    - Dump ODUk TP Mux/Demux info.\r\n");
    ADPT_PRINT("adpt_dump_otn_cfg dev,chnl    - Dump OTN configuration info.\r\n");
    ADPT_PRINT("adpt_dump_otn_tcm dev,chnl    - Dump OTN TCM CFG info.\r\n");
    ADPT_PRINT("adpt_dump_otn_tti dev,chnl    - Dump OTN TTI configuration info.\r\n");
    ADPT_PRINT("adpt_dump_otn_alm dev,chnl    - Dump OTN alarm info.\r\n");
    ADPT_PRINT("adpt_dump_otn_alm_i dev,chnl  - Dump OTN alarm interrupt info.\r\n");
    ADPT_PRINT("adpt_dump_otn_pmon dev,chnl   - Dump OTN performance info.\r\n");
    ADPT_PRINT("adpt_dump_otn_int dev,chnl    - Dump OTN interrupt cfg info.\r\n");
    ADPT_PRINT("adpt_dump_sdh_cfg dev,chnl    - Dump SDH/SONET configuration info.\r\n");
    ADPT_PRINT("adpt_dump_sdh_alm dev,chnl    - Dump SDH/SONET alarm info.\r\n");
    ADPT_PRINT("adpt_dump_sdh_alm_i dev,chnl  - Dump SDH/SONET alarm interrupt info.\r\n");
    ADPT_PRINT("adpt_dump_sdh_pmon dev,chnl   - Dump SDH/SONET performance info.\r\n");
    ADPT_PRINT("adpt_dump_sdh_int dev,chnl    - Dump SDH/SONET interrupt cfg info.\r\n");
    ADPT_PRINT("adpt_dump_gfp_cfg dev,chnl    - Dump GFP configuration info.\r\n");
    ADPT_PRINT("adpt_dump_gfp_alm dev,chnl    - Dump GFP alarm info.\r\n");
    ADPT_PRINT("adpt_dump_gfp_alm_i dev,chnl  - Dump GFP alarm interrupt info.\r\n");
    ADPT_PRINT("adpt_dump_gfp_pmon dev,chnl   - Dump GFP performance info.\r\n");
    ADPT_PRINT("adpt_dump_gfp_int dev,chnl    - Dump GFP interrupt cfg info.\r\n");
    ADPT_PRINT("adpt_dump_enet_cfg dev,chnl   - Dump ENET configuration info.\r\n");
    ADPT_PRINT("adpt_dump_enet_alm dev,chnl   - Dump ENET alarm info.\r\n");
    ADPT_PRINT("adpt_dump_enet_alm_i dev,chnl - Dump ENET alarm interrupt info.\r\n");
    ADPT_PRINT("adpt_dump_enet_pmon dev,chnl  - Dump ENET performance info.\r\n");
    ADPT_PRINT("adpt_dump_enet_int dev,chnl   - Dump ENET interrupt cfg info.\r\n");
    ADPT_PRINT("adpt_dump_fc_alm dev,chnl     - Dump FC alarm info.\r\n");
    ADPT_PRINT("adpt_dump_fc_alm_i dev,chnl   - Dump FC alarm interrupt info.\r\n");
    ADPT_PRINT("adpt_dump_fc_pmon dev,chnl    - Dump FC performance info.\r\n");
    ADPT_PRINT("adpt_dump_fc_int dev,chnl     - Dump FC interrupt cfg info.\r\n");
    ADPT_PRINT("adpt_dump_ptp dev,chnl        - Dump PTP info.\r\n");
    ADPT_PRINT("adpt_dump_sifd_alm dev        - Dump SIFD alarm info.\r\n");
    ADPT_PRINT("adpt_dump_sifd_alm_i dev      - Dump SIFD alarm interrupt info.\r\n");
    ADPT_PRINT("adpt_dump_sifd_pmon dev       - Dump SIFD performance info.\r\n");
    ADPT_PRINT("adpt_dump_loopback dev,chnl   - Dump loopback info.\r\n");
    ADPT_PRINT("adpt_dump_serdes dev,chnl     - Dump SerDes values info.\r\n");
    ADPT_PRINT("adpt_dump_serdes_st dev       - Dump SerDes status info.\r\n");
    ADPT_PRINT("adpt_dump_sdk dev             - Dump DIGI SDK info.\r\n");
    ADPT_PRINT("adpt_dump_fw dev              - Dump DIGI firmware info.\r\n");
    ADPT_PRINT("adpt_dump_fw_log dev          - Dump DIGI firmware log info.\r\n");
    ADPT_PRINT("adpt_dump_pmon_cfg dev        - Dump PMON configuration info.\r\n");
    ADPT_PRINT("adpt_dump_pmon_data dev       - Dump PMON data info.\r\n");
    ADPT_PRINT("adpt_dump_int_tree dev        - Dump interrupt tree info.\r\n");
    ADPT_PRINT("adpt_dump_port_cfg dev        - Dump SGMII port cfg info.\r\n");
    ADPT_PRINT("adpt_dump_port_pmon dev,pt    - Dump SGMII port performance info.\r\n");
    ADPT_PRINT("adpt_dump_ri_rcp_pmon dev     - Dump RI_RCP performance info.\r\n");
    ADPT_PRINT("adpt_dump_di_rcp_pmon dev     - Dump DI_RCP performance info.\r\n");
    ADPT_PRINT("adpt_dump_mgmt_alm_i dev      - Dump MGMT alarm interrupt info.\r\n");
    ADPT_PRINT("adpt_dump_ohp_alm_i dev,inst  - Dump OHP alarm interrupt info.\r\n");
    ADPT_PRINT("adpt_dump_di_rcp_alm_i dev    - Dump DI_RCP alarm interrupt info.\r\n");
    ADPT_PRINT("adpt_dump_ri_rcp_alm_i dev    - Dump RI_RCP alarm interrupt info.\r\n");
    ADPT_PRINT("adpt_dump_dcsu dev            - Dump DCSU info.\r\n");
    ADPT_PRINT("===============================================================\r\n");
} /* adpt_help */


/*******************************************************************************
* adpt_dump_all
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the all information based on device. 
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dump_all(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* basic device information */
    rc = adpt_dump_basic(dev_id);
    for (index = 0; index < DIGI_NUM_CHNL_MAX; index++)
    {
        if (adpt_handle->traffic_info[index].enable)
        {
            ADPT_PRINT("dev_id[%d] chnl_id[%d] index[%d] \r\n", dev_id, 
                       adpt_handle->traffic_info[index].chnl_id, index);
                   
            /* internal data structure information */
            rc |= adpt_dump_data(dev_id, adpt_handle->traffic_info[index].chnl_id);
            
            /* ODUk signal */
            if (NULL != adpt_handle->traffic_info[index].otu_handle)
            {
                rc |= adpt_dump_odu_mux(dev_id, adpt_handle->traffic_info[index].chnl_id);
            }
            
            if ((NULL != adpt_handle->traffic_info[index].odu_handle)
                || (NULL != adpt_handle->traffic_info[index].otu_handle)
                || (NULL != adpt_handle->traffic_info[index].map_handle))
            {
                rc |= adpt_dump_otn_cfg(dev_id, adpt_handle->traffic_info[index].chnl_id);
                rc |= adpt_dump_otn_tti(dev_id, adpt_handle->traffic_info[index].chnl_id);
                rc |= adpt_dump_otn_alm(dev_id, adpt_handle->traffic_info[index].chnl_id);
                rc |= adpt_dump_otn_pmon(dev_id, adpt_handle->traffic_info[index].chnl_id);
            }

            /* SDH */
            if (NULL != adpt_handle->traffic_info[index].cbr_handle)
            {
                rc |= adpt_dump_sdh_cfg(dev_id, adpt_handle->traffic_info[index].chnl_id);
                rc |= adpt_dump_sdh_alm(dev_id, adpt_handle->traffic_info[index].chnl_id);
                rc |= adpt_dump_sdh_pmon(dev_id, adpt_handle->traffic_info[index].chnl_id);
            }

            /* Ethernet */
            if (NULL != adpt_handle->traffic_info[index].enet_handle)
            {
                rc |= adpt_dump_enet_cfg(dev_id, adpt_handle->traffic_info[index].chnl_id);
                rc |= adpt_dump_enet_alm(dev_id, adpt_handle->traffic_info[index].chnl_id);
                rc |= adpt_dump_enet_pmon(dev_id, adpt_handle->traffic_info[index].chnl_id);
                rc |= adpt_dump_ptp(dev_id, adpt_handle->traffic_info[index].chnl_id);
            }

            /* GFP */
            if (NULL != adpt_handle->traffic_info[index].map_handle)
            {
                rc |= adpt_dump_gfp_cfg(dev_id, adpt_handle->traffic_info[index].chnl_id);
                rc |= adpt_dump_gfp_alm(dev_id, adpt_handle->traffic_info[index].chnl_id);
                rc |= adpt_dump_gfp_pmon(dev_id, adpt_handle->traffic_info[index].chnl_id);
            }
        }
    }

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_all */


/*******************************************************************************
* adpt_dump_basic
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the basic device information. 
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dump_basic(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_sw_status_t sw_status;
    digi_fw_status_t fw_status;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);
    
    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    ADPT_PRINT("\r\n===============================================================\r\n");
    ADPT_PRINT("                       Software Status \r\n");
    PMC_MEMSET((void*)&sw_status, 0, sizeof(sw_status));
    rc |= digi_software_status_get(adpt_handle->digi_handle, &sw_status);
    ADPT_PRINT("sw version   : %d.%d.%d\n", sw_status.sw_version >> 20 & 0x000000FF,
           sw_status.sw_version >> 12 & 0x000000FF,
           sw_status.sw_version >> 0  & 0x00000FFF);
    ADPT_PRINT("reset reason : %d\n", sw_status.last_reset_reason);
    ADPT_PRINT("build target : 0x%x\n", sw_status.build_device_id);
    ADPT_PRINT("device id    : 0x%x\n", sw_status.device_id);
    ADPT_PRINT("revision code: %s\n", adpt_dev_version_str(sw_status.device_revision_code));
    ADPT_PRINT("reg rw status: %s\n", sw_status.read_write_status ? "OK" : "FAIL");

    ADPT_PRINT("---------------------------------------------------------------\r\n");
    ADPT_PRINT("                       Firmware Status \r\n");
    PMC_MEMSET((void*)&fw_status, 0, sizeof(fw_status));
    rc |= digi_firmware_status_get(adpt_handle->digi_handle, &fw_status);
    ADPT_PRINT("boot loader revision    : %x.%x.%x.%x\n",
               fw_status.bootloader_version >> 24 & 0x000000FF,
               fw_status.bootloader_version >> 16 & 0x000000FF,
               fw_status.bootloader_version >> 8  & 0x000000FF,
               fw_status.bootloader_version >> 0  & 0x000000FF);
    ADPT_PRINT("boot loader build       : %x-%x-%x\n",
               fw_status.bootloader_build >> 16 & 0x0000FFFF,
               fw_status.bootloader_build >> 8  & 0x000000FF,
               fw_status.bootloader_build >> 0  & 0x000000FF);
    ADPT_PRINT("fw revision             : %x.%x.%x.%x\n",
               fw_status.fw_version >> 24 & 0x000000FF,
               fw_status.fw_version >> 16 & 0x000000FF,
               fw_status.fw_version >> 8  & 0x000000FF,
               fw_status.fw_version >> 0  & 0x000000FF);
    ADPT_PRINT("fw build                : %x\n", fw_status.fw_build);
    ADPT_PRINT("fw is alive             : 0x%x\n", fw_status.fw_is_alive);
    ADPT_PRINT("fw version not supported: 0x%x\n", fw_status.fw_version_not_supported);
    ADPT_PRINT("gen excp detected       : 0x%x\n", fw_status.gen_excp_detected);
    ADPT_PRINT("sw rst detected         : 0x%x\n", fw_status.sw_rst_detected);
    ADPT_PRINT("wdog rst detected       : 0x%x\n", fw_status.wdog_rst_detected);
    ADPT_PRINT("fw is ready             : 0x%x\n", fw_status.fw_is_ready);
    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_basic */


/*******************************************************************************
* adpt_dump_reg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the DIGI device register information. 
*
* INPUTS:
*   dev_id             - device identity.
*   start_addr         - DIGI device start register address.
*   end_addr           - DIGI device end register address.
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
PUBLIC PMC_ERROR adpt_dump_reg(UINT32 dev_id, UINT32 start_addr, UINT32 end_addr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 start_reg_addr = 0;
    UINT32 end_reg_addr = 0;
    UINT32 index = 0;
    UINT32 len = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    if (end_addr < start_addr)
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* in line with 32 bit */
    start_reg_addr = start_addr - start_addr % 4;
    end_reg_addr = end_addr - end_addr % 4;
    len = (end_reg_addr - start_reg_addr) / 4 + 1;

    ADPT_PRINT("\r\n===============================================================");
    for (index = 0; index < len; index++)
    {
        if ((index % 4) == 0)
        {
            ADPT_PRINT("\r\n0x%08x: ", (start_reg_addr + 4 * index));
        }
        ADPT_PRINT("  %08x", adpt_reg_read(dev_id, (start_reg_addr + 4 * index)));
    }
    ADPT_PRINT("\r\n===============================================================\r\n");

    PMC_RETURN(rc);
} /* adpt_dump_reg */


/*******************************************************************************
* adpt_dump_data
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the digi_cfg_container_t data structure information. 
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
PUBLIC PMC_ERROR adpt_dump_data(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 index = 0;
    UINT32 max_chnl_num = 0;
    BOOL chnl_enable = FALSE;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_DEFAULT_CHNL_ID == chnl_id)
    {
        /* dump all valid channels */
        index        = 0;
        max_chnl_num = DIGI_NUM_CHNL_MAX;
        chnl_enable  = FALSE;
    }
    else
    {
        /* get the index in traffic container */
        rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
        ADPT_RETURN_VERIFY(rc);

        max_chnl_num = index + 1;
        chnl_enable  = TRUE;
    }

    ADPT_PRINT("\r\n===============================================================\r\n");
    ADPT_PRINT(" init_state               = [%d] \r\n", adpt_handle->init_state);
    ADPT_PRINT(" digi_handle              = [%p] \r\n", adpt_handle->digi_handle);
    ADPT_PRINT(" sys_handle               = [%p] \r\n", adpt_handle->sys_handle);
    ADPT_PRINT(" dev_info                 = [%p] \r\n", adpt_handle->dev_info);
    ADPT_PRINT(" ilkn_1_ctxt              = [%p] \r\n", adpt_handle->ilkn_1_ctxt);
    ADPT_PRINT(" ilkn_2_ctxt              = [%p] \r\n", adpt_handle->ilkn_2_ctxt);
    ADPT_PRINT(" dp_mode                  = [%d] \r\n", adpt_handle->dp_mode);
    ADPT_PRINT(" di_rcp_en                = [%d] \r\n", adpt_handle->di_rcp_en);
    ADPT_PRINT(" ri_rcp_en                = [%d] \r\n", adpt_handle->ri_rcp_en);
    for (;index < max_chnl_num; index++)
    {
        if (adpt_handle->traffic_info[index].enable || chnl_enable)
        {
            ADPT_PRINT(" ---------------------------------------------------------------\r\n");
            ADPT_PRINT(" index                    = [%d] \r\n", index);
            ADPT_PRINT(" chnl_id                  = [%d] \r\n", adpt_handle->traffic_info[index].chnl_id);
            ADPT_PRINT(" traffic_mode             = [0x%x] \r\n", adpt_handle->traffic_info[index].traffic_mode);
            ADPT_PRINT(" ho_chnl_id               = [%d] \r\n", adpt_handle->traffic_info[index].ho_chnl_id);
            ADPT_PRINT(" mapping_type             = [%d] \r\n", adpt_handle->traffic_info[index].mapping_type);
            ADPT_PRINT(" oduk_type                = [%s] \r\n", adpt_odu_type_get(adpt_handle->traffic_info[index].oduk_type));
            ADPT_PRINT(" payload_type             = [%d] \r\n", adpt_handle->traffic_info[index].payload_type);
            ADPT_PRINT(" otn_dir                  = [%s] \r\n", adpt_otn_dir_str(adpt_handle->traffic_info[index].otn_dir));
            ADPT_PRINT(" tx_ts_mask[0]            = [0x%x] \r\n", adpt_handle->traffic_info[index].tx_ts_mask[0]);
            ADPT_PRINT(" tx_ts_mask[1]            = [0x%x] \r\n", adpt_handle->traffic_info[index].tx_ts_mask[1]);
            ADPT_PRINT(" tx_ts_mask[2]            = [0x%x] \r\n", adpt_handle->traffic_info[index].tx_ts_mask[2]);
            ADPT_PRINT(" rx_ts_mask[0]            = [0x%x] \r\n", adpt_handle->traffic_info[index].rx_ts_mask[0]);
            ADPT_PRINT(" rx_ts_mask[1]            = [0x%x] \r\n", adpt_handle->traffic_info[index].rx_ts_mask[1]);
            ADPT_PRINT(" rx_ts_mask[2]            = [0x%x] \r\n", adpt_handle->traffic_info[index].rx_ts_mask[2]);
            ADPT_PRINT(" oduflex_rate             = [%f] \r\n", adpt_handle->traffic_info[index].oduflex_rate);
            ADPT_PRINT(" tcm_pool_id[line][sk][0] = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[0][0]);
            ADPT_PRINT(" tcm_pool_id[line][sk][1] = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[0][1]);
            ADPT_PRINT(" tcm_pool_id[line][sk][2] = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[0][2]);
            ADPT_PRINT(" tcm_pool_id[line][sk][3] = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[0][3]);
            ADPT_PRINT(" tcm_pool_id[line][sk][4] = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[0][4]);
            ADPT_PRINT(" tcm_pool_id[line][sk][5] = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[0][5]);
            ADPT_PRINT(" tcm_pool_id[line][sk][6] = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[0][6]);
            ADPT_PRINT(" tcm_pool_id[line][so][0] = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[1][0]);
            ADPT_PRINT(" tcm_pool_id[line][so][1] = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[1][1]);
            ADPT_PRINT(" tcm_pool_id[line][so][2] = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[1][2]);
            ADPT_PRINT(" tcm_pool_id[line][so][3] = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[1][3]);
            ADPT_PRINT(" tcm_pool_id[line][so][4] = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[1][4]);
            ADPT_PRINT(" tcm_pool_id[line][so][5] = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[1][5]);
            ADPT_PRINT(" tcm_pool_id[line][so][6] = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[1][6]);
            ADPT_PRINT(" tcm_pool_id[sys][sk][0]  = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[2][0]);
            ADPT_PRINT(" tcm_pool_id[sys][sk][1]  = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[2][1]);
            ADPT_PRINT(" tcm_pool_id[sys][sk][2]  = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[2][2]);
            ADPT_PRINT(" tcm_pool_id[sys][sk][3]  = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[2][3]);
            ADPT_PRINT(" tcm_pool_id[sys][sk][4]  = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[2][4]);
            ADPT_PRINT(" tcm_pool_id[sys][sk][5]  = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[2][5]);
            ADPT_PRINT(" tcm_pool_id[sys][sk][6]  = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[2][6]);
            ADPT_PRINT(" tcm_pool_id[sys][so][0]  = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[3][0]);
            ADPT_PRINT(" tcm_pool_id[sys][so][1]  = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[3][1]);
            ADPT_PRINT(" tcm_pool_id[sys][so][2]  = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[3][2]);
            ADPT_PRINT(" tcm_pool_id[sys][so][3]  = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[3][3]);
            ADPT_PRINT(" tcm_pool_id[sys][so][4]  = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[3][4]);
            ADPT_PRINT(" tcm_pool_id[sys][so][5]  = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[3][5]);
            ADPT_PRINT(" tcm_pool_id[sys][so][6]  = [0x%x] \r\n", adpt_handle->traffic_info[index].tcm_pool_id[3][6]);
            ADPT_PRINT(" prbs_chid                = [%d] \r\n", adpt_handle->traffic_info[index].prbs_chid);
            ADPT_PRINT(" prbs_enable              = [%d] \r\n", adpt_handle->traffic_info[index].prbs_enable);
            ADPT_PRINT(" odu_stage                = [%d] \r\n", adpt_handle->traffic_info[index].odu_stage);
            ADPT_PRINT(" odu_handle               = [%p] \r\n", adpt_handle->traffic_info[index].odu_handle);
            ADPT_PRINT(" fec_type                 = [%d] \r\n", adpt_handle->traffic_info[index].fec_type);
            ADPT_PRINT(" otu_handle               = [%p] \r\n", adpt_handle->traffic_info[index].otu_handle);
            ADPT_PRINT(" enet_handle              = [%p] \r\n", adpt_handle->traffic_info[index].enet_handle);
            ADPT_PRINT(" prbs_mode                = [%d] \r\n", adpt_handle->traffic_info[index].prbs_mode);
            ADPT_PRINT(" cbr_handle               = [%p] \r\n", adpt_handle->traffic_info[index].cbr_handle);
            ADPT_PRINT(" mapping_mode             = [%d] \r\n", adpt_handle->traffic_info[index].mapping_mode);
            ADPT_PRINT(" ts_number                = [%d] \r\n", adpt_handle->traffic_info[index].ts_number);
            ADPT_PRINT(" map_enable               = [%d] \r\n", adpt_handle->traffic_info[index].map_enable);
            ADPT_PRINT(" map_handle               = [%p] \r\n", adpt_handle->traffic_info[index].map_handle);
            ADPT_PRINT(" ilkn_handle              = [%p] \r\n", adpt_handle->traffic_info[index].ilkn_handle);
            ADPT_PRINT(" ilkn_info                = [0x%x] \r\n", adpt_handle->traffic_info[index].ilkn_info);
            ADPT_PRINT(" cpb_src_index            = [0x%x] \r\n", adpt_handle->traffic_info[index].cpb_src_index);
            ADPT_PRINT(" oduk_src_index           = [0x%x] \r\n", adpt_handle->traffic_info[index].oduk_src_index);
            ADPT_PRINT(" map_sw_info              = [0x%x] \r\n", adpt_handle->traffic_info[index].map_sw_info);
            ADPT_PRINT(" ghao_id                  = [0x%x] \r\n", adpt_handle->traffic_info[index].ghao_id);
            ADPT_PRINT(" ghao_enable              = [0x%x] \r\n", adpt_handle->traffic_info[index].ghao_enable);
            ADPT_PRINT(" enable                   = [0x%x] \r\n", adpt_handle->traffic_info[index].enable);
        }
    }
    ADPT_PRINT("===============================================================\r\n");

    PMC_RETURN(rc);
} /* adpt_dump_data */


/*******************************************************************************
* adpt_dump_odu_mux_all
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps ODUk TP Mux/Demux information recovered from digi_cfg_container_t 
*   data structure. 
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
*   This is an internal API.
*
*******************************************************************************/
PRIVATE PMC_ERROR adpt_dump_odu_mux_all(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 index = 0;
    UINT32 i = 0;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    for (i = 0; i < DIGI_NUM_CHNL_MAX; i++)
    {
        if ((i != index) 
            && adpt_handle->traffic_info[i].enable
            && (adpt_handle->traffic_info[i].ho_chnl_id == chnl_id))
        {
            rc |= adpt_dump_odu_mux_all(dev_id, adpt_handle->traffic_info[i].chnl_id);
        }    
    }

    ADPT_PRINT("  [%d]   [%d]     [%d]       [%s]   [0x%x%x%x]   [0x%x%x%x]\r\n", 
               index, adpt_handle->traffic_info[index].chnl_id, 
               adpt_handle->traffic_info[index].ho_chnl_id, 
               adpt_odu_type_get(adpt_handle->traffic_info[index].oduk_type),
               adpt_handle->traffic_info[index].tx_ts_mask[2],
               adpt_handle->traffic_info[index].tx_ts_mask[1],
               adpt_handle->traffic_info[index].tx_ts_mask[0],
               adpt_handle->traffic_info[index].rx_ts_mask[2],
               adpt_handle->traffic_info[index].rx_ts_mask[1],
               adpt_handle->traffic_info[index].rx_ts_mask[0]);

    PMC_RETURN(rc);
} /* adpt_dump_odu_mux_all */


/*******************************************************************************
* adpt_dump_odu_mux
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps ODUk TP Mux/Demux information recovered from digi_cfg_container_t 
*   data structure. 
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
PUBLIC PMC_ERROR adpt_dump_odu_mux(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    ADPT_PRINT("\r\n==========================================================\r\n");
    ADPT_PRINT(" index chnl_id ho_chnl_id oduk_type tx_ts_mask rx_ts_mask\r\n");
    rc = adpt_dump_odu_mux_all(dev_id, chnl_id);
    ADPT_PRINT("==========================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_odu_mux */


/*******************************************************************************
* adpt_dump_otn_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the OTN configuration information. 
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
PUBLIC PMC_ERROR adpt_dump_otn_cfg(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    pmc_log_sev_t log_sev;
    UINT8 pt_value = 0;
    UINT8 ftfl_value[20];
    UINT32 i = 0;
    digi_otn_seg_t seg_id = DIGI_OTN_SEG_PM;
    digi_otn_tim_mode_t tim_mode = DIGI_TIM_MODE_OFF;
    BOOL enable = FALSE;
    BOOL enable1 = FALSE;
    UINT8 stat_value = 0;
    digi_mt_type_t mt_type = DIGI_MT_NORMAL;
    UINT16 gcc_value = 0;
    UINT32 deg_th = 0;
    UINT32 deg_m = 0;
    digi_tcm_mode_t tcm_mode = DIGI_TCM_TRANSPARENT;
    BOOL oh_src = FALSE;
    UINT16 tcm_pool_id_so = 0;
    UINT16 tcm_pool_id_sk = 0;
    BOOL defect_act = FALSE;
    UINT16 exp_value = 0;
    UINT8 res_value = 0;
    BOOL root_node = FALSE;
    digi_otn_server_fec_t fec_type = DIGI_OTN_SERVER_FEC_NONE;
    UINT32 err_rate = 0;
    digi_otn_err_mode_t err_mode = DIGI_OTN_ERR_NORMAL;
    UINT8 tcm_act_value = 0;
    digi_otn_prbs_pattern_t prbs_mode = DIGI_OTN_PRBS_31;
    UINT8 msi_value[80];
    UINT32 msi_len = 0;
    BOOL lo_odu = FALSE;
    digi_otn_ddeg_mon_seg_t odu_seg = DIGI_OTN_DDEG_MON_LINE;
    digi_otn_ddeg_mode_t deg_mode = DIGI_OTN_HARDWARE_DDEG;
    digi_deg_interval_t interval = DIGI_DEG_10MS;
    digi_tcm_pri_t tcm_pri = DIGI_TCM_PRI_0;
    digi_otn_loc_t location = DIGI_OTN_LINE_SIDE;
    UINT32 oh_value = 0;
    digi_oh_inst_type_t oh_type = DIGI_OH_INST_DISABLE;
    UINT32 oh_value1 = 0;
    digi_oh_inst_type_t oh_type1 = DIGI_OH_INST_DISABLE;
    UINT32 aps_value = 0; 
    UINT32 prbs_chid = 0;
    UINT32 traffic_loc = 0;
    odu_ohp_port_framer_t framer_num = ODU_OHP_PORT_FRAMER_LAST;
    UINT32 dci_chnl = 0;
    digi_dm_mode_t dm_mode = LAST_DIGI_DM;
    digi_odu_line_payload_t ts_type = DIGI_ODU_UNCHANNELIZED;
    UINT32 tcm_index = 0; 
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* check root node for chnl_id */
    if (NULL != adpt_handle->traffic_info[index].otu_handle)
    {
        root_node = TRUE;
    }

    ADPT_PRINT("\r\n=================================================================================\r\n");
    if (root_node)
    {
        /* OTU configuration */
        ADPT_PRINT(" fec_type fec_en fec_err_ins tim_mode tim_act bdi_ins mt_so mt_sk deg_m deg_th\r\n");
        rc = adpt_otn_fec_type_get(dev_id, chnl_id, &fec_type);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("  [%s]", adpt_fec_type_str(fec_type));
        rc = adpt_otn_fec_en_get(dev_id, chnl_id, &enable);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("    [%d]", enable);
        if (DIGI_PROD_APP_SYSOTN_CARD == adpt_handle->dev_info->card_type)
        {
            err_rate = 0;
            err_mode = DIGI_OTN_ERR_NORMAL;
            enable   = FALSE;
        }
        else
        {
            rc = adpt_otn_fec_err_inst_get(dev_id, chnl_id, &err_rate, &err_mode, &enable);
            ADPT_RETURN_VERIFY(rc);
        }
        ADPT_PRINT("    [%d %d %d]", err_rate, err_mode, enable);
        rc = adpt_otn_tim_det_mode_get(dev_id, 
                                       chnl_id, 
                                       DIGI_OTN_SEG_SM, 
                                       DIGI_OTN_LINE_SIDE, 
                                       &tim_mode);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("    [%s]", adpt_tim_mode_str(tim_mode));
        rc = adpt_otn_tim_act_dis_get(dev_id, 
                                      chnl_id, 
                                      DIGI_OTN_SEG_SM, 
                                      DIGI_OTN_LINE_SIDE, 
                                      &enable);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("    [%d]", !enable);
        rc = adpt_otn_tx_bdi_get(dev_id, 
                                  chnl_id, 
                                  DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE,
                                  &enable);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("     [%d]", enable);
        rc = adpt_otn_mt_sig_get(dev_id, 
                                 chnl_id, 
                                 DIGI_OTN_SEG_SM, 
                                 DIGI_OTN_LINE_SIDE, 
                                 DIGI_OTN_SOURCE_POS, 
                                 &mt_type);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("  [%s]", adpt_mt_type_str(mt_type));
        rc = adpt_otn_mt_sig_get(dev_id, 
                                 chnl_id, 
                                 DIGI_OTN_SEG_SM, 
                                 DIGI_OTN_LINE_SIDE, 
                                 DIGI_OTN_SINK_POS, 
                                 &mt_type);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT(" [%s]", adpt_mt_type_str(mt_type));
        rc = adpt_otn_deg_para_get(dev_id, 
                                   chnl_id, 
                                   DIGI_OTN_SEG_SM, 
                                   DIGI_OTN_LINE_SIDE, 
                                   &deg_th, 
                                   &deg_m);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT(" [%d]   [%d]\r\n", deg_m, deg_th);

        ADPT_PRINT(" Tx_GCC0 Rx_GCC0 GCC0_ins Tx_RES(1_10) Rx_RES RES_ins Tx_RES(1_13) Rx_RES RES_ins\r\n");
        rc = adpt_otn_tx_gcc_get(dev_id, chnl_id, DIGI_OTN_GCC_0, &gcc_value);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT(" [0x%04x]", gcc_value);
        rc = adpt_otn_rx_gcc_get(dev_id, chnl_id, DIGI_OTN_GCC_0, &gcc_value);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("[0x%04x]", gcc_value);
        rc = adpt_otn_tx_gcc_src_get(dev_id, chnl_id, DIGI_OTN_GCC_0, &oh_src);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("   [%d]", oh_src);
        rc = adpt_otn_tx_res_get(dev_id, chnl_id, DIGI_RES_1_10, &res_value);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("      [0x%02x]", res_value);
        rc = adpt_otn_rx_res_get(dev_id, chnl_id, DIGI_RES_1_10, &res_value);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("    [0x%02x]", res_value);
        rc = adpt_otn_tx_res_src_get(dev_id, chnl_id, DIGI_RES_1_10, &oh_src);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("   [%d]", oh_src);
        rc = adpt_otn_tx_res_get(dev_id, chnl_id, DIGI_RES_1_13, &res_value);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("      [0x%02x]", res_value);
        rc = adpt_otn_rx_res_get(dev_id, chnl_id, DIGI_RES_1_13, &res_value);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("    [0x%02x]", res_value);
        rc = adpt_otn_tx_res_src_get(dev_id, chnl_id, DIGI_RES_1_13, &oh_src);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("   [%d]\r\n", oh_src);

        ADPT_PRINT(" Tx_RES(1_14) Rx_RES RES_ins Tx_BDI Tx_BEI Tx_IAE Tx_BIP8 Tx_MFAS    Tx_FAS\r\n");
        rc = adpt_otn_tx_res_get(dev_id, chnl_id, DIGI_RES_1_14, &res_value);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("    [0x%02x]", res_value);
        rc = adpt_otn_rx_res_get(dev_id, chnl_id, DIGI_RES_1_14, &res_value);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("    [0x%02x]", res_value);
        rc = adpt_otn_tx_res_src_get(dev_id, chnl_id, DIGI_RES_1_14, &oh_src);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("   [%d]", oh_src);
        rc = adpt_otn_tx_otu_oh_get(dev_id, chnl_id, DIGI_OTU_OH_BDI, &oh_value, &oh_type);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("   [%d %d]", oh_value, oh_type);
        rc = adpt_otn_tx_otu_oh_get(dev_id, chnl_id, DIGI_OTU_OH_BEI_BIAE, &oh_value, &oh_type);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("  [%d %d]", oh_value, oh_type);
        rc = adpt_otn_tx_otu_oh_get(dev_id, chnl_id, DIGI_OTU_OH_IAE, &oh_value, &oh_type);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("  [%d %d]", oh_value, oh_type);
        rc = adpt_otn_tx_otu_oh_get(dev_id, chnl_id, DIGI_OTU_OH_BIP8, &oh_value, &oh_type);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("   [%d %d]", oh_value, oh_type);
        rc = adpt_otn_tx_otu_oh_get(dev_id, chnl_id, DIGI_OTU_OH_MFAS, &oh_value, &oh_type);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT(" [%d %d]", oh_value, oh_type);
        rc = adpt_otn_tx_otu_oh_get(dev_id, chnl_id, DIGI_OTU_OH_FAS_F6, &oh_value, &oh_type);
        ADPT_RETURN_VERIFY(rc);
        rc = adpt_otn_tx_otu_oh_get(dev_id, chnl_id, DIGI_OTU_OH_FAS_28, &oh_value1, &oh_type1);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT(" [0x%x%x %d %d]\r\n", oh_value, oh_value1, oh_type, oh_type1);

        ADPT_PRINT(" tx_aps_pcc rx_aps_pcc aps_ins dci_chnl frm_num\r\n");
        rc = adpt_otn_tx_aps_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, &aps_value);
        ADPT_PRINT(" [0x%08x]", aps_value);
        ADPT_RETURN_VERIFY(rc);
        rc = adpt_otn_rx_aps_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, &aps_value);
        ADPT_PRINT(" [0x%08x]", aps_value);
        ADPT_RETURN_VERIFY(rc);
        rc = adpt_otn_tx_aps_src_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, &oh_src);
        ADPT_PRINT(" [%d]", oh_src);
        ADPT_RETURN_VERIFY(rc);
        rc = adpt_otn_ohp_chnl_num_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, &dci_chnl, &framer_num);
        ADPT_PRINT("    [%d]     [%d]\r\n", dci_chnl, framer_num);
        ADPT_RETURN_VERIFY(rc);
    }
    else
    {
        /* ODTU configuration */
        ADPT_PRINT(" mt_sig_so mt_sig_sk tx_aps_pcc rx_aps_pcc aps_ins dci_chnl frm_num\r\n");
        rc = adpt_otn_mt_sig_get(dev_id, 
                                 chnl_id, 
                                 DIGI_OTN_SEG_SM, 
                                 DIGI_OTN_LINE_SIDE, 
                                 DIGI_OTN_SOURCE_POS, 
                                 &mt_type);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("  [%s]", adpt_mt_type_str(mt_type));
        rc = adpt_otn_mt_sig_get(dev_id, 
                                 chnl_id, 
                                 DIGI_OTN_SEG_SM, 
                                 DIGI_OTN_LINE_SIDE, 
                                 DIGI_OTN_SINK_POS, 
                                 &mt_type);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("    [%s]", adpt_mt_type_str(mt_type));
        rc = adpt_otn_tx_aps_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, &aps_value);
        ADPT_PRINT(" [0x%08x]", aps_value);
        ADPT_RETURN_VERIFY(rc);
        rc = adpt_otn_rx_aps_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, &aps_value);
        ADPT_PRINT(" [0x%08x]", aps_value);
        ADPT_RETURN_VERIFY(rc);
        rc = adpt_otn_tx_aps_src_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, &oh_src);
        ADPT_PRINT(" [%d]", oh_src);
        ADPT_RETURN_VERIFY(rc);
        rc = adpt_otn_ohp_chnl_num_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, &dci_chnl, &framer_num);
        ADPT_PRINT("      [%d]     [%d]\r\n", dci_chnl, framer_num);
        ADPT_RETURN_VERIFY(rc);
    }

    if (NULL != adpt_handle->traffic_info[index].odu_handle)
    {
        ADPT_PRINT(" -------------------------------------LINE---------------------------------------\r\n");
    }
    else
    {
        ADPT_PRINT(" --------------------------------------------------------------------------------\r\n");
    }
    ADPT_PRINT(" Tx_PT  Exp_PT Rx_PT  Tx_GCC1  Rx_GCC1 GCC1_ins Tx_GCC2  Rx_GCC2 GCC2_ins LTC_ACT\r\n");
    adpt_lo_odu_status_get(dev_id, chnl_id, &lo_odu);
    if ((!lo_odu) || (NULL != adpt_handle->traffic_info[index].map_handle))
    {
        rc = adpt_otn_tx_pt_get(dev_id, chnl_id, &pt_value);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT(" [0x%02x]", pt_value);
        rc = adpt_otn_exp_pt_get(dev_id, chnl_id, &pt_value);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT(" [0x%02x]", pt_value);
        rc = adpt_otn_rx_pt_get(dev_id, chnl_id, &pt_value);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT(" [0x%02x]", pt_value);
    }
    else
    {
        ADPT_PRINT(" [N/A]  [N/A]  [N/A] ");
    }
    rc = adpt_otn_tx_gcc_get(dev_id, chnl_id, DIGI_OTN_GCC_1, &gcc_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT(" [0x%04x]", gcc_value);
    rc = adpt_otn_rx_gcc_get(dev_id, chnl_id, DIGI_OTN_GCC_1, &gcc_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT(" [0x%04x]", gcc_value);
    rc = adpt_otn_tx_gcc_src_get(dev_id, chnl_id, DIGI_OTN_GCC_1, &oh_src);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("   [%d]", oh_src);
    rc = adpt_otn_tx_gcc_get(dev_id, chnl_id, DIGI_OTN_GCC_2, &gcc_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("   [0x%04x]", gcc_value);
    rc = adpt_otn_rx_gcc_get(dev_id, chnl_id, DIGI_OTN_GCC_2, &gcc_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT(" [0x%04x]", gcc_value);
    rc = adpt_otn_tx_gcc_src_get(dev_id, chnl_id, DIGI_OTN_GCC_2, &oh_src);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("   [%d]", oh_src);
    rc = adpt_otn_conseq_act_get(dev_id, chnl_id, DIGI_LTC_ACT, &defect_act);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("     [%d]\r\n", defect_act);

    ADPT_PRINT(" AIS_ACT OCI_ACT LCK_ACT PLM_SSF CSF_SSF MSIM_SSF LOOMFI_SSF LOFLOM_A Tx_EXP Rx_EXP\r\n");
    rc = adpt_otn_conseq_act_get(dev_id, chnl_id, DIGI_AIS_ACT, &defect_act);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("   [%d]", defect_act);
    rc = adpt_otn_conseq_act_get(dev_id, chnl_id, DIGI_OCI_ACT, &defect_act);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("     [%d]", defect_act);
    rc = adpt_otn_conseq_act_get(dev_id, chnl_id, DIGI_LCK_ACT, &defect_act);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("     [%d]", defect_act);
    traffic_loc = DIGI_TRAFFIC_POS_GET(adpt_handle->traffic_info[index].traffic_mode);
    if (((DIGI_ODU_STRUCT_LEVEL_LO_ODU != adpt_handle->traffic_info[index].odu_stage)
        && (DIGI_PROD_APP_SYSOTN_CARD != adpt_handle->dev_info->card_type))
        || ((DIGI_ODU_STRUCT_LEVEL_HO_ODU == adpt_handle->traffic_info[index].odu_stage)
        && (DIGI_PROD_APP_SYSOTN_CARD == adpt_handle->dev_info->card_type)
        && (DIGI_SYS_SIDE == traffic_loc))
        || (NULL != adpt_handle->traffic_info[index].map_handle))
    {
        rc = adpt_otn_conseq_act_get(dev_id, chnl_id, DIGI_PLM_TO_SSF, &defect_act);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("     [%d]", defect_act);
        rc = adpt_otn_conseq_act_get(dev_id, chnl_id, DIGI_CSF_TO_SSF, &defect_act);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("     [%d]", defect_act);
        rc = adpt_otn_conseq_act_get(dev_id, chnl_id, DIGI_MSIM_TO_SSF, &defect_act);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("      [%d]", defect_act);
        rc = adpt_otn_conseq_act_get(dev_id, chnl_id, DIGI_LOOMFI_TO_SSF, &defect_act);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("      [%d]", defect_act);
    }
    else
    {
        ADPT_PRINT("     [%d]", FALSE);
        ADPT_PRINT("     [%d]", FALSE);
        ADPT_PRINT("      [%d]", FALSE);
        ADPT_PRINT("      [%d]", FALSE);
    }
    rc = adpt_otn_conseq_act_get(dev_id, chnl_id, DIGI_LOFLOM_ACT, &defect_act);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("      [%d]", defect_act);
    rc = adpt_otn_tx_exp_get(dev_id, chnl_id, &exp_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("  [0x%04x]", exp_value);
    rc = adpt_otn_rx_exp_get(dev_id, chnl_id, &exp_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("[0x%04x]\r\n", exp_value);

    ADPT_PRINT(" EXP_ins Tx_RES(2_1) Rx_RES RES_ins Tx_RES(2_2) Rx_RES RES_ins Tx_RES(2_3) Rx_RES\r\n");
    rc = adpt_otn_tx_exp_src_get(dev_id, chnl_id, &oh_src);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("   [%d]", oh_src);
    rc = adpt_otn_tx_res_get(dev_id, chnl_id, DIGI_RES_2_1, &res_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("      [0x%02x]", res_value);
    rc = adpt_otn_rx_res_get(dev_id, chnl_id, DIGI_RES_2_1, &res_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("   [0x%02x]", res_value);
    rc = adpt_otn_tx_res_src_get(dev_id, chnl_id, DIGI_RES_2_1, &oh_src);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("   [%d]", oh_src);
    rc = adpt_otn_tx_res_get(dev_id, chnl_id, DIGI_RES_2_2, &res_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("      [0x%02x]", res_value);
    rc = adpt_otn_rx_res_get(dev_id, chnl_id, DIGI_RES_2_2, &res_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("   [0x%02x]", res_value);
    rc = adpt_otn_tx_res_src_get(dev_id, chnl_id, DIGI_RES_2_2, &oh_src);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("   [%d]", oh_src);
    rc = adpt_otn_tx_res_get(dev_id, chnl_id, DIGI_RES_2_3, &res_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("      [0x%02x]", res_value);
    rc = adpt_otn_rx_res_get(dev_id, chnl_id, DIGI_RES_2_3, &res_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("   [0x%02x]\r\n", res_value);

    ADPT_PRINT(" RES_ins Tx_RES(4_9) Rx_RES RES_ins Tx_RES(4_10) Rx_RES RES_ins Tx_RES(4_11) Rx_RES\r\n");
    rc = adpt_otn_tx_res_src_get(dev_id, chnl_id, DIGI_RES_2_3, &oh_src);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("   [%d]", oh_src);
    rc = adpt_otn_tx_res_get(dev_id, chnl_id, DIGI_RES_4_9, &res_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("      [0x%02x]", res_value);
    rc = adpt_otn_rx_res_get(dev_id, chnl_id, DIGI_RES_4_9, &res_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("   [0x%02x]", res_value);
    rc = adpt_otn_tx_res_src_get(dev_id, chnl_id, DIGI_RES_4_9, &oh_src);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("   [%d]", oh_src);
    rc = adpt_otn_tx_res_get(dev_id, chnl_id, DIGI_RES_4_10, &res_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("      [0x%02x]", res_value);
    rc = adpt_otn_rx_res_get(dev_id, chnl_id, DIGI_RES_4_10, &res_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("    [0x%02x]", res_value);
    rc = adpt_otn_tx_res_src_get(dev_id, chnl_id, DIGI_RES_4_10, &oh_src);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("   [%d]", oh_src);
    rc = adpt_otn_tx_res_get(dev_id, chnl_id, DIGI_RES_4_11, &res_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("      [0x%02x]", res_value);
    rc = adpt_otn_rx_res_get(dev_id, chnl_id, DIGI_RES_4_11, &res_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("    [0x%02x]\r\n", res_value);

    ADPT_PRINT(" RES_ins Tx_RES(4_12) Rx_RES RES_ins Tx_RES(4_13) Rx_RES RES_ins Tx_RES(4_14) Rx_RES\r\n");
    rc = adpt_otn_tx_res_src_get(dev_id, chnl_id, DIGI_RES_4_11, &oh_src);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("   [%d]", oh_src);
    rc = adpt_otn_tx_res_get(dev_id, chnl_id, DIGI_RES_4_12, &res_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("      [0x%02x]", res_value);
    rc = adpt_otn_rx_res_get(dev_id, chnl_id, DIGI_RES_4_12, &res_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("    [0x%02x]", res_value);
    rc = adpt_otn_tx_res_src_get(dev_id, chnl_id, DIGI_RES_4_12, &oh_src);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("   [%d]", oh_src);
    rc = adpt_otn_tx_res_get(dev_id, chnl_id, DIGI_RES_4_13, &res_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("      [0x%02x]", res_value);
    rc = adpt_otn_rx_res_get(dev_id, chnl_id, DIGI_RES_4_13, &res_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("    [0x%02x]", res_value);
    rc = adpt_otn_tx_res_src_get(dev_id, chnl_id, DIGI_RES_4_13, &oh_src);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("   [%d]", oh_src);
    rc = adpt_otn_tx_res_get(dev_id, chnl_id, DIGI_RES_4_14, &res_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("      [0x%02x]", res_value);
    rc = adpt_otn_rx_res_get(dev_id, chnl_id, DIGI_RES_4_14, &res_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("    [0x%02x]\r\n", res_value);

    ADPT_PRINT(" RES_ins Tx_TCM_ACT Rx_TCM_ACT TCM_ACT_ins FTFL_ins prbs_en prbs_mode dlss null_sig\r\n");
    rc = adpt_otn_tx_res_src_get(dev_id, chnl_id, DIGI_RES_4_14, &oh_src);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("   [%d]", oh_src);
    rc = adpt_otn_tx_tcm_act_get(dev_id, chnl_id, &tcm_act_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("      [0x%02x]", tcm_act_value);
    rc = adpt_otn_rx_tcm_act_get(dev_id, chnl_id, &tcm_act_value);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("    [0x%02x]", tcm_act_value);
    rc = adpt_otn_tx_tcm_act_src_get(dev_id, chnl_id, &oh_src);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("       [%d]", oh_src);
    rc = adpt_otn_tx_ftfl_src_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, &oh_src);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("        [%d]", oh_src);
    if (adpt_handle->traffic_info[index].prbs_enable)
    {
        rc = adpt_otn_prbs_en_get(dev_id, chnl_id, &prbs_chid, &prbs_mode, &enable, &enable1);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("    [%d %d]      [%d]", enable, enable1, prbs_mode);
        rc = adpt_otn_prbs_dlss_get(dev_id, chnl_id, &enable);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("    [%d]", enable);
    }
    else
    {
        ADPT_PRINT("    [0 0]      [0]    [0]");
    }
    rc = adpt_otn_null_sig_get(dev_id, chnl_id, &enable);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("    [%d]\r\n", enable);

    ADPT_PRINT(" DEG_mode DEG_loc PM_Sk TCM1_Sk TCM2_Sk TCM3_Sk TCM4_Sk TCM5_Sk TCM6_Sk PM_So\r\n");
    rc = adpt_otn_deg_mode_get(dev_id, &odu_seg, &deg_mode);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("  [%s]", adpt_deg_mode_str(deg_mode));
    ADPT_PRINT("  [%s]", adpt_deg_mon_seg_str(odu_seg));
    for (seg_id = DIGI_OTN_SEG_PM; seg_id <= DIGI_OTN_SEG_TCM6; seg_id = (digi_otn_seg_t)(seg_id + 1))
    {
        rc = adpt_otn_tcm_pri_get(dev_id, chnl_id, seg_id, DIGI_OTN_LINE_SIDE,
                                  DIGI_OTN_SINK_POS, &tcm_pri);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT(" [%s] ", adpt_tcm_pri_str(tcm_pri));
    }
    rc = adpt_otn_tcm_pri_get(dev_id, chnl_id, DIGI_OTN_SEG_PM, DIGI_OTN_LINE_SIDE,
                              DIGI_OTN_SOURCE_POS, &tcm_pri);
    ADPT_RETURN_VERIFY(rc);
    ADPT_PRINT("[%s]\r\n", adpt_tcm_pri_str(tcm_pri));

    ADPT_PRINT(" TCM1_So TCM2_So TCM3_So TCM4_So TCM5_So TCM6_So ts_type\r\n");
    for (seg_id = DIGI_OTN_SEG_TCM1; seg_id <= DIGI_OTN_SEG_TCM6; seg_id = (digi_otn_seg_t)(seg_id + 1))
    {
        rc = adpt_otn_tcm_pri_get(dev_id, chnl_id, seg_id, DIGI_OTN_LINE_SIDE,
                                  DIGI_OTN_SOURCE_POS, &tcm_pri);
        ADPT_RETURN_VERIFY(rc);
        ADPT_PRINT("  [%s]", adpt_tcm_pri_str(tcm_pri));
    }

    rc = adpt_odu_ts_type_get(dev_id, chnl_id, &ts_type);
    ADPT_RETURN_VERIFY(rc); 
    ADPT_PRINT("  [%s]\r\n", adpt_ts_type_str(ts_type));

    ADPT_PRINT(" --------------------------------------------------------------------------------\r\n");
    ADPT_PRINT(" Tx_FTFL[");
    PMC_MEMSET((void*)&ftfl_value, 0, 20);
    rc = adpt_otn_tx_ftfl_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, (UINT8*)&ftfl_value);
    ADPT_RETURN_VERIFY(rc);
    for (i = 0; i < 19; i++)
    {
        ADPT_PRINT("%d ", ftfl_value[i]);
    }
    ADPT_PRINT("%d]\r\n", ftfl_value[19]);
    ADPT_PRINT(" Rx_FTFL[");
    PMC_MEMSET((void*)&ftfl_value, 0, 20);
    rc = adpt_otn_rx_ftfl_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, (UINT8*)&ftfl_value);
    ADPT_RETURN_VERIFY(rc);
    for (i = 0; i < 19; i++)
    {
        ADPT_PRINT("%d ", ftfl_value[i]);
    }
    ADPT_PRINT("%d]\r\n", ftfl_value[19]);

    adpt_lo_odu_status_get(dev_id, chnl_id, &lo_odu);
    if ((!lo_odu) && (NULL != adpt_handle->traffic_info[index].odu_handle))
    {
        rc = adpt_msi_len_get(dev_id, chnl_id, &msi_len);
        ADPT_RETURN_VERIFY(rc);

        ADPT_PRINT(" Tx_MSI[");
        PMC_MEMSET((void*)&msi_value, 0, 80);
        rc = adpt_otn_tx_msi_get(dev_id, chnl_id, (UINT8*)&msi_value);
        ADPT_RETURN_VERIFY(rc);
        for (i = 0; i < msi_len; i++)
        {
            ADPT_PRINT("0x%x", msi_value[i]);

            if (i != msi_len)
            {
                ADPT_PRINT(" ");
            }

            if ((0 == ((i + 1) % 16)) && ((i + 1) != msi_len))
            {
                ADPT_PRINT("\r\n        ");
            }
        }
        ADPT_PRINT("]\r\n");
        ADPT_PRINT(" Exp_MSI[");
        PMC_MEMSET((void*)&msi_value, 0, 80);
        rc = adpt_otn_exp_msi_get(dev_id, chnl_id, (UINT8*)&msi_value);
        ADPT_RETURN_VERIFY(rc);
        for (i = 0; i < msi_len; i++)
        {
            ADPT_PRINT("0x%x", msi_value[i]);

            if (i != msi_len)
            {
                ADPT_PRINT(" ");
            }

            if ((0 == ((i + 1) % 16)) && ((i + 1) != msi_len))
            {
                ADPT_PRINT("\r\n         ");
            }
        }
        ADPT_PRINT("]\r\n");
        ADPT_PRINT(" Rx_MSI[");
        PMC_MEMSET((void*)&msi_value, 0, 80);
        rc = adpt_otn_rx_msi_get(dev_id, chnl_id, (UINT8*)&msi_value);
        ADPT_RETURN_VERIFY(rc);
        for (i = 0; i < msi_len; i++)
        {
            ADPT_PRINT("0x%x", msi_value[i]);

            if (i != msi_len)
            {
                ADPT_PRINT(" ");
            }

            if ((0 == ((i + 1) % 16)) && ((i + 1) != msi_len))
            {
                ADPT_PRINT("\r\n        ");
            }
        }
        ADPT_PRINT("]\r\n");
    }

    for (location = DIGI_OTN_LINE_SIDE; location < LAST_DIGI_OTN_SIDE; location = (digi_otn_loc_t)(location + 1))
    {   
        adpt_lo_odu_status_get(dev_id, chnl_id, &lo_odu);
        if (DIGI_OTN_LINE_SIDE == location)
        {
            ADPT_PRINT(" --------------------------------------------------------------------------------\r\n");
        }
        else
        {
            if (!lo_odu || (NULL == adpt_handle->traffic_info[index].odu_handle))
            {
                /* Not stage 3B */
                continue;
            }

            ADPT_PRINT(" -------------------------------------SYS----------------------------------------\r\n");
            ADPT_PRINT(" mt_sig_so mt_sig_sk FTFL_ins dci_chnl frm_num\r\n");
            rc = adpt_otn_mt_sig_get(dev_id, 
                                     chnl_id, 
                                     DIGI_OTN_SEG_SM, 
                                     location, 
                                     DIGI_OTN_SOURCE_POS, 
                                     &mt_type);
            ADPT_RETURN_VERIFY(rc);
            ADPT_PRINT("  [%s]", adpt_mt_type_str(mt_type));
            rc = adpt_otn_mt_sig_get(dev_id, 
                                     chnl_id, 
                                     DIGI_OTN_SEG_SM, 
                                     location, 
                                     DIGI_OTN_SINK_POS, 
                                     &mt_type);
            ADPT_RETURN_VERIFY(rc);
            ADPT_PRINT("    [%s]", adpt_mt_type_str(mt_type));
            rc = adpt_otn_tx_ftfl_src_get(dev_id, chnl_id, DIGI_OTN_SYS_SIDE, &oh_src);
            ADPT_RETURN_VERIFY(rc);
            ADPT_PRINT("      [%d]", oh_src);
            rc = adpt_otn_ohp_chnl_num_get(dev_id, chnl_id, DIGI_OTN_SYS_SIDE, &dci_chnl, &framer_num);
            ADPT_PRINT("      [%d]     [%d]\r\n", dci_chnl, framer_num);
            ADPT_RETURN_VERIFY(rc);

            ADPT_PRINT(" --------------------------------------------------------------------------------\r\n");
            ADPT_PRINT(" Tx_FTFL[");
            PMC_MEMSET((void*)&ftfl_value, 0, 20);
            rc = adpt_otn_tx_ftfl_get(dev_id, chnl_id, DIGI_OTN_SYS_SIDE, (UINT8*)&ftfl_value);
            ADPT_RETURN_VERIFY(rc);
            for (i = 0; i < 19; i++)
            {
                ADPT_PRINT("%d ", ftfl_value[i]);
            }
            ADPT_PRINT("%d]\r\n", ftfl_value[19]);
            ADPT_PRINT(" Rx_FTFL[");
            PMC_MEMSET((void*)&ftfl_value, 0, 20);
            rc = adpt_otn_rx_ftfl_get(dev_id, chnl_id, DIGI_OTN_SYS_SIDE, (UINT8*)&ftfl_value);
            ADPT_RETURN_VERIFY(rc);
            for (i = 0; i < 19; i++)
            {
                ADPT_PRINT("%d ", ftfl_value[i]);
            }
            ADPT_PRINT("%d]\r\n", ftfl_value[19]);
            ADPT_PRINT(" --------------------------------------------------------------------------------\r\n");
        }

        for (seg_id = DIGI_OTN_SEG_PM; seg_id <= DIGI_OTN_SEG_PM_NIM; seg_id = (digi_otn_seg_t)(seg_id + 1))
        {
            if ((seg_id > DIGI_OTN_SEG_TCM6) && (seg_id < DIGI_OTN_SEG_PM_NIM))
            {
                continue;
            }

            tcm_index = (DIGI_OTN_SEG_PM_NIM == seg_id) ? DIGI_OTN_SEG_PM : seg_id;
            tcm_pool_id_sk = adpt_handle->traffic_info[index].tcm_pool_id[location*2+DIGI_OTN_SINK_POS][tcm_index];
            tcm_pool_id_so = adpt_handle->traffic_info[index].tcm_pool_id[location*2+DIGI_OTN_SOURCE_POS][tcm_index];
            if ((DIGI_OTN_SEG_PM != seg_id) 
                && (DIGI_TCM_ENABLE != DIGI_TCM_EN_GET(tcm_pool_id_sk))
                && (DIGI_TCM_ENABLE != DIGI_TCM_EN_GET(tcm_pool_id_so)))
            {
                continue;
            }

            ADPT_PRINT(" seg_id tim_mode tim_act ltc_act tx_stat rx_stat mt_sig_so mt_sig_sk deg_m deg_th\r\n");
            ADPT_PRINT("  %s", adpt_seg_str(seg_id));
            if ((DIGI_OTN_SEG_PM == seg_id) || (DIGI_TCM_ENABLE == DIGI_TCM_EN_GET(tcm_pool_id_sk)))
            {
                rc = adpt_otn_tim_det_mode_get(dev_id, 
                                               chnl_id, 
                                               seg_id, 
                                               location, 
                                               &tim_mode);
                ADPT_RETURN_VERIFY(rc);
            }
            else
            {
                tim_mode = LAST_DIGI_TIM_MODE;
            }
            ADPT_PRINT("   [%s]", adpt_tim_mode_str(tim_mode));
            if ((DIGI_OTN_SEG_PM == seg_id) || (DIGI_TCM_ENABLE == DIGI_TCM_EN_GET(tcm_pool_id_sk)))
            {
                rc = adpt_otn_tim_act_dis_get(dev_id, 
                                              chnl_id, 
                                              seg_id, 
                                              location, 
                                              &enable);
                ADPT_RETURN_VERIFY(rc);
            }
            else
            {
                enable = FALSE;
            }
            ADPT_PRINT("    [%d]", !enable);
            if ((DIGI_OTN_SEG_PM != seg_id) && (DIGI_TCM_ENABLE == DIGI_TCM_EN_GET(tcm_pool_id_sk)))
            {
                rc = adpt_otn_ltc_act_get(dev_id, 
                                          chnl_id, 
                                          seg_id, 
                                          location, 
                                          &enable);
            }
            else
            {
                enable = FALSE;
            }
            ADPT_RETURN_VERIFY(rc);
            ADPT_PRINT("     [%d]", enable);
            if (((DIGI_OTN_SEG_PM == seg_id) && !lo_odu)
                || (DIGI_TCM_ENABLE == DIGI_TCM_EN_GET(tcm_pool_id_so)))
            {
                rc = adpt_otn_tx_stat_get(dev_id, 
                                          chnl_id, 
                                          seg_id, 
                                          location, 
                                          &stat_value);
                ADPT_RETURN_VERIFY(rc);
            }
            else
            {
                stat_value = 0;
            }
            ADPT_PRINT("     [%d]", stat_value);
            if ((DIGI_OTN_SEG_PM == seg_id)  || ((DIGI_OTN_SEG_PM_NIM != seg_id) 
                && (DIGI_TCM_ENABLE == DIGI_TCM_EN_GET(tcm_pool_id_sk))))
            {
                rc = adpt_otn_rx_stat_get(dev_id, 
                                          chnl_id, 
                                          seg_id, 
                                          location, 
                                          &stat_value);
                ADPT_RETURN_VERIFY(rc);
            }
            else
            {
                stat_value = 0;
            }
            ADPT_PRINT("     [%d]", stat_value);
            if ((DIGI_OTN_SEG_PM == seg_id) || ((DIGI_OTN_SEG_PM_NIM != seg_id)
                 && (DIGI_TCM_ENABLE == DIGI_TCM_EN_GET(tcm_pool_id_so))))
            {
                rc = adpt_otn_mt_sig_get(dev_id, 
                                         chnl_id, 
                                         seg_id, 
                                         location, 
                                         DIGI_OTN_SOURCE_POS, 
                                         &mt_type);
            } 
            else
            {
                mt_type = LAST_DIGI_MT;
            }
            ADPT_RETURN_VERIFY(rc);
            ADPT_PRINT("    [%s]", adpt_mt_type_str(mt_type));
            if ((DIGI_OTN_SEG_PM != seg_id) && (DIGI_OTN_SEG_PM_NIM != seg_id)
                && (DIGI_TCM_ENABLE == DIGI_TCM_EN_GET(tcm_pool_id_sk)))
            {
                rc = adpt_otn_mt_sig_get(dev_id, 
                                         chnl_id, 
                                         seg_id, 
                                         location, 
                                         DIGI_OTN_SINK_POS, 
                                         &mt_type);
            } 
            else
            {
                mt_type = LAST_DIGI_MT;
            }
            ADPT_RETURN_VERIFY(rc);
            ADPT_PRINT("    [%s]", adpt_mt_type_str(mt_type));
            if (((DIGI_OTN_SEG_PM == seg_id) && (DIGI_OTN_LINE_SIDE == location))
                 || ((DIGI_OTN_SEG_PM == seg_id) && (DIGI_OTN_SYS_SIDE == location) 
                 && (DIGI_OTN_HARDWARE_DDEG == deg_mode))
                 || (DIGI_TCM_ENABLE == DIGI_TCM_EN_GET(tcm_pool_id_sk)))
            {
                rc = adpt_otn_deg_para_get(dev_id, 
                                           chnl_id, 
                                           seg_id, 
                                           location, 
                                           &deg_th, 
                                           &deg_m);
                ADPT_RETURN_VERIFY(rc);
            }
            else
            {
                deg_th = 0;
                deg_m  = 0;
            }
            ADPT_PRINT("    [%d]   [%d]\r\n", deg_m, deg_th);

            ADPT_PRINT(" tcm_mode_so tcm_mode_sk ais_act enh_inv enh_deg_th tx_aps_pcc   rx_aps_pcc\r\n");
            if (DIGI_OTN_SEG_PM != seg_id)
            {
                if (DIGI_TCM_ENABLE == DIGI_TCM_EN_GET(tcm_pool_id_so))
                {
                    rc = adpt_otn_tcm_mode_get(dev_id, 
                                               chnl_id, 
                                               seg_id, 
                                               location, 
                                               DIGI_OTN_SOURCE_POS, 
                                               &tcm_mode);
                }
                else
                {
                    tcm_mode = DIGI_TCM_TRANSPARENT;
                }
            } 
            else
            {
                tcm_mode = LAST_DIGI_TCM;
            }
            ADPT_RETURN_VERIFY(rc);
            ADPT_PRINT("    [%s]", adpt_tcm_mode_str(tcm_mode));
            if (DIGI_OTN_SEG_PM != seg_id)
            {
                if (DIGI_TCM_ENABLE == DIGI_TCM_EN_GET(tcm_pool_id_sk))
                {
                    rc = adpt_otn_tcm_mode_get(dev_id, 
                                               chnl_id, 
                                               seg_id, 
                                               location, 
                                               DIGI_OTN_SINK_POS, 
                                               &tcm_mode);
                }
                else
                {
                    tcm_mode = DIGI_TCM_TRANSPARENT;
                }
            } 
            else
            {
                tcm_mode = LAST_DIGI_TCM;
            }
            ADPT_RETURN_VERIFY(rc);
            ADPT_PRINT("       [%s]", adpt_tcm_mode_str(tcm_mode));
            if ((DIGI_OTN_SEG_PM != seg_id) && (DIGI_TCM_ENABLE == DIGI_TCM_EN_GET(tcm_pool_id_sk)))
            {
                rc = adpt_otn_ais_act_get(dev_id, 
                                          chnl_id, 
                                          seg_id, 
                                          location, 
                                          &enable);
            } 
            else
            {
                enable = FALSE;
            }
            ADPT_RETURN_VERIFY(rc);
            ADPT_PRINT("      [%d]", enable);
            rc = adpt_otn_deg_mode_get(dev_id, &odu_seg, &deg_mode);
            ADPT_RETURN_VERIFY(rc);
            if ((DIGI_OTN_ENHANCED_DDEG == deg_mode) && (DIGI_OTN_LINE_SIDE == location))
            {
                rc = adpt_otn_enhanced_deg_get(dev_id, 
                                               chnl_id, 
                                               seg_id,
                                               &deg_th,
                                               &interval);
                ADPT_RETURN_VERIFY(rc);
            }
            else
            {
                deg_th       = 0;
                interval     = LAST_DIGI_DEG;
            }
            ADPT_PRINT("    [%s]", adpt_deg_interval_str(interval));
            ADPT_PRINT("     [%d]", deg_th);
            if (DIGI_OTN_SEG_PM_NIM != seg_id)
            {
                rc = adpt_otn_tx_aps_get(dev_id, chnl_id, seg_id, &aps_value);
            }
            else
            {
                aps_value = 0;
            }
            ADPT_PRINT(" [0x%08x]", aps_value);
            ADPT_RETURN_VERIFY(rc);
            if (DIGI_OTN_SEG_PM_NIM != seg_id)
            {
                rc = adpt_otn_rx_aps_get(dev_id, chnl_id, seg_id, &aps_value);
            }
            else
            {
                aps_value = 0;
            }
            ADPT_PRINT(" [0x%08x]\r\n", aps_value);
            ADPT_RETURN_VERIFY(rc);

            ADPT_PRINT(" aps_ins tx_bdi bw_act dm_mode tsd_act tsf_act\r\n");
            if (DIGI_OTN_SEG_PM_NIM != seg_id)
            {
                rc = adpt_otn_tx_aps_src_get(dev_id, chnl_id, seg_id, &oh_src);
            }
            else
            {
                oh_src = 0;
            }
            ADPT_PRINT("   [%d]", oh_src);
            ADPT_RETURN_VERIFY(rc);
            if (DIGI_TCM_ENABLE == DIGI_TCM_EN_GET(tcm_pool_id_so))
            {
                rc = adpt_otn_tx_bdi_get(dev_id, chnl_id, seg_id, location, &enable);
            }
            else
            {
                enable = FALSE;
            }
            ADPT_PRINT("    [%d]", enable);
            ADPT_RETURN_VERIFY(rc);
            if (DIGI_TCM_ENABLE == DIGI_TCM_EN_GET(tcm_pool_id_sk))
            {
                rc = adpt_otn_tcm_bw_act_get(dev_id, chnl_id, seg_id, location, &enable);
            }
            else
            {
                enable = FALSE;
            }
            ADPT_PRINT("     [%d]", enable);
            ADPT_RETURN_VERIFY(rc);
            if (((DIGI_OTN_SEG_PM == seg_id) && !lo_odu) || (DIGI_TCM_ENABLE == DIGI_TCM_EN_GET(tcm_pool_id_so)))
            {
                rc = adpt_otn_dm_mode_get(dev_id, chnl_id, seg_id, location, &dm_mode);
            }
            else
            {
                dm_mode = LAST_DIGI_DM;
            }
            ADPT_PRINT("   [%s]", adpt_dm_mode_str(dm_mode));
            ADPT_RETURN_VERIFY(rc);
            if ((DIGI_OTN_SEG_PM == seg_id) || (DIGI_TCM_ENABLE == DIGI_TCM_EN_GET(tcm_pool_id_sk)))
            {
                rc = adpt_otn_tsd_tsf_act_get(dev_id, chnl_id, seg_id, location, &enable, &enable1);
            }
            else
            {
                enable  = FALSE;
                enable1 = FALSE;
            }
            ADPT_PRINT("   [%d]     [%d]\r\n", enable, enable1);
            ADPT_RETURN_VERIFY(rc);
        }    
    }

    ADPT_PRINT("=================================================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_otn_cfg */


/*******************************************************************************
* adpt_dump_otn_tcm
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the OTN TCM configuration information from SDK. 
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
PUBLIC PMC_ERROR adpt_dump_otn_tcm(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    odu_struct_odu_frm_layer_struct_t sk_tcm_layer[ODU_SINK_MAX_PM_TCM_LAYERS];
    odu_struct_odu_frm_layer_struct_t so_tcm_layer[ODU_SOURCE_MAX_PM_TCM_LAYERS];
    UINT32 num_layer = 0;
    UINT32 tcm_index = 0;
    pmc_log_sev_t log_sev;
    digi_otn_seg_t seg_id = DIGI_OTN_SEG_PM;
    digi_tcm_mode_t tcm_mode;
    digi_otn_loc_t location = DIGI_OTN_LINE_SIDE;
    BOOL lo_odu = FALSE;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    ADPT_PRINT("\r\n===============================================================\r\n");

    for (location = DIGI_OTN_LINE_SIDE; location < LAST_DIGI_OTN_SIDE; location = (digi_otn_loc_t)(location + 1))
    {   
        if (DIGI_OTN_LINE_SIDE == location)
        {
            if (NULL != adpt_handle->traffic_info[index].odu_handle)
            {
                ADPT_PRINT(" ----------------------------LINE------------------------------\r\n");
            }
            else
            {
                ADPT_PRINT(" --------------------------------------------------------------\r\n");
            }
        }
        else
        {
            adpt_lo_odu_status_get(dev_id, chnl_id, &lo_odu);
            if (!lo_odu || (NULL == adpt_handle->traffic_info[index].odu_handle))
            {
                /* Not stage 3B */
                continue;
            }

            ADPT_PRINT(" ----------------------------SYS-------------------------------\r\n");
        }

        /* handle process */
        rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
        ADPT_RETURN_VERIFY(rc);

        PMC_MEMSET((void*)&sk_tcm_layer, 0, sizeof(sk_tcm_layer));
        rc = digi_otn_odu_rx_pm_tcmi_layer_get(adpt_handle->digi_handle,
                                               odu_handle,
                                               &sk_tcm_layer,
                                               &num_layer);
        ADPT_PRINT(" Sk_index pool_id tcm_num tcm_mode\r\n");
        for (tcm_index = 0; tcm_index < num_layer; tcm_index++)
        {
            ADPT_PRINT("   [%d]", tcm_index);
            ADPT_PRINT("    [0x%2x]", sk_tcm_layer[tcm_index].pool_id);
            ADPT_PRINT("   [0x%x]", sk_tcm_layer[tcm_index].pm_tcm_num);
            ADPT_PRINT("   [%s]\r\n", adpt_tcm_mode_str((digi_tcm_mode_t)sk_tcm_layer[tcm_index].tcm_mode));
        }

        PMC_MEMSET((void*)&so_tcm_layer, 0, sizeof(so_tcm_layer));
        rc = digi_otn_odu_tx_pm_tcmi_layer_get(adpt_handle->digi_handle,
                                               odu_handle,
                                               &so_tcm_layer,
                                               &num_layer);
        ADPT_PRINT(" So_index pool_id tcm_num tcm_mode\r\n");
        for (tcm_index = 0; tcm_index < num_layer; tcm_index++)
        {
            ADPT_PRINT("   [%d]", tcm_index);
            ADPT_PRINT("    [0x%2x]", so_tcm_layer[tcm_index].pool_id);
            ADPT_PRINT("   [0x%x]", so_tcm_layer[tcm_index].pm_tcm_num);
            ADPT_PRINT("   [%s]\r\n", adpt_tcm_mode_str((digi_tcm_mode_t)so_tcm_layer[tcm_index].tcm_mode));
        }

        ADPT_PRINT(" --------------------------------------------------------------\r\n");
        ADPT_PRINT(" seg_id tcm_mode_so tcm_mode_sk \r\n");
        for (seg_id = DIGI_OTN_SEG_PM; seg_id <= DIGI_OTN_SEG_PM_NIM; seg_id = (digi_otn_seg_t)(seg_id + 1))
        {
            if ((seg_id > DIGI_OTN_SEG_TCM6) && (seg_id < DIGI_OTN_SEG_PM_NIM))
            {
                continue;
            }

            result = adpt_otn_tcm_mode_get(dev_id, 
                                           chnl_id, 
                                           seg_id, 
                                           location, 
                                           DIGI_OTN_SOURCE_POS, 
                                           &tcm_mode);
            if (PMC_SUCCESS != result)
            {
                tcm_mode = DIGI_TCM_TRANSPARENT;
            }
            ADPT_PRINT(" [%s]     [%s] ", adpt_seg_str(seg_id), adpt_tcm_mode_str(tcm_mode));
            result = adpt_otn_tcm_mode_get(dev_id, 
                                           chnl_id, 
                                           seg_id, 
                                           location, 
                                           DIGI_OTN_SINK_POS, 
                                           &tcm_mode);
            if (PMC_SUCCESS != result)
            {
                tcm_mode = DIGI_TCM_TRANSPARENT;
            }
            ADPT_PRINT("      [%s]\r\n", adpt_tcm_mode_str(tcm_mode));
        }
    }

    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_otn_tcm */


/*******************************************************************************
* adpt_dump_otn_tti
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the OTN TTI configuration information. 
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
PUBLIC PMC_ERROR adpt_dump_otn_tti(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_otn_seg_t seg_id = DIGI_OTN_SEG_PM;
    UINT8 tti_data[64];
    UINT8 i = 0;
    UINT32 tcm_index = 0;
    pmc_log_sev_t log_sev;
    UINT16 tcm_pool_id_so = 0;
    UINT16 tcm_pool_id_sk = 0;
    digi_otn_loc_t location = DIGI_OTN_LINE_SIDE;
    BOOL lo_odu = FALSE;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    ADPT_PRINT("\r\n=================================================================================\r\n");
    /* OTU TTI */
    if (NULL != adpt_handle->traffic_info[index].otu_handle)
    {
        ADPT_PRINT("Tx TTI[%s]: ", adpt_seg_str(DIGI_OTN_SEG_SM));
        PMC_MEMSET((void*)&tti_data, 0, 64);
        rc |= adpt_otn_tx_tti_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE, &tti_data[0]);
        ADPT_RETURN_VERIFY(rc);
        for (i = 0; i < 64; i++)
        {
            ADPT_PRINT("%d ", tti_data[i]);
            if ((19 == i) || (41 == i))
            {
                ADPT_PRINT("\r\n");
            }
        }

        ADPT_PRINT("\r\nRx TTI[%s]: ", adpt_seg_str(DIGI_OTN_SEG_SM));
        PMC_MEMSET((void*)&tti_data, 0, 64);
        rc |= adpt_otn_rx_tti_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE, &tti_data[0]);
        ADPT_RETURN_VERIFY(rc);
        for (i = 0; i < 64; i++)
        {
            ADPT_PRINT("%d ", tti_data[i]);
            if ((19 == i) || (41 == i))
            {
                ADPT_PRINT("\r\n");
            }
        }

        ADPT_PRINT("\r\nExp TTI[%s]: ", adpt_seg_str(DIGI_OTN_SEG_SM));
        PMC_MEMSET((void*)&tti_data, 0, 64);
        rc |= adpt_otn_exp_tti_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                   DIGI_OTN_LINE_SIDE, &tti_data[0]);
        ADPT_RETURN_VERIFY(rc);
        for (i = 0; i < 64; i++)
        {
            ADPT_PRINT("%d ", tti_data[i]);
            if ((19 == i) || (41 == i))
            {
                ADPT_PRINT("\r\n");
            }
        }
        ADPT_PRINT("\r\n");
    }

    /* ODU TTI */
    for (location = DIGI_OTN_LINE_SIDE; location < LAST_DIGI_OTN_SIDE; location = (digi_otn_loc_t)(location + 1))
    {   
        if (DIGI_OTN_LINE_SIDE == location)
        {
            if (NULL != adpt_handle->traffic_info[index].odu_handle)
            {
                ADPT_PRINT(" -------------------------------------LINE---------------------------------------\r\n");
            }
            else
            {
                ADPT_PRINT(" --------------------------------------------------------------------------------\r\n");
            }
        }
        else
        {
            adpt_lo_odu_status_get(dev_id, chnl_id, &lo_odu);
            if (!lo_odu || (NULL == adpt_handle->traffic_info[index].odu_handle))
            {
                /* Not stage 3B */
                continue;
            }

            ADPT_PRINT(" -------------------------------------SYS----------------------------------------\r\n");
        }

        for (seg_id = DIGI_OTN_SEG_PM; seg_id <= DIGI_OTN_SEG_PM_NIM; seg_id = (digi_otn_seg_t)(seg_id + 1))
        {
            if ((seg_id > DIGI_OTN_SEG_TCM6) && (seg_id < DIGI_OTN_SEG_PM_NIM))
            {
                continue;
            }

            tcm_index = (DIGI_OTN_SEG_PM_NIM == seg_id) ? DIGI_OTN_SEG_PM : seg_id;
            tcm_pool_id_sk = adpt_handle->traffic_info[index].tcm_pool_id[location*2+DIGI_OTN_SINK_POS][tcm_index];
            tcm_pool_id_so = adpt_handle->traffic_info[index].tcm_pool_id[location*2+DIGI_OTN_SOURCE_POS][tcm_index];
            if ((DIGI_OTN_SEG_PM != seg_id) 
                && (DIGI_TCM_ENABLE != DIGI_TCM_EN_GET(tcm_pool_id_sk))
                && (DIGI_TCM_ENABLE != DIGI_TCM_EN_GET(tcm_pool_id_so)))
            {
                continue;
            }

            if (((DIGI_OTN_SEG_PM != seg_id) && (DIGI_TCM_ENABLE == DIGI_TCM_EN_GET(tcm_pool_id_so)))
                 || (DIGI_OTN_SEG_PM == seg_id))
            {
                ADPT_PRINT("Tx TTI[%s]: ", adpt_seg_str(seg_id));
                PMC_MEMSET((void*)&tti_data, 0, 64);
                rc |= adpt_otn_tx_tti_get(dev_id, chnl_id, seg_id, 
                                          location, &tti_data[0]);
                ADPT_RETURN_VERIFY(rc);
                for (i = 0; i < 64; i++)
                {
                    ADPT_PRINT("%d ", tti_data[i]);
                    if ((19 == i) || (41 == i))
                    {
                        ADPT_PRINT("\r\n");
                    }
                }
            }

            if (((DIGI_OTN_SEG_PM != seg_id) && (DIGI_TCM_ENABLE == DIGI_TCM_EN_GET(tcm_pool_id_sk)))
                 || (DIGI_OTN_SEG_PM == seg_id))
            {
                ADPT_PRINT("\r\nRx TTI[%s]: ", adpt_seg_str(seg_id));
                PMC_MEMSET((void*)&tti_data, 0, 64);
                rc |= adpt_otn_rx_tti_get(dev_id, chnl_id, seg_id, 
                                          location, &tti_data[0]);
                ADPT_RETURN_VERIFY(rc);
                for (i = 0; i < 64; i++)
                {
                    ADPT_PRINT("%d ", tti_data[i]);
                    if ((19 == i) || (41 == i))
                    {
                        ADPT_PRINT("\r\n");
                    }
                }

                ADPT_PRINT("\r\nExp TTI[%s]: ", adpt_seg_str(seg_id));
                PMC_MEMSET((void*)&tti_data, 0, 64);
                rc |= adpt_otn_exp_tti_get(dev_id, chnl_id, seg_id, 
                                           location, &tti_data[0]);
                ADPT_RETURN_VERIFY(rc);
                for (i = 0; i < 64; i++)
                {
                    ADPT_PRINT("%d ", tti_data[i]);
                    if ((19 == i) || (41 == i))
                    {
                        ADPT_PRINT("\r\n");
                    }
                }
            }
            ADPT_PRINT("\r\n");
        }
    }

    ADPT_PRINT("=================================================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_otn_tti */


/*******************************************************************************
* adpt_dump_otn_alm
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the OTN alarm information. 
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
PUBLIC PMC_ERROR adpt_dump_otn_alm(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_otn_seg_t seg_id = DIGI_OTN_SEG_PM;
    digi_otn_defect_t defect_id = DIGI_OTN_LOS; 
    BOOL defect = FALSE;
    BOOL root_node = FALSE;
    UINT32 lane_id = 0;
    UINT32 lane_num = 0;
    UINT32 client_mode = 0;
    BOOL lo_odu = FALSE;
    digi_otn_loc_t location = DIGI_OTN_LINE_SIDE;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* defect trigger */
    rc = adpt_defect_trigger(dev_id, DIGI_BLK_FULL);
    ADPT_RETURN_VERIFY(rc);

    /* get client mode */
    client_mode = DIGI_CLIENT_MODE_GET(adpt_handle->traffic_info[index].traffic_mode);

    /* check root node for chnl_id */
    if (NULL != adpt_handle->traffic_info[index].otu_handle)
    {
        root_node = TRUE;
    }

    ADPT_PRINT("\r\n=========================================================================\r\n");
    if (root_node)
    {
        /* OTU defect list */
        if ((DIGI_OTN_SERVER_OTU2F != client_mode)
             && (DIGI_OTN_SERVER_OTU1F != client_mode)
             && (DIGI_OTN_SERVER_OTU2E != client_mode)
             && (DIGI_OTN_SERVER_OTU1E != client_mode)
             && (DIGI_OTN_SERVER_OTU2 != client_mode))
        {
            ADPT_PRINT(" dLOS dLOL dLOF dOOF dLOM dOOM dIAE dBIAE dTIM dBDI dDEG dEDEG aTSF aTSD CI_SSF\r\n");
        }
        else
        {
            ADPT_PRINT(" dLOS dLOL dLOF dOOF dLOM dOOM dIAE dBIAE dTIM dBDI dDEG dEDEG aTSF aTSD CI_SSF dOTUAIS\r\n");
        }
        rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE, DIGI_OTN_LOS, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE, DIGI_OTN_LOL, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE, DIGI_OTN_LOF, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE, DIGI_OTN_OOF, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE, DIGI_OTN_LOM, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE, DIGI_OTN_OOM, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE, DIGI_OTN_IAE, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE, DIGI_OTN_BIAE, &defect);
        ADPT_PRINT("  [%d] ", defect);
        rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE, DIGI_OTN_TIM, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE, DIGI_OTN_BDI, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE, DIGI_OTN_DEG, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE, DIGI_OTN_EDEG, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE, DIGI_OTN_TSF, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE, DIGI_OTN_TSD, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE, DIGI_OTN_SSF, &defect);
        ADPT_PRINT("   [%d]", defect);
        if ((DIGI_OTN_SERVER_OTU2F != client_mode)
             && (DIGI_OTN_SERVER_OTU1F != client_mode)
             && (DIGI_OTN_SERVER_OTU2E != client_mode)
             && (DIGI_OTN_SERVER_OTU1E != client_mode)
             && (DIGI_OTN_SERVER_OTU2 != client_mode))
        {
            ADPT_PRINT("\r\n");
        }
        else
        {
            rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                      DIGI_OTN_LINE_SIDE, DIGI_OTN_OTUAIS, &defect);
            ADPT_PRINT("    [%d]\r\n", defect);
        }
        
        if ((DIGI_OTN_SERVER_OTU2F != client_mode)
             && (DIGI_OTN_SERVER_OTU1F != client_mode)
             && (DIGI_OTN_SERVER_OTU2E != client_mode)
             && (DIGI_OTN_SERVER_OTU1E != client_mode)
             && (DIGI_OTN_SERVER_OTU2 != client_mode))
        {
            if (DIGI_OTN_SERVER_OTU4 == client_mode)
            {
                lane_num = 20;
            }
            else
            {
                lane_num = 4;
            }

            /* dLOS */
            ADPT_PRINT(" dLOS [");
            for (lane_id = 0; lane_id < lane_num; lane_id++)
            {
                rc |= adpt_otn_mld_get(dev_id, chnl_id, lane_id, DIGI_OTN_LOS, &defect);
                ADPT_PRINT(" %d", defect);
            }
            ADPT_PRINT(" ]\r\n");
            /* dOOF */
            ADPT_PRINT(" dOOF [");
            for (lane_id = 0; lane_id < lane_num; lane_id++)
            {
                rc |= adpt_otn_mld_get(dev_id, chnl_id, lane_id, DIGI_OTN_OOF, &defect);
                ADPT_PRINT(" %d", defect);
            }
            ADPT_PRINT(" ]\r\n");
            /* dOTUAIS */
            ADPT_PRINT(" dOTUAIS [");
            for (lane_id = 0; lane_id < lane_num; lane_id++)
            {
                rc |= adpt_otn_mld_get(dev_id, chnl_id, lane_id, DIGI_OTN_OTUAIS, &defect);
                ADPT_PRINT(" %d", defect);
            }
            ADPT_PRINT(" ]\r\n");
            /* dLOFLANE */
            ADPT_PRINT(" dLOFLANE [");
            for (lane_id = 0; lane_id < lane_num; lane_id++)
            {
                rc |= adpt_otn_mld_get(dev_id, chnl_id, lane_id, DIGI_OTN_LOFLANE, &defect);
                ADPT_PRINT(" %d", defect);
            }
            ADPT_PRINT(" ]\r\n");
            /* dOOR */
            ADPT_PRINT(" dOOR [");
            for (lane_id = 0; lane_id < lane_num; lane_id++)
            {
                rc |= adpt_otn_mld_get(dev_id, chnl_id, lane_id, DIGI_OTN_OOR, &defect);
                ADPT_PRINT(" %d", defect);
            }
            ADPT_PRINT(" ]\r\n");
            /* dLOR */
            ADPT_PRINT(" dLOR [");
            for (lane_id = 0; lane_id < lane_num; lane_id++)
            {
                rc |= adpt_otn_mld_get(dev_id, chnl_id, lane_id, DIGI_OTN_LOR, &defect);
                ADPT_PRINT(" %d", defect);
            }
            ADPT_PRINT(" ]\r\n");
            /* dOOM */
            ADPT_PRINT(" dOOM [");
            for (lane_id = 0; lane_id < lane_num; lane_id++)
            {
                rc |= adpt_otn_mld_get(dev_id, chnl_id, lane_id, DIGI_OTN_OOM, &defect);
                ADPT_PRINT(" %d", defect);
            }
            ADPT_PRINT(" ]\r\n");
        }

        ADPT_PRINT(" dPLM dMSIM dCSF dLOOMFI \r\n");
    }
    else
    {
        /* check if the Rx is provisioned or not. */
        if ((NULL != adpt_handle->traffic_info[index].odu_handle)
            && !(DIGI_OTN_DIR_RX & adpt_handle->traffic_info[index].otn_dir))
        {
            ADPT_PRINT(" This ODU isn't provisioned in the Rx direction. \r\n");
            ADPT_PRINT(" ------------------------------------------------------------------------\r\n");
        }

        ADPT_PRINT(" dLOFLOM dOOF dPLM dMSIM dCSF dLOOMFI\r\n");
        rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE, DIGI_OTN_LOFLOM, &defect); 
        ADPT_PRINT("   [%d] ", defect);
        rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE, DIGI_OTN_OOF, &defect); 
        ADPT_PRINT("  [%d]", defect);
    }
    rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_PM, 
                              DIGI_OTN_LINE_SIDE, DIGI_OTN_PLM, &defect); 
    ADPT_PRINT("  [%d]", defect);
    rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_PM, 
                              DIGI_OTN_LINE_SIDE, DIGI_OTN_MSIM, &defect); 
    ADPT_PRINT("   [%d]", defect);
    rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_PM, 
                              DIGI_OTN_LINE_SIDE, DIGI_OTN_CSF, &defect); 
    ADPT_PRINT("  [%d]", defect);
    rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_PM, 
                              DIGI_OTN_LINE_SIDE, DIGI_OTN_LOOMFI, &defect); 
    ADPT_PRINT("    [%d]\r\n", defect);

    for (location = DIGI_OTN_LINE_SIDE; location < LAST_DIGI_OTN_SIDE; location = (digi_otn_loc_t)(location + 1))
    {   
        if (DIGI_OTN_LINE_SIDE == location)
        {
            if (NULL != adpt_handle->traffic_info[index].odu_handle)
            {
                ADPT_PRINT(" ----------------------------LINE----------------------------------------\r\n");
            }
            else
            {
                ADPT_PRINT(" ------------------------------------------------------------------------\r\n");
            }
        }
        else
        {
            adpt_lo_odu_status_get(dev_id, chnl_id, &lo_odu);
            if (!lo_odu || (NULL == adpt_handle->traffic_info[index].odu_handle))
            {
                /* Not stage 3B */
                continue;
            }

            ADPT_PRINT(" ----------------------------SYS-----------------------------------------\r\n");
            /* check if the Tx is provisioned or not. */
            if ((NULL == adpt_handle->traffic_info[index].otu_handle)
                && !(DIGI_OTN_DIR_TX & adpt_handle->traffic_info[index].otn_dir))
            {
                ADPT_PRINT(" This ODU isn't provisioned in the Tx direction. \r\n");
                ADPT_PRINT(" ------------------------------------------------------------------------\r\n");
            }

            ADPT_PRINT(" dLOFLOM dOOF \r\n");
            rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                      DIGI_OTN_SYS_SIDE, DIGI_OTN_LOFLOM, &defect); 
            ADPT_PRINT("   [%d] ", defect);
            rc |= adpt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                      DIGI_OTN_SYS_SIDE, DIGI_OTN_OOF, &defect); 
            ADPT_PRINT("  [%d]\r\n", defect);
        }

        ADPT_PRINT(" seg_id dIAE dBIAE dTIM dBDI dDEG dEDEG dLTC dLCK dOCI dAIS aTSF aTSD CI_SSF\r\n");
        for (seg_id = DIGI_OTN_SEG_PM; seg_id <= DIGI_OTN_SEG_PM_NIM; seg_id = (digi_otn_seg_t)(seg_id + 1))
        {
            if ((seg_id > DIGI_OTN_SEG_TCM6) && (seg_id < DIGI_OTN_SEG_PM_NIM))
            {
                continue;
            }

            ADPT_PRINT("  %s ", adpt_seg_str(seg_id));
            for (defect_id = DIGI_OTN_IAE; defect_id <= DIGI_OTN_SSF; defect_id = (digi_otn_defect_t)(defect_id + 1))
            {
                if ((DIGI_OTN_SEG_PM == seg_id) && ((DIGI_OTN_IAE == defect_id) ||
                    (DIGI_OTN_BIAE == defect_id) || (DIGI_OTN_LTC == defect_id)))
                {
                    defect = FALSE;
                }
                else
                {
                    rc |= adpt_otn_defect_get(dev_id, chnl_id, seg_id,
                                              location, defect_id, &defect);

                    if (DIGI_OTN_EDEG == defect_id)
                    {
                        ADPT_PRINT(" ");
                    }
                }
                ADPT_PRINT("  [%d]", defect);
            }
            ADPT_PRINT("\r\n");
        }
    }

    ADPT_PRINT("=========================================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_otn_alm */


/*******************************************************************************
* adpt_dump_otn_alm_i
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the OTN alarm interrupt information. 
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
PUBLIC PMC_ERROR adpt_dump_otn_alm_i(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_otn_seg_t seg_id = DIGI_OTN_SEG_PM;
    digi_otn_defect_t defect_id = DIGI_OTN_LOS; 
    BOOL defect = FALSE;
    BOOL root_node = FALSE;
    UINT32 lane_id = 0;
    UINT32 lane_num = 0;
    UINT32 client_mode = 0;
    BOOL lo_odu = FALSE;
    digi_otn_loc_t location = DIGI_OTN_LINE_SIDE;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* defect trigger */
    rc = adpt_defect_trigger(dev_id, DIGI_BLK_FULL);
    ADPT_RETURN_VERIFY(rc);

    /* get client mode */
    client_mode = DIGI_CLIENT_MODE_GET(adpt_handle->traffic_info[index].traffic_mode);

    /* check root node for chnl_id */
    if (NULL != adpt_handle->traffic_info[index].otu_handle)
    {
        root_node = TRUE;
    }

    ADPT_PRINT("\r\n=========================================================================\r\n");
    if (root_node)
    {
        /* OTU defect list */
        if ((DIGI_OTN_SERVER_OTU2F != client_mode)
             && (DIGI_OTN_SERVER_OTU1F != client_mode)
             && (DIGI_OTN_SERVER_OTU2E != client_mode)
             && (DIGI_OTN_SERVER_OTU1E != client_mode)
             && (DIGI_OTN_SERVER_OTU2 != client_mode))
        {
            ADPT_PRINT(" dLOS dLOL dLOF dOOF dLOM dOOM dIAE dBIAE dTIM dBDI dDEG dEDEG aTSF aTSD CI_SSF\r\n");
        }
        else
        {
            ADPT_PRINT(" dLOS dLOL dLOF dOOF dLOM dOOM dIAE dBIAE dTIM dBDI dDEG dEDEG aTSF aTSD CI_SSF dOTUAIS\r\n");
        }
        rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                    DIGI_OTN_LINE_SIDE, DIGI_OTN_LOS, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                    DIGI_OTN_LINE_SIDE, DIGI_OTN_LOL, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                    DIGI_OTN_LINE_SIDE, DIGI_OTN_LOF, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                    DIGI_OTN_LINE_SIDE, DIGI_OTN_OOF, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                    DIGI_OTN_LINE_SIDE, DIGI_OTN_LOM, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                    DIGI_OTN_LINE_SIDE, DIGI_OTN_OOM, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                    DIGI_OTN_LINE_SIDE, DIGI_OTN_IAE, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                    DIGI_OTN_LINE_SIDE, DIGI_OTN_BIAE, &defect);
        ADPT_PRINT("  [%d] ", defect);
        rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                    DIGI_OTN_LINE_SIDE, DIGI_OTN_TIM, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                    DIGI_OTN_LINE_SIDE, DIGI_OTN_BDI, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                    DIGI_OTN_LINE_SIDE, DIGI_OTN_DEG, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                    DIGI_OTN_LINE_SIDE, DIGI_OTN_EDEG, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                    DIGI_OTN_LINE_SIDE, DIGI_OTN_TSF, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                    DIGI_OTN_LINE_SIDE, DIGI_OTN_TSD, &defect);
        ADPT_PRINT("  [%d]", defect);
        rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                    DIGI_OTN_LINE_SIDE, DIGI_OTN_SSF, &defect);
        ADPT_PRINT("   [%d]", defect);
        if ((DIGI_OTN_SERVER_OTU2F != client_mode)
             && (DIGI_OTN_SERVER_OTU1F != client_mode)
             && (DIGI_OTN_SERVER_OTU2E != client_mode)
             && (DIGI_OTN_SERVER_OTU1E != client_mode)
             && (DIGI_OTN_SERVER_OTU2 != client_mode))
        {
            ADPT_PRINT("\r\n");
        }
        else
        {
            rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                        DIGI_OTN_LINE_SIDE, DIGI_OTN_OTUAIS, &defect);
            ADPT_PRINT("    [%d]\r\n", defect);
        }

        if ((DIGI_OTN_SERVER_OTU2F != client_mode)
             && (DIGI_OTN_SERVER_OTU1F != client_mode)
             && (DIGI_OTN_SERVER_OTU2E != client_mode)
             && (DIGI_OTN_SERVER_OTU1E != client_mode)
             && (DIGI_OTN_SERVER_OTU2 != client_mode))
        {
            if (DIGI_OTN_SERVER_OTU4 == client_mode)
            {
                lane_num = 20;
            }
            else
            {
                lane_num = 4;
            }

            /* dLOS */
            ADPT_PRINT(" dLOS [");
            for (lane_id = 0; lane_id < lane_num; lane_id++)
            {
                rc |= adpt_otn_mld_i_get(dev_id, chnl_id, lane_id, DIGI_OTN_LOS, &defect);
                ADPT_PRINT(" %d", defect);
            }
            ADPT_PRINT(" ]\r\n");
            /* dOOF */
            ADPT_PRINT(" dOOF [");
            for (lane_id = 0; lane_id < lane_num; lane_id++)
            {
                rc |= adpt_otn_mld_i_get(dev_id, chnl_id, lane_id, DIGI_OTN_OOF, &defect);
                ADPT_PRINT(" %d", defect);
            }
            ADPT_PRINT(" ]\r\n");
            /* dOTUAIS */
            ADPT_PRINT(" dOTUAIS [");
            for (lane_id = 0; lane_id < lane_num; lane_id++)
            {
                rc |= adpt_otn_mld_i_get(dev_id, chnl_id, lane_id, DIGI_OTN_OTUAIS, &defect);
                ADPT_PRINT(" %d", defect);
            }
            ADPT_PRINT(" ]\r\n");
            /* dLOFLANE */
            ADPT_PRINT(" dLOFLANE [");
            for (lane_id = 0; lane_id < lane_num; lane_id++)
            {
                rc |= adpt_otn_mld_i_get(dev_id, chnl_id, lane_id, DIGI_OTN_LOFLANE, &defect);
                ADPT_PRINT(" %d", defect);
            }
            ADPT_PRINT(" ]\r\n");
            /* dOOR */
            ADPT_PRINT(" dOOR [");
            for (lane_id = 0; lane_id < lane_num; lane_id++)
            {
                rc |= adpt_otn_mld_i_get(dev_id, chnl_id, lane_id, DIGI_OTN_OOR, &defect);
                ADPT_PRINT(" %d", defect);
            }
            ADPT_PRINT(" ]\r\n");
            /* dLOR */
            ADPT_PRINT(" dLOR [");
            for (lane_id = 0; lane_id < lane_num; lane_id++)
            {
                rc |= adpt_otn_mld_i_get(dev_id, chnl_id, lane_id, DIGI_OTN_LOR, &defect);
                ADPT_PRINT(" %d", defect);
            }
            ADPT_PRINT(" ]\r\n");
            /* dOOM */
            ADPT_PRINT(" dOOM [");
            for (lane_id = 0; lane_id < lane_num; lane_id++)
            {
                rc |= adpt_otn_mld_i_get(dev_id, chnl_id, lane_id, DIGI_OTN_OOM, &defect);
                ADPT_PRINT(" %d", defect);
            }
            ADPT_PRINT(" ]\r\n");
        }

        ADPT_PRINT(" dPLM dMSIM dCSF dLOOMFI \r\n");
    }
    else
    {
        ADPT_PRINT(" dLOFLOM dPLM dMSIM dCSF dLOOMFI\r\n");
        rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                    DIGI_OTN_LINE_SIDE, DIGI_OTN_LOFLOM, &defect); 
        ADPT_PRINT("   [%d] ", defect);
    }
    rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_PM, 
                                DIGI_OTN_LINE_SIDE, DIGI_OTN_PLM, &defect); 
    ADPT_PRINT("  [%d]", defect);
    rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_PM, 
                                DIGI_OTN_LINE_SIDE, DIGI_OTN_MSIM, &defect); 
    ADPT_PRINT("   [%d]", defect);
    rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_PM, 
                                DIGI_OTN_LINE_SIDE, DIGI_OTN_CSF, &defect); 
    ADPT_PRINT("  [%d]", defect);
    rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_PM, 
                                DIGI_OTN_LINE_SIDE, DIGI_OTN_LOOMFI, &defect); 
    ADPT_PRINT("    [%d]\r\n", defect);

    for (location = DIGI_OTN_LINE_SIDE; location < LAST_DIGI_OTN_SIDE; location = (digi_otn_loc_t)(location + 1))
    {   
        if (DIGI_OTN_LINE_SIDE == location)
        {
            if (NULL != adpt_handle->traffic_info[index].odu_handle)
            {
                ADPT_PRINT(" ----------------------------LINE----------------------------------------\r\n");
            }
            else
            {
                ADPT_PRINT(" ------------------------------------------------------------------------\r\n");
            }
        }
        else
        {
            adpt_lo_odu_status_get(dev_id, chnl_id, &lo_odu);
            if (!lo_odu || (NULL == adpt_handle->traffic_info[index].odu_handle))
            {
                /* Not stage 3B */
                continue;
            }

            ADPT_PRINT(" ----------------------------SYS-----------------------------------------\r\n");
            ADPT_PRINT(" dLOFLOM dOOF \r\n");
            rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                        DIGI_OTN_SYS_SIDE, DIGI_OTN_LOFLOM, &defect); 
            ADPT_PRINT("   [%d] ", defect);
            rc |= adpt_otn_defect_i_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                        DIGI_OTN_SYS_SIDE, DIGI_OTN_OOF, &defect); 
            ADPT_PRINT("  [%d]\r\n", defect);
        }

        ADPT_PRINT(" seg_id dIAE dBIAE dTIM dBDI dDEG dEDEG dLTC dLCK dOCI dAIS aTSF aTSD CI_SSF\r\n");
        for (seg_id = DIGI_OTN_SEG_PM; seg_id <= DIGI_OTN_SEG_PM_NIM; seg_id = (digi_otn_seg_t)(seg_id + 1))
        {
            if ((seg_id > DIGI_OTN_SEG_TCM6) && (seg_id < DIGI_OTN_SEG_PM_NIM))
            {
                continue;
            }

            ADPT_PRINT("  %s ", adpt_seg_str(seg_id));
            for (defect_id = DIGI_OTN_IAE; defect_id <= DIGI_OTN_SSF; defect_id = (digi_otn_defect_t)(defect_id + 1))
            {
                if ((DIGI_OTN_SEG_PM == seg_id) && ((DIGI_OTN_IAE == defect_id) ||
                    (DIGI_OTN_BIAE == defect_id) || (DIGI_OTN_LTC == defect_id)))
                {
                    defect = FALSE;
                }
                else
                {
                    rc |= adpt_otn_defect_i_get(dev_id, chnl_id, seg_id,
                                                location, defect_id, &defect);

                    if (DIGI_OTN_EDEG == defect_id)
                    {
                        ADPT_PRINT(" ");
                    }
                }
                ADPT_PRINT("  [%d]", defect);
            }
            ADPT_PRINT("\r\n");
        }
    }

    ADPT_PRINT("=========================================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_otn_alm_i */


/*******************************************************************************
* adpt_dump_otn_pmon
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the OTN performance monitor information. 
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
PUBLIC PMC_ERROR adpt_dump_otn_pmon(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_otn_seg_t seg_id = DIGI_OTN_SEG_PM;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 pm_count = 0;
    BOOL root_node = FALSE;
    BOOL lo_odu = FALSE;
    digi_otn_loc_t location = DIGI_OTN_LINE_SIDE;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* check root node for chnl_id */
    if (NULL != adpt_handle->traffic_info[index].otu_handle)
    {
        root_node = TRUE;
    }

    /* trigger PMON */
    rc = adpt_pmon_trigger(dev_id);
    ADPT_RETURN_VERIFY(rc);

    ADPT_PRINT("\r\n===============================================================\r\n");
    if (root_node)
    {
        /* FEC performance */
        switch (adpt_handle->traffic_info[index].fec_type)
        {
            case DIGI_OTN_SERVER_FEC_I4:
                ADPT_PRINT(" cor_zero_cnt cor_one_cnt uncor_cnt rx_frm_pulse_cnt total_cor_err\r\n");
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_I4_FEC_CORR_0S_COUNT, &pm_count);
                ADPT_PRINT(" [%9u]", pm_count);
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_I4_FEC_CORR_1S_COUNT, &pm_count);
                ADPT_PRINT(" [%9u]", pm_count);
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_I4_FEC_UNCORR_ERROR_COUNT, &pm_count);
                ADPT_PRINT(" [%9u]", pm_count);
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_I4_FEC_RX_FRM_PULSE_COUNT, &pm_count);
                ADPT_PRINT("   [%9u]", pm_count);
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_FEC_TOTAL_CORR_ERRS, &pm_count);
                ADPT_PRINT("   [%u]\r\n", pm_count);
                break;

            case DIGI_OTN_SERVER_FEC_I7:
                ADPT_PRINT(" cor_zero_cnt cor_one_cnt uncor_cnt rx_frm_pulse_cnt total_cor_err \r\n");
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_I7_FEC_CORR_0S_COUNT, &pm_count);
                ADPT_PRINT(" [%9u]", pm_count);
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_I7_FEC_CORR_1S_COUNT, &pm_count);
                ADPT_PRINT(" [%9u]", pm_count);
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_I7_FEC_UNCORR_ERROR_COUNT, &pm_count);
                ADPT_PRINT(" [%9u]", pm_count);
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_I7_FEC_RX_FRM_PULSE_COUNT, &pm_count);
                ADPT_PRINT("   [%9u]", pm_count);
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_FEC_TOTAL_CORR_ERRS, &pm_count);
                ADPT_PRINT("   [%u]\r\n", pm_count);
                break;
                
            case DIGI_OTN_SERVER_FEC_G709:
                ADPT_PRINT(" cor_zero_cnt cor_one_cnt uncor_cwds stats_fp total_cor_err\r\n");
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_GFEC_LPA_STATS_CORR_0S, &pm_count);
                ADPT_PRINT(" [%9u]", pm_count);
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_GFEC_LPA_STATS_CORR_1S, &pm_count);
                ADPT_PRINT(" [%9u]", pm_count);
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_GFEC_LPA_STATS_UNCORR_CWDS, &pm_count);
                ADPT_PRINT(" [%9u]", pm_count);
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_GFEC_LPA_STATS_FP, &pm_count);
                ADPT_PRINT(" [%9u]", pm_count);
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_FEC_TOTAL_CORR_ERRS, &pm_count);
                ADPT_PRINT(" [%u]\r\n", pm_count);
                break;
                
            case DIGI_OTN_SERVER_FEC_SWIZZLE_100G:
            case DIGI_OTN_SERVER_FEC_SWIZZLE_40G:
                ADPT_PRINT(" lpa_cor0    lpa_cor1 lpa_uncor_cwds lpa_fp      llsa_cor0\r\n");
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_SWZ_LPA_STATS_CORR_0S, &pm_count);
                ADPT_PRINT(" [%9u]", pm_count);
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_SWZ_LPA_STATS_CORR_1S, &pm_count);
                ADPT_PRINT(" [%9u]", pm_count);
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_SWZ_LPA_STATS_UNCORR_CWDS, &pm_count);
                ADPT_PRINT(" [%9u]", pm_count);
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_SWZ_LPA_STATS_FP, &pm_count);
                ADPT_PRINT(" [%9u]", pm_count);
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_SWZ_LLSA_STATS_CORR_0S, &pm_count);
                ADPT_PRINT(" [%u]\r\n", pm_count);
                ADPT_PRINT(" llsa_cor1   llsa_ucwds  total_cor_err\r\n");
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_SWZ_LLSA_STATS_CORR_1S, &pm_count);
                ADPT_PRINT(" [%9u]", pm_count);
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_SWZ_LLSA_STATS_UNCORR_CWDS, &pm_count);
                ADPT_PRINT(" [%9u]", pm_count);
                rc |= adpt_otn_fec_pm_get(dev_id, chnl_id, DIGI_FEC_TOTAL_CORR_ERRS, &pm_count);
                ADPT_PRINT("   [%u]\r\n", pm_count);
                break;



            default:
                break;
        }
    }

    for (location = DIGI_OTN_LINE_SIDE; location < LAST_DIGI_OTN_SIDE; location = (digi_otn_loc_t)(location + 1))
    {
        if (DIGI_OTN_LINE_SIDE == location)
        {
            if (NULL != adpt_handle->traffic_info[index].odu_handle)
            {
                ADPT_PRINT(" ----------------------------LINE------------------------------\r\n");
            }

            ADPT_PRINT(" prbs_pm\r\n");
            rc |= adpt_otn_prbs_pm_get(dev_id, chnl_id, &pm_count);
            ADPT_PRINT("  [%u]\r\n", pm_count);
        }
        else
        {
            adpt_lo_odu_status_get(dev_id, chnl_id, &lo_odu);
            if (!lo_odu || (NULL == adpt_handle->traffic_info[index].odu_handle))
            {
                /* Not stage 3B */
                continue;
            }

            ADPT_PRINT(" ----------------------------SYS-------------------------------\r\n");
        }

        if ((root_node) && (DIGI_OTN_LINE_SIDE == location))
        {
            ADPT_PRINT(" seg_id nds piae fds pbiae bei_cnt  bip_cnt \r\n");
            ADPT_PRINT("  %s ", adpt_seg_str(DIGI_OTN_SEG_SM));
            rc |= adpt_otn_pm_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, DIGI_OTN_LINE_SIDE, 
                                  DIGI_OTN_NDS, &pm_count);
            ADPT_PRINT(" [%u]", pm_count);
            rc |= adpt_otn_pm_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, DIGI_OTN_LINE_SIDE, 
                                  DIGI_OTN_PIAE, &pm_count);
            ADPT_PRINT(" [%u]", pm_count);
            rc |= adpt_otn_pm_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, DIGI_OTN_LINE_SIDE,
                                  DIGI_OTN_FDS, &pm_count);
            ADPT_PRINT("  [%u]", pm_count);
            rc |= adpt_otn_pm_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, DIGI_OTN_LINE_SIDE, 
                                  DIGI_OTN_PBIAE, &pm_count);
            ADPT_PRINT("  [%u]", pm_count);
            rc |= adpt_otn_pm_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, DIGI_OTN_LINE_SIDE, 
                                  DIGI_OTN_BEI, &pm_count);
            ADPT_PRINT(" [%7u]", pm_count);
            rc |= adpt_otn_pm_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, DIGI_OTN_LINE_SIDE, 
                                  DIGI_OTN_BIP, &pm_count);
            ADPT_PRINT(" [%u]\r\n", pm_count);
        }

        ADPT_PRINT(" seg_id nds piae fds pbiae bei_cnt  bip_cnt \r\n");
        for (seg_id = DIGI_OTN_SEG_PM; seg_id <= DIGI_OTN_SEG_PM_NIM; seg_id = (digi_otn_seg_t)(seg_id + 1))
        {
            if ((seg_id > DIGI_OTN_SEG_TCM6) && (seg_id < DIGI_OTN_SEG_PM_NIM))
            {
                continue;
            }

            ADPT_PRINT("  %s ", adpt_seg_str(seg_id));
            rc |= adpt_otn_pm_get(dev_id, chnl_id, seg_id, location, DIGI_OTN_NDS, &pm_count);
            ADPT_PRINT(" [%u]", pm_count);
            if (DIGI_OTN_SEG_PM != seg_id)
            {
                rc |= adpt_otn_pm_get(dev_id, chnl_id, seg_id, location, DIGI_OTN_PIAE, &pm_count);
            }
            else
            {
                pm_count = 0;
            }
            ADPT_PRINT(" [%u]", pm_count);
            rc |= adpt_otn_pm_get(dev_id, chnl_id, seg_id, location, DIGI_OTN_FDS, &pm_count);
            ADPT_PRINT("  [%u]", pm_count);
            if (DIGI_OTN_SEG_PM != seg_id)
            {
                rc |= adpt_otn_pm_get(dev_id, chnl_id, seg_id, location, DIGI_OTN_PBIAE, &pm_count);
            }
            else
            {
                pm_count = 0;
            }
            ADPT_PRINT("  [%u]", pm_count);
            rc |= adpt_otn_pm_get(dev_id, chnl_id, seg_id, location, DIGI_OTN_BEI, &pm_count);
            ADPT_PRINT(" [%7u]", pm_count);
            rc |= adpt_otn_pm_get(dev_id, chnl_id, seg_id, location, DIGI_OTN_BIP, &pm_count);
            ADPT_PRINT(" [%u]\r\n", pm_count);
        }
    }

    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_otn_pmon */


/*******************************************************************************
* adpt_dump_otn_int
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the OTN interrupt configuration information. 
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
PUBLIC PMC_ERROR adpt_dump_otn_int(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_otn_seg_t seg_id = DIGI_OTN_SEG_PM;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL root_node = FALSE;
    UINT32 client_mode = 0;
    BOOL lo_odu = FALSE;
    digi_otn_loc_t location = DIGI_OTN_LINE_SIDE;
    UINT32 lane_id = 0;
    UINT32 lane_max = 0;
    digi_mld_int_en_t mld_int_table;
    digi_otn_int_en_t otn_int_table;
    BOOL enable = FALSE;
    odu_struct_odu_level_t odu_level = ODU_STRUCT_LEVEL_HO_ODU;
    UINT32 int_mask = 0;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* get client mode */
    client_mode = DIGI_CLIENT_MODE_GET(adpt_handle->traffic_info[index].traffic_mode);

    /* check root node for chnl_id */
    if (NULL != adpt_handle->traffic_info[index].otu_handle)
    {
        root_node = TRUE;
    }

    ADPT_PRINT("\r\n===============================================================\r\n");
    if (root_node)
    {
        if (DIGI_ODUK_ODU4 == adpt_otu_odu_cvt((digi_otn_server_otuk_t)client_mode)) 
        {
            lane_max = 20;
        }
        else
        {
            lane_max = 4;
        }

        for (lane_id = 0; lane_id < lane_max; lane_id++)
        {
            PMC_MEMSET((void*)&mld_int_table, 0, sizeof(mld_int_table));
            rc |= adpt_mld_int_en_get(dev_id, chnl_id, lane_id, &mld_int_table);
            ADPT_PRINT(" lane_id dLOS dOOF dOOR dLOR dOOM dOTUAIS dLOFLANE \r\n");
            ADPT_PRINT("   [%2d]  [%d]  [%d]  [%d]  [%d]  [%d]     [%d]    [%d]\r\n", 
                   lane_id, mld_int_table.dlos, mld_int_table.doof, 
                   mld_int_table.door, mld_int_table.dlor, mld_int_table.doom, 
                   mld_int_table.dotuais, mld_int_table.dloflane);
        }

        ADPT_PRINT(" seg_id dLOM dOOM dBIAE dIAE dTIM dDEG dEDEG dBDI dLOF dOOF\r\n");
        ADPT_PRINT("  %s ", adpt_seg_str(DIGI_OTN_SEG_SM));
        PMC_MEMSET((void*)&otn_int_table, 0, sizeof(otn_int_table));
        rc |= adpt_otn_int_en_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, 
                                  DIGI_OTN_LINE_SIDE, &otn_int_table);
        ADPT_PRINT(" [%d]  [%d]   [%d]   [%d] [%d]  [%d]   [%d]   [%d]  [%d]  [%d]\r\n", 
               otn_int_table.dlom, otn_int_table.oom_im, otn_int_table.dbiae,
               otn_int_table.diae, otn_int_table.dtim, otn_int_table.ddeg,
               otn_int_table.dedeg, otn_int_table.dbdi, otn_int_table.dlof, 
               otn_int_table.oof);
        ADPT_PRINT(" seg_id dLOL dOTUAIS aAIS aBDI\r\n");
        ADPT_PRINT("  %s ", adpt_seg_str(DIGI_OTN_SEG_SM));
        ADPT_PRINT(" [%d]     [%d]   [%d] [%d]\r\n", otn_int_table.dlol,
               otn_int_table.dotuais, otn_int_table.aais, otn_int_table.abdi);
    }

    for (location = DIGI_OTN_LINE_SIDE; location < LAST_DIGI_OTN_SIDE; location = (digi_otn_loc_t)(location + 1))
    {
        if (DIGI_OTN_LINE_SIDE == location)
        {
            if (NULL != adpt_handle->traffic_info[index].odu_handle)
            {
                ADPT_PRINT(" ----------------------------LINE------------------------------\r\n");
                ADPT_PRINT(" dPLM dMSIM dLOOMFI \r\n");
                PMC_MEMSET((void*)&otn_int_table, 0, sizeof(otn_int_table));
                rc |= adpt_otn_int_en_get(dev_id, chnl_id, DIGI_OTN_SEG_PM, 
                                          DIGI_OTN_LINE_SIDE, &otn_int_table);
                ADPT_PRINT(" [%d]   [%d]    [%d]\r\n", otn_int_table.dplm, 
                       otn_int_table.dmsim, otn_int_table.dloomfi);
            }
            else if (NULL != adpt_handle->traffic_info[index].map_handle)
            {
                ADPT_PRINT(" dPLM dCSF \r\n");
                PMC_MEMSET((void*)&otn_int_table, 0, sizeof(otn_int_table));
                rc |= adpt_otn_int_en_get(dev_id, chnl_id, DIGI_OTN_SEG_PM, 
                                          DIGI_OTN_LINE_SIDE, &otn_int_table);
                ADPT_PRINT(" [%d]  [%d]\r\n", otn_int_table.dplm, otn_int_table.dcsf);
            }            
        }
        else
        {
            adpt_lo_odu_status_get(dev_id, chnl_id, &lo_odu);
            if (!lo_odu || (NULL == adpt_handle->traffic_info[index].odu_handle))
            {
                /* Not stage 3B */
                continue;
            }

            ADPT_PRINT(" ----------------------------SYS-------------------------------\r\n");
        }

        odu_level = adpt_otn_odu_level_get(dev_id, chnl_id, location);
        rc |= adpt_otn_defect_int_en_get(dev_id, odu_level, &int_mask);
        ADPT_PRINT(" odu_level dLCK dOCI dAIS dTIM dDEG dBDI dLTC dIAE dBIAE \r\n");
        ADPT_PRINT("   %s ", adpt_odu_level_str(odu_level));
        printf("  [%d]", (int_mask & DIGI_OTN_INT_MASK_DLCK) ? TRUE : FALSE);
        printf("  [%d]", (int_mask & DIGI_OTN_INT_MASK_DOCI) ? TRUE : FALSE);
        printf("  [%d]", (int_mask & DIGI_OTN_INT_MASK_DAIS) ? TRUE : FALSE);
        printf("  [%d]", (int_mask & DIGI_OTN_INT_MASK_DTIM) ? TRUE : FALSE);
        printf("  [%d]", (int_mask & DIGI_OTN_INT_MASK_DDEG) ? TRUE : FALSE);
        printf("  [%d]", (int_mask & DIGI_OTN_INT_MASK_DBDI) ? TRUE : FALSE);
        printf("  [%d]", (int_mask & DIGI_OTN_INT_MASK_DLTC) ? TRUE : FALSE);
        printf("  [%d]", (int_mask & DIGI_OTN_INT_MASK_DIAE) ? TRUE : FALSE);
        printf("  [%d]\r\n", (int_mask & DIGI_OTN_INT_MASK_DBIAE) ? TRUE : FALSE);
        ADPT_PRINT(" odu_level dLOFLOM dOOF aTSF aTSD aAIS aBDI \r\n");
        ADPT_PRINT("   %s ", adpt_odu_level_str(odu_level));
        printf("   [%d]", (int_mask & DIGI_OTN_INT_MASK_DLOFLOM) ? TRUE : FALSE);
        printf("   [%d]", (int_mask & DIGI_OTN_INT_MASK_OOF) ? TRUE : FALSE);
        printf("  [%d]", (int_mask & DIGI_OTN_INT_MASK_ATSF) ? TRUE : FALSE);
        printf("  [%d]", (int_mask & DIGI_OTN_INT_MASK_ATSD) ? TRUE : FALSE);
        printf("  [%d]", (int_mask & DIGI_OTN_INT_MASK_AAIS) ? TRUE : FALSE);
        printf("  [%d]\r\n", (int_mask & DIGI_OTN_INT_MASK_ABDI) ? TRUE : FALSE);

        for (seg_id = DIGI_OTN_SEG_PM; seg_id <= DIGI_OTN_SEG_PM_NIM; seg_id = (digi_otn_seg_t)(seg_id + 1))
        {
            if ((seg_id > DIGI_OTN_SEG_TCM6) && (seg_id < DIGI_OTN_SEG_PM_NIM))
            {
                continue;
            }

            ADPT_PRINT(" seg_id dedeg seg_en\r\n");
            ADPT_PRINT("  %s ", adpt_seg_str(seg_id));
            PMC_MEMSET((void*)&otn_int_table, 0, sizeof(otn_int_table));
            rc |= adpt_otn_int_en_get(dev_id, chnl_id, seg_id, location, &otn_int_table);
            ADPT_PRINT("  [%d] ", otn_int_table.dedeg);
            rc |= adpt_otn_seg_int_en_get(dev_id, chnl_id, seg_id, location, &enable);
            ADPT_PRINT("   [%d] \r\n", enable);
        }
    }

    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_otn_int */


/*******************************************************************************
* adpt_dump_sdh_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the SDH/SONET configuration information. 
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
PUBLIC PMC_ERROR adpt_dump_sdh_cfg(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_port_dir_t dir = DIGI_PORT_DIR_RX;
    digi_sdh_tti_len_t tti_mode = DIGI_SDH_TTI_LEN_64_BYTE;
    digi_sdh_pm_t bn_type = DIGI_SDH_B1;
    digi_sdh_thr_t exc_thr = DIGI_SDH_DISABLED;
    digi_sdh_thr_t deg_thr = DIGI_SDH_DISABLED;
    BOOL prbs_invert = FALSE;
    BOOL prbs_enable = FALSE;
    BOOL prbs_syn = FALSE;
    UINT32 err_cnt = 0;
    UINT8 tti_data[64];
    UINT8 tti_len = 0;
    UINT32 i = 0;
    BOOL enable = FALSE;
    digi_bip_mode_t rs_bip_mode = DIGI_SDH_BIP_ERR;
    digi_bip_mode_t ms_bip_mode = DIGI_SDH_BIP_ERR;
    pmc_log_sev_t log_sev;
    UINT8 oh_byte = 0;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable)
        || (NULL == adpt_handle->traffic_info[index].cbr_handle))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    ADPT_PRINT("\r\n===========================================================================\r\n");
    ADPT_PRINT(" bn_type prbs_inv prbs_en prbs_syn err_cnt\r\n");
    rc |= adpt_sdh_bn_type_get(dev_id, &bn_type);
    ADPT_PRINT("  [%s]", adpt_bn_type_str(bn_type));
    rc |= adpt_sdh_prbs_en_get(dev_id, chnl_id, &prbs_invert, &prbs_enable);
    ADPT_PRINT("     [%d]     [%d]", prbs_invert, prbs_enable);
    rc |= adpt_sdh_prbs_status_get(dev_id, chnl_id, &prbs_syn, &err_cnt);
    ADPT_PRINT("      [%d]     [%d]\r\n", prbs_syn, err_cnt);

    ADPT_PRINT(" --------------------------------------------------------------------------\r\n");
    ADPT_PRINT(" dir tti_mode gais_ins exc_thr deg_thr rs_bip_mode ms_bip_mode rx_J0 rx_SSM\r\n");
    for (dir = DIGI_PORT_DIR_RX; dir <= DIGI_PORT_DIR_TX; dir = (digi_port_dir_t)(dir + 1))
    {
        ADPT_PRINT(" %s", (DIGI_PORT_DIR_RX == dir) ? "Rx" : "Tx");
        rc |= adpt_sdh_tti_mode_get(dev_id, chnl_id, dir, &tti_mode);
        ADPT_PRINT("    [%s]", adpt_tti_len_str(tti_mode));
        rc |= adpt_sdh_gais_inst_get(dev_id, chnl_id, dir, SDH_PMG_PN11_INSERT_LOF, &enable);
        ADPT_PRINT("   [%d]", enable);
        rc |= adpt_sdh_gais_inst_get(dev_id, chnl_id, dir, SDH_PMG_PN11_INSERT_LOS, &enable);
        ADPT_PRINT("[%d]", enable);
        rc |= adpt_sdh_bn_thr_get(dev_id, chnl_id, dir, &exc_thr, &deg_thr);
        ADPT_PRINT("  [%s]  [%s]", adpt_bn_thr_str(exc_thr), adpt_bn_thr_str(deg_thr));
        rc |= adpt_sdh_bip_mode_get(dev_id, chnl_id, dir, &rs_bip_mode, &ms_bip_mode);
        ADPT_PRINT("   [%s]   [%s]", adpt_bip_mode_str(rs_bip_mode), adpt_bip_mode_str(ms_bip_mode));
        rc |= adpt_sdh_rx_oh_get(dev_id, chnl_id, dir, SDH_PMG_RRMP_BYTE_TYPE_J0, &oh_byte);
        ADPT_PRINT("   [%d]", oh_byte);
        rc |= adpt_sdh_rx_oh_get(dev_id, chnl_id, dir, SDH_PMG_RRMP_BYTE_TYPE_SSM, &oh_byte);
        ADPT_PRINT("    [%d]\r\n", oh_byte);
    }

    ADPT_PRINT(" dir rx_K1 rx_K2 m0_en \r\n");
    for (dir = DIGI_PORT_DIR_RX; dir <= DIGI_PORT_DIR_TX; dir = (digi_port_dir_t)(dir + 1))
    {
        ADPT_PRINT(" %s", (DIGI_PORT_DIR_RX == dir) ? "Rx" : "Tx");
        rc |= adpt_sdh_rx_oh_get(dev_id, chnl_id, dir, SDH_PMG_RRMP_BYTE_TYPE_K1, &oh_byte);
        ADPT_PRINT("   [%d]", oh_byte);
        rc |= adpt_sdh_rx_oh_get(dev_id, chnl_id, dir, SDH_PMG_RRMP_BYTE_TYPE_K2, &oh_byte);
        ADPT_PRINT("   [%d]", oh_byte);
        rc |= adpt_sdh_m0_en_get(dev_id, chnl_id, dir, &enable);
        ADPT_PRINT("   [%d]\r\n", enable);
    }

    ADPT_PRINT(" --------------------------------------------------------------------------\r\n");
    for (dir = DIGI_PORT_DIR_RX; dir <= DIGI_PORT_DIR_TX; dir = (digi_port_dir_t)(dir + 1))
    {
        rc |= adpt_sdh_tti_mode_get(dev_id, chnl_id, dir, &tti_mode);
        switch (tti_mode)
        {
            case DIGI_SDH_TTI_LEN_64_BYTE:
                tti_len = 64;
                break;

            case DIGI_SDH_TTI_LEN_16_BYTE:
                tti_len = 16;
                break;

            case DIGI_SDH_TTI_LEN_1_BYTE:
                tti_len = 1;
                break;

            default:
                tti_len = 0;
                break;
        }

        if (0 != tti_len)
        {
            PMC_MEMSET((void*)&tti_data, 0, sizeof(UINT8) * 64);
            rc |= adpt_sdh_exp_tti_get(dev_id, chnl_id, dir, (UINT8*)&tti_data[0]);
            ADPT_PRINT(" Exp_tti[%s]:", (DIGI_PORT_DIR_RX == dir) ? "Rx" : "Tx");
            for (i = 0; i < tti_len; i++)
            {
                ADPT_PRINT(" %d", tti_data[i]);
                if ((((i + 1) % 16) == 0) || (1 == tti_len))
                {
                    ADPT_PRINT("\r\n");
                }
            }
            PMC_MEMSET((void*)&tti_data, 0, sizeof(UINT8) * 64);
            rc |= adpt_sdh_rx_tti_get(dev_id, chnl_id, dir, (UINT8*)&tti_data[0]);
            ADPT_PRINT(" Rx_tti[%s]:", (DIGI_PORT_DIR_RX == dir) ? "Rx" : "Tx");
            for (i = 0; i < tti_len; i++)
            {
                ADPT_PRINT(" %d", tti_data[i]);
                if ((((i + 1) % 16) == 0) || (1 == tti_len))
                {
                    ADPT_PRINT("\r\n");
                }
            }
        }
    }
    ADPT_PRINT("===========================================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_sdh_cfg */


/*******************************************************************************
* adpt_dump_sdh_alm
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the SDH/SONET alarm information. 
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
PUBLIC PMC_ERROR adpt_dump_sdh_alm(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_sdh_defect_t defect_id = DIGI_SDH_LOS;
    BOOL defect = FALSE;
    digi_port_dir_t dir = DIGI_PORT_DIR_RX;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable)
        || (NULL == adpt_handle->traffic_info[index].cbr_handle))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* defect trigger */
    rc = adpt_defect_trigger(dev_id, DIGI_BLK_FULL);
    ADPT_RETURN_VERIFY(rc);

    ADPT_PRINT("\r\n================================================================================\r\n");
    ADPT_PRINT(" dir dLOS dAIS dLOF dOOF dR_TIM dR_TIU dR_DEG dR_EXC dM_AIS dM_RDI dM_DEG dM_EXC\r\n");
    for (dir = DIGI_PORT_DIR_RX; dir <= DIGI_PORT_DIR_TX; dir = (digi_port_dir_t)(dir + 1))
    {
        ADPT_PRINT(" %s", (DIGI_PORT_DIR_RX == dir) ? "Rx" : "Tx");
        for (defect_id = DIGI_SDH_LOS; defect_id < LAST_DIGI_SDH; defect_id = (digi_sdh_defect_t)(defect_id + 1))
        {
            rc |= adpt_sdh_defect_get(dev_id, chnl_id, dir, defect_id, &defect);
            if (defect_id < DIGI_SDH_RS_TIM)
            {
                ADPT_PRINT("  [%d]", defect);
            }
            else
            {
                ADPT_PRINT("    [%d]", defect);
            }
        }
        ADPT_PRINT("\r\n");
    }
    ADPT_PRINT("================================================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_sdh_alm */


/*******************************************************************************
* adpt_dump_sdh_alm_i
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the SDH/SONET alarm interrupt information. 
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
PUBLIC PMC_ERROR adpt_dump_sdh_alm_i(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_sdh_defect_t defect_id = DIGI_SDH_LOS;
    BOOL defect = FALSE;
    digi_port_dir_t dir = DIGI_PORT_DIR_RX;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable)
        || (NULL == adpt_handle->traffic_info[index].cbr_handle))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* defect trigger */
    rc = adpt_defect_trigger(dev_id, DIGI_BLK_FULL);
    ADPT_RETURN_VERIFY(rc);

    ADPT_PRINT("\r\n================================================================================\r\n");
    ADPT_PRINT(" dir dLOS dAIS dLOF dOOF dR_TIM dR_TIU dR_DEG dR_EXC dM_AIS dM_RDI dM_DEG dM_EXC\r\n");
    for (dir = DIGI_PORT_DIR_RX; dir <= DIGI_PORT_DIR_TX; dir = (digi_port_dir_t)(dir + 1))
    {
        ADPT_PRINT(" %s", (DIGI_PORT_DIR_RX == dir) ? "Rx" : "Tx");
        for (defect_id = DIGI_SDH_LOS; defect_id < LAST_DIGI_SDH; defect_id = (digi_sdh_defect_t)(defect_id + 1))
        {
            rc |= adpt_sdh_defect_i_get(dev_id, chnl_id, dir, defect_id, &defect);
            if (defect_id < DIGI_SDH_RS_TIM)
            {
                ADPT_PRINT("  [%d]", defect);
            }
            else
            {
                ADPT_PRINT("    [%d]", defect);
            }
        }
        ADPT_PRINT("\r\n");
    }
    ADPT_PRINT("================================================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_sdh_alm_i */


/*******************************************************************************
* adpt_dump_sdh_pmon
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the SDH/SONET performance monitor information. 
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
PUBLIC PMC_ERROR adpt_dump_sdh_pmon(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 pm_count = 0;
    digi_port_dir_t dir = DIGI_PORT_DIR_RX;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable)
        || (NULL == adpt_handle->traffic_info[index].cbr_handle))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* trigger PMON */
    adpt_pmon_trigger(dev_id);

    ADPT_PRINT("\r\n===============================================================\r\n");
    ADPT_PRINT(" dir b1_count b2_count rei_count\r\n");
    for (dir = DIGI_PORT_DIR_RX; dir <= DIGI_PORT_DIR_TX; dir = (digi_port_dir_t)(dir + 1))
    {
        ADPT_PRINT(" %s", (DIGI_PORT_DIR_RX == dir) ? "Rx" : "Tx");
        rc |= adpt_sdh_pm_get(dev_id, chnl_id, dir, DIGI_SDH_B1, &pm_count);
        ADPT_PRINT("   [%u]", pm_count);
        rc |= adpt_sdh_pm_get(dev_id, chnl_id, dir, DIGI_SDH_B2, &pm_count);
        ADPT_PRINT("      [%u]", pm_count);
        rc |= adpt_sdh_pm_get(dev_id, chnl_id, dir, DIGI_SDH_REI, &pm_count);
        ADPT_PRINT("      [%u]\r\n", pm_count);
    }
    ADPT_PRINT(" --------------------------------------------------------------\r\n");
    ADPT_PRINT(" prbs_err\r\n");
    rc |= adpt_sdh_prbs_pm_get(dev_id, chnl_id, &pm_count);
    ADPT_PRINT("   [%u]\r\n", pm_count);
    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_sdh_pmon */


/*******************************************************************************
* adpt_dump_sdh_int
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the SDH/SONET interrupt configuration information. 
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
PUBLIC PMC_ERROR adpt_dump_sdh_int(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_sdh_int_en_t int_table;
    digi_port_dir_t dir = DIGI_PORT_DIR_RX;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);
    
    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable)
        || (NULL == adpt_handle->traffic_info[index].cbr_handle))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    ADPT_PRINT("\r\n===============================================================\r\n");
    ADPT_PRINT(" dLOS dGAIS dLOF dOOF dTIM dTIU dMS_AIS dMS_RDI\r\n");
    for (dir = DIGI_PORT_DIR_RX; dir <= DIGI_PORT_DIR_TX; dir = (digi_port_dir_t)(dir + 1))
    {
        ADPT_PRINT(" %s", (DIGI_PORT_DIR_RX == dir) ? "Rx" : "Tx");
        PMC_MEMSET((void*)&int_table, 0, sizeof(int_table));
        rc |= adpt_sdh_int_en_get(dev_id, chnl_id, dir, &int_table);
        ADPT_PRINT("  [%d]", int_table.dlos);
        ADPT_PRINT("  [%d]", int_table.dgais);
        ADPT_PRINT("  [%d]", int_table.dlof);
        ADPT_PRINT("  [%d]", int_table.doof);
        ADPT_PRINT("  [%d]", int_table.dtim);
        ADPT_PRINT("  [%d]", int_table.dtiu);
        ADPT_PRINT("  [%d]", int_table.dms_ais);
        ADPT_PRINT("  [%d]\r\n", int_table.dms_rdi);
    }
    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_sdh_int */


/*******************************************************************************
* adpt_dump_gfp_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the GFP configuration information. 
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
PUBLIC PMC_ERROR adpt_dump_gfp_cfg(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    pmc_log_sev_t log_sev;
    UINT8 exi_value = 0;
    UINT8 exi_value1 = 0;
    UINT8 upi_value = 0;
    UINT8 upi_value1 = 0;
    UINT8 pti_value = 0;
    UINT32 ehec_len = 0;
    BOOL ehec_enable = FALSE;
    UINT32 ehec_err_cor = 0;
    UINT32 exi_len = 0;
    digi_gfp_fcs_mode_t fcs_mode = DIGI_GFP_NO_FCS;
    digi_gfp_rx_fcs_mode_t rx_fcs_mode = DIGI_GFP_RX_NO_FCS;
    digi_gfp_rx_act_t rx_alm_act = DIGI_RX_NO_ACTION;
    digi_gfp_tx_act_t tx_alm_act1 = DIGI_NO_ACTION;
    digi_gfp_tx_act_t tx_alm_act2 = DIGI_NO_ACTION;
    BOOL enable = FALSE;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable)
        || (NULL == adpt_handle->traffic_info[index].map_handle))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    ADPT_PRINT("\r\n======================================================================\r\n");
    ADPT_PRINT(" tx_exi exp_exi1 exp_exi2 tx_upi exp_upi rx_cmf_upi tx_pti exp_pti \r\n");
    rc |= adpt_gfp_tx_exi_get(dev_id, chnl_id, &exi_value);
    ADPT_PRINT(" [0x%02x]", exi_value);
    rc |= adpt_gfp_exp_exi_get(dev_id, chnl_id, &exi_value, &exi_value1);
    ADPT_PRINT("  [0x%02x]   [0x%02x]", exi_value, exi_value1);
    rc |= adpt_gfp_tx_upi_get(dev_id, chnl_id, &upi_value);
    ADPT_PRINT("  [0x%02x]", upi_value);
    rc |= adpt_gfp_exp_upi_get(dev_id, chnl_id, &upi_value);
    ADPT_PRINT(" [0x%02x]", upi_value);
    rc |= adpt_gfp_rx_upi_get(dev_id, chnl_id, &upi_value);
    ADPT_PRINT("    [0x%02x] ", upi_value);
    rc |= adpt_gfp_tx_pti_get(dev_id, chnl_id, &pti_value);
    ADPT_PRINT("  [0x%02x]", pti_value);
    rc |= adpt_gfp_exp_pti_get(dev_id, chnl_id, &pti_value);
    ADPT_PRINT("  [0x%02x]\r\n", pti_value);

    ADPT_PRINT(" tx_ehec_en tx_ehec_len rx_ehec_en ehec_err_cor tx_fcs_mode rx_fcs_mode \r\n");
    rc |= adpt_gfp_tx_ehec_en_get(dev_id, chnl_id, &ehec_len, &ehec_enable);
    ADPT_PRINT("     [%d]", ehec_enable);
    ADPT_PRINT("        [%d]     ", ehec_len);
    rc |= adpt_gfp_rx_ehec_en_get(dev_id, chnl_id, &ehec_err_cor, 
                                  &exi_len, &ehec_enable);
    ADPT_PRINT("    [%d]         [%d]", ehec_enable, ehec_err_cor);
    rc |= adpt_gfp_tx_fcs_mode_get(dev_id, chnl_id, &fcs_mode);
    ADPT_PRINT("       [%s]", adpt_fcs_mode_str(fcs_mode));
    rc |= adpt_gfp_rx_fcs_mode_get(dev_id, chnl_id, &rx_fcs_mode);
    ADPT_PRINT("    [%s]\r\n", adpt_rx_fcs_mode_str(rx_fcs_mode));

    if (DIGI_MAP_7_3_GFP == adpt_gfp_mapping_get(adpt_handle->traffic_info[index].mapping_mode))
    {
        ADPT_PRINT(" tx_data_upi tx_os_upi exp_upi1 exp_upi2 \r\n");
        rc |= adpt_gfp_legacy_mode_get(dev_id, chnl_id, &upi_value, &upi_value1);
        ADPT_PRINT("   [0x%02x]     [0x%02x]", upi_value, upi_value1);
        rc |= adpt_gfp_exp_upi_ext_get(dev_id, chnl_id, &upi_value, &upi_value1);
        ADPT_PRINT("    [0x%02x]   [0x%02x]\r\n", upi_value, upi_value1);
    }

    ADPT_PRINT(" ---------------------------------------------------------------------\r\n");
    ADPT_PRINT(" Rx ACT: SSF_SF OPU_PLM OPU_SSF GFP_LFD GFP_UPM GFP_LOS GFP_LOCS \r\n");
    rc |= adpt_gfp_rx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_SSF_SF, &rx_alm_act, &enable);
    ADPT_PRINT("         [%d %d]", rx_alm_act, enable);
    rc |= adpt_gfp_rx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_PLM, &rx_alm_act, &enable);
    ADPT_PRINT("   [%d %d]", rx_alm_act, enable);
    rc |= adpt_gfp_rx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_OPU_CSF, &rx_alm_act, &enable);
    ADPT_PRINT("   [%d %d]", rx_alm_act, enable);
    rc |= adpt_gfp_rx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_GFP_LFD, &rx_alm_act, &enable);
    ADPT_PRINT("   [%d %d]", rx_alm_act, enable);
    rc |= adpt_gfp_rx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_GFP_UPM, &rx_alm_act, &enable);
    ADPT_PRINT("   [%d %d]", rx_alm_act, enable);
    rc |= adpt_gfp_rx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_GFP_CSF_LOS, &rx_alm_act, &enable);
    ADPT_PRINT("   [%d %d]", rx_alm_act, enable);
    rc |= adpt_gfp_rx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_GFP_CSF_LOCS, &rx_alm_act, &enable);
    ADPT_PRINT("   [%d %d]\r\n", rx_alm_act, enable);

    ADPT_PRINT(" GFP_FDI GFP_RDI GFP_EXM SDH_SFIS_LOF\r\n");
    rc |= adpt_gfp_rx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_GFP_FDI, &rx_alm_act, &enable);
    ADPT_PRINT("  [%d %d]", rx_alm_act, enable);
    rc |= adpt_gfp_rx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_GFP_RDI, &rx_alm_act, &enable);
    ADPT_PRINT("   [%d %d]", rx_alm_act, enable);
    rc |= adpt_gfp_rx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_SDH_SFIS_LOF, &rx_alm_act, &enable);
    ADPT_PRINT("   [%d %d]", rx_alm_act, enable);
    rc |= adpt_gfp_rx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_GFP_EXM, &rx_alm_act, &enable);
    ADPT_PRINT("      [%d %d]\r\n", rx_alm_act, enable);

    ADPT_PRINT(" Tx ACT: ENET_LOS ENET_LCK ENET_BER ENET_LF ENET_RF CBRC_SYNC CBRC_LOS \r\n");
    rc |= adpt_gfp_tx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_ENET_PHY_LOS, &tx_alm_act1, &tx_alm_act2, &enable);
    ADPT_PRINT("          [%d %d]", tx_alm_act1, enable);
    rc |= adpt_gfp_tx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_ENET_BLK_LCK, &tx_alm_act1, &tx_alm_act2, &enable);
    ADPT_PRINT("    [%d %d]", tx_alm_act1, enable);
    rc |= adpt_gfp_tx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_ENET_HI_BER, &tx_alm_act1, &tx_alm_act2, &enable);
    ADPT_PRINT("    [%d %d]", tx_alm_act1, enable);
    rc |= adpt_gfp_tx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_ENET_RX_LF, &tx_alm_act1, &tx_alm_act2, &enable);
    ADPT_PRINT("    [%d %d]", tx_alm_act1, enable);
    rc |= adpt_gfp_tx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_ENET_RX_RF, &tx_alm_act1, &tx_alm_act2, &enable);
    ADPT_PRINT("   [%d %d]", tx_alm_act1, enable);
    rc |= adpt_gfp_tx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_CBRC_LOS_SYNC, &tx_alm_act1, &tx_alm_act2, &enable);
    ADPT_PRINT("    [%d %d]", tx_alm_act1, enable);
    rc |= adpt_gfp_tx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_CBRC_LOS_SIG, &tx_alm_act1, &tx_alm_act2, &enable);
    ADPT_PRINT("     [%d %d]\r\n", tx_alm_act1, enable);

    ADPT_PRINT(" FC12_SYNC FC12_LOS SDH_LOS SDH_FRM EPMM_LOS EPMM_LF EPMM_RF SW_FORCE\r\n");
    rc |= adpt_gfp_tx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_FC1200_LOS_SYNC, &tx_alm_act1, &tx_alm_act2, &enable);
    ADPT_PRINT("  [%d %d]", tx_alm_act1, enable);
    rc |= adpt_gfp_tx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_FC1200_LOS_SIG, &tx_alm_act1, &tx_alm_act2, &enable);
    ADPT_PRINT("    [%d %d]", tx_alm_act1, enable);
    rc |= adpt_gfp_tx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_SDH_LOS_SIG, &tx_alm_act1, &tx_alm_act2, &enable);
    ADPT_PRINT("    [%d %d]", tx_alm_act1, enable);
    rc |= adpt_gfp_tx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_SDH_LOS_FRM, &tx_alm_act1, &tx_alm_act2, &enable);
    ADPT_PRINT("   [%d %d]", tx_alm_act1, enable);
    rc |= adpt_gfp_tx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_EPMM_LOS, &tx_alm_act1, &tx_alm_act2, &enable);
    ADPT_PRINT("    [%d %d]", tx_alm_act1, enable);
    rc |= adpt_gfp_tx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_EPMM_LF, &tx_alm_act1, &tx_alm_act2, &enable);
    ADPT_PRINT("    [%d %d]", tx_alm_act1, enable);
    rc |= adpt_gfp_tx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_EPMM_RF, &tx_alm_act1, &tx_alm_act2, &enable);
    ADPT_PRINT("   [%d %d]", tx_alm_act1, enable);
    rc |= adpt_gfp_tx_conseq_act_get(dev_id, chnl_id, DIGI_ACT_SW_FORCE, &tx_alm_act1, &tx_alm_act2, &enable);
    ADPT_PRINT("   [%d %d]\r\n", tx_alm_act1, enable);
    ADPT_PRINT("======================================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_gfp_cfg */


/*******************************************************************************
* adpt_dump_gfp_alm
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the GFP alarm information. 
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
PUBLIC PMC_ERROR adpt_dump_gfp_alm(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL defect = FALSE;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);
    
    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable)
        || (NULL == adpt_handle->traffic_info[index].map_handle))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* defect trigger */
    rc = adpt_defect_trigger(dev_id, DIGI_BLK_FULL);
    ADPT_RETURN_VERIFY(rc);

    ADPT_PRINT("\r\n===============================================================\r\n");
    ADPT_PRINT(" dLFD dEXM dUPM dCSF dIDLE\r\n");
    rc |= adpt_gfp_defect_get(dev_id, chnl_id, DIGI_GFP_LFD, &defect);
    ADPT_PRINT("  [%d]", defect);
    rc |= adpt_gfp_defect_get(dev_id, chnl_id, DIGI_GFP_EXM, &defect);
    ADPT_PRINT("  [%d]", defect);
    rc |= adpt_gfp_defect_get(dev_id, chnl_id, DIGI_GFP_UPM, &defect);
    ADPT_PRINT("  [%d]", defect);
    rc |= adpt_gfp_defect_get(dev_id, chnl_id, DIGI_GFP_CSF, &defect);
    ADPT_PRINT("  [%d]", defect);
    rc |= adpt_gfp_defect_get(dev_id, chnl_id, DIGI_GFP_IDLE, &defect);
    ADPT_PRINT("  [%d]\r\n", defect);
    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_gfp_alm */


/*******************************************************************************
* adpt_dump_gfp_alm_i
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the GFP alarm interrupt information. 
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
PUBLIC PMC_ERROR adpt_dump_gfp_alm_i(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL defect = FALSE;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable)
        || (NULL == adpt_handle->traffic_info[index].map_handle))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* defect trigger */
    rc = adpt_defect_trigger(dev_id, DIGI_BLK_FULL);
    ADPT_RETURN_VERIFY(rc);

    ADPT_PRINT("\r\n===============================================================\r\n");
    ADPT_PRINT(" dLFD dEXM dUPM dCSF dIDLE\r\n");
    rc |= adpt_gfp_defect_i_get(dev_id, chnl_id, DIGI_GFP_LFD, &defect);
    ADPT_PRINT("  [%d]", defect);
    rc |= adpt_gfp_defect_i_get(dev_id, chnl_id, DIGI_GFP_EXM, &defect);
    ADPT_PRINT("  [%d]", defect);
    rc |= adpt_gfp_defect_i_get(dev_id, chnl_id, DIGI_GFP_UPM, &defect);
    ADPT_PRINT("  [%d]", defect);
    rc |= adpt_gfp_defect_i_get(dev_id, chnl_id, DIGI_GFP_CSF, &defect);
    ADPT_PRINT("  [%d]", defect);
    rc |= adpt_gfp_defect_i_get(dev_id, chnl_id, DIGI_GFP_IDLE, &defect);
    ADPT_PRINT("  [%d]\r\n", defect);
    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_gfp_alm_i */


/*******************************************************************************
* adpt_dump_gfp_pmon
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the GFP performance monitor information. 
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
PUBLIC PMC_ERROR adpt_dump_gfp_pmon(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 pm_count = 0;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable)
        || (NULL == adpt_handle->traffic_info[index].map_handle))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* trigger PMON */
    adpt_pmon_trigger(dev_id);

    ADPT_PRINT("\r\n===============================================================\r\n");
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_BYTE_COUNT_LSB, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_BYTE_COUNT_LSB                   = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_BYTE_COUNT_MSB, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_BYTE_COUNT_MSB                   = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_CLIENT_DATA_FRM_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_CLIENT_DATA_FRM_COUNT            = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_IDLE_FRM_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_IDLE_FRM_COUNT                   = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_CTRL_FRM_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_CTRL_FRM_COUNT                   = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_CHEC_ERR_DROP_FRM_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_CHEC_ERR_DROP_FRM_COUNT          = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_CHEC_SINGLE_CORR_ERR_FRM_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_CHEC_SINGLE_CORR_ERR_FRM_COUNT   = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_UNPACK_DROP_FRM_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_UNPACK_DROP_FRM_COUNT            = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_N_FDIS_THEC, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_N_FDIS_THEC                         = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_THEC_SINGLE_CORR_ERR_FRM_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_THEC_SINGLE_CORR_ERR_FRM_COUNT   = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_FILTER_DROP_FRM_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_FILTER_DROP_FRM_COUNT            = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_N_FDIS_EHEC_EXI, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_N_FDIS_EHEC_EXI                     = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_P_FDIS, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_P_FDIS                              = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_GENERIC_FILTER_DROP_FRM_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_GENERIC_FILTER_DROP_FRM_COUNT    = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_CSF_FRM_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_CSF_FRM_COUNT                    = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_P_FCS_ERROR, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_P_FCS_ERROR                         = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_PAUSE_FRM_DROP_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_PAUSE_FRM_DROP_COUNT             = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_PAUSE_FRM_FROWARDED_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_PAUSE_FRM_FROWARDED_COUNT        = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_CPU_FIFO_OVR_FLW_DROP_FRM_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_CPU_FIFO_OVR_FLW_DROP_FRM_COUNT  = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_TX_FRM_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_TX_FRM_COUNT                        = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_UNPACKED_FRM_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_UNPACKED_FRM_COUNT               = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_MAX_FRM_LENGTH_FRM_DROP_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_MAX_FRM_LENGTH_FRM_DROP_COUNT    = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_MIN_FRM_LENGTH_FRM_DROP_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_MIN_FRM_LENGTH_FRM_DROP_COUNT    = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_0, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_0  = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_1, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_1  = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_2, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_2  = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_3, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_3  = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_4, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_4  = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_5, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_5  = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_6, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_6  = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SO_TX_0_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SO_TX_0_COUNT                          = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SO_TX_1_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SO_TX_1_COUNT                          = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SO_TX_MGMT_FRMS, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SO_TX_MGMT_FRMS                        = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SO_TX_CLIENT_DATA_FRAME_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SO_TX_CLIENT_DATA_FRAME_COUNT          = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SO_TX_PAUSE_FRM_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SO_TX_PAUSE_FRM_COUNT                  = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SO_TX_IDLE_FRM_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SO_TX_IDLE_FRM_COUNT                   = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SO_TX_ERR_FRM_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SO_TX_ERR_FRM_COUNT                    = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SO_TX_MIN_FRM_LENGTH_FRM_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SO_TX_MIN_FRM_LENGTH_FRM_COUNT         = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_SO_TX_MAX_FRM_LENGTH_FRM_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_SO_TX_MAX_FRM_LENGTH_FRM_COUNT         = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_DCPB_SLV_DPI_PKT_DROP_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_DCPB_SLV_DPI_PKT_DROP_COUNT            = [%u] \r\n", pm_count);
    adpt_gfp_pm_get(dev_id, chnl_id, DIGI_GFP_OCPB_SLV_DPI_PKT_DROP_COUNT, &pm_count);
    ADPT_PRINT(" DIGI_GFP_OCPB_SLV_DPI_PKT_DROP_COUNT            = [%u] \r\n", pm_count);
    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_gfp_pmon */


/*******************************************************************************
* adpt_dump_gfp_int
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the GFP interrupt configuration information. 
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
PUBLIC PMC_ERROR adpt_dump_gfp_int(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_gfp_int_en_t int_table;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);
    
    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable)
        || (NULL == adpt_handle->traffic_info[index].map_handle))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    ADPT_PRINT("\r\n===============================================================\r\n");
    ADPT_PRINT(" dLFD dEXM dUPM dCSF dIDLE\r\n");
    PMC_MEMSET((void*)&int_table, 0, sizeof(int_table));
    rc = adpt_gfp_int_en_get(dev_id, chnl_id, &int_table);
    ADPT_PRINT("  [%d]", int_table.dlfd);
    ADPT_PRINT("  [%d]", int_table.dexm);
    ADPT_PRINT("  [%d]", int_table.dupm);
    ADPT_PRINT("  [%d]", int_table.dcsf);
    ADPT_PRINT("  [%d]\r\n", int_table.didle);
    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_gfp_int */


/*******************************************************************************
* adpt_dump_enet_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the Ethernet configuration information. 
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
PUBLIC PMC_ERROR adpt_dump_enet_cfg(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 max_length = 0;
    UINT32 ipg_length = 0;
    BOOL enable = FALSE;
    UINT32 pause_quanta = 0;
    UINT32 pause_quanta_thr = 0;
    UINT32 fc_thresh = 0;
    digi_enet_tx_timing_mode_t mode = DIGI_ENET_TX_LOOP_TIMING_MODE;
    pmc_log_sev_t log_sev;
    digi_enet_fault_t fault_type = DIGI_ENET_FT_CLEAR;
    digi_enet_prbs_t prbs_mode = DIGI_ENET_PRBS_9;
    UINT32 err_cnt = 0;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable)
        || (NULL == adpt_handle->traffic_info[index].enet_handle))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    ADPT_PRINT("\r\n====================================================================\r\n");
    ADPT_PRINT(" mru_len ipg_len crc_fwd crc_inst tx_time rx_fc tx_fc fc_thr rx_pause \r\n");
    adpt_enet_mru_get(dev_id, chnl_id, &max_length);
    ADPT_PRINT(" [%d]", max_length);
    adpt_enet_ipg_get(dev_id, chnl_id, &ipg_length);
    ADPT_PRINT("   [%d]", ipg_length);
    adpt_enet_crc_fwd_get(dev_id, chnl_id, &enable);
    ADPT_PRINT("     [%d]", enable);
    adpt_enet_crc_inst_get(dev_id, chnl_id, &enable);
    ADPT_PRINT("    [%d]", enable);
    rc |= adpt_enet_tx_timing_mode_get(dev_id, chnl_id, &mode);
    ADPT_PRINT("    [%s]", adpt_timing_mode_str(mode));
    adpt_enet_rx_fc_get(dev_id, chnl_id, &enable);
    ADPT_PRINT("   [%d]", enable);
    adpt_enet_tx_fc_get(dev_id, chnl_id, &pause_quanta, 
                        &pause_quanta_thr, &fc_thresh, &enable);
    ADPT_PRINT("    [%d]   [%d]", fc_thresh, enable);
    adpt_enet_fc_status_get(dev_id, chnl_id, &enable);
    ADPT_PRINT("    [%d]\r\n", enable);

    ADPT_PRINT(" phy_en test_pkt fault_inst prbs_mode prbs_en prbs_syn err_cnt \r\n");
    adpt_enet_phy_en_get(dev_id, chnl_id, &enable);
    ADPT_PRINT("   [%d]", enable);
    rc |= adpt_enet_test_pkt_get(dev_id, chnl_id, &enable);
    ADPT_PRINT("     [%d]", enable);
    adpt_enet_fault_inst_get(dev_id, chnl_id, DIGI_PORT_DIR_RX, &fault_type);
    ADPT_PRINT("   [%s]", adpt_enet_fault_str(fault_type));
    adpt_enet_fault_inst_get(dev_id, chnl_id, DIGI_PORT_DIR_TX, &fault_type);
    ADPT_PRINT("[%s]", adpt_enet_fault_str(fault_type));
    adpt_enet_prbs_get(dev_id, chnl_id, &prbs_mode, &enable);
    ADPT_PRINT("   [%d]       [%d]", prbs_mode, enable);
    adpt_enet_prbs_status_get(dev_id, chnl_id, &enable, &err_cnt); 
    ADPT_PRINT("     [%d]     [%d]\r\n", enable, err_cnt);

    ADPT_PRINT(" lldp_pkt_addr \r\n");
    rc |= adpt_enet_lpd_pkt_get(dev_id, chnl_id, DIGI_PORT_DIR_RX, &err_cnt); 
    ADPT_PRINT(" [0x%x]", err_cnt);
    rc |= adpt_enet_lpd_pkt_get(dev_id, chnl_id, DIGI_PORT_DIR_TX, &err_cnt); 
    ADPT_PRINT("[0x%x]\r\n", err_cnt);

    ADPT_PRINT("====================================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_enet_cfg */


/*******************************************************************************
* adpt_dump_enet_alm
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the Ethernet alarm information. 
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
PUBLIC PMC_ERROR adpt_dump_enet_alm(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL defect = FALSE;
    digi_port_dir_t dir = DIGI_PORT_DIR_RX;
    pmc_log_sev_t log_sev;
    UINT32 lane_id = 0;
    UINT32 lane_max = 0;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* defect trigger */
    rc = adpt_defect_trigger(dev_id, DIGI_BLK_FULL);
    ADPT_RETURN_VERIFY(rc);

    ADPT_PRINT("\r\n===============================================================\r\n");
    ADPT_PRINT(" dir dLOS dLF dRF dHI_BER dBLK_LCK dLINK_DOWN dALIGN_FAIL\r\n");
    for (dir = DIGI_PORT_DIR_RX; dir <= DIGI_PORT_DIR_TX; dir = (digi_port_dir_t)(dir + 1))
    {
        ADPT_PRINT(" %s", (DIGI_PORT_DIR_RX == dir) ? "Rx" : "Tx");
        adpt_enet_defect_get(dev_id, chnl_id, dir, DIGI_ENET_LOS, &defect);
        ADPT_PRINT("  [%d]", defect);
        adpt_enet_defect_get(dev_id, chnl_id, dir, DIGI_ENET_LF, &defect);
        ADPT_PRINT("  [%d]", defect);
        adpt_enet_defect_get(dev_id, chnl_id, dir, DIGI_ENET_RF, &defect);
        ADPT_PRINT(" [%d]", defect);
        adpt_enet_defect_get(dev_id, chnl_id, dir, DIGI_ENET_HI_BER, &defect);
        ADPT_PRINT("    [%d]", defect);
        adpt_enet_defect_get(dev_id, chnl_id, dir, DIGI_ENET_BLK_LCK, &defect);
        ADPT_PRINT("     [%d]", defect);
        adpt_enet_defect_get(dev_id, chnl_id, dir, DIGI_ENET_LINK_DOWN, &defect);
        ADPT_PRINT("       [%d]", defect);
        adpt_enet_defect_get(dev_id, chnl_id, dir, DIGI_ENET_ALIGN_STATUS, &defect);
        ADPT_PRINT("        [%d]\r\n", defect);
    }

    if (NULL != adpt_handle->traffic_info[index].map_handle)
    {
        switch (adpt_handle->traffic_info[index].mapping_mode)
        {
            case DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP:
            case DIGI_MAPOTN_ODU4P_CBR_100_GE_GMP:
                lane_max = 20;
                break;

            case DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP:
            case DIGI_MAPOTN_ODU3P_CBR_40_GE_GMP:
                lane_max = 4;
                break;

            default:
                break;
        }

        if (0 != lane_max)
        {
            ADPT_PRINT(" -----------------------------PCS------------------------------\r\n");
            for (dir = DIGI_PORT_DIR_RX; dir <= DIGI_PORT_DIR_TX; dir = (digi_port_dir_t)(dir + 1))
            {
                ADPT_PRINT(" dLOBL[%s]:", (DIGI_PORT_DIR_RX == dir) ? "Rx" : "Tx");
                for (lane_id = 0; lane_id < lane_max; lane_id++)
                {
                    rc |= adpt_enet_pcs_get(dev_id, chnl_id, dir, lane_id, DIGI_ENET_BLK_LCK, &defect);
                    ADPT_PRINT(" %d", defect);
                }
                ADPT_PRINT("\r\n dLOAM[%s]:", (DIGI_PORT_DIR_RX == dir) ? "Rx" : "Tx");
                for (lane_id = 0; lane_id < lane_max; lane_id++)
                {
                    rc |= adpt_enet_pcs_get(dev_id, chnl_id, dir, lane_id, DIGI_ENET_LOAM, &defect);
                    ADPT_PRINT(" %d", defect);
                }
                ADPT_PRINT("\r\n dHBER[%s]:", (DIGI_PORT_DIR_RX == dir) ? "Rx" : "Tx");
                for (lane_id = 0; lane_id < lane_max; lane_id++)
                {
                    rc |= adpt_enet_pcs_get(dev_id, chnl_id, dir, lane_id, DIGI_ENET_HI_BER, &defect);
                    ADPT_PRINT(" %d", defect);
                }
                ADPT_PRINT("\r\n");
            }
        }
    }
    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_enet_alm */


/*******************************************************************************
* adpt_dump_enet_alm_i
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the Ethernet alarm interrupt information. 
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
PUBLIC PMC_ERROR adpt_dump_enet_alm_i(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL defect = FALSE;
    digi_port_dir_t dir = DIGI_PORT_DIR_RX;
    pmc_log_sev_t log_sev;
    UINT32 lane_id = 0;
    UINT32 lane_max = 0;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* defect trigger */
    rc = adpt_defect_trigger(dev_id, DIGI_BLK_FULL);
    ADPT_RETURN_VERIFY(rc);

    ADPT_PRINT("\r\n===============================================================\r\n");
    ADPT_PRINT(" dir dLOS dLF dRF dHI_BER dBLK_LCK dLINK_DOWN dALIGN_FAIL\r\n");
    for (dir = DIGI_PORT_DIR_RX; dir <= DIGI_PORT_DIR_TX; dir = (digi_port_dir_t)(dir + 1))
    {
        ADPT_PRINT(" %s", (DIGI_PORT_DIR_RX == dir) ? "Rx" : "Tx");
        adpt_enet_defect_i_get(dev_id, chnl_id, dir, DIGI_ENET_LOS, &defect);
        ADPT_PRINT("  [%d]", defect);
        adpt_enet_defect_i_get(dev_id, chnl_id, dir, DIGI_ENET_LF, &defect);
        ADPT_PRINT("  [%d]", defect);
        adpt_enet_defect_i_get(dev_id, chnl_id, dir, DIGI_ENET_RF, &defect);
        ADPT_PRINT(" [%d]", defect);
        adpt_enet_defect_i_get(dev_id, chnl_id, dir, DIGI_ENET_HI_BER, &defect);
        ADPT_PRINT("    [%d]", defect);
        adpt_enet_defect_i_get(dev_id, chnl_id, dir, DIGI_ENET_BLK_LCK, &defect);
        ADPT_PRINT("     [%d]", defect);
        adpt_enet_defect_i_get(dev_id, chnl_id, dir, DIGI_ENET_LINK_DOWN, &defect);
        ADPT_PRINT("       [%d]", defect);
        adpt_enet_defect_i_get(dev_id, chnl_id, dir, DIGI_ENET_ALIGN_STATUS, &defect);
        ADPT_PRINT("        [%d]\r\n", defect);
    }

    if (NULL != adpt_handle->traffic_info[index].map_handle)
    {
        switch (adpt_handle->traffic_info[index].mapping_mode)
        {
            case DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP:
            case DIGI_MAPOTN_ODU4P_CBR_100_GE_GMP:
                lane_max = 20;
                break;

            case DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP:
            case DIGI_MAPOTN_ODU3P_CBR_40_GE_GMP:
                lane_max = 4;
                break;

            default:
                break;
        }

        if (0 != lane_max)
        {
            ADPT_PRINT(" -----------------------------PCS------------------------------\r\n");
            for (dir = DIGI_PORT_DIR_RX; dir <= DIGI_PORT_DIR_TX; dir = (digi_port_dir_t)(dir + 1))
            {
                ADPT_PRINT(" dLOBL[%s]:", (DIGI_PORT_DIR_RX == dir) ? "Rx" : "Tx");
                for (lane_id = 0; lane_id < lane_max; lane_id++)
                {
                    rc |= adpt_enet_pcs_i_get(dev_id, chnl_id, dir, lane_id, DIGI_ENET_BLK_LCK, &defect);
                    ADPT_PRINT(" %d", defect);
                }
                ADPT_PRINT("\r\n dLOAM[%s]:", (DIGI_PORT_DIR_RX == dir) ? "Rx" : "Tx");
                for (lane_id = 0; lane_id < lane_max; lane_id++)
                {
                    rc |= adpt_enet_pcs_i_get(dev_id, chnl_id, dir, lane_id, DIGI_ENET_LOAM, &defect);
                    ADPT_PRINT(" %d", defect);
                }
                ADPT_PRINT("\r\n dHBER[%s]:", (DIGI_PORT_DIR_RX == dir) ? "Rx" : "Tx");
                for (lane_id = 0; lane_id < lane_max; lane_id++)
                {
                    rc |= adpt_enet_pcs_i_get(dev_id, chnl_id, dir, lane_id, DIGI_ENET_HI_BER, &defect);
                    ADPT_PRINT(" %d", defect);
                }
                ADPT_PRINT("\r\n");
            }
        }
    }
    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_enet_alm_i */


/*******************************************************************************
* adpt_dump_enet_pmon
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the Ethernet performance monitor information. 
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
PUBLIC PMC_ERROR adpt_dump_enet_pmon(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 pm_count = 0;
    UINT32 i = 0;
    digi_pmon_enet_errors_t enet_errors;
    digi_port_dir_t dir = DIGI_PORT_DIR_RX;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* trigger PMON */
    adpt_pmon_trigger(dev_id);

    ADPT_PRINT("\r\n===============================================================\r\n");
    for (dir = DIGI_PORT_DIR_RX; dir < LAST_DIGI_PORT_DIR; dir = (digi_port_dir_t)(dir + 1))
    {
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_FRAMES_OK_LSB, &pm_count);
        ADPT_PRINT(" DIGI_FRAMES_OK_LSB[%d]                          = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_FRAMES_OK_MSB, &pm_count);
        ADPT_PRINT(" DIGI_FRAMES_OK_MSB[%d]                          = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_FRAME_CHECK_SEQUENCE_ERRORS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_FRAME_CHECK_SEQUENCE_ERRORS_LSB[%d]        = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_FRAME_CHECK_SEQUENCE_ERRORS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_FRAME_CHECK_SEQUENCE_ERRORS_MSB[%d]        = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ALIGNMENT_ERRORS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_ALIGNMENT_ERRORS_LSB[%d]                   = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ALIGNMENT_ERRORS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_ALIGNMENT_ERRORS_MSB[%d]                   = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_PAUSE_MAC_CTRL_FRAMES_LSB, &pm_count);
        ADPT_PRINT(" DIGI_PAUSE_MAC_CTRL_FRAMES_LSB[%d]              = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_PAUSE_MAC_CTRL_FRAMES_MSB, &pm_count);
        ADPT_PRINT(" DIGI_PAUSE_MAC_CTRL_FRAMES_MSB[%d]              = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_FRAME_TOO_LONG_ERRORS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_FRAME_TOO_LONG_ERRORS_LSB[%d]              = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_FRAME_TOO_LONG_ERRORS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_FRAME_TOO_LONG_ERRORS_MSB[%d]              = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_IN_RANGE_LENGTH_ERRORS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_IN_RANGE_LENGTH_ERRORS_LSB[%d]             = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_IN_RANGE_LENGTH_ERRORS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_IN_RANGE_LENGTH_ERRORS_MSB[%d]             = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_VLAN_OK_LSB, &pm_count);
        ADPT_PRINT(" DIGI_VLAN_RX_LSB[%d]                            = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_VLAN_OK_MSB, &pm_count);
        ADPT_PRINT(" DIGI_VLAN_RX_MSB[%d]                            = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_IF_OCTS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_IF_OCTS_LSB[%d]                            = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_IF_OCTS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_IF_OCTS_MSB[%d]                            = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_IF_ERRORS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_IF_ERRORS_LSB[%d]                          = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_IF_ERRORS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_IF_ERRORS_MSB[%d]                          = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_IF_UCAST_PKTS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_IF_UCAST_PKTS_LSB[%d]                      = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_IF_UCAST_PKTS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_IF_UCAST_PKTS_MSB[%d]                      = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_IF_MULTICAST_PKTS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_IF_MULTICAST_PKTS_LSB[%d]                  = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_IF_MULTICAST_PKTS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_IF_MULTICAST_PKTS_MSB[%d]                  = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_IF_BROADCAST_PKTS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_IF_BROADCAST_PKTS_LSB[%d]                  = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_IF_BROADCAST_PKTS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_IF_BROADCAST_PKTS_MSB[%d]                  = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_DROP_EVENTS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_DROP_EVENTS_LSB[%d]            = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_DROP_EVENTS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_DROP_EVENTS_MSB[%d]            = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_OCTS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_OCTS_LSB[%d]                   = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_OCTS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_OCTS_MSB[%d]                   = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_PKTS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_PKTS_LSB[%d]                   = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_PKTS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_PKTS_MSB[%d]                   = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_UNDERSIZE_PKTS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_UNDERSIZE_PKTS_LSB[%d]         = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_UNDERSIZE_PKTS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_UNDERSIZE_PKTS_MSB[%d]         = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_PKTS_64_OCTS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_PKTS_64_OCTS_LSB[%d]           = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_PKTS_64_OCTS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_PKTS_64_OCTS_MSB[%d]           = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_PKTS_65_TO_127_OCTS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_PKTS_65_TO_127_OCTS_LSB[%d]    = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_PKTS_65_TO_127_OCTS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_PKTS_65_TO_127_OCTS_MSB[%d]    = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_PKTS_128_TO_255_OCTS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_PKTS_128_TO_255_OCTS_LSB[%d]   = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_PKTS_128_TO_255_OCTS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_PKTS_128_TO_255_OCTS_MSB[%d]   = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_PKTS_256_TO_511_OCTS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_PKTS_256_TO_511_OCTS_LSB[%d]   = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_PKTS_256_TO_511_OCTS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_PKTS_256_TO_511_OCTS_MSB[%d]   = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_PKTS_512_TO_1023_OCTS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_PKTS_512_TO_1023_OCTS_LSB[%d]  = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_PKTS_512_TO_1023_OCTS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_PKTS_512_TO_1023_OCTS_MSB[%d]  = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_PKTS_1024_TO_1518_OCTS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_PKTS_1024_TO_1518_OCTS_LSB[%d] = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_PKTS_1024_TO_1518_OCTS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_PKTS_1024_TO_1518_OCTS_MSB[%d] = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_PKTS_1519_TO_MAX_OCTS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_PKTS_1519_TO_MAX_OCTS_LSB[%d]  = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_PKTS_1519_TO_MAX_OCTS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_PKTS_1519_TO_MAX_OCTS_MSB[%d]  = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_OVERSIZE_PKTS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_OVERSIZE_PKTS_LSB[%d]          = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_OVERSIZE_PKTS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_OVERSIZE_PKTS_MSB[%d]          = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_JABBER_LSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_JABBER_LSB[%d]                 = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_JABBER_MSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_JABBER_MSB[%d]                 = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_FRAGMENTS_LSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_FRAGMENTS_LSB[%d]              = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_ETHER_STATS_FRAGMENTS_MSB, &pm_count);
        ADPT_PRINT(" DIGI_ETHER_STATS_FRAGMENTS_MSB[%d]              = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_MAC_CONTROL_FRAMES_LSB, &pm_count);
        ADPT_PRINT(" DIGI_MAC_CONTROL_FRAMES_RX_LSB[%d]              = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_MAC_CONTROL_FRAMES_MSB, &pm_count);
        ADPT_PRINT(" DIGI_MAC_CONTROL_FRAMES_RX_MSB[%d]              = [%u] \r\n", dir, pm_count);
        rc |= adpt_enet_pm_get(dev_id, chnl_id, dir, DIGI_LLDP_PKT_CNT, &pm_count);
        ADPT_PRINT(" DIGI_LLDP_PKT_CNT[%d]                           = [%u] \r\n", dir, pm_count);
    }

    ADPT_PRINT(" --------------------------------------------------------------\r\n");
    for (dir = DIGI_PORT_DIR_RX; dir < LAST_DIGI_PORT_DIR; dir = (digi_port_dir_t)(dir + 1))
    {
        PMC_MEMSET((void*)&enet_errors, 0, sizeof(digi_pmon_enet_errors_t));
        rc |= adpt_enet_errors_get(dev_id, chnl_id, dir, &enet_errors);
        ADPT_PRINT(" BLOCK_ERROR_COUNT_40_BIT_LSB[%d]                = [%u] \r\n", 
                   dir, enet_errors.block_error_count_40_bit_lsb);
        ADPT_PRINT(" BLOCK_ERROR_COUNT_40_BIT_MSB[%d]                = [%u] \r\n", 
                   dir, enet_errors.block_error_count_40_bit_msb);
        for (i = 0; i < 20; i++)
        {
            ADPT_PRINT(" PER_LANE_BIP_COUNT[%d][%2d]                      = [%u] \r\n", 
                       dir, i, enet_errors.per_lane_bip_count[i]);
        }
        ADPT_PRINT(" CPB_PACKET_DROP_COUNT[%d]                       = [%u] \r\n", 
                   dir, enet_errors.block_error_count_40_bit_msb);
        ADPT_PRINT(" ETIME_REFLECT_PATH_GOOD_PKTS[%d]                = [%u] \r\n", 
                   dir, enet_errors.block_error_count_40_bit_msb);
        ADPT_PRINT(" CPB_DCPB_SLV_DPI_PACKET_DROP_COUNT[%d]          = [%u] \r\n", 
                   dir, enet_errors.cpb_dcpb_slv_dpi_packet_drop_count);
    }

    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_enet_pmon */


/*******************************************************************************
* adpt_dump_enet_int
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the Ethernet interrupt configuration information. 
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
PUBLIC PMC_ERROR adpt_dump_enet_int(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_enet_int_en_t int_table;
    digi_port_dir_t dir = DIGI_PORT_DIR_RX;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    ADPT_PRINT("\r\n===============================================================\r\n");
    ADPT_PRINT(" dLOS dLF dRF dHI_BER dBLK_LCK dSYNC_FAIL dLOAM\r\n");
    for (dir = DIGI_PORT_DIR_RX; dir < LAST_DIGI_PORT_DIR; dir = (digi_port_dir_t)(dir + 1))
    {
        PMC_MEMSET((void*)&int_table, 0, sizeof(int_table));
        rc = adpt_enet_int_en_get(dev_id, chnl_id, dir, &int_table); 
        ADPT_PRINT(" [%d]", int_table.phy_los);
        ADPT_PRINT("  [%d]", int_table.rx_loc_fault);
        ADPT_PRINT(" [%d]", int_table.rx_rem_fault);
        ADPT_PRINT("    [%d]", int_table.hi_ber);
        ADPT_PRINT("     [%d]", int_table.block_lock);
        ADPT_PRINT("       [%d]", int_table.align_status);
        ADPT_PRINT("     [%d]\r\n", int_table.loam);
    }

    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_enet_int */


/*******************************************************************************
* adpt_dump_fc_alm
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the fiber channel defect information. 
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
PUBLIC PMC_ERROR adpt_dump_fc_alm(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_port_dir_t dir = DIGI_PORT_DIR_RX;
    BOOL defect = FALSE;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable)
        || (NULL == adpt_handle->traffic_info[index].cbr_handle))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* defect trigger */
    rc = adpt_defect_trigger(dev_id, DIGI_BLK_FULL);
    ADPT_RETURN_VERIFY(rc);

    ADPT_PRINT("\r\n===============================================================\r\n");
    switch (adpt_handle->traffic_info[index].traffic_mode)
    {
        /* FC-1200 */
        case DIGI_LINE_FC1200_ODU1F:
        case DIGI_LINE_FC1200_ODU2F:
        case DIGI_LINE_FC1200_GFP_ODU2E:
        case DIGI_SYS_FC1200_ODU1F:
        case DIGI_SYS_FC1200_ODU2F:
        case DIGI_SYS_FC1200_GFP_ODU2E:
            ADPT_PRINT(" dir dRF dLF dHIGH_BER dLOSS_SYNC dLOS \r\n");
            ADPT_PRINT(" %s", "Rx");
            adpt_fc1200_defect_get(dev_id, chnl_id, DIGI_FC_RX_RF, &defect);
            ADPT_PRINT("  [%d]", defect);
            adpt_fc1200_defect_get(dev_id, chnl_id, DIGI_FC_RX_LF, &defect);
            ADPT_PRINT(" [%d]", defect);
            adpt_fc1200_defect_get(dev_id, chnl_id, DIGI_FC_RX_HIGH_BER, &defect);
            ADPT_PRINT("    [%d]", defect);
            adpt_fc1200_defect_get(dev_id, chnl_id, DIGI_FC_RX_LOSS_SYNC, &defect);
            ADPT_PRINT("        [%d]", defect);
            adpt_fc1200_defect_get(dev_id, chnl_id, DIGI_FC_RX_LOS, &defect);
            ADPT_PRINT("    [%d]\r\n", defect);
            ADPT_PRINT(" dir dLF \r\n");
            ADPT_PRINT(" %s", "Tx");
            adpt_fc1200_defect_get(dev_id, chnl_id, DIGI_FC_TX_LF, &defect);
            ADPT_PRINT("  [%d]\r\n", defect);
            break;

        /* FC-800 */
        case DIGI_LINE_FC800:
        case DIGI_SYS_FC800:
            ADPT_PRINT(" dir dDISP_INV dXDET dLOLB dASD dSYNC dLCV \r\n");
            for (dir = DIGI_PORT_DIR_RX; dir < LAST_DIGI_PORT_DIR; dir = (digi_port_dir_t)(dir + 1))
            {
                ADPT_PRINT(" %s", (DIGI_PORT_DIR_RX == dir) ? "Rx" : "Tx");
                adpt_fc800_defect_get(dev_id, chnl_id, dir, DIGI_FC_DISP_INV, &defect);
                ADPT_PRINT("      [%d]", defect);
                adpt_fc800_defect_get(dev_id, chnl_id, dir, DIGI_FC_XDET, &defect);
                ADPT_PRINT("    [%d]", defect);
                adpt_fc800_defect_get(dev_id, chnl_id, dir, DIGI_FC_DLOLB, &defect);
                ADPT_PRINT("   [%d]", defect);
                adpt_fc800_defect_get(dev_id, chnl_id, dir, DIGI_FC_ASD, &defect);
                ADPT_PRINT("  [%d]", defect);
                adpt_fc800_defect_get(dev_id, chnl_id, dir, DIGI_FC_SYNC, &defect);
                ADPT_PRINT("   [%d]", defect);
                adpt_fc800_defect_get(dev_id, chnl_id, dir, DIGI_FC_LCV, &defect);
                ADPT_PRINT("  [%d]\r\n", defect);
            }
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_fc_alm */


/*******************************************************************************
* adpt_dump_fc_alm_i
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the fiber channel defect interrupt information. 
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
PUBLIC PMC_ERROR adpt_dump_fc_alm_i(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_port_dir_t dir = DIGI_PORT_DIR_RX;
    BOOL defect = FALSE;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable)
        || (NULL == adpt_handle->traffic_info[index].cbr_handle))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* defect trigger */
    rc = adpt_defect_trigger(dev_id, DIGI_BLK_FULL);
    ADPT_RETURN_VERIFY(rc);

    ADPT_PRINT("\r\n===============================================================\r\n");
    switch (adpt_handle->traffic_info[index].traffic_mode)
    {
        /* FC-1200 */
        case DIGI_LINE_FC1200_ODU1F:
        case DIGI_LINE_FC1200_ODU2F:
        case DIGI_LINE_FC1200_GFP_ODU2E:
        case DIGI_SYS_FC1200_ODU1F:
        case DIGI_SYS_FC1200_ODU2F:
        case DIGI_SYS_FC1200_GFP_ODU2E:
            ADPT_PRINT(" dir dRF dLF dHIGH_BER dLOSS_SYNC dLOS \r\n");
            ADPT_PRINT(" %s", "Rx");
            adpt_fc1200_defect_i_get(dev_id, chnl_id, DIGI_FC_RX_RF, &defect);
            ADPT_PRINT("  [%d]", defect);
            adpt_fc1200_defect_i_get(dev_id, chnl_id, DIGI_FC_RX_LF, &defect);
            ADPT_PRINT(" [%d]", defect);
            adpt_fc1200_defect_i_get(dev_id, chnl_id, DIGI_FC_RX_HIGH_BER, &defect);
            ADPT_PRINT("    [%d]", defect);
            adpt_fc1200_defect_i_get(dev_id, chnl_id, DIGI_FC_RX_LOSS_SYNC, &defect);
            ADPT_PRINT("        [%d]", defect);
            adpt_fc1200_defect_i_get(dev_id, chnl_id, DIGI_FC_RX_LOS, &defect);
            ADPT_PRINT("    [%d]\r\n", defect);
            ADPT_PRINT(" dir dLF \r\n");
            ADPT_PRINT(" %s", "Tx");
            adpt_fc1200_defect_i_get(dev_id, chnl_id, DIGI_FC_TX_LF, &defect);
            ADPT_PRINT("  [%d]\r\n", defect);
            break;

        /* FC-800 */
        case DIGI_LINE_FC800:
        case DIGI_SYS_FC800:
            ADPT_PRINT(" dir dDISP_INV dXDET dLOLB dASD dSYNC dLCV \r\n");
            for (dir = DIGI_PORT_DIR_RX; dir < LAST_DIGI_PORT_DIR; dir = (digi_port_dir_t)(dir + 1))
            {
                ADPT_PRINT(" %s", (DIGI_PORT_DIR_RX == dir) ? "Rx" : "Tx");
                adpt_fc800_defect_i_get(dev_id, chnl_id, dir, DIGI_FC_DISP_INV, &defect);
                ADPT_PRINT("      [%d]", defect);
                adpt_fc800_defect_i_get(dev_id, chnl_id, dir, DIGI_FC_XDET, &defect);
                ADPT_PRINT("    [%d]", defect);
                adpt_fc800_defect_i_get(dev_id, chnl_id, dir, DIGI_FC_DLOLB, &defect);
                ADPT_PRINT("   [%d]", defect);
                adpt_fc800_defect_i_get(dev_id, chnl_id, dir, DIGI_FC_ASD, &defect);
                ADPT_PRINT("  [%d]", defect);
                adpt_fc800_defect_i_get(dev_id, chnl_id, dir, DIGI_FC_SYNC, &defect);
                ADPT_PRINT("   [%d]", defect);
                adpt_fc800_defect_i_get(dev_id, chnl_id, dir, DIGI_FC_LCV, &defect);
                ADPT_PRINT("  [%d]\r\n", defect);
            }
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_fc_alm_i */


/*******************************************************************************
* adpt_dump_fc_pmon
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the fiber channel performance monitor information. 
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
PUBLIC PMC_ERROR adpt_dump_fc_pmon(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 pm_count = 0;
    digi_port_dir_t dir = DIGI_PORT_DIR_RX;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable)
        || (NULL == adpt_handle->traffic_info[index].cbr_handle))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* trigger PMON */
    adpt_pmon_trigger(dev_id);

    ADPT_PRINT("\r\n===============================================================\r\n");
    switch (adpt_handle->traffic_info[index].traffic_mode)
    {
        /* FC-1200 */
        case DIGI_LINE_FC1200_ODU1F:
        case DIGI_LINE_FC1200_ODU2F:
        case DIGI_LINE_FC1200_GFP_ODU2E:
        case DIGI_SYS_FC1200_ODU1F:
        case DIGI_SYS_FC1200_ODU2F:
        case DIGI_SYS_FC1200_GFP_ODU2E:
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_RX_HIGH_ERR_CNT, &pm_count);
            ADPT_PRINT(" DIGI_RX_HIGH_ERR_CNT             = [%u] \r\n", pm_count);
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_RX_BLK_TYPE_ERR_CNT, &pm_count);
            ADPT_PRINT(" DIGI_RX_BLK_TYPE_ERR_CNT         = [%u] \r\n", pm_count);
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_RX_INVLD_BLK_CNT, &pm_count);
            ADPT_PRINT(" DIGI_RX_INVLD_BLK_CNT            = [%u] \r\n", pm_count);
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_RX_DATA_BYTE_CNT_LSB, &pm_count);
            ADPT_PRINT(" DIGI_RX_DATA_BYTE_CNT_LSB        = [%u] \r\n", pm_count);
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_RX_DATA_BYTE_CNT_MSB, &pm_count);
            ADPT_PRINT(" DIGI_RX_DATA_BYTE_CNT_MSB        = [%u] \r\n", pm_count);
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_RX_OS_CNT, &pm_count);
            ADPT_PRINT(" DIGI_RX_OS_CNT                   = [%u] \r\n", pm_count);
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_RX_VALID_FRM_CNT, &pm_count);
            ADPT_PRINT(" DIGI_RX_VALID_FRM_CNT            = [%u] \r\n", pm_count);
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_RX_INVALID_FRM_CNT, &pm_count);
            ADPT_PRINT(" DIGI_RX_INVALID_FRM_CNT          = [%u] \r\n", pm_count);
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_RX_VALID_FRM_BYTE_CNT, &pm_count);
            ADPT_PRINT(" DIGI_RX_VALID_FRM_BYTE_CNT       = [%u] \r\n", pm_count);
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_RX_GFP_FRM_CNT, &pm_count);
            ADPT_PRINT(" DIGI_RX_GFP_FRM_CNT              = [%u] \r\n", pm_count);
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_RX_GFP_ERR_FRM_CNT, &pm_count);
            ADPT_PRINT(" DIGI_RX_GFP_ERR_FRM_CNT          = [%u] \r\n", pm_count);
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_TX_BYTE_CNT_LSB, &pm_count);
            ADPT_PRINT(" DIGI_TX_BYTE_CNT_LSB             = [%u] \r\n", pm_count);
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_TX_BYTE_CNT_MSB, &pm_count);
            ADPT_PRINT(" DIGI_TX_BYTE_CNT_MSB             = [%u] \r\n", pm_count);
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_TX_OS_CNT, &pm_count);
            ADPT_PRINT(" DIGI_TX_OS_CNT                   = [%u] \r\n", pm_count);
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_TX_ERR_CTRL_BLK_CNT, &pm_count);
            ADPT_PRINT(" DIGI_TX_ERR_CTRL_BLK_CNT         = [%u] \r\n", pm_count);
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_TX_LF_OS_CNT, &pm_count);
            ADPT_PRINT(" DIGI_TX_LF_OS_CNT                = [%u] \r\n", pm_count);
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_TX_VALID_FRM_CNT, &pm_count);
            ADPT_PRINT(" DIGI_TX_VALID_FRM_CNT            = [%u] \r\n", pm_count);
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_TX_INVALID_FRM_CNT, &pm_count);
            ADPT_PRINT(" DIGI_TX_INVALID_FRM_CNT          = [%u] \r\n", pm_count);
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_TX_VALID_FRM_BYTE_CNT, &pm_count);
            ADPT_PRINT(" DIGI_TX_VALID_FRM_BYTE_CNT       = [%u] \r\n", pm_count);
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_TX_CRC_24_ERR_CNT, &pm_count);
            ADPT_PRINT(" DIGI_TX_CRC_24_ERR_CNT           = [%u] \r\n", pm_count);
            adpt_fc1200_pm_get(dev_id, chnl_id, DIGI_TX_INVALID_BLK_HDR_CNT, &pm_count);
            ADPT_PRINT(" DIGI_TX_INVALID_BLK_HDR_CNT      = [%u] \r\n", pm_count);
            break;

        /* FC-800 */
        case DIGI_LINE_FC800:
        case DIGI_SYS_FC800:
            for (dir = DIGI_PORT_DIR_RX; dir < LAST_DIGI_PORT_DIR; dir = (digi_port_dir_t)(dir + 1))
            {
                adpt_fc800_pm_get(dev_id, chnl_id, dir, DIGI_GOOD_FRM_CNT, &pm_count);
                ADPT_PRINT(" DIGI_GOOD_FRM_CNT[%d]         = [%u] \r\n", dir, pm_count);
                adpt_fc800_pm_get(dev_id, chnl_id, dir, DIGI_BAD_FRM_CNT, &pm_count);
                ADPT_PRINT(" DIGI_BAD_FRM_CNT[%d]          = [%u] \r\n", dir, pm_count);
                adpt_fc800_pm_get(dev_id, chnl_id, dir, DIGI_BYTE_CNT, &pm_count);
                ADPT_PRINT(" DIGI_BYTE_CNT[%d]             = [%u] \r\n", dir, pm_count);
                adpt_fc800_pm_get(dev_id, chnl_id, dir, DIGI_BAD_CRC_CNT, &pm_count);
                ADPT_PRINT(" DIGI_BAD_CRC_CNT[%d]          = [%u] \r\n", dir, pm_count);
                adpt_fc800_pm_get(dev_id, chnl_id, dir, DIGI_TOTAL_PKT_CNT, &pm_count);
                ADPT_PRINT(" DIGI_TOTAL_PKT_CNT[%d]        = [%u] \r\n", dir, pm_count);
                adpt_fc800_pm_get(dev_id, chnl_id, dir, DIGI_LCV_CNT, &pm_count);
                ADPT_PRINT(" DIGI_LCV_CNT[%d]              = [%u] \r\n", dir, pm_count);
            }
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_enet_pmon */


/*******************************************************************************
* adpt_dump_fc_int
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the fiber channel interrupt configuration information. 
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
PUBLIC PMC_ERROR adpt_dump_fc_int(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_fc1200_int_en_t int_table;
    digi_fc800_int_en_t fc800_int_table;
    digi_port_dir_t dir = DIGI_PORT_DIR_RX;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable)
        || (NULL == adpt_handle->traffic_info[index].cbr_handle))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* trigger PMON */
    adpt_pmon_trigger(dev_id);

    ADPT_PRINT("\r\n===============================================================\r\n");
    switch (adpt_handle->traffic_info[index].traffic_mode)
    {
        /* FC-1200 */
        case DIGI_LINE_FC1200_ODU1F:
        case DIGI_LINE_FC1200_ODU2F:
        case DIGI_LINE_FC1200_GFP_ODU2E:
        case DIGI_SYS_FC1200_ODU1F:
        case DIGI_SYS_FC1200_ODU2F:
        case DIGI_SYS_FC1200_GFP_ODU2E:
            ADPT_PRINT(" dRF dLF dHIGH_BER dLOSS_SYNC dLOS \r\n");
            PMC_MEMSET((void*)&int_table, 0, sizeof(int_table));
            rc = adpt_fc1200_int_en_get(dev_id, chnl_id, &int_table);
            ADPT_PRINT(" [%d]", int_table.drf);
            ADPT_PRINT(" [%d]", int_table.dlf);
            ADPT_PRINT("    [%d]", int_table.dhigh_ber);
            ADPT_PRINT("        [%d]", int_table.dloss_sync);
            ADPT_PRINT("    [%d]\r\n", int_table.dlos);
            break;

        /* FC-800 */
        case DIGI_LINE_FC800:
        case DIGI_SYS_FC800:
            ADPT_PRINT(" dir dDISP_INV dXDET dLOLB dASD dSYNC dLCV \r\n");
            for (dir = DIGI_PORT_DIR_RX; dir < LAST_DIGI_PORT_DIR; dir = (digi_port_dir_t)(dir + 1))
            {
                ADPT_PRINT(" %s", (DIGI_PORT_DIR_RX == dir) ? "Rx" : "Tx");
                PMC_MEMSET((void*)&fc800_int_table, 0, sizeof(fc800_int_table));
                rc = adpt_fc800_int_en_get(dev_id, chnl_id, dir, &fc800_int_table);
                ADPT_PRINT("      [%d]", fc800_int_table.ddisp_inv);
                ADPT_PRINT("    [%d]", fc800_int_table.dxdet);
                ADPT_PRINT("   [%d]", fc800_int_table.dlolb);
                ADPT_PRINT("  [%d]", fc800_int_table.dasd);
                ADPT_PRINT("   [%d]", fc800_int_table.dsync);
                ADPT_PRINT("  [%d]\r\n", fc800_int_table.dlcv);
            }
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_fc_int */


/*******************************************************************************
* adpt_dump_ptp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the PTP related information. 
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
PUBLIC PMC_ERROR adpt_dump_ptp(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* check channel ID status */
    if (!(adpt_handle->traffic_info[index].enable))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_ptp */


/*******************************************************************************
* adpt_dump_sifd_alm
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the SIFD alarm information. 
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dump_sifd_alm(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    pmc_log_sev_t log_sev;
    UINT32 index = 0;
    sifd_int_link_t *sifd_link = NULL;
    digi_sifd_int_t sifd_org;
    digi_sifd_int_t sifd_new;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* enable orignal interrupt status */
    rc = digi_sifd_int_enabled_get(adpt_handle->digi_handle, &sifd_org);
    rc = digi_sifd_int_enabled_get(adpt_handle->digi_handle, &sifd_new);
    ADPT_RETURN_VERIFY(rc);

    /* enable interrupt status */
    for (index = 0; index < DIGI_SIFD_ILKN_NUM; index++)
    {
        PMC_MEMSET((void*)&sifd_new.sifd_link[index], TRUE, sizeof(sifd_int_link_t));

        if (NULL != sifd_new.handle[index])
        {
            rc = digi_sifd_int_link_enable(adpt_handle->digi_handle, 
                                           sifd_new.handle[index], 
                                           &sifd_new.sifd_link[index]);
        }
    }
    ADPT_RETURN_VERIFY(rc);

    /* defect trigger */
    rc = adpt_defect_trigger(dev_id, DIGI_BLK_FULL);
    ADPT_RETURN_VERIFY(rc);

    ADPT_PRINT("\r\n===============================================================\r\n");

    for (index = 0; index < DIGI_SIFD_ILKN_NUM; index++)
    {
        if (NULL == adpt_handle->pmon_data->sifd_defect.handle[index])
        {
            continue;
        }

        if (0 != index)
        {
            ADPT_PRINT(" --------------------------------------------------------------\r\n");
        }

        sifd_link = &(adpt_handle->pmon_data->sifd_defect.sifd_link[index]);
        ADPT_PRINT(" rx_mf_err                 = [%u]\r\n", sifd_link->rx_mf_err);
        ADPT_PRINT(" rx_wrd_sync_lane          = [%u]\r\n", sifd_link->rx_wrd_sync_lane);
        ADPT_PRINT(" rx_wrd_sync_cnt_fill_lane = [%u]\r\n", sifd_link->rx_wrd_sync_cnt_fill_lane);
        ADPT_PRINT(" rx_crc32_err              = [%u]\r\n", sifd_link->rx_crc32_err);
        ADPT_PRINT(" rc_crc32_cnt_fill_lane    = [%u]\r\n", sifd_link->rc_crc32_cnt_fill_lane);
        ADPT_PRINT(" rx_badctl_err             = [%u]\r\n", sifd_link->rx_badctl_err);
        ADPT_PRINT(" rx_scram_err              = [%u]\r\n", sifd_link->rx_scram_err);
        ADPT_PRINT(" rx_cdr_err                = [%u]\r\n", sifd_link->rx_cdr_err);
        ADPT_PRINT(" rx_crd_cnt_fill_lane      = [%u]\r\n", sifd_link->rx_crd_cnt_fill_lane);
        ADPT_PRINT(" rx_wrd_sync_chg           = [%u]\r\n", sifd_link->rx_wrd_sync_chg);
        ADPT_PRINT(" rx_align_good             = [%d]\r\n", sifd_link->rx_align_good);
        ADPT_PRINT(" rx_align_err              = [%d]\r\n", sifd_link->rx_align_err);
        ADPT_PRINT(" current                   = [%d]\r\n", sifd_link->current);
        ADPT_PRINT(" valid                     = [%d]\r\n", sifd_link->valid);
        ADPT_PRINT(" sync                      = [%d]\r\n", sifd_link->sync);
        ADPT_PRINT(" sif                       = [%d]\r\n", sifd_link->sif);
        ADPT_PRINT(" tx_sl                     = [%u]\r\n", sifd_link->tx_sl);
        ADPT_PRINT(" rx_status_msg_valid_all   = [%d]\r\n", sifd_link->rx_status_msg_valid_all);
        ADPT_PRINT(" rx_status_msg_valid_any   = [%d]\r\n", sifd_link->rx_status_msg_valid_any);
        ADPT_PRINT(" rx_status_msg_sync        = [%d]\r\n", sifd_link->rx_status_msg_sync);
        ADPT_PRINT(" sif_all                   = [%d]\r\n", sifd_link->sif_all);
        ADPT_PRINT(" rx_sl                     = [%u]\r\n", sifd_link->rx_sl);
        ADPT_PRINT(" sif_any                   = [%d]\r\n", sifd_link->sif_any);
    }

    ADPT_PRINT("===============================================================\r\n");

    /* recover interrupt status */
    for (index = 0; index < DIGI_SIFD_ILKN_NUM; index++)
    {
        if (NULL != sifd_new.handle[index])
        {
            rc = digi_sifd_int_link_disable(adpt_handle->digi_handle, 
                                            sifd_new.handle[index], 
                                            &sifd_new.sifd_link[index]);
        }

        if (NULL != sifd_org.handle[index])
        {
            rc = digi_sifd_int_link_enable(adpt_handle->digi_handle, 
                                           sifd_org.handle[index], 
                                           &sifd_org.sifd_link[index]);
        }
    }
    ADPT_RETURN_VERIFY(rc);

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_sifd_alm */


/*******************************************************************************
* adpt_dump_sifd_alm_i
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the SIFD alarm interrupt information. 
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dump_sifd_alm_i(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    pmc_log_sev_t log_sev;
    UINT32 index = 0;
    sifd_int_link_t *sifd_link = NULL;
    digi_sifd_int_t sifd_org;
    digi_sifd_int_t sifd_new;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* enable orignal interrupt status */
    rc = digi_sifd_int_enabled_get(adpt_handle->digi_handle, &sifd_org);
    rc = digi_sifd_int_enabled_get(adpt_handle->digi_handle, &sifd_new);
    ADPT_RETURN_VERIFY(rc);

    /* enable interrupt status */
    for (index = 0; index < DIGI_SIFD_ILKN_NUM; index++)
    {
        PMC_MEMSET((void*)&sifd_new.sifd_link[index], TRUE, sizeof(sifd_int_link_t));

        if (NULL != sifd_new.handle[index])
        {
            rc = digi_sifd_int_link_enable(adpt_handle->digi_handle, 
                                           sifd_new.handle[index], 
                                           &sifd_new.sifd_link[index]);
        }
    }
    ADPT_RETURN_VERIFY(rc);

    /* defect trigger */
    rc = adpt_defect_trigger(dev_id, DIGI_BLK_FULL);
    ADPT_RETURN_VERIFY(rc);

    ADPT_PRINT("\r\n===============================================================\r\n");

    for (index = 0; index < DIGI_SIFD_ILKN_NUM; index++)
    {
        if (NULL == adpt_handle->pmon_data->sifd_defect.handle[index])
        {
            continue;
        }

        if (0 != index)
        {
            ADPT_PRINT(" --------------------------------------------------------------\r\n");
        }

        sifd_link = &(adpt_handle->pmon_data->sifd_defect.sifd_link[index]);
        ADPT_PRINT(" fifo_over_i                        = [%d]\r\n", sifd_link->fifo_over_i);
        ADPT_PRINT(" fifo_under_i                       = [%d]\r\n", sifd_link->fifo_under_i);
        ADPT_PRINT(" oob_fc_err_i                       = [%d]\r\n", sifd_link->oob_fc_err_i);
        ADPT_PRINT(" tx_status_msg_change_i             = [%d]\r\n", sifd_link->tx_status_msg_change_i);
        ADPT_PRINT(" tx_fc_cnt_fill_i                   = [%d]\r\n", sifd_link->tx_fc_cnt_fill_i);
        ADPT_PRINT(" tx_chan_cnt_fill_i                 = [%d]\r\n", sifd_link->tx_chan_cnt_fill_i);
        ADPT_PRINT(" ctlbit_err_i                       = [%d]\r\n", sifd_link->ctlbit_err_i);
        ADPT_PRINT(" mf_err_i                           = [%d]\r\n", sifd_link->mf_err_i);
        ADPT_PRINT(" badctl_err_i                       = [%d]\r\n", sifd_link->badctl_err_i);
        ADPT_PRINT(" scram_err_i                        = [%d]\r\n", sifd_link->scram_err_i);
        ADPT_PRINT(" crc32_err_i                        = [%d]\r\n", sifd_link->crc32_err_i);
        ADPT_PRINT(" cdr_err_i                          = [%d]\r\n", sifd_link->cdr_err_i);
        ADPT_PRINT(" wrd_sync_change_i                  = [%d]\r\n", sifd_link->wrd_sync_change_i);
        ADPT_PRINT(" align_change_i                     = [%d]\r\n", sifd_link->align_change_i);
        ADPT_PRINT(" align_err_i                        = [%d]\r\n", sifd_link->align_err_i);
        ADPT_PRINT(" burst_err_i                        = [%d]\r\n", sifd_link->burst_err_i);
        ADPT_PRINT(" bmin_err_i                         = [%d]\r\n", sifd_link->bmin_err_i);
        ADPT_PRINT(" bmax_err_i                         = [%d]\r\n", sifd_link->bmax_err_i);
        ADPT_PRINT(" crc24_err_i                        = [%d]\r\n", sifd_link->crc24_err_i);
        ADPT_PRINT(" ib_fc_err_i                        = [%d]\r\n", sifd_link->ib_fc_err_i);
        ADPT_PRINT(" rx_status_msg_change_i             = [%d]\r\n", sifd_link->rx_status_msg_change_i);
        ADPT_PRINT(" badctl_cnt_fill_i                  = [%d]\r\n", sifd_link->badctl_cnt_fill_i);
        ADPT_PRINT(" crc32_cnt_fill_i                   = [%d]\r\n", sifd_link->crc32_cnt_fill_i);
        ADPT_PRINT(" cdr_cnt_fill_i                     = [%d]\r\n", sifd_link->cdr_cnt_fill_i);
        ADPT_PRINT(" wrd_sync_cnt_fill_i                = [%d]\r\n", sifd_link->wrd_sync_cnt_fill_i);
        ADPT_PRINT(" align_fail_cnt_fill_i              = [%d]\r\n", sifd_link->align_fail_cnt_fill_i);
        ADPT_PRINT(" align_cnt_fill_i                   = [%d]\r\n", sifd_link->align_cnt_fill_i);
        ADPT_PRINT(" burst_cnt_fill_i                   = [%d]\r\n", sifd_link->burst_cnt_fill_i);
        ADPT_PRINT(" bmin_cnt_fill_i                    = [%d]\r\n", sifd_link->bmin_cnt_fill_i);
        ADPT_PRINT(" bmax_cnt_fill_i                    = [%d]\r\n", sifd_link->bmax_cnt_fill_i);
        ADPT_PRINT(" crc24_cnt_fill_i                   = [%d]\r\n", sifd_link->crc24_cnt_fill_i);
        ADPT_PRINT(" rx_fc_cnt_fill_i                   = [%d]\r\n", sifd_link->rx_fc_cnt_fill_i);
        ADPT_PRINT(" rx_chan_cnt_fill_i                 = [%d]\r\n", sifd_link->rx_chan_cnt_fill_i);
        ADPT_PRINT(" cpb_tx_buf_un_int_i0               = [%d]\r\n", sifd_link->cpb_tx_buf_un_int_i0);
        ADPT_PRINT(" cpb_tx_buf_align_int_i0            = [%d]\r\n", sifd_link->cpb_tx_buf_align_int_i0);
        ADPT_PRINT(" oduk_tx_buf_un_int_i0              = [%d]\r\n", sifd_link->oduk_tx_buf_un_int_i0);
        ADPT_PRINT(" oduk_tx_buf_align_int_i0           = [%d]\r\n", sifd_link->oduk_tx_buf_align_int_i0);
        ADPT_PRINT(" rx_buf_un_int_i0                   = [%d]\r\n", sifd_link->rx_buf_un_int_i0);
        ADPT_PRINT(" rx_buf_align_int_i0                = [%d]\r\n", sifd_link->rx_buf_align_int_i0);
        ADPT_PRINT(" ser_rx_los_int_i0                  = [%d]\r\n", sifd_link->ser_rx_los_int_i0);
        ADPT_PRINT(" rx_sob_err_int_i0                  = [%d]\r\n", sifd_link->rx_sob_err_int_i0);
        ADPT_PRINT(" rx_burst_len_err_int_i0            = [%d]\r\n", sifd_link->rx_burst_len_err_int_i0);
        ADPT_PRINT(" rx_burst_drop_err_int_i0           = [%d]\r\n", sifd_link->rx_burst_drop_err_int_i0);
        ADPT_PRINT(" rx_pkt_ch_change_err_int_i0        = [%d]\r\n", sifd_link->rx_pkt_ch_change_err_int_i0);
        ADPT_PRINT(" rx_eob_err_int_i0                  = [%d]\r\n", sifd_link->rx_eob_err_int_i0);
        ADPT_PRINT(" rx_sop_err_int_i0                  = [%d]\r\n", sifd_link->rx_sop_err_int_i0);
        ADPT_PRINT(" rx_eop_err_int_i0                  = [%d]\r\n", sifd_link->rx_eop_err_int_i0);
        ADPT_PRINT(" rx_il_err_all_int_i0               = [%d]\r\n", sifd_link->rx_il_err_all_int_i0);
        ADPT_PRINT(" rx_il_align_int_i0                 = [%d]\r\n", sifd_link->rx_il_align_int_i0);
        ADPT_PRINT(" rx_il_dat_err_int_i0               = [%d]\r\n", sifd_link->rx_il_dat_err_int_i0);
        ADPT_PRINT(" rx_oduk_h2b_cam_err_int_i0         = [%d]\r\n", sifd_link->rx_oduk_h2b_cam_err_int_i0);
        ADPT_PRINT(" rx_oduk_h2b_cam_mmatch_err_int_i0  = [%d]\r\n", sifd_link->rx_oduk_h2b_cam_mmatch_err_int_i0);
        ADPT_PRINT(" rx_oduk_h2b_cmpr_err_int_i0        = [%d]\r\n", sifd_link->rx_oduk_h2b_cmpr_err_int_i0);
        ADPT_PRINT(" rx_oduk_h4b_cmpr_err_int_i0        = [%d]\r\n", sifd_link->rx_oduk_h4b_cmpr_err_int_i0);

        /* interrupt clear */
        sifd_link->fifo_over_i                       = FALSE;
        sifd_link->fifo_under_i                      = FALSE;
        sifd_link->oob_fc_err_i                      = FALSE;
        sifd_link->tx_status_msg_change_i            = FALSE;
        sifd_link->tx_fc_cnt_fill_i                  = FALSE;
        sifd_link->tx_chan_cnt_fill_i                = FALSE;
        sifd_link->ctlbit_err_i                      = FALSE;
        sifd_link->mf_err_i                          = FALSE;
        sifd_link->badctl_err_i                      = FALSE;
        sifd_link->scram_err_i                       = FALSE;
        sifd_link->crc32_err_i                       = FALSE;
        sifd_link->cdr_err_i                         = FALSE;
        sifd_link->wrd_sync_change_i                 = FALSE;
        sifd_link->align_change_i                    = FALSE;
        sifd_link->align_err_i                       = FALSE;
        sifd_link->burst_err_i                       = FALSE;
        sifd_link->bmin_err_i                        = FALSE;
        sifd_link->bmax_err_i                        = FALSE;
        sifd_link->crc24_err_i                       = FALSE;
        sifd_link->ib_fc_err_i                       = FALSE;
        sifd_link->rx_status_msg_change_i            = FALSE;
        sifd_link->badctl_cnt_fill_i                 = FALSE;
        sifd_link->crc32_cnt_fill_i                  = FALSE;
        sifd_link->cdr_cnt_fill_i                    = FALSE;
        sifd_link->wrd_sync_cnt_fill_i               = FALSE;
        sifd_link->align_fail_cnt_fill_i             = FALSE;
        sifd_link->align_cnt_fill_i                  = FALSE;
        sifd_link->burst_cnt_fill_i                  = FALSE;
        sifd_link->bmin_cnt_fill_i                   = FALSE;
        sifd_link->bmax_cnt_fill_i                   = FALSE;
        sifd_link->crc24_cnt_fill_i                  = FALSE;
        sifd_link->rx_fc_cnt_fill_i                  = FALSE;
        sifd_link->rx_chan_cnt_fill_i                = FALSE;
        sifd_link->cpb_tx_buf_un_int_i0              = FALSE;
        sifd_link->cpb_tx_buf_align_int_i0           = FALSE;
        sifd_link->oduk_tx_buf_un_int_i0             = FALSE;
        sifd_link->oduk_tx_buf_align_int_i0          = FALSE;
        sifd_link->rx_buf_un_int_i0                  = FALSE;
        sifd_link->rx_buf_align_int_i0               = FALSE;
        sifd_link->ser_rx_los_int_i0                 = FALSE;
        sifd_link->rx_sob_err_int_i0                 = FALSE;
        sifd_link->rx_burst_len_err_int_i0           = FALSE;
        sifd_link->rx_burst_drop_err_int_i0          = FALSE;
        sifd_link->rx_pkt_ch_change_err_int_i0       = FALSE;
        sifd_link->rx_eob_err_int_i0                 = FALSE;
        sifd_link->rx_sop_err_int_i0                 = FALSE;
        sifd_link->rx_eop_err_int_i0                 = FALSE;
        sifd_link->rx_il_err_all_int_i0              = FALSE;
        sifd_link->rx_il_align_int_i0                = FALSE;
        sifd_link->rx_il_dat_err_int_i0              = FALSE;
        sifd_link->rx_oduk_h2b_cam_err_int_i0        = FALSE;
        sifd_link->rx_oduk_h2b_cam_mmatch_err_int_i0 = FALSE;
        sifd_link->rx_oduk_h2b_cmpr_err_int_i0       = FALSE;
        sifd_link->rx_oduk_h4b_cmpr_err_int_i0       = FALSE;
    }

    ADPT_PRINT("===============================================================\r\n");

    /* recover interrupt status */
    for (index = 0; index < DIGI_SIFD_ILKN_NUM; index++)
    {
        if (NULL != sifd_new.handle[index])
        {
            rc = digi_sifd_int_link_disable(adpt_handle->digi_handle, 
                                            sifd_new.handle[index], 
                                            &sifd_new.sifd_link[index]);
        }

        if (NULL != sifd_org.handle[index])
        {
            rc = digi_sifd_int_link_enable(adpt_handle->digi_handle, 
                                           sifd_org.handle[index], 
                                           &sifd_org.sifd_link[index]);
        }
    }
    ADPT_RETURN_VERIFY(rc);

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_sifd_alm_i */


/*******************************************************************************
* adpt_dump_sifd_pmon
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the SIFD PMON information. 
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dump_sifd_pmon(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    pmc_log_sev_t log_sev;
    UINT32 ilkn_sum = 0;
    UINT32 index = 0;
    UINT32 pm_count = 0;
    UINT32 pm_buffer[DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX];
    UINT32 i = 0;
    ilkn_sifd_type_t sifd_type[2];
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* trigger PMON */
    adpt_pmon_trigger(dev_id);

    switch (adpt_handle->dev_info->sifd_type)
    {
        case ILKN_SIFD_1:
            ilkn_sum = 1;
            sifd_type[0] = ILKN_SIFD_1;
            break;

        case ILKN_SIFD_2:
            ilkn_sum = 1;
            sifd_type[0] = ILKN_SIFD_2;
            break;
            
        case ILKN_SIFD_1_2:
            ilkn_sum = 2;
            sifd_type[0] = ILKN_SIFD_1;
            sifd_type[1] = ILKN_SIFD_2;
            break;

        default:
            break;
    }

    ADPT_PRINT("\r\n===============================================================\r\n");
    for (index = 0; index < ilkn_sum; index++)
    {
        adpt_sifd_pm_get(dev_id, sifd_type[index], DIGI_SIFD_TX_ERR_CNT_CPB, &pm_count);
        ADPT_PRINT(" DIGI_SIFD_TX_ERR_CNT_CPB         = [%u] \r\n", pm_count);
        adpt_sifd_pm_get(dev_id, sifd_type[index], DIGI_SIFD_TX_ERR_CNT_ODUK, &pm_count);
        ADPT_PRINT(" DIGI_SIFD_TX_ERR_CNT_ODUK        = [%u] \r\n", pm_count);
        adpt_sifd_pm_get(dev_id, sifd_type[index], DIGI_SIFD_RX_ERR_CNT, &pm_count);
        ADPT_PRINT(" DIGI_SIFD_RX_ERR_CNT             = [%u] \r\n", pm_count);
        adpt_sifd_pm_get(dev_id, sifd_type[index], DIGI_SIFD_TX_DIAG_CNT_CPB, &pm_count);
        ADPT_PRINT(" DIGI_SIFD_TX_DIAG_CNT_CPB        = [%u] \r\n", pm_count);
        adpt_sifd_pm_get(dev_id, sifd_type[index], DIGI_SIFD_TX_DIAG_CNT_ODUK, &pm_count);
        ADPT_PRINT(" DIGI_SIFD_TX_DIAG_CNT_ODUK       = [%u] \r\n", pm_count);
        adpt_sifd_pm_get(dev_id, sifd_type[index], DIGI_SIFD_RX_DIAG_CNT_CPB, &pm_count);
        ADPT_PRINT(" DIGI_SIFD_RX_DIAG_CNT_CPB        = [%u] \r\n", pm_count);
        adpt_sifd_pm_get(dev_id, sifd_type[index], DIGI_SIFD_RX_DIAG_CNT_ODUK, &pm_count);
        ADPT_PRINT(" DIGI_SIFD_RX_DIAG_CNT_ODUK       = [%u] \r\n", pm_count);
        adpt_sifd_pm_get(dev_id, sifd_type[index], DIGI_SIFD_CPB_DCPB_PACKET_DROP_CNT, (UINT32*)pm_buffer);
        for (i = 0; i < DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX; i++)
        {
            if (0 != pm_buffer[i])
            {
                ADPT_PRINT(" DIGI_SIFD_CPB_DCPB_PACKET_DROP_CNT[%d]  = [%u] \r\n", i, pm_buffer[i]);
            }
        }
        adpt_sifd_pm_get(dev_id, sifd_type[index], DIGI_SIFD_CPB_OCPB_PACKET_DROP_CNT, (UINT32*)pm_buffer);
        for (i = 0; i < DIGI_SIFD_ILKN_ODUK_DPI_CHNLS_MAX; i++)
        {
            if (0 != pm_buffer[i])
            {
                ADPT_PRINT(" DIGI_SIFD_CPB_OCPB_PACKET_DROP_CNT[%d]  = [%u] \r\n", i, pm_buffer[i]);
            }
        }

        if (ILKN_SIFD_1_2 == adpt_handle->dev_info->sifd_type)
        {
            ADPT_PRINT(" --------------------------------------------------------------\r\n");
        }
    }
    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_sifd_pmon */


/*******************************************************************************
* adpt_dump_loopback
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the loopback related information. 
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
PUBLIC PMC_ERROR adpt_dump_loopback(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    pmc_log_sev_t log_sev;
    BOOL enable = FALSE;
    digi_loopback_type_t loop_type;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    ADPT_PRINT("\r\n===============================================================\r\n");

    adpt_loopback_get(dev_id, chnl_id, &loop_type);
    ADPT_PRINT(" DIGI_LPBK_OUTWARD                      = [%s]\r\n", (DIGI_LPBK_OUTWARD == loop_type) ? "enable" : "disable");
    adpt_loopback_get(dev_id, chnl_id, &loop_type);
    ADPT_PRINT(" DIGI_LPBK_INWARD                       = [%s]\r\n", (DIGI_LPBK_INWARD == loop_type) ? "enable" : "disable");
    adpt_internal_loopback_get(dev_id, chnl_id, DIGI_LINEOTN_MLD_DIAGNOSTIC_LOOPBACK, &enable);
    ADPT_PRINT(" DIGI_LINEOTN_MLD_DIAGNOSTIC_LOOPBACK   = [%s]\r\n", enable ? "enable" : "disable");
    adpt_internal_loopback_get(dev_id, chnl_id, DIGI_SIFD_COPI_ILKN_CPB_INTERNAL_LOOPBACK, &enable);
    ADPT_PRINT(" DIGI_SIFD_COPI_ILKN_CPB_INTL_LOOPBACK  = [%s]\r\n", enable ? "enable" : "disable");
    adpt_internal_loopback_get(dev_id, chnl_id, DIGI_SIFD_COPI_ILKN_COREOTN_INTERNAL_LOOPBACK, &enable);
    ADPT_PRINT(" DIGI_SIFD_COPI_ILKN_COREOTN_INTL_LPBK  = [%s]\r\n", enable ? "enable" : "disable");
    adpt_internal_loopback_get(dev_id, chnl_id, DIGI_SIFD_COPI_FACILITY_LOOPBACK, &enable);
    ADPT_PRINT(" DIGI_SIFD_COPI_FACILITY_LOOPBACK       = [%s]\r\n", enable ? "enable" : "disable");
    adpt_internal_loopback_get(dev_id, chnl_id, DIGI_CBRC_DIAG_LOOPBACK, &enable);
    ADPT_PRINT(" DIGI_CBRC_DIAG_LOOPBACK                = [%s]\r\n", enable ? "enable" : "disable");
    adpt_internal_loopback_get(dev_id, chnl_id, DIGI_ENET_MAC_MLD_LOOPBACK, &enable);
    ADPT_PRINT(" DIGI_ENET_MAC_MLD_LOOPBACK             = [%s]\r\n", enable ? "enable" : "disable");
    adpt_internal_loopback_get(dev_id, chnl_id, DIGI_ENET_MAPOTN_G709_LOOPBACK, &enable);
    ADPT_PRINT(" DIGI_ENET_MAPOTN_G709_LOOPBACK         = [%s]\r\n", enable ? "enable" : "disable");
    adpt_internal_loopback_get(dev_id, chnl_id, DIGI_ENET_MAPOTN_GMP_LOOPBACK, &enable);
    ADPT_PRINT(" DIGI_ENET_MAPOTN_GMP_LOOPBACK          = [%s]\r\n", enable ? "enable" : "disable");
    adpt_ge_loopback_get(dev_id, DIGI_SERDES_INTF_OHP_0, &enable);
    ADPT_PRINT(" DIGI_SERDES_INTF_OHP_0                 = [%s]\r\n", enable ? "enable" : "disable");
    adpt_ge_loopback_get(dev_id, DIGI_SERDES_INTF_OHP_1, &enable);
    ADPT_PRINT(" DIGI_SERDES_INTF_OHP_1                 = [%s]\r\n", enable ? "enable" : "disable");
    adpt_ge_loopback_get(dev_id, DIGI_SERDES_INTF_OHP_2, &enable);
    ADPT_PRINT(" DIGI_SERDES_INTF_OHP_2                 = [%s]\r\n", enable ? "enable" : "disable");
    adpt_ge_loopback_get(dev_id, DIGI_SERDES_INTF_EMGMT, &enable);
    ADPT_PRINT(" DIGI_SERDES_INTF_EMGMT                 = [%s]\r\n", enable ? "enable" : "disable");
    adpt_ge_loopback_get(dev_id, DIGI_SERDES_INTF_RI_RCP, &enable);
    ADPT_PRINT(" DIGI_SERDES_INTF_RI_RCP                = [%s]\r\n", enable ? "enable" : "disable");
    adpt_ge_loopback_get(dev_id, DIGI_SERDES_INTF_DI_RCP, &enable);
    ADPT_PRINT(" DIGI_SERDES_INTF_DI_RCP                = [%s]\r\n", enable ? "enable" : "disable");
    adpt_ilkn_loopback_get(dev_id, DIGI_ILKN1, &enable);
    ADPT_PRINT(" DIGI_ILKN1                             = [%s]\r\n", enable ? "enable" : "disable");
    adpt_ilkn_loopback_get(dev_id, DIGI_ILKN2, &enable);
    ADPT_PRINT(" DIGI_ILKN2                             = [%s]\r\n", enable ? "enable" : "disable");

    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_loopback */


/*******************************************************************************
* adpt_dump_serdes
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the serdes related information. 
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
PUBLIC PMC_ERROR adpt_dump_serdes(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    pmc_log_sev_t log_sev;
    UINT32 traffic_loc = 0;
    UINT32 serdes_port = 0;
    UINT32 lane_index = 0;
    UINT32 *port_lane_rx = NULL;
    hostmsg_s16_intf_type_t if_type;
    serdes_s16_adapt_objects_t s16_values;
    serdes_c8_adapt_objects_t c8_values;
    serdes_t8_adapt_objects_t t8_values;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get traffic location */
    traffic_loc = DIGI_TRAFFIC_POS_GET(adpt_handle->traffic_info[index].traffic_mode);
    /* get serdes port */
    serdes_port = DIGI_SERDES_PORT_GET(chnl_id);
    if (serdes_port >= DIGI_SERDES_XFI_LANES_MAX)
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    ADPT_PRINT("\r\n===============================================================\r\n");

    if (DIGI_LINE_SIDE == traffic_loc)
    {
        port_lane_rx = adpt_handle->dev_info->line_port_lane_rx[serdes_port];
        if_type = HOSTMSG_S16_SERDES_INTF_LINE;
        ADPT_PRINT("                   S16 LINE serdes adaptation\r\n");
    }
    else
    {
        port_lane_rx = adpt_handle->dev_info->sys_port_lane_rx[serdes_port];
        if_type = HOSTMSG_S16_SERDES_INTF_SYS;
        ADPT_PRINT("                   S16 SYS serdes adaptation\r\n");
    }

    ADPT_PRINT(" lane dp_a dp_b dp_c tr_a tr_b def0 def1 def2 pga0 pga1 amp adc\r\n");
    for (lane_index = 0; lane_index < DIGI_SERDES_XFI_LANES_MAX; lane_index++)
    {
        if (DIGI_SERDES_PIN_NOT_USED != *(port_lane_rx + lane_index))
        {
            PMC_MEMSET((void*)&s16_values, 0, sizeof(s16_values));
            rc |= adpt_serdes_s16_values_get(dev_id,
                                             if_type,
                                             lane_index,
                                             &s16_values);
            ADPT_PRINT(" [%2u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u]\r\n",
                   lane_index, s16_values.dp_ffe_a, s16_values.dp_ffe_b, 
                   s16_values.dp_ffe_c, s16_values.tr_ffe_a, 
                   s16_values.tr_ffe_b, s16_values.dfe_coef0, 
                   s16_values.dfe_coef1, s16_values.dfe_coef2, 
                   s16_values.pga_gain, s16_values.pga_offset, 
                   s16_values.target_amp, s16_values.adc_map_scale);
        }                                           
    }


    if ((ILKN_SIFD_1 == adpt_handle->dev_info->sifd_type)
        || (ILKN_SIFD_1_2 == adpt_handle->dev_info->sifd_type))
    {
        /* T8 serdes adaptation information */
        if (adpt_handle->dev_info->sifd_1_cfg.lane.blk_to_E1_enable_mask & 0xFFFFF)
        {
            ADPT_PRINT("                       T8 serdes adaptation\r\n");
            ADPT_PRINT(" lane dp_a dp_b dp_c tr_a tr_b def0 def1 def2 def3 def4 def5 pga0 pga1 amp adc\r\n");
        }

        for (lane_index = 20; lane_index < 40; lane_index++)
        {
            if (0 != (adpt_handle->dev_info->sifd_1_cfg.lane.blk_to_E1_enable_mask & (1 << (39 - lane_index))))
            {
                PMC_MEMSET((void*)&t8_values, 0, sizeof(t8_values));
                rc |= adpt_serdes_t8_values_get(dev_id,
                                                lane_index,
                                                &t8_values);
                ADPT_PRINT(" [%2u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u]\r\n",
                       lane_index, t8_values.dp_ffe_a, t8_values.dp_ffe_b, 
                       t8_values.dp_ffe_c, t8_values.tr_ffe_a, 
                       t8_values.tr_ffe_b, t8_values.dfe_coef0, 
                       t8_values.dfe_coef1, t8_values.dfe_coef2, 
                       t8_values.dfe_coef3, t8_values.dfe_coef4, 
                       t8_values.dfe_coef5, t8_values.pga_gain, 
                       t8_values.pga_offset, t8_values.target_amp,
                       t8_values.adc_map_scale);
            }
        }

        /* C8 serdes adaptation information */
        if (adpt_handle->dev_info->sifd_1_cfg.lane.blk_to_N2_enable_mask & 0xFF0000)
        {
            ADPT_PRINT("                     C8 serdes adaptation\r\n");
            ADPT_PRINT(" lane dp_a amp adc\r\n");
        }

        for (lane_index = 0; lane_index < 8; lane_index++)
        {
            if (0 != (adpt_handle->dev_info->sifd_1_cfg.lane.blk_to_N2_enable_mask & (1 << (lane_index + 16))))
            {
                PMC_MEMSET((void*)&c8_values, 0, sizeof(c8_values));
                rc |= adpt_serdes_c8_values_get(dev_id,
                                                lane_index,
                                                &c8_values);
                ADPT_PRINT("  [%2u] [%u] [%u] [%u]\r\n", lane_index, c8_values.dp_ffe_a, 
                       c8_values.target_amp, c8_values.adc_map_scale);
            }
        }

        /* S16 serdes adaptation information */
        if (adpt_handle->dev_info->sifd_1_cfg.lane.blk_to_N1_enable_mask & 0xFFFF)
        {
            ADPT_PRINT("                 S16 SYS serdes adaptation\r\n");
            ADPT_PRINT(" lane dp_a dp_b dp_c tr_a tr_b def0 def1 def2 pga0 pga1 amp adc\r\n");
        }

        for (lane_index = 0; lane_index < 16; lane_index++)
        {
            if (0 != (adpt_handle->dev_info->sifd_1_cfg.lane.blk_to_N1_enable_mask & (1 << lane_index)))
            {
                PMC_MEMSET((void*)&s16_values, 0, sizeof(s16_values));
                rc |= adpt_serdes_s16_values_get(dev_id,
                                                 HOSTMSG_S16_SERDES_INTF_SYS,
                                                 lane_index,
                                                 &s16_values);
                ADPT_PRINT(" [%2u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u]\r\n",
                       lane_index, s16_values.dp_ffe_a, s16_values.dp_ffe_b, 
                       s16_values.dp_ffe_c, s16_values.tr_ffe_a, 
                       s16_values.tr_ffe_b, s16_values.dfe_coef0, 
                       s16_values.dfe_coef1, s16_values.dfe_coef2, 
                       s16_values.pga_gain, s16_values.pga_offset, 
                       s16_values.target_amp, s16_values.adc_map_scale);
            }
        }
    }

    if ((ILKN_SIFD_2 == adpt_handle->dev_info->sifd_type)
        || (ILKN_SIFD_1_2 == adpt_handle->dev_info->sifd_type))
    {
        /* T8 serdes adaptation information */
        if (adpt_handle->dev_info->sifd_2_cfg.lane.blk_to_E1_enable_mask & 0xFFFFFF)
        {
            ADPT_PRINT("                       T8 serdes adaptation\r\n");
            ADPT_PRINT(" lane dp_a dp_b dp_c tr_a tr_b def0 def1 def2 def3 def4 def5 pga0 pga1 amp adc\r\n");
        }

        for (lane_index = 0; lane_index < 24; lane_index++)
        {
            if (0 != (adpt_handle->dev_info->sifd_2_cfg.lane.blk_to_E1_enable_mask & (1 << lane_index)))
            {
                PMC_MEMSET((void*)&t8_values, 0, sizeof(t8_values));
                rc |= adpt_serdes_t8_values_get(dev_id,
                                                lane_index,
                                                &t8_values);
                ADPT_PRINT(" [%2u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u]\r\n",
                       lane_index, t8_values.dp_ffe_a, t8_values.dp_ffe_b, 
                       t8_values.dp_ffe_c, t8_values.tr_ffe_a, 
                       t8_values.tr_ffe_b, t8_values.dfe_coef0, 
                       t8_values.dfe_coef1, t8_values.dfe_coef2, 
                       t8_values.dfe_coef3, t8_values.dfe_coef4, 
                       t8_values.dfe_coef5, t8_values.pga_gain, 
                       t8_values.pga_offset, t8_values.target_amp,
                       t8_values.adc_map_scale);
            }
        }

        /* C8 serdes adaptation information */
        if (adpt_handle->dev_info->sifd_2_cfg.lane.blk_to_N2_enable_mask & 0xFF00)
        {
            ADPT_PRINT("                     C8 serdes adaptation\r\n");
            ADPT_PRINT(" lane dp_a amp adc\r\n");
        }

        for (lane_index = 0; lane_index < 8; lane_index++)
        {
            if (0 != (adpt_handle->dev_info->sifd_2_cfg.lane.blk_to_N2_enable_mask & (1 << (lane_index + 8))))
            {
                PMC_MEMSET((void*)&c8_values, 0, sizeof(c8_values));
                rc |= adpt_serdes_c8_values_get(dev_id,
                                                lane_index,
                                                &c8_values);
                ADPT_PRINT("  [%2u] [%u] [%u] [%u]\r\n", lane_index, c8_values.dp_ffe_a, 
                       c8_values.target_amp, c8_values.adc_map_scale);
            }
        }

        /* S16 serdes adaptation information */
        if (adpt_handle->dev_info->sifd_2_cfg.lane.blk_to_N1_enable_mask & 0xFF)
        {
            ADPT_PRINT("                 S16 SYS serdes adaptation\r\n");
            ADPT_PRINT(" lane dp_a dp_b dp_c tr_a tr_b def0 def1 def2 pga0 pga1 amp adc\r\n");
        }

        for (lane_index = 8; lane_index < 16; lane_index++)
        {
            if (0 != (adpt_handle->dev_info->sifd_2_cfg.lane.blk_to_N1_enable_mask & (1 << (lane_index - 8))))
            {
                PMC_MEMSET((void*)&s16_values, 0, sizeof(s16_values));
                rc |= adpt_serdes_s16_values_get(dev_id,
                                                 HOSTMSG_S16_SERDES_INTF_SYS,
                                                 lane_index,
                                                 &s16_values);
                ADPT_PRINT(" [%2u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u] [%u]\r\n",
                       lane_index, s16_values.dp_ffe_a, s16_values.dp_ffe_b, 
                       s16_values.dp_ffe_c, s16_values.tr_ffe_a, 
                       s16_values.tr_ffe_b, s16_values.dfe_coef0, 
                       s16_values.dfe_coef1, s16_values.dfe_coef2, 
                       s16_values.pga_gain, s16_values.pga_offset, 
                       s16_values.target_amp, s16_values.adc_map_scale);
            }
        }
    }

    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_serdes */


/*******************************************************************************
* adpt_dump_serdes_st
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps SerDes status information. 
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dump_serdes_st(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    serdes_adapt_status_t serdes_status;
    serdes_debug_t serdes_dbg;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    PMC_MEMSET((void*)&serdes_status, 0, sizeof(serdes_adapt_status_t));
    rc = adpt_serdes_adapt_status_get(dev_id, &serdes_status);
    ADPT_PRINT("===============================================================\r\n");
    ADPT_PRINT(" adapt_intf_type        = %s \r\n", adpt_intf_type_str(serdes_status.adapt_intf_type));
    ADPT_PRINT(" cfg_lane_msk_19_0      = 0x%x \r\n", serdes_status.cfg_lane_msk_19_0);
    ADPT_PRINT(" cfg_lane_msk_39_20     = 0x%x \r\n", serdes_status.cfg_lane_msk_39_20);
    ADPT_PRINT(" success_lane_msk_19_0  = 0x%x \r\n", serdes_status.success_lane_msk_19_0);
    ADPT_PRINT(" success_lane_msk_39_20 = 0x%x \r\n", serdes_status.success_lane_msk_39_20);
    ADPT_PRINT(" fail_lane_msk_19_0     = 0x%x \r\n", serdes_status.fail_lane_msk_19_0);
    ADPT_PRINT(" fail_lane_msk_39_20    = 0x%x \r\n", serdes_status.fail_lane_msk_39_20);
    ADPT_PRINT(" --------------------------------------------------------------\r\n");
    PMC_MEMSET((void*)&serdes_dbg, 0, sizeof(serdes_debug_t));
    rc |= adpt_serdes_debug_status_get(dev_id, TRUE, &serdes_dbg);
    ADPT_PRINT(" c8_cur_lane     = 0x%x \r\n", serdes_dbg.c8_cur_lane);
    ADPT_PRINT(" c8_cur_step     = 0x%x \r\n", serdes_dbg.c8_cur_step);
    ADPT_PRINT(" c8_config_rv    = 0x%x \r\n", serdes_dbg.c8_config_rv);
    ADPT_PRINT(" t8_cur_slice    = 0x%x \r\n", serdes_dbg.t8_cur_slice);
    ADPT_PRINT(" t8_cur_subch    = 0x%x \r\n", serdes_dbg.t8_cur_subch);
    ADPT_PRINT(" t8_cur_step     = 0x%x \r\n", serdes_dbg.t8_cur_step);
    ADPT_PRINT(" t8_config_rv    = 0x%x \r\n", serdes_dbg.t8_config_rv);
    ADPT_PRINT(" s16_cur_chnl    = 0x%x \r\n", serdes_dbg.s16_cur_chnl);
    ADPT_PRINT(" s16_cur_step    = 0x%x \r\n", serdes_dbg.s16_cur_step);
    ADPT_PRINT(" s16_config_rv   = 0x%x \r\n", serdes_dbg.s16_config_rv);
    ADPT_PRINT(" sfi51_cur_chnl  = 0x%x \r\n", serdes_dbg.sfi51_cur_chnl);
    ADPT_PRINT(" sfi51_cur_inst  = 0x%x \r\n", serdes_dbg.sfi51_cur_inst);
    ADPT_PRINT(" sfi51_cur_side  = 0x%x \r\n", serdes_dbg.sfi51_cur_side);
    ADPT_PRINT(" sfi51_config_rv = 0x%x \r\n", serdes_dbg.sfi51_config_rv);
    ADPT_PRINT(" d8_cur_step     = 0x%x \r\n", serdes_dbg.d8_cur_step);
    ADPT_PRINT(" d8_config_rv    = 0x%x \r\n", serdes_dbg.d8_config_rv);
    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_sdk */


/*******************************************************************************
* adpt_dump_sdk
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps DIGI SDK related information. 
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dump_sdk(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    pmc_log_sev_t log_sev;
    BOOL int_found = FALSE;
    digi_resource_handle_summary_t *resource_handle = NULL;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_LOW);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* DSI allocation */
    digi_debug_dsi_alloc_config_print(adpt_handle->digi_handle, (char*)"DSI Allocation");
    /* ILKN #1 */
    digi_debug_ilkn1_handle_state_all_print(adpt_handle->digi_handle);
    /* ILKN #2 */
    digi_debug_ilkn2_handle_state_all_print(adpt_handle->digi_handle);
    /* line ENET */
    digi_debug_enet_line_handle_state_all_print(adpt_handle->digi_handle);
    /* system ENET */
    digi_debug_enet_sys_handle_state_all_print(adpt_handle->digi_handle);
    /* MAPOTN */
    digi_debug_mapper_handle_state_all_print(adpt_handle->digi_handle);
    /* HO ODU */
    digi_debug_ho_odu_struct_state_all_print(adpt_handle->digi_handle);
    /* MO ODU */
    digi_debug_mo_odu_struct_state_all_print(adpt_handle->digi_handle);
    /* LO ODU */
    digi_debug_lo_odu_struct_state_all_print(adpt_handle->digi_handle);
    /* MAPOTN channel */
    digi_debug_mapotn_chnl_print(adpt_handle->digi_handle);
    /* OTN interrupt */
    digi_util_int_otn_print(adpt_handle->digi_handle,
                            &adpt_handle->pmon_data->otn_defect,
                            DIGI_UTIL_INT_PRINT_ALL,
                            &int_found);
    /* mapper interrupt */
    digi_util_int_mapper_print(adpt_handle->digi_handle,
                               &adpt_handle->pmon_data->mapper_defect,
                               DIGI_UTIL_INT_PRINT_ALL,
                               &int_found);

    /* allocate memory */
    resource_handle = (digi_resource_handle_summary_t*)PMC_CALLOC(sizeof(digi_resource_handle_summary_t));
    ADPT_PARA_VERIFY(resource_handle);

    /* restart resource */
    rc = digi_resource_state_get(adpt_handle->digi_handle, resource_handle);
    digi_resource_handle_print(adpt_handle->digi_handle, resource_handle);

    /* free memory */
    PMC_FREE(&resource_handle);

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_sdk */


/*******************************************************************************
* adpt_dump_fw
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps DIGI firmware related information. 
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dump_fw(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 value = 0;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* firmware boot information */
    adpt_dump_reg(dev_id, 0x1800060, 0x1800060);
    /* IPI bit */
    adpt_dump_reg(dev_id, 0x1801230, 0x1801230);
    /* inbound and outbound address */
    adpt_dump_reg(dev_id, 0x18001AC, 0x18001B0); 
    /* inbound message */
    value = adpt_reg_read(dev_id, 0x18001AC);
    adpt_dump_reg(dev_id, value, value + 64);
    /* outbound message */
    value = adpt_reg_read(dev_id, 0x18001B0);
    adpt_dump_reg(dev_id, value, value + 64);
    /* basic information */
    adpt_dump_basic(dev_id);

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_fw */


/*******************************************************************************
* adpt_dump_fw_log
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps DIGI firmware debug log. 
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dump_fw_log(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    digi_fw_show_debug_log(adpt_handle->digi_handle);

    PMC_RETURN(rc);
} /* adpt_dump_fw_log */


/*******************************************************************************
* adpt_dump_pmon_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps DIGI PMON related configuration information. 
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dump_pmon_cfg(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_pmon_fw_sm_dma_status_t pmon_cfg;
    UINT64 phy_addr = 0;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* PMON lower 32-bits physical address */
    adpt_dump_reg(dev_id, 0x18090A0, 0x18090A0);
    /* PMON upper 32-bits physical address */
    adpt_dump_reg(dev_id, 0x18090B0, 0x18090B0);
    ADPT_PRINT("===============================================================\r\n");
    ADPT_PRINT("pmon virtual address   = 0x%lx \r\n", adpt_handle->pmon_buf_addr);
    phy_addr = pmc_convert_mem_pool_addr_to_physical_addr(adpt_handle->pmon_pool, 
                                                          (void*)adpt_handle->pmon_buf_addr);
    ADPT_PRINT("pmon physical address  = 0x%llx \r\n", phy_addr);
    /* PMON configuration */
    PMC_MEMSET((void*)&pmon_cfg, 0, sizeof(digi_pmon_fw_sm_dma_status_t));
    rc = digi_pmon_fw_sm_dma_status_get(adpt_handle->digi_handle,
                                        &pmon_cfg);
    ADPT_RETURN_VERIFY(rc);

    ADPT_PRINT("pmon_trig_mode         = [%d] \r\n", pmon_cfg.pmon_trig_mode);
    ADPT_PRINT("pmon_sync_en           = [%d] \r\n", pmon_cfg.pmon_sync_en);
    ADPT_PRINT("trig_period            = [%d] \r\n", pmon_cfg.trig_period);
    ADPT_PRINT("pmon_xfer_enable       = [0x%x] \r\n", pmon_cfg.pmon_xfer_enable);
    ADPT_PRINT("rcp_enable             = [%d] \r\n", pmon_cfg.rcp_enable);
    ADPT_PRINT("ddeg_enable            = [%d] \r\n", pmon_cfg.ddeg_enable);
    ADPT_PRINT("hw_ddeg_enable         = [%d] \r\n", pmon_cfg.hw_ddeg_enable);
    ADPT_PRINT("lclk_ddeg_cnt          = [%d] \r\n", pmon_cfg.lclk_ddeg_cnt);
    ADPT_PRINT("lclk_ddeg_overrun      = [%d] \r\n", pmon_cfg.lclk_ddeg_intr_overrun);
    ADPT_PRINT("lclk_rcp_cnt           = [%d] \r\n", pmon_cfg.lclk_rcp_cnt);
    ADPT_PRINT("lclk_dma_xfer_cnt      = [%d] \r\n", pmon_cfg.lclk_dma_xfer_cnt);
    ADPT_PRINT("line_fec_intr_cnt      = [%d] \r\n", pmon_cfg.lineotn_fec_intr_cnt);
    ADPT_PRINT("lineotn_intr_overrun   = [%d] \r\n", pmon_cfg.lineotn_intr_overrun);
    ADPT_PRINT("lclk_otn_intr_overrun  = [%d] \r\n", pmon_cfg.lclk_otn_intr_overrun);
    ADPT_PRINT("lclk_misc_intr_overrun = [%d] \r\n", pmon_cfg.lclk_misc_intr_overrun);
    ADPT_PRINT("lclk_otn_buf_conflict  = [%d] \r\n", pmon_cfg.lclk_otn_buf_conflict);
    ADPT_PRINT("lclk_misc_buf_conflict = [%d] \r\n", pmon_cfg.lclk_misc_buf_conflict);
    ADPT_PRINT("gdma_busy_num          = [%d] \r\n", pmon_cfg.gdma_busy_num);
    ADPT_PRINT("rcp_dma_err_num        = [%d] \r\n", pmon_cfg.rcp_dma_err_num);
    ADPT_PRINT("line_fec_dma_complete  = [%d] \r\n", pmon_cfg.lineotn_fec_dma_complete);
    ADPT_PRINT("otn_dma_complete       = [%d] \r\n", pmon_cfg.otn_dma_complete);
    ADPT_PRINT("misc_dma_complete      = [%d] \r\n", pmon_cfg.misc_dma_complete);
    ADPT_PRINT("rcp_dma_complete       = [%d] \r\n", pmon_cfg.rcp_dma_complete);
    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
} /* adpt_dump_pmon_cfg */


/*******************************************************************************
* adpt_dump_pmon_data
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps DIGI PMON related data structure information. 
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dump_pmon_data(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_enet_int_chnl_t *enet_chnl = NULL;
    digi_mapper_int_chnl_t *mapper_chnl = NULL;
    digi_otn_odu_int_chnl_t *odu_chnl = NULL;
    lineotn_int_chnl_t *otu_chnl = NULL;
    coreotn_int_t *coreotn_int = NULL;
    UINT32 index = 0;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* defect trigger */
    rc = adpt_defect_trigger(dev_id, DIGI_BLK_FULL);
    ADPT_RETURN_VERIFY(rc);

    /* pmon_data check */
    ADPT_PARA_VERIFY(adpt_handle->pmon_data);

    ADPT_PRINT("\r\n===============================================================\r\n");
    /* line ENET interrupt */
    ADPT_PRINT(" --------------------------line ENET---------------------------\r\n");
    for (index = 0; index < DIGI_ENET_CHNL_MAX; index++)
    {
        if (NULL == adpt_handle->pmon_data->line_enet_defect.handle[index])
        {
            continue;
        }
        ADPT_PRINT(" --------------------------index[%d]---------------------------\r\n", index);
        enet_chnl = &adpt_handle->pmon_data->line_enet_defect.enet_chnl[index];

        /* EMAC defect and interrupt */
        ADPT_PRINT("phy_los_i           = [%d] \r\n", enet_chnl->enet.emac.phy_los_i);
        ADPT_PRINT("rx_loc_fault_i      = [%d] \r\n", enet_chnl->enet.emac.rx_loc_fault_i);
        ADPT_PRINT("rx_rem_fault_i      = [%d] \r\n", enet_chnl->enet.emac.rx_rem_fault_i);
        ADPT_PRINT("hi_ber_i            = [%d] \r\n", enet_chnl->enet.emac.hi_ber_i);
        ADPT_PRINT("block_lock_i        = [%d] \r\n", enet_chnl->enet.emac.block_lock_i);
        ADPT_PRINT("align_status_i      = [%d] \r\n", enet_chnl->enet.emac.align_status_i);
        ADPT_PRINT("emac_tx_overflow_i  = [%d] \r\n", enet_chnl->enet.emac.emac_tx_overflow_i);
        ADPT_PRINT("emac_tx_underflow_i = [%d] \r\n", enet_chnl->enet.emac.emac_tx_underflow_i);
        ADPT_PRINT("phy_los_v           = [%d] \r\n", enet_chnl->enet.emac.phy_los_v);
        ADPT_PRINT("rx_loc_fault_v      = [%d] \r\n", enet_chnl->enet.emac.rx_loc_fault_v);
        ADPT_PRINT("rx_rem_fault_v      = [%d] \r\n", enet_chnl->enet.emac.rx_rem_fault_v);
        ADPT_PRINT("block_lock_v        = [%d] \r\n", enet_chnl->enet.emac.block_lock_v);
        ADPT_PRINT("align_status_v      = [%d] \r\n", enet_chnl->enet.emac.align_status_v);
        ADPT_PRINT("link_status         = [%d] \r\n", enet_chnl->enet.emac.link_status);
        /* clear interrupt */
        enet_chnl->enet.emac.phy_los_i           = FALSE;
        enet_chnl->enet.emac.rx_loc_fault_i      = FALSE;
        enet_chnl->enet.emac.rx_rem_fault_i      = FALSE;
        enet_chnl->enet.emac.hi_ber_i            = FALSE;
        enet_chnl->enet.emac.block_lock_i        = FALSE;
        enet_chnl->enet.emac.align_status_i      = FALSE;
        enet_chnl->enet.emac.emac_tx_overflow_i  = FALSE;
        enet_chnl->enet.emac.emac_tx_underflow_i = FALSE;

        /* CPB interrupt */
        ADPT_PRINT("ch_drop_ovrsize_i   = [%d] \r\n", enet_chnl->cpb.ch_drop_ovrsize_i);
        ADPT_PRINT("ch_drop_client_i    = [%d] \r\n", enet_chnl->cpb.ch_drop_client_i);
        ADPT_PRINT("ch_drop_full_i      = [%d] \r\n", enet_chnl->cpb.ch_drop_full_i);
        /* clear interrupt */
        enet_chnl->cpb.ch_drop_ovrsize_i = FALSE;
        enet_chnl->cpb.ch_drop_client_i  = FALSE;
        enet_chnl->cpb.ch_drop_full_i    = FALSE;
    }

    /* system ENET interrupt */
    ADPT_PRINT(" -------------------------system ENET--------------------------\r\n");
    for (index = 0; index < DIGI_ENET_CHNL_MAX; index++)
    {
        if (NULL == adpt_handle->pmon_data->sys_enet_defect.handle[index])
        {
            continue;
        }
        ADPT_PRINT(" --------------------------index[%d]---------------------------\r\n", index);
        enet_chnl = &adpt_handle->pmon_data->sys_enet_defect.enet_chnl[index];

        /* EMAC defect and interrupt */
        ADPT_PRINT("phy_los_i           = [%d] \r\n", enet_chnl->enet.emac.phy_los_i);
        ADPT_PRINT("rx_loc_fault_i      = [%d] \r\n", enet_chnl->enet.emac.rx_loc_fault_i);
        ADPT_PRINT("rx_rem_fault_i      = [%d] \r\n", enet_chnl->enet.emac.rx_rem_fault_i);
        ADPT_PRINT("hi_ber_i            = [%d] \r\n", enet_chnl->enet.emac.hi_ber_i);
        ADPT_PRINT("block_lock_i        = [%d] \r\n", enet_chnl->enet.emac.block_lock_i);
        ADPT_PRINT("align_status_i      = [%d] \r\n", enet_chnl->enet.emac.align_status_i);
        ADPT_PRINT("emac_tx_overflow_i  = [%d] \r\n", enet_chnl->enet.emac.emac_tx_overflow_i);
        ADPT_PRINT("emac_tx_underflow_i = [%d] \r\n", enet_chnl->enet.emac.emac_tx_underflow_i);
        ADPT_PRINT("phy_los_v           = [%d] \r\n", enet_chnl->enet.emac.phy_los_v);
        ADPT_PRINT("rx_loc_fault_v      = [%d] \r\n", enet_chnl->enet.emac.rx_loc_fault_v);
        ADPT_PRINT("rx_rem_fault_v      = [%d] \r\n", enet_chnl->enet.emac.rx_rem_fault_v);
        ADPT_PRINT("block_lock_v        = [%d] \r\n", enet_chnl->enet.emac.block_lock_v);
        ADPT_PRINT("align_status_v      = [%d] \r\n", enet_chnl->enet.emac.align_status_v);
        ADPT_PRINT("link_status         = [%d] \r\n", enet_chnl->enet.emac.link_status);
        /* clear interrupt */
        enet_chnl->enet.emac.phy_los_i           = FALSE;
        enet_chnl->enet.emac.rx_loc_fault_i      = FALSE;
        enet_chnl->enet.emac.rx_rem_fault_i      = FALSE;
        enet_chnl->enet.emac.hi_ber_i            = FALSE;
        enet_chnl->enet.emac.block_lock_i        = FALSE;
        enet_chnl->enet.emac.align_status_i      = FALSE;
        enet_chnl->enet.emac.emac_tx_overflow_i  = FALSE;
        enet_chnl->enet.emac.emac_tx_underflow_i = FALSE;

        /* CPB interrupt */
        ADPT_PRINT("ch_drop_ovrsize_i   = [%d] \r\n", enet_chnl->cpb.ch_drop_ovrsize_i);
        ADPT_PRINT("ch_drop_client_i    = [%d] \r\n", enet_chnl->cpb.ch_drop_client_i);
        ADPT_PRINT("ch_drop_full_i      = [%d] \r\n", enet_chnl->cpb.ch_drop_full_i);
        /* clear interrupt */
        enet_chnl->cpb.ch_drop_ovrsize_i = FALSE;
        enet_chnl->cpb.ch_drop_client_i  = FALSE;
        enet_chnl->cpb.ch_drop_full_i    = FALSE;
    }

    /* MAPOTN defect and interrupt */
    ADPT_PRINT(" --------------------------MAPOTN------------------------------\r\n");
    for (index = 0; index < DIGI_MAPOTN_CHNL_MAX; index++)
    {
        if (NULL == adpt_handle->pmon_data->mapper_defect.handle[index])
        {
            continue;
        }
        ADPT_PRINT(" --------------------------index[%d]---------------------------\r\n", index);
        mapper_chnl = &adpt_handle->pmon_data->mapper_defect.mapper_chnl[index];

        if (mapper_chnl->status.rgmp_valid)
        {
            /* GMP defect and interrupt */
            ADPT_PRINT("dplm_i              = [%d] \r\n", mapper_chnl->status.rgmp.dplm_i);
            ADPT_PRINT("csf_i               = [%d] \r\n", mapper_chnl->status.rgmp.csf_i);
            ADPT_PRINT("cm_out_limit_i      = [%d] \r\n", mapper_chnl->status.rgmp.cm_out_limit_i);
            ADPT_PRINT("cnd_max_i           = [%d] \r\n", mapper_chnl->status.rgmp.cnd_max_i);
            ADPT_PRINT("jc_err_syncloss_i   = [%d] \r\n", mapper_chnl->status.rgmp.jc_err_syncloss_i);
            ADPT_PRINT("cm_cnd_crc_err_i    = [%d] \r\n", mapper_chnl->status.rgmp.cm_cnd_crc_err_i);
            ADPT_PRINT("pserv_early_i       = [%d] \r\n", mapper_chnl->status.rgmp.pserv_early_i);
            ADPT_PRINT("pserv_late_i        = [%d] \r\n", mapper_chnl->status.rgmp.pserv_late_i);
            ADPT_PRINT("ssf_sf_rcvd_i       = [%d] \r\n", mapper_chnl->status.rgmp.ssf_sf_rcvd_i);
            ADPT_PRINT("dplm_v              = [%d] \r\n", mapper_chnl->status.rgmp.dplm_v);
            ADPT_PRINT("csf_v               = [%d] \r\n", mapper_chnl->status.rgmp.csf_v);
            ADPT_PRINT("ssf_sf_rcvd_v       = [%d] \r\n", mapper_chnl->status.rgmp.ssf_sf_rcvd_v);
            mapper_chnl->status.rgmp.dplm_i            = FALSE;
            mapper_chnl->status.rgmp.csf_i             = FALSE;
            mapper_chnl->status.rgmp.cm_out_limit_i    = FALSE;
            mapper_chnl->status.rgmp.cnd_max_i         = FALSE;
            mapper_chnl->status.rgmp.jc_err_syncloss_i = FALSE;
            mapper_chnl->status.rgmp.cm_cnd_crc_err_i  = FALSE;
            mapper_chnl->status.rgmp.pserv_early_i     = FALSE;
            mapper_chnl->status.rgmp.pserv_late_i      = FALSE;
            mapper_chnl->status.rgmp.ssf_sf_rcvd_i     = FALSE;
        }

        /* GMP defect and interrupt */
        ADPT_PRINT("lofd_i              = [%d] \r\n", mapper_chnl->status.lofd_i);
        ADPT_PRINT("idle_i              = [%d] \r\n", mapper_chnl->status.idle_i);
        ADPT_PRINT("ctrlfrm_i           = [%d] \r\n", mapper_chnl->status.ctrlfrm_i);
        ADPT_PRINT("unpack_fifo_drop_i  = [%d] \r\n", mapper_chnl->status.unpack_fifo_drop_i);
        ADPT_PRINT("uchec_i             = [%d] \r\n", mapper_chnl->status.uchec_i);
        ADPT_PRINT("cchec_i             = [%d] \r\n", mapper_chnl->status.cchec_i);
        ADPT_PRINT("uthec_i             = [%d] \r\n", mapper_chnl->status.uthec_i);
        ADPT_PRINT("cthec_i             = [%d] \r\n", mapper_chnl->status.cthec_i);
        ADPT_PRINT("uehec_i             = [%d] \r\n", mapper_chnl->status.uehec_i);
        ADPT_PRINT("cehec_i             = [%d] \r\n", mapper_chnl->status.cehec_i);
        ADPT_PRINT("exi_i               = [%d] \r\n", mapper_chnl->status.exi_i);
        ADPT_PRINT("pti_upi_msmch_i     = [%d] \r\n", mapper_chnl->status.pti_upi_msmch_i);
        ADPT_PRINT("generic_i           = [%d] \r\n", mapper_chnl->status.generic_i);
        ADPT_PRINT("csf_i               = [%d] \r\n", mapper_chnl->status.csf_i);
        ADPT_PRINT("drop_i              = [%d] \r\n", mapper_chnl->status.drop_i);
        ADPT_PRINT("datafrm_i           = [%d] \r\n", mapper_chnl->status.datafrm_i);
        ADPT_PRINT("pause_i             = [%d] \r\n", mapper_chnl->status.pause_i);
        ADPT_PRINT("max_frm_len_i       = [%d] \r\n", mapper_chnl->status.max_frm_len_i);
        ADPT_PRINT("min_frm_len_i       = [%d] \r\n", mapper_chnl->status.min_frm_len_i);
        ADPT_PRINT("frm_len_stat_i_0    = [%d] \r\n", mapper_chnl->status.frm_len_stat_intr_i[0]);
        ADPT_PRINT("frm_len_stat_i_1    = [%d] \r\n", mapper_chnl->status.frm_len_stat_intr_i[1]);
        ADPT_PRINT("frm_len_stat_i_2    = [%d] \r\n", mapper_chnl->status.frm_len_stat_intr_i[2]);
        ADPT_PRINT("frm_len_stat_i_3    = [%d] \r\n", mapper_chnl->status.frm_len_stat_intr_i[3]);
        ADPT_PRINT("frm_len_stat_i_4    = [%d] \r\n", mapper_chnl->status.frm_len_stat_intr_i[4]);
        ADPT_PRINT("frm_len_stat_i_5    = [%d] \r\n", mapper_chnl->status.frm_len_stat_intr_i[5]);
        ADPT_PRINT("frm_len_stat_i_6    = [%d] \r\n", mapper_chnl->status.frm_len_stat_intr_i[6]);
        ADPT_PRINT("pfcse_i             = [%d] \r\n", mapper_chnl->status.pfcse_i);
        ADPT_PRINT("dwnstrm_frm_i       = [%d] \r\n", mapper_chnl->status.dwnstrm_frm_i);
        ADPT_PRINT("ssf_sf_rcvd_i       = [%d] \r\n", mapper_chnl->status.ssf_sf_rcvd_i);
        ADPT_PRINT("hao_status_avl_i    = [%d] \r\n", mapper_chnl->status.hao_status_avl_i);
        ADPT_PRINT("dplm_i              = [%d] \r\n", mapper_chnl->status.dplm_i);
        ADPT_PRINT("opu_csf_i           = [%d] \r\n", mapper_chnl->status.opu_csf_i);
        ADPT_PRINT("len_err_int_i       = [%d] \r\n", mapper_chnl->status.len_err_int_i);
        ADPT_PRINT("pause_frm_tx_int_i  = [%d] \r\n", mapper_chnl->status.pause_frame_tx_int_i);
        ADPT_PRINT("mgmt_frm_tx_int_i   = [%d] \r\n", mapper_chnl->status.mgmt_frame_tx_int_i);
        ADPT_PRINT("ovrsz_frm_rx_int_i  = [%d] \r\n", mapper_chnl->status.oversize_frm_rx_int_i);
        ADPT_PRINT("undrsz_frm_rx_int_i = [%d] \r\n", mapper_chnl->status.undersize_frm_rx_int_i);
        ADPT_PRINT("ord_set_ln_er_int_i = [%d] \r\n", mapper_chnl->status.ord_set_len_err_int_i);
        ADPT_PRINT("lofd_v              = [%d] \r\n", mapper_chnl->status.lofd_v);
        ADPT_PRINT("idle_v              = [%d] \r\n", mapper_chnl->status.idle_v);
        ADPT_PRINT("csf_v               = [%d] \r\n", mapper_chnl->status.csf_v);
        ADPT_PRINT("exi_v               = [%d] \r\n", mapper_chnl->status.exi_v);
        ADPT_PRINT("pti_upi_msmch_v     = [%d] \r\n", mapper_chnl->status.pti_upi_msmch_v);
        /* clear interrupt */
        mapper_chnl->status.lofd_i                 = FALSE;
        mapper_chnl->status.idle_i                 = FALSE;
        mapper_chnl->status.ctrlfrm_i              = FALSE;
        mapper_chnl->status.unpack_fifo_drop_i     = FALSE;
        mapper_chnl->status.uchec_i                = FALSE;
        mapper_chnl->status.cchec_i                = FALSE;
        mapper_chnl->status.uthec_i                = FALSE;
        mapper_chnl->status.cthec_i                = FALSE;
        mapper_chnl->status.uehec_i                = FALSE;
        mapper_chnl->status.cehec_i                = FALSE;
        mapper_chnl->status.exi_i                  = FALSE;
        mapper_chnl->status.pti_upi_msmch_i        = FALSE;
        mapper_chnl->status.generic_i              = FALSE;
        mapper_chnl->status.csf_i                  = FALSE;
        mapper_chnl->status.drop_i                 = FALSE;
        mapper_chnl->status.datafrm_i              = FALSE;
        mapper_chnl->status.pause_i                = FALSE;
        mapper_chnl->status.max_frm_len_i          = FALSE;
        mapper_chnl->status.min_frm_len_i          = FALSE;
        mapper_chnl->status.frm_len_stat_intr_i[0] = FALSE;
        mapper_chnl->status.frm_len_stat_intr_i[1] = FALSE;
        mapper_chnl->status.frm_len_stat_intr_i[2] = FALSE;
        mapper_chnl->status.frm_len_stat_intr_i[3] = FALSE;
        mapper_chnl->status.frm_len_stat_intr_i[4] = FALSE;
        mapper_chnl->status.frm_len_stat_intr_i[5] = FALSE;
        mapper_chnl->status.frm_len_stat_intr_i[6] = FALSE;
        mapper_chnl->status.pfcse_i                = FALSE;
        mapper_chnl->status.dwnstrm_frm_i          = FALSE;
        mapper_chnl->status.ssf_sf_rcvd_i          = FALSE;
        mapper_chnl->status.hao_status_avl_i       = FALSE;
        mapper_chnl->status.dplm_i                 = FALSE;
        mapper_chnl->status.opu_csf_i              = FALSE;
        mapper_chnl->status.len_err_int_i          = FALSE;
        mapper_chnl->status.pause_frame_tx_int_i   = FALSE;
        mapper_chnl->status.mgmt_frame_tx_int_i    = FALSE;
        mapper_chnl->status.oversize_frm_rx_int_i  = FALSE;
        mapper_chnl->status.undersize_frm_rx_int_i = FALSE;
        mapper_chnl->status.ord_set_len_err_int_i  = FALSE;

        /* ODUk defect and interrupt */
        adpt_odu_status_print(&mapper_chnl->fo2.status);

        /* CPB interrupt */
        ADPT_PRINT("ch_drop_ovrsize_i   = [%d] \r\n", mapper_chnl->cpb.ch_drop_ovrsize_i);
        ADPT_PRINT("ch_drop_client_i    = [%d] \r\n", mapper_chnl->cpb.ch_drop_client_i);
        ADPT_PRINT("ch_drop_full_i      = [%d] \r\n", mapper_chnl->cpb.ch_drop_full_i);
        /* clear interrupt */
        mapper_chnl->cpb.ch_drop_ovrsize_i = FALSE;
        mapper_chnl->cpb.ch_drop_client_i  = FALSE;
        mapper_chnl->cpb.ch_drop_full_i    = FALSE;
    }

    /* LINEOTN defect and interrupt */
    ADPT_PRINT(" --------------------------LINEOTN-----------------------------\r\n");
    for (index = 0; index < DIGI_OTN_SERVER_CHNL_MAX; index++)
    {
        if (NULL == adpt_handle->pmon_data->otn_defect.otn_server_handle[index])
        {
            continue;
        }
        ADPT_PRINT(" --------------------------index[%d]---------------------------\r\n", index);
        otu_chnl = &adpt_handle->pmon_data->otn_defect.otn_server_chnl[index];

        /* OTUk defect and interrupt */
        adpt_otu_status_print(otu_chnl);
    }

    /* SYSOTN defect and interrupt */
    ADPT_PRINT(" ---------------------------SYSOTN-----------------------------\r\n");
    for (index = 0; index < DIGI_OTN_SERVER_SYS_CHNL_MAX; index++)
    {
        if (NULL == adpt_handle->pmon_data->otn_defect.sysotn_server_handle[index])
        {
            continue;
        }
        ADPT_PRINT(" --------------------------index[%d]---------------------------\r\n", index);
        otu_chnl = &adpt_handle->pmon_data->otn_defect.sysotn_server_chnl[index];

        /* OTUk defect and interrupt */
        adpt_otu_status_print(otu_chnl);
    }

    /* COREOTN defect and interrupt */
    ADPT_PRINT(" --------------------------COREOTN-----------------------------\r\n");
    coreotn_int = &adpt_handle->pmon_data->otn_defect.coreotn;
    if (coreotn_int->fmf1_dmx_valid)
    {
        ADPT_PRINT("dloomfi_i           = [%d] \r\n", coreotn_int->fmf1_dmx.dloomfi_i);
        ADPT_PRINT("dloomfi_v           = [%d] \r\n", coreotn_int->fmf1_dmx.dloomfi_v);
        /* clear interrupt */
        coreotn_int->fmf1_dmx.dloomfi_i = FALSE;
    }

    if (coreotn_int->fmf2_dmx_valid)
    {
        ADPT_PRINT("dloomfi_i           = [%d] \r\n", coreotn_int->fmf2_dmx.dloomfi_i);
        ADPT_PRINT("dloomfi_v           = [%d] \r\n", coreotn_int->fmf2_dmx.dloomfi_v);
        /* clear interrupt */
        coreotn_int->fmf2_dmx.dloomfi_i = FALSE;
    }

    if (coreotn_int->fmf1_oduksc_valid)
    {
        ADPT_PRINT("acc_undf_i          = [%d] \r\n", coreotn_int->fmf1_oduksc.acc_undf_i);
        ADPT_PRINT("acc_ovf_i           = [%d] \r\n", coreotn_int->fmf1_oduksc.acc_ovf_i);
        /* clear interrupt */
        coreotn_int->fmf1_oduksc.acc_undf_i = FALSE;
        coreotn_int->fmf1_oduksc.acc_ovf_i = FALSE;
    }

    if (coreotn_int->fmf2_oduksc_valid)
    {
        ADPT_PRINT("acc_undf_i          = [%d] \r\n", coreotn_int->fmf2_oduksc.acc_undf_i);
        ADPT_PRINT("acc_ovf_i           = [%d] \r\n", coreotn_int->fmf2_oduksc.acc_ovf_i);
        /* clear interrupt */
        coreotn_int->fmf2_oduksc.acc_undf_i = FALSE;
        coreotn_int->fmf2_oduksc.acc_ovf_i = FALSE;
    }

    if (coreotn_int->fmf1_odujat_valid)
    {
        ADPT_PRINT("gain_ovf_i          = [%d] \r\n", coreotn_int->fmf1_odujat.gain_ovf_i);
        ADPT_PRINT("gain_undf_i         = [%d] \r\n", coreotn_int->fmf1_odujat.gain_undf_i);
        ADPT_PRINT("dq_limit_i          = [%d] \r\n", coreotn_int->fmf1_odujat.dq_limit_i);
        /* clear interrupt */
        coreotn_int->fmf1_odujat.gain_ovf_i = FALSE;
        coreotn_int->fmf1_odujat.gain_undf_i = FALSE;
        coreotn_int->fmf1_odujat.dq_limit_i = FALSE;
    }

    if (coreotn_int->fmf2_odujat_valid)
    {
        ADPT_PRINT("gain_ovf_i          = [%d] \r\n", coreotn_int->fmf2_odujat.gain_ovf_i);
        ADPT_PRINT("gain_undf_i         = [%d] \r\n", coreotn_int->fmf2_odujat.gain_undf_i);
        ADPT_PRINT("dq_limit_i          = [%d] \r\n", coreotn_int->fmf2_odujat.dq_limit_i);
        /* clear interrupt */
        coreotn_int->fmf2_odujat.gain_ovf_i = FALSE;
        coreotn_int->fmf2_odujat.gain_undf_i = FALSE;
        coreotn_int->fmf2_odujat.dq_limit_i = FALSE;
    }

    if ((coreotn_int->framer_stg1.tfrm_valid) || (coreotn_int->framer_stg1.rfrm_valid))
    {
        ADPT_PRINT("stg1.tfrm.dm_udr_i  = [%d] \r\n", coreotn_int->framer_stg1.tfrm.dm_fifo_udr_i);
        ADPT_PRINT("stg1.tfrm.dm_ovr_i  = [%d] \r\n", coreotn_int->framer_stg1.tfrm.dm_fifo_ovr_i);
        ADPT_PRINT("stg1.rfrm.c_dlck_i  = [%d] \r\n", coreotn_int->framer_stg1.rfrm.comb_dlck_i);
        ADPT_PRINT("stg1.rfrm.c_doci_i  = [%d] \r\n", coreotn_int->framer_stg1.rfrm.comb_doci_i);
        ADPT_PRINT("stg1.rfrm.c_dais_i  = [%d] \r\n", coreotn_int->framer_stg1.rfrm.comb_dais_i);
        ADPT_PRINT("stg1.rfrm.c_dtim_i  = [%d] \r\n", coreotn_int->framer_stg1.rfrm.comb_dtim_i);
        ADPT_PRINT("stg1.rfrm.c_ddeg_i  = [%d] \r\n", coreotn_int->framer_stg1.rfrm.comb_ddeg_i);
        ADPT_PRINT("stg1.rfrm.c_dbdi_i  = [%d] \r\n", coreotn_int->framer_stg1.rfrm.comb_dbdi_i);
        ADPT_PRINT("stg1.rfrm.c_dltc_i  = [%d] \r\n", coreotn_int->framer_stg1.rfrm.comb_dltc_i);
        ADPT_PRINT("stg1.rfrm.c_diae_i  = [%d] \r\n", coreotn_int->framer_stg1.rfrm.comb_diae_i);
        ADPT_PRINT("stg1.rfrm.c_dbiae_i = [%d] \r\n", coreotn_int->framer_stg1.rfrm.comb_dbiae_i);
        ADPT_PRINT("stg1.rfrm.c_oof_i   = [%d] \r\n", coreotn_int->framer_stg1.rfrm.comb_oof_i);
        ADPT_PRINT("stg1.rfrm.c_atsf_i  = [%d] \r\n", coreotn_int->framer_stg1.rfrm.comb_atsf_i);
        ADPT_PRINT("stg1.rfrm.c_atsd_i  = [%d] \r\n", coreotn_int->framer_stg1.rfrm.comb_atsd_i);
        ADPT_PRINT("stg1.rfrm.c_atsd_i  = [%d] \r\n", coreotn_int->framer_stg1.rfrm.comb_aais_i);
        ADPT_PRINT("stg1.rfrm.c_abdi_i  = [%d] \r\n", coreotn_int->framer_stg1.rfrm.comb_abdi_i);
        ADPT_PRINT("stg1.rfrm.dloflom_i = [%d] \r\n", coreotn_int->framer_stg1.rfrm.comb_dloflom_i);
        /* clear interrupt */
        coreotn_int->framer_stg1.tfrm.dm_fifo_udr_i  = FALSE;
        coreotn_int->framer_stg1.tfrm.dm_fifo_ovr_i  = FALSE;
        coreotn_int->framer_stg1.rfrm.comb_dlck_i    = FALSE;
        coreotn_int->framer_stg1.rfrm.comb_doci_i    = FALSE;
        coreotn_int->framer_stg1.rfrm.comb_dais_i    = FALSE;
        coreotn_int->framer_stg1.rfrm.comb_dtim_i    = FALSE;
        coreotn_int->framer_stg1.rfrm.comb_ddeg_i    = FALSE;
        coreotn_int->framer_stg1.rfrm.comb_dbdi_i    = FALSE;
        coreotn_int->framer_stg1.rfrm.comb_dltc_i    = FALSE;
        coreotn_int->framer_stg1.rfrm.comb_diae_i    = FALSE;
        coreotn_int->framer_stg1.rfrm.comb_dbiae_i   = FALSE;
        coreotn_int->framer_stg1.rfrm.comb_oof_i     = FALSE;
        coreotn_int->framer_stg1.rfrm.comb_atsf_i    = FALSE;
        coreotn_int->framer_stg1.rfrm.comb_atsd_i    = FALSE;
        coreotn_int->framer_stg1.rfrm.comb_aais_i    = FALSE;
        coreotn_int->framer_stg1.rfrm.comb_abdi_i    = FALSE;
        coreotn_int->framer_stg1.rfrm.comb_dloflom_i = FALSE;
    }

    if ((coreotn_int->framer_stg2.tfrm_valid) || (coreotn_int->framer_stg2.rfrm_valid))
    {
        ADPT_PRINT("stg2.tfrm.dm_udr_i  = [%d] \r\n", coreotn_int->framer_stg2.tfrm.dm_fifo_udr_i);
        ADPT_PRINT("stg2.tfrm.dm_ovr_i  = [%d] \r\n", coreotn_int->framer_stg2.tfrm.dm_fifo_ovr_i);
        ADPT_PRINT("stg2.rfrm.c_dlck_i  = [%d] \r\n", coreotn_int->framer_stg2.rfrm.comb_dlck_i);
        ADPT_PRINT("stg2.rfrm.c_doci_i  = [%d] \r\n", coreotn_int->framer_stg2.rfrm.comb_doci_i);
        ADPT_PRINT("stg2.rfrm.c_dais_i  = [%d] \r\n", coreotn_int->framer_stg2.rfrm.comb_dais_i);
        ADPT_PRINT("stg2.rfrm.c_dtim_i  = [%d] \r\n", coreotn_int->framer_stg2.rfrm.comb_dtim_i);
        ADPT_PRINT("stg2.rfrm.c_ddeg_i  = [%d] \r\n", coreotn_int->framer_stg2.rfrm.comb_ddeg_i);
        ADPT_PRINT("stg2.rfrm.c_dbdi_i  = [%d] \r\n", coreotn_int->framer_stg2.rfrm.comb_dbdi_i);
        ADPT_PRINT("stg2.rfrm.c_dltc_i  = [%d] \r\n", coreotn_int->framer_stg2.rfrm.comb_dltc_i);
        ADPT_PRINT("stg2.rfrm.c_diae_i  = [%d] \r\n", coreotn_int->framer_stg2.rfrm.comb_diae_i);
        ADPT_PRINT("stg2.rfrm.c_dbiae_i = [%d] \r\n", coreotn_int->framer_stg2.rfrm.comb_dbiae_i);
        ADPT_PRINT("stg2.rfrm.c_oof_i   = [%d] \r\n", coreotn_int->framer_stg2.rfrm.comb_oof_i);
        ADPT_PRINT("stg2.rfrm.c_atsf_i  = [%d] \r\n", coreotn_int->framer_stg2.rfrm.comb_atsf_i);
        ADPT_PRINT("stg2.rfrm.c_atsd_i  = [%d] \r\n", coreotn_int->framer_stg2.rfrm.comb_atsd_i);
        ADPT_PRINT("stg2.rfrm.c_atsd_i  = [%d] \r\n", coreotn_int->framer_stg2.rfrm.comb_aais_i);
        ADPT_PRINT("stg2.rfrm.c_abdi_i  = [%d] \r\n", coreotn_int->framer_stg2.rfrm.comb_abdi_i);
        ADPT_PRINT("stg2.rfrm.dloflom_i = [%d] \r\n", coreotn_int->framer_stg2.rfrm.comb_dloflom_i);
        /* clear interrupt */
        coreotn_int->framer_stg2.tfrm.dm_fifo_udr_i  = FALSE;
        coreotn_int->framer_stg2.tfrm.dm_fifo_ovr_i  = FALSE;
        coreotn_int->framer_stg2.rfrm.comb_dlck_i    = FALSE;
        coreotn_int->framer_stg2.rfrm.comb_doci_i    = FALSE;
        coreotn_int->framer_stg2.rfrm.comb_dais_i    = FALSE;
        coreotn_int->framer_stg2.rfrm.comb_dtim_i    = FALSE;
        coreotn_int->framer_stg2.rfrm.comb_ddeg_i    = FALSE;
        coreotn_int->framer_stg2.rfrm.comb_dbdi_i    = FALSE;
        coreotn_int->framer_stg2.rfrm.comb_dltc_i    = FALSE;
        coreotn_int->framer_stg2.rfrm.comb_diae_i    = FALSE;
        coreotn_int->framer_stg2.rfrm.comb_dbiae_i   = FALSE;
        coreotn_int->framer_stg2.rfrm.comb_oof_i     = FALSE;
        coreotn_int->framer_stg2.rfrm.comb_atsf_i    = FALSE;
        coreotn_int->framer_stg2.rfrm.comb_atsd_i    = FALSE;
        coreotn_int->framer_stg2.rfrm.comb_aais_i    = FALSE;
        coreotn_int->framer_stg2.rfrm.comb_abdi_i    = FALSE;
        coreotn_int->framer_stg2.rfrm.comb_dloflom_i = FALSE;
    }

    if ((coreotn_int->framer_stg3a.tfrm_valid) || (coreotn_int->framer_stg3a.rfrm_valid))
    {
        ADPT_PRINT("stg3a.tfrm.dm_udr_i  = [%d] \r\n", coreotn_int->framer_stg3a.tfrm.dm_fifo_udr_i);
        ADPT_PRINT("stg3a.tfrm.dm_ovr_i  = [%d] \r\n", coreotn_int->framer_stg3a.tfrm.dm_fifo_ovr_i);
        ADPT_PRINT("stg3a.rfrm.c_dlck_i  = [%d] \r\n", coreotn_int->framer_stg3a.rfrm.comb_dlck_i);
        ADPT_PRINT("stg3a.rfrm.c_doci_i  = [%d] \r\n", coreotn_int->framer_stg3a.rfrm.comb_doci_i);
        ADPT_PRINT("stg3a.rfrm.c_dais_i  = [%d] \r\n", coreotn_int->framer_stg3a.rfrm.comb_dais_i);
        ADPT_PRINT("stg3a.rfrm.c_dtim_i  = [%d] \r\n", coreotn_int->framer_stg3a.rfrm.comb_dtim_i);
        ADPT_PRINT("stg3a.rfrm.c_ddeg_i  = [%d] \r\n", coreotn_int->framer_stg3a.rfrm.comb_ddeg_i);
        ADPT_PRINT("stg3a.rfrm.c_dbdi_i  = [%d] \r\n", coreotn_int->framer_stg3a.rfrm.comb_dbdi_i);
        ADPT_PRINT("stg3a.rfrm.c_dltc_i  = [%d] \r\n", coreotn_int->framer_stg3a.rfrm.comb_dltc_i);
        ADPT_PRINT("stg3a.rfrm.c_diae_i  = [%d] \r\n", coreotn_int->framer_stg3a.rfrm.comb_diae_i);
        ADPT_PRINT("stg3a.rfrm.c_dbiae_i = [%d] \r\n", coreotn_int->framer_stg3a.rfrm.comb_dbiae_i);
        ADPT_PRINT("stg3a.rfrm.c_oof_i   = [%d] \r\n", coreotn_int->framer_stg3a.rfrm.comb_oof_i);
        ADPT_PRINT("stg3a.rfrm.c_atsf_i  = [%d] \r\n", coreotn_int->framer_stg3a.rfrm.comb_atsf_i);
        ADPT_PRINT("stg3a.rfrm.c_atsd_i  = [%d] \r\n", coreotn_int->framer_stg3a.rfrm.comb_atsd_i);
        ADPT_PRINT("stg3a.rfrm.c_atsd_i  = [%d] \r\n", coreotn_int->framer_stg3a.rfrm.comb_aais_i);
        ADPT_PRINT("stg3a.rfrm.c_abdi_i  = [%d] \r\n", coreotn_int->framer_stg3a.rfrm.comb_abdi_i);
        ADPT_PRINT("stg3a.rfrm.dloflom_i = [%d] \r\n", coreotn_int->framer_stg3a.rfrm.comb_dloflom_i);
        /* clear interrupt */
        coreotn_int->framer_stg3a.tfrm.dm_fifo_udr_i  = FALSE;
        coreotn_int->framer_stg3a.tfrm.dm_fifo_ovr_i  = FALSE;
        coreotn_int->framer_stg3a.rfrm.comb_dlck_i    = FALSE;
        coreotn_int->framer_stg3a.rfrm.comb_doci_i    = FALSE;
        coreotn_int->framer_stg3a.rfrm.comb_dais_i    = FALSE;
        coreotn_int->framer_stg3a.rfrm.comb_dtim_i    = FALSE;
        coreotn_int->framer_stg3a.rfrm.comb_ddeg_i    = FALSE;
        coreotn_int->framer_stg3a.rfrm.comb_dbdi_i    = FALSE;
        coreotn_int->framer_stg3a.rfrm.comb_dltc_i    = FALSE;
        coreotn_int->framer_stg3a.rfrm.comb_diae_i    = FALSE;
        coreotn_int->framer_stg3a.rfrm.comb_dbiae_i   = FALSE;
        coreotn_int->framer_stg3a.rfrm.comb_oof_i     = FALSE;
        coreotn_int->framer_stg3a.rfrm.comb_atsf_i    = FALSE;
        coreotn_int->framer_stg3a.rfrm.comb_atsd_i    = FALSE;
        coreotn_int->framer_stg3a.rfrm.comb_aais_i    = FALSE;
        coreotn_int->framer_stg3a.rfrm.comb_abdi_i    = FALSE;
        coreotn_int->framer_stg3a.rfrm.comb_dloflom_i = FALSE;
    }

    if ((coreotn_int->framer_stg3b.tfrm_valid) || (coreotn_int->framer_stg3b.rfrm_valid))
    {
        ADPT_PRINT("stg3b.tfrm.dm_udr_i  = [%d] \r\n", coreotn_int->framer_stg3b.tfrm.dm_fifo_udr_i);
        ADPT_PRINT("stg3b.tfrm.dm_ovr_i  = [%d] \r\n", coreotn_int->framer_stg3b.tfrm.dm_fifo_ovr_i);
        ADPT_PRINT("stg3b.rfrm.c_dlck_i  = [%d] \r\n", coreotn_int->framer_stg3b.rfrm.comb_dlck_i);
        ADPT_PRINT("stg3b.rfrm.c_doci_i  = [%d] \r\n", coreotn_int->framer_stg3b.rfrm.comb_doci_i);
        ADPT_PRINT("stg3b.rfrm.c_dais_i  = [%d] \r\n", coreotn_int->framer_stg3b.rfrm.comb_dais_i);
        ADPT_PRINT("stg3b.rfrm.c_dtim_i  = [%d] \r\n", coreotn_int->framer_stg3b.rfrm.comb_dtim_i);
        ADPT_PRINT("stg3b.rfrm.c_ddeg_i  = [%d] \r\n", coreotn_int->framer_stg3b.rfrm.comb_ddeg_i);
        ADPT_PRINT("stg3b.rfrm.c_dbdi_i  = [%d] \r\n", coreotn_int->framer_stg3b.rfrm.comb_dbdi_i);
        ADPT_PRINT("stg3b.rfrm.c_dltc_i  = [%d] \r\n", coreotn_int->framer_stg3b.rfrm.comb_dltc_i);
        ADPT_PRINT("stg3b.rfrm.c_diae_i  = [%d] \r\n", coreotn_int->framer_stg3b.rfrm.comb_diae_i);
        ADPT_PRINT("stg3b.rfrm.c_dbiae_i = [%d] \r\n", coreotn_int->framer_stg3b.rfrm.comb_dbiae_i);
        ADPT_PRINT("stg3b.rfrm.c_oof_i   = [%d] \r\n", coreotn_int->framer_stg3b.rfrm.comb_oof_i);
        ADPT_PRINT("stg3b.rfrm.c_atsf_i  = [%d] \r\n", coreotn_int->framer_stg3b.rfrm.comb_atsf_i);
        ADPT_PRINT("stg3b.rfrm.c_atsd_i  = [%d] \r\n", coreotn_int->framer_stg3b.rfrm.comb_atsd_i);
        ADPT_PRINT("stg3b.rfrm.c_atsd_i  = [%d] \r\n", coreotn_int->framer_stg3b.rfrm.comb_aais_i);
        ADPT_PRINT("stg3b.rfrm.c_abdi_i  = [%d] \r\n", coreotn_int->framer_stg3b.rfrm.comb_abdi_i);
        ADPT_PRINT("stg3b.rfrm.dloflom_i = [%d] \r\n", coreotn_int->framer_stg3b.rfrm.comb_dloflom_i);
        /* clear interrupt */
        coreotn_int->framer_stg3b.tfrm.dm_fifo_udr_i  = FALSE;
        coreotn_int->framer_stg3b.tfrm.dm_fifo_ovr_i  = FALSE;
        coreotn_int->framer_stg3b.rfrm.comb_dlck_i    = FALSE;
        coreotn_int->framer_stg3b.rfrm.comb_doci_i    = FALSE;
        coreotn_int->framer_stg3b.rfrm.comb_dais_i    = FALSE;
        coreotn_int->framer_stg3b.rfrm.comb_dtim_i    = FALSE;
        coreotn_int->framer_stg3b.rfrm.comb_ddeg_i    = FALSE;
        coreotn_int->framer_stg3b.rfrm.comb_dbdi_i    = FALSE;
        coreotn_int->framer_stg3b.rfrm.comb_dltc_i    = FALSE;
        coreotn_int->framer_stg3b.rfrm.comb_diae_i    = FALSE;
        coreotn_int->framer_stg3b.rfrm.comb_dbiae_i   = FALSE;
        coreotn_int->framer_stg3b.rfrm.comb_oof_i     = FALSE;
        coreotn_int->framer_stg3b.rfrm.comb_atsf_i    = FALSE;
        coreotn_int->framer_stg3b.rfrm.comb_atsd_i    = FALSE;
        coreotn_int->framer_stg3b.rfrm.comb_aais_i    = FALSE;
        coreotn_int->framer_stg3b.rfrm.comb_abdi_i    = FALSE;
        coreotn_int->framer_stg3b.rfrm.comb_dloflom_i = FALSE;
    }

    if ((coreotn_int->framer_stgN.tfrm_valid) || (coreotn_int->framer_stgN.rfrm_valid))
    {
        ADPT_PRINT("stgN.tfrm.dm_udr_i  = [%d] \r\n", coreotn_int->framer_stgN.tfrm.dm_fifo_udr_i);
        ADPT_PRINT("stgN.tfrm.dm_ovr_i  = [%d] \r\n", coreotn_int->framer_stgN.tfrm.dm_fifo_ovr_i);
        ADPT_PRINT("stgN.rfrm.c_dlck_i  = [%d] \r\n", coreotn_int->framer_stgN.rfrm.comb_dlck_i);
        ADPT_PRINT("stgN.rfrm.c_doci_i  = [%d] \r\n", coreotn_int->framer_stgN.rfrm.comb_doci_i);
        ADPT_PRINT("stgN.rfrm.c_dais_i  = [%d] \r\n", coreotn_int->framer_stgN.rfrm.comb_dais_i);
        ADPT_PRINT("stgN.rfrm.c_dtim_i  = [%d] \r\n", coreotn_int->framer_stgN.rfrm.comb_dtim_i);
        ADPT_PRINT("stgN.rfrm.c_ddeg_i  = [%d] \r\n", coreotn_int->framer_stgN.rfrm.comb_ddeg_i);
        ADPT_PRINT("stgN.rfrm.c_dbdi_i  = [%d] \r\n", coreotn_int->framer_stgN.rfrm.comb_dbdi_i);
        ADPT_PRINT("stgN.rfrm.c_dltc_i  = [%d] \r\n", coreotn_int->framer_stgN.rfrm.comb_dltc_i);
        ADPT_PRINT("stgN.rfrm.c_diae_i  = [%d] \r\n", coreotn_int->framer_stgN.rfrm.comb_diae_i);
        ADPT_PRINT("stgN.rfrm.c_dbiae_i = [%d] \r\n", coreotn_int->framer_stgN.rfrm.comb_dbiae_i);
        ADPT_PRINT("stgN.rfrm.c_oof_i   = [%d] \r\n", coreotn_int->framer_stgN.rfrm.comb_oof_i);
        ADPT_PRINT("stgN.rfrm.c_atsf_i  = [%d] \r\n", coreotn_int->framer_stgN.rfrm.comb_atsf_i);
        ADPT_PRINT("stgN.rfrm.c_atsd_i  = [%d] \r\n", coreotn_int->framer_stgN.rfrm.comb_atsd_i);
        ADPT_PRINT("stgN.rfrm.c_atsd_i  = [%d] \r\n", coreotn_int->framer_stgN.rfrm.comb_aais_i);
        ADPT_PRINT("stgN.rfrm.c_abdi_i  = [%d] \r\n", coreotn_int->framer_stgN.rfrm.comb_abdi_i);
        ADPT_PRINT("stgN.rfrm.dloflom_i = [%d] \r\n", coreotn_int->framer_stgN.rfrm.comb_dloflom_i);
        /* clear interrupt */
        coreotn_int->framer_stgN.tfrm.dm_fifo_udr_i  = FALSE;
        coreotn_int->framer_stgN.tfrm.dm_fifo_ovr_i  = FALSE;
        coreotn_int->framer_stgN.rfrm.comb_dlck_i    = FALSE;
        coreotn_int->framer_stgN.rfrm.comb_doci_i    = FALSE;
        coreotn_int->framer_stgN.rfrm.comb_dais_i    = FALSE;
        coreotn_int->framer_stgN.rfrm.comb_dtim_i    = FALSE;
        coreotn_int->framer_stgN.rfrm.comb_ddeg_i    = FALSE;
        coreotn_int->framer_stgN.rfrm.comb_dbdi_i    = FALSE;
        coreotn_int->framer_stgN.rfrm.comb_dltc_i    = FALSE;
        coreotn_int->framer_stgN.rfrm.comb_diae_i    = FALSE;
        coreotn_int->framer_stgN.rfrm.comb_dbiae_i   = FALSE;
        coreotn_int->framer_stgN.rfrm.comb_oof_i     = FALSE;
        coreotn_int->framer_stgN.rfrm.comb_atsf_i    = FALSE;
        coreotn_int->framer_stgN.rfrm.comb_atsd_i    = FALSE;
        coreotn_int->framer_stgN.rfrm.comb_aais_i    = FALSE;
        coreotn_int->framer_stgN.rfrm.comb_abdi_i    = FALSE;
        coreotn_int->framer_stgN.rfrm.comb_dloflom_i = FALSE;
    }

    /* COREOTN defect and interrupt */
    ADPT_PRINT(" --------------------------HO ODU------------------------------\r\n");
    for (index = 0; index < DIGI_OTN_SERVER_HO_CHNL_MAX; index++)
    {
        if (NULL == adpt_handle->pmon_data->otn_defect.otn_odu_ho_handle[index])
        {
            continue;
        }
        ADPT_PRINT(" --------------------------index[%d]---------------------------\r\n", index);
        odu_chnl = &adpt_handle->pmon_data->otn_defect.otn_odu_ho_chnl[index];

        /* ODUk defect and interrupt */
        adpt_odu_status_print(&odu_chnl->status);
        if (odu_chnl->stg3b_valid)
        {
            adpt_odu_status_print(&odu_chnl->stg3b);
        }
    }

    ADPT_PRINT(" --------------------------MO ODU------------------------------\r\n");
    for (index = 0; index < DIGI_OTN_SERVER_MO_CHNL_MAX; index++)
    {
        if (NULL == adpt_handle->pmon_data->otn_defect.otn_odu_mo_handle[index])
        {
            continue;
        }
        ADPT_PRINT(" --------------------------index[%d]---------------------------\r\n", index);
        odu_chnl = &adpt_handle->pmon_data->otn_defect.otn_odu_mo_chnl[index];

        /* ODUk defect and interrupt */
        adpt_odu_status_print(&odu_chnl->status);
        if (odu_chnl->stg3b_valid)
        {
            adpt_odu_status_print(&odu_chnl->stg3b);
        }
    }

    ADPT_PRINT(" --------------------------LO ODU-----------------------------\r\n");
    for (index = 0; index < DIGI_OTN_SERVER_LO_CHNL_MAX; index++)
    {
        if (NULL == adpt_handle->pmon_data->otn_defect.otn_odu_lo_handle[index])
        {
            continue;
        }
        ADPT_PRINT(" --------------------------index[%d]---------------------------\r\n", index);
        odu_chnl = &adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[index];

        /* ODUk defect and interrupt */
        adpt_odu_status_print(&odu_chnl->status);
        if (odu_chnl->stg3b_valid)
        {
            adpt_odu_status_print(&odu_chnl->stg3b);
        }
    }

    ADPT_PRINT("===============================================================\r\n");

    PMC_RETURN(rc);
} /* adpt_dump_pmon_data */


/*******************************************************************************
* adpt_dump_int_tree
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps DIGI interrupt tree information. 
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dump_int_tree(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 cpup34k_agg_int_en_reg = 0;
    UINT32 cpup34k_agg_int_val_reg = 0;
    UINT32 coreotn_int_en_reg_h = 0;
    UINT32 coreotn_int_en_reg_l = 0;
    UINT32 coreotn_int_val_reg_h = 0;
    UINT32 coreotn_int_val_reg_l = 0;
    UINT32 lineotn_int_en_reg = 0; 
    UINT32 lineotn_int_val_reg = 0;
    UINT32 mapotn_int_en_reg = 0;
    UINT32 mapotn_int_val_reg = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    cpup34k_agg_int_en_reg  = adpt_reg_read(dev_id, 0x18010D0);
    cpup34k_agg_int_val_reg = adpt_reg_read(dev_id, 0x18010D4);
    coreotn_int_en_reg_h    = adpt_reg_read(dev_id, 0xEAC03C);
    coreotn_int_en_reg_l    = adpt_reg_read(dev_id, 0xEAC038);
    coreotn_int_val_reg_h   = adpt_reg_read(dev_id, 0xEAC024);
    coreotn_int_val_reg_l   = adpt_reg_read(dev_id, 0xEAC020);
    lineotn_int_en_reg      = adpt_reg_read(dev_id, 0x100018); 
    lineotn_int_val_reg     = adpt_reg_read(dev_id, 0x10001C); 
    mapotn_int_en_reg       = adpt_reg_read(dev_id, 0x800308);
    mapotn_int_val_reg      = adpt_reg_read(dev_id, 0x800310);

    ADPT_PRINT("\r\n==============================Interrupt tree===============================\r\n");
    ADPT_PRINT("    |-> {INTB PIN}\r\n\
    |\r\n");
    ADPT_PRINT("    | |<- <En[0x%x]V[0x%x]> 31-22 CPUP34K external interrupt inputs for MIPS\r\n", 
           (cpup34k_agg_int_en_reg >> 22) & 0x3FF, (cpup34k_agg_int_val_reg >> 22) & 0x3FF);
    ADPT_PRINT("    | |<- <En[%d%d%d%d]V[%d%d%d%d]> 21-18 Interrupt from 4 SW Timers\r\n", 
           (cpup34k_agg_int_en_reg >> 21) & 0x01, (cpup34k_agg_int_en_reg >> 20) & 0x01, 
           (cpup34k_agg_int_en_reg >> 19) & 0x01, (cpup34k_agg_int_en_reg >> 18) & 0x01,
           (cpup34k_agg_int_val_reg >> 21) & 0x01, (cpup34k_agg_int_val_reg >> 20) & 0x01,
           (cpup34k_agg_int_val_reg >> 19) & 0x01, (cpup34k_agg_int_val_reg >> 18) & 0x01);
    ADPT_PRINT("    | |<- <En[%d%d]V[%d%d]> 17-16 Interrupts from PKT_CACHE\r\n", 
           (cpup34k_agg_int_en_reg >> 17) & 0x01, (cpup34k_agg_int_en_reg >> 16) & 0x01,
           (cpup34k_agg_int_val_reg >> 17) & 0x01, (cpup34k_agg_int_val_reg >> 16) & 0x01);
    ADPT_PRINT("    | |<- <En[%d]V[%d]> 15 Interrupt from DMA engine\r\n", 
           (cpup34k_agg_int_en_reg >> 15) & 0x01, 
           (cpup34k_agg_int_val_reg >> 15) & 0x01);
    ADPT_PRINT("    | |<- <En[%d%d%d%d]V[%d%d%d%d]> 14-11 Aggregated 4 IPI interrupts for PCIe host\r\n", 
           (cpup34k_agg_int_en_reg >> 14) & 0x01, (cpup34k_agg_int_en_reg >> 13) & 0x01,
           (cpup34k_agg_int_en_reg >> 12) & 0x01, (cpup34k_agg_int_en_reg >> 11) & 0x01,
           (cpup34k_agg_int_val_reg >> 14) & 0x01, (cpup34k_agg_int_val_reg >> 13) & 0x01,
           (cpup34k_agg_int_val_reg >> 12) & 0x01, (cpup34k_agg_int_val_reg >> 11) & 0x01);
    ADPT_PRINT("    | |<- <En[%d]V[%d]> 10 Aggregated miscellaneous interrupts line\r\n", 
           (cpup34k_agg_int_en_reg >> 10) & 0x01, (cpup34k_agg_int_val_reg >> 10) & 0x01);
    ADPT_PRINT("    | |   (9-0 CPUP34K external intr inputs for device Intr pin output.)\r\n");
    ADPT_PRINT("    |_|<- <En[%d]V[%d]> 9 TOPLEVEL || SYSOTN\r\n", 
           (cpup34k_agg_int_en_reg >> 9) & 0x01, (cpup34k_agg_int_val_reg >> 9) & 0x01);
    ADPT_PRINT("      |<- <En[%d]V[%d]> 8 MCPB\r\n", 
           (cpup34k_agg_int_en_reg >> 8) & 0x01, (cpup34k_agg_int_val_reg >> 8) & 0x01);
    ADPT_PRINT("      |<- <En[%d]V[%d]> 7 DCPB\r\n", 
           (cpup34k_agg_int_en_reg >> 7) & 0x01, (cpup34k_agg_int_val_reg >> 7) & 0x01);
    ADPT_PRINT("      |<- <En[%d]V[%d]> 6 SIFD\r\n", 
           (cpup34k_agg_int_en_reg >> 6) & 0x01, (cpup34k_agg_int_val_reg >> 6) & 0x01);
    ADPT_PRINT("      |<- <En[%d]V[%d]> 5 CBRC\r\n", 
           (cpup34k_agg_int_en_reg >> 5) & 0x01, (cpup34k_agg_int_val_reg >> 5) & 0x01);
    ADPT_PRINT("      |<- <En[%d]V[%d]> 4 ENET_SYS\r\n", 
           (cpup34k_agg_int_en_reg >> 4) & 0x01, (cpup34k_agg_int_val_reg >> 4) & 0x01);
    ADPT_PRINT("      |<- <En[%d]V[%d]> 3 ENET_LINE\r\n", 
           (cpup34k_agg_int_en_reg >> 3) & 0x01, (cpup34k_agg_int_val_reg >> 3) & 0x01);
    ADPT_PRINT("      |<- <En[%d]V[%d]> 2 MAPOTN\r\n", 
           (cpup34k_agg_int_en_reg >> 2) & 0x01, (cpup34k_agg_int_val_reg >> 2) & 0x01);
    ADPT_PRINT("      |---^-----\r\n\
      |   | (MAPOTN_INTR 2 [%d]  {To INTB device PIN} )\r\n\
      |   |<--<En[%d]V[%d]> 8  MPMO_INTR\r\n\
      |   |<--<En[%d]V[%d]> 7  ODUKSC_INTR\r\n\
      |   |<--<En[%d]V[%d]> 6  TGMP96_INTR\r\n\
      |   |<--<En[%d]V[%d]> 5  TGFPF96_INTR\r\n\
      |   |<--<En[%d]V[%d]> 4  MAPOTN_TX_TOP_INTR\r\n\
      |   |<--<En[%d]V[%d]> 3  MPMA_INTR\r\n\
      |   |<--<En[%d]V[%d]> 2  RGMP96_INTR\r\n\
      |   |<--<En[%d]V[%d]> 1  RGFPF96_INTR\r\n\
      |   |<--<En[%d]V[%d]> 0  MAPOTN_RX_TOP_INTR\r\n\
      |\r\n",
          (mapotn_int_val_reg >> 18) & 0x01,
          (mapotn_int_en_reg >> 8) & 0x01, (mapotn_int_val_reg >> 8) & 0x01,
          (mapotn_int_en_reg >> 7) & 0x01, (mapotn_int_val_reg >> 7) & 0x01,
          (mapotn_int_en_reg >> 6) & 0x01, (mapotn_int_val_reg >> 6) & 0x01,
          (mapotn_int_en_reg >> 5) & 0x01, (mapotn_int_val_reg >> 5) & 0x01,
          (mapotn_int_en_reg >> 4) & 0x01, (mapotn_int_val_reg >> 4) & 0x01,
          (mapotn_int_en_reg >> 3) & 0x01, (mapotn_int_val_reg >> 3) & 0x01,
          (mapotn_int_en_reg >> 2) & 0x01, (mapotn_int_val_reg >> 2) & 0x01,
          (mapotn_int_en_reg >> 1) & 0x01, (mapotn_int_val_reg >> 1) & 0x01,
          (mapotn_int_en_reg >> 0) & 0x01, (mapotn_int_val_reg >> 0) & 0x01);
    ADPT_PRINT("      |<- <En[%d]V[%d]> 1 LINEOTN\r\n", 
           (cpup34k_agg_int_en_reg >> 1) & 0x01, (cpup34k_agg_int_val_reg >> 1) & 0x01);
    ADPT_PRINT("      |---^-----\r\n\
      |   | (INT REG 2  {To INTB PIN} )\r\n\
      |   |<--<En[%d]V[%d]> 16 FEC_STAT interrupt  \r\n\
      |   |<--<En[%d]V[%d]> 15 RESERVED \r\n\
      |   |<--<En[%d]V[%d]> 14 RESERVED \r\n\
      |   |<--<En[%d]V[%d]> 13 RESERVED \r\n\
      |   |<--<En[%d]V[%d]> 12-10 I7_FEC Slice [2:0] Interrupt  \r\n\
      |   |<--<En[%d]V[%d]> 11 \r\n\
      |   |<--<En[%d]V[%d]> 10 \r\n\
      |   |<--<En[%d]V[%d]> 9-7 I4_FEC Slice [2:0] Interrupt  \r\n\
      |   |<--<En[%d]V[%d]> 8 \r\n\
      |   |<--<En[%d]V[%d]> 7 \r\n\
      |   |<--<En[%d]V[%d]> 6 GFEC_234 Interrupt Enable \r\n\
      |   |<--<En[%d]V[%d]> 5-3 Swizzle7_40G Slice [2:0] Interrupt\r\n\
      |   |<--<En[%d]V[%d]> 4 \r\n\
      |   |<--<En[%d]V[%d]> 3 \r\n\
      |   |<--<En[%d]V[%d]> 2 TOTAL STATS TIP Interrupt  \r\n\
      |   |<--<En[%d]V[%d]> 1 OTU_FRM Interrupt  \r\n\
      |   |<--<En[%d]V[%d]> 0 OTN_MLD Interrupt  \r\n\
      |\r\n",
          (lineotn_int_en_reg >> 16) & 0x01, (lineotn_int_val_reg >> 16) & 0x01,
          (lineotn_int_en_reg >> 15) & 0x01, (lineotn_int_val_reg >> 15) & 0x01,
          (lineotn_int_en_reg >> 14) & 0x01, (lineotn_int_val_reg >> 14) & 0x01,
          (lineotn_int_en_reg >> 13) & 0x01, (lineotn_int_val_reg >> 13) & 0x01,
          (lineotn_int_en_reg >> 12) & 0x01, (lineotn_int_val_reg >> 12) & 0x01,
          (lineotn_int_en_reg >> 11) & 0x01, (lineotn_int_val_reg >> 11) & 0x01,
          (lineotn_int_en_reg >> 10) & 0x01, (lineotn_int_val_reg >> 10) & 0x01,
          (lineotn_int_en_reg >> 9) & 0x01, (lineotn_int_val_reg >> 9) & 0x01,
          (lineotn_int_en_reg >> 8) & 0x01, (lineotn_int_val_reg >> 8) & 0x01,
          (lineotn_int_en_reg >> 7) & 0x01, (lineotn_int_val_reg >> 7) & 0x01,
          (lineotn_int_en_reg >> 6) & 0x01, (lineotn_int_val_reg >> 6) & 0x01,
          (lineotn_int_en_reg >> 5) & 0x01, (lineotn_int_val_reg >> 5) & 0x01,
          (lineotn_int_en_reg >> 4) & 0x01, (lineotn_int_val_reg >> 4) & 0x01,
          (lineotn_int_en_reg >> 3) & 0x01, (lineotn_int_val_reg >> 3) & 0x01,
          (lineotn_int_en_reg >> 2) & 0x01, (lineotn_int_val_reg >> 2) & 0x01,
          (lineotn_int_en_reg >> 1) & 0x01, (lineotn_int_val_reg >> 1) & 0x01,
          (lineotn_int_en_reg >> 0) & 0x01, (lineotn_int_val_reg >> 0) & 0x01
    );
    ADPT_PRINT("      |<- <En[%d]V[%d]> 0 COREOTN\r\n", 
           (cpup34k_agg_int_en_reg >> 0) & 0x01, (cpup34k_agg_int_val_reg >> 0) & 0x01);
    ADPT_PRINT("       ---^-----\r\n\
          | (INT REG 2  {To INTB PIN} )\r\n\
          |<- <En[%d]V[%d]> 0 COREOTN_CTL PCBI: ECC_FRM RAM ECC\r\n\
          |<- <En[%d]V[%d]> 1 ECC_FRM \r\n\
          |<- <En[%d]V[%d]> 2 ODU_RCP: Including ENET_FEGE used by ODU_RCP\r\n\
          |<- <En[%d]V[%d]> 3 ODU_OHP1: Including ENET_FEGE used by ODU_OHP\r\n\
          |<- <En[%d]V[%d]> 4 ODU_OHP2: Including ENET_FEGE used by ODU_OHP\r\n\
          |<- <En[%d]V[%d]> 5 ODU_OHP3: Including ENET_FEGE used by ODU_OHP\r\n\
          |<- <En[%d]V[%d]> 6 COREOTN_CTL MPMO \r\n\
          |<- <En[%d]V[%d]> 7 COREOTN_CTL MPMA \r\n\
          |<- <En[%d]V[%d]> 8 ODUK_SW \r\n\
          |<- <En[%d]V[%d]> 9 COREOTN_FO1 PCBI: Including FRM_RCP and PRBS RAM ECC\r\n\
          |<- <En[%d]V[%d]> 10 COREOTN_FO2 PCBI: Including FRM_RCP and PRBS RAM ECC\r\n\
          |<- <En[%d]V[%d]> 11 Stage 3B MPMO \r\n\
          |<- <En[%d]V[%d]> 12 Stage 4 MPMO \r\n\
          |<- <En[%d]V[%d]> 13 Stage 3B MPMA \r\n\
          |<- <En[%d]V[%d]> 14 Stage 4 MPMA \r\n\
          |<- <En[%d]V[%d]> 15 Stage 3B ODU_TFRM \r\n\
          |<- <En[%d]V[%d]> 16 Stage 4 ODU_TFRM \r\n\
          |<- <En[%d]V[%d]> 17 Stage 3B ODU_RFRM \r\n\
          |<- <En[%d]V[%d]> 18 Stage 4 ODU_RFRM \r\n\
          |<- <En[%d]V[%d]> 19 COREOTN_FMF#1 PCBI \r\n\
          |<- <En[%d]V[%d]> 20 COREOTN_FMF#2 PCBI \r\n\
          |<- <En[%d]V[%d]> 21 Stage 2 ODUKSC \r\n\
          |<- <En[%d]V[%d]> 22 Stage 3A ODUKSC \r\n\
          |<- <En[%d]V[%d]> 23 Stage 1 ODUJAT \r\n\
          |<- <En[%d]V[%d]> 24 Stage N ODUJAT \r\n\
          |<- <En[%d]V[%d]> 25 Stage 2 ODU_RFRM \r\n\
          |<- <En[%d]V[%d]> 26 Stage 3A ODU_RFRM \r\n\
          |<- <En[%d]V[%d]> 27 Stage 1 ODU_RFRM \r\n\
          |<- <En[%d]V[%d]> 28 Stage N ODU_RFRM \r\n\
          |<- <En[%d]V[%d]> 29 Stage 2 ODU_TFRM \r\n\
          |<- <En[%d]V[%d]> 30 Stage 3A ODU_TFRM \r\n\
          |<- <En[%d]V[%d]> 31 Stage 1 ODU_TFRM \r\n\
          |<- <En[%d]V[%d]> 32 Stage N ODU_TFRM \r\n\
          |<- <En[%d]V[%d]> 33 Stage 1 ODTU_MUX \r\n\
          |<- <En[%d]V[%d]> 34 Stage 2 ODTU_MUX \r\n\
          |<- <En[%d]V[%d]> 35 Stage 1 ODTU_DMX \r\n\
          |<- <En[%d]V[%d]> 36 Stage 2 ODTU_DMX \r\n",
        (coreotn_int_en_reg_l >> 0) & 0x01, (coreotn_int_val_reg_l >> 0) & 0x01,
        (coreotn_int_en_reg_l >> 1) & 0x01, (coreotn_int_val_reg_l >> 1) & 0x01,
        (coreotn_int_en_reg_l >> 2) & 0x01, (coreotn_int_val_reg_l >> 2) & 0x01,
        (coreotn_int_en_reg_l >> 3) & 0x01, (coreotn_int_val_reg_l >> 3) & 0x01,
        (coreotn_int_en_reg_l >> 4) & 0x01, (coreotn_int_val_reg_l >> 4) & 0x01,
        (coreotn_int_en_reg_l >> 5) & 0x01, (coreotn_int_val_reg_l >> 5) & 0x01,
        (coreotn_int_en_reg_l >> 6) & 0x01, (coreotn_int_val_reg_l >> 6) & 0x01,
        (coreotn_int_en_reg_l >> 7) & 0x01, (coreotn_int_val_reg_l >> 7) & 0x01,
        (coreotn_int_en_reg_l >> 8) & 0x01, (coreotn_int_val_reg_l >> 8) & 0x01,
        (coreotn_int_en_reg_l >> 9) & 0x01, (coreotn_int_val_reg_l >> 9) & 0x01,
        (coreotn_int_en_reg_l >> 10) & 0x01, (coreotn_int_val_reg_l >> 10) & 0x01,
        (coreotn_int_en_reg_l >> 11) & 0x01, (coreotn_int_val_reg_l >> 11) & 0x01,
        (coreotn_int_en_reg_l >> 12) & 0x01, (coreotn_int_val_reg_l >> 12) & 0x01,
        (coreotn_int_en_reg_l >> 13) & 0x01, (coreotn_int_val_reg_l >> 13) & 0x01,
        (coreotn_int_en_reg_l >> 14) & 0x01, (coreotn_int_val_reg_l >> 14) & 0x01,
        (coreotn_int_en_reg_l >> 15) & 0x01, (coreotn_int_val_reg_l >> 15) & 0x01,    
        (coreotn_int_en_reg_l >> 16) & 0x01, (coreotn_int_val_reg_l >> 16) & 0x01,
        (coreotn_int_en_reg_l >> 17) & 0x01, (coreotn_int_val_reg_l >> 17) & 0x01,
        (coreotn_int_en_reg_l >> 18) & 0x01, (coreotn_int_val_reg_l >> 18) & 0x01,
        (coreotn_int_en_reg_l >> 19) & 0x01, (coreotn_int_val_reg_l >> 19) & 0x01,
        (coreotn_int_en_reg_l >> 20) & 0x01, (coreotn_int_val_reg_l >> 20) & 0x01,
        (coreotn_int_en_reg_l >> 21) & 0x01, (coreotn_int_val_reg_l >> 21) & 0x01,
        (coreotn_int_en_reg_l >> 22) & 0x01, (coreotn_int_val_reg_l >> 22) & 0x01,
        (coreotn_int_en_reg_l >> 23) & 0x01, (coreotn_int_val_reg_l >> 23) & 0x01,
        (coreotn_int_en_reg_l >> 24) & 0x01, (coreotn_int_val_reg_l >> 24) & 0x01,
        (coreotn_int_en_reg_l >> 25) & 0x01, (coreotn_int_val_reg_l >> 25) & 0x01,
        (coreotn_int_en_reg_l >> 26) & 0x01, (coreotn_int_val_reg_l >> 26) & 0x01,
        (coreotn_int_en_reg_l >> 27) & 0x01, (coreotn_int_val_reg_l >> 27) & 0x01,
        (coreotn_int_en_reg_l >> 28) & 0x01, (coreotn_int_val_reg_l >> 28) & 0x01,
        (coreotn_int_en_reg_l >> 29) & 0x01, (coreotn_int_val_reg_l >> 29) & 0x01,
        (coreotn_int_en_reg_l >> 30) & 0x01, (coreotn_int_val_reg_l >> 30) & 0x01,
        (coreotn_int_en_reg_l >> 31) & 0x01, (coreotn_int_val_reg_l >> 31) & 0x01,
        (coreotn_int_en_reg_h >> 0) & 0x01, (coreotn_int_val_reg_h >> 0) & 0x01,
        (coreotn_int_en_reg_h >> 1) & 0x01, (coreotn_int_val_reg_h >> 1) & 0x01,
        (coreotn_int_en_reg_h >> 2) & 0x01, (coreotn_int_val_reg_h >> 2) & 0x01,
        (coreotn_int_en_reg_h >> 3) & 0x01, (coreotn_int_val_reg_h >> 3) & 0x01,
        (coreotn_int_en_reg_h >> 4) & 0x01, (coreotn_int_val_reg_h >> 4) & 0x01);
    ADPT_PRINT("===========================================================================\r\n");

    PMC_RETURN(rc);
}/* adpt_dump_int_tree */


/*******************************************************************************
* adpt_dump_port_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps the configurations of the DIGI RI_RCP, DI_RCP, OHP and FEGE ports. 
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dump_port_cfg(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    pmc_log_sev_t log_sev;
    BOOL drop_pause_frm = FALSE;
    BOOL enable = FALSE;
    UINT16 tx_max_frm = 0;
    UINT16 tx_min_frm = 0;
    UINT16 rx_max_frm = 0;
    UINT16 rx_min_frm = 0;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    ADPT_PRINT("\r\n===================================================================\r\n");
    ADPT_PRINT(" fc_drop_frm fc_en tx_max_frm tx_min_frm rx_max_frm rx_min_frm\r\n");
    rc |= adpt_fege_rx_fc_get(dev_id, &drop_pause_frm, &enable);
    ADPT_PRINT("    [%d]       [%d]     ", drop_pause_frm, enable);
    rc |= adpt_fege_frm_size_get(dev_id, &tx_max_frm, &tx_min_frm, &rx_max_frm, &rx_min_frm); 
    ADPT_PRINT("[%d]      [%d]      [%d]      [%d]\r\n", tx_max_frm, tx_min_frm, rx_max_frm, rx_min_frm);
    ADPT_PRINT("===================================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
}/* adpt_dump_port_cfg */


/*******************************************************************************
* adpt_dump_port_pmon
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps DIGI RI_RCP, DI_RCP, OHP and FEGE port performance count information. 
*
* INPUTS:
*   dev_id             - device identity.
*   port_type          - port type.
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
PUBLIC PMC_ERROR adpt_dump_port_pmon(UINT32 dev_id, digi_sgmii_port_type_t port_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    pmc_log_sev_t log_sev;
    UINT32 baseaddr;
    UINT32 ctrl_baseaddr  = 0xDC5028;
    UINT32 cnt_l_baseaddr = 0xDC5040;
    UINT32 cnt_m_baseaddr = 0xDC5044;
    UINT32 cnt_h_baseaddr = 0xDC5048;
    UINT32 rx_data_h = 0;
    UINT32 rx_data_m = 0;
    UINT32 rx_data_l = 0;
    UINT32 tx_data_h = 0;
    UINT32 tx_data_m = 0; 
    UINT32 tx_data_l = 0;
    UINT32 i         = 0;
    UINT32 offset_rx = 0;
    UINT32 offset_tx = 0;
    port_pmon_data_t rx_port_pmon_data[] = 
    {
        {0 , "RXFramesOK               "},
        {1 , "RXOctetsOK               "},
        {2 , "RXFrames                 "},
        {3 , "RXOctets                 "},
        {4 , "RXUnicastFramesOK        "},
        {5 , "RXBroadcastFramesOK      "},
        {6 , "RXMulticastFramesOK      "},
        {7 , "RXTaggedFramesOK         "},
        {8 , "RXMACPAUSEControlFramesOK"},
        {9 , "RXMACControlFramesOK     "},
        {10, "RXFCSError               "},
        {11, "Reserved                 "},
        {12, "RXSymbolError            "},
        {13, "RXLayer1Error            "},
        {14, "RXInRangeLengthError     "},
        {15, "RXLongLengthError        "},
        {16, "RXLongLengthCRCError     "},
        {17, "RXShortLengthError       "},
        {18, "RXShortlLengthCRCError   "},
        {19, "RXFrames64Octets         "},
        {20, "RXFrames65to127Octets    "},
        {21, "RXFrames128to255Octets   "},
        {22, "RXFrames256to511Octets   "},
        {23, "RXFrames512to1023Octets  "},
        {24, "RXFrames1024to1518Octets "},
        {25, "RXFrames1519toMaxSize    "},
        {26, "RXStationAddressFiltered "},
        {27, "RXVariable               "},
        {28, "RXFrames1519to1522Octets "},
        {29, "RXFrames1523to1548Octets "},
        {30, "RXFrames1549to2000Octets "},
        {31, "RXFrames2001to2500Octets "},
        {32, "RXFrames2501toMaxSize    "}
    };
    port_pmon_data_t tx_port_pmon_data[] = 
    {
        {33, "TXFramesOK              "},
        {34, "TXOctetsOK              "},
        {35, "TXOctets                "},
        {36, "TXTaggedFramesOK        "},
        {37, "TXMACPAUSEControlFrames "},
        {38, "TXFCSError              "},
        {39, "TXShortLengthError      "},
        {40, "TXLongLengthError       "},
        {41, "TXSystemError           "},
        {42, "TXMACError              "},
        {43, "TXJumboFramesOK         "},
        {44, "TXMACControlFramesOK    "},
        {45, "TXUnicastFramesOK       "},
        {46, "TXBroadcastFramesOK     "},
        {47, "TXMulticastFramesOK     "},
        {48, "TXUnicastFramesAttempted"},
        {49, "TXBroadcastFramesAttempt"},
        {50, "TXMulticastFramesAttempt"},
        {51, "TXFrames64Octets        "},
        {52, "TXFrames65to127Octets   "},
        {53, "TXFrames128to255Octets  "},
        {54, "TXFrames256to511Octets  "},
        {55, "TXFrames512to1023Octets "},
        {56, "TXFrames1024to1518Octets"},
        {57, "TXFrames1519toMaxSize   "},
        {58, "TXVariable              "},
        {59, "RXJumboFramesOK         "},
        {60, "TXFrames1519to1522Octets"},
        {61, "TXFrames1523to1548Octets"},
        {62, "TXFrames1549to2000Octets"},
        {63, "TXFrames2001to2500Octets"},
        {64, "TXFrames2501toMaxSize   "}
    };
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (port_type)
    {
        case DIGI_PORT_RI_RCP:
            baseaddr = 0xDC5000;
            break;

        case DIGI_PORT_DI_RCP:
            baseaddr = 0xDCD000;
            break;

        case DIGI_PORT_OHP1:
            baseaddr = 0xE86000;
            break;

        case DIGI_PORT_OHP2:
            baseaddr = 0xE8E000;
            break;

        case DIGI_PORT_OHP3:
            baseaddr = 0xE96000;
            break;

        case DIGI_PORT_MGMT:
            baseaddr = 0x11000; 
            break;

        default:
            ADPT_PRINT("Invalid SGMII port type[%d](0:RI_RCP; 1:DI_RCP; 2:OHP1; 3:OHP2; 4:OHP3; 5:MGMT)\r\n", port_type);
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }
    
    printf("============================SGMII Interface: %s===========================\r\n", adpt_sgmii_port_str(port_type));

    ctrl_baseaddr  = baseaddr + 0x28;
    cnt_l_baseaddr = baseaddr + 0x40;
    cnt_m_baseaddr = baseaddr + 0x44;
    cnt_h_baseaddr = baseaddr + 0x48;   

    /* disable LCLK to allow manual snapshot trigger */
    adpt_reg_write(dev_id, ctrl_baseaddr, (adpt_reg_read(dev_id,ctrl_baseaddr) & (~(1 << 15))));
    /* Transfer RX/TX counts to holding registers and clear the counts */
    adpt_reg_write(dev_id, ctrl_baseaddr, (adpt_reg_read(dev_id,ctrl_baseaddr) | ((1 << 1) | (1 << 5))));

    for (i = 0; i < 32; i++)
    {   
        if (i < (sizeof(rx_port_pmon_data) / sizeof(port_pmon_data_t)))
        {
            offset_rx = rx_port_pmon_data[i].offset << 4;
            rx_data_h = 0xff & adpt_reg_read(dev_id, cnt_h_baseaddr + offset_rx);
            rx_data_m = 0xffff & adpt_reg_read(dev_id, cnt_m_baseaddr + offset_rx);
            rx_data_l = 0xffff & adpt_reg_read(dev_id, cnt_l_baseaddr + offset_rx);
            printf( "%s[0x%-10llX] ", rx_port_pmon_data[i].item_name, ((UINT64)rx_data_h << 32) + ((rx_data_m << 16) + rx_data_l));
        }
        else
        {
            printf( "                         [            ] ");
        }

        if (i < (sizeof(tx_port_pmon_data) / sizeof(port_pmon_data_t)))
        {
            offset_tx = tx_port_pmon_data[i].offset << 4;
            tx_data_h = 0xff & adpt_reg_read(dev_id, cnt_h_baseaddr + offset_tx);
            tx_data_m = 0xffff & adpt_reg_read(dev_id, cnt_m_baseaddr + offset_tx);
            tx_data_l = 0xffff & adpt_reg_read(dev_id, cnt_l_baseaddr + offset_tx);        
            printf( "%s[0x%-10llX]\r\n", tx_port_pmon_data[i].item_name, ((UINT64)tx_data_h << 32) + ((tx_data_m << 16) + tx_data_l));
        }
        else
        {
            printf( "                        [            ]\r\n");
        }
    }

    printf("=============================================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
}/* adpt_dump_port_pmon */


/*******************************************************************************
* adpt_dump_ri_rcp_pmon
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps DIGI RI_RCP performance count information. 
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dump_ri_rcp_pmon(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    rc = adpt_dump_port_pmon(dev_id, DIGI_PORT_RI_RCP);

    PMC_RETURN(rc);
}/* adpt_dump_ri_rcp_pmon */


/*******************************************************************************
* adpt_dump_di_rcp_pmon
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps DIGI DI_RCP performance count information. 
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dump_di_rcp_pmon(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    rc = adpt_dump_port_pmon(dev_id, DIGI_PORT_DI_RCP);

    PMC_RETURN(rc);
}/* adpt_dump_di_rcp_pmon */


/*******************************************************************************
* adpt_dump_fege_alm_i
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps DIGI FEGE interrupts and defects information. 
*
* INPUTS:
*   *enet_fege         - ENET FEGE interrupts.
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
PRIVATE PMC_ERROR adpt_dump_fege_alm_i(enet_fege_int_t *enet_fege)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(enet_fege);

    ADPT_PRINT(" tx_tstmpr_tod_sign_ovfl_i         = [%d]\r\n", enet_fege->tx_tstmpr_tod_sign_ovfl_i);
    ADPT_PRINT(" rx_tstmpr_tod_sign_ovfl_i         = [%d]\r\n", enet_fege->rx_tstmpr_tod_sign_ovfl_i);
    ADPT_PRINT(" overflow_i                        = [%d]\r\n", enet_fege->overflow_i);
    ADPT_PRINT(" rx_comma_del_chg_i                = [%d]\r\n", enet_fege->rx_comma_del_chg_i);
    ADPT_PRINT(" tx_tstmp_fifo_i                   = [%d]\r\n", enet_fege->tx_tstmp_fifo_i);
    ADPT_PRINT(" tx_sof_i                          = [%d]\r\n", enet_fege->tx_sof_i);
    ADPT_PRINT(" rx_sof_i                          = [%d]\r\n", enet_fege->rx_sof_i);
    ADPT_PRINT(" rx_long_length_error_i            = [%d]\r\n", enet_fege->rx_long_length_error_i);
    ADPT_PRINT(" rx_inrange_length_error_i         = [%d]\r\n", enet_fege->rx_inrange_length_error_i);
    ADPT_PRINT(" rx_layer1_error_i                 = [%d]\r\n", enet_fege->rx_layer1_error_i);
    ADPT_PRINT(" rx_symbol_error_i                 = [%d]\r\n", enet_fege->rx_symbol_error_i);
    ADPT_PRINT(" rx_fcs_error_i                    = [%d]\r\n", enet_fege->rx_fcs_error_i);
    ADPT_PRINT(" rx_mac_control_frames_ok_i        = [%d]\r\n", enet_fege->rx_mac_control_frames_ok_i);
    ADPT_PRINT(" rx_mac_pause_control_frames_ok_i  = [%d]\r\n", enet_fege->rx_mac_pause_control_frames_ok_i);
    ADPT_PRINT(" rx_tagged_frames_ok_i             = [%d]\r\n", enet_fege->rx_tagged_frames_ok_i);
    ADPT_PRINT(" rx_multicast_frames_ok_i          = [%d]\r\n", enet_fege->rx_multicast_frames_ok_i);
    ADPT_PRINT(" rx_broadcast_frames_ok_i          = [%d]\r\n", enet_fege->rx_broadcast_frames_ok_i);
    ADPT_PRINT(" rx_unicast_frames_ok_i            = [%d]\r\n", enet_fege->rx_unicast_frames_ok_i);
    ADPT_PRINT(" rx_octets_i                       = [%d]\r\n", enet_fege->rx_octets_i);
    ADPT_PRINT(" rx_frames_i                       = [%d]\r\n", enet_fege->rx_frames_i);
    ADPT_PRINT(" rx_octets_ok_i                    = [%d]\r\n", enet_fege->rx_octets_ok_i);
    ADPT_PRINT(" rx_frames_ok_i                    = [%d]\r\n", enet_fege->rx_frames_ok_i);
    ADPT_PRINT(" rx_variable_i                     = [%d]\r\n", enet_fege->rx_variable_i);
    ADPT_PRINT(" rx_station_address_filtered_i     = [%d]\r\n", enet_fege->rx_station_address_filtered_i);
    ADPT_PRINT(" rx_frames_1519_to_max_size_i      = [%d]\r\n", enet_fege->rx_frames_1519_to_max_size_i);
    ADPT_PRINT(" rx_frames_1024_to_1518_octets_i   = [%d]\r\n", enet_fege->rx_frames_1024_to_1518_octets_i);
    ADPT_PRINT(" rx_frames_512_to_1023_octets_i    = [%d]\r\n", enet_fege->rx_frames_512_to_1023_octets_i);
    ADPT_PRINT(" rx_frames_256_to_511_octets_i     = [%d]\r\n", enet_fege->rx_frames_256_to_511_octets_i);
    ADPT_PRINT(" rx_frames_128_to_255_octets_i     = [%d]\r\n", enet_fege->rx_frames_128_to_255_octets_i);
    ADPT_PRINT(" rx_frames_65_to_127_octets_i      = [%d]\r\n", enet_fege->rx_frames_65_to_127_octets_i);
    ADPT_PRINT(" rx_frames_64_octets_i             = [%d]\r\n", enet_fege->rx_frames_64_octets_i);
    ADPT_PRINT(" rx_short_length_crc_error_i       = [%d]\r\n", enet_fege->rx_short_length_crc_error_i);
    ADPT_PRINT(" rx_short_length_error_i           = [%d]\r\n", enet_fege->rx_short_length_error_i);
    ADPT_PRINT(" rx_long_length_crc_error_i        = [%d]\r\n", enet_fege->rx_long_length_crc_error_i);
    ADPT_PRINT(" rx_frames_2501_to_max_size_i      = [%d]\r\n", enet_fege->rx_frames_2501_to_max_size_i);
    ADPT_PRINT(" rx_frames_2001_to_2500_octets_i   = [%d]\r\n", enet_fege->rx_frames_2001_to_2500_octets_i);
    ADPT_PRINT(" rx_frames_1549_to_2000_octets_i   = [%d]\r\n", enet_fege->rx_frames_1549_to_2000_octets_i);
    ADPT_PRINT(" rx_frames_1523_to_1548_octets_i   = [%d]\r\n", enet_fege->rx_frames_1523_to_1548_octets_i);
    ADPT_PRINT(" rx_frames_1519_to_1522_octets_i   = [%d]\r\n", enet_fege->rx_frames_1519_to_1522_octets_i);
    ADPT_PRINT(" tx_unicast_frames_attempted_i     = [%d]\r\n", enet_fege->tx_unicast_frames_attempted_i);
    ADPT_PRINT(" tx_multicast_frames_ok_i          = [%d]\r\n", enet_fege->tx_multicast_frames_ok_i);
    ADPT_PRINT(" tx_broadcast_frames_ok_i          = [%d]\r\n", enet_fege->tx_broadcast_frames_ok_i);
    ADPT_PRINT(" tx_unicast_frames_ok_i            = [%d]\r\n", enet_fege->tx_unicast_frames_ok_i);
    ADPT_PRINT(" tx_mac_control_frames_ok_i        = [%d]\r\n", enet_fege->tx_mac_control_frames_ok_i);
    ADPT_PRINT(" tx_frames_1523_to_max_size_i      = [%d]\r\n", enet_fege->tx_frames_1523_to_max_size_i);
    ADPT_PRINT(" tx_mac_error_i                    = [%d]\r\n", enet_fege->tx_mac_error_i);
    ADPT_PRINT(" tx_system_error_i                 = [%d]\r\n", enet_fege->tx_system_error_i);
    ADPT_PRINT(" tx_long_length_error_i            = [%d]\r\n", enet_fege->tx_long_length_error_i);
    ADPT_PRINT(" tx_short_length_error_i           = [%d]\r\n", enet_fege->tx_short_length_error_i);
    ADPT_PRINT(" tx_fcs_error_i                    = [%d]\r\n", enet_fege->tx_fcs_error_i);
    ADPT_PRINT(" tx_mac_pause_control_frames_ok_i  = [%d]\r\n", enet_fege->tx_mac_pause_control_frames_ok_i);
    ADPT_PRINT(" tx_tagged_frames_ok_i             = [%d]\r\n", enet_fege->tx_tagged_frames_ok_i);
    ADPT_PRINT(" tx_octets_i                       = [%d]\r\n", enet_fege->tx_octets_i);
    ADPT_PRINT(" tx_octets_ok_i                    = [%d]\r\n", enet_fege->tx_octets_ok_i);
    ADPT_PRINT(" tx_frames_ok_i                    = [%d]\r\n", enet_fege->tx_frames_ok_i);
    ADPT_PRINT(" tx_frames_2501_to_max_size_i      = [%d]\r\n", enet_fege->tx_frames_2501_to_max_size_i)
    ADPT_PRINT(" tx_frames_2001_to_2500_octets_i   = [%d]\r\n", enet_fege->tx_frames_2001_to_2500_octets_i);
    ADPT_PRINT(" tx_frames_1549_to_2000_octets_i   = [%d]\r\n", enet_fege->tx_frames_1549_to_2000_octets_i);
    ADPT_PRINT(" tx_frames_1523_to_1548_octets_i   = [%d]\r\n", enet_fege->tx_frames_1523_to_1548_octets_i);
    ADPT_PRINT(" tx_frames_1519_to_1522_octets_i   = [%d]\r\n", enet_fege->tx_frames_1519_to_1522_octets_i);
    ADPT_PRINT(" rx_frames_1523_to_max_size_i      = [%d]\r\n", enet_fege->rx_frames_1523_to_max_size_i);
    ADPT_PRINT(" tx_variable_i                     = [%d]\r\n", enet_fege->tx_variable_i);
    ADPT_PRINT(" tx_frames_1519_to_max_size_i      = [%d]\r\n", enet_fege->tx_frames_1519_to_max_size_i);
    ADPT_PRINT(" tx_frames_1024_to_1518_octets_i   = [%d]\r\n", enet_fege->tx_frames_1024_to_1518_octets_i);
    ADPT_PRINT(" tx_frames_512_to_1023_octets_i    = [%d]\r\n", enet_fege->tx_frames_512_to_1023_octets_i);
    ADPT_PRINT(" tx_frames_256_to_511_octets_i     = [%d]\r\n", enet_fege->tx_frames_256_to_511_octets_i);
    ADPT_PRINT(" tx_frames_128_to_255_octets_i     = [%d]\r\n", enet_fege->tx_frames_128_to_255_octets_i);
    ADPT_PRINT(" tx_frames_65_to_127_octets_i      = [%d]\r\n", enet_fege->tx_frames_65_to_127_octets_i);
    ADPT_PRINT(" tx_frames_64_octets_i             = [%d]\r\n", enet_fege->tx_frames_64_octets_i);
    ADPT_PRINT(" tx_multicast_frames_attempted_i   = [%d]\r\n", enet_fege->tx_multicast_frames_attempted_i);
    ADPT_PRINT(" tx_broadcast_frames_attempted_i   = [%d]\r\n", enet_fege->tx_broadcast_frames_attempted_i);
    ADPT_PRINT(" los_i                             = [%d]\r\n", enet_fege->los_i);
    ADPT_PRINT(" sdet_in_i                         = [%d]\r\n", enet_fege->sdet_in_i);
    ADPT_PRINT(" align_change_i                    = [%d]\r\n", enet_fege->align_change_i);
    ADPT_PRINT(" link_mon_i                        = [%d]\r\n", enet_fege->link_mon_i);
    ADPT_PRINT(" fec_stat_i                        = [%d]\r\n", enet_fege->fec_stat_i);
    ADPT_PRINT(" carrier_stat_i                    = [%d]\r\n", enet_fege->carrier_stat_i);
    ADPT_PRINT(" rx_err_i                          = [%d]\r\n", enet_fege->rx_err_i);
    ADPT_PRINT(" lcv_i                             = [%d]\r\n", enet_fege->lcv_i);
    ADPT_PRINT(" auto_neg_lp_ablty_upd_i           = [%d]\r\n", enet_fege->auto_neg_lp_ablty_upd_i);
    ADPT_PRINT(" link_ok_i                         = [%d]\r\n", enet_fege->link_ok_i);
    ADPT_PRINT(" sync_ok_i                         = [%d]\r\n", enet_fege->sync_ok_i);

    /* interrupt clear */
    enet_fege->tx_tstmpr_tod_sign_ovfl_i         = FALSE;
    enet_fege->rx_tstmpr_tod_sign_ovfl_i         = FALSE;
    enet_fege->overflow_i                        = FALSE;          
    enet_fege->rx_comma_del_chg_i                = FALSE;    
    enet_fege->tx_tstmp_fifo_i                   = FALSE;
    enet_fege->tx_sof_i                          = FALSE;
    enet_fege->rx_sof_i                          = FALSE;
    enet_fege->rx_long_length_error_i            = FALSE;
    enet_fege->rx_inrange_length_error_i         = FALSE;
    enet_fege->rx_layer1_error_i                 = FALSE;
    enet_fege->rx_symbol_error_i                 = FALSE;
    enet_fege->rx_fcs_error_i                    = FALSE;
    enet_fege->rx_mac_control_frames_ok_i        = FALSE;
    enet_fege->rx_mac_pause_control_frames_ok_i  = FALSE;
    enet_fege->rx_tagged_frames_ok_i             = FALSE;
    enet_fege->rx_multicast_frames_ok_i          = FALSE;
    enet_fege->rx_broadcast_frames_ok_i          = FALSE;
    enet_fege->rx_unicast_frames_ok_i            = FALSE;
    enet_fege->rx_octets_i                       = FALSE;
    enet_fege->rx_frames_i                       = FALSE;
    enet_fege->rx_octets_ok_i                    = FALSE;
    enet_fege->rx_frames_ok_i                    = FALSE;
    enet_fege->rx_variable_i                     = FALSE;
    enet_fege->rx_station_address_filtered_i     = FALSE;
    enet_fege->rx_frames_1519_to_max_size_i      = FALSE;
    enet_fege->rx_frames_1024_to_1518_octets_i   = FALSE;
    enet_fege->rx_frames_512_to_1023_octets_i    = FALSE;
    enet_fege->rx_frames_256_to_511_octets_i     = FALSE;
    enet_fege->rx_frames_128_to_255_octets_i     = FALSE;
    enet_fege->rx_frames_65_to_127_octets_i      = FALSE;
    enet_fege->rx_frames_64_octets_i             = FALSE;
    enet_fege->rx_short_length_crc_error_i       = FALSE;
    enet_fege->rx_short_length_error_i           = FALSE;
    enet_fege->rx_long_length_crc_error_i        = FALSE;
    enet_fege->rx_frames_2501_to_max_size_i      = FALSE;
    enet_fege->rx_frames_2001_to_2500_octets_i   = FALSE; 
    enet_fege->rx_frames_1549_to_2000_octets_i   = FALSE; 
    enet_fege->rx_frames_1523_to_1548_octets_i   = FALSE; 
    enet_fege->rx_frames_1519_to_1522_octets_i   = FALSE; 
    enet_fege->tx_unicast_frames_attempted_i     = FALSE; 
    enet_fege->tx_multicast_frames_ok_i          = FALSE; 
    enet_fege->tx_broadcast_frames_ok_i          = FALSE; 
    enet_fege->tx_unicast_frames_ok_i            = FALSE; 
    enet_fege->tx_mac_control_frames_ok_i        = FALSE; 
    enet_fege->tx_frames_1523_to_max_size_i      = FALSE; 
    enet_fege->tx_mac_error_i                    = FALSE; 
    enet_fege->tx_system_error_i                 = FALSE; 
    enet_fege->tx_long_length_error_i            = FALSE; 
    enet_fege->tx_short_length_error_i           = FALSE; 
    enet_fege->tx_fcs_error_i                    = FALSE; 
    enet_fege->tx_mac_pause_control_frames_ok_i  = FALSE;
    enet_fege->tx_tagged_frames_ok_i             = FALSE; 
    enet_fege->tx_octets_i                       = FALSE; 
    enet_fege->tx_octets_ok_i                    = FALSE; 
    enet_fege->tx_frames_ok_i                    = FALSE; 
    enet_fege->tx_frames_2501_to_max_size_i      = FALSE; 
    enet_fege->tx_frames_2001_to_2500_octets_i   = FALSE; 
    enet_fege->tx_frames_1549_to_2000_octets_i   = FALSE; 
    enet_fege->tx_frames_1523_to_1548_octets_i   = FALSE; 
    enet_fege->tx_frames_1519_to_1522_octets_i   = FALSE; 
    enet_fege->rx_frames_1523_to_max_size_i      = FALSE; 
    enet_fege->tx_variable_i                     = FALSE; 
    enet_fege->tx_frames_1519_to_max_size_i      = FALSE; 
    enet_fege->tx_frames_1024_to_1518_octets_i   = FALSE; 
    enet_fege->tx_frames_512_to_1023_octets_i    = FALSE; 
    enet_fege->tx_frames_256_to_511_octets_i     = FALSE;
    enet_fege->tx_frames_128_to_255_octets_i     = FALSE;
    enet_fege->tx_frames_65_to_127_octets_i      = FALSE;
    enet_fege->tx_frames_64_octets_i             = FALSE;
    enet_fege->tx_multicast_frames_attempted_i   = FALSE;
    enet_fege->tx_broadcast_frames_attempted_i   = FALSE;  
    enet_fege->los_i                             = FALSE;
    enet_fege->sdet_in_i                         = FALSE;
    enet_fege->align_change_i                    = FALSE;
    enet_fege->link_mon_i                        = FALSE;
    enet_fege->fec_stat_i                        = FALSE;
    enet_fege->carrier_stat_i                    = FALSE;
    enet_fege->rx_err_i                          = FALSE;
    enet_fege->lcv_i                             = FALSE;
    enet_fege->auto_neg_lp_ablty_upd_i           = FALSE;
    enet_fege->link_ok_i                         = FALSE;
    enet_fege->sync_ok_i                         = FALSE;

    /* status */
    ADPT_PRINT(" tx_tstmpr_tod_sign_ovfl_v         = [%d]\r\n", enet_fege->tx_tstmpr_tod_sign_ovfl_v);
    ADPT_PRINT(" rx_tstmpr_tod_sign_ovfl_v         = [%d]\r\n", enet_fege->rx_tstmpr_tod_sign_ovfl_v);
    ADPT_PRINT(" los_v                             = [%d]\r\n", enet_fege->los_v);
    ADPT_PRINT(" sdet_in_v                         = [%d]\r\n", enet_fege->sdet_in_v);
    ADPT_PRINT(" align_change_v                    = [%d]\r\n", enet_fege->align_change_v);
    ADPT_PRINT(" link_mon_v                        = [%d]\r\n", enet_fege->link_mon_v);
    ADPT_PRINT(" fec_stat_v                        = [%d]\r\n", enet_fege->fec_stat_v);
    ADPT_PRINT(" carrier_stat_v                    = [%d]\r\n", enet_fege->carrier_stat_v);
    ADPT_PRINT(" auto_neg_lp_ablty_upd_v           = [%d]\r\n", enet_fege->auto_neg_lp_ablty_upd_v);
    ADPT_PRINT(" link_ok_v                         = [%d]\r\n", enet_fege->link_ok_v);
    ADPT_PRINT(" sync_ok_v                         = [%d]\r\n", enet_fege->sync_ok_v);

    PMC_RETURN(rc);
}/* adpt_dump_ohp_alm_i */


/*******************************************************************************
* adpt_dump_mgmt_alm_i
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps DIGI MGMT FEGE interrupts and defects information. 
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dump_mgmt_alm_i(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_device_int_t int_table;
    mgmt_fege_int_t *mgmt_fege;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    PMC_MEMSET((void*)&int_table, 0, sizeof(int_table));
    rc = adpt_device_int_get(dev_id, &int_table);
    ADPT_RETURN_VERIFY(rc);

    mgmt_fege = &int_table.mgmt_fege;

    ADPT_PRINT("\r\n===============================================================\r\n");
    ADPT_PRINT(" tmvp_fifo_overflow_i              = [%d]\r\n", mgmt_fege->tmvp_fifo_overflow_i);
    ADPT_PRINT(" tmvp_invalid_dpi_id_i             = [%d]\r\n", mgmt_fege->tmvp_invalid_dpi_id_i);
    ADPT_PRINT(" tmvp_invalid_type_i               = [%d]\r\n", mgmt_fege->tmvp_invalid_type_i);
    ADPT_PRINT(" rmvp_fifo_overflow_i              = [%d]\r\n", mgmt_fege->rmvp_fifo_overflow_i);
    ADPT_PRINT(" rmvp_invalid_vlan_id_i            = [%d]\r\n", mgmt_fege->rmvp_invalid_vlan_id_i);
    ADPT_PRINT(" rmvp_invalid_type_i               = [%d]\r\n", mgmt_fege->rmvp_invalid_type_i);

    /* FEGE interrupt and defect */
    rc = adpt_dump_fege_alm_i(&mgmt_fege->enet_fege);
    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
}/* adpt_dump_mgmt_alm_i */


/*******************************************************************************
* adpt_dump_ohp_alm_i
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps DIGI OHP interrupts and defects information. 
*
* INPUTS:
*   dev_id             - device identity.
*   ohp_inst           - OHP instance.
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
PUBLIC PMC_ERROR adpt_dump_ohp_alm_i(UINT32 dev_id, odu_ohp_inst_t ohp_inst)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    odu_ohp_int_t *ohp;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    if (ohp_inst >= ODU_OHP_LAST)
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* defect trigger */
    rc = adpt_defect_trigger(dev_id, DIGI_BLK_FULL);
    ADPT_RETURN_VERIFY(rc);

    ohp = &(adpt_handle->pmon_data->otn_defect.ohp[ohp_inst]);

    ADPT_PRINT("\r\n===============================================================\r\n");
    ADPT_PRINT(" rxmac_uid_fail_i                  = [%d]\r\n", ohp->rxmac_uid_fail_i);
    ADPT_PRINT(" rxmac_frmrid_fail_i               = [%d]\r\n", ohp->rxmac_frmrid_fail_i);
    ADPT_PRINT(" rxmac_chid_fail_i                 = [%d]\r\n", ohp->rxmac_chid_fail_i);
    ADPT_PRINT(" rxmac_pkterr_i                    = [%d]\r\n", ohp->rxmac_pkterr_i);
    ADPT_PRINT(" rxmac_fifo_err_i                  = [%d]\r\n", ohp->rxmac_fifo_err_i);

    /* interrupt clear */
    ohp->rxmac_uid_fail_i    = FALSE;
    ohp->rxmac_frmrid_fail_i = FALSE;
    ohp->rxmac_chid_fail_i   = FALSE;
    ohp->rxmac_pkterr_i      = FALSE;
    ohp->rxmac_fifo_err_i    = FALSE;

    /* FEGE interrupt and defect */
    rc = adpt_dump_fege_alm_i(&ohp->enet_fege);
    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
}/* adpt_dump_ohp_alm_i */


/*******************************************************************************
* adpt_dump_di_rcp_alm_i
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps DIGI DI_RCP interrupts and defects information. 
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dump_di_rcp_alm_i(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    odu_rcp_int_t *rcp;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* defect trigger */
    rc = adpt_defect_trigger(dev_id, DIGI_BLK_FULL);
    ADPT_RETURN_VERIFY(rc);

    rcp = &adpt_handle->pmon_data->otn_defect.di_rcp;

    ADPT_PRINT("\r\n===============================================================\r\n");
    ADPT_PRINT(" pmon_tx_done_i                    = [%d]\r\n", rcp->pmon_tx_done_i);
    ADPT_PRINT(" dfec_tx_done_i                    = [%d]\r\n", rcp->dfec_tx_done_i);

    /* interrupt clear */
    rcp->pmon_tx_done_i = FALSE;
    rcp->dfec_tx_done_i = FALSE;

    /* FEGE interrupt and defect */
    rc = adpt_dump_fege_alm_i(&rcp->enet_fege);
    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
}/* adpt_dump_di_rcp_alm_i */


/*******************************************************************************
* adpt_dump_ri_rcp_alm_i
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps DIGI RI_RCP interrupts and defects information. 
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dump_ri_rcp_alm_i(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    odu_rcp_int_t *rcp;
    pmc_log_sev_t log_sev;
    PMC_ENTRY();

    /* disable log */
    log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* defect trigger */
    rc = adpt_defect_trigger(dev_id, DIGI_BLK_FULL);
    ADPT_RETURN_VERIFY(rc);

    rcp = &adpt_handle->pmon_data->otn_defect.ri_rcp;

    ADPT_PRINT("\r\n===============================================================\r\n");
    ADPT_PRINT(" pmon_tx_done_i                    = [%d]\r\n", rcp->pmon_tx_done_i);
    ADPT_PRINT(" dfec_tx_done_i                    = [%d]\r\n", rcp->dfec_tx_done_i);

    /* interrupt clear */
    rcp->pmon_tx_done_i = FALSE;
    rcp->dfec_tx_done_i = FALSE;

    /* FEGE interrupt and defect */
    rc = adpt_dump_fege_alm_i(&rcp->enet_fege);
    ADPT_PRINT("===============================================================\r\n");

    /* enable log */
    pmc_log_verbosity_level_set(log_sev);

    PMC_RETURN(rc);
}/* adpt_dump_ri_rcp_alm_i */




/*******************************************************************************
* adpt_dump_dcsu
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dumps DCSU(Digital Clock Synthesis Unit) information. 
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dump_dcsu(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_device_int_t int_table;
    UINT32 index = 0;
    PMC_ENTRY();

    rc = adpt_device_int_get(dev_id, &int_table);
    ADPT_RETURN_VERIFY(rc);

    ADPT_PRINT("\r\n===============================================================\r\n");
    ADPT_PRINT(" index err_fast_i err_slow_i lock_i err_fast_v err_slow_v lock_v \r\n");
    for (index = 0; index < DIGI_DCSU_MAX_INST; index++)
    {
        ADPT_PRINT(" %s", adpt_dcsu_str((digi_dcsu_inst_t)index));
        ADPT_PRINT("     [%d]", int_table.dcsu[index].err_fast_i);
        ADPT_PRINT("        [%d]", int_table.dcsu[index].err_slow_i);
        ADPT_PRINT("     [%d]", int_table.dcsu[index].lock_i);
        ADPT_PRINT("       [%d]", int_table.dcsu[index].err_fast_v);
        ADPT_PRINT("        [%d]", int_table.dcsu[index].err_slow_v);
        ADPT_PRINT("     [%d]\r\n", int_table.dcsu[index].lock_v);
    }
    ADPT_PRINT("===============================================================\r\n");

    PMC_RETURN(rc);
}/* adpt_dump_dcsu */


/*
** End of file
*/
