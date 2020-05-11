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

#include "epmm_loc.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
/* Defined EGR_GSUP_TX_SW_FORCE register bit possible values */
#define EPMM_LF_OS_FORCE 0x1
#define EPMM_RF_OS_FORCE 0x2
#define EPMM_OS_NO_FORCE 0x0
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
const char EPMM_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    EPMM_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char EPMM_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Public Functions
*/

/*
** Private Functions
*/
PRIVATE BOOL8 epmm_data_rate_start_state_test(epmm_handle_t *hndl,
                                             epmm_data_rate_t data_rate);

PRIVATE PMC_ERROR epmm_int_params_get(epmm_handle_t        *epmm_handle,
                                      epmm_datapath_t       datapath,
                                      UINT32                link,
                                      BOOL8                *has_exco4m,
                                      BOOL8                *has_exco3m,
                                      UINT32               *exco3m_idx,
                                      BOOL8                *has_gsup43,
                                      UINT32               *gsup43_idx);

/*******************************************************************************
* epmm_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a EPMM block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the EPMM block relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - pointer to TSB name
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   epmm_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC epmm_handle_t *epmm_ctxt_create(pmc_handle_t *parent, 
                                       UINT32 base_address, 
                                       pmc_sys_handle_t *sys_handle, 
                                       const char *tsb_name )
{
    epmm_handle_t *epmm_handle;
    UINT32        i;
    UINT32        tsb_base_addr;

    PMC_ENTRY();

    epmm_handle = (epmm_handle_t *) PMC_CTXT_CALLOC(sizeof(epmm_handle_t), parent);

    pmc_handle_init( parent, epmm_handle, sys_handle, PMC_MID_DIGI_EPMM, tsb_name, base_address );

    /* Register log strings. */
    pmc_log_block_strings_register(EPMM_LOG_ERR_STRINGS[0], EPMM_LOG_ERR_TABLE_BASE, EPMM_LOG_ERR_COUNT);

    /* Create handle for child blocks */
    for (i = 0; i < EPMM_NUM_GSUP43_C73; i++) 
    {
        tsb_base_addr = base_address + BASE_ADDR_EPMM120_GSUP43_10G_GSUP43_10G_WRAP_GSUP43_C73(i);
        epmm_handle->gsup43_c73_handle[i] = gsup43_c73_ctxt_create((pmc_handle_t *)epmm_handle, tsb_base_addr, sys_handle, "GSUP43_C73");
        PMC_CTXT_REGISTER_PTR(&epmm_handle->gsup43_c73_handle[i], parent);
    }
    epmm_handle->mpma_handle = mpma_ctxt_create((pmc_handle_t *)epmm_handle, base_address+BASE_ADDR_EPMM120_MPMA_MTSB, sys_handle, "MPMA");
    PMC_CTXT_REGISTER_PTR(&epmm_handle->mpma_handle, parent);
    epmm_handle->mpmo_handle = mpmo_ctxt_create((pmc_handle_t *)epmm_handle, base_address+BASE_ADDR_EPMM120_MPMO_MTSB, sys_handle, "MPMO");
    PMC_CTXT_REGISTER_PTR(&epmm_handle->mpmo_handle, parent);

    for (i = 0; i < EPMM_NUM_EXCO3M; i++) 
    {
        tsb_base_addr = base_address + BASE_ADDR_EPMM120_CBR_40G_EXCO3M(i);
        epmm_handle->exco3m_handle[i] = exco3m_ctxt_create((pmc_handle_t *)epmm_handle, tsb_base_addr, sys_handle, "EXCO3M");
        PMC_CTXT_REGISTER_PTR(&epmm_handle->exco3m_handle[i], parent);
    }
    epmm_handle->exco4m_handle = exco4m_ctxt_create((pmc_handle_t *)epmm_handle, base_address+BASE_ADDR_EPMM120_CBR_100G_EXCO4M, sys_handle, "EXCO4M");
    PMC_CTXT_REGISTER_PTR(&epmm_handle->exco4m_handle, parent);

    PMC_RETURN(epmm_handle);

} /* epmm_ctxt_create */

/*******************************************************************************
* epmm_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a EPMM block instance.
*
*
* INPUTS:
*   *epmm_handle        - pointer to EPMM handle instance
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
PUBLIC void epmm_ctxt_destroy(epmm_handle_t *epmm_handle)
{
    UINT32 i;

    PMC_ENTRY();

    mpma_ctxt_destroy(epmm_handle->mpma_handle);
    mpmo_ctxt_destroy(epmm_handle->mpmo_handle);
    for (i = 0; i < EPMM_NUM_EXCO3M; i++) 
    {
      exco3m_ctxt_destroy(epmm_handle->exco3m_handle[i]);
    }
    exco4m_ctxt_destroy(epmm_handle->exco4m_handle);
    for (i = 0; i < EPMM_NUM_GSUP43_C73; i++) 
    {
        gsup43_c73_ctxt_destroy(epmm_handle->gsup43_c73_handle[i]);
    }

    PMC_CTXT_FREE(&epmm_handle, epmm_handle);
    PMC_RETURN();
} /* epmm_ctxt_destroy */

/*******************************************************************************
* epmm_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a EPMM block handle instance.
*
*
* INPUTS:
*   *epmm_handle        - pointer to EPMM handle instance
*   mpmo_cfg_mode       - Controls the provisioning source of MPMO for modes that
*                         requier MPMO\n
*                         FALSE: Locally generate MPMO provisioning\n
*                         TRUE:  Externally generate MPMO provisioning\n
*                         When set to TRUE the API epmm_mpmo_ch_prov() must be
*                         called following epmm_egress_cfg() in order to provision
*                         MPMO.\n
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
PUBLIC void epmm_handle_init(epmm_handle_t *epmm_handle, 
                             BOOL8 mpmo_cfg_mode)
{
    UINT32 i;

    PMC_ENTRY();
    
    epmm_handle->var.mpmo_cfg_mode = mpmo_cfg_mode;
    /* Initialize var_t */
    epmm_var_default_init(epmm_handle);

    if(epmm_handle->mpma_db_alloc_mode == UTIL_SCHD_ALLOC_METHOD_ENET_LINE_MPMA_DB)
    { 
        mpma_handle_init(epmm_handle->mpma_handle, MPMA_ENET_LINE_DB, SCBS3_TOC_SYNC_ENET);
    } else if(epmm_handle->mpma_db_alloc_mode == UTIL_SCHD_ALLOC_METHOD_ENET_SYS_MPMA_DB)
    { 
        mpma_handle_init(epmm_handle->mpma_handle, MPMA_ENET_SYS_DB, SCBS3_TOC_SYNC_ENET);
    } else 
    {      
        mpma_handle_init(epmm_handle->mpma_handle, MPMA_ENET, SCBS3_TOC_SYNC_ENET);
    }
   
    mpmo_handle_init(epmm_handle->mpmo_handle, MPMO_ENET, EPMM_MPMO_TOC);
    
    for (i = 0; i < EPMM_NUM_GSUP43_C73; i++) 
    {
        gsup43_c73_handle_init(epmm_handle->gsup43_c73_handle[i]);
    }
    for (i = 0; i < EPMM_NUM_EXCO3M; i++) 
    {
        exco3m_handle_init(epmm_handle->exco3m_handle[i]);
    }

    exco4m_handle_init(epmm_handle->exco4m_handle);
    PMC_RETURN();
} /* epmm_handle_init */


/*******************************************************************************
* epmm_alloc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Handles internal memory allocation
*
*
* INPUTS:
*   *epmm_handle        - pointer to EPMM handle instance
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
PUBLIC void epmm_alloc_init(epmm_handle_t *epmm_handle)
{
    PMC_ENTRY();

    mpma_alloc_init(epmm_handle->mpma_handle);
    mpmo_alloc_init(epmm_handle->mpmo_handle);
   
    PMC_RETURN();
} /* epmm_alloc_init */


/*******************************************************************************
* epmm_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a EPMM block handle instance.
*
*
* INPUTS:
*   *epmm_handle        - pointer to EPMM handle instance
*   *restart_init_cfg_ptr - pointer to restart init config
*   top_energy_state    - The expected energy state 
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
PUBLIC PMC_ERROR epmm_handle_restart_init(epmm_handle_t *epmm_handle, 
                                          util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                          pmc_energy_state_t top_energy_state)
{
    PMC_ERROR rc;
    BOOL8 mismatch_channels;
    UINT32 i;

    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(EPMM_LOG_ERR_STRINGS[0], EPMM_LOG_ERR_TABLE_BASE, EPMM_LOG_ERR_COUNT);
    }

    rc = mpma_handle_restart_init(epmm_handle->mpma_handle, restart_init_cfg_ptr, top_energy_state, TRUE, &mismatch_channels);

    if (PMC_SUCCESS == rc)
    {
        rc = mpmo_handle_restart_init(epmm_handle->mpmo_handle, restart_init_cfg_ptr, top_energy_state, TRUE, &mismatch_channels);
    }
   
    for (i = 0; i < EPMM_NUM_GSUP43_C73 && PMC_SUCCESS == rc; i++) 
    {
        rc = gsup43_c73_handle_restart_init(epmm_handle->gsup43_c73_handle[i], restart_init_cfg_ptr, top_energy_state);
    }
    for (i = 0; i < EPMM_NUM_EXCO3M && PMC_SUCCESS == rc; i++) 
    {
        exco3m_handle_restart_init(epmm_handle->exco3m_handle[i], restart_init_cfg_ptr, top_energy_state);
    }

    if (PMC_SUCCESS == rc)
    {
        exco4m_handle_restart_init(epmm_handle->exco4m_handle, restart_init_cfg_ptr, top_energy_state);
    }
    PMC_RETURN(rc);
} /* epmm_handle_restart_init */


/*******************************************************************************
* epmm_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the EPMM instance to the starting state for all
*   dynamic context.  All dynamic context will be returned to the initialization 
*   values that were generate during the call to epmm_handle_init().
*
*   This function is for use by parent modules that have provided all other
*   provisions for ensuring the state of all resources tracked by this
*   context has coherent register state.  
*
*   Note that link & datapath are only provided for the 
*   PMC_ENERGY_STATE_REQUEST_RUNNING energy state
*
* INPUTS:
*   *hndl                 - pointer to EPMM handle instance
*   link                  - requested link 
*   datapath              - enum, requested datapath
*   energy_state          - enum defining state to set the EPMM module to
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_init(epmm_handle_t *hndl, 
                           UINT32 link, 
                           epmm_datapath_t datapath,
                           pmc_energy_state_request_t energy_state)
{ 
    epmm_data_rate_t    data_rate = LAST_EPMM_DATA_RATE;    
    PMC_ERROR ret_val = PMC_SUCCESS;
    BOOL8 epmm_var_default = TRUE;

    PMC_ENTRY();
    PMC_ASSERT(hndl != NULL, EPMM_ERR_INVALID_PARAMETERS, 0, 0); 
   
    /* handle energy request */
    switch(energy_state)
    {
    case PMC_ENERGY_STATE_REQUEST_RUNNING:       
    {
        /* set data_rate */
        switch (datapath) 
        {
        case EPMM_UNUSED_10G:
        case EPMM_10GE_GSUP43_73:
        case EPMM_10GE_GSUP43_73_HI_BER_MON:
        case EPMM_10GE_CBR:
        case EPMM_10GE_CBR_GSUP43_73_HI_BER_MON:
            data_rate = EPMM_LINK_10G;
            break;
        case EPMM_UNUSED_40G:
        case EPMM_40GE:
            data_rate = EPMM_LINK_40G;
            break;
        case EPMM_UNUSED_100G:
        case EPMM_100GE:
            data_rate = EPMM_LINK_100G;
            break;
        default:
            PMC_ASSERT(FALSE, EPMM_ERR_INVALID_PARAMETERS, 0, 0); 
            break;
        }

        /* EPMM TOP */
        if(PMC_SUCCESS == ret_val && epmm_data_rate_start_state_test(hndl,data_rate)== TRUE)
        {
            PMC_LOG_TRACE("configure EPMM-top energy state\n");
            /* Configure EPMM energy state */
            epmm_energy_state_reg_set(hndl, data_rate,PMC_ENERGY_STATE_RUNNING);        
        }  
        
        /* MPMA */
        if(PMC_SUCCESS == ret_val && mpma_start_state_test(hndl->mpma_handle)== TRUE)
        {
            PMC_LOG_TRACE("configure EPMM::MPMA energy state for first time\n");
            /* configure MPMA energy state */
            ret_val = mpma_init(hndl->mpma_handle, PMC_ENERGY_STATE_RUNNING);
            mpma_enable_cfg(hndl->mpma_handle, TRUE);
        }  
        /* MPMO */
        if(PMC_SUCCESS == ret_val && mpmo_start_state_test(hndl->mpmo_handle)== TRUE)
        {
            PMC_LOG_TRACE("configure EPMM::MPMO energy state for first time\n");
            /* configure MPMO energy state */
            ret_val = mpmo_init(hndl->mpmo_handle, PMC_ENERGY_STATE_RUNNING);
            mpmo_enable_cfg(hndl->mpmo_handle, TRUE);
        }     
        break;
    }         
    case PMC_ENERGY_STATE_REQUEST_RESET:
    case PMC_ENERGY_STATE_REQUEST_OPTIMAL:
    {
        /* power down block if they are not provisionned */        
        if(PMC_SUCCESS == ret_val)
        {
            /* MPMA */
            if(mpma_start_state_test(hndl->mpma_handle)== TRUE ||
               PMC_ENERGY_STATE_REQUEST_RESET == energy_state )
            {
                /* configure MPMA energy state */
                mpma_init(hndl->mpma_handle, PMC_ENERGY_STATE_RESET);               
            }  
        }
        if(PMC_SUCCESS == ret_val)
        {
            /* MPMO */
            if(mpmo_start_state_test(hndl->mpmo_handle)== TRUE ||
               PMC_ENERGY_STATE_REQUEST_RESET == energy_state )
            {
                /* configure MPMO energy state */
                mpmo_init(hndl->mpmo_handle, PMC_ENERGY_STATE_RESET);               
            }  
        }

        if(PMC_SUCCESS == ret_val && 
           (epmm_data_rate_start_state_test(hndl,EPMM_LINK_10G)== TRUE ||
            PMC_ENERGY_STATE_REQUEST_RESET == energy_state))
        {               
            /* Configure EPMM energy state */
            epmm_energy_state_reg_set(hndl,EPMM_LINK_10G ,PMC_ENERGY_STATE_RESET);        
        }  else 
        {
            epmm_var_default = FALSE;
        }
        if(PMC_SUCCESS == ret_val && 
           (epmm_data_rate_start_state_test(hndl,EPMM_LINK_40G)== TRUE ||
            PMC_ENERGY_STATE_REQUEST_RESET == energy_state))
        {               
            /* Configure EPMM energy state */
            epmm_energy_state_reg_set(hndl,EPMM_LINK_40G ,PMC_ENERGY_STATE_RESET);        
        }  else 
        {
            epmm_var_default = FALSE;
        }
        if(PMC_SUCCESS == ret_val && 
           (epmm_data_rate_start_state_test(hndl,EPMM_LINK_100G)== TRUE ||
            PMC_ENERGY_STATE_REQUEST_RESET == energy_state))
        {               
            /* Configure EPMM energy state */
            epmm_energy_state_reg_set(hndl,EPMM_LINK_100G ,PMC_ENERGY_STATE_RESET);        
        }  else 
        {
            epmm_var_default = FALSE;
        }
        if (TRUE == epmm_var_default)
        {
        
            /* Configure epmm_var as default */
            epmm_var_default_init(hndl);
        }
        break;
    }        
    default:
        PMC_ASSERT(FALSE, EPMM_ERR_INVALID_PARAMETERS, 0, 0);
    }
  
    PMC_RETURN(ret_val);

} /* epmm_init */

/*******************************************************************************
* epmm_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   EPMM instance.  
*
*
* INPUTS:
*   *hndl                       - pointer to EPMM handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8                       - FALSE:  Resource(s) not in start state \n
*                                TRUE:   Resources all in start state
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 epmm_start_state_test(epmm_handle_t *hndl)
{
    UINT32 itr;
    BOOL8 result = TRUE;

    PMC_ENTRY();
    
    PMC_ASSERT(hndl!=NULL, EPMM_ERR_INVALID_PARAMETERS, 0, 0);


    for (itr = 0; itr < EPMM_NUM_LINK; itr++)
    {
        if (hndl->var.data_rate[itr] != LAST_EPMM_DATA_RATE) 
        {
            result = FALSE;
        }
    }
    
    if(result == TRUE) {
        if((mpma_start_state_test(hndl->mpma_handle) == FALSE) ||
           (mpmo_start_state_test(hndl->mpmo_handle) == FALSE) ||
           (exco4m_start_state_test(hndl->exco4m_handle) == FALSE))
        {
            result = FALSE;
        }
    }
    
    if(result == TRUE) {
        for(itr = 0; itr < EPMM_NUM_GSUP43_C73; itr++) {
            if(result == TRUE && gsup43_c73_start_state_test(hndl->gsup43_c73_handle[itr]) == FALSE)
                result = FALSE;
        }
    }

    if(result == TRUE) {
        for(itr = 0; itr < EPMM_NUM_EXCO3M; itr++) {   
            if(result == TRUE && exco3m_start_state_test(hndl->exco3m_handle[itr]) == FALSE)
                result = FALSE;
        }
    }    
        
    PMC_RETURN(result);
    
} /* epmm_start_state_test */

/*******************************************************************************
* epmm_activated_link_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns RX and TX links status.
*                                                                               
* INPUTS:                                                                       
*   *hndl             - pointer to EPMM handle instance
*                                                                             
* OUTPUTS:
*   *rx_actv_link     - status of EPMM RX links
*   *tx_actv_link     - status of EPMM TX links
*
* RETURNS:
*   None.                                                                        
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC void epmm_activated_link_get(epmm_handle_t   *hndl,
                                    BOOL8           *rx_actv_link,
                                    BOOL8           *tx_actv_link)
{
    UINT32 itr;
    PMC_ENTRY();
    
    PMC_ASSERT(hndl != NULL, EPMM_ERR_INVALID_PARAMETERS, 0, 0);
    
    for (itr = 0; itr < EPMM_NUM_LINK; itr++)
    {
        if (itr == 0)
        {
            rx_actv_link[itr] = epmm_field_ING_10G_LINK_ENABLE_get( NULL, hndl, itr) | 
                epmm_field_ING_40G_LINK_ENABLE_get( NULL, hndl, itr) |
                epmm_field_ING_100G_LINK_ENABLE_get( NULL, hndl);

            tx_actv_link[itr] = epmm_field_EGR_10G_LINK_ENABLE_get( NULL, hndl, itr) | 
                epmm_field_EGR_40G_LINK_ENABLE_get( NULL, hndl, itr) |
                epmm_field_EGR_100G_LINK_ENABLE_get( NULL, hndl);
        }
        else if (0 == itr % 4)
        {
            rx_actv_link[itr] = epmm_field_ING_10G_LINK_ENABLE_get( NULL, hndl, itr) | 
                epmm_field_ING_40G_LINK_ENABLE_get( NULL, hndl, itr / 4);
            tx_actv_link[itr] = epmm_field_EGR_10G_LINK_ENABLE_get( NULL, hndl, itr) | 
                epmm_field_EGR_40G_LINK_ENABLE_get( NULL, hndl, itr / 4);

        }
        else
        {
            rx_actv_link[itr] = epmm_field_ING_10G_LINK_ENABLE_get( NULL, hndl, itr);
            tx_actv_link[itr] = epmm_field_EGR_10G_LINK_ENABLE_get( NULL, hndl, itr);
        }
    }

    PMC_RETURN();
}  /* epmm_activated_link_get */


/*******************************************************************************
* epmm_rx_link_rate_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns EPMM RX links datapath and rate.
*                                                                               
* INPUTS:                                                                       
*   *hndl             - pointer to EPMM handle instance
*                                                                             
* OUTPUTS:
*   *datapath         - list of link datapath
*   *rate             - list of link rate
*
* RETURNS: 
*   None.                                                                        
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC void epmm_rx_link_rate_get(epmm_handle_t       *hndl,
                                  epmm_datapath_t     *datapath,
                                  epmm_data_rate_t    *rate)
{
    UINT32 itr;
    UINT32 incr;
    UINT32 dp_sel_reg_val;
    BOOL8 rate_10G;
    BOOL8 rate_40G;
    BOOL8 rate_100G;

    PMC_ENTRY();
    
    PMC_ASSERT(hndl != NULL, EPMM_ERR_INVALID_PARAMETERS, 0, 0);

    
    for (itr = 0; itr < EPMM_NUM_LINK; itr++)
    {
        rate[itr] = LAST_EPMM_DATA_RATE; 
        datapath[itr] = EPMM_DATAPATH_UNUSED;
    }
    
    for (itr = 0; itr < EPMM_NUM_LINK; itr += incr)
    {
        incr = 1;
        rate_10G = FALSE;
        rate_40G = FALSE;
        rate_100G = FALSE;

        rate_10G = epmm_field_ING_10G_LINK_ENABLE_get(NULL, hndl, itr);
        if (0 == itr)
        {            
            rate_100G = epmm_field_ING_100G_LINK_ENABLE_get(NULL, hndl);
            rate_40G = epmm_field_ING_40G_LINK_ENABLE_get(NULL, hndl, itr);

            if (rate_100G == TRUE)
            {
                datapath[itr] = EPMM_100GE;
                rate[itr] = EPMM_LINK_100G;   
                break;             
            }
            else if (rate_40G == TRUE)
            {
                datapath[itr] = EPMM_40GE;
                rate[itr] = EPMM_LINK_40G;    
                incr = 4;
            }
            else if (rate_10G == TRUE)
            {                
                rate[itr] = EPMM_LINK_10G;
                dp_sel_reg_val = epmm_field_ING_10G_LINK_DP_SEL_get(NULL, hndl, itr);
                switch(dp_sel_reg_val)
                {
                case ENET_EPMM_DP_SEL_GSUP:
                    datapath[itr] = EPMM_10GE_GSUP43_73;
                    break;
                case ENET_EPMM_DP_SEL_GSUP_CBR:
                    datapath[itr] = EPMM_10GE_CBR;
                    break;                        
                default:
                    break;
                }                          
            }
        }
        else if (0 == (itr % 4))
        {

            rate_40G = epmm_field_ING_40G_LINK_ENABLE_get(NULL, hndl, itr / 4) ; 
            if (rate_40G == TRUE)
            {
                datapath[itr] = EPMM_40GE;
                rate[itr] = EPMM_LINK_40G;  
                incr = 4;              
            }
            else if (rate_10G == TRUE)
            {                
                rate[itr] = EPMM_LINK_10G;
                dp_sel_reg_val = epmm_field_ING_10G_LINK_DP_SEL_get(NULL, hndl, itr);
                switch(dp_sel_reg_val)
                {
                case ENET_EPMM_DP_SEL_GSUP:
                    datapath[itr] = EPMM_10GE_GSUP43_73;
                    break;
                case ENET_EPMM_DP_SEL_GSUP_CBR:
                    datapath[itr] = EPMM_10GE_CBR;
                    break;                          
                default:
                    break;
                }
            }
        }
        else 
        {
            if (rate_10G == TRUE)
            {         
                dp_sel_reg_val = epmm_field_ING_10G_LINK_DP_SEL_get(NULL, hndl, itr);
                switch(dp_sel_reg_val)
                {
                case ENET_EPMM_DP_SEL_GSUP:
                    datapath[itr] = EPMM_10GE_GSUP43_73;
                    rate[itr] = EPMM_LINK_10G;
                    break;
                case ENET_EPMM_DP_SEL_GSUP_CBR:
                    datapath[itr] = EPMM_10GE_CBR;
                    rate[itr] = EPMM_LINK_10G;
                    break;                           
                default:
                    break;                       
                }
            }
        }

            /* retrieve epmm unused rate in mpma registers */

    }

    PMC_RETURN();
}  /* epmm_rx_link_rate_get */


/*******************************************************************************
* epmm_tx_link_rate_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns EPMM TX links datapath and rate.
*                                                                               
* INPUTS:                                                                       
*   *hndl             - pointer to EPMM handle instance
*                                                                             
* OUTPUTS:
*   *datapath         - list of link datapath
*   *rate             - list of link rate
*
* RETURNS: 
*   None.                                                                        
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC void epmm_tx_link_rate_get(epmm_handle_t       *hndl,
                                  epmm_datapath_t     *datapath,
                                  epmm_data_rate_t    *rate)
{
    UINT32 itr;
    UINT32 incr;
    UINT32 dp_sel_reg_val;
    BOOL8 rate_10G;
    BOOL8 rate_40G;
    BOOL8 rate_100G;

    PMC_ENTRY();
    
    PMC_ASSERT(hndl != NULL, EPMM_ERR_INVALID_PARAMETERS, 0, 0);

    
    for (itr = 0; itr < EPMM_NUM_LINK; itr++)
    {
        rate[itr] = LAST_EPMM_DATA_RATE; 
        datapath[itr] = EPMM_DATAPATH_UNUSED;
    }
    
    for (itr = 0; itr < EPMM_NUM_LINK; itr += incr)
    {
        incr = 1;
        rate_10G = FALSE;
        rate_40G = FALSE;
        rate_100G = FALSE;

        rate_10G = epmm_field_EGR_10G_LINK_ENABLE_get(NULL, hndl, itr);
        if (0 == itr)
        {            
            rate_100G = epmm_field_EGR_100G_LINK_ENABLE_get(NULL, hndl);
            rate_40G = epmm_field_EGR_40G_LINK_ENABLE_get(NULL, hndl, itr);

            if (rate_100G == TRUE)
            {
                datapath[itr] = EPMM_100GE;
                rate[itr] = EPMM_LINK_100G;   
                break;             
            }
            else if (rate_40G == TRUE)
            {
                datapath[itr] = EPMM_40GE;
                rate[itr] = EPMM_LINK_40G;    
                incr = 4;
            }
            else if (rate_10G == TRUE)
            {                
                rate[itr] = EPMM_LINK_10G;
                dp_sel_reg_val = epmm_field_EGR_10G_LINK_DP_SEL_get(NULL, hndl, itr);
                switch(dp_sel_reg_val)
                {
                case ENET_EPMM_DP_SEL_GSUP:
                    datapath[itr] = EPMM_10GE_GSUP43_73;
                    break;
                case ENET_EPMM_DP_SEL_GSUP_CBR:
                    datapath[itr] = EPMM_10GE_CBR;
                    break;                        
                default:
                    break;
                }                          
            }
        }
        else if (0 == (itr % 4))
        {

            rate_40G = epmm_field_EGR_40G_LINK_ENABLE_get(NULL, hndl, itr / 4) ; 
            if (rate_40G == TRUE)
            {
                datapath[itr] = EPMM_40GE;
                rate[itr] = EPMM_LINK_40G;  
                incr = 4;              
            }
            else if (rate_10G == TRUE)
            {                
                rate[itr] = EPMM_LINK_10G;
                dp_sel_reg_val = epmm_field_EGR_10G_LINK_DP_SEL_get(NULL, hndl, itr);
                switch(dp_sel_reg_val)
                {
                case ENET_EPMM_DP_SEL_GSUP:
                    datapath[itr] = EPMM_10GE_GSUP43_73;
                    break;
                case ENET_EPMM_DP_SEL_GSUP_CBR:
                    datapath[itr] = EPMM_10GE_CBR;
                    break;                          
                default:
                    break;
                }
            }
        }
        else 
        {
            if (rate_10G == TRUE)
            {         
                dp_sel_reg_val = epmm_field_EGR_10G_LINK_DP_SEL_get(NULL, hndl, itr);
                switch(dp_sel_reg_val)
                {
                case ENET_EPMM_DP_SEL_GSUP:
                    datapath[itr] = EPMM_10GE_GSUP43_73;
                    rate[itr] = EPMM_LINK_10G;
                    break;
                case ENET_EPMM_DP_SEL_GSUP_CBR:
                    datapath[itr] = EPMM_10GE_CBR;
                    rate[itr] = EPMM_LINK_10G;
                    break;                           
                default:
                    break;                       
                }
            }
        }

            /* retrieve epmm unused rate in mpma registers */

    }

    PMC_RETURN();
}  /* epmm_tx_link_rate_get */



/*******************************************************************************
* epmm_data_rate_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   EPMM instance.  
*
*
* INPUTS:
*   *hndl                       - pointer to EPMM handle instance
*   data_rate                   - request status for this rate
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8                       - FALSE:  Resource(s) not in start state \n
*                                TRUE:   Resources all in start state
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 epmm_data_rate_start_state_test(epmm_handle_t *hndl,
                                             epmm_data_rate_t data_rate)
{
    UINT32 itr;
    BOOL8 result = TRUE;

    PMC_ENTRY();
    
    PMC_ASSERT(hndl!=NULL, EPMM_ERR_INVALID_PARAMETERS, 0, 0);
    

    for (itr = 0; itr < EPMM_NUM_LINK; itr++)
    {
        if (hndl->var.data_rate[itr] == data_rate) 
        {
            result = FALSE;
        }
    }   
        
    PMC_RETURN(result);
    
} /* epmm_data_rate_start_state_test */

/*******************************************************************************
* epmm_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of a EPMM block instance.
*
*
* INPUTS:
*   *epmm_handle        - pointer to EPMM handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Base address of EPMM.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 epmm_base_address_get(epmm_handle_t *epmm_handle)
{
    PMC_ENTRY();

    PMC_RETURN(epmm_handle->base.base_address);
} /* epmm_base_address_get */


/*******************************************************************************
* Initialization Configuration Functions
*******************************************************************************/
/*******************************************************************************
*  epmm_ingress_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure appropriate transcoder based on client data rate (10GE, 40GE, 100GE).
*   Configure packet maker.
*
* INPUTS:
*   hndl         - handle to the EPMM instance to be operated on
*   link         - link to configure
*   datapath     - data path to configure
*   enable       - TRUE: Enable datapath. FALSE: Disable datapath.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_ingress_cfg(epmm_handle_t *hndl, 
                                  UINT32 link, 
                                  epmm_datapath_t datapath, 
                                  BOOL8 enable)
{
    UINT32 dp_sel               = ENET_EPMM_DP_SEL_GSUP;
    BOOL8 scbs3_only             = FALSE;
    epmm_buffer_t epmm_buffer[1];
    BOOL8 epmm_unused           = FALSE;
    /* MPMA output values */
    UINT32 cfg_pp_int           = 0;
    UINT32 cfg_pp_n             = 0;
    UINT32 cfg_pp_d             = 0;
    DOUBLE server_rate          = 0;
    DOUBLE epsilon              = 0;
    PMC_ERROR ret_val           = PMC_SUCCESS;
    epmm_data_rate_t  data_rate = LAST_EPMM_DATA_RATE;
    UINT32 exco3m_inst          = 0;
    UINT32 scbs3_user_bits_3_0  = ENET_EPMM_MPMA_SCBS_USER_BITS_UNSED;
    UINT32 pkt_size             = ENET_MPMA_PKT_SIZE;

    PMC_ENTRY();

    /* Enable/disable specified datapath */
    /* Register 0x20 + 0x04*N(N=0:11): EPMM120_TOP - ING  10G Link Configuration Register */
    /* Register 0x50 + 0x04*N(N=0: 2): EPMM120_TOP - ING  40G Link Configuration Register */
    /* Register 0x5C                 : EPMM120_TOP - ING 100G Link Configuration Register */
    /* Register 0x1000 + 0x0400*A(A=0:2): CBR_40G(A) - Configuration */
    /* Register 0x2000: CBR_100G - Configuration */

    /* Translate server rate */
    switch (datapath) 
    {
    case EPMM_UNUSED_10G:
    case EPMM_10GE_GSUP43_73:
    case EPMM_10GE_GSUP43_73_HI_BER_MON:
    case EPMM_10GE_CBR:
    case EPMM_10GE_CBR_GSUP43_73_HI_BER_MON:
        server_rate = UTIL_GLOBAL_ENET_10GE_RATE;
        data_rate = EPMM_LINK_10G;
        break;
    case EPMM_UNUSED_40G:
    case EPMM_40GE:
    {        
        /* compute exco3m index */
        PMC_ASSERT(link % 4 == 0, EPMM_ERR_INVALID_MODE , 0 , 0);
        exco3m_inst = link/4;
        if (EPMM_40GE == datapath) 
        {
            server_rate = UTIL_GLOBAL_ENET_40GE_XCD_RATE;
        } else
        {
            server_rate = UTIL_GLOBAL_ENET_40GE_RATE;
        }
        data_rate = EPMM_LINK_40G;
        break;
    };
    case EPMM_UNUSED_100G:
    case EPMM_100GE:
        data_rate = EPMM_LINK_100G;
        server_rate = UTIL_GLOBAL_ENET_100GE_RATE; 
        pkt_size = 415;
        break;
    default:        
        data_rate = LAST_EPMM_DATA_RATE;
        break;
    };

    /* Determine datapath cfg */
    switch (datapath) 
    {
    case EPMM_UNUSED_10G:
    case EPMM_UNUSED_40G:
    case EPMM_UNUSED_100G:
        scbs3_only  = TRUE;
        epmm_unused = TRUE;
        scbs3_user_bits_3_0 = ENET_EPMM_MPMA_SCBS_USER_BITS_UNSED;
        break;
    case EPMM_10GE_GSUP43_73_HI_BER_MON:
        scbs3_only          = TRUE;
        epmm_unused         = FALSE;
        dp_sel              = ENET_EPMM_DP_SEL_GSUP;
        scbs3_user_bits_3_0 = ENET_EPMM_MPMA_SCBS_USER_BITS_UNSED;
        break;
    case EPMM_10GE_GSUP43_73:
      scbs3_only          = TRUE;
      epmm_unused         = FALSE;
      dp_sel              = ENET_EPMM_DP_SEL_GSUP;
      scbs3_user_bits_3_0 = ENET_EPMM_MPMA_SCBS_USER_BITS_GSUP;
      break;
    case EPMM_10GE_CBR:
    case EPMM_10GE_CBR_GSUP43_73_HI_BER_MON:
    case EPMM_40GE:
    case EPMM_100GE:
      scbs3_only          = FALSE;
      epmm_unused         = FALSE;
      dp_sel              = ENET_EPMM_DP_SEL_GSUP_CBR;
      scbs3_user_bits_3_0 = ENET_EPMM_MPMA_SCBS_USER_BITS_CBR; 
      break;
    default: 
        scbs3_only          = TRUE;
        epmm_unused         = TRUE;
        scbs3_user_bits_3_0 = ENET_EPMM_MPMA_SCBS_USER_BITS_UNSED;
        break;
    };

    /* Configure Links (they are enabled in activate functions) */    
    /* We only do it when we use more then  the MPMA::SCBS3 */
    if (PMC_SUCCESS == ret_val &&  FALSE == epmm_unused ) 
    {
        epmm_buffer_init(epmm_buffer, hndl);

        if (UTIL_GLOBAL_ENET_10GE_RATE == server_rate) 
        {
            epmm_field_ING_10G_LINK_RESET_set(epmm_buffer, hndl, link, enable==TRUE?FALSE:TRUE);
            epmm_field_ING_10G_LINK_ENABLE_set(epmm_buffer, hndl, link, 0);
            epmm_field_ING_10G_LINK_DP_SEL_set(epmm_buffer, hndl, link, dp_sel);
        } else if (UTIL_GLOBAL_ENET_40GE_RATE == server_rate ||
                   UTIL_GLOBAL_ENET_40GE_XCD_RATE == server_rate) 
        {
            epmm_field_ING_40G_LINK_RESET_set(epmm_buffer, hndl, exco3m_inst, enable==TRUE?FALSE:TRUE);
            epmm_field_ING_40G_LINK_ENABLE_set(epmm_buffer, hndl, exco3m_inst, 0);
        } else 
        {
            epmm_field_ING_100G_LINK_RESET_set(epmm_buffer, hndl, enable==TRUE?FALSE:TRUE);
            epmm_field_ING_100G_LINK_ENABLE_set(epmm_buffer, hndl, 0);            
        };
 
        epmm_buffer_flush(epmm_buffer);
    };


    /* Configure MPMA */
    if (enable) 
    {
        ret_val = mpma_ch_prov(hndl->mpma_handle, 
                               link,              
                               server_rate,       
                               pkt_size,
                               UTIL_OPSA_T_FRAME_UNUSED,
                               UTIL_OPSA_EPS_MIN, 
                               scbs3_user_bits_3_0,
                               scbs3_only,              
                               cfg_pp_int,        
                               cfg_pp_n,          
                               cfg_pp_d,          
                               epsilon);
        /* keep MPMA pkt configruation for pair MPMO configuration */
        hndl->var.cfg_pp_n = cfg_pp_n;
        hndl->var.cfg_pp_d = cfg_pp_d;
        hndl->var.cfg_pp_int = cfg_pp_int;
    } else 
    {
        ret_val = mpma_ch_deprov(hndl->mpma_handle, link);
    };

    /* GSup43 Rx component config */
    if (PMC_SUCCESS == ret_val && 
        (EPMM_10GE_GSUP43_73 == datapath || 
         EPMM_10GE_GSUP43_73_HI_BER_MON == datapath ||
         EPMM_10GE_CBR_GSUP43_73_HI_BER_MON == datapath)
        && TRUE == enable)
    {
        ret_val = gsup43_c73_rx_cfg(hndl->gsup43_c73_handle[link],
                                    ENET_EPMM_GSUP43_C73_LOS_INV,
                                    ENET_EPMM_GSUP43_C73_DSCRD_ZONE,
                                    ENET_EPMM_GSUP43_C73_RX_SIG_OS_DSCRD_EN,
                                    ENET_EPMM_GSUP43_C73_RX_SEQ_OS_DSCRD_EN, 
                                    ENET_EPMM_GSUP43_C73_RX_OS_ALL_DSCRD,
                                    ENET_EPMM_GSUP43_C73_RX_FULL_OS_RPT_DIS,
                                    ENET_EPMM_GSUP43_C73_RX_MIN_OS_RPT,
                                    ENET_EPMM_GSUP43_C73_RX_PKT_TYPE,
                                    ENET_EPMM_GSUP43_C73_RX_OS_TYPE,
                                    ENET_EPMM_GSUP43_C73_RX_125US_CNT);
    };
    if (TRUE == enable)
    {
        hndl->var.data_rate[link] = data_rate;
        hndl->var.datapath[link] = datapath;
    }
    else
    {
        hndl->var.data_rate[link] = LAST_EPMM_DATA_RATE;
        hndl->var.datapath[link] = EPMM_DATAPATH_UNUSED;
    }

    PMC_RETURN(ret_val);

} /* epmm_ingress_cfg */

/*******************************************************************************
*  epmm_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reset and unconfigure epmm mpmo link.
*
* INPUTS:
*   hndl         - handle to the EPMM instance to be operated on
*   num_links    - link to configure
*   links        - indices of links to cleanup
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
PUBLIC void epmm_mpmo_cleanup(epmm_handle_t  *hndl,
                              UINT32          num_links, 
                              UINT32         *links)
{
    PMC_ENTRY();

    /* Clean MPMO */
    mpmo_ch_clean(hndl->mpmo_handle,links,num_links);   

    PMC_RETURN();

} /* epmm_mpmo_cleanup */

/*******************************************************************************
*  epmm_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reset and unconfigure epmm link.
*
* INPUTS:
*   hndl         - handle to the EPMM instance to be operated on
*   num_links    - link to configure
*   links        - indices of links to cleanup
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
PUBLIC void epmm_cleanup(epmm_handle_t  *hndl,
                         UINT32          num_links, 
                         UINT32         *links)
{
    UINT32 dp_sel    = ENET_EPMM_DP_SEL_GSUP;
    epmm_buffer_t epmm_buffer[1];
    UINT32       exco3m_inst          = 0;
    const UINT32 enable = FALSE;
    UINT32 link_itr;
    UINT32 link_idx;

    PMC_ENTRY();

    /* Enable/disable specified datapath */
    /* Register 0x20 + 0x04*N(N=0:11): EPMM120_TOP - ING  10G Link Configuration Register */
    /* Register 0x50 + 0x04*N(N=0: 2): EPMM120_TOP - ING  40G Link Configuration Register */
    /* Register 0x5C                 : EPMM120_TOP - ING 100G Link Configuration Register */
    /* Register 0x1000 + 0x0400*A(A=0:2): CBR_40G(A) - Configuration */
    /* Register 0x2000: CBR_100G - Configuration */

    epmm_buffer_init(epmm_buffer, hndl);

    for (link_itr = 0; link_itr < num_links; link_itr++)
    {
        link_idx = links[link_itr];

        epmm_field_ING_10G_LINK_RESET_set(epmm_buffer, hndl, link_idx, enable==TRUE?FALSE:TRUE);
        epmm_field_ING_10G_LINK_ENABLE_set(epmm_buffer, hndl, link_idx, 0);
        epmm_field_ING_10G_LINK_DP_SEL_set(epmm_buffer, hndl, link_idx, dp_sel);
        epmm_field_EGR_10G_LINK_RESET_set(epmm_buffer, hndl, link_idx, enable==TRUE?FALSE:TRUE);
        epmm_field_EGR_10G_LINK_ENABLE_set(epmm_buffer, hndl, link_idx, 0);

        
        if (0 == (link_idx % 4))
        {
            exco3m_inst = link_idx/4;
            epmm_field_ING_40G_LINK_RESET_set(epmm_buffer, hndl, exco3m_inst, enable==TRUE?FALSE:TRUE);
            epmm_field_ING_40G_LINK_ENABLE_set(epmm_buffer, hndl, exco3m_inst, 0);
            epmm_field_EGR_40G_LINK_RESET_set(epmm_buffer, hndl, exco3m_inst, enable==TRUE?FALSE:TRUE);
            epmm_field_EGR_40G_LINK_ENABLE_set(epmm_buffer, hndl, exco3m_inst, 0);
            exco3m_cfg( hndl->exco3m_handle[exco3m_inst], link_idx, EXCO3M_GMP_709_40GE, enable, FALSE);
        }
        
        if (0 == link_idx)
        {
            epmm_field_ING_100G_LINK_RESET_set(epmm_buffer, hndl, enable==TRUE?FALSE:TRUE);
            epmm_field_ING_100G_LINK_ENABLE_set(epmm_buffer, hndl, 0);   
            epmm_field_EGR_100G_LINK_RESET_set(epmm_buffer, hndl, enable==TRUE?FALSE:TRUE);
            epmm_field_EGR_100G_LINK_ENABLE_set(epmm_buffer, hndl, 0);    
            exco4m_cfg( hndl->exco4m_handle, link_idx, EXCO4M_GMP_709_100GE, enable, FALSE );       
        }
        /*
         * Rather than defining a new function that does the same thing 
         * simply call gsup43_c73_init to reset the module.
         */
        gsup43_c73_init(hndl->gsup43_c73_handle[link_idx],
                        PMC_ENERGY_STATE_RESET);

        hndl->var.data_rate[link_idx] = LAST_EPMM_DATA_RATE;
        hndl->var.datapath[link_idx] = EPMM_DATAPATH_UNUSED;
    }

    /* flush epmm buffer */
    epmm_buffer_flush(epmm_buffer);

    /* Clean MPMA */
    mpma_ch_clean(hndl->mpma_handle,links,num_links);

    /* Clean MPMO */
    mpmo_ch_clean(hndl->mpmo_handle,links,num_links);   


    PMC_RETURN();

} /* epmm_cleanup */

/*******************************************************************************
*  epmm_egress_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure appropriate transdecoder based on client data rate (10GE, 40GE, 100GE)
*   Configure packet monitor
*
* INPUTS:
*   hndl         - handle to the EPMM instance to be operated on
*   link         - link to configure
*   datapath     - data path to configure
*   enable       - TRUE: Enable datapath. FALSE: Disable datapath.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_egress_cfg(epmm_handle_t *hndl, 
                                 UINT32 link, 
                                 epmm_datapath_t datapath, 
                                 BOOL8 enable)
{
    UINT32 dp_sel    = ENET_EPMM_DP_SEL_GSUP;
    epmm_buffer_t epmm_buffer[1];
    BOOL8 epmm_unused           = FALSE;
    DOUBLE server_rate          = 0;
    UINT32 cfg_pp_n             = 0;
    UINT32 cfg_pp_d             = 0;
    UINT32 cfg_pp_int           = 0;
    PMC_ERROR ret_val       = PMC_SUCCESS;
    UINT32 exco3m_inst      = 0;

    PMC_ENTRY();
       
    /* Translate server rate */
    switch (datapath) 
    {
    case EPMM_UNUSED_10G:
    case EPMM_10GE_GSUP43_73:
    case EPMM_10GE_GSUP43_73_HI_BER_MON:
    case EPMM_10GE_CBR:
    case EPMM_10GE_CBR_GSUP43_73_HI_BER_MON:
        server_rate = UTIL_GLOBAL_ENET_10GE_RATE;
        break;
    case EPMM_UNUSED_40G:
    case EPMM_40GE:
    {
        server_rate = UTIL_GLOBAL_ENET_40GE_RATE;
        /* compute exco3m index */
        PMC_ASSERT(link % 4 == 0, EPMM_ERR_INVALID_MODE , 0 , 0);
        exco3m_inst = link/4;
        break;
    };
    case EPMM_UNUSED_100G:
    case EPMM_100GE:
        server_rate = UTIL_GLOBAL_ENET_100GE_RATE; 
        break;
    default:         
        break;
    }

    /* Determine datapath cfg */
    switch (datapath) 
    {
    case EPMM_UNUSED_10G:
    case EPMM_UNUSED_40G:
    case EPMM_UNUSED_100G:
    case EPMM_10GE_GSUP43_73_HI_BER_MON:
        epmm_unused = TRUE;
        break;
    case EPMM_10GE_GSUP43_73:
        epmm_unused        = FALSE;
        dp_sel              = ENET_EPMM_DP_SEL_GSUP;
        break;
    case EPMM_10GE_CBR:
    case EPMM_10GE_CBR_GSUP43_73_HI_BER_MON:
    case EPMM_40GE:
    case EPMM_100GE:
        epmm_unused        = FALSE;
        dp_sel              = ENET_EPMM_DP_SEL_GSUP_CBR;
        break;
    default:
        epmm_unused = TRUE;
        break;
    };

    /* Configure Links (they are enabled in activate functions) */    
    if ( PMC_SUCCESS == ret_val && FALSE == epmm_unused)
    {
        epmm_buffer_init(epmm_buffer, hndl);

        if (UTIL_GLOBAL_ENET_10GE_RATE == server_rate) 
        {
            epmm_field_EGR_10G_LINK_RESET_set(epmm_buffer, hndl, link, enable==TRUE?FALSE:TRUE);
            epmm_field_EGR_10G_LINK_ENABLE_set(epmm_buffer, hndl, link, 0);
            epmm_field_EGR_10G_LINK_DP_SEL_set(epmm_buffer, hndl, link, dp_sel);
        } else if (UTIL_GLOBAL_ENET_40GE_RATE == server_rate) 
        {
            epmm_field_EGR_40G_LINK_RESET_set(epmm_buffer, hndl, exco3m_inst, enable==TRUE?FALSE:TRUE);
            epmm_field_EGR_40G_LINK_ENABLE_set(epmm_buffer, hndl, exco3m_inst, 0);
        } else 
        {
            epmm_field_EGR_100G_LINK_RESET_set(epmm_buffer, hndl, enable==TRUE?FALSE:TRUE);
            epmm_field_EGR_100G_LINK_ENABLE_set(epmm_buffer, hndl, 0);            
        };
 
        epmm_buffer_flush(epmm_buffer);
    };

    /* MPMO configuration for CBR datapaths (when provisionning is local) */
    if ( hndl->var.mpmo_cfg_mode == FALSE && (
             datapath == EPMM_10GE_CBR || 
             datapath == EPMM_10GE_CBR_GSUP43_73_HI_BER_MON ||
             datapath == EPMM_40GE ||
             datapath == EPMM_100GE ))   
    {        
        if( enable ) {
            /* get configuration form ingress MPMA config */
            cfg_pp_n = hndl->var.cfg_pp_n;
            cfg_pp_d = hndl->var.cfg_pp_d;
            cfg_pp_int = hndl->var.cfg_pp_int;
            
            ret_val = mpmo_ch_prov(hndl->mpmo_handle, 
                                   link,              
                                   server_rate,       
                                   ENET_MPMA_PKT_SIZE,
                                   cfg_pp_int,        
                                   cfg_pp_n,          
                                   cfg_pp_d,          
                                   UTIL_OPSA_T_FRAME_UNUSED,   
                                   FALSE);
            if (PMC_SUCCESS == ret_val)
                ret_val = mpmo_ch_activate(hndl->mpmo_handle,link);
            
        } else 
        {
            if (PMC_SUCCESS == ret_val)
                ret_val = mpmo_ch_deactivate(hndl->mpmo_handle,link);
            if (PMC_SUCCESS == ret_val)
                ret_val = mpmo_ch_deprov(hndl->mpmo_handle, link); 
        }      
    }
     
    /* config datapath components  */
    if (PMC_SUCCESS == ret_val && 
        (EPMM_10GE_GSUP43_73 == datapath))
    {
        if( enable ) {
            ret_val = gsup43_c73_tx_cfg(hndl->gsup43_c73_handle[link],
                                        ENET_EPMM_GSUP43_C73_TX_ERR_DROP,
                                        ENET_EPMM_GSUP43_C73_TX_PKT_TYPE,
                                        ENET_EPMM_GSUP43_C73_TX_OS_TYPE,
                                        ENET_EPMM_GSUP43_C73_FORCE_MIN_IPG,
                                        ENET_EPMM_GSUP43_C73_UNIDIR_EN,
                                        ENET_EPMM_GSUP43_C73_SEND_IDLE_EN,
                                        ENET_EPMM_GSUP43_C73_SEND_LF_EN,
                                        ENET_EPMM_GSUP43_C73_SEND_RF_EN,
                                        ENET_EPMM_GSUP43_C73_FIFO_HMARK);
        };   
    } else if (PMC_SUCCESS == ret_val && EPMM_40GE == datapath) 
    {
        exco3m_cfg( hndl->exco3m_handle[exco3m_inst], link, EXCO3M_GMP_709_40GE, enable, FALSE );
    } else if (PMC_SUCCESS == ret_val && EPMM_100GE == datapath) 
    {
        exco4m_cfg( hndl->exco4m_handle, link, EXCO4M_GMP_709_100GE, enable, FALSE );
    }
    
    PMC_RETURN(ret_val);

} /* epmm_egress_cfg */

/*******************************************************************************
*  epmm_mpma_data_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Gets MPMA packet size and packet period information for a channel.
*
* INPUTS:
*   hndl                 - handle to the EPMM instance to be operated on
*   link                  - channel ID to be configured Valid range: 0 - 11
*   pkt_size_ptr        - pointer to configured packet size
*   pkt_per_int_ptr     - pointer to configured integer portion of packet period
*   pkt_per_n_ptr       - pointer to configured numerator of packet period 
*                         fraction
*   pkt_per_d_ptr       - pointer to configured denominator of packet period 
*                         fraction
*   epsilon_ptr         - pointer to the epsilon value.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_mpma_data_get(epmm_handle_t *hndl,
                                    UINT32 link,
                                    UINT32 *pkt_size_ptr,
                                    UINT32 *pkt_per_int_ptr,
                                    UINT32 *pkt_per_n_ptr,
                                    UINT32 *pkt_per_d_ptr,
                                    DOUBLE *epsilon_ptr)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
                                
    ret_val = mpma_pkt_period_var_get(hndl->mpma_handle, /* handle */
                                      link,
                                      pkt_size_ptr,
                                      pkt_per_int_ptr,
                                      pkt_per_n_ptr,
                                      pkt_per_d_ptr,
                                      epsilon_ptr);
    PMC_RETURN(ret_val);
                           
} /* epmm_mpma_data_get */  

/*******************************************************************************
*  epmm_mpmo_data_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Gets MPMO packet size and packet period information for a channel.
*
* INPUTS:
*   hndl                - handle to the EPMM instance to be operated on
*   link                - channel ID to be configured Valid range: 0 - 11
*   pkt_size_ptr        - pointer to configured packet size
*   pkt_per_int_ptr     - pointer to configured integer portion of packet period
*   pkt_per_n_ptr       - pointer to configured numerator of packet period 
*                         fraction
*   pkt_per_d_ptr       - pointer to configured denominator of packet period 
*                         fraction
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_mpmo_data_get(epmm_handle_t *hndl,
                                    UINT32 link,
                                    UINT32 *pkt_size_ptr,
                                    UINT32 *pkt_per_int_ptr,
                                    UINT32 *pkt_per_n_ptr,
                                    UINT32 *pkt_per_d_ptr)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
                                
    ret_val = mpmo_pkt_period_var_get(hndl->mpmo_handle, /* handle */
                                      link,
                                      pkt_size_ptr,
                                      pkt_per_int_ptr,
                                      pkt_per_n_ptr,
                                      pkt_per_d_ptr);
    PMC_RETURN(ret_val);
                           
} /* epmm_mpmo_data_get */ 


/*******************************************************************************
*  epmm_mpmo_ch_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provisions a channel for the specified rate.
*   This function determines the parameters to configure for the MPMO datapath
*   and DSP. It also evaluates the scheduler to see if the specified rate can
*   be allocated. \n\n
*
*   It is the caller's responsability to ensure that this is only called for 
*   the following datapath types: \n
*   EPMM_10GE_CBR \n
*   EPMM_10GE_CBR_GSUP43_73_HI_BER_MON \n
*   EPMM_40GE \n
*   EPMM_100GE \n
*
* INPUTS:
*   hndl              - handle to the MPMO instance to be operated on
*   link              - EPMM link to provision
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
*   PMC_SUCCESS
*
* NOTES:
*   There are two stages to enable a channel:
*   1. provision/deprovision: whereby the resources (CFC pages, scheduler 
*      entries) are allocated 
*   2. activate/deactivate: whereby the settings are written to hardware and 
*      the per channel enable bit is set
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_mpmo_ch_prov(epmm_handle_t *hndl, UINT32 link, 
                                   DOUBLE client_bitrate, 
                                   UINT32 pkt_size, UINT32 pkt_per_int, 
                                   UINT32 pkt_per_n, UINT32 pkt_per_d,
                                   util_opsa_cn_frame_period_t cn_frame_period, 
                                   BOOL8 golden_cn_en)    
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if(hndl->var.mpmo_cfg_mode == FALSE)
    {
        ret_val = EPMM_ERR_INVALID_MODE;
    }

    if(ret_val==PMC_SUCCESS)
    {
        ret_val = mpmo_ch_prov(hndl->mpmo_handle, /* handle */
                               link,             /* link */
                               client_bitrate,      /* client_bitrate */
                               pkt_size,         /* pkt_size */
                               pkt_per_int,      /* pkt_per_int */
                               pkt_per_n,        /* pkt_per_n_ptr */
                               pkt_per_d,        /* pkt_per_d_ptr */
                               UTIL_OPSA_T_FRAME_UNUSED,   /* cn_frame_period */
                               FALSE);              /* golden_cn_en */
    }

    if (PMC_SUCCESS == ret_val)
        ret_val = mpmo_ch_activate(hndl->mpmo_handle,link);

    PMC_RETURN(ret_val);
                           
} /* epmm_mpmo_ch_prov */                           

/*******************************************************************************
*  epmm_mpmo_ch_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovisions a channel
*
* INPUTS:
*   hndl              - handle to the MPMO instance to be operated on
*   link              - EPMM link to provision
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_mpmo_ch_deprov(epmm_handle_t *hndl, 
                                     UINT32 link)    
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if(hndl->var.mpmo_cfg_mode == FALSE)
    {
        ret_val = EPMM_ERR_INVALID_MODE;
    }

    if (PMC_SUCCESS == ret_val)
        ret_val = mpmo_ch_deactivate(hndl->mpmo_handle,link);

    if(ret_val==PMC_SUCCESS)
    {                                
        ret_val = mpmo_ch_deprov(hndl->mpmo_handle, /* handle */
                                 link);             
    }

    PMC_RETURN(ret_val);
                           
} /* epmm_mpmo_ch_deprov */ 


/*******************************************************************************
*  epmm_rx_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable Rx INGRESS path
*
* INPUTS:
*   hndl     - handle to the EPMM instance to be operated on
*   link     - link to configure
*   datapath - datapath to configure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_rx_activate(epmm_handle_t *hndl, 
                                  UINT32 link, 
                                  epmm_datapath_t datapath)
{
    UINT32 exco3m_inst      = 0;
    PMC_ERROR ret_val = PMC_SUCCESS;
    PMC_ENTRY();

    /* Note: Link configuration needs to happen before this, or cannot write anything */
    /* Take one GSup block out of reset */

    /* Bring up 10G datapath */
    if ( datapath == EPMM_10GE_GSUP43_73) 
    {
        gsup43_c73_rx_en(hndl->gsup43_c73_handle[link], TRUE);
    } 
    if (EPMM_10GE_GSUP43_73_HI_BER_MON == datapath ||        
        EPMM_10GE_CBR_GSUP43_73_HI_BER_MON == datapath)
    {
        gsup43_c73_pcs_decoder_en(hndl->gsup43_c73_handle[link], TRUE);
    }

    ret_val = mpma_ch_activate(hndl->mpma_handle,link,TRUE);
    if (PMC_SUCCESS == ret_val) 
    {
    
        switch( datapath )
        {
        case EPMM_10GE_CBR:
        case EPMM_10GE_CBR_GSUP43_73_HI_BER_MON:
        case EPMM_10GE_GSUP43_73:
        case EPMM_10GE_GSUP43_73_HI_BER_MON:
        {
            epmm_field_ING_10G_LINK_ENABLE_set( NULL, hndl, link, 1);
            break;
        }
        case EPMM_40GE:
        {
            PMC_ASSERT(link % 4 == 0, EPMM_ERR_INVALID_MODE , 0 , 0);
            exco3m_inst = link/4;            
            epmm_field_ING_40G_LINK_ENABLE_set( NULL, hndl, exco3m_inst, 1);
            break;
        }
        case EPMM_100GE :
        {
            epmm_field_ING_100G_LINK_ENABLE_set( NULL, hndl, 1);
            break;
        }
        default :
        {
            /* do nothing for EPMM_UNUSED case */
        }
        }
    }
    
    PMC_RETURN(ret_val);
} /* epmm_rx_activate */

/*******************************************************************************
*  epmm_rx_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable Rx path.
*
* INPUTS:
*   hndl     - handle to the EPMM instance to be operated on
*   link     - link to configure
*   datapath - datapath to configure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_rx_deactivate(epmm_handle_t *hndl, 
                                    UINT32 link, 
                                    epmm_datapath_t datapath)
{
    UINT32 exco3m_inst      = 0;
    PMC_ERROR ret_val = PMC_SUCCESS;
    PMC_ENTRY();

    switch( datapath )
    {
    case EPMM_10GE_GSUP43_73:
        gsup43_c73_rx_en(hndl->gsup43_c73_handle[link], FALSE);
        epmm_field_ING_10G_LINK_ENABLE_set( NULL, hndl, link, 0);
        break;

    case EPMM_10GE_GSUP43_73_HI_BER_MON:
    case EPMM_10GE_CBR_GSUP43_73_HI_BER_MON:
        gsup43_c73_pcs_decoder_en(hndl->gsup43_c73_handle[link], FALSE);
        epmm_field_ING_10G_LINK_ENABLE_set( NULL, hndl, link, 0);
        break;

    case EPMM_40GE:
    {
        PMC_ASSERT(link % 4 == 0, EPMM_ERR_INVALID_MODE , 0 , 0);
        exco3m_inst = link/4;  
        epmm_field_ING_40G_LINK_ENABLE_set( NULL, hndl, exco3m_inst, 0);
        break;
    }
    case EPMM_100GE :
    {
        epmm_field_ING_100G_LINK_ENABLE_set( NULL, hndl, 0);
        break;
    }
    default :
    {
        /* do nothing for EPMM_UNUSED case */
    }
    }
    ret_val = mpma_ch_deactivate(hndl->mpma_handle,link);

    PMC_RETURN(ret_val);

} /* epmm_rx_deactivate */

/*******************************************************************************
*  epmm_tx_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable Tx EGRESS path
*
* INPUTS:
*   hndl     - handle to the EPMM instance to be operated on
*   link     - link to configure
*   datapath - datapath to configure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_tx_activate(epmm_handle_t *hndl, 
                                  UINT32 link, 
                                  epmm_datapath_t datapath)
{
    UINT32 exco3m_inst      = 0;
    PMC_ERROR ret_val = PMC_SUCCESS;
    PMC_ENTRY();

    /* Bring up 10G datapath */
    if (EPMM_10GE_GSUP43_73 ==  datapath) 
    {
        gsup43_c73_tx_prbs_en ( hndl->gsup43_c73_handle[link], FALSE );
        gsup43_c73_tx_en      ( hndl->gsup43_c73_handle[link], TRUE  );
    }
    if (PMC_SUCCESS == ret_val) 
    {
        switch( datapath )
        {
        case EPMM_10GE_CBR:
        case EPMM_10GE_CBR_GSUP43_73_HI_BER_MON:
        case EPMM_10GE_GSUP43_73 :
        {
            epmm_field_EGR_10G_LINK_ENABLE_set( NULL, hndl, link, 1);
            break;
        }
        case EPMM_40GE:
        {
            PMC_ASSERT(link % 4 == 0, EPMM_ERR_INVALID_MODE , 0 , 0);
            exco3m_inst = link/4;  
            epmm_field_EGR_40G_LINK_ENABLE_set( NULL, hndl, exco3m_inst, 1);
            break;
       }
        case EPMM_100GE :
        {
            epmm_field_EGR_100G_LINK_ENABLE_set( NULL, hndl, 1);
            break;
        }
        default :
        {
            /* do nothing for EPMM_UNUSED case */
        }
        }
    }

    PMC_RETURN(ret_val);
} /* epmm_tx_activate */

/*******************************************************************************
*  epmm_tx_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable Tx path.
*
* INPUTS:
*   hndl     - handle to the EPMM instance to be operated on
*   link     - link to configure
*   datapath - datapath to configure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_tx_deactivate(epmm_handle_t *hndl, 
                                    UINT32 link, 
                                    epmm_datapath_t datapath)
{
    UINT32 exco3m_inst      = 0;
    PMC_ERROR ret_val = PMC_SUCCESS;
    PMC_ENTRY();

    switch( datapath )
    {
      case EPMM_10GE_GSUP43_73 :
      case EPMM_10GE_CBR:
      case EPMM_10GE_CBR_GSUP43_73_HI_BER_MON:
        {
          epmm_field_EGR_10G_LINK_ENABLE_set( NULL, hndl, link, 0);
          break;
        }
      case EPMM_40GE:
        {
            PMC_ASSERT(link % 4 == 0, EPMM_ERR_INVALID_MODE , 0 , 0);
            exco3m_inst = link/4;  
            epmm_field_EGR_40G_LINK_ENABLE_set( NULL, hndl, exco3m_inst, 0);
            break;
       }
      case EPMM_100GE :
        {
          epmm_field_EGR_100G_LINK_ENABLE_set( NULL, hndl, 0);
          break;
        }
      default :
        {
          /* do nothing for EPMM_UNUSED case */
        }
    }

    if ( datapath == EPMM_10GE_GSUP43_73 ) 
    {
      gsup43_c73_tx_en      ( hndl->gsup43_c73_handle[link], FALSE );
    } 

    PMC_RETURN(ret_val);
} /* epmm_tx_deactivate */

/*******************************************************************************
* Run-time Operations
*******************************************************************************/
/*******************************************************************************
*  epmm_tx_lane_info_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrieves the TX_LANE_ID and TX_LANE_SKEW_FIFO_ADDR for
*   40GE/100GE datapath
*
* INPUTS:
*   hndl         - handle to the EPMM instance to be operated on
*   link         - link to configure
*   epmm_path    - path to retrieve the TX_LANE_ID and TX_LANE_SKEW_FIFO_ADDR
*
* OUTPUTS:
*   txLaneId            - array to store TX_LANE_ID, the first 4 elements are
*                         valid for 40GE while the whole 20 elements are valid for 100GE.
*   txLaneSkewFifoAddr  - array to store TX_LANE_SKEW_FIFO_ADDR, the first 4 elements are
*                         valid for 40GE while the whole 20 elements are valid for 100GE.
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_tx_lane_info_get(epmm_handle_t *hndl, 
                                       UINT32 link, 
                                       epmm_datapath_t epmm_path,  
                                       UINT8 txLaneId[20],
                                       UINT8 txLaneSkewFifoAddr[20])
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    switch ( epmm_path )
    { 
    case EPMM_40GE:
        if (3 > (link / 4))
        {
            exco3m_tx_lane_info_get(hndl->exco3m_handle[link/4], txLaneId, txLaneSkewFifoAddr);
        }
        else
        {
            rc = EPMM_ERR_INVALID_PARAMETERS;            
        }
        break;
    case EPMM_100GE:

        exco4m_tx_lane_info_get(hndl->exco4m_handle, txLaneId, txLaneSkewFifoAddr);
        break;
    default:
        rc = EPMM_ERR_INVALID_PARAMETERS;
        break;
    }
    

    PMC_RETURN(rc);

} /* epmm_tx_lane_info_get */

/*******************************************************************************
* Run-time Operations
*******************************************************************************/
/*******************************************************************************
*  epmm_lf_os_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure and enable local fault (LF) ordered sets.
*
* INPUTS:
*   hndl         - handle to the EPMM instance to be operated on
*   link         - link to configure
*   epmm_path    - path to force insertion of the LF ordered set
*   dir          - path direction
*   enable       - TRUE: Enable insertion. FALSE: Disable insertion.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*  epmm_path may be set to one of the following:
*                  0 : Ingress GSup43 path
*                  1 : Ingress CBR 10G path
*                  2 : Egress  CBR 10G path
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_lf_os_cfg(epmm_handle_t *hndl, 
                                UINT32 link, 
                                epmm_datapath_t epmm_path,  
                                epmm_link_direction_t   dir,
                                BOOL8 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* Configure value and enable/disable insertion */
    /* Register 0xC: EPMM120_TOP - Global Local Fault Ordered set Configuration Register */
    /* Register 0x60  + 0x4*N(N=0:11): EPMM120_TOP - ING Local and Remote Fault Ordered set Enable Register */
    /* Register 0x130 + 0x4*N(N=0:11): EPMM120_TOP - EGR Local and Remote Fault Ordered set Enable Register */

    switch ( epmm_path )
    { 
    case EPMM_10GE_GSUP43_73 :        
        switch (dir)                
        {
        case EPMM_LINK_DIR_INGRESS:      
            if (TRUE == enable)
            {
                epmm_field_ING_GSUP10G_FORCE_RF_set( NULL, hndl, link, 0);
            }
            epmm_field_ING_GSUP10G_FORCE_LF_set( NULL, hndl, link, enable );
            break;
		case EPMM_LINK_DIR_EGRESS:
			if ((EPMM_OS_NO_FORCE == epmm_field_EGR_GSUP_TX_SW_FORCE_get(NULL, hndl, link)) && !enable)
			{
				break;
			}
            epmm_field_EGR_GSUP_TX_SW_FORCE_set( NULL, hndl, link, TRUE == enable ? EPMM_LF_OS_FORCE : EPMM_OS_NO_FORCE);
			if (!enable)
			{
				gsup43_c73_TX_SW_RST_set(hndl->gsup43_c73_handle[link], 0);
				gsup43_c73_TX_SW_RST_set(hndl->gsup43_c73_handle[link], 1);
				gsup43_c73_TX_SW_RST_set(hndl->gsup43_c73_handle[link], 0);
			}
            break;
        default:
            rc = EPMM_ERR_INVALID_PARAMETERS;
            break;
        }
        break;
                
    case EPMM_10GE_CBR  :  
    case EPMM_10GE_CBR_GSUP43_73_HI_BER_MON:  
        switch (dir)
        {
        case EPMM_LINK_DIR_INGRESS:            
            if (TRUE == enable)
            {
                epmm_field_ING_CBR10G_FORCE_RF_set( NULL, hndl, link, 0 );                
            }
            epmm_field_ING_CBR10G_FORCE_LF_set( NULL, hndl, link, enable );            
            epmm_field_ING_CBR10G_OS_FORCE_EN_set( NULL, hndl, link, enable);
            break;
        case EPMM_LINK_DIR_EGRESS:    
            if (TRUE == enable)
            {
                epmm_field_EGR_CBR10G_FORCE_RF_set( NULL, hndl, link, 0 );
            }
            epmm_field_EGR_CBR10G_FORCE_LF_set( NULL, hndl, link, enable );
            epmm_field_EGR_CBR10G_OS_FORCE_EN_set( NULL, hndl, link, enable);
            break;
        default:
            rc = EPMM_ERR_INVALID_PARAMETERS;
            break;
        }
        break;
    case EPMM_40GE:
        if (3 > (link / 4))
        {
            switch (dir)                
            {
            case EPMM_LINK_DIR_INGRESS:
                /* 322102 In EXCO4M (100G CBR block) and EXCO3M (40G CBR block), all the Rx and Tx are with respect to OTN, not Ethernet. */
                exco3m_tx_lf_os_cfg(hndl->exco3m_handle[link/4], enable);
                break;
            case EPMM_LINK_DIR_EGRESS:
                exco3m_rx_lf_os_cfg(hndl->exco3m_handle[link/4], enable);
                break;
            default:
                rc = EPMM_ERR_INVALID_PARAMETERS;
                break;
            }
        }
        else
        {
            rc = EPMM_ERR_INVALID_PARAMETERS;            
        }
        break;
    case EPMM_100GE:

        switch (dir)                
        {
        case EPMM_LINK_DIR_INGRESS:
            exco4m_tx_lf_os_cfg(hndl->exco4m_handle, enable);
            break;
        case EPMM_LINK_DIR_EGRESS:
            exco4m_rx_lf_os_cfg(hndl->exco4m_handle, enable);
            break;
        default:
            rc = EPMM_ERR_INVALID_PARAMETERS;
            break;
        }
        break;
    default:
        rc = EPMM_ERR_INVALID_PARAMETERS;
        break;
    }
    

    PMC_RETURN(rc);

} /* epmm_lf_os_cfg */


/*******************************************************************************
*  epmm_lf_os_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the configuration and enable status of local fault (LF) ordered sets.
*
* INPUTS:
*   hndl         - handle to the EPMM instance to be operated on
*   link         - link to configure
*   epmm_path    - path to force insertion of the LF ordered set
*   dir          - path direction
*
* OUTPUTS:
*   enable       - TRUE: Enable insertion. FALSE: Disable insertion.
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*  epmm_path may be set to one of the following:
*                  0 : Ingress GSup43 path
*                  1 : Ingress CBR 10G path
*                  2 : Egress  CBR 10G path
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_lf_os_cfg_get(epmm_handle_t *hndl, 
                                    UINT32 link, 
                                    epmm_datapath_t epmm_path,  
                                    epmm_link_direction_t   dir,
                                    BOOL8 *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();
    *enable = FALSE;

    /* Get the configurion value and enable/disable insertion status*/
    /* Register 0xC: EPMM120_TOP - Global Local Fault Ordered set Configuration Register */
    /* Register 0x60  + 0x4*N(N=0:11): EPMM120_TOP - ING Local and Remote Fault Ordered set Enable Register */
    /* Register 0x130 + 0x4*N(N=0:11): EPMM120_TOP - EGR Local and Remote Fault Ordered set Enable Register */

    switch ( epmm_path )
    { 
    case EPMM_10GE_GSUP43_73 :        
        switch (dir)                
        {
        case EPMM_LINK_DIR_INGRESS:      
            *enable = epmm_field_ING_GSUP10G_FORCE_LF_get( NULL, hndl, link);
            break;
        case EPMM_LINK_DIR_EGRESS:
            if (EPMM_LF_OS_FORCE == epmm_field_EGR_GSUP_TX_SW_FORCE_get(NULL, hndl, link))
                *enable = TRUE;
            break;
        default:
            rc = EPMM_ERR_INVALID_PARAMETERS;
            break;
        }
        break;
                
    case EPMM_10GE_CBR  :  
    case EPMM_10GE_CBR_GSUP43_73_HI_BER_MON:  
        switch (dir)
        {
        case EPMM_LINK_DIR_INGRESS:            
            if (epmm_field_ING_CBR10G_OS_FORCE_EN_get( NULL, hndl, link) && epmm_field_ING_CBR10G_FORCE_LF_get( NULL, hndl, link))
                *enable = TRUE;            
            break;
            
        case EPMM_LINK_DIR_EGRESS:    
            if (epmm_field_EGR_CBR10G_OS_FORCE_EN_get( NULL, hndl, link) && epmm_field_EGR_CBR10G_FORCE_LF_get( NULL, hndl, link))
                *enable = TRUE;
            break;
            
        default:
            rc = EPMM_ERR_INVALID_PARAMETERS;
            break;
        }
        break;
    case EPMM_40GE:
        if (3 > (link / 4))
        {
            switch (dir)                
            {
            case EPMM_LINK_DIR_INGRESS:
                /* 322102 In EXCO4M (100G CBR block) and EXCO3M (40G CBR block), all the Rx and Tx are with respect to OTN, not Ethernet. */
                exco3m_tx_lf_os_cfg_get(hndl->exco3m_handle[link/4],enable);
                break;
            case EPMM_LINK_DIR_EGRESS:
                exco3m_rx_lf_os_cfg_get(hndl->exco3m_handle[link/4],enable);
                break;
            default:
                rc = EPMM_ERR_INVALID_PARAMETERS;
                break;
            }
        }
        else
        {
            rc = EPMM_ERR_INVALID_PARAMETERS;            
        }
        break;
    case EPMM_100GE:
        switch (dir)                
        {
        case EPMM_LINK_DIR_INGRESS:
            exco4m_tx_lf_os_cfg_get(hndl->exco4m_handle,enable);
            break;
        case EPMM_LINK_DIR_EGRESS:
            exco4m_rx_lf_os_cfg_get(hndl->exco4m_handle,enable);
            break;
        default:
            rc = EPMM_ERR_INVALID_PARAMETERS;
            break;
        }
        break;
    default:
        rc = EPMM_ERR_INVALID_PARAMETERS;
        break;
    }
    

    PMC_RETURN(rc);

} /* epmm_lf_os_cfg_get */



/*******************************************************************************
*  epmm_lf_os_auto_insert_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable automatic insertion of local fault (LF) ordered sets.
*
* INPUTS:
*   hndl         - handle to the EPMM instance to be operated on
*   link         - link to configure
*   epmm_path    - path to automatic insertion of the LF ordered set
*   dir          - path direction
*   enable       - TRUE: Enable insertion. FALSE: Disable insertion.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*  epmm_path may be set to one of the following:
*                  1 : Ingress CBR 10G path
*                  2 : Egress  CBR 10G path
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_lf_os_auto_insert_cfg(epmm_handle_t *hndl, 
                                UINT32 link, 
                                epmm_datapath_t epmm_path,  
                                epmm_link_direction_t   dir,
                                BOOL8 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();


    switch ( epmm_path )
    { 
    case EPMM_10GE_CBR  :  
    case EPMM_10GE_CBR_GSUP43_73_HI_BER_MON:  
        switch (dir)
        {
        case EPMM_LINK_DIR_INGRESS:            
            epmm_field_ING_CBR10G_FORCE_LF_set( NULL, hndl, link, enable );            
            break;
        case EPMM_LINK_DIR_EGRESS:    
            epmm_field_EGR_CBR10G_FORCE_LF_set( NULL, hndl, link, enable );
            break;
        default:
            rc = EPMM_ERR_INVALID_PARAMETERS;
            break;
        }
        break;
    case EPMM_10GE_GSUP43_73 :
    case EPMM_40GE:
    case EPMM_100GE:
    default:
        rc = EPMM_ERR_INVALID_PARAMETERS;
        break;
    }
    

    PMC_RETURN(rc);

} /* epmm_lf_os_auto_insert_cfg */

/*******************************************************************************
*  epmm_lf_os_auto_insert_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrives the enable status of automatic insertion of local fault (LF) ordered sets.
*
* INPUTS:
*   hndl         - handle to the EPMM instance to be operated on
*   link         - link to configure
*   epmm_path    - path to automatic insertion of the LF ordered set
*   dir          - path direction
*
* OUTPUTS:
*   enable       - TRUE: Enable insertion. FALSE: Disable insertion.
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*  epmm_path may be set to one of the following:
*                  1 : Ingress CBR 10G path
*                  2 : Egress  CBR 10G path
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_lf_os_auto_insert_cfg_get(epmm_handle_t *hndl, 
                                UINT32 link, 
                                epmm_datapath_t epmm_path,  
                                epmm_link_direction_t   dir,
                                BOOL8 *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();


    switch ( epmm_path )
    { 
    case EPMM_10GE_CBR  :  
    case EPMM_10GE_CBR_GSUP43_73_HI_BER_MON:  
        switch (dir)
        {
        case EPMM_LINK_DIR_INGRESS:            
            *enable = epmm_field_ING_CBR10G_FORCE_LF_get( NULL, hndl, link);            
            break;
        case EPMM_LINK_DIR_EGRESS:    
            *enable = epmm_field_EGR_CBR10G_FORCE_LF_get( NULL, hndl, link);
            break;
        default:
            rc = EPMM_ERR_INVALID_PARAMETERS;
            break;
        }
        break;
    case EPMM_10GE_GSUP43_73 :
    case EPMM_40GE:
    case EPMM_100GE:
    default:
        rc = EPMM_ERR_INVALID_PARAMETERS;
        break;
    }
    

    PMC_RETURN(rc);

} /* epmm_lf_os_auto_insert_cfg_get */


/*******************************************************************************
*  epmm_lf_is_force_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves the OS_FORCE_EN flag from the egress direction for the 
*  the specified link.
*
* INPUTS:
*   hndl         - handle to the EPMM instance to be operated on
*   link         - link to configure
*
* OUTPUTS:
*   force_enable - Assigned the result of the register.
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC void epmm_lf_is_force_enable(epmm_handle_t *hndl, 
                                    UINT32 link, 
                                    BOOL8 *force_enable)
{
    PMC_ENTRY();

    *force_enable = epmm_field_EGR_CBR10G_OS_FORCE_EN_get(NULL, hndl, link);

    PMC_RETURN();
} /* epmm_lf_is_force_enable */


/*******************************************************************************
*  epmm_lf_set_force_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Sets the OS_FORCE_EN for the specified link.
*  the specified link.
*
* INPUTS:
*  hndl         - handle to the EPMM instance to be operated on
*  link         - link to configure
*  enable       - Whether to enable(1) or disable(0) the register.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC void epmm_lf_set_force_enable(epmm_handle_t *hndl, 
                                    UINT32 link, 
                                    BOOL8 enable)
{
    PMC_ENTRY();

    epmm_field_EGR_CBR10G_OS_FORCE_EN_set(NULL, hndl, link, (UINT32)enable);

    PMC_RETURN();
} /* epmm_lf_set_force_enable */


/*******************************************************************************
*  epmm_rf_os_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure and enable remote fault (RF) ordered sets.
*
* INPUTS:
*   hndl         - handle to the EPMM instance to be operated on
*   link         - link to configure
*   epmm_path    - Path to force insertion of the RF ordered set
*   dir          - path direction
*   enable       - TRUE: Enable insertion. FALSE: Disable insertion.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*  epmm_path may be set to one of the following:
*                  0 - Ingress GSup43 path
*                  1 - Ingress CBR 10G path
*                  2 - Egress  CBR 10G path
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_rf_os_cfg(epmm_handle_t *hndl, 
                                UINT32 link, 
                                epmm_datapath_t epmm_path, 
                                epmm_link_direction_t   dir,
                                BOOL8 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* Configure value and enable/disable insertion */
    /* Register 0x10: EPMM120_TOP - Global Remote Fault Ordered set Configuration Register */
    /* Register 0x60  + 0x4*N(N=0:11): EPMM120_TOP - ING Local and Remote Fault Ordered set Enable Register */
    /* Register 0x130 + 0x4*N(N=0:11): EPMM120_TOP - EGR Local and Remote Fault Ordered set Enable Register */

    switch ( epmm_path )
    {
    case EPMM_10GE_GSUP43_73:
        switch (dir)                
        {
        case EPMM_LINK_DIR_INGRESS:    
            if (TRUE == enable)
            {
                epmm_field_ING_GSUP10G_FORCE_LF_set( NULL, hndl, link, 0);
            }
            epmm_field_ING_GSUP10G_FORCE_RF_set( NULL, hndl, link, enable );
            break;
        case EPMM_LINK_DIR_EGRESS:
			if ((EPMM_OS_NO_FORCE == epmm_field_EGR_GSUP_TX_SW_FORCE_get(NULL, hndl, link)) && !enable)
			{
				break;
			}
            epmm_field_EGR_GSUP_TX_SW_FORCE_set( NULL, hndl, link, TRUE == enable ? EPMM_RF_OS_FORCE : EPMM_OS_NO_FORCE);
			if (!enable)
			{
				gsup43_c73_TX_SW_RST_set(hndl->gsup43_c73_handle[link], 0);
				gsup43_c73_TX_SW_RST_set(hndl->gsup43_c73_handle[link], 1);
				gsup43_c73_TX_SW_RST_set(hndl->gsup43_c73_handle[link], 0);
			}
            break;
        default:
            rc = EPMM_ERR_INVALID_PARAMETERS;
            break;
        }
        break;
    case  EPMM_10GE_CBR :
    case EPMM_10GE_CBR_GSUP43_73_HI_BER_MON:
        switch (dir)                
        {
        case EPMM_LINK_DIR_INGRESS:     
            if (TRUE == enable)
            {
                epmm_field_ING_CBR10G_FORCE_LF_set( NULL, hndl, link, 0);
            }
            epmm_field_ING_CBR10G_FORCE_RF_set( NULL, hndl, link, enable );
            epmm_field_ING_CBR10G_OS_FORCE_EN_set( NULL, hndl, link, enable);
            break;
        case EPMM_LINK_DIR_EGRESS:
            if (TRUE == enable)
            {
                epmm_field_EGR_CBR10G_FORCE_LF_set( NULL, hndl, link, 0);
            }
            epmm_field_EGR_CBR10G_FORCE_RF_set( NULL, hndl, link, enable );
            epmm_field_EGR_CBR10G_OS_FORCE_EN_set( NULL, hndl, link, enable);
            break;
        default:
            rc = EPMM_ERR_INVALID_PARAMETERS;
            break;
        }
        break;
    case EPMM_40GE:
        if (3 > (link / 4))
        {
            switch (dir)                
            {
            case EPMM_LINK_DIR_INGRESS:
                /* 322102 In EXCO4M (100G CBR block) and EXCO3M (40G CBR block), all the Rx and Tx are with respect to OTN, not Ethernet. */
                exco3m_tx_rf_os_cfg(hndl->exco3m_handle[link/4], enable);
                break;
            case EPMM_LINK_DIR_EGRESS:
                exco3m_rx_rf_os_cfg(hndl->exco3m_handle[link/4], enable);
                break;
            default:
                rc = EPMM_ERR_INVALID_PARAMETERS;
                break;
            }
        }
        else
        {
            rc = EPMM_ERR_INVALID_PARAMETERS;            
        }
        break;
    case EPMM_100GE:

        switch (dir)                
        {
        case EPMM_LINK_DIR_INGRESS:
            exco4m_tx_rf_os_cfg(hndl->exco4m_handle, enable);
            break;
        case EPMM_LINK_DIR_EGRESS:
            exco4m_rx_rf_os_cfg(hndl->exco4m_handle, enable);
            break;
        default:
            rc = EPMM_ERR_INVALID_PARAMETERS;
            break;
        }
        break;
    default:
        rc = EPMM_ERR_INVALID_PARAMETERS;
        break;
    }
    
    PMC_RETURN(rc);

} /* epmm_rf_os_cfg */


/*******************************************************************************
*  epmm_rf_os_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the configuration and enable status of remote fault (RF) ordered sets.
*
* INPUTS:
*   hndl         - handle to the EPMM instance to be operated on
*   link         - link to configure
*   epmm_path    - path to force insertion of the LF ordered set
*   dir          - path direction
*
* OUTPUTS:
*   enable       - TRUE: Enable insertion. FALSE: Disable insertion.
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*  epmm_path may be set to one of the following:
*                  0 : Ingress GSup43 path
*                  1 : Ingress CBR 10G path
*                  2 : Egress  CBR 10G path
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_rf_os_cfg_get(epmm_handle_t *hndl, 
                                    UINT32 link, 
                                    epmm_datapath_t epmm_path,  
                                    epmm_link_direction_t   dir,
                                    BOOL8 *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();
    *enable = FALSE;

    /* Get the configurion value and enable/disable insertion status*/
    /* Register 0xC: EPMM120_TOP - Global Local Fault Ordered set Configuration Register */
    /* Register 0x60  + 0x4*N(N=0:11): EPMM120_TOP - ING Local and Remote Fault Ordered set Enable Register */
    /* Register 0x130 + 0x4*N(N=0:11): EPMM120_TOP - EGR Local and Remote Fault Ordered set Enable Register */

    switch ( epmm_path )
    { 
    case EPMM_10GE_GSUP43_73 :        
        switch (dir)                
        {
        case EPMM_LINK_DIR_INGRESS:      
            *enable = epmm_field_ING_GSUP10G_FORCE_RF_get( NULL, hndl, link);
            break;
        case EPMM_LINK_DIR_EGRESS:
            if (EPMM_RF_OS_FORCE == epmm_field_EGR_GSUP_TX_SW_FORCE_get(NULL, hndl, link)) 
                *enable = TRUE;
            break;
        default:
            rc = EPMM_ERR_INVALID_PARAMETERS;
            break;
        }
        break;
                
    case EPMM_10GE_CBR  :  
    case EPMM_10GE_CBR_GSUP43_73_HI_BER_MON:  
        switch (dir)
        {
        case EPMM_LINK_DIR_INGRESS:            
            if (epmm_field_ING_CBR10G_OS_FORCE_EN_get( NULL, hndl, link) && epmm_field_ING_CBR10G_FORCE_RF_get( NULL, hndl, link))
                *enable = TRUE;
            break;
            
        case EPMM_LINK_DIR_EGRESS:    
            if (epmm_field_EGR_CBR10G_OS_FORCE_EN_get( NULL, hndl, link) && epmm_field_EGR_CBR10G_FORCE_RF_get( NULL, hndl, link))
                *enable = TRUE;
            break;
            
        default:
            rc = EPMM_ERR_INVALID_PARAMETERS;
            break;
        }
        break;
    case EPMM_40GE:
        if (3 > (link / 4))
        {
            switch (dir)                
            {
            case EPMM_LINK_DIR_INGRESS:
                /* 322102 In EXCO4M (100G CBR block) and EXCO3M (40G CBR block), all the Rx and Tx are with respect to OTN, not Ethernet. */
                exco3m_tx_rf_os_cfg_get(hndl->exco3m_handle[link/4],enable);
                break;
            case EPMM_LINK_DIR_EGRESS:
                exco3m_rx_rf_os_cfg_get(hndl->exco3m_handle[link/4],enable);
                break;
            default:
                rc = EPMM_ERR_INVALID_PARAMETERS;
                break;
            }
        }
        else
        {
            rc = EPMM_ERR_INVALID_PARAMETERS;            
        }
        break;
    case EPMM_100GE:
        switch (dir)                
        {
        case EPMM_LINK_DIR_INGRESS:
            exco4m_tx_rf_os_cfg_get(hndl->exco4m_handle,enable);
            break;
        case EPMM_LINK_DIR_EGRESS:
            exco4m_rx_rf_os_cfg_get(hndl->exco4m_handle,enable);
            break;
        default:
            rc = EPMM_ERR_INVALID_PARAMETERS;
            break;
        }
        break;
    default:
        rc = EPMM_ERR_INVALID_PARAMETERS;
        break;
    }
    

    PMC_RETURN(rc);

} /* epmm_rf_os_cfg_get */



/*******************************************************************************
*  epmm_rf_os_auto_insert_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable automatic insertion of remote fault (RF) ordered sets.
*
* INPUTS:
*   hndl         - handle to the EPMM instance to be operated on
*   link         - link to configure
*   epmm_path    - Path to force insertion of the RF ordered set
*   dir          - path direction
*   enable       - TRUE: Enable insertion. FALSE: Disable insertion.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*  epmm_path may be set to one of the following:
*                  1 - Ingress CBR 10G path
*                  2 - Egress  CBR 10G path
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_rf_os_auto_insert_cfg(epmm_handle_t *hndl, 
                                UINT32 link, 
                                epmm_datapath_t epmm_path, 
                                epmm_link_direction_t   dir,
                                BOOL8 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    switch ( epmm_path )
    {
    case EPMM_10GE_CBR :
    case EPMM_10GE_CBR_GSUP43_73_HI_BER_MON:
        switch (dir)                
        {
        case EPMM_LINK_DIR_INGRESS:     
            epmm_field_ING_CBR10G_FORCE_RF_set( NULL, hndl, link, enable );
            break;
        case EPMM_LINK_DIR_EGRESS:
            epmm_field_EGR_CBR10G_FORCE_RF_set( NULL, hndl, link, enable );
            break;
        default:
            rc = EPMM_ERR_INVALID_PARAMETERS;
            break;
        }
        break;
    case EPMM_10GE_GSUP43_73:
    case EPMM_40GE:
    case EPMM_100GE:
    default:
        rc = EPMM_ERR_INVALID_PARAMETERS;
        break;
    }
    
    PMC_RETURN(rc);

} /* epmm_rf_os_auto_insert_cfg */

/*******************************************************************************
*  epmm_sw_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Global soft reset of 10, 40, and/or 100G datapaths
*
* INPUTS:
*   hndl         - Handle to the EPMM instance
*   link         - ENET link     to reset
*   datapath     - ENET datapath to reset
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_sw_reset(epmm_handle_t *hndl, 
                               UINT32 link, 
                               epmm_datapath_t datapath)
{
    PMC_ENTRY();

    /* UINT32 reg_value = 0; */

    switch( datapath )
    {
    case EPMM_10GE_GSUP43_73 :
    case EPMM_10GE_GSUP43_73_HI_BER_MON:
    case EPMM_10GE_CBR       :
    case EPMM_10GE_CBR_GSUP43_73_HI_BER_MON:
        {   /*                                  b:    h:    N:    value: */
        epmm_field_ING_10G_LINK_ENABLE_set( NULL, hndl, link, 0 );
        epmm_field_ING_10G_LINK_RESET_set ( NULL, hndl, link, 1 );
        epmm_field_EGR_10G_LINK_ENABLE_set( NULL, hndl, link, 0 );
        epmm_field_EGR_10G_LINK_RESET_set ( NULL, hndl, link, 1 );
        break;
        }
    case EPMM_40GE :
        {
        /*                                  b:    h:    N:      value: */
        epmm_field_ING_40G_LINK_ENABLE_set( NULL, hndl, link/4, 0 );
        epmm_field_ING_40G_LINK_RESET_set ( NULL, hndl, link/4, 1 );
        epmm_field_EGR_40G_LINK_ENABLE_set( NULL, hndl, link/4, 0 );
        epmm_field_EGR_40G_LINK_RESET_set ( NULL, hndl, link/4, 1 );
        break;
        }
    case EPMM_100GE :
        {   /*                                   b:    h:    value: */
        epmm_field_ING_100G_LINK_ENABLE_set( NULL, hndl, 0 );
        epmm_field_ING_100G_LINK_RESET_set ( NULL, hndl, 1 );
        epmm_field_EGR_100G_LINK_ENABLE_set( NULL, hndl, 0 );
        epmm_field_EGR_100G_LINK_RESET_set ( NULL, hndl, 1 );
        break;
        }
    default :
        {
        /* Do nothing for EPMM_UNUSED case */
        break;
        }
    }

    /* Reset 10G, 40G, or 100G datapath */
    /* reg_value = epmm_reg_GLOBAL_SOFT_RESET_CONFIG_REG_read(NULL, hndl); */

    /* if      (bw == 10)  { reg_value |= EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG_BIT_SW_RESET_10G_MSK; } */
    /* else if (bw == 40)  { reg_value |= EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG_BIT_SW_RESET_40G_MSK; } */
    /* else if (bw == 100) { reg_value |= EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG_BIT_SW_RESET_100G_MSK; } */

    /* epmm_reg_GLOBAL_SOFT_RESET_CONFIG_REG_write(NULL, hndl, reg_value); */

    PMC_RETURN(PMC_SUCCESS);

} /* epmm_sw_reset */

/*******************************************************************************
*  epmm_lowpwr
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Global soft reset of 10, 40, and/or 100G datapaths
*
* INPUTS:
*   hndl         - Handle to the EPMM instance
*   link         - ENET link     to reset
*   datapath     - ENET datapath to reset
*   value        - TRUE/FALSE = LOWPWR/Not
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_lowpwr(epmm_handle_t *hndl, 
                             UINT32 link, 
                             epmm_datapath_t datapath, 
                             BOOL8 value)
{
    PMC_ENTRY();

    switch( datapath )
    {
    case EPMM_40GE  : { exco3m_lowpwr(hndl->exco3m_handle[link/4], value); break; }
    case EPMM_100GE : { exco4m_lowpwr(hndl->exco4m_handle,         value); break; }
    case EPMM_10GE_GSUP43_73 :
    case EPMM_10GE_GSUP43_73_HI_BER_MON:
    case EPMM_10GE_CBR       :
    case EPMM_10GE_CBR_GSUP43_73_HI_BER_MON:
    default                  : { break; }
    }

    PMC_RETURN(PMC_SUCCESS);

} /* epmm_lowpwr */


/*******************************************************************************
*  epmm_mpmo_ckctl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure MPMO clock control ports in EPMM.
*
* INPUTS:
*   hndl              - Handle to the EPMM instance
*   ckctl_port        - clock control port number (0-25): \n
*                       Ports 0--11 are line side 10G SERDES. \n
*                       Ports 12--13 are line side 2.5G SERDES. \n
*                       Ports 14--25 are system side 10G SERDES. \n
*   chnl              - channel to map to the port specified by ckctl_port.
*   enable            - enables/disables CKCTL output
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   CBRC_ERR_NULL_HANDLE
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_mpmo_ckctl_cfg(epmm_handle_t *hndl,
                                     UINT32 ckctl_port, 
                                     UINT32 chnl,
                                     BOOL8 enable)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    ret_val = mpmo_ckctl_cfg(hndl->mpmo_handle, ckctl_port, chnl, enable);

    PMC_RETURN(ret_val);

} /* epmm_mpmo_ckctl_cfg */

/*******************************************************************************
*  epmm_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize EPMM var_t variables.
*
* INPUTS:
*   *hndl      - pointer to EPMM handle instance
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC void epmm_var_default_init(epmm_handle_t *hndl)
{
    UINT32 itr;
    PMC_ENTRY();

    hndl->var.cfg_pp_n = 0;
    hndl->var.cfg_pp_d = 0;
    hndl->var.cfg_pp_int = 0;
    for (itr = 0; itr < EPMM_NUM_LINK; itr++)
    {
        hndl->var.data_rate[itr] = LAST_EPMM_DATA_RATE;
        hndl->var.datapath[itr] = EPMM_DATAPATH_UNUSED;
    }
    PMC_RETURN();
}/* epmm_var_default_init*/

/*******************************************************************************
*  epmm_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the EPMM block energy state. 
*
* INPUTS:
*   hndl           - handle to the EPMM instance to be operated on
*   data_rate      - the data rate
*   energy_state   - energy_state
*
* OUTPUTS:
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PUBLIC void epmm_energy_state_reg_set (epmm_handle_t *hndl, 
                                       epmm_data_rate_t data_rate, 
                                       pmc_energy_state_t energy_state)
{
    BOOL8 val = (energy_state== PMC_ENERGY_STATE_RUNNING)?FALSE:TRUE;

    PMC_ENTRY();    
    PMC_ASSERT(hndl!=NULL, EPMM_ERR_INVALID_PARAMETERS, 0, 0);
    
    switch(data_rate)
    {
        case EPMM_LINK_10G:
            epmm_field_SW_RESET_10G_set(NULL, hndl, val);
            break;
        case EPMM_LINK_40G:
            epmm_field_SW_RESET_40G_set(NULL, hndl, val);
            break;
        case EPMM_LINK_100G:
            epmm_field_SW_RESET_100G_set(NULL, hndl, val);
            break;
        default:
            PMC_ASSERT(FALSE, EPMM_ERR_INVALID_PARAMETERS, 0, 0);
    }

    PMC_RETURN();
} /* epmm_energy_state_reg_set */


/*******************************************************************************
* FUNCTION: epmm_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   epmm_handle       - handle to the EPMM instance to be operated on
*   link              - Link [0..11]
*   non_null_ptr      - Pointer to be checked that can't be NULL
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_int_validate(epmm_handle_t    *epmm_handle,
                                   UINT32            link,
                                   void             *non_null_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != epmm_handle, EPMM_ERR_INVALID_PARAMETERS, 0, 0);
   
    if (link >= EPMM_NUM_LINK)
    {
        result = EPMM_ERR_INVALID_PARAMETERS;
    }

    if (NULL == non_null_ptr)
    {
        result = EPMM_ERR_INVALID_PARAMETERS;
    }


    PMC_RETURN(result);
} /* epmm_int_validate */


/*******************************************************************************
* FUNCTION: epmm_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   epmm_handle       - handle to the EPMM instance to be operated on
*   link              - Link [0..11]
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_int_chnl_enable(epmm_handle_t    *epmm_handle,
                                      UINT32            link,
                                      epmm_int_chnl_t  *int_table_ptr,
                                      epmm_int_chnl_t  *int_en_table_ptr,
                                      BOOL8              enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    BOOL8           has_exco4m;
    BOOL8           has_exco3m;
    UINT32          exco3m_idx;
    BOOL8           has_gsup43;
    UINT32          gsup43_idx;

    PMC_ENTRY();

    result = epmm_int_validate(epmm_handle, link, int_en_table_ptr);

    if (PMC_SUCCESS == result)
    {
        /* Get the right index for EXCO3M, GSUP43 for link */
        result = epmm_int_params_get(epmm_handle,
                                     epmm_handle->var.datapath[link],
                                     link,
                                     &has_exco4m,
                                     &has_exco3m,
                                     &exco3m_idx,
                                     &has_gsup43,
                                     &gsup43_idx);
    }

    if (PMC_SUCCESS == result && TRUE == has_gsup43)
    {
        result = gsup43_c73_int_chnl_enable(epmm_handle->gsup43_c73_handle[gsup43_idx],
                                            0,
                                            &int_table_ptr->gsup43_c73,
                                            &int_en_table_ptr->gsup43_c73,
                                            enable);
    }

    if (PMC_SUCCESS == result && TRUE == has_exco3m)
    {
        result = exco3m_int_chnl_enable(epmm_handle->exco3m_handle[exco3m_idx],
                                        exco3m_idx,
                                        &int_table_ptr->exco3m,
                                        &int_en_table_ptr->exco3m,
                                        enable);
    }
    
    if (PMC_SUCCESS == result && TRUE == has_exco4m)
    {
        result = exco4m_int_chnl_enable(epmm_handle->exco4m_handle,
                                        link,
                                        &int_table_ptr->exco4m,
                                        &int_en_table_ptr->exco4m,
                                        enable);
    }

    if (PMC_SUCCESS == result)
    {
        result = mpmo_int_chnl_enable(epmm_handle->mpmo_handle,
                                      link,
                                      &int_table_ptr->mpmo,
                                      &int_en_table_ptr->mpmo,
                                      enable);
    }
    if (PMC_SUCCESS == result)
    {
        result = mpma_int_chnl_enable(epmm_handle->mpma_handle,
                                      link,
                                      &int_table_ptr->mpma,
                                      &int_en_table_ptr->mpma,
                                      enable);
    }

    if (PMC_SUCCESS == result)
    {
        int_en_table_ptr->gsup43_c73_valid = has_gsup43;
        int_en_table_ptr->exco3m_valid = has_exco3m;
        int_en_table_ptr->exco4m_valid = has_exco4m;
    }

    PMC_RETURN(result);
} /* epmm_int_chnl_enable */




/*******************************************************************************
* FUNCTION: epmm_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   epmm_handle       - handle to the EPMM instance to be operated on
*   link              - Link [0..11]
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_int_chnl_clear(epmm_handle_t    *epmm_handle,
                                     UINT32            link,
                                     epmm_int_chnl_t  *int_table_ptr,
                                     epmm_int_chnl_t  *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    BOOL8           has_exco4m;
    BOOL8           has_exco3m;
    UINT32          exco3m_idx;
    BOOL8           has_gsup43;
    UINT32          gsup43_idx;

    PMC_ENTRY();

    result = epmm_int_validate(epmm_handle, link, int_en_table_ptr);

    if (PMC_SUCCESS == result)
    {
        /* Get the right index for EXCO3M, GSUP43 for link */
        result = epmm_int_params_get(epmm_handle,
                                     epmm_handle->var.datapath[link],
                                     link,
                                     &has_exco4m,
                                     &has_exco3m,
                                     &exco3m_idx,
                                     &has_gsup43,
                                     &gsup43_idx);
    }

    if (PMC_SUCCESS == result && TRUE == has_gsup43)
    {
        result = gsup43_c73_int_chnl_clear(epmm_handle->gsup43_c73_handle[gsup43_idx],
                                           0,
                                           UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, gsup43_c73),
                                           &int_en_table_ptr->gsup43_c73);
    }

    if (PMC_SUCCESS == result && TRUE == has_exco3m)
    {
        result = exco3m_int_chnl_clear(epmm_handle->exco3m_handle[exco3m_idx],
                                       exco3m_idx,
                                       UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, exco3m),
                                       &int_en_table_ptr->exco3m);
    }
    
    if (PMC_SUCCESS == result && TRUE == has_exco4m)
    {
        result = exco4m_int_chnl_clear(epmm_handle->exco4m_handle,
                                       link,
                                       UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, exco4m),
                                       &int_en_table_ptr->exco4m);
    }

    if (PMC_SUCCESS == result)
    {
        result = mpma_int_chnl_clear(epmm_handle->mpma_handle,
                                     link,
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, mpma),
                                     &int_en_table_ptr->mpma);
    }
    if (PMC_SUCCESS == result)
    {
        result = mpmo_int_chnl_clear(epmm_handle->mpmo_handle,
                                     link,
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, mpmo),
                                     &int_en_table_ptr->mpmo);
    }


    PMC_RETURN(result);
} /* epmm_int_chnl_clear */




/*******************************************************************************
* FUNCTION: epmm_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   epmm_handle       - handle to the EPMM instance to be operated on
*   link              - Link [0..11]
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
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_int_chnl_retrieve(epmm_handle_t    *epmm_handle,
                                        UINT32            link,
                                        epmm_int_chnl_t  *filt_table_ptr,
                                        epmm_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    BOOL8           has_exco4m;
    BOOL8           has_exco3m;
    UINT32          exco3m_idx;
    BOOL8           has_gsup43;
    UINT32          gsup43_idx;

    PMC_ENTRY();

    result = epmm_int_validate(epmm_handle, link, int_table_ptr);

    if (PMC_SUCCESS == result)
    {
        /* Get the right index for EXCO3M, GSUP43 for link */
        result = epmm_int_params_get(epmm_handle,
                                     epmm_handle->var.datapath[link],
                                     link,
                                     &has_exco4m,
                                     &has_exco3m,
                                     &exco3m_idx,
                                     &has_gsup43,
                                     &gsup43_idx);
    }

    if (PMC_SUCCESS == result && TRUE == has_gsup43)
    {
        result = gsup43_c73_int_chnl_retrieve(epmm_handle->gsup43_c73_handle[gsup43_idx],
                                              0,
                                              UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, gsup43_c73),
                                              &int_table_ptr->gsup43_c73);
        int_table_ptr->gsup43_c73_valid = TRUE;
    }

    if (PMC_SUCCESS == result && TRUE == has_exco3m)
    {
        result = exco3m_int_chnl_retrieve(epmm_handle->exco3m_handle[exco3m_idx],
                                          exco3m_idx,
                                          UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, exco3m),
                                          &int_table_ptr->exco3m);
    }
    
    if (PMC_SUCCESS == result && TRUE == has_exco4m)
    {
        result = exco4m_int_chnl_retrieve(epmm_handle->exco4m_handle,
                                          link,
                                          UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, exco4m),
                                          &int_table_ptr->exco4m);
    }
    
    if (PMC_SUCCESS == result)
    {
        result = mpma_int_chnl_retrieve(epmm_handle->mpma_handle,
                                        link,
                                        UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, mpma),
                                        &int_table_ptr->mpma);
    }

    if (PMC_SUCCESS == result)
    {
        result = mpmo_int_chnl_retrieve(epmm_handle->mpmo_handle,
                                        link,
                                        UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, mpmo),
                                        &int_table_ptr->mpmo);
    }
    if (PMC_SUCCESS == result)
    {
        int_table_ptr->gsup43_c73_valid = has_gsup43;
        int_table_ptr->exco3m_valid = has_exco3m;
        int_table_ptr->exco4m_valid = has_exco4m;
    }

    PMC_RETURN(result);
} /* epmm_int_chnl_retrieve */



/*******************************************************************************
* FUNCTION: epmm_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   epmm_handle       - handle to the EPMM instance to be operated on
*   link              - Link [0..11]
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_int_chnl_enabled_check(epmm_handle_t     *epmm_handle,
                                             UINT32             link,
                                             epmm_int_chnl_t   *int_en_table_ptr,
                                             BOOL8             *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    BOOL8           has_exco4m;
    BOOL8           has_exco3m;
    UINT32          exco3m_idx;
    BOOL8           has_gsup43;
    UINT32          gsup43_idx;

    PMC_ENTRY();

    result = epmm_int_validate(epmm_handle, link, int_found_ptr);

    if (PMC_SUCCESS == result)
    {
        /* Get the right index for EXCO3M, GSUP43 for link */
        result = epmm_int_params_get(epmm_handle,
                                     epmm_handle->var.datapath[link],
                                     link,
                                     &has_exco4m,
                                     &has_exco3m,
                                     &exco3m_idx,
                                     &has_gsup43,
                                     &gsup43_idx);
    }

    if ((PMC_SUCCESS == result && TRUE == has_gsup43) &&
        (FALSE == *int_found_ptr))
    {
        result = gsup43_c73_int_chnl_enabled_check(epmm_handle->gsup43_c73_handle[gsup43_idx],
                                                   0,
                                                   &int_en_table_ptr->gsup43_c73,
                                                   int_found_ptr);
    }

    if ((PMC_SUCCESS == result && TRUE == has_exco3m) &&
        (FALSE == *int_found_ptr))
    {
        result = exco3m_int_chnl_enabled_check(epmm_handle->exco3m_handle[link],
                                               link,
                                               &int_en_table_ptr->exco3m,
                                               int_found_ptr);
    }
    
    if ((PMC_SUCCESS == result && TRUE == has_exco4m) &&
        (FALSE == *int_found_ptr))
    {
        result = exco4m_int_chnl_enabled_check(epmm_handle->exco4m_handle,
                                               link,
                                               &int_en_table_ptr->exco4m,
                                               int_found_ptr);
    }

    if ((PMC_SUCCESS == result) &&
        (FALSE == *int_found_ptr))
    {
        result = mpma_int_chnl_enabled_check(epmm_handle->mpma_handle,
                                             link,
                                             &int_en_table_ptr->mpma,
                                             int_found_ptr);
    }
    if ((PMC_SUCCESS == result) &&
        (FALSE == *int_found_ptr))
    {
        result = mpmo_int_chnl_enabled_check(epmm_handle->mpmo_handle,
                                             link,
                                             &int_en_table_ptr->mpmo,
                                             int_found_ptr);
    }
    PMC_RETURN(result);
} /* epmm_int_chnl_enabled_check */


/*******************************************************************************
* epmm_int_params_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is a helper function to retrieve which interrupt structures
*   are associated with the current link.
* INPUTS:
*   epmm_handle            - handle to the EPMM instance to be operated on
*
*   datapath               - Enumerated EPMM datapath:
*                            EPMM_10GE_GSUP43_73
*                            EPMM_10GE_CBR
*                            EPMM_10GE_CBR_GSUP43_73_HI_BER_MON
*                            EPMM_40GE
*                            EPMM_100GE
*
*                            EPMM_UNUSED_10G,
*                            EPMM_UNUSED_40G,
*                            EPMM_UNUSED_100G,
*                            EPMM_DATAPATH_UNUSED
*
*   link                   - Link [0..11]
*
* OUTPUTS:
*   has_exco4m             - TRUE: EXCO4M interrupts are valid and should be
*                            retrieved
*
*   has_exco3m             - TRUE: EXCO3M interrupts are valid for index
*                            exco3m_idx and should be retrieved
*
*   exco3m_idx             - Index for the EXCO3M interrupts if
*                            has_exco3m=TRUE [0-2]
*
*   has_gsup43             - TRUE: GSUP43 interrupts are valid for index
*                            gsup43_idx and should be retrieved
*
*   gsup43_idx             - Index for the GSUP43 interrupts if
*                            has_gsup43=TRUE [0-11]
*
* RETURNS:
*   PMC_SUCCESS
*   LINEOTN_ERR_INVALID_PARAMETERS
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR epmm_int_params_get(epmm_handle_t        *epmm_handle,
                                      epmm_datapath_t       datapath,
                                      UINT32                link,
                                      BOOL8                *has_exco4m,
                                      BOOL8                *has_exco3m,
                                      UINT32               *exco3m_idx,
                                      BOOL8                *has_gsup43,
                                      UINT32               *gsup43_idx)
{
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();

    /* Initialize returned values */
    *has_exco4m = FALSE;
    *has_exco3m = FALSE;
    *exco3m_idx = 0;
    *has_gsup43 = FALSE;
    *gsup43_idx = 0;

    switch (datapath) 
    {
    case EPMM_10GE_GSUP43_73:
    case EPMM_10GE_GSUP43_73_HI_BER_MON:
    case EPMM_10GE_CBR:
    case EPMM_10GE_CBR_GSUP43_73_HI_BER_MON:
        *has_gsup43 = TRUE;
        *gsup43_idx = link;
        break;
    case EPMM_40GE:
        PMC_ASSERT(link % 4 == 0, EPMM_ERR_INVALID_MODE , 0 , 0);
        *has_exco3m = TRUE;
        *exco3m_idx = link / 4;
        break;
    case EPMM_100GE:
        *has_exco4m = TRUE;
        break;
    default:        
        break;
    };

    PMC_RETURN(result);
}/* epmm_int_params_get */


/*******************************************************************************
*  epmm_tx_upi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets expected UPI values in data or ordered set packet and the action 
*   to be done when incoming packet UPI does not match expected values: DROP or IGNORE.
*
* INPUTS:
*   hndl                - handle to the EPMM instance to be operated on
*   link                - link to configure
*   datapath            - datapath to configure
*   pkt_upi_val         - the value that identifies the transmit data as a 10GE data frame.
*   os_upi_val          - the value that identifies the transmit data as 10GE ordered set.
*   enable_drop         - when TRUE, incoming packet that has an UPI TYPE value that
*                         does not match either pkt_upi_val or os_upi_val
*                         are dropped.
*                         when FALSE, incoming data packet will be treated as a 
*                         data packet.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_tx_upi_set(epmm_handle_t  *hndl, 
                                 UINT32          link,
                                 epmm_datapath_t datapath,                                 
                                 UINT8           pkt_upi_val,
                                 UINT8           os_upi_val,
                                 BOOL8           enable_drop)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    PMC_ENTRY();


    if (datapath == EPMM_10GE_GSUP43_73 ) 
    {
        gsup43_c73_tx_upi_set(hndl->gsup43_c73_handle[link], 
                              pkt_upi_val,
                              os_upi_val,
                              enable_drop);
    } 
    else
    {
       ret_val = EPMM_ERR_INVALID_PARAMETERS; 
    }

    PMC_RETURN(ret_val);
} /* epmm_tx_upi_set */

/*******************************************************************************
*  epmm_tx_upi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves expected UPI values in data or ordered set packet and the action 
*   to be done when incoming packet UPI does not match expected values: DROP or IGNORE.
*
* INPUTS:
*   hndl                - handle to the EPMM instance to be operated on
*   link                - link to configure
*   datapath            - datapath to configure
*
* OUTPUTS:
*  *pkt_upi_val_ptr     - value that identifies the transmit data as a 10GE data frame.
*  *os_upi_val_ptr      - value that identifies the transmit data as 10GE ordered set.
*  *enable_drop_ptr     - when TRUE, incoming packet that has an UPI TYPE value that
*                         does not match either data_pkt_upi_val or os_pkt_upi_val
*                         are dropped.
*                         when FALSE, incoming data packet will be treated as a 
*                         data packet.
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_tx_upi_get(epmm_handle_t  *hndl, 
                                 UINT32          link,
                                 epmm_datapath_t datapath,                                 
                                 UINT8           *pkt_upi_val_ptr,
                                 UINT8           *os_upi_val_ptr,
                                 BOOL8           *enable_drop_ptr)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    PMC_ENTRY();


    if (datapath == EPMM_10GE_GSUP43_73 ) 
    {
        gsup43_c73_tx_upi_get(hndl->gsup43_c73_handle[link], 
                              pkt_upi_val_ptr,
                              os_upi_val_ptr,
                              enable_drop_ptr);
    } 
    else
    {
       ret_val = EPMM_ERR_INVALID_PARAMETERS; 
    }

    PMC_RETURN(ret_val);
} /* epmm_tx_upi_get */

/*******************************************************************************
*  epmm_rx_upi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the UPI values for data and ordered set frames.
*
* INPUTS:
*   hndl                - EPMM handle
*   link                - link to configure
*   datapath            - datapath to configure
*   pkt_upi_val         - Data frame UPI value
*   os_upi_val          - Ordered set frame UPI value
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_rx_upi_set(epmm_handle_t  *hndl, 
                                 UINT32          link,
                                 epmm_datapath_t datapath,                                 
                                 UINT8           pkt_upi_val,
                                 UINT8           os_upi_val)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    PMC_ENTRY();


    if (datapath == EPMM_10GE_GSUP43_73) 
    {

        gsup43_c73_rx_upi_set(hndl->gsup43_c73_handle[link], pkt_upi_val, os_upi_val);
    } 
    else
    {
       ret_val = EPMM_ERR_INVALID_PARAMETERS; 
    }

    PMC_RETURN(ret_val);
} /* epmm_rx_upi_set */

/*******************************************************************************
*  epmm_rx_upi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the configured UPI values for data and ordered set 
*   frames.
*
* INPUTS:
*   hndl                - handle to the EPMM instance to be operated on
*   link                - link to configure
*   datapath            - datapath to configure
*
* OUTPUTS:
*  *pkt_upi_val_ptr     - data frame UPI value
*  *os_upi_val_ptr      - Ordered set UPI
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_rx_upi_get(epmm_handle_t  *hndl, 
                                 UINT32          link,
                                 epmm_datapath_t datapath,                                 
                                 UINT8           *pkt_upi_val_ptr,
                                 UINT8           *os_upi_val_ptr)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    PMC_ENTRY();


    if (datapath == EPMM_10GE_GSUP43_73) 
    {
        gsup43_c73_rx_upi_get(hndl->gsup43_c73_handle[link], pkt_upi_val_ptr, os_upi_val_ptr);
    } 
    else
    {
       ret_val = EPMM_ERR_INVALID_PARAMETERS; 
    }

    PMC_RETURN(ret_val);
} /* epmm_rx_upi_get */


/*******************************************************************************
*  epmm_los_force_rf_action_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function onfigure EPMM to set RF ordered set on the 10GE egress direction
*   when RX_LOS is asserted on a 10GE port.
*   RX_LOS->Tx_RF for 10GE port in Gsup7.3 GFP mapping mode. 
*
* INPUTS:
*   hndl                - handle to the EPMM instance to be operated on
*   link                - link to configure
*   enable              - enable(TRUE) or disable(FALSE) the consequential action
*
* OUTPUTS:
*  none
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR epmm_los_force_rf_action_set(epmm_handle_t  *hndl, 
                                 UINT32          link,
                                 BOOL8           enable)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    PMC_ENTRY();

    /* EPMM:GSUP_C73:SEND_RF_EN is set to 1 by default in driver */

    /* Set TX_SW_FORCE to 0 which is 'No Force' - The forcing is to be done by CNSQ_ACT_EN */
    epmm_field_EGR_GSUP_TX_SW_FORCE_set(NULL, hndl, link, 0);

    /* Set the CNSQ_ACT_EN to either enable or disable the Consequential action in the GSUP path */
    epmm_field_EGR_GSUP_CNSQ_ACT_EN_set(NULL, hndl, link, (UINT32)enable);

    PMC_RETURN(ret_val);

} /* epmm_los_force_rf_action_set */


/*******************************************************************************
*  epmm_ing_reset_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves the value of the ING RESET register for the particular data 
*  rate and returns it.
*
* INPUTS:
*   hndl                - handle to the EPMM instance to be operated on
*   link                - link to retrieve
*   epmm_rate           - Data rate which determines the register to check
*
* OUTPUTS:
*  none
*
* RETURNS:
*  UINT33  - Register value
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 epmm_ing_reset_get(epmm_handle_t *hndl,
                                 UINT32 link,
                                 epmm_data_rate_t epmm_rate)
{
    UINT32 reg_value = 0xFFFF;

    PMC_ENTRY();

    switch (epmm_rate)
    {
      case EPMM_LINK_10G:
        reg_value = epmm_field_ING_10G_LINK_RESET_get(NULL,
                                                      hndl,
                                                      link);
        break;
        
      case EPMM_LINK_40G:
        reg_value = epmm_field_ING_40G_LINK_RESET_get(NULL,
                                                      hndl,
                                                      link);
        break;
        
      case EPMM_LINK_100G:
        reg_value = epmm_field_ING_100G_LINK_RESET_get(NULL,
                                                       hndl);
        break;
        
      default:
        break;
    }

    PMC_RETURN(reg_value);
}

/*
** Private Functions
*/

/*
** End of file
*/
