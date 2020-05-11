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
*   DESCRIPTION :
*       Main D8_TX_2X source file.
* 
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/
#include "d8_tx_2x_loc.h"
#include "d8_tx_2x_io.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
#define D8_TX_2X_PATT_GEN_SEL_NORMAL_DATA_VAL 0
#define D8_TX_2X_PATT_GEN_SEL_PRBS_DATA_VAL 1

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
const char D8_TX_2X_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    D8_TX_2X_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char D8_TX_2X_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward References
*/
PRIVATE void d8_tx_2x_loopback_cfg(d8_tx_2x_handle_t *handle,
                                   UINT8 serdes_id,
                                   BOOL8 enable);

PRIVATE void d8_tx_2x_var_default_init(d8_tx_2x_var_t *var_ptr);

PRIVATE PMC_ERROR d8_tx_2x_ctxt_audit(d8_tx_2x_handle_t *d8_tx_2x_handle,
                                      d8_tx_2x_var_t    *d8_tx_2x_var_clean,
                                      BOOL8              serdes_reset);

PUBLIC void d8_tx_2x_slice_cleanup(d8_tx_2x_handle_t *d8_tx_2x_handle,
                                   d8_tx_2x_var_t    *d8_tx_2x_var_clean);

PRIVATE BOOL8 d8_tx_2x_patt_gen_status_get(d8_tx_2x_handle_t *handle, 
                                           UINT8              serdes_id);

PRIVATE PMC_ERROR d8_tx_2x_validate_usr_patt(util_serdes_patt_cfg_t * patt_cfg);

PRIVATE PMC_ERROR util_patt_mode_to_d8_tx_2x_mode_conv(util_patt_mode_t      util_patt_mode,
                                                       d8_tx_2x_patt_mode_t *d8_tx_2x_mode_ptr);
PRIVATE PMC_ERROR util_d8_tx_2x_mode_to_patt_mode_conv(d8_tx_2x_patt_mode_t d8_tx_2x_mode,
                                                       util_patt_mode_t     *util_patt_mode_ptr);
/*
** Public Functions
*/
/*******************************************************************************
* d8_tx_2x_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a D8_TX_2X block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the D8_TX_2X block relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - pointer to TSB name
*    gen_bus_width        - Pattern generator bus width
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   d8_tx_2x_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC d8_tx_2x_handle_t *d8_tx_2x_ctxt_create(pmc_handle_t        *parent, 
                                               UINT32               base_address, 
                                               pmc_sys_handle_t    *sys_handle, 
                                               const char          *tsb_name,
                                               UINT8                gen_bus_width)
{
    d8_tx_2x_handle_t *d8_tx_2x_handle;

    PMC_ENTRY();    

    d8_tx_2x_handle = (d8_tx_2x_handle_t*)PMC_CTXT_CALLOC(sizeof(d8_tx_2x_handle_t), parent);
    
    pmc_handle_init(parent, d8_tx_2x_handle, sys_handle, PMC_MID_DIGI_D8_TX_2X_SERDES, tsb_name, base_address);

    /* Register log strings. */
    pmc_log_block_strings_register(D8_TX_2X_LOG_ERR_STRINGS[0], D8_TX_2X_LOG_ERR_TABLE_BASE, D8_TX_2X_LOG_ERR_COUNT);

    /* allocate var context */
    d8_tx_2x_handle->var =  (d8_tx_2x_var_t *) PMC_CTXT_CALLOC(sizeof(d8_tx_2x_var_t) * D8_TX_2X_MAX_NUM_SERDES, parent); 
    PMC_CTXT_REGISTER_PTR(&d8_tx_2x_handle->var, parent);
    d8_tx_2x_handle->cfg.patt_gen_bus_width = gen_bus_width;

    PMC_RETURN(d8_tx_2x_handle);
} /* d8_tx_2x_ctxt_create */  
 
/*******************************************************************************
* d8_tx_2x_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a D8_TX_2X block instance.
*
*
* INPUTS:
*   *d8_tx_2x_handle        - pointer to D8_TX_2X handle instance
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
PUBLIC void d8_tx_2x_ctxt_destroy(d8_tx_2x_handle_t *d8_tx_2x_handle)
{
    PMC_ENTRY();
    PMC_CTXT_FREE(&d8_tx_2x_handle->var, d8_tx_2x_handle);
    PMC_CTXT_FREE(&d8_tx_2x_handle, d8_tx_2x_handle);

    PMC_RETURN();
} /* d8_tx_2x_ctxt_destroy */

/*******************************************************************************
* d8_tx_2x_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a D8_TX_2X block handle instance.
*
*
* INPUTS:
*   *d8_tx_2x_handle        - pointer to D8_TX_2X handle instance
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
PUBLIC void d8_tx_2x_handle_init(d8_tx_2x_handle_t *d8_tx_2x_handle)
{
    UINT32 itr;
    PMC_ENTRY();

    for (itr = 0; itr < D8_TX_2X_MAX_NUM_SERDES; itr ++)
    {
        d8_tx_2x_var_default_init(&d8_tx_2x_handle->var[itr]);
    }

    PMC_RETURN();
} /* d8_tx_2x_handle_init */

/*******************************************************************************
* d8_tx_2x_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes the var context to its default values
*
* INPUTS:
*   *var_ptr          - pointer to d8_tx_2x variable instance.
*
* OUTPUTS:
*   None
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void d8_tx_2x_var_default_init(d8_tx_2x_var_t *var_ptr)
{

    PMC_ENTRY();

    var_ptr->reset = TRUE;
    var_ptr->patt_gen_enable = FALSE;
    var_ptr->loopback_enable = FALSE;

    PMC_RETURN();
} /*d8_tx_2x_var_default_init  */

/*******************************************************************************
* d8_tx_2x_loopback_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function activate loopback (TX2RX/Diagnostic Loopback).
*
* INPUTS:
*   *handle         - pointer to d8_tx_2x handle.
*   serdes_id       - serdes identifier.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error code is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR d8_tx_2x_loopback_activate(d8_tx_2x_handle_t *handle,
                                            UINT8              serdes_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, D8_TX_2X_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(serdes_id < D8_TX_2X_MAX_NUM_SERDES,D8_TX_2X_LOG_ERR_SERDES_ID_INVALID,0,0);
    
    /* return an error if a loopback is already active */
    if (handle->var[serdes_id].loopback_enable)
    {
        PMC_RETURN(D8_TX_2X_LOOPBACK_ALREADY_CONFIGURED);
    }
    /* configure the loopback */
    d8_tx_2x_loopback_cfg(handle,serdes_id,TRUE);
    
    handle->var[serdes_id].loopback_enable = TRUE;
    PMC_RETURN(rc);
} /* d8_tx_2x_loopback_activate */

/*******************************************************************************
* d8_tx_2x_loopback_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function deactivate loopback (TX2RX/Disgnostic)
*
* INPUTS:
*   *handle         - pointer to d8_tx_2x handle.
*   serdes_id       - serdes identifier.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error code is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR d8_tx_2x_loopback_deactivate(d8_tx_2x_handle_t *handle,
                                              UINT8              serdes_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, D8_TX_2X_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(serdes_id < D8_TX_2X_MAX_NUM_SERDES,D8_TX_2X_LOG_ERR_SERDES_ID_INVALID,0,0);

    if (handle->var[serdes_id].loopback_enable != TRUE) 
    {
        PMC_RETURN(D8_TX_2X_LOOPBACK_ALREADY_NOT_CONFIGURED);
    }
     
    /* configure the loopback */
    d8_tx_2x_loopback_cfg(handle,serdes_id,FALSE);
    /* update status */
    handle->var[serdes_id].loopback_enable = FALSE;

    PMC_RETURN(rc);
} /* d8_tx_2x_loopback_deactivate */

/*******************************************************************************
* d8_tx_2x_loopback_activate_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function return serdes loopback activation status.
*
* INPUTS:
*   *handle         - pointer to d8_tx_2x handle.
*   serdes_id       - serdes identifier.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   TRUE when loopback is activated, FALSE otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 d8_tx_2x_loopback_activate_status_get(d8_tx_2x_handle_t *handle,
                                                   UINT8              serdes_id)
{
    BOOL8 rc;
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, D8_TX_2X_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(serdes_id < D8_TX_2X_MAX_NUM_SERDES,D8_TX_2X_LOG_ERR_SERDES_ID_INVALID,0,0);

    rc = (0 != d8_tx_2x_field_P2S_DLB_EN_get(NULL,handle,serdes_id) &&
          0 != d8_tx_2x_field_PSLB_CLK_DEGLITCH_EN_get(NULL,handle,serdes_id)) ? TRUE : FALSE;

    PMC_RETURN(rc);
} /* d8_tx_2x_loopback_activate_status_get */


/*******************************************************************************
* d8_tx_2x_patt_mon_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function configures one serdes D8_2X_RX pattern monitor: 
*    - It sets pattern mode,
*    - It  enables/disables pattern inversion check. 
*    - It activates/disactivates pattern monitor.
*
*
* INPUTS:
*   *handle                - pointer to d8_2x_rx handle
*   serdes_id              - serdes identifier
*   *patt_mon_cfg_ptr      - pointer to pattern monitor configuration
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS            - when conversion is successful 
*                            otherwise a error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR d8_tx_2x_patt_mon_cfg(d8_tx_2x_handle_t          *handle,
                                       UINT8                       serdes_id,
                                       util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr)
{
    d8_tx_2x_buffer_t d8_tx_2x_buf;
    d8_tx_2x_patt_mode_t d8_tx_2x_mode;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;

    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(handle != NULL, D8_TX_2X_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(patt_mon_cfg_ptr != NULL, D8_TX_2X_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes ID */
    PMC_ASSERT(serdes_id < D8_TX_2X_MAX_NUM_SERDES, D8_TX_2X_LOG_ERR_SERDES_ID_INVALID,0,0);
    
    
    if (TRUE == patt_mon_cfg_ptr->patt_cfg.enable)
    {        
        /* Remove serdes pattern monitor  reset */
        if (D8_TX_2X_SERDES_A_IDX == serdes_id)
        {
            d8_tx_2x_field_PATT_MON_A_RSTB_set(NULL, handle, 1);
        }
        else
        {
            d8_tx_2x_field_PATT_MON_B_RSTB_set(NULL, handle, 1);            
        }

        result = util_patt_mode_to_d8_tx_2x_mode_conv(patt_mon_cfg_ptr->patt_cfg.mode, &d8_tx_2x_mode);
        
        /*  set user-defined pa_monttern when mode matches */
        if (PMC_SUCCESS == result)
        {       
            /*  set generator user-defined pattern when mode matches */
            if (D8_TX_2X_PATT_MODE_USER_DEF == d8_tx_2x_mode ||
                D8_TX_2X_PATT_MODE_CJAT == d8_tx_2x_mode)
            {
                result = d8_tx_2x_validate_usr_patt(&patt_mon_cfg_ptr->patt_cfg);
                if (PMC_SUCCESS != result)
                {
                    PMC_RETURN(result);
                }
                
                
                for (i = 0; i < D8_TX_2X_USR_PATT_NUM_WORD; i ++)
                {
                    switch (i)
                    {
                    case 0:
                        d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_field_set(NULL, handle, serdes_id, 0xffffffff, 0, patt_mon_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 1:
                        d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_2_array_field_set(NULL, handle, serdes_id, 0xff, 0, patt_mon_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    default:
                        result = D8_TX_2X_LOG_ERR_INVALID_PARAMETERS;
                        break;
                    }
                }
            }
        }
        
        if (PMC_SUCCESS == result)
        {
            /* initialize d8_2x_tx buffer */
            d8_tx_2x_buffer_init(&d8_tx_2x_buf, handle);

            d8_tx_2x_field_GEN_MON_BUS_WIDTH_set(&d8_tx_2x_buf, handle, serdes_id, handle->cfg.patt_gen_bus_width);

            /* set check inv, pattern mode and enable/disable */
            d8_tx_2x_field_MON_INV_set(&d8_tx_2x_buf, handle, serdes_id, (UINT32)patt_mon_cfg_ptr->patt_cfg.inv_cfg);      
            d8_tx_2x_field_MON_MODE_set(&d8_tx_2x_buf, handle, serdes_id, (UINT32)d8_tx_2x_mode);
            d8_tx_2x_field_MON_READ_set(&d8_tx_2x_buf, handle, serdes_id, 0);
            d8_tx_2x_field_MON_EN_set(&d8_tx_2x_buf, handle, serdes_id, 0);
            
            /* flush d8_2x_tx buffer */
            d8_tx_2x_buffer_flush(&d8_tx_2x_buf);   

        }
    }
    else
    {
        d8_tx_2x_field_MON_EN_set(NULL, handle, serdes_id, 0);
    }
    
    PMC_RETURN(result);
}/*d8_tx_2x_patt_mon_cfg*/

/*******************************************************************************
* d8_tx_2x_patt_gen_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function configures one serdes D8_TX_2X pattern generator: 
*    - It sets pattern mode.
*    - It enables/disables pattern inversion.
*    - It enables/disables error injection. 
*    - It activates/disactivates pattern generator.
*    - It sets pattern mode.
*
*
* INPUTS:
*   *handle                - pointer to d8_tx_2x handle
*   serdes_id              - serdes identifier
*   *patt_gen_cfg_ptr      - pointer to pattern generator configuration
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS            - when configuration is successful 
*                            otherwise a error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR d8_tx_2x_patt_gen_cfg(d8_tx_2x_handle_t          *handle,
                                       UINT8                       serdes_id,
                                       util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr)
{
    d8_tx_2x_buffer_t d8_tx_2x_buf;
    d8_tx_2x_patt_mode_t d8_tx_2x_mode;
    UINT32 i;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(handle != NULL, D8_TX_2X_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(patt_gen_cfg_ptr != NULL, D8_TX_2X_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes ID */
    PMC_ASSERT(serdes_id < D8_TX_2X_MAX_NUM_SERDES, D8_TX_2X_LOG_ERR_SERDES_ID_INVALID,0,0);

    /* initialize d8_tx_2x buffer */
    d8_tx_2x_buffer_init(&d8_tx_2x_buf, handle);

    if (TRUE == patt_gen_cfg_ptr->patt_cfg.enable)
    {
        /* Remove reset */
        if (D8_TX_2X_SERDES_A_IDX == serdes_id)
        {
            d8_tx_2x_field_PATT_GEN_A_RSTB_set(NULL, handle, 1);
        }
        else
        {
            d8_tx_2x_field_PATT_GEN_B_RSTB_set(NULL, handle, 1);            
        }
        

        result = util_patt_mode_to_d8_tx_2x_mode_conv(patt_gen_cfg_ptr->patt_cfg.mode, &d8_tx_2x_mode);
        
        /*  set user-defined pattern when mode matches */
        if (PMC_SUCCESS == result)
        {       
            /*  set user-defined pattern when mode matches */
            if (D8_TX_2X_PATT_MODE_USER_DEF == d8_tx_2x_mode ||
                D8_TX_2X_PATT_MODE_CJAT == d8_tx_2x_mode)
            {
                result = d8_tx_2x_validate_usr_patt(&patt_gen_cfg_ptr->patt_cfg);
                if (PMC_SUCCESS != result)
                {
                    PMC_RETURN(result);
                }
                
                for (i = 0; i < D8_TX_2X_USR_PATT_NUM_WORD; i ++)
                {
                    switch (i)
                    {
                    case 0:
                        d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_field_set(NULL, handle, serdes_id, 0xffffffff, 0, patt_gen_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 1:
                        d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_field_set(NULL, handle,  serdes_id, 0xff, 0, patt_gen_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    default:
                        result = D8_TX_2X_LOG_ERR_INVALID_PARAMETERS;
                        break;
                    }
                }
            }
        }
        
        if (PMC_SUCCESS == result)
        {           
            d8_tx_2x_field_PATT_INSERT_EN_set(&d8_tx_2x_buf, handle, serdes_id, D8_TX_2X_PATT_GEN_SEL_PRBS_DATA_VAL);

            d8_tx_2x_field_GEN_MON_BUS_WIDTH_set(&d8_tx_2x_buf, handle, serdes_id, handle->cfg.patt_gen_bus_width);
            /* set check inv, pattern mode and enable/disable */
            d8_tx_2x_field_GEN_INV_set(&d8_tx_2x_buf, handle, serdes_id, (UINT32)patt_gen_cfg_ptr->patt_cfg.inv_cfg);
            d8_tx_2x_field_GEN_MODE_set(&d8_tx_2x_buf, handle, serdes_id, (UINT32)d8_tx_2x_mode);
        }
    }
    else 
    {        
        d8_tx_2x_field_PATT_INSERT_EN_set(&d8_tx_2x_buf, handle, serdes_id, 0);
    }

    d8_tx_2x_field_GEN_EN_set(&d8_tx_2x_buf, handle, serdes_id, (UINT32)patt_gen_cfg_ptr->patt_cfg.enable ? 1 : 0);

    /* flush d8_tx_2x buffer */
    d8_tx_2x_buffer_flush(&d8_tx_2x_buf);

    handle->var[serdes_id].patt_gen_enable = patt_gen_cfg_ptr->patt_cfg.enable;
    PMC_RETURN(result);
} /*d8_tx_2x_patt_gen_cfg*/

/*******************************************************************************
* d8_tx_2x_patt_gen_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function gets config of one serdes D8_TX_2X pattern generator: 
*    - It gets pattern mode.
*
*
* INPUTS:
*   *handle                - pointer to d8_tx_2x handle
*   serdes_id              - serdes identifier
*
* OUTPUTS:
*   *patt_gen_cfg_ptr      - pointer to pattern generator configuration
*
* RETURNS:
*   PMC_SUCCESS            - when configuration is successful 
*                            otherwise a error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR d8_tx_2x_patt_gen_cfg_get(d8_tx_2x_handle_t          *handle,
                                           UINT8                       serdes_id,
                                           util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr)
{
    d8_tx_2x_buffer_t d8_tx_2x_buf;
    d8_tx_2x_patt_mode_t d8_tx_2x_mode;
    UINT32 i;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(handle != NULL, D8_TX_2X_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(patt_gen_cfg_ptr != NULL, D8_TX_2X_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes ID */
    PMC_ASSERT(serdes_id < D8_TX_2X_MAX_NUM_SERDES, D8_TX_2X_LOG_ERR_SERDES_ID_INVALID,0,0);

    /* initialize d8_tx_2x buffer */
    d8_tx_2x_buffer_init(&d8_tx_2x_buf, handle);

    patt_gen_cfg_ptr->patt_cfg.enable = handle->var[serdes_id].patt_gen_enable;

    if (TRUE == patt_gen_cfg_ptr->patt_cfg.enable)
    {
        patt_gen_cfg_ptr->patt_cfg.inv_cfg = (BOOL8)d8_tx_2x_field_GEN_INV_get(&d8_tx_2x_buf, handle, serdes_id);
        
        d8_tx_2x_mode = (d8_tx_2x_patt_mode_t)d8_tx_2x_field_GEN_MODE_get(&d8_tx_2x_buf, handle, serdes_id);
        
        result = util_d8_tx_2x_mode_to_patt_mode_conv(d8_tx_2x_mode, &(patt_gen_cfg_ptr->patt_cfg.mode));
        
        /*  set user-defined pattern when mode matches */
        if (PMC_SUCCESS == result)
        {       
            /*  set user-defined pattern when mode matches */
            if (D8_TX_2X_PATT_MODE_USER_DEF == d8_tx_2x_mode ||
                D8_TX_2X_PATT_MODE_CJAT == d8_tx_2x_mode)
            {
                for (i = 0; i < D8_TX_2X_USR_PATT_NUM_WORD; i ++)
                {
                    switch (i)
                    {
                    case 0:
                        patt_gen_cfg_ptr->patt_cfg.usr_patt[i] = 
                            d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_read(NULL, handle, serdes_id);
                        break;
                    case 1:
                        patt_gen_cfg_ptr->patt_cfg.usr_patt[i] = 0xff &
                            d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_read(NULL, handle, serdes_id);
                        break;
                    default:
                        result = D8_TX_2X_LOG_ERR_INVALID_PARAMETERS;
                        break;
                    }
                }
            }
        }
    }

    /* flush d8_tx_2x buffer */
    d8_tx_2x_buffer_flush(&d8_tx_2x_buf);

    PMC_RETURN(result);
} /*d8_tx_2x_patt_gen_cfg_get*/

/*******************************************************************************
* d8_tx_2x_patt_gen_err_insert_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables one error bit insertion in generated pattern.
*
* INPUTS:
*   *handle                - pointer to d8 tx 2 slices handle.
*   serdes_id              - serdes identifier.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error code is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR d8_tx_2x_patt_gen_err_insert_cfg(d8_tx_2x_handle_t *handle,
                                                  UINT8              serdes_id)
 
{
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, D8_TX_2X_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes ID */
    if (D8_TX_2X_MAX_NUM_SERDES <= serdes_id)
    {
        PMC_RETURN(D8_TX_2X_LOG_ERR_SERDES_ID_INVALID);
    }

    /* emulate rising edge on GEN_ERR_INJ register bit */
    /* in case bit is already high */
    d8_tx_2x_field_GEN_ERR_INJ_set(NULL, handle, serdes_id, 0);
    d8_tx_2x_field_GEN_ERR_INJ_set(NULL, handle, serdes_id, 1);

    /* lower register bit  */
    d8_tx_2x_field_GEN_ERR_INJ_set(NULL, handle, serdes_id, 0);
    
    PMC_RETURN(PMC_SUCCESS);
}/*d8_tx_2x_patt_gen_err_insert_cfg*/

/*******************************************************************************
* d8_tx_2x_tx_swing_level_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function configures TX swing level of a given serdes.
*
* INPUTS:
*   *handle                - pointer to d8 tx 2 slices handle.
*   serdes_id              - serdes identifier.
*   tx_swing_level         - TX swing level new value.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error code is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR d8_tx_2x_tx_swing_level_set(d8_tx_2x_handle_t          *handle,
                                             UINT8                       serdes_id,
                                             d8_tx_2x_tx_swing_level_t   tx_swing_level)    
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, D8_TX_2X_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes ID */
    if (D8_TX_2X_MAX_NUM_SERDES <= serdes_id)
    {
        rc = D8_TX_2X_LOG_ERR_SERDES_ID_INVALID;
    }

    if (PMC_SUCCESS == rc)
    {
        /* set register */
        d8_tx_2x_field_TX_SWING_set(NULL, handle, serdes_id, (UINT32) tx_swing_level);
    }
    
    PMC_RETURN(rc);
}/*d8_tx_2x_tx_swing_level_set*/

/*******************************************************************************
* d8_tx_2x_tx_swing_level_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function returns serdes TX swing level configured value.
*
* INPUTS:
*   *handle                - pointer to d8 tx 2 slices handle.
*   serdes_id              - serdes identifier.
*
* OUTPUTS:
*   *tx_swing_level        - TX swing level configured value.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error code is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR d8_tx_2x_tx_swing_level_get(d8_tx_2x_handle_t          *handle,
                                             UINT8                       serdes_id,
                                             d8_tx_2x_tx_swing_level_t  *tx_swing_level)
 
{
    UINT32 tmp;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, D8_TX_2X_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes ID */
    if (D8_TX_2X_MAX_NUM_SERDES <= serdes_id)
    {
        rc = D8_TX_2X_LOG_ERR_SERDES_ID_INVALID;
    }

    if (PMC_SUCCESS == rc)
    {
        /* read register */
        tmp = (d8_tx_2x_tx_swing_level_t)d8_tx_2x_field_TX_SWING_get(NULL, handle, serdes_id);
        if ((UINT32)D8_TX_2X_TX_SWING_LEVEL_326 <= tmp &&
            (UINT32)LAST_D8_TX_2X_TX_SWING_LEVEL > tmp)
        {
            *tx_swing_level = (d8_tx_2x_tx_swing_level_t)tmp;
        }
        else
        {
            *tx_swing_level = LAST_D8_TX_2X_TX_SWING_LEVEL;
            rc = D8_TX_2X_TX_SWING_INVALID;
        }
    }
    
    PMC_RETURN(rc);
}/*d8_tx_swing_level_get*/

/*******************************************************************************
* util_patt_mode_to_d8_tx_2x_mode_conv 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert pattern mode defined defined in util global to d8_tx_2x pattern mode enum.
*
*
* INPUTS:
*  util_patt_mode         - util global pattern mode enum.
*
* OUTPUTS:
*  *d8_tx_2x_mode_ptr         - matching d8_tx_2x pattern mode enum.
*
* RETURNS:
*   PMC_SUCCESS                 - when conversion is successful otherwise
*                                 a descriptive error code is returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR util_patt_mode_to_d8_tx_2x_mode_conv(util_patt_mode_t        util_patt_mode,
                                                       d8_tx_2x_patt_mode_t   *d8_tx_2x_mode_ptr)
{
    PMC_ENTRY();

    switch (util_patt_mode)
    {
    case UTIL_PATT_MODE_PRBS_5:
        *d8_tx_2x_mode_ptr = D8_TX_2X_PATT_MODE_PRBS_5;
        break;  
    case UTIL_PATT_MODE_PRBS_7:
        *d8_tx_2x_mode_ptr = D8_TX_2X_PATT_MODE_PRBS_7;
        break;  
    case UTIL_PATT_MODE_PRBS_11:
        *d8_tx_2x_mode_ptr = D8_TX_2X_PATT_MODE_PRBS_11;
        break;
    case UTIL_PATT_MODE_PRBS_15:
        *d8_tx_2x_mode_ptr = D8_TX_2X_PATT_MODE_PRBS_15;
        break;
    case UTIL_PATT_MODE_PRBS_20:
        *d8_tx_2x_mode_ptr = D8_TX_2X_PATT_MODE_PRBS_20;
        break;
    case UTIL_PATT_MODE_PRBS_23:
        *d8_tx_2x_mode_ptr = D8_TX_2X_PATT_MODE_PRBS_23;
        break;
    case UTIL_PATT_MODE_PRBS_31:
        *d8_tx_2x_mode_ptr = D8_TX_2X_PATT_MODE_PRBS_31;
        break;
    case UTIL_PATT_MODE_PRBS_9:
        *d8_tx_2x_mode_ptr = D8_TX_2X_PATT_MODE_PRBS_9;
        break; 
    case UTIL_PATT_MODE_CJAT:
        *d8_tx_2x_mode_ptr = D8_TX_2X_PATT_MODE_CJAT;
        break;
    case UTIL_PATT_MODE_USER_DEF:
        *d8_tx_2x_mode_ptr = D8_TX_2X_PATT_MODE_USER_DEF;
        break;
    default:
        PMC_RETURN(D8_TX_2X_LOG_ERR_MODE_NOT_FOUND);
    }

    PMC_RETURN(PMC_SUCCESS);
        
}/*util_patt_mode_to_d8_tx_2x_mode_conv*/

/*******************************************************************************
* util_d8_tx_2x_mode_to_patt_mode_conv 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert d8_tx_2x pattern mode enum to pattern mode defined defined in util global.
*
*
* INPUTS:
*  d8_tx_2x_mode         - util global pattern mode enum.
*
* OUTPUTS:
*  *util_patt_mode_ptr   - util global pattern mode enum.
*
* RETURNS:
*   PMC_SUCCESS          - when conversion is successful otherwise
*                          a descriptive error code is returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR util_d8_tx_2x_mode_to_patt_mode_conv(d8_tx_2x_patt_mode_t d8_tx_2x_mode,
                                                       util_patt_mode_t     *util_patt_mode_ptr)
{
    PMC_ENTRY();

    switch (d8_tx_2x_mode)
    {
    case D8_TX_2X_PATT_MODE_PRBS_5:
        *util_patt_mode_ptr = UTIL_PATT_MODE_PRBS_5;
        break;  
    case D8_TX_2X_PATT_MODE_PRBS_7:
        *util_patt_mode_ptr = UTIL_PATT_MODE_PRBS_7;
        break;
    case D8_TX_2X_PATT_MODE_PRBS_11:
        *util_patt_mode_ptr = UTIL_PATT_MODE_PRBS_11;
        break;
    case D8_TX_2X_PATT_MODE_PRBS_15:
        *util_patt_mode_ptr = UTIL_PATT_MODE_PRBS_15;
        break;
    case D8_TX_2X_PATT_MODE_PRBS_20:
        *util_patt_mode_ptr = UTIL_PATT_MODE_PRBS_20;
        break;
    case D8_TX_2X_PATT_MODE_PRBS_23:
        *util_patt_mode_ptr = UTIL_PATT_MODE_PRBS_23;
        break;
    case D8_TX_2X_PATT_MODE_PRBS_31:
        *util_patt_mode_ptr = UTIL_PATT_MODE_PRBS_31;
        break;
    case D8_TX_2X_PATT_MODE_PRBS_9:
        *util_patt_mode_ptr = UTIL_PATT_MODE_PRBS_9;
        break;
    case D8_TX_2X_PATT_MODE_CJAT:
        *util_patt_mode_ptr = UTIL_PATT_MODE_CJAT;
        break;
    case D8_TX_2X_PATT_MODE_USER_DEF:
        *util_patt_mode_ptr = UTIL_PATT_MODE_USER_DEF;
        break;
    default:
        PMC_RETURN(D8_TX_2X_LOG_ERR_MODE_NOT_FOUND);
    }

    PMC_RETURN(PMC_SUCCESS);

}/*util_d8_tx_2x_mode_to_patt_mode_conv*/


/*******************************************************************************
* d8_tx_2x_validate_usr_patt 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function verifies that the length of user-defined pattern is compatible 
*   to d8_tx_2x serdes.
*
* INPUTS:
*  *patt_cfg         - pointer to util global pattern mode enum
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS                 - when user-defined pattern is compatible otherwise
*                                 a descriptive error code is returned 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR d8_tx_2x_validate_usr_patt(util_serdes_patt_cfg_t *patt_cfg)

{
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();
    PMC_ASSERT(NULL != patt_cfg,D8_TX_2X_LOG_ERR_NULL_HANDLE,0,0);
    if (patt_cfg->usr_patt_size != D8_TX_2X_USR_PATT_BIT_SIZE)
    {
        result = D8_TX_2X_LOG_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(result);
} /* util_patt_mode_to_d8_tx_2x_mode_conv */

/*******************************************************************************
* d8_tx_2x_loopback_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function configure a D8_TX_2X instance to enable/disable loopback
*
* INPUTS:
*   *handle         - pointer to D8_TX_2X handle.
*   serdes_id       - serdes identifier.
*   enable          - TRUE to enable loopback, FALSE to disable it
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void d8_tx_2x_loopback_cfg(d8_tx_2x_handle_t *handle,
                                   UINT8 serdes_id,
                                   BOOL8 enable)
{
         
    PMC_ENTRY();
    
    d8_tx_2x_field_P2S_DLB_EN_set(NULL,handle,serdes_id,enable ? 1 : 0);
    d8_tx_2x_field_PSLB_CLK_DEGLITCH_EN_set(NULL,handle,serdes_id,enable ? 1 : 0);

    PMC_RETURN();
 
} /* d8_tx_2x_loopback_cfg */

/*******************************************************************************
* d8_tx_2x_polarity_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function configure a D8_TX_2X instance to enable/disable lane polarity 
*   inversion
*
* INPUTS:
*   *handle         - pointer to D8_TX_2X handle.
*   serdes_id       - serdes identifier.
*   enable          - TRUE to enable polarity inversion, FALSE to disable it
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void d8_tx_2x_polarity_cfg(d8_tx_2x_handle_t *handle,
                                  UINT8 serdes_id,
                                  BOOL8 enable)
{
    d8_tx_2x_buffer_t b[1];
    
    PMC_ENTRY();
       
    d8_tx_2x_buffer_init(b,handle);;
    
    d8_tx_2x_field_INV_DAT_POL_set (b, handle, serdes_id, (UINT32) enable);
 
    d8_tx_2x_buffer_flush(b);   
                                            
    PMC_RETURN();
 
} /* d8_tx_2x_polarity_cfg */


/*******************************************************************************
* d8_tx_2x_polarity_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function returns  D8_TX_2X instance lane polarity 
*   inversion status
*
* INPUTS:
*   *handle         - pointer to D8_TX_2X handle.
*   serdes_id       - serdes identifier.
*
* OUTPUTS:
*  *enable          - TRUE when polarity is inversed, FALSE otherwise
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void d8_tx_2x_polarity_get(d8_tx_2x_handle_t *handle,
                                  UINT8              serdes_id,
                                  BOOL8             *enable)
{
    PMC_ENTRY();
       
    *enable = d8_tx_2x_field_INV_DAT_POL_get(NULL, handle, serdes_id) == 0x1 ? TRUE : FALSE;
 
                                            
    PMC_RETURN();
 
} /* d8_tx_2x_polarity_get */

/*******************************************************************************
* d8_tx_2x_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on d8_tx_2x handle instance.
*
* INPUTS:
*   *d8_tx_2x_handle           - pointer to d8_tx_2x handle instance
*   *restart_init_cfg_ptr      - pointer to restart init config
*    reset                     - D8_TX_2X serdes reset status
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS : 
*     WARM : register/context are coherent
*     CRASH: everything is coherent (a cleanup may has been executed)
*   D8_TX_2X_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT:
*     incoherences have been detected on communal energy configuration
*     that is incoherent with top energy state
*      WARM : ERROR
*      CRASH: TOP shall put D8_TX_2X in RESET mode
*   
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR d8_tx_2x_handle_restart_init(d8_tx_2x_handle_t       *d8_tx_2x_handle,
                                              util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                              BOOL8                           reset)
{
    PMC_ERROR           rc = PMC_SUCCESS;
    d8_tx_2x_var_t           *d8_tx_2x_var_clean;
    
    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(D8_TX_2X_LOG_ERR_STRINGS[0], D8_TX_2X_LOG_ERR_TABLE_BASE, D8_TX_2X_LOG_ERR_COUNT);
    }

    d8_tx_2x_var_clean =  (d8_tx_2x_var_t *) PMC_CALLOC(sizeof(d8_tx_2x_var_t) * D8_TX_2X_MAX_NUM_SERDES); 

    /* 
       Recover context from register space        
       - var context is partially recovered from register space (good enough to have a valid audit)
       - only OPERATIONAL channel configruation are recovered
       - anyway a partial channel config is an error in warm restart and will be cleanup in crash-restarted
       - detailed energy states are recovered
       - incoherency in register context means that the subsystem shall be shutdown
    */

    /* 
     * Audit register context and memory context 
     *   - Audit power and decide if we should just shut down the subsystem 
     *   - Create a new context that represents the intersection between memory and register space (d8_tx_2x_var_clean)
     */
    rc = d8_tx_2x_ctxt_audit(d8_tx_2x_handle,
                             d8_tx_2x_var_clean,
                             reset); 
    
    PMC_LOG_TRACE("d8_tx_2x_ctxt_audit rc = %s\n", pmc_log_string_get(rc));
    if (D8_TX_2X_ERR_CONTEXT_CLEANUP == rc && restart_init_cfg_ptr->is_warm_restart == FALSE)
    {
        /* 
         * Cleanup 
         *   - At the end, d8_tx_2x_var_clean is the new context
         *    - We keep OPERATIONAL channel defined in d8_tx_2x_var_clean
         *    - We clean (deactivate/deprov) all channels that are not OPERATIONAL
         *       - they may have be partially provisionned
         *    - We ajust power setting based on d8_tx_2x_var_clean
         */                                    
        d8_tx_2x_slice_cleanup(d8_tx_2x_handle,
                               d8_tx_2x_var_clean); 
    }

    PMC_FREE(&d8_tx_2x_var_clean);

    PMC_RETURN(rc);
} /* d8_tx_2x_handle_restart_init */

/*******************************************************************************
* d8_tx_2x_ctxt_audit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function audits D8_TX_2X handle context variable against register space content.
*
* INPUTS:
*   *d8_tx_2x_handle           - pointer to D8_TX_2X handle instance.
*   *d8_tx_2x_var_clean        - pointer to D8_TX_2X lanes context.
*    serdes_reset              - D8_TX_2X serdes reset status.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS if auditing is successful otherwise a descriptive error code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR d8_tx_2x_ctxt_audit(d8_tx_2x_handle_t *d8_tx_2x_handle,
                                      d8_tx_2x_var_t    *d8_tx_2x_var_clean,
                                      BOOL8              serdes_reset)
{
    BOOL8 serdes_ctxt_match;
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL8 lpbk_enabled = FALSE;
    UINT32 itr;
    UINT32 jtr;
    BOOL8 patt_gen_enabled;

    PMC_ENTRY();

    PMC_ASSERT(d8_tx_2x_handle != NULL, D8_TX_2X_LOG_ERR_INVALID_PARAMETERS,0,0);
    
    serdes_ctxt_match = TRUE;

    for (itr = 0; itr < D8_TX_2X_MAX_NUM_SERDES; itr += 1)
    {
        if (FALSE == serdes_reset)
        {
            /* check d8_tx_2x provisioning & activation status */            
            if (TRUE == d8_tx_2x_handle->var[itr].reset)
            {
                for (jtr = 0; jtr < D8_TX_2X_MAX_NUM_SERDES; jtr += 1)
                {
                    if (itr == jtr)
                    {
                        continue;
                    }
                    if (FALSE == d8_tx_2x_handle->var[jtr].reset)
                    {
                        break;
                    }
                }
            
                if (D8_TX_2X_MAX_NUM_SERDES == jtr)
                {
                    serdes_ctxt_match = FALSE;
                }
            }
                     
            /* check pattern generator status */
            if (TRUE == serdes_ctxt_match)
            {
                patt_gen_enabled =  d8_tx_2x_patt_gen_status_get(d8_tx_2x_handle, itr);
                if (patt_gen_enabled != d8_tx_2x_handle->var[itr].patt_gen_enable)
                {
                    serdes_ctxt_match = FALSE; 
                }                                    
            }

            /* check loopback status */
            if (TRUE == serdes_ctxt_match)
            {
                lpbk_enabled = d8_tx_2x_loopback_activate_status_get(d8_tx_2x_handle, itr);
                
                if (TRUE == serdes_ctxt_match)
                {
                    if (lpbk_enabled != d8_tx_2x_handle->var[itr].loopback_enable)
                    {
                        serdes_ctxt_match = FALSE;                        
                    }
                }
            }
        }
        else
        {
            /* check d8_tx_2x provisioning & activation status */
            if (TRUE != d8_tx_2x_handle->var[itr].reset)
            {
                serdes_ctxt_match = FALSE;
            }
        }
        /* update returned error code anf var clean (used to cleanup the context) */
        if (FALSE == serdes_ctxt_match)
        {
            if (PMC_SUCCESS == rc)
            {
                rc = D8_TX_2X_ERR_CONTEXT_CLEANUP;
            }
            d8_tx_2x_var_default_init(&d8_tx_2x_var_clean[itr]);
        }   
        else
        {
            if (FALSE == (TRUE != d8_tx_2x_handle->var[itr].reset))
            {
                PMC_MEMCPY((void*)&d8_tx_2x_var_clean[itr], (void*)&d8_tx_2x_handle->var[itr], sizeof(d8_tx_2x_var_t));
            }
        } 
    }

    PMC_RETURN(rc);
} /* d8_tx_2x_ctxt_audit */

/*******************************************************************************
* d8_tx_2x_slice_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   cleanup D8_TX_2X block context per slice.
*
*
* INPUTS:
*   *d8_tx_2x_handle        - pointer to D8_TX_2X handle instance.
*   *d8_tx_2x_var_clean     - pointer to D8_TX_2X lanes context.
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
PUBLIC void d8_tx_2x_slice_cleanup(d8_tx_2x_handle_t *d8_tx_2x_handle,
                                   d8_tx_2x_var_t    *d8_tx_2x_var_clean)
{
    PMC_ENTRY();

    PMC_MEMCPY(&d8_tx_2x_handle->var, d8_tx_2x_var_clean, sizeof(d8_tx_2x_var_t) * D8_TX_2X_MAX_NUM_SERDES);

    PMC_RETURN();
} /* d8_tx_2x_slice_cleanup */

/*******************************************************************************
* d8_tx_2x_patt_gen_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns d8_tx_2x instance configured loopback mode.
*
* INPUTS:
*   *handle         - pointer to d8_tx_2x handle.
*   serdes_id       - serdes index.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   TRUE when pattern generator is enabed, FALSE otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 d8_tx_2x_patt_gen_status_get(d8_tx_2x_handle_t *handle, 
                                           UINT8              serdes_id)
{
    BOOL8         enabled = FALSE;

    PMC_ENTRY();
    
    if (0x1 == d8_tx_2x_field_GEN_EN_get(NULL, handle, serdes_id))
    {
        enabled = TRUE;
    }

    PMC_RETURN(enabled);
}/*d8_tx_2x_patt_gen_status_get*/

/*******************************************************************************
* d8_tx_2x_ext_ctxt_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set d8_tx_2x lanes state in reset or start mode.
*
* INPUTS:
*   *d8_tx_2x_handle      - pointer to d8_tx_2x handle instance.
*   lane_msk              - lanes mask
*   out_of_reset          - When TRUE state of d8_tx_2x lane is set to START mode.
*                           when FALSE state is set back to RESET mode.
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS when operation executes succesfully otherwise
*   a descriptive error code will be returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR d8_tx_2x_ext_ctxt_state_set(d8_tx_2x_handle_t      *d8_tx_2x_handle, 
                                             UINT16                  lane_msk,
                                             BOOL8                   out_of_reset) 
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 lane_itr;

    PMC_ENTRY();    

    PMC_ASSERT(d8_tx_2x_handle != NULL, D8_TX_2X_LOG_ERR_INVALID_PARAMETERS,0,0);

    /* for each lane, set update state */
    for (lane_itr = 0; lane_itr < D8_TX_2X_MAX_NUM_SERDES; lane_itr += 1)
    {
        if (0x1 == (0x1 & (lane_msk >> lane_itr)))
        {
            if (TRUE == out_of_reset)
            {
                /* this function should be call only once and before loopback activation*/
                if (TRUE == d8_tx_2x_handle->var[lane_itr].reset)
                {
                    d8_tx_2x_handle->var[lane_itr].reset = FALSE;
                }
                else
                {
                    rc = D8_TX_2X_ERR_STATE_NOT_RESET;
                    break;
                }
            }
            else
            {
                d8_tx_2x_handle->var[lane_itr].reset = TRUE;
                d8_tx_2x_var_default_init(&d8_tx_2x_handle->var[(lane_itr)]);
            }
        }
    }
        
    PMC_RETURN(rc);
}/*d8_tx_2x_ext_ctxt_state_set*/

/*******************************************************************************
* d8_tx_2x_reset_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns d8_tx_2x instance reset status based on 
*    context content.
*
* INPUTS:
*   *handle         - pointer to d8_tx_2x handle.
*   serdes_id       - serdes index.
*
* OUTPUTS:
*   None
*
* RETURNS:
*  TRUE when serdes bus is in reset state otherwise FALSE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 d8_tx_2x_reset_state_get(d8_tx_2x_handle_t *handle,
                                      UINT32             serdes_id) 
{
    BOOL8 rc;

    PMC_ENTRY();

    rc = handle->var[serdes_id].reset;
    
    PMC_RETURN(rc);
}/*d8_tx_2x_reset_state_get*/


/*******************************************************************************
* d8_tx_2x_ctxt_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   cleanup D8_TX_2X block context (all slice context are reinitialized).
*
*
* INPUTS:
*   *d8_tx_2x_handle        - pointer to D8_TX_2X handle instance.
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
PUBLIC void d8_tx_2x_ctxt_cleanup(d8_tx_2x_handle_t *d8_tx_2x_handle)
{
    UINT32 itr;
    PMC_ENTRY();

    for (itr = 0; itr < D8_TX_2X_MAX_NUM_SERDES; itr ++)
    {
        PMC_MEMSET((void *)&d8_tx_2x_handle->var[itr], 0, sizeof(d8_tx_2x_var_t));
    }

    PMC_RETURN();
} /* d8_tx_2x_ctxt_cleanup */
/*
** End of file
*/



