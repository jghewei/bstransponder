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
 *     and register accessor functions for the cbrc_sfis_tx block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing cbrc_sfis_tx io functions is:
 | {
 |     cbrc_sfis_tx_buffer_t b_ptr[1];
 |     cbrc_sfis_tx_buffer_init( b_ptr, h_ptr, A );
 |     value = cbrc_sfis_tx_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     cbrc_sfis_tx_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     cbrc_sfis_tx_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = cbrc_sfis_tx_field_<FIELD>_get( NULL, h_ptr, A );
 | cbrc_sfis_tx_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------+
 | | Register 0x00000180 TX_CFG |
 | +----------------------------+
 | | bit  5   R/W  TX_ODD_INV   |
 | | bit  4   R/W  TX_SW_RST    |
 | | bit  3:0 R/W  TX_NUM_LANES |
 | +----------------------------+
 | 
 | void   cbrc_sfis_tx_field_TX_ODD_INV_set         ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sfis_tx_field_TX_ODD_INV_get         ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sfis_tx_field_TX_NUM_LANES_set       ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sfis_tx_field_TX_NUM_LANES_get       ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sfis_tx_field_range_TX_NUM_LANES_set ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_sfis_tx_field_range_TX_NUM_LANES_get ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_sfis_tx_field_TX_SW_RST_set          ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sfis_tx_field_TX_SW_RST_get          ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------+
 | | Register 0x00000184 TX_INT |
 | +----------------------------+
 | | bit  1 R/W  TXOVR_I        |
 | | bit  0 R/W  TXUDR_I        |
 | +----------------------------+
 | 
 | void                 cbrc_sfis_tx_field_TXOVR_I_set_to_clear ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               cbrc_sfis_tx_field_TXOVR_I_get          ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE cbrc_sfis_tx_field_TXOVR_I_poll         ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 cbrc_sfis_tx_field_TXUDR_I_set_to_clear ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               cbrc_sfis_tx_field_TXUDR_I_get          ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE cbrc_sfis_tx_field_TXUDR_I_poll         ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------+
 | | Register 0x00000188 TX_INT_EN |
 | +-------------------------------+
 | | bit  1 R/W  TXOVR_E           |
 | | bit  0 R/W  TXUDR_E           |
 | +-------------------------------+
 | 
 | void   cbrc_sfis_tx_field_TXOVR_E_set ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sfis_tx_field_TXOVR_E_get ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sfis_tx_field_TXUDR_E_set ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sfis_tx_field_TXUDR_E_get ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------+
 | | Register 0x0000018c TX_BUFFER_CFG |
 | +-----------------------------------+
 | | bit  7 R/W  TX_FIFO_CENTER        |
 | | bit  6 R/W  TX_FIFO_AUTO_CENTER   |
 | +-----------------------------------+
 | 
 | void   cbrc_sfis_tx_field_TX_FIFO_AUTO_CENTER_set ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sfis_tx_field_TX_FIFO_AUTO_CENTER_get ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sfis_tx_field_TX_FIFO_CENTER_set      ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sfis_tx_field_TX_FIFO_CENTER_get      ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _CBRC_SFIS_TX_IO_H
#define _CBRC_SFIS_TX_IO_H

#include "cbrc_api.h"
#include "cbrc_sfis_tx_regs.h"
#include "cbrc_sfis_tx_io_inline.h"


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 cbrc_sfis_tx_buffer_init               ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void                 cbrc_sfis_tx_buffer_flush              ( cbrc_sfis_tx_buffer_t *b_ptr )
 | UINT32               cbrc_sfis_tx_reg_read                  ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 cbrc_sfis_tx_reg_write                 ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 cbrc_sfis_tx_field_set                 ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 cbrc_sfis_tx_action_on_write_field_set ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 cbrc_sfis_tx_burst_read                ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 cbrc_sfis_tx_burst_write               ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE cbrc_sfis_tx_poll                      ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 cbrc_sfis_tx_reg_TX_CFG_write                     ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 cbrc_sfis_tx_reg_TX_CFG_field_set                 ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               cbrc_sfis_tx_reg_TX_CFG_read                      ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 cbrc_sfis_tx_reg_TX_BUFFER_CFG_write              ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 cbrc_sfis_tx_reg_TX_BUFFER_CFG_field_set          ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               cbrc_sfis_tx_reg_TX_BUFFER_CFG_read               ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 cbrc_sfis_tx_reg_TX_INT_EN_write                  ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 cbrc_sfis_tx_reg_TX_INT_EN_field_set              ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               cbrc_sfis_tx_reg_TX_INT_EN_read                   ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 cbrc_sfis_tx_reg_TX_INT_write                     ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 cbrc_sfis_tx_reg_TX_INT_action_on_write_field_set ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               cbrc_sfis_tx_reg_TX_INT_read                      ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE cbrc_sfis_tx_reg_TX_INT_poll                      ( cbrc_sfis_tx_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _CBRC_SFIS_TX_IO_H */
