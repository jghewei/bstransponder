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
*  epmm_intr_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read specified status.
*
* INPUTS:
*   hndl        - handle to the EPMM instance to be operated on
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
PUBLIC PMC_ERROR epmm_intr_status_get(epmm_handle_t *hndl, epmm_status_t status_type)
{
    PMC_ENTRY();

    /* 
     * Read status from registers 0xA0 - 0xAC, 0xD0 - 0xDC and
     * 0x170 - 0x17C, 0x1A0 - 0x1AC
     */

    PMC_RETURN(PMC_SUCCESS);

} /* epmm_intr_status_get */


/*
** Private Functions
*/
   
/*
** End of file
*/
