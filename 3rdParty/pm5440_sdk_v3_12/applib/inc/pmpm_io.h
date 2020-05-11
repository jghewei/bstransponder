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
 *     and register accessor functions for the pmpm block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing pmpm io functions is:
 | {
 |     pmpm_buffer_t b_ptr[1];
 |     pmpm_buffer_init( b_ptr, h_ptr, A );
 |     value = pmpm_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     pmpm_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     pmpm_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = pmpm_field_<FIELD>_get( NULL, h_ptr, A );
 | pmpm_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------------------------+
 | | Register (0x00068100 + (N) * 0x04) RAM_POWER_CONFIG |
 | |     N[0..31]                                        |
 | +-----------------------------------------------------+
 | | bit  17:16 R/W  PWR_MODE                            |
 | | bit  9     R/W  DYNAMIC_ADDR_LS_EN                  |
 | | bit  8     R/W  DYNAMIC_LS_EN                       |
 | +-----------------------------------------------------+
 | 
 | void   pmpm_field_PWR_MODE_set           ( pmpm_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 pmpm_field_PWR_MODE_get           ( pmpm_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   pmpm_field_range_PWR_MODE_set     ( pmpm_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 pmpm_field_range_PWR_MODE_get     ( pmpm_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   pmpm_field_DYNAMIC_ADDR_LS_EN_set ( pmpm_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 pmpm_field_DYNAMIC_ADDR_LS_EN_get ( pmpm_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   pmpm_field_DYNAMIC_LS_EN_set      ( pmpm_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 pmpm_field_DYNAMIC_LS_EN_get      ( pmpm_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _PMPM_IO_H
#define _PMPM_IO_H

#include "cpb_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmpm_regs.h"
#include "pmpm_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 pmpm_buffer_init               ( pmpm_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void                 pmpm_buffer_flush              ( pmpm_buffer_t *b_ptr )
 | UINT32               pmpm_reg_read                  ( pmpm_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 pmpm_reg_write                 ( pmpm_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 pmpm_field_set                 ( pmpm_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 pmpm_action_on_write_field_set ( pmpm_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 pmpm_burst_read                ( pmpm_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 pmpm_burst_write               ( pmpm_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE pmpm_poll                      ( pmpm_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   pmpm_reg_RAM_POWER_CONFIG_array_write     ( pmpm_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   pmpm_reg_RAM_POWER_CONFIG_array_field_set ( pmpm_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 pmpm_reg_RAM_POWER_CONFIG_array_read      ( pmpm_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _PMPM_IO_H */
