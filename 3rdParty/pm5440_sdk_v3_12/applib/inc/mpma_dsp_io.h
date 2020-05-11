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
 *     and register accessor functions for the mpma_dsp block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing mpma_dsp io functions is:
 | {
 |     mpma_dsp_buffer_t b_ptr[1];
 |     mpma_dsp_buffer_init( b_ptr, h_ptr );
 |     value = mpma_dsp_field_<FIELD>_get( b_ptr, h_ptr );
 |     mpma_dsp_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     mpma_dsp_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = mpma_dsp_field_<FIELD>_get( NULL, h_ptr );
 | mpma_dsp_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +------------------------------------+
 | | Register 0x00003800 COMMON_CFG_REG |
 | +------------------------------------+
 | | bit  16  R/W  SCHEDULING_PACER_EN  |
 | | bit  8   R/W  INTERRUPT_CLR        |
 | | bit  5:0 R/W  CLIENT_WIDTH         |
 | +------------------------------------+
 | 
 | void   mpma_dsp_field_SCHEDULING_PACER_EN_set ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | UINT32 mpma_dsp_field_SCHEDULING_PACER_EN_get ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | void   mpma_dsp_field_INTERRUPT_CLR_set       ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | UINT32 mpma_dsp_field_INTERRUPT_CLR_get       ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | void   mpma_dsp_field_CLIENT_WIDTH_set        ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | UINT32 mpma_dsp_field_CLIENT_WIDTH_get        ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | void   mpma_dsp_field_range_CLIENT_WIDTH_set  ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_dsp_field_range_CLIENT_WIDTH_get  ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x00003804 + (N) * 0x14) GENERAL_CFG_REG |
 | |     N[0..95]                                       |
 | +----------------------------------------------------+
 | | bit  31:16 R/W  PKTS_PER_CN                        |
 | | bit  4     R/W  CN_MODE                            |
 | | bit  3     R/W  CN_BIT_MODE                        |
 | | bit  2     R/W  CN_ENABLE                          |
 | | bit  1     R/W  CND_ENABLE                         |
 | | bit  0     R/W  SLICE_ENABLE                       |
 | | bit  15:12 R/W  INTEGRATOR_CLAMP                   |
 | | bit  5     R/W  SCHEDULING_PACER_EN_LOCAL          |
 | +----------------------------------------------------+
 | 
 | void   mpma_dsp_field_PKTS_PER_CN_set               ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_dsp_field_PKTS_PER_CN_get               ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_dsp_field_range_PKTS_PER_CN_set         ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_dsp_field_range_PKTS_PER_CN_get         ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mpma_dsp_field_CN_MODE_set                   ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_dsp_field_CN_MODE_get                   ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_dsp_field_CN_BIT_MODE_set               ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_dsp_field_CN_BIT_MODE_get               ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_dsp_field_CN_ENABLE_set                 ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_dsp_field_CN_ENABLE_get                 ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_dsp_field_CND_ENABLE_set                ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_dsp_field_CND_ENABLE_get                ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_dsp_field_SLICE_ENABLE_set              ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_dsp_field_SLICE_ENABLE_get              ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_dsp_field_INTEGRATOR_CLAMP_set          ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_dsp_field_INTEGRATOR_CLAMP_get          ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_dsp_field_range_INTEGRATOR_CLAMP_set    ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_dsp_field_range_INTEGRATOR_CLAMP_get    ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mpma_dsp_field_SCHEDULING_PACER_EN_LOCAL_set ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_dsp_field_SCHEDULING_PACER_EN_LOCAL_get ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00003808 + (N) * 0x14) PACKET_SIZE_CFG_REG |
 | |     N[0..95]                                           |
 | +--------------------------------------------------------+
 | | bit  31:24 R/W  PKT_PERIOD_D                           |
 | | bit  16    R/W  PKT_SIZE_VAR                           |
 | | bit  8:0   R/W  PKT_SIZE                               |
 | +--------------------------------------------------------+
 | 
 | void   mpma_dsp_field_PKT_PERIOD_D_set       ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_dsp_field_PKT_PERIOD_D_get       ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_dsp_field_range_PKT_PERIOD_D_set ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_dsp_field_range_PKT_PERIOD_D_get ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mpma_dsp_field_PKT_SIZE_VAR_set       ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_dsp_field_PKT_SIZE_VAR_get       ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_dsp_field_PKT_SIZE_set           ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_dsp_field_PKT_SIZE_get           ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_dsp_field_range_PKT_SIZE_set     ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_dsp_field_range_PKT_SIZE_get     ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x0000380c + (N) * 0x14) PACKET_PERIOD_CFG_REG |
 | |     N[0..95]                                             |
 | +----------------------------------------------------------+
 | | bit  24:16 R/W  PKT_PERIOD_N                             |
 | | bit  15:0  R/W  PKT_PERIOD_INT                           |
 | +----------------------------------------------------------+
 | 
 | void   mpma_dsp_field_PKT_PERIOD_N_set         ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_dsp_field_PKT_PERIOD_N_get         ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_dsp_field_range_PKT_PERIOD_N_set   ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_dsp_field_range_PKT_PERIOD_N_get   ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mpma_dsp_field_PKT_PERIOD_INT_set       ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_dsp_field_PKT_PERIOD_INT_get       ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_dsp_field_range_PKT_PERIOD_INT_set ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_dsp_field_range_PKT_PERIOD_INT_get ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x00003810 + (N) * 0x14) DSM_CFG_REG |
 | |     N[0..95]                                   |
 | +------------------------------------------------+
 | | bit  6   R    INTEGRATOR_UNDERFLOW             |
 | | bit  5   R    INTEGRATOR_OVERFLOW              |
 | | bit  4   R/W  DSM_TYPE                         |
 | | bit  3:0 R/W  MPMA_GAIN                        |
 | | bit  10  R    CLAMP_UNDERFLOW                  |
 | | bit  9   R    CLAMP_OVERFLOW                   |
 | +------------------------------------------------+
 | 
 | void   mpma_dsp_field_DSM_TYPE_set             ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_dsp_field_DSM_TYPE_get             ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_dsp_field_MPMA_GAIN_set            ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_dsp_field_MPMA_GAIN_get            ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_dsp_field_range_MPMA_GAIN_set      ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_dsp_field_range_MPMA_GAIN_get      ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mpma_dsp_field_INTEGRATOR_UNDERFLOW_get ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | UINT32 mpma_dsp_field_INTEGRATOR_OVERFLOW_get  ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | UINT32 mpma_dsp_field_CLAMP_UNDERFLOW_get      ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | UINT32 mpma_dsp_field_CLAMP_OVERFLOW_get       ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | 
 | +------------------------------------------------+
 | | Register (0x00003814 + (N) * 0x14) CN_BASE_REG |
 | |     N[0..95]                                   |
 | +------------------------------------------------+
 | | bit  28:24 R/W  CN_BASE_OFFSET                 |
 | | bit  23:0  R/W  CN_BASE                        |
 | +------------------------------------------------+
 | 
 | void   mpma_dsp_field_CN_BASE_OFFSET_set       ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_dsp_field_CN_BASE_OFFSET_get       ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_dsp_field_range_CN_BASE_OFFSET_set ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_dsp_field_range_CN_BASE_OFFSET_get ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mpma_dsp_field_CN_BASE_set              ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_dsp_field_CN_BASE_get              ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_dsp_field_range_CN_BASE_set        ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_dsp_field_range_CN_BASE_get        ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _MPMA_DSP_IO_H
#define _MPMA_DSP_IO_H

#include "mpma_loc.h"
#include "mpma_dsp_regs.h"
#include "mpma_dsp_io_inline.h"


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mpma_dsp_buffer_init               ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | void                 mpma_dsp_buffer_flush              ( mpma_dsp_buffer_t *b_ptr )
 | UINT32               mpma_dsp_reg_read                  ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 mpma_dsp_reg_write                 ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 mpma_dsp_field_set                 ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 mpma_dsp_action_on_write_field_set ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mpma_dsp_burst_read                ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 mpma_dsp_burst_write               ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE mpma_dsp_poll                      ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   mpma_dsp_reg_COMMON_CFG_REG_write                  ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | void   mpma_dsp_reg_COMMON_CFG_REG_field_set              ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mpma_dsp_reg_COMMON_CFG_REG_read                   ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | 
 | void   mpma_dsp_reg_GENERAL_CFG_REG_array_write           ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   mpma_dsp_reg_GENERAL_CFG_REG_array_field_set       ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mpma_dsp_reg_GENERAL_CFG_REG_array_read            ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | 
 | void   mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_write       ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_field_set   ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_read        ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | 
 | void   mpma_dsp_reg_PACKET_PERIOD_CFG_REG_array_write     ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   mpma_dsp_reg_PACKET_PERIOD_CFG_REG_array_field_set ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mpma_dsp_reg_PACKET_PERIOD_CFG_REG_array_read      ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | 
 | void   mpma_dsp_reg_DSM_CFG_REG_array_write               ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   mpma_dsp_reg_DSM_CFG_REG_array_field_set           ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mpma_dsp_reg_DSM_CFG_REG_array_read                ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | 
 | void   mpma_dsp_reg_CN_BASE_REG_array_write               ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   mpma_dsp_reg_CN_BASE_REG_array_field_set           ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mpma_dsp_reg_CN_BASE_REG_array_read                ( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _MPMA_DSP_IO_H */
