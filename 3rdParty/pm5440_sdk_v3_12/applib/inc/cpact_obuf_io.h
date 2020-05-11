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
 *     and register accessor functions for the cpact_obuf block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing cpact_obuf io functions is:
 | {
 |     cpact_obuf_buffer_t b_ptr[1];
 |     cpact_obuf_buffer_init( b_ptr, h_ptr );
 |     value = cpact_obuf_field_<FIELD>_get( b_ptr, h_ptr );
 |     cpact_obuf_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     cpact_obuf_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = cpact_obuf_field_<FIELD>_get( NULL, h_ptr );
 | cpact_obuf_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------------------------------+
 | | Register (0x00000000 + (N) * 0x10) CPACT_CHNL_CFG |
 | |     N[0..95]                                      |
 | +---------------------------------------------------+
 | | bit  2:0 R/W  CH_MAP_TYPE                         |
 | +---------------------------------------------------+
 | 
 | void   cpact_obuf_field_CH_MAP_TYPE_set       ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 cpact_obuf_field_CH_MAP_TYPE_get       ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N )
 | void   cpact_obuf_field_range_CH_MAP_TYPE_set ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpact_obuf_field_range_CH_MAP_TYPE_get ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register (0x00000004 + (N) * 0x10) SOMF_CFG |
 | |     N[0..95]                                |
 | +---------------------------------------------+
 | | bit  21:16 R/W  SOMF_FIRST_DT               |
 | | bit  14:0  R/W  SOMF_CYCLE                  |
 | +---------------------------------------------+
 | 
 | void   cpact_obuf_field_SOMF_FIRST_DT_set       ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 cpact_obuf_field_SOMF_FIRST_DT_get       ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N )
 | void   cpact_obuf_field_range_SOMF_FIRST_DT_set ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpact_obuf_field_range_SOMF_FIRST_DT_get ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   cpact_obuf_field_SOMF_CYCLE_set          ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 cpact_obuf_field_SOMF_CYCLE_get          ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N )
 | void   cpact_obuf_field_range_SOMF_CYCLE_set    ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpact_obuf_field_range_SOMF_CYCLE_get    ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register (0x00000008 + (N) * 0x10) EOMF_CFG |
 | |     N[0..95]                                |
 | +---------------------------------------------+
 | | bit  21:16 R/W  EOMF_LAST_DT                |
 | | bit  14:0  R/W  EOMF_CYCLE                  |
 | +---------------------------------------------+
 | 
 | void   cpact_obuf_field_EOMF_LAST_DT_set       ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 cpact_obuf_field_EOMF_LAST_DT_get       ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N )
 | void   cpact_obuf_field_range_EOMF_LAST_DT_set ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpact_obuf_field_range_EOMF_LAST_DT_get ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   cpact_obuf_field_EOMF_CYCLE_set         ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 cpact_obuf_field_EOMF_CYCLE_get         ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N )
 | void   cpact_obuf_field_range_EOMF_CYCLE_set   ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpact_obuf_field_range_EOMF_CYCLE_get   ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _CPACT_OBUF_IO_H
#define _CPACT_OBUF_IO_H

#include "cpact_obuf_loc.h"
#include "cpact_obuf_regs.h"
#include "cpact_obuf_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 cpact_obuf_buffer_init               ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr )
 | void                 cpact_obuf_buffer_flush              ( cpact_obuf_buffer_t *b_ptr )
 | UINT32               cpact_obuf_reg_read                  ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 cpact_obuf_reg_write                 ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 cpact_obuf_field_set                 ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 cpact_obuf_action_on_write_field_set ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 cpact_obuf_burst_read                ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 cpact_obuf_burst_write               ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE cpact_obuf_poll                      ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   cpact_obuf_reg_CPACT_CHNL_CFG_array_write     ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   cpact_obuf_reg_CPACT_CHNL_CFG_array_field_set ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpact_obuf_reg_CPACT_CHNL_CFG_array_read      ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N )
 | 
 | void   cpact_obuf_reg_SOMF_CFG_array_write           ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   cpact_obuf_reg_SOMF_CFG_array_field_set       ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpact_obuf_reg_SOMF_CFG_array_read            ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N )
 | 
 | void   cpact_obuf_reg_EOMF_CFG_array_write           ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   cpact_obuf_reg_EOMF_CFG_array_field_set       ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpact_obuf_reg_EOMF_CFG_array_read            ( cpact_obuf_buffer_t *b_ptr, cpact_obuf_handle_t *h_ptr, UINT32  N )
 | 
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _CPACT_OBUF_IO_H */
