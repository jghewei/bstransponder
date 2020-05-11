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
 *     and register accessor functions for the fc_tttd block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing fc_tttd io functions is:
 | {
 |     fc_tttd_buffer_t b_ptr[1];
 |     fc_tttd_buffer_init( b_ptr, h_ptr, A );
 |     value = fc_tttd_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     fc_tttd_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     fc_tttd_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = fc_tttd_field_<FIELD>_get( NULL, h_ptr, A );
 | fc_tttd_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +--------------------------+
 | | Register 0x00050100 CFG  |
 | +--------------------------+
 | | bit  25  R/W  FORCE_LF   |
 | | bit  9:8 R/W  ZONE_THRES |
 | | bit  0   R/W  SW_RESET   |
 | +--------------------------+
 | 
 | void   fc_tttd_field_FORCE_LF_set         ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttd_field_FORCE_LF_get         ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttd_field_ZONE_THRES_set       ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttd_field_ZONE_THRES_get       ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttd_field_range_ZONE_THRES_set ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_tttd_field_range_ZONE_THRES_get ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   fc_tttd_field_SW_RESET_set         ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttd_field_SW_RESET_get         ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------+
 | | Register 0x00050104 INT   |
 | +---------------------------+
 | | bit  2 R/W  INVALBLKHDR_I |
 | | bit  1 R/W  UNDERRUN_I    |
 | | bit  0 R/W  CRC24ERR_I    |
 | +---------------------------+
 | 
 | void                 fc_tttd_field_INVALBLKHDR_I_set_to_clear ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_tttd_field_INVALBLKHDR_I_get          ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_tttd_field_INVALBLKHDR_I_poll         ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc_tttd_field_UNDERRUN_I_set_to_clear    ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_tttd_field_UNDERRUN_I_get             ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_tttd_field_UNDERRUN_I_poll            ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc_tttd_field_CRC24ERR_I_set_to_clear    ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_tttd_field_CRC24ERR_I_get             ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_tttd_field_CRC24ERR_I_poll            ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------+
 | | Register 0x00050108 INT_EN |
 | +----------------------------+
 | | bit  2 R/W  INVALBLKHDR_E  |
 | | bit  1 R/W  UNDERRUN_E     |
 | | bit  0 R/W  CRC24ERR_E     |
 | +----------------------------+
 | 
 | void   fc_tttd_field_INVALBLKHDR_E_set ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttd_field_INVALBLKHDR_E_get ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttd_field_UNDERRUN_E_set    ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttd_field_UNDERRUN_E_get    ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttd_field_CRC24ERR_E_set    ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttd_field_CRC24ERR_E_get    ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | +-------------------------------------------------+
 | | Register 0x0005010c LOCAL_FAULT_ORDERED_SET_LSB |
 | +-------------------------------------------------+
 | | bit  31:0 R/W  LFOS                             |
 | +-------------------------------------------------+
 | 
 | void   fc_tttd_lfield_LFOS_set       ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value[2] )
 | void   fc_tttd_lfield_LFOS_get       ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value[2] )
 | void   fc_tttd_lfield_range_LFOS_set ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_tttd_lfield_range_LFOS_get ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00050118 CRC_24_ERR_CNT |
 | +------------------------------------+
 | | bit  15:0 R  CRCERRCNT             |
 | +------------------------------------+
 | 
 | UINT32 fc_tttd_field_CRCERRCNT_get       ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 fc_tttd_field_range_CRCERRCNT_get ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x0005011c INVALID_BLOCK_HEADER_CNT |
 | +----------------------------------------------+
 | | bit  15:0 R  INVALBLKHDRCNT                  |
 | +----------------------------------------------+
 | 
 | UINT32 fc_tttd_field_INVALBLKHDRCNT_get       ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 fc_tttd_field_range_INVALBLKHDRCNT_get ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x00050120 CNT_UPDATE |
 | +--------------------------------+
 | | bit  31 R    TIP               |
 | | bit  0  R/W  CNTR_UPDATE       |
 | +--------------------------------+
 | 
 | void                 fc_tttd_field_CNTR_UPDATE_set ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_tttd_field_CNTR_UPDATE_get ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32               fc_tttd_field_TIP_get         ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_tttd_field_TIP_poll        ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _FC_TTTD_IO_H
#define _FC_TTTD_IO_H

#include "cbrc_api.h"
#include "fc_tttd_regs.h"
#include "fc_tttd_io_inline.h"


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 fc_tttd_buffer_init               ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void                 fc_tttd_buffer_flush              ( fc_tttd_buffer_t *b_ptr )
 | UINT32               fc_tttd_reg_read                  ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 fc_tttd_reg_write                 ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 fc_tttd_field_set                 ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 fc_tttd_action_on_write_field_set ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 fc_tttd_burst_read                ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 fc_tttd_burst_write               ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE fc_tttd_poll                      ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 fc_tttd_reg_CFG_write                             ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttd_reg_CFG_field_set                         ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttd_reg_CFG_read                              ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_LSB_write     ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_LSB_field_set ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_LSB_read      ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_MSB_write     ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_MSB_field_set ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_MSB_read      ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_tttd_reg_CNT_UPDATE_write                      ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttd_reg_CNT_UPDATE_field_set                  ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttd_reg_CNT_UPDATE_read                       ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_tttd_reg_CNT_UPDATE_poll                       ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 fc_tttd_reg_INT_EN_write                          ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttd_reg_INT_EN_field_set                      ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttd_reg_INT_EN_read                           ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_tttd_reg_INT_write                             ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttd_reg_INT_action_on_write_field_set         ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttd_reg_INT_read                              ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_tttd_reg_INT_poll                              ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               fc_tttd_reg_CRC_24_ERR_CNT_read                   ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc_tttd_reg_INVALID_BLOCK_HEADER_CNT_read         ( fc_tttd_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _FC_TTTD_IO_H */
