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
 *     and register accessor functions for the i4_fec block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing i4_fec io functions is:
 | {
 |     i4_fec_buffer_t b_ptr[1];
 |     i4_fec_buffer_init( b_ptr, h_ptr, A );
 |     value = i4_fec_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     i4_fec_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     i4_fec_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = i4_fec_field_<FIELD>_get( NULL, h_ptr, A );
 | i4_fec_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------------+
 | | Register 0x00028000 FEC_ENCODER_CONFIG |
 | +----------------------------------------+
 | | bit  0 R/W  ENCODER_DATA_TYPE          |
 | +----------------------------------------+
 | 
 | void   i4_fec_field_ENCODER_DATA_TYPE_set ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 i4_fec_field_ENCODER_DATA_TYPE_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------------+
 | | Register 0x00028004 FEC_DECODER_CONFIG |
 | +----------------------------------------+
 | | bit  11:8 R/W  DECODER_LN_DISABLE      |
 | | bit  5:4  R/W  DECODER_STATS_ENABLE    |
 | | bit  1:0  R/W  DECODER_OP_MODE         |
 | +----------------------------------------+
 | 
 | void   i4_fec_field_DECODER_LN_DISABLE_set         ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 i4_fec_field_DECODER_LN_DISABLE_get         ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   i4_fec_field_range_DECODER_LN_DISABLE_set   ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 i4_fec_field_range_DECODER_LN_DISABLE_get   ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   i4_fec_field_DECODER_STATS_ENABLE_set       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 i4_fec_field_DECODER_STATS_ENABLE_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   i4_fec_field_range_DECODER_STATS_ENABLE_set ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 i4_fec_field_range_DECODER_STATS_ENABLE_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   i4_fec_field_DECODER_OP_MODE_set            ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 i4_fec_field_DECODER_OP_MODE_get            ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   i4_fec_field_range_DECODER_OP_MODE_set      ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 i4_fec_field_range_DECODER_OP_MODE_get      ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00028008 CORRECTED_ZEROES_3_CNT |
 | +--------------------------------------------+
 | | bit  31:0 R  CORRECTED_ZEROES_3_COUNT      |
 | +--------------------------------------------+
 | 
 | UINT32 i4_fec_field_CORRECTED_ZEROES_3_COUNT_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i4_fec_field_range_CORRECTED_ZEROES_3_COUNT_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x0002800c CORRECTED_ZEROES_2_CNT |
 | +--------------------------------------------+
 | | bit  31:0 R  CORRECTED_ZEROES_2_COUNT      |
 | +--------------------------------------------+
 | 
 | UINT32 i4_fec_field_CORRECTED_ZEROES_2_COUNT_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i4_fec_field_range_CORRECTED_ZEROES_2_COUNT_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00028010 CORRECTED_ZEROES_1_CNT |
 | +--------------------------------------------+
 | | bit  31:0 R  CORRECTED_ZEROES_1_COUNT      |
 | +--------------------------------------------+
 | 
 | UINT32 i4_fec_field_CORRECTED_ZEROES_1_COUNT_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i4_fec_field_range_CORRECTED_ZEROES_1_COUNT_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00028014 CORRECTED_ZEROES_0_CNT |
 | +--------------------------------------------+
 | | bit  31:0 R  CORRECTED_ZEROES_0_COUNT      |
 | +--------------------------------------------+
 | 
 | UINT32 i4_fec_field_CORRECTED_ZEROES_0_COUNT_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i4_fec_field_range_CORRECTED_ZEROES_0_COUNT_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00028018 CORRECTED_ONES_3_CNT |
 | +------------------------------------------+
 | | bit  31:0 R  CORRECTED_ONES_3_COUNT      |
 | +------------------------------------------+
 | 
 | UINT32 i4_fec_field_CORRECTED_ONES_3_COUNT_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i4_fec_field_range_CORRECTED_ONES_3_COUNT_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0002801c CORRECTED_ONES_2_CNT |
 | +------------------------------------------+
 | | bit  31:0 R  CORRECTED_ONES_2_COUNT      |
 | +------------------------------------------+
 | 
 | UINT32 i4_fec_field_CORRECTED_ONES_2_COUNT_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i4_fec_field_range_CORRECTED_ONES_2_COUNT_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00028020 CORRECTED_ONES_1_CNT |
 | +------------------------------------------+
 | | bit  31:0 R  CORRECTED_ONES_1_COUNT      |
 | +------------------------------------------+
 | 
 | UINT32 i4_fec_field_CORRECTED_ONES_1_COUNT_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i4_fec_field_range_CORRECTED_ONES_1_COUNT_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00028024 CORRECTED_ONES_0_CNT |
 | +------------------------------------------+
 | | bit  31:0 R  CORRECTED_ONES_0_COUNT      |
 | +------------------------------------------+
 | 
 | UINT32 i4_fec_field_CORRECTED_ONES_0_COUNT_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i4_fec_field_range_CORRECTED_ONES_0_COUNT_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00028028 UNCORRECTED_ERRS_3_CNT |
 | +--------------------------------------------+
 | | bit  31:0 R  UNCORR_ERROR_3_COUNT          |
 | +--------------------------------------------+
 | 
 | UINT32 i4_fec_field_UNCORR_ERROR_3_COUNT_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i4_fec_field_range_UNCORR_ERROR_3_COUNT_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x0002802c UNCORRECTED_ERRS_2_CNT |
 | +--------------------------------------------+
 | | bit  31:0 R  UNCORR_ERROR_2_COUNT          |
 | +--------------------------------------------+
 | 
 | UINT32 i4_fec_field_UNCORR_ERROR_2_COUNT_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i4_fec_field_range_UNCORR_ERROR_2_COUNT_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00028030 UNCORRECTED_ERRS_1_CNT |
 | +--------------------------------------------+
 | | bit  31:0 R  UNCORR_ERROR_1_COUNT          |
 | +--------------------------------------------+
 | 
 | UINT32 i4_fec_field_UNCORR_ERROR_1_COUNT_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i4_fec_field_range_UNCORR_ERROR_1_COUNT_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00028034 UNCORRECTED_ERRS_0_CNT |
 | +--------------------------------------------+
 | | bit  31:0 R  UNCORR_ERROR_0_COUNT          |
 | +--------------------------------------------+
 | 
 | UINT32 i4_fec_field_UNCORR_ERROR_0_COUNT_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i4_fec_field_range_UNCORR_ERROR_0_COUNT_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00028038 RX_FRAME_PULSE_3_CNT |
 | +------------------------------------------+
 | | bit  31:0 R  RX_FRM_PULSE_3_COUNT        |
 | +------------------------------------------+
 | 
 | UINT32 i4_fec_field_RX_FRM_PULSE_3_COUNT_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i4_fec_field_range_RX_FRM_PULSE_3_COUNT_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0002803c RX_FRAME_PULSE_2_CNT |
 | +------------------------------------------+
 | | bit  31:0 R  RX_FRM_PULSE_2_COUNT        |
 | +------------------------------------------+
 | 
 | UINT32 i4_fec_field_RX_FRM_PULSE_2_COUNT_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i4_fec_field_range_RX_FRM_PULSE_2_COUNT_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00028040 RX_FRAME_PULSE_1_CNT |
 | +------------------------------------------+
 | | bit  31:0 R  RX_FRM_PULSE_1_COUNT        |
 | +------------------------------------------+
 | 
 | UINT32 i4_fec_field_RX_FRM_PULSE_1_COUNT_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i4_fec_field_range_RX_FRM_PULSE_1_COUNT_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00028044 RX_FRAME_PULSE_0_CNT |
 | +------------------------------------------+
 | | bit  31:0 R  RX_FRM_PULSE_0_COUNT        |
 | +------------------------------------------+
 | 
 | UINT32 i4_fec_field_RX_FRM_PULSE_0_COUNT_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i4_fec_field_range_RX_FRM_PULSE_0_COUNT_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------------+
 | | Register (0x00028048 + (N) * 4) LLSA_STATISTICS_CORRECTED_0S_CNT |
 | |     N[0..15]                                                     |
 | +------------------------------------------------------------------+
 | | bit  31:0 R  LLSA_STATS_LANE_CORR_0S                             |
 | +------------------------------------------------------------------+
 | 
 | void   i4_fec_lfield_LLSA_STATS_LANE_CORR_0S_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value[16] )
 | UINT32 i4_fec_lfield_range_LLSA_STATS_LANE_CORR_0S_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------------+
 | | Register (0x00028088 + (N) * 4) LLSA_STATISTICS_CORRECTED_1S_CNT |
 | |     N[0..15]                                                     |
 | +------------------------------------------------------------------+
 | | bit  31:0 R  LLSA_STATS_LANE_CORR_1S                             |
 | +------------------------------------------------------------------+
 | 
 | void   i4_fec_lfield_LLSA_STATS_LANE_CORR_1S_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value[16] )
 | UINT32 i4_fec_lfield_range_LLSA_STATS_LANE_CORR_1S_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000280c8 LLSA_UNCORRECTED_ERRS_3_CNT |
 | +-------------------------------------------------+
 | | bit  31:0 R  LLSA_UNCORR_ERROR_3_COUNT          |
 | +-------------------------------------------------+
 | 
 | UINT32 i4_fec_field_LLSA_UNCORR_ERROR_3_COUNT_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i4_fec_field_range_LLSA_UNCORR_ERROR_3_COUNT_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x000280d8 LPA_CNT_MODE |
 | +----------------------------------+
 | | bit  1:0 R/W  LPA_COUNTER_MODE   |
 | +----------------------------------+
 | 
 | void   i4_fec_field_LPA_COUNTER_MODE_set       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 i4_fec_field_LPA_COUNTER_MODE_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   i4_fec_field_range_LPA_COUNTER_MODE_set ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 i4_fec_field_range_LPA_COUNTER_MODE_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x000280e8 LLSA_CNT_MODE |
 | +-----------------------------------+
 | | bit  1:0 R/W  LLSA_COUNTER_MODE   |
 | +-----------------------------------+
 | 
 | void   i4_fec_field_LLSA_COUNTER_MODE_set       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 i4_fec_field_LLSA_COUNTER_MODE_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   i4_fec_field_range_LLSA_COUNTER_MODE_set ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 i4_fec_field_range_LLSA_COUNTER_MODE_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x000280ec LLSA_LATCH_SOURCE |
 | +---------------------------------------+
 | | bit  0 R/W  LLSA_LATCH_SOURCE         |
 | +---------------------------------------+
 | 
 | void   i4_fec_field_LLSA_LATCH_SOURCE_set ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 i4_fec_field_LLSA_LATCH_SOURCE_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------------------+
 | | Register 0x0002810c LLSA_STATISTICS_FAILURE_STAT |
 | +--------------------------------------------------+
 | | bit  3:0 R  LLSA_STATS_OTU_FAIL                  |
 | +--------------------------------------------------+
 | 
 | UINT32 i4_fec_field_LLSA_STATS_OTU_FAIL_get       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 i4_fec_field_range_LLSA_STATS_OTU_FAIL_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------+
 | | Register 0x00028114 FEC_CTRL |
 | +------------------------------+
 | | bit  2 R/W  FEC_RAM_LOWPWR   |
 | | bit  1 R/W  FEC_LOWPWR       |
 | | bit  0 R/W  FEC_SW_RST       |
 | +------------------------------+
 | 
 | void   i4_fec_field_FEC_RAM_LOWPWR_set ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 i4_fec_field_FEC_RAM_LOWPWR_get ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   i4_fec_field_FEC_LOWPWR_set     ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 i4_fec_field_FEC_LOWPWR_get     ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   i4_fec_field_FEC_SW_RST_set     ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 i4_fec_field_FEC_SW_RST_get     ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _I4_FEC_IO_H
#define _I4_FEC_IO_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "i4_fec_regs.h"
#include "i4_fec_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 i4_fec_buffer_init               ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void                 i4_fec_buffer_flush              ( i4_fec_buffer_t *b_ptr )
 | UINT32               i4_fec_reg_read                  ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 i4_fec_reg_write                 ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 i4_fec_field_set                 ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 i4_fec_action_on_write_field_set ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 i4_fec_burst_read                ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 i4_fec_burst_write               ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE i4_fec_poll                      ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   i4_fec_reg_FEC_ENCODER_CONFIG_write                          ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   i4_fec_reg_FEC_ENCODER_CONFIG_field_set                      ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 i4_fec_reg_FEC_ENCODER_CONFIG_read                           ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void   i4_fec_reg_FEC_DECODER_CONFIG_write                          ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   i4_fec_reg_FEC_DECODER_CONFIG_field_set                      ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 i4_fec_reg_FEC_DECODER_CONFIG_read                           ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void   i4_fec_reg_LPA_CNT_MODE_write                                ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   i4_fec_reg_LPA_CNT_MODE_field_set                            ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 i4_fec_reg_LPA_CNT_MODE_read                                 ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void   i4_fec_reg_LLSA_CNT_MODE_write                               ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   i4_fec_reg_LLSA_CNT_MODE_field_set                           ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 i4_fec_reg_LLSA_CNT_MODE_read                                ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void   i4_fec_reg_LLSA_LATCH_SOURCE_write                           ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   i4_fec_reg_LLSA_LATCH_SOURCE_field_set                       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 i4_fec_reg_LLSA_LATCH_SOURCE_read                            ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void   i4_fec_reg_FEC_CTRL_write                                    ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   i4_fec_reg_FEC_CTRL_field_set                                ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 i4_fec_reg_FEC_CTRL_read                                     ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i4_fec_reg_CORRECTED_ZEROES_3_CNT_read                       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i4_fec_reg_CORRECTED_ZEROES_2_CNT_read                       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i4_fec_reg_CORRECTED_ZEROES_1_CNT_read                       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i4_fec_reg_CORRECTED_ZEROES_0_CNT_read                       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i4_fec_reg_CORRECTED_ONES_3_CNT_read                         ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i4_fec_reg_CORRECTED_ONES_2_CNT_read                         ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i4_fec_reg_CORRECTED_ONES_1_CNT_read                         ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i4_fec_reg_CORRECTED_ONES_0_CNT_read                         ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i4_fec_reg_UNCORRECTED_ERRS_3_CNT_read                       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i4_fec_reg_UNCORRECTED_ERRS_2_CNT_read                       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i4_fec_reg_UNCORRECTED_ERRS_1_CNT_read                       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i4_fec_reg_UNCORRECTED_ERRS_0_CNT_read                       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i4_fec_reg_RX_FRAME_PULSE_3_CNT_read                         ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i4_fec_reg_RX_FRAME_PULSE_2_CNT_read                         ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i4_fec_reg_RX_FRAME_PULSE_1_CNT_read                         ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i4_fec_reg_RX_FRAME_PULSE_0_CNT_read                         ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i4_fec_reg_LLSA_UNCORRECTED_ERRS_3_CNT_read                  ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 i4_fec_reg_LLSA_STATISTICS_FAILURE_STAT_read                 ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void   i4_fec_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_array_burst_read ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32 i4_fec_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_array_read       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 N )
 | 
 | void   i4_fec_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_array_burst_read ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32 i4_fec_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_array_read       ( i4_fec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 N )
 | 
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _I4_FEC_IO_H */
