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
 *     and register accessor functions for the lineotn block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing lineotn io functions is:
 | {
 |     lineotn_buffer_t b_ptr[1];
 |     lineotn_buffer_init( b_ptr, h_ptr );
 |     value = lineotn_field_<FIELD>_get( b_ptr, h_ptr );
 |     lineotn_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     lineotn_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = lineotn_field_<FIELD>_get( NULL, h_ptr );
 | lineotn_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------+
 | | Register 0x00000000 MASTER_CONFIG |
 | +-----------------------------------+
 | | bit  2 R/W  PCBI_LCLK_MASK        |
 | +-----------------------------------+
 | 
 | void   lineotn_field_PCBI_LCLK_MASK_set ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 lineotn_field_PCBI_LCLK_MASK_get ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +-----------------------------+
 | | Register 0x00000008 INT0_EN |
 | +-----------------------------+
 | | bit  16:0 R/W  INT0_E       |
 | +-----------------------------+
 | 
 | void   lineotn_field_INT0_E_set       ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 lineotn_field_INT0_E_get       ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   lineotn_field_range_INT0_E_set ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 lineotn_field_range_INT0_E_get ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x0000000c INT0_SUMMARY |
 | +----------------------------------+
 | | bit  16:0 R  INT0_SUMMARY        |
 | +----------------------------------+
 | 
 | UINT32 lineotn_field_INT0_SUMMARY_get       ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | UINT32 lineotn_field_range_INT0_SUMMARY_get ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------+
 | | Register 0x00000010 INT1_EN |
 | +-----------------------------+
 | | bit  16:0 R/W  INT1_E       |
 | +-----------------------------+
 | 
 | void   lineotn_field_INT1_E_set       ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 lineotn_field_INT1_E_get       ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   lineotn_field_range_INT1_E_set ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 lineotn_field_range_INT1_E_get ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00000014 INT1_SUMMARY |
 | +----------------------------------+
 | | bit  16:0 R  INT1_SUMMARY        |
 | +----------------------------------+
 | 
 | UINT32 lineotn_field_INT1_SUMMARY_get       ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | UINT32 lineotn_field_range_INT1_SUMMARY_get ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------+
 | | Register 0x00000018 INT2_EN |
 | +-----------------------------+
 | | bit  16:0 R/W  INT2_E       |
 | +-----------------------------+
 | 
 | void   lineotn_field_INT2_E_set       ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 lineotn_field_INT2_E_get       ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   lineotn_field_range_INT2_E_set ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 lineotn_field_range_INT2_E_get ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x0000001c INT2_SUMMARY |
 | +----------------------------------+
 | | bit  16:0 R  INT2_SUMMARY        |
 | +----------------------------------+
 | 
 | UINT32 lineotn_field_INT2_SUMMARY_get       ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | UINT32 lineotn_field_range_INT2_SUMMARY_get ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------+
 | | Register 0x00000020 INT3_EN |
 | +-----------------------------+
 | | bit  16:0 R/W  INT3_E       |
 | +-----------------------------+
 | 
 | void   lineotn_field_INT3_E_set       ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 lineotn_field_INT3_E_get       ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   lineotn_field_range_INT3_E_set ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 lineotn_field_range_INT3_E_get ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00000024 INT3_SUMMARY |
 | +----------------------------------+
 | | bit  16:0 R  INT3_SUMMARY        |
 | +----------------------------------+
 | 
 | UINT32 lineotn_field_INT3_SUMMARY_get       ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | UINT32 lineotn_field_range_INT3_SUMMARY_get ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------+
 | | Register 0x00000028 TIP_STAT |
 | +------------------------------+
 | | bit  2 R  OTN_OTU_TIP        |
 | | bit  1 R  MFEC_TIP           |
 | +------------------------------+
 | 
 | UINT32               lineotn_field_OTN_OTU_TIP_get  ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE lineotn_field_OTN_OTU_TIP_poll ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               lineotn_field_MFEC_TIP_get     ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE lineotn_field_MFEC_TIP_poll    ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _LINEOTN_IO_H
#define _LINEOTN_IO_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "lineotn_regs.h"
#include "lineotn_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 lineotn_buffer_init               ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void                 lineotn_buffer_flush              ( lineotn_buffer_t *b_ptr )
 | UINT32               lineotn_reg_read                  ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 lineotn_reg_write                 ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 lineotn_field_set                 ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 lineotn_action_on_write_field_set ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 lineotn_burst_read                ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 lineotn_burst_write               ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE lineotn_poll                      ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 lineotn_reg_MASTER_CONFIG_write     ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 lineotn_reg_MASTER_CONFIG_field_set ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               lineotn_reg_MASTER_CONFIG_read      ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 lineotn_reg_INT0_EN_write           ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 lineotn_reg_INT0_EN_field_set       ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               lineotn_reg_INT0_EN_read            ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 lineotn_reg_INT1_EN_write           ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 lineotn_reg_INT1_EN_field_set       ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               lineotn_reg_INT1_EN_read            ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 lineotn_reg_INT2_EN_write           ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 lineotn_reg_INT2_EN_field_set       ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               lineotn_reg_INT2_EN_read            ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 lineotn_reg_INT3_EN_write           ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 lineotn_reg_INT3_EN_field_set       ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               lineotn_reg_INT3_EN_read            ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | UINT32               lineotn_reg_INT0_SUMMARY_read       ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | UINT32               lineotn_reg_INT1_SUMMARY_read       ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | UINT32               lineotn_reg_INT2_SUMMARY_read       ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | UINT32               lineotn_reg_INT3_SUMMARY_read       ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | UINT32               lineotn_reg_TIP_STAT_read           ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE lineotn_reg_TIP_STAT_poll           ( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _LINEOTN_IO_H */
