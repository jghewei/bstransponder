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
 *     and register accessor functions for the emac_prbs block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing emac_prbs io functions is:
 | {
 |     emac_prbs_buffer_t b_ptr[1];
 |     emac_prbs_buffer_init( b_ptr, h_ptr );
 |     value = emac_prbs_field_<FIELD>_get( b_ptr, h_ptr );
 |     emac_prbs_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     emac_prbs_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = emac_prbs_field_<FIELD>_get( NULL, h_ptr );
 | emac_prbs_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-------------------------------+
 | | Register 0x0000f800 PRBS_CTRL |
 | +-------------------------------+
 | | bit  23:12 R/W  PRBS9_ENA     |
 | | bit  11:0  R/W  PRBS31_ENA    |
 | +-------------------------------+
 | 
 | void   emac_prbs_field_PRBS9_ENA_set        ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_prbs_field_PRBS9_ENA_get        ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_prbs_field_range_PRBS9_ENA_set  ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_prbs_field_range_PRBS9_ENA_get  ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_prbs_field_PRBS31_ENA_set       ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_prbs_field_PRBS31_ENA_get       ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_prbs_field_range_PRBS31_ENA_set ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_prbs_field_range_PRBS31_ENA_get ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x0000f804 + (N) * 4) LANE_PRBS31_ERR_CNT |
 | |     N[0..11]                                        |
 | +-----------------------------------------------------+
 | | bit  15:0 R/W  PRBS31_ERR_CNT                       |
 | +-----------------------------------------------------+
 | 
 | void   emac_prbs_field_PRBS31_ERR_CNT_set       ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 emac_prbs_field_PRBS31_ERR_CNT_get       ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32  N )
 | void   emac_prbs_field_range_PRBS31_ERR_CNT_set ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_prbs_field_range_PRBS31_ERR_CNT_get ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x0000f834 + (N) * 4) LANE_PRBS9_ERR_CNT |
 | |     N[0..11]                                       |
 | +----------------------------------------------------+
 | | bit  15:0 R/W  PRBS9_ERR_CNT                       |
 | +----------------------------------------------------+
 | 
 | void   emac_prbs_field_PRBS9_ERR_CNT_set       ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 emac_prbs_field_PRBS9_ERR_CNT_get       ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32  N )
 | void   emac_prbs_field_range_PRBS9_ERR_CNT_set ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_prbs_field_range_PRBS9_ERR_CNT_get ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x0000f864 PRBS_STATUS |
 | +---------------------------------+
 | | bit  11:0 R  PRBS_ALL_ONES      |
 | +---------------------------------+
 | 
 | UINT32 emac_prbs_field_PRBS_ALL_ONES_get       ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | UINT32 emac_prbs_field_range_PRBS_ALL_ONES_get ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _EMAC_PRBS_IO_H
#define _EMAC_PRBS_IO_H

#include "emac_loc.h"
#include "emac_prbs_regs.h"
#include "emac_prbs_io_inline.h"


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 emac_prbs_buffer_init               ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void                 emac_prbs_buffer_flush              ( emac_prbs_buffer_t *b_ptr )
 | UINT32               emac_prbs_reg_read                  ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 emac_prbs_reg_write                 ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 emac_prbs_field_set                 ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 emac_prbs_action_on_write_field_set ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 emac_prbs_burst_read                ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 emac_prbs_burst_write               ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE emac_prbs_poll                      ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   emac_prbs_reg_PRBS_CTRL_write                     ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void   emac_prbs_reg_PRBS_CTRL_field_set                 ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_prbs_reg_PRBS_CTRL_read                      ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | UINT32 emac_prbs_reg_PRBS_STATUS_read                    ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void   emac_prbs_reg_LANE_PRBS31_ERR_CNT_array_write     ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   emac_prbs_reg_LANE_PRBS31_ERR_CNT_array_field_set ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_prbs_reg_LANE_PRBS31_ERR_CNT_array_read      ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32  N )
 | 
 | void   emac_prbs_reg_LANE_PRBS9_ERR_CNT_array_write      ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   emac_prbs_reg_LANE_PRBS9_ERR_CNT_array_field_set  ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_prbs_reg_LANE_PRBS9_ERR_CNT_array_read       ( emac_prbs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32  N )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _EMAC_PRBS_IO_H */
