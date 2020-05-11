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
 *     and register accessor functions for the sysotn_gfec block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing sysotn_gfec io functions is:
 | {
 |     sysotn_gfec_buffer_t b_ptr[1];
 |     sysotn_gfec_buffer_init( b_ptr, h_ptr );
 |     value = sysotn_gfec_field_<FIELD>_get( b_ptr, h_ptr );
 |     sysotn_gfec_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     sysotn_gfec_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = sysotn_gfec_field_<FIELD>_get( NULL, h_ptr );
 | sysotn_gfec_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +------------------------------------+
 | | Register 0x00005000 RS_FEC_MODE    |
 | +------------------------------------+
 | | bit  31:28 R/W  REG_TX_ENCODE_TYPE |
 | | bit  27:24 R/W  REG_RX_DECODE_TYPE |
 | | bit  23:12 R/W  REG_DISABLE_DEC    |
 | | bit  7     R/W  REG_SHUT_DOWN      |
 | | bit  6     R/W  REG_DEEP_SLEEP     |
 | | bit  5     R/W  REG_DISABLE_UNCORR |
 | | bit  4     R/W  REG_DISABLE_SCRAM  |
 | | bit  3     R/W  FEC_SW_RST         |
 | | bit  0     R/W  GFEC_LOWPWR        |
 | +------------------------------------+
 | 
 | void   sysotn_gfec_field_REG_TX_ENCODE_TYPE_set       ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_gfec_field_REG_TX_ENCODE_TYPE_get       ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_gfec_field_range_REG_TX_ENCODE_TYPE_set ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_gfec_field_range_REG_TX_ENCODE_TYPE_get ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_gfec_field_REG_RX_DECODE_TYPE_set       ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_gfec_field_REG_RX_DECODE_TYPE_get       ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_gfec_field_range_REG_RX_DECODE_TYPE_set ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_gfec_field_range_REG_RX_DECODE_TYPE_get ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_gfec_field_REG_DISABLE_DEC_set          ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_gfec_field_REG_DISABLE_DEC_get          ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_gfec_field_range_REG_DISABLE_DEC_set    ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_gfec_field_range_REG_DISABLE_DEC_get    ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_gfec_field_REG_SHUT_DOWN_set            ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_gfec_field_REG_SHUT_DOWN_get            ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_gfec_field_REG_DEEP_SLEEP_set           ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_gfec_field_REG_DEEP_SLEEP_get           ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_gfec_field_REG_DISABLE_UNCORR_set       ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_gfec_field_REG_DISABLE_UNCORR_get       ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_gfec_field_REG_DISABLE_SCRAM_set        ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_gfec_field_REG_DISABLE_SCRAM_get        ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_gfec_field_FEC_SW_RST_set               ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_gfec_field_FEC_SW_RST_get               ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_gfec_field_GFEC_LOWPWR_set              ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_gfec_field_GFEC_LOWPWR_get              ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x00005008 LPA_STATISTICS_MODE_CFG |
 | +---------------------------------------------+
 | | bit  1:0 R/W  LPA_STATS_COUNTER_MODE        |
 | +---------------------------------------------+
 | 
 | void   sysotn_gfec_field_LPA_STATS_COUNTER_MODE_set       ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_gfec_field_LPA_STATS_COUNTER_MODE_get       ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_gfec_field_range_LPA_STATS_COUNTER_MODE_set ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_gfec_field_range_LPA_STATS_COUNTER_MODE_get ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register (0x00005020 + (N) * 4)                               |
 | | LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11 |
 | |     N[0..11]                                                  |
 | +---------------------------------------------------------------+
 | | bit  31:0 R  LPA_STATS_FP                                     |
 | +---------------------------------------------------------------+
 | 
 | void   sysotn_gfec_lfield_LPA_STATS_FP_get       ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value[12] )
 | UINT32 sysotn_gfec_lfield_range_LPA_STATS_FP_get ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------------------------------------------------+
 | | Register (0x00005050 + (N) * 4) LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11 |
 | |     N[0..11]                                                                                        |
 | +-----------------------------------------------------------------------------------------------------+
 | | bit  31:0 R  LPA_STATS_UNCORR_CWDS                                                                  |
 | +-----------------------------------------------------------------------------------------------------+
 | 
 | void   sysotn_gfec_lfield_LPA_STATS_UNCORR_CWDS_get       ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value[12] )
 | UINT32 sysotn_gfec_lfield_range_LPA_STATS_UNCORR_CWDS_get ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------------------------------------+
 | | Register (0x00005080 + (N) * 4) LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11 |
 | |     N[0..11]                                                                               |
 | +--------------------------------------------------------------------------------------------+
 | | bit  31:0 R  LPA_STATS_CORR_1S                                                             |
 | +--------------------------------------------------------------------------------------------+
 | 
 | void   sysotn_gfec_lfield_LPA_STATS_CORR_1S_get       ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value[12] )
 | UINT32 sysotn_gfec_lfield_range_LPA_STATS_CORR_1S_get ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------------------------------------+
 | | Register (0x000050b0 + (N) * 4) LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11 |
 | |     N[0..11]                                                                               |
 | +--------------------------------------------------------------------------------------------+
 | | bit  31:0 R  LPA_STATS_CORR_0S                                                             |
 | +--------------------------------------------------------------------------------------------+
 | 
 | void   sysotn_gfec_lfield_LPA_STATS_CORR_0S_get       ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value[12] )
 | UINT32 sysotn_gfec_lfield_range_LPA_STATS_CORR_0S_get ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _SYSOTN_GFEC_IO_H
#define _SYSOTN_GFEC_IO_H

#include "lineotn_loc.h"
#include "sysotn_gfec_regs.h"
#include "sysotn_gfec_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sysotn_gfec_buffer_init               ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void                 sysotn_gfec_buffer_flush              ( sysotn_gfec_buffer_t *b_ptr )
 | UINT32               sysotn_gfec_reg_read                  ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 sysotn_gfec_reg_write                 ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 sysotn_gfec_field_set                 ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 sysotn_gfec_action_on_write_field_set ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 sysotn_gfec_burst_read                ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 sysotn_gfec_burst_write               ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE sysotn_gfec_poll                      ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   sysotn_gfec_reg_RS_FEC_MODE_write                                                                    ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void   sysotn_gfec_reg_RS_FEC_MODE_field_set                                                                ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sysotn_gfec_reg_RS_FEC_MODE_read                                                                     ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void   sysotn_gfec_reg_LPA_STATISTICS_MODE_CFG_write                                                        ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void   sysotn_gfec_reg_LPA_STATISTICS_MODE_CFG_field_set                                                    ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sysotn_gfec_reg_LPA_STATISTICS_MODE_CFG_read                                                         ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void   sysotn_gfec_reg_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read       ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32 sysotn_gfec_reg_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_read             ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 N )
 | 
 | void   sysotn_gfec_reg_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32 sysotn_gfec_reg_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_read       ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 N )
 | 
 | void   sysotn_gfec_reg_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read          ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32 sysotn_gfec_reg_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_read                ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 N )
 | 
 | void   sysotn_gfec_reg_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read          ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32 sysotn_gfec_reg_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_read                ( sysotn_gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 N )
 | 
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _SYSOTN_GFEC_IO_H */
