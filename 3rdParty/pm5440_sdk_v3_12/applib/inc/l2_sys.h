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

#ifndef L2_SYS_H
#define L2_SYS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*
 * the level 2 system layer provides functionality for 'change sets',
 * if change sets are enabled, it saves the list of operations,
 * when change sets are applied it replays the list of operations.
 */
#include "pmc_types.h"
#include "pmc_err.h"
#include "l3_sys.h"
#include "pmc_log.h"
#include "shadow_registers.h"
#include "change_sets.h"

#ifdef NO_CHANGE_SETS

/*******************************************************************************
* STRUCTURE: l2pmc_sys_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to track L2 state for changesets 
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
#define l2pmc_sys_handle_t l3pmc_sys_handle_t

#define sys_build_change_set( sys_handle, cs )

#define sys_disable_change_set( sys_handle )

#define cs_apply_change_set( sys_handle, cs )

#define l2sys_init( sys_handle ) \
    l3sys_init(sys_handle)

#define l2sys_halt( sys_handle ) \
    l3sys_halt(sys_handle)

#define l2sys_reg_read_modify_write( sys_handle, mem_type, reg_addr, mask, value ) \
    l3sys_reg_read_modify_write( sys_handle, mem_type, reg_addr, mask, value )

#define l2sys_reg_write( sys_handle, mem_type, reg_addr, value ) \
    l3sys_reg_write( sys_handle, mem_type, reg_addr, value )

#define l2sys_reg_read( sys_handle, mem_type, reg_addr ) \
    l3sys_reg_read( sys_handle, mem_type, reg_addr )

#define l2sys_burst_write( sys_handle, mem_type, reg_addr, len, data ) \
    l3sys_burst_write( sys_handle, mem_type, reg_addr, len, data )

#define l2sys_burst_read( sys_handle, mem_type, reg_addr, len, data ) \
    l3sys_burst_read( sys_handle, mem_type, reg_addr, len, data )

#define l2sys_poll_bit( sys_handle, mem_type, reg_addr, mask, value, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
    l3sys_poll_bit( sys_handle, reg_addr, mask, value, max_count, num_failed_polls, delay_between_polls_in_microseconds )

#define l2sys_poll( sys_handle, mem_type, reg_addr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
    l3sys_poll( sys_handle, reg_addr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )

#define pmc_log_start_operation( sys_handle, fn )

#else


/*******************************************************************************
* STRUCTURE: l2pmc_sys_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to track L2 state for changesets 
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct l2pmc_sys_handle_t 
{
    l3pmc_sys_handle_t l3sys_handle; /*!< handle to lower layer of calls */
    UINT8          use_change_set;   /*!< flag to track enabled/disabled */
    change_set_t   change_set;       /*!< change set info */
} l2pmc_sys_handle_t;

void sys_build_change_set( pmc_sys_handle_t *sys_handle, change_set_t cs );

void sys_monitor_change_set( pmc_sys_handle_t *sys_handle, change_set_t cs );

void sys_disable_change_set( pmc_sys_handle_t *sys_handle );

void cs_apply_change_set( pmc_sys_handle_t *sys_handle, change_set_t cs );

void l2sys_init(  pmc_sys_handle_t *sys_handle );

void l2sys_halt(  pmc_sys_handle_t *sys_handle );

void l2sys_reg_read_modify_write( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr, UINT32 mask, UINT32 value );

void l2sys_reg_write( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr, UINT32 value );

UINT32 l2sys_reg_read( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr ) ATTRIBUTE_WARN_UNUSED_RESULT;

void l2sys_burst_write( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr, UINT32 len, UINT32 *data );

void l2sys_burst_read( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr, UINT32 len, UINT32 *data );

PMC_ERROR l2sys_poll_bit( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr, UINT32 mask, UINT32 value, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds );

PMC_POLL_RETURN_TYPE l2sys_poll( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds );

#endif /* NO_CHANGE_SET */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* L2_SYS_H */
