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

#include "gsup43_c73_loc.h"

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
const char GSUP43_C73_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    GSUP43_C73_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char GSUP43_C73_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Public Functions
*/

/*******************************************************************************
* gsup43_c73_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a GSUP43_C73 block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the GSUP43_C73 block relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - pointer to TSB name
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   gsup43_c73_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC gsup43_c73_handle_t *gsup43_c73_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name )
{
    gsup43_c73_handle_t *gsup43_c73_handle;

    PMC_ENTRY();

    gsup43_c73_handle = (gsup43_c73_handle_t*)PMC_CTXT_CALLOC(sizeof(gsup43_c73_handle_t), parent);

    pmc_handle_init( parent, gsup43_c73_handle, sys_handle, PMC_MID_DIGI_GSUP43_C73, tsb_name, base_address );

    /* Register log strings. */
    pmc_log_block_strings_register(GSUP43_C73_LOG_ERR_STRINGS[0], GSUP43_C73_LOG_ERR_TABLE_BASE, GSUP43_C73_LOG_ERR_COUNT);

    PMC_RETURN(gsup43_c73_handle);
} /* gsup43_c73_ctxt_create */

/*******************************************************************************
* gsup43_c73_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a GSUP43_C73 block instance.
*
*
* INPUTS:
*   *gsup43_c73_handle        - pointer to GSUP43_C73 handle instance
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
PUBLIC void gsup43_c73_ctxt_destroy(gsup43_c73_handle_t *gsup43_c73_handle)
{
    PMC_ENTRY();

    PMC_CTXT_FREE(&gsup43_c73_handle, gsup43_c73_handle);

    PMC_RETURN();

} /* gsup43_c73_ctxt_destroy */

/*******************************************************************************
* gsup43_c73_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a GSUP43_C73 block handle instance.
*
*
* INPUTS:
*   *gsup43_c73_handle        - pointer to GSUP43_C73 handle instance
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
PUBLIC void gsup43_c73_handle_init(gsup43_c73_handle_t *gsup43_c73_handle)
{
    PMC_ENTRY();
    gsup43_c73_var_default_init(gsup43_c73_handle);
    PMC_RETURN();
} /* gsup43_c73_handle_init */

/*******************************************************************************
* gsup43_c73_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on gsup43_c73.
*
*
* INPUTS:
*   *gsup43_c73_handle    - pointer to GSUP43_C73 handle instance
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
PUBLIC PMC_ERROR gsup43_c73_handle_restart_init(gsup43_c73_handle_t *gsup43_c73_handle,
                                                util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                                pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(GSUP43_C73_LOG_ERR_STRINGS[0], GSUP43_C73_LOG_ERR_TABLE_BASE, GSUP43_C73_LOG_ERR_COUNT);
    }

    PMC_RETURN(result);
} /* gsup43_c73_handle_restart_init */

/*******************************************************************************
* gsup43_c73_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the gsup43_c73 instance to the starting state for all
*   dynamic context.  All dynamic context will be returned to the initialization 
*   values that were generate during the call to gsup43_c73_handle_init().
*
*   This function is for use by parent modules that have provided all other
*   provisions for ensuring the state of all resources tracked by this
*   context has coherent register state.
*
*
* INPUTS:
*   *hndl                 - pointer to gsup43_c73 handle instance
*   energy_state          - enum defining state to set the GSUP43_C73 module to
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
PUBLIC PMC_ERROR gsup43_c73_init(gsup43_c73_handle_t *hndl, pmc_energy_state_t energy_state)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    PMC_ASSERT(hndl!=NULL, GSUP43_C73_ERR_INVALID_ARGUMENTS, 0, 0); 
    
    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        /* Configure GSUP43_C73 energy state set                        RX    TX */
        gsup43_c73_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RUNNING, TRUE, TRUE);
    }
    else if (PMC_ENERGY_STATE_RESET == energy_state)
    {
        /* Configure GSUP43_C73 energy state set                      RX     TX */
        gsup43_c73_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RESET, TRUE, TRUE);
        gsup43_c73_var_default_init(hndl);

    } else {
        PMC_ASSERT(FALSE, GSUP43_C73_ERR_INVALID_ARGUMENTS, 0, 0);
    }    
  
    PMC_RETURN(ret_val);

} /* gsup43_c73_init */


/*******************************************************************************
* gsup43_c73_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   gsup43_c73 instance. Should TRUE be returned it can be assumed that no
*   resources are provisioned against the cfc fifo and it would be possible for
*   the higher layer module to put the fifo in lowpower standby mode of
*   operation.
*
* INPUTS:
*   *hndl                 - pointer to GSUP43_C73 handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL                  - FALSE:  Resource(s) not in start state \n
*                           TRUE:   Resources all in start state
*
* NOTES:
* PMC_ENTRY/PMC_RETURN not required here because of the number of times this
* function gets called.
*
*******************************************************************************/
PUBLIC BOOL gsup43_c73_start_state_test(gsup43_c73_handle_t *hndl)
{
    BOOL result = TRUE;

    PMC_ASSERT(hndl!=NULL, GSUP43_C73_ERR_INVALID_ARGUMENTS, 0, 0);

    if (hndl->var.tx_enable == TRUE || hndl->var.rx_enable == TRUE)
    {
        result = FALSE;
    }

    return(result);
} /* gsup3_c73_start_state_test */

/*******************************************************************************
*  gsup43_c73_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize gsup43_c73 var_t variables.
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
PUBLIC void gsup43_c73_var_default_init(gsup43_c73_handle_t *hndl)
{
    PMC_ENTRY();

    hndl->var.rx_enable = FALSE;
    hndl->var.tx_enable = FALSE;

    PMC_RETURN();
}/* gsup43_c73_var_default_init*/

/*******************************************************************************
* gsup43_c73_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of a GSUP43_C73 block instance.
*
*
* INPUTS:
*   *gsup43_c73_handle        - pointer to GSUP43_C73 handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Base address of GSUP43_C73.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 gsup43_c73_base_address_get(gsup43_c73_handle_t *gsup43_c73_handle)
{
    PMC_ENTRY();

    PMC_RETURN(gsup43_c73_handle->base.base_address);
} /* gsup43_c73_base_address_get */

/*******************************************************************************
* Initialization Configuration Functions
*******************************************************************************/
/*******************************************************************************
*  gsup43_c73_rx_en
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable/disable operation in Rx direction
*
* INPUTS:
*   hndl       - handle to the GSUP43_C73 instance to be operated on
*   enable     - TRUE: enable operation. FALSE: disable operation
*
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR gsup43_c73_rx_en(gsup43_c73_handle_t *hndl, BOOL enable)
{
    PMC_ENTRY();

    /* Enable/disable Rx path */
    gsup43_c73_field_RX_GSUP43_C73_EN_set(NULL, hndl, enable);
    if (TRUE == enable) 
    {
        gsup43_c73_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RUNNING, TRUE, FALSE);
    } else 
    {
        gsup43_c73_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RESET, TRUE,FALSE);
    }
    hndl->var.rx_enable = enable;

    PMC_RETURN(PMC_SUCCESS);

} /* gsup43_c73_rx_en */
/*******************************************************************************
* gsup43_c73_pcs_decoder_en
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables GSUP43_C73 receive half as a 64B/66B PCS decoder or as a 
*   per Gsup43_Clause 7.3 clause.
*
*
* INPUTS:
*   *hndl    - pointer to GSUP43_C73 handle instance.
*    enable  - when TRUE, block acts as a 64/66 B PCS decoder
*              when FALSE, block acts as per Gsup43_Clause 7.3 clause.    
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
PUBLIC void gsup43_c73_pcs_decoder_en(gsup43_c73_handle_t *hndl, 
                                      BOOL                 enable)
{
    PMC_ENTRY();
  
    /* Enable/disable Rx path */
    gsup43_c73_field_RX_GSUP43_C73_EN_set(NULL, hndl, FALSE);

    if (TRUE == enable)
    {
        gsup43_c73_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RUNNING, TRUE, FALSE);
    }
    else
    {
        gsup43_c73_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RESET, TRUE, FALSE);
    }
    hndl->var.rx_enable = enable;

    PMC_RETURN();

} /* gsup43_c73_pcs_decoder_en */

/*******************************************************************************
*  gsup43_c73_rx_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable operation in Rx direction
*
* INPUTS:
*   hndl       - handle to the GSUP43_C73 instance to be operated on
*   losInv      - configures the assertive state of the
*                LOS input signal
*                0 : LOS is expected to be asserted when the
*                    corresponding input signal is logic 1
*                1 : LOS is expected to be asserted when the
*                    corresponding input signal is logic 0
*                Default setting 0
*   dscrdZone  - sets the fill level of the downstream packet
*                buffer at which ordered set discard mechanism
*                is enabled.
*                b'00 - b'11
*                Default setting b'10
*   sigEn      - enables the discard mechanism for signal
*                ordered sets
*                0 : Signal ordered sets are not considered in
*                    the discard rate adaptation mechanism
*                1 : Signal ordered sets are considered in the
*                    discard rate adaptation mechanism
*                Default setting 0
*   seqEn      - enables the discard mechanism for
*                sequence ordered sets
*                0 : Sequence ordered sets are not considered
*                    in the discard rate adaptation mechanism
*                1 : Sequence ordered sets are considered
*                    in the discard rate adaptation mechanism
*                Default setting 1
*   allDscrd   - enables the discard of all ordered sets
*                0 : specific ordered sets in the received
*                    data path are discarded
*                1 : All ordered sets in the received data path
*                    are discarded
*                Default setting 0
*   osPrtDis   - disables the function of RX_MIN_OS_RPT[3:0]
*                when the downstream packet buffer is full
*                0 : RX_MIN_OS_RPT is always regarded
*                1 : RX_MIN_OS_RPT is ignored when the
*                    downstream packer buffer is full
*                Default setting 0
*   minOsPrt   - sets the minimum number of consecutive
*                identical ordered sets on the received
*                data path that must be detected before they
*                are eligible for discard
*                0 : ordered set may be discarded, based on the
*                    fill level of the downstream buffer (CPB)
*                N : the previous N-1 received ordered sets must
*                    be identical to the current ordered set
*                    before the current ordered set is considered
*                    eligible for the discard.
*                Default setting b'0011
*   pktType    - identifies the received data as an 10GE data
*                frame as opposed to an ordered set
*                Default setting b'11111101
*   osType     - identifies the received data as an 10GE ordered
*                set as opposed to a data packet
*                Default setting b'11111110
*   rx_125us_cnt - number of receive clock cycles needed to equal 125/2 microseconds of
*                  time. This value is used for the RX_BER state machine
*
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR gsup43_c73_rx_cfg(gsup43_c73_handle_t *hndl,
                                   UINT32 losInv,   UINT32 dscrdZone, UINT32 sigEn,
                                   UINT32 seqEn,    UINT32 allDscrd,  UINT32 osPrtDis,
                                   UINT32 minOsPrt, UINT32 pktType,   UINT32 osType,
                                   UINT32 rx_125us_cnt)
{
    gsup43_c73_buffer_t gsup43_c73_buffer[1];

    PMC_ENTRY();

    /* Configure Rx path */
    /* Register 0x1: GSUP43_C73 - Rx Configuration */
    /* Register 0x2: GSUP43_C73 - RX Gsup43 Configuration */
    /* Default values used by PV in HyPHY:
       losInv          0
       dscrdZone       2
       sigEn           0
       seqEn           1
       allDscrd        0
       osPrtDis        0
       minOsPrt        3
       pktType       0xfd
       osType        0xfe
    */

    gsup43_c73_buffer_init(gsup43_c73_buffer, hndl);
    
    gsup43_c73_field_LOS_INV_set(gsup43_c73_buffer, hndl, losInv);
    gsup43_c73_field_RX_SW_RST_set(gsup43_c73_buffer, hndl, 0);
    gsup43_c73_field_RX_OS_TYPE_set(gsup43_c73_buffer, hndl, osType);
    gsup43_c73_field_RX_PKT_TYPE_set(gsup43_c73_buffer, hndl, pktType);
    gsup43_c73_field_RX_MIN_OS_RPT_set(gsup43_c73_buffer, hndl, minOsPrt);
    gsup43_c73_field_RX_FULL_OS_RPT_DIS_set(gsup43_c73_buffer, hndl, osPrtDis);
    gsup43_c73_field_RX_OS_ALL_DSCRD_set(gsup43_c73_buffer, hndl, allDscrd);
    gsup43_c73_field_RX_SEQ_OS_DSCRD_EN_set(gsup43_c73_buffer, hndl, seqEn);
    gsup43_c73_field_RX_SIG_OS_DSCRD_EN_set(gsup43_c73_buffer, hndl, sigEn);
    gsup43_c73_field_DSCRD_ZONE_set(gsup43_c73_buffer, hndl, dscrdZone);
    gsup43_c73_field_RX_GSUP43_C73_EN_set(gsup43_c73_buffer, hndl, 0);
    gsup43_c73_buffer_flush(gsup43_c73_buffer);
   
    PMC_RETURN(PMC_SUCCESS);

} /* gsup43_c73_rx_cfg */

/*******************************************************************************
*  gsup43_c73_tx_en
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable/disable operation in Tx direction
*
* INPUTS:
*   hndl       - handle to the GSUP43_C73 instance to be operated on
*   enable     - TRUE: enable operation. FALSE: disable operation
*
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR gsup43_c73_tx_en(gsup43_c73_handle_t *hndl, BOOL enable)
{
    PMC_ENTRY();

    /* Enable/disable Tx path */
    gsup43_c73_field_TX_GSUP43_C73_EN_set(NULL, hndl, enable);
    if (TRUE == enable) 
    {
        gsup43_c73_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RUNNING, FALSE, TRUE);
    } else 
    {
      gsup43_c73_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RESET, FALSE,TRUE);
    }
    hndl->var.tx_enable = enable;

    PMC_RETURN(PMC_SUCCESS);

} /* gsup43_c73_tx_en */

/*******************************************************************************
*  gsup43_c73_tx_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable operation in Tx direction
*
* INPUTS:
*   hndl       - handle to the GSUP43_C73 instance to be operated on
*   errDrop    - when incoming packet has an UPI TYPE value
*                that does not match either TX_PKT_TYPE or
*                TX_OS_TYPE:
*                0 : incoming data packet treated as a
*                    data packet
*                1 : incoming packet will be dropped
*                Default setting 1
*   pktType    - identifies the transmit data as a 10GE
*                data frame, as opposed to an ordered set.
*                this value should be configured to match
*                the UPI value of the GFP-F frame
*                Default setting b'11111101
*   osType     - identifies the transmit data as 10GE ordered
*                set, as opposed to a data packet.
*                this value should be configured to match
*                the UPI value of the GFP-F frame
*                Default setting b'11111110
*   forceMinIpg- forces the minimum IPG value to 0 bytes
*                when the downstream packet buffer is full
*                0 : average minimum IPG is set to 12 bytes
*                    or 8 bytes
*                1 : IPG is set to 0 bytes and IPG insertion
*                    must be controlled by the upstream source
*                Default setting 0
*   unidirEn   - enable/disable the unidirectional enable
*                parameter from IEEE 802.3
*                0 : Unidirectional operation is disabled
*                1 : Unidirectional operation is enabled
*                Default setting 0
*   sendIdleEn - enable/disable idle generation when RF ordered
*                sets are detected in the receive PCS
*                0 : disable idle generation
*                1 : enable idle generation
*                Default setting 1
*   sendLfEn   - enable/disable remote fault or local fault status
*                (Idle control characters) when a remote fault or local fault is detected.
*                0 : disable generation
*                1 : enable generation
*                Default setting 0
*   sendRfEn   - enable/disable the remote fault ordered set
*                when a local fault is detected in the receive PCS
*                0 : disable generation
*                1 : enable generation
*                Default setting 1
*   fifoHmark  - identifies the number of 16-byte stored in
*                the internal FIFO which transfers from
*                the system interface
*                Default setting b'00110
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR gsup43_c73_tx_cfg(gsup43_c73_handle_t *hndl,
                                   UINT32 errDrop,     UINT32 pktType, UINT32 osType,
                                   UINT32 forceMinIpg, UINT32 unidirEn,
                                   UINT32 sendIdleEn,  UINT32 sendLfEn,
                                   UINT32 sendRfEn,    UINT32 fifoHmark)
{
    gsup43_c73_buffer_t gsup43_c73_buffer[1];

    PMC_ENTRY();

    /* Configure Tx path */
    /* Register 0x10: GSUP43_C73 - TX Configuration 1 */
    /* Register 0x11: GSUP43_C73 - TX Configuration 2 */
    /* Default values used by PV in HyPHY:
       errDrop         0
       pktType       0xfd
       osType        0xfe
       forceMinIpg     0
       unidirEn        1
       sendIdleEn      0
       sendLfEn        0
       sendRfEn        0
       fifoHmark     0x6
     */

    gsup43_c73_buffer_init(gsup43_c73_buffer, hndl);
    /* TX configuration 1 */
    gsup43_c73_field_TX_OS_TYPE_set(gsup43_c73_buffer, hndl, osType);
    gsup43_c73_field_TX_PKT_TYPE_set(gsup43_c73_buffer, hndl, pktType);
    gsup43_c73_field_TX_ERR_DROP_set(gsup43_c73_buffer, hndl, errDrop);
    gsup43_c73_field_TX_SW_RST_set(gsup43_c73_buffer, hndl, 0);
   
    /* TX configuration 2 */
    gsup43_c73_field_TX_GSUP43_C73_EN_set(gsup43_c73_buffer, hndl, 0);
    gsup43_c73_field_FORCE_MIN_IPG_set(gsup43_c73_buffer, hndl, forceMinIpg);
    gsup43_c73_field_UNIDIR_EN_set(gsup43_c73_buffer, hndl, unidirEn);
    gsup43_c73_field_SEND_IDLE_EN_set(gsup43_c73_buffer, hndl, sendIdleEn);
    gsup43_c73_field_SEND_LF_EN_set(gsup43_c73_buffer, hndl, sendLfEn);
    gsup43_c73_field_SEND_RF_EN_set(gsup43_c73_buffer, hndl, sendRfEn);
    gsup43_c73_field_PRBS31_EN_set(gsup43_c73_buffer, hndl, 0);
    gsup43_c73_field_FIFO_HMARK_set(gsup43_c73_buffer, hndl, fifoHmark);

    gsup43_c73_buffer_flush(gsup43_c73_buffer);

    PMC_RETURN(PMC_SUCCESS);

} /* gsup43_c73_tx_cfg */


/*******************************************************************************
* Run-time Operations
*******************************************************************************/
/*******************************************************************************
*  gsup43_c73_tx_prbs_en
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable/disable PRBS generation in Tx direction
*   to replace the packet data on the transmit data stream.
*
* INPUTS:
*   hndl       - handle to the GSUP43_C73 instance to be operated on
*   enable     - TRUE: enable operation. FALSE: disable operation
*
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR gsup43_c73_tx_prbs_en(gsup43_c73_handle_t *hndl, BOOL enable)
{
    PMC_ENTRY();

    /* Enable/disable PRBS generation in Tx path */
    gsup43_c73_field_PRBS31_EN_set(NULL, hndl, enable);

    PMC_RETURN(PMC_SUCCESS);

} /* gsup43_c73_tx_prbs_en */

/*******************************************************************************
*  gsup43_c73_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the gsup43_c73 block energy state. 
*
* INPUTS:
*   hndl           - handle to the gsup43_c73 instance to be operated on
*   energy_state   - energy_state
*   rx             - boolean enable for Rx reset
*   tx             - boolean enable for Tx reset
*
* OUTPUTS:
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PUBLIC void gsup43_c73_energy_state_reg_set(gsup43_c73_handle_t *hndl,
                                            pmc_energy_state_t energy_state,
                                            BOOL rx, BOOL tx)
{
    PMC_ENTRY();    
    PMC_ASSERT(hndl!=NULL, GSUP43_C73_ERR_INVALID_ARGUMENTS, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            gsup43_c73_field_SW_RST_set   (NULL, hndl, 0);
            
            if(rx == TRUE){
                gsup43_c73_field_RX_SW_RST_set(NULL, hndl, 0);
            }

            if(tx == TRUE){
                gsup43_c73_field_TX_SW_RST_set(NULL, hndl, 0);
            }
            break;
        case PMC_ENERGY_STATE_RESET:
            gsup43_c73_field_SW_RST_set   (NULL, hndl, 1);

            if(rx == TRUE){
                gsup43_c73_field_RX_SW_RST_set(NULL, hndl, 1);
            }

            if(tx == TRUE){
                gsup43_c73_field_TX_SW_RST_set(NULL, hndl, 1);
            }
            break;
        default:
            PMC_ASSERT(FALSE, GSUP43_C73_ERR_INVALID_ARGUMENTS, 0, 0);
    }

    PMC_RETURN();
} /* gsup43_c73_energy_state_reg_set */


/*******************************************************************************
*  gsup43_c73_align_poll
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Poll frame synchronization state machine
*
* INPUTS:
*   hndl     - handle to the GSUP43_C73
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
PUBLIC PMC_POLL_RETURN_TYPE gsup43_c73_los_sync_poll(gsup43_c73_handle_t *hndl)
{
    UINT32 num_failed_polls;
    PMC_POLL_RETURN_TYPE poll_res;

    PMC_ENTRY();

    poll_res = (PMC_POLL_RETURN_TYPE)gsup43_c73_field_HI_BER_V_poll(NULL, hndl, 
                                                                        0, 
                                                                        PMC_POLL_EQUAL,
                                                                        10, 
                                                                        &num_failed_polls, 
                                                                        10);
    PMC_RETURN(poll_res);

} /* gsup43_c73_los_sync_poll */

/*******************************************************************************
*  gsup43_c73_tx_upi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the expected UPI values in data or ordered set packet and the action 
*   to be done when incoming packet UPI does not match expected values: DROP or IGNORE.
*
* INPUTS:
*   hndl                - handle to the GSUP43_C73 instance to be operated on
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
PUBLIC PMC_ERROR gsup43_c73_tx_upi_set(gsup43_c73_handle_t *hndl,                               
                                       UINT8                pkt_upi_val,
                                       UINT8                os_upi_val,
                                       BOOL8                enable_drop)
{
    gsup43_c73_buffer_t buf[1];

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, GSUP43_C73_ERR_INVALID_ARGUMENTS, 0, 0); 

    gsup43_c73_buffer_init(buf, hndl);

    /* set TX_OS_TYPE, TX_PKT_TYPE, TX_OS_TYPE */
    gsup43_c73_field_TX_ERR_DROP_set(buf, hndl, TRUE == enable_drop ? 1 : 0);
    gsup43_c73_field_TX_PKT_TYPE_set(buf, hndl, pkt_upi_val);
    gsup43_c73_field_TX_OS_TYPE_set(buf, hndl, os_upi_val);

    gsup43_c73_buffer_flush(buf);
    PMC_RETURN(PMC_SUCCESS);

} /* gsup43_c73_tx_upi_set */

/*******************************************************************************
*  gsup43_c73_tx_upi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the expected UPI values in data or ordered set packet and the action 
*   to be done when incoming packet UPI does not match expected values: DROP or IGNORE.
*
* INPUTS:
*   hndl                - handle to the GSUP43_C73 instance to be operated on
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
PUBLIC PMC_ERROR gsup43_c73_tx_upi_get(gsup43_c73_handle_t *hndl,                               
                                       UINT8               *pkt_upi_val_ptr,
                                       UINT8               *os_upi_val_ptr,
                                       BOOL8               *enable_drop_ptr)
{
    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, GSUP43_C73_ERR_INVALID_ARGUMENTS, 0, 0); 
    PMC_ASSERT(NULL != pkt_upi_val_ptr, GSUP43_C73_ERR_INVALID_ARGUMENTS, 0, 0); 
    PMC_ASSERT(NULL != os_upi_val_ptr, GSUP43_C73_ERR_INVALID_ARGUMENTS, 0, 0); 
    PMC_ASSERT(NULL != enable_drop_ptr, GSUP43_C73_ERR_INVALID_ARGUMENTS, 0, 0); 

    /* set TX_OS_TYPE, TX_PKT_TYPE, TX_OS_TYPE */
    *enable_drop_ptr = gsup43_c73_field_TX_ERR_DROP_get(NULL, hndl);
    *pkt_upi_val_ptr = gsup43_c73_field_TX_PKT_TYPE_get(NULL, hndl);
    *os_upi_val_ptr = gsup43_c73_field_TX_OS_TYPE_get(NULL, hndl);

    PMC_RETURN(PMC_SUCCESS);

} /* gsup43_c73_tx_upi_get */

/*******************************************************************************
*  gsup43_c73_rx_upi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the UPI values for data and ordered set packets.
*
* INPUTS:
*   hndl                - GSUP43_C73 handle 
*   pkt_upi_val         - data frame UPI
*   os_upi_val          - orderred set UPI
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR gsup43_c73_rx_upi_set(gsup43_c73_handle_t *hndl,                               
                                       UINT8                pkt_upi_val,
                                       UINT8                os_upi_val)
{
    gsup43_c73_buffer_t buf[1];

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, GSUP43_C73_ERR_INVALID_ARGUMENTS, 0, 0); 

    gsup43_c73_buffer_init(buf, hndl);

    /* set RX_OS_TYPE, RX_PKT_TYPE */
    gsup43_c73_field_RX_PKT_TYPE_set(buf, hndl, pkt_upi_val);
    gsup43_c73_field_RX_OS_TYPE_set(buf, hndl, os_upi_val);

    gsup43_c73_buffer_flush(buf);
    PMC_RETURN(PMC_SUCCESS);

} /* gsup43_c73_rx_upi_set */

/*******************************************************************************
*  gsup43_c73_rx_upi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the configured UPI values for data and ordered set 
*   frames.
*
* INPUTS:
*   hndl                - GSUP43_C73 handle
*
* OUTPUTS:
*  *pkt_upi_val_ptr     - RX data frame UPI 
*  *os_upi_val_ptr      - RX ordered set UPI 
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR gsup43_c73_rx_upi_get(gsup43_c73_handle_t *hndl,                               
                                       UINT8 *pkt_upi_val_ptr,
                                       UINT8 *os_upi_val_ptr)
{
    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, GSUP43_C73_ERR_INVALID_ARGUMENTS, 0, 0); 
    PMC_ASSERT(NULL != pkt_upi_val_ptr, GSUP43_C73_ERR_INVALID_ARGUMENTS, 0, 0); 
    PMC_ASSERT(NULL != os_upi_val_ptr, GSUP43_C73_ERR_INVALID_ARGUMENTS, 0, 0); 

    /* Retrieve RX_OS_TYPE and RX_PKT_TYPE */
    *pkt_upi_val_ptr = gsup43_c73_field_RX_PKT_TYPE_get(NULL, hndl);
    *os_upi_val_ptr = gsup43_c73_field_RX_OS_TYPE_get(NULL, hndl);

    PMC_RETURN(PMC_SUCCESS);

} /* gsup43_c73_rx_upi_get */



/*******************************************************************************
* FUNCTION: gsup43_c73_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   gsup43_c73_handle - GSUP43_C73 handle
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
PUBLIC PMC_ERROR gsup43_c73_int_validate(gsup43_c73_handle_t    *gsup43_c73_handle,
                                         UINT32                  link,
                                         void                   *non_null_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != gsup43_c73_handle, GSUP43_C73_ERR_INVALID_ARGUMENTS, 0, 0);
   
    if (link >= GSUP43_C73_NUM_LINKS)
    {
        result = GSUP43_C73_ERR_INVALID_ARGUMENTS;
    }

    if (NULL == non_null_ptr)
    {
        result = GSUP43_C73_ERR_INVALID_ARGUMENTS;
    }


    PMC_RETURN(result);
} /* gsup43_c73_int_validate */



/*******************************************************************************
* FUNCTION: gsup43_c73_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   gsup43_c73_handle - GSUP43_C73 handle
*   link              - Link (unused)
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
PUBLIC PMC_ERROR gsup43_c73_int_chnl_enable(gsup43_c73_handle_t    *gsup43_c73_handle,
                                            UINT32                  link,
                                            gsup43_c73_int_chnl_t  *int_table_ptr,
                                            gsup43_c73_int_chnl_t  *int_en_table_ptr,
                                            BOOL8                   enable)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    result = gsup43_c73_int_validate(gsup43_c73_handle, link, int_en_table_ptr);


    #ifdef GSUP43_INT_SINGLE
    #undef GSUP43_INT_SINGLE
    #endif
    #define GSUP43_INT_SINGLE    UTIL_GLOBAL_INT_ENABLE_SINGLE

    #ifdef GSUP43_INT_SINGLE_IC
    #undef GSUP43_INT_SINGLE_IC
    #endif
    #define GSUP43_INT_SINGLE_IC    UTIL_GLOBAL_INT_ENABLE_SINGLE


    GSUP43_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* gsup43_c73_int_chnl_enable */




/*******************************************************************************
* FUNCTION: gsup43_c73_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   gsup43_c73_handle - GSUP43_C73 handle
*   link              - Link (unused)
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
PUBLIC PMC_ERROR gsup43_c73_int_chnl_clear(gsup43_c73_handle_t    *gsup43_c73_handle,
                                           UINT32                  link,
                                           gsup43_c73_int_chnl_t  *int_table_ptr,
                                           gsup43_c73_int_chnl_t  *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    result = gsup43_c73_int_validate(gsup43_c73_handle, link, int_en_table_ptr);
    

    #ifdef GSUP43_INT_SINGLE
    #undef GSUP43_INT_SINGLE
    #endif
    #define GSUP43_INT_SINGLE UTIL_GLOBAL_INT_CLEAR_SINGLE

    #ifdef GSUP43_INT_SINGLE_IC
    #undef GSUP43_INT_SINGLE_IC
    #endif
    #define GSUP43_INT_SINGLE_IC    GSUP43_INT_CLEAR_SINGLE
    

    GSUP43_INT_TABLE_DEFINE();


    PMC_RETURN(result);
} /* gsup43_c73_int_chnl_clear */




/*******************************************************************************
* FUNCTION: gsup43_c73_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   gsup43_c73_handle - GSUP43_C73 handle
*   link              - Link (unused)
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
PUBLIC PMC_ERROR gsup43_c73_int_chnl_retrieve(gsup43_c73_handle_t    *gsup43_c73_handle,
                                              UINT32                  link,
                                              gsup43_c73_int_chnl_t  *filt_table_ptr,
                                              gsup43_c73_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    result = gsup43_c73_int_validate(gsup43_c73_handle, link, int_table_ptr);
    

    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(gsup43_c73_handle, gsup43_c73, UTIL_GLOBAL_INT_OPTIMIZE);
    

    #ifdef GSUP43_INT_SINGLE
    #undef GSUP43_INT_SINGLE
    #endif
    #define GSUP43_INT_SINGLE UTIL_GLOBAL_INT_RETRIEVE_SINGLE
    
    #ifdef GSUP43_INT_SINGLE_IC
    #undef GSUP43_INT_SINGLE_IC
    #endif
    #define GSUP43_INT_SINGLE_IC    GSUP43_INT_RETRIEVE_SINGLE
    

    GSUP43_INT_TABLE_DEFINE();
    
    
    #ifdef GSUP43_STATUS_SINGLE
    #undef GSUP43_STATUS_SINGLE
    #endif
    #define GSUP43_STATUS_SINGLE UTIL_GLOBAL_STATUS_RETRIEVE_SINGLE
    

    GSUP43_STATUS_TABLE_DEFINE();
    
    
    UTIL_GLOBAL_INT_BUFFER_FLUSH(gsup43_c73_handle, gsup43_c73);


    PMC_RETURN(result);
} /* gsup43_c73_int_chnl_retrieve */



/*******************************************************************************
* FUNCTION: gsup43_c73_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   gsup43_c73_handle - GSUP43_C73 handle
*   link              - Link (unused)
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
PUBLIC PMC_ERROR gsup43_c73_int_chnl_enabled_check(gsup43_c73_handle_t     *gsup43_c73_handle,
                                                   UINT32                   link,
                                                   gsup43_c73_int_chnl_t   *int_en_table_ptr,
                                                   BOOL8                   *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    result = gsup43_c73_int_validate(gsup43_c73_handle, link, int_found_ptr);
    

    #ifdef GSUP43_INT_SINGLE
    #undef GSUP43_INT_SINGLE
    #endif
    #define GSUP43_INT_SINGLE UTIL_GLOBAL_INT_CHECK_SINGLE
    
    #ifdef GSUP43_INT_SINGLE_IC
    #undef GSUP43_INT_SINGLE_IC
    #endif
    #define GSUP43_INT_SINGLE_IC    GSUP43_INT_CHECK_SINGLE
    

    GSUP43_INT_TABLE_DEFINE();


    PMC_RETURN(result);
} /* gsup43_c73_int_chnl_enabled_check */


/*******************************************************************************
* FUNCTION: gsup43_c73_TX_SW_RST_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Writes to the TX_SW_RST bit.
*   
*
* INPUTS:
*   h_ptr - GSUP43_C73 handle
*   value - value to write to this register
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR gsup43_c73_TX_SW_RST_set(gsup43_c73_handle_t *h_ptr,
                                           UINT32 value )  
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    gsup43_c73_field_TX_SW_RST_set( NULL, h_ptr, value);

    PMC_RETURN(result);
} /* gsup43_c73_TX_SW_RST_set */



/*
** Private Functions
*/

/*
** End of file
*/
