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

#ifndef _FEC_PMON_H
#define _FEC_PMON_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "fec_pmon_api.h"


/*
** Enumerated Types
*/

/*
** Constants
*/

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/



/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC fec_pmon_handle_t *fec_pmon_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void fec_pmon_ctxt_destroy(fec_pmon_handle_t *fec_pmon_handle);
PUBLIC void fec_pmon_handle_init(fec_pmon_handle_t *fec_pmon_handle);
PUBLIC UINT32 fec_pmon_base_address_get(fec_pmon_handle_t *fec_pmon_handle);

PUBLIC UINT32 fec_pmon_unit_test_make_test(fec_pmon_handle_t *fec_pmon_handle, UINT32 val);


#ifdef __cplusplus
}
#endif

#endif /* _FEC_PMON_H */

/*
** end of file
*/
