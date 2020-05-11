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

#include "emac_loc.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
/* TX credit count default value for 100G  */
#define EMAC_TX_CREDIT_COUNT_MAX_100G_DEFAULT_VAL 0x20


/*EMAC PRBS CTXT INDICES*/
#define EMAC_ING_PRBS_CTXT_INDEX                 0
#define EMAC_EGR_PRBS_CTXT_INDEX                 1

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
    
PRIVATE PMC_ERROR emac_tx_ipg_default_value_cfg(emac_handle_t *hndl, 
                                                UINT32 link, 
                                                emac_data_rate_t data_rate);

PRIVATE PMC_ERROR emac_link_ING_DSI_CTRL_cfg(emac_handle_t *hndl, 
                                             UINT32 link,
                                             emac_data_rate_t data_rate,
                                             emac_rx_data_src_t value);

PRIVATE PMC_ERROR emac_link_EGR_DSI_CTRL_cfg(emac_handle_t *hndl, 
                                             UINT32 link,
                                             emac_data_rate_t data_rate,
                                             emac_tx_data_dst_t value);

PRIVATE PMC_ERROR emac_link_id_check(UINT32 link);

PRIVATE PMC_ERROR emac_link_rate_check(UINT32 link, 
                                       emac_data_rate_t data_rate) ;


PRIVATE PMC_ERROR util_patt_mode_to_emac_mode_conv(util_patt_mode_t    util_patt_mode,
                                                   emac_prbs_type_t   *emac_prbs_type);

PRIVATE PMC_ERROR emac_int_chnl_end_link_get(emac_handle_t  *emac_handle_t,
                                             BOOL8           is_multichannels,
                                             UINT32          start_link,
                                             UINT32         *end_link_ptr);

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
const char EMAC_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    EMAC_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char EMAC_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Public Functions
*/

/*******************************************************************************
* emac_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a EMAC block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the EMAC block relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - pointer to TSB name
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   emac_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC emac_handle_t *emac_ctxt_create(pmc_handle_t *parent, 
                                       UINT32 base_address, 
                                       pmc_sys_handle_t *sys_handle, 
                                       const char *tsb_name )
{
    emac_handle_t *emac_handle;
    UINT32 i;

    PMC_ENTRY();    

    emac_handle = (emac_handle_t*)PMC_CTXT_CALLOC(sizeof(emac_handle_t), parent);
    
    pmc_handle_init( parent, emac_handle, sys_handle, PMC_MID_DIGI_EMAC, tsb_name, base_address );

    /* Register log strings. */
    pmc_log_block_strings_register(EMAC_LOG_ERR_STRINGS[0], EMAC_LOG_ERR_TABLE_BASE, EMAC_LOG_ERR_COUNT);

    /* Create handle for child blocks */
    emac_handle->etime_handle = etime_ctxt_create((pmc_handle_t *)emac_handle, base_address+BASE_ADDR_EMAC_ETIME, sys_handle, "ETIME");
    PMC_CTXT_REGISTER_PTR(&emac_handle->etime_handle, parent);
    
    emac_handle->var.prbs_ctxt = (emac_prbs_ctxt_t **)PMC_CTXT_CALLOC(sizeof(emac_prbs_ctxt_t *) * EMAC_NUM_LINKS, parent);
    PMC_CTXT_REGISTER_PTR(&emac_handle->var.prbs_ctxt, parent);
    
    for (i = 0; i < EMAC_NUM_LINKS; i++)
    {
        emac_handle->var.prbs_ctxt[i] = (emac_prbs_ctxt_t *)PMC_CTXT_CALLOC(sizeof(emac_prbs_ctxt_t) * (UINT32)EMAC_DIR_EGR, parent); 
        PMC_CTXT_REGISTER_PTR(&emac_handle->var.prbs_ctxt[i], parent);
    }

    PMC_RETURN(emac_handle);
} /* emac_ctxt_create */  
 
/*******************************************************************************
* emac_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a EMAC block instance.
*
*
* INPUTS:
*   *emac_handle        - pointer to EMAC handle instance
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
PUBLIC void emac_ctxt_destroy(emac_handle_t *emac_handle)
{
    UINT32 i;

    PMC_ENTRY();
    etime_ctxt_destroy(emac_handle->etime_handle);
    
    for (i = 0; i < EMAC_NUM_LINKS; i++)
    {
        if (NULL != emac_handle->var.prbs_ctxt[i])
        {
            PMC_CTXT_FREE(&emac_handle->var.prbs_ctxt[i], emac_handle);
        }
    }

    PMC_CTXT_FREE(&emac_handle->var.prbs_ctxt, emac_handle);

    PMC_CTXT_FREE(&emac_handle, emac_handle);

    PMC_RETURN();
} /* emac_ctxt_destroy */

/*******************************************************************************
* emac_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a EMAC block handle instance.
*
*
* INPUTS:
*   *emac_handle        - pointer to EMAC handle instance
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
PUBLIC void emac_handle_init(emac_handle_t *emac_handle)
{
    UINT32 i;
    PMC_ENTRY();

    for (i =0; i< EMAC_NUM_LINKS; i++)
    {
        emac_handle->var.tx_enable[i] = FALSE;
        emac_handle->var.rx_enable[i] = FALSE;
        emac_handle->var.data_rate[i] = LAST_EMAC_DATA_RATE;
        emac_handle->var.prbs_ctxt[i][EMAC_ING_PRBS_CTXT_INDEX].enable = FALSE;
        emac_handle->var.prbs_ctxt[i][EMAC_EGR_PRBS_CTXT_INDEX].enable = FALSE;
    }

    etime_handle_init(emac_handle->etime_handle);
    emac_var_default_init(emac_handle);

    PMC_RETURN();
} /* emac_handle_init */

/*******************************************************************************
* emac_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on EMAC.
*
* INPUTS:
*   *emac_handle              - pointer to emac handle instance
*   *restart_init_cfg_ptr     - pointer to restart init config
*   top_energy_state_reg      - The expected energy state 
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
PUBLIC PMC_ERROR emac_handle_restart_init(emac_handle_t *emac_handle,
                                          util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                          pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(EMAC_LOG_ERR_STRINGS[0], EMAC_LOG_ERR_TABLE_BASE, EMAC_LOG_ERR_COUNT);
    }

    ret_val = etime_handle_restart_init(emac_handle->etime_handle, restart_init_cfg_ptr, top_energy_state_reg);

    PMC_RETURN(ret_val);

} /* emac_handle_restart_init */
/*******************************************************************************
* emac_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the EMAC instance to the starting state for all
*   dynamic context.  All dynamic context will be returned to the initialization 
*   values that were generate during the call to emac_handle_init().
*
*   This function is for use by parent modules that have provided all other
*   provisions for ensuring the state of all resources tracked by this
*   context has coherent register state.  
*
*
* INPUTS:
*   *hndl                 - pointer to EMAC handle instance
*   energy_state          - enum defining state to set the EMAC module to
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
PUBLIC PMC_ERROR emac_init(emac_handle_t *hndl, pmc_energy_state_t energy_state)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, EMAC_LOG_ERR_INVALID_PARAMETERS, 0, 0); 
    
    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        if (PMC_SUCCESS == ret_val && TRUE == emac_start_state_test(hndl))
        {
            /* Configure EMAC energy state set */
            emac_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RUNNING);
            /* Configures none default values */
            emac_block_non_default_cfg(hndl);
            /* Initialize child blocks */
            ret_val = etime_init(hndl->etime_handle, PMC_ENERGY_STATE_RUNNING);
            if (PMC_SUCCESS == ret_val)
            {
                emac_field_EMAC_ING_DPI_DROP_MASK_set(NULL, hndl, 160);
                emac_field_EMAC_ING_DPI_ERR_MASK_set(NULL, hndl, 255);
            }
        }               
    }
    else if (PMC_ENERGY_STATE_RESET == energy_state)
    {
        /* Configure EMAC energy state set */
        emac_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RESET);
        emac_var_default_init(hndl);
        /* Initialize child blocks */
        ret_val = etime_init(hndl->etime_handle, PMC_ENERGY_STATE_RESET);
    }else {
        PMC_ASSERT(FALSE, EMAC_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }
  
    PMC_RETURN(ret_val);

} /* emac_init */

/*******************************************************************************
* emac_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   EMAC instance.  
*
*
* INPUTS:
*   *hndl               - pointer to emac handle instance
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
PUBLIC BOOL8 emac_start_state_test(emac_handle_t *hndl)
{
    UINT32 itr;
    BOOL8 result = TRUE;
    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, EMAC_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    /* no local var_t in emac to test at this time */

    if(etime_start_state_test(hndl->etime_handle) == FALSE)
    {
        result = FALSE;
    }
    
    if (FALSE != result)
    {
        for (itr = 0; itr < EMAC_NUM_LINKS && FALSE != result; itr++)
        {
            if (hndl->var.tx_enable[itr] || hndl->var.rx_enable[itr])
            {
                result = FALSE;
                break;
            }
        }
    }

    PMC_RETURN(result);

}/* emac_start_state_test */

/*******************************************************************************
* emac_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of a EMAC block instance.
*
*
* INPUTS:
*   *emac_handle        - pointer to EMAC handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Base address of emac.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 emac_base_address_get(emac_handle_t *emac_handle)
{
    PMC_ENTRY();
    PMC_RETURN(emac_handle->base.base_address);
} /* emac_base_address_get */

/*******************************************************************************
* Initialization Configuration Functions
*******************************************************************************/
/*******************************************************************************
*  emac_mac_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure MAC segments according to data rate and specified parameters. 
*   Multiple segments are used to support the higher rates (40GE, 100GE):
*   1-12 x 10G : SEG0 - SEG11
*   1 x 100G : SEG0, 1-2 x 10G : SEG10-11
*   1-3 x 40G : SEG0, SEG4, SEG8
*   1-4 x 10G : SEG0-3, 1-2 x 40G : SEG4, SEG8
*   1-8 x 10G : SEG0-7, 1 x 40G : SEG8
*
* INPUTS:
*   hndl      - handle to the EMAC instance to be operated on
*   link      - link to configure
*   data_rate - data rate for the link
*   rx_data_src - data source for Rx MAC
*   tx_data_dst - data destination for Tx MAC
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_mac_cfg(emac_handle_t     *hndl, 
                              UINT32             link, 
                              emac_data_rate_t   data_rate,
                              emac_rx_data_src_t rx_data_src,
                              emac_tx_data_dst_t tx_data_dst)
{
    UINT32 start_bit;
    UINT32 stop_bit;
    emac_mac_buffer_t emac_mac_buffer[1]; 
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    /* Initialize buffer */
    emac_mac_buffer_init(emac_mac_buffer, hndl, link);

    /* check parameters value */
    result = emac_link_id_check(link);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }

    result = emac_link_rate_check(link, data_rate);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }    

    /* Calculate field offset and values for the link */
    start_bit = link*2;

    /* Make sure loopbacks are disabled */
    if(hndl->var.loopback[link] != EMAC_DISABLE_LOOPBACK)
    {
        PMC_ASSERT(FALSE, EMAC_LOG_ERR_LOOPBACK_ENABLED, 0, 0);
        emac_loopback_cfg(hndl,link,EMAC_DISABLE_LOOPBACK);
    }


    /* Take specified link out of low power */
    emac_link_lowpwr_cfg(hndl, link, data_rate, EMAC_DIR_BOTH, FALSE);

    /* Configure core segment */
    switch( data_rate ) 
    {
    case EMAC_LINK_100G: {
    
        /*In 100G, segments > 0 must be 0*/
        stop_bit = (start_bit + (EMAC_MODE_CTL_100G_NB_CFG_LINKS * 2)) - 1; 
        break;
    } 
    case EMAC_LINK_40G: {

        /*In 40G, segments > link/4 must be 0*/
        stop_bit = (start_bit + (EMAC_MODE_CTL_40G_NB_CFG_LINKS * 2)) - 1;
        break;
    } 
    default:
    {   
        /*In 10G, all 4 segmentEMAC_MODE_CTL_40G_STOP_BIT;s must be 0*/
        start_bit &= 0xFFFFFFF8; /* Truncating 2 LSBs to get link 0, 4 or 8*/
        stop_bit = (start_bit + (EMAC_MODE_CTL_10G_NB_CFG_LINKS * 2)) - 1;
    } /* end if */
    }

    emac_core_field_range_MODE_CTL_set(NULL, hndl, start_bit, stop_bit, data_rate);

    /* Configure PMA interface */
    result = emac_link_ING_DSI_CTRL_cfg(hndl, link, data_rate, rx_data_src);
    
    if (result == PMC_SUCCESS) 
    {
        result = emac_link_EGR_DSI_CTRL_cfg(hndl, link, data_rate, tx_data_dst);
    }

    /* check that DSI interfaces were configured properly */
    if (result != PMC_SUCCESS) 
    {
        PMC_RETURN(result);
    }

    /* Enable core segment */
    emac_core_field_range_SEG_CTL_set(NULL, hndl, link, link, TRUE);

    /* Resetting MAC layer */
    emac_link_reset_cfg(hndl, link, EMAC_DIR_BOTH, TRUE);

    /* Applying default settings */
    emac_max_frm_len_cfg(hndl, link, EMAC_FRM_LENGTH_DEFAULT);

    emac_mac_field_WAN_MODE_set(emac_mac_buffer, hndl, link, MAC_REG_CTRL_AND_CFG_BIT_WAN_MODE_DFLT);
    emac_mac_field_PROMIS_EN_set(emac_mac_buffer, hndl, link, MAC_REG_CTRL_AND_CFG_BIT_PROMIS_EN_DFLT);
    emac_mac_field_TX_ADDR_INS_set(emac_mac_buffer, hndl, link, MAC_REG_CTRL_AND_CFG_BIT_TX_ADDR_INS_DFLT);
    emac_mac_field_TX_PAD_EN_set(emac_mac_buffer, hndl, link, MAC_REG_CTRL_AND_CFG_BIT_CMD_FRAME_EN_DFLT);
    emac_mac_field_CMD_FRAME_EN_set(emac_mac_buffer, hndl, link, MAC_REG_CTRL_AND_CFG_BIT_CMD_FRAME_EN_DFLT);
    emac_mac_field_NO_LGTH_CHECK_set(emac_mac_buffer, hndl, link, MAC_REG_CTRL_AND_CFG_BIT_NO_LGTH_CHECK_DFLT);
    /* flush buffer to activate memory operations */
    emac_mac_buffer_flush(emac_mac_buffer);

    emac_flow_ctrl_mode_cfg(hndl, link, EMAC_DEFAULT_PAUSE_MODE, 
                            EMAC_DEFAULT_PAUSE_PFC_COMP, 
                            EMAC_DEFAULT_PAUSE_FWD,
                            EMAC_DEFAULT_PAUSE_IGNORE);

    /* Set 100G segment TX CREDIT COUNT */
    switch( data_rate ) 
    {
    case EMAC_LINK_100G: 
    {
        emac_field_EMAC_TX_CREDIT_COUNT_INIT_100G_set(NULL, hndl, EMAC_TX_CREDIT_COUNT_MAX_100G_DEFAULT_VAL);
        emac_field_EMAC_TX_CREDIT_COUNT_MAX_100G_set(NULL, hndl, EMAC_TX_CREDIT_COUNT_MAX_100G_DEFAULT_VAL);
        break;
    } 
    default:
        break;
    }

    /* Resetting PCS layer and credit counters */
    emac_pcs_field_RESET_set( NULL, hndl, link, TRUE );

    emac_mac_field_CREDIT_INIT_set( NULL, hndl, link, EMAC_RX_CREDIT_10G ); 
    emac_mac_field_CREDIT_RESET_set( NULL , hndl, link, TRUE);


    /* Set IPG default value */
    result = emac_tx_ipg_default_value_cfg(hndl, link, data_rate);

    if (PMC_SUCCESS != result) 
    {
        PMC_RETURN(result);
    }

    /* Resetting MAC layer */
    emac_mac_field_SW_RESET_set( NULL, hndl, link, TRUE );
    emac_field_range_EMAC_STAT_RAM_LOWPWR_set( NULL, hndl, link, link, 0);  

    /* store the data_rate & rx_data_src - needed if a loopback is enabled */
    hndl->var.data_rate[link] = data_rate;         
    hndl->var.rx_data_src[link] = rx_data_src;

    PMC_RETURN(result);

} /* emac_mac_cfg */

/*******************************************************************************
*  emac_mac_uncfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Unconfigure MAC segments of a link
*
* INPUTS:
*   hndl      - handle to the EMAC instance to be operated on
*   link      - link to configure
*   data_rate - data rate for the link
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_mac_uncfg(emac_handle_t *hndl, 
                                UINT32 link, 
                                emac_data_rate_t data_rate)
{
    PMC_ERROR result;


    PMC_ENTRY();

    /* check parameters value */
    result = emac_link_rate_check(link, data_rate);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }

    result = emac_link_rate_check(link, data_rate);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }  

    if (NULL != hndl->var.prbs_ctxt && 
        NULL != hndl->var.prbs_ctxt[link])
    {
        emac_prbs_field_range_PRBS31_ENA_set(NULL, hndl, link, link, FALSE);
        emac_prbs_field_range_PRBS9_ENA_set(NULL, hndl, link, link, FALSE);
        hndl->var.prbs_ctxt[link][EMAC_ING_PRBS_CTXT_INDEX].enable = FALSE;
        hndl->var.prbs_ctxt[link][EMAC_EGR_PRBS_CTXT_INDEX].enable = FALSE;
    }  

    /* Resetting the PCS layer */
    emac_pcs_field_RESET_set( NULL, hndl, link, TRUE );

    /* Resetting MAC layer */
    emac_mac_field_SW_RESET_set( NULL, hndl, link, TRUE );
    emac_mac_field_CREDIT_RESET_set( NULL, hndl, link, TRUE);

    /* disable core segment */
    emac_core_field_range_SEG_CTL_set(NULL, hndl, link, link, FALSE);

    /* Resetting link layer */
    emac_link_reset_cfg(hndl, link, EMAC_DIR_BOTH, TRUE);

    /* Take specified link to low power */
    emac_link_lowpwr_cfg(hndl, link, data_rate, EMAC_DIR_BOTH, TRUE);
 
    emac_field_range_EMAC_STAT_RAM_LOWPWR_set( NULL, hndl, link, link, 1);    
    
    /* Check if user forgot to disable loopbacks */
    if (hndl->var.loopback[link] != EMAC_DISABLE_LOOPBACK)
    {
        PMC_ASSERT(hndl!=NULL, EMAC_LOG_ERR_LOOPBACK_ENABLED, 0, 0);
        emac_loopback_cfg(hndl,link,EMAC_DISABLE_LOOPBACK); 
    }

    hndl->var.data_rate[link] = LAST_EMAC_DATA_RATE;         

    PMC_RETURN(PMC_SUCCESS);

} /* emac_mac_uncfg */


/*******************************************************************************
*  emac_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Unconfigure MAC segment of a link
*
* INPUTS:
*   hndl           - handle to the EMAC instance to be operated on
*   num_links      - link to configure
*   links          - indices of links to cleanup
*   ing_ts_en      - enable or disable ingress timestamp feature
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
PUBLIC PMC_ERROR emac_cleanup(emac_handle_t    *hndl,
                              UINT32            num_links, 
                              UINT32           *links,
                              BOOL8             ing_ts_en)
{
    UINT32 link_itr;
    UINT32 link;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    for (link_itr = 0; link_itr < num_links; link_itr++)
    {
        link = links[link_itr];

        emac_rx_enable(hndl,link,FALSE);
        emac_tx_enable(hndl,link,FALSE);

        /* Resetting the PCS layer */
        emac_pcs_field_RESET_set( NULL, hndl, link, TRUE );
        
        /* Resetting MAC layer */
        emac_mac_field_SW_RESET_set( NULL, hndl, link, TRUE );
        emac_mac_field_CREDIT_RESET_set( NULL, hndl, link, TRUE);
        
        /* disable core segment */
        emac_core_field_range_SEG_CTL_set(NULL, hndl, link, link, FALSE);
        
        /* Resetting link layer */
        emac_link_reset_cfg(hndl, link, EMAC_DIR_BOTH, TRUE);
        
        /* Take specified link to low power */
        (void) emac_link_lowpwr_cfg(hndl, link, EMAC_LINK_10G, EMAC_DIR_BOTH, TRUE);
  
        if (PMC_SUCCESS == result)
        {
            result = emac_ingr_data_sink_cfg( hndl, link,  EMAC_LINK_10G, FALSE);
        }

        if (0 == (link % 4))
        {
            emac_field_range_EMAC_LINK_40_100G_CLK_LOWPWR_set( NULL, hndl, link/EMAC_40G_NB_PMA_LANES, link/EMAC_40G_NB_PMA_LANES, TRUE);
        }

            
        emac_field_range_EMAC_STAT_RAM_LOWPWR_set(NULL, hndl, link, link, 1);    
        
        /* Check if user forgot to disable loopbacks */
        if (hndl->var.loopback[link] != EMAC_DISABLE_LOOPBACK)
        {
            PMC_ASSERT(hndl!=NULL, EMAC_LOG_ERR_LOOPBACK_ENABLED, 0, 0);
            emac_loopback_cfg(hndl,link,EMAC_DISABLE_LOOPBACK); 
        }

        /* also cleanup ETIME */        
        if (PMC_SUCCESS == result)
        {
            (void) etime_tx_ts_ena( hndl->etime_handle, link, FALSE);
        }

        /* cleanup PRBS */
        emac_prbs_field_range_PRBS31_ENA_set(NULL, hndl, link, link, FALSE);   
        emac_prbs_field_range_PRBS9_ENA_set(NULL, hndl, link, link, FALSE);
        hndl->var.tx_enable[link] = FALSE;
        hndl->var.rx_enable[link] = FALSE;
        hndl->var.data_rate[link] = LAST_EMAC_DATA_RATE;
        hndl->var.loopback[link] = EMAC_DISABLE_LOOPBACK;
    } 
    
    if (PMC_SUCCESS == result &&
        FALSE == ing_ts_en)
    {
        (void) etime_rx_ts_ena( hndl->etime_handle, FALSE);
    }
    PMC_RETURN(PMC_SUCCESS);

} /* emac_cleanup */

/*******************************************************************************
*  emac_src_dst_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure src and dst of DSI interface
*
* INPUTS:
*   hndl      - handle to the EMAC instance to be operated on
*   link      - link to configure
*   data_rate - data rate for the link
*   rx_data_src - data source for Rx MAC
*   tx_data_dst - data destination for Tx MAC
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_src_dst_cfg(emac_handle_t *hndl, 
                                  UINT32 link, 
                                  emac_data_rate_t data_rate,                               
                                  emac_rx_data_src_t rx_data_src,                              
                                  emac_tx_data_dst_t tx_data_dst)
{
    PMC_ERROR result;


    PMC_ENTRY();

    /* check parameters value */
    result = emac_link_rate_check(link, data_rate);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }

    result = emac_link_rate_check(link, data_rate);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }  

    
    result = emac_link_ING_DSI_CTRL_cfg(hndl, link, data_rate, rx_data_src);
    
    if (result == PMC_SUCCESS) 
    {
        result = emac_link_EGR_DSI_CTRL_cfg(hndl, link, data_rate, tx_data_dst);
    }
    
    PMC_RETURN(result);

} /* emac_src_dst_cfg */


/*******************************************************************************
*  emac_tx_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable/disable Tx path.
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*   enable   - TRUE: enable link
*              FALSE: disable link
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_tx_enable(emac_handle_t *hndl, 
                                UINT32 link, 
                                BOOL8 enable)
{
    PMC_ENTRY();

    /* Enable/disable specified link */
    emac_mac_field_TX_ENA_set(NULL, hndl, link, enable);

    if (enable) 
    {
        emac_field_range_EGR_XOFF_OVERRIDE_set(NULL, hndl, link, link, 0);
    } 
    else 
    {
        emac_field_range_EGR_XOFF_OVERRIDE_set(NULL, hndl, link, link, 1);
    }

      
    /* update context */    
    hndl->var.tx_enable[link] = enable;

    PMC_RETURN(PMC_SUCCESS);

} /* emac_tx_enable */

/*******************************************************************************
*  emac_tx_is_enabled
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Determin if the TX path is enabled.
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to determine configure
*
* OUTPUTS:
*
* RETURNS:
*   TRUE if the link is enabled. FALSE otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 emac_tx_is_enabled(emac_handle_t *hndl, 
                               UINT32 link)
{
    PMC_ENTRY();
    BOOL8 is_enabled = FALSE;

    is_enabled = emac_mac_field_TX_ENA_get(NULL, hndl, link);

    PMC_RETURN(is_enabled);

} /* emac_tx_is_enabled */

/*******************************************************************************
*  emac_rx_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable/disable Rx path.
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*   enable   - TRUE: enable link
*              FALSE: disable link
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_rx_enable(emac_handle_t *hndl, 
                                UINT32 link, 
                                BOOL8 enable)
{
    PMC_ENTRY();

    /* Enable/disable specified link */
    emac_mac_field_RX_ENA_set(NULL, hndl, link, enable);

    /* update context */
    hndl->var.rx_enable[link] = enable;

    PMC_RETURN(PMC_SUCCESS);

} /* emac_rx_enable */


/*******************************************************************************
* Run-time Operations
*******************************************************************************/
/*******************************************************************************
*  emac_link_lowpwr_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Low power configuration for specified link
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*   data_rate - data rate for the link to be put in / out of low power
*   data_direction - data direction on the link to be put in / out of low power
*   lowpwr   - TRUE: Put link in low power
*              FALSE: Take link out of low power
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESNS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_link_lowpwr_cfg(emac_handle_t *hndl, 
                                      UINT32 link, 
                                      emac_data_rate_t data_rate, 
                                      emac_data_dir_t data_direction, 
                                      BOOL8 lowpwr)
{
    UINT32 i;
    UINT32 last_index;
    PMC_ERROR result;

    PMC_ENTRY();

    /* check parameters value */
    result = emac_link_rate_check(link, data_rate);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }

    result = emac_link_rate_check(link, data_rate);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }  

    /* Configure low power for link */
    /* Register 0x14: EMAC120_TOP - EMAC PMA Interface Clock Low Power Control */
    last_index = link + EMAC_10G_NB_PMA_LANES;

    if (EMAC_LINK_100G == data_rate) 
    {
        last_index = link + EMAC_100G_NB_PMA_LANES;
    } 
    else if (EMAC_LINK_40G == data_rate) 
    {
        last_index = link + EMAC_40G_NB_PMA_LANES;
    };

    if (EMAC_DIR_EGR != data_direction)
    {
        for (i = link; i < last_index; i++) 
        {
            emac_field_range_EMAC_RX_PMA_CLK_LOWPWR_set( NULL, hndl, i, i, lowpwr );
        }
    }

    if (EMAC_DIR_ING != data_direction)
    {
        for (i = link; i < last_index; i++) 
        {
            emac_field_range_EMAC_TX_PMA_CLK_LOWPWR_set( NULL, hndl, i, i, lowpwr );
        }
    }

    if (data_direction == EMAC_DIR_BOTH)
    {
        if (EMAC_LINK_10G == data_rate) 
        {
            emac_field_range_EMAC_LINK_10G_CLK_LOWPWR_set( NULL, hndl, link, link, lowpwr );
        } 
        else 
        {
            emac_field_range_EMAC_LINK_40_100G_CLK_LOWPWR_set( NULL, hndl, link/EMAC_40G_NB_PMA_LANES, link/EMAC_40G_NB_PMA_LANES, lowpwr );
        } /* end if */
    }

    PMC_RETURN(PMC_SUCCESS);

} /* emac_link_lowpwr_cfg */

/*******************************************************************************
*  emac_link_lowpwr_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve low power configuration for specified link
*
* INPUTS:
*   hndl                 - handle to the EMAC instance to be operated on
*   link                 - link to configure
*   data_direction       - data direction on the link to be put in / out of low power
*   check_40G100G_lanes  - when TRUE, 40G and 100G lanes low power is returned when data rate matches 40G or 100G
*   data_rate            - data rate for the link to be put in / out of low power
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 emac_link_lowpwr_get(emac_handle_t    *hndl, 
                                  UINT32            link,
                                  emac_data_dir_t   data_direction, 
                                  BOOL8             check_40G100G_lanes,
                                  emac_data_rate_t  data_rate)
{
    UINT32 i;
    UINT32 last_index;
    BOOL8 rc = TRUE;

    PMC_ENTRY();
    
    
    /* Configure low power for link */
    /* Register 0x14: EMAC120_TOP - EMAC PMA Interface Clock Low Power Control */
    last_index = link + EMAC_10G_NB_PMA_LANES;
    if (TRUE == check_40G100G_lanes)
    {
        switch (data_rate)
        {
        case EMAC_LINK_40G:
            last_index = link + EMAC_40G_NB_PMA_LANES;
            break;
        case EMAC_LINK_100G:
            last_index = link + EMAC_100G_NB_PMA_LANES;
            break;
        default:
            break;
        }
    }

    
    if (EMAC_DIR_ING != data_direction)
    {
        for (i = link; i < last_index; i++) 
        {
            rc &= emac_field_range_EMAC_TX_PMA_CLK_LOWPWR_get( NULL, hndl, i, i);
        }
    }

    if (EMAC_DIR_EGR != data_direction)
    {
        for (i = link; i < last_index; i++) 
        {
            rc &= emac_field_range_EMAC_RX_PMA_CLK_LOWPWR_get( NULL, hndl, i, i);
        }
    }

    if (data_direction == EMAC_DIR_BOTH)
    {
        if (TRUE == check_40G100G_lanes && EMAC_LINK_10G != data_rate)
        {        
            if (0 == link ||
                (0 == (link % 4)))
            {
                rc &= emac_field_range_EMAC_LINK_40_100G_CLK_LOWPWR_get( NULL, hndl, link/EMAC_40G_NB_PMA_LANES, link/EMAC_40G_NB_PMA_LANES);
            } 
            else
            {
                rc = FALSE;
            }
        }
        else
        {
            rc &= emac_field_range_EMAC_LINK_10G_CLK_LOWPWR_get(NULL, hndl, link, link);
        }  
    }

    PMC_RETURN(rc);

} /* emac_link_lowpwr_get */

/*******************************************************************************
*  emac_link_reset_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reset configuration for specified link
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*   data_direction - data direction on the link to be put in / out of reset
*   reset    - TRUE: Put link in reset
*              FALSE: Take link out of reset
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_link_reset_cfg(emac_handle_t *hndl, 
                                     UINT32 link, 
                                     emac_data_dir_t data_direction, 
                                     BOOL8 reset)
{
    /* reset value is not used */
    UINT32 value;

    PMC_ENTRY();

    value = (1 << link);

    /* Configure reset for link */
    /* Register 0x4: EMAC120_TOP - AGB FIFO Link Reset */
    if (EMAC_DIR_EGR == data_direction || EMAC_DIR_BOTH == data_direction)
    {
        emac_field_EGR_LINK_RESET_set( NULL, hndl, value );
    };
    
    if (EMAC_DIR_ING == data_direction || EMAC_DIR_BOTH == data_direction)
    {
        emac_field_ING_LINK_RESET_set( NULL, hndl, value );
    };


    PMC_RETURN(PMC_SUCCESS);

} /* emac_link_reset_cfg */

/*******************************************************************************
*  emac_auto_lf_rf_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables automatic handling of local/remote faults.
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*   auto_fault_handling  - TRUE: Automatically insert remote faults/idles
*                         FALSE: Do not automatically insert remote faults/idles
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_auto_lf_rf_cfg(emac_handle_t *hndl, 
                                     UINT32 link, 
                                     BOOL8 auto_fault_handling)
{
    emac_mac_buffer_t emac_mac_buffer[1];

    PMC_ENTRY();

    emac_mac_buffer_init(emac_mac_buffer, hndl, link);

    emac_mac_field_FLT_HDL_DIS_set( emac_mac_buffer, hndl, link, auto_fault_handling == TRUE?0:1);
    
    emac_mac_buffer_flush(emac_mac_buffer);

    PMC_RETURN(PMC_SUCCESS);

} /* emac_auto_lf_rf_cfg */

/*******************************************************************************
*  emac_auto_lf_rf_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function get the  automatic handling of local/remote faults mode
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*
* OUTPUTS:
*   *auto_fault_handling  - TRUE: Automatically insert remote faults/idles
*                           FALSE: Do not automatically insert remote faults/idles
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_auto_lf_rf_cfg_get(emac_handle_t *hndl, 
                                         UINT32 link, 
                                         BOOL8 *auto_fault_handling)
{
    UINT32 value;
    PMC_ENTRY();

    value = emac_mac_field_FLT_HDL_DIS_get( NULL, hndl, link);
    
    *auto_fault_handling = (value == 1)?FALSE:TRUE;

    PMC_RETURN(PMC_SUCCESS);

} /* emac_auto_lf_rf_cfg_get */

/*******************************************************************************
*  emac_lf_rf_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure MAC to automatically insert remote faults and idles in the egress
*   direction on detection of local faults and remote fauls, respectively, on the ingress direction. 
*   Configure MAC to permanently send local fault or remote fault. If local or
*   remote faults are inserted, backpressure is enabled to avoid errors.
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*   auto_fault_handling  - TRUE: Automatically insert remote faults/idles
*                         FALSE: Do not automatically insert remote faults/idles
*   send_lf  - TRUE: Permanently send local fault sequences. auto_fault_handling must be set to FALSE.
*   send_rf  - TRUE: Permanently send remote fault sequences. auto_fault_handling must be set to FALSE.
*   halt_traffic_lf_rf  - TRUE: Enable XOFF to DCPB to stop traffic when LF or RF is set.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_lf_rf_cfg(emac_handle_t *hndl, 
                                UINT32 link, 
                                BOOL8 auto_fault_handling, 
                                BOOL8 send_lf, 
                                BOOL8 send_rf,
                                BOOL8 halt_traffic_lf_rf)
{
    emac_mac_buffer_t emac_mac_buffer[1];

    PMC_ENTRY();

    emac_mac_buffer_init(emac_mac_buffer, hndl, link);

    /* 
       Configure FLT_HDL_DIS, SEND_LF, SEND_RF bits in
       Register 0x8008 + 1024*A(A=0:11): MAC(A) - Control and Configuration 
    */
    if ( auto_fault_handling )
    {        
        /* clear fault insertions */
        /*                              b:    h:    A:    value: */
        emac_mac_field_SEND_LF_set    ( emac_mac_buffer, hndl, link, 0 );
        emac_mac_field_SEND_RF_set    ( emac_mac_buffer, hndl, link, 0 );
        
        /* revert to auto insert */
        emac_mac_field_FLT_HDL_DIS_set( emac_mac_buffer, hndl, link, 0 );
        emac_mac_buffer_flush(emac_mac_buffer);
    } 
    else 
    {
        if (send_lf ^ send_rf)
        {
            if (halt_traffic_lf_rf == TRUE)
            {
                emac_field_range_EGR_XOFF_OVERRIDE_set ( NULL, hndl, link, link, 1 );
            }
        }
        emac_mac_field_FLT_HDL_DIS_set( emac_mac_buffer, hndl, link, 1       );
        emac_mac_field_SEND_LF_set    ( emac_mac_buffer, hndl, link, send_lf );
        emac_mac_field_SEND_RF_set    ( emac_mac_buffer, hndl, link, send_rf );
        emac_mac_buffer_flush(emac_mac_buffer);

        if (send_lf ^ send_rf)
        {
            if (halt_traffic_lf_rf == TRUE)
            {
                emac_field_range_EGR_XOFF_OVERRIDE_set ( NULL, hndl, link, link, 0);
            }
        }      
    }

    

    PMC_RETURN(PMC_SUCCESS);

} /* emac_lf_rf_cfg */

/*******************************************************************************
*  emac_fault_mode_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure MAC fault insertion related control bits. Provides workaround
*   to ensure proper fault insertion signal on ETRANS path applications.\n\n
*
*   This API sets a link for Unidirectional Ethernet mode (IEEE 802.3 
*   clause 66.3.2.1).  To set this mode flt_hdl_dis, send_lf and send_rf
*   must all be set to TRUE.\n\n
*
*   It is the responsability of the calling API to ensure that the 
*   combination of settings do not generate an illegal state.
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*   flt_hdl_dis  - TRUE: Disable automatically insert remote faults/idles
*                  FALSE: Enable automatically insert remote faults/idles
*   send_lf  - TRUE: Permanently send local fault sequences. auto_fault_handling must be set to FALSE.
*   send_rf  - TRUE: Permanently send remote fault sequences. auto_fault_handling must be set to FALSE.
*   etrans_fault_on - TRUE:  Perform ETRANS path config when forcing LF/RF
*                     FALSE: Do not perform ETRANS path unique config.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_fault_mode_cfg(emac_handle_t *hndl, 
                                     UINT32 link, 
                                     BOOL8 flt_hdl_dis, 
                                     BOOL8 send_lf, 
                                     BOOL8 send_rf,
                                     BOOL8 etrans_fault_on)
{
    emac_mac_buffer_t emac_mac_buffer[1];

    PMC_ENTRY();

    emac_mac_buffer_init(emac_mac_buffer, hndl, link);

    if (etrans_fault_on)
        emac_field_range_EGR_XOFF_OVERRIDE_set ( NULL, hndl, link, link, 1 );


    emac_mac_field_SEND_LF_set    ( emac_mac_buffer, hndl, link, send_lf );
    emac_mac_field_SEND_RF_set    ( emac_mac_buffer, hndl, link, send_rf );
    emac_mac_field_FLT_HDL_DIS_set( emac_mac_buffer, hndl, link, flt_hdl_dis );

    emac_mac_buffer_flush(emac_mac_buffer);

    if(etrans_fault_on)
        emac_field_range_EGR_XOFF_OVERRIDE_set ( NULL, hndl, link, link, 0 );

    PMC_RETURN(PMC_SUCCESS);

} /* emac_fault_mode_cfg */


/*******************************************************************************
*  emac_fault_mode_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves configuration setting related to emac_fault_mode_cfg()
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*
* OUTPUTS:
*   flt_hdl_dis_ptr - pointer to FLT_HDL_DIS setting
*   send_lf_ptr     - pointer to SEND_LF setting
*   send_rf_ptr     - pointer to SEND_RF setting
*  
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_fault_mode_cfg_get(emac_handle_t *hndl, 
                                         UINT32 link, 
                                         BOOL8 *flt_hdl_dis_ptr, 
                                         BOOL8 *send_lf_ptr, 
                                         BOOL8 *send_rf_ptr)
{
    emac_mac_buffer_t emac_mac_buffer[1];

    PMC_ENTRY();

    emac_mac_buffer_init(emac_mac_buffer, hndl, link);

    *send_lf_ptr =     emac_mac_field_SEND_LF_get    ( emac_mac_buffer, hndl, link);
    *send_rf_ptr =     emac_mac_field_SEND_RF_get    ( emac_mac_buffer, hndl, link);
    *flt_hdl_dis_ptr = emac_mac_field_FLT_HDL_DIS_get( emac_mac_buffer, hndl, link);

    PMC_RETURN(PMC_SUCCESS);

} /* emac_fault_mode_cfg */

/*******************************************************************************
*  emac_idle_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure MAC to permanently send XL/CGMII Idle sequences even when faults
*   are received. 
*
* INPUTS:
*   hndl       - handle to the EMAC instance to be operated on
*   link       - link to configure
*   send_idle  - TRUE: Permanently sends XL/CGMII Idle sequences even  
*                when faults are received.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_idle_cfg(emac_handle_t *hndl, 
                                UINT32 link, 
                                BOOL8 send_idle )
{
    emac_mac_buffer_t emac_mac_buffer[1];

    PMC_ENTRY();

    emac_mac_buffer_init(emac_mac_buffer, hndl, link);

    /* 
       Configure SEND_IDLE bit in
       Register 0x8008 + 1024*A(A=0:11): MAC(A) - Control and Configuration 
    */
    emac_mac_field_SEND_IDLE_set    ( emac_mac_buffer, hndl, link, send_idle );

    emac_mac_buffer_flush(emac_mac_buffer);

    PMC_RETURN(PMC_SUCCESS);

} /* emac_idle_cfg */

/*******************************************************************************
*  emac_unidirectional_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the Unidirectional Ethernet mode (IEEE 802.3 
*   clause 66.3.2.1) for a provided 10G MAC terminated link.
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*
* OUTPUTS:
*   *enable                - TRUE: unidirectional mode enabled
*                            FALSE: unidirectional mode disabled
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_unidirectional_mode_get(emac_handle_t *hndl, 
                                              UINT32 link, 
                                              BOOL8 *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 auto_fault_handling;
    UINT32 send_lf;
    UINT32 send_rf;

    PMC_ENTRY();

    auto_fault_handling = emac_mac_field_FLT_HDL_DIS_get( NULL, hndl, link);
    send_lf = emac_mac_field_SEND_LF_get( NULL, hndl, link);
    send_rf = emac_mac_field_SEND_RF_get( NULL, hndl, link);

    if (auto_fault_handling == 0 && 
        send_lf == 0 && 
        send_rf == 0)
    {
        *enable = FALSE;
    } else if (auto_fault_handling == 1 && 
               send_lf == 1 && 
               send_rf == 1)
    {
        *enable = TRUE;
    } else 
    {
        *enable = FALSE;
        rc = EMAC_LOG_ERR_INVALID_PARAMETERS;
    }
    
    PMC_RETURN(rc);

} /* emac_unidirectional_mode_get */

/*******************************************************************************
*  emac_crc_fwd_is_enabled
* ______________________________________________________________________________
*
* DESCRIPTION:
*     Determine if the CRC forwarding is enabled.
*
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to determine the crc enabled status of.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE if the link is enabled. FALSE otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 emac_crc_fwd_is_enabled(emac_handle_t *hndl, 
                                    UINT32 link)
{
    PMC_ENTRY();

    BOOL8 is_enabled = FALSE;

    if (emac_mac_field_CRC_FWD_get(NULL, hndl, link) == 1) {

        is_enabled = TRUE;
    }

    PMC_RETURN(is_enabled);

} /* emac_crc_fwd_is_enabled */

/*******************************************************************************
*  emac_crc_fwd_en
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable/disable CRC forwarding in Rx
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*   enable   - TRUE: enable CRC forwarding
*              FALSE: disable CRC forwarding
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_crc_fwd_en(emac_handle_t *hndl, 
                                 UINT32 link, 
                                 BOOL8 enable)
{
    PMC_ENTRY();

    /* Enable/disable specified link */
    emac_mac_field_CRC_FWD_set(NULL, hndl, link, enable);

    PMC_RETURN(PMC_SUCCESS);

} /* emac_crc_fwd_en */

/*******************************************************************************
*  emac_no_length_check_en
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable/disable payload NO LENGTH CHECK
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*   enable   - TRUE: enable payload NO LENGTH CHECK
*              FALSE: disable payload NO LENGTH CHECK
*
* OUTPUTS:
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void emac_no_length_check_en(emac_handle_t *hndl, 
                                 UINT32 link, 
                                 BOOL8 enable)
{
    PMC_ENTRY();

    /* Enable/disable NO LENGTH CHECK */
    emac_mac_field_NO_LGTH_CHECK_set(NULL, hndl, link, enable);


    PMC_RETURN();

} /* emac_no_length_check_en */

/*******************************************************************************
*  emac_flow_ctrl_mode_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure Link Pause or PFC mode.
*
* INPUTS:
*   hndl            - handle to the EMAC instance to be operated on
*   link            - link to configure
*   pause_mode      - PFC or legacy Pause mode
*   pause_pfc_comp  - TRUE: Pause frames in legacy pause mode are processed 
*                           similar to PFC frames in PFC mode, 
*                           i.e. the transmit path is not paused by incoming Pause frames, 
*                           only the pause 
*                           status is asserted as long as the internal pause timer has not expired.
*                     FALSE: When a Pause frame is received, the quanta is 
*                            extracted and loaded into an 
*                            internal timer to pause the transmitter.
*   pause_fwd       - TRUE: Pause frames are forwarded on.
*                     FALSE: Pause frames are terminated and discarded.
*   pause_ignore    - TRUE: Received pause frames are ignored by the MAC.
*                     FALSE: The transmit process is stopped for the amount of
*                            time specified in the pause quanta 
*                             received within a pause frame.
*                     Only used if pause_mode == EMAC_LEGACY_PAUSE.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_flow_ctrl_mode_cfg(emac_handle_t *hndl, 
                                         UINT32 link, 
                                         emac_pause_mode_t pause_mode,
                                         BOOL8 pause_pfc_comp, 
                                         BOOL8 pause_fwd, 
                                         BOOL8 pause_ignore)
{
    emac_mac_buffer_t b[1];

    PMC_ENTRY();

    /* Initialize buffer */
    emac_mac_buffer_init( b, hndl, link);

    /* Configure pause mode */
    emac_mac_field_PFC_MODE_set(b, hndl, link, pause_mode);
    /* Configure pause forwarding */
    emac_mac_field_PAUSE_FWD_set(b, hndl, link, pause_fwd);

    /* Configure pause behaviour in Legacy Pause mode */
    if (EMAC_LEGACY_PAUSE == pause_mode) 
    {
        emac_mac_field_PAUSE_PFC_COMP_set(b, hndl, link, pause_pfc_comp);
        emac_mac_field_PAUSE_IGNORE_set(b, hndl, link, pause_ignore);
    } /* end if */

    emac_mac_buffer_flush( b );

    PMC_RETURN(PMC_SUCCESS);

} /* emac_flow_ctrl_mode_cfg */


/*******************************************************************************
*  emac_flow_ctrl_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get flow control configuration
*
* INPUTS:
*   hndl            - handle to the EMAC instance to be operated on
*   link            - link to configure
*
* OUTPUTS:
*   *enable       - FALSE: Received pause frames are ignored by the MAC.
*                    TRUE: The transmit process is stopped for the amount of
*                          time specified in the pause quanta 
*                          received within a pause frame.
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_flow_ctrl_mode_get(emac_handle_t *hndl, 
                                         UINT32         link, 
                                         BOOL8         *enable)
{
    UINT32 pause_ignore;

    PMC_ENTRY();
    
    pause_ignore = emac_mac_field_PAUSE_IGNORE_get(NULL, hndl, link);
    *enable = (pause_ignore == 1)?FALSE:TRUE;
                
    PMC_RETURN(PMC_SUCCESS);

} /* emac_flow_ctrl_mode_cfg */

/*******************************************************************************
*  emac_pause_frame_fwd_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure Link Pause.
*
* INPUTS:
*   hndl            - handle to the EMAC instance to be operated on
*   link            - link to configure
*   pause_fwd       - TRUE: Pause frames are forwarded on.
*                     FALSE: Pause frames are terminated and discarded.
*
* OUTPUTS:
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void emac_pause_frame_fwd_cfg(emac_handle_t *hndl, 
                                          UINT32 link, 
                                          BOOL8 pause_fwd)
{

    PMC_ENTRY();

    /* Configure pause forwarding */
    emac_mac_field_PAUSE_FWD_set(NULL, hndl, link, pause_fwd);


    PMC_RETURN();

} /* emac_pause_frame_fwd_cfg */

/*******************************************************************************
*  emac_pause_frame_fwd_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves Link Pause frame forwarding enable status.
*
* INPUTS:
*   hndl            - handle to the EMAC instance to be operated on
*   link            - link to configure
*
* OUTPUTS:
*  *pause_fwd_ptr   - TRUE: Pause frames are forwarded on.
*                     FALSE: Pause frames are terminated and discarded.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void emac_pause_frame_fwd_get(emac_handle_t *hndl, 
                                     UINT32         link, 
                                     BOOL8         *pause_fwd_ptr)
{

    PMC_ENTRY();

    /* Configure pause forwarding */
   *pause_fwd_ptr = emac_mac_field_PAUSE_FWD_get(NULL, hndl, link);

    PMC_RETURN();

} /* emac_pause_frame_fwd_get */

/*******************************************************************************
*  emac_pause_quanta_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure Pause Quanta value and threshold for the specified priority class.
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*   prio_class - PFC priority class to configure
*   quanta_val - PFC quanta value
*   quanta_thres - PFC quanta threshold
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_pause_quanta_cfg(emac_handle_t *hndl, 
                                       UINT32 link, 
                                       UINT32 prio_class, 
                                       UINT32 quanta_val, 
                                       UINT32 quanta_thres)
{
    PMC_ENTRY();

    /* Configure the pause quanta for the specified priority class */
    switch (prio_class) 
    {
    case 0:
        emac_mac_field_CL0_PAUSE_QUANTA_set ( NULL, hndl, link, quanta_val ); 
        emac_mac_field_CL0_PAUSE_QUANTA_THR_set ( NULL, hndl, link, quanta_thres ); 
        break;
    case 1:
        emac_mac_field_CL1_PAUSE_QUANTA_set ( NULL, hndl, link, quanta_val ); 
        emac_mac_field_CL1_PAUSE_QUANTA_THR_set ( NULL, hndl, link, quanta_thres ); 
        break;
    case 2:
        emac_mac_field_CL2_PAUSE_QUANTA_set ( NULL, hndl, link, quanta_val ); 
        emac_mac_field_CL2_PAUSE_QUANTA_THR_set ( NULL, hndl, link, quanta_thres ); 
        break;
    case 3:
        emac_mac_field_CL3_PAUSE_QUANTA_set ( NULL, hndl, link, quanta_val ); 
        emac_mac_field_CL3_PAUSE_QUANTA_THR_set ( NULL, hndl, link, quanta_thres ); 
        break;
    case 4:
        emac_mac_field_CL4_PAUSE_QUANTA_set ( NULL, hndl, link, quanta_val ); 
        emac_mac_field_CL4_PAUSE_QUANTA_THR_set ( NULL, hndl, link, quanta_thres ); 
        break;
    case 5:
        emac_mac_field_CL5_PAUSE_QUANTA_set ( NULL, hndl, link, quanta_val ); 
        emac_mac_field_CL5_PAUSE_QUANTA_THR_set ( NULL, hndl, link, quanta_thres ); 
        break;
    case 6:
        emac_mac_field_CL6_PAUSE_QUANTA_set ( NULL, hndl, link, quanta_val ); 
        emac_mac_field_CL6_PAUSE_QUANTA_THR_set ( NULL, hndl, link, quanta_thres ); 
        break;
    default:
        emac_mac_field_CL7_PAUSE_QUANTA_set ( NULL, hndl, link, quanta_val ); 
        emac_mac_field_CL7_PAUSE_QUANTA_THR_set ( NULL, hndl, link, quanta_thres ); 
        break;
    } /* end switch */

    PMC_RETURN(PMC_SUCCESS);

} /* emac_pause_quanta_cfg */

/*******************************************************************************
*  emac_pause_quanta_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get Pause Quanta value and threshold for the specified priority class.
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*   prio_class - PFC priority class to configure
*
* OUTPUTS:
*   *quanta_val - PFC quanta value
*   *quanta_thres - PFC quanta threshold
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_pause_quanta_get(emac_handle_t *hndl, 
                                       UINT32 link, 
                                       UINT32 prio_class, 
                                       UINT32 *quanta_val, 
                                       UINT32 *quanta_thres)
{
    PMC_ENTRY();

    /* Configure the pause quanta for the specified priority class */
    switch (prio_class) 
    {
    case 0:
        *quanta_val = emac_mac_field_CL0_PAUSE_QUANTA_get ( NULL, hndl, link); 
        *quanta_thres = emac_mac_field_CL0_PAUSE_QUANTA_THR_get ( NULL, hndl, link); 
        break;
    case 1:
        *quanta_val = emac_mac_field_CL1_PAUSE_QUANTA_get ( NULL, hndl, link ); 
        *quanta_thres = emac_mac_field_CL1_PAUSE_QUANTA_THR_get ( NULL, hndl, link); 
        break;
    case 2:
        *quanta_val = emac_mac_field_CL2_PAUSE_QUANTA_get ( NULL, hndl, link ); 
        *quanta_thres = emac_mac_field_CL2_PAUSE_QUANTA_THR_get ( NULL, hndl, link); 
        break;
    case 3:
        *quanta_val = emac_mac_field_CL3_PAUSE_QUANTA_get ( NULL, hndl, link ); 
        *quanta_thres = emac_mac_field_CL3_PAUSE_QUANTA_THR_get ( NULL, hndl, link); 
        break;
    case 4:
        *quanta_val = emac_mac_field_CL4_PAUSE_QUANTA_get ( NULL, hndl, link ); 
        *quanta_thres = emac_mac_field_CL4_PAUSE_QUANTA_THR_get ( NULL, hndl, link); 
        break;
    case 5:
        *quanta_val = emac_mac_field_CL5_PAUSE_QUANTA_get ( NULL, hndl, link ); 
        *quanta_thres = emac_mac_field_CL5_PAUSE_QUANTA_THR_get ( NULL, hndl, link); 
        break;
    case 6:
        *quanta_val = emac_mac_field_CL6_PAUSE_QUANTA_get ( NULL, hndl, link ); 
        *quanta_thres = emac_mac_field_CL6_PAUSE_QUANTA_THR_get ( NULL, hndl, link); 
        break;
    default:
        *quanta_val = emac_mac_field_CL7_PAUSE_QUANTA_get ( NULL, hndl, link ); 
        *quanta_thres = emac_mac_field_CL7_PAUSE_QUANTA_THR_get ( NULL, hndl, link); 
        break;
    } /* end switch */

    PMC_RETURN(PMC_SUCCESS);

} /* emac_pause_quanta_get */

/*******************************************************************************
*  emac_max_frm_len_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure maximum supported frame length.
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*   max_frm_length - Maximum supported frame length. Maximum supported is 16352 (0x3FE0) bytes. 
*                    Typical settings are 1518 for standard. The default is 1536 after reset.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_max_frm_len_cfg(emac_handle_t *hndl, 
                                      UINT32 link, 
                                      UINT32 max_frm_length)
{
    PMC_ENTRY();

    emac_mac_field_FRM_LENGTH_set(NULL, hndl, link, max_frm_length);

    PMC_RETURN(PMC_SUCCESS);

} /* emac_max_frm_len_cfg */

/*******************************************************************************
*  emac_tx_ipg_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the Transmit Inter-Packet Gap.
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*   tx_ipg_val - Tx Inter-Packet Gap
*                For 10G link:
*                    it is the number of octets in step of 4. Valid values are 8, 12, 16, ... 100. 
*                    A default of 12 must be set to conform to IEEE802.3ae. 
*                    Note: When set to 8, PCS layers may not be able to perform clock rate compensation.
*                    
*                For 40G and 100G:
*                    the TX_IPG_LENGTH bit field controls the transmit PPM compensation.
*                    When == 2'b00 : Normal mode of operation. Markers compensation active meaning effective +60ppm.
*                    When == 2'b01 : Default compensation plus 60ppm (total of +120ppm).
*                    When == 2'b10 : Default compensation plus 140ppm (total of +200ppm).
*                    When == 2'b11 : Default compensation plus 200ppm (total of +260ppm).
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS when operation is successful otherwise a descriptive error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_tx_ipg_cfg(emac_handle_t *hndl, 
                                 UINT32 link, 
                                 UINT32 tx_ipg_val)
{
    PMC_ENTRY();
    /*
      In 10G mode, the TX_IPG_LENGTH bit field controls transmit IPG value, 
      depending on the LAN or WAN mode (WAN_MODE field at MAC block register 
      Control and Configuration).
      
      LAN mode: number of octets in step of 'd4. Valid values are 'd8, 'd12, 
      'd16, ... 'd100. DIC is fully supported for any setting. A default of 
      'd12 must be set to conform to IEEE802.3ae. Note: When set to 'd8, the 
      PCS layer may not be able to perform clock rate compensation.
      
      WAN mode: Not supported. 
    */
      
    if (EMAC_NUM_LINKS <= link)
    {
        PMC_RETURN(EMAC_LOG_ERR_INVALID_LINK_ID);
    }
    switch (hndl->var.data_rate[link])
    {
    case EMAC_LINK_10G:        
        PMC_LOG_TRACE("About to check 10G link %d IPG = %d", link, tx_ipg_val);
        if (0 != (tx_ipg_val % 4) || 
            (8 > tx_ipg_val) ||
            (100 < tx_ipg_val))
        {
            PMC_RETURN(EMAC_LOG_ERR_INVALID_TX_IPG_LEN);
        }
        break;
    case EMAC_LINK_40G:
    case EMAC_LINK_100G:
        /* 
           In 40/100G mode, the TX_IPG_LENGTH bit field controls the transmit 
           PPM compensation.  
           When TX_IPG_LENGTH[6:5] = 2'b00 : Normal mode of operation. 
                                             Markers compensation active meaning 
                                             effective +60ppm.
           When TX_IPG_LENGTH[6:5] = 2'b01 : Default compensation plus 60ppm 
                                             (total of +120ppm).
           When TX_IPG_LENGTH[6:5] = 2'b10 : Default compensation plus 140ppm 
                                             (total of +200ppm).
           When TX_IPG_LENGTH[6:5] = 2'b11 : Default compensation plus 200ppm 
                                             (total of +260ppm).
           Note: TX_IPG_LENGTH[4:0] is ignored.
        */
        PMC_LOG_TRACE("About to check 40G or 100G link %d IPG: %d", link, tx_ipg_val);
        if (3 < tx_ipg_val)
        {
            PMC_RETURN(EMAC_LOG_ERR_INVALID_TX_IPG_LEN);
        }
        tx_ipg_val = tx_ipg_val << 5;
        break; 
    default:
        PMC_LOG_TRACE("EMAC link %d not provionned IPG: %d", link, tx_ipg_val);
        PMC_RETURN(EMAC_LOG_ERR_LINK_NOT_PROV);
        break;
    }

    emac_mac_field_TX_IPG_LENGTH_set(NULL, hndl, link, tx_ipg_val);

    PMC_RETURN(PMC_SUCCESS);
} /* emac_tx_ipg_cfg */

/*******************************************************************************
*  emac_ins_mac_addr_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set TX_ADDR_INS and Ethernet core MAC address configuration,
*   which controls the transmit MAC address generation.
*
* INPUTS:
*   hndl           - handle to the EMAC instance to be operated on
*   link           - link to configure
*   tx_addr_ins_en - The TX_ADDR_INS bit controls the transmit MAC address generation
*   mac_addr_msb   - MSB of MAC address
*   mac_addr_lsb   - LSB of MAC address
*
* OUTPUTS:
*
* RETURNS:
*   
*
* NOTES:
*
*******************************************************************************/
PUBLIC void emac_ins_mac_addr_set(emac_handle_t *hndl,
                                  UINT32 link,
                                  BOOL8 tx_addr_ins_en,
                                  UINT32 mac_addr_msb,
                                  UINT32 mac_addr_lsb)
{   
    PMC_ENTRY();

    PMC_ASSERT(link < EMAC_NUM_LINKS, EMAC_LOG_ERR_INVALID_LINK_ID, 0, 0);

    emac_mac_field_TX_ADDR_INS_set(NULL, hndl, link, tx_addr_ins_en);
    emac_mac_field_MAC_ADDR_LSB_set(NULL, hndl, link, mac_addr_lsb);
    emac_mac_field_MAC_ADDR_MSB_set(NULL, hndl, link, mac_addr_msb);

    PMC_RETURN();
} /* emac_ins_mac_addr_set */

/*******************************************************************************
*  emac_ins_mac_addr_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get TX_ADDR_INS and Ethernet core MAC address configuration,
*   which controls the transmit MAC address generation.
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*
* OUTPUTS:
*   *tx_addr_ins_en  - The TX_ADDR_INS bit controls the transmit MAC address generation
*   *mac_addr_msb    - MSB of MAC address
*   *mac_addr_lsb    - LSB of MAC address
*
* RETURNS:
*   
*
* NOTES:
*
*******************************************************************************/
PUBLIC void emac_ins_mac_addr_get(emac_handle_t *hndl,
                                  UINT32 link,
                                  BOOL8 *tx_addr_ins_en,
                                  UINT32 *mac_addr_msb,
                                  UINT32 *mac_addr_lsb)
{  
    PMC_ENTRY();

    PMC_ASSERT(link < EMAC_NUM_LINKS, EMAC_LOG_ERR_INVALID_LINK_ID, 0, 0);

    *tx_addr_ins_en = emac_mac_field_TX_ADDR_INS_get(NULL, hndl, link);
    *mac_addr_lsb = emac_mac_field_MAC_ADDR_LSB_get(NULL, hndl, link);
    *mac_addr_msb = emac_mac_field_MAC_ADDR_MSB_get(NULL, hndl, link);

    PMC_RETURN();
} /* emac_ins_mac_addr_get */

/*******************************************************************************
*  emac_pma_rx_to_tx_lpbk_en
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable/disable Rx to Tx loopback in the PMA.
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*   enable   - TRUE: Enable loopback.
*              FALSE: Disable loopback.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_pma_rx_to_tx_lpbk_en(emac_handle_t *hndl, 
                                           UINT32 link, 
                                           BOOL8 enable)
{
    PMC_ENTRY();

    /* Enable/disable loopback */

    PMC_RETURN(PMC_SUCCESS);

} /* emac_pma_rx_to_tx_lpbk_en */

/*******************************************************************************
*  emac_pma_tx_to_rx_lpbk_en
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable/disable Tx to Rx loopback in the PMA.
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*   enable   - TRUE: Enable loopback.
*              FALSE: Disable loopback.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_pma_tx_to_rx_lpbk_en(emac_handle_t *hndl, 
                                           UINT32 link, 
                                           BOOL8 enable)
{
    PMC_ENTRY();

    /* Enable/disable loopback */

    PMC_RETURN(PMC_SUCCESS);

} /* emac_pma_tx_to_rx_lpbk_en */

/*******************************************************************************
*  emac_pcs_lpbk_en
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable/disable loopback in the PCS.
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*   enable   - TRUE: Enable loopback.
*              FALSE: Disable loopback.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_pcs_lpbk_en(emac_handle_t *hndl, 
                                  UINT32 link, 
                                  BOOL8 enable)
{
    PMC_ENTRY();

    emac_pcs_field_LOOPBACK_set(NULL, hndl, link, enable);

    PMC_RETURN(PMC_SUCCESS);

} /* emac_pcs_lpbk_en */

/*******************************************************************************
*  emac_pcs_test_patt_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the test pattern generator/monitor in the PCS.
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*   square_wave - TRUE: Enable square wave pattern. FALSE: Enable pseudo-random pattern.
*                 Only valid for 10GBASE-R. Scrambled idle test pattern are generated for 100GBASE-R and 40GBASE-R.
*   zero_pattern - TRUE: Use zeroes data pattern. FALSE: Use LF data pattern.
*                 Only valid for 10GBASE-R. Scrambled idle test pattern are generated for 100GBASE-R and 40GBASE-R.
*   tx_en    - Enable/disable transmit test pattern.
*   rx_en    - Enable/disable receive test pattern.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*   The test pattern generator/checker conforms to sections 49.2.8, 49.2.12, and 52.9 in IEEE 802.3-2008.
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_pcs_test_patt_cfg(emac_handle_t *hndl, 
                                        UINT32 link, 
                                        BOOL8 square_wave, 
                                        BOOL8 zero_pattern,
                                        BOOL8 tx_en, 
                                        BOOL8 rx_en)
{
    emac_pcs_buffer_t emac_pcs_buffer[1];

    PMC_ENTRY();

    emac_pcs_buffer_init(emac_pcs_buffer, hndl, link);

    /* Configure test pattern generator/checker */
    emac_pcs_field_TEST_PATTERN_set(emac_pcs_buffer, hndl, link, square_wave);
    emac_pcs_field_DATA_PATTERN_set(emac_pcs_buffer, hndl, link, zero_pattern);
    emac_pcs_field_TX_TEST_EN_set(emac_pcs_buffer, hndl, link, tx_en);
    emac_pcs_field_RX_TEST_EN_set(emac_pcs_buffer, hndl, link, rx_en);

    emac_pcs_buffer_flush(emac_pcs_buffer);

    PMC_RETURN(PMC_SUCCESS);

} /* emac_pcs_test_patt_cfg */

/*******************************************************************************
*  emac_pcs_10g_test_patt_seed_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the test pattern seeds in the PCS for 10G Base-R.
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*   seed_sel - Seed A or B to configure
*   seed_msb - Upper 26-bits of the seed
*   seed_lsb - Lower 32-bits of the seed
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*   The test pattern generator/checker conforms to sections 49.2.8, 49.2.12, and 52.9 in IEEE 802.3-2008.
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_pcs_10g_test_patt_seed_cfg(emac_handle_t *hndl, 
                                                 UINT32 link,  
                                                 emac_pcs_seed_sel_t seed_sel, 
                                                 UINT32 seed_msb, 
                                                 UINT32 seed_lsb)
{
    PMC_ENTRY();

    if (EMAC_PCS_TEST_SEED_A == seed_sel) 
    {
        emac_pcs_field_TEST_SEED_A_57_48_set(NULL, hndl, link, (seed_msb & 0xffff0000) >> 16);
        emac_pcs_field_TEST_SEED_A_47_32_set(NULL, hndl, link, (seed_msb & 0xffff));
        emac_pcs_field_TEST_SEED_A_31_16_set(NULL, hndl, link, (seed_lsb & 0xffff0000) >> 16);
        emac_pcs_field_TEST_SEED_A_15_0_set(NULL, hndl, link, (seed_lsb & 0xffff));
    } 
    else 
    {
        emac_pcs_field_TEST_SEED_B_57_48_set(NULL, hndl, link, (seed_msb & 0xffff0000) >> 16);
        emac_pcs_field_TEST_SEED_B_47_32_set(NULL, hndl, link, (seed_msb & 0xffff));
        emac_pcs_field_TEST_SEED_B_31_16_set(NULL, hndl, link, (seed_lsb & 0xffff0000) >> 16);
        emac_pcs_field_TEST_SEED_B_15_0_set(NULL, hndl, link, (seed_lsb & 0xffff));
    } /* end if */

    PMC_RETURN(PMC_SUCCESS);

} /* emac_pcs_10g_test_patt_seed_cfg */

/*******************************************************************************
*  emac_mon_prbs_resync
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function forces EMAC PRBS monitor resynchronization.
*
* INPUTS:
*   hndl      - handle to the EMAC instance to be operated on
*   link      - link to configure
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_mon_prbs_resync(emac_handle_t   *hndl, 
                                      UINT32           link)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 err_cnt;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, EMAC_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(PMC_SUCCESS == emac_link_id_check(link), EMAC_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (FALSE == hndl->var.rx_enable[link])
    {
        rc = EMAC_LOG_ERR_LINK_NOT_PROV;
    }

    err_cnt = emac_prbs_reg_LANE_PRBS31_ERR_CNT_array_read(NULL, hndl, link);
    err_cnt = emac_prbs_reg_LANE_PRBS9_ERR_CNT_array_read(NULL, hndl, link);
    
    PMC_RETURN(rc);
}/* emac_mon_prbs_resync */

/*******************************************************************************
*  emac_prbs_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure PRBS pattern and enable/disable generation.
*
* INPUTS:
*   hndl            - handle to the EMAC instance to be operated on
*   link            - link to configure
*   data_direction  - datapath direction INGRESS or EGRESS.
*   prbs_type       - PRBS type
*   enable          - Enable/disable receive test pattern.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*   The test pattern generator/checker conforms to sections 49.2.8, 49.2.12, and 52.9 in IEEE 802.3-2008.
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_prbs_cfg(emac_handle_t   *hndl, 
                               UINT32           link,  
                               emac_data_dir_t  data_direction,
                               util_patt_mode_t prbs_type,
                               BOOL8            enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    emac_prbs_type_t emac_prbs_type;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != hndl, EMAC_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(PMC_SUCCESS == emac_link_id_check(link), EMAC_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    if (FALSE == hndl->var.rx_enable[link] && EMAC_DIR_ING == data_direction)
    {
        rc = EMAC_LOG_ERR_LINK_NOT_PROV;
    }

    if(FALSE == hndl->var.tx_enable[link]  && EMAC_DIR_EGR == data_direction)
    {
        rc = EMAC_LOG_ERR_LINK_NOT_PROV;
    }


    if (PMC_SUCCESS == rc && EMAC_DIR_ING == data_direction)
    {
        if (TRUE == hndl->var.prbs_ctxt[link][EMAC_ING_PRBS_CTXT_INDEX].enable &&
            TRUE == enable)
        {
            rc = EMAC_LOG_ERR_PRBS_ALREADY_ENABLED;
        }
        else if (FALSE == hndl->var.prbs_ctxt[link][EMAC_ING_PRBS_CTXT_INDEX].enable &&
                 FALSE == enable)
        {
            rc = EMAC_LOG_ERR_PRBS_NOT_ENABLED;
        }
        else
        {
            hndl->var.prbs_ctxt[link][EMAC_ING_PRBS_CTXT_INDEX].enable = enable;
        }
    }


    if (PMC_SUCCESS == rc && EMAC_DIR_EGR == data_direction)
    {
        rc = util_patt_mode_to_emac_mode_conv(prbs_type, &emac_prbs_type);

        if (PMC_SUCCESS == rc)
        {
            if (TRUE == hndl->var.prbs_ctxt[link][EMAC_EGR_PRBS_CTXT_INDEX].enable &&
                TRUE == enable)
            {                
                rc = EMAC_LOG_ERR_PRBS_ALREADY_ENABLED;
            }
            else if (FALSE == hndl->var.prbs_ctxt[link][EMAC_EGR_PRBS_CTXT_INDEX].enable&&
                     FALSE == enable)
            {
                rc = EMAC_LOG_ERR_PRBS_NOT_ENABLED;
            }
            else
            {
                /* Configure PRBS generator/checker */
                switch(emac_prbs_type)
                {
                case EMAC_PRBS31:
                    emac_prbs_field_range_PRBS31_ENA_set(NULL, hndl, link, link, enable);
                    break;

                case EMAC_PRBS9:
                    emac_prbs_field_range_PRBS9_ENA_set(NULL, hndl, link, link, enable);
                    break;

                default:
                    break;
                }
                
                hndl->var.prbs_ctxt[link][EMAC_EGR_PRBS_CTXT_INDEX].enable = enable;
            }
        }
    }

    PMC_RETURN(rc);
} /* emac_prbs_cfg */

/*******************************************************************************
*  emac_prbs_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve PRBS pattern and enable/disable generation.
*
* INPUTS:
*   hndl            - handle to the EMAC instance to be operated on
*   link            - link to configure
*   data_direction  - datapath direction INGRESS or EGRESS.
*
* OUTPUTS:
*   *prbs_type      - PRBS type
*   *enable         - Enable/disable receive test pattern.
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*   The test pattern generator/checker conforms to sections 49.2.8, 49.2.12, and 52.9 in IEEE 802.3-2008.
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_prbs_get(emac_handle_t   *hndl, 
                               UINT32           link,  
                               emac_data_dir_t  data_direction,
                               util_patt_mode_t *prbs_type,
                               BOOL8            *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 prbs_en = 0;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != hndl, EMAC_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != prbs_type, EMAC_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != enable, EMAC_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(PMC_SUCCESS == emac_link_id_check(link), EMAC_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    if (FALSE == hndl->var.rx_enable[link] && EMAC_DIR_ING == data_direction)
    {
        rc = EMAC_LOG_ERR_LINK_NOT_PROV;
    }

    if(FALSE == hndl->var.tx_enable[link]  && EMAC_DIR_EGR == data_direction)
    {
        rc = EMAC_LOG_ERR_LINK_NOT_PROV;
    }

    if (PMC_SUCCESS == rc && EMAC_DIR_ING == data_direction)
    {
        *prbs_type = LAST_UTIL_PATT_MODE;
        *enable = hndl->var.prbs_ctxt[link][EMAC_ING_PRBS_CTXT_INDEX].enable;
    }

    if (PMC_SUCCESS == rc && EMAC_DIR_EGR == data_direction)
    {
        *enable = hndl->var.prbs_ctxt[link][EMAC_EGR_PRBS_CTXT_INDEX].enable;
        if (*enable)
        {
            prbs_en = emac_prbs_field_range_PRBS31_ENA_get(NULL, hndl, link, link);

            if (0 != prbs_en)
            {
                *prbs_type = UTIL_PATT_MODE_PRBS_31; 
            }
            else
            {
                *prbs_type = UTIL_PATT_MODE_PRBS_9; 
            }
        }
        else
        {
            *prbs_type = LAST_UTIL_PATT_MODE;
        }
    }

    PMC_RETURN(rc);
} /* emac_prbs_get */

/*******************************************************************************
*  emac_prbs_error_cnt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves EMAC PRBS monitor status.
*
* INPUTS:
*   hndl      - handle to the EMAC instance to be operated on
*   link      - link to configure
*   prbs_type - PRBS type, only PRBS9 and PRBS31 are supported.
*
* OUTPUTS:
*   *err_cnt  - error counts
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*   The test pattern generator/checker conforms to sections 49.2.8, 49.2.12, and 52.9 in IEEE 802.3-2008.
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_prbs_error_cnt_get(emac_handle_t         *hndl, 
                                         UINT32                 link, 
                                         util_patt_mode_t       prbs_type,
                                         UINT32                *err_cnt)
{
    PMC_ERROR rc = PMC_SUCCESS;
    emac_prbs_type_t emac_prbs_type;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != hndl, EMAC_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(PMC_SUCCESS == emac_link_id_check(link), EMAC_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    if (FALSE == hndl->var.rx_enable[link])
    {
        rc = EMAC_LOG_ERR_LINK_NOT_PROV;
    }

    if (PMC_SUCCESS == rc && TRUE != hndl->var.prbs_ctxt[link][EMAC_ING_PRBS_CTXT_INDEX].enable)
    {
        rc = EMAC_LOG_ERR_PRBS_NOT_ENABLED;
    }

    if (PMC_SUCCESS == rc)
    {        
        rc = util_patt_mode_to_emac_mode_conv(prbs_type , &emac_prbs_type);
    }

    if (PMC_SUCCESS == rc)
    {        
        switch(emac_prbs_type)
        {
        case EMAC_PRBS31:
            *err_cnt = emac_prbs_reg_LANE_PRBS31_ERR_CNT_array_read(NULL, hndl, link);
            break;
        case EMAC_PRBS9:
            *err_cnt = emac_prbs_reg_LANE_PRBS9_ERR_CNT_array_read(NULL, hndl, link);
            break;
        default:
            rc = EMAC_LOG_ERR_PRBS_NOT_ENABLED;
            break;
        }
    }

    PMC_RETURN(rc);
} /* emac_prbs_err_cnt_get */

/*******************************************************************************
*  emac_prbs_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves EMAC PRBS monitor status.
*   This function should be called before emac_prbs_err_cnt_get() 
*   to determine if EMAC PRBS monitor is locked or not.
*
* INPUTS:
*   hndl      - handle to the EMAC instance to be operated on
*   link      - link to configure
*
* OUTPUTS:
*   *status    - PRBS monitor status
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*   The test pattern generator/checker conforms to sections 49.2.8, 49.2.12, and 52.9 in IEEE 802.3-2008.
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_prbs_status_get(emac_handle_t         *hndl, 
                                      UINT32                 link,
                                      util_patt_status_t    *status)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 tmp_stat;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != hndl, EMAC_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(PMC_SUCCESS == emac_link_id_check(link), EMAC_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    if (FALSE == hndl->var.rx_enable[link] && FALSE == hndl->var.tx_enable[link])
    {
        rc = EMAC_LOG_ERR_LINK_NOT_PROV;
    }        
    
    if (PMC_SUCCESS == rc)
    {
        /* Check that EMAC PRBS monitor is enabled */
        if (FALSE == hndl->var.prbs_ctxt[link][EMAC_ING_PRBS_CTXT_INDEX].enable)
        {
            *status = UTIL_PATT_STATUS_DISABLED;        
        }
        else
        {
            /* feature is not supported for REV A board */
            if (FALSE == pmc_is_digi_rev_a_revision(&hndl->base))
            {
                
                tmp_stat = emac_prbs_field_range_PRBS_ALL_ONES_get(NULL, hndl, link, link);
                if (0 != tmp_stat)
                {
                    *status = UTIL_PATT_STATUS_ONES_DET;
                }
                else
                {
                    *status = UTIL_PATT_STATUS_LOCKED;
                }
            }
            else
            {
                rc = EMAC_LOG_ERR_UNSUPPORTED_REV_A_FEATURE;
            }
        }
    }

    PMC_RETURN(rc);
} /* emac_prbs_cfg */

/*******************************************************************************
*  emac_pver_vl_intvl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure Alignment marker period (for PVER)
*
* INPUTS:
*   hndl      - handle to the EMAC instance to be operated on
*   enable    - Enable/disable 1023 64B/66B alignment marker interval mode
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*   This is for simulation purposes ONLY
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_pver_vl_intvl_cfg(emac_handle_t *hndl, 
                                        BOOL8 enable)
{
    PMC_ENTRY();

    emac_core_field_VL_INTVL_set(NULL, hndl, enable);

    PMC_RETURN(PMC_SUCCESS);
} /* emac_pver_vl_intvl_cfg */



/*******************************************************************************
*  emac_link_intr_enable_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the EMAC link interrupt enable
*
* INPUTS:
*   hndl           - handle to the EMAC instance to be operated on
*   link           - link to configure
*   data_rate      - data rate for the link
*   enable         - enable / disable the interrupt propagation
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_link_intr_enable_set(emac_handle_t    *hndl, 
                                           UINT32           link, 
                                           emac_data_rate_t data_rate, 
                                           BOOL8             enable)
{
    UINT32 value;
    UINT32 align_status_value;
    PMC_ERROR result;

    emac_buffer_t emac_buffer[1];


    PMC_ENTRY();

    /* Initialize buffer */
    emac_buffer_init(emac_buffer, hndl);

    /* check parameters value */
    result = emac_link_rate_check(link, data_rate);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }

    result = emac_link_rate_check(link, data_rate);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }  

    /* EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_LOC_FAULT_E_MSK does not have a per-link mask */
    /* EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_REM_FAULT_E_MSK does not have a per-link mask */

    /* following fields do not have a per-link mask that is why value is pre-processed */
    value = ((UINT32)enable << link);
    
    emac_field_RX_LOC_FAULT_E_set(emac_buffer, hndl, value);
    emac_field_RX_REM_FAULT_E_set(emac_buffer, hndl, value);    
    emac_field_PHY_LOS_E_set(emac_buffer, hndl, value);
    emac_field_HI_BER_E_set(emac_buffer, hndl, value);    
    emac_field_ING_AGB_OVERFLOW_E_set(emac_buffer, hndl, value);    
    emac_field_EGR_AGB_OVERFLOW_E_set(emac_buffer, hndl, value);    
    emac_field_EMAC_TX_OVERFLOW_E_set(emac_buffer, hndl, value);    

    /* Make sure to enable the 10x links BLOCK_LOCK in 100G, 4x links in 40G. */
    if (EMAC_LINK_100G == data_rate && EMAC_100G_LINK_0 == link) 
    {   
        value = ((0x3FF * (UINT32)enable) << link);
        align_status_value = ((UINT32)enable << (link / 4));
    } 
    else if (EMAC_LINK_40G  == data_rate) 
    {
        value = ((0xF * (UINT32)enable) << (link / 4));
        align_status_value = ((UINT32)enable << (link / 4));
    } 
    else 
    {   
        align_status_value = ((UINT32)enable << link);
    };

    emac_field_BLOCK_LOCK_E_set(emac_buffer, hndl, value);
    emac_field_ALIGN_STATUS_E_set(emac_buffer, hndl, align_status_value);
    

    emac_buffer_flush(emac_buffer);

    PMC_RETURN(PMC_SUCCESS);

} /* emac_link_intr_enable_set */

/*******************************************************************************
*  emac_egr_data_squelching_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the EMAC Tx Data Squelching
*
* INPUTS:
*   hndl           - handle to the EMAC instance to be operated on
*   link           - link to configure
*   data_rate      - data rate for the link
*   enable         - enable / disable
*   pattern        - 16-bit squelching pattern
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_egr_data_squelching_cfg (emac_handle_t    *hndl, 
                                               UINT32           link, 
                                               emac_data_rate_t data_rate, 
                                               BOOL8             enable,
                                               UINT32           pattern)
{  
    UINT32 i;
    PMC_ERROR result;

    emac_buffer_t emac_buffer[1];

    PMC_ENTRY();

    /* Initialize buffer */
    emac_buffer_init(emac_buffer, hndl);

    /* check parameters value */
    result = emac_link_rate_check(link, data_rate);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }

    result = emac_link_rate_check(link, data_rate);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }  

    switch (data_rate)
    {
    case EMAC_LINK_100G:
        for (i = link; i < (link + EMAC_100G_NB_PMA_LANES); i++) 
        {
            emac_field_range_EGR_DSI_OVR_EN_set (emac_buffer, hndl, i, i, enable);
        };
        break;

    case EMAC_LINK_40G:
        for (i = link; i < (link + EMAC_40G_NB_PMA_LANES); i++) 
        {
            emac_field_range_EGR_DSI_OVR_EN_set (emac_buffer, hndl, i, i, enable);
        };
        break;

    default:
        emac_field_range_EGR_DSI_OVR_EN_set (emac_buffer, hndl, link, link, enable);    
    };

    emac_field_EGR_DSI_OVR_PAT_set (emac_buffer, hndl, pattern);

    emac_buffer_flush(emac_buffer);

    PMC_RETURN(PMC_SUCCESS);

} /* emac_egr_data_squelching_cfg */

/*******************************************************************************
*  emac_ingr_data_sink_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the EMAC Rx Data Sink (when in EPMM configuration)
*
* INPUTS:
*   hndl           - handle to the EMAC instance to be operated on
*   link           - link to configure
*   data_rate      - data rate for the link
*   enable         - enable / disable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_ingr_data_sink_cfg (emac_handle_t    *hndl, 
                                          UINT32           link, 
                                          emac_data_rate_t data_rate, 
                                          BOOL8             enable)
{  
    PMC_ERROR result;


    PMC_ENTRY();

    /* check parameters value */
    result = emac_link_rate_check(link, data_rate);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }
    
    emac_field_range_ING_DATA_SINK_set (NULL, hndl, link, link, enable);

    PMC_RETURN(PMC_SUCCESS);

} /* emac_ingr_data_sink_cfg */

/*******************************************************************************
*  emac_ingr_data_sink_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve the EMAC Rx Data Sink (when in EPMM configuration)
*
* INPUTS:
*   hndl           - handle to the EMAC instance to be operated on
*   link           - link to configure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 emac_ingr_data_sink_get (emac_handle_t    *hndl, 
                                      UINT32           link)
{  
    UINT32 reg_val;

    PMC_ENTRY();
    
    reg_val = (BOOL8)emac_field_range_ING_DATA_SINK_get (NULL, hndl, link, link);

    PMC_RETURN(reg_val);

} /* emac_ingr_data_sink_cfg */

/*******************************************************************************
*  emac_ts_mask_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the EMAC Rx Data Sink (when in EPMM configuration)
*
* INPUTS:
*   hndl           - handle to the EMAC instance to be operated on
*   link           - link to configure
*   data_rate      - data rate for the link
*   mask_start     - 15-bit value
*   mask_end       - 15-bit value
*   enable         - enable / disable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_ts_mask_cfg (emac_handle_t    *hndl, 
                                   UINT32           link, 
                                   emac_data_rate_t data_rate,
                                   UINT32           mask_start,
                                   UINT32           mask_end,
                                   BOOL8             enable)
{
    UINT32 disable;
    PMC_ERROR result;

    emac_buffer_t emac_buffer[1];

    PMC_ENTRY();

    /* Initialize buffer */
    emac_buffer_init(emac_buffer, hndl);

    /* check parameters value */
    result = emac_link_rate_check(link, data_rate);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }

    result = emac_link_rate_check(link, data_rate);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }  


    emac_field_TS_MASK_START_set (emac_buffer, hndl, mask_start);
    emac_field_TS_MASK_END_set   (emac_buffer, hndl, mask_end);

    disable =  ~(UINT32)enable;
    
    if (EMAC_LINK_100G == data_rate) 
    {
        /* In 100G, make sure to disable the unused 40G links (4 & 8) */
        emac_field_range_TS_MASK_DISABLE_set (emac_buffer, hndl, 0, 0, disable);
        emac_field_range_TS_MASK_DISABLE_set (emac_buffer, hndl, 4, 4, 1);
        emac_field_range_TS_MASK_DISABLE_set (emac_buffer, hndl, 8, 8, 1);
    } 
    else if (EMAC_LINK_40G == data_rate) 
    {
        emac_field_range_TS_MASK_DISABLE_set (emac_buffer, hndl, (link / 4), (link / 4), disable);
    } 
    else 
    {
        /* In 10G, the TS_MASK is always disabled */
        emac_field_range_TS_MASK_DISABLE_set (emac_buffer, hndl, (link / 4), (link / 4), 1);
    }

    emac_buffer_flush(emac_buffer);

    PMC_RETURN(PMC_SUCCESS);

} /* emac_ts_mask_cfg */

/*******************************************************************************
*  emac_ingr_dpi_err_mask_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the EMAC Ingress Packet FCS and length error mask. The err_cfg
*   parameter can be one of the followig:
*       EMAC_DPI_ERR_FCS_ENABLE      - FCS error may contribute to DPI error/drop
*       EMAC_DPI_ERR_FCS_DISABLE     - FCS error will not contribute to DPI error/drop
*       EMAC_DPI_ERR_LEN_ENABLE      - Length error may contribute to DPI error/drop
*       EMAC_DPI_ERR_lEN_DISABLE     - Length error will not contribute to DPI error/drop
*
* INPUTS:
*   hndl                - handle to the EMAC instance to be operated on
*   link                - link to configure
*   data_rate           - data rate for the link
*   err_cfg             - emac dpi error mask configuration (see emac_dpi_err_cfg_t)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_ingr_dpi_err_mask_cfg(emac_handle_t    *hndl, 
                                            UINT32           link, 
                                            emac_data_rate_t data_rate,
                                            emac_dpi_err_cfg_t err_cfg)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    /* check parameters value */
    result = emac_link_rate_check(link, data_rate);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }  
    switch (err_cfg)   
    {
        case EMAC_INGR_DPI_ERR_FCS_ENABLE:
            emac_field_range_EMAC_ING_DPI_FCS_MASK_set(NULL, hndl, link, link, 0);
            break;
        case EMAC_INGR_DPI_ERR_FCS_DISABLE:
            emac_field_range_EMAC_ING_DPI_FCS_MASK_set(NULL, hndl, link, link, 1);
            break;
        case EMAC_INGR_DPI_ERR_LEN_ENABLE:
            emac_field_range_EMAC_ING_DPI_LEN_MASK_set(NULL, hndl, link, link, 0);
            break;
        case EMAC_INGR_DPI_ERR_LEN_DISABLE:
            emac_field_range_EMAC_ING_DPI_LEN_MASK_set(NULL, hndl, link, link, 1);
            break;
        default:
            PMC_ASSERT(TRUE, EMAC_LOG_ERR_INVALID_PARAMETERS,err_cfg,0);
    }
    PMC_RETURN(result);        
} /* emac_ingr_dpi_err_mask_cfg */


/*******************************************************************************
*  emac_ingr_dpi_err_mask_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Return the configured EMAC Ingress Packet FCS and length error mask. If 
*   fcs_mask_ptr is true, an fcs error on the link will assert an ingress 
*   DPI error or drop indication. If len_mask is true, a length error will 
*   assert a DPI error or drop indication.
*
* INPUTS:
*   hndl                - handle to the EMAC instance to be operated on
*   link                - link to configure
*   data_rate           - data rate for the link
*   fcs_mask_ptr        - If true, mask the fcs error from contributing to 
*                         DPI error/drop. 
*   len_mask_ptr        - If true, mask the legth error from contributing to 
*                         DPI error/drop. 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_ingr_dpi_err_mask_get(emac_handle_t    *hndl, 
                                            UINT32           link, 
                                            emac_data_rate_t data_rate,
                                            BOOL8            *fcs_mask_ptr,
                                            BOOL8            *len_mask_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 fcs_mask, len_mask;
    PMC_ENTRY();

    /* check parameters value */
    result = emac_link_rate_check(link, data_rate);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }  

    /* Retrieved the configures fcs and length masks */
    fcs_mask = emac_field_range_EMAC_ING_DPI_FCS_MASK_get(NULL, hndl, link, link);
    len_mask = emac_field_range_EMAC_ING_DPI_LEN_MASK_get(NULL, hndl, link, link);
    *fcs_mask_ptr = (fcs_mask == 1);
    *len_mask_ptr = (len_mask == 1);
    PMC_RETURN(PMC_SUCCESS);

} /* emac_ingr_dpi_err_mask_get */


/*******************************************************************************
*  emac_egr_mac_link_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reset the egress link
*
* INPUTS:
*   hndl           - handle to the EMAC instance to be operated on
*   link           - link to configure
*   data_rate      - data rate for the link
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_egr_mac_link_reset (emac_handle_t    *hndl, 
                                          UINT32           link, 
                                          emac_data_rate_t data_rate)
{
    PMC_ERROR result;

    PMC_ENTRY();


    /* check parameters value */
    result = emac_link_rate_check(link, data_rate);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }

    result = emac_link_rate_check(link, data_rate);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }  

    /* Stop the traffic on egress path */
    emac_field_range_EGR_XOFF_OVERRIDE_set(NULL, hndl, link, link, 1);

    /* Disable the Tx MAC */
    emac_mac_field_TX_ENA_set(NULL, hndl, link, FALSE);

    /* Apply an egress link reset */
    emac_field_range_EGR_LINK_RESET_set(NULL, hndl, link, link, 1);

    /* Apply a Tx MAC reset */
    emac_mac_field_SW_RESET_set(NULL, hndl, link, TRUE);

    /* Enable the Tx MAC */
    emac_mac_field_TX_ENA_set(NULL, hndl, link, TRUE);

    /* Resume the traffic on egress path */
    emac_field_range_EGR_XOFF_OVERRIDE_set(NULL, hndl, link, link, 0);

    
    PMC_RETURN(PMC_SUCCESS);

} /* emac_egr_mac_link_reset */

/*******************************************************************************
*  emac_etime_ts_lowpwr_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Force the ETIME TS block in low power mode
*
* INPUTS:
*   hndl           - handle to the EMAC instance to be operated on
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_etime_ts_lowpwr_set (emac_handle_t *hndl)
{
    PMC_ENTRY();

    emac_field_ETIME_TS_LOWPWR_set(NULL, hndl, 1);

    PMC_RETURN(PMC_SUCCESS);

} /* emac_etime_ts_lowpwr_set */

/*******************************************************************************
*  emac_sw_reset_core_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set the EMAC software reset
*
* INPUTS:
*   hndl           - handle to the EMAC instance to be operated on
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_sw_reset_core_set (emac_handle_t *hndl)
{
    PMC_ENTRY();

    emac_field_EMAC_CORE_SW_RESET_set(NULL, hndl, 1);
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* emac_sw_reset_core_set */

/*******************************************************************************
*  etime_sw_reset_core_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set the ETIME software reset
*
* INPUTS:
*   hndl           - handle to the EMAC instance to be operated on
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_sw_reset_core_set (emac_handle_t *hndl)
{
    PMC_ENTRY();


    PMC_RETURN(PMC_SUCCESS);

} /* etime_sw_reset_core_set */

/*******************************************************************************
*  emac_sw_reset_nregs_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set the EMAC software reset
*
* INPUTS:
*   hndl           - handle to the EMAC instance to be operated on
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_sw_reset_nregs_set (emac_handle_t *hndl)
{
    PMC_ENTRY();

    emac_field_EMAC_NREGS_SW_RESET_set(NULL, hndl, 1);
    
    PMC_RETURN(PMC_SUCCESS);

} /* emac_sw_reset_nregs_set */

/*******************************************************************************
*  etime_sw_reset_nregs_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set the ETIME software reset
*
* INPUTS:
*   hndl           - handle to the EMAC instance to be operated on
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_sw_reset_nregs_set (emac_handle_t *hndl)
{

    PMC_ENTRY();
    
    PMC_RETURN(PMC_SUCCESS);

    
} /* etime_sw_reset_nregs_set */


/*******************************************************************************
* FUNCTION: emac_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   emac_handle       - Pointer to subsystem handle
*   link              - Link
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
PUBLIC PMC_ERROR emac_int_validate(emac_handle_t    *emac_handle,
                                   UINT32            link,
                                   void             *non_null_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != emac_handle, EMAC_LOG_ERR_NULL_HANDLE, 0, 0);
   
    if (link >= EMAC_NUM_LINKS)
    {
        result = EMAC_LOG_ERR_INVALID_LINK_ID;
    }

    if (NULL == non_null_ptr)
    {
        result = EMAC_LOG_ERR_INVALID_PARAMETERS;
    }


    PMC_RETURN(result);
} /* emac_int_validate */


/*******************************************************************************
* FUNCTION: emac_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   emac_handle       - Pointer to subsystem handle
*   link              - Link
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*   is_multichannels  - TRUE if function is being called in multi channel case
*                       FALSE if function is being called for a real channel
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_int_chnl_enable(emac_handle_t    *emac_handle,
                                      UINT32            link,
                                      emac_int_chnl_t  *int_table_ptr,
                                      emac_int_chnl_t  *int_en_table_ptr,
                                      BOOL8             enable,
                                      BOOL8             is_multichannels)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = EMAC_NUM_LINKS;
    UINT32          link2 = EMAC_NUM_LINKS;
    UINT32          link1_end = EMAC_NUM_LINKS;


    PMC_ENTRY();

    result = emac_int_validate(emac_handle, link, int_en_table_ptr);

    if (PMC_SUCCESS == result)
    {
        link1 = link;
        link2 = link;

        /* Retrieve the last link of the ENET channel to be used for LOS interrupt */
        result = emac_int_chnl_end_link_get(emac_handle,
                                            is_multichannels,
                                            link1,
                                            &link1_end);
    }

    #ifdef EMAC_INT
    #undef EMAC_INT
    #endif
    #define EMAC_INT    UTIL_GLOBAL_INT_ENABLE
    
    #ifdef EMAC_INT_RANGE
    #undef EMAC_INT_RANGE
    #endif
    #define EMAC_INT_RANGE    UTIL_GLOBAL_INT_ENABLE_RANGE

    /* For LOS interrupts, need to set PHY_LOS_E bits for all of the links that the channel uses */
    #ifdef EMAC_INT_RANGE_LOS
    #undef EMAC_INT_RANGE_LOS
    #endif
    #define EMAC_INT_RANGE_LOS(handle, tsb_name, field_type, tsb_field, struct_field) \
    do {                                                                            \
        if ((NULL == int_table_ptr) ||                                              \
            (TRUE == int_table_ptr->struct_field)) {                                \
            tsb_name##_##field_type##_##tsb_field##_E_set(NULL, handle, link1, link1_end, (enable?((1<<(link1_end-link1+1)) -1):0)); \
            int_en_table_ptr->struct_field = enable;                                \
        }                                                                           \
    } while(0)

    #ifdef EMAC_INT_40G
    #undef EMAC_INT_40G
    #endif
    #define EMAC_INT_40G(handle, tsb_name, tsb_field, struct_field)             \
    do {                                                                        \
        if (TRUE == int_table_ptr->struct_field) {                              \
            tsb_name##_field_range_##tsb_field##_E_set(NULL, handle, link/EMAC_40G_NB_PMA_LANES, link/EMAC_40G_NB_PMA_LANES, enable); \
            int_en_table_ptr->struct_field = enable;                            \
        }                                                                       \
    } while(0)

    if (PMC_SUCCESS == result)
    {
        EMAC_INT_TABLE_DEFINE();
    }

    PMC_RETURN(result);
} /* emac_int_chnl_enable */




/*******************************************************************************
* FUNCTION: emac_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   emac_handle       - Pointer to subsystem handle
*   link              - Link
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   is_multichannels  - TRUE if function is being called in multi channel case
*                       FALSE if function is being called for a real channel
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_int_chnl_clear(emac_handle_t    *emac_handle,
                                     UINT32            link,
                                     emac_int_chnl_t  *int_table_ptr,
                                     emac_int_chnl_t  *int_en_table_ptr,
                                     BOOL8             is_multichannels)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = EMAC_NUM_LINKS;
    UINT32          link2 = EMAC_NUM_LINKS;
    UINT32          link1_end = EMAC_NUM_LINKS;


    PMC_ENTRY();

    result = emac_int_validate(emac_handle, link, int_en_table_ptr);

    if (PMC_SUCCESS == result)
    {
        link1 = link;
        link2 = link;

        /* Retrieve the last link of the ENET channel to be used for LOS interrupt */
        result = emac_int_chnl_end_link_get(emac_handle,
                                            is_multichannels,
                                            link1,
                                            &link1_end);
    }

    #ifdef EMAC_INT
    #undef EMAC_INT
    #endif
    #define EMAC_INT UTIL_GLOBAL_INT_CLEAR
    
    #ifdef EMAC_INT_RANGE
    #undef EMAC_INT_RANGE
    #endif
    #define EMAC_INT_RANGE UTIL_GLOBAL_INT_CLEAR_RANGE

    /* For LOS interrupts, need to clear PHY_LOS_I bits of all of the links that the channel uses */
    #ifdef EMAC_INT_RANGE_LOS
    #undef EMAC_INT_RANGE_LOS
    #endif
    #define EMAC_INT_RANGE_LOS(handle, tsb_name, field_type, tsb_field, struct_field) \
    do {                                                                            \
        if ((NULL == int_table_ptr) ||                                              \
            (TRUE == int_table_ptr->struct_field)) {                                \
            tsb_name##_##field_type##_##tsb_field##_I_set_to_clear(NULL, handle, link1, link1_end, ((1<<(link1_end-link1+1)) -1)); \
        }                                                                       \
    } while(0)

    #ifdef EMAC_INT_40G
    #undef EMAC_INT_40G
    #endif
    #define EMAC_INT_40G(handle, tsb_name, tsb_field, struct_field)             \
    do {                                                                        \
        if ((NULL == int_table_ptr) ||                                          \
            (TRUE == int_table_ptr->struct_field)) {                            \
            tsb_name##_field_range_##tsb_field##_I_set_to_clear(NULL, handle, link/EMAC_40G_NB_PMA_LANES, link/EMAC_40G_NB_PMA_LANES, TRUE); \
        }                                                                       \
    } while(0)

    if (PMC_SUCCESS == result)
    {
        EMAC_INT_TABLE_DEFINE();
    }

    PMC_RETURN(result);
} /* emac_int_chnl_clear */




/*******************************************************************************
* FUNCTION: emac_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   emac_handle       - Pointer to subsystem handle
*   link              - Link
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
PUBLIC PMC_ERROR emac_int_chnl_retrieve(emac_handle_t    *emac_handle,
                                        UINT32            link,
                                        emac_int_chnl_t  *filt_table_ptr,
                                        emac_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = EMAC_NUM_LINKS;
    UINT32          link2 = EMAC_NUM_LINKS;
    UINT32          link1_end = EMAC_NUM_LINKS;


    PMC_ENTRY();

    result = emac_int_validate(emac_handle, link, int_table_ptr);

    if (PMC_SUCCESS == result)
    {
        link1 = link;
        link2 = link;

        /* Retrieve the last link of the ENET channel to be used for LOS interrupt */
        result = emac_int_chnl_end_link_get(emac_handle,
                                            FALSE, /* emac_int_chnl_retrieve is never called for multichannels */
                                            link1,
                                            &link1_end);
    }
    
    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(emac_handle, emac, UTIL_GLOBAL_INT_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT(emac_handle, emac_pcs, UTIL_GLOBAL_INT_OPTIMIZE);

    #ifdef EMAC_INT
    #undef EMAC_INT
    #endif
    #define EMAC_INT UTIL_GLOBAL_INT_RETRIEVE
    
    #ifdef EMAC_INT_RANGE
    #undef EMAC_INT_RANGE
    #endif
    #define EMAC_INT_RANGE UTIL_GLOBAL_INT_RETRIEVE_RANGE
    
    /* For LOS interrupts, need to set returned boolean (emac_int_chnl_t.phy_los_i)
     * to TRUE if any of the channel's links has LOS interrupt state (PHY_LOS_I) set. */
    #ifdef EMAC_INT_RANGE_LOS
    #undef EMAC_INT_RANGE_LOS
    #endif
    #define EMAC_INT_RANGE_LOS(handle, tsb_name, field_type, tsb_field, struct_field) \
    do {                                                                            \
        if ((NULL == filt_table_ptr) ||                                             \
            (TRUE == filt_table_ptr->struct_field)) {                               \
            if (tsb_name##_##field_type##_##tsb_field##_I_get(tsb_name##_buffer_ptr, handle, link1, link1_end) != 0) int_table_ptr->struct_field = TRUE; \
            else int_table_ptr->struct_field = FALSE;                               \
        }                                                                           \
    } while(0)

    #ifdef EMAC_INT_40G
    #undef EMAC_INT_40G
    #endif
    #define EMAC_INT_40G(handle, tsb_name, tsb_field, struct_field)             \
    do {                                                                        \
        if ((NULL == filt_table_ptr) ||                                             \
            (TRUE == filt_table_ptr->struct_field)) {                               \
            int_table_ptr->struct_field = tsb_name##_field_range_##tsb_field##_I_get(tsb_name##_buffer_ptr, handle, link/EMAC_40G_NB_PMA_LANES, link/EMAC_40G_NB_PMA_LANES); \
        }                                                                           \
    } while(0)

    if (PMC_SUCCESS == result)
    {
        EMAC_INT_TABLE_DEFINE();
    }
    
    #ifdef EMAC_STATUS
    #undef EMAC_STATUS
    #endif
    #define EMAC_STATUS UTIL_GLOBAL_STATUS_RETRIEVE
    
    #ifdef EMAC_STATUS_RANGE
    #undef EMAC_STATUS_RANGE
    #endif
    #define EMAC_STATUS_RANGE UTIL_GLOBAL_STATUS_RETRIEVE_RANGE
    
    /* For LOS interrupts, need to set returned boolean (emac_int_chnl_t.phy_los_v)
     * to TRUE if any of the channel's links has interrupt value (PHY_LOS_V) set. */
    #ifdef EMAC_STATUS_RANGE_LOS
    #undef EMAC_STATUS_RANGE_LOS
    #endif
    #define EMAC_STATUS_RANGE_LOS(handle, tsb_name, field_type, tsb_field, struct_field) \
    do {                                                                            \
        if ((NULL == filt_table_ptr) ||                                             \
            (TRUE == filt_table_ptr->struct_field)) {                               \
            if (tsb_name##_##field_type##_##tsb_field##_get(tsb_name##_buffer_ptr, handle, link1, link1_end) != 0) int_table_ptr->struct_field = TRUE; \
            else int_table_ptr->struct_field = FALSE;                               \
        }                                                                           \
    } while(0)

    #ifdef EMAC_STATUS_40G
    #undef EMAC_STATUS_40G
    #endif
    #define EMAC_STATUS_40G(handle, tsb_name, tsb_field, struct_field)          \
    do {                                                                        \
        int_table_ptr->struct_field = tsb_name##_field_range_##tsb_field##_get(NULL, handle, link/EMAC_40G_NB_PMA_LANES, link/EMAC_40G_NB_PMA_LANES); \
    } while(0)

    if (PMC_SUCCESS == result)
    {
        EMAC_STATUS_TABLE_DEFINE();

        UTIL_GLOBAL_INT_BUFFER_FLUSH(emac_handle, emac);
        UTIL_GLOBAL_INT_BUFFER_FLUSH(emac_handle, emac_pcs);
    }

    PMC_RETURN(result);
} /* emac_int_chnl_retrieve */



/*******************************************************************************
* FUNCTION: emac_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   emac_handle       - Pointer to subsystem handle
*   link              - Link
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   is_multichannels  - TRUE if function is being called in multi channel case
*                       FALSE if function is being called for a real channel
*
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
PUBLIC PMC_ERROR emac_int_chnl_enabled_check(emac_handle_t     *emac_handle,
                                             UINT32             link,
                                             emac_int_chnl_t   *int_en_table_ptr,
                                             BOOL8             *int_found_ptr,
                                             BOOL8              is_multichannels)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = EMAC_NUM_LINKS;
    UINT32          link2 = EMAC_NUM_LINKS;
    UINT32          link1_end = EMAC_NUM_LINKS;


    PMC_ENTRY();

    result = emac_int_validate(emac_handle, link, int_found_ptr);

    if (PMC_SUCCESS == result)
    {
        link1 = link;
        link2 = link;

        /* Retrieve the last link of the ENET channel to be used for LOS interrupt */
        result = emac_int_chnl_end_link_get(emac_handle,
                                            is_multichannels,
                                            link1,
                                            &link1_end);
    }

    #ifdef EMAC_INT
    #undef EMAC_INT
    #endif
    #define EMAC_INT UTIL_GLOBAL_INT_CHECK
    
    #ifdef EMAC_INT_RANGE
    #undef EMAC_INT_RANGE
    #endif
    #define EMAC_INT_RANGE UTIL_GLOBAL_INT_CHECK_RANGE
    
    /* For LOS interrupts, need to set returned boolean (*int_found_ptr)
     * to TRUE if any of the channel's links has LOS interrupt state (PHY_LOS_I) set. */
    #ifdef EMAC_INT_RANGE_LOS
    #undef EMAC_INT_RANGE_LOS
    #endif
    #define EMAC_INT_RANGE_LOS(handle, tsb_name, field_type, tsb_field, struct_field) \
    do {                                                                            \
        if ((FALSE == *int_found_ptr) &&                                            \
            (TRUE == int_en_table_ptr->struct_field)) {                             \
            if (tsb_name##_##field_type##_##tsb_field##_I_get(NULL, handle, link1, link1_end) != 0) *int_found_ptr = TRUE; \
        }                                                                           \
    } while(0)

    #ifdef EMAC_INT_40G
    #undef EMAC_INT_40G
    #endif
    #define EMAC_INT_40G(handle, tsb_name, tsb_field, struct_field)             \
    do {                                                                        \
        if ((FALSE == *int_found_ptr) &&                                        \
            (TRUE == int_en_table_ptr->struct_field)) {                         \
            *int_found_ptr = tsb_name##_field_range_##tsb_field##_I_get(NULL, handle, link/EMAC_40G_NB_PMA_LANES, link/EMAC_40G_NB_PMA_LANES); \
        }                                                                       \
    } while(0)

    if (PMC_SUCCESS == result)
    {
        EMAC_INT_TABLE_DEFINE();
    }

    PMC_RETURN(result);
} /* emac_int_chnl_enabled_check */


/*******************************************************************************
* emac_errored_frames_drop_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable or disable the drop of EMAC ingress errored frames for a given type 
*   of error (only for FCS and packet length error).
*   This configuration is applicable to all links.
*
*
* INPUTS:
*   hndl             - handle to the ENET instance to be operated on.
*   error_type       - type of error: FCS, packet length, LF, RF and vlan's. 
*   enable           - when TRUE EMAC is configured to drop FCS errored frames.
*                      when FALSE EMAC is configured to forward FCS errored frames.
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
PUBLIC void emac_errored_frames_drop_cfg(emac_handle_t       *hndl,
                                         emac_error_t         error_type,
                                         BOOL8                enable)
{
    UINT8     drop_msk;
    UINT32    bit_idx;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, EMAC_LOG_ERR_NULL_HANDLE, 0, 0);

    /* read FCS_MSK and DROP_MSK current value */
    drop_msk = emac_field_EMAC_ING_DPI_DROP_MASK_get(NULL, hndl);
    bit_idx = (UINT8) error_type;
    if (TRUE == enable)
    { 
        drop_msk = drop_msk & ((~(1 << bit_idx)) & 0xff);
    }
    else
    {
        drop_msk = drop_msk | (1 << bit_idx);
    }        


    /* update drop msk register only when if new value different to previous value */
    emac_field_EMAC_ING_DPI_DROP_MASK_set(NULL, hndl, drop_msk);
    PMC_LOG_TRACE("EMAC_ING_DPI_DROP_MASK err_type = %d -> drop_msk = %x\n", (UINT8)error_type, drop_msk);
    

    PMC_RETURN();
} /*emac_errored_frames_drop_cfg*/

/*******************************************************************************
* emac_errored_frames_drop_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Check if EMAC ingress errored frames for a given type of error will be 
*   dropped.
*
*
* INPUTS:
*   hndl             - handle to the EMAC instance to be operated on.
*   error_type       - type of error: FCS, packet length, LF, RF and vlan's. 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE if error_type enabled, FALSE if not.
*
* NOTES:
*
*******************************************************************************/  
PUBLIC BOOL8 emac_errored_frames_drop_get(emac_handle_t *hndl,
                                          emac_error_t  error_type)
{
    UINT32 drop_msk_exp,drop_msk;
    UINT8 bit_idx;
    BOOL8 enabled;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != hndl, EMAC_LOG_ERR_NULL_HANDLE, 0, 0);

    /* check if error_type is enabled */
    drop_msk = emac_field_EMAC_ING_DPI_DROP_MASK_get(NULL, hndl);
    bit_idx = (UINT8) error_type;
    drop_msk_exp = (1 << bit_idx);
    enabled = ((drop_msk & drop_msk_exp) == 0);

    PMC_RETURN(enabled);
} /*emac_errored_frames_drop_get*/

/*******************************************************************************
* emac_errored_frames_dpi_error_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure whether a particular type of packet error will cause a DPI error
*   to be asserted. This configuration is applicable to all links.
*
* INPUTS:
*   hndl             - handle to the EMAC instance to be operated on.
*   error_type       - type of error: FCS, packet length, LF, RF and vlan's. 
*   enable           - If true, error_type will result in a DPI error.\n
*                      If false, error_type will result in a DPI error.
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
PUBLIC void emac_errored_frames_dpi_error_cfg(emac_handle_t *hndl,
                                              emac_error_t error_type,
                                              BOOL8 enable)
{
    UINT8     err_msk;
    UINT32    bit_idx;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, EMAC_LOG_ERR_NULL_HANDLE, 0, 0);

    /* read ERR_MSK current value */
    err_msk = emac_field_EMAC_ING_DPI_ERR_MASK_get(NULL, hndl);
    bit_idx = (UINT8) error_type;
    if (TRUE == enable)
    { 
        err_msk = err_msk & ((~(1 << bit_idx)) & 0xff);
    }
    else
    {
        err_msk = err_msk | (1 << bit_idx);
    }        

    /* update drop msk register only when if new value different to previous value */
    emac_field_EMAC_ING_DPI_ERR_MASK_set(NULL, hndl, err_msk);
    PMC_LOG_TRACE("EMAC_ING_DPI_ERR_MASK err_type = %d -> err_msk = %x\n", (UINT8)error_type, err_msk);
    

    PMC_RETURN();
} /* emac_errored_frames_dpi_error_cfg */

/*******************************************************************************
* emac_errored_frames_dpi_error_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Check if packets errors of a particular type will cause a DPI error 
*   to be asserted. 
*
* INPUTS:
*   hndl             - handle to the EMAC instance to be operated on.
*   error_type       - type of error: FCS, packet length, LF, RF and vlan's. 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE if error_type enabled, FALSE if not.
*
* NOTES:
*
*******************************************************************************/  
BOOL8 emac_errored_frames_dpi_error_get(emac_handle_t *hndl,
                                        emac_error_t error_type)
{
    UINT8     err_msk, err_msk_exp;
    UINT32    bit_idx;
    BOOL8 enabled;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, EMAC_LOG_ERR_NULL_HANDLE, 0, 0);

    /* read ERR_MSK current value */
    err_msk = emac_field_EMAC_ING_DPI_ERR_MASK_get(NULL, hndl);
    bit_idx = (UINT8) error_type;
    err_msk_exp = (1 << bit_idx);
     
    enabled = ((err_msk & err_msk_exp) == 0);
    PMC_RETURN(enabled);
} /* emac_errored_frames_dpi_error_get */

/*
** Private Functions
*/
   
/*******************************************************************************
* emac_link_ING_DSI_CTRL_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   enable or disable ingress dsi controller for a specific link
*
*
* INPUTS:
*   hndl             - handle to the EMAC instance to be operated on
*   link             - link to configure
*   data_rate        - data rate for the link
*   value            - configure switch: TRUE: enable, FALSE: disable 
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
PRIVATE PMC_ERROR emac_link_ING_DSI_CTRL_cfg(emac_handle_t *hndl, 
                                             UINT32 link,
                                             emac_data_rate_t data_rate,
                                             emac_rx_data_src_t value)
{

    UINT32 nb_dsi_links;
    UINT32 i;
    emac_buffer_t emac_buffer[1];

    PMC_ENTRY();


    /* Configure core segment */
    switch( data_rate ) 
    {
    case EMAC_LINK_100G: {
    
        nb_dsi_links = EMAC_100G_NB_PMA_LANES;
        break;
    } 
    case EMAC_LINK_40G: {

        nb_dsi_links = EMAC_40G_NB_PMA_LANES;
        break;
    } 
    default:
    {   
        nb_dsi_links = EMAC_10G_NB_PMA_LANES;
    } /* end if */
    }

    /* initialize EMAC buffer used to optimize read/modify access */
    emac_buffer_init(emac_buffer, hndl);

    for(i = 0;  i < nb_dsi_links; i++){
    
        /* call appropriate emac_field_ING_DSI_CTRL_X function depending on link id */
        switch (link + i) 
        {
        case 0:
        {
            emac_field_ING_DSI_CTRL_0_set(emac_buffer, hndl, value);
            break;
        }
        case 1:
        {
            emac_field_ING_DSI_CTRL_1_set(emac_buffer, hndl, value);
            break;
        }
        case 2:
        {
            emac_field_ING_DSI_CTRL_2_set(emac_buffer, hndl, value);
            break;
        }
        case 3:
        {
            emac_field_ING_DSI_CTRL_3_set(emac_buffer, hndl, value);
            break;
        }
        case 4:
        {
            emac_field_ING_DSI_CTRL_4_set(emac_buffer, hndl, value);
            break;
        }
        case 5:
        {
            emac_field_ING_DSI_CTRL_5_set(emac_buffer, hndl, value);
            break;
        }
        case 6:
        {
            emac_field_ING_DSI_CTRL_6_set(emac_buffer, hndl, value);
            break;
        }
        case 7:
        {
            emac_field_ING_DSI_CTRL_7_set(emac_buffer, hndl, value);
            break;
        }
        case 8:
        {
            emac_field_ING_DSI_CTRL_8_set(emac_buffer, hndl, value);
            break;
        }
        case 9:
        {
            emac_field_ING_DSI_CTRL_9_set(emac_buffer, hndl, value);
            break;
        }
        case 10:
        {
            emac_field_ING_DSI_CTRL_10_set(emac_buffer, hndl, value);
            break;
        }
        case 11:
        {
            emac_field_ING_DSI_CTRL_11_set(emac_buffer, hndl, value);
            break;
        }
        default:
        {
            PMC_RETURN(EMAC_LOG_ERR_INVALID_PARAMETERS);
        }
        }
    }

    emac_buffer_flush(emac_buffer);
    PMC_RETURN(PMC_SUCCESS);
}
   
/*******************************************************************************
* emac_link_rx_data_src_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve ingress dsi controller setting for a specific link
*
*
* INPUTS:
*   hndl             - handle to the EMAC instance to be operated on
*   link             - link to configure
*
* OUTPUTS:
*   None.
*
* RETURNS:
*      Ingress DSI controller setting
*
* NOTES:
*
*******************************************************************************/  
PUBLIC  emac_rx_data_src_t  emac_link_rx_data_src_get(emac_handle_t *hndl, 
                                                      UINT32         link)
{
    emac_rx_data_src_t rc;
    PMC_ENTRY();


    /* call appropriate emac_field_ING_DSI_CTRL_X function depending on link id */
    switch (link) 
    {
    case 0:
    {
        rc = (emac_rx_data_src_t)emac_field_ING_DSI_CTRL_0_get(NULL, hndl);
        break;
    }
    case 1:
    {
        rc = (emac_rx_data_src_t)emac_field_ING_DSI_CTRL_1_get(NULL, hndl);
        break;
    }
    case 2:
    {
        rc = (emac_rx_data_src_t)emac_field_ING_DSI_CTRL_2_get(NULL, hndl);
        break;
        }
    case 3:
    {
        rc = (emac_rx_data_src_t)emac_field_ING_DSI_CTRL_3_get(NULL, hndl);
        break;
    }
    case 4:
    {
        rc = (emac_rx_data_src_t)emac_field_ING_DSI_CTRL_4_get(NULL, hndl);
        break;
    }
    case 5:
    {
        rc = (emac_rx_data_src_t)emac_field_ING_DSI_CTRL_5_get(NULL, hndl);
        break;
    }
    case 6:
    {
        rc = (emac_rx_data_src_t)emac_field_ING_DSI_CTRL_6_get(NULL, hndl);
        break;
    }
    case 7:
    {
        rc = (emac_rx_data_src_t)emac_field_ING_DSI_CTRL_7_get(NULL, hndl);
        break;
    }
    case 8:
    {
        rc = (emac_rx_data_src_t)emac_field_ING_DSI_CTRL_8_get(NULL, hndl);
        break;
    }
    case 9:
    {
        rc = (emac_rx_data_src_t)emac_field_ING_DSI_CTRL_9_get(NULL, hndl);
        break;
    }
    case 10:
    {
        rc = (emac_rx_data_src_t)emac_field_ING_DSI_CTRL_10_get(NULL, hndl);
        break;
    }
    case 11:
    {
        rc = (emac_rx_data_src_t)emac_field_ING_DSI_CTRL_11_get(NULL, hndl);
        break;
        }
    default:
    {
        rc = EMAC_RX_SRC_UNKNOWN;
        break;
    }
    }
    PMC_RETURN(rc);
}/*emac_link_rx_data_src_get*/

/*******************************************************************************
* emac_link_tx_data_src_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve egress dsi controller setting for a specific link
*
*
* INPUTS:
*   hndl             - handle to the EMAC instance to be operated on
*   link             - link to configure
*
* OUTPUTS:
*   None.
*
* RETURNS:
*      Egress DSI controller setting
*
* NOTES:
*
*******************************************************************************/  
PUBLIC emac_tx_data_dst_t emac_link_tx_data_dst_get(emac_handle_t *hndl, 
                                                    UINT32 link)
{
    emac_tx_data_dst_t rc;
    PMC_ENTRY();


    /* call appropriate emac_field_EGR_DSI_CTRL_X function depending on link id */
    switch (link) 
    {
    case 0:
    {
        rc = (emac_tx_data_dst_t)emac_field_EGR_DSI_CTRL_0_get(NULL, hndl);
        break;
    }
    case 1:
    {
        rc = (emac_tx_data_dst_t)emac_field_EGR_DSI_CTRL_1_get(NULL, hndl);
        break;
    }
    case 2:
    {
        rc = (emac_tx_data_dst_t)emac_field_EGR_DSI_CTRL_2_get(NULL, hndl);
        break;
        }
    case 3:
    {
        rc = (emac_tx_data_dst_t)emac_field_EGR_DSI_CTRL_3_get(NULL, hndl);
        break;
    }
    case 4:
    {
        rc = (emac_tx_data_dst_t)emac_field_EGR_DSI_CTRL_4_get(NULL, hndl);
        break;
    }
    case 5:
    {
        rc = (emac_tx_data_dst_t)emac_field_EGR_DSI_CTRL_5_get(NULL, hndl);
        break;
    }
    case 6:
    {
        rc = (emac_tx_data_dst_t)emac_field_EGR_DSI_CTRL_6_get(NULL, hndl);
        break;
    }
    case 7:
    {
        rc = (emac_tx_data_dst_t)emac_field_EGR_DSI_CTRL_7_get(NULL, hndl);
        break;
    }
    case 8:
    {
        rc = (emac_tx_data_dst_t)emac_field_EGR_DSI_CTRL_8_get(NULL, hndl);
        break;
    }
    case 9:
    {
        rc = (emac_tx_data_dst_t)emac_field_EGR_DSI_CTRL_9_get(NULL, hndl);
        break;
    }
    case 10:
    {
        rc = (emac_tx_data_dst_t)emac_field_EGR_DSI_CTRL_10_get(NULL, hndl);
        break;
    }
    case 11:
    {
        rc = (emac_tx_data_dst_t)emac_field_EGR_DSI_CTRL_11_get(NULL, hndl);
        break;
        }
    default:
    {
        rc = EMAC_TX_DST_UNKNOWN;
        break;
    }
    }
    PMC_RETURN(rc);
}/*emac_link_tx_data_src_get*/


/*******************************************************************************
* emac_link_EGR_DSI_CTRL_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   enable or disable egress dsi controller for a specific link
*
*
* INPUTS:
*   hndl             - handle to the EMAC instance to be operated on
*   link             - link to configure
*   data_rate        - data rate for the link
*   value            - configure switch: TRUE: enable, FALSE: disable 
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
PRIVATE PMC_ERROR emac_link_EGR_DSI_CTRL_cfg(emac_handle_t *hndl, 
                                             UINT32 link,
                                             emac_data_rate_t data_rate,
                                             emac_tx_data_dst_t value)
{
    UINT32 nb_dsi_links;
    emac_buffer_t emac_buffer[1];
    UINT32 i;

    PMC_ENTRY();

    /* Configure core segment */
    switch( data_rate ) 
    {
    case EMAC_LINK_100G: {
    
        nb_dsi_links = EMAC_100G_NB_PMA_LANES;
        break;
    } 
    case EMAC_LINK_40G: {

        nb_dsi_links = EMAC_40G_NB_PMA_LANES;
        break;
    } 
    default:
    {   
        nb_dsi_links = EMAC_10G_NB_PMA_LANES;
    } /* end if */
    }

    /* initialize EMAC buffer used to optimize read/modify access */
    emac_buffer_init(emac_buffer, hndl);

    for(i = 0;  i < nb_dsi_links; i++){

    
        /* call appropriate emac_field_EGR_DSI_CTRL_X function depending on link id */
        switch (link + i) 
        {
        case 0:
        {
            emac_field_EGR_DSI_CTRL_0_set(emac_buffer, hndl, value);
            break;
        }
        case 1:
        {
            emac_field_EGR_DSI_CTRL_1_set(emac_buffer, hndl, value);
            break;
        }
        case 2:
        {
            emac_field_EGR_DSI_CTRL_2_set(emac_buffer, hndl, value);
            break;
        }
        case 3:
        {
            emac_field_EGR_DSI_CTRL_3_set(emac_buffer, hndl, value);
            break;
        }
        case 4:
        {
            emac_field_EGR_DSI_CTRL_4_set(emac_buffer, hndl, value);
            break;
        }
        case 5:
        {
            emac_field_EGR_DSI_CTRL_5_set(emac_buffer, hndl, value);
            break;
        }
        case 6:
        {
            emac_field_EGR_DSI_CTRL_6_set(emac_buffer, hndl, value);
            break;
        }
        case 7:
        {
            emac_field_EGR_DSI_CTRL_7_set(emac_buffer, hndl, value);
            break;
        }
        case 8:
        {
            emac_field_EGR_DSI_CTRL_8_set(emac_buffer, hndl, value);
            break;
        }
        case 9:
        {
            emac_field_EGR_DSI_CTRL_9_set(emac_buffer, hndl, value);
            break;
        }
        case 10:
        {
            emac_field_EGR_DSI_CTRL_10_set(emac_buffer, hndl, value);
            break;
        }
        case 11:
        {
            emac_field_EGR_DSI_CTRL_11_set(emac_buffer, hndl, value);
            break;
        }
        default:
        {
            PMC_RETURN(EMAC_LOG_ERR_INVALID_PARAMETERS);
        }
        }
    }

    emac_buffer_flush(emac_buffer);

    PMC_RETURN(PMC_SUCCESS);
}

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
PRIVATE PMC_ERROR emac_link_id_check(UINT32 link)   
{
    PMC_ENTRY();

    if (link >= EMAC_NUM_LINKS )
    {
        PMC_RETURN(EMAC_LOG_ERR_INVALID_PARAMETERS);
    }

    PMC_RETURN(PMC_SUCCESS);
}


/*******************************************************************************
* emac_link_rate_check
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
*      EMAC_LOG_ERR_INVALID_PARAMETERS   - when link id is invalid
*      PMC_SUCCESS                       - when link id is valid
*
* NOTES:
*
*******************************************************************************/    
PRIVATE PMC_ERROR emac_link_rate_check(UINT32 link, 
                                       emac_data_rate_t data_rate)   
{
    PMC_ENTRY();

    /* check that link rate is superorior to 0 */
    if (data_rate != EMAC_LINK_100G && data_rate != EMAC_LINK_40G 
        && data_rate != EMAC_LINK_10G )
    {
        PMC_RETURN(EMAC_LOG_ERR_INVALID_PARAMETERS);
    }
    
    /* Check for valid link */
    switch(data_rate)
    {
    case EMAC_LINK_100G:
        if (EMAC_100G_LINK_0 != link) 
        {
            PMC_RETURN(EMAC_LOG_ERR_INVALID_PARAMETERS);
        }
        break;        
    case EMAC_LINK_40G:
        if ((EMAC_40G_LINK_0 != link) && (EMAC_40G_LINK_4 != link) && (EMAC_40G_LINK_8 != link)) 
        {
            PMC_RETURN(EMAC_LOG_ERR_INVALID_PARAMETERS);
        }
        break;
    case EMAC_LINK_10G:
        break;
    default:
        PMC_RETURN(EMAC_LOG_ERR_INVALID_PARAMETERS);
    }
        

    PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
*  emac_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the EMAC block energy state from EMAC local register fields. 
*
* INPUTS:
*   hndl           - handle to the EMAC instance to be operated on
*   energy_state   - energy_state
*
* OUTPUTS:
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PUBLIC void emac_energy_state_reg_set ( emac_handle_t *hndl, pmc_energy_state_t energy_state)
{
    PMC_ENTRY();    
    PMC_ASSERT(hndl!=NULL, EMAC_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    switch(energy_state)
    {
    case PMC_ENERGY_STATE_RUNNING:
        emac_field_ETIME_TS_LOWPWR_set(NULL, hndl, 0);
        emac_field_ETIME_NREGS_SW_RESET_set(NULL, hndl, 0);
        emac_field_ETIME_CORE_SW_RESET_set(NULL, hndl, 0);
        emac_field_EMAC_NREGS_SW_RESET_set(NULL, hndl, 0);
        emac_field_EMAC_CORE_SW_RESET_set(NULL, hndl, 0);
        break;
    case PMC_ENERGY_STATE_RESET:
        emac_field_ETIME_TS_LOWPWR_set(NULL, hndl, 1);
        emac_field_ETIME_NREGS_SW_RESET_set(NULL, hndl, 1);
        emac_field_ETIME_CORE_SW_RESET_set(NULL, hndl, 1);
        emac_field_EMAC_NREGS_SW_RESET_set(NULL, hndl, 1);
        emac_field_EMAC_CORE_SW_RESET_set(NULL, hndl, 1);
        emac_field_EMAC_STAT_RAM_LOWPWR_set( NULL, hndl, 0xFFF);    
        break;
    default:
        PMC_ASSERT(FALSE, EMAC_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }

    PMC_RETURN();
} /* emac_energy_state_reg_set */

/*******************************************************************************
*  emac_block_non_default_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures Non-default register settings associated with the EMAC block in 
*   ENET subsystem.
*
*
* INPUTS:
*   *hndl         - pointer to EMAC handle instance
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
PUBLIC void emac_block_non_default_cfg(emac_handle_t *hndl)
{
    emac_buffer_t emac_buffer[1];
    emac_core_buffer_t emac_core_buffer[1];

    PMC_ENTRY();

    /* initialize EMAC buffer used to optimize read/modify access */
    emac_buffer_init(emac_buffer, hndl);
    emac_core_buffer_init(emac_core_buffer, hndl);

    /* 
     * Take EMAC and ETIME out of reset
     * Register 0x10: EMAC120_TOP - ETIME Low Power Control
     * Register 0x0: EMAC120_TOP - Software Reset
     */
    
    /* Configure non-default static values */    
    emac_core_field_SEG3_INIT_CREDIT_set(emac_core_buffer, hndl, EMAC_CORE_TDM_CREDIT_VAL);
    emac_core_field_SEG2_INIT_CREDIT_set(emac_core_buffer, hndl, EMAC_CORE_TDM_CREDIT_VAL);
    emac_core_field_SEG1_INIT_CREDIT_set(emac_core_buffer, hndl, EMAC_CORE_TDM_CREDIT_VAL);
    emac_core_field_SEG0_INIT_CREDIT_set(emac_core_buffer, hndl, EMAC_CORE_TDM_CREDIT_VAL);

    emac_core_field_SEG4_INIT_CREDIT_set(emac_core_buffer, hndl, EMAC_CORE_TDM_CREDIT_VAL);
    emac_core_field_SEG5_INIT_CREDIT_set(emac_core_buffer, hndl, EMAC_CORE_TDM_CREDIT_VAL);
    emac_core_field_SEG6_INIT_CREDIT_set(emac_core_buffer, hndl, EMAC_CORE_TDM_CREDIT_VAL);
    emac_core_field_SEG7_INIT_CREDIT_set(emac_core_buffer, hndl, EMAC_CORE_TDM_CREDIT_VAL);

    emac_core_field_SEG8_INIT_CREDIT_set(emac_core_buffer, hndl, EMAC_CORE_TDM_CREDIT_VAL);
    emac_core_field_SEG9_INIT_CREDIT_set(emac_core_buffer, hndl, EMAC_CORE_TDM_CREDIT_VAL);
    emac_core_field_SEG10_INIT_CREDIT_set(emac_core_buffer, hndl, EMAC_CORE_TDM_CREDIT_VAL);
    emac_core_field_SEG11_INIT_CREDIT_set(emac_core_buffer, hndl, EMAC_CORE_TDM_CREDIT_VAL);

    emac_buffer_flush(emac_buffer);
    emac_core_buffer_flush(emac_core_buffer);

    PMC_RETURN();
}

/*******************************************************************************
*  emac_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize EMAC var_t variables.
*
* INPUTS:
*   *hndl      - pointer to EMAC handle instance
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC void emac_var_default_init(emac_handle_t *hndl)
{
    UINT32 itr;
    PMC_ENTRY();

    for (itr = 0; itr < EMAC_NUM_LINKS; itr++)
    {
        hndl->var.tx_enable[itr] = FALSE;
        hndl->var.rx_enable[itr] = FALSE;
        hndl->var.loopback[itr] = EMAC_DISABLE_LOOPBACK;
        hndl->var.prbs_ctxt[itr]->enable = FALSE;
    }
    PMC_RETURN();
}/* emac_var_default_init*/


/*******************************************************************************
*  emac_loopback_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable or disable an enet looopback
*
* INPUTS:
*   *hndl         - pointer to EMAC handle instance
*   link          - link to configure
*   loopback      - EMAC_MAC_LOOPBACK         - MAC loopback
*                 - EMAC_MLD_LOOPBACK         - MAC loopback at MLD
*                 - EMAC_DISABLE_LOOPBACK     - Disable loopback
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_loopback_cfg(emac_handle_t *hndl,
                                   UINT32 link, 
                                   emac_loopback_cfg_t loopback)
{
    PMC_ERROR rc = PMC_SUCCESS;
    emac_loopback_cfg_t prev_loopback;

    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, EMAC_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((link < EMAC_NUM_LINKS), EMAC_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    prev_loopback = hndl->var.loopback[link];

    /* If a loopback is being enabled, check that the current state is loopback disabled */
    if (loopback != EMAC_DISABLE_LOOPBACK)
    {
        PMC_ASSERT(prev_loopback == EMAC_DISABLE_LOOPBACK, EMAC_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }

    PMC_LOG_TRACE("ENET MAC loopback: %d link = %d\n", loopback,link);
    switch(loopback)
    {

    case EMAC_MLD_LOOPBACK:
        /* enable tx to rx loopback */
        rc = emac_link_ING_DSI_CTRL_cfg(hndl, link, hndl->var.data_rate[link],EMAC_RX_SRC_TX_EMAC_PMA);
        break;

    case EMAC_DISABLE_LOOPBACK:
        if (prev_loopback == EMAC_MLD_LOOPBACK)
        {
            /* disable loopback & restore previous settings */    
            rc = emac_link_ING_DSI_CTRL_cfg(hndl, link, hndl->var.data_rate[link],hndl->var.rx_data_src[link]);
        }
        break;

    default:
        PMC_ASSERT(FALSE, EMAC_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }
    if (rc == PMC_SUCCESS)
    {
        hndl->var.loopback[link] = loopback;
    }

    PMC_RETURN(rc);
} /*emac_loopback_cfg*/

/*******************************************************************************
*  emac_loopback_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the status of teh EMAC loopback. The loopback_ptr 
*   parameter will be set to one of the following:
*                        EMAC_MAC_LOOPBACK         - MAC loopback
*                        EMAC_MLD_LOOPBACK         - MAC loopback at MLD
*                        EMAC_DISABLE_LOOPBACK     - Loopback is disable
*
* INPUTS:
*   *hndl         - pointer to EMAC handle instance
*   link          - link to configure
*
* OUTPUTS:
*   *loopback_ptr - loopback status (see emac_loopback_cfg_t)
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_loopback_status_get(emac_handle_t *hndl,
                                          UINT32 link, 
                                          emac_loopback_cfg_t *loopback_ptr)
{
    PMC_ENTRY();
    PMC_ASSERT(hndl!=NULL, EMAC_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(link < EMAC_NUM_LINKS, EMAC_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    *loopback_ptr = hndl->var.loopback[link];

    PMC_RETURN(PMC_SUCCESS);
} /* emac_loopback_status_get */


/*******************************************************************************
* emac_activated_link_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns all started channels
*                                                                               
* INPUTS:                                                                       
*   *hndl             - pointer to EMAC handle instance
*                                                                             
* OUTPUTS:
*   *rx_actv_link     - status of emac RX channels
*   *tx_actv_link     - status of emac TX channels
*
* RETURNS: 
*   None.                                                                        
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC void emac_activated_link_get(emac_handle_t   *hndl,
                                    BOOL8           *rx_actv_link,
                                    BOOL8           *tx_actv_link)
{
    UINT32 itr;
    PMC_ENTRY();
    
    PMC_ASSERT(hndl != NULL, EMAC_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    for (itr = 0; itr < EMAC_NUM_LINKS; itr++)
    {
        rx_actv_link[itr] = emac_mac_field_RX_ENA_get(NULL, hndl, itr);
        tx_actv_link[itr] = emac_mac_field_TX_ENA_get(NULL, hndl, itr);
    }

    PMC_RETURN();
}  /* emac_activated_link_get */

/*******************************************************************************
* emac_link_rate_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns all started channels
*                                                                               
* INPUTS:                                                                       
*   *hndl             - pointer to EMAC handle instance
*                                                                             
* OUTPUTS:
*   *data_rate        - EMAC links data rate
*
* RETURNS: 
*   None.                                                                        
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC void emac_link_rate_get(emac_handle_t       *hndl,
                               emac_data_rate_t    *data_rate)
{
    UINT32 itr;
    UINT32 mode_ctl;
    UINT32 incr;
    PMC_ENTRY();
    
    PMC_ASSERT(hndl != NULL, EMAC_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    mode_ctl = emac_core_field_MODE_CTL_get(NULL, hndl);

    
    for (itr = 0; itr < EMAC_NUM_LINKS; itr++)
    {
        data_rate[itr] = LAST_EMAC_DATA_RATE; 
    }
    
    for (itr = 0; itr < EMAC_NUM_LINKS; itr += incr)
    {
        incr = 1;
        data_rate[itr] = (emac_data_rate_t)((mode_ctl >> (itr * 2)) & 0x3);
        if ( EMAC_LINK_100G == data_rate[itr])
        {
            break;
        }
        if ( EMAC_LINK_40G == data_rate[itr])
        {
            incr = 4;
        }
    }

    PMC_RETURN();
}  /* emac_link_rate_get */

/*******************************************************************************
* emac_mac_tx_flush_toggle
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function toggles the TX_FLUSH bit from 1 to 0.
*                                                                               
* INPUTS:                                                                       
*   *hndl             - pointer to EMAC handle instance
*   link              - EMAC MAC instance 0 to 11
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
PUBLIC void emac_mac_tx_flush_toggle(emac_handle_t       *hndl,
                                     UINT32              link)
{
    PMC_ENTRY();
    
    PMC_ASSERT(hndl != NULL, EMAC_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(link < 12, EMAC_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    emac_mac_field_TX_FLUSH_set( NULL, hndl, link, 1);
    PMC_Q_USLEEP(hndl,5);
    emac_mac_field_TX_FLUSH_set( NULL, hndl, link, 0);

    PMC_RETURN();
}  /* emac_mac_tx_flush_toggle */


/*******************************************************************************
*  emac_tx_ipg_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the Transmit Inter-Packet Gap.
*
* INPUTS:
*   hndl           - handle to the EMAC instance to be operated on
*   link           - link to configure
*   data_rate      - data rate for the link
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS when operation is successful otherwise a descriptive error code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR emac_tx_ipg_default_value_cfg(emac_handle_t *hndl, 
                                                UINT32 link, 
                                                emac_data_rate_t data_rate)
{
    UINT32 tx_ipg_val;

    PMC_ENTRY();

    
    if (EMAC_NUM_LINKS <= link)
    {
        PMC_RETURN(EMAC_LOG_ERR_INVALID_LINK_ID);
    }

    switch (data_rate)
    {
    case EMAC_LINK_10G:
        tx_ipg_val = 12;
        break;
    case EMAC_LINK_40G:
    case EMAC_LINK_100G:
        tx_ipg_val = 0;
        break; 
    default:
        PMC_RETURN(EMAC_LOG_ERR_LINK_NOT_PROV);
        break;
    }

    emac_mac_field_TX_IPG_LENGTH_set(NULL, hndl, link, tx_ipg_val);
    
    PMC_RETURN(PMC_SUCCESS);
}


/*******************************************************************************
* util_patt_mode_to_emac_mode_conv 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert pattern mode defined defined in util global to emac PRBS type enum.
*
*
* INPUTS:
*  util_patt_mode         - util global pattern mode enum
*
* OUTPUTS:
*   *emac_prbs_type       - emac PRBS type
*
* RETURNS:
*   PMC_SUCCESS                 - when conversion is done 
*                                 otherwise a descriptive error is returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR util_patt_mode_to_emac_mode_conv(util_patt_mode_t    util_patt_mode,
                                                   emac_prbs_type_t   *emac_prbs_type)
{
    PMC_ENTRY();

    switch (util_patt_mode)
    {
    case UTIL_PATT_MODE_PRBS_31:
        *emac_prbs_type = EMAC_PRBS31;
        break;
    case UTIL_PATT_MODE_PRBS_9:
        *emac_prbs_type = EMAC_PRBS9;
        break; 
    default:
        PMC_RETURN(EMAC_LOG_ERR_PRBS_TYPE);
    }

    PMC_RETURN(PMC_SUCCESS);
        
}/*util_patt_mode_to_emac_mode_conv*/

/*******************************************************************************
* FUNCTION: emac_int_chnl_end_link_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns the end link of an ENET channel given its start link
*
*
* INPUTS:
*   *emac_handle     - Pointer to EMAC handle instance to be operated on
*   is_multichannels - TRUE if function is being called in multi channel case
*                      FALSE if function is being called for a real channel
*   start_link       - The channel's start link
*
* OUTPUTS:
*   *end_link_ptr   - The channel's end link
*
* RETURNS:
*   PMC_SUCCESS when operation is successful otherwise a descriptive error code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR emac_int_chnl_end_link_get(emac_handle_t  *emac_handle,
                                             BOOL8           is_multichannels,
                                             UINT32          start_link,
                                             UINT32         *end_link_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 channel_num_links = 0;


    PMC_ENTRY();

    if (TRUE == is_multichannels)
    {
        /* If this function is being called for multi channel case,
         * then we operate on a single link. */
        *end_link_ptr = start_link;
    }
    else
    {
        /* If this function is not being called for multi channel case (real channel),
         * then end link is determined based on the channel's rate. */

        switch (emac_handle->var.data_rate[start_link])
        {
            case EMAC_LINK_10G:
            {
                channel_num_links = EMAC_10G_NB_PMA_LANES;
            }
            break;

            case EMAC_LINK_40G:
            {
                channel_num_links = EMAC_40G_NB_PMA_LANES;
            }
            break;

            case EMAC_LINK_100G:
            {
                channel_num_links = EMAC_100G_NB_PMA_LANES;
            }
            break;

            default:
            {
                rc = EMAC_LOG_ERR_INVALID_PARAMETERS;
            }
        }

        *end_link_ptr = start_link + channel_num_links - 1;
    }

    PMC_RETURN(rc);
}


/*******************************************************************************
* FUNCTION: emac_100G_check_align_status_v_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Check the value of align_status_v for 100G mode only.
*
* INPUTS:
*   hndl     - pointer to DIGI handle instance.
*
* OUTPUTS;
*   None.
*
* RETURNS:
*   TRUE - align_status_v is set;
*   FALSE - otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL emac_100G_check_align_status_v_get(emac_handle_t *hndl)
{
    UINT32 align_status_v = 0;
    BOOL   rc             = FALSE;

    PMC_ENTRY();

    align_status_v = emac_field_ALIGN_STATUS_V_get(NULL, hndl);

    if (0x1 == align_status_v)
    {
        rc = TRUE;
    }

    PMC_RETURN(rc);
}


/*******************************************************************************
* FUNCTION: emac_100G_disable_align_status_loc_fault()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable the align_status and rx_loc_fault interrupt enables
*
* INPUTS:
*   hndl     - pointer to EMAC handle instance.
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
PUBLIC void emac_100G_disable_align_status_loc_fault(emac_handle_t *hndl)
{
    PMC_ENTRY();
    emac_field_ALIGN_STATUS_E_set(NULL, hndl, 0x0);
    emac_field_RX_LOC_FAULT_E_set(NULL, hndl, 0x0);
    PMC_RETURN();
}


/*******************************************************************************
* FUNCTION: emac_100G_enable_align_status_loc_fault()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable the align_status and rx_loc_fault interrupt enables
*
* INPUTS:
*   hndl     - pointer to EMAC handle instance.
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
PUBLIC void emac_100G_enable_align_status_loc_fault(emac_handle_t *hndl)
{
    PMC_ENTRY();
    emac_field_ALIGN_STATUS_E_set(NULL, hndl, 0x1);
    emac_field_RX_LOC_FAULT_E_set(NULL, hndl, 0xFFF);
    PMC_RETURN();
}


/*******************************************************************************
* FUNCTION: emac_100G_clear_align_status_loc_fault()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Write to clear align_status_i and rx_loc_fault_i
*
* INPUTS:
*  hndl     - pointer to EMAC handle instance.
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
PUBLIC void emac_100G_clear_align_status_loc_fault(emac_handle_t *hndl)
{
    PMC_ENTRY();
    emac_field_ALIGN_STATUS_I_set_to_clear(NULL, hndl, 0x7);
    emac_field_RX_LOC_FAULT_I_set_to_clear(NULL, hndl, 0xFFF);
    PMC_RETURN();
}


/*******************************************************************************
*  emac_pcs_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resets the EMAC PCS block
*
* INPUTS:
*   hndl           - handle to the EMAC instance to be operated on
*   link           - indice of link to reset
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
PUBLIC void emac_pcs_reset(emac_handle_t    *hndl,
                           UINT32           link)
{
    PMC_ENTRY();

    /* Resetting the PCS layer */
    emac_pcs_field_RESET_set( NULL, hndl, link, TRUE );


    PMC_RETURN();
}



/*
** End of file
*/
