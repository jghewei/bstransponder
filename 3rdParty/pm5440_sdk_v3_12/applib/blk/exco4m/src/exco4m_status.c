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

/*
** Public Functions
*/
#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */ 
/* LCOV_EXCL_START */
/*******************************************************************************
*  exco4m_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read interrupt status and values
*
* INPUTS:
*   hndl        - handle to the EXCO4M instance to be operated on
*   clear_int   - flag to clear interrupt status bit
*
* OUTPUTS:
*   *tx_status_ptr - TX interrupts & values (offsets MUST match digi_enet_api.h defines)
*                    Bit 9: TX_LOS_V  (LOS detected)
*                    Bit 8: TX_LOS_I  (LOS transition)
*                    Bit 7: TX_LF_OS_DET_I  (LF detected)
*                    Bit 6: TX_RF_OS_DET_I  (RF detected)
*                    Bit 5: TX_IDLE_TEST_ERR_DET_I  (idle test pattern removed)
*                    Bit 4: TX_SKEW_RANGE_VIOL_I  (skew range violation detected)
*                    Bit 3: TX_PCS_INV_BLK_DET_I  (PCS invalid block detected)
*                    Bit 2: TX_PCS_ERR_BLK_DET_I  (PCS error block detected)
*                    Bit 1: TX_PCS_HBER_V  (high BER detected)
*                    Bit 0: TX_PCS_HBER_I  (high BER transition)
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*   *** Assuming CLEAR ON WRITE interrupt mode ***
*
*******************************************************************************/
PUBLIC PMC_ERROR exco4m_status_get(exco4m_handle_t *hndl, BOOL8 clear_int, UINT32 *tx_status_ptr)
{
    UINT32 tx_status = 0;
    PMC_ENTRY();

    if (exco4m_field_TX_LOS_V_get(NULL, hndl)) {              /* LOS detected? */
        tx_status |= (1 << 9);
    }
    if (exco4m_field_TX_LOS_I_get(NULL, hndl)) {              /* LOS transition? */
        if (clear_int)
            exco4m_field_TX_LOS_I_set_to_clear(NULL, hndl, 1);
        tx_status |= (1 << 8);
    }
    if (exco4m_field_TX_LF_OS_DET_I_get(NULL, hndl)) {        /* LF detected? */
        if (clear_int)
            exco4m_field_TX_LF_OS_DET_I_set_to_clear(NULL, hndl, 1);
        tx_status |= (1 << 7);
    }
    if (exco4m_field_TX_RF_OS_DET_I_get(NULL, hndl)) {        /* RF detected? */
        if (clear_int)
            exco4m_field_TX_RF_OS_DET_I_set_to_clear(NULL, hndl, 1);
        tx_status |= (1 << 6);
    }
    if (exco4m_field_TX_IDLE_TEST_ERR_DET_I_get(NULL, hndl)) {  /* idle test pattern removed? */
        if (clear_int)
            exco4m_field_TX_IDLE_TEST_ERR_DET_I_set_to_clear(NULL, hndl, 1);
        tx_status |= (1 << 5);
    }
    if (exco4m_field_TX_SKEW_RANGE_VIOL_I_get(NULL, hndl)) {  /* skew range violation detected? */
        if (clear_int)
            exco4m_field_TX_SKEW_RANGE_VIOL_I_set_to_clear(NULL, hndl, 1);
        tx_status |= (1 << 4);
    }
    if (exco4m_field_TX_PCS_INV_BLK_DET_I_get(NULL, hndl)) {  /* PCS invalid block detected? */
        if (clear_int)
            exco4m_field_TX_PCS_INV_BLK_DET_I_set_to_clear(NULL, hndl, 1);
        tx_status |= (1 << 3);
    }
    if (exco4m_field_TX_PCS_ERR_BLK_DET_I_get(NULL, hndl)) {  /* PCS error block detected? */
        if (clear_int)
            exco4m_field_TX_PCS_ERR_BLK_DET_I_set_to_clear(NULL, hndl, 1);
        tx_status |= (1 << 2);
    }
    if (exco4m_field_TX_PCS_HBER_V_get(NULL, hndl)) {         /* high BER detected? */
        tx_status |= (1 << 1);
    }
    if (exco4m_field_TX_PCS_HBER_I_get(NULL, hndl)) {         /* high BER transition? */
        if (clear_int)
            exco4m_field_TX_PCS_HBER_I_set_to_clear(NULL, hndl, 1);
        tx_status |= (1 << 0);
    }

    *tx_status_ptr = tx_status;

    PMC_RETURN(PMC_SUCCESS);

} /* exco4m_status_get */
/* LCOV_EXCL_STOP */
#endif/*#ifndef DOXYGEN_PUBLIC_ONLY*/


/*
** Private Functions
*/

/*
** End of file
*/
