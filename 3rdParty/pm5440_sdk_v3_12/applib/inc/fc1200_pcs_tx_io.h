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
 *     and register accessor functions for the fc1200_pcs_tx block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing fc1200_pcs_tx io functions is:
 | {
 |     fc1200_pcs_tx_buffer_t b_ptr[1];
 |     fc1200_pcs_tx_buffer_init( b_ptr, h_ptr, A );
 |     value = fc1200_pcs_tx_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     fc1200_pcs_tx_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     fc1200_pcs_tx_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = fc1200_pcs_tx_field_<FIELD>_get( NULL, h_ptr, A );
 | fc1200_pcs_tx_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +------------------------------+
 | | Register 0x00050000 CFG      |
 | +------------------------------+
 | | bit  8 R/W  CORRUPT_BLK_MODE |
 | | bit  5 R/W  LOS_LF_ENA       |
 | | bit  4 R/W  EXT_LF_ENA       |
 | | bit  3 R/W  FORCE_LF         |
 | | bit  1 R/W  PRBS31_ENA       |
 | | bit  0 R/W  SW_RESET         |
 | +------------------------------+
 | 
 | void   fc1200_pcs_tx_field_CORRUPT_BLK_MODE_set ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pcs_tx_field_CORRUPT_BLK_MODE_get ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pcs_tx_field_LOS_LF_ENA_set       ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pcs_tx_field_LOS_LF_ENA_get       ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pcs_tx_field_EXT_LF_ENA_set       ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pcs_tx_field_EXT_LF_ENA_get       ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pcs_tx_field_FORCE_LF_set         ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pcs_tx_field_FORCE_LF_get         ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pcs_tx_field_PRBS31_ENA_set       ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pcs_tx_field_PRBS31_ENA_get       ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pcs_tx_field_SW_RESET_set         ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pcs_tx_field_SW_RESET_get         ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------+
 | | Register 0x00050004 PMON_CNT_STAT |
 | +-----------------------------------+
 | | bit  8 R    TIP                   |
 | | bit  0 R/W  PMON_UPDATE           |
 | +-----------------------------------+
 | 
 | void                 fc1200_pcs_tx_field_PMON_UPDATE_set ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc1200_pcs_tx_field_PMON_UPDATE_get ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32               fc1200_pcs_tx_field_TIP_get         ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_tx_field_TIP_poll        ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------------+
 | | Register 0x00050008 LOCAL_FAULT_ORDERED_SET_CFG |
 | +-------------------------------------------------+
 | | bit  27:24 R/W  LF_O_CODE                       |
 | | bit  23:0  R/W  LF_ORDERED_SET                  |
 | +-------------------------------------------------+
 | 
 | void   fc1200_pcs_tx_field_LF_O_CODE_set            ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pcs_tx_field_LF_O_CODE_get            ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pcs_tx_field_range_LF_O_CODE_set      ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc1200_pcs_tx_field_range_LF_O_CODE_get      ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   fc1200_pcs_tx_field_LF_ORDERED_SET_set       ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pcs_tx_field_LF_ORDERED_SET_get       ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pcs_tx_field_range_LF_ORDERED_SET_set ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc1200_pcs_tx_field_range_LF_ORDERED_SET_get ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0005000c TX_DATA_BYTE_CNT_LSB |
 | +------------------------------------------+
 | | bit  31:0 R  DATA_BYTE_CNT               |
 | +------------------------------------------+
 | 
 | void   fc1200_pcs_tx_lfield_DATA_BYTE_CNT_get       ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value[2] )
 | UINT32 fc1200_pcs_tx_lfield_range_DATA_BYTE_CNT_get ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00050014 TX_ORDERED_SET_CNT |
 | +----------------------------------------+
 | | bit  31:0 R  ORDERED_SET_CNT           |
 | +----------------------------------------+
 | 
 | UINT32 fc1200_pcs_tx_field_ORDERED_SET_CNT_get       ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 fc1200_pcs_tx_field_range_ORDERED_SET_CNT_get ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00050018 ERR_CTRL_BLOCK_CNT |
 | +----------------------------------------+
 | | bit  31:0 R  ERROR_BLK_CNT             |
 | +----------------------------------------+
 | 
 | UINT32 fc1200_pcs_tx_field_ERROR_BLK_CNT_get       ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 fc1200_pcs_tx_field_range_ERROR_BLK_CNT_get ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x0005001c LOCAL_FAULT_ORDERED_SET_CNT |
 | +-------------------------------------------------+
 | | bit  31:0 R  LF_OS_CNT                          |
 | +-------------------------------------------------+
 | 
 | UINT32 fc1200_pcs_tx_field_LF_OS_CNT_get       ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 fc1200_pcs_tx_field_range_LF_OS_CNT_get ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------+
 | | Register 0x00050020 INT_EN |
 | +----------------------------+
 | | bit  1 R/W  BUF_UNDERRUN_E |
 | | bit  0 R/W  LOCAL_FAULT_E  |
 | +----------------------------+
 | 
 | void   fc1200_pcs_tx_field_BUF_UNDERRUN_E_set ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pcs_tx_field_BUF_UNDERRUN_E_get ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pcs_tx_field_LOCAL_FAULT_E_set  ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pcs_tx_field_LOCAL_FAULT_E_get  ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------+
 | | Register 0x00050024 INT_INDICATION |
 | +------------------------------------+
 | | bit  1 R/W  BUF_UNDERRUN_I         |
 | | bit  0 R/W  LOCAL_FAULT_I          |
 | +------------------------------------+
 | 
 | void                 fc1200_pcs_tx_field_BUF_UNDERRUN_I_set_to_clear ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc1200_pcs_tx_field_BUF_UNDERRUN_I_get          ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_tx_field_BUF_UNDERRUN_I_poll         ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc1200_pcs_tx_field_LOCAL_FAULT_I_set_to_clear  ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc1200_pcs_tx_field_LOCAL_FAULT_I_get           ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_tx_field_LOCAL_FAULT_I_poll          ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------+
 | | Register 0x00050028 INT_STAT |
 | +------------------------------+
 | | bit  1 R  BUF_UNDERRUN_V     |
 | | bit  0 R  LOCAL_FAULT_V      |
 | +------------------------------+
 | 
 | UINT32               fc1200_pcs_tx_field_BUF_UNDERRUN_V_get  ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_tx_field_BUF_UNDERRUN_V_poll ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               fc1200_pcs_tx_field_LOCAL_FAULT_V_get   ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_tx_field_LOCAL_FAULT_V_poll  ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _FC1200_PCS_TX_IO_H
#define _FC1200_PCS_TX_IO_H

#include "cbrc_api.h"
#include "fc1200_pcs_tx_regs.h"
#include "fc1200_pcs_tx_io_inline.h"


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 fc1200_pcs_tx_buffer_init               ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void                 fc1200_pcs_tx_buffer_flush              ( fc1200_pcs_tx_buffer_t *b_ptr )
 | UINT32               fc1200_pcs_tx_reg_read                  ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 fc1200_pcs_tx_reg_write                 ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 fc1200_pcs_tx_field_set                 ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 fc1200_pcs_tx_action_on_write_field_set ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 fc1200_pcs_tx_burst_read                ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 fc1200_pcs_tx_burst_write               ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_tx_poll                      ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 fc1200_pcs_tx_reg_CFG_write                                ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc1200_pcs_tx_reg_CFG_field_set                            ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc1200_pcs_tx_reg_CFG_read                                 ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CFG_write        ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CFG_field_set    ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CFG_read         ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc1200_pcs_tx_reg_PMON_CNT_STAT_write                      ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc1200_pcs_tx_reg_PMON_CNT_STAT_field_set                  ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc1200_pcs_tx_reg_PMON_CNT_STAT_read                       ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_tx_reg_PMON_CNT_STAT_poll                       ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 fc1200_pcs_tx_reg_INT_EN_write                             ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc1200_pcs_tx_reg_INT_EN_field_set                         ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc1200_pcs_tx_reg_INT_EN_read                              ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc1200_pcs_tx_reg_INT_INDICATION_write                     ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc1200_pcs_tx_reg_INT_INDICATION_action_on_write_field_set ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc1200_pcs_tx_reg_INT_INDICATION_read                      ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_tx_reg_INT_INDICATION_poll                      ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               fc1200_pcs_tx_reg_INT_STAT_read                            ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pcs_tx_reg_INT_STAT_poll                            ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               fc1200_pcs_tx_reg_TX_DATA_BYTE_CNT_LSB_read                ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc1200_pcs_tx_reg_TX_DATA_BYTE_CNT_MSB_read                ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc1200_pcs_tx_reg_TX_ORDERED_SET_CNT_read                  ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc1200_pcs_tx_reg_ERR_CTRL_BLOCK_CNT_read                  ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CNT_read         ( fc1200_pcs_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _FC1200_PCS_TX_IO_H */
