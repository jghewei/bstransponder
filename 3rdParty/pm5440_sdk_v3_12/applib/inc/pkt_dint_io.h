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
 *     and register accessor functions for the pkt_dint block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing pkt_dint io functions is:
 | {
 |     pkt_dint_buffer_t b_ptr[1];
 |     pkt_dint_buffer_init( b_ptr, h_ptr );
 |     value = pkt_dint_field_<FIELD>_get( b_ptr, h_ptr );
 |     pkt_dint_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     pkt_dint_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = pkt_dint_field_<FIELD>_get( NULL, h_ptr );
 | pkt_dint_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +------------------------------------------------+
 | | Register (0x00040400 + (N) * 4) CHANNEL_ID_REG |
 | |     N[0..11]                                   |
 | +------------------------------------------------+
 | | bit  3:0 R/W  ID                               |
 | +------------------------------------------------+
 | 
 | void   pkt_dint_field_ID_set       ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 pkt_dint_field_ID_get       ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | void   pkt_dint_field_range_ID_set ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 pkt_dint_field_range_ID_get ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00040430 CHANNEL_ID_UPDATE_REG |
 | +-------------------------------------------+
 | | bit  0 R/W  ID_UPDATE                     |
 | +-------------------------------------------+
 | 
 | void   pkt_dint_field_ID_UPDATE_set ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 pkt_dint_field_ID_UPDATE_get ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register (0x00040434 + (N) * 4) ZONE_CFG_REG |
 | |     N[0..11]                                 |
 | +----------------------------------------------+
 | | bit  7:6 R/W  ZONE_XOFF_SET_THRESHOLD        |
 | | bit  5:4 R/W  ZONE_XOFF_CLEAR_THRESHOLD      |
 | | bit  0   R/W  UF_ZONE_MODE                   |
 | +----------------------------------------------+
 | 
 | void   pkt_dint_field_ZONE_XOFF_SET_THRESHOLD_set         ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 pkt_dint_field_ZONE_XOFF_SET_THRESHOLD_get         ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | void   pkt_dint_field_range_ZONE_XOFF_SET_THRESHOLD_set   ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 pkt_dint_field_range_ZONE_XOFF_SET_THRESHOLD_get   ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   pkt_dint_field_ZONE_XOFF_CLEAR_THRESHOLD_set       ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 pkt_dint_field_ZONE_XOFF_CLEAR_THRESHOLD_get       ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | void   pkt_dint_field_range_ZONE_XOFF_CLEAR_THRESHOLD_set ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 pkt_dint_field_range_ZONE_XOFF_CLEAR_THRESHOLD_get ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   pkt_dint_field_UF_ZONE_MODE_set                    ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 pkt_dint_field_UF_ZONE_MODE_get                    ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | +-------------------------------------------------+
 | | Register (0x00040464 + (N) * 4) CFC_XOFF_TH_REG |
 | |     N[0..11]                                    |
 | +-------------------------------------------------+
 | | bit  3:0 R/W  CFC_XOFF_THRESHOLD                |
 | +-------------------------------------------------+
 | 
 | void   pkt_dint_field_CFC_XOFF_THRESHOLD_set       ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 pkt_dint_field_CFC_XOFF_THRESHOLD_get       ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | void   pkt_dint_field_range_CFC_XOFF_THRESHOLD_set ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 pkt_dint_field_range_CFC_XOFF_THRESHOLD_get ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00040494 CFC_FIFO_INT_REG |
 | +--------------------------------------+
 | | bit  23:12 R/W  CFC_FIFO_UDR_I       |
 | | bit  11:0  R/W  CFC_FIFO_OVR_I       |
 | +--------------------------------------+
 | 
 | void                 pkt_dint_field_CFC_FIFO_UDR_I_set_to_clear       ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               pkt_dint_field_CFC_FIFO_UDR_I_get                ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void                 pkt_dint_field_range_CFC_FIFO_UDR_I_set_to_clear ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               pkt_dint_field_range_CFC_FIFO_UDR_I_get          ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE pkt_dint_field_range_CFC_FIFO_UDR_I_poll         ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE pkt_dint_field_CFC_FIFO_UDR_I_poll               ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 pkt_dint_field_CFC_FIFO_OVR_I_set_to_clear       ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               pkt_dint_field_CFC_FIFO_OVR_I_get                ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void                 pkt_dint_field_range_CFC_FIFO_OVR_I_set_to_clear ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               pkt_dint_field_range_CFC_FIFO_OVR_I_get          ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE pkt_dint_field_range_CFC_FIFO_OVR_I_poll         ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE pkt_dint_field_CFC_FIFO_OVR_I_poll               ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------+
 | | Register 0x00040498 CFC_FIFO_INT_EN_REG |
 | +-----------------------------------------+
 | | bit  23:12 R/W  CFC_FIFO_UDR_E          |
 | | bit  11:0  R/W  CFC_FIFO_OVR_E          |
 | +-----------------------------------------+
 | 
 | void   pkt_dint_field_CFC_FIFO_UDR_E_set       ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 pkt_dint_field_CFC_FIFO_UDR_E_get       ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   pkt_dint_field_range_CFC_FIFO_UDR_E_set ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 pkt_dint_field_range_CFC_FIFO_UDR_E_get ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   pkt_dint_field_CFC_FIFO_OVR_E_set       ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 pkt_dint_field_CFC_FIFO_OVR_E_get       ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   pkt_dint_field_range_CFC_FIFO_OVR_E_set ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 pkt_dint_field_range_CFC_FIFO_OVR_E_get ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x0004049c CFC_FIFO_INT_STAT_REG |
 | +-------------------------------------------+
 | | bit  23:12 R  CFC_FIFO_UDR_V              |
 | | bit  11:0  R  CFC_FIFO_OVR_V              |
 | +-------------------------------------------+
 | 
 | UINT32               pkt_dint_field_CFC_FIFO_UDR_V_get        ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | UINT32               pkt_dint_field_range_CFC_FIFO_UDR_V_get  ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE pkt_dint_field_range_CFC_FIFO_UDR_V_poll ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE pkt_dint_field_CFC_FIFO_UDR_V_poll       ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               pkt_dint_field_CFC_FIFO_OVR_V_get        ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | UINT32               pkt_dint_field_range_CFC_FIFO_OVR_V_get  ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE pkt_dint_field_range_CFC_FIFO_OVR_V_poll ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE pkt_dint_field_CFC_FIFO_OVR_V_poll       ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------------+
 | | Register 0x000404b0 PACKET_DEINTERLEAVER_SW_RESET |
 | +---------------------------------------------------+
 | | bit  0 R/W  SOFT_RST                              |
 | +---------------------------------------------------+
 | 
 | void   pkt_dint_field_SOFT_RST_set ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 pkt_dint_field_SOFT_RST_get ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _PKT_DINT_IO_H
#define _PKT_DINT_IO_H

#include "cbrc_api.h"
#include "pkt_dint_regs.h"
#include "pkt_dint_io_inline.h"


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 pkt_dint_buffer_init               ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void                 pkt_dint_buffer_flush              ( pkt_dint_buffer_t *b_ptr )
 | UINT32               pkt_dint_reg_read                  ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 pkt_dint_reg_write                 ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 pkt_dint_field_set                 ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 pkt_dint_action_on_write_field_set ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 pkt_dint_burst_read                ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 pkt_dint_burst_write               ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE pkt_dint_poll                      ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 pkt_dint_reg_PACKET_DEINTERLEAVER_SW_RESET_write        ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 pkt_dint_reg_PACKET_DEINTERLEAVER_SW_RESET_field_set    ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               pkt_dint_reg_PACKET_DEINTERLEAVER_SW_RESET_read         ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void                 pkt_dint_reg_CHANNEL_ID_REG_array_write                 ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 pkt_dint_reg_CHANNEL_ID_REG_array_field_set             ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               pkt_dint_reg_CHANNEL_ID_REG_array_read                  ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | void                 pkt_dint_reg_ZONE_CFG_REG_array_write                   ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 pkt_dint_reg_ZONE_CFG_REG_array_field_set               ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               pkt_dint_reg_ZONE_CFG_REG_array_read                    ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | void                 pkt_dint_reg_CFC_XOFF_TH_REG_array_write                ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 pkt_dint_reg_CFC_XOFF_TH_REG_array_field_set            ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               pkt_dint_reg_CFC_XOFF_TH_REG_array_read                 ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | void                 pkt_dint_reg_CHANNEL_ID_UPDATE_REG_write                ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 pkt_dint_reg_CHANNEL_ID_UPDATE_REG_field_set            ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               pkt_dint_reg_CHANNEL_ID_UPDATE_REG_read                 ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void                 pkt_dint_reg_CFC_FIFO_INT_EN_REG_write                  ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 pkt_dint_reg_CFC_FIFO_INT_EN_REG_field_set              ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               pkt_dint_reg_CFC_FIFO_INT_EN_REG_read                   ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void                 pkt_dint_reg_CFC_FIFO_INT_REG_write                     ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 pkt_dint_reg_CFC_FIFO_INT_REG_action_on_write_field_set ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               pkt_dint_reg_CFC_FIFO_INT_REG_read                      ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE pkt_dint_reg_CFC_FIFO_INT_REG_poll                      ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               pkt_dint_reg_CFC_FIFO_INT_STAT_REG_read                 ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE pkt_dint_reg_CFC_FIFO_INT_STAT_REG_poll                 ( pkt_dint_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _PKT_DINT_IO_H */
