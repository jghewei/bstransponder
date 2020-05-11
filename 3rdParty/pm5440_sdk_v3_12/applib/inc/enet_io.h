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
 *     and register accessor functions for the enet block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing enet io functions is:
 | {
 |     enet_buffer_t b_ptr[1];
 |     enet_buffer_init( b_ptr, h_ptr );
 |     value = enet_field_<FIELD>_get( b_ptr, h_ptr );
 |     enet_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     enet_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = enet_field_<FIELD>_get( NULL, h_ptr );
 | enet_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------+
 | | Register 0x00000000 MTSB_SW_RESET |
 | +-----------------------------------+
 | | bit  4 R/W  ETRANS120_SW_RST      |
 | | bit  3 R/W  EPMM120_SW_RST        |
 | | bit  2 R/W  EMPEI120_E_SW_RST     |
 | | bit  1 R/W  EMPEI120_I_SW_RST     |
 | | bit  0 R/W  EMAC120_SW_RST        |
 | +-----------------------------------+
 | 
 | void   enet_field_ETRANS120_SW_RST_set  ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_ETRANS120_SW_RST_get  ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | void   enet_field_EPMM120_SW_RST_set    ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_EPMM120_SW_RST_get    ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | void   enet_field_EMPEI120_E_SW_RST_set ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_EMPEI120_E_SW_RST_get ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | void   enet_field_EMPEI120_I_SW_RST_set ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_EMPEI120_I_SW_RST_get ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | void   enet_field_EMAC120_SW_RST_set    ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_EMAC120_SW_RST_get    ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | 
 | +-----------------------------------------+
 | | Register 0x00000004 MTSB_LOW_POWER_MODE |
 | +-----------------------------------------+
 | | bit  12 R/W  ETRANS120_RAM_LOWPWR       |
 | | bit  11 R/W  EPMM120_RAM_LOWPWR         |
 | | bit  10 R/W  EMPEI120_E_RAM_LOWPWR      |
 | | bit  9  R/W  EMPEI120_I_RAM_LOWPWR      |
 | | bit  8  R/W  EMAC120_RAM_LOWPWR         |
 | | bit  4  R/W  ETRANS120_LOWPWR           |
 | | bit  3  R/W  EPMM120_LOWPWR             |
 | | bit  2  R/W  EMPEI120_E_LOWPWR          |
 | | bit  1  R/W  EMPEI120_I_LOWPWR          |
 | | bit  0  R/W  EMAC120_LOWPWR             |
 | +-----------------------------------------+
 | 
 | void   enet_field_ETRANS120_RAM_LOWPWR_set  ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_ETRANS120_RAM_LOWPWR_get  ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | void   enet_field_EPMM120_RAM_LOWPWR_set    ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_EPMM120_RAM_LOWPWR_get    ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | void   enet_field_EMPEI120_E_RAM_LOWPWR_set ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_EMPEI120_E_RAM_LOWPWR_get ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | void   enet_field_EMPEI120_I_RAM_LOWPWR_set ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_EMPEI120_I_RAM_LOWPWR_get ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | void   enet_field_EMAC120_RAM_LOWPWR_set    ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_EMAC120_RAM_LOWPWR_get    ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | void   enet_field_ETRANS120_LOWPWR_set      ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_ETRANS120_LOWPWR_get      ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | void   enet_field_EPMM120_LOWPWR_set        ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_EPMM120_LOWPWR_get        ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | void   enet_field_EMPEI120_E_LOWPWR_set     ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_EMPEI120_E_LOWPWR_get     ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | void   enet_field_EMPEI120_I_LOWPWR_set     ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_EMPEI120_I_LOWPWR_get     ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | void   enet_field_EMAC120_LOWPWR_set        ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_EMAC120_LOWPWR_get        ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x00000008 SUBSYSTEM_CTRL |
 | +------------------------------------+
 | | bit  2 R/W  SS_PG_OVERRIDE         |
 | | bit  1 R/W  SS_CLR_INT             |
 | | bit  0 R/W  SS_WCI_MODE            |
 | +------------------------------------+
 | 
 | void   enet_field_SS_PG_OVERRIDE_set ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_SS_PG_OVERRIDE_get ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | void   enet_field_SS_CLR_INT_set     ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_SS_CLR_INT_get     ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | void   enet_field_SS_WCI_MODE_set    ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_SS_WCI_MODE_get    ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x0000000c ENET_DATAPATH_CFG |
 | +---------------------------------------+
 | | bit  15    R/W  LINK_100G_EN          |
 | | bit  14:12 R/W  LINK_40G_EN           |
 | | bit  11:0  R/W  LINK_10G_EN           |
 | +---------------------------------------+
 | 
 | void   enet_field_LINK_100G_EN_set      ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_LINK_100G_EN_get      ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | void   enet_field_LINK_40G_EN_set       ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_LINK_40G_EN_get       ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | void   enet_field_range_LINK_40G_EN_set ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_field_range_LINK_40G_EN_get ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enet_field_LINK_10G_EN_set       ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_field_LINK_10G_EN_get       ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | void   enet_field_range_LINK_10G_EN_set ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_field_range_LINK_10G_EN_get ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000010 ENET_SS_INT_SMRY_PGE_0 |
 | +--------------------------------------------+
 | | bit  24    R  EPMM_MPMO_INT                |
 | | bit  23    R  EPMM_MPMA_INT                |
 | | bit  22    R  EPMM_EXCO4M_INT              |
 | | bit  21:19 R  EPMM_EXCO3M_INT              |
 | | bit  18:7  R  EPMM_GSUP_INT                |
 | | bit  6     R  EPMM_TOP_INT                 |
 | | bit  5     R  ECLASS_E_INT                 |
 | | bit  4     R  EMPEI_E_INT                  |
 | | bit  3     R  ECLASS_I_INT                 |
 | | bit  2     R  EMPEI_I_INT                  |
 | | bit  1     R  ETIME_INT                    |
 | | bit  0     R  EMAC_INT                     |
 | +--------------------------------------------+
 | 
 | UINT32 enet_field_EPMM_MPMO_INT_get         ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | UINT32 enet_field_EPMM_MPMA_INT_get         ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | UINT32 enet_field_EPMM_EXCO4M_INT_get       ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | UINT32 enet_field_EPMM_EXCO3M_INT_get       ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | UINT32 enet_field_range_EPMM_EXCO3M_INT_get ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 enet_field_EPMM_GSUP_INT_get         ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | UINT32 enet_field_range_EPMM_GSUP_INT_get   ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 enet_field_EPMM_TOP_INT_get          ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | UINT32 enet_field_ECLASS_E_INT_get          ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | UINT32 enet_field_EMPEI_E_INT_get           ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | UINT32 enet_field_ECLASS_I_INT_get          ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | UINT32 enet_field_EMPEI_I_INT_get           ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | UINT32 enet_field_ETIME_INT_get             ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | UINT32 enet_field_EMAC_INT_get              ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x00000014 ENET_SS_INT_SMRY_PG_1 |
 | +-------------------------------------------+
 | | bit  22    R  ETRANS_EXCO4M_INT           |
 | | bit  21:19 R  ETRANS_EXCO3M_INT           |
 | | bit  12:1  R  ETRANS_GSUP_INT             |
 | | bit  0     R  ETRANS_TOP_INT              |
 | +-------------------------------------------+
 | 
 | UINT32 enet_field_ETRANS_EXCO4M_INT_get       ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | UINT32 enet_field_ETRANS_EXCO3M_INT_get       ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | UINT32 enet_field_range_ETRANS_EXCO3M_INT_get ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 enet_field_ETRANS_GSUP_INT_get         ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | UINT32 enet_field_range_ETRANS_GSUP_INT_get   ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 enet_field_ETRANS_TOP_INT_get          ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00000020 + (N) * 8) SUBSTSEM_INT_EN_PAGE_0 |
 | |     N[0..3]                                            |
 | +--------------------------------------------------------+
 | | bit  24    R/W  EPMM_MPMO_INT_EN                       |
 | | bit  23    R/W  EPMM_MPMA_INT_EN                       |
 | | bit  22    R/W  EPMM_EXCO4M_INT_EN                     |
 | | bit  21:19 R/W  EPMM_EXCO3M_INT_EN                     |
 | | bit  18:7  R/W  EPMM_GSUP_INT_EN                       |
 | | bit  6     R/W  EPMM_TOP_INT_EN                        |
 | | bit  5     R/W  ECLASS_E_INT_EN                        |
 | | bit  4     R/W  EMPEI_E_INT_EN                         |
 | | bit  3     R/W  ECLASS_I_INT_EN                        |
 | | bit  2     R/W  EMPEI_I_INT_EN                         |
 | | bit  1     R/W  ETIME_INT_EN                           |
 | | bit  0     R/W  EMAC_INT_EN                            |
 | +--------------------------------------------------------+
 | 
 | void   enet_field_EPMM_MPMO_INT_EN_set         ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 enet_field_EPMM_MPMO_INT_EN_get         ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N )
 | void   enet_field_EPMM_MPMA_INT_EN_set         ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 enet_field_EPMM_MPMA_INT_EN_get         ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N )
 | void   enet_field_EPMM_EXCO4M_INT_EN_set       ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 enet_field_EPMM_EXCO4M_INT_EN_get       ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N )
 | void   enet_field_EPMM_EXCO3M_INT_EN_set       ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 enet_field_EPMM_EXCO3M_INT_EN_get       ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N )
 | void   enet_field_range_EPMM_EXCO3M_INT_EN_set ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_field_range_EPMM_EXCO3M_INT_EN_get ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   enet_field_EPMM_GSUP_INT_EN_set         ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 enet_field_EPMM_GSUP_INT_EN_get         ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N )
 | void   enet_field_range_EPMM_GSUP_INT_EN_set   ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_field_range_EPMM_GSUP_INT_EN_get   ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   enet_field_EPMM_TOP_INT_EN_set          ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 enet_field_EPMM_TOP_INT_EN_get          ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N )
 | void   enet_field_ECLASS_E_INT_EN_set          ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 enet_field_ECLASS_E_INT_EN_get          ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N )
 | void   enet_field_EMPEI_E_INT_EN_set           ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 enet_field_EMPEI_E_INT_EN_get           ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N )
 | void   enet_field_ECLASS_I_INT_EN_set          ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 enet_field_ECLASS_I_INT_EN_get          ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N )
 | void   enet_field_EMPEI_I_INT_EN_set           ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 enet_field_EMPEI_I_INT_EN_get           ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N )
 | void   enet_field_ETIME_INT_EN_set             ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 enet_field_ETIME_INT_EN_get             ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N )
 | void   enet_field_EMAC_INT_EN_set              ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 enet_field_EMAC_INT_EN_get              ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00000024 + (N) * 8) SUBSTSEM_INT_EN_PAGE_1 |
 | |     N[0..3]                                            |
 | +--------------------------------------------------------+
 | | bit  16    R/W  ETRANS_EXCO4M_INT_EN                   |
 | | bit  15:13 R/W  ETRANS_EXCO3M_INT_EN                   |
 | | bit  12:1  R/W  ETRANS_GSUP_INT_EN                     |
 | | bit  0     R/W  ETRANS_TOP_INT_EN                      |
 | +--------------------------------------------------------+
 | 
 | void   enet_field_ETRANS_EXCO4M_INT_EN_set       ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 enet_field_ETRANS_EXCO4M_INT_EN_get       ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N )
 | void   enet_field_ETRANS_EXCO3M_INT_EN_set       ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 enet_field_ETRANS_EXCO3M_INT_EN_get       ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N )
 | void   enet_field_range_ETRANS_EXCO3M_INT_EN_set ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_field_range_ETRANS_EXCO3M_INT_EN_get ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   enet_field_ETRANS_GSUP_INT_EN_set         ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 enet_field_ETRANS_GSUP_INT_EN_get         ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N )
 | void   enet_field_range_ETRANS_GSUP_INT_EN_set   ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_field_range_ETRANS_GSUP_INT_EN_get   ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   enet_field_ETRANS_TOP_INT_EN_set          ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 enet_field_ETRANS_TOP_INT_EN_get          ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _ENET_IO_H
#define _ENET_IO_H

#include "enet_loc.h"
#include "enet_regs.h"
#include "enet_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 enet_buffer_init               ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | void                 enet_buffer_flush              ( enet_buffer_t *b_ptr )
 | UINT32               enet_reg_read                  ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 enet_reg_write                 ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 enet_field_set                 ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 enet_action_on_write_field_set ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 enet_burst_read                ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 enet_burst_write               ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE enet_poll                      ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   enet_reg_MTSB_SW_RESET_write                    ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | void   enet_reg_MTSB_SW_RESET_field_set                ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 enet_reg_MTSB_SW_RESET_read                     ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | 
 | void   enet_reg_MTSB_LOW_POWER_MODE_write              ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | void   enet_reg_MTSB_LOW_POWER_MODE_field_set          ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 enet_reg_MTSB_LOW_POWER_MODE_read               ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | 
 | void   enet_reg_SUBSYSTEM_CTRL_write                   ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | void   enet_reg_SUBSYSTEM_CTRL_field_set               ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 enet_reg_SUBSYSTEM_CTRL_read                    ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | 
 | void   enet_reg_ENET_DATAPATH_CFG_write                ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 value )
 | void   enet_reg_ENET_DATAPATH_CFG_field_set            ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 enet_reg_ENET_DATAPATH_CFG_read                 ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | 
 | void   enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_write     ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_field_set ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_read      ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N )
 | 
 | void   enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_write     ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_field_set ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_read      ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32 enet_reg_ENET_SS_INT_SMRY_PGE_0_read            ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | 
 | UINT32 enet_reg_ENET_SS_INT_SMRY_PG_1_read             ( enet_buffer_t *b_ptr, enet_handle_t *h_ptr )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _ENET_IO_H */
