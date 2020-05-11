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
 *     and register accessor functions for the rttp block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing rttp io functions is:
 | {
 |     rttp_buffer_t b_ptr[1];
 |     rttp_buffer_init( b_ptr, h_ptr, A );
 |     value = rttp_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     rttp_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     rttp_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = rttp_field_<FIELD>_get( NULL, h_ptr, A );
 | rttp_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------+
 | | Register 0x00000500 IND_ADR |
 | +-----------------------------+
 | | bit  15   R    BUSY         |
 | | bit  14   R/W  RWB          |
 | | bit  13:6 R/W  IADDR        |
 | +-----------------------------+
 | 
 | void                 rttp_field_RWB_set         ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               rttp_field_RWB_get         ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void                 rttp_field_IADDR_set       ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               rttp_field_IADDR_get       ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void                 rttp_field_range_IADDR_set ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               rttp_field_range_IADDR_get ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32               rttp_field_BUSY_get        ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE rttp_field_BUSY_poll       ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------+
 | | Register 0x00000504 IND_DATA |
 | +------------------------------+
 | | bit  15:0 R/W  DATA          |
 | +------------------------------+
 | 
 | void   rttp_field_DATA_set       ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rttp_field_DATA_get       ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rttp_field_range_DATA_set ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rttp_field_range_DATA_get ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000508 PATH_INT_STAT_1 |
 | +-------------------------------------+
 | | bit  0 R  S_INT                     |
 | +-------------------------------------+
 | 
 | UINT32 rttp_field_S_INT_get ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _RTTP_IO_H
#define _RTTP_IO_H

#include "sdh_pmg_api.h"
#include "rttp_regs.h"
#include "rttp_io_inline.h"


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 rttp_buffer_init               ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void                 rttp_buffer_flush              ( rttp_buffer_t *b_ptr )
 | UINT32               rttp_reg_read                  ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 rttp_reg_write                 ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 rttp_field_set                 ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 rttp_action_on_write_field_set ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rttp_burst_read                ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 rttp_burst_write               ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE rttp_poll                      ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 rttp_reg_IND_ADR_write        ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 rttp_reg_IND_ADR_field_set    ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               rttp_reg_IND_ADR_read         ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE rttp_reg_IND_ADR_poll         ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 rttp_reg_IND_DATA_write       ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 rttp_reg_IND_DATA_field_set   ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               rttp_reg_IND_DATA_read        ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               rttp_reg_PATH_INT_STAT_1_read ( rttp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _RTTP_IO_H */
