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
 *     and register accessor functions for the fc1200_pcs_rx block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing fc1200_pcs_rx io functions is:
 | {
 |     fc1200_pcs_rx_buffer_t b_ptr[1];
 |     fc1200_pcs_rx_buffer_init( b_ptr, h_ptr, A );
 |     value = fc1200_pcs_rx_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     fc1200_pcs_rx_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     fc1200_pcs_rx_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = fc1200_pcs_rx_field_<FIELD>_get( NULL, h_ptr, A );
 | fc1200_pcs_rx_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------+
 | | Register 0x0002f000 CFG_REG      |
 | +----------------------------------+
 | | bit  4 R/W  RESTORE_INVALID_IDLE |
 | | bit  2 R/W  PRBS31_ENA           |
 | | bit  1 R/W  DESCRAMBLER_ENA      |
 | | bit  0 R/W  SW_RESET             |
 | +----------------------------------+
 | 
 | void   fc1200_pcs_rx_field_RESTORE_INVALID_IDLE_set ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pcs_rx_field_RESTORE_INVALID_IDLE_get ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pcs_rx_field_PRBS31_ENA_set           ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pcs_rx_field_PRBS31_ENA_get           ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pcs_rx_field_DESCRAMBLER_ENA_set      ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pcs_rx_field_DESCRAMBLER_ENA_get      ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pcs_rx_field_SW_RESET_set             ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pcs_rx_field_SW_RESET_get             ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------+
 | | Register 0x0002f004 PMON_CNT_STAT |
 | +-----------------------------------+
 | | bit  8 R    TIP                   |
 | | bit  0 R/W  PMON_UPDATE           |
 | +-----------------------------------+
 | 
 | void                 fc1200_pcs_rx_field_PMON_UPDATE_set ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc1200_pcs_rx_field_PMON_UPDATE_get ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32               fc1200_pcs_rx_field_TIP_get         ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_TIP_poll        ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------+
 | | Register 0x0002f008 HIGH_BER_CFG_REG |
 | +--------------------------------------+
 | | bit  15:0 R/W  COUNT_125US           |
 | +--------------------------------------+
 | 
 | void   fc1200_pcs_rx_field_COUNT_125US_set       ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pcs_rx_field_COUNT_125US_get       ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pcs_rx_field_range_COUNT_125US_set ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc1200_pcs_rx_field_range_COUNT_125US_get ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------+
 | | Register 0x0002f014 STAT_REG |
 | +------------------------------+
 | | bit  1 R  REMOTE_FAULT       |
 | | bit  0 R  LOCAL_FAULT        |
 | +------------------------------+
 | 
 | UINT32 fc1200_pcs_rx_field_REMOTE_FAULT_get ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 fc1200_pcs_rx_field_LOCAL_FAULT_get  ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------+
 | | Register 0x0002f018 INT_EN_REG |
 | +--------------------------------+
 | | bit  5 R/W  RF_E               |
 | | bit  4 R/W  LF_E               |
 | | bit  2 R/W  HIGH_BER_E         |
 | | bit  1 R/W  LOSS_SYNC_E        |
 | | bit  0 R/W  LOS_E              |
 | +--------------------------------+
 | 
 | void   fc1200_pcs_rx_field_RF_E_set        ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pcs_rx_field_RF_E_get        ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pcs_rx_field_LF_E_set        ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pcs_rx_field_LF_E_get        ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pcs_rx_field_HIGH_BER_E_set  ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pcs_rx_field_HIGH_BER_E_get  ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pcs_rx_field_LOSS_SYNC_E_set ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pcs_rx_field_LOSS_SYNC_E_get ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pcs_rx_field_LOS_E_set       ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pcs_rx_field_LOS_E_get       ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------------+
 | | Register 0x0002f01c INT_INDICATION_REG |
 | +----------------------------------------+
 | | bit  5 R/W  RF_I                       |
 | | bit  4 R/W  LF_I                       |
 | | bit  2 R/W  HIGH_BER_I                 |
 | | bit  1 R/W  LOSS_SYNC_I                |
 | | bit  0 R/W  LOS_I                      |
 | +----------------------------------------+
 | 
 | void                 fc1200_pcs_rx_field_RF_I_set_to_clear        ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc1200_pcs_rx_field_RF_I_get                 ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_RF_I_poll                ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc1200_pcs_rx_field_LF_I_set_to_clear        ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc1200_pcs_rx_field_LF_I_get                 ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_LF_I_poll                ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc1200_pcs_rx_field_HIGH_BER_I_set_to_clear  ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc1200_pcs_rx_field_HIGH_BER_I_get           ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_HIGH_BER_I_poll          ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc1200_pcs_rx_field_LOSS_SYNC_I_set_to_clear ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc1200_pcs_rx_field_LOSS_SYNC_I_get          ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_LOSS_SYNC_I_poll         ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc1200_pcs_rx_field_LOS_I_set_to_clear       ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc1200_pcs_rx_field_LOS_I_get                ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_LOS_I_poll               ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------+
 | | Register 0x0002f020 INT_STAT_REG |
 | +----------------------------------+
 | | bit  5 R  RF_V                   |
 | | bit  4 R  LF_V                   |
 | | bit  2 R  HIGH_BER_V             |
 | | bit  1 R  LOSS_SYNC_V            |
 | | bit  0 R  LOS_V                  |
 | +----------------------------------+
 | 
 | UINT32               fc1200_pcs_rx_field_RF_V_get         ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_RF_V_poll        ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               fc1200_pcs_rx_field_LF_V_get         ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_LF_V_poll        ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               fc1200_pcs_rx_field_HIGH_BER_V_get   ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_HIGH_BER_V_poll  ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               fc1200_pcs_rx_field_LOSS_SYNC_V_get  ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_LOSS_SYNC_V_poll ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               fc1200_pcs_rx_field_LOS_V_get        ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_LOS_V_poll       ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------+
 | | Register 0x0002f024 ERR_CNTS   |
 | +--------------------------------+
 | | bit  23:16 R  HIGH_BER_ERR_CNT |
 | | bit  15:0  R  PRBS31_ERR_CNT   |
 | +--------------------------------+
 | 
 | UINT32 fc1200_pcs_rx_field_HIGH_BER_ERR_CNT_get       ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 fc1200_pcs_rx_field_range_HIGH_BER_ERR_CNT_get ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 fc1200_pcs_rx_field_PRBS31_ERR_CNT_get         ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 fc1200_pcs_rx_field_range_PRBS31_ERR_CNT_get   ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x0002f028 BLOCK_ERR_CNTS |
 | +------------------------------------+
 | | bit  31:16 R  BLK_TYPE_ERR_CNT     |
 | | bit  15:0  R  INVLD_BLK_CNT        |
 | +------------------------------------+
 | 
 | UINT32 fc1200_pcs_rx_field_BLK_TYPE_ERR_CNT_get       ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 fc1200_pcs_rx_field_range_BLK_TYPE_ERR_CNT_get ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 fc1200_pcs_rx_field_INVLD_BLK_CNT_get          ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 fc1200_pcs_rx_field_range_INVLD_BLK_CNT_get    ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x0002f02c RXD_DATA_BYTE_CNT_LSB |
 | +-------------------------------------------+
 | | bit  31:0 R  DATA_BYTE_CNT                |
 | +-------------------------------------------+
 | 
 | void   fc1200_pcs_rx_lfield_DATA_BYTE_CNT_get       ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value[2] )
 | UINT32 fc1200_pcs_rx_lfield_range_DATA_BYTE_CNT_get ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x0002f034 RXD_ORDERED_SET_CNT |
 | +-----------------------------------------+
 | | bit  31:0 R  ORDERED_SET_CNT            |
 | +-----------------------------------------+
 | 
 | UINT32 fc1200_pcs_rx_field_ORDERED_SET_CNT_get       ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 fc1200_pcs_rx_field_range_ORDERED_SET_CNT_get ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _FC1200_PCS_RX_IO_H
#define _FC1200_PCS_RX_IO_H

#include "cbrc_api.h"
#include "fc1200_pcs_rx_regs.h"
#include "fc1200_pcs_rx_io_inline.h"


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 fc1200_pcs_rx_buffer_init               ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void                 fc1200_pcs_rx_buffer_flush              ( fc1200_pcs_rx_buffer_t *b_ptr )
 | UINT32               fc1200_pcs_rx_reg_read                  ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 fc1200_pcs_rx_reg_write                 ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 fc1200_pcs_rx_field_set                 ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 fc1200_pcs_rx_action_on_write_field_set ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 fc1200_pcs_rx_burst_read                ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 fc1200_pcs_rx_burst_write               ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_rx_poll                      ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 fc1200_pcs_rx_reg_CFG_REG_write                                ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc1200_pcs_rx_reg_CFG_REG_field_set                            ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc1200_pcs_rx_reg_CFG_REG_read                                 ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc1200_pcs_rx_reg_HIGH_BER_CFG_REG_write                       ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc1200_pcs_rx_reg_HIGH_BER_CFG_REG_field_set                   ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc1200_pcs_rx_reg_HIGH_BER_CFG_REG_read                        ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc1200_pcs_rx_reg_PMON_CNT_STAT_write                          ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc1200_pcs_rx_reg_PMON_CNT_STAT_field_set                      ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc1200_pcs_rx_reg_PMON_CNT_STAT_read                           ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_rx_reg_PMON_CNT_STAT_poll                           ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 fc1200_pcs_rx_reg_INT_EN_REG_write                             ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc1200_pcs_rx_reg_INT_EN_REG_field_set                         ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc1200_pcs_rx_reg_INT_EN_REG_read                              ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc1200_pcs_rx_reg_INT_INDICATION_REG_write                     ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc1200_pcs_rx_reg_INT_INDICATION_REG_action_on_write_field_set ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc1200_pcs_rx_reg_INT_INDICATION_REG_read                      ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_rx_reg_INT_INDICATION_REG_poll                      ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               fc1200_pcs_rx_reg_INT_STAT_REG_read                            ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_rx_reg_INT_STAT_REG_poll                            ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               fc1200_pcs_rx_reg_STAT_REG_read                                ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc1200_pcs_rx_reg_ERR_CNTS_read                                ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc1200_pcs_rx_reg_BLOCK_ERR_CNTS_read                          ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc1200_pcs_rx_reg_RXD_DATA_BYTE_CNT_LSB_read                   ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc1200_pcs_rx_reg_RXD_DATA_BYTE_CNT_MSB_read                   ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc1200_pcs_rx_reg_RXD_ORDERED_SET_CNT_read                     ( fc1200_pcs_rx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _FC1200_PCS_RX_IO_H */
