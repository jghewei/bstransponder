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
 *     and register accessor functions for the mpma block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing mpma io functions is:
 | {
 |     mpma_buffer_t b_ptr[1];
 |     mpma_buffer_init( b_ptr, h_ptr );
 |     value = mpma_field_<FIELD>_get( b_ptr, h_ptr );
 |     mpma_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     mpma_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = mpma_field_<FIELD>_get( NULL, h_ptr );
 | mpma_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------------+
 | | Register 0x00000000 GEN_CFG            |
 | +----------------------------------------+
 | | bit  30    R/W  BWR_IND_SEL            |
 | | bit  28    R/W  FORCE_DEFAULT_PKT_SIZE |
 | | bit  25:16 R/W  DEFAULT_PKT_SIZE       |
 | | bit  13:12 R/W  BUS_WIDTH              |
 | | bit  8     R/W  DPI_DYNAMIC_LS_EN      |
 | | bit  4     R/W  DCI_DYNAMIC_LS_EN      |
 | | bit  0     R/W  MPMA_EN                |
 | +----------------------------------------+
 | 
 | void   mpma_field_BWR_IND_SEL_set            ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | UINT32 mpma_field_BWR_IND_SEL_get            ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | void   mpma_field_FORCE_DEFAULT_PKT_SIZE_set ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | UINT32 mpma_field_FORCE_DEFAULT_PKT_SIZE_get ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | void   mpma_field_DEFAULT_PKT_SIZE_set       ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | UINT32 mpma_field_DEFAULT_PKT_SIZE_get       ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | void   mpma_field_range_DEFAULT_PKT_SIZE_set ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_field_range_DEFAULT_PKT_SIZE_get ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mpma_field_BUS_WIDTH_set              ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | UINT32 mpma_field_BUS_WIDTH_get              ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | void   mpma_field_range_BUS_WIDTH_set        ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_field_range_BUS_WIDTH_get        ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mpma_field_DPI_DYNAMIC_LS_EN_set      ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | UINT32 mpma_field_DPI_DYNAMIC_LS_EN_get      ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | void   mpma_field_DCI_DYNAMIC_LS_EN_set      ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | UINT32 mpma_field_DCI_DYNAMIC_LS_EN_get      ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | void   mpma_field_MPMA_EN_set                ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | UINT32 mpma_field_MPMA_EN_get                ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | 
 | +-------------------------------+
 | | Register 0x00000004 CSI_CFG_0 |
 | +-------------------------------+
 | | bit  28:24 R/W  CSI_SF        |
 | | bit  20:16 R/W  CSI_SD        |
 | | bit  12:8  R/W  CSI_NORMAL    |
 | | bit  4:0   R/W  CSI_FORCE_TO  |
 | +-------------------------------+
 | 
 | void   mpma_field_CSI_SF_set             ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | UINT32 mpma_field_CSI_SF_get             ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | void   mpma_field_range_CSI_SF_set       ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_field_range_CSI_SF_get       ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mpma_field_CSI_SD_set             ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | UINT32 mpma_field_CSI_SD_get             ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | void   mpma_field_range_CSI_SD_set       ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_field_range_CSI_SD_get       ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mpma_field_CSI_NORMAL_set         ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | UINT32 mpma_field_CSI_NORMAL_get         ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | void   mpma_field_range_CSI_NORMAL_set   ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_field_range_CSI_NORMAL_get   ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mpma_field_CSI_FORCE_TO_set       ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | UINT32 mpma_field_CSI_FORCE_TO_get       ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | void   mpma_field_range_CSI_FORCE_TO_set ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_field_range_CSI_FORCE_TO_get ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x00000008 CSI_CFG_1  |
 | +--------------------------------+
 | | bit  20:16 R/W  CSI_FORCE_AWAY |
 | | bit  4:0   R/W  CSI_SSF        |
 | +--------------------------------+
 | 
 | void   mpma_field_CSI_FORCE_AWAY_set       ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | UINT32 mpma_field_CSI_FORCE_AWAY_get       ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | void   mpma_field_range_CSI_FORCE_AWAY_set ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_field_range_CSI_FORCE_AWAY_get ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mpma_field_CSI_SSF_set              ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | UINT32 mpma_field_CSI_SSF_get              ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | void   mpma_field_range_CSI_SSF_set        ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_field_range_CSI_SSF_get        ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x0000000c GEN_CFG_1     |
 | +-----------------------------------+
 | | bit  5:0 R/W  PSD_FIFO_FULL_THRES |
 | +-----------------------------------+
 | 
 | void   mpma_field_PSD_FIFO_FULL_THRES_set       ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | UINT32 mpma_field_PSD_FIFO_FULL_THRES_get       ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | void   mpma_field_range_PSD_FIFO_FULL_THRES_set ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_field_range_PSD_FIFO_FULL_THRES_get ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register (0x00000030 + (N) * 0x8) CH_CFG |
 | |     N[0..95]                             |
 | +------------------------------------------+
 | | bit  30 R/W  BWR_IND                     |
 | | bit  28 R/W  FORCE_AWAY                  |
 | | bit  24 R/W  FORCE_TO                    |
 | | bit  20 R/W  DEFAULT_PKT_SIZE_EN         |
 | | bit  16 R/W  CH_EN                       |
 | +------------------------------------------+
 | 
 | void   mpma_field_BWR_IND_set             ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_field_BWR_IND_get             ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_field_FORCE_AWAY_set          ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_field_FORCE_AWAY_get          ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_field_FORCE_TO_set            ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_field_FORCE_TO_get            ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_field_DEFAULT_PKT_SIZE_EN_set ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_field_DEFAULT_PKT_SIZE_EN_get ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_field_CH_EN_set               ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_field_CH_EN_get               ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | 
 | +---------------------------------------------+
 | | Register (0x00000400 + (N) * 0x4) CFG_RAM_0 |
 | |     N[0..95]                                |
 | +---------------------------------------------+
 | | bit  25:16 R/W  PKT_SIZE                    |
 | | bit  12:8  R/W  PSD_FIFO_REFILL_THRES       |
 | | bit  4     R/W  TS_ENABLE                   |
 | | bit  1:0   R/W  MKR_MODE                    |
 | +---------------------------------------------+
 | 
 | void   mpma_field_PKT_SIZE_set                    ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_field_PKT_SIZE_get                    ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_field_range_PKT_SIZE_set              ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_field_range_PKT_SIZE_get              ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mpma_field_PSD_FIFO_REFILL_THRES_set       ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_field_PSD_FIFO_REFILL_THRES_get       ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_field_range_PSD_FIFO_REFILL_THRES_set ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_field_range_PSD_FIFO_REFILL_THRES_get ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mpma_field_TS_ENABLE_set                   ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_field_TS_ENABLE_get                   ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_field_MKR_MODE_set                    ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpma_field_MKR_MODE_get                    ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | void   mpma_field_range_MKR_MODE_set              ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_field_range_MKR_MODE_get              ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000600 PSD_FIFO_OVR_INT_REG_0 |
 | +--------------------------------------------+
 | | bit  31:0 R/W  PSD_FIFO_OVR_I              |
 | +--------------------------------------------+
 | 
 | void                 mpma_lfield_PSD_FIFO_OVR_I_set_to_clear       ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value[3] )
 | void                 mpma_lfield_PSD_FIFO_OVR_I_get                ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value[3] )
 | void                 mpma_lfield_range_PSD_FIFO_OVR_I_set_to_clear ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               mpma_lfield_range_PSD_FIFO_OVR_I_get          ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE mpma_lfield_range_PSD_FIFO_OVR_I_poll         ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE mpma_lfield_PSD_FIFO_OVR_I_poll               ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------+
 | | Register 0x0000060c PSD_FIFO_UDR_INT_REG_0 |
 | +--------------------------------------------+
 | | bit  31:0 R/W  PSD_FIFO_UDR_I              |
 | +--------------------------------------------+
 | 
 | void                 mpma_lfield_PSD_FIFO_UDR_I_set_to_clear       ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value[3] )
 | void                 mpma_lfield_PSD_FIFO_UDR_I_get                ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value[3] )
 | void                 mpma_lfield_range_PSD_FIFO_UDR_I_set_to_clear ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               mpma_lfield_range_PSD_FIFO_UDR_I_get          ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE mpma_lfield_range_PSD_FIFO_UDR_I_poll         ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE mpma_lfield_PSD_FIFO_UDR_I_poll               ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000640 PSD_FIFO_OVR_INT_EN_REG_0 |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  PSD_FIFO_OVR_E                 |
 | +-----------------------------------------------+
 | 
 | void   mpma_lfield_PSD_FIFO_OVR_E_set       ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value[3] )
 | void   mpma_lfield_PSD_FIFO_OVR_E_get       ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value[3] )
 | void   mpma_lfield_range_PSD_FIFO_OVR_E_set ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_lfield_range_PSD_FIFO_OVR_E_get ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x0000064c PSD_FIFO_UDR_INT_EN_REG_0 |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  PSD_FIFO_UDR_E                 |
 | +-----------------------------------------------+
 | 
 | void   mpma_lfield_PSD_FIFO_UDR_E_set       ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value[3] )
 | void   mpma_lfield_PSD_FIFO_UDR_E_get       ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value[3] )
 | void   mpma_lfield_range_PSD_FIFO_UDR_E_set ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpma_lfield_range_PSD_FIFO_UDR_E_get ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _MPMA_IO_H
#define _MPMA_IO_H

#include "mpma_loc.h"
#include "mpma_regs.h"
#include "mpma_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mpma_buffer_init               ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | void                 mpma_buffer_flush              ( mpma_buffer_t *b_ptr )
 | UINT32               mpma_reg_read                  ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 mpma_reg_write                 ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 mpma_field_set                 ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 mpma_action_on_write_field_set ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mpma_burst_read                ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 mpma_burst_write               ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE mpma_poll                      ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mpma_reg_GEN_CFG_write                                    ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | void                 mpma_reg_GEN_CFG_field_set                                ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpma_reg_GEN_CFG_read                                     ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | 
 | void                 mpma_reg_CSI_CFG_0_write                                  ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | void                 mpma_reg_CSI_CFG_0_field_set                              ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpma_reg_CSI_CFG_0_read                                   ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | 
 | void                 mpma_reg_CSI_CFG_1_write                                  ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | void                 mpma_reg_CSI_CFG_1_field_set                              ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpma_reg_CSI_CFG_1_read                                   ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | 
 | void                 mpma_reg_GEN_CFG_1_write                                  ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | void                 mpma_reg_GEN_CFG_1_field_set                              ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpma_reg_GEN_CFG_1_read                                   ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | 
 | void                 mpma_reg_CH_CFG_array_write                               ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mpma_reg_CH_CFG_array_field_set                           ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpma_reg_CH_CFG_array_read                                ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mpma_reg_CFG_RAM_0_array_write                            ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mpma_reg_CFG_RAM_0_array_field_set                        ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpma_reg_CFG_RAM_0_array_read                             ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mpma_reg_PSD_FIFO_OVR_INT_EN_REG_0_write                  ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | void                 mpma_reg_PSD_FIFO_OVR_INT_EN_REG_0_field_set              ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpma_reg_PSD_FIFO_OVR_INT_EN_REG_0_read                   ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | 
 | void                 mpma_reg_PSD_FIFO_OVR_INT_EN_REG_1_write                  ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | void                 mpma_reg_PSD_FIFO_OVR_INT_EN_REG_1_field_set              ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpma_reg_PSD_FIFO_OVR_INT_EN_REG_1_read                   ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | 
 | void                 mpma_reg_PSD_FIFO_OVR_INT_EN_REG_2_write                  ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | void                 mpma_reg_PSD_FIFO_OVR_INT_EN_REG_2_field_set              ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpma_reg_PSD_FIFO_OVR_INT_EN_REG_2_read                   ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | 
 | void                 mpma_reg_PSD_FIFO_UDR_INT_EN_REG_0_write                  ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | void                 mpma_reg_PSD_FIFO_UDR_INT_EN_REG_0_field_set              ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpma_reg_PSD_FIFO_UDR_INT_EN_REG_0_read                   ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | 
 | void                 mpma_reg_PSD_FIFO_UDR_INT_EN_REG_1_write                  ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | void                 mpma_reg_PSD_FIFO_UDR_INT_EN_REG_1_field_set              ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpma_reg_PSD_FIFO_UDR_INT_EN_REG_1_read                   ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | 
 | void                 mpma_reg_PSD_FIFO_UDR_INT_EN_REG_2_write                  ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | void                 mpma_reg_PSD_FIFO_UDR_INT_EN_REG_2_field_set              ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpma_reg_PSD_FIFO_UDR_INT_EN_REG_2_read                   ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | 
 | void                 mpma_reg_PSD_FIFO_OVR_INT_REG_0_write                     ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | void                 mpma_reg_PSD_FIFO_OVR_INT_REG_0_action_on_write_field_set ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpma_reg_PSD_FIFO_OVR_INT_REG_0_read                      ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpma_reg_PSD_FIFO_OVR_INT_REG_0_poll                      ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpma_reg_PSD_FIFO_OVR_INT_REG_1_write                     ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | void                 mpma_reg_PSD_FIFO_OVR_INT_REG_1_action_on_write_field_set ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpma_reg_PSD_FIFO_OVR_INT_REG_1_read                      ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpma_reg_PSD_FIFO_OVR_INT_REG_1_poll                      ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpma_reg_PSD_FIFO_OVR_INT_REG_2_write                     ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | void                 mpma_reg_PSD_FIFO_OVR_INT_REG_2_action_on_write_field_set ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpma_reg_PSD_FIFO_OVR_INT_REG_2_read                      ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpma_reg_PSD_FIFO_OVR_INT_REG_2_poll                      ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpma_reg_PSD_FIFO_UDR_INT_REG_0_write                     ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | void                 mpma_reg_PSD_FIFO_UDR_INT_REG_0_action_on_write_field_set ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpma_reg_PSD_FIFO_UDR_INT_REG_0_read                      ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpma_reg_PSD_FIFO_UDR_INT_REG_0_poll                      ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpma_reg_PSD_FIFO_UDR_INT_REG_1_write                     ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | void                 mpma_reg_PSD_FIFO_UDR_INT_REG_1_action_on_write_field_set ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpma_reg_PSD_FIFO_UDR_INT_REG_1_read                      ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpma_reg_PSD_FIFO_UDR_INT_REG_1_poll                      ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpma_reg_PSD_FIFO_UDR_INT_REG_2_write                     ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 value )
 | void                 mpma_reg_PSD_FIFO_UDR_INT_REG_2_action_on_write_field_set ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpma_reg_PSD_FIFO_UDR_INT_REG_2_read                      ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpma_reg_PSD_FIFO_UDR_INT_REG_2_poll                      ( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _MPMA_IO_H */
