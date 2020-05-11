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
 *     and register accessor functions for the rxjc_proc block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing rxjc_proc io functions is:
 | {
 |     rxjc_proc_buffer_t b_ptr[1];
 |     rxjc_proc_buffer_init( b_ptr, h_ptr );
 |     value = rxjc_proc_field_<FIELD>_get( b_ptr, h_ptr );
 |     rxjc_proc_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     rxjc_proc_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = rxjc_proc_field_<FIELD>_get( NULL, h_ptr );
 | rxjc_proc_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +--------------------------------------------+
 | | Register 0x00000000 CM_STAT_UPDATE_DISABLE |
 | +--------------------------------------------+
 | | bit  0 R/W  CM_ST_UPDATE_DISBLE            |
 | +--------------------------------------------+
 | 
 | void   rxjc_proc_field_CM_ST_UPDATE_DISBLE_set ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32 value )
 | UINT32 rxjc_proc_field_CM_ST_UPDATE_DISBLE_get ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr )
 | 
 | +----------------------------------------+
 | | Register 0x00000004 PMON_SYNCLOSS_CHID |
 | +----------------------------------------+
 | | bit  6:0 R/W  PMON_SYNCLOSS_CHID       |
 | +----------------------------------------+
 | 
 | void   rxjc_proc_field_PMON_SYNCLOSS_CHID_set       ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32 value )
 | UINT32 rxjc_proc_field_PMON_SYNCLOSS_CHID_get       ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr )
 | void   rxjc_proc_field_range_PMON_SYNCLOSS_CHID_set ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rxjc_proc_field_range_PMON_SYNCLOSS_CHID_get ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000008 PMON_SYNCLOSS_CNT |
 | +---------------------------------------+
 | | bit  31:0 R  PMON_SYNCLOSS_CNT        |
 | +---------------------------------------+
 | 
 | UINT32 rxjc_proc_field_PMON_SYNCLOSS_CNT_get       ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr )
 | UINT32 rxjc_proc_field_range_PMON_SYNCLOSS_CNT_get ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00000800 + (N) * 0x10) COMMON_CFG_1 |
 | |     N[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  29:16 R/W  CM_MAX                          |
 | | bit  13:0  R/W  CM_MIN                          |
 | +-------------------------------------------------+
 | 
 | void   rxjc_proc_field_CM_MAX_set       ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 rxjc_proc_field_CM_MAX_get       ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N )
 | void   rxjc_proc_field_range_CM_MAX_set ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rxjc_proc_field_range_CM_MAX_get ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   rxjc_proc_field_CM_MIN_set       ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 rxjc_proc_field_CM_MIN_get       ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N )
 | void   rxjc_proc_field_range_CM_MIN_set ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rxjc_proc_field_range_CM_MIN_get ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00000804 + (N) * 0x10) COMMON_CFG_2 |
 | |     N[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  22:16 R/W  M                               |
 | | bit  9:0   R/W  CND_MAX                         |
 | +-------------------------------------------------+
 | 
 | void   rxjc_proc_field_M_set             ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 rxjc_proc_field_M_get             ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N )
 | void   rxjc_proc_field_range_M_set       ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rxjc_proc_field_range_M_get       ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   rxjc_proc_field_CND_MAX_set       ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 rxjc_proc_field_CND_MAX_get       ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N )
 | void   rxjc_proc_field_range_CND_MAX_set ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rxjc_proc_field_range_CND_MAX_get ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00000808 + (N) * 0x10) COMMON_CFG_3 |
 | |     N[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  25:24 R/W  NJO_PJO_MODE                    |
 | | bit  20:0  R/W  SERVER_SIZE                     |
 | +-------------------------------------------------+
 | 
 | void   rxjc_proc_field_NJO_PJO_MODE_set       ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 rxjc_proc_field_NJO_PJO_MODE_get       ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N )
 | void   rxjc_proc_field_range_NJO_PJO_MODE_set ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rxjc_proc_field_range_NJO_PJO_MODE_get ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   rxjc_proc_field_SERVER_SIZE_set        ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 rxjc_proc_field_SERVER_SIZE_get        ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N )
 | void   rxjc_proc_field_range_SERVER_SIZE_set  ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rxjc_proc_field_range_SERVER_SIZE_get  ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x00000e00 + (N) * 0x4) CM_STAT_REG |
 | |     N[0..95]                                  |
 | +-----------------------------------------------+
 | | bit  13:0 R  CM_STATUS                        |
 | +-----------------------------------------------+
 | 
 | UINT32 rxjc_proc_field_CM_STATUS_get       ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N )
 | UINT32 rxjc_proc_field_range_CM_STATUS_get ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _RXJC_PROC_IO_H
#define _RXJC_PROC_IO_H

#include "rxjc_proc_loc.h"
#include "rxjc_proc_regs.h"
#include "rxjc_proc_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 rxjc_proc_buffer_init               ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr )
 | void                 rxjc_proc_buffer_flush              ( rxjc_proc_buffer_t *b_ptr )
 | UINT32               rxjc_proc_reg_read                  ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 rxjc_proc_reg_write                 ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 rxjc_proc_field_set                 ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 rxjc_proc_action_on_write_field_set ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rxjc_proc_burst_read                ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 rxjc_proc_burst_write               ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE rxjc_proc_poll                      ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   rxjc_proc_reg_CM_STAT_UPDATE_DISABLE_write     ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32 value )
 | void   rxjc_proc_reg_CM_STAT_UPDATE_DISABLE_field_set ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 rxjc_proc_reg_CM_STAT_UPDATE_DISABLE_read      ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr )
 | 
 | void   rxjc_proc_reg_PMON_SYNCLOSS_CHID_write         ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32 value )
 | void   rxjc_proc_reg_PMON_SYNCLOSS_CHID_field_set     ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 rxjc_proc_reg_PMON_SYNCLOSS_CHID_read          ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr )
 | 
 | void   rxjc_proc_reg_COMMON_CFG_1_array_write         ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   rxjc_proc_reg_COMMON_CFG_1_array_field_set     ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 rxjc_proc_reg_COMMON_CFG_1_array_read          ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N )
 | 
 | void   rxjc_proc_reg_COMMON_CFG_2_array_write         ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   rxjc_proc_reg_COMMON_CFG_2_array_field_set     ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 rxjc_proc_reg_COMMON_CFG_2_array_read          ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N )
 | 
 | void   rxjc_proc_reg_COMMON_CFG_3_array_write         ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   rxjc_proc_reg_COMMON_CFG_3_array_field_set     ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 rxjc_proc_reg_COMMON_CFG_3_array_read          ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32 rxjc_proc_reg_CM_STAT_REG_array_read           ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32 rxjc_proc_reg_PMON_SYNCLOSS_CNT_read           ( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _RXJC_PROC_IO_H */
