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

#include "empei_e_loc.h"

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
*  empei_e_pmon_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Reads the specified PMON count.
*
* INPUTS:
*   hndl          - handle to the EMPEI_E instance to be operated on
*   link          - link to configure
*   count_type    - counter type to read
*
* OUTPUTS:
*   count_val_ptr - pointer to value read
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_pmon_get(empei_e_handle_t *hndl, UINT32 link, empei_e_pmon_type_enum_t count_type,
                                  UINT32 *count_val_ptr)
{
    PMC_ENTRY();


    PMC_RETURN(PMC_SUCCESS);

} /* empei_e_pmon_get */

/*
** Private Functions
*/
   
/*
** End of file
*/
