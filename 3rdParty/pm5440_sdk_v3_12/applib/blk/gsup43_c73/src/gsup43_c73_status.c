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
** Public Functions
*/
#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */ 
/* LCOV_EXCL_START */
/*******************************************************************************
*  gsup43_c73_intr_enable_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable/disable specified interrupt
*
* INPUTS:
*   hndl        - handle to the GSUP43_C73 instance to be operated on
*   int_type    - Interrupt to enable/disable
*   enable      - TRUE: enable interrupt. FALSE: disable interrupt
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERROR - meaningful error messages
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR gsup43_c73_intr_enable_set(gsup43_c73_handle_t *hndl, 
                                         gsup43_c73_int_type_enum_t int_type, BOOL enable)
{
    PMC_ENTRY();

    switch (int_type) {
        case GSUP43_C73_LOS:
            gsup43_c73_field_LOS_E_set(NULL, hndl, enable);
            break;
        case GSUP43_C73_LOS_SYNC: 
            gsup43_c73_field_LOS_SYNC_E_set(NULL, hndl, enable);
            break;
        case GSUP43_C73_HI_BER: 
            gsup43_c73_field_HI_BER_E_set(NULL, hndl, enable);
            break;
        case GSUP43_C73_LINK_FAIL: 
            gsup43_c73_field_LINK_FAIL_E_set(NULL, hndl, enable);
            break;
        case GSUP43_C73_RX_FAULT: 
            gsup43_c73_field_RX_FAULT_E_set(NULL, hndl, enable);
            break;
        case GSUP43_C73_BLK_TYPE_ERR: 
            gsup43_c73_field_BLK_TYPE_ERR_E_set(NULL, hndl, enable);
            break;
        case GSUP43_C73_RX_PRBS31_ERR: 
            gsup43_c73_field_RX_PRBS31_ERR_E_set(NULL, hndl, enable);
            break;
        case GSUP43_C73_ALL_ONES_OR_ZEROS: 
            gsup43_c73_field_ALL_ONES_OR_ZEROS_E_set(NULL, hndl, enable);
            break;
        case GSUP43_C73_RX_RF: 
            gsup43_c73_field_RX_RF_E_set( NULL, hndl, enable );
            break;
        case GSUP43_C73_RX_LF:
            gsup43_c73_field_RX_LF_E_set( NULL, hndl, enable );
            break;
            /* Tx interrupts */
        case GSUP43_C73_FIFO_UNDRUN:
        gsup43_c73_field_FIFO_UNDRUN_E_set( NULL, hndl, enable );
            break;
        case GSUP43_C73_FIFO_OVRUN:
        gsup43_c73_field_FIFO_OVRUN_E_set( NULL, hndl, enable );
            break;
        case GSUP43_C73_SEND_RF:
        gsup43_c73_field_SEND_RF_E_set( NULL, hndl, enable );
            break;
        case GSUP43_C73_SEND_LF:
           gsup43_c73_field_SEND_LF_E_set( NULL, hndl, enable );
            break;
        case GSUP43_C73_SEND_IDLE:
        gsup43_c73_field_SEND_IDLE_E_set( NULL, hndl, enable );
            break;
        case GSUP43_C73_TYPE_ERR:
        gsup43_c73_field_TYPE_ERR_E_set( NULL, hndl, enable );
            break;

        default: 
            break;
    } /* end switch */

    PMC_RETURN(PMC_SUCCESS);

} /* gsup43_c73_intr_enable_set */

/*******************************************************************************
*  gsup43_c73_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read interrupt status 
*
* INPUTS:
*   hndl        - handle to the GSUP43_C73 instance to be operated on
*
* OUTPUTS:
*   *rx_status_ptr - Rx interrupts
*                    Bit 7: ALL_ONES_OR_ZEROS_IC
*                    Bit 6: RX_PRBS31_ERR_I
*                    Bit 5: BLK_TYPE_ERR_I
*                    Bit 4: RX_FAULT_IC
*                    Bit 3: LINK_FAIL_IC
*                    Bit 2: HI_BER_IC
*                    Bit 1: LOS_SYNC_IC
*                    Bit 0: LOS_IC
*
*   *tx_status_ptr - Tx interrupts
*                    Bit 5: TYPE_ERR_I
*                    Bit 4: SEND_IDLE_I
*                    Bit 3: SEND_LF_I
*                    Bit 2: SEND_RF_I
*                    Bit 1: FIFO_OVRUN_I
*                    Bit 0: FIFO_UNDRUN_I
*
* RETURNS:
*   PMC_ERROR - meaningful error messages
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR gsup43_c73_status_get(gsup43_c73_handle_t *hndl, UINT32 *rx_status_ptr, UINT32 *tx_status_ptr)
{
    PMC_ENTRY();
    /* Read interrupts for Rx and Tx */
    /* Register 0x5: GSUP43_C73 - RX INTERRUPT INDICATION */
    /* Register 0x18: GSUP43_C73 - TX Interrupt Indication */

    PMC_RETURN(PMC_SUCCESS);

} /* gsup43_c73_status_get */

/*******************************************************************************
*  gsup43_c73_global_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read interrupt status and values
*
* INPUTS:
*   hndl        - handle to the GSUP43_C73 instance to be operated on
*   clear_int   - flag to clear interrupt status bit
*
* OUTPUTS:
*   *status_ptr - interrupts & values  (offsets MUST match digi_enet_api.h defines)
*                 Bit 21: SEND_IDLE_V  (sending Idle control characters)
*                 Bit 20: SEND_LF_V  (sending LF)
*                 Bit 19: SEND_RF_V  (sending RF)
*                 Bit 18: TYPE_ERR_I  (UPI type error detected)
*                 Bit 17: FIFO_OVRUN_I  (FIFO overrun occurred)
*                 Bit 16: FIFO_UNDRUN_I  (FIFO underrun occurred)
*                 Bit 15: RF_FAULT  (RF detected)
*                 Bit 14: LF_FAULT  (LF detected)
*                 Bit 13: ALL_ONES_OR_ZEROS_V  (receiving input pattern of 1's & 0's)
*                 Bit 12: ALL_ONES_OR_ZEROS_IC  (transition on receiving 1's & 0's)
*                 Bit 11: RX_PRBS31_ERR_I  (PRBS31 error detected)
*                 Bit 10: BLK_TYPE_ERR_I  (64B/66B type field error detected)
*                 Bit  9: RX_FAULT_V  (link fault detected)
*                 Bit  8: RX_FAULT_IC  (link fault transition)
*                 Bit  7: LINK_FAIL_V  (link fail detected)
*                 Bit  6: LINK_FAIL_IC  (link fail transition)
*                 Bit  5: HI_BER_V  (high BER detected)
*                 Bit  4: HI_BER_IC  (high BER transition)
*                 Bit  3: LOS_SYNC_V  (RX synchronization in LOS)
*                 Bit  2: LOS_SYNC_IC  (RX synchronization transition)
*                 Bit  1: LOS_V  (LOS detected)
*                 Bit  0: LOS_IC  (LOS transition)
*
* RETURNS:
*   PMC_ERROR - meaningful error messages
*
* NOTES:
*   *** Assuming CLEAR ON WRITE interrupt mode ***
*
*******************************************************************************/
PUBLIC PMC_ERROR gsup43_c73_global_status_get(gsup43_c73_handle_t *hndl, BOOL clear_int, UINT32 *status_ptr)
{
    UINT32 status = 0;
    PMC_ENTRY();

    if (gsup43_c73_field_SEND_IDLE_V_get(NULL, hndl)) {       /* sending Idle control characters */
        status |= (1 << 21);
    }
    if (gsup43_c73_field_SEND_LF_V_get(NULL, hndl)) {         /* sending LF */
        status |= (1 << 20);
    }
    if (gsup43_c73_field_SEND_RF_V_get(NULL, hndl)) {         /* sending RF */
        status |= (1 << 19);
    }
    if (gsup43_c73_field_TYPE_ERR_I_get(NULL, hndl)) {        /* UPI type error detected? */
        if (clear_int)
            gsup43_c73_field_TYPE_ERR_I_set_to_clear(NULL, hndl, 1);
        status |= (1 << 18);
    }
    if (gsup43_c73_field_FIFO_OVRUN_I_get(NULL, hndl)) {      /* FIFO overrun occurred? */
        if (clear_int)
            gsup43_c73_field_FIFO_OVRUN_I_set_to_clear(NULL, hndl, 1);
        status |= (1 << 17);
    }
    if (gsup43_c73_field_FIFO_UNDRUN_I_get(NULL, hndl)) {     /* FIFO underrun occurred? */
        if (clear_int)
            gsup43_c73_field_FIFO_UNDRUN_I_set_to_clear(NULL, hndl, 1);
        status |= (1 << 16);
    }
    if (gsup43_c73_field_RF_FAULT_get(NULL, hndl)) {          /* RF detected? */
        status |= (1 << 15);
    }
    if (gsup43_c73_field_LF_FAULT_get(NULL, hndl)) {          /* LF detected? */
        status |= (1 << 14);
    }
    if (gsup43_c73_field_ALL_ONES_OR_ZEROS_V_get(NULL, hndl)) {  /* receiving input pattern of 1's & 0's? */
        status |= (1 << 13);
    }
    if (gsup43_c73_field_ALL_ONES_OR_ZEROS_IC_get(NULL, hndl)) {  /* transition on receiving 1's & 0's? */
        if (clear_int)
            gsup43_c73_field_ALL_ONES_OR_ZEROS_IC_set_to_clear(NULL, hndl, 1);
        status |= (1 << 12);
    }
    if (gsup43_c73_field_RX_PRBS31_ERR_I_get(NULL, hndl)) {   /* PRBS31 error detected? */
        if (clear_int)
            gsup43_c73_field_RX_PRBS31_ERR_I_set_to_clear(NULL, hndl, 1);
        status |= (1 << 11);
    }
    if (gsup43_c73_field_BLK_TYPE_ERR_I_get(NULL, hndl)) {    /* 64B/66B type field error detected? */
        if (clear_int)
            gsup43_c73_field_BLK_TYPE_ERR_I_set_to_clear(NULL, hndl, 1);
        status |= (1 << 10);
    }
    if (gsup43_c73_field_RX_FAULT_V_get(NULL, hndl)) {        /* link fault detected? */
        status |= (1 << 9);
    }
    if (gsup43_c73_field_RX_FAULT_IC_get(NULL, hndl)) {       /* link fault transition? */
        if (clear_int)
            gsup43_c73_field_RX_FAULT_IC_set_to_clear(NULL, hndl, 1);
        status |= (1 << 8);
    }
    if (gsup43_c73_field_LINK_FAIL_V_get(NULL, hndl)) {       /* link fail detected? */
        status |= (1 << 7);
    }
    if (gsup43_c73_field_LINK_FAIL_IC_get(NULL, hndl)) {      /* link fail transition? */
        if (clear_int)
            gsup43_c73_field_LINK_FAIL_IC_set_to_clear(NULL, hndl, 1);
        status |= (1 << 6);
    }
    if (gsup43_c73_field_HI_BER_V_get(NULL, hndl)) {          /* high BER detected? */
        status |= (1 << 5);
    }
    if (gsup43_c73_field_HI_BER_IC_get(NULL, hndl)) {         /* high BER transition? */
        if (clear_int)
            gsup43_c73_field_HI_BER_IC_set_to_clear(NULL, hndl, 1);
        status |= (1 << 4);
    }
    if (gsup43_c73_field_LOS_SYNC_V_get(NULL, hndl)) {        /* RX synchronization in LOS? */
        status |= (1 << 3);
    }
    if (gsup43_c73_field_LOS_SYNC_IC_get(NULL, hndl)) {       /* RX synchronization transition? */
        if (clear_int)
            gsup43_c73_field_LOS_SYNC_IC_set_to_clear(NULL, hndl, 1);
        status |= (1 << 2);
    }

    if (gsup43_c73_field_LOS_V_get(NULL, hndl)) {             /* LOS detected? */
        status |= (1 << 1);
    }
    if (gsup43_c73_field_LOS_IC_get(NULL, hndl)) {            /* LOS transition? */
        if (clear_int)
            gsup43_c73_field_LOS_IC_set_to_clear(NULL, hndl, 1);
        status |= (1 << 0);
    }

    *status_ptr = status;

    PMC_RETURN(PMC_SUCCESS);

} /* gsup43_c73_global_status_get */
/* LCOV_EXCL_STOP */
#endif/*#ifndef DOXYGEN_PUBLIC_ONLY*/


/*
** Private Functions
*/
   
/*
** End of file
*/
