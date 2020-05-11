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

#include "oduksc_loc.h"


/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
/* Define TARGET_NUMERATOR value for ODUFlex clients */
#define ODUFLEX_TARGET_NUMERATOR { \
   /* Index is number of ODU0's in ODUFlex container */  \
   /* 1,      2,        3,        4,        5,        6,        7,        8,        9,        10, */  \
   {15206400, 3785694 , 5679497 , 7572662 , 9465828 , 11358993, 13252159, 15145325, 16966598, 18851775}, /* oduflex 1 - 10  */ \
   {20736953, 22622130, 24507308, 26392485, 28277663, 30162841, 32048018, 33933196, 35818373, 37703551}, /* oduflex 11 - 20 */ \
   {39588728, 41473906, 43359083, 45244261, 47129438, 49014616, 50899793, 52784971, 54670148, 56555326}, /* oduflex 21 - 30 */ \
   {58440504, 30162841, 29982193, 30890744, 31799296, 32707847, 33616398, 34524950, 35433501, 36342052}, /* oduflex 31 - 40 */ \
   {37250604, 38159155, 39067706, 39976257, 40884809, 41793361, 42701911, 43610463, 44519014, 45427565}, /* oduflex 41 - 50 */ \
   {46336117, 47244668, 48153219, 49061771, 49970322, 50878873, 51787424, 52695976, 53604527, 54513078}, /* oduflex 51 - 60 */ \
   {55421630, 56330181, 57238732, 29073642, 29527917, 29982193, 30436469, 30890744, 31345020, 31799296}, /* oduflex 61 - 70 */ \
   {32253571, 32707847, 33162123, 33616398, 34070674, 34524950, 34979225, 35433501, 35887777, 36342052}, /* oduflex 71 - 80 */ \
} /* ODUFlex TARGET_NUMERATOR sequence */ 

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
const char ODUKSC_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    ODUKSC_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char ODUKSC_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */


/*
** Forward References
*/
PRIVATE void oduksc_var_default_init(oduksc_handle_t *oduksc_handle);

PRIVATE PMC_ERROR oduksc_bmp_mode_cgen_chnl_cfg(oduksc_handle_t *oduksc_handle,
                                                UINT32 chnl,
                                                util_global_odukp_type_t server_type,
                                                util_global_odukp_type_t client_type,
                                                UINT32 client_rate,
                                                UINT32 rate_ramp);                                       

PRIVATE PMC_ERROR oduksc_cgen_chnl_update(oduksc_handle_t *oduksc_handle,
                                          UINT32 chnl);

PRIVATE PMC_ERROR oduksc_cgen_standby_params_set(oduksc_handle_t *oduksc_handle,
                                                 UINT32 chnl,
                                                 UINT32 cgen_m,
                                                 UINT32 cgen_n,
                                                 UINT32 cgen_m2,
                                                 UINT32 cgen_n2,
                                                 UINT32 cgen_s,
                                                 UINT32 cgen_u_d,
                                                 UINT32 chnl_reset);

PRIVATE PMC_ERROR oduksc_cgen_active_params_get(oduksc_handle_t *oduksc_handle,
                                                UINT32 chnl,
                                                UINT32 *cgen_m,
                                                UINT32 *cgen_n,
                                                UINT32 *cgen_m2,
                                                UINT32 *cgen_n2,
                                                UINT32 *cgen_s,
                                                UINT32 *cgen_u_d);

PRIVATE PMC_ERROR oduksc_client_rate_calc(util_global_odukp_type_t client_type,
                                          UINT32 client_rate,
                                          UINT32 *rate);

PRIVATE PMC_ERROR oduksc_core_cgen_chnl_cfg(oduksc_handle_t *oduksc_handle,
                                            UINT32 chnl,
                                            util_global_odukp_type_t server_type,
                                            util_global_odukp_type_t client_type,
                                            UINT32 client_rate,
                                            UINT32 odu_flex_data,
                                            UINT32 mc_numer);

PRIVATE PMC_ERROR oduksc_chnl_activate_internal(oduksc_handle_t *oduksc_handle,
                                                UINT32 chnl, BOOL8 super_prov);

PRIVATE PMC_ERROR oduksc_chnl_deactivate_internal(oduksc_handle_t *oduksc_handle,
                                                  UINT32 chnl,
                                                  BOOL8 super_deprov);                                                                                            
PRIVATE PMC_ERROR oduksc_ri_cfg(oduksc_handle_t *oduksc_handle, UINT32 chnl,
                                UINT32 ri_id);

PRIVATE PMC_ERROR oduksc_ri_activate(oduksc_handle_t *oduksc_handle, UINT32 ri_id);

PRIVATE PMC_ERROR oduksc_ri_deactivate(oduksc_handle_t *oduksc_handle,
                                       UINT32 ri_id);

PRIVATE PMC_ERROR oduksc_ri_get(oduksc_handle_t *oduksc_handle, UINT32 *ri_id);

PRIVATE PMC_ERROR oduksc_oduflex_gfp_rate_get(oduksc_handle_t *oduksc_handle,
                                              UINT32 tribslots,
                                              DOUBLE *oduflex_rate_bps);

PRIVATE PMC_ERROR oduksc_slot_num_get(oduksc_handle_t *oduksc_handle,
                                      UINT32 chnl,
                                      UINT32 *slot_num);

PRIVATE PMC_ERROR oduksc_int_validate(oduksc_handle_t *oduksc_handle,
                                      void *non_null_ptr);

PRIVATE PMC_ERROR oduksc_int_chnl_validate(oduksc_handle_t *oduksc_handle,
                                           UINT32 chnl_id,
                                           void *non_null_ptr);
/*
** Public Functions
*/

/*******************************************************************************
* oduksc_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a oduksc block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the oduksc subsystem relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifier of the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   oduksc_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC oduksc_handle_t *oduksc_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    oduksc_handle_t *oduksc_handle;
    PMC_ENTRY();
    oduksc_handle = (oduksc_handle_t*)PMC_CTXT_CALLOC(sizeof(oduksc_handle_t), parent);

    pmc_handle_init(parent, oduksc_handle, sys_handle, PMC_MID_DIGI_ODUKSC, tsb_name, base_address);

    /* Register log strings. */
    pmc_log_block_strings_register(ODUKSC_LOG_ERR_STRINGS[0], ODUKSC_LOG_ERR_TABLE_BASE, ODUKSC_LOG_ERR_COUNT);

    oduksc_handle->scbs3_handle = scbs3_ctxt_create((pmc_handle_t *)oduksc_handle, base_address+BASE_ADDR_ODUKSC_SCBS3, sys_handle, "SCBS3");
    PMC_CTXT_REGISTER_PTR(&oduksc_handle->scbs3_handle, parent);


    PMC_RETURN(oduksc_handle);
} /* oduksc_ctxt_create */


/*******************************************************************************
* oduksc_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a oduksc block instance.
*
*
* INPUTS:
*   *oduksc_handle   - pointer to ODUKSC handle instance
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
PUBLIC void oduksc_ctxt_destroy(oduksc_handle_t *oduksc_handle)
{
    PMC_ENTRY();

    if (TRUE == oduksc_handle->cfg.scbs3_present) 
    {
        scbs3_ctxt_destroy(oduksc_handle->scbs3_handle);
    }

    if (oduksc_handle->cfg.db_handle != 0)
    {
        gen_db_detach(&(oduksc_handle->base), 
                      oduksc_handle->cfg.db_id,
                      oduksc_handle->cfg.db_handle);
    }

    UINT32 chnl;
    for (chnl = 0; chnl < oduksc_handle->cfg.max_chnl; chnl++)
    {
        PMC_FREE(&oduksc_handle->var.ch_ctxt_ptr[chnl].ghao);
    }

    PMC_CTXT_FREE(&(oduksc_handle->var.ch_ctxt_ptr), oduksc_handle);
    PMC_CTXT_FREE(&oduksc_handle, oduksc_handle);
    PMC_RETURN();
} /* oduksc_ctxt_destroy */


/*******************************************************************************
* oduksc_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an ODUKSC block handle instance.
*
*
* INPUTS:
*   *oduksc_handle     - pointer to ODUKSC handle instance
*   oduksc_type_id     - ODUKSC ID of this instance \n
*                        Refer to oduksc_type_id_t \n
*   userbits_del       - Side band user bits
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
PUBLIC void oduksc_handle_init(oduksc_handle_t *oduksc_handle,
                               oduksc_type_id_t oduksc_type_id,
                               UINT32 userbits_del)
{
    /* variable declaration */
    UINT32      size;
    scbs3_cfg_t scbs_cfg;

    PMC_ENTRY();
    
    oduksc_handle->cfg.total_rate       = ODUKSC_TOTAL_RATE;
    oduksc_handle->cfg.total_rate_units = ODUKSC_SCBS3_TOTAL_RATE_UNITS;
    oduksc_handle->cfg.bus_width        = ODUKSC_BUS_WIDTH;
    oduksc_handle->cfg.cal_entries_alloc_method = UTIL_SCHD_ALLOC_METHOD_GENERAL_LOCAL ;

    /* SCBS3 parameters */
    oduksc_handle->cfg.scbs3_cfg.num_grp              = ODUKSC_SCBS3_NUM_GRP;
    oduksc_handle->cfg.scbs3_cfg.max_chnl             = ODUKSC_NUM_CHNL;
    oduksc_handle->cfg.scbs3_cfg.max_rows             = ODUKSC_SCBS3_MAX_ROWS;
    oduksc_handle->cfg.scbs3_cfg.total_rate           = ODUKSC_SCBS3_TOTAL_RATE;
    oduksc_handle->cfg.scbs3_cfg.total_rate_units     = ODUKSC_SCBS3_TOTAL_RATE_UNITS;
    oduksc_handle->cfg.scbs3_cfg.toc_sync_en          = ODUKSC_SCBS3_TOC_SYNC_EN;
    oduksc_handle->cfg.scbs3_cfg.toc_sync             = ODUKSC_SCBS3_TOC_SYNC;
    oduksc_handle->cfg.scbs3_cfg.page_swap_mode       = (scbs3_page_swap_mode_t)ODUKSC_SCBS3_PAGE_SWAP_BIT;
    oduksc_handle->cfg.scbs3_cfg.bonus_mode_enabled   = ODUKSC_SCBS3_BONUS_MODE;
    oduksc_handle->cfg.scbs3_cfg.enable_row_resizing  = ODUKSC_SCBS3_ROW_RESIZE;
    
    
    /* Initialize oduksc_cfg_t structure for a specific parent */
    oduksc_handle->cfg.userbits_del      = userbits_del;        
    oduksc_handle->cfg.db_id             = UTIL_GEN_DB_UNUSED;
    oduksc_handle->cfg.scbs3_present     = TRUE;
    oduksc_handle->cfg.ramp_fast_mode_en = FALSE;
    oduksc_handle->cfg.ramp_fast_mode_factor = 1;

    /* Configure parameters for specific instantiations. */
    PMC_LOG_TRACE("oduksc_type_id= %d\n", oduksc_type_id);
    switch (oduksc_type_id) {
        case ODUKSC_COREOTN_ODUKSC1:
            oduksc_handle->cfg.max_chnl                 = ODUKSC_NUM_CHNL;
            oduksc_handle->cfg.cal_entries_alloc_method = UTIL_SCHD_ALLOC_METHOD_COREOTN_ODUKSC1;  
            oduksc_handle->cfg.db_en                    = TRUE;
            oduksc_handle->cfg.db_id                    = UTIL_GEN_DB_LINE_CORE_OTN_RX_DB;
            oduksc_handle->cfg.scbs3_cfg.toc_sync       = 0x11;
            oduksc_handle->cfg.scbs3_cfg.toc_sync_en    = TRUE;
            break;
        case ODUKSC_COREOTN_ODUKSC2:
            oduksc_handle->cfg.max_chnl                 = ODUKSC_NUM_CHNL;
            oduksc_handle->cfg.cal_entries_alloc_method = UTIL_SCHD_ALLOC_METHOD_COREOTN_ODUKSC2; 
            oduksc_handle->cfg.db_en                    = TRUE;
            oduksc_handle->cfg.db_id                    = UTIL_GEN_DB_LINE_CORE_OTN_RX_DB;
            oduksc_handle->cfg.scbs3_cfg.toc_sync       = 0x11;
            oduksc_handle->cfg.scbs3_cfg.toc_sync_en    = TRUE;
            break;
        case ODUKSC_MAPOTN:
            oduksc_handle->cfg.max_chnl                 = ODUKSC_NUM_CHNL;
            oduksc_handle->cfg.cal_entries_alloc_method = UTIL_SCHD_ALLOC_METHOD_MAPOTN_ODUKSC;  
            oduksc_handle->cfg.db_en                    = TRUE;
            oduksc_handle->cfg.db_id                    = UTIL_GEN_DB_ODUKSC_DB;
            oduksc_handle->cfg.scbs3_cfg.toc_sync_en    = TRUE;
            oduksc_handle->cfg.scbs3_cfg.toc_sync       = 0;
            break;

        case ODUKSC_LINEOTN:
            oduksc_handle->cfg.max_chnl                 = 12; /* LineOTN has only 12 HO time slots*/
            oduksc_handle->cfg.cal_entries_alloc_method = UTIL_SCHD_ALLOC_METHOD_LINEOTN_LOCAL;  
            oduksc_handle->cfg.db_en                    = FALSE;
            break;

        case ODUKSC_COREOTN_ODUJAT1:
            oduksc_handle->cfg.max_chnl         = ODUKSC_NUM_CHNL;
            oduksc_handle->cfg.cal_entries_alloc_method = UTIL_SCHD_ALLOC_METHOD_GENERAL_LOCAL ;
            oduksc_handle->cfg.db_en            = FALSE;
            oduksc_handle->cfg.scbs3_present    = FALSE;
            break;
        case ODUKSC_COREOTN_ODUJAT2:
            oduksc_handle->cfg.max_chnl         = ODUKSC_NUM_CHNL;
            oduksc_handle->cfg.cal_entries_alloc_method = UTIL_SCHD_ALLOC_METHOD_GENERAL_LOCAL ;
            oduksc_handle->cfg.db_en            = FALSE;
            oduksc_handle->cfg.scbs3_present    = FALSE;
            break;
        default:
            PMC_DEBUG(ODUKSC_ERR_INVALID_PARAMETERS, oduksc_type_id, 0);
            break;
    } /* end switch */

    if( oduksc_handle->cfg.cal_entries_alloc_method == 3  ||
        oduksc_handle->cfg.cal_entries_alloc_method == 12 ||
        oduksc_handle->cfg.cal_entries_alloc_method == 13 )
    {
        /* attach to the databse */
        oduksc_handle->cfg.db_handle = gen_db_attach(&(oduksc_handle->base),oduksc_handle->cfg.db_id);
    }
    
    PMC_LOG_TRACE("oduksc_handle->cfg.db_id= %d, oduksc_handle->cfg.db_handle=%d \n", oduksc_handle->cfg.db_id, oduksc_handle->cfg.db_handle);

    size = sizeof(oduksc_ch_ctxt_struct_t) * oduksc_handle->cfg.max_chnl;
    oduksc_handle->var.ch_ctxt_ptr = (oduksc_ch_ctxt_struct_t*)PMC_CTXT_CALLOC(size, oduksc_handle);
    PMC_CTXT_REGISTER_PTR(&oduksc_handle->var.ch_ctxt_ptr, oduksc_handle);

    UINT32 chnl;
    for (chnl = 0; chnl < oduksc_handle->cfg.max_chnl; chnl++)
    {
        oduksc_handle->var.ch_ctxt_ptr[chnl].ghao = (oduksc_ch_ctxt_ghao_t*) PMC_CALLOC( sizeof(oduksc_ch_ctxt_ghao_t));
    }

    /* Initialize var_t parameters */
    oduksc_var_default_init(oduksc_handle);

    if (oduksc_handle->cfg.scbs3_present == TRUE)
    {
        /* Initialize SCBS3 */
        scbs_cfg.num_grp             = oduksc_handle->cfg.scbs3_cfg.num_grp;
        scbs_cfg.max_chnl            = oduksc_handle->cfg.scbs3_cfg.max_chnl;
        scbs_cfg.max_rows            = oduksc_handle->cfg.scbs3_cfg.max_rows;
        scbs_cfg.total_rate          = oduksc_handle->cfg.scbs3_cfg.total_rate;
        scbs_cfg.total_rate_units    = oduksc_handle->cfg.scbs3_cfg.total_rate_units;
        scbs_cfg.toc_sync_en         = oduksc_handle->cfg.scbs3_cfg.toc_sync_en;
        scbs_cfg.toc_sync            = oduksc_handle->cfg.scbs3_cfg.toc_sync;
        scbs_cfg.page_swap_mode      = oduksc_handle->cfg.scbs3_cfg.page_swap_mode;
        scbs_cfg.bonus_mode_enabled  = oduksc_handle->cfg.scbs3_cfg.bonus_mode_enabled;
        scbs_cfg.enable_row_resizing = oduksc_handle->cfg.scbs3_cfg.enable_row_resizing;
        scbs_cfg.cal_entries_alloc_method = oduksc_handle->cfg.cal_entries_alloc_method;
        
        scbs3_handle_init(oduksc_handle->scbs3_handle, &scbs_cfg);      
    }      

    oduksc_handle->cfg.num_mult = 1;
    oduksc_handle->cfg.demon_mult = 1;
 
    PMC_RETURN();

} /* oduksc_handle_init */

/*******************************************************************************
*  oduksc_alloc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*  
* Handle allocation of local memory.
*
* INPUTS:
*   *oduksc_handle       - pointer to ODUKSC handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*   
*  
* NOTES:
*
*******************************************************************************/
PUBLIC void oduksc_alloc_init(oduksc_handle_t *oduksc_handle)
{
    PMC_ENTRY();  

    scbs3_alloc_init(oduksc_handle->scbs3_handle);


    UINT32 chnl;
    
    for (chnl = 0; chnl < oduksc_handle->cfg.max_chnl; chnl++)
    {
        oduksc_handle->var.ch_ctxt_ptr[chnl].ghao = (oduksc_ch_ctxt_ghao_t*) PMC_CALLOC( sizeof(oduksc_ch_ctxt_ghao_t));
    }

    PMC_RETURN();
} /* oduksc_alloc_init */

/*******************************************************************************
*  oduksc_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on ODUKSC handle instance.
*
* INPUTS:
*   *oduksc_handle            - pointer to ODUKSC handle instance
*   *restart_init_cfg_ptr     - pointer to restart init config
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
PUBLIC PMC_ERROR oduksc_handle_restart_init(oduksc_handle_t *oduksc_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;


    PMC_ENTRY();  

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(ODUKSC_LOG_ERR_STRINGS[0], ODUKSC_LOG_ERR_TABLE_BASE, ODUKSC_LOG_ERR_COUNT);
    }

    if (TRUE == oduksc_handle->cfg.scbs3_present) 
    {
        result = scbs3_handle_restart_init(oduksc_handle->scbs3_handle,restart_init_cfg_ptr, PMC_ENERGY_STATE_RUNNING, TRUE);
    }

    PMC_RETURN(result);
} /* oduksc_handle_restart_init */


/*******************************************************************************
*  oduksc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize ODUKSC with parameters specific to this instantiation.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   energy_state      - enum defining state to set the ODUKSC module to
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   This function uses pmc_energy_state_t for defining its energy states as there 
*   are no optimization in energy state required for ODUKSC module
*
*******************************************************************************/
PUBLIC PMC_ERROR oduksc_init(oduksc_handle_t *oduksc_handle,
                             pmc_energy_state_t energy_state)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    PMC_ASSERT(oduksc_handle!=NULL, ODUKSC_ERR_INVALID_PARAMETERS, 0, 0);
    
    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        /* Set the number of clock cycles by which the delayed versions of the
           user-programmable output signals lag the undelayed versions */
        oduksc_field_USERBITS_DEL_set(NULL, oduksc_handle, oduksc_handle->cfg.userbits_del);
    
        if (oduksc_handle->cfg.scbs3_present == TRUE)
        {
            /* Initialize SCBS3 scheduler */
            ret_val = scbs3_schd_init(oduksc_handle->scbs3_handle, energy_state);
        }
    }
    else if (PMC_ENERGY_STATE_RESET == energy_state)
    {
        oduksc_var_default_init(oduksc_handle);
        
        if (oduksc_handle->cfg.scbs3_present == TRUE) {
            ret_val = scbs3_schd_init(oduksc_handle->scbs3_handle, energy_state);
        }
    } else {
        PMC_ASSERT(FALSE, ODUKSC_ERR_INVALID_PARAMETERS, 0, 0);
    }
    
    PMC_RETURN(ret_val);
} /* oduksc_init */

/*******************************************************************************
* oduksc_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   ODUKSC instance.  It will test the channel state for all resources within
*   the ODUKSC instance if any are not in ODUKSC_CHNL_START state then FALSE
*   is returned, else TRUE.  Should TRUE be returned it can be assumed that
*   no resources are provisioned against the ODUKSC and it would be
*   possible for the higher layer module to put the module in lowpower standby
*   mode of operation.
*
*
* INPUTS:
*   *oduksc_handle        - pointer to ODUKSC handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL                  - FALSE:  Resource(s) not in start state \n
*                           TRUE:   Resources all in start state
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL oduksc_start_state_test(oduksc_handle_t *oduksc_handle) 
{
    UINT32 chnl;
    PMC_ENTRY();
    PMC_ASSERT(oduksc_handle!=NULL, ODUKSC_ERR_INVALID_PARAMETERS, 0, 0);

    for (chnl = 0; chnl < oduksc_handle->cfg.max_chnl; chnl++) 
    {
        if(oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state != ODUKSC_CHNL_START)
           PMC_RETURN(FALSE);
    } 
    
    if(scbs3_start_state_test(oduksc_handle->scbs3_handle) == FALSE)
        PMC_RETURN(FALSE);
    
        
    PMC_RETURN(TRUE);

} /* oduksc_start_state_test */


/*******************************************************************************
*  oduksc_reset_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Puts ODUKSC in reset or takes the ODUKSC out of reset.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   reset             - reset value \n 
*                       0 -- takes out of reset \n
*                       1 -- puts in reset \n
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
PUBLIC PMC_ERROR oduksc_reset_cfg(oduksc_handle_t *oduksc_handle, UINT32 reset)
{
    /* variable declaration */

    PMC_ENTRY();

    oduksc_field_SOFT_RESET_set(NULL, oduksc_handle, reset);

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_reset_cfg */


/*******************************************************************************
*  oduksc_chnl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provisions a channel for the specified rate.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   server_type       - HO ODUk rate \n
*                       Refer to util_global_odukp_type_t\n
*   client_type       - LO ODUj rate \n
*                       Refer to util_global_odukp_type_t \n
*   client_rate       - client data rate kbits/sec for the case of ODUFLEX
*   mapping_mode      - mapping mode of the channel \n
*                       Refer to util_global_mapping_mode_t \n
*   oduflex_gfp_rate  - ODUflex GFP container rate in number of tribslots
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
PUBLIC PMC_ERROR oduksc_chnl_prov(oduksc_handle_t *oduksc_handle,
                                  UINT32 chnl,
                                  util_global_odukp_type_t server_type,
                                  util_global_odukp_type_t client_type,
                                  UINT32 client_rate,
                                  util_global_mapping_mode_t mapping_mode,
                                  UINT32 oduflex_gfp_rate)
{
    /* variable declaration */
    UINT32 rate=0;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state != ODUKSC_CHNL_START)
    {
        /* channel is not in start state (e.g. it has either been already provisioned or provisioned and activated) */
        PMC_RETURN(ODUKSC_CHNL_STATE_NOT_START);
    }

    if(mapping_mode == UTIL_GLOBAL_GFP)
    {
        oduksc_handle->var.ch_ctxt_ptr[chnl].scbs3_user_bits=0x1;
    }
    else
    {
        oduksc_handle->var.ch_ctxt_ptr[chnl].scbs3_user_bits=0x0;
    }

    /* Calculate client_rate */
    oduksc_client_rate_calc(client_type, client_rate, &rate);

    PMC_LOG_TRACE("%s, %s, %d,  before scbs3 \n", __FILE__, __FUNCTION__, __LINE__ );

    if (oduksc_handle->cfg.scbs3_present == TRUE)
    {
        /* Provision SCBS3 */
        ret_val = scbs3_chnl_prov(oduksc_handle->scbs3_handle, chnl, rate, oduksc_handle->cfg.total_rate_units, oduksc_handle->var.ch_ctxt_ptr[chnl].scbs3_user_bits);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        
        /* Activate SCBS3 */
        ret_val = scbs3_chnl_activate(oduksc_handle->scbs3_handle, chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        
        PMC_LOG_TRACE("%s, %s, %d,  after scbs3 \n", __FILE__, __FUNCTION__, __LINE__ );
        
        if(UTIL_GLOBAL_BMP == mapping_mode)
        {
            /* Provision credit generator for BMP mode */
            ret_val = oduksc_bmp_mode_cgen_chnl_cfg(oduksc_handle, chnl, server_type, client_type, client_rate, 0);
            if (ret_val != PMC_SUCCESS) {
                PMC_RETURN(ret_val);
            }
        }
        else
        {
            /* Provision credit generator */
            ret_val = oduksc_cgen_chnl_cfg(oduksc_handle, chnl, server_type, client_type, client_rate, oduflex_gfp_rate);
            if (ret_val != PMC_SUCCESS) {
                PMC_RETURN(ret_val);
            }    
        }
    }
    else {
        
        /* Need a different cgen_chnl_cfg for ODUJAT 
        ** this will not be used as oduksw_odujat_chnl_prov() replaces this
        */
        oduksc_core_cgen_chnl_cfg(oduksc_handle, chnl, server_type, client_type,
                                  client_rate, 0, 0);
    }

    /* Update channel state */
    oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state = ODUKSC_CHNL_PROVISION_PENDING;

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_chnl_prov */

/*******************************************************************************
*  oduksc_odujat_chnl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provisions a channel for the specified rate.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   server_type       - HO ODUk rate \n
*                       Refer to util_global_odukp_type_t\n
*   client_type       - LO ODUj rate \n
*                       Refer to util_global_odukp_type_t \n
*   client_rate       - client data rate kbits/sec for the case of ODUFLEX
*   odu_flex_data     - number of ODU0s in the ODUFlex container.\n
*   mc_numer          - MC_NUMERATOR value to be used in N calculation \n
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
PUBLIC PMC_ERROR oduksc_odujat_chnl_prov(oduksc_handle_t *oduksc_handle,
                                         UINT32 chnl,
                                         util_global_odukp_type_t server_type,
                                         util_global_odukp_type_t client_type,
                                         UINT32 client_rate,
                                         UINT32 odu_flex_data,
                                         UINT32 mc_numer)
{
    /* variable declaration */
    UINT32 rate = 0;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state != ODUKSC_CHNL_START)
    {
        /* channel is not in start state (e.g. it has either been already provisioned or provisioned and activated) */
        PMC_RETURN(ODUKSC_CHNL_STATE_NOT_START);
    }

    oduksc_handle->var.ch_ctxt_ptr[chnl].scbs3_user_bits=0x0;

    /* Calculate client_rate */
    oduksc_client_rate_calc(client_type, client_rate, &rate);

    /* Need a different cgen_chnl_cfg for ODUJAT */
    ret_val = oduksc_core_cgen_chnl_cfg(oduksc_handle, chnl, server_type, 
                                       client_type, client_rate, odu_flex_data,
                                       mc_numer);

    if (PMC_SUCCESS == ret_val)
    {
        /* Update channel state */
        oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state = ODUKSC_CHNL_PROVISION_PENDING;
    }
    
    PMC_RETURN(ret_val);

} /* oduksc_odujat_chnl_prov */


/*******************************************************************************
*  oduksc_chnl_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provisions a channel for the specified rate.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   chnl_state        - Enum, See oduksc_chnl_state_t 
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
PUBLIC PMC_ERROR oduksc_chnl_state_set(oduksc_handle_t *oduksc_handle,
                                       UINT32 chnl,
                                       oduksc_chnl_state_t chnl_state)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    
    oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state = chnl_state;
    
    PMC_RETURN(ret_val);
} /* oduksc_chnl_state_set */  
                                     

/*******************************************************************************
*  oduksc_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovisions the specified channel.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to deprovision
*                       Valid range: 0 - 95
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
PUBLIC PMC_ERROR oduksc_chnl_deprov(oduksc_handle_t *oduksc_handle,
                                    UINT32 chnl)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state != ODUKSC_CHNL_PROVISIONED)
    {
        /* channel is not in provisioned state */
        PMC_RETURN(ODUKSC_CHNL_STATE_NOT_PROVISIONED);
    }

    if (oduksc_handle->cfg.scbs3_present == TRUE)
    {
        /* Deactivate SCBS3 */
        ret_val = scbs3_chnl_deactivate(oduksc_handle->scbs3_handle, chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }

        /* Deprovision SCBS3 */
        ret_val = scbs3_chnl_deprov(oduksc_handle->scbs3_handle, chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }

    /* Update channel state and its var context*/
    oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state      = ODUKSC_CHNL_DEPROVISION_PENDING;
    oduksc_handle->var.ch_ctxt_ptr[chnl].scbs3_user_bits  = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_u_d         = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_s           = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m2          = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n2          = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m           = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n           = 0;

    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_oduflex_gfp_rate = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_oduflex_gfp_rate = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_rate     = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_rate      = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_cgen_m    = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_cgen_n    = 0;


    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_chnl_deprov */


/*******************************************************************************
*  oduksc_chnl_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   cleanup the specified channel.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to deprovision
*                       Valid range: 0 - 95
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
PUBLIC PMC_ERROR oduksc_chnl_cleanup(oduksc_handle_t *oduksc_handle,
                                     UINT32 chnl)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();


    if (oduksc_handle->cfg.scbs3_present == TRUE)
    {
        /* Deactivate SCBS3 */
        ret_val = scbs3_chnl_deactivate(oduksc_handle->scbs3_handle, chnl);

        /* Deprovision SCBS3 */
        ret_val = scbs3_chnl_deprov(oduksc_handle->scbs3_handle, chnl);
    }

    /* Update channel state and its var context*/
    oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state      = ODUKSC_CHNL_DEPROVISION_PENDING;
    oduksc_handle->var.ch_ctxt_ptr[chnl].scbs3_user_bits  = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_u_d         = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_s           = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m2          = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n2          = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m           = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n           = 0;

    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_oduflex_gfp_rate = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_oduflex_gfp_rate = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_rate     = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_rate      = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_cgen_m    = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_cgen_n    = 0;


    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_chnl_cleanup */

/*******************************************************************************
*  oduksc_chnl_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates a provisioned channel.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to activate
*                       Valid range: 0 - 95
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
PUBLIC PMC_ERROR oduksc_chnl_activate(oduksc_handle_t *oduksc_handle,
                                      UINT32 chnl)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    
    ret_val = oduksc_chnl_activate_internal(oduksc_handle, chnl, FALSE);
    
    
    PMC_RETURN(ret_val);
} /* oduksc_chnl_activate */

/*******************************************************************************
*  oduksc_chnl_activate_super_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates a provisioned channel.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to activate
*                       Valid range: 0 - 95
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
PUBLIC PMC_ERROR oduksc_chnl_activate_super_prov(oduksc_handle_t *oduksc_handle,
                                                 UINT32 chnl)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    
    ret_val = oduksc_chnl_activate_internal(oduksc_handle, chnl, TRUE);
    
    
    PMC_RETURN(ret_val);
} /* oduksc_chnl_activate_super_prov */

/*******************************************************************************
* oduksc_int_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   unchannelized interrupt table.
*   
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
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
PUBLIC PMC_ERROR oduksc_int_enable(oduksc_handle_t *oduksc_handle,
                                   oduksc_int_t *int_table_ptr,
                                   oduksc_int_t *int_en_table_ptr,
                                   BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();    
    
    result = oduksc_int_validate(oduksc_handle, int_en_table_ptr);
    
    if (PMC_SUCCESS == result)
    {        
        #ifdef ODUKSC_INT
        #undef ODUKSC_INT
        #endif
        #define ODUKSC_INT    UTIL_GLOBAL_INT_ENABLE_SINGLE
        
        ODUKSC_INT_TABLE_DEFINE();    
    }

    PMC_RETURN(result);
} /* oduksc_int_enable */

/*******************************************************************************
* oduksc_int_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   unchannelized interrupt table.
*   
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
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
PUBLIC PMC_ERROR oduksc_int_clear(oduksc_handle_t *oduksc_handle,
                                  oduksc_int_t *int_table_ptr,
                                  oduksc_int_t *int_en_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    result = oduksc_int_validate(oduksc_handle, int_en_table_ptr);
    
    if (PMC_SUCCESS == result)
    {        
        #ifdef ODUKSC_INT
        #undef ODUKSC_INT
        #endif
        #define ODUKSC_INT UTIL_GLOBAL_INT_CLEAR_SINGLE
    
        ODUKSC_INT_TABLE_DEFINE();
    }
    
    PMC_RETURN(result);
} /* oduksc_int_clear */

/*******************************************************************************
* oduksc_int_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the unchannelized interrupt table.
*   
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
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
PUBLIC PMC_ERROR oduksc_int_retrieve(oduksc_handle_t *oduksc_handle,
                                     oduksc_int_t *filt_table_ptr,
                                     oduksc_int_t *int_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    result = oduksc_int_validate(oduksc_handle, int_table_ptr);
    
    if (PMC_SUCCESS == result)
    {        
        UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(oduksc_handle, oduksc, UTIL_GLOBAL_INT_NO_OPTIMIZE);
    
        #ifdef ODUKSC_INT
        #undef ODUKSC_INT
        #endif
        #define ODUKSC_INT UTIL_GLOBAL_INT_RETRIEVE_SINGLE
    
        ODUKSC_INT_TABLE_DEFINE();

        UTIL_GLOBAL_INT_BUFFER_FLUSH(oduksc_handle, oduksc);
    }

    PMC_RETURN(result);
} /* oduksc_int_retrieve */

/*******************************************************************************
* oduksc_int_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled unchannelized 
*   interrupt table.
*   
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
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
PUBLIC PMC_ERROR oduksc_int_enabled_check(oduksc_handle_t *oduksc_handle,
                                          oduksc_int_t *int_en_table_ptr,
                                          BOOL8 *int_found_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    result = oduksc_int_validate(oduksc_handle, int_found_ptr);
    if (NULL == int_en_table_ptr)
    {
    	result = ODUKSC_ERR_INVALID_PARAMETERS;
    }
    
    if (PMC_SUCCESS == result)
    {        
       #ifdef ODUKSC_INT
       #undef ODUKSC_INT
       #endif
       #define ODUKSC_INT UTIL_GLOBAL_INT_CHECK_SINGLE
        
        ODUKSC_INT_TABLE_DEFINE();
    }
    
    PMC_RETURN(result);
} /* oduksc_int_enabled_check */

/*******************************************************************************
* oduksc_int_chnl_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
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
PUBLIC PMC_ERROR oduksc_int_chnl_enable(oduksc_handle_t *oduksc_handle,
                                        UINT32 chnl_id,
                                        oduksc_int_chnl_t *int_table_ptr,
                                        oduksc_int_chnl_t *int_en_table_ptr,
                                        BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();    

    result = oduksc_int_chnl_validate(oduksc_handle, chnl_id, int_en_table_ptr);
    
    if (PMC_SUCCESS == result)
    {
        #ifdef ODUKSC_INT_CHNL_RANGE
        #undef ODUKSC_INT_CHNL_RANGE
        #endif
        #define ODUKSC_INT_CHNL_RANGE    UTIL_GLOBAL_INT_LFIELD_ENABLE_SINGLE
        
        ODUKSC_INT_CHNL_TABLE_DEFINE();    
    }

    PMC_RETURN(result);
} /* oduksc_int_chnl_enable */

/*******************************************************************************
* oduksc_int_chnl_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
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
PUBLIC PMC_ERROR oduksc_int_chnl_clear(oduksc_handle_t *oduksc_handle,
                                       UINT32 chnl_id,
                                       oduksc_int_chnl_t *int_table_ptr,
                                       oduksc_int_chnl_t *int_en_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    result = oduksc_int_chnl_validate(oduksc_handle, chnl_id, (void *)1);
    
    if (PMC_SUCCESS == result)
    {
        #ifdef ODUKSC_INT_CHNL_RANGE
        #undef ODUKSC_INT_CHNL_RANGE
        #endif
        #define ODUKSC_INT_CHNL_RANGE UTIL_GLOBAL_INT_LFIELD_CLEAR_SINGLE
            
        ODUKSC_INT_CHNL_TABLE_DEFINE();
    }

    PMC_RETURN(result);
} /* oduksc_int_chnl_clear */

/*******************************************************************************
* oduksc_int_chnl_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
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
PUBLIC PMC_ERROR oduksc_int_chnl_retrieve(oduksc_handle_t *oduksc_handle,
                                          UINT32 chnl_id,
                                          oduksc_int_chnl_t *filt_table_ptr,
                                          oduksc_int_chnl_t *int_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    result = oduksc_int_chnl_validate(oduksc_handle, chnl_id, int_table_ptr);
    
    if (PMC_SUCCESS == result)
    {
        
        UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(oduksc_handle, oduksc, UTIL_GLOBAL_INT_NO_OPTIMIZE);
        
        #ifdef ODUKSC_INT_CHNL_RANGE
        #undef ODUKSC_INT_CHNL_RANGE
        #endif
        #define ODUKSC_INT_CHNL_RANGE UTIL_GLOBAL_INT_LFIELD_RETRIEVE_SINGLE
        
        ODUKSC_INT_CHNL_TABLE_DEFINE();
        
        #ifdef ODUKSC_STATUS_CHNL_RANGE
        #undef ODUKSC_STATUS_CHNL_RANGE
        #endif
        #define ODUKSC_STATUS_CHNL_RANGE UTIL_GLOBAL_STATUS_LFIELD_RETRIEVE_SINGLE
        
        ODUKSC_STATUS_CHNL_TABLE_DEFINE();
        
        UTIL_GLOBAL_INT_BUFFER_FLUSH(oduksc_handle, oduksc);
    }

    PMC_RETURN(result);
} /* oduksc_int_chnl_retrieve */

/*******************************************************************************
* oduksc_int_chnl_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
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
PUBLIC PMC_ERROR oduksc_int_chnl_enabled_check(oduksc_handle_t *oduksc_handle,
                                               UINT32 chnl_id,
                                               oduksc_int_chnl_t *int_en_table_ptr,
                                               BOOL8 *int_found_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    result = oduksc_int_chnl_validate(oduksc_handle, chnl_id, int_found_ptr);
    
    if (PMC_SUCCESS == result)
    {
        #ifdef ODUKSC_INT_CHNL_RANGE
        #undef ODUKSC_INT_CHNL_RANGE
        #endif
        #define ODUKSC_INT_CHNL_RANGE UTIL_GLOBAL_INT_LFIELD_CHECK_SINGLE
        
        ODUKSC_INT_CHNL_TABLE_DEFINE();
    }
    
    PMC_RETURN(result);
} /* oduksc_int_chnl_enabled_check */

/*******************************************************************************
*  oduksc_chnl_activate_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates a provisioned channel.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to activate
*                       Valid range: 0 - 95
*   super_prov        - boolean value to determine if this is used for super_prov
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
PRIVATE PMC_ERROR oduksc_chnl_activate_internal(oduksc_handle_t *oduksc_handle,
                                                UINT32 chnl, BOOL8 super_prov)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state != ODUKSC_CHNL_PROVISIONED)
    {
        /* channel is not in provisioned state (e.g. it is either in start state or has been already provisioned and activated) */
        PMC_RETURN(ODUKSC_CHNL_STATE_NOT_PROVISIONED);
    } 

    /* Enable base credit generation */
    oduksc_field_BASE_GEN_EN_set( NULL, oduksc_handle , chnl, 1);
  
    /* Update channel state */
    oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state = ODUKSC_CHNL_ACTIVATED;

    /* Copy active page to standby page */
    ret_val = oduksc_cgen_chnl_update(oduksc_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_chnl_activate_internal */

/*******************************************************************************
*  oduksc_chnl_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates a provisioned channel.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to activate
*                       Valid range: 0 - 95
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
PUBLIC PMC_ERROR oduksc_chnl_deactivate(oduksc_handle_t *oduksc_handle,
                                        UINT32 chnl)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    
    ret_val = oduksc_chnl_deactivate_internal(oduksc_handle, chnl, FALSE);
    
    
    PMC_RETURN(ret_val);
} /* oduksc_chnl_deactivate */


/*******************************************************************************
*  oduksc_chnl_deactivate_super_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates a provisioned channel.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to activate
*                       Valid range: 0 - 95
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
PUBLIC PMC_ERROR oduksc_chnl_deactivate_super_deprov(oduksc_handle_t *oduksc_handle,
                                                     UINT32 chnl)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    
    ret_val = oduksc_chnl_deactivate_internal(oduksc_handle, chnl, TRUE);
    
    
    PMC_RETURN(ret_val);
} /* oduksc_chnl_deactivate_super_deprov */


/*******************************************************************************
*  oduksc_chnl_deactivate_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates a provisioned channel.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to deactivate
*                       Valid range: 0 - 95
*   super_deprov      - boolean to indicate if we are operating in super-deprov \n
*                       TRUE: super-deprov mode. Channel state checking is not performed \n
*                       FALSE: normal deprov. Channel state checking is performed.
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
PRIVATE PMC_ERROR oduksc_chnl_deactivate_internal(oduksc_handle_t *oduksc_handle,
                                                  UINT32 chnl,
                                                  BOOL8 super_deprov)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state != ODUKSC_CHNL_ACTIVATED)
    {
        /* channel is not in activated or reprovisioned state */
        PMC_RETURN(ODUKSC_CHNL_STATE_NOT_ACTIVATED);
    }
    
    /* Disable base credit generation */
    oduksc_field_BASE_GEN_EN_set( NULL, oduksc_handle , chnl, 0);

    /* Update channel state */
    oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state = ODUKSC_CHNL_PROVISIONED;

    /* Copy active page to standby page */
    if (super_deprov == FALSE)
    {
        ret_val = oduksc_cgen_chnl_update(oduksc_handle, chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_chnl_deactivate_internal */

/*******************************************************************************
*  oduksc_chnl_base_gen_en_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Modifes the BASE_GEN_EN register field setting
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to deactivate
*                       Valid range: 0 - 95
*   value             - value to apply to BASE_GEN_EN field
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
PUBLIC void oduksc_chnl_base_gen_en_cfg(oduksc_handle_t *oduksc_handle,
                                        UINT32 chnl,
                                        BOOL8 value)
{
    PMC_ENTRY();

    /* Enable or Disable base credit generation */
    oduksc_field_BASE_GEN_EN_set( NULL, oduksc_handle , chnl, value);

    PMC_RETURN();

} /* oduksc_chnl_base_gen_en_cfg */

/*******************************************************************************
* oduksc_chnl_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean register and memory context of specied channels. 
*                                                                                                                                                     
* INPUTS:                                                                       
*   *oduksc_handle      - pointer to ODUKSC handle instance
*   *chnl               - array of channel ID to be cleaned
*   num_chnl            - number of channels to be cleaned
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
PUBLIC void oduksc_chnl_clean(oduksc_handle_t *oduksc_handle,
                              UINT32 *chnl,
                              UINT32 num_chnl)
{   
    PMC_ERROR ret_val;   
    UINT32 chnl_itr, num_entries;
    scbs3_page_t active_page;
    BOOL8 need_an_update_ch[num_chnl];
    BOOL8 need_an_update;

    PMC_ENTRY();

   
    if (oduksc_handle->cfg.scbs3_present == TRUE)
    {
        /* get active page */
        scbs3_page_get(oduksc_handle->scbs3_handle,SCBS3_ACTIVE_MEM,&active_page);
        
        /* clean sub-block */
        scbs3_chnl_clean(oduksc_handle->scbs3_handle,FALSE, chnl,num_chnl);
    }

    need_an_update = FALSE;
    if (num_chnl != 0)
    {
        
        /*
         * Reset the entire array to ensure all values are FALSE before starting
         * down the path of setting some of them to TRUE.
         */
        PMC_MEMSET(&need_an_update_ch[0], 0, sizeof(need_an_update_ch));
    }
    for (chnl_itr = 0; chnl_itr < num_chnl; chnl_itr ++)
    {
        oduksc_handle->var.ch_ctxt_ptr[chnl[chnl_itr]].chnl_state =  ODUKSC_CHNL_START;
        /* Disable base credit generation */
        oduksc_field_BASE_GEN_EN_set( NULL, oduksc_handle , chnl[chnl_itr], 0);
        oduksc_zone_pushback_cfg(oduksc_handle, chnl[chnl_itr], 0);
        if (oduksc_handle->cfg.scbs3_present == TRUE)
        {
            
            /* 
               Update channel state and its var context
               check if we need to trigger calendar update... (first from the context, then from the SCBS3 active page 
            */
            if (ODUKSC_CHNL_START != oduksc_handle->var.ch_ctxt_ptr[chnl[chnl_itr]].chnl_state)
            {
                oduksc_handle->var.ch_ctxt_ptr[chnl[chnl_itr]].chnl_state = ODUKSC_CHNL_DEPROVISION_PENDING;
                need_an_update_ch[chnl_itr] = TRUE;
            } else 
            {
                num_entries = scbs3_num_entries_per_chnl_get(oduksc_handle->scbs3_handle,&active_page,chnl[chnl_itr]);
                if (num_entries != 0)
                {
                    /* they are entries in reg space that are not in the context */
                    /* they will be cleaned at the page swap */
                    oduksc_handle->var.ch_ctxt_ptr[chnl[chnl_itr]].chnl_state = ODUKSC_CHNL_DEPROVISION_PENDING;
                    need_an_update_ch[chnl_itr] = TRUE;
                }
            }
        } else 
        {
            oduksc_handle->var.ch_ctxt_ptr[chnl[chnl_itr]].chnl_state = ODUKSC_CHNL_DEPROVISION_PENDING;
            need_an_update_ch[chnl_itr] = TRUE;
        }
        oduksc_handle->var.ch_ctxt_ptr[chnl[chnl_itr]].scbs3_user_bits  = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl[chnl_itr]].cgen_u_d         = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl[chnl_itr]].cgen_s           = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl[chnl_itr]].cgen_m2          = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl[chnl_itr]].cgen_n2          = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl[chnl_itr]].cgen_m           = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl[chnl_itr]].cgen_n           = 0;

        oduksc_handle->var.ch_ctxt_ptr[chnl[chnl_itr]].ghao->current_rate     = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl[chnl_itr]].ghao->target_rate      = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl[chnl_itr]].ghao->target_cgen_m    = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl[chnl_itr]].ghao->target_cgen_n    = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl[chnl_itr]].ghao->current_oduflex_gfp_rate = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl[chnl_itr]].ghao->target_oduflex_gfp_rate = 0;

    }

    for (chnl_itr = 0; chnl_itr < num_chnl; chnl_itr ++)
    {
        if (need_an_update_ch[chnl_itr] == TRUE)
        {
            /* Configure standby page for channel */
            ret_val = oduksc_cgen_chnl_update(oduksc_handle, chnl[chnl_itr]);
            if (ret_val != PMC_SUCCESS) {
                PMC_RETURN();
            }
            need_an_update = TRUE;
        }
    }

    if (need_an_update == TRUE)
    {
        /* Swap standby and active pages */
        ret_val = oduksc_page_swap(oduksc_handle, oduksc_handle->cfg.scbs3_present);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN();
        }
    }

    for (chnl_itr = 0; chnl_itr < num_chnl; chnl_itr ++)
    {
        if (need_an_update_ch[chnl_itr] == TRUE)
        {
            /* Configure standby page for channel so that it will be identical to active page
               except for channel reset which will always be cleared for activated channel */
            ret_val = oduksc_cgen_chnl_update(oduksc_handle, chnl[chnl_itr]);
            if (ret_val != PMC_SUCCESS) {
                PMC_RETURN();
            }
        }
    }
     
    PMC_RETURN();
} /* oduksc_chnl_clean */


/*******************************************************************************
*  oduksc_chnl_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Updates the CGEN standby page with the current state values for specified
*   channel. Optionally updates the SCBS3 standby pages with the current number
*   of slots for all channels (SCBS3 doesn't need to be updated when
*   provisioning a G.HAO ramp). Then, swaps the standby and active pages. Also
*   updates again the CGEN standby page for specified channel, so that the
*   standby and active pages are identical. This allows any other channel to
*   perform a page swaps when it gets configured without affecting active
*   channels.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to update
*                       Valid range: 0 - 95
*   update_scbs3      - 0: Update CGEN page only
*                       1: Update SCBS3 + CGEN pages
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
PUBLIC PMC_ERROR oduksc_chnl_update(oduksc_handle_t *oduksc_handle,
                                    UINT32 chnl,
                                    BOOL update_scbs3)
{
    /* variable declaration */
    UINT32 num_iter;
    UINT32 swap_type;
    PMC_ERROR ret_val = PMC_SUCCESS;  
    oduksc_buffer_t oduksc_buf[1];
    PMC_ENTRY();
 
    if (update_scbs3)
    {
        ret_val = scbs3_page_udpate_and_swap(oduksc_handle->scbs3_handle);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        swap_type = 0x3;
    }
    else
    {
        swap_type = 0x2;
    }

    /* Configure standby page for channel */
    ret_val = oduksc_cgen_chnl_update(oduksc_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    /* Perform Page Swap */
    /* swap pages in SCBS3 and Credit Generator  */
    oduksc_buffer_init(oduksc_buf, oduksc_handle);
    oduksc_field_PAGE_SWAP_TYPE_set(oduksc_buf, oduksc_handle, swap_type);
    oduksc_field_PAGE_SWAP_REQ_set (oduksc_buf, oduksc_handle, 1);
    oduksc_buffer_flush(oduksc_buf);

    
    /* WAIT UNTILL PAGE_SWAP_REQ IS CLEARED. */
    /* poll the PAGE_SWAP_REQ bit for acknowledgment of the swap request */
    ret_val = oduksc_field_PAGE_SWAP_REQ_poll ( NULL, oduksc_handle, 0, PMC_POLL_EQUAL,
                                                ODUKSC_NUM_USEC_ITERATIONS, &num_iter, 1 );
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ODUKSC_PAGE_SWAP_REQ_TIMEOUT);
    }

    /* Configure standby page for channel so that it will be identical to active page
       except for channel reset which will always be cleared for activated channel */
    ret_val = oduksc_cgen_chnl_update(oduksc_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_chnl_update */

/*******************************************************************************
*  oduksc_all_chnls_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Updates the CGEN standby page with the current state values for specified
*   channel. Optionally updates the SCBS3 standby pages with the current number
*   of slots for all channels. Then, swaps the standby and active pages.
*   This function does not configure the new standby page after the page swap.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   *chnl_ids         - pointer to channel IDs to update
*   num_chnls         - number of channels to be updated
*   update_scbs3      - 0: Update CGEN page only
*                       1: Update SCBS3 + CGEN pages
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
PUBLIC PMC_ERROR oduksc_all_chnls_update(oduksc_handle_t *oduksc_handle,
                                         UINT32 *chnl_ids,
                                         UINT32 num_chnls,
                                         BOOL update_scbs3)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;  
    UINT32 i = 0;
    UINT32 num_iter;
    UINT32 swap_type;
    oduksc_buffer_t oduksc_buf[1];


    PMC_ENTRY();

    if (update_scbs3)
    {
        ret_val = scbs3_page_udpate_and_swap(oduksc_handle->scbs3_handle);
        
        if (PMC_SUCCESS == ret_val)
        {
            swap_type = 0x3;
        }
    }
    else
    {
        swap_type = 0x2;
    }

    if (PMC_SUCCESS == ret_val)
    {
        for (i = 0; i < num_chnls && PMC_SUCCESS == ret_val; i++)
        {        
            /* Configure standby page for channel */
            ret_val = oduksc_cgen_chnl_update(oduksc_handle, chnl_ids[i]);
        }
    }

    /* Perform Page Swap */
    if (PMC_SUCCESS == ret_val)
    {
        /* swap pages in SCBS3 and Credit Generator  */
        oduksc_buffer_init(oduksc_buf, oduksc_handle);
        oduksc_field_PAGE_SWAP_TYPE_set(oduksc_buf, oduksc_handle, swap_type);
        oduksc_field_PAGE_SWAP_REQ_set (oduksc_buf, oduksc_handle, 1);
        oduksc_buffer_flush(oduksc_buf);

        /* WAIT UNTILL PAGE_SWAP_REQ IS CLEARED. */
        /* poll the PAGE_SWAP_REQ bit for acknowledgment of the swap request */
        ret_val = oduksc_field_PAGE_SWAP_REQ_poll ( NULL, oduksc_handle, 0, PMC_POLL_EQUAL,
                                                    ODUKSC_NUM_USEC_ITERATIONS, &num_iter, 1 );
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ODUKSC_PAGE_SWAP_REQ_TIMEOUT);
        }

    }

    if (PMC_SUCCESS == ret_val)
    {
        for (i = 0; i < num_chnls && PMC_SUCCESS == ret_val; i++)
        {        
            /* Configure standby page for channel */
            ret_val = oduksc_cgen_chnl_update(oduksc_handle, chnl_ids[i]);
        }
    }
   
    PMC_RETURN(ret_val);

} /* oduksc_all_chnls_update */


/*******************************************************************************
*  oduksc_core_cgen_chnl_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Updates the standby or active pages associated with ODUKSC (SCBS3 and CGEN)
*   with the state-values of activated channels. It then swaps the standby and
*   active pages.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to update
*                       Valid range: 0 - 95
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
PUBLIC PMC_ERROR oduksc_core_cgen_chnl_update(oduksc_handle_t *oduksc_handle,
                                              UINT32 chnl)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 chnl_tmp;
    PMC_ENTRY();

    for (chnl_tmp = 0; chnl_tmp < oduksc_handle->cfg.max_chnl; chnl_tmp++)
    {
        PMC_LOG_TRACE("ODUJAT:ODUSKC_CHANNEL STATE: %u\n", oduksc_handle->var.ch_ctxt_ptr[chnl_tmp].chnl_state);   
    }
   
    /* Update CGEN only, not SCBS3 */
    ret_val = oduksc_chnl_update(oduksc_handle, chnl, FALSE);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    PMC_RETURN(ret_val);
} /* oduksc_core_cgen_chnl_update */


/*******************************************************************************
*  oduksc_page_swap
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Swap the standby and active pages for the credit generator and optionally
*   the SCBS3. It is assumed the credits page is properly configured for all
*   channel. This function is used when multiple channels need to be updated
*   simultaneously with a single page swap. It is also expected that, after the
*   swap, the new standby page is configured the same way as what has become
*   the active page.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   update_scbs3      - 0: Update CGEN page only \n
*                       1: Update SCBS3 + CGEN pages
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
PUBLIC PMC_ERROR oduksc_page_swap(oduksc_handle_t *oduksc_handle,
                                  BOOL update_scbs3)
{
    /* variable declaration */
    UINT32 num_iter;
    UINT32 swap_type;
    PMC_ERROR ret_val;   
    oduksc_buffer_t oduksc_buf[1];
    PMC_ENTRY();

    if (update_scbs3)
    {
        ret_val = scbs3_page_udpate_and_swap(oduksc_handle->scbs3_handle);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        swap_type = 0x3;
    }
    else
    {
        swap_type = 0x2;
    }

    /* Perform Page Swap */
    /* swap pages in SCBS3 and Credit Generator  */
    oduksc_buffer_init(oduksc_buf, oduksc_handle);
    oduksc_field_PAGE_SWAP_TYPE_set(oduksc_buf, oduksc_handle, swap_type);
    oduksc_field_PAGE_SWAP_REQ_set (oduksc_buf, oduksc_handle, 1);
    oduksc_buffer_flush(oduksc_buf);

    
    /* WAIT UNTILL PAGE_SWAP_REQ IS CLEARED. */
    /* poll the PAGE_SWAP_REQ bit for acknowledgment of the swap request */
    ret_val = oduksc_field_PAGE_SWAP_REQ_poll ( NULL, oduksc_handle, 0, PMC_POLL_EQUAL,
                                                ODUKSC_NUM_USEC_ITERATIONS, &num_iter, 1 );
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ODUKSC_PAGE_SWAP_REQ_TIMEOUT);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_page_swap */


/*******************************************************************************
*  oduksc_zone_pushback_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures source FIFO Empty Zone Pushback for a specific channel.
*
* INPUTS:
*   *oduksc_handle   - pointer to ODUKSC handle instance
*   chnl             - channel ID to associate the ramp interval
*                      Valid range: 0 - 95
*   zone_pb_en       - source FIFO Empty Zone Pushback \n
*                      0 -- empty Zone indication from the source FIFO will have
*                           no effect on the RD_CHAN_EN output for the channel \n
*                      1 -- empty Zone indication from the source FIFO will 
*                           squelch the RD_CHAN_EN output for the channel
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
PUBLIC PMC_ERROR oduksc_zone_pushback_cfg(oduksc_handle_t *oduksc_handle, 
                                          UINT32 chnl, 
                                          UINT32 zone_pb_en)
{
    /* variable declaration */

    PMC_ENTRY();

    /* source FIFO Empty Zone Pushback for a specific channel */
    oduksc_field_SRC_ZONE_PB_EN_set( NULL, oduksc_handle, chnl, zone_pb_en);

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_zone_pushback_cfg */

/*******************************************************************************
*  oduksc_zone_pushback_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get FIFO Empty Zone Pushback for a specific channel.
*
* INPUTS:
*   *oduksc_handle   - pointer to ODUKSC handle instance
*   chnl             - channel ID to associate the ramp interval
*                      Valid range: 0 - 95

* OUTPUTS:
*   None.
*
* RETURNS:
*  UINT32            - The FIFO empty zone pushback value
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 oduksc_zone_pushback_cfg_get(oduksc_handle_t *oduksc_handle, 
                                           UINT32 chnl)
{
    /* variable declaration */
    UINT32 result;
    PMC_ENTRY();

    /* source FIFO Empty Zone Pushback for a specific channel */
    result = oduksc_field_SRC_ZONE_PB_EN_get( NULL, oduksc_handle, chnl);

    PMC_RETURN(result);

} /* oduksc_zone_pushback_cfg_get */

/*******************************************************************************
*  oduksc_bmp_mode_cgen_chnl_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Get client rate from the credit generator configuration for the specified 
*    channel. This function assumes that this channel is mapped with BMP
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*
* OUTPUTS:
*   *client_rate      - client data rate 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR oduksc_bmp_mode_cgen_chnl_cfg_get(oduksc_handle_t *oduksc_handle,
                                                   UINT32 chnl,
                                                   DOUBLE *client_rate)
{
    PMC_ERROR rc = PMC_SUCCESS;
    scbs3_page_t actv_page;
    UINT32 cgen_m, cgen_n;
    UINT32 num_cal_entries;

    PMC_ENTRY();
    
    *client_rate = 0;

    /* get number of calendar entries from scbs3 active page */
    scbs3_page_get(oduksc_handle->scbs3_handle, SCBS3_ACTIVE_MEM,&actv_page);
    num_cal_entries = scbs3_num_entries_per_chnl_get(oduksc_handle->scbs3_handle, &actv_page,chnl);
    
    /* Reads from the active page */
    cgen_m   = oduksc_field_CGEN_STATE_RAM_ACTV_DWORD0_get(NULL, oduksc_handle, chnl);
    cgen_n   = oduksc_field_CGEN_STATE_RAM_ACTV_DWORD1_get(NULL, oduksc_handle, chnl);

    if ((cgen_m == 41078125 && cgen_n == 44229888 && num_cal_entries == 8 )||
        (cgen_m == 41078125 && cgen_n == 49758624  && num_cal_entries == 9))
    {
        *client_rate = UTIL_GLOBAL_ODU2E_RATE;
    }
    else if ((cgen_m == 41078125 && cgen_n == 44416512  && num_cal_entries == 8) ||
             (cgen_m == 41078125 &&  cgen_n == 49968576  && num_cal_entries == 9))
    {
        *client_rate  = UTIL_GLOBAL_ODU1E_RATE;
    } 
    else if (cgen_m == 1 && cgen_n == 1)
    {
        *client_rate  = cgen_m;
    }    else
    {
            rc = ODUKSC_INCOHERENT_BMP_CONFIG;
    }

    PMC_RETURN(rc);
} /* oduksc_bmp_mode_cgen_chnl_cfg_get */

/*
** Private Functions
*/

/*******************************************************************************
*  oduksc_scbs3_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Return SCBS3 handle 
*
* INPUTS:
*   hndl              - pointer to ODUKSC handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   scbs3_handle_t    - The scbs3 handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC scbs3_handle_t* oduksc_scbs3_handle_get(oduksc_handle_t *hndl)
{
    PMC_ENTRY();
    PMC_RETURN(hndl->scbs3_handle);    
} /*oduksc_scbs3_handle_get*/

/*******************************************************************************
*  oduksc_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize ODUKSC var_t variables.
*
* INPUTS:
*   *oduksc_handle   - pointer to ODUKSC handle instance
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
PRIVATE void oduksc_var_default_init(oduksc_handle_t *oduksc_handle)
{
    /* variable declaration */
    UINT32 chnl;
    UINT32 i;

    PMC_ENTRY();

    /* Configure default ODUKSC variable parameters for each channel */
    for (chnl = 0; chnl < oduksc_handle->cfg.max_chnl; chnl++) {
        oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state       = ODUKSC_CHNL_START;
        oduksc_handle->var.ch_ctxt_ptr[chnl].scbs3_user_bits  = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_u_d         = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_s           = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m2          = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n2          = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m           = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n           = 0;

        oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_oduflex_gfp_rate = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_oduflex_gfp_rate = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_rate = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_rate = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_cgen_m = 0;
        oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_cgen_n = 0;
    }


    for (i=0; i<ODUKSC_RI_NUM; i++)
    {
        oduksc_handle->var.ri_state_array[i] = RI_AVAILABLE;
    }

    PMC_RETURN();

} /* oduksc_var_default_init */


/*******************************************************************************
*  oduksc_cgen_chnl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Calculates the m and n values to be configured as base credit parameters
*    for the rate in argument. The state variables are updated with m and n.
*    This function doesn't configure the ramp parameters.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   server_type       - HO ODUk rate \n
*                       Refer to util_global_odukp_type_t \n
*   client_type       - LO ODUj rate \n
*                       Refer to util_global_odukp_type_t \n
*   client_rate       - client data rate kbits/sec
*   oduflex_gfp_rate  - ODUflex GFP container rate in number of tribslots
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
PUBLIC PMC_ERROR oduksc_cgen_chnl_cfg(oduksc_handle_t *oduksc_handle,
                                      UINT32 chnl,
                                      util_global_odukp_type_t server_type,
                                      util_global_odukp_type_t client_type,
                                      UINT32 client_rate,
                                      UINT32 oduflex_gfp_rate)
{
    /* variable declaration */
    UINT32 target_cgen_m, target_cgen_n;
    UINT32 rate ;
    UINT32 cm;
    UINT32 time_slot;
    UINT32 num_recs;
    UINT32 num_cal_entries;            
    DOUBLE decimal_n_over_d;
    UINT32 local_cgen_n;
    UINT32 local_cgen_m; 
    BOOL8 use_float32_mode = digi_use_float32_get();

    /* variable initialization */
    target_cgen_m = 0;
    target_cgen_n = 0;
    rate = 0;
    cm = 0;
    time_slot = 0;
    num_recs = 0;
    num_cal_entries = 0;


    PMC_ENTRY();

    oduksc_client_rate_calc(client_type, client_rate, &rate);

    /* if DB being used get the num_cal_entries from DB
       otherwise calculate time_slot based on rate */
    if (oduksc_handle->cfg.db_en == TRUE)
    {
        if(oduksc_handle->cfg.db_id == UTIL_GEN_DB_ODUKSC_DB)
        {
            mapotn_db_key key;

            key.calendar_entry = DO_NOT_USE_KEY;
            key.channel_num = chnl;

            num_recs=gen_db_query_count(&(oduksc_handle->base), oduksc_handle->cfg.db_id, oduksc_handle->cfg.db_handle, (void *)&key);

            if (num_recs == 0)
            {
                PMC_RETURN(ODUKSC_ERR_DB_ENTRY_NOT_FOUND);
            }

        }
        else if(oduksc_handle->cfg.db_id == UTIL_GEN_DB_LINE_CORE_OTN_RX_DB)
        {

            /*only check LO channels for ODUKSC2*/
            if(oduksc_handle->cfg.cal_entries_alloc_method == UTIL_SCHD_ALLOC_METHOD_COREOTN_ODUKSC2)
            {
                line_core_otn_db_key key;

                key.calendar_entry = DO_NOT_USE_KEY;
                key.ho_channel = DO_NOT_USE_KEY;
                key.mo_channel = DO_NOT_USE_KEY;
                key.lo_channel = chnl;
                key.trib_slot = DO_NOT_USE_KEY;

                PMC_LOG_TRACE("UTIL_GEN_DB_LINE_CORE_OTN_RX_DB\n");
                PMC_LOG_TRACE("key.calendar_entry = %u, key.lo_channel = %u\n", key.calendar_entry, key.lo_channel);

                num_recs=gen_db_query_count(&(oduksc_handle->base), oduksc_handle->cfg.db_id, oduksc_handle->cfg.db_handle, (void *)&key);
            }

            /* not two stage - schedule for each HO channel */
            if (num_recs == 0)
            {
                line_core_otn_db_key one_stage_key;

                one_stage_key.calendar_entry = DO_NOT_USE_KEY;
                one_stage_key.ho_channel = DO_NOT_USE_KEY;
                one_stage_key.mo_channel = chnl;
                one_stage_key.lo_channel = DO_NOT_USE_KEY;
                one_stage_key.trib_slot = DO_NOT_USE_KEY;

                num_recs=gen_db_query_count(&(oduksc_handle->base), oduksc_handle->cfg.db_id, oduksc_handle->cfg.db_handle, (void *)&one_stage_key);

                if (num_recs == 0)
                {
                    line_core_otn_db_key no_stage_key;

                    no_stage_key.calendar_entry = DO_NOT_USE_KEY;
                    no_stage_key.ho_channel = chnl;
                    no_stage_key.mo_channel = DO_NOT_USE_KEY;
                    no_stage_key.lo_channel = DO_NOT_USE_KEY;
                    no_stage_key.trib_slot = DO_NOT_USE_KEY;

                    num_recs=gen_db_query_count(&(oduksc_handle->base), oduksc_handle->cfg.db_id, oduksc_handle->cfg.db_handle, (void *)&no_stage_key);

                    if (num_recs == 0)
                    {
                        PMC_RETURN(ODUKSC_ERR_DB_ENTRY_NOT_FOUND);
                    }

                }
            }
        }
        else
        {
            PMC_RETURN(ODUKSC_ERR_INVALID_PARAMETERS);
        } 
        time_slot = num_recs;    
    }
    else 
    {
        PMC_ASSERT((oduksc_handle->cfg.total_rate/oduksc_handle->cfg.max_chnl) != 0, ODUKSC_ERR_DIV_BY_ZERO,0,0);
        /* client rate should be in total_rate_uints which is Kbps*/
        num_cal_entries = rate / (oduksc_handle->cfg.total_rate/oduksc_handle->cfg.max_chnl);
        if (rate % (oduksc_handle->cfg.total_rate/oduksc_handle->cfg.max_chnl) != 0)
        {
            num_cal_entries += 1;
        }
    }
    
    /* NEED TO CLEAN THIS UP ONCE WE KNOW IF LINEOTN ALSO WILL BE USING DB OR NOT*/
    if(client_type==UTIL_GLOBAL_ODU0)
    {
        target_cgen_m = 8;
        target_cgen_n = 9;
        num_cal_entries = 1;
    }
    else if(client_type==UTIL_GLOBAL_ODU1)
    {
        target_cgen_m = 956;
        target_cgen_n = 1071;
        num_cal_entries = 2;
    }
    else if((client_type==UTIL_GLOBAL_ODU1E || client_type==UTIL_GLOBAL_ODU1E_ETRANS) && time_slot == 9)
    {
        target_cgen_m = 41078125;
        target_cgen_n = 49968576;
        num_cal_entries = 9;
    }
    else if(client_type == UTIL_GLOBAL_ODU1F && time_slot == 9)
    {
        target_cgen_m = 1643125;
        target_cgen_n = 1959552;
        num_cal_entries = 9;
    }
    else if(client_type==UTIL_GLOBAL_ODU1E || client_type==UTIL_GLOBAL_ODU1E_ETRANS)
    {
        target_cgen_m = 41078125;
        target_cgen_n = 44416512;
        num_cal_entries = 8;
    }
    else if(client_type == UTIL_GLOBAL_ODU1F)
    {
        target_cgen_m = 1643125;
        target_cgen_n = 1741824;
        num_cal_entries = 8;
    }
    else if(client_type == UTIL_GLOBAL_ODU2)
    {
        target_cgen_m = 1912;
        target_cgen_n = 2133;
        num_cal_entries = 8;
    }
    else if((client_type == UTIL_GLOBAL_ODU2E || client_type == UTIL_GLOBAL_ODU2E_ETRANS) && time_slot == 9)
    {
        target_cgen_m = 41078125;
        target_cgen_n = 49758624;
        num_cal_entries = 9;
    }
    else if(client_type == UTIL_GLOBAL_ODU2F && time_slot == 9)
    {
        target_cgen_m = 27933125;
        target_cgen_n = 33172416;
        num_cal_entries = 9;
    }
    else if(client_type == UTIL_GLOBAL_ODU2E || client_type == UTIL_GLOBAL_ODU2E_ETRANS)
    {
        target_cgen_m = 41078125;
        target_cgen_n = 44229888;
        num_cal_entries = 8;
    }
    else if(client_type == UTIL_GLOBAL_ODU2F)
    {
        target_cgen_m = 27933125;
        target_cgen_n = 29486592;
        num_cal_entries = 8;
    }
    else if(client_type == UTIL_GLOBAL_ODU3 && time_slot == 32)
    {
        target_cgen_m = 478;
        target_cgen_n = 531;
        num_cal_entries = 32;
    }
    else if(client_type == UTIL_GLOBAL_ODU3 && time_slot == 31)
    {
        target_cgen_m = 17499661;
        target_cgen_n = 18832500;
        num_cal_entries = 31;
    }
    else if(client_type == UTIL_GLOBAL_ODU3) 
    {
        target_cgen_m = 478;
        target_cgen_n = 531;
        num_cal_entries = 32;
    }
    else if(client_type == UTIL_GLOBAL_ODU3E1 && time_slot == 33)
    {
        target_cgen_m = 41078125;
        target_cgen_n = 45419616;
        num_cal_entries = 33;
    }
    else if(client_type == UTIL_GLOBAL_ODU3E2 && time_slot == 33)
    {
        target_cgen_m = 183552;
        target_cgen_n = 202895;
        num_cal_entries = 33;
    }
    else if(client_type == UTIL_GLOBAL_ODU3E1)
    {
        target_cgen_m = 41078125;
        target_cgen_n = 44043264;
        num_cal_entries = 32;
    }
    else if(client_type == UTIL_GLOBAL_ODU3E2)
    {
        target_cgen_m = 17208;
        target_cgen_n = 18445;
        num_cal_entries = 32;
    }
    else if(client_type == UTIL_GLOBAL_ODU4)
    {
        target_cgen_m = 1912;
        target_cgen_n = 2043;
        num_cal_entries = 80;
    }
    else if(client_type == UTIL_GLOBAL_ODUFLEX_GFP)
    {
        if(oduksc_handle->cfg.db_en == FALSE)
        {
            time_slot = num_cal_entries;
        }

        oduksc_oduflex_gfp_m_n_get(oduksc_handle, oduflex_gfp_rate, &target_cgen_m, &target_cgen_n);
        if (time_slot > oduflex_gfp_rate)
        {
            /* Decrease m to compensate for the higher number of calendar slots currently used */
            if (!use_float32_mode) {
                target_cgen_m = (UINT32)PMC_ROUND((DOUBLE)target_cgen_m * (DOUBLE)oduflex_gfp_rate / (DOUBLE)time_slot);
            } else {
                target_cgen_m = (UINT32)PMC_ROUND((FLOAT)target_cgen_m * (FLOAT)oduflex_gfp_rate / (FLOAT)time_slot);
            }
        }
    }
    else if(client_type == UTIL_GLOBAL_ODUFLEX_CBR)
    {
        if(oduksc_handle->cfg.db_en == FALSE)
        {
            time_slot = num_cal_entries;
        }

        PMC_ASSERT(time_slot != 0, ODUKSC_ERR_DIV_BY_ZERO,0,0);
        if (!use_float32_mode) {
            target_cgen_m = (UINT32)((DOUBLE)((DOUBLE)(client_rate)*1000) / (DOUBLE)(25*time_slot));
        } else {
            target_cgen_m = (UINT32)((FLOAT)((FLOAT)(client_rate)*1000) / (FLOAT)(25*time_slot));
        }
        target_cgen_n = 55987200;
    }
    else
    {
        if(oduksc_handle->cfg.db_en == FALSE)
        {
            time_slot = num_cal_entries;
        }

        PMC_ASSERT(time_slot != 0, ODUKSC_ERR_DIV_BY_ZERO,0,0);
        if (!use_float32_mode) {
            target_cgen_m = (UINT32)((DOUBLE)((DOUBLE)(client_rate)*1000) / (DOUBLE)(25*time_slot));
        } else {
            target_cgen_m = (UINT32)((FLOAT)((FLOAT)(client_rate)*1000) / (FLOAT)(25*time_slot));
        }
        target_cgen_n = 55987200;
    }


    if(oduksc_handle->cfg.db_en == FALSE)
    {
        time_slot = num_cal_entries;
    }
    else
    {
        time_slot = num_recs;
    }
    PMC_LOG_TRACE("time_slot = %d\n", time_slot);


    if(oduksc_handle->cfg.num_mult != 1 ||  oduksc_handle->cfg.demon_mult != 1)
    {
        /* If there is a multiplier applied on the DCI clock, it's require to rationalize
        the cgen_n and cgen_d*/
        if (!use_float32_mode) {
            decimal_n_over_d =  (target_cgen_m * (DOUBLE)oduksc_handle->cfg.num_mult)/(target_cgen_n * (DOUBLE)oduksc_handle->cfg.demon_mult);
        } else {
            decimal_n_over_d =  (DOUBLE)((target_cgen_m * (FLOAT)oduksc_handle->cfg.num_mult)/(target_cgen_n * (FLOAT)oduksc_handle->cfg.demon_mult));
        }
        /*ODUKSC has 26 bits of precision*/
        util_opsa_find_rational_less_than_one(decimal_n_over_d, 0x3FFFFFF, &local_cgen_m, &local_cgen_n);
    }
    else
    {
        local_cgen_m = target_cgen_m;
        local_cgen_n = target_cgen_n;
    }


    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m   = local_cgen_m;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n   = local_cgen_n;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_u_d = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m2  = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n2  = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_s   = 0;

    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_oduflex_gfp_rate = oduflex_gfp_rate;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_oduflex_gfp_rate = oduflex_gfp_rate;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_rate = rate;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_rate  = rate;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_cgen_m = local_cgen_m;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_cgen_n = local_cgen_n;

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_cgen_chnl_cfg */


/*******************************************************************************
*  oduksc_cgen_cgen_compare
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Calculate local cgen_n and cgen_m and compare them with expected values.
*
* INPUTS:
*   *oduksc_handle      - pointer to ODUKSC handle instance
*    target_cgen_m      - target cgen_m
*    target_cgen_n      - target cgen_n
*    expected_cgen_m    - expected cgen_m
*    expected_cgen_n    - expected cgen_n
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL                 - TRUE when local values match given ones, otherwise FALSE.
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 oduksc_cgen_compare(oduksc_handle_t *oduksc_handle,
                                  UINT32 target_cgen_m,
                                  UINT32 target_cgen_n,
                                  UINT32 expected_cgen_m,
                                  UINT32 expected_cgen_n)
{
    UINT32 local_cgen_n;
    UINT32 local_cgen_m;
    BOOL8 result = FALSE;         
    DOUBLE decimal_n_over_d;

    PMC_ENTRY();

    /* calculate local cgen */
    if(oduksc_handle->cfg.num_mult != 1 ||  oduksc_handle->cfg.demon_mult != 1)
    {
        /* If there is a multiplier applied on the DCI clock, it's require to rationalize
           the cgen_n and cgen_d*/
        decimal_n_over_d =  (target_cgen_m * (DOUBLE)oduksc_handle->cfg.num_mult)/(target_cgen_n * (DOUBLE)oduksc_handle->cfg.demon_mult);
            /*ODUKSC has 26 bits of precision*/
        util_opsa_find_rational_less_than_one(decimal_n_over_d, 0x3FFFFFF, &local_cgen_m, &local_cgen_n);
    }
    else
    {
        local_cgen_m = target_cgen_m;
        local_cgen_n = target_cgen_n;
    }

    /* compare with expected values */
    if (local_cgen_n == expected_cgen_n &&
        local_cgen_m == expected_cgen_m)
    {
        result = TRUE;
    }
    
    PMC_RETURN(result);
}/* oduksc_cgen_compare */

/*******************************************************************************
*  oduksc_cgen_chnl_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Gets Channel Credit generator configuration data.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*
*
* OUTPUTS:
*   server_type       - HO ODUk rate \n
*                       Refer to util_global_odukp_type_t \n
*   client_type       - LO ODUj rate \n
*                       Refer to util_global_odukp_type_t \n
*   client_rate       - client data rate kbits/sec
*   rate_ramp         - rate ramp on/off \n
*                       0 -- rate ramping off \n
*                       1 -- rate ramping on \n
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void oduksc_cgen_chnl_cfg_get(oduksc_handle_t *oduksc_handle,
                                     UINT32 chnl,
                                     util_global_odukp_type_t *server_type,
                                     util_global_odukp_type_t *client_type,
                                     UINT32 *client_rate,
                                     UINT32 *rate_ramp)
{
    UINT32 time_slot;
    UINT32 num_recs;
    UINT32 num_cal_entries;
    UINT32 cgen_m;
    UINT32 cgen_n;
    UINT32 cgen_m2;
    UINT32 cgen_n2;
    UINT32 cgen_s;
    UINT32 cgen_u_d;
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL8 client_type_found;
    
    PMC_ENTRY();

    time_slot = 0;
    num_recs = 0;
    num_cal_entries = 0;

    *client_type =   UTIL_GLOBAL_LAST_ODUK;
    *server_type =   UTIL_GLOBAL_LAST_ODUK;  
    
    /* if DB being used get the num_cal_entries from DB 
       otherwise calculate time_slot based on rate */
    if (oduksc_handle->cfg.db_en == TRUE)
    {
        if(oduksc_handle->cfg.db_id == UTIL_GEN_DB_ODUKSC_DB)
        {
            mapotn_db_key key;
                
            key.calendar_entry = DO_NOT_USE_KEY;
            key.channel_num = chnl;
                
            num_recs=gen_db_query_count(&(oduksc_handle->base), oduksc_handle->cfg.db_id, oduksc_handle->cfg.db_handle, (void *)&key);
            
            if (num_recs == 0)
            {
                PMC_RETURN();
            }
                
        }
        else if(oduksc_handle->cfg.db_id == UTIL_GEN_DB_LINE_CORE_OTN_RX_DB)
        {
            line_core_otn_db_key key;
                
            key.calendar_entry = DO_NOT_USE_KEY;
            key.ho_channel = DO_NOT_USE_KEY;
            key.mo_channel = DO_NOT_USE_KEY;
            key.lo_channel = chnl;
            key.trib_slot = DO_NOT_USE_KEY;
            
            PMC_LOG_TRACE("UTIL_GEN_DB_LINE_CORE_OTN_RX_DB\n");
            PMC_LOG_TRACE("key.calendar_entry = %u, key.lo_channel = %u\n", key.calendar_entry, key.lo_channel);
                           
            num_recs=gen_db_query_count(&(oduksc_handle->base), oduksc_handle->cfg.db_id, oduksc_handle->cfg.db_handle, (void *)&key);
            
            /* not two stage - schedule for each HO channel */
            if (num_recs == 0)
            {
                line_core_otn_db_key one_stage_key;
                
                one_stage_key.calendar_entry = DO_NOT_USE_KEY;
                one_stage_key.ho_channel = DO_NOT_USE_KEY;
                one_stage_key.mo_channel = chnl;
                one_stage_key.lo_channel = DO_NOT_USE_KEY;
                one_stage_key.trib_slot = DO_NOT_USE_KEY;
               
                num_recs=gen_db_query_count(&(oduksc_handle->base), oduksc_handle->cfg.db_id, oduksc_handle->cfg.db_handle, (void *)&one_stage_key);
                
                if (num_recs == 0)
                {
                    line_core_otn_db_key no_stage_key;
                    
                    no_stage_key.calendar_entry = DO_NOT_USE_KEY;
                    no_stage_key.ho_channel = chnl;
                    no_stage_key.mo_channel = DO_NOT_USE_KEY;
                    no_stage_key.lo_channel = DO_NOT_USE_KEY;
                    no_stage_key.trib_slot = DO_NOT_USE_KEY;
               
                    num_recs=gen_db_query_count(&(oduksc_handle->base), oduksc_handle->cfg.db_id, oduksc_handle->cfg.db_handle, (void *)&no_stage_key);

                    if (num_recs == 0)
                    {
                        PMC_RETURN();
                    }                    

                }
            }    
        }
        else
        {
            PMC_RETURN();
        } 
        time_slot = num_recs;    
    }
    
    rc = oduksc_cgen_active_params_get(oduksc_handle,
                                       chnl,
                                       &cgen_m,
                                       &cgen_n,
                                       &cgen_m2,
                                       &cgen_n2,
                                       &cgen_s,
                                       &cgen_u_d);

    client_type_found = FALSE;

    if (TRUE == oduksc_cgen_compare(oduksc_handle, 8, 9, cgen_m, cgen_n))
    {
        *client_type = UTIL_GLOBAL_ODU0;
        client_type_found = TRUE;
    }
    
    if (FALSE == client_type_found &&
        TRUE == oduksc_cgen_compare(oduksc_handle, 956, 1071, cgen_m, cgen_n))
    {
        *client_type = UTIL_GLOBAL_ODU1;
        client_type_found = TRUE;
    }
    
    if (FALSE == client_type_found &&
        TRUE == oduksc_cgen_compare(oduksc_handle, 41078125, 49968576, cgen_m, cgen_n))
    {
        *client_type = UTIL_GLOBAL_ODU1E;
        client_type_found = TRUE;
        num_cal_entries = 9;
    }
    if (FALSE == client_type_found &&
        TRUE == oduksc_cgen_compare(oduksc_handle, 41078125, 44416512, cgen_m, cgen_n))
    {
        *client_type = UTIL_GLOBAL_ODU1E; /* fuzzy can be UTIL_GLOBAL_ODU1E_ETRANS */
        client_type_found = TRUE;
        num_cal_entries = 8;
    }

    if (FALSE == client_type_found &&
        TRUE == oduksc_cgen_compare(oduksc_handle, 41078125, 44416512, cgen_m, cgen_n) &&
        9 == time_slot)
    {
        *client_type = UTIL_GLOBAL_ODU1F;
        client_type_found = TRUE;
        num_cal_entries = 9;
    }
    
    if (FALSE == client_type_found &&
        TRUE == oduksc_cgen_compare(oduksc_handle, 1643125, 1741824, cgen_m, cgen_n))
    {
        *client_type = UTIL_GLOBAL_ODU1F;
        client_type_found = TRUE;
        num_cal_entries = 8;
    }

    if (FALSE == client_type_found &&
        TRUE == oduksc_cgen_compare(oduksc_handle, 1912, 2133, cgen_m, cgen_n))
    {
        *client_type = UTIL_GLOBAL_ODU2;
        client_type_found = TRUE;
        num_cal_entries = 8;
    }

    if (FALSE == client_type_found &&
        TRUE == oduksc_cgen_compare(oduksc_handle, 41078125, 49758624, cgen_m, cgen_n) &&
        9 == time_slot)
    {
        *client_type = UTIL_GLOBAL_ODU2E;
        client_type_found = TRUE;
        num_cal_entries = 8;
    }

    if (FALSE == client_type_found &&
        TRUE == oduksc_cgen_compare(oduksc_handle, 41078125, 44229888, cgen_m, cgen_n))
    {
        *client_type = UTIL_GLOBAL_ODU2E; /* fuzzy can be UTIL_GLOBAL_ODU1E_ETRANS */
        client_type_found = TRUE;
        num_cal_entries = 8;
    }

    if (FALSE == client_type_found &&
        TRUE == oduksc_cgen_compare(oduksc_handle, 27933125, 33172416, cgen_m, cgen_n)&&
        9 == time_slot)
    {
        *client_type = UTIL_GLOBAL_ODU2F;
        client_type_found = TRUE;
        num_cal_entries = 8;
    }

    if (FALSE == client_type_found &&
        TRUE == oduksc_cgen_compare(oduksc_handle, 27933125, 29486592, cgen_m, cgen_n))
    {
        *client_type = UTIL_GLOBAL_ODU2F;
        client_type_found = TRUE;
        num_cal_entries = 8;
    }

    if (FALSE == client_type_found &&
        TRUE == oduksc_cgen_compare(oduksc_handle, 478, 531, cgen_m, cgen_n))
    {
        *client_type = UTIL_GLOBAL_ODU3;
        client_type_found = TRUE;        
        num_cal_entries = 32;
    }

    if (FALSE == client_type_found &&
        TRUE == oduksc_cgen_compare(oduksc_handle, 17499661, 18832500, cgen_m, cgen_n) &&
        31 == time_slot)
    {
        *client_type = UTIL_GLOBAL_ODU3;
        client_type_found = TRUE;
        num_cal_entries = 31;
    }

    if (FALSE == client_type_found &&
        TRUE == oduksc_cgen_compare(oduksc_handle, 41078125, 45419616, cgen_m, cgen_n) &&
        33 == time_slot)
    {
        *client_type = UTIL_GLOBAL_ODU3E1;
        client_type_found = TRUE;
        num_cal_entries = 33;
    }
    if (FALSE == client_type_found &&
        TRUE == oduksc_cgen_compare(oduksc_handle, 41078125, 44043264, cgen_m, cgen_n))
    {
        *client_type = UTIL_GLOBAL_ODU3E1;
        client_type_found = TRUE;
        num_cal_entries = 32;
    }
        
    if (FALSE == client_type_found &&
        TRUE == oduksc_cgen_compare(oduksc_handle, 183552, 202895, cgen_m, cgen_n) &&
        33 == time_slot)
    {
        *client_type = UTIL_GLOBAL_ODU3E2;
        client_type_found = TRUE;
        num_cal_entries = 33;
    }
        
    if (FALSE == client_type_found &&
        TRUE == oduksc_cgen_compare(oduksc_handle, 17208, 18445, cgen_m, cgen_n))
    {
        *client_type = UTIL_GLOBAL_ODU3E2;
        client_type_found = TRUE;
        num_cal_entries = 32;
    }


    if (FALSE == client_type_found &&
        TRUE == oduksc_cgen_compare(oduksc_handle, 1912, 2043, cgen_m, cgen_n))
    {
        *client_type = UTIL_GLOBAL_ODU4;
        client_type_found = TRUE;
        num_cal_entries = 80;
    }

    PMC_RETURN();

} /* oduksc_cgen_chnl_cfg_get */

/*******************************************************************************
*  oduksc_core_cgen_chnl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Configures channel coreotn credit generator. 
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   server_type       - HO ODUk rate \n
*                       Refer to util_global_odukp_type_t \n
*   client_type       - LO ODUj rate \n
*                       Refer to util_global_odukp_type_t \n
*   client_rate       - client data rate kbits/sec if client_type is set to 
*                       ODUFLEX_CBR or ODUFLEX_GFP
*   odu_flex_data     - number of ODU0's in ODUFLEX container if client_type is
*                       set to ODUFLEX_CBR or ODUFLEX_GFP
*   mc_numer          - MC_NUMERATOR value to be used in ODUFLEX N calculation.
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
PRIVATE PMC_ERROR oduksc_core_cgen_chnl_cfg(oduksc_handle_t *oduksc_handle,
                                            UINT32 chnl,
                                            util_global_odukp_type_t server_type,
                                            util_global_odukp_type_t client_type,
                                            UINT32 client_rate,
                                            UINT32 odu_flex_data,
                                            UINT32 mc_numer)
{
    /* variable declaration */
    UINT32 target_cgen_m;
    DOUBLE target_cgen_n;
    UINT32 rate;
    UINT32 oduflex_gfp_target_n[8][10] = ODUFLEX_TARGET_NUMERATOR;
    UINT32 row, col;
    BOOL8 use_float32_mode = digi_use_float32_get();

    /* M is a don't care */
    target_cgen_m = 0;

    PMC_ENTRY();

     oduksc_client_rate_calc(client_type, client_rate, &rate);
   
    switch (client_type)
    {
        case UTIL_GLOBAL_ODU0:
            target_cgen_n = 15206400;
            break;
        case UTIL_GLOBAL_ODU1:
            target_cgen_n = 3785694;
            break;
        case UTIL_GLOBAL_ODU2:
            target_cgen_n = 15079150;
            break;
        case UTIL_GLOBAL_ODU3:
            /*if (server_type == UTIL_GLOBAL_ODU4)
            {
                target_cgen_n = 29092579;
            }
            else {    
                target_cgen_n = 30031049;
            } */
            PMC_ASSERT(rate != 0, ODUKSC_ERR_DIV_BY_ZERO, 0, 0);
            if (!use_float32_mode) {
                target_cgen_n = (mc_numer * odu_flex_data * ((DOUBLE)34993.0 * 10 * 384)) / ((DOUBLE)rate * 96);
            } else {
                target_cgen_n = (DOUBLE)((mc_numer * odu_flex_data * ((FLOAT)34993.0F * 10 * 384)) / ((FLOAT)rate * 96));
            }
            break;
        case UTIL_GLOBAL_ODU4:
            target_cgen_n = 36107247;
            break;  
        case UTIL_GLOBAL_ODUFLEX_GFP:
            row = (odu_flex_data - 1) / 10;
            col = (odu_flex_data - 1) % 10;
            
            if (row > 7 || col > 9)
            {
                PMC_RETURN(ODUKSC_ERR_INVALID_PARAMETERS);
            }
            
            target_cgen_n = oduflex_gfp_target_n[row][col]; 
        break;
        /* overclocked ODU2 and ODU3 rates will use the following formula */ 
        case UTIL_GLOBAL_ODUFLEX_CBR:
        case UTIL_GLOBAL_ODU1E:
        case UTIL_GLOBAL_ODU1E_ETRANS:
        case UTIL_GLOBAL_ODU1F:
        case UTIL_GLOBAL_ODU2E:
        case UTIL_GLOBAL_ODU2E_ETRANS:        
        case UTIL_GLOBAL_ODU2F:
        case UTIL_GLOBAL_ODU3E1:
        case UTIL_GLOBAL_ODU3E2:
            /* (MC_NUMERATOR * Timeslots * 349.92e6 * 384) / (ODUk/flex rate in bps * 96) */
            PMC_ASSERT(rate != 0, ODUKSC_ERR_DIV_BY_ZERO, 0, 0);
            if (!use_float32_mode) {
                target_cgen_n = (mc_numer * odu_flex_data * ((DOUBLE)34993.0 * 10 * 384)) / ((DOUBLE)rate * 96);
            } else {
                target_cgen_n = (DOUBLE)((mc_numer * odu_flex_data * ((FLOAT)34993.0F * 10 * 384)) / ((FLOAT)rate * 96));
            }
            break;
        default:
            PMC_RETURN(ODUKSC_ERR_INVALID_PARAMETERS);
    }
    
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m   = target_cgen_m;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n   = target_cgen_n;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_u_d = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m2  = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n2  = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_s   = 0;

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_core_cgen_chnl_cfg */


/*******************************************************************************
*  oduksc_bmp_mode_cgen_chnl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Configures channel credit generator for BMP mode.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   server_type       - HO ODUk rate \n
*                       Refer to util_global_odukp_type_t \n
*   client_type       - LO ODUj rate \n
*                       Refer to util_global_odukp_type_t \n
*   client_rate       - client data rate kbits/sec
*   rate_ramp         - rate ramp on/off \n
*                       0 -- rate ramping off \n
*                       1 -- rate ramping on \n
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
PRIVATE PMC_ERROR oduksc_bmp_mode_cgen_chnl_cfg(oduksc_handle_t *oduksc_handle,
                                                UINT32 chnl,
                                                util_global_odukp_type_t server_type,
                                                util_global_odukp_type_t client_type,
                                                UINT32 client_rate,
                                                UINT32 rate_ramp)
{
    /* variable declaration */
    UINT32 target_cgen_m, target_cgen_n;
    UINT32 rate ;
    UINT32 cm;
    UINT32 time_slot;
    UINT32 num_recs;
    UINT32 num_cal_entries;
    DOUBLE decimal_n_over_d;

    /* variable initialization */
    target_cgen_m = 0;
    target_cgen_n = 0;
    rate = 0;
    cm = 0;
    time_slot = 0;
    num_recs = 0;
    num_cal_entries = 0;

    PMC_ENTRY();
    
    oduksc_client_rate_calc(client_type, client_rate, &rate);
    
    /* if DB being used get the num_cal_entries from DB 
       otherwise calculate time_slot based on rate */
    if (oduksc_handle->cfg.db_en == TRUE)
    {
        if(oduksc_handle->cfg.db_id == UTIL_GEN_DB_ODUKSC_DB)
        {
            mapotn_db_key key;
                
            key.calendar_entry = DO_NOT_USE_KEY;
            key.channel_num = chnl;
                
            num_recs=gen_db_query_count(&(oduksc_handle->base), oduksc_handle->cfg.db_id, oduksc_handle->cfg.db_handle, (void *)&key);
            
            if (num_recs == 0)
            {
                PMC_RETURN(ODUKSC_ERR_DB_ENTRY_NOT_FOUND);
            }
        }
        else
        {
            PMC_RETURN(ODUKSC_ERR_INVALID_PARAMETERS);
        } 
        time_slot = num_recs;    
    }
    else 
    {
        PMC_ASSERT((oduksc_handle->cfg.total_rate/oduksc_handle->cfg.max_chnl) != 0, ODUKSC_ERR_DIV_BY_ZERO,0,0);
        /* client rate should be in total_rate_uints which is Kbps*/
        num_cal_entries = rate / (oduksc_handle->cfg.total_rate/oduksc_handle->cfg.max_chnl);
        if (rate % (oduksc_handle->cfg.total_rate/oduksc_handle->cfg.max_chnl) != 0)
        {
            num_cal_entries += 1;
        }
        time_slot = num_cal_entries;
    }
    
    PMC_LOG_TRACE("time_slot = %d\n", time_slot);

    PMC_ASSERT(time_slot != 0, ODUKSC_ERR_DIV_BY_ZERO,0,0);
    if(client_type != UTIL_GLOBAL_ODU1E_ETRANS && client_type != UTIL_GLOBAL_ODU2E_ETRANS)
    {
        target_cgen_m = 1;
        target_cgen_n = 1;        

    } else {
        if(client_type == UTIL_GLOBAL_ODU2E_ETRANS  && time_slot == 8)
        {
            target_cgen_m = 41078125;
            target_cgen_n = 44229888;
        }
        else if(client_type == UTIL_GLOBAL_ODU2E_ETRANS  && time_slot == 9)
        {
            target_cgen_m = 41078125;
            target_cgen_n = 49758624;
        }
        else if(client_type == UTIL_GLOBAL_ODU1E_ETRANS  && time_slot == 8)
        {
            target_cgen_m = 41078125;
            target_cgen_n = 44416512;
        }
        else if(client_type == UTIL_GLOBAL_ODU1E_ETRANS  && time_slot == 9)
        {
            target_cgen_m = 41078125;
            target_cgen_n = 49968576;
        }
        else
        {
            PMC_ASSERT(FALSE, ODUKSC_ERR_INVALID_PARAMETERS, 0, 0);
        }

    }

    
    PMC_LOG_TRACE("target_cgen_m:%d, target_cgen_n:%d\n", target_cgen_m, target_cgen_n);
    

    if(oduksc_handle->cfg.num_mult != 1 ||  oduksc_handle->cfg.demon_mult != 1)
    {
        /* If there is a multiplier applied on the DCI clock, it's require to rationalize
        the cgen_n and cgen_d*/
        decimal_n_over_d =  (target_cgen_m * (DOUBLE)oduksc_handle->cfg.num_mult)/(target_cgen_n * (DOUBLE)oduksc_handle->cfg.demon_mult);
        /*ODUKSC has 26 bits of precision*/
        util_opsa_find_rational_less_than_one(decimal_n_over_d, 0x3FFFFFF, &target_cgen_m, &target_cgen_n);
    }

    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m   = target_cgen_m;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n   = target_cgen_n;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_u_d = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m2  = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n2  = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_s   = 0;

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_bmp_mode_cgen_chnl_cfg */


/*******************************************************************************
*  oduksc_cgen_chnl_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Updates the credit generator standby page for the specified channel. The
*   context is copied to the standby page and the reset is controlled by the
*   channel state.
*
* INPUTS:
*   *oduksc_handle   - pointer to ODUKSC handle instance
*   chnl             - channel ID to be configured
*                      Valid range: 0 - 95
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
PRIVATE PMC_ERROR oduksc_cgen_chnl_update(oduksc_handle_t *oduksc_handle,
                                          UINT32 chnl)
{
    /* variable declaration */

    PMC_ENTRY();


    if(oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state == ODUKSC_CHNL_PROVISION_PENDING ||
       oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state == ODUKSC_CHNL_PROVISIONED)
    {
        /* Update the standby pages for the channels that are being
           provisioned with CHAN_RESET set to 1 */
        oduksc_cgen_standby_params_set(oduksc_handle,
                                       chnl,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m2,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n2,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_s,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_u_d,
                                       1);

        /* Update the channel state */
        oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state = ODUKSC_CHNL_PROVISIONED;
    }
    else if(oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state == ODUKSC_CHNL_DEPROVISION_PENDING)
    {
        /* Update the standby pages for the channels that are being
           deprovisioned with CHAN_RESET set to 1 */
        oduksc_cgen_standby_params_set(oduksc_handle,
                                       chnl,
                                       0,
                                       0,
                                       0,
                                       0,
                                       0,
                                       0,
                                       1);
        /* Update the channel state */
        oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state = ODUKSC_CHNL_START;
    }
    else if(oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state == ODUKSC_CHNL_REPROV_PENDING)
    {
        /* Update the standby pages for the channels that are being
           reprovisioned with CHAN_RESET set to 1 */
        oduksc_cgen_standby_params_set(oduksc_handle,
                                       chnl,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m2,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n2,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_s,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_u_d,
                                       1);

        /* Update the channel state */
        oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state = ODUKSC_CHNL_ACTIVATED;
    }
    else if(oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state == ODUKSC_CHNL_REPROV_NO_RST_PENDING)
    {
        /* Update the standby pages for the channels that are being
           reprovisioned with CHAN_RESET set to 0 */
        oduksc_cgen_standby_params_set(oduksc_handle,
                                       chnl,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m2,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n2,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_s,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_u_d,
                                       0);

        /* Update the channel state */
        oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state = ODUKSC_CHNL_ACTIVATED;
    }
    else
    {
        /* Update the standby pages for the channels that are already active
           with CHAN_RESET set to 0 */
        oduksc_cgen_standby_params_set(oduksc_handle,
                                       chnl,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m2,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n2,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_s,
                                       oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_u_d,
                                       0);

        /* Don't change the channel state */
    }

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_cgen_chnl_update */


/*******************************************************************************
*  oduksc_cgen_standby_params_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the standby page credit generator parameters.
*
* INPUTS:
*   *oduksc_handle   - pointer to ODUKSC handle instance
*   chnl             - channel ID to be configured
*                      Valid range: 0 - 95
*   cgen_m           - base credit generator numerator
*   cgen_n           - base credit generator denominator
*   cgen_m2          - ramp credit generator end numerator
*   cgen_n2          - ramp credit generator denominator
*   cgen_s           - ramp credit generator numerator increment (ramp step)
*   cgen_u_d         - ramp credit generator up/down flag \n
*                      0 -- ramp down \n
*                      1 -- ramp up \n
*   chnl_reset       - channel reset request, this should be set to 1 if any
*                      part of the state vector is being changed
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
PRIVATE PMC_ERROR oduksc_cgen_standby_params_set(oduksc_handle_t *oduksc_handle,
                                                 UINT32 chnl,
                                                 UINT32 cgen_m,
                                                 UINT32 cgen_n,
                                                 UINT32 cgen_m2,
                                                 UINT32 cgen_n2,
                                                 UINT32 cgen_s,
                                                 UINT32 cgen_u_d,
                                                 UINT32 chnl_reset)
{
    /* variable declaration */
    UINT32 write_data[5];

    PMC_ENTRY();

    if (cgen_m > ODUKSC_CGEN_MAX_VALUE ||
        cgen_n > ODUKSC_CGEN_MAX_VALUE ||
        cgen_m2 > ODUKSC_CGEN_MAX_VALUE ||
        cgen_n2 > ODUKSC_CGEN_MAX_VALUE ||
        cgen_s > ODUKSC_CGEN_MAX_VALUE ||
        cgen_u_d > 1 || chnl_reset > 1)
    {
        PMC_RETURN(ODUKSC_ERR_INVALID_PARAMETERS);
    }

    write_data[0] = cgen_m;
    write_data[1] = cgen_n;
    write_data[2] = cgen_m2;
    write_data[3] = cgen_n2;
    write_data[4] = cgen_s | (cgen_u_d << 26) | (chnl_reset << 27);

    oduksc_burst_write( NULL,
                        oduksc_handle,
                        MEM_TYPE_STATUS,
                        PMC_ODUKSC_CORE_REG_CGEN_STATE_RAM_STBY_DWORD0_ACCESS(chnl),
                        5,
                        write_data );

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_cgen_standby_params_set */


/*******************************************************************************
*  oduksc_cgen_active_params_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets the active page credit generator parameters.
*
*
* INPUTS:
*   *oduksc_handle   - pointer to ODUKSC handle instance
*   chnl             - channel ID to be configured
*                      Valid range: 0 - 95
*
* OUTPUTS:
*   *cgen_m          - pointer to base credit generator numerator
*   *cgen_n          - pointer to base credit generator denominator
*   *cgen_m2         - pointer to ramp credit generator end numerator
*   *cgen_n2         - pointer to ramp credit generator denominator
*   *cgen_s          - pointer to ramp credit generator numerator increment
*                      (ramp step)
*   *cgen_u_d        - pointer to ramp credit generator up/down flag
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR oduksc_cgen_active_params_get(oduksc_handle_t *oduksc_handle,
                                                UINT32 chnl,
                                                UINT32 *cgen_m,
                                                UINT32 *cgen_n,
                                                UINT32 *cgen_m2,
                                                UINT32 *cgen_n2,
                                                UINT32 *cgen_s,
                                                UINT32 *cgen_u_d)
{
    /* variable declaration */

    PMC_ENTRY();

    *cgen_m   = oduksc_field_CGEN_STATE_RAM_ACTV_DWORD0_get(NULL, oduksc_handle, chnl);
    *cgen_n   = oduksc_field_CGEN_STATE_RAM_ACTV_DWORD1_get(NULL, oduksc_handle, chnl);
    *cgen_m2  = oduksc_field_CGEN_STATE_RAM_ACTV_DWORD2_get(NULL, oduksc_handle, chnl);
    *cgen_n2  = oduksc_field_CGEN_STATE_RAM_ACTV_DWORD3_get(NULL, oduksc_handle, chnl);
    *cgen_s   = oduksc_field_range_CGEN_STATE_RAM_ACTV_DWORD4_get(NULL, oduksc_handle, chnl, 0, 25 );
    *cgen_u_d = oduksc_field_range_CGEN_STATE_RAM_ACTV_DWORD4_get(NULL, oduksc_handle, chnl, 26, 26);

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_cgen_active_params_get */


/*******************************************************************************
*  oduksc_client_rate_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Calculates the client rate based on the inputs rates.
*
* INPUTS:
*   client_type       - LO ODUj rate \n
*                       Refer to util_global_odukp_type_t \n
*   client_rate       - client data rate kbits/sec
*
* OUTPUTS:
*   *rate             - pointer to the calculated rate
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR oduksc_client_rate_calc(util_global_odukp_type_t client_type,
                                          UINT32 client_rate,
                                          UINT32 *rate)
{
    /* variable declaration */
    UINT32 cal_rate;

    PMC_ENTRY();

    if (client_type == UTIL_GLOBAL_ODUFLEX_CBR 
        || client_type == UTIL_GLOBAL_ODUFLEX_GFP)
    {
        cal_rate = client_rate;
    }
    else if (client_type == UTIL_GLOBAL_ODU0)
    {
        cal_rate = (UINT32)(UTIL_GLOBAL_ODU0_RATE/1000);
    }
    else if (client_type == UTIL_GLOBAL_ODU1)
    {
        cal_rate = (UINT32)(UTIL_GLOBAL_ODU1_RATE/1000);
    }
    else if(client_type == UTIL_GLOBAL_ODU1E || client_type == UTIL_GLOBAL_ODU1E_ETRANS)
    {
        cal_rate = (UINT32)(UTIL_GLOBAL_ODU1E_RATE/1000);
    }
    else if(client_type == UTIL_GLOBAL_ODU1F)
    {
        cal_rate = (UINT32)(UTIL_GLOBAL_ODU1F_RATE/1000);
    }
    else if(client_type == UTIL_GLOBAL_ODU2)
    {
        cal_rate = (UINT32)(UTIL_GLOBAL_ODU2_RATE/1000);
    }
    else if(client_type == UTIL_GLOBAL_ODU2E || client_type == UTIL_GLOBAL_ODU2E_ETRANS)
    {
        cal_rate = (UINT32)(UTIL_GLOBAL_ODU2E_RATE/1000);
    }
    else if(client_type == UTIL_GLOBAL_ODU2F)
    {
        cal_rate = (UINT32)(UTIL_GLOBAL_ODU2F_RATE/1000);
    }
    else if(client_type == UTIL_GLOBAL_ODU3)
    {
        cal_rate = (UINT32)(UTIL_GLOBAL_ODU3_RATE/1000);
    }
    else if(client_type == UTIL_GLOBAL_ODU3E1)
    {
        cal_rate = (UINT32)(UTIL_GLOBAL_ODU3E1_RATE/1000);
    }
    else if(client_type == UTIL_GLOBAL_ODU3E2)
    {
        cal_rate = (UINT32)(UTIL_GLOBAL_ODU3E2_RATE/1000);
    }
    else if (client_type == UTIL_GLOBAL_ODU4)
    {
        cal_rate = (UINT32)(UTIL_GLOBAL_ODU4_RATE/1000);
    }
    else
    {
        PMC_RETURN(ODUKSC_ERR_INVALID_PARAMETERS);
    }

    *rate = cal_rate;

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_client_rate_calc */


/*******************************************************************************
*  oduksc_oduflex_gfp_m_n_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get the m and n credit generator values for the ODUflex(GFP) number of
*   tributary slots.
*
* INPUTS:
*   *oduksc_handle   - pointer to ODUKSC handle instance
*   tribslots        - number of tributary slots
*
*
* OUTPUTS:
*   *cgen_m          - pointer to credit generator m value
*   *cgen_n          - pointer to credit generator n value
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR oduksc_oduflex_gfp_m_n_get(oduksc_handle_t *oduksc_handle,
                                            UINT32 tribslots,
                                            UINT32 *cgen_m,
                                            UINT32 *cgen_n)
{
    /* variable declaration */

    PMC_ENTRY();

    if (tribslots >= 1 && tribslots <= 8)
    {
        /* 1.24917723 Gb/s per TS*/
        *cgen_m = 55518988; /* 13879747*4 */
        *cgen_n = 62208000; /* 15552000*4 */
    }
    else if (tribslots <= 32)
    {
        /* 1.25447054 Gb/s per TS*/
        *cgen_m = 60146661;
        *cgen_n = 67108863;
    }
    else if (tribslots <= 80)
    {
        /* 1.301467133 Gb/s per TS*/
        *cgen_m = 62399963;
        *cgen_n = 67108863;
    }
    else
    {
        PMC_RETURN(ODUKSC_ERR_INVALID_PARAMETERS);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_oduflex_gfp_m_n_get */


/*******************************************************************************
*  oduksc_adjust_dci_clk
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function updates the context's numerator and denomination multiplier.
*   This can be used if there is a change to the reference clk frequency.
*
* INPUTS:
*   *oduksc_handle   - pointer to ODUKSC handle instance
*   num_mult         - multiplier for the numerator
*   denom_mult       - multiplier for the denominator
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
PUBLIC PMC_ERROR oduksc_adjust_dci_clk(oduksc_handle_t *oduksc_handle,
                                       UINT32 num_mult,
                                       UINT32 denom_mult)
{

    PMC_ENTRY();

    oduksc_handle->cfg.num_mult = num_mult;
    oduksc_handle->cfg.demon_mult = denom_mult;
   
    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_adjust_dci_clk */

/*******************************************************************************
*  oduksc_chnl_scbs3_cgen_reprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reprovision the specified ODUFLEX channel for a specific rate in G.HAO. No
*   ramp is configured. The scbs3 is configured with the number of calendar
*   slots corresponding to the new rate. The credit generator is configured so
*   that the rate doesn't change despite the new number of calendar slots.
*   In G.HAO ODUflex resizing, the functions are called in the following order:
*   Ramp-up:
*   1) oduksc_chnl_scbs3_cgen_reprov
*   2) oduksc_chnl_cgen_ramp_prov
*   3) oduksc_chnl_cgen_ramp_start
*   4) oduksc_chnl_cgen_ramp_end_prov
*   5) oduksc_ramp_done_isr
*   Ramp-down:
*   1) oduksc_chnl_cgen_ramp_prov
*   2) oduksc_chnl_cgen_ramp_start
*   3) oduksc_chnl_cgen_ramp_end_prov
*   4) oduksc_ramp_done_isr
*   5) oduksc_chnl_scbs3_cgen_reprov
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   client_type       - LO ODUj rate \n
*                       Refer to util_global_odukp_type_t \n
*   oduflex_gfp_rate  - ODUflex GFP container rate in number of tribslots
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
PUBLIC PMC_ERROR oduksc_chnl_scbs3_cgen_reprov(oduksc_handle_t *oduksc_handle,
                                               UINT32 chnl,
                                               util_global_odukp_type_t client_type,
                                               UINT32 oduflex_gfp_rate)
{

    /* variable declaration */
    DOUBLE oduflex_rate_bps;
    UINT32 oduflex_rate_kbps;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state != ODUKSC_CHNL_ACTIVATED)
    {
        /* channel is not in activated or provisioned state */
        PMC_RETURN(ODUKSC_CHNL_STATE_NOT_ACTIVATED);
    }

    if(client_type != UTIL_GLOBAL_ODUFLEX_GFP)
    {
        /* channel is not provisioned in ODUFLEX GFP */
        PMC_RETURN(ODUKSC_ERR_INVALID_PARAMETERS);
    }


    /* Get final oduflex rate */
    ret_val = oduksc_oduflex_gfp_rate_get(oduksc_handle, oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_oduflex_gfp_rate, &oduflex_rate_bps);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    oduflex_rate_kbps = (UINT32)PMC_ROUND(oduflex_rate_bps / 1000);


    PMC_LOG_TRACE("%s, %s, %d,  before scbs3 \n", __FILE__, __FUNCTION__, __LINE__ );

    /* Reprovision SCBS3 */
    ret_val = scbs3_chnl_reprov(oduksc_handle->scbs3_handle, chnl, oduflex_rate_kbps, oduksc_handle->cfg.total_rate_units);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    PMC_LOG_TRACE("%s, %s, %d,  after scbs3 \n", __FILE__, __FUNCTION__, __LINE__ );

    /* Reprovision credit generator with current rate, so the credits will be ajusted with the new number of slots in scbs3 */
    ret_val = oduksc_cgen_chnl_cfg(oduksc_handle, chnl, UTIL_GLOBAL_ODUK_DONT_CARE, client_type, oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_rate, oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_oduflex_gfp_rate);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    PMC_LOG_TRACE("G.HAO: Reprovision calendar/credits from %d to %d TS; Rate: %d kb/s\n", oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_oduflex_gfp_rate, oduflex_gfp_rate, oduflex_rate_kbps);
    PMC_LOG_TRACE("cgen_m  = %d\n", oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m);
    PMC_LOG_TRACE("cgen_n  = %d\n", oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n);
    PMC_LOG_TRACE("cgen_m2 = %d\n", oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m2);
    PMC_LOG_TRACE("cgen_n2 = %d\n", oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n2);
    PMC_LOG_TRACE("cgen_s  = %d\n", oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_s);

    /* Update channel state */
    oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state = ODUKSC_CHNL_REPROV_PENDING;

    /* Apply the new configuration */
    ret_val = oduksc_chnl_update(oduksc_handle, chnl, TRUE);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_chnl_scbs3_cgen_reprov */


/*******************************************************************************
*  oduksc_chnl_cgen_ramp_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provision the ramp configuration for the specified ODUFLEX channel in G.HAO.
*   The ramp is not started yet. The scbs3 and the current rate are not
*   modified. The credit generator and ramp modulo counter are configured so
*   that the ramp is ready to be started.
*   In G.HAO ODUflex resizing, the functions are called in the following order:
*   Ramp-up:
*   1) oduksc_chnl_scbs3_cgen_reprov
*   2) oduksc_chnl_cgen_ramp_prov
*   3) oduksc_chnl_cgen_ramp_start
*   4) oduksc_chnl_cgen_ramp_end_prov
*   5) oduksc_ramp_done_isr
*   Ramp-down:
*   1) oduksc_chnl_cgen_ramp_prov
*   2) oduksc_chnl_cgen_ramp_start
*   3) oduksc_chnl_cgen_ramp_end_prov
*   4) oduksc_ramp_done_isr
*   5) oduksc_chnl_scbs3_cgen_reprov
*
* INPUTS:
*   *oduksc_handle          - pointer to ODUKSC handle instance
*   chnl                    - channel ID to be configured
*                             Valid range: 0 - 95
*   client_type             - LO ODUj rate \n
*                             Refer to util_global_odukp_type_t \n
*   oduflex_gfp_rate_final  - ODUflex GFP container rate in number of
*                             tribslots after ramp
*   end_for_bwr_ind_clear   - If set, the ramp will end 250us before the target
*                             rate is reached to allow clearing BWR_IND
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
PUBLIC PMC_ERROR oduksc_chnl_cgen_ramp_prov(oduksc_handle_t *oduksc_handle,
                                            UINT32 chnl,
                                            util_global_odukp_type_t client_type,
                                            UINT32 oduflex_gfp_rate_final,
                                            BOOL end_for_bwr_ind_clear)
{

    /* variable declaration */
    DOUBLE oduflex_rate_final_bps;
    DOUBLE oduflex_rate_final_ramp;
    UINT32 cgen_m;
    UINT32 cgen_n;
    UINT32 cgen_m2;
    UINT32 cgen_n2;
    UINT32 cgen_s;
    UINT32 cgen_u_d;
    UINT32 current_rate;
    UINT32 is_ramp_up;
    UINT32 target_rate;
    UINT32 target_cgen_m;
    UINT32 target_cgen_n;
    UINT32 overspeed_factor;
    DOUBLE ramp_rate;
    UINT32 ri_id;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state != ODUKSC_CHNL_ACTIVATED)
    {
        /* channel is not in activated or provisioned state */
        PMC_RETURN(ODUKSC_CHNL_STATE_NOT_ACTIVATED);
    }

    if(client_type != UTIL_GLOBAL_ODUFLEX_GFP)
    {
        /* channel is not provisioned in ODUFLEX GFP */
        PMC_RETURN(ODUKSC_ERR_INVALID_PARAMETERS);
    }

    /* Set useful variables */
    overspeed_factor = 1;
    ramp_rate = (DOUBLE)ODUKSC_RAMP_RATE * (DOUBLE)overspeed_factor * 125e-6 / 8;

    /* Get final oduflex rate */
    ret_val = oduksc_oduflex_gfp_rate_get(oduksc_handle, oduflex_gfp_rate_final, &oduflex_rate_final_bps);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    target_rate = (UINT32)PMC_ROUND(oduflex_rate_final_bps / 1000);

    /* Get final m/n values */
    ret_val = oduksc_oduflex_gfp_m_n_get(oduksc_handle, oduflex_gfp_rate_final, &target_cgen_m, &target_cgen_n);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    /* Check if we ramp up or down */
    if (target_rate > oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_rate)
    {
        /* Ramp-up */
        is_ramp_up = 1;
    }
    else if (target_rate < oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_rate)
    {
        /* Ramp-down */
        is_ramp_up = 0;
    }
    else
    {
        PMC_RETURN(ODUKSC_ERR_INVALID_PARAMETERS);
    }

    if (end_for_bwr_ind_clear)
    {
        /* Two ramp: end some intervals before the final rate for BWR indicator clear */
        if (is_ramp_up)
        {
            /* Ramp-up: end 128kbps less that final rate */
            oduflex_rate_final_ramp = oduflex_rate_final_bps - ODUKSC_RAMP_RATE * 250E-6;
        }
        else
        {
            /* Ramp-up: end 128kbps more that final rate */
            oduflex_rate_final_ramp = oduflex_rate_final_bps + ODUKSC_RAMP_RATE * 250E-6;
        }
    }
    else
    {
        /* Single ramp */
        oduflex_rate_final_ramp = oduflex_rate_final_bps;
    }

    /* Retrieve m and n */
    cgen_m = oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m;
    cgen_n = oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n;

    /* Set m2, n2, s (m,n stay constant) */
    if (is_ramp_up)
    {
        /* s(ramp#1) = overspeed_ratio*floor[ ramp_rate*N2_max*96/(384*349.92E6*t(final)*overspeed_ratio) ] */
        cgen_s = overspeed_factor * (UINT32)(ramp_rate * (DOUBLE)ODUKSC_RAMP_N2_MAX * (DOUBLE)ODUKSC_NUM_CHNL / ((DOUBLE)ODUKSC_BUS_WIDTH * (DOUBLE)ODUKSC_CLOCK_SPEED * (DOUBLE)oduflex_gfp_rate_final * overspeed_factor));
        /* N2(ramp#1) = s(ramp#1)*384*349.92E6*t(final)/(96*ramp_rate) */
        cgen_n2 = (UINT32)((DOUBLE)cgen_s * (DOUBLE)ODUKSC_BUS_WIDTH * (DOUBLE)ODUKSC_CLOCK_SPEED * (DOUBLE)oduflex_gfp_rate_final / ((DOUBLE)ODUKSC_NUM_CHNL * ramp_rate));
        /* M2(ramp#1) = PMC_ROUND[ N2(ramp#1)* (rate(final)-128kbps)*96/(384*349.92E6*t(final)) - M(ramp)/N(ramp)) ] */
        cgen_m2 = (UINT32)PMC_ROUND((DOUBLE)cgen_n2 * ((DOUBLE)oduflex_rate_final_ramp * (DOUBLE)ODUKSC_NUM_CHNL / ((DOUBLE)ODUKSC_BUS_WIDTH * (DOUBLE)ODUKSC_CLOCK_SPEED * (DOUBLE)oduflex_gfp_rate_final) - (DOUBLE)cgen_m / (DOUBLE)cgen_n));
    }
    else
    {
        /* s(ramp#1) = overspeed_ratio*floor[ ramp_rate*N2_max*96/(384*349.92E6*t(initial)*overspeed_ratio) ] */
        cgen_s = overspeed_factor * (UINT32)(ramp_rate * (DOUBLE)ODUKSC_RAMP_N2_MAX * (DOUBLE)ODUKSC_NUM_CHNL / ((DOUBLE)ODUKSC_BUS_WIDTH * (DOUBLE)ODUKSC_CLOCK_SPEED * (DOUBLE)oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_oduflex_gfp_rate * overspeed_factor));
        /* N2(ramp#1) = s(ramp#1)*384*349.92E6*t(initial)/(96*ramp_rate) */
        cgen_n2 = (UINT32)((DOUBLE)cgen_s * (DOUBLE)ODUKSC_BUS_WIDTH * (DOUBLE)ODUKSC_CLOCK_SPEED * (DOUBLE)oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_oduflex_gfp_rate / ((DOUBLE)ODUKSC_NUM_CHNL * ramp_rate));
        /* M2(ramp#1) = -PMC_ROUND[ N2(ramp#1)* (rate(final)-128kbps)*96/(384*349.92E6*t(initial)) - M(ramp)/N(ramp)) ] */
        cgen_m2 = (UINT32)(-PMC_ROUND((DOUBLE)cgen_n2 * ((DOUBLE)oduflex_rate_final_ramp * (DOUBLE)ODUKSC_NUM_CHNL / ((DOUBLE)ODUKSC_BUS_WIDTH * (DOUBLE)ODUKSC_CLOCK_SPEED * (DOUBLE)oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_oduflex_gfp_rate) - (DOUBLE)cgen_m / (DOUBLE)cgen_n)));
        /* Decrease m to compensate for the higher number of calendar slots currently used */
        target_cgen_m = (UINT32)PMC_ROUND((DOUBLE)target_cgen_m * (DOUBLE)oduflex_gfp_rate_final / (DOUBLE)oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_oduflex_gfp_rate);
    }
    /* ramp-up/down */
    cgen_u_d = is_ramp_up;
    
    if (oduksc_handle->cfg.db_id == UTIL_GEN_DB_LINE_CORE_OTN_RX_DB)
    {
        /* In COREOTN, S is 8X larger to compensate the 125us pulse intervals */
        cgen_s = cgen_s * 8;
    }

    /* Save context */
    current_rate = oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_rate;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m  = cgen_m;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n  = cgen_n;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m2 = cgen_m2;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n2 = cgen_n2;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_s = cgen_s;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_u_d = cgen_u_d;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_oduflex_gfp_rate = oduflex_gfp_rate_final;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_rate = current_rate;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_rate = target_rate;;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_cgen_m = target_cgen_m;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_cgen_n = target_cgen_n;

    if (is_ramp_up)
    {
        PMC_LOG_TRACE("G.HAO: Ramp-up to %d TS; Rate: %d -> %d kb/s\n", oduflex_gfp_rate_final, current_rate, target_rate);
    }
    else
    {
        PMC_LOG_TRACE("G.HAO: Ramp-down to %d TS; Rate: %d -> %d kb/s\n", oduflex_gfp_rate_final, current_rate, target_rate);
    }
    PMC_LOG_TRACE("cgen_m  = %d\n", cgen_m);
    PMC_LOG_TRACE("cgen_n  = %d\n", cgen_n);
    PMC_LOG_TRACE("cgen_m2 = %d\n", cgen_m2);
    PMC_LOG_TRACE("cgen_n2 = %d\n", cgen_n2);
    PMC_LOG_TRACE("cgen_s  = %d\n", cgen_s);

    /* Update channel state */
    oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state = ODUKSC_CHNL_REPROV_PENDING;

    /* Apply the new configuration */
    ret_val = oduksc_chnl_update(oduksc_handle, chnl, FALSE);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    /* Get the available ramp interval pulse generator */
    ret_val = oduksc_ri_get(oduksc_handle, &ri_id);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    /* Associate the ramp interval pulse generator to a channel and configure
       the ramp interval pulse generator parameters */
    oduksc_ri_cfg(oduksc_handle, chnl, ri_id);

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_chnl_cgen_ramp_prov */


/*******************************************************************************
*  oduksc_chnl_cgen_ramp_start
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Start the ramp for the resizing of an ODUFLEX channel in G.HAO. The ramp
*   modulo counter is started and the credit generator will start using it. When
*   the ramp-up or ramp-down will be completed, the ramp done interrupt will be
*   generated.
*   In G.HAO ODUflex resizing, the functions are called in the following order:
*   Ramp-up:
*   1) oduksc_chnl_scbs3_cgen_reprov
*   2) oduksc_chnl_cgen_ramp_prov
*   3) oduksc_chnl_cgen_ramp_start
*   4) oduksc_chnl_cgen_ramp_end_prov
*   5) oduksc_ramp_done_isr
*   Ramp-down:
*   1) oduksc_chnl_cgen_ramp_prov
*   2) oduksc_chnl_cgen_ramp_start
*   3) oduksc_chnl_cgen_ramp_end_prov
*   4) oduksc_ramp_done_isr
*   5) oduksc_chnl_scbs3_cgen_reprov
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   client_type       - LO ODUj rate \n
*                       Refer to util_global_odukp_type_t \n
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
PUBLIC PMC_ERROR oduksc_chnl_cgen_ramp_start(oduksc_handle_t *oduksc_handle,
                                             UINT32 chnl,
                                             util_global_odukp_type_t client_type)
{

    /* variable declaration */
    UINT32 ri_id;

    PMC_ENTRY();

    if (oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state != ODUKSC_CHNL_ACTIVATED)
    {
        /* channel is not in activated or provisioned state */
        PMC_RETURN(ODUKSC_CHNL_STATE_NOT_ACTIVATED);
    }

    if(client_type != UTIL_GLOBAL_ODUFLEX_GFP)
    {
        /* channel is not provisioned in ODUFLEX GFP */
        PMC_RETURN(ODUKSC_ERR_INVALID_PARAMETERS);
    }


    /* Get ri_id associated with channel */
    ri_id = oduksc_field_RI_PGEN_SEL_get( NULL, oduksc_handle, chnl);

    /* Enable Ramp Done indication Interrupt */
    oduksc_lfield_range_RAMP_DONE_E_set( NULL, oduksc_handle , chnl, chnl, 1);

    /* Enable ramp credit generation */
    oduksc_field_RAMP_GEN_EN_set( NULL, oduksc_handle , chnl, 1);

    /* Activate ramp generator */
    oduksc_ri_activate(oduksc_handle, ri_id);

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_chnl_cgen_ramp_start */


/*******************************************************************************
*  oduksc_chnl_cgen_ramp_end_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provision the last portion of the ramp for the specified ODUFLEX channel in
*   G.HAO. This function is used to configure the second part of the ramp and
*   is called just before BWR_IND is deasserted. The second part of the ramp
*   lasts exactly 250us before reaching target rate and then the ramp is done.
*   This guarantees BWR_IND is cleared between 125us and 250us before the end
*   of the ramp.
*   In G.HAO ODUflex resizing, the functions are called in the following order:
*   Ramp-up:
*   1) oduksc_chnl_scbs3_cgen_reprov
*   2) oduksc_chnl_cgen_ramp_prov
*   3) oduksc_chnl_cgen_ramp_start
*   4) oduksc_chnl_cgen_ramp_end_prov
*   5) oduksc_ramp_done_isr
*   Ramp-down:
*   1) oduksc_chnl_cgen_ramp_prov
*   2) oduksc_chnl_cgen_ramp_start
*   3) oduksc_chnl_cgen_ramp_end_prov
*   4) oduksc_ramp_done_isr
*   5) oduksc_chnl_scbs3_cgen_reprov
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   client_type       - LO ODUj rate \n
*                       Refer to util_global_odukp_type_t \n
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES: Critical delay for G.HAO. This function should be executed by the CPU
*        instead of the host.
*
*******************************************************************************/
PUBLIC PMC_ERROR oduksc_chnl_cgen_ramp_end_prov(oduksc_handle_t *oduksc_handle,
                                                UINT32 chnl,
                                                util_global_odukp_type_t client_type)
{

    /* variable declaration */
    UINT32 cgen_s;
    UINT32 current_slot_num;
    UINT32 overspeed_factor;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state != ODUKSC_CHNL_ACTIVATED)
    {
        /* channel is not in activated or provisioned state */
        PMC_RETURN(ODUKSC_CHNL_STATE_NOT_ACTIVATED);
    }

    if(client_type != UTIL_GLOBAL_ODUFLEX_GFP)
    {
        /* channel is not provisioned in ODUFLEX GFP */
        PMC_RETURN(ODUKSC_ERR_INVALID_PARAMETERS);
    }


    /* Set useful variables */
    overspeed_factor = 1;

    /* Get current number of timeslots */
    ret_val = oduksc_slot_num_get(oduksc_handle, chnl, &current_slot_num);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    cgen_s = oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_s / overspeed_factor;

    /* Save context (only s and m2 change) */
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_s = cgen_s;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m2 = oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m2 + ODUKSC_RAMP_NB_INTERVAL_250US * cgen_s;

    if (oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_u_d)
    {
        PMC_LOG_TRACE("G.HAO: Ramp-up 250us to %d TS; Rate: %d kb/s\n", oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_oduflex_gfp_rate, oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_rate);
    }
    else
    {
        PMC_LOG_TRACE("G.HAO: Ramp-down 250us to %d TS; Rate: %d kb/s\n", oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_oduflex_gfp_rate, oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_rate);
    }
    PMC_LOG_TRACE("cgen_m  = %d\n", oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m);
    PMC_LOG_TRACE("cgen_n  = %d\n", oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n);
    PMC_LOG_TRACE("cgen_m2 = %d\n", oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m2);
    PMC_LOG_TRACE("cgen_n2 = %d\n", oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n2);
    PMC_LOG_TRACE("cgen_s  = %d\n", oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_s);

    /* Update channel state */
    oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state = ODUKSC_CHNL_REPROV_NO_RST_PENDING;

    /* Apply the new configuration */
    ret_val = oduksc_chnl_update(oduksc_handle, chnl, FALSE);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    /* Clear Ramp Done indication Interrupt */
    oduksc_lfield_range_RAMP_DONE_I_set_to_clear( NULL, oduksc_handle , chnl, chnl, 1);

   PMC_RETURN(PMC_SUCCESS);


} /* oduksc_chnl_cgen_ramp_end_prov */


/*******************************************************************************
*  oduksc_ramp_done_isr
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is called by some high level Interrupt Service Routine (ISR).
*   The structure of ISRs is not clear. So for now it is assumed that once SW
*   Driver is interrupted, some high level ISR will root cause it and if the
*   root cause was ramp_done, then it will call this routine.
*   In G.HAO ODUflex resizing, the functions are called in the following order:
*   Ramp-up:
*   1) oduksc_chnl_scbs3_cgen_reprov
*   2) oduksc_chnl_cgen_ramp_prov
*   3) oduksc_chnl_cgen_ramp_start
*   4) oduksc_chnl_cgen_ramp_end_prov
*   5) oduksc_ramp_done_isr
*   Ramp-down:
*   1) oduksc_chnl_cgen_ramp_prov
*   2) oduksc_chnl_cgen_ramp_start
*   3) oduksc_chnl_cgen_ramp_end_prov
*   4) oduksc_ramp_done_isr
*   5) oduksc_chnl_scbs3_cgen_reprov
*
* INPUTS:
*   *oduksc_handle   - pointer to ODUKSC handle instance
*   chnl             - channel ID to be configured
*                      Valid range: 0 - 95
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
PUBLIC PMC_ERROR oduksc_ramp_done_isr(oduksc_handle_t *oduksc_handle,
                                      UINT32 chnl)
{

    /* variable declaration */
    UINT32 ri_id;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    ri_id = oduksc_field_RI_PGEN_SEL_get( NULL, oduksc_handle, chnl);

    ret_val = oduksc_ri_deactivate(oduksc_handle, ri_id);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    if (oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state != ODUKSC_CHNL_ACTIVATED)
    {
        /* channel is not in activated state */
        PMC_RETURN(ODUKSC_CHNL_STATE_NOT_ACTIVATED);
    }

    /* Reprovision credit generator */
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m   =
        oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_cgen_m;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n   =
        oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_cgen_n;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_u_d = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m2  = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n2  = 0;
    oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_s   = 0;

    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_oduflex_gfp_rate = oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_oduflex_gfp_rate;
    oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_rate = oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->target_rate;

    oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state = ODUKSC_CHNL_REPROV_PENDING;

    ret_val = oduksc_chnl_update(oduksc_handle, chnl, FALSE);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    /* Disable ramp credit generation */
    /* Must be after oduksc_chnl_update, otherwise the ramp will be removed before CGEN update */
    oduksc_field_RAMP_GEN_EN_set( NULL, oduksc_handle , chnl, 0);

    /* Clear Ramp Done indication Interrupt */
    oduksc_lfield_range_RAMP_DONE_I_set_to_clear( NULL, oduksc_handle , chnl, chnl, 1);

    PMC_LOG_TRACE("G.HAO: Ramp completed to %d TS; Rate: %d kb/s\n", oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_oduflex_gfp_rate, oduksc_handle->var.ch_ctxt_ptr[chnl].ghao->current_rate);
    PMC_LOG_TRACE("cgen_m  = %d\n", oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m);
    PMC_LOG_TRACE("cgen_n  = %d\n", oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n);
    PMC_LOG_TRACE("cgen_m2 = %d\n", oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_m2);
    PMC_LOG_TRACE("cgen_n2 = %d\n", oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_n2);
    PMC_LOG_TRACE("cgen_s  = %d\n", oduksc_handle->var.ch_ctxt_ptr[chnl].cgen_s);

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_ramp_done_isr */


/*******************************************************************************
*  oduksc_chnl_ramp_done_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read the ramp done interrupt status for specified channel.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   ramp_done         - Ramp done status for channel
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
PUBLIC PMC_ERROR oduksc_chnl_ramp_done_get(oduksc_handle_t *oduksc_handle,
                                           UINT32 chnl,
                                           UINT32 *ramp_done)
{
    /* variable declaration */


    PMC_ENTRY();

    if (oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state != ODUKSC_CHNL_ACTIVATED)
    {
        /* channel is not in activated state */
        PMC_RETURN(ODUKSC_CHNL_STATE_NOT_ACTIVATED);
    }


    /* Read interrupt status */
    *ramp_done = oduksc_lfield_range_RAMP_DONE_I_get ( NULL, oduksc_handle, chnl, chnl );

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_chnl_ramp_done_get */

/*******************************************************************************
*  oduksc_chnl_ramp_done_poll
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Poll the ramp done interrupt for specified channel. This function waits
*   until the ramp done interrupt is set in the register.
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   poll_delay_us     - Number of microseconds between read
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
PUBLIC PMC_ERROR oduksc_chnl_ramp_done_poll(oduksc_handle_t *oduksc_handle,
                                            UINT32 chnl,
                                            UINT32 poll_delay_us)
{
    /* variable declaration */
    UINT32 poll_iter;
    UINT32 value;
    UINT32 num_iter;
    PMC_ERROR ret_val = PMC_SUCCESS;


    PMC_ENTRY();

    if (oduksc_handle->var.ch_ctxt_ptr[chnl].chnl_state != ODUKSC_CHNL_ACTIVATED)
    {
        /* channel is not in activated state */
        PMC_RETURN(ODUKSC_CHNL_STATE_NOT_ACTIVATED);
    }


    poll_iter = (UINT32)(1.25 * 200 / ((DOUBLE)poll_delay_us * 1e-6)) + 1;
    value = 1 << (chnl & 0x1F);

    /* Poll interrupt */
    ret_val = oduksc_reg_RAMP_DONE_INTS_array_poll ( NULL, oduksc_handle, chnl >> 5, value, value, PMC_POLL_EQUAL,
                                                     poll_iter, &num_iter, poll_delay_us );
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ODUKSC_PAGE_SWAP_REQ_TIMEOUT);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_chnl_ramp_done_poll */


/*******************************************************************************
*  oduksc_ri_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Associates a ramp interval pulse generator to a specific channel. Configures
*   ramp interval (TN) numerator and (TM) denominatior for the specified ramp
*   interval pulse generator based on the input interval.
*
* INPUTS:
*   *oduksc_handle   - pointer to ODUKSC handle instance
*   chnl             - channel ID to associate with the ramp interval
*                      Valid range: 0 - 95
*   ri_id            - ramp interval pulse generator to be configured
*                      Valid range: 0 - 5
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
PRIVATE PMC_ERROR oduksc_ri_cfg(oduksc_handle_t *oduksc_handle, UINT32 chnl,
                                UINT32 ri_id)
{
    /* variable declaration */

    PMC_ENTRY();

    /* Associate a ramp interval pulse generator to a specific channel */
    oduksc_field_RI_PGEN_SEL_set( NULL, oduksc_handle, chnl, ri_id);

    if (oduksc_handle->cfg.db_id == UTIL_GEN_DB_LINE_CORE_OTN_RX_DB)
    {
        /* In COREOTN, TM/TN generate 125us pulse intervals (TN 8X larger) */
        oduksc_field_TM_set( NULL, oduksc_handle, ri_id, ODUKSC_RAMP_INTERVAL_TM);
        oduksc_field_TN_set( NULL, oduksc_handle, ri_id, 8 * ODUKSC_RAMP_INTERVAL_TN);
    }
    else
    {
        /* In MAPOTN, TM/TN generate 15.625us pulse intervals */
        oduksc_field_TM_set( NULL, oduksc_handle, ri_id, ODUKSC_RAMP_INTERVAL_TM);
        oduksc_field_TN_set( NULL, oduksc_handle, ri_id, ODUKSC_RAMP_INTERVAL_TN);
    }

    /* Update the ramp inteval pulse generator state to unavailable*/
    oduksc_handle->var.ri_state_array[ri_id] = RI_UNAVAILABLE;

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_ri_cfg */


/*******************************************************************************
*  oduksc_ri_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates the ramp interval pulse generator.
*
* INPUTS:
*   *oduksc_handle   - pointer to ODUKSC handle instance
*   ri_id            - ramp interval pulse generator to be activated
*                      Valid range: 0 - 5
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
PRIVATE PMC_ERROR oduksc_ri_activate(oduksc_handle_t *oduksc_handle,
                                     UINT32 ri_id)
{
    /* variable declaration */

    PMC_ENTRY();

    /* Activate the ramp interval pulse generator */
    oduksc_field_RI_EN_set( NULL, oduksc_handle, ri_id, 1);

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_ri_activate */


/*******************************************************************************
*  oduksc_ri_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates the ramp interval pulse generator.
*
* INPUTS:
*   *oduksc_handle   - pointer to ODUKSC handle instance
*   ri_id            - ramp interval pulse generator to be deactivated
*                      Valid range: 0 - 5
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
PRIVATE PMC_ERROR oduksc_ri_deactivate(oduksc_handle_t *oduksc_handle,
                                       UINT32 ri_id)
{
    /* variable declaration */

    PMC_ENTRY();

    if (ri_id > ODUKSC_RI_NUM)
    {
        PMC_RETURN(ODUKSC_ERR_INVALID_PARAMETERS);
    }

    /* Deactivate the ramp interval pulse generator */
    oduksc_field_RI_EN_set( NULL, oduksc_handle, ri_id, 0);

    /* Update the ramp inteval pulse generator state to available*/
    oduksc_handle->var.ri_state_array[ri_id] = RI_AVAILABLE;

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_ri_deactivate */


/*******************************************************************************
*  oduksc_ri_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets the available and unsed ramp interval pulse generator.
*
* INPUTS:
*   *oduksc_handle   - pointer to ODUKSC handle instance
*
*
* OUTPUTS:
*   *ri_id           - pointer to available ramp interval pulse generator
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR oduksc_ri_get(oduksc_handle_t *oduksc_handle, UINT32 *ri_id)
{
    /* variable declaration */
    UINT32 i;

    PMC_ENTRY();

    for(i=0; i<ODUKSC_RI_NUM; i++)
    {
        if(oduksc_handle->var.ri_state_array[i]==RI_AVAILABLE)
        {
            *ri_id = i;
            PMC_RETURN(PMC_SUCCESS);
        }
    }

    PMC_RETURN(ODUKSC_RI_NOT_FOUND);


} /* oduksc_ri_get */


/*******************************************************************************
*  oduksc_oduflex_gfp_rate_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get the rate in kbps for the ODUflex(GFP) number of tributary slots.
*
* INPUTS:
*   *oduksc_handle   - pointer to ODUKSC handle instance
*   tribslots        - number of tributary slots
*
*
* OUTPUTS:
*   *oduflex_rate_bps - pointer to oduflex rate in bps
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR oduksc_oduflex_gfp_rate_get(oduksc_handle_t *oduksc_handle,
                                              UINT32 tribslots,
                                              DOUBLE *oduflex_rate_bps)
{
    /* variable declaration */

    PMC_ENTRY();

    if (tribslots >= 1 && tribslots <= 8)
    {
        /* 1.24917723 Gb/s per TS*/
        *oduflex_rate_bps = (tribslots * UTIL_GLOBAL_ODU2_TS_RATE);
    }
    else if (tribslots <= 32)
    {
        /* 1.25447054 Gb/s per TS*/
        *oduflex_rate_bps = (tribslots * UTIL_GLOBAL_ODU3_TS_RATE);
    }
    else if (tribslots <= 80)
    {
        /* 1.301467133 Gb/s per TS*/
        *oduflex_rate_bps = (tribslots * UTIL_GLOBAL_ODU4_TS_RATE);
    }
    else
    {
        PMC_RETURN(ODUKSC_ERR_INVALID_PARAMETERS);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_oduflex_gfp_rate_get */


/*******************************************************************************
*  oduksc_oduflex_gfp_rate_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get the number of calendar slots for the specified channel.
*
* INPUTS:
*   *oduksc_handle   - pointer to ODUKSC handle instance
*   chnl             - channel ID for which the number of slots is requested
*                      Valid range: 0 - 95
*
*
* OUTPUTS:
*   *slot_num        - pointer to the number of calendar slots for channel
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR oduksc_slot_num_get(oduksc_handle_t *oduksc_handle,
                                      UINT32 chnl,
                                      UINT32 *slot_num)
{
    /* variable declaration */

    /* variable declaration */
    UINT32 num_recs;

    /* variable initialization */
    num_recs = 0;

    PMC_ENTRY();

    /* if DB being used get the num_cal_entries from DB
       otherwise calculate time_slot based on rate */
    if (oduksc_handle->cfg.db_en == TRUE)
    {
        if(oduksc_handle->cfg.db_id == UTIL_GEN_DB_ODUKSC_DB)
        {
            mapotn_db_key key;

            key.calendar_entry = DO_NOT_USE_KEY;
            key.channel_num = chnl;

            num_recs=gen_db_query_count(&(oduksc_handle->base), oduksc_handle->cfg.db_id, oduksc_handle->cfg.db_handle, (void *)&key);

            if (num_recs == 0)
            {
                PMC_RETURN(ODUKSC_ERR_DB_ENTRY_NOT_FOUND);
            }

        }
        else if(oduksc_handle->cfg.db_id == UTIL_GEN_DB_LINE_CORE_OTN_RX_DB)
        {
            line_core_otn_db_key key;

            key.calendar_entry = DO_NOT_USE_KEY;
            key.ho_channel = DO_NOT_USE_KEY;
            key.mo_channel = DO_NOT_USE_KEY;
            key.lo_channel = chnl;
            key.trib_slot = DO_NOT_USE_KEY;

            PMC_LOG_TRACE("UTIL_GEN_DB_LINE_CORE_OTN_RX_DB\n");
            PMC_LOG_TRACE("key.calendar_entry = %u, key.lo_channel = %u\n", key.calendar_entry, key.lo_channel);

            num_recs=gen_db_query_count(&(oduksc_handle->base), oduksc_handle->cfg.db_id, oduksc_handle->cfg.db_handle, (void *)&key);

            /* not two stage - schedule for each HO channel */
            if (num_recs == 0)
            {
                line_core_otn_db_key one_stage_key;

                one_stage_key.calendar_entry = DO_NOT_USE_KEY;
                one_stage_key.ho_channel = DO_NOT_USE_KEY;
                one_stage_key.mo_channel = chnl;
                one_stage_key.lo_channel = DO_NOT_USE_KEY;
                one_stage_key.trib_slot = DO_NOT_USE_KEY;

                num_recs=gen_db_query_count(&(oduksc_handle->base), oduksc_handle->cfg.db_id, oduksc_handle->cfg.db_handle, (void *)&one_stage_key);

                if (num_recs == 0)
                {
                    line_core_otn_db_key no_stage_key;

                    no_stage_key.calendar_entry = DO_NOT_USE_KEY;
                    no_stage_key.ho_channel = chnl;
                    no_stage_key.mo_channel = DO_NOT_USE_KEY;
                    no_stage_key.lo_channel = DO_NOT_USE_KEY;
                    no_stage_key.trib_slot = DO_NOT_USE_KEY;

                    num_recs=gen_db_query_count(&(oduksc_handle->base), oduksc_handle->cfg.db_id, oduksc_handle->cfg.db_handle, (void *)&no_stage_key);

                    if (num_recs == 0)
                    {
                        PMC_RETURN(ODUKSC_ERR_DB_ENTRY_NOT_FOUND);
                    }

                }
            }
        }
        else
        {
            PMC_RETURN(ODUKSC_ERR_INVALID_PARAMETERS);
        }
        *slot_num = num_recs;
    }
    else
    {
        PMC_RETURN(ODUKSC_ERR_INVALID_PARAMETERS);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* oduksc_oduflex_gfp_rate_get */



/*******************************************************************************
* oduksc_int_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   *oduksc_handle    - pointer to ODUKSC handle instance
*   non_null_ptr      - pointer to check
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
PRIVATE PMC_ERROR oduksc_int_validate(oduksc_handle_t *oduksc_handle,
                                      void *non_null_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != oduksc_handle, ODUKSC_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (NULL == non_null_ptr)
    {
        result = ODUKSC_ERR_INVALID_PARAMETERS;
    }
    PMC_RETURN(result);
} /* oduksc_int_validate */
/*******************************************************************************
* oduksc_int_chnl_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt channel functions.
*   
*
* INPUTS:
*   *oduksc_handle   - pointer to ODUKSC handle instance
*   chnl_id          - chnl id
*   non_null_ptr     - pointer to check
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR oduksc_int_chnl_validate(oduksc_handle_t *oduksc_handle,
                                           UINT32 chnl_id,
                                           void *non_null_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != oduksc_handle, ODUKSC_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (chnl_id >= oduksc_handle->cfg.max_chnl)
    {
        result = ODUKSC_ERR_INVALID_PARAMETERS;
    }

    if (PMC_SUCCESS == result && NULL == non_null_ptr)
    {
        result = ODUKSC_ERR_INVALID_PARAMETERS;
    }
    PMC_RETURN(result);
} /* oduksc_int_chnl_validate */

/*
** End of file
*/
