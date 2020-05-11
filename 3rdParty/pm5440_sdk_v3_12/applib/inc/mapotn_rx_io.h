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
 *     and register accessor functions for the mapotn_rx block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing mapotn_rx io functions is:
 | {
 |     mapotn_rx_buffer_t b_ptr[1];
 |     mapotn_rx_buffer_init( b_ptr, h_ptr );
 |     value = mapotn_rx_field_<FIELD>_get( b_ptr, h_ptr );
 |     mapotn_rx_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     mapotn_rx_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = mapotn_rx_field_<FIELD>_get( NULL, h_ptr );
 | mapotn_rx_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +--------------------------------+
 | | Register 0x00000000 SOFT_RST   |
 | +--------------------------------+
 | | bit  3 R/W  MPMA_SOFT_RST      |
 | | bit  2 R/W  RGMP96_SOFT_RST    |
 | | bit  1 R/W  RGFPF96_SOFT_RST   |
 | | bit  0 R/W  MAPOTN_RX_SOFT_RST |
 | +--------------------------------+
 | 
 | void   mapotn_rx_field_MPMA_SOFT_RST_set      ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rx_field_MPMA_SOFT_RST_get      ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rx_field_RGMP96_SOFT_RST_set    ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rx_field_RGMP96_SOFT_RST_get    ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rx_field_RGFPF96_SOFT_RST_set   ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rx_field_RGFPF96_SOFT_RST_get   ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rx_field_MAPOTN_RX_SOFT_RST_set ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rx_field_MAPOTN_RX_SOFT_RST_get ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +---------------------------------+
 | | Register 0x00000004 LOWPWR_MODE |
 | +---------------------------------+
 | | bit  2 R/W  MPMA_LOWPWR         |
 | | bit  1 R/W  RGMP96_LOWPWR       |
 | | bit  0 R/W  RGFPF96_LOWPWR      |
 | +---------------------------------+
 | 
 | void   mapotn_rx_field_MPMA_LOWPWR_set    ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rx_field_MPMA_LOWPWR_get    ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rx_field_RGMP96_LOWPWR_set  ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rx_field_RGMP96_LOWPWR_get  ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rx_field_RGFPF96_LOWPWR_set ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rx_field_RGFPF96_LOWPWR_get ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x00000008 DCI_DPI_ASYNC_FIFO_RSTB |
 | +---------------------------------------------+
 | | bit  0 R/W  FIFO_RSTB                       |
 | +---------------------------------------------+
 | 
 | void   mapotn_rx_field_FIFO_RSTB_set ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rx_field_FIFO_RSTB_get ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +---------------------------------------------------------+
 | | Register 0x0000000c DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE |
 | +---------------------------------------------------------+
 | | bit  0 R/W  FIFO_FULL_E                                 |
 | +---------------------------------------------------------+
 | 
 | void   mapotn_rx_field_FIFO_FULL_E_set ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rx_field_FIFO_FULL_E_get ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +--------------------------------------------------+
 | | Register 0x00000010 DCI_DPI_ASYNC_FIFO_FULL_INTR |
 | +--------------------------------------------------+
 | | bit  0 R/W  FIFO_FULL_I                          |
 | +--------------------------------------------------+
 | 
 | void                 mapotn_rx_field_FIFO_FULL_I_set_to_clear ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32               mapotn_rx_field_FIFO_FULL_I_get          ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_rx_field_FIFO_FULL_I_poll         ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x00000018 RAM_LOWPWR_MODE |
 | +-------------------------------------+
 | | bit  3 R/W  TOP_RAM_LOWPWR          |
 | | bit  2 R/W  MPMA_RAM_LOWPWR         |
 | | bit  1 R/W  RGMP96_RAM_LOWPWR       |
 | | bit  0 R/W  RGFPF96_RAM_LOWPWR      |
 | +-------------------------------------+
 | 
 | void   mapotn_rx_field_TOP_RAM_LOWPWR_set     ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rx_field_TOP_RAM_LOWPWR_get     ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rx_field_MPMA_RAM_LOWPWR_set    ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rx_field_MPMA_RAM_LOWPWR_get    ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rx_field_RGMP96_RAM_LOWPWR_set  ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rx_field_RGMP96_RAM_LOWPWR_get  ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rx_field_RGFPF96_RAM_LOWPWR_set ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rx_field_RGFPF96_RAM_LOWPWR_get ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +-----------------------------------------------------------+
 | | Register (0x0000001c + (N) * 0x4) DATA_PATH_CTRL_REGISTER |
 | |     N[0..2]                                               |
 | +-----------------------------------------------------------+
 | | bit  31:0 R/W  DCI2DPI_EN                                 |
 | +-----------------------------------------------------------+
 | 
 | void   mapotn_rx_lfield_DCI2DPI_EN_set       ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rx_lfield_DCI2DPI_EN_get       ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rx_lfield_range_DCI2DPI_EN_set ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rx_lfield_range_DCI2DPI_EN_get ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x00000300 + (N) * 0x4) INTR_EN_REG |
 | |     N[0..3]                                   |
 | +-----------------------------------------------+
 | | bit  8 R/W  MPMO_INTR_EN                      |
 | | bit  7 R/W  ODUKSC_INTR_EN                    |
 | | bit  6 R/W  TGMP96_INTR_EN                    |
 | | bit  5 R/W  TGFPF96_INTR_EN                   |
 | | bit  4 R/W  MAPOTN_TX_TOP_INTR_EN             |
 | | bit  3 R/W  MPMA_INTR_EN                      |
 | | bit  2 R/W  RGMP96_INTR_EN                    |
 | | bit  1 R/W  RGFPF96_INTR_EN                   |
 | | bit  0 R/W  MAPOTN_RX_TOP_INTR_EN             |
 | +-----------------------------------------------+
 | 
 | void   mapotn_rx_field_MPMO_INTR_EN_set          ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rx_field_MPMO_INTR_EN_get          ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rx_field_ODUKSC_INTR_EN_set        ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rx_field_ODUKSC_INTR_EN_get        ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rx_field_TGMP96_INTR_EN_set        ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rx_field_TGMP96_INTR_EN_get        ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rx_field_TGFPF96_INTR_EN_set       ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rx_field_TGFPF96_INTR_EN_get       ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rx_field_MAPOTN_TX_TOP_INTR_EN_set ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rx_field_MAPOTN_TX_TOP_INTR_EN_get ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rx_field_MPMA_INTR_EN_set          ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rx_field_MPMA_INTR_EN_get          ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rx_field_RGMP96_INTR_EN_set        ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rx_field_RGMP96_INTR_EN_get        ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rx_field_RGFPF96_INTR_EN_set       ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rx_field_RGFPF96_INTR_EN_get       ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rx_field_MAPOTN_RX_TOP_INTR_EN_set ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rx_field_MAPOTN_RX_TOP_INTR_EN_get ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | +----------------------------------+
 | | Register 0x00000310 INTR_REG     |
 | +----------------------------------+
 | | bit  19:16 R  MAPOTN_INTR        |
 | | bit  8     R  MPMO_INTR          |
 | | bit  7     R  ODUKSC_INTR        |
 | | bit  6     R  TGMP96_INTR        |
 | | bit  5     R  TGFPF96_INTR       |
 | | bit  4     R  MAPOTN_TX_TOP_INTR |
 | | bit  3     R  MPMA_INTR          |
 | | bit  2     R  RGMP96_INTR        |
 | | bit  1     R  RGFPF96_INTR       |
 | | bit  0     R  MAPOTN_RX_TOP_INTR |
 | +----------------------------------+
 | 
 | UINT32 mapotn_rx_field_MAPOTN_INTR_get        ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | UINT32 mapotn_rx_field_range_MAPOTN_INTR_get  ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mapotn_rx_field_MPMO_INTR_get          ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | UINT32 mapotn_rx_field_ODUKSC_INTR_get        ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | UINT32 mapotn_rx_field_TGMP96_INTR_get        ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | UINT32 mapotn_rx_field_TGFPF96_INTR_get       ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | UINT32 mapotn_rx_field_MAPOTN_TX_TOP_INTR_get ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | UINT32 mapotn_rx_field_MPMA_INTR_get          ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | UINT32 mapotn_rx_field_RGMP96_INTR_get        ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | UINT32 mapotn_rx_field_RGFPF96_INTR_get       ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | UINT32 mapotn_rx_field_MAPOTN_RX_TOP_INTR_get ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _MAPOTN_RX_IO_H
#define _MAPOTN_RX_IO_H

#include "mapotn_loc.h"
#include "mapotn_rx_regs.h"
#include "mapotn_rx_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mapotn_rx_buffer_init               ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void                 mapotn_rx_buffer_flush              ( mapotn_rx_buffer_t *b_ptr )
 | UINT32               mapotn_rx_reg_read                  ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 mapotn_rx_reg_write                 ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 mapotn_rx_field_set                 ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rx_action_on_write_field_set ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rx_burst_read                ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 mapotn_rx_burst_write               ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE mapotn_rx_poll                      ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mapotn_rx_reg_LOWPWR_MODE_write                                      ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rx_reg_LOWPWR_MODE_field_set                                  ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rx_reg_LOWPWR_MODE_read                                       ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_RSTB_write                          ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_RSTB_field_set                      ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_RSTB_read                           ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rx_reg_RAM_LOWPWR_MODE_write                                  ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rx_reg_RAM_LOWPWR_MODE_field_set                              ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rx_reg_RAM_LOWPWR_MODE_read                                   ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_burst_write              ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_field_set                ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_burst_read               ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rx_reg_DATA_PATH_CTRL_REGISTER_array_read                     ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rx_reg_INTR_EN_REG_array_write                                ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mapotn_rx_reg_INTR_EN_REG_array_field_set                            ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rx_reg_INTR_EN_REG_array_read                                 ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_write              ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_field_set          ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_read               ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_write                     ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_action_on_write_field_set ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_read                      ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_rx_reg_DCI_DPI_ASYNC_FIFO_FULL_INTR_poll                      ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               mapotn_rx_reg_INTR_REG_read                                          ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rx_reg_SOFT_RST_write                                         ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rx_reg_SOFT_RST_field_set                                     ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rx_reg_SOFT_RST_read                                          ( mapotn_rx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _MAPOTN_RX_IO_H */
