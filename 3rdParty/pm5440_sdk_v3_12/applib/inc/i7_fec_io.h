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
 *     and register accessor functions for the i7_fec block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing i7_fec io functions is:
 | {
 |     i7_fec_buffer_t b_ptr[1];
 |     i7_fec_buffer_init( b_ptr, h_ptr, A );
 |     value = i7_fec_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     i7_fec_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     i7_fec_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = i7_fec_field_<FIELD>_get( NULL, h_ptr, A );
 | i7_fec_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +--------------------------------+
 | | Register 0x0002e000 FEC_CONFIG |
 | +--------------------------------+
 | | bit  7:6 R/W  STATS_ENABLE     |
 | | bit  5:2 R/W  DECODER_DISABLE  |
 | | bit  1   R/W  DECODE_TYPE      |
 | | bit  0   R/W  ENCODE_TYPE      |
 | +--------------------------------+
 | 
 | void   i7_fec_field_STATS_ENABLE_set          ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 i7_fec_field_STATS_ENABLE_get          ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   i7_fec_field_range_STATS_ENABLE_set    ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 i7_fec_field_range_STATS_ENABLE_get    ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   i7_fec_field_DECODER_DISABLE_set       ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 i7_fec_field_DECODER_DISABLE_get       ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   i7_fec_field_range_DECODER_DISABLE_set ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 i7_fec_field_range_DECODER_DISABLE_get ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   i7_fec_field_DECODE_TYPE_set           ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 i7_fec_field_DECODE_TYPE_get           ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   i7_fec_field_ENCODE_TYPE_set           ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 i7_fec_field_ENCODE_TYPE_get           ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------------+
 | | Register 0x0002e004 CORRECTED_ZEROES_3_CNT |
 | +--------------------------------------------+
 | | bit  31:0 R  CORRECTED_ZEROES_3_COUNT      |
 | +--------------------------------------------+
 | 
 | UINT32 i7_fec_field_CORRECTED_ZEROES_3_COUNT_get       ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i7_fec_field_range_CORRECTED_ZEROES_3_COUNT_get ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x0002e008 CORRECTED_ZEROES_2_CNT |
 | +--------------------------------------------+
 | | bit  31:0 R  CORRECTED_ZEROES_2_COUNT      |
 | +--------------------------------------------+
 | 
 | UINT32 i7_fec_field_CORRECTED_ZEROES_2_COUNT_get       ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i7_fec_field_range_CORRECTED_ZEROES_2_COUNT_get ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x0002e00c CORRECTED_ZEROES_1_CNT |
 | +--------------------------------------------+
 | | bit  31:0 R  CORRECTED_ZEROES_1_COUNT      |
 | +--------------------------------------------+
 | 
 | UINT32 i7_fec_field_CORRECTED_ZEROES_1_COUNT_get       ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i7_fec_field_range_CORRECTED_ZEROES_1_COUNT_get ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x0002e010 CORRECTED_ZEROES_0_CNT |
 | +--------------------------------------------+
 | | bit  31:0 R  CORRECTED_ZEROES_0_COUNT      |
 | +--------------------------------------------+
 | 
 | UINT32 i7_fec_field_CORRECTED_ZEROES_0_COUNT_get       ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i7_fec_field_range_CORRECTED_ZEROES_0_COUNT_get ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0002e014 CORRECTED_ONES_3_CNT |
 | +------------------------------------------+
 | | bit  31:0 R  CORRECTED_ONES_3_COUNT      |
 | +------------------------------------------+
 | 
 | UINT32 i7_fec_field_CORRECTED_ONES_3_COUNT_get       ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i7_fec_field_range_CORRECTED_ONES_3_COUNT_get ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0002e018 CORRECTED_ONES_2_CNT |
 | +------------------------------------------+
 | | bit  31:0 R  CORRECTED_ONES_2_COUNT      |
 | +------------------------------------------+
 | 
 | UINT32 i7_fec_field_CORRECTED_ONES_2_COUNT_get       ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i7_fec_field_range_CORRECTED_ONES_2_COUNT_get ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0002e01c CORRECTED_ONES_1_CNT |
 | +------------------------------------------+
 | | bit  31:0 R  CORRECTED_ONES_1_COUNT      |
 | +------------------------------------------+
 | 
 | UINT32 i7_fec_field_CORRECTED_ONES_1_COUNT_get       ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i7_fec_field_range_CORRECTED_ONES_1_COUNT_get ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0002e020 CORRECTED_ONES_0_CNT |
 | +------------------------------------------+
 | | bit  31:0 R  CORRECTED_ONES_0_COUNT      |
 | +------------------------------------------+
 | 
 | UINT32 i7_fec_field_CORRECTED_ONES_0_COUNT_get       ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i7_fec_field_range_CORRECTED_ONES_0_COUNT_get ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x0002e024 UNCORRECTED_ERRS_3_CNT |
 | +--------------------------------------------+
 | | bit  31:0 R  UNCORR_ERROR_3_COUNT          |
 | +--------------------------------------------+
 | 
 | UINT32 i7_fec_field_UNCORR_ERROR_3_COUNT_get       ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i7_fec_field_range_UNCORR_ERROR_3_COUNT_get ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x0002e028 UNCORRECTED_ERRS_2_CNT |
 | +--------------------------------------------+
 | | bit  31:0 R  UNCORR_ERROR_2_COUNT          |
 | +--------------------------------------------+
 | 
 | UINT32 i7_fec_field_UNCORR_ERROR_2_COUNT_get       ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i7_fec_field_range_UNCORR_ERROR_2_COUNT_get ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x0002e02c UNCORRECTED_ERRS_1_CNT |
 | +--------------------------------------------+
 | | bit  31:0 R  UNCORR_ERROR_1_COUNT          |
 | +--------------------------------------------+
 | 
 | UINT32 i7_fec_field_UNCORR_ERROR_1_COUNT_get       ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i7_fec_field_range_UNCORR_ERROR_1_COUNT_get ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x0002e030 UNCORRECTED_ERRS_0_CNT |
 | +--------------------------------------------+
 | | bit  31:0 R  UNCORR_ERROR_0_COUNT          |
 | +--------------------------------------------+
 | 
 | UINT32 i7_fec_field_UNCORR_ERROR_0_COUNT_get       ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i7_fec_field_range_UNCORR_ERROR_0_COUNT_get ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0002e034 RX_FRAME_PULSE_3_CNT |
 | +------------------------------------------+
 | | bit  31:0 R  RX_FRM_PULSE_3_COUNT        |
 | +------------------------------------------+
 | 
 | UINT32 i7_fec_field_RX_FRM_PULSE_3_COUNT_get       ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i7_fec_field_range_RX_FRM_PULSE_3_COUNT_get ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0002e038 RX_FRAME_PULSE_2_CNT |
 | +------------------------------------------+
 | | bit  31:0 R  RX_FRM_PULSE_2_COUNT        |
 | +------------------------------------------+
 | 
 | UINT32 i7_fec_field_RX_FRM_PULSE_2_COUNT_get       ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i7_fec_field_range_RX_FRM_PULSE_2_COUNT_get ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0002e03c RX_FRAME_PULSE_1_CNT |
 | +------------------------------------------+
 | | bit  31:0 R  RX_FRM_PULSE_1_COUNT        |
 | +------------------------------------------+
 | 
 | UINT32 i7_fec_field_RX_FRM_PULSE_1_COUNT_get       ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i7_fec_field_range_RX_FRM_PULSE_1_COUNT_get ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0002e040 RX_FRAME_PULSE_0_CNT |
 | +------------------------------------------+
 | | bit  31:0 R  RX_FRM_PULSE_0_COUNT        |
 | +------------------------------------------+
 | 
 | UINT32 i7_fec_field_RX_FRM_PULSE_0_COUNT_get       ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i7_fec_field_range_RX_FRM_PULSE_0_COUNT_get ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x0002e0a8 LLSA_LATCH_SOURCE |
 | +---------------------------------------+
 | | bit  0 R/W  LLSA_LATCH_SOURCE         |
 | +---------------------------------------+
 | 
 | void   i7_fec_field_LLSA_LATCH_SOURCE_set ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 i7_fec_field_LLSA_LATCH_SOURCE_get ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | +-------------------------------+
 | | Register 0x0002e0cc FEC_CTRL  |
 | +-------------------------------+
 | | bit  2 R/W  FEC_SW_RAM_LOWPWR |
 | | bit  1 R/W  FEC_SW_LOWPWR     |
 | | bit  0 R/W  FEC_SW_RST        |
 | +-------------------------------+
 | 
 | void   i7_fec_field_FEC_SW_RAM_LOWPWR_set ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 i7_fec_field_FEC_SW_RAM_LOWPWR_get ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   i7_fec_field_FEC_SW_LOWPWR_set     ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 i7_fec_field_FEC_SW_LOWPWR_get     ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   i7_fec_field_FEC_SW_RST_set        ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 i7_fec_field_FEC_SW_RST_get        ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _I7_FEC_IO_H
#define _I7_FEC_IO_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "i7_fec_regs.h"
#include "i7_fec_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 i7_fec_buffer_init               ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void                 i7_fec_buffer_flush              ( i7_fec_buffer_t *b_ptr )
 | UINT32               i7_fec_reg_read                  ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 i7_fec_reg_write                 ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 i7_fec_field_set                 ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 i7_fec_action_on_write_field_set ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 i7_fec_burst_read                ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 i7_fec_burst_write               ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE i7_fec_poll                      ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   i7_fec_reg_FEC_CONFIG_write            ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   i7_fec_reg_FEC_CONFIG_field_set        ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 i7_fec_reg_FEC_CONFIG_read             ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void   i7_fec_reg_LLSA_LATCH_SOURCE_write     ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   i7_fec_reg_LLSA_LATCH_SOURCE_field_set ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 i7_fec_reg_LLSA_LATCH_SOURCE_read      ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void   i7_fec_reg_FEC_CTRL_write              ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   i7_fec_reg_FEC_CTRL_field_set          ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 i7_fec_reg_FEC_CTRL_read               ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i7_fec_reg_CORRECTED_ZEROES_3_CNT_read ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i7_fec_reg_CORRECTED_ZEROES_2_CNT_read ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i7_fec_reg_CORRECTED_ZEROES_1_CNT_read ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i7_fec_reg_CORRECTED_ZEROES_0_CNT_read ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i7_fec_reg_CORRECTED_ONES_3_CNT_read   ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i7_fec_reg_CORRECTED_ONES_2_CNT_read   ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i7_fec_reg_CORRECTED_ONES_1_CNT_read   ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i7_fec_reg_CORRECTED_ONES_0_CNT_read   ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i7_fec_reg_UNCORRECTED_ERRS_3_CNT_read ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i7_fec_reg_UNCORRECTED_ERRS_2_CNT_read ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i7_fec_reg_UNCORRECTED_ERRS_1_CNT_read ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i7_fec_reg_UNCORRECTED_ERRS_0_CNT_read ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i7_fec_reg_RX_FRAME_PULSE_3_CNT_read   ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i7_fec_reg_RX_FRAME_PULSE_2_CNT_read   ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i7_fec_reg_RX_FRAME_PULSE_1_CNT_read   ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i7_fec_reg_RX_FRAME_PULSE_0_CNT_read   ( i7_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _I7_FEC_IO_H */
