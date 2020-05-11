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
 *     and register accessor functions for the otudg block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing otudg io functions is:
 | {
 |     otudg_buffer_t b_ptr[1];
 |     otudg_buffer_init( b_ptr, h_ptr );
 |     value = otudg_field_<FIELD>_get( b_ptr, h_ptr );
 |     otudg_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     otudg_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = otudg_field_<FIELD>_get( NULL, h_ptr );
 | otudg_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------+
 | | Register 0x00000400 FIFO_CTRL_REG |
 | +-----------------------------------+
 | | bit  23:16 R/W  CENTER_THRES      |
 | | bit  7:0   R/W  A_FULL_THRES      |
 | +-----------------------------------+
 | 
 | void   otudg_field_CENTER_THRES_set       ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otudg_field_CENTER_THRES_get       ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otudg_field_range_CENTER_THRES_set ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otudg_field_range_CENTER_THRES_get ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   otudg_field_A_FULL_THRES_set       ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otudg_field_A_FULL_THRES_get       ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otudg_field_range_A_FULL_THRES_set ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otudg_field_range_A_FULL_THRES_get ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x00000404 + (N) * 0x10) GEN_CFG_REG |
 | |     N[0..11]                                   |
 | +------------------------------------------------+
 | | bit  0 R/W  SLICE_EN                           |
 | +------------------------------------------------+
 | 
 | void   otudg_field_SLICE_EN_set ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otudg_field_SLICE_EN_get ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | +------------------------------------------------------+
 | | Register (0x00000408 + (N) * 0x10) SLICE_DSM_NUM_REG |
 | |     N[0..11]                                         |
 | +------------------------------------------------------+
 | | bit  23:0 R/W  DSM_NUM                               |
 | +------------------------------------------------------+
 | 
 | void   otudg_field_DSM_NUM_set       ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otudg_field_DSM_NUM_get       ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otudg_field_range_DSM_NUM_set ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otudg_field_range_DSM_NUM_get ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x0000040c + (N) * 0x10) SLICE_DSM_DEN_REG |
 | |     N[0..11]                                         |
 | +------------------------------------------------------+
 | | bit  23:0 R/W  DSM_DEN                               |
 | +------------------------------------------------------+
 | 
 | void   otudg_field_DSM_DEN_set       ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otudg_field_DSM_DEN_get       ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otudg_field_range_DSM_DEN_set ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otudg_field_range_DSM_DEN_get ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x000004f0 V_FIFO_OVR_INT_REG |
 | +----------------------------------------+
 | | bit  11:0 R/W  V_FIFO_OVR_I            |
 | +----------------------------------------+
 | 
 | void                 otudg_field_V_FIFO_OVR_I_set_to_clear       ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otudg_field_V_FIFO_OVR_I_get                ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otudg_field_range_V_FIFO_OVR_I_set_to_clear ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otudg_field_range_V_FIFO_OVR_I_get          ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otudg_field_range_V_FIFO_OVR_I_poll         ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otudg_field_V_FIFO_OVR_I_poll               ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------+
 | | Register 0x000004f4 V_FIFO_UDR_INT_REG |
 | +----------------------------------------+
 | | bit  11:0 R/W  V_FIFO_UDR_I            |
 | +----------------------------------------+
 | 
 | void                 otudg_field_V_FIFO_UDR_I_set_to_clear       ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otudg_field_V_FIFO_UDR_I_get                ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otudg_field_range_V_FIFO_UDR_I_set_to_clear ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otudg_field_range_V_FIFO_UDR_I_get          ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otudg_field_range_V_FIFO_UDR_I_poll         ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otudg_field_V_FIFO_UDR_I_poll               ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x000004f8 V_FIFO_OVR_INT_ENREG |
 | +------------------------------------------+
 | | bit  11:0 R/W  V_FIFO_OVR_E              |
 | +------------------------------------------+
 | 
 | void   otudg_field_V_FIFO_OVR_E_set       ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otudg_field_V_FIFO_OVR_E_get       ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otudg_field_range_V_FIFO_OVR_E_set ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otudg_field_range_V_FIFO_OVR_E_get ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x000004fc V_FIFO_UDR_INT_EN_REG |
 | +-------------------------------------------+
 | | bit  11:0 R/W  V_FIFO_UDR_E               |
 | +-------------------------------------------+
 | 
 | void   otudg_field_V_FIFO_UDR_E_set       ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otudg_field_V_FIFO_UDR_E_get       ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otudg_field_range_V_FIFO_UDR_E_set ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otudg_field_range_V_FIFO_UDR_E_get ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _OTUDG_IO_H
#define _OTUDG_IO_H

#include "pmc_sys.h"
#include "pmc_types.h"
#include "otudg_regs.h"
#include "otudg_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 otudg_buffer_init               ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otudg_buffer_flush              ( otudg_buffer_t *b_ptr )
 | UINT32               otudg_reg_read                  ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 otudg_reg_write                 ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 otudg_field_set                 ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 otudg_action_on_write_field_set ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 otudg_burst_read                ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 otudg_burst_write               ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE otudg_poll                      ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 otudg_reg_FIFO_CTRL_REG_write                          ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otudg_reg_FIFO_CTRL_REG_field_set                      ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otudg_reg_FIFO_CTRL_REG_read                           ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otudg_reg_GEN_CFG_REG_array_write                      ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otudg_reg_GEN_CFG_REG_array_field_set                  ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otudg_reg_GEN_CFG_REG_array_read                       ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otudg_reg_SLICE_DSM_NUM_REG_array_write                ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otudg_reg_SLICE_DSM_NUM_REG_array_field_set            ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otudg_reg_SLICE_DSM_NUM_REG_array_read                 ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otudg_reg_SLICE_DSM_DEN_REG_array_write                ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otudg_reg_SLICE_DSM_DEN_REG_array_field_set            ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otudg_reg_SLICE_DSM_DEN_REG_array_read                 ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otudg_reg_V_FIFO_OVR_INT_ENREG_write                   ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otudg_reg_V_FIFO_OVR_INT_ENREG_field_set               ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otudg_reg_V_FIFO_OVR_INT_ENREG_read                    ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otudg_reg_V_FIFO_UDR_INT_EN_REG_write                  ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otudg_reg_V_FIFO_UDR_INT_EN_REG_field_set              ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otudg_reg_V_FIFO_UDR_INT_EN_REG_read                   ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otudg_reg_V_FIFO_OVR_INT_REG_write                     ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otudg_reg_V_FIFO_OVR_INT_REG_action_on_write_field_set ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otudg_reg_V_FIFO_OVR_INT_REG_read                      ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otudg_reg_V_FIFO_OVR_INT_REG_poll                      ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otudg_reg_V_FIFO_UDR_INT_REG_write                     ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otudg_reg_V_FIFO_UDR_INT_REG_action_on_write_field_set ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otudg_reg_V_FIFO_UDR_INT_REG_read                      ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otudg_reg_V_FIFO_UDR_INT_REG_poll                      ( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _OTUDG_IO_H */
