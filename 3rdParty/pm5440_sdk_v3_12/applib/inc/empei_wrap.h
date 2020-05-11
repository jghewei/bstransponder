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

#ifndef _EMPEI_WRAP_H
#define _EMPEI_WRAP_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "util_global.h"
#include "empei_wrap_api.h"
#include "empei_e.h"
#include "empei_i.h"

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
/*******************************************************************************
* STRUCTURE: empei_wrap_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This is the config struct for initial values.
*
*******************************************************************************/
typedef struct empei_wrap_cfg_struct_t{  

} empei_wrap_cfg_t;

/*******************************************************************************
* STRUCTURE: empei_wrap_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is to illustrate the dynamically changing context.

*******************************************************************************/
typedef struct empei_wrap_var_struct_t {

} empei_wrap_var_t;

/*******************************************************************************
* STRUCTURE: empei_wrap_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is the empei_wrap context 
*
* ELEMENTS:
*   base - base PMC handle 
*   cfg  - static config
*   var  - dynamic context 
*   empei_e_handle - EMPEI_E handle 
*   empei_i_handle - EMPEI_I handle 
*******************************************************************************/
struct empei_wrap_handle_struct_t {
    pmc_handle_t base;
    
    empei_wrap_cfg_t    cfg;
    empei_wrap_var_t    var;
    
    /* Child handles needed for EMPEI_WRAP */ 
    empei_e_handle_t *empei_e_handle;
    empei_i_handle_t *empei_i_handle;
    
};

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC empei_wrap_handle_t *empei_wrap_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void empei_wrap_ctxt_destroy(empei_wrap_handle_t *empei_wrap_handle);
PUBLIC void empei_wrap_handle_init(empei_wrap_handle_t *empei_wrap_handle);
PUBLIC PMC_ERROR empei_wrap_handle_restart_init(empei_wrap_handle_t *empei_wrap_handle,
                                                util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                                pmc_energy_state_t energy_state);
PUBLIC UINT32 empei_wrap_base_address_get(empei_wrap_handle_t *empei_wrap_handle);
PUBLIC PMC_ERROR empei_wrap_dummy(empei_wrap_handle_t *empei_wrap_handle);

#ifdef __cplusplus
}
#endif

#endif /* _EMPEI_WRAP_H */

/* 
** end of file 
*/
