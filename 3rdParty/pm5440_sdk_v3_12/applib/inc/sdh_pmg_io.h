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
 *     and register accessor functions for the sdh_pmg block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing sdh_pmg io functions is:
 | {
 |     sdh_pmg_buffer_t b_ptr[1];
 |     sdh_pmg_buffer_init( b_ptr, h_ptr );
 |     value = sdh_pmg_field_<FIELD>_get( b_ptr, h_ptr );
 |     sdh_pmg_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     sdh_pmg_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = sdh_pmg_field_<FIELD>_get( NULL, h_ptr );
 | sdh_pmg_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------------+
 | | Register 0x00000000 TOP_CFG_REG |
 | +---------------------------------+
 | | bit  12 R/W  RRMP_U_ASCLK       |
 | | bit  9  R/W  LBIP_BER_SEL       |
 | | bit  0  R/W  OC_768_MODE        |
 | +---------------------------------+
 | 
 | void   sdh_pmg_field_LBIP_BER_SEL_set ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 value )
 | UINT32 sdh_pmg_field_LBIP_BER_SEL_get ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
 | void   sdh_pmg_field_OC_768_MODE_set  ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 value )
 | UINT32 sdh_pmg_field_OC_768_MODE_get  ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
 | void   sdh_pmg_field_RRMP_U_ASCLK_set ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 value )
 | UINT32 sdh_pmg_field_RRMP_U_ASCLK_get ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
 | 
 | +----------------------------------+
 | | Register 0x00000004 TOP_SW_RESET |
 | +----------------------------------+
 | | bit  0 R/W  TOP_SW_RST           |
 | +----------------------------------+
 | 
 | void   sdh_pmg_field_TOP_SW_RST_set ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 value )
 | UINT32 sdh_pmg_field_TOP_SW_RST_get ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
 | 
 | +--------------------------------------------------------------+
 | | Register (0x00000008 + (N) * 0x4) CHANNEL_LOW_POWER_SW_RESET |
 | |     N[0..3]                                                  |
 | +--------------------------------------------------------------+
 | | bit  10 R/W  CH_SBER_U_SW_RST                                |
 | | bit  9  R/W  CH_RTTP_U_SW_RST                                |
 | | bit  8  R/W  CH_RRMP_U_SW_RST                                |
 | | bit  0  R/W  CH_LOW_PWR                                      |
 | +--------------------------------------------------------------+
 | 
 | void   sdh_pmg_field_CH_SBER_U_SW_RST_set ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sdh_pmg_field_CH_SBER_U_SW_RST_get ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32  N )
 | void   sdh_pmg_field_CH_RTTP_U_SW_RST_set ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sdh_pmg_field_CH_RTTP_U_SW_RST_get ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32  N )
 | void   sdh_pmg_field_CH_RRMP_U_SW_RST_set ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sdh_pmg_field_CH_RRMP_U_SW_RST_get ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32  N )
 | void   sdh_pmg_field_CH_LOW_PWR_set       ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sdh_pmg_field_CH_LOW_PWR_get       ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32  N )
 | 
 | +---------------------------------------------------+
 | | Register (0x00000018 + (N) * 0x4) CHANNEL_CFG_REG |
 | |     N[0..3]                                       |
 | +---------------------------------------------------+
 | | bit  9 R/W  LOF_PN11_INSERT                       |
 | | bit  8 R/W  LOS_PN11_INSERT                       |
 | | bit  0 R/W  CHNL_EN                               |
 | +---------------------------------------------------+
 | 
 | void   sdh_pmg_field_LOF_PN11_INSERT_set ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sdh_pmg_field_LOF_PN11_INSERT_get ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32  N )
 | void   sdh_pmg_field_LOS_PN11_INSERT_set ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sdh_pmg_field_LOS_PN11_INSERT_get ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32  N )
 | void   sdh_pmg_field_CHNL_EN_set         ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sdh_pmg_field_CHNL_EN_get         ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32  N )
 | 
 | +---------------------------------+
 | | Register 0x00000028 MTSB_INT_EN |
 | +---------------------------------+
 | | bit  11:8 R/W  SBER_U_INT_EN    |
 | | bit  7:4  R/W  RTTP_U_INT_EN    |
 | | bit  3:0  R/W  RRMP_U_INT_EN    |
 | +---------------------------------+
 | 
 | void   sdh_pmg_field_SBER_U_INT_EN_set       ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 value )
 | UINT32 sdh_pmg_field_SBER_U_INT_EN_get       ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
 | void   sdh_pmg_field_range_SBER_U_INT_EN_set ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sdh_pmg_field_range_SBER_U_INT_EN_get ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sdh_pmg_field_RTTP_U_INT_EN_set       ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 value )
 | UINT32 sdh_pmg_field_RTTP_U_INT_EN_get       ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
 | void   sdh_pmg_field_range_RTTP_U_INT_EN_set ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sdh_pmg_field_range_RTTP_U_INT_EN_get ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sdh_pmg_field_RRMP_U_INT_EN_set       ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 value )
 | UINT32 sdh_pmg_field_RRMP_U_INT_EN_get       ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
 | void   sdh_pmg_field_range_RRMP_U_INT_EN_set ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sdh_pmg_field_range_RRMP_U_INT_EN_get ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x0000002c INT_SUMMARY |
 | +---------------------------------+
 | | bit  11:8 R  SBER_U_INT         |
 | | bit  7:4  R  RTTP_U_INT         |
 | | bit  3:0  R  RRMP_U_INT         |
 | +---------------------------------+
 | 
 | UINT32 sdh_pmg_field_SBER_U_INT_get       ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
 | UINT32 sdh_pmg_field_range_SBER_U_INT_get ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 sdh_pmg_field_RTTP_U_INT_get       ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
 | UINT32 sdh_pmg_field_range_RTTP_U_INT_get ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 sdh_pmg_field_RRMP_U_INT_get       ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
 | UINT32 sdh_pmg_field_range_RRMP_U_INT_get ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00000030 TIP_SUMMARY |
 | +---------------------------------+
 | | bit  15:0 R  RRMP_U_TIP         |
 | +---------------------------------+
 | 
 | UINT32               sdh_pmg_field_RRMP_U_TIP_get        ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
 | UINT32               sdh_pmg_field_range_RRMP_U_TIP_get  ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE sdh_pmg_field_range_RRMP_U_TIP_poll ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE sdh_pmg_field_RRMP_U_TIP_poll       ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _SDH_PMG_IO_H
#define _SDH_PMG_IO_H

#include "sdh_pmg_api.h"
#include "sdh_pmg_regs.h"
#include "sdh_pmg_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sdh_pmg_buffer_init               ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
 | void                 sdh_pmg_buffer_flush              ( sdh_pmg_buffer_t *b_ptr )
 | UINT32               sdh_pmg_reg_read                  ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 sdh_pmg_reg_write                 ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 sdh_pmg_field_set                 ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 sdh_pmg_action_on_write_field_set ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 sdh_pmg_burst_read                ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 sdh_pmg_burst_write               ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE sdh_pmg_poll                      ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sdh_pmg_reg_TOP_CFG_REG_write                          ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 value )
 | void                 sdh_pmg_reg_TOP_CFG_REG_field_set                      ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sdh_pmg_reg_TOP_CFG_REG_read                           ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
 | 
 | void                 sdh_pmg_reg_TOP_SW_RESET_write                         ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 value )
 | void                 sdh_pmg_reg_TOP_SW_RESET_field_set                     ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sdh_pmg_reg_TOP_SW_RESET_read                          ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
 | 
 | void                 sdh_pmg_reg_MTSB_INT_EN_write                          ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 value )
 | void                 sdh_pmg_reg_MTSB_INT_EN_field_set                      ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sdh_pmg_reg_MTSB_INT_EN_read                           ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
 | 
 | void                 sdh_pmg_reg_CHANNEL_LOW_POWER_SW_RESET_array_write     ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 sdh_pmg_reg_CHANNEL_LOW_POWER_SW_RESET_array_field_set ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sdh_pmg_reg_CHANNEL_LOW_POWER_SW_RESET_array_read      ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32  N )
 | 
 | void                 sdh_pmg_reg_CHANNEL_CFG_REG_array_write                ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 sdh_pmg_reg_CHANNEL_CFG_REG_array_field_set            ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sdh_pmg_reg_CHANNEL_CFG_REG_array_read                 ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               sdh_pmg_reg_INT_SUMMARY_read                           ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
 | 
 | UINT32               sdh_pmg_reg_TIP_SUMMARY_read                           ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sdh_pmg_reg_TIP_SUMMARY_poll                           ( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _SDH_PMG_IO_H */
