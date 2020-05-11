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
 *     and register accessor functions for the d8_rx_2x_cdru block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing d8_rx_2x_cdru io functions is:
 | {
 |     d8_rx_2x_cdru_buffer_t b_ptr[1];
 |     d8_rx_2x_cdru_buffer_init( b_ptr, h_ptr, A );
 |     value = d8_rx_2x_cdru_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     d8_rx_2x_cdru_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     d8_rx_2x_cdru_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = d8_rx_2x_cdru_field_<FIELD>_get( NULL, h_ptr, A );
 | d8_rx_2x_cdru_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------------------+
 | | Register 0x00000200 DELTA8_CDRU_CFG_1 |
 | +---------------------------------------+
 | | bit  18 R/W  FILT_EN                  |
 | | bit  17 R/W  IPD_EN                   |
 | | bit  16 R/W  INV_REC_DATA             |
 | | bit  1  R/W  TED_SEL                  |
 | +---------------------------------------+
 | 
 | void   d8_rx_2x_cdru_field_FILT_EN_set      ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 d8_rx_2x_cdru_field_FILT_EN_get      ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A )
 | void   d8_rx_2x_cdru_field_IPD_EN_set       ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 d8_rx_2x_cdru_field_IPD_EN_get       ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A )
 | void   d8_rx_2x_cdru_field_INV_REC_DATA_set ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 d8_rx_2x_cdru_field_INV_REC_DATA_get ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A )
 | void   d8_rx_2x_cdru_field_TED_SEL_set      ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 d8_rx_2x_cdru_field_TED_SEL_get      ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------------+
 | | Register 0x0000020c DELTA8_CDRU_CFG_4 |
 | +---------------------------------------+
 | | bit  30:21 R/W  LOS_CNT               |
 | | bit  20    R/W  LOS_ENABLE            |
 | +---------------------------------------+
 | 
 | void   d8_rx_2x_cdru_field_LOS_CNT_set       ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 d8_rx_2x_cdru_field_LOS_CNT_get       ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A )
 | void   d8_rx_2x_cdru_field_range_LOS_CNT_set ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_cdru_field_range_LOS_CNT_get ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_cdru_field_LOS_ENABLE_set    ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 d8_rx_2x_cdru_field_LOS_ENABLE_get    ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _D8_RX_2X_CDRU_IO_H
#define _D8_RX_2X_CDRU_IO_H

#include "d8_rx_2x.h"
#include "d8_rx_2x_cdru_regs.h"
#include "d8_rx_2x_cdru_io_inline.h"


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 d8_rx_2x_cdru_buffer_init               ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A )
 | void                 d8_rx_2x_cdru_buffer_flush              ( d8_rx_2x_cdru_buffer_t *b_ptr )
 | UINT32               d8_rx_2x_cdru_reg_read                  ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 d8_rx_2x_cdru_reg_write                 ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 d8_rx_2x_cdru_field_set                 ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 d8_rx_2x_cdru_action_on_write_field_set ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 d8_rx_2x_cdru_burst_read                ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 d8_rx_2x_cdru_burst_write               ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_cdru_poll                      ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_1_write     ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_1_field_set ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_1_read      ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A )
 | 
 | void   d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_4_write     ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_4_field_set ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 d8_rx_2x_cdru_reg_DELTA8_CDRU_CFG_4_read      ( d8_rx_2x_cdru_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 A )
 | 
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _D8_RX_2X_CDRU_IO_H */
