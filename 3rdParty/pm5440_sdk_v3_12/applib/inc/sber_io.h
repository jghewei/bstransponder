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
 *     and register accessor functions for the sber block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing sber io functions is:
 | {
 |     sber_buffer_t b_ptr[1];
 |     sber_buffer_init( b_ptr, h_ptr, A );
 |     value = sber_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     sber_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     sber_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = sber_field_<FIELD>_get( NULL, h_ptr, A );
 | sber_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------+
 | | Register 0x00000600 IND_ADR |
 | +-----------------------------+
 | | bit  15   R    BUSY         |
 | | bit  14   R/W  RWB          |
 | | bit  10:6 R/W  IADDR        |
 | +-----------------------------+
 | 
 | void                 sber_field_RWB_set         ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               sber_field_RWB_get         ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void                 sber_field_IADDR_set       ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               sber_field_IADDR_get       ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void                 sber_field_range_IADDR_set ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               sber_field_range_IADDR_get ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32               sber_field_BUSY_get        ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sber_field_BUSY_poll       ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------+
 | | Register 0x00000604 IND_DATA |
 | +------------------------------+
 | | bit  15:0 R/W  DATA          |
 | +------------------------------+
 | 
 | void   sber_field_DATA_set       ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 sber_field_DATA_get       ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   sber_field_range_DATA_set ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sber_field_range_DATA_get ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000608 PATH_INT_STAT_1 |
 | +-------------------------------------+
 | | bit  0 R  S_INT                     |
 | +-------------------------------------+
 | 
 | UINT32 sber_field_S_INT_get ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _SBER_IO_H
#define _SBER_IO_H

#include "sdh_pmg_api.h"
#include "sber_regs.h"
#include "sber_io_inline.h"


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sber_buffer_init               ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void                 sber_buffer_flush              ( sber_buffer_t *b_ptr )
 | UINT32               sber_reg_read                  ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 sber_reg_write                 ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 sber_field_set                 ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 sber_action_on_write_field_set ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 sber_burst_read                ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 sber_burst_write               ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE sber_poll                      ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sber_reg_IND_ADR_write        ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 sber_reg_IND_ADR_field_set    ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sber_reg_IND_ADR_read         ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sber_reg_IND_ADR_poll         ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sber_reg_IND_DATA_write       ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 sber_reg_IND_DATA_field_set   ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sber_reg_IND_DATA_read        ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               sber_reg_PATH_INT_STAT_1_read ( sber_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _SBER_IO_H */
