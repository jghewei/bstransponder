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

#ifndef NO_CONFIG_CACHE

/*
 * this module converts read/modify/writes of config space into register writes.
 */
#include "pmc_types.h"
#include "pmc_sys.h"
#include "pmc_log.h"
#include "int_cache.h"

/*********************************************************************
* int_cache_init
* ____________________________________________________________________
*
* DESCRIPTION:
*   Initialize the interrupt cache. Context resides in sys_handle.
*
* INPUTS:
*    *int_cache_ptr - Pointer to int cache
*
* OUTPUTS: 
*   None
*
* RETURNS:
*    None
*
* NOTES:
* 
**********************************************************************/
PUBLIC void int_cache_init(int_cache_t *int_cache_ptr)
{
    int_cache_ptr->enable = FALSE;
    pmc_htable_init(&int_cache_ptr->htable,2048);
    int_cache_ptr->num_reg = 0;
    int_cache_ptr->num_read = 0;
    int_cache_ptr->num_cached_read = 0;               
} /* int_cache_init */

/*********************************************************************
* int_cache_uninit
* ____________________________________________________________________
*
* DESCRIPTION:
*   Initialize the interrupt cache. Context resides in sys_handle.
*
* INPUTS:
*    *int_cache_ptr - Pointer to int cache
*
* OUTPUTS: 
*   None
*
* RETURNS:
*    None
*
* NOTES:
* 
**********************************************************************/
PUBLIC void int_cache_uninit(int_cache_t *int_cache_ptr)
{
    pmc_htable_uninit(&int_cache_ptr->htable);
} /* int_cache_uninit */

/*********************************************************************
* int_cache_reg_read
* ____________________________________________________________________
*
* DESCRIPTION:
*   Read at a specified address from the cache. 
*
* INPUTS:
*    *int_cache_ptr - Pointer to int cache
*    addr        - register address
*
* OUTPUTS: 
*   *value       - register value (valid only when this function 
*                  return TRUE)
*
* RETURNS:
*    TRUE  : reg addr is in the cache, *value is valid
*    FALSE : reg addr is not in the cache, *value is invalid
*
* NOTES:
* 
**********************************************************************/
PUBLIC BOOL8 int_cache_reg_read(int_cache_t *int_cache_ptr,
                                UINT32 addr, 
                                UINT32 *value)
{
    BOOL8 result = FALSE;

    result = pmc_htable_lookup(&int_cache_ptr->htable, addr, value);

    if (result == TRUE)
        int_cache_ptr->num_cached_read += 1;
    int_cache_ptr->num_read += 1;
    return result;
} /* int_cache_reg_read */

/*********************************************************************
* int_cache_reg_add
* ____________________________________________________________________
*
* DESCRIPTION:
*   Add a register (addr,value) pair in the cache
*
* INPUTS:
*    *int_cache_ptr - Pointer to int cache
*    addr        - register address
*    value       - register value
*
* OUTPUTS: 
*   None
*
* RETURNS:
*   None
*
* NOTES:
* 
**********************************************************************/
PUBLIC void int_cache_reg_add(int_cache_t *int_cache_ptr,
                              UINT32 addr, 
                              UINT32 value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    
    rc = pmc_htable_add(&int_cache_ptr->htable, addr,value);
    PMC_ASSERT(rc, PMC_ERR_FAIL,0,0); 
    
    int_cache_ptr->num_reg+=1;    
} /* int_cache_reg_add */

/*********************************************************************
* int_cache_enable_set
* ____________________________________________________________________
*
* DESCRIPTION:
*   Enable/Disable interrupt caching. The cache can be optionally 
*   cleared. The thread id of the caller is captured by the function. 
*   The interrupt caching is enabled only on the thread that has called 
*   this function 
*
* INPUTS:
*   *sys_handle     - pointer to sys handle
*    *int_cache_ptr - Pointer to int cache
*    enable      - TRUE enables the cache, FALSE disables the cache
*    clear       - TRUE clears the cache, FALSE is not clearing the the cache
*
* OUTPUTS: 
*   None
*
* RETURNS:
*   None
*
* NOTES:
* 
**********************************************************************/
PUBLIC void int_cache_enable_set(pmc_sys_handle_t *sys_handle,
                                 int_cache_t *int_cache_ptr, 
                                 BOOL8 enable, 
                                 BOOL8 clear)
{    
    /* the interrupt caching system is only valid on a thread */
    int_cache_ptr->thread_id  = pmc_thread_id_get(sys_handle);
    int_cache_ptr->enable     = enable;
    if (TRUE == clear)
    {        
        PMC_LOG(PMC_LOG_SEV_MEDIUM, PMC_SHARED_LOG_TRACE,0,0,
                "Clearing int cache (th_id = %d, num_reg=%d, num_read=%d, run_cached_read=%d)\n",
                int_cache_ptr->thread_id,       
                int_cache_ptr->num_reg,
                int_cache_ptr->num_read,
                int_cache_ptr->num_cached_read);
        pmc_htable_clear(&int_cache_ptr->htable);
        int_cache_ptr->num_reg = 0;
        int_cache_ptr->num_read = 0;
        int_cache_ptr->num_cached_read = 0;        
    }
} /* int_cache_enable_set */

/*********************************************************************
* int_cache_enable_get
* ____________________________________________________________________
*
* DESCRIPTION:
*   Return if the cache is enabled. The cache is only enabled for the 
*   thread that has enabled the cache.
*
* INPUTS:
*   *sys_handle     - pointer to sys handle
*    *int_cache_ptr - Pointer to int cache
*
* OUTPUTS: 
*   None
*
* RETURNS:
*   TRUE: cache is enabled\n
*   FALSE: cache is disabled
*
* NOTES:
* 
**********************************************************************/
PUBLIC BOOL8 int_cache_enable_get(pmc_sys_handle_t *sys_handle,int_cache_t *int_cache_ptr)
{    
    UINT32 thread_id;
    if (int_cache_ptr->enable == TRUE)
    {
        thread_id = pmc_thread_id_get(sys_handle);
        if (thread_id == int_cache_ptr->thread_id)
        {
            return TRUE;
        }
    }
    return FALSE;

} /*int_cache_enable_get*/


#endif /* ifndef NO_CONFIG_CACHE */
