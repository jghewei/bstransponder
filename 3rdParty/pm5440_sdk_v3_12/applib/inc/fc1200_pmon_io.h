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
 *     and register accessor functions for the fc1200_pmon block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing fc1200_pmon io functions is:
 | {
 |     fc1200_pmon_buffer_t b_ptr[1];
 |     fc1200_pmon_buffer_init( b_ptr, h_ptr, A );
 |     value = fc1200_pmon_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     fc1200_pmon_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     fc1200_pmon_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = fc1200_pmon_field_<FIELD>_get( NULL, h_ptr, A );
 | fc1200_pmon_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-------------------------------------+
 | | Register 0x00000000 CFG_REG         |
 | +-------------------------------------+
 | | bit  2:1 R/W  INVALID_BLK_TYPE_MODE |
 | | bit  0   R/W  SW_RESET              |
 | +-------------------------------------+
 | 
 | void   fc1200_pmon_field_INVALID_BLK_TYPE_MODE_set       ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pmon_field_INVALID_BLK_TYPE_MODE_get       ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pmon_field_range_INVALID_BLK_TYPE_MODE_set ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc1200_pmon_field_range_INVALID_BLK_TYPE_MODE_get ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   fc1200_pmon_field_SW_RESET_set                    ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pmon_field_SW_RESET_get                    ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------+
 | | Register 0x00000004 PMON_CNT_STAT |
 | +-----------------------------------+
 | | bit  8 R    TIP                   |
 | | bit  0 R/W  PMON_UPDATE           |
 | +-----------------------------------+
 | 
 | void                 fc1200_pmon_field_PMON_UPDATE_set ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc1200_pmon_field_PMON_UPDATE_get ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | UINT32               fc1200_pmon_field_TIP_get         ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pmon_field_TIP_poll        ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------+
 | | Register 0x00000014 DLOLB_EN_REG |
 | +----------------------------------+
 | | bit  4 R/W  FORCE_DLOLB          |
 | | bit  2 R/W  LOS_DLOLB_ENA        |
 | | bit  1 R/W  LOSS_SYNC_DLOLB_ENA  |
 | | bit  0 R/W  HIGH_BER_DLOLB_ENA   |
 | +----------------------------------+
 | 
 | void   fc1200_pmon_field_FORCE_DLOLB_set         ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pmon_field_FORCE_DLOLB_get         ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pmon_field_LOS_DLOLB_ENA_set       ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pmon_field_LOS_DLOLB_ENA_get       ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pmon_field_LOSS_SYNC_DLOLB_ENA_set ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pmon_field_LOSS_SYNC_DLOLB_ENA_get ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pmon_field_HIGH_BER_DLOLB_ENA_set  ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pmon_field_HIGH_BER_DLOLB_ENA_get  ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------------+
 | | Register 0x00000018 PERF_MON_STAT_REG |
 | +---------------------------------------+
 | | bit  3 R  IN_FRAME                    |
 | | bit  2 R  LOS                         |
 | | bit  1 R  LOSS_SYNC                   |
 | | bit  0 R  HIGH_BER                    |
 | +---------------------------------------+
 | 
 | UINT32 fc1200_pmon_field_IN_FRAME_get  ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | UINT32 fc1200_pmon_field_LOS_get       ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | UINT32 fc1200_pmon_field_LOSS_SYNC_get ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | UINT32 fc1200_pmon_field_HIGH_BER_get  ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------------+
 | | Register 0x0000001c VALID_FRAME_CNT_REG |
 | +-----------------------------------------+
 | | bit  31:0 R  VALID_FRM_COUNT            |
 | +-----------------------------------------+
 | 
 | UINT32 fc1200_pmon_field_VALID_FRM_COUNT_get       ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | UINT32 fc1200_pmon_field_range_VALID_FRM_COUNT_get ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000020 INVALID_FRAME_CNT_REG |
 | +-------------------------------------------+
 | | bit  31:0 R  INVLD_FRM_COUNT              |
 | +-------------------------------------------+
 | 
 | UINT32 fc1200_pmon_field_INVLD_FRM_COUNT_get       ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | UINT32 fc1200_pmon_field_range_INVLD_FRM_COUNT_get ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x00000024 VALID_FRAME_BYTE_CNT_REG |
 | +----------------------------------------------+
 | | bit  31:0 R  VALID_FRM_BYTE_COUNT            |
 | +----------------------------------------------+
 | 
 | UINT32 fc1200_pmon_field_VALID_FRM_BYTE_COUNT_get       ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | UINT32 fc1200_pmon_field_range_VALID_FRM_BYTE_COUNT_get ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x00000028 INT_EN_REG |
 | +--------------------------------+
 | | bit  2 R/W  LOS_INT_E          |
 | | bit  1 R/W  LOSS_SYNC_INT_E    |
 | | bit  0 R/W  HIGH_BER_INT_E     |
 | +--------------------------------+
 | 
 | void   fc1200_pmon_field_LOS_INT_E_set       ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pmon_field_LOS_INT_E_get       ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pmon_field_LOSS_SYNC_INT_E_set ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pmon_field_LOSS_SYNC_INT_E_get ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | void   fc1200_pmon_field_HIGH_BER_INT_E_set  ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc1200_pmon_field_HIGH_BER_INT_E_get  ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------------+
 | | Register 0x0000002c INT_INDICATION_REG |
 | +----------------------------------------+
 | | bit  2 R/W  LOS_INT_I                  |
 | | bit  1 R/W  LOSS_SYNC_INT_I            |
 | | bit  0 R/W  HIGH_BER_INT_I             |
 | +----------------------------------------+
 | 
 | void                 fc1200_pmon_field_LOS_INT_I_set_to_clear       ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc1200_pmon_field_LOS_INT_I_get                ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pmon_field_LOS_INT_I_poll               ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc1200_pmon_field_LOSS_SYNC_INT_I_set_to_clear ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc1200_pmon_field_LOSS_SYNC_INT_I_get          ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pmon_field_LOSS_SYNC_INT_I_poll         ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc1200_pmon_field_HIGH_BER_INT_I_set_to_clear  ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc1200_pmon_field_HIGH_BER_INT_I_get           ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pmon_field_HIGH_BER_INT_I_poll          ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------+
 | | Register 0x00000030 INT_STAT |
 | +------------------------------+
 | | bit  2 R  LOS_INT_V          |
 | | bit  1 R  LOSS_SYNC_INT_V    |
 | | bit  0 R  HIGH_BER_INT_V     |
 | +------------------------------+
 | 
 | UINT32               fc1200_pmon_field_LOS_INT_V_get        ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pmon_field_LOS_INT_V_poll       ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               fc1200_pmon_field_LOSS_SYNC_INT_V_get  ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pmon_field_LOSS_SYNC_INT_V_poll ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               fc1200_pmon_field_HIGH_BER_INT_V_get   ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pmon_field_HIGH_BER_INT_V_poll  ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _FC1200_PMON_IO_H
#define _FC1200_PMON_IO_H

#include "fc1200_pmon_api.h"
#include "fc1200_pmon_regs.h"
#include "fc1200_pmon_io_inline.h"


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 fc1200_pmon_buffer_init               ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | void                 fc1200_pmon_buffer_flush              ( fc1200_pmon_buffer_t *b_ptr )
 | UINT32               fc1200_pmon_reg_read                  ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 fc1200_pmon_reg_write                 ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 fc1200_pmon_field_set                 ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 fc1200_pmon_action_on_write_field_set ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 fc1200_pmon_burst_read                ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 fc1200_pmon_burst_write               ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE fc1200_pmon_poll                      ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 fc1200_pmon_reg_CFG_REG_write                                ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc1200_pmon_reg_CFG_REG_field_set                            ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc1200_pmon_reg_CFG_REG_read                                 ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc1200_pmon_reg_DLOLB_EN_REG_write                           ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc1200_pmon_reg_DLOLB_EN_REG_field_set                       ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc1200_pmon_reg_DLOLB_EN_REG_read                            ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc1200_pmon_reg_PMON_CNT_STAT_write                          ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc1200_pmon_reg_PMON_CNT_STAT_field_set                      ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc1200_pmon_reg_PMON_CNT_STAT_read                           ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pmon_reg_PMON_CNT_STAT_poll                           ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 fc1200_pmon_reg_INT_EN_REG_write                             ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc1200_pmon_reg_INT_EN_REG_field_set                         ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc1200_pmon_reg_INT_EN_REG_read                              ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc1200_pmon_reg_INT_INDICATION_REG_write                     ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc1200_pmon_reg_INT_INDICATION_REG_action_on_write_field_set ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc1200_pmon_reg_INT_INDICATION_REG_read                      ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pmon_reg_INT_INDICATION_REG_poll                      ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               fc1200_pmon_reg_INT_STAT_read                                ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc1200_pmon_reg_INT_STAT_poll                                ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               fc1200_pmon_reg_PERF_MON_STAT_REG_read                       ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc1200_pmon_reg_VALID_FRAME_CNT_REG_read                     ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc1200_pmon_reg_INVALID_FRAME_CNT_REG_read                   ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc1200_pmon_reg_VALID_FRAME_BYTE_CNT_REG_read                ( fc1200_pmon_buffer_t *b_ptr, fc1200_pmon_handle_t *h_ptr, UINT32 A )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _FC1200_PMON_IO_H */
