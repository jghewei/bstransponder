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
** Public Functions
*/

/*******************************************************************************
*  emac_intr_enable_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read specified status.
*
* INPUTS:
*   hndl        - handle to the EMAC instance to be operated on
*   link        - link to configure
*   status_type - interrupt status type to enable
*   enable      - TRUE: Enable interrupt
*                 FALSE: Disable interrupt
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
PUBLIC PMC_ERROR emac_intr_enable_set(emac_handle_t *hndl, UINT32 link, emac_intr_status_t status_type, BOOL enable)
{
    PMC_ENTRY();

    /* 
     * Configure interrupt enable in registers 0x60-0x6C, 0x70, 0x78 & 0x7C
     */

    /* Serial number #4 
       Module: EMAC 
       Register : 0x10060 EMAC Ingress PCS interrupt En. (1/4)
       Field: R/W RX_LOC_FAULT_E [11:0]
       Field: R/W RX_REM_FAULT_E[11:0]
       Description/Action: each bit represent PCS out Interrupt en into the SS, set all bits to 0x1.*/
    emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__write( NULL, hndl, 0xffffff ); 

    /* Serial number #5
       Module: EMAC
       Register : 0x10064 EMAC Ingress PCS interrupt En. (2/4)
       Field: R/W PHY_LOS_E[11:0]
       Description/Action: each bit represent PCS out Interrupt en into the SS, set all bits to 0x1. */
    emac_reg_EMAC_INGRESS_PCS_INT_EN_2_4__write( NULL, hndl, 0xfff );

    /* Serial number #6
       Module: EMAC
       Register : 0x10068 EMAC Ingress PCS interrupt En. (3/4)
       Field: R/W HI_BER_E[11:0]
       Description/Action: each bit represent PCS out Interrupt en into the SS, set all bits to 0x1. */
    emac_reg_EMAC_INGRESS_PCS_INT_EN_3_4__write( NULL, hndl, 0xfff );   

    /* Serial number #7
       Module: EMAC
       Register : 0x1006C EMAC Ingress PCS interrupt En. (4/4)
       Field: R/W BLOCK_LOCK_E[11:0] (bits 0..11)
       Field: R/W ALIGN_STATUS_E[2:0] (bits 16:18)
       Description/Action: each bit represent PCS out Interrupt en into the SS, set all bits to 0x1. */
    emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__write( NULL, hndl, 0x70fff );

    /* Serial number #8
       Module: EMAC
       Register : 0x10070 EMAC AGB Overflow interrupt En.
       Field: R/W ING_ABG_OVERFLOW_E[11:0] (bits 23..12)
       Field: R/W EGR_ABG_OVERFLOW _E[11:0] (bits 11:0)
       Description/Action:  set all bits to 0x1 */
    emac_reg_AGB_OVERFLOW_INT_EN_write( NULL, hndl, 0xffffff );

    /* Serial number #9
       Module: EMAC
       Register : 0x10078 EMAC Egress overflow interrupt En.
       Field: R/W EMAC_TX_UNDERFLOW_E[11:0] (bits 23..12)
       Field: R/W EMAC_TX_OVERFLOW _E[11:0] (bits 11:0)
       Description/Action:  set all bits to 0x1 */
    emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_write( NULL, hndl, 0xffffff );

    /* Serial number #10
       Module: EMAC
       Register : 0x1007C EMAC  Host Timeout Interface interrupt En.
       Field: R/W EMAC_HOST_TIMEOUT_E[0] 
       Description/Action: set bit to 0x1 */
    PMC_RETURN(PMC_SUCCESS);

} /* emac_intr_enable_set */



/*******************************************************************************
*  emac_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read interrupt status and values.
*
* DESCRIPTION:
*   Read interrupt status and values
*
* INPUTS:
*   hndl        - handle to the EMAC instance to be operated on
*   link        - link to configure
*   clear_int   - flag to clear interrupt status bit
*
* OUTPUTS:
*   *status_ptr - interrupts & values  (offsets MUST match digi_enet_api.h defines)
*                 Bit 16: EMAC_TX_OVERFLOW_I  (MAC egress overflow occurred)
*                 Bit 15: ING_AGB_OVERFLOW_I  (AGB ingress overflow occurred)
*                 Bit 14: EGR_AGB_OVERFLOW_I  (AGB egress overflow occurred)
*                 Bit 13: LINK_STATUS  (link is up)
*                 Bit 12: PAUSE_STATUS  (egress is paused)
*                 Bit 11: ALIGN_STATUS_V  (loss of multi-lane alignment)
*                 Bit 10: ALIGN_STATUS_I  (multi-lane alignment transition)
*                 Bit  9: BLOCK_LOCK_V  (PCS not locked to received blocks)
*                 Bit  8: BLOCK_LOCK_I  (PCS lock transition to received blocks)
*                 Bit  7: HI_BER_V  (high BER detected)
*                 Bit  6: HI_BER_I  (high BER transition)
*                 Bit  5: RX_REM_FAULT_V  (RF detected)
*                 Bit  4: RX_REM_FAULT_I  (RF transition)
*                 Bit  3: RX_LOC_FAULT_V  (LF detected)
*                 Bit  2: RX_LOC_FAULT_I  (LF transition)
*                 Bit  1: PHY_LOS_V  (PHY LOS detected)
*                 Bit  0: PHY_LOS_I  (PHY LOS transition)
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*   *** Assuming CLEAR ON WRITE interrupt mode ***
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_status_get(emac_handle_t *hndl, UINT32 link, BOOL clear_int, UINT32 *status_ptr)
{
    UINT32 status = 0;
    PMC_ENTRY();

    if (emac_field_range_EMAC_TX_OVERFLOW_I_get(NULL, hndl, link, link)) {  /* MAC egress overflow? */
        if (clear_int)
            emac_field_range_EMAC_TX_OVERFLOW_I_set_to_clear(NULL, hndl, link, link, 1);
        status |= (1 << 16);
    }
    if (emac_field_range_ING_AGB_OVERFLOW_I_get(NULL, hndl, link, link)) {  /* AGB ingress overflow? */
        if (clear_int)
            emac_field_range_ING_AGB_OVERFLOW_I_set_to_clear(NULL, hndl, link, link, 1);
        status |= (1 << 15);
    }
    if (emac_field_range_EGR_AGB_OVERFLOW_I_get(NULL, hndl, link, link)) {  /* AGB egress overflow? */
        if (clear_int)
            emac_field_range_EGR_AGB_OVERFLOW_I_set_to_clear(NULL, hndl, link, link, 1);
        status |= (1 << 14);
    }
    if (emac_pcs_field_LINK_STATUS_get(NULL, hndl, link)) {               /* link up? */
        status |= (1 << 13);
    }
    if (emac_mac_field_range_PAUSE_STATUS_get(NULL, hndl, link, 0, 0)) {  /* egress paused (priority 0)? */
        status |= (1 << 12);
    }
    if (emac_field_range_ALIGN_STATUS_V_get(NULL, hndl, link/4, link/4)) {    /* loss of multi-lane alignment? */
        status |= (1 << 11);
    }
    if (emac_field_range_ALIGN_STATUS_I_get(NULL, hndl, link/4, link/4)) {    /* multi-lane alignment transition? */
        if (clear_int)
            emac_field_range_ALIGN_STATUS_I_set_to_clear(NULL, hndl, link/4, link/4, 1);
        status |= (1 << 10);
    }
    if (emac_field_range_BLOCK_LOCK_V_get(NULL, hndl, link, link)) {      /* PCS not locked to received blocks? */
        status |= (1 << 9);
    }
    if (emac_field_range_BLOCK_LOCK_I_get(NULL, hndl, link, link)) {      /* PCS lock transition? */
        if (clear_int)
            emac_field_range_BLOCK_LOCK_I_set_to_clear(NULL, hndl, link, link, 1);
        status |= (1 << 8);
    }
    if (emac_field_range_HI_BER_V_get(NULL, hndl, link, link)) {          /* high BER detected? */
        status |= (1 << 7);
    }
    if (emac_field_range_HI_BER_I_get(NULL, hndl, link, link)) {          /* high BER transition? */
        if (clear_int)
            emac_field_range_HI_BER_I_set_to_clear(NULL, hndl, link, link, 1);
        status |= (1 << 6);
    }
    if (emac_field_range_RX_REM_FAULT_V_get(NULL, hndl, link, link)) {    /* RF detected? */
        status |= (1 << 5);
    }
    if (emac_field_range_RX_REM_FAULT_I_get(NULL, hndl, link, link)) {    /* RF transition? */
        if (clear_int)
            emac_field_range_RX_REM_FAULT_I_set_to_clear(NULL, hndl, link, link, 1);
        status |= (1 << 4);
    }
    if (emac_field_range_RX_LOC_FAULT_V_get(NULL, hndl, link, link)) {    /* LF detected? */
        status |= (1 << 3);
    }
    if (emac_field_range_RX_LOC_FAULT_I_get(NULL, hndl, link, link)) {    /* LF transition? */
        if (clear_int)
            emac_field_range_RX_LOC_FAULT_I_set_to_clear(NULL, hndl, link, link, 1);
        status |= (1 << 2);
    }
    if (emac_field_range_PHY_LOS_V_get(NULL, hndl, link, link)) {         /* PHY LOS detected? */
        status |= (1 << 1);
    }
    if (emac_field_range_PHY_LOS_I_get(NULL, hndl, link, link)) {         /* PHY LOS transition? */
        if (clear_int)
            emac_field_range_PHY_LOS_I_set_to_clear(NULL, hndl, link, link, 1);
        status |= (1 << 0);
    }

    *status_ptr = status;

    PMC_RETURN(PMC_SUCCESS);

} /* emac_status_get */

/*******************************************************************************
*  emac_pause_stat_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read Pause status.
*
* INPUTS:
*   hndl          - handle to the EMAC instance to be operated on
*   link          - link to configure
*
* OUTPUTS:
*   *pause_stat_ptr - Pause status. Bits 7:0 indicate the status for each pause class
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_pause_stat_get(emac_handle_t *hndl, UINT32 link, UINT32 *pause_stat_ptr)
{
    PMC_ENTRY();

    /* Read status */
    *pause_stat_ptr = emac_mac_field_PAUSE_STATUS_get(NULL, hndl, link);

    PMC_RETURN(PMC_SUCCESS);

} /* emac_pause_stat_get */

/*******************************************************************************
*  emac_tx_enable_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read TX enable status.
*
* INPUTS:
*   hndl          - handle to the EMAC instance to be operated on
*   link          - link to configure
*
* OUTPUTS:
*   *enable_ptr   - TX enable status (enabled = TRUE & disabled = FALSE).
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_tx_enable_get(emac_handle_t *hndl, UINT32 link, BOOL8 *enable_ptr)
{
    PMC_ENTRY();

    /* Read status */
    *enable_ptr = emac_mac_field_TX_ENA_get(NULL, hndl, link);

    PMC_RETURN(PMC_SUCCESS);

} /* emac_tx_enable_get */

/*******************************************************************************
*  emac_max_frm_len_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read maximum supported frame length.
*
* INPUTS:
*   hndl          - handle to the EMAC instance to be operated on
*   link          - link to configure
*
* OUTPUTS:
*   *length_ptr   - maximum supported frame length
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_max_frm_len_get(emac_handle_t *hndl, UINT32 link, UINT32 *length_ptr)
{
    PMC_ENTRY();

    /* Read length */
    *length_ptr = emac_mac_field_FRM_LENGTH_get(NULL, hndl, link);

    PMC_RETURN(PMC_SUCCESS);

} /* emac_max_frm_len_get */

/*******************************************************************************
*  emac_tx_ipg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read transmit Inter-Packet Gap.
*
* INPUTS:
*   hndl          - handle to the EMAC instance to be operated on
*   link          - link to configure
*
* OUTPUTS:
*   *gap_ptr      - TX Inter-Packet gap
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_tx_ipg_get(emac_handle_t *hndl, UINT32 link, UINT32 *gap_ptr)
{
    PMC_ENTRY();

    UINT32 tx_ipg_val;

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
      
    if (EMAC_NUM_LINKS <= link) {

        PMC_RETURN(EMAC_LOG_ERR_INVALID_LINK_ID);
    }

    /* Read IPG */
    tx_ipg_val  = emac_mac_field_TX_IPG_LENGTH_get(NULL, hndl, link);

    switch (hndl->var.data_rate[link]) {

        case EMAC_LINK_10G:        
            break;
        case EMAC_LINK_40G:
        case EMAC_LINK_100G:
            /* 
               In 40/100G mode, the TX_IPG_LENGTH bit field controls the 
               transmit PPM compensation.  
               When TX_IPG_LENGTH[6:5] = 2'b00 : Normal mode of operation. 
                                                 Markers compensation active 
                                                 meaning effective +60ppm.
               When TX_IPG_LENGTH[6:5] = 2'b01 : Default compensation 
                                                 plus 60ppm (total of +120ppm).
               When TX_IPG_LENGTH[6:5] = 2'b10 : Default compensation 
                                                 plus 140ppm (total of +200ppm).
               When TX_IPG_LENGTH[6:5] = 2'b11 : Default compensation 
                                                 plus 200ppm (total of +260ppm).
               Note: TX_IPG_LENGTH[4:0] is ignored.
            */
            tx_ipg_val = tx_ipg_val >> 5;
            break; 
        default:
            PMC_LOG_TRACE("EMAC link %d not provionned IPG: %d", link, tx_ipg_val);
            PMC_RETURN(EMAC_LOG_ERR_LINK_NOT_PROV);
            break;
    }

    *gap_ptr = tx_ipg_val;

    PMC_RETURN(PMC_SUCCESS);
} /* emac_tx_ipg_get */


/*
** Private Functions
*/
   
/*
** End of file
*/
