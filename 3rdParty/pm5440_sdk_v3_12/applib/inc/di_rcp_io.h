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
 *     and register accessor functions for the di_rcp block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing di_rcp io functions is:
 | {
 |     di_rcp_buffer_t b_ptr[1];
 |     di_rcp_buffer_init( b_ptr, h_ptr );
 |     value = di_rcp_field_<FIELD>_get( b_ptr, h_ptr );
 |     di_rcp_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     di_rcp_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = di_rcp_field_<FIELD>_get( NULL, h_ptr );
 | di_rcp_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------+
 | | Register 0x00008000 REG_SW_RESET |
 | +----------------------------------+
 | | bit  1 R/W  REG_SOFT_SW_RESET    |
 | | bit  0 R/W  REG_SW_RESET         |
 | +----------------------------------+
 | 
 | void   di_rcp_field_REG_SOFT_SW_RESET_set ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 di_rcp_field_REG_SOFT_SW_RESET_get ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   di_rcp_field_REG_SW_RESET_set      ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 di_rcp_field_REG_SW_RESET_get      ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +--------------------------------+
 | | Register 0x00008004 REG_LOWPWR |
 | +--------------------------------+
 | | bit  0 R/W  REG_LOWPWR         |
 | +--------------------------------+
 | 
 | void   di_rcp_field_REG_LOWPWR_set ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 di_rcp_field_REG_LOWPWR_get ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x0000800c REG_RAM_PWR_MGMT |
 | +--------------------------------------+
 | | bit  0 R/W  REG_RAM_LOWPWR           |
 | +--------------------------------------+
 | 
 | void   di_rcp_field_REG_RAM_LOWPWR_set ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 di_rcp_field_REG_RAM_LOWPWR_get ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +---------------------------------+
 | | Register 0x00008010 IRQ_SUMMARY |
 | +---------------------------------+
 | | bit  2:0 R  INTERRUPT_SUMMARY   |
 | +---------------------------------+
 | 
 | UINT32               di_rcp_field_INTERRUPT_SUMMARY_get        ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | UINT32               di_rcp_field_range_INTERRUPT_SUMMARY_get  ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE di_rcp_field_range_INTERRUPT_SUMMARY_poll ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE di_rcp_field_INTERRUPT_SUMMARY_poll       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------+
 | | Register 0x00008014 REG_GOJF           |
 | +----------------------------------------+
 | | bit  21:16 R/W  REG_FIFO_MIN_SLOT_FREE |
 | | bit  13:8  R/W  REG_FIFO_MIN_LVL       |
 | | bit  4     R/W  REG_ETH_TYPE_IN_PDU    |
 | | bit  2     R/W  REG_ETH_RX_10B_SWIZZLE |
 | | bit  1     R/W  REG_ETH_TX_10B_SWIZZLE |
 | | bit  0     R/W  REG_ETH_TX_8B_SWIZZLE  |
 | +----------------------------------------+
 | 
 | void   di_rcp_field_REG_FIFO_MIN_SLOT_FREE_set       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 di_rcp_field_REG_FIFO_MIN_SLOT_FREE_get       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   di_rcp_field_range_REG_FIFO_MIN_SLOT_FREE_set ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 di_rcp_field_range_REG_FIFO_MIN_SLOT_FREE_get ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   di_rcp_field_REG_FIFO_MIN_LVL_set             ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 di_rcp_field_REG_FIFO_MIN_LVL_get             ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   di_rcp_field_range_REG_FIFO_MIN_LVL_set       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 di_rcp_field_range_REG_FIFO_MIN_LVL_get       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   di_rcp_field_REG_ETH_TYPE_IN_PDU_set          ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 di_rcp_field_REG_ETH_TYPE_IN_PDU_get          ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   di_rcp_field_REG_ETH_RX_10B_SWIZZLE_set       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 di_rcp_field_REG_ETH_RX_10B_SWIZZLE_get       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   di_rcp_field_REG_ETH_TX_10B_SWIZZLE_set       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 di_rcp_field_REG_ETH_TX_10B_SWIZZLE_get       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   di_rcp_field_REG_ETH_TX_8B_SWIZZLE_set        ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 di_rcp_field_REG_ETH_TX_8B_SWIZZLE_get        ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +-----------------------------------+
 | | Register 0x00008020 REG_ETH_TX_EN |
 | +-----------------------------------+
 | | bit  28:1 R/W  REG_ETH_TX_EN      |
 | +-----------------------------------+
 | 
 | void   di_rcp_field_REG_ETH_TX_EN_set       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 di_rcp_field_REG_ETH_TX_EN_get       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   di_rcp_field_range_REG_ETH_TX_EN_set ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 di_rcp_field_range_REG_ETH_TX_EN_get ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00008024 REG_ETH_MAC_DA1 |
 | +-------------------------------------+
 | | bit  31:0 R/W  REG_ETH_MAC_DA       |
 | +-------------------------------------+
 | 
 | void   di_rcp_lfield_REG_ETH_MAC_DA_set       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[2] )
 | void   di_rcp_lfield_REG_ETH_MAC_DA_get       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[2] )
 | void   di_rcp_lfield_range_REG_ETH_MAC_DA_set ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 di_rcp_lfield_range_REG_ETH_MAC_DA_get ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x0000802c REG_ETH_MAC_SA1 |
 | +-------------------------------------+
 | | bit  31:0 R/W  REG_ETH_MAC_SA       |
 | +-------------------------------------+
 | 
 | void   di_rcp_lfield_REG_ETH_MAC_SA_set       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[2] )
 | void   di_rcp_lfield_REG_ETH_MAC_SA_get       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[2] )
 | void   di_rcp_lfield_range_REG_ETH_MAC_SA_set ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 di_rcp_lfield_range_REG_ETH_MAC_SA_get ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00008034 REG_ETH_ETYPE_BASE |
 | +----------------------------------------+
 | | bit  15:0 R/W  REG_ETH_ETYPE_BASE      |
 | +----------------------------------------+
 | 
 | void   di_rcp_field_REG_ETH_ETYPE_BASE_set       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 di_rcp_field_REG_ETH_ETYPE_BASE_get       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   di_rcp_field_range_REG_ETH_ETYPE_BASE_set ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 di_rcp_field_range_REG_ETH_ETYPE_BASE_get ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00008038 REG_PMON_TX_TRIG |
 | +--------------------------------------+
 | | bit  0 R/W  REG_PMON_TX_TRIG         |
 | +--------------------------------------+
 | 
 | void   di_rcp_field_REG_PMON_TX_TRIG_set ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 di_rcp_field_REG_PMON_TX_TRIG_get ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +-----------------------------------------+
 | | Register 0x00008040 REG_STG_1_N_D_RCOHM |
 | +-----------------------------------------+
 | | bit  11:0 R/W  REG_STG_1_N_D_RCOHM      |
 | +-----------------------------------------+
 | 
 | void   di_rcp_field_REG_STG_1_N_D_RCOHM_set       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 di_rcp_field_REG_STG_1_N_D_RCOHM_get       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   di_rcp_field_range_REG_STG_1_N_D_RCOHM_set ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 di_rcp_field_range_REG_STG_1_N_D_RCOHM_get ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00008044 REG_STG_2_D_RCOHM1 |
 | +----------------------------------------+
 | | bit  31:0 R/W  REG_STG_2_D_RCOHM       |
 | +----------------------------------------+
 | 
 | void   di_rcp_lfield_REG_STG_2_D_RCOHM_set       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   di_rcp_lfield_REG_STG_2_D_RCOHM_get       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   di_rcp_lfield_range_REG_STG_2_D_RCOHM_set ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 di_rcp_lfield_range_REG_STG_2_D_RCOHM_get ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00008060 REG_GEN_IRQ_I |
 | +-----------------------------------+
 | | bit  12 R/W  DFEC_TX_DONE_I       |
 | | bit  4  R/W  DEFECTS_TIP_I        |
 | | bit  0  R/W  PMON_TX_DONE_I       |
 | +-----------------------------------+
 | 
 | void                 di_rcp_field_DFEC_TX_DONE_I_set_to_clear ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32               di_rcp_field_DFEC_TX_DONE_I_get          ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE di_rcp_field_DFEC_TX_DONE_I_poll         ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 di_rcp_field_DEFECTS_TIP_I_set_to_clear  ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32               di_rcp_field_DEFECTS_TIP_I_get           ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE di_rcp_field_DEFECTS_TIP_I_poll          ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 di_rcp_field_PMON_TX_DONE_I_set_to_clear ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32               di_rcp_field_PMON_TX_DONE_I_get          ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE di_rcp_field_PMON_TX_DONE_I_poll         ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------+
 | | Register 0x00008080 GEN_IRQ_E |
 | +-------------------------------+
 | | bit  12 R/W  DFEC_TX_DONE_E   |
 | | bit  4  R/W  DEFECTS_TIP_E    |
 | | bit  0  R/W  PMON_TX_DONE_E   |
 | +-------------------------------+
 | 
 | void   di_rcp_field_DFEC_TX_DONE_E_set ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 di_rcp_field_DFEC_TX_DONE_E_get ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   di_rcp_field_DEFECTS_TIP_E_set  ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 di_rcp_field_DEFECTS_TIP_E_get  ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   di_rcp_field_PMON_TX_DONE_E_set ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 di_rcp_field_PMON_TX_DONE_E_get ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +-----------------------------------------------+
 | | Register (0x00008100 + (N) * 4) SDH_PMON_COPY |
 | |     N[0..63]                                  |
 | +-----------------------------------------------+
 | | bit  15:0 R/W  SDH_PMON_COPY                  |
 | +-----------------------------------------------+
 | 
 | void   di_rcp_field_SDH_PMON_COPY_set       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 di_rcp_field_SDH_PMON_COPY_get       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   di_rcp_field_range_SDH_PMON_COPY_set ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 di_rcp_field_range_SDH_PMON_COPY_get ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x00008200 + (N) * 4) OTN_PMON_COPY |
 | |     N[0..1599]                                |
 | +-----------------------------------------------+
 | | bit  28:16 R/W  F_EBC                         |
 | | bit  12:0  R/W  N_EBC                         |
 | +-----------------------------------------------+
 | 
 | void   di_rcp_field_F_EBC_set       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 di_rcp_field_F_EBC_get       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   di_rcp_field_range_F_EBC_set ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 di_rcp_field_range_F_EBC_get ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   di_rcp_field_N_EBC_set       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 di_rcp_field_N_EBC_get       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   di_rcp_field_range_N_EBC_set ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 di_rcp_field_range_N_EBC_get ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x00009b00 + (N) * 4) PRBS_PMON_COPY |
 | |     N[0..47]                                   |
 | +------------------------------------------------+
 | | bit  15:0 R/W  TSE                             |
 | +------------------------------------------------+
 | 
 | void   di_rcp_field_TSE_set       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 di_rcp_field_TSE_get       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   di_rcp_field_range_TSE_set ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 di_rcp_field_range_TSE_get ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _DI_RCP_IO_H
#define _DI_RCP_IO_H

#include "odu_rcp_loc.h"
#include "di_rcp_regs.h"
#include "di_rcp_io_inline.h"


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 di_rcp_buffer_init               ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void                 di_rcp_buffer_flush              ( di_rcp_buffer_t *b_ptr )
 | UINT32               di_rcp_reg_read                  ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 di_rcp_reg_write                 ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 di_rcp_field_set                 ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 di_rcp_action_on_write_field_set ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 di_rcp_burst_read                ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 di_rcp_burst_write               ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE di_rcp_poll                      ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 di_rcp_reg_REG_SW_RESET_write                      ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 di_rcp_reg_REG_SW_RESET_field_set                  ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               di_rcp_reg_REG_SW_RESET_read                       ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 di_rcp_reg_REG_LOWPWR_write                        ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 di_rcp_reg_REG_LOWPWR_field_set                    ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               di_rcp_reg_REG_LOWPWR_read                         ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 di_rcp_reg_REG_RAM_PWR_MGMT_write                  ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 di_rcp_reg_REG_RAM_PWR_MGMT_field_set              ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               di_rcp_reg_REG_RAM_PWR_MGMT_read                   ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 di_rcp_reg_REG_GOJF_write                          ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 di_rcp_reg_REG_GOJF_field_set                      ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               di_rcp_reg_REG_GOJF_read                           ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 di_rcp_reg_REG_ETH_TX_EN_write                     ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 di_rcp_reg_REG_ETH_TX_EN_field_set                 ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               di_rcp_reg_REG_ETH_TX_EN_read                      ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 di_rcp_reg_REG_ETH_MAC_DA1_write                   ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 di_rcp_reg_REG_ETH_MAC_DA1_field_set               ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               di_rcp_reg_REG_ETH_MAC_DA1_read                    ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 di_rcp_reg_REG_ETH_MAC_DA2_write                   ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 di_rcp_reg_REG_ETH_MAC_DA2_field_set               ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               di_rcp_reg_REG_ETH_MAC_DA2_read                    ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 di_rcp_reg_REG_ETH_MAC_SA1_write                   ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 di_rcp_reg_REG_ETH_MAC_SA1_field_set               ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               di_rcp_reg_REG_ETH_MAC_SA1_read                    ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 di_rcp_reg_REG_ETH_MAC_SA2_write                   ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 di_rcp_reg_REG_ETH_MAC_SA2_field_set               ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               di_rcp_reg_REG_ETH_MAC_SA2_read                    ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 di_rcp_reg_REG_ETH_ETYPE_BASE_write                ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 di_rcp_reg_REG_ETH_ETYPE_BASE_field_set            ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               di_rcp_reg_REG_ETH_ETYPE_BASE_read                 ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 di_rcp_reg_REG_STG_1_N_D_RCOHM_write               ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 di_rcp_reg_REG_STG_1_N_D_RCOHM_field_set           ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               di_rcp_reg_REG_STG_1_N_D_RCOHM_read                ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 di_rcp_reg_REG_STG_2_D_RCOHM1_write                ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 di_rcp_reg_REG_STG_2_D_RCOHM1_field_set            ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               di_rcp_reg_REG_STG_2_D_RCOHM1_read                 ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 di_rcp_reg_REG_STG_2_D_RCOHM2_write                ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 di_rcp_reg_REG_STG_2_D_RCOHM2_field_set            ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               di_rcp_reg_REG_STG_2_D_RCOHM2_read                 ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 di_rcp_reg_REG_STG_2_D_RCOHM3_write                ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 di_rcp_reg_REG_STG_2_D_RCOHM3_field_set            ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               di_rcp_reg_REG_STG_2_D_RCOHM3_read                 ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 di_rcp_reg_REG_PMON_TX_TRIG_write                  ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 di_rcp_reg_REG_PMON_TX_TRIG_field_set              ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               di_rcp_reg_REG_PMON_TX_TRIG_read                   ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 di_rcp_reg_GEN_IRQ_E_write                         ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 di_rcp_reg_GEN_IRQ_E_field_set                     ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               di_rcp_reg_GEN_IRQ_E_read                          ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | UINT32               di_rcp_reg_IRQ_SUMMARY_read                        ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE di_rcp_reg_IRQ_SUMMARY_poll                        ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 di_rcp_reg_REG_GEN_IRQ_I_write                     ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 di_rcp_reg_REG_GEN_IRQ_I_action_on_write_field_set ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               di_rcp_reg_REG_GEN_IRQ_I_read                      ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE di_rcp_reg_REG_GEN_IRQ_I_poll                      ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 di_rcp_reg_OTN_PMON_COPY_array_write               ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 di_rcp_reg_OTN_PMON_COPY_array_field_set           ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               di_rcp_reg_OTN_PMON_COPY_array_read                ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 di_rcp_reg_PRBS_PMON_COPY_array_write              ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 di_rcp_reg_PRBS_PMON_COPY_array_field_set          ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               di_rcp_reg_PRBS_PMON_COPY_array_read               ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 di_rcp_reg_SDH_PMON_COPY_array_write               ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 di_rcp_reg_SDH_PMON_COPY_array_field_set           ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               di_rcp_reg_SDH_PMON_COPY_array_read                ( di_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _DI_RCP_IO_H */
