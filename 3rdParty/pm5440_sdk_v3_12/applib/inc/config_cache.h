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

#ifndef _CONFIG_CACHE_H
#define _CONFIG_CACHE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * this module converts read/modify/writes of config space into register writes.
 */
#include "pmc_types.h"
#include "shadow_registers.h"

typedef sr_memory_region_t config_cache_t; /*!< Opaque type for saving the previous value of configuration registers */

config_cache_t config_cache_create();

void config_cache_delete(config_cache_t config_cache);

void config_cache_init_simple_defaults( config_cache_t config_cache, simple_memory_defaults *defaults, UINT32 len );

void config_cache_update( config_cache_t config_cache, UINT32 reg_addr, UINT32 value );

UINT32 config_cache_read( void *sys_handle, config_cache_t config_cache, UINT32 reg_addr );

UINT32 config_cache_convert_RMW_to_write( void *sys_handle, config_cache_t config_cache, UINT32 reg_addr, UINT32 mask, UINT32 value );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CONFIG_CACHE_H */
