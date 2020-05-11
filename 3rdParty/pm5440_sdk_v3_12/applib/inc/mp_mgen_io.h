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
 *     and register accessor functions for the mp_mgen block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing mp_mgen io functions is:
 | {
 |     mp_mgen_buffer_t b_ptr[1];
 |     mp_mgen_buffer_init( b_ptr, h_ptr );
 |     value = mp_mgen_field_<FIELD>_get( b_ptr, h_ptr );
 |     mp_mgen_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     mp_mgen_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = mp_mgen_field_<FIELD>_get( NULL, h_ptr );
 | mp_mgen_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------------------------+
 | | Register (0x00000000 + (N) * 0x8) IPT_ENTRY |
 | |     N[0..95]                                |
 | +---------------------------------------------+
 | | bit  16   R/W  IPT_VALID                    |
 | | bit  14:8 R/W  IPT_ID                       |
 | | bit  6:0  R/W  IPT_VA                       |
 | +---------------------------------------------+
 | 
 | void   mp_mgen_field_IPT_VALID_set    ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mp_mgen_field_IPT_VALID_get    ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N )
 | void   mp_mgen_field_IPT_ID_set       ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mp_mgen_field_IPT_ID_get       ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N )
 | void   mp_mgen_field_range_IPT_ID_set ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mp_mgen_field_range_IPT_ID_get ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mp_mgen_field_IPT_VA_set       ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mp_mgen_field_IPT_VA_get       ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N )
 | void   mp_mgen_field_range_IPT_VA_set ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mp_mgen_field_range_IPT_VA_get ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register (0x00000004 + (N) * 0x8) AMP_CFGS |
 | |     N[0..95]                               |
 | +--------------------------------------------+
 | | bit  25:24 R/W  NJO_TYPE                   |
 | | bit  21:16 R/W  NJO_POS                    |
 | | bit  11:0  R/W  NJO_ODTU_CYCLE             |
 | +--------------------------------------------+
 | 
 | void   mp_mgen_field_NJO_TYPE_set             ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mp_mgen_field_NJO_TYPE_get             ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N )
 | void   mp_mgen_field_range_NJO_TYPE_set       ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mp_mgen_field_range_NJO_TYPE_get       ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mp_mgen_field_NJO_POS_set              ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mp_mgen_field_NJO_POS_get              ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N )
 | void   mp_mgen_field_range_NJO_POS_set        ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mp_mgen_field_range_NJO_POS_get        ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mp_mgen_field_NJO_ODTU_CYCLE_set       ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mp_mgen_field_NJO_ODTU_CYCLE_get       ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N )
 | void   mp_mgen_field_range_NJO_ODTU_CYCLE_set ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mp_mgen_field_range_NJO_ODTU_CYCLE_get ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00000400 + (N) * 0x4) DIV_48_M_LAST_TXR |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  21:16 R/W  LAST_TXR_BYTES                      |
 | | bit  13:8  R/W  MOD_48_M                            |
 | | bit  5:0   R/W  DIV_48_M                            |
 | +-----------------------------------------------------+
 | 
 | void                 mp_mgen_field_LAST_TXR_BYTES_set        ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               mp_mgen_field_LAST_TXR_BYTES_get        ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N )
 | void                 mp_mgen_field_range_LAST_TXR_BYTES_set  ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               mp_mgen_field_range_LAST_TXR_BYTES_get  ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE mp_mgen_field_range_LAST_TXR_BYTES_poll ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE mp_mgen_field_LAST_TXR_BYTES_poll       ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mp_mgen_field_MOD_48_M_set              ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               mp_mgen_field_MOD_48_M_get              ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N )
 | void                 mp_mgen_field_range_MOD_48_M_set        ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               mp_mgen_field_range_MOD_48_M_get        ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void                 mp_mgen_field_DIV_48_M_set              ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               mp_mgen_field_DIV_48_M_get              ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N )
 | void                 mp_mgen_field_range_DIV_48_M_set        ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               mp_mgen_field_range_DIV_48_M_get        ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _MP_MGEN_IO_H
#define _MP_MGEN_IO_H

#include "pmc_sys.h"
#include "pmc_types.h"
#include "mp_mgen_loc.h"
#include "mp_mgen.h"
#include "mp_mgen_regs.h"
#include "mp_mgen_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mp_mgen_buffer_init               ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr )
 | void                 mp_mgen_buffer_flush              ( mp_mgen_buffer_t *b_ptr )
 | UINT32               mp_mgen_reg_read                  ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 mp_mgen_reg_write                 ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 mp_mgen_field_set                 ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 mp_mgen_action_on_write_field_set ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mp_mgen_burst_read                ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 mp_mgen_burst_write               ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE mp_mgen_poll                      ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mp_mgen_reg_IPT_ENTRY_array_write             ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mp_mgen_reg_IPT_ENTRY_array_field_set         ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mp_mgen_reg_IPT_ENTRY_array_read              ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mp_mgen_reg_AMP_CFGS_array_write              ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mp_mgen_reg_AMP_CFGS_array_field_set          ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mp_mgen_reg_AMP_CFGS_array_read               ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mp_mgen_reg_DIV_48_M_LAST_TXR_array_write     ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mp_mgen_reg_DIV_48_M_LAST_TXR_array_field_set ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mp_mgen_reg_DIV_48_M_LAST_TXR_array_read      ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE mp_mgen_reg_DIV_48_M_LAST_TXR_array_poll      ( mp_mgen_buffer_t *b_ptr, mp_mgen_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _MP_MGEN_IO_H */
