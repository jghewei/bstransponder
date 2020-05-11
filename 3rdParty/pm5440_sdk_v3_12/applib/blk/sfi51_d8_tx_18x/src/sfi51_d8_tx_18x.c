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
*       Main SFI51_D8_TX_18X source file.
* 
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "sfi51_d8_tx_18x_loc.h"
#include "digi120_sfi51_tx_18x_glue_map.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/* pattern monitor bus width default value */
#define  SFI51_D8_TX_18X_GEN_BUS_WIDTH_VAL 8
#define SFI51_D8_TX_18X_CSU_DIV_COMPUTE(r, n, d)        \
    (DOUBLE) r + ( (DOUBLE) n / (DOUBLE) d );    
#define SFI51_D8_TX_18X_REDCLK_SRC_1_2_CSU_FRAC_DIV 0   
#define SFI51_D8_TX_18X_REDCLK_SRC_1_2_CSU_DENOMIN 0xffffffff18ULL
#define SFI51_D8_TX_18X_REDCLK_SRC_1_2_CSU_INT_DIV 0x30

#define SFI51_D8_TX_18X_SERDES_SLICE_GET(serdes_id) (serdes_id / D8_TX_2X_MAX_NUM_SERDES)
#define SFI51_D8_TX_18X_SERDES_PIN_GET(serdes_id) (serdes_id % D8_TX_2X_MAX_NUM_SERDES)

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
const char SFI51_D8_TX_18X_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    SFI51_D8_TX_18X_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char SFI51_D8_TX_18X_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward References
*/
PRIVATE void sfi51_d8_tx_18x_var_default_init(sfi51_d8_tx_18x_var_t *var_ptr);

PRIVATE void sfi51_d8_tx_18x_csu_div_val_set(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle, 
                                             UINT64 csu_n, 
                                             UINT64 csu_denomin, 
                                             UINT32 csu_r);

PRIVATE void sfi51_d8_tx_18x_csu_acb_reset(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle,
                                           BOOL8 reset, 
                                           BOOL8 loop_timing);

PRIVATE BOOL8 sfi51_d8_tx_18x_validate_serdes_id(UINT8  serdes_id,
                                                 UINT8 *serdes_slice,
                                                 UINT8 *serdes_pin);

PRIVATE PMC_ERROR sfi51_d8_tx_18x_acb_ft_loop_timing_disable(sfi51_d8_tx_18x_handle_t *handle);

PRIVATE PMC_ERROR sfi51_d8_tx_18x_acb_ft_loop_timing_enable(sfi51_d8_tx_18x_handle_t *handle, 
                                                            UINT32 pkt_period,
                                                            UINT32 pkt_size);

PRIVATE PMC_ERROR sfi51_d8_tx_18x_ctxt_audit(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle,
                                             sfi51_d8_tx_18x_var_t    *sfi51_d8_tx_18x_var_clean,
                                             BOOL8                     serdes_reset);

PRIVATE PMC_ERROR sfi51_d8_tx_18x_ctxt_cleanup(sfi51_d8_tx_18x_handle_t  *sfi51_d8_tx_18x_handle,
                                               sfi51_d8_tx_18x_var_t     *sfi51_d8_tx_18x_var_clean);

PRIVATE void sfi51_d8_tx_18x_cleanup(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle);

PRIVATE PMC_ERROR sfi51_d8_tx_18x_loopback_cfg_get(sfi51_d8_tx_18x_handle_t *handle, 
                                                   BOOL8                    *lpbk_enabled_ptr);

PUBLIC PMC_ERROR sfi51_d8_tx_18x_loopback_int_activate(sfi51_d8_tx_18x_handle_t *handle);

/*
** Public Functions
*/
/*******************************************************************************
* sfi51_d8_tx_18x_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a SFI51_D8_TX_18X block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the SFI51_D8_TX_18X block relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - pointer to TSB name
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   sfi51_d8_tx_18x_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_ctxt_create(pmc_handle_t *parent, 
                                                             UINT32 base_address, 
                                                             pmc_sys_handle_t *sys_handle, 
                                                             const char *tsb_name )
{
    sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle;
    UINT32 i;
    const char* SFI51_ACB_FT_HANDLE_NAME = "ACB_FT";
    const char* D8_TX_2X_HANDLE_NAME[] = {"D8_TX_2X_0", "D8_TX_2X_1",   \
                                          "D8_TX_2X_2", "D8_TX_2X_3",   \
                                          "D8_TX_2X_4", "D8_TX_2X_5",   \
                                          "D8_TX_2X_6", "D8_TX_2X_7",   \
                                          "D8_TX_2X_8"};
    
    PMC_ENTRY();    
    
    sfi51_d8_tx_18x_handle = (sfi51_d8_tx_18x_handle_t*)PMC_CTXT_CALLOC(sizeof(sfi51_d8_tx_18x_handle_t), parent);
    
    pmc_handle_init( parent, sfi51_d8_tx_18x_handle, sys_handle, PMC_MID_DIGI_SFI51_18X_TX_SERDES_GLUE, tsb_name, base_address );

    sfi51_d8_tx_18x_handle->d8_tx_2x_hndl = (d8_tx_2x_handle_t **)PMC_CTXT_CALLOC(sizeof(d8_tx_2x_handle_t *) * SFI51_D8_TX_NUM_SLICES, parent);
    
    PMC_CTXT_REGISTER_PTR(&sfi51_d8_tx_18x_handle->d8_tx_2x_hndl, parent);
    for (i = 0; i < SFI51_D8_TX_NUM_SLICES; i++)
    {
        sfi51_d8_tx_18x_handle->d8_tx_2x_hndl[i] = d8_tx_2x_ctxt_create((pmc_handle_t *) sfi51_d8_tx_18x_handle, 
                                                                        BASE_ADDR_DIGI120_SFI51_TX_18X_GLUE_SFI5_1_TX_2X_SERDES_SLICE_SFI51_TX_2X_SLICE(i) + base_address, 
                                                                        sys_handle, D8_TX_2X_HANDLE_NAME[i],
                                                                        SFI51_D8_TX_18X_GEN_BUS_WIDTH_VAL);
        PMC_CTXT_REGISTER_PTR(&sfi51_d8_tx_18x_handle->d8_tx_2x_hndl[i], parent);
    }

    sfi51_d8_tx_18x_handle->acb_ft_handle = acb_ft_ctxt_create(
        (pmc_handle_t *) sfi51_d8_tx_18x_handle, 
        base_address+BASE_ADDR_DIGI120_SFI51_TX_18X_GLUE_CNI_UNI_1G_CSU_PM64_82_12_A_BRIDGE_ACB_FT_INST_ACB_FT,
        sys_handle ,
        SFI51_ACB_FT_HANDLE_NAME);
    PMC_CTXT_REGISTER_PTR(&sfi51_d8_tx_18x_handle->acb_ft_handle, parent);
    /* Register log strings. */
    pmc_log_block_strings_register(SFI51_D8_TX_18X_LOG_ERR_STRINGS[0], SFI51_D8_TX_18X_LOG_ERR_TABLE_BASE, SFI51_D8_TX_18X_LOG_ERR_COUNT);

    PMC_RETURN(sfi51_d8_tx_18x_handle);
} /* sfi51_d8_tx_18x_ctxt_create */  
 
/*******************************************************************************
* sfi51_d8_tx_18x_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a SFI51_D8_TX_18X block instance.
*
*
* INPUTS:
*   *sfi51_d8_tx_18x_handle        - pointer to SFI51_D8_TX_18X handle instance
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
PUBLIC void sfi51_d8_tx_18x_ctxt_destroy(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle)
{
    UINT32 i;

    PMC_ENTRY();
    
    for (i = 0; i < SFI51_D8_TX_NUM_SLICES; i++)
    {
        d8_tx_2x_ctxt_destroy(sfi51_d8_tx_18x_handle->d8_tx_2x_hndl[i]);
    }
    
    acb_ft_ctxt_destroy(sfi51_d8_tx_18x_handle->acb_ft_handle);
    PMC_CTXT_FREE(&sfi51_d8_tx_18x_handle->d8_tx_2x_hndl, sfi51_d8_tx_18x_handle);
    PMC_CTXT_FREE(&sfi51_d8_tx_18x_handle, sfi51_d8_tx_18x_handle);

    PMC_RETURN();
} /* sfi51_d8_tx_18x_ctxt_destroy */

/*******************************************************************************
* sfi51_d8_tx_18x_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a SFI51_D8_TX_18X block handle instance.
*
*
* INPUTS:
*   *sfi51_d8_tx_18x_handle        - pointer to SFI51_D8_TX_18X handle instance
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
PUBLIC void sfi51_d8_tx_18x_handle_init(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle)
{
    UINT32 i;
    PMC_ENTRY();
    
    sfi51_d8_tx_18x_var_default_init(&sfi51_d8_tx_18x_handle->var);
    acb_ft_handle_init(sfi51_d8_tx_18x_handle->acb_ft_handle,ACB_FT_SFI51);       
    for (i = 0; i < SFI51_D8_TX_NUM_SLICES; i++)
    {
        d8_tx_2x_handle_init(sfi51_d8_tx_18x_handle->d8_tx_2x_hndl[i]);
    }

    PMC_RETURN();
} /* sfi51_d8_tx_18x_handle_init */

/*******************************************************************************
* sfi51_d8_tx_18x_ext_ctxt_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set sfi51_d8_tx_18x lanes state in reset or start mode.
*
* INPUTS:
*   *sfi51_d8_tx_18x_handle    - pointer to SFI51_D8_TX_18X handle instance.
*   out_of_reset               - When TRUE state of SFI51_D8_TX_18X lane is set to START mode.
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
PUBLIC PMC_ERROR sfi51_d8_tx_18x_ext_ctxt_state_set(sfi51_d8_tx_18x_handle_t      *sfi51_d8_tx_18x_handle, 
                                                    BOOL8                          out_of_reset) 
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();    

    PMC_ASSERT(sfi51_d8_tx_18x_handle != NULL, SFI51_D8_TX_18X_LOG_ERR_INVALID_PARAMETERS,0,0);

    /* update state */
    if (TRUE == out_of_reset)
    {
        /* this function should be call only once and before loopback activation*/
        if (SFI51_D8_TX_18X_RESET == sfi51_d8_tx_18x_handle->var.state)
        {
            sfi51_d8_tx_18x_handle->var.state = SFI51_D8_TX_18X_START;
        }
        else
        {
            rc = SFI51_D8_TX_18X_STATE_NOT_RESET;
        }
    }
    else
    {
        sfi51_d8_tx_18x_handle->var.state = SFI51_D8_TX_18X_RESET;
        sfi51_d8_tx_18x_cleanup(sfi51_d8_tx_18x_handle);
    }
        
    PMC_RETURN(rc);
}/*sfi51_d8_tx_18x_ext_ctxt_state_set*/

/*******************************************************************************
* sfi51_d8_tx_18x_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes the var context to its default values
*
* INPUTS:
*   *var_ptr          - pointer to sfi51_d8_tx_18x variable instance.
*
* OUTPUTS:
*   None
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void sfi51_d8_tx_18x_var_default_init(sfi51_d8_tx_18x_var_t *var_ptr)
{

    PMC_ENTRY();

    var_ptr->state = SFI51_D8_TX_18X_RESET;    
    var_ptr->use_acb = FALSE;
    var_ptr->loopback_enable = FALSE;
    var_ptr->has_lpbk_acb_ft_backup = FALSE;
    var_ptr->has_loop_timing_enabled = FALSE;
    var_ptr->has_force_nominal_acb_ft_backup = FALSE;


    PMC_RETURN();
} /*sfi51_d8_tx_18x_var_default_init  */
 

/*******************************************************************************
* sfi51_d8_tx_18x_loopback_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function activate loopback (TX2RX/Diagnostic Loopback).
*
* INPUTS:
*   *handle         - pointer to sfi51_d8_tx_18x handle.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS      - when API execution is successful otherwise a 
*                      descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sfi51_d8_tx_18x_loopback_activate(sfi51_d8_tx_18x_handle_t *handle)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, SFI51_D8_TX_18X_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(handle->var.loopback_enable == FALSE, SFI51_D8_TX_18X_LOOPBACK_ALREADY_CONFIGURED, 0, 0);
    
    /* configure the loopback */
    rc = sfi51_d8_tx_18x_loopback_int_activate(handle);

    if (PMC_SUCCESS == rc) 
    {        
        handle->var.loopback_enable = TRUE;
    }
    PMC_RETURN(rc);
} /* sfi51_d8_tx_18x_loopback_activate */

/*******************************************************************************
* sfi51_d8_tx_18x_loopback_int_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function activate loopback (TX2RX/Diagnostic Loopback).
*
* INPUTS:
*   *handle         - pointer to sfi51_d8_tx_18x handle.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS      - when API execution is successful otherwise a 
*                      descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sfi51_d8_tx_18x_loopback_int_activate(sfi51_d8_tx_18x_handle_t *handle)
{
    UINT8 i;
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, SFI51_D8_TX_18X_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes state */
    if (SFI51_D8_TX_18X_RESET == handle->var.state) 
    {
        rc = SFI51_D8_TX_18X_STATE_IN_RESET;
    }

    /* configure the loopback */
    for (i = 0; i < SFI51_D8_TX_NUM_SLICES && PMC_SUCCESS == rc; i++)
    {
        rc = d8_tx_2x_loopback_activate(handle->d8_tx_2x_hndl[i], 0);
        if (PMC_SUCCESS == rc)
        {
            rc = d8_tx_2x_loopback_activate(handle->d8_tx_2x_hndl[i], 1);
        }
    }

    PMC_RETURN(rc);
} /* sfi51_d8_tx_18x_loopback_int_activate */

/*******************************************************************************
* sfi51_d8_tx_18x_loopback_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function deactivate loopback (TX2RX/Disgnostic)
*
* INPUTS:
*   *handle         - pointer to sfi51_d8_tx_18x handle.
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
PUBLIC PMC_ERROR sfi51_d8_tx_18x_loopback_deactivate(sfi51_d8_tx_18x_handle_t *handle)
{
    UINT8 i;
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, SFI51_D8_TX_18X_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(handle->var.loopback_enable == TRUE, SFI51_D8_TX_18X_LOOPBACK_ALREADY_NOT_CONFIGURED, 0, 0);
    
    /* check serdes state */
    if (SFI51_D8_TX_18X_RESET == handle->var.state) 
    {
        rc = SFI51_D8_TX_18X_STATE_IN_RESET;
    }

    /* configure the loopback */
    for (i = 0; i < SFI51_D8_TX_NUM_SLICES && PMC_SUCCESS == rc; i++)
    {
        rc = d8_tx_2x_loopback_deactivate(handle->d8_tx_2x_hndl[i], 0 );
        rc = d8_tx_2x_loopback_deactivate(handle->d8_tx_2x_hndl[i], 1);
    }

    if (PMC_SUCCESS == rc) 
    {        
        handle->var.loopback_enable = FALSE;
    }
    

    PMC_RETURN(rc);
} /* sfi51_d8_tx_18x_loopback_deactivate */

/*******************************************************************************
* sfi51_d8_tx_18x_patt_mon_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function configures one serdes SFI51_D8_TX_18X pattern monitor: 
*    - It sets pattern mode,
*    - It  enables/disables pattern inversion check. 
*    - It activates/disactivates pattern monitor.
*
*
* INPUTS:
*   *handle                - sfi51_d8_tx_18x handle
*   serdes_id              - serdes identifier
*   *patt_mon_cfg_ptr      - pattern monitor configuration
*
* OUTPUTS:
*  NONE.
*
* RETURNS:
*   PMC_SUCCESS            - when conversion is successful 
*                            otherwise a error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sfi51_d8_tx_18x_patt_mon_cfg(sfi51_d8_tx_18x_handle_t *handle,
                                              UINT8 serdes_id,
                                              util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr)
{

    PMC_ERROR result = PMC_SUCCESS;
    UINT8 d8_slice;
    UINT8 d8_pin;

    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(handle != NULL, SFI51_D8_TX_18X_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes ID */
    if (FALSE == sfi51_d8_tx_18x_validate_serdes_id(serdes_id, &d8_slice, &d8_pin))
    {
        PMC_RETURN(SFI51_D8_TX_18X_LOG_ERR_SERDES_ID_INVALID);
    }
    
    result = d8_tx_2x_patt_mon_cfg(handle->d8_tx_2x_hndl[d8_slice], 
                                   d8_pin,
                                   patt_mon_cfg_ptr);

    
    PMC_RETURN(result);
}/*sfi51_d8_tx_18x_patt_mon_cfg*/


/*******************************************************************************
* sfi51_d8_tx_18x_patt_gen_err_insert_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function enables or disables error insertion in generated pattern.
*
* INPUTS:
*   *handle          - pointer to sfi51_d8_tx_18x handle.
*   serdes_id        - serdes identifier.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS       - when API execution is successful otherwise a 
*                       descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sfi51_d8_tx_18x_patt_gen_err_insert_cfg(sfi51_d8_tx_18x_handle_t *handle,
                                                         UINT8 serdes_id)
 
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 d8_slice;
    UINT8 d8_pin;


    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(handle != NULL, SFI51_D8_TX_18X_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes ID */
    if (FALSE == sfi51_d8_tx_18x_validate_serdes_id(serdes_id, &d8_slice, &d8_pin))
    {
        PMC_RETURN(SFI51_D8_TX_18X_LOG_ERR_SERDES_ID_INVALID);
    }
    
    result = d8_tx_2x_patt_gen_err_insert_cfg(handle->d8_tx_2x_hndl[d8_slice], 
                                              d8_pin);

    
    PMC_RETURN(result);
}

/*******************************************************************************
* sfi51_d8_tx_18x_patt_gen_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function configures one serdes SFI51_D8_TX_18X pattern generator: 
*    - It sets pattern mode.
*    - It enables/disables pattern inversion.
*    - It enables/disables error injection. 
*    - It activates/disactivates pattern generator.
*    - It sets pattern mode.
*
*
* INPUTS:
*   *handle                - pointer to sfi51_d8_tx_18x handle
*   serdes_id              - serdes identifier
*   *patt_gen_cfg_ptr      - pattern generator configuration
*
* OUTPUTS:
*  NONE.
*
* RETURNS:
*   PMC_SUCCESS            - when configuration is successful 
*                            otherwise a error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sfi51_d8_tx_18x_patt_gen_cfg(sfi51_d8_tx_18x_handle_t *handle,
                                              UINT8 serdes_id,
                                              util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 d8_slice;
    UINT8 d8_pin;

    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(handle != NULL, SFI51_D8_TX_18X_LOG_ERR_NULL_HANDLE, 0, 0);

    /* check serdes ID */
    if (FALSE == sfi51_d8_tx_18x_validate_serdes_id(serdes_id, &d8_slice, &d8_pin))
    {
        PMC_RETURN(SFI51_D8_TX_18X_LOG_ERR_SERDES_ID_INVALID);
    }
    
    result = d8_tx_2x_patt_gen_cfg(handle->d8_tx_2x_hndl[d8_slice], 
                                   d8_pin,
                                   patt_gen_cfg_ptr);


    PMC_RETURN(result);
}/*sfi51_d8_tx_18x_patt_gen_cfg*/

/*******************************************************************************
* sfi51_d8_tx_18x_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provision a sfi51_d8_tx_18x instance.sfi51_d8_tx_18x is mainly
*   provision by FW.  This function handle ACB, and 
*   PMM configuration in loopback (RX to TX) configuration.
*
* INPUTS:
*   *sfi51_d8_tx_18x_handle           - pointer to sfi51_d8_tx_18x handle instance.
*   *acb_cfg_ptr                      - pointer to ACB_FT provisioning configuration
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
PUBLIC PMC_ERROR sfi51_d8_tx_18x_prov(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle, 
                                      acb_ft_prov_cfg_t *acb_cfg_ptr) 
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT64 csu_n;
    UINT32 csu_r;

    PMC_ENTRY();

    PMC_ASSERT(sfi51_d8_tx_18x_handle != NULL, SFI51_D8_TX_18X_LOG_ERR_INVALID_PARAMETERS,0,0);

    if (SFI51_D8_TX_18X_START != sfi51_d8_tx_18x_handle->var.state) 
    {
        PMC_RETURN(SFI51_D8_TX_18X_STATE_NOT_START);
    }
    
    /* Determine if we need the to configure the ACB_FT */
    if (NULL == acb_cfg_ptr) 
    {
        sfi51_d8_tx_18x_handle->var.use_acb = FALSE;
    } else if (ACB_FT_MODE_NOMINAL == acb_cfg_ptr->mode) 
    {
        sfi51_d8_tx_18x_handle->var.use_acb = FALSE;
    } else 
    {
        sfi51_d8_tx_18x_handle->var.use_acb = TRUE;
    }
    
    if (TRUE == sfi51_d8_tx_18x_handle->var.use_acb) 
    {

        /* extract required ACB_FT configuration field from CSU */
        sfi51_d8_tx_18x_csu_div_val_get(sfi51_d8_tx_18x_handle,&acb_cfg_ptr->csu_div, &csu_n, &acb_cfg_ptr->csu_denomin, (UINT32 *) &csu_r);
        
        /* Release reset to ACB_FT prior to use it */
        sfi51_d8_tx_18x_csu_acb_reset(sfi51_d8_tx_18x_handle, FALSE,(ACB_FT_MODE_LOOP_TIMING == acb_cfg_ptr->mode));
        
        /* provision ACB_FT block */
        rc = acb_ft_prov(sfi51_d8_tx_18x_handle->acb_ft_handle, acb_cfg_ptr);
        
        if (PMC_SUCCESS == rc) 
        {       
            acb_ft_pll_s_max_cfg(sfi51_d8_tx_18x_handle->acb_ft_handle, acb_cfg_ptr->max_ppm, csu_n, acb_cfg_ptr->csu_denomin, csu_r);
        }
    }
    /* Flag to indicate whether loop timing is enabled */
    sfi51_d8_tx_18x_handle->var.has_loop_timing_enabled = (sfi51_d8_tx_18x_handle->var.use_acb && (acb_cfg_ptr->mode == ACB_FT_MODE_LOOP_TIMING));
    
    
    if (PMC_SUCCESS == rc) 
    {
        /* update state */
        sfi51_d8_tx_18x_handle->var.state = SFI51_D8_TX_18X_EQUIPPED;
    }

    PMC_RETURN(rc);
} /* sfi51_d8_tx_18x_prov */

/*******************************************************************************
* sfi51_d8_tx_18x_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function deprovision a sfi51_d8_tx_18x  instance.
*
* INPUTS:
*   *sfi51_d8_tx_18x_handle           - pointer to sfi51_d8_tx_18x handle instance.
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
PUBLIC PMC_ERROR sfi51_d8_tx_18x_deprov(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle) 
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(sfi51_d8_tx_18x_handle != NULL, SFI51_D8_TX_18X_LOG_ERR_INVALID_PARAMETERS,0,0);
    if (SFI51_D8_TX_18X_EQUIPPED != sfi51_d8_tx_18x_handle->var.state) 
    {
        PMC_RETURN(SFI51_D8_TX_18X_STATE_NOT_EQUIPPED);
    }   

    if (TRUE == sfi51_d8_tx_18x_handle->var.use_acb) 
    {
        
        /* deprovision ACB_FT block */
        rc = acb_ft_deprov(sfi51_d8_tx_18x_handle->acb_ft_handle);
        
        if (PMC_SUCCESS == rc) 
        {        
            /* Put ACB in reset mode */
            sfi51_d8_tx_18x_csu_acb_reset(sfi51_d8_tx_18x_handle, TRUE,FALSE);      
        }
    }

    if (PMC_SUCCESS == rc) 
    {  
        /* update state */
        sfi51_d8_tx_18x_handle->var.state = SFI51_D8_TX_18X_START;
        sfi51_d8_tx_18x_handle->var.use_acb = FALSE;
    }

    PMC_RETURN(rc);    
} /* sfi51_d8_tx_18x_deprov */

/*******************************************************************************
* sfi51_d8_tx_18x_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function activate a D8 serdes instance.
*
* INPUTS:
*   *sfi51_d8_tx_18x_handle           - pointer to sfi51_d8_tx_18x handle instance.
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
PUBLIC PMC_ERROR sfi51_d8_tx_18x_activate(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle) 
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(sfi51_d8_tx_18x_handle != NULL, SFI51_D8_TX_18X_LOG_ERR_INVALID_PARAMETERS,0,0);
    if (SFI51_D8_TX_18X_EQUIPPED != sfi51_d8_tx_18x_handle->var.state) 
    {
        PMC_RETURN(SFI51_D8_TX_18X_STATE_NOT_EQUIPPED);
    }
    
    if (TRUE == sfi51_d8_tx_18x_handle->var.use_acb) 
    {
        rc = acb_ft_activate(sfi51_d8_tx_18x_handle->acb_ft_handle);
    }
    
    if (PMC_SUCCESS == rc) 
    {
        /* update state */
        sfi51_d8_tx_18x_handle->var.state = SFI51_D8_TX_18X_OPERATIONAL;
    }

    PMC_RETURN(rc);
} /* sfi51_d8_tx_18x_activate */

/*******************************************************************************
* sfi51_d8_tx_18x_loop_timing_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure loop timing
*
* INPUTS:
*   *handle                  - pointer to sfi51_d8_tx_18x handle instance.
*   port_mode                - otu rate
*   enable                   - enable/disable
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None.
*
* NOTES:
*    This function is used to enable loop timing when using lineotn mld 
*    facility loopback.
*******************************************************************************/
PUBLIC PMC_ERROR sfi51_d8_tx_18x_loop_timing_cfg(sfi51_d8_tx_18x_handle_t *handle, 
                                                 sfi51_d8_tx_18x_port_mode_t port_mode, 
                                                 BOOL enable) 
{
    UINT32 pkt_period;
    UINT32 pkt_size;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(handle != NULL, SFI51_D8_TX_18X_LOG_ERR_INVALID_PARAMETERS,0,0);

    if (SFI51_D8_TX_18X_OPERATIONAL != handle->var.state) 
    {
        PMC_RETURN(SFI51_D8_TX_18X_STATE_NOT_OPERATIONAL);
    }
    
    /* determine acb sertings based on rate */
    switch(port_mode)
    {
    case SFI51_D8_TX_18X_OTU3:
        pkt_period = 150;
        pkt_size = 162;
        break;
    case SFI51_D8_TX_18X_OTU3E1:
    case SFI51_D8_TX_18X_ODU3E2:
        pkt_period = 234;
        pkt_size = 262;
        break;
    case SFI51_D8_TX_18X_OC_768:
        pkt_period = 120;
        pkt_size = 120;
        break;
    default:
        PMC_ASSERT(FALSE,SFI51_D8_TX_18X_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }

    /*
     *   Configure ACB FT:
     *       -If the /ACB FT is already provisioned, save the conext and restore it when the
     */
    if (enable)
        rc = sfi51_d8_tx_18x_acb_ft_loop_timing_enable(handle, pkt_period,pkt_size);
    else
        rc = sfi51_d8_tx_18x_acb_ft_loop_timing_disable(handle);

    if (enable)
    {
        rc = acb_ft_activate(handle->acb_ft_handle);
    }
    handle->var.use_acb = enable;
    PMC_RETURN(rc);
} /* sfi51_d8_tx_18x_loop_timing_cfg */



/*******************************************************************************
* sfi51_d8_tx_18x_acb_ft_loop_timing_enable
* ______________________________________________________________________________
*
* DESCRIPTION: 
*    Enable ACB FT loop timing. 
*
* INPUTS:
*   *handle         - pointer to sfi51 handle
*   pkt_period      - ACB FT packet period
*   pkt_size        - ACB FT packet size
*   
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
PRIVATE PMC_ERROR sfi51_d8_tx_18x_acb_ft_loop_timing_enable(sfi51_d8_tx_18x_handle_t *handle, 
                                                            UINT32 pkt_period,
                                                            UINT32 pkt_size)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    if (acb_ft_provisionned(handle->acb_ft_handle))
    {
        /* when ACB FT is already provisionned - save the context */
        handle->var.has_lpbk_acb_ft_backup = TRUE;
        rc = acb_ft_deactivate(handle->acb_ft_handle);
        if (PMC_SUCCESS == rc) 
        {
            rc = acb_ft_context_push(handle->acb_ft_handle, UTIL_GLOBAL_ACB_FT_DEFAULT_CTXT_IDX);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = acb_ft_deprov(handle->acb_ft_handle);
            /* toggle reset */
            enh_8g_csu_field_ACB_RSTB_set(NULL,handle,0);
            enh_8g_csu_field_ACB_RSTB_set(NULL,handle,1);
            sfi51_tx_18x_serdes_glue_field_RXS_CTRL_RSTB_set(NULL,handle, 0);
            sfi51_tx_18x_serdes_glue_field_LOOPTIME_RSTB_set(NULL,handle,0);
            sfi51_tx_18x_serdes_glue_field_DIGITAL_RSTB_set(NULL,handle,0);
            sfi51_tx_18x_serdes_glue_field_SW_RESET_set(NULL,handle,1);
            handle->var.has_lpbk_acb_ft_backup = TRUE;
            sfi51_tx_18x_serdes_glue_field_PMM_ENABLE_set(NULL,handle,0);
        }
    }
    sfi51_d8_tx_18x_csu_acb_reset(handle, FALSE, TRUE); 

    /* Configure CSU */
    enh_8g_csu_field_FRAC_DIV_MOD_OVR_ENB_set(NULL,handle,1);  /* default is 0 */
    enh_8g_csu_field_PLL_S_OUT_FRAC_DIV_MOD_SEL_set(NULL,handle,0) ; /* default is 0 */
    enh_8g_csu_field_CS_CTRL_1_set(NULL,handle,0);      /* default is 1 */
    enh_8g_csu_field_ACB_CLK_FACT_set(NULL,handle,2);   /* default is 1 */


    if (rc == PMC_SUCCESS)
    {
        rc = acb_ft_sfi51_loop_timing_cfg(handle->acb_ft_handle,pkt_period, pkt_size);
    }

    sfi51_tx_18x_serdes_glue_field_PMM_ENABLE_set(NULL,handle,0); 
    sfi51_tx_18x_serdes_glue_field_PKT_PERIOD_set(NULL,handle,pkt_period);
    sfi51_tx_18x_serdes_glue_field_PKT_SIZE_set(NULL,handle,pkt_size);
    sfi51_tx_18x_serdes_glue_field_PMM_ENABLE_set(NULL,handle,1);
    PMC_RETURN(rc);
}/* sfi51_d8_tx_18x_acb_ft_loop_timing_enable */

/*******************************************************************************
* sfi51_d8_tx_18x_acb_ft_loop_timing_disable
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Disable ACB FT loop timing. If there was a previously stored ACB FT context,
*   it will re restored.
*
* INPUTS:
*   *handle         - pointer to sfi51 handle
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error is returned. 
*
* NOTES:
*   -This function is used when the lineotn mld facility loopback is disabled.
*
*******************************************************************************/
PRIVATE PMC_ERROR sfi51_d8_tx_18x_acb_ft_loop_timing_disable(sfi51_d8_tx_18x_handle_t *handle)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    rc = acb_ft_deactivate(handle->acb_ft_handle);

    /* Toggle reset */
    enh_8g_csu_field_ACB_RSTB_set(NULL,handle,0);
    enh_8g_csu_field_ACB_RSTB_set(NULL,handle,1);

    /* Configure CSU */
    enh_8g_csu_field_FRAC_DIV_MOD_OVR_ENB_set(NULL,handle,0);  /* default is 0 */
    enh_8g_csu_field_PLL_S_OUT_FRAC_DIV_MOD_SEL_set(NULL,handle,0) ; /* default is 0 */
    enh_8g_csu_field_CS_CTRL_1_set(NULL,handle,1);      /* default is 1 */
    enh_8g_csu_field_ACB_CLK_FACT_set(NULL,handle,1);   /* default is 1 */
    sfi51_tx_18x_serdes_glue_field_PMM_ENABLE_set(NULL,handle,0); 
    sfi51_tx_18x_serdes_glue_field_PKT_PERIOD_set(NULL,handle,0);
    sfi51_tx_18x_serdes_glue_field_PKT_SIZE_set(NULL,handle,0);

    if (PMC_SUCCESS == rc)
    {
        if (handle->var.has_lpbk_acb_ft_backup)
        {
            rc = acb_ft_context_pop(handle->acb_ft_handle, UTIL_GLOBAL_ACB_FT_DEFAULT_CTXT_IDX);
            
            sfi51_d8_tx_18x_csu_acb_reset(handle, FALSE, handle->var.has_loop_timing_enabled);

            if (PMC_SUCCESS == rc) 
            {
                rc = acb_ft_activate(handle->acb_ft_handle);
            }

            handle->var.has_lpbk_acb_ft_backup = FALSE;
        }
        else
        {
            sfi51_d8_tx_18x_csu_acb_reset(handle, TRUE, FALSE); /* reset = true, loop timing = don't care */
        }
    }

    PMC_RETURN(rc);
} /* sfi51_d8_tx_18x_acb_ft_loop_timing_disable */

/*******************************************************************************
* sfi51_d8_tx_18x_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function deactivate a D8 serdes instance.
*
* INPUTS:
*   *sfi51_d8_tx_18x_handle           - pointer to sfi51_d8_tx_18x handle instance.
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
PUBLIC PMC_ERROR sfi51_d8_tx_18x_deactivate(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle) 
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(sfi51_d8_tx_18x_handle != NULL, SFI51_D8_TX_18X_LOG_ERR_INVALID_PARAMETERS,0,0);

    if (SFI51_D8_TX_18X_OPERATIONAL != sfi51_d8_tx_18x_handle->var.state) 
    {
        PMC_RETURN(SFI51_D8_TX_18X_STATE_NOT_OPERATIONAL);
    }

    if (TRUE == sfi51_d8_tx_18x_handle->var.use_acb) 
    {
        rc = acb_ft_deactivate(sfi51_d8_tx_18x_handle->acb_ft_handle);
    }

    if (PMC_SUCCESS == rc) 
    {
        /* update state */
        sfi51_d8_tx_18x_handle->var.state = SFI51_D8_TX_18X_EQUIPPED;
    }

    PMC_RETURN(rc);
} /* sfi51_d8_tx_18x_deactivate */


/*******************************************************************************
* sfi51_d8_tx_18x_restart
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function puts back SFI51_D8_TX_18X serdes in start mode or in loopback mode.
*
* INPUTS:
*   *sfi51_d8_tx_18x_handle  - pointer to SFI51_D8_TX_18X handle instance.
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
PUBLIC void sfi51_d8_tx_18x_restart(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle) 
{

    PMC_ENTRY();

    PMC_ASSERT(sfi51_d8_tx_18x_handle != NULL, SFI51_D8_TX_18X_LOG_ERR_INVALID_PARAMETERS,0,0);
    
    if (SFI51_D8_TX_18X_RESET !=  sfi51_d8_tx_18x_handle->var.state)
    {
        acb_ft_restart(sfi51_d8_tx_18x_handle->acb_ft_handle, FALSE);
        
        /* Put ACB in reset mode */
        sfi51_d8_tx_18x_csu_acb_reset(sfi51_d8_tx_18x_handle, TRUE, FALSE);
        
        sfi51_d8_tx_18x_handle->var.use_acb = FALSE;

        /* update state */
        sfi51_d8_tx_18x_handle->var.state = SFI51_D8_TX_18X_START;    

        if (TRUE == sfi51_d8_tx_18x_handle->var.loopback_enable)
        {
            sfi51_d8_tx_18x_loopback_int_activate(sfi51_d8_tx_18x_handle);
        }
    }

    PMC_RETURN();
} /* sfi51_d8_tx_18x_restart */

/*******************************************************************************
* sfi51_d8_tx_18x_csu_div_val_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function set csu divider values.
*
* INPUTS:
*   *sfi51_d8_tx_18x_handle - pointer to SFI51_D8_TX_18X handle instance.
*   csu_n                   - CSU divider nominator
*   csu_denomin             - CSU divider denominator
*   csu_r                   - CSU PLL integer multiplication
*
* OUTPUTS:
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void sfi51_d8_tx_18x_csu_div_val_set(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle, 
                                             UINT64 csu_n, 
                                             UINT64 csu_denomin, 
                                             UINT32 csu_r)
{
    
    PMC_ENTRY();
       
    enh_8g_csu_field_INT_DIV_set(NULL,sfi51_d8_tx_18x_handle, csu_r);
    enh_8g_csu_field_DENOMIN_UPPER_set(NULL,sfi51_d8_tx_18x_handle, (UINT32)(csu_denomin >> 32) & 0xffffffff);
    enh_8g_csu_field_DENOMIN_LOWER_set(NULL,sfi51_d8_tx_18x_handle,(UINT32)(csu_denomin) & 0xffffffff);
    enh_8g_csu_field_FRAC_DIV_UPPER_set(NULL,sfi51_d8_tx_18x_handle, ((UINT32)(csu_n >> 32)) & 0xffffffff);
    enh_8g_csu_field_FRAC_DIV_LOWER_set(NULL,sfi51_d8_tx_18x_handle, ((UINT32)csu_n) & 0xffffffff);

    PMC_RETURN();
}  /*sfi51_d8_tx_18x_csu_div_val_set*/

/*******************************************************************************
* sfi51_d8_tx_18x_csu_div_val_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extract csu divider values
*
* INPUTS:
*   *sfi51_d8_tx_18x_handle - pointer to SFI51_D8_TX_18X handle instance.
*
* OUTPUTS:
*   *csu_div              - CSU divider in DOUBLE notation
*   *csu_n                - CSU divider nominator
*   *csu_denomin          - CSU divider denominator
*   *csu_r                - CSU PLL integer multiplication
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void sfi51_d8_tx_18x_csu_div_val_get(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle, 
                                             DOUBLE *csu_div,
                                             UINT64 *csu_n, 
                                             UINT64 *csu_denomin, 
                                             UINT32 *csu_r)
{
    UINT32 DENOMIN_UPPER;
    UINT32 DENOMIN_LOWER ;
    UINT32 FRAC_DIV_UPPER;
    UINT32 FRAC_DIV_LOWER;
    
    PMC_ENTRY();
       
    *csu_r         = enh_8g_csu_field_INT_DIV_get(NULL,sfi51_d8_tx_18x_handle);
    DENOMIN_UPPER  = enh_8g_csu_field_DENOMIN_UPPER_get(NULL,sfi51_d8_tx_18x_handle);
    DENOMIN_LOWER  = enh_8g_csu_field_DENOMIN_LOWER_get(NULL,sfi51_d8_tx_18x_handle);
    FRAC_DIV_UPPER = enh_8g_csu_field_FRAC_DIV_UPPER_get(NULL,sfi51_d8_tx_18x_handle);
    FRAC_DIV_LOWER = enh_8g_csu_field_FRAC_DIV_LOWER_get(NULL,sfi51_d8_tx_18x_handle);

    *csu_denomin = ((UINT64) DENOMIN_UPPER << 32) + DENOMIN_LOWER;
    *csu_n = ((UINT64)FRAC_DIV_UPPER << 32) + FRAC_DIV_LOWER;
    *csu_div = SFI51_D8_TX_18X_CSU_DIV_COMPUTE((*csu_r), (*csu_n), *(csu_denomin));

    PMC_RETURN();
}  /*sfi51_d8_tx_18x_csu_div_val_get*/

/*******************************************************************************
* sfi51_d8_tx_18x_csu_acb_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the CSU ACB reset
*
* INPUTS:
*   *sfi51_d8_tx_18x_handle      - pointer to SFI51_D8_TX_18X handle instance.
*   reset                        - control ACB reset
*   loop_timing                  - loop_timing mode enabled
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
PRIVATE void sfi51_d8_tx_18x_csu_acb_reset(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle,
                                           BOOL8 reset, 
                                           BOOL8 loop_timing)
{    
    sfi51_tx_18x_serdes_glue_buffer_t b_serdes_glue[1];
    enh_8g_csu_buffer_t b_csu[1];

    PMC_ENTRY();
    
    sfi51_tx_18x_serdes_glue_buffer_init(b_serdes_glue,sfi51_d8_tx_18x_handle);
    enh_8g_csu_buffer_init( b_csu, sfi51_d8_tx_18x_handle );

    if (TRUE == reset) 
    {
        enh_8g_csu_field_FRAC_DIV_MOD_OVR_ENB_set(b_csu,sfi51_d8_tx_18x_handle,0);
        enh_8g_csu_field_PLL_S_OUT_FRAC_DIV_MOD_SEL_set(b_csu,sfi51_d8_tx_18x_handle,0);
        enh_8g_csu_field_CS_CTRL_1_set(b_csu,sfi51_d8_tx_18x_handle,1);
        sfi51_tx_18x_serdes_glue_field_CKCTL_SEL_set(b_serdes_glue,sfi51_d8_tx_18x_handle,0);
        sfi51_tx_18x_serdes_glue_field_RXS_CTRL_RSTB_set(b_serdes_glue,sfi51_d8_tx_18x_handle,1);
        sfi51_tx_18x_serdes_glue_field_LOOPTIME_RSTB_set(b_serdes_glue,sfi51_d8_tx_18x_handle,1);
        sfi51_tx_18x_serdes_glue_field_DIGITAL_RSTB_set(b_serdes_glue,sfi51_d8_tx_18x_handle,1);
        sfi51_tx_18x_serdes_glue_field_SW_RESET_set(b_serdes_glue,sfi51_d8_tx_18x_handle,0);
    } else 
    {
        enh_8g_csu_field_FRAC_DIV_MOD_OVR_ENB_set(b_csu,sfi51_d8_tx_18x_handle,1);
        enh_8g_csu_field_PLL_S_OUT_FRAC_DIV_MOD_SEL_set(b_csu,sfi51_d8_tx_18x_handle,0);
        enh_8g_csu_field_CS_CTRL_1_set(b_csu,sfi51_d8_tx_18x_handle,0);
        sfi51_tx_18x_serdes_glue_field_CKCTL_SEL_set(b_serdes_glue,sfi51_d8_tx_18x_handle, (loop_timing ? 0 : 1));
        sfi51_tx_18x_serdes_glue_field_RXS_CTRL_RSTB_set(b_serdes_glue,sfi51_d8_tx_18x_handle,1);
        sfi51_tx_18x_serdes_glue_field_LOOPTIME_RSTB_set(b_serdes_glue,sfi51_d8_tx_18x_handle,1);
        sfi51_tx_18x_serdes_glue_field_DIGITAL_RSTB_set(b_serdes_glue,sfi51_d8_tx_18x_handle,1);
        sfi51_tx_18x_serdes_glue_field_SW_RESET_set(b_serdes_glue,sfi51_d8_tx_18x_handle,0);

    }
    sfi51_tx_18x_serdes_glue_buffer_flush(b_serdes_glue);
    enh_8g_csu_buffer_flush(b_csu);

    PMC_RETURN();
} /*sfi51_d8_tx_18x_csu_acb_reset*/

/*******************************************************************************
* sfi51_d8_tx_18x_validate_serdes_id
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
PRIVATE BOOL8 sfi51_d8_tx_18x_validate_serdes_id(UINT8  serdes_id,
                                                 UINT8 *serdes_slice,
                                                 UINT8 *serdes_pin)
{

    PMC_ENTRY();

    /* check serdes ID */
    if ((SFI51_D8_TX_NUM_SLICES * 2) <= serdes_id)
    {
        PMC_RETURN(FALSE);
    }

    *serdes_slice = SFI51_D8_TX_18X_SERDES_SLICE_GET(serdes_id);
    *serdes_pin = SFI51_D8_TX_18X_SERDES_PIN_GET(serdes_id);

    PMC_RETURN(TRUE);
    
}
 
/*******************************************************************************
* sfi51_d8_tx_18x_polarity_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function configure a sfi51  instance to enable/disable lane polarity 
*   inversion
*
* INPUTS:
*   *handle         - pointer to sfi51_d8_tx_18x handle.
*   serdes_id       - serdes identifier.
*   enable          - TRUE to enable polarity inversion, FALSE to disable it.
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
PUBLIC PMC_ERROR sfi51_d8_tx_18x_polarity_cfg(sfi51_d8_tx_18x_handle_t *handle,
                                              UINT8                     serdes_id,
                                              BOOL8                     enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 d8_slice;
    UINT8 d8_pin;

    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, SFI51_D8_TX_18X_LOG_ERR_NULL_HANDLE, 0, 0);
   
    if ( TRUE == sfi51_d8_tx_18x_validate_serdes_id(serdes_id, &d8_slice, &d8_pin))
    {
        /* configure the polarity */
        d8_tx_2x_polarity_cfg(handle->d8_tx_2x_hndl[d8_slice], 
                              d8_pin,
                              enable);
    }
    else 
    {
        rc = SFI51_D8_TX_18X_LOG_ERR_SERDES_ID_INVALID;
    }

    PMC_RETURN(rc);

} /* sfi51_d8_tx_18x_polarity_cfg */



/*******************************************************************************
* sfi51_d8_tx_18x_polarity_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function returns sfi51 d8 serdes polarity status for a given lane.
*
* INPUTS:
*   *handle         - pointer to sfi51_d8_tx_18x handle.
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
PUBLIC PMC_ERROR sfi51_d8_tx_18x_polarity_get(sfi51_d8_tx_18x_handle_t *handle,
                                              UINT8                     serdes_id,
                                              BOOL8                    *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 d8_slice;
    UINT8 d8_pin;

    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, SFI51_D8_TX_18X_LOG_ERR_NULL_HANDLE, 0, 0);
   
    if ( TRUE == sfi51_d8_tx_18x_validate_serdes_id(serdes_id, &d8_slice, &d8_pin))
    {
        /* configure the polarity */
        d8_tx_2x_polarity_get(handle->d8_tx_2x_hndl[d8_slice], 
                              d8_pin,                                   
                              enable);
    }
    else 
    {
        rc = SFI51_D8_TX_18X_LOG_ERR_SERDES_ID_INVALID;
    }

    PMC_RETURN(rc);

} /* sfi51_d8_tx_18x_polarity_get */


/*******************************************************************************
* sfi51_d8_tx_18x_swing_level_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function configures TX swing level for a given D8 serdes in SFI51 bus.
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
PUBLIC PMC_ERROR sfi51_d8_tx_18x_swing_level_set(sfi51_d8_tx_18x_handle_t    *handle,
                                                 UINT8                        serdes_id,
                                                 d8_tx_2x_tx_swing_level_t    tx_swing_level)     
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 d8_slice;
    UINT8 d8_pin;

    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, SFI51_D8_TX_18X_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    /* check serdes ID */
    if (FALSE == sfi51_d8_tx_18x_validate_serdes_id(serdes_id, &d8_slice, &d8_pin))
    {
        rc = SFI51_D8_TX_18X_LOG_ERR_SERDES_ID_INVALID;
    }

    if (PMC_SUCCESS == rc)
    {
        /* set register */
        d8_tx_2x_tx_swing_level_set(handle->d8_tx_2x_hndl[d8_slice], 
                                    d8_pin, 
                                    tx_swing_level);
    }
    
    PMC_RETURN(rc);
}/*sfi51_d8_tx_18x_swing_level_set*/

/*******************************************************************************
* sfi51_d8_tx_18x_swing_level_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function returns TX swing level of a given D8 serdes in SFI51 bus.
*
* INPUTS:
*   *handle                - pointer to d8 tx 2 slices handle.
*   serdes_id              - serdes identifier.
*
* OUTPUTS:
*   *tx_swing_level         - TX swing level configured value.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error code is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sfi51_d8_tx_18x_swing_level_get(sfi51_d8_tx_18x_handle_t    *handle,
                                                 UINT8                        serdes_id,
                                                 d8_tx_2x_tx_swing_level_t   *tx_swing_level)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 d8_slice;
    UINT8 d8_pin;

    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, SFI51_D8_TX_18X_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    /* check serdes ID */
    if (FALSE == sfi51_d8_tx_18x_validate_serdes_id(serdes_id, &d8_slice, &d8_pin))
    {
        rc = SFI51_D8_TX_18X_LOG_ERR_SERDES_ID_INVALID;
    }

    if (PMC_SUCCESS == rc)
    {
        /* read register value */
        rc = d8_tx_2x_tx_swing_level_get(handle->d8_tx_2x_hndl[d8_slice], 
                                         d8_pin,
                                         tx_swing_level);
    }
    
    PMC_RETURN(rc);
}/*sfi51_d8_tx_18x_swing_level_set*/


/*******************************************************************************
* FUNCTION: sfi51_d8_tx_18x_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   sfi51_d8_tx_18x_handle      - Pointer to subsystem handle
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
PUBLIC PMC_ERROR sfi51_d8_tx_18x_int_validate(sfi51_d8_tx_18x_handle_t  *sfi51_d8_tx_18x_handle,
                                              UINT32                     serdes_id,
                                              void                      *non_null_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != sfi51_d8_tx_18x_handle, SFI51_D8_TX_18X_LOG_ERR_NULL_HANDLE, 0, 0);
   
    if (serdes_id >= SFI51_D8_TX_MAX_NUM_SERDES)
    {
        result = SFI51_D8_TX_18X_LOG_ERR_INVALID_PARAMETERS;
    }

    if (NULL == non_null_ptr)
    {
        result = SFI51_D8_TX_18X_LOG_ERR_INVALID_PARAMETERS;
    }


    PMC_RETURN(result);
} /* sfi51_d8_tx_18x_int_validate */


/*******************************************************************************
* sfi51_d8_tx_18x_max_ppm_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Function sets ACB FT maximum PPM.
*
* INPUTS:
*   *handle                - pointer to d8 tx 2 slices handle.
*   max_ppm                - maximum PPM.
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
PUBLIC void sfi51_d8_tx_18x_max_ppm_set(sfi51_d8_tx_18x_handle_t    *handle,
                                        UINT32                       max_ppm)
{
    UINT64 csu_n;
    UINT64 csu_d;
    UINT32 csu_r;
    DOUBLE csu_div;
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, SFI51_D8_TX_18X_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    sfi51_d8_tx_18x_csu_div_val_get(handle, &csu_div, &csu_n, &csu_d, &csu_r);
    acb_ft_pll_s_max_cfg(handle->acb_ft_handle, max_ppm, csu_n, csu_d, csu_r);

    PMC_RETURN();
}/*sfi51_d8_tx_18x_max_ppm_set*/

/*******************************************************************************
* sfi51_d8_tx_18x_squelch_default_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function set the SQUELCH overwrite default value.
*
* INPUTS:
*   *sfi51_d8_tx_18x_handle           - pointer to sfi51_d8_tx_18x handle instance.
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
PUBLIC void sfi51_d8_tx_18x_squelch_default_set(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle) 
{
    sfi51_tx_18x_serdes_glue_buffer_t buf[1];

    PMC_ENTRY();

    PMC_ASSERT(sfi51_d8_tx_18x_handle != NULL, SFI51_D8_TX_18X_LOG_ERR_INVALID_PARAMETERS,0,0);

    sfi51_tx_18x_serdes_glue_buffer_init(buf,sfi51_d8_tx_18x_handle);

    sfi51_tx_18x_serdes_glue_field_SQUELCH_OVR_set(buf, 
                                                   sfi51_d8_tx_18x_handle,
                                                   0);
    sfi51_tx_18x_serdes_glue_field_SQUELCH_OVR_MODE_set(buf, 
                                                        sfi51_d8_tx_18x_handle,
                                                        0x3);
    
    sfi51_tx_18x_serdes_glue_buffer_flush(buf);

    PMC_RETURN();
} /* sfi51_d8_tx_18x_squelch_default_set */


/*******************************************************************************
* sfi51_d8_tx_18x_tx_refclk_source_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Function sets SFI51 interface TX reference clock source.
*
* INPUTS:
*   *sfi51_d8_tx_18x_handle           - pointer to sfi51_d8_tx_18x handle instance.
*    refclk_src                       - reference clock source.
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
PUBLIC PMC_ERROR sfi51_d8_tx_18x_tx_refclk_source_set(sfi51_d8_tx_18x_handle_t          *sfi51_d8_tx_18x_handle,
                                                      util_global_sfi51_refclk_source_t  refclk_src)
{
    PMC_ERROR rc = PMC_SUCCESS;
    DOUBLE csu_div;
    UINT64 csu_n;
    UINT64 csu_denomin;
    UINT32 csu_r;
    BOOL8 update_csu = FALSE;

    PMC_ENTRY();

    PMC_ASSERT(sfi51_d8_tx_18x_handle != NULL, SFI51_D8_TX_18X_LOG_ERR_INVALID_PARAMETERS,0,0);

    
    if (LAST_UTIL_GLOBAL_SFI51_REFCLK_SRC > refclk_src)
    {
        sfi51_tx_18x_serdes_glue_field_REFCLK_SEL_set(NULL, 
                                                      sfi51_d8_tx_18x_handle,
                                                      (UINT32)refclk_src);
    }
    else
    {
        rc = SFI51_D8_TX_18X_LOG_ERR_INVALID_PARAMETERS;
    }

    if (PMC_SUCCESS == rc)
    {
        switch(refclk_src)
        {
        case UTIL_GLOBAL_SFI51_REFCLK_SRC_0:
            if (TRUE == sfi51_d8_tx_18x_handle->var.csu_ctxt_backup.valid)
            {
                csu_n = sfi51_d8_tx_18x_handle->var.csu_ctxt_backup.csu_n;
                csu_denomin = sfi51_d8_tx_18x_handle->var.csu_ctxt_backup.csu_denomin;
                csu_r = sfi51_d8_tx_18x_handle->var.csu_ctxt_backup.csu_r;
                sfi51_d8_tx_18x_handle->var.csu_ctxt_backup.valid = FALSE;
                update_csu = TRUE;
            }
            break;
            /* passthru */
        case UTIL_GLOBAL_SFI51_REFCLK_SRC_1:
        case UTIL_GLOBAL_SFI51_REFCLK_SRC_2:
            if (FALSE == sfi51_d8_tx_18x_handle->var.csu_ctxt_backup.valid)
            {
                sfi51_d8_tx_18x_csu_div_val_get(sfi51_d8_tx_18x_handle,
                                                &csu_div,
                                                &sfi51_d8_tx_18x_handle->var.csu_ctxt_backup.csu_n,
                                                &sfi51_d8_tx_18x_handle->var.csu_ctxt_backup.csu_denomin,
                                                &sfi51_d8_tx_18x_handle->var.csu_ctxt_backup.csu_r);
                sfi51_d8_tx_18x_handle->var.csu_ctxt_backup.valid = TRUE;

                csu_n = SFI51_D8_TX_18X_REDCLK_SRC_1_2_CSU_FRAC_DIV;
                csu_denomin = SFI51_D8_TX_18X_REDCLK_SRC_1_2_CSU_DENOMIN;
                csu_r = SFI51_D8_TX_18X_REDCLK_SRC_1_2_CSU_INT_DIV;
                update_csu = TRUE;
            }
            break;
            /* passthru */
        default:
            break;
        }
    }

    /* reprovision CSU and ACB Ft if needed */
    if (TRUE == update_csu)
    {
        sfi51_d8_tx_18x_csu_div_val_set(sfi51_d8_tx_18x_handle,
                                        csu_n,
                                        csu_denomin,
                                        csu_r);      
    }

    PMC_RETURN(rc);
} /* sfi51_d8_tx_18x_tx_refclk_source_set */


/*******************************************************************************
* sfi51_d8_tx_18x_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on sfi51_d8_tx_18x handle instance.
*
* INPUTS:
*   *sfi51_d8_tx_18x_handle    - pointer to sfi51_d8_tx_18x handle instance
*   *restart_init_cfg_ptr      - pointer to restart init config
*    reset                     - SFI51_D8_TX_18X serdes reset status
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS : 
*     WARM : register/context are coherent
*     CRASH: everything is coherent (a cleanup may has been executed)
*   SFI51_D8_TX_18X_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT:
*     incoherences have been detected on communal energy configuration
*     that is incoherent with top energy state
*      WARM : ERROR
*      CRASH: TOP shall put SFI51_D8_TX_18X in RESET mode
*   
subsystem shall be put in reset mode 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sfi51_d8_tx_18x_handle_restart_init(sfi51_d8_tx_18x_handle_t       *sfi51_d8_tx_18x_handle,
                                                     util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                                     BOOL8                           reset)
{
    PMC_ERROR           rc = PMC_SUCCESS;
    sfi51_d8_tx_18x_var_t           *sfi51_d8_tx_18x_var_clean;
    
    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(SFI51_D8_TX_18X_LOG_ERR_STRINGS[0], SFI51_D8_TX_18X_LOG_ERR_TABLE_BASE, SFI51_D8_TX_18X_LOG_ERR_COUNT);
    }

    sfi51_d8_tx_18x_var_clean =  (sfi51_d8_tx_18x_var_t *) PMC_CALLOC(sizeof(sfi51_d8_tx_18x_var_t)); 

    rc = acb_ft_handle_restart_init(sfi51_d8_tx_18x_handle->acb_ft_handle, restart_init_cfg_ptr, PMC_ENERGY_STATE_RUNNING);

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
     *   - Create a new context that represents the intersection between memory and register space (sfi51_d8_tx_18x_var_clean)
     */
    rc = sfi51_d8_tx_18x_ctxt_audit(sfi51_d8_tx_18x_handle,
                        sfi51_d8_tx_18x_var_clean,
                        reset); 
    
    PMC_LOG_TRACE("sfi51_d8_tx_18x_ctxt_audit rc = %s\n", pmc_log_string_get(rc));
    if (SFI51_D8_TX_18X_ERR_CONTEXT_CLEANUP == rc && restart_init_cfg_ptr->is_warm_restart == FALSE)
    {
        /* 
         * Cleanup 
         *   - At the end, sfi51_d8_tx_18x_var_clean is the new context
         *    - We keep OPERATIONAL channel defined in sfi51_d8_tx_18x_var_clean
         *    - We clean (deactivate/deprov) all channels that are not OPERATIONAL
         *       - they may have be partially provisionned
         *    - We ajust power setting based on sfi51_d8_tx_18x_var_clean
         */                                    
        rc = sfi51_d8_tx_18x_ctxt_cleanup(sfi51_d8_tx_18x_handle,
                                          sfi51_d8_tx_18x_var_clean); 
    }

    PMC_FREE(&sfi51_d8_tx_18x_var_clean);

    PMC_RETURN(rc);
} /* sfi51_d8_tx_18x_handle_restart_init */

/*******************************************************************************
* sfi51_d8_tx_18x_ctxt_audit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function audits SFI51_D8_TX_18X handle context variable against register space content.
*
* INPUTS:
*   *sfi51_d8_tx_18x_handle           - pointer to SFI51_D8_TX_18X handle instance.
*   *sfi51_d8_tx_18x_var_clean        - pointer to SFI51_D8_TX_18X lanes context.
*    serdes_reset                     - SFI51_D8_TX_18X serdes reset state.
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
PRIVATE PMC_ERROR sfi51_d8_tx_18x_ctxt_audit(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle,
                                             sfi51_d8_tx_18x_var_t    *sfi51_d8_tx_18x_var_clean,
                                             BOOL8                     serdes_reset)
{
    BOOL8 serdes_ctxt_match;
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL8 lpbk_enabled = FALSE;
    UINT32 acb_ft_csu_reset;

    PMC_ENTRY();

    PMC_ASSERT(sfi51_d8_tx_18x_handle != NULL, SFI51_D8_TX_18X_LOG_ERR_INVALID_PARAMETERS,0,0);
    
    serdes_ctxt_match = TRUE;

    if (FALSE == serdes_reset)
    {
        /* check sfi51_d8_tx_18x provisioning & activation status */
        if (SFI51_D8_TX_18X_RESET == sfi51_d8_tx_18x_handle->var.state ||
            LAST_SFI51_D8_TX_18X_STATE == sfi51_d8_tx_18x_handle->var.state)
        {
            serdes_ctxt_match = FALSE;
        }
        
        /* audit acb_ft activation && prov status if necessary */
        if (TRUE == serdes_ctxt_match)
        {
            if (SFI51_D8_TX_18X_EQUIPPED != sfi51_d8_tx_18x_handle->var.state)
            {
                acb_ft_csu_reset =  enh_8g_csu_field_CS_CTRL_1_get(NULL,
                                                                   sfi51_d8_tx_18x_handle);
                
                if ((TRUE == sfi51_d8_tx_18x_handle->var.use_acb &&
                     1 == acb_ft_csu_reset  &&
                     2 != enh_8g_csu_field_ACB_CLK_FACT_get(NULL, sfi51_d8_tx_18x_handle)))
                {
                    /* acb should be activated only if has_acb is TRUE and sfi51_d8_tx_18x is activated */
                    /* sfi51_d8_tx_18x_handle->var.has_acb */
                    serdes_ctxt_match = FALSE;
                }
                /* check loopback status */
                if (TRUE == serdes_ctxt_match)
                {
                    rc = sfi51_d8_tx_18x_loopback_cfg_get(sfi51_d8_tx_18x_handle, &lpbk_enabled);
                    if (PMC_SUCCESS != rc)
                    {
                        serdes_ctxt_match = FALSE;
                    }
                        
                    if (TRUE == serdes_ctxt_match)
                    {
                        if (lpbk_enabled != sfi51_d8_tx_18x_handle->var.loopback_enable)
                        {
                            serdes_ctxt_match = FALSE;                        
                        }
                    }
                }
            }
        }
    }
    else
    {
        /* check sfi51_d8_tx_18x provisioning & activation status */
        if (SFI51_D8_TX_18X_RESET != sfi51_d8_tx_18x_handle->var.state &&
            LAST_SFI51_D8_TX_18X_STATE != sfi51_d8_tx_18x_handle->var.state)
        {
            serdes_ctxt_match = FALSE;
        }
    }
    /* update returned error code anf var clean (used to cleanup the context) */
    if (FALSE == serdes_ctxt_match)
    {
        if (PMC_SUCCESS == rc)
        {
            rc = SFI51_D8_TX_18X_ERR_CONTEXT_CLEANUP;
        }
        sfi51_d8_tx_18x_var_default_init(sfi51_d8_tx_18x_var_clean);
    }   
    else
    {
        if (FALSE == (SFI51_D8_TX_18X_RESET != sfi51_d8_tx_18x_handle->var.state &&
                      LAST_SFI51_D8_TX_18X_STATE != sfi51_d8_tx_18x_handle->var.state))
        {
            PMC_MEMCPY((void*)sfi51_d8_tx_18x_var_clean, (void*)&sfi51_d8_tx_18x_handle->var, sizeof(sfi51_d8_tx_18x_var_t));
        }
    } 

    PMC_RETURN(rc);
} /* sfi51_d8_tx_18x_ctxt_audit */

/*******************************************************************************
* sfi51_d8_tx_18x_ctxt_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function cleans up SFI51_D8_TX_18X handle context. 
*
* INPUTS:
*   *sfi51_d8_tx_18x_handle           - pointer to SFI51_D8_TX_18X handle instance.
*   *sfi51_d8_tx_18x_var_clean        - pointer to SFI51_D8_TX_18X lanes clean context.
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
PRIVATE PMC_ERROR sfi51_d8_tx_18x_ctxt_cleanup(sfi51_d8_tx_18x_handle_t  *sfi51_d8_tx_18x_handle,
                                               sfi51_d8_tx_18x_var_t     *sfi51_d8_tx_18x_var_clean)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(sfi51_d8_tx_18x_handle != NULL, SFI51_D8_TX_18X_LOG_ERR_INVALID_PARAMETERS,0,0);
    
    if (SFI51_D8_TX_18X_RESET == sfi51_d8_tx_18x_var_clean->state)
    {
        sfi51_d8_tx_18x_cleanup(sfi51_d8_tx_18x_handle);
    }

    PMC_RETURN(rc);
} /* sfi51_d8_tx_18x_ctxt_cleanup */

/*******************************************************************************
* sfi51_d8_tx_18x_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function cleanup SFI51_D8_TX_18X serdes lane context.
*
* INPUTS:
*   *sfi51_d8_tx_18x_handle           - pointer to SFI51_D8_TX_18X handle instance.
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
PRIVATE void sfi51_d8_tx_18x_cleanup(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle) 
{
    UINT32 itr;
    PMC_ENTRY();

    PMC_ASSERT(sfi51_d8_tx_18x_handle != NULL, SFI51_D8_TX_18X_LOG_ERR_INVALID_PARAMETERS,0,0);

    /* cleanup ACB register space and context */
    acb_ft_cleanup(sfi51_d8_tx_18x_handle->acb_ft_handle);

    /* update state */
    /* cleanup d8_tx_2x slices */
    for (itr = 0; itr < SFI51_D8_TX_NUM_SLICES; itr += 1)
    {
        d8_tx_2x_ctxt_cleanup(sfi51_d8_tx_18x_handle->d8_tx_2x_hndl[itr]);
    }

    sfi51_d8_tx_18x_var_default_init(&sfi51_d8_tx_18x_handle->var);

    PMC_RETURN();
} /* sfi51_d8_tx_18x_cleanup */

/*******************************************************************************
* sfi51_d8_tx_18x_loopback_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns SFI51_D8_TX_18X instance configured loopback mode.
*
* INPUTS:
*   *handle         - pointer to sfi51_d8_tx_18x handle.
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
PRIVATE PMC_ERROR sfi51_d8_tx_18x_loopback_cfg_get(sfi51_d8_tx_18x_handle_t *handle, 
                                                   BOOL8                    *lpbk_enabled_ptr) 
{
    UINT32 itr;
    UINT32 jtr;
    BOOL8 lpbk_enabled = FALSE;
    BOOL8 tmp;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    for (itr = 0; itr < SFI51_D8_TX_NUM_SLICES && PMC_SUCCESS == rc; itr += 1)
    {
        for (jtr = 0; jtr < D8_TX_2X_MAX_NUM_SERDES; jtr += 1)
        {
            tmp = d8_tx_2x_loopback_activate_status_get(handle->d8_tx_2x_hndl[itr], jtr);

            if (0 == itr && 0 == jtr)
            {
                lpbk_enabled = tmp;
            }

            if (lpbk_enabled != tmp)
            {
                rc = SFI51_D8_TX_18X_ERR_CONTEXT_REG_INCOHERENT;
            }
        }
    }
    
    if (PMC_SUCCESS == rc)
    {
        *lpbk_enabled_ptr = lpbk_enabled;
    }

    PMC_RETURN(rc);
}/*sfi51_d8_tx_18x_loopback_cfg_get*/

/*******************************************************************************
* sfi51_d8_tx_18x_reset_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns SFI51_D8_TX_18X instance reset status based on 
*    context content.
*
* INPUTS:
*   *handle         - pointer to sfi51_d8_tx_18x handle.
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
PUBLIC BOOL8 sfi51_d8_tx_18x_reset_state_get(sfi51_d8_tx_18x_handle_t *handle) 
{
    BOOL8 rc;

    PMC_ENTRY();

    rc = (SFI51_D8_TX_18X_RESET == handle->var.state) ? TRUE : FALSE;
    
    PMC_RETURN(rc);
}/*sfi51_d8_tx_18x_reset_state_get*/

/*
** End of file
*/



