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
 *     and register accessor functions for the cbrc_fc_ilvr block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing cbrc_fc_ilvr io functions is:
 | {
 |     cbrc_fc_ilvr_buffer_t b_ptr[1];
 |     cbrc_fc_ilvr_buffer_init( b_ptr, h_ptr );
 |     value = cbrc_fc_ilvr_field_<FIELD>_get( b_ptr, h_ptr );
 |     cbrc_fc_ilvr_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     cbrc_fc_ilvr_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = cbrc_fc_ilvr_field_<FIELD>_get( NULL, h_ptr );
 | cbrc_fc_ilvr_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------------------+
 | | Register 0x00031600 ILVR_TSB_INT_ENABLE_REG0 |
 | +----------------------------------------------+
 | | bit  31:20 R/W  FC1200_PTE_IRQ_EN0           |
 | | bit  19:8  R/W  FC800_PMG_IRQ_EN0            |
 | | bit  6:4   R/W  CBRFSGM_IRQ_EN0              |
 | | bit  2     R/W  PKTINT_IRQ_EN0               |
 | | bit  1     R/W  MPMA_IRQ_EN0                 |
 | | bit  0     R/W  CBRINT_IRQ_EN0               |
 | +----------------------------------------------+
 | 
 | void   cbrc_fc_ilvr_field_FC1200_PTE_IRQ_EN0_set       ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_FC1200_PTE_IRQ_EN0_get       ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_range_FC1200_PTE_IRQ_EN0_set ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_range_FC1200_PTE_IRQ_EN0_get ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_fc_ilvr_field_FC800_PMG_IRQ_EN0_set        ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_FC800_PMG_IRQ_EN0_get        ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_range_FC800_PMG_IRQ_EN0_set  ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_range_FC800_PMG_IRQ_EN0_get  ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_fc_ilvr_field_CBRFSGM_IRQ_EN0_set          ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_CBRFSGM_IRQ_EN0_get          ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_range_CBRFSGM_IRQ_EN0_set    ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_range_CBRFSGM_IRQ_EN0_get    ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_fc_ilvr_field_PKTINT_IRQ_EN0_set           ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_PKTINT_IRQ_EN0_get           ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_MPMA_IRQ_EN0_set             ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_MPMA_IRQ_EN0_get             ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_CBRINT_IRQ_EN0_set           ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_CBRINT_IRQ_EN0_get           ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x00031604 ILVR_TSB_INT_ENABLE_REG1 |
 | +----------------------------------------------+
 | | bit  31:20 R/W  FC1200_PTE_IRQ_EN1           |
 | | bit  19:8  R/W  FC800_PMG_IRQ_EN1            |
 | | bit  6:4   R/W  CBRFSGM_IRQ_EN1              |
 | | bit  2     R/W  PKTINT_IRQ_EN1               |
 | | bit  1     R/W  MPMA_IRQ_EN1                 |
 | | bit  0     R/W  CBRINT_IRQ_EN1               |
 | +----------------------------------------------+
 | 
 | void   cbrc_fc_ilvr_field_FC1200_PTE_IRQ_EN1_set       ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_FC1200_PTE_IRQ_EN1_get       ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_range_FC1200_PTE_IRQ_EN1_set ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_range_FC1200_PTE_IRQ_EN1_get ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_fc_ilvr_field_FC800_PMG_IRQ_EN1_set        ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_FC800_PMG_IRQ_EN1_get        ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_range_FC800_PMG_IRQ_EN1_set  ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_range_FC800_PMG_IRQ_EN1_get  ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_fc_ilvr_field_CBRFSGM_IRQ_EN1_set          ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_CBRFSGM_IRQ_EN1_get          ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_range_CBRFSGM_IRQ_EN1_set    ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_range_CBRFSGM_IRQ_EN1_get    ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_fc_ilvr_field_PKTINT_IRQ_EN1_set           ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_PKTINT_IRQ_EN1_get           ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_MPMA_IRQ_EN1_set             ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_MPMA_IRQ_EN1_get             ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_CBRINT_IRQ_EN1_set           ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_CBRINT_IRQ_EN1_get           ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x00031608 ILVR_TSB_INT_ENABLE_REG2 |
 | +----------------------------------------------+
 | | bit  31:20 R/W  FC1200_PTE_IRQ_EN2           |
 | | bit  19:8  R/W  FC800_PMG_IRQ_EN2            |
 | | bit  6:4   R/W  CBRFSGM_IRQ_EN2              |
 | | bit  2     R/W  PKTINT_IRQ_EN2               |
 | | bit  1     R/W  MPMA_IRQ_EN2                 |
 | | bit  0     R/W  CBRINT_IRQ_EN2               |
 | +----------------------------------------------+
 | 
 | void   cbrc_fc_ilvr_field_FC1200_PTE_IRQ_EN2_set       ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_FC1200_PTE_IRQ_EN2_get       ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_range_FC1200_PTE_IRQ_EN2_set ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_range_FC1200_PTE_IRQ_EN2_get ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_fc_ilvr_field_FC800_PMG_IRQ_EN2_set        ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_FC800_PMG_IRQ_EN2_get        ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_range_FC800_PMG_IRQ_EN2_set  ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_range_FC800_PMG_IRQ_EN2_get  ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_fc_ilvr_field_CBRFSGM_IRQ_EN2_set          ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_CBRFSGM_IRQ_EN2_get          ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_range_CBRFSGM_IRQ_EN2_set    ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_range_CBRFSGM_IRQ_EN2_get    ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_fc_ilvr_field_PKTINT_IRQ_EN2_set           ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_PKTINT_IRQ_EN2_get           ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_MPMA_IRQ_EN2_set             ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_MPMA_IRQ_EN2_get             ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_CBRINT_IRQ_EN2_set           ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_CBRINT_IRQ_EN2_get           ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x0003160c ILVR_TSB_INT_ENABLE_REG3 |
 | +----------------------------------------------+
 | | bit  31:20 R/W  FC1200_PTE_IRQ_EN3           |
 | | bit  19:8  R/W  FC800_PMG_IRQ_EN3            |
 | | bit  6:4   R/W  CBRFSGM_IRQ_EN3              |
 | | bit  2     R/W  PKTINT_IRQ_EN3               |
 | | bit  1     R/W  MPMA_IRQ_EN3                 |
 | | bit  0     R/W  CBRINT_IRQ_EN3               |
 | +----------------------------------------------+
 | 
 | void   cbrc_fc_ilvr_field_FC1200_PTE_IRQ_EN3_set       ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_FC1200_PTE_IRQ_EN3_get       ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_range_FC1200_PTE_IRQ_EN3_set ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_range_FC1200_PTE_IRQ_EN3_get ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_fc_ilvr_field_FC800_PMG_IRQ_EN3_set        ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_FC800_PMG_IRQ_EN3_get        ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_range_FC800_PMG_IRQ_EN3_set  ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_range_FC800_PMG_IRQ_EN3_get  ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_fc_ilvr_field_CBRFSGM_IRQ_EN3_set          ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_CBRFSGM_IRQ_EN3_get          ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_range_CBRFSGM_IRQ_EN3_set    ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_range_CBRFSGM_IRQ_EN3_get    ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   cbrc_fc_ilvr_field_PKTINT_IRQ_EN3_set           ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_PKTINT_IRQ_EN3_get           ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_MPMA_IRQ_EN3_set             ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_MPMA_IRQ_EN3_get             ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_CBRINT_IRQ_EN3_set           ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_CBRINT_IRQ_EN3_get           ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00031610 + (N) * 0x4) ILVR_CHN_CONFIG_REG |
 | |     N[0..11]                                          |
 | +-------------------------------------------------------+
 | | bit  1 R/W  FC800PMG_CH_EN                            |
 | | bit  0 R/W  FCPTE_CH_EN                               |
 | +-------------------------------------------------------+
 | 
 | void   cbrc_fc_ilvr_field_FC800PMG_CH_EN_set ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_FC800PMG_CH_EN_get ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | void   cbrc_fc_ilvr_field_FCPTE_CH_EN_set    ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_FCPTE_CH_EN_get    ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | +------------------------------------------+
 | | Register 0x00031640 ILVR_GLOBAL_CTRL_REG |
 | +------------------------------------------+
 | | bit  4 R/W  PKTINT_RAM_SD_REG            |
 | | bit  2 R/W  MPMA_RAM_SD_REG              |
 | | bit  1 R/W  CBR_CH_EN                    |
 | | bit  0 R/W  ILVR_SYNC_RST                |
 | +------------------------------------------+
 | 
 | void   cbrc_fc_ilvr_field_PKTINT_RAM_SD_REG_set ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_PKTINT_RAM_SD_REG_get ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_MPMA_RAM_SD_REG_set   ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_MPMA_RAM_SD_REG_get   ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_CBR_CH_EN_set         ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_CBR_CH_EN_get         ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_ILVR_SYNC_RST_set     ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_ILVR_SYNC_RST_get     ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x00031644 GENERAL_PURPOSE_CTRL_REG |
 | +----------------------------------------------+
 | | bit  7:0 R/W  GPO_REG                        |
 | +----------------------------------------------+
 | 
 | void   cbrc_fc_ilvr_field_GPO_REG_set       ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_GPO_REG_get       ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   cbrc_fc_ilvr_field_range_GPO_REG_set ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cbrc_fc_ilvr_field_range_GPO_REG_get ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _CBRC_FC_ILVR_IO_H
#define _CBRC_FC_ILVR_IO_H

#include "cbrc_api.h"
#include "cbrc_fc_ilvr_regs.h"
#include "cbrc_fc_ilvr_io_inline.h"


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 cbrc_fc_ilvr_buffer_init               ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void                 cbrc_fc_ilvr_buffer_flush              ( cbrc_fc_ilvr_buffer_t *b_ptr )
 | UINT32               cbrc_fc_ilvr_reg_read                  ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 cbrc_fc_ilvr_reg_write                 ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 cbrc_fc_ilvr_field_set                 ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 cbrc_fc_ilvr_action_on_write_field_set ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 cbrc_fc_ilvr_burst_read                ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 cbrc_fc_ilvr_burst_write               ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE cbrc_fc_ilvr_poll                      ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   cbrc_fc_ilvr_reg_ILVR_TSB_INT_ENABLE_REG0_write      ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void   cbrc_fc_ilvr_reg_ILVR_TSB_INT_ENABLE_REG0_field_set  ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cbrc_fc_ilvr_reg_ILVR_TSB_INT_ENABLE_REG0_read       ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void   cbrc_fc_ilvr_reg_ILVR_TSB_INT_ENABLE_REG1_write      ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void   cbrc_fc_ilvr_reg_ILVR_TSB_INT_ENABLE_REG1_field_set  ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cbrc_fc_ilvr_reg_ILVR_TSB_INT_ENABLE_REG1_read       ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void   cbrc_fc_ilvr_reg_ILVR_TSB_INT_ENABLE_REG2_write      ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void   cbrc_fc_ilvr_reg_ILVR_TSB_INT_ENABLE_REG2_field_set  ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cbrc_fc_ilvr_reg_ILVR_TSB_INT_ENABLE_REG2_read       ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void   cbrc_fc_ilvr_reg_ILVR_TSB_INT_ENABLE_REG3_write      ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void   cbrc_fc_ilvr_reg_ILVR_TSB_INT_ENABLE_REG3_field_set  ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cbrc_fc_ilvr_reg_ILVR_TSB_INT_ENABLE_REG3_read       ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void   cbrc_fc_ilvr_reg_ILVR_GLOBAL_CTRL_REG_write          ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void   cbrc_fc_ilvr_reg_ILVR_GLOBAL_CTRL_REG_field_set      ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cbrc_fc_ilvr_reg_ILVR_GLOBAL_CTRL_REG_read           ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void   cbrc_fc_ilvr_reg_GENERAL_PURPOSE_CTRL_REG_write      ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void   cbrc_fc_ilvr_reg_GENERAL_PURPOSE_CTRL_REG_field_set  ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cbrc_fc_ilvr_reg_GENERAL_PURPOSE_CTRL_REG_read       ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void   cbrc_fc_ilvr_reg_ILVR_CHN_CONFIG_REG_array_write     ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   cbrc_fc_ilvr_reg_ILVR_CHN_CONFIG_REG_array_field_set ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cbrc_fc_ilvr_reg_ILVR_CHN_CONFIG_REG_array_read      ( cbrc_fc_ilvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _CBRC_FC_ILVR_IO_H */
