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
 *     and register accessor functions for the dcs block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing dcs io functions is:
 | {
 |     dcs_buffer_t b_ptr[1];
 |     dcs_buffer_init( b_ptr, h_ptr, A );
 |     value = dcs_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     dcs_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     dcs_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = dcs_field_<FIELD>_get( NULL, h_ptr, A );
 | dcs_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------+
 | | Register 0x00060000 CONFIG |
 | +----------------------------+
 | | bit  0 R/W  DCS_ENABLE     |
 | +----------------------------+
 | 
 | void   dcs_field_DCS_ENABLE_set ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 dcs_field_DCS_ENABLE_get ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------------+
 | | Register 0x00060004 TRANSFER_MODE     |
 | +---------------------------------------+
 | | bit  25 R/W  DCS_DISABLE_BURST_CREDIT |
 | | bit  24 R/W  DCS_INFINITE_QUANTUM     |
 | | bit  16 R/W  DCS_QUANTUM_MODE         |
 | | bit  8  R/W  DCS_BURST_MODE           |
 | | bit  0  R/W  DCS_XFER_MODE            |
 | +---------------------------------------+
 | 
 | void   dcs_field_DCS_DISABLE_BURST_CREDIT_set ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 dcs_field_DCS_DISABLE_BURST_CREDIT_get ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   dcs_field_DCS_INFINITE_QUANTUM_set     ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 dcs_field_DCS_INFINITE_QUANTUM_get     ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   dcs_field_DCS_QUANTUM_MODE_set         ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 dcs_field_DCS_QUANTUM_MODE_get         ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   dcs_field_DCS_BURST_MODE_set           ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 dcs_field_DCS_BURST_MODE_get           ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   dcs_field_DCS_XFER_MODE_set            ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 dcs_field_DCS_XFER_MODE_get            ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------+
 | | Register 0x00060008 CREDIT_CONFIG |
 | +-----------------------------------+
 | | bit  24:16 R/W  DCS_BURST_SIZE    |
 | | bit  15:0  R/W  DCS_QUANTUM_SIZE  |
 | +-----------------------------------+
 | 
 | void   dcs_field_DCS_BURST_SIZE_set         ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 dcs_field_DCS_BURST_SIZE_get         ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   dcs_field_range_DCS_BURST_SIZE_set   ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcs_field_range_DCS_BURST_SIZE_get   ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   dcs_field_DCS_QUANTUM_SIZE_set       ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 dcs_field_DCS_QUANTUM_SIZE_get       ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   dcs_field_range_DCS_QUANTUM_SIZE_set ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcs_field_range_DCS_QUANTUM_SIZE_get ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x0006000c QUEUE_OFFSET_CONFIG |
 | +-----------------------------------------+
 | | bit  7:0 R/W  DCS_QUEUE_OFFSET          |
 | +-----------------------------------------+
 | 
 | void   dcs_field_DCS_QUEUE_OFFSET_set       ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 dcs_field_DCS_QUEUE_OFFSET_get       ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   dcs_field_range_DCS_QUEUE_OFFSET_set ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcs_field_range_DCS_QUEUE_OFFSET_get ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00060010 XOFF_MODE      |
 | +------------------------------------+
 | | bit  17:16 R/W  DCS_LINK_XOFF_MODE |
 | | bit  1:0   R/W  DCS_CH_XOFF_MODE   |
 | +------------------------------------+
 | 
 | void   dcs_field_DCS_LINK_XOFF_MODE_set       ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 dcs_field_DCS_LINK_XOFF_MODE_get       ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   dcs_field_range_DCS_LINK_XOFF_MODE_set ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcs_field_range_DCS_LINK_XOFF_MODE_get ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   dcs_field_DCS_CH_XOFF_MODE_set         ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 dcs_field_DCS_CH_XOFF_MODE_get         ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   dcs_field_range_DCS_CH_XOFF_MODE_set   ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcs_field_range_DCS_CH_XOFF_MODE_get   ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00060014 PAGE_SWAP_CTRL |
 | +------------------------------------+
 | | bit  0 R/W  DCS_PAGE_SWAP          |
 | +------------------------------------+
 | 
 | void   dcs_field_DCS_PAGE_SWAP_set ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 dcs_field_DCS_PAGE_SWAP_get ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------+
 | | Register 0x00060018 PAGE_SWAP_STAT |
 | +------------------------------------+
 | | bit  0 R  DCS_PAGE_SWAP_STATUS     |
 | +------------------------------------+
 | 
 | UINT32               dcs_field_DCS_PAGE_SWAP_STATUS_get  ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE dcs_field_DCS_PAGE_SWAP_STATUS_poll ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------+
 | | Register 0x0006001c TOP_OF_PAGE_CONFIG |
 | +----------------------------------------+
 | | bit  23:16 R/W  DCS_TOP_OF_PAGE_PTR    |
 | | bit  9:8   R/W  DCS_TOP_OF_PAGE_MODE   |
 | | bit  0     R/W  DCS_TOP_OF_PAGE        |
 | +----------------------------------------+
 | 
 | void   dcs_field_DCS_TOP_OF_PAGE_PTR_set        ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 dcs_field_DCS_TOP_OF_PAGE_PTR_get        ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   dcs_field_range_DCS_TOP_OF_PAGE_PTR_set  ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcs_field_range_DCS_TOP_OF_PAGE_PTR_get  ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   dcs_field_DCS_TOP_OF_PAGE_MODE_set       ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 dcs_field_DCS_TOP_OF_PAGE_MODE_get       ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   dcs_field_range_DCS_TOP_OF_PAGE_MODE_set ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcs_field_range_DCS_TOP_OF_PAGE_MODE_get ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   dcs_field_DCS_TOP_OF_PAGE_set            ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 dcs_field_DCS_TOP_OF_PAGE_get            ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------------------+
 | | Register 0x00060020 STANDBY_PAGE_END_CONFIG |
 | +---------------------------------------------+
 | | bit  7:0 R/W  DCS_PAGE_END_PTR              |
 | +---------------------------------------------+
 | 
 | void   dcs_field_DCS_PAGE_END_PTR_set       ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 dcs_field_DCS_PAGE_END_PTR_get       ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   dcs_field_range_DCS_PAGE_END_PTR_set ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcs_field_range_DCS_PAGE_END_PTR_get ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00060024 QUEUE_RESET |
 | +---------------------------------+
 | | bit  7:0 R/W  DCS_Q_RESET       |
 | +---------------------------------+
 | 
 | void   dcs_field_DCS_Q_RESET_set       ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 dcs_field_DCS_Q_RESET_get       ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   dcs_field_range_DCS_Q_RESET_set ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcs_field_range_DCS_Q_RESET_get ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00060400 + (N) * 0x4) STANDBY_CALENDAR_RAM |
 | |     N[0..95]                                           |
 | +--------------------------------------------------------+
 | | bit  25:24 R/W  C_RAM_SF1                              |
 | | bit  23:16 R/W  C_RAM_SF2                              |
 | | bit  15:8  R/W  C_RAM_CHANNEL                          |
 | | bit  6:0   R/W  C_RAM_IDENTIFIER                       |
 | +--------------------------------------------------------+
 | 
 | void   dcs_field_C_RAM_SF1_set              ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 dcs_field_C_RAM_SF1_get              ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   dcs_field_range_C_RAM_SF1_set        ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcs_field_range_C_RAM_SF1_get        ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   dcs_field_C_RAM_SF2_set              ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 dcs_field_C_RAM_SF2_get              ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   dcs_field_range_C_RAM_SF2_set        ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcs_field_range_C_RAM_SF2_get        ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   dcs_field_C_RAM_CHANNEL_set          ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 dcs_field_C_RAM_CHANNEL_get          ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   dcs_field_range_C_RAM_CHANNEL_set    ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcs_field_range_C_RAM_CHANNEL_get    ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   dcs_field_C_RAM_IDENTIFIER_set       ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 dcs_field_C_RAM_IDENTIFIER_get       ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   dcs_field_range_C_RAM_IDENTIFIER_set ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcs_field_range_C_RAM_IDENTIFIER_get ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _DCS_IO_H
#define _DCS_IO_H

#include "cpb_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "dcs_regs.h"
#include "dcs_io_inline.h"


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 dcs_buffer_init               ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void                 dcs_buffer_flush              ( dcs_buffer_t *b_ptr )
 | UINT32               dcs_reg_read                  ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 dcs_reg_write                 ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 dcs_field_set                 ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 dcs_action_on_write_field_set ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 dcs_burst_read                ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 dcs_burst_write               ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE dcs_poll                      ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 dcs_reg_CONFIG_write                         ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 dcs_reg_CONFIG_field_set                     ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcs_reg_CONFIG_read                          ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | void                 dcs_reg_TRANSFER_MODE_write                  ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 dcs_reg_TRANSFER_MODE_field_set              ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcs_reg_TRANSFER_MODE_read                   ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | void                 dcs_reg_CREDIT_CONFIG_write                  ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 dcs_reg_CREDIT_CONFIG_field_set              ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcs_reg_CREDIT_CONFIG_read                   ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | void                 dcs_reg_QUEUE_OFFSET_CONFIG_write            ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 dcs_reg_QUEUE_OFFSET_CONFIG_field_set        ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcs_reg_QUEUE_OFFSET_CONFIG_read             ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | void                 dcs_reg_XOFF_MODE_write                      ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 dcs_reg_XOFF_MODE_field_set                  ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcs_reg_XOFF_MODE_read                       ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | void                 dcs_reg_TOP_OF_PAGE_CONFIG_write             ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 dcs_reg_TOP_OF_PAGE_CONFIG_field_set         ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcs_reg_TOP_OF_PAGE_CONFIG_read              ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | void                 dcs_reg_QUEUE_RESET_write                    ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 dcs_reg_QUEUE_RESET_field_set                ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcs_reg_QUEUE_RESET_read                     ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | void                 dcs_reg_PAGE_SWAP_CTRL_write                 ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 dcs_reg_PAGE_SWAP_CTRL_field_set             ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcs_reg_PAGE_SWAP_CTRL_read                  ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | void                 dcs_reg_STANDBY_PAGE_END_CONFIG_write        ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 dcs_reg_STANDBY_PAGE_END_CONFIG_field_set    ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcs_reg_STANDBY_PAGE_END_CONFIG_read         ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | void                 dcs_reg_STANDBY_CALENDAR_RAM_array_write     ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 dcs_reg_STANDBY_CALENDAR_RAM_array_field_set ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcs_reg_STANDBY_CALENDAR_RAM_array_read      ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | UINT32               dcs_reg_PAGE_SWAP_STAT_read                  ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE dcs_reg_PAGE_SWAP_STAT_poll                  ( dcs_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _DCS_IO_H */
