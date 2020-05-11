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
 *     and register accessor functions for the tgmp block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing tgmp io functions is:
 | {
 |     tgmp_buffer_t b_ptr[1];
 |     tgmp_buffer_init( b_ptr, h_ptr );
 |     value = tgmp_field_<FIELD>_get( b_ptr, h_ptr );
 |     tgmp_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     tgmp_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = tgmp_field_<FIELD>_get( NULL, h_ptr );
 | tgmp_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------------------------+
 | | Register (0x00000004 + (N) * 0x4) CHANNEL_RESET_REG |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  0 R/W  CHAN_RSTB                               |
 | +-----------------------------------------------------+
 | 
 | void   tgmp_field_CHAN_RSTB_set ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 tgmp_field_CHAN_RSTB_get ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32  N )
 | 
 | +------------------------------------------------------+
 | | Register (0x00000200 + (N) * 0x4) TXJC_RES_OVRFLW_EN |
 | |     N[0..2]                                          |
 | +------------------------------------------------------+
 | | bit  31:0 R/W  TXJC_RES_OVRFLW_E                     |
 | +------------------------------------------------------+
 | 
 | void   tgmp_lfield_TXJC_RES_OVRFLW_E_set       ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   tgmp_lfield_TXJC_RES_OVRFLW_E_get       ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   tgmp_lfield_range_TXJC_RES_OVRFLW_E_set ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tgmp_lfield_range_TXJC_RES_OVRFLW_E_get ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00000220 + (N) * 0x4) TXJC_RES_OVRFLW_INT |
 | |     N[0..2]                                           |
 | +-------------------------------------------------------+
 | | bit  31:0 R/W  TXJC_RES_OVRFLW_I                      |
 | +-------------------------------------------------------+
 | 
 | void   tgmp_lfield_TXJC_RES_OVRFLW_I_set_to_clear       ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   tgmp_lfield_TXJC_RES_OVRFLW_I_get                ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   tgmp_lfield_range_TXJC_RES_OVRFLW_I_set_to_clear ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tgmp_lfield_range_TXJC_RES_OVRFLW_I_get          ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE tgmp_lfield_range_TXJC_RES_OVRFLW_I_poll( tgmp_buffer_t *b_ptr,
 | tgmp_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE tgmp_lfield_range_TXJC_RES_OVRFLW_I_poll( tgmp_buffer_t *b_ptr,
 | tgmp_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "tgmp_lfield_range_TXJC_RES_OVRFLW_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "tgmp_lfield_range_TXJC_RES_OVRFLW_I_set_to_clear", start_bit, stop_bit);
 |     return tgmp_reg_TXJC_RES_OVRFLW_INT_array_poll( b_ptr,
 |                                                     h_ptr,
 |                                                     word_number,
 |                                                     mask,
 |                                                     ( value << field_ofs),
 |                                                     cmp,
 |                                                     max_count,
 |                                                     num_failed_polls,
 |                                                     delay_between_polls_in_microseconds);
 | }
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _TGMP_IO_H
#define _TGMP_IO_H

#include "tgmp_loc.h"
#include "tgmp_regs.h"
#include "tgmp_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 tgmp_buffer_init               ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr )
 | void                 tgmp_buffer_flush              ( tgmp_buffer_t *b_ptr )
 | UINT32               tgmp_reg_read                  ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 tgmp_reg_write                 ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 tgmp_field_set                 ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 tgmp_action_on_write_field_set ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 tgmp_burst_read                ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 tgmp_burst_write               ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE tgmp_poll                      ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 tgmp_reg_CHANNEL_RESET_REG_array_write                       ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 tgmp_reg_CHANNEL_RESET_REG_array_field_set                   ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               tgmp_reg_CHANNEL_RESET_REG_array_read                        ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 tgmp_reg_TXJC_RES_OVRFLW_EN_array_burst_write                ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 tgmp_reg_TXJC_RES_OVRFLW_EN_array_field_set                  ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 tgmp_reg_TXJC_RES_OVRFLW_EN_array_burst_read                 ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               tgmp_reg_TXJC_RES_OVRFLW_EN_array_read                       ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 tgmp_reg_TXJC_RES_OVRFLW_INT_array_burst_write               ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 tgmp_reg_TXJC_RES_OVRFLW_INT_array_action_on_write_field_set ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 tgmp_reg_TXJC_RES_OVRFLW_INT_array_burst_read                ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               tgmp_reg_TXJC_RES_OVRFLW_INT_array_read                      ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE tgmp_reg_TXJC_RES_OVRFLW_INT_array_poll                      ( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _TGMP_IO_H */
