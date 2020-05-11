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
/*******************************************************************************
*  gsup43_c73_cnt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Reads the specified counter.
*
* INPUTS:
*   hndl          - handle to the GSUP43_C73 instance to be operated on
*   update_cnt    - TRUE: Trigger a counter update, then read the counter.
*                   FALSE: Do not trigger update before reading the counter.
*   count_type    - counter type to read
*
* OUTPUTS:
*   *count_val_ptr - counter value
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*   count_type may be one of the following:
*                   0 - block type error cnt [7:0]
*                   1 - High BER cnt [7:0]
*                   2 - Rx block error cnt [31:0]
*                   3 - Rx Ordered Set Discard cnt [39:0]
*                   4 - PRBS-31 error cnt [7:0]
*                   5 - Rx data byte cnt [39:0]
*                   6 - Rx Ordered Set cnt [39:0]
*                   7 - Rx packet cnt [39:0]
*                   8 - Tx packet cnt [39:0]
*                   9 - Tx data byte cnt [39:0]
*                  10 - Tx Ordered Set cnt [39:0]
*
*******************************************************************************/
PUBLIC PMC_ERROR gsup43_c73_cnt_get(gsup43_c73_handle_t *hndl, BOOL update_cnt, UINT32 count_type,
                                    UINT32 *count_val_ptr)
{
    PMC_ENTRY();

    /* Trigger PMON_UPDATE and read counter */
    /* Register 0x0: GSUP43_C73 - Configuration */
    /* Registers 0x7-0xF */
    /* Registers 0x12-0x16 */

    PMC_RETURN(PMC_SUCCESS);

} /* gsup43_c73_cnt_get */


/*
** Private Functions
*/

/*
** End of file
*/
