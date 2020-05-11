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
 *     and register accessor functions for the mapotn_tx block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing mapotn_tx io functions is:
 | {
 |     mapotn_tx_buffer_t b_ptr[1];
 |     mapotn_tx_buffer_init( b_ptr, h_ptr );
 |     value = mapotn_tx_field_<FIELD>_get( b_ptr, h_ptr );
 |     mapotn_tx_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     mapotn_tx_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = mapotn_tx_field_<FIELD>_get( NULL, h_ptr );
 | mapotn_tx_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +--------------------------------+
 | | Register 0x00080000 SOFT_RST   |
 | +--------------------------------+
 | | bit  4 R/W  ODUKSC_SOFT_RST    |
 | | bit  3 R/W  MPMO_SOFT_RST      |
 | | bit  2 R/W  TGMP96_SOFT_RST    |
 | | bit  1 R/W  TGFPF96_SOFT_RST   |
 | | bit  0 R/W  MAPOTN_TX_SOFT_RST |
 | +--------------------------------+
 | 
 | void   mapotn_tx_field_ODUKSC_SOFT_RST_set    ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tx_field_ODUKSC_SOFT_RST_get    ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_tx_field_MPMO_SOFT_RST_set      ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tx_field_MPMO_SOFT_RST_get      ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_tx_field_TGMP96_SOFT_RST_set    ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tx_field_TGMP96_SOFT_RST_get    ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_tx_field_TGFPF96_SOFT_RST_set   ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tx_field_TGFPF96_SOFT_RST_get   ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_tx_field_MAPOTN_TX_SOFT_RST_set ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tx_field_MAPOTN_TX_SOFT_RST_get ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +---------------------------------+
 | | Register 0x00080004 LOWPWR_MODE |
 | +---------------------------------+
 | | bit  3 R/W  ODUKSC_LOWPWR       |
 | | bit  2 R/W  MPMO_LOWPWR         |
 | | bit  1 R/W  TGMP96_LOWPWR       |
 | | bit  0 R/W  TGFPF96_LOWPWR      |
 | +---------------------------------+
 | 
 | void   mapotn_tx_field_ODUKSC_LOWPWR_set  ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tx_field_ODUKSC_LOWPWR_get  ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_tx_field_MPMO_LOWPWR_set    ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tx_field_MPMO_LOWPWR_get    ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_tx_field_TGMP96_LOWPWR_set  ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tx_field_TGMP96_LOWPWR_get  ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_tx_field_TGFPF96_LOWPWR_set ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tx_field_TGFPF96_LOWPWR_get ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +-------------------------------------+
 | | Register 0x0008000c RAM_LOWPWR_MODE |
 | +-------------------------------------+
 | | bit  3 R/W  ODUKSC_RAM_LOWPWR       |
 | | bit  2 R/W  MPMO_RAM_LOWPWR         |
 | | bit  1 R/W  TGMP96_RAM_LOWPWR       |
 | | bit  0 R/W  TGFPF96_RAM_LOWPWR      |
 | +-------------------------------------+
 | 
 | void   mapotn_tx_field_ODUKSC_RAM_LOWPWR_set  ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tx_field_ODUKSC_RAM_LOWPWR_get  ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_tx_field_MPMO_RAM_LOWPWR_set    ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tx_field_MPMO_RAM_LOWPWR_get    ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_tx_field_TGMP96_RAM_LOWPWR_set  ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tx_field_TGMP96_RAM_LOWPWR_get  ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_tx_field_TGFPF96_RAM_LOWPWR_set ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tx_field_TGFPF96_RAM_LOWPWR_get ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +-----------------------------------+
 | | Register 0x00080010 ENET_FIFO_CLR |
 | +-----------------------------------+
 | | bit  0 R/W  FIFO_CLRB             |
 | +-----------------------------------+
 | 
 | void   mapotn_tx_field_FIFO_CLRB_set ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tx_field_FIFO_CLRB_get ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x00080020 ENET_FIFO_OVFL_INT_EN |
 | +-------------------------------------------+
 | | bit  0 R/W  ENET_FIFO_OVFL_E              |
 | +-------------------------------------------+
 | 
 | void   mapotn_tx_field_ENET_FIFO_OVFL_E_set ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tx_field_ENET_FIFO_OVFL_E_get ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +-----------------------------------------+
 | | Register 0x00080024 ENET_FIFO_OVFL_INTR |
 | +-----------------------------------------+
 | | bit  0 R/W  ENET_FIFO_OVFL_I            |
 | +-----------------------------------------+
 | 
 | void                 mapotn_tx_field_ENET_FIFO_OVFL_I_set_to_clear ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32               mapotn_tx_field_ENET_FIFO_OVFL_I_get          ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_tx_field_ENET_FIFO_OVFL_I_poll         ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _MAPOTN_TX_IO_H
#define _MAPOTN_TX_IO_H

#include "mapotn_loc.h"
#include "mapotn_tx_regs.h"
#include "mapotn_tx_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mapotn_tx_buffer_init               ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void                 mapotn_tx_buffer_flush              ( mapotn_tx_buffer_t *b_ptr )
 | UINT32               mapotn_tx_reg_read                  ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 mapotn_tx_reg_write                 ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 mapotn_tx_field_set                 ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tx_action_on_write_field_set ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tx_burst_read                ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 mapotn_tx_burst_write               ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE mapotn_tx_poll                      ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mapotn_tx_reg_LOWPWR_MODE_write                             ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_tx_reg_LOWPWR_MODE_field_set                         ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_tx_reg_LOWPWR_MODE_read                              ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_tx_reg_RAM_LOWPWR_MODE_write                         ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_tx_reg_RAM_LOWPWR_MODE_field_set                     ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_tx_reg_RAM_LOWPWR_MODE_read                          ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_tx_reg_ENET_FIFO_CLR_write                           ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_tx_reg_ENET_FIFO_CLR_field_set                       ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_tx_reg_ENET_FIFO_CLR_read                            ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_tx_reg_ENET_FIFO_OVFL_INT_EN_write                   ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_tx_reg_ENET_FIFO_OVFL_INT_EN_field_set               ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_tx_reg_ENET_FIFO_OVFL_INT_EN_read                    ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_tx_reg_ENET_FIFO_OVFL_INTR_write                     ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_tx_reg_ENET_FIFO_OVFL_INTR_action_on_write_field_set ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_tx_reg_ENET_FIFO_OVFL_INTR_read                      ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_tx_reg_ENET_FIFO_OVFL_INTR_poll                      ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_tx_reg_SOFT_RST_write                                ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_tx_reg_SOFT_RST_field_set                            ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_tx_reg_SOFT_RST_read                                 ( mapotn_tx_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _MAPOTN_TX_IO_H */
