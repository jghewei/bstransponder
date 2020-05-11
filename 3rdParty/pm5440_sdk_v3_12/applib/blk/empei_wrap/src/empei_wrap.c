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

#include "empei_wrap_loc.h"

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
/* Only include the strings if logging with text*/
#ifdef PMC_LOG_STRINGS

/* 
 * The following creates the LOG/ERR strings from above by pulling out the text
 * portion of the LOG_ERR_TABLE above 
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) str,
const char EMPEI_WRAP_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    EMPEI_WRAP_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char EMPEI_WRAP_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Public Functions
*/

/*******************************************************************************
* empei_wrap_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a EMPEI_WRAP subsystem instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the EMPEI_WRAP subsystem relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - the name of this TSB.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   empei_wrap_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC empei_wrap_handle_t *empei_wrap_ctxt_create(pmc_handle_t *parent, 
                                                   UINT32 base_address, 
                                                   pmc_sys_handle_t *sys_handle, 
                                                   const char *tsb_name )
{
    empei_wrap_handle_t *empei_wrap_handle;
    
    PMC_ENTRY();

    empei_wrap_handle = (empei_wrap_handle_t*)PMC_CTXT_CALLOC(sizeof(empei_wrap_handle_t), parent);
    
    pmc_handle_init( parent, empei_wrap_handle, sys_handle, PMC_MID_DIGI_EMPEI_WRAP, tsb_name, base_address );

    /* Register log strings. */
    pmc_log_block_strings_register(EMPEI_WRAP_LOG_ERR_STRINGS[0], EMPEI_WRAP_LOG_ERR_TABLE_BASE, EMPEI_WRAP_LOG_ERR_COUNT);

    /* Create handle for child blocks */
    empei_wrap_handle->empei_e_handle = empei_e_ctxt_create((pmc_handle_t *)empei_wrap_handle, base_address+BASE_ADDR_EMPEI120_WRAP_EMPEI120_E, sys_handle, "EMPEI_E");
    PMC_CTXT_REGISTER_PTR(&empei_wrap_handle->empei_e_handle, parent);
    empei_wrap_handle->empei_i_handle = empei_i_ctxt_create((pmc_handle_t *)empei_wrap_handle, base_address+BASE_ADDR_EMPEI120_WRAP_EMPEI120_I, sys_handle, "EMPEI_I");   
    PMC_CTXT_REGISTER_PTR(&empei_wrap_handle->empei_i_handle, parent);
    
    PMC_RETURN(empei_wrap_handle);
} /* empei_wrap_ctxt_create */  
 
/*******************************************************************************
* empei_wrap_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a EMPEI_WRAP subsystem instance.
*
*
* INPUTS:
*   *empei_wrap_handle      - pointer to EMPEI_WRAP handle instance to be operated on
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
PUBLIC void empei_wrap_ctxt_destroy(empei_wrap_handle_t *empei_wrap_handle)
{
    PMC_ENTRY();

    empei_e_ctxt_destroy(empei_wrap_handle->empei_e_handle);
    empei_i_ctxt_destroy(empei_wrap_handle->empei_i_handle);
    
    PMC_CTXT_FREE(&empei_wrap_handle, empei_wrap_handle);

    PMC_RETURN();
} /* empei_wrap_ctxt_destroy */

/*******************************************************************************
* empei_wrap_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a EMPEI_WRAP subsystem handle instance.
*
*
* INPUTS:
*   *empei_wrap_handle      - pointer to EMPEI_WRAP handle instance to be operated on
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
PUBLIC void empei_wrap_handle_init(empei_wrap_handle_t *empei_wrap_handle)
{
    
    PMC_ENTRY();

    empei_e_handle_init(empei_wrap_handle->empei_e_handle);
    empei_i_handle_init(empei_wrap_handle->empei_i_handle);

    PMC_RETURN();
} /* empei_wrap_handle_init */

/*******************************************************************************
* empei_wrap_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on empei_wrap.
*
* INPUTS:
*   *empei_wrap_handle              - pointer to empei_wrap handle instance
*   *restart_init_cfg_ptr       - pointer to restart init config
*   top_energy_state_reg        - The expected energy state 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS : 
*     WARM : register/context are coherent
*     CRASH: everything is coherent (a cleanup may has been executed)
*   
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_wrap_handle_restart_init(empei_wrap_handle_t *empei_wrap_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                            pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(EMPEI_WRAP_LOG_ERR_STRINGS[0], EMPEI_WRAP_LOG_ERR_TABLE_BASE, EMPEI_WRAP_LOG_ERR_COUNT);
    }

    ret_val = empei_e_handle_restart_init(empei_wrap_handle->empei_e_handle, restart_init_cfg_ptr, top_energy_state_reg);
    
    if (PMC_SUCCESS == ret_val)
    {
        ret_val = empei_i_handle_restart_init(empei_wrap_handle->empei_i_handle, restart_init_cfg_ptr, top_energy_state_reg);
    }

    PMC_RETURN(ret_val);

} /* empei_wrap_handle_restart_init */
/*******************************************************************************
* empei_wrap_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of a EMPEI_WRAP subsystem instance.
*
*
* INPUTS:
*   *empei_wrap_handle      - pointer to EMPEI_WRAP handle instance to be operated on
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
PUBLIC UINT32 empei_wrap_base_address_get(empei_wrap_handle_t *empei_wrap_handle)
{    
    PMC_ENTRY();

    PMC_RETURN(empei_wrap_handle->base.base_address);
} /* empei_wrap_base_address_get */

/*******************************************************************************
* Initialization Configuration Functions
*******************************************************************************/
/*******************************************************************************
*  empei_wrap_dummy
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Dummy function for PVER wrapper purposes
*
* INPUTS:
*   *hndl      - pointer to EMPEI_WRAP handle instance to be operated on
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_wrap_dummy(empei_wrap_handle_t *hndl)
{
    PMC_ENTRY();
                               
    PMC_RETURN(PMC_SUCCESS);
} /* empei_wrap_dummy */

/*******************************************************************************
* Run-time Operations
*******************************************************************************/


/*
** Private Functions
*/
    
/*
** End of file
*/
