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
 *     and register accessor functions for the cbrc_fc_dlvr block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing cbrc_fc_dlvr io functions is:
 | {
 |     cbrc_fc_dlvr_buffer_t b_ptr[1];
 |     cbrc_fc_dlvr_buffer_init( b_ptr, h_ptr );
 |     value = cbrc_fc_dlvr_field_<FIELD>_get( b_ptr, h_ptr );
 |     cbrc_fc_dlvr_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     cbrc_fc_dlvr_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = cbrc_fc_dlvr_field_<FIELD>_get( NULL, h_ptr );
 | cbrc_fc_dlvr_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-------------------------------------------+
 | | Register 0x00053000 DLVR_TSB_INT_EN_REG_0 |
 | +-------------------------------------------+
 | | bit  27:16 R/W  FC1200_PTD_IRQ_EN0        |
 | | bit  15:4  R/W  FC800_PMG_IRQ_EN0         |
 | | bit  2     R/W  PKTDINT_IRQ_EN0           |
 | | bit  1     R/W  MPMO_IRQ_EN0              |
 | | bit  0     R/W  CBRDINT_IRQ_EN0           |
 | +-------------------------------------------+
 | 
 | void   cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN0_set       ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN0_get       ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN0_set ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN0_get ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN0_set        ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN0_get        ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN0_set  ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN0_get  ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_fc_dlvr_field_PKTDINT_IRQ_EN0_set          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_PKTDINT_IRQ_EN0_get          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_dlvr_field_MPMO_IRQ_EN0_set             ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_MPMO_IRQ_EN0_get             ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_dlvr_field_CBRDINT_IRQ_EN0_set          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_CBRDINT_IRQ_EN0_get          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x00053004 DLVR_TSB_INT_EN_REG_1 |
 | +-------------------------------------------+
 | | bit  27:16 R/W  FC1200_PTD_IRQ_EN1        |
 | | bit  15:4  R/W  FC800_PMG_IRQ_EN1         |
 | | bit  2     R/W  PKTDINT_IRQ_EN1           |
 | | bit  1     R/W  MPMO_IRQ_EN1              |
 | | bit  0     R/W  CBRDINT_IRQ_EN1           |
 | +-------------------------------------------+
 | 
 | void   cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN1_set       ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN1_get       ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN1_set ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN1_get ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN1_set        ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN1_get        ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN1_set  ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN1_get  ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_fc_dlvr_field_PKTDINT_IRQ_EN1_set          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_PKTDINT_IRQ_EN1_get          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_dlvr_field_MPMO_IRQ_EN1_set             ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_MPMO_IRQ_EN1_get             ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_dlvr_field_CBRDINT_IRQ_EN1_set          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_CBRDINT_IRQ_EN1_get          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x00053008 DLVR_TSB_INT_EN_REG_2 |
 | +-------------------------------------------+
 | | bit  27:16 R/W  FC1200_PTD_IRQ_EN2        |
 | | bit  15:4  R/W  FC800_PMG_IRQ_EN2         |
 | | bit  2     R/W  PKTDINT_IRQ_EN2           |
 | | bit  1     R/W  MPMO_IRQ_EN2              |
 | | bit  0     R/W  CBRDINT_IRQ_EN2           |
 | +-------------------------------------------+
 | 
 | void   cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN2_set       ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN2_get       ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN2_set ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN2_get ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN2_set        ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN2_get        ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN2_set  ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN2_get  ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_fc_dlvr_field_PKTDINT_IRQ_EN2_set          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_PKTDINT_IRQ_EN2_get          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_dlvr_field_MPMO_IRQ_EN2_set             ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_MPMO_IRQ_EN2_get             ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_dlvr_field_CBRDINT_IRQ_EN2_set          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_CBRDINT_IRQ_EN2_get          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x0005300c DLVR_TSB_INT_EN_REG_3 |
 | +-------------------------------------------+
 | | bit  27:16 R/W  FC1200_PTD_IRQ_EN3        |
 | | bit  15:4  R/W  FC800_PMG_IRQ_EN3         |
 | | bit  2     R/W  PKTDINT_IRQ_EN3           |
 | | bit  1     R/W  MPMO_IRQ_EN3              |
 | | bit  0     R/W  CBRDINT_IRQ_EN3           |
 | +-------------------------------------------+
 | 
 | void   cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN3_set       ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN3_get       ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN3_set ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN3_get ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN3_set        ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN3_get        ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN3_set  ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN3_get  ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_fc_dlvr_field_PKTDINT_IRQ_EN3_set          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_PKTDINT_IRQ_EN3_get          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_dlvr_field_MPMO_IRQ_EN3_set             ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_MPMO_IRQ_EN3_get             ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_dlvr_field_CBRDINT_IRQ_EN3_set          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_CBRDINT_IRQ_EN3_get          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00053010 + (N) * 0x4) DLVR_CHANNEL_CFG_REG |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  3 R/W  FC800PMG_CH_EN                             |
 | | bit  2 R/W  FCPTD_CH_EN                                |
 | | bit  1 R/W  LPBK_MUX_SEL                               |
 | | bit  0 R/W  FCPTD_FC800PMG_SEL                         |
 | +--------------------------------------------------------+
 | 
 | void   cbrc_fc_dlvr_field_FC800PMG_CH_EN_set     ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_FC800PMG_CH_EN_get     ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | void   cbrc_fc_dlvr_field_FCPTD_CH_EN_set        ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_FCPTD_CH_EN_get        ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | void   cbrc_fc_dlvr_field_LPBK_MUX_SEL_set       ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_LPBK_MUX_SEL_get       ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | void   cbrc_fc_dlvr_field_FCPTD_FC800PMG_SEL_set ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_FCPTD_FC800PMG_SEL_get ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | +------------------------------------------+
 | | Register 0x00053040 DLVR_GLOBAL_CTRL_REG |
 | +------------------------------------------+
 | | bit  4 R/W  CBRDINT_RAM_SD_REG           |
 | | bit  3 R/W  PKTDINT_RAM_SD_REG           |
 | | bit  2 R/W  MPMO_RAM_SD_REG              |
 | | bit  1 R/W  CBR_CH_EN                    |
 | | bit  0 R/W  DLVR_SYNC_RST                |
 | +------------------------------------------+
 | 
 | void   cbrc_fc_dlvr_field_CBRDINT_RAM_SD_REG_set ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_CBRDINT_RAM_SD_REG_get ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_dlvr_field_PKTDINT_RAM_SD_REG_set ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_PKTDINT_RAM_SD_REG_get ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_dlvr_field_MPMO_RAM_SD_REG_set    ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_MPMO_RAM_SD_REG_get    ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_dlvr_field_CBR_CH_EN_set          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_CBR_CH_EN_get          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_dlvr_field_DLVR_SYNC_RST_set      ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_dlvr_field_DLVR_SYNC_RST_get      ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _CBRC_FC_DLVR_IO_H
#define _CBRC_FC_DLVR_IO_H

#include "cbrc_api.h"
#include "cbrc_fc_dlvr_regs.h"
#include "cbrc_fc_dlvr_io_inline.h"


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 cbrc_fc_dlvr_buffer_init               ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void                 cbrc_fc_dlvr_buffer_flush              ( cbrc_fc_dlvr_buffer_t *b_ptr )
 | UINT32               cbrc_fc_dlvr_reg_read                  ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 cbrc_fc_dlvr_reg_write                 ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 cbrc_fc_dlvr_field_set                 ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 cbrc_fc_dlvr_action_on_write_field_set ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 cbrc_fc_dlvr_burst_read                ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 cbrc_fc_dlvr_burst_write               ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE cbrc_fc_dlvr_poll                      ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_write          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void   cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_field_set      ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_read           ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void   cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_write          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void   cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_field_set      ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_read           ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void   cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_write          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void   cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_field_set      ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_read           ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void   cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_write          ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void   cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_field_set      ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_read           ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void   cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_write           ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void   cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_field_set       ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_read            ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void   cbrc_fc_dlvr_reg_DLVR_CHANNEL_CFG_REG_array_write     ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   cbrc_fc_dlvr_reg_DLVR_CHANNEL_CFG_REG_array_field_set ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cbrc_fc_dlvr_reg_DLVR_CHANNEL_CFG_REG_array_read      ( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _CBRC_FC_DLVR_IO_H */
