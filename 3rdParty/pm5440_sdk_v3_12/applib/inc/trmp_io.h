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
 *     and register accessor functions for the trmp block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing trmp io functions is:
 | {
 |     trmp_buffer_t b_ptr[1];
 |     trmp_buffer_init( b_ptr, h_ptr, A );
 |     value = trmp_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     trmp_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     trmp_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = trmp_field_<FIELD>_get( NULL, h_ptr, A );
 | trmp_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------+
 | | Register 0x00001040 CFG   |
 | +---------------------------+
 | | bit  15 R/W  SCRAMA1A2SEL |
 | | bit  14 R/W  SCRAMJ0SEL   |
 | | bit  13 R/W  FRM768SEL    |
 | | bit  12 R/W  M0EN         |
 | | bit  3  R/W  STRACEEN     |
 | | bit  2  R/W  J0Z0INCEN    |
 | | bit  1  R/W  Z0DEF        |
 | | bit  0  R/W  A1A2EN       |
 | +---------------------------+
 | 
 | void   trmp_field_SCRAMA1A2SEL_set ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_SCRAMA1A2SEL_get ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_SCRAMJ0SEL_set   ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_SCRAMJ0SEL_get   ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_FRM768SEL_set    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_FRM768SEL_get    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_M0EN_set         ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_M0EN_get         ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_STRACEEN_set     ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_STRACEEN_get     ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_J0Z0INCEN_set    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_J0Z0INCEN_get    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_Z0DEF_set        ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_Z0DEF_get        ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_A1A2EN_set       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_A1A2EN_get       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | +-------------------------------+
 | | Register 0x00001044 INSERTION |
 | +-------------------------------+
 | | bit  12 R/W  NATIONALEN       |
 | | bit  10 R/W  E2REGEN          |
 | | bit  9  R/W  Z2REGEN          |
 | | bit  8  R/W  Z1REGEN          |
 | | bit  7  R/W  S1REGEN          |
 | | bit  6  R/W  D4D12REGEN       |
 | | bit  5  R/W  K1K2REGEN        |
 | | bit  4  R/W  D1D3REGEN        |
 | | bit  3  R/W  F1REGEN          |
 | | bit  2  R/W  E1REGEN          |
 | | bit  1  R/W  Z0REGEN          |
 | | bit  0  R/W  J0REGEN          |
 | +-------------------------------+
 | 
 | void   trmp_field_NATIONALEN_set ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_NATIONALEN_get ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_E2REGEN_set    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_E2REGEN_get    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_Z2REGEN_set    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_Z2REGEN_get    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_Z1REGEN_set    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_Z1REGEN_get    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_S1REGEN_set    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_S1REGEN_get    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_D4D12REGEN_set ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_D4D12REGEN_get ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_K1K2REGEN_set  ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_K1K2REGEN_get  ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_D1D3REGEN_set  ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_D1D3REGEN_get  ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_F1REGEN_set    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_F1REGEN_get    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_E1REGEN_set    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_E1REGEN_get    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_Z0REGEN_set    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_Z0REGEN_get    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_J0REGEN_set    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_J0REGEN_get    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------+
 | | Register 0x0000104c TX_J0_AND_Z0 |
 | +----------------------------------+
 | | bit  15:8 R/W  J0V               |
 | | bit  7:0  R/W  Z0V               |
 | +----------------------------------+
 | 
 | void   trmp_field_J0V_set       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_J0V_get       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_range_J0V_set ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 trmp_field_range_J0V_get ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   trmp_field_Z0V_set       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_Z0V_get       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_range_Z0V_set ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 trmp_field_range_Z0V_get ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00001050 TX_E1_AND_F1 |
 | +----------------------------------+
 | | bit  15:8 R/W  E1V               |
 | | bit  7:0  R/W  F1V               |
 | +----------------------------------+
 | 
 | void   trmp_field_E1V_set       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_E1V_get       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_range_E1V_set ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 trmp_field_range_E1V_get ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   trmp_field_F1V_set       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_F1V_get       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_range_F1V_set ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 trmp_field_range_F1V_get ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00001054 TX_D1D3_AND_D4D12 |
 | +---------------------------------------+
 | | bit  15:8 R/W  D1D3V                  |
 | | bit  7:0  R/W  D4D12V                 |
 | +---------------------------------------+
 | 
 | void   trmp_field_D1D3V_set        ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_D1D3V_get        ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_range_D1D3V_set  ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 trmp_field_range_D1D3V_get  ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   trmp_field_D4D12V_set       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_D4D12V_get       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_range_D4D12V_set ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 trmp_field_range_D4D12V_get ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00001058 TX_K1_AND_K2 |
 | +----------------------------------+
 | | bit  15:8 R/W  K1V               |
 | | bit  7:0  R/W  K2V               |
 | +----------------------------------+
 | 
 | void   trmp_field_K1V_set       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_K1V_get       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_range_K1V_set ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 trmp_field_range_K1V_get ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   trmp_field_K2V_set       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_K2V_get       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_range_K2V_set ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 trmp_field_range_K2V_get ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x0000105c TX_S1_AND_Z1 |
 | +----------------------------------+
 | | bit  15:8 R/W  S1V               |
 | | bit  7:0  R/W  Z1V               |
 | +----------------------------------+
 | 
 | void   trmp_field_S1V_set       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_S1V_get       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_range_S1V_set ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 trmp_field_range_S1V_get ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   trmp_field_Z1V_set       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_Z1V_get       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_range_Z1V_set ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 trmp_field_range_Z1V_get ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00001060 TX_Z2_AND_E2 |
 | +----------------------------------+
 | | bit  15:8 R/W  Z2V               |
 | | bit  7:0  R/W  E2V               |
 | +----------------------------------+
 | 
 | void   trmp_field_Z2V_set       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_Z2V_get       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_range_Z2V_set ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 trmp_field_range_Z2V_get ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   trmp_field_E2V_set       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_E2V_get       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_range_E2V_set ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 trmp_field_range_E2V_get ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x0000106c TX_NATIONAL |
 | +---------------------------------+
 | | bit  7:0 R/W  NATIONALV         |
 | +---------------------------------+
 | 
 | void   trmp_field_NATIONALV_set       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 trmp_field_NATIONALV_get       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   trmp_field_range_NATIONALV_set ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 trmp_field_range_NATIONALV_get ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 +------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _TRMP_IO_H
#define _TRMP_IO_H

#include "cbrc_api.h"
#include "trmp_regs.h"
#include "trmp_io_inline.h"


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 trmp_buffer_init               ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void                 trmp_buffer_flush              ( trmp_buffer_t *b_ptr )
 | UINT32               trmp_reg_read                  ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 trmp_reg_write                 ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 trmp_field_set                 ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 trmp_action_on_write_field_set ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 trmp_burst_read                ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 trmp_burst_write               ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE trmp_poll                      ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------+
 | void   trmp_reg_CFG_write                   ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   trmp_reg_CFG_field_set               ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 trmp_reg_CFG_read                    ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void   trmp_reg_INSERTION_write             ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   trmp_reg_INSERTION_field_set         ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 trmp_reg_INSERTION_read              ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void   trmp_reg_TX_J0_AND_Z0_write          ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   trmp_reg_TX_J0_AND_Z0_field_set      ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 trmp_reg_TX_J0_AND_Z0_read           ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void   trmp_reg_TX_E1_AND_F1_write          ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   trmp_reg_TX_E1_AND_F1_field_set      ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 trmp_reg_TX_E1_AND_F1_read           ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void   trmp_reg_TX_D1D3_AND_D4D12_write     ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   trmp_reg_TX_D1D3_AND_D4D12_field_set ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 trmp_reg_TX_D1D3_AND_D4D12_read      ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void   trmp_reg_TX_K1_AND_K2_write          ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   trmp_reg_TX_K1_AND_K2_field_set      ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 trmp_reg_TX_K1_AND_K2_read           ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void   trmp_reg_TX_S1_AND_Z1_write          ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   trmp_reg_TX_S1_AND_Z1_field_set      ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 trmp_reg_TX_S1_AND_Z1_read           ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void   trmp_reg_TX_Z2_AND_E2_write          ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   trmp_reg_TX_Z2_AND_E2_field_set      ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 trmp_reg_TX_Z2_AND_E2_read           ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void   trmp_reg_TX_NATIONAL_write           ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   trmp_reg_TX_NATIONAL_field_set       ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 trmp_reg_TX_NATIONAL_read            ( trmp_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _TRMP_IO_H */
