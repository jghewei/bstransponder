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
 *     and register accessor functions for the odtu_mux block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing odtu_mux io functions is:
 | {
 |     odtu_mux_buffer_t b_ptr[1];
 |     odtu_mux_buffer_init( b_ptr, h_ptr );
 |     value = odtu_mux_field_<FIELD>_get( b_ptr, h_ptr );
 |     odtu_mux_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     odtu_mux_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = odtu_mux_field_<FIELD>_get( NULL, h_ptr );
 | odtu_mux_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------------------+
 | | Register (0x00000000 + (N) * 0x4) CAL_EN_CTRL |
 | |     N[0..95]                                  |
 | +-----------------------------------------------+
 | | bit  0 R/W  CAL_EN                            |
 | +-----------------------------------------------+
 | 
 | void   odtu_mux_field_CAL_EN_set ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odtu_mux_field_CAL_EN_get ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32  N )
 | 
 | +---------------------------------------------------+
 | | Register (0x00000180 + (N) * 0x4) HO_CH_RSTB_CTRL |
 | |     N[0..95]                                      |
 | +---------------------------------------------------+
 | | bit  0 R/W  HO_CH_RSTB                            |
 | +---------------------------------------------------+
 | 
 | void   odtu_mux_field_HO_CH_RSTB_set ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odtu_mux_field_HO_CH_RSTB_get ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32  N )
 | 
 | +-------------------------------------------------+
 | | Register (0x00000300 + (N) * 0x4) LO_DPATH_CTRL |
 | |     N[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  16 R/W  PASSTHRU                           |
 | | bit  8  R/W  LO_CH_RSTB                         |
 | +-------------------------------------------------+
 | 
 | void   odtu_mux_field_PASSTHRU_set   ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odtu_mux_field_PASSTHRU_get   ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32  N )
 | void   odtu_mux_field_LO_CH_RSTB_set ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odtu_mux_field_LO_CH_RSTB_get ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32  N )
 | 
 | +-------------------------------+
 | | Register 0x00000480 CONFIG    |
 | +-------------------------------+
 | | bit  8 R/W  FORCE_CONFIG_ADDR |
 | | bit  0 R/W  FORCE_CONFIG      |
 | +-------------------------------+
 | 
 | void   odtu_mux_field_FORCE_CONFIG_ADDR_set ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 value )
 | UINT32 odtu_mux_field_FORCE_CONFIG_ADDR_get ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr )
 | void   odtu_mux_field_FORCE_CONFIG_set      ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 value )
 | UINT32 odtu_mux_field_FORCE_CONFIG_get      ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00000500 + (N) * 0x4) TXJC_RES_OVERFLOW_EN |
 | |     N[0..2]                                            |
 | +--------------------------------------------------------+
 | | bit  31:0 R/W  TXJC_RES_OVERFLOW_E                     |
 | +--------------------------------------------------------+
 | 
 | void   odtu_mux_lfield_TXJC_RES_OVERFLOW_E_set       ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_mux_lfield_TXJC_RES_OVERFLOW_E_get       ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_mux_lfield_range_TXJC_RES_OVERFLOW_E_set ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_mux_lfield_range_TXJC_RES_OVERFLOW_E_get ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x00000520 + (N) * 0x4) SW_OVFLW_EN |
 | |     N[0..2]                                   |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  SW_OVFLW_E                     |
 | +-----------------------------------------------+
 | 
 | void   odtu_mux_lfield_SW_OVFLW_E_set       ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_mux_lfield_SW_OVFLW_E_get       ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_mux_lfield_range_SW_OVFLW_E_set ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_mux_lfield_range_SW_OVFLW_E_get ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x00000540 + (N) * 0x4) TXJC_RES_OVERFLOW_INT |
 | |     N[0..2]                                             |
 | +---------------------------------------------------------+
 | | bit  31:0 R/W  TXJC_RES_OVERFLOW_I                      |
 | +---------------------------------------------------------+
 | 
 | void   odtu_mux_lfield_TXJC_RES_OVERFLOW_I_set_to_clear       ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_mux_lfield_TXJC_RES_OVERFLOW_I_get                ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_mux_lfield_range_TXJC_RES_OVERFLOW_I_set_to_clear ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_mux_lfield_range_TXJC_RES_OVERFLOW_I_get          ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE odtu_mux_lfield_range_TXJC_RES_OVERFLOW_I_poll( odtu_mux_buffer_t *b_ptr,
 | odtu_mux_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE odtu_mux_lfield_range_TXJC_RES_OVERFLOW_I_poll( odtu_mux_buffer_t *b_ptr,
 | odtu_mux_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_mux_lfield_range_TXJC_RES_OVERFLOW_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_mux_lfield_range_TXJC_RES_OVERFLOW_I_set_to_clear", start_bit, stop_bit);
 |     return odtu_mux_reg_TXJC_RES_OVERFLOW_INT_array_poll( b_ptr,
 |                                                           h_ptr,
 |                                                           word_number,
 |                                                           mask,
 |                                                           ( value << field_ofs),
 |                                                           cmp,
 |                                                           max_count,
 |                                                           num_failed_polls,
 |                                                           delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +------------------------------------------------+
 | | Register (0x00000560 + (N) * 0x4) SW_OVFLW_INT |
 | |     N[0..2]                                    |
 | +------------------------------------------------+
 | | bit  31:0 R/W  SW_OVFLW_I                      |
 | +------------------------------------------------+
 | 
 | void   odtu_mux_lfield_SW_OVFLW_I_set_to_clear       ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_mux_lfield_SW_OVFLW_I_get                ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_mux_lfield_range_SW_OVFLW_I_set_to_clear ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_mux_lfield_range_SW_OVFLW_I_get          ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE odtu_mux_lfield_range_SW_OVFLW_I_poll( odtu_mux_buffer_t *b_ptr,
 | odtu_mux_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE odtu_mux_lfield_range_SW_OVFLW_I_poll( odtu_mux_buffer_t *b_ptr,
 | odtu_mux_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_mux_lfield_range_SW_OVFLW_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_mux_lfield_range_SW_OVFLW_I_set_to_clear", start_bit, stop_bit);
 |     return odtu_mux_reg_SW_OVFLW_INT_array_poll( b_ptr,
 |                                                  h_ptr,
 |                                                  word_number,
 |                                                  mask,
 |                                                  ( value << field_ofs),
 |                                                  cmp,
 |                                                  max_count,
 |                                                  num_failed_polls,
 |                                                  delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +---------------------------------------------------------+
 | | Register (0x00000580 + (N) * 0x4) TXJC_RES_OVERFLOW_VAL |
 | |     N[0..2]                                             |
 | +---------------------------------------------------------+
 | | bit  31:0 R  TXJC_RES_OVERFLOW_V                        |
 | +---------------------------------------------------------+
 | 
 | void   odtu_mux_lfield_TXJC_RES_OVERFLOW_V_get       ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 odtu_mux_lfield_range_TXJC_RES_OVERFLOW_V_get ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE odtu_mux_lfield_range_TXJC_RES_OVERFLOW_V_poll( odtu_mux_buffer_t *b_ptr,
 | odtu_mux_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE odtu_mux_lfield_range_TXJC_RES_OVERFLOW_V_poll( odtu_mux_buffer_t *b_ptr,
 | odtu_mux_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_mux_lfield_range_TXJC_RES_OVERFLOW_V_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_mux_lfield_range_TXJC_RES_OVERFLOW_V_set_to_clear", start_bit, stop_bit);
 |     return odtu_mux_reg_TXJC_RES_OVERFLOW_VAL_array_poll( b_ptr,
 |                                                           h_ptr,
 |                                                           word_number,
 |                                                           mask,
 |                                                           ( value << field_ofs),
 |                                                           cmp,
 |                                                           max_count,
 |                                                           num_failed_polls,
 |                                                           delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +----------------------------------------------------+
 | | Register (0x000005a0 + (N) * 0x4) SW_OVFLW_INT_VAL |
 | |     N[0..2]                                        |
 | +----------------------------------------------------+
 | | bit  31:0 R  SW_OVFLW_V                            |
 | +----------------------------------------------------+
 | 
 | void   odtu_mux_lfield_SW_OVFLW_V_get       ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 odtu_mux_lfield_range_SW_OVFLW_V_get ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE odtu_mux_lfield_range_SW_OVFLW_V_poll( odtu_mux_buffer_t *b_ptr,
 | odtu_mux_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE odtu_mux_lfield_range_SW_OVFLW_V_poll( odtu_mux_buffer_t *b_ptr,
 | odtu_mux_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_mux_lfield_range_SW_OVFLW_V_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_mux_lfield_range_SW_OVFLW_V_set_to_clear", start_bit, stop_bit);
 |     return odtu_mux_reg_SW_OVFLW_INT_VAL_array_poll( b_ptr,
 |                                                      h_ptr,
 |                                                      word_number,
 |                                                      mask,
 |                                                      ( value << field_ofs),
 |                                                      cmp,
 |                                                      max_count,
 |                                                      num_failed_polls,
 |                                                      delay_between_polls_in_microseconds);
 | }
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _ODTU_MUX_IO_H
#define _ODTU_MUX_IO_H

#include "odtu_mux_loc.h"
#include "odtu_mux_regs.h"
#include "odtu_mux_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 odtu_mux_buffer_init               ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr )
 | void                 odtu_mux_buffer_flush              ( odtu_mux_buffer_t *b_ptr )
 | UINT32               odtu_mux_reg_read                  ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 odtu_mux_reg_write                 ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 odtu_mux_field_set                 ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 odtu_mux_action_on_write_field_set ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_mux_burst_read                ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 odtu_mux_burst_write               ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE odtu_mux_poll                      ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 odtu_mux_reg_CONFIG_write                                          ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 value )
 | void                 odtu_mux_reg_CONFIG_field_set                                      ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odtu_mux_reg_CONFIG_read                                           ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr )
 | 
 | void                 odtu_mux_reg_CAL_EN_CTRL_array_write                               ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odtu_mux_reg_CAL_EN_CTRL_array_field_set                           ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odtu_mux_reg_CAL_EN_CTRL_array_read                                ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odtu_mux_reg_HO_CH_RSTB_CTRL_array_write                           ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odtu_mux_reg_HO_CH_RSTB_CTRL_array_field_set                       ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odtu_mux_reg_HO_CH_RSTB_CTRL_array_read                            ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odtu_mux_reg_LO_DPATH_CTRL_array_write                             ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odtu_mux_reg_LO_DPATH_CTRL_array_field_set                         ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odtu_mux_reg_LO_DPATH_CTRL_array_read                              ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odtu_mux_reg_TXJC_RES_OVERFLOW_EN_array_burst_write                ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_mux_reg_TXJC_RES_OVERFLOW_EN_array_field_set                  ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_mux_reg_TXJC_RES_OVERFLOW_EN_array_burst_read                 ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_mux_reg_TXJC_RES_OVERFLOW_EN_array_read                       ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odtu_mux_reg_SW_OVFLW_EN_array_burst_write                         ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_mux_reg_SW_OVFLW_EN_array_field_set                           ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_mux_reg_SW_OVFLW_EN_array_burst_read                          ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_mux_reg_SW_OVFLW_EN_array_read                                ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odtu_mux_reg_TXJC_RES_OVERFLOW_INT_array_burst_write               ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_mux_reg_TXJC_RES_OVERFLOW_INT_array_action_on_write_field_set ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_mux_reg_TXJC_RES_OVERFLOW_INT_array_burst_read                ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_mux_reg_TXJC_RES_OVERFLOW_INT_array_read                      ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE odtu_mux_reg_TXJC_RES_OVERFLOW_INT_array_poll                      ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_mux_reg_SW_OVFLW_INT_array_burst_write                        ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_mux_reg_SW_OVFLW_INT_array_action_on_write_field_set          ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_mux_reg_SW_OVFLW_INT_array_burst_read                         ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_mux_reg_SW_OVFLW_INT_array_read                               ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE odtu_mux_reg_SW_OVFLW_INT_array_poll                               ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_mux_reg_TXJC_RES_OVERFLOW_VAL_array_burst_read                ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_mux_reg_TXJC_RES_OVERFLOW_VAL_array_read                      ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE odtu_mux_reg_TXJC_RES_OVERFLOW_VAL_array_poll                      ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_mux_reg_SW_OVFLW_INT_VAL_array_burst_read                     ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_mux_reg_SW_OVFLW_INT_VAL_array_read                           ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE odtu_mux_reg_SW_OVFLW_INT_VAL_array_poll                           ( odtu_mux_buffer_t *b_ptr, odtu_mux_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _ODTU_MUX_IO_H */
