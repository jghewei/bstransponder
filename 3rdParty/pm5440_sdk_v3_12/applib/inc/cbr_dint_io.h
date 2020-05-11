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
 *     and register accessor functions for the cbr_dint block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing cbr_dint io functions is:
 | {
 |     cbr_dint_buffer_t b_ptr[1];
 |     cbr_dint_buffer_init( b_ptr, h_ptr );
 |     value = cbr_dint_field_<FIELD>_get( b_ptr, h_ptr );
 |     cbr_dint_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     cbr_dint_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = cbr_dint_field_<FIELD>_get( NULL, h_ptr );
 | cbr_dint_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------------------------------------+
 | | Register (0x00040000 + (N) * 4) MULTI_LANE_40G_GROUP_CONFIG_REG |
 | |     N[0..2]                                                     |
 | +-----------------------------------------------------------------+
 | | bit  0 R/W  MULTI_LANE_40G_SEL                                  |
 | +-----------------------------------------------------------------+
 | 
 | void   cbr_dint_field_MULTI_LANE_40G_SEL_set ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 cbr_dint_field_MULTI_LANE_40G_SEL_get ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | +----------------------------------------+
 | | Register 0x0004000c CHANNEL_UPDATE_REG |
 | +----------------------------------------+
 | | bit  0 R/W  CH_UPDATE                  |
 | +----------------------------------------+
 | 
 | void   cbr_dint_field_CH_UPDATE_set ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbr_dint_field_CH_UPDATE_get ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00040010 + (N) * 4) CHANNELN_CONFIG_REG |
 | |     N[0..11]                                        |
 | +-----------------------------------------------------+
 | | bit  15:11 R/W  CHN_FIFO_TH                         |
 | | bit  7:4   R/W  CHN_ID                              |
 | | bit  1     R/W  CHN_EN                              |
 | | bit  0     R/W  CHN_RESET                           |
 | +-----------------------------------------------------+
 | 
 | void   cbr_dint_field_CHN_FIFO_TH_set       ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 cbr_dint_field_CHN_FIFO_TH_get       ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | void   cbr_dint_field_range_CHN_FIFO_TH_set ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbr_dint_field_range_CHN_FIFO_TH_get ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   cbr_dint_field_CHN_ID_set            ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 cbr_dint_field_CHN_ID_get            ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | void   cbr_dint_field_range_CHN_ID_set      ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbr_dint_field_range_CHN_ID_get      ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   cbr_dint_field_CHN_EN_set            ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 cbr_dint_field_CHN_EN_get            ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | void   cbr_dint_field_CHN_RESET_set         ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 cbr_dint_field_CHN_RESET_get         ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | +-----------------------------------------+
 | | Register 0x00040040 FIFO_UF_OVF_INT_VAL |
 | +-----------------------------------------+
 | | bit  27:16 R  FIFO_OVF_INT_V            |
 | | bit  11:0  R  FIFO_UF_INT_V             |
 | +-----------------------------------------+
 | 
 | UINT32               cbr_dint_field_FIFO_OVF_INT_V_get        ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | UINT32               cbr_dint_field_range_FIFO_OVF_INT_V_get  ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE cbr_dint_field_range_FIFO_OVF_INT_V_poll ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE cbr_dint_field_FIFO_OVF_INT_V_poll       ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               cbr_dint_field_FIFO_UF_INT_V_get         ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | UINT32               cbr_dint_field_range_FIFO_UF_INT_V_get   ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE cbr_dint_field_range_FIFO_UF_INT_V_poll  ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE cbr_dint_field_FIFO_UF_INT_V_poll        ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------+
 | | Register 0x00040044 FIFO_UF_OVF_INT_EN |
 | +----------------------------------------+
 | | bit  27:16 R/W  FIFO_OVF_INT_E         |
 | | bit  11:0  R/W  FIFO_UF_INT_E          |
 | +----------------------------------------+
 | 
 | void   cbr_dint_field_FIFO_OVF_INT_E_set       ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbr_dint_field_FIFO_OVF_INT_E_get       ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbr_dint_field_range_FIFO_OVF_INT_E_set ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbr_dint_field_range_FIFO_OVF_INT_E_get ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbr_dint_field_FIFO_UF_INT_E_set        ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbr_dint_field_FIFO_UF_INT_E_get        ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbr_dint_field_range_FIFO_UF_INT_E_set  ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbr_dint_field_range_FIFO_UF_INT_E_get  ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00040048 FIFO_UF_OVF_INT_ACTIVE |
 | +--------------------------------------------+
 | | bit  27:16 R/W  FIFO_OVF_INT_I             |
 | | bit  11:0  R/W  FIFO_UF_INT_I              |
 | +--------------------------------------------+
 | 
 | void                 cbr_dint_field_FIFO_OVF_INT_I_set_to_clear       ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               cbr_dint_field_FIFO_OVF_INT_I_get                ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void                 cbr_dint_field_range_FIFO_OVF_INT_I_set_to_clear ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               cbr_dint_field_range_FIFO_OVF_INT_I_get          ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE cbr_dint_field_range_FIFO_OVF_INT_I_poll         ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE cbr_dint_field_FIFO_OVF_INT_I_poll               ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 cbr_dint_field_FIFO_UF_INT_I_set_to_clear        ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               cbr_dint_field_FIFO_UF_INT_I_get                 ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void                 cbr_dint_field_range_FIFO_UF_INT_I_set_to_clear  ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               cbr_dint_field_range_FIFO_UF_INT_I_get           ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE cbr_dint_field_range_FIFO_UF_INT_I_poll          ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE cbr_dint_field_FIFO_UF_INT_I_poll                ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------+
 | | Register 0x0004005c STAGING_BUF_OVF_INT_VAL |
 | +---------------------------------------------+
 | | bit  11:0 R  STAG_BUF_OVF_INT_V             |
 | +---------------------------------------------+
 | 
 | UINT32               cbr_dint_field_STAG_BUF_OVF_INT_V_get        ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | UINT32               cbr_dint_field_range_STAG_BUF_OVF_INT_V_get  ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE cbr_dint_field_range_STAG_BUF_OVF_INT_V_poll ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE cbr_dint_field_STAG_BUF_OVF_INT_V_poll       ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------+
 | | Register 0x00040060 STAGING_BUF_OVF_INT_EN |
 | +--------------------------------------------+
 | | bit  11:0 R/W  STAG_BUF_OVF_INT_E          |
 | +--------------------------------------------+
 | 
 | void   cbr_dint_field_STAG_BUF_OVF_INT_E_set       ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbr_dint_field_STAG_BUF_OVF_INT_E_get       ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbr_dint_field_range_STAG_BUF_OVF_INT_E_set ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbr_dint_field_range_STAG_BUF_OVF_INT_E_get ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00040064 STAGING_BUF_OVF_INT_ACTIVE |
 | +------------------------------------------------+
 | | bit  11:0 R/W  STAG_BUF_OVF_INT_I              |
 | +------------------------------------------------+
 | 
 | void                 cbr_dint_field_STAG_BUF_OVF_INT_I_set_to_clear       ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               cbr_dint_field_STAG_BUF_OVF_INT_I_get                ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void                 cbr_dint_field_range_STAG_BUF_OVF_INT_I_set_to_clear ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               cbr_dint_field_range_STAG_BUF_OVF_INT_I_get          ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE cbr_dint_field_range_STAG_BUF_OVF_INT_I_poll         ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE cbr_dint_field_STAG_BUF_OVF_INT_I_poll               ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _CBR_DINT_IO_H
#define _CBR_DINT_IO_H

#include "cbrc_api.h"
#include "cbr_dint_regs.h"
#include "cbr_dint_io_inline.h"


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 cbr_dint_buffer_init               ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void                 cbr_dint_buffer_flush              ( cbr_dint_buffer_t *b_ptr )
 | UINT32               cbr_dint_reg_read                  ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 cbr_dint_reg_write                 ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 cbr_dint_field_set                 ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 cbr_dint_action_on_write_field_set ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 cbr_dint_burst_read                ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 cbr_dint_burst_write               ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE cbr_dint_poll                      ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 cbr_dint_reg_CHANNELN_CONFIG_REG_array_write                      ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 cbr_dint_reg_CHANNELN_CONFIG_REG_array_field_set                  ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               cbr_dint_reg_CHANNELN_CONFIG_REG_array_read                       ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | void                 cbr_dint_reg_MULTI_LANE_40G_GROUP_CONFIG_REG_array_write          ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 cbr_dint_reg_MULTI_LANE_40G_GROUP_CONFIG_REG_array_field_set      ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               cbr_dint_reg_MULTI_LANE_40G_GROUP_CONFIG_REG_array_read           ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | void                 cbr_dint_reg_CHANNEL_UPDATE_REG_write                             ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 cbr_dint_reg_CHANNEL_UPDATE_REG_field_set                         ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               cbr_dint_reg_CHANNEL_UPDATE_REG_read                              ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void                 cbr_dint_reg_FIFO_UF_OVF_INT_EN_write                             ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 cbr_dint_reg_FIFO_UF_OVF_INT_EN_field_set                         ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               cbr_dint_reg_FIFO_UF_OVF_INT_EN_read                              ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void                 cbr_dint_reg_STAGING_BUF_OVF_INT_EN_write                         ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 cbr_dint_reg_STAGING_BUF_OVF_INT_EN_field_set                     ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               cbr_dint_reg_STAGING_BUF_OVF_INT_EN_read                          ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void                 cbr_dint_reg_FIFO_UF_OVF_INT_ACTIVE_write                         ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 cbr_dint_reg_FIFO_UF_OVF_INT_ACTIVE_action_on_write_field_set     ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               cbr_dint_reg_FIFO_UF_OVF_INT_ACTIVE_read                          ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE cbr_dint_reg_FIFO_UF_OVF_INT_ACTIVE_poll                          ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 cbr_dint_reg_STAGING_BUF_OVF_INT_ACTIVE_write                     ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 cbr_dint_reg_STAGING_BUF_OVF_INT_ACTIVE_action_on_write_field_set ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               cbr_dint_reg_STAGING_BUF_OVF_INT_ACTIVE_read                      ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE cbr_dint_reg_STAGING_BUF_OVF_INT_ACTIVE_poll                      ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               cbr_dint_reg_FIFO_UF_OVF_INT_VAL_read                             ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE cbr_dint_reg_FIFO_UF_OVF_INT_VAL_poll                             ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               cbr_dint_reg_STAGING_BUF_OVF_INT_VAL_read                         ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE cbr_dint_reg_STAGING_BUF_OVF_INT_VAL_poll                         ( cbr_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _CBR_DINT_IO_H */
