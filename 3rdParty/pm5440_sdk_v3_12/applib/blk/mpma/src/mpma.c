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
#include "mpma_loc.h"


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
/* Only include the strings if logging with text*/
#ifdef PMC_LOG_STRINGS

/* 
 * The following creates the LOG/ERR strings from above by pulling out the text
 * portion of the LOG_ERR_TABLE above 
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) str,
const char MPMA_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    MPMA_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char MPMA_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward References
*/
PRIVATE PMC_ERROR mpma_int_validate(mpma_handle_t *mpma_handle,
                                    UINT32 chnl_id,
                                    void *non_null_ptr);
PRIVATE void mpma_var_default_init(mpma_handle_t *hndl);
PRIVATE PMC_ERROR mpma_ch_init(mpma_handle_t *hndl, UINT32 chnl);
PRIVATE PMC_ERROR mpma_schd_ch_prov(mpma_handle_t *hndl, UINT32 chnl, 
                                    DOUBLE client_bitrate, 
                                    UINT32 scbs3_user_bits_3_0);
PRIVATE PMC_ERROR mpma_schd_ch_deprov(mpma_handle_t *hndl, UINT32 chnl);
PRIVATE void mpma_schd_ch_activate(mpma_handle_t *hndl, UINT32 chnl);
PRIVATE void mpma_schd_ch_deactivate(mpma_handle_t *hndl, UINT32 chnl,
                                     BOOL super_deprov);
PRIVATE void mpma_schd_ch_activate_super_prov(mpma_handle_t *hndl, UINT32 chnl);
PRIVATE PMC_ERROR mpma_ch_activate_internal(mpma_handle_t *hndl, UINT32 chnl, 
                                            BOOL dsp_datapath, BOOL8 super_prov);
PRIVATE PMC_ERROR mpma_ch_deactivate_internal(mpma_handle_t *hndl, UINT32 chnl,
                                              BOOL super_deprov);                                            
PRIVATE void mpma_cfc_pages_thres_set(mpma_handle_t *hndl, UINT32 chnl,
                                      DOUBLE client_bitrate);
PRIVATE PMC_ERROR mpma_ch_activate_helper(mpma_handle_t *hndl, UINT32 chnl);
PRIVATE void mpma_ch_enable_helper(mpma_handle_t *hndl, UINT32 chnl, BOOL dsp_datapath);
PRIVATE PMC_ERROR mpma_ch_deactivate_helper(mpma_handle_t *hndl, UINT32 chnl);
PRIVATE void mpma_ch_disable_helper(mpma_handle_t *hndl, UINT32 chnl);
                                        
PRIVATE PMC_ERROR mpma_non_default_csi_cfg(mpma_handle_t *hndl);
PRIVATE PMC_ERROR mpma_audit_var_data_register_get(mpma_handle_t *mpma_handle, 
                                                   mpma_var_t *var_ptr);
PRIVATE PMC_ERROR mpma_audit_ctxt_register_compare_clean(mpma_handle_t *mpma_handle,
                                                         mpma_var_t *ctxt_var_ptr,
                                                         mpma_var_t *reg_var_ptr,
                                                         util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                                         BOOL8 * mismatch_channels);
PRIVATE PMC_ERROR mpma_internal_ch_prov(mpma_handle_t *hndl, UINT32 chnl,
                                        DOUBLE client_bitrate,
                                        UINT32 pkt_size, 
                                        util_opsa_cn_frame_period_t cn_frame_period,
                                        util_opsa_pkt_period_calc_t pkt_period_calc, 
                                        UINT32 scbs3_user_bits_3_0, BOOL sch_only, 
                                        UINT32 pkt_per_int, UINT32 pkt_per_n,
                                        UINT32 pkt_per_d, DOUBLE epsilon,
                                        UINT32 corner_frequency);
PRIVATE BOOL8 mpma_non_rev_a_instance_type_test(mpma_handle_t *hndl);
/*
** Public Functions
*/

/*******************************************************************************
* mpma_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for an MPMA block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the MPMA subsystem relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifier of the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   mpma_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC mpma_handle_t *mpma_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    mpma_handle_t *mpma_handle;

    PMC_ENTRY();

    mpma_handle = (mpma_handle_t*)PMC_CTXT_CALLOC(sizeof(mpma_handle_t), parent);
    
    pmc_handle_init(parent, mpma_handle, sys_handle, PMC_MID_DIGI_MPMA, tsb_name, base_address);

    /* Register log strings. */
    pmc_log_block_strings_register(MPMA_LOG_ERR_STRINGS[0], MPMA_LOG_ERR_TABLE_BASE, MPMA_LOG_ERR_COUNT);
    pmc_log_block_strings_register(UTIL_OPSA_LOG_ERR_STRINGS[0], UTIL_OPSA_LOG_ERR_TABLE_BASE, UTIL_OPSA_LOG_ERR_COUNT);

    mpma_handle->cfc_handle   = cfc_ctxt_create((pmc_handle_t *)mpma_handle, base_address+BASE_ADDR_MPMA_MPMA_DP_CFC, sys_handle, "CFC");
    PMC_CTXT_REGISTER_PTR(&mpma_handle->cfc_handle, parent);
    mpma_handle->scbs3_handle = scbs3_ctxt_create((pmc_handle_t *)mpma_handle, base_address+BASE_ADDR_MPMA_MPMA_DP_SCBS3, sys_handle, "SCBS3");
    PMC_CTXT_REGISTER_PTR(&mpma_handle->scbs3_handle, parent);

    mpma_handle->var.ch_cfg_ptr = NULL;

    PMC_RETURN(mpma_handle);
} /* mpma_ctxt_create */

 
/*******************************************************************************
* mpma_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a MPMA block instance.
*
*
* INPUTS:
*   *mpma_handle         - pointer to MPMA handle instance
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
PUBLIC void mpma_ctxt_destroy(mpma_handle_t *mpma_handle)
{
    PMC_ENTRY();

    cfc_ctxt_destroy(mpma_handle->cfc_handle);
    scbs3_ctxt_destroy(mpma_handle->scbs3_handle);
    
    if (mpma_handle->cfg.default_param.db_handle != 0)
    {
        gen_db_detach(&(mpma_handle->base), 
                      mpma_handle->cfg.default_param.db_id,
                      mpma_handle->cfg.default_param.db_handle);
    }

    PMC_CTXT_FREE(&(mpma_handle->var.ch_cfg_ptr), mpma_handle);

    PMC_CTXT_FREE(&mpma_handle, mpma_handle);

    PMC_RETURN();
} /* mpma_ctxt_destroy */

/*******************************************************************************
* mpma_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an MPMA block handle instance.
*
*
* INPUTS:
*   *mpma_handle       - pointer to MPMA handle instance
*   mpma_type_id       - MPMA ID of this instance
*   scbs3_toc_sync     - Entry that corresponds to the top of calendar entry. \n
*                        toc_sync[6:2] defines the calendar memory row. \n
*                        toc_sync[1:0] defines the entry within the row. \n
*                        This input is only used for MPMA_MAPOTN. Set to 0 if 
*                        the MPMA instance does not need to synchronize to
*                        top of calendar.  
*
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
PUBLIC void mpma_handle_init(mpma_handle_t *mpma_handle, 
                             mpma_type_id_t mpma_type_id, UINT32 scbs3_toc_sync)
                             
{
    UINT32      size;
    cfc_cfg_t   cfc_cfg;
    scbs3_cfg_t scbs_cfg;
    UINT32 i; /* Counter */
    
    PMC_ENTRY();

    /* Check for valid type ID */
    if (mpma_type_id >= MPMA_NUM_TYPES) {
        PMC_DEBUG(MPMA_LOG_ERR_INVALID_PARAMETERS, mpma_type_id, 0);
        PMC_RETURN();
    }
    
    /* Update static configurations */
    mpma_handle->cfg.default_param.mpma_type_id = mpma_type_id;
    mpma_handle->cfg.default_param.num_chnl     = MPMA_NUM_CHNL;
    mpma_handle->cfg.default_param.bus_width    = MPMA_BUS_WIDTH_48_BYTE;
    mpma_handle->cfg.default_param.lowest_bw    = MPMA_LOWEST_BANDWIDTH;
    mpma_handle->cfg.default_param.ts_enable    = FALSE;
    mpma_handle->cfg.default_param.mkr_mode     = MPMA_MKR_MODE;
    mpma_handle->cfg.default_param.cn_bit_mode  = 0;
    mpma_handle->cfg.default_param.cal_entries_alloc_method = MPMA_DEFAULT_CAL_ENTRIES_ALLOC_METHOD ; /* as default always use algorithm #1 */
    mpma_handle->cfg.default_param.db_id        = LAST_UTIL_GEN_DB;
    mpma_handle->cfg.default_param.pkt_size_range = MPMA_DEFAULT_PKT_SIZE_RANGE;
    mpma_handle->cfg.default_param.cn_frame_period = UTIL_OPSA_T_FRAME_UNUSED;
    mpma_handle->cfg.default_param.schd_pacer_en = FALSE;
    
    /* CFC parameters */
    /* Number of FIFO channels in CFC. Equivalent to channel count. */
    mpma_handle->cfg.cfc_cfg.max_fifo_num = MPMA_NUM_CHNL;

    /* SCBS3 parameters */
    mpma_handle->cfg.scbs3_cfg.scbs3_present    = TRUE;
    mpma_handle->cfg.scbs3_cfg.total_rate       = (UINT32)MPMA_SCBS3_TOTAL_RATE;
    mpma_handle->cfg.scbs3_cfg.total_rate_units = MPMA_SCBS3_TOTAL_RATE_UNITS;
    mpma_handle->cfg.scbs3_cfg.page_swap_mode   = SCBS3_PAGE_SWAP_BIT;
    
    /* Configure parameters for specific instantiations. */
    switch (mpma_type_id) {
    case MPMA_COREOTN:
        mpma_handle->cfg.default_param.cn_mode = 0;    
        mpma_handle->cfg.default_param.cn_enable = FALSE;  
        mpma_handle->cfg.default_param.cnd_enable = TRUE; 
        
        mpma_handle->cfg.default_param.ts_enable  = TRUE;
        mpma_handle->cfg.cfc_cfg.page_size          = MPMA_COREOTN_CFC_PAGE_SIZE;
        mpma_handle->cfg.default_param.lowest_bw    = MPMA_LOWEST_BANDWIDTH;
        mpma_handle->cfg.default_param.cal_entries_alloc_method = MPMA_COREOTN_DB_CAL_ENTRIES_ALLOC_METHOD;
        mpma_handle->cfg.default_param.db_id     = UTIL_GEN_DB_LINE_CORE_OTN_RX_DB;
         
        /* SCBS3 TOC_SYNC data - slave scheduler */
        mpma_handle->cfg.scbs3_cfg.toc_sync_en = FALSE;
        mpma_handle->cfg.scbs3_cfg.toc_sync = 0;
        mpma_handle->cfg.default_param.bwr_ind_sel = 0; /* from input port */
        break;
    case MPMA_COREOTN_FROM_MAPOTN: 
        mpma_handle->cfg.default_param.cn_mode = 0;    
        mpma_handle->cfg.default_param.cn_enable = FALSE;  
        mpma_handle->cfg.default_param.cnd_enable = FALSE;
       
        mpma_handle->cfg.default_param.ts_enable  = TRUE;
        mpma_handle->cfg.cfc_cfg.page_size        = MPMA_COREOTN_CFC_PAGE_SIZE;
        mpma_handle->cfg.default_param.lowest_bw    = MPMA_LOWEST_BANDWIDTH;
        mpma_handle->cfg.default_param.cal_entries_alloc_method = MPMA_COREOTN_FROM_MAPOTN_DB_CAL_ENTRIES_ALLOC_METHOD;
        mpma_handle->cfg.default_param.db_id     = UTIL_GEN_DB_MPMA_DB;
        
        /* SCBS3 TOC_SYNC data - slave scheduler */
        mpma_handle->cfg.scbs3_cfg.toc_sync_en = FALSE;
        mpma_handle->cfg.scbs3_cfg.toc_sync = 0;
        mpma_handle->cfg.default_param.bwr_ind_sel = 1; /* from register */
        break;
    case MPMA_COREOTN_CTL:
        /* 12 channel version */
        mpma_handle->cfg.default_param.num_chnl     = MPMA_NUM_CHNL_REDUCED;
        mpma_handle->cfg.cfc_cfg.max_fifo_num       = MPMA_NUM_CHNL_REDUCED;
        
        mpma_handle->cfg.default_param.cn_mode      = 0;    
        mpma_handle->cfg.default_param.cn_enable    = FALSE;  
        mpma_handle->cfg.default_param.cnd_enable   = FALSE;
        
        mpma_handle->cfg.default_param.ts_enable    = TRUE;
        mpma_handle->cfg.cfc_cfg.page_size          = MPMA_COREOTN_CTL_CFC_PAGE_SIZE;
        mpma_handle->cfg.default_param.cal_entries_alloc_method = MPMA_MPMO_COREOTN_CTL_DB_CAL_ENTRIES_ALLOC_METHOD;
        mpma_handle->cfg.default_param.db_id        = UTIL_GEN_DB_LINEOTN_SYSOTN_MODE;
        
        /* SCBS3 TOC_SYNC data - slave scheduler */
        mpma_handle->cfg.scbs3_cfg.toc_sync_en      = FALSE;
        mpma_handle->cfg.scbs3_cfg.toc_sync         = 0;
        
        mpma_handle->cfg.default_param.lowest_bw    = MPMA_LOWEST_BANDWIDTH_REDUCED;
        break;
    case MPMA_MAPOTN:
        mpma_handle->cfg.default_param.cn_mode = 0;    
        mpma_handle->cfg.default_param.cn_enable = TRUE;  
        mpma_handle->cfg.default_param.cnd_enable = FALSE;
        mpma_handle->cfg.default_param.pkt_size_range = MPMA_MAPOTN_PKT_SIZE_RANGE;
        mpma_handle->cfg.cfc_cfg.page_size       = MPMA_MAPOTN_CFC_PAGE_SIZE;
        mpma_handle->cfg.default_param.lowest_bw    = MPMA_LOWEST_BANDWIDTH;
        mpma_handle->cfg.default_param.cal_entries_alloc_method = MPMA_DB_CAL_ENTRIES_ALLOC_METHOD;
        mpma_handle->cfg.default_param.db_id     = UTIL_GEN_DB_MPMA_DB;
        
        /* SCBS3 TOC_SYNC data - needs to sync to TOC input pin */
        mpma_handle->cfg.scbs3_cfg.toc_sync_en = TRUE;         /* MPMA in MAPOTN needs to sync to TOC. */
        mpma_handle->cfg.scbs3_cfg.toc_sync = scbs3_toc_sync;  /* MPMA in MAPOTN needs to sync to 'scbs3_toc_sync'. */
        break;
    case MPMA_ENET:
    case MPMA_ENET_LINE_DB:
    case MPMA_ENET_SYS_DB:
        /* 12 channel version */
        mpma_handle->cfg.default_param.num_chnl     = MPMA_NUM_CHNL_REDUCED;
        mpma_handle->cfg.cfc_cfg.max_fifo_num = MPMA_NUM_CHNL_REDUCED;
        mpma_handle->cfg.default_param.cn_mode = 0;    
        mpma_handle->cfg.default_param.cn_enable = FALSE;  
        mpma_handle->cfg.default_param.cnd_enable = FALSE;
        
        mpma_handle->cfg.default_param.pkt_size_range = MPMA_CBRC_ENET_PKT_SIZE_RANGE;
        
        mpma_handle->cfg.cfc_cfg.page_size = MPMA_ENET_CFC_PAGE_SIZE;
        
        /* SCBS3 TOC_SYNC data */
        mpma_handle->cfg.scbs3_cfg.toc_sync_en = TRUE;
        mpma_handle->cfg.scbs3_cfg.toc_sync = scbs3_toc_sync;
        
        if(mpma_type_id == MPMA_ENET_LINE_DB)
        {
            /* database settings for MPMA in ENET */
            mpma_handle->cfg.default_param.cal_entries_alloc_method = UTIL_SCHD_ALLOC_METHOD_ENET_LINE_MPMA_DB;
            mpma_handle->cfg.default_param.db_id     = UTIL_GEN_DB_ENET_LINE_MPMA;
        } else if(mpma_type_id == MPMA_ENET_SYS_DB)
        {
            /* database settings for MPMA in ENET */
            mpma_handle->cfg.default_param.cal_entries_alloc_method = UTIL_SCHD_ALLOC_METHOD_ENET_SYS_MPMA_DB;
            mpma_handle->cfg.default_param.db_id     = UTIL_GEN_DB_ENET_SYS_MPMA;
        }
        
        mpma_handle->cfg.default_param.lowest_bw    = MPMA_LOWEST_BANDWIDTH_REDUCED;
        break;
    case MPMA_CBRC:
        /* 12 channel version */
        mpma_handle->cfg.default_param.num_chnl     = MPMA_NUM_CHNL_REDUCED;
        mpma_handle->cfg.cfc_cfg.max_fifo_num       = MPMA_NUM_CHNL_REDUCED;
        mpma_handle->cfg.default_param.cn_mode      = 0;    
        mpma_handle->cfg.default_param.cn_enable    = FALSE;  
        mpma_handle->cfg.default_param.cnd_enable   = FALSE;
        
        mpma_handle->cfg.default_param.pkt_size_range = MPMA_CBRC_ENET_PKT_SIZE_RANGE;
        
        mpma_handle->cfg.scbs3_cfg.scbs3_present = FALSE;
        mpma_handle->cfg.cfc_cfg.page_size       = MPMA_CBRC_CFC_PAGE_SIZE;
        
        /* SCBS3 TOC_SYNC data */
        mpma_handle->cfg.scbs3_cfg.toc_sync_en = FALSE; 
        mpma_handle->cfg.scbs3_cfg.toc_sync = 0; 
        
        mpma_handle->cfg.default_param.lowest_bw    = MPMA_LOWEST_BANDWIDTH_REDUCED;
        break;
    default:
        PMC_DEBUG(MPMA_LOG_ERR_INVALID_PARAMETERS, mpma_type_id, 0);
        break;
    } /* end switch */
    
    size = sizeof(mpma_ch_cfg_struct_t) * mpma_handle->cfg.default_param.num_chnl;
    mpma_handle->var.ch_cfg_ptr = (mpma_ch_cfg_struct_t*)PMC_CTXT_CALLOC(size, mpma_handle);
    PMC_CTXT_REGISTER_PTR(&mpma_handle->var.ch_cfg_ptr, mpma_handle);

    /* Initialize the channels such that all use the full version of MPMA by
       default */
    for (i = 0; i < mpma_handle->cfg.default_param.num_chnl; i++) {
        mpma_handle->var.ch_cfg_ptr[i].sch_only = FALSE;
    }
    
    /* access DB except for method 1,2 and 6 */
    if( (mpma_handle->cfg.default_param.cal_entries_alloc_method > 2 &&
        mpma_handle->cfg.default_param.cal_entries_alloc_method < 6) ||
         (mpma_handle->cfg.default_param.cal_entries_alloc_method > 6) )
    {
        /* attach to the databse */
        mpma_handle->cfg.default_param.db_handle = gen_db_attach(&(mpma_handle->base),mpma_handle->cfg.default_param.db_id);
        
    } 
    
    /* Initialize var_t parameters */
    mpma_var_default_init(mpma_handle);

    /* Initialize CFC */
    cfc_cfg.max_fifo_num = mpma_handle->cfg.cfc_cfg.max_fifo_num;
    cfc_cfg.page_size    = mpma_handle->cfg.cfc_cfg.page_size;
    cfc_handle_init(mpma_handle->cfc_handle, &cfc_cfg);

    /* Initialize SCBS3 if present */
    if (mpma_handle->cfg.scbs3_cfg.scbs3_present) {
        scbs_cfg.num_grp             = MPMA_SCBS3_NUM_GRP;
        scbs_cfg.max_rows            = MPMA_SCBS3_MAX_ROWS;
        scbs_cfg.bonus_mode_enabled  = MPMA_SCBS3_BONUS_MODE;
        scbs_cfg.enable_row_resizing = MPMA_SCBS3_ROW_RESIZE;
    
     /* Keep 96 entries regardless of MPMA instantiation */    
        scbs_cfg.max_chnl          = MPMA_NUM_CHNL;  

        scbs_cfg.total_rate        = mpma_handle->cfg.scbs3_cfg.total_rate;
        scbs_cfg.total_rate_units  = mpma_handle->cfg.scbs3_cfg.total_rate_units;
        scbs_cfg.toc_sync          = mpma_handle->cfg.scbs3_cfg.toc_sync;
        scbs_cfg.toc_sync_en       = mpma_handle->cfg.scbs3_cfg.toc_sync_en;
        scbs_cfg.page_swap_mode    = mpma_handle->cfg.scbs3_cfg.page_swap_mode;
        scbs_cfg.cal_entries_alloc_method = mpma_handle->cfg.default_param.cal_entries_alloc_method;
        scbs_cfg.db_id = mpma_handle->cfg.default_param.db_id;
        
        scbs3_handle_init(mpma_handle->scbs3_handle, &scbs_cfg);
    }

    PMC_RETURN();
} /* mpma_handle_init */


/*******************************************************************************
* Initialization Configuration Functions
*******************************************************************************/
/*******************************************************************************
*  mpma_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize MPMA with parameters specific to this instantiation.
*
* INPUTS:
*   *hndl        - pointer to MPMA handle instance
*   energy_state - enum defining state to set the MPMA module to
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   This function uses pmc_energy_state_t for defining its energy states as there 
*   are no optimization in energy state required for MPMA module
*
*******************************************************************************/
PUBLIC PMC_ERROR mpma_init(mpma_handle_t *hndl, 
                           pmc_energy_state_t energy_state)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 chnl;
    
    PMC_ENTRY();
    PMC_ASSERT(hndl!=NULL, MPMA_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        /* Disable MPMA */
        mpma_enable_cfg(hndl, FALSE);
        /* Disable all MPMA channels */
        for (chnl = 0; chnl < hndl->cfg.default_param.num_chnl; chnl++) {
            mpma_ch_enable_cfg(hndl, chnl, FALSE);
        }
        
        /* Apply non-default CSI values as defined in OIF Recommendation */
        mpma_non_default_csi_cfg(hndl);
    
        /* Configure MPMA parameters specific to each instantiation */
        mpma_field_BUS_WIDTH_set(NULL, hndl, hndl->cfg.default_param.bus_width);

        mpma_field_BWR_IND_SEL_set(NULL, hndl, hndl->cfg.default_param.bwr_ind_sel);

        /* DSP configuration */
        mpma_dsp_field_CLIENT_WIDTH_set(NULL, hndl, MPMA_CLIENT_WIDTH);
      
        if(hndl->cfg.default_param.mpma_type_id != MPMA_COREOTN_CTL)
        {
            /* Configure PSD FIFO full threshold */
            mpma_field_PSD_FIFO_FULL_THRES_set(NULL, hndl, (hndl->cfg.default_param.cn_enable == TRUE)?32:28);
        }
        else
        {
            /* Configure PSD FIFO full threshold */
            mpma_field_PSD_FIFO_FULL_THRES_set(NULL, hndl, 12);
        }
    
        /* Initialize IPT in CFC */
        ret_val = cfc_init(hndl->cfc_handle, energy_state);
        if(PMC_SUCCESS == ret_val)
        {    
            ret_val = cfc_bypass(hndl->cfc_handle, FALSE);
        }
    
        /* Initialize SCBS3 scheduler */
        if (PMC_SUCCESS == ret_val && hndl->cfg.scbs3_cfg.scbs3_present) {
            ret_val = scbs3_schd_init(hndl->scbs3_handle, energy_state);
        }
    }
    else if (PMC_ENERGY_STATE_RESET == energy_state)
    {
        mpma_var_default_init(hndl);
        ret_val = cfc_init(hndl->cfc_handle, energy_state);
        
        if (PMC_SUCCESS == ret_val && hndl->cfg.scbs3_cfg.scbs3_present) {
            ret_val = scbs3_schd_init(hndl->scbs3_handle, energy_state);
        }
    } else {
        PMC_ASSERT(FALSE, MPMA_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }

    PMC_RETURN(ret_val);

} /* mpma_init */


/*******************************************************************************
* mpma_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   MPMA instance.  It will test the channel state for all resources within
*   the MPMA instance if any are not in MPMA_CHNL_START state then FALSE
*   is returned, else TRUE.  Should TRUE be returned it can be assumed that
*   no resources are provisioned against the MPMA and it would be
*   possible for the higher layer module to put the module in lowpower standby
*   mode of operation.
*
*
* INPUTS:
*   *hndl          - pointer to MPMA handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL           - FALSE:  Resource(s) not in start state \n
*                    TRUE:   Resources all in start state
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL mpma_start_state_test(mpma_handle_t *hndl) 
{
    UINT32 chnl;
    PMC_ENTRY();
    PMC_ASSERT(hndl!=NULL, MPMA_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    for (chnl = 0; chnl < hndl->cfg.default_param.num_chnl; chnl++) 
    {
        if(hndl->var.ch_cfg_ptr[chnl].state != MPMA_CHNL_START)
           PMC_RETURN(FALSE);
    } 
    
    if(cfc_start_state_test(hndl->cfc_handle) == FALSE ||
       scbs3_start_state_test(hndl->scbs3_handle) == FALSE)
        PMC_RETURN(FALSE);
    
        
    PMC_RETURN(TRUE);

} /* mpma_start_state_test */

/*******************************************************************************
* mpma_enable_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/disables the packet maker functionality.
*
* INPUTS:
*   *hndl    - pointer to MPMA handle instance
*   enable   - TRUE: enable MPMA. FALSE: disable MPMA.
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
PUBLIC PMC_ERROR mpma_enable_cfg(mpma_handle_t *hndl, BOOL enable)
{
    PMC_ENTRY();
    
    /* Enable/disable MPMA */
    mpma_field_MPMA_EN_set(NULL, hndl, enable);
    
    PMC_RETURN(PMC_SUCCESS);
} /* mpma_enable_cfg */

/*******************************************************************************
* mpma_enable_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get whether the packet maker functionality is enabled or disabled.
*
* INPUTS:
*   *hndl    - pointer to MPMA handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8    - TRUE: enable MPMA \n
*              FALSE: disable MPMA
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 mpma_enable_get(mpma_handle_t *hndl)
{
    BOOL8 enable;
    PMC_ENTRY();
    
    enable = mpma_field_MPMA_EN_get(NULL, hndl);
    
    PMC_RETURN(enable);
} /* mpma_enable_get */


/*******************************************************************************
*  mpma_csi_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure CSI values.
*
* INPUTS:
*   *hndl    - pointer to MPMA handle instance
*   csi_type - CSI type to configure
*   csi_val  - value to use
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   Defaults values: \n
*     csi_type            value \n
*     MPMA_CSI_NORMAL     0x0C \n
*     MPMA_CSI_SSF        0x18 \n
*     MPMA_CSI_SF         0x14 \n
*     MPMA_CSI_SD         0x10 \n
*     MPMA_CSI_FORCE_TO   0x08 \n
*     MPMA_CSI_FORCE_AWAY 0x1C
*
*******************************************************************************/
PUBLIC PMC_ERROR mpma_csi_cfg(mpma_handle_t *hndl, mpma_csi_type_t csi_type, UINT32 csi_val)
{
    PMC_ENTRY();
    
    /* Configure specified value */
    switch (csi_type) {
    case MPMA_CSI_NORMAL:
        mpma_field_CSI_NORMAL_set(NULL, hndl, csi_val);
        break;
    case MPMA_CSI_SSF:
        mpma_field_CSI_SSF_set(NULL, hndl, csi_val);
        break;
    case MPMA_CSI_SF:
        mpma_field_CSI_SF_set(NULL, hndl, csi_val);
        break;
    case MPMA_CSI_SD:
        mpma_field_CSI_SD_set(NULL, hndl, csi_val);
        break;
    case MPMA_CSI_FORCE_TO:
        mpma_field_CSI_FORCE_TO_set(NULL, hndl, csi_val);
        break;
    case MPMA_CSI_FORCE_AWAY:
        mpma_field_CSI_FORCE_AWAY_set(NULL, hndl, csi_val);
        break;
    default:
        PMC_RETURN(MPMA_LOG_ERR_INVALID_PARAMETERS); 
        break;
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mpma_csi_cfg */


/*******************************************************************************
*  mpma_ch_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provisions a channel for the specified rate.
*   This function determines the parameters to configure for the MPMA datapath
*   and DSP. It also evaluates the scheduler to see if the specified rate can
*   be allocated.
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - channel to provision
*   client_bitrate    - client bit rate (in Hz)
*   pkt_size          - desired minimum packet size. 
*                       Set pkt_size to 0 to use internal defaults.
*   cn_frame_period   - parent Cn frame period that the client is demapped 
*                       from. Only required for cn_mode = 0 (source node  
*                       reference clock timing (B + C mode)).
*                       Set to UTIL_OPSA_T_FRAME_UNUSED if unused.
*   pkt_period_calc   - method for calculating packet period and size. \n
*                       When using UTIL_OPSA_USE_OIF_xxx_BYTE, 
*                       pkt_size should be set within the valid OIF range,
*                       and pkt_size_range is ignored.
*   scbs3_user_bits_3_0 - SCBS3 userbits [3:0] to output with the channel
*                         during the calendar slot.
*                         These bits might be used as downstream MUX 
*                         select bits (for bus aggregation).
*                         Set to 0 if unused.
*   sch_only          - set to TRUE if this channel is to utilize the MPMA
*                       scheduler functionality only (light) or FALSE for
*                       full MPMA with CFC FIFOs
*   pkt_per_int       - configured packet size
*   pkt_per_n         - configured numerator of packet period fraction
*   pkt_per_d         - configured denominator of packet period fraction
*   epsilon           - minimum epsilon
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   There are two stages to enable a channel: \n
*   1. provision/deprovision: whereby the resources (CFC pages, scheduler 
*      entries) are allocated \n
*   2. activate/deactivate: whereby the settings are written to hardware and 
*      the per channel enable bit is set
*
*******************************************************************************/
PUBLIC PMC_ERROR mpma_ch_prov(mpma_handle_t *hndl, UINT32 chnl,
                              DOUBLE client_bitrate,
                              UINT32 pkt_size, 
                              util_opsa_cn_frame_period_t cn_frame_period,
                              util_opsa_pkt_period_calc_t pkt_period_calc, 
                              UINT32 scbs3_user_bits_3_0, BOOL sch_only, 
                              UINT32 pkt_per_int, UINT32 pkt_per_n,
                              UINT32 pkt_per_d, DOUBLE epsilon)                              
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* hard codes corner frequency to 50 */
    ret_val = mpma_internal_ch_prov(hndl, chnl,
                                    client_bitrate,
                                    pkt_size, 
                                    cn_frame_period,
                                    pkt_period_calc, 
                                    scbs3_user_bits_3_0, sch_only, 
                                    pkt_per_int, pkt_per_n,
                                    pkt_per_d, epsilon, 50);   
    
    PMC_RETURN(ret_val);

} /* mpma_ch_prov */

/*******************************************************************************
*  mpma_fo2_ch_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provisions a channel for the specified rate.
*   This function determines the parameters to configure for the MPMA datapath
*   and DSP. It also evaluates the scheduler to see if the specified rate can
*   be allocated.
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - channel to provision
*   client_bitrate    - client bit rate (in Hz)
*   pkt_size          - desired minimum packet size. 
*                       Set pkt_size to 0 to use internal defaults.
*   cn_frame_period   - parent Cn frame period that the client is demapped 
*                       from. Only required for cn_mode = 0 (source node  
*                       reference clock timing (B + C mode)).
*                       Set to UTIL_OPSA_T_FRAME_UNUSED if unused.
*   pkt_period_calc   - method for calculating packet period and size. \n
*                       When using UTIL_OPSA_USE_OIF_xxx_BYTE, 
*                       pkt_size should be set within the valid OIF range,
*                       and pkt_size_range is ignored.
*   scbs3_user_bits_3_0 - SCBS3 userbits [3:0] to output with the channel
*                         during the calendar slot.
*                         These bits might be used as downstream MUX 
*                         select bits (for bus aggregation).
*                         Set to 0 if unused.
*   sch_only          - set to TRUE if this channel is to utilize the MPMA
*                       scheduler functionality only (light) or FALSE for
*                       full MPMA with CFC FIFOs
*   pkt_per_int       - configured packet size
*   pkt_per_n         - configured numerator of packet period fraction
*   pkt_per_d         - configured denominator of packet period fraction
*   epsilon           - minimum epsilon
*   bmp_map_mode      - indication if MPMA is used for BMP mapping
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   There are two stages to enable a channel: \n
*   1. provision/deprovision: whereby the resources (CFC pages, scheduler 
*      entries) are allocated  \n
*   2. activate/deactivate: whereby the settings are written to hardware and 
*      the per channel enable bit is set
*
*******************************************************************************/
PUBLIC PMC_ERROR mpma_fo2_ch_prov(mpma_handle_t *hndl, UINT32 chnl,
                                  DOUBLE client_bitrate,
                                  UINT32 pkt_size, 
                                  util_opsa_cn_frame_period_t cn_frame_period,
                                  util_opsa_pkt_period_calc_t pkt_period_calc, 
                                  UINT32 scbs3_user_bits_3_0, BOOL sch_only, 
                                  UINT32 pkt_per_int, UINT32 pkt_per_n,
                                  UINT32 pkt_per_d, DOUBLE epsilon,
                                  BOOL8 bmp_map_mode)                              
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    UINT32 corner_frequency = bmp_map_mode == TRUE ? 12:50;

    /* This API call is only valid for FO2 MPMA instance */
    PMC_ASSERT(hndl->cfg.default_param.mpma_type_id == MPMA_COREOTN_FROM_MAPOTN, MPMA_ERR_CODE_ASSERT, 0, 0);

    /* hard codes corner frequency to 50 */
    ret_val = mpma_internal_ch_prov(hndl, chnl,
                                    client_bitrate,
                                    pkt_size, 
                                    cn_frame_period,
                                    pkt_period_calc, 
                                    scbs3_user_bits_3_0, sch_only, 
                                    pkt_per_int, pkt_per_n,
                                    pkt_per_d, epsilon, corner_frequency);   
    
    PMC_RETURN(ret_val);

} /* mpma_fo2_ch_prov */


/*******************************************************************************
*  mpma_ch_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovisions the specified channel.
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - channel to deprovision
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
PUBLIC PMC_ERROR mpma_ch_deprov(mpma_handle_t *hndl, UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (hndl->var.ch_cfg_ptr[chnl].state != MPMA_CHNL_EQUIPPED) {

        ret_val = MPMA_LOG_CODE_INVALID_CHANNEL_STATE;

    } else {

        ret_val = mpma_schd_ch_deprov(hndl, chnl);
    }

    PMC_RETURN(ret_val);

} /* mpma_ch_deprov */

/*******************************************************************************
*  mpma_ch_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates a provisioned channel.
*   This function writes the calculated parameters for the provisioned channel 
*   to hardware.
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - channel to activate
*   dsp_datapath      - TRUE: enable MPMA DSP. FALSE: disable MPMA DSP.
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
PUBLIC PMC_ERROR mpma_ch_activate(mpma_handle_t *hndl, UINT32 chnl, BOOL dsp_datapath)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    
    ret_val = mpma_ch_activate_internal(hndl, chnl, dsp_datapath, FALSE);
    
    PMC_RETURN(ret_val);
} /* mpma_ch_activate */

/*******************************************************************************
*  mpma_ch_activate_super_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates a provisioned channel.
*   This function writes the calculated parameters for the provisioned channel 
*   to hardware.
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - channel to activate
*   dsp_datapath      - TRUE: enable MPMA DSP. FALSE: disable MPMA DSP.
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
PUBLIC PMC_ERROR mpma_ch_activate_super_prov(mpma_handle_t *hndl, UINT32 chnl, BOOL dsp_datapath)
{
    PMC_ERROR ret_val= PMC_SUCCESS;

    PMC_ENTRY();
    
    ret_val = mpma_ch_activate_internal(hndl, chnl, dsp_datapath, TRUE);
    
    PMC_RETURN(ret_val);
} /* mpma_ch_activate_super_prov */
    
    
/*******************************************************************************
*  mpma_ch_activate_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates a provisioned channel.
*   This function writes the calculated parameters for the provisioned channel 
*   to hardware.
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - channel to activate
*   dsp_datapath      - TRUE: enable MPMA DSP. FALSE: disable MPMA DSP.
*   super_prov        - boolean value to determine if this is used for COREOTN 
*                       super prov mode
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
PRIVATE PMC_ERROR mpma_ch_activate_internal(mpma_handle_t *hndl, UINT32 chnl, 
                                            BOOL dsp_datapath, BOOL8 super_prov)
{
    PMC_ERROR ret_val= PMC_SUCCESS;

    PMC_ENTRY();

    if (hndl->var.ch_cfg_ptr[chnl].state != MPMA_CHNL_EQUIPPED) {
        PMC_RETURN(MPMA_LOG_CODE_INVALID_CHANNEL_STATE);
    }

    /* If the channel was not set in schedule only mode, 
       perform typical MPMA activation */
    if (hndl->var.ch_cfg_ptr[chnl].sch_only == FALSE) {
            
        ret_val = mpma_ch_activate_helper(hndl, chnl);

        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }

    /* All channels must be scheduled regardless of scheduler mode */
    if (super_prov)
    {
        mpma_schd_ch_activate_super_prov(hndl, chnl);
    }
    else {
        mpma_schd_ch_activate(hndl, chnl);
    }        
        
    /* If the channel was not set in schedule only mode, 
       perform typical MPMA activation */
    if (hndl->var.ch_cfg_ptr[chnl].sch_only == FALSE) {

        mpma_ch_enable_helper(hndl, chnl, dsp_datapath);
    }

    /* Update channel state */
    hndl->var.ch_cfg_ptr[chnl].state = MPMA_CHNL_OPERATIONAL;

    PMC_RETURN(PMC_SUCCESS);

} /* mpma_ch_activate_internal */

/*******************************************************************************
*  mpma_ch_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates a provisioned channel.
*   This function disables the specified channel, and frees the FIFO and 
*   calendar resources used by the channel.
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - channel to deactivate
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
PUBLIC PMC_ERROR mpma_ch_deactivate(mpma_handle_t *hndl, UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    ret_val = mpma_ch_deactivate_internal(hndl, chnl, FALSE);

    PMC_RETURN(ret_val);
} /* mpma_ch_deactivate */  

/*******************************************************************************
*  mpma_ch_deactivate_super_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates a provisioned channel.
*   This function disables the specified channel, and frees the FIFO and 
*   calendar resources used by the channel.
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - channel to deactivate
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
PUBLIC PMC_ERROR mpma_ch_deactivate_super_prov(mpma_handle_t *hndl, UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    ret_val = mpma_ch_deactivate_internal(hndl, chnl, TRUE);

    PMC_RETURN(ret_val);
} /* mpma_ch_deactivate_super_prov */                                                     

/*******************************************************************************
*  mpma_ch_deactivate_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates a provisioned channel.
*   This function disables the specified channel, and frees the FIFO and 
*   calendar resources used by the channel.
*
* INPUTS:
*   *hndl              - pointer to MPMA handle instance
*   chnl              - channel to deactivate
*   super_deprov      - boolean to indicate if this function is used in
*                       bulk deprovisioning mode.
*                       TRUE: bulk deprovisioning
*                       FALSE: normal deprovisioning
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
PRIVATE PMC_ERROR mpma_ch_deactivate_internal(mpma_handle_t *hndl, UINT32 chnl,
                                              BOOL super_deprov)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();

    if (hndl->var.ch_cfg_ptr[chnl].state != MPMA_CHNL_OPERATIONAL) {
        PMC_RETURN(MPMA_LOG_CODE_INVALID_CHANNEL_STATE);
    }

    /* If the channel was not set in schedule only mode, 
       perform typical MPMA deactivation */
    if (hndl->var.ch_cfg_ptr[chnl].sch_only == FALSE) {
            
        mpma_ch_disable_helper(hndl, chnl);
    }

    /* Update channel state */
    hndl->var.ch_cfg_ptr[chnl].state = MPMA_CHNL_EQUIPPED;
    /* All channels must be descheduled regardless of scheduler mode */
    mpma_schd_ch_deactivate(hndl, chnl, super_deprov);

    /* If the channel was not set in schedule only mode, 
       perform typical MPMA deactivation */
    if (hndl->var.ch_cfg_ptr[chnl].sch_only == FALSE) {
            
        ret_val = mpma_ch_deactivate_helper(hndl, chnl);
    }

    PMC_RETURN(ret_val);

} /* mpma_ch_deactivate_internal */

/*******************************************************************************
*  mpma_ch_clean
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clean a set of channels. Memory context and register space are cleaned
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - set of channels to clean
*   num_chnl          - number of channels to clean
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
PUBLIC void mpma_ch_clean(mpma_handle_t *hndl, UINT32 *chnl, UINT32 num_chnl)
{
    UINT32 chnl_itr;    
    
    PMC_ENTRY();

    for (chnl_itr = 0; chnl_itr < num_chnl;chnl_itr ++)
    {
        /* disable channels*/    
        mpma_ch_disable_helper(hndl, chnl[chnl_itr]);
        /* Update channel state */
        hndl->var.ch_cfg_ptr[chnl[chnl_itr]].state = MPMA_CHNL_START;
        
        /* Set the channel sch_only state back to the default false state */
        hndl->var.ch_cfg_ptr[chnl[chnl_itr]].sch_only = FALSE;        
    }

    /* clean all CFC FIFO ressource */
    cfc_fifo_clean(hndl->cfc_handle,chnl,num_chnl);

    /* clean SCBS3 */
    if (hndl->cfg.scbs3_cfg.scbs3_present) {
        scbs3_chnl_clean(hndl->scbs3_handle, TRUE,chnl,num_chnl);
    }

    PMC_RETURN();

} /* mpma_ch_clean */

#ifdef PMC_INERNAL
/*******************************************************************************
*  mpma_ch_resize_initial
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initial steps to resize the bandwidth of the specified channel.
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - channel to resize
*   resize_type       - resize type (increase/decrease bandwidth)
*   client_bitrate    - client bit rate (in Hz)
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
PUBLIC PMC_ERROR mpma_ch_resize_initial(mpma_handle_t *hndl, UINT32 chnl, 
                                        mpma_resize_type_t resize_type,
                                        DOUBLE client_bitrate)
{
    PMC_ERROR ret_val= PMC_SUCCESS;
    
    PMC_ENTRY();

    if (hndl->var.ch_cfg_ptr[chnl].state != MPMA_CHNL_OPERATIONAL) {
        PMC_RETURN(MPMA_LOG_CODE_INVALID_CHANNEL_STATE);
    }

    if (MPMA_CHNL_INCREASE_BW == resize_type) {

        /* Calculate the number of pages for CFC */
        hndl->var.ch_cfg_ptr[chnl].cfc_num_pages = mpma_cfc_num_pages_calc(hndl, client_bitrate);
        /* Resize CFC FIFO */
        ret_val = cfc_fifo_resize(hndl->cfc_handle,
                                  chnl,
                                  hndl->var.ch_cfg_ptr[chnl].cfc_num_pages);
        if (ret_val != PMC_SUCCESS) {
            /* Need to return SCBS3 to old rate. */
            PMC_RETURN(ret_val);
        }

    }

    PMC_RETURN(PMC_SUCCESS);

} /* mpma_ch_resize_initial */

/*******************************************************************************
*  mpma_ch_resize_ramp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the MPMA during bandwidth ramp for the specified channel.
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - channel to resize
*   client_bitrate    - client bit rate (in Hz)
*
* OUTPUTS:
*   *pkt_size_ptr     - configured packet size
*   *pkt_per_n_ptr    - configured numerator of packet period fraction
*   *pkt_per_d_ptr    - configured denominator of packet period fraction
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mpma_ch_resize_ramp(mpma_handle_t *hndl, UINT32 chnl,
                                     DOUBLE client_bitrate,
                                     UINT32 *pkt_size_ptr, UINT32 *pkt_per_n_ptr, UINT32 *pkt_per_d_ptr)
{
    UINT32 *pp_int_ptr   = &(hndl->var.ch_cfg_ptr[chnl].pkt_period_int);
    UINT32 *pp_n_ptr     = &(hndl->var.ch_cfg_ptr[chnl].pkt_period_n);
    UINT32 *pp_d_ptr     = &(hndl->var.ch_cfg_ptr[chnl].pkt_period_d);
    UINT32 *ps_ptr       = &(hndl->var.ch_cfg_ptr[chnl].pkt_size);
    DOUBLE *eps_ptr      = &(hndl->var.ch_cfg_ptr[chnl].epsilon);
    DOUBLE *n_ratio_ptr  = &(hndl->var.ch_cfg_ptr[chnl].n_ratio);
    DOUBLE t_frame = (DOUBLE)(hndl->var.ch_cfg_ptr[chnl].t_frame_n) / hndl->var.ch_cfg_ptr[chnl].t_frame_d / UTIL_OPSA_REF_FREQ;
    PMC_ERROR ret_val;

    PMC_ENTRY();
    
    if (hndl->var.ch_cfg_ptr[chnl].state != MPMA_CHNL_OPERATIONAL) {
        PMC_RETURN(MPMA_LOG_CODE_INVALID_CHANNEL_STATE);
    }

    /* Disable the DSP for the channel */
    mpma_dsp_ch_enable_cfg(hndl, chnl, FALSE);

    /* 
     * Calculate packet period parameters. 
     * Use the same parent frame period and previously configured packet size.
     */
    ret_val = util_opsa_pkt_period_calc(client_bitrate, t_frame, *ps_ptr, *ps_ptr, hndl->var.ch_cfg_ptr[chnl].pkt_period_calc,
                                        pp_int_ptr, pp_n_ptr, pp_d_ptr, ps_ptr, eps_ptr, n_ratio_ptr);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    /* Write MPMA settings to hardware */
    mpma_ch_bw_cfg(hndl, chnl);

    /* Return configured parameters. */
    *pkt_size_ptr = *ps_ptr;
    *pkt_per_n_ptr = *pp_int_ptr + (*pp_n_ptr) * (*pp_d_ptr);
    *pkt_per_d_ptr = *pp_d_ptr;

    PMC_RETURN(PMC_SUCCESS);

} /* mpma_ch_resize_ramp */

/*******************************************************************************
*  mpma_ch_resize_end
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Completes the bandwidth resize of the specified channel.
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - channel to resize
*   resize_type       - resize type (increase/decrease bandwidth)
*   client_bitrate    - client bit rate (in Hz)
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
PUBLIC PMC_ERROR mpma_ch_resize_end(mpma_handle_t *hndl, UINT32 chnl, 
                                    mpma_resize_type_t resize_type,
                                    DOUBLE client_bitrate)
{
    PMC_ERROR ret_val;
    
    PMC_ENTRY();
    
    if (hndl->var.ch_cfg_ptr[chnl].state != MPMA_CHNL_OPERATIONAL) {
        PMC_RETURN(MPMA_LOG_CODE_INVALID_CHANNEL_STATE);
    }

    /* Enable the DSP for the channel */
    mpma_dsp_ch_enable_cfg(hndl, chnl, TRUE);

    if (MPMA_CHNL_DECREASE_BW == resize_type) {

        /* Calculate the number of pages for CFC */
        hndl->var.ch_cfg_ptr[chnl].cfc_num_pages = mpma_cfc_num_pages_calc(hndl, client_bitrate);

        /* Resize CFC FIFO */
        ret_val = cfc_fifo_resize(hndl->cfc_handle,
                                  chnl,
                                  hndl->var.ch_cfg_ptr[chnl].cfc_num_pages);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }

    }

    PMC_RETURN(PMC_SUCCESS);

} /* mpma_ch_resize_end */
#endif /* PMC_INTERNAL */

/*******************************************************************************
*  mpma_scbs3_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Return SCBS3 handle 
*
* INPUTS:
*   *hndl           - pointer to MPMA handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   scbs3_handle_t  - The scbs3 handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC scbs3_handle_t* mpma_scbs3_handle_get(mpma_handle_t *hndl)
{
    PMC_ENTRY();
    PMC_RETURN(hndl->scbs3_handle);    
} /*mpma_scbs3_handle_get*/


/*******************************************************************************
* mpma_ch_enable_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get whether packet maker functionality is enabled or disabled
*
* INPUTS:
*   *hndl    - pointer to MPMA handle instance
*   chnl     - channel to get
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8    - TRUE: enable MPMA \n
*              FALSE: disable MPMA
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 mpma_ch_enable_get(mpma_handle_t *hndl, UINT32 chnl)
{
    BOOL8 enable;
    PMC_ENTRY();
    
    /* Enable/disable MPMA */
    enable = mpma_field_CH_EN_get(NULL, hndl, chnl);

    PMC_RETURN(enable);

} /* mpma_ch_enable_get */


/*******************************************************************************
* mpma_int_chnl_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *mpma_handle      - pointer to MPMA handle instance
*   chnl_id           - Channel ID: Valid range: 0 - 95
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
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
PUBLIC PMC_ERROR mpma_int_chnl_enable(mpma_handle_t *mpma_handle,
                                      UINT32 chnl_id,
                                      mpma_int_chnl_t *int_table_ptr,
                                      mpma_int_chnl_t *int_en_table_ptr,
                                      BOOL8 enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = chnl_id;
    UINT32          link2 = chnl_id;
    
    PMC_ENTRY();
    
    result = mpma_int_validate(mpma_handle, chnl_id, int_en_table_ptr);
    
#ifdef MPMA_INT_RANGE
#undef MPMA_INT_RANGE
#endif
#define MPMA_INT_RANGE    UTIL_GLOBAL_INT_ENABLE_RANGE
        
    MPMA_INT_TABLE_DEFINE();
    
    /* CFC */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_enable(mpma_handle->cfc_handle,
                                     chnl_id,
                                     &int_table_ptr->cfc,
                                     &int_en_table_ptr->cfc,
                                     enable);
                                     
    }

    PMC_RETURN(result);
} /* mpma_int_chnl_enable */

/*******************************************************************************
* mpma_int_chnl_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *mpma_handle      - pointer to MPMA handle instance
*   chnl_id           - Channel ID: Valid range: 0 - 95
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
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
PUBLIC PMC_ERROR mpma_int_chnl_clear(mpma_handle_t *mpma_handle,
                                     UINT32 chnl_id,
                                     mpma_int_chnl_t *int_table_ptr,
                                     mpma_int_chnl_t *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = chnl_id;
    UINT32          link2 = chnl_id;
    
    PMC_ENTRY();
    
    result = mpma_int_validate(mpma_handle, chnl_id, mpma_handle);
    
#ifdef MPMA_INT_RANGE
#undef MPMA_INT_RANGE
#endif
#define MPMA_INT_RANGE UTIL_GLOBAL_INT_CLEAR_RANGE
    
    MPMA_INT_TABLE_DEFINE();
    
    /* CFC */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_clear(mpma_handle->cfc_handle,
                                    chnl_id,
                                    int_en_table_ptr!= NULL?&int_en_table_ptr->cfc:NULL,
                                    int_table_ptr!=NULL?&int_table_ptr->cfc:NULL);
                                     
    }
    
    PMC_RETURN(result);
} /* mpma_int_chnl_clear */

/*******************************************************************************
* mpma_int_chnl_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   *mpma_handle      - pointer to MPMA handle instance
*   chnl_id           - Channel ID: Valid range: 0 - 95
*   filt_table_ptr    - pointer to filter table. Mark fields as TRUE within this
*                       table to indicate that the interrupt/status will be retrieved
*                       to the output table. NULL to retrieve all 
*                       interrupt/status.
*
* OUTPUTS:
*   int_table_ptr     - pointer to channel interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mpma_int_chnl_retrieve(mpma_handle_t *mpma_handle,
                                        UINT32 chnl_id,
                                        mpma_int_chnl_t *filt_table_ptr,
                                        mpma_int_chnl_t *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = chnl_id;
    UINT32          link2 = chnl_id;

    PMC_ENTRY();

    result = mpma_int_validate(mpma_handle, chnl_id, int_table_ptr);

    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(mpma_handle, mpma, UTIL_GLOBAL_INT_OPTIMIZE);
    
#ifdef MPMA_INT_RANGE
#undef MPMA_INT_RANGE
#endif
#define MPMA_INT_RANGE UTIL_GLOBAL_INT_RETRIEVE_RANGE
    
    MPMA_INT_TABLE_DEFINE();

    UTIL_GLOBAL_INT_BUFFER_FLUSH(mpma_handle, mpma);

     /* CFC */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_retrieve(mpma_handle->cfc_handle,
                                       chnl_id,
                                       UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, cfc),
                                       &int_table_ptr->cfc);
                                     
    }


    PMC_RETURN(result);
} /* mpma_int_chnl_retrieve */


/*******************************************************************************
* mpma_int_chnl_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*  *mpma_handle      - pointer to MPMA handle instance
*  sector            - Which 32 bit set of registers is being set. 
*                      0: 0-31
*                      1: 32-63
*                      2: 64-95
*
* OUTPUTS:
*  reg_addrs         - Array of addresses
*  index             - Index into the array.
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void mpma_int_chnl_pack_registers(mpma_handle_t *mpma_handle,
                                         UINT32 sector,
                                         UINT32 reg_addrs[],
                                         UINT32 *index)
{
    UINT32 local_index;
    PMC_ENTRY();

    local_index = *index;

    reg_addrs[local_index++] = ((pmc_handle_t *)mpma_handle)->base_address + PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_0 + sector*4;
    reg_addrs[local_index++] = ((pmc_handle_t *)mpma_handle)->base_address + PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_0 + sector*4;
    reg_addrs[local_index++] = ((pmc_handle_t *)mpma_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_OVR_INT_REG_0 + sector*4;
    reg_addrs[local_index++] = ((pmc_handle_t *)mpma_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_UDR_INT_REG_0 + sector*4;
    reg_addrs[local_index++] = ((pmc_handle_t *)mpma_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_IPT_ERR_INT_REG_0 + sector*4;

    *index = local_index;

    PMC_RETURN();
} /* mpma_int_chnl_pack_registers */


/*******************************************************************************
* mpma_int_chnl_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   *mpma_handle      - pointer to MPMA handle instance
*   chnl_id           - Channel ID: Valid range: 0 - 95
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mpma_int_chnl_enabled_check(mpma_handle_t *mpma_handle,
                                             UINT32 chnl_id,
                                             mpma_int_chnl_t *int_en_table_ptr,
                                             BOOL8 *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = chnl_id;
    UINT32          link2 = chnl_id;
    
    PMC_ENTRY();
    
    result = mpma_int_validate(mpma_handle, chnl_id, int_found_ptr);
    
#ifdef MPMA_INT_RANGE
#undef MPMA_INT_RANGE
#endif
#define MPMA_INT_RANGE UTIL_GLOBAL_INT_CHECK_RANGE
    
    MPMA_INT_TABLE_DEFINE();
    
     /* CFC */
    if (PMC_SUCCESS == result && 
        *int_found_ptr == FALSE)
    {
        result = cfc_int_chnl_enabled_check(mpma_handle->cfc_handle,
                                            chnl_id,
                                            &int_en_table_ptr->cfc,
                                            int_found_ptr);
                                     
    }

    PMC_RETURN(result);
} /* mpma_int_chnl_enabled_check */

/*
** Private Functions
*/ 
/*******************************************************************************
*  mpma_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize MPMA var_t variables.
*
* INPUTS:
*   *hndl    - pointer to MPMA handle instance
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
PRIVATE void mpma_var_default_init(mpma_handle_t *hndl)
{
    UINT32 chnl;

    PMC_ENTRY();

    /* Configure default MPMA variable parameters for each channel */
    for (chnl = 0; chnl < hndl->cfg.default_param.num_chnl; chnl++) {
        hndl->var.ch_cfg_ptr[chnl].state               = MPMA_CHNL_START;
        hndl->var.ch_cfg_ptr[chnl].client_bitrate      = 0;
        hndl->var.ch_cfg_ptr[chnl].t_frame_n           = 0;
        hndl->var.ch_cfg_ptr[chnl].t_frame_d           = 1;
        hndl->var.ch_cfg_ptr[chnl].t_frame             = 0;
        hndl->var.ch_cfg_ptr[chnl].pkt_period_int      = 0;
        hndl->var.ch_cfg_ptr[chnl].pkt_period_n        = 0;
        hndl->var.ch_cfg_ptr[chnl].pkt_period_d        = 1;
        hndl->var.ch_cfg_ptr[chnl].pkt_size            = 0;
        hndl->var.ch_cfg_ptr[chnl].pkts_per_cn         = 0;
        hndl->var.ch_cfg_ptr[chnl].cn_base             = 0;
        hndl->var.ch_cfg_ptr[chnl].mpma_gain           = 0;
        hndl->var.ch_cfg_ptr[chnl].epsilon             = 0;
        hndl->var.ch_cfg_ptr[chnl].n_ratio             = 0;
        hndl->var.ch_cfg_ptr[chnl].pkt_period_calc     = UTIL_OPSA_EPS_MIN;
        hndl->var.ch_cfg_ptr[chnl].xoff_thres          = 0;
        hndl->var.ch_cfg_ptr[chnl].cfc_num_pages       = 0;
    }

    PMC_RETURN();
} /* mpma_var_default_init */

/*******************************************************************************
*  mpma_ch_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize channel with default parameters.
*
* INPUTS:
*   *hndl    - pointer to MPMA handle instance
*   chnl     - channel to initialize
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
PRIVATE PMC_ERROR mpma_ch_init(mpma_handle_t *hndl, UINT32 chnl)
{
    UINT32 writeWord;

    PMC_ENTRY();
    if(hndl->cfg.default_param.mpma_type_id != MPMA_COREOTN_CTL)
    {
          writeWord = (MPMA_PSD_FIFO_REFILL_THRES << CORE_REG_CFG_RAM_0_BIT_PSD_FIFO_REFILL_THRES_OFF) & CORE_REG_CFG_RAM_0_BIT_PSD_FIFO_REFILL_THRES_MSK;
    }
    else
    {
          writeWord = (MPMA_PSD_FIFO_REFILL_THRES_CTL << CORE_REG_CFG_RAM_0_BIT_PSD_FIFO_REFILL_THRES_OFF) & CORE_REG_CFG_RAM_0_BIT_PSD_FIFO_REFILL_THRES_MSK;
    }

    writeWord += (hndl->cfg.default_param.ts_enable << CORE_REG_CFG_RAM_0_BIT_TS_ENABLE_OFF) & CORE_REG_CFG_RAM_0_BIT_TS_ENABLE_MSK;
    writeWord += (hndl->cfg.default_param.mkr_mode << CORE_REG_CFG_RAM_0_BIT_MKR_MODE_OFF) & CORE_REG_CFG_RAM_0_BIT_MKR_MODE_MSK;
   
    /* Configure MPMA core default parameters */
    mpma_reg_CFG_RAM_0_array_write(NULL, hndl, chnl,writeWord);

    /* DSP configurations */
    mpma_dsp_field_CN_BIT_MODE_set(NULL, hndl, chnl, hndl->cfg.default_param.cn_bit_mode);


    mpma_dsp_field_CN_ENABLE_set(NULL, hndl, chnl, hndl->cfg.default_param.cn_enable);
    mpma_dsp_field_CND_ENABLE_set(NULL, hndl, chnl, hndl->cfg.default_param.cnd_enable);
    
    mpma_dsp_field_PKT_SIZE_VAR_set(NULL, hndl, chnl, MPMA_PKT_SIZE_VAR);
    mpma_dsp_field_DSM_TYPE_set(NULL, hndl, chnl, MPMA_DSM_TYPE);

    PMC_RETURN(PMC_SUCCESS);

} /* mpma_ch_init */


/*******************************************************************************
*  mpma_ch_bw_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure bandwidth related parameters for the channel.
*
* INPUTS:
*   *hndl    - pointer to MPMA handle instance
*   chnl     - channel to initialize
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
PUBLIC PMC_ERROR mpma_ch_bw_cfg(mpma_handle_t *hndl, UINT32 chnl)
{
    mpma_dsp_buffer_t mpma_buf[1];
    PMC_ENTRY();
    
    /* Configure MPMA core with packet size */
    mpma_field_PKT_SIZE_set(NULL, hndl, chnl, hndl->var.ch_cfg_ptr[chnl].pkt_size);
    mpma_dsp_buffer_init(mpma_buf, hndl);
    /* DSP configurations */
    if(hndl->cfg.default_param.cn_enable == FALSE)
    { 
        mpma_dsp_field_PKT_PERIOD_INT_set(mpma_buf, hndl, chnl, hndl->var.ch_cfg_ptr[chnl].pkt_period_int);
        mpma_dsp_field_PKT_PERIOD_N_set(mpma_buf, hndl, chnl, ((hndl->var.ch_cfg_ptr[chnl].pkt_period_n)&0x1FF));
        mpma_dsp_buffer_flush(mpma_buf);
        mpma_dsp_buffer_init(mpma_buf, hndl);
        mpma_dsp_field_PKT_PERIOD_D_set(mpma_buf, hndl, chnl, (hndl->var.ch_cfg_ptr[chnl].pkt_period_d)&0xFF);
    }
    mpma_dsp_field_PKT_SIZE_set(mpma_buf, hndl, chnl, hndl->var.ch_cfg_ptr[chnl].pkt_size);
    mpma_dsp_buffer_flush(mpma_buf);

    mpma_dsp_field_CN_BASE_set(NULL, hndl, chnl, hndl->var.ch_cfg_ptr[chnl].cn_base);
    mpma_dsp_field_MPMA_GAIN_set(NULL, hndl, chnl, hndl->var.ch_cfg_ptr[chnl].mpma_gain);

    mpma_dsp_buffer_init(mpma_buf, hndl);
    mpma_dsp_field_PKTS_PER_CN_set(mpma_buf, hndl, chnl, hndl->var.ch_cfg_ptr[chnl].pkts_per_cn);
    if(mpma_non_rev_a_instance_type_test(hndl)==TRUE)
    {
        if(hndl->cfg.default_param.mpma_type_id == MPMA_COREOTN)
        {
            mpma_dsp_field_INTEGRATOR_CLAMP_set(mpma_buf, hndl, chnl, hndl->var.ch_cfg_ptr[chnl].mpma_gain);
        } else 
        {          
            /* Set INTEGRATOR_CLAMP field to MPMA_GAIN -1, saturating at zero. */
            if(hndl->var.ch_cfg_ptr[chnl].mpma_gain <= 1)
            {
                mpma_dsp_field_INTEGRATOR_CLAMP_set(mpma_buf, hndl, chnl, 0);  
            } else {
                mpma_dsp_field_INTEGRATOR_CLAMP_set(mpma_buf, hndl, chnl, (hndl->var.ch_cfg_ptr[chnl].mpma_gain - 1));
            }
        }
    }
    mpma_dsp_buffer_flush(mpma_buf);

    PMC_RETURN(PMC_SUCCESS);

} /* mpma_ch_bw_cfg */

/*******************************************************************************
* mpma_ch_enable_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/disables the per-channel packet maker functionality.
*
* INPUTS:
*   *hndl    - pointer to MPMA handle instance
*   chnl     - channel to configure
*   enable   - TRUE: enable MPMA. FALSE: disable MPMA.
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
PUBLIC PMC_ERROR mpma_ch_enable_cfg(mpma_handle_t *hndl, UINT32 chnl, BOOL enable)
{
    PMC_ENTRY();
    
    /* Enable/disable MPMA */
    mpma_field_CH_EN_set(NULL, hndl, chnl, enable);
    
    PMC_RETURN(PMC_SUCCESS);
} /* mpma_ch_enable_cfg */

/*******************************************************************************
* mpma_dsp_ch_enable_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/disables the per-channel packet maker DSP functionality.
*
* INPUTS:
*   *hndl    - pointer to MPMA handle instance
*   chnl     - channel to configure
*   enable   - TRUE: enable MPMA DSP. FALSE: disable MPMA DSP.
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
PUBLIC PMC_ERROR mpma_dsp_ch_enable_cfg(mpma_handle_t *hndl, UINT32 chnl, BOOL enable)
{
    PMC_ENTRY();
    
    /* Enable/disable MPMA DSP */
    mpma_dsp_field_SLICE_ENABLE_set(NULL, hndl, chnl, enable);
    
    PMC_RETURN(PMC_SUCCESS);
} /* mpma_dsp_ch_enable_cfg */

/*******************************************************************************
* mpma_cfc_num_pages_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Calculates the number of pages to allocate in the CFC 
*   based on the client bit rate. This function is used by MPMA instances that
*   do not require database access.
*
* INPUTS:
*   *hndl          - pointer to MPMA handle instance
*   client_bitrate - client bit rate
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32         - Number of pages to allocate.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 mpma_cfc_num_pages_calc(mpma_handle_t *hndl, DOUBLE client_bitrate)
{
    UINT32 result;
    
    PMC_ENTRY();
    /* Allocate one page per ODU0. */
    /*return PMC_CEIL(client_bitrate/MPMA_LOWEST_BANDWIDTH);*/
    PMC_ASSERT(hndl->cfg.default_param.lowest_bw != 0, MPMA_LOG_ERR_DIVIDED_BY_ZERO, 0, 0);
    /* each MPMA instance has different bandwidth */

    if (!digi_use_float32_get())
    {
        result = (UINT32)PMC_CEIL(client_bitrate/ (hndl->cfg.default_param.lowest_bw) );
    }
    else
    {
        result = (UINT32)PMC_CEILF((FLOAT)client_bitrate/ (FLOAT)(hndl->cfg.default_param.lowest_bw) );
    }
    
    PMC_RETURN(result);

} /* mpma_cfc_num_pages_calc */


/*******************************************************************************
*  mpma_schd_ch_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function for the scheduler only. It is called via
*   mpma_ch_prov regardless of using the light or full MPMA
*
* INPUTS:
*   *hndl               - pointer to MPMA handle instance
*   chnl                - channel to provision
*   client_bitrate      - client bitrate in bps.
*   scbs3_user_bits_3_0 - SCBS3 userbits [3:0] to output with the channel
*                         during the calendar slot.
*                         These bits might be used as downstream MUX 
*                         select bits (for bus aggregation).
*                         Set to 0 if unused.
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mpma_schd_ch_prov(mpma_handle_t *hndl, UINT32 chnl, 
                                    DOUBLE client_bitrate, 
                                    UINT32 scbs3_user_bits_3_0)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 scbs3_rate;

    PMC_ENTRY();

    /* 
     * Provision SCBS3 after packet period calculations have been done successfully.
     * Convert the rate to a number that the SCBS3 can handle.
     * The highest rate is 100Gbit/s. Divide by 100 to fit into a UINT32 
     * and use 100 bit/s rate units.
     */
    if (hndl->cfg.scbs3_cfg.scbs3_present) {
        if (!digi_use_float32_get()) {
            scbs3_rate = (UINT32)(client_bitrate / MPMA_SCBS3_RATE_CONV_FACTOR);
        } else {
            scbs3_rate = (UINT32)((FLOAT)client_bitrate / MPMA_SCBS3_RATE_CONV_FACTOR);
        }
        ret_val = scbs3_chnl_prov(hndl->scbs3_handle, chnl, 
                                  scbs3_rate, UTIL_SCHD_100_BIT_SEC, scbs3_user_bits_3_0);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }

    /* Update channel state */
    hndl->var.ch_cfg_ptr[chnl].state = MPMA_CHNL_EQUIPPED;

    PMC_RETURN(PMC_SUCCESS);

} /* mpma_schd_ch_prov */

/*******************************************************************************
*  mpma_schd_ch_reprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function for the scheduler only. 
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - channel to deprovision
*   client_bitrate    - client bitrate in bps
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
PUBLIC PMC_ERROR mpma_schd_ch_reprov(mpma_handle_t *hndl, UINT32 chnl, 
                                     DOUBLE client_bitrate)
{
    UINT32 scbs3_rate;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* Deprovision SCBS3 */
    if (hndl->cfg.scbs3_cfg.scbs3_present) {
        scbs3_rate = (UINT32)(client_bitrate / MPMA_SCBS3_RATE_CONV_FACTOR);
        ret_val = scbs3_chnl_reprov(hndl->scbs3_handle, chnl, scbs3_rate, UTIL_SCHD_100_BIT_SEC);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        scbs3_page_udpate_and_swap(hndl->scbs3_handle);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mpma_schd_ch_reprov */

/*******************************************************************************
*  mpma_scbs3_all_chnls_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates all provisioned channel.
*   This function writes the calculated parameters for the provisioned channel
*   to hardware.
*
* INPUTS:
*   *hndl           - pointer to MPMA handle instance
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
PUBLIC PMC_ERROR mpma_scbs3_all_chnls_activate(mpma_handle_t *hndl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
        
    PMC_ENTRY();
    
    /* swap the page and write all channels to the offline page */
    ret_val = scbs3_page_udpate_and_swap(hndl->scbs3_handle);
    
    PMC_RETURN(ret_val);
} /* mpma_scbs3_all_chnls_activate */


/*******************************************************************************
*  mpma_scbs3_all_chnls_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates all provisioned channel.
*   This function writes the calculated parameters for the provisioned channel
*   to hardware.
*
* INPUTS:
*   *hndl           - pointer to MPMA handle instance
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
PUBLIC PMC_ERROR mpma_scbs3_all_chnls_deactivate(mpma_handle_t *hndl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
        
    PMC_ENTRY();
    
    /* swap the page and write all channels to the offline page */
    ret_val = scbs3_page_udpate_and_swap(hndl->scbs3_handle);
    
    PMC_RETURN(ret_val);
} /* mpma_scbs3_all_chnls_deactivate */


/*******************************************************************************
*  mpma_schd_ch_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function for the scheduler only. It is called via
*   mpma_ch_deprov regardless of using the light or full MPMA
*
* INPUTS:
*   *hndl           - pointer to MPMA handle instance
*   chnl            - channel to deprovision
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
PRIVATE PMC_ERROR mpma_schd_ch_deprov(mpma_handle_t *hndl, UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* Deprovision SCBS3 */
    if (hndl->cfg.scbs3_cfg.scbs3_present) {
        ret_val = scbs3_chnl_deprov(hndl->scbs3_handle, chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }

    /* Update channel state */
    hndl->var.ch_cfg_ptr[chnl].state = MPMA_CHNL_START;

    /* Set the channel sch_only state back to the default false state */
    hndl->var.ch_cfg_ptr[chnl].sch_only = FALSE;

    PMC_RETURN(PMC_SUCCESS);

} /* mpma_schd_ch_deprov */

/*******************************************************************************
*  mpma_schd_ch_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function for the scheduler only. It is called via
*   mpma_ch_activate regardless of using the light or full MPMA
*
* INPUTS:
*   *hndl           - pointer to MPMA handle instance
*   chnl            - channel to activate
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
PRIVATE void mpma_schd_ch_activate(mpma_handle_t *hndl, UINT32 chnl)
{
    PMC_ENTRY();

    /* Activate SCBS3 and write the configuration to hardware */
    if (hndl->cfg.scbs3_cfg.scbs3_present) {
        scbs3_chnl_activate(hndl->scbs3_handle, chnl);
        scbs3_page_udpate_and_swap(hndl->scbs3_handle);
        
    }

    PMC_RETURN();
} /* mpma_schd_ch_activate */

/*******************************************************************************
*  mpma_schd_ch_activate_super_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function for the scheduler only. It is called via
*   mpma_ch_activate regardless of using the light or full MPMA
*
* INPUTS:
*   *hndl           - pointer to MPMA handle instance
*   chnl            - channel to activate
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
PRIVATE void mpma_schd_ch_activate_super_prov(mpma_handle_t *hndl, UINT32 chnl)
{
    PMC_ENTRY();

    /* Activate SCBS3 and write the configuration to hardware */
    if (hndl->cfg.scbs3_cfg.scbs3_present) {
        scbs3_chnl_activate(hndl->scbs3_handle, chnl);      
        
        if (hndl->cfg.default_param.mpma_type_id != MPMA_COREOTN &&
            hndl->cfg.default_param.mpma_type_id != MPMA_COREOTN_FROM_MAPOTN &&
            hndl->cfg.default_param.mpma_type_id != MPMA_COREOTN_CTL)
        {
            scbs3_page_udpate_and_swap(hndl->scbs3_handle);
        }                                            
    }

    PMC_RETURN();
} /* mpma_schd_ch_activate_super_prov */

/*******************************************************************************
*  mpma_sched_ch_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function for the scheduler only. It is called via
*   mpma_ch_activate regardless of using the light or full MPMA
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - channel to deactivate
*   super_deprov      - boolean to indicate if this function is used in 
*                       bulk deprovisioning mode. \n
*                       TRUE: bulk deprovisioning \n
*                       FALSE: normal deprovisioning
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
PRIVATE void mpma_schd_ch_deactivate(mpma_handle_t *hndl, UINT32 chnl,
                                     BOOL super_deprov)
{
    PMC_ENTRY();

    /* Deactivate SCBS3 */
    if (hndl->cfg.scbs3_cfg.scbs3_present) {
        scbs3_chnl_deactivate(hndl->scbs3_handle, chnl);
        
        if (super_deprov)
        {
            if (hndl->cfg.default_param.mpma_type_id != MPMA_COREOTN)
            {
                scbs3_page_udpate_and_swap(hndl->scbs3_handle);
            }        
        }
        else {
            scbs3_page_udpate_and_swap(hndl->scbs3_handle);
        }
    }

    PMC_RETURN();
} /* mpma_schd_ch_deactivate */


/*******************************************************************************
*  mpma_pkt_period_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This calculates packet size(B) and packet period(T) for a given client rate
*   and the desired packet size.
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   client_bitrate    - client bit rate (in Hz)
*   pkt_size          - desired minimum packet size. 
*                       Set pkt_size to 0 to use internal defaults.
*   pkt_size_range    - desired range of packet size. 
*                       Packet sizes from pkt_size to pkt_size + pkt_size_range
*                       will be considered.
*   cn_frame_period   - parent Cn frame period that the client is demapped 
*                       from. Only required for cn_mode = 0 (source node  
*                       reference clock timing (B + C mode)).
*                       Set to UTIL_OPSA_T_FRAME_UNUSED if unused.
*   pkt_period_calc   - method for calculating packet period and size.
*                       When using UTIL_OPSA_USE_OIF_xxx_BYTE, 
*                       pkt_size should be set within the valid OIF range,
*                       and pkt_size_range is ignored.
*
* OUTPUTS:
*   *pkt_size_ptr     - configured packet size
*   *pkt_per_int_ptr  - configured integer portion of packet period
*   *pkt_per_n_ptr    - configured numerator of packet period fraction
*   *pkt_per_d_ptr    - configured denominator of packet period fraction
*   *epsilon_ptr      - configured epsilon 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR mpma_pkt_period_calc(mpma_handle_t *hndl, 
                                      DOUBLE client_bitrate, 
                                      UINT32 pkt_size, 
                                      UINT32 pkt_size_range,
                                      util_opsa_cn_frame_period_t cn_frame_period,
                                      util_opsa_pkt_period_calc_t pkt_period_calc, 
                                      UINT32 *pkt_size_ptr,
                                      UINT32 *pkt_per_int_ptr,
                                      UINT32 *pkt_per_n_ptr, 
                                      UINT32 *pkt_per_d_ptr,
                                      DOUBLE *epsilon_ptr)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    UINT32 pp_int;
    UINT32 pp_n;    
    UINT32 pp_d;    
    UINT32 ps;      
    DOUBLE eps;     
    DOUBLE n_ratio;
    UINT32 pkt_size_min;
    UINT32 pkt_size_max;
    DOUBLE t_frame;
    UINT32 t_frame_n;
    UINT32 t_frame_d;

    PMC_ENTRY();

    /* There may be "division by 0" in the code bellow, and we have to check it
       here. */
    PMC_ASSERT(client_bitrate != 0, MPMA_LOG_ERR_DIVIDED_BY_ZERO, 0, 0);

    /* Check if specified packet size is valid. */
    if (0 == pkt_size) {
        /* Use default packet size */
        pkt_size_min = MPMA_DEFAULT_PKT_SIZE;
        pkt_size_max = MPMA_DEFAULT_PKT_SIZE;
    } else {
        pkt_size_min = pkt_size;
        pkt_size_max = pkt_size + pkt_size_range;
        /* Check for valid packet size range */
        if ((pkt_size_min < MPMA_PKT_SIZE_MIN) ||
            (pkt_size_max > MPMA_PKT_SIZE_MAX)) {
            PMC_RETURN(MPMA_LOG_ERR_INVALID_PARAMETERS);
        }
    }
    

    if(UTIL_OPSA_MAPOTN_ODUFLEX_T_FRAME == cn_frame_period)
    {
        /* Calculate packet period parameters. */
        t_frame = UTIL_OPSA_ODUFLEX_PAYLOAD_BYTES/client_bitrate;
    }
    else 
    {
        util_opsa_cn_frame_period_get(cn_frame_period, &t_frame_n, &t_frame_d);

        /* Calculate packet period parameters. */
        t_frame = (DOUBLE)t_frame_n / t_frame_d / UTIL_OPSA_REF_FREQ;
    }

        
    if ((UTIL_OPSA_EPS_MIN == pkt_period_calc) || 
        (UTIL_OPSA_INT_PKT_PER == pkt_period_calc)) {
        /* Minimum epsilon or integer packet period. */
        ret_val = util_opsa_pkt_period_calc(client_bitrate, t_frame,
                                            pkt_size_min, pkt_size_max, 
                                            pkt_period_calc, &pp_int, &pp_n,
                                            &pp_d, &ps, &eps, &n_ratio);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    } 
    else {
        PMC_RETURN(MPMA_LOG_ERR_INVALID_PARAMETERS);
    } 
    
    /* Return configured parameters. */
    *pkt_size_ptr = ps;
    *pkt_per_int_ptr = pp_int;
    *pkt_per_n_ptr = pp_n;
    *pkt_per_d_ptr = pp_d;
    *epsilon_ptr = eps;

    PMC_RETURN(PMC_SUCCESS);
} /*  mpma_pkt_period_calc */

/*******************************************************************************
*  mpma_oif_pkt_period_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This calculates packet size(B) and packet period(T) for a given client rate
*   and the desired packet size as defined in OIF recommendations.
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   client_bitrate    - client bit rate (in Hz)
*   pkt_size          - desired minimum packet size. 
*                       Set pkt_size to 0 to use internal defaults.
*   header_size       - header size.
*   pkt_period_calc   - method for calculating packet period and size.
*                       When using UTIL_OPSA_USE_OIF_xxx_BYTE, 
*                       pkt_size should be set within the valid OIF range,
*                       and pkt_size_range is ignored.
*
* OUTPUTS:
*   *pkt_size_ptr     - configured packet size
*   *pkt_per_int_ptr  - configured integer portion of packet period
*   *pkt_per_n_ptr    - configured numerator of packet period fraction
*   *pkt_per_d_ptr    - configured denominator of packet period fraction
*   *epsilon_ptr      - minimum epsilon
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mpma_oif_pkt_period_calc(mpma_handle_t *hndl, 
                                          DOUBLE client_bitrate, 
                                          UINT32 pkt_size, 
                                          UINT32 header_size,
                                          util_opsa_pkt_period_calc_t pkt_period_calc, 
                                          UINT32 *pkt_size_ptr,
                                          UINT32 *pkt_per_int_ptr,
                                          UINT32 *pkt_per_n_ptr, 
                                          UINT32 *pkt_per_d_ptr,
                                          DOUBLE *epsilon_ptr)
{
    /* variable declaration */
    PMC_ERROR ret_val;
    
    UINT32 pp_int;
    UINT32 pp_n;
    UINT32 pp_d;
    UINT32 ps;
    DOUBLE eps;
    UINT32 max_pkt_size;

    PMC_ENTRY();

    /* Check if specified packet size is valid. */
    max_pkt_size = pkt_size - header_size;
    PMC_LOG_TRACE("pkt_size = %u\n", pkt_size);
    PMC_LOG_TRACE("header_size = %u\n", header_size);
    PMC_LOG_TRACE("max_pkt_size = %u\n", max_pkt_size);
    
    if ((UTIL_OPSA_USE_OIF_128_BYTE == pkt_period_calc) ||
        (UTIL_OPSA_USE_OIF_256_BYTE == pkt_period_calc) ||
        (UTIL_OPSA_USE_OIF_512_BYTE == pkt_period_calc)) 
    {
        /* OIF recommendations for packet period. */
        ret_val = util_opsa_oif_pkt_period_calc(client_bitrate, max_pkt_size, 
                                                pkt_period_calc, &pp_int,
                                                &pp_n, &pp_d, &ps, &eps);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }
    else {
        PMC_RETURN(MPMA_LOG_ERR_INVALID_PARAMETERS);
    } 
    
    /* Return configured parameters. */
    *pkt_size_ptr = ps;
    *pkt_per_int_ptr = pp_int;
    *pkt_per_n_ptr = pp_n;
    *pkt_per_d_ptr = pp_d;
    *epsilon_ptr = eps;

    PMC_RETURN(PMC_SUCCESS);
} /*  mpma_oif_pkt_period_calc */

/*******************************************************************************
*  mpma_pkt_period_var_set_double
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function updates var_t data with channel rate, packet size, packet
*   period information. This function also calculates MPMA gain based on the 
*   packet period.
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - channel to provision
*   client_bitrate    - client bit rate (in Hz)
*   cn_frame_period   - parent Cn frame period that the client is demapped 
*                       from. Only required for cn_mode = 0 (source node  
*                       reference clock timing (B + C mode)).
*                       Set to UTIL_OPSA_T_FRAME_UNUSED if unused.
*   pkt_period_calc   - method for calculating packet period and size.
*                       When using UTIL_OPSA_USE_OIF_xxx_BYTE, 
*                       pkt_size should be set within the valid OIF range,
*                       and pkt_size_range is ignored.
*   pkt_size          - configured packet size
*   pkt_per_int       - configured integer portion of packet period
*   pkt_per_n         - configured numerator of packet period fraction
*   pkt_per_d         - configured denominator of packet period fraction
*   epsilon           - minimum epsilon        
*   corner_frequency  - corner frequency in units of Hz.
*
* OUTPUTS:
*   None.
*   
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mpma_pkt_period_var_set_double(mpma_handle_t *hndl, UINT32 chnl, 
                                         DOUBLE client_bitrate, 
                                         util_opsa_cn_frame_period_t cn_frame_period,
                                         util_opsa_pkt_period_calc_t pkt_period_calc, 
                                         UINT32 pkt_size,
                                         UINT32 pkt_per_int,
                                         UINT32 pkt_per_n, 
                                         UINT32 pkt_per_d, DOUBLE epsilon,
                                         UINT32 corner_frequency)
{
    /* variable declaration */
    DOUBLE pkt_period=0;
    UINT32 t_frame_n=0;
    UINT32 t_frame_d=0;
    DOUBLE t_frame=0;
    DOUBLE temp_r=0;
    DOUBLE pkt_period_s=0;

    PMC_ENTRY();

    hndl->var.ch_cfg_ptr[chnl].pkt_size = pkt_size;
    hndl->var.ch_cfg_ptr[chnl].pkt_period_int = pkt_per_int;
    hndl->var.ch_cfg_ptr[chnl].pkt_period_n = pkt_per_n;
    hndl->var.ch_cfg_ptr[chnl].pkt_period_d = pkt_per_d;
    hndl->var.ch_cfg_ptr[chnl].epsilon = epsilon;   
    
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].pkt_size=%d \n", hndl->var.ch_cfg_ptr[chnl].pkt_size);
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].pkt_period_int=%d \n", hndl->var.ch_cfg_ptr[chnl].pkt_period_int);
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].pkt_period_n=%d \n", hndl->var.ch_cfg_ptr[chnl].pkt_period_n);
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].pkt_period_d=%d \n", hndl->var.ch_cfg_ptr[chnl].pkt_period_d);
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].epsilon=%f \n", hndl->var.ch_cfg_ptr[chnl].epsilon);
    
    util_opsa_cn_frame_period_get(cn_frame_period, &t_frame_n, &t_frame_d);
    hndl->var.ch_cfg_ptr[chnl].t_frame_n = t_frame_n;
    hndl->var.ch_cfg_ptr[chnl].t_frame_d = t_frame_d;
    
    /*DOUBLE *n_ratio_ptr  = &(hndl->var.ch_cfg_ptr[chnl].n_ratio);*/
    
    /* Record the channel information */
    hndl->var.ch_cfg_ptr[chnl].client_bitrate  = client_bitrate;
    hndl->var.ch_cfg_ptr[chnl].pkt_period_calc = pkt_period_calc;
    
    if(UTIL_OPSA_MAPOTN_ODUFLEX_T_FRAME == cn_frame_period)
    {
        /* Calculate packet period parameters. */
        t_frame = UTIL_OPSA_ODUFLEX_PAYLOAD_BYTES/client_bitrate;
    } else {
        /* Calculate packet period parameters. */
        t_frame = (DOUBLE)t_frame_n / t_frame_d / UTIL_OPSA_REF_FREQ;
    }

    PMC_LOG_TRACE("t_frame_n = %d \n", t_frame_n);
    PMC_LOG_TRACE("t_frame_d = %d \n", t_frame_d);
    PMC_LOG_TRACE("t_frame = %f \n", t_frame);

    temp_r = ((DOUBLE)(INT32)pkt_per_n)/((DOUBLE)(INT32)pkt_per_d);
    pkt_period= ((DOUBLE)pkt_per_int + temp_r);

    /*pkt_period = ((DOUBLE) pkt_per_int + (DOUBLE)(pkt_per_n)/ (pkt_per_d))/UTIL_OPSA_REF_FREQ;*/
    if (pkt_period != 0) {
        if(UTIL_OPSA_MAPOTN_ODUFLEX_T_FRAME == cn_frame_period)
        {   
             hndl->var.ch_cfg_ptr[chnl].pkts_per_cn = (UINT32)(  t_frame*UTIL_OPSA_REF_FREQ / pkt_period); 
        } else 
        {
            hndl->var.ch_cfg_ptr[chnl].pkts_per_cn =  (DOUBLE)t_frame_n / (DOUBLE)t_frame_d * (DOUBLE)(INT32)pkt_per_d / ( ((DOUBLE)pkt_per_int * (DOUBLE)(INT32)pkt_per_d) + (DOUBLE)(INT32)pkt_per_n);
        }
    } else {
        hndl->var.ch_cfg_ptr[chnl].pkts_per_cn = 0;
    }
    
    PMC_LOG_TRACE("pkt_period = %f \n", pkt_period);
    PMC_LOG_TRACE("hndl->var.ch_cfg_ptr[chnl].pkts_per_cn = %d \n", hndl->var.ch_cfg_ptr[chnl].pkts_per_cn);

    hndl->var.ch_cfg_ptr[chnl].cn_base = pkt_size * hndl->var.ch_cfg_ptr[chnl].pkts_per_cn;
    PMC_LOG_TRACE("hndl->var.ch_cfg_ptr[chnl].cn_base = %d \n", hndl->var.ch_cfg_ptr[chnl].cn_base);
    
    /* Calculate MPMA gain */
    pkt_period_s = pkt_period / UTIL_OPSA_REF_FREQ;

    util_opsa_mpma_gain_calc(pkt_period_s, (DOUBLE)corner_frequency, mpma_non_rev_a_instance_type_test(hndl), &(hndl->var.ch_cfg_ptr[chnl].mpma_gain));


    PMC_RETURN(PMC_SUCCESS);

} /* mpma_pkt_period_var_set_double */

/*******************************************************************************
*  mpma_pkt_period_var_set_float
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function updates var_t data with channel rate, packet size, packet
*   period information. This function also calculates MPMA gain based on the 
*   packet period.
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - channel to provision
*   client_bitrate    - client bit rate (in Hz)
*   cn_frame_period   - parent Cn frame period that the client is demapped 
*                       from. Only required for cn_mode = 0 (source node  
*                       reference clock timing (B + C mode)).
*                       Set to UTIL_OPSA_T_FRAME_UNUSED if unused.
*   pkt_period_calc   - method for calculating packet period and size.
*                       When using UTIL_OPSA_USE_OIF_xxx_BYTE, 
*                       pkt_size should be set within the valid OIF range,
*                       and pkt_size_range is ignored.
*   pkt_size          - configured packet size
*   pkt_per_int       - configured integer portion of packet period
*   pkt_per_n         - configured numerator of packet period fraction
*   pkt_per_d         - configured denominator of packet period fraction
*   epsilon           - minimum epsilon        
*   corner_frequency  - corner frequency in units of Hz.
*
* OUTPUTS:
*   None.
*   
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mpma_pkt_period_var_set_float(mpma_handle_t *hndl, UINT32 chnl, 
                                         DOUBLE client_bitrate, 
                                         util_opsa_cn_frame_period_t cn_frame_period,
                                         util_opsa_pkt_period_calc_t pkt_period_calc, 
                                         UINT32 pkt_size,
                                         UINT32 pkt_per_int,
                                         UINT32 pkt_per_n, 
                                         UINT32 pkt_per_d, DOUBLE epsilon,
                                         UINT32 corner_frequency)
{
    /* variable declaration */
    FLOAT pkt_period=0;
    UINT32 t_frame_n=0;
    UINT32 t_frame_d=0;
    FLOAT t_frame=0;
    FLOAT temp_r=0;
    FLOAT pkt_period_s=0;

    PMC_ENTRY();

    hndl->var.ch_cfg_ptr[chnl].pkt_size = pkt_size;
    hndl->var.ch_cfg_ptr[chnl].pkt_period_int = pkt_per_int;
    hndl->var.ch_cfg_ptr[chnl].pkt_period_n = pkt_per_n;
    hndl->var.ch_cfg_ptr[chnl].pkt_period_d = pkt_per_d;
    hndl->var.ch_cfg_ptr[chnl].epsilon = epsilon;   
    
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].pkt_size=%d \n", hndl->var.ch_cfg_ptr[chnl].pkt_size);
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].pkt_period_int=%d \n", hndl->var.ch_cfg_ptr[chnl].pkt_period_int);
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].pkt_period_n=%d \n", hndl->var.ch_cfg_ptr[chnl].pkt_period_n);
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].pkt_period_d=%d \n", hndl->var.ch_cfg_ptr[chnl].pkt_period_d);
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].epsilon=%f \n", hndl->var.ch_cfg_ptr[chnl].epsilon);
    
    util_opsa_cn_frame_period_get(cn_frame_period, &t_frame_n, &t_frame_d);
    hndl->var.ch_cfg_ptr[chnl].t_frame_n = t_frame_n;
    hndl->var.ch_cfg_ptr[chnl].t_frame_d = t_frame_d;
    
    /*DOUBLE *n_ratio_ptr  = &(hndl->var.ch_cfg_ptr[chnl].n_ratio);*/
    
    /* Record the channel information */
    hndl->var.ch_cfg_ptr[chnl].client_bitrate  = client_bitrate;
    hndl->var.ch_cfg_ptr[chnl].pkt_period_calc = pkt_period_calc;
    
    if(UTIL_OPSA_MAPOTN_ODUFLEX_T_FRAME == cn_frame_period)
    {
        /* Calculate packet period parameters. */
        t_frame = UTIL_OPSA_ODUFLEX_PAYLOAD_BYTES/(FLOAT)client_bitrate;
    } else {
        /* Calculate packet period parameters. */
        t_frame = (FLOAT)t_frame_n / t_frame_d / (FLOAT)UTIL_OPSA_REF_FREQ;
    }

    PMC_LOG_TRACE("t_frame_n = %d \n", t_frame_n);
    PMC_LOG_TRACE("t_frame_d = %d \n", t_frame_d);
    PMC_LOG_TRACE("t_frame = %f \n", t_frame);

    temp_r = ((FLOAT)(INT32)pkt_per_n)/((FLOAT)(INT32)pkt_per_d);
    pkt_period= ((FLOAT)pkt_per_int + temp_r);

    /*pkt_period = ((DOUBLE) pkt_per_int + (DOUBLE)(pkt_per_n)/ (pkt_per_d))/UTIL_OPSA_REF_FREQ;*/
    if (pkt_period != 0) {
        if(UTIL_OPSA_MAPOTN_ODUFLEX_T_FRAME == cn_frame_period)
        {   
             hndl->var.ch_cfg_ptr[chnl].pkts_per_cn = (UINT32)(  t_frame*(FLOAT)UTIL_OPSA_REF_FREQ / pkt_period); 
        } else 
        {
            hndl->var.ch_cfg_ptr[chnl].pkts_per_cn =  (FLOAT)t_frame_n / (FLOAT)t_frame_d * (FLOAT)(INT32)pkt_per_d / ( ((FLOAT)pkt_per_int * (FLOAT)(INT32)pkt_per_d) + (FLOAT)(INT32)pkt_per_n);
        }
    } else {
        hndl->var.ch_cfg_ptr[chnl].pkts_per_cn = 0;
    }
    
    PMC_LOG_TRACE("pkt_period = %f \n", pkt_period);
    PMC_LOG_TRACE("hndl->var.ch_cfg_ptr[chnl].pkts_per_cn = %d \n", hndl->var.ch_cfg_ptr[chnl].pkts_per_cn);

    hndl->var.ch_cfg_ptr[chnl].cn_base = pkt_size * hndl->var.ch_cfg_ptr[chnl].pkts_per_cn;
    PMC_LOG_TRACE("hndl->var.ch_cfg_ptr[chnl].cn_base = %d \n", hndl->var.ch_cfg_ptr[chnl].cn_base);
    
    /* Calculate MPMA gain */
    pkt_period_s = pkt_period / (FLOAT)UTIL_OPSA_REF_FREQ;

    util_opsa_mpma_gain_calc(pkt_period_s, (FLOAT)corner_frequency, mpma_non_rev_a_instance_type_test(hndl), &(hndl->var.ch_cfg_ptr[chnl].mpma_gain));


    PMC_RETURN(PMC_SUCCESS);

} /* mpma_pkt_period_var_set_float */

/*******************************************************************************
*  mpma_pkt_period_var_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function updates var_t data with channel rate, packet size, packet
*   period information. This function also calculates MPMA gain based on the 
*   packet period.
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - channel to provision
*   client_bitrate    - client bit rate (in Hz)
*   cn_frame_period   - parent Cn frame period that the client is demapped 
*                       from. Only required for cn_mode = 0 (source node  
*                       reference clock timing (B + C mode)).
*                       Set to UTIL_OPSA_T_FRAME_UNUSED if unused.
*   pkt_period_calc   - method for calculating packet period and size.
*                       When using UTIL_OPSA_USE_OIF_xxx_BYTE, 
*                       pkt_size should be set within the valid OIF range,
*                       and pkt_size_range is ignored.
*   pkt_size          - configured packet size
*   pkt_per_int       - configured integer portion of packet period
*   pkt_per_n         - configured numerator of packet period fraction
*   pkt_per_d         - configured denominator of packet period fraction
*   epsilon           - minimum epsilon        
*   corner_frequency  - corner frequency in units of Hz.
*
* OUTPUTS:
*   None.
*   
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mpma_pkt_period_var_set(mpma_handle_t *hndl, UINT32 chnl, 
                                         DOUBLE client_bitrate, 
                                         util_opsa_cn_frame_period_t cn_frame_period,
                                         util_opsa_pkt_period_calc_t pkt_period_calc, 
                                         UINT32 pkt_size,
                                         UINT32 pkt_per_int,
                                         UINT32 pkt_per_n, 
                                         UINT32 pkt_per_d, DOUBLE epsilon,
                                         UINT32 corner_frequency)
{
    PMC_ERROR result;

    PMC_ENTRY();

    if (!digi_use_float32_get())
    {
        result = mpma_pkt_period_var_set_double(hndl, chnl, 
                                         client_bitrate, 
                                         cn_frame_period,
                                         pkt_period_calc, 
                                         pkt_size,
                                         pkt_per_int,
                                         pkt_per_n, 
                                         pkt_per_d, epsilon,
                                         corner_frequency);
    }
    else
    {
        result = mpma_pkt_period_var_set_float(hndl, chnl, 
                                         client_bitrate, 
                                         cn_frame_period,
                                         pkt_period_calc, 
                                         pkt_size,
                                         pkt_per_int,
                                         pkt_per_n, 
                                         pkt_per_d, epsilon,
                                         corner_frequency);
    }

    PMC_RETURN(result);
} /* mpma_pkt_period_var_set */

/*******************************************************************************
*  mpma_cfc_pages_thres_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function for mpma_ch_prov and sets up the cfc_num_pages
*   and sets the xoff_thres
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - channel to operate on
*   client_bitrate    - client bitrate in bps
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
PRIVATE void mpma_cfc_pages_thres_set(mpma_handle_t *hndl, UINT32 chnl,
                                      DOUBLE client_bitrate)
{
    mapotn_db_key       key;
    UINT32              num_cal_entries;

    PMC_ENTRY();

    /* Get the the number of pages for CFC */
    PMC_LOG_TRACE("Normal operation\n");   
    if(hndl->cfg.default_param.cal_entries_alloc_method == MPMA_DB_CAL_ENTRIES_ALLOC_METHOD)
    {
        
        PMC_LOG_TRACE("database is: %d\n", hndl->cfg.default_param.db_id);
       
        /* go throught the database and get the number of calendar entries for 
           the channel */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.channel_num = chnl;
        num_cal_entries=gen_db_query_count(&(hndl->base),hndl->cfg.default_param.db_id,hndl->cfg.default_param.db_handle, (void *)&key);

        PMC_ASSERT(num_cal_entries != 0, MPMA_LOG_ERR_DB_ENTRY_NOT_FOUND, 0, 0);

        /* number of pages in CFC needs to match the number of calendar slots
        ** for the client rate
        */
        hndl->var.ch_cfg_ptr[chnl].cfc_num_pages = num_cal_entries;

    }   
    else 
    {
        PMC_LOG_TRACE("EMULATION or non DB access\n");
        /* for other variations */
        hndl->var.ch_cfg_ptr[chnl].cfc_num_pages = mpma_cfc_num_pages_calc(hndl, client_bitrate);
    } 
    PMC_LOG_TRACE("num_pages = %u\n", hndl->var.ch_cfg_ptr[chnl].cfc_num_pages);
    
    
    PMC_RETURN();
} /* mpma_cfc_pages_thres_set */

/*******************************************************************************
*  mpma_ch_activate_helper
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function for mpma_ch_activate. 
*   It writes some MPMA settings to hardware and builds the CFC FIFO
*
* INPUTS:
*   *hndl           - pointer to MPMA handle instance
*   chnl            - channel to activate
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
PRIVATE PMC_ERROR mpma_ch_activate_helper(mpma_handle_t *hndl, UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* Write MPMA settings to hardware */
    mpma_ch_init(hndl, chnl);
    mpma_ch_bw_cfg(hndl, chnl);

    /* Build CFC FIFO */
    ret_val = cfc_fifo_build(hndl->cfc_handle, chnl, hndl->var.ch_cfg_ptr[chnl].cfc_num_pages);


    /* take the specified CFC FIFO out of reset */
    if (ret_val == PMC_SUCCESS) {
        ret_val = cfc_fifo_activate(hndl->cfc_handle,chnl);
    }


    PMC_RETURN(ret_val);
} /* mpma_ch_activate_helper */

/*******************************************************************************
*  mpma_ch_enable_helper
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function for mpma_ch_activate. 
*   It enables the MPMA channel
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - channel to enable 
*   dsp_datapath      - TRUE: enable MPMA DSP. FALSE: disable MPMA DSP.
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
PRIVATE void mpma_ch_enable_helper(mpma_handle_t *hndl, UINT32 chnl, BOOL dsp_datapath)
{
    PMC_ENTRY();

    /* Enable the MPMA channel */
    mpma_ch_enable_cfg(hndl, chnl, TRUE);
#if  defined(PMC_SW_SIMULATION)
    if(dsp_datapath == TRUE)
    {
        mpma_dsp_ch_enable_cfg(hndl, chnl, TRUE);
    }
#endif /* defined(PMC_EMULATION) || defined(PMC_VERIFICATION) || defined(PMC_SW_SIMULATION) */
    PMC_RETURN();
} /* mpma_ch_enable_helper */

/*******************************************************************************
*  mpma_ch_deactivate_helper
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function for mpma_ch_deactivate. 
*   It destroys the CFC FIFO
*
* INPUTS:
*   *hndl           - pointer to MPMA handle instance
*   chnl            - channel to deactivate
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
PRIVATE PMC_ERROR mpma_ch_deactivate_helper(mpma_handle_t *hndl, UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* Destroy CFC FIFO */
    ret_val = cfc_fifo_destroy(hndl->cfc_handle, chnl);

    PMC_RETURN(ret_val);
} /* mpma_ch_deactivate_helper */

/*******************************************************************************
*  mpma_ch_disable_helper
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function for mpma_ch_deactivate. 
*   It disables the MPMA channel
*
* INPUTS:
*   *hndl           - pointer to MPMA handle instance
*   chnl            - channel to disable 
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
PRIVATE void mpma_ch_disable_helper(mpma_handle_t *hndl, UINT32 chnl)
{
    PMC_ENTRY();

    /* Disable the MPMA channel */
    mpma_ch_enable_cfg(hndl, chnl, FALSE);
    mpma_dsp_ch_enable_cfg(hndl, chnl, FALSE);

    PMC_RETURN();
} /* mpma_ch_disable_helper */


/*******************************************************************************
*  mpma_pkt_period_var_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrievs var_t data of packet size, packet period information
*   for a channel. 
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - channel to operate on
*
* OUTPUTS:
*   *pkt_size_ptr     - pointer to configured packet size
*   *pkt_per_int_ptr  - pointer to configured integer portion of packet period
*   *pkt_per_n_ptr    - pointer to configured numerator of packet period fraction
*   *pkt_per_d_ptr    - pointer to configured denominator of packet period fraction
*   *epsilon_ptr      - pointer to configured minimum epsilon  
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR mpma_pkt_period_var_get(mpma_handle_t *hndl, UINT32 chnl, 
                                         UINT32 *pkt_size_ptr,
                                         UINT32 *pkt_per_int_ptr,
                                         UINT32 *pkt_per_n_ptr, 
                                         UINT32 *pkt_per_d_ptr,
                                         DOUBLE *epsilon_ptr)
{
    /* variable declaration */
    UINT32 ps_ptr = (hndl->var.ch_cfg_ptr[chnl].pkt_size); 
    UINT32 pp_int_ptr = (hndl->var.ch_cfg_ptr[chnl].pkt_period_int);  
    UINT32 pp_n_ptr = (hndl->var.ch_cfg_ptr[chnl].pkt_period_n);  
    UINT32 pp_d_ptr = (hndl->var.ch_cfg_ptr[chnl].pkt_period_d);
    DOUBLE eps_ptr  = (hndl->var.ch_cfg_ptr[chnl].epsilon);

    PMC_ENTRY();

    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].pkt_size=%d \n", hndl->var.ch_cfg_ptr[chnl].pkt_size);
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].pkt_period_int=%d \n", hndl->var.ch_cfg_ptr[chnl].pkt_period_int);
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].pkt_period_n=%d \n", hndl->var.ch_cfg_ptr[chnl].pkt_period_n);
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].pkt_period_d=%d \n", hndl->var.ch_cfg_ptr[chnl].pkt_period_d);
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].epsilon=%f \n", hndl->var.ch_cfg_ptr[chnl].epsilon);
    
    *pkt_size_ptr = ps_ptr;
    *pkt_per_int_ptr = pp_int_ptr;
    *pkt_per_n_ptr = pp_n_ptr;
    *pkt_per_d_ptr = pp_d_ptr; 
    *epsilon_ptr = eps_ptr;

    PMC_RETURN(PMC_SUCCESS);

} /* mpma_pkt_period_var_get */

/*******************************************************************************
*  mpma_schd_pacer_enable_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function controls enabling or disabling of the scheduling pacer.
*   Scheduling pacer needs to be enabled if the upstream block does not 
*   maintain an evenly space scheduling calendar. 
*   
*   The following rule applies to MPMA instances in COREOTN.
*   For FO1 MPMA: Set to 0 if the channel is unchannelized.
*                 Set to 1 if the channel is muxed.
*   For FO2 MPMA: Always set to 1
*   For CTL MPMA: Always set to 0 
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   enable            - enables scheduling pacer mode \n
*                       TRUE -- enables scheduling pacer mode \n
*                       FALSE -- disables scheduling pacer mode
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
PUBLIC PMC_ERROR mpma_schd_pacer_enable_cfg(mpma_handle_t *hndl, BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    if(mpma_non_rev_a_instance_type_test(hndl) == TRUE)
    {
        PMC_ASSERT(enable == FALSE, MPMA_ERR_CODE_ASSERT, 0, 0);
    }

    mpma_dsp_field_SCHEDULING_PACER_EN_set(NULL, hndl, enable);
    
    PMC_RETURN(result);
} /* mpma_schd_pacer_enable_cfg */


/*******************************************************************************
*  mpma_ch_pkt_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the packet size/period configuration
*
* INPUTS:
*   *hndl           - pointer to MPMA handle instance
*   chnl            - channel to operate on
*
* OUTPUTS:
*   *pkt_size_ptr   - pointer to configured packet size
*   *pkts_per_cn    - pointer to configured packet per cn
*   *cn_base        - pointer to configured cn base
*   
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void mpma_ch_pkt_cfg_get(mpma_handle_t *hndl, 
                                UINT32 chnl,
                                UINT32 *pkt_size_ptr,
                                UINT32 *pkts_per_cn,
                                UINT32 *cn_base)
{

    PMC_ENTRY();
 
    PMC_ASSERT(hndl!=NULL, MPMA_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    *pkt_size_ptr    = mpma_field_PKT_SIZE_get(NULL, hndl, chnl);
    *pkts_per_cn     = mpma_dsp_field_PKTS_PER_CN_get(NULL, hndl, chnl);
    *cn_base         = mpma_dsp_field_CN_BASE_get(NULL, hndl, chnl);

    PMC_RETURN();

} /* mpma_ch_pkt_cfg_get */

/*******************************************************************************
*  mpma_non_default_csi_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures non-default register settings for CSI value as
*   defined in OIF recommendations.
*
* INPUTS:
*   *hndl           - pointer to MPMA handle instance
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   OIF Standard values: \n
*     csi_type            value \n
*     MPMA_CSI_NORMAL     0x04  \n
*     MPMA_CSI_SSF        0x10  \n
*     MPMA_CSI_SF         0x0C  \n
*     MPMA_CSI_SD         0x08  \n
*     MPMA_CSI_FORCE_TO   0x00  \n
*     MPMA_CSI_FORCE_AWAY 0x1C  \n
*
*******************************************************************************/
PRIVATE PMC_ERROR mpma_non_default_csi_cfg(mpma_handle_t *hndl)
{
    PMC_ERROR result = PMC_SUCCESS;
    mpma_buffer_t mpma_buf[1];
    
    PMC_ENTRY();
    
    mpma_buffer_init(mpma_buf, hndl);
    mpma_field_CSI_FORCE_AWAY_set(mpma_buf, hndl, 0x1C);
    mpma_field_CSI_SSF_set(mpma_buf, hndl, 0x10);
    mpma_buffer_flush(mpma_buf);
    
    mpma_field_CSI_SF_set(mpma_buf, hndl, 0x0C);
    mpma_field_CSI_SD_set(mpma_buf, hndl, 0x08);
    mpma_field_CSI_NORMAL_set(mpma_buf, hndl, 0x04);
    mpma_field_CSI_FORCE_TO_set(mpma_buf, hndl, 0x00);
    mpma_buffer_flush(mpma_buf);

    PMC_RETURN(result);

} /* mpma_non_default_csi_cfg */


/*******************************************************************************
*  mpma_alloc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*  
*  Handle allocation of local non-context memory.
*
* INPUTS:
*   *mpma_handle         - pointer to MPMA handle instance
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
PUBLIC void mpma_alloc_init(mpma_handle_t *mpma_handle)
{
    PMC_ENTRY();
    cfc_alloc_init(mpma_handle->cfc_handle);
    scbs3_alloc_init(mpma_handle->scbs3_handle);

    PMC_RETURN();
}

/*******************************************************************************
*  mpma_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on MPMA handle instance.
*
* INPUTS:
*   *mpma_handle                - pointer to MPMA handle instance
*   *restart_init_cfg_ptr       - pointer to restart init config
*   top_energy_state            - The expected energy state 
*   ptr_init_only               - TRUE: update the var, audit registers, etc.
*                                 FALSE: only call handle_restart_init on
*                                 children
*   ptr_init_only               - TRUE: only initialize dynamically allocated pointers\n
*                                 FALSE: perform full handle restart
*
* OUTPUTS:
*   mismatch_mpma_channels      - Indicates a mismatch between the 
*                                 register space and context.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*  
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mpma_handle_restart_init(mpma_handle_t *mpma_handle,
                                          util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                          pmc_energy_state_t top_energy_state,
                                          BOOL8 ptr_init_only,
                                          BOOL8 * mismatch_mpma_channels)
{
    PMC_ERROR result = PMC_SUCCESS;

    mpma_var_t * register_var = NULL;

    cfc_cfg_t   cfc_cfg;

    PMC_ENTRY();  
    cfc_cfg.max_fifo_num = mpma_handle->cfg.cfc_cfg.max_fifo_num;
    cfc_cfg.page_size    = mpma_handle->cfg.cfc_cfg.page_size;
    cfc_handle_restart_init(mpma_handle->cfc_handle, &cfc_cfg, restart_init_cfg_ptr, top_energy_state);

    if (TRUE == mpma_handle->cfg.scbs3_cfg.scbs3_present) 
    {
        result = scbs3_handle_restart_init(mpma_handle->scbs3_handle,restart_init_cfg_ptr, PMC_ENERGY_STATE_RUNNING, TRUE);
    }

    if (PMC_SUCCESS == result && FALSE == ptr_init_only)
    {
        register_var = (mpma_var_t*)PMC_CALLOC(sizeof(mpma_var_t));
        register_var->ch_cfg_ptr = (mpma_ch_cfg_struct_t*)PMC_CALLOC(sizeof(mpma_ch_cfg_struct_t)*80);   

        result = mpma_audit_var_data_register_get(mpma_handle, 
                                                  register_var);
                                                  

        result = mpma_audit_ctxt_register_compare_clean(mpma_handle,
                                                        &(mpma_handle->var),
                                                        register_var,
                                                        restart_init_cfg_ptr,
                                                        mismatch_mpma_channels);

        PMC_FREE(&(register_var->ch_cfg_ptr));
        PMC_FREE(&(register_var));
    }

    PMC_RETURN(result);
} /* mpma_handle_restart_init */


/*******************************************************************************
*  mpma_audit_var_data_register_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Generates mpma_var_t structure data from reading device registe space.
*
*   Not all fields are updated.  
* 
*
* INPUTS:
*   *mpma_handle            - pointer to MPMA handle instance
*
*
* OUTPUTS:
*   *var_ptr                - pointer to mpma_var_t type structure with 
*                             subelements defined
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*  
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mpma_audit_var_data_register_get(mpma_handle_t *mpma_handle, 
                                                   mpma_var_t *var_ptr)
{

    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i, ch_en, pkt_size, pkts_per_cn, cn_base;
    PMC_ENTRY();
   
    for(i=0;i<80;i++)
    {
        ch_en = mpma_ch_enable_get(mpma_handle, i);
        if(ch_en == 1)
            var_ptr->ch_cfg_ptr[i].state = MPMA_CHNL_OPERATIONAL;
        else
            var_ptr->ch_cfg_ptr[i].state = MPMA_CHNL_START;


        mpma_ch_pkt_cfg_get(mpma_handle, 
                            i,
                            &pkt_size,
                            &pkts_per_cn,
                            &cn_base);

        var_ptr->ch_cfg_ptr[i].pkt_size = pkt_size;
        var_ptr->ch_cfg_ptr[i].pkts_per_cn = pkts_per_cn;
        var_ptr->ch_cfg_ptr[i].cn_base = cn_base;

        var_ptr->ch_cfg_ptr[i].mpma_gain = mpma_dsp_field_MPMA_GAIN_get(NULL, mpma_handle, i);


    }

    PMC_RETURN(result);
} /* mpma_audit_var_data_register_get */

/*******************************************************************************
*  mpma_audit_ctxt_register_compare_clean
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function compared context with register space.  It will identify
*   if there is a mismatch.\n\n
*
*   Only recoverable and relevant fields from device registers are compared.\n\n
*
*   Results of comparison output with mismatch_*_ptr parameters.
*   Logical meaning:\n
*        TRUE:  Mismatch present
*        FALSE: No mismatch present
*
*   In the case of crash restart mode it will attempt to correct a mismatch
*   by returning the mismatch back to defualt state in the register and
*   context space.\n
* 
*
* INPUTS:
*   *mpma_handle            - pointer to MPMA handle instance
*   *ctxt_var_ptr           - pointer to Context view of device configuration
*   *reg_var_ptr            - pointer to register space recovered view of context
*   *restart_init_cfg_ptr   - pointer to restart init config
*
* OUTPUTS:
*   *mismatch_channels      - pointer to channels mismatch status
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*  
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mpma_audit_ctxt_register_compare_clean(mpma_handle_t *mpma_handle,
                                                         mpma_var_t *ctxt_var_ptr,
                                                         mpma_var_t *reg_var_ptr,
                                                         util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                                         BOOL8 * mismatch_channels)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    UINT32 ch_array[1] = {0xFF};
    *mismatch_channels = FALSE;

    PMC_ENTRY();

    /* For MPMA, checking these 4 parameters is sufficient */
   
    for(i=0;i<80;i++)
    {
        if(ctxt_var_ptr->ch_cfg_ptr[i].state != reg_var_ptr->ch_cfg_ptr[i].state)
        {
            *mismatch_channels = TRUE;
            if(!restart_init_cfg_ptr->is_warm_restart)
            {
                ch_array[0] = i;
                PMC_LOG_TRACE("mpma_audit_ctxt_register_compare_clean - Calling mpma_ch_clean on channel %u\n",i);
                mpma_ch_clean(mpma_handle,ch_array,1);
            }
        } 
        else if( (ctxt_var_ptr->ch_cfg_ptr[i].state != MPMA_CHNL_START) &&
                     (ctxt_var_ptr->ch_cfg_ptr[i].pkt_size != reg_var_ptr->ch_cfg_ptr[i].pkt_size ||
                      ctxt_var_ptr->ch_cfg_ptr[i].pkts_per_cn != reg_var_ptr->ch_cfg_ptr[i].pkts_per_cn ||
                      ctxt_var_ptr->ch_cfg_ptr[i].cn_base != reg_var_ptr->ch_cfg_ptr[i].cn_base ||
                      ctxt_var_ptr->ch_cfg_ptr[i].mpma_gain != reg_var_ptr->ch_cfg_ptr[i].mpma_gain))
        {
            *mismatch_channels = TRUE;
            if(!restart_init_cfg_ptr->is_warm_restart)
            {
                ch_array[0] = i;
                PMC_LOG_TRACE("mpma_audit_ctxt_register_compare_clean - Calling mpma_ch_clean on channel %u\n",i);
                mpma_ch_clean(mpma_handle,ch_array,1);
            }
        }

    }

    PMC_RETURN(result);
} /* mpma_audit_ctxt_register_compare_clean */

/*******************************************************************************
*  mpma_internal_ch_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provisions a channel for the specified rate.
*   This function determines the parameters to configure for the MPMA datapath
*   and DSP. It also evaluates the scheduler to see if the specified rate can
*   be allocated.
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*   chnl              - channel to provision
*   client_bitrate    - client bit rate (in Hz)
*   pkt_size          - desired minimum packet size. 
*                       Set pkt_size to 0 to use internal defaults.
*   cn_frame_period   - parent Cn frame period that the client is demapped 
*                       from. Only required for cn_mode = 0 (source node  
*                       reference clock timing (B + C mode)).
*                       Set to UTIL_OPSA_T_FRAME_UNUSED if unused.
*   pkt_period_calc   - method for calculating packet period and size. \n
*                       When using UTIL_OPSA_USE_OIF_xxx_BYTE, 
*                       pkt_size should be set within the valid OIF range,
*                       and pkt_size_range is ignored.
*   scbs3_user_bits_3_0 - SCBS3 userbits [3:0] to output with the channel
*                         during the calendar slot.
*                         These bits might be used as downstream MUX 
*                         select bits (for bus aggregation).
*                         Set to 0 if unused.
*   sch_only          - set to TRUE if this channel is to utilize the MPMA
*                       scheduler functionality only (light) or FALSE for
*                       full MPMA with CFC FIFOs
*   pkt_per_int       - configured packet size
*   pkt_per_n         - configured numerator of packet period fraction
*   pkt_per_d         - configured denominator of packet period fraction
*   epsilon           - minimum epsilon
*   corner_frequency  - corner frequency in units of Hz.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR mpma_internal_ch_prov(mpma_handle_t *hndl, UINT32 chnl,
                                        DOUBLE client_bitrate,
                                        UINT32 pkt_size, 
                                        util_opsa_cn_frame_period_t cn_frame_period,
                                        util_opsa_pkt_period_calc_t pkt_period_calc, 
                                        UINT32 scbs3_user_bits_3_0, BOOL sch_only, 
                                        UINT32 pkt_per_int, UINT32 pkt_per_n,
                                        UINT32 pkt_per_d, DOUBLE epsilon,
                                        UINT32 corner_frequency)                              
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    /* only used for calculator function */
    UINT32 pkt_size_loc;
    UINT32 pkt_per_int_loc;
    UINT32 pkt_per_n_loc;
    UINT32 pkt_per_d_loc;
    DOUBLE eps_loc;

    PMC_ENTRY();

    if (hndl->var.ch_cfg_ptr[chnl].state != MPMA_CHNL_START) {
        PMC_RETURN(MPMA_LOG_CODE_INVALID_CHANNEL_STATE);
    }
    
    /* If the channel was not set in schedule only mode, 
       perform typical MPMA provisioning */
    if (sch_only == FALSE) 
    {
        if ((hndl->cfg.default_param.mpma_type_id != MPMA_COREOTN) 
            && (hndl->cfg.default_param.mpma_type_id != MPMA_COREOTN_FROM_MAPOTN) 
            && (hndl->cfg.default_param.mpma_type_id != MPMA_COREOTN_CTL)
            && (hndl->cfg.default_param.mpma_type_id != MPMA_MAPOTN)
            )
        {     
            ret_val = mpma_pkt_period_calc(hndl, client_bitrate, pkt_size, 
                                           hndl->cfg.default_param.pkt_size_range, 
                                           cn_frame_period, pkt_period_calc, 
                                           &pkt_size_loc, &pkt_per_int_loc, 
                                           &pkt_per_n_loc, &pkt_per_d_loc, 
                                           &eps_loc);

            pkt_size = pkt_size_loc;
            pkt_per_int = pkt_per_int_loc;
            pkt_per_n = pkt_per_n_loc;
            pkt_per_d = pkt_per_d_loc;
            epsilon = eps_loc;
        }
        
        if (PMC_SUCCESS == ret_val) 
        {
            ret_val = mpma_pkt_period_var_set(hndl, chnl, client_bitrate, 
                                              cn_frame_period, pkt_period_calc, 
                                              pkt_size, pkt_per_int,
                                              pkt_per_n, pkt_per_d, epsilon, corner_frequency); 
        }                            
    }
    
    /* Set the sch_only state of the chanel */
    hndl->var.ch_cfg_ptr[chnl].sch_only = sch_only;

    /* All channels must be scheduled regardless of scheduler mode */
    if (PMC_SUCCESS == ret_val)
    {
        ret_val = mpma_schd_ch_prov(hndl, chnl, client_bitrate, 
                                    scbs3_user_bits_3_0);
    }

    /* If the channel was not set in schedule only mode, 
       perform typical MPMA provisioning */
    if (PMC_SUCCESS == ret_val)
    {
        mpma_cfc_pages_thres_set(hndl, chnl, client_bitrate);
    }
    
    PMC_RETURN(ret_val);

} /* mpma_internal_ch_prov */

/*******************************************************************************
*  mpma_non_rev_a_instance_type_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function determines the MPMA block revision type.
*
* INPUTS:
*   *hndl             - pointer to MPMA handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8             - TRUE:  MPMA instance is not Rev A type \n
*                       FALSE: MPMA instance is Rev A type
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 mpma_non_rev_a_instance_type_test(mpma_handle_t *hndl)
{

    PMC_ENTRY();

    if(FALSE == pmc_is_digi_rev_a_revision(&hndl->base))
    {
        /* Non Rev A device use non Rev A MPMA for FO1, FO2,  */
        if(hndl->cfg.default_param.mpma_type_id == MPMA_COREOTN ||
           hndl->cfg.default_param.mpma_type_id == MPMA_COREOTN_FROM_MAPOTN)
            PMC_RETURN(TRUE);
    }        


    PMC_RETURN(FALSE);

} /* mpma_non_rev_a_instance_type_test */

/*******************************************************************************
*  mpma_ghao_fifo_resize
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resize the CFC fifo to the specified number of tribslots.
*
* INPUTS:
*   *hndl                  - pointer to MPMA handle instance
*   chnl                   - channel to resize
*   oduflex_num_tribslots  - final number of ODUflex tribslots
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
PUBLIC PMC_ERROR mpma_ghao_fifo_resize(mpma_handle_t *hndl,
                                       UINT32 chnl,
                                       UINT32 oduflex_num_tribslots)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    DOUBLE client_bitrate = 0;
    PMC_ENTRY();


    
    /* Save the number of pages for CFC */
    if(hndl->cfg.default_param.cal_entries_alloc_method == MPMA_DB_CAL_ENTRIES_ALLOC_METHOD)
    {
        hndl->var.ch_cfg_ptr[chnl].cfc_num_pages = oduflex_num_tribslots;
    }
    else
    {
        if (oduflex_num_tribslots >= 1 && oduflex_num_tribslots <= 8)
        {
            /* 1.24917723 Gb/s per TS*/
            client_bitrate = (oduflex_num_tribslots * UTIL_GLOBAL_ODU2_TS_RATE);
        }
        else if (oduflex_num_tribslots <= 32)
        {
            /* 1.25447054 Gb/s per TS*/
            client_bitrate = (oduflex_num_tribslots * UTIL_GLOBAL_ODU3_TS_RATE);
        }
        else if (oduflex_num_tribslots <= 80)
        {
            /* 1.301467133 Gb/s per TS*/
            client_bitrate = (oduflex_num_tribslots * UTIL_GLOBAL_ODU4_TS_RATE);
        }

        hndl->var.ch_cfg_ptr[chnl].cfc_num_pages = mpma_cfc_num_pages_calc(hndl, client_bitrate);
    }

    /* Resize CFC FIFO */
    ret_val = cfc_fifo_resize(hndl->cfc_handle,
                              chnl, 
                              hndl->var.ch_cfg_ptr[chnl].cfc_num_pages);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mpma_ghao_fifo_resize */

/*******************************************************************************
*  mpma_ghao_bwr_end_param_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Set new PKT_SIZE, PKT_PERIOD_INT, PKT_PERIOD_N, PKT_PERIOD_D, MPMA_GAIN.
*  This can be called at the end of the G.HAO ramp, but must be called before
*  turning the DSP back on.
*
* INPUTS:
*   *hndl                  - pointer to MPMA handle instance
*   chnl                   - channel to resize
*   odu_bitrate            - client rate in bits per second
*   cn_frame_period        - Cn frame period (see util_opsa_cn_frame_period_t)
*   pkt_size               - packet size returned from OIF calculator
*   pkt_per_int            - integer packets per period from OIF calculator
*   pkt_per_n              - Decimal (numerator) pkt per period
*   pkt_per_d              - Decimal (denomenator) pkt per period
*   eps_loc                - epsilon returned from OIF calculator
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
PUBLIC PMC_ERROR mpma_ghao_bwr_end_param_set(mpma_handle_t *hndl,
                                             UINT32 chnl,
                                             DOUBLE odu_bitrate,
                                             util_opsa_cn_frame_period_t cn_frame_period,
                                             UINT32 pkt_size,
                                             UINT32 pkt_per_int,
                                             UINT32 pkt_per_n,
                                             UINT32 pkt_per_d,
                                             DOUBLE eps_loc)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* Set new packet size */        
    if (PMC_SUCCESS == ret_val) 
    {
            /*sets the following values in context:
            - pkt_size
            - pkt_period_int
            - pkt_period_n
            - pkt_period_d
            - epsilon
            - pkt_per cn
            - cn base
            - mpma gain */


            ret_val = mpma_pkt_period_var_set(hndl, 
                                              chnl,
                                              odu_bitrate,
                                              cn_frame_period,
                                              UTIL_OPSA_EPS_MIN,
                                              pkt_size,
                                              pkt_per_int,
                                              pkt_per_n,
                                              pkt_per_d,
                                              eps_loc, 50);
    }

 

    if(ret_val == PMC_SUCCESS)
    {
          /*NOTE a special sequence is required to change the pkt_size hitlessly 
          - Force the MPMA the use "default pkt size" for the channel you 
            want to change
          - Change the pkt size, while in force mode
          - remove the force mode*/

          /* set DEFAULT_PKT_SIZE to desired value */
          mpma_field_DEFAULT_PKT_SIZE_set(NULL, hndl, pkt_size);


          /* set DEFAULT_PKT_SIZE_EN */
          mpma_field_DEFAULT_PKT_SIZE_EN_set(NULL, hndl, chnl, 1);


          /* set FORCE_DEFAULT_PKT_SIZE to 1*/
          mpma_field_FORCE_DEFAULT_PKT_SIZE_set(NULL, hndl, 1);

          /*
          To update these call mpma_ch_bw_cfg()
          - DSP PKT_SIZE
          - DSP PKT_PERIOD_INT , PKT_PERIOD_N, PKT_PERIOD_D
          - DSP MPMA_GAIN
          - DSP PKTS_PER_CN
          - DSP CN_BASE */

          ret_val = mpma_ch_bw_cfg(hndl,
                                   chnl);


          /* set FORCE_DEFAULT_PKT_SIZE to 0 */
          mpma_field_FORCE_DEFAULT_PKT_SIZE_set(NULL, hndl, 0);


          /* set DEFAULT_PKT_SIZE_EN to 0 */
          mpma_field_DEFAULT_PKT_SIZE_EN_set(NULL, hndl, chnl, 0);



    }

    PMC_RETURN(ret_val);

} /* mpma_ghao_bwr_end_param_set */

/*******************************************************************************
* mpma_ghao_bwr_ind_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets BWR_IND in the MPMA that that it can get tunneled on the DPI
*   interface.
*
* INPUTS:
*   *hndl    - pointer to MPMA handle instance
*   chnl     - channel to configure
*   bwr_ind  - 0 : broadcast BWR_IND = 0 on DPI type interface\n
*              1 : broadcast BWR_IND = 1 on DPI type interface
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
PUBLIC PMC_ERROR mpma_ghao_bwr_ind_set(mpma_handle_t *hndl,
                                       UINT32 chnl,
                                       UINT32 bwr_ind)
{
    PMC_ENTRY();
    
    /* Enable/disable MPMA DSP */
    mpma_field_BWR_IND_set(NULL, hndl, chnl, bwr_ind);
    
    PMC_RETURN(PMC_SUCCESS);
} /* mpma_ghao_bwr_ind_set */

/*******************************************************************************
* mpma_int_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   *mpma_handle     - pointer to MPMA handle instance
*   chnl_id          - chnl id
*   non_null_ptr     - Pointer to check that shouldn't be NULL
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mpma_int_validate(mpma_handle_t *mpma_handle,
                                    UINT32 chnl_id,
                                    void *non_null_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != mpma_handle, MPMA_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (chnl_id >= mpma_handle->cfg.default_param.num_chnl)
    {
        result = MPMA_LOG_ERR_INVALID_PARAMETERS;
    }
    
    if (NULL == non_null_ptr)
    {
        result = MPMA_LOG_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(result);
} /* mpma_int_validate */

/*
** End of file
*/
