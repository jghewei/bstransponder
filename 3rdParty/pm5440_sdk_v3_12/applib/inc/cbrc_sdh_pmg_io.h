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
 *     and register accessor functions for the cbrc_sdh_pmg block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing cbrc_sdh_pmg io functions is:
 | {
 |     cbrc_sdh_pmg_buffer_t b_ptr[1];
 |     cbrc_sdh_pmg_buffer_init( b_ptr, h_ptr, A );
 |     value = cbrc_sdh_pmg_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     cbrc_sdh_pmg_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     cbrc_sdh_pmg_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = cbrc_sdh_pmg_field_<FIELD>_get( NULL, h_ptr, A );
 | cbrc_sdh_pmg_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------+
 | | Register 0x00002000 CFG           |
 | +-----------------------------------+
 | | bit  10  R/W  SDH_PMG_2_LOWPWR    |
 | | bit  9   R/W  SDH_PMG_1_LOWPWR    |
 | | bit  8   R/W  SDH_PMG_0_LOWPWR    |
 | | bit  7   R/W  SDH_PMG_2_SW_RST    |
 | | bit  6   R/W  SDH_PMG_1_SW_RST    |
 | | bit  5   R/W  SDH_PMG_0_SW_RST    |
 | | bit  4   R/W  CBRC_SDH_PMG_SW_RST |
 | | bit  3:0 R/W  PRBS_MON_CH_SEL     |
 | +-----------------------------------+
 | 
 | void   cbrc_sdh_pmg_field_SDH_PMG_2_LOWPWR_set      ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_LOWPWR_get      ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sdh_pmg_field_SDH_PMG_1_LOWPWR_set      ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_LOWPWR_get      ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sdh_pmg_field_SDH_PMG_0_LOWPWR_set      ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_LOWPWR_get      ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sdh_pmg_field_SDH_PMG_2_SW_RST_set      ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_SW_RST_get      ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sdh_pmg_field_SDH_PMG_1_SW_RST_set      ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_SW_RST_get      ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sdh_pmg_field_SDH_PMG_0_SW_RST_set      ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_SW_RST_get      ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sdh_pmg_field_CBRC_SDH_PMG_SW_RST_set   ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_CBRC_SDH_PMG_SW_RST_get   ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sdh_pmg_field_PRBS_MON_CH_SEL_set       ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_PRBS_MON_CH_SEL_get       ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sdh_pmg_field_range_PRBS_MON_CH_SEL_set ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_range_PRBS_MON_CH_SEL_get ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00002004 INT_SUMMARY |
 | +---------------------------------+
 | | bit  2 R  SDH_PMG_2_INT         |
 | | bit  1 R  SDH_PMG_1_INT         |
 | | bit  0 R  SDH_PMG_0_INT         |
 | +---------------------------------+
 | 
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_INT_get ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_INT_get ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_INT_get ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------+
 | | Register 0x00002008 INT_PIN_EN |
 | +--------------------------------+
 | | bit  11 R/W  SDH_PMG_2_S_E     |
 | | bit  10 R/W  SDH_PMG_1_S_E     |
 | | bit  9  R/W  SDH_PMG_0_S_E     |
 | | bit  8  R/W  SDH_PMG_2_D_E     |
 | | bit  7  R/W  SDH_PMG_1_D_E     |
 | | bit  6  R/W  SDH_PMG_0_D_E     |
 | | bit  5  R/W  SDH_PMG_2_M_E     |
 | | bit  4  R/W  SDH_PMG_1_M_E     |
 | | bit  3  R/W  SDH_PMG_0_M_E     |
 | | bit  2  R/W  SDH_PMG_2_P_E     |
 | | bit  1  R/W  SDH_PMG_1_P_E     |
 | | bit  0  R/W  SDH_PMG_0_P_E     |
 | +--------------------------------+
 | 
 | void   cbrc_sdh_pmg_field_SDH_PMG_2_S_E_set ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_S_E_get ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sdh_pmg_field_SDH_PMG_1_S_E_set ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_S_E_get ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sdh_pmg_field_SDH_PMG_0_S_E_set ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_S_E_get ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sdh_pmg_field_SDH_PMG_2_D_E_set ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_D_E_get ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sdh_pmg_field_SDH_PMG_1_D_E_set ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_D_E_get ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sdh_pmg_field_SDH_PMG_0_D_E_set ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_D_E_get ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sdh_pmg_field_SDH_PMG_2_M_E_set ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_M_E_get ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sdh_pmg_field_SDH_PMG_1_M_E_set ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_M_E_get ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sdh_pmg_field_SDH_PMG_0_M_E_set ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_M_E_get ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sdh_pmg_field_SDH_PMG_2_P_E_set ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_P_E_get ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sdh_pmg_field_SDH_PMG_1_P_E_set ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_P_E_get ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   cbrc_sdh_pmg_field_SDH_PMG_0_P_E_set ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_P_E_get ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _CBRC_SDH_PMG_IO_H
#define _CBRC_SDH_PMG_IO_H

#include "cbrc_api.h"
#include "cbrc_sdh_pmg_regs.h"
#include "cbrc_sdh_pmg_io_inline.h"


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 cbrc_sdh_pmg_buffer_init               ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void                 cbrc_sdh_pmg_buffer_flush              ( cbrc_sdh_pmg_buffer_t *b_ptr )
 | UINT32               cbrc_sdh_pmg_reg_read                  ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 cbrc_sdh_pmg_reg_write                 ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 cbrc_sdh_pmg_field_set                 ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 cbrc_sdh_pmg_action_on_write_field_set ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 cbrc_sdh_pmg_burst_read                ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 cbrc_sdh_pmg_burst_write               ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE cbrc_sdh_pmg_poll                      ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   cbrc_sdh_pmg_reg_CFG_write            ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   cbrc_sdh_pmg_reg_CFG_field_set        ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cbrc_sdh_pmg_reg_CFG_read             ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void   cbrc_sdh_pmg_reg_INT_PIN_EN_write     ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   cbrc_sdh_pmg_reg_INT_PIN_EN_field_set ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cbrc_sdh_pmg_reg_INT_PIN_EN_read      ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 cbrc_sdh_pmg_reg_INT_SUMMARY_read     ( cbrc_sdh_pmg_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _CBRC_SDH_PMG_IO_H */
