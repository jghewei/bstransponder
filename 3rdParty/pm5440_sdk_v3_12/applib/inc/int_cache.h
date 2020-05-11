/*******************************************************************************
*   COPYRIGHT (C) 2011-2012 PMC-SIERRA, INC. ALL RIGHTS RESERVED.                  
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
******************************************************************************/

#ifndef _INT_CACHE_H
#define _INT_CACHE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * this module converts read/modify/writes of config space into register writes.
 */
#include "pmc_types.h"
#include "pmc_htable.h"

#define INT_CACHE_HTABLE_SIZE 1024

/*******************************************************************************
* STRUCTURE: pmc_sys_int_cache_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to define interrupt cache descriptor
*
* ELEMENTS:
*       enabled           - TRUE means enabled, FALSE means disabled
*       htable            - hash table 
*       thread_id         - the thread id on which the cache is active
*       num_reg           - number of register in the cache
*       num_read          - number of read executed on the cache
*       num_cached_read   - number of cached read
*******************************************************************************/
typedef struct int_cache_t
{   
    BOOL8 enable;
    pmc_htable_t htable;
    UINT32 thread_id;
    UINT32 num_reg;
    UINT32 num_cached_read;
    UINT32 num_read;
} int_cache_t;

PUBLIC void int_cache_init(int_cache_t *int_cache);
PUBLIC void int_cache_uninit(int_cache_t *int_cache);
PUBLIC BOOL8 int_cache_reg_read(int_cache_t *int_cache, UINT32 reg_addr, UINT32 *value);
PUBLIC void int_cache_reg_add(int_cache_t *int_cache, UINT32 reg_addr, UINT32 value);
PUBLIC BOOL8 int_cache_enable_get(pmc_sys_handle_t *sys_handle,int_cache_t *int_cache_ptr);
PUBLIC void int_cache_enable_set(pmc_sys_handle_t *sys_handle,
                                 int_cache_t *int_cache_ptr, 
                                 BOOL8 enable, 
                                 BOOL8 clear);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _INT_CACHE_H */
