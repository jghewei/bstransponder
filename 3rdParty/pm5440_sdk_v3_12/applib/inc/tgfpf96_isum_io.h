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
 *     and register accessor functions for the tgfpf96_isum block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing tgfpf96_isum io functions is:
 | {
 |     tgfpf96_isum_buffer_t b_ptr[1];
 |     tgfpf96_isum_buffer_init( b_ptr, h_ptr );
 |     value = tgfpf96_isum_field_<FIELD>_get( b_ptr, h_ptr );
 |     tgfpf96_isum_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     tgfpf96_isum_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = tgfpf96_isum_field_<FIELD>_get( NULL, h_ptr );
 | tgfpf96_isum_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------------------+
 | | Register 0x00000000 INT_SUMMARY_REG_0 |
 | +---------------------------------------+
 | | bit  31:0 R  INT_SUMMARY_0            |
 | +---------------------------------------+
 | 
 | UINT32               tgfpf96_isum_field_INT_SUMMARY_0_get        ( tgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32               tgfpf96_isum_field_range_INT_SUMMARY_0_get  ( tgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE tgfpf96_isum_field_range_INT_SUMMARY_0_poll ( tgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE tgfpf96_isum_field_INT_SUMMARY_0_poll       ( tgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x00000004 INT_SUMMARY_REG_1 |
 | +---------------------------------------+
 | | bit  26:0 R  INT_SUMMARY_1            |
 | +---------------------------------------+
 | 
 | UINT32               tgfpf96_isum_field_INT_SUMMARY_1_get        ( tgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32               tgfpf96_isum_field_range_INT_SUMMARY_1_get  ( tgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE tgfpf96_isum_field_range_INT_SUMMARY_1_poll ( tgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE tgfpf96_isum_field_INT_SUMMARY_1_poll       ( tgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _TGFPF96_ISUM_IO_H
#define _TGFPF96_ISUM_IO_H

#include "mapotn_loc.h"
#include "tgfpf96_isum_regs.h"
#include "tgfpf96_isum_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 tgfpf96_isum_buffer_init               ( tgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void                 tgfpf96_isum_buffer_flush              ( tgfpf96_isum_buffer_t *b_ptr )
 | UINT32               tgfpf96_isum_reg_read                  ( tgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 tgfpf96_isum_reg_write                 ( tgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 tgfpf96_isum_field_set                 ( tgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 tgfpf96_isum_action_on_write_field_set ( tgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 tgfpf96_isum_burst_read                ( tgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 tgfpf96_isum_burst_write               ( tgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE tgfpf96_isum_poll                      ( tgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | UINT32               tgfpf96_isum_reg_INT_SUMMARY_REG_0_read ( tgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE tgfpf96_isum_reg_INT_SUMMARY_REG_0_poll ( tgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               tgfpf96_isum_reg_INT_SUMMARY_REG_1_read ( tgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE tgfpf96_isum_reg_INT_SUMMARY_REG_1_poll ( tgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _TGFPF96_ISUM_IO_H */
