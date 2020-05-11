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
 *     and register accessor functions for the emac_core block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing emac_core io functions is:
 | {
 |     emac_core_buffer_t b_ptr[1];
 |     emac_core_buffer_init( b_ptr, h_ptr );
 |     value = emac_core_field_<FIELD>_get( b_ptr, h_ptr );
 |     emac_core_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     emac_core_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = emac_core_field_<FIELD>_get( NULL, h_ptr );
 | emac_core_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------------+
 | | Register 0x0000b000 SEG_ACTIVATION_CTRL |
 | +-----------------------------------------+
 | | bit  11:0 R/W  SEG_CTL                  |
 | +-----------------------------------------+
 | 
 | void   emac_core_field_SEG_CTL_set       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_core_field_SEG_CTL_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_core_field_range_SEG_CTL_set ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_core_field_range_SEG_CTL_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x0000b004 SEG_MODE_CTRL |
 | +-----------------------------------+
 | | bit  23:0 R/W  MODE_CTL           |
 | +-----------------------------------+
 | 
 | void   emac_core_field_MODE_CTL_set       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_core_field_MODE_CTL_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_core_field_range_MODE_CTL_set ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_core_field_range_MODE_CTL_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x0000b008 TX_CLK_SELECTION_CTRL |
 | +-------------------------------------------+
 | | bit  23:0 R  TXCLK_CTL                    |
 | +-------------------------------------------+
 | 
 | UINT32 emac_core_field_TXCLK_CTL_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | UINT32 emac_core_field_range_TXCLK_CTL_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x0000b00c USER_DEFINED_TX_PREAMBLE |
 | +----------------------------------------------+
 | | bit  23:0 R/W  TX_PRMBL_CTL                  |
 | +----------------------------------------------+
 | 
 | void   emac_core_field_TX_PRMBL_CTL_set       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_core_field_TX_PRMBL_CTL_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_core_field_range_TX_PRMBL_CTL_set ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_core_field_range_TX_PRMBL_CTL_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x0000b020 BASE_R_FAULT_STATEMACHINE_CTRL |
 | +----------------------------------------------------+
 | | bit  15:0 R/W  WAN_RS_COL_CNT                      |
 | +----------------------------------------------------+
 | 
 | void   emac_core_field_WAN_RS_COL_CNT_set       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_core_field_WAN_RS_COL_CNT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_core_field_range_WAN_RS_COL_CNT_set ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_core_field_range_WAN_RS_COL_CNT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x0000b024 TDM_CREDIT_INITIALIZATION_1_3_ |
 | +----------------------------------------------------+
 | | bit  31:24 R/W  SEG3_INIT_CREDIT                   |
 | | bit  23:16 R/W  SEG2_INIT_CREDIT                   |
 | | bit  15:8  R/W  SEG1_INIT_CREDIT                   |
 | | bit  7:0   R/W  SEG0_INIT_CREDIT                   |
 | +----------------------------------------------------+
 | 
 | void   emac_core_field_SEG3_INIT_CREDIT_set       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_core_field_SEG3_INIT_CREDIT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_core_field_range_SEG3_INIT_CREDIT_set ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_core_field_range_SEG3_INIT_CREDIT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_core_field_SEG2_INIT_CREDIT_set       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_core_field_SEG2_INIT_CREDIT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_core_field_range_SEG2_INIT_CREDIT_set ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_core_field_range_SEG2_INIT_CREDIT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_core_field_SEG1_INIT_CREDIT_set       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_core_field_SEG1_INIT_CREDIT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_core_field_range_SEG1_INIT_CREDIT_set ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_core_field_range_SEG1_INIT_CREDIT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_core_field_SEG0_INIT_CREDIT_set       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_core_field_SEG0_INIT_CREDIT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_core_field_range_SEG0_INIT_CREDIT_set ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_core_field_range_SEG0_INIT_CREDIT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x0000b028 TDM_CREDIT_INITIALIZATION_2_3_ |
 | +----------------------------------------------------+
 | | bit  31:24 R/W  SEG7_INIT_CREDIT                   |
 | | bit  23:16 R/W  SEG6_INIT_CREDIT                   |
 | | bit  15:8  R/W  SEG5_INIT_CREDIT                   |
 | | bit  7:0   R/W  SEG4_INIT_CREDIT                   |
 | +----------------------------------------------------+
 | 
 | void   emac_core_field_SEG7_INIT_CREDIT_set       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_core_field_SEG7_INIT_CREDIT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_core_field_range_SEG7_INIT_CREDIT_set ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_core_field_range_SEG7_INIT_CREDIT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_core_field_SEG6_INIT_CREDIT_set       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_core_field_SEG6_INIT_CREDIT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_core_field_range_SEG6_INIT_CREDIT_set ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_core_field_range_SEG6_INIT_CREDIT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_core_field_SEG5_INIT_CREDIT_set       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_core_field_SEG5_INIT_CREDIT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_core_field_range_SEG5_INIT_CREDIT_set ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_core_field_range_SEG5_INIT_CREDIT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_core_field_SEG4_INIT_CREDIT_set       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_core_field_SEG4_INIT_CREDIT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_core_field_range_SEG4_INIT_CREDIT_set ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_core_field_range_SEG4_INIT_CREDIT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x0000b02c TDM_CREDIT_INITIALIZATION_3_3_ |
 | +----------------------------------------------------+
 | | bit  31:24 R/W  SEG11_INIT_CREDIT                  |
 | | bit  23:16 R/W  SEG10_INIT_CREDIT                  |
 | | bit  15:8  R/W  SEG9_INIT_CREDIT                   |
 | | bit  7:0   R/W  SEG8_INIT_CREDIT                   |
 | +----------------------------------------------------+
 | 
 | void   emac_core_field_SEG11_INIT_CREDIT_set       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_core_field_SEG11_INIT_CREDIT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_core_field_range_SEG11_INIT_CREDIT_set ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_core_field_range_SEG11_INIT_CREDIT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_core_field_SEG10_INIT_CREDIT_set       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_core_field_SEG10_INIT_CREDIT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_core_field_range_SEG10_INIT_CREDIT_set ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_core_field_range_SEG10_INIT_CREDIT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_core_field_SEG9_INIT_CREDIT_set        ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_core_field_SEG9_INIT_CREDIT_get        ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_core_field_range_SEG9_INIT_CREDIT_set  ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_core_field_range_SEG9_INIT_CREDIT_get  ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_core_field_SEG8_INIT_CREDIT_set        ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_core_field_SEG8_INIT_CREDIT_get        ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_core_field_range_SEG8_INIT_CREDIT_set  ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_core_field_range_SEG8_INIT_CREDIT_get  ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0000b030 TDM_CREDIT_STAT_1_3_ |
 | +------------------------------------------+
 | | bit  31:24 R  SEG3_REG_CREDIT            |
 | | bit  23:16 R  SEG2_REG_CREDIT            |
 | | bit  15:8  R  SEG1_REG_CREDIT            |
 | | bit  7:0   R  SEG0_REG_CREDIT            |
 | +------------------------------------------+
 | 
 | UINT32 emac_core_field_SEG3_REG_CREDIT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | UINT32 emac_core_field_range_SEG3_REG_CREDIT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 emac_core_field_SEG2_REG_CREDIT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | UINT32 emac_core_field_range_SEG2_REG_CREDIT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 emac_core_field_SEG1_REG_CREDIT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | UINT32 emac_core_field_range_SEG1_REG_CREDIT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 emac_core_field_SEG0_REG_CREDIT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | UINT32 emac_core_field_range_SEG0_REG_CREDIT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0000b034 TDM_CREDIT_STAT_2_3_ |
 | +------------------------------------------+
 | | bit  31:24 R  SEG7_REG_CREDIT            |
 | | bit  23:16 R  SEG6_REG_CREDIT            |
 | | bit  15:8  R  SEG5_REG_CREDIT            |
 | | bit  7:0   R  SEG4_REG_CREDIT            |
 | +------------------------------------------+
 | 
 | UINT32 emac_core_field_SEG7_REG_CREDIT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | UINT32 emac_core_field_range_SEG7_REG_CREDIT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 emac_core_field_SEG6_REG_CREDIT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | UINT32 emac_core_field_range_SEG6_REG_CREDIT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 emac_core_field_SEG5_REG_CREDIT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | UINT32 emac_core_field_range_SEG5_REG_CREDIT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 emac_core_field_SEG4_REG_CREDIT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | UINT32 emac_core_field_range_SEG4_REG_CREDIT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0000b038 TDM_CREDIT_STAT_3_3_ |
 | +------------------------------------------+
 | | bit  31:24 R  SEG11_REG_CREDIT           |
 | | bit  23:16 R  SEG10_REG_CREDIT           |
 | | bit  15:8  R  SEG9_REG_CREDIT            |
 | | bit  7:0   R  SEG8_REG_CREDIT            |
 | +------------------------------------------+
 | 
 | UINT32 emac_core_field_SEG11_REG_CREDIT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | UINT32 emac_core_field_range_SEG11_REG_CREDIT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 emac_core_field_SEG10_REG_CREDIT_get       ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | UINT32 emac_core_field_range_SEG10_REG_CREDIT_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 emac_core_field_SEG9_REG_CREDIT_get        ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | UINT32 emac_core_field_range_SEG9_REG_CREDIT_get  ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 emac_core_field_SEG8_REG_CREDIT_get        ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | UINT32 emac_core_field_range_SEG8_REG_CREDIT_get  ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register 0x0000b040 VIRTUAL_LANE_MARKER_INTERVAL_CTRL |
 | +-------------------------------------------------------+
 | | bit  0 R/W  VL_INTVL                                  |
 | +-------------------------------------------------------+
 | 
 | void   emac_core_field_VL_INTVL_set ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_core_field_VL_INTVL_get ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _EMAC_CORE_IO_H
#define _EMAC_CORE_IO_H

#include "emac_loc.h"
#include "emac_core_regs.h"
#include "emac_core_io_inline.h"


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 emac_core_buffer_init               ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void                 emac_core_buffer_flush              ( emac_core_buffer_t *b_ptr )
 | UINT32               emac_core_reg_read                  ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 emac_core_reg_write                 ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 emac_core_field_set                 ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 emac_core_action_on_write_field_set ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 emac_core_burst_read                ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 emac_core_burst_write               ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE emac_core_poll                      ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   emac_core_reg_SEG_ACTIVATION_CTRL_write                   ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void   emac_core_reg_SEG_ACTIVATION_CTRL_field_set               ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_core_reg_SEG_ACTIVATION_CTRL_read                    ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void   emac_core_reg_SEG_MODE_CTRL_write                         ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void   emac_core_reg_SEG_MODE_CTRL_field_set                     ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_core_reg_SEG_MODE_CTRL_read                          ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void   emac_core_reg_USER_DEFINED_TX_PREAMBLE_write              ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void   emac_core_reg_USER_DEFINED_TX_PREAMBLE_field_set          ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_core_reg_USER_DEFINED_TX_PREAMBLE_read               ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void   emac_core_reg_BASE_R_FAULT_STATEMACHINE_CTRL_write        ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void   emac_core_reg_BASE_R_FAULT_STATEMACHINE_CTRL_field_set    ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_core_reg_BASE_R_FAULT_STATEMACHINE_CTRL_read         ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void   emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__write        ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void   emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__field_set    ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__read         ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void   emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__write        ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void   emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__field_set    ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__read         ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void   emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__write        ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void   emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__field_set    ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__read         ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void   emac_core_reg_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_write     ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void   emac_core_reg_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_field_set ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_core_reg_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_read      ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | UINT32 emac_core_reg_TX_CLK_SELECTION_CTRL_read                  ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | UINT32 emac_core_reg_TDM_CREDIT_STAT_1_3__read                   ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | UINT32 emac_core_reg_TDM_CREDIT_STAT_2_3__read                   ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | UINT32 emac_core_reg_TDM_CREDIT_STAT_3_3__read                   ( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 +------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _EMAC_CORE_IO_H */
