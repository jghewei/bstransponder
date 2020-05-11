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
*   DESCRIPTION:
*
*   NOTES:
*
********************************************************************************/

#include "l1_sys.h"
#include "pmc_log.h"

/*******************************************************************************
* l1sys_allocate
* ___________________________________________________________________________
*
* DESCRIPTION:
*   allocates a pmc_internal_handle and assigns it to the first field
*   in the customer_sys_handle.
*
* INPUTS:
*   customer_sys_handle - Opaque customer system handle.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*   The 'sys_handle' holds two things,... the customer sys handle and the pmc internal handle.
*   
*   the first field in the customer handle must be a pointer to the pmc internal handle.
*  
*   although the customer sys handle is passed to all the l1sys, l2sys, l3sys, and l4sys layers,
*   only the first field is dereferenced and cast into the pmc internal handle.
*  
*******************************************************************************/
void l1sys_allocate(pmc_sys_handle_t *customer_sys_handle)
{
    l1pmc_sys_handle_t *h = (l1pmc_sys_handle_t*)PMC_CALLOC( sizeof( l1pmc_sys_handle_t ) );
    *(l1pmc_sys_handle_t**)customer_sys_handle = h;
    l2sys_init(customer_sys_handle);
}

/*******************************************************************************
* l1sys_deallocate
* ___________________________________________________________________________
*
* DESCRIPTION:
*   frees the memory associated with the l1sys handle.
*
* INPUTS:
*   customer_sys_handle - customer system handle
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void l1sys_deallocate(pmc_sys_handle_t *customer_sys_handle)
{
    l1pmc_sys_handle_t *h = *(l1pmc_sys_handle_t**)customer_sys_handle;
    l2sys_halt(customer_sys_handle);
    PMC_FREE(&h);
}
