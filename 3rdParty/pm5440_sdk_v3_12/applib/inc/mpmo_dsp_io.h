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
 *     and register accessor functions for the mpmo_dsp block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing mpmo_dsp io functions is:
 | {
 |     mpmo_dsp_buffer_t b_ptr[1];
 |     mpmo_dsp_buffer_init( b_ptr, h_ptr );
 |     value = mpmo_dsp_field_<FIELD>_get( b_ptr, h_ptr );
 |     mpmo_dsp_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     mpmo_dsp_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = mpmo_dsp_field_<FIELD>_get( NULL, h_ptr );
 | mpmo_dsp_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +------------------------------------+
 | | Register 0x00006000 COMMON_CFG_REG |
 | +------------------------------------+
 | | bit  0 R/W  CN_REQ_MODE            |
 | +------------------------------------+
 | 
 | void   mpmo_dsp_field_CN_REQ_MODE_set ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_dsp_field_CN_REQ_MODE_get ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | +------------------------------------------------------------------+
 | | Register (0x00006004 + (N) * 0x8) FRAME_PERIOD_NUMERATOR_CFG_REG |
 | |     N[0..7]                                                      |
 | +------------------------------------------------------------------+
 | | bit  31:0 R/W  FRM_PERIOD_N                                      |
 | +------------------------------------------------------------------+
 | 
 | void   mpmo_dsp_field_FRM_PERIOD_N_set       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_FRM_PERIOD_N_get       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_range_FRM_PERIOD_N_set ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_dsp_field_range_FRM_PERIOD_N_get ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------------+
 | | Register (0x00006008 + (N) * 0x8) FRAME_PERIOD_DENOMINATOR_CFG_REG |
 | |     N[0..7]                                                        |
 | +--------------------------------------------------------------------+
 | | bit  31   R/W  FP_ENABLE                                           |
 | | bit  23:0 R/W  FRM_PERIOD_D                                        |
 | +--------------------------------------------------------------------+
 | 
 | void   mpmo_dsp_field_FP_ENABLE_set          ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_FP_ENABLE_get          ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_FRM_PERIOD_D_set       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_FRM_PERIOD_D_get       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_range_FRM_PERIOD_D_set ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_dsp_field_range_FRM_PERIOD_D_get ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x00006044 + (N) * 0x14) GENERAL_CFG_REG |
 | |     N[0..95]                                       |
 | +----------------------------------------------------+
 | | bit  29:27 R/W  FRM_CNT_SEL                        |
 | | bit  26:22 R/W  AGE_INT_GAIN                       |
 | | bit  21:17 R/W  AGE_PROP_GAIN                      |
 | | bit  16:12 R/W  ZONE_CENT_GAIN                     |
 | | bit  11:8  R/W  LPF_GAIN                           |
 | | bit  7     R/W  AGE_GAIN_UPDATE                    |
 | | bit  6     R/W  HOLDOVER                           |
 | | bit  5     R/W  ZONE_CENT_CONTINUOUS               |
 | | bit  4     R/W  ZONE_CENT_TRIGGER                  |
 | | bit  3     R/W  ZONE_CENT_ENABLE                   |
 | | bit  2     R/W  CN_BIT_MODE                        |
 | | bit  1     R/W  CN_GENERATOR_ENABLE                |
 | | bit  0     R/W  SLICE_ENABLE                       |
 | +----------------------------------------------------+
 | 
 | void   mpmo_dsp_field_FRM_CNT_SEL_set          ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_FRM_CNT_SEL_get          ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_range_FRM_CNT_SEL_set    ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_dsp_field_range_FRM_CNT_SEL_get    ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mpmo_dsp_field_AGE_INT_GAIN_set         ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_AGE_INT_GAIN_get         ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_range_AGE_INT_GAIN_set   ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_dsp_field_range_AGE_INT_GAIN_get   ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mpmo_dsp_field_AGE_PROP_GAIN_set        ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_AGE_PROP_GAIN_get        ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_range_AGE_PROP_GAIN_set  ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_dsp_field_range_AGE_PROP_GAIN_get  ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mpmo_dsp_field_ZONE_CENT_GAIN_set       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_ZONE_CENT_GAIN_get       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_range_ZONE_CENT_GAIN_set ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_dsp_field_range_ZONE_CENT_GAIN_get ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mpmo_dsp_field_LPF_GAIN_set             ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_LPF_GAIN_get             ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_range_LPF_GAIN_set       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_dsp_field_range_LPF_GAIN_get       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mpmo_dsp_field_AGE_GAIN_UPDATE_set      ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_AGE_GAIN_UPDATE_get      ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_HOLDOVER_set             ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_HOLDOVER_get             ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_ZONE_CENT_CONTINUOUS_set ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_ZONE_CENT_CONTINUOUS_get ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_ZONE_CENT_TRIGGER_set    ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_ZONE_CENT_TRIGGER_get    ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_ZONE_CENT_ENABLE_set     ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_ZONE_CENT_ENABLE_get     ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_CN_BIT_MODE_set          ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_CN_BIT_MODE_get          ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_CN_GENERATOR_ENABLE_set  ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_CN_GENERATOR_ENABLE_get  ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_SLICE_ENABLE_set         ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_SLICE_ENABLE_get         ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | 
 | +------------------------------------------------------------+
 | | Register (0x00006048 + (N) * 0x14) CN_BASE_INTEGER_CFG_REG |
 | |     N[0..95]                                               |
 | +------------------------------------------------------------+
 | | bit  23:0  R/W  CN_BASE_INT                                |
 | | bit  31    R/W  FAST_LOCK_ENABLE                           |
 | | bit  27:24 R/W  FAST_LOCK_THRESH                           |
 | +------------------------------------------------------------+
 | 
 | void   mpmo_dsp_field_CN_BASE_INT_set            ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_CN_BASE_INT_get            ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_range_CN_BASE_INT_set      ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_dsp_field_range_CN_BASE_INT_get      ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mpmo_dsp_field_FAST_LOCK_ENABLE_set       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_FAST_LOCK_ENABLE_get       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_FAST_LOCK_THRESH_set       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_FAST_LOCK_THRESH_get       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_range_FAST_LOCK_THRESH_set ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_dsp_field_range_FAST_LOCK_THRESH_get ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------+
 | | Register (0x0000604c + (N) * 0x14) CN_BASE_NUMERATOR_CFG_REG |
 | |     N[0..95]                                                 |
 | +--------------------------------------------------------------+
 | | bit  31:0 R/W  CN_BASE_N                                     |
 | +--------------------------------------------------------------+
 | 
 | void   mpmo_dsp_field_CN_BASE_N_set       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_CN_BASE_N_get       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_range_CN_BASE_N_set ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_dsp_field_range_CN_BASE_N_get ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------------+
 | | Register (0x00006050 + (N) * 0x14) CN_BASE_DENOMINATOR_CFG_REG |
 | |     N[0..95]                                                   |
 | +----------------------------------------------------------------+
 | | bit  31:0 R/W  CN_BASE_D                                       |
 | +----------------------------------------------------------------+
 | 
 | void   mpmo_dsp_field_CN_BASE_D_set       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_CN_BASE_D_get       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_range_CN_BASE_D_set ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_dsp_field_range_CN_BASE_D_get ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register (0x00006054 + (N) * 0x14) ODUFLEX_RESIZING_CFG_REG |
 | |     N[0..95]                                                |
 | +-------------------------------------------------------------+
 | | bit  29:28 R/W  BWR_RAMP_DELAY                              |
 | | bit  19:0  R/W  BWR_STEP                                    |
 | +-------------------------------------------------------------+
 | 
 | void   mpmo_dsp_field_BWR_RAMP_DELAY_set       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_BWR_RAMP_DELAY_get       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_range_BWR_RAMP_DELAY_set ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_dsp_field_range_BWR_RAMP_DELAY_get ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mpmo_dsp_field_BWR_STEP_set             ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_dsp_field_BWR_STEP_get             ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_dsp_field_range_BWR_STEP_set       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_dsp_field_range_BWR_STEP_get       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x00006684 + (N) * 0x8) DEBUG684_REG |
 | |     N[0..79]                                   |
 | +------------------------------------------------+
 | | bit  31:0 R  INTEGRATOR684                     |
 | +------------------------------------------------+
 | 
 | UINT32 mpmo_dsp_field_INTEGRATOR684_get       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | UINT32 mpmo_dsp_field_range_INTEGRATOR684_get ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _MPMO_DSP_IO_H
#define _MPMO_DSP_IO_H

#include "mpmo_loc.h"
#include "mpmo_dsp_regs.h"
#include "mpmo_dsp_io_inline.h"


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mpmo_dsp_buffer_init               ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void                 mpmo_dsp_buffer_flush              ( mpmo_dsp_buffer_t *b_ptr )
 | UINT32               mpmo_dsp_reg_read                  ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 mpmo_dsp_reg_write                 ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 mpmo_dsp_field_set                 ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 mpmo_dsp_action_on_write_field_set ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mpmo_dsp_burst_read                ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 mpmo_dsp_burst_write               ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE mpmo_dsp_poll                      ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   mpmo_dsp_reg_COMMON_CFG_REG_write                             ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void   mpmo_dsp_reg_COMMON_CFG_REG_field_set                         ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mpmo_dsp_reg_COMMON_CFG_REG_read                              ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void   mpmo_dsp_reg_FRAME_PERIOD_NUMERATOR_CFG_REG_array_write       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   mpmo_dsp_reg_FRAME_PERIOD_NUMERATOR_CFG_REG_array_field_set   ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mpmo_dsp_reg_FRAME_PERIOD_NUMERATOR_CFG_REG_array_read        ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | 
 | void   mpmo_dsp_reg_FRAME_PERIOD_DENOMINATOR_CFG_REG_array_write     ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   mpmo_dsp_reg_FRAME_PERIOD_DENOMINATOR_CFG_REG_array_field_set ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mpmo_dsp_reg_FRAME_PERIOD_DENOMINATOR_CFG_REG_array_read      ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | 
 | void   mpmo_dsp_reg_GENERAL_CFG_REG_array_write                      ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set                  ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mpmo_dsp_reg_GENERAL_CFG_REG_array_read                       ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | 
 | void   mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_write              ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_field_set          ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_read               ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | 
 | void   mpmo_dsp_reg_CN_BASE_NUMERATOR_CFG_REG_array_write            ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   mpmo_dsp_reg_CN_BASE_NUMERATOR_CFG_REG_array_field_set        ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mpmo_dsp_reg_CN_BASE_NUMERATOR_CFG_REG_array_read             ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | 
 | void   mpmo_dsp_reg_CN_BASE_DENOMINATOR_CFG_REG_array_write          ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   mpmo_dsp_reg_CN_BASE_DENOMINATOR_CFG_REG_array_field_set      ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mpmo_dsp_reg_CN_BASE_DENOMINATOR_CFG_REG_array_read           ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | 
 | void   mpmo_dsp_reg_ODUFLEX_RESIZING_CFG_REG_array_write             ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   mpmo_dsp_reg_ODUFLEX_RESIZING_CFG_REG_array_field_set         ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mpmo_dsp_reg_ODUFLEX_RESIZING_CFG_REG_array_read              ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32 mpmo_dsp_reg_DEBUG684_REG_array_read                          ( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _MPMO_DSP_IO_H */
