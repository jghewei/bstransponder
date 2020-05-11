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

#include "etime_loc.h"
#include "pmc_err.h"
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
** Forwarded references
*/
PRIVATE PMC_ERROR etime_link_id_check(UINT32 link) ;

PRIVATE PMC_ERROR etime_link_rate_check(UINT32 link,    
                                        etime_data_rate_t data_rate) ;                      

PRIVATE PMC_ERROR etime_ing_47b_m_ch_set(etime_handle_t *hndl, 
                                         UINT32 channel, 
                                         UINT32 new_value);

PRIVATE PMC_ERROR etime_egr_47b_m_ch_set(etime_handle_t *hndl, 
                                         UINT32 channel, 
                                         UINT32 new_value);



PRIVATE PMC_ERROR etime_tx_ts_ena_channel_set(etime_handle_t *hndl,
                                              UINT32 channel,
                                              UINT32 value);


PRIVATE BOOL8 etime_tx_ts_ena_channel_get(etime_handle_t *hndl,
                                          UINT32 channel);

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
const char ETIME_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    ETIME_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char ETIME_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Public Functions
*/

/*******************************************************************************
* etime_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a ETIME block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the ETIME block relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - pointer to TSB name
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   etime_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC etime_handle_t *etime_ctxt_create(pmc_handle_t *parent, 
                                         UINT32 base_address, 
                                         pmc_sys_handle_t *sys_handle, 
                                         const char *tsb_name )
{
    etime_handle_t *etime_handle;

    PMC_ENTRY();

    etime_handle = (etime_handle_t*)PMC_CTXT_CALLOC(sizeof(etime_handle_t), parent);
    
    pmc_handle_init( parent, etime_handle, sys_handle, PMC_MID_DIGI_ETIME, tsb_name, base_address );

    /* Register log strings. */
    pmc_log_block_strings_register(ETIME_LOG_ERR_STRINGS[0], ETIME_LOG_ERR_TABLE_BASE, ETIME_LOG_ERR_COUNT);

    PMC_RETURN(etime_handle);
} /* etime_ctxt_create */  
 
/*******************************************************************************
* etime_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a ETIME block instance.
*
*
* INPUTS:
*   *etime_handle        - pointer to etime handle instance
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
PUBLIC void etime_ctxt_destroy(etime_handle_t *etime_handle)
{
    PMC_ENTRY();

    PMC_CTXT_FREE(&etime_handle, etime_handle);

    PMC_RETURN();
} /* etime_ctxt_destroy */

/*******************************************************************************
* etime_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a ETIME block handle instance.
*
*
* INPUTS:
*   *etime_handle        - pointer to ETIME handle instance
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
PUBLIC void etime_handle_init(etime_handle_t *etime_handle)
{
    PMC_ENTRY();

    PMC_RETURN();
} /* etime_handle_init */

/*******************************************************************************
* etime_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on etime.
*
*
* INPUTS:
*   *etime_handle         - pointer to ETIME handle instance
*   *restart_init_cfg_ptr - pointer to restart init config
*   top_energy_state_reg  - The expected energy state 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS : 
*     WARM : register/context are coherent
*     CRASH: everything is coherent (a cleanup may has been executed)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_handle_restart_init(etime_handle_t *etime_handle,
                                           util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                           pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(ETIME_LOG_ERR_STRINGS[0], ETIME_LOG_ERR_TABLE_BASE, ETIME_LOG_ERR_COUNT);
    }

    PMC_RETURN(ret_val);
} /* etime_handle_restart_init */
/*******************************************************************************
* etime_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the ETIME instance to the starting state for all
*   dynamic context.  All dynamic context will be returned to the initialization 
*   values that were generate during the call to etime_handle_init().
*
*   This function is for use by parent modules that have provided all other
*   provisions for ensuring the state of all resources tracked by this
*   context has coherent register state.  For example, if the parent module
*   were reset the cfc module was forced to the reset state than the call to
*   this API will force this context to be coherent with this starting state.
*
*
* INPUTS:
*   *hndl                 - pointer to ETIME handle instance
*   energy_state          - enum defining state to set the ETIME module to
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
PUBLIC PMC_ERROR etime_init(etime_handle_t *hndl, pmc_energy_state_t energy_state)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    PMC_ASSERT(hndl!=NULL, ETIME_LOG_ERR_INVALID_PARAMETERS, 0, 0); 
    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        /* Configure ETIME energy state set */
        etime_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RUNNING);
        /* Configure not defualt values */
        etime_block_non_default_cfg(hndl);
        /* No child blocks */
    }
    else if (PMC_ENERGY_STATE_RESET == energy_state)
    {
        /* Configure ETIME energy state set */
        etime_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RESET);
        /* No var_t members */
        /* No child blocks */
    }else {
        PMC_ASSERT(FALSE, ETIME_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }    
  
    PMC_RETURN(ret_val);

} /* etime_init */

/*******************************************************************************
* etime_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   ETIME instance.  
*
*
* INPUTS:
*   *hndl                      - pointer to ETIME handle instance
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
PUBLIC BOOL8 etime_start_state_test(etime_handle_t *hndl)
{
    BOOL8 start_state = TRUE;

    PMC_ENTRY();

    PMC_ASSERT(hndl != NULL, ETIME_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Module contains no var_t context, no testing to be performed */
    PMC_RETURN(start_state);
}/* etime_start_state_test */

/*******************************************************************************
* etime_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of a ETIME block instance.
*
*
* INPUTS:
*   *etime_handle        - pointer to ETIME handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Base address of ETIME.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 etime_base_address_get(etime_handle_t *etime_handle)
{
    PMC_ENTRY();

    PMC_RETURN(etime_handle->base.base_address);
} /* etime_base_address_get */

/*******************************************************************************
* Initialization Configuration Functions
*******************************************************************************/

/*******************************************************************************
*  etime_refl_lnk_rst
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reset reflection link
*
* INPUTS:
*   hndl      - handle to the ETIME instance to be operated on
*   link      - link to reset
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_refl_lnk_rst(etime_handle_t *hndl, 
                                    UINT32 link)
{
    PMC_ENTRY();


    switch (link)
    {
    case 0:     
        etime_field_REFL_LINK_RESET_0_set(NULL, hndl, 1);
        break;    
    case 1:     
        etime_field_REFL_LINK_RESET_1_set(NULL, hndl, 1);
        break;   
    case 2:     
        etime_field_REFL_LINK_RESET_2_set(NULL, hndl, 1);
        break;
    
    case 3:
        
        etime_field_REFL_LINK_RESET_3_set(NULL, hndl, 1);
        break;
    case 4:
        
        etime_field_REFL_LINK_RESET_4_set(NULL, hndl, 1);
        break;
    case 5:
        
        etime_field_REFL_LINK_RESET_5_set(NULL, hndl, 1);
        break;
    case 6:
        
        etime_field_REFL_LINK_RESET_6_set(NULL, hndl, 1);
        break;
    case 7:
        
        etime_field_REFL_LINK_RESET_7_set(NULL, hndl, 1);
        break;
    case 8:
        
        etime_field_REFL_LINK_RESET_8_set(NULL, hndl, 1);
        break;
    case 9:
        
        etime_field_REFL_LINK_RESET_9_set(NULL, hndl, 1);
        break;
    case 10:
        
        etime_field_REFL_LINK_RESET_10_set(NULL, hndl, 1);
        break;
    case 11:    
        etime_field_REFL_LINK_RESET_11_set(NULL, hndl, 1);
        break;
    default:
        PMC_RETURN(ETIME_LOG_ERR_INVALID_PARAMETERS);
    }   

    PMC_RETURN(PMC_SUCCESS);

} /* etime_refl_lnk_rst */


/*******************************************************************************
*  etime_rx_ts_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure ingress links for specified client data rate and CFC mode
*
* INPUTS:
*   hndl         - handle to the ETIME instance to be operated on
*   link         - link to configure
*   data_rate    - data rate for the link
*   cfc_mode     - CFC mode for the link (48B or 47B)
*   link_latency - the link latency
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_rx_ts_cfg(etime_handle_t *hndl, 
                                 UINT32 link, 
                                 etime_data_rate_t data_rate, 
                                 etime_cfc_mode_t cfc_mode, 
                                 UINT32 link_latency)
{
    PMC_ERROR result;

    PMC_ENTRY();

    /* check parameters value */
    result = etime_link_id_check(link);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }

    result = etime_link_rate_check(link, data_rate);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }   

    /* Register 0x0000006c ING_47B_M */
    etime_ing_47b_m_ch_set(hndl, link, cfc_mode);
            
    PMC_RETURN(PMC_SUCCESS);
    
} /* etime_rx_ts_cfg */

/*******************************************************************************
*  etime_tx_ts_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure egress links for specified client data rate and CFC mode
*
* INPUTS:
*   hndl         - handle to the ETIME instance to be operated on
*   link         - link to configure
*   data_rate    - data rate for the link
*   cfc_mode     - CFC mode for the link (48B or 47B)
*   link_latency - the link latency.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_tx_ts_cfg(etime_handle_t *hndl, 
                                 UINT32 link, 
                                 etime_data_rate_t data_rate, 
                                 etime_cfc_mode_t cfc_mode, 
                                 UINT32 link_latency)
{
    PMC_ERROR result;

    PMC_ENTRY();

    /* check parameters value */
    result = etime_link_id_check(link);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }

    result = etime_link_rate_check(link, data_rate);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }   

    /* Register 0x00000070 EGR_47B_M */
    etime_egr_47b_m_ch_set(hndl, link, cfc_mode);

        
    PMC_RETURN(PMC_SUCCESS);

} /* etime_tx_ts_cfg */

/*******************************************************************************
*  etime_refl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure reflection links for specified client data rate and pkt dest.
*
* INPUTS:
*   hndl          - handle to the ETIME instance to be operated on
*   link          - link to configure
*   data_rate     - data rate for the link
*   drop_err      - Timeout or timestamp mismatch packets behavior
*   refl_dst      - Destination of reflected packets (CPU or MGMT)
*   use_page_swap - TRUE if page swaping is in use.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_refl_cfg(etime_handle_t *hndl, 
                                UINT32 link, 
                                etime_data_rate_t data_rate,
                                etime_refl_drop_err_t drop_err, 
                                etime_refl_dst_t refl_dst,
                                BOOL8 use_page_swap)
{
    etime_buffer_t etime_buffer[1];

    PMC_ENTRY();
    
/* Initialize etime buffer */
    etime_buffer_init(etime_buffer, hndl);

    /* Check for valid link */
    if (ETIME_LINK_100G == data_rate) 
    {
        if (ETIME_100G_LINK_0 != link) 
        {
            PMC_RETURN(ETIME_LOG_ERR_INVALID_PARAMETERS);
        }
    } 
    else if (ETIME_LINK_40G == data_rate) 
    {
        if ((ETIME_40G_LINK_0 != link) && (ETIME_40G_LINK_4 != link) && (ETIME_40G_LINK_8 != link))
        {
            PMC_RETURN(ETIME_LOG_ERR_INVALID_PARAMETERS);
        }
    } /* end if */
    



    /* Configure link scheduler */ 
    switch (data_rate)
    {
    case ETIME_LINK_100G:
        /* Put inactive value in Reflection path DPI calendar */
        etime_field_EXT_CPB_LINK_SCH_TS0_set(etime_buffer, hndl, 0x0);
        etime_field_EXT_CPB_LINK_SCH_TS1_set(etime_buffer, hndl, 0x0);
        etime_field_EXT_CPB_LINK_SCH_TS2_set(etime_buffer, hndl, 0x0);
        etime_field_EXT_CPB_LINK_SCH_TS3_set(etime_buffer, hndl, 0x0);
        etime_field_EXT_CPB_LINK_SCH_TS4_set(etime_buffer, hndl, 0x0);
        etime_field_EXT_CPB_LINK_SCH_TS5_set(etime_buffer, hndl, 0x0);
        etime_field_EXT_CPB_LINK_SCH_TS6_set(etime_buffer, hndl, 0x0);
        etime_field_EXT_CPB_LINK_SCH_TS7_set(etime_buffer, hndl, 0x0);
        etime_field_EXT_CPB_LINK_SCH_TS8_set(etime_buffer, hndl, 0x0);
        etime_field_EXT_CPB_LINK_SCH_TS9_set(etime_buffer, hndl, 0x0);
        etime_field_EXT_CPB_LINK_SCH_TS10_set(etime_buffer, hndl, 0xF);
        etime_field_EXT_CPB_LINK_SCH_TS11_set(etime_buffer, hndl, 0xF);
        break;

    case ETIME_LINK_40G:
        switch (link)
        {
        case ETIME_40G_LINK_0:
            etime_field_EXT_CPB_LINK_SCH_TS0_set(etime_buffer, hndl, link);
            etime_field_EXT_CPB_LINK_SCH_TS3_set(etime_buffer, hndl, link);
            etime_field_EXT_CPB_LINK_SCH_TS6_set(etime_buffer, hndl, link);
            etime_field_EXT_CPB_LINK_SCH_TS9_set(etime_buffer, hndl, link);
            break;
        case ETIME_40G_LINK_4:
            etime_field_EXT_CPB_LINK_SCH_TS1_set(etime_buffer, hndl, link);
            etime_field_EXT_CPB_LINK_SCH_TS4_set(etime_buffer, hndl, link);
            etime_field_EXT_CPB_LINK_SCH_TS7_set(etime_buffer, hndl, link);
            etime_field_EXT_CPB_LINK_SCH_TS10_set(etime_buffer, hndl, link);
            break;
        default:
            etime_field_EXT_CPB_LINK_SCH_TS2_set(etime_buffer, hndl, link);
            etime_field_EXT_CPB_LINK_SCH_TS5_set(etime_buffer, hndl, link);
            etime_field_EXT_CPB_LINK_SCH_TS8_set(etime_buffer, hndl, link);
            etime_field_EXT_CPB_LINK_SCH_TS11_set(etime_buffer, hndl, link);
            break;
        } /* end if */
        break;

    default:
        if (0 == link) {
            etime_field_EXT_CPB_LINK_SCH_TS0_set(etime_buffer, hndl, link);

        } else if (1 == link) {
            etime_field_EXT_CPB_LINK_SCH_TS3_set(etime_buffer, hndl, link);

        } else if (2 == link) {
            etime_field_EXT_CPB_LINK_SCH_TS6_set(etime_buffer, hndl, link);

        } else if (3 == link) {
            etime_field_EXT_CPB_LINK_SCH_TS9_set(etime_buffer, hndl, link);

        } else if (4 == link) {
            etime_field_EXT_CPB_LINK_SCH_TS1_set(etime_buffer, hndl, link);

        } else if (5 == link) {
            etime_field_EXT_CPB_LINK_SCH_TS4_set(etime_buffer, hndl, link);

        } else if (6 == link) {
            etime_field_EXT_CPB_LINK_SCH_TS7_set(etime_buffer, hndl, link);

        } else if (7 == link) {
            etime_field_EXT_CPB_LINK_SCH_TS10_set(etime_buffer, hndl, link);

        } else if (8 == link) {
            etime_field_EXT_CPB_LINK_SCH_TS2_set(etime_buffer, hndl, link);

        } else if (9 == link) {
            etime_field_EXT_CPB_LINK_SCH_TS5_set(etime_buffer, hndl, link);

        } else if (10 == link) {
            etime_field_EXT_CPB_LINK_SCH_TS8_set(etime_buffer, hndl, link);

        } else if (11 == link) {
            etime_field_EXT_CPB_LINK_SCH_TS11_set(etime_buffer, hndl, link);

        } 

    } /* switch data_rate */

    etime_buffer_flush(etime_buffer);

    if( use_page_swap == FALSE ) {
        /* Update Reflection DPI scheduler */
        etime_field_EXT_CPB_LINK_SCH_UPD_set(NULL, hndl, 1);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* etime_refl_cfg */

/*******************************************************************************
*  etime_rx_ts_ena
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable or disable timestamping on ingress links
*
* INPUTS:
*   hndl      - handle to the ETIME instance to be operated on
*   enable    - enable timestamping on ingress links
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_rx_ts_ena(etime_handle_t *hndl, BOOL8 enable)
{
    PMC_ENTRY();

    /* Register 0x8: ETIME120 - Ingress Link Timestamping Enable Register */
    etime_field_RX_TS_ENA_set(NULL, hndl, enable);

    PMC_RETURN(PMC_SUCCESS);

} /* etime_rx_ts_ena */

/*******************************************************************************
*  etime_tx_ts_ena
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable or disable timestamping on egress link
*
* INPUTS:
*   hndl      - handle to the ETIME instance to be operated on
*   link      - link to configure
*   enable    - enable timestamping on egress link
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_tx_ts_ena(etime_handle_t *hndl, 
                                 UINT32 link, 
                                 BOOL8 enable)
{
    
    PMC_ENTRY();
    
    /* Register 0xC: ETIME120 - Egress Link Timestamping Enable Register */
    etime_tx_ts_ena_channel_set(hndl, link, enable);
    
    PMC_RETURN(PMC_SUCCESS);

} /* etime_tx_ts_ena */

/*******************************************************************************
*  etime_refl_ena
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable or disable reflection link
*
* INPUTS:
*   hndl      - handle to the ETIME instance to be operated on
*   enable    - Enable reflection link
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_refl_ena(etime_handle_t *hndl, BOOL8 enable)
{
    PMC_ENTRY();

    /* Register 0x0000029c ENA_REG */
    etime_field_REFL_PATH_ENA_set(NULL, hndl, enable);

    PMC_RETURN(PMC_SUCCESS);

} /* etime_refl_ena */


/*******************************************************************************
* etime_activated_link_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns ETIME RX and TX channels status: enabled or disabled.
*                                                                               
* INPUTS:                                                                       
*   *hndl             - pointer to ETIME handle instance
*                                                                             
* OUTPUTS:
*   *rx_actv          - status of etime RX block
*   *tx_actv_link     - list of etime TX channels status
*
* RETURNS:
*   None.                                                                        
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC void etime_activated_link_get(etime_handle_t   *hndl,
                                     BOOL8           *rx_actv,
                                     BOOL8           *tx_actv_link)
{
    UINT32 itr;
    PMC_ENTRY();
    
    PMC_ASSERT(hndl != NULL, ETIME_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    *rx_actv = etime_field_RX_TS_ENA_get(NULL, hndl);

    for (itr = 0; itr < ETIME_NUM_LINKS; itr++)
    {
        tx_actv_link[itr] = (etime_tx_ts_ena_channel_get(hndl, itr) && etime_field_REFL_PATH_ENA_get(NULL, hndl));
    }

    PMC_RETURN();
}  /* etime_activated_link_get */


/*******************************************************************************
*  etime_ptp_cntr_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure PTP_CLK Timestamp Counter
*
* INPUTS:
*   hndl          - handle to the ETIME instance to be operated on
*   offset_quanta - Defines the size of changes to nano seconds/fractional seconds value in TOD_OFFSET_IN for
*                   TOD_OFFSET_UPD_NOW operation
*   ptp_freq      - Frequency of PTP_CLK
*   enable        - enable PTP_CLK Timestamp Counter
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_ptp_cntr_cfg(etime_handle_t *hndl, UINT32 offset_quanta, etime_ptp_freq_t ptp_freq, BOOL8 enable)
{
    PMC_ENTRY();

    /* Only supporting 125 MHz default clock frequency for now */
    if (ETIME_PTP_CNTR_125MHZ != ptp_freq) {
        PMC_RETURN(ETIME_LOG_ERR_INVALID_PARAMETERS);
    }

    /* Disabling PTP TS counter during configuration */
    etime_field_TS_CNTR_EN_set(NULL, hndl, 0);

    /* Programming OFFSET Quanta (using defaults for other settings */
    etime_field_OFFSET_UPD_QUANTA_set(NULL, hndl, offset_quanta);

    /* Enabling / Disabling PTP TS counter */
    etime_field_TS_CNTR_EN_set(NULL, hndl, enable);

    PMC_RETURN(PMC_SUCCESS);

} /* etime_ptp_cntr_cfg */

/*******************************************************************************
*  etime_line_ref_cntr_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure LINE_REF_CLK Timestamp Counter
*
* INPUTS:
*   hndl          - handle to the ETIME instance to be operated on
*   sampling_intvl- Defines the number of LINE_REF clock cycles between samples of the FRC counters
*   line_ref_freq - Frequency of LINE_REF_CLK
*   enable        - enable LINE_REF_CLK Timestamp Counter
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_line_ref_cntr_cfg(etime_handle_t *hndl, UINT32 sampling_intvl, etime_line_ref_freq_t line_ref_freq, BOOL8 enable)
{
    PMC_ENTRY();

    /* Register 0x8: ETIME120 - Ingress Link Timestamping Enable Register */
    /* Register 0x264: ETIME120 - ETIME Ingress Per link configuration register */

    PMC_RETURN(ETIME_LOG_ERR_UNKNOWN);

} /* etime_line_ref_cntr_cfg */


/*******************************************************************************
 * Run-time Operations
 *******************************************************************************/
/*******************************************************************************
*  etime_tod_offset_upd
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Update TOD offset
*
* INPUTS:
*   hndl          - handle to the ETIME instance to be operated on
*   ts_page       - Timestamp page
*   offset_sec_hi - Offset seconds value (High bits)
*   offset_sec_lo - Offset seconds value (Low bits)
*   offset_nsec   - Offset nanoseconds value
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_tod_offset_upd(etime_handle_t *hndl, etime_ts_page_t ts_page, 
                                      UINT32 offset_sec_hi, UINT32 offset_sec_lo, UINT32 offset_nsec)
{
    etime_buffer_t etime_buffer[1];

    PMC_ENTRY();
    
/*  Initialize etime buffer */
    etime_buffer_init(etime_buffer, hndl);

    etime_field_OFFS_TS_PAGE_set(etime_buffer, hndl,  ts_page);
    etime_field_TOD_OFFS_SEC_HI_set(etime_buffer, hndl, offset_sec_hi);
    etime_field_TOD_OFFS_SEC_LO_set(etime_buffer, hndl, offset_sec_lo);
    etime_field_TOD_OFFS_NSEC_set(etime_buffer, hndl, offset_nsec);

    etime_buffer_flush(etime_buffer);

    PMC_RETURN(PMC_SUCCESS);

} /* etime_tod_offset_upd */

/*******************************************************************************
*  etime_tod_upd_time_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Delayed TOD offset time configuration
*
* INPUTS:
*   hndl            - handle to the ETIME instance to be operated on
*   upd_time_sec_hi - Offset update time seconds value (High bits)
*   upd_time_sec_lo - Offset update time seconds value (Low bits)
*   upd_time_nsec   - Offset update time nanoseconds value
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_tod_upd_time_cfg(etime_handle_t *hndl, UINT32 upd_time_sec_hi, UINT32 upd_time_sec_lo, UINT32 upd_time_nsec)
{
    etime_buffer_t etime_buffer[1];

    PMC_ENTRY();

/*  Initialize etime buffer */
    etime_buffer_init(etime_buffer, hndl);

    /* Setting new TOD offset update time */
    etime_field_OFFS_UPD_TIME_SEC_HI_set(etime_buffer, hndl, upd_time_sec_hi);
    etime_field_OFFS_UPD_TIME_SEC_LO_set(etime_buffer, hndl, upd_time_sec_lo);
    etime_field_OFFS_UPD_TIME_NSEC_set(etime_buffer, hndl, upd_time_nsec);


/*  flush etime buffer to activate device access */
    etime_buffer_flush(etime_buffer);

    PMC_RETURN(PMC_SUCCESS);

} /* etime_tod_upd_time_cfg */

/*******************************************************************************
*  etime_cfc_offset_upd
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Update CFC offset
*
* INPUTS:
*   hndl           - handle to the ETIME instance to be operated on
*   offset_nsec_hi - Offset nanoseconds value (High bits)
*   offset_nsec_lo - Offset nanoseconds value (Low bits)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_cfc_offset_upd(etime_handle_t *hndl, UINT32 offset_nsec_hi, UINT32 offset_nsec_lo)
{
    PMC_ENTRY();

    etime_field_CFC_OFFS_NSEC_HI_set(NULL, hndl, offset_nsec_hi);
    etime_field_CFC_OFFS_NSEC_LO_set(NULL, hndl, offset_nsec_lo);
    PMC_RETURN(PMC_SUCCESS);

} /* etime_cfc_offset_upd */



/*******************************************************************************
*  etime_irigi_config_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Obtain the IRIGI configuration
*
* INPUTS:
*   hndl           - handle to the ETIME instance to be operated on
*
* OUTPUTS:
*   *mode          - determines the mode between TOD and B00x mode
*   *is_enabled    - determines if the IRIGI is enabled / disabled
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PUBLIC void etime_irigi_config_get(etime_handle_t    *hndl,
                                   etime_irig_mode_t *mode,
                                   BOOL8              *is_enabled)
{
    PMC_ENTRY();

    UINT32 uint32_mode;

    uint32_mode = etime_field_IRIGI_MODE_get(NULL, hndl);
    *is_enabled = etime_field_IRIGI_ENA_get(NULL, hndl);

    if (uint32_mode == 0x0) {

       *mode = ETIME_IRIG_TOD;

    } else if (uint32_mode == 0x1) {

       *mode = ETIME_IRIG_B00X;

    } else {

#ifndef PMC_SW_SIMULATION
       PMC_ASSERT(FALSE, ETIME_LOG_ERR_INVALID_PARAMETERS, 0, 0);
#else   
       *mode = ETIME_IRIG_TOD;
#endif

    }
  
    PMC_RETURN();

} /* etime_irigi_config_get */

/*******************************************************************************
*  etime_irigi_config_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the IRIGI
*
* INPUTS:
*   hndl           - handle to the ETIME instance to be operated on
*   mode           - select between TOD and B00x mode
*   enable         - enable / disable the IRIGI
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_irigi_config_set(etime_handle_t    *hndl,
                                        etime_irig_mode_t mode,
                                        BOOL8              enable)
{
    etime_buffer_t etime_buffer[1];

    PMC_ENTRY();

    etime_buffer_init(etime_buffer, hndl);

    /* Check if the mode is valid */
    if (mode != ETIME_IRIG_TOD && mode != ETIME_IRIG_B00X) {
        PMC_RETURN(ETIME_LOG_ERR_INVALID_PARAMETERS);
    }

    etime_field_IRIGI_ENA_set(etime_buffer, hndl, enable);

    etime_field_IRIGI_MODE_set(etime_buffer, hndl, mode);
  
    etime_buffer_flush(etime_buffer);

    PMC_RETURN(PMC_SUCCESS);

} /* etime_irigi_config_set */

/*******************************************************************************
*  etime_irigo_config_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Obtain the IRIGO configuration
*
* INPUTS:
*   hndl           - handle to the ETIME instance to be operated on
*
* OUTPUTS:
*   *mode          - determines the mode between TOD and B00x mode
*   *is_enabled    - determines if the IRIGO is enabled / disabled
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC void etime_irigo_config_get(etime_handle_t    *hndl,
                                   etime_irig_mode_t *mode,
                                   BOOL8              *is_enabled)
{
    PMC_ENTRY();

    UINT32 uint32_mode;

    uint32_mode = etime_field_IRIGO_MODE_get(NULL, hndl);
    *is_enabled = etime_field_IRIGO_ENA_get(NULL, hndl);

    if (uint32_mode == 0x0) {

       *mode = ETIME_IRIG_TOD;

    } else if (uint32_mode == 0x1) {

       *mode = ETIME_IRIG_B00X;

    } else {

#ifndef PMC_SW_SIMULATION
       PMC_ASSERT(FALSE, ETIME_LOG_ERR_INVALID_PARAMETERS, 0, 0);
#else   
       *mode = ETIME_IRIG_TOD;
#endif

    }
  
    PMC_RETURN();

} /* etime_irigo_config_get */

/*******************************************************************************
*  etime_irigo_config_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the IRIGO
*
* INPUTS:
*   hndl           - handle to the ETIME instance to be operated on
*   mode           - select between TOD and B00x mode
*   enable         - enable / disable the IRIGO
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_irigo_config_set(etime_handle_t    *hndl,
                                        etime_irig_mode_t mode,
                                        BOOL8             enable)
{

    etime_buffer_t etime_buffer[1];

    PMC_ENTRY();

    etime_buffer_init(etime_buffer, hndl);

    /* Check if the mode is valid */
    if (mode != ETIME_IRIG_TOD && mode != ETIME_IRIG_B00X) {
        PMC_RETURN(ETIME_LOG_ERR_INVALID_PARAMETERS);
    }

    etime_field_IRIGO_ENA_set(etime_buffer, hndl, enable);

    etime_field_IRIGO_MODE_set(etime_buffer, hndl, mode);
  
    etime_buffer_flush(etime_buffer);

    PMC_RETURN(PMC_SUCCESS);

} /* etime_irigo_config_set */

/*******************************************************************************
*  etime_irigi_data_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read the IRIGI data port
*
* INPUTS:
*   hndl           - handle to the ETIME instance to be operated on
*
* OUTPUTS:
*   data4_ptr       - Up to 100-bit of data to be Rxed on the IRIGI datain
*   data3_ptr       - Up to 100-bit of data to be Rxed on the IRIGI datain
*   data2_ptr       - Up to 100-bit of data to be Rxed on the IRIGI datain
*   data1_ptr       - Up to 100-bit of data to be Rxed on the IRIGI datain
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*        - Data4[31:0] is MSB, Data1[3:0] is LSB
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_irigi_data_get(etime_handle_t    *hndl,
                                      UINT32            *data4_ptr,
                                      UINT32            *data3_ptr,
                                      UINT32            *data2_ptr,
                                      UINT32            *data1_ptr)
{
    UINT32 data4;
    UINT32 data3;
    UINT32 data2;
    UINT32 data1;

    PMC_ENTRY();


    data4 = etime_field_RX_VEC_IRIGI_4_get(NULL, hndl) & ETIME120_REG_RX_VEC_IRIGI_4_REG_BIT_RX_VEC_IRIGI_4_MSK;
    data3 = etime_field_RX_VEC_IRIGI_3_get(NULL, hndl) & ETIME120_REG_RX_VEC_IRIGI_3_REG_BIT_RX_VEC_IRIGI_3_MSK;
    data2 = etime_field_RX_VEC_IRIGI_2_get(NULL, hndl) & ETIME120_REG_RX_VEC_IRIGI_2_REG_BIT_RX_VEC_IRIGI_2_MSK;
    data1 = etime_field_RX_VEC_IRIGI_1_get(NULL, hndl) & ETIME120_REG_RX_VEC_IRIGI_1_REG_BIT_RX_VEC_IRIGI_1_MSK;

    *data4_ptr = data4;
    *data3_ptr = data3;
    *data2_ptr = data2;
    *data1_ptr = data1;

    PMC_RETURN(PMC_SUCCESS);

} /* etime_irigi_data_get */

/*******************************************************************************
*  etime_irigo_data_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Write the IRIGO data port
*
* INPUTS:
*   hndl           - handle to the ETIME instance to be operated on
*   data4          - Up to 100-bit of data to be Txed on the IRIGO dataout
*   data3          - Up to 100-bit of data to be Txed on the IRIGO dataout
*   data2          - Up to 100-bit of data to be Txed on the IRIGO dataout
*   data1          - Up to 100-bit of data to be Txed on the IRIGO dataout
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*                  - Data4[31:0] is MSB, Data1[3:0] is LSB
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_irigo_data_set(etime_handle_t    *hndl,
                                      UINT32            data4,
                                      UINT32            data3,
                                      UINT32            data2,
                                      UINT32            data1)
{
    PMC_ENTRY();

    /* Check if that the data0 is not outside range */
    if (data1 > ETIME120_REG_TX_VEC_IRIGO_1_REG_BIT_TX_VEC_IRIGO_1_MSK) {
        PMC_RETURN(ETIME_LOG_ERR_INVALID_PARAMETERS);
    }

    etime_field_TX_VEC_IRIGO_4_set(NULL, hndl, data4);
    etime_field_TX_VEC_IRIGO_3_set(NULL, hndl, data3);
    etime_field_TX_VEC_IRIGO_2_set(NULL, hndl, data2);
    etime_field_TX_VEC_IRIGO_1_set(NULL, hndl, data1);
  
    PMC_RETURN(PMC_SUCCESS);

} /* etime_irigo_data_set */



/*
** Private Functions
*/
      
/*******************************************************************************
* emac_link_id_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Check that link id is valid.
*
*
* INPUTS:
*   link           - link to configure
*
* OUTPUTS:
*   None.
*
* RETURNS:
*      EMAC_LOG_ERR_INVALID_PARAMETERS   - when link id is invalid
*      PMC_SUCCESS                       - when link id is valid
*
* NOTES:
*
*******************************************************************************/    
PRIVATE PMC_ERROR etime_link_id_check(UINT32 link)   
{
    PMC_ENTRY();

    if (link >= ETIME_NUM_LINKS )
    {
        PMC_RETURN(ETIME_LOG_ERR_INVALID_PARAMETERS);
    }

    PMC_RETURN(PMC_SUCCESS);
}


/*******************************************************************************
* etime_link_rate_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Check that link id is valid accroding to link rate.
*
*
* INPUTS:
*   link           - link to configure
*   data_rate      - data rate for the link
*
* OUTPUTS:
*   None.
*
* RETURNS:
*      ETIME_LOG_ERR_INVALID_PARAMETERS   - when link id is invalid
*      PMC_SUCCESS                       - when link id is valid
*
* NOTES:
*
*******************************************************************************/    
PRIVATE PMC_ERROR etime_link_rate_check(UINT32 link,    
                                       etime_data_rate_t data_rate)   
{
    PMC_ENTRY();

    /* check that link rate is superorior to 0 */
    if (data_rate != ETIME_LINK_100G && data_rate != ETIME_LINK_40G 
        && data_rate != ETIME_LINK_10G )
    {
        PMC_RETURN(ETIME_LOG_ERR_INVALID_PARAMETERS);
    }
        
    /* Check for valid link */
    if (ETIME_LINK_100G == data_rate) 
    {
        if (ETIME_100G_LINK_0 != link) 
        {
            PMC_RETURN(ETIME_LOG_ERR_INVALID_PARAMETERS);
        }
    } 
    else if (ETIME_LINK_40G == data_rate) 
    {
        if ((ETIME_40G_LINK_0 != link) && (ETIME_40G_LINK_4 != link) && (ETIME_40G_LINK_8 != link)) 
        {
            PMC_RETURN(ETIME_LOG_ERR_INVALID_PARAMETERS);
        }
    } /* end if */

    PMC_RETURN(PMC_SUCCESS);
}                                                                                                      
/*******************************************************************************
*  etime_ing_47b_m_ch_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set value in ingress 47b mode for a specific channel
*
* INPUTS:
*   hndl          - handle to the ETIME instance to be operated on
*   channel       - channel id to set
*   new_value     - value to write in field 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR etime_ing_47b_m_ch_set(etime_handle_t *hndl, 
                                         UINT32 channel, 
                                         UINT32 new_value)
{
    PMC_ENTRY();
        
    switch (channel)
    {
    case 0:      
        etime_field_ING_47B_M_CH0_set(NULL, hndl, new_value);
        break;    
    case 1:     
        etime_field_ING_47B_M_CH1_set(NULL, hndl, new_value);
        break;   
    case 2:
        etime_field_ING_47B_M_CH2_set(NULL, hndl, new_value);   
        break;
    
    case 3:
        etime_field_ING_47B_M_CH3_set(NULL, hndl, new_value);
        break;
    case 4:
        
        etime_field_ING_47B_M_CH4_set(NULL, hndl, new_value);
        break;
    case 5:
        
        etime_field_ING_47B_M_CH5_set(NULL, hndl, new_value);
        break;
    case 6:
        
        etime_field_ING_47B_M_CH6_set(NULL, hndl, new_value);
        break;
    case 7:
        
        etime_field_ING_47B_M_CH7_set(NULL, hndl, new_value);
        break;
    case 8:
        
        etime_field_ING_47B_M_CH8_set(NULL, hndl, new_value);
        break;
    case 9:
        
        etime_field_ING_47B_M_CH9_set(NULL, hndl, new_value);
        break;
    case 10:
        
        etime_field_ING_47B_M_CH10_set(NULL, hndl, new_value);
        break;
    case 11:
        etime_field_ING_47B_M_CH11_set(NULL, hndl, new_value);  
        break;
    default:
        PMC_RETURN(ETIME_LOG_ERR_INVALID_PARAMETERS);
    }

    PMC_RETURN(PMC_SUCCESS);    
}

/*******************************************************************************
*  etime_ing_47b_m_ch_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set value in egress counter 47 bit mode for a specific channel
*
* INPUTS:
*   hndl          - handle to the ETIME instance to be operated on
*   channel       - channel id to set
*   new_value     - value to write in field 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR etime_egr_47b_m_ch_set(etime_handle_t *hndl, 
                                         UINT32 channel, 
                                         UINT32 new_value)
{
    PMC_ENTRY();
        
    switch (channel)
    {
    case 0:      
        etime_field_EGR_47B_M_CH0_set(NULL, hndl, new_value);
        break;    
    case 1:     
        etime_field_EGR_47B_M_CH1_set(NULL, hndl, new_value);
        break;   
    case 2:
        etime_field_EGR_47B_M_CH2_set(NULL, hndl, new_value);   
        break;
    
    case 3:
        etime_field_EGR_47B_M_CH3_set(NULL, hndl, new_value);
        break;
    case 4:
        
        etime_field_EGR_47B_M_CH4_set(NULL, hndl, new_value);
        break;
    case 5:
        
        etime_field_EGR_47B_M_CH5_set(NULL, hndl, new_value);
        break;
    case 6:
        
        etime_field_EGR_47B_M_CH6_set(NULL, hndl, new_value);
        break;
    case 7:
        
        etime_field_EGR_47B_M_CH7_set(NULL, hndl, new_value);
        break;
    case 8:
        
        etime_field_EGR_47B_M_CH8_set(NULL, hndl, new_value);
        break;
    case 9:
        
        etime_field_EGR_47B_M_CH9_set(NULL, hndl, new_value);
        break;
    case 10:
        
        etime_field_EGR_47B_M_CH10_set(NULL, hndl, new_value);
        break;
    case 11:
        etime_field_EGR_47B_M_CH11_set(NULL, hndl, new_value);  
        break;
    default:
        PMC_RETURN(ETIME_LOG_ERR_INVALID_PARAMETERS);
        
    }

    PMC_RETURN(PMC_SUCCESS);
}






/*******************************************************************************
*   etime_tx_ts_ena_channel_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable or disable egress timestamp feature
*
* INPUTS:
*   hndl           - handle to the ETIME instance to be operated on
*   channel        - channel to disable
*   value          - value to set 167

*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR etime_tx_ts_ena_channel_set(etime_handle_t *hndl,
                                              UINT32 channel,
                                              UINT32 value
    )
{

    PMC_ENTRY();
        
    switch (channel)
    {
    case 0:      
        etime_field_TX_TS_ENA_LINK0_set(NULL, hndl, value);
        break;    
    case 1 :    
        etime_field_TX_TS_ENA_LINK1_set(NULL, hndl, value);
        break;   
    case 2:
        etime_field_TX_TS_ENA_LINK2_set(NULL, hndl, value);     
        break;
    
    case 3:
        etime_field_TX_TS_ENA_LINK3_set(NULL, hndl, value);
        break;
    case 4:
        
        etime_field_TX_TS_ENA_LINK4_set(NULL, hndl, value);
        break;
    case 5:
        
        etime_field_TX_TS_ENA_LINK5_set(NULL, hndl, value);
        break;
    case 6:
        
        etime_field_TX_TS_ENA_LINK6_set(NULL, hndl, value);
        break;
    case 7:
        
        etime_field_TX_TS_ENA_LINK7_set(NULL, hndl, value);
        break;
    case 8:
        
        etime_field_TX_TS_ENA_LINK8_set(NULL, hndl, value);
        break;
    case 9:
        
        etime_field_TX_TS_ENA_LINK9_set(NULL, hndl, value);
        break;
    case 10:
        
        etime_field_TX_TS_ENA_LINK10_set(NULL, hndl, value);
        break;
    case 11:
        etime_field_TX_TS_ENA_LINK11_set(NULL, hndl, value);    
        break;
    default:
        PMC_RETURN(ETIME_LOG_ERR_INVALID_PARAMETERS);
    }


    PMC_RETURN(PMC_SUCCESS);    
}

/*******************************************************************************
*   etime_tx_ts_ena_channel_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Return status of egress timestamp feature: ON or OFF
*
* INPUTS:
*   hndl           - handle to the ETIME instance to be operated on
*   channel        - channel id.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE when link egress TS feature is enabled, FALSE otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 etime_tx_ts_ena_channel_get(etime_handle_t *hndl,
                                          UINT32          channel)
{
    BOOL8 rc;
    PMC_ENTRY();
        
    switch (channel)
    {
    case 0:      
        rc = etime_field_TX_TS_ENA_LINK0_get(NULL, hndl);
        break;    
    case 1 :    
        rc = etime_field_TX_TS_ENA_LINK1_get(NULL, hndl);
        break;   
    case 2:
        rc = etime_field_TX_TS_ENA_LINK2_get(NULL, hndl);     
        break;
    
    case 3:
        rc = etime_field_TX_TS_ENA_LINK3_get(NULL, hndl);
        break;
    case 4:
        
        rc = etime_field_TX_TS_ENA_LINK4_get(NULL, hndl);
        break;
    case 5:
        
        rc = etime_field_TX_TS_ENA_LINK5_get(NULL, hndl);
        break;
    case 6:
        
        rc = etime_field_TX_TS_ENA_LINK6_get(NULL, hndl);
        break;
    case 7:
        
        rc = etime_field_TX_TS_ENA_LINK7_get(NULL, hndl);
        break;
    case 8:
        
        rc = etime_field_TX_TS_ENA_LINK8_get(NULL, hndl);
        break;
    case 9:
        
        rc = etime_field_TX_TS_ENA_LINK9_get(NULL, hndl);
        break;
    case 10:
        
        rc = etime_field_TX_TS_ENA_LINK10_get(NULL, hndl);
        break;
    case 11:
        rc = etime_field_TX_TS_ENA_LINK11_get(NULL, hndl);    
        break;
    default:
        PMC_RETURN(FALSE);
    }


    PMC_RETURN(rc);    
}


/*******************************************************************************
*  etime_block_non_default_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures Non-default register settings associated with the ETIME block in 
*   ENET subsystem.
*
*
* INPUTS:
*   *hndl         - pointer to ETIME handle instance
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
PUBLIC void etime_block_non_default_cfg(etime_handle_t *hndl)
{

    etime_buffer_t etime_buffer[1];

    PMC_ENTRY();

    etime_buffer_init(etime_buffer, hndl);

    /* Reset links */
    etime_field_REFL_LINK_RESET_0_set(etime_buffer, hndl, 1);
    etime_field_REFL_LINK_RESET_1_set(etime_buffer, hndl, 1);
    etime_field_REFL_LINK_RESET_2_set(etime_buffer, hndl, 1);
    etime_field_REFL_LINK_RESET_3_set(etime_buffer, hndl, 1);
    etime_field_REFL_LINK_RESET_4_set(etime_buffer, hndl, 1);
    etime_field_REFL_LINK_RESET_5_set(etime_buffer, hndl, 1);
    etime_field_REFL_LINK_RESET_6_set(etime_buffer, hndl, 1);
    etime_field_REFL_LINK_RESET_7_set(etime_buffer, hndl, 1);
    etime_field_REFL_LINK_RESET_8_set(etime_buffer, hndl, 1);
    etime_field_REFL_LINK_RESET_9_set(etime_buffer, hndl, 1);
    etime_field_REFL_LINK_RESET_10_set(etime_buffer, hndl, 1);
    etime_field_REFL_LINK_RESET_11_set(etime_buffer, hndl, 1);

    /* Put inactive value in Reflection path DPI calendar */
    etime_field_EXT_CPB_LINK_SCH_TS0_set(etime_buffer, hndl, 0xf);
    etime_field_EXT_CPB_LINK_SCH_TS1_set(etime_buffer, hndl, 0xf);
    etime_field_EXT_CPB_LINK_SCH_TS2_set(etime_buffer, hndl, 0xf);
    etime_field_EXT_CPB_LINK_SCH_TS3_set(etime_buffer, hndl, 0xf);
    etime_field_EXT_CPB_LINK_SCH_TS4_set(etime_buffer, hndl, 0xf);
    etime_field_EXT_CPB_LINK_SCH_TS5_set(etime_buffer, hndl, 0xf);
    etime_field_EXT_CPB_LINK_SCH_TS6_set(etime_buffer, hndl, 0xf);
    etime_field_EXT_CPB_LINK_SCH_TS7_set(etime_buffer, hndl, 0xf);
    etime_field_EXT_CPB_LINK_SCH_TS8_set(etime_buffer, hndl, 0xf);
    etime_field_EXT_CPB_LINK_SCH_TS9_set(etime_buffer, hndl, 0xf);
    etime_field_EXT_CPB_LINK_SCH_TS10_set(etime_buffer, hndl, 0xf);
    etime_field_EXT_CPB_LINK_SCH_TS11_set(etime_buffer, hndl, 0xf);


    /* Set default number of calendar entries in DPI calendar */
    etime_field_EXT_CPB_LINK_SCH_OFFSET_set(etime_buffer, hndl, ETIME_REFL_DFLT_OFFSET);
    etime_field_EXT_CPB_LINK_SCH_LAST_TIMESLOT_set(etime_buffer, hndl, ETIME_REFL_DFLT_LST_TS);
    etime_field_PMON_CNT_CLR_set(etime_buffer, hndl, 1);

    /* Update Reflection DPI scheduler */
    etime_field_EXT_CPB_LINK_SCH_UPD_set(etime_buffer, hndl, 1);

    etime_buffer_flush(etime_buffer);

    PMC_RETURN();
} /*etime_block_non_default_cfg*/

/*******************************************************************************
*  etime_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the ETIME block energy state. 
*
* INPUTS:
*   hndl           - handle to the ETIME instance to be operated on
*   energy_state   - energy_state
*
* OUTPUTS:
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PUBLIC void etime_energy_state_reg_set ( etime_handle_t *hndl, pmc_energy_state_t energy_state)
{
    PMC_ENTRY();    
    PMC_ASSERT(hndl!=NULL, ETIME_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            etime_field_RAM_LOWPOWER_REG_BIT_set(NULL, hndl, 0);
            break;
        case PMC_ENERGY_STATE_RESET:
            etime_field_RAM_LOWPOWER_REG_BIT_set(NULL, hndl, 1);
            break;
        default:
            PMC_ASSERT(FALSE, ETIME_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }

    PMC_RETURN();
    
}

/*
** End of file
*/
