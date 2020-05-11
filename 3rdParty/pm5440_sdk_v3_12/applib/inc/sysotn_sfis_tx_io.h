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
 *     and register accessor functions for the sysotn_sfis_tx block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing sysotn_sfis_tx io functions is:
 | {
 |     sysotn_sfis_tx_buffer_t b_ptr[1];
 |     sysotn_sfis_tx_buffer_init( b_ptr, h_ptr );
 |     value = sysotn_sfis_tx_field_<FIELD>_get( b_ptr, h_ptr );
 |     sysotn_sfis_tx_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     sysotn_sfis_tx_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = sysotn_sfis_tx_field_<FIELD>_get( NULL, h_ptr );
 | sysotn_sfis_tx_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------+
 | | Register 0x00004080 TX_CFG |
 | +----------------------------+
 | | bit  5   R/W  TX_ODD_INV   |
 | | bit  4   R/W  TX_SW_RST    |
 | | bit  3:0 R/W  TX_NUM_LANES |
 | +----------------------------+
 | 
 | void   sysotn_sfis_tx_field_TX_ODD_INV_set         ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_tx_field_TX_ODD_INV_get         ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_tx_field_TX_NUM_LANES_set       ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_tx_field_TX_NUM_LANES_get       ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_tx_field_range_TX_NUM_LANES_set ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_sfis_tx_field_range_TX_NUM_LANES_get ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_sfis_tx_field_TX_SW_RST_set          ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_tx_field_TX_SW_RST_get          ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +----------------------------+
 | | Register 0x00004084 TX_INT |
 | +----------------------------+
 | | bit  1 R/W  TXOVR_I        |
 | | bit  0 R/W  TXUDR_I        |
 | +----------------------------+
 | 
 | void                 sysotn_sfis_tx_field_TXOVR_I_set_to_clear ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_tx_field_TXOVR_I_get          ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_tx_field_TXOVR_I_poll         ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_sfis_tx_field_TXUDR_I_set_to_clear ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_tx_field_TXUDR_I_get          ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_tx_field_TXUDR_I_poll         ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------+
 | | Register 0x00004088 TX_INT_EN |
 | +-------------------------------+
 | | bit  1 R/W  TXOVR_E           |
 | | bit  0 R/W  TXUDR_E           |
 | +-------------------------------+
 | 
 | void   sysotn_sfis_tx_field_TXOVR_E_set ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_tx_field_TXOVR_E_get ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_tx_field_TXUDR_E_set ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_tx_field_TXUDR_E_get ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +-----------------------------------+
 | | Register 0x0000408c TX_BUFFER_CFG |
 | +-----------------------------------+
 | | bit  7 R/W  TX_FIFO_CENTER        |
 | | bit  6 R/W  TX_FIFO_AUTO_CENTER   |
 | +-----------------------------------+
 | 
 | void   sysotn_sfis_tx_field_TX_FIFO_AUTO_CENTER_set ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_tx_field_TX_FIFO_AUTO_CENTER_get ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_tx_field_TX_FIFO_CENTER_set      ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_tx_field_TX_FIFO_CENTER_get      ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _SYSOTN_SFIS_TX_IO_H
#define _SYSOTN_SFIS_TX_IO_H

#include "lineotn_loc.h"
#include "sysotn_sfis_tx_regs.h"
#include "sysotn_sfis_tx_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sysotn_sfis_tx_buffer_init               ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void                 sysotn_sfis_tx_buffer_flush              ( sysotn_sfis_tx_buffer_t *b_ptr )
 | UINT32               sysotn_sfis_tx_reg_read                  ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 sysotn_sfis_tx_reg_write                 ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 sysotn_sfis_tx_field_set                 ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 sysotn_sfis_tx_action_on_write_field_set ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 sysotn_sfis_tx_burst_read                ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 sysotn_sfis_tx_burst_write               ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_tx_poll                      ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sysotn_sfis_tx_reg_TX_CFG_write                     ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_sfis_tx_reg_TX_CFG_field_set                 ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_sfis_tx_reg_TX_CFG_read                      ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_sfis_tx_reg_TX_BUFFER_CFG_write              ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_sfis_tx_reg_TX_BUFFER_CFG_field_set          ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_sfis_tx_reg_TX_BUFFER_CFG_read               ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_sfis_tx_reg_TX_INT_EN_write                  ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_sfis_tx_reg_TX_INT_EN_field_set              ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_sfis_tx_reg_TX_INT_EN_read                   ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_sfis_tx_reg_TX_INT_write                     ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_sfis_tx_reg_TX_INT_action_on_write_field_set ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_sfis_tx_reg_TX_INT_read                      ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_tx_reg_TX_INT_poll                      ( sysotn_sfis_tx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _SYSOTN_SFIS_TX_IO_H */
