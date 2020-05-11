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

#include "etrans_loc.h"

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
const char ETRANS_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    ETRANS_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char ETRANS_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Public Functions
*/

/*
** Private Functions
*/
PRIVATE BOOL8 etrans_data_rate_start_state_test(etrans_handle_t *hndl,etrans_data_rate_t data_rate);

PRIVATE PMC_ERROR etrans_int_params_get(etrans_handle_t      *etrans_handle,
                                        etrans_datapath_t     datapath,
                                        UINT32                link,
                                        BOOL8                *has_exco4m,
                                        BOOL8                *has_exco3m,
                                        UINT32               *exco3m_idx,
                                        BOOL8                *has_gsup43,
                                        UINT32               *gsup43_idx,
                                        BOOL8                *has_etrans_10g,
                                        BOOL8                *has_etrans_40g,
                                        BOOL8                *has_etrans_100g,
                                        UINT32               *etrans_idx);

/*******************************************************************************
* etrans_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a ETRANS block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the ETRANS block relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - pointer to TSB name
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   etrans_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC etrans_handle_t *etrans_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name )
{
    etrans_handle_t *etrans_handle;
    UINT32        i;
    UINT32        tsb_base_addr;

    PMC_ENTRY();

    etrans_handle = (etrans_handle_t *) PMC_CTXT_CALLOC(sizeof(etrans_handle_t), parent);

    pmc_handle_init( parent, etrans_handle, sys_handle, PMC_MID_DIGI_ETRANS, tsb_name, base_address );

    /* Register log strings. */
    pmc_log_block_strings_register(ETRANS_LOG_ERR_STRINGS[0], ETRANS_LOG_ERR_TABLE_BASE, ETRANS_LOG_ERR_COUNT);

    /* Create handle for child blocks */
    for (i = 0; i <= MAX_ETRANS120_GSUP43_10G_GSUP43_10G_WRAP_GSUP43_C73; i++) {
        tsb_base_addr = base_address + BASE_ADDR_ETRANS120_GSUP43_10G_GSUP43_10G_WRAP_GSUP43_C73(i);
        etrans_handle->gsup43_c73_handle[i] = gsup43_c73_ctxt_create((pmc_handle_t *)etrans_handle, tsb_base_addr, sys_handle, "GSUP43_C73");
        PMC_CTXT_REGISTER_PTR(&etrans_handle->gsup43_c73_handle[i], parent)
    }
    for (i = 0; i <= MAX_ETRANS120_CBR_40G_EXCO3M; i++) {
        tsb_base_addr = base_address + BASE_ADDR_ETRANS120_CBR_40G_EXCO3M(i);
        etrans_handle->exco3m_handle[i] = exco3m_ctxt_create((pmc_handle_t *)etrans_handle, tsb_base_addr, sys_handle, "EXCO3M");
        PMC_CTXT_REGISTER_PTR(&etrans_handle->exco3m_handle[i], parent);
    }
    etrans_handle->exco4m_handle = exco4m_ctxt_create((pmc_handle_t *)etrans_handle, base_address+BASE_ADDR_ETRANS120_CBR_100G_EXCO4M, sys_handle, "EXCO4M");
    PMC_CTXT_REGISTER_PTR(&etrans_handle->exco4m_handle, parent);

    PMC_RETURN(etrans_handle);
} /* etrans_ctxt_create */

/*******************************************************************************
* etrans_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a ETRANS block instance.
*
*
* INPUTS:
*   *etrans_handle        - pointer to ETRANS handle instance
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
PUBLIC void etrans_ctxt_destroy(etrans_handle_t *etrans_handle)
{
    UINT32 i;
    
    PMC_ENTRY();

    for (i = 0; i <= MAX_ETRANS120_GSUP43_10G_GSUP43_10G_WRAP_GSUP43_C73; i++) {
        gsup43_c73_ctxt_destroy(etrans_handle->gsup43_c73_handle[i]);
    }
    for (i = 0; i <= MAX_ETRANS120_CBR_40G_EXCO3M; i++) {
      exco3m_ctxt_destroy(etrans_handle->exco3m_handle[i]);
    }
    exco4m_ctxt_destroy(etrans_handle->exco4m_handle);
 

    PMC_CTXT_FREE(&etrans_handle, etrans_handle);

    PMC_RETURN();
} /* etrans_ctxt_destroy */

/*******************************************************************************
* etrans_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a ETRANS block handle instance.
*
*
* INPUTS:
*   *etrans_handle        - pointer to ETRANS handle instance
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
PUBLIC void etrans_handle_init(etrans_handle_t *etrans_handle)
{
    UINT32 i;

    PMC_ENTRY();

    for (i = 0; i <= MAX_ETRANS120_GSUP43_10G_GSUP43_10G_WRAP_GSUP43_C73; i++) {
        gsup43_c73_handle_init(etrans_handle->gsup43_c73_handle[i]);
    }
    for (i = 0; i <= MAX_ETRANS120_CBR_40G_EXCO3M; i++) {
        exco3m_handle_init(etrans_handle->exco3m_handle[i]);
    }

    exco4m_handle_init(etrans_handle->exco4m_handle);

    etrans_var_default_init(etrans_handle);

    PMC_RETURN();

} /* etrans_handle_init */

/*******************************************************************************
* etrans_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on etrans.
*
*
* INPUTS:
*   *etrans_handle        - pointer to ETRANS handle instance
*   *restart_init_cfg_ptr - pointer to restart init config
*   top_energy_state_reg  - The expected energy state 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS : 
*       if successful
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etrans_handle_restart_init(etrans_handle_t *etrans_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                            pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 i;

    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(ETRANS_LOG_ERR_STRINGS[0], ETRANS_LOG_ERR_TABLE_BASE, ETRANS_LOG_ERR_COUNT);
    }

    for (i = 0; i <= MAX_ETRANS120_GSUP43_10G_GSUP43_10G_WRAP_GSUP43_C73 && PMC_SUCCESS == ret_val; i++) {
        gsup43_c73_handle_restart_init(etrans_handle->gsup43_c73_handle[i], restart_init_cfg_ptr, top_energy_state_reg);
    }
    for (i = 0; i <= MAX_ETRANS120_CBR_40G_EXCO3M && PMC_SUCCESS == ret_val; i++) {
        exco3m_handle_restart_init(etrans_handle->exco3m_handle[i], restart_init_cfg_ptr, top_energy_state_reg);
    }

    exco4m_handle_restart_init(etrans_handle->exco4m_handle, restart_init_cfg_ptr, top_energy_state_reg);
    PMC_RETURN(ret_val);
} /* etrans_handle_restart_init */

/*******************************************************************************
* etrans_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the ETRANS instance to the starting state for all
*   dynamic context.  All dynamic context will be returned to the initialization 
*   values that were generate during the call to etrans_handle_init().
*
*   This function is for use by parent modules that have provided all other
*   provisions for ensuring the state of all resources tracked by this
*   context has coherent register state.  
*
*   Note that link & datapath are only provided for the 
*   PMC_ENERGY_STATE_REQUEST_RUNNING energy state
*
* INPUTS:
*   *hndl                 - pointer to ETRANS handle instance
*   link                  - requested link 
*   datapath              - enum, requested datapath
*   energy_state          - enum defining state to set the ETRANS module to
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
PUBLIC PMC_ERROR etrans_init(etrans_handle_t *hndl, 
                             UINT32 link, 
                             etrans_datapath_t datapath,
                             pmc_energy_state_request_t energy_state)
{
    etrans_data_rate_t    data_rate = LAST_ETRANS_DATA_RATE;
    PMC_ERROR ret_val = PMC_SUCCESS;
    BOOL8 etrans_var_default = TRUE;

    PMC_ENTRY();
    PMC_ASSERT(hndl != NULL, ETRANS_LOG_ERR_INVALID_PARAMETERS, 0, 0); 
   
    /* handle energy request */
    switch(energy_state)
    {
    case PMC_ENERGY_STATE_REQUEST_RUNNING:       
    {
        /* set data_rate */
        switch (datapath) 
        {
        case ETRANS_BMP_GSUP43_C71_10GE_MAC:
        case ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC:
        case ETRANS_GFP_GSUP43_C73_10GE_MAC:
            data_rate = ETRANS_LINK_10G;
            break;
        case ETRANS_GMP_G709_40GE_MAC:
        case ETRANS_GFP_GSUP43_C62_G709_40GE_MAC:
            data_rate = ETRANS_LINK_40G;
            break;
        case ETRANS_GMP_G709_100GE_MAC:
        case ETRANS_GFP_GSUP43_C62_G709_100GE_MAC:
            data_rate = ETRANS_LINK_100G;
            break;
        default:
            PMC_ASSERT(FALSE, ETRANS_LOG_ERR_INVALID_PARAMETERS, 0, 0); 
            break;
        }

        if(PMC_SUCCESS == ret_val && etrans_start_state_test(hndl)== TRUE)
        {
            /* Configure non default valuse */
            etrans_block_non_default_cfg(hndl);
        }

        /* ETRANS TOP */
        if(PMC_SUCCESS == ret_val && etrans_data_rate_start_state_test(hndl,data_rate)== TRUE)
        {
            PMC_LOG_TRACE("configure ETRANS-top energy state\n");
            /* Configure ETRANS energy state */
            etrans_energy_state_reg_set(hndl, data_rate,PMC_ENERGY_STATE_RUNNING);        
        }                  
        break;
    }         
    case PMC_ENERGY_STATE_REQUEST_RESET:
    case PMC_ENERGY_STATE_REQUEST_OPTIMAL:
    {               
        if(PMC_SUCCESS == ret_val && 
           (etrans_data_rate_start_state_test(hndl,ETRANS_LINK_10G)== TRUE ||
            PMC_ENERGY_STATE_REQUEST_RESET == energy_state))
        {               
            /* Configure ETRANS energy state */
            etrans_energy_state_reg_set(hndl,ETRANS_LINK_10G ,PMC_ENERGY_STATE_RESET);        
        }  else 
        {
            etrans_var_default = FALSE;
        }
        if(PMC_SUCCESS == ret_val && 
           (etrans_data_rate_start_state_test(hndl,ETRANS_LINK_40G)== TRUE ||
            PMC_ENERGY_STATE_REQUEST_RESET == energy_state))
        {               
            /* Configure ETRANS energy state */
            etrans_energy_state_reg_set(hndl,ETRANS_LINK_40G ,PMC_ENERGY_STATE_RESET);        
        }  else 
        {
            etrans_var_default = FALSE;
        }
        if(PMC_SUCCESS == ret_val && 
           (etrans_data_rate_start_state_test(hndl,ETRANS_LINK_100G)== TRUE ||
            PMC_ENERGY_STATE_REQUEST_RESET == energy_state))
        {               
            /* Configure ETRANS energy state */
            etrans_energy_state_reg_set(hndl,ETRANS_LINK_100G ,PMC_ENERGY_STATE_RESET);        
        }  else 
        {
            etrans_var_default = FALSE;
        }
        if (TRUE == etrans_var_default)
        {        
            /* Configure etrans_var as default */
            etrans_var_default_init(hndl);
        }
        break;
    }        
    default:
        PMC_ASSERT(FALSE, ETRANS_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }
  
    PMC_RETURN(ret_val);
} /* etrans_init */


/*******************************************************************************
*  etrans_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize ETRANS var_t variables.
*
* INPUTS:
*   *hndl      - pointer to ETRANS handle instance
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC void etrans_var_default_init(etrans_handle_t *hndl)
{
    UINT32 itr;
    PMC_ENTRY();

    for (itr = 0; itr < ETRANS_NUM_LINKS; itr++)
    {
        hndl->var.data_rate[itr] = LAST_ETRANS_DATA_RATE;
        hndl->var.datapath[itr] = ETRANS_UNUSED;
        hndl->var.loopback[itr] = FALSE; 
    }
    PMC_RETURN();
}/* etrans_var_default_init*/


/*******************************************************************************
* etrans_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   ETRASNS instance.  
*
*
* INPUTS:
*   *hndl                       - pointer to ETRANS handle instance
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
PUBLIC BOOL8 etrans_start_state_test(etrans_handle_t *hndl)
{
    BOOL8 result = TRUE;    
    UINT32 itr;    
        
    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, ETRANS_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    for (itr = 0; itr < ETRANS_NUM_LINKS; itr++)
    {
        if (hndl->var.data_rate[itr] != LAST_ETRANS_DATA_RATE) 
        {
            result = FALSE;
        }
    }

    
    for(itr = 0; itr < ETRANS_NUM_GSUP43_C73; itr++){
        if(result == TRUE && gsup43_c73_start_state_test(hndl->gsup43_c73_handle[itr]) == FALSE)
            result = FALSE;
    }
    
    if(result == TRUE){
        for(itr = 0; itr < ETRANS_NUM_EXCO3M; itr++){   
            if(result == TRUE && exco3m_start_state_test(hndl->exco3m_handle[itr]) == FALSE)
                result = FALSE;
        }
    }
        
    if(result == TRUE && exco4m_start_state_test(hndl->exco4m_handle) == FALSE)
        result = FALSE;

    PMC_RETURN(result);
} /* etrans_start_state_test */

/*******************************************************************************
* etrans_activated_link_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns all started channels
*                                                                               
* INPUTS:                                                                       
*   *hndl             - pointer to ETRANS handle instanec
*                                                                             
* OUTPUTS:
*   *rx_actv_link     - status of etrans RX channels
*   *tx_actv_link     - status of etrans TX channels
*
* RETURNS:
*   None.                                                                        
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC void etrans_activated_link_get(etrans_handle_t *hndl,
                                      BOOL8           *rx_actv_link,
                                      BOOL8           *tx_actv_link)
{
    UINT32 itr;

    PMC_ENTRY();
    
    PMC_ASSERT(hndl != NULL, ETRANS_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    /* check RX and TX link enable status */
    for (itr = 0; itr < ETRANS_NUM_LINKS; itr++)
    {
        if (itr == 0)
        {
            rx_actv_link[itr] = etrans_field_RX_10G_LINK_EN_get( NULL, hndl, itr) | etrans_field_RX_40G_LINK_EN_get( NULL, hndl, itr) | etrans_field_RX_100G_LINK_EN_get( NULL, hndl);

            tx_actv_link[itr] = etrans_field_TX_10G_LINK_EN_get( NULL, hndl, itr) | etrans_field_TX_40G_LINK_EN_get( NULL, hndl, itr) | etrans_field_TX_100G_LINK_EN_get( NULL, hndl);
        }
        else if (0 == itr % 4)
        {
            rx_actv_link[itr] = etrans_field_RX_10G_LINK_EN_get( NULL, hndl, itr) | etrans_field_RX_40G_LINK_EN_get(NULL, hndl, itr / 4);

            tx_actv_link[itr] = etrans_field_TX_10G_LINK_EN_get( NULL, hndl, itr) | etrans_field_TX_40G_LINK_EN_get(NULL, hndl, itr / 4);
        }
        else
        {
            rx_actv_link[itr] = etrans_field_RX_10G_LINK_EN_get( NULL, hndl, itr);
            tx_actv_link[itr] = etrans_field_TX_10G_LINK_EN_get( NULL, hndl, itr);
        }
    }

    PMC_RETURN();
}  /* etrans_activated_link_get */



/*******************************************************************************
* etrans_rx_link_rate_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns all started channels
*                                                                               
* INPUTS:                                                                       
*   *hndl        - pointer to ETRANS handle instance
*                                                                             
* OUTPUTS:
*   *datapath    - ETRANS links datapath
*   *rate        - ETRANS links data rate
*
* RETURNS: 
*   None.                                                                        
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC void etrans_rx_link_rate_get(etrans_handle_t       *hndl,
                                    etrans_datapath_t     *datapath,
                                    etrans_data_rate_t    *rate)
{
    UINT32 itr;
    UINT32 incr;
    BOOL8 rate_10G;
    BOOL8 rate_40G;
    BOOL8 rate_100G;
    UINT32 dp_sel_reg_val;
    PMC_ENTRY();
    
    PMC_ASSERT(hndl != NULL, ETRANS_LOG_ERR_INVALID_PARAMETERS, 0, 0);


    
    for (itr = 0; itr < ETRANS_NUM_LINKS; itr++)
    {
        rate[itr] = LAST_ETRANS_DATA_RATE; 
        datapath[itr] = ETRANS_UNUSED;
    }
    
    for (itr = 0; itr < ETRANS_NUM_LINKS; itr += incr)
    {        
        incr = 1;
        rate_10G = FALSE;
        rate_40G = FALSE;
        rate_100G = FALSE;

        rate_10G = etrans_field_RX_10G_LINK_EN_get(NULL, hndl, itr);

        if (0 == itr)
        {
            rate_100G = etrans_field_RX_100G_LINK_EN_get(NULL, hndl);
            rate_40G = etrans_field_RX_40G_LINK_EN_get(NULL, hndl, itr);
            
            if (TRUE == rate_100G)
            {
                rate[itr] = ETRANS_LINK_100G;
                dp_sel_reg_val = etrans_field_RX_100G_LINK_DP_SEL_get(NULL, hndl);
                switch(dp_sel_reg_val)
                {
                case ENET_ETRANS_DP_SEL_GSUP43_C71:
                    datapath[itr] = ETRANS_GMP_G709_100GE_MAC;
                    break;
                case ENET_ETRANS_DP_SEL_GSUP43_C62:
                    datapath[itr] = ETRANS_GFP_GSUP43_C62_G709_100GE_MAC;
                    break;
                default:
                    break;
                }
            }
            else if (TRUE == rate_40G)
            {
                rate[itr] = ETRANS_LINK_40G;
                dp_sel_reg_val = etrans_field_RX_40G_LINK_DP_SEL_get(NULL, hndl, itr / 4);
                switch(dp_sel_reg_val)
                {
                case ENET_ETRANS_DP_SEL_GSUP43_C71:
                    datapath[itr] = ETRANS_GMP_G709_40GE_MAC;
                    break;
                case ENET_ETRANS_DP_SEL_GSUP43_C62:
                    datapath[itr] = ETRANS_GFP_GSUP43_C62_G709_40GE_MAC;
                    break;
                default:
                    break;
                }
            }
            else if (TRUE == rate_10G)
            {
                rate[itr] = ETRANS_LINK_10G;
                dp_sel_reg_val = etrans_field_RX_10G_LINK_DP_SEL_get(NULL, hndl, itr);
                switch(dp_sel_reg_val)
                {
                case ENET_ETRANS_DP_SEL_GSUP43_C71:
                    datapath[itr] = ETRANS_BMP_GSUP43_C71_10GE_MAC;
                    break;
                case ENET_ETRANS_DP_SEL_GSUP43_C62:
                    datapath[itr] = ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC;
                    break;
                case ENET_ETRANS_DP_SEL_GSUP43_C73:
                    datapath[itr] = ETRANS_GFP_GSUP43_C73_10GE_MAC;
                    break;
                default:
                    break;
                }
            }
        }
        else if (0 == (itr % 4))
        {
            rate_40G = etrans_field_RX_40G_LINK_EN_get(NULL, hndl, itr / 4);
            
            if (TRUE == rate_40G)
            {
                rate[itr] = ETRANS_LINK_40G;
                dp_sel_reg_val = etrans_field_RX_40G_LINK_DP_SEL_get(NULL, hndl, itr / 4);
                switch(dp_sel_reg_val)
                {
                case ENET_ETRANS_DP_SEL_GSUP43_C71:
                    datapath[itr] = ETRANS_GMP_G709_40GE_MAC;
                    break;
                case ENET_ETRANS_DP_SEL_GSUP43_C62:
                    datapath[itr] = ETRANS_GFP_GSUP43_C62_G709_40GE_MAC;
                    break;
                default:
                    break;
                }
            }
            else if (TRUE == rate_10G)
            {
                rate[itr] = ETRANS_LINK_10G;
                dp_sel_reg_val = etrans_field_RX_10G_LINK_DP_SEL_get(NULL, hndl, itr);
                switch(dp_sel_reg_val)
                {
                case ENET_ETRANS_DP_SEL_GSUP43_C71:
                    datapath[itr] = ETRANS_BMP_GSUP43_C71_10GE_MAC;
                    break;
                case ENET_ETRANS_DP_SEL_GSUP43_C62:
                    datapath[itr] = ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC;
                    break;
                case ENET_ETRANS_DP_SEL_GSUP43_C73:
                    datapath[itr] = ETRANS_GFP_GSUP43_C73_10GE_MAC;
                    break;
                default:
                    break;
                }
            }
        }
        else
        {
            if (TRUE == rate_10G)
            {
                rate[itr] = ETRANS_LINK_10G;
                dp_sel_reg_val = etrans_field_RX_10G_LINK_DP_SEL_get(NULL, hndl, itr);
                switch(dp_sel_reg_val)
                {
                case ENET_ETRANS_DP_SEL_GSUP43_C71:
                    datapath[itr] = ETRANS_BMP_GSUP43_C71_10GE_MAC;
                    break;
                case ENET_ETRANS_DP_SEL_GSUP43_C62:
                    datapath[itr] = ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC;
                    break;
                case ENET_ETRANS_DP_SEL_GSUP43_C73:
                    datapath[itr] = ETRANS_GFP_GSUP43_C73_10GE_MAC;
                    break;
                default:
                    break;
                }
            }
        }
                            
    }

    PMC_RETURN();
}  /* etrans_rx_link_rate_get */


/*******************************************************************************
* etrans_tx_link_rate_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns all started channels
*                                                                               
* INPUTS:                                                                       
*   *hndl        - pointer to ETRANS handle instance
*                                                                             
* OUTPUTS:
*   *datapath    - ETRANS links datapath
*   *rate        - ETRANS links data rate
*
* RETURNS: 
*   None.                                                                        
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC void etrans_tx_link_rate_get(etrans_handle_t     *hndl,
                                    etrans_datapath_t     *datapath,
                                    etrans_data_rate_t    *rate)
{
    UINT32 itr;
    UINT32 incr;
    BOOL8 rate_10G;
    BOOL8 rate_40G;
    BOOL8 rate_100G;
    UINT32 dp_sel_reg_val;
    PMC_ENTRY();
    
    PMC_ASSERT(hndl != NULL, ETRANS_LOG_ERR_INVALID_PARAMETERS, 0, 0);


    
    for (itr = 0; itr < ETRANS_NUM_LINKS; itr++)
    {
        rate[itr] = LAST_ETRANS_DATA_RATE; 
        datapath[itr] = ETRANS_UNUSED;
    }
    
    for (itr = 0; itr < ETRANS_NUM_LINKS; itr += incr)
    {        
        incr = 1;
        rate_10G = FALSE;
        rate_40G = FALSE;
        rate_100G = FALSE;

        rate_10G = etrans_field_TX_10G_LINK_EN_get(NULL, hndl, itr);

        if (0 == itr)
        {
            rate_100G = etrans_field_TX_100G_LINK_EN_get(NULL, hndl);
            rate_40G = etrans_field_TX_40G_LINK_EN_get(NULL, hndl, itr);
            
            if (TRUE == rate_100G)
            {
                rate[itr] = ETRANS_LINK_100G;
                dp_sel_reg_val = etrans_field_TX_100G_LINK_DP_SEL_get(NULL, hndl);
                switch(dp_sel_reg_val)
                {
                case ENET_ETRANS_DP_SEL_GSUP43_C71:
                    datapath[itr] = ETRANS_GMP_G709_100GE_MAC;
                    break;
                case ENET_ETRANS_DP_SEL_GSUP43_C62:
                    datapath[itr] = ETRANS_GFP_GSUP43_C62_G709_100GE_MAC;
                    break;
                default:
                    break;
                }
            }
            else if (TRUE == rate_40G)
            {
                rate[itr] = ETRANS_LINK_40G;
                dp_sel_reg_val = etrans_field_TX_40G_LINK_DP_SEL_get(NULL, hndl, itr / 4);
                switch(dp_sel_reg_val)
                {
                case ENET_ETRANS_DP_SEL_GSUP43_C71:
                    datapath[itr] = ETRANS_GMP_G709_40GE_MAC;
                    break;
                case ENET_ETRANS_DP_SEL_GSUP43_C62:
                    datapath[itr] = ETRANS_GFP_GSUP43_C62_G709_40GE_MAC;
                    break;
                default:
                    break;
                }
            }
            else if (TRUE == rate_10G)
            {
                rate[itr] = ETRANS_LINK_10G;
                dp_sel_reg_val = etrans_field_TX_10G_LINK_DP_SEL_get(NULL, hndl, itr);
                switch(dp_sel_reg_val)
                {
                case ENET_ETRANS_DP_SEL_GSUP43_C71:
                    datapath[itr] = ETRANS_BMP_GSUP43_C71_10GE_MAC;
                    break;
                case ENET_ETRANS_DP_SEL_GSUP43_C62:
                    datapath[itr] = ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC;
                    break;
                case ENET_ETRANS_DP_SEL_GSUP43_C73:
                    datapath[itr] = ETRANS_GFP_GSUP43_C73_10GE_MAC;
                    break;
                default:
                    break;
                }
            }
        }
        else if (0 == (itr % 4))
        {
            rate_40G = etrans_field_TX_40G_LINK_EN_get(NULL, hndl, itr / 4);
            
            if (TRUE == rate_40G)
            {
                rate[itr] = ETRANS_LINK_40G;
                dp_sel_reg_val = etrans_field_TX_40G_LINK_DP_SEL_get(NULL, hndl, itr / 4);
                switch(dp_sel_reg_val)
                {
                case ENET_ETRANS_DP_SEL_GSUP43_C71:
                    datapath[itr] = ETRANS_GMP_G709_40GE_MAC;
                    break;
                case ENET_ETRANS_DP_SEL_GSUP43_C62:
                    datapath[itr] = ETRANS_GFP_GSUP43_C62_G709_40GE_MAC;
                    break;
                default:
                    break;
                }
            }
            else if (TRUE == rate_10G)
            {
                rate[itr] = ETRANS_LINK_10G;
                dp_sel_reg_val = etrans_field_TX_10G_LINK_DP_SEL_get(NULL, hndl, itr);
                switch(dp_sel_reg_val)
                {
                case ENET_ETRANS_DP_SEL_GSUP43_C71:
                    datapath[itr] = ETRANS_BMP_GSUP43_C71_10GE_MAC;
                    break;
                case ENET_ETRANS_DP_SEL_GSUP43_C62:
                    datapath[itr] = ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC;
                    break;
                case ENET_ETRANS_DP_SEL_GSUP43_C73:
                    datapath[itr] = ETRANS_GFP_GSUP43_C73_10GE_MAC;
                    break;
                default:
                    break;
                }
            }
        }
        else
        {
            if (TRUE == rate_10G)
            {
                rate[itr] = ETRANS_LINK_10G;
                dp_sel_reg_val = etrans_field_TX_10G_LINK_DP_SEL_get(NULL, hndl, itr);
                switch(dp_sel_reg_val)
                {
                case ENET_ETRANS_DP_SEL_GSUP43_C71:
                    datapath[itr] = ETRANS_BMP_GSUP43_C71_10GE_MAC;
                    break;
                case ENET_ETRANS_DP_SEL_GSUP43_C62:
                    datapath[itr] = ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC;
                    break;
                case ENET_ETRANS_DP_SEL_GSUP43_C73:
                    datapath[itr] = ETRANS_GFP_GSUP43_C73_10GE_MAC;
                    break;
                default:
                    break;
                }
            }
        }
        
    }

    PMC_RETURN();
}  /* etrans_tx_link_rate_get */



/*******************************************************************************
* etrans_data_rate_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   ETRANS instance.  
*
*
* INPUTS:
*   *hndl                       - pointer to ETRANS handle instance
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
PRIVATE BOOL8 etrans_data_rate_start_state_test(etrans_handle_t *hndl,etrans_data_rate_t data_rate)
{
    UINT32 itr;
    BOOL8 result = TRUE;

    PMC_ENTRY();
    
    PMC_ASSERT(hndl!=NULL, ETRANS_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    

    for (itr = 0; itr < ETRANS_NUM_LINKS; itr++)
    {
        if (hndl->var.data_rate[itr] == data_rate) 
        {
            result = FALSE;
        }
    }   
        
    PMC_RETURN(result);
    
} /* etrans_data_rate_start_state_test */


/*******************************************************************************
* etrans_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of a ETRANS block instance.
*
*
* INPUTS:
*   *etrans_handle        - pointer to ETRANS handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Base address of ETRANS.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 etrans_base_address_get(etrans_handle_t *etrans_handle)
{
    PMC_ENTRY();

    PMC_RETURN(etrans_handle->base.base_address);
} /* etrans_base_address_get */

/*******************************************************************************
* Initialization Configuration Functions
*******************************************************************************/
/*******************************************************************************
*  etrans_rx_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure ingress links for specified rate and datapath
*
* INPUTS:
*   hndl     - handle to the ETRANS instance to be operated on
*   link     - link to configure
*   chan     - channel to configure (from MAPOTN: 0 to 95)
*   datapath - datapath to configure
*   enable   - TRUE: enable datapath. FALSE: disable datapath.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etrans_rx_cfg(etrans_handle_t *hndl, UINT32 link, UINT32 chan, etrans_datapath_t datapath, BOOL8 enable)
{
    UINT32 data_path_select = ENET_ETRANS_DP_SEL_GSUP43_C73;
    UINT32 pkt_type         = ENET_ETRANS_GSUP43_C73_RX_PKT_TYPE;
    PMC_ERROR ret_val       = PMC_SUCCESS;
    UINT32 exco3m_inst      = 0;
    DOUBLE rate             = 0;
    etrans_data_rate_t data_rate = LAST_ETRANS_DATA_RATE;

    etrans_buffer_t etrans_buffer[1];
    BOOL8 disable;
    
    PMC_ENTRY();
    PMC_ASSERT(datapath != ETRANS_UNUSED, ETRANS_ERR_INVALID_MODE , 0 , 0);

    disable = TRUE == enable ? FALSE : TRUE;

    /* Configure specified link for the datapath */
    /* Register 0x98 + 0x04*N(N=0:11): ETRANS120_TOP - RX  10G Link Cfg Reg */
    /* Register 0xC8 + 0x04*N(N=0: 2): ETRANS120_TOP - RX  40G Link Cfg Reg */
    /* Register 0xD4                 : ETRANS120_TOP - RX 100G Link Cfg Reg */
    /* Register 0x1000 + 0x0400*A(A=0:2): CBR_40G(A) - Configuration */
    /* Register 0x2000: CBR_100G - Configuration */

    if ( enable ) {
      /* Write link value to channel memory */
      etrans_ingress_link_to_chan_map_cfg( hndl, chan, link );
    }

    switch (datapath) 
    {
    case ETRANS_BMP_GSUP43_C71_10GE_MAC:
    case ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC:
    case ETRANS_GFP_GSUP43_C73_10GE_MAC:
        rate = UTIL_GLOBAL_ENET_10GE_RATE;
        data_rate = ETRANS_LINK_10G;
        break;
    case ETRANS_GMP_G709_40GE_MAC:
    case ETRANS_GFP_GSUP43_C62_G709_40GE_MAC:
    {
        rate = UTIL_GLOBAL_ENET_40GE_RATE;
        data_rate = ETRANS_LINK_40G;
        /* compute exco3m index */
        PMC_ASSERT(link % 4 == 0, ETRANS_ERR_INVALID_MODE , 0 , 0);
        exco3m_inst = link/4;
        break;
    };
    case ETRANS_GMP_G709_100GE_MAC:
    case ETRANS_GFP_GSUP43_C62_G709_100GE_MAC:
        rate = UTIL_GLOBAL_ENET_100GE_RATE; 
        data_rate = ETRANS_LINK_100G;
        break;
    default:
        break;
    };

    /* Determine datapath cfg */
    switch (datapath) 
    {
    case ETRANS_BMP_GSUP43_C71_10GE_MAC:
    case ETRANS_GMP_G709_40GE_MAC:
    case ETRANS_GMP_G709_100GE_MAC:
        data_path_select = ENET_ETRANS_DP_SEL_GSUP43_C71;
        break;
    case ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC :  
        pkt_type = 1;
    case ETRANS_GFP_GSUP43_C62_G709_40GE_MAC:
    case ETRANS_GFP_GSUP43_C62_G709_100GE_MAC:
        data_path_select = ENET_ETRANS_DP_SEL_GSUP43_C62;
        break;
    case ETRANS_GFP_GSUP43_C73_10GE_MAC : 
        data_path_select = ENET_ETRANS_DP_SEL_GSUP43_C73;
        break;
    default:
        break;
    };
    
    /* Configure Links (they are enabled in activate functions) */    
    /* We only do it when we use more then  the MPMA::SCBS3 */
    if (PMC_SUCCESS == ret_val ) 
    {
        etrans_buffer_init(etrans_buffer, hndl);

        if (UTIL_GLOBAL_ENET_10GE_RATE == rate) 
        {
            etrans_field_RX_10G_LINK_RST_set(etrans_buffer, hndl, link, disable);
            etrans_field_RX_10G_LINK_EN_set(etrans_buffer, hndl, link, 0);
            etrans_field_RX_10G_LINK_DP_SEL_set(etrans_buffer, hndl, link, data_path_select);       
        } else if (UTIL_GLOBAL_ENET_40GE_RATE == rate) 
        {
            etrans_field_RX_40G_LINK_RST_set(etrans_buffer, hndl, exco3m_inst, disable);
            etrans_field_RX_40G_LINK_EN_set(etrans_buffer, hndl, exco3m_inst, 0);
            etrans_field_RX_40G_LINK_DP_SEL_set(etrans_buffer, hndl, exco3m_inst, data_path_select);
        } else 
        {
            etrans_field_RX_100G_LINK_RST_set(etrans_buffer, hndl,  disable);
            etrans_field_RX_100G_LINK_EN_set(etrans_buffer, hndl,  0);
            etrans_field_RX_100G_LINK_DP_SEL_set(etrans_buffer, hndl,  data_path_select);
        };
 
        etrans_buffer_flush(etrans_buffer);
    };

    /* configurat GSUP component */
    if (ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC == datapath ||
        ETRANS_GFP_GSUP43_C73_10GE_MAC == datapath) 
    {
        if( enable ) {
            ret_val = gsup43_c73_rx_cfg(hndl->gsup43_c73_handle[link],
                                        ENET_ETRANS_GSUP43_C73_LOS_INV,
                                        ENET_ETRANS_GSUP43_C73_DSCRD_ZONE,
                                        ENET_ETRANS_GSUP43_C73_RX_SIG_OS_DSCRD_EN,
                                        ENET_ETRANS_GSUP43_C73_RX_SEQ_OS_DSCRD_EN,
                                        ENET_ETRANS_GSUP43_C73_RX_OS_ALL_DSCRD,
                                        ENET_ETRANS_GSUP43_C73_RX_FULL_OS_RPT_DIS,
                                        ENET_ETRANS_GSUP43_C73_RX_MIN_OS_RPT,
                                        pkt_type,
                                        ENET_ETRANS_GSUP43_C73_RX_OS_TYPE,
                                        ENET_ETRANS_GSUP43_C73_RX_125US_CNT);
        }       
    }
        

    if (TRUE == enable)
    {
        hndl->var.data_rate[link] = data_rate;
        hndl->var.datapath[link] = datapath;
    }
    else
    {
        hndl->var.data_rate[link] = LAST_ETRANS_DATA_RATE;
        hndl->var.datapath[link] = ETRANS_UNUSED;
    }

    PMC_RETURN(ret_val);

} /* etrans_rx_cfg */

/*******************************************************************************
*  etrans_tx_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure egress links for specified rate and datapath
*
* INPUTS:
*   hndl     - handle to the ETRANS instance to be operated on
*   link     - link to configure
*   chan     - channel to configure (to MAPOTN: 0 to 95)
*   datapath - datapath to configure
*   enable   - TRUE: enable datapath. FALSE: disable datapath.
*   use_page_swap - use page swapping for the egress scheduler
*   pmon_only   - Set to true if the enet line is only being used as a pmon.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etrans_tx_cfg(etrans_handle_t *hndl, 
                               UINT32 link, 
                               UINT32 chan, 
                               etrans_datapath_t datapath, 
                               BOOL8 enable, 
                               BOOL8 use_page_swap,
                               BOOL8 pmon_only)
{
    UINT32 data_path_select = ENET_ETRANS_DP_SEL_GSUP43_C73;
    UINT32 pkt_type         = ENET_ETRANS_GSUP43_C73_RX_PKT_TYPE;
    PMC_ERROR ret_val       = PMC_SUCCESS;
    UINT32 exco3m_inst      = 0;
    DOUBLE rate             = 0;
    etrans_buffer_t etrans_buffer[1];
    BOOL8 disable;

    PMC_ENTRY();
    PMC_ASSERT(datapath != ETRANS_UNUSED, ETRANS_ERR_INVALID_MODE , 0 , 0);

    disable = TRUE == enable ? FALSE : TRUE;

    /* Configure specified link for the datapath */
    /* Register 0x148 + 0x04*N(N=0:11): ETRANS120_TOP - TX  10G Link Cfg */
    /* Register 0x178 + 0x04*N(N=0: 2): ETRANS120_TOP - TX  40G Link Cfg */
    /* Register 0x184                 : ETRANS120_TOP - TX 100G Link Cfg */
    /* Register 0x1000 + 0x0400*A(A=0:2): CBR_40G(A) - Configuration */
    /* Register 0x2000: CBR_100G - Configuration */

    if( enable ) {
      /* Write link value to channel memory */
      etrans_egress_chan_to_link_map_cfg( hndl, link, chan );
    }
    if (!pmon_only)
    {
        etrans_egress_scheduler_cfg( hndl, link, datapath, enable, use_page_swap );
    }


    switch (datapath) 
    {
    case ETRANS_BMP_GSUP43_C71_10GE_MAC:
    case ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC:
    case ETRANS_GFP_GSUP43_C73_10GE_MAC:
        rate = UTIL_GLOBAL_ENET_10GE_RATE;
        break;
    case ETRANS_GMP_G709_40GE_MAC:
    case ETRANS_GFP_GSUP43_C62_G709_40GE_MAC:
    {
        rate = UTIL_GLOBAL_ENET_40GE_RATE;
        /* compute exco3m index */
        PMC_ASSERT(link % 4 == 0, ETRANS_ERR_INVALID_MODE , 0 , 0);
        exco3m_inst = link/4;
        break;
    };
    case ETRANS_GMP_G709_100GE_MAC:
    case ETRANS_GFP_GSUP43_C62_G709_100GE_MAC:
        rate = UTIL_GLOBAL_ENET_100GE_RATE; 
        break;
    default:
        break;
    };

    /* Determine datapath cfg */
    switch (datapath) 
    {
    case ETRANS_BMP_GSUP43_C71_10GE_MAC:
    case ETRANS_GMP_G709_40GE_MAC:
    case ETRANS_GMP_G709_100GE_MAC:
        data_path_select = ENET_ETRANS_DP_SEL_GSUP43_C71;
        break;
    case ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC :  
        pkt_type = 1;
    case ETRANS_GFP_GSUP43_C62_G709_40GE_MAC:
    case ETRANS_GFP_GSUP43_C62_G709_100GE_MAC:
        data_path_select = ENET_ETRANS_DP_SEL_GSUP43_C62;
        break;
    case ETRANS_GFP_GSUP43_C73_10GE_MAC : 
        data_path_select = ENET_ETRANS_DP_SEL_GSUP43_C73;
        break;
    default:
        break;
    };

    if (PMC_SUCCESS == ret_val ) 
    {
        etrans_buffer_init(etrans_buffer, hndl);

        if (UTIL_GLOBAL_ENET_10GE_RATE == rate) 
        {
            etrans_field_TX_10G_LINK_RST_set(etrans_buffer, hndl, link, disable);
            etrans_field_TX_10G_LINK_EN_set(etrans_buffer, hndl, link, 0);
            etrans_field_TX_10G_LINK_DP_SEL_set(etrans_buffer, hndl, link, data_path_select);       
        } else if (UTIL_GLOBAL_ENET_40GE_RATE == rate) 
        {
            etrans_field_TX_40G_LINK_RST_set(etrans_buffer, hndl, exco3m_inst, disable);
            etrans_field_TX_40G_LINK_EN_set(etrans_buffer, hndl, exco3m_inst, 0);
            etrans_field_TX_40G_LINK_DP_SEL_set(etrans_buffer, hndl, exco3m_inst, data_path_select);
        } else 
        {
            etrans_field_TX_100G_LINK_RST_set(etrans_buffer, hndl,  disable);
            etrans_field_TX_100G_LINK_EN_set(etrans_buffer, hndl,  0);
            etrans_field_TX_100G_LINK_DP_SEL_set(etrans_buffer, hndl,  data_path_select);
        };
 
        etrans_buffer_flush(etrans_buffer);
    };


    /* configurat GSUP/xco3|4m component */
    switch (datapath) 
    {
    case ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC:
    case ETRANS_GFP_GSUP43_C73_10GE_MAC:
        if( enable ) {
            ret_val = gsup43_c73_tx_cfg(hndl->gsup43_c73_handle[link],
                                        ENET_ETRANS_GSUP43_C73_TX_ERR_DROP,
                                        pkt_type,
                                        ENET_ETRANS_GSUP43_C73_TX_OS_TYPE,
                                        ENET_ETRANS_GSUP43_C73_FORCE_MIN_IPG,
                                        ENET_ETRANS_GSUP43_C73_UNIDIR_EN,
                                        ENET_ETRANS_GSUP43_C73_SEND_IDLE_EN,
                                        ENET_ETRANS_GSUP43_C73_SEND_LF_EN,
                                        ENET_ETRANS_GSUP43_C73_SEND_RF_EN,
                                        ENET_ETRANS_GSUP43_C73_FIFO_HMARK);
            
            /* Note: These test fields are hidden, so we use the defaults, and don't control them */
            /* SCRMBLR_EN       = 1'b1 */
            /* TX_BIT_ORDR_INV  = 1'b1 */
        }    
        break;
    case ETRANS_GFP_GSUP43_C62_G709_40GE_MAC:
        ret_val = exco3m_cfg( hndl->exco3m_handle[exco3m_inst], link, EXCO3M_GFP_GSUP43_C62_40GE, enable, TRUE );
        break;
    case ETRANS_GMP_G709_40GE_MAC:
        ret_val = exco3m_cfg( hndl->exco3m_handle[exco3m_inst], link, EXCO3M_GMP_709_40GE, enable, TRUE );
        break;
    case ETRANS_GFP_GSUP43_C62_G709_100GE_MAC:
        ret_val = exco4m_cfg( hndl->exco4m_handle, link, EXCO4M_GFP_GSUP43_C62_100GE, enable, TRUE );
        break;
    case ETRANS_GMP_G709_100GE_MAC:
        ret_val = exco4m_cfg( hndl->exco4m_handle, link, EXCO4M_GMP_709_100GE, enable, TRUE );
        break;
    default:
        break;
    }

     /* check if the user forgot to disable a previously enabled loopback */
    if (hndl->var.loopback[link] == TRUE)
    {
        PMC_ASSERT(FALSE, ETRANS_LOG_ERR_LOOPBACK, 0 , 0);
        etrans_loopback_cfg(hndl, link, FALSE);
    }
 
    PMC_RETURN(PMC_SUCCESS);

} /* etrans_tx_cfg */

/*******************************************************************************
*  etrans_rx_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reset and unconfigure  link.
*
* INPUTS:
*   hndl     - handle to the ETRANS instance to be operated on
*   num_links      - link to configure
*   links          - indices of links to cleanup
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC void etrans_cleanup(etrans_handle_t *hndl,
                           UINT32           num_links, 
                           UINT32          *links)
{
    UINT32 data_path_select = ENET_ETRANS_DP_SEL_GSUP43_C73;
    PMC_ERROR ret_val       = PMC_SUCCESS;
    UINT32 exco3m_inst      = 0;

    etrans_buffer_t etrans_buffer[1];
    BOOL8 disable;
    UINT32 link_itr;
    UINT32 link_idx;
    
    PMC_ENTRY();

    disable = TRUE;

    /* Configure specified link for the datapath */
    /* Register 0x98 + 0x04*N(N=0:11): ETRANS120_TOP - RX  10G Link Cfg Reg */
    /* Register 0xC8 + 0x04*N(N=0: 2): ETRANS120_TOP - RX  40G Link Cfg Reg */
    /* Register 0xD4                 : ETRANS120_TOP - RX 100G Link Cfg Reg */
    /* Register 0x1000 + 0x0400*A(A=0:2): CBR_40G(A) - Configuration */
    /* Register 0x2000: CBR_100G - Configuration */


    
    /* Configure Links (they are enabled in activate functions) */    
    /* We only do it when we use more then  the MPMA::SCBS3 */

    etrans_buffer_init(etrans_buffer, hndl);


    for (link_itr = 0; link_itr < num_links; link_itr++)
    {
        link_idx = links[link_itr];
        etrans_field_RX_10G_LINK_RST_set(etrans_buffer, hndl, link_idx, disable);
        etrans_field_RX_10G_LINK_EN_set(etrans_buffer, hndl, link_idx, 0);
        etrans_field_RX_10G_LINK_DP_SEL_set(etrans_buffer, hndl, link_idx, data_path_select);  
        etrans_field_TX_10G_LINK_RST_set(etrans_buffer, hndl, link_idx, disable);
        etrans_field_TX_10G_LINK_EN_set(etrans_buffer, hndl, link_idx, 0);
        etrans_field_TX_10G_LINK_DP_SEL_set(etrans_buffer, hndl, link_idx, data_path_select);      
        
        if (0 == (link_idx % 4))
        {
            exco3m_inst = link_idx / 4;
            etrans_field_RX_40G_LINK_RST_set(etrans_buffer, hndl, exco3m_inst, disable);
            etrans_field_RX_40G_LINK_EN_set(etrans_buffer, hndl, exco3m_inst, 0);
            etrans_field_RX_40G_LINK_DP_SEL_set(etrans_buffer, hndl, exco3m_inst, data_path_select);
            etrans_field_TX_40G_LINK_RST_set(etrans_buffer, hndl, exco3m_inst, disable);
            etrans_field_TX_40G_LINK_EN_set(etrans_buffer, hndl, exco3m_inst, 0);
            etrans_field_TX_40G_LINK_DP_SEL_set(etrans_buffer, hndl, exco3m_inst, data_path_select);

            ret_val = exco3m_cfg( hndl->exco3m_handle[exco3m_inst], link_idx, EXCO3M_GFP_GSUP43_C62_40GE, FALSE, TRUE);
            ret_val = exco3m_cfg( hndl->exco3m_handle[exco3m_inst], link_idx, EXCO3M_GMP_709_40GE, FALSE, TRUE );
            ret_val = exco3m_cfg( hndl->exco3m_handle[exco3m_inst], link_idx, EXCO3M_GFP_GSUP43_C62_40GE, FALSE, TRUE);
        }
        if (0 == link_idx)
        {
            etrans_field_RX_100G_LINK_RST_set(etrans_buffer, hndl,  disable);
            etrans_field_RX_100G_LINK_EN_set(etrans_buffer, hndl,  0);
            etrans_field_RX_100G_LINK_DP_SEL_set(etrans_buffer, hndl,  data_path_select);
            etrans_field_TX_100G_LINK_RST_set(etrans_buffer, hndl,  disable);
            etrans_field_TX_100G_LINK_EN_set(etrans_buffer, hndl,  0);
            etrans_field_TX_100G_LINK_DP_SEL_set(etrans_buffer, hndl,  data_path_select);
            ret_val = exco4m_cfg( hndl->exco4m_handle, link_idx, EXCO4M_GFP_GSUP43_C62_100GE, FALSE, TRUE);
            ret_val = exco4m_cfg( hndl->exco4m_handle, link_idx, EXCO4M_GMP_709_100GE, FALSE, TRUE);
        } 

        etrans_reg_GLOBAL_LPBK_CFG_field_set(NULL,hndl,link_idx, link_idx, 0);

        /*
         * Rather than defining a new function that does the same thing 
         * simply call gsup43_c73_init to reset the module.
         */
        gsup43_c73_init(hndl->gsup43_c73_handle[link_idx],
                        PMC_ENERGY_STATE_RESET);
        hndl->var.data_rate[link_idx] = LAST_ETRANS_DATA_RATE;
        hndl->var.datapath[link_idx] = ETRANS_UNUSED;
    }

    etrans_buffer_flush(etrans_buffer);
    

    PMC_RETURN();

} /* etrans_cleanup */


/*******************************************************************************
*  etrans_ingress_link_to_chan_map_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure Ingress Channel-to-Link Mapper to map traffic
*   from 96 OTN Demapper channels to 12 links
*
* INPUTS:
*   hndl - handle to the ETRANS instance to be operated on
*   chnl - GFP channel source (0-95)
*   link - link to map the channel into (0-11)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etrans_ingress_link_to_chan_map_cfg(etrans_handle_t *hndl, UINT32 chnl, UINT32 link)
{
    PMC_ENTRY();

    etrans_field_RX_CHID_set( NULL, hndl, link,  chnl );

    PMC_RETURN(PMC_SUCCESS);

} /* etrans_ingress_link_to_chan_map_cfg */

/*******************************************************************************
*  etrans_ingress_link_to_chan_map_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure Ingress Channel-to-Link Mapper to map traffic
*   from 96 OTN Demapper channels to 12 links
*
* INPUTS:
*   hndl - handle to the ETRANS instance to be operated on
*   link - link to map the channel into (0-11)
*
* OUTPUTS:
*
* RETURNS:
*   The chnl number associated with the link. GFP channel source (0-95)
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 etrans_ingress_link_to_chan_map_get(etrans_handle_t *hndl, UINT32 link)
{
    UINT32 return_chnl;

    PMC_ENTRY();

    return_chnl = etrans_field_RX_CHID_get(NULL, hndl, link);

    PMC_RETURN(return_chnl);

} /* etrans_ingress_link_to_chan_map_get */

/*******************************************************************************
*  etrans_egress_chan_to_link_map_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure Egress Link-to-Channel Mapper to map traffic from 12 links to
*   96 OTN Mapper channels
*
* INPUTS:
*   hndl - handle to the ETRANS instance to be operated on
*   link - link source (0-11)
*   chnl - GFP channel destination (0-95)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etrans_egress_chan_to_link_map_cfg(etrans_handle_t *hndl, UINT32 link, UINT32 chnl)
{
    PMC_ENTRY();

    /* Map the link into the specified channel */
    /* Register 0x264 - 270: ETRANS120_TOP - TX Channel ID LookUp Configuration Register0 - 3*/

    switch ( link )
    {
    case 0:
        etrans_field_TX_CHID_LINK0_set(NULL,hndl,chnl);
        break;
    case 1:
        etrans_field_TX_CHID_LINK1_set(NULL,hndl,chnl);
        break;
    case 2:
        etrans_field_TX_CHID_LINK2_set(NULL,hndl,chnl);
        break;
    case 3:
        etrans_field_TX_CHID_LINK3_set(NULL,hndl,chnl);
        break;
    case 4:
        etrans_field_TX_CHID_LINK4_set(NULL,hndl,chnl);
        break;
    case 5:
        etrans_field_TX_CHID_LINK5_set(NULL,hndl,chnl);
        break;
    case 6:
        etrans_field_TX_CHID_LINK6_set(NULL,hndl,chnl);
        break;
    case 7:
        etrans_field_TX_CHID_LINK7_set(NULL,hndl,chnl);
        break;
    case 8:
        etrans_field_TX_CHID_LINK8_set(NULL,hndl,chnl);
        break;
    case 9:
        etrans_field_TX_CHID_LINK9_set(NULL,hndl,chnl);
        break;
    case 10:
        etrans_field_TX_CHID_LINK10_set(NULL,hndl,chnl);
        break;
    case 11:
        etrans_field_TX_CHID_LINK11_set(NULL,hndl,chnl);
        break;
    default:
        break;
    }

    PMC_RETURN(PMC_SUCCESS);

} /* etrans_egress_chan_to_link_map_cfg */

/*******************************************************************************
*  etrans_egress_scheduler_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure egress output scheduler to route transcoder links to the mapper
*
* INPUTS:
*   hndl     - handle to the ETRANS instance to be operated on
*   link     - link source (0-11)
*   datapath - the etrans datapath. See etrans_datapath_t for more information.
*   enable   - TRUE: enable datapath. FALSE: disable datapath.
*   use_page_swap - TRUE : wait for input signal top of page and page switch before
*                          switching to new calendar
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etrans_egress_scheduler_cfg(etrans_handle_t *hndl, UINT32 link, etrans_datapath_t datapath, BOOL8 enable, BOOL8 use_page_swap)
{

    UINT32 slot_val;
    etrans_buffer_t etrans_buffer[1];

    PMC_ENTRY();
    
    slot_val = (enable == FALSE)?0XF:link; 

    /* Map the link into the specified calendar timeslot */
    /* Register 0x204: ETRANS120_TOP - TX Op Sch Cal Cfg0 */
    /* Register 0x208: ETRANS120_TOP - TX Op Sch Cal Cfg1 */
    /* -------------------------------- */
    /* Set up the Tx calendar -> MAPOTN */
    /* Read, modify, write              */
    /* -------------------------------- */
    /* Get current slot, with 8/4 fields split in CFG0/CFG1 registers */
    /* Regardless we need to modify the CFG1 register to update the 12 slot calendar */

    etrans_buffer_init(etrans_buffer, hndl);

    switch( datapath ) {
      case ETRANS_BMP_GSUP43_C71_10GE_MAC : 
      case ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC :
      case ETRANS_GFP_GSUP43_C73_10GE_MAC : 
      {      
          switch ( link )
          {
          case 0:
              etrans_field_TX_SLOT_0_set(etrans_buffer,hndl,slot_val);
              break;
          case 1:
              etrans_field_TX_SLOT_1_set(etrans_buffer,hndl,slot_val);
              break;
          case 2:
              etrans_field_TX_SLOT_2_set(etrans_buffer,hndl,slot_val);
              break;
          case 3:
              etrans_field_TX_SLOT_3_set(etrans_buffer,hndl,slot_val);
              break;
          case 4:
              etrans_field_TX_SLOT_4_set(etrans_buffer,hndl,slot_val);
              break;
          case 5:
              etrans_field_TX_SLOT_5_set(etrans_buffer,hndl,slot_val);
              break;
          case 6:
              etrans_field_TX_SLOT_6_set(etrans_buffer,hndl,slot_val);     
              break;              
          case 7:
              etrans_field_TX_SLOT_7_set(etrans_buffer,hndl,slot_val);
              break;
          case 8:
              etrans_field_TX_SLOT_8_set(etrans_buffer,hndl,slot_val);
              break;
          case 9:
              etrans_field_TX_SLOT_9_set(etrans_buffer,hndl,slot_val);
              break;
          case 10:
              etrans_field_TX_SLOT_10_set(etrans_buffer,hndl,slot_val);
              break;
          case 11:
              etrans_field_TX_SLOT_11_set(etrans_buffer,hndl,slot_val);
              break;
          default:
              break;
          };
          break;
      }
    case ETRANS_GFP_GSUP43_C62_G709_40GE_MAC :
    case ETRANS_GMP_G709_40GE_MAC : 
    {
        switch( link ) {
        case 0 : {
            etrans_field_TX_SLOT_0_set(etrans_buffer,hndl,slot_val);
            etrans_field_TX_SLOT_1_set(etrans_buffer,hndl,slot_val);
            etrans_field_TX_SLOT_2_set(etrans_buffer,hndl,slot_val);
            etrans_field_TX_SLOT_3_set(etrans_buffer,hndl,slot_val);
            break;
        };
        case 4 : {
            etrans_field_TX_SLOT_4_set(etrans_buffer,hndl,slot_val);
            etrans_field_TX_SLOT_5_set(etrans_buffer,hndl,slot_val);
            etrans_field_TX_SLOT_6_set(etrans_buffer,hndl,slot_val);
            etrans_field_TX_SLOT_7_set(etrans_buffer,hndl,slot_val);
            break;
        }
        case 8 : {
            etrans_field_TX_SLOT_8_set(etrans_buffer,hndl,slot_val);
            etrans_field_TX_SLOT_9_set(etrans_buffer,hndl,slot_val);
            etrans_field_TX_SLOT_10_set(etrans_buffer,hndl,slot_val);
            etrans_field_TX_SLOT_11_set(etrans_buffer,hndl,slot_val);
            break;
        }
        default :
            break;
        }
        break;
    }
    case ETRANS_GFP_GSUP43_C62_G709_100GE_MAC :
    case ETRANS_GMP_G709_100GE_MAC : 
      {
          etrans_field_TX_SLOT_0_set(etrans_buffer,hndl,slot_val);
          etrans_field_TX_SLOT_1_set(etrans_buffer,hndl,slot_val);
          etrans_field_TX_SLOT_2_set(etrans_buffer,hndl,slot_val);
          etrans_field_TX_SLOT_3_set(etrans_buffer,hndl,slot_val);
          etrans_field_TX_SLOT_4_set(etrans_buffer,hndl,slot_val);
          etrans_field_TX_SLOT_5_set(etrans_buffer,hndl,slot_val);
          etrans_field_TX_SLOT_6_set(etrans_buffer,hndl,slot_val);
          etrans_field_TX_SLOT_7_set(etrans_buffer,hndl,slot_val);
          etrans_field_TX_SLOT_8_set(etrans_buffer,hndl,slot_val);
          etrans_field_TX_SLOT_9_set(etrans_buffer,hndl,slot_val);
          break;
      }
    default:
        break;
    }
    /* update calendar */
    if( FALSE == use_page_swap ) 
    {
        etrans_field_TX_LINK_SCH_UPDATE_set(etrans_buffer,hndl,1);
    }
    etrans_buffer_flush(etrans_buffer);

    PMC_RETURN(PMC_SUCCESS);

} /* etrans_egress_scheduler_cfg */

/*******************************************************************************
* Run-time Operations
*******************************************************************************/

/*******************************************************************************
*  etrans_loopback_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure loopbacks.
*
* INPUTS:
*   hndl    - handle to the ETRANS instance to be operated on
*   link    - link to configure
*   enable  - TRUE: enable loopback. FALSE: disable loopback.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etrans_loopback_cfg(etrans_handle_t *hndl,
                                     UINT32 link,
                                     BOOL8 enable)
{
    PMC_ENTRY();

    UINT32 no_bits; 
    UINT32 i; 
    /* Configure loopback on the specified link */
    /* Register 0x8C: ETRANS120_TOP - Global Loopback Cfg */

    /* If the link operating at 10G rate is to be put into loopback mode, then only 1 bit corresponding to this link is
     * asserted.
     * If the link operating at 40G rate is to be put into loopback mode, then only 4 bits corresponding to this 40G
     * link are asserted.
     * If the link operating at 100G rate is to be put into loopback mode, then only 10 bits corresponding to this
     * 100G link are asserted.
     */

    if (enable && hndl->var.loopback[link] != FALSE)
    {
        PMC_RETURN(ETRANS_LOG_ERR_LOOPBACK);
    }
    
    
    switch(hndl->var.data_rate[link])
    {
    case ETRANS_LINK_10G:
        no_bits = 1;
        break;
    case ETRANS_LINK_40G:
        no_bits = 4;
        break;
    case ETRANS_LINK_100G:
        no_bits = 10;
        break; 
    default:
        PMC_ASSERT(FALSE, ETRANS_ERR_INVALID_MODE , 0 , 0);
    }
    for(i=0; i < no_bits; i++) 
    {
        etrans_field_range_GBL_ETRANS_LB_EN_set(NULL,hndl,link + i, link + i,(enable ? 1 : 0));
    }
    
    hndl->var.loopback[link] = enable;
    PMC_RETURN(PMC_SUCCESS);
    
} /* etrans_loopback_cfg */

/*******************************************************************************
*  etrans_loopback_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the status of the ETRANS loopback.
*
* INPUTS:
*   hndl            - handle to the ETRANS instance to be operated on
*   link            - link to configure
*
* OUTPUTS:
*   *enabled_ptr    - TRUE: loopback enabled. FALSE: loopback not active.
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etrans_loopback_status_get(etrans_handle_t *hndl,
                                            UINT32 link,
                                            BOOL8 *enabled_ptr)
{
    PMC_ENTRY();

    *enabled_ptr = hndl->var.loopback[link];
    PMC_RETURN(PMC_SUCCESS);
} /* etrans_loopback_status_get */

/*******************************************************************************
*  etrans_rx_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable Rx INGRESS path
*
* INPUTS:
*   hndl     - handle to the ETRANS instance to be operated on
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
PUBLIC PMC_ERROR etrans_rx_activate(etrans_handle_t *hndl, UINT32 link, etrans_datapath_t datapath)
{
    UINT32 exco3m_inst      = 0;

    PMC_ENTRY();

    /* Note: Link configuration needs to happen before this, or cannot write anything */
    /* Take one GSup block out of reset */

    /* Bring up 10G datapath */
    if (( datapath == ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC ) ||
        ( datapath == ETRANS_GFP_GSUP43_C73_10GE_MAC            ))
      {
        gsup43_c73_rx_en      ( hndl->gsup43_c73_handle[link], TRUE );
      }

    switch( datapath )
      {
      case ETRANS_BMP_GSUP43_C71_10GE_MAC:
      case ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC :
      case ETRANS_GFP_GSUP43_C73_10GE_MAC :
        {
          etrans_field_RX_10G_LINK_EN_set( NULL, hndl, link, 1);
          break;
        }
      case ETRANS_GMP_G709_40GE_MAC:
      case ETRANS_GFP_GSUP43_C62_G709_40GE_MAC:
        {
          switch( link ) {
          case 0 : { 
              exco3m_inst = 0;
              break;
            }
          case 4 : {
              exco3m_inst = 1;
              break;
            }
          case 8 : {
              exco3m_inst = 2;
              break;
            }
          }
          etrans_field_RX_40G_LINK_EN_set( NULL, hndl, exco3m_inst, 1);
          break;
       }
      case ETRANS_GMP_G709_100GE_MAC :
      case ETRANS_GFP_GSUP43_C62_G709_100GE_MAC :
        {
          etrans_field_RX_100G_LINK_EN_set( NULL, hndl, 1);
          break;
        }
      default :
        {
          /* do nothing for ETRANS_UNUSED case */
        }
      }

    PMC_RETURN(PMC_SUCCESS);

} /* etrans_rx_activate */

/*******************************************************************************
*  etrans_rx_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable Rx path.
*
* INPUTS:
*   hndl     - handle to the ETRANS instance to be operated on
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
PUBLIC PMC_ERROR etrans_rx_deactivate(etrans_handle_t *hndl, UINT32 link, etrans_datapath_t datapath)
{
    UINT32 exco3m_inst      = 0;
    PMC_ENTRY();

    /* Bring up 10G datapath */
    if (( datapath == ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC ) ||
        ( datapath == ETRANS_GFP_GSUP43_C73_10GE_MAC            ))
      {
        gsup43_c73_rx_en      ( hndl->gsup43_c73_handle[link], FALSE );
      }

    switch( datapath )
      {
      case ETRANS_BMP_GSUP43_C71_10GE_MAC:
      case ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC :
      case ETRANS_GFP_GSUP43_C73_10GE_MAC :
        {
          etrans_field_RX_10G_LINK_EN_set( NULL, hndl, link, 0);
          break;
        }
      case ETRANS_GMP_G709_40GE_MAC:
      case ETRANS_GFP_GSUP43_C62_G709_40GE_MAC:
        {
          switch( link ) {
          case 0 : { 
              exco3m_inst = 0;
              break;
            }
          case 4 : {
              exco3m_inst = 1;
              break;
            }
          case 8 : {
              exco3m_inst = 2;
              break;
            }
          }
          etrans_field_RX_40G_LINK_EN_set( NULL, hndl, exco3m_inst, 0);
          break;
       }
      case ETRANS_GMP_G709_100GE_MAC :
      case ETRANS_GFP_GSUP43_C62_G709_100GE_MAC :
        {
          etrans_field_RX_100G_LINK_EN_set( NULL, hndl, 0);
          break;
        }
      default :
        {
          /* do nothing for ETRANS_UNUSED case */
        }
      }


    PMC_RETURN(PMC_SUCCESS);

} /* etrans_rx_deactivate */

/*******************************************************************************
*  etrans_tx_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable Tx EGRESS path
*
* INPUTS:
*   hndl     - handle to the ETRANS instance to be operated on
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
PUBLIC PMC_ERROR etrans_tx_activate(etrans_handle_t *hndl, UINT32 link, etrans_datapath_t datapath)
{
    UINT32 exco3m_inst      = 0;
    PMC_ENTRY();

    /* Bring up 10G datapath */
    if (( datapath == ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC ) ||
        ( datapath == ETRANS_GFP_GSUP43_C73_10GE_MAC            ))
    {
        gsup43_c73_tx_prbs_en ( hndl->gsup43_c73_handle[link], FALSE );
        gsup43_c73_tx_en      ( hndl->gsup43_c73_handle[link], TRUE  );
    }

    switch( datapath )
      {
      case ETRANS_BMP_GSUP43_C71_10GE_MAC:
      case ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC :
      case ETRANS_GFP_GSUP43_C73_10GE_MAC :
        {
          etrans_field_TX_10G_LINK_EN_set( NULL, hndl, link, 1);
          break;
        }
      case ETRANS_GMP_G709_40GE_MAC:
      case ETRANS_GFP_GSUP43_C62_G709_40GE_MAC:
        {
          switch( link ) {
          case 0 : { 
              exco3m_inst = 0;
              break;
            }
          case 4 : {
              exco3m_inst = 1;
              break;
            }
          case 8 : {
              exco3m_inst = 2;
              break;
            }
          }
          etrans_field_TX_40G_LINK_EN_set( NULL, hndl, exco3m_inst, 1);
          break;
       }
      case ETRANS_GMP_G709_100GE_MAC :
      case ETRANS_GFP_GSUP43_C62_G709_100GE_MAC :
        {
          etrans_field_TX_100G_LINK_EN_set( NULL, hndl, 1);
          break;
        }
      default :
        {
          /* do nothing for ETRANS_UNUSED case */
        }
      }

    PMC_RETURN(PMC_SUCCESS);

} /* etrans_tx_activate */

/*******************************************************************************
*  etrans_tx_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable Tx path.
*
* INPUTS:
*   hndl     - handle to the ETRANS instance to be operated on
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
PUBLIC PMC_ERROR etrans_tx_deactivate(etrans_handle_t *hndl, UINT32 link, etrans_datapath_t datapath)
{
    UINT32 exco3m_inst      = 0;
    PMC_ENTRY();

    /* Bring up 10G datapath */
    if (( datapath == ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC ) ||
        ( datapath == ETRANS_GFP_GSUP43_C73_10GE_MAC            ))
      {
        gsup43_c73_tx_en      ( hndl->gsup43_c73_handle[link], FALSE );
      }

    switch( datapath )
      {
      case ETRANS_BMP_GSUP43_C71_10GE_MAC:
      case ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC :
      case ETRANS_GFP_GSUP43_C73_10GE_MAC :
        {
          etrans_field_TX_10G_LINK_EN_set( NULL, hndl, link, 0);
          break;
        }
      case ETRANS_GMP_G709_40GE_MAC:
      case ETRANS_GFP_GSUP43_C62_G709_40GE_MAC:
        {
          switch( link ) {
          case 0 : { 
              exco3m_inst = 0;
              break;
            }
          case 4 : {
              exco3m_inst = 1;
              break;
            }
          case 8 : {
              exco3m_inst = 2;
              break;
            }
          }
          etrans_field_TX_40G_LINK_EN_set( NULL, hndl, exco3m_inst, 0);
          break;
       }
      case ETRANS_GMP_G709_100GE_MAC :
      case ETRANS_GFP_GSUP43_C62_G709_100GE_MAC :
        {
          etrans_field_TX_100G_LINK_EN_set( NULL, hndl, 0);
          break;
        }
      default :
        {
          /* do nothing for ETRANS_UNUSED case */
        }
      }

    PMC_RETURN(PMC_SUCCESS);

} /* etrans_tx_deactivate */

/*******************************************************************************
*  etrans_sw_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Global soft reset of 10, 40, and/or 100G datapaths
*
* INPUTS:
*   hndl         - Handle to the ETRANS instance
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
PUBLIC PMC_ERROR etrans_sw_reset(etrans_handle_t *hndl, UINT32 link, etrans_datapath_t datapath)
{
    PMC_ENTRY();

    /* UINT32 reg_value = 0; */

    switch( datapath )
        {
        case ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC :
        case ETRANS_GFP_GSUP43_C73_10GE_MAC            :
        case ETRANS_BMP_GSUP43_C71_10GE_MAC            :
            {   /*                                b:    h:    N:    value: */
                etrans_field_RX_10G_LINK_EN_set ( NULL, hndl, link, 0 );
                etrans_field_RX_10G_LINK_RST_set( NULL, hndl, link, 1 );
                etrans_field_TX_10G_LINK_EN_set ( NULL, hndl, link, 0 );
                etrans_field_TX_10G_LINK_RST_set( NULL, hndl, link, 1 );
                break;
            }
        case ETRANS_GFP_GSUP43_C62_G709_40GE_MAC :
        case ETRANS_GMP_G709_40GE_MAC            :
            {
                /*                                b:    h:    N:      value: */
                etrans_field_RX_40G_LINK_EN_set ( NULL, hndl, link/4, 0 );
                etrans_field_RX_40G_LINK_RST_set( NULL, hndl, link/4, 1 );
                etrans_field_TX_40G_LINK_EN_set ( NULL, hndl, link/4, 0 );
                etrans_field_TX_40G_LINK_RST_set( NULL, hndl, link/4, 1 );
                break;
            }
        case ETRANS_GFP_GSUP43_C62_G709_100GE_MAC :
        case ETRANS_GMP_G709_100GE_MAC            :
            {   /*                                 b:    h:    value: */
                etrans_field_RX_100G_LINK_EN_set ( NULL, hndl, 0 );
                etrans_field_RX_100G_LINK_RST_set( NULL, hndl, 1 );
                etrans_field_TX_100G_LINK_EN_set ( NULL, hndl, 0 );
                etrans_field_TX_100G_LINK_RST_set( NULL, hndl, 1 );
                break;
            }
        default :
            {
                /* Do nothing for ETRANS_UNUSED case */
                break;
            }
        }

    /* Reset 10G, 40G, or 100G datapath */
    /* reg_value = etrans_reg_GLOBAL_SOFT_RESET_CONFIG_REG_read(NULL, hndl); */

    /* if      (bw == 10)  { reg_value |= ETRANS120_REG_GLOBAL_SOFT_RESET_CONFIG_REG_BIT_SW_RESET_10G_MSK; } */
    /* else if (bw == 40)  { reg_value |= ETRANS120_REG_GLOBAL_SOFT_RESET_CONFIG_REG_BIT_SW_RESET_40G_MSK; } */
    /* else if (bw == 100) { reg_value |= ETRANS120_REG_GLOBAL_SOFT_RESET_CONFIG_REG_BIT_SW_RESET_100G_MSK; } */

    /* etrans_reg_GLOBAL_SOFT_RESET_CONFIG_REG_write(NULL, hndl, reg_value); */

    PMC_RETURN(PMC_SUCCESS);

} /* etrans_sw_reset */

/*******************************************************************************
*  etrans_lowpwr
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Global soft reset of 10, 40, and/or 100G datapaths
*
* INPUTS:
*   hndl         - Handle to the ETRANS instance
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
PUBLIC PMC_ERROR etrans_lowpwr(etrans_handle_t *hndl, UINT32 link, etrans_datapath_t datapath, BOOL8 value)
{
    PMC_ENTRY();

    switch( datapath )
        {
        case ETRANS_GFP_GSUP43_C62_G709_40GE_MAC       :
        case ETRANS_GMP_G709_40GE_MAC                  : { exco3m_lowpwr(hndl->exco3m_handle[link/4], value); break; }
        case ETRANS_GFP_GSUP43_C62_G709_100GE_MAC      :
        case ETRANS_GMP_G709_100GE_MAC                 : { exco4m_lowpwr(hndl->exco4m_handle,         value); break; }
        case ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC :
        case ETRANS_GFP_GSUP43_C73_10GE_MAC            :
        case ETRANS_BMP_GSUP43_C71_10GE_MAC            :
        default                                        : { break; }
        }

    PMC_RETURN(PMC_SUCCESS);

} /* etrans_lowpwr */

/*******************************************************************************
*  etrans_block_non_default_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures Non-default register settings associated with the ETRANS block in 
*   ENET subsystem.
*
*
* INPUTS:
*   *hndl         - pointer to ESTRNS handle instance
*
*
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
PUBLIC void etrans_block_non_default_cfg(etrans_handle_t *hndl)
{
    UINT32 ii;

    PMC_ENTRY();

    /* Unprovision all link channel ID register locations */
    for (ii = 0; ii < PMC_ETRANS120_REG_LINKID_MAPPING_INDEX_N_SIZE; ii++) {
        etrans_field_RX_CHID_set( NULL, hndl, ii, 0 );
    }
    PMC_RETURN();
} /* etrans_block_non_default_cfg */

/*******************************************************************************
*  etrans_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the ETRANS block energy state. 
*
* INPUTS:
*   hndl           - handle to the ETRANS instance to be operated on
*   data_rate      - the data rate
*   energy_state   - energy state
*
* OUTPUTS:
*
* RETURNS:
*   NONE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void etrans_energy_state_reg_set ( etrans_handle_t *hndl, etrans_data_rate_t data_rate , pmc_energy_state_t energy_state)
{
    BOOL8 val = (energy_state== PMC_ENERGY_STATE_RUNNING)?FALSE:TRUE;
    PMC_ENTRY();    

    PMC_ASSERT(hndl!=NULL, ETRANS_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    switch(data_rate)
    {
        case ETRANS_LINK_10G:
            etrans_field_SW_10G_RST_set(NULL, hndl, val);
            break;
        case ETRANS_LINK_40G:
            etrans_field_SW_40G_RST_set(NULL, hndl, val);
            break;
        case ETRANS_LINK_100G:
            etrans_field_SW_100G_RST_set(NULL, hndl, val);
            break;
        default:
            PMC_ASSERT(FALSE, ETRANS_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }

    PMC_RETURN();
} /* etrans_energy_state_reg_set */

/*******************************************************************************
*  etrans_lock_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Return ETRANS TX lock or alignment status. 
*
* INPUTS:
*   hndl     - handle to the ETRANS instance to be operated on
*   link     - link to configure
*   datapath - enum, requested datapath
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Return ETRANS TX lock or alignment status.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 etrans_lock_status_get(etrans_handle_t    *hndl, 
                                    UINT32              link, 
                                    etrans_datapath_t   datapath)
{
    BOOL8 rc = TRUE;
    PMC_POLL_RETURN_TYPE poll_rc = -1;
    UINT32 exco3m_inst;

    PMC_ENTRY();    

    switch( datapath )
    {
    case ETRANS_BMP_GSUP43_C71_10GE_MAC:
    case ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC :
    case ETRANS_GFP_GSUP43_C73_10GE_MAC :
    {
        poll_rc = gsup43_c73_los_sync_poll(hndl->gsup43_c73_handle[link]);        
        break;
    }
    case ETRANS_GMP_G709_40GE_MAC:
    case ETRANS_GFP_GSUP43_C62_G709_40GE_MAC:
    {
        switch( link ) {
        case 0 : { 
            exco3m_inst = 0;
            break;
        }
        case 4 : {
            exco3m_inst = 1;
            break;
        }
        case 8 : {
            exco3m_inst = 2;
            break;
            }
        default:  
            rc = FALSE;
            break;
        }
        if (TRUE == rc)
        {
            poll_rc = exco3m_tx_align_poll(hndl->exco3m_handle[exco3m_inst]);
        }
        break;
    }
    case ETRANS_GMP_G709_100GE_MAC :
    case ETRANS_GFP_GSUP43_C62_G709_100GE_MAC :
    {            
        poll_rc = exco4m_tx_align_poll(hndl->exco4m_handle);
        break;
    }
    default :
        /* do nothing for ETRANS_UNUSED case */
        break;
    }

    /* post-process poll returned value */
    if (0 == poll_rc)
    {
        rc = TRUE;
    }
    else
    {
        rc = FALSE;
    }

    PMC_RETURN(rc);
} /* etrans_lock_status_get */

/*******************************************************************************
* FUNCTION: etrans_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   etrans_handle     - handle to the ETRANS instance to be operated on
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
PUBLIC PMC_ERROR etrans_int_validate(etrans_handle_t    *etrans_handle,
                                     UINT32              link,
                                     void               *non_null_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != etrans_handle, ETRANS_LOG_ERR_INVALID_PARAMETERS, 0, 0);
   
    if (link >= ETRANS_NUM_LINKS)
    {
        result = ETRANS_LOG_ERR_INVALID_PARAMETERS;
    }

    if (NULL == non_null_ptr)
    {
        result = ETRANS_LOG_ERR_INVALID_PARAMETERS;
    }


    PMC_RETURN(result);
} /* etrans_int_validate */


/*******************************************************************************
* FUNCTION: etrans_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   etrans_handle     - handle to the ETRANS instance to be operated on
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
PUBLIC PMC_ERROR etrans_int_chnl_enable(etrans_handle_t    *etrans_handle,
                                        UINT32              link,
                                        etrans_int_chnl_t  *int_table_ptr,
                                        etrans_int_chnl_t  *int_en_table_ptr,
                                        BOOL8                enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1;
    UINT32          link2;
    BOOL8           has_exco4m;
    BOOL8           has_exco3m;
    UINT32          exco3m_idx;
    BOOL8           has_gsup43;
    UINT32          gsup43_idx;
    BOOL8           has_etrans_10g;
    BOOL8           has_etrans_40g;
    BOOL8           has_etrans_100g;
    UINT32          etrans_idx;

    PMC_ENTRY();

    result = etrans_int_validate(etrans_handle, link, int_en_table_ptr);

    if (PMC_SUCCESS == result)
    {
        /* Get the right index for EXCO3M, GSUP43, ETRANS for link */
        result = etrans_int_params_get(etrans_handle,
                                     etrans_handle->var.datapath[link],
                                     link,
                                     &has_exco4m,
                                     &has_exco3m,
                                     &exco3m_idx,
                                     &has_gsup43,
                                     &gsup43_idx,
                                     &has_etrans_10g,
                                     &has_etrans_40g,
                                     &has_etrans_100g,
                                     &etrans_idx);
    }

    if (PMC_SUCCESS == result && TRUE == has_gsup43)
    {
        result = gsup43_c73_int_chnl_enable(etrans_handle->gsup43_c73_handle[gsup43_idx],
                                            0,
                                            &int_table_ptr->gsup43_c73,
                                            &int_en_table_ptr->gsup43_c73,
                                            enable);
    }

    if (PMC_SUCCESS == result && TRUE == has_exco3m)
    {
        result = exco3m_int_chnl_enable(etrans_handle->exco3m_handle[exco3m_idx],
                                        exco3m_idx,
                                        &int_table_ptr->exco3m,
                                        &int_en_table_ptr->exco3m,
                                        enable);
    }
    
    if (PMC_SUCCESS == result && TRUE == has_exco4m)
    {
        result = exco4m_int_chnl_enable(etrans_handle->exco4m_handle,
                                        link,
                                        &int_table_ptr->exco4m,
                                        &int_en_table_ptr->exco4m,
                                        enable);
    }

    if (PMC_SUCCESS == result)
    {

#ifdef ETRANS_INT_SINGLE
#undef ETRANS_INT_SINGLE
#endif
#define ETRANS_INT_SINGLE    UTIL_GLOBAL_INT_ENABLE_SINGLE
    
#ifdef ETRANS_INT_RANGE
#undef ETRANS_INT_RANGE
#endif
#define ETRANS_INT_RANGE    UTIL_GLOBAL_INT_ENABLE_RANGE
    
#ifdef ETRANS_INT_FIELD
#undef ETRANS_INT_FIELD
#endif
#define ETRANS_INT_FIELD    UTIL_GLOBAL_INT_ENABLE_FIELD12
   

        link1 = etrans_idx;
        link2 = etrans_idx;
        if (PMC_SUCCESS == result && TRUE == has_etrans_10g)
        {
            ETRANS_10G_INT_TABLE_DEFINE();
        }
        else if (PMC_SUCCESS == result && TRUE == has_etrans_40g)
        {
            ETRANS_40G_INT_TABLE_DEFINE();
        }
        else if (PMC_SUCCESS == result && TRUE == has_etrans_100g)
        {
            ETRANS_100G_INT_TABLE_DEFINE();
        }

        int_en_table_ptr->gsup43_c73_valid = has_gsup43;
        int_en_table_ptr->exco3m_valid = has_exco3m;
        int_en_table_ptr->exco4m_valid = has_exco4m;
        int_en_table_ptr->etrans_10g_valid = has_etrans_10g;
        int_en_table_ptr->etrans_40g_valid = has_etrans_40g;
        int_en_table_ptr->etrans_100g_valid = has_etrans_100g;
    }    

    PMC_RETURN(result);
} /* etrans_int_chnl_enable */




/*******************************************************************************
* FUNCTION: etrans_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   etrans_handle     - handle to the ETRANS instance to be operated on
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
PUBLIC PMC_ERROR etrans_int_chnl_clear(etrans_handle_t    *etrans_handle,
                                       UINT32              link,
                                       etrans_int_chnl_t  *int_table_ptr,
                                       etrans_int_chnl_t  *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1;
    UINT32          link2;
    BOOL8           has_exco4m;
    BOOL8           has_exco3m;
    UINT32          exco3m_idx;
    BOOL8           has_gsup43;
    UINT32          gsup43_idx;
    BOOL8           has_etrans_10g;
    BOOL8           has_etrans_40g;
    BOOL8           has_etrans_100g;
    UINT32          etrans_idx;

    PMC_ENTRY();

    result = etrans_int_validate(etrans_handle, link, int_en_table_ptr);

    if (PMC_SUCCESS == result)
    {
        /* Get the right index for EXCO3M, GSUP43, ETRANS for link */
        result = etrans_int_params_get(etrans_handle,
                                     etrans_handle->var.datapath[link],
                                     link,
                                     &has_exco4m,
                                     &has_exco3m,
                                     &exco3m_idx,
                                     &has_gsup43,
                                     &gsup43_idx,
                                     &has_etrans_10g,
                                     &has_etrans_40g,
                                     &has_etrans_100g,
                                     &etrans_idx);
    }

    if (PMC_SUCCESS == result && TRUE == has_gsup43)
    {
        result = gsup43_c73_int_chnl_clear(etrans_handle->gsup43_c73_handle[gsup43_idx],
                                           0,
                                           UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, gsup43_c73),
                                           &int_en_table_ptr->gsup43_c73);
    }

    if (PMC_SUCCESS == result && TRUE == has_exco3m)
    {
        result = exco3m_int_chnl_clear(etrans_handle->exco3m_handle[exco3m_idx],
                                       link,
                                       UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, exco3m),
                                       &int_en_table_ptr->exco3m);
    }
    
    if (PMC_SUCCESS == result && TRUE == has_exco4m)
    {
        result = exco4m_int_chnl_clear(etrans_handle->exco4m_handle,
                                       link,
                                       UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, exco4m),
                                       &int_en_table_ptr->exco4m);
    }

    if (PMC_SUCCESS == result)
    {
#ifdef ETRANS_INT_SINGLE
#undef ETRANS_INT_SINGLE
#endif
#define ETRANS_INT_SINGLE UTIL_GLOBAL_INT_CLEAR_SINGLE
    
#ifdef ETRANS_INT_RANGE
#undef ETRANS_INT_RANGE
#endif
#define ETRANS_INT_RANGE    UTIL_GLOBAL_INT_CLEAR_RANGE
    
#ifdef ETRANS_INT_FIELD
#undef ETRANS_INT_FIELD
#endif
#define ETRANS_INT_FIELD    UTIL_GLOBAL_INT_CLEAR_FIELD12
   

        link1 = etrans_idx;
        link2 = etrans_idx;
        if (PMC_SUCCESS == result && TRUE == has_etrans_10g)
        {
            ETRANS_10G_INT_TABLE_DEFINE();
        }
        else if (PMC_SUCCESS == result && TRUE == has_etrans_40g)
        {
            ETRANS_40G_INT_TABLE_DEFINE();
        }
        else if (PMC_SUCCESS == result && TRUE == has_etrans_100g)
        {
            ETRANS_100G_INT_TABLE_DEFINE();
        }
    }

    PMC_RETURN(result);
} /* etrans_int_chnl_clear */




/*******************************************************************************
* FUNCTION: etrans_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   etrans_handle     - handle to the ETRANS instance to be operated on
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
PUBLIC PMC_ERROR etrans_int_chnl_retrieve(etrans_handle_t    *etrans_handle,
                                          UINT32              link,
                                          etrans_int_chnl_t  *filt_table_ptr,
                                          etrans_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1;
    UINT32          link2;
    BOOL8           has_exco4m;
    BOOL8           has_exco3m;
    UINT32          exco3m_idx;
    BOOL8           has_gsup43;
    UINT32          gsup43_idx;
    BOOL8           has_etrans_10g;
    BOOL8           has_etrans_40g;
    BOOL8           has_etrans_100g;
    UINT32          etrans_idx;

    PMC_ENTRY();

    result = etrans_int_validate(etrans_handle, link, int_table_ptr);

    if (PMC_SUCCESS == result)
    {
        /* Get the right index for EXCO3M, GSUP43, ETRANS for link */
        result = etrans_int_params_get(etrans_handle,
                                     etrans_handle->var.datapath[link],
                                     link,
                                     &has_exco4m,
                                     &has_exco3m,
                                     &exco3m_idx,
                                     &has_gsup43,
                                     &gsup43_idx,
                                     &has_etrans_10g,
                                     &has_etrans_40g,
                                     &has_etrans_100g,
                                     &etrans_idx);
    }

    if (PMC_SUCCESS == result && TRUE == has_gsup43)
    {
        result = gsup43_c73_int_chnl_retrieve(etrans_handle->gsup43_c73_handle[gsup43_idx],
                                              0,
                                              UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, gsup43_c73),
                                              &int_table_ptr->gsup43_c73);
        int_table_ptr->gsup43_c73_valid = TRUE;
    }

    if (PMC_SUCCESS == result && TRUE == has_exco3m)
    {
        result = exco3m_int_chnl_retrieve(etrans_handle->exco3m_handle[exco3m_idx],
                                          exco3m_idx,
                                          UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, exco3m),
                                          &int_table_ptr->exco3m);
    }
    
    if (PMC_SUCCESS == result && TRUE == has_exco4m)
    {
        result = exco4m_int_chnl_retrieve(etrans_handle->exco4m_handle,
                                          link,
                                          UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, exco4m),
                                          &int_table_ptr->exco4m);
    }

    if (PMC_SUCCESS == result)
    {

        UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(etrans_handle, etrans, UTIL_GLOBAL_INT_OPTIMIZE);

#ifdef ETRANS_INT_SINGLE
#undef ETRANS_INT_SINGLE
#endif
#define ETRANS_INT_SINGLE UTIL_GLOBAL_INT_RETRIEVE_SINGLE
    
#ifdef ETRANS_INT_RANGE
#undef ETRANS_INT_RANGE
#endif
#define ETRANS_INT_RANGE    UTIL_GLOBAL_INT_RETRIEVE_RANGE
    
#ifdef ETRANS_INT_FIELD
#undef ETRANS_INT_FIELD
#endif
#define ETRANS_INT_FIELD    UTIL_GLOBAL_INT_RETRIEVE_FIELD12
   

        link1 = etrans_idx;
        link2 = etrans_idx;
        if (PMC_SUCCESS == result && TRUE == has_etrans_10g)
        {
            ETRANS_10G_INT_TABLE_DEFINE();
        }
        else if (PMC_SUCCESS == result && TRUE == has_etrans_40g)
        {
            ETRANS_40G_INT_TABLE_DEFINE();
        }
        else if (PMC_SUCCESS == result && TRUE == has_etrans_100g)
        {
            ETRANS_100G_INT_TABLE_DEFINE();
        }
    
    
#ifdef ETRANS_STATUS_SINGLE
#undef ETRANS_STATUS_SINGLE
#endif
#define ETRANS_STATUS_SINGLE UTIL_GLOBAL_STATUS_RETRIEVE_SINGLE
    
#ifdef ETRANS_STATUS_RANGE
#undef ETRANS_STATUS_RANGE
#endif
#define ETRANS_STATUS_RANGE    UTIL_GLOBAL_STATUS_RETRIEVE_RANGE
    
#ifdef ETRANS_STATUS_FIELD
#undef ETRANS_STATUS_FIELD
#endif
#define ETRANS_STATUS_FIELD    UTIL_GLOBAL_STATUS_RETRIEVE_FIELD12
   

        link1 = etrans_idx;
        link2 = etrans_idx;
        if (PMC_SUCCESS == result && TRUE == has_etrans_10g)
        {
            ETRANS_10G_STATUS_TABLE_DEFINE();
        }
        else if (PMC_SUCCESS == result && TRUE == has_etrans_40g)
        {
            ETRANS_40G_STATUS_TABLE_DEFINE();
        }
        else if (PMC_SUCCESS == result && TRUE == has_etrans_100g)
        {
            ETRANS_100G_STATUS_TABLE_DEFINE();
        }
    
        UTIL_GLOBAL_INT_BUFFER_FLUSH(etrans_handle, etrans);

        int_table_ptr->gsup43_c73_valid = has_gsup43;
        int_table_ptr->exco3m_valid = has_exco3m;
        int_table_ptr->exco4m_valid = has_exco4m;
        int_table_ptr->etrans_10g_valid = has_etrans_10g;
        int_table_ptr->etrans_40g_valid = has_etrans_40g;
        int_table_ptr->etrans_100g_valid = has_etrans_100g;
    }

    PMC_RETURN(result);
} /* etrans_int_chnl_retrieve */



/*******************************************************************************
* FUNCTION: etrans_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   etrans_handle     - handle to the ETRANS instance to be operated on
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
PUBLIC PMC_ERROR etrans_int_chnl_enabled_check(etrans_handle_t     *etrans_handle,
                                               UINT32               link,
                                               etrans_int_chnl_t   *int_en_table_ptr,
                                               BOOL8               *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1;
    UINT32          link2;
    BOOL8           has_exco4m;
    BOOL8           has_exco3m;
    UINT32          exco3m_idx;
    BOOL8           has_gsup43;
    UINT32          gsup43_idx;
    BOOL8           has_etrans_10g;
    BOOL8           has_etrans_40g;
    BOOL8           has_etrans_100g;
    UINT32          etrans_idx;

    PMC_ENTRY();

    result = etrans_int_validate(etrans_handle, link, int_found_ptr);

    if (PMC_SUCCESS == result)
    {
        /* Get the right index for EXCO3M, GSUP43, ETRANS for link */
        result = etrans_int_params_get(etrans_handle,
                                     etrans_handle->var.datapath[link],
                                     link,
                                     &has_exco4m,
                                     &has_exco3m,
                                     &exco3m_idx,
                                     &has_gsup43,
                                     &gsup43_idx,
                                     &has_etrans_10g,
                                     &has_etrans_40g,
                                     &has_etrans_100g,
                                     &etrans_idx);
    }

    if ((PMC_SUCCESS == result && TRUE == has_gsup43) &&
        (FALSE == *int_found_ptr))
    {
        result = gsup43_c73_int_chnl_enabled_check(etrans_handle->gsup43_c73_handle[gsup43_idx],
                                                   0,
                                                   &int_en_table_ptr->gsup43_c73,
                                                   int_found_ptr);
    }

    if ((PMC_SUCCESS == result && TRUE == has_exco3m) &&
        (FALSE == *int_found_ptr))
    {
        result = exco3m_int_chnl_enabled_check(etrans_handle->exco3m_handle[link],
                                               link,
                                               &int_en_table_ptr->exco3m,
                                               int_found_ptr);
    }
    
    if ((PMC_SUCCESS == result && TRUE == has_exco4m) &&
        (FALSE == *int_found_ptr))
    {
        result = exco4m_int_chnl_enabled_check(etrans_handle->exco4m_handle,
                                               link,
                                               &int_en_table_ptr->exco4m,
                                               int_found_ptr);
    }

    if (PMC_SUCCESS == result)
    {
#ifdef ETRANS_INT_SINGLE
#undef ETRANS_INT_SINGLE
#endif
#define ETRANS_INT_SINGLE UTIL_GLOBAL_INT_CHECK_SINGLE
    
#ifdef ETRANS_INT_RANGE
#undef ETRANS_INT_RANGE
#endif
#define ETRANS_INT_RANGE    UTIL_GLOBAL_INT_CHECK_RANGE
    
#ifdef ETRANS_INT_FIELD
#undef ETRANS_INT_FIELD
#endif
#define ETRANS_INT_FIELD    UTIL_GLOBAL_INT_CHECK_FIELD12
   

        link1 = etrans_idx;
        link2 = etrans_idx;
        if (PMC_SUCCESS == result && TRUE == has_etrans_10g)
        {
            ETRANS_10G_INT_TABLE_DEFINE();
        }
        else if (PMC_SUCCESS == result && TRUE == has_etrans_40g)
        {
            ETRANS_40G_INT_TABLE_DEFINE();
        }
        else if (PMC_SUCCESS == result && TRUE == has_etrans_100g)
        {
            ETRANS_100G_INT_TABLE_DEFINE();
        }
    }

    PMC_RETURN(result);
} /* etrans_int_chnl_enabled_check */


/*******************************************************************************
* etrans_int_params_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is a helper function to retrieve which interrupt structures
*   are associated with the current link.
* INPUTS:
*   etrans_handle          - handle to the ETRANS instance to be operated on
*
*   datapath               - Enumerated ETRANS datapath:
*                            ETRANS_GFP_GSUP43_C62_G709_100GE_MAC,
*                            ETRANS_GFP_GSUP43_C62_G709_40GE_MAC,
*                            ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC,
*                            ETRANS_GFP_GSUP43_C73_10GE_MAC,
*                            ETRANS_GMP_G709_100GE_MAC,
*                            ETRANS_GMP_G709_40GE_MAC,
*                            ETRANS_BMP_GSUP43_C71_10GE_MAC,
*                            ETRANS_UNUSED
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
*   has_etrans_10g         - TRUE: ETRANS 10G interrupts are valid for index
*                            etrans_idx and should be retrieved
*
*   has_etrans_40g         - TRUE: ETRANS 40G interrupts are valid for index
*                            etrans_idx and should be retrieved
*
*   has_etrans_100g        - TRUE: ETRANS 100G interrupts are valid for index
*                            etrans_idx and should be retrieved
*
*   etrans_idx             - Index for the ETRANS 10/40/100 interrupts if
*                            has_etrans10g/has_etrans40g/has_etrans100g=TRUE [0-11]
*
* RETURNS:
*   PMC_SUCCESS
*   LINEOTN_ERR_INVALID_PARAMETERS
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR etrans_int_params_get(etrans_handle_t      *etrans_handle,
                                        etrans_datapath_t     datapath,
                                        UINT32                link,
                                        BOOL8                *has_exco4m,
                                        BOOL8                *has_exco3m,
                                        UINT32               *exco3m_idx,
                                        BOOL8                *has_gsup43,
                                        UINT32               *gsup43_idx,
                                        BOOL8                *has_etrans_10g,
                                        BOOL8                *has_etrans_40g,
                                        BOOL8                *has_etrans_100g,
                                        UINT32               *etrans_idx)
{
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();

    /* Initialize returned values */
    *has_exco4m = FALSE;
    *has_exco3m = FALSE;
    *exco3m_idx = 0;
    *has_gsup43 = FALSE;
    *gsup43_idx = 0;
    *has_etrans_10g = FALSE;
    *has_etrans_40g = FALSE;
    *has_etrans_100g = FALSE;
    *etrans_idx = 0;

    switch (datapath) 
    {
    case ETRANS_GFP_GSUP43_C62_G709_100GE_MAC:
    case ETRANS_GMP_G709_100GE_MAC:
        *has_etrans_100g = TRUE;
        *etrans_idx = 0;
        *has_exco4m = TRUE;
        break;
    case ETRANS_GFP_GSUP43_C62_G709_40GE_MAC:
    case ETRANS_GMP_G709_40GE_MAC:
        PMC_ASSERT(link % 4 == 0, ETRANS_ERR_INVALID_MODE , 0 , 0);
        *has_etrans_40g = TRUE;
        *etrans_idx = link / 4;
        *has_exco3m = TRUE;
        *exco3m_idx = link / 4;
        break;
    case ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC:
    case ETRANS_GFP_GSUP43_C73_10GE_MAC:
        *has_etrans_10g = TRUE;
        *etrans_idx = link;
        *has_gsup43 = TRUE;
        *gsup43_idx = link;
        break;
    case ETRANS_BMP_GSUP43_C71_10GE_MAC:
        *has_etrans_10g = TRUE;
        *etrans_idx = link;
        break;
    default:        
        break;
    };

    PMC_RETURN(result);
}/* etrans_int_params_get */



/*******************************************************************************
*  etrans_tx_upi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets expected UPI values in data or ordered set packet and the action 
*   to be done when incoming packet UPI does not match expected values: DROP or IGNORE.
*
* INPUTS:
*   hndl           - handle to the ETRANS instance to be operated on
*   link           - link to configure
*   datapath       - datapath to configure
*   pkt_upi_val    - the value that identifies the transmit data as a 10GE data frame.
*   os_upi_val     - the value that identifies the transmit data as 10GE ordered set.
*   enable_drop    - when TRUE, incoming packet that has an UPI TYPE value that
*                    does not match either pkt_upi_val or os_upi_val
*                    are dropped.
*                    when FALSE, incoming data packet will be treated as a 
*                    data packet.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etrans_tx_upi_set(etrans_handle_t     *hndl, 
                                   UINT32               link, 
                                    etrans_datapath_t   datapath,
                                   UINT8                pkt_upi_val,
                                   UINT8                os_upi_val,
                                   BOOL8                enable_drop)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* Bring up 10G datapath */
    if (( datapath == ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC ) ||
        ( datapath == ETRANS_GFP_GSUP43_C73_10GE_MAC            ))
    {
        gsup43_c73_tx_upi_set(hndl->gsup43_c73_handle[link], 
                              pkt_upi_val,
                              os_upi_val,
                              enable_drop);
    }
    else
    {
        ret_val = ETRANS_LOG_ERR_INVALID_PARAMETERS;
    }
    
    PMC_RETURN(ret_val);

} /* etrans_tx_upi_set */

/*******************************************************************************
*  etrans_tx_upi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves expected UPI values in data or ordered set packet and the action 
*   to be done when incoming packet UPI does not match expected values: DROP or IGNORE.
*
* INPUTS:
*   hndl     - handle to the ETRANS instance to be operated on
*   link     - link to configure
*   datapath - datapath to configure
*
* OUTPUTS:
*  *pkt_upi_val_ptr     - the value that identifies the transmit data as a 10GE data frame.
*  *os_upi_val_ptr      - the value that identifies the transmit data as 10GE ordered set.
*  *enable_drop_ptr     - when TRUE, incoming packet that has an UPI TYPE value that
*                         does not match either pkt_upi_val or os_upi_val
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
PUBLIC PMC_ERROR etrans_tx_upi_get(etrans_handle_t     *hndl, 
                                   UINT32               link, 
                                    etrans_datapath_t   datapath,
                                   UINT8               *pkt_upi_val_ptr,
                                   UINT8               *os_upi_val_ptr,
                                   BOOL8               *enable_drop_ptr)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* Bring up 10G datapath */
    if (( datapath == ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC ) ||
        ( datapath == ETRANS_GFP_GSUP43_C73_10GE_MAC            ))
    {
        gsup43_c73_tx_upi_get(hndl->gsup43_c73_handle[link], 
                              pkt_upi_val_ptr,
                              os_upi_val_ptr,
                              enable_drop_ptr);
    }
    else
    {
        ret_val = ETRANS_LOG_ERR_INVALID_PARAMETERS;
    }
    
    PMC_RETURN(ret_val);

} /* etrans_tx_upi_get */

/*******************************************************************************
*  etrans_rx_upi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to configure the UPI values of data & ordered set frames.
*
* INPUTS:
*   hndl           - ETRANS handle
*   link           - link to configure
*   datapath       - datapath to configure
*   pkt_upi_val    - data frame UPI value
*   os_upi_val     - Ordered Set UPI value.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etrans_rx_upi_set(etrans_handle_t     *hndl, 
                                   UINT32               link, 
                                   etrans_datapath_t    datapath,
                                   UINT8                pkt_upi_val,
                                   UINT8                os_upi_val)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (( datapath == ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC ) ||
        ( datapath == ETRANS_GFP_GSUP43_C73_10GE_MAC            ))
    {
        ret_val = gsup43_c73_rx_upi_set(hndl->gsup43_c73_handle[link], pkt_upi_val, os_upi_val);
    }
    else
    {
        ret_val = ETRANS_LOG_ERR_INVALID_PARAMETERS;
    }
    
    PMC_RETURN(ret_val);

} /* etrans_rx_upi_set */

/*******************************************************************************
*  etrans_rx_upi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the configured UPI values for data and ordered set 
*   frames.
*
* INPUTS:
*   hndl     - ETRANS handle
*   link     - link to configure
*   datapath - datapath to configure
*
* OUTPUTS:
*  *pkt_upi_val_ptr     - Data frame UPI value
*  *os_upi_val_ptr      - Ordered set UPI value
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etrans_rx_upi_get(etrans_handle_t     *hndl, 
                                   UINT32               link, 
                                   etrans_datapath_t   datapath,
                                   UINT8               *pkt_upi_val_ptr,
                                   UINT8               *os_upi_val_ptr)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* Check if datapath is GSUP43 7.3 */
    if (( datapath == ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC ) ||
        ( datapath == ETRANS_GFP_GSUP43_C73_10GE_MAC            ))
    {
        ret_val = gsup43_c73_rx_upi_get(hndl->gsup43_c73_handle[link], 
                              pkt_upi_val_ptr,
                              os_upi_val_ptr);
    }
    else
    {
        ret_val = ETRANS_LOG_ERR_INVALID_PARAMETERS;
    }
    
    PMC_RETURN(ret_val);

} /* etrans_rx_upi_get */

/*******************************************************************************
* Run-time Operations
*******************************************************************************/
/*******************************************************************************
*  etrans_tx_lane_info_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrieves the TX_LANE_ID and TX_LANE_SKEW_FIFO_ADDR for
*   40GE/100GE datapath
*
* INPUTS:
*   hndl         - handle to the EPMM instance to be operated on
*   link         - link to configure
*   etrans_path    - path to retrieve the TX_LANE_ID and TX_LANE_SKEW_FIFO_ADDR
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
PUBLIC PMC_ERROR etrans_tx_lane_info_get(etrans_handle_t *hndl, 
                                         UINT32 link, 
                                         etrans_datapath_t etrans_path,  
                                         UINT8 txLaneId[20],
                                         UINT8 txLaneSkewFifoAddr[20])
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    switch ( etrans_path )
    { 
    case ETRANS_GFP_GSUP43_C62_G709_40GE_MAC:
    case ETRANS_GMP_G709_40GE_MAC:
        if (3 > (link / 4))
        {
            exco3m_tx_lane_info_get(hndl->exco3m_handle[link/4], txLaneId, txLaneSkewFifoAddr);
        }
        else
        {
            rc = ETRANS_LOG_ERR_INVALID_PARAMETERS;            
        }
        break;
    case ETRANS_GFP_GSUP43_C62_G709_100GE_MAC:
    case ETRANS_GMP_G709_100GE_MAC:

        exco4m_tx_lane_info_get(hndl->exco4m_handle, txLaneId, txLaneSkewFifoAddr);
        break;
    default:
        rc = ETRANS_LOG_ERR_INVALID_PARAMETERS;
        break;
    }
    

    PMC_RETURN(rc);

} /* etrans_tx_lane_info_get */

/*
** Private Functions
*/

/*
** End of file
*/
