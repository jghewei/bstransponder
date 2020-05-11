/******************************************************************************
 *   COPYRIGHT (C) 2013 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
 * -------------------------------------------------------------------------
 *   DESCRIPTION:
 *     contains prototypes for the field accessor functions
 *     and register accessor functions for the rttp_indirect block
 *****************************************************************************/

/*-----------------------------------------------------------------------------------------------.
 | The pattern for reading indirect registers is:
 | {
 |     rttp_indirect_ACCEPTED_TRACE_buffer_t b_ptr[1];
 |     rttp_indirect_ACCEPTED_TRACE_buffer_init(b_ptr);
 |     rttp_indirect_ACCEPTED_TRACE_buffer_retrieve(h_ptr, some_index, some_other_index, b_ptr);
 |     value = rttp_indirect_<FIELD1>_get(b_ptr);
 |     value = rttp_indirect_<FIELD2>_get(b_ptr);
 | ...
 | 
 | The pattern for writing indirect registers is:
 | {
 |     rttp_indirect_CAPTURED_TRACE_buffer_t b_ptr[1];
 |     CAPTURED_TRACE_buffer_init(b_ptr);
 |     value = rttp_indirect_<FIELD1>_set(b_ptr, value1);
 |     value = rttp_indirect_<FIELD2>_set(b_ptr, value2);
 | ...
 |     rttp_indirect_CAPTURED_TRACE_buffer_applyh_ptr, some_index, some_other_index, b_ptr);
 | }
 | 
 | note: the indexing arguments are defined by the user.
 | 
 +----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------.
 | 
 | +------------------------------------------------+
 | | Register 0x00000000 TRACE_MISMATCH_UNSTBL_STAT |
 | +------------------------------------------------+
 | | bit  1 R  TIM_V                                |
 | | bit  0 R  TIU_V                                |
 | +------------------------------------------------+
 | 
 | UINT32 rttp_indirect_ifield_TIM_V_get ( rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_t *b_ptr )
 | UINT32 rttp_indirect_ifield_TIU_V_get ( rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_t *b_ptr )
 | 
 | +--------------------------------------------------+
 | | Register 0x00000001 TRACE_MISMATCH_UNSTBL_INT_EN |
 | +--------------------------------------------------+
 | | bit  1 R/W  TIM_E                                |
 | | bit  0 R/W  TIU_E                                |
 | +--------------------------------------------------+
 | 
 | void   rttp_indirect_ifield_TIM_E_set ( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_t *b_ptr, UINT32 value )
 | UINT32 rttp_indirect_ifield_TIM_E_get ( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_t *b_ptr )
 | void   rttp_indirect_ifield_TIU_E_set ( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_t *b_ptr, UINT32 value )
 | UINT32 rttp_indirect_ifield_TIU_E_get ( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_t *b_ptr )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000002 TRACE_MISMATCH_UNSTBL_INT_STAT |
 | +----------------------------------------------------+
 | | bit  1 R/W  TIM_I                                  |
 | | bit  0 R/W  TIU_I                                  |
 | +----------------------------------------------------+
 | 
 | void   rttp_indirect_ifield_TIM_I_set ( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t *b_ptr, UINT32 value )
 | UINT32 rttp_indirect_ifield_TIM_I_get ( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t *b_ptr )
 | void   rttp_indirect_ifield_TIU_I_set ( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t *b_ptr, UINT32 value )
 | UINT32 rttp_indirect_ifield_TIU_I_get ( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t *b_ptr )
 | 
 | +-------------------------------+
 | | Register 0x00000003 TRACE_CFG |
 | +-------------------------------+
 | | bit  6   R/W  TTMMONEN        |
 | | bit  5   R/W  SYNC_CRLF       |
 | | bit  4   R/W  ZEROEN          |
 | | bit  3   R/W  PER5            |
 | | bit  2   R/W  LENGTH16        |
 | | bit  1:0 R/W  ALGO            |
 | +-------------------------------+
 | 
 | void   rttp_indirect_ifield_TTMMONEN_set  ( rttp_indirect_TRACE_CFG_buffer_t *b_ptr, UINT32 value )
 | UINT32 rttp_indirect_ifield_TTMMONEN_get  ( rttp_indirect_TRACE_CFG_buffer_t *b_ptr )
 | void   rttp_indirect_ifield_SYNC_CRLF_set ( rttp_indirect_TRACE_CFG_buffer_t *b_ptr, UINT32 value )
 | UINT32 rttp_indirect_ifield_SYNC_CRLF_get ( rttp_indirect_TRACE_CFG_buffer_t *b_ptr )
 | void   rttp_indirect_ifield_ZEROEN_set    ( rttp_indirect_TRACE_CFG_buffer_t *b_ptr, UINT32 value )
 | UINT32 rttp_indirect_ifield_ZEROEN_get    ( rttp_indirect_TRACE_CFG_buffer_t *b_ptr )
 | void   rttp_indirect_ifield_PER5_set      ( rttp_indirect_TRACE_CFG_buffer_t *b_ptr, UINT32 value )
 | UINT32 rttp_indirect_ifield_PER5_get      ( rttp_indirect_TRACE_CFG_buffer_t *b_ptr )
 | void   rttp_indirect_ifield_LENGTH16_set  ( rttp_indirect_TRACE_CFG_buffer_t *b_ptr, UINT32 value )
 | UINT32 rttp_indirect_ifield_LENGTH16_get  ( rttp_indirect_TRACE_CFG_buffer_t *b_ptr )
 | void   rttp_indirect_ifield_ALGO_set      ( rttp_indirect_TRACE_CFG_buffer_t *b_ptr, UINT32 value )
 | UINT32 rttp_indirect_ifield_ALGO_get      ( rttp_indirect_TRACE_CFG_buffer_t *b_ptr )
 | 
 | +--------------------------------------------------+
 | | Register (0x00000040 + (N) * 0x1) CAPTURED_TRACE |
 | |     N[0..63]                                     |
 | +--------------------------------------------------+
 | | bit  7:0 R  CTRACE                               |
 | +--------------------------------------------------+
 | 
 | UINT32 rttp_indirect_ifield_CTRACE_get ( rttp_indirect_CAPTURED_TRACE_buffer_t *b_ptr )
 | 
 | +--------------------------------------------------+
 | | Register (0x00000080 + (N) * 0x1) ACCEPTED_TRACE |
 | |     N[0..63]                                     |
 | +--------------------------------------------------+
 | | bit  7:0 R  ATRACE                               |
 | +--------------------------------------------------+
 | 
 | UINT32 rttp_indirect_ifield_ATRACE_get ( rttp_indirect_ACCEPTED_TRACE_buffer_t *b_ptr )
 | 
 | +--------------------------------------------------+
 | | Register (0x000000c0 + (N) * 0x1) EXPECTED_TRACE |
 | |     N[0..63]                                     |
 | +--------------------------------------------------+
 | | bit  7:0 R/W  ETRACE                             |
 | +--------------------------------------------------+
 | 
 | void   rttp_indirect_ifield_ETRACE_set ( rttp_indirect_EXPECTED_TRACE_buffer_t *b_ptr, UINT32 value )
 | UINT32 rttp_indirect_ifield_ETRACE_get ( rttp_indirect_EXPECTED_TRACE_buffer_t *b_ptr )
 +---------------------------------------------------------------------------------------------------------------------*/
#ifndef _RTTP_INDIRECT_IO_H
#define _RTTP_INDIRECT_IO_H

#include "sdh_pmg_api.h"
#include "rttp_indirect_regs.h"
#include "rttp_indirect_io_inline.h"


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 rttp_indirect_buffer_init               ( rttp_indirect_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void                 rttp_indirect_buffer_flush              ( rttp_indirect_buffer_t *b_ptr )
 | UINT32               rttp_indirect_reg_read                  ( rttp_indirect_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 rttp_indirect_reg_write                 ( rttp_indirect_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 rttp_indirect_field_set                 ( rttp_indirect_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 rttp_indirect_action_on_write_field_set ( rttp_indirect_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rttp_indirect_burst_read                ( rttp_indirect_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 rttp_indirect_burst_write               ( rttp_indirect_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE rttp_indirect_poll                      ( rttp_indirect_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _RTTP_INDIRECT_IO_H */
