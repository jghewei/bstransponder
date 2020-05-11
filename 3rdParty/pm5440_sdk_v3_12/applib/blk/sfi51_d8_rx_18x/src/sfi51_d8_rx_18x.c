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
*       Main SFI51_D8_RX_18X source file.
* 
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "sfi51_d8_rx_18x_loc.h"
#include "digi120_sfi51_rx_18x_glue_map.h"
#include "sfi51_rx_18x_serdes_glue_io.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
/* pattern monitor bus width default value */
#define  SFI51_D8_RX_18X_MON_BUS_WIDTH_VAL 8

#define SFI51_D8_RX_18X_SERDES_SLICE_GET(serdes_id) (serdes_id / D8_RX_2X_MAX_NUM_SERDES)
#define SFI51_D8_RX_18X_SERDES_PIN_GET(serdes_id) (serdes_id % D8_RX_2X_MAX_NUM_SERDES)
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
const char SFI51_D8_RX_18X_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    SFI51_D8_RX_18X_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char SFI51_D8_RX_18X_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward References
*/
PRIVATE BOOL8 sfi51_d8_rx_18x_validate_serdes_id(UINT8  serdes_id,
                                                 UINT8 *serdes_slice,
                                                 UINT8 *serdes_pin);

PRIVATE PMC_ERROR sfi51_d8_rx_18x_txdclk_cfg(sfi51_d8_rx_18x_handle_t *handle,
                                             BOOL8                     enable);

PRIVATE PMC_ERROR sfi51_d8_rx_18x_ctxt_audit(sfi51_d8_rx_18x_handle_t *sfi51_d8_rx_18x_handle,
                                             sfi51_d8_rx_18x_var_t    *sfi51_d8_rx_18x_var_clean,
                                             BOOL8                     serdes_reset);

PRIVATE PMC_ERROR sfi51_d8_rx_18x_ctxt_cleanup(sfi51_d8_rx_18x_handle_t  *sfi51_d8_rx_18x_handle,
                                               sfi51_d8_rx_18x_var_t     *sfi51_d8_rx_18x_var_clean);

PRIVATE void sfi51_d8_rx_18x_cleanup(sfi51_d8_rx_18x_handle_t *sfi51_d8_rx_18x_handle) ;

PRIVATE PMC_ERROR sfi51_d8_rx_18x_loopback_cfg_get(sfi51_d8_rx_18x_handle_t *handle, 
                                                   BOOL8                    *lpbk_enabled_ptr) ;

PRIVATE void sfi51_d8_rx_18x_var_default_init(sfi51_d8_rx_18x_var_t *var_ptr);
/*
** Public Functions
*/
/*******************************************************************************
* sfi51_d8_rx_18x_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a SFI51_D8_RX_18X block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the SFI51_D8_RX_18X block relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - pointer to TSB name
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   sfi51_d8_rx_18x_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC sfi51_d8_rx_18x_handle_t *sfi51_d8_rx_18x_ctxt_create(pmc_handle_t *parent, 
                                                             UINT32 base_address, 
                                                             pmc_sys_handle_t *sys_handle, 
                                                             const char *tsb_name )
{
    sfi51_d8_rx_18x_handle_t *sfi51_d8_rx_18x_handle;    
    UINT32 i;
    /*! String names for the handles for logging */
    const char* D8_RX_2X_HANDLE_NAME[] = {"D8_RX_2X_0", "D8_RX_2X_1",   \
                                          "D8_RX_2X_2", "D8_RX_2X_3",   \
                                          "D8_RX_2X_4", "D8_RX_2X_5",   \
                                          "D8_RX_2X_6", "D8_RX_2X_7",   \
                                          "D8_RX_2X_8"};
    
    PMC_ENTRY();    

    sfi51_d8_rx_18x_handle = (sfi51_d8_rx_18x_handle_t*)PMC_CTXT_CALLOC(sizeof(sfi51_d8_rx_18x_handle_t), parent);
    
    pmc_handle_init( parent, sfi51_d8_rx_18x_handle, sys_handle, PMC_MID_DIGI_SFI51_18X_RX_SERDES_GLUE, tsb_name, base_address );

    sfi51_d8_rx_18x_handle->d8_rx_2x_hndl = (d8_rx_2x_handle_t **)PMC_CTXT_CALLOC(sizeof(d8_rx_2x_handle_t *) * (SFI51_D8_RX_NUM_SLICES), parent);
    PMC_CTXT_REGISTER_PTR(&sfi51_d8_rx_18x_handle->d8_rx_2x_hndl, parent);

    /* prepare D8_RX_2X handle name */
    
    for (i = 0; i < (SFI51_D8_RX_NUM_SLICES); i++)
    {        
        sfi51_d8_rx_18x_handle->d8_rx_2x_hndl[i] = d8_rx_2x_ctxt_create((pmc_handle_t *) sfi51_d8_rx_18x_handle, 
                                                                        BASE_ADDR_DIGI120_SFI51_RX_18X_GLUE_SFI5_1_RX_2X_SERDES_SLICE_SFI51_RX_2X_SLICE(i) + base_address, 
                                                                        sys_handle, D8_RX_2X_HANDLE_NAME[i],
                                                                        SFI51_D8_RX_18X_MON_BUS_WIDTH_VAL);
        PMC_CTXT_REGISTER_PTR(&sfi51_d8_rx_18x_handle->d8_rx_2x_hndl[i], parent);
    }
    
    /* Register log strings. */
    pmc_log_block_strings_register(SFI51_D8_RX_18X_LOG_ERR_STRINGS[0], SFI51_D8_RX_18X_LOG_ERR_TABLE_BASE, SFI51_D8_RX_18X_LOG_ERR_COUNT);
    
    PMC_RETURN(sfi51_d8_rx_18x_handle);
} /* sfi51_d8_rx_18x_ctxt_create */  
 
/*******************************************************************************
* sfi51_d8_rx_18x_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a SFI51_D8_RX_18X block instance.
*
*
* INPUTS:
*   *sfi51_d8_rx_18x_handle        - pointer to SFI51_D8_RX_18X handle instance
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
PUBLIC void sfi51_d8_rx_18x_ctxt_destroy(sfi51_d8_rx_18x_handle_t *sfi51_d8_rx_18x_handle)
{
    UINT32 i;

    PMC_ENTRY();
    
    for (i = 0; i < (SFI51_D8_RX_NUM_SLICES); i++)
    {
        d8_rx_2x_ctxt_destroy(sfi51_d8_rx_18x_handle->d8_rx_2x_hndl[i]);
    }
    
    PMC_CTXT_FREE(&sfi51_d8_rx_18x_handle->d8_rx_2x_hndl, sfi51_d8_rx_18x_handle);
    PMC_CTXT_FREE(&sfi51_d8_rx_18x_handle, sfi51_d8_rx_18x_handle);

    PMC_RETURN();
} /* sfi51_d8_rx_18x_ctxt_destroy */

/*******************************************************************************
* sfi51_d8_rx_18x_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a SFI51_D8_RX_18X block handle instance.
*
*
* INPUTS:
*   *sfi51_d8_rx_18x_handle        - pointer to SFI51_D8_RX_18X handle instance
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
PUBLIC void sfi51_d8_rx_18x_handle_init(sfi51_d8_rx_18x_handle_t *sfi51_d8_rx_18x_handle)
{
    UINT32 i;
    PMC_ENTRY();

    sfi51_d8_rx_18x_var_default_init(&sfi51_d8_rx_18x_handle->var);

    for (i = 0; i < (SFI51_D8_RX_NUM_SLICES); i++)
    {
        d8_rx_2x_handle_init(sfi51_d8_rx_18x_handle->d8_rx_2x_hndl[i]);
    }

    PMC_RETURN();
} /* sfi51_d8_rx_18x_handle_init */

/*******************************************************************************
* sfi51_d8_rx_18x_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes the var context to its default values
*
* INPUTS:
*   *var_ptr          - pointer to sfi51_d8_rx_18x variable instance.
*
* OUTPUTS:
*   None
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void sfi51_d8_rx_18x_var_default_init(sfi51_d8_rx_18x_var_t *var_ptr)
{

    PMC_ENTRY();

    var_ptr->state = SFI51_D8_RX_18X_RESET; 
    var_ptr->loopback_enable = FALSE;

    PMC_RETURN();
} /*sfi51_d8_rx_18x_var_default_init  */

/*******************************************************************************
* sfi51_d8_rx_18x_loopback_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function activate loopback (TX2RX/Diagnostic Loopback).
*
* INPUTS:
*   *handle         - pointer to sfi51_d8_rx_18x handle.
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
PUBLIC PMC_ERROR sfi51_d8_rx_18x_loopback_activate(sfi51_d8_rx_18x_handle_t *handle)
{
    UINT8 i;
    UINT32 jtr;
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, SFI51_D8_RX_18X_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(handle->var.loopback_enable == FALSE, SFI51_D8_RX_18X_LOOPBACK_ALREADY_CONFIGURED, 0, 0);
    
    /* configure the loopback */
    for (i = 0; i < SFI51_D8_RX_NUM_SLICES && PMC_SUCCESS == rc; i++)
    {
        for (jtr = 0; jtr < D8_RX_2X_MAX_NUM_SERDES; jtr += 1)
        {
            rc = d8_rx_2x_loopback_activate(handle->d8_rx_2x_hndl[i], jtr);
        }
    }

    if (PMC_SUCCESS == rc) 
    {        
        handle->var.loopback_enable = TRUE;
    }
    PMC_RETURN(rc);
} /* sfi51_d8_rx_18x_loopback_activate */

/*******************************************************************************
* sfi51_d8_rx_18x_loopback_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function deactivate loopback (TX2RX/Disgnostic)
*
* INPUTS:
*   *handle         - pointer to sfi51_d8_rx_18x handle.
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
PUBLIC PMC_ERROR sfi51_d8_rx_18x_loopback_deactivate(sfi51_d8_rx_18x_handle_t *handle)
{
    UINT8 i;
    UINT8 jtr;
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, SFI51_D8_RX_18X_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(handle->var.loopback_enable == TRUE, SFI51_D8_RX_18X_LOOPBACK_ALREADY_NOT_CONFIGURED, 0, 0);
   
    /* configure the loopback */
    for (i = 0; i < SFI51_D8_RX_NUM_SLICES && PMC_SUCCESS == rc; i++)
    {
        for (jtr = 0; jtr < D8_RX_2X_MAX_NUM_SERDES; jtr += 1)
        {
            rc = d8_rx_2x_loopback_deactivate(handle->d8_rx_2x_hndl[i], jtr);
        }
    }

    if (PMC_SUCCESS == rc) 
    {        
        handle->var.loopback_enable = FALSE;
    }
    

    PMC_RETURN(rc);
} /* sfi51_d8_rx_18x_loopback_deactivate */

/*******************************************************************************
* sfi51_d8_rx_18x_patt_mon_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function configures one serdes SFI51_D8_RX_18X pattern monitor: 
*    - It sets pattern mode,
*    - It  enables/disables pattern inversion check. 
*    - It activates/disactivates pattern monitor.
*
*
* INPUTS:
*   *handle                - sfi51_d8_rx_18x handle
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
PUBLIC PMC_ERROR sfi51_d8_rx_18x_patt_mon_cfg(sfi51_d8_rx_18x_handle_t *handle,
                                              UINT8 serdes_id,
                                              util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr)
{

    PMC_ERROR result = PMC_SUCCESS;
    UINT8 d8_slice;
    UINT8 d8_pin;

    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(handle != NULL, SFI51_D8_RX_18X_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes ID */
    if (FALSE == sfi51_d8_rx_18x_validate_serdes_id(serdes_id, &d8_slice, &d8_pin))
    {
        PMC_RETURN(SFI51_D8_RX_18X_LOG_ERR_SERDES_ID_INVALID);
    }
    
    result = d8_rx_2x_patt_mon_cfg(handle->d8_rx_2x_hndl[d8_slice], 
                                   d8_pin, 
                                   patt_mon_cfg_ptr);

    
    PMC_RETURN(result);
}/* sfi51_d8_rx_18x_patt_mon_cfg */


/*******************************************************************************
* sfi51_d8_rx_18x_patt_mon_resync
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function enables pattern monitor check. d8_rx_2x_patt_mon_cfg function shall 
*    be called prior to this function. This function shall be called after
*    device starts receiving pattern. 
*
*
* INPUTS:
*   *handle                - sfi51_d8_rx_18x handle.
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
PUBLIC PMC_ERROR sfi51_d8_rx_18x_patt_mon_resync(sfi51_d8_rx_18x_handle_t *handle,
                                                 UINT8 serdes_id)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 d8_slice;
    UINT8 d8_pin;

    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, D8_RX_2X_LOG_ERR_NULL_HANDLE, 0, 0);

    /* check serdes ID */
    if (FALSE == sfi51_d8_rx_18x_validate_serdes_id(serdes_id, &d8_slice, &d8_pin))
    {
        PMC_RETURN(SFI51_D8_RX_18X_LOG_ERR_SERDES_ID_INVALID);
    }

    result = d8_rx_2x_patt_mon_resync(handle->d8_rx_2x_hndl[d8_slice], 
                                      d8_pin);

    PMC_RETURN(result);
}

/*******************************************************************************
* sfi51_d8_rx_18x_patt_mon_err_count_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function read and returns pattern monitor status and errors counter value.
*
* INPUTS:
*   *handle                 - sfi51_d8_rx_18x handle
*   serdes_id               - serdes identifier
* OUTPUTS:
*   *status                 - Pattern monitor status: Locked, trying to lock, etc.                  
*   *err_count                - Number of errors detected by pattern monitor. 
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
PUBLIC PMC_ERROR sfi51_d8_rx_18x_patt_mon_err_count_get(sfi51_d8_rx_18x_handle_t *handle,
                                                        UINT8 serdes_id,
                                                        util_patt_status_t    *status,
                                                        UINT32 *err_count)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 d8_slice;
    UINT8 d8_pin;

    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(handle != NULL, SFI51_D8_RX_18X_LOG_ERR_NULL_HANDLE, 0, 0);

    /* check serdes ID */
    if (FALSE == sfi51_d8_rx_18x_validate_serdes_id(serdes_id, &d8_slice, &d8_pin))
    {
        PMC_RETURN(SFI51_D8_RX_18X_LOG_ERR_SERDES_ID_INVALID);
    }
    
    result = d8_rx_2x_patt_mon_err_count_get(handle->d8_rx_2x_hndl[d8_slice], 
                                             d8_pin, 
                                             status, err_count);

    PMC_RETURN(PMC_SUCCESS);
}  


/*******************************************************************************
* sfi51_d8_rx_18x_patt_gen_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function configures one serdes SFI51_D8_RX_18X pattern generator: 
*    - It sets pattern mode.
*    - It enables/disables pattern inversion.
*    - It enables/disables error injection. 
*    - It activates/disactivates pattern generator.
*    - It sets pattern mode.
*
*
* INPUTS:
*   *handle                - sfi51_d8_rx_18x handle
*   serdes_id              - serdes identifier
*   *patt_gen_cfg_ptr      - pattern generator configuration
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS            - when configuration is successful 
*                            otherwise a error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sfi51_d8_rx_18x_patt_gen_cfg(sfi51_d8_rx_18x_handle_t *handle,
                                              UINT8 serdes_id,
                                              util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 d8_slice;
    UINT8 d8_pin;

    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(handle != NULL, SFI51_D8_RX_18X_LOG_ERR_NULL_HANDLE, 0, 0);

    /* check serdes ID */
    if (FALSE == sfi51_d8_rx_18x_validate_serdes_id(serdes_id, &d8_slice, &d8_pin))
    {
        PMC_RETURN(SFI51_D8_RX_18X_LOG_ERR_SERDES_ID_INVALID);
    }
    
    result = d8_rx_2x_patt_gen_cfg(handle->d8_rx_2x_hndl[d8_slice], 
                                   d8_pin, 
                                   patt_gen_cfg_ptr);


    PMC_RETURN(result);
}/* sfi51_d8_rx_18x_patt_gen_cfg */

/*******************************************************************************
* sfi51_d8_rx_18x_validate_serdes_id
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function validates serdes id value.
*
* INPUTS:
*   serdes_id                 - serdes identifier.
*
* OUTPUTS:
*  *serdes_slice              - serdes slice.
*   *serdes_pin               - serdes pin.
*
* RETURNS:
*   TRUE           - when serdes id is in range of possible values. 
*   FALSE          - otherwise.
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 sfi51_d8_rx_18x_validate_serdes_id(UINT8  serdes_id,
                                                 UINT8 *serdes_slice,
                                                 UINT8 *serdes_pin)
{

    PMC_ENTRY();

    /* check serdes ID */
    if (SFI51_D8_RX_18X_MAX_NUM_SERDES <= serdes_id)
    {
        PMC_RETURN(FALSE);
    }

    *serdes_slice = SFI51_D8_RX_18X_SERDES_SLICE_GET(serdes_id);
    *serdes_pin = SFI51_D8_RX_18X_SERDES_PIN_GET(serdes_id);

    PMC_RETURN(TRUE);
    
}

/*******************************************************************************
* sfi51_d8_rx_18x_polarity_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function configure a sfi51  instance to enable/disable lane polarity 
*   inversion
*
* INPUTS:
*   *handle         - pointer to sfi51_d8_rx_18x handle
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
PUBLIC PMC_ERROR sfi51_d8_rx_18x_polarity_cfg(sfi51_d8_rx_18x_handle_t *handle,
                                              UINT8                     serdes_id,
                                              BOOL8                     enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 d8_slice;
    UINT8 d8_pin;

    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, SFI51_D8_RX_18X_LOG_ERR_NULL_HANDLE, 0, 0);
   
    if (TRUE == sfi51_d8_rx_18x_validate_serdes_id(serdes_id, &d8_slice, &d8_pin))
    {
        /* configure polarity */
        d8_rx_2x_polarity_cfg(handle->d8_rx_2x_hndl[d8_slice], 
                                   d8_pin, 
                              enable);
    }
    else 
    {
        rc = SFI51_D8_RX_18X_LOG_ERR_SERDES_ID_INVALID;
    }

    PMC_RETURN(rc);

} /* sfi51_d8_rx_18x_polarity_cfg */

/*******************************************************************************
* sfi51_d8_rx_18x_polarity_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function returns sfi51 instance lane polarity status.
*
* INPUTS:
*   *handle         - pointer to sfi51_d8_rx_18x handle
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
PUBLIC PMC_ERROR sfi51_d8_rx_18x_polarity_get(sfi51_d8_rx_18x_handle_t *handle,
                                              UINT8                     serdes_id,
                                              BOOL8                    *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 d8_slice;
    UINT8 d8_pin;

    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, SFI51_D8_RX_18X_LOG_ERR_NULL_HANDLE, 0, 0);
   
    if ( TRUE == sfi51_d8_rx_18x_validate_serdes_id(serdes_id, &d8_slice, &d8_pin))
    {
        /* configure polarity */
        d8_rx_2x_polarity_get(handle->d8_rx_2x_hndl[d8_slice], 
                              d8_pin,
                              enable);
    }
    else 
    {
        rc = SFI51_D8_RX_18X_LOG_ERR_SERDES_ID_INVALID;
    }

    PMC_RETURN(rc);

} /* sfi51_d8_rx_18x_polarity_get */

/*******************************************************************************
* sfi51_d8_rx_18x_txdclk_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function to enable the TXDCLK on an sfi51 instance.
*
* INPUTS:
*   *handle         - pointer to sfi51_d8_rx_18x handle
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
PUBLIC PMC_ERROR sfi51_d8_rx_18x_txdclk_enable(sfi51_d8_rx_18x_handle_t *handle)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    
    rc = sfi51_d8_rx_18x_txdclk_cfg(handle, TRUE);

    PMC_RETURN(rc);

} /* sfi51_d8_rx_18x_txdclk_enable */

/*******************************************************************************
* sfi51_d8_rx_18x_txdclk_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function to disable the TXDCLK on an sfi51 instance.
*
* INPUTS:
*   *handle         - pointer to sfi51_d8_rx_18x handle
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
PUBLIC PMC_ERROR sfi51_d8_rx_18x_txdclk_disable(sfi51_d8_rx_18x_handle_t *handle)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    rc = sfi51_d8_rx_18x_txdclk_cfg(handle, FALSE);

    PMC_RETURN(rc);

} /* sfi51_d8_rx_18x_txdclk_disable */

/*******************************************************************************
* sfi51_d8_rx_18x_txdclk_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function to enable or disable the TXDCLK on an sfi51 instance.
*
* INPUTS:
*   *handle         - pointer to sfi51_d8_rx_18x handle
*   enable          - TRUE to enable the TXDCLK , FALSE to disable it
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
PRIVATE PMC_ERROR sfi51_d8_rx_18x_txdclk_cfg(sfi51_d8_rx_18x_handle_t *handle,
                                             BOOL8                     enable)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, SFI51_D8_RX_18X_LOG_ERR_NULL_HANDLE, 0, 0);
   
    /* configure polarity */
    sfi51_rx_18x_serdes_glue_field_TXDCK_SLICE_EN_set(NULL, handle, enable);

    PMC_RETURN(rc);

} /* sfi51_d8_rx_18x_txdclk_cfg */



/*******************************************************************************
* FUNCTION: sfi51_d8_rx_18x_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   sfi51_d8_rx_18x_handle      - Pointer to subsystem handle
*   serdes_id                   - Serdes identifier
*   non_null_ptr                - Pointer to be checked that can't be NULL
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sfi51_d8_rx_18x_int_validate(sfi51_d8_rx_18x_handle_t  *sfi51_d8_rx_18x_handle,
                                              UINT32                     serdes_id,
                                              void                      *non_null_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != sfi51_d8_rx_18x_handle, SFI51_D8_RX_18X_LOG_ERR_NULL_HANDLE, 0, 0);
   
    if (serdes_id >= SFI51_D8_RX_18X_MAX_NUM_SERDES)
    {
        result = SFI51_D8_RX_18X_LOG_ERR_INVALID_PARAMETERS;
    }

    if (NULL == non_null_ptr)
    {
        result = SFI51_D8_RX_18X_LOG_ERR_INVALID_PARAMETERS;
    }


    PMC_RETURN(result);
} /* sfi51_d8_rx_18x_int_validate */

/*******************************************************************************
* sfi51_d8_rx_18x_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on sfi51_d8_rx_18x handle instance.
*
* INPUTS:
*   *sfi51_d8_rx_18x_handle             - pointer to sfi51_d8_rx_18x handle instance
*   *restart_init_cfg_ptr   - pointer to restart init config
*    sfi51_d8_rx_18x_reset_msk          - SFI51_D8_RX_18X serdes reset bits mask 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS : 
*     WARM : register/context are coherent
*     CRASH: everything is coherent (a cleanup may has been executed)
*   SFI51_D8_RX_18X_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT:
*     incoherences have been detected on communal energy configuration
*     that is incoherent with top energy state
*      WARM : ERROR
*      CRASH: TOP shall put SFI51_D8_RX_18X in RESET mode
*   
subsystem shall be put in reset mode 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sfi51_d8_rx_18x_handle_restart_init(sfi51_d8_rx_18x_handle_t       *sfi51_d8_rx_18x_handle,
                                                     util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                                     BOOL8                           sfi51_d8_rx_18x_reset_msk)
{
    PMC_ERROR           rc = PMC_SUCCESS;
    sfi51_d8_rx_18x_var_t           *sfi51_d8_rx_18x_var_clean;
    
    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(SFI51_D8_RX_18X_LOG_ERR_STRINGS[0], SFI51_D8_RX_18X_LOG_ERR_TABLE_BASE, SFI51_D8_RX_18X_LOG_ERR_COUNT);
    }

    sfi51_d8_rx_18x_var_clean =  (sfi51_d8_rx_18x_var_t *) PMC_CALLOC(sizeof(sfi51_d8_rx_18x_var_t)); 

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
     *   - Create a new context that represents the intersection between memory and register space (sfi51_d8_rx_18x_var_clean)
     */
    rc = sfi51_d8_rx_18x_ctxt_audit(sfi51_d8_rx_18x_handle,
                                    sfi51_d8_rx_18x_var_clean,
                                    sfi51_d8_rx_18x_reset_msk); 
    
    PMC_LOG_TRACE("sfi51_d8_rx_18x_ctxt_audit rc = %s\n", pmc_log_string_get(rc));
    if (SFI51_D8_RX_18X_ERR_CONTEXT_CLEANUP == rc && restart_init_cfg_ptr->is_warm_restart == FALSE)
    {
        /* 
         * Cleanup 
         *   - At the end, sfi51_d8_rx_18x_var_clean is the new context
         *    - We keep OPERATIONAL channel defined in sfi51_d8_rx_18x_var_clean
         *    - We clean (deactivate/deprov) all channels that are not OPERATIONAL
         *       - they may have be partially provisionned
         *    - We ajust power setting based on sfi51_d8_rx_18x_var_clean
         */                                    
        rc = sfi51_d8_rx_18x_ctxt_cleanup(sfi51_d8_rx_18x_handle,
                                          sfi51_d8_rx_18x_var_clean); 
    }

    PMC_FREE(&sfi51_d8_rx_18x_var_clean);

    PMC_RETURN(rc);
} /* sfi51_d8_rx_18x_handle_restart_init */

/*******************************************************************************
* sfi51_d8_rx_18x_ctxt_audit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function audits SFI51_D8_RX_18X handle context variable against register space content.
*
* INPUTS:
*   *sfi51_d8_rx_18x_handle           - pointer to SFI51_D8_RX_18X handle instance.
*   *sfi51_d8_rx_18x_var_clean        - pointer to SFI51_D8_RX_18X lanes context.
*    reset                            - SFI51_D8_RX_18X serdes reset state.
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
PRIVATE PMC_ERROR sfi51_d8_rx_18x_ctxt_audit(sfi51_d8_rx_18x_handle_t *sfi51_d8_rx_18x_handle,
                                             sfi51_d8_rx_18x_var_t    *sfi51_d8_rx_18x_var_clean,
                                             BOOL8                     reset)
{
    BOOL8 serdes_ctxt_match;
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL8 lpbk_enabled = FALSE;

    PMC_ENTRY();

    PMC_ASSERT(sfi51_d8_rx_18x_handle != NULL, SFI51_D8_RX_18X_LOG_ERR_INVALID_PARAMETERS,0,0);
    
    serdes_ctxt_match = TRUE;

    if (FALSE == reset)
    {
        /* check sfi51_d8_rx_18x provisioning & activation status */
        if (SFI51_D8_RX_18X_RESET == sfi51_d8_rx_18x_handle->var.state ||
            LAST_SFI51_D8_RX_18X_STATE == sfi51_d8_rx_18x_handle->var.state)
        {
            serdes_ctxt_match = FALSE;
        }
        
        /* audit acb_ft activation && prov status if necessary */
        if (TRUE == serdes_ctxt_match)
        {
            rc = sfi51_d8_rx_18x_loopback_cfg_get(sfi51_d8_rx_18x_handle, &lpbk_enabled);
            if (PMC_SUCCESS != rc)
            {
                serdes_ctxt_match = FALSE;
            }
            else
            {
                if (lpbk_enabled != sfi51_d8_rx_18x_handle->var.loopback_enable)
                {
                    serdes_ctxt_match = FALSE;                        
                }
            }
        }
    }
    else
    {
        /* check sfi51_d8_rx_18x provisioning & activation status */
        if (SFI51_D8_RX_18X_RESET != sfi51_d8_rx_18x_handle->var.state &&
            LAST_SFI51_D8_RX_18X_STATE != sfi51_d8_rx_18x_handle->var.state)
        {
            serdes_ctxt_match = FALSE;
        }
    }
    /* update returned error code anf var clean (used to cleanup the context) */
    if (FALSE == serdes_ctxt_match)
    {
        if (PMC_SUCCESS == rc)
        {
            rc = SFI51_D8_RX_18X_ERR_CONTEXT_CLEANUP;
        }
        sfi51_d8_rx_18x_var_default_init(sfi51_d8_rx_18x_var_clean);
    }   
    else
    { 
        if (FALSE == (SFI51_D8_RX_18X_RESET != sfi51_d8_rx_18x_handle->var.state &&
                      LAST_SFI51_D8_RX_18X_STATE != sfi51_d8_rx_18x_handle->var.state))
        {
            PMC_MEMCPY((void*)sfi51_d8_rx_18x_var_clean, (void*)&sfi51_d8_rx_18x_handle->var, sizeof(sfi51_d8_rx_18x_var_t));
        }
    } 

    PMC_RETURN(rc);
} /* sfi51_d8_rx_18x_ctxt_audit */

/*******************************************************************************
* sfi51_d8_rx_18x_ctxt_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function cleans up SFI51_D8_RX_18X handle context. 
*
* INPUTS:
*   *sfi51_d8_rx_18x_handle           - pointer to SFI51_D8_RX_18X handle instance.
*   *sfi51_d8_rx_18x_var_clean        - pointer to SFI51_D8_RX_18X lanes clean context.
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
PRIVATE PMC_ERROR sfi51_d8_rx_18x_ctxt_cleanup(sfi51_d8_rx_18x_handle_t  *sfi51_d8_rx_18x_handle,
                                               sfi51_d8_rx_18x_var_t     *sfi51_d8_rx_18x_var_clean)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(sfi51_d8_rx_18x_handle != NULL, SFI51_D8_RX_18X_LOG_ERR_INVALID_PARAMETERS,0,0);
    
    if (SFI51_D8_RX_18X_RESET == sfi51_d8_rx_18x_var_clean->state)
    {
        sfi51_d8_rx_18x_cleanup(sfi51_d8_rx_18x_handle);
    }

    PMC_RETURN(rc);
} /* sfi51_d8_rx_18x_ctxt_cleanup */

/*******************************************************************************
* sfi51_d8_rx_18x_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function cleanup SFI51_D8_RX_18X serdes lane context.
*
* INPUTS:
*   *sfi51_d8_rx_18x_handle           - pointer to SFI51_D8_RX_18X handle instance.
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
PRIVATE void sfi51_d8_rx_18x_cleanup(sfi51_d8_rx_18x_handle_t *sfi51_d8_rx_18x_handle) 
{
    UINT32 itr;
    PMC_ENTRY();

    PMC_ASSERT(sfi51_d8_rx_18x_handle != NULL, SFI51_D8_RX_18X_LOG_ERR_INVALID_PARAMETERS,0,0);

    /* update state */
    /* cleanup d8_rx_2x slices */
    for (itr = 0; itr < (SFI51_D8_RX_NUM_SLICES); itr += 1)
    {
        d8_rx_2x_ctxt_cleanup(sfi51_d8_rx_18x_handle->d8_rx_2x_hndl[itr]);
    }

    sfi51_d8_rx_18x_var_default_init(&sfi51_d8_rx_18x_handle->var);

    PMC_RETURN();
} /* sfi51_d8_rx_18x_cleanup */

/*******************************************************************************
* sfi51_d8_rx_18x_loopback_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns SFI51_D8_RX_18X instance configured loopback mode.
*
* INPUTS:
*   *handle         - pointer to sfi51_d8_rx_18x handle.
*
* OUTPUTS:
*   lpbk_enabled_ptr - loopback activation status.
*
* RETURNS:
*  PMC_SUCCESS if loopback status is retrieved properly otherwie a descriptive
*  error code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sfi51_d8_rx_18x_loopback_cfg_get(sfi51_d8_rx_18x_handle_t *handle, 
                                                   BOOL8                    *lpbk_enabled_ptr) 
{
    UINT32 itr;
    UINT32 jtr;
    BOOL8 lpbk_enabled = FALSE;
    BOOL8 tmp;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    for (itr = 0; itr <(SFI51_D8_RX_NUM_SLICES) && PMC_SUCCESS == rc; itr += 1)
    {
        for (jtr = 0; jtr < D8_RX_2X_MAX_NUM_SERDES; jtr += 1)
        {
            tmp = d8_rx_2x_loopback_activate_status_get(handle->d8_rx_2x_hndl[itr], jtr);

            if (0 == itr && 0 == jtr)
            {
                lpbk_enabled = tmp;
            }

            if (lpbk_enabled != tmp)
            {
                rc = SFI51_D8_RX_18X_ERR_CONTEXT_REG_INCOHERENT;
            }
        }
    }
    
    if (PMC_SUCCESS == rc)
    {
        *lpbk_enabled_ptr = lpbk_enabled;
    }

    PMC_RETURN(rc);
}/*sfi51_d8_rx_18x_loopback_cfg_get*/

/*******************************************************************************
* sfi51_d8_rx_18x_ext_ctxt_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set sfi51_d8_rx_18x lanes state in reset or start mode.
*
* INPUTS:
*   *sfi51_d8_rx_18x_handle    - pointer to SFI51_D8_RX_18X handle instance.
*   out_of_reset               - When TRUE state of SFI51_D8_RX_18X lane is set to START mode.
*                                when FALSE state is set back to RESET mode.
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
PUBLIC PMC_ERROR sfi51_d8_rx_18x_ext_ctxt_state_set(sfi51_d8_rx_18x_handle_t      *sfi51_d8_rx_18x_handle, 
                                                    BOOL8                          out_of_reset) 
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();    

    PMC_ASSERT(sfi51_d8_rx_18x_handle != NULL, SFI51_D8_RX_18X_LOG_ERR_INVALID_PARAMETERS,0,0);

    /* update state */
    if (TRUE == out_of_reset)
    {
        /* this function should be call only once and before loopback activation*/
        if (SFI51_D8_RX_18X_RESET == sfi51_d8_rx_18x_handle->var.state)
        {
            sfi51_d8_rx_18x_handle->var.state = SFI51_D8_RX_18X_START;
        }
        else
        {
            rc = SFI51_D8_RX_18X_STATE_NOT_RESET;
        }
    }
    else
    {
        sfi51_d8_rx_18x_handle->var.state = SFI51_D8_RX_18X_RESET;
        sfi51_d8_rx_18x_cleanup(sfi51_d8_rx_18x_handle);
    }
        
    PMC_RETURN(rc);
}/*sfi51_d8_rx_18x_ext_ctxt_state_set*/

/*******************************************************************************
* sfi51_d8_rx_18x_reset_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns SFI51_D8_RX_18X instance reset status based on 
*    context content.
*
* INPUTS:
*   *handle         - pointer to sfi51_d8_rx_18x handle.
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
PUBLIC BOOL8 sfi51_d8_rx_18x_reset_state_get(sfi51_d8_rx_18x_handle_t *handle) 
{
    BOOL8 rc;

    PMC_ENTRY();

    rc = (SFI51_D8_RX_18X_RESET == handle->var.state) ? TRUE : FALSE;
    
    PMC_RETURN(rc);
}/*sfi51_d8_rx_18x_reset_state_get*/

/*
** End of file
*/



