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
 *     and register accessor functions for the cbr_fsgm block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing cbr_fsgm io functions is:
 | {
 |     cbr_fsgm_buffer_t b_ptr[1];
 |     cbr_fsgm_buffer_init( b_ptr, h_ptr );
 |     value = cbr_fsgm_field_<FIELD>_get( b_ptr, h_ptr );
 |     cbr_fsgm_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     cbr_fsgm_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = cbr_fsgm_field_<FIELD>_get( NULL, h_ptr );
 | cbr_fsgm_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +------------------------------------+
 | | Register 0x00000000 GLOBAL_CFG_REG |
 | +------------------------------------+
 | | bit  5 R/W  MS_AIS_RSOH_FILL       |
 | | bit  4 R/W  MODE_40G               |
 | | bit  0 R/W  SW_RESET               |
 | +------------------------------------+
 | 
 | void   cbr_fsgm_field_MS_AIS_RSOH_FILL_set ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value )
 | UINT32 cbr_fsgm_field_MS_AIS_RSOH_FILL_get ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
 | void   cbr_fsgm_field_MODE_40G_set         ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value )
 | UINT32 cbr_fsgm_field_MODE_40G_get         ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
 | void   cbr_fsgm_field_SW_RESET_set         ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value )
 | UINT32 cbr_fsgm_field_SW_RESET_get         ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
 | 
 | +------------------------------------------------+
 | | Register (0x00000004 + (channel) * 0x0000000c) |
 | | CHANNEL_0_CFG_REG_by_channel                   |
 | |     channel[0..3]                              |
 | +------------------------------------------------+
 | | bit  6   R/W  CH_n_FAULT_INS_ENA               |
 | | bit  1   R/W  CH_n_PN11_MON_ENA                |
 | | bit  8   R/W  CH_n_FAULT_PROP_ENA              |
 | | bit  4   R/W  CH_n_FORCE_FAULT                 |
 | | bit  0   R/W  CH_n_ENA                         |
 | | bit  3:2 R/W  CH_n_PATT_MODE                   |
 | | bit  7   R/W  CH_n_LOS_PROP_ENA                |
 | | bit  5   R/W  CH_n_LOS_INS_ENA                 |
 | +------------------------------------------------+
 | 
 | void   cbr_fsgm_field_CH_n_FAULT_INS_ENA_set   ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 value )
 | UINT32 cbr_fsgm_field_CH_n_FAULT_INS_ENA_get   ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel )
 | void   cbr_fsgm_field_CH_n_PN11_MON_ENA_set    ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 value )
 | UINT32 cbr_fsgm_field_CH_n_PN11_MON_ENA_get    ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel )
 | void   cbr_fsgm_field_CH_n_FAULT_PROP_ENA_set  ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 value )
 | UINT32 cbr_fsgm_field_CH_n_FAULT_PROP_ENA_get  ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel )
 | void   cbr_fsgm_field_CH_n_FORCE_FAULT_set     ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 value )
 | UINT32 cbr_fsgm_field_CH_n_FORCE_FAULT_get     ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel )
 | void   cbr_fsgm_field_CH_n_ENA_set             ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 value )
 | UINT32 cbr_fsgm_field_CH_n_ENA_get             ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel )
 | void   cbr_fsgm_field_CH_n_PATT_MODE_set       ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 value )
 | UINT32 cbr_fsgm_field_CH_n_PATT_MODE_get       ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel )
 | void   cbr_fsgm_field_range_CH_n_PATT_MODE_set ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbr_fsgm_field_range_CH_n_PATT_MODE_get ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 start_bit, UINT32 stop_bit )
 | void   cbr_fsgm_field_CH_n_LOS_PROP_ENA_set    ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 value )
 | UINT32 cbr_fsgm_field_CH_n_LOS_PROP_ENA_get    ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel )
 | void   cbr_fsgm_field_CH_n_LOS_INS_ENA_set     ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 value )
 | UINT32 cbr_fsgm_field_CH_n_LOS_INS_ENA_get     ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel )
 | 
 | +-------------------------------------------------------------------------------------+
 | | Register (0x00000008 + (channel) * 0x0000000c) CHANNEL_0_GAP_RATIO_N_REG_by_channel |
 | |     channel[0..3]                                                                   |
 | +-------------------------------------------------------------------------------------+
 | | bit  31:0 R/W  CH_n_GAP_NUM                                                         |
 | +-------------------------------------------------------------------------------------+
 | 
 | void   cbr_fsgm_field_CH_n_GAP_NUM_set       ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 value )
 | UINT32 cbr_fsgm_field_CH_n_GAP_NUM_get       ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel )
 | void   cbr_fsgm_field_range_CH_n_GAP_NUM_set ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbr_fsgm_field_range_CH_n_GAP_NUM_get ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------------------------+
 | | Register (0x0000000c + (channel) * 0x0000000c) CHANNEL_0_GAP_RATIO_D_REG_by_channel |
 | |     channel[0..3]                                                                   |
 | +-------------------------------------------------------------------------------------+
 | | bit  31:0 R/W  CH_n_GAP_DENOM                                                       |
 | +-------------------------------------------------------------------------------------+
 | 
 | void   cbr_fsgm_field_CH_n_GAP_DENOM_set       ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 value )
 | UINT32 cbr_fsgm_field_CH_n_GAP_DENOM_get       ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel )
 | void   cbr_fsgm_field_range_CH_n_GAP_DENOM_set ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbr_fsgm_field_range_CH_n_GAP_DENOM_get ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x00000034 INT_EN_REG |
 | +--------------------------------+
 | | bit  3 R/W  CH3_PN11_DET_E     |
 | | bit  2 R/W  CH2_PN11_DET_E     |
 | | bit  1 R/W  CH1_PN11_DET_E     |
 | | bit  0 R/W  CH0_PN11_DET_E     |
 | +--------------------------------+
 | 
 | void   cbr_fsgm_field_CH3_PN11_DET_E_set ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value )
 | UINT32 cbr_fsgm_field_CH3_PN11_DET_E_get ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
 | void   cbr_fsgm_field_CH2_PN11_DET_E_set ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value )
 | UINT32 cbr_fsgm_field_CH2_PN11_DET_E_get ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
 | void   cbr_fsgm_field_CH1_PN11_DET_E_set ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value )
 | UINT32 cbr_fsgm_field_CH1_PN11_DET_E_get ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
 | void   cbr_fsgm_field_CH0_PN11_DET_E_set ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value )
 | UINT32 cbr_fsgm_field_CH0_PN11_DET_E_get ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
 | 
 | +----------------------------------------+
 | | Register 0x00000038 INT_INDICATION_REG |
 | +----------------------------------------+
 | | bit  3 R/W  CH3_PN11_DET_I             |
 | | bit  2 R/W  CH2_PN11_DET_I             |
 | | bit  1 R/W  CH1_PN11_DET_I             |
 | | bit  0 R/W  CH0_PN11_DET_I             |
 | +----------------------------------------+
 | 
 | void                 cbr_fsgm_field_CH3_PN11_DET_I_set_to_clear ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value )
 | UINT32               cbr_fsgm_field_CH3_PN11_DET_I_get          ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH3_PN11_DET_I_poll         ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 cbr_fsgm_field_CH2_PN11_DET_I_set_to_clear ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value )
 | UINT32               cbr_fsgm_field_CH2_PN11_DET_I_get          ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH2_PN11_DET_I_poll         ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 cbr_fsgm_field_CH1_PN11_DET_I_set_to_clear ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value )
 | UINT32               cbr_fsgm_field_CH1_PN11_DET_I_get          ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH1_PN11_DET_I_poll         ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 cbr_fsgm_field_CH0_PN11_DET_I_set_to_clear ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value )
 | UINT32               cbr_fsgm_field_CH0_PN11_DET_I_get          ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH0_PN11_DET_I_poll         ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------+
 | | Register 0x0000003c INT_STAT_REG |
 | +----------------------------------+
 | | bit  3 R  CH3_PN11_DET_V         |
 | | bit  2 R  CH2_PN11_DET_V         |
 | | bit  1 R  CH1_PN11_DET_V         |
 | | bit  0 R  CH0_PN11_DET_V         |
 | +----------------------------------+
 | 
 | UINT32               cbr_fsgm_field_CH3_PN11_DET_V_get  ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH3_PN11_DET_V_poll ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               cbr_fsgm_field_CH2_PN11_DET_V_get  ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH2_PN11_DET_V_poll ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               cbr_fsgm_field_CH1_PN11_DET_V_get  ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH1_PN11_DET_V_poll ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               cbr_fsgm_field_CH0_PN11_DET_V_get  ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH0_PN11_DET_V_poll ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _CBR_FSGM_IO_H
#define _CBR_FSGM_IO_H

#include "cbr_fsgm_api.h"
#include "cbr_fsgm_regs.h"
#include "cbr_fsgm_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 cbr_fsgm_buffer_init               ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
 | void                 cbr_fsgm_buffer_flush              ( cbr_fsgm_buffer_t *b_ptr )
 | UINT32               cbr_fsgm_reg_read                  ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 cbr_fsgm_reg_write                 ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 cbr_fsgm_field_set                 ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 cbr_fsgm_action_on_write_field_set ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 cbr_fsgm_burst_read                ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 cbr_fsgm_burst_write               ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE cbr_fsgm_poll                      ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 cbr_fsgm_reg_GLOBAL_CFG_REG_write                                 ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value )
 | void                 cbr_fsgm_reg_GLOBAL_CFG_REG_field_set                             ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               cbr_fsgm_reg_GLOBAL_CFG_REG_read                                  ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
 | 
 | void                 cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_write             ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 value )
 | void                 cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_field_set         ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_read              ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel )
 | 
 | void                 cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_D_REG_by_channel_array_write     ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 value )
 | void                 cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_D_REG_by_channel_array_field_set ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_D_REG_by_channel_array_read      ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel )
 | 
 | void                 cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_N_REG_by_channel_array_write     ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 value )
 | void                 cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_N_REG_by_channel_array_field_set ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_N_REG_by_channel_array_read      ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32  channel )
 | 
 | void                 cbr_fsgm_reg_INT_EN_REG_write                                     ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value )
 | void                 cbr_fsgm_reg_INT_EN_REG_field_set                                 ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               cbr_fsgm_reg_INT_EN_REG_read                                      ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
 | 
 | void                 cbr_fsgm_reg_INT_INDICATION_REG_write                             ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 value )
 | void                 cbr_fsgm_reg_INT_INDICATION_REG_action_on_write_field_set         ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               cbr_fsgm_reg_INT_INDICATION_REG_read                              ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE cbr_fsgm_reg_INT_INDICATION_REG_poll                              ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               cbr_fsgm_reg_INT_STAT_REG_read                                    ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE cbr_fsgm_reg_INT_STAT_REG_poll                                    ( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _CBR_FSGM_IO_H */
