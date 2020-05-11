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

#ifndef CHANGE_SETS_H
#define CHANGE_SETS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "pmc_types.h"

typedef void *change_set_t; /*!< Opaque type for recording register operations in "change sets" */

/* cs_add_operation is just for debugging */
void cs_add_operation( change_set_t cs, const char *fn );
void cs_add_reg_write( change_set_t cs, int mem_type, UINT32 addr, UINT32 value );
void cs_add_reg_read( change_set_t cs, int mem_type, UINT32 addr, UINT32 value );
void cs_add_reg_read_modify_write( change_set_t cs, int mem_type, UINT32 addr, UINT32 mask, UINT32 value );
void cs_add_reg_burst_write( change_set_t cs, int mem_type, UINT32 addr, UINT32 len, UINT32 *value );
void cs_add_reg_burst_read( change_set_t cs, int mem_type, UINT32 addr, UINT32 len, UINT32 *value );
void cs_add_poll_bit( change_set_t public_cs, int mem_type, UINT32 addr, UINT32 mask, UINT32 value, UINT32 max_count, UINT32 delay_between_polls_in_microseconds );
void cs_add_poll( change_set_t public_cs, int mem_type, UINT32 addr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 delay_between_polls_in_microseconds );

change_set_t cs_allocate();
void cs_apply(pmc_sys_handle_t sys, change_set_t cs);
void cs_show( pmc_sys_handle_t sys, change_set_t cs);
void cs_deallocate(change_set_t cs);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CHANGE_SETS_H */
