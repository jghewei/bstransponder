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
*       Main T8 source file.
* 
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "t8_loc.h"
#include "t8_io.h"
#include "t8_40n_io.h"
#include "t8_adsp_io.h"
/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
#define T8_PATT_GEN_SEL_NORMAL_DATA_VAL 0
#define T8_PATT_GEN_SEL_PRBS_DATA_VAL 1
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
const char T8_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    T8_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char T8_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward References
*/
PRIVATE void t8_loopback_cfg(t8_handle_t *handle,
                             UINT8 serdes_id,
                             BOOL8 enable);

PRIVATE BOOL8 t8_serdes_id_validate(UINT8 serdes_id);

PRIVATE void t8_var_default_init(t8_var_t *var_ptr);

PRIVATE PMC_ERROR t8_ctxt_audit(t8_handle_t *t8_handle,
                                t8_var_t    *t8_var_clean,
                                UINT32       t8_reset_word);

PRIVATE PMC_ERROR t8_ctxt_cleanup(t8_handle_t *t8_handle,
                                  t8_var_t    *t8_var_clean);

PRIVATE PMC_ERROR t8_loopback_cfg_get(t8_handle_t         *handle, 
                                      UINT8                serdes_id,
                                      t8_loopback_mode_t  *lpbk_mode_ptr);

PRIVATE PMC_ERROR t8_tx_patt_gen_status_get(t8_handle_t   *handle, 
                                            UINT8          serdes_id,
                                            BOOL8         *enabled);

PRIVATE PMC_ERROR t8_validate_usr_patt(util_serdes_patt_cfg_t *patt_cfg);

PRIVATE PMC_ERROR util_patt_mode_to_t8_mode_conv(util_patt_mode_t util_patt_mode,
                                                 t8_patt_mode_t *t8_mode_ptr);

/*
** Public Functions
*/
/*******************************************************************************
* t8_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a T8 block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the T8 block relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - pointer to TSB name
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   t8_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC t8_handle_t *t8_ctxt_create(pmc_handle_t *parent, 
                                   UINT32 base_address, 
                                   pmc_sys_handle_t *sys_handle, 
                                   const char *tsb_name )
{
    t8_handle_t *t8_handle;

    PMC_ENTRY();    

    t8_handle = (t8_handle_t*)PMC_CTXT_CALLOC(sizeof(t8_handle_t), parent);
    
    pmc_handle_init( parent, t8_handle, sys_handle, PMC_MID_DIGI_T8_SERDES, tsb_name, base_address );

    /* Register log strings. */
    pmc_log_block_strings_register(T8_LOG_ERR_STRINGS[0], T8_LOG_ERR_TABLE_BASE, T8_LOG_ERR_COUNT);

    /* allocate var context */
    t8_handle->var =  (t8_var_t *) PMC_CTXT_CALLOC(sizeof(t8_var_t) * 2, parent); 
    PMC_CTXT_REGISTER_PTR(&t8_handle->var, parent);
    PMC_RETURN(t8_handle);
} /* t8_ctxt_create */  
 
/*******************************************************************************
* t8_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a T8 block instance.
*
*
* INPUTS:
*   *t8_handle        - pointer to T8 handle instance
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
PUBLIC void t8_ctxt_destroy(t8_handle_t *t8_handle)
{
    PMC_ENTRY();

    PMC_CTXT_FREE(&t8_handle->var, t8_handle);
    PMC_CTXT_FREE(&t8_handle, t8_handle);

    PMC_RETURN();
} /* t8_ctxt_destroy */

/*******************************************************************************
* t8_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a T8 block handle instance.
*
*
* INPUTS:
*   *t8_handle        - pointer to T8 handle instance
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
PUBLIC void t8_handle_init(t8_handle_t *t8_handle)
{
    UINT32 itr;
    PMC_ENTRY();

    for (itr = 0; itr < T8_MAX_NUM_SERDES; itr += 1)
    {
        t8_var_default_init(&t8_handle->var[itr]);
    }
    PMC_RETURN();

} /* t8_handle_init */

/*******************************************************************************
* t8_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes the var context to its default values
*
* INPUTS:
*   *var_ptr          - pointer to t8 variable instance.
*
* OUTPUTS:
*   None
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void t8_var_default_init(t8_var_t *var_ptr)
{

    PMC_ENTRY();

    var_ptr->reset =TRUE;
    var_ptr->patt_gen_enable = FALSE;
    var_ptr->loopback_mode   = LAST_T8_LOOPBACK_MODE;
    var_ptr->inv_dfe_error   = FALSE;
    
    PMC_RETURN();
} /*t8_var_default_init  */

/*******************************************************************************
* t8_patt_mon_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function configures one serdes T8 pattern monitor: 
*    - It sets pattern mode,
*    - It enables/disables pattern inversion check. 
*    - It activates/disactivates pattern monitor.
*
*
* INPUTS:
*   *handle                - t8 handle
*   serdes_id              - serdes identifier
*   *patt_mon_cfg_ptr      - pattern monitor configuration
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS            - when conversion is successful 
*                            otherwise a error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR t8_patt_mon_cfg(t8_handle_t *handle,
                                 UINT8 serdes_id,
                                 util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr)
{
    t8_buffer_t t8_buf;
    t8_patt_mode_t t8_mode;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    UINT32 usr_patt_size;

    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(handle != NULL, T8_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(patt_mon_cfg_ptr != NULL, T8_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes ID */ 
    if (T8_MAX_NUM_SERDES <= serdes_id)
    {
        PMC_RETURN(T8_LOG_ERR_SERDES_ID_INVALID);
    } 

    if (TRUE == patt_mon_cfg_ptr->patt_cfg.enable)
    {        
        /* reset serdes monitor until configuration is over */
        t8_field_MON_RSTB_set(NULL, handle, serdes_id, 0);        /* clear serdes monitor reset */
        t8_field_MON_RSTB_set(NULL, handle, serdes_id, 1);

        result = util_patt_mode_to_t8_mode_conv(patt_mon_cfg_ptr->patt_cfg.mode, &t8_mode);
        if (PMC_SUCCESS == result)
        {       
            /* initialize t8 buffer */     
            t8_buffer_init(&t8_buf, handle, (UINT32)serdes_id);   

            /*  set user-defined pa_monttern when mode matches */
            if (T8_PATT_MODE_USER_DEF == t8_mode ||
                T8_PATT_MODE_CJAT == t8_mode)
            {
                result = t8_validate_usr_patt(&patt_mon_cfg_ptr->patt_cfg);
                if (PMC_SUCCESS != result)
                {
                    PMC_RETURN(result);
                }
                
                usr_patt_size = patt_mon_cfg_ptr->patt_cfg.usr_patt_size / 32;
                if ((patt_mon_cfg_ptr->patt_cfg.usr_patt_size % 32) > 0)
                {
                    usr_patt_size += 1;
                }
                
                for (i = 0; i < usr_patt_size; i += 1)
                {
                    switch (i)
                    {
                    case 0:
                        t8_field_CHK_USR_PAT_31_0_set(NULL, handle, serdes_id, patt_mon_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 1:
                        t8_field_CHK_USR_PAT_39_32_set(NULL, handle, serdes_id, patt_mon_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    default: 
                        result = T8_LOG_ERR_INVALID_PARAMETERS;
                        break;
                    }
                }
            }   
        }

        
        if (PMC_SUCCESS == result)
        {
            /* set check inv, pattern mode and enable/disable */
            t8_field_CHK_INV_set(&t8_buf, handle, serdes_id, (UINT32)patt_mon_cfg_ptr->patt_cfg.inv_cfg);  
            t8_field_CHK_MODE_set(&t8_buf, handle, serdes_id, (UINT32) t8_mode);
            t8_field_CHK_READ_set(&t8_buf, handle, serdes_id, 0);
            
            /* flush t8 buffer */ 
            t8_buffer_flush(&t8_buf);
        }
    }
    else
    {
        t8_field_CHK_EN_set(NULL, handle, serdes_id, (UINT32)patt_mon_cfg_ptr->patt_cfg.enable); 
    }
    
    PMC_RETURN(result);
}/*t8_patt_mon_cfg*/

/*******************************************************************************
* t8_patt_mon_resync
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function enables pattern monitor check. t8_patt_mon_cfg function shall 
*    be called prior to this function. This function shall be called after
*    device starts receiving pattern. 
*
*
* INPUTS:
*   *handle                - t8 handle.
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
PUBLIC PMC_ERROR t8_patt_mon_resync(t8_handle_t *handle,
                                    UINT8 serdes_id)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, T8_LOG_ERR_NULL_HANDLE, 0, 0);
    /* check serdes ID */
    if (T8_MAX_NUM_SERDES <= serdes_id)
    {
        PMC_RETURN(T8_LOG_ERR_SERDES_ID_INVALID);
    }

    /* toggle enable  */
    t8_field_CHK_EN_set(NULL, handle, serdes_id, 0);
    t8_field_CHK_EN_set(NULL, handle, serdes_id, 1);    

    /* activate pattern monitor error counter read and update in mon_err_cnt register  */
    /* emulate faling-edge to allow future read */
    t8_field_CHK_READ_set (NULL, handle, serdes_id, 1);
    t8_field_CHK_READ_set (NULL, handle, serdes_id, 0);

    PMC_RETURN(result);
}/* t8_patt_mon_resync */

/*******************************************************************************
* t8_patt_gen_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function configures one serdes T8 pattern generator: 
*    - It sets pattern mode.
*    - It enables/disables pattern inversion.
*    - It enables/disables error injection. 
*    - It activates/disactivates pattern generator.
*    - It sets pattern mode.
*
*
* INPUTS:
*   *handle                - t8 handle
*   serdes_id              - serdes identifier
*   *patt_gen_cfg_ptr      - pattern generator configuration

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
PUBLIC PMC_ERROR t8_patt_gen_cfg(t8_handle_t *handle,
                                 UINT8 serdes_id,
                                 util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr)
{    t8_buffer_t t8_buf;
    t8_patt_mode_t t8_mode;
    UINT32 i;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 usr_patt_size;

    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(handle != NULL, T8_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(patt_gen_cfg_ptr != NULL, T8_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes ID */ 
    if (T8_MAX_NUM_SERDES <= serdes_id)
    {
        PMC_RETURN(T8_LOG_ERR_SERDES_ID_INVALID);
    } 

    /* check if we have a conflict between pattern generator and FACILITY loopback */
    if (T8_FACILITY_LOOPBACK == handle->var[serdes_id].loopback_mode )
    {
        PMC_RETURN(T8_PATT_GEN_FACILITY_LOOPBACK_CONFLICT);
    }

    /* initialize t8 buffer */     
    t8_buffer_init(&t8_buf, handle, (UINT32)serdes_id);  

    if (TRUE == patt_gen_cfg_ptr->patt_cfg.enable)
    {
        result = util_patt_mode_to_t8_mode_conv(patt_gen_cfg_ptr->patt_cfg.mode, &t8_mode);
        
        if (PMC_SUCCESS == result)
        {       
            /*  set user-defined pa_monttern when mode matches */
            if (T8_PATT_MODE_USER_DEF == t8_mode ||
                T8_PATT_MODE_CJAT == t8_mode)
            {
                result = t8_validate_usr_patt(&patt_gen_cfg_ptr->patt_cfg);
                if (PMC_SUCCESS != result)
                {
                    PMC_RETURN(result);
                }

                usr_patt_size = patt_gen_cfg_ptr->patt_cfg.usr_patt_size / 32;
                if ((patt_gen_cfg_ptr->patt_cfg.usr_patt_size % 32) > 0)
                {
                    usr_patt_size += 1;
                }
            
                for (i = 0; i < usr_patt_size; i += 1)
                {
                    switch (i)
                    {
                    case 0:
                        t8_field_GEN_USR_PAT_31_0_set(NULL, handle, serdes_id, patt_gen_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 1:
                        t8_field_GEN_USR_PAT_39_32_set(NULL, handle, serdes_id, patt_gen_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    default: 
                        result = T8_LOG_ERR_INVALID_PARAMETERS;
                        break;
                    }
                }
            }    
        }
            
        if (PMC_SUCCESS == result)
        {  
            /* set check inv, pattern mode and select */
            t8_field_GEN_INV_set(&t8_buf, handle, serdes_id, (UINT32)patt_gen_cfg_ptr->patt_cfg.inv_cfg);
            t8_field_GEN_MODE_set(&t8_buf, handle, serdes_id, (UINT32)t8_mode);
            t8_field_PATT_GEN_SEL_set(&t8_buf, handle, serdes_id,  T8_PATT_GEN_SEL_PRBS_DATA_VAL); 
            t8_field_GEN_EN_set(&t8_buf, handle, serdes_id, (UINT32)patt_gen_cfg_ptr->patt_cfg.enable);
            /* flush t8 buffer */ 
            t8_buffer_flush(&t8_buf);  
        }       
        /* reset serdes generator until configuration is over */
        t8_field_GEN_RSTB_set(NULL, handle, serdes_id, 0);
        /* clear serdes generator reset */
        t8_field_GEN_RSTB_set(NULL, handle, serdes_id, 1);
    }
    else
    {
        t8_field_PATT_GEN_SEL_set(&t8_buf, handle, serdes_id,  T8_PATT_GEN_SEL_NORMAL_DATA_VAL);
        t8_field_GEN_EN_set(&t8_buf, handle, serdes_id, (UINT32)patt_gen_cfg_ptr->patt_cfg.enable);
        /* flush t8 buffer */ 
        t8_buffer_flush(&t8_buf);  
    }

    /* update status */
    handle->var[serdes_id].patt_gen_enable = patt_gen_cfg_ptr->patt_cfg.enable;

    PMC_RETURN(result);
}/*t8_patt_gen_cfg*/


/*******************************************************************************
* t8_patt_gen_err_insert_cfg
* ______________________________________________________________________________
*
** DESCRIPTION:
*   The function enables one error bit insertion in generated pattern.
*
* INPUTS:
*   *handle         - pointer to t8 serdes handle.
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
PUBLIC PMC_ERROR t8_patt_gen_err_insert_cfg(t8_handle_t *handle,
                                            UINT8 serdes_id)
 
{
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, T8_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes ID */
    if (T8_MAX_NUM_SERDES <= serdes_id)
    {
        PMC_RETURN(T8_LOG_ERR_SERDES_ID_INVALID);
    }


    /* emulate rising edge on GEN_ERR_INJ register bit */
    /* in case bit is already high */
    t8_field_GEN_ERR_INJ_set(NULL, handle, serdes_id, 0);
    t8_field_GEN_ERR_INJ_set(NULL, handle, serdes_id, 1);

    /* lower register bit  */
    t8_field_GEN_ERR_INJ_set(NULL, handle, serdes_id, 0);
    
    PMC_RETURN(PMC_SUCCESS);
}/*t8_patt_gen_err_insert_cfg*/


/*******************************************************************************
* t8_patt_gen_bus_width_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function sets pattern bus width associated to serdes rate.
*
* INPUTS:
*   *handle         - pointer to t8 handle.
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
PUBLIC void t8_patt_gen_bus_width_set(t8_handle_t           *handle,
                                      UINT8                  serdes_id,
                                      UINT8                  bus_width)
{

    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, T8_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(serdes_id < T8_MAX_NUM_SERDES,T8_LOG_ERR_SERDES_ID_INVALID,0,0);

    t8_field_PATT_BUS_WIDTH_set(NULL, handle, serdes_id, bus_width);
    
    PMC_RETURN();
}/*t8_patt_gen_bus_width_set*/

/*******************************************************************************
* t8_loopback_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function activate loopbacks.
*
* INPUTS:
*   *handle         - pointer to t8 handle.
*   serdes_id       - serdes identifier.
*   loopback_mode   - loopback mode to activate
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
PUBLIC PMC_ERROR t8_loopback_activate(t8_handle_t *handle,
                                      UINT8 serdes_id,
                                      t8_loopback_mode_t loopback_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, T8_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(serdes_id < T8_MAX_NUM_SERDES,T8_LOG_ERR_SERDES_ID_INVALID,0,0);
    if (handle->var[serdes_id].loopback_mode != LAST_T8_LOOPBACK_MODE)
    {
        PMC_RETURN(T8_LOOPBACK_ALREADY_CONFIGURED);
    }
    
    /* check if we have a conflict between pattern generator and FACILITY loopback */
    if (T8_FACILITY_LOOPBACK == loopback_mode && TRUE == handle->var[serdes_id].patt_gen_enable)
    {
        PMC_RETURN(T8_PATT_GEN_FACILITY_LOOPBACK_CONFLICT);
    }

    /* configure the loopback */
    handle->var[serdes_id].loopback_mode = loopback_mode;
    t8_loopback_cfg(handle,serdes_id,TRUE);

    PMC_RETURN(rc);
} /* t8_loopback_activate */

/*******************************************************************************
* t8_loopback_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function deactivate loopbacks.
*
* INPUTS:
*   *handle         - pointer to t8 handle.
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
PUBLIC PMC_ERROR t8_loopback_deactivate(t8_handle_t *handle,
                                        UINT8 serdes_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, T8_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(serdes_id < T8_MAX_NUM_SERDES,T8_LOG_ERR_SERDES_ID_INVALID,0,0);
    if (handle->var[serdes_id].loopback_mode == LAST_T8_LOOPBACK_MODE)
    {
        PMC_RETURN(T8_LOOPBACK_ALREADY_NOT_CONFIGURED);
    }
   
    /* configure the loopback */
    t8_loopback_cfg(handle,serdes_id,FALSE);
    handle->var[serdes_id].loopback_mode = LAST_T8_LOOPBACK_MODE;

    PMC_RETURN(rc);
} /* t8_loopback_deactivate */

/*******************************************************************************
* t8_loopback_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function returns the status of the T8 SERDES loopback. If loopbacks are
*   disabled, the return value will be 'LAST_T8_LOOPBACK'.
*
* INPUTS:
*   *handle             - pointer to t8 handle.
*   serdes_id           - serdes identifier.
*
* OUTPUTS:
*  *loopback_mode_ptr   - Current loopback mode
*
* RETURNS:
*   PMC_SUCCESS         - when API execution is successful otherwise a 
*                         descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR t8_loopback_status_get(t8_handle_t *handle,
                                        UINT8 serdes_id,
                                        t8_loopback_mode_t *loopback_mode_ptr)
{
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, T8_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(serdes_id < T8_MAX_NUM_SERDES,T8_LOG_ERR_SERDES_ID_INVALID,0,0);
    
    *loopback_mode_ptr = handle->var[serdes_id].loopback_mode;

    PMC_RETURN(PMC_SUCCESS);
} /* t8_loopback_status_get */

/*******************************************************************************
* t8_patt_mon_err_count_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function reads and returns pattern monitor status and errors counter value.
*
* INPUTS:
*   *handle                 - t8 handle
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
PUBLIC PMC_ERROR t8_patt_mon_err_count_get(t8_handle_t           *handle,
                                           UINT8                  serdes_id,
                                           util_patt_status_t    *status,
                                           UINT32                *err_count)
{

    /* check parameters */
    PMC_ASSERT(handle != NULL, T8_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(status != NULL, T8_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(err_count != NULL, T8_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    PMC_ENTRY();

    /* emulate faling-edge to allow future read */
    t8_field_CHK_READ_set (NULL, handle, serdes_id, 0);

    /* activate pattern monitor error counter read and update in mon_err_cnt register  */
    t8_field_CHK_READ_set (NULL, handle, serdes_id, 1);

    /* emulate faling-edge to allow future read */
    t8_field_CHK_READ_set (NULL, handle, serdes_id, 0);

    *err_count = t8_field_CHK_ERR_CNT_get (NULL, handle, serdes_id);
    
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
* util_patt_mode_to_t8_mode_conv 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert pattern mode defined defined in util global to t8 pattern mode enum.
*
*
* INPUTS:
*  util_patt_mode         - util global pattern mode enum
*
* OUTPUTS:
*   t8_mode               - t8 pattern mode enum
*
* RETURNS:
*   T8_LOG_ERR_MODE_NOT_FOUND   - when enum cannot be converted.
*   PMC_SUCCESS                 - when conversion is successful 
*
* NOTES:
*
******************************************************************************/
PRIVATE PMC_ERROR util_patt_mode_to_t8_mode_conv(util_patt_mode_t util_patt_mode,
                                                 t8_patt_mode_t *t8_mode)
{
    PMC_ENTRY();

    switch (util_patt_mode)
    {
    case UTIL_PATT_MODE_PRBS_5:
        *t8_mode = T8_PATT_MODE_PRBS_5;
        break;  
    case UTIL_PATT_MODE_PRBS_7:
        *t8_mode = T8_PATT_MODE_PRBS_7;
        break;   
    case UTIL_PATT_MODE_PRBS_11:
        *t8_mode = T8_PATT_MODE_PRBS_11;
        break;
    case UTIL_PATT_MODE_PRBS_15:
        *t8_mode = T8_PATT_MODE_PRBS_15;
        break;
    case UTIL_PATT_MODE_PRBS_23:
        *t8_mode = T8_PATT_MODE_PRBS_23;
        break;
    case UTIL_PATT_MODE_PRBS_31:
        *t8_mode = T8_PATT_MODE_PRBS_31;
        break;
    case UTIL_PATT_MODE_CJAT:
        *t8_mode = T8_PATT_MODE_CJAT;
        break;
    case UTIL_PATT_MODE_USER_DEF:
        *t8_mode = T8_PATT_MODE_USER_DEF;
        break;
    default:
        PMC_RETURN(T8_LOG_ERR_MODE_NOT_FOUND);
    }

    PMC_RETURN(PMC_SUCCESS);
        
}/*util_patt_mode_to_t8_mode_conv*/


/*******************************************************************************
* t8_validate_usr_patt
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function verifies that the length of user-defined pattern is compatible 
*   to T8 serdes.
*
*
* INPUTS:
*  patt_cfg         - pointer to pattern configuration
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS                 - when user-defined pattern is correct is successful 
*                                 otherwise a descriptive error is returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR t8_validate_usr_patt(util_serdes_patt_cfg_t * patt_cfg)
{
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();
    
    PMC_ASSERT(NULL != patt_cfg,T8_LOG_ERR_NULL_HANDLE,0,0);
    if (patt_cfg->usr_patt_size != T8_USR_PATT_BIT_SIZE)
    {
        result = T8_LOG_ERR_USR_PATT_SIZE_INVALID;
    }

    PMC_RETURN(result);
} /* util_patt_mode_to_t8_mode_conv */

/*******************************************************************************
* t8_loopback_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function configures a T8 instance to enable/disable loopback
*
* INPUTS:
*   *handle         - pointer to T8 handle.
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
PRIVATE void t8_loopback_cfg(t8_handle_t *handle,
                             UINT8 serdes_id,
                             BOOL8 enable)
{
    t8_buffer_t b[1];

    PMC_ENTRY();

    t8_buffer_init(b,handle,serdes_id);

    t8_field_DLOS_RSTB_set(NULL,handle,serdes_id,(enable ? 0 : 1));

    if (TRUE == enable)
    {
        t8_field_DIG_LPBK_SYNC_set(NULL,handle,serdes_id, 1);
    }

    t8_field_TPCOUT_DEGLITCH_EN_set(NULL,handle,serdes_id, 0);
    t8_field_BADJ_CLK_DEGLITCH_EN_set(NULL,handle,serdes_id, 0);
    if (T8_DIAGNOSTIC_LOOPBACK == handle->var[serdes_id].loopback_mode) 
    {    
        t8_field_TX_TO_RX_LPBK_EN_set(b,handle,serdes_id,(enable ? 1 : 0));
    } else 
    {
        t8_field_RX_TO_TX_LPBK_EN_set(b,handle,serdes_id,(enable ? 1 : 0));
    }
    t8_field_DLB_ENB_set(b,handle,serdes_id,(enable ? 0 : 1));

    t8_buffer_flush(b);

        
    t8_field_TPCOUT_DEGLITCH_EN_set(NULL,handle,serdes_id, 1);
    t8_field_BADJ_CLK_DEGLITCH_EN_set(NULL,handle,serdes_id, 1);

    if (TRUE == enable)
    {
        PMC_Q_USLEEP(handle,2);
        t8_field_DIG_LPBK_SYNC_set(NULL,handle,serdes_id,0);
    }

    t8_field_PCBI_FIFO_HW_RESET_set(NULL,handle,serdes_id,1);  
    t8_field_PCBI_FIFO_HW_RESET_set(NULL,handle,serdes_id,0);  

    PMC_RETURN();
}

/*******************************************************************************
* t8_polarity_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function configure a T8 instance to enable/disable lane polarity 
*   inversion
*
* INPUTS:
*   *handle         - pointer to T8 handle.
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
PUBLIC void t8_polarity_cfg(t8_handle_t              *handle,
                            UINT8                     serdes_id,
                            util_global_direction_t   dir,
                            BOOL8                     enable)
{
    BOOL cfg_rx = FALSE;
    BOOL cfg_tx = FALSE;

    PMC_ENTRY();
       
    /* check parameters */
    PMC_ASSERT(NULL != handle, T8_LOG_ERR_NULL_HANDLE, 0, 0);

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
        t8_field_INV_TX_DATA_set(NULL, handle, serdes_id, (UINT32) enable);
    }

    if (TRUE == cfg_rx)
    {
        t8_40n_field_INV_DFE_OUT_set(NULL, handle, serdes_id, ((UINT32) ~enable) & 0x1); 
        t8_adsp_field_INV_DFE_OUT_set(NULL, handle, serdes_id, ((UINT32) ~enable) & 0x1);
    }

    PMC_RETURN();
 
} /* t8_polarity_cfg */

/*******************************************************************************
* t8_dfe_error_invert
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function inverts the error signal of the equalizer. 
*   This function should be called only when serdes polarity is inverted.
*
* INPUTS:
*   *handle         - pointer to T8 handle.
*   serdes_id       - serdes identifier.
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
PUBLIC void t8_dfe_error_invert(t8_handle_t *handle,
                                UINT8 serdes_id)
{
    
    PMC_ENTRY();
       
    /* check parameters */
    PMC_ASSERT(NULL != handle, T8_LOG_ERR_NULL_HANDLE, 0, 0);

    handle->var[serdes_id].inv_dfe_error = ((UINT8) ~handle->var[serdes_id].inv_dfe_error) & 0x1;
    t8_40n_field_INV_DFE_ERROR_set(NULL, handle, serdes_id, (UINT8)handle->var[serdes_id].inv_dfe_error); 
                                            
    PMC_RETURN();
 
} /* t8_dfe_error_invert */


/*******************************************************************************
* t8_dfe_error_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function returns value of T8 serdes error signal of the equalizer. 
*
* INPUTS:
*   *handle         - pointer to T8 handle.
*   serdes_id       - serdes identifier.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   Value of error signal of the equalizer.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 t8_dfe_error_get(t8_handle_t *handle,
                              UINT8 serdes_id)
{
    
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(NULL != handle, T8_LOG_ERR_NULL_HANDLE, 0, 0);
                  
    PMC_RETURN(handle->var[serdes_id].inv_dfe_error);
 
} /* t8_dfe_error_get */


/*******************************************************************************
* t8_tx_swing_emphasis_levels_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function configures a T8 tx swing and de-emphasis levels.
*   inversion
*
* INPUTS:
*   *handle          - pointer to T8 handle.
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
PUBLIC PMC_ERROR t8_tx_swing_emphasis_levels_set(t8_handle_t           *handle,
                                                 UINT8                  serdes_id,
                                                 t8_tx_swing_levels_t  *levels_cfg_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    t8_buffer_t b[1];
    
    PMC_ENTRY();

    PMC_ASSERT(NULL != handle, T8_LOG_ERR_NULL_HANDLE,0,0);
    PMC_ASSERT(NULL != levels_cfg_ptr, T8_LOG_ERR_SERDES_ID_INVALID, 0, 0);

    if (FALSE == t8_serdes_id_validate(serdes_id))
    {
        rc = T8_LOG_ERR_SERDES_ID_INVALID;
    }
    else 
    {
        t8_buffer_init(b,handle,serdes_id);
  
        t8_buffer_init(b,handle,serdes_id);
        t8_field_PCBI_TX_DPENI1_set(b, handle, serdes_id, levels_cfg_ptr->tx_dpeni1);
        t8_field_PCBI_TX_DPENI2_set(b, handle, serdes_id, levels_cfg_ptr->tx_dpeni2);
        t8_buffer_flush(b);    

        t8_field_PCBI_TX_DENI_set(NULL, handle, serdes_id, levels_cfg_ptr->deni);
        t8_field_PCBI_TX_PSELI_set(NULL, handle, serdes_id, levels_cfg_ptr->tx_psel);
        t8_field_TX_WIZARD_EN_set(NULL, handle, serdes_id, 0x0);
    }

    PMC_RETURN(rc);    
} /* t8_tx_swing_emphasis_levels_set */

/*******************************************************************************
* t8_dc_coupling_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*  The function configures a T8 serdes in DC coupling mode or put it back in
*  in AC coupling mode.
*
* INPUTS:
*   *handle              - pointer to T8 handle.
*   serdes_id            - serdes identifier.
*   dc_coupling_enabled  - when TRUE, DC coupling is enabled otherwise AC coupling is enabled
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
PUBLIC PMC_ERROR t8_dc_coupling_set(t8_handle_t    *handle,
                                    UINT8           serdes_id,
                                    BOOL8           dc_coupling_enabled)
{
    PMC_ERROR rc = PMC_SUCCESS;
    t8_buffer_t b[1];
    
    PMC_ENTRY();

    PMC_ASSERT(NULL != handle, T8_LOG_ERR_NULL_HANDLE,0,0);
       
    if (FALSE == t8_serdes_id_validate(serdes_id))
    {
        rc = T8_LOG_ERR_SERDES_ID_INVALID;
    }
    else 
    {
        t8_buffer_init(b,handle,serdes_id);
        if (TRUE == dc_coupling_enabled)
        {
            t8_field_range_TXRXCS_MODE_set(NULL, handle, serdes_id, 0, 1, 0x2);
        }
        else
        {
            t8_field_range_TXRXCS_MODE_set(NULL, handle, serdes_id, 0, 1, 0x3);
        }

        t8_buffer_flush(b);    
    }

    PMC_RETURN(rc);    
} /* t8_dc_coupling_set */

/*******************************************************************************
* t8_serdes_id_validate
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
PRIVATE BOOL8 t8_serdes_id_validate(UINT8 serdes_id)
{
    BOOL8 rc;

    PMC_ENTRY();

    rc =    T8_MAX_NUM_SERDES <= serdes_id ? FALSE : TRUE;

    PMC_RETURN(rc);
}/*serdes_id_validate*/

/*******************************************************************************
* t8_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on t8 handle instance.
*
* INPUTS:
*   *t8_handle             - pointer to t8 handle instance
*   *restart_init_cfg_ptr  - pointer to restart init config
*    t8_reset_msk          - T8 serdes reset bits mask 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS : 
*     WARM : register/context are coherent
*     CRASH: everything is coherent (a cleanup may has been executed)
*   T8_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT:
*     incoherences have been detected on communal energy configuration
*     that is incoherent with top energy state
*      WARM : ERROR
*      CRASH: TOP shall put T8 in RESET mode
*   
subsystem shall be put in reset mode 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR t8_handle_restart_init(t8_handle_t                     *t8_handle,
                                        util_global_restart_init_cfg_t  *restart_init_cfg_ptr,
                                        UINT32                           t8_reset_msk)
{
    PMC_ERROR           rc = PMC_SUCCESS;
    t8_var_t           *t8_var_clean;
    
    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(T8_LOG_ERR_STRINGS[0], T8_LOG_ERR_TABLE_BASE, T8_LOG_ERR_COUNT);
    }

    t8_var_clean =  (t8_var_t *) PMC_CALLOC(sizeof(t8_var_t) * T8_MAX_NUM_SERDES); 

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
     *   - Create a new context that represents the intersection between memory and register space (t8_var_clean)
     */
    rc = t8_ctxt_audit(t8_handle,
                       t8_var_clean,
                       t8_reset_msk); 
    
    PMC_LOG_TRACE("t8_ctxt_audit rc = %s\n", pmc_log_string_get(rc));
    if (T8_ERR_CONTEXT_CLEANUP == rc && restart_init_cfg_ptr->is_warm_restart == FALSE)
    {
        /* 
         * Cleanup 
         *   - At the end, t8_var_clean is the new context
         *    - We keep OPERATIONAL channel defined in t8_var_clean
         *    - We clean (deactivate/deprov) all channels that are not OPERATIONAL
         *       - they may have be partially provisionned
         *    - We ajust power setting based on t8_var_clean
         */                                    
        rc = t8_ctxt_cleanup(t8_handle,
                             t8_var_clean); 
    }

    PMC_FREE(&t8_var_clean);

    PMC_RETURN(rc);
} /* t8_handle_restart_init */

/*******************************************************************************
* t8_ctxt_audit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function audits T8 handle context variable against register space content.
*
* INPUTS:
*   *t8_handle           - pointer to T8 handle instance.
*   *t8_var_clean        - pointer to T8 lanes context.
*    t8_reset_msk        - T8 serdes reset bits mask.
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
PRIVATE PMC_ERROR t8_ctxt_audit(t8_handle_t *t8_handle,
                                t8_var_t    *t8_var_clean,
                                UINT32       t8_reset_msk)
{
    UINT32 itr;
    UINT32 jtr;
    BOOL8 serdes_ctxt_match;
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL8 serdes_reset;
    t8_loopback_mode_t lpbk_mode;
    BOOL8 patt_gen_enabled;

    PMC_ENTRY();

    PMC_ASSERT(t8_handle != NULL, T8_LOG_ERR_INVALID_PARAMETERS,0,0);

    for (itr = 0; itr < T8_MAX_NUM_SERDES; itr += 1)
    {
        serdes_ctxt_match = TRUE;
        serdes_reset = (0x1 == (0x1 & (t8_reset_msk >> (itr / T8_MAX_NUM_SERDES)))) ? TRUE : FALSE;

        if (FALSE == serdes_reset)
        {
            /* check t8 provisioning & activation status */
            if (TRUE == t8_handle->var[itr].reset)
            {
                for (jtr = 0; jtr < T8_MAX_NUM_SERDES; jtr += 1)
                {
                    if (itr == jtr)
                    {
                        continue;
                    }
                    if (FALSE == t8_handle->var[jtr].reset)
                    {
                        break;
                    }
                }
                if (T8_MAX_NUM_SERDES == jtr)
                {
                    serdes_ctxt_match = FALSE;
                }
            } 
            
            /* check pattern generator status */
            if (TRUE == serdes_ctxt_match)
            {
                rc = t8_tx_patt_gen_status_get(t8_handle,itr, &patt_gen_enabled);
                if (PMC_SUCCESS == rc)
                {
                    if (patt_gen_enabled != t8_handle->var[itr].patt_gen_enable)
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
                rc = t8_loopback_cfg_get(t8_handle,itr, &lpbk_mode );
                if (PMC_SUCCESS == rc)
                {
                    if (t8_handle->var[itr].loopback_mode !=
                        lpbk_mode)
                    {
                        serdes_ctxt_match = FALSE;                        
                    }
                }
                else
                {
                    serdes_ctxt_match = FALSE; 
                }
            }            
        }
        else
        {
            /* check t8 provisioning & activation status */
            if (TRUE != t8_handle->var[itr].reset)
            {
                serdes_ctxt_match = FALSE;               
            }
        }

        /* update returned error code anf var clean (used to cleanup the context) */
        if (FALSE == serdes_ctxt_match)
        {
            if (PMC_SUCCESS == rc)
            {
                rc = T8_ERR_CONTEXT_CLEANUP;
            }
            t8_var_default_init(&t8_var_clean[itr]);
        }   
        else
        {
            if (TRUE != serdes_reset ||
                TRUE == t8_handle->var[itr].reset)
            {
                PMC_MEMCPY((void*)&t8_var_clean[itr], (void*)&t8_handle->var[itr], sizeof(t8_var_t));
            }
        } 
    }

    PMC_RETURN(rc);
} /* t8_ctxt_audit */

/*******************************************************************************
* t8_ctxt_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function cleans up T8 handle context. 
*
* INPUTS:
*   *t8_handle           - pointer to T8 handle instance.
*   *t8_var_clean        - pointer to T8 lanes clean context.
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
PRIVATE PMC_ERROR t8_ctxt_cleanup(t8_handle_t  *t8_handle,
                                  t8_var_t     *t8_var_clean)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(t8_handle != NULL, T8_LOG_ERR_INVALID_PARAMETERS,0,0);

    PMC_MEMCPY(t8_handle->var, t8_var_clean, sizeof(t8_var_t) * T8_MAX_NUM_SERDES);

    PMC_RETURN(rc);
} /* t8_ctxt_cleanup */

/*******************************************************************************
* t8_loopback_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns T8 instance configured loopback mode.
*
* INPUTS:
*   *handle         - pointer to t8 handle.
*   serdes_id       - serdes index.
*
* OUTPUTS:
*   *lpbk_mode      - loopback mode..
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR t8_loopback_cfg_get(t8_handle_t         *handle, 
                                      UINT8                serdes_id,
                                      t8_loopback_mode_t  *lpbk_mode) 
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    *lpbk_mode = LAST_T8_LOOPBACK_MODE;
    
    /* consult register space to determine configure loopback mode */
    if (0x0 == t8_field_DLB_ENB_get(NULL,handle,serdes_id))
    {
        if (0x1 == t8_field_TX_TO_RX_LPBK_EN_get(NULL, handle,serdes_id))
        {
            *lpbk_mode = T8_DIAGNOSTIC_LOOPBACK;
        }
        if (0x1 == t8_field_RX_TO_TX_LPBK_EN_get(NULL, handle,serdes_id))
        {
            if (LAST_T8_LOOPBACK_MODE == *lpbk_mode)
            {
                *lpbk_mode = T8_FACILITY_LOOPBACK;
            }
            else
            {
                rc = T8_ERR_CONTEXT_REG_INCOHERENT;
            }
        }
    }

    PMC_RETURN(rc);
}/*t8_loopback_cfg_get*/

/*******************************************************************************
* t8_tx_patt_gen_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns T8 instance configured loopback mode.
*
* INPUTS:
*   *handle         - pointer to t8 handle.
*   serdes_id       - serdes index.
*
* OUTPUTS:
*   *enabled       - pattern generator enable status.
*
* RETURNS:
*   PMC_SUCCESS - if successful, otherwise T8_ERR_CONTEXT_REG_INCOHERENT
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR t8_tx_patt_gen_status_get(t8_handle_t   *handle, 
                                            UINT8          serdes_id,
                                            BOOL8         *enabled) 
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    *enabled = FALSE;
    
    /* consult register space to determine patter monitor status */
    if (0x1 == t8_field_GEN_EN_get(NULL, handle, serdes_id))
    {
        *enabled = TRUE;
        if (T8_PATT_GEN_SEL_PRBS_DATA_VAL != t8_field_PATT_GEN_SEL_get(NULL, handle, serdes_id))
        {            
            rc = T8_ERR_CONTEXT_REG_INCOHERENT;
        }
    }

    PMC_RETURN(rc);
}/*t8_tx_patt_gen_status_get*/

/*******************************************************************************
* t8_ext_ctxt_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set t8 lanes state in reset or start mode.
*
* INPUTS:
*   *t8_handle            - pointer to T8 handle instance.
*   lane_msk              - lanes mask
*   out_of_reset          - When TRUE state of T8 lane is set to START mode.
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
PUBLIC PMC_ERROR t8_ext_ctxt_state_set(t8_handle_t       *t8_handle, 
                                       UINT16             lane_msk,
                                       BOOL8              out_of_reset) 
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 lane_itr;

    PMC_ENTRY();    

    PMC_ASSERT(t8_handle != NULL, T8_LOG_ERR_INVALID_PARAMETERS,0,0);

    /* for each lane, set update state */
    for (lane_itr = 0; lane_itr < T8_MAX_NUM_SERDES; lane_itr += 1)
    {
        if (0x1 == (0x1 & (lane_msk >> lane_itr)))
        {
            if (TRUE == out_of_reset)
            {
                /* this function should be call only once and before loopback activation*/
                if (TRUE == t8_handle->var[lane_itr].reset)
                {
                    t8_handle->var[lane_itr].reset = FALSE;
                }
                else
                {
                    rc = T8_STATE_NOT_RESET;
                    break;
                }
            }
            else
            {
                t8_handle->var[lane_itr].reset = TRUE;
                t8_var_default_init(&t8_handle->var[(lane_itr)]);
            }
        }
    }
        
    PMC_RETURN(rc);
}/*t8_ext_ctxt_state_set*/

/*******************************************************************************
* t8_reset_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns T8 instance reset status based on 
*    context content.
*
* INPUTS:
*   *handle         - pointer to t8 handle.
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
PUBLIC BOOL8 t8_reset_state_get(t8_handle_t *handle,
                                UINT32       serdes_id) 
{
    BOOL8 rc;

    PMC_ENTRY();

    rc = handle->var[serdes_id].reset;
    
    PMC_RETURN(rc);
}/*t8_reset_state_get*/

/*******************************************************************************
* t8_vert_hist_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves T8 SerDes histogram information.
*
* INPUTS:
*   *t8_handle_all  - Pointer to t8 handle.
*   lane_0_19_msk   - Bitmask of lanes 0-19 to configure. 
*                     Bit 0 corresponds to lane 0.
*   lane_20_39_msk  - Bitmask of lanes 20-39 to configure. 
*                     Bit 0 corresponds to lane 20.
*                     Ignored for S16 serdes.
*   test_length     - Enum,requested the number of samples to capture
*                     UTIL_VHIST_DEBUG
*                     UTIL_VHIST_SHORT
*                     UTIL_VHIST_LONG
*                     UTIL_VHIST_USERDEF
*   userdef_length  - The vertical histgram counter-timer limit value 
*                     that is defined by user.
*                     This parameter is only valid if test_lenth is set to 
*                     UTIL_VHIST_USERDEF.
*
* OUTPUTS:
*   bin_buffer      - T8 histogram value.
*
* RETURNS:
*  PMC_SUCCESS    - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR t8_vert_hist_get(t8_handle_t     **t8_handle_all,
                                  UINT32            lane_0_19_msk,
                                  UINT32            lane_20_39_msk,
                                  util_vhist_test_length_t  test_length,
                                  UINT64            userdef_length,
                                  UINT64            bin_buffer[][64])
{
    UINT32 i;
    UINT32 j;
    UINT32 capture_running;
    UINT32 bin;
    UINT32 slice; 
    UINT64 bin_value_31_0;
    UINT64 bin_value_39_32;
    UINT32 eye_index;
    UINT32 id = 0;
    UINT32 itr = 0;
    UINT64 msk = 0;
    UINT64 flag_one = 0;	
    UINT64 all_lane_msk = 0;
    UINT32 cnt_tim_limit_31_0 = 0, cnt_tim_limit_39_32 = 0;
    t8_handle_t *t8_handle;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT((NULL != t8_handle_all), T8_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    all_lane_msk = lane_20_39_msk;
    all_lane_msk = (all_lane_msk << (T8_MAX_NUM_SERDES*10)) + lane_0_19_msk;

    switch(test_length) {
    case UTIL_VHIST_LONG:
        cnt_tim_limit_31_0 = 0xFFFFFFFF;
        cnt_tim_limit_39_32 = 0x0F;
        break;
    case UTIL_VHIST_SHORT:
        cnt_tim_limit_31_0 = 0xFFFFFFFF;
        cnt_tim_limit_39_32 = 0x0;
        break;
    case UTIL_VHIST_USERDEF:
        cnt_tim_limit_31_0 = (UINT32)(userdef_length & 0xFFFFFFFF);
        cnt_tim_limit_39_32 = (UINT32)((userdef_length >> 32) & 0xFF);
        break;
    case UTIL_VHIST_DEBUG:
    default:
        cnt_tim_limit_31_0 = 0x0FFFFFFF;
        cnt_tim_limit_39_32 = 0x0;
        break;
    }

    flag_one = 1;    
    for (itr = 0; itr < UTIL_GLOBAL_MAX_T8_E1_IN_BP_LR; itr++)
    {
        /* Immediate can not shift more than 30 bit. Need use one uint64 valiable to handle the shift */
        msk  = flag_one << itr;

        if(0 != (all_lane_msk & msk))
        {
            slice = itr / T8_MAX_NUM_SERDES;
            id    = ((itr % 2)== 0) ? 1 : 0;
            t8_handle = t8_handle_all[slice];

            /* Set VHIST_CNT_TIM_LIMIT_31_0 to 0xFFFFFFFF*/
            t8_field_VHIST_CNT_TIM_LIMIT_31_0_set(NULL, t8_handle, id, cnt_tim_limit_31_0);

            /* Set VHIST_CNT_TIM_LIMIT_39_32 to 0x00 */
            t8_field_VHIST_CNT_TIM_LIMIT_39_32_set(NULL, t8_handle, id, cnt_tim_limit_39_32);

            /* Set VHIST_PERBIN_VALUE_RD_ENB to 1 */
            t8_field_VHIST_PERBIN_VALUE_RD_ENB_set(NULL, t8_handle, id, 0x1);

            /* Set VHIST_CAPT_ENB to 1 */
            t8_field_VHIST_CAPT_ENB_set(NULL, t8_handle, id, 0x1);

            /* Set VHIST_TITAN_TARGET_AMP_WR_EN to 1 */
            t8_field_VHIST_TITAN_TARGET_AMP_WR_EN_set(NULL, t8_handle, id, 0x1);

            /* Set VHIST_CAPT_START to 0 */
            t8_field_VHIST_CAPT_START_set(NULL, t8_handle, id, 0x0);

            /* Set VHIST_TITAN_TARGET_AMP_WR_EN to 0 */
            t8_field_VHIST_TITAN_TARGET_AMP_WR_EN_set(NULL, t8_handle, id, 0x0);

            /* Set VHIST_CAPT_ENB to 0 */
            t8_field_VHIST_CAPT_ENB_set(NULL, t8_handle, id, 0x0);

            /* Set VHIST_CAPT_START to 1 */
            t8_field_VHIST_CAPT_START_set(NULL, t8_handle, id, 0x1);

        }
    }
    
    bin  = 0;
    for (i = 0; i < 8; i++)
    {
        /* Wait for captures to complete */
        for (itr = 0; itr < UTIL_GLOBAL_MAX_T8_E1_IN_BP_LR; itr++)
        {
        /* Immediate can not shift more than 30 bit. Need use one uint64 valiable to handle the shift */
            msk  = flag_one << itr;

            if(0 != (all_lane_msk & msk))
            {
                slice = itr / T8_MAX_NUM_SERDES;
                id    = ((itr % 2)== 0) ? 1 : 0;
                t8_handle = t8_handle_all[slice];
                
                do
                {
                    capture_running = t8_field_VHIST_CAPT_RUNNING_get(NULL, t8_handle, id);
                    PMC_ATOMIC_YIELD(t8_handle->base.parent_handle, 10000);
                } 
                while (capture_running & 0x1);
            }
        }
        
        for (j = 0; j < 8; j++)
        {
            for (itr = 0; itr < UTIL_GLOBAL_MAX_T8_E1_IN_BP_LR; itr++)
            {
            /* Immediate can not shift more than 30 bit. Need use one uint64 valiable to handle the shift */
                msk  = flag_one << itr;
                
                if(0 != (all_lane_msk & msk))
                {
                    slice = itr / T8_MAX_NUM_SERDES;
                    id    = ((itr % 2)== 0) ? 1 : 0;
                    t8_handle = t8_handle_all[slice];
                
                    bin_value_31_0 = t8_field_VHIST_PERBIN_VALUE_31_0_get(NULL, t8_handle, id);
                    bin_value_39_32 = t8_field_VHIST_PERBIN_VALUE_39_32_get(NULL, t8_handle, id);

                    /* Reorder the bins.  Bin number 0 to 31 is the center of the eye to the top of the eye.
                       Bin number 63 to 32 is the bottom of the eye to the center. */
                    if (bin < 32)
                    {
                        eye_index = 31 - bin;
                    }
                    else
                    {
                        eye_index = 95 - bin;
                    }
                    bin_buffer[itr][eye_index] = bin_value_31_0 + ( bin_value_39_32 << 32 );
        			

                    /* Set VHIST_PERBIN_VALUE_RD_ENB to 0 */
                    t8_field_VHIST_PERBIN_VALUE_RD_ENB_set(NULL, t8_handle, id, 0x0);

                    /* Set VHIST_PERBIN_VALUE_RD_ENB to 1 */
                    t8_field_VHIST_PERBIN_VALUE_RD_ENB_set(NULL, t8_handle, id, 0x1);
                }
            }
            bin++;
        }
    }

    for (itr = 0; itr < UTIL_GLOBAL_MAX_T8_E1_IN_BP_LR; itr++)
    {
      /* Immediate can not shift more than 30 bit. Need use one uint64 valiable to handle the shift */
       msk  = flag_one << itr;
        
        if(0 != (all_lane_msk & msk))
        {
            slice = itr / T8_MAX_NUM_SERDES;
            id    = ((itr % 2)== 0) ? 1 : 0;
            t8_handle = t8_handle_all[slice];

            /* Set VHIST_CAPT_START to 0 */
            t8_field_VHIST_CAPT_START_set(NULL, t8_handle, id, 0x0);
        }
    }
    
    PMC_RETURN(rc);
} /* t8_vert_hist_get */

/*
** End of file
*/

