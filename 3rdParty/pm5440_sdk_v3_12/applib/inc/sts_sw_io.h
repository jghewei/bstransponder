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
 *     and register accessor functions for the sts_sw block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing sts_sw io functions is:
 | {
 |     sts_sw_buffer_t b_ptr[1];
 |     sts_sw_buffer_init( b_ptr, h_ptr );
 |     value = sts_sw_field_<FIELD>_get( b_ptr, h_ptr );
 |     sts_sw_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     sts_sw_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = sts_sw_field_<FIELD>_get( NULL, h_ptr );
 | sts_sw_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------+
 | | Register 0x00000000 CONFIG |
 | +----------------------------+
 | | bit  8 R/W  MXDX_MODE_EN   |
 | +----------------------------+
 | 
 | void   sts_sw_field_MXDX_MODE_EN_set ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32 value )
 | UINT32 sts_sw_field_MXDX_MODE_EN_get ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register (0x00001000 + (N) * 0x20) HO_CFG |
 | |     N[0..95]                              |
 | +-------------------------------------------+
 | | bit  22:16 R/W  EOMF_CNT                  |
 | | bit  14:8  R/W  LAST_CNT                  |
 | | bit  6:0   R/W  HO_CNT_TOT                |
 | +-------------------------------------------+
 | 
 | void   sts_sw_field_EOMF_CNT_set         ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sts_sw_field_EOMF_CNT_get         ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N )
 | void   sts_sw_field_range_EOMF_CNT_set   ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sts_sw_field_range_EOMF_CNT_get   ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   sts_sw_field_LAST_CNT_set         ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sts_sw_field_LAST_CNT_get         ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N )
 | void   sts_sw_field_range_LAST_CNT_set   ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sts_sw_field_range_LAST_CNT_get   ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   sts_sw_field_HO_CNT_TOT_set       ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sts_sw_field_HO_CNT_TOT_get       ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N )
 | void   sts_sw_field_range_HO_CNT_TOT_set ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sts_sw_field_range_HO_CNT_TOT_get ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00001010 + (N) * 0x20) HO_CYCLE_CFG |
 | |     N[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  22:16 R/W  EOMF_CYCLE                      |
 | | bit  14:8  R/W  LAST_CYCLE                      |
 | | bit  6:0   R/W  FIRST_CYCLE                     |
 | +-------------------------------------------------+
 | 
 | void   sts_sw_field_EOMF_CYCLE_set        ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sts_sw_field_EOMF_CYCLE_get        ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N )
 | void   sts_sw_field_range_EOMF_CYCLE_set  ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sts_sw_field_range_EOMF_CYCLE_get  ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   sts_sw_field_LAST_CYCLE_set        ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sts_sw_field_LAST_CYCLE_get        ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N )
 | void   sts_sw_field_range_LAST_CYCLE_set  ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sts_sw_field_range_LAST_CYCLE_get  ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   sts_sw_field_FIRST_CYCLE_set       ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sts_sw_field_FIRST_CYCLE_get       ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N )
 | void   sts_sw_field_range_FIRST_CYCLE_set ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sts_sw_field_range_FIRST_CYCLE_get ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register (0x00002000 + (N) * 0x4) IPT |
 | |     N[0..95]                          |
 | +---------------------------------------+
 | | bit  16   R/W  IPT_VALID              |
 | | bit  14:8 R/W  IPT_ID                 |
 | | bit  6:0  R/W  IPT_VA                 |
 | +---------------------------------------+
 | 
 | void   sts_sw_field_IPT_VALID_set    ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sts_sw_field_IPT_VALID_get    ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N )
 | void   sts_sw_field_IPT_ID_set       ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sts_sw_field_IPT_ID_get       ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N )
 | void   sts_sw_field_range_IPT_ID_set ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sts_sw_field_range_IPT_ID_get ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   sts_sw_field_IPT_VA_set       ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sts_sw_field_IPT_VA_get       ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N )
 | void   sts_sw_field_range_IPT_VA_set ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sts_sw_field_range_IPT_VA_get ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00002200 + (N) * 0x4) CALENDAR_CHANNEL_ID |
 | |     N[0..95]                                          |
 | +-------------------------------------------------------+
 | | bit  24  R/W  MAP_DMSIM_TO_SSF                        |
 | | bit  16  R/W  LO_EOMF_CYCLE                           |
 | | bit  8   R/W  LO_FIRST_CYCLE                          |
 | | bit  6:0 R/W  CHAN_ID                                 |
 | +-------------------------------------------------------+
 | 
 | void   sts_sw_field_MAP_DMSIM_TO_SSF_set ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sts_sw_field_MAP_DMSIM_TO_SSF_get ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N )
 | void   sts_sw_field_LO_EOMF_CYCLE_set    ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sts_sw_field_LO_EOMF_CYCLE_get    ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N )
 | void   sts_sw_field_LO_FIRST_CYCLE_set   ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sts_sw_field_LO_FIRST_CYCLE_get   ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N )
 | void   sts_sw_field_CHAN_ID_set          ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sts_sw_field_CHAN_ID_get          ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N )
 | void   sts_sw_field_range_CHAN_ID_set    ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sts_sw_field_range_CHAN_ID_get    ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _STS_SW_IO_H
#define _STS_SW_IO_H

#include "sts_sw.h"
#include "sts_sw_regs.h"
#include "sts_sw_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sts_sw_buffer_init               ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr )
 | void                 sts_sw_buffer_flush              ( sts_sw_buffer_t *b_ptr )
 | UINT32               sts_sw_reg_read                  ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 sts_sw_reg_write                 ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 sts_sw_field_set                 ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 sts_sw_action_on_write_field_set ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 sts_sw_burst_read                ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 sts_sw_burst_write               ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE sts_sw_poll                      ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   sts_sw_reg_CONFIG_write                        ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32 value )
 | void   sts_sw_reg_CONFIG_field_set                    ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sts_sw_reg_CONFIG_read                         ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr )
 | 
 | void   sts_sw_reg_IPT_array_write                     ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   sts_sw_reg_IPT_array_field_set                 ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sts_sw_reg_IPT_array_read                      ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N )
 | 
 | void   sts_sw_reg_HO_CFG_array_write                  ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   sts_sw_reg_HO_CFG_array_field_set              ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sts_sw_reg_HO_CFG_array_read                   ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N )
 | 
 | void   sts_sw_reg_HO_CYCLE_CFG_array_write            ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   sts_sw_reg_HO_CYCLE_CFG_array_field_set        ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sts_sw_reg_HO_CYCLE_CFG_array_read             ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N )
 | 
 | void   sts_sw_reg_CALENDAR_CHANNEL_ID_array_write     ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   sts_sw_reg_CALENDAR_CHANNEL_ID_array_field_set ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sts_sw_reg_CALENDAR_CHANNEL_ID_array_read      ( sts_sw_buffer_t *b_ptr, sts_sw_handle_t *h_ptr, UINT32  N )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _STS_SW_IO_H */
