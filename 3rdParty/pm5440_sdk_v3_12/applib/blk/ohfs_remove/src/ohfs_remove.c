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
*   DESCRIPTION :
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/
#include "ohfs_remove_loc.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*
** Local Macro Definitions
*/
#define MAX_PT_GET_LOOP    10


/*
** Local Structures and Unionsgg
*/

/*
** Local Variables
*/

/*
** Log Messaging
*/
/* Only include the strings if logging with text*/
#ifdef PMC_LOG_STRINGS

/* 
 * The following creates the LOG/ERR strings from above by pulling out the text
 * portion of the LOG_ERR_TABLE above 
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) str,
const char OHFS_REMOVE_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    OHFS_REMOVE_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char OHFS_REMOVE_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

PRIVATE PMC_ERROR ohfs_remove_max_ts_calc(ohfs_remove_handle_t *ohfs_remove_handle, 
                                          UINT32 chnl, util_global_odukp_type_t server_type,
                                          util_global_odu_line_payload_t ts_type,
                                          UINT32 *ts_num_type);

/*
** Public Functions
*/

/*******************************************************************************
* ohfs_remove_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a OHFS_REMOVE block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the OHFS_REMOVE subsystem relative
*                           to the device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifier of the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   ohfs_remove_handle_t  - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC ohfs_remove_handle_t *ohfs_remove_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    ohfs_remove_handle_t *ohfs_remove_handle;

    PMC_ENTRY();

    ohfs_remove_handle = (ohfs_remove_handle_t*)PMC_CTXT_CALLOC(sizeof(ohfs_remove_handle_t), parent);
    
    pmc_handle_init(parent, ohfs_remove_handle, sys_handle, PMC_MID_DIGI_OHFS_REMOVE, tsb_name, base_address);
    
    /* Register log strings. */
    pmc_log_block_strings_register(OHFS_REMOVE_LOG_ERR_STRINGS[0], OHFS_REMOVE_LOG_ERR_TABLE_BASE, OHFS_REMOVE_LOG_ERR_COUNT);
    
    PMC_RETURN(ohfs_remove_handle);
} /* ohfs_remove_ctxt_create */

 
/*******************************************************************************
* ohfs_remove_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a OHFS_REMOVE block instance.
*
*
* INPUTS:
*   *ohfs_remove_handle       - pointer to OHFS_REMOVE handle instance
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
PUBLIC void ohfs_remove_ctxt_destroy(ohfs_remove_handle_t *ohfs_remove_handle)
{
    PMC_ENTRY();
    
    PMC_CTXT_FREE(&ohfs_remove_handle, ohfs_remove_handle);
    
    PMC_RETURN();
} /* ohfs_remove_ctxt_destroy */

/*******************************************************************************
* ohfs_remove_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an OHFS_REMOVE block handle instance.
*
*
* INPUTS:
*   *ohfs_remove_handle       - pointer to OHFS_REMOVE handle instance
*   ohfs_remove_instance      - OHFS_REMOVE of this instance \n 
*                               Refer to ohfs_remove_instance_type_t \n
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
PUBLIC void ohfs_remove_handle_init(ohfs_remove_handle_t *ohfs_remove_handle, 
                                    ohfs_remove_instance_type_t ohfs_remove_instance)
{
    /* variable declaration */
    PMC_ENTRY();
    
    /* configure the parent that is using this block */
    ohfs_remove_handle->cfg.instance = ohfs_remove_instance;
    
    /* be default, MSI mode is auto */
    ohfs_remove_handle->cfg.msi_cfg_mode = UTIL_GLOBAL_OTN_MSI_CFG_AUTO;
    
    PMC_RETURN();
} /* ohfs_remove_handle_init */

/*******************************************************************************
* ohfs_remove_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on ohfs_remove.
*
*
* INPUTS:
*   *ohfs_remove_handle   - pointer to OHFS_REMOVE handle instance
*   *restart_init_cfg_ptr - pointer to restart init config
*   top_energy_state_reg  - The expected energy state 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*   PMC_SUCCESS : \n
*     WARM : register/context are coherent\n
*     CRASH: everything is coherent (a cleanup may has been executed)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_remove_handle_restart_init(ohfs_remove_handle_t *ohfs_remove_handle,
                                                 util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                                 pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(OHFS_REMOVE_LOG_ERR_STRINGS[0], OHFS_REMOVE_LOG_ERR_TABLE_BASE, OHFS_REMOVE_LOG_ERR_COUNT);
    }

    PMC_RETURN(ret_val);
} /* ohfs_remove_handle_restart_init */


/*******************************************************************************
*  ohfs_remove_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize OHFS_REMOVE with parameters specific to this instantiation.
*
* INPUTS:
*   *ohfs_remove_handle         - pointer to OHFS_REMOVE handle instance
*   energy_state                - enum defining state to set the OHFS_REMOVE 
*                                 module to
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
PUBLIC PMC_ERROR ohfs_remove_init(ohfs_remove_handle_t *ohfs_remove_handle, 
                                  pmc_energy_state_t energy_state)
{
    /* variable declaration */
    PMC_ENTRY();
    PMC_ASSERT(ohfs_remove_handle!=NULL, OHFS_REMOVE_ERR_INVALID_PARAMETERS, 0, 0); 
    
    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        ohfs_remove_block_non_default_cfg(ohfs_remove_handle);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_remove_init */


/*******************************************************************************
* ohfs_remove_block_non_default_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize common configuration register for all channels. That is required
*   for proper operation of this block.
*
* INPUTS:
*   *ohfs_remove_handle         - pointer to OHFS_REMOVE handle instance
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
PUBLIC UINT32 ohfs_remove_block_non_default_cfg(ohfs_remove_handle_t *ohfs_remove_handle)
{
    UINT32 chnl_itr;

    PMC_ENTRY();

    for (chnl_itr = 0;chnl_itr < OHFS_REMOVE_MAX_CHNL_NUM; chnl_itr++) {
      ohfs_remove_reg_COMMON_CFG_array_write(NULL, ohfs_remove_handle, chnl_itr, 0x00330000);
      ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_write(NULL, ohfs_remove_handle, chnl_itr, 0x00000000);
      ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_write(NULL, ohfs_remove_handle, chnl_itr, 0x00000000);
      ohfs_remove_reg_LO_ID_MSI_CFG_array_write(NULL, ohfs_remove_handle, chnl_itr, 0x00007F00);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_remove_block_non_default_cfg */

/*******************************************************************************
* ohfs_remove_ram_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Depending on the state resets all the channel based configuration registers 
*   to its default value, or make channel registers accessible and apply any
*   non-default configuration to the set of registers associated with this 
*   channel.
*
* INPUTS:
*   *ohfs_remove_handle         - pointer to OHFS_REMOVE handle instance
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
PUBLIC UINT32 ohfs_remove_ram_init(ohfs_remove_handle_t *ohfs_remove_handle)
{
    UINT32 chnl_itr;

    PMC_ENTRY();

    for (chnl_itr = 0;chnl_itr < OHFS_REMOVE_MAX_CHNL_NUM; chnl_itr++) 
    {
        ohfs_remove_reg_TS_ADDRESS_IPT_array_write(NULL, ohfs_remove_handle, chnl_itr, 0x00007F00);
        
        ohfs_remove_reg_COMMON_CFG_array_write(NULL, ohfs_remove_handle, chnl_itr, 0x00000000);
        ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_write(NULL, ohfs_remove_handle, chnl_itr, 0x00000000);
        ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_write(NULL, ohfs_remove_handle, chnl_itr, 0x00000000);
        ohfs_remove_reg_LO_ID_MSI_CFG_array_write(NULL, ohfs_remove_handle, chnl_itr, 0x00007F00);       
    }

    /* MSI_STAT_UPDATE_DISABLE needs to be 0 */
    ohfs_remove_field_MSI_STAT_UPDATE_DISABLE_set(NULL, ohfs_remove_handle, FALSE);
    
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_remove_ram_init */

/*******************************************************************************
* ohfs_remove_chnl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes and provisions a channel based on the channel's
*   client type and ODUk rate.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95    
*   server_type         - For RGMP, LO OPUk rate \n
*                         For ODTU_DMX, HO ODU rate \n
*                         Refer to util_global_odukp_type_t \n
*   client_type         - Client type if used by RGMP \n
*                         LO ODUj rate if used by ODTU_DMX \n
*                         If referring to Client type, the valid values are: \n                         
*                          ODUKP_PACKET_GFP         = 1, \n
*                          ODU4P_100_GE_GFP         = 2, \n
*                          ODU3P_40_GE_GFP          = 3, \n
*                          ODU2P_10_GE_7_3_GFP      = 4, \n
*                          ODU2P_10_GE_6_2_GFP      = 5, \n
*                          ODU2EP_FC_1200_GFP       = 6, \n
*                          ODU2P_FC_800_GMP         = 7, \n  
*                          ODU2EP_10G_GDPS_GMP      = 8, \n 
*                          ODU2P_5G_GDPS_GMP        = 9, \n
*                          ODU2P_CPRI_5_GMP         = 10, \n 
*                          ODU2P_CPRI_16_GMP        = 11, \n 
*                          ODU2P_CPRI_6_GMP         = 12, \n
*                          ODU3P_40_GE_GMP          = 13, \n
*                          ODU4P_100_GE_GMP         = 14, \n 
*                          ODU3E2P_40_G_CBR_GMP     = 15, \n 
*                          ODUJP_CBRX_GMP           = 16, \n 
*                          ODUFLEX_5G_GDPS_BMP      = 17, \n
*                          ODUFLEX_CPRI_5_BMP       = 18, \n
*                          ODUFLEX_CPRI_16_BMP      = 19, \n
*                          ODUFLEX_CPRI_6_BMP       = 20, \n
*                          ODUFLEX_FC_800_BMP       = 21, \n
*                          ODU2EP_FC_1200_BMP       = 22, \n
*                          ODU2FP_FC_1200_BMP       = 23, \n
*                          ODU1FP_FC_1200_BMP       = 24, \n
*                          ODU1EP_CBR_10G3_7_2_BMP  = 25, \n
*                          ODU2EP_CBR_10G3_7_1_BMP  = 26, \n
*                          ODU3P_OC_768_CB_BMP      = 27, \n
*                          ODU2P_OC_192_CBR_BMP     = 28, \n
*                          ODUKP_CBRX_B_A_BMP       = 29, \n
*                          ODU3P_OC_768_CB_AMP      = 30, \n
*                          ODU2P_OC_192_CBR_AMP     = 31, \n
*                          ODUKP_CBRX_A_AMP         = 32, \n
*                         If referring to LO ODUj rate \n
*                         Refer to util_global_odukp_type_t \n
*   mapping_mode        - Mapping mode \n
*                         Refer to util_global_mapping_mode_t\n
*   ts_type             - tributary slot type \n
*                         Refer to util_global_odu_line_payload_t \n
*                         Only used if ODTU_MUX/DMX is being used \n
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
PUBLIC PMC_ERROR ohfs_remove_chnl_prov(ohfs_remove_handle_t *ohfs_remove_handle, 
                                       UINT32 chnl,
                                       util_global_odukp_type_t server_type,
                                       UINT32 client_type,
                                       util_global_mapping_mode_t mapping_mode,
                                       util_global_odu_line_payload_t ts_type)
{
    /* variable declaration */
    UINT32 ts_num_type;
    UINT32 opukhdr_data;
    UINT32 msi_chk_en;
    UINT32 pt_chk_en;
    UINT32 csf_chk_en;
    UINT32 msi_accept_cnt;
    UINT32 pt_accept_cnt; 
    util_global_opu_payload_type_t pt; 
    UINT32 ho_rcoh_en;
    UINT32 flex_rcoh_en;
    UINT32 dcsf_to_ssf; 
    UINT32 dplm_to_ssf; 
    UINT32 sf_to_ssf;
    UINT32 dloomfi_to_ssf = 0;

    ohfs_remove_buffer_t ohfs_buf[1];
    
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    util_global_map_adapt_func_t adaptation_function = (util_global_map_adapt_func_t)client_type;
    
    PMC_ENTRY();

    /* Determine PT */
    ret_val = util_global_opu_pt_assign((util_global_parent_t)ohfs_remove_handle->cfg.instance, server_type, client_type,
                                        mapping_mode, ts_type, &pt);

    if(ohfs_remove_handle->cfg.instance == OHFS_REMOVE_INSTANCE_RGMP) 
    {       
       PMC_LOG(PMC_LOG_SEV_LOW, OHFS_REMOVE_LOG_CODE_LOG, 0, 0, 
            "ohfs_remove_handle->cfg.instance = %u, mapping_mode = %u\n", 
            ohfs_remove_handle->cfg.instance, mapping_mode);
            
        /*trib_slot = 1;*/                    
        opukhdr_data = 0;   
        msi_chk_en = 0;    
        pt_chk_en = 1;     
        csf_chk_en = 1;    
        msi_accept_cnt = 0;
        pt_accept_cnt = 3;             
        ho_rcoh_en = 0;  
        flex_rcoh_en = 0;   
        dcsf_to_ssf = 0;   
        dplm_to_ssf = 1;   
        sf_to_ssf = 1;

        switch(mapping_mode)
        {
            case UTIL_GLOBAL_GFP:
                if(UTIL_GLOBAL_MAP_ODU2P_10_GE_7_3_GFP  == adaptation_function)
                    opukhdr_data = 1;

                flex_rcoh_en = (server_type == UTIL_GLOBAL_ODUFLEX_GFP ? 1 : 0);
                break;

            case UTIL_GLOBAL_GMP:
            case UTIL_GLOBAL_BMP:
            case UTIL_GLOBAL_AMP:
                break;
            default:
                    PMC_RETURN(OHFS_REMOVE_ERR_INVALID_PARAMETERS);
       }  
        
    }
    else if(ohfs_remove_handle->cfg.instance == OHFS_REMOVE_INSTANCE_DEMUX)
    {
        PMC_LOG(PMC_LOG_SEV_LOW, OHFS_REMOVE_LOG_CODE_LOG, 0, 0, "ohfs_remove_handle->cfg.instance = %u, mapping_mode = %u\n", ohfs_remove_handle->cfg.instance, mapping_mode);
        
        opukhdr_data = 0;      
        csf_chk_en = 0;    
        msi_accept_cnt = 3;
        pt_accept_cnt = 3;             
        ho_rcoh_en = 0;  
        flex_rcoh_en = 0;   
        dcsf_to_ssf = 0;   

        if(ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
        {
            sf_to_ssf = 0;
            msi_chk_en = 0;
            pt_chk_en = 0;
            dplm_to_ssf = 0;
        } else {
            sf_to_ssf = 1;
            msi_chk_en = 1;
            pt_chk_en = 1;
            dplm_to_ssf = 1;
        }

        if(server_type == UTIL_GLOBAL_ODU4){
            dloomfi_to_ssf = 1;
        }

    }
    else
    {
        PMC_RETURN(OHFS_REMOVE_ERR_INVALID_PARAMETERS);
    }
    
    if (PMC_SUCCESS == ret_val) 
    { 
        /* configure TS_NUM_TYPE */
        ret_val = ohfs_remove_max_ts_calc(ohfs_remove_handle, chnl, 
            server_type, ts_type, &ts_num_type);

        if (PMC_SUCCESS == ret_val)
        {
            /* common configuration */
            ohfs_remove_buffer_init(ohfs_buf, ohfs_remove_handle);
            ohfs_remove_field_TS_NUM_TYPE_set(ohfs_buf, ohfs_remove_handle, chnl, ts_num_type);
            ohfs_remove_field_OPUKHDR_DATA_set(ohfs_buf, ohfs_remove_handle, chnl, opukhdr_data);
            ohfs_remove_field_PT_set(ohfs_buf, ohfs_remove_handle, chnl, (UINT32) pt);
            ohfs_remove_field_HO_RCOH_EN_set(ohfs_buf, ohfs_remove_handle, chnl, ho_rcoh_en);
            ohfs_remove_field_FLEX_RCOH_EN_set(ohfs_buf, ohfs_remove_handle, chnl, flex_rcoh_en);
            ohfs_remove_buffer_flush(ohfs_buf);
        }
    }


    if (PMC_SUCCESS == ret_val) 
    {
        ret_val = ohfs_remove_psi_check_cfg(ohfs_remove_handle, chnl, pt_chk_en, csf_chk_en, msi_chk_en);
    }
    
    if (PMC_SUCCESS == ret_val) 
    {
        ret_val = ohfs_remove_accept_cnt_cfg(ohfs_remove_handle, chnl, pt_accept_cnt, msi_accept_cnt);
    }
    
    if (PMC_SUCCESS == ret_val) 
    { 
        ret_val = ohfs_remove_signal_fail_cfg(ohfs_remove_handle, chnl, dcsf_to_ssf, dplm_to_ssf, sf_to_ssf);
    }

    if(PMC_SUCCESS == ret_val) 
    {
        ret_val = ohfs_remove_map_ssf_set(ohfs_remove_handle, chnl, OHFS_REMOVE_MAP_DLOOMFI_TO_SSF, dloomfi_to_ssf);
    }
    
    if (PMC_SUCCESS == ret_val) 
    { 
        /* disable fixed stuff removal*/
        ret_val = ohfs_remove_fs_enbl(ohfs_remove_handle, chnl, 0);
    }
    
    if (PMC_SUCCESS == ret_val) 
    {
    /* configure FS stripping 
    ** For DMX, only perfrom FS stripping if server_type == ODU4
    */
        if ((ohfs_remove_handle->cfg.instance != OHFS_REMOVE_INSTANCE_DEMUX 
             && (mapping_mode !=  UTIL_GLOBAL_GFP || adaptation_function == UTIL_GLOBAL_MAP_ODU2EP_FC_1200_GFP)) 
             || ((ohfs_remove_handle->cfg.instance == OHFS_REMOVE_INSTANCE_DEMUX) 
             && server_type == UTIL_GLOBAL_ODU4))
        {
            ret_val = ohfs_remove_fs_stripping_cfg(ohfs_remove_handle, chnl, server_type, mapping_mode); 
        } 
    }
    
    PMC_RETURN(ret_val);

} /* ohfs_remove_chnl_prov */


/*******************************************************************************
* ohfs_remove_chnl_unprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function deprovisions the specified channel.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   cycle               - calendar cycle to be configured
*                         Valid range: 0 - 95
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95    
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
PUBLIC PMC_ERROR ohfs_remove_chnl_unprov(ohfs_remove_handle_t *ohfs_remove_handle, 
                                         UINT32 cycle, UINT32 chnl)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    /* set TS_IPT_VALID to 0 for all corresponding entries */
    ohfs_remove_field_TS_IPT_VALID_set(NULL, ohfs_remove_handle, cycle, 0);
    ohfs_remove_field_TS_VALID_set(NULL, ohfs_remove_handle, cycle, 0);
 
     
    PMC_RETURN(ret_val);

} /* ohfs_remove_chnl_unprov */

/*******************************************************************************
* ohfs_remove_chnl_cycle_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean a set of OHFS_REMOVE channels and cycles (demux mode)
*                                                                                                                                      
* INPUTS:                                                                       
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   *ho_chnl            - set of HO channel ID to be cleaned
*   num_ho_chnl         - number of channel ID to be cleaned
*   *lo_chnl            - set of channel ID to be cleaned
*   num_lo_chnl         - number of channel ID to be cleaned
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
PUBLIC void ohfs_remove_chnl_cycle_clean( ohfs_remove_handle_t *ohfs_remove_handle, 
                                          UINT32               *ho_chnl,
                                          UINT32               num_ho_chnl,
                                          UINT32               *lo_chnl,
                                          UINT32               num_lo_chnl) 
{
    UINT32 ho_chnl_itr, ipt_itr,  id;
    BOOL8   hit;
    ohfs_remove_buffer_t ohfs_buf[1];

    PMC_ENTRY();
   
    /* Disable channel stuffs */
    for(ho_chnl_itr = 0; ho_chnl_itr < num_ho_chnl;ho_chnl_itr++)
    {
        (void) ohfs_remove_pt_cfg(ohfs_remove_handle, ho_chnl[ho_chnl_itr], 0);        
        (void) ohfs_remove_accept_cnt_cfg(ohfs_remove_handle, ho_chnl[ho_chnl_itr], 0, 0x3);
        ohfs_remove_field_TS_NUM_TYPE_set(NULL, ohfs_remove_handle, ho_chnl[ho_chnl_itr], 0);  
        ohfs_remove_field_OPUKHDR_DATA_set(NULL, ohfs_remove_handle, ho_chnl[ho_chnl_itr], 0);  
        ohfs_remove_field_NULL_FRAME_MODE_set(NULL, ohfs_remove_handle, ho_chnl[ho_chnl_itr], 0);    
        (void) ohfs_remove_psi_check_cfg(ohfs_remove_handle, ho_chnl[ho_chnl_itr], 0, 0, 0);
        (void) ohfs_remove_signal_fail_cfg(ohfs_remove_handle, ho_chnl[ho_chnl_itr], 0, 0, 0);
        (void) ohfs_remove_map_ssf_set(ohfs_remove_handle, ho_chnl[ho_chnl_itr], OHFS_REMOVE_MAP_DLOOMFI_TO_SSF, 0);
        (void) ohfs_remove_fs_cfg(ohfs_remove_handle, ho_chnl[ho_chnl_itr], 0, 0, 0, 0, 0, 0);
    }

    /* clean IPT */
    for(ipt_itr=0; ipt_itr<OHFS_REMOVE_MAX_CHNL_NUM; ipt_itr++)
    {
        hit = FALSE;
        {
            id    = ohfs_remove_field_TS_IPT_ID_get(NULL, ohfs_remove_handle, ipt_itr); 
            for(ho_chnl_itr = 0; ho_chnl_itr < num_ho_chnl && hit == FALSE;ho_chnl_itr++)
            {  
                if (id == ho_chnl[ho_chnl_itr])
                {
                    hit = TRUE;
                    ohfs_remove_field_TS_IPT_VALID_set(NULL, ohfs_remove_handle, ipt_itr, 0); 
                    ohfs_remove_buffer_init(ohfs_buf, ohfs_remove_handle);
                    ohfs_remove_field_TS_IPT_VALID_set(ohfs_buf, ohfs_remove_handle, ipt_itr, 0);
                    ohfs_remove_field_TS_IPT_ID_set(ohfs_buf, ohfs_remove_handle, ipt_itr, 0x7F);
                    ohfs_remove_field_TS_IPT_FRM_NUM_set(ohfs_buf, ohfs_remove_handle, ipt_itr, 0);
                    ohfs_remove_buffer_flush(ohfs_buf);
                }
            }           
        }
    }
    
    PMC_RETURN();

} /* ohfs_remove_chnl_cycle_clean */

/*******************************************************************************
* ohfs_remove_chnl_cycle_clean_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Get the set of cycle to clean 
*                                                                                                                                      
* INPUTS:                                                                       
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   *ho_chnl            - set of HO channel ID to be cleaned
*   num_ho_chnl         - number of channel ID to be cleaned
*   *lo_chnl            - set of channel ID to be cleaned
*   num_lo_chnl         - number of channel ID to be cleaned
*
* OUTPUTS:
*   *cleaned_cycle      - the list of cycle cleaned 
*   num_cleaned_cycle   - number of cycle to be cleaned
*
* RETURNS:
*   None.                                                                     
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/                                        
PUBLIC void ohfs_remove_chnl_cycle_clean_get( ohfs_remove_handle_t *ohfs_remove_handle, 
                                              UINT32               *ho_chnl,
                                              UINT32               num_ho_chnl,
                                              UINT32               *lo_chnl,
                                              UINT32               num_lo_chnl,                                          
                                              UINT32               cleaned_cycle[],                                          
                                              UINT32               *num_cleaned_cycle) 
{
    UINT32 ho_chnl_itr, lo_chnl_itr,ipt_itr,  id, lo_id;
    BOOL8   hit;

    PMC_ENTRY();
      
    *num_cleaned_cycle = 0;
    /* clean IPT */
    for(ipt_itr=0; ipt_itr<OHFS_REMOVE_MAX_CHNL_NUM; ipt_itr++)
    {
        hit = FALSE;
        {
            id    = ohfs_remove_field_TS_IPT_ID_get(NULL, ohfs_remove_handle, ipt_itr); 
            for(ho_chnl_itr = 0; ho_chnl_itr < num_ho_chnl && hit == FALSE;ho_chnl_itr++)
            {  
                if (id == ho_chnl[ho_chnl_itr])
                {
                    hit = TRUE;                   
                    /* capture cleaned cycle */
                    cleaned_cycle[*num_cleaned_cycle] = ipt_itr;
                    *num_cleaned_cycle += 1;                    
                }
            }
            if (FALSE == hit)
            {                
                lo_id = ohfs_remove_field_LO_ID_get(NULL, ohfs_remove_handle, ipt_itr);  
                for(lo_chnl_itr = 0; lo_chnl_itr < num_lo_chnl && hit == FALSE;lo_chnl_itr++)
                {
                    if (lo_id == lo_chnl[lo_chnl_itr])
                    {
                        cleaned_cycle[*num_cleaned_cycle] = ipt_itr;
                        *num_cleaned_cycle += 1;           
                        hit = TRUE;
                    }
                }   
                hit = FALSE;
            }
        }
    }
    
    PMC_RETURN();

} /* ohfs_remove_chnl_cycle_clean_get */

/*******************************************************************************
* ohfs_remove_lo_cycle_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean a set of OHFS_REMOVE cycles (demux mode)
*                                                                                                                                      
* INPUTS:                                                                       
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   *cycle              - set of cycle to be cleaned
*   num_cycle           - number of cycle to be cleaned
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
PUBLIC void ohfs_remove_lo_cycle_clean( ohfs_remove_handle_t *ohfs_remove_handle, 
                                        UINT32                *cycle,
                                        UINT32               num_cycle) 
{
    UINT32 cycle_itr;

    PMC_ENTRY();
   
    /* clean IPT */
    for(cycle_itr=0; cycle_itr<num_cycle; cycle_itr++)
    {             
        ohfs_remove_field_FIRST_TS_set(NULL, ohfs_remove_handle, cycle[cycle_itr], 0);
        ohfs_remove_field_LAST_TS_set(NULL, ohfs_remove_handle, cycle[cycle_itr], 0);
        ohfs_remove_field_CHAN_MAP_TYPE_set(NULL, ohfs_remove_handle, cycle[cycle_itr], 0);        
        ohfs_remove_field_TS_VALID_set(NULL, ohfs_remove_handle, cycle[cycle_itr], 0);
        ohfs_remove_field_MSI_set(NULL, ohfs_remove_handle, cycle[cycle_itr], 0);
        ohfs_remove_field_LO_ID_set(NULL, ohfs_remove_handle, cycle[cycle_itr], 0x7F);        

    }
    
    PMC_RETURN();

} /* ohfs_remove_lo_cycle_clean */

/*******************************************************************************
* ohfs_remove_chnl_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean a set of OHFS_REMOVE channels (demapper mode)
*                                                                                                                                      
* INPUTS:                                                                       
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   *chnl               - set of channel ID to be cleaned
*   num_chnl            - number of channel ID to be cleaned
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None.                                                                     
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/                                        
PUBLIC void ohfs_remove_chnl_clean( ohfs_remove_handle_t *ohfs_remove_handle, 
                                    UINT32               *chnl,
                                    UINT32               num_chnl ) 
{
    UINT32 chnl_itr, ipt_itr,valid, id;
    BOOL8   hit;
    ohfs_remove_buffer_t ohfs_buf[1];

    PMC_ENTRY();
    /* disable fixed stuff */
    for(chnl_itr = 0; chnl_itr < num_chnl;chnl_itr++)
    {
        ohfs_remove_fs_enbl(ohfs_remove_handle, chnl[chnl_itr], 0);       
    }
    /* clean IPT */
    for(ipt_itr=0; ipt_itr<OHFS_REMOVE_MAX_CHNL_NUM; ipt_itr++)
    {
        valid = ohfs_remove_field_TS_IPT_VALID_get(NULL, ohfs_remove_handle, ipt_itr); 
        id    = ohfs_remove_field_TS_IPT_ID_get(NULL, ohfs_remove_handle, ipt_itr); 
        hit = FALSE;
        for(chnl_itr = 0; chnl_itr < num_chnl && hit == FALSE;chnl_itr++)
        {  
            if (1 == valid && id == chnl[chnl_itr])
            {
                hit = TRUE;
                ohfs_remove_field_TS_IPT_VALID_set(NULL, ohfs_remove_handle, ipt_itr, 0); 
                ohfs_remove_buffer_init(ohfs_buf, ohfs_remove_handle);
                ohfs_remove_field_TS_IPT_VALID_set(ohfs_buf, ohfs_remove_handle, ipt_itr, 0); 
                ohfs_remove_field_TS_IPT_ID_set(ohfs_buf, ohfs_remove_handle, ipt_itr, 0x7F);
                ohfs_remove_field_TS_IPT_FRM_NUM_set(ohfs_buf, ohfs_remove_handle, ipt_itr, 0);
                ohfs_remove_buffer_flush(ohfs_buf);
            }
        }
    }
    
    PMC_RETURN();

} /* ohfs_remove_chnl_clean */

/*******************************************************************************
* ohfs_remove_accpt_pt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns a pointer to the accepted PT value which resulted in 
*   PT mismatch.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *accpt_pt           - pointer to the accepted PT value 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_remove_accpt_pt_get(ohfs_remove_handle_t *ohfs_remove_handle, 
                                          UINT32 chnl,
                                          UINT8 *accpt_pt)
{
    /* variable declaration */
    UINT8 first = 0xFF;
    UINT8 second = 0xFF;
    UINT32 loop = 0;
    
    PMC_ENTRY();

    while ( loop < MAX_PT_GET_LOOP )
    {
        first = ohfs_remove_field_PT_STATUS_get(NULL, ohfs_remove_handle, chnl);

        if (loop != 0 && first == second)
        {
            break;
        }
        second = ohfs_remove_field_PT_STATUS_get(NULL, ohfs_remove_handle, chnl);

        if ( first == second )
        {
            break;
        }
        loop++;
    }
    *accpt_pt = first;

    PMC_RETURN(PMC_SUCCESS);
    
} /* ohfs_remove_accpt_pt_get */       


/*******************************************************************************
* ohfs_remove_exp_pt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns a pointer to the expected PT value.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95 
*
* OUTPUTS:
*   *exp_pt             - pointer to expected PT value
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_remove_exp_pt_get(ohfs_remove_handle_t *ohfs_remove_handle, 
                                        UINT32 chnl,
                                        UINT8 *exp_pt)
{
    /* variable declaration */
    
    PMC_ENTRY();
   
    *exp_pt = ohfs_remove_field_PT_get(NULL, ohfs_remove_handle, chnl);
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* ohfs_remove_exp_pt_get */                             
                                    

/*******************************************************************************
* ohfs_remove_accpt_msi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns a pointer to the accepted MSI value which resulted in 
*   MSI mismatch.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   cycle               - calendar cycle to be configured
*                         Valid range: 0 - 95 
*
* OUTPUTS:
*   *msi                - pointer to MSI value
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_remove_accpt_msi_get(ohfs_remove_handle_t *ohfs_remove_handle, 
                                           UINT32 cycle,
                                           UINT8 *msi)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    /* set PT_STAT_UPDATE_DISABLE to '1' */
    ohfs_remove_field_MSI_STAT_UPDATE_DISABLE_set(NULL, ohfs_remove_handle, FALSE);

    /* read accepted PT value trough PT_STATUS register */
    *msi = ohfs_remove_field_MSI_STATUS_get(NULL, ohfs_remove_handle, cycle);
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* ohfs_remove_accpt_msi_get */ 


/*******************************************************************************
* ohfs_remove_rcoh_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns a pointer to the RCOH header received which is 
*   different from the last received RCOH.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *rcoh               - pointer to RCOH header received
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_remove_rcoh_get(ohfs_remove_handle_t *ohfs_remove_handle, 
                                      UINT32 chnl,
                                      UINT32 *rcoh)
{
    /* variable declaration */
     
    PMC_ENTRY();

    /* read RCOH status trough RCOH_STATUS register */
    *rcoh = ohfs_remove_field_RCOH_STATUS_get(NULL, ohfs_remove_handle, chnl);
    
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_remove_rcoh_get */


/*******************************************************************************
* ohfs_remove_trnsfr_rcoh_to_shadow_en
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function triggers the transfer of RCOH from status memory to shadow
*   memory.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
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
PUBLIC PMC_ERROR ohfs_remove_trnsfr_rcoh_to_shadow_en(ohfs_remove_handle_t *ohfs_remove_handle)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    /* set TRSFR_RCOH_TO_SHADOW_EN bit to '1' */
    ohfs_remove_field_TRNSFR_RCOH_TO_SHADOW_EN_set(NULL, ohfs_remove_handle, TRUE);
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* ohfs_remove_trnsfr_rcoh_to_shadow_en */


/*******************************************************************************
* ohfs_remove_func_mode_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures functional mode for a channel. 
*   Note: Functional mode can be changed dynamically.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   func_mode           - functional mode settings
*                         For GMP and GFP this will always be operational mode \n
*                         0 -- Operational \n
*                         1 -- Null Frame mode \n
*                         2 -- Pass through \n
*                         3 -- Non intrusive mode \n
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
PUBLIC PMC_ERROR ohfs_remove_func_mode_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                           UINT32 chnl,
                                           UINT32 func_mode)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    if( func_mode == 0 || func_mode == 2 || func_mode == 3 ) 
    {
        /* Operational/Passthru/non_intrusive */
        /* set NULL_FRAME_MODE  to zero */
        ohfs_remove_field_NULL_FRAME_MODE_set(NULL, ohfs_remove_handle, chnl, FALSE );
    }
    else if( func_mode == 1 ) /* Null Frame mode*/
    {
        /* set NULL_FRAME_MODE  to one */
        ohfs_remove_field_NULL_FRAME_MODE_set(NULL, ohfs_remove_handle, chnl, TRUE );
    }
    else 
    {
        PMC_RETURN(OHFS_REMOVE_ERR_INVALID_PARAMETERS);
    }
    
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_remove_func_mode_cfg */

/*******************************************************************************
* ohfs_remove_null_frame_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the NULL FRAME mode for a channel. 
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8               - TRUE: NULL frame mode is set\n
*                         FALSE:NULL frame mode is not set
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 ohfs_remove_null_frame_mode_get(ohfs_remove_handle_t *ohfs_remove_handle, 
                                            UINT32 chnl)
{
    BOOL8 null_frame_mode;

    PMC_ENTRY();
    
    null_frame_mode = ohfs_remove_field_NULL_FRAME_MODE_get(NULL, ohfs_remove_handle, chnl);
        
    PMC_RETURN(null_frame_mode);

} /* ohfs_remove_null_frame_mode_get */

/*
** Private Functions
*/


/*******************************************************************************
* ohfs_remove_fs_stripping_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures Fixed Stuff Removal for a channel based on the ODUk
*   rate.
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   oduk_type           - OPUk rate if used by TGMP/RGMP or \n
*                         HO ODUk rate if ODTU_MUX/DMX \n
*                         Refer to util_global_odukp_type_t \n
*   mapping_mode        - mapping mode \n
*                         Refer to util_global_mapping_mode_t \n
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
PUBLIC PMC_ERROR ohfs_remove_fs_stripping_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                              UINT32 chnl,
                                              util_global_odukp_type_t oduk_type,
                                              util_global_mapping_mode_t mapping_mode)
{
    /* variable declaration */
    UINT32 fs1_en;  
    UINT32 fs1_16_8;
    UINT32 fs1_addr;
    UINT32 fs2_en;  
    UINT32 fs2_16_8;
    UINT32 fs2_addr;
    
    PMC_ENTRY();
    if (mapping_mode == UTIL_GLOBAL_GMP)
    {
        if (oduk_type == UTIL_GLOBAL_ODU4)
        {
            fs1_en = 1;  
            fs1_16_8 = 0;
            fs1_addr = 3816;
            fs2_en = 0 ;  
            fs2_16_8= 0;
            fs2_addr = 0;
        }
        else
        {
            fs1_en = 0;  
            fs1_16_8 = 0;
            fs1_addr = 0 ;
            fs2_en = 0 ;  
            fs2_16_8= 0;
            fs2_addr = 0;
        }
    }
    else if(oduk_type == UTIL_GLOBAL_ODU1 || oduk_type == UTIL_GLOBAL_ODU1E 
            || oduk_type == UTIL_GLOBAL_ODU1E_ETRANS || oduk_type == UTIL_GLOBAL_ODU1F )
    {
        fs1_en = 0;  
        fs1_16_8 = 0;
        fs1_addr = 0 ;
        fs2_en = 0 ;  
        fs2_16_8= 0;
        fs2_addr = 0;
    }
    else if (oduk_type == UTIL_GLOBAL_ODU2 || oduk_type == UTIL_GLOBAL_ODU2E 
             || oduk_type == UTIL_GLOBAL_ODU2E_ETRANS || oduk_type == UTIL_GLOBAL_ODU2F)
    {
        fs1_en = 1;  
        fs1_16_8 = 1;
        fs1_addr = 1904;
        fs2_en = 0;  
        fs2_16_8= 0;
        fs2_addr = 0;
    }
    else if (oduk_type == UTIL_GLOBAL_ODU3 || oduk_type == UTIL_GLOBAL_ODU3E1 
             || oduk_type == UTIL_GLOBAL_ODU3E2)
    {
        fs1_en = 1;  
        fs1_16_8 = 1;
        fs1_addr = 1264;
        fs2_en = 1;  
        fs2_16_8= 1;
        fs2_addr = 2544;
    }
    else if (oduk_type == UTIL_GLOBAL_ODU4)
    {
        fs1_en = 1;  
        fs1_16_8 = 0;
        fs1_addr = 3816;
        fs2_en = 0;  
        fs2_16_8= 0;
        fs2_addr = 0;
    }
    else if (oduk_type == UTIL_GLOBAL_ODUFLEX_CBR)
    {
        fs1_en = 0;  
        fs1_16_8 = 0;
        fs1_addr = 0;
        fs2_en = 0;  
        fs2_16_8= 0;
        fs2_addr = 0;
    }
    else 
    {
        PMC_RETURN(OHFS_REMOVE_ERR_INVALID_PARAMETERS);
    }
    
    ohfs_remove_field_FS1_EN_set(NULL, ohfs_remove_handle, chnl, fs1_en);
    ohfs_remove_field_FS1_16_8_set(NULL, ohfs_remove_handle, chnl, fs1_16_8);
    ohfs_remove_field_FS1_ADDR_set(NULL, ohfs_remove_handle, chnl, fs1_addr);
    ohfs_remove_field_FS2_EN_set(NULL, ohfs_remove_handle, chnl, fs2_en);  
    ohfs_remove_field_FS2_16_8_set(NULL, ohfs_remove_handle, chnl, fs2_16_8);
    ohfs_remove_field_FS2_ADDR_set(NULL, ohfs_remove_handle, chnl, fs2_addr);
    
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_remove_fs_stripping_cfg */

/*******************************************************************************
* ohfs_remove_fs_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures fixed stuff.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   fs1_en              - value for fs1_en 
*   fs1_16_8            - value for fs1_16_8
*   fs1_addr            - value for fs1_addr
*   fs2_en              - value for fs2_en
*   fs2_16_8            - value for fs2_16_8
*   fs2_addr            - value for fs2_addr
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
PUBLIC PMC_ERROR ohfs_remove_fs_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                    UINT32 chnl, UINT32 fs1_en, UINT32 fs1_16_8,
                                    UINT32 fs1_addr, UINT32 fs2_en, UINT32 fs2_16_8,
                                    UINT32 fs2_addr)
{                 
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();                           

    ohfs_remove_field_FS1_EN_set(NULL, ohfs_remove_handle, chnl, fs1_en);
    ohfs_remove_field_FS1_16_8_set(NULL, ohfs_remove_handle, chnl, fs1_16_8);
    ohfs_remove_field_FS1_ADDR_set(NULL, ohfs_remove_handle, chnl, fs1_addr);
    ohfs_remove_field_FS2_EN_set(NULL, ohfs_remove_handle, chnl, fs2_en);  
    ohfs_remove_field_FS2_16_8_set(NULL, ohfs_remove_handle, chnl, fs2_16_8);
    ohfs_remove_field_FS2_ADDR_set(NULL, ohfs_remove_handle, chnl, fs2_addr);

    PMC_RETURN(result);
} /* ohfs_remove_fs_cfg */

/*******************************************************************************
* ohfs_remove_max_ts_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function calculates the maximum number of Tributary slots in current 
*   HO channel.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   server_type         - HO ODU type \n
*                         Refer to util_global_odukp_type_t \n
*   ts_type             - tributary slot type \n
*                         Refer to util_global_odu_line_payload_t \n
*
* OUTPUTS:
*   *ts_num_type        - A pointer to the value ot write into the TS_NUM_TYPE
*                         register bit field.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_remove_max_ts_calc(ohfs_remove_handle_t *ohfs_remove_handle, 
                                        UINT32 chnl, 
                                        util_global_odukp_type_t server_type,
                                        util_global_odu_line_payload_t ts_type,
                                        UINT32 *ts_num_type)
{
    /* variable declaration */
    UINT32 max_ts;
    
    PMC_ENTRY();
    
    *ts_num_type = 0;
    
    if (ohfs_remove_handle->cfg.instance == OHFS_REMOVE_INSTANCE_RGMP)
    {
        max_ts = 1;
        *ts_num_type = 0;
    }
    else {
        switch (server_type)
        {
            case UTIL_GLOBAL_ODU1:
                max_ts = 2;
                *ts_num_type = 1;
                break;
            case UTIL_GLOBAL_ODU2:
            case UTIL_GLOBAL_ODU2E:
            case UTIL_GLOBAL_ODU2F:
            case UTIL_GLOBAL_ODU1F:
            case UTIL_GLOBAL_ODU1E:
            case UTIL_GLOBAL_ODU2E_ETRANS:
                if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
                {
                    max_ts = 4;
                    *ts_num_type = 2;
                }
                else if (ts_type == UTIL_GLOBAL_ODU_TS_1G25)
                {
                    max_ts = 8;
                    *ts_num_type = 3;
                }
                else if (ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
                {
                    *ts_num_type = 3;
                }
                break;
            case UTIL_GLOBAL_ODU3:
            case UTIL_GLOBAL_ODU3E2:
            case UTIL_GLOBAL_ODU3E1:
                if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
                {
                    max_ts = 16;
                    *ts_num_type = 4;
                }
                else if (ts_type == UTIL_GLOBAL_ODU_TS_1G25)
                {
                    max_ts = 32;
                    *ts_num_type = 5;
                }
                else if (ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
                {
                    *ts_num_type = 5;
                }
                break;        
            case UTIL_GLOBAL_ODU4:
                if (ts_type == UTIL_GLOBAL_ODU_TS_1G25)
                {
                    max_ts = 80;
                    *ts_num_type = 6;
                }
                else if (ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
                {
                    *ts_num_type = 6;
                }
                else 
                {
                    PMC_RETURN(OHFS_REMOVE_ERR_INVALID_PARAMETERS);
                }
                break;
            case UTIL_GLOBAL_ODUFLEX_CBR:
            case UTIL_GLOBAL_ODUFLEX_GFP:
            case UTIL_GLOBAL_ODU0:
                *ts_num_type = 0;
                break;
            default:
                PMC_RETURN(OHFS_REMOVE_ERR_INVALID_PARAMETERS);
        }  
    }
    
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_remove_max_ts_calc */


/*******************************************************************************
* ohfs_remove_psi_check_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures whether Payload type (PT), Client Signal Fail 
*   (CSF) and Multiplex Structure Identifier (MSI) should be checked or not for 
*   a channel.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   pt_chk_en           - enables/disables whether PT should be checked or not \n
*                         0 -- PT is not checked \n
*                         1 -- Accepted PT is compared with configured \n
*   csf_chk_en          - enables/disables whether CSF should be checked or not \n
*                         0 -- CSF is not checked \n
*                         1 -- CSF interrupt is raised if CSF bit is set in OPUk
*                              over head \n
*   msi_chk_en          - enables/disables whether MSI should be checked or not \n
*                         0 -- MSI is not checked \n
*                         1 -- Accepted MSI is compared with configured \n
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
PUBLIC PMC_ERROR ohfs_remove_psi_check_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                           UINT32 chnl,
                                           UINT32 pt_chk_en, 
                                           UINT32 csf_chk_en, 
                                           UINT32 msi_chk_en)
{
    /* variable declaration */
    ohfs_remove_buffer_t ohfs_buf[1];

    PMC_ENTRY();
   
    ohfs_remove_buffer_init(ohfs_buf, ohfs_remove_handle);
    ohfs_remove_field_PT_CHK_EN_set(ohfs_buf, ohfs_remove_handle, chnl, pt_chk_en);
    ohfs_remove_field_MSI_CHK_EN_set(ohfs_buf, ohfs_remove_handle, chnl, msi_chk_en);
    ohfs_remove_field_CSF_CHK_EN_set(ohfs_buf, ohfs_remove_handle, chnl, csf_chk_en);
    ohfs_remove_buffer_flush(ohfs_buf);
    
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_remove_psi_check_cfg */


/*******************************************************************************
* ohfs_remove_accept_cnt_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures PT value and the number of PT matches required to 
*   accept the PT value
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   pt_accept_cnt       - defines the number of PT matches required to accept 
*                         the PT value
*   msi_accept_cnt      - defines the number of MSI matches required to accept 
*                         the MSI value
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
PUBLIC PMC_ERROR ohfs_remove_accept_cnt_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                            UINT32 chnl, 
                                            UINT32 pt_accept_cnt, 
                                            UINT32 msi_accept_cnt)
{
    /* variable declaration */
    ohfs_remove_buffer_t ohfs_buf[1];

    PMC_ENTRY();
    
    ohfs_remove_buffer_init(ohfs_buf, ohfs_remove_handle);
    ohfs_remove_field_PT_ACCEPTANCE_CNT_set(ohfs_buf, ohfs_remove_handle, chnl, pt_accept_cnt);
    ohfs_remove_field_MSI_ACCEPTANCE_CNT_set(ohfs_buf, ohfs_remove_handle, chnl, msi_accept_cnt);
    ohfs_remove_buffer_flush(ohfs_buf);

    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_remove_accept_cnt_cfg */


/*******************************************************************************
* ohfs_remove_pt_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures PT value and the number of PT matches required to 
*   accept the PT value
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   pt                  - PT value
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
PUBLIC PMC_ERROR ohfs_remove_pt_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                    UINT32 chnl, 
                                    UINT8 pt)
{
    /* variable declaration */

    PMC_ENTRY();
    
    ohfs_remove_field_PT_set(NULL, ohfs_remove_handle, chnl, pt);
   
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_remove_pt_cfg */

    
/*******************************************************************************
* ohfs_remove_flex_rcoh_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures whether to check Flex Rcoh Header.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   enable              - indicates whether to check Flex Rcoh header \n
*                         0 -- do not check Flex Rcoh Header \n
*                         1 -- check Flex Rcoh Header \n
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
PUBLIC PMC_ERROR ohfs_remove_flex_rcoh_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                           UINT32 chnl,
                                           UINT32 enable)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    ohfs_remove_field_FLEX_RCOH_EN_set(NULL, ohfs_remove_handle, chnl, enable);
    
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_remove_flex_rcoh_cfg */


/*******************************************************************************
* ohfs_remove_ho_rcoh_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures whether to check HO Rcoh header.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   enable              - indicates whether to check Flex Rcoh header \n
*                         0 -- do not check HO Rcoh Header \n
*                         1 -- check HO Rcoh Header \n
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
PUBLIC PMC_ERROR ohfs_remove_ho_rcoh_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                         UINT32 chnl,
                                         UINT32 enable)
{
    /* variable declaration */
     
    PMC_ENTRY();
    
    ohfs_remove_field_HO_RCOH_EN_set(NULL, ohfs_remove_handle, chnl, enable);

    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_remove_ho_rcoh_cfg */


/*******************************************************************************
* ohfs_remove_signal_fail_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures signal fail handeling for a channel.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   dcsf_to_ssf         - defines whether CSF indication should be passed to 
*                         egress SSF in operation mode \n
*                         0 -- CSF will not result in egress SSF \n
*                         1 -- CSF will result in egress SSF \n
*   dplm_to_ssf         - defines whether PT mismatch indication should be 
*                         passed to egress SSF in operation mode \n
*                         0 -- PT mismatch event will not result in egress SSF \n
*                         1 -- PT mismatch event will result in egress SSF \n
*   sf_to_ssf           - defines whether ingress SF indication should be passed 
*                         to egress SSF in operation mode \n
*                         0 -- Ingress SF will result in egress SF \n
*                         1 -- Ingress SF will result in egress SSF \n 
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
PUBLIC PMC_ERROR ohfs_remove_signal_fail_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                             UINT32 chnl,
                                             UINT32 dcsf_to_ssf,
                                             UINT32 dplm_to_ssf,
                                             UINT32 sf_to_ssf)
{
    /* variable declaration */
    ohfs_remove_buffer_t ohfs_buf[1];
    
    PMC_ENTRY();
    
    ohfs_remove_buffer_init(ohfs_buf, ohfs_remove_handle);
    ohfs_remove_field_MAP_SF_TO_SSF_set(ohfs_buf, ohfs_remove_handle, chnl, sf_to_ssf);
    ohfs_remove_field_MAP_DPLM_TO_SSF_set(ohfs_buf, ohfs_remove_handle, chnl, dplm_to_ssf);
    ohfs_remove_field_MAP_DCSF_TO_SSF_set(ohfs_buf, ohfs_remove_handle, chnl, dcsf_to_ssf);  
    ohfs_remove_buffer_flush(ohfs_buf);

    
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_remove_signal_fail_cfg */

/*******************************************************************************
* ohfs_remove_map_to_ssf_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures signal fail handeling for a channel.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   map_ssf_type        - Which type of defect to map to ssf 
*   enable              - defines whether the specified defect should be 
*                         passed to egress SSF in operation mode \n
*                         0 -- disable propagation \n
*                         1 -- enable propagation \n
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
PUBLIC PMC_ERROR ohfs_remove_map_ssf_set(ohfs_remove_handle_t *ohfs_remove_handle, 
                                         UINT32 chnl,
                                         ohfs_remove_map_ssf_type_t map_ssf_type,
                                         UINT32 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    /* variable declaration */
    
    PMC_ENTRY();
    
    switch (map_ssf_type)
    {
    case OHFS_REMOVE_MAP_SF_TO_SSF :
        {
            ohfs_remove_field_MAP_SF_TO_SSF_set(NULL, ohfs_remove_handle, chnl, enable);    
            break;
        }
    case OHFS_REMOVE_MAP_DCSF_TO_SSF:
        {
            ohfs_remove_field_MAP_DCSF_TO_SSF_set(NULL, ohfs_remove_handle, chnl, enable);                          
            break; 
        }

    case OHFS_REMOVE_MAP_DLOOMFI_TO_SSF:
        {
            ohfs_remove_field_MAP_DLOOMFI_TO_SSF_set(NULL, ohfs_remove_handle, chnl, enable);
            break;
        }    
    case OHFS_REMOVE_MAP_DPLM_TO_SSF:
        {
            ohfs_remove_field_MAP_DPLM_TO_SSF_set(NULL, ohfs_remove_handle, chnl, enable);
            break;
        }
    default:
        rc = OHFS_REMOVE_ERR_INVALID_PARAMETERS;
    };
    
    PMC_RETURN(rc);

} /* ohfs_remove_map_ssf_set */


/*******************************************************************************
* ohfs_remove_map_ssf_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures signal fail handeling for a channel.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   map_ssf_type        - Which type of defect to map to ssf 
*
* OUTPUTS:
*   *enable        - defines whether the specified defect  indication should be 
*                         passed to egress SSF in operation mode \n
*                         0 -- disabled \n
*                         1 -- enabled \n

*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_remove_map_ssf_get(ohfs_remove_handle_t *ohfs_remove_handle, 
                                         UINT32 chnl,
                                         ohfs_remove_map_ssf_type_t map_ssf_type,                       
                                         UINT32 *enable)
{
    /* variable declaration */
    
    PMC_ENTRY();

    PMC_ERROR rc = PMC_SUCCESS;
    /* variable declaration */
    
    switch (map_ssf_type)
    {
    case OHFS_REMOVE_MAP_SF_TO_SSF :
        {
            *enable = ohfs_remove_field_MAP_SF_TO_SSF_get(NULL, ohfs_remove_handle, chnl);    
            break;
        }
    case OHFS_REMOVE_MAP_DCSF_TO_SSF:
        {
            *enable = ohfs_remove_field_MAP_DCSF_TO_SSF_get(NULL, ohfs_remove_handle, chnl);                          
            break; 
        }
    case OHFS_REMOVE_MAP_DLOOMFI_TO_SSF:
        {
            *enable = ohfs_remove_field_MAP_DLOOMFI_TO_SSF_get(NULL, ohfs_remove_handle, chnl);
            break;
        }    
    case OHFS_REMOVE_MAP_DPLM_TO_SSF:
        {
            *enable = ohfs_remove_field_MAP_DPLM_TO_SSF_get(NULL, ohfs_remove_handle, chnl);
            break;
        }
    default:
        rc = OHFS_REMOVE_ERR_INVALID_PARAMETERS;
    };
    
    PMC_RETURN(rc);
} /* ohfs_remove_map_ssf_get */

/*******************************************************************************
* ohfs_remove_ipt_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures inverted page table for TS address lookup.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   cycle               - calendar cycle to be configured
*                         Valid range: 0 - 95
*   chnl                - HO channel ID which belongs to this cycle
*                         Valid range: 0 - 95
*   frm_num             - occurrence number of the HO channel in a calendar cycle
*                         When the first tributary slot is added for a new HO,
*                         this argument should be set to 0. Every time a new
*                         tributary slot is added for the same HO channel, this
*                         argument needs to be incremented by 1. frm_num should
*                         not go beyond the maximum number of tributary slots 
*                         occupied by a HO channel
*   valid               - determines if the calendar slot is occupied by the 
*                         channel \n
*                         0 -- calendar slot is unoccupied \n
*                         1 -- calendar slot is occupied \n

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
PUBLIC PMC_ERROR ohfs_remove_ipt_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                     UINT32 cycle,
                                     UINT32 chnl,
                                     UINT32 frm_num,
                                     UINT32 valid)                                 
{
    /* variable declaration */
    ohfs_remove_buffer_t ohfs_buf[1];

    PMC_ENTRY();
    
    if(1 == valid)
    {
        ohfs_remove_buffer_init(ohfs_buf, ohfs_remove_handle);
        ohfs_remove_field_TS_IPT_ID_set(ohfs_buf, ohfs_remove_handle, cycle, chnl);
        ohfs_remove_field_TS_IPT_FRM_NUM_set(ohfs_buf, ohfs_remove_handle, cycle, frm_num);
        ohfs_remove_buffer_flush(ohfs_buf);

        ohfs_remove_buffer_init(ohfs_buf, ohfs_remove_handle);
        ohfs_remove_field_TS_IPT_VALID_set(ohfs_buf, ohfs_remove_handle, cycle, valid); 
        ohfs_remove_field_TS_IPT_ID_set(ohfs_buf, ohfs_remove_handle, cycle, chnl);
        ohfs_remove_field_TS_IPT_FRM_NUM_set(ohfs_buf, ohfs_remove_handle, cycle, frm_num);
        ohfs_remove_buffer_flush(ohfs_buf);
    }
    else 
    {
        ohfs_remove_field_TS_IPT_VALID_set(NULL, ohfs_remove_handle, cycle, valid);

        ohfs_remove_buffer_init(ohfs_buf, ohfs_remove_handle);
        ohfs_remove_field_TS_IPT_ID_set(ohfs_buf, ohfs_remove_handle, cycle, chnl);
        ohfs_remove_field_TS_IPT_FRM_NUM_set(ohfs_buf, ohfs_remove_handle, cycle, frm_num);
        ohfs_remove_field_TS_IPT_VALID_set(ohfs_buf, ohfs_remove_handle, cycle, valid);
        ohfs_remove_buffer_flush(ohfs_buf);
    }
    
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_remove_ipt_cfg */
    

/*******************************************************************************
* ohfs_remove_ts_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures trib slot based configurations like trib slot valid,
*   last and first trib slot, and LO ID.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   cycle               - calendar cycle to be configured
*                         Valid range: 0 - 95
*   mapping_mode        - the mapping mode of the channel \n
*                         Refer to util_global_mapping_mode_t \n
*   last_ts             - indicates whether this trib slot is the last trib slot
*                         for current LO channel \n
*                         0 -- not last trib slot \n
*                         1 -- last trib slot \n
*   first_ts            - indicates whether this trib slot is the first trib 
*                         slot for current LO channel \n
*                         0 -- not first trib slot \n
*                         1 -- first trib slot \n
*   valid               - qualifies the table entry \n
*                         0 -- table entry not valid \n
*                         1 -- table entry is valid \n
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
PUBLIC PMC_ERROR ohfs_remove_ts_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                    UINT32 cycle,
                                    util_global_mapping_mode_t mapping_mode,
                                    UINT32 last_ts,
                                    UINT32 first_ts,
                                    UINT32 valid)                                 
{
    /* variable declaration */
    UINT32 map_type;
    ohfs_remove_buffer_t ohfs_buf[1];
    
    PMC_ENTRY();
    
    ohfs_remove_buffer_init(ohfs_buf, ohfs_remove_handle);
    
    if (ohfs_remove_handle->cfg.instance == OHFS_REMOVE_INSTANCE_RGMP)
    {
        switch (mapping_mode)
        {
            case UTIL_GLOBAL_AMP:
                map_type = mapping_mode;
                break;
            case UTIL_GLOBAL_BMP:
                map_type = mapping_mode;
                break;
            case UTIL_GLOBAL_GMP:
                /* GMP = 2 in mapping_mode, but the value should be 4 in the register */
                map_type = 0x4;
                break;
          default:
                map_type = UTIL_GLOBAL_BMP;
                break;
        } 
        
        ohfs_remove_field_CHAN_MAP_TYPE_set(ohfs_buf, ohfs_remove_handle, cycle, map_type); 
    }
    else {
        map_type = mapping_mode;
    }
    
    ohfs_remove_field_CHAN_MAP_TYPE_set(NULL, ohfs_remove_handle, cycle, map_type);  
    ohfs_remove_field_LAST_TS_set(ohfs_buf, ohfs_remove_handle, cycle, last_ts);
    ohfs_remove_field_FIRST_TS_set(ohfs_buf, ohfs_remove_handle, cycle, first_ts);
    ohfs_remove_field_TS_VALID_set(ohfs_buf, ohfs_remove_handle, cycle, valid);
    ohfs_remove_buffer_flush(ohfs_buf);
    
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_remove_ts_cfg */

/*******************************************************************************
* ohfs_remove_ts_valid_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures trib slot based configurations like trib slot valid,
*   last and first trib slot, and LO ID.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   cycle               - calendar cycle to be configured
*                         Valid range: 0 - 95
*   valid               - qualifies the table entry \n
*                         0 -- table entry not valid \n
*                         1 -- table entry is valid \n
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
PUBLIC PMC_ERROR ohfs_remove_ts_valid_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                          UINT32 cycle,
                                          BOOL8 valid) 
{               
    PMC_ERROR result = PMC_SUCCESS;
                         
    PMC_ENTRY();
    
    /* set ts_valid for all tribslots and turn MSI_CHK_EN on for all tribslots */
    ohfs_remove_field_TS_VALID_set(NULL, ohfs_remove_handle, cycle, valid);
    
    PMC_RETURN(result);
} /* ohfs_remove_ts_valid_cfg */


/*******************************************************************************
* ohfs_remove_ts_valid_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures trib slot based configurations like trib slot valid,
*   last and first trib slot, and LO ID.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   cycle               - calendar cycle to be configured
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8 - TRUE : TS_VALID is set for this cycle
*           FALSE : TS_VALID is not set for this cycle
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 ohfs_remove_ts_valid_get(ohfs_remove_handle_t *ohfs_remove_handle, 
                                      UINT32 cycle) 
{               
    BOOL8 valid;
                         
    PMC_ENTRY();
    
    /* set ts_valid for all tribslots and turn MSI_CHK_EN on for all tribslots */
    valid  = ohfs_remove_field_TS_VALID_get(NULL, ohfs_remove_handle, cycle);
    
    PMC_RETURN(valid);
} /* ohfs_remove_ts_valid_get */

/*******************************************************************************
* ohfs_remove_dmx_chan_mapping_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the channel mapping type.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   cycle               - calendar cycle to be configured
*                         Valid range: 0 - 95
*   map_type            - the mapping mode of the channel \n
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
PUBLIC PMC_ERROR ohfs_remove_dmx_chan_mapping_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                                  UINT32 cycle,
                                                  UINT32 map_type)                                 
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    ohfs_remove_field_CHAN_MAP_TYPE_set(NULL, ohfs_remove_handle, cycle, map_type);     
    
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_remove_dmx_chan_mapping_cfg */


/*******************************************************************************
* ohfs_remove_msi_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the expected MSI for one tributary slot in a LO
*   ODU channel.
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   cycle               - Calendar cycle to be configured
*                         Valid range: 0 - 95
*   server_type         - server ODUk type \n
*                         Refer to util_global_odukp_type_t \n
*   client_type         - client type \n
*                         Refer to util_global_odukp_type_t \n
*   ts_type             - tributary slot type \n
*                         Refer to util_global_odu_line_payload_t \n
*   ts_port             - tributary port number where the tributary slot will be
*                         transported.
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
PUBLIC PMC_ERROR ohfs_remove_msi_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                     UINT32 cycle, 
                                     util_global_odukp_type_t server_type, 
                                     UINT32 client_type, 
                                     util_global_odu_line_payload_t ts_type, 
                                     UINT32 ts_port)                                 
{
    PMC_ENTRY();
    
    /* variable declaration */
    UINT32 msi_byte;
    
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    if (ohfs_remove_handle->cfg.msi_cfg_mode == UTIL_GLOBAL_OTN_MSI_CFG_AUTO)
    {
        ret_val = util_global_msi_cfg(ts_port, server_type, client_type, ts_type, 
                                      ts_port, &msi_byte);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
                                           
        ohfs_remove_field_MSI_set(NULL, ohfs_remove_handle, cycle, msi_byte);
    }
    
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_remove_msi_cfg */

/*******************************************************************************
* ohfs_remove_msi_lo_id_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the expected MSI for one tributary slot in a LO
*   ODU channel along with the LO chnl ID.
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   cycle               - Calendar cycle to be configured
*                         Valid range: 0 - 95
*   server_type         - server ODUk type \n
*                         Refer to util_global_odukp_type_t \n
*   client_type         - client type \n
*                         Refer to util_global_odukp_type_t \n
*   ts_type             - tributary slot type \n
*                         Refer to util_global_odu_line_payload_t \n
*   ts_port             - tributary port number where the tributary slot will be
*                         transported.
*   chnl                - LO channel ID which this trib slot belongs to
*                         Valid range: 0 - 95
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
PUBLIC PMC_ERROR ohfs_remove_msi_lo_id_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                           UINT32 cycle, 
                                           util_global_odukp_type_t server_type, 
                                           UINT32 client_type, 
                                           util_global_odu_line_payload_t ts_type,
                                           UINT32 ts_port,
                                           UINT32 chnl)                                 
{
    /* variable declaration */
    UINT32 msi_byte;
    ohfs_remove_buffer_t ohfs_remove_buf[1];
    
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();

    ohfs_remove_buffer_init(ohfs_remove_buf, ohfs_remove_handle);
    if (ohfs_remove_handle->cfg.msi_cfg_mode == UTIL_GLOBAL_OTN_MSI_CFG_AUTO)
    {
        ret_val = util_global_msi_cfg(ts_port, server_type, client_type, ts_type, 
                                      ts_port, &msi_byte);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
                                           
        ohfs_remove_field_MSI_set(ohfs_remove_buf, ohfs_remove_handle, cycle, msi_byte);
    }
    
    ohfs_remove_field_LO_ID_set(ohfs_remove_buf, ohfs_remove_handle, cycle, chnl);
    ohfs_remove_buffer_flush(ohfs_remove_buf);

    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_remove_msi_lo_id_cfg */

/*******************************************************************************
* ohfs_remove_exp_msi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the expected MSI for one calendar cycle of a LO
*   ODU channel. 
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   cycle               - Calendar cycles to be configured
*                         Valid range: 0 - 95
*   msi_val             - MSI 
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
PUBLIC PMC_ERROR ohfs_remove_exp_msi_set(ohfs_remove_handle_t *ohfs_remove_handle, 
                                         UINT32 cycle, 
                                         UINT8 msi_val)                                 
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    ohfs_remove_field_MSI_set(NULL, ohfs_remove_handle, cycle, msi_val);
    
    PMC_RETURN(result);

} /* ohfs_remove_exp_msi_set */

/*******************************************************************************
* ohfs_remove_exp_msi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the expected MSI for one calendar slot of a LO
*   ODU channel. 
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   cycle               - calendar cycle of a LO ODU 
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *msi_val            - MSI byte of this LO channel
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_remove_exp_msi_get(ohfs_remove_handle_t *ohfs_remove_handle, 
                                         UINT32 cycle, 
                                         UINT8 *msi_val)                                 
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    *msi_val = ohfs_remove_field_MSI_get(NULL, ohfs_remove_handle, cycle);
    
    PMC_RETURN(result);

} /* ohfs_remove_exp_msi_get */


/*******************************************************************************
* ohfs_remove_lo_chnl_id_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the LO channel ID.
*
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   cycle               - calendar cycle to be configured
*                         Valid range: 0 - 95
*   chnl                - LO channel ID which this trib slot belongs to
*                         Valid range: 0 - 95
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
PUBLIC PMC_ERROR ohfs_remove_lo_chnl_id_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                            UINT32 cycle, UINT32 chnl)                                 
{
    /* variable declaration */
     
    PMC_ENTRY();
    
    ohfs_remove_field_LO_ID_set(NULL, ohfs_remove_handle, cycle, chnl);
    
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_remove_lo_chnl_id_cfg */



/*******************************************************************************
* ohfs_remove_fs_enbl
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables/diables Fixed Stuff Removal for a channel.
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance  
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   enbl                - enables/disables fixed stuff removal
*                         0 -- disables fixed stuff removal
*                         1 -- enables fixed stuff removal
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
PUBLIC PMC_ERROR ohfs_remove_fs_enbl(ohfs_remove_handle_t *ohfs_remove_handle, 
                                     UINT32 chnl,
                                     UINT32 enbl)
{
    /* variable declaration */
    UINT32 fs1_en, fs2_en;
    ohfs_remove_buffer_t ohfs_remove_buf[1];
    
    PMC_ENTRY();
    if(1 == enbl)
    {
        fs1_en = 1;
        fs2_en = 1;
    } 
    else if(0 == enbl)
    {
        fs1_en = 0;
        fs2_en = 0;
    }
    else
    {
        PMC_RETURN(OHFS_REMOVE_ERR_INVALID_PARAMETERS);
    }
    /* Disable fixed stuffs  */
    ohfs_remove_buffer_init(ohfs_remove_buf, ohfs_remove_handle);
    ohfs_remove_field_FS1_EN_set(ohfs_remove_buf, ohfs_remove_handle, chnl, fs1_en);
    ohfs_remove_field_FS2_EN_set(ohfs_remove_buf, ohfs_remove_handle, chnl, fs2_en);
    ohfs_remove_buffer_flush(ohfs_remove_buf);
    
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_remove_fs_enbl */

/*******************************************************************************
* ohfs_remove_ho_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function clears HO channel related configuration from OHFS_REMOVE when
*   a HO is deprovisioned from ODTU_DMX.
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
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
PUBLIC PMC_ERROR ohfs_remove_ho_chnl_deprov(ohfs_remove_handle_t *ohfs_remove_handle, 
                                            UINT32 chnl)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();    
                                          
    result = ohfs_remove_accept_cnt_cfg(ohfs_remove_handle, chnl, 0, 0x3);
    
    if (PMC_SUCCESS == result)
    {
        result = ohfs_remove_pt_cfg(ohfs_remove_handle, chnl, 0);  
    } 
    
    if (PMC_SUCCESS == result)
    {
        ohfs_remove_field_TS_NUM_TYPE_set(NULL, ohfs_remove_handle, chnl, 0);  
        ohfs_remove_field_OPUKHDR_DATA_set(NULL, ohfs_remove_handle, chnl, 0);  
        ohfs_remove_field_NULL_FRAME_MODE_set(NULL, ohfs_remove_handle, chnl, 0);     
        
        result = ohfs_remove_psi_check_cfg(ohfs_remove_handle, chnl, 0, 0, 0);
    }
    
    if (PMC_SUCCESS == result)
    {
        result = ohfs_remove_signal_fail_cfg(ohfs_remove_handle, chnl, 0, 0, 0);
    }  

    if(PMC_SUCCESS == result)
    {
        result = ohfs_remove_map_ssf_set(ohfs_remove_handle, chnl, OHFS_REMOVE_MAP_DLOOMFI_TO_SSF, 0);
    }
    
    if (PMC_SUCCESS == result)
    {
        result = ohfs_remove_fs_cfg(ohfs_remove_handle, chnl, 0, 0, 0, 0, 0, 0);
    }                                          

    PMC_RETURN(result);
    
} /* ohfs_remove_ho_chnl_deprov */


/*******************************************************************************
* ohfs_remove_ipt_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function Prints the IPT table for ohfs_remove.
*   
*
* INPUTS:
*   *ohfs_remove_handle - pointer to OHFS_REMOVE handle instance                
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
PUBLIC PMC_ERROR ohfs_remove_ipt_print(ohfs_remove_handle_t *ohfs_remove_handle)
{
    PMC_ENTRY();
    
    /* variable declaration */
    UINT32 i, chnl, valid, occr;
      
    for(i=0; i<OHFS_REMOVE_MAX_CHNL_NUM; i++)
    {
        chnl = ohfs_remove_field_TS_IPT_ID_get(NULL, ohfs_remove_handle, i);
        valid = ohfs_remove_field_TS_IPT_VALID_get(NULL, ohfs_remove_handle, i);
        occr = ohfs_remove_field_TS_IPT_FRM_NUM_get(NULL, ohfs_remove_handle, i);
        PMC_LOG_TRACE("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tOHFS_REMOVE IPT ID loc:%d,  chnl_id:%d  valid:%d  occr:%d\n", i, chnl, valid, occr);
    }
    
    PMC_RETURN(PMC_SUCCESS);
} /* ohfs_remove_ipt_print */ 

/*******************************************************************************
* ohfs_remove_mapping_mode_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns the mapping mode at a specified calendar cycle
*                                                                               
* INPUTS:                                                                       
*   *ohfs_remove_handle      - pointer to OHFS_REMOVE handle instance
*   calendar_cycle           - The calendar cycle
*                                                                 
* OUTPUTS:
*   None.
*
* RETURNS:
*   util_global_mapping_mode_t - The mapping mode
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC util_global_mapping_mode_t ohfs_remove_mapping_mode_get(ohfs_remove_handle_t *ohfs_remove_handle,
                                                               UINT32 calendar_cycle)
{
    util_global_mapping_mode_t mode;
    UINT32 map_type;
    PMC_ENTRY();
    
    PMC_ASSERT(ohfs_remove_handle != NULL, OHFS_REMOVE_ERR_INVALID_PARAMETERS, 0, 0);
    
    map_type = ohfs_remove_field_CHAN_MAP_TYPE_get(NULL, ohfs_remove_handle, calendar_cycle);     
    if (map_type == 0)
    {
        mode = UTIL_GLOBAL_BMP;
    } else if (map_type == 1)
    {
        mode = UTIL_GLOBAL_AMP;
    } else if (map_type == 4)
    {
        mode = UTIL_GLOBAL_GMP;
    } else 
    {
        mode = UTIL_GLOBAL_NO_MAP;
    }

    PMC_RETURN(mode);
}  /* ohfs_remove_mapping_mode_get*/

/*******************************************************************************
* ohfs_remove_ext_payload_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns the extended payload bit
*                                                                               
* INPUTS:                                                                       
*   *ohfs_remove_handle      - pointer to OHFS_REMOVE handle instance
*   chnl                     - the channel 
*                                                                 
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32                   - The extended payload value
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC UINT32 ohfs_remove_ext_payload_get(ohfs_remove_handle_t *ohfs_remove_handle,
                                          UINT32 chnl)
{
    UINT32 result;
    PMC_ENTRY();
    result = ohfs_remove_field_OPUKHDR_DATA_get(NULL, ohfs_remove_handle, chnl);
    PMC_RETURN(result);
} /*ohfs_remove_ext_payload_get*/

/*******************************************************************************
* ohfs_remove_fixed_stuff_enable_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns TRUE when fixed stuff is enabled
*                                                                               
* INPUTS:                                                                       
*   *ohfs_remove_handle      - pointer to OHFS_REMOVE handle instance
*   chnl                     - the channel 
*                                                                 
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8                    - TRUE:  when fixed is enabled\n
*                              FALSE: when it is not 
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC BOOL8 ohfs_remove_fixed_stuff_enable_get(ohfs_remove_handle_t *ohfs_remove_handle,
                                                 UINT32 chnl)
{
    BOOL8 result = FALSE;
    UINT32 fs1_en, fs2_en;
    PMC_ENTRY();
    fs1_en = ohfs_remove_field_FS1_EN_get(NULL, ohfs_remove_handle, chnl);
    fs2_en = ohfs_remove_field_FS2_EN_get(NULL, ohfs_remove_handle, chnl);
    
    if (fs1_en == 1 || fs2_en == 1)
    {
        result = TRUE;        
    }
    PMC_RETURN(result);
} /* ohfs_remove_fixed_stuff_enable_get */

/*******************************************************************************
* ohfs_remove_reg_ctxt_recover
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function recovers a part of the OHFS_REMOVE
*                                                                               
* INPUTS:                                                                       
*   *ohfs_remove_handle      - pointer to OHFS_REMOVE handle instance
*                                                                 
* OUTPUTS:
*   *ohfs_remove_recover_reg - OHFS_REMOVE reg context 
*
* RETURNS:
*   None.
*                                                                               
* NOTES:                                                                        
*                                                                               
****************************************************************************/
PUBLIC void ohfs_remove_reg_ctxt_recover(ohfs_remove_handle_t *ohfs_remove_handle, 
                                         ohfs_remove_recover_var_t *ohfs_remove_recover_reg)
{
    UINT32 chnl_cycle_itr;

    PMC_ENTRY();

    for(chnl_cycle_itr = 0;chnl_cycle_itr < OHFS_REMOVE_MAX_CHNL_NUM;chnl_cycle_itr++)
    {
        ohfs_remove_recover_reg->ts_num[chnl_cycle_itr]               = ohfs_remove_field_TS_NUM_TYPE_get(NULL, ohfs_remove_handle, chnl_cycle_itr);
        ohfs_remove_recover_reg->has_fixed_stuff[chnl_cycle_itr]      = ohfs_remove_fixed_stuff_enable_get(ohfs_remove_handle,chnl_cycle_itr);

        ohfs_remove_recover_reg->cycle[chnl_cycle_itr].ho_chnl_id     = ohfs_remove_field_TS_IPT_ID_get(NULL, ohfs_remove_handle, chnl_cycle_itr);
        ohfs_remove_recover_reg->cycle[chnl_cycle_itr].ho_cycle_valid = ohfs_remove_field_TS_IPT_VALID_get(NULL, ohfs_remove_handle, chnl_cycle_itr);
        ohfs_remove_recover_reg->cycle[chnl_cycle_itr].ho_frm_num     = ohfs_remove_field_TS_IPT_FRM_NUM_get(NULL, ohfs_remove_handle, chnl_cycle_itr);
        ohfs_remove_recover_reg->cycle[chnl_cycle_itr].lo_map_type    = ohfs_remove_field_CHAN_MAP_TYPE_get(NULL, ohfs_remove_handle, chnl_cycle_itr);     
        ohfs_remove_recover_reg->cycle[chnl_cycle_itr].lo_chnl_id     = ohfs_remove_field_LO_ID_get(NULL, ohfs_remove_handle, chnl_cycle_itr);
        ohfs_remove_recover_reg->cycle[chnl_cycle_itr].lo_last_ts     = ohfs_remove_field_LAST_TS_get(NULL, ohfs_remove_handle, chnl_cycle_itr);
        ohfs_remove_recover_reg->cycle[chnl_cycle_itr].lo_first_ts    = ohfs_remove_field_FIRST_TS_get(NULL, ohfs_remove_handle, chnl_cycle_itr);
        ohfs_remove_recover_reg->cycle[chnl_cycle_itr].lo_ts_valid    = ohfs_remove_field_TS_VALID_get(NULL, ohfs_remove_handle, chnl_cycle_itr);        
    }
    
    PMC_RETURN();
} /* ohfs_remove_reg_ctxt_recover */

/*******************************************************************************
* ohfs_remove_reg_ctxt_ho_chnl_cfg_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns the channel configuration based on the recovered 
*   register context 
*                                                                               
* INPUTS:                                                                       
*   *ohfs_remove_handle      - pointer to OHFS_REMOVE handle instance
*   *ohfs_remove_recover_reg - OHFS_REMOVE reg context 
*   chnl                     - Channel ID
*
* OUTPUTS:
*   *num_tribslot            - Number of trib slot for this channel 
*   *cal_array               - The calendar entries for this channel 
*
* RETURNS:
*   None.
*                                                                               
* NOTES:                                                                        
*                                                                               
****************************************************************************/
PUBLIC void ohfs_remove_reg_ctxt_ho_chnl_cfg_get(ohfs_remove_handle_t *ohfs_remove_handle, 
                                                 ohfs_remove_recover_var_t *ohfs_remove_recover_reg,
                                                 UINT32 chnl,
                                                 UINT8 *num_tribslot,
                                                 UINT8 cal_array[UTIL_GLOBAL_MAX_NUM_TS])
{
    UINT32 chnl_cycle_itr;
    UINT8 tribslot = 0;

    PMC_ENTRY();
    PMC_MEMSET(cal_array,0xFF,sizeof(UINT8)*UTIL_GLOBAL_MAX_NUM_TS);
    for(chnl_cycle_itr = 0;chnl_cycle_itr < OHFS_REMOVE_MAX_CHNL_NUM;chnl_cycle_itr++)
    {
        if (ohfs_remove_recover_reg->cycle[chnl_cycle_itr].ho_chnl_id == chnl)
        {
            /* if tribslot goes beyond 80, then assert */
            PMC_ASSERT(tribslot < UTIL_GLOBAL_MAX_NUM_TS, OHFS_REMOVE_ERR_CODE_ASSERT, 0, 0);

            cal_array[tribslot] = chnl_cycle_itr;
            tribslot += 1;  
        }        
    }
    *num_tribslot = tribslot;
    PMC_RETURN();
} /* ohfs_remove_reg_ctxt_ho_chnl_cfg_get */

/*******************************************************************************
* ohfs_remove_reg_ctxt_lo_chnl_cfg_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns the channel configuration based on the recovered 
*   register context 
*                                                                               
* INPUTS:                                                                       
*   *ohfs_remove_handle      - pointer to OHFS_REMOVE handle instance
*   *ohfs_remove_recover_reg - OHFS_REMOVE reg context 
*   lo_chnl                  - LO Channel ID
*
* OUTPUTS:
*   *num_tribslot  - Number of trib slot for this channel 
*   *mapping_mode  - The mapping mode for this channel 
*   *ho_chnl       - The HO channel associated to this channel
*   *cal_array     - The calendar entries for this channel 
*   *ts_array      - The lo tribslot entries for this channel 
*
* RETURNS:
*   None.
*                                                                               
* NOTES:                                                                        
*                                                                               
****************************************************************************/
PUBLIC void ohfs_remove_reg_ctxt_lo_chnl_cfg_get(ohfs_remove_handle_t *ohfs_remove_handle, 
                                                 ohfs_remove_recover_var_t *ohfs_remove_recover_reg,
                                                 UINT8 lo_chnl,
                                                 UINT8 *num_tribslot,
                                                 UINT8 *mapping_mode,
                                                 UINT8 *ho_chnl,
                                                 UINT8 cal_array[UTIL_GLOBAL_MAX_NUM_TS],
                                                 UINT8 ts_array[UTIL_GLOBAL_MAX_NUM_TS])
{
    UINT32 chnl_cycle_itr;
    UINT8 lo_tribslot = 0;
    UINT8 ho_tribslot = 0;

    PMC_ENTRY();
    PMC_MEMSET(cal_array,0xFF,sizeof(UINT8)*UTIL_GLOBAL_MAX_NUM_TS);
    PMC_MEMSET(ts_array,0xFF,sizeof(UINT8)*UTIL_GLOBAL_MAX_NUM_TS);
    for(chnl_cycle_itr = 0;chnl_cycle_itr < OHFS_REMOVE_MAX_CHNL_NUM;chnl_cycle_itr++)
    {
        if (ohfs_remove_recover_reg->cycle[chnl_cycle_itr].lo_ts_valid == 1 &&
            ohfs_remove_recover_reg->cycle[chnl_cycle_itr].lo_chnl_id == lo_chnl)
        {
            /* if tribslot goes beyond 80, then assert */
            PMC_ASSERT(lo_tribslot < UTIL_GLOBAL_MAX_NUM_TS, OHFS_REMOVE_ERR_CODE_ASSERT, 0, 0);

            cal_array[lo_tribslot] = chnl_cycle_itr;
            if (lo_tribslot == 0)
            {
                *ho_chnl = ohfs_remove_recover_reg->cycle[chnl_cycle_itr].ho_chnl_id;
                *mapping_mode  = ohfs_remove_recover_reg->cycle[chnl_cycle_itr].lo_map_type;
            }
            lo_tribslot +=1;       
        }        
    }
    *num_tribslot = lo_tribslot;

    /* search trib slot ID for this LO channel */
    lo_tribslot = 0;
    ho_tribslot = 0;    
    for(chnl_cycle_itr = 0;chnl_cycle_itr < OHFS_REMOVE_MAX_CHNL_NUM;chnl_cycle_itr++)
    {
        if (ohfs_remove_recover_reg->cycle[chnl_cycle_itr].ho_cycle_valid == 1 &&
            ohfs_remove_recover_reg->cycle[chnl_cycle_itr].ho_chnl_id == *ho_chnl )
        {
            if (ohfs_remove_recover_reg->cycle[chnl_cycle_itr].lo_ts_valid == 1 &&
                ohfs_remove_recover_reg->cycle[chnl_cycle_itr].lo_chnl_id == lo_chnl)
            {
                /* if tribslot goes beyond 80, then assert */
                PMC_ASSERT(lo_tribslot < UTIL_GLOBAL_MAX_NUM_TS, OHFS_REMOVE_ERR_CODE_ASSERT, 0, 0);

                ts_array[lo_tribslot] = ho_tribslot;
                lo_tribslot += 1;    
            }            
            ho_tribslot += 1;
        }        
    }
    
    PMC_RETURN();
} /* ohfs_remove_reg_ctxt_lo_chnl_cfg_get */

/*******************************************************************************
* ohfs_remove_msi_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets MSI configuration mode
*
*
* INPUTS:
*   *ohfs_remove_handle     - pointer to OHFS_REMOVE handle instance
*   msi_mode                - Enum for MSI configuration mode. See util_global_otn_msi_cfg_mode_t.
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
PUBLIC void ohfs_remove_msi_mode_set(ohfs_remove_handle_t *ohfs_remove_handle,
                                     util_global_otn_msi_cfg_mode_t msi_mode)
{
    PMC_ENTRY();
    
    /* be default, MSI mode is auto */
    ohfs_remove_handle->cfg.msi_cfg_mode = msi_mode;
        
    PMC_RETURN();
} /* ohfs_remove_msi_mode_set */

/*******************************************************************************
* ohfs_remove_msi_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets MSI configuration mode
*
*
* INPUTS:
*   *ohfs_remove_handle     - pointer to OHFS_REMOVE handle instance
*
* OUTPUTS:
*   *msi_mode               - current MSI configuration mode used in the device.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void ohfs_remove_msi_mode_get(ohfs_remove_handle_t *ohfs_remove_handle,
                                     util_global_otn_msi_cfg_mode_t *msi_mode)
{
    PMC_ENTRY();
    
    /* be default, MSI mode is auto */
    *msi_mode = ohfs_remove_handle->cfg.msi_cfg_mode;
        
    PMC_RETURN();
} /* ohfs_remove_msi_mode_get */

/*
** End of file
*/
