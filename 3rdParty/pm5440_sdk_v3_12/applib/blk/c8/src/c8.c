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
*       Main C8 source file.
* 
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "c8_loc.h"
#include "c8_io.h"
#include "c8_adsp_io.h"
#include "c8_mdsp_io.h" 

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
#define C8_PATT_GEN_SEL_NORMAL_DATA_VAL 0
#define C8_PATT_GEN_SEL_PRBS_DATA_VAL 1

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
const char C8_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    C8_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char C8_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward References
*/
PRIVATE void c8_loopback_cfg(c8_handle_t *handle,
                             UINT8        serdes_id,
                             BOOL8        enable);

PRIVATE BOOL8 c8_serdes_id_validate(UINT8 serdes_id);

PRIVATE void c8_var_default_init(c8_var_t *var_ptr);

PRIVATE PMC_ERROR c8_ctxt_audit(c8_handle_t *c8_handle,
                                c8_var_t    *c8_var_clean,
                                UINT32       c8_reset_word);

PRIVATE PMC_ERROR c8_ctxt_cleanup(c8_handle_t *c8_handle,
                                  c8_var_t    *c8_var_clean);

PRIVATE BOOL8 c8_loopback_cfg_get(c8_handle_t         *handle, 
                                  UINT8                serdes_id);

PRIVATE PMC_ERROR c8_tx_patt_gen_status_get(c8_handle_t   *handle, 
                                            UINT8          serdes_id,
                                            BOOL8         *enabled);

PRIVATE PMC_ERROR c8_validate_usr_patt(util_serdes_patt_cfg_t *patt_cfg);

PRIVATE PMC_ERROR util_patt_mode_to_c8_mode_conv(util_patt_mode_t   util_patt_mode,
                                                 c8_patt_mode_t    *c8_mode_ptr);     
/*
** Public Functions
*/

/*******************************************************************************
* c8_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a C8 block instance.
*
*
* INPUTS:
*  *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address         - base address of the C8 block relative to the
*                           device memory space
*  *sys_handle           - pointer to user system handle
*   *tsb_name            - pointer to TSB name
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   c8_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC c8_handle_t *c8_ctxt_create(pmc_handle_t *parent, 
                                   UINT32 base_address, 
                                   pmc_sys_handle_t *sys_handle, 
                                   const char *tsb_name)
{
    c8_handle_t *c8_handle;

    PMC_ENTRY();    

    c8_handle = (c8_handle_t*)PMC_CTXT_CALLOC(sizeof(c8_handle_t), parent);
    
    pmc_handle_init( parent, c8_handle, sys_handle, PMC_MID_DIGI_C8_SERDES, tsb_name, base_address );

    /* Register log strings. */
    pmc_log_block_strings_register(C8_LOG_ERR_STRINGS[0], C8_LOG_ERR_TABLE_BASE, C8_LOG_ERR_COUNT);

    /* allocate var context */
    c8_handle->var =  (c8_var_t *) PMC_CTXT_CALLOC(sizeof(c8_var_t) * C8_MAX_NUM_SERDES, parent); 
    PMC_CTXT_REGISTER_PTR(&c8_handle->var, parent);

    PMC_RETURN(c8_handle);

} /* c8_ctxt_create */  
 
/*******************************************************************************
* c8_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a C8 block instance.
*
*
* INPUTS:
*   *c8_handle        - pointer to C8 handle instance
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
PUBLIC void c8_ctxt_destroy(c8_handle_t *c8_handle)
{
    PMC_ENTRY();
    PMC_CTXT_FREE(&c8_handle->var, c8_handle);
    PMC_CTXT_FREE(&c8_handle, c8_handle);

    PMC_RETURN();
} /* c8_ctxt_destroy */

/*******************************************************************************
* c8_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a C8 block handle instance.
*
*
* INPUTS:
*   *c8_handle        - pointer to C8 handle instance
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
PUBLIC void c8_handle_init(c8_handle_t *c8_handle)
{
    UINT32 itr;
    PMC_ENTRY();

    for (itr = 0; itr < C8_MAX_NUM_SERDES; itr += 1)
    {
        c8_var_default_init(&c8_handle->var[itr]);
    }

    PMC_RETURN();

} /* c8_handle_init */

/*******************************************************************************
* c8_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes the var context to its default values
*
* INPUTS:
*   *var_ptr          - pointer to c8 variable instance.
*
* OUTPUTS:
*   None
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void c8_var_default_init(c8_var_t *var_ptr)
{

    PMC_ENTRY();

    var_ptr->reset           = TRUE;
    var_ptr->patt_gen_enable = FALSE;
    var_ptr->loopback_enable = FALSE;
    
    PMC_RETURN();
} /*c8_var_default_init  */

/*******************************************************************************
* c8_loopback_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function activate loopback (TX2RX/Diagnostic Loopback).
*
* INPUTS:
*   *handle         - pointer to c8 handle.
*   serdes_id       - serdes identifier.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR c8_loopback_activate(c8_handle_t *handle,
                                      UINT8 serdes_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, C8_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(serdes_id < C8_MAX_NUM_SERDES,C8_LOG_ERR_SERDES_ID_INVALID,0,0);
    PMC_ASSERT(handle->var[serdes_id].loopback_enable == FALSE, C8_LOOPBACK_ALREADY_CONFIGURED, 0, 0);
    
    /* configure the loopback */
    c8_loopback_cfg(handle,serdes_id,TRUE);
    
    handle->var[serdes_id].loopback_enable = TRUE;
    PMC_RETURN(rc);
} /* c8_loopback_activate */

/*******************************************************************************
* c8_loopback_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function deactivate loopback (TX2RX/Disgnostic)
*
* INPUTS:
*   *handle         - pointer to c8 handle.
*   serdes_id       - serdes identifier.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR c8_loopback_deactivate(c8_handle_t *handle,
                                        UINT8 serdes_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, C8_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(serdes_id < C8_MAX_NUM_SERDES,C8_LOG_ERR_SERDES_ID_INVALID,0,0);
    PMC_ASSERT(handle->var[serdes_id].loopback_enable == TRUE, C8_LOOPBACK_ALREADY_NOT_CONFIGURED, 0, 0);
   
    /* configure the loopback */
    c8_loopback_cfg(handle,serdes_id,FALSE);
    /* update status */
    handle->var[serdes_id].loopback_enable = FALSE;

    PMC_RETURN(rc);
} /* c8_loopback_deactivate */

/*******************************************************************************
* c8_loopback_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function returns the status of the C8 SERDES loopback. 
*
* INPUTS:
*   *handle         - pointer to c8 handle.
*   serdes_id       - serdes identifier.
*
* OUTPUTS:
*  *enabled_ptr     - Set to TRUE if the loopback is enabled.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR c8_loopback_status_get(c8_handle_t *handle,
                                        UINT8 serdes_id,
                                        BOOL8 *enabled_ptr)
{
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, C8_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(serdes_id < C8_MAX_NUM_SERDES,C8_LOG_ERR_SERDES_ID_INVALID,0,0);
    
    *enabled_ptr = handle->var[serdes_id].loopback_enable;
    PMC_RETURN(PMC_SUCCESS);
} /* c8_loopback_status_get */

/*******************************************************************************
* c8_patt_mon_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function configures one serdes C8 pattern monitor: 
*    - It sets pattern mode,
*    - It enables/disables pattern inversion check. 
*    - It activates/disactivates pattern monitor.
*
*
* INPUTS:
*   *handle                - c8 handle
*   serdes_id              - serdes identifier
*   *patt_mon_cfg_ptr      - pattern monitor configuration
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS            - when conversion is successful 
*                            otherwise a descriptive error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR c8_patt_mon_cfg(c8_handle_t *handle,
                                 UINT8 serdes_id,
                                 util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr)
{
    c8_buffer_t c8_buf;
    c8_patt_mode_t c8_mode;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    UINT32 usr_patt_size;

    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(handle != NULL, C8_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(patt_mon_cfg_ptr != NULL, C8_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes ID */ 
    if (C8_MAX_NUM_SERDES <= serdes_id)
    {
        PMC_RETURN(C8_LOG_ERR_SERDES_ID_INVALID);
    } 

    if (TRUE == patt_mon_cfg_ptr->patt_cfg.enable)
    {

        /* clear serdes monitor reset */
        c8_field_PATT_MON_RSTB_set(NULL, handle, serdes_id, 0);
        /* reset serdes monitor until configuration is over */
        c8_field_PATT_MON_RSTB_set(NULL, handle, serdes_id, 1);

        c8_field_CHK_EN_set(NULL, handle, serdes_id, 0);

        result = util_patt_mode_to_c8_mode_conv(patt_mon_cfg_ptr->patt_cfg.mode, &c8_mode);
        
        if (PMC_SUCCESS == result)
        {       
            /*  set user-defined pa_monttern when mode matches */
            if (C8_PATT_MODE_CJAT == c8_mode)
            {
                result = c8_validate_usr_patt(&patt_mon_cfg_ptr->patt_cfg);
                if (PMC_SUCCESS != result)
                {
                    PMC_RETURN(result);
                }
                
                usr_patt_size = patt_mon_cfg_ptr->patt_cfg.usr_patt_size / 32;
                if ((patt_mon_cfg_ptr->patt_cfg.usr_patt_size % 32) > 0)
                {
                    usr_patt_size += 1;
                }
                
                for (i = 0; i < usr_patt_size; i ++)
                {
                    switch (i)
                    {
                    case 0:
                        c8_field_CHK_USR_PAT_31_0_set(NULL, handle, serdes_id, patt_mon_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 1:
                        c8_field_CHK_USR_PAT_39_32_set(NULL, handle, serdes_id, patt_mon_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    default: 
                        result = C8_LOG_ERR_INVALID_PARAMETERS;
                        break;
                    }
                }
            }
        }
        if (PMC_SUCCESS == result)
        {
            /* initialize c8 buffer */     
            c8_buffer_init(&c8_buf, handle, (UINT32)serdes_id);    
            
            /* set check inv, pattern mode and enable/disable */
            c8_field_CHK_INV_set(&c8_buf, handle, serdes_id, (UINT32)patt_mon_cfg_ptr->patt_cfg.inv_cfg);  
            c8_field_CHK_MODE_set(&c8_buf, handle, serdes_id, (UINT32) c8_mode);  
            c8_field_CHK_EN_set(&c8_buf, handle, serdes_id, 0);

            
            /* flush c8 buffer */ 
            c8_buffer_flush(&c8_buf);    
        }
        
    }
    else
    {
        c8_field_CHK_EN_set(NULL, handle, serdes_id, (UINT32)patt_mon_cfg_ptr->patt_cfg.enable);
    }
    
    PMC_RETURN(result);
}/*c8_patt_mon_cfg*/

/*******************************************************************************
* c8_patt_mon_resync
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function enables pattern monitor check. c8_patt_mon_cfg function shall 
*    be called prior to this function. This function shall be called after
*    device starts receiving pattern. 
*
*
* INPUTS:
*   *handle                - c8 handle.
*   serdes_id              - serdes identifier.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS            - when conversion is successful 
*                            otherwise a error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR c8_patt_mon_resync(c8_handle_t *handle,
                                    UINT8 serdes_id)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, C8_LOG_ERR_NULL_HANDLE, 0, 0);
    /* check serdes ID */
    if (C8_MAX_NUM_SERDES <= serdes_id)
    {
        PMC_RETURN(C8_LOG_ERR_SERDES_ID_INVALID);
    }

    /* toggle enable  */
    c8_field_CHK_EN_set(NULL, handle, serdes_id, 0);
    c8_field_CHK_EN_set(NULL, handle, serdes_id, 1);

    /* activate pattern monitor error counter read and update in mon_err_cnt register  */
    /* emulate faling-edge to allow future read */
    c8_field_CHK_READ_set (NULL, handle, serdes_id, 1);
    c8_field_CHK_READ_set (NULL, handle, serdes_id, 0);

    PMC_RETURN(result);
}

/*******************************************************************************
* c8_patt_gen_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function configures one serdes C8 pattern generator: 
*    - It sets pattern mode.
*    - It enables/disables pattern inversion.
*    - It enables/disables error injection. 
*    - It activates/disactivates pattern generator.
*    - It sets pattern mode.
*
*
* INPUTS:
*   *handle                - c8 handle
*   serdes_id              - serdes identifier
*   *patt_gen_cfg_ptr      - pattern generator configuration
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS            - when configuration is successful 
*                            otherwise a descriptive error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR c8_patt_gen_cfg(c8_handle_t *handle,
                                 UINT8 serdes_id,
                                 util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr)
{
    c8_buffer_t c8_buf;
    c8_patt_mode_t c8_mode;
    UINT32 i;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 usr_patt_size;

    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(handle != NULL, C8_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(patt_gen_cfg_ptr != NULL, C8_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes ID */ 
    if (C8_MAX_NUM_SERDES <= serdes_id)
    {
        PMC_RETURN(C8_LOG_ERR_SERDES_ID_INVALID);
    } 
       

    /* initialize c8 buffer */     
    c8_buffer_init(&c8_buf, handle, (UINT32)serdes_id); 

    if (TRUE == patt_gen_cfg_ptr->patt_cfg.enable)
    {

        /* reset serdes generator until configuration is over */
        c8_field_GEN_EN_set(NULL, handle, serdes_id, 0);

        result = util_patt_mode_to_c8_mode_conv(patt_gen_cfg_ptr->patt_cfg.mode, &c8_mode);
        
        /*  set user-defined pa_monttern when mode matches */
        if (PMC_SUCCESS == result)
        {       
            /*  set user-defined pa_monttern when mode matches */
            if (C8_PATT_MODE_CJAT == c8_mode)
            {
                result = c8_validate_usr_patt(&patt_gen_cfg_ptr->patt_cfg);
                if (PMC_SUCCESS != result)
                {
                    PMC_RETURN(result);
                }
                
                usr_patt_size = patt_gen_cfg_ptr->patt_cfg.usr_patt_size / 32;
                if ((patt_gen_cfg_ptr->patt_cfg.usr_patt_size % 32) > 0)
                {
                    usr_patt_size += 1;
                }
                
                for (i = 0; i < usr_patt_size; i ++)
                {
                    switch (i)
                    {
                    case 0:
                        c8_field_GEN_USR_PAT_31_0_set(NULL, handle, serdes_id, patt_gen_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 1:
                        c8_field_GEN_USR_PAT_39_32_set(NULL, handle, serdes_id, patt_gen_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    default: 
                        result = C8_LOG_ERR_INVALID_PARAMETERS;
                        break;
                    }
                }
            }
        }

        if (PMC_SUCCESS == result)
        {       
            
            /* set check inv, pattern mode and select */
            c8_field_GEN_INV_set(&c8_buf, handle, serdes_id, (UINT32)patt_gen_cfg_ptr->patt_cfg.inv_cfg);
            c8_field_GEN_MODE_set(&c8_buf, handle, serdes_id, (UINT32)c8_mode);
            c8_field_PATT_GEN_SEL_set(&c8_buf, handle, serdes_id,  C8_PATT_GEN_SEL_PRBS_DATA_VAL); 
            c8_field_GEN_EN_set(&c8_buf, handle, serdes_id, 1);
        }
        
        /* flush c8 buffer */ 
        c8_buffer_flush(&c8_buf);

        /* clear serdes generator reset */
        c8_field_PATT_GEN_RSTB_set(NULL, handle, serdes_id, 0);
        c8_field_PATT_GEN_RSTB_set(NULL, handle, serdes_id, 1);
    }
    else
    {
        c8_field_PATT_GEN_SEL_set(NULL, handle, serdes_id,  C8_PATT_GEN_SEL_NORMAL_DATA_VAL);
    }


    handle->var[serdes_id].patt_gen_enable = patt_gen_cfg_ptr->patt_cfg.enable;

    PMC_RETURN(result);
}/*c8_patt_gen_cfg*/

/*******************************************************************************
* c8_patt_gen_err_insert_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables one error bit insertion in generated pattern.
*
* INPUTS:
*   *handle         - pointer to c8 serdes handle.
*   serdes_id       - serdes identifier.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS     - when API execution is successful otherwise a 
*                     descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR c8_patt_gen_err_insert_cfg(c8_handle_t *handle,
                                            UINT8 serdes_id)
 
{
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, C8_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes ID */
    if (C8_MAX_NUM_SERDES <= serdes_id)
    {
        PMC_RETURN(C8_LOG_ERR_SERDES_ID_INVALID);
    }

    /* emulate rising edge on GEN_ERR_INK register bit */
    /* in case bit is already high */
    c8_field_GEN_ERR_INJ_set(NULL, handle, serdes_id, 0);
    c8_field_GEN_ERR_INJ_set(NULL, handle, serdes_id, 1);

    /* lower register bit  */
    c8_field_GEN_ERR_INJ_set(NULL, handle, serdes_id, 0);
    
    PMC_RETURN(PMC_SUCCESS);
}/*c8_patt_gen_err_insert_cfg*/


/*******************************************************************************
* c8_patt_gen_bus_width_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function sets pattern bus width associated to serdes rate.
*
* INPUTS:
*   *handle         - pointer to c8 handle.
*   serdes_id       - serdes identifier.
*   bus_width       - pattern bus width associated to serdes
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC void c8_patt_gen_bus_width_set(c8_handle_t           *handle,
                                      UINT8                  serdes_id,
                                      UINT8                  bus_width)
{

    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, C8_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(TRUE == c8_serdes_id_validate(serdes_id), C8_LOG_ERR_SERDES_ID_INVALID,0,0);

    c8_field_PATT_BUS_WIDTH_set(NULL, handle, serdes_id, bus_width);
    
    PMC_RETURN();
}/*c8_patt_gen_bus_width_set*/


/*******************************************************************************
* c8_patt_mon_err_count_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function read and returns RX pattern monitor status and errors counter value.
*
* INPUTS:
*   *handle                 - c8 handle
*   serdes_id               - serdes identifier
* OUTPUTS:
*   *status                 - Pattern monitor status: Locked, trying to lock, etc.                  
*   *err_count              - Number of errors detected by pattern monitor. 
*                             The value shall be ignored when pattern monitor
*                             is not in locked state.
*
* RETURNS:
*   PMC_SUCCESS             - when counter value is successfully read 
*                             otherwise a error code is returned
*
* NOTES:
*
*******************************************************************************/   
PUBLIC UINT32 c8_patt_mon_err_count_get(c8_handle_t           *handle,
                                        UINT8                  serdes_id,
                                        util_patt_status_t    *status,
                                        UINT32                *err_count)
{

    /* check parameters */
    PMC_ASSERT(handle != NULL, C8_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(status != NULL, C8_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(err_count != NULL, C8_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    PMC_ENTRY();

    /* activate pattern monitor error counter read and update in mon_err_cnt register  */
    c8_field_CHK_READ_set (NULL, handle, serdes_id, 1);

    /* emulate faling-edge to allow future read */
    c8_field_CHK_READ_set (NULL, handle, serdes_id, 0);

    *err_count = c8_field_CHK_ERR_CNT_get (NULL, handle, serdes_id);
    

    switch(*err_count)
    {
    case 0xFFFFFFFF: 
        *status = UTIL_PATT_STATUS_DISABLED;
        break;
    case 0xFFFFFFFE: 
        *status = UTIL_PATT_STATUS_TRYING_TO_LOCK;
        break;
    case 0xFFFFFFFD:
        *status = UTIL_PATT_STATUS_ZERO_DET;
        break;
    default:
        *status = UTIL_PATT_STATUS_LOCKED;
        break;
    }

    PMC_RETURN(PMC_SUCCESS);
}  

/*******************************************************************************
* util_patt_mode_to_c8_mode_conv 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert pattern mode defined defined in util global to c8 pattern mode enum.
*
*
* INPUTS:
*  util_patt_mode        - util global pattern mode enum
*
* OUTPUTS:
*   *c8_mode_ptr         - c8 pattern mode enum
*
* RETURNS:
*   PMC_SUCCESS          - when conversion is done 
*                          otherwise a descriptive error is returned
*
* NOTES:
*
******************************************************************************/
PRIVATE PMC_ERROR util_patt_mode_to_c8_mode_conv(util_patt_mode_t util_patt_mode,
                                                 c8_patt_mode_t *c8_mode_ptr)
{
    PMC_ENTRY();

    switch (util_patt_mode)
    {
    case UTIL_PATT_MODE_PRBS_5:
        *c8_mode_ptr= C8_PATT_MODE_PRBS_5;
        break;  
    case UTIL_PATT_MODE_PRBS_7:
        *c8_mode_ptr= C8_PATT_MODE_PRBS_7;
        break;  
    case UTIL_PATT_MODE_PRBS_11:
        *c8_mode_ptr= C8_PATT_MODE_PRBS_11;
        break;
    case UTIL_PATT_MODE_PRBS_15:
        *c8_mode_ptr= C8_PATT_MODE_PRBS_15;
        break;
    case UTIL_PATT_MODE_PRBS_23:
        *c8_mode_ptr= C8_PATT_MODE_PRBS_23;
        break;
    case UTIL_PATT_MODE_PRBS_31:
        *c8_mode_ptr= C8_PATT_MODE_PRBS_31;
        break;
    case UTIL_PATT_MODE_CJAT:
        *c8_mode_ptr= C8_PATT_MODE_CJAT;
        break;  
    default:
        PMC_RETURN(C8_LOG_ERR_MODE_NOT_FOUND);
    }

    PMC_RETURN(PMC_SUCCESS);
        
}/*util_patt_mode_to_c8_mode_conv*/

/*******************************************************************************
* c8_validate_usr_patt
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function verifies that the length of user-defined pattern is compatible 
*   to C8 serdes.
*
* INPUTS:
*  patt_cfg         - pattern configuration
*
* OUTPUTS:
*  NONE.
*
* RETURNS:
*   PMC_SUCCESS                 - when user-defined pattern is correct is successful 
*                                 otherwise a descriptive error is returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR c8_validate_usr_patt(util_serdes_patt_cfg_t *patt_cfg)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    PMC_ASSERT(NULL != patt_cfg,C8_LOG_ERR_NULL_HANDLE,0,0);

    if (patt_cfg->usr_patt_size != C8_USR_PATT_BIT_SIZE)
    {
        result = C8_LOG_ERR_USR_PATT_SIZE_INVALID;
    }

    PMC_RETURN(result);
} /* c8_validate_usr_patt */

/*******************************************************************************
* c8_loopback_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function configure a C8 instance to enable/disable loopback
*
* INPUTS:
*   *handle         - pointer to C8 handle.
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
PRIVATE void c8_loopback_cfg(c8_handle_t *handle,
                             UINT8 serdes_id,
                             BOOL8 enable)
{
    PMC_ENTRY();

    PMC_ASSERT(NULL != handle,C8_LOG_ERR_NULL_HANDLE,0,0);
    
    /* stop clocks */
    c8_field_DGLT_TPCOUT_EN_set(NULL,handle,serdes_id,0);
    c8_field_DGLT_BADJ_EN_set(NULL,handle,serdes_id,0);
    /* enable/disable loopback */
    c8_field_DLPBK_EN_set(NULL,handle,serdes_id,(enable ? 1 : 0));
    
    /* start clocks */
    c8_field_DGLT_TPCOUT_EN_set(NULL,handle,serdes_id, 1);
    c8_field_DGLT_BADJ_EN_set(NULL,handle,serdes_id, 1); 
    /*Resync the deskew FIFO*/
    c8_field_SYNC_set(NULL,handle,serdes_id,0);

    c8_field_DLOS_OVR_EN_set(NULL,handle,serdes_id,enable ? 1 : 0);
    c8_field_DLOS_OVR_set(NULL,handle,serdes_id,0);
    
    PMC_RETURN();
 
} /* c8_loopback_cfg */

/*******************************************************************************
* c8_polarity_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function configure a C8 instance to enable/disable lane polarity 
*   inversion
*
* INPUTS:
*   *handle         - pointer to C8 handle.
*   serdes_id       - serdes identifier.
*   dir             - serdes logical lane direction.
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
PUBLIC void c8_polarity_cfg(c8_handle_t              *handle,
                            UINT8                     serdes_id,
                            util_global_direction_t   dir,
                            BOOL8                     enable)
{
    BOOL cfg_rx = FALSE;
    BOOL cfg_tx = FALSE;


    PMC_ENTRY();
    PMC_ASSERT(NULL != handle,C8_LOG_ERR_NULL_HANDLE,0,0);
    
    switch (dir)
    {
    case UTIL_GLOBAL_DIR_TX:
        cfg_tx = TRUE;
        break;
    case UTIL_GLOBAL_DIR_RX:
        cfg_rx = TRUE;
        break;
    case UTIL_GLOBAL_DIR_BOTH:
        cfg_rx = TRUE;
        cfg_tx = TRUE;
        break;
    default:
        break;
    }
               
    if (TRUE == cfg_tx)
    {
        c8_field_INV_TX_DATA_set(NULL, handle, serdes_id, (UINT32) enable);
    }

    if (TRUE == cfg_rx)
    {
        c8_mdsp_field_INV_REC_DATA_set(NULL, handle, serdes_id, (UINT32) enable);
    }
    
    PMC_RETURN();
 
} /* c8_polarity_cfg */

/*******************************************************************************
* c8_tx_swing_emphasis_levels_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function configure a C8 instance tx_swing and de-emphasis levels.
*
* INPUTS:
*   *handle          - pointer to C8 handle.
*   serdes_id        - serdes identifier.
*   levels_cfg_ptr   - pointer to TX swing and de-emphasis levels to
*                      set in device
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successful otherwise a descriptive error 
*   code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR c8_tx_swing_emphasis_levels_set(c8_handle_t           *handle,
                                                 UINT8                  serdes_id,
                                                 c8_tx_swing_levels_t  *levels_cfg_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    c8_buffer_t b[1];
    
    PMC_ENTRY();

    PMC_ASSERT(NULL != handle, C8_LOG_ERR_NULL_HANDLE,0,0);
    PMC_ASSERT(NULL != levels_cfg_ptr, C8_LOG_ERR_SERDES_ID_INVALID, 0, 0);
       
    if (FALSE == c8_serdes_id_validate(serdes_id))
    {
        rc = C8_LOG_ERR_SERDES_ID_INVALID;
    }
    else 
    {
        c8_buffer_init(b,handle,serdes_id);
        c8_field_TX_DPENI1_set(b, handle, serdes_id, levels_cfg_ptr->tx_dpeni1);
        c8_field_TX_DPENI2_set(b, handle, serdes_id, levels_cfg_ptr->tx_dpeni2);
        c8_buffer_flush(b);    
        c8_field_TX_DENI_set(NULL, handle, serdes_id, levels_cfg_ptr->deni);
        c8_field_TX_PSELI_set(NULL, handle, serdes_id, levels_cfg_ptr->tx_psel);
        c8_field_TX_WIZARD_EN_set(NULL, handle, serdes_id, 0x0);
    }

    PMC_RETURN(rc);    
} /* c8_tx_swing_emphasis_levels_set */




/*******************************************************************************
* c8_serdes_id_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Validates that given serdes id is in range of values.
*
* INPUTS:
*   serdes_id        - serdes identifier.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   TRUE when serdes id is in range of values otherwise FALSE.
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 c8_serdes_id_validate(UINT8 serdes_id)
{
    BOOL8 rc;
    PMC_ENTRY();

    rc = C8_MAX_NUM_SERDES <= serdes_id ? FALSE : TRUE;

    PMC_RETURN(rc);
}/*serdes_id_validate*/

/*******************************************************************************
* c8_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on c8 handle instance.
*
* INPUTS:
*   *c8_handle             - pointer to c8 handle instance
*   *restart_init_cfg_ptr  - pointer to restart init config
*    c8_reset_msk          - c8 serdes reset bits mask 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS : 
*     WARM : register/context are coherent
*     CRASH: everything is coherent (a cleanup may has been executed)
*   C8_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT:
*     incoherences have been detected on communal energy configuration
*     that is incoherent with top energy state
*      WARM : ERROR
*      CRASH: TOP shall put C8 in RESET mode
*   
subsystem shall be put in reset mode 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR c8_handle_restart_init(c8_handle_t                      *c8_handle,
                                        util_global_restart_init_cfg_t  *restart_init_cfg_ptr,
                                        UINT32                           c8_reset_msk)
{
    PMC_ERROR           rc = PMC_SUCCESS;
    c8_var_t           *c8_var_clean;
    
    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(C8_LOG_ERR_STRINGS[0], C8_LOG_ERR_TABLE_BASE, C8_LOG_ERR_COUNT);
    }

    c8_var_clean =  (c8_var_t *) PMC_CALLOC(sizeof(c8_var_t) * C8_MAX_NUM_SERDES); 

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
     *   - Create a new context that represents the intersection between memory and register space (c8_var_clean)
     */
    rc = c8_ctxt_audit(c8_handle,
                       c8_var_clean,
                       c8_reset_msk); 
    
    PMC_LOG_TRACE("c8_ctxt_audit rc = %s\n", pmc_log_string_get(rc));
    if (C8_ERR_CONTEXT_CLEANUP == rc && restart_init_cfg_ptr->is_warm_restart == FALSE)
    {
        /* 
         * Cleanup 
         *   - At the end, c8_var_clean is the new context
         *    - We keep OPERATIONAL channel defined in c8_var_clean
         *    - We clean (deactivate/deprov) all channels that are not OPERATIONAL
         *       - they may have be partially provisionned
         *    - We ajust power setting based on c8_var_clean
         */                                    
        rc = c8_ctxt_cleanup(c8_handle,
                             c8_var_clean); 
    }

    PMC_FREE(&c8_var_clean);

    PMC_RETURN(rc);
} /* c8_handle_restart_init */

/*******************************************************************************
* c8_ctxt_audit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function audits c8 handle context variable against register space content.
*
* INPUTS:
*   *c8_handle           - pointer to c8 handle instance.
*   *c8_var_clean        - pointer to c8 lanes context.
*    c8_reset_msk        - c8 serdes reset bits mask.
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
PRIVATE PMC_ERROR c8_ctxt_audit(c8_handle_t *c8_handle,
                                c8_var_t    *c8_var_clean,
                                UINT32       c8_reset_msk)
{
    UINT32 itr;
    UINT32 jtr;
    BOOL8 serdes_ctxt_match;
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL8 serdes_reset;
    BOOL8 lpbk_enabled;
    BOOL8 patt_gen_enabled;

    PMC_ENTRY();

    PMC_ASSERT(c8_handle != NULL, C8_LOG_ERR_INVALID_PARAMETERS,0,0);
    
    for (itr = 0; itr < C8_MAX_NUM_SERDES; itr += 1)
    {
        serdes_ctxt_match = TRUE;
        serdes_reset = (0x1 == (0x1 & (c8_reset_msk >> (itr / 2)))) ? TRUE : FALSE;

        if (FALSE == serdes_reset)
        {
            /* check c8 provisioning & activation status */
            if (TRUE == c8_handle->var[itr].reset)
            {
                for (jtr = 0; jtr < C8_MAX_NUM_SERDES; jtr += 1)
                {
                    if (itr == jtr)
                    {
                        continue;
                    }
                    if (FALSE == c8_handle->var[jtr].reset)
                    {
                        break;
                    }
                }
                if (C8_MAX_NUM_SERDES == jtr)
                {
                    serdes_ctxt_match = FALSE;
                }
            } 
            
            /* check pattern generator status */
            if (TRUE == serdes_ctxt_match)
            {
                rc = c8_tx_patt_gen_status_get(c8_handle,itr, &patt_gen_enabled);
                if (PMC_SUCCESS == rc)
                {
                    if (patt_gen_enabled != c8_handle->var[itr].patt_gen_enable)
                    {
                        serdes_ctxt_match = FALSE; 
                    }                                    
                }
                else
                {
                    serdes_ctxt_match = FALSE; 
                }                
            }
            
            /* check loopback status */
            if (TRUE == serdes_ctxt_match)
            {
                lpbk_enabled = c8_loopback_cfg_get(c8_handle,itr);
               
                if (c8_handle->var[itr].loopback_enable !=
                    lpbk_enabled)
                {
                    serdes_ctxt_match = FALSE;                        
                }
            }            
        }
        else
        {
            /* check c8 provisioning & activation status */
            if (TRUE != c8_handle->var[itr].reset)
            {
                serdes_ctxt_match = FALSE;               
            }
        }

        /* update returned error code anf var clean (used to cleanup the context) */
        if (FALSE == serdes_ctxt_match)
        {
            if (PMC_SUCCESS == rc)
            {
                rc = C8_ERR_CONTEXT_CLEANUP;
            }
            c8_var_default_init(&c8_var_clean[itr]);
        }   
        else
        {
            if (TRUE != serdes_reset ||
                TRUE == c8_handle->var[itr].reset)
            {
                PMC_MEMCPY((void*)&c8_var_clean[itr], (void*)&c8_handle->var[itr], sizeof(c8_var_t));
            }
        } 
    }

    PMC_RETURN(rc);
} /* c8_ctxt_audit */

/*******************************************************************************
* c8_ctxt_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function cleans up c8 handle context. 
*
* INPUTS:
*   *c8_handle           - pointer to c8 handle instance.
*   *c8_var_clean        - pointer to c8 lanes clean context.
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
PRIVATE PMC_ERROR c8_ctxt_cleanup(c8_handle_t  *c8_handle,
                                  c8_var_t     *c8_var_clean)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(c8_handle != NULL, C8_LOG_ERR_INVALID_PARAMETERS,0,0);

    PMC_MEMCPY(c8_handle->var, c8_var_clean, sizeof(c8_var_t) * C8_MAX_NUM_SERDES);

    PMC_RETURN(rc);
} /* c8_ctxt_cleanup */

/*******************************************************************************
* c8_loopback_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns c8 instance configured loopback mode.
*
* INPUTS:
*   *handle         - pointer to c8 handle.
*   serdes_id       - serdes index.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   BOOL8 - TRUE - loopback enabled, FALSE - disabled.
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 c8_loopback_cfg_get(c8_handle_t *handle, 
                                  UINT8         serdes_id) 
{
    BOOL8 lpbk_enabled = FALSE;

    PMC_ENTRY();

    if (0x0 == c8_field_DLPBK_EN_get(NULL,handle,serdes_id))
    {
        lpbk_enabled = TRUE;
    }

    PMC_RETURN(lpbk_enabled);
}/*c8_loopback_cfg_get*/


/*******************************************************************************
* c8_tx_patt_gen_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns c8 instance configured loopback mode.
*
* INPUTS:
*   *handle         - pointer to c8 handle.
*   serdes_id       - serdes index.
*
* OUTPUTS:
*   *enabled        - TRUE when pattern generator is enabled, FALSE otherwise.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR c8_tx_patt_gen_status_get(c8_handle_t *handle, 
                                            UINT8         serdes_id,
                                            BOOL8         *enabled) 
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    *enabled = FALSE;
    
    if (0x1 == c8_field_GEN_EN_get(NULL, handle, serdes_id))
    {
        *enabled = TRUE;
        if (C8_PATT_GEN_SEL_PRBS_DATA_VAL != c8_field_PATT_GEN_SEL_get(NULL, handle, serdes_id))
        {            
            rc = C8_ERR_CONTEXT_REG_INCOHERENT;
        }
    }

    PMC_RETURN(rc);
}/*c8_tx_patt_gen_status_get*/

/*******************************************************************************
* c8_ext_ctxt_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set c8 lanes state in reset or start mode.
*
* INPUTS:
*   *c8_handle            - pointer to c8 handle instance.
*   lane_msk              - lanes mask
*   out_of_reset          - When TRUE state of c8 lane is set to START mode.
*                           when FALSE1
 state is set back to RESET mode.
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
PUBLIC PMC_ERROR c8_ext_ctxt_state_set(c8_handle_t      *c8_handle, 
                                       UINT16            lane_msk,
                                       BOOL8             out_of_reset) 
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 lane_itr;

    PMC_ENTRY();    

    PMC_ASSERT(c8_handle != NULL, C8_LOG_ERR_INVALID_PARAMETERS,0,0);

    /* for each lane, set update state */
    for (lane_itr = 0; lane_itr < C8_MAX_NUM_SERDES; lane_itr += 1)
    {
        if (0x1 == (0x1 & (lane_msk >> lane_itr)))
        {
            if (TRUE == out_of_reset)
            {
                /* this function should be call only once and before loopback activation*/
                if (TRUE == c8_handle->var[lane_itr].reset)
                {
                    c8_handle->var[lane_itr].reset = FALSE;
                }
                else
                {
                    rc = C8_STATE_NOT_RESET;
                    break;
                }
            }
            else
            {
                c8_handle->var[lane_itr].reset = TRUE;
                c8_var_default_init(&c8_handle->var[(lane_itr)]);
            }
        }
    }
        
    PMC_RETURN(rc);
}/*c8_ext_ctxt_state_set*/

/*******************************************************************************
* c8_reset_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns c8 instance reset status based on 
*    context content.
*
* INPUTS:
*   *handle         - pointer to c8 handle.
*    serdes_id      - serdes index
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
PUBLIC BOOL8 c8_reset_state_get(c8_handle_t *handle,
                                UINT32       serdes_id) 
{
    BOOL8 rc;

    PMC_ENTRY();

    PMC_ASSERT(handle != NULL, C8_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(serdes_id < C8_MAX_NUM_SERDES,C8_LOG_ERR_SERDES_ID_INVALID,0,0);

    rc = handle->var[serdes_id].reset;
    
    PMC_RETURN(rc);
}/*c8_reset_state_get*/

/*
** End of file
*/
