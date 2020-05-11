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

#include "exco4m_loc.h"

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
const char EXCO4M_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    EXCO4M_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char EXCO4M_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Public Functions
*/

/*******************************************************************************
* exco4m_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a EXCO4M block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the EXCO4M block relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - pointer to TSB name
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   exco4m_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC exco4m_handle_t *exco4m_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name )
{
    exco4m_handle_t *exco4m_handle;

    PMC_ENTRY();
    exco4m_handle = (exco4m_handle_t*)PMC_CTXT_CALLOC(sizeof(exco4m_handle_t), parent);

    pmc_handle_init( parent, exco4m_handle, sys_handle, PMC_MID_DIGI_EXCO4M, tsb_name, base_address );

    /* Register log strings. */
    pmc_log_block_strings_register(EXCO4M_LOG_ERR_STRINGS[0], EXCO4M_LOG_ERR_TABLE_BASE, EXCO4M_LOG_ERR_COUNT);

    PMC_RETURN(exco4m_handle);
} /* exco4m_ctxt_create */

/*******************************************************************************
* exco4m_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a EXCO4M block instance.
*
*
* INPUTS:
*   *exco4m_handle        - pointer to EXCO4M handle instance
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
PUBLIC void exco4m_ctxt_destroy(exco4m_handle_t *exco4m_handle)
{
    PMC_ENTRY();
    PMC_CTXT_FREE(&exco4m_handle, exco4m_handle);

    PMC_RETURN();
} /* exco4m_ctxt_destroy */

/*******************************************************************************
* exco4m_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a EXCO4M block handle instance.
*
*
* INPUTS:
*   *exco4m_handle        - pointer to EXCO4M handle instance
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
PUBLIC void exco4m_handle_init(exco4m_handle_t *exco4m_handle)
{
    PMC_ENTRY();
    PMC_RETURN();
} /* exco4m_handle_init */

/*******************************************************************************
* exco4m_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on exco4m.
*
*
* INPUTS:
*   *exco4m_handle        - pointer to EXCO4M handle instance
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
PUBLIC PMC_ERROR exco4m_handle_restart_init(exco4m_handle_t *exco4m_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                            pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(EXCO4M_LOG_ERR_STRINGS[0], EXCO4M_LOG_ERR_TABLE_BASE, EXCO4M_LOG_ERR_COUNT);
    }

    PMC_RETURN(ret_val);
} /* exco4m_handle_restart_init */

/*******************************************************************************
* exco4m_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the EXCO4M instance to the starting state for all
*   dynamic context.  All dynamic context will be returned to the initialization 
*   values that were generate during the call to exco4m_handle_init().
*
*   This function is for use by parent modules that have provided all other
*   provisions for ensuring the state of all resources tracked by this
*   context has coherent register state.
*
*
* INPUTS:
*   *hndl                 - pointer to EXCO4M handle instance
*   energy_state          - enum defining state to set the EXCO4M module to
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
PUBLIC PMC_ERROR exco4m_init(exco4m_handle_t *hndl, pmc_energy_state_t energy_state)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    PMC_ENTRY();
    PMC_ASSERT(hndl!=NULL, EXCO4M_ERR_INVALID_PARAMETERS, 0, 0); 
    
    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        /* no action required for initializing the exco4m module for
           PMC_ENERGY_STAET_RUNNING */
    }
    else if (PMC_ENERGY_STATE_RESET == energy_state)
    {
        /* no action required for initializing the exco4m module for
           PMC_ENERGY_STAET_RESET */
    } else {
        PMC_ASSERT(FALSE, EXCO4M_ERR_INVALID_PARAMETERS, 0, 0);
    }    
  
    PMC_RETURN(ret_val);

} /* exco4m_init */


/*******************************************************************************
* exco4m_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   exco4m instance.
*
* INPUTS:
*   *hndl                 - pointer to EXCO4M handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8                  - FALSE:  Resource(s) not in start state \n
*                           TRUE:   Resources all in start state
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 exco4m_start_state_test(exco4m_handle_t *hndl)
{
    PMC_ENTRY();
    PMC_ASSERT(hndl!=NULL, EXCO4M_ERR_INVALID_PARAMETERS, 0, 0);

    /* Module contains no var_t context, no testing to be performed */

    PMC_RETURN(TRUE);

} /* exco4m_start_state_test */

/*******************************************************************************
* exco4m_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of a EXCO4M block instance.
*
*
* INPUTS:
*   *exco4m_handle        - pointer to EXCO4M handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Base address of EXCO4M.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 exco4m_base_address_get(exco4m_handle_t *exco4m_handle)
{
    PMC_ENTRY();
    PMC_RETURN(exco4m_handle->base.base_address);
} /* exco4m_base_address_get */


/*******************************************************************************
* Initialization Configuration Functions
*******************************************************************************/


/*******************************************************************************
*  exco4m_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure exco4m block.
*
* INPUTS:
*   hndl     - handle to the EXCO4M instance to be operated on
*   link     - link to activate
*   datapath - datapath to configure
*   enable   - TRUE: Enable datapath. FALSE: Disable.
*   set_rx_pcs_push_ena - FALSE: for EPMM. TRUE: for ETRANS
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR exco4m_cfg(exco4m_handle_t *hndl, UINT32 link, exco4m_datapath_t datapath, BOOL8 enable, BOOL8 set_rx_pcs_push_ena ) {

    PMC_ENTRY();

    if( enable ) {
      exco4m_field_EXCO_CORE_SW_RESET_set( NULL, hndl, 0);

      if( datapath == EXCO4M_GFP_GSUP43_C62_100GE ) {
        exco4m_field_RX_CGMII_MODE_set( NULL, hndl, 1 );
        exco4m_field_TX_CGMII_MODE_set( NULL, hndl, 1 );
      } else {
        exco4m_field_RX_CGMII_MODE_set( NULL, hndl, 0);
        exco4m_field_TX_CGMII_MODE_set( NULL, hndl, 0);
        
        exco4m_reg_RX_GLOBAL_CTRL_field_set( NULL, hndl, 0x00000004, 2, 1);
        exco4m_reg_TX_GLOBAL_CTRL_field_set( NULL, hndl, 0x00000004, 2, 1);
      }

      exco4m_field_RX_PCS_PUSH_ENA_set( NULL, hndl, set_rx_pcs_push_ena );

    } else {
      exco4m_field_EXCO_CORE_SW_RESET_set( NULL, hndl, 1);
    }


    PMC_RETURN(PMC_SUCCESS);

} /* exco4m_cfg */



/*******************************************************************************
* Run-time Operations
*******************************************************************************/
/*******************************************************************************
*  exco4m_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*   hndl     - handle to the EXCO4M instance to be operated on
*   link     - link to activate
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR exco4m_activate(exco4m_handle_t *hndl, UINT32 link)
{
    PMC_ENTRY();
    
    PMC_RETURN(PMC_SUCCESS);

} /* exco4m_activate */

/*******************************************************************************
*  exco4m_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable channel.
*
* INPUTS:
*   hndl     - handle to the EXCO4M instance to be operated on
*   link     - link to configure

*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR exco4m_deactivate(exco4m_handle_t *hndl, UINT32 link)
{
    PMC_ENTRY();
    PMC_RETURN(PMC_SUCCESS);

} /* exco4m_deactivate */

/*******************************************************************************
*  exco4m_lowpwr
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable channel.
*
* INPUTS:
*   hndl     - handle to the EXCO4M instance to be operated on
*   value    - TRUE/FALSE = LOWPWR/Not
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR exco4m_lowpwr(exco4m_handle_t *hndl, BOOL8 value)
{
    PMC_ENTRY();


    PMC_RETURN(PMC_SUCCESS);

} /* exco4m_lowpwr */

/*******************************************************************************
*  exco4m_tx_lane_info_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   retrieves the TX_LANE_ID and TX_LANE_SKEW_FIFO_ADDR.
*
* INPUTS:
*   hndl     - handle to the EXCO4M instance to be operated on
*
* OUTPUTS:
*   txLaneId            - array to store TX_LANE_ID
*   txLaneSkewFifoAddr  - array to store TX_LANE_SKEW_FIFO_ADDR
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void exco4m_tx_lane_info_get(exco4m_handle_t *hndl,
                                    UINT8 txLaneId[20],
                                    UINT8 txLaneSkewFifoAddr[20])
{
    UINT8 i;
    
    PMC_ENTRY();

    for( i=0; i<20; i++)
    {
        txLaneId[i] = exco4m_field_TX_PCS_LANE_ID_get(NULL, hndl, i);
        txLaneSkewFifoAddr[i] = exco4m_field_TX_PCS_LANE_SKEW_FIFO_ADDR_get(NULL, hndl, i);
    }

    PMC_RETURN();

} /* exco4m_tx_lane_info_get */

/*******************************************************************************
*  exco4m_rx_lf_os_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   inserts or disables local fault ordered sets in RX datapath.
*
* INPUTS:
*   hndl     - handle to the EXCO4M instance to be operated on
*   enable   - TRUE/FALSE = insert/disable
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
PUBLIC void exco4m_rx_lf_os_cfg(exco4m_handle_t *hndl,
                                BOOL8            enable)
{
    PMC_ENTRY();

    exco4m_field_RX_FORCE_LF_OS_set(NULL, hndl, enable);

    PMC_RETURN();

} /* exco4m_rx_lf_os_cfg */

/*******************************************************************************
*  exco4m_rx_lf_os_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   retrieves the inserts or disables status of local fault ordered sets in RX datapath.
*
* INPUTS:
*   hndl     - handle to the EXCO4M instance to be operated on
*
* OUTPUTS:
*   enable   - TRUE/FALSE = insert/disable
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void exco4m_rx_lf_os_cfg_get(exco4m_handle_t *hndl,
                                    BOOL8           *enable)
{
    PMC_ENTRY();

    *enable = exco4m_field_RX_FORCE_LF_OS_get(NULL, hndl);

    PMC_RETURN();

} /* exco4m_rx_lf_os_cfg_get */


/*******************************************************************************
*  exco4m_tx_lf_os_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   inserts or disables local fault ordered sets in TX datapath.
*
* INPUTS:
*   hndl     - handle to the EXCO4M instance to be operated on
*   enable   - TRUE/FALSE = insert/disable
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
PUBLIC void exco4m_tx_lf_os_cfg(exco4m_handle_t *hndl,
                                BOOL8            enable)
{
    PMC_ENTRY();

    exco4m_field_TX_FORCE_LF_OS_set(NULL, hndl, enable);

    PMC_RETURN();

} /* exco4m_tx_lf_os_cfg */

/*******************************************************************************
*  exco4m_tx_lf_os_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   retrieves the inserts or disables status of local fault ordered sets in TX datapath.
*
* INPUTS:
*   hndl     - handle to the EXCO4M instance to be operated on
*
* OUTPUTS:
*   enable   - TRUE/FALSE = insert/disable
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void exco4m_tx_lf_os_cfg_get(exco4m_handle_t *hndl,
                                    BOOL8           *enable)
{
    PMC_ENTRY();

    *enable = exco4m_field_TX_FORCE_LF_OS_get(NULL, hndl);

    PMC_RETURN();

} /* exco4m_tx_lf_os_cfg_get */


/*******************************************************************************
*  exco4m_rx_rf_os_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   inserts or disables remote fault ordered sets in RX datapath.
*
* INPUTS:
*   hndl     - handle to the EXCO4M instance to be operated on
*   enable   - TRUE/FALSE = insert/disable
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
PUBLIC void exco4m_rx_rf_os_cfg(exco4m_handle_t *hndl,
                                BOOL8            enable)
{
    PMC_ENTRY();

    exco4m_field_RX_FORCE_RF_OS_set(NULL, hndl, enable);

    PMC_RETURN();

} /* exco4m_rx_rf_os_cfg */

/*******************************************************************************
*  exco4m_rx_rf_os_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   retrieves the inserts or disables status of remote fault ordered sets in RX datapath.
*
* INPUTS:
*   hndl     - handle to the EXCO4M instance to be operated on
*
* OUTPUTS:
*   enable   - TRUE/FALSE = insert/disable
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void exco4m_rx_rf_os_cfg_get(exco4m_handle_t *hndl,
                                    BOOL8           *enable)
{
    PMC_ENTRY();

    *enable = exco4m_field_RX_FORCE_RF_OS_get(NULL, hndl);

    PMC_RETURN();

} /* exco4m_rx_rf_os_cfg_get */

/*******************************************************************************
*  exco4m_tx_rf_os_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   inserts or disables remote fault ordered sets in TX datapath.
*
* INPUTS:
*   hndl     - handle to the EXCO4M instance to be operated on
*   enable   - TRUE/FALSE = insert/disable
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
PUBLIC void exco4m_tx_rf_os_cfg(exco4m_handle_t *hndl,
                                BOOL8            enable)
{
    PMC_ENTRY();

    exco4m_field_TX_FORCE_RF_OS_set(NULL, hndl, enable);

    PMC_RETURN();

} /* exco4m_tx_rf_os_cfg */

/*******************************************************************************
*  exco4m_tx_rf_os_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   retrieves the inserts or disables status of remote fault ordered sets in TX datapath.
*
* INPUTS:
*   hndl     - handle to the EXCO4M instance to be operated on
*
* OUTPUTS:
*   enable   - TRUE/FALSE = insert/disable
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void exco4m_tx_rf_os_cfg_get(exco4m_handle_t *hndl,
                                    BOOL8           *enable)
{
    PMC_ENTRY();

    *enable = exco4m_field_TX_FORCE_RF_OS_get(NULL, hndl);

    PMC_RETURN();

} /* exco4m_tx_rf_os_cfg_get */

/*******************************************************************************
*  exco4m_tx_align_poll
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Poll loss align marker interrupt
*
* INPUTS:
*   hndl     - handle to the EXCO4M instance to be operated on
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
PUBLIC PMC_POLL_RETURN_TYPE exco4m_tx_align_poll(exco4m_handle_t *hndl)
{
    UINT32 num_failed_polls;
    UINT32 i;
    PMC_POLL_RETURN_TYPE rc;

    PMC_ENTRY();

    rc = 0;
    for (i = 0; i < EXCO4M_N_LANES && rc == 0; i++)
    {
        rc = exco4m_field_TX_LOAM_V_poll(NULL, hndl,
                                         i, 0,
                                         PMC_POLL_EQUAL, 1000,
                                         &num_failed_polls,
                                         1);
    }


    PMC_RETURN(rc);
} /* exco4m_tx_align_poll */


/*******************************************************************************
* FUNCTION: exco4m_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   exco4m_handle     - handle to the EXCO4M instance to be operated on
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
PUBLIC PMC_ERROR exco4m_int_validate(exco4m_handle_t  *exco4m_handle,
                                     UINT32            link,
                                     void             *non_null_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != exco4m_handle, EXCO4M_ERR_INVALID_PARAMETERS, 0, 0);
   
    if (link != 0)
    {
        result = EXCO4M_ERR_INVALID_PARAMETERS;
    }

    if (NULL == non_null_ptr)
    {
        result = EXCO4M_ERR_INVALID_PARAMETERS;
    }


    PMC_RETURN(result);
} /* exco4m_int_validate */


/*******************************************************************************
* FUNCTION: exco4m_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   exco4m_handle     - handle to the EXCO4M instance to be operated on
*   link              - Link
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
PUBLIC PMC_ERROR exco4m_int_chnl_enable(exco4m_handle_t    *exco4m_handle,
                                        UINT32              link,
                                        exco4m_int_chnl_t  *int_table_ptr,
                                        exco4m_int_chnl_t  *int_en_table_ptr,
                                        BOOL8                enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          i;

    PMC_ENTRY();

    result = exco4m_int_validate(exco4m_handle, link, int_en_table_ptr);



    #ifdef EXCO4M_INT
    #undef EXCO4M_INT
    #endif
    #define EXCO4M_INT    UTIL_GLOBAL_INT_ENABLE_SINGLE
    
    
    #ifdef EXCO4M_INT_ARRAY
    #undef EXCO4M_INT_ARRAY
    #endif
    #define EXCO4M_INT_ARRAY    UTIL_GLOBAL_INT_ENABLE_ARRAY_SINGLE


    EXCO4M_INT_TABLE_DEFINE();


#ifdef DIGI_FW_INTR_MON
/* Interrupts are in FW */
#else
    EXCO4M_INT_TABLE_DEFINE_FW();
#endif


    PMC_RETURN(result);
} /* exco4m_int_chnl_enable */




/*******************************************************************************
* FUNCTION: exco4m_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   exco4m_handle     - handle to the EXCO4M instance to be operated on
*   link              - Link
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
PUBLIC PMC_ERROR exco4m_int_chnl_clear(exco4m_handle_t    *exco4m_handle,
                                       UINT32              link,
                                       exco4m_int_chnl_t  *int_table_ptr,
                                       exco4m_int_chnl_t  *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          i;

    PMC_ENTRY();

    result = exco4m_int_validate(exco4m_handle, link, int_en_table_ptr);
    

    #ifdef EXCO4M_INT
    #undef EXCO4M_INT
    #endif
    #define EXCO4M_INT UTIL_GLOBAL_INT_CLEAR_SINGLE
    
    
    #ifdef EXCO4M_INT_ARRAY
    #undef EXCO4M_INT_ARRAY
    #endif
    #define EXCO4M_INT_ARRAY UTIL_GLOBAL_INT_CLEAR_ARRAY_SINGLE
    
    

    EXCO4M_INT_TABLE_DEFINE();

   
#ifdef DIGI_FW_INTR_MON
/* Interrupts are in FW */
#else
    EXCO4M_INT_TABLE_DEFINE_FW();
#endif


    PMC_RETURN(result);
} /* exco4m_int_chnl_clear */




/*******************************************************************************
* FUNCTION: exco4m_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   exco4m_handle     - handle to the EXCO4M instance to be operated on
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
PUBLIC PMC_ERROR exco4m_int_chnl_retrieve(exco4m_handle_t    *exco4m_handle,
                                          UINT32              link,
                                          exco4m_int_chnl_t  *filt_table_ptr,
                                          exco4m_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          i;

    PMC_ENTRY();

    result = exco4m_int_validate(exco4m_handle, link, int_table_ptr);
    
    
    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(exco4m_handle, exco4m, UTIL_GLOBAL_INT_OPTIMIZE);
    
    #ifdef EXCO4M_INT
    #undef EXCO4M_INT
    #endif
    #define EXCO4M_INT UTIL_GLOBAL_INT_RETRIEVE_SINGLE
    
    
    #ifdef EXCO4M_INT_ARRAY
    #undef EXCO4M_INT_ARRAY
    #endif
    #define EXCO4M_INT_ARRAY UTIL_GLOBAL_INT_RETRIEVE_ARRAY_SINGLE
    
    

    EXCO4M_INT_TABLE_DEFINE();
    
   
#ifdef DIGI_FW_INTR_MON
/* Interrupts are in FW */
#else
    EXCO4M_INT_TABLE_DEFINE_FW();
#endif

    
    #ifdef EXCO4M_STATUS
    #undef EXCO4M_STATUS
    #endif
    #define EXCO4M_STATUS UTIL_GLOBAL_STATUS_RETRIEVE_SINGLE
    
    #ifdef EXCO4M_STATUS_ARRAY
    #undef EXCO4M_STATUS_ARRAY
    #endif
    #define EXCO4M_STATUS_ARRAY UTIL_GLOBAL_STATUS_RETRIEVE_ARRAY_SINGLE
    

    EXCO4M_STATUS_TABLE_DEFINE();
    
    UTIL_GLOBAL_INT_BUFFER_FLUSH(exco4m_handle, exco4m);


    PMC_RETURN(result);
} /* exco4m_int_chnl_retrieve */



/*******************************************************************************
* FUNCTION: exco4m_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   exco4m_handle     - handle to the EXCO4M instance to be operated on
*   link              - Link
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
PUBLIC PMC_ERROR exco4m_int_chnl_enabled_check(exco4m_handle_t     *exco4m_handle,
                                               UINT32               link,
                                               exco4m_int_chnl_t   *int_en_table_ptr,
                                               BOOL8                *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          i;

    PMC_ENTRY();

    result = exco4m_int_validate(exco4m_handle, link, int_found_ptr);
    

    #ifdef EXCO4M_INT
    #undef EXCO4M_INT
    #endif
    #define EXCO4M_INT UTIL_GLOBAL_INT_CHECK_SINGLE
    
    #ifdef EXCO4M_INT_ARRAY
    #undef EXCO4M_INT_ARRAY
    #endif
    #define EXCO4M_INT_ARRAY UTIL_GLOBAL_INT_CHECK_ARRAY_SINGLE
    

    EXCO4M_INT_TABLE_DEFINE();

   
#ifdef DIGI_FW_INTR_MON
/* Interrupts are in FW */
#else
    EXCO4M_INT_TABLE_DEFINE_FW();
#endif


    PMC_RETURN(result);
} /* exco4m_int_chnl_enabled_check */

/*
** Private Functions
*/

/*
** End of file
*/
