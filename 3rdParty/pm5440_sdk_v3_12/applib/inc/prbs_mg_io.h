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
 *     and register accessor functions for the prbs_mg block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing prbs_mg io functions is:
 | {
 |     prbs_mg_buffer_t b_ptr[1];
 |     prbs_mg_buffer_init( b_ptr, h_ptr );
 |     value = prbs_mg_field_<FIELD>_get( b_ptr, h_ptr );
 |     prbs_mg_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     prbs_mg_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = prbs_mg_field_<FIELD>_get( NULL, h_ptr );
 | prbs_mg_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------------+
 | | Register 0x00001000 TOP_CFG_REG |
 | +---------------------------------+
 | | bit  2 R/W  PRBS_MG_RX_CHNL_EN  |
 | | bit  1 R/W  PRBS_MG_TX_CHNL_EN  |
 | | bit  0 R/W  OC_768_MODE         |
 | +---------------------------------+
 | 
 | void   prbs_mg_field_PRBS_MG_RX_CHNL_EN_set ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 prbs_mg_field_PRBS_MG_RX_CHNL_EN_get ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   prbs_mg_field_PRBS_MG_TX_CHNL_EN_set ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 prbs_mg_field_PRBS_MG_TX_CHNL_EN_get ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   prbs_mg_field_OC_768_MODE_set        ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 prbs_mg_field_OC_768_MODE_get        ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | +-----------------------------------+
 | | Register 0x00001004 SW_RESET_REG_ |
 | +-----------------------------------+
 | | bit  1 R/W  PRBS_RX_SW_RST        |
 | | bit  0 R/W  PRBS_TX_SW_RST        |
 | +-----------------------------------+
 | 
 | void   prbs_mg_field_PRBS_RX_SW_RST_set ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 prbs_mg_field_PRBS_RX_SW_RST_get ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   prbs_mg_field_PRBS_TX_SW_RST_set ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 prbs_mg_field_PRBS_TX_SW_RST_get ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | +-------------------------------------+
 | | Register 0x00001008 ASCLK_PULSE_REG |
 | +-------------------------------------+
 | | bit  0 R/W  ASCLK_PULSE             |
 | +-------------------------------------+
 | 
 | void   prbs_mg_field_ASCLK_PULSE_set ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 prbs_mg_field_ASCLK_PULSE_get ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x0000100c LOW_POWER_CFG_ |
 | +------------------------------------+
 | | bit  1 R/W  PRBS_RX_LOW_PWR        |
 | | bit  0 R/W  PRBS_TX_LOW_PWR        |
 | +------------------------------------+
 | 
 | void   prbs_mg_field_PRBS_RX_LOW_PWR_set ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 prbs_mg_field_PRBS_RX_LOW_PWR_get ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   prbs_mg_field_PRBS_TX_LOW_PWR_set ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 prbs_mg_field_PRBS_TX_LOW_PWR_get ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | +---------------------------------+
 | | Register 0x00001010 MTSB_INT_EN |
 | +---------------------------------+
 | | bit  7:4 R/W  SHPI_U_INT_EN     |
 | | bit  3:0 R/W  PRGM_INT_EN       |
 | +---------------------------------+
 | 
 | void   prbs_mg_field_SHPI_U_INT_EN_set       ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 prbs_mg_field_SHPI_U_INT_EN_get       ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   prbs_mg_field_range_SHPI_U_INT_EN_set ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 prbs_mg_field_range_SHPI_U_INT_EN_get ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   prbs_mg_field_PRGM_INT_EN_set         ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 prbs_mg_field_PRGM_INT_EN_get         ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   prbs_mg_field_range_PRGM_INT_EN_set   ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 prbs_mg_field_range_PRGM_INT_EN_get   ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00001014 INT_SUMMARY |
 | +---------------------------------+
 | | bit  7:4 R  SHPI_U_INT          |
 | | bit  3:0 R  PRGM_INT            |
 | +---------------------------------+
 | 
 | UINT32 prbs_mg_field_SHPI_U_INT_get       ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | UINT32 prbs_mg_field_range_SHPI_U_INT_get ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 prbs_mg_field_PRGM_INT_get         ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | UINT32 prbs_mg_field_range_PRGM_INT_get   ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00001018 TIP_SUMMARY |
 | +---------------------------------+
 | | bit  7:4 R  SHPI_U_TIP          |
 | | bit  3:0 R  PRGM_TIP            |
 | +---------------------------------+
 | 
 | UINT32               prbs_mg_field_SHPI_U_TIP_get        ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | UINT32               prbs_mg_field_range_SHPI_U_TIP_get  ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE prbs_mg_field_range_SHPI_U_TIP_poll ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE prbs_mg_field_SHPI_U_TIP_poll       ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               prbs_mg_field_PRGM_TIP_get          ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | UINT32               prbs_mg_field_range_PRGM_TIP_get    ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE prbs_mg_field_range_PRGM_TIP_poll   ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE prbs_mg_field_PRGM_TIP_poll         ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _PRBS_MG_IO_H
#define _PRBS_MG_IO_H

#include "cbrc_api.h"
#include "prbs_mg_regs.h"
#include "prbs_mg_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 prbs_mg_buffer_init               ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void                 prbs_mg_buffer_flush              ( prbs_mg_buffer_t *b_ptr )
 | UINT32               prbs_mg_reg_read                  ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 prbs_mg_reg_write                 ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 prbs_mg_field_set                 ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 prbs_mg_action_on_write_field_set ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 prbs_mg_burst_read                ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 prbs_mg_burst_write               ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE prbs_mg_poll                      ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 prbs_mg_reg_TOP_CFG_REG_write         ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 prbs_mg_reg_TOP_CFG_REG_field_set     ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               prbs_mg_reg_TOP_CFG_REG_read          ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void                 prbs_mg_reg_SW_RESET_REG__write       ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 prbs_mg_reg_SW_RESET_REG__field_set   ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               prbs_mg_reg_SW_RESET_REG__read        ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void                 prbs_mg_reg_LOW_POWER_CFG__write      ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 prbs_mg_reg_LOW_POWER_CFG__field_set  ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               prbs_mg_reg_LOW_POWER_CFG__read       ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void                 prbs_mg_reg_MTSB_INT_EN_write         ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 prbs_mg_reg_MTSB_INT_EN_field_set     ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               prbs_mg_reg_MTSB_INT_EN_read          ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void                 prbs_mg_reg_ASCLK_PULSE_REG_write     ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 prbs_mg_reg_ASCLK_PULSE_REG_field_set ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               prbs_mg_reg_ASCLK_PULSE_REG_read      ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | UINT32               prbs_mg_reg_INT_SUMMARY_read          ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | UINT32               prbs_mg_reg_TIP_SUMMARY_read          ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE prbs_mg_reg_TIP_SUMMARY_poll          ( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _PRBS_MG_IO_H */
