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
 *     and register accessor functions for the cbr_int block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing cbr_int io functions is:
 | {
 |     cbr_int_buffer_t b_ptr[1];
 |     cbr_int_buffer_init( b_ptr, h_ptr );
 |     value = cbr_int_field_<FIELD>_get( b_ptr, h_ptr );
 |     cbr_int_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     cbr_int_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = cbr_int_field_<FIELD>_get( NULL, h_ptr );
 | cbr_int_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------------------------------------+
 | | Register (0x00020000 + (N) * 4) MULTI_LANE_40G_GROUP_CONFIG_REG |
 | |     N[0..2]                                                     |
 | +-----------------------------------------------------------------+
 | | bit  0 R/W  MULTI_LANE_40G_SEL                                  |
 | +-----------------------------------------------------------------+
 | 
 | void   cbr_int_field_MULTI_LANE_40G_SEL_set ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 cbr_int_field_MULTI_LANE_40G_SEL_get ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | +----------------------------------------+
 | | Register 0x0002000c CHANNEL_UPDATE_REG |
 | +----------------------------------------+
 | | bit  0 R/W  CH_UPDATE                  |
 | +----------------------------------------+
 | 
 | void   cbr_int_field_CH_UPDATE_set ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbr_int_field_CH_UPDATE_get ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00020010 + (N) * 4) CHANNELN_CONFIG_REG |
 | |     N[0..11]                                        |
 | +-----------------------------------------------------+
 | | bit  7:4 R/W  CHN_ID                                |
 | | bit  0   R/W  CHN_EN                                |
 | +-----------------------------------------------------+
 | 
 | void   cbr_int_field_CHN_ID_set       ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 cbr_int_field_CHN_ID_get       ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | void   cbr_int_field_range_CHN_ID_set ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbr_int_field_range_CHN_ID_get ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   cbr_int_field_CHN_EN_set       ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 cbr_int_field_CHN_EN_get       ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------------------+
 | | Register 0x00020040 CBR_INTERLEAVER_GENERAL_CONFIG_REG |
 | +--------------------------------------------------------+
 | | bit  0 R/W  CBR_INT_RESET                              |
 | +--------------------------------------------------------+
 | 
 | void   cbr_int_field_CBR_INT_RESET_set ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbr_int_field_CBR_INT_RESET_get ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 +----------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _CBR_INT_IO_H
#define _CBR_INT_IO_H

#include "cbrc_api.h"
#include "cbr_int_regs.h"
#include "cbr_int_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 cbr_int_buffer_init               ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void                 cbr_int_buffer_flush              ( cbr_int_buffer_t *b_ptr )
 | UINT32               cbr_int_reg_read                  ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 cbr_int_reg_write                 ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 cbr_int_field_set                 ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 cbr_int_action_on_write_field_set ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 cbr_int_burst_read                ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 cbr_int_burst_write               ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE cbr_int_poll                      ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   cbr_int_reg_CBR_INTERLEAVER_GENERAL_CONFIG_REG_write        ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void   cbr_int_reg_CBR_INTERLEAVER_GENERAL_CONFIG_REG_field_set    ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cbr_int_reg_CBR_INTERLEAVER_GENERAL_CONFIG_REG_read         ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void   cbr_int_reg_CHANNELN_CONFIG_REG_array_write                 ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   cbr_int_reg_CHANNELN_CONFIG_REG_array_field_set             ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cbr_int_reg_CHANNELN_CONFIG_REG_array_read                  ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | void   cbr_int_reg_MULTI_LANE_40G_GROUP_CONFIG_REG_array_write     ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   cbr_int_reg_MULTI_LANE_40G_GROUP_CONFIG_REG_array_field_set ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cbr_int_reg_MULTI_LANE_40G_GROUP_CONFIG_REG_array_read      ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | void   cbr_int_reg_CHANNEL_UPDATE_REG_write                        ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void   cbr_int_reg_CHANNEL_UPDATE_REG_field_set                    ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cbr_int_reg_CHANNEL_UPDATE_REG_read                         ( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _CBR_INT_IO_H */
