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

#include "fec_pmon_loc.h"


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
* fec_pmon_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a FEC_PMON block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the FEC_PMON subsystem relative to 
*                           the device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - the name of this TSB.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   fec_pmon_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC fec_pmon_handle_t *fec_pmon_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    fec_pmon_handle_t *fec_pmon_handle;

    PMC_ENTRY();
    fec_pmon_handle = (fec_pmon_handle_t*)PMC_CTXT_CALLOC(sizeof(fec_pmon_handle_t), parent);
    
    pmc_handle_init(parent, fec_pmon_handle, sys_handle, PMC_MID_DIGI_FEC_PMON, tsb_name, base_address);
    
    /*fec_pmon_handle->cfc_handle = cfc_ctxt_create((pmc_handle_t *)fec_pmon_handle, base_address+BASE_ADDR_fec_pmon_CFC, sys_handle, "CFC1"); */

    PMC_RETURN(fec_pmon_handle);
} /* fec_pmon_ctxt_create */

 
/*******************************************************************************
* fec_pmon_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a FEC_PMON block instance.
*
*
* INPUTS:
*   *fec_pmon_handle           - pointer to FEC_PMON handle instance
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
PUBLIC void fec_pmon_ctxt_destroy(fec_pmon_handle_t *fec_pmon_handle)
{
    PMC_ENTRY();
    PMC_CTXT_FREE(&fec_pmon_handle, fec_pmon_handle);

    PMC_RETURN();
} /* fec_pmon_ctxt_destroy */


/*******************************************************************************
* fec_pmon_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a FEC_PMON block handle instance.
*
*
* INPUTS:
*   *fec_pmon_handle           - pointer to FEC_PMON handle instance
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
PUBLIC void fec_pmon_handle_init(fec_pmon_handle_t *fec_pmon_handle)
{
    PMC_ENTRY();

    PMC_RETURN();
} /* fec_pmon_handle_init */

/*******************************************************************************
* fec_pmon_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of a FEC_PMON block instance.
*
*
* INPUTS:
*   *fec_pmon_handle           - pointer to FEC_PMON handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Base address
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 fec_pmon_base_address_get(fec_pmon_handle_t *fec_pmon_handle)
{
    PMC_ENTRY();

    PMC_RETURN(fec_pmon_handle->base.base_address);
} /* fec_pmon_base_address_get */


/*******************************************************************************
* fec_pmon_unit_test_make_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of a FEC_PMON block instance.
*
*
* INPUTS:
*   *fec_pmon_handle           - pointer to FEC_PMON handle instance.
*   val                        - a value.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   NONE
*    
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 fec_pmon_unit_test_make_test(fec_pmon_handle_t *fec_pmon_handle, UINT32 val)
{
    fec_pmon_buffer_t b[1];
   
    PMC_ENTRY();

    fec_pmon_buffer_init( b, fec_pmon_handle );
    
    PMC_LOG_TRACE("%u, cfc/unit_test makefile testing...\n", val);
    
    /* flush any changed fields */
    fec_pmon_buffer_flush( b );
    
    PMC_RETURN(0);
} /* fec_pmon_unit_test_make_test */

/*
** Private Functions
*/


/*
** End of file
*/
