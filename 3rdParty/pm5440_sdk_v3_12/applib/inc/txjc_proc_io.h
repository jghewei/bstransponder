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
 *     and register accessor functions for the txjc_proc block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing txjc_proc io functions is:
 | {
 |     txjc_proc_buffer_t b_ptr[1];
 |     txjc_proc_buffer_init( b_ptr, h_ptr );
 |     value = txjc_proc_field_<FIELD>_get( b_ptr, h_ptr );
 |     txjc_proc_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     txjc_proc_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = txjc_proc_field_<FIELD>_get( NULL, h_ptr );
 | txjc_proc_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-------------------------------------------------------+
 | | Register (0x00000000 + (N) * 0x10) CHANNEL_COMMON_CFG |
 | |     N[0..95]                                          |
 | +-------------------------------------------------------+
 | | bit  5   R/W  SPECIAL_MODE_2_SEL                      |
 | | bit  4   R/W  PREV_CN_SEL                             |
 | | bit  3:2 R/W  PJO_CNT                                 |
 | | bit  1:0 R/W  NJO_CNT                                 |
 | +-------------------------------------------------------+
 | 
 | void   txjc_proc_field_SPECIAL_MODE_2_SEL_set ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 txjc_proc_field_SPECIAL_MODE_2_SEL_get ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N )
 | void   txjc_proc_field_PREV_CN_SEL_set        ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 txjc_proc_field_PREV_CN_SEL_get        ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N )
 | void   txjc_proc_field_PJO_CNT_set            ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 txjc_proc_field_PJO_CNT_get            ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N )
 | void   txjc_proc_field_range_PJO_CNT_set      ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 txjc_proc_field_range_PJO_CNT_get      ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   txjc_proc_field_NJO_CNT_set            ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 txjc_proc_field_NJO_CNT_get            ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N )
 | void   txjc_proc_field_range_NJO_CNT_set      ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 txjc_proc_field_range_NJO_CNT_get      ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x00000004 + (N) * 0x10) GOLDEN_CN_REG |
 | |     N[0..95]                                     |
 | +--------------------------------------------------+
 | | bit  23:0 R/W  CN_GOLDEN                         |
 | +--------------------------------------------------+
 | 
 | void   txjc_proc_field_CN_GOLDEN_set       ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 txjc_proc_field_CN_GOLDEN_get       ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N )
 | void   txjc_proc_field_range_CN_GOLDEN_set ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 txjc_proc_field_range_CN_GOLDEN_get ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x00000008 + (N) * 0x10) CN_RAMP_NUMERATOR |
 | |     N[0..95]                                         |
 | +------------------------------------------------------+
 | | bit  23:0 R/W  CN_GOLDEN_NUMERATOR                   |
 | +------------------------------------------------------+
 | 
 | void   txjc_proc_field_CN_GOLDEN_NUMERATOR_set       ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 txjc_proc_field_CN_GOLDEN_NUMERATOR_get       ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N )
 | void   txjc_proc_field_range_CN_GOLDEN_NUMERATOR_set ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 txjc_proc_field_range_CN_GOLDEN_NUMERATOR_get ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x0000000c + (N) * 0x10) CN_RAMP_DENOMINATOR |
 | |     N[0..95]                                           |
 | +--------------------------------------------------------+
 | | bit  23:0 R/W  CN_GOLDEN_DENOMINATOR                   |
 | +--------------------------------------------------------+
 | 
 | void   txjc_proc_field_CN_GOLDEN_DENOMINATOR_set       ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 txjc_proc_field_CN_GOLDEN_DENOMINATOR_get       ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N )
 | void   txjc_proc_field_range_CN_GOLDEN_DENOMINATOR_set ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 txjc_proc_field_range_CN_GOLDEN_DENOMINATOR_get ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _TXJC_PROC_IO_H
#define _TXJC_PROC_IO_H

#include "txjc_proc_loc.h"
#include "txjc_proc_regs.h"
#include "txjc_proc_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 txjc_proc_buffer_init               ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr )
 | void                 txjc_proc_buffer_flush              ( txjc_proc_buffer_t *b_ptr )
 | UINT32               txjc_proc_reg_read                  ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 txjc_proc_reg_write                 ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 txjc_proc_field_set                 ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 txjc_proc_action_on_write_field_set ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 txjc_proc_burst_read                ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 txjc_proc_burst_write               ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE txjc_proc_poll                      ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   txjc_proc_reg_CHANNEL_COMMON_CFG_array_write      ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   txjc_proc_reg_CHANNEL_COMMON_CFG_array_field_set  ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 txjc_proc_reg_CHANNEL_COMMON_CFG_array_read       ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N )
 | 
 | void   txjc_proc_reg_GOLDEN_CN_REG_array_write           ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   txjc_proc_reg_GOLDEN_CN_REG_array_field_set       ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 txjc_proc_reg_GOLDEN_CN_REG_array_read            ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N )
 | 
 | void   txjc_proc_reg_CN_RAMP_NUMERATOR_array_write       ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   txjc_proc_reg_CN_RAMP_NUMERATOR_array_field_set   ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 txjc_proc_reg_CN_RAMP_NUMERATOR_array_read        ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N )
 | 
 | void   txjc_proc_reg_CN_RAMP_DENOMINATOR_array_write     ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   txjc_proc_reg_CN_RAMP_DENOMINATOR_array_field_set ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 txjc_proc_reg_CN_RAMP_DENOMINATOR_array_read      ( txjc_proc_buffer_t *b_ptr, txjc_proc_handle_t *h_ptr, UINT32  N )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _TXJC_PROC_IO_H */
