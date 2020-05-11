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
*     ACB FT functions implementation.
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "acb_ft_loc.h"
#include "acb_ft_io.h"
#include "acb_ft_io_inline.h"


/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
/* ACB FT PLL_S_MAX default value */
#define  ACB_FT_PLL_S_MAX_DEF_VAL 0x12

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
const char ACB_FT_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    ACB_FT_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char ACB_FT_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */


/*
** Forward References
*/
PRIVATE void acb_ft_var_default_init(acb_ft_handle_t *acb_ft_handle);

PRIVATE void acb_ft_init_reset(acb_ft_handle_t *acb_ft_handle);

PRIVATE void acb_ft_init_ctrl_cfg(acb_ft_handle_t *acb_ft_handle);

PRIVATE void acb_ft_pkt_period_calc(
    acb_ft_handle_t *acb_ft_handle, 
    acb_ft_prov_cfg_t *cfg_ptr, 
    UINT32 *pkt_period_int, 
    UINT32 *pkt_period_d_msb, 
    UINT32 *pkt_period_d_lsb, 
    UINT32 *pkt_period_n_msb, 
    UINT32 *pkt_period_n_lsb, 
    UINT32 *thermometer_int, 
    UINT32 *thermometer_d, 
    UINT32 *thermometer_n,
    DOUBLE *pkt_period);

PRIVATE void acb_ft_pkt_size_calc(
    acb_ft_handle_t *acb_ft_handle, 
    acb_ft_prov_cfg_t *cfg_ptr,
    UINT32 *pkt_size, 
    UINT32 *pkt_size_var, 
    UINT32 *fb_wordsize,
    INT32 *cnt_n,
    UINT32 *cnt_d);

PRIVATE void acb_ft_gain_calc(
    acb_ft_handle_t *acb_ft_handle, 
    acb_ft_prov_cfg_t *cfg_ptr,
    DOUBLE packet_period,
    UINT8 * int_gain, 
    UINT8 *filt_gain, 
    UINT8 *final_shift);

PRIVATE void acb_ft_pkt_period_cfg(
    acb_ft_handle_t *acb_ft_handle, 
    UINT32 pkt_period_int, 
    UINT32 pkt_period_d_msb, 
    UINT32 pkt_period_d_lsb, 
    UINT32 pkt_period_n_msb, 
    UINT32 pkt_period_n_lsb,
    UINT32 thermometer_int, 
    UINT32 thermometer_d, 
    UINT32 thermometer_n);

PRIVATE void acb_ft_pkt_size_cfg(acb_ft_handle_t  *acb_ft_handle, 
                                 UINT32            pkt_size, 
                                 UINT32            pkt_size_var,     
                                 UINT32            fb_wordsize,
                                 INT32             cnt_n,
                                 UINT32            cnt_d);

PRIVATE void acb_ft_enable(acb_ft_handle_t  *acb_ft_handle,
                           BOOL8             enable);

PRIVATE void acb_ft_pkt_period_conv(
    UINT32 pkt_period_int, 
    INT32 pkt_period_n, 
    UINT32 pkt_period_d, 
    DOUBLE *pkt_period);

PRIVATE void acb_ft_gain_cfg(
    acb_ft_handle_t *acb_ft_handle, 
    UINT8 int_gain, 
    UINT8 filt_gain, 
    UINT8 final_shift);

PRIVATE void acb_ft_max_ppm_set(acb_ft_handle_t *acb_ft_handle, 
                                UINT32           max_ppm,
                                UINT64           csu_n,
                                UINT64           csu_d,
                                UINT32           csu_r,
                                UINT32          *pll_s_max);

PRIVATE void acb_ft_sfi51_max_ppm_set(acb_ft_handle_t *acb_ft_handle,
                                      UINT32          *pll_s_max);

PRIVATE void acb_ft_context_get(acb_ft_handle_t  *acb_ft_handle,
                                     UINT8        ctxt_idx);

PRIVATE void acb_ft_s16_loop_timing_int_deactivate(acb_ft_handle_t *acb_ft_handle);

PRIVATE void acb_ft_context_read(acb_ft_handle_t        *acb_ft_handle,
                                 acb_ft_ctxt_t          *acb_ft_ctxt_ptr);

PRIVATE PMC_ERROR acb_ft_context_write(acb_ft_handle_t        *acb_ft_handle,
                                       acb_ft_ctxt_t          *acb_ft_ctxt_ptr);

/*
** Public Functions
*/

/*******************************************************************************
* acb_ft_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a ACB_FT block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the ACB_FT subsystem relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifier of the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   acb_ft_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC acb_ft_handle_t *acb_ft_ctxt_create(pmc_handle_t *parent, 
                       UINT32 base_address, 
                       pmc_sys_handle_t *sys_handle, 
                       const char *tsb_name)
{
    acb_ft_handle_t *acb_ft_handle;

    PMC_ENTRY();
    acb_ft_handle = (acb_ft_handle_t *)PMC_CTXT_CALLOC(sizeof(acb_ft_handle_t), parent);
    
    pmc_handle_init(parent, acb_ft_handle, sys_handle, PMC_MID_DIGI_ACB_FT, tsb_name, base_address);
    
    /* Register log strings. */
    pmc_log_block_strings_register(ACB_FT_LOG_ERR_STRINGS[0], ACB_FT_LOG_ERR_TABLE_BASE, ACB_FT_LOG_ERR_COUNT);

    acb_ft_handle->var.ctxt = (acb_ft_ctxt_t *)PMC_CTXT_CALLOC(sizeof(acb_ft_ctxt_t) * ACB_FT_MAX_NUM_CTXT, parent);
    PMC_CTXT_REGISTER_PTR(&acb_ft_handle->var.ctxt, parent);

    PMC_RETURN(acb_ft_handle);

} /* acb_ft_ctxt_create */


/*******************************************************************************
* acb_ft_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a ACB_FT block instance.
*
*
* INPUTS:
*   *acb_ft_handle         - pointer to ACB_FT handle instance
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
PUBLIC void acb_ft_ctxt_destroy(acb_ft_handle_t *acb_ft_handle)
{
    PMC_ENTRY();

    PMC_CTXT_FREE(&acb_ft_handle->var.ctxt, acb_ft_handle); 
   
    PMC_CTXT_FREE(&acb_ft_handle, acb_ft_handle);

    PMC_RETURN();    
} /* acb_ft_ctxt_destroy */

/*******************************************************************************
* acb_ft_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an ACB_FT block handle instance.
*
*
* INPUTS:
*   *acb_ft_handle          - pointer to ACB_FT handle instance
*   parent                  - the parent of ths blk
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
PUBLIC void acb_ft_handle_init(acb_ft_handle_t *acb_ft_handle, 
                               acb_ft_parent_t parent)
{
    /* Variable declaration */
    
    PMC_ENTRY();
    
    acb_ft_var_default_init(acb_ft_handle);
    
    PMC_RETURN();
    
} /* acb_ft_handle_init */

/*******************************************************************************
* acb_ft_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on acb_ft.
*
* INPUTS:
*   *acb_ft_handle              - pointer to acb_ft handle instance
*   *restart_init_cfg_ptr       - pointer to restart init config
*   top_energy_state_reg        - The expected energy state 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS : 
*     WARM : register/context are coherent
*     CRASH: everything is coherent (a cleanup may has been executed)
*   
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR acb_ft_handle_restart_init(acb_ft_handle_t                *acb_ft_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                            pmc_energy_state_t              top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();

    PMC_ASSERT(NULL != acb_ft_handle, ACB_FT_ERR_INVALID_PARAMETERS, 0, 0); 
    
    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(ACB_FT_LOG_ERR_STRINGS[0], ACB_FT_LOG_ERR_TABLE_BASE, ACB_FT_LOG_ERR_COUNT);
    }

    PMC_RETURN(ret_val);

} /* acb_ft_handle_restart_init */

/*******************************************************************************
* acb_ft_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of an ACB_FT block instance.
*
*
* INPUTS:
*   *acb_ft_handle           - pointer to ACB_FT handle instance.
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
PUBLIC UINT32 acb_ft_base_address_get(acb_ft_handle_t *acb_ft_handle)
{
    PMC_ENTRY();
    PMC_RETURN(acb_ft_handle->base.base_address);
} /* acb_ft_base_address_get */

/*******************************************************************************
* acb_ft_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the ACB_FT with the requested configuration. It 
*   evaluate if the requested configuration is coherent of and returns an error 
*   for a invalid configuration.  
*
* INPUTS:
*   *acb_ft_handle           - pointer to ACB_FT handle instance.
*   *cfg_ptr                 - pointer to ACB_FT provisioning configuration 
*                              structure.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise and descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR  acb_ft_prov(acb_ft_handle_t    *acb_ft_handle, 
                              acb_ft_prov_cfg_t  *cfg_ptr)
{
    UINT8  int_gain          = 0;
    UINT8  filt_gain         = 0;
    UINT8  final_shift       = 0;
    UINT32 pkt_period_int    = 0;
    UINT32 pkt_period_d_msb  = 0;
    UINT32 pkt_period_d_lsb  = 0;
    UINT32 pkt_period_n_msb  = 0;
    UINT32 pkt_period_n_lsb  = 0;
    DOUBLE pkt_period        = 0;
    UINT32 thermometer_int   = 0;
    UINT32 thermometer_d     = 0;
    UINT32 thermometer_n     = 0;
    UINT32 pkt_size          = 0;
    UINT32 pkt_size_var      = 0;
    UINT32 fb_wordsize       = 0;
    INT32  cnt_n             = 0;
    UINT32 cnt_d             = 0;

    PMC_ERROR rc = PMC_SUCCESS;    
        
    PMC_ENTRY();

    PMC_ASSERT(acb_ft_handle != NULL, ACB_FT_ERR_INVALID_PARAMETERS,0,0);
    
    if (ACB_FT_START != acb_ft_handle->var.state) 
    {
        PMC_RETURN(ACB_FT_STATE_NOT_START);
    }
        
    /* update var context */
    acb_ft_handle->var.mode      = cfg_ptr->mode;

    /* 
       resolve ACB configuration 
    */
    PMC_LOG_TRACE("ACB_FT mode = %d\n", (UINT32)acb_ft_handle->var.mode);
    if (ACB_FT_MODE_NOMINAL != acb_ft_handle->var.mode)
    {
        if (PMC_SUCCESS == rc) 
        {
            acb_ft_pkt_period_calc(acb_ft_handle,
                                   cfg_ptr,
                                   &pkt_period_int,
                                   &pkt_period_d_msb,
                                   &pkt_period_d_lsb,
                                   &pkt_period_n_msb, 
                                   &pkt_period_n_lsb,
                                   &thermometer_int,
                                   &thermometer_d,
                                   &thermometer_n,
                                   &pkt_period);  
        }
        if (PMC_SUCCESS == rc) 
        {
            acb_ft_pkt_size_calc(acb_ft_handle, cfg_ptr, &pkt_size, &pkt_size_var, &fb_wordsize, &cnt_n, &cnt_d);
        }
        if (PMC_SUCCESS == rc) 
        {
            acb_ft_gain_calc(acb_ft_handle,cfg_ptr,pkt_period, &int_gain, &filt_gain, &final_shift);
        }        

        PMC_LOG_TRACE("ACB_FT configuration\n");
        PMC_LOG_TRACE("pkt_period_int    = %d\n", pkt_period_int  );
        PMC_LOG_TRACE("pkt_period_d_msb  = %d\n", pkt_period_d_msb);
        PMC_LOG_TRACE("pkt_period_d_lsb  = %d\n", pkt_period_d_lsb);
        PMC_LOG_TRACE("pkt_period_n_msb  = %d (%u) \n", pkt_period_n_msb, (UINT32)pkt_period_n_msb);
        PMC_LOG_TRACE("pkt_period_n_lsb  = %d (%u) \n", pkt_period_n_lsb, (UINT32)pkt_period_n_lsb );
        PMC_LOG_TRACE("thermometer_int   = %d\n", thermometer_int );
        PMC_LOG_TRACE("thermometer_d     = %d\n", thermometer_d   );
        PMC_LOG_TRACE("thermometer_n     = %d\n", thermometer_n   );
        PMC_LOG_TRACE("pkt_period        = %f\n", pkt_period      );
        PMC_LOG_TRACE("pkt_size          = %d\n", pkt_size        );
        PMC_LOG_TRACE("pkt_size_var      = %d\n", pkt_size_var    );
        PMC_LOG_TRACE("fb_wordsize       = %d\n", fb_wordsize     );
        PMC_LOG_TRACE("cnt_n            = %d (%u)\n",cnt_n, (UINT32) cnt_n  );
        PMC_LOG_TRACE("cnt_d             = %d\n", cnt_d           );
        PMC_LOG_TRACE("int_gain          = %d\n", int_gain        );
        PMC_LOG_TRACE("filt_gain         = %d\n", filt_gain       );
        PMC_LOG_TRACE("final_shift       = %d\n", final_shift     );
    }
    if (PMC_SUCCESS == rc) 
    {    
        /* 
           Provision the ACB_FT 
        */
        acb_ft_init_reset(acb_ft_handle);
        
        /* Configure control */
        acb_ft_init_ctrl_cfg(acb_ft_handle);   
        
        if (ACB_FT_MODE_NOMINAL != acb_ft_handle->var.mode)
        {
            acb_ft_pkt_period_cfg( acb_ft_handle,
                                   pkt_period_int,
                                   pkt_period_d_msb,
                                   pkt_period_d_lsb,
                                   pkt_period_n_msb,
                                   pkt_period_n_lsb,
                                   thermometer_int,
                                   thermometer_d,
                                   thermometer_n);                              
            acb_ft_pkt_size_cfg(acb_ft_handle,
                                pkt_size, 
                                pkt_size_var, 
                                fb_wordsize, 
                                cnt_n, 
                                cnt_d);  
            acb_ft_gain_cfg(acb_ft_handle,int_gain, filt_gain, final_shift);
        }


        /* update block state */
        acb_ft_handle->var.state = ACB_FT_EQUIPPED;
    }     

    PMC_RETURN(rc);
} /* acb_ft_prov*/

/*******************************************************************************
* acb_ft_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovision the ACB_FT block
*
*
* INPUTS:
*   *acb_ft_handle           - pointer to ACB_FT handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise and descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR acb_ft_deprov(acb_ft_handle_t *acb_ft_handle)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(acb_ft_handle != NULL, ACB_FT_ERR_INVALID_PARAMETERS,0,0);
    if (ACB_FT_EQUIPPED !=  acb_ft_handle->var.state) 
    {
        PMC_RETURN(ACB_FT_STATE_NOT_EQUIPPED);
    }

    /* update block state */
    acb_ft_handle->var.state = ACB_FT_START;
    acb_ft_handle->var.mode  = LAST_ACB_FT_MODE;

    PMC_RETURN(rc);
} /* acb_ft_deprov */

/*******************************************************************************
* acb_ft_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Cleanup ACB FT context.
*
*
* INPUTS:
*   *acb_ft_handle           - pointer to ACB_FT handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void acb_ft_cleanup(acb_ft_handle_t *acb_ft_handle)
{

    PMC_ENTRY();

    /* update block state */
    acb_ft_handle->var.state = ACB_FT_START;
    acb_ft_handle->var.mode  = LAST_ACB_FT_MODE;

    PMC_RETURN();
} /* acb_ft_cleanup */

/*******************************************************************************
* acb_ft_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activate the ACB_FT block
*
* INPUTS:
*   *acb_ft_handle           - pointer to ACB_FT handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise and descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR acb_ft_activate(acb_ft_handle_t *acb_ft_handle)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(acb_ft_handle != NULL, ACB_FT_ERR_INVALID_PARAMETERS,0,0);
    if (ACB_FT_EQUIPPED !=  acb_ft_handle->var.state) 
    {
        PMC_RETURN(ACB_FT_STATE_NOT_EQUIPPED);
    }

    acb_ft_enable(acb_ft_handle, TRUE);

    /* update block state */
    acb_ft_handle->var.state = ACB_FT_OPERATIONAL;

    PMC_RETURN(rc);
} /* acb_ft_activate */

/*******************************************************************************
* acb_ft_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivate the ACB_FT block
*
* INPUTS:
*   *acb_ft_handle           - pointer to ACB_FT handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise and descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR acb_ft_deactivate(acb_ft_handle_t *acb_ft_handle)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(acb_ft_handle != NULL, ACB_FT_ERR_INVALID_PARAMETERS,0,0);
    if (ACB_FT_OPERATIONAL != acb_ft_handle->var.state) 
    {
        PMC_RETURN(PMC_SUCCESS);
    }

    acb_ft_enable(acb_ft_handle, FALSE);
    
    /* update block state */
    acb_ft_handle->var.state = ACB_FT_EQUIPPED;

    PMC_RETURN(rc);

} /*acb_ft_deactivate  */

/*******************************************************************************
* acb_ft_restart
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivate the ACB_FT block
*
* INPUTS:
*   *acb_ft_handle           - pointer to ACB_FT handle instance.
*    is_s16_parent           - TRUE when acb_ft is in s16 serdes, 
*                              FALSE when it belongs to a sfi51_d8_tx_18x.
* OUTPUTS:
*   None.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void acb_ft_restart(acb_ft_handle_t *acb_ft_handle,
                           BOOL8            is_s16_parent)
{

    PMC_ENTRY();

    PMC_ASSERT(acb_ft_handle != NULL, ACB_FT_ERR_INVALID_PARAMETERS,0,0);

    if (TRUE == is_s16_parent)
    {
        acb_ft_s16_loop_timing_int_deactivate(acb_ft_handle);
    }

    acb_ft_enable(acb_ft_handle, FALSE);
    
    /* update block state */
    acb_ft_handle->var.state = ACB_FT_START;
    acb_ft_handle->var.mode  = LAST_ACB_FT_MODE;

    PMC_RETURN();

} /*acb_ft_restart  */

/*******************************************************************************
* acb_ft_context_push
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function stores the current context (all computed fields). This function 
*   shall be called when the block is in ACB_FT_EQUIPPED state. This function 
*   does not modify the state of the HW block. acb_ft_context_push can be called to 
*   restore stored context.
*  
* INPUTS:
*   *acb_ft_handle           - pointer to ACB_FT handle instance.
*    ctxt_idx                - ACB FT context index.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise and descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR acb_ft_context_push(acb_ft_handle_t *acb_ft_handle,
                                     UINT8            ctxt_idx)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(acb_ft_handle != NULL, ACB_FT_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(ctxt_idx < ACB_FT_MAX_NUM_CTXT, ACB_FT_ERR_INVALID_PARAMETERS, 0, 0);

    if (ACB_FT_EQUIPPED !=  acb_ft_handle->var.state) 
    {
        PMC_RETURN(ACB_FT_STATE_NOT_EQUIPPED);
    }

    if (FALSE != acb_ft_handle->var.ctxt[ctxt_idx].active)
    {
        PMC_RETURN(ACB_FT_CONTEXT_ALREADY_STORED);
    }
    /* allocate the context */
    acb_ft_handle->var.ctxt[ctxt_idx].active = TRUE;
    /* read current context */
    acb_ft_context_get(acb_ft_handle, ctxt_idx);
    acb_ft_handle->var.ctxt[ctxt_idx].mode = acb_ft_handle->var.mode;

    PMC_RETURN(rc);
} /* acb_ft_context_push */

/*******************************************************************************
* acb_ft_context_pop
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function restores a stored context 
*  
* INPUTS:
*   *acb_ft_handle           - pointer to ACB_FT handle instance.
*    ctxt_idx                - ACB FT context index.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise and descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR acb_ft_context_pop(acb_ft_handle_t  *acb_ft_handle,
                                    UINT8             ctxt_idx)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(acb_ft_handle != NULL, ACB_FT_ERR_INVALID_PARAMETERS,0,0);
    if (ACB_FT_EQUIPPED !=  acb_ft_handle->var.state) 
    {
        PMC_RETURN(ACB_FT_STATE_NOT_EQUIPPED);
    }
    if (FALSE == acb_ft_handle->var.ctxt[ctxt_idx].active)
    {
        PMC_RETURN(ACB_FT_CONTEXT_NOT_AVAILABLE);
    }
    
    acb_ft_handle->var.mode = acb_ft_handle->var.ctxt[ctxt_idx].mode;    
    PMC_LOG_TRACE("ACB_FT mode = %d\n", (UINT32)acb_ft_handle->var.mode);


    acb_ft_init_reset(acb_ft_handle);
    
    /* Configure control */
    acb_ft_init_ctrl_cfg(acb_ft_handle);   

    acb_ft_context_write(acb_ft_handle, &acb_ft_handle->var.ctxt[ctxt_idx]);
    
    acb_ft_handle->var.ctxt[ctxt_idx].active = FALSE;

    PMC_RETURN(rc);
}/* acb_ft_context_pop */


/*******************************************************************************
* acb_ft_context_write
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function restores a stored context.
*  
* INPUTS:
*   *acb_ft_handle          - pointer to ACB_FT handle instance.
*   acb_ft_ctxt_ptr         - pointer to context that stores parameters values 
*                             to write in register space.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise and descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR acb_ft_context_write(acb_ft_handle_t       *acb_ft_handle,
                                       acb_ft_ctxt_t         *acb_ft_ctxt_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(acb_ft_handle != NULL, ACB_FT_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(acb_ft_ctxt_ptr != NULL, ACB_FT_ERR_INVALID_PARAMETERS,0,0);

    if (ACB_FT_EQUIPPED !=  acb_ft_handle->var.state) 
    {
        PMC_RETURN(ACB_FT_STATE_NOT_EQUIPPED);
    }
    
    acb_ft_pkt_period_cfg( acb_ft_handle,
                           acb_ft_ctxt_ptr->pkt_period_int,
                           acb_ft_ctxt_ptr->pkt_period_d_msb,
                           acb_ft_ctxt_ptr->pkt_period_d_lsb,
                           acb_ft_ctxt_ptr->pkt_period_n_msb,
                           acb_ft_ctxt_ptr->pkt_period_n_lsb,
                           acb_ft_ctxt_ptr->thermometer_int,
                           acb_ft_ctxt_ptr->thermometer_d,
                           acb_ft_ctxt_ptr->thermometer_n);   

    acb_ft_pkt_size_cfg(acb_ft_handle,
                        acb_ft_ctxt_ptr->pkt_size,
                        acb_ft_ctxt_ptr->pkt_size_var, 
                        acb_ft_ctxt_ptr->fb_wordsize, 
                        acb_ft_ctxt_ptr->cnt_n, 
                        acb_ft_ctxt_ptr->cnt_d);  
    acb_ft_gain_cfg(acb_ft_handle,
                    acb_ft_ctxt_ptr->int_gain, 
                    acb_ft_ctxt_ptr->filt_gain, 
                    acb_ft_ctxt_ptr->final_shift);

    acb_ft_field_PLL_S_MAX_set(NULL,acb_ft_handle, acb_ft_ctxt_ptr->pll_s_max); 
    
    
    PMC_RETURN(rc);
}/* acb_ft_context_write */


/*******************************************************************************
* acb_ft_flush_context
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function delete a stored context. 
*  
* INPUTS:
*   *acb_ft_handle           - pointer to ACB_FT handle instance.
*    ctxt_idx                - ACB FT context index.
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
PUBLIC void acb_ft_flush_context(acb_ft_handle_t  *acb_ft_handle,
                                 UINT8             ctxt_idx)
{
    PMC_ENTRY();
    
    PMC_ASSERT(acb_ft_handle != NULL, ACB_FT_ERR_INVALID_PARAMETERS,0,0);
    /* delete stored context */
    if (FALSE != acb_ft_handle->var.ctxt[ctxt_idx].active) 
    {
        acb_ft_handle->var.ctxt[ctxt_idx].active = FALSE;
    }

    PMC_RETURN();
}

/*******************************************************************************
* acb_ft_context_copy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function copies one context from one location to another.
*  
* INPUTS:
*   *acb_ft_handle           - pointer to ACB_FT handle instance.
*    from_ctxt_idx           - source ACB FT context index
*    to_ctxt_idx             - destination ACB FT context index
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
PUBLIC void acb_ft_context_copy(acb_ft_handle_t  *acb_ft_handle, 
                                UINT8             from_ctxt_idx, 
                                UINT8             to_ctxt_idx)
{    
    PMC_ENTRY();

    PMC_ASSERT(acb_ft_handle != NULL, ACB_FT_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(from_ctxt_idx < ACB_FT_MAX_NUM_CTXT, ACB_FT_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(to_ctxt_idx < ACB_FT_MAX_NUM_CTXT, ACB_FT_ERR_INVALID_PARAMETERS, 0, 0);
    
    PMC_MEMCPY(&acb_ft_handle->var.ctxt[to_ctxt_idx], &acb_ft_handle->var.ctxt[from_ctxt_idx], sizeof(acb_ft_ctxt_t));

    PMC_RETURN();
}/*acb_ft_context_copy*/


/*******************************************************************************
* acb_ft_provisionned
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function return TRUE when this ACB_FT instance is provisionned
*  
* INPUTS:
*   *acb_ft_handle           - pointer to ACB_FT handle instance.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   TRUE when block is provisionned
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 acb_ft_provisionned(acb_ft_handle_t *acb_ft_handle)
{
    BOOL8 result = FALSE;
    PMC_ENTRY();
    
    if (ACB_FT_OPERATIONAL ==  acb_ft_handle->var.state) 
    {
        result = TRUE;
    }

    PMC_RETURN(result);
}
/*******************************************************************************
*  acb_ft_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize the ACB_FT block dynamic context to its initial value
*
* INPUTS:
*   *acb_ft_handle           - pointer to ACB_FT handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void acb_ft_var_default_init(acb_ft_handle_t *acb_ft_handle)
{
    UINT8 itr;

    PMC_ENTRY();

    acb_ft_handle->var.state     = ACB_FT_START;
    acb_ft_handle->var.mode      = LAST_ACB_FT_MODE;

    for (itr =0; itr < ACB_FT_MAX_NUM_CTXT; itr++)
    {
        acb_ft_handle->var.ctxt[itr].active      = FALSE;
    }

    PMC_RETURN();
}

/*******************************************************************************
* acb_ft_pkt_period_conv
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function return the packet period in the DOUBLE notation
*
* INPUTS:
*   pkt_period_int          - interger part of the packet period
*   pkt_period_n            - numerator of the packet period
*   pkt_period_d            - denominator of the packet pereiod 

* OUTPUTS:
*    pkt_period             - packet period in double notation
*
* RETURNS:
*    None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void acb_ft_pkt_period_conv(
    UINT32 pkt_period_int, 
    INT32 pkt_period_n, 
    UINT32 pkt_period_d, 
    DOUBLE *pkt_period)
{
    PMC_ENTRY();
    
    PMC_ASSERT(pkt_period_d != 0, ACB_FT_ERR_INVALID_PARAMETERS, 0, 0);

    if (!digi_use_float32_get())
    {
        *pkt_period = (DOUBLE) pkt_period_int + (DOUBLE) pkt_period_n / (DOUBLE) pkt_period_d;
    }
    else
    {
        *pkt_period = (DOUBLE) ((FLOAT)pkt_period_int + (FLOAT) pkt_period_n / (FLOAT) pkt_period_d);
    }
    
    PMC_RETURN();
}

/*******************************************************************************
* acb_ft_pkt_size_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function calculates  packet size and the feedback clock parameters 
*   from the acb_ft configruation
* 
* INPUTS:
*   acb_ft_handle           - pointer to ACB_FT handle instance
*   cfg_ptr                 - pointer to SCB_FT provisionning configuration
* 
* OUTPUTS:
*  *pkt_size                - packet size
*  *pkt_size_var            - packet size variation 
*  *fb_wordsize             - feedback clock word size
*  *cnt_n                   - feedback clock counter DSM numerator 
*  *cnt_d                   - feedback clock counter DSM denominator 
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void acb_ft_pkt_size_calc(acb_ft_handle_t    *acb_ft_handle, 
                                  acb_ft_prov_cfg_t  *cfg_ptr,
                                  UINT32             *pkt_size, 
                                  UINT32             *pkt_size_var,
                                  UINT32             *fb_wordsize,
                                  INT32              *cnt_n,
                                  UINT32             *cnt_d)
{   
    DOUBLE cnt_nd;
    FLOAT f_cnt_nd;
    BOOL8 use_float32_mode = FALSE;
    
    PMC_ENTRY();

    use_float32_mode = digi_use_float32_get();

    if (ACB_FT_MODE_LINE_TIMING_125MHz == cfg_ptr->mode  ||
        ACB_FT_MODE_LINE_TIMING_PGMRCLK == cfg_ptr->mode)
    {    
        *pkt_size_var = 1;
    } else 
    {
        *pkt_size_var = 0;
    }

    if (ACB_FT_MODE_LOOP_TIMING == cfg_ptr->mode ||        
        ACB_FT_MODE_LINE_TIMING_125MHz == cfg_ptr->mode ||
        ACB_FT_MODE_LINE_TIMING_PGMRCLK == cfg_ptr->mode)
    {
        *fb_wordsize = 0;
    }
    else 
    {
        if (ACB_FT_SERDES_8b == cfg_ptr->serdes_if)
        {
            *fb_wordsize = ((cfg_ptr->num_lane * 2)/4) - 1;
        } else 
        {
            if (0 != cfg_ptr->num_lane)
            {
                *fb_wordsize = (cfg_ptr->num_lane * (UINT32) cfg_ptr->serdes_if) - 1;
            }
            else
            {
                *fb_wordsize = 0;
            }
        }
    }

    if (ACB_FT_MODE_LINE_TIMING_125MHz == cfg_ptr->mode)
    {
         *pkt_size = ACB_FT_PTP_PKT_SIZE;
    }
    else if ( ACB_FT_MODE_LINE_TIMING_PGMRCLK == cfg_ptr->mode)
    {
        *pkt_size = cfg_ptr->packet_size_C*4;
    }
    else if ( ACB_FT_MODE_B_ON_C == cfg_ptr->mode)
    {
        *pkt_size = cfg_ptr->packet_size_B;
    }
    else 
    {
        *pkt_size = cfg_ptr->packet_size_C;
    }
    /*compute cnt_n, cnt_d values */
    if (ACB_FT_MODE_LINE_TIMING_125MHz == cfg_ptr->mode)
    {
        *cnt_n = 0xffffffff;
        *cnt_d = 33;
    } 
    else if (ACB_FT_MODE_LOOP_TIMING == cfg_ptr->mode || 
             ACB_FT_MODE_LINE_TIMING_PGMRCLK == cfg_ptr->mode)
    {
        *cnt_n = 0;
        *cnt_d = 1;
    }
    else 
    {        
        if (cfg_ptr->client_type != ACB_FT_OTN && 
            cfg_ptr->client_type != ACB_FT_OTN_ODU4_ODU3)
        {
            if (!use_float32_mode)
            {
                cnt_nd = (cfg_ptr->capture_rate/cfg_ptr->transmit_rate);
                if (ACB_FT_SERDES_20b == cfg_ptr->serdes_if ) 
                {
                    PMC_LOG_TRACE("cnt_nd: %f, cfg_ptr->serdes_if:%d \n", cnt_nd, cfg_ptr->serdes_if);
                    cnt_nd *= 10.0/8.0;
                    PMC_LOG_TRACE("cnt_nd %f\n", cnt_nd);
                }
                util_global_fract_to_dec_conv(cnt_nd, (UINT32*) cnt_n, cnt_d);
                /* minus 1 */
                *cnt_n = (*cnt_n - *cnt_d);
            }
            else
            {
                f_cnt_nd = ((FLOAT)cfg_ptr->capture_rate/(FLOAT)cfg_ptr->transmit_rate);
                if (ACB_FT_SERDES_20b == cfg_ptr->serdes_if) 
                {
                    PMC_LOG_TRACE("cnt_nd: %f, cfg_ptr->serdes_if:%d \n", f_cnt_nd, cfg_ptr->serdes_if);
                    f_cnt_nd *= 10.0/8.0;
                    PMC_LOG_TRACE("cnt_nd %f\n", f_cnt_nd);
                }
                util_global_fract_to_dec_conv((DOUBLE)f_cnt_nd, (UINT32*) cnt_n, cnt_d);
                /* minus 1 */
                *cnt_n = (*cnt_n - *cnt_d);
            }
        }
        else
        {

            if (!use_float32_mode)
            {
                cnt_nd = (cfg_ptr->capture_rate/cfg_ptr->transmit_rate);
                /* // this condition can happen only when serdes is used as a clock otherwise */
                /* // either ACB_FT_SERDES_20b == cfg_ptr->serdes_if && 20 == data_cfg_bus_width */
                /* // or ACB_FT_SERDES_20b != cfg_ptr->serdes_if and the condition witll not the TRUE */
                if (ACB_FT_SERDES_20b == cfg_ptr->serdes_if && 
                    ACB_FT_SERDES_32b == cfg_ptr->master_serdes_if)
                {
                    PMC_LOG_TRACE("cnt_nd: %f, cfg_ptr->serdes_if:%d \n", cnt_nd, cfg_ptr->serdes_if);
                    cnt_nd *= 20;
                    cnt_nd /= 32;
                    PMC_LOG_TRACE("cnt_nd %f\n", cnt_nd);
                    if (0 != cfg_ptr->num_lane)
                    {
                        *fb_wordsize = (cfg_ptr->num_lane * (UINT32) cfg_ptr->master_serdes_if) - 1;
                    }                        
                }
                util_global_fract_to_dec_conv(cnt_nd, (UINT32*) cnt_n, cnt_d);
                /* minus 1 */
                *cnt_n = (*cnt_n - *cnt_d);
            }
            else
            {
                f_cnt_nd = ((FLOAT)cfg_ptr->capture_rate/(FLOAT)cfg_ptr->transmit_rate);
                /* // this condition can happen only when serdes is used as a clock otherwise */
                /* // either ACB_FT_SERDES_20b == cfg_ptr->serdes_if && 20 == data_cfg_bus_width */
                /* // or ACB_FT_SERDES_20b != cfg_ptr->serdes_if and the condition witll not the TRUE */
                if (ACB_FT_SERDES_20b == cfg_ptr->serdes_if && 
                    ACB_FT_SERDES_32b == cfg_ptr->master_serdes_if)
                {
                    PMC_LOG_TRACE("cnt_nd: %f, cfg_ptr->serdes_if:%d \n", f_cnt_nd, cfg_ptr->serdes_if);
                    f_cnt_nd *= (20 / ((FLOAT)32));
                    PMC_LOG_TRACE("cnt_nd %f\n", f_cnt_nd);
                    if (0 != cfg_ptr->num_lane)
                    {
                        *fb_wordsize = (cfg_ptr->num_lane * (UINT32) cfg_ptr->master_serdes_if) - 1;
                    }
                }
                util_global_fract_to_dec_conv((DOUBLE)f_cnt_nd, (UINT32*) cnt_n, cnt_d);
                /* minus 1 */
                *cnt_n = (*cnt_n - *cnt_d);
            }
        }
    }

    PMC_RETURN();
} /*acb_ft_pkt_size_calc*/

/*******************************************************************************
* acb_ft_pkt_period_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function computes  packet period and thermometer values base on the 
*   configuration
*
* INPUTS:
*   acb_ft_handle           - pointer to ACB_FT handle instance
*   cfg_ptr                 - pointer to ACB_FT provisoning configuration
* 
* OUTPUTS:
*   *pkt_period_int          - pkt period (interger part)
*   *pkt_period_d_msb        - pkt period (denominator MSB)
*   *pkt_period_d_lsb        - pkt period (denominator LSB)
*   *pkt_period_n_msb        - pkt period (numerator MSB)
*   *pkt_period_n_lsb        - pkt period (numerator LSB)
*   *thermometer_int         - thermometer integer value
*   *thermometer_d           - thermometer denominator value
*   *thermometer_n           - thermometer numerator value
*   *pkt_period             - pkt period in DOUBLE notation 
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void acb_ft_pkt_period_calc(acb_ft_handle_t   *acb_ft_handle, 
                                    acb_ft_prov_cfg_t *cfg_ptr, 
                                    UINT32            *pkt_period_int, 
                                    UINT32            *pkt_period_d_msb, 
                                    UINT32            *pkt_period_d_lsb, 
                                    UINT32            *pkt_period_n_msb, 
                                    UINT32            *pkt_period_n_lsb, 
                                    UINT32            *thermometer_int, 
                                    UINT32            *thermometer_d, 
                                    UINT32            *thermometer_n,
                                    DOUBLE            *pkt_period)
{
    DOUBLE Tc;
    DOUBLE Tb;
    DOUBLE Tca;
    DOUBLE Gt;
    DOUBLE nominal_epsilon;
    FLOAT f_nominal_epsilon;
    BOOL8 is_signed;
    
    PMC_ENTRY();
    *pkt_period_n_msb = 0;
    *pkt_period_d_msb = 0;

    /* thermometer is 0 except for B + C mode */
    *thermometer_int = 0;
    *thermometer_d = 0; 
    *thermometer_n = 0;
    
    switch(cfg_ptr->mode) 
    {
    case ACB_FT_MODE_C : 
    case ACB_FT_MODE_LOOP_TIMING:
    case ACB_FT_MODE_LINE_TIMING_125MHz:
    case ACB_FT_MODE_LINE_TIMING_PGMRCLK:
    {
        *pkt_period_int   = cfg_ptr->packet_per_int_C;
        *pkt_period_d_lsb = cfg_ptr->packet_per_d_C;
        if ((INT32) cfg_ptr->packet_per_n_C < 0)
        {
            *pkt_period_n_msb = 0x1FF;
        }
        *pkt_period_n_lsb = cfg_ptr->packet_per_n_C;
        acb_ft_pkt_period_conv(cfg_ptr->packet_per_int_C,cfg_ptr->packet_per_n_C,cfg_ptr->packet_per_d_C,&Tc);
        *pkt_period = Tc;
        break;
    }
    case ACB_FT_MODE_B_ON_C : 
    {
        *pkt_period_int   = cfg_ptr->packet_per_int_B;
        *pkt_period_d_lsb = cfg_ptr->packet_per_d_B;
        if ((INT32) cfg_ptr->packet_per_n_B < 0)
        {
            *pkt_period_n_msb = 0x1FF;
        }
        *pkt_period_n_lsb = cfg_ptr->packet_per_n_B;
        acb_ft_pkt_period_conv(cfg_ptr->packet_per_int_B,cfg_ptr->packet_per_n_B,cfg_ptr->packet_per_d_B,&Tb);        
        *pkt_period = Tb;
        break;
    }
    case  ACB_FT_MODE_B_AND_C : 
    {
        PMC_ASSERT(cfg_ptr->packet_size_B != 0, ACB_FT_ERR_INVALID_PARAMETERS, 0, 0);
        acb_ft_pkt_period_conv(cfg_ptr->packet_per_int_B,cfg_ptr->packet_per_n_B,cfg_ptr->packet_per_d_B,&Tb);
        acb_ft_pkt_period_conv(cfg_ptr->packet_per_int_C,cfg_ptr->packet_per_n_C,cfg_ptr->packet_per_d_C,&Tc);
        
        PMC_LOG_TRACE("TB:%f\n", Tb); 
        PMC_LOG_TRACE("TC:%f\n", Tc); 

        if (!digi_use_float32_get())
        {
            /* nominal epsilon for the source server */
            nominal_epsilon = (cfg_ptr->server_rate * Tb)/(cfg_ptr->ref_clk*2*8.0) - cfg_ptr->packet_size_B;
            PMC_LOG_TRACE("nominal_epsilon:%f\n", nominal_epsilon); 
            /* calculate period for A+B mode */
            Tca = Tc* (( cfg_ptr->packet_size_B + nominal_epsilon)/cfg_ptr->packet_size_B);
            PMC_LOG_TRACE("Tca:%f\n", Tca); 
            is_signed = (Tca < 0 )?TRUE:FALSE;        
            *pkt_period_int   = PMC_ABS(PMC_FLOOR(Tca)); 
            PMC_LOG_TRACE("pkt_period_int:%d\n", *pkt_period_int);               
            /* compute donominator and numerator */
            util_global_fract_to_dec_conv((PMC_FABS(Tca) - *pkt_period_int), pkt_period_n_lsb, pkt_period_d_lsb);
            *pkt_period_n_msb = (is_signed == TRUE)? 0x1FF:0x00;
            *pkt_period = Tca;
            
            /* compute thermometer value */
            Gt = Tb/cfg_ptr->packet_size_B;
            PMC_LOG_TRACE("Gt:%f\n", Gt); 
            *thermometer_int = PMC_FLOOR(Gt);
            util_global_fract_to_dec_conv((Gt - *thermometer_int), thermometer_n, thermometer_d);
        }
        else
        {
            /* nominal epsilon for the source server */
            f_nominal_epsilon = ((FLOAT)cfg_ptr->server_rate * (FLOAT)Tb)/((FLOAT)cfg_ptr->ref_clk*2*8.0) - cfg_ptr->packet_size_B;
            PMC_LOG_TRACE("nominal_epsilon:%f\n", f_nominal_epsilon); 
            /* calculate period for A+B mode */
            Tca = (DOUBLE)((FLOAT)Tc* (( cfg_ptr->packet_size_B + f_nominal_epsilon)/cfg_ptr->packet_size_B));
            PMC_LOG_TRACE("Tca:%f\n", Tca); 
            is_signed = (Tca < 0 )?TRUE:FALSE;        
            *pkt_period_int   = PMC_ABS(PMC_FLOORF(Tca)); 
            PMC_LOG_TRACE("pkt_period_int:%d\n", *pkt_period_int);               
            /* compute donominator and numerator */
            util_global_fract_to_dec_conv((PMC_FABSF(Tca) - *pkt_period_int), pkt_period_n_lsb, pkt_period_d_lsb);
            *pkt_period_n_msb = (is_signed == TRUE)? 0x1FF:0x00;
            *pkt_period = Tca;
            
            /* compute thermometer value */
            Gt = (DOUBLE)((FLOAT)Tb/cfg_ptr->packet_size_B);
            PMC_LOG_TRACE("Gt:%f\n", Gt); 
            *thermometer_int = PMC_FLOORF(Gt);
            util_global_fract_to_dec_conv((Gt - *thermometer_int), thermometer_n, thermometer_d);
        }

        break;    
    }
    default:
    {
        PMC_ASSERT(FALSE, ACB_FT_ERR_INVALID_PARAMETERS, 0, 0);
        break;
    }
    }
    
    PMC_RETURN();
} /* acb_ft_pkt_period_calc */

/*******************************************************************************
* acb_ft_gain_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function calculate 3 gain parameters for PMM adaptation
*
* INPUTS:
*   acb_ft_handle           - pointer to ACB_FT handle instance
*   cfg_ptr                 - pointer to ACB_FT provisoning configuration
*   packet_period           - packet period in DOUBLE notation
* 
* OUTPUTS:
*   int_gain                - the gain of the integral path through the ACB_FT 
*                             control
*                             loop's loop filter...
*   filt_gain               - the bandwidth of the low pass filter in the phase 
*                             adaptation circuit
*   final_shift             - the value of the shift at the output of the control 
*                             loop
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void acb_ft_gain_calc(acb_ft_handle_t   *acb_ft_handle, 
                              acb_ft_prov_cfg_t *cfg_ptr,
                              DOUBLE             packet_period,
                              UINT8             *int_gain, 
                              UINT8             *filt_gain, 
                              UINT8             *final_shift) 
{
    DOUBLE outdiv;
    DOUBLE Kvco;
    DOUBLE Fvco;
    DOUBLE Fref;
    DOUBLE Fbyte;
    DOUBLE fc_actual;
    UINT64 denomin_max;
    UINT32 fc;
    
    PMC_ENTRY();

    PMC_ASSERT( cfg_ptr->transmit_rate != 0, ACB_FT_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT( cfg_ptr->csu_denomin != 0, ACB_FT_ERR_INVALID_PARAMETERS, 0, 0);

    /* Calculate final_shift */    
    fc = (UINT32) cfg_ptr->client_type;

    if (!digi_use_float32_get())
    {
        Fref = cfg_ptr->ref_clk;
        Fvco = Fref * cfg_ptr->csu_div;
        Fbyte = (cfg_ptr->capture_rate)/8;
        /* The overall divider ratio from the VCO to a byte/character rate clock. */
        outdiv = Fvco/Fbyte;  
        PMC_LOG_TRACE(  "Fvco: %f, Fbyte:%f\n",Fvco, Fbyte);

        denomin_max = ((UINT64) 0xFFFFFFFFFFULL / cfg_ptr->csu_denomin) * cfg_ptr->csu_denomin;
        PMC_LOG_TRACE("denomin_max %llu\n", denomin_max);
        
        Kvco = (Fref * PMC_POW(2,16))/ ((DOUBLE)denomin_max  * outdiv);
        PMC_LOG_TRACE(  "Kvco: %f\n",Kvco);
        
        *final_shift = PMC_FLOOR( PMC_MATH_LOG((fc*2*UTIL_OPSA_PI)/Kvco )/PMC_MATH_LOG(2));
        
        /* Calculate filt_gain */
        fc_actual = (PMC_POW(2,*final_shift) * Kvco)/ (2 * UTIL_OPSA_PI);        
        *filt_gain = PMC_FLOOR( PMC_MATH_LOG( (cfg_ptr->ref_clk*2) /(10*UTIL_OPSA_PI*fc_actual*packet_period)  )/PMC_MATH_LOG(2) );
        
        if(*filt_gain > filt_gain_max)
            *filt_gain = filt_gain_max;
    }
    else
    {
        Fref = cfg_ptr->ref_clk;
        Fvco = (DOUBLE)((FLOAT)Fref * (FLOAT)cfg_ptr->csu_div);
        Fbyte = (DOUBLE)(((FLOAT)cfg_ptr->capture_rate)/8);
        /* The overall divider ratio from the VCO to a byte/character rate clock. */
        outdiv = (DOUBLE)((FLOAT)Fvco/(FLOAT)Fbyte);  
        PMC_LOG_TRACE(  "Fvco: %f, Fbyte:%f\n",Fvco, Fbyte);

        denomin_max = ((UINT64) 0xFFFFFFFFFFULL / cfg_ptr->csu_denomin) * cfg_ptr->csu_denomin;
        PMC_LOG_TRACE("denomin_max %llu\n", denomin_max);
        
        Kvco = (DOUBLE)(((FLOAT)Fref * PMC_POWF(2,16))/ (denomin_max  * (FLOAT)outdiv));
        PMC_LOG_TRACE(  "Kvco: %f\n",Kvco);
        
        *final_shift = PMC_FLOORF( PMC_MATH_LOGF((fc*2*UTIL_OPSA_PI)/(FLOAT)Kvco )/PMC_MATH_LOGF(2));
        
        /* Calculate filt_gain */
        fc_actual = (DOUBLE)((PMC_POWF(2,*final_shift) * (FLOAT)Kvco)/ (2 * UTIL_OPSA_PI));        
        *filt_gain = PMC_FLOORF( PMC_MATH_LOGF( ((FLOAT)cfg_ptr->ref_clk*2) /(10*UTIL_OPSA_PI*(FLOAT)fc_actual*packet_period)  )/PMC_MATH_LOGF(2) );
        
        if(*filt_gain > filt_gain_max)
            *filt_gain = filt_gain_max;
    }
    
    /* int_gain is always set to 0 */
    *int_gain = 0;

    PMC_RETURN();
} /* acb_ft_gain_calc */

/*******************************************************************************
* acb_ft_pkt_period_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configure computed packet period and thermometer values 
*
* INPUTS:
*   acb_ft_handle           - pointer to ACB_FT handle instance
*   pkt_period_int          - pkt period (interger part)
*   pkt_period_d_msb        - pkt period (denominator MSB)
*   pkt_period_d_lsb        - pkt period (denominator LSB)
*   pkt_period_n_msb        - pkt period (numerator MSB)
*   pkt_period_n_lsb        - pkt period (numerator LSB)
*   thermometer_int         - thermometer integer value
*   thermometer_d           - thermometer denominator value
*   thermometer_n           - thermometer numerator value
* OUTPUTS:
*   None.
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PRIVATE void acb_ft_pkt_period_cfg(
    acb_ft_handle_t *acb_ft_handle, 
    UINT32 pkt_period_int, 
    UINT32 pkt_period_d_msb, 
    UINT32 pkt_period_d_lsb, 
    UINT32 pkt_period_n_msb, 
    UINT32 pkt_period_n_lsb,
    UINT32 thermometer_int, 
    UINT32 thermometer_d, 
    UINT32 thermometer_n)
{
    acb_ft_buffer_t acb_ft_buffer[1];

    PMC_ENTRY();

    /* Initilize ACB buffer */
    acb_ft_buffer_init(acb_ft_buffer,acb_ft_handle);

    acb_ft_field_PKT_PERIOD_INT_set(acb_ft_buffer,acb_ft_handle,pkt_period_int);
    acb_ft_field_PKT_PERIOD_D_MSB_set(acb_ft_buffer,acb_ft_handle,pkt_period_d_msb);
    acb_ft_field_PKT_PERIOD_D_LSB_set(acb_ft_buffer,acb_ft_handle,pkt_period_d_lsb);
    acb_ft_field_PKT_PERIOD_N_MSB_set(acb_ft_buffer,acb_ft_handle,pkt_period_n_msb);
    acb_ft_field_PKT_PERIOD_N_LSB_set(acb_ft_buffer,acb_ft_handle,pkt_period_n_lsb);
    acb_ft_field_THERMOMETER_INT_set(acb_ft_buffer,acb_ft_handle,thermometer_int);
    acb_ft_field_THERMOMETER_N_set(acb_ft_buffer,acb_ft_handle,thermometer_n);
    acb_ft_field_THERMOMETER_D_set(acb_ft_buffer,acb_ft_handle,thermometer_d);
    acb_ft_buffer_flush( acb_ft_buffer );

    PMC_RETURN();
} /*acb_ft_pkt_period_cfg*/

/*******************************************************************************
* acb_ft_sfi51_loop_timing_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   acb_ft loop timing config for sfi5.1
*
* INPUTS:
*   acb_ft_handle           - pointer to ACB_FT handle instance
*   pkt_period_int          - pkt period (interger part)
*   pkt_size                - pkt size
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise and descriptive 
*   error code is returned.
*
* NOTES:
*   This function is used to enable loop timing when using lineotn/cbrc mld 
*   facility loopback.
*
*******************************************************************************/
PUBLIC PMC_ERROR acb_ft_sfi51_loop_timing_cfg(acb_ft_handle_t *acb_ft_handle, 
                                              UINT32 pkt_period_int, 
                                              UINT32 pkt_size) 
{
    PMC_ERROR rc = PMC_SUCCESS;
    acb_ft_buffer_t acb_ft_buffer[1];

    PMC_ENTRY();

    /* Initilize ACB buffer */

    PMC_ASSERT(acb_ft_handle != NULL, ACB_FT_ERR_INVALID_PARAMETERS,0,0);
    
    if (ACB_FT_START != acb_ft_handle->var.state) 
    {
        PMC_RETURN(ACB_FT_STATE_NOT_START);
    }
    acb_ft_handle->var.state = ACB_FT_EQUIPPED;
    acb_ft_handle->var.mode = ACB_FT_MODE_LOOP_TIMING;

    acb_ft_init_ctrl_cfg(acb_ft_handle);

    acb_ft_buffer_init(acb_ft_buffer,acb_ft_handle);

    acb_ft_field_FB_WORDSIZE_set(acb_ft_buffer,acb_ft_handle,0);
    acb_ft_field_FINAL_SHIFT_set(acb_ft_buffer,acb_ft_handle,8); /*default */
    acb_ft_field_INT_GAIN_set(acb_ft_buffer,acb_ft_handle,0); /* default is 8*/
    acb_ft_field_FILT_GAIN_set(acb_ft_buffer,acb_ft_handle,8); /* default */
    acb_ft_field_PKT_PERIOD_INT_set(acb_ft_buffer,acb_ft_handle,pkt_period_int);
    acb_ft_field_PKT_PERIOD_D_LSB_set(acb_ft_buffer,acb_ft_handle,1);
    acb_ft_field_PKT_SIZE_set(acb_ft_buffer,acb_ft_handle,pkt_size);
    acb_ft_field_CNT_D_set(acb_ft_buffer,acb_ft_handle,1);
    acb_ft_buffer_flush(acb_ft_buffer);

    PMC_RETURN(rc);
} /*acb_ft_sfi51_loop_timing_cfg*/

/*******************************************************************************
* acb_ft_s16_loop_timing_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   acb_ft loop timing config for s16
*
* INPUTS:
*   acb_ft_handle           - pointer to ACB_FT handle instance
*   
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise and descriptive 
*   error code is returned.
*
* NOTES:
*   This function is used to enable loop timing when using s16 facility, 
*   lineotn mld and cbrc mld loopbacks.
*
*******************************************************************************/
PUBLIC PMC_ERROR acb_ft_s16_loop_timing_activate(acb_ft_handle_t *acb_ft_handle)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(acb_ft_handle != NULL, ACB_FT_ERR_INVALID_PARAMETERS,0,0);
    if (ACB_FT_EQUIPPED !=  acb_ft_handle->var.state) 
    {
        PMC_RETURN(ACB_FT_STATE_NOT_EQUIPPED);
    }

    acb_ft_field_PMM_ADAPT_ENABLE_set(NULL,acb_ft_handle, 0);
    acb_ft_field_FIFO_ADAPT_ENABLE_set(NULL,acb_ft_handle, 0);
    acb_ft_field_FINAL_SHIFT_set(NULL,acb_ft_handle,8);     /* default */
    acb_ft_field_FILT_GAIN_set(NULL,acb_ft_handle,8);       /* default */
    acb_ft_field_PLL_S_MAX_set(NULL, acb_ft_handle, 19);
	
    acb_ft_field_PLL_S_MIN_set(NULL, acb_ft_handle, 3); 
    acb_ft_field_FIFO_ADAPT_ENABLE_set(NULL,acb_ft_handle, 1);

    /* update block state */
    acb_ft_handle->var.state = ACB_FT_OPERATIONAL;
    PMC_RETURN(rc);
} /*acb_ft_s16_loop_timing_activate */

/*******************************************************************************
* acb_ft_s16_loop_timing_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   acb_ft loop timing config for s16
*
* INPUTS:
*   acb_ft_handle           - pointer to ACB_FT handle instance
*   
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise and descriptive 
*   error code is returned.
*
* NOTES:
*   This function is used to enable loop timing when using s16 facility, 
*   lineotn mld and cbrc mld loopbacks.
*
*******************************************************************************/
PUBLIC PMC_ERROR acb_ft_s16_loop_timing_deactivate(acb_ft_handle_t *acb_ft_handle)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(acb_ft_handle != NULL, ACB_FT_ERR_INVALID_PARAMETERS,0,0);
    if (ACB_FT_OPERATIONAL != acb_ft_handle->var.state) 
    {
        PMC_RETURN(PMC_SUCCESS);
    }

    acb_ft_s16_loop_timing_int_deactivate(acb_ft_handle);

    /* update block state */
    acb_ft_handle->var.state = ACB_FT_EQUIPPED;

    PMC_RETURN(rc);
} /*acb_ft_s16_loop_timing_deactivate */

/*******************************************************************************
* acb_ft_s16_loop_timing_int_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   acb_ft loop timing config for s16
*
* INPUTS:
*   acb_ft_handle           - pointer to ACB_FT handle instance
*   
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*   This function is used to enable loop timing when using s16 facility, 
*   lineotn mld and cbrc mld loopbacks.
*
*******************************************************************************/
PRIVATE void acb_ft_s16_loop_timing_int_deactivate(acb_ft_handle_t *acb_ft_handle)
{

    PMC_ENTRY();

    PMC_ASSERT(acb_ft_handle != NULL, ACB_FT_ERR_INVALID_PARAMETERS,0,0);

    acb_ft_field_FIFO_ADAPT_ENABLE_set(NULL,acb_ft_handle, 0);
    acb_ft_field_PLL_S_MAX_set(NULL, acb_ft_handle, 18); /* default */
    acb_ft_field_PLL_S_MIN_set(NULL, acb_ft_handle, 3);  /* default */

    PMC_RETURN();
} /*acb_ft_s16_loop_timing_int_deactivate */

/*******************************************************************************
* acb_ft_gain_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures computed ACB again into register fields. 
*   
*
* INPUTS:
*   acb_ft_handle           - pointer to ACB_FT handle instance
*   int_gain                - the gain of the integral path through the ACB_FT 
*                             control
*                             loop's loop filter...
*   filt_gain               - the bandwidth of the low pass filter in the phase 
*                             adaptation circuit
*   final_shift             - the value of the shift at the output of the control 
*                             loop
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
PRIVATE void acb_ft_gain_cfg(
    acb_ft_handle_t *acb_ft_handle, 
    UINT8 int_gain, 
    UINT8 filt_gain, 
    UINT8 final_shift)
{
    acb_ft_buffer_t acb_ft_buffer[1]; 

    PMC_ENTRY();

    /* Initilize ACB buffer */
    acb_ft_buffer_init(acb_ft_buffer,acb_ft_handle); 
    acb_ft_field_INT_GAIN_set(acb_ft_buffer,acb_ft_handle,int_gain); 
    acb_ft_field_FILT_GAIN_set(acb_ft_buffer,acb_ft_handle,filt_gain); 
    acb_ft_field_FINAL_SHIFT_set(acb_ft_buffer,acb_ft_handle,final_shift);    
    /* the output Delta Sigma Modulator is on or off based if acb is in master mode */
    if (ACB_FT_MODE_LOOP_TIMING == acb_ft_handle->var.mode ||
        ACB_FT_MODE_LINE_TIMING_125MHz == acb_ft_handle->var.mode ||
        ACB_FT_MODE_LINE_TIMING_PGMRCLK == acb_ft_handle->var.mode)
    {
        acb_ft_field_FIFO_CENT_ENABLE_set(acb_ft_buffer,acb_ft_handle,FALSE); 
    } else 
    {
        acb_ft_field_FIFO_CENT_ENABLE_set(acb_ft_buffer,acb_ft_handle,TRUE); 
    }
    acb_ft_field_PMM_ADAPT_ENABLE_set(acb_ft_buffer,acb_ft_handle,FALSE); 
    
    acb_ft_buffer_flush( acb_ft_buffer ); 

    PMC_RETURN();
}


/*******************************************************************************
* acb_ft_pkt_size_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This function configures computed fb clock parameters
*
* INPUTS:
*  acb_ft_handle           - pointer to ACB_FT handle instance
*  pkt_size                - packet size
*  pkt_size_var            - packet size variation
*  fb_wordsize             - feedback clock word size
*  cnt_n                   - feedback clock counter DSM numerator 
*  cnt_d                   - feedback clock counter DSM denominator 
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
PRIVATE void acb_ft_pkt_size_cfg(acb_ft_handle_t  *acb_ft_handle, 
                                 UINT32            pkt_size, 
                                 UINT32            pkt_size_var,     
                                 UINT32            fb_wordsize,
                                 INT32             cnt_n,
                                 UINT32            cnt_d)
{
    acb_ft_buffer_t acb_ft_buffer[1];

    PMC_ENTRY();

    /* Initilize ACB buffer */
    acb_ft_buffer_init(acb_ft_buffer,acb_ft_handle);
    
    acb_ft_field_FB_WORDSIZE_set(acb_ft_buffer,acb_ft_handle,fb_wordsize);
    acb_ft_field_PKT_SIZE_set(acb_ft_buffer,acb_ft_handle,pkt_size);
    acb_ft_field_PKT_SIZE_VAR_set(acb_ft_buffer,acb_ft_handle,pkt_size_var);
    acb_ft_field_CNT_D_set(acb_ft_buffer,acb_ft_handle,cnt_d);
    acb_ft_field_CNT_N_set(acb_ft_buffer,acb_ft_handle,cnt_n);    
    acb_ft_buffer_flush( acb_ft_buffer );

    PMC_RETURN();
}

/*******************************************************************************
* acb_ft_init_ctrl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes ACB based on its operating mode
*
* INPUTS:
*   acb_ft_handle           - pointer to ACB_FT handle instance
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
PRIVATE void acb_ft_init_ctrl_cfg(acb_ft_handle_t *acb_ft_handle) 
{
    acb_ft_buffer_t acb_ft_buffer[1]; 

    BOOL8 b_interface_enable;
    BOOL8 b_ckctl_mode;
    BOOL8 c_ckctl_mode;
    BOOL8 force_nominal;
    
    PMC_ENTRY();
    
    /* Initilize ACB buffer */
    acb_ft_buffer_init(acb_ft_buffer,acb_ft_handle);
    force_nominal = FALSE;
    switch(acb_ft_handle->var.mode) 
    {
    case ACB_FT_MODE_C : 
    case ACB_FT_MODE_B_ON_C : 
    {
        b_interface_enable = FALSE;
        b_ckctl_mode = TRUE;
        c_ckctl_mode = TRUE;
        break;
    }   
    case  ACB_FT_MODE_B_AND_C : 
    {
        b_interface_enable = TRUE;
        b_ckctl_mode = TRUE;
        c_ckctl_mode = TRUE;
        break;
    }
    case  ACB_FT_MODE_LOOP_TIMING : 
    case  ACB_FT_MODE_LINE_TIMING_125MHz:
    case ACB_FT_MODE_LINE_TIMING_PGMRCLK:
    {
        b_interface_enable = FALSE;
        b_ckctl_mode = FALSE;
        c_ckctl_mode = FALSE;
        break;
    }   
    case  ACB_FT_MODE_NOMINAL : 
    {
        b_interface_enable = FALSE;
        b_ckctl_mode = FALSE;
        c_ckctl_mode = FALSE;
        force_nominal = TRUE;
        break;
    }
    default :
    {
        PMC_ASSERT(FALSE, ACB_FT_ERR_INVALID_PARAMETERS, 0, 0);
        break;
    }
    }
    
    acb_ft_field_B_INTERFACE_ENABLE_set(acb_ft_buffer,acb_ft_handle,b_interface_enable);
    acb_ft_field_C_CKCTL_MODE_set(acb_ft_buffer,acb_ft_handle,c_ckctl_mode);
    acb_ft_field_B_CKCTL_MODE_set(acb_ft_buffer,acb_ft_handle,b_ckctl_mode);
    acb_ft_field_FORCE_NOMINAL_set(acb_ft_buffer,acb_ft_handle,force_nominal);
    acb_ft_field_OUTPUT_UPDATE_RATE_set(acb_ft_buffer,acb_ft_handle,ACB_FT_OUTPUT_UPDATE_RATE);

    acb_ft_buffer_flush( acb_ft_buffer );
 
    PMC_RETURN();
}

/*******************************************************************************
* acb_ft_init_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function toggle the ACB_FT software reset. 
*
* INPUTS:
*   acb_ft_handle           - pointer to ACB_FT handle instance
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
PRIVATE void acb_ft_init_reset(acb_ft_handle_t *acb_ft_handle) 
{    
    PMC_ENTRY();
 
    acb_ft_field_SOFTWARE_RESET_set(NULL,acb_ft_handle,TRUE);
    acb_ft_field_SOFTWARE_RESET_set(NULL,acb_ft_handle,FALSE);
    
    PMC_RETURN();
} /* acb_ft_init_reset */

/*******************************************************************************
* acb_ft_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to enable/disable the ACB_FT block
*
* INPUTS:
*   acb_ft_handle           - pointer to ACB_FT handle instance.
*   enable                  - TRUE to enable the ACB_FT.
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
PRIVATE void acb_ft_enable(acb_ft_handle_t  *acb_ft_handle,
                           BOOL8             enable)
{
    acb_ft_mode_t mode = acb_ft_handle->var.mode;
    UINT32 accel_mode = 0;

    PMC_ENTRY();

    /* enable fifo cent accel mode only if loop timing is disabled */
    if (enable && ACB_FT_MODE_LOOP_TIMING != mode && ACB_FT_MODE_LINE_TIMING_125MHz != mode &&
        ACB_FT_MODE_LINE_TIMING_PGMRCLK != mode)
        accel_mode = 1;

    /* FIFO_CENT_ACCEL register should only be changed when PMM_ADAPT_ENABLE is de-asserted. */
    acb_ft_field_PMM_ADAPT_ENABLE_set(NULL,acb_ft_handle,0);
    acb_ft_field_FIFO_CENT_ACCEL_set(NULL,acb_ft_handle,accel_mode);

    if (enable)
        acb_ft_field_PMM_ADAPT_ENABLE_set(NULL,acb_ft_handle,1);
    
    PMC_RETURN();
} /*acb_ft_enable */


/*******************************************************************************
* acb_ft_max_ppm_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the maximum PPM of the ACB FT.
*
* INPUTS:
*   acb_ft_handle      - pointer to ACB_FT handle instance
*   max_ppm            - maximum PPM associated to ACB_FT
*   csu_n              - CSU PLL Fractional Multiplication Nominator
*   csu_d              - CSU PLL Fractional Multiplication     
*   csu_r              - CSU PLL Integer Multiplication    
*
* OUTPUTS:
*   pll_s_max          - calculated pll_s_max value
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void acb_ft_max_ppm_set(acb_ft_handle_t *acb_ft_handle, 
                               UINT32           max_ppm,
                               UINT64           csu_n,
                               UINT64           csu_d,
                               UINT32           csu_r,
                               UINT32          *pll_s_max)
{
    DOUBLE pll_s_max_tmp = 0.0;

    PMC_ENTRY();       

    if (0 != max_ppm)
    {
        
        PMC_LOG_TRACE("max_ppm = %d, csu_n = %lld, csu_d = %llu, csu_r = %d\n",
                      max_ppm,
                      csu_n, 
                      csu_d, 
                      csu_r);
        if (0 !=  (csu_n*csu_d+csu_r))
        {
            if (!digi_use_float32_get())
            {
                pll_s_max_tmp = PMC_CEIL(PMC_MATH_LOG(((DOUBLE)(((max_ppm * ((csu_r*csu_d)+csu_n))) / 65536)) / 1e6) / PMC_MATH_LOG(2));
            }
            else
            {
                pll_s_max_tmp = (DOUBLE)(PMC_CEILF(PMC_MATH_LOGF(((FLOAT)(((max_ppm * ((csu_r*csu_d)+csu_n))) / 65536)) / 1e6) / PMC_MATH_LOGF(2)));
            }
        }
    }

    if (0.0 == pll_s_max_tmp || 31 < (UINT32)pll_s_max_tmp)
    {
        pll_s_max_tmp = ACB_FT_PLL_S_MAX_DEF_VAL;
    }

    PMC_LOG_TRACE("PLL S MAX = %x\n", (UINT32)pll_s_max_tmp);

    *pll_s_max = (UINT32)pll_s_max_tmp;

    PMC_RETURN();
} /*acb_ft_max_ppm_set */

/*******************************************************************************
* acb_ft_sfi51_max_ppm_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the maximum PPM of SFI51 ACB FT.
*
* INPUTS:
*   acb_ft_handle      - pointer to ACB_FT handle instance  
*
* OUTPUTS:
*   pll_s_max          - calculated pll_s_max value
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void acb_ft_sfi51_max_ppm_set(acb_ft_handle_t *acb_ft_handle,
                                     UINT32          *pll_s_max)
{
    DOUBLE pll_s_max_tmp = 0.0;
    UINT32 fb_wordsize;
    UINT32 final_shift;

    PMC_ENTRY();       

    fb_wordsize = acb_ft_field_FB_WORDSIZE_get(NULL,acb_ft_handle);
    final_shift = acb_ft_field_FINAL_SHIFT_get(NULL,acb_ft_handle);

    PMC_LOG_TRACE("fb_wordsize = %x, final_shift = %x\n",
                  fb_wordsize, 
                  final_shift);
    if (!digi_use_float32_get())
    {
        pll_s_max_tmp = 4 + final_shift + PMC_CEIL(PMC_MATH_LOG(fb_wordsize) / PMC_MATH_LOG(2)); 
    }
    else
    {
        pll_s_max_tmp = (DOUBLE)(4 + final_shift + PMC_CEILF(PMC_MATH_LOGF(fb_wordsize) / PMC_MATH_LOGF(2))); 
    }

    if (31 < (UINT32)pll_s_max_tmp)
    {
        pll_s_max_tmp = ACB_FT_PLL_S_MAX_DEF_VAL;
    }

    PMC_LOG_TRACE("PLL S MAX = %x\n", (UINT32)pll_s_max_tmp);

    *pll_s_max = (UINT32)pll_s_max_tmp;

    PMC_RETURN();
} /*acb_ft_sfi51_max_ppm_set */

/*******************************************************************************
* acb_ft_pll_s_max_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the maximum PPM of the ACB FT.
*
* INPUTS:
*   acb_ft_handle      - pointer to ACB_FT handle instance
*   max_ppm            - maximum PPM associated to ACB_FT
*   csu_n              - CSU PLL Fractional Multiplication Nominator
*   csu_d              - CSU PLL Fractional Multiplication     
*   csu_r              - CSU PLL Integer Multiplication    
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
PUBLIC void acb_ft_pll_s_max_cfg(acb_ft_handle_t *acb_ft_handle, 
                                 UINT32           max_ppm,
                                 UINT64           csu_n,
                                 UINT64           csu_d,
                                 UINT32           csu_r)
{
    UINT32 pll_s_max_1 = 0;
    UINT32 pll_s_max_2 = 0;
    PMC_ENTRY();


    acb_ft_max_ppm_set(acb_ft_handle, max_ppm, csu_n, csu_d, csu_r, &pll_s_max_1);
    acb_ft_sfi51_max_ppm_set(acb_ft_handle, &pll_s_max_2);

    if (pll_s_max_1 > pll_s_max_2)
    {
        acb_ft_field_PLL_S_MAX_set(NULL, acb_ft_handle, pll_s_max_1);
    }
    else
    {
        acb_ft_field_PLL_S_MAX_set(NULL, acb_ft_handle, pll_s_max_2);
    }

    PMC_RETURN();
}



/*******************************************************************************
* acb_ft_context_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function read some cfg parameters from the register space and save it
*   in the driver context
*
* INPUTS:
*   acb_ft_handle           - pointer to ACB_FT handle instance.
*   ctxt_idx                - ACB FT context index.
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
PRIVATE void acb_ft_context_get(acb_ft_handle_t *acb_ft_handle,
                                UINT8            ctxt_idx)
{

    PMC_ENTRY();

    PMC_ASSERT(acb_ft_handle != NULL, ACB_FT_ERR_INVALID_PARAMETERS, 0, 0);

    acb_ft_context_read(acb_ft_handle, &acb_ft_handle->var.ctxt[ctxt_idx]);
    
    PMC_RETURN();
}

/*******************************************************************************
* acb_ft_context_read
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function read some configuration parameters from  ACB register space
*
* INPUTS:
*   acb_ft_handle           - pointer to ACB_FT handle instance.
*   acb_ft_ctxt_ptr         - pointer to context in which parameters will be stored.
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
PRIVATE void acb_ft_context_read(acb_ft_handle_t        *acb_ft_handle,
                                 acb_ft_ctxt_t          *acb_ft_ctxt_ptr)
{

    PMC_ENTRY();

    PMC_ASSERT(acb_ft_handle != NULL, ACB_FT_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(acb_ft_ctxt_ptr != NULL, ACB_FT_ERR_INVALID_PARAMETERS, 0, 0);

    acb_ft_ctxt_ptr->pkt_period_int   =  acb_ft_field_PKT_PERIOD_INT_get(NULL,acb_ft_handle);
    acb_ft_ctxt_ptr->pkt_period_d_msb = acb_ft_field_PKT_PERIOD_D_MSB_get(NULL,acb_ft_handle);
    acb_ft_ctxt_ptr->pkt_period_d_lsb = acb_ft_field_PKT_PERIOD_D_LSB_get(NULL,acb_ft_handle);
    acb_ft_ctxt_ptr->pkt_period_n_msb = acb_ft_field_PKT_PERIOD_N_MSB_get(NULL,acb_ft_handle);
    acb_ft_ctxt_ptr->pkt_period_n_lsb = acb_ft_field_PKT_PERIOD_N_LSB_get(NULL,acb_ft_handle);
    acb_ft_ctxt_ptr->thermometer_int  = acb_ft_field_THERMOMETER_INT_get(NULL,acb_ft_handle);
    acb_ft_ctxt_ptr->thermometer_n    = acb_ft_field_THERMOMETER_N_get(NULL,acb_ft_handle);
    acb_ft_ctxt_ptr->thermometer_d    = acb_ft_field_THERMOMETER_D_get(NULL,acb_ft_handle);

    acb_ft_ctxt_ptr->int_gain =    acb_ft_field_INT_GAIN_get(NULL,acb_ft_handle);
    acb_ft_ctxt_ptr->filt_gain =   acb_ft_field_FILT_GAIN_get(NULL,acb_ft_handle);
    acb_ft_ctxt_ptr->final_shift = acb_ft_field_FINAL_SHIFT_get(NULL,acb_ft_handle);
    acb_ft_ctxt_ptr->fb_wordsize = acb_ft_field_FB_WORDSIZE_get(NULL,acb_ft_handle);
    acb_ft_ctxt_ptr->pkt_size =    acb_ft_field_PKT_SIZE_get(NULL,acb_ft_handle);
    acb_ft_ctxt_ptr->pkt_size_var = acb_ft_field_PKT_SIZE_VAR_get(NULL,acb_ft_handle);
    acb_ft_ctxt_ptr->cnt_d =       acb_ft_field_CNT_D_get(NULL,acb_ft_handle);
    acb_ft_ctxt_ptr->cnt_n =       acb_ft_field_CNT_N_get(NULL,acb_ft_handle);
    acb_ft_ctxt_ptr->pll_s_max =   acb_ft_field_PLL_S_MAX_get(NULL,acb_ft_handle);
    
    PMC_RETURN();
}/*acb_ft_context_read*/

/*******************************************************************************
* acb_ft_reprov_pkt_size_configs
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to reprovision ACB configs in the event of an
*   "on-the_fly" packet size change.  This API may be used on an operational
*   channel without the need to deprovision and reprovision the channel.
*
* INPUTS:
*   *acb_ft_handle           - pointer to ACB_FT handle instance.
*   *cfg_ptr                 - pointer to ACB_FT provisioning configuration 
*                              structure
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
PUBLIC PMC_ERROR  acb_ft_reprov_pkt_size_configs(acb_ft_handle_t    *acb_ft_handle, 
                                                 acb_ft_prov_cfg_t  *cfg_ptr)
{
    UINT8  int_gain          = 0;
    UINT8  filt_gain         = 0;
    UINT8  final_shift       = 0;
    UINT32 pkt_period_int    = 0;
    UINT32 pkt_period_d_msb  = 0;
    UINT32 pkt_period_d_lsb  = 0;
    UINT32 pkt_period_n_msb  = 0;
    UINT32 pkt_period_n_lsb  = 0;
    DOUBLE pkt_period        = 0;
    UINT32 thermometer_int   = 0;
    UINT32 thermometer_d     = 0;
    UINT32 thermometer_n     = 0;
    UINT32 pkt_size          = 0;
    UINT32 pkt_size_var      = 0;
    UINT32 fb_wordsize       = 0;
    INT32  cnt_n             = 0;
    UINT32 cnt_d             = 0;

    PMC_ERROR rc = PMC_SUCCESS;    
        
    PMC_ENTRY();

    PMC_ASSERT(acb_ft_handle != NULL, ACB_FT_ERR_INVALID_PARAMETERS,0,0);    

    /* 
       resolve ACB configuration 
    */
    if (PMC_SUCCESS == rc) 
    {
        acb_ft_pkt_period_calc(acb_ft_handle,
                               cfg_ptr,
                               &pkt_period_int,
                               &pkt_period_d_msb,
                               &pkt_period_d_lsb,
                               &pkt_period_n_msb, 
                               &pkt_period_n_lsb,
                               &thermometer_int,
                               &thermometer_d,
                               &thermometer_n,
                               &pkt_period);  
    }
    if (PMC_SUCCESS == rc) 
    {
        acb_ft_pkt_size_calc(acb_ft_handle, cfg_ptr, &pkt_size, &pkt_size_var, &fb_wordsize, &cnt_n, &cnt_d);
    }
    if (PMC_SUCCESS == rc) 
    {
        acb_ft_gain_calc(acb_ft_handle,cfg_ptr,pkt_period, &int_gain, &filt_gain, &final_shift);
    }        

    PMC_LOG_TRACE("ACB_FT configuration\n");
    PMC_LOG_TRACE("pkt_period_int    = %d\n", pkt_period_int  );
    PMC_LOG_TRACE("pkt_period_d_msb  = %d\n", pkt_period_d_msb);
    PMC_LOG_TRACE("pkt_period_d_lsb  = %d\n", pkt_period_d_lsb);
    PMC_LOG_TRACE("pkt_period_n_msb  = %d (%u) \n", pkt_period_n_msb, (UINT32)pkt_period_n_msb);
    PMC_LOG_TRACE("pkt_period_n_lsb  = %d (%u) \n", pkt_period_n_lsb, (UINT32)pkt_period_n_lsb );
    PMC_LOG_TRACE("thermometer_int   = %d\n", thermometer_int );
    PMC_LOG_TRACE("thermometer_d     = %d\n", thermometer_d   );
    PMC_LOG_TRACE("thermometer_n     = %d\n", thermometer_n   );
    PMC_LOG_TRACE("pkt_period        = %f\n", pkt_period      );
    PMC_LOG_TRACE("pkt_size          = %d\n", pkt_size        );
    PMC_LOG_TRACE("pkt_size_var      = %d\n", pkt_size_var    );
    PMC_LOG_TRACE("fb_wordsize       = %d\n", fb_wordsize     );
    PMC_LOG_TRACE("cnt_n            = %d (%u)\n",cnt_n, (UINT32) cnt_n  );
    PMC_LOG_TRACE("cnt_d             = %d\n", cnt_d           );
    PMC_LOG_TRACE("int_gain          = %d\n", int_gain        );
    PMC_LOG_TRACE("filt_gain         = %d\n", filt_gain       );
    PMC_LOG_TRACE("final_shift       = %d\n", final_shift     );


    if (PMC_SUCCESS == rc) 
    {   
        /*set PMM_ADAPT_ENABLE to 0 so configs can be safely modified*/
        acb_ft_field_PMM_ADAPT_ENABLE_set(NULL,acb_ft_handle, 0);
        /* 
           Provision the ACB_FT 
        */
        acb_ft_init_reset(acb_ft_handle);
        
        /* Configure control */
        acb_ft_init_ctrl_cfg(acb_ft_handle);   
        
        
        acb_ft_pkt_period_cfg( acb_ft_handle,
                                   pkt_period_int,
                                   pkt_period_d_msb,
                                   pkt_period_d_lsb,
                                   pkt_period_n_msb,
                                   pkt_period_n_lsb,
                                   thermometer_int,
                                   thermometer_d,
                                   thermometer_n);                              
        acb_ft_pkt_size_cfg(acb_ft_handle,
                                pkt_size, 
                                pkt_size_var, 
                                fb_wordsize, 
                                cnt_n, 
                                cnt_d);  
        acb_ft_gain_cfg(acb_ft_handle,int_gain, filt_gain, final_shift);

        acb_ft_field_PMM_ADAPT_ENABLE_set(NULL,acb_ft_handle, 1);
    }     

    PMC_RETURN(rc);
} /* acb_ft_reprov_pkt_size_configs */

/*******************************************************************************
* acb_ft_cntn_cntd_recalculate_and_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function recalculates and reconfigure CNT_N and CNT_D values according
*   to master lane and current lane bus width values.
*
* INPUTS:
*   acb_ft_handle           - pointer to ACB_FT handle instance
*   master_lane_bus_width   - master lane bus width
*   lane_bus_width          - slave lane bus width
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
PUBLIC void acb_ft_cntn_cntd_recalculate_and_cfg(acb_ft_handle_t    *acb_ft_handle, 
                                                 UINT8               master_lane_bus_width,
                                                 UINT8               lane_bus_width)
{
    UINT32 cnt_d;
    INT32 cnt_n;
    DOUBLE f_cnt_nd;
    acb_ft_buffer_t acb_ft_buffer[1];
    
    PMC_ENTRY();

    PMC_ASSERT(acb_ft_handle != NULL, ACB_FT_ERR_INVALID_PARAMETERS,0,0);        
    PMC_ASSERT(master_lane_bus_width != 0, ACB_FT_ERR_INVALID_PARAMETERS,0,0); 
    PMC_ASSERT(lane_bus_width != 0, ACB_FT_ERR_INVALID_PARAMETERS,0,0);  

    if (master_lane_bus_width != lane_bus_width)
    {

        cnt_n = acb_ft_field_CNT_N_get(NULL,acb_ft_handle);  
        cnt_d = acb_ft_field_CNT_D_get(NULL,acb_ft_handle);  

        if (0 != cnt_d) 
        {
            if (!digi_use_float32_get())
            {                
                f_cnt_nd = (cnt_n / (DOUBLE)cnt_d) + 1;  
                f_cnt_nd = (f_cnt_nd * master_lane_bus_width) / lane_bus_width;
            }
            else
            {
                f_cnt_nd = (cnt_n / (FLOAT)cnt_d) + 1; /* plus 1 */                
                f_cnt_nd = (f_cnt_nd * master_lane_bus_width) / lane_bus_width;

            }
            
            util_global_fract_to_dec_conv(f_cnt_nd, (UINT32*) &cnt_n, &cnt_d);
            
            /* minus 1 */
            cnt_n = (cnt_n - cnt_d);
            
            /* write parameter new value in register */            
            /* Initilize ACB buffer */
            BOOL8 pmm_adapt_en;
            pmm_adapt_en = acb_ft_field_PMM_ADAPT_ENABLE_get(NULL, acb_ft_handle);

            acb_ft_field_PMM_ADAPT_ENABLE_set(NULL, acb_ft_handle, 0);

            acb_ft_buffer_init(acb_ft_buffer,acb_ft_handle);
            
            acb_ft_field_CNT_D_set(acb_ft_buffer,acb_ft_handle,cnt_d);
            acb_ft_field_CNT_N_set(acb_ft_buffer,acb_ft_handle,cnt_n);  
            
            
            acb_ft_buffer_flush( acb_ft_buffer );
            if (TRUE == pmm_adapt_en) 
            {                
                acb_ft_field_PMM_ADAPT_ENABLE_set(NULL, acb_ft_handle, 1);
            }
        }
    }    

    PMC_RETURN();
}/*acb_ft_cntn_cntd_recalculate_and_cfg*/

/*
** End of file
*/
