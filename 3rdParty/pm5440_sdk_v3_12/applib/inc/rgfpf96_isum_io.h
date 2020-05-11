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
 *     and register accessor functions for the rgfpf96_isum block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing rgfpf96_isum io functions is:
 | {
 |     rgfpf96_isum_buffer_t b_ptr[1];
 |     rgfpf96_isum_buffer_init( b_ptr, h_ptr );
 |     value = rgfpf96_isum_field_<FIELD>_get( b_ptr, h_ptr );
 |     rgfpf96_isum_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     rgfpf96_isum_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = rgfpf96_isum_field_<FIELD>_get( NULL, h_ptr );
 | rgfpf96_isum_field_<FIELD>_set( NULL, h_ptr , value );
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
 | UINT32               rgfpf96_isum_field_INT_SUMMARY_0_get        ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32               rgfpf96_isum_field_range_INT_SUMMARY_0_get  ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE rgfpf96_isum_field_range_INT_SUMMARY_0_poll ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE rgfpf96_isum_field_INT_SUMMARY_0_poll       ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x00000004 INT_SUMMARY_REG_1 |
 | +---------------------------------------+
 | | bit  31:0 R  INT_SUMMARY_1            |
 | +---------------------------------------+
 | 
 | UINT32               rgfpf96_isum_field_INT_SUMMARY_1_get        ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32               rgfpf96_isum_field_range_INT_SUMMARY_1_get  ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE rgfpf96_isum_field_range_INT_SUMMARY_1_poll ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE rgfpf96_isum_field_INT_SUMMARY_1_poll       ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x00000008 INT_SUMMARY_REG_2 |
 | +---------------------------------------+
 | | bit  31:0 R  INT_SUMMARY_2            |
 | +---------------------------------------+
 | 
 | UINT32               rgfpf96_isum_field_INT_SUMMARY_2_get        ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32               rgfpf96_isum_field_range_INT_SUMMARY_2_get  ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE rgfpf96_isum_field_range_INT_SUMMARY_2_poll ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE rgfpf96_isum_field_INT_SUMMARY_2_poll       ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x0000000c INT_SUMMARY_REG_3 |
 | +---------------------------------------+
 | | bit  31:0 R  INT_SUMMARY_3            |
 | +---------------------------------------+
 | 
 | UINT32               rgfpf96_isum_field_INT_SUMMARY_3_get        ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32               rgfpf96_isum_field_range_INT_SUMMARY_3_get  ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE rgfpf96_isum_field_range_INT_SUMMARY_3_poll ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE rgfpf96_isum_field_INT_SUMMARY_3_poll       ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x00000010 INT_SUMMARY_REG_4 |
 | +---------------------------------------+
 | | bit  14:0 R  INT_SUMMARY_4            |
 | +---------------------------------------+
 | 
 | UINT32               rgfpf96_isum_field_INT_SUMMARY_4_get        ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32               rgfpf96_isum_field_range_INT_SUMMARY_4_get  ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE rgfpf96_isum_field_range_INT_SUMMARY_4_poll ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE rgfpf96_isum_field_INT_SUMMARY_4_poll       ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _RGFPF96_ISUM_IO_H
#define _RGFPF96_ISUM_IO_H

#include "mapotn_loc.h"
#include "rgfpf96_isum_regs.h"
#include "rgfpf96_isum_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 rgfpf96_isum_buffer_init               ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void                 rgfpf96_isum_buffer_flush              ( rgfpf96_isum_buffer_t *b_ptr )
 | UINT32               rgfpf96_isum_reg_read                  ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 rgfpf96_isum_reg_write                 ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 rgfpf96_isum_field_set                 ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 rgfpf96_isum_action_on_write_field_set ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rgfpf96_isum_burst_read                ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 rgfpf96_isum_burst_write               ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE rgfpf96_isum_poll                      ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | UINT32               rgfpf96_isum_reg_INT_SUMMARY_REG_0_read ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE rgfpf96_isum_reg_INT_SUMMARY_REG_0_poll ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               rgfpf96_isum_reg_INT_SUMMARY_REG_1_read ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE rgfpf96_isum_reg_INT_SUMMARY_REG_1_poll ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               rgfpf96_isum_reg_INT_SUMMARY_REG_2_read ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE rgfpf96_isum_reg_INT_SUMMARY_REG_2_poll ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               rgfpf96_isum_reg_INT_SUMMARY_REG_3_read ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE rgfpf96_isum_reg_INT_SUMMARY_REG_3_poll ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               rgfpf96_isum_reg_INT_SUMMARY_REG_4_read ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE rgfpf96_isum_reg_INT_SUMMARY_REG_4_poll ( rgfpf96_isum_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _RGFPF96_ISUM_IO_H */
