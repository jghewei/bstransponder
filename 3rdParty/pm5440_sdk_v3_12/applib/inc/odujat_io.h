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
 *     and register accessor functions for the odujat block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing odujat io functions is:
 | {
 |     odujat_buffer_t b_ptr[1];
 |     odujat_buffer_init( b_ptr, h_ptr );
 |     value = odujat_field_<FIELD>_get( b_ptr, h_ptr );
 |     odujat_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     odujat_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = odujat_field_<FIELD>_get( NULL, h_ptr );
 | odujat_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------+
 | | Register 0x00000000 CFG     |
 | +-----------------------------+
 | | bit  17:16 R/W  FIFO_LIMIT  |
 | | bit  9     R/W  RAM_LS      |
 | | bit  8     R/W  PG_OVERRIDE |
 | +-----------------------------+
 | 
 | void   odujat_field_FIFO_LIMIT_set       ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 value )
 | UINT32 odujat_field_FIFO_LIMIT_get       ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
 | void   odujat_field_range_FIFO_LIMIT_set ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odujat_field_range_FIFO_LIMIT_get ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   odujat_field_RAM_LS_set           ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 value )
 | UINT32 odujat_field_RAM_LS_get           ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
 | void   odujat_field_PG_OVERRIDE_set      ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 value )
 | UINT32 odujat_field_PG_OVERRIDE_get      ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
 | 
 | +--------------------------------+
 | | Register 0x00000004 ERR_INT_EN |
 | +--------------------------------+
 | | bit  2 R/W  DQ_LIMIT_E         |
 | | bit  1 R/W  GAIN_UNDF_E        |
 | | bit  0 R/W  GAIN_OVF_E         |
 | +--------------------------------+
 | 
 | void   odujat_field_DQ_LIMIT_E_set  ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 value )
 | UINT32 odujat_field_DQ_LIMIT_E_get  ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
 | void   odujat_field_GAIN_UNDF_E_set ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 value )
 | UINT32 odujat_field_GAIN_UNDF_E_get ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
 | void   odujat_field_GAIN_OVF_E_set  ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 value )
 | UINT32 odujat_field_GAIN_OVF_E_get  ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
 | 
 | +------------------------------+
 | | Register 0x00000008 ERR_INTS |
 | +------------------------------+
 | | bit  2 R/W  DQ_LIMIT_I       |
 | | bit  1 R/W  GAIN_UNDF_I      |
 | | bit  0 R/W  GAIN_OVF_I       |
 | +------------------------------+
 | 
 | void                 odujat_field_DQ_LIMIT_I_set_to_clear  ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 value )
 | UINT32               odujat_field_DQ_LIMIT_I_get           ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odujat_field_DQ_LIMIT_I_poll          ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odujat_field_GAIN_UNDF_I_set_to_clear ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 value )
 | UINT32               odujat_field_GAIN_UNDF_I_get          ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odujat_field_GAIN_UNDF_I_poll         ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odujat_field_GAIN_OVF_I_set_to_clear  ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 value )
 | UINT32               odujat_field_GAIN_OVF_I_get           ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odujat_field_GAIN_OVF_I_poll          ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------+
 | | Register 0x0000000c ERR_CHAN |
 | +------------------------------+
 | | bit  14:8 R  LIMIT_ERR_CHAN  |
 | | bit  6:0  R  GAIN_ERR_CHAN   |
 | +------------------------------+
 | 
 | UINT32 odujat_field_LIMIT_ERR_CHAN_get       ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
 | UINT32 odujat_field_range_LIMIT_ERR_CHAN_get ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 odujat_field_GAIN_ERR_CHAN_get        ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
 | UINT32 odujat_field_range_GAIN_ERR_CHAN_get  ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x00000100 + (N) * 0x10) CHAN_CFG_0 |
 | |     N[0..95]                                  |
 | +-----------------------------------------------+
 | | bit  31    R/W  JAT_ENABLE                    |
 | | bit  24    R/W  DQ_AT_LIMIT                   |
 | | bit  20:18 R/W  CND_SELECT                    |
 | | bit  17    R/W  ADD_RESIDUE                   |
 | | bit  16    R/W  ADD_CND                       |
 | | bit  11:8  R/W  LOOP_ALPHA_SELECT             |
 | | bit  4:0   R/W  CONV_GAIN_SELECT              |
 | +-----------------------------------------------+
 | 
 | void   odujat_field_JAT_ENABLE_set              ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odujat_field_JAT_ENABLE_get              ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N )
 | void   odujat_field_DQ_AT_LIMIT_set             ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odujat_field_DQ_AT_LIMIT_get             ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N )
 | void   odujat_field_CND_SELECT_set              ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odujat_field_CND_SELECT_get              ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N )
 | void   odujat_field_range_CND_SELECT_set        ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odujat_field_range_CND_SELECT_get        ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odujat_field_ADD_RESIDUE_set             ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odujat_field_ADD_RESIDUE_get             ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N )
 | void   odujat_field_ADD_CND_set                 ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odujat_field_ADD_CND_get                 ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N )
 | void   odujat_field_LOOP_ALPHA_SELECT_set       ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odujat_field_LOOP_ALPHA_SELECT_get       ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N )
 | void   odujat_field_range_LOOP_ALPHA_SELECT_set ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odujat_field_range_LOOP_ALPHA_SELECT_get ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odujat_field_CONV_GAIN_SELECT_set        ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odujat_field_CONV_GAIN_SELECT_get        ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N )
 | void   odujat_field_range_CONV_GAIN_SELECT_set  ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odujat_field_range_CONV_GAIN_SELECT_get  ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x00000104 + (N) * 0x10) CHAN_CFG_1 |
 | |     N[0..95]                                  |
 | +-----------------------------------------------+
 | | bit  25:0 R/W  MC_NUMERATOR                   |
 | +-----------------------------------------------+
 | 
 | void   odujat_field_MC_NUMERATOR_set       ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odujat_field_MC_NUMERATOR_get       ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N )
 | void   odujat_field_range_MC_NUMERATOR_set ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odujat_field_range_MC_NUMERATOR_get ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x00000108 + (N) * 0x10) CHAN_CFG_2 |
 | |     N[0..95]                                  |
 | +-----------------------------------------------+
 | | bit  24:16 R/W  FIFO_LEVEL_LIMIT              |
 | | bit  9:0   R/W  CND_DELAY                     |
 | +-----------------------------------------------+
 | 
 | void   odujat_field_FIFO_LEVEL_LIMIT_set       ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odujat_field_FIFO_LEVEL_LIMIT_get       ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N )
 | void   odujat_field_range_FIFO_LEVEL_LIMIT_set ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odujat_field_range_FIFO_LEVEL_LIMIT_get ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odujat_field_CND_DELAY_set              ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odujat_field_CND_DELAY_get              ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N )
 | void   odujat_field_range_CND_DELAY_set        ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odujat_field_range_CND_DELAY_get        ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _ODUJAT_IO_H
#define _ODUJAT_IO_H

#include "odujat_loc.h"
#include "odujat_regs.h"
#include "odujat_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 odujat_buffer_init               ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
 | void                 odujat_buffer_flush              ( odujat_buffer_t *b_ptr )
 | UINT32               odujat_reg_read                  ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 odujat_reg_write                 ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 odujat_field_set                 ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 odujat_action_on_write_field_set ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odujat_burst_read                ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 odujat_burst_write               ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE odujat_poll                      ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 odujat_reg_CFG_write                          ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 value )
 | void                 odujat_reg_CFG_field_set                      ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odujat_reg_CFG_read                           ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
 | 
 | void                 odujat_reg_CHAN_CFG_0_array_write             ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odujat_reg_CHAN_CFG_0_array_field_set         ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odujat_reg_CHAN_CFG_0_array_read              ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odujat_reg_CHAN_CFG_1_array_write             ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odujat_reg_CHAN_CFG_1_array_field_set         ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odujat_reg_CHAN_CFG_1_array_read              ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odujat_reg_CHAN_CFG_2_array_write             ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odujat_reg_CHAN_CFG_2_array_field_set         ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odujat_reg_CHAN_CFG_2_array_read              ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odujat_reg_ERR_INT_EN_write                   ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 value )
 | void                 odujat_reg_ERR_INT_EN_field_set               ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odujat_reg_ERR_INT_EN_read                    ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
 | 
 | void                 odujat_reg_ERR_INTS_write                     ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 value )
 | void                 odujat_reg_ERR_INTS_action_on_write_field_set ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odujat_reg_ERR_INTS_read                      ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odujat_reg_ERR_INTS_poll                      ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               odujat_reg_ERR_CHAN_read                      ( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _ODUJAT_IO_H */
