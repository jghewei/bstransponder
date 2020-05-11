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
 *     and register accessor functions for the stli_768 block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing stli_768 io functions is:
 | {
 |     stli_768_buffer_t b_ptr[1];
 |     stli_768_buffer_init( b_ptr, h_ptr );
 |     value = stli_768_field_<FIELD>_get( b_ptr, h_ptr );
 |     stli_768_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     stli_768_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = stli_768_field_<FIELD>_get( NULL, h_ptr );
 | stli_768_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------------+
 | | Register 0x00001020 PGM_CLK_CFG |
 | +---------------------------------+
 | | bit  15:12 R/W  INTERLEAVEEN    |
 | +---------------------------------+
 | 
 | void   stli_768_field_INTERLEAVEEN_set       ( stli_768_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 stli_768_field_INTERLEAVEEN_get       ( stli_768_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   stli_768_field_range_INTERLEAVEEN_set ( stli_768_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 stli_768_field_range_INTERLEAVEEN_get ( stli_768_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +-------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _STLI_768_IO_H
#define _STLI_768_IO_H

#include "cbrc_api.h"
#include "stli_768_regs.h"
#include "stli_768_io_inline.h"


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 stli_768_buffer_init               ( stli_768_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void                 stli_768_buffer_flush              ( stli_768_buffer_t *b_ptr )
 | UINT32               stli_768_reg_read                  ( stli_768_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 stli_768_reg_write                 ( stli_768_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 stli_768_field_set                 ( stli_768_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 stli_768_action_on_write_field_set ( stli_768_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 stli_768_burst_read                ( stli_768_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 stli_768_burst_write               ( stli_768_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE stli_768_poll                      ( stli_768_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-------------------------------------------------------------------------------------------------------------------------------------+
 | void   stli_768_reg_PGM_CLK_CFG_write     ( stli_768_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void   stli_768_reg_PGM_CLK_CFG_field_set ( stli_768_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 stli_768_reg_PGM_CLK_CFG_read      ( stli_768_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 +------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _STLI_768_IO_H */
