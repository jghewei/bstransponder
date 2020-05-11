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
 *     and register accessor functions for the shpi block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing shpi io functions is:
 | {
 |     shpi_buffer_t b_ptr[1];
 |     shpi_buffer_init( b_ptr, h_ptr, A );
 |     value = shpi_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     shpi_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     shpi_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = shpi_field_<FIELD>_get( NULL, h_ptr, A );
 | shpi_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------+
 | | Register 0x00001840 IND_ADR |
 | +-----------------------------+
 | | bit  15  R    BUSY          |
 | | bit  14  R/W  RWB           |
 | | bit  8:6 R/W  IADDR         |
 | +-----------------------------+
 | 
 | void                 shpi_field_RWB_set         ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               shpi_field_RWB_get         ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void                 shpi_field_IADDR_set       ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               shpi_field_IADDR_get       ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void                 shpi_field_range_IADDR_set ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               shpi_field_range_IADDR_get ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32               shpi_field_BUSY_get        ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE shpi_field_BUSY_poll       ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------+
 | | Register 0x00001844 IND_DATA |
 | +------------------------------+
 | | bit  15:0 R/W  DATA          |
 | +------------------------------+
 | 
 | void   shpi_field_DATA_set       ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 shpi_field_DATA_get       ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   shpi_field_range_DATA_set ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 shpi_field_range_DATA_get ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _SHPI_IO_H
#define _SHPI_IO_H

#include "cbrc_api.h"
#include "shpi_regs.h"
#include "shpi_io_inline.h"


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 shpi_buffer_init               ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void                 shpi_buffer_flush              ( shpi_buffer_t *b_ptr )
 | UINT32               shpi_reg_read                  ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 shpi_reg_write                 ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 shpi_field_set                 ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 shpi_action_on_write_field_set ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 shpi_burst_read                ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 shpi_burst_write               ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE shpi_poll                      ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 shpi_reg_IND_ADR_write      ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 shpi_reg_IND_ADR_field_set  ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               shpi_reg_IND_ADR_read       ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE shpi_reg_IND_ADR_poll       ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 shpi_reg_IND_DATA_write     ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 shpi_reg_IND_DATA_field_set ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               shpi_reg_IND_DATA_read      ( shpi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _SHPI_IO_H */
