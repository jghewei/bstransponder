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
#include "mpmo_loc.h"


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
/* Configured frame periods in the MPMO DSP. */

PRIVATE const mpmo_frm_period_cnt_t mpmo_frm_period_cnt[MPMO_NUM_FRM_PERIOD_CNT] =   /*!< see mpmo_frm_period_cnt_t*/
{
    /*      frm_period_type,         frm_period_n,      frm_period_d */
    {UTIL_OPSA_MAPOTN_ODU0_T_FRAME,       MAPOTN_ODU0_T_FRAME_N,    MAPOTN_ODU0_T_FRAME_D},
    {UTIL_OPSA_MAPOTN_ODU1_T_FRAME,       MAPOTN_ODU1_T_FRAME_N,    MAPOTN_ODU1_T_FRAME_D},
    {UTIL_OPSA_MAPOTN_ODU2_T_FRAME,       MAPOTN_ODU2_T_FRAME_N,    MAPOTN_ODU2_T_FRAME_D},
    {UTIL_OPSA_MAPOTN_ODU2E_T_FRAME,     MAPOTN_ODU2E_T_FRAME_N,   MAPOTN_ODU2E_T_FRAME_D},
    {UTIL_OPSA_MAPOTN_ODU3_T_FRAME,       MAPOTN_ODU3_T_FRAME_N,    MAPOTN_ODU3_T_FRAME_D},
    {UTIL_OPSA_MAPOTN_ODU3E1_T_FRAME,   MAPOTN_ODU3E1_T_FRAME_N,  MAPOTN_ODU3E1_T_FRAME_D},
    {UTIL_OPSA_MAPOTN_ODU3E2_T_FRAME,   MAPOTN_ODU3E2_T_FRAME_N,  MAPOTN_ODU3E2_T_FRAME_D},
    {UTIL_OPSA_MAPOTN_ODU4_T_FRAME,       MAPOTN_ODU4_T_FRAME_N,    MAPOTN_ODU4_T_FRAME_D},
};



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
const char MPMO_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    MPMO_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char MPMO_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward References
*/
PRIVATE PMC_ERROR mpmo_int_validate(mpmo_handle_t *mpmo_handle,
                                    UINT32 chnl_id,
                                    void *non_null_ptr);
PRIVATE void mpmo_var_default_init(mpmo_handle_t *hndl);  
PRIVATE PMC_ERROR mpmo_cn_frame_period_get(mpmo_handle_t *hndl,
                                           util_opsa_cn_frame_period_t cn_frame_period,
                                           UINT32 *frm_period_idx, UINT32 *cn_frm_per_n, UINT32 *cn_frm_per_d);                                         
PRIVATE PMC_ERROR mpmo_ch_init(mpmo_handle_t *hndl, UINT32 chnl);
PRIVATE PMC_ERROR mpmo_dsp_ch_enable_cfg(mpmo_handle_t *hndl, UINT32 chnl, BOOL enable);
PRIVATE UINT32 mpmo_cfc_num_pages_calc(mpmo_handle_t *hndl, DOUBLE client_bitrate);

PRIVATE PMC_ERROR mpmo_ch_ssf_cn_center_cfg(mpmo_handle_t *hndl, UINT32 chnl, BOOL enable);
PRIVATE PMC_ERROR mpmo_ch_cn_frame_period_default_cfg(mpmo_handle_t *hndl, UINT32 chnl,
                                                      util_opsa_cn_frame_period_t cn_frame_period);
PRIVATE PMC_ERROR mpmo_ch_cn_frame_period_remove(mpmo_handle_t *hndl, UINT32 chnl);

PRIVATE PMC_ERROR mpmo_audit_var_data_register_get(mpmo_handle_t *mpmo_handle, 
                                                   mpmo_var_t *var_ptr);

PRIVATE PMC_ERROR mpmo_audit_ctxt_register_compare_clean(mpmo_handle_t *mpmo_handle,
                                                         mpmo_var_t *ctxt_var_ptr,
                                                         mpmo_var_t *reg_var_ptr,
                                                         util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                                         BOOL8 * mismatch_channels);
PRIVATE PMC_ERROR mpmo_internal_ch_prov(mpmo_handle_t *hndl, UINT32 chnl,
                                        DOUBLE client_bitrate,
                                        UINT32 pkt_size, UINT32 pkt_per_int,
                                        UINT32 pkt_per_n, UINT32 pkt_per_d,
                                        util_opsa_cn_frame_period_t cn_frame_period,
                                        BOOL golden_cn_en,
                                        BOOL8 bmp_map_mode,
                                        BOOL8 true_prov,
                                        BOOL8 super_prov,
                                        BOOL8 is_sonet,
                                        util_global_odukp_type_t odu_type);
PRIVATE BOOL8 mpmo_non_rev_a_instance_type_test(mpmo_handle_t *hndl);
PRIVATE PMC_ERROR mpmo_ch_activate_internal(mpmo_handle_t *hndl, 
                                            UINT32 chnl, 
                                            BOOL8 super_prov,
                                            BOOL8 true_activate);

PRIVATE PMC_ERROR mpmo_ch_deactivate_internal(mpmo_handle_t *hndl,
                                              UINT32 chnl,
                                              BOOL true_deactivate,
                                              BOOL super_deprov);


PRIVATE PMC_ERROR mpmo_ch_deprov_internal(mpmo_handle_t *hndl, 
                                          UINT32 chnl,
                                          BOOL8 true_deprov,
                                          BOOL8 super_deprov);

/*

 ** Public Functions
*/

/*******************************************************************************
* mpmo_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for an MPMO block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the MPMO subsystem relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifier of the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   mpmo_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC mpmo_handle_t *mpmo_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    mpmo_handle_t *mpmo_handle;

    PMC_ENTRY();
    mpmo_handle = (mpmo_handle_t*)PMC_CTXT_CALLOC(sizeof(mpmo_handle_t), parent);

    pmc_handle_init(parent, mpmo_handle, sys_handle, PMC_MID_DIGI_MPMO, tsb_name, base_address);

    /* Register log strings. */
    pmc_log_block_strings_register(MPMO_LOG_ERR_STRINGS[0], MPMO_LOG_ERR_TABLE_BASE, MPMO_LOG_ERR_COUNT);
    pmc_log_block_strings_register(UTIL_OPSA_LOG_ERR_STRINGS[0], UTIL_OPSA_LOG_ERR_TABLE_BASE, UTIL_OPSA_LOG_ERR_COUNT);

    /* Create context for child blocks. */
    mpmo_handle->cfc_handle   = cfc_ctxt_create((pmc_handle_t *)mpmo_handle, base_address+BASE_ADDR_MPMO_MPMO_DP_CFC, sys_handle, "CFC");
    PMC_CTXT_REGISTER_PTR(&mpmo_handle->cfc_handle, parent);
    mpmo_handle->scbs3_handle = scbs3_ctxt_create((pmc_handle_t *)mpmo_handle, base_address+BASE_ADDR_MPMO_MPMO_DP_SCBS3, sys_handle, "SCBS3");
    PMC_CTXT_REGISTER_PTR(&mpmo_handle->scbs3_handle, parent);

    mpmo_handle->var.ch_cfg_ptr = NULL;

    PMC_RETURN(mpmo_handle);
} /* mpmo_ctxt_create */


/*******************************************************************************
* mpmo_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a MPMO block instance.
*
*
* INPUTS:
*   *mpmo_handle         - pointer to MPMO handle instance
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
PUBLIC void mpmo_ctxt_destroy(mpmo_handle_t *mpmo_handle)
{
    PMC_ENTRY();

    cfc_ctxt_destroy(mpmo_handle->cfc_handle);
    scbs3_ctxt_destroy(mpmo_handle->scbs3_handle);

    if (mpmo_handle->cfg.default_param.db_handle != 0)
    {
        gen_db_detach(&(mpmo_handle->base),
                      mpmo_handle->cfg.default_param.db_id,
                      mpmo_handle->cfg.default_param.db_handle);
    }

    PMC_CTXT_FREE(&(mpmo_handle->var.ch_cfg_ptr), mpmo_handle);
    PMC_CTXT_FREE(&mpmo_handle, mpmo_handle);

    PMC_RETURN();
} /* mpmo_ctxt_destroy */

/*******************************************************************************
* mpmo_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an MPMO block handle instance.
*
*
* INPUTS:
*   *mpmo_handle    - pointer to MPMO handle instance
*   mpmo_type_id    - Enum. See mpmo_type_id_t.
*   scbs3_toc_sync  - controls whether this MPMO needs to sync to top of the
*                     calendar
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
PUBLIC void mpmo_handle_init(mpmo_handle_t *mpmo_handle,
                             mpmo_type_id_t mpmo_type_id,
                             UINT32 scbs3_toc_sync)
{
    UINT32      size;
    cfc_cfg_t   cfc_cfg;
    scbs3_cfg_t scbs_cfg;
    UINT32 i;

    PMC_ENTRY();

    /* Check for valid type ID */
    if (mpmo_type_id >= MPMO_NUM_TYPES) {
        PMC_DEBUG(MPMO_LOG_ERR_INVALID_PARAMETERS, mpmo_type_id, 0);
        PMC_RETURN();
    }

    /* Update static configurations */
    mpmo_handle->cfg.default_param.mpmo_type_id   = mpmo_type_id;
    mpmo_handle->cfg.default_param.num_chnl       = MPMO_NUM_CHNL;
    mpmo_handle->cfg.default_param.dsp_present    = TRUE;
    mpmo_handle->cfg.default_param.zone_sel       = 1;
    mpmo_handle->cfg.default_param.seq_chk_en     = FALSE;
    mpmo_handle->cfg.default_param.pacer_mode     = MPMO_PACER_DEFAULT;
    mpmo_handle->cfg.default_param.ssf_gen_en     = TRUE;
    mpmo_handle->cfg.default_param.ssf_thres_en   = FALSE;
    mpmo_handle->cfg.default_param.bus_width      = MPMO_BUS_WIDTH_48_BYTE;
    mpmo_handle->cfg.default_param.ts_enable      = FALSE;
    mpmo_handle->cfg.default_param.cn_req_mode    = 0;
    mpmo_handle->cfg.default_param.cn_bit_mode    = 0;
    mpmo_handle->cfg.default_param.cal_entries_alloc_method = MPMO_DEFAULT_CAL_ENTRIES_ALLOC_METHOD ; /* as default always use algorithm #1 */
    mpmo_handle->cfg.default_param.db_id          = LAST_UTIL_GEN_DB;
    mpmo_handle->cfg.default_param.mpmo_frm_period_flex_alloc_method = FALSE;

    /*g.hao related configs*/
    mpmo_handle->cfg.default_param.age_filter_en = 0;
    mpmo_handle->cfg.default_param.ramp_stop_mode = 0;
    mpmo_handle->cfg.default_param.ramp_extend_en = 0;
    mpmo_handle->cfg.default_param.bwr_ind_mode = 0;

    /* CFC parameters */
    /* Number of FIFO channels in CFC. Equivalent to channel count. */
    mpmo_handle->cfg.cfc_cfg.max_fifo_num = MPMO_NUM_CHNL;

    /* SCBS3 parameters */
    mpmo_handle->cfg.scbs3_cfg.scbs3_present    = TRUE;
    mpmo_handle->cfg.scbs3_cfg.total_rate       = (UINT32)MPMO_SCBS3_TOTAL_RATE;
    mpmo_handle->cfg.scbs3_cfg.total_rate_units = MPMO_SCBS3_TOTAL_RATE_UNITS;
    mpmo_handle->cfg.scbs3_cfg.page_swap_mode   = SCBS3_PAGE_SWAP_BIT;

    /* Configure parameters for specific instantiations. */
    switch (mpmo_type_id) {
    case MPMO_COREOTN_MUX_FRAMER:
        mpmo_handle->cfg.default_param.ssf_gen_en = TRUE; 
        
        /* Temporarily disable packet replacement in FO1 MPMO
        ** need to enable this once the bring-up sequence is revised
        */
        mpmo_handle->cfg.default_param.seq_chk_en = FALSE;
        mpmo_handle->cfg.default_param.pacer_mode = MPMO_PACER_AUTOMATIC;
        mpmo_handle->cfg.default_param.ts_enable  = TRUE;
        mpmo_handle->cfg.default_param.cal_entries_alloc_method = MPMO_COREOTN_MUX_FRAMER_DB_CAL_ENTRIES_ALLOC_METHOD;
        mpmo_handle->cfg.default_param.db_id     = UTIL_GEN_DB_LINE_CORE_OTN_TX_DB; 
        mpmo_handle->cfg.default_param.mpmo_frm_period_flex_alloc_method = TRUE;

        /*g.hao related configs*/
        mpmo_handle->cfg.default_param.age_filter_en = 1;
        mpmo_handle->cfg.default_param.ramp_stop_mode = 0;
        mpmo_handle->cfg.default_param.ramp_extend_en = 0;
        mpmo_handle->cfg.default_param.bwr_ind_mode = 1;

        mpmo_handle->cfg.cfc_cfg.page_size          = MPMO_COREOTN_CFC_PAGE_SIZE;
        mpmo_handle->cfg.default_param.lowest_bw    = MPMO_LOWEST_BANDWIDTH;
        /* SCBS3 data - slave scheduler */
        mpmo_handle->cfg.scbs3_cfg.toc_sync_en      = FALSE;
        mpmo_handle->cfg.scbs3_cfg.toc_sync         = 0;
        break;
    case MPMO_COREOTN_TO_MAPOTN:
        mpmo_handle->cfg.default_param.ssf_gen_en  = TRUE; 
        mpmo_handle->cfg.default_param.dsp_present = FALSE;
        mpmo_handle->cfg.default_param.seq_chk_en  = TRUE;
        mpmo_handle->cfg.default_param.pacer_mode  = MPMO_PACER_CONTINUOUS;
        mpmo_handle->cfg.default_param.ts_enable   = TRUE;
        mpmo_handle->cfg.default_param.cal_entries_alloc_method = MPMO_COREOTN_TO_MAPOTN_DB_CAL_ENTRIES_ALLOC_METHOD;
        mpmo_handle->cfg.default_param.db_id     = UTIL_GEN_DB_ODUKSC_DB; 
        mpmo_handle->cfg.default_param.mpmo_frm_period_flex_alloc_method = TRUE;


           /*g.hao related configs*/     
         mpmo_handle->cfg.default_param.age_filter_en = 1;   
         mpmo_handle->cfg.default_param.ramp_stop_mode = 0;  
         mpmo_handle->cfg.default_param.ramp_extend_en = 0;  
         mpmo_handle->cfg.default_param.bwr_ind_mode = 1;

        mpmo_handle->cfg.cfc_cfg.page_size          = MPMO_COREOTN_CFC_PAGE_SIZE;
        mpmo_handle->cfg.default_param.lowest_bw    = MPMO_LOWEST_BANDWIDTH;
        /* SCBS3 data - slave scheduler */
        mpmo_handle->cfg.scbs3_cfg.toc_sync_en      = FALSE;
        mpmo_handle->cfg.scbs3_cfg.toc_sync         = 0;
        break;
    case MPMO_COREOTN_CTL:
        /* 12 channel version */
        mpmo_handle->cfg.default_param.num_chnl     = MPMO_NUM_CHNL_REDUCED;
        mpmo_handle->cfg.cfc_cfg.max_fifo_num       = MPMO_NUM_CHNL_REDUCED;
        mpmo_handle->cfg.default_param.dsp_present  = FALSE;
        mpmo_handle->cfg.default_param.seq_chk_en   = TRUE;
        mpmo_handle->cfg.default_param.pacer_mode   = MPMO_PACER_AUTOMATIC;
        mpmo_handle->cfg.default_param.ts_enable    = TRUE;
        mpmo_handle->cfg.default_param.cal_entries_alloc_method = MPMA_MPMO_COREOTN_CTL_DB_CAL_ENTRIES_ALLOC_METHOD;
        mpmo_handle->cfg.default_param.db_id        = UTIL_GEN_DB_LINEOTN_SYSOTN_MODE; 

        mpmo_handle->cfg.cfc_cfg.page_size          = MPMO_COREOTN_CTL_CFC_PAGE_SIZE;
        mpmo_handle->cfg.default_param.lowest_bw    = MPMO_LOWEST_BANDWIDTH_REDUCED;

        /* SCBS3 data - slave scheduler */
        mpmo_handle->cfg.scbs3_cfg.toc_sync_en      = FALSE;
        mpmo_handle->cfg.scbs3_cfg.toc_sync         = 0;
        break;
    case MPMO_MAPOTN:
        mpmo_handle->cfg.default_param.zone_sel = 0;
        mpmo_handle->cfg.default_param.cal_entries_alloc_method = MPMO_DB_CAL_ENTRIES_ALLOC_METHOD;
        mpmo_handle->cfg.default_param.db_id     = UTIL_GEN_DB_MPMO_DB;
        mpmo_handle->cfg.default_param.pacer_mode     = MPMO_PACER_ZONE_HOLDOFF;
        /*ssf_gen_en is always false for mapotn */
        mpmo_handle->cfg.default_param.ssf_gen_en     = FALSE;
        mpmo_handle->cfg.default_param.ssf_thres_en   = TRUE;
        mpmo_handle->cfg.cfc_cfg.page_size          = MPMO_MAPOTN_CFC_PAGE_SIZE;
        mpmo_handle->cfg.default_param.lowest_bw    = MPMO_LOWEST_BANDWIDTH;
        /* SCBS3 data - slave scheduler */
        mpmo_handle->cfg.scbs3_cfg.toc_sync_en      = TRUE;
        mpmo_handle->cfg.scbs3_cfg.toc_sync         = 5;
        break;
    case MPMO_ENET:
        /* 12 channel version */
        mpmo_handle->cfg.default_param.num_chnl     = MPMO_NUM_CHNL_REDUCED;
        mpmo_handle->cfg.cfc_cfg.max_fifo_num       = MPMO_NUM_CHNL_REDUCED;
        mpmo_handle->cfg.default_param.dsp_present  = FALSE;
        mpmo_handle->cfg.default_param.zone_sel     = 0;
        mpmo_handle->cfg.cfc_cfg.page_size = MPMO_ENET_CFC_PAGE_SIZE;
        mpmo_handle->cfg.default_param.lowest_bw    = MPMO_LOWEST_BANDWIDTH_REDUCED;
        mpmo_handle->cfg.default_param.pacer_mode   = MPMO_PACER_ZONE_HOLDOFF;

        /* SCBS3 data - NEEDS TO BE FIXED: inheriting from epmm setting */
        mpmo_handle->cfg.scbs3_cfg.scbs3_present    = FALSE;
        mpmo_handle->cfg.scbs3_cfg.toc_sync_en      = TRUE;
        mpmo_handle->cfg.scbs3_cfg.toc_sync         = scbs3_toc_sync;
        break;
    case MPMO_CBRC:
        /* 12 channel version */
        mpmo_handle->cfg.default_param.num_chnl     = MPMO_NUM_CHNL_REDUCED;
        mpmo_handle->cfg.cfc_cfg.max_fifo_num       = MPMO_NUM_CHNL_REDUCED;
        mpmo_handle->cfg.default_param.dsp_present  = FALSE;
        mpmo_handle->cfg.default_param.zone_sel     = 0;
        mpmo_handle->cfg.cfc_cfg.page_size          = MPMO_CBRC_CFC_PAGE_SIZE;
        mpmo_handle->cfg.default_param.lowest_bw    = MPMO_LOWEST_BANDWIDTH_REDUCED;
        mpmo_handle->cfg.default_param.cal_entries_alloc_method = 6; /* set to local mode */

        /* SCBS3 data - slave scheduler */
        mpmo_handle->cfg.scbs3_cfg.toc_sync_en      = FALSE;
        mpmo_handle->cfg.scbs3_cfg.toc_sync         = 0;
        break;
    case MPMO_CBRC_DB:
        /* 12 channel version */
        mpmo_handle->cfg.default_param.num_chnl     = MPMO_NUM_CHNL_REDUCED;
        mpmo_handle->cfg.cfc_cfg.max_fifo_num       = MPMO_NUM_CHNL_REDUCED;
        mpmo_handle->cfg.default_param.dsp_present  = FALSE;
        mpmo_handle->cfg.default_param.zone_sel     = 0;
        mpmo_handle->cfg.cfc_cfg.page_size          = MPMO_CBRC_CFC_PAGE_SIZE;
        mpmo_handle->cfg.default_param.lowest_bw    = MPMO_LOWEST_BANDWIDTH_REDUCED;
        /* set to db mode */
        mpmo_handle->cfg.default_param.cal_entries_alloc_method = UTIL_SCHD_ALLOC_METHOD_CBRC_MPMO_DB;
        mpmo_handle->cfg.default_param.db_id        = UTIL_GEN_DB_CBRC_MPMO;

        /* SCBS3 data - slave scheduler */
        mpmo_handle->cfg.scbs3_cfg.toc_sync_en      = FALSE;
        mpmo_handle->cfg.scbs3_cfg.toc_sync         = 0;
        break;
    default:
        PMC_DEBUG(MPMO_LOG_ERR_INVALID_PARAMETERS, mpmo_type_id, 0);
        break;
    } /* end switch */

    /* allow using the fixed array of Cn frame periods for MAPOTN */
    if ((mpmo_handle->cfg.default_param.mpmo_frm_period_flex_alloc_method) == FALSE)
    {
            mpmo_handle->cfg.default_param.frm_period_cnt_ptr = mpmo_frm_period_cnt;
    }
    else {
        /* otherwise, initialize the array to default state */
        for (i = 0; i < MPMO_NUM_FRM_PERIOD_CNT; i++)
        {
            mpmo_handle->cfg.default_param.frm_period_cnt[i].frm_period_type = UTIL_OPSA_T_FRAME_UNUSED;
            mpmo_handle->cfg.default_param.frm_period_cnt[i].frm_period_n = 0;
            mpmo_handle->cfg.default_param.frm_period_cnt[i].frm_period_d = 1;
        }
    }

    size = sizeof(mpmo_ch_cfg_struct_t) * mpmo_handle->cfg.default_param.num_chnl;
    mpmo_handle->var.ch_cfg_ptr = (mpmo_ch_cfg_struct_t*)PMC_CTXT_CALLOC(size, mpmo_handle);
    PMC_CTXT_REGISTER_PTR(&mpmo_handle->var.ch_cfg_ptr, mpmo_handle);



    /* access DB except for method 1,2 and 6 */
    if( (mpmo_handle->cfg.default_param.cal_entries_alloc_method > 2 &&
        mpmo_handle->cfg.default_param.cal_entries_alloc_method < 6) ||
         (mpmo_handle->cfg.default_param.cal_entries_alloc_method > 6) )
    {
        /* attach to the databse */
        mpmo_handle->cfg.default_param.db_handle = gen_db_attach(&(mpmo_handle->base),mpmo_handle->cfg.default_param.db_id);

    }

    PMC_LOG_TRACE("mpmo_handle->cfg.default_param.db_handle = %u\n", mpmo_handle->cfg.default_param.db_handle);

    /* Initialize var_t parameters */
    mpmo_var_default_init(mpmo_handle);

    /* Initialize CFC */
    cfc_cfg.max_fifo_num = mpmo_handle->cfg.cfc_cfg.max_fifo_num;
    cfc_cfg.page_size    = mpmo_handle->cfg.cfc_cfg.page_size;
    cfc_handle_init(mpmo_handle->cfc_handle, &cfc_cfg);

    /* Initialize SCBS3 if present */
    if (mpmo_handle->cfg.scbs3_cfg.scbs3_present)
    {
        scbs_cfg.num_grp             = MPMO_SCBS3_NUM_GRP;
        scbs_cfg.max_rows            = MPMO_SCBS3_MAX_ROWS;
        scbs_cfg.bonus_mode_enabled  = MPMO_SCBS3_BONUS_MODE;
        scbs_cfg.enable_row_resizing = MPMO_SCBS3_ROW_RESIZE;

        /* Keep 96 entries regardless of MPMO instantiation */
        scbs_cfg.max_chnl          = MPMO_NUM_CHNL;
        scbs_cfg.total_rate        = mpmo_handle->cfg.scbs3_cfg.total_rate;
        scbs_cfg.total_rate_units  = mpmo_handle->cfg.scbs3_cfg.total_rate_units;
        scbs_cfg.toc_sync          = mpmo_handle->cfg.scbs3_cfg.toc_sync;
        scbs_cfg.toc_sync_en       = mpmo_handle->cfg.scbs3_cfg.toc_sync_en;
        scbs_cfg.page_swap_mode    = mpmo_handle->cfg.scbs3_cfg.page_swap_mode;
        scbs_cfg.cal_entries_alloc_method = mpmo_handle->cfg.default_param.cal_entries_alloc_method;

        scbs3_handle_init(mpmo_handle->scbs3_handle, &scbs_cfg);
    }

    mpmo_handle->cfg.num_mult = 1;
    mpmo_handle->cfg.denom_mult = 1;

    PMC_RETURN();

} /* mpmo_handle_init */


/*******************************************************************************
* Initialization Configuration Functions
*******************************************************************************/
/*******************************************************************************
*  mpmo_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize MPMO with parameters specific to this instantiation.
*
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
*   energy_state   - enum defining state to set the MPMO module to
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   This function uses pmc_energy_state_t for defining its energy states as there 
*   are no optimization in energy state required for MPMO module
*
*******************************************************************************/
PUBLIC PMC_ERROR mpmo_init(mpmo_handle_t *hndl, 
                           pmc_energy_state_t energy_state)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 chnl;
    
    PMC_ENTRY();
    PMC_ASSERT(hndl!=NULL, MPMO_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        /* Disable MPMO */
        mpmo_enable_cfg(hndl, FALSE);
        /* Disable all MPMO channels */
        for (chnl = 0; chnl < hndl->cfg.default_param.num_chnl; chnl++) {
            mpmo_ch_enable_cfg(hndl, chnl, FALSE);
        }

        /* Configure MPMO parameters specific to each instantiation */
        mpmo_field_ZONE_SEL_set(NULL, hndl, hndl->cfg.default_param.zone_sel);
        mpmo_field_SEQ_CHK_EN_set(NULL, hndl, hndl->cfg.default_param.seq_chk_en);
        mpmo_field_PACER_MODE_set(NULL, hndl, hndl->cfg.default_param.pacer_mode);
        mpmo_field_SSF_GEN_EN_set(NULL, hndl, hndl->cfg.default_param.ssf_gen_en);
        mpmo_field_SSF_THRES_EN_set(NULL, hndl, hndl->cfg.default_param.ssf_thres_en);

        /*g.hao related configs*/
        mpmo_field_AGE_FILTER_EN_set(NULL, hndl, hndl->cfg.default_param.age_filter_en);
        mpmo_field_RAMP_STOP_MODE_set(NULL, hndl, hndl->cfg.default_param.ramp_stop_mode);
        mpmo_field_RAMP_EXTEND_EN_set(NULL, hndl, hndl->cfg.default_param.ramp_extend_en);
        mpmo_field_BWR_IND_MODE_set(NULL, hndl, hndl->cfg.default_param.bwr_ind_mode);

        mpmo_field_BUS_WIDTH_set(NULL, hndl, hndl->cfg.default_param.bus_width);

        

        /* cn_req_mode is 0 for all instantiations */
        mpmo_dsp_field_CN_REQ_MODE_set(NULL, hndl, hndl->cfg.default_param.cn_req_mode);
    
        mpmo_cn_frame_period_assign(hndl);

        /* Initialize IPT in CFC */
        ret_val = cfc_init(hndl->cfc_handle, energy_state);
        if(PMC_SUCCESS == ret_val)
        {    
            ret_val = cfc_bypass(hndl->cfc_handle, FALSE);
        }

        /* Initialize SCBS3 scheduler if present */
        if (PMC_SUCCESS == ret_val && hndl->cfg.scbs3_cfg.scbs3_present)
        {
            ret_val = scbs3_schd_init(hndl->scbs3_handle, energy_state);
        }
    }
    else if (PMC_ENERGY_STATE_RESET == energy_state)
    {
        mpmo_var_default_init(hndl);
        ret_val = cfc_init(hndl->cfc_handle, energy_state);
        
        if (PMC_SUCCESS == ret_val && hndl->cfg.scbs3_cfg.scbs3_present) 
        {
            ret_val = scbs3_schd_init(hndl->scbs3_handle, energy_state);
        }  
    } else {
        PMC_ASSERT(FALSE, MPMO_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }

    PMC_RETURN(ret_val);

} /* mpmo_init */


/*******************************************************************************
* mpmo_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   MPMO instance.  It will test the channel state for all resources within
*   the MPMO instance if any are not in MPMO_CHNL_START state then FALSE
*   is returned, else TRUE.  Should TRUE be returned it can be assumed that
*   no resources are provisioned against the MPMO and it would be
*   possible for the higher layer module to put the module in lowpower standby
*   mode of operation.
*
*
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
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
PUBLIC BOOL mpmo_start_state_test(mpmo_handle_t *hndl) 
{
    UINT32 chnl;
    PMC_ENTRY();
    PMC_ASSERT(hndl!=NULL, MPMO_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    for (chnl = 0; chnl < hndl->cfg.default_param.num_chnl; chnl++) 
    {
        if(hndl->var.ch_cfg_ptr[chnl].state != MPMO_CHNL_START)
           PMC_RETURN(FALSE);
    } 
    
    if(cfc_start_state_test(hndl->cfc_handle) == FALSE ||
       scbs3_start_state_test(hndl->scbs3_handle) == FALSE)
        PMC_RETURN(FALSE);
    
        
    PMC_RETURN(TRUE);

} /* mpmo_start_state_test */


/*******************************************************************************
*  mpmo_cn_frame_period_assign
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize MPMO with parameters specific to this instantiation.
*
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
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
PUBLIC PMC_ERROR mpmo_cn_frame_period_assign(mpmo_handle_t *hndl)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 idx;
    UINT32 frm_per_n;
    UINT32 frm_per_d;
    
    DOUBLE decimal_d_over_n;

    PMC_ENTRY();

    if (hndl->cfg.default_param.mpmo_frm_period_flex_alloc_method == FALSE)
    {
        /* Configure frame period counters. */
        for (idx = 0; idx < MPMO_NUM_FRM_PERIOD_CNT; idx++) {
            
            if(hndl->cfg.num_mult != 1 || hndl->cfg.denom_mult != 1)
            {
                /* If there is a multiplier applied on the DCI clock, it's require to rationalize
                the cgen_n and cgen_d*/
                decimal_d_over_n = (DOUBLE)(hndl->cfg.default_param.frm_period_cnt_ptr[idx].frm_period_d * hndl->cfg.denom_mult) / (DOUBLE)(hndl->cfg.default_param.frm_period_cnt_ptr[idx].frm_period_n * hndl->cfg.num_mult);
                                
                util_opsa_find_rational_less_than_one(decimal_d_over_n, 0xFFFFFF, &frm_per_d, &frm_per_n);

            }
            else
            {
                frm_per_n = hndl->cfg.default_param.frm_period_cnt_ptr[idx].frm_period_n;
                frm_per_d = hndl->cfg.default_param.frm_period_cnt_ptr[idx].frm_period_d;  
            }

            mpmo_dsp_field_FP_ENABLE_set(NULL, hndl, idx, FALSE);
            mpmo_dsp_field_FRM_PERIOD_N_set(NULL, hndl, idx, frm_per_n);
            mpmo_dsp_field_FRM_PERIOD_D_set(NULL, hndl, idx, frm_per_d);
            mpmo_dsp_field_FP_ENABLE_set(NULL, hndl, idx, TRUE);
        }
    }

    PMC_RETURN(result);
} /* mpmo_cn_frame_period_assign */



/*******************************************************************************
*  mpmo_adjust_dci_clk
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This method may be use to program a specific frm_period_d and
*   frm_period_n (ie, not using the value provided in context).
*   
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
*   numer_mult     - the frame period n to force
*   denom_mult     - the frame period d to force
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
PUBLIC PMC_ERROR mpmo_adjust_dci_clk(mpmo_handle_t *hndl,
                                     UINT32 numer_mult,
                                     UINT32 denom_mult)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    hndl->cfg.num_mult = numer_mult;
    hndl->cfg.denom_mult = denom_mult;

    PMC_RETURN(result);
} /* mpmo_adjust_dci_clk */



/*******************************************************************************
*  mpmo_cn_frame_period_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configues the frame period if it is not already programmed in the DSP.
*
* INPUTS:
*   *hndl           - pointer to MPMO handle instance
*   chnl            - channel ID
*                     Valid range: 0 - 95
*   cn_frame_period - ENUM, see util_opsa_cn_frame_period_t
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
PUBLIC PMC_ERROR mpmo_cn_frame_period_cfg(mpmo_handle_t *hndl, UINT32 chnl,
                                          util_opsa_cn_frame_period_t cn_frame_period)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 idx = 0;
    UINT32 empty_cnt = 0;
    UINT32 frm_idx = MPMO_NUM_FRM_PERIOD_CNT;
    UINT32 frm_per_n, frm_per_d;
    BOOL frm_present = FALSE;
    DOUBLE decimal_d_over_n;
    UINT32 frm_per_n_adjusted;
    UINT32 frm_per_d_adjusted;
    
    mpmo_dsp_buffer_t mpmo_buf[1];

    PMC_ENTRY();
    
    /* be sure that the CN generator is enabled */
    mpmo_dsp_field_CN_GENERATOR_ENABLE_set(NULL, hndl, chnl, 1);

    if (hndl->cfg.default_param.mpmo_frm_period_flex_alloc_method == TRUE)
    {
        for (idx = 0; idx < MPMO_NUM_FRM_PERIOD_CNT; idx++)
        {
            /* first check if the frame period has been already programmed in DSP */
            if (hndl->cfg.default_param.frm_period_cnt[idx].frm_period_type == cn_frame_period)
            {
                frm_present = TRUE;
                frm_idx = idx;
                break;
            }
            else if (hndl->cfg.default_param.frm_period_cnt[idx].frm_period_type == UTIL_OPSA_T_FRAME_UNUSED
                     && empty_cnt == 0) 
            { 
                /* store the first available empty slot in case frame period is not present */
                frm_idx = idx;
                empty_cnt++;
            }                     
        }  /* end of for loop */ 
        
        /* after the full look-up, frame period is not found and the counter is full */
        if (frm_present == FALSE && empty_cnt == 0)
        {
            PMC_RETURN(MPMO_LOG_ERR_FRM_PERIOD_CNT_FULL);
        }
        
        if (frm_present == FALSE)
        {
            /* found an empty index */
            hndl->cfg.default_param.frm_period_cnt[frm_idx].frm_period_type = cn_frame_period;

            /* get frame period_n and period_d */
            util_opsa_cn_frame_period_get(cn_frame_period, &frm_per_n, &frm_per_d);
            
            /* update the global DSP param */
            hndl->cfg.default_param.frm_period_cnt[frm_idx].frm_period_n = frm_per_n;
            hndl->cfg.default_param.frm_period_cnt[frm_idx].frm_period_d = frm_per_d;

             
            if(hndl->cfg.denom_mult != 1 || hndl->cfg.num_mult != 1)
            {
                /* If there is a multiplier applied on the DCI clock, it's require to rationalize
                the cgen_n and cgen_d*/
                if (!digi_use_float32_get())
                {
                    decimal_d_over_n =  (DOUBLE)(frm_per_d*hndl->cfg.denom_mult)/(DOUBLE)(frm_per_n* hndl->cfg.num_mult);
                }
                else
                {
                    decimal_d_over_n =  (DOUBLE)((FLOAT)(frm_per_d*hndl->cfg.denom_mult)/(FLOAT)(frm_per_n* hndl->cfg.num_mult));
                }
                
                /*frame period resolution is 24 bits*/
                util_opsa_find_rational_less_than_one(decimal_d_over_n, 0xFFFFFF, &frm_per_d_adjusted, &frm_per_n_adjusted);

                mpmo_dsp_buffer_init(mpmo_buf, hndl);     
                mpmo_dsp_field_FP_ENABLE_set(mpmo_buf, hndl, frm_idx, FALSE);
                mpmo_dsp_field_FRM_PERIOD_D_set(mpmo_buf, hndl, frm_idx, frm_per_d_adjusted);
                mpmo_dsp_buffer_flush(mpmo_buf);
                
                mpmo_dsp_field_FRM_PERIOD_N_set(mpmo_buf, hndl, frm_idx, frm_per_n_adjusted);
                mpmo_dsp_buffer_flush(mpmo_buf);
                
                mpmo_dsp_field_FRM_PERIOD_D_set(mpmo_buf, hndl, frm_idx, frm_per_d_adjusted);
                mpmo_dsp_field_FP_ENABLE_set(mpmo_buf, hndl, frm_idx, TRUE);
                mpmo_dsp_buffer_flush(mpmo_buf);
            }
            else
            {
                mpmo_dsp_buffer_init(mpmo_buf, hndl);     
                mpmo_dsp_field_FP_ENABLE_set(mpmo_buf, hndl, frm_idx, FALSE);
                mpmo_dsp_field_FRM_PERIOD_D_set(mpmo_buf, hndl, frm_idx, frm_per_d);
                mpmo_dsp_buffer_flush(mpmo_buf);
                
                mpmo_dsp_field_FRM_PERIOD_N_set(mpmo_buf, hndl, frm_idx, frm_per_n);
                mpmo_dsp_buffer_flush(mpmo_buf);
                
                mpmo_dsp_field_FRM_PERIOD_D_set(mpmo_buf, hndl, frm_idx, frm_per_d);
                mpmo_dsp_field_FP_ENABLE_set(mpmo_buf, hndl, frm_idx, TRUE);
                mpmo_dsp_buffer_flush(mpmo_buf);
            }

        }
 
        /* update var_t for channel context */
        hndl->var.ch_cfg_ptr[chnl].frm_period_idx = frm_idx;
        hndl->var.ch_cfg_ptr[chnl].frm_period_n = hndl->cfg.default_param.frm_period_cnt[frm_idx].frm_period_n;
        hndl->var.ch_cfg_ptr[chnl].frm_period_d = hndl->cfg.default_param.frm_period_cnt[frm_idx].frm_period_d;  
    }
    else {
         
        result = mpmo_cn_frame_period_get(hndl, cn_frame_period, &idx,
                                          &frm_per_n, &frm_per_d);
                                          
        /* update var_t for channel context */
        hndl->var.ch_cfg_ptr[chnl].frm_period_idx = idx;
        hndl->var.ch_cfg_ptr[chnl].frm_period_n = frm_per_n;
        hndl->var.ch_cfg_ptr[chnl].frm_period_d = frm_per_d;
    }

    PMC_RETURN(result);
} /* mpmo_cn_frame_period_cfg */

/*******************************************************************************
*  mpmo_ch_ssf_cn_center_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get the Cn Center for the channel.
*
* INPUTS:
*   *hndl    - pointer to MPMO handle instance
*   chnl     - channel to initialize
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOl     - TRUE: the SSF Cn Center is enabled \n
*              FALSE: the SSF Cn Center is disabled
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL mpmo_ch_ssf_cn_center_get(mpmo_handle_t *hndl, UINT32 chnl)
{
    BOOL result;
    PMC_ENTRY();

    /* enable/disable SSF CN CENTER */
    result = mpmo_field_SSF_CN_CENTER_EN_get(NULL, hndl, chnl);

    PMC_RETURN(result);

} /* mpmo_ch_ssf_cn_center_get */

/*******************************************************************************
*  mpmo_ch_zone_cent_gain_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API calculates zone centering gain based on the ppm_offset for the 
*   speed of the zone centering.
*
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
*   chnl           - channel to initialize
*   ppm_offset     - desired speed of centering, specified as a ppm offset from nominal
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
PUBLIC PMC_ERROR mpmo_ch_zone_cent_gain_cfg(mpmo_handle_t *hndl, UINT32 chnl,
                                            UINT32 ppm_offset)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 mult;
    UINT32 cn_golden_int;
    UINT32 cn_golden_n;
    UINT32 cn_golden_d;
    DOUBLE cn_nominal;
    DOUBLE zone_cent_gain;
    FLOAT f_cn_nominal;
    FLOAT f_zone_cent_gain;
    UINT32 lpf_gain;
   
    DOUBLE client_bitrate = hndl->var.ch_cfg_ptr[chnl].client_bitrate;

    UINT32 *frm_period_n_ptr = &(hndl->var.ch_cfg_ptr[chnl].frm_period_n);
    UINT32 *frm_period_d_ptr = &(hndl->var.ch_cfg_ptr[chnl].frm_period_d);

    PMC_ENTRY();

    
    if (hndl->cfg.default_param.cn_bit_mode == 1)
    {
        mult = 8;
    }
    else {
        mult = 1;
    }
    
    /* calculate golden Cn except when golden_cn_cal_en = 1 */
    if (hndl->var.ch_cfg_ptr[chnl].golden_cn_cal_en == FALSE)
    {
        result = util_global_golden_cn_calc(*frm_period_n_ptr, *frm_period_d_ptr, 
                                            client_bitrate, &cn_golden_int, 
                                            &cn_golden_n, &cn_golden_d);
    }
    else {
        /* cn_base_* is equivalent to cn_golden */
        cn_golden_int = hndl->var.ch_cfg_ptr[chnl].cn_base_int;
        cn_golden_n = hndl->var.ch_cfg_ptr[chnl].cn_base_n;
        cn_golden_d = hndl->var.ch_cfg_ptr[chnl].cn_base_d;
    }
    
    if (!digi_use_float32_get())
    {
        /* convert cn_nominal into double */
        cn_nominal = cn_golden_int + ((DOUBLE)cn_golden_n/cn_golden_d);
        cn_nominal = cn_nominal * mult;
        
        /* retreive the configured lpf_gain */
        lpf_gain = hndl->var.ch_cfg_ptr[chnl].lpf_gain;

        zone_cent_gain = PMC_FLOOR((PMC_MATH_LOG((cn_nominal * (DOUBLE)ppm_offset) / 1000000.0) / PMC_MATH_LOG(2)) + lpf_gain);
        
        if (zone_cent_gain - lpf_gain > 5)
        {
            zone_cent_gain = lpf_gain + 5;
        }
        else if (zone_cent_gain < 0)
        {
            /* do not allow programming a negative value */
            zone_cent_gain = 0;
        }
           
        /* configure zone_cent_gain */
        mpmo_dsp_field_ZONE_CENT_GAIN_set(NULL, hndl, chnl, zone_cent_gain);
    }
    else
    {
        /* convert cn_nominal into double */
        f_cn_nominal = cn_golden_int + ((FLOAT)cn_golden_n/cn_golden_d);
        f_cn_nominal = f_cn_nominal * mult;
        
        /* retreive the configured lpf_gain */
        lpf_gain = hndl->var.ch_cfg_ptr[chnl].lpf_gain;

        f_zone_cent_gain = PMC_FLOORF((PMC_MATH_LOGF((f_cn_nominal * (FLOAT)ppm_offset) / 1000000.0F) / PMC_MATH_LOGF(2)) + lpf_gain);
        
        if (f_zone_cent_gain - lpf_gain > 5)
        {
            f_zone_cent_gain = lpf_gain + 5;
        }
        else if (f_zone_cent_gain < 0)
        {
            /* do not allow programming a negative value */
            f_zone_cent_gain = 0;
        }
           
        /* configure zone_cent_gain */
        mpmo_dsp_field_ZONE_CENT_GAIN_set(NULL, hndl, chnl, (UINT32)f_zone_cent_gain);
    }   

    PMC_RETURN(result);

} /* mpmo_ch_zone_cent_gain_cfg */


/*******************************************************************************
* mpmo_int_chnl_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *mpmo_handle       - pointer to MPMO handle instance
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
PUBLIC PMC_ERROR mpmo_int_chnl_enable(mpmo_handle_t *mpmo_handle,
                                      UINT32 chnl_id,
                                      mpmo_int_chnl_t *int_table_ptr,
                                      mpmo_int_chnl_t *int_en_table_ptr,
                                      BOOL8 enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = chnl_id * 2;
    UINT32          link2 = chnl_id * 2;
    
    PMC_ENTRY();
    
    result = mpmo_int_validate(mpmo_handle, chnl_id, int_en_table_ptr);
    
#ifdef MPMO_INT_RANGE
#undef MPMO_INT_RANGE
#endif
#define MPMO_INT_RANGE    UTIL_GLOBAL_INT_ENABLE_RANGE
        
    MPMO_INT_TABLE_PKT_DROP_0_DEFINE();

    link1 += 1;
    link2 += 1;
    MPMO_INT_TABLE_PKT_DROP_1_DEFINE();
    
    /* CFC */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_enable(mpmo_handle->cfc_handle,
                                     chnl_id,
                                     &int_table_ptr->cfc,
                                     &int_en_table_ptr->cfc,
                                     enable);
                                     
    }

    PMC_RETURN(result);
} /* mpmo_int_chnl_enable */

/*******************************************************************************
* mpmo_int_chnl_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *mpmo_handle      - pointer to MPMO handle instance
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
PUBLIC PMC_ERROR mpmo_int_chnl_clear(mpmo_handle_t  *mpmo_handle,
                                     UINT32 chnl_id,
                                     mpmo_int_chnl_t *int_table_ptr,
                                     mpmo_int_chnl_t *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = chnl_id;
    UINT32          link2 = chnl_id;
    
    PMC_ENTRY();
    
    result = mpmo_int_validate(mpmo_handle, chnl_id, mpmo_handle);
    
#ifdef MPMO_INT_RANGE
#undef MPMO_INT_RANGE
#endif
#define MPMO_INT_RANGE UTIL_GLOBAL_INT_CLEAR_RANGE
            
    MPMO_INT_TABLE_PKT_DROP_0_DEFINE();
    link1 += 1;
    link2 += 1;
    MPMO_INT_TABLE_PKT_DROP_1_DEFINE();
    
    /* CFC */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_clear(mpmo_handle->cfc_handle,
                                    chnl_id,
                                    int_en_table_ptr!= NULL?&int_en_table_ptr->cfc:NULL,
                                    int_table_ptr!=NULL?&int_table_ptr->cfc:NULL);
                                     
    }
    
    PMC_RETURN(result);
} /* mpmo_int_chnl_clear */

/*******************************************************************************
* mpmo_int_chnl_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   *mpmo_handle      - pointer to MPMO handle instance
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
PUBLIC PMC_ERROR mpmo_int_chnl_retrieve(mpmo_handle_t *mpmo_handle,
                                        UINT32 chnl_id,
                                        mpmo_int_chnl_t *filt_table_ptr,
                                        mpmo_int_chnl_t *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = chnl_id;
    UINT32          link2 = chnl_id;

    PMC_ENTRY();

    result = mpmo_int_validate(mpmo_handle, chnl_id, int_table_ptr);

    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(mpmo_handle, mpmo, UTIL_GLOBAL_INT_OPTIMIZE);
    
#ifdef MPMO_INT_RANGE
#undef MPMO_INT_RANGE
#endif
#define MPMO_INT_RANGE UTIL_GLOBAL_INT_RETRIEVE_RANGE
    
    MPMO_INT_TABLE_PKT_DROP_0_DEFINE();
    link1 += 1;
    link2 += 1;
    MPMO_INT_TABLE_PKT_DROP_1_DEFINE();
    
    UTIL_GLOBAL_INT_BUFFER_FLUSH(mpmo_handle, mpmo);

     /* CFC */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_retrieve(mpmo_handle->cfc_handle,
                                       chnl_id,
                                       UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, cfc),
                                       &int_table_ptr->cfc);
                                     
    }


    PMC_RETURN(result);
} /* mpmo_int_chnl_retrieve */

/*******************************************************************************
* mpmo_int_chnl_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   *mpmo_handle      - pointer to MPMO handle instance
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
PUBLIC PMC_ERROR mpmo_int_chnl_enabled_check(mpmo_handle_t *mpmo_handle,
                                             UINT32 chnl_id,
                                             mpmo_int_chnl_t *int_en_table_ptr,
                                             BOOL8 *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = chnl_id;
    UINT32          link2 = chnl_id;
    
    PMC_ENTRY();
    
    result = mpmo_int_validate(mpmo_handle, chnl_id, int_found_ptr);
    
#ifdef MPMO_INT_RANGE
#undef MPMO_INT_RANGE
#endif
#define MPMO_INT_RANGE UTIL_GLOBAL_INT_CHECK_RANGE
    

    MPMO_INT_TABLE_PKT_DROP_0_DEFINE();
    link1 += 1;
    link2 += 1;
    MPMO_INT_TABLE_PKT_DROP_1_DEFINE();
    
     /* CFC */
    if (PMC_SUCCESS == result && 
        *int_found_ptr == FALSE)
    {
        result = cfc_int_chnl_enabled_check(mpmo_handle->cfc_handle,
                                            chnl_id,
                                            &int_en_table_ptr->cfc,
                                            int_found_ptr);
                                     
    }

    PMC_RETURN(result);
} /* mpmo_int_chnl_enabled_check */

/*******************************************************************************
*  mpmo_ch_cn_frame_period_default_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Removes the cn frame period from the DSP if no channel is using the frame
*   period in the DSP.
*
* INPUTS:
*   *hndl           - pointer to MPMO handle instance
*   chnl            - channel ID
*                     Valid range: 0 - 95
*   cn_frame_period - ENUM, see util_opsa_cn_frame_period_t
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
PRIVATE PMC_ERROR mpmo_ch_cn_frame_period_default_cfg(mpmo_handle_t *hndl, UINT32 chnl,
                                                      util_opsa_cn_frame_period_t cn_frame_period)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 idx;

    PMC_ENTRY();

    for (idx = 0; idx < MPMO_NUM_FRM_PERIOD_CNT; idx++)
    {
        /* first check if the frame period exists in DSP */
        if (hndl->cfg.default_param.frm_period_cnt[idx].frm_period_type == cn_frame_period)
        {
            /* remove from the DSP */
            hndl->cfg.default_param.frm_period_cnt[idx].frm_period_type = UTIL_OPSA_T_FRAME_UNUSED;
            hndl->cfg.default_param.frm_period_cnt[idx].frm_period_n = 0;
            hndl->cfg.default_param.frm_period_cnt[idx].frm_period_d = 1;

            /* break when the frame period is found and initialized */
            break;
        }
    }

    if (idx == MPMO_NUM_FRM_PERIOD_CNT)
    {
        PMC_RETURN(MPMO_LOG_ERR_FRM_PERIOD_NOT_FOUND);
    }

    PMC_RETURN(result);
} /* mpmo_ch_cn_frame_period_default_cfg */

/*******************************************************************************
*  mpmo_ckctl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure clock control ports.
*
* INPUTS:
*   *hndl       - pointer to MPMO handle instance
*   ckctl_port  - clock control port number (0-25): \n
*                 Ports 0--11 are line side 10G SERDES. \n
*                 Ports 12--13 are line side 2.5G SERDES. \n
*                 Ports 14--25 are system side 10G SERDES. \n
*   chnl        - channel to map to the port specified by ckctl_port.
*   enable      - enables/disables CKCTL output
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
PUBLIC PMC_ERROR mpmo_ckctl_cfg(mpmo_handle_t *hndl, UINT32 ckctl_port, UINT32 chnl,
                                BOOL enable)
{
    PMC_ENTRY();

    if (ckctl_port >= MPMO_MAX_CKCTL_PORT) {
        PMC_RETURN(MPMO_LOG_ERR_INVALID_PARAMETERS);
    }

    /* Configure channel to output */
    mpmo_field_CKCTL_PORT_ID_set(NULL, hndl, ckctl_port, chnl);
#if  defined(PMC_SW_SIMULATION)
    /* Enable/disable output */
    mpmo_field_CKCTL_EN_set(NULL, hndl, ckctl_port, enable);
#endif /* defined(PMC_EMULATION)|| defined(PMC_VERIFICATION) || defined(PMC_SW_SIMULATION) */

    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_ckctl_cfg */

/*******************************************************************************
*  mpmo_ckctl_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures clock control ports CKCTL_EN field.  This function is provided
*   for the special case of CBRC with mode FC1200 TTT.  In this mode, the
*   MPMO must transfer FIFO depth data to ACB and the MPMO is not enabled as
*   part of the data path.
*
* INPUTS:
*   *hndl       - pointer to MPMO handle instance
*   ckctl_port  - clock control port number (0-25): \n
*                 Ports 0--11 are line side 10G SERDES. \n
*                 Ports 12--13 are line side 2.5G SERDES. \n
*                 Ports 14--25 are system side 10G SERDES. \n
*   enable      - enables/disables CKCTL output
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
PUBLIC PMC_ERROR mpmo_ckctl_en_set(mpmo_handle_t *hndl, UINT32 ckctl_port, BOOL enable)
{
    PMC_ENTRY();

    if (ckctl_port >= MPMO_MAX_CKCTL_PORT) {
        PMC_RETURN(MPMO_LOG_ERR_INVALID_PARAMETERS);
    }

    /* Enable/disable output */
    mpmo_field_CKCTL_EN_set(NULL, hndl, ckctl_port, enable);

    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_ckctl_en_set */


/*******************************************************************************
* mpmo_enable_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/disables the packet maker functionality.
*
* INPUTS:
*   *hndl    - pointer to MPMO handle instance
*   enable   - TRUE: enable MPMO. FALSE: disable MPMO.
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
PUBLIC PMC_ERROR mpmo_enable_cfg(mpmo_handle_t *hndl, BOOL enable)
{
    PMC_ENTRY();

    /* Enable/disable MPMO */
    mpmo_field_MPMO_EN_set(NULL, hndl, enable);

    PMC_RETURN(PMC_SUCCESS);
} /* mpmo_enable_cfg */

/*******************************************************************************
* mpmo_enable_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get whether the packet maker functionality is enabled or disabled.
*
* INPUTS:
*   *hndl    - pointer to MPMO handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8    - TRUE: enable MPMO \n
*              FALSE: disable MPMO
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 mpmo_enable_get(mpmo_handle_t *hndl)
{
    BOOL8 enable;
    PMC_ENTRY();
    
    enable = mpmo_field_MPMO_EN_get(NULL, hndl);
    
    PMC_RETURN(enable);
} /* mpmo_enable_get */


/*******************************************************************************
*  mpmo_csi_normal_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure CSI_NORMAL value and CSI mask enable/disable.
*
* INPUTS:
*   *hndl       - pointer to MPMO handle instance
*   csi_normal  - value to use
*   csi_mask_en - enables/disables masking of the lower two bits of the ingress
*                 5 bit CSI value: \n
*                 TRUE: Bottom two bits are squelched when comparing
*                       the CSI value. \n
*                 FALSE: All 5-bits of the CSI value is used for comparison. \n
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
*     MPMO_CSI_NORMAL     0x0C \n
*
*******************************************************************************/
PUBLIC PMC_ERROR mpmo_csi_normal_cfg(mpmo_handle_t *hndl, UINT32 csi_normal,
                                     BOOL csi_mask_en)
{
    PMC_ENTRY();

    /* Configure specified value */
    mpmo_field_CSI_NORMAL_set(NULL, hndl, csi_normal);

    /* Enable/disable mask */
    mpmo_field_CSI_MASK_EN_set(NULL, hndl, csi_mask_en);

    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_csi_normal_cfg */


/*******************************************************************************
*  mpmo_ch_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provisions a channel for the specified rate.
*   This function determines the parameters to configure for the MPMO datapath
*   and DSP. It also evaluates the scheduler to see if the specified rate can
*   be allocated.
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   chnl              - channel to provision
*   client_bitrate    - client bit rate (in Hz)
*   pkt_size          - packet size configured at the packet maker.
*   pkt_per_int       - integer portion for packet period
*                       configured at the packet maker.
*   pkt_per_n         - numerator for packet period fraction
*                       configured at the packet maker.
*   pkt_per_d         - denominator for packet period fraction
*                       configured at the packet maker.
*   cn_frame_period   - frame period for generating Cn value.
*                       When MPMO instantiation is MPMO_COREOTN_MUX_FRAMER or
*                       MPMO_MAPOTN with non BMP mapped clients, set to an
*                       appropriate value. Set to UTIL_OPSA_T_FRAME_UNUSED
*                       for all other MPMO instantiations.
*   golden_cn_en      - enable golden cn calculation (this will be enabled if
*                       data is coming from ENET)
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
PUBLIC PMC_ERROR mpmo_ch_prov(mpmo_handle_t *hndl, UINT32 chnl,
                              DOUBLE client_bitrate,
                              UINT32 pkt_size, UINT32 pkt_per_int,
                              UINT32 pkt_per_n, UINT32 pkt_per_d,
                              util_opsa_cn_frame_period_t cn_frame_period,
                              BOOL golden_cn_en)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    ret_val = mpmo_internal_ch_prov(hndl, chnl,
                                    client_bitrate,
                                    pkt_size, pkt_per_int,
                                    pkt_per_n, pkt_per_d,
                                    cn_frame_period,
                                    golden_cn_en,
                                    FALSE,
                                    TRUE,
                                    FALSE,
                                    FALSE,
                                    UTIL_GLOBAL_LAST_ODUK);

    PMC_RETURN(ret_val);

} /* mpmo_ch_prov */

/*******************************************************************************
*  mpmo_ch_super_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provisions a channel for the specified rate.
*   This function determines the parameters to configure for the MPMO datapath
*   and DSP. It also evaluates the scheduler to see if the specified rate can
*   be allocated.
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   chnl              - channel to provision
*   client_bitrate    - client bit rate (in Hz)
*   pkt_size          - packet size configured at the packet maker.
*   pkt_per_int       - integer portion for packet period
*                       configured at the packet maker.
*   pkt_per_n         - numerator for packet period fraction
*                       configured at the packet maker.
*   pkt_per_d         - denominator for packet period fraction
*                       configured at the packet maker.
*   cn_frame_period   - frame period for generating Cn value.
*                       When MPMO instantiation is MPMO_COREOTN_MUX_FRAMER or
*                       MPMO_MAPOTN with non BMP mapped clients, set to an
*                       appropriate value. Set to UTIL_OPSA_T_FRAME_UNUSED
*                       for all other MPMO instantiations.
*   golden_cn_en      - enable golden cn calculation (this will be enabled if
*                       data is coming from ENET)
*   odu_type          - Required for COREOTN and SIFD MPMOs
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
PUBLIC PMC_ERROR mpmo_ch_super_prov(mpmo_handle_t *hndl, UINT32 chnl,
                                    DOUBLE client_bitrate,
                                    UINT32 pkt_size, UINT32 pkt_per_int,
                                    UINT32 pkt_per_n, UINT32 pkt_per_d,
                                    util_opsa_cn_frame_period_t cn_frame_period,
                                    BOOL golden_cn_en,
                                    util_global_odukp_type_t odu_type)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    ret_val = mpmo_internal_ch_prov(hndl, chnl,
                                    client_bitrate,
                                    pkt_size, pkt_per_int,
                                    pkt_per_n, pkt_per_d,
                                    cn_frame_period,
                                    golden_cn_en,
                                    FALSE,
                                    TRUE,
                                    TRUE,
                                    FALSE,
                                    odu_type);

    PMC_RETURN(ret_val);

} /* mpmo_ch_super_prov */

/*******************************************************************************
*  mpmo_ch_prov_for_maint_sig_insertion
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provisions a channel for the specified rate.
*   This function determines the parameters to configure for the MPMO datapath
*   and DSP. It also evaluates the scheduler to see if the specified rate can
*   be allocated.
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   chnl              - channel to provision
*   client_bitrate    - client bit rate (in Hz)
*   pkt_size          - packet size configured at the packet maker.
*   pkt_per_int       - integer portion for packet period
*                       configured at the packet maker.
*   pkt_per_n         - numerator for packet period fraction
*                       configured at the packet maker.
*   pkt_per_d         - denominator for packet period fraction
*                       configured at the packet maker.
*   cn_frame_period   - frame period for generating Cn value.
*                       When MPMO instantiation is MPMO_COREOTN_MUX_FRAMER or
*                       MPMO_MAPOTN with non BMP mapped clients, set to an
*                       appropriate value. Set to UTIL_OPSA_T_FRAME_UNUSED
*                       for all other MPMO instantiations.
*   golden_cn_en      - enable golden cn calculation (this will be enabled if
*                       data is coming from ENET)
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
PUBLIC PMC_ERROR mpmo_ch_prov_for_maint_sig_insertion(mpmo_handle_t *hndl, UINT32 chnl,
                                                      DOUBLE client_bitrate,
                                                      UINT32 pkt_size, UINT32 pkt_per_int,
                                                      UINT32 pkt_per_n, UINT32 pkt_per_d,
                                                      util_opsa_cn_frame_period_t cn_frame_period,
                                                      BOOL golden_cn_en)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    ret_val = mpmo_internal_ch_prov(hndl, chnl,
                                    client_bitrate,
                                    pkt_size, pkt_per_int,
                                    pkt_per_n, pkt_per_d,
                                    cn_frame_period,
                                    golden_cn_en,
                                    FALSE,
                                    FALSE,
                                    TRUE,
                                    FALSE,
                                    UTIL_GLOBAL_LAST_ODUK);

    PMC_RETURN(ret_val);

} /* mpmo_ch_prov_for_maint_sig_insertion */

/*******************************************************************************
*  mpmo_mapotn_ch_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provisions a channel for the specified rate.
*   This function determines the parameters to configure for the MPMO datapath
*   and DSP. It also evaluates the scheduler to see if the specified rate can
*   be allocated.
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   chnl              - channel to provision
*   client_bitrate    - client bit rate (in Hz)
*   pkt_size          - packet size configured at the packet maker.
*   pkt_per_int       - integer portion for packet period
*                       configured at the packet maker.
*   pkt_per_n         - numerator for packet period fraction
*                       configured at the packet maker.
*   pkt_per_d         - denominator for packet period fraction
*                       configured at the packet maker.
*   cn_frame_period   - frame period for generating Cn value.
*                       When MPMO instantiation is MPMO_COREOTN_MUX_FRAMER or
*                       MPMO_MAPOTN with non BMP mapped clients, set to an
*                       appropriate value. Set to UTIL_OPSA_T_FRAME_UNUSED
*                       for all other MPMO instantiations.
*   golden_cn_en      - enable golden cn calculation (this will be enabled if
*                       data is coming from ENET)
*   bmp_map_mode      - indication if the MPMO is accociated with MAPOTN mapper 
*                       MPMO used for BMP mapping \n
*                       TRUE:  MAPOTN mapper MPMO used with BMP mapping \n
*                       FALSE: MAPOTN mapper MPMO not used with BMP mapping
*   is_sonet          - TRUE : traffic is SONET
*                       FALSE : traffic is not SONET
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
PUBLIC PMC_ERROR mpmo_mapotn_ch_prov(mpmo_handle_t *hndl, UINT32 chnl,
                                     DOUBLE client_bitrate,
                                     UINT32 pkt_size, UINT32 pkt_per_int,
                                     UINT32 pkt_per_n, UINT32 pkt_per_d,
                                     util_opsa_cn_frame_period_t cn_frame_period,
                                     BOOL golden_cn_en,
                                     BOOL8 bmp_map_mode,
                                     BOOL8 is_sonet)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    ret_val = mpmo_internal_ch_prov(hndl, chnl,
                                    client_bitrate,
                                    pkt_size, pkt_per_int,
                                    pkt_per_n, pkt_per_d,
                                    cn_frame_period,
                                    golden_cn_en,
                                    bmp_map_mode,
                                    TRUE,
                                    FALSE,
                                    is_sonet,
                                    UTIL_GLOBAL_LAST_ODUK);

    PMC_RETURN(ret_val);

} /* mpmo_mapotn_ch_prov */

/*******************************************************************************
*  mpmo_internal_ch_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provisions a channel for the specified rate.
*   This function determines the parameters to configure for the MPMO datapath
*   and DSP. It also evaluates the scheduler to see if the specified rate can
*   be allocated.
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   chnl              - channel to provision
*   client_bitrate    - client bit rate (in Hz)
*   pkt_size          - packet size configured at the packet maker.
*   pkt_per_int       - integer portion for packet period
*                       configured at the packet maker.
*   pkt_per_n         - numerator for packet period fraction
*                       configured at the packet maker.
*   pkt_per_d         - denominator for packet period fraction
*                       configured at the packet maker.
*   cn_frame_period   - frame period for generating Cn value.
*                       When MPMO instantiation is MPMO_COREOTN_MUX_FRAMER or
*                       MPMO_MAPOTN with non BMP mapped clients, set to an
*                       appropriate value. Set to UTIL_OPSA_T_FRAME_UNUSED
*                       for all other MPMO instantiations.
*   golden_cn_en      - enable golden cn calculation (this will be enabled if
*                       data is coming from ENET)
*   bmp_map_mode      - indication if the MPMO is accociated with MAPOTN mapper 
*                       MPMO used for BMP mapping \n
*                       TRUE:  MAPOTN mapper MPMO used with BMP mapping \n
*                       FALSE: MAPOTN mapper MPMO not used with BMP mapping
*   true_prov         - boolean value to determine if this is being used for
*                       special OCI insertion in SYSOTN application
*   super_prov        - boolean value to determine if this is being used for super_prov
*   is_sonet          - TRUE : traffic for mapotn MPMO is sonet
*                       FALSE : traffic for mapotn MPMO is not sonet
*   odu_type          - required for SIFD and COREOTN MPMOs
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
PRIVATE PMC_ERROR mpmo_internal_ch_prov(mpmo_handle_t *hndl, UINT32 chnl,
                                        DOUBLE client_bitrate,
                                        UINT32 pkt_size, UINT32 pkt_per_int,
                                        UINT32 pkt_per_n, UINT32 pkt_per_d,
                                        util_opsa_cn_frame_period_t cn_frame_period,
                                        BOOL golden_cn_en,
                                        BOOL8 bmp_map_mode,
                                        BOOL8 true_prov,
                                        BOOL8 super_prov,
                                        BOOL8 is_sonet,
                                        util_global_odukp_type_t odu_type)
{
    UINT32 *pp_int_ptr   = &(hndl->var.ch_cfg_ptr[chnl].pkt_period_int);
    UINT32 *pp_n_ptr     = &(hndl->var.ch_cfg_ptr[chnl].pkt_period_n);
    UINT32 *pp_d_ptr     = &(hndl->var.ch_cfg_ptr[chnl].pkt_period_d);
    UINT32 *ps_ptr       = &(hndl->var.ch_cfg_ptr[chnl].pkt_size);

    UINT32 *cn_base_int_ptr  = &(hndl->var.ch_cfg_ptr[chnl].cn_base_int);
    UINT32 *cn_base_n_ptr    = &(hndl->var.ch_cfg_ptr[chnl].cn_base_n);
    UINT32 *cn_base_d_ptr    = &(hndl->var.ch_cfg_ptr[chnl].cn_base_d);

    UINT32 *frm_period_n_ptr = &(hndl->var.ch_cfg_ptr[chnl].frm_period_n);
    UINT32 *frm_period_d_ptr = &(hndl->var.ch_cfg_ptr[chnl].frm_period_d);


    UINT32 num_cal_entries = 0;
    UINT32 ppm_offset = 10;

    PMC_ERROR ret_val;
    UINT32 scbs3_rate;

    BOOL8 use_float32_mode = digi_use_float32_get();

    PMC_ENTRY();
    
    PMC_LOG_TRACE("pkt_size:%d, pkt_per_int:%d, pkt_per_n:%d, pkt_per_d:%d\n",pkt_size, pkt_per_int, pkt_per_n, pkt_per_d);
    
    if (true_prov) /* true prov occurs with ODUKSW chnl map */
    {
        if (((hndl->cfg.default_param.mpmo_type_id == MPMO_COREOTN_CTL) ||
            (hndl->cfg.default_param.mpmo_type_id == MPMO_COREOTN_MUX_FRAMER)))
        {
            if (super_prov)
            {
                if (hndl->var.ch_cfg_ptr[chnl].state != MPMO_CHNL_START)
                {
                    PMC_RETURN(MPMO_LOG_CODE_INVALID_CHANNEL_STATE);
                }
            }
            else {    
                if(hndl->var.ch_cfg_ptr[chnl].state != MPMO_CHNL_MAINT_SIG_OPERATIONAL)
                {
                    PMC_RETURN(MPMO_LOG_CODE_INVALID_CHANNEL_STATE);
                }
            
            
                /*set CH_EN and SLICE_ENABLE to 0 so new configurations can get
                programmed in the event of a changing packet size*/
                mpmo_ch_enable_cfg(hndl, chnl, FALSE);

                if (hndl->var.ch_cfg_ptr[chnl].cn_generate_en) {
                    mpmo_dsp_ch_enable_cfg(hndl, chnl, FALSE);
                }
            }
        }
        else {   
            if (hndl->var.ch_cfg_ptr[chnl].state != MPMO_CHNL_START) {
                PMC_RETURN(MPMO_LOG_CODE_INVALID_CHANNEL_STATE);
            }
        }
    }
    
    /* Check for valid packet size range. */
    if ((pkt_size < MPMO_PKT_SIZE_MIN) || (pkt_size > MPMO_PKT_SIZE_MAX)) {
        PMC_RETURN(MPMO_LOG_ERR_INVALID_PARAMETERS);
    }

    /* Record the channel information */
    hndl->var.ch_cfg_ptr[chnl].client_bitrate = client_bitrate;

    /* Record the specified packet size. */
    *ps_ptr = pkt_size;

    /* If MPMO instance is MPMO_COREOTN_MUX_FRAMER, update 'cn_generate_en' */
    if ((  MPMO_COREOTN_MUX_FRAMER == hndl->cfg.default_param.mpmo_type_id
        || MPMO_MAPOTN == hndl->cfg.default_param.mpmo_type_id
)
        && (UTIL_OPSA_T_FRAME_UNUSED != cn_frame_period))
    {
        hndl->var.ch_cfg_ptr[chnl].cn_generate_en = TRUE;
    }
    else
    {
        hndl->var.ch_cfg_ptr[chnl].cn_generate_en = FALSE;
    }

    /* Record the specified packet period. */
    *pp_int_ptr   = pkt_per_int;
    *pp_n_ptr     = pkt_per_n;
    *pp_d_ptr     = pkt_per_d;

    /* Calculate DSP parameters if the DSP is instantiated and Cn generation is enabled. */
    if ((hndl->cfg.default_param.dsp_present)
        && (hndl->var.ch_cfg_ptr[chnl].cn_generate_en))
    {
        /* Record the specified packet period. */
        if (0 == pkt_per_d) {
            PMC_RETURN(MPMO_LOG_ERR_INVALID_PARAMETERS);
        } 

        /* Configure the frame period fractions */
        ret_val = mpmo_cn_frame_period_cfg(hndl, chnl, cn_frame_period);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }

        PMC_LOG_TRACE("*frm_period_n_ptr = 0x%x, *frm_period_d_ptr = 0x%x\n", *frm_period_n_ptr, *frm_period_d_ptr);

        /* if ENET intf then generate golden Cn */
        if (golden_cn_en == 1)
        {
            hndl->var.ch_cfg_ptr[chnl].golden_cn_cal_en = TRUE;

            /* Calculate the Golden Cn,base. */
            ret_val= util_global_golden_cn_calc(*frm_period_n_ptr, *frm_period_d_ptr, client_bitrate, cn_base_int_ptr, cn_base_n_ptr, cn_base_d_ptr);
            if (ret_val != PMC_SUCCESS)
            {
                PMC_RETURN(ret_val);
            }

            /* enable SSF CN CENTER_EN */
            mpmo_ch_ssf_cn_center_cfg(hndl, chnl, TRUE);

        }
        else
        {

            /* Calculate the fractional representation of Cn,base. */
            util_opsa_mpmo_cn_base_calc(*frm_period_n_ptr, *frm_period_d_ptr,
                                        *pp_int_ptr, *pp_n_ptr, *pp_d_ptr, *ps_ptr,
                                        cn_base_int_ptr, cn_base_n_ptr, cn_base_d_ptr);
        }

        /* Calculate the LPF gain. */
        util_opsa_mpmo_lpf_gain_calc(*frm_period_n_ptr, *frm_period_d_ptr,
                                     &(hndl->var.ch_cfg_ptr[chnl].lpf_gain));
    }
    else
    {
        /* do not touch CN_GENERATOR_ENABLE for FO1 MPMO */
        if (MPMO_COREOTN_MUX_FRAMER != hndl->cfg.default_param.mpmo_type_id) 
        {
            /* Disable Cn generation as by default it is on and getting garbage cn in BMP case */
            mpmo_dsp_field_CN_GENERATOR_ENABLE_set(NULL, hndl, chnl, 0);
        }
                    
        if(MPMO_MAPOTN == hndl->cfg.default_param.mpmo_type_id)
        {
            mpmo_ch_ssf_cn_center_cfg(hndl, chnl, TRUE);
        }
    }

    if (hndl->var.ch_cfg_ptr[chnl].cn_generate_en == TRUE)
    {
        if((hndl->cfg.default_param.mpmo_type_id == MPMO_COREOTN_MUX_FRAMER ||
           hndl->cfg.default_param.mpmo_type_id == MPMO_COREOTN_TO_MAPOTN) &&
           ((odu_type == UTIL_GLOBAL_ODU1E) ||
           (odu_type == UTIL_GLOBAL_ODU2E) ||
           (odu_type == UTIL_GLOBAL_ODU1F) ||
           (odu_type == UTIL_GLOBAL_ODU2F) ||
           (odu_type == UTIL_GLOBAL_ODUFLEX_CBR) ||
           (hndl->cfg.default_param.mpmo_type_id == MPMO_MAPOTN && is_sonet == FALSE)))
        {
            ppm_offset = 60;
        }
        else
        {
            ppm_offset = 20;
        }

        ret_val = mpmo_ch_zone_cent_gain_cfg(hndl, chnl, ppm_offset);
        if (ret_val != PMC_SUCCESS) 
        {
            PMC_RETURN(ret_val);
        }
    }
 


    /*
     * Provision SCBS3 after packet period calculations have been done successfully.
     * Convert the rate to a number that the SCBS3 can handle.
     * The highest rate is 100Gbit/s. Divide by 100 to fit into a UINT32
     * and use 100 bit/s rate units.
    */
    if(hndl->var.ch_cfg_ptr[chnl].state == MPMO_CHNL_START)
    {
        if(hndl->cfg.scbs3_cfg.scbs3_present)
        {
            if (!use_float32_mode) {
                scbs3_rate = (UINT32)(client_bitrate / MPMO_SCBS3_RATE_CONV_FACTOR);
            } else {
                scbs3_rate = (UINT32)((FLOAT)client_bitrate / MPMO_SCBS3_RATE_CONV_FACTOR);
            }
            ret_val = scbs3_chnl_prov(hndl->scbs3_handle, chnl,
                                      scbs3_rate, UTIL_SCHD_100_BIT_SEC,
                                      MPMO_SCBS3_READ_EN_USER_BITS);
            if (ret_val != PMC_SUCCESS) {
                PMC_RETURN(ret_val);
            }
        }
    }

    /* Get the the number of pages for CFC */
    if ((hndl->cfg.default_param.cal_entries_alloc_method == MPMO_DB_CAL_ENTRIES_ALLOC_METHOD 
        && hndl->cfg.default_param.mpmo_type_id == MPMO_MAPOTN)
        || (hndl->cfg.default_param.cal_entries_alloc_method == MPMO_COREOTN_TO_MAPOTN_DB_CAL_ENTRIES_ALLOC_METHOD
            && hndl->cfg.default_param.mpmo_type_id == MPMO_COREOTN_TO_MAPOTN)) 
    {
        /* go throught the database and get the number of calendar entries for
           the channel */
            mapotn_db_key       key;     
            key.calendar_entry = DO_NOT_USE_KEY;
            key.channel_num = chnl;
            num_cal_entries=gen_db_query_count(&(hndl->base),hndl->cfg.default_param.db_id,hndl->cfg.default_param.db_handle, (void *)&key);

            if(num_cal_entries == 0)
            {
                PMC_RETURN(MPMO_LOG_ERR_DB_ENTRY_NOT_FOUNDED);
            }
       
        /* number of pages in CFC needs to match the number of calendar slots
         * for the client rate
         */
        hndl->var.ch_cfg_ptr[chnl].cfc_num_pages = num_cal_entries;
    }
    else if (hndl->cfg.default_param.cal_entries_alloc_method == MPMO_COREOTN_MUX_FRAMER_DB_CAL_ENTRIES_ALLOC_METHOD 
             && hndl->cfg.default_param.mpmo_type_id == MPMO_COREOTN_MUX_FRAMER)
    {
        line_core_otn_db_key two_stage_key;
        
        two_stage_key.calendar_entry = DO_NOT_USE_KEY;
        two_stage_key.ho_channel = DO_NOT_USE_KEY;
        two_stage_key.mo_channel = DO_NOT_USE_KEY;
        two_stage_key.lo_channel = chnl;
        two_stage_key.trib_slot = DO_NOT_USE_KEY;
        
        PMC_LOG_TRACE("1. Two stage muxing: UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER/UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN\n");
        PMC_LOG_TRACE("two_stage_key.lo_channel = %u\n", two_stage_key.lo_channel);
                   
        num_cal_entries=gen_db_query_count(&(hndl->base), hndl->cfg.default_param.db_id, hndl->cfg.default_param.db_handle, (void *)&two_stage_key);
        
        if (num_cal_entries == 0)
        {
            line_core_otn_db_key one_stage_key;
        
            one_stage_key.calendar_entry = DO_NOT_USE_KEY;
            one_stage_key.ho_channel = DO_NOT_USE_KEY;
            one_stage_key.mo_channel = chnl;
            one_stage_key.lo_channel = DO_NOT_USE_KEY;
            one_stage_key.trib_slot = DO_NOT_USE_KEY;
        
            PMC_LOG_TRACE("2. One stage muxing: UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER/UTIL_GEN_DB_ACCESSOR_MPMA\n");
            PMC_LOG_TRACE("one_stage_key.mo_channel = %u\n", one_stage_key.mo_channel);
        
            num_cal_entries=gen_db_query_count(&(hndl->base), hndl->cfg.default_param.db_id, hndl->cfg.default_param.db_handle, (void *)&one_stage_key);     
            
            if (num_cal_entries == 0)
            {
                line_core_otn_db_key no_stage_key;
                
                no_stage_key.calendar_entry = DO_NOT_USE_KEY;
                no_stage_key.ho_channel = chnl;
                no_stage_key.mo_channel = DO_NOT_USE_KEY;
                no_stage_key.lo_channel = DO_NOT_USE_KEY;
                no_stage_key.trib_slot = DO_NOT_USE_KEY;
                
                PMC_LOG_TRACE("2. No stage muxing: UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER/UTIL_GEN_DB_ACCESSOR_MPMA\n");
                PMC_LOG_TRACE("no_stage_key.ho_channel = %u", no_stage_key.ho_channel);
                
                num_cal_entries=gen_db_query_count(&(hndl->base), hndl->cfg.default_param.db_id, hndl->cfg.default_param.db_handle, (void *)&no_stage_key);  

            }

         }
        
        if(true_prov)
        {
            /* number of pages in CFC needs to match the number of calendar slots
             * for the client rate
             */
            hndl->var.ch_cfg_ptr[chnl].cfc_num_pages = num_cal_entries;
        }
        else
        {
            hndl->var.ch_cfg_ptr[chnl].cfc_num_pages = 0;
        }
    }
    else
    {
        /* for other variations */
        hndl->var.ch_cfg_ptr[chnl].cfc_num_pages = mpmo_cfc_num_pages_calc(hndl, client_bitrate);         
    }

    PMC_LOG_TRACE("num_pages = %u\n", hndl->var.ch_cfg_ptr[chnl].cfc_num_pages);

    /* Calculate XOFF_THRES (back-pressure) and DATA_AVAIL threshold associated with the channel. */

     hndl->var.ch_cfg_ptr[chnl].xoff_thres = mpmo_xoff_thres_calc(hndl, hndl->var.ch_cfg_ptr[chnl].cfc_num_pages, bmp_map_mode);


    /* for G.HAO, it's required that the AGE_INT_GAIN and AGE_PROP gain are programmed
      properly */
    if ((hndl->cfg.default_param.dsp_present) && 
        (hndl->cfg.default_param.mpmo_type_id == MPMO_COREOTN_MUX_FRAMER))
    {
        if (!super_prov)
        {
            mpmo_ghao_age_gain_set(hndl,
                                   chnl,
                                   num_cal_entries);
        }                                 
    }

    /* Update channel state */
    if (true_prov)
    {
        hndl->var.ch_cfg_ptr[chnl].state = MPMO_CHNL_EQUIPPED;
    }
    else if(true_prov == FALSE)
    {
        hndl->var.ch_cfg_ptr[chnl].state = MPMO_CHNL_MAINT_SIG_EQUIPPED;
    }

            
    PMC_LOG_TRACE("hndl->var.ch_cfg_ptr[%u].state = %u\n", chnl, hndl->var.ch_cfg_ptr[chnl].state);
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].pkt_size=%d \n", hndl->var.ch_cfg_ptr[chnl].pkt_size);
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].pkt_period_int=%d \n", hndl->var.ch_cfg_ptr[chnl].pkt_period_int);
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].pkt_period_n=%d \n", hndl->var.ch_cfg_ptr[chnl].pkt_period_n);
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].pkt_period_d=%d \n", hndl->var.ch_cfg_ptr[chnl].pkt_period_d);

    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_internal_ch_prov */

/*******************************************************************************
*  mpmo_ch_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovisions the specified channel.
*
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
*   chnl           - channel to deprovision                       
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
PUBLIC PMC_ERROR mpmo_ch_deprov(mpmo_handle_t *hndl, UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    
    ret_val = mpmo_ch_deprov_internal(hndl,
                                      chnl,
                                      TRUE,
                                      FALSE);

    PMC_RETURN(ret_val);

} /* mpmo_ch_deprov */

/*******************************************************************************
*  mpmo_ch_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovisions the specified channel.
*
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
*   chnl           - channel to deprovision                       
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
PUBLIC PMC_ERROR mpmo_ch_deprov_super_deprov(mpmo_handle_t *hndl, 
                                             UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    
    ret_val = mpmo_ch_deprov_internal(hndl,
                                      chnl,
                                      TRUE,
                                      TRUE);

    PMC_RETURN(ret_val);

} /* mpmo_ch_deprov */


/*******************************************************************************
*  mpmo_ch_deprov_for_maint_sig_insertion
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovisions the specified channel.
*
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
*   chnl           - channel to deprovision                       
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
PUBLIC PMC_ERROR mpmo_ch_deprov_for_maint_sig_insertion(mpmo_handle_t *hndl, UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    
    ret_val = mpmo_ch_deprov_internal(hndl,
                                      chnl,
                                      FALSE,
                                      FALSE);

    PMC_RETURN(ret_val);

} /* mpmo_ch_deprov_for_maint_sig_insertion */


/*******************************************************************************
*  mpmo_ch_deprov_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovisions the specified channel.
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   chnl              - channel to deprovision
*   true_deprov       - TRUE : Used to deprov all other MPMOs
*                              other than COREOTN CTL and FO1
*                              instances \n
*                       FASLE : the "dummy" deactivate (to
*                               support open ODUKSW connection).
*                               State must be ignored
*   super_deprov      - boolean to indicate if this function is used in
*                       super deprov mode. \n
*                       TRUE: super-deprov \n
*                       FALSE: normal deprov 
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
PRIVATE PMC_ERROR mpmo_ch_deprov_internal(mpmo_handle_t *hndl, 
                                          UINT32 chnl,
                                          BOOL8 true_deprov,
                                          BOOL8 super_deprov)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if(true_deprov)
    {
        if (hndl->var.ch_cfg_ptr[chnl].state != MPMO_CHNL_EQUIPPED) {
            PMC_RETURN(MPMO_LOG_CODE_INVALID_CHANNEL_STATE);
        }
    }
    else
    {
        if (hndl->var.ch_cfg_ptr[chnl].state != MPMO_CHNL_MAINT_SIG_EQUIPPED) {
            PMC_RETURN(MPMO_LOG_CODE_INVALID_CHANNEL_STATE);
        }
    }

    if(hndl->var.ch_cfg_ptr[chnl].golden_cn_cal_en == TRUE ||
       MPMO_MAPOTN == hndl->cfg.default_param.mpmo_type_id)      
    {
        /* disable SSF CN CENTER_EN */
        mpmo_ch_ssf_cn_center_cfg(hndl, chnl, FALSE);
        hndl->var.ch_cfg_ptr[chnl].golden_cn_cal_en = FALSE;
    }


    if((true_deprov &&
       (hndl->cfg.default_param.mpmo_type_id != MPMO_COREOTN_CTL) &&
       (hndl->cfg.default_param.mpmo_type_id != MPMO_COREOTN_MUX_FRAMER)) ||
       true_deprov == FALSE)
    {
        /* Deprovision SCBS3 */
        if (hndl->cfg.scbs3_cfg.scbs3_present) {
            ret_val = scbs3_chnl_deprov(hndl->scbs3_handle, chnl);
        }
    }



    if (PMC_SUCCESS == ret_val)
    {
        if(true_deprov &&
           (hndl->cfg.default_param.mpmo_type_id == MPMO_COREOTN_CTL ||
            hndl->cfg.default_param.mpmo_type_id == MPMO_COREOTN_MUX_FRAMER))
        {
            /* Update channel state */
            if (super_deprov == FALSE)
            {
                hndl->var.ch_cfg_ptr[chnl].state = MPMO_CHNL_MAINT_SIG_OPERATIONAL;
            }
            else {
                hndl->var.ch_cfg_ptr[chnl].state = MPMO_CHNL_START;
            }                                
        }
        else
        {
            hndl->var.ch_cfg_ptr[chnl].state = MPMO_CHNL_START;
        }
    }

    /* check if this is the last channel with the frame period,
    ** if it is remove the frame period from DSP
    */
    if ((hndl->cfg.default_param.dsp_present)
        && (hndl->var.ch_cfg_ptr[chnl].cn_generate_en))
    {
        ret_val = mpmo_ch_cn_frame_period_remove(hndl, chnl);
    } else 
    {
        /* restore  Cn generation default */
        mpmo_dsp_field_CN_GENERATOR_ENABLE_set(NULL, hndl, chnl, 1);
    }

    PMC_RETURN(ret_val);

} /* mpmo_ch_deprov_internal */



/*******************************************************************************
*  mpmo_ch_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates a provisioned channel.
*   This function writes the calculated parameters for the provisioned channel
*   to hardware.
*
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
*   chnl           - channel to activate
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
PUBLIC PMC_ERROR mpmo_ch_activate(mpmo_handle_t *hndl, UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    PMC_ENTRY();
    
    ret_val = mpmo_ch_activate_internal(hndl, chnl, FALSE, TRUE);
    
    PMC_RETURN(ret_val);
} /* mpmo_ch_activate */

/*******************************************************************************
*  mpmo_ch_activate_super_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates a provisioned channel.
*   This function writes the calculated parameters for the provisioned channel
*   to hardware.
*
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
*   chnl           - channel to activate
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
PUBLIC PMC_ERROR mpmo_ch_activate_super_prov(mpmo_handle_t *hndl, UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    
    PMC_ENTRY();
    
    ret_val = mpmo_ch_activate_internal(hndl, chnl, TRUE, TRUE);
    
    PMC_RETURN(ret_val);
} /* mpmo_ch_activate_super_prov */

/*******************************************************************************
*  mpmo_scbs3_all_chnls_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates all provisioned channel.
*   This function writes the calculated parameters for the provisioned channel
*   to hardware.
*
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
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
PUBLIC PMC_ERROR mpmo_scbs3_all_chnls_activate(mpmo_handle_t *hndl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    ret_val = scbs3_page_udpate_and_swap(hndl->scbs3_handle);
    
    PMC_RETURN(ret_val);
} /* mpmo_scbs3_all_chnls_activate */

/*******************************************************************************
*  mpmo_scbs3_all_chnls_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates all provisioned channel.
*   This function writes the calculated parameters for the provisioned channel
*   to hardware.
*
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
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
PUBLIC PMC_ERROR mpmo_scbs3_all_chnls_deactivate(mpmo_handle_t *hndl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    ret_val = scbs3_page_udpate_and_swap(hndl->scbs3_handle);
    
    PMC_RETURN(ret_val);
} /* mpmo_scbs3_all_chnls_deactivate */

/*******************************************************************************
*  mpmo_ch_activate_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates a provisioned channel.
*   This function writes the calculated parameters for the provisioned channel
*   to hardware.
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   chnl              - channel to activate
*   super_prov        - boolean value to determine if this is called for
*                       COREOTN super prov mode
*   true_activate     - boolean value to determine if this is called for 
*                       a special maintenace signal insertion at COREOTN_CTL:MPMO
*                       without ODUKSW connection
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
PRIVATE PMC_ERROR mpmo_ch_activate_internal(mpmo_handle_t *hndl, UINT32 chnl, 
                                            BOOL8 super_prov,
                                            BOOL8 true_activate)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (true_activate)
    {
        if (hndl->var.ch_cfg_ptr[chnl].state != MPMO_CHNL_EQUIPPED) {
                PMC_RETURN(MPMO_LOG_CODE_INVALID_CHANNEL_STATE);
        }
    }
    else
    {
        if (hndl->var.ch_cfg_ptr[chnl].state != MPMO_CHNL_MAINT_SIG_EQUIPPED) {
            PMC_RETURN(MPMO_LOG_CODE_INVALID_CHANNEL_STATE);
        } 
    }
    
    /* Write MPMO settings to hardware */
    mpmo_ch_init(hndl, chnl);
    mpmo_ch_threshold_cfg(hndl, chnl);
    mpmo_ch_bw_cfg(hndl, chnl);


    if(true_activate)
    {

        /* Build CFC FIFO */
        ret_val = cfc_fifo_build(hndl->cfc_handle, chnl, hndl->var.ch_cfg_ptr[chnl].cfc_num_pages);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }

    }

    /* Activate SCBS3 and write the configuration to hardware */
    if (hndl->cfg.scbs3_cfg.scbs3_present) {
        scbs3_chnl_activate(hndl->scbs3_handle, chnl);
      
        /* check if COREOTN is operating in super prov mode */      
        if (super_prov)
        {   
            /* only write to the offline page without page swap for each channel */
            if (hndl->cfg.default_param.mpmo_type_id != MPMO_COREOTN_MUX_FRAMER)
            {
                scbs3_page_udpate_and_swap(hndl->scbs3_handle);
            }   
        }
        else {
            scbs3_page_udpate_and_swap(hndl->scbs3_handle);
        }            
    }

    
    if(true_activate)
    {
        /* take the specified CFC FIFO out of reset */
        ret_val = cfc_fifo_activate(hndl->cfc_handle,chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }



    /* Enable the MPMO channel */
    mpmo_ch_enable_cfg(hndl, chnl, TRUE);
    /* Enable DSP if generating Cn. */
    if (hndl->var.ch_cfg_ptr[chnl].cn_generate_en) {
        mpmo_dsp_ch_enable_cfg(hndl, chnl, TRUE);
    } else {
        mpmo_dsp_ch_enable_cfg(hndl, chnl, FALSE);
    }
    
    /* Update channel state */  
    if (true_activate)     
    {
        hndl->var.ch_cfg_ptr[chnl].state = MPMO_CHNL_OPERATIONAL;
    }
    else
    {
       hndl->var.ch_cfg_ptr[chnl].state = MPMO_CHNL_MAINT_SIG_OPERATIONAL;
    }
    
    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_ch_activate_internal */

/*******************************************************************************
*  mpmo_ch_activate_for_maint_sig_insertion
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates a provisioned channel.
*   This function writes the calculated parameters for the provisioned channel
*   to hardware.
*
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
*   chnl           - channel to activate
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
PUBLIC PMC_ERROR mpmo_ch_activate_for_maint_sig_insertion(mpmo_handle_t *hndl, UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
  
    PMC_ENTRY();
      
    ret_val = mpmo_ch_activate_internal(hndl, chnl, FALSE, FALSE);
    
    PMC_RETURN(ret_val);
} /* mpmo_ch_activate_for_maint_sig_insertion */   


/*******************************************************************************
*  mpmo_ch_deactivate_super_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates a provisioned channel.
*   This function disables the specified channel, and frees the FIFO and
*   calendar resources used by the channel.
*
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
*   chnl           - channel to deactivate
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
PUBLIC PMC_ERROR mpmo_ch_deactivate_super_prov(mpmo_handle_t *hndl,
                                               UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
  
    ret_val = mpmo_ch_deactivate_internal(hndl,
                                          chnl,
                                          TRUE,
                                          TRUE);

    PMC_RETURN(ret_val);

} /* mpmo_ch_deactivate */


/*******************************************************************************
*  mpmo_ch_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates a provisioned channel.
*   This function disables the specified channel, and frees the FIFO and
*   calendar resources used by the channel.
*
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
*   chnl           - channel to deactivate
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
PUBLIC PMC_ERROR mpmo_ch_deactivate(mpmo_handle_t *hndl,
                                    UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
  
    ret_val = mpmo_ch_deactivate_internal(hndl,
                                          chnl,
                                          TRUE,
                                          FALSE);

    PMC_RETURN(ret_val);

} /* mpmo_ch_deactivate */


/*******************************************************************************
*  mpmo_ch_deactivate_for_maint_sig_insertion
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates a provisioned channel.
*   This function disables the specified channel, and frees the FIFO and
*   calendar resources used by the channel.
*
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
*   chnl           - channel to deactivate
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
PUBLIC PMC_ERROR mpmo_ch_deactivate_for_maint_sig_insertion(mpmo_handle_t *hndl,
                                                            UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
  
    ret_val = mpmo_ch_deactivate_internal(hndl,
                                          chnl,
                                          FALSE,
                                          FALSE);

    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_ch_deactivate */



/*******************************************************************************
*  mpmo_ch_deactivate_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates a provisioned channel.
*   This function disables the specified channel, and frees the FIFO and
*   calendar resources used by the channel.
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   chnl              - channel to deactivate
*   true_deactivate   - TRUE : Used to deativate all other MPMOs
*                              other than COREOTN CTL and FO1
*                              instances \n
*                       FASLE : the "dummy" deactivate (to
*                               support open ODUKSW connection).
*                               state must be ignored
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
PRIVATE PMC_ERROR mpmo_ch_deactivate_internal(mpmo_handle_t *hndl,
                                              UINT32 chnl,
                                              BOOL true_deactivate,
                                              BOOL super_deprov)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if(true_deactivate)
    {
        if (hndl->var.ch_cfg_ptr[chnl].state != MPMO_CHNL_OPERATIONAL) {
            PMC_RETURN(MPMO_LOG_CODE_INVALID_CHANNEL_STATE);
        }
    }
    else
    {
        if (hndl->var.ch_cfg_ptr[chnl].state != MPMO_CHNL_MAINT_SIG_OPERATIONAL) {
            PMC_RETURN(MPMO_LOG_CODE_INVALID_CHANNEL_STATE);
        }
    }

    /* it is required to keep the channel enabled and the calendar
    active for MPMO_COREOTN_CTL and MPMO_COREOTN_MUX_FRAMER MPMO until the
    last deactivate step (moving to MPMO_CHNL_MAINT_SIG_EQUIPPED)*/
    if (super_deprov || (true_deactivate && 
        ((hndl->cfg.default_param.mpmo_type_id != MPMO_COREOTN_CTL) &&
        (hndl->cfg.default_param.mpmo_type_id != MPMO_COREOTN_MUX_FRAMER))) || 
        true_deactivate == FALSE)
    {

        /* Disable the MPMO channel */
        mpmo_ch_enable_cfg(hndl, chnl, FALSE);
        mpmo_dsp_ch_enable_cfg(hndl, chnl, FALSE);

        /* Deactivate SCBS3 */
        if (hndl->cfg.scbs3_cfg.scbs3_present) {
            scbs3_chnl_deactivate(hndl->scbs3_handle, chnl);
            
            /* check if COREOTN is operating in super prov mode */      
            if (super_deprov)
            {   
                /* only write to the offline page without page swap for each channel */
                if (hndl->cfg.default_param.mpmo_type_id != MPMO_COREOTN_MUX_FRAMER)
                {
                    scbs3_page_udpate_and_swap(hndl->scbs3_handle);
                }   
            }
            else {
                scbs3_page_udpate_and_swap(hndl->scbs3_handle);
            } 
        }

    }

    if(true_deactivate)
    {
        /* Destroy CFC FIFO */
        ret_val = cfc_fifo_destroy(hndl->cfc_handle, chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
            
        /* put the specified CFC FIFO into reset */
        ret_val = cfc_fifo_deactivate(hndl->cfc_handle,chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }

    if(true_deactivate)
    {
        hndl->var.ch_cfg_ptr[chnl].state = MPMO_CHNL_EQUIPPED;        
    }
    else
    {
        hndl->var.ch_cfg_ptr[chnl].state = MPMO_CHNL_MAINT_SIG_EQUIPPED;       
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_ch_deactivate */

/*******************************************************************************
* mpmo_coreotn_fo1_scbs3_chnl_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates the specified channel in the FO1::MPMO::SCB3.
*
* INPUTS:
*   *hndl           - pointer to MPMO handle instance
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
PUBLIC PMC_ERROR mpmo_coreotn_fo1_scbs3_chnl_deactivate(mpmo_handle_t *hndl, UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    util_schd_chnl_state_t chnl_state_ptr;

    PMC_ENTRY();

    ret_val = util_schd_chnl_state_get(hndl->scbs3_handle->util_schd_handle,
                                       chnl, &chnl_state_ptr);
                                       
    /* Move channel state to UTIL_SCHD_CHNL_START */
    if (PMC_SUCCESS == ret_val && chnl_state_ptr == UTIL_SCHD_CHNL_OPERATIONAL)
    {
        ret_val = util_schd_chnl_state_set(hndl->scbs3_handle->util_schd_handle,
                                           chnl,
                                           UTIL_SCHD_CHNL_START);
    
    
        /* set dummy_chnl_state to FALSE */
        if (PMC_SUCCESS == ret_val)
        {
            ret_val = util_schd_chnl_state_dummy_set(hndl->scbs3_handle->util_schd_handle,
                                                     chnl, FALSE);
        }                                               
    }
    PMC_RETURN(ret_val);

} /* mpmo_coreotn_fo1_scbs3_chnl_deactivate */



/*******************************************************************************
*  mpmo_ch_restore_clean
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clean a set of channels. Memory context and register space are cleaned
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   clean_chnl        - set of channels to clean
*   num_clean_chnl    - number of channels to clean
*   restore_chnl      - set of channels to restore
*   num_restore_chnl  - number of channels to restore
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
PUBLIC void mpmo_ch_restore_clean(mpmo_handle_t *hndl, 
                                  UINT32 *clean_chnl, 
                                  UINT32 num_clean_chnl, 
                                  UINT32 *restore_chnl, 
                                  UINT32 num_restore_chnl)
{
    UINT32 clean_chnl_itr, restore_chnl_itr;
    BOOL8 equipped = FALSE;

    UINT32 equipped_chnl[MPMO_NUM_CHNL];
    UINT32 true_clean_chnl[MPMO_NUM_CHNL];
    UINT32 num_equipped_chnl, num_true_clean_chnl;

    PMC_ENTRY();
    
    num_true_clean_chnl = 0;
    num_equipped_chnl = 0;

    for(clean_chnl_itr = 0; clean_chnl_itr < num_clean_chnl;clean_chnl_itr++)
    {
        equipped = FALSE;
        if (hndl->var.ch_cfg_ptr[clean_chnl[clean_chnl_itr]].state == MPMO_CHNL_OPERATIONAL ||
            hndl->var.ch_cfg_ptr[clean_chnl[clean_chnl_itr]].state == MPMO_CHNL_EQUIPPED ||
            hndl->var.ch_cfg_ptr[clean_chnl[clean_chnl_itr]].state == MPMO_CHNL_MAINT_SIG_OPERATIONAL)
        {
             for(restore_chnl_itr = 0; restore_chnl_itr < num_restore_chnl && equipped == FALSE;restore_chnl_itr++)
             {
                 if (restore_chnl[restore_chnl_itr] == clean_chnl[clean_chnl_itr])
                 {
                     equipped = TRUE;
                     hndl->var.ch_cfg_ptr[clean_chnl[clean_chnl_itr]].state = MPMO_CHNL_MAINT_SIG_OPERATIONAL;
                     equipped_chnl[num_equipped_chnl] = clean_chnl[clean_chnl_itr];
                     num_equipped_chnl += 1; 
                 }
             }
        }
        if (equipped == FALSE)
        {
            true_clean_chnl[num_true_clean_chnl] = clean_chnl[clean_chnl_itr];
            num_true_clean_chnl += 1;
        }
    }
  
    /* clean channel */
    if (num_true_clean_chnl != 0)
    {
        mpmo_ch_clean(hndl,&true_clean_chnl[0],num_true_clean_chnl);
    }
    /* CFC FIFO cleanup */
    if (num_equipped_chnl != 0)
    {
        cfc_fifo_clean(hndl->cfc_handle, &equipped_chnl[0],num_equipped_chnl);
    }
    
    PMC_RETURN();

} /* mpmo_ch_restore_clean */

/*******************************************************************************
*  mpmo_ch_clean
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clean a set of channels. Memory context and register space are cleaned
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   chnl              - set of channels to clean
*   num_chnl          - number of channels to clean
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
PUBLIC void mpmo_ch_clean(mpmo_handle_t *hndl, UINT32 *chnl, UINT32 num_chnl)
{
    UINT32 chnl_itr;    
    
    PMC_ENTRY();
    
    /* clean sub-blocks */
    if (hndl->cfg.scbs3_cfg.scbs3_present) 
    {
        scbs3_chnl_clean(hndl->scbs3_handle, TRUE,chnl,num_chnl);
    }
    cfc_fifo_clean(hndl->cfc_handle, chnl,num_chnl);

    for (chnl_itr = 0; chnl_itr < num_chnl;chnl_itr ++)
    {
        /* Disable the MPMO channel */
        mpmo_ch_enable_cfg(hndl, chnl[chnl_itr], FALSE);
        mpmo_dsp_ch_enable_cfg(hndl, chnl[chnl_itr], FALSE);
        /* disable SSF CN CENTER_EN */
        mpmo_ch_ssf_cn_center_cfg(hndl, chnl[chnl_itr], FALSE);
        hndl->var.ch_cfg_ptr[chnl[chnl_itr]].golden_cn_cal_en = FALSE;
        hndl->var.ch_cfg_ptr[chnl[chnl_itr]].state = MPMO_CHNL_START;      
        if (hndl->cfg.default_param.dsp_present)
        {
            mpmo_ch_cn_frame_period_remove(hndl, chnl[chnl_itr]);
        }
    }

    PMC_RETURN();

} /* mpmo_ch_clean */

#ifdef PMC_INERNAL
/*******************************************************************************
*  mpmo_ch_resize_initial
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initial steps to resize the bandwidth of the specified channel.
*   This function should be called before starting the ramp.
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   chnl              - channel to resize
*   resize_type       - resize type (increase/decrease bandwidth)
*   client_bitrate    - client bit rate (in Hz)
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
PUBLIC PMC_ERROR mpmo_ch_resize_initial(mpmo_handle_t *hndl, UINT32 chnl,
                                        mpmo_resize_type_t resize_type,
                                        DOUBLE client_bitrate)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    BOOL8 bmp_map_mode = FALSE;

    PMC_ENTRY();

    if (hndl->var.ch_cfg_ptr[chnl].state != MPMO_CHNL_OPERATIONAL) {
        PMC_RETURN(MPMO_LOG_CODE_INVALID_CHANNEL_STATE);
    }

    if (MPMO_CHNL_INCREASE_BW == resize_type) {

        /* Calculate the number of pages for CFC */
        hndl->var.ch_cfg_ptr[chnl].cfc_num_pages = mpmo_cfc_num_pages_calc(hndl, client_bitrate);
        /* Resize CFC FIFO */
        ret_val = cfc_fifo_resize(hndl->cfc_handle,
                                  chnl, hndl->var.ch_cfg_ptr[chnl].cfc_num_pages);
        if (ret_val != PMC_SUCCESS) {
            /* Need to return SCBS3 to old rate. */
            PMC_RETURN(ret_val);
        }

        /* Update XOFF_THRES (back-pressure) and DATA_AVAIL threshold associated with the channel). */
        hndl->var.ch_cfg_ptr[chnl].xoff_thres = mpmo_xoff_thres_calc(hndl, hndl->var.ch_cfg_ptr[chnl].cfc_num_pages, bmp_map_mode);


        mpmo_ch_threshold_cfg(hndl, chnl);
    }

    /* Enable holdover for the DSP for the channel */
    if (hndl->var.ch_cfg_ptr[chnl].cn_generate_en) {
        mpmo_dsp_ch_holdover_cfg(hndl, chnl, TRUE);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_ch_resize_initial */

/*******************************************************************************
*  mpmo_ch_resize_ramp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the MPMO during bandwidth ramp for the specified channel.
*   This function should be called after the ramp has started.
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   chnl              - channel to resize
*   resize_step       - resize step in bytes [19:0].
*                       Two's complement signed fractional number.
*                       The output Cn value is incremented by BWR_STEP/2^16
*                       bytes every output Cn frame period.
*   client_bitrate    - client bit rate (in Hz)
*   pkt_size          - packet size configured at the packet maker.
*   pkt_per_int       - integer portion for packet period
*                       configured at the packet maker.
*   pkt_per_n         - numerator for packet period fraction
*                       configured at the packet maker.
*   pkt_per_d         - denominator for packet period fraction
*                       configured at the packet maker.
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
PUBLIC PMC_ERROR mpmo_ch_resize_ramp(mpmo_handle_t *hndl, UINT32 chnl,
                                     UINT32 resize_step, DOUBLE client_bitrate,
                                     UINT32 pkt_size, UINT32 pkt_per_int, UINT32 pkt_per_n, UINT32 pkt_per_d)
{
    UINT32 *pp_int_ptr   = &(hndl->var.ch_cfg_ptr[chnl].pkt_period_int);
    UINT32 *pp_n_ptr     = &(hndl->var.ch_cfg_ptr[chnl].pkt_period_n);
    UINT32 *pp_d_ptr     = &(hndl->var.ch_cfg_ptr[chnl].pkt_period_d);
    UINT32 *ps_ptr       = &(hndl->var.ch_cfg_ptr[chnl].pkt_size);

    UINT32 *cn_base_int_ptr  = &(hndl->var.ch_cfg_ptr[chnl].cn_base_int);
    UINT32 *cn_base_n_ptr    = &(hndl->var.ch_cfg_ptr[chnl].cn_base_n);
    UINT32 *cn_base_d_ptr    = &(hndl->var.ch_cfg_ptr[chnl].cn_base_d);
    UINT32 *frm_period_n_ptr = &(hndl->var.ch_cfg_ptr[chnl].frm_period_n);
    UINT32 *frm_period_d_ptr = &(hndl->var.ch_cfg_ptr[chnl].frm_period_d);

    PMC_ENTRY();

    if (hndl->var.ch_cfg_ptr[chnl].state != MPMO_CHNL_OPERATIONAL) {
        PMC_RETURN(MPMO_LOG_CODE_INVALID_CHANNEL_STATE);
    }

    /* Only need to configure DSP if Cn generation is enabled. */
    if (!hndl->var.ch_cfg_ptr[chnl].cn_generate_en) {
        PMC_RETURN(PMC_SUCCESS);
    }

    /* Check for valid packet size range. */
    if ((pkt_size < MPMO_PKT_SIZE_MIN) || (pkt_size > MPMO_PKT_SIZE_MAX)) {
        PMC_RETURN(MPMO_LOG_ERR_INVALID_PARAMETERS);
    }
    /* Record the specified packet size. */
    *ps_ptr = pkt_size;

    /* Disable the DSP for the channel */
    mpmo_dsp_ch_enable_cfg(hndl, chnl, FALSE);

    /* Record the specified packet period. */
    if (0 == pkt_per_d) {
        PMC_RETURN(MPMO_LOG_ERR_INVALID_PARAMETERS);
    } else {
        *pp_int_ptr   = pkt_per_int;
        *pp_n_ptr     = pkt_per_n;
        *pp_d_ptr     = pkt_per_d;
    }

    /* Calculate the fractional representation of Cn,base. */
    util_opsa_mpmo_cn_base_calc(*frm_period_n_ptr, *frm_period_d_ptr,
                                *pp_int_ptr, *pp_n_ptr, *pp_d_ptr, *ps_ptr,
                                cn_base_int_ptr, cn_base_n_ptr, cn_base_d_ptr);

    /* Write MPMO settings to hardware */
    mpmo_ch_bw_cfg(hndl, chnl);

    /* Configure the resize step. */
    mpmo_dsp_field_BWR_STEP_set(NULL, hndl, chnl,resize_step);

    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_ch_resize_ramp */

/*******************************************************************************
*  mpmo_ch_resize_end
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Completes the bandwidth resize of the specified channel.
*   This function should be called after the ramp has completed.
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
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
PUBLIC PMC_ERROR mpmo_ch_resize_end(mpmo_handle_t *hndl, UINT32 chnl,
                                    mpmo_resize_type_t resize_type,
                                    DOUBLE client_bitrate)
{
    PMC_ERROR ret_val;
    BOOL8 bmp_map_mode = FALSE;
    PMC_ENTRY();

    if (hndl->var.ch_cfg_ptr[chnl].state != MPMO_CHNL_OPERATIONAL) {
        PMC_RETURN(MPMO_LOG_CODE_INVALID_CHANNEL_STATE);
    }

    /* Configure DSP if Cn generation is enabled. */
    if (!hndl->var.ch_cfg_ptr[chnl].cn_generate_en) {
        /* Enable the DSP for the channel */
        mpmo_dsp_ch_enable_cfg(hndl, chnl, TRUE);

        /* Disable holdover for the DSP for the channel */
        mpmo_dsp_ch_holdover_cfg(hndl, chnl, FALSE);
    }

    if (MPMO_CHNL_DECREASE_BW == resize_type) {

        /* Calculate the number of pages for CFC */
        hndl->var.ch_cfg_ptr[chnl].cfc_num_pages = mpmo_cfc_num_pages_calc(hndl, client_bitrate);

        /* Update XOFF_THRES (back-pressure) and DATA_AVAIL threshold associated with the channel). */
        hndl->var.ch_cfg_ptr[chnl].xoff_thres = mpmo_xoff_thres_calc(hndl, hndl->var.ch_cfg_ptr[chnl].cfc_num_pages, bmp_map_mode);
        mpmo_ch_threshold_cfg(hndl, chnl);

        /* Resize CFC FIFO */
        ret_val = cfc_fifo_resize(hndl->cfc_handle,
                                  chnl, hndl->var.ch_cfg_ptr[chnl].cfc_num_pages);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_ch_resize_end */
#endif /* PMC_INERNAL */

/*******************************************************************************
*  mpmo_ch_latency_cfg_double
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function calculates the latency required in MPMO to hold off reading
*   data from ODUKSW.
*
* INPUTS:
*   *hndl               - pointer to MPMO handle instance
*   chnl                - channel to provision
*   client_bitrate      - client bit rate (in Hz)
*   pkt_size            - packet size configured at the packet maker.
*   fifo_depth          - depth of ODUKSW based on the channel rate information
*   hys_buffer_en       - custom hysteresis buffer size mode:\n
*                         TRUE : custom hysteresis buffer size mode is enable 
*                         FALSE : custom hysteresis buffer size mode is disabled
*   hys_buffer          - hysteresis buffer size in refclk cycles
*   latency_s           - backplane latency in seconds. This represents a
*                         'centered' FIFO. Only valid if backplane inteface is
*                         used in the datapath. Otherwise, set to 0.
*   s_old_ratio         - percentage of ODUKSW depth that is used to calculate
*                         AGE_OLD. (AGE_OLD lies between high threshold of the
*                         age of the packet and the ideal age of the packet).
*   odu_flex_client     - TRUE : the channel is an ODUflex client
*                         FALSE : the channel is not an ODUflex client
*   num_odu_flex        - if odu_flex_client == TRUE, the number of
*                         tribslots the flex occupies
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
PUBLIC PMC_ERROR mpmo_ch_latency_cfg_double(mpmo_handle_t *hndl, UINT32 chnl,
                                     DOUBLE client_bitrate, UINT32 pkt_size,
                                     UINT32 fifo_depth, BOOL hys_buffer_en, 
                                     UINT32 hys_buffer,
                                     UINT32 latency_s,
                                     UINT32 s_old_ratio,
                                     BOOL   odu_flex_client,
                                     UINT32 num_odu_flex)
{
    /* variable declaration */
    PMC_ERROR ret_code = PMC_SUCCESS;
    UINT32 age_thres_high;
    UINT32 age_thres_low;
    UINT32 age_ideal;
    UINT32 age_old;
    UINT32 age_thres_delta_words;
    UINT32 age_thres_delta_refclk;
    UINT32 age_old_words;
    UINT32 age_old_refclk;
    UINT32 center_refclk;
    UINT32 age_center_words;
    DOUBLE words_per_pkt;
    UINT32 one_pkt_latency;

    mpmo_buffer_t mpmo_buf[1];

    PMC_ENTRY();
    


    if(odu_flex_client == TRUE && hys_buffer_en == FALSE )
    {
        fifo_depth = fifo_depth/num_odu_flex;

        /* number of words from centered FIFO to upper and lower 'fences' */
        words_per_pkt = (DOUBLE)((pkt_size + UTIL_OPSA_HEADER_SIZE) / (DOUBLE)UTIL_OPSA_DPI_WORD_SIZE/1.0);
        age_thres_delta_words = (UINT32)( (fifo_depth- words_per_pkt) * (DOUBLE)(0.5 - MPMO_UPPER_LOWER_FENCE));



        ret_code = util_opsa_mpmo_latency_calc(UTIL_GLOBAL_ODU2_TS_RATE, pkt_size,
                                               age_thres_delta_words,
                                               &age_thres_delta_refclk);

        if (ret_code != PMC_SUCCESS)
        {
            PMC_RETURN(ret_code);
        }

        /* number of words from centered FIFO to s_old threshold */
        age_old_words = (UINT32)( (fifo_depth- words_per_pkt) * (DOUBLE)(s_old_ratio/100.0));

        ret_code = util_opsa_mpmo_latency_calc(UTIL_GLOBAL_ODU2_TS_RATE, pkt_size,
                                               age_old_words, &age_old_refclk);


        if (ret_code != PMC_SUCCESS)
        {
            PMC_RETURN(ret_code);
        }

        /* number of words in a centered FIFO */
        age_center_words = (UINT32)( (fifo_depth- words_per_pkt) * 0.5 ) + words_per_pkt;
        
        ret_code = util_opsa_mpmo_latency_calc(UTIL_GLOBAL_ODU2_TS_RATE, pkt_size,
                                               age_center_words, &center_refclk);

    }
    else if(odu_flex_client == FALSE && hys_buffer_en == FALSE )
    {

        /* number of words from centered FIFO to upper and lower 'fences' */
        words_per_pkt = (DOUBLE)((pkt_size + UTIL_OPSA_HEADER_SIZE) / (DOUBLE)UTIL_OPSA_DPI_WORD_SIZE/1.0);
        age_thres_delta_words = (UINT32)( (fifo_depth- words_per_pkt) * (DOUBLE)(0.5 - MPMO_UPPER_LOWER_FENCE));



        ret_code = util_opsa_mpmo_latency_calc(client_bitrate, pkt_size,
                                               age_thres_delta_words,
                                               &age_thres_delta_refclk);

        if (ret_code != PMC_SUCCESS)
        {
            PMC_RETURN(ret_code);
        }

        /* number of words from centered FIFO to s_old threshold */
        age_old_words = (UINT32)( (fifo_depth- words_per_pkt) * (DOUBLE)(s_old_ratio/100.0));
    
        ret_code = util_opsa_mpmo_latency_calc(client_bitrate, pkt_size,
                                               age_old_words, &age_old_refclk);
    
    
        if (ret_code != PMC_SUCCESS)
        {
            PMC_RETURN(ret_code);
        }

        /* number of words in a centered FIFO */
        age_center_words = (UINT32)( (fifo_depth- words_per_pkt) * 0.5 ) + words_per_pkt;
        
        ret_code = util_opsa_mpmo_latency_calc(client_bitrate, pkt_size,
                                               age_center_words, &center_refclk);
        if (ret_code != PMC_SUCCESS)
        {
            PMC_RETURN(ret_code);
        }
    } 
    else /* hys_buffer_en == TRUE */
    {
        words_per_pkt = PMC_CEIL ( (DOUBLE)((pkt_size + UTIL_OPSA_HEADER_SIZE) / (DOUBLE)UTIL_OPSA_DPI_WORD_SIZE/1.0) );
        
        if(odu_flex_client)
        {
            ret_code = util_opsa_mpmo_latency_calc(client_bitrate, pkt_size,
                                                  (UINT32) words_per_pkt,
                                                   &one_pkt_latency);
        }
        else
        {
            ret_code = util_opsa_mpmo_latency_calc(UTIL_GLOBAL_ODU2_TS_RATE, pkt_size,
                                                  (UINT32) words_per_pkt,
                                                   &one_pkt_latency);
        }

        if (ret_code != PMC_SUCCESS)
        {
            PMC_RETURN(ret_code);
        }
        
        age_thres_delta_refclk = (hys_buffer - one_pkt_latency) *  (0.5 - MPMO_UPPER_LOWER_FENCE);
        age_old_refclk = (hys_buffer - one_pkt_latency) * (DOUBLE)(s_old_ratio / 100.0);
        center_refclk = (hys_buffer - one_pkt_latency) *(0.5) + one_pkt_latency;
        
    }                                  
    center_refclk += latency_s;
    
    age_thres_high = center_refclk + age_thres_delta_refclk;
    age_thres_low = center_refclk - age_thres_delta_refclk;
    age_old = center_refclk + age_old_refclk;
    age_ideal = center_refclk;

    PMC_LOG_TRACE("age_thres_high = %u, age_thres_low = %u, age_old = %u, age_ideal = %u\n", age_thres_high, age_thres_low, age_old, age_ideal);

    mpmo_buffer_init(mpmo_buf, hndl);
    mpmo_field_AGE_IDEAL_set(mpmo_buf, hndl, chnl, age_ideal);
    mpmo_field_AGE_OLD_set(mpmo_buf, hndl, chnl, age_old);
    mpmo_buffer_flush(mpmo_buf); 
    
    mpmo_field_AGE_THRES_3_2_set(mpmo_buf, hndl, chnl, age_thres_high);
    mpmo_field_AGE_THRES_1_0_set(mpmo_buf, hndl, chnl, age_thres_low);    
    mpmo_buffer_flush(mpmo_buf);

    PMC_RETURN(PMC_SUCCESS);
} /* mpmo_ch_latency_cfg_double */

/*******************************************************************************
*  mpmo_ch_latency_cfg_float
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function calculates the latency required in MPMO to hold off reading
*   data from ODUKSW.
*
* INPUTS:
*   *hndl               - pointer to MPMO handle instance
*   chnl                - channel to provision
*   client_bitrate      - client bit rate (in Hz)
*   pkt_size            - packet size configured at the packet maker.
*   fifo_depth          - depth of ODUKSW based on the channel rate information
*   hys_buffer_en       - custom hysteresis buffer size mode:\n
*                         TRUE : custom hysteresis buffer size mode is enable 
*                         FALSE : custom hysteresis buffer size mode is disabled
*   hys_buffer          - hysteresis buffer size in refclk cycles
*   latency_s           - backplane latency in seconds. This represents a
*                         'centered' FIFO. Only valid if backplane inteface is
*                         used in the datapath. Otherwise, set to 0.
*   s_old_ratio         - percentage of ODUKSW depth that is used to calculate
*                         AGE_OLD. (AGE_OLD lies between high threshold of the
*                         age of the packet and the ideal age of the packet).
*   odu_flex_client     - TRUE : the channel is an ODUflex client
*                         FALSE : the channel is not an ODUflex client
*   num_odu_flex        - if odu_flex_client == TRUE, the number of
*                         tribslots the flex occupies
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
PUBLIC PMC_ERROR mpmo_ch_latency_cfg_float(mpmo_handle_t *hndl, UINT32 chnl,
                                     DOUBLE client_bitrate, UINT32 pkt_size,
                                     UINT32 fifo_depth, BOOL hys_buffer_en, 
                                     UINT32 hys_buffer,
                                     UINT32 latency_s,
                                     UINT32 s_old_ratio,
                                     BOOL   odu_flex_client,
                                     UINT32 num_odu_flex)
{
    /* variable declaration */
    PMC_ERROR ret_code = PMC_SUCCESS;
    UINT32 age_thres_high;
    UINT32 age_thres_low;
    UINT32 age_ideal;
    UINT32 age_old;
    UINT32 age_thres_delta_words;
    UINT32 age_thres_delta_refclk;
    UINT32 age_old_words;
    UINT32 age_old_refclk;
    UINT32 center_refclk;
    UINT32 age_center_words;
    FLOAT words_per_pkt;
    UINT32 one_pkt_latency;

    mpmo_buffer_t mpmo_buf[1];

    PMC_ENTRY();
    


    if(odu_flex_client == TRUE && hys_buffer_en == FALSE )
    {
        fifo_depth = fifo_depth/num_odu_flex;

        /* number of words from centered FIFO to upper and lower 'fences' */
        words_per_pkt = (FLOAT)((pkt_size + UTIL_OPSA_HEADER_SIZE) / (FLOAT)UTIL_OPSA_DPI_WORD_SIZE/1.0F);
        age_thres_delta_words = (UINT32)( (fifo_depth- words_per_pkt) * (FLOAT)(0.5F - (FLOAT)MPMO_UPPER_LOWER_FENCE));



        ret_code = util_opsa_mpmo_latency_calc(UTIL_GLOBAL_ODU2_TS_RATE, pkt_size,
                                               age_thres_delta_words,
                                               &age_thres_delta_refclk);

        if (ret_code != PMC_SUCCESS)
        {
            PMC_RETURN(ret_code);
        }

        /* number of words from centered FIFO to s_old threshold */
        age_old_words = (UINT32)( (fifo_depth- words_per_pkt) * (FLOAT)(s_old_ratio/100.0F));

        ret_code = util_opsa_mpmo_latency_calc(UTIL_GLOBAL_ODU2_TS_RATE, pkt_size,
                                               age_old_words, &age_old_refclk);


        if (ret_code != PMC_SUCCESS)
        {
            PMC_RETURN(ret_code);
        }

        /* number of words in a centered FIFO */
        age_center_words = (UINT32)( (fifo_depth- words_per_pkt) * 0.5F ) + words_per_pkt;
        
        ret_code = util_opsa_mpmo_latency_calc(UTIL_GLOBAL_ODU2_TS_RATE, pkt_size,
                                               age_center_words, &center_refclk);

    }
    else if(odu_flex_client == FALSE && hys_buffer_en == FALSE )
    {

        /* number of words from centered FIFO to upper and lower 'fences' */
        words_per_pkt = (FLOAT)((pkt_size + UTIL_OPSA_HEADER_SIZE) / (FLOAT)UTIL_OPSA_DPI_WORD_SIZE/1.0F);
        age_thres_delta_words = (UINT32)( (fifo_depth- words_per_pkt) * (FLOAT)(0.5F - (FLOAT)MPMO_UPPER_LOWER_FENCE));



        ret_code = util_opsa_mpmo_latency_calc(client_bitrate, pkt_size,
                                               age_thres_delta_words,
                                               &age_thres_delta_refclk);

        if (ret_code != PMC_SUCCESS)
        {
            PMC_RETURN(ret_code);
        }

        /* number of words from centered FIFO to s_old threshold */
        age_old_words = (UINT32)( (fifo_depth- words_per_pkt) * (FLOAT)(s_old_ratio/100.0F));
    
        ret_code = util_opsa_mpmo_latency_calc(client_bitrate, pkt_size,
                                               age_old_words, &age_old_refclk);
    
    
        if (ret_code != PMC_SUCCESS)
        {
            PMC_RETURN(ret_code);
        }

        /* number of words in a centered FIFO */
        age_center_words = (UINT32)( (fifo_depth- words_per_pkt) * 0.5F ) + words_per_pkt;
        
        ret_code = util_opsa_mpmo_latency_calc(client_bitrate, pkt_size,
                                               age_center_words, &center_refclk);
        if (ret_code != PMC_SUCCESS)
        {
            PMC_RETURN(ret_code);
        }
    } 
    else /* hys_buffer_en == TRUE */
    {
        words_per_pkt = PMC_CEILF( (FLOAT)((pkt_size + UTIL_OPSA_HEADER_SIZE) / (FLOAT)UTIL_OPSA_DPI_WORD_SIZE/1.0F) );
        
        if(odu_flex_client)
        {
            ret_code = util_opsa_mpmo_latency_calc(client_bitrate, pkt_size,
                                                  (UINT32) words_per_pkt,
                                                   &one_pkt_latency);
        }
        else
        {
            ret_code = util_opsa_mpmo_latency_calc(UTIL_GLOBAL_ODU2_TS_RATE, pkt_size,
                                                  (UINT32) words_per_pkt,
                                                   &one_pkt_latency);
        }

        if (ret_code != PMC_SUCCESS)
        {
            PMC_RETURN(ret_code);
        }
        
        age_thres_delta_refclk = (hys_buffer - one_pkt_latency) *  (0.5F - (FLOAT)MPMO_UPPER_LOWER_FENCE);
        age_old_refclk = (hys_buffer - one_pkt_latency) * (FLOAT)(s_old_ratio / 100.0F);
        center_refclk = (hys_buffer - one_pkt_latency) *(0.5F) + one_pkt_latency;
        
    }                                  
    center_refclk += latency_s;
    
    age_thres_high = center_refclk + age_thres_delta_refclk;
    age_thres_low = center_refclk - age_thres_delta_refclk;
    age_old = center_refclk + age_old_refclk;
    age_ideal = center_refclk;

    PMC_LOG_TRACE("age_thres_high = %u, age_thres_low = %u, age_old = %u, age_ideal = %u\n", age_thres_high, age_thres_low, age_old, age_ideal);

    mpmo_buffer_init(mpmo_buf, hndl);
    mpmo_field_AGE_IDEAL_set(mpmo_buf, hndl, chnl, age_ideal);
    mpmo_field_AGE_OLD_set(mpmo_buf, hndl, chnl, age_old);
    mpmo_buffer_flush(mpmo_buf); 
    
    mpmo_field_AGE_THRES_3_2_set(mpmo_buf, hndl, chnl, age_thres_high);
    mpmo_field_AGE_THRES_1_0_set(mpmo_buf, hndl, chnl, age_thres_low);    
    mpmo_buffer_flush(mpmo_buf);

    PMC_RETURN(PMC_SUCCESS);
} /* mpmo_ch_latency_cfg_float */

/*******************************************************************************
*  mpmo_ch_latency_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function calculates the latency required in MPMO to hold off reading
*   data from ODUKSW.
*
* INPUTS:
*   *hndl               - pointer to MPMO handle instance
*   chnl                - channel to provision
*   client_bitrate      - client bit rate (in Hz)
*   pkt_size            - packet size configured at the packet maker.
*   fifo_depth          - depth of ODUKSW based on the channel rate information
*   hys_buffer_en       - custom hysteresis buffer size mode:\n
*                         TRUE : custom hysteresis buffer size mode is enable 
*                         FALSE : custom hysteresis buffer size mode is disabled
*   hys_buffer          - hysteresis buffer size in refclk cycles
*   latency_s           - backplane latency in seconds. This represents a
*                         'centered' FIFO. Only valid if backplane inteface is
*                         used in the datapath. Otherwise, set to 0.
*   s_old_ratio         - percentage of ODUKSW depth that is used to calculate
*                         AGE_OLD. (AGE_OLD lies between high threshold of the
*                         age of the packet and the ideal age of the packet).
*   odu_flex_client     - TRUE : the channel is an ODUflex client
*                         FALSE : the channel is not an ODUflex client
*   num_odu_flex        - if odu_flex_client == TRUE, the number of
*                         tribslots the flex occupies
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
PUBLIC PMC_ERROR mpmo_ch_latency_cfg(mpmo_handle_t *hndl, UINT32 chnl,
                                     DOUBLE client_bitrate, UINT32 pkt_size,
                                     UINT32 fifo_depth, BOOL hys_buffer_en, 
                                     UINT32 hys_buffer,
                                     UINT32 latency_s,
                                     UINT32 s_old_ratio,
                                     BOOL   odu_flex_client,
                                     UINT32 num_odu_flex)
{
    PMC_ERROR result;

    PMC_ENTRY();

    if (!digi_use_float32_get())
    {
        result = mpmo_ch_latency_cfg_double(hndl, chnl,
                                     client_bitrate, pkt_size,
                                     fifo_depth, hys_buffer_en, 
                                     hys_buffer,
                                     latency_s,
                                     s_old_ratio,
                                     odu_flex_client,
                                     num_odu_flex);
    }
    else
    {
        result = mpmo_ch_latency_cfg_float(hndl, chnl,
                                     client_bitrate, pkt_size,
                                     fifo_depth, hys_buffer_en, 
                                     hys_buffer,
                                     latency_s,
                                     s_old_ratio,
                                     odu_flex_client,
                                     num_odu_flex);
    }

    PMC_RETURN(result);
} /* mpmo_ch_latency_cfg */

/*******************************************************************************
*  mpmo_cfc_fifo_overflow_mon
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Monitors CFC FIFO overflow interupt of the specified channel.
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   chnl              - channel to monitor

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
PUBLIC PMC_ERROR mpmo_cfc_fifo_overflow_mon(mpmo_handle_t *hndl, UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    ret_val = cfc_fifo_overflow_mon(hndl->cfc_handle, chnl);
    if (ret_val != PMC_SUCCESS)
    {
        PMC_RETURN(ret_val);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_cfc_fifo_overflow_mon */


/*******************************************************************************
*  mpmo_pkt_period_var_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrievs var_t data of packet size, packet period information
*   for a channel. 
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   chnl              - channel to operate on
*
* OUTPUTS:
*   *pkt_size_ptr     - pointer to configured packet size
*   *pkt_per_int_ptr  - pointer to configured integer portion of packet period
*   *pkt_per_n_ptr    - pointer to configured numerator of packet period fraction
*   *pkt_per_d_ptr    - pointer to configured denominator of packet period fraction
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mpmo_pkt_period_var_get(mpmo_handle_t *hndl, UINT32 chnl, 
                                         UINT32 *pkt_size_ptr,
                                         UINT32 *pkt_per_int_ptr,
                                         UINT32 *pkt_per_n_ptr, 
                                         UINT32 *pkt_per_d_ptr)
{
    /* variable declaration */ 
    UINT32 ps_ptr = (hndl->var.ch_cfg_ptr[chnl].pkt_size); 
    UINT32 pp_int_ptr = (hndl->var.ch_cfg_ptr[chnl].pkt_period_int);  
    UINT32 pp_n_ptr = (hndl->var.ch_cfg_ptr[chnl].pkt_period_n);  
    UINT32 pp_d_ptr = (hndl->var.ch_cfg_ptr[chnl].pkt_period_d);

    PMC_ENTRY();

    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].pkt_size=%d \n", hndl->var.ch_cfg_ptr[chnl].pkt_size);
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].pkt_period_int=%d \n", hndl->var.ch_cfg_ptr[chnl].pkt_period_int);
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].pkt_period_n=%d \n", hndl->var.ch_cfg_ptr[chnl].pkt_period_n);
    PMC_LOG_TRACE("var.ch_cfg_ptr[chnl].pkt_period_d=%d \n", hndl->var.ch_cfg_ptr[chnl].pkt_period_d);
    
    *pkt_size_ptr = ps_ptr;
    *pkt_per_int_ptr = pp_int_ptr;
    *pkt_per_n_ptr = pp_n_ptr;
    *pkt_per_d_ptr = pp_d_ptr; 

    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_pkt_period_var_get */

/*******************************************************************************
*  mpmo_scbs3_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Return SCBS3 handle 
*
* INPUTS:
*   *hndl           - pointer to MPMO handle instance
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
PUBLIC scbs3_handle_t* mpmo_scbs3_handle_get(mpmo_handle_t *hndl)
{
    PMC_ENTRY();
    PMC_RETURN(hndl->scbs3_handle);    
} /*mpmo_scbs3_handle_get*/


/*******************************************************************************
*  mpmo_ch_pkt_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the packet size/cn_base/frm_period configuration
*
* INPUTS:
*   *hndl                  - pointer to MPMO handle instance
*   chnl                   - channel to operate on
*
* OUTPUTS:
*   *pkt_size_ptr         - pointer to configured packet size
*   *cn_base_int_ptr      - Cn,base Integer [23:0] \n
*                           (Cn,base = cn_base_int + cn_base_n/cn_base_d)
*   *cn_base_n_ptr        - Cn,base Numerator [31:0]
*   *cn_base_d_ptr        - Cn,base Denominator [31:0]
*   *cn_frm_per_n_ptr     - Numerator for the frame period
*   *cn_frm_per_d_ptr     - Denominator for the frame period
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mpmo_ch_pkt_cfg_get(mpmo_handle_t *hndl, 
                                     UINT32 chnl,
                                     UINT32 *pkt_size_ptr,
                                     UINT32 *cn_base_int_ptr, 
                                     UINT32 *cn_base_n_ptr, 
                                     UINT32 *cn_base_d_ptr,
                                     UINT32 *cn_frm_per_n_ptr, 
                                     UINT32 *cn_frm_per_d_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 frm_period_idx;

    PMC_ENTRY();
 
    PMC_ASSERT(hndl!=NULL, MPMO_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    frm_period_idx = mpmo_dsp_field_FRM_CNT_SEL_get(NULL, hndl, chnl);
    
    if (mpmo_dsp_field_FP_ENABLE_get(NULL, hndl, frm_period_idx) == FALSE)
    {
        rc = MPMO_LOG_ERR_FRM_PERIOD_NOT_FOUND;
    }  else
    {
        *cn_frm_per_n_ptr = mpmo_dsp_field_FRM_PERIOD_N_get(NULL, hndl, frm_period_idx);
        *cn_frm_per_d_ptr = mpmo_dsp_field_FRM_PERIOD_D_get(NULL, hndl, frm_period_idx);
        *cn_base_int_ptr = mpmo_dsp_field_CN_BASE_INT_get(NULL, hndl, chnl);
        *cn_base_n_ptr   = mpmo_dsp_field_CN_BASE_N_get(NULL, hndl, chnl);
        *cn_base_d_ptr   = mpmo_dsp_field_CN_BASE_D_get(NULL, hndl, chnl);    
        *pkt_size_ptr    = mpmo_field_PKT_SIZE_get(NULL, hndl, chnl);
    }

    PMC_RETURN(rc);
} /* mpmo_ch_pkt_cfg_get */


/*******************************************************************************
* mpmo_ch_enable_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get whether packet maker functionality is enabled or disabled
*
* INPUTS:
*   *hndl    - pointer to MPMO handle instance
*   chnl     - channel to get
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8    - TRUE: enable MPMO \n
*              FALSE: disable MPMO.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 mpmo_ch_enable_get(mpmo_handle_t *hndl, UINT32 chnl)
{
    BOOL8 enable;
    PMC_ENTRY();
    
    /* Enable/disable MPMO */
    enable = mpmo_field_CH_EN_get(NULL, hndl, chnl);

    PMC_RETURN(enable);

} /* mpmo_ch_enable_get */

/*
** Private Functions
*/
/*******************************************************************************
*  mpmo_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize MPMO var_t variables.
*
* INPUTS:
*   *hndl    - pointer to MPMO handle instance
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
PRIVATE void mpmo_var_default_init(mpmo_handle_t *hndl)
{
    UINT32 chnl;

    PMC_ENTRY();

    /* Configure default MPMO variable parameters for each channel */
    for (chnl = 0; chnl < hndl->cfg.default_param.num_chnl; chnl++) {
        hndl->var.ch_cfg_ptr[chnl].state            = MPMO_CHNL_START;
        hndl->var.ch_cfg_ptr[chnl].client_bitrate   = 0;
        hndl->var.ch_cfg_ptr[chnl].pkt_period_int   = 0;
        hndl->var.ch_cfg_ptr[chnl].pkt_period_n     = 0;
        hndl->var.ch_cfg_ptr[chnl].pkt_period_d     = 1;
        hndl->var.ch_cfg_ptr[chnl].pkt_size         = 0;

        hndl->var.ch_cfg_ptr[chnl].cn_base_int      = 0;
        hndl->var.ch_cfg_ptr[chnl].cn_base_n        = 0;
        hndl->var.ch_cfg_ptr[chnl].cn_base_d        = 0;
        hndl->var.ch_cfg_ptr[chnl].frm_period_idx   = 0;
        hndl->var.ch_cfg_ptr[chnl].frm_period_n     = 0;
        hndl->var.ch_cfg_ptr[chnl].frm_period_d     = 1;
        hndl->var.ch_cfg_ptr[chnl].lpf_gain         = 0;
        hndl->var.ch_cfg_ptr[chnl].zone_cent_en     = TRUE;
        hndl->var.ch_cfg_ptr[chnl].golden_cn_cal_en = FALSE;
        hndl->var.ch_cfg_ptr[chnl].cn_generate_en   = FALSE;

        hndl->var.ch_cfg_ptr[chnl].xoff_thres       = 0;
        hndl->var.ch_cfg_ptr[chnl].cfc_num_pages    = 0;
    }

    PMC_RETURN();
} /* mpmo_var_default_init */

/*******************************************************************************
* mpmo_cn_frame_period_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   For the specified Cn frame period, returns the index of where it is
*   configured in the hardware.
*   Returns the fractions for the frame period for the specified
*    Cn frame period.
*
* INPUTS:
*   *hndl            - pointer to MPMO handle instance
*   cn_frame_period  - Cn frame period.
*
* OUTPUTS:
*   frm_period_idx   - Index where the frame period is configured.
*   cn_frm_per_n     - Numerator for the frame period
*   cn_frm_per_d     - Denominator for the frame period
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mpmo_cn_frame_period_get(mpmo_handle_t *hndl,
                                           util_opsa_cn_frame_period_t cn_frame_period,
                                           UINT32 *frm_period_idx, UINT32 *cn_frm_per_n, UINT32 *cn_frm_per_d)
{
    UINT32 idx;
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    /* Find the specified Cn frame period. */
    for (idx = 0; idx < MPMO_NUM_FRM_PERIOD_CNT; idx++) {
        if (cn_frame_period == hndl->cfg.default_param.frm_period_cnt_ptr[idx].frm_period_type) {
            *cn_frm_per_n = hndl->cfg.default_param.frm_period_cnt_ptr[idx].frm_period_n;
            *cn_frm_per_d = hndl->cfg.default_param.frm_period_cnt_ptr[idx].frm_period_d;
            *frm_period_idx = idx;
            break;
        }
    }

    /* Check if the specified Cn frame period was found. */
    if (MPMO_NUM_FRM_PERIOD_CNT == idx) {
        *cn_frm_per_n = 0;
        *cn_frm_per_d = 1;
        result = MPMO_LOG_ERR_FRM_PERIOD_NOT_FOUND;
    }
    PMC_RETURN(result);
} /* mpmo_cn_frame_period_get */

/*******************************************************************************
* mpmo_ch_cn_frame_period_remove
* ______________________________________________________________________________
*
* DESCRIPTION:
*   For the specified Cn frame period, returns the index of where it is
*   configured in the hardware.
*   Returns the fractions for the frame period for the specified
*    Cn frame period.
*
* INPUTS:
*   *hndl    - pointer to MPMO handle instance
*   chnl     - channel to operate on
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
PRIVATE PMC_ERROR mpmo_ch_cn_frame_period_remove(mpmo_handle_t *hndl, UINT32 chnl)
{
    PMC_ERROR result = PMC_SUCCESS;
    util_opsa_cn_frame_period_t ch_frame_period;
    UINT32 idx, ch_frm_per_n, ch_frm_per_d;
    UINT32 i, active_chnl_cnt = 0;

    PMC_ENTRY();

    if (hndl->cfg.default_param.mpmo_frm_period_flex_alloc_method == TRUE)
    {
        idx = hndl->var.ch_cfg_ptr[chnl].frm_period_idx;
        
        /* found the idx of the frame period cnt */
        ch_frame_period = hndl->cfg.default_param.frm_period_cnt[idx].frm_period_type;
        ch_frm_per_n = hndl->cfg.default_param.frm_period_cnt[idx].frm_period_n;
        ch_frm_per_d = hndl->cfg.default_param.frm_period_cnt[idx].frm_period_d;
        
        /* check to see if there is any channel using the same frame period */
        /* ignore for the current channel as the channel context has not been
        ** updated for the  current channel yet
        */
        for (i = 0; i < hndl->cfg.default_param.num_chnl; i++)
        {
            if ((i != chnl) 
                && (hndl->var.ch_cfg_ptr[i].frm_period_n == ch_frm_per_n)
                && (hndl->var.ch_cfg_ptr[i].frm_period_d == ch_frm_per_d))
            {
                /* can't remove */
                active_chnl_cnt++;
            }
        }
        
        if (active_chnl_cnt == 0)
        {
            result = mpmo_ch_cn_frame_period_default_cfg(hndl, chnl, ch_frame_period);
        }
    }
    
    PMC_RETURN(result);

} /* mpmo_ch_cn_frame_period_remove */

/*******************************************************************************
*  mpmo_ch_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize channel with default parameters.
*
* INPUTS:
*   *hndl    - pointer to MPMO handle instance
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
PRIVATE PMC_ERROR mpmo_ch_init(mpmo_handle_t *hndl, UINT32 chnl)
{
    mpmo_dsp_buffer_t mpmo_dsp_buf[1];

    PMC_ENTRY();

    /* Configure MPMO core default parameters */
    mpmo_field_TS_ENABLE_set(NULL, hndl, chnl, hndl->cfg.default_param.ts_enable);

    /* Configure DSP if generating Cn. */
    if (hndl->var.ch_cfg_ptr[chnl].cn_generate_en) {
        mpmo_dsp_buffer_init(mpmo_dsp_buf, hndl);
        mpmo_dsp_field_CN_BIT_MODE_set(mpmo_dsp_buf, hndl, chnl, hndl->cfg.default_param.cn_bit_mode);

        mpmo_dsp_field_LPF_GAIN_set(mpmo_dsp_buf, hndl, chnl, hndl->var.ch_cfg_ptr[chnl].lpf_gain);
        mpmo_dsp_field_ZONE_CENT_ENABLE_set(mpmo_dsp_buf, hndl, chnl, hndl->var.ch_cfg_ptr[chnl].zone_cent_en);
        mpmo_dsp_buffer_flush(mpmo_dsp_buf);
    }

    if(mpmo_non_rev_a_instance_type_test(hndl) == TRUE && hndl->cfg.default_param.mpmo_type_id == MPMO_COREOTN_MUX_FRAMER)
    {
        mpmo_dsp_buffer_init(mpmo_dsp_buf, hndl);
        mpmo_dsp_field_FAST_LOCK_ENABLE_set(mpmo_dsp_buf, hndl, chnl, 0x1);
        mpmo_dsp_field_FAST_LOCK_THRESH_set(mpmo_dsp_buf, hndl, chnl, 0x7);
        mpmo_dsp_buffer_flush(mpmo_dsp_buf);
    }


    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_ch_init */

/*******************************************************************************
*  mpmo_ch_threshold_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure XOFF and DATA_AVAIL thresholds for the channel.
*
* INPUTS:
*   *hndl    - pointer to MPMO handle instance
*   chnl     - channel to configure
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
PUBLIC PMC_ERROR mpmo_ch_threshold_cfg(mpmo_handle_t *hndl, UINT32 chnl)
{
    mpmo_buffer_t mpmo_buf[1];
    PMC_ENTRY();

    /* Configure thresholds */
    mpmo_buffer_init(mpmo_buf, hndl);
    mpmo_field_XOFF_THRES_set(mpmo_buf, hndl, chnl, hndl->var.ch_cfg_ptr[chnl].xoff_thres);
    mpmo_buffer_flush(mpmo_buf);
    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_ch_threshold_cfg */

/*******************************************************************************
*  mpmo_ch_bw_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure bandwidth related parameters for the channel.
*
* INPUTS:
*   *hndl    - pointer to MPMO handle instance
*   chnl     - channel to configure
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
PUBLIC PMC_ERROR mpmo_ch_bw_cfg(mpmo_handle_t *hndl, UINT32 chnl)
{

    UINT32 cn_base_int_old;
    UINT32 cn_base_n_old;
    UINT32 cn_base_d_old;
    DOUBLE cn_base_new;
    UINT32 cn_base_int_new;
    DOUBLE cn_decimal;
    FLOAT f_cn_base_new;
    FLOAT f_cn_decimal;
    UINT32 cn_base_n_new;
    UINT32 cn_base_d_new;

    PMC_ENTRY();

    /* Configure MPMO core with packet size */
    mpmo_field_PKT_SIZE_set(NULL, hndl, chnl, hndl->var.ch_cfg_ptr[chnl].pkt_size);

    /* Configure DSP if generating Cn. */
    if (hndl->var.ch_cfg_ptr[chnl].cn_generate_en) {


        if(hndl->cfg.num_mult != 1 || hndl->cfg.denom_mult != 1)
        {

            cn_base_int_old = hndl->var.ch_cfg_ptr[chnl].cn_base_int;
            cn_base_n_old = hndl->var.ch_cfg_ptr[chnl].cn_base_n;
            cn_base_d_old = hndl->var.ch_cfg_ptr[chnl].cn_base_d;

            if (!digi_use_float32_get())
            {
                cn_base_new = (DOUBLE)cn_base_int_old + ((DOUBLE)cn_base_n_old/(DOUBLE)cn_base_d_old);
                cn_base_new = cn_base_new*((DOUBLE)hndl->cfg.num_mult/(DOUBLE)hndl->cfg.denom_mult);

                cn_base_int_new = (UINT32)cn_base_new;
                cn_decimal = cn_base_new-cn_base_int_new;
                util_opsa_find_rational_less_than_one(cn_decimal,UTIL_OPSA_CN_BASE_D_MAX, &cn_base_n_new, &cn_base_d_new);
            }
            else
            {
                f_cn_base_new = (FLOAT)cn_base_int_old + ((FLOAT)cn_base_n_old/(FLOAT)cn_base_d_old);
                f_cn_base_new = f_cn_base_new*((FLOAT)hndl->cfg.num_mult/(FLOAT)hndl->cfg.denom_mult);

                cn_base_int_new = (UINT32)f_cn_base_new;
                f_cn_decimal = f_cn_base_new-cn_base_int_new;
                util_opsa_find_rational_less_than_one((DOUBLE)f_cn_decimal,UTIL_OPSA_CN_BASE_D_MAX, &cn_base_n_new, &cn_base_d_new);
            }
        }
        else
        {
            cn_base_int_new = hndl->var.ch_cfg_ptr[chnl].cn_base_int;
            cn_base_n_new =  hndl->var.ch_cfg_ptr[chnl].cn_base_n;
            cn_base_d_new = hndl->var.ch_cfg_ptr[chnl].cn_base_d;
        }


        mpmo_dsp_field_CN_BASE_INT_set(NULL, hndl, chnl, cn_base_int_new);
        mpmo_dsp_field_CN_BASE_N_set(NULL, hndl, chnl, cn_base_n_new);
        mpmo_dsp_field_CN_BASE_D_set(NULL, hndl, chnl, cn_base_d_new);

        mpmo_dsp_field_FRM_CNT_SEL_set(NULL, hndl, chnl, hndl->var.ch_cfg_ptr[chnl].frm_period_idx);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_ch_bw_cfg */

/*******************************************************************************
* mpmo_ch_enable_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/disables the per-channel packet maker functionality.
*
* INPUTS:
*   *hndl    - pointer to MPMO handle instance
*   chnl     - channel to configure
*   enable   - TRUE: enable MPMO. FALSE: disable MPMO.
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
PUBLIC PMC_ERROR mpmo_ch_enable_cfg(mpmo_handle_t *hndl, UINT32 chnl, BOOL enable)
{
    PMC_ENTRY();

    /* Enable/disable MPMO */
    mpmo_field_CH_EN_set(NULL, hndl, chnl, enable);

    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_ch_enable_cfg */

/*******************************************************************************
* mpmo_dsp_ch_enable_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/disables the per-channel packet monitor DSP functionality.
*
* INPUTS:
*   *hndl    - pointer to MPMO handle instance
*   chnl     - channel to configure
*   enable   - TRUE: enable MPMO DSP. FALSE: disable MPMO DSP.
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
PRIVATE PMC_ERROR mpmo_dsp_ch_enable_cfg(mpmo_handle_t *hndl, UINT32 chnl, BOOL enable)
{
    PMC_ENTRY();

    /* Enable/disable MPMO DSP */
    mpmo_dsp_field_SLICE_ENABLE_set(NULL, hndl, chnl, enable);

    PMC_RETURN(PMC_SUCCESS);
} /* mpmo_dsp_ch_enable_cfg */


/*******************************************************************************
* mpmo_cfc_num_pages_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Calculates the number of pages to allocate in the CFC
*   based on the client bit rate.
*
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
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
PRIVATE UINT32 mpmo_cfc_num_pages_calc(mpmo_handle_t *hndl, DOUBLE client_bitrate)
{
    PMC_ENTRY();

    PMC_ASSERT(hndl->cfg.default_param.lowest_bw != 0, MPMO_LOG_ERR_DIVIDED_BY_ZERO, 0, 0);
    /* each MPMO instance has different bandwidth */
    PMC_RETURN((UINT32)ceil(client_bitrate/ (hndl->cfg.default_param.lowest_bw) ));

} /* mpmo_cfc_num_pages_calc */


/*******************************************************************************
* mpmo_xoff_thres_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Calculates the XOFF (back-pressure) threshold based on the
*   number of pages allocated in the CFC.
*
* INPUTS:
*   *hndl         - pointer to MPMO handle instance
*   cfc_num_pages - number of pages allocated in the CFC
*   bmp_map_mode  - indication if the MPMO is accociated with MAPOTN mapper 
*                   MPMO used for BMP mapping \n
*                   TRUE:  MAPOTN mapper MPMO used with BMP mapping \n
*                   FALSE: MAPOTN mapper MPMO not used with BMP mapping
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32        - Calculated XOFF threshold.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 mpmo_xoff_thres_calc(mpmo_handle_t *hndl, UINT32 cfc_num_pages,
                                   BOOL8 bmp_map_mode)
{
    UINT32 fifo_size;
    UINT32 xoff_thres;

    PMC_ENTRY();

    /* XOFF_THRES will be set at FIFO_SIZE/2+1. */
     fifo_size = cfc_num_pages * hndl->cfg.cfc_cfg.page_size;

    if(bmp_map_mode == TRUE)
    {
        /* 3/4 * (cal_num*4) + 1 - the caller of this 
           API must pass in number of calendar entries
           into cfc_num_pages parameter */
        xoff_thres = 3 * cfc_num_pages + 1;
    } else {
        xoff_thres = fifo_size/2 + 1;
    }




    PMC_RETURN(xoff_thres);

} /* mpmo_xoff_thres_calc */

/*******************************************************************************
*  mpmo_ch_ssf_cn_center_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/disables SSF Cn Center for the channel.
*
* INPUTS:
*   *hndl    - pointer to MPMO handle instance
*   chnl     - channel to configure
*   enable   - TRUE: enable the SSF Cn Center \n
*              FALSE: disable the SSF Cn Center
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
PRIVATE PMC_ERROR mpmo_ch_ssf_cn_center_cfg(mpmo_handle_t *hndl, UINT32 chnl, BOOL enable)
{
    PMC_ENTRY();

    /* enable/disable SSF CN CENTER */
    mpmo_field_SSF_CN_CENTER_EN_set(NULL, hndl, chnl, enable);

    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_ch_ssf_cn_center_cfg */




/*******************************************************************************
*  mpmo_alloc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*  
*  Function handles any allocation of non-context memory here and in any
*  underlying blocks.
*
* INPUTS:
*   *mpmo_handle        - pointer to MPMO handle instance
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
PUBLIC void mpmo_alloc_init(mpmo_handle_t *mpmo_handle)
{
    PMC_ENTRY();
    cfc_alloc_init(mpmo_handle->cfc_handle);
    scbs3_alloc_init(mpmo_handle->scbs3_handle);

    PMC_RETURN();
}

/*******************************************************************************
*  mpmo_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on MPMO handle instance.
*
* INPUTS:
*   *mpmo_handle                - pointer to MPMO handle instance
*   *restart_init_cfg_ptr       - pointer to restart init config
*   top_energy_state            - The expected energy state 
*   ptr_init_only               - TRUE: update the var, audit registers, etc. \n
*                                 FALSE: only call handle_restart_init on
*                                 children
*
* OUTPUTS:
*   mismatch_mpmo_channels      - Indicates a mismatch between the 
*                                 register space and context.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*  
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mpmo_handle_restart_init(mpmo_handle_t *mpmo_handle,
                                          util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                          pmc_energy_state_t top_energy_state,
                                          BOOL8 ptr_init_only,
                                          BOOL8 * mismatch_mpmo_channels)
{
    PMC_ERROR result = PMC_SUCCESS;
    mpmo_var_t * register_var = NULL;
    cfc_cfg_t   cfc_cfg;

    PMC_ENTRY();  
    /* Initialize CFC */
    cfc_cfg.max_fifo_num = mpmo_handle->cfg.cfc_cfg.max_fifo_num;
    cfc_cfg.page_size    = mpmo_handle->cfg.cfc_cfg.page_size;
    cfc_handle_restart_init(mpmo_handle->cfc_handle, &cfc_cfg, restart_init_cfg_ptr, top_energy_state);

    /* allow using the fixed array of Cn frame periods for MAPOTN */
    if ((mpmo_handle->cfg.default_param.mpmo_frm_period_flex_alloc_method) == FALSE)
    {
            mpmo_handle->cfg.default_param.frm_period_cnt_ptr = mpmo_frm_period_cnt;
    }

    if (TRUE == mpmo_handle->cfg.scbs3_cfg.scbs3_present) 
    {
        result = scbs3_handle_restart_init(mpmo_handle->scbs3_handle,restart_init_cfg_ptr, PMC_ENERGY_STATE_RUNNING, TRUE);
    }
    
    if (PMC_SUCCESS == result && FALSE == ptr_init_only)
    {
        register_var = (mpmo_var_t*)PMC_CALLOC(sizeof(mpmo_var_t));
        register_var->ch_cfg_ptr = (mpmo_ch_cfg_struct_t*)PMC_CALLOC(sizeof(mpmo_ch_cfg_struct_t)*80);   
        
        result = mpmo_audit_var_data_register_get(mpmo_handle, 
                                                  register_var);
                                              
        result = mpmo_audit_ctxt_register_compare_clean(mpmo_handle,
                                                        &(mpmo_handle->var),
                                                        register_var,
                                                        restart_init_cfg_ptr,
                                                        mismatch_mpmo_channels);

        PMC_FREE(&(register_var->ch_cfg_ptr));
        PMC_FREE(&(register_var));
    }

    PMC_RETURN(result);
} /* mpmo_handle_restart_init */

/*******************************************************************************
* mpmo_debug_data_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns the date from debug registers for a specified channel.\n\n
*
*   The channel must be activated for the data request to be successful.
*   
*
* INPUTS:
*   *mpmo_handle          - pointer to MPMO handle instance
*   chnl                  - Channel number to operate on
*
* OUTPUTS:
*   *debug_pkt_age_ptr    - Pointer to the recovered packet age for the channel.
*   *debug_ramp_state_ptr - Pointer to rame state for the channel
*   *debug_curr_csi_ptr   - Pointer to current CSI for the channel
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mpmo_debug_data_get(mpmo_handle_t *mpmo_handle,
                                    UINT32 chnl,
                                    UINT32 *debug_pkt_age_ptr,
                                    UINT32 *debug_ramp_state_ptr,
                                    UINT32 *debug_curr_csi_ptr)
{

    UINT32 num_failed_polls = 0;
    PMC_POLL_RETURN_TYPE poll_result = PMC_SUCCESS;
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();


    PMC_ASSERT(mpmo_handle!=NULL, MPMO_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    /* test to confirm channel in operation state or return error */
    if(mpmo_handle->var.ch_cfg_ptr[chnl].state != MPMO_CHNL_OPERATIONAL)
        result = MPMO_LOG_CODE_INVALID_CHANNEL_STATE;

    if(mpmo_handle->cfg.default_param.num_chnl < chnl && PMC_SUCCESS==result)
        result = MPMO_LOG_ERR_INVALID_CHNL;

    /* confirm debug mechanism is available, if not clean up */
    if(PMC_SUCCESS == result && 0 == mpmo_field_DEBUG_DATA_REQ_get(NULL, mpmo_handle))
    {
            /* abort if debug access not in valid starting state */
            mpmo_field_DEBUG_DATA_REQ_ABORT_set(NULL, mpmo_handle, 0);
            mpmo_field_DEBUG_DATA_REQ_ABORT_set(NULL, mpmo_handle, 1);
            poll_result = mpmo_field_DEBUG_DATA_REQ_ABORT_poll(NULL, mpmo_handle,
                                                               0, PMC_POLL_EQUAL, 125,
                                                               &num_failed_polls, 1 );
            PMC_ASSERT(poll_result==PMC_SUCCESS, MPMO_LOG_ERR_ASSERT, 0, 0);
            /* confirm expected starting state okay after recovery else assert */
            if(0 != mpmo_field_DEBUG_DATA_REQ_get(NULL, mpmo_handle))
                PMC_ASSERT(FALSE, MPMO_LOG_ERR_ASSERT, 0, 0);
    }

    /* poll to confirm the debug access is successful */
    if(PMC_SUCCESS == result)
    {
        /* set address for debug query */
        mpmo_field_DEBUG_DATA_REQ_ID_set(NULL, mpmo_handle, chnl);
    
        /* request transfer of PKT_AGE data */
        mpmo_field_DEBUG_DATA_REQ_set(NULL, mpmo_handle, 1);
    }

    /* poll to confirm the debug access is successful */
    if(PMC_SUCCESS == result)
    {
        poll_result = mpmo_field_DEBUG_DATA_REQ_poll(NULL, mpmo_handle,
                                                     0, PMC_POLL_EQUAL, 125,
                                                     &num_failed_polls, 1 );

        if(poll_result==PMC_SHARED_ERR_CODE_POLL_TIMEOUT)
        {
            PMC_LOG(PMC_LOG_SEV_HIGHEST, PMC_SHARED_ERR_CODE_POLL_TIMEOUT, 0, 0);
            PMC_LOG_TRACE("PMC_SHARED_ERR_CODE_POLL_TIMEOUT");
            /* abort if debug access success timed out on polling */
            mpmo_field_DEBUG_DATA_REQ_ABORT_set(NULL, mpmo_handle, 0);
            mpmo_field_DEBUG_DATA_REQ_ABORT_set(NULL, mpmo_handle, 1);
            poll_result = mpmo_field_DEBUG_DATA_REQ_ABORT_poll(NULL, mpmo_handle,
                                                               0, PMC_POLL_EQUAL, 125,
                                                               &num_failed_polls, 1 );
            PMC_ASSERT(poll_result==PMC_SUCCESS, MPMO_LOG_ERR_ASSERT, 0, 0);
            result = MPMO_LOG_ERR_DEBUG_ACCESS_FAIL;
        }
    }

    /* poll to confirm the debug access is successful */
    if(PMC_SUCCESS == result)
    {
        *debug_pkt_age_ptr    = mpmo_field_PKT_AGE_get(NULL, mpmo_handle);  
        *debug_ramp_state_ptr = mpmo_field_RAMP_STATE_get(NULL, mpmo_handle);  
        *debug_curr_csi_ptr   = mpmo_field_CURR_CSI_get(NULL, mpmo_handle);  
    }

    PMC_RETURN(result);
} /* mpmo_debug_data_get */

/*******************************************************************************
* mpmo_raw_latency_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns the PKT_AGE using a raw retieval mechanism.  If the channel handle is 
*   invalid for this feature an error will be returned.\n\n
*
*   The raw retrieval modifies the MPMO configuration so that MPMO has no
*   influence on the latency measurment.\n\n
*
*   The raw retrieval mechanism is destructive to the flow of traffic on all
*   channel.  This API should be used for development purposes only.  It
*   should not be used for operational paths in fielded systems.
*   
*
* INPUTS:
*   *mpmo_handle       - pointer to MPMO handle instance
*   chnl               - Channel number to operate on
*
* OUTPUTS:
*   *minimum_latency  - Pointer to the recovered packet age for the channel.
*                       Units are equal to 3.125 nseconds each
*   *maximum_latency  - Pointer to the recovered packet age for the channel.
*                       Units are equal to 3.125 nseconds each
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mpmo_raw_latency_get(mpmo_handle_t *mpmo_handle,
                                      UINT32 chnl,
                                      UINT32 *minimum_latency,
                                      UINT32 *maximum_latency)
{
    UINT32 pacer_mode, pkt_size;
    UINT32 pkt_age, dummy, i;
    PMC_ERROR result = PMC_SUCCESS;
    

    PMC_ENTRY();

    PMC_ASSERT(mpmo_handle!=NULL, MPMO_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    /* test to confirm channel in operation state or return error */
    if(mpmo_handle->var.ch_cfg_ptr[chnl].state != MPMO_CHNL_OPERATIONAL)
        result = MPMO_LOG_CODE_INVALID_CHANNEL_STATE;

    if(mpmo_handle->cfg.default_param.num_chnl < chnl && PMC_SUCCESS==result)
        result = MPMO_LOG_ERR_INVALID_CHNL;


    if(PMC_SUCCESS == result)
    {
        mpmo_field_CH_EN_set(NULL, mpmo_handle, chnl, 0);
    
        pacer_mode = mpmo_field_PACER_MODE_get(NULL, mpmo_handle);
        mpmo_field_PACER_MODE_set(NULL, mpmo_handle, 0);
    
        pkt_size = mpmo_field_PKT_SIZE_get(NULL, mpmo_handle, chnl);
        mpmo_field_PKT_SIZE_set(NULL, mpmo_handle, chnl, (pkt_size-1));
    
        mpmo_field_CH_EN_set(NULL, mpmo_handle, chnl, 1);
    
        *minimum_latency = 0xFFFFFFFF;
        *maximum_latency = 0x0;
    
        for(i=0; (i < 200) && (result == PMC_SUCCESS); i++)
        {
            result = mpmo_debug_data_get(mpmo_handle,
                                         chnl,
                                         &pkt_age,
                                         &dummy,
                                         &dummy);
            PMC_ATOMIC_YIELD(mpmo_handle->base.parent_handle->parent_handle, 0);                             
            *minimum_latency = pkt_age < *minimum_latency ? pkt_age:*minimum_latency;
            *maximum_latency = pkt_age > *maximum_latency ? pkt_age:*maximum_latency;
        }
    
        /* revert setting modifications */
        mpmo_field_CH_EN_set(NULL, mpmo_handle, chnl, 0);
        mpmo_field_PACER_MODE_set(NULL, mpmo_handle, pacer_mode);
        mpmo_field_PKT_SIZE_set(NULL, mpmo_handle, chnl, pkt_size);
        mpmo_field_CH_EN_set(NULL, mpmo_handle, chnl, 1);
    }

    PMC_RETURN(result);
} /* mpmo_raw_latency_get */

/*******************************************************************************
*  mpmo_audit_var_data_register_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Generates mpmo_var_t structure data from reading device registe space.
*
*   Not all fields are updated.  
* 
*
* INPUTS:
*   *mpmo_handle           - pointer to MPMO handle instance
*
*
* OUTPUTS:
*   *var_ptr               - pointer to mpmo_var_t type structure with 
*                            subelements defined
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*  
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mpmo_audit_var_data_register_get(mpmo_handle_t *mpmo_handle, 
                                                   mpmo_var_t *var_ptr)
{

    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i, ch_en, pkt_size, cn_base_int, cn_base_n, cn_base_d,
           cn_frm_per_n, cn_frm_per_d, cfc_num_pages;
    PMC_ENTRY();
   
    for(i=0;i<80;i++)
    {
        ch_en = mpmo_ch_enable_get(mpmo_handle, i);
        if(ch_en == 1)
            var_ptr->ch_cfg_ptr[i].state = MPMO_CHNL_OPERATIONAL;
        else
            var_ptr->ch_cfg_ptr[i].state = MPMO_CHNL_START;



        mpmo_ch_pkt_cfg_get(mpmo_handle, 
                            i,
                            &pkt_size,
                            &cn_base_int, 
                            &cn_base_n, 
                            &cn_base_d,
                            &cn_frm_per_n, 
                            &cn_frm_per_d);

        var_ptr->ch_cfg_ptr[i].pkt_size = pkt_size;
        var_ptr->ch_cfg_ptr[i].cn_base_int = cn_base_int;
        var_ptr->ch_cfg_ptr[i].cn_base_n = cn_base_n;
        var_ptr->ch_cfg_ptr[i].cn_base_d = cn_base_d;
        var_ptr->ch_cfg_ptr[i].frm_period_n = cn_frm_per_n;
        var_ptr->ch_cfg_ptr[i].frm_period_d = cn_frm_per_d;

        cfc_fifo_current_num_pages_get(mpmo_handle->cfc_handle, 
                                       i, 
                                       &cfc_num_pages);

        var_ptr->ch_cfg_ptr[i].cfc_num_pages = cfc_num_pages;

        

    }

    PMC_RETURN(result);
} /* mpmo_audit_var_data_register_get */

/*******************************************************************************
*  mpmo_audit_ctxt_register_compare_clean
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
*   *mpmo_handle            - pointer to MPMO handle instance
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
PRIVATE PMC_ERROR mpmo_audit_ctxt_register_compare_clean(mpmo_handle_t *mpmo_handle,
                                                         mpmo_var_t *ctxt_var_ptr,
                                                         mpmo_var_t *reg_var_ptr,
                                                         util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                                         BOOL8 * mismatch_channels)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    UINT32 ch_array[1] = {0xFF};
    *mismatch_channels = FALSE;

    PMC_ENTRY();

    /* For MPMO, checking these 4 parameters is sufficient */
   
    for(i=0;i<80;i++)
    {
        if(ctxt_var_ptr->ch_cfg_ptr[i].state != reg_var_ptr->ch_cfg_ptr[i].state)
        {
            *mismatch_channels = TRUE;
            if(!restart_init_cfg_ptr->is_warm_restart)
            {
                ch_array[0] = i;
                PMC_LOG_TRACE("mpmo_audit_ctxt_register_compare_clean - Calling mpmo_ch_clean on channel %u\n",i);
                mpmo_ch_clean(mpmo_handle,ch_array,1);
            }
        }
        else if( (ctxt_var_ptr->ch_cfg_ptr[i].state != MPMO_CHNL_START) &&
                    (ctxt_var_ptr->ch_cfg_ptr[i].pkt_size != reg_var_ptr->ch_cfg_ptr[i].pkt_size ||
                    ctxt_var_ptr->ch_cfg_ptr[i].cn_base_int != reg_var_ptr->ch_cfg_ptr[i].cn_base_int ||
                    ctxt_var_ptr->ch_cfg_ptr[i].cn_base_n != reg_var_ptr->ch_cfg_ptr[i].cn_base_n ||
                    ctxt_var_ptr->ch_cfg_ptr[i].cn_base_d != reg_var_ptr->ch_cfg_ptr[i].cn_base_d ||
                    ctxt_var_ptr->ch_cfg_ptr[i].frm_period_n != reg_var_ptr->ch_cfg_ptr[i].frm_period_n ||
                    ctxt_var_ptr->ch_cfg_ptr[i].frm_period_d != reg_var_ptr->ch_cfg_ptr[i].frm_period_d))


        {
            *mismatch_channels = TRUE;
            if(!restart_init_cfg_ptr->is_warm_restart)
            {
                ch_array[0] = i;
                PMC_LOG_TRACE("mpmo_audit_ctxt_register_compare_clean - Calling mpmo_ch_clean on channel %u\n",i);
                mpmo_ch_clean(mpmo_handle,ch_array,1);
            }
        }

    }

    PMC_RETURN(result);
} /* mpmo_audit_ctxt_register_compare_clean */

/*******************************************************************************
*  mpmo_non_rev_a_instance_type_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function determines the MPMO block revision type.
*
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8          - TRUE:  MPMO instance is not Rev A type \n
*                    FALSE: MPMO instance is Rev A type
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 mpmo_non_rev_a_instance_type_test(mpmo_handle_t *hndl)
{

    PMC_ENTRY();

    if(FALSE == pmc_is_digi_rev_a_revision(&hndl->base))
    {
        /* Non Rev A device use non Rev A MPMO for FO1, FO2,  */
        if(hndl->cfg.default_param.mpmo_type_id == MPMO_COREOTN_MUX_FRAMER ||
           hndl->cfg.default_param.mpmo_type_id == MPMO_COREOTN_TO_MAPOTN)
            PMC_RETURN(TRUE);
    }        


    PMC_RETURN(FALSE);

} /* mpmo_non_rev_a_instance_type_test */

/*******************************************************************************
*  mpmo_ghao_age_gain_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set AGE_GAIN_PROP and AGE_GAIN_INT.  These are require parameters
*   only when the MPMO is operating in "GMP special mode"
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   chnl              - channel to operate on
*   num_entries       - The number of calendar slots the channel
*                       occupies
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
PUBLIC PMC_ERROR mpmo_ghao_age_gain_set(mpmo_handle_t *hndl, 
                                        UINT32 chnl,
                                        UINT32 num_entries)
{
    mpmo_dsp_buffer_t mpmo_dsp_buf[1];

    UINT32 array_idx;

    UINT32 age_prop_gain_coreotn_array[80] = MPMO_GHAO_COREOTN_AGE_PROP_GAIN;
    UINT32 age_int_gain_coreotn_array[80] = MPMO_GHAO_COREOTN_AGE_INT_GAIN;





    UINT32 age_prop_gain = 0;
    UINT32 age_int_gain = 0;
    
    PMC_ENTRY();
    
    array_idx = num_entries -1;


    /* Update the age gain parameters based on the MPMO insatance */
    
    if(hndl->cfg.default_param.mpmo_type_id == MPMO_COREOTN_MUX_FRAMER)
    {
      
        age_prop_gain = age_prop_gain_coreotn_array[array_idx];
        age_int_gain = age_int_gain_coreotn_array[array_idx];
    }

    
    mpmo_dsp_buffer_init(mpmo_dsp_buf, hndl);
    mpmo_dsp_field_AGE_PROP_GAIN_set(mpmo_dsp_buf, hndl, chnl, age_prop_gain);
    mpmo_dsp_field_AGE_INT_GAIN_set(mpmo_dsp_buf, hndl, chnl, age_int_gain);
    
    mpmo_dsp_field_AGE_GAIN_UPDATE_set(mpmo_dsp_buf, hndl, chnl, 0);

    mpmo_dsp_buffer_flush(mpmo_dsp_buf);

    /*toggle the AGE GAIN UPDATE bit for the change to occur */
    mpmo_dsp_field_AGE_GAIN_UPDATE_set(NULL, hndl, chnl, 1);


    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_ghao_age_gain_set */


/*******************************************************************************
*  mpmo_ghao_fifo_resize
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resize the CFC fifo to the specified number of tribslots. Update the XOFF
*   threshold accordingly.
*
* INPUTS:
*   *hndl                 - pointer to MPMO handle instance
*   chnl                  - channel to resize
*   oduflex_num_tribslots - Final number of tribslots after G.HAO
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
PUBLIC PMC_ERROR mpmo_ghao_fifo_resize(mpmo_handle_t *hndl,
                                       UINT32 chnl,
                                       UINT32 oduflex_num_tribslots)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    PMC_ENTRY();
    
    /* Save the number of pages for CFC */
    hndl->var.ch_cfg_ptr[chnl].cfc_num_pages = oduflex_num_tribslots;

    /* Resize CFC FIFO */
    ret_val = cfc_fifo_resize(hndl->cfc_handle,
                              chnl,
                              hndl->var.ch_cfg_ptr[chnl].cfc_num_pages);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    /* Update XOFF_THRES (back-pressure) */
    hndl->var.ch_cfg_ptr[chnl].xoff_thres = mpmo_xoff_thres_calc(hndl, hndl->var.ch_cfg_ptr[chnl].cfc_num_pages, FALSE);
    mpmo_ch_threshold_cfg(hndl, chnl);

    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_ghao_fifo_resize */


/*******************************************************************************
*  mpmo_ghao_fifo_resize_no_thresh_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resize the CFC fifo to the specified number of tribslots. The XOFF
*   threshold is not modified. The threshold is expected to be updated in
*   BWR phase using mpmo_ghao_threshold_update() and the information given by
*   mpmo_ghao_threshold_update_setup().
*
* INPUTS:
*   *hndl                 - pointer to MPMO handle instance
*   chnl                  - channel to resize
*   oduflex_num_tribslots - Final number of tribslots after G.HAO
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
PUBLIC PMC_ERROR mpmo_ghao_fifo_resize_no_thresh_update(mpmo_handle_t *hndl,
                                                        UINT32 chnl,
                                                        UINT32 oduflex_num_tribslots)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    PMC_ENTRY();
    
    /* Save the number of pages for CFC */
    hndl->var.ch_cfg_ptr[chnl].cfc_num_pages = oduflex_num_tribslots;

    /* Resize CFC FIFO */
    ret_val = cfc_fifo_resize(hndl->cfc_handle,
                              chnl,
                              hndl->var.ch_cfg_ptr[chnl].cfc_num_pages);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_ghao_fifo_resize_no_thresh_update */


/*******************************************************************************
*  mpmo_schd_ch_reprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function for the scheduler only. 
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   chnl              - channel to reprovision
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
PUBLIC PMC_ERROR mpmo_schd_ch_reprov(mpmo_handle_t *hndl, UINT32 chnl, 
                                     DOUBLE client_bitrate)
{
    UINT32 scbs3_rate;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* Deprovision SCBS3 */
    if (hndl->cfg.scbs3_cfg.scbs3_present) {
        scbs3_rate = (UINT32)(client_bitrate / 100);
        ret_val = scbs3_chnl_reprov(hndl->scbs3_handle, chnl, scbs3_rate, UTIL_SCHD_100_BIT_SEC);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        scbs3_page_udpate_and_swap(hndl->scbs3_handle);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_schd_ch_reprov */



/*******************************************************************************
*  mpmo_bwr_ind_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Function used to control the receipt of BWR_IND in the MPMO.
*
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
*
* OUTPUTS:
*   bwr_ind        - UINT32[3] array for BWR_IND_I (bitmask of 96)
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mpmo_bwr_ind_get(mpmo_handle_t *hndl, 
                                  UINT32 bwr_ind[3])
{

    

    PMC_ENTRY();

    mpmo_lfield_BWR_RAMP_I_get(NULL, hndl, bwr_ind);


    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_bwr_ind_get */



/*******************************************************************************
*  mpmo_xoff_thresh_change
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Increment or decrement the XOFF threshold by 1 depending on increase flag.
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   chnl              - Channel id to perform the increase/decrease on
*   increase          - TRUE : increse the xoff threshold by 1 word \n
*                       FALSE : decrease the xoff threshold by 1 word
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
PUBLIC PMC_ERROR mpmo_xoff_thresh_change(mpmo_handle_t *hndl, 
                                         UINT32 chnl,
                                         BOOL8 increase)
{
    PMC_ENTRY();

              
    if(increase)
    {
        hndl->var.ch_cfg_ptr[chnl].xoff_thres++;
    }
    else
    {
        hndl->var.ch_cfg_ptr[chnl].xoff_thres--;
    }

    PMC_LOG_TRACE("Call mpmo_xoff_thresh_change(): chnl_id=%d, xoff_thres=%d\n", chnl, hndl->var.ch_cfg_ptr[chnl].xoff_thres);

    mpmo_field_XOFF_THRES_set(NULL, hndl, chnl, hndl->var.ch_cfg_ptr[chnl].xoff_thres);


    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_xoff_thresh_change */



/*******************************************************************************
*  mpmo_xoff_thresh_change_delta
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Increment or decrement the XOFF threshold by the given delta.
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   chnl              - Channel id to perform the increase/decrease on
*   threshold_delta   - Value added to current XOFF threshold which; delta can
*                       be positive or negative.
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
PUBLIC PMC_ERROR mpmo_xoff_thresh_change_delta(mpmo_handle_t *hndl, 
                                               UINT32 chnl,
                                               INT32 threshold_delta)
{
    PMC_ENTRY();

    if ((INT32)hndl->var.ch_cfg_ptr[chnl].xoff_thres + threshold_delta < 0)
    {
        PMC_RETURN(MPMO_LOG_ERR_INVALID_PARAMETERS);
    }

    hndl->var.ch_cfg_ptr[chnl].xoff_thres += threshold_delta;

    PMC_LOG_TRACE("Call mpmo_xoff_thresh_change_delta(): chnl_id=%d, threshold_delta=%d, xoff_thres=%d\n", chnl, threshold_delta, hndl->var.ch_cfg_ptr[chnl].xoff_thres);

    mpmo_field_XOFF_THRES_set(NULL, hndl, chnl, hndl->var.ch_cfg_ptr[chnl].xoff_thres);


    PMC_RETURN(PMC_SUCCESS);

} /* mpmo_xoff_thresh_change_delta */



/*******************************************************************************
*  mpmo_set_custom_userbit_mask
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Wrapper function to set a custom userbit mask  
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   chnl_id           - Channel id to apply the mask to
*   enable            - TRUE : use the userbit_mask to set the userbits
*                              in the MPMO 
*   set_mask          - Mask to identify the bits that can be changed
*   userbit_mask      - The desired userbit_mask (indexed by calendar
*                       entry.  userbit_mask[0][0th bit] = cal entry 0.
*                       userbit_mask[0][31st bit] = cal entry 95.
*   config_update     - TRUE : will update the standby and active
*                              page with the userbits
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
PUBLIC PMC_ERROR mpmo_set_custom_userbit_mask(mpmo_handle_t *hndl,
                                              UINT32 chnl_id,
                                              BOOL enable,
                                              UINT32 set_mask[3],
                                              UINT32 userbit_mask[3],
                                              BOOL8 config_update)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;


    PMC_ENTRY();


    ret_val = scbs3_set_custom_userbit_mask(hndl->scbs3_handle,
                                            chnl_id,
                                            enable,
                                            set_mask,
                                            userbit_mask);
                                            


    /*this will write the new userbit information to the online
    and offline page*/
    if(config_update == TRUE)
    {
        ret_val = scbs3_page_udpate_and_swap(hndl->scbs3_handle);
    }

    PMC_RETURN(ret_val);

} /* mpmo_set_custom_userbit_mask */



/*******************************************************************************
*  mpmo_ghao_bwr_setup_param_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Wrapper function to set parameters before the ramp:
*      - CN_TARGET \n
*      - RAMP_UP_DWN \n
*      - RAMP_EN \n
*      - HOLDOVER \n
*   This places the MPMO into GMP special mode
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   chnl              - Channel id to operate on
*   target_ts         - the number of tribslots at the end of the ramp
*   increase          - TRUE : the rate is ramping up \
*                       FALSE : rate is ramping down
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
PUBLIC PMC_ERROR mpmo_ghao_bwr_setup_param_set(mpmo_handle_t *hndl,
                                               UINT32 chnl,
                                               UINT32 target_ts,
                                               BOOL8 increase)
{
    UINT32 frm_period_idx;
    DOUBLE mf_period_sec = 0;
    DOUBLE cn_target;
    UINT32 cn_target_int;
    UINT32 n;
    DOUBLE target_rate;

    DOUBLE bwr_step;
    int bwr_step_int; /*must have integer type here!*/
    util_opsa_cn_frame_period_t frm_period_type;

    PMC_ERROR ret_val = PMC_SUCCESS;


    PMC_ENTRY();

    frm_period_idx = hndl->var.ch_cfg_ptr[chnl].frm_period_idx;


    if ((hndl->cfg.default_param.mpmo_frm_period_flex_alloc_method) == FALSE)
    {
        frm_period_type = hndl->cfg.default_param.frm_period_cnt_ptr[frm_period_idx].frm_period_type;
    }
    else
    {
        frm_period_type = hndl->cfg.default_param.frm_period_cnt[frm_period_idx].frm_period_type;
    }


    if(hndl->var.ch_cfg_ptr[chnl].cn_generate_en)
    {

        /*look up the frame period of the server using the context */
        switch (frm_period_type) 
        {

            case UTIL_OPSA_COREOTN_ODU1_1G25_T_FRAME:
                mf_period_sec = COREOTN_ODU1_1G25_T_FRAME;
                n = 8;
                break;
            case UTIL_OPSA_COREOTN_ODU2_1G25_T_FRAME:
                mf_period_sec = COREOTN_ODU2_1G25_T_FRAME;
                n = 8;
                break;
            case UTIL_OPSA_COREOTN_ODU3_1G25_T_FRAME:
                mf_period_sec = COREOTN_ODU3_1G25_T_FRAME;
                n = 8;
                break;
            case UTIL_OPSA_COREOTN_ODU3E2_1G25_T_FRAME:
                mf_period_sec = COREOTN_ODU3E2_1G25_T_FRAME;
                n = 8;
                break;
            case UTIL_OPSA_COREOTN_ODU4_1G25_T_FRAME:
                mf_period_sec = COREOTN_ODU4_1G25_T_FRAME;
                n = 8;
                break;

            default:
                PMC_RETURN(MPMO_LOG_ERR_INVALID_PARAMETERS);
                break;
        }

        /* calculate target Cn */

        if (target_ts <= 8)
        {
            target_rate = (target_ts * UTIL_GLOBAL_ODU2_TS_RATE);
        } 
        else if (target_ts <= 32)
        {
            target_rate = (target_ts * UTIL_GLOBAL_ODU3_TS_RATE);
        } 
        else 
        {
            target_rate = (target_ts * UTIL_GLOBAL_ODU4_TS_RATE);
        }


        cn_target = (target_rate/8)*mf_period_sec;

        
        if(increase)
        {
            /*round down cn_target for increase*/
            cn_target_int = (UINT32)(cn_target);
        }
        else
        {
            /*round up cn_target for decrease*/
            cn_target_int = (UINT32)(cn_target+0.5);
        }

        if (TRUE == pmc_mpmo_mpma_reg_revision(&hndl->base))
        {
            mpmo_field_CN_TARGET_set(NULL, hndl, chnl, cn_target_int);
        }
        else
        {
            mpmo_field_CN_TARGET2_set(NULL, hndl, chnl, cn_target_int);
        }

        /* Now calculate BWR_IND step 
        need to use n because some interface requires this in bits some in bytes */
        bwr_step = (512000000/n)*(pow(mf_period_sec,2))*(pow(2,16));

        bwr_step_int = (UINT32)(bwr_step+0.5);
        
        if(increase == FALSE)
        {
            /*perform 2s complement for decrease*/
            bwr_step_int = -bwr_step_int;
        }

        mpmo_dsp_field_BWR_STEP_set(NULL, hndl, chnl, (bwr_step_int&0xFFFFF));
    }

    /* set ramp_up_down */
    if (TRUE == pmc_mpmo_mpma_reg_revision(&hndl->base))
    {
        mpmo_field_RAMP_UP_DWN_set(NULL, hndl, chnl, increase);
    }
    else
    {
        mpmo_field_RAMP_UP_DWN2_set(NULL, hndl, chnl, increase);
    }


    /* set RAMP_EN */
    mpmo_field_RAMP_EN_set(NULL, hndl, chnl, TRUE);


    /* set HOLDOVER bit to put into GMP special mode*/
    mpmo_dsp_field_HOLDOVER_set(NULL, hndl, chnl, TRUE);

    PMC_RETURN(ret_val);

} /* mpmo_ghao_bwr_rate_param_set */

/*******************************************************************************
*  mpmo_ghao_bwr_end_param_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Function that sets the BWR END parameters
*
* INPUTS: 
*   *hndl                  - pointer to MPMO handle instance
*   chnl                   - Channel id to operate on 
*   client_bitrate         - client rate in bits per second
*   pkt_size               - packet size returned from OIF calculator
*   pkt_per_int            - integer packets per period from OIF calculator
*   pkt_per_n              - Decimal (numerator) pkt per period
*   pkt_per_d              - Decimal (denomenator) pkt per period
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
PUBLIC PMC_ERROR mpmo_ghao_bwr_end_param_set(mpmo_handle_t *hndl,
                                             UINT32 chnl,
                                             DOUBLE client_bitrate,
                                             UINT32 pkt_size,
                                             UINT32 pkt_per_int,
                                             UINT32 pkt_per_n,
                                             UINT32 pkt_per_d)
{

    UINT32 *pp_int_ptr   = &(hndl->var.ch_cfg_ptr[chnl].pkt_period_int);
    UINT32 *pp_n_ptr     = &(hndl->var.ch_cfg_ptr[chnl].pkt_period_n);
    UINT32 *pp_d_ptr     = &(hndl->var.ch_cfg_ptr[chnl].pkt_period_d);
    UINT32 *ps_ptr       = &(hndl->var.ch_cfg_ptr[chnl].pkt_size);

    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* Check for valid packet size range. */
    if ((pkt_size < MPMO_PKT_SIZE_MIN) || (pkt_size > MPMO_PKT_SIZE_MAX)) {
        PMC_RETURN(MPMO_LOG_ERR_INVALID_PARAMETERS);
    }

    /* Record the channel information */
    hndl->var.ch_cfg_ptr[chnl].client_bitrate = client_bitrate;

    /* Record the specified packet size. */
    *ps_ptr = pkt_size;

    /* Record the specified packet period. */
    *pp_int_ptr   = pkt_per_int;
    *pp_n_ptr     = pkt_per_n;
    *pp_d_ptr     = pkt_per_d;


    /* new write pkt_size */

    mpmo_field_PKT_SIZE_set(NULL, hndl, chnl, hndl->var.ch_cfg_ptr[chnl].pkt_size);


    /* set RAMP_EN */

    mpmo_field_RAMP_EN_set(NULL, hndl, chnl, FALSE);


    /* wait 200 us before taking out of HOLDOVER mode */
    PMC_Q_USLEEP(hndl,200);
    


    /* set HOLDOVER bit to put into GMP special mode*/

    mpmo_dsp_field_HOLDOVER_set(NULL, hndl, chnl, FALSE);




    PMC_RETURN(ret_val);

} /* mpmo_ghao_bwr_rate_param_set */

/*******************************************************************************
*  mpmo_ghao_calculate_new_cn_max_cn_d
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Calculates a new Cn base for the MPMO that maximizes the denominator 
*
* INPUTS: 
*   *hndl                  - pointer to MPMO handle instance
*   chnl                   - Channel id to operate on
*   client_bitrate         - client rate in bits per second
*   pkt_size               - packet size returned from OIF calculator
*   pkt_per_int            - integer packets per period from OIF calculator
*   pkt_per_n              - Decimal (numerator) pkt per period
*   pkt_per_d              - Decimal (denomenator) pkt per period
*
* OUTPUTS:
*   *cn_base_int           - Integer of Cn base
*   *cn_base_n             - Decimal (numerator) of cn base
*   *cn_base_d             - Decimal (denomenator) of cn base
*   *program_cn_n_first    - TRUE : program new numerator before
*                                   new denominator \n
*                            FALSE : program new denominator
*                                    before new numerator
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mpmo_ghao_calculate_new_cn_max_cn_d(mpmo_handle_t *hndl,
                                                     UINT32 chnl,
                                                     DOUBLE client_bitrate,
                                                     UINT32 pkt_size,
                                                     UINT32 pkt_per_int,
                                                     UINT32 pkt_per_n,
                                                     UINT32 pkt_per_d,
                                                     UINT32 *cn_base_int,
                                                     UINT32 *cn_base_n,
                                                     UINT32 *cn_base_d,
                                                     BOOL *program_cn_n_first)
{


    UINT32 frm_period_idx;
    util_opsa_cn_frame_period_t cn_frame_period;
    UINT32 cn_d_mult;


    UINT32 *frm_period_n_ptr = &(hndl->var.ch_cfg_ptr[chnl].frm_period_n);
    UINT32 *frm_period_d_ptr = &(hndl->var.ch_cfg_ptr[chnl].frm_period_d);


    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* Check for valid packet size range. */
    if ((pkt_size < MPMO_PKT_SIZE_MIN) || (pkt_size > MPMO_PKT_SIZE_MAX)) {
        PMC_RETURN(MPMO_LOG_ERR_INVALID_PARAMETERS);
    }



    frm_period_idx = hndl->var.ch_cfg_ptr[chnl].frm_period_idx;

    /*look up the frame period of the server using the context */
    if ((hndl->cfg.default_param.mpmo_frm_period_flex_alloc_method) == FALSE)
    {
        cn_frame_period = hndl->cfg.default_param.frm_period_cnt_ptr[frm_period_idx].frm_period_type;
    }
    else
    {
        cn_frame_period = hndl->cfg.default_param.frm_period_cnt[frm_period_idx].frm_period_type;
    }
 
    /* Calculate the fractional representation of Cn,base. */
    util_opsa_mpmo_cn_base_calc(*frm_period_n_ptr, *frm_period_d_ptr,
                                pkt_per_int, pkt_per_n, pkt_per_d, pkt_size,
                                cn_base_int, cn_base_n, cn_base_d);

    /*now, maximize Cn denominator*/

    cn_d_mult = 0xFFFFFFFF/(*cn_base_d);


    *cn_base_n = (*cn_base_n)*cn_d_mult;
    *cn_base_d = (*cn_base_d)*cn_d_mult;


    /* decide programming order based on maginitude
    of new and old cn_base_numberator*/
    if(hndl->var.ch_cfg_ptr[chnl].cn_base_n < (*cn_base_n))
    {   
        *program_cn_n_first = FALSE;
    }
    else
    {
        *program_cn_n_first = TRUE;
    }



    PMC_RETURN(ret_val);

} /* mpmo_ghao_calculate_new_cn_max_cn_d */

/*******************************************************************************
*  mpmo_ghao_new_cn_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Wrapper function to set a custom userbit mask  
*
* INPUTS: 
*   *hndl                  - pointer to MPMO handle instance
*   chnl                   - Channel id to operate on 
*   program_cn_n_first     - TRUE : program Cn numerator first and then
*                                   Cn denomenator \n
*                            FALSE : programing Cn denomenator first and
*                                    the Cn numerator
*   cn_base_int            - Cn base interger component
*   cn_base_n              - Cn base decimal numerator component
*   cn_base_d              - Cn base decimal denomenator component
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
PUBLIC PMC_ERROR mpmo_ghao_new_cn_set(mpmo_handle_t *hndl,
                                      UINT32 chnl,
                                      BOOL program_cn_n_first,
                                      UINT32 cn_base_int,
                                      UINT32 cn_base_n,
                                      UINT32 cn_base_d)
{


    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    hndl->var.ch_cfg_ptr[chnl].cn_base_int = cn_base_int;
    hndl->var.ch_cfg_ptr[chnl].cn_base_n = cn_base_n;
    hndl->var.ch_cfg_ptr[chnl].cn_base_d = cn_base_d;



    mpmo_dsp_field_CN_BASE_INT_set(NULL, hndl, chnl, hndl->var.ch_cfg_ptr[chnl].cn_base_int);
    
    if(program_cn_n_first == TRUE)
    {
        mpmo_dsp_field_CN_BASE_N_set(NULL, hndl, chnl, hndl->var.ch_cfg_ptr[chnl].cn_base_n);
        mpmo_dsp_field_CN_BASE_D_set(NULL, hndl, chnl, hndl->var.ch_cfg_ptr[chnl].cn_base_d);
    }
    else
    {
        mpmo_dsp_field_CN_BASE_D_set(NULL, hndl, chnl, hndl->var.ch_cfg_ptr[chnl].cn_base_d);
        mpmo_dsp_field_CN_BASE_N_set(NULL, hndl, chnl, hndl->var.ch_cfg_ptr[chnl].cn_base_n);
    }


    PMC_RETURN(ret_val);
} /*mpmo_ghao_new_cn_set */

/*******************************************************************************
* mpmo_int_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   *mpmo_handle     - pointer to MPMO handle instance
*   chnl_id          - chnl id
*   non_null_ptr     - Pointer to check that shouldn't be NULL
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
PRIVATE PMC_ERROR mpmo_int_validate(mpmo_handle_t *mpmo_handle,
                                    UINT32 chnl_id,
                                    void *non_null_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != mpmo_handle, MPMO_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (chnl_id >= mpmo_handle->cfg.default_param.num_chnl)
    {
        result = MPMO_LOG_ERR_INVALID_PARAMETERS;
    }

    if (NULL == non_null_ptr)
    {
        result = MPMO_LOG_ERR_INVALID_PARAMETERS;
    } 

    PMC_RETURN(result);
} /* mpmo_int_validate */


/*******************************************************************************
*  mpmo_calendar_pinch_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Wrapper function to set a custom userbit mask  
*
* INPUTS:
*   *hndl               - pointer to MPMO handle instance
*   cal_pinch_type      - See util_global_calendar_pinch_t.  Used to set a custom
*                         calendar pinch scenario.
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
PUBLIC PMC_ERROR mpmo_calendar_pinch_mode_set(mpmo_handle_t *hndl,
                                              util_global_calendar_pinch_t cal_pinch_type)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;


    PMC_ENTRY();


    ret_val = scbs3_calendar_pinch_mode_set(hndl->scbs3_handle,
                                            cal_pinch_type);
    
    
    PMC_RETURN(ret_val);

} /* mpmo_calendar_pinch_mode_set */



/*******************************************************************************
*  mpmo_reprov_pkt_size_configs
* ______________________________________________________________________________
*
* DESCRIPTION:
*   API used to change configurations relating the a packet size change
*   on the fly (while a channel is operational).
*
* INPUTS:
*   *hndl             - pointer to MPMO handle instance
*   chnl              - Channel ID
*   pkt_size          - packet size configured at the packet maker.
*   pkt_per_int       - integer portion for packet period
*                       configured at the packet maker.
*   pkt_per_n         - numerator for packet period fraction
*                       configured at the packet maker.
*   pkt_per_d         - denominator for packet period fraction
*                       configured at the packet maker.
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
PUBLIC PMC_ERROR mpmo_reprov_pkt_size_configs(mpmo_handle_t *hndl,
                                              UINT32 chnl,
                                              UINT32 pkt_size,
                                              UINT32 pkt_per_int,
                                              UINT32 pkt_per_n,
                                              UINT32 pkt_per_d)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT32 *pp_int_ptr   = &(hndl->var.ch_cfg_ptr[chnl].pkt_period_int);
    UINT32 *pp_n_ptr     = &(hndl->var.ch_cfg_ptr[chnl].pkt_period_n);
    UINT32 *pp_d_ptr     = &(hndl->var.ch_cfg_ptr[chnl].pkt_period_d);
    UINT32 *ps_ptr       = &(hndl->var.ch_cfg_ptr[chnl].pkt_size);

    UINT32 *cn_base_int_ptr  = &(hndl->var.ch_cfg_ptr[chnl].cn_base_int);
    UINT32 *cn_base_n_ptr    = &(hndl->var.ch_cfg_ptr[chnl].cn_base_n);
    UINT32 *cn_base_d_ptr    = &(hndl->var.ch_cfg_ptr[chnl].cn_base_d);

    UINT32 *frm_period_n_ptr = &(hndl->var.ch_cfg_ptr[chnl].frm_period_n);
    UINT32 *frm_period_d_ptr = &(hndl->var.ch_cfg_ptr[chnl].frm_period_d);

    PMC_ENTRY();


    /*set CH_EN and SLICE_ENABLE to 0 so new configurations can get
    programmed in the event of a changing packet size*/
    mpmo_ch_enable_cfg(hndl, chnl, FALSE);

    if (hndl->var.ch_cfg_ptr[chnl].cn_generate_en) {
        mpmo_dsp_ch_enable_cfg(hndl, chnl, FALSE);
    }


    /* Record the specified packet size. */
    *ps_ptr = pkt_size;
    /* Record the specified packet period. */
    *pp_int_ptr   = pkt_per_int;
    *pp_n_ptr     = pkt_per_n;
    *pp_d_ptr     = pkt_per_d;


    if(hndl->var.ch_cfg_ptr[chnl].cn_generate_en) {

            /* Calculate the fractional representation of Cn,base. */
            util_opsa_mpmo_cn_base_calc(*frm_period_n_ptr, *frm_period_d_ptr,
                                        *pp_int_ptr, *pp_n_ptr, *pp_d_ptr, *ps_ptr,
                                        cn_base_int_ptr, cn_base_n_ptr, cn_base_d_ptr);
    
    }


    mpmo_ch_bw_cfg(hndl, chnl);


    mpmo_ch_enable_cfg(hndl, chnl, TRUE);

    if (hndl->var.ch_cfg_ptr[chnl].cn_generate_en) {
        mpmo_dsp_ch_enable_cfg(hndl, chnl, TRUE);
    }



    PMC_RETURN(ret_val);

} /* mpmo_reprov_pkt_size_configs */


/*******************************************************************************
*  mpmo_client_bitrate_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   API used to get the client bitrate in context.
*
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
*   chnl           - channel id
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   DOUBLE         - client bitrate in context
*
* NOTES:
*
*******************************************************************************/
PUBLIC DOUBLE mpmo_client_bitrate_get(mpmo_handle_t *hndl,
                                      UINT32 chnl)
{

    PMC_ENTRY();
    PMC_RETURN(hndl->var.ch_cfg_ptr[chnl].client_bitrate);

} /* mpmo_client_bitrate_get */

/*******************************************************************************
*  mpmo_ch_fifo_clear_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   API set value inf MPMO CFC fifo clear bit.
*
* INPUTS:
*   *hndl          - pointer to MPMO handle instance
*   chnl           - channel id
*   reset          - when TRUE: FIFO is into reset
*                    when FALSE: FIFO is out of reset.
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
PUBLIC void mpmo_ch_fifo_clear_set(mpmo_handle_t *hndl,
                                   UINT32         chnl, 
                                   BOOL8          reset)
{

    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, MPMO_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < hndl->cfg.default_param.num_chnl, MPMO_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    cfc_fifo_reset_cfg(hndl->cfc_handle, chnl, TRUE == reset ? 0 : 1);

    PMC_RETURN();

} /* mpmo_client_bitrate_get */


/*
** End of file
*/
