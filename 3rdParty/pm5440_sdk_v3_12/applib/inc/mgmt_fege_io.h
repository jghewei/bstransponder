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
 *     and register accessor functions for the mgmt_fege block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing mgmt_fege io functions is:
 | {
 |     mgmt_fege_buffer_t b_ptr[1];
 |     mgmt_fege_buffer_init( b_ptr, h_ptr );
 |     value = mgmt_fege_field_<FIELD>_get( b_ptr, h_ptr );
 |     mgmt_fege_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     mgmt_fege_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = mgmt_fege_field_<FIELD>_get( NULL, h_ptr );
 | mgmt_fege_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +--------------------------------+
 | | Register 0x00000000 SW_RESET   |
 | +--------------------------------+
 | | bit  0 R/W  MGMT_FEGE_SW_RESET |
 | +--------------------------------+
 | 
 | void   mgmt_fege_field_MGMT_FEGE_SW_RESET_set ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mgmt_fege_field_MGMT_FEGE_SW_RESET_get ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x00000010 INT_EN         |
 | +------------------------------------+
 | | bit  5 R/W  RMVP_INVALID_TYPE_E    |
 | | bit  4 R/W  RMVP_INVALID_VLAN_ID_E |
 | | bit  3 R/W  RMVP_FIFO_OVERFLOW_E   |
 | | bit  2 R/W  TMVP_INVALID_TYPE_E    |
 | | bit  1 R/W  TMVP_INVALID_DPI_ID_E  |
 | | bit  0 R/W  TMVP_FIFO_OVERFLOW_E   |
 | +------------------------------------+
 | 
 | void   mgmt_fege_field_RMVP_INVALID_TYPE_E_set    ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mgmt_fege_field_RMVP_INVALID_TYPE_E_get    ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | void   mgmt_fege_field_RMVP_INVALID_VLAN_ID_E_set ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mgmt_fege_field_RMVP_INVALID_VLAN_ID_E_get ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | void   mgmt_fege_field_RMVP_FIFO_OVERFLOW_E_set   ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mgmt_fege_field_RMVP_FIFO_OVERFLOW_E_get   ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | void   mgmt_fege_field_TMVP_INVALID_TYPE_E_set    ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mgmt_fege_field_TMVP_INVALID_TYPE_E_get    ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | void   mgmt_fege_field_TMVP_INVALID_DPI_ID_E_set  ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mgmt_fege_field_TMVP_INVALID_DPI_ID_E_get  ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | void   mgmt_fege_field_TMVP_FIFO_OVERFLOW_E_set   ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mgmt_fege_field_TMVP_FIFO_OVERFLOW_E_get   ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x00000014 INT_STAT       |
 | +------------------------------------+
 | | bit  5 R/W  RMVP_INVALID_TYPE_I    |
 | | bit  4 R/W  RMVP_INVALID_VLAN_ID_I |
 | | bit  3 R/W  RMVP_FIFO_OVERFLOW_I   |
 | | bit  2 R/W  TMVP_INVALID_TYPE_I    |
 | | bit  1 R/W  TMVP_INVALID_DPI_ID_I  |
 | | bit  0 R/W  TMVP_FIFO_OVERFLOW_I   |
 | +------------------------------------+
 | 
 | void                 mgmt_fege_field_RMVP_INVALID_TYPE_I_set_to_clear    ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               mgmt_fege_field_RMVP_INVALID_TYPE_I_get             ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mgmt_fege_field_RMVP_INVALID_TYPE_I_poll            ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mgmt_fege_field_RMVP_INVALID_VLAN_ID_I_set_to_clear ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               mgmt_fege_field_RMVP_INVALID_VLAN_ID_I_get          ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mgmt_fege_field_RMVP_INVALID_VLAN_ID_I_poll         ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mgmt_fege_field_RMVP_FIFO_OVERFLOW_I_set_to_clear   ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               mgmt_fege_field_RMVP_FIFO_OVERFLOW_I_get            ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mgmt_fege_field_RMVP_FIFO_OVERFLOW_I_poll           ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mgmt_fege_field_TMVP_INVALID_TYPE_I_set_to_clear    ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               mgmt_fege_field_TMVP_INVALID_TYPE_I_get             ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mgmt_fege_field_TMVP_INVALID_TYPE_I_poll            ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mgmt_fege_field_TMVP_INVALID_DPI_ID_I_set_to_clear  ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               mgmt_fege_field_TMVP_INVALID_DPI_ID_I_get           ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mgmt_fege_field_TMVP_INVALID_DPI_ID_I_poll          ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mgmt_fege_field_TMVP_FIFO_OVERFLOW_I_set_to_clear   ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               mgmt_fege_field_TMVP_FIFO_OVERFLOW_I_get            ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mgmt_fege_field_TMVP_FIFO_OVERFLOW_I_poll           ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------+
 | | Register 0x00000020 TMVP_ETHERNET_TYPE |
 | +----------------------------------------+
 | | bit  15:0 R/W  TMVP_ETHERNET_TYPE      |
 | +----------------------------------------+
 | 
 | void   mgmt_fege_field_TMVP_ETHERNET_TYPE_set       ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mgmt_fege_field_TMVP_ETHERNET_TYPE_get       ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | void   mgmt_fege_field_range_TMVP_ETHERNET_TYPE_set ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mgmt_fege_field_range_TMVP_ETHERNET_TYPE_get ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00000024 RMVP_ETHERNET_TYPE |
 | +----------------------------------------+
 | | bit  15:0 R/W  RMVP_ETHERNET_TYPE      |
 | +----------------------------------------+
 | 
 | void   mgmt_fege_field_RMVP_ETHERNET_TYPE_set       ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mgmt_fege_field_RMVP_ETHERNET_TYPE_get       ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | void   mgmt_fege_field_range_RMVP_ETHERNET_TYPE_set ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mgmt_fege_field_range_RMVP_ETHERNET_TYPE_get ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x0000002c FEGE_SERDES_DATA_IF_CTRL |
 | +----------------------------------------------+
 | | bit  1 R/W  RDATA_BIT_SWAP                   |
 | | bit  0 R/W  TDATA_BIT_SWAP                   |
 | +----------------------------------------------+
 | 
 | void   mgmt_fege_field_RDATA_BIT_SWAP_set ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mgmt_fege_field_RDATA_BIT_SWAP_get ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | void   mgmt_fege_field_TDATA_BIT_SWAP_set ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mgmt_fege_field_TDATA_BIT_SWAP_get ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | 
 | +-----------------------------------------------+
 | | Register (0x00000100 + (N) * 0x4) TX_VLAN_LUT |
 | |     N[0..63]                                  |
 | +-----------------------------------------------+
 | | bit  13   R/W  TXLUT_VALID                    |
 | | bit  12   R/W  TXLUT_VLAN_ID_REPLACE          |
 | | bit  11:0 R/W  TXLUT_VLAN_ID                  |
 | +-----------------------------------------------+
 | 
 | void   mgmt_fege_field_TXLUT_VALID_set           ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mgmt_fege_field_TXLUT_VALID_get           ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N )
 | void   mgmt_fege_field_TXLUT_VLAN_ID_REPLACE_set ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mgmt_fege_field_TXLUT_VLAN_ID_REPLACE_get ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N )
 | void   mgmt_fege_field_TXLUT_VLAN_ID_set         ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mgmt_fege_field_TXLUT_VLAN_ID_get         ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N )
 | void   mgmt_fege_field_range_TXLUT_VLAN_ID_set   ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mgmt_fege_field_range_TXLUT_VLAN_ID_get   ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x00000200 + (N) * 0x4) RX_VLAN_LUT |
 | |     N[0..63]                                  |
 | +-----------------------------------------------+
 | | bit  15   R/W  RXLUT_VALID                    |
 | | bit  14   R/W  RXLUT_PRIORITY_ENABLE          |
 | | bit  13   R/W  RXLUT_FIFO_ZONE_ENABLE         |
 | | bit  12   R/W  RXLUT_FCS_ENABLE               |
 | | bit  11:0 R/W  RXLUT_VLAN_ID                  |
 | +-----------------------------------------------+
 | 
 | void   mgmt_fege_field_RXLUT_VALID_set            ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mgmt_fege_field_RXLUT_VALID_get            ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N )
 | void   mgmt_fege_field_RXLUT_PRIORITY_ENABLE_set  ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mgmt_fege_field_RXLUT_PRIORITY_ENABLE_get  ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N )
 | void   mgmt_fege_field_RXLUT_FIFO_ZONE_ENABLE_set ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mgmt_fege_field_RXLUT_FIFO_ZONE_ENABLE_get ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N )
 | void   mgmt_fege_field_RXLUT_FCS_ENABLE_set       ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mgmt_fege_field_RXLUT_FCS_ENABLE_get       ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N )
 | void   mgmt_fege_field_RXLUT_VLAN_ID_set          ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mgmt_fege_field_RXLUT_VLAN_ID_get          ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N )
 | void   mgmt_fege_field_range_RXLUT_VLAN_ID_set    ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mgmt_fege_field_range_RXLUT_VLAN_ID_get    ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _MGMT_FEGE_IO_H
#define _MGMT_FEGE_IO_H

#include "mgmt_fege_loc.h"
#include "mgmt_fege_regs.h"
#include "mgmt_fege_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mgmt_fege_buffer_init               ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | void                 mgmt_fege_buffer_flush              ( mgmt_fege_buffer_t *b_ptr )
 | UINT32               mgmt_fege_reg_read                  ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 mgmt_fege_reg_write                 ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 mgmt_fege_field_set                 ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 mgmt_fege_action_on_write_field_set ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mgmt_fege_burst_read                ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 mgmt_fege_burst_write               ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE mgmt_fege_poll                      ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mgmt_fege_reg_SW_RESET_write                     ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | void                 mgmt_fege_reg_SW_RESET_field_set                 ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mgmt_fege_reg_SW_RESET_read                      ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | 
 | void                 mgmt_fege_reg_TMVP_ETHERNET_TYPE_write           ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | void                 mgmt_fege_reg_TMVP_ETHERNET_TYPE_field_set       ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mgmt_fege_reg_TMVP_ETHERNET_TYPE_read            ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | 
 | void                 mgmt_fege_reg_RMVP_ETHERNET_TYPE_write           ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | void                 mgmt_fege_reg_RMVP_ETHERNET_TYPE_field_set       ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mgmt_fege_reg_RMVP_ETHERNET_TYPE_read            ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | 
 | void                 mgmt_fege_reg_FEGE_SERDES_DATA_IF_CTRL_write     ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | void                 mgmt_fege_reg_FEGE_SERDES_DATA_IF_CTRL_field_set ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mgmt_fege_reg_FEGE_SERDES_DATA_IF_CTRL_read      ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | 
 | void                 mgmt_fege_reg_RX_VLAN_LUT_array_write            ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mgmt_fege_reg_RX_VLAN_LUT_array_field_set        ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mgmt_fege_reg_RX_VLAN_LUT_array_read             ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mgmt_fege_reg_INT_EN_write                       ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | void                 mgmt_fege_reg_INT_EN_field_set                   ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mgmt_fege_reg_INT_EN_read                        ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | 
 | void                 mgmt_fege_reg_INT_STAT_write                     ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 value )
 | void                 mgmt_fege_reg_INT_STAT_action_on_write_field_set ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mgmt_fege_reg_INT_STAT_read                      ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mgmt_fege_reg_INT_STAT_poll                      ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mgmt_fege_reg_TX_VLAN_LUT_array_write            ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mgmt_fege_reg_TX_VLAN_LUT_array_field_set        ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mgmt_fege_reg_TX_VLAN_LUT_array_read             ( mgmt_fege_buffer_t *b_ptr, mgmt_fege_handle_t *h_ptr, UINT32  N )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _MGMT_FEGE_IO_H */
