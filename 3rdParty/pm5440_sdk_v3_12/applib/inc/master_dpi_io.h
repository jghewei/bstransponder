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
 *     and register accessor functions for the master_dpi block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing master_dpi io functions is:
 | {
 |     master_dpi_buffer_t b_ptr[1];
 |     master_dpi_buffer_init( b_ptr, h_ptr, A );
 |     value = master_dpi_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     master_dpi_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     master_dpi_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = master_dpi_field_<FIELD>_get( NULL, h_ptr, A );
 | master_dpi_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------------+
 | | Register 0x00020000 ZONE_CONFIG |
 | +---------------------------------+
 | | bit  17 R/W  ZONE_EN            |
 | +---------------------------------+
 | 
 | void   master_dpi_field_ZONE_EN_set ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 master_dpi_field_ZONE_EN_get ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------------------+
 | | Register (0x00020400 + (N) * 0x4) CH_CONFIG |
 | |     N[0..255]                               |
 | +---------------------------------------------+
 | | bit  16  R/W  EOP_HOLE_SIZE                 |
 | | bit  6:0 R/W  SOP_HOLE_SIZE                 |
 | +---------------------------------------------+
 | 
 | void   master_dpi_field_EOP_HOLE_SIZE_set       ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 master_dpi_field_EOP_HOLE_SIZE_get       ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   master_dpi_field_SOP_HOLE_SIZE_set       ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 master_dpi_field_SOP_HOLE_SIZE_get       ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   master_dpi_field_range_SOP_HOLE_SIZE_set ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 master_dpi_field_range_SOP_HOLE_SIZE_get ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register (0x00020800 + (N) * 0x4) ZONE_0_1 |
 | |     N[0..255]                              |
 | +--------------------------------------------+
 | | bit  16:0 R/W  ZONE_THR_0_1                |
 | +--------------------------------------------+
 | 
 | void   master_dpi_field_ZONE_THR_0_1_set       ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 master_dpi_field_ZONE_THR_0_1_get       ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   master_dpi_field_range_ZONE_THR_0_1_set ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 master_dpi_field_range_ZONE_THR_0_1_get ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register (0x00020c00 + (N) * 0x4) ZONE_1_2 |
 | |     N[0..255]                              |
 | +--------------------------------------------+
 | | bit  16:0 R/W  ZONE_THR_1_2                |
 | +--------------------------------------------+
 | 
 | void   master_dpi_field_ZONE_THR_1_2_set       ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 master_dpi_field_ZONE_THR_1_2_get       ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   master_dpi_field_range_ZONE_THR_1_2_set ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 master_dpi_field_range_ZONE_THR_1_2_get ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register (0x00021000 + (N) * 0x4) ZONE_2_3 |
 | |     N[0..255]                              |
 | +--------------------------------------------+
 | | bit  16:0 R/W  ZONE_THR_2_3                |
 | +--------------------------------------------+
 | 
 | void   master_dpi_field_ZONE_THR_2_3_set       ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 master_dpi_field_ZONE_THR_2_3_get       ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   master_dpi_field_range_ZONE_THR_2_3_set ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 master_dpi_field_range_ZONE_THR_2_3_get ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x00021c00 + (N) * 0x4) ZONE_OVRD_CONFIG |
 | |     N[0..255]                                      |
 | +----------------------------------------------------+
 | | bit  25 R/W  ZONE_OVRD_EN                          |
 | | bit  24 R/W  ZONE_OVRD_CFG                         |
 | +----------------------------------------------------+
 | 
 | void   master_dpi_field_ZONE_OVRD_EN_set  ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 master_dpi_field_ZONE_OVRD_EN_get  ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   master_dpi_field_ZONE_OVRD_CFG_set ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 master_dpi_field_ZONE_OVRD_CFG_get ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _MASTER_DPI_IO_H
#define _MASTER_DPI_IO_H

#include "cpb_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "master_dpi_regs.h"
#include "master_dpi_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 master_dpi_buffer_init               ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void                 master_dpi_buffer_flush              ( master_dpi_buffer_t *b_ptr )
 | UINT32               master_dpi_reg_read                  ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 master_dpi_reg_write                 ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 master_dpi_field_set                 ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 master_dpi_action_on_write_field_set ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 master_dpi_burst_read                ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 master_dpi_burst_write               ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE master_dpi_poll                      ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   master_dpi_reg_ZONE_CONFIG_write                ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   master_dpi_reg_ZONE_CONFIG_field_set            ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 master_dpi_reg_ZONE_CONFIG_read                 ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | void   master_dpi_reg_CH_CONFIG_array_write            ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   master_dpi_reg_CH_CONFIG_array_field_set        ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 master_dpi_reg_CH_CONFIG_array_read             ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   master_dpi_reg_ZONE_0_1_array_write             ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   master_dpi_reg_ZONE_0_1_array_field_set         ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 master_dpi_reg_ZONE_0_1_array_read              ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   master_dpi_reg_ZONE_1_2_array_write             ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   master_dpi_reg_ZONE_1_2_array_field_set         ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 master_dpi_reg_ZONE_1_2_array_read              ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   master_dpi_reg_ZONE_2_3_array_write             ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   master_dpi_reg_ZONE_2_3_array_field_set         ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 master_dpi_reg_ZONE_2_3_array_read              ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   master_dpi_reg_ZONE_OVRD_CONFIG_array_write     ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   master_dpi_reg_ZONE_OVRD_CONFIG_array_field_set ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 master_dpi_reg_ZONE_OVRD_CONFIG_array_read      ( master_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _MASTER_DPI_IO_H */
