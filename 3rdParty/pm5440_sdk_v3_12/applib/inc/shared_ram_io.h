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
 *     and register accessor functions for the shared_ram block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing shared_ram io functions is:
 | {
 |     shared_ram_buffer_t b_ptr[1];
 |     shared_ram_buffer_init( b_ptr, h_ptr, A );
 |     value = shared_ram_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     shared_ram_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     shared_ram_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = shared_ram_field_<FIELD>_get( NULL, h_ptr, A );
 | shared_ram_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------+
 | | Register 0x00001000 MASTER_CONFIG |
 | +-----------------------------------+
 | | bit  4:1 R/W  FEC_SELECT          |
 | | bit  0   R/W  RESET               |
 | +-----------------------------------+
 | 
 | void   shared_ram_field_FEC_SELECT_set       ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 shared_ram_field_FEC_SELECT_get       ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   shared_ram_field_range_FEC_SELECT_set ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 shared_ram_field_range_FEC_SELECT_get ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   shared_ram_field_RESET_set            ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 shared_ram_field_RESET_get            ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------+
 | | Register 0x00001028 LOWPWR_CONFIG |
 | +-----------------------------------+
 | | bit  31:28 R/W  REGION_F3_LOWPWR  |
 | | bit  27:24 R/W  REGION_F2_LOWPWR  |
 | | bit  23:20 R/W  REGION_F1_LOWPWR  |
 | | bit  19:16 R/W  REGION_E_LOWPWR   |
 | | bit  15:13 R/W  REGION_D2_LOWPWR  |
 | | bit  12:9  R/W  REGION_D1_LOWPWR  |
 | | bit  8:6   R/W  REGION_C_LOWPWR   |
 | | bit  5:4   R/W  REGION_B_LOWPWR   |
 | | bit  3:0   R/W  REGION_A_LOWPWR   |
 | +-----------------------------------+
 | 
 | void   shared_ram_field_REGION_F3_LOWPWR_set       ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 shared_ram_field_REGION_F3_LOWPWR_get       ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   shared_ram_field_range_REGION_F3_LOWPWR_set ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 shared_ram_field_range_REGION_F3_LOWPWR_get ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   shared_ram_field_REGION_F2_LOWPWR_set       ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 shared_ram_field_REGION_F2_LOWPWR_get       ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   shared_ram_field_range_REGION_F2_LOWPWR_set ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 shared_ram_field_range_REGION_F2_LOWPWR_get ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   shared_ram_field_REGION_F1_LOWPWR_set       ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 shared_ram_field_REGION_F1_LOWPWR_get       ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   shared_ram_field_range_REGION_F1_LOWPWR_set ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 shared_ram_field_range_REGION_F1_LOWPWR_get ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   shared_ram_field_REGION_E_LOWPWR_set        ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 shared_ram_field_REGION_E_LOWPWR_get        ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   shared_ram_field_range_REGION_E_LOWPWR_set  ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 shared_ram_field_range_REGION_E_LOWPWR_get  ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   shared_ram_field_REGION_D2_LOWPWR_set       ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 shared_ram_field_REGION_D2_LOWPWR_get       ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   shared_ram_field_range_REGION_D2_LOWPWR_set ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 shared_ram_field_range_REGION_D2_LOWPWR_get ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   shared_ram_field_REGION_D1_LOWPWR_set       ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 shared_ram_field_REGION_D1_LOWPWR_get       ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   shared_ram_field_range_REGION_D1_LOWPWR_set ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 shared_ram_field_range_REGION_D1_LOWPWR_get ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   shared_ram_field_REGION_C_LOWPWR_set        ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 shared_ram_field_REGION_C_LOWPWR_get        ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   shared_ram_field_range_REGION_C_LOWPWR_set  ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 shared_ram_field_range_REGION_C_LOWPWR_get  ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   shared_ram_field_REGION_B_LOWPWR_set        ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 shared_ram_field_REGION_B_LOWPWR_get        ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   shared_ram_field_range_REGION_B_LOWPWR_set  ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 shared_ram_field_range_REGION_B_LOWPWR_get  ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   shared_ram_field_REGION_A_LOWPWR_set        ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 shared_ram_field_REGION_A_LOWPWR_get        ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   shared_ram_field_range_REGION_A_LOWPWR_set  ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 shared_ram_field_range_REGION_A_LOWPWR_get  ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _SHARED_RAM_IO_H
#define _SHARED_RAM_IO_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "shared_ram_regs.h"
#include "shared_ram_io_inline.h"


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 shared_ram_buffer_init               ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void                 shared_ram_buffer_flush              ( shared_ram_buffer_t *b_ptr )
 | UINT32               shared_ram_reg_read                  ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 shared_ram_reg_write                 ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 shared_ram_field_set                 ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 shared_ram_action_on_write_field_set ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 shared_ram_burst_read                ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 shared_ram_burst_write               ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE shared_ram_poll                      ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +--------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   shared_ram_reg_MASTER_CONFIG_write     ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   shared_ram_reg_MASTER_CONFIG_field_set ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 shared_ram_reg_MASTER_CONFIG_read      ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void   shared_ram_reg_LOWPWR_CONFIG_write     ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   shared_ram_reg_LOWPWR_CONFIG_field_set ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 shared_ram_reg_LOWPWR_CONFIG_read      ( shared_ram_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 +-------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _SHARED_RAM_IO_H */
