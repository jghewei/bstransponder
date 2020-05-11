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
** Public Functions
*/
/*******************************************************************************
*  etrans_intr_enable_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure specified status.
*
* INPUTS:
*   hndl        - handle to the ETRANS instance to be operated on
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
PUBLIC UINT32 etrans_intr_enable_set(etrans_handle_t *hndl, BOOL8 enable)
{
    PMC_ENTRY();

    /* 
     * Configure interrupt enable in registers 0x118 - 0x124,
     * 0x1C8 - 0x1D4, 0x23C - 0x250
     */

    PMC_RETURN(PMC_SUCCESS);

} /* etrans_intr_enable_set */

/*******************************************************************************
*  etrans_lenfifo_ovr_intr_enable_set_10g
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure specified channel of LenFIFO OVR Interrupt Enable
*
* INPUTS:
*   hndl        - handle to the ETRANS instance to be operated on
*   link        - link to configure
*   enable      - TRUE: Enable interrupt
*                 FALSE: Disable interrupt
* OUTPUTS:
*
* RETURNS:
*   PMC_ERROR - meaningful error messages
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 etrans_lenfifo_ovr_intr_enable_set_10g(etrans_handle_t *hndl, UINT32 link, BOOL8 enable) {
  PMC_ENTRY();
  PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
*  etrans_lenfifo_ovr_intr_enable_set_40g
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure specified channel of LenFIFO OVR Interrupt Enable
*
* INPUTS:
*   hndl        - handle to the ETRANS instance to be operated on
*   link        - link to configure
*   enable      - TRUE: Enable interrupt
*                 FALSE: Disable interrupt
* OUTPUTS:
*
* RETURNS:
*   PMC_ERROR - meaningful error messages
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 etrans_lenfifo_ovr_intr_enable_set_40g(etrans_handle_t *hndl, UINT32 link, BOOL8 enable) {
  PMC_ENTRY();
  PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
*  etrans_lenfifo_ovr_intr_enable_set_100g
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure specified channel of LenFIFO OVR Interrupt Enable
*
* INPUTS:
*   hndl        - handle to the ETRANS instance to be operated on
*   enable      - TRUE: Enable interrupt
*                 FALSE: Disable interrupt
* OUTPUTS:
*
* RETURNS:
*   PMC_ERROR - meaningful error messages
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 etrans_lenfifo_ovr_intr_enable_set_100g(etrans_handle_t *hndl, BOOL8 enable) {
  PMC_ENTRY();
  PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
*  etrans_intr_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read specified status.
*
* INPUTS:
*   hndl        - handle to the ETRANS instance to be operated on
*   status_type - status type to read
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Status value. Bits 11:0 correspond to links 11:0.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 etrans_intr_status_get(etrans_handle_t *hndl, etrans_status_t status_type)
{
    PMC_ENTRY();

    /* 
     * Read status from registers 0x128 - 0x134, 
     * 0x1D8 - 0x1E4, 0x254 - 0x268
     */

    PMC_RETURN(PMC_SUCCESS);

} /* etrans_intr_status_get */


/*
** Private Functions
*/
   
/*
** End of file
*/
