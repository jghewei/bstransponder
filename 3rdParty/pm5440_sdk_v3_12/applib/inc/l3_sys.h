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

#ifndef l3_SYS_H
#define l3_SYS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*
 * the level 3 system layer caches the config space registers (converts read/modify/writes of config space into writes).
 */
#include "pmc_types.h"
#include "pmc_err.h"
#include "l4_sys.h"
#include "pmc_log.h"
#include "shadow_registers.h"
#include "config_cache.h"
#include "int_cache.h"

#ifdef NO_CONFIG_CACHE

/*******************************************************************************
* STRUCTURE: l3pmc_sys_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to track L3 state for configuration cache 
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef l4pmc_sys_handle_t l3pmc_sys_handle_t;

#define l3sys_init( sys_handle ) \
    l4sys_init(sys_handle)

#define l3sys_halt( sys_handle ) \
    l4sys_halt(sys_handle)

#define l3sys_reg_read_modify_write( sys_handle, mem_type, reg_addr, mask, value ) \
    l4sys_reg_read_modify_write( sys_handle, reg_addr, mask, value )

#define l3sys_reg_write( sys_handle, mem_type, reg_addr, value ) \
    l4sys_reg_write( sys_handle, reg_addr, value )

#define l3sys_reg_read( sys_handle, mem_type, reg_addr ) \
    l4sys_reg_read( sys_handle, reg_addr )

#define l3sys_burst_write( sys_handle, mem_type, reg_addr, len, data ) \
    l4sys_burst_write( sys_handle, reg_addr, len, data )

#define l3sys_burst_read( sys_handle, mem_type, reg_addr, len, data ) \
    l4sys_burst_read( sys_handle, reg_addr, len, data )

#define sys_config_cache_init_defaults( sys_handle, defaults, len )

#define sys_config_cache_disable(  sys_handle )

#define sys_config_cache_enable( sys_handle )

#define sys_config_cache_flush( sys_handle )

#else /* NO_CONFIG_CACHE */

/*******************************************************************************
* STRUCTURE: l3pmc_sys_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to track L3 state for configuration cache 
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct l3pmc_sys_handle_t 
{
    l4pmc_sys_handle_t  l4sys_handle;  /*!< handle to lower layer interface */
    BOOL            use_config_cache;  /*!< enable the use of a config cache */
    config_cache_t  config_cache;      /*!< configuration cache info */
    int_cache_t     int_cache;         /*!< interrupt cache data */
} l3pmc_sys_handle_t;

void l3sys_init(  pmc_sys_handle_t *sys_handle );

void l3sys_halt(  pmc_sys_handle_t *sys_handle );

void sys_config_cache_init_defaults( pmc_sys_handle_t *sys_handle, simple_memory_defaults *defaults, UINT32 len );

void sys_config_cache_disable( pmc_sys_handle_t *sys_handle );

void sys_config_cache_enable( pmc_sys_handle_t *sys_handle );

void sys_config_cache_flush( pmc_sys_handle_t *sys_handle );

void l3sys_reg_write( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr, UINT32 value );

UINT32 l3sys_reg_read( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr ) ATTRIBUTE_WARN_UNUSED_RESULT;

void l3sys_reg_read_modify_write( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr, UINT32 mask, UINT32 value );

void l3sys_burst_write( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr, UINT32 len, UINT32 *data );


void l3sys_burst_read( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr, UINT32 len, UINT32 *data );

#endif /* NO_CONFIG_CACHE */

#define l3sys_poll_bit( sys, addr, mask, value, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
    l4sys_poll_bit( sys, addr, mask, value, max_count, num_failed_polls, delay_between_polls_in_microseconds )

#define l3sys_poll( sys, addr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
    l4sys_poll( sys, addr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* l3_SYS_H */
