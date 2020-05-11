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

#include "odujat_loc.h"


/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
/* Define MC_NUMERATOR value for ODUFlex clients */
#define ODUFLEX_MC_NUMERATOR { \
   /* Index is number of ODU0's in ODUFlex container */  \
   /* 1,      2,       3,        4,        5,        6,        7,        8,        9,        10, */  \
   {13516800, 3379200 , 5068800 , 6758400 , 8448000 , 10137600, 11827200, 13516800, 15206400, 16896000}, /* oduflex 1 - 10  */ \
   {18585600, 20275200, 21964800, 23654400, 25344000, 27033600, 28723200, 30412800, 32102400, 33792000}, /* oduflex 11 - 20 */ \
   {35481600, 37171200, 38860800, 40550400, 42240000, 43929600, 45619200, 47308800, 48998400, 50688000}, /* oduflex 21 - 30 */ \
   {52377600, 27033600, 27878400, 28723200, 29568000, 30412800, 31257600, 32102400, 32947200, 33792000}, /* oduflex 31 - 40 */ \
   {34636800, 35481600, 36326400, 37171200, 38016000, 38860800, 39705600, 40550400, 41395200, 42240000}, /* oduflex 41 - 50 */ \
   {43084800, 43929600, 44774400, 45619200, 46464000, 47308800, 48153600, 48998400, 49843200, 50688000}, /* oduflex 51 - 60 */ \
   {51532800, 52377600, 53222400, 27033600, 27456000, 27878400, 28300800, 28723200, 29145600, 29568000}, /* oduflex 61 - 70 */ \
   {29990400, 30412800, 30835200, 31257600, 31680000, 32102400, 32524800, 32947200, 33369600, 33792000}, /* oduflex 71 - 80 */ \
} /* ODUFlex MC_NUMERATOR sequence */ 

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
const char ODUJAT_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    ODUJAT_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char ODUJAT_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */


/*
** Forward References
*/
PRIVATE void odujat_var_default_init(odujat_handle_t *odujat_handle);

                                              
PRIVATE PMC_ERROR odujat_chnl_deactivate_internal(odujat_handle_t *odujat_handle,
                                                  UINT32 chnl,
                                                  BOOL8 super_prov);
                                                                                              
PRIVATE UINT32 odujat_chnl_conv_gain_calc(odujat_handle_t *odujat_handle,
                                            UINT32 chnl,
                                            util_global_odukp_type_t odu_type,
                                            UINT32 odu_flex_data);

PRIVATE UINT32 odujat_chnl_loop_alpha_calc(odujat_handle_t *odujat_handle,
                                            UINT32 chnl,
                                            util_global_odukp_type_t odu_type,
                                            UINT32 odu_flex_data);
PRIVATE PMC_ERROR odujat_int_validate(odujat_handle_t *odujat_handle,
                                      UINT32 chnl_id,
                                      void *non_null_ptr);
/*
** Public Functions
*/

/*******************************************************************************
* odujat_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for an ODUJAT block instance.
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
*   odujat_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC odujat_handle_t *odujat_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    odujat_handle_t *odujat_handle;

    PMC_ENTRY();
    
    odujat_handle = (odujat_handle_t*)PMC_CTXT_CALLOC(sizeof(odujat_handle_t), parent);

    pmc_handle_init(parent, odujat_handle, sys_handle, PMC_MID_DIGI_ODUJAT, tsb_name, base_address);

    /* Register log strings. */
    pmc_log_block_strings_register(ODUJAT_LOG_ERR_STRINGS[0], ODUJAT_LOG_ERR_TABLE_BASE, ODUJAT_LOG_ERR_COUNT);

    odujat_handle->oduksc_handle = oduksc_ctxt_create((pmc_handle_t *)odujat_handle, base_address+BASE_ADDR_ODUJAT_ODUKSC_CORE, sys_handle, "ODUJAT_ODUKSC");
    PMC_CTXT_REGISTER_PTR(&odujat_handle->oduksc_handle, parent);
    odujat_handle->cfc_handle = cfc_ctxt_create((pmc_handle_t *)odujat_handle, base_address+BASE_ADDR_ODUJAT_FIFO_CFC_CFC, sys_handle, "ODUJAT_CFC"); 
    PMC_CTXT_REGISTER_PTR(&odujat_handle->cfc_handle, parent);

    PMC_RETURN(odujat_handle);
} /* odujat_ctxt_create */


/*******************************************************************************
* odujat_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for an ODUJAT block instance.
*
*
* INPUTS:
*   *odujat_handle   - pointer to ODUJAT handle instance
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
PUBLIC void odujat_ctxt_destroy(odujat_handle_t *odujat_handle)
{
    PMC_ENTRY();
    
    PMC_LOG_TRACE("odujat_handle->cfg.db_id = %u\n", odujat_handle->cfg.db_id);
    PMC_LOG_TRACE("odujat_handle->db_handle = %u\n", odujat_handle->cfg.db_handle);

    if (odujat_handle->cfg.db_id > 0)
    {
        gen_db_detach(&(odujat_handle->base), 
                      odujat_handle->cfg.db_id,
                      odujat_handle->cfg.db_handle);
    }
    
    oduksc_ctxt_destroy(odujat_handle->oduksc_handle);
    cfc_ctxt_destroy(odujat_handle->cfc_handle);

    PMC_CTXT_FREE(&(odujat_handle->var.ch_ctxt_ptr), odujat_handle);
    
    PMC_CTXT_FREE(&odujat_handle, odujat_handle);

    PMC_RETURN();
    
} /* odujat_ctxt_destroy */

/*******************************************************************************
* odujat_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an ODUJAT block handle instance.
*
*
* INPUTS:
*   *odujat_handle     - pointer to ODUJAT handle instance
*   odujat_id          - Enum for ODUJAT type. See odujat_type_id_t.
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
PUBLIC void odujat_handle_init(odujat_handle_t *odujat_handle, 
                               odujat_type_id_t odujat_id)
{
    /* variable declaration */
    UINT32              size;
    cfc_cfg_t           cfc_cfg;
    oduksc_type_id_t    oduksc_id = (oduksc_type_id_t)0;

    PMC_ENTRY();
    
    /* ODUJAT parameters */
    odujat_handle->cfg.odujat_type_id   = odujat_id;
    odujat_handle->cfg.max_chnl         = ODUJAT_NUM_CHNL; 
    odujat_handle->oduksc_handle->cfg.scbs3_present = FALSE;

    
    switch (odujat_id)
    {
        case ODUJAT_FMF1:
            oduksc_id = ODUKSC_COREOTN_ODUJAT1;
            odujat_handle->cfg.cal_entries_alloc_method = 12;
            odujat_handle->cfg.db_id = UTIL_GEN_DB_LINE_CORE_OTN_RX_DB;
            break;
        case ODUJAT_FMF2:
            oduksc_id = ODUKSC_COREOTN_ODUJAT2;
            odujat_handle->cfg.cal_entries_alloc_method = 13;
            odujat_handle->cfg.db_id = UTIL_GEN_DB_LINE_CORE_OTN_RX_DB;
            break;
        default:
            PMC_DEBUG(ODUJAT_ERR_INVALID_ARGS, odujat_id, 0);
        break;         
    }
    
    odujat_handle->cfg.db_handle = gen_db_attach(&(odujat_handle->base), odujat_handle->cfg.db_id);
    
    oduksc_handle_init(odujat_handle->oduksc_handle, oduksc_id, 0);
    
    /* CFC initialization */
    /* CFC parameters */
    odujat_handle->cfg.cfc_cfg.max_fifo_num  = ODUJAT_CFC_MAX_FIFO_NUM; 
    odujat_handle->cfg.cfc_cfg.page_size     = ODUJAT_CFC_PAGE_SIZE; 
     
    /* Initialize CFC */
    cfc_cfg.max_fifo_num = odujat_handle->cfg.cfc_cfg.max_fifo_num;
    cfc_cfg.page_size    = odujat_handle->cfg.cfc_cfg.page_size;
    cfc_handle_init(odujat_handle->cfc_handle, &cfc_cfg);
    
    size = sizeof(odujat_ch_ctxt_struct_t) * odujat_handle->cfg.max_chnl;
    odujat_handle->var.ch_ctxt_ptr = (odujat_ch_ctxt_struct_t*)PMC_CTXT_CALLOC(size, odujat_handle);
    PMC_CTXT_REGISTER_PTR(&odujat_handle->var.ch_ctxt_ptr, odujat_handle);
    
    /* Initialize var_t parameters */
    odujat_var_default_init(odujat_handle);

    PMC_RETURN();
} /* odujat_handle_init */

/*******************************************************************************
* odujat_alloc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Handles any local memory allocation freed in ctxt_destroy
*
* INPUTS:
*   *odujat_handle        - pointer to ODUJAT handle instance
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
PUBLIC void odujat_alloc_init(odujat_handle_t *odujat_handle)
{
    PMC_ENTRY();

    cfc_alloc_init(odujat_handle->cfc_handle);

    oduksc_alloc_init(odujat_handle->oduksc_handle);
    
    PMC_RETURN();
} /* odujat_alloc_init */

/*******************************************************************************
* odujat_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reinitializes an ODUJAT block handle instance.
*
*
* INPUTS:
*   *odujat_handle        - pointer to ODUJAT handle instance
*   *restart_init_cfg_ptr - pointer to restart init config
*   top_energy_state      - The expected energy state 
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
PUBLIC void odujat_handle_restart_init(odujat_handle_t *odujat_handle, 
                                       util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                       pmc_energy_state_t top_energy_state)
{
    /* variable declaration */
    cfc_cfg_t           cfc_cfg;

    PMC_ENTRY();
    
    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(ODUJAT_LOG_ERR_STRINGS[0], ODUJAT_LOG_ERR_TABLE_BASE, ODUJAT_LOG_ERR_COUNT);
    }

    /* Initialize CFC */
    cfc_cfg.max_fifo_num = odujat_handle->cfg.cfc_cfg.max_fifo_num;
    cfc_cfg.page_size    = odujat_handle->cfg.cfc_cfg.page_size;
    cfc_handle_restart_init(odujat_handle->cfc_handle, &cfc_cfg, restart_init_cfg_ptr, top_energy_state);
    
    PMC_RETURN();
} /* odujat_handle_restart_init */


/*******************************************************************************
*  odujat_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize ODUJAT:CFC parameters and global configuration.
*
* INPUTS:
*   *odujat_handle  - pointer to ODUJAT handle instance
*   mode            - FIFO filtering mode \n
*                     0 -- FIFO operates in bypass mode. \n
*                     1 -- FIFO reads are generated by the JAT.
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
PUBLIC PMC_ERROR odujat_init(odujat_handle_t *odujat_handle, UINT32 mode)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    /* Initialize CFC */
    if (ret_val == PMC_SUCCESS)
    {
        ret_val = cfc_init(odujat_handle->cfc_handle, PMC_ENERGY_STATE_RUNNING);
    }      
    if (ret_val == PMC_SUCCESS)
    {
        ret_val = cfc_bypass(odujat_handle->cfc_handle, FALSE);
    }      

    /* Configure RAM_LS */
    odujat_field_RAM_LS_set(NULL, odujat_handle, 1);
    
    /* Configure PG_OVERRIDE to 0 */
    odujat_field_PG_OVERRIDE_set(NULL, odujat_handle, 0);
       
    /* remove oduksc soft reset */ 
    if (ret_val == PMC_SUCCESS)
    {
        ret_val = oduksc_reset_cfg(odujat_handle->oduksc_handle, 0);
    }      

    PMC_RETURN(ret_val);
} /* odujat_init */

/*******************************************************************************
*  odujat_chnl_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures static per channel configuration
*
* INPUTS:
*   *odujat_handle      - pointer to ODUJAT handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   mode                - enables or disables JAT  \n
*                         0 -- JAT in bypass mode \n
*                         1 -- JAT enabled
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
PUBLIC PMC_ERROR odujat_chnl_init(odujat_handle_t *odujat_handle,
                                  UINT32 chnl, odujat_chnl_mode_t mode)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    odujat_buffer_t odujat_buffer[1];
    
    PMC_ENTRY();
   
    odujat_buffer_init(odujat_buffer, odujat_handle);
    odujat_field_JAT_ENABLE_set(odujat_buffer, odujat_handle, chnl, mode);   
    
    /* save to context */
    odujat_handle->var.ch_ctxt_ptr[chnl].chnl_mode = mode;
    
    /* global channel setting */
    /* DQ_AT_LIMIT = 1 */
    odujat_field_DQ_AT_LIMIT_set(odujat_buffer, odujat_handle, chnl, 1);
   
    /* ADD_RESIDUE = 1 */
    odujat_field_ADD_RESIDUE_set(odujat_buffer, odujat_handle, chnl, 1);
    /* ADD_CND = 1 */
    odujat_field_ADD_CND_set(odujat_buffer, odujat_handle, chnl, 1);
    
    if (mode == ODUJAT_CHNL_ENABLED)
    {    
        /* when JAT is enabled, CnD is sinked and JAT sends out 0s */
        odujat_field_CND_SELECT_set(odujat_buffer, odujat_handle, chnl, 1);          
    }
    else {
        odujat_field_CND_SELECT_set(NULL, odujat_handle, chnl, 0);
    }   
    odujat_buffer_flush(odujat_buffer);

    odujat_buffer_init(odujat_buffer, odujat_handle);

    /* odujat_buffer = 182 */
    odujat_field_CND_DELAY_set(NULL, odujat_handle, chnl, 182);
    
    /* FIFO_LEVEL_LIMIT */
    odujat_field_FIFO_LEVEL_LIMIT_set(NULL, odujat_handle, chnl, 384);   
    odujat_buffer_flush(odujat_buffer);

    PMC_RETURN(result);
} /* odujat_chnl_init */

/*******************************************************************************
*  odujat_chnl_loop_alpha_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Calculates the value of alpha used in the jitter attenuator loop filter.  It
*   is assumed that the LOOP_ALPHA_SELECT register bit field will be set with
*   this value immediately after calling this function.
*
* INPUTS:
*   *odujat_handle      - pointer to ODUJAT handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   odu_type            - ODUk rate. See to util_global_odukp_type_t
*   odu_flex_data       - number of ODU0's in the client.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32              - The value to write into the LOOP_ALPHA_SELECT register 
*                         bit field.
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 odujat_chnl_loop_alpha_calc(odujat_handle_t *odujat_handle,
                                            UINT32 chnl,
                                            util_global_odukp_type_t odu_type,
                                            UINT32 odu_flex_data)
{
    /* variable declaration */
    UINT32 loop_alpha;
    
    PMC_ENTRY();
    
    switch (odu_type)
    {
        case UTIL_GLOBAL_ODU0:
            loop_alpha = 8;
        break;
        case UTIL_GLOBAL_ODU1:
            loop_alpha = 9;
        break;
        case UTIL_GLOBAL_ODU2:
        case UTIL_GLOBAL_ODU2E:
        case UTIL_GLOBAL_ODU2E_ETRANS:
        case UTIL_GLOBAL_ODU2F:
        case UTIL_GLOBAL_ODU1E:
        case UTIL_GLOBAL_ODU1E_ETRANS:
        case UTIL_GLOBAL_ODU1F:
            loop_alpha = 11;
        break;
        case UTIL_GLOBAL_ODU3:
        case UTIL_GLOBAL_ODU3E2:
        case UTIL_GLOBAL_ODU3E1:
            loop_alpha = 13;
        break;
        case UTIL_GLOBAL_ODU4:
            loop_alpha = 14;
        break;
        case UTIL_GLOBAL_ODUFLEX_CBR: 
        case UTIL_GLOBAL_ODUFLEX_GFP:
            if (odu_flex_data == 1)
            {
                loop_alpha = 8;
            }
            else if (odu_flex_data > 1 && odu_flex_data < 4)
            {
                loop_alpha = 9;
            }
            else if (odu_flex_data > 3 && odu_flex_data < 8)
            {
                loop_alpha = 10;
            }
            else if (odu_flex_data > 7 && odu_flex_data < 13)
            {
                loop_alpha = 11;
            }
            else if (odu_flex_data > 12 && odu_flex_data < 23)
            {
                loop_alpha = 12;
            }
            else if (odu_flex_data > 23 && odu_flex_data < 70)
            {
                loop_alpha = 13;
            }
            else {
                loop_alpha = 14;
            }   
        break;
        default:
            PMC_RETURN(ODUJAT_ERR_INVALID_ARGS);   
    }
    
    PMC_RETURN(loop_alpha);
} /* odujat_chnl_loop_alpha_calc */

/*******************************************************************************
*  odujat_chnl_conv_gain_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Calculates the conversion gain value.  It is assuemd that the
*   CONV_GAIN_SELECT register bit field will be set using this value immediately
*   after calling this function.
*
* INPUTS:
*   *odujat_handle      - pointer to ODUJAT handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   odu_type            - ODUk rate. See to util_global_odukp_type_t
*   odu_flex_data       - number of ODU0's in the client.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32              - The conversion gain value
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 odujat_chnl_conv_gain_calc(odujat_handle_t *odujat_handle,
                                            UINT32 chnl,
                                            util_global_odukp_type_t odu_type,
                                            UINT32 odu_flex_data)
{
    /* variable declaration */
    UINT32 conv_gain;
    
    PMC_ENTRY();
    
    switch (odu_type)
    {
        case UTIL_GLOBAL_ODU0:
            conv_gain = 6;
        break;
        case UTIL_GLOBAL_ODU1:
        case UTIL_GLOBAL_ODU2:
        case UTIL_GLOBAL_ODU2E:
        case UTIL_GLOBAL_ODU2E_ETRANS:
        case UTIL_GLOBAL_ODU2F:
        case UTIL_GLOBAL_ODU1E:
        case UTIL_GLOBAL_ODU1E_ETRANS:
        case UTIL_GLOBAL_ODU1F:
            conv_gain = 3;
        break;
        case UTIL_GLOBAL_ODU3:
        case UTIL_GLOBAL_ODU3E2:
        case UTIL_GLOBAL_ODU3E1:
            conv_gain = 2;
        break;
        case UTIL_GLOBAL_ODU4:
            conv_gain = 1;
        break;
        case UTIL_GLOBAL_ODUFLEX_CBR: 
        case UTIL_GLOBAL_ODUFLEX_GFP:
            if (odu_flex_data == 1)
            {
                conv_gain = 6;
            }
            else if (odu_flex_data > 1 && odu_flex_data < 32)
            {
                conv_gain = 3;
            }
            else if (odu_flex_data > 31 && odu_flex_data < 64)
            {
                conv_gain = 2;
            }
            else {
                conv_gain = 1;
            }   
        break;
        default:
            PMC_RETURN(ODUJAT_ERR_INVALID_ARGS);   
    }
    
    
    PMC_RETURN(conv_gain);
} /* odujat_chnl_conv_gain_calc */

/*******************************************************************************
*  odujat_chnl_mc_numer_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the numerator Mc which is added to output of the conversion gain.
*
* INPUTS:
*   *odujat_handle      - pointer to ODUJAT handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   odu_type            - ODUk rate. See to util_global_odukp_type_t
*   odu_flex_data       - number of ODU0's in the client.
*
* OUTPUTS:
*   *mc_num             - current MC_NUMERATOR value
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odujat_chnl_mc_numer_cfg(odujat_handle_t *odujat_handle,
                                          UINT32 chnl,
                                          util_global_odukp_type_t odu_type,
                                          UINT32 odu_flex_data,
                                          UINT32 *mc_num)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 mc_numer = 0;
    UINT32 row, col;
    UINT32 flex_mc_numer_array[8][10] = ODUFLEX_MC_NUMERATOR;
    
    PMC_ENTRY();
    
    switch (odu_type)
    {
        case UTIL_GLOBAL_ODU0:
        case UTIL_GLOBAL_ODU2:
        case UTIL_GLOBAL_ODU2E:
        case UTIL_GLOBAL_ODU2E_ETRANS:
        case UTIL_GLOBAL_ODU2F:
        case UTIL_GLOBAL_ODU1E:
        case UTIL_GLOBAL_ODU1E_ETRANS:
        case UTIL_GLOBAL_ODU1F:
            mc_numer = 13516800;
        break;
        case UTIL_GLOBAL_ODU1:
            mc_numer = 3379200;
        break;
        case UTIL_GLOBAL_ODU3:
        case UTIL_GLOBAL_ODU3E2:
        case UTIL_GLOBAL_ODU3E1:
            mc_numer = 27033600;
        break;
        case UTIL_GLOBAL_ODU4:
            mc_numer = 33792000;
        break;
        case UTIL_GLOBAL_ODUFLEX_CBR: 
        case UTIL_GLOBAL_ODUFLEX_GFP:
            /* find the correct row in the matrix */
            row = (odu_flex_data - 1) / 10;
      
            /* find the correct column in the matrix */
            col = (odu_flex_data - 1) % 10;
            
            if (row > 7 || col > 9)
            {
                PMC_RETURN(ODUJAT_ERR_INVALID_ARGS);
            }
            mc_numer = flex_mc_numer_array[row][col];
        break;
        default:
            ret_val = ODUJAT_ERR_INVALID_ARGS;
        break;   
    }
    
    *mc_num = mc_numer;
    
    odujat_field_MC_NUMERATOR_set(NULL, odujat_handle, chnl, mc_numer);
    
    PMC_RETURN(ret_val);
} /* odujat_chnl_mc_numer_cfg */


/*******************************************************************************
*  odujat_chnl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provisions a channel for the specified rate.
*
* INPUTS:
*   *odujat_handle      - pointer to ODUJAT handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   mux_stage           - Mux stage instance.
*   ho_chnl             - HO channel ID that this LO channel belongs to
*                         Valid range: 0 - 95
*   ho_odu_type         - HO ODUk rate. See util_global_odukp_type_t.
*   lo_odu_type         - LO ODUk rate. See to util_global_odukp_type_t
*   odu_flex_data       - number of ODU0's in the client.
*   client_rate         - ODU data rate kbits/sec for the case of ODUFLEX 
*   mode                - enables or disables JAT \n
*                         0 -- JAT in bypass mode \n
*                         1 -- JAT enabled
*   super_prov          - boolean value to determine if this is used for super_prov
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
PUBLIC PMC_ERROR odujat_chnl_prov(odujat_handle_t *odujat_handle,
                                  UINT32 chnl,
                                  UINT32 mux_stage, UINT32 ho_chnl,
                                  util_global_odukp_type_t ho_odu_type,
                                  util_global_odukp_type_t lo_odu_type,
                                  UINT32 odu_flex_data,
                                  UINT32 client_rate,
                                  odujat_chnl_mode_t mode,
                                  BOOL8 super_prov)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    line_core_otn_db_key    key;
    UINT32            num_of_recs;
    odujat_buffer_t odujat_buffer[1];
    
    UINT32 mc_num;
    UINT32 conv_gain;
    UINT32 loop_alpha;

    PMC_ENTRY();

/* Next 48 lines are the contentx of odujat_chnl_init.  This is done to enable
 * writing the complete register rather than RMW the register twice. */
    odujat_buffer_init(odujat_buffer, odujat_handle);
    odujat_field_JAT_ENABLE_set(odujat_buffer, odujat_handle, chnl, mode);   
    
    /* save to context */
    odujat_handle->var.ch_ctxt_ptr[chnl].chnl_mode = mode;
    
    /* global channel setting */
    /* DQ_AT_LIMIT = 1 */
    odujat_field_DQ_AT_LIMIT_set(odujat_buffer, odujat_handle, chnl, 1);
   
    /* ADD_RESIDUE = 1 */
    odujat_field_ADD_RESIDUE_set(odujat_buffer, odujat_handle, chnl, 1);
    /* ADD_CND = 1 */
    odujat_field_ADD_CND_set(odujat_buffer, odujat_handle, chnl, 1);
    
    if (mode == ODUJAT_CHNL_ENABLED)
    {    
        /* when JAT is enabled, CnD is sinked and JAT sends out 0s */
        odujat_field_CND_SELECT_set(odujat_buffer, odujat_handle, chnl, 1);     
    }
    else {

        if (odujat_handle->cfg.odujat_type_id == ODUJAT_FMF2 
            && mux_stage == 1
            && ho_odu_type == lo_odu_type)
        {
            odujat_field_CND_SELECT_set(odujat_buffer, odujat_handle, chnl, 7);
        }
        else {
            odujat_field_CND_SELECT_set(odujat_buffer, odujat_handle, chnl, 0);
        }
    }   

    if (odujat_handle->var.ch_ctxt_ptr[chnl].chnl_state != ODUJAT_CHNL_START)
    {
        /* channel is not in start state (e.g. it has either been already provisioned or provisioned and activated) */
        odujat_buffer_flush(odujat_buffer);
        PMC_RETURN(ODUJAT_CHNL_STATE_NOT_START);
    }

    /* configure loop_alpha */
    if (PMC_SUCCESS == ret_val)
    {
        loop_alpha = odujat_chnl_loop_alpha_calc(odujat_handle, chnl, lo_odu_type,
                                                 odu_flex_data);

        odujat_field_LOOP_ALPHA_SELECT_set(odujat_buffer, odujat_handle, chnl, loop_alpha);
    
    }                                             
    
    /* configure conv_gain */
    if (PMC_SUCCESS == ret_val)
    {
        conv_gain = odujat_chnl_conv_gain_calc(odujat_handle, chnl, lo_odu_type,
                                               odu_flex_data);
        odujat_field_CONV_GAIN_SELECT_set(odujat_buffer, odujat_handle, chnl, conv_gain);
    }
    
    odujat_buffer_flush(odujat_buffer);
    /* access DB to get calendar entries for the channel */
    
    /* retrieve the database for the current scheduler state */
     /* query the record to update */
    if (odujat_handle->cfg.odujat_type_id == ODUJAT_FMF1)
    {
        PMC_LOG_TRACE("ODUJAT instance = %u, key is mo = %u\n", odujat_handle->cfg.odujat_type_id, chnl);        
        
        if (mux_stage == 0)
        {
            /* no stage muxing - passthru */
            key.calendar_entry = DO_NOT_USE_KEY;
            key.ho_channel = chnl;
            key.mo_channel = DO_NOT_USE_KEY;
            key.lo_channel = DO_NOT_USE_KEY;
            key.trib_slot  = DO_NOT_USE_KEY;
        }    
        else {    
            key.calendar_entry = DO_NOT_USE_KEY;
            key.ho_channel = DO_NOT_USE_KEY;
            key.mo_channel = chnl;
            key.lo_channel = DO_NOT_USE_KEY;
            key.trib_slot  = DO_NOT_USE_KEY;     
        }
    }
    else 
    {
        PMC_LOG_TRACE("ODUJAT instance = %u, key is lo = %u\n", odujat_handle->cfg.odujat_type_id, chnl);
        
        if (mux_stage == 0)
        {
            /* no stage muxing - passthru */
            key.calendar_entry = DO_NOT_USE_KEY;
            key.ho_channel = chnl;
            key.mo_channel = DO_NOT_USE_KEY;
            key.lo_channel = DO_NOT_USE_KEY;
            key.trib_slot  = DO_NOT_USE_KEY;
        }
        else if (mux_stage == 1)
        {
            /* if one stage muxing and passthru in FMF2 */
            key.calendar_entry = DO_NOT_USE_KEY;
            key.ho_channel = DO_NOT_USE_KEY;
            key.mo_channel = chnl;
            key.lo_channel = DO_NOT_USE_KEY;
            key.trib_slot  = DO_NOT_USE_KEY;
        }
        else {
            key.calendar_entry = DO_NOT_USE_KEY;
            key.ho_channel = DO_NOT_USE_KEY;
            key.mo_channel = DO_NOT_USE_KEY;
            key.lo_channel = chnl;
            key.trib_slot  = DO_NOT_USE_KEY;
        }
    }   

    /* find the total number of records that matches the MO or LO channel ID */
    num_of_recs = gen_db_query_count(&(odujat_handle->base),
                                    odujat_handle->cfg.db_id,
                                    odujat_handle->cfg.db_handle, (void *)&key);
    
    PMC_LOG_TRACE("num_of_recs = %u\n", num_of_recs);

    if (num_of_recs == 0)
    {
        PMC_RETURN(ODUJAT_ERR_DB_ENTRY_NOT_FOUND);
    }    
 
    if (PMC_SUCCESS == ret_val)
    {
        ret_val = cfc_fifo_build(odujat_handle->cfc_handle, chnl, num_of_recs);
    }                                     

    /* configure FIFO_LIMIT */
    if (PMC_SUCCESS == ret_val)
    {
        ret_val = odujat_fifo_limit_set(odujat_handle, lo_odu_type);
    }       
    
    /* configure MC_NUMERATOR */
    /* mc_numerator must be configured prior to configuring oduksc core */
    if (PMC_SUCCESS == ret_val)
    {
        ret_val = odujat_chnl_mc_numer_cfg(odujat_handle, chnl, lo_odu_type,
                                           num_of_recs, &mc_num); /* odu_flex_data, &mc_num); */
    }  
    
    if (PMC_SUCCESS == ret_val)
    {
        /* ODUKSC credit generator configuration */
        ret_val = oduksc_odujat_chnl_prov(odujat_handle->oduksc_handle, chnl,
                                          ho_odu_type, lo_odu_type, client_rate,
                                          num_of_recs, mc_num); /*odu_flex_data, mc_num);*/
    
        /* Update channel state */
        odujat_handle->var.ch_ctxt_ptr[chnl].chnl_state = ODUJAT_CHNL_PROVISION_PENDING;
    }

         
    if (PMC_SUCCESS == ret_val)
    {
        if (super_prov == FALSE)
        {
            ret_val = oduksc_core_cgen_chnl_update(odujat_handle->oduksc_handle, chnl);
        }
    }                                                                
    
    if (PMC_SUCCESS == ret_val)
    {
        /* Update channel state */
        odujat_handle->var.ch_ctxt_ptr[chnl].chnl_state = ODUJAT_CHNL_PROVISIONED;
    }                               
    
   
    odujat_buffer_init(odujat_buffer, odujat_handle);
    /* odujat_buffer = 182 */
    odujat_field_CND_DELAY_set(odujat_buffer, odujat_handle, chnl, 182);
    
    /* FIFO_LEVEL_LIMIT */
    odujat_field_FIFO_LEVEL_LIMIT_set(odujat_buffer, odujat_handle, chnl, 384);   
    odujat_buffer_flush(odujat_buffer);

    PMC_RETURN(ret_val);
    
} /* odujat_chnl_prov */    

/*******************************************************************************
*  odujat_chnl_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates a provisioned channel.
*
* INPUTS:
*   *odujat_handle      - pointer to ODUJAT handle instance
*   chnl                - channel ID to activate
*                         Valid range: 0 - 95
*   super_prov          - boolean value to determine if this is used for super_prov
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odujat_chnl_activate(odujat_handle_t *odujat_handle,
                                      UINT32 chnl, BOOL8 super_prov)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();

    if (odujat_handle->var.ch_ctxt_ptr[chnl].chnl_state != ODUJAT_CHNL_PROVISIONED)
    {
        /* channel is not in provisioned state (e.g. it is either in start state or has been already provisioned and activated) */
        PMC_RETURN(ODUJAT_CHNL_STATE_NOT_PROVISIONED);
    }

    /* Activate ODUKSC channel */
    if (super_prov == FALSE)
    {
        ret_val = oduksc_chnl_activate(odujat_handle->oduksc_handle, chnl);
    }
    else {
        ret_val = oduksc_chnl_activate_super_prov(odujat_handle->oduksc_handle,
                                                  chnl);
    }                                               
  
    /* Activate CFC */
    if (PMC_SUCCESS == ret_val)
    {
        ret_val = cfc_fifo_activate(odujat_handle->cfc_handle, chnl);
    }
    
    if (PMC_SUCCESS == ret_val)
    {                                  
        /* Update channel state */
        odujat_handle->var.ch_ctxt_ptr[chnl].chnl_state = ODUJAT_CHNL_ACTIVATED;
    }

    PMC_RETURN(ret_val);

} /* odujat_chnl_activate */

/*******************************************************************************
*  odujat_oduksc_page_swap
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Swap the standby and active pages for the credit generator.
*   It is assumed the credits page is properly configured for all
*   channel. This function is used when multiple channels need to be updated
*   simultaneously with a single page swap.
*   This function does not configure the new offline page after the page
*   swap to match the first page.
*
* INPUTS:
*   *odujat_handle    - pointer to ODUJAT handle instance
*   *chnl_ids_ptr     - pointer to an array of channel IDs
*   num_lo_chnls      - number of channels in the 'chnl_ids_ptr' array
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
PUBLIC PMC_ERROR odujat_oduksc_page_swap(odujat_handle_t *odujat_handle,
                                         UINT32 *chnl_ids_ptr,
                                         UINT32 num_lo_chnls,
                                         BOOL update_scbs3)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    result = oduksc_all_chnls_update(odujat_handle->oduksc_handle,
                                     chnl_ids_ptr,
                                     num_lo_chnls,
                                     FALSE);
    
    PMC_RETURN(result);
} /* odujat_oduksc_page_swap */                                          

/*******************************************************************************
*  odujat_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates a provisioned channel.
*
* INPUTS:
*   *odujat_handle      - pointer to ODUJAT handle instance
*   chnl                - channel ID to activate
*                         Valid range: 0 - 95
*   mux_stage           - mux stage
*   mo_odu3e2_3e1       - boolean to indicate if MO is ODU3e1 or ODU3e2 \n
*                         TRUE: MO is ODU3e1 or ODU3e2 \n
*                         FALSE: MO is not ODU3e1 or ODU3e2
*   super_deprov        - boolean to indicate if we are in super-deprov mode \n
*                         TRUE: super-deprov mode. oduksc update is not performed \n
*                         FALSE: normal deprov mode. oduksc update is performed
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
PUBLIC PMC_ERROR odujat_chnl_deprov(odujat_handle_t *odujat_handle, 
                                    UINT32 chnl,
                                    UINT32 mux_stage, 
                                    BOOL8 mo_odu3e2_3e1,
                                    BOOL8 super_deprov)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 ipt_valid;
    UINT32 ipt_id;
   
    UINT32 i;
    
    PMC_ENTRY();
    
    if (odujat_handle->var.ch_ctxt_ptr[chnl].chnl_state != ODUJAT_CHNL_PROVISIONED)
    {
        /* channel is not in provisioned state */
        PMC_RETURN(ODUJAT_CHNL_STATE_NOT_PROVISIONED);
    }
    
    ret_val = odujat_chnl_init(odujat_handle, chnl, ODUJAT_CHNL_IN_BYPASS);
    
    if (PMC_SUCCESS == ret_val)
    {
        /* deprovision ODUKSC channel */
        ret_val = oduksc_chnl_deprov(odujat_handle->oduksc_handle, chnl);
    }
    
    /* Destroy Channelize FIFO CFC */
    if (PMC_SUCCESS == ret_val)
    {
        ret_val = cfc_fifo_destroy(odujat_handle->cfc_handle, chnl); 
        
        /* check if we missed one entry for MO ODU3e1, ODU3e2 cases */
        if (mo_odu3e2_3e1)
        {
            for (i = 0; i < 96; i++)
            {
                cfc_ipt_entry_get(odujat_handle->cfc_handle, 
                                  i,
                                  &ipt_valid,
                                  &ipt_id);
                if (ipt_id == chnl && ipt_valid == 1)
                {
                    ret_val = cfc_ipt_cfg(odujat_handle->cfc_handle, 
                                          i, 0, 0x7F, 0);
                }
            }
        }                                                                                                    
    }

    if (PMC_SUCCESS == ret_val)
    {
        odujat_handle->var.ch_ctxt_ptr[chnl].chnl_state = ODUJAT_CHNL_DEPROVISION_PENDING;

        if (super_deprov == FALSE)
        {
            ret_val = oduksc_core_cgen_chnl_update(odujat_handle->oduksc_handle, 
                                                   chnl);
        }          
    }

    if (PMC_SUCCESS == ret_val)
    {
        /* Update channel state */
        odujat_handle->var.ch_ctxt_ptr[chnl].chnl_state = ODUJAT_CHNL_START;
    } 
    
    PMC_RETURN(ret_val);
    
} /* odujat_chnl_deprov */

/*******************************************************************************
*  odujat_chnl_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates a provisioned channel.
*
* INPUTS:
*   *odujat_handle    - pointer to ODUJAT handle instance
*   chnl              - channel ID to deactivate
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
PUBLIC PMC_ERROR odujat_chnl_deactivate(odujat_handle_t *odujat_handle,
                                        UINT32 chnl)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    ret_val = odujat_chnl_deactivate_internal(odujat_handle, chnl, FALSE);
    
    PMC_RETURN(ret_val);
                                                      
} /* odujat_chnl_deactivate */


/*******************************************************************************
*  odujat_chnl_deactivate_super_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates a provisioned channel.
*
* INPUTS:
*   *odujat_handle    - pointer to ODUJAT handle instance
*   chnl              - channel ID to deactivate
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
PUBLIC PMC_ERROR odujat_chnl_deactivate_super_deprov(odujat_handle_t *odujat_handle,
                                                     UINT32 chnl)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    ret_val = odujat_chnl_deactivate_internal(odujat_handle, chnl, TRUE);
    
    PMC_RETURN(ret_val);
                                                      
} /* odujat_chnl_deactivate_super_deprov */

/*******************************************************************************
*  odujat_oduksc_chnl_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provisions a channel for the specified rate.
*
* INPUTS:
*   *odujat_handle    - pointer to ODUJAT handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   chnl_state        - Enum, see oduksc_chnl_state_t
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
PUBLIC PMC_ERROR odujat_oduksc_chnl_state_set(odujat_handle_t *odujat_handle,
                                              UINT32 chnl,
                                              oduksc_chnl_state_t chnl_state)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    
    ret_val = oduksc_chnl_state_set(odujat_handle->oduksc_handle, chnl, chnl_state);
    
    PMC_RETURN(ret_val);
} /* odujat_oduksc_chnl_state_set */


/*******************************************************************************
*  odujat_chnl_deactivate_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates a provisioned channel.
*
* INPUTS:
*   *odujat_handle    - pointer to ODUJAT handle instance
*   chnl              - channel ID to deactivate
*                       Valid range: 0 - 95
*   super_prov        - boolean value to indicate if we are in super-prov mode \n
*                       TRUE: super-prov mode. oduksc update is not performed \n
*                       FALSE: normal mode. oduksc update is performed
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
PRIVATE PMC_ERROR odujat_chnl_deactivate_internal(odujat_handle_t *odujat_handle,
                                                  UINT32 chnl,
                                                  BOOL8 super_prov)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (odujat_handle->var.ch_ctxt_ptr[chnl].chnl_state != ODUJAT_CHNL_ACTIVATED)
    {
        /* channel is not in activated or reprovisioned state */
        PMC_RETURN(ODUJAT_CHNL_STATE_NOT_ACTIVATED);
    }

    /* Disable ODUKSC base credit generation */
    if (super_prov == TRUE)
    {
        ret_val = oduksc_chnl_deactivate_super_deprov(odujat_handle->oduksc_handle, chnl);
    }
    else {
        ret_val = oduksc_chnl_deactivate(odujat_handle->oduksc_handle, chnl);
    }
                                                      
    /* Put the CFC fifo in reset */
    if (PMC_SUCCESS == ret_val)
    {
        ret_val = cfc_fifo_deactivate(odujat_handle->cfc_handle, chnl);
    }    
     
    if (PMC_SUCCESS == ret_val)
    {                        
        /* Update channel state */
        odujat_handle->var.ch_ctxt_ptr[chnl].chnl_state = ODUJAT_CHNL_PROVISIONED;
    }
       
    PMC_RETURN(ret_val);
} /* odujat_chnl_deactivate_internal */

/*******************************************************************************
*  odujat_fifo_limit_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures FIFO_LIMIT which marks the fifo fill level limit. When the FIFO
*   fill level reaches the limit specified by this API, data will be dequeued 
*   from the channelized data FIFO.
*
* INPUTS:
*   *odujat_handle      - pointer to ODUJAT handle instance
*   lo_odu_type         - Enum for LO ODU container. See util_global_odukp_type_t
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
PUBLIC PMC_ERROR odujat_fifo_limit_set(odujat_handle_t *odujat_handle,
                                       util_global_odukp_type_t lo_odu_type)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 fifo_limit;
    
    PMC_ENTRY();
    
    if (lo_odu_type == UTIL_GLOBAL_ODU0)
    {
        fifo_limit = 2;
    }
    else {
        fifo_limit = 1;
    }
       
    odujat_field_FIFO_LIMIT_set(NULL, odujat_handle, fifo_limit);
       
    PMC_RETURN(result);
} /* odujat_fifo_limit_set */

/*******************************************************************************
*  odujat_chnl_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures FIFO_LIMIT which marks the fifo fill level limit. When the FIFO
*   fill level reaches the limit specified by this API, data will be dequeued 
*   from the channelized data FIFO.
*
* INPUTS:
*   *odujat_handle  - pointer to ODUJAT handle instance
*   chnl            - channel number
*   mode            - 0: disable ODUJAT
*                     1: enable ODUJAT
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
PUBLIC PMC_ERROR odujat_chnl_enable(odujat_handle_t *odujat_handle, 
                                    UINT32 chnl,
                                    BOOL8 mode)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    odujat_field_JAT_ENABLE_set(NULL, odujat_handle, chnl, mode);
       
    PMC_RETURN(result);
} /* odujat_chnl_enable */


/*******************************************************************************
* odujat_chnl_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean register and memory context of specied channels. 
*                                                                                                                                                     
* INPUTS:                                                                       
*   *odujat_handle      - pointer to ODUJAT handle instance
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
PUBLIC void odujat_chnl_clean(odujat_handle_t *odujat_handle,
                              UINT32 *chnl,
                              UINT32 num_chnl)
{   
    UINT32 chnl_itr;

    PMC_ENTRY();

    oduksc_chnl_clean(odujat_handle->oduksc_handle,chnl,num_chnl);
    cfc_fifo_clean(odujat_handle->cfc_handle,chnl,num_chnl);
    
    for (chnl_itr = 0; chnl_itr < num_chnl; chnl_itr ++)
    {
        (void) odujat_chnl_init(odujat_handle, chnl[chnl_itr], ODUJAT_CHNL_IN_BYPASS);
        odujat_handle->var.ch_ctxt_ptr[chnl[chnl_itr]].chnl_state =  ODUJAT_CHNL_START;
    }
     
    PMC_RETURN();
} /* odujat_chnl_clean */


/*******************************************************************************
* odujat_int_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   unchannelized interrupt table.
*   
*
* INPUTS:
*   *odujat_handle    - pointer to ODUJAT handle instance
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
PUBLIC PMC_ERROR odujat_int_enable(odujat_handle_t *odujat_handle,
                                   odujat_int_t *int_table_ptr,
                                   odujat_int_t *int_en_table_ptr,
                                   BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();    
    
#ifdef ODUJAT_INT
#undef ODUJAT_INT
#endif
#define ODUJAT_INT    UTIL_GLOBAL_INT_ENABLE_SINGLE
        
    ODUJAT_INT_TABLE_DEFINE();    

    PMC_RETURN(result);
} /* odujat_int_enable */

/*******************************************************************************
* odujat_int_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   unchannelized interrupt table.
*   
*
* INPUTS:
*   *odujat_handle    - pointer to ODUJAT handle instance
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
PUBLIC PMC_ERROR odujat_int_clear(odujat_handle_t *odujat_handle,
                                  odujat_int_t *int_table_ptr,
                                  odujat_int_t *int_en_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    
    #ifdef ODUJAT_INT
    #undef ODUJAT_INT
    #endif
    #define ODUJAT_INT UTIL_GLOBAL_INT_CLEAR_SINGLE
    
    ODUJAT_INT_TABLE_DEFINE();
    
    PMC_RETURN(result);
} /* odujat_int_clear */

/*******************************************************************************
* odujat_int_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the unchannelized interrupt table.
*   
*
* INPUTS:
*   *odujat_handle    - pointer to ODUJAT handle instance
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
PUBLIC PMC_ERROR odujat_int_retrieve(odujat_handle_t *odujat_handle,
                                     odujat_int_t *filt_table_ptr,
                                     odujat_int_t *int_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(odujat_handle, odujat, UTIL_GLOBAL_INT_OPTIMIZE);
    
    #ifdef ODUJAT_INT
    #undef ODUJAT_INT
    #endif
    #define ODUJAT_INT UTIL_GLOBAL_INT_RETRIEVE_SINGLE
    
    ODUJAT_INT_TABLE_DEFINE();

    UTIL_GLOBAL_INT_BUFFER_FLUSH(odujat_handle, odujat);

    PMC_RETURN(result);
} /* odujat_int_retrieve */

/*******************************************************************************
* odujat_int_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled unchannelized 
*   interrupt table.
*   
*
* INPUTS:
*   *odujat_handle    - pointer to ODUJAT handle instance
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
PUBLIC PMC_ERROR odujat_int_enabled_check(odujat_handle_t *odujat_handle,
                                          odujat_int_t *int_en_table_ptr,
                                          BOOL8 *int_found_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
        
    #ifdef ODUJAT_INT
    #undef ODUJAT_INT
    #endif
    #define ODUJAT_INT UTIL_GLOBAL_INT_CHECK_SINGLE
    
    ODUJAT_INT_TABLE_DEFINE();
    
    PMC_RETURN(result);
} /* odujat_int_enabled_check */

/*******************************************************************************
* odujat_int_chnl_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *odujat_handle    - pointer to ODUJAT handle instance
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
PUBLIC PMC_ERROR odujat_int_chnl_enable(odujat_handle_t *odujat_handle,
                                        UINT32 chnl_id,
                                        odujat_int_chnl_t *int_table_ptr,
                                        odujat_int_chnl_t *int_en_table_ptr,
                                        BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();    

    result = odujat_int_validate(odujat_handle, chnl_id, int_en_table_ptr);
    
    /* CFC */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_enable(odujat_handle->cfc_handle,
                                     chnl_id,
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, cfc),
                                     &int_en_table_ptr->cfc,
                                     enable);
    }

    PMC_RETURN(result);
} /* odujat_int_chnl_enable */

/*******************************************************************************
* odujat_int_chnl_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *odujat_handle    - pointer to ODUJAT handle instance
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
PUBLIC PMC_ERROR odujat_int_chnl_clear(odujat_handle_t *odujat_handle,
                                       UINT32 chnl_id,
                                       odujat_int_chnl_t *int_table_ptr,
                                       odujat_int_chnl_t *int_en_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    result = odujat_int_validate(odujat_handle, chnl_id, (void *)1);
    
    /* CFC */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_clear(odujat_handle->cfc_handle,
                                    chnl_id,
                                    UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, cfc),
                                    UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, cfc));
    }

    PMC_RETURN(result);
} /* odujat_int_chnl_clear */

/*******************************************************************************
* odujat_int_chnl_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   *odujat_handle    - pointer to ODUJAT handle instance
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
PUBLIC PMC_ERROR odujat_int_chnl_retrieve(odujat_handle_t *odujat_handle,
                                          UINT32 chnl_id,
                                          odujat_int_chnl_t *filt_table_ptr,
                                          odujat_int_chnl_t *int_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    result = odujat_int_validate(odujat_handle, chnl_id, int_table_ptr);

    /* CFC */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_retrieve(odujat_handle->cfc_handle,
                                       chnl_id,
                                       UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, cfc),
                                       &int_table_ptr->cfc);
    }

    PMC_RETURN(result);
} /* odujat_int_chnl_retrieve */

/*******************************************************************************
* odujat_int_chnl_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   *odujat_handle    - pointer to ODUJAT handle instance
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
PUBLIC PMC_ERROR odujat_int_chnl_enabled_check(odujat_handle_t *odujat_handle,
                                               UINT32 chnl_id,
                                               odujat_int_chnl_t *int_en_table_ptr,
                                               BOOL8 *int_found_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    result = odujat_int_validate(odujat_handle, chnl_id, int_found_ptr);    
    
    /* CFC */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_enabled_check(odujat_handle->cfc_handle,
                                            chnl_id,
                                            UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, cfc),
                                               int_found_ptr);
    }
    
    PMC_RETURN(result);
} /* odujat_int_chnl_enabled_check */


/*******************************************************************************
*  odujat_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize ODUJAT var_t variables.
*
* INPUTS:
*   *odujat_handle   - pointer to ODUJAT handle instance
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
PRIVATE void odujat_var_default_init(odujat_handle_t *odujat_handle)
{
    /* variable declaration */
    UINT32 chnl;

    PMC_ENTRY();
    
    /* argument checking */
    PMC_ASSERT(odujat_handle!=NULL, ODUJAT_ERR_INVALID_PTR, 0, 0);

    /* Configure default ODUJAT variable parameters for each channel */
    for (chnl = 0; chnl < odujat_handle->cfg.max_chnl; chnl++) 
    {
        odujat_handle->var.ch_ctxt_ptr[chnl].chnl_state  = ODUJAT_CHNL_START;
        odujat_handle->var.ch_ctxt_ptr[chnl].chnl_mode   = ODUJAT_CHNL_IN_BYPASS;
    }
    
    PMC_RETURN();
} /* odujat_var_default_init */

/*******************************************************************************
*  odujat_ghao_resize_fifo
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resize the CFC fifo to the specified number of tribslots.
*
* INPUTS:
*   *odujat_handle         - pointer to ODUJAT handle instance
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
PUBLIC PMC_ERROR odujat_ghao_resize_fifo(odujat_handle_t *odujat_handle,
                                         UINT32 chnl,
                                         UINT32 oduflex_num_tribslots)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    PMC_ENTRY();

    /* Resize CFC FIFO */
    ret_val = cfc_fifo_resize(odujat_handle->cfc_handle, chnl, oduflex_num_tribslots);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* odujat_ghao_resize_fifo */

/*******************************************************************************
* odujat_int_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   *odujat_handle   - pointer to ODUJAT handle instance
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
PRIVATE PMC_ERROR odujat_int_validate(odujat_handle_t *odujat_handle,
                                      UINT32 chnl_id,
                                      void *non_null_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != odujat_handle, ODUJAT_ERR_INVALID_ARGS, 0, 0);
    
    if (chnl_id >= odujat_handle->cfg.max_chnl)
    {     
        result = ODUJAT_ERR_INVALID_ARGS;
    }
    if (PMC_SUCCESS == result && NULL == non_null_ptr)
    {
        result = ODUJAT_ERR_INVALID_ARGS;
    }
    PMC_RETURN(result);
} /* odujat_int_validate */

/*
** End of file
*/
