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
 *     and register accessor functions for the sysotn block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing sysotn io functions is:
 | {
 |     sysotn_buffer_t b_ptr[1];
 |     sysotn_buffer_init( b_ptr, h_ptr );
 |     value = sysotn_field_<FIELD>_get( b_ptr, h_ptr );
 |     sysotn_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     sysotn_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = sysotn_field_<FIELD>_get( NULL, h_ptr );
 | sysotn_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------+
 | | Register 0x00000000 MASTER_CONFIG |
 | +-----------------------------------+
 | | bit  10 R/W  FEC_FAIL_DLOM_ENABLE |
 | | bit  9  R/W  BYPASS_GFEC_TX       |
 | | bit  8  R/W  BYPASS_GFEC_RX       |
 | | bit  7  R/W  OTL_MODE             |
 | | bit  6  R/W  OTN_OTU_LOWPWR       |
 | | bit  5  R/W  SFIS_LOWPWR          |
 | | bit  4  R/W  OPSM4_LOWPWR         |
 | | bit  3  R/W  GFEC_234_LOWPWR      |
 | | bit  2  R/W  PCBI_LCLK_MASK       |
 | +-----------------------------------+
 | 
 | void   sysotn_field_FEC_FAIL_DLOM_ENABLE_set ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_field_FEC_FAIL_DLOM_ENABLE_get ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_field_BYPASS_GFEC_TX_set       ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_field_BYPASS_GFEC_TX_get       ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_field_BYPASS_GFEC_RX_set       ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_field_BYPASS_GFEC_RX_get       ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_field_OTL_MODE_set             ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_field_OTL_MODE_get             ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_field_OTN_OTU_LOWPWR_set       ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_field_OTN_OTU_LOWPWR_get       ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_field_SFIS_LOWPWR_set          ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_field_SFIS_LOWPWR_get          ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_field_OPSM4_LOWPWR_set         ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_field_OPSM4_LOWPWR_get         ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_field_GFEC_234_LOWPWR_set      ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_field_GFEC_234_LOWPWR_get      ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_field_PCBI_LCLK_MASK_set       ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_field_PCBI_LCLK_MASK_get       ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +--------------------------------+
 | | Register 0x00000004 TOP_INT_EN |
 | +--------------------------------+
 | | bit  31:0 R/W  TOP_INT_E       |
 | +--------------------------------+
 | 
 | void   sysotn_field_TOP_INT_E_set       ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_field_TOP_INT_E_get       ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_field_range_TOP_INT_E_set ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_field_range_TOP_INT_E_get ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000010 FEC_STAT_UPDATE |
 | +-------------------------------------+
 | | bit  1 R/W  RS_FEC_STATS_UPDATE     |
 | +-------------------------------------+
 | 
 | void   sysotn_field_RS_FEC_STATS_UPDATE_set ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_field_RS_FEC_STATS_UPDATE_get ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 +---------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _SYSOTN_IO_H
#define _SYSOTN_IO_H

#include "lineotn_loc.h"
#include "sysotn_regs.h"
#include "sysotn_io_inline.h"


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sysotn_buffer_init               ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void                 sysotn_buffer_flush              ( sysotn_buffer_t *b_ptr )
 | UINT32               sysotn_reg_read                  ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 sysotn_reg_write                 ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 sysotn_field_set                 ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 sysotn_action_on_write_field_set ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 sysotn_burst_read                ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 sysotn_burst_write               ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE sysotn_poll                      ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------+
 | void   sysotn_reg_MASTER_CONFIG_write       ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void   sysotn_reg_MASTER_CONFIG_field_set   ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sysotn_reg_MASTER_CONFIG_read        ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void   sysotn_reg_TOP_INT_EN_write          ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void   sysotn_reg_TOP_INT_EN_field_set      ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sysotn_reg_TOP_INT_EN_read           ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void   sysotn_reg_FEC_STAT_UPDATE_write     ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void   sysotn_reg_FEC_STAT_UPDATE_field_set ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sysotn_reg_FEC_STAT_UPDATE_read      ( sysotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _SYSOTN_IO_H */
