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
 *     and register accessor functions for the cpb_ctrl block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing cpb_ctrl io functions is:
 | {
 |     cpb_ctrl_buffer_t b_ptr[1];
 |     cpb_ctrl_buffer_init( b_ptr, h_ptr );
 |     value = cpb_ctrl_field_<FIELD>_get( b_ptr, h_ptr );
 |     cpb_ctrl_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     cpb_ctrl_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = cpb_ctrl_field_<FIELD>_get( NULL, h_ptr );
 | cpb_ctrl_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-------------------------------+
 | | Register 0x00040110 INTSUM    |
 | +-------------------------------+
 | | bit  23:16 R  SLV_DPI_INT_SUM |
 | +-------------------------------+
 | 
 | UINT32 cpb_ctrl_field_SLV_DPI_INT_SUM_get       ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | UINT32 cpb_ctrl_field_range_SLV_DPI_INT_SUM_get ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x00040180 + (N) * 0x4) MASK_DCS_0_REG |
 | |     N[0..7]                                      |
 | +--------------------------------------------------+
 | | bit  31:0 R/W  XOFF_MASK_DCS_0                   |
 | +--------------------------------------------------+
 | 
 | void   cpb_ctrl_lfield_XOFF_MASK_DCS_0_set       ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value[8] )
 | void   cpb_ctrl_lfield_XOFF_MASK_DCS_0_get       ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value[8] )
 | void   cpb_ctrl_lfield_range_XOFF_MASK_DCS_0_set ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpb_ctrl_lfield_range_XOFF_MASK_DCS_0_get ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x000401a0 + (N) * 0x4) MASK_DCS_1_REG |
 | |     N[0..7]                                      |
 | +--------------------------------------------------+
 | | bit  31:0 R/W  XOFF_MASK_DCS_1                   |
 | +--------------------------------------------------+
 | 
 | void   cpb_ctrl_lfield_XOFF_MASK_DCS_1_set       ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value[8] )
 | void   cpb_ctrl_lfield_XOFF_MASK_DCS_1_get       ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value[8] )
 | void   cpb_ctrl_lfield_range_XOFF_MASK_DCS_1_set ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpb_ctrl_lfield_range_XOFF_MASK_DCS_1_get ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x000401c0 + (N) * 0x4) MASK_DCS_2_REG |
 | |     N[0..7]                                      |
 | +--------------------------------------------------+
 | | bit  31:0 R/W  XOFF_MASK_DCS_2                   |
 | +--------------------------------------------------+
 | 
 | void   cpb_ctrl_lfield_XOFF_MASK_DCS_2_set       ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value[8] )
 | void   cpb_ctrl_lfield_XOFF_MASK_DCS_2_get       ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value[8] )
 | void   cpb_ctrl_lfield_range_XOFF_MASK_DCS_2_set ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpb_ctrl_lfield_range_XOFF_MASK_DCS_2_get ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x000401e0 + (N) * 0x4) MASK_DCS_3_REG |
 | |     N[0..7]                                      |
 | +--------------------------------------------------+
 | | bit  31:0 R/W  XOFF_MASK_DCS_3                   |
 | +--------------------------------------------------+
 | 
 | void   cpb_ctrl_lfield_XOFF_MASK_DCS_3_set       ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value[8] )
 | void   cpb_ctrl_lfield_XOFF_MASK_DCS_3_get       ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value[8] )
 | void   cpb_ctrl_lfield_range_XOFF_MASK_DCS_3_set ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpb_ctrl_lfield_range_XOFF_MASK_DCS_3_get ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------+
 | | Register 0x00041000 MCPB_IMUX |
 | +-------------------------------+
 | | bit  16 R/W  IM_IGR_EXT_SEL   |
 | | bit  8  R/W  IM_EGR_EXT_SEL   |
 | | bit  0  R/W  IM_REFLECT_SEL   |
 | +-------------------------------+
 | 
 | void   cpb_ctrl_field_IM_IGR_EXT_SEL_set ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 cpb_ctrl_field_IM_IGR_EXT_SEL_get ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   cpb_ctrl_field_IM_EGR_EXT_SEL_set ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 cpb_ctrl_field_IM_EGR_EXT_SEL_get ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   cpb_ctrl_field_IM_REFLECT_SEL_set ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 cpb_ctrl_field_IM_REFLECT_SEL_get ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | +-------------------------------+
 | | Register 0x00041004 MCPB_EMUX |
 | +-------------------------------+
 | | bit  16 R/W  EM_EGR_EXT_SEL   |
 | | bit  8  R/W  EM_IGR_EXT_SEL   |
 | +-------------------------------+
 | 
 | void   cpb_ctrl_field_EM_EGR_EXT_SEL_set ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 cpb_ctrl_field_EM_EGR_EXT_SEL_get ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   cpb_ctrl_field_EM_IGR_EXT_SEL_set ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 cpb_ctrl_field_EM_IGR_EXT_SEL_get ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | +------------------------------+
 | | Register 0x00041014 DCPB_MUX |
 | +------------------------------+
 | | bit  9 R/W  IGR_SIFD2_SEL    |
 | | bit  8 R/W  EGR_SIFD2_SEL    |
 | | bit  1 R/W  IGR_SIFD1_SEL    |
 | | bit  0 R/W  EGR_SIFD1_SEL    |
 | +------------------------------+
 | 
 | void   cpb_ctrl_field_IGR_SIFD2_SEL_set ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 cpb_ctrl_field_IGR_SIFD2_SEL_get ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   cpb_ctrl_field_EGR_SIFD2_SEL_set ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 cpb_ctrl_field_EGR_SIFD2_SEL_get ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   cpb_ctrl_field_IGR_SIFD1_SEL_set ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 cpb_ctrl_field_IGR_SIFD1_SEL_get ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   cpb_ctrl_field_EGR_SIFD1_SEL_set ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 cpb_ctrl_field_EGR_SIFD1_SEL_get ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 +----------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _CPB_CTRL_IO_H
#define _CPB_CTRL_IO_H

#include "cpb_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "cpb_ctrl_regs.h"
#include "cpb_ctrl_io_inline.h"


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 cpb_ctrl_buffer_init               ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void                 cpb_ctrl_buffer_flush              ( cpb_ctrl_buffer_t *b_ptr )
 | UINT32               cpb_ctrl_reg_read                  ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 cpb_ctrl_reg_write                 ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 cpb_ctrl_field_set                 ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 cpb_ctrl_action_on_write_field_set ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 cpb_ctrl_burst_read                ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 cpb_ctrl_burst_write               ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE cpb_ctrl_poll                      ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   cpb_ctrl_reg_MCPB_IMUX_write                  ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void   cpb_ctrl_reg_MCPB_IMUX_field_set              ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpb_ctrl_reg_MCPB_IMUX_read                   ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | void   cpb_ctrl_reg_MCPB_EMUX_write                  ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void   cpb_ctrl_reg_MCPB_EMUX_field_set              ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpb_ctrl_reg_MCPB_EMUX_read                   ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | void   cpb_ctrl_reg_DCPB_MUX_write                   ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void   cpb_ctrl_reg_DCPB_MUX_field_set               ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpb_ctrl_reg_DCPB_MUX_read                    ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | void   cpb_ctrl_reg_MASK_DCS_0_REG_array_burst_write ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void   cpb_ctrl_reg_MASK_DCS_0_REG_array_field_set   ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void   cpb_ctrl_reg_MASK_DCS_0_REG_array_burst_read  ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32 cpb_ctrl_reg_MASK_DCS_0_REG_array_read        ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 N )
 | 
 | void   cpb_ctrl_reg_MASK_DCS_1_REG_array_burst_write ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void   cpb_ctrl_reg_MASK_DCS_1_REG_array_field_set   ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void   cpb_ctrl_reg_MASK_DCS_1_REG_array_burst_read  ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32 cpb_ctrl_reg_MASK_DCS_1_REG_array_read        ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 N )
 | 
 | void   cpb_ctrl_reg_MASK_DCS_2_REG_array_burst_write ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void   cpb_ctrl_reg_MASK_DCS_2_REG_array_field_set   ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void   cpb_ctrl_reg_MASK_DCS_2_REG_array_burst_read  ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32 cpb_ctrl_reg_MASK_DCS_2_REG_array_read        ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 N )
 | 
 | void   cpb_ctrl_reg_MASK_DCS_3_REG_array_burst_write ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void   cpb_ctrl_reg_MASK_DCS_3_REG_array_field_set   ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void   cpb_ctrl_reg_MASK_DCS_3_REG_array_burst_read  ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32 cpb_ctrl_reg_MASK_DCS_3_REG_array_read        ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 N )
 | 
 | UINT32 cpb_ctrl_reg_INTSUM_read                      ( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _CPB_CTRL_IO_H */
