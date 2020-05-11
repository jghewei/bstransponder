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
 *     and register accessor functions for the srli_768 block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing srli_768 io functions is:
 | {
 |     srli_768_buffer_t b_ptr[1];
 |     srli_768_buffer_init( b_ptr, h_ptr );
 |     value = srli_768_field_<FIELD>_get( b_ptr, h_ptr );
 |     srli_768_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     srli_768_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = srli_768_field_<FIELD>_get( NULL, h_ptr );
 | srli_768_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------------+
 | | Register 0x00000080 PGM_CLK_CFG |
 | +---------------------------------+
 | | bit  15:12 R/W  INTERLEAVEEN    |
 | +---------------------------------+
 | 
 | void   srli_768_field_INTERLEAVEEN_set       ( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 value )
 | UINT32 srli_768_field_INTERLEAVEEN_get       ( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
 | void   srli_768_field_range_INTERLEAVEEN_set ( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 srli_768_field_range_INTERLEAVEEN_get ( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _SRLI_768_IO_H
#define _SRLI_768_IO_H

#include "sdh_pmg_api.h"
#include "srli_768_regs.h"
#include "srli_768_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 srli_768_buffer_init               ( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
 | void                 srli_768_buffer_flush              ( srli_768_buffer_t *b_ptr )
 | UINT32               srli_768_reg_read                  ( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 srli_768_reg_write                 ( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 srli_768_field_set                 ( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 srli_768_action_on_write_field_set ( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 srli_768_burst_read                ( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 srli_768_burst_write               ( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE srli_768_poll                      ( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------+
 | void   srli_768_reg_PGM_CLK_CFG_write     ( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 value )
 | void   srli_768_reg_PGM_CLK_CFG_field_set ( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 srli_768_reg_PGM_CLK_CFG_read      ( srli_768_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _SRLI_768_IO_H */
